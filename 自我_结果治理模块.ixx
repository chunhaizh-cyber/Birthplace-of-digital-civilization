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
import 任务模块.治理协议;

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

    inline const char* 缺口反馈消费策略文本(枚举_任务管理缺口反馈消费策略 策略) noexcept
    {
        switch (策略) {
        case 枚举_任务管理缺口反馈消费策略::继续处理:
            return "继续处理";
        case 枚举_任务管理缺口反馈消费策略::挂起观察:
            return "挂起观察";
        case 枚举_任务管理缺口反馈消费策略::使用已完善方法:
            return "使用已完善方法";
        case 枚举_任务管理缺口反馈消费策略::再派生缺口需求:
            return "再派生缺口需求";
        default:
            return "未定义";
        }
    }
}

export {

enum class 枚举_实践结果消息类型_v0 : std::uint8_t {
    未定义 = 0,
    执行结果 = 1,
    缺口结果 = 2,
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
    case 枚举_实践结果消息类型_v0::缺口结果:
        return "缺口结果";
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



} // export
