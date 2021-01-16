//
// Created by erthax on 15.01.2021.
//

#ifndef CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H
#define CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H

#include "memory.h"
#include "code_writer.h"
#include "tokens.h"

class Expression;


extern Memory *memory;
extern CodeWriter *code;

//____________________________Commands_______________________________
class Command {
public:
    virtual void generateInstructions() { throw "in virtual method od Command"; }

    static void loadIdentifierAddress(Register *tAddressRegister, Identifier *tIdentifier, bool checkInitialized);

    static void loadValueToRegister(Register *tAddressRegister, Value *tValue);

    static void checkIfInitialized(Identifier *tIdentifier);

};

class ReadCommand : public Command {
public:
    explicit ReadCommand(Identifier *tIdentifier) { this->identifier = tIdentifier; };

    void generateInstructions() override;

private:
    Identifier *identifier;
};

class WriteCommand : public Command {
public:
    explicit WriteCommand(Value *tValue) { this->value = tValue; };

    void generateInstructions() override;

private:
    Value *value;
};

class IsCommand : public Command {
public:
    explicit IsCommand(Identifier *tIdentifier, Expression *tExpression) {
        this->identifier = tIdentifier;
        this->expression = tExpression;
    };

    void generateInstructions() override;

private:
    Identifier *identifier;
    Expression *expression;
};


//___________________________Expressions______________________________
class Expression {
public:
    virtual void generateExpressionValue(Register *resultRegister) { throw "in virtual method of Expression"; };
};

class ValueExpression : public Expression {
public:
    explicit ValueExpression(Value *tValue) : value(tValue) {};

    void generateExpressionValue(Register *resultRegister) override;

private:
    Value *value;
};

class AddExpression : public Expression {
public:
    explicit AddExpression(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateExpressionValue(Register *resultRegister) override;

private:
    Value *value1;
    Value *value2;
};

class SubExpression : public Expression {
public:
    explicit SubExpression(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateExpressionValue(Register *resultRegister) override;

private:
    Value *value1;
    Value *value2;
};

class MulExpression : public Expression {
public:
    explicit MulExpression(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateExpressionValue(Register *resultRegister) override;

private:
    Value *value1;
    Value *value2;
};

class DivExpression : public Expression {
public:
    explicit DivExpression(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateExpressionValue(Register *resultRegister) override;

private:
    Value *value1;
    Value *value2;
};

class ModExpression : public Expression {
public:
    explicit ModExpression(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateExpressionValue(Register *resultRegister) override;

private:
    Value *value1;
    Value *value2;
};

//____________________________Conditions_______________________________
class Condition {

};

#endif //CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H
