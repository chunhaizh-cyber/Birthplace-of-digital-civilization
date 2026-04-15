module;

#include <cstdint>

export module 自我线程模块.消息路由;

import 自我线程模块.消息协议;

namespace 自我线程消息路由 {

enum class 枚举_首收件人 : std::uint8_t {
    自我线程 = 0,
};

inline 枚举_首收件人 判定首收件人(const 自我线程消息协议::结构_治理消息&) noexcept
{
    return 枚举_首收件人::自我线程;
}

inline bool 是否需要上浮到自我线程(const 自我线程消息协议::结构_治理消息&) noexcept
{
    return true;
}

inline bool 是否需要路由回父任务(const 自我线程消息协议::结构_治理消息& 消息) noexcept
{
    using namespace 自我线程消息协议;
    return (消息.头.处理类型 == 枚举_消息处理类型::派生需求上报消息
            || 消息.头.处理类型 == 枚举_消息处理类型::结果与关键中间状态回流消息)
        && 消息.头.父任务主键 != 0;
}

inline bool 是否影响全局优先级(const 自我线程消息协议::结构_治理消息& 消息) noexcept
{
    using namespace 自我线程消息协议;
    switch (消息.头.处理类型) {
    case 枚举_消息处理类型::派生需求上报消息:
        return false;
    case 枚举_消息处理类型::未定义:
        return false;
    default:
        return true;
    }
}

inline bool 是否触发二次特征重算(const 自我线程消息协议::结构_治理消息& 消息) noexcept
{
    using namespace 自我线程消息协议;
    return 消息.头.处理类型 != 枚举_消息处理类型::未定义;
}

} // namespace 自我线程消息路由

export namespace 自我线程消息路由 {
using ::自我线程消息路由::枚举_首收件人;
using ::自我线程消息路由::判定首收件人;
using ::自我线程消息路由::是否需要上浮到自我线程;
using ::自我线程消息路由::是否需要路由回父任务;
using ::自我线程消息路由::是否影响全局优先级;
using ::自我线程消息路由::是否触发二次特征重算;
}

export using 自我线程消息路由::枚举_首收件人;
export using 自我线程消息路由::判定首收件人;
export using 自我线程消息路由::是否需要上浮到自我线程;
export using 自我线程消息路由::是否需要路由回父任务;
export using 自我线程消息路由::是否影响全局优先级;
export using 自我线程消息路由::是否触发二次特征重算;
