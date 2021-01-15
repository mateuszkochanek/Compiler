//
// Created by erthax on 14.01.2021.
//

#ifndef CULTIVATED_COMPILER_VARIABLE_H
#define CULTIVATED_COMPILER_VARIABLE_H

#include <iostream>
#include <utility>

enum eVariableType {
    ARRAY, SINGLE, CONSTANT
};

struct Variable {
    // Single variable declaration
    Variable(uint tAddress, std::string tPid) : address(tAddress),
                                                pid(std::move(tPid)) { this->variableType = eVariableType::SINGLE; }

    // Array variable declaration
    Variable(uint tAddress, std::string tPid, uint tStart, uint tEnd) : address(tAddress), pid(std::move(tPid)),
                                                                        start(tStart),
                                                                        end(tEnd) { this->variableType = eVariableType::ARRAY; }
    // Constant variable declaration
    Variable(uint tAddress, std::string tPid, uint tValue) : address(tAddress),
                                                                     pid(tPid), value(tValue) {this->variableType = eVariableType::CONSTANT;}

    eVariableType variableType;
    uint address;
    std::string pid;

    // needed for arrays
    uint start{0};
    uint end{0};

    // needed for costants
    uint value{0}; //TODO probably should be deleted, we will se if its needed, same with CONSTANT type

    // needed for error checks
    bool bInitialized{false};
    bool bDeclared{true};

};

#endif //CULTIVATED_COMPILER_VARIABLE_H
