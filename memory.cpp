//
// Created by erthax on 14.01.2021.
//

#include "memory.h"

Memory::Memory() {

}

void Memory::create_registers() {

}

void Memory::declare_variable(std::string tPid) {
    if (symbol_table.find(tPid) != symbol_table.end()) {
        throw tPid + " is already declared";
    }
    this->symbol_table[tPid] = new Variable(this->memoryAddresses, tPid);
    this->memoryAddresses++;
}

void Memory::declare_array(std::string tPid, uint tStart, uint tEnd) {
    if (symbol_table.find(tPid) != symbol_table.end()) {
        throw tPid + " is already declared";
    }
    this->symbol_table[tPid] = new Variable(this->memoryAddresses, tPid, tStart, tEnd);
    this->memoryAddresses += (tEnd - tStart) + 1;
}
