module;

// 文件头部规则注释模块：
// 1. 本模块只负责 WebView2 窗口、导航和显示层消息桥接，不承载业务权威事实。
// 2. 诊断文本和路径文本只用于日志 / 显示，不得反向作为机器业务判断来源。
// 3. 通用格式化工具优先复用 全局共享函数类，不在本文件保留重复包装。

#include <atomic>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <exception>
#include <fstream>
#include <filesystem>
#include <future>
#include <iomanip>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <objbase.h>
#include <wrl.h>
#include <wincodec.h>

#include "WebView2导入.h"
#include "预处理开关变量.h"

#pragma comment(lib, "Windowscodecs.lib")

module 控制面板WebView2;

import 全局共享函数类;
import 控制面板类;
import 日志模块;
import 任务模块.管理界面线程;
import 自我类;
import 自我线程模块;
import 外设观察报告队列;
import 外设线程_D455深度相机;

using Microsoft::WRL::ComPtr;

namespace {
    constexpr UINT 私有_WM_刷新控制面板窗口 = WM_APP + 220;
    constexpr UINT 私有_WM_页面刷新JSON = WM_APP + 221;
    constexpr UINT 私有_WM_保存自我场景截图 = WM_APP + 222;
    constexpr wchar_t 私有_控制面板窗口类名[] = L"鱼巢控制面板WebView2重构窗口";

    enum class 枚举_WebView2窗口用途 : std::uintptr_t {
        控制面板 = 1,
        相机播放 = 2,
        自我场景 = 3,
    };

    using CreateCoreWebView2EnvironmentWithOptionsFn = HRESULT(STDAPICALLTYPE*)(
        PCWSTR,
        PCWSTR,
        ICoreWebView2EnvironmentOptions*,
        ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler*);

    struct 结构_WebView2窗口上下文 {
        枚举_WebView2窗口用途 用途 = 枚举_WebView2窗口用途::控制面板;
        HMODULE 加载器模块 = nullptr;
        ComPtr<ICoreWebView2Controller> 控制器{};
        ComPtr<ICoreWebView2> WebView{};
        std::filesystem::path 当前HTML临时路径{};
    };

    struct 结构_页面刷新投递 {
        std::string 页面{};
        std::string JSON{};
        std::int64_t 生成耗时毫秒 = 0;
    };

    struct 结构_自我场景截图请求 {
        std::filesystem::path 输出路径{};
        std::promise<bool> 完成{};
        std::atomic_bool 已完成{ false };
    };

    std::mutex 私有_窗口互斥{};
    std::mutex 私有_相机播放窗口互斥{};
    std::mutex 私有_自我场景窗口互斥{};
    std::atomic<HWND> 私有_窗口句柄{ nullptr };
    std::atomic<HWND> 私有_相机播放窗口句柄{ nullptr };
    std::atomic<HWND> 私有_自我场景窗口句柄{ nullptr };
    std::atomic_bool 私有_相机播放窗口启动中{ false };
    std::atomic_bool 私有_自我场景窗口启动中{ false };
    std::atomic<int> 私有_启动诊断码{ 0 };
    std::atomic_bool 私有_控制面板窗口线程运行中{ false };
    std::atomic_bool 私有_相机播放窗口线程运行中{ false };
    std::atomic_bool 私有_自我场景窗口线程运行中{ false };
    std::atomic_bool 私有_相机窗口启动过外设线程{ false };
    std::mutex 私有_自我场景帧缓存互斥{};
    std::string 私有_自我场景最近帧JSON{};
    std::uint64_t 私有_自我场景最近帧签名 = 0;
    std::chrono::steady_clock::time_point 私有_自我场景最近集中刷新时间{};
    std::atomic<std::uint64_t> 私有_自我场景待刷新请求数{ 0 };
    constexpr auto 私有_自我场景帧最小刷新间隔 = std::chrono::seconds(3);
    constexpr std::size_t 私有_NavigateToString安全字节数 = 1024u * 1024u;

    struct 结构_窗口线程运行守卫 {
        explicit 结构_窗口线程运行守卫(std::atomic_bool& 标记) noexcept
            : 标记指针(&标记)
        {
            标记.store(true);
        }

        ~结构_窗口线程运行守卫()
        {
            if (标记指针) {
                标记指针->store(false);
            }
        }

        结构_窗口线程运行守卫(const 结构_窗口线程运行守卫&) = delete;
        结构_窗口线程运行守卫& operator=(const 结构_窗口线程运行守卫&) = delete;

    private:
        std::atomic_bool* 标记指针 = nullptr;
    };

    struct 结构_相机帧JSON {
        bool 成功 = false;
        bool 已停止 = false;
        int 源宽 = 0;
        int 源高 = 0;
        int 宽 = 0;
        int 高 = 0;
        std::uint32_t 深度帧号 = 0;
        std::uint32_t 彩色帧号 = 0;
        std::uint64_t 设备时间_us = 0;
        std::size_t 分割簇数 = 0;
        std::int64_t 分割像素数 = 0;
        std::int64_t 轮廓线像素数 = 0;
        std::string 彩色RGB_Base64{};
        std::string 分割RGB_Base64{};
        std::string 消息{};
        bool 等待中 = false;
    };

    void 私有_记录WebView2诊断(
        const std::string& 阶段,
        int 诊断码,
        HRESULT COM结果 = S_OK,
        DWORD Win32错误 = ERROR_SUCCESS,
        const std::string& 附加 = {}) noexcept
    {
#if 鱼巢_开关_启用逻辑错误排查日志输出
        try {
            std::ostringstream 输出;
            输出 << "控制面板WebView2/" << 阶段
                << " | 诊断码=" << 诊断码;
            if (COM结果 != S_OK) {
                输出 << " | HRESULT=" << U32十六进制8位文本(static_cast<std::uint32_t>(COM结果));
            }
            if (Win32错误 != ERROR_SUCCESS) {
                输出 << " | Win32=" << Win32错误;
            }
            if (!附加.empty()) {
                输出 << " | " << 附加;
            }
            项目运行错误日志(输出.str());
        }
        catch (...) {
        }
#else
        (void)阶段;
        (void)诊断码;
        (void)COM结果;
        (void)Win32错误;
        (void)附加;
#endif
    }

    // 功能：服务所在模块的内部辅助流程。
    std::wstring 私有_UTF8转宽字串(const std::string& 输入)
    {
        if (输入.empty()) {
            return {};
        }

        const int 所需长度 = MultiByteToWideChar(CP_UTF8, 0, 输入.c_str(), -1, nullptr, 0);
        if (所需长度 <= 0) {
            return {};
        }

        std::wstring 输出(static_cast<std::size_t>(所需长度), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, 输入.c_str(), -1, 输出.data(), 所需长度);
        if (!输出.empty() && 输出.back() == L'\0') {
            输出.pop_back();
        }
        return 输出;
    }

    // 功能：把本地 HTML 文件路径转换为 WebView2 可导航的 file URI。
    std::wstring 私有_文件路径转URI(const std::filesystem::path& 路径)
    {
        auto 文本 = std::filesystem::absolute(路径).wstring();
        std::replace(文本.begin(), 文本.end(), L'\\', L'/');
        if (文本.rfind(L"//", 0) == 0) {
            return L"file:" + 文本;
        }
        return L"file:///" + 文本;
    }

    // 功能：生成当前 WebView2 窗口复用的临时 HTML 文件路径。
    std::filesystem::path 私有_控制面板HTML临时路径(
        HWND 窗口,
        枚举_WebView2窗口用途 用途)
    {
        auto 目录 = std::filesystem::temp_directory_path() / L"fishnest_control_panel_webview2";
        std::filesystem::create_directories(目录);

        std::wstring 文件名 = L"panel_";
        文件名 += std::to_wstring(GetCurrentProcessId());
        文件名 += L"_";
        文件名 += std::to_wstring(reinterpret_cast<std::uintptr_t>(窗口));
        文件名 += L"_";
        文件名 += std::to_wstring(static_cast<std::uintptr_t>(用途));
        文件名 += L".html";
        return 目录 / 文件名;
    }

    // 功能：把 UTF-8 HTML 写入 WebView2 临时文件。
    bool 私有_写入控制面板HTML临时文件(
        const std::filesystem::path& 路径,
        const std::string& HTML) noexcept
    {
        try {
            std::ofstream 输出(路径, std::ios::binary | std::ios::trunc);
            if (!输出) {
                return false;
            }
            输出.write(HTML.data(), static_cast<std::streamsize>(HTML.size()));
            return static_cast<bool>(输出);
        }
        catch (...) {
            return false;
        }
    }

    // 功能：在 HTML 过大或内存导航失败时改用临时文件导航。
    bool 私有_用临时HTML文件导航(
        HWND 窗口,
        结构_WebView2窗口上下文& 上下文,
        const std::string& HTML) noexcept
    {
        try {
            if (上下文.当前HTML临时路径.empty()) {
                上下文.当前HTML临时路径 = 私有_控制面板HTML临时路径(窗口, 上下文.用途);
            }
            if (!私有_写入控制面板HTML临时文件(上下文.当前HTML临时路径, HTML)) {
                私有_记录WebView2诊断(
                    "临时HTML写入失败",
                    31,
                    S_OK,
                    GetLastError(),
                    "路径=" + 路径UTF8文本(上下文.当前HTML临时路径)
                        + " | HTML字节=" + std::to_string(HTML.size()));
                return false;
            }

            const auto URI = 私有_文件路径转URI(上下文.当前HTML临时路径);
            const HRESULT 导航结果 = 上下文.WebView->Navigate(URI.c_str());
            if (FAILED(导航结果)) {
                私有_记录WebView2诊断(
                    "临时HTML导航失败",
                    32,
                    导航结果,
                    GetLastError(),
                    "路径=" + 路径UTF8文本(上下文.当前HTML临时路径)
                        + " | HTML字节=" + std::to_string(HTML.size()));
                return false;
            }
            return true;
        }
        catch (const std::exception& e) {
            私有_记录WebView2诊断(
                "临时HTML导航异常",
                33,
                S_OK,
                GetLastError(),
                std::string("原因=") + e.what());
            return false;
        }
        catch (...) {
            私有_记录WebView2诊断("临时HTML导航未知异常", 34, S_OK, GetLastError());
            return false;
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_宽字串转UTF8(const std::wstring& 输入)
    {
        if (输入.empty()) {
            return {};
        }

        const int 所需长度 = WideCharToMultiByte(CP_UTF8, 0, 输入.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (所需长度 <= 0) {
            return {};
        }

        std::string 输出(static_cast<std::size_t>(所需长度), '\0');
        WideCharToMultiByte(CP_UTF8, 0, 输入.c_str(), -1, 输出.data(), 所需长度, nullptr, nullptr);
        if (!输出.empty() && 输出.back() == '\0') {
            输出.pop_back();
        }
        return 输出;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    bool 私有_解析U64(std::wstring_view 文本, std::uint64_t& 输出值) noexcept
    {
        if (文本.empty()) {
            return false;
        }
        std::uint64_t 值 = 0;
        for (const auto 字符 : 文本) {
            if (字符 < L'0' || 字符 > L'9') {
                return false;
            }
            const auto 数字 = static_cast<std::uint64_t>(字符 - L'0');
            if (值 > (UINT64_MAX - 数字) / 10) {
                return false;
            }
            值 = 值 * 10 + 数字;
        }
        输出值 = 值;
        return true;
    }

    bool 私有_解析节点消息(
        const std::wstring& 消息,
        const std::wstring_view 前缀,
        std::uint64_t* 请求号,
        std::string* 展开类型,
        std::uintptr_t* 节点指针,
        std::uintptr_t* 附加参数 = nullptr) noexcept
    {
        if (!消息.starts_with(前缀)) {
            return false;
        }

        const auto 前缀长度 = 前缀.size();
        const std::size_t 第一分隔 = 消息.find(L':', 前缀长度);
        if (第一分隔 == std::wstring::npos) {
            return false;
        }
        const std::size_t 第二分隔 = 消息.find(L':', 第一分隔 + 1);
        if (第二分隔 == std::wstring::npos) {
            return false;
        }
        const std::size_t 第三分隔 = 消息.find(L':', 第二分隔 + 1);

        std::uint64_t 解析值 = 0;
        if (请求号) {
            if (!私有_解析U64(
                    std::wstring_view(消息).substr(前缀长度, 第一分隔 - 前缀长度),
                    解析值)) {
                return false;
            }
            *请求号 = 解析值;
        }
        if (展开类型) {
            *展开类型 = 私有_宽字串转UTF8(消息.substr(第一分隔 + 1, 第二分隔 - 第一分隔 - 1));
        }
        if (节点指针) {
            const auto 节点文本 = 第三分隔 == std::wstring::npos
                ? std::wstring_view(消息).substr(第二分隔 + 1)
                : std::wstring_view(消息).substr(第二分隔 + 1, 第三分隔 - 第二分隔 - 1);
            if (!私有_解析U64(节点文本, 解析值)) {
                return false;
            }
            *节点指针 = static_cast<std::uintptr_t>(解析值);
        }
        if (附加参数) {
            if (第三分隔 == std::wstring::npos) {
                *附加参数 = 0;
            }
            else {
                if (!私有_解析U64(std::wstring_view(消息).substr(第三分隔 + 1), 解析值)) {
                    return false;
                }
                *附加参数 = static_cast<std::uintptr_t>(解析值);
            }
        }
        return true;
    }

    bool 私有_解析展开消息(
        const std::wstring& 消息,
        std::uint64_t* 请求号,
        std::string* 展开类型,
        std::uintptr_t* 节点指针,
        std::uintptr_t* 附加参数 = nullptr) noexcept
    {
        return 私有_解析节点消息(消息, L"expand:", 请求号, 展开类型, 节点指针, 附加参数);
    }

    // 功能：解析输入文本、消息、场景或运行包。
    bool 私有_解析详情消息(
        const std::wstring& 消息,
        std::uint64_t* 请求号,
        std::string* 展开类型,
        std::uintptr_t* 节点指针) noexcept
    {
        return 私有_解析节点消息(消息, L"detail:", 请求号, 展开类型, 节点指针);
    }

    // 功能：解析 URI 百分号编码文本。
    std::string 私有_URI百分号解码(std::string_view 文本)
    {
        auto 十六进制值 = [](const char 字符) -> int {
            if (字符 >= '0' && 字符 <= '9') {
                return 字符 - '0';
            }
            if (字符 >= 'A' && 字符 <= 'F') {
                return 字符 - 'A' + 10;
            }
            if (字符 >= 'a' && 字符 <= 'f') {
                return 字符 - 'a' + 10;
            }
            return -1;
        };

        std::string 输出{};
        输出.reserve(文本.size());
        for (std::size_t 索引 = 0; 索引 < 文本.size(); ++索引) {
            if (文本[索引] == '%' && 索引 + 2 < 文本.size()) {
                const int 高位 = 十六进制值(文本[索引 + 1]);
                const int 低位 = 十六进制值(文本[索引 + 2]);
                if (高位 >= 0 && 低位 >= 0) {
                    输出.push_back(static_cast<char>((高位 << 4) | 低位));
                    索引 += 2;
                    continue;
                }
            }
            输出.push_back(文本[索引]);
        }
        return 输出;
    }

    // 功能：解析 SQL 控制面板树子链读取消息。
    bool 私有_解析SQL子链消息(
        const std::wstring& 消息,
        std::uint64_t* 请求号,
        std::string* 区段ID,
        std::string* 节点键) noexcept
    {
        constexpr std::wstring_view 前缀 = L"sql-subtree:";
        if (!消息.starts_with(前缀)) {
            return false;
        }

        const auto 第一分隔 = 消息.find(L':', 前缀.size());
        if (第一分隔 == std::wstring::npos) {
            return false;
        }
        const auto 第二分隔 = 消息.find(L':', 第一分隔 + 1);
        if (第二分隔 == std::wstring::npos) {
            return false;
        }

        std::uint64_t 解析请求号 = 0;
        if (!私有_解析U64(
                std::wstring_view(消息).substr(前缀.size(), 第一分隔 - 前缀.size()),
                解析请求号)) {
            return false;
        }

        if (请求号) {
            *请求号 = 解析请求号;
        }
        if (区段ID) {
            *区段ID = 私有_宽字串转UTF8(
                消息.substr(第一分隔 + 1, 第二分隔 - 第一分隔 - 1));
        }
        if (节点键) {
            *节点键 = 私有_URI百分号解码(
                私有_宽字串转UTF8(消息.substr(第二分隔 + 1)));
        }
        return true;
    }

    // 功能：设置对象字段、状态或运行参数。
    bool 私有_解析线程池大小设置消息(
        const std::wstring& 消息,
        std::uint64_t* worker数量) noexcept
    {
        constexpr std::wstring_view 前缀 = L"settings:worker-pool-size:";
        if (!消息.starts_with(前缀)) {
            return false;
        }

        std::uint64_t 解析值 = 0;
        if (!私有_解析U64(std::wstring_view(消息).substr(前缀.size()), 解析值)) {
            return false;
        }
        if (worker数量) {
            *worker数量 = 解析值;
        }
        return true;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::filesystem::path 私有_模块目录() noexcept
    {
        wchar_t 缓冲区[MAX_PATH]{};
        const DWORD 长度 = GetModuleFileNameW(nullptr, 缓冲区, MAX_PATH);
        if (长度 == 0 || 长度 >= MAX_PATH) {
            return {};
        }
        return std::filesystem::path(缓冲区).parent_path();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::vector<std::filesystem::path> 私有_加载器候选路径()
    {
        std::vector<std::filesystem::path> 路径集{};
        const auto 模块目录 = 私有_模块目录();
        const auto 相对路径 = std::filesystem::path(L"third_party")
            / L"webview2"
            / 鱼巢_WEBVIEW2_SDK_VERSION_W
            / L"pkg"
            / L"build"
            / L"native"
#if defined(_WIN64)
            / L"x64"
#else
            / L"x86"
#endif
            / L"WebView2Loader.dll";

        std::error_code 错误{};
        const auto 当前目录 = std::filesystem::current_path(错误);

        if (!模块目录.empty()) {
            路径集.push_back((模块目录 / 相对路径).lexically_normal());
            路径集.push_back((模块目录 / L"WebView2Loader.dll").lexically_normal());
        }
        if (!错误) {
            路径集.push_back((当前目录 / 相对路径).lexically_normal());
            路径集.push_back((当前目录 / L"WebView2Loader.dll").lexically_normal());
        }
        路径集.push_back(L"WebView2Loader.dll");
        return 路径集;
    }

    // 功能：创建并返回或登记对应对象。
    CreateCoreWebView2EnvironmentWithOptionsFn 私有_加载创建函数(HMODULE* 加载器模块) noexcept
    {
        DWORD 最近加载错误 = ERROR_SUCCESS;
        std::filesystem::path 最近加载路径{};
        for (const auto& 路径 : 私有_加载器候选路径()) {
            if (路径.empty()) {
                continue;
            }

            HMODULE 模块 = LoadLibraryW(路径.c_str());
            if (!模块) {
                最近加载错误 = GetLastError();
                最近加载路径 = 路径;
                continue;
            }

            auto* 函数 = reinterpret_cast<CreateCoreWebView2EnvironmentWithOptionsFn>(
                GetProcAddress(模块, "CreateCoreWebView2EnvironmentWithOptions"));
            if (函数) {
                if (加载器模块) {
                    *加载器模块 = 模块;
                }
                return 函数;
            }

            私有_记录WebView2诊断(
                "加载器缺少创建函数",
                8,
                S_OK,
                GetLastError(),
                "路径=" + 路径UTF8文本(路径));
            FreeLibrary(模块);
        }

        私有_记录WebView2诊断(
            "加载器加载失败",
            8,
            S_OK,
            最近加载错误,
            最近加载路径.empty() ? std::string{} : ("最近路径=" + 路径UTF8文本(最近加载路径)));
        return nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    结构_WebView2窗口上下文* 私有_取窗口上下文(HWND 窗口) noexcept
    {
        return reinterpret_cast<结构_WebView2窗口上下文*>(GetWindowLongPtrW(窗口, GWLP_USERDATA));
    }

    bool 私有_确保窗口类已注册() noexcept;
    bool 私有_打开相机播放窗口(HWND 来源窗口) noexcept;
    bool 私有_打开自我场景窗口(HWND 来源窗口) noexcept;

    // 功能：完成自我场景截图请求并记录失败原因。
    void 私有_完成自我场景截图请求(
        const std::shared_ptr<结构_自我场景截图请求>& 请求,
        const bool 成功,
        const HRESULT COM结果 = S_OK,
        const DWORD Win32错误 = ERROR_SUCCESS,
        const std::string& 附加 = {}) noexcept
    {
        if (!请求 || 请求->已完成.exchange(true)) {
            return;
        }
        if (!成功) {
            std::string 诊断 = 请求->输出路径.empty()
                ? std::string{}
                : ("路径=" + 路径UTF8文本(请求->输出路径));
            if (!附加.empty()) {
                if (!诊断.empty()) {
                    诊断 += " | ";
                }
                诊断 += 附加;
            }
            私有_记录WebView2诊断(
                "自我场景截图失败",
                52,
                COM结果,
                Win32错误,
                诊断);
        }
        try {
            请求->完成.set_value(成功);
        }
        catch (...) {
        }
    }

    // 功能：把 WebView2 截图流保存为 PNG 文件。
    bool 私有_保存自我场景截图流到文件(
        IStream& 截图流,
        const std::filesystem::path& 输出路径) noexcept
    {
        try {
            LARGE_INTEGER 起点{};
            if (FAILED(截图流.Seek(起点, STREAM_SEEK_SET, nullptr))) {
                return false;
            }

            STATSTG 状态{};
            if (FAILED(截图流.Stat(&状态, STATFLAG_NONAME))) {
                return false;
            }

            const auto 父目录 = 输出路径.parent_path();
            if (!父目录.empty()) {
                std::error_code 创建错误{};
                std::filesystem::create_directories(父目录, 创建错误);
                if (创建错误) {
                    return false;
                }
            }

            std::ofstream 输出(输出路径, std::ios::binary | std::ios::trunc);
            if (!输出) {
                return false;
            }

            std::vector<char> 缓冲(64 * 1024);
            ULONGLONG 剩余字节 = 状态.cbSize.QuadPart;
            while (剩余字节 > 0) {
                const ULONG 本次请求 = static_cast<ULONG>(
                    std::min<ULONGLONG>(剩余字节, static_cast<ULONGLONG>(缓冲.size())));
                ULONG 已读 = 0;
                const HRESULT 读取结果 = 截图流.Read(缓冲.data(), 本次请求, &已读);
                if (FAILED(读取结果) || 已读 == 0) {
                    return false;
                }
                输出.write(缓冲.data(), static_cast<std::streamsize>(已读));
                if (!输出) {
                    return false;
                }
                剩余字节 -= 已读;
            }
            return true;
        }
        catch (...) {
            return false;
        }
    }

    // 功能：在自我场景 WebView2 UI 线程保存当前窗口 PNG 预览。
    void 私有_捕获自我场景窗口PNG(
        HWND 窗口,
        std::shared_ptr<结构_自我场景截图请求> 请求) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || 上下文->用途 != 枚举_WebView2窗口用途::自我场景 || !上下文->WebView) {
            私有_完成自我场景截图请求(
                请求,
                false,
                E_FAIL,
                ERROR_SUCCESS,
                "自我场景窗口未就绪");
            return;
        }

        ComPtr<IStream> 截图流{};
        const HRESULT 建流结果 = CreateStreamOnHGlobal(nullptr, TRUE, &截图流);
        if (FAILED(建流结果) || !截图流) {
            私有_完成自我场景截图请求(
                请求,
                false,
                FAILED(建流结果) ? 建流结果 : E_FAIL,
                GetLastError(),
                "创建截图内存流失败");
            return;
        }

        auto 回调 = Microsoft::WRL::Callback<ICoreWebView2CapturePreviewCompletedHandler>(
            [截图流, 请求](HRESULT 捕获结果) -> HRESULT {
                if (FAILED(捕获结果)) {
                    私有_完成自我场景截图请求(
                        请求,
                        false,
                        捕获结果,
                        ERROR_SUCCESS,
                        "CapturePreview失败");
                    return S_OK;
                }
                const bool 保存成功 = 请求
                    && 私有_保存自我场景截图流到文件(*截图流.Get(), 请求->输出路径);
                if (保存成功) {
                    项目运行日志(
                        "控制面板WebView2/自我场景截图已保存 | 路径="
                        + 路径UTF8文本(请求->输出路径));
                }
                私有_完成自我场景截图请求(
                    请求,
                    保存成功,
                    S_OK,
                    保存成功 ? ERROR_SUCCESS : GetLastError(),
                    保存成功 ? std::string{} : std::string("写入PNG文件失败"));
                return S_OK;
            });

        const HRESULT 捕获调用结果 = 上下文->WebView->CapturePreview(
            COREWEBVIEW2_CAPTURE_PREVIEW_IMAGE_FORMAT_PNG,
            截图流.Get(),
            回调.Get());
        if (FAILED(捕获调用结果)) {
            私有_完成自我场景截图请求(
                请求,
                false,
                捕获调用结果,
                GetLastError(),
                "CapturePreview调用失败");
        }
    }

    // 功能：把窗口客户区位图保存为 PNG 文件。
    bool 私有_保存窗口客户区PNG(
        HWND 窗口,
        const std::filesystem::path& 输出路径) noexcept
    {
        if (!窗口 || !IsWindow(窗口) || 输出路径.empty()) {
            私有_记录WebView2诊断("截图参数无效", 35);
            return false;
        }

        RECT 客户区{};
        if (!GetClientRect(窗口, &客户区)) {
            私有_记录WebView2诊断("截图读取客户区失败", 36, S_OK, GetLastError());
            return false;
        }

        const int 宽 = 客户区.right - 客户区.left;
        const int 高 = 客户区.bottom - 客户区.top;
        if (宽 <= 0 || 高 <= 0) {
            私有_记录WebView2诊断("截图客户区尺寸无效", 37);
            return false;
        }

        std::error_code 目录错误{};
        const auto 父目录 = 输出路径.parent_path();
        if (!父目录.empty()) {
            std::filesystem::create_directories(父目录, 目录错误);
            if (目录错误) {
                私有_记录WebView2诊断(
                    "截图创建目录失败",
                    38,
                    S_OK,
                    ERROR_SUCCESS,
                    "路径=" + 路径UTF8文本(父目录));
                return false;
            }
        }

        HDC 窗口DC = GetDC(窗口);
        if (!窗口DC) {
            私有_记录WebView2诊断("截图获取窗口DC失败", 39, S_OK, GetLastError());
            return false;
        }

        HDC 内存DC = CreateCompatibleDC(窗口DC);
        HBITMAP 位图 = 内存DC ? CreateCompatibleBitmap(窗口DC, 宽, 高) : nullptr;
        HGDIOBJ 原对象 = 位图 ? SelectObject(内存DC, 位图) : nullptr;
        const BOOL 已复制 = 位图
            ? (BitBlt(内存DC, 0, 0, 宽, 高, 窗口DC, 0, 0, SRCCOPY)
                || PrintWindow(窗口, 内存DC, PW_CLIENTONLY))
            : FALSE;
        if (原对象) {
            SelectObject(内存DC, 原对象);
        }
        if (内存DC) {
            DeleteDC(内存DC);
        }
        ReleaseDC(窗口, 窗口DC);

        if (!位图 || !已复制) {
            if (位图) {
                DeleteObject(位图);
            }
            私有_记录WebView2诊断("截图复制窗口像素失败", 40, S_OK, GetLastError());
            return false;
        }

        const HRESULT COM初始化 = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        const bool 本函数初始化COM = SUCCEEDED(COM初始化);
        if (FAILED(COM初始化) && COM初始化 != RPC_E_CHANGED_MODE) {
            DeleteObject(位图);
            私有_记录WebView2诊断("截图COM初始化失败", 41, COM初始化);
            return false;
        }

        bool 保存成功 = false;
        try {
            ComPtr<IWICImagingFactory> 工厂;
            HRESULT 结果 = CoCreateInstance(
                CLSID_WICImagingFactory,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&工厂));
            if (SUCCEEDED(结果)) {
                ComPtr<IWICBitmap> WIC位图;
                结果 = 工厂->CreateBitmapFromHBITMAP(
                    位图,
                    nullptr,
                    WICBitmapIgnoreAlpha,
                    &WIC位图);
                if (SUCCEEDED(结果)) {
                    ComPtr<IWICStream> 流;
                    结果 = 工厂->CreateStream(&流);
                    if (SUCCEEDED(结果)) {
                        const auto 宽路径 = 输出路径.wstring();
                        结果 = 流->InitializeFromFilename(宽路径.c_str(), GENERIC_WRITE);
                    }
                    ComPtr<IWICBitmapEncoder> 编码器;
                    if (SUCCEEDED(结果)) {
                        结果 = 工厂->CreateEncoder(GUID_ContainerFormatPng, nullptr, &编码器);
                    }
                    if (SUCCEEDED(结果)) {
                        结果 = 编码器->Initialize(流.Get(), WICBitmapEncoderNoCache);
                    }
                    ComPtr<IWICBitmapFrameEncode> 帧;
                    if (SUCCEEDED(结果)) {
                        结果 = 编码器->CreateNewFrame(&帧, nullptr);
                    }
                    if (SUCCEEDED(结果)) {
                        结果 = 帧->Initialize(nullptr);
                    }
                    if (SUCCEEDED(结果)) {
                        结果 = 帧->SetSize(static_cast<UINT>(宽), static_cast<UINT>(高));
                    }
                    WICPixelFormatGUID 像素格式 = GUID_WICPixelFormat32bppBGRA;
                    if (SUCCEEDED(结果)) {
                        结果 = 帧->SetPixelFormat(&像素格式);
                    }
                    if (SUCCEEDED(结果)) {
                        结果 = 帧->WriteSource(WIC位图.Get(), nullptr);
                    }
                    if (SUCCEEDED(结果)) {
                        结果 = 帧->Commit();
                    }
                    if (SUCCEEDED(结果)) {
                        结果 = 编码器->Commit();
                    }
                }
            }

            保存成功 = SUCCEEDED(结果);
            if (!保存成功) {
                私有_记录WebView2诊断(
                    "截图PNG编码失败",
                    42,
                    结果,
                    ERROR_SUCCESS,
                    "路径=" + 路径UTF8文本(输出路径));
            }
        }
        catch (...) {
            私有_记录WebView2诊断("截图PNG编码捕获未知异常", 43);
            保存成功 = false;
        }

        DeleteObject(位图);
        if (本函数初始化COM) {
            CoUninitialize();
        }
        return 保存成功;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_Base64编码(const std::vector<std::uint8_t>& 数据)
    {
        static constexpr char 字典[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string 输出;
        输出.reserve(((数据.size() + 2) / 3) * 4);

        std::uint32_t 缓冲 = 0;
        int 位数 = -6;
        for (const auto 字节 : 数据) {
            缓冲 = (缓冲 << 8) | 字节;
            位数 += 8;
            while (位数 >= 0) {
                输出.push_back(字典[(缓冲 >> 位数) & 0x3F]);
                位数 -= 6;
            }
        }
        if (位数 > -6) {
            输出.push_back(字典[((缓冲 << 8) >> (位数 + 8)) & 0x3F]);
        }
        while (输出.size() % 4 != 0) {
            输出.push_back('=');
        }
        return 输出;
    }

    // 功能：计算权重、状态、差值或派生结果。
    void 私有_计算相机显示尺寸(int 源宽, int 源高, int& 宽, int& 高) noexcept
    {
        constexpr int 最大宽 = 640;
        宽 = 0;
        高 = 0;
        if (源宽 <= 0 || 源高 <= 0) {
            return;
        }
        if (源宽 <= 最大宽) {
            宽 = 源宽;
            高 = 源高;
            return;
        }
        宽 = 最大宽;
        高 = std::max(1, static_cast<int>(
            (static_cast<long long>(源高) * 最大宽) / 源宽));
    }

    // 功能：服务所在模块的内部辅助流程。
    std::vector<std::uint8_t> 私有_缩放RGB(
        const std::vector<std::uint8_t>& 源,
        int 源宽,
        int 源高,
        int 宽,
        int 高)
    {
        std::vector<std::uint8_t> 输出(static_cast<std::size_t>(宽) * static_cast<std::size_t>(高) * 3, 0);
        const auto 源像素数 = static_cast<std::size_t>(源宽) * static_cast<std::size_t>(源高);
        if (源宽 <= 0
            || 源高 <= 0
            || 宽 <= 0
            || 高 <= 0
            || 源.size() != 源像素数 * 3) {
            return 输出;
        }
        for (int y = 0; y < 高; ++y) {
            const int sy = std::min(源高 - 1, static_cast<int>(
                (static_cast<long long>(y) * 源高) / 高));
            for (int x = 0; x < 宽; ++x) {
                const int sx = std::min(源宽 - 1, static_cast<int>(
                    (static_cast<long long>(x) * 源宽) / 宽));
                const auto 源索引 = (static_cast<std::size_t>(sy) * static_cast<std::size_t>(源宽)
                    + static_cast<std::size_t>(sx)) * 3;
                const auto 目标索引 = (static_cast<std::size_t>(y) * static_cast<std::size_t>(宽)
                    + static_cast<std::size_t>(x)) * 3;
                输出[目标索引] = 源[源索引];
                输出[目标索引 + 1] = 源[源索引 + 1];
                输出[目标索引 + 2] = 源[源索引 + 2];
            }
        }
        return 输出;
    }

    // 功能：确保目标结构、状态或前置条件存在并可用。
    bool 私有_确保相机播放外设线程(std::string& 消息) noexcept
    {
        try {
            auto& 线程 = 获取全局外设线程_D455深度相机();
            if (线程.是否运行中()) {
                消息 = "正在读取外设线程视频材料";
                return true;
            }

            线程.等待停止();
            结构_D455深度相机线程配置 配置{};
            配置.启用轻量观察报告 = false;
            const bool 已启动 = 启动外设线程_D455深度相机(配置);
            if (已启动) {
                私有_相机窗口启动过外设线程.store(true);
                消息 = "D455外设线程已启动，等待视频材料";
                return true;
            }

            消息 = "D455外设线程启动失败";
            return false;
        }
        catch (const std::exception& e) {
            消息 = std::string("D455外设线程启动异常: ") + e.what();
            return false;
        }
        catch (...) {
            消息 = "D455外设线程启动发生未知异常";
            return false;
        }
    }

    // 功能：构建运行所需的数据结构或中间结果。
    结构_相机帧JSON 私有_构建相机帧JSON数据(const 结构_D455控制面板视频快照& 快照)
    {
        结构_相机帧JSON 输出{};
        if (!快照.成功) {
            输出.消息 = 快照.失败原因.empty()
                ? "D455外设线程视频材料不可用"
                : 快照.失败原因;
            return 输出;
        }

        输出.成功 = true;
        输出.源宽 = static_cast<int>(快照.帧宽度);
        输出.源高 = static_cast<int>(快照.帧高度);
        输出.深度帧号 = static_cast<std::uint32_t>(快照.深度帧号);
        输出.彩色帧号 = static_cast<std::uint32_t>(快照.彩色帧号);
        输出.分割簇数 = 快照.分割簇数量;
        输出.分割像素数 = 快照.分割像素数量;
        输出.轮廓线像素数 = 快照.轮廓线像素数量;
        输出.消息 = "已更新";

        私有_计算相机显示尺寸(输出.源宽, 输出.源高, 输出.宽, 输出.高);
        const auto RGB = 私有_缩放RGB(快照.颜色RGB, 输出.源宽, 输出.源高, 输出.宽, 输出.高);
        const auto 分割RGB = 私有_缩放RGB(快照.分割RGB, 输出.源宽, 输出.源高, 输出.宽, 输出.高);
        输出.彩色RGB_Base64 = 私有_Base64编码(RGB);
        输出.分割RGB_Base64 = 私有_Base64编码(分割RGB);
        return 输出;
    }

    // 功能：从指定来源读取数据或状态。
    结构_相机帧JSON 私有_读取相机帧(bool 确保启动)
    {
        try {
            std::string 外设线程消息;
            if (确保启动 && !私有_确保相机播放外设线程(外设线程消息)) {
                结构_相机帧JSON 输出{};
                输出.消息 = 外设线程消息;
                return 输出;
            }

            auto 输出 = 私有_构建相机帧JSON数据(读取最新D455控制面板视频快照());
            if (!输出.成功 && !输出.已停止) {
                输出.等待中 = 确保启动;
                if (!外设线程消息.empty()) {
                    输出.消息 = 外设线程消息 + "；" + 输出.消息;
                }
            }
            return 输出;
        }
        catch (const std::exception& e) {
            项目运行错误日志(std::string("控制面板相机画面/读取外设线程视频异常 | 原因=") + e.what());
            结构_相机帧JSON 输出{};
            输出.消息 = "相机视频材料读取异常";
            return 输出;
        }
        catch (...) {
            项目运行错误日志("控制面板相机画面/读取外设线程视频未知异常");
            结构_相机帧JSON 输出{};
            输出.消息 = "相机视频材料读取未知异常";
            return 输出;
        }
    }

    // 功能：停止线程、模块或运行流程。
    结构_相机帧JSON 私有_停止相机播放()
    {
        结构_相机帧JSON 输出{};
        输出.已停止 = true;
        输出.消息 = "已停止播放";
        if (私有_相机窗口启动过外设线程.exchange(false)) {
            const auto 队列状态 = 读取外设观察队列状态();
            if (队列状态.等待项数量 == 0) {
                停止外设线程_D455深度相机();
                输出.消息 = "已停止播放，并停止控制面板启动的D455外设线程";
            }
            else {
                输出.消息 = "已停止播放；D455外设线程仍有等待项，未停止";
            }
        }
        return 输出;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_相机帧JSON文本(const 结构_相机帧JSON& 帧)
    {
        std::ostringstream 输出;
        输出 << "{";
        输出 << "\"ok\":" << (帧.成功 ? "true" : "false") << ",";
        输出 << "\"stopped\":" << (帧.已停止 ? "true" : "false") << ",";
        输出 << "\"waiting\":" << (帧.等待中 ? "true" : "false") << ",";
        输出 << "\"sourceWidth\":" << 帧.源宽 << ",";
        输出 << "\"sourceHeight\":" << 帧.源高 << ",";
        输出 << "\"width\":" << 帧.宽 << ",";
        输出 << "\"height\":" << 帧.高 << ",";
        输出 << "\"depthFrame\":" << 帧.深度帧号 << ",";
        输出 << "\"colorFrame\":" << 帧.彩色帧号 << ",";
        输出 << "\"deviceTimeUs\":" << 帧.设备时间_us << ",";
        输出 << "\"segmentCount\":" << 帧.分割簇数 << ",";
        输出 << "\"segmentPixels\":" << 帧.分割像素数 << ",";
        输出 << "\"contourPixels\":" << 帧.轮廓线像素数 << ",";
        输出 << "\"message\":";
        追加JSON字符串(输出, 帧.消息);
        输出 << ",";
        输出 << "\"error\":";
        追加JSON字符串(输出, 帧.成功 || 帧.已停止 || 帧.等待中 ? std::string_view{} : std::string_view(帧.消息));
        输出 << ",";
        输出 << "\"colorRGB\":";
        追加JSON字符串(输出, 帧.彩色RGB_Base64);
        输出 << ",";
        输出 << "\"segmentationRGB\":";
        追加JSON字符串(输出, 帧.分割RGB_Base64);
        输出 << "}";
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_发送相机帧到页面(HWND 窗口, const 结构_相机帧JSON& 帧) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }
        const auto JSON = 私有_相机帧JSON文本(帧);
        const auto 宽JSON = 私有_UTF8转宽字串(JSON);
        if (宽JSON.empty()) {
            return;
        }
        const std::wstring 脚本 = L"window.__panelApplyCameraVideoFrame(" + 宽JSON + L");";
        (void)上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_发送相机窗口状态到页面(HWND 窗口, bool 成功, std::string_view 消息) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        std::ostringstream JSON;
        JSON << "{\"ok\":" << (成功 ? "true" : "false") << ",\"message\":";
        追加JSON字符串(JSON, 消息);
        JSON << "}";
        const auto 宽JSON = 私有_UTF8转宽字串(JSON.str());
        if (宽JSON.empty()) {
            return;
        }

        const std::wstring 脚本 = L"window.__panelApplyCameraWindowState(" + 宽JSON + L");";
        (void)上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_发送自我场景窗口状态到页面(HWND 窗口, bool 成功, std::string_view 消息) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        std::ostringstream JSON;
        JSON << "{\"ok\":" << (成功 ? "true" : "false") << ",\"message\":";
        追加JSON字符串(JSON, 消息);
        JSON << "}";
        const auto 宽JSON = 私有_UTF8转宽字串(JSON.str());
        if (宽JSON.empty()) {
            return;
        }

        const std::wstring 脚本 = L"window.__panelApplySceneWindowState(" + 宽JSON + L");";
        (void)上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
    }

    // 功能：生成自我场景增量刷新轻量返回帧。
    std::string 私有_生成自我场景无变化帧JSON(
        const bool 已节流,
        std::string_view 消息)
    {
        std::ostringstream JSON;
        JSON << "{\"ok\":true,\"unchanged\":true";
        JSON << ",\"throttled\":" << (已节流 ? "true" : "false");
        JSON << ",\"message\":";
        追加JSON字符串(JSON, 消息);
        JSON << "}";
        return JSON.str();
    }

    // 功能：把自我场景帧 JSON 推送到页面脚本。
    void 私有_执行自我场景帧脚本(
        结构_WebView2窗口上下文& 上下文,
        const std::string& JSON) noexcept
    {
        const auto 宽JSON = 私有_UTF8转宽字串(JSON);
        if (宽JSON.empty()) {
            return;
        }

        const std::wstring 脚本 = L"window.__panelApplySelfSceneFrame(" + 宽JSON + L");";
        (void)上下文.WebView->ExecuteScript(脚本.c_str(), nullptr);
    }

    // 功能：向自我场景独立窗口推送只读场景复现帧。
    void 私有_发送自我场景帧到页面(HWND 窗口) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        私有_自我场景待刷新请求数.fetch_add(1, std::memory_order_relaxed);
        std::string JSON{};
        std::unique_lock<std::mutex> 帧锁(私有_自我场景帧缓存互斥, std::try_to_lock);
        if (!帧锁.owns_lock()) {
            JSON = 私有_生成自我场景无变化帧JSON(true, "已记录自我场景刷新请求，等待3秒集中刷新。");
            私有_执行自我场景帧脚本(*上下文, JSON);
            return;
        }

        const auto 当前时间 = std::chrono::steady_clock::now();
        if (!私有_自我场景最近帧JSON.empty()
            && 私有_自我场景最近集中刷新时间.time_since_epoch().count() > 0
            && 当前时间 - 私有_自我场景最近集中刷新时间 < 私有_自我场景帧最小刷新间隔) {
            JSON = 私有_生成自我场景无变化帧JSON(true, "已记录自我场景刷新请求，等待3秒集中刷新。");
            帧锁.unlock();
            私有_执行自我场景帧脚本(*上下文, JSON);
            return;
        }

        if (!自我.已初始化()) {
            (void)初始化自我环境();
        }

        const auto 快照 = 读取控制面板快照(10, 24);
        const auto 当前签名 = 生成自我场景复现签名(快照);
        const auto 本批请求数 = 私有_自我场景待刷新请求数.exchange(0, std::memory_order_relaxed);
        私有_自我场景最近集中刷新时间 = 当前时间;
        if (!私有_自我场景最近帧JSON.empty()
            && 当前签名 == 私有_自我场景最近帧签名) {
            JSON = 私有_生成自我场景无变化帧JSON(
                false,
                std::string("3秒集中刷新完成，本批记录")
                + std::to_string(本批请求数)
                + "次刷新请求，画面无变化。");
        }
        else {
            JSON = 生成自我场景复现JSON(快照);
            私有_自我场景最近帧签名 = 当前签名;
            私有_自我场景最近帧JSON = JSON;
        }
        帧锁.unlock();
        私有_执行自我场景帧脚本(*上下文, JSON);
    }

    // 功能：把页面刷新 JSON 投递回 WebView2 UI 线程执行。
    void 私有_执行页面刷新JSON到页面(
        HWND 窗口,
        const 结构_页面刷新投递& 投递) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        const auto 宽JSON = 私有_UTF8转宽字串(投递.JSON);
        if (宽JSON.empty()) {
            私有_记录WebView2诊断(
                "页面刷新JSON转宽字串失败",
                48,
                S_OK,
                GetLastError(),
                "页面=" + 投递.页面
                + " | JSON字节=" + std::to_string(投递.JSON.size())
                + " | 生成耗时ms=" + std::to_string(投递.生成耗时毫秒));
            return;
        }

        const std::wstring 脚本 = L"window.__panelApplyPageRefresh(" + 宽JSON + L");";
        const HRESULT 脚本结果 = 上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
        if (FAILED(脚本结果)) {
            私有_记录WebView2诊断(
                "页面刷新脚本执行失败",
                49,
                脚本结果,
                GetLastError(),
                "页面=" + 投递.页面
                + " | JSON字节=" + std::to_string(投递.JSON.size())
                + " | 生成耗时ms=" + std::to_string(投递.生成耗时毫秒));
            return;
        }
        if (投递.生成耗时毫秒 > 500) {
            项目运行日志(
                "控制面板WebView2/页面刷新较慢 | 页面="
                + 投递.页面
                + " | JSON字节=" + std::to_string(投递.JSON.size())
                + " | 生成耗时ms=" + std::to_string(投递.生成耗时毫秒));
        }
    }

    // 功能：异步读取页面刷新 JSON，避免 WebView2 UI 线程被 SQL 查询阻塞。
    void 私有_发送页面刷新到页面(HWND 窗口, std::string_view 页面) noexcept
    {
        const std::string 页面文本(页面);
        std::thread([窗口, 页面文本]() noexcept {
            std::unique_ptr<结构_页面刷新投递> 投递{ new(std::nothrow) 结构_页面刷新投递{} };
            if (!投递) {
                私有_记录WebView2诊断(
                    "页面刷新JSON投递分配失败",
                    50,
                    S_OK,
                    ERROR_OUTOFMEMORY,
                    "页面=" + 页面文本);
                return;
            }
            投递->页面 = 页面文本;
            const auto 开始时间 = std::chrono::steady_clock::now();
            try {
                投递->JSON = 读取控制面板页面刷新JSON(页面文本);
            }
            catch (const std::exception& 异常) {
                std::ostringstream JSON;
                JSON << "{\"ok\":false,\"kind\":\"sql-section\",\"page\":";
                追加JSON字符串(JSON, 页面文本);
                JSON << ",\"error\":";
                追加JSON字符串(JSON, std::string("页面刷新异常 | 原因=") + 异常.what());
                JSON << "}";
                投递->JSON = JSON.str();
            }
            catch (...) {
                std::ostringstream JSON;
                JSON << "{\"ok\":false,\"kind\":\"sql-section\",\"page\":";
                追加JSON字符串(JSON, 页面文本);
                JSON << ",\"error\":\"页面刷新异常 | 原因=未知异常\"}";
                投递->JSON = JSON.str();
            }
            投递->生成耗时毫秒 = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - 开始时间).count();
            if (!IsWindow(窗口)
                || !PostMessageW(
                    窗口,
                    私有_WM_页面刷新JSON,
                    0,
                    reinterpret_cast<LPARAM>(投递.get()))) {
                私有_记录WebView2诊断(
                    "页面刷新JSON投递失败",
                    50,
                    S_OK,
                    GetLastError(),
                    "页面=" + 页面文本
                    + " | JSON字节=" + std::to_string(投递->JSON.size())
                    + " | 生成耗时ms=" + std::to_string(投递->生成耗时毫秒));
                return;
            }
            (void)投递.release();
        }).detach();
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_调整WebView尺寸(HWND 窗口) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->控制器) {
            return;
        }

        RECT 区域{};
        GetClientRect(窗口, &区域);
        上下文->控制器->put_Bounds(区域);
    }

    // 功能：根据当前输入生成目标数据、场景、动态或回执。
    std::string 私有_生成相机播放HTML()
    {
        return R"CAMERA(<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>鱼巢相机画面</title>
  <style>
    :root{
      --bg:#0b1116;
      --surface:#111923;
      --surface-2:#162230;
      --line:#2c3a49;
      --ink:#edf2f7;
      --muted:#9aa8b8;
      --accent:#10b981;
      --danger:#fb7185;
    }
    *{box-sizing:border-box}
    body{
      margin:0;
      min-height:100vh;
      color:var(--ink);
      font-family:"Microsoft YaHei UI","PingFang SC","Source Han Sans SC",sans-serif;
      background:var(--bg);
    }
    .shell{
      min-height:100vh;
      display:grid;
      grid-template-rows:auto minmax(0,1fr) auto;
      gap:14px;
      padding:16px;
    }
    .toolbar{
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:16px;
      padding:14px 16px;
      border:1px solid var(--line);
      border-radius:8px;
      background:var(--surface);
    }
    .title{
      min-width:0;
      display:grid;
      gap:6px;
    }
    .title strong{
      font-size:22px;
      line-height:1.25;
    }
    .status{
      color:var(--muted);
      font-size:13px;
      line-height:1.6;
      word-break:break-word;
    }
    .status.ok{color:var(--accent);font-weight:700}
    .status.error{color:var(--danger);font-weight:700}
    .actions{
      display:flex;
      gap:10px;
      flex-wrap:wrap;
      justify-content:flex-end;
    }
    button{
      min-height:40px;
      padding:0 14px;
      border:none;
      border-radius:8px;
      cursor:pointer;
      color:#fff;
      background:var(--accent);
      font-weight:700;
      white-space:nowrap;
    }
    button.secondary{
      color:var(--ink);
      border:1px solid var(--line);
      background:var(--surface-2);
    }
    button.danger{background:#be123c}
    .viewer-grid{
      min-height:0;
      display:grid;
      grid-template-columns:repeat(2,minmax(0,1fr));
      gap:14px;
    }
    .viewer{
      min-width:0;
      min-height:0;
      display:grid;
      grid-template-rows:auto minmax(0,1fr);
      gap:10px;
      padding:14px;
      border:1px solid var(--line);
      border-radius:8px;
      background:var(--surface);
    }
    .viewer-head{
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:12px;
    }
    .viewer-title{
      font-size:16px;
      font-weight:800;
      line-height:1.4;
    }
    .viewer-meta{
      color:var(--muted);
      font-size:12px;
      line-height:1.5;
      text-align:right;
      white-space:nowrap;
    }
    .canvas-shell{
      min-height:0;
      border:1px solid #223040;
      border-radius:8px;
      overflow:hidden;
      background:#05080c;
      display:flex;
      align-items:center;
      justify-content:center;
    }
    canvas{
      width:100%;
      height:100%;
      display:block;
      object-fit:contain;
      background:#05080c;
    }
    .stat-grid{
      display:grid;
      grid-template-columns:repeat(4,minmax(0,1fr));
      gap:10px;
    }
    .stat{
      min-width:0;
      padding:11px 13px;
      border:1px solid var(--line);
      border-radius:8px;
      background:var(--surface);
    }
    .stat-label{
      color:var(--muted);
      font-size:12px;
      line-height:1.5;
    }
    .stat-value{
      margin-top:5px;
      font-size:15px;
      font-weight:800;
      line-height:1.4;
      word-break:break-word;
    }
    @media (max-width:980px){
      .toolbar{align-items:flex-start;flex-direction:column}
      .actions{width:100%;justify-content:flex-start}
      button{flex:1 1 120px}
      .viewer-grid{grid-template-columns:1fr}
      .stat-grid{grid-template-columns:repeat(2,minmax(0,1fr))}
    }
    @media (max-width:560px){
      .stat-grid{grid-template-columns:1fr}
    }
  </style>
</head>
<body>
  <main class="shell">
    <header class="toolbar">
      <div class="title">
        <strong>D455 相机画面</strong>
        <div id="camera-status" class="status" role="status">正在启动相机...</div>
      </div>
      <div class="actions">
        <button id="camera-start" type="button">启动</button>
        <button id="camera-capture" class="secondary" type="button">刷新一帧</button>
        <button id="camera-stop" class="danger" type="button">停止</button>
      </div>
    </header>
    <section class="viewer-grid">
      <section class="viewer">
        <div class="viewer-head">
          <div class="viewer-title">RGB</div>
          <div id="camera-rgb-meta" class="viewer-meta">--</div>
        </div>
        <div class="canvas-shell">
          <canvas id="camera-rgb-canvas" width="640" height="480"></canvas>
        </div>
      </section>
      <section class="viewer">
        <div class="viewer-head">
          <div class="viewer-title">分割完成</div>
          <div id="camera-contour-meta" class="viewer-meta">--</div>
        </div>
        <div class="canvas-shell">
          <canvas id="camera-contour-canvas" width="640" height="480"></canvas>
        </div>
      </section>
    </section>
    <section class="stat-grid" aria-label="相机帧状态">
      <div class="stat">
        <div class="stat-label">帧尺寸</div>
        <div id="camera-size-stat" class="stat-value">--</div>
      </div>
      <div class="stat">
        <div class="stat-label">源尺寸</div>
        <div id="camera-source-stat" class="stat-value">--</div>
      </div>
      <div class="stat">
        <div class="stat-label">帧号</div>
        <div id="camera-frame-stat" class="stat-value">--</div>
      </div>
      <div class="stat">
        <div class="stat-label">分割簇数</div>
        <div id="camera-contour-stat" class="stat-value">--</div>
      </div>
    </section>
  </main>
  <script>
    let 相机自动采集句柄 = 0;
    let 相机请求中 = false;

    function 设置相机状态(text, kind = '') {
      const status = document.getElementById('camera-status');
      if (!status) return;
      status.textContent = text || '';
      status.classList.toggle('ok', kind === 'ok');
      status.classList.toggle('error', kind === 'error');
    }

    function 解码Base64字节(text) {
      const bin = atob(text || '');
      const bytes = new Uint8Array(bin.length);
      for (let i = 0; i < bin.length; ++i) {
        bytes[i] = bin.charCodeAt(i);
      }
      return bytes;
    }

    function 清空相机画布(canvasId) {
      const canvas = document.getElementById(canvasId);
      if (!canvas) return;
      const ctx = canvas.getContext('2d');
      ctx.fillStyle = '#05080c';
      ctx.fillRect(0, 0, canvas.width, canvas.height);
    }

    function 绘制RGB画面(data) {
      const canvas = document.getElementById('camera-rgb-canvas');
      if (!canvas || !data || !data.colorRGB) return;
      const w = Number(data.width || 0);
      const h = Number(data.height || 0);
      if (w <= 0 || h <= 0) return;
      canvas.width = w;
      canvas.height = h;
      const bytes = 解码Base64字节(data.colorRGB);
      const ctx = canvas.getContext('2d');
      const image = ctx.createImageData(w, h);
      for (let i = 0, j = 0; i < bytes.length && j < image.data.length; i += 3, j += 4) {
        image.data[j] = bytes[i] || 0;
        image.data[j + 1] = bytes[i + 1] || 0;
        image.data[j + 2] = bytes[i + 2] || 0;
        image.data[j + 3] = 255;
      }
      ctx.putImageData(image, 0, 0);
    }

    function 绘制分割完成画面(data) {
      const canvas = document.getElementById('camera-contour-canvas');
      if (!canvas || !data || !data.segmentationRGB) return;
      const w = Number(data.width || 0);
      const h = Number(data.height || 0);
      if (w <= 0 || h <= 0) return;
      canvas.width = w;
      canvas.height = h;
      const rgb = 解码Base64字节(data.segmentationRGB);
      const ctx = canvas.getContext('2d');
      const image = ctx.createImageData(w, h);
      const pixelCount = Math.min(w * h, Math.floor(rgb.length / 3));
      for (let i = 0, j = 0; i < pixelCount && j < image.data.length; ++i, j += 4) {
        const k = i * 3;
        image.data[j] = rgb[k] || 0;
        image.data[j + 1] = rgb[k + 1] || 0;
        image.data[j + 2] = rgb[k + 2] || 0;
        image.data[j + 3] = 255;
      }
      ctx.putImageData(image, 0, 0);
    }

    function 更新相机统计(data) {
      const setText = (id, text) => {
        const node = document.getElementById(id);
        if (node) node.textContent = text;
      };
      setText('camera-size-stat', `${data.width || 0} x ${data.height || 0}`);
      setText('camera-source-stat', `${data.sourceWidth || 0} x ${data.sourceHeight || 0}`);
      setText('camera-frame-stat', `D${data.depthFrame || 0} / C${data.colorFrame || 0}`);
      setText('camera-contour-stat', String(data.segmentCount || 0));
      setText('camera-rgb-meta', `${data.width || 0} x ${data.height || 0}`);
      setText('camera-contour-meta', `${data.segmentCount || 0} 个分割簇 / 轮廓线 ${data.contourPixels || 0} 像素`);
    }

    function 发送相机消息(message) {
      if (!(window.chrome && window.chrome.webview)) {
        设置相机状态('WebView2 相机接口未连接。', 'error');
        return false;
      }
      window.chrome.webview.postMessage(message);
      return true;
    }

    function 请求相机帧(message = 'camera:capture') {
      if (相机请求中) return;
      相机请求中 = true;
      设置相机状态('正在读取相机帧...');
      if (!发送相机消息(message)) {
        相机请求中 = false;
      }
    }

    function 启动相机采集() {
      if (相机自动采集句柄) {
        clearInterval(相机自动采集句柄);
      }
      请求相机帧('camera:start');
      相机自动采集句柄 = window.setInterval(() => 请求相机帧('camera:capture'), 16);
    }

    function 停止相机采集() {
      if (相机自动采集句柄) {
        clearInterval(相机自动采集句柄);
        相机自动采集句柄 = 0;
      }
      相机请求中 = false;
      发送相机消息('camera:stop');
    }

    window.__panelApplyCameraVideoFrame = function(data) {
      相机请求中 = false;
      if (!data || typeof data !== 'object') {
        设置相机状态('相机返回数据无效。', 'error');
        return;
      }
      if (data.stopped) {
        设置相机状态(data.message || '已停止。');
        return;
      }
      if (data.waiting) {
        设置相机状态(data.message || '正在等待外设线程视频材料。');
        return;
      }
      if (!data.ok) {
        设置相机状态(data.error || '相机采集失败。', 'error');
        return;
      }
      绘制RGB画面(data);
      绘制分割完成画面(data);
      更新相机统计(data);
      设置相机状态(data.message || '已更新。', 'ok');
    };

    const 相机启动按钮 = document.getElementById('camera-start');
    const 相机刷新按钮 = document.getElementById('camera-capture');
    const 相机停止按钮 = document.getElementById('camera-stop');
    if (相机启动按钮) {
      相机启动按钮.addEventListener('click', 启动相机采集);
    }
    if (相机刷新按钮) {
      相机刷新按钮.addEventListener('click', () => 请求相机帧('camera:capture'));
    }
    if (相机停止按钮) {
      相机停止按钮.addEventListener('click', 停止相机采集);
    }
    window.addEventListener('beforeunload', () => {
      if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage('camera:stop');
      }
    });

    清空相机画布('camera-rgb-canvas');
    清空相机画布('camera-contour-canvas');
    window.setTimeout(启动相机采集, 150);
  </script>
</body>
</html>)CAMERA";
    }

    // 功能：根据当前输入生成目标数据、场景、动态或回执。
    std::string 私有_生成页面HTML(枚举_WebView2窗口用途 用途)
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            return 私有_生成相机播放HTML();
        }

        if (用途 == 枚举_WebView2窗口用途::自我场景) {
            if (!自我.已初始化()) {
                (void)初始化自我环境();
            }
            const auto 快照 = 读取控制面板快照(10, 24);
            return 生成自我场景独立窗口HTML(快照);
        }
        return 生成SQL控制面板启动HTML();
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_刷新页面(HWND 窗口) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        const auto HTML = 私有_生成页面HTML(上下文->用途);
        if (HTML.empty()) {
            私有_记录WebView2诊断("刷新页面HTML为空", 30);
            return;
        }
        if (HTML.size() > 私有_NavigateToString安全字节数) {
            (void)私有_用临时HTML文件导航(窗口, *上下文, HTML);
            return;
        }

        const auto 宽HTML = 私有_UTF8转宽字串(HTML);
        if (宽HTML.empty()) {
            私有_记录WebView2诊断(
                "刷新页面HTML转宽字串失败",
                35,
                S_OK,
                GetLastError(),
                "HTML字节=" + std::to_string(HTML.size()));
            return;
        }

        const HRESULT 导航结果 = 上下文->WebView->NavigateToString(宽HTML.c_str());
        if (FAILED(导航结果)) {
            私有_记录WebView2诊断(
                "NavigateToString失败转临时HTML",
                36,
                导航结果,
                GetLastError(),
                "HTML字节=" + std::to_string(HTML.size()));
            (void)私有_用临时HTML文件导航(窗口, *上下文, HTML);
        }
    }

    // 功能：初始化相关对象、状态或运行上下文。
    bool 私有_初始化WebView2(HWND 窗口) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文) {
            私有_启动诊断码.store(7);
            私有_记录WebView2诊断("初始化缺少窗口上下文", 7);
            return false;
        }

        auto* 创建环境 = 私有_加载创建函数(&上下文->加载器模块);
        if (!创建环境) {
            私有_启动诊断码.store(8);
            私有_记录WebView2诊断("初始化缺少创建环境函数", 8);
            return false;
        }

        const auto 用户数据目录 = (私有_模块目录() / L"webview2-user-data").lexically_normal();
        const HRESULT 结果 = 创建环境(
            nullptr,
            用户数据目录.c_str(),
            nullptr,
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [窗口](HRESULT 环境结果, ICoreWebView2Environment* 环境) -> HRESULT {
                    if (FAILED(环境结果) || !环境) {
                        私有_启动诊断码.store(9);
                        私有_记录WebView2诊断(
                            "创建环境回调失败",
                            9,
                            FAILED(环境结果) ? 环境结果 : E_FAIL);
                        return FAILED(环境结果) ? 环境结果 : E_FAIL;
                    }

                    return 环境->CreateCoreWebView2Controller(
                        窗口,
                        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                            [窗口](HRESULT 控制器结果, ICoreWebView2Controller* 控制器) -> HRESULT {
                                auto* 上下文 = 私有_取窗口上下文(窗口);
                                if (!上下文 || FAILED(控制器结果) || !控制器) {
                                    私有_启动诊断码.store(10);
                                    私有_记录WebView2诊断(
                                        "创建控制器回调失败",
                                        10,
                                        FAILED(控制器结果) ? 控制器结果 : E_FAIL,
                                        ERROR_SUCCESS,
                                        上下文 ? std::string{} : std::string("窗口上下文为空"));
                                    return FAILED(控制器结果) ? 控制器结果 : E_FAIL;
                                }

                                上下文->控制器 = 控制器;
                                上下文->控制器->get_CoreWebView2(&上下文->WebView);
                                if (!上下文->WebView) {
                                    私有_启动诊断码.store(11);
                                    私有_记录WebView2诊断("控制器未返回WebView", 11, E_FAIL);
                                    return E_FAIL;
                                }

                                ComPtr<ICoreWebView2Settings> 设置{};
                                if (SUCCEEDED(上下文->WebView->get_Settings(&设置)) && 设置) {
                                    (void)设置->put_AreDefaultContextMenusEnabled(TRUE);
                                    (void)设置->put_AreDevToolsEnabled(FALSE);
                                    (void)设置->put_IsStatusBarEnabled(FALSE);
                                }

                                (void)上下文->WebView->add_WebMessageReceived(
                                    Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                        [窗口](ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs* 参数) -> HRESULT {
                                            if (!参数) {
                                                return S_OK;
                                            }

                                            LPWSTR 消息文本 = nullptr;
                                            if (FAILED(参数->TryGetWebMessageAsString(&消息文本)) || !消息文本) {
                                                return S_OK;
                                            }

                                            const std::wstring 消息 = 消息文本;
                                            CoTaskMemFree(消息文本);
                                            if (消息 == L"refresh") {
                                                PostMessageW(窗口, 私有_WM_刷新控制面板窗口, 0, 0);
                                                return S_OK;
                                            }
                                            constexpr std::wstring_view 页面刷新前缀 = L"refresh-page:";
                                            if (消息.starts_with(页面刷新前缀)) {
                                                const auto 页面 = 私有_宽字串转UTF8(
                                                    消息.substr(页面刷新前缀.size()));
                                                const auto 是SQL区段 = [](std::string_view 区段) noexcept {
                                                    return 区段 == "metrics"
                                                        || 区段 == "threads"
                                                        || 区段 == "threadEvents"
                                                        || 区段 == "actions"
                                                        || 区段 == "causalInfo"
                                                        || 区段 == "causalChain"
                                                        || 区段 == "demandTree"
                                                        || 区段 == "need-tree"
                                                        || 区段 == "need-list"
                                                        || 区段 == "taskTree"
                                                        || 区段 == "task-tree"
                                                        || 区段 == "methodTree"
                                                        || 区段 == "method-tree"
                                                        || 区段 == "worldTree"
                                                        || 区段 == "world-tree"
                                                        || 区段 == "worldRelations"
                                                        || 区段 == "causal-info"
                                                        || 区段 == "lexemeTree"
                                                        || 区段 == "features"
                                                        || 区段 == "catalog";
                                                };
                                                if (auto* 上下文 = 私有_取窗口上下文(窗口);
                                                    上下文 && 上下文->用途 == 枚举_WebView2窗口用途::控制面板 && !是SQL区段(页面)) {
                                                    私有_记录WebView2诊断(
                                                        "SQL控制面板拒绝live页面刷新消息",
                                                        47,
                                                        S_OK,
                                                        ERROR_SUCCESS,
                                                        "页面=" + 页面);
                                                    return S_OK;
                                                }
                                                私有_发送页面刷新到页面(窗口, 页面);
                                                return S_OK;
                                            }
                                            if (消息 == L"camera:open-window") {
                                                const bool 成功 = 私有_打开相机播放窗口(窗口);
                                                私有_发送相机窗口状态到页面(
                                                    窗口,
                                                    成功,
                                                    成功 ? "独立播放窗口启动请求已发送。" : "独立播放窗口启动请求失败。");
                                                return S_OK;
                                            }
                                            if (消息 == L"scene:open-window") {
                                                const bool 成功 = 私有_打开自我场景窗口(窗口);
                                                私有_发送自我场景窗口状态到页面(
                                                    窗口,
                                                    成功,
                                                    成功 ? "独立场景窗口启动请求已发送。" : "独立场景窗口启动请求失败。");
                                                return S_OK;
                                            }
                                            if (消息 == L"scene:refresh") {
                                                私有_发送自我场景帧到页面(窗口);
                                                return S_OK;
                                            }
                                            if (消息 == L"camera:start" || 消息 == L"camera:capture") {
                                                私有_发送相机帧到页面(窗口, 私有_读取相机帧(true));
                                                return S_OK;
                                            }
                                            if (消息 == L"camera:stop") {
                                                私有_发送相机帧到页面(窗口, 私有_停止相机播放());
                                                return S_OK;
                                            }

                                            std::uint64_t worker数量 = 0;
                                            if (私有_解析线程池大小设置消息(消息, &worker数量)) {
                                                if (worker数量 > static_cast<std::uint64_t>(
                                                    std::numeric_limits<std::size_t>::max())) {
                                                    私有_记录WebView2诊断(
                                                        "参数设置超出本机size_t范围",
                                                        14,
                                                        S_OK,
                                                        ERROR_SUCCESS,
                                                        "worker数=" + std::to_string(worker数量));
                                                    私有_发送页面刷新到页面(窗口, "settings");
                                                    return S_OK;
                                                }
                                                (void)任务管理界面线程::保存任务管理工作线程池大小(
                                                    自我,
                                                    static_cast<std::size_t>(worker数量));
                                                私有_发送页面刷新到页面(窗口, "settings");
                                                return S_OK;
                                            }

                                            std::uint64_t 请求号 = 0;
                                            std::uintptr_t 节点指针 = 0;
                                            std::uintptr_t 附加参数 = 0;
                                            std::string 展开类型{};
                                            std::string SQL区段ID{};
                                            std::string SQL节点键{};
                                            if (私有_解析SQL子链消息(消息, &请求号, &SQL区段ID, &SQL节点键)) {
                                                auto* 上下文 = 私有_取窗口上下文(窗口);
                                                if (上下文 && 上下文->WebView) {
                                                    const auto 子链JSON = 读取SQL控制面板子链JSON(SQL区段ID, SQL节点键);
                                                    const auto 宽子链JSON = 私有_UTF8转宽字串(子链JSON);
                                                    std::wstring 脚本 =
                                                        L"window.__panelApplySQLSubtree("
                                                        + std::to_wstring(请求号)
                                                        + L", "
                                                        + 宽子链JSON
                                                        + L");";
                                                    (void)上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
                                                }
                                                return S_OK;
                                            }
                                            if (私有_解析展开消息(消息, &请求号, &展开类型, &节点指针, &附加参数)) {
                                                auto* 上下文 = 私有_取窗口上下文(窗口);
                                                if (上下文 && 上下文->用途 == 枚举_WebView2窗口用途::控制面板) {
                                                    私有_记录WebView2诊断("SQL控制面板拒绝live展开消息", 45);
                                                    return S_OK;
                                                }
                                                if (上下文 && 上下文->WebView) {
                                                    const auto 子项JSON = 读取控制面板节点子项JSON(展开类型, 节点指针, 16, 附加参数);
                                                    const auto 宽子项JSON = 私有_UTF8转宽字串(子项JSON);
                                                    std::wstring 脚本 =
                                                        L"window.__panelApplyExpand("
                                                        + std::to_wstring(请求号)
                                                        + L", "
                                                        + 宽子项JSON
                                                        + L");";
                                                    (void)上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
                                                }
                                                return S_OK;
                                            }

                                            if (私有_解析详情消息(消息, &请求号, &展开类型, &节点指针)) {
                                                auto* 上下文 = 私有_取窗口上下文(窗口);
                                                if (上下文 && 上下文->用途 == 枚举_WebView2窗口用途::控制面板) {
                                                    私有_记录WebView2诊断("SQL控制面板拒绝live详情消息", 46);
                                                    return S_OK;
                                                }
                                                if (上下文 && 上下文->WebView) {
                                                    const auto 详情JSON = 读取控制面板节点详情JSON(展开类型, 节点指针, 16);
                                                    const auto 宽详情JSON = 私有_UTF8转宽字串(详情JSON);
                                                    std::wstring 脚本 =
                                                        L"window.__panelApplyDetail("
                                                        + std::to_wstring(请求号)
                                                        + L", "
                                                        + 宽详情JSON
                                                        + L");";
                                                    (void)上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
                                                }
                                            }
                                            return S_OK;
                                        }).Get(),
                                    nullptr);

                                私有_启动诊断码.store(12);
                                私有_调整WebView尺寸(窗口);
                                PostMessageW(窗口, 私有_WM_刷新控制面板窗口, 0, 0);
                                return S_OK;
                            }).Get());
                }).Get());

        if (FAILED(结果)) {
            私有_启动诊断码.store(13);
            私有_记录WebView2诊断(
                "创建环境调用失败",
                13,
                结果,
                ERROR_SUCCESS,
                "用户数据目录=" + 路径UTF8文本(用户数据目录));
            return false;
        }

        return true;
    }

    // 功能：服务所在模块的内部辅助流程。
    LRESULT CALLBACK 私有_窗口过程(HWND 窗口, UINT 消息, WPARAM wParam, LPARAM lParam)
    {
        switch (消息) {
        case WM_CREATE: {
            枚举_WebView2窗口用途 用途 = 枚举_WebView2窗口用途::控制面板;
            if (auto* 创建参数 = reinterpret_cast<CREATESTRUCTW*>(lParam); 创建参数 && 创建参数->lpCreateParams) {
                const auto 原始用途 = reinterpret_cast<std::uintptr_t>(创建参数->lpCreateParams);
                if (原始用途 == static_cast<std::uintptr_t>(枚举_WebView2窗口用途::相机播放)) {
                    用途 = 枚举_WebView2窗口用途::相机播放;
                }
                else if (原始用途 == static_cast<std::uintptr_t>(枚举_WebView2窗口用途::自我场景)) {
                    用途 = 枚举_WebView2窗口用途::自我场景;
                }
            }
            auto* 上下文 = new(std::nothrow) 结构_WebView2窗口上下文{};
            if (!上下文) {
                return -1;
            }
            上下文->用途 = 用途;
            SetWindowLongPtrW(窗口, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(上下文));
            if (!私有_初始化WebView2(窗口)) {
                const auto 诊断码 = 私有_启动诊断码.load();
                私有_记录WebView2诊断(
                    "弹窗提示初始化失败",
                    诊断码,
                    S_OK,
                    GetLastError(),
                    "HWND=" + std::to_string(reinterpret_cast<std::uintptr_t>(窗口)));
                std::wstring 正文 =
                    L"WebView2 初始化失败。\n诊断码："
                    + std::to_wstring(诊断码);
#if 鱼巢_开关_启用逻辑错误排查日志输出
                正文 += L"\n详细信息已写入 ./日志。";
#else
                正文 += L"\n诊断日志输出已关闭。";
#endif
#if 鱼巢_开关_启用UI直接提示输出 && 鱼巢_开关_启用项目弹窗错误
                MessageBoxW(窗口, 正文.c_str(), L"鱼巢控制面板", MB_OK | MB_ICONERROR);
#endif
            }
            return 0;
        }
        case WM_SIZE:
            私有_调整WebView尺寸(窗口);
            return 0;
        case 私有_WM_刷新控制面板窗口:
            私有_刷新页面(窗口);
            return 0;
        case 私有_WM_页面刷新JSON: {
            std::unique_ptr<结构_页面刷新投递> 投递{
                reinterpret_cast<结构_页面刷新投递*>(lParam)
            };
            if (投递) {
                私有_执行页面刷新JSON到页面(窗口, *投递);
            }
            return 0;
        }
        case 私有_WM_保存自我场景截图: {
            std::unique_ptr<std::shared_ptr<结构_自我场景截图请求>> 请求{
                reinterpret_cast<std::shared_ptr<结构_自我场景截图请求>*>(lParam)
            };
            if (请求 && *请求) {
                私有_捕获自我场景窗口PNG(窗口, *请求);
            }
            return 0;
        }
        case WM_CLOSE:
            DestroyWindow(窗口);
            return 0;
        case WM_DESTROY: {
            auto* 上下文 = 私有_取窗口上下文(窗口);
            const auto 用途 = 上下文 ? 上下文->用途 : 枚举_WebView2窗口用途::控制面板;
            if (用途 == 枚举_WebView2窗口用途::相机播放) {
                (void)私有_停止相机播放();
            }
            if (上下文) {
                if (!上下文->当前HTML临时路径.empty()) {
                    std::error_code 删除错误{};
                    std::filesystem::remove(上下文->当前HTML临时路径, 删除错误);
                }
                上下文->WebView.Reset();
                上下文->控制器.Reset();
                if (上下文->加载器模块) {
                    FreeLibrary(上下文->加载器模块);
                }
                delete 上下文;
                SetWindowLongPtrW(窗口, GWLP_USERDATA, 0);
            }
            if (用途 == 枚举_WebView2窗口用途::相机播放) {
                私有_相机播放窗口句柄.store(nullptr);
                私有_相机播放窗口启动中.store(false);
            }
            else if (用途 == 枚举_WebView2窗口用途::自我场景) {
                私有_自我场景窗口句柄.store(nullptr);
                私有_自我场景窗口启动中.store(false);
            }
            else {
                私有_窗口句柄.store(nullptr);
            }
            PostQuitMessage(0);
            return 0;
        }
        default:
            break;
        }

        return DefWindowProcW(窗口, 消息, wParam, lParam);
    }

    // 功能：注册方法、模板、对象或运行入口。
    bool 私有_确保窗口类已注册() noexcept
    {
        static std::once_flag 一次{};
        static bool 已注册 = false;

        std::call_once(一次, []() {
            WNDCLASSEXW 窗口类{};
            窗口类.cbSize = sizeof(窗口类);
            窗口类.style = CS_HREDRAW | CS_VREDRAW;
            窗口类.lpfnWndProc = 私有_窗口过程;
            窗口类.hInstance = GetModuleHandleW(nullptr);
            窗口类.hCursor = LoadCursorW(nullptr, IDC_ARROW);
            窗口类.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
            窗口类.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
            窗口类.lpszClassName = 私有_控制面板窗口类名;

            const ATOM 结果 = RegisterClassExW(&窗口类);
            const DWORD 错误 = GetLastError();
            已注册 = (结果 != 0) || (错误 == ERROR_CLASS_ALREADY_EXISTS);
            if (!已注册) {
                私有_记录WebView2诊断("窗口类注册失败", 2, S_OK, 错误);
            }
        });

        return 已注册;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::atomic<HWND>& 私有_窗口句柄槽(枚举_WebView2窗口用途 用途) noexcept
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            return 私有_相机播放窗口句柄;
        }
        if (用途 == 枚举_WebView2窗口用途::自我场景) {
            return 私有_自我场景窗口句柄;
        }
        return 私有_窗口句柄;
    }

    // 功能：启动线程、模块或运行流程。
    std::atomic_bool& 私有_窗口启动中槽(枚举_WebView2窗口用途 用途) noexcept
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            return 私有_相机播放窗口启动中;
        }
        return 私有_自我场景窗口启动中;
    }

    // 功能：读取指定窗口用途对应的线程运行标记。
    std::atomic_bool& 私有_窗口线程运行中槽(枚举_WebView2窗口用途 用途) noexcept
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            return 私有_相机播放窗口线程运行中;
        }
        if (用途 == 枚举_WebView2窗口用途::自我场景) {
            return 私有_自我场景窗口线程运行中;
        }
        return 私有_控制面板窗口线程运行中;
    }

    // 功能：服务所在模块的内部辅助流程。
    const wchar_t* 私有_窗口标题(枚举_WebView2窗口用途 用途) noexcept
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            return L"鱼巢相机画面";
        }
        if (用途 == 枚举_WebView2窗口用途::自我场景) {
            return L"鱼巢自我场景视频窗口";
        }
        return L"鱼巢控制面板";
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_窗口尺寸(
        枚举_WebView2窗口用途 用途,
        int& 宽,
        int& 高) noexcept
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            宽 = 1280;
            高 = 820;
            return;
        }
        if (用途 == 枚举_WebView2窗口用途::自我场景) {
            宽 = 1500;
            高 = 920;
            return;
        }
        宽 = 1400;
        高 = 900;
    }

    // 功能：服务所在模块的内部辅助流程。
    int 私有_独立窗口诊断码(枚举_WebView2窗口用途 用途) noexcept
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            return 16;
        }
        if (用途 == 枚举_WebView2窗口用途::自我场景) {
            return 19;
        }
        return 2;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_窗口用途文本(枚举_WebView2窗口用途 用途)
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            return "相机播放";
        }
        if (用途 == 枚举_WebView2窗口用途::自我场景) {
            return "自我场景";
        }
        return "控制面板";
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_独立窗口线程主体(枚举_WebView2窗口用途 用途) noexcept
    {
        结构_窗口线程运行守卫 运行守卫{ 私有_窗口线程运行中槽(用途) };
        HRESULT COM结果 = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        const bool 已初始化COM = SUCCEEDED(COM结果);
        if (!已初始化COM) {
            私有_记录WebView2诊断(
                私有_窗口用途文本(用途) + "窗口线程COM初始化失败",
                私有_独立窗口诊断码(用途),
                COM结果);
        }

        try {
            if (!私有_确保窗口类已注册()) {
                私有_记录WebView2诊断(
                    私有_窗口用途文本(用途) + "窗口线程停止：窗口类未注册",
                    私有_独立窗口诊断码(用途));
                私有_窗口启动中槽(用途).store(false);
                if (已初始化COM) {
                    CoUninitialize();
                }
                return;
            }

            int 宽 = 0;
            int 高 = 0;
            私有_窗口尺寸(用途, 宽, 高);
            HWND 窗口 = CreateWindowExW(
                WS_EX_APPWINDOW,
                私有_控制面板窗口类名,
                私有_窗口标题(用途),
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                宽,
                高,
                nullptr,
                nullptr,
                GetModuleHandleW(nullptr),
                reinterpret_cast<LPVOID>(static_cast<std::uintptr_t>(用途)));

            if (!窗口) {
                私有_记录WebView2诊断(
                    私有_窗口用途文本(用途) + "窗口创建失败",
                    私有_独立窗口诊断码(用途) + 1,
                    S_OK,
                    GetLastError());
                私有_窗口启动中槽(用途).store(false);
                if (已初始化COM) {
                    CoUninitialize();
                }
                return;
            }

            私有_窗口句柄槽(用途).store(窗口);
            私有_窗口启动中槽(用途).store(false);
            ShowWindow(窗口, SW_SHOW);
            UpdateWindow(窗口);
            SetForegroundWindow(窗口);

            MSG 消息{};
            while (GetMessageW(&消息, nullptr, 0, 0) > 0) {
                TranslateMessage(&消息);
                DispatchMessageW(&消息);
            }
        }
        catch (...) {
            私有_记录WebView2诊断(
                私有_窗口用途文本(用途) + "窗口线程捕获未知异常",
                私有_独立窗口诊断码(用途) + 2);
            私有_窗口启动中槽(用途).store(false);
        }

        if (已初始化COM) {
            CoUninitialize();
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_打开相机播放窗口(HWND 来源窗口) noexcept
    {
        try {

            // 功能：按函数名执行对应处理。
            if (auto* 现有窗口 = 私有_相机播放窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
                ShowWindow(现有窗口, IsIconic(现有窗口) ? SW_RESTORE : SW_SHOW);
                SetForegroundWindow(现有窗口);
                return true;
            }
            if (私有_相机播放窗口启动中.load()) {
                return true;
            }

            std::lock_guard<std::mutex> 锁(私有_相机播放窗口互斥);

            // 功能：按函数名执行对应处理。
            if (auto* 现有窗口 = 私有_相机播放窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
                ShowWindow(现有窗口, IsIconic(现有窗口) ? SW_RESTORE : SW_SHOW);
                SetForegroundWindow(现有窗口);
                return true;
            }
            if (私有_相机播放窗口启动中.load()) {
                return true;
            }

            私有_相机播放窗口启动中.store(true);
            std::thread(私有_独立窗口线程主体, 枚举_WebView2窗口用途::相机播放).detach();
            return true;
        }
        catch (...) {
            私有_相机播放窗口启动中.store(false);
            私有_记录WebView2诊断("打开相机播放窗口捕获未知异常", 18);
            return false;
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_打开自我场景窗口(HWND 来源窗口) noexcept
    {
        try {

            // 功能：按函数名执行对应处理。
            if (auto* 现有窗口 = 私有_自我场景窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
                PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
                ShowWindow(现有窗口, IsIconic(现有窗口) ? SW_RESTORE : SW_SHOW);
                SetForegroundWindow(现有窗口);
                return true;
            }
            if (私有_自我场景窗口启动中.load()) {
                return true;
            }

            std::lock_guard<std::mutex> 锁(私有_自我场景窗口互斥);

            // 功能：按函数名执行对应处理。
            if (auto* 现有窗口 = 私有_自我场景窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
                PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
                ShowWindow(现有窗口, IsIconic(现有窗口) ? SW_RESTORE : SW_SHOW);
                SetForegroundWindow(现有窗口);
                return true;
            }
            if (私有_自我场景窗口启动中.load()) {
                return true;
            }

            私有_自我场景窗口启动中.store(true);
            std::thread(私有_独立窗口线程主体, 枚举_WebView2窗口用途::自我场景).detach();
            return true;
        }
        catch (...) {
            私有_自我场景窗口启动中.store(false);
            私有_记录WebView2诊断("打开自我场景窗口捕获未知异常", 21);
            return false;
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_窗口线程主体(std::promise<bool> 启动结果) noexcept
    {
        结构_窗口线程运行守卫 运行守卫{ 私有_控制面板窗口线程运行中 };
        HRESULT COM结果 = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        const bool 已初始化COM = SUCCEEDED(COM结果);
        if (!已初始化COM) {
            私有_记录WebView2诊断("COM初始化失败", 1, COM结果);
        }

        try {
            私有_启动诊断码.store(1);
            if (!私有_确保窗口类已注册()) {
                私有_启动诊断码.store(2);
                私有_记录WebView2诊断("窗口线程停止：窗口类未注册", 2);
                启动结果.set_value(false);
                if (已初始化COM) {
                    CoUninitialize();
                }
                return;
            }

            // 功能：按函数名执行对应处理。
            if (auto* 现有窗口 = 私有_窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
                私有_启动诊断码.store(3);
                PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
                ShowWindow(现有窗口, IsIconic(现有窗口) ? SW_RESTORE : SW_SHOW);
                BringWindowToTop(现有窗口);
                SetWindowPos(现有窗口, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
                SetForegroundWindow(现有窗口);
                启动结果.set_value(true);
                if (已初始化COM) {
                    CoUninitialize();
                }
                return;
            }

            私有_启动诊断码.store(4);
            HWND 窗口 = CreateWindowExW(
                WS_EX_APPWINDOW,
                私有_控制面板窗口类名,
                L"鱼巢控制面板",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                1400,
                900,
                nullptr,
                nullptr,
                GetModuleHandleW(nullptr),
                nullptr);

            if (!窗口) {
                const DWORD 错误 = GetLastError();
                私有_启动诊断码.store(5);
                私有_记录WebView2诊断("窗口创建失败", 5, S_OK, 错误);
                启动结果.set_value(false);
                if (已初始化COM) {
                    CoUninitialize();
                }
                return;
            }

            私有_窗口句柄.store(窗口);
            私有_启动诊断码.store(6);
            ShowWindow(窗口, SW_SHOW);
            UpdateWindow(窗口);
            BringWindowToTop(窗口);
            SetWindowPos(窗口, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            SetForegroundWindow(窗口);
            项目运行日志("控制面板WebView2/窗口已显示 | 诊断码=6");
            启动结果.set_value(true);

            MSG 消息{};
            while (GetMessageW(&消息, nullptr, 0, 0) > 0) {
                TranslateMessage(&消息);
                DispatchMessageW(&消息);
            }
        }
        catch (...) {
            私有_启动诊断码.store(14);
            私有_记录WebView2诊断("窗口线程捕获未知异常", 14);
            try {
                启动结果.set_value(false);
            }
            catch (...) {
            }
        }

        if (已初始化COM) {
            CoUninitialize();
        }
    }
}

// 功能：启动线程、模块或运行流程。
bool 启动控制面板WebView2窗口() noexcept
{
    try {
        私有_启动诊断码.store(0);

        // 功能：按函数名执行对应处理。
        if (auto* 现有窗口 = 私有_窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
            PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
            ShowWindow(现有窗口, IsIconic(现有窗口) ? SW_RESTORE : SW_SHOW);
            BringWindowToTop(现有窗口);
            SetWindowPos(现有窗口, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            SetForegroundWindow(现有窗口);
            return true;
        }

        std::lock_guard<std::mutex> 锁(私有_窗口互斥);

        // 功能：按函数名执行对应处理。
        if (auto* 现有窗口 = 私有_窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
            PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
            ShowWindow(现有窗口, IsIconic(现有窗口) ? SW_RESTORE : SW_SHOW);
            BringWindowToTop(现有窗口);
            SetWindowPos(现有窗口, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            SetForegroundWindow(现有窗口);
            return true;
        }

        std::promise<bool> 启动结果{};
        auto 结果 = 启动结果.get_future();
        std::thread(私有_窗口线程主体, std::move(启动结果)).detach();
        const bool 成功 = 结果.get();
        if (!成功) {
            私有_记录WebView2诊断(
                "启动窗口线程返回失败",
                私有_启动诊断码.load());
        }
        else {
            项目运行日志(
                "控制面板WebView2/启动窗口线程返回成功 | 诊断码="
                + std::to_string(私有_启动诊断码.load()));
        }
        return 成功;
    }
    catch (...) {
        私有_启动诊断码.store(15);
        私有_记录WebView2诊断("启动窗口捕获未知异常", 15);
        return false;
    }
}

// 功能：启动控制面板相机播放独立窗口。
bool 启动控制面板相机播放窗口() noexcept
{
    return 私有_打开相机播放窗口(nullptr);
}

// 功能：启动控制面板自我场景独立窗口。
bool 启动控制面板WebView2自我场景窗口() noexcept
{
    return 私有_打开自我场景窗口(nullptr);
}

// 功能：打开自我场景 WebView2 窗口并保存当前 PNG 预览，必要时用窗口客户区截图兜底。
bool 保存控制面板WebView2自我场景窗口截图(
    const std::filesystem::path& 输出路径,
    const std::uint32_t 等待渲染毫秒) noexcept
{
    const HWND 原有窗口 = 私有_自我场景窗口句柄.load();
    const bool 已有窗口 = 原有窗口 && IsWindow(原有窗口);
    bool 已调用打开窗口 = false;
    auto 关闭临时窗口 = [&]() noexcept {
        if (已有窗口 || !已调用打开窗口) {
            return;
        }
        if (auto* 当前窗口 = 私有_自我场景窗口句柄.load(); 当前窗口 && IsWindow(当前窗口)) {
            PostMessageW(当前窗口, WM_CLOSE, 0, 0);
        }
    };

    try {
        if (输出路径.empty()) {
            私有_记录WebView2诊断("自我场景截图输出路径为空", 52);
            return false;
        }
        已调用打开窗口 = true;
        if (!私有_打开自我场景窗口(nullptr)) {
            私有_记录WebView2诊断(
                "自我场景截图打开窗口失败",
                52,
                S_OK,
                ERROR_SUCCESS,
                "路径=" + 路径UTF8文本(输出路径));
            关闭临时窗口();
            return false;
        }

        constexpr auto 最大窗口就绪等待 = std::chrono::seconds(10);
        const auto 等待开始 = std::chrono::steady_clock::now();
        HWND 自我场景窗口 = nullptr;
        while (std::chrono::steady_clock::now() - 等待开始 < 最大窗口就绪等待) {
            自我场景窗口 = 私有_自我场景窗口句柄.load();
            if (自我场景窗口 && IsWindow(自我场景窗口)) {
                auto* 上下文 = 私有_取窗口上下文(自我场景窗口);
                if (上下文 && 上下文->WebView) {
                    break;
                }
            }
            Sleep(50);
        }

        if (!自我场景窗口 || !IsWindow(自我场景窗口)) {
            私有_记录WebView2诊断(
                "自我场景截图等待窗口超时",
                52,
                S_OK,
                ERROR_TIMEOUT,
                "路径=" + 路径UTF8文本(输出路径));
            关闭临时窗口();
            return false;
        }
        if (auto* 上下文 = 私有_取窗口上下文(自我场景窗口); !上下文 || !上下文->WebView) {
            私有_记录WebView2诊断(
                "自我场景截图等待WebView超时",
                52,
                S_OK,
                ERROR_TIMEOUT,
                "路径=" + 路径UTF8文本(输出路径));
            关闭临时窗口();
            return false;
        }

        PostMessageW(自我场景窗口, 私有_WM_刷新控制面板窗口, 0, 0);
        ShowWindow(自我场景窗口, IsIconic(自我场景窗口) ? SW_RESTORE : SW_SHOW);
        BringWindowToTop(自我场景窗口);
        SetWindowPos(自我场景窗口, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetForegroundWindow(自我场景窗口);
        UpdateWindow(自我场景窗口);
        if (等待渲染毫秒 > 0) {
            Sleep(等待渲染毫秒);
        }

        auto 请求 = std::make_shared<结构_自我场景截图请求>();
        请求->输出路径 = 输出路径;
        auto 完成Future = 请求->完成.get_future();
        auto* 投递 = new(std::nothrow) std::shared_ptr<结构_自我场景截图请求>(请求);
        if (!投递) {
            私有_记录WebView2诊断(
                "自我场景截图请求分配失败",
                52,
                S_OK,
                ERROR_OUTOFMEMORY,
                "路径=" + 路径UTF8文本(输出路径));
            const bool 兜底成功 = 私有_保存窗口客户区PNG(自我场景窗口, 输出路径);
            关闭临时窗口();
            return 兜底成功;
        }
        if (!PostMessageW(
            自我场景窗口,
            私有_WM_保存自我场景截图,
            0,
            reinterpret_cast<LPARAM>(投递))) {
            delete 投递;
            私有_记录WebView2诊断(
                "自我场景截图请求投递失败",
                52,
                S_OK,
                GetLastError(),
                "路径=" + 路径UTF8文本(输出路径));
            const bool 兜底成功 = 私有_保存窗口客户区PNG(自我场景窗口, 输出路径);
            关闭临时窗口();
            return 兜底成功;
        }

        const auto 完成等待 = std::chrono::milliseconds(
            static_cast<std::int64_t>(等待渲染毫秒) + 10000);
        if (完成Future.wait_for(完成等待) != std::future_status::ready) {
            if (!请求->已完成.exchange(true)) {
                私有_记录WebView2诊断(
                    "自我场景截图保存超时",
                    52,
                    S_OK,
                    ERROR_TIMEOUT,
                    "路径=" + 路径UTF8文本(输出路径));
            }
            const bool 兜底成功 = 私有_保存窗口客户区PNG(自我场景窗口, 输出路径);
            关闭临时窗口();
            return 兜底成功;
        }
        const bool 主路径成功 = 完成Future.get();
        const bool 成功 = 主路径成功 || 私有_保存窗口客户区PNG(自我场景窗口, 输出路径);
        关闭临时窗口();
        return 成功;
    }
    catch (...) {
        私有_记录WebView2诊断(
            "自我场景截图捕获未知异常",
            52,
            S_OK,
            GetLastError(),
            输出路径.empty() ? std::string{} : ("路径=" + 路径UTF8文本(输出路径)));
        关闭临时窗口();
        return false;
    }
}

// 功能：请求控制面板相关窗口关闭并等待窗口线程收束。
void 请求关闭控制面板WebView2窗口() noexcept
{
    try {
        auto 请求关闭 = [](std::atomic<HWND>& 窗口槽) noexcept {
            if (auto* 窗口 = 窗口槽.load(); 窗口 && IsWindow(窗口)) {
                PostMessageW(窗口, WM_CLOSE, 0, 0);
            }
        };

        请求关闭(私有_相机播放窗口句柄);
        请求关闭(私有_自我场景窗口句柄);
        请求关闭(私有_窗口句柄);

        constexpr int 最大等待次数 = 100;
        for (int 次数 = 0; 次数 < 最大等待次数; ++次数) {
            const HWND 控制面板窗口 = 私有_窗口句柄.load();
            const HWND 相机窗口 = 私有_相机播放窗口句柄.load();
            const HWND 自我场景窗口 = 私有_自我场景窗口句柄.load();
            const bool 控制面板活动 = 私有_控制面板窗口线程运行中.load()
                || (控制面板窗口 && IsWindow(控制面板窗口));
            const bool 相机活动 = 私有_相机播放窗口线程运行中.load()
                || 私有_相机播放窗口启动中.load()
                || (相机窗口 && IsWindow(相机窗口));
            const bool 自我场景活动 = 私有_自我场景窗口线程运行中.load()
                || 私有_自我场景窗口启动中.load()
                || (自我场景窗口 && IsWindow(自我场景窗口));
            if (!控制面板活动 && !相机活动 && !自我场景活动) {
                return;
            }
            Sleep(50);
        }

        私有_记录WebView2诊断(
            "请求关闭窗口超时",
            23,
            S_OK,
            ERROR_SUCCESS,
            std::string("控制面板线程=")
                + (私有_控制面板窗口线程运行中.load() ? "是" : "否")
                + " | 相机线程=" + (私有_相机播放窗口线程运行中.load() ? "是" : "否")
                + " | 自我场景线程=" + (私有_自我场景窗口线程运行中.load() ? "是" : "否"));
    }
    catch (...) {
        私有_记录WebView2诊断("请求关闭窗口捕获未知异常", 24);
    }
}

// 功能：等待线程、任务、外设或条件变化。
void 等待控制面板WebView2窗口关闭() noexcept
{
    while (true) {
        auto* 控制面板窗口 = 私有_窗口句柄.load();
        auto* 相机窗口 = 私有_相机播放窗口句柄.load();
        auto* 自我场景窗口 = 私有_自我场景窗口句柄.load();
        const bool 控制面板活动 = 私有_控制面板窗口线程运行中.load()
            || (控制面板窗口 && IsWindow(控制面板窗口));
        const bool 相机活动 = 私有_相机播放窗口线程运行中.load()
            || 私有_相机播放窗口启动中.load()
            || (相机窗口 && IsWindow(相机窗口));
        const bool 自我场景活动 = 私有_自我场景窗口线程运行中.load()
            || 私有_自我场景窗口启动中.load()
            || (自我场景窗口 && IsWindow(自我场景窗口));
        if (!控制面板活动 && !相机活动 && !自我场景活动) {
            break;
        }
        Sleep(100);
    }
}

// 功能：读取并返回指定对象、状态或运行材料。
int 获取控制面板WebView2启动诊断码() noexcept
{
    return 私有_启动诊断码.load();
}
