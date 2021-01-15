//
// Created by erthax on 14.01.2021.
//

#ifndef CULTIVATED_COMPILER_MEMORY_H
#define CULTIVATED_COMPILER_MEMORY_H

#include <string>
#include <map>
#include <utility>

#include "variable.h"

class Register {
public:
    explicit Register(std::string tName) : name(std::move(tName)) { this->full = false; }

    void freeRegister() { this->full = false; }

    void fillRegister() { this->full = true; }

    std::string getName() { return this->name; }

    bool isFull() const { return full; }

    void setName(const std::string &tName) { Register::name = tName; }

    void setFull(bool tFull) { Register::full = tFull; }

private:
    std::string name;
    bool full = false;
};

class Memory {
public:
    Memory();

    void declareVariable(std::string pid);

    void declareArray(std::string pid, uint start, uint end);

    void freeRegisters();

    Register *getFreeRegister();


private:
    void createRegisters();

    std::map<std::string, Register *> registers;
    uint memoryAddresses{0};
    std::map<std::string, Variable *> symbolTable;
};


#endif //CULTIVATED_COMPILER_MEMORY_H
