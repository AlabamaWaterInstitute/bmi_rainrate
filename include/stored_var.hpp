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
    void* get_ptr() {/*_lnf()*/ return std::move(this->var); }
    T* get_var() {/*_lnf()*/ return this->var; }
    T get_value() {/*_lnf()*/ return *this->var; }
    void set_value(T value) {/*_lnf()*/ *this->var = value; }
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
    std::string setup_info;
    // #define _si() fprintf(stderr, "%s\n", this->setup_info.c_str());
    StoredVar(AnyType* var, std::string name, std::string type, std::string units, std::string location, int item_count, int grid):
        var(var), name(name), type(type), units(units), location(location), item_count(item_count), grid(grid) {}
    StoredVar(AnyType var, std::string name, std::string type, std::string units, std::string location, int item_count, int grid):
        name(name), type(type), units(units), location(location), item_count(item_count), grid(grid) {
        this->var = new AnyType(var);
    }
    ~StoredVar() {
        delete this->var;
    }
    StoredVar* setup(std::string setup_info) {
        this->setup_info = setup_info;
        return this;
    }
    void* get_ptr() {/*_lnf() _si()*/ return this->var->get_ptr(); }
    template <typename T>
    T get_value() {/*_lnf() _si()*/ return this->var->get_value<T>(); }
    template <typename T>
    T* get_var() {/*_lnf() _si()*/ return this->var->get_var<T>(); }
    template <typename T>
    void set_value(T value) {/*_lnf() _si()*/ this->var->set_value(value); }

    AnyType* get_var() {/*_lnf() _si()*/ return this->var; }
    AnyType get_value() {/*_lnf() _si()*/ return *this->var; }
    void set_value(AnyType value) {/*_lnf() _si()*/ *this->var = value; }
    
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