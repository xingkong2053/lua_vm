//
// Created by Administrator on 2024/4/25.
//

#include "Operator.h"
#include <utility>
#include "../arithmetic/number.h"
#include "../consts.h"
#include <cmath>

Operator::Operator(IntegerFunc iFunc, DoubleFunc dFunc): integerFunc(std::move(iFunc)),doubleFunc(std::move(dFunc)) {}

Operator getOperator(ArithOp op) {
    switch (op) {
        case ArithOp::LUA_OPADD:
            return {[](int a, int b) { return a + b; }, [](double a, double b) { return a + b; }};
        case ArithOp::LUA_OPSUB:
            return {[](int a, int b) { return a - b; }, [](double a, double b) { return a - b; }};
        case ArithOp::LUA_OPMUL:
            return {[](int a, int b) { return a * b; }, [](double a, double b) { return a * b; }};
        case ArithOp::LUA_OPMOD:
            return {iMod, dMod};
        case ArithOp::LUA_OPPOW:
            return {nullptr, [](double a, double b) { return std::pow(a, b);}};
        case ArithOp::LUA_OPDIV:
            return {nullptr, [](double a, double b) { return a / b;}};
        case ArithOp::LUA_OPIDIV:
            return {iFloorDiv, dFloorDiv};

            /* 位运算 */
        case ArithOp::LUA_OPBAND:
            return {[](int a, int b) { return a & b; }, nullptr};
        case ArithOp::LUA_OPBOR:
            return {[](int a, int b) { return a | b; }, nullptr};
        case ArithOp::LUA_OPBXOR:
            return {[](int a, int b) { return a ^ b; }, nullptr};
        case ArithOp::LUA_OPSHL:
            return {[](int a, int b) { return a << b; }, nullptr};
        case ArithOp::LUA_OPSHR:
            return {[](int a, int b) { return a >> b; }, nullptr};
        case ArithOp::LUA_OPUNM:
            return {[](int a, int b) { return -a; }, [](double a, double b) { return -a; }};
        case ArithOp::LUA_OPBNOT:
            return {[](int a, int b) { return int(~a); }, nullptr};
        default:
            return {nullptr, nullptr};

    }
}
