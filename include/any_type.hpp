#pragma once
#ifndef ANY_TYPE_HPP
#define ANY_TYPE_HPP

#include "basic_includes.hpp"

// Lightweight anytype equivalent to allow fully dynamic and flexible variable storage
// Yes, I am aware that this is essentially C++ sacrilege, but it is incredibly convenient for this use case


// Clean version of AnyType using the design laid out in any_type_doc.py

class AnyType {
    public:
        std::string type;
        void* var;
        void* (*_get_val)(const AnyType*);
        void* (*_get_copy)(const AnyType*);
        void (*_set_val)(AnyType*, void*);
        void (*_destroy)(AnyType*);
        std::string (*_to_string)(const AnyType*);

        void* get_ptr() {
            return this->var;
        }
        
        template <typename T>
        static void* _get(const AnyType* self) {
            return static_cast<T*>(self->var);
        }

        template <typename T>
        static void* _copy(const AnyType* self) {
            return new T(*static_cast<T*>(self->var));
        }

        template <typename T>
        static void _set(AnyType* self, void* value) {
            *static_cast<T*>(self->var) = *static_cast<T*>(value);
        }

        template <typename T>
        static void _del(AnyType* self) {
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
        }

        template <typename T>
        AnyType(T value) {
            this->var = new T(value);
            this->type = typeid(T).name();
            this->_get_val = this->_get<T>;
            this->_get_copy = this->_copy<T>;
            this->_set_val = this->_set<T>;
            this->_destroy = this->_del<T>;
            this->_to_string = this->_to_str<T>;
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
            
        }

        AnyType(AnyType* other) {
            if (other == nullptr) {
                throw std::runtime_error("Null pointer" + string_trace());
            }
            this->transfer(this, other);
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
            return *this;
        }

        template <typename T>
        operator T() {
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
            return this->type == typeid(T).name() && *static_cast<T*>(this->var) == other;
        }

        bool safe_cast(const AnyType& other) {
            return this->type == other.type;
        }

        template <typename T>
        bool safe_cast(const T& value) {
            return this->type == typeid(T).name();
        }

        template <typename T>
        bool safe_cast() {
            return this->type == typeid(T).name();
        }

};

template <>
inline std::string AnyType::_to_str<std::string>(const AnyType* self) {
    return *static_cast<std::string*>(self->var);
}

template <typename T>
inline std::string AnyType::_to_str(const AnyType* self) {
    return std::to_string(*static_cast<T*>(self->var));
}

#endif