//
// Created by erthax on 15.01.2021.
//

#include "code_generator.h"

void CodeGenerator::generateMachineCode() {
    int size = this->commandList->size();
    for (int i = 0; i < size; i++) {
        (*commandList)[i]->generateInstructions();
    }
}

void CodeGenerator::setCommandList(CommandList *tCommandList) {
    //TODO add halt command/instruction at the end
    this->commandList = tCommandList;
}
