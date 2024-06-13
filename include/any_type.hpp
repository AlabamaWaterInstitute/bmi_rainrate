#pragma once
#ifndef ANY_TYPE_HPP
#define ANY_TYPE_HPP

#include "basic_includes.hpp"

// Lightweight anytype equivalent to allow fully dynamic and flexible variable storage
// Yes, I am aware that this is essentially C++ sacrilege, but it is incredibly convenient for this use case

class AnyType {
    public:
        std::string type;
        AnyType() {}
        template <typename T>
        AnyType(T value) {
            this->var = new T(value);
            this->type = typeid(T).name();
        }
        AnyType(void* value, std::string type) {
            this->var = value;
            this->type = type;
        }
        AnyType(const AnyType& other) {
            this->var = other.var;
            this->type = other.type;
        }
        ~AnyType() {
            // if (this->var) {
            //     switch (this->type[0]) {
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
        }
        void* get_ptr() {
            return std::move(this->var);
        }
        template <typename T>
        T* get_var() {
            return static_cast<T*>(this->var);
        }
        template <typename T>
        T get_value() {
            return *static_cast<T*>(this->var);
        }
        template <typename T>
        void set_value(T value) {
            *static_cast<T*>(this->var) = value;
        }
        operator bool() {
            return this->var != nullptr;
        }
        operator void*() {
            return this->var;
        }
        AnyType operator = (const AnyType& other) {
            this->var = other.var;
            return *this;
        }
        AnyType* operator = (AnyType* other) {
            this->var = other->var;
            return this;
        }
        template <typename T>
        operator T() {
            return *static_cast<T*>(this->var);
        }
        template <typename T>
        bool safe_cast(const T& value) {
            return this->type == typeid(T).name();
        }
    private:
        void* var = nullptr;
};







#endif