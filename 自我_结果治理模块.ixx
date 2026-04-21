module;

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>

#include "需求类.h"
#include "任务类.h"
#include "方法类.h"
#include "实践结果消息.h"

export module 自我_结果治理模块;

import 自我模块;
import 本能动作管理模块;
import 学习执行模块;
import 任务管理任务模块;

namespace 自我_结果治理模块_detail {
    inline void 追加摘要片段(std::string& 摘要, const std::string& 片段)
    {
        if (片段.empty()) {
            return;
        }
        if (!摘要.empty()) {
            摘要 += " | ";
        }
        摘要 += 片段;
    }

    inline const char* 学习反馈消费策略文本(枚举_任务管理学习反馈消费策略 策略) noexcept
    {
        switch (策略) {
        case 枚举_任务管理学习反馈消费策略::继续学习:
            return "继续学习";
        case 枚举_任务管理学习反馈消费策略::挂起观察:
            return "挂起观察";
        case 枚举_任务管理学习反馈消费策略::使用已完善方法:
            return "使用已完善方法";
        case 枚举_任务管理学习反馈消费策略::再派生学习任务:
            return "再派生学习任务";
        default:
            return "未定义";
        }
    }
}

export {

enum class 枚举_实践结果消息类型_v0 : std::uint8_t {
    未定义 = 0,
    执行结果 = 1,
    学习结果 = 2,
};

struct 结构_需求结算结果_最小 {
    I64 安全增量 = 0;
    I64 服务增量 = 0;
    I64 方向综合得分 = 0;
    I64 普通项综合得分 = 0;
    I64 否决项综合扣分 = 0;
    I64 正向证据数 = 0;
    I64 负向证据数 = 0;
    I64 持平证据数 = 0;
    I64 零增量项数 = 0;
    I64 否决项命中数 = 0;
    I64 经验正项后果值 = 0;
    I64 经验负项后果值 = 0;
    I64 禁止项后果值 = 0;
    bool 需求已满足 = false;
    bool 有正向推进 = false;
    bool 建议转学习 = false;
    bool 命中经验正项 = false;
    bool 命中经验负项 = false;
    bool 命中禁止项 = false;
    bool 后果零增量 = false;
    I64 缺口类型 = 0;
    std::string 多特征裁决明细{};
    std::string 否决项命中面{};
    std::string 零增量摘要{};
    std::string 后果分层摘要{};
    std::string 结算摘要{};
};

struct 结构_自我特征快照_最小 {
    I64 安全值 = 0;
    I64 服务值 = 0;
    I64 物理安全值 = 0;
    I64 风险安全值 = 0;
};

struct 结构_实践结果消息_v0 {
    bool 有效 = false;
    枚举_实践结果消息类型_v0 类型 = 枚举_实践结果消息类型_v0::未定义;
    bool 成功 = false;
    任务类::节点类* 来源任务头结点 = nullptr;
    任务类::节点类* 来源宿主任务头结点 = nullptr;
    任务类::节点类* 来源当前步骤结点 = nullptr;
    方法类::节点类* 来源方法首节点 = nullptr;
    任务类::节点类* 最近结果结点 = nullptr;
    需求类::节点类* 对应需求 = nullptr;
    std::string 来源承接标签{};
    std::string 当前建议状态{};
    std::string 当前下一步去向建议{};
    std::string 摘要{};
    bool 建议进入等待 = false;
};

struct 结构_执行后果日志摘要_v0 {
    std::string 多特征裁决明细日志{};
    std::string 否决项命中面日志{};
    std::string 零增量日志{};
    std::string 经验项后果日志{};
    std::string 禁止项后果日志{};
    std::string 结果结算摘要日志{};
};

inline const char* 实践结果消息类型标签_(枚举_实践结果消息类型_v0 类型) noexcept
{
    switch (类型) {
    case 枚举_实践结果消息类型_v0::执行结果:
        return "执行结果";
    case 枚举_实践结果消息类型_v0::学习结果:
        return "学习结果";
    default:
        return "未定义";
    }
}

inline I64 计算目标差值绝对量_(I64 目标值, I64 当前值) noexcept
{
    const I64 差值 = 目标值 - 当前值;
    return 差值 < 0 ? -差值 : 差值;
}

inline void 追加结算摘要片段_(std::string& 摘要, const std::string& 片段)
{
    自我_结果治理模块_detail::追加摘要片段(摘要, 片段);
}

inline 结构_自我特征快照_最小 读取自我特征快照_最小_v0(const 自我类& 自我对象) noexcept
{
    结构_自我特征快照_最小 输出{};
    输出.安全值 = 自我对象.获取安全值();
    输出.服务值 = 自我对象.获取服务值();
    输出.物理安全值 = 自我对象.获取物理安全值();
    输出.风险安全值 = 自我对象.获取风险安全值();
    return 输出;
}

inline 结构_实践结果消息_v0 由实践结果消息转换_最小_v0(
    const 结构_实践结果消息& 消息,
    枚举_实践结果消息类型_v0 类型 = 枚举_实践结果消息类型_v0::执行结果,
    bool 成功 = true) noexcept
{
    结构_实践结果消息_v0 输出{};
    输出.有效 = 消息.来源承接类型 != 枚举_实践结果来源承接类型::未定义
        || 消息.来源宿主任务指针 != 0
        || 消息.来源当前方法首节点指针 != 0
        || !消息.摘要.empty();
    输出.类型 = 类型;
    输出.成功 = 成功;
    输出.来源任务头结点 = reinterpret_cast<任务类::节点类*>(消息.来源宿主任务指针);
    输出.来源宿主任务头结点 = reinterpret_cast<任务类::节点类*>(消息.来源宿主任务指针);
    输出.来源当前步骤结点 = reinterpret_cast<任务类::节点类*>(消息.来源当前步骤指针);
    输出.来源方法首节点 = reinterpret_cast<方法类::节点类*>(消息.来源当前方法首节点指针);
    输出.最近结果结点 = reinterpret_cast<任务类::节点类*>(消息.最近结果节点指针或镜像);
    输出.来源承接标签 = 实践结果来源承接类型文本(消息.来源承接类型);
    输出.当前建议状态 = 消息.当前步骤后状态建议;
    输出.当前下一步去向建议 = 消息.当前下一步去向建议;
    输出.建议进入等待 = 消息.当前下一步去向建议.find("等待") != std::string::npos;
    输出.摘要 = 消息.摘要.empty()
        ? 构造实践结果消息摘要(消息)
        : 消息.摘要;
    return 输出;
}

inline 结构_实践结果消息_v0 生成执行实践结果消息_最小_v0(
    const 结构_本能动作步骤结果& 执行结果,
    const 结构_本能动作执行上下文& 上下文) noexcept
{
    结构_实践结果消息_v0 输出{};
    输出.有效 = true;
    输出.类型 = 枚举_实践结果消息类型_v0::执行结果;
    输出.成功 = 执行结果.成功码 == 0;
    输出.来源任务头结点 = reinterpret_cast<任务类::节点类*>(上下文.任务节点);
    输出.来源宿主任务头结点 = reinterpret_cast<任务类::节点类*>(上下文.任务节点);
    输出.来源方法首节点 = reinterpret_cast<方法类::节点类*>(上下文.方法根);
    输出.来源承接标签 = "执行";
    输出.当前建议状态 = 输出.成功 ? "已完成" : "已失败";
    输出.当前下一步去向建议 = 输出.成功 ? "继续治理" : "挂起观察";
    输出.建议进入等待 = !输出.成功;
    输出.摘要 = 执行结果.备注.empty()
        ? (输出.成功 ? "本能动作执行完成" : "本能动作执行失败")
        : 执行结果.备注;
    return 输出;
}

inline 结构_实践结果消息_v0 生成学习实践结果消息_最小_v0(
    const 结构_学习执行结果& 执行结果) noexcept
{
    auto 输出 = 由实践结果消息转换_最小_v0(
        执行结果.实践结果消息,
        枚举_实践结果消息类型_v0::学习结果,
        执行结果.失败摘要.empty());

    输出.有效 = 输出.有效 || 执行结果.已形成学习反馈 || 执行结果.已产出实践结果消息;
    输出.来源承接标签 = "学习";
    if (输出.当前建议状态.empty()) {
        输出.当前建议状态 = 执行结果.当前状态文本;
    }
    if (输出.当前下一步去向建议.empty()) {
        输出.当前下一步去向建议 =
            自我_结果治理模块_detail::学习反馈消费策略文本(执行结果.建议消费动作);
    }
    if (输出.摘要.empty()) {
        输出.摘要 = !执行结果.摘要.empty()
            ? 执行结果.摘要
            : (执行结果.失败摘要.empty() ? "学习执行完成" : 执行结果.失败摘要);
    }
    输出.建议进入等待 = 输出.当前下一步去向建议.find("挂起") != std::string::npos
        || 输出.当前下一步去向建议.find("继续学习") != std::string::npos;
    return 输出;
}

inline std::string 构造实践结果消息摘要_最小_v0(const 结构_实践结果消息_v0& 消息)
{
    std::ostringstream 输出{};
    输出 << "类型=" << 实践结果消息类型标签_(消息.类型)
        << " | 成功=" << (消息.成功 ? "1" : "0")
        << " | 宿主=" << reinterpret_cast<std::uintptr_t>(消息.来源宿主任务头结点)
        << " | 步骤=" << reinterpret_cast<std::uintptr_t>(消息.来源当前步骤结点)
        << " | 方法=" << reinterpret_cast<std::uintptr_t>(消息.来源方法首节点);
    if (消息.最近结果结点) {
        输出 << " | 最近结果=" << reinterpret_cast<std::uintptr_t>(消息.最近结果结点);
    }
    if (!消息.当前建议状态.empty()) {
        输出 << " | 状态建议=" << 消息.当前建议状态;
    }
    if (!消息.当前下一步去向建议.empty()) {
        输出 << " | 去向建议=" << 消息.当前下一步去向建议;
    }
    if (!消息.摘要.empty()) {
        输出 << " | 摘要=" << 消息.摘要;
    }
    return 输出.str();
}

inline std::string 构造需求结算摘要_最小_v0(const 结构_需求结算结果_最小& 结果)
{
    std::string 摘要{};
    追加结算摘要片段_(摘要, "安全增量=" + std::to_string(结果.安全增量));
    追加结算摘要片段_(摘要, "服务增量=" + std::to_string(结果.服务增量));
    追加结算摘要片段_(摘要, "综合得分=" + std::to_string(结果.方向综合得分));
    if (结果.需求已满足) {
        追加结算摘要片段_(摘要, "需求已满足");
    }
    if (结果.建议转学习) {
        追加结算摘要片段_(摘要, "建议转学习");
    }
    if (!结果.结算摘要.empty()) {
        追加结算摘要片段_(摘要, 结果.结算摘要);
    }
    return 摘要;
}

inline 结构_执行后果日志摘要_v0 生成执行后果日志摘要_最小_v0(
    const std::string& 需求主键,
    const 结构_需求结算结果_最小& 结果,
    const 结构_实践结果消息_v0& 执行结果)
{
    const std::string need = 需求主键.empty() ? std::string("<null>") : 需求主键;

    结构_执行后果日志摘要_v0 输出{};
    输出.多特征裁决明细日志 =
        "[自我][结果治理] need=" + need
        + " | total=" + std::to_string(结果.方向综合得分)
        + " | normal=" + std::to_string(结果.普通项综合得分)
        + " | veto=" + std::to_string(结果.否决项综合扣分)
        + " | detail=" + 结果.多特征裁决明细;
    输出.否决项命中面日志 =
        "[自我][结果治理] need=" + need
        + " | veto_hits=" + std::to_string(结果.否决项命中数)
        + " | forbid=" + std::to_string(结果.命中禁止项 ? 1 : 0)
        + " | detail=" + (结果.否决项命中面.empty() ? std::string("<none>") : 结果.否决项命中面);
    输出.零增量日志 =
        "[自我][结果治理] need=" + need
        + " | zero_count=" + std::to_string(结果.零增量项数)
        + " | zero_effect=" + std::to_string(结果.后果零增量 ? 1 : 0)
        + " | detail=" + (结果.零增量摘要.empty() ? std::string("<none>") : 结果.零增量摘要);
    输出.经验项后果日志 =
        "[自我][结果治理] need=" + need
        + " | exp_pos=" + std::to_string(结果.经验正项后果值)
        + " | exp_neg=" + std::to_string(结果.经验负项后果值)
        + " | layered=" + 结果.后果分层摘要;
    输出.禁止项后果日志 =
        "[自我][结果治理] need=" + need
        + " | forbid=" + std::to_string(结果.禁止项后果值)
        + " | layered=" + 结果.后果分层摘要;
    输出.结果结算摘要日志 =
        "[自我][结果治理] need=" + need
        + " | success=" + std::to_string(执行结果.成功 ? 1 : 0)
        + " | result=" + 构造实践结果消息摘要_最小_v0(执行结果)
        + " | summary=" + 构造需求结算摘要_最小_v0(结果);
    return 输出;
}

} // export
