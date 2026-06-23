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
    std::uintptr_t 生命周期抽象特征指针 = 0;
    I64 生命周期值 = 0;
    枚举_控制面板摘要线程生命周期状态 生命周期 =
        枚举_控制面板摘要线程生命周期状态::未启动;

    std::uintptr_t 健康状态抽象特征指针 = 0;
    I64 健康状态值 = 0;
    bool 健康运行 = false;

    std::uint64_t 快照序号 = 0;

    std::uint64_t 世界版本 = 0;

    时间戳 采集时间 = 0;

    I64 自我线程生命周期值 = 0;
    I64 自我线程最近阶段值 = 0;
    I64 自我线程最近去向值 = 0;
    I64 自我线程Tick计数值 = 0;
    I64 自我线程Mailbox待消费数值 = 0;

    std::size_t 运行事实有运行证据方法数 = 0;
    std::size_t 运行事实失败任务数 = 0;
    std::size_t 运行事实等待任务数 = 0;
    std::string 运行事实最近方法主键{};
    时间戳 运行事实最近方法时间 = 0;
    时间戳 运行事实最近任务时间 = 0;

    I64 自检线程生命周期值 = 0;
    I64 自检线程最近模式值 = 0;
    I64 自检线程健康状态值 = 0;
    bool 自检线程健康运行 = false;
    std::uint64_t 自检线程Tick计数 = 0;
    std::uint64_t 自检线程累计发现问题数 = 0;
    std::uint64_t 自检线程累计提交需求数 = 0;
    std::uint64_t 自检线程累计提交报告数 = 0;
    std::uint64_t 自检线程累计需求化失败数 = 0;
    时间戳 自检线程最近检查时间 = 0;
    时间戳 自检线程最近提交时间 = 0;

    std::size_t 活跃需求数 = 0;

    std::size_t 等待子需求数 = 0;

    std::size_t 已满足需求数 = 0;

    std::size_t 筹办中任务数 = 0;

    std::size_t 执行中任务数 = 0;

    std::size_t 等待任务数 = 0;

    std::size_t 方法头节点数 = 0;

    std::size_t 方法补齐问题数 = 0;

    std::size_t 能力包缺失数 = 0;

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
