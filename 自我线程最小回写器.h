#pragma once

#include <string>

#include "任务管理任务模块.h"
#include "自我线程运行包.h"

class 自我类;

namespace 自我线程最小回写器 {

struct 结构_最小回写结果 {
    bool 已生成结算包 = false;
    bool 已复用现有动态 = false;
    bool 已复用现有因果 = false;
    bool 已写入动态 = false;
    bool 已写入因果候选 = false;
    动态节点类* 动态节点 = nullptr;
    因果实例节点类* 因果节点 = nullptr;
    自我线程运行包::结构_回流结算包 结算包{};
    std::string 摘要{};
};

const char* 结算结论文本(自我线程运行包::枚举_结算结论 结论) noexcept;

bool 执行最小回流结算与回写(
    自我类& 自我对象,
    时间戳 now,
    const 自我线程运行包::结构_一步治理任务包* 任务包,
    const 结构_任务管理结果& 治理结果,
    结构_最小回写结果* 输出 = nullptr) noexcept;

} // namespace 自我线程最小回写器
