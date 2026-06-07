module;

#include <cstdint>
#include <string>
#include <vector>

#include "基础信息节点类型.h"
#include "需求节点类型.h"
#include "方法类.h"
#include "任务主信息类.h"

export module 任务模块.管理线程协议;

export import 任务模块.实体;
export import 任务模块.工作线程协议;
export import 任务模块.工作线程消息协议;
export import 任务模块.治理协议;
export import 任务模块.筹办;
export import 任务模块.执行;
export import 任务模块.运行包;
export import 外设观察报告队列;

export namespace 任务管理线程协议 {

inline constexpr std::uint64_t 任务管理工作线程池默认大小_常量 = 20;
inline constexpr std::uint64_t 任务管理工作线程池最小大小_常量 = 1;
inline constexpr std::uint64_t 任务管理工作线程池最大大小_常量 = 128;

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

enum class 枚举_任务调度事件类型 : std::uint8_t {
    未定义 = 0,
    任务已创建 = 1,
    任务已进入待筹办 = 2,
    筹办回执已同步 = 3,
    任务已就绪 = 4,
    执行回执已同步 = 5,
    条件缺口已满足 = 6,
    派生需求已回写 = 7,
    状态推进请求 = 8,
    租约超时 = 9,
    人工重试 = 10,
    人工取消 = 11,
    普通推进 = 12,
};

enum class 枚举_派生需求入树处理结果 : std::uint8_t {
    未定义 = 0,
    已新增 = 1,
    已更新 = 2,
    已合并 = 3,
    已拒绝 = 4,
    已延后 = 5,
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

enum class 枚举_任务工作项类型 : std::uint8_t {
    未定义 = 0,
    筹办 = 1,
    执行 = 2,
};

struct 结构_任务治理请求 {
    // 请求类型决定界面线程如何解释本请求：
    // 发起任务允许任务信息节点为空；调度、重筹办、等待等治理请求必须有任务信息节点。
    // 任务状态值推进不作为自我线程可提交的跨线程请求；外部只提交生命周期事件。
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
    // 仅供任务管理界面线程内部把已裁决状态写入任务壳。
    // 自我线程不得构造本结构来指定目标任务状态或阶段。
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;
    枚举_任务状态 建议任务状态 = 枚举_任务状态::未定义;
    枚举_任务阶段 建议任务阶段 = 枚举_任务阶段::未定义;
    bool 写入任务状态 = true;
    bool 写入任务阶段 = false;
    bool 等待缺口唤醒 = false;
    时间戳 当前时间 = 0;
};

struct 结构_任务状态推进结果 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;
    枚举_任务状态 原任务状态 = 枚举_任务状态::未定义;
    枚举_任务状态 新任务状态 = 枚举_任务状态::未定义;
    枚举_任务阶段 原任务阶段 = 枚举_任务阶段::未定义;
    枚举_任务阶段 新任务阶段 = 枚举_任务阶段::未定义;
    bool 已提交任务状态 = false;
    bool 已提交任务阶段 = false;
    bool 等待缺口唤醒 = false;
    动态节点类* 任务状态动作动态 = nullptr;
    std::string 摘要{};
};

struct 结构_观察事实更新任务唤醒通知 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;
    std::uint64_t 报告ID = 0;
    std::uint64_t 等待项ID = 0;
    bool 安全根路径 = false;
    时间戳 当前时间 = 0;
    std::string 事件摘要{};
};

struct 结构_父任务等待条件变化通知 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 父需求 = nullptr;
    需求节点类* 子需求 = nullptr;
    任务节点类* 子任务信息节点 = nullptr;
    动态节点类* 子任务状态动作动态 = nullptr;
    bool 父需求仍需推进 = false;
    bool 子需求已满足 = false;
    时间戳 当前时间 = 0;
    std::string 事件摘要{};
};

struct 结构_任务价值结算完成通知 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;
    时间戳 当前时间 = 0;
    std::string 事件摘要{};
};

struct 结构_任务执行前许可请求 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;
    方法类::节点类* 当前方法 = nullptr;
    std::uint64_t 请求ID = 0;
    std::uint64_t 工作项ID = 0;
    时间戳 当前时间 = 0;
    I64 预计安全变化量 = 0;
    I64 预计服务变化量 = 0;
    bool 禁止项接口已预留 = true;
    bool 禁止项命中 = false;
    bool 方向证据已提供 = false;
    bool 方向不一致 = false;
    // 仅供日志展示，不作为机器审批语义。
    std::string 请求摘要{};
};

struct 结构_任务执行前许可结果 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;
    方法类::节点类* 当前方法 = nullptr;
    std::uint64_t 工作项ID = 0;
    bool 已审批 = false;
    bool 允许执行 = false;
    bool 需要等待 = false;
    bool 需要收束 = false;
    bool 禁止项接口已预留 = true;
    bool 禁止项命中 = false;
    bool 方向证据已提供 = false;
    bool 方向不一致 = false;
    // 仅供日志展示，不作为机器审批语义。
    std::string 原因{};
};

struct 结构_任务权重固化请求 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 来源需求 = nullptr;
    I64 承接安全权重上限 = 0;
    I64 承接服务权重上限 = 0;
    I64 任务权重评分 = 0;
    I64 任务权重版本号 = 0;
    时间戳 当前时间 = 0;
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

struct 结构_派生需求入树回执 {
    任务节点类* 任务信息节点 = nullptr;
    需求节点类* 父需求 = nullptr;
    需求节点类* 派生需求 = nullptr;
    枚举_派生需求入树处理结果 处理结果 =
        枚举_派生需求入树处理结果::未定义;
    bool 是否阻塞父任务推进 = false;
    时间戳 当前时间 = 0;
    std::string 原派生消息幂等键{};
    std::string 原派生消息摘要{};
    // 仅用于回执日志和人工排查；不得作为任务推进、筹办、结算或因果匹配依据。
    std::string 原因摘要{};
};

struct 结构_任务调度运行参数 {
    std::uint64_t 任务管理工作线程池当前有效大小 =
        任务管理工作线程池默认大小_常量;
    std::uint64_t 任务管理工作线程池待保存大小 =
        任务管理工作线程池默认大小_常量;
    std::uint64_t 任务管理工作线程池参数版本 = 1;
    std::string 最近参数保存结果 = "尚未保存";
    std::string 最近参数错误{};
    std::string 最近参数应用结果 = "尚未应用";
};

struct 结构_任务工作项 {
    std::uint64_t 请求ID = 0;
    std::uint64_t 工作项ID = 0;
    std::string 幂等键{};

    std::uint64_t 状态版本 = 0;
    std::uint64_t 来源调度事件ID = 0;
    枚举_任务调度事件类型 来源调度事件类型 =
        枚举_任务调度事件类型::未定义;
    时间戳 创建时间 = 0;
    时间戳 入队时间 = 0;
    时间戳 最早派发时间 = 0;
    时间戳 租约截止时间 = 0;
    std::uint32_t 重试次数 = 0;
    std::string 入队原因{};

    枚举_任务工作项类型 工作类型 = 枚举_任务工作项类型::未定义;

    任务节点类* 任务信息节点 = nullptr;
    存在节点类* 任务虚拟存在 = nullptr;
    需求节点类* 来源需求 = nullptr;

    时间戳 当前时间 = 0;

    结构_任务根节点 任务根节点{};
    结构_任务控制态 任务控制态{};
    结构_任务局部运行态 任务局部运行态{};

    std::string 任务摘要{};
    std::string 摘要{};

    bool 存在外设观察等待项 = false;
    结构_外设观察等待项 外设观察等待项{};
};

struct 结构_任务筹办情况 {
    bool 有效 = false;
    bool 已执行筹办 = false;
    任务模块::筹办::结构_任务推进回执 筹办回执{};
    特征节点类* 方法运行结果 = nullptr;
    场景节点类* 结果场景 = nullptr;
    枚举_任务状态 建议任务状态 = 枚举_任务状态::未定义;
    枚举_任务阶段 建议任务阶段 = 枚举_任务阶段::未定义;
    // 仅供程序调试和日志展示，绝不能作为逻辑语义、业务判断、去重、结算或因果依据。
    std::string 诊断摘要{};
};

struct 结构_任务执行情况 {
    bool 有效 = false;
    bool 已执行方法 = false;
    任务模块::筹办::结构_任务推进回执 执行回执{};
    结构_任务步骤执行结果_最小 执行数据{};
    特征节点类* 方法运行结果 = nullptr;
    场景节点类* 输出结果场景 = nullptr;
    枚举_任务状态 建议任务状态 = 枚举_任务状态::未定义;
    枚举_任务阶段 建议任务阶段 = 枚举_任务阶段::未定义;
    // 仅供程序调试和日志展示，绝不能作为逻辑语义、业务判断、去重、结算或因果依据。
    std::string 诊断摘要{};
};

struct 结构_任务工作结果 {
    std::uint64_t 请求ID = 0;
    std::uint64_t 工作项ID = 0;
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
    // 仅供程序调试和日志展示，绝不能作为逻辑语义、业务判断、去重、结算或因果依据。
    std::string 等待原因{};

    结构_任务控制态 最新任务控制态{};
    结构_任务局部运行态 最新任务局部运行态{};

    结构_任务管理结果 治理结果{};

    bool 存在任务筹办情况 = false;
    结构_任务筹办情况 任务筹办情况{};

    bool 存在任务执行情况 = false;
    结构_任务执行情况 任务执行情况{};

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

    bool 存在外设消息承接结果 = false;
    bool 外设消息命中报告 = false;
    std::uint64_t 外设观察等待项ID = 0;
    std::uint64_t 外设观察报告ID = 0;
    std::uint64_t 外设观察承接上行消息数 = 0;
    // 仅供程序调试和日志展示，绝不能作为逻辑语义、业务判断、去重、结算或因果依据。
    std::string 外设消息承接摘要{};

    std::string 摘要{};
};

struct 结构_任务工作完成项 {
    结构_任务工作项 工作项{};
    结构_任务工作结果 工作结果{};

    bool 已派发 = false;
    时间戳 派发时间 = 0;
    时间戳 完成时间 = 0;
    std::string 完成摘要{};
};

struct 结构_任务界面承接结果 {
    std::uint64_t 请求ID = 0;
    std::string 幂等键{};

    bool 已接收 = false;
    bool 已校验 = false;
    bool 已绑定任务虚拟存在 = false;
    bool 已初始化任务语义 = false;
    bool 已构造工作项 = false;
    bool 已派发工作线程 = false;
    bool 已进入等待队列 = false;
    std::uint64_t 工作项ID = 0;

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
    std::uint64_t 累计派发工作项数 = 0;
    std::uint64_t 累计收到工作结果数 = 0;
    std::uint64_t 累计上行消息数 = 0;
    std::uint64_t 累计调度事件数 = 0;
    std::uint64_t 累计消费工作项数 = 0;
    std::uint64_t 累计巡检任务数 = 0;
    std::uint64_t 累计巡检入队任务数 = 0;
    std::uint64_t 累计后台worker取项数 = 0;
    std::uint64_t 累计后台worker派发数 = 0;
    std::uint64_t 累计派生需求入树回执数 = 0;
    std::uint64_t 累计状态提交待确认项数 = 0;

    std::uint64_t 当前请求队列长度 = 0;
    std::uint64_t 当前等待工作项数 = 0;
    std::uint64_t 当前状态提交待确认项数 = 0;
    std::uint64_t 当前待筹办工作项数 = 0;
    std::uint64_t 当前待执行工作项数 = 0;
    std::uint64_t 当前完成工作项数 = 0;
    std::uint64_t 当前死信工作项数 = 0;
    std::uint64_t 当前在途工作项数 = 0;
    std::uint64_t 当前任务状态快照数 = 0;
    std::uint64_t 当前等待任务索引数 = 0;
    std::uint64_t 当前挂起任务索引数 = 0;
    std::uint64_t 当前上行队列长度 = 0;
    bool 后台worker池运行 = false;
    std::uint64_t 后台worker线程数 = 0;
    std::uint64_t 任务管理工作线程池默认大小 =
        任务管理工作线程池默认大小_常量;
    std::uint64_t 任务管理工作线程池最小大小 =
        任务管理工作线程池最小大小_常量;
    std::uint64_t 任务管理工作线程池最大大小 =
        任务管理工作线程池最大大小_常量;
    std::uint64_t 任务管理工作线程池当前有效大小 =
        任务管理工作线程池默认大小_常量;
    std::uint64_t 任务管理工作线程池待保存大小 =
        任务管理工作线程池默认大小_常量;
    std::uint64_t 任务管理工作线程池参数版本 = 1;

    std::uint64_t 最近请求ID = 0;
    std::uint64_t 最近工作项ID = 0;
    std::uint64_t 最近调度事件ID = 0;
    std::uint64_t 最近任务状态版本 = 0;
    std::uint64_t 最近后台worker序号 = 0;
    std::uint64_t 最近后台worker工作项ID = 0;
    std::uint64_t 最近状态提交确认ID = 0;

    std::string 最近任务主键{};
    std::string 最近任务虚拟存在主键{};
    std::string 最近需求主键{};
    std::string 最近任务状态{};
    std::string 最近调度动作{};
    std::string 最近调度事件类型{};
    std::string 最近工作项类型{};
    std::string 最近入队原因{};
    std::string 最近派生需求入树处理结果{};
    std::string 最近派生需求入树摘要{};
    std::string 待筹办队列头部任务主键{};
    std::string 待执行队列头部任务主键{};
    I64 待筹办队列头部优先级 = 0;
    I64 待执行队列头部优先级 = 0;
    std::string 待筹办队列头部排序兜底键{};
    std::string 待执行队列头部排序兜底键{};
    std::string 最近排序原因{};
    std::uint64_t 最近巡检任务数 = 0;
    std::uint64_t 最近巡检入队任务数 = 0;
    std::uint64_t 最近巡检耗时微秒 = 0;
    std::uint64_t 最近巡检可推进任务数 = 0;
    std::uint64_t 最近巡检等待任务数 = 0;
    std::uint64_t 最近巡检挂起任务数 = 0;
    std::uint64_t 最近巡检终结任务数 = 0;
    std::uint64_t 最近巡检跳过已有工作项数 = 0;
    std::uint64_t 最近巡检跳过缺结构数 = 0;
    std::uint64_t 最近巡检跳过不可推进数 = 0;
    std::uint64_t 最近巡检跳过入队预算数 = 0;
    std::string 最近不可推进原因{};
    std::string 最近后台worker任务主键{};
    std::string 最近后台worker工作项类型{};
    std::string 最近后台worker状态{};
    std::string 最近后台worker原因{};
    std::string 最近状态提交确认任务主键{};
    std::string 最近状态提交确认状态{};
    std::string 最近参数保存结果 = "尚未保存";
    std::string 最近参数错误{};
    std::string 最近参数应用结果 = "尚未应用";
    std::string 最近说明{};
};

} // namespace 任务管理线程协议
