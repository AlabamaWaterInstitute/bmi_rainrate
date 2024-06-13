#pragma once
#ifndef STORED_VAR_HPP
#define STORED_VAR_HPP

#include "basic_includes.hpp"

#define USE_ANY_TYPE 1

#ifndef USE_ANY_TYPE


template <typename T>
struct StoredVar {
    T* var;
    std::string name;
    std::string type;
    std::string units;
    std::string location;
    int item_count;
    int grid;
    StoredVar(T* var, std::string name, std::string type, std::string units, std::string location, int item_count, int grid):
        var(var), name(name), type(type), units(units), location(location), item_count(item_count), grid(grid) {}
    StoredVar(T var, std::string name, std::string type, std::string units, std::string location, int item_count, int grid):
        name(name), type(type), units(units), location(location), item_count(item_count), grid(grid) {
        this->var = new T(var);
    }
    ~StoredVar() {
        delete this->var;
    }
    void* get_ptr() { return std::move(this->var); }
    T* get_var() { return this->var; }
    T get_value() { return *this->var; }
    void set_value(T value) { *this->var = value; }
};

#else
#include "any_type.hpp"
// Alternative to the above, using AnyType

struct StoredVar {
    AnyType* var;
    std::string name;
    std::string type;
    std::string units;
    std::string location;
    int item_count;
    int grid;
    StoredVar(AnyType* var, std::string name, std::string type, std::string units, std::string location, int item_count, int grid):
        var(var), name(name), type(type), units(units), location(location), item_count(item_count), grid(grid) {}
    StoredVar(AnyType var, std::string name, std::string type, std::string units, std::string location, int item_count, int grid):
        name(name), type(type), units(units), location(location), item_count(item_count), grid(grid) {
        this->var = new AnyType(var);
    }
    ~StoredVar() {
        delete this->var;
    }
    void* get_ptr() { return std::move(this->var); }
    AnyType* get_var() { return this->var; }
    AnyType get_value() { return *this->var; }
    void set_value(AnyType value) { *this->var = value; }
    template <typename T>
    T* get_var() { return this->var->get_var<T>(); }
    template <typename T>
    void set_value(T value) { this->var->set_value(value); }
};
#endif

struct VarList : public std::vector<StoredVar*> {
    VarList() {}
    VarList(StoredVar* var) {
        this->push_back(var);
    }
    VarList(StoredVar var) {
        this->push_back(new StoredVar(var));
    }
    VarList(std::vector<StoredVar*> vars) {
        for (auto var : vars) {
            this->push_back(var);
        }
    }
    VarList(std::vector<StoredVar*> *vars) {
        for (auto var : *vars) {
            this->push_back(var);
        }
    }
    VarList(VarList* other) {
        for (auto var : *other) {
            this->push_back(var);
        }
    }
    // Need to be able to convert from brace-enclosed lists
    VarList(std::initializer_list<StoredVar*> vars) {
        for (auto var : vars) {
            this->push_back(var);
        }
    }
    void extend(VarList* other) {
        for (auto var : *other) {
            this->push_back(var);
        }
    }
    void extend(VarList other) {
        for (auto var : other) {
            this->push_back(var);
        }
    }
    void extend(std::vector<StoredVar*> other) {
        for (auto var : other) {
            this->push_back(var);
        }
    }
    void extend(std::vector<StoredVar*> *other) {
        for (auto var : *other) {
            this->push_back(var);
        }
    }
    
};

#endif