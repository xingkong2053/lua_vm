//
// Created by Administrator on 2024/4/23.
//

#ifndef LUA_VM_CONSTS_H
#define LUA_VM_CONSTS_H

enum class LuaType {
    LUA_TNONE = -1,
    LUA_TNIL,
    LUA_TBOOLEAN,
    LUA_TLIGHTUSERDATA,
    LUA_TNUMBER,
    LUA_TSTRING,
    LUA_TTABLE,
    LUA_TFUNCTION,
    LUA_TUSERDATA,
    LUA_TTHREAD
};

enum class ArithOp {
    LUA_OPADD,          // +
    LUA_OPSUB,          // -
    LUA_OPMUL,          // *
    LUA_OPMOD,          // %
    LUA_OPPOW,          // ^
    LUA_OPDIV,          // /
    LUA_OPIDIV,         // //
    LUA_OPBAND,         // &
    LUA_OPBOR,          // |
    LUA_OPBXOR,         // ~
    LUA_OPSHL,          // <<
    LUA_OPSHR,          // >>
    LUA_OPUNM,          // -
    LUA_OPBNOT          // ~
};

enum class CompareOp {
    LUA_OPEQ,           // ==
    LUA_OPLT,           // <
    LUA_OPLE            // <=
};

#endif //LUA_VM_CONSTS_H
