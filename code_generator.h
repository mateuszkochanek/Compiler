//
// Created by erthax on 15.01.2021.
//

#ifndef CULTIVATED_COMPILER_CODE_GENERATOR_H
#define CULTIVATED_COMPILER_CODE_GENERATOR_H

#include <vector>
#include <string>
#include "instruction_generators.h"

class CodeGenerator {
public:
    CodeGenerator() = default;

    void generateMachineCode();

    void setCommandList(CommandList *tCommandList);

private:
    CommandList *commandList;
};


#endif //CULTIVATED_COMPILER_CODE_GENERATOR_H
