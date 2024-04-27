//
// Created by Administrator on 2024/4/26.
//
#include "misc.h"

using namespace std;

/* 二元算数运算指令 */
void _binaryArith(Instruction instr, LuaVM vm, ArithOp op) {
    auto [a, b, c] = instr.getABC();
    vm ->GetRK(b);
    vm ->GetRK(c);
    vm ->Arith(op);
    vm ->Replace(a + 1);
}

/* 一元算数运算指令 */
void _unaryArith(Instruction instr, LuaVM vm, ArithOp op) {
    auto [a, b, _] = instr.getABC();
    vm ->PushValue(b + 1);
    vm ->Arith(op);
    vm ->Replace(a + 1);
}

void _compare(Instruction instr, LuaVM vm, CompareOp op) {
    // 比较寄存器或常量表中的两个值，并将比较结果和操作数a匹配
    auto [a, b, c] = instr.getABC();
    vm ->GetRK(b);
    vm ->GetRK(c);
    if(vm ->Compare(-2, -1, op) != (a != 0)){
        // 如果匹配，则跳过下一条指令
        vm ->AddPC(1);
    }
    vm ->Pop(2);
}

///* 二元逻辑运算指令 */
//void _binaryLogic(Instruction instr, LuaVM vm, ArithOp op) {
//    auto [a, b, c] = instr.getABC();
//    vm ->GetRK(b);
//    vm ->GetRK(c);
//    vm ->Arith(op);
//    vm ->Replace(a + 1);
//}
//
///* 二元比较运算指令 */
//void _binaryComp(Instruction instr, LuaVM vm, ArithOp op) {
//    auto [a, b, c] = instr.getABC();
//

void move(Instruction instr, LuaVM vm){
    auto [a/* dist register */, b/* source register */, c] = instr.getABC();
    vm -> Copy(b + 1, a + 1 /* 栈索引 */);
}

void jmp(Instruction instr, LuaVM vm){
    auto [a, bx] = instr.getAsBx();
    vm -> AddPC(bx);
    if(a!=0) {
        // 操作数A与upvalue有关
        throw runtime_error("todo");
    }
}

void loadNil(Instruction instr, LuaVM vm){
    /* a 寄存器起始索引，b 寄存器数量 */
    auto [a, b, c] = instr.getABC();
    a++;
    vm -> PushNil();
    for(int i = a; i <= a + b; ++i){
        vm->Copy(-1, i);
    }
    vm -> Pop(1);
}

void loadBool(Instruction instr, LuaVM vm){
    /* a register index, b value */
    /* 如果寄存器c非0则跳过下一条指令 */
    auto [a, b, c] = instr.getABC();
    vm ->PushBoolean(b != 0);
    vm ->Replace(a + 1);
    if(c != 0) {
        vm ->AddPC(1);
    }
}

void loadK(Instruction instr, LuaVM vm){
    auto [a, bx] = instr.getABx();
    vm ->GetConst(bx);
    vm ->Replace(a + 1);
}

void loadKX(Instruction instr, LuaVM vm){
    auto [a, _] = instr.getABx();
    // 从 EXTRAARG 指令中获取常量表索引
    auto ax = Instruction(vm -> Fetch()).getAx();
    vm ->GetConst(ax);
    vm ->Replace(a + 1);
}

void len(Instruction instr, LuaVM vm){
    auto [a, b, _] = instr.getABC();
    vm ->Len(b + 1);
    vm ->Replace(a + 1);
}

void concat(Instruction instr, LuaVM vm){
    // a 目标寄存器， b 开始索引， c 结束索引
    auto [a, b, c] = instr.getABC();
    auto n = c - b + 1;
    vm ->Checkstack(n);
    for(int i = b + 1; i <= c + 1; ++i){
        // 在concat前面一般都有push操作， 为什么这里还要push？
        vm ->PushValue(i);
    }
    vm ->Concat(n);
    vm ->Replace(a + 1);
}

void _not(Instruction instr, LuaVM vm){
    // 对应lua语言中的逻辑非
    auto [a, b, _] = instr.getABC();
    vm ->PushBoolean(!vm ->ToBoolean(b + 1));
    vm ->Replace(a + 1);
}

void testset(Instruction instr, LuaVM vm){
    auto [a, b, c] = instr.getABC();
    if(vm ->ToBoolean(b + 1) == (c!=0)){
        vm ->Copy(b + 1, a + 1);
    } else {
        vm ->AddPC(1);
    }
}

void test(Instruction instr, LuaVM vm){
    auto [a, _, c] = instr.getABC();
    /* 判断寄存器a中的值转换为bool值之后和操作数c所代表的布尔值是否一致 */
    if(vm ->ToBoolean(a + 1) == (c != 0)){
        vm ->AddPC(1);
    }
}

void forPrep(Instruction instr, LuaVM vm){
    auto [a, sBx] = instr.getAsBx();
    a++;
    vm ->PushValue(a);
    vm ->PushValue(a + 2);
    /* R(A) -= R(A+2) */
    vm ->Arith(ArithOp::LUA_OPSUB);
    vm ->Replace(a);
    /* 跳转到FORLOOP指令 */
    vm ->AddPC(sBx);
}

void forLoop(Instruction instr, LuaVM vm) {
    auto [a, sBx] = instr.getAsBx();
    a++;
    /* R(A) += R(A+2) */
    vm ->PushValue(a + 2);
    vm ->PushValue(a);
    vm ->Arith(ArithOp::LUA_OPADD);
    vm ->Replace(a);

    /* 判断步长是否为正数 */
    auto step = vm ->ToNumber(a + 2);
    if((step >=0 && vm->Compare(a, a+1, CompareOp::LUA_OPLE/* Ra <= Ra+1 */)) ||
    step < 0 && vm ->Compare(a+1, a, CompareOp::LUA_OPLE/* Ra >= Ra + 1 */)) {
        vm ->AddPC(sBx);
        vm ->Copy(a, a + 3); // 变量 i
    }
}