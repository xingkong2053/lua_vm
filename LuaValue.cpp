//
// Created by Administrator on 2024/4/23.
//

#include "LuaValue.h"

#include <utility>
#include "consts.h"

LuaType LuaValueInt::type() const {
    return LuaType::LUA_TNUMBER;
}

LuaValueInt::LuaValueInt(int val) : item(val) {

}

string LuaValueInt::to_string() const {
    return std::to_string(item);
}

pair<int, bool> LuaValueInt::to_int() const {
    return {item, true};
}

pair<double, bool> LuaValueInt::to_double() const {
    return { static_cast<double>(item), true };
}

bool LuaValueInt::to_bool() const {
    return item != 0;
}

LuaType LuaValueNil::type() const {
    return LuaType::LUA_TNIL;
}

string LuaValueNil::to_string() const {
    return {};
}

pair<int, bool> LuaValueNil::to_int() const {
    return {0, false};
}

bool LuaValueNil::to_bool() const {
    return false;
}

LuaType LuaValueNumber::type() const {
    return LuaType::LUA_TNUMBER;
}

LuaValueNumber::LuaValueNumber(double val): item(val) {

}

string LuaValueNumber::to_string() const {
    auto [val, ok] = to_int();
    return std::to_string(ok ? val: item);
}

pair<int, bool> LuaValueNumber::to_int() const {
    int val = static_cast<int>(item);
    return { val, val == item };
}

pair<double, bool> LuaValueNumber::to_double() const {
    return {item, true};
}

bool LuaValueNumber::to_bool() const {
    return item > 0 || item < 0;
}

LuaValueBool::LuaValueBool(bool val): item(val) {

}

LuaType LuaValueBool::type() const {
    return LuaType::LUA_TBOOLEAN;
}

string LuaValueBool::to_string() const {
    return item ? "true": "false";
}

pair<int, bool> LuaValueBool::to_int() const {
    return {0, false};
}

bool LuaValueBool::to_bool() const {
    return item;
}

LuaValueString::LuaValueString(std::string val): item(std::move(val)) {

}

LuaType LuaValueString::type() const {
    return LuaType::LUA_TSTRING;
}

string LuaValueString::to_string() const {
    return item;
}

pair<int, bool> LuaValueString::to_int() const {
    try {
        return { std::stoi(item), true };
    } catch (std::exception &e) {
        return { 0, false };
    }
}

pair<double, bool> LuaValueString::to_double() const {
    try {
        return { std::stod(item), true };
    } catch (std::exception &e) {
        return { 0, false };
    }
}

bool LuaValueString::to_bool() const {
    return !item.empty();
}

bool LuaValue::eq(const shared_ptr<LuaValue>& a, const shared_ptr<LuaValue>& b) {
    switch (a->type()) {
        case LuaType::LUA_TNIL:
            return b->type() == LuaType::LUA_TNIL;
        case LuaType::LUA_TNUMBER: {
            auto [a_int, ok] = a->to_int();
            auto [b_int, ok2] = b->to_int();
            if (ok && ok2) {
                return a_int == b_int;
            }
            auto [a_double, ok3] = a->to_double();
            auto [b_double, ok4] = b->to_double();
            if (ok3 && ok4) {
                return a_double == b_double;
            }
            return false;
        }
        case LuaType::LUA_TBOOLEAN:
            return a->to_bool() == b->to_bool();
        case LuaType::LUA_TSTRING:
            return a->to_string() == b->to_string();
        default:
            return a == b;
    }
}

bool LuaValue::lt(const shared_ptr<LuaValue>& a, const shared_ptr<LuaValue>& b) {
    /* 及支持数字及字符串类型的比较 */
    switch (a->type()) {
        case LuaType::LUA_TNUMBER: {
            auto [a_int, ok] = a->to_int();
            auto [b_int, ok2] = b->to_int();
            if (ok && ok2) {
                return a_int < b_int;
            }
            auto [a_double, ok3] = a->to_double();
            auto [b_double, ok4] = b->to_double();
            if (ok3 && ok4) {
                return a_double < b_double;
            }
        }
        case LuaType::LUA_TSTRING:
            return a->to_string() < b->to_string();
        default:;
    }
    throw runtime_error("invalid compare");
}

bool LuaValue::le(const shared_ptr<LuaValue> &a, const shared_ptr<LuaValue> &b) {
    /* 及支持数字及字符串类型的比较 */
    switch(a->type()) {
        case LuaType::LUA_TNUMBER: {
            auto [a_int, ok] = a->to_int();
            auto [b_int, ok2] = b->to_int();
            if (ok && ok2) {
                return a_int <= b_int;
            }
            auto [a_double, ok3] = a->to_double();
            auto [b_double, ok4] = b->to_double();
            if (ok3 && ok4) {
                return a_double <= b_double;
            }
        }
        case LuaType::LUA_TSTRING:
            return a->to_string() <= b->to_string();
        default:;
    }
    throw runtime_error("invalid compare");
}
