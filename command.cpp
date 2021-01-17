//
// Created by erthax on 15.01.2021.
//
#include "instruction_generators.h"

//______________________________Command_________________________________
void Command::loadIdentifierAddress(Register *tAddressRegister, Identifier *tIdentifier, bool checkIsInitialized) {
    if (tIdentifier->type == eIdentifier::VARIABLE_IDENTIFIER) {
        if (checkIsInitialized) {
            Command::checkIfInitialized(tIdentifier->variable);
        }
        if (tIdentifier->variable->variableType == eVariableType::ITERATOR) {
            throw tIdentifier->variable->pid + " is an array iterator";
        }
        if (tIdentifier->variable->variableType == eVariableType::ARRAY) {
            throw  "wrong use of array variable " + tIdentifier->variable->pid;
        }
        code->loadNumberToRegister(tAddressRegister->getName(), tIdentifier->variable->address);
    } else if (tIdentifier->type == eIdentifier::NUMBER_ARRAY_IDENTIFIER) {
        uint cellAddress = tIdentifier->array->address;
        if (tIdentifier->array->variableType == eVariableType::SINGLE) {
            throw  "wrong use of variable " + tIdentifier->array->pid;
        }
        if (tIdentifier->index < tIdentifier->array->start || tIdentifier->index > tIdentifier->array->end) {
            throw tIdentifier->array->pid + " wrong array range";
        }
        cellAddress += tIdentifier->index - tIdentifier->array->start;
        code->loadNumberToRegister(tAddressRegister->getName(), cellAddress);
    } else {
        if (tIdentifier->array->variableType == eVariableType::SINGLE) {
            throw  "wrong use of variable " + tIdentifier->variable->pid;
        }
        Command::checkIfInitialized(tIdentifier->variable);
        Register *helperRegister = memory->getFreeRegister();
        code->loadNumberToRegister(helperRegister->getName(),
                                   tIdentifier->variable->address); //we load index variable address
        code->load(helperRegister->getName(), helperRegister->getName()); // we load numValue of index variable
        code->loadNumberToRegister(tAddressRegister->getName(),
                                   tIdentifier->array->address);// load to register array address
        code->add(tAddressRegister->getName(),
                  helperRegister->getName());// add to array address index variable numValue
        code->loadNumberToRegister(helperRegister->getName(), tIdentifier->array->start);// load to register array start
        code->sub(tAddressRegister->getName(), helperRegister->getName());// sub array start from addressRegister
        helperRegister->setFull(false);
    }
}


void Command::loadValueToRegister(Register *tValueRegister, Value *tValue) {
    Register *addressRegister = memory->getFreeRegister();
    if (tValue->type == eValue::IDENTIFIER_VALUE) { // numValue with some variable
        if (tValue->identifier->type == eIdentifier::VARIABLE_IDENTIFIER &&
            tValue->identifier->variable->variableType ==
            eVariableType::ITERATOR) { // if its an iterator called from value we load it
            // we now for sure that its a VARIABLE_IDENTIFIER
            code->loadNumberToRegister(addressRegister->getName(), tValue->identifier->variable->address);
            code->load(tValueRegister->getName(), addressRegister->getName());
        } else {
            Command::loadIdentifierAddress(addressRegister, tValue->identifier, true);
            code->load(tValueRegister->getName(), addressRegister->getName());
        }
    } else { // numValue with a constant variable
        if (tValue->identifier->variable->bInitialized) { //TODO check when its worth it to save constant instead of gen it again
            Command::loadIdentifierAddress(addressRegister, tValue->identifier, true);
            code->load(tValueRegister->getName(), addressRegister->getName());
        } else { // if const was never created, create, save it in memory and load it for our use
            code->loadNumberToRegister(tValueRegister->getName(),
                                       tValue->numValue); // we load number value to our register
            Command::loadIdentifierAddress(addressRegister, tValue->identifier,
                                           false); // we load constant address to our register
            code->store(tValueRegister->getName(),
                        addressRegister->getName()); // we store our value in address from addressRegister
            tValue->identifier->variable->bInitialized = true; // constant is initialized
            // our value is still in tValueRegister
        }
    }
    addressRegister->setFull(false);
}


void Command::checkIfInitialized(Variable *tVaraiable) {
    if (!(tVaraiable->bInitialized)) {
        throw tVaraiable->pid + " was not initialized";
    }
}

//____________________________ReadCommand_______________________________
void ReadCommand::generateInstructions() {
    Register *addressRegister = memory->getFreeRegister();
    Command::loadIdentifierAddress(addressRegister, identifier, false);
    code->get(addressRegister->getName()); // we save input to address we got
    if (identifier->type == eIdentifier::VARIABLE_IDENTIFIER) {
        identifier->variable->bInitialized = true;
    } // we set variable to initialized
    memory->freeRegisters();
}


//____________________________WriteCommand_______________________________
void WriteCommand::generateInstructions() {
    Register *addressRegister = memory->getFreeRegister();
    if (value->type == eValue::IDENTIFIER_VALUE) { // numValue with some variable
        if (value->identifier->type == eIdentifier::VARIABLE_IDENTIFIER &&
                value->identifier->variable->variableType ==
            eVariableType::ITERATOR) { // if its an iterator called from value we load it
            // we now for sure that its a VARIABLE_IDENTIFIER
            code->loadNumberToRegister(addressRegister->getName(), value->identifier->variable->address);
        } else{
            Command::loadIdentifierAddress(addressRegister, this->value->identifier, true);
        }
    } else { // numValue with a constant variable
        if (value->identifier->variable->bInitialized) {
            Command::loadIdentifierAddress(addressRegister,
                                           this->value->identifier,
                                           false); // if constant was created before, load its address
        } else { // if const was never created, create and save it in memory
            Register *helperRegister = memory->getFreeRegister();
            code->loadNumberToRegister(helperRegister->getName(), value->numValue);
            Command::loadIdentifierAddress(addressRegister, this->value->identifier, false);
            code->store(helperRegister->getName(), addressRegister->getName());
            value->identifier->variable->bInitialized = true; // constant is initialized
        }
    }
    code->put(addressRegister->getName());
    memory->freeRegisters();
}


//_____________________________IsCommand________________________________
void IsCommand::generateInstructions() {
    // generate expression value into specified register
    Register *resultRegister = memory->getFreeRegister();
    expression->generateExpressionValue(resultRegister);
    memory->freeRegisters();
    resultRegister->setFull(true);
    // save this value into address in our value in IsCommand
    Register *addressRegister = memory->getFreeRegister();
    Command::loadIdentifierAddress(addressRegister, identifier, false);
    code->store(resultRegister->getName(), addressRegister->getName());
    if (identifier->type == eIdentifier::VARIABLE_IDENTIFIER) {
        identifier->variable->bInitialized = true;
    } // we set variable to initialized
    memory->freeRegisters();
}


//_____________________________IfCommand________________________________
void IfCommand::generateInstructions() {

    Register *conditionRegister = memory->getFreeRegister();
    condition->generateConditionValue(conditionRegister);
    int instructionCountBeforeCommands = code->getInstructionCount();
    memory->freeRegisters();
    int size = this->commandList->size();
    for (int i = 0; i < size; i++) {
        (*commandList)[i]->generateInstructions();
    }
    int instructionCountAfterCommands = code->getInstructionCount();

    code->putInstructionAtIndex(
            new Instruction("JUMP", std::to_string(instructionCountAfterCommands - instructionCountBeforeCommands + 1)),
            instructionCountBeforeCommands);
    memory->freeRegisters();
}


//_____________________________IfElseCommand________________________________
void IfElseCommand::generateInstructions() {

    Register *conditionRegister = memory->getFreeRegister();
    condition->generateConditionValue(conditionRegister);

    int instructionCountBeforeIfCommands = code->getInstructionCount();
    memory->freeRegisters();
    int sizeIf = this->commandListIf->size();
    for (int i = 0; i < sizeIf; i++) {
        (*commandListIf)[i]->generateInstructions();
    }

    int instructionCountBeforeElseCommands = code->getInstructionCount();
    memory->freeRegisters();
    int sizeElse = this->commandListElse->size();
    for (int i = 0; i < sizeElse; i++) {
        (*commandListElse)[i]->generateInstructions();
    }

    int instructionCountAfterCommands = code->getInstructionCount();

    code->putInstructionAtIndex( // Jump that skips If commands
            new Instruction("JUMP",
                            std::to_string(instructionCountBeforeElseCommands - instructionCountBeforeIfCommands + 2)),
            instructionCountBeforeIfCommands);
    memory->freeRegisters();
    code->putInstructionAtIndex( // Jump that skips Else commands when if commands finished
            new Instruction("JUMP",
                            std::to_string(instructionCountAfterCommands - instructionCountBeforeElseCommands + 1)),
            instructionCountBeforeElseCommands + 1);
    memory->freeRegisters();
}


//______________________________WhileCommand_________________________________
void WhileCommand::generateInstructions() { // very similar to if, just add jump that jumps before condition
    int instructionCountBeforeCondition = code->getInstructionCount();
    Register *conditionRegister = memory->getFreeRegister();
    condition->generateConditionValue(conditionRegister);
    int instructionCountBeforeCommands = code->getInstructionCount();
    memory->freeRegisters();
    int size = this->commandList->size();
    for (int i = 0; i < size; i++) {
        (*commandList)[i]->generateInstructions();
    }
    int instructionCountAfterCommands = code->getInstructionCount();
    code->jump(
            -(instructionCountAfterCommands - instructionCountBeforeCondition + 1)); // +1 bcs of put instruction below

    code->putInstructionAtIndex(
            new Instruction("JUMP", std::to_string(instructionCountAfterCommands - instructionCountBeforeCommands + 2)),
            instructionCountBeforeCommands);
    memory->freeRegisters();
}


//______________________________RepeatCommand_________________________________
void RepeatCommand::generateInstructions() { // very similar to if, just add jump that jumps before condition
    int instructionCountBeforeCommands = code->getInstructionCount();
    int size = this->commandList->size();
    for (int i = 0; i < size; i++) {
        (*commandList)[i]->generateInstructions();
    }
    Register *conditionRegister = memory->getFreeRegister();
    condition->generateConditionValue(conditionRegister);
    memory->freeRegisters();
    int instructionCountAfterCommands = code->getInstructionCount();
    code->jump(-(instructionCountAfterCommands - instructionCountBeforeCommands));

    memory->freeRegisters();
}


//_______________________________ForCommand__________________________________
void ForCommand::generateInstructions() {
    // create iterator variable and iterator identifier with this variable
    memory->getIterator(pid)->isInsideLoop = true;
    Value *iteratorValue = new Value(new Identifier(pid)); // we have value created

    Register *valueRegister = memory->getFreeRegister();
    Register *addressRegister = memory->getFreeRegister();
    Command::loadValueToRegister(valueRegister, value1); // we load starting value to register
    code->loadNumberToRegister(addressRegister->getName(),
                               iteratorValue->identifier->variable->address); //we load iterator address to register
    code->store(valueRegister->getName(), addressRegister->getName()); // we store starting iterator val in iterator
    iteratorValue->identifier->variable->bInitialized = true;

    std::string iteratorEndPid = pid + std::to_string(memory->iteratorCount);
    // if value2 is NUMBER_VALUE we dont have to do anything (our ending point is a constant and wont change inside a loop)
    if (value2->type != eValue::NUMBER_VALUE) {// if value2 is VARIABLE_IDENTIFIER
        Command::loadValueToRegister(valueRegister, value2);
        memory->declareIterator(iteratorEndPid);
        Value *iteratorEndValue = new Value(new Identifier(iteratorEndPid));
        Command::loadValueToRegister(valueRegister, value2); // we load ending value to register
        code->loadNumberToRegister(addressRegister->getName(),
                                   iteratorEndValue->identifier->variable->address); //we load iterator address to register
        code->store(valueRegister->getName(), addressRegister->getName()); // we store starting iterator val in iterator
        this->value2 = iteratorEndValue;
    }
    memory->freeRegisters(); // we have both values prepared for for loop

    int instructionCountBeforeCondition = code->getInstructionCount();
    LessEqualCondition *endingCondition = new LessEqualCondition(iteratorValue, this->value2);
    valueRegister = memory->getFreeRegister();
    endingCondition->generateConditionValue(valueRegister);
    memory->freeRegisters();

    int instructionCountBeforeCommands = code->getInstructionCount();
    int size = this->commandList->size();
    for (int i = 0; i < size; i++) {
        (*commandList)[i]->generateInstructions();
    }
    valueRegister = memory->getFreeRegister();
    addressRegister = memory->getFreeRegister();
    code->loadNumberToRegister(addressRegister->getName(),
                               iteratorValue->identifier->variable->address); // we load iterator address
    code->load(valueRegister->getName(), addressRegister->getName()); // we load iterator value
    code->inc(valueRegister->getName()); // we increase value
    code->store(valueRegister->getName(), addressRegister->getName()); //we store the value
    int instructionCountAfterCommands = code->getInstructionCount();
    code->jump(
            -(instructionCountAfterCommands - instructionCountBeforeCondition + 1)); // +1 bcs of put instruction below

    code->putInstructionAtIndex(
            new Instruction("JUMP", std::to_string(instructionCountAfterCommands - instructionCountBeforeCommands + 2)),
            instructionCountBeforeCommands);

    memory->freeRegisters();

    memory->getIterator(pid)->isInsideLoop = false;
    if (value2->type != eValue::NUMBER_VALUE)
        memory->eraseIterator(iteratorEndPid);

}


//_____________________________ForDowntoCommand________________________________
void ForDowntoCommand::generateInstructions() {
    // create iterator variable and iterator identifier with this variable
    memory->getIterator(pid)->isInsideLoop = true;
    Value *iteratorValue = new Value(new Identifier(pid)); // we have value created

    Register *valueRegister = memory->getFreeRegister();
    Register *addressRegister = memory->getFreeRegister();
    Command::loadValueToRegister(valueRegister, value1); // we load starting value to register
    code->loadNumberToRegister(addressRegister->getName(),
                               iteratorValue->identifier->variable->address); //we load iterator address to register
    code->store(valueRegister->getName(), addressRegister->getName()); // we store starting iterator val in iterator
    iteratorValue->identifier->variable->bInitialized = true;

    std::string iteratorEndPid = pid + std::to_string(memory->iteratorCount);
    // if value2 is NUMBER_VALUE we dont have to do anything (our ending point is a constant and wont change inside a loop)
    if (value2->type != eValue::NUMBER_VALUE) {// if value2 is VARIABLE_IDENTIFIER
        Command::loadValueToRegister(valueRegister, value2);
        memory->declareIterator(iteratorEndPid);
        Value *iteratorEndValue = new Value(new Identifier(iteratorEndPid));
        Command::loadValueToRegister(valueRegister, value2); // we load ending value to register
        code->loadNumberToRegister(addressRegister->getName(),
                                   iteratorEndValue->identifier->variable->address); //we load iterator address to register
        code->store(valueRegister->getName(), addressRegister->getName()); // we store starting iterator val in iterator
        this->value2 = iteratorEndValue;
    }
    memory->freeRegisters(); // we have both values prepared for for loop

    int instructionCountBeforeCondition = code->getInstructionCount();
    GreaterEqualCondition *endingCondition = new GreaterEqualCondition(iteratorValue, this->value2);
    valueRegister = memory->getFreeRegister();
    endingCondition->generateConditionValue(valueRegister);
    memory->freeRegisters();

    int instructionCountBeforeCommands = code->getInstructionCount();
    int size = this->commandList->size();
    for (int i = 0; i < size; i++) {
        (*commandList)[i]->generateInstructions();
    }
    valueRegister = memory->getFreeRegister();
    addressRegister = memory->getFreeRegister();
    code->loadNumberToRegister(addressRegister->getName(),
                               iteratorValue->identifier->variable->address); // we load iterator address
    code->load(valueRegister->getName(), addressRegister->getName()); // we load iterator value
    code->jzero(valueRegister->getName(), 4);
    code->dec(valueRegister->getName()); // we increase value
    code->store(valueRegister->getName(), addressRegister->getName()); //we store the value
    int instructionCountAfterCommands = code->getInstructionCount();
    code->jump(
            -(instructionCountAfterCommands - instructionCountBeforeCondition + 1)); // +1 bcs of put instruction below

    code->putInstructionAtIndex(
            new Instruction("JUMP", std::to_string(instructionCountAfterCommands - instructionCountBeforeCommands + 2)),
            instructionCountBeforeCommands);

    memory->freeRegisters();
    memory->getIterator(pid)->isInsideLoop = false;
    if (value2->type != eValue::NUMBER_VALUE)
        memory->eraseIterator(iteratorEndPid);

}