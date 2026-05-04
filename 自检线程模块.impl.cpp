module;

#include <algorithm>
#include <atomic>
#include <chrono>
#include <exception>
#include <limits>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "日志接入.h"
#include "世界树类.h"
#include "方法类.h"
#include "本能方法类.h"
#include "任务类.h"
#include "语素类.h"

module 自检线程模块;

import 自我模块;
import 自我模块.特征定义;
import 自我线程模块;
import 自我线程模块.消息协议;
import 任务管理任务模块;

namespace {
    using 方法节点 = 方法类::节点类;
    using 任务节点 = 任务类::节点类;

    constexpr const char* 自检发出者_服务根 = "自检线程/服务根";
    constexpr const char* 自检发出者_安全根 = "自检线程/安全根";
    constexpr 时间戳 微秒每秒 = 1000000;
    std::atomic_uint64_t 自检报告序号{0};

    I64 私有_U64转I64(std::uint64_t 值) noexcept
    {
        constexpr auto 最大值 = static_cast<std::uint64_t>((std::numeric_limits<I64>::max)());
        if (值 > 最大值) {
            return (std::numeric_limits<I64>::max)();
        }
        return static_cast<I64>(值);
    }

    void 私有_绑定自检摘要特征槽(结构_自检线程摘要& 摘要) noexcept
    {
        摘要.生命周期抽象特征指针 = reinterpret_cast<std::uintptr_t>(自我特征定义类::类型_线程_生命周期状态());
        摘要.最近模式抽象特征指针 = reinterpret_cast<std::uintptr_t>(自我特征定义类::类型_线程_运行模式());
        摘要.健康状态抽象特征指针 = reinterpret_cast<std::uintptr_t>(自我特征定义类::类型_线程_健康状态());
        摘要.Tick计数抽象特征指针 = reinterpret_cast<std::uintptr_t>(自我特征定义类::类型_线程_Tick计数());
        摘要.累计提交需求数抽象特征指针 = reinterpret_cast<std::uintptr_t>(自我特征定义类::类型_自检_累计提交需求数());
        摘要.累计发现问题数抽象特征指针 = reinterpret_cast<std::uintptr_t>(自我特征定义类::类型_自检_累计发现问题数());
        摘要.最近检查时间抽象特征指针 = reinterpret_cast<std::uintptr_t>(自我特征定义类::类型_自检_最近检查时间());
        摘要.最近提交时间抽象特征指针 = reinterpret_cast<std::uintptr_t>(自我特征定义类::类型_自检_最近提交时间());
    }

    const char* 私有_布尔文本(bool 值) noexcept
    {
        return 值 ? "是" : "否";
    }

    const 词性节点类* 私有_特征词(const std::string& 名称) noexcept
    {
        return 语素集.添加词性词(名称, "名词");
    }

    template<class T节点>
    bool 私有_节点属于独立树(const T节点* 根节点, const T节点* 节点) noexcept
    {
        if (!根节点 || !节点) {
            return false;
        }
        if (根节点 == 节点) {
            return true;
        }
        for (auto* 当前 = reinterpret_cast<const T节点*>(根节点->链下);
             当前 && 当前 != 根节点;
             当前 = reinterpret_cast<const T节点*>(当前->链下)) {
            if (当前 == 节点) {
                return true;
            }
        }
        return false;
    }

    std::vector<方法节点*> 私有_枚举方法首节点(std::size_t 上限, std::size_t* 游标) noexcept
    {
        std::vector<方法节点*> 全量{};
        auto* 自我存在 = 自我.获取自我存在();
        auto* 方法根节点 = 自我存在
            ? reinterpret_cast<方法节点*>(世界树.存在().获取方法根节点(自我存在))
            : nullptr;
        if (!方法根节点) {
            return 全量;
        }

        for (auto* 当前 = reinterpret_cast<方法节点*>(方法根节点->链下);
             当前 && 当前 != 方法根节点;
             当前 = reinterpret_cast<方法节点*>(当前->链下)) {
            if (当前->主信息.节点种类 == 枚举_方法节点种类::方法首节点) {
                全量.push_back(当前);
            }
        }
        if (全量.empty() || 上限 == 0 || 全量.size() <= 上限) {
            if (游标) {
                *游标 = 0;
            }
            return 全量;
        }

        std::vector<方法节点*> 输出{};
        输出.reserve(上限);
        std::size_t 起点 = 游标 ? (*游标 % 全量.size()) : 0;
        for (std::size_t i = 0; i < 上限; ++i) {
            输出.push_back(全量[(起点 + i) % 全量.size()]);
        }
        if (游标) {
            *游标 = (起点 + 上限) % 全量.size();
        }
        return 输出;
    }

    struct 结构_方法结构计数 {
        I64 条件数 = 0;
        I64 结果数 = 0;
    };

    结构_方法结构计数 私有_统计方法直接结构(const 方法节点* 方法头) noexcept
    {
        结构_方法结构计数 计数{};
        if (!方法头 || !方法头->子) {
            return 计数;
        }

        auto* 首节点 = reinterpret_cast<const 方法节点*>(方法头->子);
        auto* 当前 = 首节点;
        std::size_t 保护 = 0;
        do {
            if (当前->主信息.节点种类 == 枚举_方法节点种类::方法条件节点) {
                ++计数.条件数;
            } else if (当前->主信息.节点种类 == 枚举_方法节点种类::方法结果节点) {
                ++计数.结果数;
            }
            当前 = 当前 ? reinterpret_cast<const 方法节点*>(当前->下) : nullptr;
            ++保护;
        } while (当前 && 当前 != 首节点 && 保护 < 2048);
        return 计数;
    }

    std::string 私有_方法标题(const 方法节点* 方法) noexcept
    {
        if (!方法) {
            return "空方法";
        }
        if (方法->主信息.动作名) {
            return 方法->主信息.动作名->获取主键();
        }
        if (方法->主信息.主结果特征类型) {
            return 方法->主信息.主结果特征类型->获取主键();
        }
        return 方法->获取主键();
    }

    I64 私有_统计结构可用方法数量() noexcept
    {
        I64 数量 = 0;
        for (auto* 方法 : 私有_枚举方法首节点(4096, nullptr)) {
            if (!方法) {
                continue;
            }
            const auto 计数 = 私有_统计方法直接结构(方法);
            if (方法->主信息.有动作骨架()
                && 方法->主信息.方法虚拟存在.有效()
                && 计数.条件数 > 0
                && 计数.结果数 > 0) {
                ++数量;
            }
        }
        return 数量;
    }

    bool 私有_方法引用命中(
        const 可解析引用<方法节点类>& 引用,
        const 方法节点* 方法) noexcept
    {
        if (!方法) {
            return false;
        }
        if (引用.指针 && reinterpret_cast<const 方法节点*>(引用.指针) == 方法) {
            return true;
        }
        return !引用.主键.empty() && 引用.主键 == 方法->获取主键();
    }

    bool 私有_任务仍在处理缺口(const 任务节点* 任务) noexcept
    {
        if (!任务) {
            return false;
        }
        if (任务->主信息.当前阶段 == 枚举_任务阶段::已完成) {
            return false;
        }
        switch (任务->主信息.状态) {
        case 枚举_任务状态::完成:
        case 枚举_任务状态::失败:
        case 枚举_任务状态::取消:
        case 枚举_任务状态::超时:
            return false;
        default:
            return true;
        }
    }

    bool 私有_任务命中方法结构学习积压(
        const 任务节点* 任务,
        const 方法节点* 方法) noexcept
    {
        if (!任务 || !方法 || !私有_任务仍在处理缺口(任务)) {
            return false;
        }

        const bool 指向目标方法 =
            私有_方法引用命中(任务->主信息.等待学习方法首节点, 方法)
            || 私有_方法引用命中(任务->主信息.来源方法首节点, 方法)
            || 私有_方法引用命中(任务->主信息.候选方法首节点, 方法)
            || 私有_方法引用命中(任务->主信息.可用方法首节点, 方法)
            || 私有_方法引用命中(任务->主信息.执行方法, 方法)
            || 任务->主信息.来源方法主键 == 方法->获取主键();
        if (!指向目标方法) {
            return false;
        }

        const bool 尝试学习任务 =
            任务->主信息.选中自我能力值 == static_cast<I64>(枚举_本能方法ID::自我_尝试学习)
            || 任务->主信息.任务角色 == 枚举_任务角色::学习任务
            || 任务->主信息.等待学习唤醒;
        const bool 结构缺口任务 =
            任务->主信息.当前阶段 == 枚举_任务阶段::筹办
            || 任务->主信息.当前阶段 == 枚举_任务阶段::执行
            || 任务->主信息.当前阶段 == 枚举_任务阶段::阻塞
            || 任务->主信息.状态 == 枚举_任务状态::筹办中
            || 任务->主信息.状态 == 枚举_任务状态::执行中
            || 任务->主信息.状态 == 枚举_任务状态::等待中;
        return 尝试学习任务 || 结构缺口任务;
    }

    bool 私有_存在方法结构学习积压(const 方法节点* 方法) noexcept
    {
        if (!方法) {
            return false;
        }
        auto* 自我存在 = 自我.获取自我存在();
        auto* 任务根节点 = 自我存在
            ? reinterpret_cast<任务节点*>(世界树.存在().获取任务根节点(自我存在))
            : nullptr;
        if (!任务根节点) {
            return false;
        }
        std::size_t 保护 = 0;
        for (auto* 当前 = reinterpret_cast<任务节点*>(任务根节点->链下);
             当前 && 当前 != 任务根节点 && 保护 < 4096;
             当前 = reinterpret_cast<任务节点*>(当前->链下), ++保护) {
            if (私有_任务命中方法结构学习积压(当前, 方法)) {
                return true;
            }
        }
        return false;
    }

    bool 私有_是否去重抑制中(
        std::unordered_map<std::string, 时间戳>& 去重账,
        const std::string& 去重键,
        时间戳 now,
        时间戳 抑制微秒) noexcept
    {
        if (去重键.empty()) {
            return false;
        }
        const auto it = 去重账.find(去重键);
        if (it != 去重账.end()
            && it->second != 0
            && now >= it->second
            && now - it->second < 抑制微秒) {
            return true;
        }
        去重账[去重键] = now;
        return false;
    }

    std::uint64_t 私有_折叠主键(const std::string& 主键) noexcept
    {
        std::uint64_t 值 = 1469598103934665603ull;
        for (const unsigned char 字节 : 主键) {
            值 ^= static_cast<std::uint64_t>(字节);
            值 *= 1099511628211ull;
        }
        return 值;
    }

    结构_任务管理派生需求消息 私有_构造自检派生需求(
        const std::string& 去重键,
        const char* 发出者,
        const 方法节点* 方法,
        const std::string& 特征名称,
        const std::string& 当前状态,
        const std::string& 目标状态,
        const std::string& 缺口类型,
        const std::string& 观察摘要,
        枚举_任务管理派生需求类型 需求类型,
        枚举_任务管理阻塞级别 阻塞级别,
        bool 影响服务,
        bool 影响安全,
        bool 影响父任务,
        时间戳 now) noexcept
    {
        结构_任务管理派生需求消息 派生{};
        派生.消息头.发出者主键 = 发出者 ? 发出者 : 自检发出者_服务根;
        派生.消息头.产生时间 = now;
        派生.消息头.幂等键 = 去重键;
        派生.消息头.允许正式资产提交 = false;
        派生.当前方法主键 = 方法 ? 方法->获取主键() : std::string{};
        派生.方法虚拟存在指针 = 方法 && 方法->主信息.方法虚拟存在.指针
            ? reinterpret_cast<std::uintptr_t>(方法->主信息.方法虚拟存在.指针)
            : 0;
        派生.方法虚拟存在主键 = 方法 ? 方法->主信息.方法虚拟存在.主键 : std::string{};
        派生.方法需求位 = 特征名称;
        派生.本能能力缺口类型 = 缺口类型;
        派生.主观察特征 = 观察摘要;
        派生.任务挂接方式 = 枚举_需求任务挂接方式::增补为新步骤;
        派生.派生需求类型 = 需求类型;
        派生.阻塞级别 = 阻塞级别;
        派生.是否本地不可消化 = true;
        派生.是否阻塞当前步骤 = false;
        派生.是否影响父任务推进 = 影响父任务;
        派生.是否可能影响服务 = 影响服务;
        派生.是否可能影响安全 = 影响安全;
        派生.是否建议线程重判 = true;
        派生.触发动作主体主键 = "自检线程";
        派生.触发动作主体类型文本 = "独立高优先级自检线程";
        派生.触发动作语义键 = "自检发现缺口";
        if (const auto* 特征 = 私有_特征词(特征名称)) {
            派生.需求特征类型指针 = reinterpret_cast<std::uintptr_t>(特征);
            派生.需求特征类型主键 = 特征->获取主键();
        }
        派生.当前等待原因 = 当前状态 + " -> " + 目标状态;
        派生.缺口项集.push_back(结构_任务管理未满足项{
            缺口类型,
            方法 ? 方法->获取主键() : std::string("自我"),
            影响父任务,
            true
        });
        同步任务管理派生需求消息语义槽(派生);
        return 派生;
    }

    bool 私有_提交自检报告(
        const 结构_任务管理派生需求消息& 派生) noexcept
    {
        自我线程消息协议::结构_自检报告消息 报告{};
        报告.报告ID = 自检报告序号.fetch_add(1, std::memory_order_relaxed) + 1;
        报告.生成时间 = 派生.消息头.产生时间 != 0
            ? 派生.消息头.产生时间
            : 结构体_时间戳::当前_微秒();

        自我线程消息协议::结构_自检缺口候选消息 候选{};
        候选.缺口类型抽象特征指针 = 派生.需求特征类型指针;
        候选.缺口类型值 = static_cast<I64>(派生.派生需求类型);
        候选.目标主键 = 私有_折叠主键(
            !派生.当前方法主键.empty()
                ? 派生.当前方法主键
                : 派生.方法虚拟存在主键);
        候选.目标指针 = 派生.方法虚拟存在指针;
        候选.证据特征指针 = 派生.需求特征类型指针;
        候选.证据值 = static_cast<I64>(派生.阻塞级别);
        候选.建议需求类型抽象特征指针 = 派生.需求特征类型指针;
        候选.建议需求类型值 = static_cast<I64>(派生.派生需求类型);
        候选.建议生成需求 = true;
        报告.缺口候选集.push_back(候选);

        return 投递自检报告消息(报告);
    }

    bool 私有_到期(
        std::unordered_map<std::string, std::chrono::steady_clock::time_point>& 最近检查时刻,
        const std::string& 键,
        std::chrono::milliseconds 间隔) noexcept
    {
        const auto 现在 = std::chrono::steady_clock::now();
        const auto it = 最近检查时刻.find(键);
        if (it != 最近检查时刻.end() && 现在 - it->second < 间隔) {
            return false;
        }
        最近检查时刻[键] = 现在;
        return true;
    }
}

const char* 自检线程生命周期文本(枚举_自检线程生命周期状态 状态) noexcept
{
    switch (状态) {
    case 枚举_自检线程生命周期状态::未启动: return "未启动";
    case 枚举_自检线程生命周期状态::启动中: return "启动中";
    case 枚举_自检线程生命周期状态::运行中: return "运行中";
    case 枚举_自检线程生命周期状态::停止请求中: return "停止请求中";
    case 枚举_自检线程生命周期状态::已停止: return "已停止";
    case 枚举_自检线程生命周期状态::故障: return "故障";
    default: return "未定义";
    }
}

const char* 自检线程模式文本(枚举_自检线程模式 模式) noexcept
{
    return 模式 == 枚举_自检线程模式::休眠期 ? "休眠期" : "运行中";
}

自检线程类::自检线程类() = default;

自检线程类::~自检线程类()
{
    请求停止();
    等待停止();
}

bool 自检线程类::启动()
{
    {
        std::lock_guard<std::mutex> 锁(状态锁_);
        if (生命周期_ == 枚举_自检线程生命周期状态::运行中
            || 生命周期_ == 枚举_自检线程生命周期状态::启动中) {
            return true;
        }
        生命周期_ = 枚举_自检线程生命周期状态::启动中;
        停止请求_.store(false);
        健康运行_ = false;
        记录事件_已加锁(结构体_时间戳::当前_微秒(), "自检线程启动请求");
    }

    try {
        工作线程_ = std::thread(&自检线程类::主循环_, this);
        return true;
    } catch (const std::exception& 异常) {
        std::lock_guard<std::mutex> 锁(状态锁_);
        生命周期_ = 枚举_自检线程生命周期状态::故障;
        健康运行_ = false;
        记录事件_已加锁(结构体_时间戳::当前_微秒(), std::string("自检线程启动失败 | ") + 异常.what());
        项目记录异常日志(异常, "自检线程启动");
        return false;
    }
}

void 自检线程类::请求停止()
{
    {
        std::lock_guard<std::mutex> 锁(状态锁_);
        if (生命周期_ == 枚举_自检线程生命周期状态::未启动
            || 生命周期_ == 枚举_自检线程生命周期状态::已停止) {
            return;
        }
        生命周期_ = 枚举_自检线程生命周期状态::停止请求中;
        记录事件_已加锁(结构体_时间戳::当前_微秒(), "自检线程停止请求");
    }
    停止请求_.store(true);
    唤醒条件_.notify_all();
}

void 自检线程类::等待停止()
{
    if (工作线程_.joinable()) {
        工作线程_.join();
    }
}

void 自检线程类::请求休眠期自检()
{
    休眠期自检请求_.store(true);
    {
        std::lock_guard<std::mutex> 锁(状态锁_);
        记录事件_已加锁(结构体_时间戳::当前_微秒(), "休眠期自检请求");
    }
    唤醒条件_.notify_all();
}

bool 自检线程类::是否健康运行() const noexcept
{
    std::lock_guard<std::mutex> 锁(状态锁_);
    return 健康运行_;
}

枚举_自检线程生命周期状态 自检线程类::读取生命周期状态() const noexcept
{
    std::lock_guard<std::mutex> 锁(状态锁_);
    return 生命周期_;
}

结构_自检线程摘要 自检线程类::读取摘要() const
{
    std::lock_guard<std::mutex> 锁(状态锁_);
    结构_自检线程摘要 摘要{};
    私有_绑定自检摘要特征槽(摘要);
    摘要.生命周期 = 生命周期_;
    摘要.生命周期值 = static_cast<I64>(生命周期_);
    摘要.最近模式 = 最近模式_;
    摘要.最近模式值 = static_cast<I64>(最近模式_);
    摘要.健康运行 = 健康运行_;
    摘要.健康状态值 = 健康运行_ ? 1 : 0;
    摘要.Tick计数 = Tick计数_;
    摘要.Tick计数值 = 私有_U64转I64(Tick计数_);
    摘要.累计提交需求数 = 累计提交需求数_;
    摘要.累计提交需求数值 = 私有_U64转I64(累计提交需求数_);
    摘要.累计发现问题数 = 累计发现问题数_;
    摘要.累计发现问题数值 = 私有_U64转I64(累计发现问题数_);
    摘要.最近检查时间 = 最近检查时间_;
    摘要.最近检查时间值 = 最近检查时间_;
    摘要.最近提交时间 = 最近提交时间_;
    摘要.最近提交时间值 = 最近提交时间_;
    摘要.最近需求摘要 = 最近需求摘要_;
    摘要.最近事件摘要 = 最近事件_.empty() ? std::string{} : 最近事件_.back();
    摘要.最近事件列表.assign(最近事件_.begin(), 最近事件_.end());
    return 摘要;
}

std::vector<std::string> 自检线程类::读取最近事件列表(std::size_t 上限) const
{
    std::lock_guard<std::mutex> 锁(状态锁_);
    std::vector<std::string> 输出{};
    const auto 数量 = (std::min)(上限, 最近事件_.size());
    输出.reserve(数量);
    auto it = 最近事件_.end();
    for (std::size_t i = 0; i < 数量; ++i) {
        --it;
        输出.push_back(*it);
    }
    std::reverse(输出.begin(), 输出.end());
    return 输出;
}

void 自检线程类::记录事件_已加锁(时间戳 now, const std::string& 摘要)
{
    std::ostringstream 输出;
    输出 << now << " | " << 摘要;
    最近事件_.push_back(输出.str());
    while (最近事件_.size() > 配置_.事件保留上限) {
        最近事件_.pop_front();
    }
}

void 自检线程类::主循环_()
{
    {
        std::lock_guard<std::mutex> 锁(状态锁_);
        生命周期_ = 枚举_自检线程生命周期状态::运行中;
        健康运行_ = true;
        记录事件_已加锁(结构体_时间戳::当前_微秒(), "自检线程已启动");
    }

    while (!停止请求_.load()) {
        const auto now = 结构体_时间戳::当前_微秒();
        try {
            {
                std::lock_guard<std::mutex> 锁(状态锁_);
                ++Tick计数_;
                最近检查时间_ = now;
            }

            执行运行中自检_(now);
            if (休眠期自检请求_.exchange(false)) {
                执行休眠期自检_(now);
            }
        } catch (const std::exception& 异常) {
            项目记录异常日志(异常, "自检线程/主循环");
            std::lock_guard<std::mutex> 锁(状态锁_);
            生命周期_ = 枚举_自检线程生命周期状态::故障;
            健康运行_ = false;
            记录事件_已加锁(now, std::string("自检线程故障 | ") + 异常.what());
        } catch (...) {
            项目致命日志("自检线程/主循环 | 未知异常");
            std::lock_guard<std::mutex> 锁(状态锁_);
            生命周期_ = 枚举_自检线程生命周期状态::故障;
            健康运行_ = false;
            记录事件_已加锁(now, "自检线程故障 | 未知异常");
        }

        std::unique_lock<std::mutex> 锁(状态锁_);
        唤醒条件_.wait_for(锁, 配置_.Tick间隔, [&] {
            return 停止请求_.load() || 休眠期自检请求_.load();
        });
    }

    {
        std::lock_guard<std::mutex> 锁(状态锁_);
        生命周期_ = 枚举_自检线程生命周期状态::已停止;
        健康运行_ = false;
        记录事件_已加锁(结构体_时间戳::当前_微秒(), "自检线程已停止");
    }
}

void 自检线程类::执行运行中自检_(时间戳 now)
{
    std::size_t 本轮提交 = 0;
    const auto 抑制微秒 =
        static_cast<时间戳>(std::chrono::duration_cast<std::chrono::microseconds>(配置_.去重抑制间隔).count());

    if (私有_到期(最近检查时刻_, "方法结构", 配置_.方法结构检查间隔)) {
        for (auto* 方法 : 私有_枚举方法首节点(配置_.单类扫描上限, &方法扫描游标_)) {
            if (!方法 || 本轮提交 >= 配置_.每轮提交上限) {
                break;
            }
            if (私有_存在方法结构学习积压(方法)) {
                continue;
            }
            const auto 计数 = 私有_统计方法直接结构(方法);
            const bool 结构不完整 =
                !方法->主信息.有动作骨架()
                || 计数.条件数 <= 0
                || 计数.结果数 <= 0;
            if (!结构不完整) {
                continue;
            }

            const std::string 去重键 = "自检|方法结构|"
                + 方法->获取主键()
                + "|动作=" + (方法->主信息.有动作骨架() ? "1" : "0")
                + "|条件=" + std::to_string(计数.条件数)
                + "|结果=" + std::to_string(计数.结果数);
            if (私有_是否去重抑制中(去重账_, 去重键, now, 抑制微秒)) {
                continue;
            }

            std::ostringstream 观察;
            观察 << "方法结构不完整 | 方法=" << 私有_方法标题(方法)
                << " | 动作=" << 私有_布尔文本(方法->主信息.有动作骨架())
                << " | 条件数=" << 计数.条件数
                << " | 结果数=" << 计数.结果数;
            const auto 派生 = 私有_构造自检派生需求(
                去重键,
                自检发出者_服务根,
                方法,
                "方法结构是否完整",
                "0",
                "1",
                "学习_补齐方法结构",
                观察.str(),
                枚举_任务管理派生需求类型::缺方法,
                枚举_任务管理阻塞级别::中等,
                true,
                false,
                true,
                now);
            if (私有_提交自检报告(派生)) {
                ++本轮提交;
                std::lock_guard<std::mutex> 锁(状态锁_);
                最近模式_ = 枚举_自检线程模式::运行中;
                ++累计提交需求数_;
                ++累计发现问题数_;
                最近提交时间_ = now;
                最近需求摘要_ = 构造任务管理派生需求消息文本(派生);
                记录事件_已加锁(now, "发现并提交需求 | " + 最近需求摘要_);
            }
        }
    }

    if (本轮提交 < 配置_.每轮提交上限
        && 私有_到期(最近检查时刻_, "方法稳定性", 配置_.方法稳定性检查间隔)) {
        for (auto* 方法 : 私有_枚举方法首节点(配置_.单类扫描上限, &方法扫描游标_)) {
            if (!方法 || 本轮提交 >= 配置_.每轮提交上限) {
                break;
            }
            const bool 有运行证据 = 方法->主信息.统计.命中次数 >= 3;
            const bool 稳定性不足 = 有运行证据 && 方法->主信息.统计.可信度 <= 0;
            if (!稳定性不足) {
                continue;
            }
            const std::string 去重键 = "自检|方法稳定性|" + 方法->获取主键();
            if (私有_是否去重抑制中(去重账_, 去重键, now, 抑制微秒)) {
                continue;
            }
            std::ostringstream 观察;
            观察 << "方法稳定性不足 | 方法=" << 私有_方法标题(方法)
                << " | 命中次数=" << 方法->主信息.统计.命中次数
                << " | 可信度=" << 方法->主信息.统计.可信度;
            const auto 派生 = 私有_构造自检派生需求(
                去重键,
                自检发出者_服务根,
                方法,
                "方法稳定状态",
                "不稳定",
                "稳定",
                "学习_验证方法稳定性",
                观察.str(),
                枚举_任务管理派生需求类型::风险处置,
                枚举_任务管理阻塞级别::中等,
                true,
                false,
                false,
                now);
            if (私有_提交自检报告(派生)) {
                ++本轮提交;
                std::lock_guard<std::mutex> 锁(状态锁_);
                最近模式_ = 枚举_自检线程模式::运行中;
                ++累计提交需求数_;
                ++累计发现问题数_;
                最近提交时间_ = now;
                最近需求摘要_ = 构造任务管理派生需求消息文本(派生);
                记录事件_已加锁(now, "发现并提交需求 | " + 最近需求摘要_);
            }
        }
    }

    if (本轮提交 < 配置_.每轮提交上限
        && 私有_到期(最近检查时刻_, "方法运行账", 配置_.方法稳定性检查间隔)) {
        for (auto* 方法 : 私有_枚举方法首节点(配置_.单类扫描上限, &方法扫描游标_)) {
            if (!方法 || 本轮提交 >= 配置_.每轮提交上限) {
                break;
            }
            const bool 运行账不完整 =
                方法->主信息.统计.命中次数 > 0
                && 方法->主信息.最近执行时间 == 0
                && 方法->主信息.最近完成时间 == 0;
            if (!运行账不完整) {
                continue;
            }
            const std::string 去重键 = "自检|方法运行账|" + 方法->获取主键();
            if (私有_是否去重抑制中(去重账_, 去重键, now, 抑制微秒)) {
                continue;
            }
            std::ostringstream 观察;
            观察 << "方法运行账不完整 | 方法=" << 私有_方法标题(方法)
                << " | 命中次数=" << 方法->主信息.统计.命中次数
                << " | 最近执行时间=" << 方法->主信息.最近执行时间
                << " | 最近完成时间=" << 方法->主信息.最近完成时间;
            const auto 派生 = 私有_构造自检派生需求(
                去重键,
                自检发出者_服务根,
                方法,
                "方法运行账完整性",
                "不完整",
                "完整",
                "管理_检查方法运行账完整性",
                观察.str(),
                枚举_任务管理派生需求类型::缺信息,
                枚举_任务管理阻塞级别::轻微,
                true,
                false,
                false,
                now);
            if (私有_提交自检报告(派生)) {
                ++本轮提交;
                std::lock_guard<std::mutex> 锁(状态锁_);
                最近模式_ = 枚举_自检线程模式::运行中;
                ++累计提交需求数_;
                ++累计发现问题数_;
                最近提交时间_ = now;
                最近需求摘要_ = 构造任务管理派生需求消息文本(派生);
                记录事件_已加锁(now, "发现并提交需求 | " + 最近需求摘要_);
            }
        }
    }

    if (本轮提交 == 0) {
        std::lock_guard<std::mutex> 锁(状态锁_);
        最近模式_ = 枚举_自检线程模式::运行中;
        记录事件_已加锁(now, "运行中自检完成 | 未发现需提交缺口");
    }
}

void 自检线程类::执行休眠期自检_(时间戳 now)
{
    ++休眠批次号_;
    std::size_t 本轮提交 = 0;
    const auto 抑制微秒 =
        static_cast<时间戳>(std::chrono::duration_cast<std::chrono::microseconds>(配置_.去重抑制间隔).count());

    auto* 自我存在 = 自我.获取自我存在();
    auto* 方法根节点 = 自我存在
        ? reinterpret_cast<方法节点*>(世界树.存在().获取方法根节点(自我存在))
        : nullptr;
    auto* 任务根节点 = 自我存在
        ? reinterpret_cast<任务节点*>(世界树.存在().获取任务根节点(自我存在))
        : nullptr;

    for (auto* 方法 : 私有_枚举方法首节点(配置_.单类扫描上限, &休眠方法扫描游标_)) {
        if (!方法 || 本轮提交 >= 配置_.每轮提交上限) {
            break;
        }
        if (方法->主信息.方法虚拟存在.有效()) {
            continue;
        }
        const std::string 去重键 = "自检|休眠|方法虚拟存在|" + 方法->获取主键();
        if (私有_是否去重抑制中(去重账_, 去重键, now, 抑制微秒)) {
            continue;
        }
        std::ostringstream 观察;
        观察 << "方法虚拟存在完整性缺口 | 方法=" << 私有_方法标题(方法);
        const auto 派生 = 私有_构造自检派生需求(
            去重键,
            自检发出者_服务根,
            方法,
            "方法虚拟存在完整性",
            "不完整",
            "完整",
            "管理_确保单承载存在",
            观察.str(),
            枚举_任务管理派生需求类型::缺资源,
            枚举_任务管理阻塞级别::轻微,
            true,
            false,
            false,
            now);
        if (私有_提交自检报告(派生)) {
            ++本轮提交;
            std::lock_guard<std::mutex> 锁(状态锁_);
            最近模式_ = 枚举_自检线程模式::休眠期;
            ++累计提交需求数_;
            ++累计发现问题数_;
            最近提交时间_ = now;
            最近需求摘要_ = 构造任务管理派生需求消息文本(派生);
            记录事件_已加锁(now, "休眠期发现并提交需求 | " + 最近需求摘要_);
        }
    }

    if (本轮提交 < 配置_.每轮提交上限 && 自我存在) {
        I64 当前可用方法数量 = 0;
        const I64 统计可用方法数量 = 私有_统计结构可用方法数量();
        if (自我.读取自我I64特征当前值(
                自我特征定义类::类型_自我_可用方法数量(),
                当前可用方法数量)
            && 当前可用方法数量 != 统计可用方法数量) {
            const std::string 去重键 = "自检|休眠|可用方法数量镜像|"
                + std::to_string(当前可用方法数量)
                + "->"
                + std::to_string(统计可用方法数量);
            if (!私有_是否去重抑制中(去重账_, 去重键, now, 抑制微秒)) {
                std::ostringstream 观察;
                观察 << "可用方法数量镜像不一致 | 当前=" << 当前可用方法数量
                    << " | 统计=" << 统计可用方法数量;
                const auto 派生 = 私有_构造自检派生需求(
                    去重键,
                    自检发出者_服务根,
                    nullptr,
                    "可用方法数量",
                    std::to_string(当前可用方法数量),
                    std::to_string(统计可用方法数量),
                    "自我_同步可用方法数量镜像",
                    观察.str(),
                    枚举_任务管理派生需求类型::补偿动作,
                    枚举_任务管理阻塞级别::轻微,
                    true,
                    false,
                    false,
                    now);
                if (私有_提交自检报告(派生)) {
                    ++本轮提交;
                    std::lock_guard<std::mutex> 锁(状态锁_);
                    最近模式_ = 枚举_自检线程模式::休眠期;
                    ++累计提交需求数_;
                    ++累计发现问题数_;
                    最近提交时间_ = now;
                    最近需求摘要_ = 构造任务管理派生需求消息文本(派生);
                    记录事件_已加锁(now, "休眠期发现并提交需求 | " + 最近需求摘要_);
                }
            }
        }
    }

    if (本轮提交 == 0) {
        std::lock_guard<std::mutex> 锁(状态锁_);
        最近模式_ = 枚举_自检线程模式::休眠期;
        记录事件_已加锁(
            now,
            "休眠期自检完成 | 批次=" + std::to_string(休眠批次号_) + " | 未发现需提交缺口");
    }
}

自检线程类& 获取全局自检线程() noexcept
{
    static 自检线程类 实例{};
    return 实例;
}

bool 启动自检线程()
{
    return 获取全局自检线程().启动();
}

void 停止自检线程()
{
    auto& 线程 = 获取全局自检线程();
    线程.请求停止();
    线程.等待停止();
}

void 请求休眠期自检()
{
    获取全局自检线程().请求休眠期自检();
}

结构_自检线程摘要 读取自检线程摘要()
{
    return 获取全局自检线程().读取摘要();
}
