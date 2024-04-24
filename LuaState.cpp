//
// Created by Administrator on 2024/4/23.
//

#include "LuaState.h"
#include "utils.h"

LuaState::LuaState() {
    stack = make_shared<luaStack>(20);
}

int LuaState::GetTop() const {
    return stack -> top;
}

int LuaState::AbsIndex(int idx) const {
    return stack->absIndex(idx);
}

bool LuaState::Checkstack(int n) {
    stack ->check(n);
    return true;
}

void LuaState::Pop(int n) {
    for(auto i = 0; i < n; i++ ){
        stack -> pop();
    }
}

void LuaState::Copy(int fromIdx, int toIdx) {
    auto val = stack ->get(fromIdx);
    stack->set(toIdx, val);
}

void LuaState::PushValue(int idx) {
    auto val = stack ->get(idx);
    stack ->push(val);
}

void LuaState::Replace(int idx) {
    auto val = stack -> pop();
    stack ->set(idx, val);
}

void LuaState::Rotate(int idx, int n) {
    auto top = stack -> top - 1; // -1 对应vector索引
    auto bottom = stack ->absIndex(idx) - 1;
    auto pivot = n >= 0 ? top - n: bottom - n -1;
    stack ->reverse(bottom, pivot);
    stack ->reverse(pivot + 1, top);
    stack ->reverse(bottom, top);
}

void LuaState::Insert(int idx) {
    Rotate(idx, 1);
}

void LuaState::Remove(int idx) {
    Rotate(idx, -1);
    stack -> pop();
}

void LuaState::SetTop(int idx) {
    auto absI = AbsIndex(idx);
    if(absI < 0) { panic("stack underflow"); }
    auto diff = GetTop() - absI;
    if(diff > 0) {
        Pop(diff);
    } else {
        for(auto i = 0; i > diff; i--) {
            PushNil();
        }
    }
}

void LuaState::PushNil() {
    stack ->push(make_shared<LuaValueNil>());
}

void LuaState::PushBoolean(bool b) {
    stack ->push(make_shared<LuaValueBool>(b));
}

void LuaState::PushInteger(int n) {
    stack ->push(make_shared<LuaValueInt>(n));
}

void LuaState::PushNumber(double n) {
    stack ->push(make_shared<LuaValueNumber>(n));
}

void LuaState::PushString(const string &s) {
    stack ->push(make_shared<LuaValueString>(s));
}

void LuaState::Debug() {
    auto top = GetTop();
    if(!top) {
        cout << "<empty>" << endl;
        return;
    }
    string log;
    for (int i = 1; i <= top; ++i) {
        auto val = stack ->get(i);
        auto typ = val -> type();
        switch (typ) {
            case LuaType::LUA_TBOOLEAN:
            case LuaType::LUA_TNUMBER:
            case LuaType::LUA_TSTRING:
                log.append("[");
                log.append(val -> to_string());
                log.append("]");
                break;
            default:
                log.append("[<");
                log.append(TypeName(typ));
                log.append(">]");
        }
    }
    cout << log << endl;
}

std::string LuaState::TypeName(LuaType tp) const {
    switch (tp) {
        case LuaType::LUA_TNONE:
            return "no_value";
        case LuaType::LUA_TNIL:
            return "nil";
        case LuaType::LUA_TBOOLEAN:
            return "boolean";
        case LuaType::LUA_TLIGHTUSERDATA:
            return "user_data";
        case LuaType::LUA_TNUMBER:
            return "number";
        case LuaType::LUA_TSTRING:
            return "string";
        case LuaType::LUA_TTABLE:
            return "table";
        case LuaType::LUA_TFUNCTION:
            return "function";
        case LuaType::LUA_TUSERDATA:
            return "user_data";
        case LuaType::LUA_TTHREAD:
            return "thread";
    }
}
