//
// Created by erthax on 14.01.2021.
//

#ifndef CULTIVATED_COMPILER_VARIABLE_H
#define CULTIVATED_COMPILER_VARIABLE_H

#include <iostream>
#include <utility>

enum eVariableType {
    ARRAY, SINGLE
};

struct Variable {
    // Single variable declaration
    Variable(uint tAddress, std::string tPid) : adress(tAddress),
                                                pid(std::move(tPid)) { variableType = eVariableType::SINGLE; }
    // Array variable declaration
    Variable(uint tAddress, std::string tPid, uint tStart, uint tEnd) : adress(tAddress), pid(std::move(tPid)),
                                                                        start(tStart),
                                                                        end(tEnd) { variableType = eVariableType::ARRAY; }

    eVariableType variableType;
    uint adress;
    std::string pid;

    // needed for arrays
    uint start{0};
    uint end{0};

    // needed for error checks
    bool bInitialized{false};
    bool bDeclared{true};
};

#endif //CULTIVATED_COMPILER_VARIABLE_H
