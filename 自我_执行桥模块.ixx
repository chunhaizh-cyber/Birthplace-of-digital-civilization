module;

#include <chrono>
#include <cstdint>
#include <future>
#include <string>

#include "世界树类.h"
#include "需求类.h"
#include "任务类.h"
#include "方法类.h"
#include "方法主信息类.h"

export module 自我_执行桥模块;

import 自我模块;
import 本能动作管理模块;
import 自我_结果治理模块;

export {

enum class 枚举_任务执行记账结果 : std::uint8_t {
    未定义 = 0,
    成功 = 1,
    失败 = 2,
    转学习 = 3,
};

struct 结构_方法上下文 {
    时间戳 now = 0;
    场景节点类* 输入观测包场景 = nullptr;
    场景节点类* 任务条件场景 = nullptr;
    场景节点类* 调用覆盖场景 = nullptr;
    方法类::节点类* 方法首节点 = nullptr;
    const 场景节点类* 当前场景 = nullptr;
    const 场景节点类* 条件场景 = nullptr;
};

struct 结构_方法执行结果 {
    bool 成功 = false;
    场景节点类* 结果存在集场景 = nullptr;
    场景节点类* 结果事件集场景 = nullptr;
    std::int64_t 成功码 = 0;
    std::int64_t 置信度 = 0;
    std::int64_t 质量_Q10000 = 0;
    std::int64_t 成本_Q10000 = 0;
    std::string 解释{};
};

inline 结构_方法执行结果 从本能动作结果转换_最小_v0(const 结构_本能动作步骤结果& 输入) noexcept
{
    结构_方法执行结果 输出{};
    输出.成功 = 输入.成功码 == 0;
    输出.成功码 = 输入.成功码;
    输出.置信度 = 输出.成功 ? 10000 : 0;
    输出.质量_Q10000 = 输入.质量_Q10000;
    输出.成本_Q10000 = 输入.成本_Q10000;
    输出.解释 = 输入.备注;
    return 输出;
}

class 自我_执行桥类 {
public:
    void 启动(std::uint32_t n)
    {
        工作者数量_ = n == 0 ? 1 : n;
        停止请求_ = false;
        取消请求_ = false;
    }

    void 请求停止()
    {
        停止请求_ = true;
    }

    void 等待停止()
    {
    }

    void 取消全部()
    {
        取消请求_ = true;
    }

    结构_方法执行结果 执行方法_同步等待(
        方法类::节点类* 方法首节点,
        场景节点类* 当前场景,
        场景节点类* 条件场景,
        时间戳 timeoutUs)
    {
        auto* 方法信息 = 方法首节点 ? &方法首节点->主信息 : nullptr;
        if (!方法信息 || 方法信息->节点种类 != 枚举_方法节点种类::方法首节点) {
            结构_方法执行结果 输出{};
            输出.成功码 = -1;
            输出.解释 = "方法首节点无效";
            return 输出;
        }

        if (方法信息->动作句柄.类型 != 枚举_动作句柄类型::本能函数ID) {
            结构_方法执行结果 输出{};
            输出.成功码 = -2;
            输出.解释 = "当前执行桥仅支持本能函数ID";
            return 输出;
        }

        auto 执行任务 = [=]() -> 结构_方法执行结果 {
            if (停止请求_ || 取消请求_) {
                结构_方法执行结果 输出{};
                输出.成功码 = -4;
                输出.解释 = "执行桥已停止或取消";
                return 输出;
            }

            结构_本能动作执行上下文 上下文{};
            上下文.世界树 = &世界树;
            上下文.场景 = 当前场景 ? 当前场景 : 条件场景;
            上下文.输入场景 = 条件场景 ? 条件场景 : 当前场景;
            上下文.输出场景 = 当前场景 ? 当前场景 : 条件场景;
            上下文.方法根 = 方法首节点;

            结构_本能动作步骤结果 步骤结果{};
            (void)取本能动作集().运行(
                static_cast<枚举_本能动作ID>(方法信息->动作句柄.本能ID),
                上下文,
                &步骤结果);
            return 从本能动作结果转换_最小_v0(步骤结果);
        };

        if (timeoutUs == 0) {
            return 执行任务();
        }

        auto Future = std::async(std::launch::async, 执行任务);
        const auto 状态 = Future.wait_for(std::chrono::microseconds(timeoutUs));
        if (状态 == std::future_status::ready) {
            return Future.get();
        }

        取消全部();

        结构_方法执行结果 输出{};
        输出.成功码 = -3;
        输出.解释 = "timeout";
        return 输出;
    }

private:
    std::uint32_t 工作者数量_ = 1;
    bool 停止请求_ = false;
    bool 取消请求_ = false;
};

inline 任务类::节点类* 确保根任务头_最小_v0(
    需求类::节点类* need,
    const 词性节点类* 名称,
    时间戳 now,
    const std::string& 调用点)
{
    (void)名称;
    (void)now;
    (void)调用点;
    auto* 自我对象 = 自我类::当前活动自我();
    if (!自我对象 || !need) {
        return nullptr;
    }
    if (need == 自我对象->获取安全根需求()) {
        return 自我对象->获取安全根任务();
    }
    if (need == 自我对象->获取服务根需求()) {
        return 自我对象->获取服务根任务();
    }
    return nullptr;
}

inline 任务类::节点类* 确保叶子任务头_最小_v0(
    需求类::节点类* need,
    需求类::节点类* 安全需求,
    需求类::节点类* 服务需求,
    任务类::节点类*& 安全任务,
    任务类::节点类*& 服务任务,
    const 词性节点类* 安全根任务名称,
    const 词性节点类* 服务根任务名称,
    时间戳 now,
    const std::string& 调用点)
{
    安全任务 = 确保根任务头_最小_v0(安全需求, 安全根任务名称, now, 调用点 + "/安全根");
    服务任务 = 确保根任务头_最小_v0(服务需求, 服务根任务名称, now, 调用点 + "/服务根");

    if (need == 安全需求) {
        return 安全任务;
    }
    if (need == 服务需求) {
        return 服务任务;
    }

    auto* 自我对象 = 自我类::当前活动自我();
    return 自我对象 ? 自我对象->获取当前主任务() : nullptr;
}

inline 任务类::节点类* 确保执行步骤节点_最小_v0(
    任务类::节点类* 任务头,
    方法类::节点类* 当前方法,
    时间戳 now,
    const std::string& 调用点)
{
    (void)当前方法;
    (void)now;
    (void)调用点;
    return 任务头;
}

inline 任务类::节点类* 创建执行结果节点_最小_v0(
    任务类::节点类* 任务头,
    任务类::节点类* 步骤节点,
    方法类::节点类* 当前方法,
    const 结构_方法执行结果& 执行结果,
    枚举_任务执行记账结果 记账结果,
    bool 建议转学习,
    时间戳 now,
    const std::string& 调用点)
{
    (void)任务头;
    (void)当前方法;
    (void)执行结果;
    (void)记账结果;
    (void)建议转学习;
    (void)now;
    (void)调用点;
    return 步骤节点;
}

inline void 推进任务执行前状态_最小_v0(
    任务类::节点类* 主任务,
    任务类::节点类* 当前步骤,
    方法类::节点类* 当前方法,
    时间戳 now,
    const std::string& 调用点)
{
    (void)主任务;
    (void)当前步骤;
    (void)当前方法;
    (void)now;
    (void)调用点;
}

inline void 推进任务执行后状态_最小_v0(
    任务类::节点类* 主任务,
    任务类::节点类* 当前步骤,
    方法类::节点类* 当前方法,
    const 结构_需求结算结果_最小& 结算结果,
    枚举_任务执行记账结果 记账结果,
    const 结构_方法执行结果& 执行结果,
    任务类::节点类* 最近结果节点,
    时间戳 now,
    const std::string& 调用点)
{
    (void)主任务;
    (void)当前步骤;
    (void)当前方法;
    (void)结算结果;
    (void)记账结果;
    (void)执行结果;
    (void)最近结果节点;
    (void)now;
    (void)调用点;
}

} // export
