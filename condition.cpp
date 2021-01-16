//
// Created by erthax on 15.01.2021.
//

#include "instruction_generators.h"

//______________________________EqualCondition_________________________________
void EqualCondition::generateConditionValue(Register *conditionRegister) {
    Register* firstHelperRegister = memory->getFreeRegister();
    Register* secondHelperRegister = memory->getFreeRegister();
    Command::loadValueToRegister(conditionRegister, value1); // conditionRegister has value1
    Command::loadValueToRegister(firstHelperRegister, value2); // firstHelperRegister has value2
    code->reset(secondHelperRegister->getName());
    code->add(secondHelperRegister->getName(), conditionRegister->getName()); // secondHelperRegister has value1 copy

    code->sub(conditionRegister->getName(), firstHelperRegister->getName()); // conditionRegister = value1 - value2
    code->sub(firstHelperRegister->getName(), secondHelperRegister->getName()); // firstHelperRegister = value2 - value1
    code->add(conditionRegister->getName(), firstHelperRegister->getName()); // if conditionRegister = 0 its ok.

    //here in command we add jzero (jumps) that do action specified in command if it hits zero
}