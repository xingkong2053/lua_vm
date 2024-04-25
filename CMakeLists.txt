cmake_minimum_required(VERSION 3.27)
project(lua_vm)

set(CMAKE_CXX_STANDARD 17)

add_executable(lua_vm main.cpp
        consts.h
        LuaValue.cpp
        LuaValue.h
        luaStack.cpp
        luaStack.h
        utils.cpp
        utils.h
        lua_state/LuaState.cpp
        lua_state/LuaState.h
        arithmetic/number.cpp
        arithmetic/number.h
        lua_state/Operator.cpp
        lua_state/Operator.h
        binchunk/BinaryChunk.cpp
        binchunk/BinaryChunk.h
        binchunk/BinaryChunk.h
        reader/Reader.cpp
        reader/Reader.h
        reader/print.cpp
        reader/print.h)
