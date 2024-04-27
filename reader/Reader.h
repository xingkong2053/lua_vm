//
// Created by Administrator on 2024/4/25.
//

#ifndef LUA_VM_READER_H
#define LUA_VM_READER_H
#include <vector>
#include <cstdint>
#include "../utils.h"
#include <memory>
#include "../LuaValue.h"

using namespace std;

const string LUA_SIGNATURE = "\x1bLua"; /* ESC L u a*/
const char LUA_VERSION = 0x53;
const int LUAC_FORMAT = 0;
const string LUAC_DATA = "\x19\x93\r\n\x1a\n";
const int LUA_INTEGER_SIZE = 8;
const int LUA_NUMBER_SIZE = 8;
const int LUA_INSTRUCTION_SIZE = 4;
const int LUA_CINT_SIZE = 4;
const int LUA_CSIZET_SIZE = 8;
const int LUA_LUACINT_SIZE = 8;
const int LUA_LUACNUM_SIZE = 8;
const int LUAC_INT = 0x5678;
const double LUAC_NUM = 370.5;

enum class ValTag {
    NIL         = 0x00,
    BOOLEAN     = 0x01,
    NUMBER      = 0x03,
    INTEGER     = 0x13,
    LONG_STR    = 0x04,
    SHORT_STR   = 0x14,
};

struct Upvalue {
    char instack;
    char idx;
};

struct LocVar {
    string varName;
    unsigned int startPC;
    unsigned int endPC;
};

struct Prototype {
    string source;
    int tag;
    int line_defined;
    int last_line_defined;
    int num_params;
    int is_vararg;
    int max_stack_size;
    vector<unsigned int> code;
    vector<shared_ptr<LuaValue>> constants;
    vector<Upvalue> upvalues;
    vector<shared_ptr<Prototype>> prototypes;
    vector<int> line_info;
    vector<LocVar> loc_vars;
    vector<string> upvalue_names;
};


class Reader {
private :
    int cursor;
    string filePath;
public:
    /* 注意类型是unsigned char或 bytes */
    std::vector<unsigned char> data;
    streamsize size;
    Reader(const std::string& path);
    char readByte();
    /* 以小端序读取int32类型数据 */
    unsigned int readUInt();
    /* 以小端序读取int64类型数据 */
    unsigned long readUInt64();
    /* 读取double */
    double readDouble();
    /* 读取字符串 */
    string readString();
    /*  */
    vector<char> readBytes(size_t n);
    void checkHeader();

    /* 读取指令 */
    vector<unsigned int> readCode();
    shared_ptr<LuaValue> readConstant();
    vector<shared_ptr<LuaValue>> readConstants();
    vector<Upvalue> readUpvalues();
    shared_ptr<Prototype> readProto(string& parentSource);
    vector<shared_ptr<Prototype>> readProtos(string& parentSource);
    vector<int> readLineInfo();
    /* 读取局部标量表 */
    vector<LocVar> readLocVars();
    /* 读取upvalue名列表 */
    vector<string> readUpvalueNames();

    shared_ptr<Prototype> unDump();
};


#endif //LUA_VM_READER_H
