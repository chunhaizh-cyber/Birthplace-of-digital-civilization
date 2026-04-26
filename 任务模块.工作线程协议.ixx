module;

#include <cstdint>
#include <string>
#include <vector>

export module 任务模块.工作线程协议;

import 任务管理任务模块.消息协议;

export enum class 枚举_任务控制事件类型 : std::uint8_t {
    未定义 = 0,
    启动 = 1,
    打断 = 2,
    收束 = 3,
    停止 = 4,
    优先级刷新 = 5,
    预算刷新 = 6,
    子结果送达 = 7,
};

export enum class 枚举_任务推进事件类型 : std::uint8_t {
    未定义 = 0,
    当前步骤可执行 = 1,
    当前步骤阻塞 = 2,
    派生需求上报 = 3,
    局部结果返回 = 4,
    最终结果返回 = 5,
    异常上报 = 6,
};

export struct 结构_任务控制事件 {
    std::uint64_t 任务根ID = 0;
    枚举_任务控制事件类型 事件类型 = 枚举_任务控制事件类型::未定义;
    std::int64_t 优先级 = 0;
    std::int64_t 时间预算_ms = 0;
    std::int64_t 资源预算 = 0;
    std::uintptr_t 子结果指针 = 0;
    std::string 说明{};
};

export struct 结构_任务推进事件 {
    std::uint64_t 任务根ID = 0;
    枚举_任务推进事件类型 事件类型 = 枚举_任务推进事件类型::未定义;
    std::uintptr_t 当前步骤指针 = 0;
    std::uintptr_t 结果指针 = 0;
    std::string 等待原因{};
    std::string 说明{};
};

export namespace 任务线程消息合同 {

struct 结构_线程消息输入 {
    结构_任务控制事件 控制事件{};
    bool 存在控制请求 = false;
    结构_任务管理控制请求 控制请求{};
    时间戳 当前时间 = 0;
    std::string 调用点{};
};

struct 结构_线程控制消费 {
    bool 已消费控制请求 = false;
    结构_任务管理控制请求 已消费控制请求对象{};
    结构_任务管理控制响应记录 控制响应{};
    bool 控制阻断执行 = false;
};

struct 结构_线程状态回报 {
    bool 已生成线程状态消息 = false;
    结构_任务管理线程状态消息 状态消息{};
};

struct 结构_线程结果上行 {
    std::vector<结构_任务推进事件> 推进事件列表{};
    std::vector<结构_任务管理上行消息> 上行消息集{};
};

struct 结构_线程消息处理合同 {
    结构_线程消息输入 消息输入{};
    结构_线程控制消费 控制消费{};
    结构_线程状态回报 状态回报{};
    结构_线程结果上行 结果上行{};
};

} // namespace 任务线程消息合同

export using 任务线程消息合同::结构_线程消息输入;
export using 任务线程消息合同::结构_线程控制消费;
export using 任务线程消息合同::结构_线程状态回报;
export using 任务线程消息合同::结构_线程结果上行;
export using 任务线程消息合同::结构_线程消息处理合同;
