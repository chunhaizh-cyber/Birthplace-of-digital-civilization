#pragma once

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "自我类.h"

enum class 枚举_线程生命周期状态 : std::uint8_t {
    未启动 = 0,
    启动中 = 1,
    运行中 = 2,
    停止请求中 = 3,
    收尾中 = 4,
    已停止 = 5,
    故障 = 6,
};

enum class 枚举_自我线程运行阶段 : std::uint8_t {
    未定义 = 0,
    自我初始化 = 1,
    线程启动中 = 2,
    运行态建立 = 3,
    主循环消息归并 = 4,
    主循环初始化补齐 = 5,
    主循环主链回填 = 6,
    主循环先天维护 = 7,
    主循环上位输入刷新 = 8,
    主循环任务治理 = 9,
    主循环学习调度 = 10,
    主循环学习执行 = 11,
    主循环心跳巡检 = 12,
    停止请求中 = 13,
    线程收尾 = 14,
    线程停止 = 15,
    线程故障 = 16,
};

enum class 枚举_自我线程最终去向 : std::uint8_t {
    未定义 = 0,
    继续提交 = 1,
    回到重筹办 = 2,
    转入学习 = 3,
    进入收束 = 4,
    待机 = 5,
    停止 = 6,
};

struct 结构_自我运行阶段事件 {
    时间戳 时间 = 0;
    枚举_自我线程运行阶段 阶段 = 枚举_自我线程运行阶段::未定义;
    std::string 阶段键{};
    std::string 摘要{};
    std::uintptr_t 自我存在 = 0;
    std::uintptr_t 当前主需求 = 0;
    std::uintptr_t 当前主任务 = 0;
    std::uintptr_t 当前主方法 = 0;
    std::uintptr_t 当前管理任务 = 0;
};

struct 结构_自我线程配置 {
    std::chrono::microseconds Tick间隔{ 250000 };
    std::size_t 运行阶段事件保留上限 = 32;
};

struct 结构_自我线程快照 {
    枚举_线程生命周期状态 生命周期 = 枚举_线程生命周期状态::未启动;
    bool 数据层已初始化 = false;
    bool 锚点已初始化 = false;
    bool 运行环境已初始化 = false;
    bool 初始化完成 = false;
    bool 健康运行 = false;
    bool 首轮运行已完成 = false;
    bool 待机状态 = false;
    bool 本次启动来自故障恢复 = false;

    I64 安全值 = 0;
    I64 服务值 = 0;
    I64 物理安全 = 0;
    I64 风险安全 = 0;
    I64 安全保障差值 = 0;
    I64 时序正向步长 = 0;
    I64 时序反向步长 = 0;
    I64 服务时序衰减步长 = 0;

    std::uint64_t Tick计数 = 0;
    std::uint64_t 累计故障次数 = 0;
    std::uint64_t 累计恢复次数 = 0;
    时间戳 最近Tick时间 = 0;
    时间戳 上次故障时间 = 0;

    std::uintptr_t 自我存在 = 0;
    std::uintptr_t 自我现实场景 = 0;
    std::uintptr_t 自我内部世界 = 0;
    std::uintptr_t 当前主需求 = 0;
    std::uintptr_t 当前主任务 = 0;
    std::uintptr_t 当前主方法 = 0;
    std::uintptr_t 当前管理任务 = 0;
    std::uintptr_t 任务管理当前步骤 = 0;
    std::uintptr_t 任务管理最近结果 = 0;

    std::size_t 基础信息节点数 = 0;
    std::size_t 场景数 = 0;
    std::size_t 存在数 = 0;
    std::size_t 特征数 = 0;
    std::size_t 状态数 = 0;
    std::size_t 动态数 = 0;
    std::size_t 因果实例数 = 0;
    std::size_t 因果模板数 = 0;
    std::size_t 需求数 = 0;
    std::size_t 任务数 = 0;
    std::size_t 方法数 = 0;
    std::size_t 学习任务总数 = 0;
    std::size_t 学习任务就绪数 = 0;
    std::size_t 学习任务等待数 = 0;
    std::size_t 学习任务执行中数 = 0;

    std::string 自我现实场景名称{};
    std::string 自我内部世界名称{};
    std::string 任务管理根层重判结果{};
    std::string 任务管理执行前门控结果{};
    std::string 任务管理上层反馈摘要{};
    std::string 任务管理当前缺口{};
    std::string 任务管理当前去向{};
    std::string 任务管理总控结果{};
    std::string 任务管理反馈类型{};
    std::string 任务管理反馈摘要{};
    std::string 任务管理方法来源{};
    std::string 任务管理步骤位{};
    std::string 任务管理步骤语义{};
    std::string 任务管理当前步骤标题{};
    std::string 任务管理最近结果标题{};
    std::string 任务管理目标结果摘要{};
    std::string 主循环归并来源{};
    std::string 主循环归并摘要{};
    std::string 主消息心跳车道状态{};
    std::string 任务治理车道状态{};
    std::string 学习车道状态{};
    std::string 延迟事实车道状态{};
    std::string 根任务结构调整车道状态{};
    std::string 运行时车道摘要{};
    std::string 学习调度摘要{};
    std::string 学习当前阶段{};
    std::string 学习当前状态{};
    std::string 学习当前任务标题{};
    std::string 学习当前方法标题{};
    std::string 学习最近摘要{};
    std::string 学习最近失败摘要{};
    std::string 学习最近回流摘要{};
    枚举_自我线程最终去向 当前最终去向枚举 = 枚举_自我线程最终去向::未定义;
    std::string 当前最终去向{};
    枚举_自我线程运行阶段 当前运行阶段枚举 = 枚举_自我线程运行阶段::未定义;
    std::string 当前运行阶段{};
    std::string 上次故障摘要{};
    std::string 最近恢复摘要{};
    std::string 最近运行摘要{};
    std::string 最近故障摘要{};
    std::vector<结构_自我运行阶段事件> 运行阶段事件{};
};

class 自我线程类 {
public:
    struct 结构_循环结果 {
        bool 已补齐初始化 = false;
        bool 已回填主链 = false;
        bool 服务衰减步长重算触发 = false;
        bool 安全增加步长重算触发 = false;
        bool 安全减少步长重算触发 = false;
        bool 安全低位回升触发 = false;
        bool 安全高位回落触发 = false;
        bool 服务归零待机保护触发 = false;
        bool 服务衰减触发 = false;
        bool 风险安全回归触发 = false;
        bool 任务管理治理触发 = false;
        bool 任务管理状态推进触发 = false;
        bool 任务管理步骤结果触发 = false;
        bool 任务管理证据触发 = false;
        bool 任务管理上位输入已刷新 = false;
        bool 学习调度触发 = false;
        bool 学习执行触发 = false;
        bool 学习回流触发 = false;
        bool 主循环归并触发 = false;
        bool 已归并任务治理结果 = false;
        bool 已归并学习结果 = false;
        bool 已归并学习回流 = false;
        std::uintptr_t 任务管理当前步骤指针 = 0;
        std::uintptr_t 任务管理最近结果指针 = 0;
        std::size_t 学习任务总数 = 0;
        std::size_t 学习任务就绪数 = 0;
        std::size_t 学习任务等待数 = 0;
        std::size_t 学习任务执行中数 = 0;
        std::string 任务管理根层重判结果{};
        std::string 任务管理执行前门控结果{};
        std::string 任务管理上层反馈摘要{};
        std::string 任务管理当前缺口{};
        std::string 任务管理当前去向{};
        std::string 任务管理总控结果{};
        std::string 任务管理反馈类型{};
        std::string 任务管理反馈摘要{};
        std::string 任务管理方法来源{};
        std::string 任务管理步骤位{};
        std::string 任务管理步骤语义{};
        std::string 任务管理当前步骤{};
        std::string 任务管理最近结果{};
        std::string 任务管理目标结果摘要{};
        std::string 主循环归并来源{};
        std::string 主循环归并摘要{};
        std::string 主消息心跳车道状态{};
        std::string 任务治理车道状态{};
        std::string 学习车道状态{};
        std::string 延迟事实车道状态{};
        std::string 根任务结构调整车道状态{};
        std::string 运行时车道摘要{};
        std::string 任务管理摘要{};
        std::string 学习调度摘要{};
        std::string 学习当前阶段{};
        std::string 学习当前状态{};
        std::string 学习当前任务{};
        std::string 学习当前方法{};
        std::string 学习最近摘要{};
        std::string 学习最近失败{};
        std::string 学习最近回流{};
        枚举_自我线程最终去向 当前最终去向 = 枚举_自我线程最终去向::未定义;
        枚举_自我线程运行阶段 当前运行阶段 = 枚举_自我线程运行阶段::未定义;
    };

public:
    自我线程类();
    ~自我线程类();

    自我线程类(const 自我线程类&) = delete;
    自我线程类& operator=(const 自我线程类&) = delete;
    自我线程类(自我线程类&&) = delete;
    自我线程类& operator=(自我线程类&&) = delete;

    bool 自我初始化(const std::string& 调用点 = "自我线程类::自我初始化");
    bool 启动(const std::string& 调用点 = "自我线程类::启动");
    void 请求停止(const std::string& 调用点 = "自我线程类::请求停止");
    void 等待停止(const std::string& 调用点 = "自我线程类::等待停止");
    void 停止(const std::string& 调用点 = "自我线程类::停止");

    bool 是否运行中() const noexcept;
    bool 是否初始化完成() const noexcept;
    bool 是否健康运行() const noexcept;

    结构_自我线程快照 读取快照() const;

    结构_自我线程配置& 配置() noexcept;
    const 结构_自我线程配置& 配置() const noexcept;

private:
    void 主循环_();
    结构_循环结果 执行主循环一轮_(时间戳 now);
    void 刷新初始化标记_已加锁() noexcept;
    void 刷新快照_已加锁(
        时间戳 now,
        枚举_自我线程运行阶段 当前运行阶段,
        const std::string& 最近运行摘要);
    void 记录阶段事件_已加锁(
        时间戳 now,
        枚举_自我线程运行阶段 阶段,
        const std::string& 摘要);
    void 故障收口_已加锁(时间戳 now, const std::string& 摘要);

private:
    mutable std::mutex 状态锁_{};
    std::thread 工作线程_{};
    std::atomic_bool 停止请求_{ false };
    std::atomic_bool 运行中_{ false };

    结构_自我线程配置 配置_{};
    枚举_线程生命周期状态 生命周期_ = 枚举_线程生命周期状态::未启动;
    bool 数据层已初始化_ = false;
    bool 锚点已初始化_ = false;
    bool 运行环境已初始化_ = false;
    bool 初始化完成_ = false;
    bool 健康运行_ = false;
    bool 首轮运行已完成_ = false;
    bool 本次启动来自故障恢复_ = false;
    bool 待故障恢复启动_ = false;
    std::uint64_t Tick计数_ = 0;
    std::uint64_t 累计故障次数_ = 0;
    std::uint64_t 累计恢复次数_ = 0;
    时间戳 最近Tick时间_ = 0;
    时间戳 上次故障时间_ = 0;
    std::string 上次故障摘要_{};
    std::string 最近恢复摘要_{};
    std::string 最近故障摘要_{};
    结构_循环结果 最近循环结果_{};
    结构_自我线程快照 最近快照_{};
};

自我线程类& 获取全局自我线程() noexcept;
bool 初始化自我环境(const std::string& 调用点 = "初始化自我环境");
bool 启动自我线程(const std::string& 调用点 = "启动自我线程");
void 停止自我线程(const std::string& 调用点 = "停止自我线程");
结构_自我线程快照 读取全局自我线程快照();
