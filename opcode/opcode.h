//
// Created by Administrator on 2024/4/26.
//

#ifndef LUA_VM_OPCODE_H
#define LUA_VM_OPCODE_H

#include <utility>

#include "Instruction.h"
#include "instructions/misc.h"
#include "../consts.h"

struct opcode {
    char testFlag;
    char setAFlag;
    OpArgType argBMode;
    OpArgType argCMode;
    OpMode opMode;
    string name;
    function<void (Instruction, LuaVM)> action;
};
/* 和 OpName保持一致 */
const opcode opcodes[] = {
        {0, 1, OpArgR, OpArgN, IABC /* */, "MOVE    ", [](Instruction i, LuaVM vm){ move(i, vm);} }, // R(A) := R(B)
        {0, 1, OpArgK, OpArgN, IABx /* */, "LOADK   ", [](Instruction i, LuaVM vm){ loadK(i, vm);} }, // R(A) := Kst(Bx)
        {0, 1, OpArgN, OpArgN, IABx /* */, "LOADKX  ", [](Instruction i, LuaVM vm){ loadKX(i, vm);} }, // R(A) := Kst(extra arg)
        {0, 1, OpArgU, OpArgU, IABC /* */, "LOADBOOL", [](Instruction i, LuaVM vm){ loadBool(i, vm);} }, // R(A) := (bool)B; if (C) pc++
        {0, 1, OpArgU, OpArgN, IABC /* */, "LOADNIL ", [](Instruction i, LuaVM vm){ loadNil(i, vm);}}, // R(A), R(A+1), ..., R(A+B) := nil
        {0, 1, OpArgU, OpArgN, IABC /* */, "GETUPVAL", [](Instruction i, LuaVM vm){}}, // R(A) := UpValue[B]
        {0, 1, OpArgU, OpArgK, IABC /* */, "GETTABUP", [](Instruction i, LuaVM vm){}}, // R(A) := UpValue[B][RK(C)]
        {0, 1, OpArgR, OpArgK, IABC /* */, "GETTABLE", [](Instruction i, LuaVM vm){}}, // R(A) := R(B)[RK(C)]
        {0, 0, OpArgK, OpArgK, IABC /* */, "SETTABUP", [](Instruction i, LuaVM vm){}}, // UpValue[A][RK(B)] := RK(C)
        {0, 0, OpArgU, OpArgN, IABC /* */, "SETUPVAL", [](Instruction i, LuaVM vm){}}, // UpValue[B] := R(A)
        {0, 0, OpArgK, OpArgK, IABC /* */, "SETTABLE", [](Instruction i, LuaVM vm){}}, // R(A)[RK(B)] := RK(C)
        {0, 1, OpArgU, OpArgU, IABC /* */, "NEWTABLE", [](Instruction i, LuaVM vm){}}, // R(A) := {} (size = B,C)
        {0, 1, OpArgR, OpArgK, IABC /* */, "SELF    ", [](Instruction i, LuaVM vm){}}, // R(A+1) := R(B); R(A) := R(B)[RK(C)]
        {0, 1, OpArgK, OpArgK, IABC /* */, "ADD     ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPADD);}}, // R(A) := RK(B) + RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "SUB     ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPSUB);}}, // R(A) := RK(B) - RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "MUL     ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPMUL);}}, // R(A) := RK(B) * RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "MOD     ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPMOD);}}, // R(A) := RK(B) % RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "POW     ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPPOW);}}, // R(A) := RK(B) ^ RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "DIV     ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPDIV);}}, // R(A) := RK(B) / RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "IDIV    ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPIDIV);}}, // R(A) := RK(B) // RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "BAND    ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPBAND);}}, // R(A) := RK(B) & RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "BOR     ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPBOR);}}, // R(A) := RK(B) | RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "BXOR    ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPBXOR);}}, // R(A) := RK(B) ~ RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "SHL     ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPSHL);}}, // R(A) := RK(B) << RK(C)
        {0, 1, OpArgK, OpArgK, IABC /* */, "SHR     ", [](Instruction i, LuaVM vm){ _binaryArith(i, vm, ArithOp::LUA_OPSHR);}}, // R(A) := RK(B) >> RK(C)
        {0, 1, OpArgR, OpArgN, IABC /* */, "UNM     ", [](Instruction i, LuaVM vm){ _unaryArith(i, vm, ArithOp::LUA_OPUNM);}}, // R(A) := -R(B)
        {0, 1, OpArgR, OpArgN, IABC /* */, "BNOT    ", [](Instruction i, LuaVM vm){ _unaryArith(i, vm, ArithOp::LUA_OPBNOT);}}, // R(A) := ~R(B)
        {0, 1, OpArgR, OpArgN, IABC /* */, "NOT     ", [](Instruction i, LuaVM vm){ _not(i, vm);}}, // R(A) := not R(B)
        {0, 1, OpArgR, OpArgN, IABC /* */, "LEN     ", [](Instruction i, LuaVM vm){ len(i, vm); }}, // R(A) := length of R(B)
        {0, 1, OpArgR, OpArgR, IABC /* */, "CONCAT  ", [](Instruction i, LuaVM vm){ concat(i, vm);}}, // R(A) := R(B).. ... ..R(C)
        {0, 0, OpArgR, OpArgN, IAsBx /**/, "JMP     ", [](Instruction i, LuaVM vm){ jmp(i, vm);} }, // pc+=sBx; if (A) close all upvalues >= R(A - 1)
        {1, 0, OpArgK, OpArgK, IABC /* */, "EQ      ", [](Instruction i, LuaVM vm){ _compare(i, vm, CompareOp::LUA_OPEQ);}}, // if ((RK(B) == RK(C)) ~= A) then pc++
        {1, 0, OpArgK, OpArgK, IABC /* */, "LT      ", [](Instruction i, LuaVM vm){ _compare(i, vm, CompareOp::LUA_OPLT);}}, // if ((RK(B) <  RK(C)) ~= A) then pc++
        {1, 0, OpArgK, OpArgK, IABC /* */, "LE      ", [](Instruction i, LuaVM vm){ _compare(i, vm, CompareOp::LUA_OPLE);}}, // if ((RK(B) <= RK(C)) ~= A) then pc++
        {1, 0, OpArgN, OpArgU, IABC /* */, "TEST    ", [](Instruction i, LuaVM vm){ test(i, vm);}}, // if not (R(A) <=> C) then pc++
        {1, 1, OpArgR, OpArgU, IABC /* */, "TESTSET ", [](Instruction i, LuaVM vm){ testset(i, vm);}}, // if (R(B) <=> C) then R(A) := R(B) else pc++
        {0, 1, OpArgU, OpArgU, IABC /* */, "CALL    ", [](Instruction i, LuaVM vm){}}, // R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
        {0, 1, OpArgU, OpArgU, IABC /* */, "TAILCALL", [](Instruction i, LuaVM vm){}}, // return R(A)(R(A+1), ... ,R(A+B-1))
        {0, 0, OpArgU, OpArgN, IABC /* */, "RETURN  ", [](Instruction i, LuaVM vm){}}, // return R(A), ... ,R(A+B-2)
        {0, 1, OpArgR, OpArgN, IAsBx /**/, "FORLOOP ", [](Instruction i, LuaVM vm){ forLoop(i, vm);}  }, // R(A)+=R(A+2); if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }
        {0, 1, OpArgR, OpArgN, IAsBx /**/, "FORPREP ", [](Instruction i, LuaVM vm){ forPrep(i, vm);}  }, // R(A)-=R(A+2); pc+=sBx
        {0, 0, OpArgN, OpArgU, IABC /* */, "TFORCALL", [](Instruction i, LuaVM vm){}  }, // R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2));
        {0, 1, OpArgR, OpArgN, IAsBx /**/, "TFORLOOP", [](Instruction i, LuaVM vm){}  }, // if R(A+1) ~= nil then { R(A)=R(A+1); pc += sBx }
        {0, 0, OpArgU, OpArgU, IABC /* */, "SETLIST ", [](Instruction i, LuaVM vm){}  }, // R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B
        {0, 1, OpArgU, OpArgN, IABx /* */, "CLOSURE ", [](Instruction i, LuaVM vm){}  }, // R(A) := closure(KPROTO[Bx])
        {0, 1, OpArgU, OpArgN, IABC /* */, "VARARG  ", [](Instruction i, LuaVM vm){}  }, // R(A), R(A+1), ..., R(A+B-2) = vararg
        {0, 0, OpArgU, OpArgU, IAx /*  */, "EXTRAARG", [](Instruction i, LuaVM vm){}  }, // extra (larger) argument for previous opcode
};
#endif //LUA_VM_OPCODE_H
