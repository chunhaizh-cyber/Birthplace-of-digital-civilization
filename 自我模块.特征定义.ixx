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
    static const 词性节点类* 类型_自检_范围() noexcept;
    static const 词性节点类* 类型_自检_缺口类型() noexcept;
    static const 词性节点类* 类型_自检_来源对象类型() noexcept;
    static const 词性节点类* 类型_自检_来源方法能力() noexcept;
    static const 词性节点类* 类型_自检_来源方法头节点() noexcept;
    static const 词性节点类* 类型_自检_目标特征类型() noexcept;
    static const 词性节点类* 类型_自检_缺失结构类型() noexcept;
    static const 词性节点类* 类型_自检_建议需求类型() noexcept;
    static const 词性节点类* 类型_自检_证据类型() noexcept;
    static const 词性节点类* 类型_自检_严重程度() noexcept;
    static const 词性节点类* 类型_自检_是否建议生成需求() noexcept;

    static const 词性节点类* 类型_任务_当前阶段() noexcept;
    static const 词性节点类* 类型_任务_选中自我能力() noexcept;
    static const 词性节点类* 类型_任务_状态() noexcept;
    static const 词性节点类* 类型_任务_角色() noexcept;
    static const 词性节点类* 类型_任务_节点种类() noexcept;
    static const 词性节点类* 类型_任务_队列状态() noexcept;
    static const 词性节点类* 类型_任务_局部运行状态() noexcept;

    static const 词性节点类* 类型_任务管理_缺口类型() noexcept;
    static const 词性节点类* 类型_任务管理_本能能力缺口类型() noexcept;
    static const 词性节点类* 类型_任务管理_功能域() noexcept;
    static const 词性节点类* 类型_任务管理_下一步去向() noexcept;
    static const 词性节点类* 类型_任务管理_治理状态迁移() noexcept;
    static const 词性节点类* 类型_任务管理_方法需求位() noexcept;
    static const 词性节点类* 类型_任务管理_总控结果() noexcept;
    static const 词性节点类* 类型_任务管理_根层重判结果() noexcept;
    static const 词性节点类* 类型_任务管理_根层重判时机() noexcept;
    static const 词性节点类* 类型_任务管理_根层结果来源类型() noexcept;
    static const 词性节点类* 类型_任务管理_目标对齐结论() noexcept;
    static const 词性节点类* 类型_任务管理_结果方向结论摘要() noexcept;
    static const 词性节点类* 类型_任务管理_根层禁止原因类型() noexcept;
    static const 词性节点类* 类型_任务管理_执行前门控结果() noexcept;
    static const 词性节点类* 类型_任务管理_反馈类型() noexcept;
    static const 词性节点类* 类型_任务管理_方法来源() noexcept;
    static const 词性节点类* 类型_任务管理_步骤位类型() noexcept;
    static const 词性节点类* 类型_任务管理_步骤语义类型() noexcept;
    static const 词性节点类* 类型_任务管理_双面比较结果() noexcept;
    static const 词性节点类* 类型_任务管理_后果治理() noexcept;
    static const 词性节点类* 类型_任务管理_分身状态() noexcept;
    static const 词性节点类* 类型_任务管理_主体汇总策略() noexcept;
    static const 词性节点类* 类型_任务管理_最小原语ID() noexcept;
    static const 词性节点类* 类型_任务管理_触发事件类型() noexcept;
    static const 词性节点类* 类型_任务管理_触发来源() noexcept;
    static const 词性节点类* 类型_任务管理_学习承接类型() noexcept;
    static const 词性节点类* 类型_任务管理_学习反馈消费策略() noexcept;
    static const 词性节点类* 类型_任务管理_治理恢复事件处理方式() noexcept;
    static const 词性节点类* 类型_任务管理_控制意图() noexcept;
    static const 词性节点类* 类型_任务管理_上行消息类型() noexcept;
    static const 词性节点类* 类型_任务管理_结果状态() noexcept;
    static const 词性节点类* 类型_任务管理_任务执行记账结果() noexcept;
    static const 词性节点类* 类型_任务管理_线程运行状态() noexcept;
    static const 词性节点类* 类型_任务管理_线程结束状态() noexcept;
    static const 词性节点类* 类型_任务管理_控制响应状态() noexcept;
    static const 词性节点类* 类型_任务管理_派生需求类型() noexcept;
    static const 词性节点类* 类型_任务管理_阻塞级别() noexcept;
    static const 词性节点类* 类型_任务管理_线程补充特征类型() noexcept;
    static const 词性节点类* 类型_任务管理_任务挂接方式() noexcept;
    static const 词性节点类* 类型_任务管理_学习目标类型() noexcept;
    static const 词性节点类* 类型_任务管理_学习需求类型() noexcept;
    static const 词性节点类* 类型_任务管理_学习动作类型() noexcept;
    static const 词性节点类* 类型_任务管理_学习原函数类型() noexcept;
    static const 词性节点类* 类型_任务管理_学习程序决策() noexcept;
    static const 词性节点类* 类型_任务管理_原语类型() noexcept;
    static const 词性节点类* 类型_任务管理_原语候选比较基准类型() noexcept;
    static const 词性节点类* 类型_任务管理_原语候选来源锚点类型() noexcept;

    static const 词性节点类* 类型_学习_阶段() noexcept;
    static const 词性节点类* 类型_学习_状态() noexcept;
    static const 词性节点类* 类型_学习_等待原因() noexcept;
    static const 词性节点类* 类型_学习_影响面() noexcept;
    static const 词性节点类* 类型_学习_结果状态() noexcept;
    static const 词性节点类* 类型_学习_外层裁决() noexcept;
    static const 词性节点类* 类型_学习_执行策略() noexcept;
    static const 词性节点类* 类型_学习_对象来源类型() noexcept;

    static const 词性节点类* 类型_运行包_包状态() noexcept;
    static const 词性节点类* 类型_运行包_需求大类() noexcept;
    static const 词性节点类* 类型_运行包_服务子类() noexcept;
    static const 词性节点类* 类型_运行包_安全子类() noexcept;
    static const 词性节点类* 类型_运行包_动作类型() noexcept;
    static const 词性节点类* 类型_运行包_结算结论() noexcept;
    static const 词性节点类* 类型_运行包_学习样本类别() noexcept;

    static const 词性节点类* 类型_工作线程_控制事件类型() noexcept;
    static const 词性节点类* 类型_工作线程_推进事件类型() noexcept;

    static const 词性节点类* 类型_消息_来源链() noexcept;
    static const 词性节点类* 类型_消息_处理类型() noexcept;
    static const 词性节点类* 类型_消息_优先级() noexcept;
    static const 词性节点类* 类型_消息_动作类型() noexcept;
    static const 词性节点类* 类型_消息_存在类型() noexcept;
    static const 词性节点类* 类型_消息_特征类型() noexcept;
    static const 词性节点类* 类型_消息_结果状态() noexcept;
    static const 词性节点类* 类型_消息_执行尝试汇总状态() noexcept;
    static const 词性节点类* 类型_消息_执行尝试验证状态() noexcept;
    static const 词性节点类* 类型_消息_执行尝试对账结论() noexcept;
};
