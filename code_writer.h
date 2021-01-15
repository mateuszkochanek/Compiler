//
// Created by erthax on 15.01.2021.
//

#ifndef CULTIVATED_COMPILER_CODE_WRITER_H
#define CULTIVATED_COMPILER_CODE_WRITER_H

#include <string>
#include <vector>
#include <fstream>

class Instruction {
public:
    Instruction(std::string tOrder) : order(tOrder), RegName1(""), RegName2("") {}

    Instruction(std::string tOrder, std::string tRegName) : order(tOrder), RegName1(tRegName), RegName2("") {}

    Instruction(std::string tOrder, std::string tRegName1, std::string tRegName2) : order(tOrder), RegName1(tRegName1),
                                                                                    RegName2(tRegName2) {}

    std::string to_string() { return this->order + " " + this->RegName1 + " " + this->RegName2; }

private:
    std::string order;
    std::string RegName1;
    std::string RegName2;
};

class CodeWriter {
public:
    CodeWriter() = default;;

    void writeOutput(const std::string& tFileName);

    void loadValueToRegister(std::string tRegisterName, uint value);

    void get(std::string tRegName);

    void put(std::string tRegName);

    void load(std::string tRegName1, std::string tRegName2);

    void store(std::string tRegName1, std::string tRegName2);

    void add(std::string tRegName1, std::string tRegName2);

    void sub(std::string tRegName1, std::string tRegName2);

    void reset(std::string tRegName);

    void inc(std::string tRegName);

    void dec(std::string tRegName);

    void shr(std::string tRegName);

    void shl(std::string tRegName);

    void jump(uint j);

    void jzero(std::string tRegName, uint j);

    void jodd(std::string tRegName, uint j);

    void halt();


private:
    std::vector<Instruction *> instructionList;
    uint instructionCount{0};
};


#endif //CULTIVATED_COMPILER_CODE_WRITER_H
