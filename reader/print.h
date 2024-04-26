//
// Created by Administrator on 2024/4/25.
//

#ifndef LUA_VM_PRINT_H
#define LUA_VM_PRINT_H
#include "Reader.h"
#include <memory>
#include "../opcode/Instruction.h"

void list(const shared_ptr<Prototype>& p, bool detail);
void printHeader(const shared_ptr<Prototype>& p);
void printCode(const shared_ptr<Prototype>& p);
// 打印常量表，局部变量表和Upvalue表
void printDetail(const shared_ptr<Prototype>& p);
// 打印指令操作数
void printOperands(Instruction ins);

#endif //LUA_VM_PRINT_H
