module;

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "基础数据类型.h"
#include "需求类.h"
export module 自我线程模块;

import 自我模块;
import 任务管理任务模块;
import 任务模块.实体;
import 任务模块.管理工作线程;
import 自我线程模块.消息协议;
import 自我线程模块.消息处理器;
import 自我线程模块.消息批次执行器;
export enum class 枚举_线程生命周期状态 : std::uint8_t {
    未启动 = 0,
    启动中 = 1,
    运行中 = 2,
    停止请求中 = 3,
    收尾中 = 4,
    已停止 = 5,
    故障 = 6,
};

export enum class 枚举_自我线程运行阶段 : std::uint8_t {
    未定义 = 0,
    自我初始化 = 1,
    线程启动中 = 2,
    运行态建立 = 3,
    主循环消息归并 = 4,
    主循环初始化补齐 = 5,
    主循环主链回填 = 6,
    主循环根层裁决 = 7,
    主循环任务治理 = 8,
    主循环结果回流 = 9,
    主循环心跳巡检 = 10,
    停止请求中 = 11,
    线程收尾 = 12,
    线程停止 = 13,
    线程故障 = 14,
};

export enum class 枚举_自我线程最终去向 : std::uint8_t {
    未定义 = 0,
    继续提交 = 1,
    回到重筹办 = 2,
    转入学习 = 3,
    进入收束 = 4,
    待机 = 5,
    停止 = 6,
};

export struct 结构_自我运行阶段事件 {
    时间戳 时间 = 0;
    枚举_自我线程运行阶段 阶段 = 枚举_自我线程运行阶段::未定义;
    std::string 摘要{};
    std::uintptr_t 自我存在 = 0;
    std::uintptr_t 当前主需求 = 0;
    std::uintptr_t 当前主任务 = 0;
    std::uintptr_t 当前主方法 = 0;
    std::uintptr_t 当前管理任务 = 0;
};

export struct 结构_自我线程配置 {
    std::chrono::microseconds Tick间隔{250000};
    std::size_t 运行阶段事件保留上限 = 32;
};

export struct 结构_线程状态切换上报参数 {
    结构_线程存在初始化参数 线程存在初始化{};
    自我线程消息协议::枚举_存在类型 线程存在类型 =
        自我线程消息协议::枚举_存在类型::未定义;
    自我线程消息协议::枚举_特征类型 状态特征类型 =
        自我线程消息协议::枚举_特征类型::生命周期状态;
    I64 旧状态 = 0;
    I64 新状态 = 0;
    时间戳 发生时间 = 0;
    自我线程消息协议::枚举_消息来源链 来源链 =
        自我线程消息协议::枚举_消息来源链::线程直达消息;
    自我线程消息协议::枚举_存在类型 决策者类型 =
        自我线程消息协议::枚举_存在类型::未定义;
    std::uint64_t 决策者主键 = 0;
    std::string 决策者标签{};
    std::string 触发动作{};
    std::string 原因键{};
    std::string 原因摘要{};
    自我线程消息协议::枚举_存在类型 直属拥有者类型 =
        自我线程消息协议::枚举_存在类型::未定义;
    std::uint64_t 直属拥有者主键 = 0;
    std::uint64_t 关联任务主键 = 0;
    std::uint64_t 关联线程主键 = 0;
    bool 是否故障触发 = false;
    bool 是否stop链触发 = false;
    bool 无变化时仅同步特征 = true;
};

export class 自我线程类 {
public:
    struct 结构_需求列表项 {
        const 词性节点类* 需求类型 = nullptr;
        std::string 需求类型名称{};
        std::uintptr_t 需求节点指针 = 0;
        std::string 需求节点主键{};
        I64 节点安全权重 = 0;
        I64 节点服务权重 = 0;
        I64 类型安全权重汇总 = 0;
        I64 类型服务权重汇总 = 0;
    };

    struct 结构_循环结果 {
        bool 已补齐初始化 = false;
        bool 已回填主链 = false;
        bool 任务管理治理触发 = false;
        bool 任务管理证据触发 = false;
        bool 任务管理上位输入已刷新 = false;
        bool 需求树已更新 = false;
        bool 有派生需求待实例化 = false;
        bool 有子结果待路由 = false;

        std::size_t 治理mailbox待消费数 = 0;
        std::string 任务管理当前去向{};
        std::string 最近消费治理事件摘要{};
        std::string 最近消费治理事件幂等键{};
        std::uint32_t 最近消费治理事件重放代数 = 0;
        std::string 最近消费治理事件结果{};
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
    bool 是否首轮运行已完成() const noexcept;
    bool 是否本次启动来自故障恢复() const noexcept;
    枚举_线程生命周期状态 读取生命周期状态() const noexcept;
    枚举_自我线程运行阶段 读取最近运行阶段() const noexcept;
    枚举_自我线程最终去向 读取最近最终去向() const noexcept;
    std::uint64_t 读取Tick计数() const noexcept;
    std::uint64_t 读取累计故障次数() const noexcept;
    std::uint64_t 读取累计恢复次数() const noexcept;
    时间戳 读取最近Tick时间() const noexcept;
    时间戳 读取上次故障时间() const noexcept;
    std::string 读取上次故障摘要() const;
    std::string 读取最近恢复摘要() const;
    std::string 读取最近故障摘要() const;
    std::string 读取最近运行摘要() const;
    结构_自我线程配置& 配置() noexcept;
    const 结构_自我线程配置& 配置() const noexcept;

private:
    struct 结构_主循环骨架上下文 {
        时间戳 当前时间 = 0;
        结构_循环结果 结果{};
        自我线程消息协议::结构_治理消息 已消费消息{};
        std::deque<结构_治理消息> 本轮冻结消息{};
        自我线程消息批次执行器::结构_治理消息批次 本轮治理消息批次{};
        自我线程消息处理器::结构_需求分支刷新范围 需求树变更刷新范围{};
        自我线程消息批次执行器::结构_线程裁决结果 线程裁决结果{};
        结构_任务局部运行态 任务局部运行态{};
        任务管理工作线程::结构_工作线程桥接结果 桥接结果{};
        bool 命中治理邮箱事件 = false;
        bool 已应用需求树变更 = false;
    };

    friend bool 投递治理外部反馈事件(
        const std::string& 摘要,
        const std::string& 来源主观察特征);
    friend bool 上报线程状态变化(
        const 自我线程消息协议::结构_线程状态变化消息段& 消息段,
        const 结构_线程存在初始化参数* 线程存在初始化,
        const std::string& 调用点);
    friend bool 切换线程状态并上报(
        const 结构_线程状态切换上报参数& 参数,
        const std::string& 调用点);

    void 主循环_();
    结构_循环结果 执行主循环一轮_(时间戳 now);
    void 步骤_冻结本轮消息批次_(结构_主循环骨架上下文* 上下文);
    void 步骤_规范化消息并落一次特征账_(结构_主循环骨架上下文* 上下文);
    void 步骤_重算受影响二次特征_(结构_主循环骨架上下文* 上下文);
    void 步骤_刷新线程上下文并生成治理帧_(结构_主循环骨架上下文* 上下文);
    void 步骤_整理需求并做根层重判_(结构_主循环骨架上下文* 上下文);
    void 步骤_生成并执行主派发决议_(结构_主循环骨架上下文* 上下文);
    void 步骤_路由结果并协调父任务_(结构_主循环骨架上下文* 上下文);
    void 步骤_本轮结算学习触发故障收口_(结构_主循环骨架上下文* 上下文);
    bool 投递治理消息(const 结构_治理消息& 消息);
    void 刷新初始化标记_已加锁() noexcept;
    void 刷新治理恢复事件镜像_已加锁() noexcept;
    void 记录阶段事件_已加锁(
        时间戳 now,
        枚举_自我线程运行阶段 阶段,
        const std::string& 摘要);
    void 故障收口_已加锁(时间戳 now, const std::string& 摘要);

private:
    mutable std::mutex 状态锁_{};
    std::condition_variable 唤醒条件_{};
    std::thread 工作线程_{};
    std::atomic_bool 停止请求_{false};
    std::atomic_bool 运行中_{false};

    结构_自我线程配置 配置_{};
    枚举_线程生命周期状态 生命周期_ = 枚举_线程生命周期状态::未启动;
    bool 初始化完成_ = false;
    bool 健康运行_ = false;
    bool 首轮运行已完成_ = false;
    bool 本次启动来自故障恢复_ = false;
    bool 待故障恢复启动_ = false;
    std::uint64_t Tick计数_ = 0;
    std::uint64_t 治理事件序号_ = 0;
    std::uint64_t 累计故障次数_ = 0;
    std::uint64_t 累计恢复次数_ = 0;
    std::uint64_t 最近显式治理事件ID_ = 0;
    std::uint64_t 累计显式治理事件消费数_ = 0;
    时间戳 最近Tick时间_ = 0;
    时间戳 上次故障时间_ = 0;
    std::string 上次故障摘要_{};
    std::string 最近恢复摘要_{};
    std::string 最近故障摘要_{};
    std::deque<结构_治理消息> 治理mailbox_{};
    std::deque<结构_治理消息> 本轮冻结治理批次_{};
    std::vector<结构_需求列表项> 服务优先需求列表_{};
    std::vector<结构_需求列表项> 安全优先需求列表_{};
    std::vector<结构_自我运行阶段事件> 运行阶段事件_{};
    结构_循环结果 最近循环结果_{};
};

export 自我线程类& 获取全局自我线程() noexcept;
export bool 初始化自我环境(const std::string& 调用点 = "初始化自我环境");
export bool 启动自我线程(const std::string& 调用点 = "启动自我线程");
export void 停止自我线程(const std::string& 调用点 = "停止自我线程");
export bool 上报线程状态变化(
    const 自我线程消息协议::结构_线程状态变化消息段& 消息段,
    const 结构_线程存在初始化参数* 线程存在初始化 = nullptr,
    const std::string& 调用点 = "上报线程状态变化");
export bool 切换线程状态并上报(
    const 结构_线程状态切换上报参数& 参数,
    const std::string& 调用点 = "切换线程状态并上报");
export bool 投递治理外部反馈事件(
    const std::string& 摘要,
    const std::string& 来源主观察特征 = "外部反馈/人工注入");
