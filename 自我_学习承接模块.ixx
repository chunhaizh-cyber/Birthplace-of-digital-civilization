module;

#include <algorithm>
#include <string>

#include "需求类.h"
#include "任务类.h"
#include "方法类.h"

export module 自我_学习承接模块;

import 自我模块;
import 自我模块.特征定义;
import 自我线程模块;
import 任务管理任务模块;

namespace 自我_学习承接模块_detail {
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

struct 结构_已消费学习回流_最小_v0 {
    bool 有效 = false;
    bool 已形成学习反馈 = false;
    bool 已完善方法 = false;
    bool 建议继续学习 = false;
    bool 建议挂起观察 = false;
    bool 建议使用已完善方法 = false;
    bool 建议再派生学习任务 = false;
    需求类::节点类* 来源需求 = nullptr;
    任务类::节点类* 来源任务头结点 = nullptr;
    任务类::节点类* 来源宿主任务头结点 = nullptr;
    任务类::节点类* 来源当前步骤结点 = nullptr;
    方法类::节点类* 来源当前方法首节点 = nullptr;
    任务类::节点类* 最近结果结点 = nullptr;
    std::uint64_t 账本ID = 0;
    std::string 建议状态{};
    std::string 建议去向{};
    std::string 摘要{};
};

struct 结构_学习承接快照_最小_v0 {
    I64 待学习方法数量 = 0;
    bool 正在尝试学习 = false;
    bool 应触发学习 = false;
    bool 应申请重试恢复 = false;
    bool 应申请收束恢复 = false;
    std::size_t 恢复请求数 = 0;
    枚举_自我线程最终去向 最近最终去向 = 枚举_自我线程最终去向::未定义;
    std::string 最近学习反馈摘要{};
    std::string 最近恢复摘要{};
};

inline 结构_学习承接快照_最小_v0 读取学习承接快照_最小_v0(
    const 自我类& 自我对象,
    const 自我线程类* 自我线程对象 = nullptr) noexcept
{
    结构_学习承接快照_最小_v0 输出{};
    (void)自我对象.读取自我I64特征当前值(
        自我特征定义类::类型_自我_待学习方法数量(),
        输出.待学习方法数量);

    I64 尝试学习状态值 = 0;
    (void)自我对象.读取自我I64特征当前值(
        自我特征定义类::类型_自我_尝试学习状态(),
        尝试学习状态值);
    输出.正在尝试学习 = 尝试学习状态值 != 0;

    if (!自我线程对象) {
        return 输出;
    }

    输出.最近最终去向 = 自我线程对象->读取最近最终去向();
    输出.最近恢复摘要 = 自我线程对象->读取最近恢复摘要();

    const auto 恢复快照 = 自我线程对象->读取最近学习恢复接口快照();
    输出.应触发学习 = 恢复快照.应触发学习;
    输出.应申请重试恢复 = 恢复快照.应申请重试恢复;
    输出.应申请收束恢复 = 恢复快照.应申请收束恢复;
    输出.恢复请求数 = 恢复快照.恢复请求列表.size();
    输出.最近学习反馈摘要 = !恢复快照.摘要.empty()
        ? 恢复快照.摘要
        : 恢复快照.恢复请求摘要;
    return 输出;
}

inline bool 同步学习承接镜像_最小_v0(
    自我类& 自我对象,
    const 结构_学习承接快照_最小_v0& 快照,
    时间戳 now = 0,
    const std::string& 调用点 = "同步学习承接镜像_最小_v0") noexcept
{
    if (now == 0) {
        now = 结构体_时间戳::当前_微秒();
    }

    bool 成功 = true;
    成功 = 自我对象.写入自我I64特征当前值(
        自我特征定义类::类型_自我_待学习方法数量(),
        快照.待学习方法数量,
        now,
        调用点 + "/待学习方法数量")
        && 成功;
    成功 = 自我对象.写入自我I64特征当前值(
        自我特征定义类::类型_自我_尝试学习状态(),
        快照.正在尝试学习 ? 1 : 0,
        now,
        调用点 + "/尝试学习状态")
        && 成功;
    return 成功;
}

} // export
