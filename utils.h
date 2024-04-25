//
// Created by Administrator on 2024/4/23.
//

#ifndef LUA_VM_UTILS_H
#define LUA_VM_UTILS_H
#include<string>
#include<vector>

void panic(std::string str);

std::string vec_to_str(const std::vector<char> &v);

#endif //LUA_VM_UTILS_H
