#pragma once

#include <string>

#include "学习调度模块.h"

struct 结构_学习执行结果 {
    bool 已命中任务 = false;
    bool 已推进阶段 = false;
    bool 已采样 = false;
    bool 已更新 = false;
    bool 已验证 = false;
    bool 已提交 = false;
    bool 已回滚 = false;
    bool 已回流 = false;
    std::uint64_t 账本ID = 0;
    枚举_学习阶段 当前阶段 = 枚举_学习阶段::未定义;
    枚举_学习状态 当前状态 = 枚举_学习状态::未定义;
    std::string 当前阶段文本{};
    std::string 当前状态文本{};
    std::string 摘要{};
    std::string 失败摘要{};
    std::string 回流摘要{};
};

namespace 学习执行模块 {

结构_学习执行结果 执行一步学习任务(
    const 结构_学习调度项& 调度项,
    时间戳 当前时间,
    const std::string& 调用点) noexcept;

} // namespace 学习执行模块
