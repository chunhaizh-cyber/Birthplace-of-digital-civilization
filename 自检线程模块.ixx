module;

#include <chrono>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <condition_variable>
#include <unordered_map>
#include <vector>

#include "基础数据类型.h"

export module 自检线程模块;

export enum class 枚举_自检线程生命周期状态 : std::uint8_t {
    未启动 = 0,
    启动中 = 1,
    运行中 = 2,
    停止请求中 = 3,
    已停止 = 4,
    故障 = 5,
};

export enum class 枚举_自检线程模式 : std::uint8_t {
    运行中 = 0,
    休眠期 = 1,
};

export struct 结构_自检线程配置 {
    std::chrono::milliseconds Tick间隔{500};
    std::chrono::milliseconds 方法结构检查间隔{5000};
    std::chrono::milliseconds 方法运行账检查间隔{15000};
    std::chrono::milliseconds 方法补齐停滞检查间隔{20000};
    std::chrono::milliseconds 去重抑制间隔{300000};
    std::size_t 每轮提交上限 = 3;
    std::size_t 单类扫描上限 = 64;
    std::size_t 事件保留上限 = 64;
};

export struct 结构_自检线程摘要 {
    std::uintptr_t 生命周期抽象特征指针 = 0;
    I64 生命周期值 = 0;
    枚举_自检线程生命周期状态 生命周期 = 枚举_自检线程生命周期状态::未启动;

    std::uintptr_t 最近模式抽象特征指针 = 0;
    I64 最近模式值 = 0;
    枚举_自检线程模式 最近模式 = 枚举_自检线程模式::运行中;

    std::uintptr_t 健康状态抽象特征指针 = 0;
    I64 健康状态值 = 0;
    bool 健康运行 = false;

    std::uintptr_t Tick计数抽象特征指针 = 0;
    I64 Tick计数值 = 0;
    std::uint64_t Tick计数 = 0;

    std::uintptr_t 累计提交需求数抽象特征指针 = 0;
    I64 累计提交需求数值 = 0;
    std::uint64_t 累计提交需求数 = 0;

    I64 累计提交报告数值 = 0;
    std::uint64_t 累计提交报告数 = 0;

    I64 累计需求化失败数值 = 0;
    std::uint64_t 累计需求化失败数 = 0;

    std::uintptr_t 累计发现问题数抽象特征指针 = 0;
    I64 累计发现问题数值 = 0;
    std::uint64_t 累计发现问题数 = 0;

    std::uintptr_t 最近检查时间抽象特征指针 = 0;
    I64 最近检查时间值 = 0;
    时间戳 最近检查时间 = 0;

    std::uintptr_t 最近提交时间抽象特征指针 = 0;
    I64 最近提交时间值 = 0;
    时间戳 最近提交时间 = 0;

    std::string 最近事件摘要{};
    std::string 最近需求摘要{};
    std::vector<std::string> 最近事件列表{};
};

export class 自检线程类 {
public:
    自检线程类();
    ~自检线程类();

    自检线程类(const 自检线程类&) = delete;
    自检线程类& operator=(const 自检线程类&) = delete;

    bool 启动();
    void 请求停止();
    void 等待停止();
    void 请求休眠期自检();

    bool 是否健康运行() const noexcept;
    枚举_自检线程生命周期状态 读取生命周期状态() const noexcept;
    结构_自检线程摘要 读取摘要() const;
    std::vector<std::string> 读取最近事件列表(std::size_t 上限 = 32) const;

private:
    void 主循环_();
    void 执行运行中自检_(时间戳 now);
    void 执行休眠期自检_(时间戳 now);
    void 记录事件_已加锁(时间戳 now, const std::string& 摘要);

private:
    mutable std::mutex 状态锁_{};
    std::condition_variable 唤醒条件_{};
    std::thread 工作线程_{};
    std::atomic_bool 停止请求_{false};
    std::atomic_bool 休眠期自检请求_{false};

    结构_自检线程配置 配置_{};
    枚举_自检线程生命周期状态 生命周期_ = 枚举_自检线程生命周期状态::未启动;
    枚举_自检线程模式 最近模式_ = 枚举_自检线程模式::运行中;
    bool 健康运行_ = false;
    std::uint64_t Tick计数_ = 0;
    std::uint64_t 累计提交需求数_ = 0;
    std::uint64_t 累计提交报告数_ = 0;
    std::uint64_t 累计需求化失败数_ = 0;
    std::uint64_t 累计发现问题数_ = 0;
    时间戳 最近检查时间_ = 0;
    时间戳 最近提交时间_ = 0;
    std::string 最近需求摘要_{};
    std::deque<std::string> 最近事件_{};
    std::unordered_map<std::string, 时间戳> 去重账_{};
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> 最近检查时刻_{};
    std::size_t 方法扫描游标_ = 0;
    std::size_t 休眠方法扫描游标_ = 0;
    std::uint64_t 休眠批次号_ = 0;
};

export 自检线程类& 获取全局自检线程() noexcept;
export bool 启动自检线程();
export void 停止自检线程();
export void 请求休眠期自检();
export 结构_自检线程摘要 读取自检线程摘要();
export const char* 自检线程生命周期文本(枚举_自检线程生命周期状态 状态) noexcept;
export const char* 自检线程模式文本(枚举_自检线程模式 模式) noexcept;
