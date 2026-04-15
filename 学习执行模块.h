#pragma once

#include <string>

#include "学习调度模块.h"
#include "实践结果消息.h"

class 自我类;

struct 结构_学习执行结果 {
    bool 已命中任务 = false;
    bool 已推进阶段 = false;
    bool 已采样 = false;
    bool 已更新 = false;
    bool 已验证 = false;
    bool 已提交 = false;
    bool 已回滚 = false;
    bool 已回流 = false;
    bool 已形成学习反馈 = false;
    bool 已完善方法 = false;
    bool 已产出实践结果消息 = false;
    std::uint64_t 账本ID = 0;
    枚举_学习阶段 当前阶段 = 枚举_学习阶段::未定义;
    枚举_学习状态 当前状态 = 枚举_学习状态::未定义;
    枚举_任务管理学习反馈消费策略 建议消费动作 =
        static_cast<枚举_任务管理学习反馈消费策略>(0);
    std::string 当前阶段文本{};
    std::string 当前状态文本{};
    结构_实践结果消息 实践结果消息{};
    std::string 摘要{};
    std::string 失败摘要{};
    std::string 学习情况摘要{};
    std::string 学习反馈摘要{};
    std::string 已完善方法摘要{};
    std::string 回流摘要{};
};

namespace 学习执行模块 {

结构_学习执行结果 执行一步学习任务(
    自我类& 自我对象,
    const 结构_学习调度项& 调度项,
    时间戳 当前时间,
    const std::string& 调用点) noexcept;

} // namespace 学习执行模块
