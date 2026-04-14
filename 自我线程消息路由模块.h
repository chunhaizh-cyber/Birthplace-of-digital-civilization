#pragma once

#include "自我线程消息协议模块.h"

namespace 自我线程消息路由 {

inline 自我线程消息协议::枚举_首收件人 判定首收件人(const 自我线程消息协议::结构_治理消息& 消息) noexcept
{
    using namespace 自我线程消息协议;
    if (消息.头.仅局部有效 && 消息.头.目标任务主键 != 0) {
        return 枚举_首收件人::任务管理工作线程;
    }

    switch (消息.头.大类) {
    case 枚举_消息大类::控制消息:
    case 枚举_消息大类::全局环境消息:
    case 枚举_消息大类::工作线程状态消息:
    case 枚举_消息大类::任务推进关键事件消息:
    case 枚举_消息大类::派生需求消息:
    case 枚举_消息大类::结果回流消息:
    case 枚举_消息大类::学习与审计消息:
        return 枚举_首收件人::自我线程;
    case 枚举_消息大类::未定义:
    default:
        return 枚举_首收件人::自我线程;
    }
}

inline bool 是否需要上浮到自我线程(const 自我线程消息协议::结构_治理消息& 消息) noexcept
{
    using namespace 自我线程消息协议;
    if (判定首收件人(消息) == 枚举_首收件人::自我线程) {
        return true;
    }

    switch (消息.头.子类) {
    case 枚举_消息子类::局部外部反馈:
        return false;
    default:
        return true;
    }
}

inline bool 是否需要路由回父任务(const 自我线程消息协议::结构_治理消息& 消息) noexcept
{
    using namespace 自我线程消息协议;
    switch (消息.头.大类) {
    case 枚举_消息大类::结果回流消息:
        return true;
    case 枚举_消息大类::派生需求消息:
        return 消息.头.父任务主键 != 0;
    default:
        return false;
    }
}

} // namespace 自我线程消息路由
