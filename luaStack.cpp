//
// Created by Administrator on 2024/4/23.
//

#include "luaStack.h"
#include <memory>
#include "utils.h"
#include <algorithm>

luaStack::luaStack(int size) {
    slots = make_shared<Stack>(size, make_shared<LuaValueNil>());
    top = 0;
}

void luaStack::check(int n) {
    auto free = slots->size() - top;
    for(auto i = free; i < n; i++) {
        slots->push_back(make_shared<LuaValueNil>());
    }
}

void luaStack::push(shared_ptr<LuaValue> val) {
    if(top == slots -> size()) {
        panic("stack overflow");
    }
    slots -> at(top) = val;
    top ++;
}

shared_ptr<LuaValue> luaStack::pop() {
    if(top < 1){
        panic("stack underflow");
    }
    top --;
    auto ret = slots ->at(top);
    slots -> at(top) = shared_ptr<LuaValueNil>();
    return ret;
}

int luaStack::absIndex(int idx) const {
    if(idx >= 0) {
        return (size_t)idx;
    }
    return size_t(top + idx + 1);
}

bool luaStack::isValid(int idx) const {
    auto _idx = absIndex(idx);
    return _idx > 0 && _idx <= top;
}

shared_ptr<LuaValue> luaStack::get(int idx) {
    if(!isValid(idx)) {
        panic("invalid index!");
    }
    auto _idx = absIndex(idx);
    return slots ->at(_idx - 1);
}

void luaStack::set(int idx, shared_ptr<LuaValue> val) {
    if(!isValid(idx)) {
        panic("invalid index!");
    }
    auto _idx = absIndex(idx);
    slots->at(_idx - 1) = val;
}

void luaStack::reverse(int s, int e) {
    auto si = slots -> begin() + s, ei = slots -> begin() + e + 1;
    std::reverse(si, ei);
}
