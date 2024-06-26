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

template <typename T>
struct ShiftQueue {
    std::string name;
    std::vector<T*> queue;
    T* init_val;
    int max_size;
    ShiftQueue(std::string name, T* init_val, int max_size): name(name), init_val(init_val), max_size(max_size) {
        for (int i = 0; i < max_size; i++) {
            this->queue.push_back(init_val);
        }
    }
    T*& operator [] (int index) {
        if (index < 0) {
            index = this->max_size + index;
        }
        if (index < 0 || index >= this->max_size) {
            return this->init_val;
        }
        return this->queue[index];
    }
    T get() {
        return (*this)[0];
    }
    void shift() {
        for (int i = 0; i < this->max_size - 1; i++) {
            this->queue[i] = this->queue[i + 1];
        }
        (*this)[-1] = this->init_val;
    }
    void push(T* val) {
        this->shift();
        (*this)[-1] = val;
    }
    void push(T val) {
        this->shift();
        (*this)[-1] = new T(val);
    }
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
    T get_value() {/*_lnf() _si()*/ return *this->var->get_val(); }
    template <typename T>
    T* get_var() {/*_lnf() _si()*/ return this->var->get_val(); }
    template <typename T>
    void set_value(T value) {/*_lnf() _si()*/ this->var->set_val(value); }

    AnyType* get_var() {/*_lnf() _si()*/ return this->var; }
    AnyType get_value() {/*_lnf() _si()*/ return *this->var; }
    void set_value(AnyType value) {/*_lnf() _si()*/ *this->var = value; }
    
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
    void shift(bool verbose = false) {
        std::string before = std::string(*this);
        // std::vector<AnyType> new_queue;
        for (int i = 0; i < this->max_size - 1; i++) {
            this->queue[i].set_val(this->queue[i + 1].get_val());
        }
        this->queue[this->max_size - 1].set_val(this->init_val.get_val());
        std::string after = std::string(*this);
        if (verbose&&after!=before) {
            std::cout << "Shifted " << this->name << " from " << before << " to " << after << std::endl;
        }
    }
    // void push(AnyType val, bool verbose = false) {
    //     auto prev = (*this)[-1];
    //     this->shift(verbose);
    //     this->queue.push_back(val);
    //     if (verbose) {
    //     }
    // }
    void push(AnyType val, bool verbose = false) {
        std::string prev = (*this)[-1].to_string();
        
        this->shift(verbose);
        // this->queue.push_back(AnyType(val));
        this->queue[this->max_size - 1].set_val(val.get_val());
        if (verbose&&prev!=val.to_string()) {
            std::cout << "Pushed " << val.to_string() << " to " << this->name << " from " << prev << std::endl;
        }
    }
    operator std::string () {
        std::string out = this->name + ": ";
        for (int i = 0; i < this->max_size; i++) {
            out += this->queue[i].to_string() + ", ";
        }
        return out;
    }
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