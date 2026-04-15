module;

#include <cstdint>
#include <string>
#include <vector>

export module 任务模块.工作线程协议;

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
    std::string 摘要{};
};

export struct 结构_任务推进事件 {
    std::uint64_t 任务根ID = 0;
    枚举_任务推进事件类型 事件类型 = 枚举_任务推进事件类型::未定义;
    std::uintptr_t 当前步骤指针 = 0;
    std::uintptr_t 结果指针 = 0;
    std::string 等待原因{};
    std::string 派生需求摘要{};
    std::string 摘要{};
};
