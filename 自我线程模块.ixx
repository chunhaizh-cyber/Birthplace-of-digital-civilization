module;

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include "基础数据类型.h"
#include "需求类.h"
export module 自我线程模块;

import 自我模块;
import 任务管理任务模块;
import 任务模块.实体;
import 任务模块.管理工作线程;
import 任务模块.运行包;
import 自我线程模块.消息协议;
import 自我线程模块.消息处理器;
import 自我线程模块.消息批次执行器;
export enum class 枚举_线程生命周期状态 : std::uint8_t {
    未启动 = 0,
    启动中 = 1,
    运行中 = 2,
    停止请求中 = 3,
    收尾中 = 4,
    已停止 = 5,
    故障 = 6,
};

export enum class 枚举_自我线程运行阶段 : std::uint8_t {
    未定义 = 0,
    自我初始化 = 1,
    线程启动中 = 2,
    运行态建立 = 3,
    主循环消息归并 = 4,
    主循环初始化补齐 = 5,
    主循环主链回填 = 6,
    主循环根层裁决 = 7,
    主循环任务治理 = 8,
    主循环结果回流 = 9,
    主循环心跳巡检 = 10,
    停止请求中 = 11,
    线程收尾 = 12,
    线程停止 = 13,
    线程故障 = 14,
};

export enum class 枚举_自我线程最终去向 : std::uint8_t {
    未定义 = 0,
    继续提交 = 1,
    回到重筹办 = 2,
    转入学习 = 3,
    进入收束 = 4,
    待机 = 5,
    停止 = 6,
};

export struct 结构_自我运行阶段事件 {
    时间戳 时间 = 0;
    枚举_自我线程运行阶段 阶段 = 枚举_自我线程运行阶段::未定义;
    std::string 摘要{};
    std::uintptr_t 自我存在 = 0;
    std::uintptr_t 当前主需求 = 0;
    std::uintptr_t 当前主任务 = 0;
    std::uintptr_t 当前主方法 = 0;
    std::uintptr_t 当前管理任务 = 0;
};

export struct 结构_自我线程配置 {
    std::chrono::microseconds Tick间隔{250000};
    std::size_t 运行阶段事件保留上限 = 32;
};

export struct 结构_线程状态切换上报参数 {
    结构_线程存在初始化参数 线程存在初始化{};
    自我线程消息协议::枚举_存在类型 线程存在类型 =
        自我线程消息协议::枚举_存在类型::未定义;
    自我线程消息协议::枚举_特征类型 状态特征类型 =
        自我线程消息协议::枚举_特征类型::生命周期状态;
    I64 旧状态 = 0;
    I64 新状态 = 0;
    时间戳 发生时间 = 0;
    自我线程消息协议::枚举_消息来源链 来源链 =
        自我线程消息协议::枚举_消息来源链::线程直达消息;
    自我线程消息协议::枚举_存在类型 决策者类型 =
        自我线程消息协议::枚举_存在类型::未定义;
    std::uint64_t 决策者主键 = 0;
    std::string 决策者标签{};
    std::string 触发动作{};
    std::string 原因键{};
    std::string 原因摘要{};
    自我线程消息协议::枚举_存在类型 直属拥有者类型 =
        自我线程消息协议::枚举_存在类型::未定义;
    std::uint64_t 直属拥有者主键 = 0;
    std::uint64_t 关联任务主键 = 0;
    std::uint64_t 关联线程主键 = 0;
    bool 是否故障触发 = false;
    bool 是否stop链触发 = false;
    bool 无变化时仅同步特征 = true;
};

export class 自我线程类 {
public:
    struct 结构_需求列表项 {
        const 词性节点类* 需求类型 = nullptr;
        std::string 需求类型名称{};
        std::uintptr_t 需求节点指针 = 0;
        std::string 需求节点主键{};
        I64 节点安全权重 = 0;
        I64 节点服务权重 = 0;
        I64 类型安全权重汇总 = 0;
        I64 类型服务权重汇总 = 0;
    };

    struct 结构_循环结果 {
        bool 已补齐初始化 = false;
        bool 已回填主链 = false;
        bool 任务管理治理触发 = false;
        bool 任务管理证据触发 = false;
        bool 任务管理上位输入已刷新 = false;
        bool 需求树已更新 = false;
        bool 有派生需求待实例化 = false;
        bool 有子结果待路由 = false;

        std::size_t 治理mailbox待消费数 = 0;
        std::string 任务管理当前去向{};
        std::string 最近二次特征摘要{};
        std::string 最近影响项摘要{};
        std::string 最近主派发判定二次特征摘要{};
        std::string 最近根值摘要{};
        std::string 最近回流摘要{};
        std::string 最近结算摘要{};
        枚举_自我线程最终去向 当前最终去向 = 枚举_自我线程最终去向::未定义;
        枚举_自我线程运行阶段 当前运行阶段 = 枚举_自我线程运行阶段::未定义;
    };

    struct 结构_需求候选快照 {
        std::string 候选类型{};
        std::string 来源消息类型{};
        std::string 当前方法主键{};
        std::uint64_t 关联任务主键 = 0;
        bool 建议升格为正式需求 = false;
        bool 建议新任务入口 = false;
        bool 需要外部支撑 = false;
        bool 影响父任务推进 = false;
        std::string 摘要{};
    };

    struct 结构_任务权重候选快照 {
        std::string 对象类型{};
        std::uint64_t 对象主键 = 0;
        I64 原值 = 0;
        I64 候选值 = 0;
        I64 变化量 = 0;
        std::string 摘要{};
    };

    struct 结构_父子依赖权重候选快照 {
        std::uint64_t 父任务主键 = 0;
        std::uint64_t 子任务主键 = 0;
        I64 候选值 = 0;
        I64 变化量 = 0;
        bool 需要父任务处理 = false;
        std::string 摘要{};
    };

    struct 结构_方法禁止项快照 {
        std::string 方法主键{};
        std::string 来源消息类型{};
        std::string 禁止项类别{};
        std::string 方向区间{};
        I64 严重级别 = 0;
        bool 是否阻断 = false;
        bool 是否已验证 = false;
        bool 是否建议恢复后解除 = false;
        bool 反项需独立验证 = true;
        std::string 原因摘要{};
        std::string 摘要{};
    };

    struct 结构_需求与权重治理输出快照 {
        bool 已生成 = false;
        bool 已完成需求与权重更新 = false;
        std::string 主导需求候选{};
        std::uint64_t 主导任务候选主键 = 0;
        I64 最高优先级候选 = 0;
        std::string 摘要{};
        std::vector<结构_需求候选快照> 需求候选列表{};
        std::vector<结构_任务权重候选快照> 任务权重候选列表{};
        std::vector<结构_父子依赖权重候选快照> 父子依赖权重候选列表{};
        std::vector<结构_方法禁止项快照> 方法禁止项列表{};
    };

    struct 结构_主派发动作快照 {
        std::string 动作类型{};
        std::uint64_t 任务根ID = 0;
        bool 允许并发 = true;
        std::uint32_t 并发组 = 0;
        std::string 目标队列{};
        std::string 摘要{};
    };

    struct 结构_队列裁决快照 {
        std::string 队列名称{};
        bool 已命中 = false;
        I64 变更数量 = 0;
        std::string 裁决{};
        std::string 摘要{};
    };

    struct 结构_队列成员快照 {
        std::string 成员稳定键{};
        std::uint64_t 任务根ID = 0;
        bool 当前在队列 = false;
        时间戳 首次入队时间 = 0;
        时间戳 最近入队时间 = 0;
        时间戳 最近出队时间 = 0;
        std::string 来源分类{};
        std::string 来源动作{};
        std::string 来源摘要{};
        std::string 最近事件摘要{};
    };

    struct 结构_队列成员事件快照 {
        std::string 成员稳定键{};
        std::uint64_t 任务根ID = 0;
        bool 是否入队 = false;
        时间戳 发生时间 = 0;
        std::string 来源分类{};
        std::string 来源动作{};
        std::string 摘要{};
    };

    struct 结构_队列治理状态快照 {
        std::string 队列名称{};
        bool 已命中 = false;
        bool 当前活跃 = false;
        I64 本轮变更数量 = 0;
        std::uint64_t 累计命中次数 = 0;
        I64 累计变更数量 = 0;
        std::uint64_t 连续活跃轮数 = 0;
        std::uint64_t 累计活跃轮数 = 0;
        std::uint64_t 累计清空次数 = 0;
        std::size_t 本轮动作数 = 0;
        std::size_t 当前可见数量 = 0;
        std::size_t 历史峰值数量 = 0;
        std::string 数量口径{};
        std::uint64_t 最近任务根ID = 0;
        时间戳 最近激活时间 = 0;
        时间戳 最近清空时间 = 0;
        时间戳 最近更新时间 = 0;
        std::vector<std::uint64_t> 当前成员任务根ID集{};
        std::vector<std::uint64_t> 最近移出任务根ID集{};
        std::string 成员来源分类摘要{};
        std::string 成员来源摘要{};
        std::vector<结构_队列成员快照> 当前成员列表{};
        std::vector<结构_队列成员事件快照> 最近成员事件列表{};
        std::string 最近裁决{};
        std::string 最近动作摘要{};
        std::string 摘要{};
    };

    struct 结构_队列治理输出快照 {
        bool 已生成 = false;
        std::string 摘要{};
        std::vector<结构_队列治理状态快照> 队列状态列表{};
    };

    struct 结构_派发占用登记快照 {
        std::size_t 动作序号 = 0;
        std::string 动作类型{};
        std::string 占用队列{};
        std::uint64_t 包主键 = 0;
        std::uint64_t 任务根ID = 0;
        bool 已登记占用 = false;
        bool 已下发控制投影 = false;
        bool 已释放占用 = false;
        std::string 占用对象键{};
        std::string 控制投影摘要{};
        std::string 摘要{};
    };

    struct 结构_派发回执快照 {
        std::size_t 动作序号 = 0;
        std::string 动作类型{};
        std::uint64_t 包主键 = 0;
        std::uint64_t 任务根ID = 0;
        bool 已生成任务包 = false;
        bool 已封口任务包 = false;
        bool 已实际派发 = false;
        bool 已形成回流 = false;
        std::string 错误摘要{};
        std::string 摘要{};
    };

    struct 结构_主派发输出快照 {
        bool 已生成 = false;
        bool 已生成治理桥预判 = false;
        bool 已生成治理桥结果 = false;
        bool 有派生需求待实例化 = false;
        自我线程消息批次执行器::枚举_批次执行去向 批次执行去向 =
            自我线程消息批次执行器::枚举_批次执行去向::继续提交;
        bool 允许任务入口实例化 = false;
        bool 允许工作线程推进 = false;
        bool 治理桥允许进入执行链 = false;
        bool 治理桥要求触发学习 = false;
        bool 治理桥要求进入等待链 = false;
        bool 治理桥要求停止 = false;
        std::size_t 来源需求候选数 = 0;
        std::size_t 来源方法禁止项数 = 0;
        std::size_t 绝对禁止项数 = 0;
        std::size_t 已验证降根禁止项数 = 0;
        std::size_t 目标反向禁止项数 = 0;
        std::size_t 负区间禁止项数 = 0;
        I64 需求联合压力值 = 0;
        I64 禁止联合压力值 = 0;
        I64 短期收益估计值 = 0;
        I64 根方向收益估计值 = 0;
        I64 风险付出值 = 0;
        I64 恢复付出值 = 0;
        I64 收益估计值 = 0;
        I64 禁止付出值 = 0;
        bool 命中需求升格建议 = false;
        bool 命中新任务入口建议 = false;
        bool 命中禁止项重筹办建议 = false;
        bool 命中绝对禁止项 = false;
        bool 命中已验证降根禁止项 = false;
        bool 命中目标反向禁止项 = false;
        bool 命中方法硬阻断 = false;
        bool 命中绝对边界优先层 = false;
        bool 命中长期根方向否决层 = false;
        bool 命中收益放行层 = false;
        bool 命中禁止降级层 = false;
        bool 命中常规放行层 = false;
        bool 命中需求需求冲突 = false;
        bool 命中需求禁止项冲突 = false;
        bool 命中禁止项禁止项冲突 = false;
        bool 命中可改路由证据 = false;
        bool 命中仅能降级证据 = false;
        bool 命中收益恢复放行 = false;
        bool 命中继续提交级路由 = false;
        bool 命中重筹办级路由 = false;
        bool 命中待机级路由 = false;
        bool 命中收束级路由 = false;
        bool 细则允许继续提交级 = false;
        bool 细则允许重筹办级 = false;
        bool 细则允许待机级 = false;
        bool 细则允许收束级 = false;
        bool 命中绝对证据覆盖规则 = false;
        bool 命中长期根方向覆盖规则 = false;
        bool 命中收益恢复覆盖规则 = false;
        bool 命中已验证降根覆盖规则 = false;
        bool 命中目标反向仅降级规则 = false;
        bool 命中冲突仅降级规则 = false;
        bool 命中联合需求主导规则 = false;
        bool 命中联合禁止主导规则 = false;
        bool 命中联合收益恢复规则 = false;
        bool 命中联合僵持重筹办规则 = false;
        I64 联合需求主导阈值 = 0;
        I64 联合禁止主导阈值 = 0;
        I64 联合收益恢复裕量阈值 = 0;
        I64 联合僵持差值阈值 = 0;
        bool 局部收益不可覆盖长期根方向 = false;
        bool 收益覆盖禁止付出 = false;
        std::string 决定性需求候选摘要{};
        std::string 决定性方法禁止项摘要{};
        std::string 联合需求候选摘要{};
        std::string 联合方法禁止项摘要{};
        std::string 优先级链命中层{};
        std::string 冲突消解摘要{};
        std::string 逐类覆盖摘要{};
        std::string 逐类路由摘要{};
        std::string 逐类路由细则摘要{};
        std::string 多证据覆盖摘要{};
        std::string 多证据联合裁决摘要{};
        std::string 联合阈值细则摘要{};
        std::string 覆盖优先细则摘要{};
        std::string 根层判定摘要{};
        std::string 主派发判定二次特征摘要{};
        std::string 治理桥预判去向{};
        std::string 治理桥预判摘要{};
        std::string 治理桥功能域{};
        std::string 治理桥状态迁移{};
        std::string 治理桥总控结果{};
        std::string 治理桥去向{};
        std::string 治理桥摘要{};
        std::string 主派发摘要{};
        std::string 当前去向{};
        std::vector<结构_主派发动作快照> 派发动作列表{};
        std::vector<结构_队列裁决快照> 队列裁决列表{};
        std::vector<任务运行包::结构_一步治理任务包> 一步治理任务包集{};
        std::vector<结构_派发占用登记快照> 派发占用登记集{};
        std::vector<结构_派发回执快照> 派发回执集{};
    };

    struct 结构_回流结算快照 {
        std::uint64_t 包主键 = 0;
        std::uint64_t 任务根ID = 0;
        std::uint64_t 父任务主键 = 0;
        std::uint64_t 结果节点主键 = 0;
        bool 已封口 = false;
        bool 是否有效推进 = false;
        bool 建议进入收束 = false;
        bool 建议重试 = false;
        bool 需要父任务协调 = false;
        std::string 回流队列键{};
        std::string 父任务协调键{};
        std::string 父任务协调阶段{};
        std::string 父任务局部更新摘要{};
        std::string 结算结论摘要{};
        std::string 恢复建议摘要{};
        std::string 学习摘要{};
        std::string 故障摘要{};
        std::string 下一轮唤醒原因{};
        std::string 摘要{};
    };

    struct 结构_否定项快照 {
        时间戳 发生时间 = 0;
        std::uint64_t 目标任务主键 = 0;
        I64 风险等级 = 0;
        std::string 原因摘要{};
        std::string 摘要{};
    };

    struct 结构_学习触发快照 {
        时间戳 发生时间 = 0;
        std::uint64_t 锚点主键 = 0;
        std::uint64_t 来源任务主键 = 0;
        std::uint64_t 来源方法主键 = 0;
        I64 置信度 = 0;
        std::string 样本类别{};
        std::string 摘要{};
    };

    struct 结构_关键中间状态快照 {
        时间戳 发生时间 = 0;
        std::uint64_t 动作主体主键 = 0;
        std::uint64_t 变化主体主键 = 0;
        std::uint64_t 特征主键 = 0;
        bool 已恢复 = false;
        std::string 动作语义键{};
        std::string 特征类型摘要{};
        std::string 特征沉淀键{};
        std::string 变化摘要{};
        std::string 来源消息类型{};
        std::string 摘要{};
    };

    struct 结构_关键中间状态分组快照 {
        std::string 特征沉淀键{};
        std::string 特征类型摘要{};
        std::string 来源消息类型{};
        std::size_t 条目数 = 0;
        std::size_t 未恢复数 = 0;
        时间戳 最近发生时间 = 0;
        std::string 最近动作语义键{};
        std::string 最近变化摘要{};
        std::string 摘要{};
    };

    struct 结构_双值结算账快照 {
        bool 已生成 = false;
        I64 直接服务净变化 = 0;
        I64 准备性服务净变化 = 0;
        I64 服务净变化 = 0;
        I64 物理安全净变化 = 0;
        I64 风险安全净变化 = 0;
        I64 安全净变化 = 0;
        std::string 摘要{};
    };

    struct 结构_学习恢复接口快照 {
        bool 已生成 = false;
        bool 应触发学习 = false;
        bool 应申请重试恢复 = false;
        bool 应申请收束恢复 = false;
        bool 应申请低频整理 = false;
        std::size_t 学习触发数 = 0;
        std::string 恢复请求摘要{};
        std::string 整理请求摘要{};
        std::string 摘要{};
        struct 结构_恢复请求分组快照 {
            std::string 请求类型{};
            bool 已触发 = false;
            std::size_t 请求数 = 0;
            std::uint64_t 首目标任务主键 = 0;
            std::uint64_t 首关联包主键 = 0;
            std::string 唤醒原因{};
            std::string 摘要{};
        };
        std::vector<结构_恢复请求分组快照> 恢复请求列表{};
    };

    struct 结构_结算治理输出快照 {
        bool 已生成 = false;
        bool 是否有效推进 = false;
        bool 已执行固定机制 = false;
        I64 服务净变化 = 0;
        I64 安全净变化 = 0;
        std::string 固定机制摘要{};
        std::string 下一轮唤醒原因{};
        结构_双值结算账快照 双值结算账{};
        结构_学习恢复接口快照 学习恢复接口{};
        std::string 摘要{};
        std::vector<结构_回流结算快照> 回流结算列表{};
        std::vector<结构_否定项快照> 否定项列表{};
        std::vector<结构_学习触发快照> 学习触发列表{};
        std::vector<结构_关键中间状态快照> 关键中间状态列表{};
        std::vector<结构_关键中间状态分组快照> 关键中间状态分组列表{};
    };

public:
    自我线程类();
    ~自我线程类();

    自我线程类(const 自我线程类&) = delete;
    自我线程类& operator=(const 自我线程类&) = delete;
    自我线程类(自我线程类&&) = delete;
    自我线程类& operator=(自我线程类&&) = delete;

    bool 自我初始化(const std::string& 调用点 = "自我线程类::自我初始化");
    bool 启动(const std::string& 调用点 = "自我线程类::启动");
    void 请求停止(const std::string& 调用点 = "自我线程类::请求停止");
    void 等待停止(const std::string& 调用点 = "自我线程类::等待停止");
    void 停止(const std::string& 调用点 = "自我线程类::停止");

    bool 是否运行中() const noexcept;
    bool 是否初始化完成() const noexcept;
    bool 是否健康运行() const noexcept;
    bool 是否首轮运行已完成() const noexcept;
    bool 是否本次启动来自故障恢复() const noexcept;
    枚举_线程生命周期状态 读取生命周期状态() const noexcept;
    枚举_自我线程运行阶段 读取最近运行阶段() const noexcept;
    枚举_自我线程最终去向 读取最近最终去向() const noexcept;
    std::uint64_t 读取Tick计数() const noexcept;
    std::uint64_t 读取累计故障次数() const noexcept;
    std::uint64_t 读取累计恢复次数() const noexcept;
    时间戳 读取最近Tick时间() const noexcept;
    时间戳 读取上次故障时间() const noexcept;
    std::string 读取上次故障摘要() const;
    std::string 读取最近恢复摘要() const;
    std::string 读取最近故障摘要() const;
    std::string 读取最近运行摘要() const;
    结构_需求与权重治理输出快照 读取最近需求与权重治理输出快照() const;
    结构_主派发输出快照 读取最近主派发输出快照() const;
    结构_队列治理输出快照 读取最近队列治理输出快照() const;
    结构_结算治理输出快照 读取最近结算治理输出快照() const;
    结构_双值结算账快照 读取最近双值结算账快照() const;
    结构_学习恢复接口快照 读取最近学习恢复接口快照() const;
    std::vector<结构_学习恢复接口快照::结构_恢复请求分组快照> 读取最近恢复请求分组快照() const;
    std::vector<结构_回流结算快照> 读取最近回流结算列表快照() const;
    std::vector<结构_关键中间状态分组快照> 读取最近关键中间状态分组快照() const;
    结构_自我线程配置& 配置() noexcept;
    const 结构_自我线程配置& 配置() const noexcept;

private:
    enum class 枚举_主派发动作类型 : std::uint8_t {
        未定义 = 0,
        实例化任务入口 = 1,
        推进任务管理工作线程 = 2,
        进入等待队列 = 3,
        进入收束停止队列 = 4,
        回到重筹办 = 5,
        进入回流队列 = 6,
    };

    enum class 枚举_治理队列类型 : std::uint8_t {
        未定义 = 0,
        可运行 = 1,
        等待 = 2,
        回流 = 3,
        收束停止 = 4,
        重筹办 = 5,
        工作线程 = 6,
    };

    enum class 枚举_队列成员来源分类 : std::uint8_t {
        未定义 = 0,
        结果回流 = 1,
        任务入口实例化 = 2,
        工作线程推进 = 3,
        等待裁决 = 4,
        收束停止裁决 = 5,
        重筹办裁决 = 6,
        回流裁决 = 7,
    };

    struct 结构_根层判定结果 {
        bool 已生成 = false;
        枚举_任务管理根层重判结果 根层重判结果 = 枚举_任务管理根层重判结果::未定义;
        枚举_任务管理执行前门控结果 执行前门控结果 = 枚举_任务管理执行前门控结果::未定义;
        枚举_自我线程最终去向 当前最终去向 = 枚举_自我线程最终去向::未定义;
        std::uintptr_t 主导需求节点 = 0;
        std::uintptr_t 主导任务节点 = 0;
        I64 最高优先级候选 = 0;
        std::size_t 需求候选数 = 0;
        std::size_t 方法禁止项数 = 0;
        std::size_t 绝对禁止项数 = 0;
        std::size_t 已验证降根禁止项数 = 0;
        std::size_t 目标反向禁止项数 = 0;
        std::size_t 负区间禁止项数 = 0;
        I64 需求联合压力值 = 0;
        I64 禁止联合压力值 = 0;
        I64 短期收益估计值 = 0;
        I64 根方向收益估计值 = 0;
        I64 风险付出值 = 0;
        I64 恢复付出值 = 0;
        I64 收益估计值 = 0;
        I64 禁止付出值 = 0;
        bool 命中需求升格建议 = false;
        bool 命中新任务入口建议 = false;
        bool 命中禁止项重筹办建议 = false;
        bool 命中绝对禁止项 = false;
        bool 命中已验证降根禁止项 = false;
        bool 命中目标反向禁止项 = false;
        bool 命中方法硬阻断 = false;
        bool 允许任务入口实例化 = false;
        bool 已生成治理桥预判 = false;
        bool 治理桥允许进入执行链 = false;
        bool 治理桥要求触发学习 = false;
        bool 治理桥要求进入等待链 = false;
        bool 治理桥要求停止 = false;
        bool 命中绝对边界优先层 = false;
        bool 命中长期根方向否决层 = false;
        bool 命中收益放行层 = false;
        bool 命中禁止降级层 = false;
        bool 命中常规放行层 = false;
        bool 命中需求需求冲突 = false;
        bool 命中需求禁止项冲突 = false;
        bool 命中禁止项禁止项冲突 = false;
        bool 命中可改路由证据 = false;
        bool 命中仅能降级证据 = false;
        bool 命中收益恢复放行 = false;
        bool 命中继续提交级路由 = false;
        bool 命中重筹办级路由 = false;
        bool 命中待机级路由 = false;
        bool 命中收束级路由 = false;
        bool 细则允许继续提交级 = false;
        bool 细则允许重筹办级 = false;
        bool 细则允许待机级 = false;
        bool 细则允许收束级 = false;
        bool 命中绝对证据覆盖规则 = false;
        bool 命中长期根方向覆盖规则 = false;
        bool 命中收益恢复覆盖规则 = false;
        bool 命中已验证降根覆盖规则 = false;
        bool 命中目标反向仅降级规则 = false;
        bool 命中冲突仅降级规则 = false;
        bool 命中联合需求主导规则 = false;
        bool 命中联合禁止主导规则 = false;
        bool 命中联合收益恢复规则 = false;
        bool 命中联合僵持重筹办规则 = false;
        I64 联合需求主导阈值 = 0;
        I64 联合禁止主导阈值 = 0;
        I64 联合收益恢复裕量阈值 = 0;
        I64 联合僵持差值阈值 = 0;
        bool 局部收益不可覆盖长期根方向 = false;
        bool 收益覆盖禁止付出 = false;
        std::string 决定性需求候选摘要{};
        std::string 决定性方法禁止项摘要{};
        std::string 联合需求候选摘要{};
        std::string 联合方法禁止项摘要{};
        std::string 优先级链命中层{};
        std::string 冲突消解摘要{};
        std::string 逐类覆盖摘要{};
        std::string 逐类路由摘要{};
        std::string 逐类路由细则摘要{};
        std::string 多证据覆盖摘要{};
        std::string 多证据联合裁决摘要{};
        std::string 联合阈值细则摘要{};
        std::string 覆盖优先细则摘要{};
        std::string 主派发判定二次特征摘要{};
        std::string 治理桥预判去向{};
        std::string 治理桥预判摘要{};
        std::string 治理桥去向{};
        std::string 治理桥摘要{};
        std::string 上层反馈摘要{};
        std::string 摘要{};
    };

    struct 结构_主派发动作 {
        bool 已生成 = false;
        枚举_主派发动作类型 动作类型 = 枚举_主派发动作类型::未定义;
        std::uint64_t 任务根ID = 0;
        std::uintptr_t 主导任务节点 = 0;
        bool 允许并发 = true;
        std::uint32_t 并发组 = 0;
        std::string 目标队列{};
        std::string 摘要{};
    };

    struct 结构_队列裁决 {
        bool 已生成 = false;
        枚举_治理队列类型 队列类型 = 枚举_治理队列类型::未定义;
        bool 已命中 = false;
        I64 变更数量 = 0;
        std::string 队列名称{};
        std::string 裁决{};
        std::string 摘要{};
    };

    struct 结构_派发回执 {
        bool 已生成 = false;
        std::size_t 动作序号 = 0;
        枚举_主派发动作类型 动作类型 = 枚举_主派发动作类型::未定义;
        std::uint64_t 包主键 = 0;
        std::uint64_t 任务根ID = 0;
        bool 已生成任务包 = false;
        bool 已封口任务包 = false;
        bool 已实际派发 = false;
        bool 已形成回流 = false;
        std::string 错误摘要{};
        std::string 摘要{};
    };

    struct 结构_派发占用登记 {
        bool 已生成 = false;
        std::size_t 动作序号 = 0;
        枚举_主派发动作类型 动作类型 = 枚举_主派发动作类型::未定义;
        枚举_治理队列类型 占用队列类型 = 枚举_治理队列类型::未定义;
        std::uint64_t 包主键 = 0;
        std::uint64_t 任务根ID = 0;
        std::uintptr_t 主导任务节点 = 0;
        bool 已登记占用 = false;
        bool 已下发控制投影 = false;
        bool 已释放占用 = false;
        std::string 占用对象键{};
        std::string 控制投影摘要{};
        std::string 摘要{};
    };

    struct 结构_主派发决议 {
        bool 已生成 = false;
        结构_根层判定结果 根层判定{};
        自我线程消息批次执行器::枚举_批次执行去向 批次执行去向 =
            自我线程消息批次执行器::枚举_批次执行去向::继续提交;
        枚举_任务管理根层重判结果 根层重判结果 = 枚举_任务管理根层重判结果::未定义;
        枚举_任务管理执行前门控结果 执行前门控结果 = 枚举_任务管理执行前门控结果::未定义;
        std::uintptr_t 主导需求节点 = 0;
        std::uintptr_t 主导任务节点 = 0;
        bool 允许任务入口实例化 = false;
        bool 允许工作线程推进 = false;
        std::vector<结构_主派发动作> 派发动作列表{};
        std::vector<结构_队列裁决> 队列裁决列表{};
        std::vector<任务运行包::结构_一步治理任务包> 一步治理任务包集{};
        std::vector<结构_派发占用登记> 派发占用登记集{};
        std::vector<结构_派发回执> 派发回执集{};
        std::string 可运行队列裁决{};
        std::string 等待队列裁决{};
        std::string 回流队列裁决{};
        std::string 收束停止队列裁决{};
        std::string 任务管理当前去向{};
        std::string 摘要{};
    };

    struct 结构_队列治理对象 {
        struct 结构_成员对象 {
            std::string 成员稳定键{};
            std::uint64_t 任务根ID = 0;
            bool 当前在队列 = false;
            时间戳 首次入队时间 = 0;
            时间戳 最近入队时间 = 0;
            时间戳 最近出队时间 = 0;
            枚举_队列成员来源分类 来源分类 = 枚举_队列成员来源分类::未定义;
            std::string 来源动作{};
            std::string 来源摘要{};
            std::string 最近事件摘要{};
        };

        bool 已初始化 = false;
        枚举_治理队列类型 队列类型 = 枚举_治理队列类型::未定义;
        std::string 队列名称{};
        时间戳 最近更新时间 = 0;
        std::uint64_t 最近任务根ID = 0;
        bool 当前已命中 = false;
        bool 当前活跃 = false;
        I64 本轮变更数量 = 0;
        std::uint64_t 累计命中次数 = 0;
        I64 累计变更数量 = 0;
        std::uint64_t 连续活跃轮数 = 0;
        std::uint64_t 累计活跃轮数 = 0;
        std::uint64_t 累计清空次数 = 0;
        std::size_t 本轮动作数 = 0;
        std::size_t 当前可见数量 = 0;
        std::size_t 历史峰值数量 = 0;
        std::string 数量口径{};
        时间戳 最近激活时间 = 0;
        时间戳 最近清空时间 = 0;
        std::vector<std::uint64_t> 当前成员任务根ID集{};
        std::vector<std::uint64_t> 最近移出任务根ID集{};
        std::string 成员来源分类摘要{};
        std::string 成员来源摘要{};
        std::vector<结构_成员对象> 成员对象列表{};
        std::vector<结构_队列成员事件快照> 最近成员事件列表{};
        std::string 最近裁决{};
        std::string 最近动作摘要{};
        std::string 最近摘要{};
    };

    struct 结构_结果路由动作 {
        bool 已生成 = false;
        bool 已完成全局判断 = false;
        bool 已路由父任务 = false;
        bool 已补记父任务影响 = false;
        bool 已加入回流队列 = false;
        bool 是否最终结果 = false;
        bool 是否需要父任务处理 = false;
        bool 是否形成否定项候选 = false;
        std::uint64_t 任务根ID = 0;
        std::uint64_t 回流结算包主键 = 0;
        std::uintptr_t 父任务节点 = 0;
        std::uintptr_t 子结果节点 = 0;
        std::string 父任务协调键{};
        std::string 父任务协调阶段{};
        std::string 父任务局部更新摘要{};
        std::string 摘要{};
    };

    struct 结构_回流结算草稿 {
        bool 已生成 = false;
        bool 已封口 = false;
        std::uint64_t 包主键 = 0;
        std::uint64_t 任务根ID = 0;
        std::uint64_t 父任务主键 = 0;
        std::uint64_t 结果节点主键 = 0;
        std::uintptr_t 父任务节点 = 0;
        std::uintptr_t 结果节点 = 0;
        bool 是否有效推进 = false;
        bool 建议进入收束 = false;
        bool 建议重试 = false;
        bool 需要父任务协调 = false;
        std::string 回流队列键{};
        std::string 父任务协调键{};
        std::string 父任务协调阶段{};
        std::string 父任务局部更新摘要{};
        std::string 恢复建议摘要{};
        std::string 下一轮唤醒原因{};
        std::string 结算结论摘要{};
        std::string 学习摘要{};
        std::string 故障摘要{};
        std::string 摘要{};
    };

    struct 结构_否定项候选 {
        时间戳 发生时间 = 0;
        std::uint64_t 目标任务主键 = 0;
        std::uintptr_t 相关结果节点 = 0;
        I64 风险等级 = 0;
        std::string 原因摘要{};
        std::string 摘要{};
    };

    struct 结构_学习触发项 {
        时间戳 发生时间 = 0;
        std::uint64_t 锚点主键 = 0;
        std::uint64_t 来源任务主键 = 0;
        std::uint64_t 来源方法主键 = 0;
        I64 置信度 = 0;
        std::string 样本类别{};
        std::string 摘要{};
    };

    struct 结构_关键中间状态沉淀项 {
        时间戳 发生时间 = 0;
        std::uint64_t 动作主体主键 = 0;
        std::uint64_t 变化主体主键 = 0;
        std::uint64_t 特征主键 = 0;
        bool 已恢复 = false;
        std::string 动作语义键{};
        std::string 特征类型摘要{};
        std::string 特征沉淀键{};
        std::string 变化摘要{};
        std::string 来源消息类型{};
        std::string 摘要{};
    };

    struct 结构_本轮结算封口 {
        bool 已生成 = false;
        bool 是否有效推进 = false;
        bool 已执行固定机制 = false;
        bool 已生成否定项候选 = false;
        bool 已触发学习 = false;
        bool 已生成故障收口摘要 = false;
        std::size_t 新增否定项候选数 = 0;
        std::size_t 新增学习触发数 = 0;
        std::size_t 新增关键中间状态沉淀数 = 0;
        I64 服务净变化 = 0;
        I64 安全净变化 = 0;
        std::string 固定机制摘要{};
        std::string 否定项摘要{};
        std::string 学习摘要{};
        std::string 故障收口摘要{};
        std::string 下一轮唤醒原因{};
        std::string 摘要{};
    };

    struct 结构_主循环骨架上下文 {
        时间戳 当前时间 = 0;
        结构_循环结果 结果{};
        自我线程消息协议::结构_治理消息 已消费消息{};
        std::deque<结构_治理消息> 本轮冻结消息{};
        自我线程消息批次执行器::结构_治理消息批次 本轮治理消息批次{};
        自我线程消息处理器::结构_需求分支刷新范围 需求树变更刷新范围{};
        自我线程消息批次执行器::结构_线程裁决结果 线程裁决结果{};
        结构_任务局部运行态 任务局部运行态{};
        结构_任务根节点 本轮任务根节点{};
        结构_任务控制态 本轮任务控制态{};
        结构_主派发决议 本轮主派发决议{};
        结构_结果路由动作 本轮结果路由动作{};
        结构_本轮结算封口 本轮结算封口{};
        任务管理工作线程::结构_工作线程桥接结果 桥接结果{};
        I64 本轮服务净变化 = 0;
        I64 本轮安全净变化 = 0;
        I64 本轮直接服务净变化 = 0;
        I64 本轮准备性服务净变化 = 0;
        I64 本轮物理安全净变化 = 0;
        I64 本轮风险安全净变化 = 0;
        I64 本轮最高优先级候选 = 0;
        需求类::节点类* 本轮主导需求候选 = nullptr;
        std::uintptr_t 本轮主导任务候选指针 = 0;
        bool 命中治理邮箱事件 = false;
        bool 已应用需求树变更 = false;
        bool 本轮有需求与权重输入 = false;
        bool 本轮已完成需求与权重更新 = false;
    };

    friend bool 投递治理外部反馈事件(
        const std::string& 摘要,
        const std::string& 来源主观察特征);
    friend bool 上报线程状态变化(
        const 自我线程消息协议::结构_线程状态变化消息段& 消息段,
        const 结构_线程存在初始化参数* 线程存在初始化,
        const std::string& 调用点);
    friend bool 切换线程状态并上报(
        const 结构_线程状态切换上报参数& 参数,
        const std::string& 调用点);
    friend bool 上报任务管理上行消息(
        const 结构_任务管理上行消息& 消息,
        const std::string& 调用点);

    void 主循环_();
    结构_循环结果 执行主循环一轮_(时间戳 now);
    void 步骤_冻结本轮消息批次_(结构_主循环骨架上下文* 上下文);
    void 步骤_规范化消息并落一次特征账_(结构_主循环骨架上下文* 上下文);
    void 步骤_重算受影响二次特征_(结构_主循环骨架上下文* 上下文);
    void 步骤_刷新线程上下文并生成治理帧_(结构_主循环骨架上下文* 上下文);
    void 步骤_整理需求并做根层重判_(结构_主循环骨架上下文* 上下文);
    void 步骤_生成主派发判定二次特征_(结构_主循环骨架上下文* 上下文);
    void 步骤_生成并执行主派发决议_(结构_主循环骨架上下文* 上下文);
    void 步骤_路由结果并协调父任务_(结构_主循环骨架上下文* 上下文);
    void 步骤_本轮结算学习触发故障收口_(结构_主循环骨架上下文* 上下文);
    bool 投递治理消息(const 结构_治理消息& 消息);
    void 刷新初始化标记_已加锁() noexcept;
    void 刷新治理恢复事件镜像_已加锁() noexcept;
    void 记录阶段事件_已加锁(
        时间戳 now,
        枚举_自我线程运行阶段 阶段,
        const std::string& 摘要);
    void 故障收口_已加锁(时间戳 now, const std::string& 摘要);

private:
    mutable std::mutex 状态锁_{};
    std::condition_variable 唤醒条件_{};
    std::thread 工作线程_{};
    std::atomic_bool 停止请求_{false};
    std::atomic_bool 运行中_{false};

    结构_自我线程配置 配置_{};
    枚举_线程生命周期状态 生命周期_ = 枚举_线程生命周期状态::未启动;
    bool 初始化完成_ = false;
    bool 健康运行_ = false;
    bool 首轮运行已完成_ = false;
    bool 本次启动来自故障恢复_ = false;
    bool 待故障恢复启动_ = false;
    std::uint64_t Tick计数_ = 0;
    std::uint64_t 治理事件序号_ = 0;
    std::uint64_t 累计故障次数_ = 0;
    std::uint64_t 累计恢复次数_ = 0;
    时间戳 最近Tick时间_ = 0;
    时间戳 上次故障时间_ = 0;
    std::string 上次故障摘要_{};
    std::string 最近恢复摘要_{};
    std::string 最近故障摘要_{};
    std::deque<结构_治理消息> 治理mailbox_{};
    std::deque<结构_治理消息> 本轮冻结治理批次_{};
    std::unordered_map<std::string, 自我线程消息协议::结构_执行尝试汇总> 执行尝试汇总表_{};
    结构_需求与权重治理输出快照 最近需求与权重治理输出快照_{};
    结构_主派发输出快照 最近主派发输出快照_{};
    结构_队列治理输出快照 最近队列治理输出快照_{};
    结构_结算治理输出快照 最近结算治理输出快照_{};
    std::vector<结构_队列治理对象> 队列治理对象集_{};
    std::deque<结构_结果路由动作> 回流队列_{};
    std::deque<结构_回流结算草稿> 回流结算包草稿池_{};
    std::deque<结构_否定项候选> 否定项候选池_{};
    std::deque<结构_学习触发项> 学习候选池_{};
    std::deque<结构_关键中间状态沉淀项> 关键中间状态池_{};
    std::vector<结构_需求列表项> 服务优先需求列表_{};
    std::vector<结构_需求列表项> 安全优先需求列表_{};
    std::vector<结构_自我运行阶段事件> 运行阶段事件_{};
    结构_循环结果 最近循环结果_{};
};

export 自我线程类& 获取全局自我线程() noexcept;
export bool 初始化自我环境(const std::string& 调用点 = "初始化自我环境");
export bool 启动自我线程(const std::string& 调用点 = "启动自我线程");
export void 停止自我线程(const std::string& 调用点 = "停止自我线程");
export bool 上报线程状态变化(
    const 自我线程消息协议::结构_线程状态变化消息段& 消息段,
    const 结构_线程存在初始化参数* 线程存在初始化 = nullptr,
    const std::string& 调用点 = "上报线程状态变化");
export bool 切换线程状态并上报(
    const 结构_线程状态切换上报参数& 参数,
    const std::string& 调用点 = "切换线程状态并上报");
export bool 上报任务管理上行消息(
    const 结构_任务管理上行消息& 消息,
    const std::string& 调用点 = "上报任务管理上行消息");
export bool 投递治理外部反馈事件(
    const std::string& 摘要,
    const std::string& 来源主观察特征 = "外部反馈/人工注入");
