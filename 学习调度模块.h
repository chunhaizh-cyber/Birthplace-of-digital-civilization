#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "学习对象适配器.h"

struct 结构_学习调度项 {
    结构_学习任务实体 实体{};
    结构_学习对象适配结果 适配结果{};
    I64 当前优先级 = 0;
    std::string 去重键{};
    std::string 可合并键{};
    std::string 摘要{};
};

struct 结构_学习调度快照 {
    std::size_t 任务表总数 = 0;
    std::size_t 就绪队列数 = 0;
    std::size_t 等待表数 = 0;
    std::size_t 执行中数 = 0;
    std::vector<结构_学习调度项> 就绪队列{};
    std::vector<结构_学习调度项> 等待表{};
    std::vector<结构_学习调度项> 执行中列表{};
    std::string 摘要{};
};

namespace 学习调度模块 {

结构_学习调度快照 构建学习调度快照() noexcept;

bool 选择一条可执行学习任务(
    结构_学习调度项* 输出,
    结构_学习调度快照* 快照输出 = nullptr) noexcept;

} // namespace 学习调度模块
