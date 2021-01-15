//
// Created by erthax on 14.01.2021.
//

#include "memory.h"

Memory::Memory() {
    this->createRegisters();
}

void Memory::createRegisters() {
    registers["a"] = new Register("a");
    registers["b"] = new Register("b");
    registers["c"] = new Register("c");
    registers["d"] = new Register("d");
    registers["e"] = new Register("e");
    registers["f"] = new Register("f");
}

Variable *Memory::getVariable(std::string tVarName) {
    checkIfDeclared(tVarName);
    return symbolTable[tVarName];
}

void Memory::checkIfDeclared(std::string tVarName) {
    if (symbolTable.find(tVarName) == symbolTable.end()) {
        throw tVarName + " was not declared";
    }
}

void Memory::declareVariable(std::string pid) {
    if (symbolTable.find(pid) != symbolTable.end()) {
        throw pid + " is already declared";
    }
    this->symbolTable[pid] = new Variable(this->memoryAddresses, pid);
    this->memoryAddresses++;
}

void Memory::declareArray(std::string tPid, uint tStart, uint tEnd) {
    if (symbolTable.find(tPid) != symbolTable.end()) {
        throw tPid + " is already declared";
    }
    this->symbolTable[tPid] = new Variable(this->memoryAddresses, tPid, tStart, tEnd);
    this->memoryAddresses += (tEnd - tStart) + 1;
}

void Memory::declareConstant(std::string tPid, uint value) {
    if (symbolTable.find(tPid) == symbolTable.end()) {
        this->symbolTable[tPid] = new Variable(this->memoryAddresses, tPid, value);
        this->memoryAddresses++;
    }
}

void Memory::freeRegisters() {
    for (auto &it : registers) {
        if (it.second->isFull()) {
            it.second->freeRegister();
            //reset(it.second);
        }
    }
}

Register *Memory::getFreeRegister() {
    for (auto &it : registers) {
        if (!(it.second->isFull())) {
            it.second->setFull(true);
            return it.second;
        }
    }
    throw "no more registers rry"; //TODO jezeli wszystkie pełne wyrzucać cos do zmiennej w pamięci? ale nie powinno tak być
}

