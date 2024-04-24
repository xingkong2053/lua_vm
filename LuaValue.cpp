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

LuaType LuaValueNil::type() const {
    return LuaType::LUA_TNIL;
}

string LuaValueNil::to_string() const {
    return {};
}

LuaType LuaValueNumber::type() const {
    return LuaType::LUA_TNUMBER;
}

LuaValueNumber::LuaValueNumber(double val): item(val) {

}

string LuaValueNumber::to_string() const {
    return std::to_string(item);
}

LuaValueBool::LuaValueBool(bool val): item(val) {

}

LuaType LuaValueBool::type() const {
    return LuaType::LUA_TBOOLEAN;
}

string LuaValueBool::to_string() const {
    return item ? "true": "false";
}

LuaValueString::LuaValueString(std::string val): item(std::move(val)) {

}

LuaType LuaValueString::type() const {
    return LuaType::LUA_TSTRING;
}

string LuaValueString::to_string() const {
    return string("\"") + item + "\"";
}
