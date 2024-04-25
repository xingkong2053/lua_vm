//
// Created by Administrator on 2024/4/25.
//

#ifndef LUA_VM_OPERATOR_H
#define LUA_VM_OPERATOR_H
#include <functional>
#include "../consts.h"

typedef std::function<int (int, int)> IntegerFunc;
typedef std::function<double (double, double)> DoubleFunc;

class Operator {
public:
    IntegerFunc integerFunc;
    DoubleFunc doubleFunc;
public:
    Operator(IntegerFunc iFunc, DoubleFunc dFunc);
};

Operator getOperator(ArithOp op);


#endif //LUA_VM_OPERATOR_H
