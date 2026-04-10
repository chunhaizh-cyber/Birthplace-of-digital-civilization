#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "任务管理任务模块.h"

enum class 枚举_学习阶段 : std::uint8_t {
    未定义 = 0,
    待适配 = 1,
    待调度 = 2,
    采样 = 3,
    更新 = 4,
    验证 = 5,
    提交 = 6,
    回滚 = 7,
};

enum class 枚举_学习状态 : std::uint8_t {
    未定义 = 0,
    待创建 = 1,
    挂起 = 2,
    可调度 = 3,
    执行中 = 4,
    已提交 = 5,
    已回滚 = 6,
    已失败 = 7,
};

enum class 枚举_学习等待原因 : std::uint8_t {
    无 = 0,
    原料不足 = 1,
    仅需求位方法头 = 2,
    等待对象补齐 = 3,
    等待再次命中 = 4,
    根层禁止学习 = 5,
};

struct 结构_学习任务同步输入 {
    时间戳 当前时间 = 0;
    std::string 去重键{};
    任务类::节点类* 学习子任务 = nullptr;
    方法类::节点类* 学习方法 = nullptr;
    任务类::节点类* 来源宿主任务 = nullptr;
    任务类::节点类* 来源步骤 = nullptr;
    任务类::节点类* 来源结果 = nullptr;
    方法类::节点类* 来源方法 = nullptr;
    枚举_任务管理根层重判结果 最近根层重判结果 = 枚举_任务管理根层重判结果::未定义;
    枚举_任务管理缺口类型 当前缺口类型 = 枚举_任务管理缺口类型::未定义;
    枚举_任务管理下一步去向 当前下一步去向 = 枚举_任务管理下一步去向::未定义;
    枚举_任务管理方法来源 当前方法来源 = 枚举_任务管理方法来源::未定义;
    bool 仅需求位方法头 = false;
    bool 可直接进入学习 = false;
    std::string 最近反馈摘要{};
};

struct 结构_学习任务实体 {
    std::uint64_t 账本ID = 0;
    时间戳 最近更新时间 = 0;
    std::string 去重键{};
    std::string 可合并键{};
    枚举_学习阶段 当前阶段 = 枚举_学习阶段::未定义;
    枚举_学习状态 当前状态 = 枚举_学习状态::未定义;
    枚举_学习等待原因 当前等待原因 = 枚举_学习等待原因::无;
    I64 当前优先级 = 0;
    I64 重试次数 = 0;

    任务类::节点类* 学习子任务 = nullptr;
    方法类::节点类* 学习方法 = nullptr;
    任务类::节点类* 来源宿主任务 = nullptr;
    任务类::节点类* 来源步骤 = nullptr;
    任务类::节点类* 来源结果 = nullptr;
    方法类::节点类* 来源方法 = nullptr;

    枚举_任务管理根层重判结果 最近根层重判结果 = 枚举_任务管理根层重判结果::未定义;
    枚举_任务管理缺口类型 当前缺口类型 = 枚举_任务管理缺口类型::未定义;
    枚举_任务管理下一步去向 当前下一步去向 = 枚举_任务管理下一步去向::未定义;
    枚举_任务管理方法来源 当前方法来源 = 枚举_任务管理方法来源::未定义;

    bool 仅需求位方法头 = false;
    bool 可直接进入学习 = false;
    bool 本轮新建 = false;
    bool 本轮更新 = false;

    std::string 学习子任务标题{};
    std::string 学习方法标题{};
    std::string 来源宿主任务标题{};
    std::string 来源步骤标题{};
    std::string 来源结果标题{};
    std::string 来源方法标题{};
    std::string 对象来源摘要{};
    std::string 待学习方法骨架摘要{};
    std::string 最近执行摘要{};
    std::string 最近失败摘要{};
    std::string 最近回流摘要{};
    std::string 最近反馈摘要{};
    std::string 摘要{};
};

struct 结构_学习账本快照 {
    std::size_t 总数 = 0;
    std::size_t 活跃数 = 0;
    std::size_t 挂起数 = 0;
    std::size_t 可调度数 = 0;
    std::size_t 执行中数 = 0;
    std::size_t 已完成数 = 0;
    std::string 最近摘要{};
    std::vector<结构_学习任务实体> 任务列表{};
};

struct 结构_学习任务更新输入 {
    std::uint64_t 账本ID = 0;
    时间戳 当前时间 = 0;

    bool 覆写阶段 = false;
    枚举_学习阶段 当前阶段 = 枚举_学习阶段::未定义;

    bool 覆写状态 = false;
    枚举_学习状态 当前状态 = 枚举_学习状态::未定义;

    bool 覆写等待原因 = false;
    枚举_学习等待原因 当前等待原因 = 枚举_学习等待原因::无;

    bool 覆写可合并键 = false;
    std::string 可合并键{};

    bool 覆写当前优先级 = false;
    I64 当前优先级 = 0;

    bool 覆写仅需求位方法头 = false;
    bool 仅需求位方法头 = false;

    bool 覆写可直接进入学习 = false;
    bool 可直接进入学习 = false;

    bool 累加重试次数 = false;
    I64 重试次数增量 = 1;

    bool 覆写对象来源摘要 = false;
    std::string 对象来源摘要{};

    bool 覆写待学习方法骨架摘要 = false;
    std::string 待学习方法骨架摘要{};

    bool 覆写最近执行摘要 = false;
    std::string 最近执行摘要{};

    bool 覆写最近失败摘要 = false;
    std::string 最近失败摘要{};

    bool 覆写最近回流摘要 = false;
    std::string 最近回流摘要{};
};

namespace 学习任务模块 {

bool 创建或更新学习任务(
    const 结构_学习任务同步输入& 输入,
    结构_学习任务实体* 输出 = nullptr) noexcept;

bool 读取学习任务实体(
    std::uint64_t 账本ID,
    结构_学习任务实体* 输出) noexcept;

bool 更新学习任务实体(
    const 结构_学习任务更新输入& 输入,
    结构_学习任务实体* 输出 = nullptr) noexcept;

bool 读取学习账本快照(
    结构_学习账本快照* 输出) noexcept;

std::vector<结构_学习任务实体> 枚举学习任务实体();

I64 统计活跃学习任务数量() noexcept;

} // namespace 学习任务模块
