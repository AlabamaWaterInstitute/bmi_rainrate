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
        AnyType(T value) {/*_lnf()*/
            this->var = new T(value);
            this->type = typeid(T).name();
        }
        AnyType(void* value, std::string type) {/*_lnf()*/
            this->var = value;
            this->type = type;
        }
        AnyType(const AnyType& other) {/*_lnf()*/
            if (&other == nullptr) {/*_lnf()*/
                throw std::runtime_error("Null pointer");
            }
            this->var = other.var;
            this->type = other.type;
            // _ex()
        }
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
            this->var = other.var;
            return *this;
        }
        AnyType* operator = (AnyType* other) {/*_lnf()*/
            this->var = other->var;
            return this;
        }
        template <typename T>
        operator T() {/*_lnf()*/
            if (!this->safe_cast<T>()) {/*_lnf()*/
                throw std::runtime_error("Invalid cast: " + this->type + " to " + typeid(T).name());
            }
            return *static_cast<T*>(this->var);
        }
        template <typename T>
        bool safe_cast(const T& value) {/*_lnf()*/
            return this->type == typeid(T).name();
        }
        template <typename T>
        bool safe_cast() {/*_lnf()*/
            return this->type == typeid(T).name();
        }
    private:
        void* var = nullptr;
};







#endif