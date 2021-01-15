//
// Created by erthax on 14.01.2021.
//

#ifndef CULTIVATED_COMPILER_TOKENS_H
#define CULTIVATED_COMPILER_TOKENS_H

enum eIdentifier {
    VARIABLE_IDENTIFIER, VARIABLE_ARRAY_IDENTIFIER, NUMBER_ARRAY_IDENTIFIER
};

enum eValue {
    NUMBER_VALUE, IDENTIFIER_VALUE
};

struct Identifier {
    // Identifier for single fariables
    explicit Identifier(std::string tVarPid) : varPid(tVarPid) { type = eIdentifier::VARIABLE_IDENTIFIER; };

    // Identifier for arrays with variables as index
    Identifier(std::string tArrayPid, std::string tVarPid) : arrayPid(tArrayPid),
                                                             varPid(tVarPid) { type = eIdentifier::VARIABLE_ARRAY_IDENTIFIER; };

    //Identifier for arrays with numbers as index
    Identifier(std::string tArrayPid, uint tIndex) : arrayPid(tArrayPid),
                                                     index(tIndex) { type = eIdentifier::NUMBER_ARRAY_IDENTIFIER; };

    eIdentifier type;
    std::string varPid{""};
    std::string arrayPid{""};
    uint index{0};
};

struct Value {
    // Value for number value input
    explicit Value(uint tValue) : value(tValue) { type = eValue::NUMBER_VALUE; };

    // Value for Identifier input
    explicit Value(Identifier *tIdentifier) : identifier(tIdentifier) { type = eValue::IDENTIFIER_VALUE; };

    eValue type;
    uint value{0};
    Identifier *identifier{};
};

#endif //CULTIVATED_COMPILER_TOKENS_H
