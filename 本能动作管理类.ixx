module;

#include <algorithm>
#include <cstdint>
#include <exception>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "基础信息节点类型.h"
#include "本能方法类.h"
#include "方法类.h"
#include "方法虚拟存在服务类.h"
#include "世界树类.h"
#include "语素类.h"
#include "语素节点类型.h"

export module 本能动作管理模块;

import 本能动作模块;

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

} // namespace 本能动作管理模块_detail

export using 枚举_本能动作ID = 枚举_本能方法ID;

export {

inline constexpr std::int64_t 本能动作错误_未注册 = -20001;
inline constexpr std::int64_t 本能动作错误_执行失败 = -20002;
inline constexpr std::int64_t 本能动作错误_动作异常 = -20003;
inline constexpr std::int64_t 本能动作错误_动作未知异常 = -20004;
inline constexpr std::int64_t 本能动作错误_方法首节点缺失 = -20005;
inline constexpr std::int64_t 本能动作错误_方法节点类型错误 = -20006;
inline constexpr std::int64_t 本能动作错误_缺动作入口 = -20007;
inline constexpr std::int64_t 本能动作错误_动作入口不归本类执行 = -20008;
inline constexpr std::int64_t 本能动作错误_输入参数不满足 = -20009;

inline constexpr std::int64_t 本能动作运行结果_成功 = 0;
inline constexpr std::int64_t 本能动作运行结果_未知 = -1;

const 词性节点类* 本能动作运行结果语义词(const char* 文本) noexcept
{
    return 语素集.添加词性词(文本 ? 文本 : "运行结果未知", "名词");
}

const 词性节点类* 本能动作运行结果码特征词() noexcept
{
    return 语素集.添加词性词("本能动作运行结果码", "名词");
}

const 词性节点类* 本能动作运行开始时间特征词() noexcept
{
    return 语素集.添加词性词("本能动作运行开始时间", "名词");
}

const 词性节点类* 本能动作运行虚拟存在类型词() noexcept
{
    return 语素集.添加词性词("本能动作运行虚拟存在", "名词");
}

const 词性节点类* 本能动作方法执行情况特征词() noexcept
{
    return 语素集.添加词性词("方法执行情况", "名词");
}

const 词性节点类* 本能动作执行成功词() noexcept
{
    return 语素集.添加词性词("执行成功", "名词");
}

inline const char* 本能动作运行结果语义_按码(std::int64_t 运行码) noexcept
{
    switch (运行码) {
    case 本能动作运行结果_成功: return "运行成功";
    case 本能动作错误_未注册: return "本能动作未注册";
    case 本能动作错误_执行失败: return "执行失败";
    case 本能动作错误_动作异常: return "执行异常";
    case 本能动作错误_动作未知异常: return "执行未知异常";
    case 本能动作错误_方法首节点缺失: return "方法首节点缺失";
    case 本能动作错误_方法节点类型错误: return "方法节点类型错误";
    case 本能动作错误_缺动作入口: return "动作入口缺失";
    case 本能动作错误_动作入口不归本类执行: return "动作入口不归本类执行";
    case 本能动作错误_输入参数不满足: return "输入参数不满足";
    case 本能方法结果_任务筹办已建立方法头待重入: return "任务筹办待重入";
    case 本能方法结果_任务缺条件需自我线程: return "任务缺条件";
    default: return 运行码 < 0 ? "运行失败" : "运行成功";
    }
}

抽象特征节点类* 取或创建本能动作运行结果抽象特征(const char* 语义) noexcept
{
    auto* 抽象根 = 世界树.取或创建抽象特征根();
    if (!抽象根) {
        return nullptr;
    }
    auto* 运行结果父 = 世界树.取或创建抽象特征_按类型(
        抽象根,
        本能动作运行结果语义词("本能动作运行结果"));
    if (!运行结果父) {
        return nullptr;
    }
    return 世界树.取或创建抽象特征_按类型(
        运行结果父,
        本能动作运行结果语义词(语义));
}

基础信息节点类* 本能动作运行结果宿主(
    方法类::节点类* 方法首节点,
    场景节点类* 输出结果场景,
    时间戳 now = 结构体_时间戳::当前_微秒()) noexcept
{
    if (方法首节点 && 方法首节点->主信息.节点种类() == 枚举_方法节点种类::方法首节点) {
        const auto* 首节点信息 = 方法首节点->主信息.取首节点信息();
        if (auto* 方法虚拟存在 = 首节点信息 ? 首节点信息->方法虚拟存在.指针 : nullptr) {
            return reinterpret_cast<基础信息节点类*>(方法虚拟存在);
        }
        if (auto* 方法虚拟存在 = 方法虚拟存在服务类::取或创建方法虚拟存在(方法首节点, nullptr, now)) {
            return reinterpret_cast<基础信息节点类*>(方法虚拟存在);
        }
    }
    return reinterpret_cast<基础信息节点类*>(输出结果场景);
}

存在节点类* 创建本能动作运行结果实例(
    方法类::节点类* 方法首节点,
    场景节点类* 输出结果场景,
    const char* 语义,
    std::int64_t 运行码,
    时间戳 now = 结构体_时间戳::当前_微秒()) noexcept
{
    auto* 宿主 = 本能动作运行结果宿主(方法首节点, 输出结果场景, now);
    if (!宿主) {
        return nullptr;
    }
    auto* 运行存在 = 世界树.取或创建子存在_按类型并按特征I64(
        宿主,
        本能动作运行虚拟存在类型词(),
        本能动作运行开始时间特征词(),
        static_cast<I64>(now));
    if (!运行存在) {
        return nullptr;
    }
    (void)世界树.写入特征_I64(
        reinterpret_cast<基础信息节点类*>(运行存在),
        本能动作运行结果码特征词(),
        static_cast<I64>(运行码),
        now);
    (void)世界树.写入特征_指针(
        reinterpret_cast<基础信息节点类*>(运行存在),
        本能动作方法执行情况特征词(),
        本能动作运行结果语义词(语义),
        now);
    return 运行存在;
}

存在节点类* 创建本能动作运行结果实例_按码(
    方法类::节点类* 方法首节点,
    场景节点类* 输出结果场景,
    std::int64_t 运行码,
    时间戳 now = 结构体_时间戳::当前_微秒()) noexcept
{
    return 创建本能动作运行结果实例(
        方法首节点,
        输出结果场景,
        本能动作运行结果语义_按码(运行码),
        运行码,
        now);
}

std::int64_t 本能动作运行结果值(const 存在节点类* 运行结果存在) noexcept
{
    if (!运行结果存在) {
        return 本能动作运行结果_未知;
    }
    I64 输出值 = 本能动作运行结果_未知;
    if (世界树.读取特征_I64(
            reinterpret_cast<const 基础信息节点类*>(运行结果存在),
            本能动作运行结果码特征词(),
            输出值)) {
        return static_cast<std::int64_t>(输出值);
    }
    if (世界树.读取特征_I64(运行结果存在, 输出值)) {
        return static_cast<std::int64_t>(输出值);
    }

    void* 执行情况 = nullptr;
    if (世界树.读取特征_指针(
            reinterpret_cast<const 基础信息节点类*>(运行结果存在),
            本能动作方法执行情况特征词(),
            执行情况)) {
        return 执行情况 == 本能动作执行成功词()
            ? 本能动作运行结果_成功
            : 本能动作错误_执行失败;
    }
    return 本能动作运行结果_未知;
}

bool 本能动作运行成功(const 存在节点类* 运行结果存在) noexcept
{
    return 本能动作运行结果值(运行结果存在) == 本能动作运行结果_成功;
}

std::string 本能动作运行结果文本(const 存在节点类* 运行结果存在)
{
    const auto 运行码 = 本能动作运行结果值(运行结果存在);
    return 本能动作运行结果语义_按码(运行码);
}

using 本能动作函数 = std::function<存在节点类*(
    方法类::节点类* 方法首节点,
    场景节点类* 输入参数场景,
    场景节点类* 输出结果场景)>;
using 本能函数 = 本能动作函数;

struct 结构体_本能动作登记信息 {
    本能动作函数 动作函数{};
    方法类::节点类* 方法信息首节点 = nullptr;
    std::string 函数名称{};
};

class 本能动作管理类 {
public:
    bool 注册(
        枚举_本能动作ID 动作ID,
        本能动作函数 动作函数,
        std::string 函数名称 = {},
        方法类::节点类* 方法首节点 = nullptr)
    {
        if (动作ID == 枚举_本能动作ID::未定义) return false;
        if (!动作函数) return false;
        if (函数名称.empty()) {
            函数名称 = 本能动作管理模块_detail::默认函数名称(动作ID);
        }
        if (函数名称.empty()) return false;

        结构体_本能动作登记信息 登记信息{};
        登记信息.动作函数 = std::move(动作函数);
        登记信息.方法信息首节点 = 方法首节点;
        登记信息.函数名称 = std::move(函数名称);

        std::lock_guard 锁(mu_);
        表_[动作ID] = std::move(登记信息);
        return true;
    }

    bool 有(枚举_本能动作ID 动作ID) const
    {
        std::lock_guard 锁(mu_);
        return 表_.find(动作ID) != 表_.end();
    }

    std::optional<结构体_本能动作登记信息> 查询(枚举_本能动作ID 动作ID) const
    {
        std::lock_guard 锁(mu_);
        const auto it = 表_.find(动作ID);
        if (it == 表_.end()) return std::nullopt;
        return it->second;
    }

    std::optional<std::pair<枚举_本能动作ID, 结构体_本能动作登记信息>> 查询_按函数名(const std::string& 函数名称) const
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

    存在节点类* 检查是否可调用(
        枚举_本能动作ID 动作ID,
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景 = nullptr) const noexcept
    {
        auto 设置结果 = [&](std::int64_t 运行码) {
            return 创建本能动作运行结果实例_按码(
                方法首节点,
                输出结果场景,
                运行码);
        };

        if (动作ID == 枚举_本能动作ID::未定义) {
            return 设置结果(本能动作错误_未注册);
        }

        const auto 已注册动作 = 查询(动作ID);
        if (!已注册动作.has_value() || !已注册动作->动作函数) {
            return 设置结果(本能动作错误_未注册);
        }

        auto* 方法 = 方法首节点 ? 方法首节点 : 已注册动作->方法信息首节点;
        if (!方法) {
            return 设置结果(本能动作错误_方法首节点缺失);
        }
        if (方法->主信息.节点种类() != 枚举_方法节点种类::方法首节点) {
            return 设置结果(本能动作错误_方法节点类型错误);
        }

        if (!方法->主信息.有动作骨架()) {
            return 设置结果(本能动作错误_缺动作入口);
        }

        const auto* 首节点信息 = 方法->主信息.取首节点信息();
        if (!首节点信息) {
            return 设置结果(本能动作错误_方法节点类型错误);
        }

        const auto& 动作句柄 = 首节点信息->动作句柄;
        if (动作句柄.有效()) {
            if (动作句柄.类型 != 枚举_动作句柄类型::本能函数ID) {
                return 设置结果(本能动作错误_动作入口不归本类执行);
            }
            if (动作句柄.本能ID == 0 || static_cast<枚举_本能动作ID>(动作句柄.本能ID) != 动作ID) {
                return 设置结果(本能动作错误_缺动作入口);
            }
        }
        else if (首节点信息->动作名) {
            const auto 解析ID = 查询ID(首节点信息->动作名->获取主键());
            if (解析ID == 枚举_本能动作ID::未定义 || 解析ID != 动作ID) {
                return 设置结果(本能动作错误_缺动作入口);
            }
        }

        场景节点类* 条件场景 = nullptr;
        bool 有条件判定索引 = false;
        if (auto* 条件节点信息 = 方法->主信息.取条件节点信息()) {
            条件场景 = 条件节点信息->条件场景.指针;
            有条件判定索引 = !条件节点信息->条件判定索引.empty();
        }
        const bool 需要输入参数 = 条件场景 || 有条件判定索引;
        if (!需要输入参数) {
            return 创建本能动作运行结果实例(方法, 输出结果场景, "运行准备完成", 本能动作运行结果_成功);
        }
        if (!条件场景) {
            return 设置结果(本能动作错误_输入参数不满足);
        }
        if (!输入参数场景) {
            return 设置结果(本能动作错误_输入参数不满足);
        }

        const auto 匹配结果 = 本能动作类::场景_模式匹配(输入参数场景, 条件场景);
        if (!匹配结果.匹配成功) {
            return 设置结果(本能动作错误_输入参数不满足);
        }
        return 创建本能动作运行结果实例(方法, 输出结果场景, "运行准备完成", 本能动作运行结果_成功);
    }

    存在节点类* 执行功能函数(
        枚举_本能动作ID 动作ID,
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) const noexcept
    {
        auto* 检查结果 = 检查是否可调用(动作ID, 方法首节点, 输入参数场景, 输出结果场景);
        if (!本能动作运行成功(检查结果)) {
            return 检查结果;
        }

        const auto 已注册动作 = 查询(动作ID);
        auto* 方法 = 方法首节点 ? 方法首节点 : (已注册动作 ? 已注册动作->方法信息首节点 : nullptr);
        if (!已注册动作.has_value() || !已注册动作->动作函数) {
            return 创建本能动作运行结果实例_按码(
                方法,
                输出结果场景,
                本能动作错误_未注册);
        }

        try {
            if (auto* 运行结果 = 已注册动作->动作函数(方法, 输入参数场景, 输出结果场景)) {
                const auto 运行码 = 本能动作运行结果值(运行结果);
                (void)世界树.写入特征_I64(
                    reinterpret_cast<基础信息节点类*>(运行结果),
                    本能动作运行结果码特征词(),
                    static_cast<I64>(运行码),
                    结构体_时间戳::当前_微秒());
                return 运行结果;
            }
            return 创建本能动作运行结果实例(
                方法,
                输出结果场景,
                "运行结果未知",
                本能动作运行结果_未知);
        }
        catch (const std::exception&) {
            return 创建本能动作运行结果实例_按码(
                方法,
                输出结果场景,
                本能动作错误_动作异常);
        }
        catch (...) {
            return 创建本能动作运行结果实例_按码(
                方法,
                输出结果场景,
                本能动作错误_动作未知异常);
        }
    }

private:
    mutable std::mutex mu_{};
    std::unordered_map<枚举_本能动作ID, 结构体_本能动作登记信息> 表_{};
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
