module;

#include <algorithm>
#include <cstdint>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "基础信息节点类型.h"
#include "本能方法类.h"

export module 本能动作管理模块;

namespace 本能动作管理模块_detail {
    inline std::string 默认函数名称(枚举_本能方法ID 动作ID)
    {
        const auto* 元信息 = 本能方法类::查询元信息(动作ID);
        if (!元信息 || !元信息->动作名称) {
            return {};
        }
        return 元信息->动作名称;
    }

    inline std::vector<枚举_本能方法ID> 默认动作ID列表()
    {
        return 本能方法类::枚举默认自我本能方法ID();
    }
}

export {

using 枚举_本能动作ID = 枚举_本能方法ID;

inline constexpr std::int64_t 本能动作错误_未注册 = -20001;
inline constexpr std::int64_t 本能动作错误_执行失败 = -20002;
inline constexpr std::int64_t 本能动作错误_动作异常 = -20003;
inline constexpr std::int64_t 本能动作错误_动作未知异常 = -20004;

struct 结构_本能动作执行上下文 {
    void* 世界树 = nullptr;
    void* 场景管理 = nullptr;

    void* 任务 = nullptr;
    void* 任务节点 = nullptr;

    场景节点类* 场景 = nullptr;
    场景节点类* 输入场景 = nullptr;
    场景节点类* 输出场景 = nullptr;
    存在节点类* 目标存在 = nullptr;

    void* 方法根 = nullptr;
    std::uint64_t 请求ID = 0;
};

struct 结构_本能动作步骤结果 {
    枚举_本能动作ID 动作ID = 枚举_本能动作ID::未定义;

    std::int64_t 成功码 = 0;
    std::int64_t 质量_Q10000 = 0;
    std::uint64_t 成本_us = 0;
    std::int64_t 成本_Q10000 = 0;

    std::string 备注{};
};

using 本能动作函数 = std::function<结构_本能动作步骤结果(结构_本能动作执行上下文&)>;
using 兼容场景本能函数 = std::function<bool(场景节点类* 输入场景, 场景节点类* 输出场景)>;
using 本能函数 = 本能动作函数;

struct 结构体_本能动作上下文 {
    本能动作函数 调用点{};
    void* 方法信息首节点 = nullptr;
    std::string 函数名称{};
};

class 本能动作管理类 {
public:
    bool 注册(
        枚举_本能动作ID 动作ID,
        本能动作函数 动作函数,
        std::string 函数名称 = {},
        void* 方法首节点 = nullptr)
    {
        if (动作ID == 枚举_本能动作ID::未定义) return false;
        if (!动作函数) return false;
        if (函数名称.empty()) {
            函数名称 = 本能动作管理模块_detail::默认函数名称(动作ID);
        }
        if (函数名称.empty()) return false;

        结构体_本能动作上下文 上下文{};
        上下文.调用点 = std::move(动作函数);
        上下文.方法信息首节点 = 方法首节点;
        上下文.函数名称 = std::move(函数名称);

        std::lock_guard 锁(mu_);
        表_[动作ID] = std::move(上下文);
        return true;
    }

    bool 注册_兼容(
        枚举_本能动作ID 动作ID,
        兼容场景本能函数 动作函数,
        std::string 函数名称 = {},
        void* 方法首节点 = nullptr)
    {
        if (!动作函数) return false;

        return 注册(
            动作ID,
            [动作函数 = std::move(动作函数), 动作ID](结构_本能动作执行上下文& 上下文) mutable {
                结构_本能动作步骤结果 结果{};
                结果.动作ID = 动作ID;
                if (动作函数(上下文.输入场景, 上下文.输出场景)) {
                    return 结果;
                }

                结果.成功码 = 本能动作错误_执行失败;
                结果.备注 = "兼容场景本能函数返回失败";
                return 结果;
            },
            std::move(函数名称),
            方法首节点);
    }

    bool 补全方法信息(枚举_本能动作ID 动作ID, void* 方法首节点)
    {
        std::lock_guard 锁(mu_);
        const auto it = 表_.find(动作ID);
        if (it == 表_.end()) return false;
        it->second.方法信息首节点 = 方法首节点;
        return true;
    }

    bool 注销(枚举_本能动作ID 动作ID)
    {
        std::lock_guard 锁(mu_);
        return 表_.erase(动作ID) > 0;
    }

    bool 有(枚举_本能动作ID 动作ID) const
    {
        std::lock_guard 锁(mu_);
        return 表_.find(动作ID) != 表_.end();
    }

    std::optional<结构体_本能动作上下文> 查询(枚举_本能动作ID 动作ID) const
    {
        std::lock_guard 锁(mu_);
        const auto it = 表_.find(动作ID);
        if (it == 表_.end()) return std::nullopt;
        return it->second;
    }

    std::optional<std::pair<枚举_本能动作ID, 结构体_本能动作上下文>> 查询_按函数名(const std::string& 函数名称) const
    {
        if (函数名称.empty()) return std::nullopt;

        std::lock_guard 锁(mu_);
        for (const auto& [动作ID, 上下文] : 表_) {
            if (上下文.函数名称 == 函数名称) {
                return std::make_pair(动作ID, 上下文);
            }
        }
        return std::nullopt;
    }

    std::string 查询函数名称(枚举_本能动作ID 动作ID) const
    {
        {
            std::lock_guard 锁(mu_);
            const auto it = 表_.find(动作ID);
            if (it != 表_.end() && !it->second.函数名称.empty()) {
                return it->second.函数名称;
            }
        }
        return 本能动作管理模块_detail::默认函数名称(动作ID);
    }

    枚举_本能动作ID 查询ID(const std::string& 函数名称) const
    {
        if (函数名称.empty()) return 枚举_本能动作ID::未定义;

        {
            std::lock_guard 锁(mu_);
            for (const auto& [动作ID, 上下文] : 表_) {
                if (上下文.函数名称 == 函数名称) {
                    return 动作ID;
                }
            }
        }

        for (const auto 动作ID : 本能动作管理模块_detail::默认动作ID列表()) {
            if (本能动作管理模块_detail::默认函数名称(动作ID) == 函数名称) {
                return 动作ID;
            }
        }
        return 枚举_本能动作ID::未定义;
    }

    std::vector<枚举_本能动作ID> 枚举已注册ID() const
    {
        std::vector<枚举_本能动作ID> 输出{};
        std::lock_guard 锁(mu_);
        输出.reserve(表_.size());
        for (const auto& [动作ID, _] : 表_) {
            输出.push_back(动作ID);
        }
        std::sort(输出.begin(), 输出.end(), [](const auto 左, const auto 右) {
            return static_cast<std::uint32_t>(左) < static_cast<std::uint32_t>(右);
        });
        return 输出;
    }

    bool 运行(
        枚举_本能动作ID 动作ID,
        结构_本能动作执行上下文& 上下文,
        结构_本能动作步骤结果* 输出 = nullptr) const noexcept
    {
        auto 已注册动作 = 查询(动作ID);
        if (!已注册动作.has_value() || !已注册动作->调用点) {
            if (输出) {
                输出->动作ID = 动作ID;
                输出->成功码 = 本能动作错误_未注册;
                输出->备注 = "本能动作未注册";
            }
            return false;
        }

        try {
            auto 结果 = 已注册动作->调用点(上下文);
            if (结果.动作ID == 枚举_本能动作ID::未定义) {
                结果.动作ID = 动作ID;
            }
            const bool 成功 = 结果.成功码 == 0;
            if (输出) {
                *输出 = std::move(结果);
            }
            return 成功;
        }
        catch (const std::exception& 异常) {
            if (输出) {
                输出->动作ID = 动作ID;
                输出->成功码 = 本能动作错误_动作异常;
                输出->备注 = 异常.what();
            }
            return false;
        }
        catch (...) {
            if (输出) {
                输出->动作ID = 动作ID;
                输出->成功码 = 本能动作错误_动作未知异常;
                输出->备注 = "本能动作抛出未知异常";
            }
            return false;
        }
    }

    bool 运行_兼容(
        枚举_本能动作ID 动作ID,
        场景节点类* 输入场景,
        场景节点类* 输出场景,
        结构_本能动作步骤结果* 输出 = nullptr) const noexcept
    {
        结构_本能动作执行上下文 上下文{};
        上下文.输入场景 = 输入场景;
        上下文.输出场景 = 输出场景 ? 输出场景 : 输入场景;
        上下文.场景 = 上下文.输出场景 ? 上下文.输出场景 : 上下文.输入场景;
        return 运行(动作ID, 上下文, 输出);
    }

    void 调试清空运行期缓存() noexcept
    {
        std::lock_guard 锁(mu_);
        表_.clear();
    }

private:
    mutable std::mutex mu_{};
    std::unordered_map<枚举_本能动作ID, 结构体_本能动作上下文> 表_{};
};

inline const 结构_本能方法元信息* 查询本能动作元信息(枚举_本能动作ID 动作ID) noexcept
{
    return 本能方法类::查询元信息(动作ID);
}

inline std::vector<枚举_本能动作ID> 枚举默认本能动作ID()
{
    return 本能方法类::枚举默认自我本能方法ID();
}

inline 本能动作管理类& 取本能动作集() noexcept
{
    static 本能动作管理类 s_实例{};
    return s_实例;
}

} // export
