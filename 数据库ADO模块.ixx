module;

#include <string>
#include <string_view>
#include <vector>

export module 数据库ADO模块;

export struct 结构_ADO查询结果 {
    std::vector<std::string> 列名{};
    std::vector<std::vector<std::string>> 行集{};
};

// 功能：生成 SQL Server Windows 身份认证 ADO 连接串。
export std::string 生成SQLServerWindows认证ADO连接串(
    std::string_view 服务器 = R"(.\SQLEXPRESS)",
    std::string_view 数据库 = "FishnestProjection");

// 功能：通过 ADO 执行 SQL 查询并返回文本化结果集。
export bool 执行ADO查询(
    const std::string& 连接串,
    const std::string& SQL,
    结构_ADO查询结果& 结果,
    std::string& 错误);

// 功能：通过 ADO 执行不返回结果集的 SQL 命令。
export bool 执行ADO命令(
    const std::string& 连接串,
    const std::string& SQL,
    std::string& 错误,
    int 命令超时秒 = 30);
