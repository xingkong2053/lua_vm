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

#endif //LUA_VM_CONSTS_H
