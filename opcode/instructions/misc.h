//
// Created by Administrator on 2024/4/26.
//

#ifndef LUA_VM_MISC_H
#define LUA_VM_MISC_H

#include "../Instruction.h"
#include "../../lua_state/LuaState.h"

typedef shared_ptr<LuaState> LuaVM;

// MOVE 移动指令
void move(Instruction instr, LuaVM vm);
// JMP 跳转指令 通常和TEST配合使用
void jmp(Instruction instr, LuaVM vm);
// LOADNIL
void loadNil(Instruction instr, LuaVM vm);
// LOADBOOL 给单个寄存器设置布尔值
void loadBool(Instruction instr, LuaVM vm);
// LOADK
void loadK(Instruction instr, LuaVM vm);
// LOADKX
void loadKX(Instruction instr, LuaVM vm);
// LEN
void len(Instruction instr, LuaVM vm);
// CONCAT
void concat(Instruction instr, LuaVM vm);
// NOT
void _not(Instruction instr, LuaVM vm);

void testset(Instruction instr, LuaVM vm);

void test(Instruction instr, LuaVM vm);

void forPrep(Instruction instr, LuaVM vm);

void forLoop(Instruction instr, LuaVM vm);

void _binaryArith(Instruction instr, LuaVM vm, ArithOp op);

void _unaryArith(Instruction instr, LuaVM vm, ArithOp op);

void _compare(Instruction instr, LuaVM vm, CompareOp op);

#endif //LUA_VM_MISC_H
