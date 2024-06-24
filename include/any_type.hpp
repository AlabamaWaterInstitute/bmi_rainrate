#pragma once
#ifndef ANY_TYPE_HPP
#define ANY_TYPE_HPP

#include "basic_includes.hpp"

// Lightweight anytype equivalent to allow fully dynamic and flexible variable storage
// Yes, I am aware that this is essentially C++ sacrilege, but it is incredibly convenient for this use case

class AnyType {
    public:
        std::string type;
        std::size_t size = 0;
        std::string value_str;
        AnyType() {
            this->var = nullptr;
            this->type = "";
            this->value_str = "";
            this->size = 0;
        }
        template <typename T>
        AnyType operator = (T value) {
            this->var = new T(value);
            this->type = typeid(T).name();
            std::stringstream ss;
            ss << value;
            this->value_str = ss.str();
            this->size = sizeof(value);
            // std::cout << "Type: " << this->type << std::endl;
            if (this->type == "") {
                throw std::runtime_error("Null type");
            }
            return *this;
        }
        AnyType(const AnyType& other) {/*_lnf()*/
            if (&other == nullptr) {/*_lnf()*/
                throw std::runtime_error("Null pointer" + string_trace());
            }
            // this->var = other.var;
            this->type = other.type;
            this->value_str = other.value_str;
            this->resize(other.size);
            memcpy(this->var, other.var, this->size);
            // std::cout << "Type: " << this->type << std::endl;
            if (this->type == "") {/*_lnf()*/
                throw std::runtime_error("Null type" + string_trace());
            }
            // _ex()
        }
        AnyType(AnyType* other) {/*_lnf()*/
            if (other == nullptr) {/*_lnf()*/
                throw std::runtime_error("Null pointer" + string_trace());
            }
            // this->var = other->var;
            this->type = other->type;
            this->value_str = other->value_str;
            this->resize(this->size);
            memcpy(this->var, other->var, this->size);
            // std::cout << "Type: " << this->type << std::endl;
            if (this->type == "") {/*_lnf()*/
                throw std::runtime_error("Null type" + string_trace());
            }
            // _ex()
        }
        // AnyType(AnyType&& other) {/*_lnf()*/
        //     if (&other == nullptr) {/*_lnf()*/
        //         throw std::runtime_error("Null pointer" + string_trace());
        //     }
        //     this->var = other.var;
        //     this->type = other.type;
        //     // std::cout << "Type: " << this->type << std::endl;
        //     if (this->type == "") {/*_lnf()*/
        //         throw std::runtime_error("Null type" + string_trace());
        //     }
        //     other.var = nullptr;
        //     other.type = "";
        //     // _ex()
        // }
        template <typename T>
        AnyType(T value) {/*_lnf()*/
            this->var = new T(value);
            this->type = typeid(T).name();
            std::stringstream ss;
            ss << value;
            this->value_str = ss.str();
            this->size = sizeof(value);
            // std::cout << "Type: " << this->type << std::endl;
            if (this->type == "") {/*_lnf()*/
                throw std::runtime_error("Null type");
            }
        }
        // AnyType(void* value, std::string type, std::string value_str = "") {/*_lnf()*/
        //     this->var = value;
        //     this->type = type;
        //     this->value_str = value_str;

        //     // std::cout << "Type: " << this->type << std::endl;
        //     if (this->type == "") {/*_lnf()*/
        //         throw std::runtime_error("Null type" + string_trace());
        //     }
        // }
        ~AnyType() {/*_lnf()*/
            // if (this->var) {/*_lnf()*/
            //     switch (this->type[0]) {/*_lnf()*/
            //         case 'i':
            //             delete static_cast<int*>(this->var);
            //             break;
            //         case 'f':
            //             delete static_cast<float*>(this->var);
            //             break;
            //         case 'd':
            //             delete static_cast<double*>(this->var);
            //             break;
            //         case 's':
            //             delete static_cast<short*>(this->var);
            //             break;
            //         case 'l':
            //             delete static_cast<long*>(this->var);
            //             break;
            //         default:
            //             // std::string error = "Unknown type: ";
            //             // error += this->type;
            //             // We don't know what it is, so we ignore it
            //             break;
            //     }
            // }
            free(this->var);
        }
        void resize(std::size_t size) {/*_lnf()*/
            if (this->size == 0) {
                this->var = malloc(size);
            }
            else if (this->size == size) {
                return;
            } else {
                // void* temp = malloc(this->size);
                // memcpy(temp, this->var, this->size);
                this->var = realloc(this->var, size);
                // memcpy(this->var, temp, this->size);
                // free(temp);
            }
            this->size = size;
        }
        void* get_ptr() {/*_lnf()*/
            return std::move(this->var);
        }
        template <typename T>
        T* get_var() {/*_lnf()*/
            return static_cast<T*>(this->var);
        }
        template <typename T>
        T get_value() {/*_lnf()*/
            return *static_cast<T*>(this->var);
        }
        template <typename T>
        void set_value(T value) {/*_lnf()*/
            *static_cast<T*>(this->var) = value;
        }
        operator bool() {/*_lnf()*/
            return this->var != nullptr;
        }
        operator void*() {/*_lnf()*/
            return this->var;
        }
        AnyType operator = (const AnyType& other) {/*_lnf()*/
            // if (!this->safe_cast(other)&&this->type!="") {/*_lnf()*/
            //     std::string msg = "Invalid cast: " + this->type + " to " + other.type;
            //     std::string caller = string_trace();
            //     throw std::runtime_error(msg + "\n" + caller);
            // }
            // this->var = other.var;
            this->type = other.type;
            this->value_str = other.value_str;
            this->resize(other.size);
            memcpy(this->var, other.var, this->size);
            return *this;
        }
        AnyType* operator = (AnyType* other) {/*_lnf()*/
            // if (!this->safe_cast(other)&&this->type!="") {/*_lnf()*/
            //     std::string msg = "Invalid cast: " + this->type + " to " + other->type;
            //     std::string caller = string_trace();
            //     throw std::runtime_error(msg + "\n" + caller);
            // }
            // this->var = other->var;
            this->type = other->type;
            this->value_str = other->value_str;
            this->resize(other->size);
            memcpy(this->var, other->var, this->size);
            return this;
        }
        template <typename T>
        operator T() {/*_lnf()*/
            if (!this->safe_cast<T>()) {/*_lnf()*/
                std::string msg = "Invalid cast: " + this->type + " to " + typeid(T).name();
                std::string caller = string_trace();
                throw std::runtime_error(msg + "\n" + caller);
            }
            return *static_cast<T*>(this->var);
        }
        bool safe_cast(const AnyType& other) {/*_lnf()*/
            return this->type == other.type;
        }
        template <typename T>
        bool safe_cast(const T& value) {/*_lnf()*/
            return this->type == typeid(T).name();
        }
        template <typename T>
        bool safe_cast() {/*_lnf()*/
            return this->type == typeid(T).name();
        }
        bool operator == (const AnyType& other) {/*_lnf()*/
            return this->type == other.type && this->var == other.var;
        }
        template <typename T>
        bool operator == (const T& other) {/*_lnf()*/
            return this->type == typeid(T).name() && *static_cast<T*>(this->var) == other;
        }
        operator std::string() {/*_lnf()*/
            return this->cast_to_string();
        }
        std::string to_string() {/*_lnf()*/
            return this->cast_to_string();
        }
        void overwrite(const AnyType& other) {/*_lnf()*/
            if (!this->safe_cast(other)) {/*_lnf()*/
                std::string msg = "Invalid cast: " + this->type + " to " + other.type;
                std::string caller = string_trace();
                throw std::runtime_error(msg + "\n" + caller);
            }
            this->resize(other.size);
            memcpy(this->var, other.var, this->size);
            this->type = other.type;
            this->value_str = other.value_str;
        }
        std::string cast_to_string() {/*_lnf()*/
            switch (this->type[0]) {/*_lnf()*/
                case 'i':
                    return std::to_string(*static_cast<int*>(this->var));
                case 'f':
                    return std::to_string(*static_cast<float*>(this->var));
                case 'd':
                    return std::to_string(*static_cast<double*>(this->var));
                case 's':
                    return std::to_string(*static_cast<short*>(this->var));
                case 'l':
                    return std::to_string(*static_cast<long*>(this->var));
                default:
                    return "Unknown type";
            }
        }
        
    private:
        void* var = nullptr;
};







#endif