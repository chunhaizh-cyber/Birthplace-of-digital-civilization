module;

// 文件头部规则注释模块：
// 1. 本接口只暴露 SQL Server ADO 访问和控制面板显示镜像初始化能力。
// 2. SQL 写入只作为控制面板显示镜像，不得作为业务权威源或恢复源。
// 3. 写命令必须受 预处理开关变量.h 中 SQL 控制面板同步写入开关控制。

#include <string>
#include <string_view>
#include <cstdint>
#include <optional>
#include <vector>

export module 数据库ADO模块;

export struct 结构_ADO查询结果 {
    std::vector<std::vector<std::string>> 行集{};
};

export struct 结构_ADO字段恢复比对结果 {
    bool 匹配 = false;
    std::size_t 预期行数 = 0;
    std::size_t 恢复行数 = 0;
    std::string 首个差异{};
};

export enum class 枚举_SQL控制面板显示域 : std::uint8_t {
    任务 = 0,
    需求,
    方法,
    世界,
    动态
};

export struct 结构_SQL控制面板显示项 {
    枚举_SQL控制面板显示域 显示域 = 枚举_SQL控制面板显示域::任务;
    std::string 主键{};
    std::string 父主键{};
    std::optional<std::int64_t> 状态值{};
    std::string 状态文本{};
    std::optional<std::int64_t> 满足关系掩码{};
    std::optional<bool> 已满足{};
    std::string 类型文本{};
    std::string 显示摘要{};
};

// 功能：生成 SQL Server Windows 身份认证 ADO 连接串。
export std::string 生成SQLServerWindows认证ADO连接串(
    std::string_view 服务器 = R"(.\SQLEXPRESS)",
    std::string_view 数据库 = "鱼巢投影库");

// 功能：通过 ADO 执行 SQL 查询并返回文本化结果集。
export bool 执行ADO查询(
    const std::string& 连接串,
    const std::string& SQL,
    结构_ADO查询结果& 结果,
    std::string& 错误);

// 功能：从 SQL Server 恢复字段行集并与原始字段值逐项比较。
export bool 执行ADO字段恢复比对(
    const std::string& 连接串,
    const std::string& SQL,
    const std::vector<std::vector<std::string>>& 预期行集,
    结构_ADO字段恢复比对结果& 比对结果,
    std::string& 错误);

// 功能：通过 ADO 执行不返回结果集的 SQL 命令。
export bool 执行ADO命令(
    const std::string& 连接串,
    const std::string& SQL,
    std::string& 错误,
    int 命令超时秒 = 30);

// 功能：初始化控制面板运行态 SQL 显示镜像，清空本次运行态表并写入运行批次。
export bool 初始化SQL控制面板运行态投影(
    std::string_view 来源标记,
    std::string& 运行ID,
    std::string& 错误);

// 功能：同步新增或更新一个控制面板运行态显示项。
export bool 同步写入SQL控制面板显示项(
    const 结构_SQL控制面板显示项& 显示项,
    std::string& 错误);

// 功能：同步删除一个控制面板运行态显示项。
export bool 同步删除SQL控制面板显示项(
    枚举_SQL控制面板显示域 显示域,
    std::string_view 主键,
    std::string_view 显示摘要,
    std::string& 错误);

// 功能：追加一条控制面板运行态事件流水。
export bool 追加SQL控制面板事件流水(
    枚举_SQL控制面板显示域 显示域,
    std::string_view 操作,
    std::string_view 对象主键,
    std::string_view 显示摘要,
    std::string& 错误);

// 功能：写入控制面板 SQL 同步状态。
export bool 写入SQL控制面板同步状态(
    std::string_view 同步域,
    std::string_view 最近阶段,
    bool 成功,
    std::string_view 最近错误,
    std::string& 错误);
