//
// Created by erthax on 15.01.2021.
//

#ifndef CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H
#define CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H

#include <vector>

#include "memory.h"
#include "code_writer.h"
#include "tokens.h"

class Command;
class Expression;
class Condition;

extern Memory *memory;
extern CodeWriter *code;

typedef std::vector<Command *> CommandList;

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

class IfCommand : public Command {
public:
    explicit IfCommand(Condition *tCondition, CommandList *tCommandList) {
        this->condition = tCondition;
        this->commandList = tCommandList;
    };

    void generateInstructions() override;

private:
    Condition *condition;
    CommandList *commandList;
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
class Condition { ;
public:
    virtual void generateConditionValue(Register *resultRegister) { throw "in virtual method of Condition"; }
};

class EqualCondition: public Condition {
public:
    explicit EqualCondition(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateConditionValue(Register *conditionRegister) override;

private:
    Value *value1;
    Value *value2;
};

class DiffCondition: public Condition {
public:
    explicit DiffCondition(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateConditionValue(Register *conditionRegister) override;

private:
    Value *value1;
    Value *value2;
};

class LessThanCondition: public Condition {
public:
    explicit LessThanCondition(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateConditionValue(Register *conditionRegister) override;

private:
    Value *value1;
    Value *value2;
};

class GreaterThanCondition: public Condition {
public:
    explicit GreaterThanCondition(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateConditionValue(Register *conditionRegister) override;

private:
    Value *value1;
    Value *value2;
};

class LessEqualCondition: public Condition {
public:
    explicit LessEqualCondition(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateConditionValue(Register *conditionRegister) override;

private:
    Value *value1;
    Value *value2;
};

class GreaterEqualCondition: public Condition {
public:
    explicit GreaterEqualCondition(Value *tValue1, Value *tValue2) : value1(tValue1), value2(tValue2) {};

    void generateConditionValue(Register *conditionRegister) override;

private:
    Value *value1;
    Value *value2;
};

#endif //CULTIVATED_COMPILER_INSTRUCTION_GENERATORS_H
