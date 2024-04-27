#include "lua_state/LuaState.h"
#include <fstream>
#include "reader/Reader.h"
#include "reader/print.h"
#include <memory>
#include "opcode/opcode.h"

using namespace std;

void run(shared_ptr<Prototype> p) {
    /* 获取最大栈长度 */
    int maxStackSize = p->max_stack_size;
    /* 创建虚拟机实例 */
    auto vm =  make_shared<LuaState>(maxStackSize + 8, p);
    vm -> SetTop(maxStackSize);
    while(true) {
        auto pc = vm -> PC();
        auto instr =  Instruction(vm -> Fetch());
        if(instr.getOpCode() == OP_RETURN) {
            break;
        }
        printf("[%02d]: %s ", pc + 1, instr.getName().c_str());
        opcodes[instr.getOpCode()].action(instr, vm);
        vm -> Debug();
    }
}

int main() {

    Reader r("C:\\Users\\Administrator\\Documents\\lua\\func.out");
    auto p = r.unDump();
    list(p, false);

//    LuaState vm(20, "");
//    vm.Debug();
//    vm.PushBoolean(true);
//    vm.Debug();
//    vm.PushInteger(0);
//    vm.Debug();
//    vm.PushNil();
//    vm.Debug();
//    vm.PushString("nihao");
//    vm.Debug();
//    vm.PushValue(-4);
//    vm.Debug();
//    vm.Replace(2);
//    vm.Debug();
//    vm.SetTop(6);
//    vm.Debug();
//    vm.Remove(6);
//    vm.Debug();
//    vm.SetTop(-5);
//    vm.Debug();

//    LuaValueNumber n1(1.1);
//    auto [i1, ok] = n1.to_int();
//    LuaValueNumber n2(1.0);
//    auto [i2, ok2] = n2.to_int();
//    cout << ok << " " << ok2 << endl;

//    vm.PushInteger(1);
//    vm.PushString("2.0");
//    vm.PushString("3.0");
//    vm.PushNumber(4.0);
//    vm.Debug();
//    vm.Arith(ArithOp::LUA_OPADD);
//    vm.Debug();
//    vm.Arith(ArithOp::LUA_OPBNOT);
//    vm.Debug();
//    vm.Len(2);
//    vm.Debug();
//    vm.Concat(3);
//    vm.Debug();
//    vm.PushBoolean(vm.Compare(1,2, CompareOp::LUA_OPEQ));
//    vm.Debug();

    return 0;
}
