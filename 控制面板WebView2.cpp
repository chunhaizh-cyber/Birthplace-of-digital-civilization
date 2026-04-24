module;

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <future>
#include <mutex>
#include <string>
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
import 自我模块;
import 自我线程模块;

using Microsoft::WRL::ComPtr;

namespace {
    constexpr UINT 私有_WM_刷新控制面板窗口 = WM_APP + 220;
    constexpr wchar_t 私有_控制面板窗口类名[] = L"鱼巢控制面板WebView2重构窗口";

    using CreateCoreWebView2EnvironmentWithOptionsFn = HRESULT(STDAPICALLTYPE*)(
        PCWSTR,
        PCWSTR,
        ICoreWebView2EnvironmentOptions*,
        ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler*);

    struct 结构_WebView2窗口上下文 {
        HMODULE 加载器模块 = nullptr;
        ComPtr<ICoreWebView2Controller> 控制器{};
        ComPtr<ICoreWebView2> WebView{};
    };

    std::mutex 私有_窗口互斥{};
    std::atomic<HWND> 私有_窗口句柄{ nullptr };
    std::atomic<int> 私有_启动诊断码{ 0 };

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

        try {
            if (请求号) {
                *请求号 = std::stoull(消息.substr(前缀长度, 第一分隔 - 前缀长度));
            }
            if (展开类型) {
                *展开类型 = 私有_宽字串转UTF8(消息.substr(第一分隔 + 1, 第二分隔 - 第一分隔 - 1));
            }
            if (节点指针) {
                const auto 节点文本 = 第三分隔 == std::wstring::npos
                    ? 消息.substr(第二分隔 + 1)
                    : 消息.substr(第二分隔 + 1, 第三分隔 - 第二分隔 - 1);
                *节点指针 = static_cast<std::uintptr_t>(std::stoull(节点文本));
            }
            if (附加参数) {
                *附加参数 = 第三分隔 == std::wstring::npos
                    ? 0
                    : static_cast<std::uintptr_t>(std::stoull(消息.substr(第三分隔 + 1)));
            }
            return true;
        }
        catch (...) {
            return false;
        }
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

    std::filesystem::path 私有_模块目录() noexcept
    {
        wchar_t 缓冲区[MAX_PATH]{};
        const DWORD 长度 = GetModuleFileNameW(nullptr, 缓冲区, MAX_PATH);
        if (长度 == 0 || 长度 >= MAX_PATH) {
            return {};
        }
        return std::filesystem::path(缓冲区).parent_path();
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
        for (const auto& 路径 : 私有_加载器候选路径()) {
            if (路径.empty()) {
                continue;
            }

            HMODULE 模块 = LoadLibraryW(路径.c_str());
            if (!模块) {
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

            FreeLibrary(模块);
        }

        return nullptr;
    }

    结构_WebView2窗口上下文* 私有_取窗口上下文(HWND 窗口) noexcept
    {
        return reinterpret_cast<结构_WebView2窗口上下文*>(GetWindowLongPtrW(窗口, GWLP_USERDATA));
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

    std::string 私有_生成页面HTML()
    {
        if (!自我.已初始化()) {
            (void)初始化自我环境("控制面板WebView2/刷新");
        }

        const auto 快照 = 读取控制面板快照(10, 24);
        return 生成控制面板HTML(快照, 24);
    }

    void 私有_刷新页面(HWND 窗口) noexcept
    {
        auto* 上下文 = 私有_取窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        const auto HTML = 私有_生成页面HTML();
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
            return false;
        }

        auto* 创建环境 = 私有_加载创建函数(&上下文->加载器模块);
        if (!创建环境) {
            私有_启动诊断码.store(8);
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
                        return FAILED(环境结果) ? 环境结果 : E_FAIL;
                    }

                    return 环境->CreateCoreWebView2Controller(
                        窗口,
                        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                            [窗口](HRESULT 控制器结果, ICoreWebView2Controller* 控制器) -> HRESULT {
                                auto* 上下文 = 私有_取窗口上下文(窗口);
                                if (!上下文 || FAILED(控制器结果) || !控制器) {
                                    私有_启动诊断码.store(10);
                                    return FAILED(控制器结果) ? 控制器结果 : E_FAIL;
                                }

                                上下文->控制器 = 控制器;
                                上下文->控制器->get_CoreWebView2(&上下文->WebView);
                                if (!上下文->WebView) {
                                    私有_启动诊断码.store(11);
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
            return false;
        }

        return true;
    }

    LRESULT CALLBACK 私有_窗口过程(HWND 窗口, UINT 消息, WPARAM wParam, LPARAM lParam)
    {
        switch (消息) {
        case WM_CREATE: {
            auto* 上下文 = new(std::nothrow) 结构_WebView2窗口上下文{};
            if (!上下文) {
                return -1;
            }
            SetWindowLongPtrW(窗口, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(上下文));
            if (!私有_初始化WebView2(窗口)) {
                MessageBoxW(窗口, L"WebView2 初始化失败。", L"鱼巢控制面板", MB_OK | MB_ICONERROR);
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
            if (上下文) {
                上下文->WebView.Reset();
                上下文->控制器.Reset();
                if (上下文->加载器模块) {
                    FreeLibrary(上下文->加载器模块);
                }
                delete 上下文;
                SetWindowLongPtrW(窗口, GWLP_USERDATA, 0);
            }
            私有_窗口句柄.store(nullptr);
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
            已注册 = (结果 != 0) || (GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
        });

        return 已注册;
    }

    void 私有_窗口线程主体(std::promise<bool> 启动结果) noexcept
    {
        HRESULT COM结果 = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        const bool 已初始化COM = SUCCEEDED(COM结果);

        try {
            私有_启动诊断码.store(1);
            if (!私有_确保窗口类已注册()) {
                私有_启动诊断码.store(2);
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
                私有_启动诊断码.store(5);
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
        return 结果.get();
    }
    catch (...) {
        私有_启动诊断码.store(15);
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
