//
// Created by erthax on 15.01.2021.
//

#include "instruction_generators.h"

//______________________________Expression_________________________________


//___________________________ValueExpression_______________________________
void ValueExpression::generateExpressionValue(Register *resultRegister) {
    Command::loadValueToRegister(resultRegister, this->value);
}


//____________________________AddExpression________________________________
void AddExpression::generateExpressionValue(Register *resultRegister) {
    Register* valueRegister = memory->getFreeRegister();
    Command::loadValueToRegister(resultRegister, this->value1);
    Command::loadValueToRegister(valueRegister, this->value2);
    code->add(resultRegister->getName(), valueRegister->getName());
}


//____________________________SubExpression________________________________
void SubExpression::generateExpressionValue(Register *resultRegister) {
    Register* valueRegister = memory->getFreeRegister();
    Command::loadValueToRegister(resultRegister, this->value1);
    Command::loadValueToRegister(valueRegister, this->value2);
    code->sub(resultRegister->getName(), valueRegister->getName());
}