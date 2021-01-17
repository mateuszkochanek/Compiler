//
// Created by erthax on 14.01.2021.
//

#ifndef CULTIVATED_COMPILER_TOKENS_H
#define CULTIVATED_COMPILER_TOKENS_H

#include "memory.h"

extern Memory *memory;

enum eIdentifier {
    VARIABLE_IDENTIFIER, VARIABLE_ARRAY_IDENTIFIER, NUMBER_ARRAY_IDENTIFIER, ITERATOR_IDENTIFIER
};

enum eValue {
    NUMBER_VALUE, IDENTIFIER_VALUE, ITERATOR_VALUE
};

struct Identifier {
    // Identifier for single fariables
    explicit Identifier(std::string tVarPid) {
        this->type = eIdentifier::VARIABLE_IDENTIFIER;
        this->variable = memory->getVariable(tVarPid);
    };

    // Identifier for identifiers created in commands, mostly iterators TODO delete it and enum type
    explicit Identifier(std::string tVarPid, eIdentifier tType) {
        this->type = tType;
        this->variable = memory->getVariable(tVarPid);
    };

    // Identifier for arrays with variables as index
    Identifier(std::string tArrayPid, std::string tVarPid) {
        this->type = eIdentifier::VARIABLE_ARRAY_IDENTIFIER;
        this->array = memory->getVariable(tArrayPid);
        this->variable = memory->getVariable(tVarPid);
    };

    //Identifier for arrays with numbers as index
    Identifier(std::string tArrayPid, uint tIndex) {
        this->type = eIdentifier::NUMBER_ARRAY_IDENTIFIER;
        this->array = memory->getVariable(tArrayPid);
        this->index = tIndex;
    };

    eIdentifier type;
    Variable *variable;
    Variable *array;
    uint index{0};
};

struct Value {
    // Value for number numValue input. Its the only place where we create constants. We only reserve memory for it here.
    explicit Value(uint tValue) : numValue(tValue) {
        this->type = eValue::NUMBER_VALUE;
        std::string constPid = std::to_string(tValue);
        memory->declareConstant(constPid, tValue);
        this->identifier = new Identifier(constPid);
    };

    // Value for Identifier created from commands, mostyly iterators TODO delete it and enum type
    Value(Identifier *tIdentifier, eValue tType) : identifier(tIdentifier), type(tType) {};

    // Value for Identifier input
    explicit Value(Identifier *tIdentifier) : identifier(tIdentifier) { this->type = eValue::IDENTIFIER_VALUE; };


    eValue type;
    uint numValue{0};
    Identifier *identifier{};
};

#endif //CULTIVATED_COMPILER_TOKENS_H
