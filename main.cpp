#include "lua_state/LuaState.h"

using namespace std;

int main() {

    LuaState vm;
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

    vm.PushInteger(1);
    vm.PushString("2.0");
    vm.PushString("3.0");
    vm.PushNumber(4.0);
    vm.Debug();
    vm.Arith(ArithOp::LUA_OPADD);
    vm.Debug();
    vm.Arith(ArithOp::LUA_OPBNOT);
    vm.Debug();
    vm.Len(2);
    vm.Debug();
    vm.Concat(3);
    vm.Debug();
    vm.PushBoolean(vm.Compare(1,2, CompareOp::LUA_OPEQ));
    vm.Debug();

    return 0;
}
