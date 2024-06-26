#pragma once
#ifndef STORED_VAR_HPP
#define STORED_VAR_HPP

#include "basic_includes.hpp"
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
    void* get_ptr() { return this->var->get_ptr(); }
    template <typename T>
    T get_value() { return *this->var->get_val(); }
    template <typename T>
    T* get_var() { return this->var->get_val(); }
    template <typename T>
    void set_value(T value) { this->var->set_val(value); }

    AnyType* get_var() { return this->var; }
    AnyType get_value() { return *this->var; }
    void set_value(AnyType value) { *this->var = value; }
    
};

struct ShiftQueue {
    std::string name;
    std::vector<AnyType> queue;
    AnyType init_val;
    int max_size;
    ShiftQueue(std::string name, AnyType init_val, int max_size): name(name), init_val(init_val), max_size(max_size) {
        max_size = std::max(max_size, 1); // Ensure max_size is at least 1 (no zero-size queues
        for (int i = 0; i < max_size; i++) {
            this->queue.push_back(init_val);
        }
    }
    template <typename T>
    ShiftQueue(std::string name, T init_val, int max_size): name(name), max_size(max_size) {
        max_size = std::max(max_size, 1); // Ensure max_size is at least 1 (no zero-size queues
        this->init_val = AnyType(init_val);
        for (int i = 0; i < max_size; i++) {
            this->queue.push_back(this->init_val);
        }
    }
    AnyType& operator [] (int index) {
        if (index < 0) {
            index = this->max_size + index;
        }
        if (index < 0 || index >= this->max_size) {
            return this->init_val;
        }
        return this->queue[index];
    }
    AnyType get() {
        return (*this)[0];
    }
    void shift() {
        for (int i = 0; i < this->max_size - 1; i++) {
            this->queue[i].set_val(this->queue[i + 1].get_val());
        }
        this->queue[this->max_size - 1].set_val(this->init_val.get_val());
    }
    void push(AnyType val) {
        std::string prev = (*this)[-1].to_string();
        this->shift();
        this->queue[this->max_size - 1].set_val(val.get_val());
    }
    operator std::string () {
        std::string out = this->name + ": ";
        for (int i = 0; i < this->max_size; i++) {
            out += this->queue[i].to_string() + ", ";
        }
        return out;
    }
};

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