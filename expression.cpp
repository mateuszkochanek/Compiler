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
    Register *dividentRegister = memory->getFreeRegister(); // we will have reminder here
    Register *divisorRegister = memory->getFreeRegister();
    Register *multiplyRegister = memory->getFreeRegister();
    Command::loadValueToRegister(dividentRegister, this->value1);
    Command::loadValueToRegister(divisorRegister, this->value2);
    Register *ifConditionRegister = memory->getFreeRegister();
    Register *whileConditionRegister = memory->getFreeRegister();
    code->reset(resultRegister->getName());
    //if divisor = 0 finish (result is also 0 so its good)
        code->jzero(divisorRegister->getName(), 24);
    // else do division
        // preparing registers
        code->reset(multiplyRegister->getName());
        code->inc(multiplyRegister->getName());
        code->reset(ifConditionRegister->getName());


        //while(dividend - divisor > 0) czyli jezeli wynik = 0 top przerywamy
        //{
        //    code->shl(divisorRegister->getName());
        //    code->shl(multiplyRegister->getName());
        //}
        code->reset(whileConditionRegister->getName());
        code->add(whileConditionRegister->getName(), dividentRegister->getName());
        code->sub(whileConditionRegister->getName(), divisorRegister->getName());
        code->jzero(whileConditionRegister->getName(), 4);
        code->shl(divisorRegister->getName());
        code->shl(multiplyRegister->getName());
        code->jump(-6);


        //do {
        //    if( 0 >= divisor - divident)
        //    {
        //          divident = divident - divisor; FIRST PART
        //          result = result + multiple;
        //          divisor = divisor >> 1; // Divide by two.
        //          multiple       = multiple       >> 1;
        //    } else {
        //          divisor = divisor >> 1; // Divide by two. SECOND PART
        //          multiple       = multiple       >> 1;
        //    }
        //} while(multiple != 0);

        code->reset(ifConditionRegister->getName());
        code->add(ifConditionRegister->getName(), divisorRegister->getName());
        code->sub(ifConditionRegister->getName(), dividentRegister->getName());
        code->jzero(ifConditionRegister->getName(), 4);
        // if it is not 0
            code->shr(divisorRegister->getName()); //second part
            code->shr(multiplyRegister->getName());
            code->jump(5);
        // else
            code->sub(dividentRegister->getName(), divisorRegister->getName());
            code->add(resultRegister->getName(), multiplyRegister->getName());
            code->shr(divisorRegister->getName()); // first part
            code->shr(multiplyRegister->getName());
        code->jzero(multiplyRegister->getName(), 2);
        code->jump(-12);
}