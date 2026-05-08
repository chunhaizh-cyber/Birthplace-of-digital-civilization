#pragma once

#include "任务主信息类.h"

class 任务类 : public 链表模板<任务主信息类> {
public:
    using 基类 = 链表模板<任务主信息类>;
    using 节点类 = typename 基类::节点类;

public:
    static 存在节点类* 取或创建_任务虚拟存在(
        节点类* 任务头结点,
        场景节点类* 承载场景 = nullptr);

    static bool 记录任务虚拟存在特征(
        节点类* 任务头结点,
        const 词性节点类* 特征类型,
        const 特征值& 值,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 初始化任务虚拟存在信息(
        节点类* 任务头结点,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 同步任务运行镜像特征(
        节点类* 任务头结点,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 写入任务根实体特征(
        节点类* 任务头结点,
        std::uint64_t 任务根ID,
        std::uintptr_t 来源需求指针,
        std::uintptr_t 世界任务根指针,
        std::uintptr_t 父任务指针,
        std::uintptr_t 管理对象任务指针,
        I64 初始授权等级,
        bool 只读,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 写入任务控制实体特征(
        节点类* 任务头结点,
        std::uint64_t 任务根ID,
        I64 队列状态值,
        I64 全局优先级,
        I64 解阻加权,
        I64 时间预算_ms,
        I64 资源预算,
        std::uint64_t 绑定工作线程ID,
        bool 已派发,
        bool 等待子结果,
        bool 请求收束,
        bool 请求停止,
        bool 是否已响应控制意图,
        bool 控制请求待确认,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 写入任务局部运行实体特征(
        节点类* 任务头结点,
        std::uint64_t 任务根ID,
        I64 当前状态值,
        std::uintptr_t 当前步骤前沿,
        std::uintptr_t 最近局部结果,
        I64 局部完成度,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 任务状态已终结(枚举_任务状态 状态) noexcept;
    static 枚举_任务状态 读取任务状态(
        const 节点类* 任务节点,
        枚举_任务状态 默认值 = 枚举_任务状态::未定义,
        场景节点类* 承载场景 = nullptr) noexcept;
    static bool 写入任务状态(
        节点类* 任务节点,
        枚举_任务状态 值,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;
    static 枚举_任务节点种类 读取任务节点种类(
        const 节点类* 任务节点,
        枚举_任务节点种类 默认值 = 枚举_任务节点种类::未定义,
        场景节点类* 承载场景 = nullptr) noexcept;
    static bool 写入任务节点种类(
        节点类* 任务节点,
        枚举_任务节点种类 值,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;
    static bool 任务允许派发(const 节点类* 任务节点) noexcept;
    static void 确保任务初始状态(节点类* 任务节点) noexcept;
    static void 标记任务待重筹办(节点类* 任务节点, 时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;
    static void 标记任务等待(节点类* 任务节点, 时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;
    static void 标记任务已承接(节点类* 任务节点, 时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;
    static void 标记任务已派发(节点类* 任务节点, 时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;
    static void 标记任务负载等待(节点类* 任务节点, 时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;
    static void 应用任务状态(节点类* 任务节点, 枚举_任务状态 新状态, 时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;
    static 节点类* 读取任务管理对象任务(节点类* 任务节点) noexcept;
    static const 节点类* 读取任务管理对象任务(const 节点类* 任务节点) noexcept;
    static 节点类* 读取任务管理对象或自身(节点类* 任务节点) noexcept;
    static bool 任务管理对象指向(const 节点类* 任务节点, const 节点类* 目标任务) noexcept;
    static bool 写入任务管理对象任务(
        节点类* 任务节点,
        节点类* 管理对象任务,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;
    static 方法节点类* 读取任务执行方法(节点类* 任务节点) noexcept;
    static const 方法节点类* 读取任务执行方法(const 节点类* 任务节点) noexcept;
    static bool 任务执行方法指向(const 节点类* 任务节点, const 方法节点类* 目标方法) noexcept;
    static bool 写入任务执行方法(
        节点类* 任务节点,
        方法节点类* 执行方法,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;
    static I64 读取任务选中自我能力值(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务选中自我能力值(
        节点类* 任务头结点,
        I64 值,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());
    static 枚举_任务角色 读取任务角色(
        const 节点类* 任务头结点,
        枚举_任务角色 默认值 = 枚举_任务角色::未定义,
        场景节点类* 承载场景 = nullptr);
    static bool 写入任务角色(
        节点类* 任务头结点,
        枚举_任务角色 值,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());
    static 枚举_任务阶段 读取任务当前阶段(
        const 节点类* 任务头结点,
        枚举_任务阶段 默认值 = 枚举_任务阶段::未定义,
        场景节点类* 承载场景 = nullptr);
    static bool 写入任务当前阶段(
        节点类* 任务头结点,
        枚举_任务阶段 值,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());
    static bool 读取任务等待缺口唤醒(const 节点类* 任务头结点, bool 默认值 = false, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务等待缺口唤醒(
        节点类* 任务头结点,
        bool 值,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static I64 读取任务基准优先级(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务基准优先级(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务局部优先级偏移(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务局部优先级偏移(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务调度优先级(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务调度优先级(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务承接安全权重上限(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务承接安全权重上限(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务承接服务权重上限(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务承接服务权重上限(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务权重评分(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务权重评分(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务权重版本号(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务权重版本号(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务已折算安全值(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务已折算安全值(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务已折算服务值(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务已折算服务值(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务运行次数(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务运行次数(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务成功次数(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务成功次数(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务失败次数(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务失败次数(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务连续失败次数(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务连续失败次数(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务转入缺口需求次数(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务转入缺口需求次数(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());
    static I64 读取任务最近错误码(const 节点类* 任务头结点, I64 默认值 = 0, 场景节点类* 承载场景 = nullptr);
    static bool 写入任务最近错误码(节点类* 任务头结点, I64 值, 场景节点类* 承载场景 = nullptr, 时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 同步任务特征使用统计(
        节点类* 任务头结点,
        const 结构_特征使用统计& 统计,
        枚举_特征生命周期 生命周期 = 枚举_特征生命周期::候选,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    I64 读取任务特征账本生命周期标准特征(
        节点类* 任务头结点,
        I64 默认值 = 0,
        场景节点类* 承载场景 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());


    节点类* 创建任务头结点(
        需求节点类* 来源需求,
        节点类* 上级任务节点 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    节点类* 创建任务执行步骤节点(
        节点类* 所属任务头结点,
        节点类* 上级任务节点 = nullptr,
        方法节点类* 当前方法首节点 = nullptr,
        状态节点类* 步骤目标状态 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

};
