module;

#include <cstdint>
#include <deque>
#include <sstream>
#include <string>

#include "基础数据类型.h"

export module 自我线程_消息邮箱模块;

import 自我线程模块.消息协议;

export namespace 自我线程_消息邮箱模块 {

std::uint8_t 治理消息优先级值(
    const 自我线程消息协议::枚举_治理消息优先级 优先级) noexcept
{
    return static_cast<std::uint8_t>(优先级);
}

const char* 治理消息处理类型文本(
    const 自我线程消息协议::枚举_消息处理类型 处理类型) noexcept
{
    switch (处理类型) {
    case 自我线程消息协议::枚举_消息处理类型::控制域事实消息: return "控制域事实";
    case 自我线程消息协议::枚举_消息处理类型::业务域事实消息: return "业务域事实";
    case 自我线程消息协议::枚举_消息处理类型::延迟事实消息: return "延迟事实";
    case 自我线程消息协议::枚举_消息处理类型::工作线程状态消息: return "线程状态变化";
    case 自我线程消息协议::枚举_消息处理类型::任务推进关键事件消息: return "任务推进关键事件";
    case 自我线程消息协议::枚举_消息处理类型::派生需求上报消息: return "派生需求上报";
    case 自我线程消息协议::枚举_消息处理类型::结果与关键中间状态回流消息: return "结果与关键中间状态回流";
    case 自我线程消息协议::枚举_消息处理类型::自检报告消息: return "自检报告";
    case 自我线程消息协议::枚举_消息处理类型::控制面板刷新请求消息: return "控制面板刷新请求";
    case 自我线程消息协议::枚举_消息处理类型::未定义:
    default:
        return "未定义";
    }
}

bool 消息排在前面(
    const 自我线程消息协议::结构_治理消息& 左,
    const 自我线程消息协议::结构_治理消息& 右) noexcept
{
    const auto 左优先级 = 治理消息优先级值(左.头.优先级);
    const auto 右优先级 = 治理消息优先级值(右.头.优先级);
    if (左优先级 != 右优先级) {
        return 左优先级 > 右优先级;
    }
    if (左.头.产生时间 != 右.头.产生时间) {
        return 左.头.产生时间 < 右.头.产生时间;
    }
    return 左.头.消息主键 < 右.头.消息主键;
}

std::string 双队列摘要(
    const std::deque<自我线程消息协议::结构_治理消息>& 冻结队列,
    const std::deque<自我线程消息协议::结构_治理消息>& 接收队列)
{
    std::ostringstream 输出;
    输出 << "冻结=" << 冻结队列.size()
        << " | 待消费=" << 接收队列.size();
    if (!冻结队列.empty()) {
        输出 << " | 本轮首项=" << 治理消息处理类型文本(冻结队列.front().头.处理类型);
    } else if (!接收队列.empty()) {
        输出 << " | 下轮首项=" << 治理消息处理类型文本(接收队列.front().头.处理类型);
    }
    return 输出.str();
}

} // namespace 自我线程_消息邮箱模块
