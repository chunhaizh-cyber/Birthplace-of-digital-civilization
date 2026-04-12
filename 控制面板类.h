#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "自我线程类.h"

struct 结构_控制面板树节点 {
    std::uintptr_t 节点指针 = 0;
    std::string 文本{};
    std::vector<结构_控制面板树节点> 子项{};
};

struct 结构_控制面板列表项 {
    std::uintptr_t 节点指针 = 0;
    std::string 标题{};
    std::string 摘要{};
};

struct 结构_控制面板快照 {
    bool 世界树已初始化 = false;
    bool 自我已初始化 = false;
    bool 自我线程已初始化 = false;
    bool 自我线程运行中 = false;
    bool 自我线程健康运行 = false;
    bool 自我线程首轮运行已完成 = false;
    bool 自我线程本次启动来自故障恢复 = false;
    bool 自我线程解封前验收通过 = false;
    bool 自我线程重外部输入保持封闭 = true;
    bool 自我存在已建立 = false;
    bool 自我内部世界已建立 = false;
    bool 自我待机状态 = false;
    枚举_线程生命周期状态 自我线程生命周期 = 枚举_线程生命周期状态::未启动;

    std::size_t 世界根子节点数 = 0;
    std::size_t 基础信息节点数 = 0;
    std::size_t 世界树活跃节点数 = 0;
    std::size_t 场景数 = 0;
    std::size_t 存在数 = 0;
    std::size_t 特征数 = 0;
    std::size_t 状态数 = 0;
    std::size_t 动态数 = 0;
    std::size_t 因果实例数 = 0;
    std::size_t 因果模板数 = 0;
    std::size_t 先天动作动态数 = 0;
    std::size_t 先天动作因果数 = 0;
    std::size_t 需求数 = 0;
    std::size_t 任务数 = 0;
    std::size_t 方法数 = 0;
    std::size_t 学习任务总数 = 0;
    std::size_t 学习任务活跃数 = 0;
    std::size_t 学习任务挂起数 = 0;
    std::size_t 学习任务可调度数 = 0;
    std::size_t 学习任务执行中数 = 0;
    std::size_t 学习任务已完成数 = 0;
    std::size_t 学习就绪队列数 = 0;
    std::size_t 学习等待表数 = 0;
    std::size_t 历史宿主残留节点数 = 0;
    std::size_t 历史宿主残留特征数 = 0;
    std::size_t 历史宿主残留状态数 = 0;
    std::size_t 历史宿主残留动态数 = 0;
    std::size_t 历史宿主残留因果数 = 0;
    std::size_t 历史宿主残留提示数 = 0;
    std::size_t 历史宿主残留告警数 = 0;
    std::size_t 历史宿主残留阻断数 = 0;
    bool 历史宿主残留需要阻断 = false;

    std::int64_t 自我安全值 = 0;
    std::int64_t 自我服务值 = 0;
    std::int64_t 自我物理安全值 = 0;
    std::int64_t 自我风险安全值 = 0;
    std::int64_t 自我待学习方法数量 = 0;
    std::int64_t 自我安全根方向差值 = 0;
    std::int64_t 自我时序正向步长 = 0;
    std::int64_t 自我时序反向步长 = 0;
    std::int64_t 自我服务时序衰减步长 = 0;
    std::uint64_t 自我Tick计数 = 0;
    std::uint64_t 自我线程累计故障次数 = 0;
    std::uint64_t 自我线程累计恢复次数 = 0;
    时间戳 自我最近Tick时间 = 0;
    时间戳 自我线程上次故障时间 = 0;

    std::uintptr_t 自我存在指针 = 0;
    std::uintptr_t 自我现实场景指针 = 0;
    std::uintptr_t 自我内部世界指针 = 0;
    std::uintptr_t 当前主需求指针 = 0;
    std::uintptr_t 当前主任务指针 = 0;
    std::uintptr_t 当前主方法指针 = 0;
    std::uintptr_t 任务管理当前步骤指针 = 0;
    std::uintptr_t 任务管理最近结果指针 = 0;

    std::string 自我现实场景名称{};
    std::string 自我内部世界名称{};
    std::string 任务管理根层重判结果{};
    std::string 任务管理执行前门控结果{};
    std::string 任务管理上层反馈摘要{};
    std::string 任务管理锚点裁决摘要{};
    std::string 任务管理当前缺口{};
    std::string 任务管理当前去向{};
    std::string 任务管理总控结果{};
    std::string 任务管理反馈类型{};
    std::string 任务管理反馈摘要{};
    std::string 任务管理方法来源{};
    std::string 任务管理步骤位{};
    std::string 任务管理步骤语义{};
    std::string 任务管理当前步骤标题{};
    std::string 任务管理最近结果标题{};
    std::string 任务管理目标结果摘要{};
    std::string 任务管理恢复点类型{};
    std::string 任务管理恢复投影摘要{};
    bool 任务管理恢复存在待消费学习回流 = false;
    bool 任务管理恢复存在待消费外部反馈 = false;
    std::size_t 治理mailbox待消费数 = 0;
    std::uint64_t 治理mailbox最老等待微秒 = 0;
    std::size_t 治理mailbox可恢复事件数 = 0;
    std::string 主循环归并来源{};
    std::string 主循环归并摘要{};
    std::string 主消息心跳车道状态{};
    std::string 任务治理车道状态{};
    std::string 学习车道状态{};
    std::string 延迟事实车道状态{};
    std::string 根任务结构调整车道状态{};
    std::string 运行时车道摘要{};
    std::string 治理mailbox摘要{};
    std::string 治理mailbox类型计数摘要{};
    std::string 治理mailbox最老等待摘要{};
    std::string 治理mailbox最近拦截摘要{};
    std::string 治理mailbox待恢复摘要{};
    std::string 最近消费治理事件摘要{};
    std::string 最近消费治理事件优先级{};
    std::string 最近消费治理事件幂等键{};
    std::uint32_t 最近消费治理事件重放代数 = 0;
    std::uint32_t 最近消费治理事件消费次数 = 0;
    std::string 最近消费治理事件结果{};
    std::string 自我摘要{};
    std::string 自我线程摘要{};
    std::string 运行时摘要{};
    std::string 任务管理专项摘要{};
    std::string 任务管理上下文摘要{};
    std::string 任务管理输出摘要{};
    std::string 任务管理治理实例堆摘要{};
    std::string 任务管理恢复快照摘要{};
    std::string 动态来源摘要{};
    std::string 动态分类摘要{};
    std::string 动态方法摘要{};
    std::string 动态任务摘要{};
    std::string 动态场景摘要{};
    std::string 动态树摘要{};
    std::string 学习账本摘要{};
    std::string 历史宿主残留严重级别{};
    std::string 学习调度摘要{};
    std::string 学习专项摘要{};
    std::string 学习当前阶段{};
    std::string 学习当前状态{};
    std::string 学习当前影响面{};
    std::string 学习当前任务标题{};
    std::string 学习当前方法标题{};
    std::string 学习最近摘要{};
    std::string 学习最近失败摘要{};
    std::string 学习最近回流摘要{};
    std::string 学习最近回放摘要{};
    std::string 自我线程动作最近摘要{};
    std::string 自我线程解封前验收摘要{};
    std::string 自我线程当前最终去向{};
    std::string 自我线程当前阶段{};
    std::string 自我线程上次故障摘要{};
    std::string 自我线程最近恢复摘要{};
    std::string 自我线程最近故障摘要{};

    std::vector<结构_控制面板列表项> 世界列表{};
    std::vector<结构_控制面板列表项> 场景列表{};
    std::vector<结构_控制面板列表项> 存在列表{};
    std::vector<结构_控制面板列表项> 需求列表{};
    std::vector<结构_控制面板列表项> 任务列表{};
    std::vector<结构_控制面板列表项> 方法列表{};
    std::vector<结构_控制面板列表项> 动态列表{};
    std::vector<结构_控制面板列表项> 动态来源列表{};
    std::vector<结构_控制面板列表项> 动态分类列表{};
    std::vector<结构_控制面板列表项> 动态方法列表{};
    std::vector<结构_控制面板列表项> 动态任务列表{};
    std::vector<结构_控制面板列表项> 动态场景列表{};
    std::vector<结构_控制面板列表项> 动态树列表{};
    std::vector<结构_控制面板列表项> 任务管理输入列表{};
    std::vector<结构_控制面板列表项> 任务管理输出列表{};
    std::vector<结构_控制面板列表项> 任务管理触发列表{};
    std::vector<结构_控制面板列表项> 学习任务列表{};
    std::vector<结构_控制面板列表项> 学习就绪列表{};
    std::vector<结构_控制面板列表项> 学习等待列表{};
    std::vector<结构_控制面板列表项> 先天动作动态列表{};
    std::vector<结构_控制面板列表项> 先天动作因果列表{};
    std::vector<结构_控制面板列表项> 历史宿主残留列表{};
    std::vector<std::string> 历史宿主残留阻断摘要{};
    std::vector<结构_自我运行阶段事件> 自我运行阶段事件{};

    结构_控制面板树节点 世界树根{};
    结构_控制面板树节点 需求树根{};
    结构_控制面板树节点 任务树根{};
    结构_控制面板树节点 方法树根{};
};

enum class 枚举_控制面板命令 {
    无 = 0,
    输出摘要 = 1,
    输出任务管理摘要 = 2,
    输出学习摘要 = 3,
    输出历史宿主残留摘要 = 4,
    归档历史宿主残留 = 5,
    清理历史宿主残留 = 6,
    生成HTML = 7,
    打开窗口 = 8,
};

结构_控制面板快照 读取控制面板快照(
    std::size_t 树深度上限 = 6,
    std::size_t 树广度上限 = 12);

std::string 渲染控制面板摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 树层数上限 = 4,
    std::size_t 列表预览上限 = 8);

std::string 渲染任务管理摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限 = 12);

std::string 渲染学习摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限 = 12);

std::string 渲染历史宿主残留摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限 = 12);

std::string 生成控制面板HTML(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限 = 12);

bool 归档历史宿主残留快照(
    std::filesystem::path* 输出路径 = nullptr,
    std::string* 摘要输出 = nullptr);

bool 清理历史宿主残留(std::string* 摘要输出 = nullptr);

bool 保存控制面板HTML(
    const std::filesystem::path& 输出路径,
    const 结构_控制面板快照& 快照);

bool 启动控制面板窗口() noexcept;
void 等待控制面板窗口关闭() noexcept;
int 获取控制面板启动诊断码() noexcept;

std::filesystem::path 默认控制面板HTML路径();

枚举_控制面板命令 解析控制面板命令行(int argc, char** argv) noexcept;
