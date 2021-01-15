//
// Created by erthax on 14.01.2021.
//

#include "memory.h"

Memory::Memory() {

}

void Memory::createRegisters() {

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
