#pragma once
#ifndef ANY_TYPE_HPP
#define ANY_TYPE_HPP

#include "basic_includes.hpp"

// Lightweight anytype equivalent to allow fully dynamic and flexible variable storage
// Yes, I am aware that this is essentially C++ sacrilege, but it is incredibly convenient for this use case


// Clean version of AnyType using the design laid out in any_type_doc.py
#ifdef DEBUG_0
    #define template_log(msg) std::string log_msg = msg;\
            log_msg += typeid(T).name();\
            log_msg += " from ";\
            log_msg += std::to_string(uuid);\
            _log(log_msg.c_str());
    #define member_log(msg) std::string log_msg = msg;\
            log_msg += this->type;\
            log_msg += " from ";\
            log_msg += std::to_string(this->uuid);\
            _log(log_msg.c_str());
    #define static_log(msg) std::string log_msg = msg;\
            log_msg += self->type;\
            log_msg += " from ";\
            log_msg += std::to_string(self->uuid);\
            _log(log_msg.c_str());
#else
    #define template_log(msg)
    #define member_log(msg)
    #define static_log(msg)
#endif

extern std::map<std::string, std::map<std::string, int>> anytype_allocs;
extern size_t anytype_uuid;

inline void anytype_alloc(std::string type, std::string call_type) {
    /*
    e.g.
    {
        "i": {
            "malloc": 5,
            "free": 3
            }
    }
    */
    if (anytype_allocs.find(type) == anytype_allocs.end()) {
        anytype_allocs[type] = {
            {"malloc", 0},
            {"free", 0}
        };
    }
    if (anytype_allocs[type].find(call_type) == anytype_allocs[type].end()) {
        anytype_allocs[type][call_type] = 0;
    }
    anytype_allocs[type][call_type]++;
    
    //check for double free
    if (anytype_allocs[type]["free"] > anytype_allocs[type]["malloc"]) {
        std::string msg = "Double free of type: " + type;
        throw std::runtime_error(msg + string_trace());
    }
}

class AnyType {
    public:
        std::string type;
        void* var;
        void* (*_get_val)(const AnyType*);
        void* (*_get_copy)(const AnyType*);
        void (*_set_val)(AnyType*, void*);
        void (*_destroy)(AnyType*);
        std::string (*_to_string)(const AnyType*);
        int uuid;

        void* get_ptr() {
            return this->var;
        }
        
        template <typename T>
        static void* _get(const AnyType* self) {
            int uuid = self->uuid;
            static_log("Getting value of type: ");
            return static_cast<T*>(self->var);
        }

        template <typename T>
        static void* _copy(const AnyType* self) {
            int uuid = self->uuid;
            static_log("Getting copy of value of type: ");
            return new T(*static_cast<T*>(self->var));
        }

        template <typename T>
        static void _set(AnyType* self, void* value) {
            int uuid = self->uuid;
            static_log("Setting value of type: ");
            *static_cast<T*>(self->var) = *static_cast<T*>(value);
        }

        template <typename T>
        static void _del(AnyType* self) {
            int uuid = self->uuid;
            static_log("Deleting value of type: ");
            if (self->var == nullptr) {
                return;
            }
            delete static_cast<T*>(self->var);
            self->var = nullptr;
        }

        // _to_str needs to have specialized definition for std::string
        // as to_string will break for std::string
        // putting the definitions at the bottom of the file

        template <typename T>
        static std::string _to_str(const AnyType* self);

        void* get_val() {
            return this->_get_val(this);
        }

        void* get_copy() {
            return this->_get_copy(this);
        }

        void set_val(void* value) {
            this->_set_val(this, value);
        }

        template <typename T>
        void set_val(T value) {
            *static_cast<T*>(this->var) = value;
        }

        void destroy() {
            anytype_alloc(this->type, "free");
            this->_destroy(this);
        }

        std::string to_string() {
            return this->_to_string(this);
        }

        AnyType() {
            this->var = nullptr;
            this->type = "";
            this->_get_val = nullptr;
            this->_get_copy = nullptr;
            this->_set_val = nullptr;
            this->_destroy = nullptr;
            this->_to_string = nullptr;
            this->uuid = anytype_uuid++;
        }

        template <typename T>
        AnyType(T value) {
            this->uuid = anytype_uuid++;
            template_log("Creating value of type: ");
            this->var = new T(value);
            this->type = typeid(T).name();
            this->_get_val = this->_get<T>;
            this->_get_copy = this->_copy<T>;
            this->_set_val = this->_set<T>;
            this->_destroy = this->_del<T>;
            this->_to_string = this->_to_str<T>;
            anytype_alloc(this->type, "malloc");
        }

        static void transfer(AnyType* self, const AnyType* other) {
            self->type = other->type;
            self->var = other->_get_copy(other);
            self->_get_val = other->_get_val;
            self->_get_copy = other->_get_copy;
            self->_set_val = other->_set_val;
            self->_destroy = other->_destroy;
            self->_to_string = other->_to_string;
        }

        ~AnyType() {
            this->destroy();
        }

        AnyType(const AnyType& other) {
            if (&other == nullptr) {
                throw std::runtime_error("Null pointer" + string_trace());
            }
            this->transfer(this, &other);
            this->uuid = anytype_uuid++;
            member_log("Copy[0]ing value of type: ");
            anytype_alloc(this->type, "malloc");
            
        }

        AnyType(AnyType* other) {
            if (other == nullptr) {
                throw std::runtime_error("Null pointer" + string_trace());
            }
            this->transfer(this, other);
            this->uuid = anytype_uuid++;
            member_log("Copy[1]ing value of type: ");
            anytype_alloc(this->type, "malloc");
            
        }

        AnyType operator = (const AnyType& other) {
            this->transfer(this, &other);
            return *this;
        }

        AnyType* operator = (AnyType* other) {
            this->transfer(this, other);
            return this;
        }

        template <typename T>
        AnyType operator = (T value) {
            template_log("Setting value of type: ");
            if (this->var != nullptr) {
                this->destroy();
            }
            this->var = new T(value);
            this->type = typeid(T).name();
            this->_get_val = this->_get<T>;
            this->_get_copy = this->_copy<T>;
            this->_set_val = this->_set<T>;
            this->_destroy = this->_del<T>;
            this->_to_string = this->_to_str<T>;
            anytype_alloc(this->type, "malloc");
            return *this;
        }

        template <typename T>
        operator T() {
            template_log("Casting value of type: ");
            return *static_cast<T*>(this->var);
        }

        operator std::string() {
            return this->to_string();
        }

        bool operator == (const AnyType& other) {
            return this->type == other.type && this->var == other.var;
        }

        template <typename T>
        bool operator == (const T& other) {
            template_log("Comparing value of type: ");
            return this->type == typeid(T).name() && *static_cast<T*>(this->var) == other;
        }

        bool safe_cast(const AnyType& other) {
            return this->type == other.type;
        }

        template <typename T>
        bool safe_cast(const T& value) {
            template_log("Checking cast of type: ");
            return this->type == typeid(T).name();
        }

        template <typename T>
        bool safe_cast() {
            template_log("Checking cast of type: ");
            return this->type == typeid(T).name();
        }

};

template <>
inline std::string AnyType::_to_str<std::string>(const AnyType* self) {
    static_log("Converting value of type: ");
    return *static_cast<std::string*>(self->var);
}

template <typename T>
inline std::string AnyType::_to_str(const AnyType* self) {
    int uuid = self->uuid;
    template_log("Converting value of type: ");
    return std::to_string(*static_cast<T*>(self->var));
}

#endif