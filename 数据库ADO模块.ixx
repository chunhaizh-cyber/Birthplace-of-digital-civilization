module;

// 文件头部规则注释模块：
// 1. 本接口只暴露 SQL Server ADO 访问和控制面板显示镜像初始化能力。
// 2. SQL 写入只作为控制面板显示镜像，不得作为业务权威源或恢复源。
// 3. 写命令必须受 预处理开关变量.h 中 SQL 控制面板同步写入开关控制。

#include <string>
#include <string_view>
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
