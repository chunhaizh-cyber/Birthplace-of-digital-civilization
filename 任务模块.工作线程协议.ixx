module;

#include <cstdint>
#include "语素信息类型.h"

export module 任务模块.工作线程协议;

import 自我类.特征定义;

export enum class 枚举_任务推进事件类型 : std::uint8_t {
    未定义 = 0,
    当前步骤可执行 = 1,
    当前步骤阻塞 = 2,
    派生需求上报 = 3,
    局部结果返回 = 4,
    最终结果返回 = 5,
    异常上报 = 6,
};

export struct 结构_任务推进事件 {
    std::uint64_t 任务根ID = 0;
    枚举_任务推进事件类型 事件类型 = 枚举_任务推进事件类型::未定义;
    const 语素入口节点类* 事件类型抽象特征指针 = nullptr;
    std::int64_t 事件类型值 = static_cast<std::int64_t>(枚举_任务推进事件类型::未定义);
    std::uintptr_t 当前步骤指针 = 0;
    std::uintptr_t 结果指针 = 0;
};

// 功能：同步线程、任务、场景或对象之间的状态。
export inline void 同步工作线程事件语义槽(结构_任务推进事件& 事件) noexcept
{
    事件.事件类型抽象特征指针 = 自我特征定义类::类型_工作线程_推进事件类型();
    事件.事件类型值 = static_cast<std::int64_t>(事件.事件类型);
}

