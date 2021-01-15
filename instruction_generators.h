//
// Created by erthax on 15.01.2021.
//

#ifndef CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H
#define CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H

#include "memory.h"
#include "code_writer.h"

extern Memory* memory;
extern CodeWriter* code;

class Command {
public:
    virtual void generateInstructions();
};

class Expression {

};

class Condition {

};

#endif //CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H
