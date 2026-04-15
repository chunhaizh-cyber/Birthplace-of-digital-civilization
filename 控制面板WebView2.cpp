#include "控制面板WebView2.h"

#include <atomic>
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
#include <wrl/event.h>

#include "WebView2导入.h"
#include "控制面板类.h"

import 自我模块;
import 自我线程模块;

using Microsoft::WRL::ComPtr;

namespace {
    constexpr UINT 私有_WM_刷新控制面板窗口 = WM_APP + 101;
    constexpr wchar_t 私有_控制面板窗口类名[] = L"鱼巢控制面板WebView2窗口";

    using CreateCoreWebView2EnvironmentWithOptionsFn = HRESULT(STDAPICALLTYPE*)(
        PCWSTR,
        PCWSTR,
        ICoreWebView2EnvironmentOptions*,
        ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler*);

    struct 结构_控制面板WebView2上下文 {
        HMODULE 加载器模块 = nullptr;
        ComPtr<ICoreWebView2Controller> 控制器{};
        ComPtr<ICoreWebView2> WebView{};
        bool WebView已就绪 = false;
    };

    std::mutex 私有_控制面板窗口互斥{};
    std::atomic<HWND> 私有_控制面板窗口句柄{ nullptr };
    std::atomic<int> 私有_控制面板启动诊断码{ 0 };

    void 私有_确保控制面板线程存在(const std::string& 调用点) noexcept
    {
        if (!自我.已初始化()) {
            (void)初始化自我环境(调用点 + "/初始化自我");
        }
        if (!自我.已初始化()) {
            return;
        }

        结构_线程存在初始化参数 参数{};
        参数.线程标签 = "控制面板窗口线程";
        参数.线程类型 = "控制面板线程";
        (void)自我.确保线程子存在(参数, 调用点);
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

    std::filesystem::path 私有_取模块目录() noexcept
    {
        wchar_t 缓冲区[MAX_PATH]{};
        const DWORD 缓冲区长度 = static_cast<DWORD>(sizeof(缓冲区) / sizeof(缓冲区[0]));
        const DWORD 长度 = GetModuleFileNameW(nullptr, 缓冲区, 缓冲区长度);
        if (长度 == 0 || 长度 >= 缓冲区长度) {
            return {};
        }

        return std::filesystem::path(缓冲区).parent_path();
    }

    std::vector<std::filesystem::path> 私有_WebView2加载器候选路径()
    {
        const auto 模块目录 = 私有_取模块目录();
        std::error_code 错误{};
        const auto 当前目录 = std::filesystem::current_path(错误);
        const auto 仓库依赖相对路径 = std::filesystem::path(L"third_party") / L"webview2" /
            鱼巢_WEBVIEW2_SDK_VERSION_W / L"pkg" / L"build" / L"native" / L"x64" / L"WebView2Loader.dll";

        std::vector<std::filesystem::path> 输出{};
        输出.reserve(8);

        if (!模块目录.empty()) {
            输出.push_back((模块目录 / L".." / L".." / 仓库依赖相对路径).lexically_normal());
            输出.push_back(模块目录 / L"WebView2Loader.dll");
        }
        if (!错误) {
            输出.push_back((当前目录 / 仓库依赖相对路径).lexically_normal());
            输出.push_back((当前目录 / L"WebView2Loader.dll").lexically_normal());
        }
        输出.push_back(L"WebView2Loader.dll");
        输出.push_back(L"C:\\Program Files\\Microsoft Visual Studio\\18\\Professional\\Common7\\IDE\\PrivateAssemblies\\WebView2Loader.dll");
        输出.push_back(L"C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\PrivateAssemblies\\WebView2Loader.dll");
        输出.push_back(L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\Common7\\IDE\\PrivateAssemblies\\WebView2Loader.dll");
        return 输出;
    }

    CreateCoreWebView2EnvironmentWithOptionsFn 私有_载入WebView2创建函数(HMODULE* 模块句柄输出) noexcept
    {
        for (const auto& 路径 : 私有_WebView2加载器候选路径()) {
            if (路径.empty()) {
                continue;
            }

            HMODULE 模块句柄 = LoadLibraryW(路径.c_str());
            if (!模块句柄) {
                continue;
            }

            auto* 函数 = reinterpret_cast<CreateCoreWebView2EnvironmentWithOptionsFn>(
                GetProcAddress(模块句柄, "CreateCoreWebView2EnvironmentWithOptions"));
            if (函数) {
                if (模块句柄输出) {
                    *模块句柄输出 = 模块句柄;
                }
                return 函数;
            }

            FreeLibrary(模块句柄);
        }

        return nullptr;
    }

    结构_控制面板WebView2上下文* 私有_取控制面板窗口上下文(HWND 窗口) noexcept
    {
        return reinterpret_cast<结构_控制面板WebView2上下文*>(GetWindowLongPtrW(窗口, GWLP_USERDATA));
    }

    void 私有_显示控制面板窗口(HWND 窗口) noexcept
    {
        if (!窗口 || !IsWindow(窗口)) {
            return;
        }

        ShowWindow(窗口, IsIconic(窗口) ? SW_RESTORE : SW_SHOW);
        SetForegroundWindow(窗口);
    }

    std::string 私有_生成控制面板HTML快照()
    {
        if (!自我.已初始化()) {
            (void)初始化自我环境("控制面板WebView2/刷新");
        }

        const auto 快照 = 读取控制面板快照(8, 20);
        return 生成控制面板HTML(快照, 20);
    }

    void 私有_调整WebView区域(HWND 窗口) noexcept
    {
        auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
        if (!上下文 || !上下文->控制器) {
            return;
        }

        RECT 区域{};
        GetClientRect(窗口, &区域);
        上下文->控制器->put_Bounds(区域);
    }

    void 私有_刷新控制面板页面(HWND 窗口) noexcept
    {
        auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
        if (!上下文 || !上下文->WebView) {
            return;
        }

        const auto HTML = 私有_生成控制面板HTML快照();
        const auto 宽HTML = 私有_UTF8转宽字串(HTML);
        if (!宽HTML.empty()) {
            (void)上下文->WebView->NavigateToString(宽HTML.c_str());
        }
    }

    bool 私有_初始化控制面板WebView2(HWND 窗口) noexcept
    {
        auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
        if (!上下文) {
            私有_控制面板启动诊断码.store(7);
            return false;
        }

        auto* 创建环境 = 私有_载入WebView2创建函数(&上下文->加载器模块);
        if (!创建环境) {
            私有_控制面板启动诊断码.store(8);
            return false;
        }

        const auto 用户数据目录 = (私有_取模块目录() / L"webview2-user-data").lexically_normal();
        const HRESULT 启动结果 = 创建环境(
            nullptr,
            用户数据目录.c_str(),
            nullptr,
            Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [窗口](HRESULT 环境结果, ICoreWebView2Environment* 环境) -> HRESULT {
                    auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
                    if (!上下文 || FAILED(环境结果) || !环境) {
                        私有_控制面板启动诊断码.store(9);
                        return 环境结果;
                    }

                    return 环境->CreateCoreWebView2Controller(
                        窗口,
                        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                            [窗口](HRESULT 控制器结果, ICoreWebView2Controller* 控制器) -> HRESULT {
                                auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
                                if (!上下文 || FAILED(控制器结果) || !控制器) {
                                    私有_控制面板启动诊断码.store(10);
                                    return 控制器结果;
                                }

                                上下文->控制器 = 控制器;
                                上下文->控制器->get_CoreWebView2(&上下文->WebView);
                                if (!上下文->WebView) {
                                    私有_控制面板启动诊断码.store(11);
                                    return E_FAIL;
                                }

                                ComPtr<ICoreWebView2Settings> 设置{};
                                if (SUCCEEDED(上下文->WebView->get_Settings(&设置)) && 设置) {
                                    (void)设置->put_AreDefaultContextMenusEnabled(TRUE);
                                    (void)设置->put_AreDevToolsEnabled(FALSE);
                                    (void)设置->put_IsStatusBarEnabled(FALSE);

                                    ComPtr<ICoreWebView2Settings3> 设置3{};
                                    if (SUCCEEDED(设置.As(&设置3)) && 设置3) {
                                        (void)设置3->put_AreBrowserAcceleratorKeysEnabled(TRUE);
                                    }
                                }

                                (void)上下文->WebView->add_WebMessageReceived(
                                    Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                        [窗口](ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs* 参数) -> HRESULT {
                                            if (!参数) {
                                                return S_OK;
                                            }

                                            LPWSTR 原始消息 = nullptr;
                                            if (FAILED(参数->TryGetWebMessageAsString(&原始消息)) || !原始消息) {
                                                return S_OK;
                                            }

                                            const std::wstring 消息 = 原始消息;
                                            CoTaskMemFree(原始消息);

                                            if (消息 == L"refresh") {
                                                PostMessageW(窗口, 私有_WM_刷新控制面板窗口, 0, 0);
                                            }

                                            return S_OK;
                                        }).Get(),
                                    nullptr);

                                上下文->WebView已就绪 = true;
                                私有_控制面板启动诊断码.store(12);
                                私有_调整WebView区域(窗口);
                                PostMessageW(窗口, 私有_WM_刷新控制面板窗口, 0, 0);
                                return S_OK;
                            }).Get());
                }).Get());

        if (FAILED(启动结果)) {
            私有_控制面板启动诊断码.store(13);
            return false;
        }

        return true;
    }

    LRESULT CALLBACK 私有_控制面板窗口过程(HWND 窗口, UINT 消息, WPARAM wParam, LPARAM lParam)
    {
        switch (消息) {
        case WM_CREATE: {
            auto* 上下文 = new(std::nothrow) 结构_控制面板WebView2上下文{};
            if (!上下文) {
                return -1;
            }

            SetWindowLongPtrW(窗口, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(上下文));
            if (!私有_初始化控制面板WebView2(窗口)) {
                MessageBoxW(窗口, L"WebView2 初始化失败。", L"鱼巢控制面板", MB_ICONERROR | MB_OK);
            }
            return 0;
        }
        case WM_SIZE:
            私有_调整WebView区域(窗口);
            return 0;
        case 私有_WM_刷新控制面板窗口:
            私有_刷新控制面板页面(窗口);
            私有_显示控制面板窗口(窗口);
            return 0;
        case WM_CLOSE:
            DestroyWindow(窗口);
            return 0;
        case WM_DESTROY: {
            auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
            if (上下文) {
                上下文->WebView.Reset();
                上下文->控制器.Reset();
                if (上下文->加载器模块) {
                    FreeLibrary(上下文->加载器模块);
                }
                delete 上下文;
                SetWindowLongPtrW(窗口, GWLP_USERDATA, 0);
            }

            私有_控制面板窗口句柄.store(nullptr);
            PostQuitMessage(0);
            return 0;
        }
        default:
            break;
        }

        return DefWindowProcW(窗口, 消息, wParam, lParam);
    }

    bool 私有_确保控制面板窗口类已注册() noexcept
    {
        static std::once_flag 一次{};
        static bool 已注册 = false;

        std::call_once(一次, []() {
            WNDCLASSEXW 窗口类{};
            窗口类.cbSize = sizeof(窗口类);
            窗口类.style = CS_HREDRAW | CS_VREDRAW;
            窗口类.lpfnWndProc = 私有_控制面板窗口过程;
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

    void 私有_控制面板窗口线程主体(std::promise<bool> 启动结果) noexcept
    {
        HRESULT COM结果 = S_OK;
        bool 已初始化COM = false;

        try {
            COM结果 = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
            已初始化COM = SUCCEEDED(COM结果);

            私有_控制面板启动诊断码.store(1);
            if (!私有_确保控制面板窗口类已注册()) {
                私有_控制面板启动诊断码.store(2);
                启动结果.set_value(false);
                if (已初始化COM) {
                    CoUninitialize();
                }
                return;
            }

            auto* 现有窗口 = 私有_控制面板窗口句柄.load();
            if (现有窗口 && IsWindow(现有窗口)) {
                私有_控制面板启动诊断码.store(3);
                PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
                启动结果.set_value(true);
                if (已初始化COM) {
                    CoUninitialize();
                }
                return;
            }

            私有_控制面板启动诊断码.store(4);
            HWND 窗口 = CreateWindowExW(
                WS_EX_APPWINDOW,
                私有_控制面板窗口类名,
                L"鱼巢控制面板",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT,
                1360, 860,
                nullptr,
                nullptr,
                GetModuleHandleW(nullptr),
                nullptr);

            if (!窗口) {
                私有_控制面板启动诊断码.store(5);
                启动结果.set_value(false);
                if (已初始化COM) {
                    CoUninitialize();
                }
                return;
            }

            私有_控制面板窗口句柄.store(窗口);
            私有_控制面板启动诊断码.store(6);
            ShowWindow(窗口, SW_SHOW);
            UpdateWindow(窗口);
            启动结果.set_value(true);

            MSG 消息{};
            while (GetMessageW(&消息, nullptr, 0, 0) > 0) {
                TranslateMessage(&消息);
                DispatchMessageW(&消息);
            }

            if (已初始化COM) {
                CoUninitialize();
            }
        }
        catch (...) {
            私有_控制面板启动诊断码.store(14);
            try {
                启动结果.set_value(false);
            }
            catch (...) {
            }
            if (已初始化COM) {
                CoUninitialize();
            }
        }
    }
}

bool 启动控制面板WebView2窗口() noexcept
{
    try {
        私有_控制面板启动诊断码.store(0);
        auto* 现有窗口 = 私有_控制面板窗口句柄.load();
        if (现有窗口 && IsWindow(现有窗口)) {
            PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
            return true;
        }

        std::lock_guard<std::mutex> 锁(私有_控制面板窗口互斥);
        现有窗口 = 私有_控制面板窗口句柄.load();
        if (现有窗口 && IsWindow(现有窗口)) {
            PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
            return true;
        }

        std::promise<bool> 启动结果{};
        auto 结果 = 启动结果.get_future();
        (void)私有_确保控制面板线程存在("控制面板WebView2/启动线程");
        std::thread(私有_控制面板窗口线程主体, std::move(启动结果)).detach();
        return 结果.get();
    }
    catch (...) {
        私有_控制面板启动诊断码.store(15);
        return false;
    }
}

void 等待控制面板WebView2窗口关闭() noexcept
{
    while (true) {
        auto* 窗口 = 私有_控制面板窗口句柄.load();
        if (!窗口 || !IsWindow(窗口)) {
            break;
        }
        Sleep(100);
    }
}

int 获取控制面板WebView2启动诊断码() noexcept
{
    return 私有_控制面板启动诊断码.load();
}
