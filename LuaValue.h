//
// Created by Administrator on 2024/4/23.
//

#ifndef LUA_VM_LUAVALUE_H
#define LUA_VM_LUAVALUE_H

#include <iostream>
#include "consts.h"

using namespace std;

typedef int lua_t ;

class LuaValue {
public:
    virtual LuaType type() const = 0;
    virtual string to_string() const = 0;
};

class LuaValueNil: public LuaValue {
public:
    LuaType type() const override;
    string to_string() const override;
};

class LuaValueInt: public LuaValue {
private:
    int item;
public:
    LuaValueInt(int val);
    LuaType type() const override;
    string to_string() const override;
};

class LuaValueNumber: public LuaValue {
private:
    double item;
public:
    LuaValueNumber(double val);
    LuaType type() const override;
    string to_string() const override;
};

class LuaValueBool: public LuaValue {
private:
    bool item;
public:
    LuaValueBool(bool val);
    LuaType type() const override;
    string to_string() const override;
};

class LuaValueString: public LuaValue {
private:
    std::string item;
public:
    LuaValueString(std::string val);
    LuaType type() const override;
    string to_string() const override;
};



#endif //LUA_VM_LUAVALUE_H
