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
    Variable(uint tAddress, std::string tPid) : adress(tAddress),
                                                pid(std::move(tPid)) { variableType = eVariableType::SINGLE; }

    Variable(uint tAddress, std::string tPid, uint tStart, uint tEnd) : adress(tAddress), pid(std::move(tPid)),
                                                                        start(tStart),
                                                                        end(tEnd) { variableType = eVariableType::ARRAY; }

    eVariableType variableType;
    uint adress;
    std::string pid;

    // needed for arrays
    uint start{};
    uint end{};

    // needed for error checks
    bool bInitialized{};
    bool bDeclared{};
};

#endif //CULTIVATED_COMPILER_VARIABLE_H
