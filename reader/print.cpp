//
// Created by Administrator on 2024/4/25.
//

#include "print.h"

using namespace std;

void list(const shared_ptr<Prototype>& p){
    printHeader(p);
    printCode(p);
    for(auto &sub : p->prototypes) {
        list(sub);
    }
}

void printHeader(const shared_ptr<Prototype>& p) {
    string funcType("main");
    if(p->line_defined > 0) {
        funcType = "function";
    }
    string varargFlag;
    if(p->is_vararg) {
        varargFlag = "+";
    }
    printf("\n%s <%s:%d,%d> (%zu instructions)\n",
        funcType.c_str(), p->source.c_str(),p->line_defined, p->last_line_defined, p->code.size());
    printf("%d%s params, %d slots, %zu upvalues, ",
           p->num_params, varargFlag.c_str(),  p->max_stack_size, p->upvalues.size());
    printf("%zu locals, %zu constants, %zu functions\n",
           p->loc_vars.size(), p->constants.size(), p->prototypes.size());
}

void printCode(const shared_ptr<Prototype>& p) {
    for(int i = 0; i < p->code.size(); i++) {
        string line("-");
        if(p->line_info.size() > 0) {
            line = to_string(p->line_info[i]);
        }
        printf("\t%d\t[%s]\t0x%08X\n", i + 1, line.c_str(), p -> code[i]);
    }
}