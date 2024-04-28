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

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cout << "Usage: lua_vm.exe file" << endl;
        return 0;
    }
    Reader r(argv[1]);
    auto p = r.unDump();
    list(p, true);
    run(p);

    return 0;
}
