//
// Created by erthax on 14.01.2021.
//

#ifndef CULTIVATED_COMPILER_VARIABLE_H
#define CULTIVATED_COMPILER_VARIABLE_H

#include <iostream>

enum eVariableType {
    ARRAY, SINGLE
};

struct Variable {
    eVariableType variableType;
    uint adress;

    // needed for arrays
    uint start;
    uint end;

    // needed for error checks
    bool bInitialized;
    bool bDeclared;
};

#endif //CULTIVATED_COMPILER_VARIABLE_H
