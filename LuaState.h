//
// Created by Administrator on 2024/4/23.
//

#ifndef LUA_VM_LUASTATE_H
#define LUA_VM_LUASTATE_H
#include "consts.h"
#include "luaStack.h"
#include <iostream>
#include <memory>

class ILuaState {
public:
    /* 栈操作 */
    virtual int GetTop() const = 0;
    virtual int AbsIndex(int idx) const = 0;
    virtual bool Checkstack(int n) = 0;
    virtual void Pop(int n) = 0;
    virtual void Copy(int fromIdx, int toIdx) = 0;
    virtual void PushValue(int idx) = 0;
    virtual void Replace(int idx) = 0;
    virtual void Insert(int idx) = 0;
    virtual void Remove(int idx) = 0;
    virtual void Rotate(int idx, int n) = 0;
    virtual void SetTop(int idx) = 0;
    /* 访问功能 */
    virtual std::string TypeName(LuaType tp) const = 0;
//    virtual LuaType Type(int idx) const = 0;
//    virtual bool IsNone(int idx) const = 0;
//    virtual bool IsNil(int idx) const = 0;
//    virtual bool IsNoneOrNil(int idx) const = 0;
//    virtual bool IsBoolean(int idx) const = 0;
//    virtual bool IsInteger(int idx) const = 0;
//    virtual bool IsNumber(int idx) const = 0;
//    virtual bool IsString(int idx) const = 0;
//    virtual bool ToBoolean(int idx) const = 0;
//    virtual int ToInteger(int idx) const = 0;
//    virtual std::pair<int, bool> ToIntegerX(int idx) const = 0;
//    virtual double ToNumber(int idx) const = 0;
//    virtual std::pair<double, bool> ToNumberX(int idx) const = 0;
//    virtual std::string ToString(int idx) const = 0;
//    virtual std::pair<std::string, bool> ToStringX(int idx) const = 0;
    /* push功能 */
    virtual void PushNil() = 0;
    virtual void PushBoolean(bool b) = 0;
    virtual void PushInteger(int n) = 0;
    virtual void PushNumber(double n) = 0;
    virtual void PushString(const std::string& s) = 0;
    virtual ~ILuaState() {} // 声明虚析构函数，以便正确释放派生类对象
};

/**
 * LuaState封装整个Lua解释器的状态
 */
class LuaState: public ILuaState {
private:
    std::shared_ptr<luaStack> stack;
public:
    LuaState();
    int GetTop() const override;
    int AbsIndex(int idx) const override;
    /* 检查栈空间并扩容栈，永远返回true */
    bool Checkstack(int n) override;
    /* 从栈顶弹出n个值 */
    void Pop(int n) override;
    /* 复制值 */
    void Copy(int fromIdx, int toIdx) override;
    /* 把指定索引处的值推入栈顶 */
    void PushValue(int idx) override;
    /* 把栈顶处的值弹出，然后写入指定位置 */
    void Replace(int idx) override;
    /* 把栈顶值弹出，然后插入指定位置 */
    void Insert(int idx) override;
    /* 删除指定索引处的值 */
    void Remove(int idx) override;
    /* 将[idx, top]索引区间内的值朝栈顶方向旋转n个位置，n为负数则相反 */
    void Rotate(int idx, int n) override;
    /* 根据情况执行弹出或推入操作 */
    void SetTop(int idx) override;

    std::string TypeName(LuaType tp) const override;

    /* push功能 */
    void PushNil() override;
    void PushBoolean(bool b) override;
    void PushInteger(int n) override;
    void PushNumber(double n) override;
    void PushString(const std::string& s) override;

    void Debug();
};


#endif //LUA_VM_LUASTATE_H