//
// Created by Administrator on 2024/4/25.
//

#ifndef LUA_VM_PRINT_H
#define LUA_VM_PRINT_H
#include "Reader.h"
#include <memory>

void list(const shared_ptr<Prototype>& p);
void printHeader(const shared_ptr<Prototype>& p);
void printCode(const shared_ptr<Prototype>& p);


#endif //LUA_VM_PRINT_H