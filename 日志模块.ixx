module;

#include <chrono>
#include <atomic>
#include <cstdio>
#include <ctime>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <format>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <windows.h>

export module 日志模块;

// 说明：
// - 运行日志与异常日志分两个文件。
// - 每天按日期自动切换新文件（YYYYMMDD）。
// - 线程安全；失败静默（避免日志写入导致业务崩溃）。
/*
用法（最小接入）

程序初始化（比如 OnInitDialog() 或主线程启动处）加：

import 日志模块;

void 初始化日志()
{
    日志参数 p{};
    p.根目录 = std::filesystem::path(L"./日志");
    p.文件前缀 = "海鱼";
    p.每条刷新 = true;      // 崩溃也尽量能落盘；想更快可设 false
    日志::初始化(p);
}


写运行日志：

日志::运行("系统启动");
日志::运行f("帧号={}, 点数={}", frameId, n);
日志::运行_警告("深度无效比例偏高");


写异常日志：

try {
    // ...
} catch (const std::exception& e) {
    日志::记录异常(e, "点簇分割失败");
}


关闭（可选，程序退出时）：

日志::关闭();
注意:日志模块内部保证线程安全（至少保护队列/文件写入）。
不要在持有大锁时做 heavy 日志：
大锁区内只做：LOG_MIN("xxx", a,b,c) 这种轻量入队
复杂内容：先拷贝数据到局部变量，退出锁区后再格式化/输出
日志里别调用会拿世界树锁的函数（尤其是 to_string(节点*) 这类“看着无害”的东西）。
必要时加“限流/采样”：例如同一类日志每秒最多N条，否则日志本身就是性能炸弹。
日志锁顺序固定：如果你一定要同步输出（方案A），规定“永远先拿日志锁，再拿其他锁”，并且最好只在调试模式这样做。
*/
export enum class 枚举_日志类别 : std::int8_t {
    运行 = 0,
    异常 = 1,
};

export enum class 枚举_日志级别 : std::int8_t {
    调试 = 0,
    信息 = 1,
    警告 = 2,
    错误 = 3,
    致命 = 4,
};

export struct 日志参数 {
    // 日志根目录（默认 ./日志）
    std::filesystem::path 根目录 = std::filesystem::path(L"./日志");
    // 文件名前缀（默认 海鱼）
    std::string 文件前缀 = "海鱼";
    // 写入后是否每条都 flush（默认 true，便于崩溃前落盘；追求性能可关）
    bool 每条刷新 = true;
    // 是否同时输出到调试器（OutputDebugStringW），默认 false
    bool 输出到调试器 = false;
};

export namespace 日志 {
    // 初始化/反初始化
    void 初始化(const 日志参数& p = 日志参数{});
    void 关闭();

    // 基础写入（字符串）
    void 写(枚举_日志类别 类别, 枚举_日志级别 级别, std::string_view msg);

    // 便捷：运行日志
    inline void 运行(std::string_view msg) { 写(枚举_日志类别::运行, 枚举_日志级别::信息, msg); }
    inline void 运行_警告(std::string_view msg) { 写(枚举_日志类别::运行, 枚举_日志级别::警告, msg); }
    inline void 运行_错误(std::string_view msg) { 写(枚举_日志类别::运行, 枚举_日志级别::错误, msg); }

    // 便捷：异常日志
    inline void 异常(std::string_view msg) { 写(枚举_日志类别::异常, 枚举_日志级别::错误, msg); }
    inline void 异常_致命(std::string_view msg) { 写(枚举_日志类别::异常, 枚举_日志级别::致命, msg); }

    // format 版本（C++20 std::format）
    template <class... Args>
    inline void 运行f(std::string_view fmt, Args&&... args) {
        const auto msg = std::vformat(fmt, std::make_format_args(args...));
        写(枚举_日志类别::运行, 枚举_日志级别::信息, msg);
    }
    template <class... Args>
    inline void 异常f(std::string_view fmt, Args&&... args) {
        const auto msg = std::vformat(fmt, std::make_format_args(args...));
        写(枚举_日志类别::异常, 枚举_日志级别::错误, msg);
    }

    // 便捷：记录 std::exception
    void 记录异常(const std::exception& e, std::string_view 上下文 = {});
}

export void 初始化项目日志() noexcept;
export void 关闭项目日志() noexcept;
export void 项目运行日志(const std::string& 文本) noexcept;
export void 项目运行警告日志(const std::string& 文本) noexcept;
export void 项目运行错误日志(const std::string& 文本) noexcept;
export void 项目弹窗错误提示(const std::string& 标题, const std::string& 文本) noexcept;
export void 项目自检无上级需求日志(const std::string& 文本) noexcept;
export void 项目记录异常日志(const std::exception& 异常, const std::string& 上下文) noexcept;
export void 项目致命日志(const std::string& 文本) noexcept;

// =========================
// 实现（放在同一模块，便于直接 import 使用）
// =========================

namespace 日志::detail {
    inline std::wstring utf8_to_wide_lossy(std::string_view text) {
        if (text.empty()) return {};
        const int needed = ::MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
        if (needed <= 0) {
            return std::wstring(text.begin(), text.end());
        }
        std::wstring out(static_cast<std::size_t>(needed), L'\0');
        const int written = ::MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), out.data(), needed);
        if (written <= 0) {
            return std::wstring(text.begin(), text.end());
        }
        return out;
    }

    inline std::filesystem::path utf8_path(std::string_view text) {
        return std::filesystem::path(utf8_to_wide_lossy(text));
    }

    [[noreturn]] inline void 报告低级错误并终止_(
        std::string_view 模块,
        std::string_view 函数,
        std::string_view 细节) {
        static std::atomic_bool 已报告{ false };
        const bool 首次 = !已报告.exchange(true, std::memory_order_acq_rel);

        std::string utf8{};
        utf8.reserve(256 + 模块.size() + 函数.size() + 细节.size());
        utf8.append("检测到低级错误，请立即修复。\n\n模块: ");
        utf8.append(模块);
        utf8.append("\n函数: ");
        utf8.append(函数);
        if (!细节.empty()) {
            utf8.append("\n细节: ");
            utf8.append(细节);
        }

        const auto 标题 = utf8_to_wide_lossy("低级错误");
        const auto 正文 = utf8_to_wide_lossy(utf8);
        ::OutputDebugStringW(正文.c_str());
        try {
            ::CreateDirectoryW(L".\\日志", nullptr);
            std::ofstream 紧急日志(".\\日志\\鱼巢_popup_fatal.log", std::ios::app);
            if (紧急日志) {
                紧急日志 << utf8 << "\n";
            }
        }
        catch (...) {
        }
        if (首次) {
            ::MessageBoxW(nullptr, 正文.c_str(), 标题.c_str(), MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
        }

        std::terminate();
    }

    struct 单文件日志器 {
        std::mutex mtx;
        std::ofstream ofs;
        std::string 当前日期; // YYYYMMDD
        std::filesystem::path 根目录;
        std::string 文件前缀;
        std::string 文件标识; // "run" / "exception"
        bool 每条刷新 = true;
        bool 输出到调试器 = false;

        static std::string 取日期_YYYYMMDD() {
            using namespace std::chrono;
            auto now = system_clock::now();
            std::time_t tt = system_clock::to_time_t(now);
            std::tm tm{};
#if defined(_WIN32)
            localtime_s(&tm, &tt);
#else
            localtime_r(&tt, &tm);
#endif
            char buf[16]{};
            std::snprintf(buf, sizeof(buf), "%04d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
            return std::string(buf);
        }

        static std::string 取时间戳_YYYYMMDD_hhmmss_mmm() {
            using namespace std::chrono;
            auto now = system_clock::now();
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            std::time_t tt = system_clock::to_time_t(now);
            std::tm tm{};
#if defined(_WIN32)
            localtime_s(&tm, &tt);
#else
            localtime_r(&tt, &tm);
#endif
            char buf[64]{};
            std::snprintf(buf, sizeof(buf),
                "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                (int)ms.count());
            return std::string(buf);
        }

        static const char* 级别字符串(枚举_日志级别 lv) {
            switch (lv) {
            case 枚举_日志级别::调试: return "DBG";
            case 枚举_日志级别::信息: return "INF";
            case 枚举_日志级别::警告: return "WRN";
            case 枚举_日志级别::错误: return "ERR";
            case 枚举_日志级别::致命: return "FTL";
            default: return "UNK";
            }
        }

        void 配置(const 日志参数& p, std::string 标识) {
            根目录 = p.根目录;
            文件前缀 = p.文件前缀;
            文件标识 = std::move(标识);
            每条刷新 = p.每条刷新;
            输出到调试器 = p.输出到调试器;
        }

        std::filesystem::path 生成文件路径(const std::string& yyyymmdd) const {
            // logs/海鱼_run_20260118.低值g
            std::string name = std::format("{}_{}_{}.低值g", 文件前缀, 文件标识, yyyymmdd);
            return 根目录 / utf8_path(name);
        }

        void 确保打开_已加锁() {
            const std::string today = 取日期_YYYYMMDD();
            if (ofs.is_open() && today == 当前日期) return;

            // 切日 / 首次
            try {
                std::filesystem::create_directories(根目录);
            }
            catch (const std::exception& e) {
                报告低级错误并终止_("日志模块", "日志::detail::单文件日志器::确保打开_已加锁/create_directories", e.what());
            }
            catch (...) {
                报告低级错误并终止_("日志模块", "日志::detail::单文件日志器::确保打开_已加锁/create_directories", "未知异常");
            }

            try {
                if (ofs.is_open()) ofs.close();
                当前日期 = today;
                auto path = 生成文件路径(today);
                ofs.open(path, std::ios::out | std::ios::app);
            }
            catch (const std::exception& e) {
                报告低级错误并终止_("日志模块", "日志::detail::单文件日志器::确保打开_已加锁/open", e.what());
            }
            catch (...) {
                报告低级错误并终止_("日志模块", "日志::detail::单文件日志器::确保打开_已加锁/open", "未知异常");
            }
        }

        void 写一行(枚举_日志级别 lv, std::string_view msg) {
            std::scoped_lock lk(mtx);
            确保打开_已加锁();
            if (!ofs.is_open()) return;

            // thread id
            std::ostringstream tid;
            tid << std::this_thread::get_id();
            const auto 线程ID文本 = tid.str();

            const std::string ts = 取时间戳_YYYYMMDD_hhmmss_mmm();
            std::string 行文本{};
            行文本.reserve(ts.size() + 线程ID文本.size() + msg.size() + 32);
            行文本.append(ts);
            行文本.append(" [");
            行文本.append(级别字符串(lv));
            行文本.append("] [T");
            行文本.append(线程ID文本);
            行文本.append("] ");
            行文本.append(msg);

            ofs << 行文本 << "\n";

            if (每条刷新) ofs.flush();

            if (输出到调试器) {
                const auto 调试文本 = utf8_to_wide_lossy(行文本 + "\n");
                ::OutputDebugStringW(调试文本.c_str());
            }
        }

        void 关闭() {
            std::scoped_lock lk(mtx);
            try {
                if (ofs.is_open()) ofs.close();
            }
            catch (const std::exception& e) {
                报告低级错误并终止_("日志模块", "日志::detail::单文件日志器::关闭", e.what());
            }
            catch (...) {
                报告低级错误并终止_("日志模块", "日志::detail::单文件日志器::关闭", "未知异常");
            }
            std::string{}.swap(当前日期);
            std::filesystem::path{}.swap(根目录);
            std::string{}.swap(文件前缀);
            std::string{}.swap(文件标识);
            每条刷新 = true;
            输出到调试器 = false;
        }
    };

    inline 单文件日志器& g_run() {
        static 单文件日志器 实例{};
        return 实例;
    }

    inline 单文件日志器& g_ex() {
        static 单文件日志器 实例{};
        return 实例;
    }

    inline 单文件日志器& g_selfcheck_orphan() {
        static 单文件日志器 实例{};
        return 实例;
    }

    inline std::mutex& g_init_mtx() {
        static std::mutex 实例{};
        return 实例;
    }

    inline bool& g_inited() {
        static bool 实例 = false;
        return 实例;
    }

    inline std::atomic_bool& g_shutdown() {
        static std::atomic_bool 实例 = false;
        return 实例;
    }

    inline 日志参数& g_param() {
        static 日志参数 实例{};
        return 实例;
    }

    inline void 确保初始化_已加锁() {
        if (g_inited()) return;
        g_run().配置(g_param(), "run");
        g_ex().配置(g_param(), "exception");
        g_selfcheck_orphan().配置(g_param(), "selfcheck_orphan_requirement");
        g_inited() = true;
    }
}

namespace 日志 {
    inline void 初始化(const 日志参数& p) {
        std::scoped_lock lk(detail::g_init_mtx());
        detail::g_param() = p;
        detail::g_shutdown().store(false, std::memory_order_release);
        detail::g_inited() = false;
        detail::确保初始化_已加锁();
    }

    inline void 关闭() {
        std::scoped_lock lk(detail::g_init_mtx());
        detail::g_shutdown().store(true, std::memory_order_release);
        if (!detail::g_inited()) return;
        detail::g_run().关闭();
        detail::g_ex().关闭();
        detail::g_selfcheck_orphan().关闭();
        detail::g_param() = 日志参数{};
        std::filesystem::path{}.swap(detail::g_param().根目录);
        std::string{}.swap(detail::g_param().文件前缀);
        detail::g_inited() = false;
    }

    inline void 写(枚举_日志类别 类别, 枚举_日志级别 级别, std::string_view msg) {
        if (detail::g_shutdown().load(std::memory_order_acquire)) {
            return;
        }

        // 无论是否显式初始化，都能工作：用默认参数懒初始化
        if (!detail::g_inited()) {
            // 使用try_lock避免死锁
            if (detail::g_init_mtx().try_lock()) {
                if (!detail::g_shutdown().load(std::memory_order_acquire) && !detail::g_inited()) {
                    detail::确保初始化_已加锁();
                }
                detail::g_init_mtx().unlock();
            } else {
                // 如果无法获取锁，直接返回，避免死锁
                return;
            }
        }

        try {
            if (类别 == 枚举_日志类别::运行) {
                detail::g_run().写一行(级别, msg);
            }
            else {
                detail::g_ex().写一行(级别, msg);
            }
        }
        catch (const std::exception& e) {
            detail::报告低级错误并终止_("日志模块", "日志::写", e.what());
        }
        catch (...) {
            detail::报告低级错误并终止_("日志模块", "日志::写", "未知异常");
        }
    }

    inline void 记录异常(const std::exception& e, std::string_view 上下文) {
        if (上下文.empty()) {
            写(枚举_日志类别::异常, 枚举_日志级别::错误, std::string_view(e.what()));
        }
        else {
            写(枚举_日志类别::异常, 枚举_日志级别::错误, std::format("{} | {}", 上下文, e.what()));
        }
    }
}

void 初始化项目日志() noexcept
{
    try {
        日志参数 参数{};
        参数.根目录 = std::filesystem::path(L"./日志");
        参数.文件前缀 = "鱼巢";
        参数.每条刷新 = true;
        参数.输出到调试器 = false;
        日志::初始化(参数);
    }
    catch (...) {
    }
}

void 关闭项目日志() noexcept
{
    try {
        日志::关闭();
    }
    catch (...) {
    }
}

void 项目运行日志(const std::string& 文本) noexcept
{
    try {
        日志::运行(文本);
    }
    catch (...) {
    }
}

void 项目运行警告日志(const std::string& 文本) noexcept
{
    try {
        日志::运行_警告(文本);
    }
    catch (...) {
    }
}

void 项目运行错误日志(const std::string& 文本) noexcept
{
    try {
        日志::运行_错误(文本);
    }
    catch (...) {
    }
}

void 项目弹窗错误提示(const std::string& 标题, const std::string& 文本) noexcept
{
    try {
        日志::运行_错误("弹窗错误提示 | 标题=" + 标题 + " | 文本=" + 文本);
        const auto 宽标题 = 日志::detail::utf8_to_wide_lossy(标题);
        const auto 宽文本 = 日志::detail::utf8_to_wide_lossy(文本);
        ::MessageBoxW(nullptr, 宽文本.c_str(), 宽标题.c_str(), MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
    }
    catch (...) {
    }
}

void 项目自检无上级需求日志(const std::string& 文本) noexcept
{
    try {
        if (日志::detail::g_shutdown().load(std::memory_order_acquire)) {
            return;
        }
        if (!日志::detail::g_inited()) {
            if (日志::detail::g_init_mtx().try_lock()) {
                if (!日志::detail::g_shutdown().load(std::memory_order_acquire)
                    && !日志::detail::g_inited()) {
                    日志::detail::确保初始化_已加锁();
                }
                日志::detail::g_init_mtx().unlock();
            }
            else {
                return;
            }
        }
        日志::detail::g_selfcheck_orphan().写一行(枚举_日志级别::警告, 文本);
    }
    catch (...) {
    }
}

void 项目记录异常日志(const std::exception& 异常, const std::string& 上下文) noexcept
{
    try {
        日志::记录异常(异常, 上下文);
    }
    catch (...) {
    }
}

void 项目致命日志(const std::string& 文本) noexcept
{
    try {
        日志::异常_致命(文本);
    }
    catch (...) {
    }
}

