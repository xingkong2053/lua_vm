//
// Created by Administrator on 2024/4/26.
//

#ifndef LUA_VM_INSTRUCTION_H
#define LUA_VM_INSTRUCTION_H
#include <string>
#include <functional>
#include "../lua_state/LuaState.h"

using namespace std;

/* 指令编码模式 */
enum OpMode {
    IABC,
    IABx,
    IAsBx,
    IAx,
};

/* 指令操作码 47 */
enum OpName{
    OP_MOVE,        OP_LOADK,       OP_LOADKX,      OP_LOADBOOL,
    OP_LOADNIL,     OP_GETUPVAL,    OP_GETTABUP,    OP_GETTABLE,
    OP_SETTABUP,    OP_SETUPVAL,    OP_SETTABLE,    OP_NEWTABLE,
    OP_SELF,        OP_ADD,         OP_SUB,
    OP_MUL,         OP_MOD,         OP_POW,         OP_DIV,
    OP_IDIV,        OP_BAND,        OP_BOR,         OP_BXOR,
    OP_SHL,         OP_SHR,         OP_UNM,         OP_BNOT,
    OP_NOT,         OP_LEN,         OP_CONCAT,      OP_JMP,
    OP_EQ,          OP_LT,          OP_LE,          OP_TEST,
    OP_TESTSET,     OP_CALL,        OP_TAILCALL,    OP_RETURN,
    OP_FORLOOP,     OP_FORPREP,     OP_TFORCALL,    OP_TFORLOOP,
    OP_SETLIST,     OP_CLOSURE,     OP_VARARG,      OP_EXTRAARG,
};

enum OpArgType {
    /* not used */
    OpArgN,
    /* used */
    OpArgU,
    /* registers or a jump offset*/
    OpArgR,
    /* constant or register/constant */
    OpArgK,
};

/* 18位无符号数最大值 */
const int MAXARG_Bx = (1 << 18) - 1;
/* 18位有符号数最大值 */
const int MAXARG_sBx = MAXARG_Bx >> 1;

class Instruction {
private:
    unsigned int instr;
public:
    explicit Instruction(unsigned int instr);
    int getOpCode() const;
    std::tuple<int, int, int> getABC() const;
    pair<int, int> getABx() const;
    pair<int, int> getAsBx() const;
    int getAx() const;
    string getName() const;
    char getOpMode() const;
    char getArgBMode() const;
    char getArgCMode() const;
};


#endif //LUA_VM_INSTRUCTION_H
