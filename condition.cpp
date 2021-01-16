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

    //here we add jzero that does commands specified in command if it hits zero
    code->jzero(conditionRegister->getName(), 2);
}


//______________________________DiffCondition_________________________________
void DiffCondition::generateConditionValue(Register *conditionRegister) {
    Register* firstHelperRegister = memory->getFreeRegister();
    Register* secondHelperRegister = memory->getFreeRegister();
    Command::loadValueToRegister(conditionRegister, value1); // conditionRegister has value1
    Command::loadValueToRegister(firstHelperRegister, value2); // firstHelperRegister has value2
    code->reset(secondHelperRegister->getName());
    code->add(secondHelperRegister->getName(), conditionRegister->getName()); // secondHelperRegister has value1 copy

    code->sub(conditionRegister->getName(), firstHelperRegister->getName()); // conditionRegister = value1 - value2
    code->sub(firstHelperRegister->getName(), secondHelperRegister->getName()); // firstHelperRegister = value2 - value1
    code->add(conditionRegister->getName(), firstHelperRegister->getName()); // if conditionRegister = 0 its NOT ok.

    //here we add jzero that skips commands specified in command if it hits zero
    code->jzero(conditionRegister->getName(), 2);
    code->jump(2); // this jumps over big jump that skips all commands if jzero before didn't execute

}

//____________________________LessThanCondition________________________________
void LessThanCondition::generateConditionValue(Register *conditionRegister) {
    Register* firstHelperRegister = memory->getFreeRegister();
    Command::loadValueToRegister(conditionRegister, value2); // conditionRegister has value2
    Command::loadValueToRegister(firstHelperRegister, value1); // firstHelperRegister has value1

    code->sub(conditionRegister->getName(), firstHelperRegister->getName()); // firstHelperRegister = value2 - value1

    //here we add jzero that skips commands specified in command if it hits zero
    code->jzero(conditionRegister->getName(), 2);
    code->jump(2);// this jumps over big jump that skips all commands if jzero before didn't execute
}


//____________________________GreaterThanCondition_______________________________
void GreaterThanCondition::generateConditionValue(Register *conditionRegister) {
    Register* firstHelperRegister = memory->getFreeRegister();
    Command::loadValueToRegister(conditionRegister, value1); // conditionRegister has value1
    Command::loadValueToRegister(firstHelperRegister, value2); // firstHelperRegister has value2

    code->sub(conditionRegister->getName(), firstHelperRegister->getName()); // firstHelperRegister = value1 - value2

    //here we add jzero that skips commands specified in command if it hits zero
    code->jzero(conditionRegister->getName(), 2);
    code->jump(2);// this jumps over big jump that skips all commands if jzero before didn't execute
}


//____________________________LessEqualCondition_______________________________
void LessEqualCondition::generateConditionValue(Register *conditionRegister) {
    Register* firstHelperRegister = memory->getFreeRegister();
    Command::loadValueToRegister(conditionRegister, value1); // conditionRegister has value1
    Command::loadValueToRegister(firstHelperRegister, value2); // firstHelperRegister has value2

    code->sub(conditionRegister->getName(), firstHelperRegister->getName()); // firstHelperRegister = value1 - value2

    //here we add jzero that skips jump that skips all commands if its zero
    code->jzero(conditionRegister->getName(), 2);
}


//____________________________GreaterThanCondition_______________________________
void GreaterEqualCondition::generateConditionValue(Register *conditionRegister) {
    Register* firstHelperRegister = memory->getFreeRegister();
    Command::loadValueToRegister(conditionRegister, value2); // conditionRegister has value2
    Command::loadValueToRegister(firstHelperRegister, value1); // firstHelperRegister has value1

    code->sub(conditionRegister->getName(), firstHelperRegister->getName()); // firstHelperRegister = value2 - value1

    //here we add jzero that skips jump that skips all commands if its zero
    code->jzero(conditionRegister->getName(), 2);
}