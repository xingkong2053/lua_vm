//
// Created by Administrator on 2024/4/26.
//

#include "Instruction.h"
#include "opcode.h"

/*
 31       22       13       5    0
  +-------+^------+-^-----+-^-----
  |b=9bits |c=9bits |a=8bits|op=6|
  +-------+^------+-^-----+-^-----
  |    bx=18bits    |a=8bits|op=6|
  +-------+^------+-^-----+-^-----
  |   sbx=18bits    |a=8bits|op=6|
  +-------+^------+-^-----+-^-----
  |    ax=26bits            |op=6|
  +-------+^------+-^-----+-^-----
 31      23      15       7      0
*/

Instruction::Instruction(unsigned int instr) :instr(instr) {

}


int Instruction::getOpCode() const {
    return int(instr & 0x3F);
}

std::tuple<int, int, int> Instruction::getABC() const {
    auto a = int(instr >> 6 & 0xFF);
    auto c = int(instr >> 14 & 0x1FF);
    auto b = int(instr >> 23 & 0x1FF);
    return {a, b, c};
}

pair<int, int> Instruction::getABx() const {
    auto a = int(instr >> 6 & 0xFF);
    auto bx = int(instr >> 14);
    return {a, bx};
}

pair<int, int> Instruction::getAsBx() const {
    auto [a, bx] = getABx();
    return {a, bx - MAXARG_sBx};
}

int Instruction::getAx() const {
    return int(instr >> 6);
}

string Instruction::getName() const {
    return opcodes[getOpCode()].name;
}

char Instruction::getOpMode() const {
    return opcodes[getOpCode()].opMode;
}

char Instruction::getArgBMode() const {
    return opcodes[getOpCode()].argBMode;
}

char Instruction::getArgCMode() const {
    return opcodes[getOpCode()].argCMode;
}
