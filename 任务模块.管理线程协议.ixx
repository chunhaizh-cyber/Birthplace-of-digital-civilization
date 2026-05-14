module;

#include <cstdint>
#include <string>
#include <vector>

#include "基础信息节点类型.h"
#include "需求节点类型.h"
#include "任务主信息类.h"

export module 任务模块.管理线程协议;

export import 任务模块.实体;
export import 任务模块.工作线程协议;
export import 任务模块.工作线程消息协议;
export import 任务模块.治理协议;
export import 任务模块.筹办;
export import 任务模块.执行;
export import 任务模块.运行包;

export namespace 任务管理线程协议 {

enum class 枚举_任务管理请求类型 : std::uint8_t {
    未定义 = 0,
    发起任务 = 1,
    调度任务 = 2,
    重筹办任务 = 3,
    等待任务 = 4,
    取消任务 = 5,
    更新权重 = 6,
    同步回执 = 7,
};

enum class 枚举_任务推进模式 : std::uint8_t {
    自动 = 0,
    只创建 = 1,
    只筹办 = 2,
    只执行 = 3,
    只同步回执 = 4,
    只状态推进 = 5,
};

enum class 枚举_任务界面请求状态 : std::uint8_t {
    未定义 = 0,
    已接收 = 1,
    校验失败 = 2,
    已绑定任务虚拟存在 = 3,
    已初始化任务语义 = 4,
    等待工作线程 = 5,
    已派发工作线程 = 6,
    等待工作结果 = 7,
    已收到工作结果 = 8,
    已生成上行 = 9,
    已完成 = 10,
    已取消 = 11,
    故障 = 12,
};

enum class 枚举_任务界面调度动作 : std::uint8_t {
    未定义 = 0,
    挂起等待 = 1,
    派发工作线程 = 2,
    重新排队 = 3,
    结束调度 = 4,
    拒绝请求 = 5,
};

struct 结构_任务治理请求 {
    // 请求类型决定界面线程如何解释本请求：
    // 发起任务允许任务信息节点为空；调度和状态推进类请求必须有任务信息节点。
    枚举_任务管理请求类型 请求类型 = 枚举_任务管理请求类型::调度任务;

    std::uint64_t 请求ID = 0;
    std::string 幂等键{};

    任务节点类* 任务信息节点 = nullptr;
    任务节点类* 上级任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;

    存在节点类* 发起存在 = nullptr;
    存在节点类* 作用对象 = nullptr;
    场景节点类* 所属场景 = nullptr;

    时间戳 当前时间 = 0;

    bool 允许创建任务信息节点 = false;
    bool 允许创建任务虚拟存在 = true;
    bool 允许初始化任务语义 = true;
    bool 允许派发工作线程 = true;
    bool 允许复用等待任务 = true;

    枚举_任务状态 建议初始任务状态 = 枚举_任务状态::未启动;
    I64 需求承接权重 = 0;
    I64 任务初始权重 = 0;
    const 语素入口节点类* 任务完成判据 = nullptr;
    const 语素入口节点类* 需求回写规则 = nullptr;
    const 语素入口节点类* 调度原因 = nullptr;

    std::string 请求摘要{};
};

struct 结构_任务状态推进请求 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;
    枚举_任务状态 建议任务状态 = 枚举_任务状态::未定义;
    枚举_任务阶段 建议任务阶段 = 枚举_任务阶段::未定义;
    bool 写入任务状态 = true;
    bool 写入任务阶段 = false;
    bool 等待缺口唤醒 = false;
    时间戳 当前时间 = 0;
    std::string 原因摘要{};
};

struct 结构_任务权重固化请求 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;
    I64 承接安全权重上限 = 0;
    I64 承接服务权重上限 = 0;
    I64 任务权重评分 = 0;
    I64 任务权重版本号 = 0;
    时间戳 当前时间 = 0;
    std::string 原因摘要{};
};

struct 结构_任务价值结算回执 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;
    bool 写入已折算安全值 = false;
    bool 写入已折算服务值 = false;
    I64 已折算安全值 = 0;
    I64 已折算服务值 = 0;
    时间戳 当前时间 = 0;
    std::string 反馈事件主键{};
};

struct 结构_任务工作包 {
    std::uint64_t 请求ID = 0;
    std::uint64_t 工作包ID = 0;
    std::string 幂等键{};

    任务节点类* 任务信息节点 = nullptr;
    存在节点类* 任务虚拟存在 = nullptr;
    需求节点类* 来源需求 = nullptr;

    时间戳 当前时间 = 0;

    结构_任务根节点 任务根节点{};
    结构_任务控制态 任务控制态{};
    结构_任务局部运行态 任务局部运行态{};
    结构_任务控制事件 控制事件{};

    枚举_任务管理根层重判结果 根层重判结果 =
        枚举_任务管理根层重判结果::未定义;
    枚举_任务管理执行前门控结果 执行前门控结果 =
        枚举_任务管理执行前门控结果::未定义;

    bool 存在一步治理任务包 = false;
    任务运行包::结构_一步治理任务包 一步治理请求包{};

    // 推进模式由界面线程根据请求类型和任务状态决定。
    // 默认工作包不应同时筹办和执行；若要自动模式，必须由调用方显式给出。
    枚举_任务推进模式 推进模式 = 枚举_任务推进模式::自动;

    bool 允许筹办 = true;
    bool 允许执行方法 = true;
    bool 允许派生需求上报 = true;
    bool 允许正式资产提交 = false;

    std::string 任务摘要{};
    std::string 摘要{};
};

struct 结构_任务工作结果 {
    std::uint64_t 请求ID = 0;
    std::uint64_t 工作包ID = 0;
    std::string 幂等键{};

    任务节点类* 任务信息节点 = nullptr;
    存在节点类* 任务虚拟存在 = nullptr;
    需求节点类* 来源需求 = nullptr;

    bool 已处理 = false;
    bool 推进成功 = false;
    bool 被门控阻断 = false;
    bool 已执行筹办 = false;
    bool 已执行方法 = false;
    bool 任务完成 = false;
    bool 任务失败 = false;
    bool 需要等待 = false;
    bool 产生派生需求 = false;
    bool 建议自我线程重判 = false;
    bool 建议界面线程下一轮 = false;
    bool 建议进入收束 = false;

    I64 错误码 = 0;
    std::string 错误说明{};
    std::string 等待原因{};

    结构_任务控制态 最新任务控制态{};
    结构_任务局部运行态 最新任务局部运行态{};

    结构_任务管理结果 治理结果{};

    bool 存在任务推进回执 = false;
    任务模块::筹办::结构_任务推进回执 任务推进回执{};

    bool 存在方法执行数据 = false;
    结构_任务步骤执行结果_最小 方法执行数据{};
    特征节点类* 方法运行结果 = nullptr;

    std::vector<结构_任务推进事件> 推进事件列表{};
    std::vector<结构_任务管理上行消息> 上行消息集{};

    枚举_任务管理下一步去向 下一步去向 =
        枚举_任务管理下一步去向::未定义;
    枚举_任务管理总控结果 最近总控结果 =
        枚举_任务管理总控结果::未定义;

    bool 任务管理上位输入已刷新 = false;
    bool 任务管理治理触发 = false;

    bool 已生成一步治理任务包 = false;
    bool 已生成回流结算包 = false;
    任务运行包::结构_一步治理任务包 一步治理请求包{};
    任务运行包::结构_回流结算包 回流结算包{};

    std::string 摘要{};
};

struct 结构_任务界面承接结果 {
    std::uint64_t 请求ID = 0;
    std::string 幂等键{};

    bool 已接收 = false;
    bool 已校验 = false;
    bool 已绑定任务虚拟存在 = false;
    bool 已初始化任务语义 = false;
    bool 已构造工作包 = false;
    bool 已派发工作线程 = false;
    bool 已进入等待队列 = false;

    任务节点类* 任务信息节点 = nullptr;
    存在节点类* 任务虚拟存在 = nullptr;
    需求节点类* 来源需求 = nullptr;

    结构_任务根节点 任务根节点{};
    结构_任务控制态 最新任务控制态{};
    结构_任务局部运行态 最新任务局部运行态{};
    std::vector<结构_任务推进事件> 推进事件列表{};
    std::vector<结构_任务管理上行消息> 上行消息集{};

    枚举_任务管理下一步去向 下一步去向 =
        枚举_任务管理下一步去向::未定义;
    枚举_任务管理总控结果 最近总控结果 =
        枚举_任务管理总控结果::未定义;
    bool 任务管理上位输入已刷新 = false;
    bool 任务管理治理触发 = false;

    枚举_任务界面请求状态 请求状态 =
        枚举_任务界面请求状态::未定义;
    枚举_任务界面调度动作 调度动作 =
        枚举_任务界面调度动作::未定义;

    std::string 说明{};
};

struct 结构_任务界面线程快照 {
    bool 已启动 = false;
    bool 正在处理 = false;
    bool 故障 = false;

    std::uint64_t 累计接收请求数 = 0;
    std::uint64_t 累计绑定任务虚拟存在数 = 0;
    std::uint64_t 累计派发工作包数 = 0;
    std::uint64_t 累计收到工作结果数 = 0;
    std::uint64_t 累计上行消息数 = 0;

    std::uint64_t 当前请求队列长度 = 0;
    std::uint64_t 当前等待工作包数 = 0;
    std::uint64_t 当前上行队列长度 = 0;

    std::uint64_t 最近请求ID = 0;
    std::uint64_t 最近工作包ID = 0;

    std::string 最近任务主键{};
    std::string 最近任务虚拟存在主键{};
    std::string 最近需求主键{};
    std::string 最近调度动作{};
    std::string 最近说明{};
};

} // namespace 任务管理线程协议
