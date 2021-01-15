//
// Created by erthax on 15.01.2021.
//
#include "instruction_generators.h"
//______________________________Command_________________________________
// TODO maybe make it static with identifier as param? if so change WriteCommand constructor
void Command::loadIdentifierAddress(Register *addressRegister) const {
    if (identifier->type == eIdentifier::VARIABLE_IDENTIFIER) {
        code->loadValueToRegister(addressRegister->getName(), identifier->variable->address);
    } else if (identifier->type == eIdentifier::NUMBER_ARRAY_IDENTIFIER) {
        uint cellAdress = identifier->array->address;
        cellAdress += identifier->index - identifier->array->start;
        code->loadValueToRegister(addressRegister->getName(), cellAdress);
    } else {
        Register *helperRegister = memory->getFreeRegister();
        code->loadValueToRegister(helperRegister->getName(),
                                  identifier->variable->address); //we load index variable address
        code->load(helperRegister->getName(), helperRegister->getName()); // we load numValue of index variable
        code->loadValueToRegister(addressRegister->getName(),
                                  identifier->array->address);// load to register array address
        code->add(addressRegister->getName(), helperRegister->getName());// add to array address index variable numValue
        code->loadValueToRegister(helperRegister->getName(), identifier->array->start);// load to register array start
        code->sub(addressRegister->getName(), helperRegister->getName());// sub array start from adressREgister
    }
}

void Command::checkIfInitialized() {
    if(!(value->identifier->variable->bInitialized)){
        throw identifier->variable->pid + "was not initialized";
    }
}

//____________________________ReadCommand_______________________________
void ReadCommand::generateInstructions() {
    Register *addressRegister = memory->getFreeRegister();
    loadIdentifierAddress(addressRegister);
    code->get(addressRegister->getName()); // we save input to address we got
    if(identifier->type == eIdentifier::VARIABLE_IDENTIFIER) {
        identifier->variable->bInitialized = true;
    } // we set variable to initialized
    memory->freeRegisters();
}


//____________________________WriteCommand_______________________________
void WriteCommand::generateInstructions() {
    Register *addressRegister = memory->getFreeRegister();
    if(value->type == eValue::IDENTIFIER_VALUE){ // numValue with some variable
        loadIdentifierAddress(addressRegister);
    } else { // numValue with a constant variable
        if(value->identifier->variable->bInitialized){ //TODO check when its worth it to save constant instead of gen it again
            loadIdentifierAddress(addressRegister); // if constant was created before, load its address
        } else { // if const was never created, create and save it in memory
            Register *helperRegister = memory->getFreeRegister();
            code->loadValueToRegister(helperRegister->getName(), value->numValue);
            loadIdentifierAddress(addressRegister);
            code->store(helperRegister->getName(), addressRegister->getName());
            value->identifier->variable->bInitialized = true; // constant is initialized
        }
    }
    code->put(addressRegister->getName());
    memory->freeRegisters();
}