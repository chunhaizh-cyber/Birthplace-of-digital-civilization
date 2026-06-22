module;

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

export module 线程生命周期消息模块;

import 全局共享函数类;
import 日志模块;

export enum class 枚举_线程生命周期消息事件 : std::uint8_t {
    创建 = 0,
    状态变化 = 1,
    退出前 = 2,
    已退出 = 3,
    故障 = 4,
    异常退出 = 5,
};

export struct 结构_线程生命周期消息 {
    std::uint64_t 消息ID = 0;
    枚举_线程生命周期消息事件 事件类型 = 枚举_线程生命周期消息事件::状态变化;
    std::uint64_t 发生时间 = 0;
    std::string 线程逻辑ID{};
    std::uint64_t 系统线程ID = 0;
    bool 自动填充当前系统线程ID = true;
    std::string 线程名称{};
    std::string 线程用途{};
    std::string 线程类别{};
    std::string 所属模块{};
    std::string 创建者逻辑ID{};
    std::string 创建者名称{};
    std::string 所属线程池ID{};
    std::string 所属线程池名称{};
    std::string 旧生命周期状态{};
    std::string 新生命周期状态{};
    std::string 旧运行状态{};
    std::string 新运行状态{};
    bool 是否堵塞 = false;
    bool 是否暂停 = false;
    bool 是否健康 = true;
    bool 是否正常退出 = false;
    std::uint64_t 关联任务ID = 0;
    std::uint64_t 关联工作项ID = 0;
    std::string 原因键{};
    std::string 显示摘要{};
};

export struct 结构_控制面板线程信息项 {
    std::uint64_t 首次消息ID = 0;
    std::uint64_t 最近消息ID = 0;
    std::uint64_t 创建时间 = 0;
    std::uint64_t 最近更新时间 = 0;
    std::uint64_t 退出时间 = 0;
    std::string 线程逻辑ID{};
    std::uint64_t 系统线程ID = 0;
    std::string 线程名称{};
    std::string 线程用途{};
    std::string 线程类别{};
    std::string 所属模块{};
    std::string 创建者逻辑ID{};
    std::string 创建者名称{};
    std::string 所属线程池ID{};
    std::string 所属线程池名称{};
    std::string 生命周期状态{};
    std::string 运行状态{};
    bool 是否堵塞 = false;
    bool 是否暂停 = false;
    bool 是否健康 = true;
    bool 是否已退出 = false;
    bool 是否故障 = false;
    bool 创建消息已到 = false;
    std::uint64_t 关联任务ID = 0;
    std::uint64_t 关联工作项ID = 0;
    std::string 最近事件类型{};
    std::string 最近事件键{};
    std::string 最近事件摘要{};
    std::uint64_t 版本 = 0;
    std::uint64_t 迟到消息数 = 0;
};

export const char* 线程生命周期消息事件文本(
    枚举_线程生命周期消息事件 事件) noexcept;

namespace {
    std::atomic_uint64_t g_线程生命周期消息序号{ 1 };
    std::mutex g_线程信息表锁;
    std::mutex g_消息中间件维护锁;
    std::unordered_map<std::string, 结构_控制面板线程信息项> g_线程信息表;
    bool g_线程信息表缓存已加载 = false;

    // 功能：服务所在模块的内部辅助流程。
    std::filesystem::path 私有_消息目录()
    {
        return std::filesystem::absolute(std::filesystem::path(L"消息中间件"));
    }

    // 功能：服务所在模块的内部辅助流程。
    std::filesystem::path 私有_已消费目录()
    {
        return 私有_消息目录() / std::filesystem::path(L"已消费");
    }

    // 功能：服务所在模块的内部辅助流程。
    std::filesystem::path 私有_归档目录()
    {
        return 私有_消息目录() / std::filesystem::path(L"归档");
    }

    // 功能：服务所在模块的内部辅助流程。
    std::filesystem::path 私有_线程信息表缓存路径()
    {
        return 私有_消息目录() / std::filesystem::path(L"control_panel_thread_info_table.cache");
    }

    constexpr auto 消息中间件临时文件保留时长 = std::chrono::minutes(5);
    constexpr auto 消息中间件已消费短期保留时长 = std::chrono::minutes(10);
    constexpr auto 消息中间件无效消息保留时长 = std::chrono::minutes(10);
    constexpr auto 消息中间件归档保留时长 = std::chrono::hours(24 * 30);
    constexpr std::uint64_t 消息归档最早可信发生时间 = 1577836800000000ull;

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_是终态事件(const 枚举_线程生命周期消息事件 事件) noexcept
    {
        return 事件 == 枚举_线程生命周期消息事件::已退出
            || 事件 == 枚举_线程生命周期消息事件::故障
            || 事件 == 枚举_线程生命周期消息事件::异常退出;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::uint64_t 私有_当前时间_微秒() noexcept
    {
        const auto now = std::chrono::system_clock::now().time_since_epoch();
        return static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::microseconds>(now).count());
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_转义字段值(std::string_view 文本)
    {
        std::string 输出;
        输出.reserve(文本.size());
        for (const char 字符 : 文本) {
            switch (字符) {
            case '\\':
                输出 += "\\\\";
                break;
            case '\n':
                输出 += "\\n";
                break;
            case '\r':
                输出 += "\\r";
                break;
            case '\t':
                输出 += "\\t";
                break;
            default:
                输出.push_back(字符);
                break;
            }
        }
        return 输出;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_反转义字段值(std::string_view 文本)
    {
        std::string 输出;
        输出.reserve(文本.size());
        bool 转义中 = false;
        for (const char 字符 : 文本) {
            if (!转义中) {
                if (字符 == '\\') {
                    转义中 = true;
                }
                else {
                    输出.push_back(字符);
                }
                continue;
            }

            switch (字符) {
            case 'n': 输出.push_back('\n'); break;
            case 'r': 输出.push_back('\r'); break;
            case 't': 输出.push_back('\t'); break;
            case '\\': 输出.push_back('\\'); break;
            default:
                输出.push_back(字符);
                break;
            }
            转义中 = false;
        }
        if (转义中) {
            输出.push_back('\\');
        }
        return 输出;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    std::uint64_t 私有_解析U64(const std::string& 文本) noexcept
    {
        if (文本.empty()) {
            return 0;
        }
        try {
            return static_cast<std::uint64_t>(std::stoull(文本));
        } catch (...) {
            return 0;
        }
    }

    // 功能：解析输入文本、消息、场景或运行包。
    bool 私有_解析布尔(const std::string& 文本) noexcept
    {
        return 文本 == "1" || 文本 == "true" || 文本 == "是";
    }

    // 功能：服务所在模块的内部辅助流程。
    const std::string& 私有_取字段(
        const std::unordered_map<std::string, std::string>& 字段,
        const char* 键) noexcept
    {
        static const std::string 空{};
        const auto it = 字段.find(键);
        return it == 字段.end() ? 空 : it->second;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_项字段键(const std::size_t 索引, const char* 键)
    {
        return std::string("项.") + std::to_string(索引) + "." + 键;
    }

    // 功能：服务所在模块的内部辅助流程。
    const std::string& 私有_取项字段(
        const std::unordered_map<std::string, std::string>& 字段,
        const std::size_t 索引,
        const char* 键) noexcept
    {
        static const std::string 空{};
        const auto it = 字段.find(私有_项字段键(索引, 键));
        return it == 字段.end() ? 空 : it->second;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    枚举_线程生命周期消息事件 私有_解析事件类型(
        const std::string& 文本,
        const std::string& 代码文本) noexcept
    {
        const auto 代码 = 私有_解析U64(代码文本);
        if (代码 <= static_cast<std::uint64_t>(枚举_线程生命周期消息事件::异常退出)
            && !代码文本.empty()) {
            return static_cast<枚举_线程生命周期消息事件>(代码);
        }
        if (文本 == "创建") return 枚举_线程生命周期消息事件::创建;
        if (文本 == "退出前") return 枚举_线程生命周期消息事件::退出前;
        if (文本 == "已退出") return 枚举_线程生命周期消息事件::已退出;
        if (文本 == "故障") return 枚举_线程生命周期消息事件::故障;
        if (文本 == "异常退出") return 枚举_线程生命周期消息事件::异常退出;
        return 枚举_线程生命周期消息事件::状态变化;
    }

    // 功能：从指定来源读取数据或状态。
    std::unordered_map<std::string, std::string> 私有_读取字段文件(
        const std::filesystem::path& 路径)
    {
        std::unordered_map<std::string, std::string> 字段{};
        std::ifstream 输入(路径, std::ios::binary);
        if (!输入) {
            return 字段;
        }

        std::string 行;
        while (std::getline(输入, 行)) {
            if (!行.empty() && 行.back() == '\r') {
                行.pop_back();
            }
            const auto 分隔 = 行.find('=');
            if (分隔 == std::string::npos) {
                continue;
            }
            字段.emplace(
                行.substr(0, 分隔),
                私有_反转义字段值(std::string_view(行).substr(分隔 + 1)));
        }
        return 字段;
    }

    // 功能：服务所在模块的内部辅助流程。
    结构_线程生命周期消息 私有_从字段构造消息(
        const std::unordered_map<std::string, std::string>& 字段)
    {
        结构_线程生命周期消息 消息{};
        消息.消息ID = 私有_解析U64(私有_取字段(字段, "消息ID"));
        消息.事件类型 = 私有_解析事件类型(
            私有_取字段(字段, "事件类型"),
            私有_取字段(字段, "事件类型代码"));
        消息.发生时间 = 私有_解析U64(私有_取字段(字段, "发生时间"));
        消息.线程逻辑ID = 私有_取字段(字段, "线程逻辑ID");
        消息.系统线程ID = 私有_解析U64(私有_取字段(字段, "系统线程ID"));
        消息.线程名称 = 私有_取字段(字段, "线程名称");
        消息.线程用途 = 私有_取字段(字段, "线程用途");
        消息.线程类别 = 私有_取字段(字段, "线程类别");
        消息.所属模块 = 私有_取字段(字段, "所属模块");
        消息.创建者逻辑ID = 私有_取字段(字段, "创建者逻辑ID");
        消息.创建者名称 = 私有_取字段(字段, "创建者名称");
        消息.所属线程池ID = 私有_取字段(字段, "所属线程池ID");
        消息.所属线程池名称 = 私有_取字段(字段, "所属线程池名称");
        消息.旧生命周期状态 = 私有_取字段(字段, "旧生命周期状态");
        消息.新生命周期状态 = 私有_取字段(字段, "新生命周期状态");
        消息.旧运行状态 = 私有_取字段(字段, "旧运行状态");
        消息.新运行状态 = 私有_取字段(字段, "新运行状态");
        消息.是否堵塞 = 私有_解析布尔(私有_取字段(字段, "是否堵塞"));
        消息.是否暂停 = 私有_解析布尔(私有_取字段(字段, "是否暂停"));
        消息.是否健康 = !字段.contains("是否健康")
            || 私有_解析布尔(私有_取字段(字段, "是否健康"));
        消息.是否正常退出 = 私有_解析布尔(私有_取字段(字段, "是否正常退出"));
        消息.关联任务ID = 私有_解析U64(私有_取字段(字段, "关联任务ID"));
        消息.关联工作项ID = 私有_解析U64(私有_取字段(字段, "关联工作项ID"));
        消息.原因键 = 私有_取字段(字段, "原因键");
        消息.显示摘要 = 私有_取字段(字段, "显示摘要");
        return 消息;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_写字段(std::ofstream& 输出, std::string_view 键, std::string_view 值)
    {
        输出 << 键 << '=' << 私有_转义字段值(值) << '\n';
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_事件文件代码(const 枚举_线程生命周期消息事件 事件)
    {
        return std::to_string(static_cast<std::uint32_t>(事件));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_写项字段(
        std::ofstream& 输出,
        const std::size_t 索引,
        const char* 键,
        std::string_view 值)
    {
        私有_写字段(输出, 私有_项字段键(索引, 键), 值);
    }

    // 功能：服务所在模块的内部辅助流程。
    结构_控制面板线程信息项 私有_从缓存字段构造线程信息项(
        const std::unordered_map<std::string, std::string>& 字段,
        const std::size_t 索引)
    {
        结构_控制面板线程信息项 项{};
        项.首次消息ID = 私有_解析U64(私有_取项字段(字段, 索引, "首次消息ID"));
        项.最近消息ID = 私有_解析U64(私有_取项字段(字段, 索引, "最近消息ID"));
        项.创建时间 = 私有_解析U64(私有_取项字段(字段, 索引, "创建时间"));
        项.最近更新时间 = 私有_解析U64(私有_取项字段(字段, 索引, "最近更新时间"));
        项.退出时间 = 私有_解析U64(私有_取项字段(字段, 索引, "退出时间"));
        项.线程逻辑ID = 私有_取项字段(字段, 索引, "线程逻辑ID");
        项.系统线程ID = 私有_解析U64(私有_取项字段(字段, 索引, "系统线程ID"));
        项.线程名称 = 私有_取项字段(字段, 索引, "线程名称");
        项.线程用途 = 私有_取项字段(字段, 索引, "线程用途");
        项.线程类别 = 私有_取项字段(字段, 索引, "线程类别");
        项.所属模块 = 私有_取项字段(字段, 索引, "所属模块");
        项.创建者逻辑ID = 私有_取项字段(字段, 索引, "创建者逻辑ID");
        项.创建者名称 = 私有_取项字段(字段, 索引, "创建者名称");
        项.所属线程池ID = 私有_取项字段(字段, 索引, "所属线程池ID");
        项.所属线程池名称 = 私有_取项字段(字段, 索引, "所属线程池名称");
        项.生命周期状态 = 私有_取项字段(字段, 索引, "生命周期状态");
        项.运行状态 = 私有_取项字段(字段, 索引, "运行状态");
        项.是否堵塞 = 私有_解析布尔(私有_取项字段(字段, 索引, "是否堵塞"));
        项.是否暂停 = 私有_解析布尔(私有_取项字段(字段, 索引, "是否暂停"));
        项.是否健康 = !字段.contains(私有_项字段键(索引, "是否健康"))
            || 私有_解析布尔(私有_取项字段(字段, 索引, "是否健康"));
        项.是否已退出 = 私有_解析布尔(私有_取项字段(字段, 索引, "是否已退出"));
        项.是否故障 = 私有_解析布尔(私有_取项字段(字段, 索引, "是否故障"));
        项.创建消息已到 = 私有_解析布尔(私有_取项字段(字段, 索引, "创建消息已到"));
        项.关联任务ID = 私有_解析U64(私有_取项字段(字段, 索引, "关联任务ID"));
        项.关联工作项ID = 私有_解析U64(私有_取项字段(字段, 索引, "关联工作项ID"));
        项.最近事件类型 = 私有_取项字段(字段, 索引, "最近事件类型");
        项.最近事件键 = 私有_取项字段(字段, 索引, "最近事件键");
        项.最近事件摘要 = 私有_取项字段(字段, 索引, "最近事件摘要");
        项.版本 = 私有_解析U64(私有_取项字段(字段, 索引, "版本"));
        项.迟到消息数 = 私有_解析U64(私有_取项字段(字段, 索引, "迟到消息数"));
        return 项;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_加载线程信息表缓存_已持锁() noexcept
    {
        if (g_线程信息表缓存已加载) {
            return;
        }
        g_线程信息表缓存已加载 = true;

        try {
            const auto 路径 = 私有_线程信息表缓存路径();
            if (!std::filesystem::exists(路径)) {
                return;
            }

            const auto 字段 = 私有_读取字段文件(路径);
            const auto 项数 = 私有_解析U64(私有_取字段(字段, "项数"));
            for (std::uint64_t i = 0; i < 项数; ++i) {
                auto 缓存项 = 私有_从缓存字段构造线程信息项(
                    字段,
                    static_cast<std::size_t>(i));
                if (缓存项.线程逻辑ID.empty()) {
                    continue;
                }

                auto& 当前项 = g_线程信息表[缓存项.线程逻辑ID];
                if (当前项.线程逻辑ID.empty()
                    || 缓存项.最近更新时间 > 当前项.最近更新时间
                    || (缓存项.最近更新时间 == 当前项.最近更新时间
                        && 缓存项.最近消息ID > 当前项.最近消息ID)) {
                    当前项 = std::move(缓存项);
                }
            }
        } catch (const std::exception& 异常) {
            项目运行错误日志(
                std::string("线程生命周期消息/线程信息表缓存读取失败 | 异常=") + 异常.what());
        } catch (...) {
            项目运行错误日志("线程生命周期消息/线程信息表缓存读取失败 | 异常=未知");
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_写线程信息表缓存_已持锁() noexcept
    {
        try {
            const auto 目录 = 私有_消息目录();
            std::filesystem::create_directories(目录);
            const auto 正式路径 = 私有_线程信息表缓存路径();
            const auto 临时路径 = 目录 / std::filesystem::path(L"control_panel_thread_info_table.cache.tmp");

            std::vector<const 结构_控制面板线程信息项*> 项集;
            项集.reserve(g_线程信息表.size());
            for (const auto& [_, 项] : g_线程信息表) {
                项集.push_back(&项);
            }
            std::sort(
                项集.begin(),
                项集.end(),
                [](const auto* 左, const auto* 右) {
                    if (左->创建时间 != 右->创建时间) {
                        return 左->创建时间 < 右->创建时间;
                    }
                    return 左->线程逻辑ID < 右->线程逻辑ID;
                });

            {
                std::ofstream 输出(临时路径, std::ios::binary | std::ios::trunc);
                if (!输出) {
                    项目运行错误日志("线程生命周期消息/线程信息表缓存打开失败 | 路径=" + 临时路径.string());
                    return;
                }
                私有_写字段(输出, "版本", "1");
                私有_写字段(输出, "项数", std::to_string(项集.size()));
                for (std::size_t i = 0; i < 项集.size(); ++i) {
                    const auto& 项 = *项集[i];
                    私有_写项字段(输出, i, "首次消息ID", std::to_string(项.首次消息ID));
                    私有_写项字段(输出, i, "最近消息ID", std::to_string(项.最近消息ID));
                    私有_写项字段(输出, i, "创建时间", std::to_string(项.创建时间));
                    私有_写项字段(输出, i, "最近更新时间", std::to_string(项.最近更新时间));
                    私有_写项字段(输出, i, "退出时间", std::to_string(项.退出时间));
                    私有_写项字段(输出, i, "线程逻辑ID", 项.线程逻辑ID);
                    私有_写项字段(输出, i, "系统线程ID", std::to_string(项.系统线程ID));
                    私有_写项字段(输出, i, "线程名称", 项.线程名称);
                    私有_写项字段(输出, i, "线程用途", 项.线程用途);
                    私有_写项字段(输出, i, "线程类别", 项.线程类别);
                    私有_写项字段(输出, i, "所属模块", 项.所属模块);
                    私有_写项字段(输出, i, "创建者逻辑ID", 项.创建者逻辑ID);
                    私有_写项字段(输出, i, "创建者名称", 项.创建者名称);
                    私有_写项字段(输出, i, "所属线程池ID", 项.所属线程池ID);
                    私有_写项字段(输出, i, "所属线程池名称", 项.所属线程池名称);
                    私有_写项字段(输出, i, "生命周期状态", 项.生命周期状态);
                    私有_写项字段(输出, i, "运行状态", 项.运行状态);
                    私有_写项字段(输出, i, "是否堵塞", 布尔文本_一或零(项.是否堵塞));
                    私有_写项字段(输出, i, "是否暂停", 布尔文本_一或零(项.是否暂停));
                    私有_写项字段(输出, i, "是否健康", 布尔文本_一或零(项.是否健康));
                    私有_写项字段(输出, i, "是否已退出", 布尔文本_一或零(项.是否已退出));
                    私有_写项字段(输出, i, "是否故障", 布尔文本_一或零(项.是否故障));
                    私有_写项字段(输出, i, "创建消息已到", 布尔文本_一或零(项.创建消息已到));
                    私有_写项字段(输出, i, "关联任务ID", std::to_string(项.关联任务ID));
                    私有_写项字段(输出, i, "关联工作项ID", std::to_string(项.关联工作项ID));
                    私有_写项字段(输出, i, "最近事件类型", 项.最近事件类型);
                    私有_写项字段(输出, i, "最近事件键", 项.最近事件键);
                    私有_写项字段(输出, i, "最近事件摘要", 项.最近事件摘要);
                    私有_写项字段(输出, i, "版本", std::to_string(项.版本));
                    私有_写项字段(输出, i, "迟到消息数", std::to_string(项.迟到消息数));
                }
            }

            std::error_code ec;
            std::filesystem::remove(正式路径, ec);
            ec.clear();
            std::filesystem::rename(临时路径, 正式路径, ec);
            if (ec) {
                项目运行错误日志(
                    "线程生命周期消息/线程信息表缓存提交失败 | 错误=" + ec.message());
            }
        } catch (const std::exception& 异常) {
            项目运行错误日志(
                std::string("线程生命周期消息/线程信息表缓存写入失败 | 异常=") + 异常.what());
        } catch (...) {
            项目运行错误日志("线程生命周期消息/线程信息表缓存写入失败 | 异常=未知");
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_应用线程生命周期消息_已持锁(const 结构_线程生命周期消息& 消息)
    {
        if (消息.线程逻辑ID.empty()) {
            return;
        }

        auto& 项 = g_线程信息表[消息.线程逻辑ID];
        const bool 新终态消息 = 私有_是终态事件(消息.事件类型);
        const bool 旧项已终态 = 项.是否已退出 || 项.是否故障;
        if (旧项已终态 && !新终态消息 && 消息.发生时间 < 项.最近更新时间) {
            ++项.迟到消息数;
            项.最近事件摘要 = "迟到线程状态消息已保留但未覆盖终态";
            return;
        }

        if (项.线程逻辑ID.empty()) {
            项.线程逻辑ID = 消息.线程逻辑ID;
            项.首次消息ID = 消息.消息ID;
        }

        if (消息.事件类型 == 枚举_线程生命周期消息事件::创建) {
            项.创建消息已到 = true;
            if (项.创建时间 == 0) {
                项.创建时间 = 消息.发生时间;
            }
        }
        else if (项.创建时间 == 0) {
            项.创建时间 = 消息.发生时间;
        }

        if (消息.系统线程ID != 0) 项.系统线程ID = 消息.系统线程ID;
        if (!消息.线程名称.empty()) 项.线程名称 = 消息.线程名称;
        if (!消息.线程用途.empty()) 项.线程用途 = 消息.线程用途;
        if (!消息.线程类别.empty()) 项.线程类别 = 消息.线程类别;
        if (!消息.所属模块.empty()) 项.所属模块 = 消息.所属模块;
        if (!消息.创建者逻辑ID.empty()) 项.创建者逻辑ID = 消息.创建者逻辑ID;
        if (!消息.创建者名称.empty()) 项.创建者名称 = 消息.创建者名称;
        if (!消息.所属线程池ID.empty()) 项.所属线程池ID = 消息.所属线程池ID;
        if (!消息.所属线程池名称.empty()) 项.所属线程池名称 = 消息.所属线程池名称;
        if (!消息.新生命周期状态.empty()) 项.生命周期状态 = 消息.新生命周期状态;
        if (!消息.新运行状态.empty()) 项.运行状态 = 消息.新运行状态;
        项.是否堵塞 = 消息.是否堵塞;
        项.是否暂停 = 消息.是否暂停;
        项.是否健康 = 消息.是否健康;
        if (!新终态消息) {
            项.是否已退出 = false;
            项.是否故障 = false;
            项.退出时间 = 0;
        }
        if (消息.关联任务ID != 0) 项.关联任务ID = 消息.关联任务ID;
        if (消息.关联工作项ID != 0) 项.关联工作项ID = 消息.关联工作项ID;

        if (消息.事件类型 == 枚举_线程生命周期消息事件::故障
            || 消息.事件类型 == 枚举_线程生命周期消息事件::异常退出) {
            项.是否故障 = true;
            项.是否健康 = false;
            项.退出时间 = 消息.发生时间;
            if (项.生命周期状态.empty()) {
                项.生命周期状态 = "故障";
            }
        }
        if (消息.事件类型 == 枚举_线程生命周期消息事件::已退出
            || 消息.事件类型 == 枚举_线程生命周期消息事件::异常退出) {
            项.是否已退出 = true;
            项.退出时间 = 消息.发生时间;
            if (项.生命周期状态.empty()) {
                项.生命周期状态 = "已退出";
            }
        }

        项.最近消息ID = 消息.消息ID;
        项.最近更新时间 = 消息.发生时间;
        项.最近事件类型 = 线程生命周期消息事件文本(消息.事件类型);
        项.最近事件键 = 消息.原因键;
        项.最近事件摘要 = 消息.显示摘要;
        ++项.版本;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_是正式线程生命周期消息文件(const std::filesystem::directory_entry& 项) noexcept
    {
        if (!项.is_regular_file()) {
            return false;
        }
        const auto 路径 = 项.path();
        if (路径.extension() != L".msg") {
            return false;
        }
        const auto 文件名 = 路径.filename().wstring();
        return 文件名.rfind(L"thread_lifecycle_", 0) == 0;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_移动到已消费(const std::filesystem::path& 源路径) noexcept
    {
        try {
            std::filesystem::create_directories(私有_已消费目录());
            auto 目标路径 = 私有_已消费目录() / 源路径.filename();
            std::uint64_t 序号 = 1;
            while (std::filesystem::exists(目标路径)) {
                目标路径 = 私有_已消费目录()
                    / (源路径.stem().wstring()
                        + L"."
                        + std::to_wstring(序号++)
                        + 源路径.extension().wstring());
            }
            std::filesystem::rename(源路径, 目标路径);
        } catch (const std::exception& 异常) {
            项目运行错误日志(
                std::string("线程生命周期消息/消费后移动失败 | 异常=") + 异常.what());
        } catch (...) {
            项目运行错误日志("线程生命周期消息/消费后移动失败 | 异常=未知");
        }
    }

    template <class Duration>
    bool 私有_文件早于(
        const std::filesystem::path& 路径,
        const Duration 保留时长) noexcept
    {
        std::error_code ec;
        const auto 写入时间 = std::filesystem::last_write_time(路径, ec);
        if (ec) {
            return false;
        }
        const auto 当前时间 = std::filesystem::file_time_type::clock::now();
        return 当前时间 - 写入时间 > 保留时长;
    }

    // 功能：删除指定对象、状态或登记项。
    bool 私有_删除文件(
        const std::filesystem::path& 路径,
        const char* 原因,
        std::size_t& 删除失败数) noexcept
    {
        std::error_code ec;
        const bool 已删除 = std::filesystem::remove(路径, ec);
        if (ec) {
            ++删除失败数;
            项目运行错误日志(
                std::string("线程生命周期消息/消息中间件删除失败 | 原因=")
                + 原因
                + " | 路径="
                + 路径.string()
                + " | 错误="
                + ec.message());
            return false;
        }
        return 已删除;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_归档日期文本(const std::uint64_t 发生时间微秒) noexcept
    {
        const auto 有效时间 = 发生时间微秒 < 消息归档最早可信发生时间
            ? 私有_当前时间_微秒()
            : 发生时间微秒;
        const auto 秒值 = static_cast<std::time_t>(有效时间 / 1000000ull);
        std::tm 本地时间{};
#if defined(_WIN32)
        if (localtime_s(&本地时间, &秒值) != 0) {
            return "unknown";
        }
#else
        if (localtime_r(&秒值, &本地时间) == nullptr) {
            return "unknown";
        }
#endif
        std::ostringstream 输出;
        输出 << std::put_time(&本地时间, "%Y%m%d");
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_消息值得归档(const 结构_线程生命周期消息& 消息) noexcept
    {
        return 消息.事件类型 == 枚举_线程生命周期消息事件::创建
            || 消息.事件类型 == 枚举_线程生命周期消息事件::退出前
            || 私有_是终态事件(消息.事件类型)
            || 消息.是否堵塞
            || 消息.是否暂停
            || !消息.是否健康
            || 消息.关联任务ID != 0
            || 消息.关联工作项ID != 0
            || !消息.原因键.empty()
            || !消息.显示摘要.empty();
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_归档已消费消息文件(
        const std::filesystem::path& 源路径,
        const 结构_线程生命周期消息& 消息) noexcept
    {
        try {
            const auto 目录 = 私有_归档目录();
            std::filesystem::create_directories(目录);
            const auto 日期 = 私有_归档日期文本(消息.发生时间);
            const auto 归档路径 =
                目录 / std::filesystem::path(std::string("thread_lifecycle_") + 日期 + ".archive");

            std::ifstream 输入(源路径, std::ios::binary);
            if (!输入) {
                项目运行错误日志("线程生命周期消息/归档失败 | 原因=源文件打开失败 | 路径=" + 源路径.string());
                return false;
            }
            std::ofstream 输出(归档路径, std::ios::binary | std::ios::app);
            if (!输出) {
                项目运行错误日志("线程生命周期消息/归档失败 | 原因=归档文件打开失败 | 路径=" + 归档路径.string());
                return false;
            }

            输出 << "\n";
            私有_写字段(输出, "记录类型", "线程生命周期消息");
            私有_写字段(输出, "归档源文件", 源路径.filename().string());
            私有_写字段(输出, "归档时间", std::to_string(私有_当前时间_微秒()));
            私有_写字段(输出, "原始消息开始", "1");
            输出 << 输入.rdbuf();
            输出 << "\n";
            私有_写字段(输出, "记录结束", "线程生命周期消息");
            return true;
        } catch (const std::exception& 异常) {
            项目运行错误日志(
                std::string("线程生命周期消息/归档失败 | 异常=") + 异常.what());
            return false;
        } catch (...) {
            项目运行错误日志("线程生命周期消息/归档失败 | 异常=未知");
            return false;
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_维护消息中间件根目录(
        std::size_t& 删除临时文件数,
        std::size_t& 删除无效消息数,
        std::size_t& 删除失败数)
    {
        const auto 目录 = 私有_消息目录();
        if (!std::filesystem::exists(目录)) {
            return;
        }

        for (const auto& 项 : std::filesystem::directory_iterator(目录)) {
            if (!项.is_regular_file()) {
                continue;
            }

            const auto 路径 = 项.path();
            if (路径.extension() == L".tmp"
                && 私有_文件早于(路径, 消息中间件临时文件保留时长)
                && 私有_删除文件(路径, "陈旧临时文件", 删除失败数)) {
                ++删除临时文件数;
                continue;
            }

            if (!私有_是正式线程生命周期消息文件(项)) {
                continue;
            }

            const auto 字段 = 私有_读取字段文件(路径);
            const auto 消息 = 私有_从字段构造消息(字段);
            if ((消息.消息ID == 0 || 消息.线程逻辑ID.empty())
                && 私有_文件早于(路径, 消息中间件无效消息保留时长)
                && 私有_删除文件(路径, "根目录无效消息", 删除失败数)) {
                ++删除无效消息数;
            }
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_维护消息中间件已消费目录(
        std::size_t& 归档消息数,
        std::size_t& 删除已消费普通消息数,
        std::size_t& 删除无效消息数,
        std::size_t& 删除临时文件数,
        std::size_t& 归档失败数,
        std::size_t& 删除失败数)
    {
        const auto 目录 = 私有_已消费目录();
        if (!std::filesystem::exists(目录)) {
            return;
        }

        for (const auto& 项 : std::filesystem::directory_iterator(目录)) {
            if (!项.is_regular_file()) {
                continue;
            }

            const auto 路径 = 项.path();
            if (路径.extension() == L".tmp"
                && 私有_文件早于(路径, 消息中间件临时文件保留时长)
                && 私有_删除文件(路径, "已消费目录陈旧临时文件", 删除失败数)) {
                ++删除临时文件数;
                continue;
            }

            if (!私有_是正式线程生命周期消息文件(项)) {
                continue;
            }

            const auto 字段 = 私有_读取字段文件(路径);
            const auto 消息 = 私有_从字段构造消息(字段);
            if (消息.消息ID == 0 || 消息.线程逻辑ID.empty()) {
                if (私有_文件早于(路径, 消息中间件无效消息保留时长)
                    && 私有_删除文件(路径, "已消费目录无效消息", 删除失败数)) {
                    ++删除无效消息数;
                }
                continue;
            }

            if (私有_消息值得归档(消息)) {
                if (私有_归档已消费消息文件(路径, 消息)) {
                    ++归档消息数;
                    私有_删除文件(路径, "已归档已消费消息", 删除失败数);
                }
                else {
                    ++归档失败数;
                }
                continue;
            }

            if (私有_文件早于(路径, 消息中间件已消费短期保留时长)
                && 私有_删除文件(路径, "普通已消费消息", 删除失败数)) {
                ++删除已消费普通消息数;
            }
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_维护消息中间件归档目录(
        std::size_t& 删除过期归档数,
        std::size_t& 删除失败数)
    {
        const auto 目录 = 私有_归档目录();
        if (!std::filesystem::exists(目录)) {
            return;
        }

        for (const auto& 项 : std::filesystem::directory_iterator(目录)) {
            if (!项.is_regular_file()) {
                continue;
            }
            const auto 路径 = 项.path();
            if (路径.extension() == L".archive"
                && 私有_文件早于(路径, 消息中间件归档保留时长)
                && 私有_删除文件(路径, "过期归档", 删除失败数)) {
                ++删除过期归档数;
            }
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_维护消息中间件文件() noexcept
    {
        std::lock_guard<std::mutex> 锁(g_消息中间件维护锁);
        std::size_t 归档消息数 = 0;
        std::size_t 删除已消费普通消息数 = 0;
        std::size_t 删除无效消息数 = 0;
        std::size_t 删除临时文件数 = 0;
        std::size_t 删除过期归档数 = 0;
        std::size_t 归档失败数 = 0;
        std::size_t 删除失败数 = 0;
        try {
            私有_维护消息中间件根目录(
                删除临时文件数,
                删除无效消息数,
                删除失败数);
            私有_维护消息中间件已消费目录(
                归档消息数,
                删除已消费普通消息数,
                删除无效消息数,
                删除临时文件数,
                归档失败数,
                删除失败数);
            私有_维护消息中间件归档目录(
                删除过期归档数,
                删除失败数);
        } catch (const std::exception& 异常) {
            项目运行错误日志(
                std::string("线程生命周期消息/消息中间件维护失败 | 异常=") + 异常.what());
            return;
        } catch (...) {
            项目运行错误日志("线程生命周期消息/消息中间件维护失败 | 异常=未知");
            return;
        }

        const auto 变化数 =
            归档消息数
            + 删除已消费普通消息数
            + 删除无效消息数
            + 删除临时文件数
            + 删除过期归档数
            + 归档失败数
            + 删除失败数;
        if (变化数 == 0) {
            return;
        }

        项目运行日志(
            "线程生命周期消息/消息中间件维护 | 归档="
            + std::to_string(归档消息数)
            + " | 删除普通已消费="
            + std::to_string(删除已消费普通消息数)
            + " | 删除无效="
            + std::to_string(删除无效消息数)
            + " | 删除临时="
            + std::to_string(删除临时文件数)
            + " | 删除过期归档="
            + std::to_string(删除过期归档数)
            + " | 归档失败="
            + std::to_string(归档失败数)
            + " | 删除失败="
            + std::to_string(删除失败数));
    }
}

// 功能：按函数名执行对应处理。
export const char* 线程生命周期消息事件文本(
    const 枚举_线程生命周期消息事件 事件) noexcept
{
    switch (事件) {
    case 枚举_线程生命周期消息事件::创建: return "创建";
    case 枚举_线程生命周期消息事件::状态变化: return "状态变化";
    case 枚举_线程生命周期消息事件::退出前: return "退出前";
    case 枚举_线程生命周期消息事件::已退出: return "已退出";
    case 枚举_线程生命周期消息事件::故障: return "故障";
    case 枚举_线程生命周期消息事件::异常退出: return "异常退出";
    default: return "状态变化";
    }
}

// 功能：按函数名执行对应处理。
export std::uint64_t 线程生命周期当前时间_微秒() noexcept
{
    return 私有_当前时间_微秒();
}

// 功能：按函数名执行对应处理。
export std::uint64_t 当前系统线程ID_控制面板线程消息() noexcept
{
#if defined(_WIN32)
    return static_cast<std::uint64_t>(::GetCurrentThreadId());
#else
    return static_cast<std::uint64_t>(
        std::hash<std::thread::id>{}(std::this_thread::get_id()));
#endif
}

// 功能：按函数名执行对应处理。
export bool 发送线程生命周期消息(结构_线程生命周期消息 消息) noexcept
{
    if (消息.线程逻辑ID.empty()) {
        项目运行错误日志("线程生命周期消息/拒绝发送 | 原因=线程逻辑ID为空");
        return false;
    }

    if (消息.发生时间 == 0) {
        消息.发生时间 = 线程生命周期当前时间_微秒();
    }
    if (消息.消息ID == 0) {
        const auto 序号 = g_线程生命周期消息序号.fetch_add(1, std::memory_order_relaxed) % 1000;
        消息.消息ID = 消息.发生时间 * 1000ull + 序号;
    }
    if (消息.系统线程ID == 0 && 消息.自动填充当前系统线程ID) {
        消息.系统线程ID = 当前系统线程ID_控制面板线程消息();
    }
    if (消息.线程名称.empty()) {
        消息.线程名称 = 消息.线程逻辑ID;
    }
    if (消息.新生命周期状态.empty()) {
        switch (消息.事件类型) {
        case 枚举_线程生命周期消息事件::创建:
            消息.新生命周期状态 = "启动中";
            break;
        case 枚举_线程生命周期消息事件::退出前:
            消息.新生命周期状态 = "收尾中";
            break;
        case 枚举_线程生命周期消息事件::已退出:
            消息.新生命周期状态 = "已退出";
            break;
        case 枚举_线程生命周期消息事件::故障:
        case 枚举_线程生命周期消息事件::异常退出:
            消息.新生命周期状态 = "故障";
            break;
        default:
            消息.新生命周期状态 = "运行中";
            break;
        }
    }

    try {
        const auto 目录 = 私有_消息目录();
        std::filesystem::create_directories(目录);
        const auto 文件名 =
            std::wstring(L"thread_lifecycle_")
            + std::to_wstring(static_cast<std::uint32_t>(消息.事件类型))
            + L"_"
            + std::to_wstring(消息.发生时间)
            + L"_"
            + std::to_wstring(消息.消息ID);
        const auto 临时路径 = 目录 / (文件名 + L".tmp");
        const auto 正式路径 = 目录 / (文件名 + L".msg");

        {
            std::ofstream 输出(临时路径, std::ios::binary | std::ios::trunc);
            if (!输出) {
                项目运行错误日志("线程生命周期消息/临时文件打开失败 | 路径=" + 临时路径.string());
                return false;
            }
            私有_写字段(输出, "消息ID", std::to_string(消息.消息ID));
            私有_写字段(输出, "事件类型代码", 私有_事件文件代码(消息.事件类型));
            私有_写字段(输出, "事件类型", 线程生命周期消息事件文本(消息.事件类型));
            私有_写字段(输出, "发生时间", std::to_string(消息.发生时间));
            私有_写字段(输出, "线程逻辑ID", 消息.线程逻辑ID);
            私有_写字段(输出, "系统线程ID", std::to_string(消息.系统线程ID));
            私有_写字段(输出, "线程名称", 消息.线程名称);
            私有_写字段(输出, "线程用途", 消息.线程用途);
            私有_写字段(输出, "线程类别", 消息.线程类别);
            私有_写字段(输出, "所属模块", 消息.所属模块);
            私有_写字段(输出, "创建者逻辑ID", 消息.创建者逻辑ID);
            私有_写字段(输出, "创建者名称", 消息.创建者名称);
            私有_写字段(输出, "所属线程池ID", 消息.所属线程池ID);
            私有_写字段(输出, "所属线程池名称", 消息.所属线程池名称);
            私有_写字段(输出, "旧生命周期状态", 消息.旧生命周期状态);
            私有_写字段(输出, "新生命周期状态", 消息.新生命周期状态);
            私有_写字段(输出, "旧运行状态", 消息.旧运行状态);
            私有_写字段(输出, "新运行状态", 消息.新运行状态);
            私有_写字段(输出, "是否堵塞", 布尔文本_一或零(消息.是否堵塞));
            私有_写字段(输出, "是否暂停", 布尔文本_一或零(消息.是否暂停));
            私有_写字段(输出, "是否健康", 布尔文本_一或零(消息.是否健康));
            私有_写字段(输出, "是否正常退出", 布尔文本_一或零(消息.是否正常退出));
            私有_写字段(输出, "关联任务ID", std::to_string(消息.关联任务ID));
            私有_写字段(输出, "关联工作项ID", std::to_string(消息.关联工作项ID));
            私有_写字段(输出, "原因键", 消息.原因键);
            私有_写字段(输出, "显示摘要", 消息.显示摘要);
        }

        std::filesystem::rename(临时路径, 正式路径);
        return true;
    } catch (const std::exception& 异常) {
        项目运行错误日志(
            std::string("线程生命周期消息/写入失败 | 异常=") + 异常.what());
        return false;
    } catch (...) {
        项目运行错误日志("线程生命周期消息/写入失败 | 异常=未知");
        return false;
    }
}

// 功能：从指定来源读取数据或状态。
export std::vector<结构_控制面板线程信息项> 读取控制面板线程信息表快照()
{
    std::vector<结构_控制面板线程信息项> 输出;
    {
        std::lock_guard<std::mutex> 锁(g_线程信息表锁);
        私有_加载线程信息表缓存_已持锁();
        输出.reserve(g_线程信息表.size());
        for (const auto& [_, 项] : g_线程信息表) {
            输出.push_back(项);
        }
    }
    std::sort(
        输出.begin(),
        输出.end(),
        [](const auto& 左, const auto& 右) {
            if (左.创建时间 != 右.创建时间) {
                return 左.创建时间 < 右.创建时间;
            }
            return 左.线程逻辑ID < 右.线程逻辑ID;
        });
    return 输出;
}

// 功能：从指定来源读取数据或状态。
export std::vector<结构_控制面板线程信息项> 消费并读取控制面板线程信息表()
{
    struct 待消费项 {
        std::filesystem::path 路径{};
        结构_线程生命周期消息 消息{};
    };
    std::vector<待消费项> 待消费{};

    try {
        const auto 目录 = 私有_消息目录();
        if (std::filesystem::exists(目录)) {
            for (const auto& 项 : std::filesystem::directory_iterator(目录)) {
                if (!私有_是正式线程生命周期消息文件(项)) {
                    continue;
                }
                const auto 字段 = 私有_读取字段文件(项.path());
                auto 消息 = 私有_从字段构造消息(字段);
                if (消息.线程逻辑ID.empty() || 消息.消息ID == 0) {
                    项目运行错误日志("线程生命周期消息/消费跳过无效文件 | 路径=" + 项.path().string());
                    continue;
                }
                待消费.push_back(待消费项{ 项.path(), std::move(消息) });
            }
        }
    } catch (const std::exception& 异常) {
        项目运行错误日志(
            std::string("线程生命周期消息/扫描失败 | 异常=") + 异常.what());
    } catch (...) {
        项目运行错误日志("线程生命周期消息/扫描失败 | 异常=未知");
    }

    std::sort(
        待消费.begin(),
        待消费.end(),
        [](const 待消费项& 左, const 待消费项& 右) {
            if (左.消息.发生时间 != 右.消息.发生时间) {
                return 左.消息.发生时间 < 右.消息.发生时间;
            }
            return 左.消息.消息ID < 右.消息.消息ID;
        });

    {
        std::lock_guard<std::mutex> 锁(g_线程信息表锁);
        私有_加载线程信息表缓存_已持锁();
        for (const auto& 项 : 待消费) {
            私有_应用线程生命周期消息_已持锁(项.消息);
        }
        if (!待消费.empty()) {
            私有_写线程信息表缓存_已持锁();
        }
    }
    for (const auto& 项 : 待消费) {
        私有_移动到已消费(项.路径);
    }
    私有_维护消息中间件文件();

    return 读取控制面板线程信息表快照();
}
