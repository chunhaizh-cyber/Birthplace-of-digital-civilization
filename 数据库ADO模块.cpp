module;

// 文件头部规则注释模块：
// 1. 本模块只提供 SQL Server ADO 访问基础能力，不承载业务权威事实。
// 2. ADO 写命令受 预处理开关变量.h 中 SQL 控制面板同步写入开关控制；关闭时不得实际写库。
// 3. 查询和字段恢复比对只服务显示 / 诊断 / 存储验证，不得反向作为业务判断来源。

#include <comdef.h>
#include <atomic>
#include <cstdint>
#include <cstddef>
#include <iomanip>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "日志接入.h"
#include "预处理开关变量.h"

#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" \
    rename("EOF", "ADOEOF") \
    rename("BOF", "ADOBOF")

module 数据库ADO模块;

import 全局共享函数类;

namespace {
    std::atomic_bool 私有_SQL控制面板运行态可用{ false };
    std::atomic_bool 私有_SQL控制面板数据库未启动弹窗已提示{ false };
    std::mutex 私有_SQL控制面板运行态状态互斥{};
    std::string 私有_SQL控制面板运行态不可用原因{ "尚未初始化" };

    // 功能：把 UTF-8 文本转换为 Windows 宽字串。
    std::wstring 私有_UTF8转宽字串(const std::string_view 输入)
    {
        if (输入.empty()) {
            return {};
        }
        const int 所需长度 = MultiByteToWideChar(
            CP_UTF8,
            0,
            输入.data(),
            static_cast<int>(输入.size()),
            nullptr,
            0);
        if (所需长度 <= 0) {
            return {};
        }
        std::wstring 输出(static_cast<std::size_t>(所需长度), L'\0');
        MultiByteToWideChar(
            CP_UTF8,
            0,
            输入.data(),
            static_cast<int>(输入.size()),
            输出.data(),
            所需长度);
        return 输出;
    }

    // 功能：把 Windows 宽字串转换为 UTF-8 文本。
    std::string 私有_宽字串转UTF8(const std::wstring_view 输入)
    {
        if (输入.empty()) {
            return {};
        }
        const int 所需长度 = WideCharToMultiByte(
            CP_UTF8,
            0,
            输入.data(),
            static_cast<int>(输入.size()),
            nullptr,
            0,
            nullptr,
            nullptr);
        if (所需长度 <= 0) {
            return {};
        }
        std::string 输出(static_cast<std::size_t>(所需长度), '\0');
        WideCharToMultiByte(
            CP_UTF8,
            0,
            输入.data(),
            static_cast<int>(输入.size()),
            输出.data(),
            所需长度,
            nullptr,
            nullptr);
        return 输出;
    }

    // 功能：把 BSTR 转换为 UTF-8 文本。
    std::string 私有_BSTR转UTF8(BSTR 文本)
    {
        if (文本 == nullptr) {
            return {};
        }
        return 私有_宽字串转UTF8(std::wstring_view(文本, SysStringLen(文本)));
    }

    // 功能：把 COM 异常转换为项目错误文本。
    std::string 私有_COM错误文本(const std::string_view 阶段, const _com_error& 错误)
    {
        std::string 文本(阶段);
        文本 += "失败 | HRESULT=";
        文本 += U32十六进制8位文本(static_cast<std::uint32_t>(错误.Error()));
        const auto 描述 = 错误.Description();
        if (描述.length() > 0) {
            文本 += " | 描述=";
            文本 += 私有_BSTR转UTF8(static_cast<BSTR>(描述));
        }
        else if (错误.ErrorMessage() != nullptr) {
            文本 += " | 描述=";
            文本 += 私有_宽字串转UTF8(错误.ErrorMessage());
        }
        return 文本;
    }

    // 功能：判断 ADO 错误是否指向 SQL Server 未启动或无法连接。
    bool 私有_是SQLServer未启动或拒绝访问错误(const std::string_view 错误)
    {
        return 错误.find("SQL Server 不存在或拒绝访问") != std::string_view::npos
            || 错误.find("ConnectionOpen") != std::string_view::npos
            || 错误.find("SQL Server does not exist or access denied") != std::string_view::npos
            || 错误.find("无法打开与 SQL Server 的连接") != std::string_view::npos
            || 错误.find("Named Pipes Provider") != std::string_view::npos
            || 错误.find("SQL Network Interfaces") != std::string_view::npos
            || 错误.find("network-related or instance-specific") != std::string_view::npos
            || 错误.find("Error Locating Server/Instance Specified") != std::string_view::npos;
    }

    // 功能：对 SQL Server 未启动或拒绝访问给出一次性弹窗提醒。
    void 私有_弹窗提示SQLServer未启动一次(
        const std::string_view 阶段,
        const std::string_view 错误)
    {
        if (!私有_是SQLServer未启动或拒绝访问错误(错误)) {
            return;
        }

        bool 未提示 = false;
        if (!私有_SQL控制面板数据库未启动弹窗已提示.compare_exchange_strong(未提示, true)) {
            return;
        }

        std::ostringstream 文本;
        文本
            << "检测到 SQL Server 数据库未启动或拒绝访问。\n\n"
            << "实例: .\\SQLEXPRESS\n"
            << "服务: SQL Server (SQLEXPRESS) / MSSQL$SQLEXPRESS\n"
            << "阶段: " << 阶段 << "\n\n"
            << "当前只影响控制面板 SQL 显示镜像，业务结构不会回滚。"
            << "\n请启动数据库服务后重新启动鱼巢，或重新执行需要 SQL 投影的命令。\n\n"
            << "原始错误: " << 错误;
        项目弹窗错误提示("鱼巢 - 数据库未启动提醒", 文本.str());
    }

    // 功能：记录控制面板运行态 SQL 显示镜像不可用原因。
    void 私有_标记SQL控制面板运行态不可用(const std::string_view 原因)
    {
        私有_SQL控制面板运行态可用.store(false);
        {
            std::lock_guard<std::mutex> 锁{ 私有_SQL控制面板运行态状态互斥 };
            私有_SQL控制面板运行态不可用原因 = 原因.empty()
                ? "未知原因"
                : std::string(原因);
        }
        私有_弹窗提示SQLServer未启动一次("控制面板运行态SQL初始化", 原因);
    }

    // 功能：记录控制面板运行态 SQL 显示镜像已初始化可用。
    void 私有_标记SQL控制面板运行态可用()
    {
        std::lock_guard<std::mutex> 锁{ 私有_SQL控制面板运行态状态互斥 };
        私有_SQL控制面板运行态不可用原因.clear();
        私有_SQL控制面板运行态可用.store(true);
    }

    // 功能：检查控制面板运行态 SQL 显示镜像是否已初始化可用。
    bool 私有_确认SQL控制面板运行态可用(std::string& 错误)
    {
        if (私有_SQL控制面板运行态可用.load()) {
            return true;
        }

        std::lock_guard<std::mutex> 锁{ 私有_SQL控制面板运行态状态互斥 };
        错误 = "SQL控制面板运行态不可用 | 原因=";
        错误 += 私有_SQL控制面板运行态不可用原因.empty()
            ? "尚未初始化"
            : 私有_SQL控制面板运行态不可用原因;
        return false;
    }

    struct 结构_COM初始化 {
        bool 需要反初始化 = false;
        bool 可继续 = false;
        HRESULT 结果 = S_OK;

        // 功能：初始化当前线程 COM 环境，兼容已用其他模型初始化的线程。
        结构_COM初始化()
        {
            结果 = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            if (SUCCEEDED(结果)) {
                需要反初始化 = true;
                可继续 = true;
            }
            else if (结果 == RPC_E_CHANGED_MODE) {
                可继续 = true;
            }
        }

        ~结构_COM初始化()
        {
            if (需要反初始化) {
                CoUninitialize();
            }
        }
    };

    // 功能：打开 ADO 数据库连接。
    bool 私有_打开ADO连接(
        const std::string& 连接串,
        ADODB::_ConnectionPtr& 连接,
        std::string& 错误)
    {
        try {
            const HRESULT 创建结果 = 连接.CreateInstance(__uuidof(ADODB::Connection));
            if (FAILED(创建结果)) {
                错误 = "ADO连接对象创建失败 | HRESULT=" + U32十六进制8位文本(static_cast<std::uint32_t>(创建结果));
                return false;
            }
            连接->ConnectionTimeout = 5;
            连接->CommandTimeout = 30;
            const auto 宽连接串 = 私有_UTF8转宽字串(连接串);
            连接->Open(
                _bstr_t(宽连接串.c_str()),
                _bstr_t(L""),
                _bstr_t(L""),
                ADODB::adConnectUnspecified);
            return true;
        }
        catch (const _com_error& COM错误) {
            错误 = 私有_COM错误文本("ADO打开连接", COM错误);
            私有_弹窗提示SQLServer未启动一次("ADO打开连接", 错误);
            return false;
        }
    }

    // 功能：把 ADO 字段值转换为 UTF-8 文本。
    std::string 私有_ADO值转UTF8(const _variant_t& 值)
    {
        if (值.vt == VT_EMPTY || 值.vt == VT_NULL) {
            return {};
        }
        if (值.vt == VT_BSTR) {
            return 私有_BSTR转UTF8(值.bstrVal);
        }
        try {
            _variant_t 文本值(值);
            文本值.ChangeType(VT_BSTR);
            return 私有_BSTR转UTF8(文本值.bstrVal);
        }
        catch (const _com_error&) {
            return {};
        }
    }

    // 功能：关闭 ADO 连接并吞掉关闭异常。
    void 私有_关闭ADO连接(ADODB::_ConnectionPtr& 连接) noexcept
    {
        try {
            if (连接 != nullptr && 连接->State == ADODB::adStateOpen) {
                连接->Close();
            }
        }
        catch (...) {
        }
    }

    // 功能：截断字段差异文本，避免日志输出整行大字段。
    std::string 私有_截断字段差异文本(const std::string& 文本)
    {
        constexpr std::size_t 上限 = 160;
        if (文本.size() <= 上限) {
            return 文本;
        }
        return 文本.substr(0, 上限) + "...";
    }

    // 功能：把文本编码为 SQL Server Unicode 字符串字面量。
    std::string 私有_SQL字符串(const std::string_view 文本)
    {
        std::string 输出 = "N'";
        输出.reserve(文本.size() + 3);
        for (const char 字符 : 文本) {
            if (字符 == '\'') {
                输出 += "''";
            }
            else {
                输出.push_back(字符);
            }
        }
        输出 += "'";
        return 输出;
    }

    // 功能：把可空文本编码为 SQL Server 字面量。
    std::string 私有_SQL可空字符串(const std::string_view 文本)
    {
        if (文本.empty()) {
            return "NULL";
        }
        return 私有_SQL字符串(文本);
    }

    // 功能：把可空整数编码为 SQL Server 字面量。
    std::string 私有_SQL可空整数(const std::optional<std::int64_t> 值)
    {
        if (!值.has_value()) {
            return "NULL";
        }
        return std::to_string(*值);
    }

    // 功能：把可空布尔值编码为 SQL Server bit 字面量。
    std::string 私有_SQL可空布尔(const std::optional<bool> 值)
    {
        if (!值.has_value()) {
            return "NULL";
        }
        return *值 ? "1" : "0";
    }

    // 功能：读取 SQL 控制面板显示域的显示文本。
    std::string_view 私有_SQL控制面板显示域文本(const 枚举_SQL控制面板显示域 显示域)
    {
        switch (显示域) {
        case 枚举_SQL控制面板显示域::任务:
            return "任务";
        case 枚举_SQL控制面板显示域::需求:
            return "需求";
        case 枚举_SQL控制面板显示域::方法:
            return "方法";
        case 枚举_SQL控制面板显示域::世界:
            return "世界";
        case 枚举_SQL控制面板显示域::动态:
            return "动态";
        default:
            return {};
        }
    }

    // 功能：读取 SQL 控制面板显示域对应的运行态表名。
    std::string_view 私有_SQL控制面板显示域表名(const 枚举_SQL控制面板显示域 显示域)
    {
        switch (显示域) {
        case 枚举_SQL控制面板显示域::任务:
            return "[鱼巢].[当前任务显示项]";
        case 枚举_SQL控制面板显示域::需求:
            return "[鱼巢].[当前需求显示项]";
        case 枚举_SQL控制面板显示域::方法:
            return "[鱼巢].[当前方法显示项]";
        case 枚举_SQL控制面板显示域::世界:
            return "[鱼巢].[当前世界显示项]";
        case 枚举_SQL控制面板显示域::动态:
            return "[鱼巢].[当前动态显示项]";
        default:
            return {};
        }
    }

    // 功能：读取 SQL 控制面板显示域对应的主键列名。
    std::string_view 私有_SQL控制面板显示域主键列(const 枚举_SQL控制面板显示域 显示域)
    {
        switch (显示域) {
        case 枚举_SQL控制面板显示域::任务:
            return "[任务主键]";
        case 枚举_SQL控制面板显示域::需求:
            return "[需求主键]";
        case 枚举_SQL控制面板显示域::方法:
            return "[方法主键]";
        case 枚举_SQL控制面板显示域::世界:
            return "[对象主键]";
        case 枚举_SQL控制面板显示域::动态:
            return "[动态主键]";
        default:
            return {};
        }
    }

    // 功能：读取当前控制面板运行批次 ID。
    bool 私有_读取SQL控制面板当前运行ID(std::string& 运行ID, std::string& 错误)
    {
        运行ID.clear();
        错误.clear();

        if (!私有_确认SQL控制面板运行态可用(错误)) {
            return false;
        }

        const auto 投影库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "鱼巢投影库");
        结构_ADO查询结果 查询结果{};
        std::string ADO错误{};
        if (!执行ADO查询(
            投影库连接串,
            "SELECT TOP (1) CONVERT(varchar(36), [运行ID]) FROM [鱼巢].[控制面板运行批次] ORDER BY [启动时间] DESC;",
            查询结果,
            ADO错误)) {
            错误 = "读取控制面板运行ID失败 | " + ADO错误;
            return false;
        }
        if (查询结果.行集.empty() || 查询结果.行集.front().empty() || 查询结果.行集.front().front().empty()) {
            错误 = "读取控制面板运行ID失败 | 原因=运行批次为空";
            return false;
        }
        运行ID = 查询结果.行集.front().front();
        return true;
    }

    // 功能：生成控制面板事件流水追加 SQL 片段。
    std::string 私有_SQL控制面板事件流水追加片段(
        const std::string& 运行ID,
        const 枚举_SQL控制面板显示域 显示域,
        const std::string_view 操作,
        const std::string_view 对象主键,
        const std::string_view 显示摘要)
    {
        std::ostringstream SQL;
        SQL
            << "INSERT INTO [鱼巢].[控制面板事件流水] ([运行ID], [发生时间], [领域], [操作], [对象主键], [显示摘要])\n"
            << "VALUES (CONVERT(uniqueidentifier, "
            << 私有_SQL字符串(运行ID)
            << "), SYSUTCDATETIME(), "
            << 私有_SQL字符串(私有_SQL控制面板显示域文本(显示域))
            << ", "
            << 私有_SQL字符串(操作)
            << ", "
            << 私有_SQL可空字符串(对象主键)
            << ", "
            << 私有_SQL可空字符串(显示摘要)
            << ");\n";
        return SQL.str();
    }

    // 功能：生成任务显示项 upsert 脚本。
    std::string 私有_SQL任务显示项Upsert脚本(const std::string& 运行ID, const 结构_SQL控制面板显示项& 显示项)
    {
        std::ostringstream SQL;
        SQL
            << "MERGE [鱼巢].[当前任务显示项] AS 目标\n"
            << "USING (SELECT CONVERT(uniqueidentifier, " << 私有_SQL字符串(运行ID) << ") AS [运行ID], "
            << 私有_SQL字符串(显示项.主键) << " AS [任务主键], "
            << 私有_SQL可空字符串(显示项.父主键) << " AS [父任务主键], "
            << 私有_SQL可空整数(显示项.状态值) << " AS [状态值], "
            << 私有_SQL可空字符串(显示项.状态文本) << " AS [状态文本], "
            << 私有_SQL可空字符串(显示项.显示摘要) << " AS [显示摘要]) AS 源\n"
            << "ON 目标.[任务主键] = 源.[任务主键]\n"
            << "WHEN MATCHED THEN UPDATE SET [运行ID]=源.[运行ID], [父任务主键]=源.[父任务主键], [状态值]=源.[状态值], [状态文本]=源.[状态文本], [更新时间]=SYSUTCDATETIME(), [显示摘要]=源.[显示摘要]\n"
            << "WHEN NOT MATCHED THEN INSERT ([运行ID], [任务主键], [父任务主键], [状态值], [状态文本], [更新时间], [显示摘要]) VALUES (源.[运行ID], 源.[任务主键], 源.[父任务主键], 源.[状态值], 源.[状态文本], SYSUTCDATETIME(), 源.[显示摘要]);\n";
        return SQL.str();
    }

    // 功能：生成需求显示项 upsert 脚本。
    std::string 私有_SQL需求显示项Upsert脚本(const std::string& 运行ID, const 结构_SQL控制面板显示项& 显示项)
    {
        std::ostringstream SQL;
        SQL
            << "MERGE [鱼巢].[当前需求显示项] AS 目标\n"
            << "USING (SELECT CONVERT(uniqueidentifier, " << 私有_SQL字符串(运行ID) << ") AS [运行ID], "
            << 私有_SQL字符串(显示项.主键) << " AS [需求主键], "
            << 私有_SQL可空字符串(显示项.父主键) << " AS [父需求主键], "
            << 私有_SQL可空整数(显示项.满足关系掩码) << " AS [满足关系掩码], "
            << 私有_SQL可空布尔(显示项.本轮已截止) << " AS [本轮已截止], "
            << 私有_SQL可空字符串(显示项.显示摘要) << " AS [显示摘要]) AS 源\n"
            << "ON 目标.[需求主键] = 源.[需求主键]\n"
            << "WHEN MATCHED THEN UPDATE SET [运行ID]=源.[运行ID], [父需求主键]=源.[父需求主键], [满足关系掩码]=源.[满足关系掩码], [本轮已截止]=源.[本轮已截止], [更新时间]=SYSUTCDATETIME(), [显示摘要]=源.[显示摘要]\n"
            << "WHEN NOT MATCHED THEN INSERT ([运行ID], [需求主键], [父需求主键], [满足关系掩码], [本轮已截止], [更新时间], [显示摘要]) VALUES (源.[运行ID], 源.[需求主键], 源.[父需求主键], 源.[满足关系掩码], 源.[本轮已截止], SYSUTCDATETIME(), 源.[显示摘要]);\n";
        return SQL.str();
    }

    // 功能：生成方法显示项 upsert 脚本。
    std::string 私有_SQL方法显示项Upsert脚本(const std::string& 运行ID, const 结构_SQL控制面板显示项& 显示项)
    {
        std::ostringstream SQL;
        SQL
            << "MERGE [鱼巢].[当前方法显示项] AS 目标\n"
            << "USING (SELECT CONVERT(uniqueidentifier, " << 私有_SQL字符串(运行ID) << ") AS [运行ID], "
            << 私有_SQL字符串(显示项.主键) << " AS [方法主键], "
            << 私有_SQL可空字符串(显示项.父主键) << " AS [父方法主键], "
            << 私有_SQL可空整数(显示项.状态值) << " AS [状态值], "
            << 私有_SQL可空字符串(显示项.显示摘要) << " AS [显示摘要]) AS 源\n"
            << "ON 目标.[方法主键] = 源.[方法主键]\n"
            << "WHEN MATCHED THEN UPDATE SET [运行ID]=源.[运行ID], [父方法主键]=源.[父方法主键], [状态值]=源.[状态值], [更新时间]=SYSUTCDATETIME(), [显示摘要]=源.[显示摘要]\n"
            << "WHEN NOT MATCHED THEN INSERT ([运行ID], [方法主键], [父方法主键], [状态值], [更新时间], [显示摘要]) VALUES (源.[运行ID], 源.[方法主键], 源.[父方法主键], 源.[状态值], SYSUTCDATETIME(), 源.[显示摘要]);\n";
        return SQL.str();
    }

    // 功能：生成世界显示项 upsert 脚本。
    std::string 私有_SQL世界显示项Upsert脚本(const std::string& 运行ID, const 结构_SQL控制面板显示项& 显示项)
    {
        std::ostringstream SQL;
        SQL
            << "MERGE [鱼巢].[当前世界显示项] AS 目标\n"
            << "USING (SELECT CONVERT(uniqueidentifier, " << 私有_SQL字符串(运行ID) << ") AS [运行ID], "
            << 私有_SQL字符串(显示项.主键) << " AS [对象主键], "
            << 私有_SQL可空字符串(显示项.父主键) << " AS [父对象主键], "
            << 私有_SQL可空字符串(显示项.类型文本) << " AS [对象类型], "
            << 私有_SQL可空字符串(显示项.显示摘要) << " AS [显示摘要]) AS 源\n"
            << "ON 目标.[对象主键] = 源.[对象主键]\n"
            << "WHEN MATCHED THEN UPDATE SET [运行ID]=源.[运行ID], [父对象主键]=源.[父对象主键], [对象类型]=源.[对象类型], [更新时间]=SYSUTCDATETIME(), [显示摘要]=源.[显示摘要]\n"
            << "WHEN NOT MATCHED THEN INSERT ([运行ID], [对象主键], [父对象主键], [对象类型], [更新时间], [显示摘要]) VALUES (源.[运行ID], 源.[对象主键], 源.[父对象主键], 源.[对象类型], SYSUTCDATETIME(), 源.[显示摘要]);\n";
        return SQL.str();
    }

    // 功能：生成动态显示项 upsert 脚本。
    std::string 私有_SQL动态显示项Upsert脚本(const std::string& 运行ID, const 结构_SQL控制面板显示项& 显示项)
    {
        std::ostringstream SQL;
        SQL
            << "MERGE [鱼巢].[当前动态显示项] AS 目标\n"
            << "USING (SELECT CONVERT(uniqueidentifier, " << 私有_SQL字符串(运行ID) << ") AS [运行ID], "
            << 私有_SQL字符串(显示项.主键) << " AS [动态主键], "
            << 私有_SQL可空字符串(显示项.父主键) << " AS [来源主键], "
            << 私有_SQL可空字符串(显示项.类型文本) << " AS [动态类型], "
            << 私有_SQL可空字符串(显示项.显示摘要) << " AS [显示摘要]) AS 源\n"
            << "ON 目标.[动态主键] = 源.[动态主键]\n"
            << "WHEN MATCHED THEN UPDATE SET [运行ID]=源.[运行ID], [来源主键]=源.[来源主键], [动态类型]=源.[动态类型], [更新时间]=SYSUTCDATETIME(), [显示摘要]=源.[显示摘要]\n"
            << "WHEN NOT MATCHED THEN INSERT ([运行ID], [动态主键], [来源主键], [动态类型], [更新时间], [显示摘要]) VALUES (源.[运行ID], 源.[动态主键], 源.[来源主键], 源.[动态类型], SYSUTCDATETIME(), 源.[显示摘要]);\n";
        return SQL.str();
    }

    // 功能：按显示域生成显示项 upsert 脚本。
    std::string 私有_SQL控制面板显示项Upsert脚本(const std::string& 运行ID, const 结构_SQL控制面板显示项& 显示项)
    {
        switch (显示项.显示域) {
        case 枚举_SQL控制面板显示域::任务:
            return 私有_SQL任务显示项Upsert脚本(运行ID, 显示项);
        case 枚举_SQL控制面板显示域::需求:
            return 私有_SQL需求显示项Upsert脚本(运行ID, 显示项);
        case 枚举_SQL控制面板显示域::方法:
            return 私有_SQL方法显示项Upsert脚本(运行ID, 显示项);
        case 枚举_SQL控制面板显示域::世界:
            return 私有_SQL世界显示项Upsert脚本(运行ID, 显示项);
        case 枚举_SQL控制面板显示域::动态:
            return 私有_SQL动态显示项Upsert脚本(运行ID, 显示项);
        default:
            return {};
        }
    }

    // 功能：执行 SQL 控制面板运行态写命令。
    bool 私有_执行SQL控制面板写命令(
        const std::string& 阶段,
        const std::string& SQL,
        std::string& 错误)
    {
        if (!私有_确认SQL控制面板运行态可用(错误)) {
            错误 = 阶段 + "失败 | " + 错误;
            return false;
        }

        const auto 投影库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "鱼巢投影库");
        std::string ADO错误{};
        if (!执行ADO命令(投影库连接串, SQL, ADO错误)) {
            错误 = 阶段 + "失败 | " + ADO错误;
            return false;
        }
        return true;
    }

    // 功能：生成控制面板运行态投影库建库脚本。
    std::string 私有_SQL控制面板运行态建库脚本()
    {
        return "IF DB_ID(N'鱼巢投影库') IS NULL CREATE DATABASE [鱼巢投影库];\n";
    }

    // 功能：生成控制面板运行态显示镜像的最小表结构脚本。
    std::string 私有_SQL控制面板运行态建表脚本()
    {
        std::ostringstream SQL;
        SQL
            << "IF SCHEMA_ID(N'鱼巢') IS NULL EXEC(N'CREATE SCHEMA [鱼巢]');\n"
            << "IF OBJECT_ID(N'[鱼巢].[控制面板运行批次]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[控制面板运行批次] (\n"
            << "    [运行ID] uniqueidentifier NOT NULL PRIMARY KEY,\n"
            << "    [启动时间] datetime2(3) NOT NULL,\n"
            << "    [来源标记] nvarchar(256) NOT NULL,\n"
            << "    [进程ID] int NOT NULL,\n"
            << "    [同步开关] bit NOT NULL,\n"
            << "    [状态] nvarchar(64) NOT NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[控制面板事件流水]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[控制面板事件流水] (\n"
            << "    [事件ID] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,\n"
            << "    [运行ID] uniqueidentifier NOT NULL,\n"
            << "    [发生时间] datetime2(3) NOT NULL,\n"
            << "    [领域] nvarchar(64) NOT NULL,\n"
            << "    [操作] nvarchar(64) NOT NULL,\n"
            << "    [对象主键] nvarchar(256) NULL,\n"
            << "    [显示摘要] nvarchar(1024) NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前任务显示项]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[当前任务显示项] (\n"
            << "    [运行ID] uniqueidentifier NOT NULL,\n"
            << "    [任务主键] nvarchar(256) NOT NULL PRIMARY KEY,\n"
            << "    [父任务主键] nvarchar(256) NULL,\n"
            << "    [状态值] bigint NULL,\n"
            << "    [状态文本] nvarchar(128) NULL,\n"
            << "    [更新时间] datetime2(3) NOT NULL,\n"
            << "    [显示摘要] nvarchar(1024) NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前需求显示项]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[当前需求显示项] (\n"
            << "    [运行ID] uniqueidentifier NOT NULL,\n"
            << "    [需求主键] nvarchar(256) NOT NULL PRIMARY KEY,\n"
            << "    [父需求主键] nvarchar(256) NULL,\n"
            << "    [满足关系掩码] bigint NULL,\n"
            << "    [本轮已截止] bit NULL,\n"
            << "    [更新时间] datetime2(3) NOT NULL,\n"
            << "    [显示摘要] nvarchar(1024) NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前需求显示项]', N'U') IS NOT NULL AND COL_LENGTH(N'鱼巢.当前需求显示项', N'本轮已截止') IS NULL\n"
            << "EXEC(N'ALTER TABLE [鱼巢].[当前需求显示项] ADD [本轮已截止] bit NULL');\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前需求显示项]', N'U') IS NOT NULL AND COL_LENGTH(N'鱼巢.当前需求显示项', N'已满足') IS NOT NULL AND COL_LENGTH(N'鱼巢.当前需求显示项', N'本轮已截止') IS NOT NULL\n"
            << "EXEC(N'UPDATE [鱼巢].[当前需求显示项] SET [本轮已截止] = COALESCE([本轮已截止], [已满足])');\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前方法显示项]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[当前方法显示项] (\n"
            << "    [运行ID] uniqueidentifier NOT NULL,\n"
            << "    [方法主键] nvarchar(256) NOT NULL PRIMARY KEY,\n"
            << "    [父方法主键] nvarchar(256) NULL,\n"
            << "    [状态值] bigint NULL,\n"
            << "    [更新时间] datetime2(3) NOT NULL,\n"
            << "    [显示摘要] nvarchar(1024) NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前世界显示项]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[当前世界显示项] (\n"
            << "    [运行ID] uniqueidentifier NOT NULL,\n"
            << "    [对象主键] nvarchar(256) NOT NULL PRIMARY KEY,\n"
            << "    [父对象主键] nvarchar(256) NULL,\n"
            << "    [对象类型] nvarchar(128) NULL,\n"
            << "    [更新时间] datetime2(3) NOT NULL,\n"
            << "    [显示摘要] nvarchar(1024) NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前动态显示项]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[当前动态显示项] (\n"
            << "    [运行ID] uniqueidentifier NOT NULL,\n"
            << "    [动态主键] nvarchar(256) NOT NULL PRIMARY KEY,\n"
            << "    [来源主键] nvarchar(256) NULL,\n"
            << "    [动态类型] nvarchar(128) NULL,\n"
            << "    [更新时间] datetime2(3) NOT NULL,\n"
            << "    [显示摘要] nvarchar(1024) NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[SQL投影同步状态]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[SQL投影同步状态] (\n"
            << "    [运行ID] uniqueidentifier NOT NULL,\n"
            << "    [同步域] nvarchar(128) NOT NULL PRIMARY KEY,\n"
            << "    [最近阶段] nvarchar(128) NOT NULL,\n"
            << "    [最近状态] nvarchar(64) NOT NULL,\n"
            << "    [更新时间] datetime2(3) NOT NULL,\n"
            << "    [最近错误] nvarchar(1024) NULL\n"
            << ");\n";
        return SQL.str();
    }

    // 功能：生成控制面板运行态启动清库和运行批次写入脚本。
    std::string 私有_SQL控制面板运行态启动清库脚本(
        const std::string& 运行ID,
        const std::string_view 来源标记)
    {
        std::ostringstream SQL;
        SQL
            << "SET XACT_ABORT ON;\n"
            << "BEGIN TRANSACTION;\n"
            << "DELETE FROM [鱼巢].[SQL投影同步状态];\n"
            << "DELETE FROM [鱼巢].[当前动态显示项];\n"
            << "DELETE FROM [鱼巢].[当前世界显示项];\n"
            << "DELETE FROM [鱼巢].[当前方法显示项];\n"
            << "DELETE FROM [鱼巢].[当前需求显示项];\n"
            << "DELETE FROM [鱼巢].[当前任务显示项];\n"
            << "DELETE FROM [鱼巢].[控制面板事件流水];\n"
            << "DELETE FROM [鱼巢].[控制面板运行批次];\n"
            << "DECLARE @运行ID uniqueidentifier = CONVERT(uniqueidentifier, "
            << 私有_SQL字符串(运行ID)
            << ");\n"
            << "INSERT INTO [鱼巢].[控制面板运行批次] ([运行ID], [启动时间], [来源标记], [进程ID], [同步开关], [状态])\n"
            << "VALUES (@运行ID, SYSUTCDATETIME(), "
            << 私有_SQL字符串(来源标记)
            << ", "
            << GetCurrentProcessId()
            << ", 1, N'运行中');\n"
            << "INSERT INTO [鱼巢].[SQL投影同步状态] ([运行ID], [同步域], [最近阶段], [最近状态], [更新时间], [最近错误])\n"
            << "VALUES (@运行ID, N'运行态初始化', N'启动清库', N'完成', SYSUTCDATETIME(), NULL);\n"
            << "INSERT INTO [鱼巢].[控制面板事件流水] ([运行ID], [发生时间], [领域], [操作], [对象主键], [显示摘要])\n"
            << "VALUES (@运行ID, SYSUTCDATETIME(), N'SQL控制面板', N'启动清库', CONVERT(nvarchar(36), @运行ID), N'控制面板运行态显示镜像已清空并写入运行批次');\n"
            << "COMMIT TRANSACTION;\n";
        return SQL.str();
    }
}

// 功能：生成 SQL Server Windows 身份认证 ADO 连接串。
std::string 生成SQLServerWindows认证ADO连接串(
    const std::string_view 服务器,
    const std::string_view 数据库)
{
    std::string 连接串 = "Provider=SQLOLEDB;Data Source=";
    连接串.append(服务器);
    连接串 += ";Initial Catalog=";
    连接串.append(数据库);
    连接串 += ";Integrated Security=SSPI;Persist Security Info=False;Connect Timeout=3;";
    return 连接串;
}

// 功能：通过 ADO 执行 SQL 查询并返回文本化结果集。
bool 执行ADO查询(
    const std::string& 连接串,
    const std::string& SQL,
    结构_ADO查询结果& 结果,
    std::string& 错误)
{
    结果 = {};
    错误.clear();

    const 结构_COM初始化 COM初始化{};
    if (!COM初始化.可继续) {
        错误 = "ADO查询COM初始化失败 | HRESULT=" + U32十六进制8位文本(static_cast<std::uint32_t>(COM初始化.结果));
        return false;
    }

    ADODB::_ConnectionPtr 连接;
    if (!私有_打开ADO连接(连接串, 连接, 错误)) {
        return false;
    }

    try {
        const auto 宽SQL = 私有_UTF8转宽字串(SQL);
        _variant_t 影响行数;
        ADODB::_RecordsetPtr 记录集 = 连接->Execute(
            _bstr_t(宽SQL.c_str()),
            &影响行数,
            ADODB::adCmdText);
        if (记录集 == nullptr) {
            私有_关闭ADO连接(连接);
            return true;
        }

        ADODB::FieldsPtr 字段集 = 记录集->Fields;
        const long 字段数量 = 字段集->Count;

        while (!记录集->GetADOEOF()) {
            std::vector<std::string> 行;
            行.reserve(static_cast<std::size_t>(字段数量));
            for (long i = 0; i < 字段数量; ++i) {
                ADODB::FieldPtr 字段 = 字段集->GetItem(_variant_t(i));
                行.push_back(私有_ADO值转UTF8(字段->Value));
            }
            结果.行集.push_back(std::move(行));
            记录集->MoveNext();
        }
        if (记录集->State == ADODB::adStateOpen) {
            记录集->Close();
        }
        私有_关闭ADO连接(连接);
        return true;
    }
    catch (const _com_error& COM错误) {
        错误 = 私有_COM错误文本("ADO执行查询", COM错误);
        私有_关闭ADO连接(连接);
        return false;
    }
}

// 功能：从 SQL Server 恢复字段行集并与原始字段值逐项比较。
bool 执行ADO字段恢复比对(
    const std::string& 连接串,
    const std::string& SQL,
    const std::vector<std::vector<std::string>>& 预期行集,
    结构_ADO字段恢复比对结果& 比对结果,
    std::string& 错误)
{
    比对结果 = {};
    错误.clear();

    结构_ADO查询结果 恢复结果{};
    if (!执行ADO查询(连接串, SQL, 恢复结果, 错误)) {
        return false;
    }

    比对结果.预期行数 = 预期行集.size();
    比对结果.恢复行数 = 恢复结果.行集.size();
    if (比对结果.预期行数 != 比对结果.恢复行数) {
        比对结果.首个差异 = "行数不一致 | 预期="
            + std::to_string(比对结果.预期行数)
            + " | 恢复="
            + std::to_string(比对结果.恢复行数);
        return true;
    }

    for (std::size_t 行号 = 0; 行号 < 预期行集.size(); ++行号) {
        const auto& 预期行 = 预期行集[行号];
        const auto& 恢复行 = 恢复结果.行集[行号];
        if (预期行.size() != 恢复行.size()) {
            比对结果.首个差异 = "字段数不一致 | 行="
                + std::to_string(行号 + 1)
                + " | 预期="
                + std::to_string(预期行.size())
                + " | 恢复="
                + std::to_string(恢复行.size());
            return true;
        }

        for (std::size_t 字段号 = 0; 字段号 < 预期行.size(); ++字段号) {
            if (预期行[字段号] == 恢复行[字段号]) {
                continue;
            }
            比对结果.首个差异 = "字段值不一致 | 行="
                + std::to_string(行号 + 1)
                + " | 字段="
                + std::to_string(字段号 + 1)
                + " | 预期="
                + 私有_截断字段差异文本(预期行[字段号])
                + " | 恢复="
                + 私有_截断字段差异文本(恢复行[字段号]);
            return true;
        }
    }

    比对结果.匹配 = true;
    return true;
}

// 功能：通过 ADO 执行不返回结果集的 SQL 命令。
bool 执行ADO命令(
    const std::string& 连接串,
    const std::string& SQL,
    std::string& 错误,
    const int 命令超时秒)
{
    错误.clear();

#if !鱼巢_开关_启用SQL控制面板同步写入
    (void)连接串;
    (void)SQL;
    (void)命令超时秒;
    错误 = "ADO命令写入已被预处理开关关闭 | 开关=鱼巢_开关_启用SQL控制面板同步写入";
    return false;
#else
    const 结构_COM初始化 COM初始化{};
    if (!COM初始化.可继续) {
        错误 = "ADO命令COM初始化失败 | HRESULT=" + U32十六进制8位文本(static_cast<std::uint32_t>(COM初始化.结果));
        return false;
    }

    ADODB::_ConnectionPtr 连接;
    if (!私有_打开ADO连接(连接串, 连接, 错误)) {
        return false;
    }

    try {
        if (命令超时秒 >= 0) {
            连接->CommandTimeout = 命令超时秒;
        }
        const auto 宽SQL = 私有_UTF8转宽字串(SQL);
        _variant_t 影响行数;
        连接->Execute(
            _bstr_t(宽SQL.c_str()),
            &影响行数,
            ADODB::adExecuteNoRecords | ADODB::adCmdText);
        私有_关闭ADO连接(连接);
        return true;
    }
    catch (const _com_error& COM错误) {
        错误 = 私有_COM错误文本("ADO执行命令", COM错误);
        私有_关闭ADO连接(连接);
        return false;
    }
#endif
}

// 功能：初始化控制面板运行态 SQL 显示镜像，清空本次运行态表并写入运行批次。
bool 初始化SQL控制面板运行态投影(
    const std::string_view 来源标记,
    std::string& 运行ID,
    std::string& 错误)
{
    运行ID.clear();
    错误.clear();

#if !鱼巢_开关_启用SQL控制面板同步写入
    (void)来源标记;
    错误 = "SQL控制面板同步写入已被预处理开关关闭";
    私有_标记SQL控制面板运行态不可用(错误);
    return false;
#else
    const auto 主库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "master");
    const auto 投影库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "鱼巢投影库");

    结构_ADO查询结果 查询结果{};
    std::string ADO错误{};
    if (!执行ADO查询(主库连接串, "SELECT CONVERT(varchar(36), NEWID());", 查询结果, ADO错误)
        || 查询结果.行集.empty()
        || 查询结果.行集.front().empty()
        || 查询结果.行集.front().front().empty()) {
        错误 = ADO错误.empty() ? "生成运行ID失败" : ("生成运行ID失败 | " + ADO错误);
        私有_标记SQL控制面板运行态不可用(错误);
        return false;
    }
    运行ID = 查询结果.行集.front().front();

    if (!执行ADO命令(主库连接串, 私有_SQL控制面板运行态建库脚本(), ADO错误)) {
        错误 = "控制面板运行态建库失败 | " + ADO错误;
        私有_标记SQL控制面板运行态不可用(错误);
        return false;
    }
    if (!执行ADO命令(投影库连接串, 私有_SQL控制面板运行态建表脚本(), ADO错误)) {
        错误 = "控制面板运行态建表失败 | " + ADO错误;
        私有_标记SQL控制面板运行态不可用(错误);
        return false;
    }
    if (!执行ADO命令(
        投影库连接串,
        私有_SQL控制面板运行态启动清库脚本(运行ID, 来源标记),
        ADO错误)) {
        错误 = "控制面板运行态启动清库失败 | " + ADO错误;
        私有_标记SQL控制面板运行态不可用(错误);
        return false;
    }
    私有_标记SQL控制面板运行态可用();
    return true;
#endif
}

// 功能：同步新增或更新一个控制面板运行态显示项。
bool 同步写入SQL控制面板显示项(
    const 结构_SQL控制面板显示项& 显示项,
    std::string& 错误)
{
    错误.clear();
    if (显示项.主键.empty()) {
        错误 = "同步写入SQL控制面板显示项失败 | 原因=主键为空";
        return false;
    }
    if (私有_SQL控制面板显示域表名(显示项.显示域).empty()) {
        错误 = "同步写入SQL控制面板显示项失败 | 原因=显示域非法";
        return false;
    }

    std::string 运行ID{};
    if (!私有_读取SQL控制面板当前运行ID(运行ID, 错误)) {
        return false;
    }

    std::ostringstream SQL;
    SQL
        << "SET XACT_ABORT ON;\n"
        << "BEGIN TRANSACTION;\n"
        << 私有_SQL控制面板显示项Upsert脚本(运行ID, 显示项)
        << 私有_SQL控制面板事件流水追加片段(
            运行ID,
            显示项.显示域,
            "新增或更新",
            显示项.主键,
            显示项.显示摘要)
        << "COMMIT TRANSACTION;\n";
    return 私有_执行SQL控制面板写命令("同步写入SQL控制面板显示项", SQL.str(), 错误);
}

// 功能：同步删除一个控制面板运行态显示项。
bool 同步删除SQL控制面板显示项(
    const 枚举_SQL控制面板显示域 显示域,
    const std::string_view 主键,
    const std::string_view 显示摘要,
    std::string& 错误)
{
    错误.clear();
    if (主键.empty()) {
        错误 = "同步删除SQL控制面板显示项失败 | 原因=主键为空";
        return false;
    }
    const auto 表名 = 私有_SQL控制面板显示域表名(显示域);
    const auto 主键列 = 私有_SQL控制面板显示域主键列(显示域);
    if (表名.empty() || 主键列.empty()) {
        错误 = "同步删除SQL控制面板显示项失败 | 原因=显示域非法";
        return false;
    }

    std::string 运行ID{};
    if (!私有_读取SQL控制面板当前运行ID(运行ID, 错误)) {
        return false;
    }

    std::ostringstream SQL;
    SQL
        << "SET XACT_ABORT ON;\n"
        << "BEGIN TRANSACTION;\n"
        << "DELETE FROM "
        << 表名
        << " WHERE "
        << 主键列
        << " = "
        << 私有_SQL字符串(主键)
        << ";\n"
        << 私有_SQL控制面板事件流水追加片段(运行ID, 显示域, "删除", 主键, 显示摘要)
        << "COMMIT TRANSACTION;\n";
    return 私有_执行SQL控制面板写命令("同步删除SQL控制面板显示项", SQL.str(), 错误);
}

// 功能：追加一条控制面板运行态事件流水。
bool 追加SQL控制面板事件流水(
    const 枚举_SQL控制面板显示域 显示域,
    const std::string_view 操作,
    const std::string_view 对象主键,
    const std::string_view 显示摘要,
    std::string& 错误)
{
    错误.clear();
    if (操作.empty()) {
        错误 = "追加SQL控制面板事件流水失败 | 原因=操作为空";
        return false;
    }
    if (私有_SQL控制面板显示域文本(显示域).empty()) {
        错误 = "追加SQL控制面板事件流水失败 | 原因=显示域非法";
        return false;
    }

    std::string 运行ID{};
    if (!私有_读取SQL控制面板当前运行ID(运行ID, 错误)) {
        return false;
    }

    std::ostringstream SQL;
    SQL
        << "SET XACT_ABORT ON;\n"
        << "BEGIN TRANSACTION;\n"
        << 私有_SQL控制面板事件流水追加片段(运行ID, 显示域, 操作, 对象主键, 显示摘要)
        << "COMMIT TRANSACTION;\n";
    return 私有_执行SQL控制面板写命令("追加SQL控制面板事件流水", SQL.str(), 错误);
}

// 功能：写入控制面板 SQL 同步状态。
bool 写入SQL控制面板同步状态(
    const std::string_view 同步域,
    const std::string_view 最近阶段,
    const bool 成功,
    const std::string_view 最近错误,
    std::string& 错误)
{
    错误.clear();
    if (同步域.empty()) {
        错误 = "写入SQL控制面板同步状态失败 | 原因=同步域为空";
        return false;
    }
    if (最近阶段.empty()) {
        错误 = "写入SQL控制面板同步状态失败 | 原因=最近阶段为空";
        return false;
    }

    std::string 运行ID{};
    if (!私有_读取SQL控制面板当前运行ID(运行ID, 错误)) {
        return false;
    }

    std::ostringstream SQL;
    SQL
        << "MERGE [鱼巢].[SQL投影同步状态] AS 目标\n"
        << "USING (SELECT CONVERT(uniqueidentifier, "
        << 私有_SQL字符串(运行ID)
        << ") AS [运行ID], "
        << 私有_SQL字符串(同步域)
        << " AS [同步域], "
        << 私有_SQL字符串(最近阶段)
        << " AS [最近阶段], "
        << (成功 ? "N'完成'" : "N'失败'")
        << " AS [最近状态], "
        << 私有_SQL可空字符串(最近错误)
        << " AS [最近错误]) AS 源\n"
        << "ON 目标.[同步域] = 源.[同步域]\n"
        << "WHEN MATCHED THEN UPDATE SET [运行ID]=源.[运行ID], [最近阶段]=源.[最近阶段], [最近状态]=源.[最近状态], [更新时间]=SYSUTCDATETIME(), [最近错误]=源.[最近错误]\n"
        << "WHEN NOT MATCHED THEN INSERT ([运行ID], [同步域], [最近阶段], [最近状态], [更新时间], [最近错误]) VALUES (源.[运行ID], 源.[同步域], 源.[最近阶段], 源.[最近状态], SYSUTCDATETIME(), 源.[最近错误]);\n";
    return 私有_执行SQL控制面板写命令("写入SQL控制面板同步状态", SQL.str(), 错误);
}
