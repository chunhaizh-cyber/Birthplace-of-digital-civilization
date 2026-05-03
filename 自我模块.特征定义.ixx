module;

#include "语素信息类型.h"

export module 自我模块.特征定义;

export class 自我特征定义类 {
public:
    static void 初始化依赖语素() noexcept;

    static const 词性节点类* 类型_自我_安全值() noexcept;
    static const 词性节点类* 类型_自我_服务值() noexcept;
    static const 词性节点类* 类型_自我_物理安全() noexcept;
    static const 词性节点类* 类型_自我_风险安全() noexcept;
    static const 词性节点类* 类型_自我_情绪ID() noexcept;
    static const 词性节点类* 类型_自我_情绪强度() noexcept;
    static const 词性节点类* 类型_自我_外显情绪ID() noexcept;
    static const 词性节点类* 类型_自我_外显情绪强度() noexcept;
    static const 词性节点类* 类型_自我_待学习方法数量() noexcept;
    static const 词性节点类* 类型_自我_可用方法数量() noexcept;
    static const 词性节点类* 类型_自我_当前主需求() noexcept;
    static const 词性节点类* 类型_自我_当前主任务() noexcept;
    static const 词性节点类* 类型_自我_当前主方法() noexcept;
    static const 词性节点类* 类型_自我_外设可用性() noexcept;
    static const 词性节点类* 类型_自我_尝试学习状态() noexcept;
    static const 词性节点类* 类型_自我_自我能力() noexcept;
    static const 词性节点类* 类型_自我_待机状态() noexcept;
    static const 词性节点类* 类型_自我_时序正向步长() noexcept;
    static const 词性节点类* 类型_自我_时序反向步长() noexcept;
    static const 词性节点类* 类型_自我_服务时序衰减步长() noexcept;

    static const 词性节点类* 类型_线程_生命周期状态() noexcept;
    static const 词性节点类* 类型_线程_控制意图() noexcept;
    static const 词性节点类* 类型_线程_实际运行状态() noexcept;
    static const 词性节点类* 类型_线程_健康状态() noexcept;
    static const 词性节点类* 类型_线程_当前预算() noexcept;
    static const 词性节点类* 类型_线程_绑定任务() noexcept;
    static const 词性节点类* 类型_线程_阻塞状态() noexcept;
    static const 词性节点类* 类型_线程_运行模式() noexcept;
    static const 词性节点类* 类型_线程_邮箱深度() noexcept;
    static const 词性节点类* 类型_线程_心跳健康() noexcept;
    static const 词性节点类* 类型_线程_Tick计数() noexcept;
    static const 词性节点类* 类型_线程_最近Tick时间() noexcept;

    static const 词性节点类* 类型_摘要_快照序号() noexcept;
    static const 词性节点类* 类型_摘要_世界版本() noexcept;
    static const 词性节点类* 类型_摘要_采集时间() noexcept;
    static const 词性节点类* 类型_摘要_活跃需求数() noexcept;
    static const 词性节点类* 类型_摘要_等待子需求数() noexcept;
    static const 词性节点类* 类型_摘要_已满足需求数() noexcept;
    static const 词性节点类* 类型_摘要_筹办中任务数() noexcept;
    static const 词性节点类* 类型_摘要_执行中任务数() noexcept;
    static const 词性节点类* 类型_摘要_等待任务数() noexcept;
    static const 词性节点类* 类型_摘要_方法头节点数() noexcept;
    static const 词性节点类* 类型_摘要_方法结构缺口数() noexcept;
    static const 词性节点类* 类型_摘要_能力包缺失数() noexcept;
    static const 词性节点类* 类型_摘要_条件结果对缺失数() noexcept;

    static const 词性节点类* 类型_自检_累计提交需求数() noexcept;
    static const 词性节点类* 类型_自检_累计发现问题数() noexcept;
    static const 词性节点类* 类型_自检_最近检查时间() noexcept;
    static const 词性节点类* 类型_自检_最近提交时间() noexcept;

    static const 词性节点类* 类型_任务_当前阶段() noexcept;
    static const 词性节点类* 类型_任务_选中自我能力() noexcept;
    static const 词性节点类* 类型_任务_状态() noexcept;
    static const 词性节点类* 类型_任务_角色() noexcept;
    static const 词性节点类* 类型_任务_节点种类() noexcept;

    static const 词性节点类* 类型_任务管理_缺口类型() noexcept;
    static const 词性节点类* 类型_任务管理_本能能力缺口类型() noexcept;
};
