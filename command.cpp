//
// Created by erthax on 15.01.2021.
//
#include "instruction_generators.h"

//______________________________Command_________________________________
// TODO maybe make it static with identifier as param? if so change WriteCommand constructor
void Command::loadIdentifierAddress(Register *tAddressRegister, Identifier *tIdentifier) {
    if (tIdentifier->type == eIdentifier::VARIABLE_IDENTIFIER) {
        code->loadNumberToRegister(tAddressRegister->getName(), tIdentifier->variable->address);
    } else if (tIdentifier->type == eIdentifier::NUMBER_ARRAY_IDENTIFIER) {
        uint cellAdress = tIdentifier->array->address;
        cellAdress += tIdentifier->index - tIdentifier->array->start;
        code->loadNumberToRegister(tAddressRegister->getName(), cellAdress);
    } else {
        Register *helperRegister = memory->getFreeRegister();
        code->loadNumberToRegister(helperRegister->getName(),
                                   tIdentifier->variable->address); //we load index variable address
        code->load(helperRegister->getName(), helperRegister->getName()); // we load numValue of index variable
        code->loadNumberToRegister(tAddressRegister->getName(),
                                   tIdentifier->array->address);// load to register array address
        code->add(tAddressRegister->getName(),
                  helperRegister->getName());// add to array address index variable numValue
        code->loadNumberToRegister(helperRegister->getName(), tIdentifier->array->start);// load to register array start
        code->sub(tAddressRegister->getName(), helperRegister->getName());// sub array start from adressREgister
    }
}


void Command::loadValueToRegister(Register *tValueRegister, Value *tValue) {
    Register* adressRegister = memory->getFreeRegister();
    if (tValue->type == eValue::IDENTIFIER_VALUE) { // numValue with some variable
        checkIfInitialized(tValue);
        Command::loadIdentifierAddress(adressRegister, tValue->identifier);
        code->load(tValueRegister->getName(), adressRegister->getName());
    } else { // numValue with a constant variable
        if (tValue->identifier->variable->bInitialized) { //TODO check when its worth it to save constant instead of gen it again
            Command::loadIdentifierAddress(adressRegister, tValue->identifier);
            code->load(tValueRegister->getName(), adressRegister->getName());
        } else { // if const was never created, create, save it in memory and load it for our use
            code->loadNumberToRegister(tValueRegister->getName(), tValue->numValue); // we load number value to our register
            Command::loadIdentifierAddress(adressRegister, tValue->identifier); // we load constant address to our register
            code->store(adressRegister->getName(), tValueRegister->getName()); // we store our value in address from addressREgister
            tValue->identifier->variable->bInitialized = true; // constant is initialized
            // our value is still in tValueRegister
        }
    }
}


void Command::checkIfInitialized(Value *tValue) {
    if (!(tValue->identifier->variable->bInitialized)) {
        throw tValue->identifier->variable->pid + "was not initialized";
    }
}

//____________________________ReadCommand_______________________________
void ReadCommand::generateInstructions() {
    Register *addressRegister = memory->getFreeRegister();
    Command::loadIdentifierAddress(addressRegister, this->identifier);
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
        Command::loadIdentifierAddress(addressRegister, this->value->identifier);
    } else { // numValue with a constant variable
        if (value->identifier->variable->bInitialized) {
            Command::loadIdentifierAddress(addressRegister,
                                           this->value->identifier); // if constant was created before, load its address
        } else { // if const was never created, create and save it in memory
            Register *helperRegister = memory->getFreeRegister();
            code->loadNumberToRegister(helperRegister->getName(), value->numValue);
            Command::loadIdentifierAddress(addressRegister, this->value->identifier);
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
    Register *valueRegister = memory->getFreeRegister();
    expression->generateExpressionValue(valueRegister);
    memory->freeRegisters();
    // save this value into addres in our value in IsCommand
    valueRegister->setFull(true);
    Register *addressRegister = memory->getFreeRegister();
    Command::loadIdentifierAddress(addressRegister, this->identifier);
    code->store(valueRegister->getName(), addressRegister->getName());
    if (identifier->type == eIdentifier::VARIABLE_IDENTIFIER) {
        identifier->variable->bInitialized = true;
    } // we set variable to initialized
}