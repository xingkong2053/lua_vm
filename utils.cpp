//
// Created by Administrator on 2024/4/23.
//

#include "utils.h"
#include<iostream>

void panic(std::string str) {
    std::cerr << str << std::endl;
    exit(-1);
}

std::string vec_to_str(const std::vector<char> &v){
    return { v.begin(), v.end() };
}