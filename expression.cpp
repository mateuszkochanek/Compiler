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
    Register *valueRegister = memory->getFreeRegister();
    Command::loadValueToRegister(resultRegister, this->value1);
    Command::loadValueToRegister(valueRegister, this->value2);
    code->add(resultRegister->getName(), valueRegister->getName());
}


//____________________________SubExpression________________________________
void SubExpression::generateExpressionValue(Register *resultRegister) {
    Register *valueRegister = memory->getFreeRegister();
    Command::loadValueToRegister(resultRegister, this->value1);
    Command::loadValueToRegister(valueRegister, this->value2);
    code->sub(resultRegister->getName(), valueRegister->getName());
}


//____________________________MulExpression________________________________
void MulExpression::generateExpressionValue(Register *resultRegister) {
    Register *multiplierRegister = memory->getFreeRegister();
    Register *multiplicandRegister = memory->getFreeRegister();
    code->reset(resultRegister->getName());
    Command::loadValueToRegister(multiplierRegister, this->value1);
    Command::loadValueToRegister(multiplicandRegister, this->value2);


    //we can add jumps to finish if either of them is zero
    code->jzero(multiplicandRegister->getName(), 9);
    uint jumpAdress = code->getInstructionCount();
    //if multiplicand is odd we add multiplier to result
    code->jodd(multiplicandRegister->getName(), 4); // here we check if multi is odd, if it it we jump
        // here we now that multi is not odd
        code->shl(multiplierRegister->getName());
        code->shr(multiplicandRegister->getName());
        code->jump(-3);
    //here we jumped if multi was odd
    code->add(resultRegister->getName(), multiplierRegister->getName());
    code->shl(multiplierRegister->getName());
    code->shr(multiplicandRegister->getName());
    // if multiplicand is not zero we jump to start
    code->jump(-8);

}


//____________________________DivExpression________________________________
void DivExpression::generateExpressionValue(Register *resultRegister) {
    Register *dividentRegister = memory->getFreeRegister();
    Register *divisorRegister = memory->getFreeRegister();
    code->reset(resultRegister->getName());
    Command::loadValueToRegister(dividentRegister, this->value1);
    Command::loadValueToRegister(divisorRegister, this->value2);


}