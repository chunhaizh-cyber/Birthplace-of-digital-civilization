module;

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include "基础数据类型.h"

export module 任务管理任务模块.消息协议;

export {

enum class 枚举_任务管理上行消息类型 : std::uint8_t {
    未定义 = 0,
    线程状态消息 = 1,
    结果状态消息 = 2,
};

enum class 枚举_任务管理结果状态 : std::uint8_t {
    未定义 = 0,
    成功 = 1,
    失败 = 2,
    部分完成 = 3,
    中断 = 4,
    收束完成 = 5,
    异常结束 = 6,
};

enum class 枚举_任务管理线程运行状态 : std::uint8_t {
    未定义 = 0,
    已启动 = 1,
    推进中 = 2,
    等待中 = 3,
    阻塞 = 4,
    故障 = 5,
    收束中 = 6,
    已停止 = 7,
};

enum class 枚举_任务管理线程补充特征类型 : std::uint8_t {
    未定义 = 0,
    当前步骤主键 = 1,
    当前步骤前沿 = 2,
    当前等待原因 = 3,
    是否可继续推进 = 4,
    是否已响应控制意图 = 5,
    当前预算消耗 = 6,
    当前时间消耗 = 7,
    是否超时 = 8,
    是否触发异常 = 9,
    当前局部风险等级 = 10,
    建议自我线程重判 = 11,
    建议收束 = 12,
    建议恢复重建 = 13,
};

struct 结构_任务管理消息头 {
    std::string 发出者主键{};
    std::string 任务管理任务主键{};
    std::string 任务主键{};
    std::string 父任务主键{};
    std::string 关联场景主键{};
    时间戳 产生时间 = 0;
    std::string 幂等键{};
};

struct 结构_任务管理线程补充特征 {
    枚举_任务管理线程补充特征类型 特征类型 = 枚举_任务管理线程补充特征类型::未定义;
    std::string 文本值{};
    I64 数值 = 0;
    bool 布尔值 = false;
    std::uintptr_t 指针值 = 0;
};

struct 结构_任务管理线程状态消息 {
    结构_任务管理消息头 消息头{};
    枚举_任务管理线程运行状态 当前运行状态 = 枚举_任务管理线程运行状态::未定义;
    std::vector<结构_任务管理线程补充特征> 补充特征集{};
};

struct 结构_任务管理结果状态消息 {
    结构_任务管理消息头 消息头{};

    std::string 结果节点主键{};
    std::uintptr_t 结果节点指针 = 0;
    std::string 结果节点场景主键{};
    bool 是否已正式写入结果节点 = false;

    枚举_任务管理结果状态 结果状态 = 枚举_任务管理结果状态::未定义;
    double 完成度 = 0.0;
    特征值 结果值{};
    bool 是否成功 = false;
    I64 错误码 = 0;
    时间戳 完成时间 = 0;

    bool 是否存在关键中间状态 = false;
    bool 是否仍有未满足项 = false;
    std::string 未满足项类型摘要{};
    bool 是否需要外部支撑 = false;
    bool 是否需要父任务处理 = false;
    bool 是否需要自我线程重判 = false;
    bool 是否疑似需要新任务入口 = false;
    bool 可能影响父任务 = false;
};

using 任务管理上行消息载荷 =
    std::variant<std::monostate, 结构_任务管理线程状态消息, 结构_任务管理结果状态消息>;

struct 结构_任务管理上行消息 {
    枚举_任务管理上行消息类型 类型 = 枚举_任务管理上行消息类型::未定义;
    任务管理上行消息载荷 载荷{};
};

inline const char* 任务管理上行消息类型文本(枚举_任务管理上行消息类型 类型) noexcept
{
    switch (类型) {
    case 枚举_任务管理上行消息类型::线程状态消息: return "线程状态消息";
    case 枚举_任务管理上行消息类型::结果状态消息: return "结果状态消息";
    default: return "未定义";
    }
}

} // export
