//
// Created by erthax on 14.01.2021.
//

#ifndef CULTIVATED_COMPILER_MEMORY_H
#define CULTIVATED_COMPILER_MEMORY_H

#include <string>
#include <map>

#include "variable.h"


class Memory {
public:
    Memory();

    void declareVariable(std::string pid);

    void declareArray(std::string pid, uint start, uint end);


private:
    void createRegisters();

    uint memoryAddresses{0};
    std::map<std::string, Variable *> symbolTable;
};


#endif //CULTIVATED_COMPILER_MEMORY_H
