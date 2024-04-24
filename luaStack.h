//
// Created by Administrator on 2024/4/23.
//

#ifndef LUA_VM_LUASTACK_H
#define LUA_VM_LUASTACK_H


#include <vector>
#include <memory>
#include "LuaValue.h"

typedef vector<shared_ptr<LuaValue>> Stack;

class luaStack {
public:
    // 栈容器
    shared_ptr<Stack> slots;
public:
    // 栈顶
    int top;
    luaStack(int size);
//    检查栈的空闲空间是否还能容纳n个值，如果不能，则扩容
    void check(int n);
//
    void push(shared_ptr<LuaValue> val);
    shared_ptr<LuaValue> pop();
//    将相对索引转化为绝对索引
    int absIndex(int idx) const;
    bool isValid(int idx) const;
    shared_ptr<LuaValue> get(int idx);
    void set(int idx, shared_ptr<LuaValue> val);
    void reverse(int s, int e);
};


#endif //LUA_VM_LUASTACK_H
