//
// Created by Administrator on 2024/4/25.
//

#ifndef LUA_VM_BINARYCHUNK_H
#define LUA_VM_BINARYCHUNK_H

#include <string>

struct Header {
    char signature[4];
    char version[4];
    char format[4];
    char luaData[6];
    char cintSize;
    char sizetSize;
    char instructionSize;
    char luaIntegerSize;
    char luaNumberSize;
    int luacInt;
    int luacNum;
};

class BinaryChunk: public Header {
    /* 主函数Upvalue数量 */
    char sizeUpvalues;

};


#endif //LUA_VM_BINARYCHUNK_H
