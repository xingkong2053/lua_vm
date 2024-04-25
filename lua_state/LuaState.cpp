//
// Created by Administrator on 2024/4/23.
//

#include "LuaState.h"

#include <utility>
#include "../utils.h"
#include "Operator.h"

LuaState::LuaState(int stackSize, string proto): proto(std::move(proto)), pc(0) {
    stack = make_shared<luaStack>(stackSize);
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
            case LuaType::LUA_TNUMBER: {
                log.append("[");
                auto str = val->to_string();
                log.append(str);
                log.append("]");
                break;
            }
            case LuaType::LUA_TSTRING: {
                log.append("[\"");
                log.append(val->to_string());
                log.append("\"]");
                break;
            }
            default: {
                log.append("[<");
                log.append(TypeName(typ));
                log.append(">]");
            }
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

void LuaState::Arith(ArithOp op) {
    shared_ptr<LuaValue> a, b;
    b = stack -> pop();
    if(op != ArithOp::LUA_OPUNM && op != ArithOp::LUA_OPBNOT) {
        a = stack -> pop();
    } else {
        a = b;
    }
    auto res = arith(a, b, getOperator(op));
    if(res) {
        stack -> push(res);
    } else{
        panic("arith error");
    }
}

void LuaState::Len(int idx) {
    auto val = stack ->get(idx);
    if(val -> type() == LuaType::LUA_TSTRING){
        stack ->push(make_shared<LuaValueInt>(val -> to_string().size()));
        return;
    }
    panic("cannot get value length");
}

void LuaState::Concat(int n) {
    if(n == 0) {
        stack ->push(make_shared<LuaValueString>(""));
    } else if(n >= 2){
        while(n-- > 1) {
            if(IsString(-1) && IsString(-2)) {
                auto a = stack->pop(), b = stack->pop();
                auto s = b -> to_string() + a -> to_string();
                stack ->push(make_shared<LuaValueString>(s));
            } else {
                throw runtime_error("concat error");
            }
        }
    }
    // n == 1, do nothing
}

bool LuaState::Compare(int idx1, int idx2, CompareOp op) {
    /* 从栈中获取操作数 */
    auto a = stack -> get(idx1);
    auto b = stack -> get(idx2);
    switch (op) {
        case CompareOp::LUA_OPEQ:
            return LuaValue::eq(a , b);
        case CompareOp::LUA_OPLT:
            return LuaValue::lt(a, b);
        case CompareOp::LUA_OPLE:
            return LuaValue::le(a, b);
        default:
            throw runtime_error("invalid compare operation");
    }
}

pair<string, bool> LuaState::ToStringX(int idx) const {
    auto val = stack->get(idx);
    auto type = val->type();
    if(type == LuaType::LUA_TNUMBER || type == LuaType::LUA_TSTRING) {
        return { val -> to_string(), true };
    }
    return { "", false };
}

string LuaState::ToString(int idx) const {
    auto [ str, ok ] = ToStringX(idx);
    return str;
}

bool LuaState::IsString(int idx) const {
    auto typ = stack->get(idx)->type();
    return typ == LuaType::LUA_TSTRING || typ == LuaType::LUA_TNUMBER;
}

int LuaState::PC() {
    return pc;
}

void LuaState::GetRK(int rk) {

}

void LuaState::AddPC(int n) {
    pc += n;
}

uint32_t LuaState::Fetch() {
    return 0;
}

void LuaState::GetConst(int idx) {

}



shared_ptr<LuaValue> arith(shared_ptr<LuaValue> a, shared_ptr<LuaValue> b, Operator op){
    if(!op.doubleFunc) {
        /* 位运算 */
        auto [ a_int, ok ] = a -> to_int();
        auto [ b_int, ok2 ] = b -> to_int();
        if(ok && ok2) {
            return make_shared<LuaValueInt>(op.integerFunc(a_int, b_int));
        }
    } else {
        if(op.integerFunc) { /* 尝试将值转换为int */
            auto [ a_int, ok ] = a -> to_int();
            auto [ b_int, ok2 ] = b -> to_int();
            if(ok && ok2) {
                return make_shared<LuaValueInt>(op.integerFunc(a_int, b_int));
            }
        }
        /* 不成功则尝试将值转换为浮点数 */
        auto [ a_num, ok ] = a -> to_double();
        auto [ b_num, ok2 ] = b -> to_double();
        if(ok && ok2) {
            return make_shared<LuaValueNumber>(op.doubleFunc(a_num, b_num));
        }
    }
    return nullptr;
}
