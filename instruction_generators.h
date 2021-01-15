//
// Created by erthax on 15.01.2021.
//

#ifndef CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H
#define CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H

#include "memory.h"
#include "code_writer.h"
#include "tokens.h"

extern Memory* memory;
extern CodeWriter* code;

//____________________________Commands_______________________________
class Command {
public:
    virtual void generateInstructions(){throw "in virtual gen Instructions";}
protected:
    void loadIdentifierAddress(Register *addressRegister) const;
    void checkIfInitialized();
    Identifier* identifier;
    Value* value;
};

class ReadCommand: public Command{
public:
    explicit ReadCommand(Identifier* tIdentifier){this->identifier = tIdentifier;};
    void generateInstructions() override;
};

class WriteCommand: public Command{
public:
    explicit WriteCommand(Value* tValue){this->value = tValue; this->identifier = tValue->identifier;};
    void generateInstructions() override;
};



//___________________________Expressions______________________________
class Expression {

};

//____________________________Conditions_______________________________
class Condition {

};

#endif //CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H
