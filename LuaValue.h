//
// Created by Administrator on 2024/4/23.
//

#ifndef LUA_VM_LUAVALUE_H
#define LUA_VM_LUAVALUE_H

#include <iostream>
#include <memory>
#include "consts.h"

using namespace std;

typedef int lua_t ;

class LuaValue {
public:
    virtual LuaType type() const = 0;
    virtual string to_string() const = 0;
    virtual pair<int, bool> to_int() const = 0;
    virtual pair<double, bool> to_double() const {
        return {0, false};
    }
    virtual bool to_bool() const = 0;

    /* 比较方法 */
    /* 相等 */
    static bool eq(const shared_ptr<LuaValue>& a, const shared_ptr<LuaValue>& b);
    /* 小于 */
    static bool lt(const shared_ptr<LuaValue>& a, const shared_ptr<LuaValue>& b);
    /* 小于等于 */
    static bool le(const shared_ptr<LuaValue>& a, const shared_ptr<LuaValue>& b);
};

class LuaValueNil: public LuaValue {
public:
    LuaType type() const override;
    string to_string() const override;
    pair<int, bool> to_int() const override;
    bool to_bool() const override;
};

class LuaValueInt: public LuaValue {
private:
    long item;
public:
    LuaValueInt(int val);
    LuaType type() const override;
    string to_string() const override;
    pair<int, bool> to_int() const override;
    pair<double, bool> to_double() const override;
    bool to_bool() const override;
};

class LuaValueNumber: public LuaValue {
private:
    double item;
public:
    LuaValueNumber(double val);
    LuaType type() const override;
    string to_string() const override;
    pair<int, bool> to_int() const override;
    pair<double, bool> to_double() const override;
    bool to_bool() const override;
};

class LuaValueBool: public LuaValue {
private:
    bool item;
public:
    LuaValueBool(bool val);
    LuaType type() const override;
    string to_string() const override;
    pair<int, bool> to_int() const override;
    bool to_bool() const override;
};

class LuaValueString: public LuaValue {
private:
    std::string item;
public:
    LuaValueString(std::string val);
    LuaType type() const override;
    string to_string() const override;
    pair<int, bool> to_int() const override;
    pair<double, bool> to_double() const override;
    bool to_bool() const override;
};



#endif //LUA_VM_LUAVALUE_H
