module;

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "基础数据类型.h"
#include "基础信息节点类型.h"
#include "任务主信息类.h"
#include "本能方法类.h"
#include "日志接入.h"

export module 动作线程模块;

import 本能动作管理模块;

export {

class 世界树类;
class 三维场景管理类;

struct 动作执行上下文 {
    世界树类* 世界树 = nullptr;
    三维场景管理类* 场景管理 = nullptr;

    任务主信息类* 任务 = nullptr;
    void* 任务节点 = nullptr;

    场景节点类* 场景 = nullptr;
    场景节点类* 输入场景 = nullptr;
    场景节点类* 输出场景 = nullptr;
    存在节点类* 目标存在 = nullptr;

    void* 方法根 = nullptr;
    std::uint64_t 请求ID = 0;
};

struct 结构_动作步骤结果 {
    枚举_本能方法ID 动作ID = 枚举_本能方法ID::未定义;

    std::int64_t 成功码 = 0;
    std::int64_t 质量_Q10000 = 0;
    std::uint64_t 成本_us = 0;
    std::int64_t 成本_Q10000 = 0;
    场景节点类* 结果存在集场景 = nullptr;
    场景节点类* 结果事件集场景 = nullptr;

    std::string 备注{};
};

using 结构体_动作步骤结果 = 结构_动作步骤结果;

struct 结构_动作执行结果 {
    std::uint64_t 请求ID = 0;
    std::int64_t 总成功码 = 0;

    时间戳 开始_us = 0;
    时间戳 结束_us = 0;

    std::vector<结构_动作步骤结果> 步骤结果{};

    [[nodiscard]] std::uint64_t 总耗时_us() const noexcept
    {
        return (结束_us >= 开始_us) ? (结束_us - 开始_us) : 0;
    }
};

using 结构体_动作执行结果 = 结构_动作执行结果;

struct 结构_动作执行请求 {
    std::uint64_t 请求ID = 0;
    std::vector<枚举_本能方法ID> 执行序列{};

    场景节点类* 场景 = nullptr;
    场景节点类* 输入场景 = nullptr;
    场景节点类* 输出场景 = nullptr;
    存在节点类* 目标存在 = nullptr;
    void* 任务节点 = nullptr;
    void* 方法根 = nullptr;

    bool 失败继续 = false;
};

using 结构体_动作执行请求 = 结构_动作执行请求;

class 动作线程类 {
public:
    using 最小动作函数 = std::function<结构_动作步骤结果(动作执行上下文&)>;

private:
    struct 队列项 {
        结构_动作执行请求 请求{};
        std::promise<结构_动作执行结果> 承诺{};
    };

    using 函数句柄 = std::shared_ptr<const 最小动作函数>;
    using 注册表 = std::unordered_map<std::uint32_t, 函数句柄>;

    static constexpr std::int64_t 错误_未启动 = -10000;
    static constexpr std::int64_t 错误_动作未注册 = -10001;
    static constexpr std::int64_t 错误_动作异常 = -10002;
    static constexpr std::int64_t 错误_动作未知异常 = -10003;
    static constexpr std::int64_t 错误_队列已满 = -10004;

    std::atomic<世界树类*> 世界树_{ nullptr };
    std::atomic<三维场景管理类*> 场景管理_{ nullptr };

    mutable std::mutex 注册表写锁_{};
    std::atomic<std::shared_ptr<const 注册表>> 注册表快照_{ std::make_shared<const 注册表>() };

    std::mutex 队列锁_{};
    std::condition_variable 队列条件_{};
    std::queue<队列项> 队列_{};
    std::atomic<std::size_t> 队列上限_{ 0 };

    std::atomic<bool> 停止请求_{ false };
    std::atomic<bool> 运行中_{ false };
    std::thread 工作线程_{};

    std::atomic<std::uint64_t> 下一个请求ID_{ 1 };

public:
    动作线程类() = default;
    ~动作线程类() { 停止(true); }

    动作线程类(const 动作线程类&) = delete;
    动作线程类& operator=(const 动作线程类&) = delete;

public:
    void 绑定环境(世界树类* 世界树, 三维场景管理类* 场景管理) noexcept
    {
        世界树_.store(世界树, std::memory_order_release);
        场景管理_.store(场景管理, std::memory_order_release);
    }

    void 设置队列上限(std::size_t 上限) noexcept
    {
        队列上限_.store(上限, std::memory_order_release);
    }

    [[nodiscard]] std::size_t 取队列上限() const noexcept
    {
        return 队列上限_.load(std::memory_order_acquire);
    }

    [[nodiscard]] std::size_t 队列上限() const noexcept
    {
        return 取队列上限();
    }

    std::size_t 取队列深度()
    {
        std::lock_guard<std::mutex> 锁(队列锁_);
        return 队列_.size();
    }

    std::size_t 队列深度()
    {
        return 取队列深度();
    }

    [[nodiscard]] bool 是否运行中() const noexcept
    {
        return 运行中_.load(std::memory_order_acquire);
    }

    [[nodiscard]] bool 运行中() const noexcept
    {
        return 是否运行中();
    }

public:
    void 注册本能(枚举_本能方法ID 动作ID, 最小动作函数 动作函数)
    {
        if (!动作函数) {
            return;
        }

        auto 新函数句柄 = std::make_shared<const 最小动作函数>(std::move(动作函数));
        const auto 索引键 = static_cast<std::uint32_t>(动作ID);

        std::lock_guard<std::mutex> 锁(注册表写锁_);
        auto 当前快照 = 注册表快照_.load(std::memory_order_acquire);
        auto 新快照 = std::make_shared<注册表>(当前快照 ? *当前快照 : 注册表{});
        (*新快照)[索引键] = std::move(新函数句柄);
        注册表快照_.store(std::const_pointer_cast<const 注册表>(新快照), std::memory_order_release);
    }

    void 取消注册(枚举_本能方法ID 动作ID)
    {
        const auto 索引键 = static_cast<std::uint32_t>(动作ID);

        std::lock_guard<std::mutex> 锁(注册表写锁_);
        auto 当前快照 = 注册表快照_.load(std::memory_order_acquire);
        if (!当前快照 || 当前快照->empty()) {
            return;
        }

        auto 新快照 = std::make_shared<注册表>(*当前快照);
        新快照->erase(索引键);
        注册表快照_.store(std::const_pointer_cast<const 注册表>(新快照), std::memory_order_release);
    }

    [[nodiscard]] bool 已注册(枚举_本能方法ID 动作ID) const
    {
        const auto 索引键 = static_cast<std::uint32_t>(动作ID);
        auto 当前快照 = 注册表快照_.load(std::memory_order_acquire);
        return 当前快照 && 当前快照->find(索引键) != 当前快照->end();
    }

    [[nodiscard]] std::vector<枚举_本能方法ID> 列出已注册动作() const
    {
        std::vector<枚举_本能方法ID> 输出{};
        auto 当前快照 = 注册表快照_.load(std::memory_order_acquire);
        if (!当前快照 || 当前快照->empty()) {
            return 输出;
        }

        输出.reserve(当前快照->size());
        for (const auto& [键, _] : *当前快照) {
            输出.push_back(static_cast<枚举_本能方法ID>(键));
        }
        std::sort(输出.begin(), 输出.end(), [](const auto 左, const auto 右) {
            return static_cast<std::uint32_t>(左) < static_cast<std::uint32_t>(右);
        });
        return 输出;
    }

    bool 从本能动作管理器注册(枚举_本能方法ID 动作ID)
    {
        auto 已注册动作 = 取本能动作集().查询(动作ID);
        if (!已注册动作.has_value() || !已注册动作->调用点) {
            return false;
        }

        auto 动作上下文 = *已注册动作;
        注册本能(动作ID, [动作ID, 动作上下文 = std::move(动作上下文)](动作执行上下文& 上下文) mutable {
            结构_本能动作执行上下文 本能上下文{};
            本能上下文.世界树 = 上下文.世界树;
            本能上下文.场景管理 = 上下文.场景管理;
            本能上下文.任务 = 上下文.任务;
            本能上下文.任务节点 = 上下文.任务节点;
            本能上下文.场景 = 上下文.场景;
            本能上下文.输入场景 = 上下文.输入场景;
            本能上下文.输出场景 = 上下文.输出场景;
            本能上下文.目标存在 = 上下文.目标存在;
            本能上下文.方法根 = 上下文.方法根;
            本能上下文.请求ID = 上下文.请求ID;

            const auto 本能结果 = 动作上下文.调用点(本能上下文);

            结构_动作步骤结果 输出{};
            输出.动作ID = 本能结果.动作ID == 枚举_本能方法ID::未定义 ? 动作ID : 本能结果.动作ID;
            输出.成功码 = 本能结果.成功码;
            输出.质量_Q10000 = 本能结果.质量_Q10000;
            输出.成本_us = 本能结果.成本_us;
            输出.成本_Q10000 = 本能结果.成本_Q10000;
            输出.结果存在集场景 = 本能结果.结果存在集场景;
            输出.结果事件集场景 = 本能结果.结果事件集场景;
            输出.备注 = 本能结果.备注;
            return 输出;
        });
        return true;
    }

    std::size_t 从本能动作管理器批量注册(
        const std::vector<枚举_本能方法ID>& 动作ID列表 = {})
    {
        const auto 待注册列表 = 动作ID列表.empty() ? 取本能动作集().枚举已注册ID() : 动作ID列表;

        std::size_t 已注册数量 = 0;
        for (const auto 动作ID : 待注册列表) {
            if (从本能动作管理器注册(动作ID)) {
                ++已注册数量;
            }
        }
        return 已注册数量;
    }

public:
    void 启动()
    {
        if (工作线程_.joinable() && !运行中_.load(std::memory_order_acquire)) {
            等待线程结束();
        }

        if (运行中_.exchange(true, std::memory_order_acq_rel)) {
            return;
        }

        停止请求_.store(false, std::memory_order_release);
        工作线程_ = std::thread([this] { 线程函数_(); });
        项目运行日志("[动作线程] 启动");
    }

    void 请求退出() noexcept
    {
        停止请求_.store(true, std::memory_order_release);
        队列条件_.notify_all();
    }

    void 等待线程结束()
    {
        if (!工作线程_.joinable()) {
            return;
        }
        if (std::this_thread::get_id() == 工作线程_.get_id()) {
            return;
        }
        工作线程_.join();
    }

    void 停止(bool 等待 = true)
    {
        if (!工作线程_.joinable() && !运行中_.load(std::memory_order_acquire)) {
            return;
        }

        请求退出();
        if (!等待) {
            return;
        }

        等待线程结束();
        运行中_.store(false, std::memory_order_release);
        项目运行日志("[动作线程] 停止");
    }

public:
    std::future<结构_动作执行结果> 提交(const 结构_动作执行请求& 请求)
    {
        auto 副本 = 请求;
        return 提交(std::move(副本));
    }

    std::future<结构_动作执行结果> 提交(结构_动作执行请求&& 请求)
    {
        if (请求.请求ID == 0) {
            请求.请求ID = 下一个请求ID_.fetch_add(1, std::memory_order_acq_rel);
        }

        if (!运行中_.load(std::memory_order_acquire) || 停止请求_.load(std::memory_order_acquire)) {
            return 立即返回失败_(请求.请求ID, 错误_未启动, "动作线程未启动或正在退出");
        }

        const auto 队列上限 = 队列上限_.load(std::memory_order_acquire);

        队列项 项{};
        项.请求 = std::move(请求);
        auto future = 项.承诺.get_future();

        {
            std::lock_guard<std::mutex> 锁(队列锁_);

            if (停止请求_.load(std::memory_order_acquire)) {
                return 立即返回失败_(项.请求.请求ID, 错误_未启动, "动作线程正在退出");
            }

            if (队列上限 > 0 && 队列_.size() >= 队列上限) {
                return 立即返回失败_(项.请求.请求ID, 错误_队列已满, "动作队列已满");
            }

            队列_.push(std::move(项));
        }

        队列条件_.notify_one();
        return future;
    }

    std::future<结构_动作执行结果> 提交执行序列(
        std::vector<枚举_本能方法ID> 执行序列,
        场景节点类* 场景,
        场景节点类* 输出场景 = nullptr,
        存在节点类* 目标存在 = nullptr,
        bool 失败继续 = false,
        void* 任务节点 = nullptr,
        void* 方法根 = nullptr)
    {
        结构_动作执行请求 请求{};
        请求.执行序列 = std::move(执行序列);
        请求.场景 = 场景;
        请求.输入场景 = 场景;
        请求.输出场景 = 输出场景 ? 输出场景 : 场景;
        请求.目标存在 = 目标存在;
        请求.失败继续 = 失败继续;
        请求.任务节点 = 任务节点;
        请求.方法根 = 方法根;
        return 提交(std::move(请求));
    }

private:
    static std::future<结构_动作执行结果> 立即返回失败_(
        std::uint64_t 请求ID,
        std::int64_t 错误码,
        std::string 备注)
    {
        std::promise<结构_动作执行结果> 承诺{};
        auto future = 承诺.get_future();

        结构_动作执行结果 结果{};
        结果.请求ID = 请求ID;
        结果.总成功码 = 错误码;
        结果.开始_us = 结构体_时间戳::当前_微秒();
        结果.结束_us = 结果.开始_us;

        if (!备注.empty()) {
            结构_动作步骤结果 步骤{};
            步骤.动作ID = 枚举_本能方法ID::未定义;
            步骤.成功码 = 错误码;
            步骤.备注 = std::move(备注);
            结果.步骤结果.push_back(std::move(步骤));
        }

        承诺.set_value(std::move(结果));
        return future;
    }

    [[nodiscard]] 函数句柄 取动作函数_(枚举_本能方法ID 动作ID) const
    {
        const auto 索引键 = static_cast<std::uint32_t>(动作ID);
        auto 当前快照 = 注册表快照_.load(std::memory_order_acquire);
        if (!当前快照) {
            return {};
        }

        const auto it = 当前快照->find(索引键);
        return it != 当前快照->end() ? it->second : 函数句柄{};
    }

    void 线程函数_()
    {
        while (true) {
            队列项 项{};
            bool 取到任务 = false;

            {
                std::unique_lock<std::mutex> 锁(队列锁_);
                队列条件_.wait(锁, [this] {
                    return 停止请求_.load(std::memory_order_acquire) || !队列_.empty();
                });

                if (!队列_.empty()) {
                    项 = std::move(队列_.front());
                    队列_.pop();
                    取到任务 = true;
                } else if (停止请求_.load(std::memory_order_acquire)) {
                    break;
                }
            }

            if (!取到任务) {
                continue;
            }

            结构_动作执行结果 结果{};
            结果.请求ID = 项.请求.请求ID;
            结果.开始_us = 结构体_时间戳::当前_微秒();

            动作执行上下文 上下文{};
            上下文.世界树 = 世界树_.load(std::memory_order_acquire);
            上下文.场景管理 = 场景管理_.load(std::memory_order_acquire);
            上下文.任务节点 = 项.请求.任务节点;
            上下文.输入场景 = 项.请求.输入场景 ? 项.请求.输入场景 : 项.请求.场景;
            上下文.输出场景 = 项.请求.输出场景 ? 项.请求.输出场景 : 上下文.输入场景;
            上下文.场景 = 上下文.输出场景 ? 上下文.输出场景 : 上下文.输入场景;
            上下文.目标存在 = 项.请求.目标存在;
            上下文.方法根 = 项.请求.方法根;
            上下文.请求ID = 结果.请求ID;

            const bool 首错即停 = !项.请求.失败继续;

            for (const auto 动作ID : 项.请求.执行序列) {
                auto 动作函数 = 取动作函数_(动作ID);
                if (!动作函数) {
                    结构_动作步骤结果 步骤{};
                    步骤.动作ID = 动作ID;
                    步骤.成功码 = 错误_动作未注册;
                    步骤.备注 = "动作未注册";
                    结果.步骤结果.push_back(std::move(步骤));

                    if (结果.总成功码 == 0) {
                        结果.总成功码 = 错误_动作未注册;
                    }
                    if (首错即停) {
                        break;
                    }
                    continue;
                }

                try {
                    auto 开始时间 = 结构体_时间戳::当前_微秒();
                    auto 步骤 = (*动作函数)(上下文);
                    auto 结束时间 = 结构体_时间戳::当前_微秒();

                    if (步骤.动作ID == 枚举_本能方法ID::未定义) {
                        步骤.动作ID = 动作ID;
                    }
                    if (步骤.成本_us == 0 && 结束时间 >= 开始时间) {
                        步骤.成本_us = 结束时间 - 开始时间;
                    }
                    if (步骤.成功码 != 0 && 结果.总成功码 == 0) {
                        结果.总成功码 = 步骤.成功码;
                    }
                    结果.步骤结果.push_back(std::move(步骤));

                    if (首错即停 && 结果.总成功码 != 0) {
                        break;
                    }
                }
                catch (const std::exception& 异常) {
                    结构_动作步骤结果 步骤{};
                    步骤.动作ID = 动作ID;
                    步骤.成功码 = 错误_动作异常;
                    步骤.备注 = 异常.what();
                    结果.步骤结果.push_back(std::move(步骤));
                    项目记录异常日志(异常, "动作线程类::线程函数_");

                    if (结果.总成功码 == 0) {
                        结果.总成功码 = 错误_动作异常;
                    }
                    if (首错即停) {
                        break;
                    }
                }
                catch (...) {
                    结构_动作步骤结果 步骤{};
                    步骤.动作ID = 动作ID;
                    步骤.成功码 = 错误_动作未知异常;
                    步骤.备注 = "动作抛出未知异常";
                    结果.步骤结果.push_back(std::move(步骤));
                    项目运行错误日志("动作线程类::线程函数_ 捕获未知异常");

                    if (结果.总成功码 == 0) {
                        结果.总成功码 = 错误_动作未知异常;
                    }
                    if (首错即停) {
                        break;
                    }
                }
            }

            结果.结束_us = 结构体_时间戳::当前_微秒();
            项.承诺.set_value(std::move(结果));
        }

        运行中_.store(false, std::memory_order_release);
        项目运行日志("[动作线程] 线程退出");
    }
};

} // export
