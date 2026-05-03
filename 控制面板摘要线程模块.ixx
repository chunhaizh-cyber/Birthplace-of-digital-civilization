module;

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

#include "基础数据类型.h"

export module 控制面板摘要线程模块;

export enum class 枚举_控制面板摘要线程生命周期状态 : std::uint8_t {
    未启动 = 0,
    启动中 = 1,
    运行中 = 2,
    停止请求中 = 3,
    已停止 = 4,
    故障 = 5,
};

export struct 结构_控制面板摘要线程配置 {
    std::chrono::milliseconds Tick间隔{1000};
};

export struct 结构_控制面板摘要快照 {
    std::uintptr_t 快照序号抽象特征指针 = 0;
    I64 快照序号值 = 0;
    std::uint64_t 快照序号 = 0;

    std::uintptr_t 世界版本抽象特征指针 = 0;
    I64 世界版本值 = 0;
    std::uint64_t 世界版本 = 0;

    std::uintptr_t 采集时间抽象特征指针 = 0;
    I64 采集时间值 = 0;
    时间戳 采集时间 = 0;

    std::string 自我线程摘要{};
    std::string 需求树摘要{};
    std::string 任务树摘要{};
    std::string 方法树摘要{};
    std::string 运行事实摘要{};
    std::string 自检摘要{};

    std::uintptr_t 活跃需求数抽象特征指针 = 0;
    I64 活跃需求数值 = 0;
    std::size_t 活跃需求数 = 0;

    std::uintptr_t 等待子需求数抽象特征指针 = 0;
    I64 等待子需求数值 = 0;
    std::size_t 等待子需求数 = 0;

    std::uintptr_t 已满足需求数抽象特征指针 = 0;
    I64 已满足需求数值 = 0;
    std::size_t 已满足需求数 = 0;

    std::uintptr_t 筹办中任务数抽象特征指针 = 0;
    I64 筹办中任务数值 = 0;
    std::size_t 筹办中任务数 = 0;

    std::uintptr_t 执行中任务数抽象特征指针 = 0;
    I64 执行中任务数值 = 0;
    std::size_t 执行中任务数 = 0;

    std::uintptr_t 等待任务数抽象特征指针 = 0;
    I64 等待任务数值 = 0;
    std::size_t 等待任务数 = 0;

    std::uintptr_t 方法头节点数抽象特征指针 = 0;
    I64 方法头节点数值 = 0;
    std::size_t 方法头节点数 = 0;

    std::uintptr_t 方法结构缺口数抽象特征指针 = 0;
    I64 方法结构缺口数值 = 0;
    std::size_t 方法结构缺口数 = 0;

    std::uintptr_t 能力包缺失数抽象特征指针 = 0;
    I64 能力包缺失数值 = 0;
    std::size_t 能力包缺失数 = 0;

    std::uintptr_t 条件结果对缺失数抽象特征指针 = 0;
    I64 条件结果对缺失数值 = 0;
    std::size_t 条件结果对缺失数 = 0;
};

export class 控制面板摘要线程类 {
public:
    控制面板摘要线程类();
    ~控制面板摘要线程类();

    控制面板摘要线程类(const 控制面板摘要线程类&) = delete;
    控制面板摘要线程类& operator=(const 控制面板摘要线程类&) = delete;

    bool 启动();
    void 请求停止();
    void 等待停止();
    void 请求刷新();

    bool 是否健康运行() const noexcept;
    枚举_控制面板摘要线程生命周期状态 读取生命周期状态() const noexcept;
    结构_控制面板摘要快照 读取最新快照() const;

private:
    void 主循环_();
    结构_控制面板摘要快照 采集快照_();
    void 写入快照_(结构_控制面板摘要快照 快照);

private:
    mutable std::mutex 状态锁_{};
    std::condition_variable 唤醒条件_{};
    std::thread 工作线程_{};
    std::atomic_bool 停止请求_{false};
    std::atomic_bool 刷新请求_{false};

    结构_控制面板摘要线程配置 配置_{};
    枚举_控制面板摘要线程生命周期状态 生命周期_ =
        枚举_控制面板摘要线程生命周期状态::未启动;
    bool 健康运行_ = false;
    std::uint64_t 快照序号_ = 0;
    结构_控制面板摘要快照 最新快照_{};
};

export 控制面板摘要线程类& 获取全局控制面板摘要线程() noexcept;
export bool 启动控制面板摘要线程();
export void 停止控制面板摘要线程();
export bool 请求刷新控制面板摘要();
export 结构_控制面板摘要快照 读取最新控制面板摘要快照();
export const char* 控制面板摘要线程生命周期文本(
    枚举_控制面板摘要线程生命周期状态 状态) noexcept;
