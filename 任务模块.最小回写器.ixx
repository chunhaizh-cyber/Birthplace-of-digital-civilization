module;

#include <string>
#include "基础数据类型.h"
#include "动态类.h"
#include "因果类.h"

export module 任务模块.最小回写器;

import 自我模块;
import 任务管理任务模块;
import 任务模块.运行包;

export namespace 任务最小回写器 {

struct 结构_最小回写结果 {
    bool 已生成结算包 = false;
    bool 已复用现有动态 = false;
    bool 已复用现有因果 = false;
    bool 已写入动态 = false;
    bool 已写入因果候选 = false;
    动态节点类* 动态节点 = nullptr;
    因果实例节点类* 因果节点 = nullptr;
    任务运行包::结构_回流结算包 结算包{};
    std::string 摘要{};
};

const char* 结算结论文本(任务运行包::枚举_结算结论 结论) noexcept;

bool 执行最小回流结算与回写(
    自我类& 自我对象,
    时间戳 now,
    const 任务运行包::结构_一步治理任务包* 任务包,
    const 结构_任务管理结果& 治理结果,
    结构_最小回写结果* 输出 = nullptr) noexcept;

} // namespace 任务最小回写器
