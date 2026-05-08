module;

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

export module 控制面板类;

export {

struct 结构_控制面板树节点 {
    std::uintptr_t 节点指针 = 0;
    std::uintptr_t 附加参数 = 0;
    std::string 文本{};
    bool 默认展开 = false;
    bool 可延迟展开 = false;
    std::string 展开类型{};
    std::vector<结构_控制面板树节点> 子项{};
};

struct 结构_控制面板快照 {
    bool 世界树已初始化 = false;
    bool 自我已初始化 = false;
    bool 自我线程已初始化 = false;
    bool 自我线程运行中 = false;
    bool 自我线程健康运行 = false;
    bool 自我存在已建立 = false;
    bool 自我内部世界已建立 = false;
    bool 自我待机状态 = false;
    std::uint8_t 自我线程生命周期 = 0;

    std::size_t 基础信息节点数 = 0;
    std::size_t 场景数 = 0;
    std::size_t 存在数 = 0;
    std::size_t 特征数 = 0;
    std::size_t 状态数 = 0;
    std::size_t 动态数 = 0;
    std::size_t 因果模板数 = 0;
    std::size_t 因果证据动态样本数 = 0;
    std::size_t 需求数 = 0;
    std::size_t 任务数 = 0;
    std::size_t 任务头节点数 = 0;
    std::size_t 任务步骤节点数 = 0;
    std::size_t 任务其他节点数 = 0;
    std::size_t 任务状态未定义数 = 0;
    std::size_t 任务状态未启动数 = 0;
    std::size_t 任务状态运行中数 = 0;
    std::size_t 任务状态挂起数 = 0;
    std::size_t 任务状态完成数 = 0;
    std::size_t 任务状态失败数 = 0;
    std::size_t 任务状态取消数 = 0;
    std::size_t 任务状态超时数 = 0;
    std::size_t 任务状态就绪数 = 0;
    std::size_t 任务状态执行中数 = 0;
    std::size_t 任务状态筹办中数 = 0;
    std::size_t 任务状态排队中数 = 0;
    std::size_t 任务状态等待中数 = 0;
    std::size_t 任务状态无法执行数 = 0;
    std::size_t 任务状态待重筹办数 = 0;
    std::size_t 方法数 = 0;
    std::size_t 方法首节点数 = 0;
    std::size_t 方法条件无结果数 = 0;
    std::size_t 方法结果无条件数 = 0;
    std::size_t 方法条件结果不成对数 = 0;
    std::size_t 线程数 = 0;

    std::size_t 需求已满足数 = 0;
    std::size_t 需求状态已达未截止数 = 0;
    std::size_t 需求未满足数 = 0;

    std::int64_t 自我安全值 = 0;
    std::int64_t 自我服务值 = 0;
    std::int64_t 自我风险安全值 = 0;
    std::int64_t 自我待处理方法数量 = 0;
    std::int64_t 自我可用方法数量 = 0;
    std::uint64_t 自我Tick计数 = 0;
    std::uint64_t 自我线程累计故障次数 = 0;
    std::uint64_t 自我线程累计恢复次数 = 0;

    std::uintptr_t 自我存在指针 = 0;
    std::uintptr_t 当前主需求指针 = 0;
    std::uintptr_t 当前主任务指针 = 0;
    std::uintptr_t 当前主方法指针 = 0;

    std::string 自我存在标题{};
    std::string 当前主需求标题{};
    std::string 当前主任务标题{};
    std::string 当前主方法标题{};

    std::string 自我线程当前阶段{};
    std::string 自我线程当前最终去向{};
    std::string 自我线程最近运行摘要{};
    std::string 自我线程最近恢复摘要{};
    std::string 自我线程最近故障摘要{};

    bool 控制面板摘要线程已启动 = false;
    bool 控制面板摘要线程健康运行 = false;
    std::uint8_t 控制面板摘要线程生命周期 = 0;
    std::uint64_t 控制面板摘要快照序号 = 0;
    std::string 控制面板摘要线程摘要{};
    std::string 控制面板摘要_需求树{};
    std::string 控制面板摘要_任务树{};
    std::string 控制面板摘要_方法树{};
    std::string 控制面板摘要_运行事实{};
    std::string 控制面板摘要_自检{};

    bool 任务管理工作线程已启动 = false;
    bool 任务管理工作线程正在执行 = false;
    bool 任务管理工作线程已收到请求 = false;
    std::uint64_t 任务管理工作线程累计推进次数 = 0;
    std::uint64_t 任务管理工作线程当前排队数 = 0;
    std::string 任务管理工作线程最近因果链状态{};
    std::string 任务管理工作线程最近缺口归类{};
    std::uint64_t 任务管理工作线程最近特征变化数 = 0;
    std::string 任务管理最近功能域{};
    std::string 任务管理最近缺口类型{};
    std::string 任务管理最近下一步去向{};
    std::string 任务管理最近总控结果{};
    std::string 任务管理最近恢复摘要{};

    std::size_t 缺口需求总数 = 0;
    std::size_t 缺口需求活跃数 = 0;
    std::size_t 缺口需求可调度数 = 0;
    std::size_t 缺口恢复请求数 = 0;
    std::size_t 缺口因兜底切换进入就绪数 = 0;
    std::size_t 缺口因兜底切换进入采样数 = 0;
    bool 应触发缺口承接 = false;
    bool 缺口应申请重试恢复 = false;
    bool 缺口应申请收束恢复 = false;
    std::string 缺口当前阶段{};
    std::string 缺口当前状态{};
    std::string 缺口当前任务标题{};
    std::string 缺口当前方法标题{};
    std::string 缺口调度摘要{};
    std::string 缺口最近摘要{};
    std::string 缺口最近失败摘要{};
    std::string 缺口最近反馈摘要{};
    std::string 缺口最近回流摘要{};
    std::string 缺口固定机制观察摘要{};
    std::string 缺口首个兜底切换就绪摘要{};
    std::string 缺口首个兜底切换采样摘要{};

    结构_控制面板树节点 线程状态树根{};
    结构_控制面板树节点 世界树根{};
    结构_控制面板树节点 需求树根{};
    结构_控制面板树节点 需求列表树根{};
    结构_控制面板树节点 任务树根{};
    结构_控制面板树节点 方法树根{};
};

enum class 枚举_控制面板命令 {
    无 = 0,
    输出摘要 = 1,
    输出任务管理摘要 = 2,
    输出缺口摘要 = 3,
    生成HTML = 4,
    打开窗口 = 5,
};

结构_控制面板快照 读取控制面板快照(
    std::size_t 树深度上限 = 10,
    std::size_t 树广度上限 = 24);

std::string 渲染控制面板摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 树层数上限 = 4,
    std::size_t 列表预览上限 = 8);

std::string 渲染任务管理摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限 = 12);

std::string 渲染缺口摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限 = 12);

std::string 生成控制面板HTML(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限 = 12);

std::string 读取控制面板节点子项JSON(
    std::string_view 展开类型,
    std::uintptr_t 节点指针,
    std::size_t 树广度上限 = 16,
    std::uintptr_t 附加参数 = 0);

std::string 读取控制面板节点详情JSON(
    std::string_view 展开类型,
    std::uintptr_t 节点指针,
    std::size_t 树广度上限 = 16);

bool 保存控制面板HTML(
    const std::filesystem::path& 输出路径,
    const 结构_控制面板快照& 快照);

bool 启动控制面板窗口() noexcept;
void 等待控制面板窗口关闭() noexcept;
int 获取控制面板启动诊断码() noexcept;

std::filesystem::path 默认控制面板HTML路径();

枚举_控制面板命令 解析控制面板命令行(int argc, char** argv) noexcept;

} // export
