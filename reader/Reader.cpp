//
// Created by Administrator on 2024/4/25.
//

#include <string>
#include "Reader.h"
#include <fstream>
#include <iostream>
using namespace std;

Reader::Reader(const std::string& path): filePath(path) {
    ifstream file(path, ios::binary);
    if(!file.is_open()) {
        throw runtime_error("file open failed");
    };
    file.seekg(0, ios::end);
    auto fileSize = file.tellg();
    file.seekg(0, ios::beg);

    data = vector<char>(fileSize);
    size = fileSize;
    file.read(data.data(), fileSize);

    file.close();

    if(file.fail()) {
        throw runtime_error("file read failed");
    }
    cursor = 0;
}

char Reader::readByte() {
    if(cursor >= size) {
        throw runtime_error("read out of range");
    }
    return data[cursor++];
}

unsigned int Reader::readUInt() {
    if(cursor + 4 > size) {
        throw runtime_error("read out of range");
    }
    unsigned int result = 0, shift = 0;
    for(auto i = cursor; i < cursor + 4; i++) {
        result = result + (int)(data[i] << shift);
        shift += 8;
    }
    cursor += 4;
    return result;
}

unsigned long Reader::readUInt64() {
    if(cursor + 8 > size) {
        throw runtime_error("read out of range");
    }
    unsigned long result = 0; int shift = 0;
    for(auto i = cursor; i < cursor + 8; i++) {
        result = result + ((long)data[i] << shift);
        shift += 8;
    }
    cursor += 8;
    return result;
}

double Reader::readDouble() {
    auto size_d = sizeof(double);
    if(cursor + size_d > size){
        throw runtime_error("read out of range");
    }
    double result;
    memcpy(&result, data.data() + cursor, size_d);
    cursor += int(size_d);
    return result;
}

string Reader::readString() {
    auto len = size_t(readByte());
    if(!len) {
        return "";
    }
    if (len == 0xff) {
        len = readUInt64();
    }
    auto bytes = readBytes(len - 1);
    return {(char *)bytes.data(), bytes.size()};
}

vector<char> Reader::readBytes(size_t n) {
    if(cursor + n > size) {
        throw runtime_error("read out of range");
    }
    vector<char> result(n);
    for(int i = 0; i < n; i++) {
        result[i] = data[cursor++];
    }
    return result;
}

void Reader::checkHeader() {
    if(vec_to_str(readBytes(4)) != LUA_SIGNATURE) {
        throw runtime_error("not a lua file");
    } else if(readByte() != LUA_VERSION) {
        throw runtime_error("version not supported");
    } else if(readByte() != LUAC_FORMAT) {
        throw runtime_error("format not supported");
    } else if(vec_to_str(readBytes(6)) != LUAC_DATA) {
        throw runtime_error("corrupted");
    } else if(readByte() != LUA_CINT_SIZE) {
        throw runtime_error("int size not supported");
    } else if(readByte() != LUA_CSIZET_SIZE) {
        throw runtime_error("size_t size not supported");
    } else if(readByte() != LUA_INSTRUCTION_SIZE) {
        throw runtime_error("instruction size not supported");
    } else if(readByte() != LUA_INTEGER_SIZE) {
        throw runtime_error("lua_Integer size not supported");
    } else if(readByte() != LUA_NUMBER_SIZE) {
        throw runtime_error("lua_Number size not supported");
    } else if(readUInt64() != LUAC_INT) {
        throw runtime_error("endianness not supported");
    } else if(readDouble() != LUAC_NUM) {
        throw runtime_error("float format not supported");
    }
}

vector<unsigned int> Reader::readCode() {
    auto len = readUInt();
    vector<unsigned int> codes(len);
    for(int i = 0; i < len; i++) {
        codes[i] = readUInt();
    }
    return codes;
}

shared_ptr<LuaValue> Reader::readConstant() {
    auto tag = readByte();
    switch((ValTag)tag) {
        case ValTag::NIL: return make_shared<LuaValueNil>();
        case ValTag::BOOLEAN: return make_shared<LuaValueBool>(readByte() != 0);
        case ValTag::INTEGER: return make_shared<LuaValueInt>((long)readUInt64());
        case ValTag::NUMBER: return make_shared<LuaValueNumber>(readDouble());
        case ValTag::SHORT_STR:
        case ValTag::LONG_STR: return make_shared<LuaValueString>(readString());
    }
}

vector<shared_ptr<LuaValue>> Reader::readConstants() {
    auto n = readUInt();
    vector<shared_ptr<LuaValue>> constants(n);
    for(int i = 0; i < n; i++) {
        constants[i] = readConstant();
    }
    return constants;
}

vector<Upvalue> Reader::readUpvalues() {
    auto n = readUInt();
    if(!n) return {};
    vector<Upvalue> upvalues(n);
    for(int i = 0; i < n; i++) {
        upvalues[i].instack = readByte();
        upvalues[i].idx = readByte();
    }
    return upvalues;
}

shared_ptr<Prototype> Reader::readProto(string &parentSource) {
    auto source = readString();
    if(source.empty()) {
        source = parentSource;
    }
    auto res = make_shared<Prototype>();
    res->source = source;
    res->line_defined = readUInt();
    res->last_line_defined = readUInt();
    res->num_params = readByte();
    res->is_vararg = readByte();
    res->max_stack_size = readByte();
    res->code = readCode();
    res->constants = readConstants();
    res->upvalues = readUpvalues();
    res->prototypes = readProtos(source);
    res->line_info = readLineInfo();
    res->loc_vars = readLocVars();
    res->upvalue_names = readUpvalueNames();
    return res;
}

vector<shared_ptr<Prototype>> Reader::readProtos(string& parentSource) {
    auto n = readUInt();
    vector<shared_ptr<Prototype>> protos(n);
    for(int i = 0; i < n; i++) {
        protos[i] = readProto(parentSource);
    }
    return protos;
}

vector<int> Reader::readLineInfo() {
    auto n = readUInt();
    vector<int> lineInfo(n);
    for(int i = 0; i < n; i++) {
        lineInfo[i] = (int)readUInt();
    }
    return lineInfo;
}

vector<LocVar> Reader::readLocVars() {
    auto n = readUInt();
    vector<LocVar> locVars(n);
    for(int i = 0; i < n; i++) {
        locVars[i].varName = readString();
        locVars[i].startPC = readUInt();
        locVars[i].endPC = readUInt();
    }
    return locVars;
}

vector<string> Reader::readUpvalueNames() {
    auto n = readUInt();
    vector<string> upvalueNames(n);
    for(int i = 0; i < n; i++) {
        upvalueNames[i] = readString();
    }
    return upvalueNames;
}

shared_ptr<Prototype> Reader::unDump() {
    checkHeader();
    readByte(); // 跳过upvalue数量
    return readProto((string &) "");
}
