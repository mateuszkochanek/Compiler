//
// Created by erthax on 14.01.2021.
//

#ifndef CULTIVATED_COMPILER_MEMORY_H
#define CULTIVATED_COMPILER_MEMORY_H

#include <map>

#include "variable.h"


class Memory {
public:
    Memory();
    void declare_variable(std::string pid);
    void declare_array(std::string pid, uint start, uint end);


private:
    void create_registers();
    uint memoryAddresses{0};
    std::map <std::string, Variable*> symbol_table;
};


#endif //CULTIVATED_COMPILER_MEMORY_H
