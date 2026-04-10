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
    bool 自我存在已建立 = false;
    bool 自我内部世界已建立 = false;
    bool 自我待机状态 = false;
    枚举_线程生命周期状态 自我线程生命周期 = 枚举_线程生命周期状态::未启动;

    std::size_t 世界根子节点数 = 0;
    std::size_t 基础信息节点数 = 0;
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

    std::int64_t 自我安全值 = 0;
    std::int64_t 自我服务值 = 0;
    std::int64_t 自我物理安全值 = 0;
    std::int64_t 自我风险安全值 = 0;
    std::int64_t 自我安全根方向差值 = 0;
    std::int64_t 自我时序正向步长 = 0;
    std::int64_t 自我时序反向步长 = 0;
    std::int64_t 自我服务时序衰减步长 = 0;
    std::uint64_t 自我Tick计数 = 0;
    时间戳 自我最近Tick时间 = 0;

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
    std::string 任务管理当前缺口{};
    std::string 任务管理当前去向{};
    std::string 任务管理方法来源{};
    std::string 任务管理步骤位{};
    std::string 任务管理当前步骤标题{};
    std::string 任务管理最近结果标题{};
    std::string 任务管理目标结果摘要{};
    std::string 自我摘要{};
    std::string 自我线程摘要{};
    std::string 运行时摘要{};
    std::string 任务管理专项摘要{};
    std::string 任务管理上下文摘要{};
    std::string 任务管理输出摘要{};
    std::string 自我线程当前阶段{};
    std::string 自我线程最近故障摘要{};

    std::vector<结构_控制面板列表项> 世界列表{};
    std::vector<结构_控制面板列表项> 场景列表{};
    std::vector<结构_控制面板列表项> 存在列表{};
    std::vector<结构_控制面板列表项> 需求列表{};
    std::vector<结构_控制面板列表项> 任务列表{};
    std::vector<结构_控制面板列表项> 方法列表{};
    std::vector<结构_控制面板列表项> 任务管理输入列表{};
    std::vector<结构_控制面板列表项> 任务管理输出列表{};
    std::vector<结构_控制面板列表项> 任务管理触发列表{};
    std::vector<结构_控制面板列表项> 先天动作动态列表{};
    std::vector<结构_控制面板列表项> 先天动作因果列表{};
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
    生成HTML = 3,
    打开窗口 = 4,
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

std::string 生成控制面板HTML(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限 = 12);

bool 保存控制面板HTML(
    const std::filesystem::path& 输出路径,
    const 结构_控制面板快照& 快照);

bool 启动控制面板窗口() noexcept;
void 等待控制面板窗口关闭() noexcept;
int 获取控制面板启动诊断码() noexcept;

std::filesystem::path 默认控制面板HTML路径();

枚举_控制面板命令 解析控制面板命令行(int argc, char** argv) noexcept;
