module;

#include <cstdint>
#include <string>

export module 任务模块.实体;

export enum class 枚举_任务队列状态 : std::uint8_t {
    未定义 = 0,
    可运行 = 1,
    等待 = 2,
    已派发 = 3,
    等待子结果 = 4,
    收束中 = 5,
    已停止 = 6,
    已结算 = 7,
};

export enum class 枚举_任务局部运行状态 : std::uint8_t {
    未定义 = 0,
    已受理 = 1,
    展开步骤中 = 2,
    等待前序步骤 = 3,
    等待条件 = 4,
    等待子结果 = 5,
    当前步骤可执行 = 6,
    局部完成 = 7,
    局部失败 = 8,
    已中断 = 9,
    已收束 = 10,
    已停止 = 11,
};

export struct 结构_任务根节点 {
    std::uint64_t 任务根ID = 0;
    std::uintptr_t 来源需求指针 = 0;
    std::uintptr_t 世界任务根指针 = 0;
    std::uintptr_t 父任务指针 = 0;
    std::uintptr_t 管理对象任务指针 = 0;
    std::int64_t 初始授权等级 = 0;
    bool 只读 = true;
};

export struct 结构_任务控制态 {
    std::uint64_t 任务根ID = 0;
    枚举_任务队列状态 队列状态 = 枚举_任务队列状态::未定义;
    std::int64_t 全局优先级 = 0;
    std::int64_t 解阻加权 = 0;
    std::int64_t 时间预算_ms = 0;
    std::int64_t 资源预算 = 0;
    std::uint64_t 绑定工作线程ID = 0;
    bool 已派发 = false;
    bool 等待子结果 = false;
    bool 请求收束 = false;
    bool 请求停止 = false;
    bool 是否已响应控制意图 = false;
    bool 控制请求待确认 = false;
};

export struct 结构_任务局部运行态 {
    std::uint64_t 任务根ID = 0;
    枚举_任务局部运行状态 当前状态 = 枚举_任务局部运行状态::未定义;
    std::uintptr_t 当前步骤前沿 = 0;
    std::string 当前等待原因{};
    std::uintptr_t 最近局部结果 = 0;
    std::int64_t 局部完成度 = 0;
};
