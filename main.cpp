#include "LuaState.h"

using namespace std;

int main() {

    LuaState vm;
    vm.Debug();
    vm.PushBoolean(true);
    vm.Debug();
    vm.PushInteger(0);
    vm.Debug();
    vm.PushNil();
    vm.Debug();
    vm.PushString("nihao");
    vm.Debug();
    vm.PushValue(-4);
    vm.Debug();
    vm.Replace(2);
    vm.Debug();
    vm.SetTop(6);
    vm.Debug();
    vm.Remove(6);
    vm.Debug();
    vm.SetTop(-5);
    vm.Debug();

    return 0;
}
