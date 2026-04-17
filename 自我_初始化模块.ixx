module;

#include <string>

export module 自我_初始化模块;

import 自我模块;
import 自我模块.初始化;

export {

using 结构_自我初始化结果_最小_v0 = ::自我初始化模块::结构_自我初始化结果;

inline bool 执行自我初始化_最小_v0(
    自我类& 自我对象,
    const std::string& 调用点 = "执行自我初始化_最小_v0",
    结构_自我初始化结果_最小_v0* 输出 = nullptr) noexcept
{
    return ::自我初始化模块::执行自我初始化(自我对象, 调用点, 输出);
}

inline 结构_自我初始化结果_最小_v0 读取自我初始化结果_最小_v0(
    const 自我类& 自我对象) noexcept
{
    return ::自我初始化模块::读取自我初始化结果(自我对象);
}

} // export
