//
// Created by erthax on 15.01.2021.
//

#include "code_writer.h"

void CodeWriter::writeOutput(const std::string& tFileName) {
    std::ofstream output;
    output.open(tFileName);
    for (auto &it : instructionList) {
        output << it->to_string() + "\n";
    }
    output.close();
}

void CodeWriter::loadNumberToRegister(std::string tRegName, uint value) {
    std::vector<std::string> value_instructions;
    this->reset(tRegName);
    if (value > 6) {
        value--;
        value_instructions.emplace_back("INC");
        while (value > 0) {
            if (value % 2 == 0) {
                value /= 2;
                value_instructions.emplace_back("SHL");
            } else {
                value--;
                value_instructions.emplace_back("INC");
            }
        }
        for (int i = value_instructions.size() - 1; i >= 0; i--) {
            if (value_instructions[i] == "INC") {
                this->inc(tRegName);
            } else {
                this->shl(tRegName);
            }
        }
    } else {
        for (int i = 0; i < value; i++) {
            this->inc(tRegName);
        }
    }
}

// Istruction generation
void CodeWriter::get(std::string tRegName) {
    this->instructionList.push_back(new Instruction("GET", tRegName));
    this->instructionCount++;
}

void CodeWriter::put(std::string tRegName) {
    this->instructionList.push_back(new Instruction("PUT", tRegName));
    this->instructionCount++;
}

void CodeWriter::load(std::string tRegName1, std::string tRegName2) {
    this->instructionList.push_back(new Instruction("LOAD", tRegName1, tRegName2));
    this->instructionCount++;
}

void CodeWriter::store(std::string tRegName1, std::string tRegName2) {
    this->instructionList.push_back(new Instruction("STORE", tRegName1, tRegName2));
    this->instructionCount++;
}

void CodeWriter::add(std::string tRegName1, std::string tRegName2) {
    this->instructionList.push_back(new Instruction("ADD", tRegName1, tRegName2));
    this->instructionCount++;
}

void CodeWriter::sub(std::string tRegName1, std::string tRegName2) {
    this->instructionList.push_back(new Instruction("SUB", tRegName1, tRegName2));
    this->instructionCount++;
}

void CodeWriter::reset(std::string tRegName) {
    this->instructionList.push_back(new Instruction("RESET", tRegName));
    this->instructionCount++;
}

void CodeWriter::inc(std::string tRegName) {
    this->instructionList.push_back(new Instruction("INC", tRegName));
    this->instructionCount++;
}

void CodeWriter::dec(std::string tRegName) {
    this->instructionList.push_back(new Instruction("DEC", tRegName));
    this->instructionCount++;
}

void CodeWriter::shr(std::string tRegName) {
    this->instructionList.push_back(new Instruction("SHR", tRegName));
    this->instructionCount++;
}

void CodeWriter::shl(std::string tRegName) {
    this->instructionList.push_back(new Instruction("SHL", tRegName));
    this->instructionCount++;
}

void CodeWriter::jump(int j) {
    this->instructionList.push_back(new Instruction("JUMP", std::to_string(j)));
    this->instructionCount++;
}

void CodeWriter::jzero(std::string tRegName, int j) {
    this->instructionList.push_back(new Instruction("JZERO", tRegName, std::to_string(j)));
    this->instructionCount++;
}

void CodeWriter::jodd(std::string tRegName, int j) {
    this->instructionList.push_back(new Instruction("JODD", tRegName, std::to_string(j)));
    this->instructionCount++;
}

void CodeWriter::halt() {
    this->instructionList.push_back(new Instruction("HALT"));
    this->instructionCount++;
}

uint CodeWriter::getInstructionCount() const {
    return instructionCount;
}

void CodeWriter::setInstructionCount(uint instructionCount) {
    CodeWriter::instructionCount = instructionCount;
}
