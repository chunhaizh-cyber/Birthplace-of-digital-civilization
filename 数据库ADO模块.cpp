module;

// 文件头部规则注释模块：
// 1. 本模块只提供 SQL Server ADO 访问基础能力，不承载业务权威事实。
// 2. ADO 写命令受 预处理开关变量.h 中 SQL 控制面板同步写入开关控制；关闭时不得实际写库。
// 3. 查询和字段恢复比对只服务显示 / 诊断 / 存储验证，不得反向作为业务判断来源。

#include <comdef.h>
#include <cstdint>
#include <cstddef>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "预处理开关变量.h"

#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" \
    rename("EOF", "ADOEOF") \
    rename("BOF", "ADOBOF")

module 数据库ADO模块;

namespace {

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

    // 功能：把 HRESULT 转换为固定宽度十六进制文本。
    std::string 私有_HRESULT文本(const HRESULT 值)
    {
        std::ostringstream 输出;
        输出 << "0x"
            << std::uppercase
            << std::hex
            << std::setw(8)
            << std::setfill('0')
            << static_cast<std::uint32_t>(值);
        return 输出.str();
    }

    // 功能：把 COM 异常转换为项目错误文本。
    std::string 私有_COM错误文本(const std::string_view 阶段, const _com_error& 错误)
    {
        std::string 文本(阶段);
        文本 += "失败 | HRESULT=";
        文本 += 私有_HRESULT文本(错误.Error());
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
                错误 = "ADO连接对象创建失败 | HRESULT=" + 私有_HRESULT文本(创建结果);
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
        错误 = "ADO查询COM初始化失败 | HRESULT=" + 私有_HRESULT文本(COM初始化.结果);
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
        错误 = "ADO命令COM初始化失败 | HRESULT=" + 私有_HRESULT文本(COM初始化.结果);
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
