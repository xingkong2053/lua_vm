//
// Created by Administrator on 2024/4/25.
//

#include "print.h"
#include "../opcode/Instruction.h"

using namespace std;

void list(const shared_ptr<Prototype>& p, bool detail){
    printHeader(p);
    printCode(p);
    if(detail) printDetail(p);
    for(auto &sub : p->prototypes) {
        list(sub, detail);
    }
}

void printHeader(const shared_ptr<Prototype>& p) {
    string funcType("main");
    if(p->line_defined > 0) {
        funcType = "function";
    }
    string varargFlag;
    if(p->is_vararg) {
        varargFlag = "+";
    }
    printf("\n%s <%s:%d,%d> (%zu instructions)\n",
        funcType.c_str(), p->source.c_str(),p->line_defined, p->last_line_defined, p->code.size());
    printf("%d%s params, %d slots, %zu upvalues, ",
           p->num_params, varargFlag.c_str(),  p->max_stack_size, p->upvalues.size());
    printf("%zu locals, %zu constants, %zu functions\n",
           p->loc_vars.size(), p->constants.size(), p->prototypes.size());
}

void printCode(const shared_ptr<Prototype>& p) {
    for(int i = 0; i < p->code.size(); i++) {
        string line("-");
        if(p->line_info.size() > 0) {
            line = to_string(p->line_info[i]);
        }
        Instruction instr(p->code[i]);
        printf("\t%d\t[%s]\t%s\t", i + 1, line.c_str(), instr.getName().c_str());
        printOperands(instr);
        printf("\n");
    }
}

void printDetail(const shared_ptr<Prototype>& p){
    /* 常量表 */
    auto n = p->constants.size();
    printf("constants (%zu):\n", n);
    printf("\tidx\tvalue\n");
    for (int i = 0; i <n; ++i) {
        printf("\t%d\t%s\n", i + 1, p->constants[i]->to_string().c_str());
    }
    n = p->loc_vars.size();
    /* 局部变量表 */
    printf("locals (%zu):\n", n);
    printf("\tidx\tname\tstart\tend\n");
    for (int i = 0; i <n; ++i) {
        auto locVar = p->loc_vars[i];
        printf("\t%d\t%s\t%d\t%d\n", i + 1,
               locVar.varName.c_str(), locVar.startPC + 1, locVar.endPC + 1);
    }
    /* 闭包 */
    n = p->upvalues.size();
    printf("upvalues (%zu):\n", n);
    printf("\tidx\tname\tinstack\tidx\n");
    for (int i = 0; i <n; ++i) {
        auto upvalue = p->upvalues[i];
        string name("-");
        if(p->upvalue_names.size() > 0) {
            name = p->upvalue_names[i];
        }
        printf("\t%d\t%s\t%d\t%d\n", i + 1, name.c_str(), upvalue.instack, upvalue.idx);
    }
}

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

void printOperands(Instruction ins){
    switch (ins.getOpMode()) {
        case IABC: {
            auto [a, b, c] = ins.getABC();
            printf("%d\t", a);
            if(ins.getArgBMode() != OpArgN) {  // 在某些指令中b，c参数未使用
                printf("%d\t", b > 0xff ? -1-b&0xff /* 最高位为1表示常量表索引，按负数输出 */: b);
            }
            if(ins.getArgCMode() != OpArgN) {
                printf("%d\t", c > 0xff ? -1-c&0xff: c);
            }
            break;
        }
        case IABx: {
            auto [a, bx] = ins.getABx();
            printf("%d\t", a);
            if(ins.getArgBMode() == OpArgK) { /* 操作数Bx表示常量表索引 */
                printf("%d\t", -1 - bx);
            } else if(ins.getArgBMode() == OpArgU) {
                printf("%d\t", bx);
            }
            break;
        }
        case IAsBx: {
            auto [a, sbx] = ins.getAsBx();
            printf("%d\t%d", a, sbx);
            break;
        }
        case IAx: {
            auto ax = ins.getAx();
            printf("%d", -1 - ax);
            break;
        }
    }
}