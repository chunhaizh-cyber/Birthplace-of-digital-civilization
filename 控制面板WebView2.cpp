module;

#include <atomic>
#include <algorithm>
#include <cstdint>
#include <exception>
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

#include "WebView2导入.h"

module 控制面板WebView2;

import 控制面板类;
import 日志模块;
import 任务模块.管理界面线程;
import 自我类;
import 自我线程模块;
import D455相机模块;
import 基础数据类型模块;

using Microsoft::WRL::ComPtr;

namespace {
    constexpr UINT 私有_WM_刷新控制面板窗口 = WM_APP + 220;
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
    std::mutex 私有_相机互斥{};
    std::unique_ptr<D455_相机实现> 私有_相机{};
    bool 私有_相机已打开 = false;
    HMODULE 私有_RealSense运行时模块 = nullptr;

    struct 结构_相机轮廓框 {
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
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
        std::size_t 轮廓数 = 0;
        std::string 彩色RGB_Base64{};
        std::string 轮廓掩膜_Base64{};
        std::vector<结构_相机轮廓框> 轮廓框{};
        std::string 消息{};
    };

    std::string 私有_HRESULT文本(HRESULT 值)
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

    std::string 私有_路径UTF8(const std::filesystem::path& 路径)
    {
        const auto 文本 = 路径.u8string();
        return std::string(
            reinterpret_cast<const char*>(文本.data()),
            文本.size());
    }

    void 私有_记录WebView2诊断(
        const std::string& 阶段,
        int 诊断码,
        HRESULT COM结果 = S_OK,
        DWORD Win32错误 = ERROR_SUCCESS,
        const std::string& 附加 = {}) noexcept
    {
        try {
            std::ostringstream 输出;
            输出 << "控制面板WebView2/" << 阶段
                << " | 诊断码=" << 诊断码;
            if (COM结果 != S_OK) {
                输出 << " | HRESULT=" << 私有_HRESULT文本(COM结果);
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
    }

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

    bool 私有_解析详情消息(
        const std::wstring& 消息,
        std::uint64_t* 请求号,
        std::string* 展开类型,
        std::uintptr_t* 节点指针) noexcept
    {
        return 私有_解析节点消息(消息, L"detail:", 请求号, 展开类型, 节点指针);
    }

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

    std::filesystem::path 私有_模块目录() noexcept
    {
        wchar_t 缓冲区[MAX_PATH]{};
        const DWORD 长度 = GetModuleFileNameW(nullptr, 缓冲区, MAX_PATH);
        if (长度 == 0 || 长度 >= MAX_PATH) {
            return {};
        }
        return std::filesystem::path(缓冲区).parent_path();
    }

    bool 私有_确保RealSense运行时(std::string& 错误消息) noexcept
    {
        if (私有_RealSense运行时模块) {
            return true;
        }

        try {
            const auto 模块目录 = 私有_模块目录();
            const auto 主DLL = (模块目录 / L"realsense2.dll").lexically_normal();
            const auto LZ4DLL = (模块目录 / L"lz4.dll").lexically_normal();

            if (!std::filesystem::exists(主DLL)) {
                错误消息 = "RealSense 运行时缺失: " + 私有_路径UTF8(主DLL);
                return false;
            }
            if (!std::filesystem::exists(LZ4DLL)) {
                错误消息 = "RealSense 依赖缺失: " + 私有_路径UTF8(LZ4DLL);
                return false;
            }

            私有_RealSense运行时模块 = LoadLibraryExW(
                主DLL.c_str(),
                nullptr,
                LOAD_WITH_ALTERED_SEARCH_PATH);
            if (!私有_RealSense运行时模块) {
                const DWORD 错误 = GetLastError();
                std::ostringstream 输出;
                输出 << "RealSense 运行时加载失败"
                    << " | Win32=" << 错误
                    << " | 路径=" << 私有_路径UTF8(主DLL);
                错误消息 = 输出.str();
                return false;
            }

            return true;
        }
        catch (const std::exception& e) {
            错误消息 = std::string("RealSense 运行时检查异常: ") + e.what();
            return false;
        }
        catch (...) {
            错误消息 = "RealSense 运行时检查发生未知异常";
            return false;
        }
    }

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
                "路径=" + 私有_路径UTF8(路径));
            FreeLibrary(模块);
        }

        私有_记录WebView2诊断(
            "加载器加载失败",
            8,
            S_OK,
            最近加载错误,
            最近加载路径.empty() ? std::string{} : ("最近路径=" + 私有_路径UTF8(最近加载路径)));
        return nullptr;
    }

    结构_WebView2窗口上下文* 私有_取窗口上下文(HWND 窗口) noexcept
    {
        return reinterpret_cast<结构_WebView2窗口上下文*>(GetWindowLongPtrW(窗口, GWLP_USERDATA));
    }

    bool 私有_确保窗口类已注册() noexcept;
    bool 私有_打开相机播放窗口(HWND 来源窗口) noexcept;
    bool 私有_打开自我场景窗口(HWND 来源窗口) noexcept;

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

    void 私有_追加JSON字符串(std::ostringstream& 输出, std::string_view 文本)
    {
        输出 << '"';
        for (const char 字符 : 文本) {
            switch (字符) {
            case '\\': 输出 << "\\\\"; break;
            case '"': 输出 << "\\\""; break;
            case '\n': 输出 << "\\n"; break;
            case '\r': 输出 << "\\r"; break;
            case '\t': 输出 << "\\t"; break;
            default:
                if (static_cast<unsigned char>(字符) < 0x20) {
                    输出 << "\\u"
                        << std::hex
                        << std::setw(4)
                        << std::setfill('0')
                        << static_cast<int>(static_cast<unsigned char>(字符))
                        << std::dec
                        << std::setfill(' ');
                }
                else {
                    输出 << 字符;
                }
                break;
            }
        }
        输出 << '"';
    }

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

    std::vector<std::uint8_t> 私有_构建相机RGB(
        const 结构体_原始场景帧& 帧,
        int 宽,
        int 高)
    {
        std::vector<std::uint8_t> 输出(static_cast<std::size_t>(宽) * static_cast<std::size_t>(高) * 3, 0);
        if (宽 <= 0 || 高 <= 0 || 帧.宽度 <= 0 || 帧.高度 <= 0) {
            return 输出;
        }

        const bool 有颜色 = 帧.有效颜色();
        for (int y = 0; y < 高; ++y) {
            const int sy = std::min(帧.高度 - 1, static_cast<int>(
                (static_cast<long long>(y) * 帧.高度) / 高));
            for (int x = 0; x < 宽; ++x) {
                const int sx = std::min(帧.宽度 - 1, static_cast<int>(
                    (static_cast<long long>(x) * 帧.宽度) / 宽));
                const auto 源索引 = static_cast<std::size_t>(sy) * static_cast<std::size_t>(帧.宽度)
                    + static_cast<std::size_t>(sx);
                const auto 目标索引 = (static_cast<std::size_t>(y) * static_cast<std::size_t>(宽)
                    + static_cast<std::size_t>(x)) * 3;
                const Color 颜色 = 有颜色 ? 帧.颜色[源索引] : Color{ 0, 0, 0 };
                输出[目标索引] = 颜色.r;
                输出[目标索引 + 1] = 颜色.g;
                输出[目标索引 + 2] = 颜色.b;
            }
        }
        return 输出;
    }

    std::vector<std::uint8_t> 私有_构建源轮廓掩膜(const 结构体_原始场景帧& 帧)
    {
        const auto 像素数 = static_cast<std::size_t>(帧.宽度) * static_cast<std::size_t>(帧.高度);
        std::vector<std::uint8_t> 输出(像素数, 0);
        if (像素数 == 0) {
            return 输出;
        }

        if (帧.有效前景提示()) {
            for (std::size_t i = 0; i < 像素数; ++i) {
                输出[i] = 帧.前景提示[i] ? 128 : 0;
            }
        }

        for (const auto& 轮廓 : 帧.轮廓观测列表) {
            if (轮廓.w <= 0 || 轮廓.h <= 0) {
                continue;
            }
            if (!轮廓.掩膜.empty()
                && 轮廓.掩膜.size() == static_cast<std::size_t>(轮廓.w) * static_cast<std::size_t>(轮廓.h)) {
                for (int rv = 0; rv < 轮廓.h; ++rv) {
                    const int y = 轮廓.y + rv;
                    if (y < 0 || y >= 帧.高度) {
                        continue;
                    }
                    for (int ru = 0; ru < 轮廓.w; ++ru) {
                        const int x = 轮廓.x + ru;
                        if (x < 0 || x >= 帧.宽度) {
                            continue;
                        }
                        const auto 掩膜索引 = static_cast<std::size_t>(rv) * static_cast<std::size_t>(轮廓.w)
                            + static_cast<std::size_t>(ru);
                        if (轮廓.掩膜[掩膜索引]) {
                            输出[static_cast<std::size_t>(y) * static_cast<std::size_t>(帧.宽度)
                                + static_cast<std::size_t>(x)] = 255;
                        }
                    }
                }
            }
        }
        return 输出;
    }

    std::vector<std::uint8_t> 私有_缩放掩膜(
        const std::vector<std::uint8_t>& 源,
        int 源宽,
        int 源高,
        int 宽,
        int 高)
    {
        std::vector<std::uint8_t> 输出(static_cast<std::size_t>(宽) * static_cast<std::size_t>(高), 0);
        if (源.empty() || 源宽 <= 0 || 源高 <= 0 || 宽 <= 0 || 高 <= 0) {
            return 输出;
        }
        for (int y = 0; y < 高; ++y) {
            const int sy = std::min(源高 - 1, static_cast<int>(
                (static_cast<long long>(y) * 源高) / 高));
            for (int x = 0; x < 宽; ++x) {
                const int sx = std::min(源宽 - 1, static_cast<int>(
                    (static_cast<long long>(x) * 源宽) / 宽));
                输出[static_cast<std::size_t>(y) * static_cast<std::size_t>(宽) + static_cast<std::size_t>(x)] =
                    源[static_cast<std::size_t>(sy) * static_cast<std::size_t>(源宽) + static_cast<std::size_t>(sx)];
            }
        }
        return 输出;
    }

    结构_相机轮廓框 私有_缩放轮廓框(const 结构体_轮廓观测& 轮廓, int 源宽, int 源高, int 宽, int 高) noexcept
    {
        结构_相机轮廓框 输出{};
        if (源宽 <= 0 || 源高 <= 0 || 宽 <= 0 || 高 <= 0) {
            return 输出;
        }
        const int x0 = std::clamp(static_cast<int>((static_cast<long long>(轮廓.x) * 宽) / 源宽), 0, 宽 - 1);
        const int y0 = std::clamp(static_cast<int>((static_cast<long long>(轮廓.y) * 高) / 源高), 0, 高 - 1);
        const int x1 = std::clamp(static_cast<int>((static_cast<long long>(轮廓.x + 轮廓.w) * 宽) / 源宽), x0 + 1, 宽);
        const int y1 = std::clamp(static_cast<int>((static_cast<long long>(轮廓.y + 轮廓.h) * 高) / 源高), y0 + 1, 高);
        输出.x = x0;
        输出.y = y0;
        输出.w = x1 - x0;
        输出.h = y1 - y0;
        return 输出;
    }

    结构_相机帧JSON 私有_构建相机帧JSON数据(const 结构体_原始场景帧& 帧)
    {
        结构_相机帧JSON 输出{};
        if (!帧.有效深度()) {
            输出.消息 = "相机帧无有效深度数据";
            return 输出;
        }

        输出.成功 = true;
        输出.源宽 = 帧.宽度;
        输出.源高 = 帧.高度;
        输出.深度帧号 = 帧.时间戳.深度帧号;
        输出.彩色帧号 = 帧.时间戳.彩色帧号;
        输出.设备时间_us = 帧.时间戳.设备时间_us;
        输出.轮廓数 = 帧.轮廓观测列表.size();
        输出.消息 = "已更新";

        私有_计算相机显示尺寸(输出.源宽, 输出.源高, 输出.宽, 输出.高);
        const auto RGB = 私有_构建相机RGB(帧, 输出.宽, 输出.高);
        const auto 源掩膜 = 私有_构建源轮廓掩膜(帧);
        const auto 掩膜 = 私有_缩放掩膜(源掩膜, 输出.源宽, 输出.源高, 输出.宽, 输出.高);
        输出.彩色RGB_Base64 = 私有_Base64编码(RGB);
        输出.轮廓掩膜_Base64 = 私有_Base64编码(掩膜);

        输出.轮廓框.reserve(帧.轮廓观测列表.size());
        for (const auto& 轮廓 : 帧.轮廓观测列表) {
            if (轮廓.w > 0 && 轮廓.h > 0) {
                输出.轮廓框.push_back(私有_缩放轮廓框(轮廓, 输出.源宽, 输出.源高, 输出.宽, 输出.高));
            }
        }
        return 输出;
    }

    结构_相机帧JSON 私有_读取相机帧()
    {
        std::lock_guard<std::mutex> 锁(私有_相机互斥);
        try {
            if (!私有_相机) {
                std::string 运行时错误;
                if (!私有_确保RealSense运行时(运行时错误)) {
                    项目运行错误日志("控制面板相机画面/" + 运行时错误);
                    结构_相机帧JSON 输出{};
                    输出.消息 = 运行时错误;
                    return 输出;
                }

                D455_相机实现::配置项 配置{};
                配置.启用轮廓提取 = true;
                配置.轮廓_输出原始掩膜 = true;
                私有_相机 = std::make_unique<D455_相机实现>(配置);
                私有_相机已打开 = false;
            }

            if (!私有_相机已打开) {
                if (!私有_相机->打开()) {
                    私有_相机.reset();
                    私有_相机已打开 = false;
                    项目运行错误日志("控制面板相机画面/D455打开失败");
                    结构_相机帧JSON 输出{};
                    输出.消息 = "D455 打开失败";
                    return 输出;
                }
                私有_相机已打开 = true;
            }

            结构体_原始场景帧 帧{};
            if (!私有_相机->采集一帧(帧)) {
                项目运行错误日志("控制面板相机画面/D455采集一帧失败");
                结构_相机帧JSON 输出{};
                输出.消息 = "D455 采集一帧失败";
                return 输出;
            }
            return 私有_构建相机帧JSON数据(帧);
        }
        catch (...) {
            项目运行错误日志("控制面板相机画面/采集过程发生异常");
            结构_相机帧JSON 输出{};
            输出.消息 = "相机采集异常";
            return 输出;
        }
    }

    结构_相机帧JSON 私有_停止相机()
    {
        std::lock_guard<std::mutex> 锁(私有_相机互斥);
        try {
            if (私有_相机) {
                私有_相机->关闭();
            }
        }
        catch (...) {
        }
        私有_相机.reset();
        私有_相机已打开 = false;

        结构_相机帧JSON 输出{};
        输出.已停止 = true;
        输出.消息 = "已停止";
        return 输出;
    }

    std::string 私有_相机帧JSON文本(const 结构_相机帧JSON& 帧)
    {
        std::ostringstream 输出;
        输出 << "{";
        输出 << "\"ok\":" << (帧.成功 ? "true" : "false") << ",";
        输出 << "\"stopped\":" << (帧.已停止 ? "true" : "false") << ",";
        输出 << "\"sourceWidth\":" << 帧.源宽 << ",";
        输出 << "\"sourceHeight\":" << 帧.源高 << ",";
        输出 << "\"width\":" << 帧.宽 << ",";
        输出 << "\"height\":" << 帧.高 << ",";
        输出 << "\"depthFrame\":" << 帧.深度帧号 << ",";
        输出 << "\"colorFrame\":" << 帧.彩色帧号 << ",";
        输出 << "\"deviceTimeUs\":" << 帧.设备时间_us << ",";
        输出 << "\"contourCount\":" << 帧.轮廓数 << ",";
        输出 << "\"message\":";
        私有_追加JSON字符串(输出, 帧.消息);
        输出 << ",";
        输出 << "\"error\":";
        私有_追加JSON字符串(输出, 帧.成功 || 帧.已停止 ? std::string_view{} : std::string_view(帧.消息));
        输出 << ",";
        输出 << "\"colorRGB\":";
        私有_追加JSON字符串(输出, 帧.彩色RGB_Base64);
        输出 << ",";
        输出 << "\"contourMask\":";
        私有_追加JSON字符串(输出, 帧.轮廓掩膜_Base64);
        输出 << ",\"boxes\":[";
        for (std::size_t i = 0; i < 帧.轮廓框.size(); ++i) {
            if (i > 0) {
                输出 << ",";
            }
            const auto& 框 = 帧.轮廓框[i];
            输出 << "{\"x\":" << 框.x
                << ",\"y\":" << 框.y
                << ",\"w\":" << 框.w
                << ",\"h\":" << 框.h
                << "}";
        }
        输出 << "]}";
        return 输出.str();
    }

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
        const std::wstring 脚本 = L"window.__panelApplyCameraFrame(" + 宽JSON + L");";
        (void)上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
    }

    void 私有_发送相机窗口状态到页面(HWND 窗口, bool 成功, std::string_view 消息) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        std::ostringstream JSON;
        JSON << "{\"ok\":" << (成功 ? "true" : "false") << ",\"message\":";
        私有_追加JSON字符串(JSON, 消息);
        JSON << "}";
        const auto 宽JSON = 私有_UTF8转宽字串(JSON.str());
        if (宽JSON.empty()) {
            return;
        }

        const std::wstring 脚本 = L"window.__panelApplyCameraWindowState(" + 宽JSON + L");";
        (void)上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
    }

    void 私有_发送自我场景窗口状态到页面(HWND 窗口, bool 成功, std::string_view 消息) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        std::ostringstream JSON;
        JSON << "{\"ok\":" << (成功 ? "true" : "false") << ",\"message\":";
        私有_追加JSON字符串(JSON, 消息);
        JSON << "}";
        const auto 宽JSON = 私有_UTF8转宽字串(JSON.str());
        if (宽JSON.empty()) {
            return;
        }

        const std::wstring 脚本 = L"window.__panelApplySceneWindowState(" + 宽JSON + L");";
        (void)上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
    }

    void 私有_发送页面刷新到页面(HWND 窗口, std::string_view 页面) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        const auto JSON = 读取控制面板页面刷新JSON(页面);
        const auto 宽JSON = 私有_UTF8转宽字串(JSON);
        if (宽JSON.empty()) {
            return;
        }

        const std::wstring 脚本 = L"window.__panelApplyPageRefresh(" + 宽JSON + L");";
        (void)上下文->WebView->ExecuteScript(脚本.c_str(), nullptr);
    }

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
          <div class="viewer-title">轮廓图</div>
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
        <div class="stat-label">轮廓数</div>
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

    function 绘制轮廓画面(data) {
      const canvas = document.getElementById('camera-contour-canvas');
      if (!canvas || !data || !data.contourMask) return;
      const w = Number(data.width || 0);
      const h = Number(data.height || 0);
      if (w <= 0 || h <= 0) return;
      canvas.width = w;
      canvas.height = h;
      const mask = 解码Base64字节(data.contourMask);
      const ctx = canvas.getContext('2d');
      const image = ctx.createImageData(w, h);
      for (let i = 0, j = 0; i < mask.length && j < image.data.length; ++i, j += 4) {
        const v = mask[i] || 0;
        if (v >= 220) {
          image.data[j] = 22;
          image.data[j + 1] = 214;
          image.data[j + 2] = 143;
          image.data[j + 3] = 255;
        } else if (v > 0) {
          image.data[j] = 54;
          image.data[j + 1] = 93;
          image.data[j + 2] = 105;
          image.data[j + 3] = 255;
        } else {
          image.data[j] = 5;
          image.data[j + 1] = 8;
          image.data[j + 2] = 12;
          image.data[j + 3] = 255;
        }
      }
      ctx.putImageData(image, 0, 0);
      ctx.lineWidth = Math.max(1, Math.round(Math.min(w, h) / 240));
      ctx.strokeStyle = '#fb7185';
      (Array.isArray(data.boxes) ? data.boxes : []).forEach((box) => {
        const x = Number(box.x || 0);
        const y = Number(box.y || 0);
        const bw = Number(box.w || 0);
        const bh = Number(box.h || 0);
        if (bw > 0 && bh > 0) {
          ctx.strokeRect(x + 0.5, y + 0.5, Math.max(1, bw - 1), Math.max(1, bh - 1));
        }
      });
    }

    function 更新相机统计(data) {
      const setText = (id, text) => {
        const node = document.getElementById(id);
        if (node) node.textContent = text;
      };
      setText('camera-size-stat', `${data.width || 0} x ${data.height || 0}`);
      setText('camera-source-stat', `${data.sourceWidth || 0} x ${data.sourceHeight || 0}`);
      setText('camera-frame-stat', `D${data.depthFrame || 0} / C${data.colorFrame || 0}`);
      setText('camera-contour-stat', String(data.contourCount || 0));
      setText('camera-rgb-meta', `${data.width || 0} x ${data.height || 0}`);
      setText('camera-contour-meta', `${data.contourCount || 0} 个轮廓`);
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
      相机自动采集句柄 = window.setInterval(() => 请求相机帧('camera:capture'), 1000);
    }

    function 停止相机采集() {
      if (相机自动采集句柄) {
        clearInterval(相机自动采集句柄);
        相机自动采集句柄 = 0;
      }
      相机请求中 = false;
      发送相机消息('camera:stop');
    }

    window.__panelApplyCameraFrame = function(data) {
      相机请求中 = false;
      if (!data || typeof data !== 'object') {
        设置相机状态('相机返回数据无效。', 'error');
        return;
      }
      if (data.stopped) {
        设置相机状态(data.message || '已停止。');
        return;
      }
      if (!data.ok) {
        设置相机状态(data.error || '相机采集失败。', 'error');
        return;
      }
      绘制RGB画面(data);
      绘制轮廓画面(data);
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

    std::string 私有_生成页面HTML(枚举_WebView2窗口用途 用途)
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            return 私有_生成相机播放HTML();
        }

        if (!自我.已初始化()) {
            (void)初始化自我环境();
        }

        if (用途 == 枚举_WebView2窗口用途::自我场景) {
            const auto 快照 = 读取控制面板快照(10, 24);
            return 生成自我场景独立窗口HTML(快照);
        }
        const auto 快照 = 读取控制面板主窗口快照(10, 24);
        return 生成控制面板HTML(快照, 24);
    }

    void 私有_刷新页面(HWND 窗口) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        const auto HTML = 私有_生成页面HTML(上下文->用途);
        const auto 宽HTML = 私有_UTF8转宽字串(HTML);
        if (!宽HTML.empty()) {
            (void)上下文->WebView->NavigateToString(宽HTML.c_str());
        }
    }

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
                                            if (消息 == L"camera:start" || 消息 == L"camera:capture") {
                                                私有_发送相机帧到页面(窗口, 私有_读取相机帧());
                                                return S_OK;
                                            }
                                            if (消息 == L"camera:stop") {
                                                私有_发送相机帧到页面(窗口, 私有_停止相机());
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
                                            if (私有_解析展开消息(消息, &请求号, &展开类型, &节点指针, &附加参数)) {
                                                auto* 上下文 = 私有_取窗口上下文(窗口);
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
                "用户数据目录=" + 私有_路径UTF8(用户数据目录));
            return false;
        }

        return true;
    }

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
                const std::wstring 正文 =
                    L"WebView2 初始化失败。\n诊断码："
                    + std::to_wstring(诊断码)
                    + L"\n详细信息已写入 ./日志。";
                MessageBoxW(窗口, 正文.c_str(), L"鱼巢控制面板", MB_OK | MB_ICONERROR);
            }
            return 0;
        }
        case WM_SIZE:
            私有_调整WebView尺寸(窗口);
            return 0;
        case 私有_WM_刷新控制面板窗口:
            私有_刷新页面(窗口);
            return 0;
        case WM_CLOSE:
            DestroyWindow(窗口);
            return 0;
        case WM_DESTROY: {
            auto* 上下文 = 私有_取窗口上下文(窗口);
            const auto 用途 = 上下文 ? 上下文->用途 : 枚举_WebView2窗口用途::控制面板;
            if (用途 == 枚举_WebView2窗口用途::相机播放) {
                (void)私有_停止相机();
            }
            if (上下文) {
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

    std::atomic_bool& 私有_窗口启动中槽(枚举_WebView2窗口用途 用途) noexcept
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            return 私有_相机播放窗口启动中;
        }
        return 私有_自我场景窗口启动中;
    }

    const wchar_t* 私有_窗口标题(枚举_WebView2窗口用途 用途) noexcept
    {
        if (用途 == 枚举_WebView2窗口用途::相机播放) {
            return L"鱼巢相机画面";
        }
        if (用途 == 枚举_WebView2窗口用途::自我场景) {
            return L"鱼巢自我所在场景";
        }
        return L"鱼巢控制面板";
    }

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

    void 私有_独立窗口线程主体(枚举_WebView2窗口用途 用途) noexcept
    {
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

    bool 私有_打开相机播放窗口(HWND 来源窗口) noexcept
    {
        try {
            if (auto* 现有窗口 = 私有_相机播放窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
                ShowWindow(现有窗口, IsIconic(现有窗口) ? SW_RESTORE : SW_SHOW);
                SetForegroundWindow(现有窗口);
                return true;
            }
            if (私有_相机播放窗口启动中.load()) {
                return true;
            }

            std::lock_guard<std::mutex> 锁(私有_相机播放窗口互斥);
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

    bool 私有_打开自我场景窗口(HWND 来源窗口) noexcept
    {
        try {
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

    void 私有_窗口线程主体(std::promise<bool> 启动结果) noexcept
    {
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

            if (auto* 现有窗口 = 私有_窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
                私有_启动诊断码.store(3);
                PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
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

bool 启动控制面板WebView2窗口() noexcept
{
    try {
        私有_启动诊断码.store(0);

        if (auto* 现有窗口 = 私有_窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
            PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
            ShowWindow(现有窗口, IsIconic(现有窗口) ? SW_RESTORE : SW_SHOW);
            SetForegroundWindow(现有窗口);
            return true;
        }

        std::lock_guard<std::mutex> 锁(私有_窗口互斥);
        if (auto* 现有窗口 = 私有_窗口句柄.load(); 现有窗口 && IsWindow(现有窗口)) {
            PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
            ShowWindow(现有窗口, IsIconic(现有窗口) ? SW_RESTORE : SW_SHOW);
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
        return 成功;
    }
    catch (...) {
        私有_启动诊断码.store(15);
        私有_记录WebView2诊断("启动窗口捕获未知异常", 15);
        return false;
    }
}

void 等待控制面板WebView2窗口关闭() noexcept
{
    while (true) {
        auto* 窗口 = 私有_窗口句柄.load();
        if (!窗口 || !IsWindow(窗口)) {
            break;
        }
        Sleep(100);
    }
}

int 获取控制面板WebView2启动诊断码() noexcept
{
    return 私有_启动诊断码.load();
}
