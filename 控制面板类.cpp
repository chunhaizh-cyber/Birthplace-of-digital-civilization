#include "控制面板类.h"
#include "控制面板WebView2.h"

#include <algorithm>
#include <atomic>
#include <fstream>
#include <future>
#include <mutex>
#include <sstream>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <variant>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

#include "世界树类.h"
#include "自我类.h"
#include "自我线程类.h"
#include "本能方法类.h"
#include "语素类.h"
#include "需求类.h"
#include "任务类.h"
#include "方法类.h"
#include "任务管理任务模块.h"
#include "学习任务模块.h"
#include "学习调度模块.h"

namespace {
    constexpr UINT 私有_WM_刷新控制面板窗口 = WM_APP + 1;
    constexpr int 私有_ID_刷新按钮 = 1001;
    constexpr int 私有_ID_摘要文本框 = 1002;
    constexpr int 私有_ID_内容文本框 = 1003;
    constexpr int 私有_ID_页面标题 = 1004;
    constexpr int 私有_ID_树控件 = 1005;
    constexpr int 私有_ID_导航概览按钮 = 1101;
    constexpr int 私有_ID_导航世界树按钮 = 1102;
    constexpr int 私有_ID_导航需求树按钮 = 1103;
    constexpr int 私有_ID_导航任务树按钮 = 1104;
    constexpr int 私有_ID_导航方法树按钮 = 1105;
    constexpr int 私有_ID_导航线程事件按钮 = 1106;
    constexpr int 私有_ID_导航先天动作按钮 = 1107;
    constexpr wchar_t 私有_控制面板窗口类名[] = L"鱼巢控制面板独立窗口";

    enum class 枚举_控制面板页面 : int {
        概览 = 0,
        世界树 = 1,
        需求树 = 2,
        任务树 = 3,
        方法树 = 4,
        线程事件 = 5,
        先天动作 = 6,
    };

    struct 结构_控制面板窗口上下文 {
        HWND 页面标题 = nullptr;
        HWND 摘要文本框 = nullptr;
        HWND 内容文本框 = nullptr;
        HWND 树控件 = nullptr;
        HWND 刷新按钮 = nullptr;
        HWND 导航概览按钮 = nullptr;
        HWND 导航世界树按钮 = nullptr;
        HWND 导航需求树按钮 = nullptr;
        HWND 导航任务树按钮 = nullptr;
        HWND 导航方法树按钮 = nullptr;
        HWND 导航线程事件按钮 = nullptr;
        HWND 导航先天动作按钮 = nullptr;
        HFONT 文本字体 = nullptr;
        HFONT 标题字体 = nullptr;
        枚举_控制面板页面 当前页面 = 枚举_控制面板页面::概览;
        结构_控制面板快照 当前快照{};
        bool 已加载快照 = false;
    };

    std::mutex 私有_控制面板窗口互斥{};
    std::atomic<HWND> 私有_控制面板窗口句柄{ nullptr };
    std::atomic<int> 私有_控制面板启动诊断码{ 0 };

    std::string 私有_转义HTML(std::string_view 文本)
    {
        std::string 输出;
        输出.reserve(文本.size() + 16);
        for (const char ch : 文本) {
            switch (ch) {
            case '&': 输出 += "&amp;"; break;
            case '<': 输出 += "&lt;"; break;
            case '>': 输出 += "&gt;"; break;
            case '"': 输出 += "&quot;"; break;
            case '\'': 输出 += "&#39;"; break;
            default: 输出.push_back(ch); break;
            }
        }
        return 输出;
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

    std::string 私有_安全词(const 词性节点类* 词) noexcept
    {
        if (!词) return {};
        try {
            return 语素集.获取词(词);
        }
        catch (...) {
            return {};
        }
    }

    std::string 私有_基础节点标题(const 基础信息节点类* 节点);
    void 私有_追加列表预览(
        std::ostringstream& 输出,
        std::string_view 标题,
        const std::vector<结构_控制面板列表项>& 列表,
        std::size_t 上限);

    std::string 私有_安全文本(std::string 文本, const std::string& 回退)
    {
        return 文本.empty() ? 回退 : std::move(文本);
    }

    std::string 私有_精简节点标题(std::string 文本)
    {
        const auto 分隔位置 = 文本.find(" | ");
        if (分隔位置 != std::string::npos) {
            文本.erase(分隔位置);
        }
        return 文本;
    }

    void 私有_收集树标题映射(
        const 结构_控制面板树节点& 节点,
        std::unordered_map<std::uintptr_t, std::string>* 输出)
    {
        if (!输出) return;
        if (节点.节点指针 != 0 && !节点.文本.empty()) {
            输出->try_emplace(节点.节点指针, 私有_精简节点标题(节点.文本));
        }
        for (const auto& 子项 : 节点.子项) {
            私有_收集树标题映射(子项, 输出);
        }
    }

    std::string 私有_替换指针文本(
        std::string 文本,
        const std::unordered_map<std::uintptr_t, std::string>& 映射)
    {
        std::size_t 搜索位置 = 0;
        while ((搜索位置 = 文本.find("ptr#", 搜索位置)) != std::string::npos) {
            const auto 数字起点 = 搜索位置 + 4;
            auto 数字终点 = 数字起点;
            while (数字终点 < 文本.size() && std::isdigit(static_cast<unsigned char>(文本[数字终点]))) {
                ++数字终点;
            }

            if (数字终点 == 数字起点) {
                搜索位置 = 数字起点;
                continue;
            }

            std::uintptr_t 指针值 = 0;
            try {
                指针值 = static_cast<std::uintptr_t>(std::stoull(文本.substr(数字起点, 数字终点 - 数字起点)));
            }
            catch (...) {
                搜索位置 = 数字终点;
                continue;
            }

            const auto it = 映射.find(指针值);
            if (it == 映射.end() || it->second.empty()) {
                搜索位置 = 数字终点;
                continue;
            }

            文本.replace(搜索位置, 数字终点 - 搜索位置, it->second);
            搜索位置 += it->second.size();
        }
        return 文本;
    }

    void 私有_替换树内指针文本(
        结构_控制面板树节点* 节点,
        const std::unordered_map<std::uintptr_t, std::string>& 映射)
    {
        if (!节点) return;
        节点->文本 = 私有_替换指针文本(std::move(节点->文本), 映射);
        for (auto& 子项 : 节点->子项) {
            私有_替换树内指针文本(&子项, 映射);
        }
    }

    void 私有_替换列表内指针文本(
        std::vector<结构_控制面板列表项>* 列表,
        const std::unordered_map<std::uintptr_t, std::string>& 映射)
    {
        if (!列表) return;
        for (auto& 项 : *列表) {
            项.标题 = 私有_替换指针文本(std::move(项.标题), 映射);
            项.摘要 = 私有_替换指针文本(std::move(项.摘要), 映射);
        }
    }

    void 私有_替换快照内指针文本(结构_控制面板快照* 快照)
    {
        if (!快照) return;

        std::unordered_map<std::uintptr_t, std::string> 映射{};
        for (const auto& 项 : 快照->世界列表) {
            if (项.节点指针 != 0 && !项.标题.empty()) 映射.try_emplace(项.节点指针, 私有_精简节点标题(项.标题));
        }
        for (const auto& 项 : 快照->场景列表) {
            if (项.节点指针 != 0 && !项.标题.empty()) 映射.try_emplace(项.节点指针, 私有_精简节点标题(项.标题));
        }
        for (const auto& 项 : 快照->存在列表) {
            if (项.节点指针 != 0 && !项.标题.empty()) 映射.try_emplace(项.节点指针, 私有_精简节点标题(项.标题));
        }
        for (const auto& 项 : 快照->需求列表) {
            if (项.节点指针 != 0 && !项.标题.empty()) 映射.try_emplace(项.节点指针, 私有_精简节点标题(项.标题));
        }
        for (const auto& 项 : 快照->任务列表) {
            if (项.节点指针 != 0 && !项.标题.empty()) 映射.try_emplace(项.节点指针, 私有_精简节点标题(项.标题));
        }
        for (const auto& 项 : 快照->方法列表) {
            if (项.节点指针 != 0 && !项.标题.empty()) 映射.try_emplace(项.节点指针, 私有_精简节点标题(项.标题));
        }

        私有_收集树标题映射(快照->需求树根, &映射);
        私有_收集树标题映射(快照->任务树根, &映射);
        私有_收集树标题映射(快照->方法树根, &映射);

        私有_替换列表内指针文本(&快照->世界列表, 映射);
        私有_替换列表内指针文本(&快照->场景列表, 映射);
        私有_替换列表内指针文本(&快照->存在列表, 映射);
        私有_替换列表内指针文本(&快照->需求列表, 映射);
        私有_替换列表内指针文本(&快照->任务列表, 映射);
        私有_替换列表内指针文本(&快照->方法列表, 映射);
        私有_替换列表内指针文本(&快照->先天动作动态列表, 映射);
        私有_替换列表内指针文本(&快照->先天动作因果列表, 映射);

        私有_替换树内指针文本(&快照->世界树根, 映射);
        私有_替换树内指针文本(&快照->需求树根, 映射);
        私有_替换树内指针文本(&快照->任务树根, 映射);
        私有_替换树内指针文本(&快照->方法树根, 映射);

        快照->自我摘要 = 私有_替换指针文本(std::move(快照->自我摘要), 映射);
        快照->自我线程摘要 = 私有_替换指针文本(std::move(快照->自我线程摘要), 映射);
        快照->运行时摘要 = 私有_替换指针文本(std::move(快照->运行时摘要), 映射);
    }

    std::string 私有_世界类型文本(枚举_世界类型 世界类型)
    {
        switch (世界类型) {
        case 枚举_世界类型::现实世界: return "现实世界";
        case 枚举_世界类型::内部世界: return "内部世界";
        case 枚举_世界类型::文本世界: return "文本世界";
        case 枚举_世界类型::想象世界: return "想象世界";
        case 枚举_世界类型::记忆世界: return "记忆世界";
        case 枚举_世界类型::推理世界: return "推理世界";
        case 枚举_世界类型::虚拟世界: return "虚拟世界";
        default: return "其它";
        }
    }

    std::string 私有_任务状态文本(枚举_任务状态 状态)
    {
        switch (状态) {
        case 枚举_任务状态::未启动: return "未启动";
        case 枚举_任务状态::运行中: return "运行中";
        case 枚举_任务状态::挂起: return "挂起";
        case 枚举_任务状态::完成: return "完成";
        case 枚举_任务状态::失败: return "失败";
        case 枚举_任务状态::取消: return "取消";
        default: return "未定义";
        }
    }

    std::string 私有_任务节点种类文本(枚举_任务节点种类 种类)
    {
        switch (种类) {
        case 枚举_任务节点种类::头结点: return "头结点";
        case 枚举_任务节点种类::步骤节点: return "步骤节点";
        case 枚举_任务节点种类::结果节点: return "结果节点";
        default: return "未定义";
        }
    }

    std::string 私有_方法节点种类文本(枚举_方法节点种类 种类)
    {
        switch (种类) {
        case 枚举_方法节点种类::方法首节点: return "方法首";
        case 枚举_方法节点种类::方法条件节点: return "方法条件";
        case 枚举_方法节点种类::方法结果节点: return "方法结果";
        default: return "未定义";
        }
    }

    std::string 私有_线程生命周期文本(枚举_线程生命周期状态 状态)
    {
        switch (状态) {
        case 枚举_线程生命周期状态::未启动: return "未启动";
        case 枚举_线程生命周期状态::启动中: return "启动中";
        case 枚举_线程生命周期状态::运行中: return "运行中";
        case 枚举_线程生命周期状态::停止请求中: return "停止请求中";
        case 枚举_线程生命周期状态::收尾中: return "收尾中";
        case 枚举_线程生命周期状态::已停止: return "已停止";
        case 枚举_线程生命周期状态::故障: return "故障";
        default: return "未定义";
        }
    }

    std::string 私有_动作事件相位文本(枚举_动作事件相位 相位)
    {
        switch (相位) {
        case 枚举_动作事件相位::开始运行: return "开始运行";
        case 枚举_动作事件相位::完成: return "完成";
        case 枚举_动作事件相位::失败: return "失败";
        default: return "未定义";
        }
    }

    const 结构_本能方法元信息* 私有_查询内部先天动作元信息(枚举_本能方法ID 本能ID) noexcept
    {
        const auto* 元信息 = 本能方法类::查询元信息(本能ID);
        if (!元信息 || 元信息->分类 != 枚举_本能方法分类::内部先天动作) {
            return nullptr;
        }
        return 元信息;
    }

    const 结构_本能方法元信息* 私有_匹配内部先天动作元信息_按名称(std::string_view 动作名) noexcept
    {
        if (动作名.empty()) {
            return nullptr;
        }

        constexpr 枚举_本能方法ID 内部先天动作列表[] = {
            枚举_本能方法ID::自我_服务归零待机保护,
            枚举_本能方法ID::自我_定时衰减服务值,
            枚举_本能方法ID::自我_按双值重算服务衰减步长,
            枚举_本能方法ID::自我_按服务值重算安全增加步长,
            枚举_本能方法ID::自我_按服务值重算安全减少步长,
            枚举_本能方法ID::自我_安全低位回升,
            枚举_本能方法ID::自我_安全高位回落,
            枚举_本能方法ID::自我_风险安全回归,
        };

        for (const auto 本能ID : 内部先天动作列表) {
            const auto* 元信息 = 私有_查询内部先天动作元信息(本能ID);
            if (元信息 && 元信息->动作名称 && 动作名 == std::string_view{ 元信息->动作名称 }) {
                return 元信息;
            }
        }
        return nullptr;
    }

    const 结构_本能方法元信息* 私有_取动态对应内部先天动作元信息(const 动态节点主信息类* 动态主信息) noexcept
    {
        if (!动态主信息) {
            return nullptr;
        }

        if (const auto* 来源方法 = reinterpret_cast<const 方法类::节点类*>(动态主信息->来源动作.指针)) {
            const auto& 动作句柄 = 来源方法->主信息.动作句柄;
            if (动作句柄.类型 == 枚举_动作句柄类型::本能函数ID) {
                return 私有_查询内部先天动作元信息(static_cast<枚举_本能方法ID>(动作句柄.本能ID));
            }
        }

        return 私有_匹配内部先天动作元信息_按名称(私有_安全词(动态主信息->来源动作名));
    }

    const 结构_本能方法元信息* 私有_取因果对应内部先天动作元信息(const 因果实例主信息类* 因果主信息) noexcept
    {
        if (!因果主信息) {
            return nullptr;
        }

        if (const auto* 元信息 = 私有_匹配内部先天动作元信息_按名称(因果主信息->动作语义键)) {
            return 元信息;
        }
        return 私有_匹配内部先天动作元信息_按名称(私有_安全词(因果主信息->名称));
    }

    std::string 私有_状态引用标题(const 可解析引用<状态节点类>& 状态引用)
    {
        if (状态引用.指针) {
            return 私有_基础节点标题(reinterpret_cast<const 基础信息节点类*>(状态引用.指针));
        }
        if (!状态引用.主键.empty()) {
            return "状态#" + 状态引用.主键;
        }
        return "无";
    }

    std::string 私有_先天动作动态摘要(const 动态节点主信息类& 主信息)
    {
        std::ostringstream 输出;
        输出 << "相位 " << 私有_动作事件相位文本(主信息.来源动作相位);

        if (主信息.初始状态.指针 || !主信息.初始状态.主键.empty()) {
            输出 << " | 条件 " << 私有_状态引用标题(主信息.初始状态);
        }
        if (主信息.结果状态.指针 || !主信息.结果状态.主键.empty()) {
            输出 << " | 结果 " << 私有_状态引用标题(主信息.结果状态);
        }
        else {
            输出 << " | 本轮仅记录动作事实";
        }

        if (主信息.动态特征.指针 && 主信息.动态特征.指针->主信息) {
            输出 << " | 特征 "
                << 私有_安全文本(私有_安全词(主信息.动态特征.指针->主信息->类型), "未命名特征");
        }
        return 输出.str();
    }

    std::string 私有_先天动作因果摘要(const 因果实例节点类* 因果节点, const 因果实例主信息类& 主信息)
    {
        std::ostringstream 输出;
        const auto 自然语言 = 世界树.因果().生成实例因果自然语言(因果节点);
        if (!自然语言.empty()) {
            输出 << 自然语言 << " | ";
        }

        输出 << "条件 " << 主信息.条件状态.size()
            << " | 结果 " << 主信息.结果状态.size();

        if (主信息.主结果状态.指针 || !主信息.主结果状态.主键.empty()) {
            输出 << " | 主结果 " << 私有_状态引用标题(主信息.主结果状态);
        }
        if (主信息.动作时间 != 0) {
            输出 << " | 动作时间 " << 主信息.动作时间;
        }
        if (主信息.结果时间 != 0) {
            输出 << " | 结果时间 " << 主信息.结果时间;
        }
        return 输出.str();
    }

    void 私有_追加阶段事件预览(
        std::ostringstream& 输出,
        const std::vector<结构_自我运行阶段事件>& 事件列表,
        std::size_t 上限)
    {
        输出 << "自我线程阶段事件\n";
        if (事件列表.empty()) {
            输出 << "  - 暂无事件\n";
            return;
        }

        const std::size_t 总数 = 事件列表.size();
        const std::size_t 起点 = 总数 > 上限 ? (总数 - 上限) : 0;
        for (std::size_t 索引 = 起点; 索引 < 总数; ++索引) {
            const auto& 事件 = 事件列表[索引];
            输出 << "  - [" << 事件.时间 << "] "
                << 私有_安全文本(事件.阶段键, "未命名阶段");
            if (!事件.摘要.empty()) {
                输出 << " | " << 事件.摘要;
            }
            输出 << '\n';
        }
        if (起点 > 0) {
            输出 << "  - ... 省略 " << 起点 << " 条较早事件\n";
        }
    }

    std::string 私有_线程事件内容(const 结构_控制面板快照& 快照)
    {
        std::ostringstream 输出;
        输出 << "自我线程运行态\n";
        输出 << "  - 生命周期: " << 私有_线程生命周期文本(快照.自我线程生命周期) << '\n';
        输出 << "  - 已初始化: " << (快照.自我线程已初始化 ? "是" : "否") << '\n';
        输出 << "  - 运行中: " << (快照.自我线程运行中 ? "是" : "否") << '\n';
        输出 << "  - 健康运行: " << (快照.自我线程健康运行 ? "是" : "否") << '\n';
        输出 << "  - 首轮运行已完成: " << (快照.自我线程首轮运行已完成 ? "是" : "否") << '\n';
        输出 << "  - Tick计数: " << 快照.自我Tick计数 << '\n';
        输出 << "  - 最近Tick时间: " << 快照.自我最近Tick时间 << '\n';
        输出 << "  - 当前阶段: " << 私有_安全文本(快照.自我线程当前阶段, "无") << '\n';
        输出 << "  - 当前最终去向: " << 私有_安全文本(快照.自我线程当前最终去向, "无") << '\n';
        输出 << "  - 本次启动来自故障恢复: " << (快照.自我线程本次启动来自故障恢复 ? "是" : "否") << '\n';
        输出 << "  - 累计故障次数: " << 快照.自我线程累计故障次数 << '\n';
        输出 << "  - 累计恢复次数: " << 快照.自我线程累计恢复次数 << '\n';
        if (快照.自我线程上次故障时间 != 0) {
            输出 << "  - 上次故障时间: " << 快照.自我线程上次故障时间 << '\n';
        }
        if (!快照.自我线程上次故障摘要.empty()) {
            输出 << "  - 上次故障摘要: " << 快照.自我线程上次故障摘要 << '\n';
        }
        if (!快照.自我线程最近恢复摘要.empty()) {
            输出 << "  - 最近恢复摘要: " << 快照.自我线程最近恢复摘要 << '\n';
        }
        if (!快照.主循环归并摘要.empty()) {
            输出 << "  - 主循环归并: " << 快照.主循环归并摘要 << '\n';
        }
        if (!快照.运行时车道摘要.empty()) {
            输出 << "  - 运行时车道: " << 快照.运行时车道摘要 << '\n';
        }
        输出 << "  - 线程摘要: " << 私有_安全文本(快照.自我线程摘要, "无") << '\n';
        if (!快照.自我线程最近故障摘要.empty()) {
            输出 << "  - 最近故障: " << 快照.自我线程最近故障摘要 << '\n';
        }
        输出 << '\n';

        输出 << "阶段事件明细\n";
        if (快照.自我运行阶段事件.empty()) {
            输出 << "  - 暂无事件\n";
            return 输出.str();
        }

        for (const auto& 事件 : 快照.自我运行阶段事件) {
            输出 << "  - 时间: " << 事件.时间
                << " | 阶段: " << 私有_安全文本(事件.阶段键, "未命名阶段");
            if (!事件.摘要.empty()) {
                输出 << " | 摘要: " << 事件.摘要;
            }
            if (事件.当前主需求 != 0) {
                输出 << " | 主需求指针: " << 事件.当前主需求;
            }
            if (事件.当前主任务 != 0) {
                输出 << " | 主任务指针: " << 事件.当前主任务;
            }
            if (事件.当前主方法 != 0) {
                输出 << " | 主方法指针: " << 事件.当前主方法;
            }
            输出 << '\n';
        }

        return 输出.str();
    }

    std::string 私有_先天动作内容(const 结构_控制面板快照& 快照)
    {
        std::ostringstream 输出;
        输出 << "先天动作检查\n";
        输出 << "  - 先天动作动态数量: " << 快照.先天动作动态数 << '\n';
        输出 << "  - 先天动作因果数量: " << 快照.先天动作因果数 << '\n';
        输出 << "  - 本轮快照总动态数量: " << 快照.动态数 << '\n';
        输出 << "  - 本轮快照总因果实例数量: " << 快照.因果实例数 << '\n';
        输出 << '\n';

        私有_追加列表预览(输出, "先天动作动态预览", 快照.先天动作动态列表, 10);
        输出 << '\n';
        私有_追加列表预览(输出, "先天动作因果预览", 快照.先天动作因果列表, 10);
        return 输出.str();
    }

    std::string 私有_特征值文本(const 特征值& 值)
    {
        if (const auto* 标量 = std::get_if<I64>(&值)) {
            return std::to_string(*标量);
        }
        if (const auto* 向量句柄 = std::get_if<VecU句柄>(&值)) {
            return 向量句柄->有效()
                ? std::string("VecU#") + std::to_string(向量句柄->主信息指针)
                : "VecU#空";
        }
        if (const auto* 指针 = std::get_if<指针句柄>(&值)) {
            return 指针->有效()
                ? std::string("ptr#") + std::to_string(指针->指针)
                : "ptr#空";
        }
        return "空";
    }

    template<class T节点, class T回调>
    void 私有_枚举子节点(const T节点* 父节点, T回调&& 回调)
    {
        if (!父节点 || !父节点->子) return;

        auto* 起点 = static_cast<const T节点*>(父节点->子);
        auto* 当前 = 起点;
        do {
            回调(当前);
            当前 = static_cast<const T节点*>(当前->下);
        } while (当前 && 当前 != 起点);
    }

    template<class T节点, class T回调>
    void 私有_前序遍历独立树(const T节点* 父节点, T回调&& 回调)
    {
        私有_枚举子节点(父节点, [&](const T节点* 子节点) {
            回调(子节点);
            私有_前序遍历独立树(子节点, 回调);
        });
    }

    template<class T节点>
    std::size_t 私有_统计独立树节点数(const T节点* 根节点)
    {
        std::size_t 数量 = 0;
        私有_前序遍历独立树(根节点, [&](const T节点*) { ++数量; });
        return 数量;
    }

    std::string 私有_需求标题(const 需求类::节点类* 节点)
    {
        if (!节点) return "空需求";
        const auto& 主信息 = 节点->主信息;
        return 私有_安全文本(私有_安全词(主信息.类型), std::string("需求#") + 节点->获取主键());
    }

    std::string 私有_需求摘要(const 需求类::节点类* 节点)
    {
        if (!节点) return {};
        const auto& 主信息 = 节点->主信息;
        std::ostringstream 输出;
        输出 << "权重 " << 主信息.权重;
        const auto 相关任务 = 主信息.相关任务.指针
            ? reinterpret_cast<const 任务类::节点类*>(主信息.相关任务.指针)
            : nullptr;
        if (相关任务) {
            输出 << " | 相关任务 " << 私有_安全文本(私有_安全词(相关任务->主信息.名称), "未命名任务");
        }
        return 输出.str();
    }

    std::string 私有_任务标题(const 任务类::节点类* 节点)
    {
        if (!节点) return "空任务";
        const auto& 主信息 = 节点->主信息;
        return 私有_安全文本(私有_安全词(主信息.名称), std::string("任务#") + 节点->获取主键());
    }

    std::string 私有_任务摘要(const 任务类::节点类* 节点)
    {
        if (!节点) return {};
        const auto& 主信息 = 节点->主信息;
        std::ostringstream 输出;
        输出 << 私有_任务状态文本(主信息.状态)
            << " | " << 私有_任务节点种类文本(主信息.节点种类)
            << " | 调度优先级 " << 主信息.调度优先级;
        if (主信息.来源需求.指针) {
            const auto* 来源需求 = reinterpret_cast<const 需求类::节点类*>(主信息.来源需求.指针);
            输出 << " | 来源 " << 私有_需求标题(来源需求);
        }
        if (主信息.目标状态.指针 || !主信息.目标状态.主键.empty()) {
            输出 << " | 目标 " << 私有_状态引用标题(主信息.目标状态);
        }
        if (主信息.结果状态信息.指针 || !主信息.结果状态信息.主键.empty()) {
            输出 << " | 结果 " << 私有_状态引用标题(主信息.结果状态信息);
        }
        return 输出.str();
    }

    std::string 私有_方法标题(const 方法类::节点类* 节点)
    {
        if (!节点) return "空方法";
        const auto& 主信息 = 节点->主信息;
        return 私有_安全文本(私有_安全词(主信息.动作名), std::string("方法#") + 节点->获取主键());
    }

    std::string 私有_方法摘要(const 方法类::节点类* 节点)
    {
        if (!节点) return {};
        const auto& 主信息 = 节点->主信息;
        std::ostringstream 输出;
        输出 << 私有_方法节点种类文本(主信息.节点种类);
        if (主信息.来源任务.指针) {
            const auto* 来源任务 = reinterpret_cast<const 任务类::节点类*>(主信息.来源任务.指针);
            输出 << " | 来源 " << 私有_任务标题(来源任务);
        }
        if (主信息.动作句柄.有效()) {
            输出 << " | 动作句柄已绑定";
        }
        return 输出.str();
    }

    const char* 私有_布尔文本(bool 值) noexcept
    {
        return 值 ? "是" : "否";
    }

    std::string 私有_任务管理功能域文本(枚举_任务管理功能域 域)
    {
        switch (域) {
        case 枚举_任务管理功能域::筹办: return "筹办";
        case 枚举_任务管理功能域::维护: return "维护";
        case 枚举_任务管理功能域::纠偏: return "纠偏";
        case 枚举_任务管理功能域::收束: return "收束";
        case 枚举_任务管理功能域::等待: return "等待";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理缺口文本(枚举_任务管理缺口类型 缺口)
    {
        switch (缺口) {
        case 枚举_任务管理缺口类型::无缺口: return "无缺口";
        case 枚举_任务管理缺口类型::治理任务缺失: return "治理任务缺失";
        case 枚举_任务管理缺口类型::宿主任务缺失: return "宿主任务缺失";
        case 枚举_任务管理缺口类型::方法挂点缺失: return "方法挂点缺失";
        case 枚举_任务管理缺口类型::任务待启动: return "任务待启动";
        case 枚举_任务管理缺口类型::任务运行中: return "任务运行中";
        case 枚举_任务管理缺口类型::任务已终结: return "任务已终结";
        case 枚举_任务管理缺口类型::上位等待约束: return "上位等待约束";
        case 枚举_任务管理缺口类型::上位收束约束: return "上位收束约束";
        case 枚举_任务管理缺口类型::上位停止约束: return "上位停止约束";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理去向文本(枚举_任务管理下一步去向 去向)
    {
        switch (去向) {
        case 枚举_任务管理下一步去向::回到重筹办: return "回到重筹办";
        case 枚举_任务管理下一步去向::绑定方法: return "绑定方法";
        case 枚举_任务管理下一步去向::进入运行: return "进入运行";
        case 枚举_任务管理下一步去向::保持运行: return "保持运行";
        case 枚举_任务管理下一步去向::保持等待: return "保持等待";
        case 枚举_任务管理下一步去向::进入收束: return "进入收束";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理状态迁移文本(枚举_任务管理治理状态迁移 迁移)
    {
        switch (迁移) {
        case 枚举_任务管理治理状态迁移::回到重筹办: return "回到重筹办";
        case 枚举_任务管理治理状态迁移::进入挂起: return "进入挂起";
        case 枚举_任务管理治理状态迁移::进入运行: return "进入运行";
        case 枚举_任务管理治理状态迁移::保持运行: return "保持运行";
        case 枚举_任务管理治理状态迁移::保持等待: return "保持等待";
        case 枚举_任务管理治理状态迁移::进入收束: return "进入收束";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理总控结果文本(枚举_任务管理总控结果 结果)
    {
        switch (结果) {
        case 枚举_任务管理总控结果::治理任务已补齐: return "治理任务已补齐";
        case 枚举_任务管理总控结果::宿主待重筹办: return "宿主待重筹办";
        case 枚举_任务管理总控结果::宿主已挂起: return "宿主已挂起";
        case 枚举_任务管理总控结果::宿主已进入运行: return "宿主已进入运行";
        case 枚举_任务管理总控结果::宿主保持运行: return "宿主保持运行";
        case 枚举_任务管理总控结果::宿主保持等待: return "宿主保持等待";
        case 枚举_任务管理总控结果::宿主进入收束: return "宿主进入收束";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理根层重判结果文本(枚举_任务管理根层重判结果 结果)
    {
        switch (结果) {
        case 枚举_任务管理根层重判结果::允许普通推进: return "允许普通推进";
        case 枚举_任务管理根层重判结果::待机保持: return "待机保持";
        case 枚举_任务管理根层重判结果::收束优先: return "收束优先";
        case 枚举_任务管理根层重判结果::停止退出边界: return "停止退出边界";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理执行前门控结果文本(枚举_任务管理执行前门控结果 结果)
    {
        switch (结果) {
        case 枚举_任务管理执行前门控结果::允许普通执行: return "允许普通执行";
        case 枚举_任务管理执行前门控结果::仅允许等待: return "仅允许等待";
        case 枚举_任务管理执行前门控结果::仅允许收束: return "仅允许收束";
        case 枚举_任务管理执行前门控结果::禁止继续: return "禁止继续";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理反馈类型文本(枚举_任务管理反馈类型 类型)
    {
        switch (类型) {
        case 枚举_任务管理反馈类型::治理推进: return "治理推进";
        case 枚举_任务管理反馈类型::等待维持: return "等待维持";
        case 枚举_任务管理反馈类型::收束回执: return "收束回执";
        case 枚举_任务管理反馈类型::重筹办提示: return "重筹办提示";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理方法来源文本(枚举_任务管理方法来源 来源)
    {
        switch (来源) {
        case 枚举_任务管理方法来源::已挂方法复用: return "已挂方法复用";
        case 枚举_任务管理方法来源::当前主方法回填: return "当前主方法回填";
        case 枚举_任务管理方法来源::最近结果回看: return "最近结果回看";
        case 枚举_任务管理方法来源::方法树首节点候选: return "方法树首节点候选";
        case 枚举_任务管理方法来源::条件节点筛选通过: return "条件节点筛选通过";
        case 枚举_任务管理方法来源::默认本能兜底: return "默认本能兜底";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理步骤位类型文本(枚举_任务管理步骤位类型 类型)
    {
        switch (类型) {
        case 枚举_任务管理步骤位类型::执行位: return "执行位";
        case 枚举_任务管理步骤位类型::等待位: return "等待位";
        case 枚举_任务管理步骤位类型::回接位: return "回接位";
        case 枚举_任务管理步骤位类型::补条件位: return "补条件位";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理步骤语义类型文本(枚举_任务管理步骤语义类型 类型)
    {
        switch (类型) {
        case 枚举_任务管理步骤语义类型::治理步骤: return "治理步骤";
        case 枚举_任务管理步骤语义类型::业务推进步骤: return "业务推进步骤";
        case 枚举_任务管理步骤语义类型::等待维持步骤: return "等待维持步骤";
        case 枚举_任务管理步骤语义类型::回接派生步骤: return "回接派生步骤";
        case 枚举_任务管理步骤语义类型::补条件步骤: return "补条件步骤";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理双面比较结果文本(枚举_任务管理双面比较结果 类型)
    {
        switch (类型) {
        case 枚举_任务管理双面比较结果::待补齐: return "待补齐";
        case 枚举_任务管理双面比较结果::已对齐: return "已对齐";
        case 枚举_任务管理双面比较结果::存在差额: return "存在差额";
        default: return "未定义";
        }
    }

    std::string 私有_任务管理后果治理文本(枚举_任务管理后果治理 类型)
    {
        switch (类型) {
        case 枚举_任务管理后果治理::无额外后果: return "无额外后果";
        case 枚举_任务管理后果治理::补齐双面: return "补齐双面";
        case 枚举_任务管理后果治理::继续派生业务步骤: return "继续派生业务步骤";
        case 枚举_任务管理后果治理::保持等待: return "保持等待";
        case 枚举_任务管理后果治理::回接后保持等待: return "回接后保持等待";
        case 枚举_任务管理后果治理::回接后继续派生: return "回接后继续派生";
        case 枚举_任务管理后果治理::进入收束: return "进入收束";
        default: return "未定义";
        }
    }

    std::string 私有_基础节点标题(const 基础信息节点类* 节点)
    {
        return 世界树.获取名称(节点);
    }

    template<class T节点, class T标题Fn>
    void 私有_构建独立树节点(
        const T节点* 父节点,
        结构_控制面板树节点* 输出,
        std::size_t 当前深度,
        std::size_t 深度上限,
        std::size_t 广度上限,
        T标题Fn&& 标题函数)
    {
        if (!输出 || !父节点 || 当前深度 >= 深度上限) return;

        std::size_t 已添加 = 0;
        std::size_t 已省略 = 0;
        私有_枚举子节点(父节点, [&](const T节点* 子节点) {
            if (已添加 >= 广度上限) {
                ++已省略;
                return;
            }

            结构_控制面板树节点 子项{};
            子项.节点指针 = reinterpret_cast<std::uintptr_t>(子节点);
            子项.文本 = 标题函数(子节点);
            私有_构建独立树节点(
                子节点,
                &子项,
                当前深度 + 1,
                深度上限,
                广度上限,
                标题函数);
            输出->子项.push_back(std::move(子项));
            ++已添加;
        });

        if (已省略 > 0) {
            结构_控制面板树节点 省略项{};
            省略项.文本 = "... 省略 " + std::to_string(已省略) + " 个节点";
            输出->子项.push_back(std::move(省略项));
        }
    }

    void 私有_构建世界树节点(
        const 基础信息节点类* 父节点,
        结构_控制面板树节点* 输出,
        std::size_t 当前深度,
        std::size_t 深度上限,
        std::size_t 广度上限)
    {
        if (!输出 || !父节点 || 当前深度 >= 深度上限) return;

        const auto 子节点列表 = 世界树.获取子节点(父节点 == 世界树.世界根() ? nullptr : 父节点);
        std::size_t 已添加 = 0;
        for (auto* 子节点 : 子节点列表) {
            if (已添加 >= 广度上限) {
                break;
            }
            结构_控制面板树节点 子项{};
            子项.节点指针 = reinterpret_cast<std::uintptr_t>(子节点);
            子项.文本 = 私有_基础节点标题(子节点);
            私有_构建世界树节点(
                子节点,
                &子项,
                当前深度 + 1,
                深度上限,
                广度上限);
            输出->子项.push_back(std::move(子项));
            ++已添加;
        }
        if (子节点列表.size() > 已添加) {
            结构_控制面板树节点 省略项{};
            省略项.文本 = "... 省略 " + std::to_string(子节点列表.size() - 已添加) + " 个节点";
            输出->子项.push_back(std::move(省略项));
        }
    }

    template<class T节点, class T标题Fn, class T摘要Fn>
    void 私有_采集独立树列表(
        const T节点* 根节点,
        std::vector<结构_控制面板列表项>* 输出,
        T标题Fn&& 标题函数,
        T摘要Fn&& 摘要函数)
    {
        if (!输出 || !根节点) return;

        私有_前序遍历独立树(根节点, [&](const T节点* 节点) {
            结构_控制面板列表项 项{};
            项.节点指针 = reinterpret_cast<std::uintptr_t>(节点);
            项.标题 = 标题函数(节点);
            项.摘要 = 摘要函数(节点);
            输出->push_back(std::move(项));
        });
    }

    void 私有_渲染树前几行(
        const 结构_控制面板树节点& 节点,
        std::ostringstream& 输出,
        std::size_t 深度上限,
        std::size_t 行数上限,
        std::size_t 当前深度,
        std::size_t& 已输出行数)
    {
        if (已输出行数 >= 行数上限 || 当前深度 > 深度上限) return;

        输出 << std::string(当前深度 * 2, ' ') << "- " << 节点.文本 << '\n';
        ++已输出行数;
        for (const auto& 子项 : 节点.子项) {
            if (已输出行数 >= 行数上限) break;
            私有_渲染树前几行(子项, 输出, 深度上限, 行数上限, 当前深度 + 1, 已输出行数);
        }
    }

    std::string 私有_列表HTML(const std::vector<结构_控制面板列表项>& 列表, std::size_t 预览上限)
    {
        std::ostringstream 输出;
        输出 << "<table class='data-table'><thead><tr><th>标题</th><th>摘要</th></tr></thead><tbody>";
        if (列表.empty()) {
            输出 << "<tr><td colspan='2' class='muted'>暂无数据</td></tr>";
        }
        else {
            const auto 上限 = (std::min)(预览上限, 列表.size());
            for (std::size_t 索引 = 0; 索引 < 上限; ++索引) {
                const auto& 项 = 列表[索引];
                输出 << "<tr><td>" << 私有_转义HTML(项.标题)
                    << "</td><td>" << 私有_转义HTML(项.摘要) << "</td></tr>";
            }
            if (列表.size() > 上限) {
                输出 << "<tr><td colspan='2' class='muted'>... 省略 "
                    << (列表.size() - 上限) << " 项</td></tr>";
            }
        }
        输出 << "</tbody></table>";
        return 输出.str();
    }

    void 私有_追加树HTML(const 结构_控制面板树节点& 节点, std::ostringstream& 输出)
    {
        输出 << "<li>";
        if (节点.子项.empty()) {
            输出 << "<div class='tree-leaf'>" << 私有_转义HTML(节点.文本) << "</div>";
        }
        else {
            输出 << "<details open><summary>" << 私有_转义HTML(节点.文本) << "</summary><ul>";
            for (const auto& 子项 : 节点.子项) {
                私有_追加树HTML(子项, 输出);
            }
            输出 << "</ul></details>";
        }
        输出 << "</li>";
    }

    std::string 私有_树HTML(const 结构_控制面板树节点& 根节点)
    {
        std::ostringstream 输出;
        输出 << "<ul class='tree-root'>";
        私有_追加树HTML(根节点, 输出);
        输出 << "</ul>";
        return 输出.str();
    }

    结构_控制面板窗口上下文* 私有_取控制面板窗口上下文(HWND 窗口) noexcept
    {
        return reinterpret_cast<结构_控制面板窗口上下文*>(GetWindowLongPtrW(窗口, GWLP_USERDATA));
    }

    void 私有_显示控制面板窗口(HWND 窗口) noexcept
    {
        if (!窗口) return;
        if (IsIconic(窗口)) {
            ShowWindow(窗口, SW_RESTORE);
        }
        else {
            ShowWindow(窗口, SW_SHOW);
        }
        SetForegroundWindow(窗口);
    }

    void 私有_设置窗口文本(HWND 控件, const std::string& 文本)
    {
        if (!控件) return;
        const auto 宽文本 = 私有_UTF8转宽字串(文本);
        SetWindowTextW(控件, 宽文本.c_str());
    }

    bool 私有_页面使用树控件(枚举_控制面板页面 页面) noexcept
    {
        (void)页面;
        return false;
    }

    const 结构_控制面板树节点* 私有_页面对应树(
        const 结构_控制面板快照& 快照,
        枚举_控制面板页面 页面) noexcept
    {
        switch (页面) {
        case 枚举_控制面板页面::世界树:
            return &快照.世界树根;
        case 枚举_控制面板页面::需求树:
            return &快照.需求树根;
        case 枚举_控制面板页面::任务树:
            return &快照.任务树根;
        case 枚举_控制面板页面::方法树:
            return &快照.方法树根;
        default:
            return nullptr;
        }
    }

    HTREEITEM 私有_插入树控件节点(HWND 树控件, HTREEITEM 父项, const 结构_控制面板树节点& 节点)
    {
        if (!树控件) return nullptr;

        auto 宽文本 = 私有_UTF8转宽字串(节点.文本);
        TVINSERTSTRUCTW 插入结构{};
        插入结构.hParent = 父项 ? 父项 : TVI_ROOT;
        插入结构.hInsertAfter = TVI_LAST;
        插入结构.item.mask = TVIF_TEXT;
        插入结构.item.pszText = 宽文本.empty() ? const_cast<wchar_t*>(L"") : 宽文本.data();

        const HTREEITEM 当前项 = TreeView_InsertItem(树控件, &插入结构);
        for (const auto& 子项 : 节点.子项) {
            (void)私有_插入树控件节点(树控件, 当前项, 子项);
        }
        if (当前项) {
            TreeView_Expand(树控件, 当前项, TVE_EXPAND);
        }
        return 当前项;
    }

    void 私有_刷新树控件(HWND 树控件, const 结构_控制面板树节点* 根节点)
    {
        if (!树控件) return;
        TreeView_DeleteAllItems(树控件);
        if (!根节点) return;
        const HTREEITEM 根项 = 私有_插入树控件节点(树控件, nullptr, *根节点);
        if (根项) {
            TreeView_SelectItem(树控件, 根项);
            TreeView_EnsureVisible(树控件, 根项);
        }
    }

    void 私有_追加树文本(
        const 结构_控制面板树节点& 节点,
        std::ostringstream& 输出,
        const std::string& 前缀,
        bool 是否最后一个)
    {
        输出 << 前缀;
        if (!前缀.empty()) {
            输出 << (是否最后一个 ? "└─ " : "├─ ");
        }
        输出 << 节点.文本 << '\n';

        const std::string 子前缀 = 前缀 + (前缀.empty() ? "" : (是否最后一个 ? "   " : "│  "));
        for (std::size_t i = 0; i < 节点.子项.size(); ++i) {
            私有_追加树文本(
                节点.子项[i],
                输出,
                子前缀,
                i + 1 == 节点.子项.size());
        }
    }

    std::string 私有_渲染树文本(const 结构_控制面板树节点& 根节点)
    {
        std::ostringstream 输出;
        输出 << 根节点.文本 << '\n';
        for (std::size_t i = 0; i < 根节点.子项.size(); ++i) {
            私有_追加树文本(
                根节点.子项[i],
                输出,
                "",
                i + 1 == 根节点.子项.size());
        }
        return 输出.str();
    }

    void 私有_追加列表项(
        std::vector<结构_控制面板列表项>* 列表,
        std::uintptr_t 节点指针,
        std::string 标题,
        std::string 摘要)
    {
        if (!列表) {
            return;
        }

        结构_控制面板列表项 项{};
        项.节点指针 = 节点指针;
        项.标题 = std::move(标题);
        项.摘要 = std::move(摘要);
        列表->push_back(std::move(项));
    }

    void 私有_构建任务管理专项快照(结构_控制面板快照* 快照)
    {
        if (!快照) {
            return;
        }

        const auto 上下文 = 任务管理任务模块::读取任务管理上下文(自我);
        结构_任务管理结果 最近结果{};
        const bool 有最近治理结果 = 任务管理任务模块::读取最近治理结果(&最近结果);

        私有_追加列表项(
            &快照->任务管理输入列表,
            reinterpret_cast<std::uintptr_t>(上下文.当前主需求),
            "当前主需求",
            私有_需求标题(上下文.当前主需求));
        私有_追加列表项(
            &快照->任务管理输入列表,
            reinterpret_cast<std::uintptr_t>(上下文.当前管理任务),
            "当前管理任务",
            私有_任务标题(上下文.当前管理任务));
        私有_追加列表项(
            &快照->任务管理输入列表,
            reinterpret_cast<std::uintptr_t>(上下文.当前宿主任务),
            "当前宿主任务",
            私有_任务标题(上下文.当前宿主任务));
        私有_追加列表项(
            &快照->任务管理输入列表,
            reinterpret_cast<std::uintptr_t>(上下文.当前方法),
            "当前方法",
            私有_方法标题(上下文.当前方法));
        私有_追加列表项(
            &快照->任务管理输入列表,
            reinterpret_cast<std::uintptr_t>(上下文.当前步骤),
            "当前步骤",
            私有_任务标题(上下文.当前步骤));
        私有_追加列表项(
            &快照->任务管理输入列表,
            reinterpret_cast<std::uintptr_t>(上下文.最近结果),
            "最近结果",
            私有_任务标题(上下文.最近结果));
        私有_追加列表项(
            &快照->任务管理输入列表,
            0,
            "当前缺口类型",
            私有_任务管理缺口文本(上下文.当前缺口类型));
        私有_追加列表项(
            &快照->任务管理输入列表,
            0,
            "当前下一步去向",
            私有_任务管理去向文本(上下文.当前下一步去向));
        私有_追加列表项(
            &快照->任务管理输入列表,
            0,
            "最近根层重判结果",
            私有_任务管理根层重判结果文本(上下文.最近根层重判结果));
        私有_追加列表项(
            &快照->任务管理输入列表,
            0,
            "最近执行前门控结果",
            私有_任务管理执行前门控结果文本(上下文.最近执行前门控结果));
        私有_追加列表项(
            &快照->任务管理输入列表,
            0,
            "最近功能域",
            私有_任务管理功能域文本(上下文.最近功能域));
        私有_追加列表项(
            &快照->任务管理输入列表,
            0,
            "最近上层反馈摘要",
            上下文.最近上层反馈摘要.empty() ? "空" : 上下文.最近上层反馈摘要);
        私有_追加列表项(
            &快照->任务管理输入列表,
            0,
            "最近总控结果",
            私有_任务管理总控结果文本(上下文.最近总控结果));
        私有_追加列表项(
            &快照->任务管理输入列表,
            0,
            "最近反馈类型",
            私有_任务管理反馈类型文本(上下文.最近反馈类型));
        私有_追加列表项(
            &快照->任务管理输入列表,
            0,
            "最近反馈摘要",
            上下文.最近反馈摘要.empty() ? "空" : 上下文.最近反馈摘要);

        {
            std::ostringstream 输出;
            输出 << "主需求=" << 私有_需求标题(上下文.当前主需求)
                << " | 管理任务=" << 私有_任务标题(上下文.当前管理任务)
                << " | 宿主任务=" << 私有_任务标题(上下文.当前宿主任务)
                << " | 当前方法=" << 私有_方法标题(上下文.当前方法)
                << " | 当前步骤=" << 私有_任务标题(上下文.当前步骤)
                << " | 最近结果=" << 私有_任务标题(上下文.最近结果)
                << " | 缺口=" << 私有_任务管理缺口文本(上下文.当前缺口类型)
                << " | 去向=" << 私有_任务管理去向文本(上下文.当前下一步去向)
                << " | 根层重判=" << 私有_任务管理根层重判结果文本(上下文.最近根层重判结果)
                << " | 门控=" << 私有_任务管理执行前门控结果文本(上下文.最近执行前门控结果)
                << " | 最近域=" << 私有_任务管理功能域文本(上下文.最近功能域);
            if (!上下文.最近上层反馈摘要.empty()) {
                输出 << " | 上层反馈=" << 上下文.最近上层反馈摘要;
            }
            if (!上下文.最近反馈摘要.empty()) {
                输出 << " | 最近反馈=" << 上下文.最近反馈摘要;
            }
            快照->任务管理上下文摘要 = 输出.str();
        }

        if (!有最近治理结果) {
            快照->任务管理输出摘要 = "暂无最近治理结果缓存";
            快照->任务管理专项摘要 = 快照->任务管理上下文摘要 + " | 最近治理结果=空";
            return;
        }

        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "当前功能域",
            私有_任务管理功能域文本(最近结果.当前功能域));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "当前缺口类型",
            私有_任务管理缺口文本(最近结果.当前缺口类型));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "当前下一步去向",
            私有_任务管理去向文本(最近结果.当前下一步去向));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "当前治理状态迁移",
            私有_任务管理状态迁移文本(最近结果.当前治理状态迁移));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "当前方法来源",
            私有_任务管理方法来源文本(最近结果.当前方法来源));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "当前步骤位类型",
            私有_任务管理步骤位类型文本(最近结果.当前步骤位类型));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "当前步骤语义类型",
            私有_任务管理步骤语义类型文本(最近结果.当前步骤语义类型));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "最近根层重判结果",
            私有_任务管理根层重判结果文本(最近结果.最近根层重判结果));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "最近执行前门控结果",
            私有_任务管理执行前门控结果文本(最近结果.最近执行前门控结果));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "最近总控结果",
            私有_任务管理总控结果文本(最近结果.最近总控结果));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "最近反馈类型",
            私有_任务管理反馈类型文本(最近结果.最近反馈类型));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "宿主目标结果比较",
            私有_任务管理双面比较结果文本(最近结果.当前宿主目标结果比较));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "步骤目标结果比较",
            私有_任务管理双面比较结果文本(最近结果.当前步骤目标结果比较));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "结果目标结果比较",
            私有_任务管理双面比较结果文本(最近结果.当前结果目标结果比较));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "当前后果治理",
            私有_任务管理后果治理文本(最近结果.当前后果治理));
        私有_追加列表项(
            &快照->任务管理输出列表,
            reinterpret_cast<std::uintptr_t>(最近结果.当前子任务节点),
            "当前派生子任务",
            私有_任务标题(最近结果.当前子任务节点));
        私有_追加列表项(
            &快照->任务管理输出列表,
            reinterpret_cast<std::uintptr_t>(最近结果.当前学习方法节点),
            "当前学习方法",
            私有_方法标题(最近结果.当前学习方法节点));
        私有_追加列表项(
            &快照->任务管理输出列表,
            reinterpret_cast<std::uintptr_t>(最近结果.当前步骤节点),
            "当前步骤节点",
            私有_任务标题(最近结果.当前步骤节点));
        私有_追加列表项(
            &快照->任务管理输出列表,
            reinterpret_cast<std::uintptr_t>(最近结果.最新结果节点),
            "最新结果节点",
            私有_任务标题(最近结果.最新结果节点));
        私有_追加列表项(
            &快照->任务管理输出列表,
            reinterpret_cast<std::uintptr_t>(最近结果.最新目标状态),
            "最新目标状态",
            最近结果.最新目标状态 ? 私有_基础节点标题(reinterpret_cast<const 基础信息节点类*>(最近结果.最新目标状态)) : "无");
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "待学习方法数量",
            std::to_string(快照->自我待学习方法数量));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "学习账本总数",
            std::to_string(快照->学习任务总数));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "学习账本挂起数",
            std::to_string(快照->学习任务挂起数));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "学习账本可调度数",
            std::to_string(快照->学习任务可调度数));
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "学习账本摘要",
            快照->学习账本摘要.empty() ? "空" : 快照->学习账本摘要);
        {
            std::ostringstream 状态迁移摘要;
            状态迁移摘要 << 私有_任务状态文本(最近结果.宿主任务旧状态)
                << " -> " << 私有_任务状态文本(最近结果.宿主任务新状态);
            私有_追加列表项(
                &快照->任务管理输出列表,
                0,
                "宿主任务状态迁移",
                状态迁移摘要.str());
        }
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "最近上层反馈摘要",
            最近结果.最近上层反馈摘要.empty() ? "空" : 最近结果.最近上层反馈摘要);
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "最近反馈摘要",
            最近结果.最近反馈摘要.empty() ? "空" : 最近结果.最近反馈摘要);
        私有_追加列表项(
            &快照->任务管理输出列表,
            0,
            "治理摘要",
            最近结果.摘要.empty() ? "空" : 最近结果.摘要);

        私有_追加列表项(&快照->任务管理触发列表, 0, "补齐治理任务", 私有_布尔文本(最近结果.已补齐治理任务));
        私有_追加列表项(&快照->任务管理触发列表, 0, "选中宿主任务", 私有_布尔文本(最近结果.已选中宿主任务));
        私有_追加列表项(&快照->任务管理触发列表, 0, "绑定方法挂点", 私有_布尔文本(最近结果.已绑定方法挂点));
        私有_追加列表项(&快照->任务管理触发列表, 0, "推进任务状态", 私有_布尔文本(最近结果.已推进任务状态));
        私有_追加列表项(&快照->任务管理触发列表, 0, "创建步骤节点", 私有_布尔文本(最近结果.已创建步骤节点));
        私有_追加列表项(&快照->任务管理触发列表, 0, "创建结果节点", 私有_布尔文本(最近结果.已创建结果节点));
        私有_追加列表项(&快照->任务管理触发列表, 0, "创建业务子任务", 私有_布尔文本(最近结果.已创建业务子任务));
        私有_追加列表项(&快照->任务管理触发列表, 0, "复用业务子任务", 私有_布尔文本(最近结果.已复用业务子任务));
        私有_追加列表项(&快照->任务管理触发列表, 0, "创建补条件子任务", 私有_布尔文本(最近结果.已创建补条件子任务));
        私有_追加列表项(&快照->任务管理触发列表, 0, "复用补条件子任务", 私有_布尔文本(最近结果.已复用补条件子任务));
        私有_追加列表项(&快照->任务管理触发列表, 0, "创建学习子任务", 私有_布尔文本(最近结果.已创建学习子任务));
        私有_追加列表项(&快照->任务管理触发列表, 0, "复用学习子任务", 私有_布尔文本(最近结果.已复用学习子任务));
        私有_追加列表项(&快照->任务管理触发列表, 0, "创建学习方法头", 私有_布尔文本(最近结果.已创建学习方法头));
        私有_追加列表项(&快照->任务管理触发列表, 0, "复用学习方法头", 私有_布尔文本(最近结果.已复用学习方法头));
        私有_追加列表项(&快照->任务管理触发列表, 0, "创建学习方法条件节点", 私有_布尔文本(最近结果.已创建学习方法条件节点));
        私有_追加列表项(&快照->任务管理触发列表, 0, "创建学习方法结果节点", 私有_布尔文本(最近结果.已创建学习方法结果节点));
        私有_追加列表项(&快照->任务管理触发列表, 0, "命中最近结果", 私有_布尔文本(最近结果.已命中最近结果));
        私有_追加列表项(&快照->任务管理触发列表, 0, "复用当前步骤", 私有_布尔文本(最近结果.已复用当前步骤));
        私有_追加列表项(&快照->任务管理触发列表, 0, "生成治理证据", 私有_布尔文本(最近结果.已生成治理证据));
        私有_追加列表项(&快照->任务管理触发列表, 0, "写回目标状态", 私有_布尔文本(最近结果.已写回目标状态));
        私有_追加列表项(&快照->任务管理触发列表, 0, "由目标差额驱动", 私有_布尔文本(最近结果.已由目标差额驱动));
        私有_追加列表项(&快照->任务管理触发列表, 0, "受上位输入约束", 私有_布尔文本(最近结果.已受上位输入约束));

        {
            std::ostringstream 输出;
            输出 << "功能域=" << 私有_任务管理功能域文本(最近结果.当前功能域)
                << " | 缺口=" << 私有_任务管理缺口文本(最近结果.当前缺口类型)
                << " | 去向=" << 私有_任务管理去向文本(最近结果.当前下一步去向)
                << " | 子任务=" << 私有_任务标题(最近结果.当前子任务节点)
                << " | 学习方法=" << 私有_方法标题(最近结果.当前学习方法节点)
                << " | 学习账本=" << 快照->学习任务总数
                << "/" << 快照->学习任务挂起数
                << "/" << 快照->学习任务可调度数
                << " | 迁移=" << 私有_任务管理状态迁移文本(最近结果.当前治理状态迁移)
                << " | 方法来源=" << 私有_任务管理方法来源文本(最近结果.当前方法来源)
                << " | 步骤位=" << 私有_任务管理步骤位类型文本(最近结果.当前步骤位类型)
                << " | 宿主比较=" << 私有_任务管理双面比较结果文本(最近结果.当前宿主目标结果比较)
                << " | 后果治理=" << 私有_任务管理后果治理文本(最近结果.当前后果治理)
                << " | 语义=" << 私有_任务管理步骤语义类型文本(最近结果.当前步骤语义类型)
                << " | 根层重判=" << 私有_任务管理根层重判结果文本(最近结果.最近根层重判结果)
                << " | 门控=" << 私有_任务管理执行前门控结果文本(最近结果.最近执行前门控结果)
                << " | 总控=" << 私有_任务管理总控结果文本(最近结果.最近总控结果)
                << " | 反馈=" << 私有_任务管理反馈类型文本(最近结果.最近反馈类型);
            if (!最近结果.最近上层反馈摘要.empty()) {
                输出 << " | 上层反馈=" << 最近结果.最近上层反馈摘要;
            }
            if (!最近结果.最近反馈摘要.empty()) {
                输出 << " | 摘要=" << 最近结果.最近反馈摘要;
            }
            快照->任务管理输出摘要 = 输出.str();
        }

        快照->任务管理专项摘要 = 快照->任务管理上下文摘要
            + "\n"
            + 快照->任务管理输出摘要;
    }

    void 私有_构建学习专项快照(结构_控制面板快照* 快照)
    {
        if (!快照) {
            return;
        }

        const auto 学习任务列表 = 学习任务模块::枚举学习任务实体();
        const auto 学习调度快照 = 学习调度模块::构建学习调度快照();

        快照->学习就绪队列数 = 学习调度快照.就绪队列数;
        快照->学习等待表数 = 学习调度快照.等待表数;
        快照->学习调度摘要 = 学习调度快照.摘要;
        快照->学习任务执行中数 = 学习调度快照.执行中数;

        std::size_t 已完成数 = 0;
        for (const auto& 实体 : 学习任务列表) {
            if (实体.当前状态 == 枚举_学习状态::已提交
                || 实体.当前状态 == 枚举_学习状态::已回滚
                || 实体.当前状态 == 枚举_学习状态::已失败) {
                ++已完成数;
            }

            结构_控制面板列表项 项{};
            项.节点指针 = reinterpret_cast<std::uintptr_t>(实体.学习子任务);
            项.标题 = 实体.学习子任务标题.empty() ? "空学习任务" : 实体.学习子任务标题;
            项.摘要 = 实体.摘要;
            快照->学习任务列表.push_back(std::move(项));
        }
        快照->学习任务已完成数 = 已完成数;

        for (const auto& 调度项 : 学习调度快照.就绪队列) {
            结构_控制面板列表项 项{};
            项.节点指针 = reinterpret_cast<std::uintptr_t>(调度项.实体.学习子任务);
            项.标题 = 调度项.实体.学习子任务标题.empty() ? "空学习任务" : 调度项.实体.学习子任务标题;
            项.摘要 = 调度项.摘要;
            快照->学习就绪列表.push_back(std::move(项));
        }

        for (const auto& 调度项 : 学习调度快照.等待表) {
            结构_控制面板列表项 项{};
            项.节点指针 = reinterpret_cast<std::uintptr_t>(调度项.实体.学习子任务);
            项.标题 = 调度项.实体.学习子任务标题.empty() ? "空学习任务" : 调度项.实体.学习子任务标题;
            项.摘要 = 调度项.摘要;
            快照->学习等待列表.push_back(std::move(项));
        }

        if (快照->学习当前阶段.empty() && !快照->学习就绪列表.empty()) {
            快照->学习当前阶段 = "待调度";
        }
        if (快照->学习当前状态.empty() && !快照->学习就绪列表.empty()) {
            快照->学习当前状态 = "可调度";
        }

        std::ostringstream 输出;
        输出 << "学习账本=" << 快照->学习任务总数
            << " | 就绪=" << 快照->学习就绪队列数
            << " | 等待=" << 快照->学习等待表数
            << " | 执行中=" << 快照->学习任务执行中数
            << " | 已完成=" << 快照->学习任务已完成数
            << " | 线程阶段=" << 私有_安全文本(快照->学习当前阶段, "空")
            << " | 线程状态=" << 私有_安全文本(快照->学习当前状态, "空");
        if (!快照->学习当前任务标题.empty()) {
            输出 << " | 当前任务=" << 快照->学习当前任务标题;
        }
        if (!快照->学习当前方法标题.empty()) {
            输出 << " | 当前方法=" << 快照->学习当前方法标题;
        }
        if (!快照->学习最近摘要.empty()) {
            输出 << " | 最近摘要=" << 快照->学习最近摘要;
        }
        if (!快照->学习最近失败摘要.empty()) {
            输出 << " | 最近失败=" << 快照->学习最近失败摘要;
        }
        if (!快照->学习最近回流摘要.empty()) {
            输出 << " | 最近回流=" << 快照->学习最近回流摘要;
        }
        if (!快照->学习调度摘要.empty()) {
            输出 << " | 调度=" << 快照->学习调度摘要;
        }
        快照->学习专项摘要 = 输出.str();
    }

    void 私有_追加列表预览(
        std::ostringstream& 输出,
        std::string_view 标题,
        const std::vector<结构_控制面板列表项>& 列表,
        std::size_t 上限)
    {
        输出 << 标题 << ":\n";
        if (列表.empty()) {
            输出 << "  - 暂无数据\n";
            return;
        }

        const auto 数量 = (std::min)(上限, 列表.size());
        for (std::size_t i = 0; i < 数量; ++i) {
            输出 << "  - " << 列表[i].标题;
            if (!列表[i].摘要.empty()) {
                输出 << " | " << 列表[i].摘要;
            }
            输出 << '\n';
        }
        if (列表.size() > 数量) {
            输出 << "  - ... 省略 " << (列表.size() - 数量) << " 项\n";
        }
    }

    std::wstring 私有_页面标题(枚举_控制面板页面 页面, const 结构_控制面板快照& 快照)
    {
        switch (页面) {
        case 枚举_控制面板页面::概览:
            return L"概览与自我信息";
        case 枚举_控制面板页面::世界树:
            return L"世界树";
        case 枚举_控制面板页面::需求树:
            return 私有_UTF8转宽字串("自我需求树 (" + std::to_string(快照.需求数) + ")");
        case 枚举_控制面板页面::任务树:
            return 私有_UTF8转宽字串("自我任务树 (" + std::to_string(快照.任务数) + ")");
        case 枚举_控制面板页面::方法树:
            return 私有_UTF8转宽字串("自我方法树 (" + std::to_string(快照.方法数) + ")");
        case 枚举_控制面板页面::线程事件:
            return 私有_UTF8转宽字串("自我线程事件 (" + std::to_string(快照.自我运行阶段事件.size()) + ")");
        case 枚举_控制面板页面::先天动作:
            return 私有_UTF8转宽字串(
                "先天动作检查 (动态 "
                + std::to_string(快照.先天动作动态数)
                + " / 因果 "
                + std::to_string(快照.先天动作因果数)
                + ")");
        default:
            return L"控制面板";
        }
    }

    std::string 私有_常驻摘要文本(const 结构_控制面板快照& 快照)
    {
        std::ostringstream 输出;
        输出 << "自我: " << 快照.自我摘要 << '\n';
        输出 << "自我线程: " << 快照.自我线程摘要 << '\n';
        输出 << "运行时: " << 快照.运行时摘要 << '\n';
        输出 << "场景: 现实=" << 私有_安全文本(快照.自我现实场景名称, "无")
            << " | 内部=" << 私有_安全文本(快照.自我内部世界名称, "无") << '\n';
        输出 << "统计: 状态=" << 快照.状态数
            << " | 动态=" << 快照.动态数
            << " | 因果总数=" << (快照.因果实例数 + 快照.因果模板数)
            << " | 先天动作动态=" << 快照.先天动作动态数
            << " | 先天动作因果=" << 快照.先天动作因果数
            << " | 世界树节点=" << 快照.基础信息节点数
            << " | 需求=" << 快照.需求数
            << " | 任务=" << 快照.任务数
            << " | 方法=" << 快照.方法数;
        return 输出.str();
    }

    std::string 私有_概览内容(const 结构_控制面板快照& 快照)
    {
        std::ostringstream 输出;
        输出 << "自我信息\n";
        输出 << "  - 已初始化: " << (快照.自我已初始化 ? "是" : "否") << '\n';
        输出 << "  - 自我存在已建立: " << (快照.自我存在已建立 ? "是" : "否") << '\n';
        输出 << "  - 自我内部世界已建立: " << (快照.自我内部世界已建立 ? "是" : "否") << '\n';
        输出 << "  - 待机状态: " << (快照.自我待机状态 ? "是" : "否") << '\n';
        输出 << "  - 安全值: " << 快照.自我安全值 << '\n';
        输出 << "  - 服务值: " << 快照.自我服务值 << '\n';
        输出 << "  - 物理安全值: " << 快照.自我物理安全值 << '\n';
        输出 << "  - 风险安全值: " << 快照.自我风险安全值 << '\n';
        输出 << "  - 待学习方法数量: " << 快照.自我待学习方法数量 << '\n';
        输出 << "  - 安全根方向差值: " << 快照.自我安全根方向差值 << '\n';
        输出 << "  - 时序正向步长: " << 快照.自我时序正向步长 << '\n';
        输出 << "  - 时序反向步长: " << 快照.自我时序反向步长 << '\n';
        输出 << "  - 服务时序衰减步长: " << 快照.自我服务时序衰减步长 << '\n';
        输出 << "  - 现实场景: " << 私有_安全文本(快照.自我现实场景名称, "无") << '\n';
        输出 << "  - 内部世界: " << 私有_安全文本(快照.自我内部世界名称, "无") << '\n';
        输出 << '\n';

        输出 << "自我线程\n";
        输出 << "  - 生命周期: " << 私有_线程生命周期文本(快照.自我线程生命周期) << '\n';
        输出 << "  - 已初始化: " << (快照.自我线程已初始化 ? "是" : "否") << '\n';
        输出 << "  - 运行中: " << (快照.自我线程运行中 ? "是" : "否") << '\n';
        输出 << "  - 健康运行: " << (快照.自我线程健康运行 ? "是" : "否") << '\n';
        输出 << "  - 首轮运行已完成: " << (快照.自我线程首轮运行已完成 ? "是" : "否") << '\n';
        输出 << "  - Tick计数: " << 快照.自我Tick计数 << '\n';
        输出 << "  - 最近Tick时间: " << 快照.自我最近Tick时间 << '\n';
        输出 << "  - 当前阶段: " << 私有_安全文本(快照.自我线程当前阶段, "无") << '\n';
        输出 << "  - 当前最终去向: " << 私有_安全文本(快照.自我线程当前最终去向, "无") << '\n';
        输出 << "  - 本次启动来自故障恢复: " << (快照.自我线程本次启动来自故障恢复 ? "是" : "否") << '\n';
        输出 << "  - 累计故障次数: " << 快照.自我线程累计故障次数 << '\n';
        输出 << "  - 累计恢复次数: " << 快照.自我线程累计恢复次数 << '\n';
        if (快照.自我线程上次故障时间 != 0) {
            输出 << "  - 上次故障时间: " << 快照.自我线程上次故障时间 << '\n';
        }
        if (!快照.自我线程上次故障摘要.empty()) {
            输出 << "  - 上次故障摘要: " << 快照.自我线程上次故障摘要 << '\n';
        }
        if (!快照.自我线程最近恢复摘要.empty()) {
            输出 << "  - 最近恢复摘要: " << 快照.自我线程最近恢复摘要 << '\n';
        }
        输出 << "  - 线程摘要: " << 私有_安全文本(快照.自我线程摘要, "无") << '\n';
        if (!快照.自我线程最近故障摘要.empty()) {
            输出 << "  - 最近故障: " << 快照.自我线程最近故障摘要 << '\n';
        }
        输出 << '\n';

        输出 << "主要统计\n";
        输出 << "  - 场景数量: " << 快照.场景数 << '\n';
        输出 << "  - 存在数量: " << 快照.存在数 << '\n';
        输出 << "  - 特征数量: " << 快照.特征数 << '\n';
        输出 << "  - 状态数量: " << 快照.状态数 << '\n';
        输出 << "  - 动态数量: " << 快照.动态数 << '\n';
        输出 << "  - 因果实例数量: " << 快照.因果实例数 << '\n';
        输出 << "  - 因果模板数量: " << 快照.因果模板数 << '\n';
        输出 << "  - 因果总数量: " << (快照.因果实例数 + 快照.因果模板数) << '\n';
        输出 << "  - 先天动作动态数量: " << 快照.先天动作动态数 << '\n';
        输出 << "  - 先天动作因果数量: " << 快照.先天动作因果数 << '\n';
        输出 << "  - 世界根子节点数量: " << 快照.世界根子节点数 << '\n';
        输出 << "  - 世界树基础信息节点数量: " << 快照.基础信息节点数 << '\n';
        输出 << "  - 自我需求树节点数量: " << 快照.需求数 << '\n';
        输出 << "  - 自我任务树节点数量: " << 快照.任务数 << '\n';
        输出 << "  - 自我方法树节点数量: " << 快照.方法数 << '\n';
        输出 << '\n';

        私有_追加列表预览(输出, "场景预览", 快照.场景列表, 6);
        输出 << '\n';
        私有_追加列表预览(输出, "存在预览", 快照.存在列表, 6);
        输出 << '\n';
        私有_追加列表预览(输出, "需求预览", 快照.需求列表, 6);
        输出 << '\n';
        私有_追加列表预览(输出, "任务预览", 快照.任务列表, 6);
        输出 << '\n';
        私有_追加列表预览(输出, "方法预览", 快照.方法列表, 6);
        输出 << '\n';
        私有_追加列表预览(输出, "先天动作动态预览", 快照.先天动作动态列表, 6);
        输出 << '\n';
        私有_追加列表预览(输出, "先天动作因果预览", 快照.先天动作因果列表, 6);
        输出 << '\n';
        私有_追加阶段事件预览(输出, 快照.自我运行阶段事件, 6);

        return 输出.str();
    }

    std::string 私有_页面内容(const 结构_控制面板快照& 快照, 枚举_控制面板页面 页面)
    {
        switch (页面) {
        case 枚举_控制面板页面::概览:
            return 私有_概览内容(快照);
        case 枚举_控制面板页面::世界树:
            return 私有_渲染树文本(快照.世界树根);
        case 枚举_控制面板页面::需求树:
            return 私有_渲染树文本(快照.需求树根);
        case 枚举_控制面板页面::任务树:
            return 私有_渲染树文本(快照.任务树根);
        case 枚举_控制面板页面::方法树:
            return 私有_渲染树文本(快照.方法树根);
        case 枚举_控制面板页面::线程事件:
            return 私有_线程事件内容(快照);
        case 枚举_控制面板页面::先天动作:
            return 私有_先天动作内容(快照);
        default:
            return {};
        }
    }

    void 私有_更新导航按钮状态(结构_控制面板窗口上下文* 上下文) noexcept
    {
        if (!上下文) return;
        const BOOL 概览启用 = 上下文->当前页面 != 枚举_控制面板页面::概览;
        const BOOL 世界树启用 = 上下文->当前页面 != 枚举_控制面板页面::世界树;
        const BOOL 需求树启用 = 上下文->当前页面 != 枚举_控制面板页面::需求树;
        const BOOL 任务树启用 = 上下文->当前页面 != 枚举_控制面板页面::任务树;
        const BOOL 方法树启用 = 上下文->当前页面 != 枚举_控制面板页面::方法树;
        const BOOL 线程事件启用 = 上下文->当前页面 != 枚举_控制面板页面::线程事件;
        const BOOL 先天动作启用 = 上下文->当前页面 != 枚举_控制面板页面::先天动作;
        if (上下文->导航概览按钮) EnableWindow(上下文->导航概览按钮, 概览启用);
        if (上下文->导航世界树按钮) EnableWindow(上下文->导航世界树按钮, 世界树启用);
        if (上下文->导航需求树按钮) EnableWindow(上下文->导航需求树按钮, 需求树启用);
        if (上下文->导航任务树按钮) EnableWindow(上下文->导航任务树按钮, 任务树启用);
        if (上下文->导航方法树按钮) EnableWindow(上下文->导航方法树按钮, 方法树启用);
        if (上下文->导航线程事件按钮) EnableWindow(上下文->导航线程事件按钮, 线程事件启用);
        if (上下文->导航先天动作按钮) EnableWindow(上下文->导航先天动作按钮, 先天动作启用);
    }

    void 私有_布局控制面板窗口(HWND 窗口, int 宽度, int 高度) noexcept
    {
        auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
        if (!上下文) return;

        constexpr int 边距 = 12;
        constexpr int 左侧导航宽度 = 136;
        constexpr int 导航按钮宽度 = 112;
        constexpr int 导航按钮高度 = 34;
        constexpr int 导航间距 = 8;
        constexpr int 刷新按钮宽度 = 96;
        constexpr int 标题高度 = 30;
        constexpr int 摘要高度 = 108;
        constexpr int 区块间距 = 10;

        const int 左侧 = 边距;
        int y = 边距;
        const HWND 导航按钮列表[] = {
            上下文->导航概览按钮,
            上下文->导航世界树按钮,
            上下文->导航需求树按钮,
            上下文->导航任务树按钮,
            上下文->导航方法树按钮,
            上下文->导航线程事件按钮,
            上下文->导航先天动作按钮,
        };

        for (HWND 按钮 : 导航按钮列表) {
            if (!按钮) continue;
            MoveWindow(按钮, 左侧, y, 导航按钮宽度, 导航按钮高度, TRUE);
            y += 导航按钮高度 + 导航间距;
        }

        if (上下文->刷新按钮) {
            MoveWindow(
                上下文->刷新按钮,
                左侧,
                y + 6,
                刷新按钮宽度,
                导航按钮高度,
                TRUE);
        }

        const int 内容左侧 = 左侧导航宽度 + 边距 * 2;
        const int 内容宽度 = (std::max)(宽度 - 内容左侧 - 边距, 180);
        const int 标题顶部 = 边距;
        if (上下文->页面标题) {
            MoveWindow(
                上下文->页面标题,
                内容左侧,
                标题顶部,
                内容宽度,
                标题高度,
                TRUE);
        }

        const int 摘要顶部 = 标题顶部 + 标题高度 + 4;
        if (上下文->摘要文本框) {
            MoveWindow(
                上下文->摘要文本框,
                内容左侧,
                摘要顶部,
                内容宽度,
                摘要高度,
                TRUE);
        }

        const int 内容顶部 = 摘要顶部 + 摘要高度 + 区块间距;
        if (上下文->内容文本框) {
            MoveWindow(
                上下文->内容文本框,
                内容左侧,
                内容顶部,
                内容宽度,
                (std::max)(高度 - 内容顶部 - 边距, 120),
                TRUE);
        }
        if (上下文->树控件) {
            MoveWindow(
                上下文->树控件,
                内容左侧,
                内容顶部,
                内容宽度,
                (std::max)(高度 - 内容顶部 - 边距, 120),
                TRUE);
        }
    }

    void 私有_应用控制面板页面(HWND 窗口)
    {
        auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
        if (!上下文 || !上下文->已加载快照) return;

        SetWindowTextW(上下文->页面标题, 私有_页面标题(上下文->当前页面, 上下文->当前快照).c_str());
        私有_设置窗口文本(上下文->摘要文本框, 私有_常驻摘要文本(上下文->当前快照));
        const bool 使用树控件 = 私有_页面使用树控件(上下文->当前页面) && 上下文->树控件 != nullptr;
        if (上下文->内容文本框) {
            ShowWindow(上下文->内容文本框, 使用树控件 ? SW_HIDE : SW_SHOW);
        }
        if (上下文->树控件) {
            ShowWindow(上下文->树控件, 使用树控件 ? SW_SHOW : SW_HIDE);
        }
        if (使用树控件) {
            私有_刷新树控件(上下文->树控件, 私有_页面对应树(上下文->当前快照, 上下文->当前页面));
        }
        else {
            私有_设置窗口文本(上下文->内容文本框, 私有_页面内容(上下文->当前快照, 上下文->当前页面));
        }
        私有_更新导航按钮状态(上下文);

        std::wstring 标题 = L"鱼巢控制面板";
        标题 += L"  状态 ";
        标题 += std::to_wstring(上下文->当前快照.状态数);
        标题 += L"  动态 ";
        标题 += std::to_wstring(上下文->当前快照.动态数);
        标题 += L"  因果 ";
        标题 += std::to_wstring(上下文->当前快照.因果实例数 + 上下文->当前快照.因果模板数);
        SetWindowTextW(窗口, 标题.c_str());
    }

    void 私有_刷新控制面板窗口内容(HWND 窗口, bool 强制刷新快照)
    {
        auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
        if (!上下文) return;

        if (强制刷新快照 || !上下文->已加载快照) {
            if (!自我.已初始化()) {
                (void)初始化自我环境("控制面板窗口/刷新");
            }
            上下文->当前快照 = 读取控制面板快照(8, 20);
            上下文->已加载快照 = true;
        }

        私有_应用控制面板页面(窗口);
    }

    void 私有_切换控制面板页面(HWND 窗口, 枚举_控制面板页面 页面)
    {
        auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
        if (!上下文) return;
        上下文->当前页面 = 页面;
        私有_刷新控制面板窗口内容(窗口, false);
    }

    LRESULT CALLBACK 私有_控制面板窗口过程(HWND 窗口, UINT 消息, WPARAM wParam, LPARAM lParam)
    {
        switch (消息) {
        case WM_CREATE: {
            auto* 上下文 = new(std::nothrow) 结构_控制面板窗口上下文{};
            if (!上下文) {
                return -1;
            }

            SetWindowLongPtrW(窗口, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(上下文));
            const auto 实例 = GetModuleHandleW(nullptr);

            上下文->导航概览按钮 = CreateWindowExW(0, L"BUTTON", L"概览", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, 窗口, reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_导航概览按钮)), 实例, nullptr);
            上下文->导航世界树按钮 = CreateWindowExW(0, L"BUTTON", L"世界树", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, 窗口, reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_导航世界树按钮)), 实例, nullptr);
            上下文->导航需求树按钮 = CreateWindowExW(0, L"BUTTON", L"需求树", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, 窗口, reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_导航需求树按钮)), 实例, nullptr);
            上下文->导航任务树按钮 = CreateWindowExW(0, L"BUTTON", L"任务树", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, 窗口, reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_导航任务树按钮)), 实例, nullptr);
            上下文->导航方法树按钮 = CreateWindowExW(0, L"BUTTON", L"方法树", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, 窗口, reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_导航方法树按钮)), 实例, nullptr);
            上下文->导航线程事件按钮 = CreateWindowExW(0, L"BUTTON", L"线程事件", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, 窗口, reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_导航线程事件按钮)), 实例, nullptr);
            上下文->导航先天动作按钮 = CreateWindowExW(0, L"BUTTON", L"先天动作", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, 窗口, reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_导航先天动作按钮)), 实例, nullptr);
            上下文->刷新按钮 = CreateWindowExW(0, L"BUTTON", L"刷新", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, 窗口, reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_刷新按钮)), 实例, nullptr);

            上下文->页面标题 = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_LEFT,
                0, 0, 0, 0, 窗口, reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_页面标题)), 实例, nullptr);

            上下文->摘要文本框 = CreateWindowExW(
                WS_EX_CLIENTEDGE,
                L"EDIT",
                L"",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                0, 0, 0, 0,
                窗口,
                reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_摘要文本框)),
                实例,
                nullptr);

            上下文->内容文本框 = CreateWindowExW(
                WS_EX_CLIENTEDGE,
                L"EDIT",
                L"",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY,
                0, 0, 0, 0,
                窗口,
                reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_内容文本框)),
                实例,
                nullptr);

            上下文->树控件 = CreateWindowExW(
                WS_EX_CLIENTEDGE,
                WC_TREEVIEWW,
                L"",
                WS_CHILD | WS_VISIBLE | WS_TABSTOP | TVS_HASLINES | TVS_LINESATROOT |
                TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
                0, 0, 0, 0,
                窗口,
                reinterpret_cast<HMENU>(static_cast<INT_PTR>(私有_ID_树控件)),
                实例,
                nullptr);

            if (!上下文->导航概览按钮 || !上下文->导航世界树按钮 || !上下文->导航需求树按钮 ||
                !上下文->导航任务树按钮 || !上下文->导航方法树按钮 || !上下文->导航线程事件按钮 ||
                !上下文->导航先天动作按钮 ||
                !上下文->刷新按钮 ||
                !上下文->页面标题 || !上下文->摘要文本框 || !上下文->内容文本框) {
                return -1;
            }

            上下文->文本字体 = CreateFontW(
                -18, 0, 0, 0,
                FW_NORMAL,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY,
                FIXED_PITCH | FF_MODERN,
                L"Consolas");

            上下文->标题字体 = CreateFontW(
                -24, 0, 0, 0,
                FW_BOLD,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE,
                L"Microsoft YaHei UI");

            const HWND 全部控件[] = {
                上下文->导航概览按钮,
                上下文->导航世界树按钮,
                上下文->导航需求树按钮,
                上下文->导航任务树按钮,
                上下文->导航方法树按钮,
                上下文->导航线程事件按钮,
                上下文->导航先天动作按钮,
                上下文->刷新按钮,
                上下文->摘要文本框,
                上下文->内容文本框,
                上下文->树控件,
            };

            if (上下文->文本字体) {
                for (HWND 控件 : 全部控件) {
                    if (控件) {
                        SendMessageW(控件, WM_SETFONT, reinterpret_cast<WPARAM>(上下文->文本字体), TRUE);
                    }
                }
            }
            if (上下文->标题字体) {
                SendMessageW(上下文->页面标题, WM_SETFONT, reinterpret_cast<WPARAM>(上下文->标题字体), TRUE);
            }

            私有_刷新控制面板窗口内容(窗口, true);
            return 0;
        }
        case WM_SIZE:
            私有_布局控制面板窗口(窗口, LOWORD(lParam), HIWORD(lParam));
            return 0;
        case WM_COMMAND:
            if (HIWORD(wParam) == BN_CLICKED) {
                switch (LOWORD(wParam)) {
                case 私有_ID_刷新按钮:
                    私有_刷新控制面板窗口内容(窗口, true);
                    return 0;
                case 私有_ID_导航概览按钮:
                    私有_切换控制面板页面(窗口, 枚举_控制面板页面::概览);
                    return 0;
                case 私有_ID_导航世界树按钮:
                    私有_切换控制面板页面(窗口, 枚举_控制面板页面::世界树);
                    return 0;
                case 私有_ID_导航需求树按钮:
                    私有_切换控制面板页面(窗口, 枚举_控制面板页面::需求树);
                    return 0;
                case 私有_ID_导航任务树按钮:
                    私有_切换控制面板页面(窗口, 枚举_控制面板页面::任务树);
                    return 0;
                case 私有_ID_导航方法树按钮:
                    私有_切换控制面板页面(窗口, 枚举_控制面板页面::方法树);
                    return 0;
                case 私有_ID_导航线程事件按钮:
                    私有_切换控制面板页面(窗口, 枚举_控制面板页面::线程事件);
                    return 0;
                case 私有_ID_导航先天动作按钮:
                    私有_切换控制面板页面(窗口, 枚举_控制面板页面::先天动作);
                    return 0;
                default:
                    break;
                }
            }
            break;
        case 私有_WM_刷新控制面板窗口:
            私有_刷新控制面板窗口内容(窗口, true);
            私有_显示控制面板窗口(窗口);
            return 0;
        case WM_CLOSE:
            DestroyWindow(窗口);
            return 0;
        case WM_DESTROY: {
            auto* 上下文 = 私有_取控制面板窗口上下文(窗口);
            if (上下文) {
                if (上下文->文本字体) {
                    DeleteObject(上下文->文本字体);
                }
                if (上下文->标题字体) {
                    DeleteObject(上下文->标题字体);
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

    bool 私有_确保控制面板通用控件已初始化() noexcept
    {
        static std::once_flag 一次{};
        static bool 已初始化 = false;

        std::call_once(一次, []() {
            INITCOMMONCONTROLSEX 初始化结构{};
            初始化结构.dwSize = sizeof(初始化结构);
            初始化结构.dwICC = ICC_TREEVIEW_CLASSES | ICC_STANDARD_CLASSES;
            已初始化 = InitCommonControlsEx(&初始化结构) != FALSE;
        });

        return 已初始化;
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
        try {
            私有_控制面板启动诊断码.store(1);
            (void)私有_确保控制面板通用控件已初始化();
            if (!私有_确保控制面板窗口类已注册()) {
                私有_控制面板启动诊断码.store(2);
                启动结果.set_value(false);
                return;
            }

            auto* 现有窗口 = 私有_控制面板窗口句柄.load();
            if (现有窗口 && IsWindow(现有窗口)) {
                私有_控制面板启动诊断码.store(3);
                PostMessageW(现有窗口, 私有_WM_刷新控制面板窗口, 0, 0);
                启动结果.set_value(true);
                return;
            }

            私有_控制面板启动诊断码.store(4);
            HWND 窗口 = CreateWindowExW(
                WS_EX_APPWINDOW,
                私有_控制面板窗口类名,
                L"鱼巢控制面板",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT,
                980, 720,
                nullptr,
                nullptr,
                GetModuleHandleW(nullptr),
                nullptr);

            if (!窗口) {
                私有_控制面板启动诊断码.store(5);
                启动结果.set_value(false);
                return;
            }

            私有_控制面板启动诊断码.store(6);
            私有_控制面板窗口句柄.store(窗口);
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
            私有_控制面板启动诊断码.store(9);
            try {
                启动结果.set_value(false);
            }
            catch (...) {
            }
        }
    }
}

结构_控制面板快照 读取控制面板快照(
    std::size_t 树深度上限,
    std::size_t 树广度上限)
{
    结构_控制面板快照 快照{};
    const auto 自我线程快照 = 读取全局自我线程快照();
    const auto 全部动态 = 世界树.动态().枚举全部动态();
    const auto 全部因果实例 = 世界树.因果().枚举因果实例();

    快照.世界树已初始化 = 世界树.世界根() != nullptr;
    快照.自我已初始化 = 自我.已初始化();
    快照.自我线程已初始化 = 自我线程快照.初始化完成;
    快照.自我线程运行中 = 自我线程快照.生命周期 == 枚举_线程生命周期状态::运行中;
    快照.自我线程健康运行 = 自我线程快照.健康运行;
    快照.自我线程首轮运行已完成 = 自我线程快照.首轮运行已完成;
    快照.自我线程生命周期 = 自我线程快照.生命周期;
    快照.基础信息节点数 = 世界树.基础信息().枚举全部节点().size();
    快照.场景数 = 世界树.场景().枚举全部场景().size();
    快照.存在数 = 世界树.存在().枚举全部存在().size();
    快照.特征数 = 世界树.基础信息().枚举节点_按类型<特征节点主信息类>().size();
    快照.状态数 = 世界树.状态().枚举全部状态().size();
    快照.动态数 = 全部动态.size();
    快照.因果实例数 = 全部因果实例.size();
    快照.因果模板数 = 世界树.因果().枚举因果模板().size();
    快照.世界根子节点数 = 世界树.获取子节点(nullptr).size();

    for (auto* 动态节点 : 全部动态) {
        const auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
        const auto* 元信息 = 私有_取动态对应内部先天动作元信息(动态主信息);
        if (!动态主信息 || !元信息) {
            continue;
        }

        结构_控制面板列表项 项{};
        项.节点指针 = reinterpret_cast<std::uintptr_t>(动态节点);
        项.标题 = 私有_安全文本(
            元信息->动作名称 ? std::string(元信息->动作名称) : 私有_安全词(动态主信息->来源动作名),
            "未命名先天动作");
        项.摘要 = 私有_先天动作动态摘要(*动态主信息);
        快照.先天动作动态列表.push_back(std::move(项));
    }
    std::reverse(快照.先天动作动态列表.begin(), 快照.先天动作动态列表.end());
    快照.先天动作动态数 = 快照.先天动作动态列表.size();

    for (auto* 因果节点 : 全部因果实例) {
        const auto* 因果主信息 = 世界树.因果().取实例主信息(因果节点);
        const auto* 元信息 = 私有_取因果对应内部先天动作元信息(因果主信息);
        if (!因果主信息 || !元信息) {
            continue;
        }

        结构_控制面板列表项 项{};
        项.节点指针 = reinterpret_cast<std::uintptr_t>(因果节点);
        项.标题 = 私有_安全文本(
            元信息->动作名称 ? std::string(元信息->动作名称) : 因果主信息->动作语义键,
            "未命名先天动作因果");
        项.摘要 = 私有_先天动作因果摘要(因果节点, *因果主信息);
        快照.先天动作因果列表.push_back(std::move(项));
    }
    std::reverse(快照.先天动作因果列表.begin(), 快照.先天动作因果列表.end());
    快照.先天动作因果数 = 快照.先天动作因果列表.size();

    auto* 自我存在 = 自我.获取自我存在();
    auto* 自我现实场景 = 自我.获取自我现实场景();
    auto* 自我内部世界 = 自我.获取自我内部世界();
    auto* 当前主需求 = 自我.获取当前主需求();
    auto* 当前主任务 = 自我.获取当前主任务();
    auto* 当前主方法 = 自我.获取当前主方法();

    快照.自我存在已建立 = 自我存在 != nullptr;
    快照.自我内部世界已建立 = 自我内部世界 != nullptr;
    快照.自我待机状态 = 自我.是否待机状态();
    快照.自我存在指针 = reinterpret_cast<std::uintptr_t>(自我存在);
    快照.自我现实场景指针 = reinterpret_cast<std::uintptr_t>(自我现实场景);
    快照.自我内部世界指针 = reinterpret_cast<std::uintptr_t>(自我内部世界);
    快照.当前主需求指针 = reinterpret_cast<std::uintptr_t>(当前主需求);
    快照.当前主任务指针 = reinterpret_cast<std::uintptr_t>(当前主任务);
    快照.当前主方法指针 = reinterpret_cast<std::uintptr_t>(当前主方法);
    快照.任务管理当前步骤指针 = 自我线程快照.任务管理当前步骤;
    快照.任务管理最近结果指针 = 自我线程快照.任务管理最近结果;
    快照.自我Tick计数 = 自我线程快照.Tick计数;
    快照.自我最近Tick时间 = 自我线程快照.最近Tick时间;
    快照.自我线程当前最终去向 = 自我线程快照.当前最终去向;
    快照.自我线程当前阶段 = 自我线程快照.当前运行阶段;
    快照.自我线程最近故障摘要 = 自我线程快照.最近故障摘要;
    快照.自我运行阶段事件 = 自我线程快照.运行阶段事件;
    快照.任务管理根层重判结果 = 自我线程快照.任务管理根层重判结果;
    快照.任务管理执行前门控结果 = 自我线程快照.任务管理执行前门控结果;
    快照.任务管理上层反馈摘要 = 自我线程快照.任务管理上层反馈摘要;
    快照.任务管理当前缺口 = 自我线程快照.任务管理当前缺口;
    快照.任务管理当前去向 = 自我线程快照.任务管理当前去向;
    快照.任务管理总控结果 = 自我线程快照.任务管理总控结果;
    快照.任务管理反馈类型 = 自我线程快照.任务管理反馈类型;
    快照.任务管理反馈摘要 = 自我线程快照.任务管理反馈摘要;
    快照.任务管理方法来源 = 自我线程快照.任务管理方法来源;
    快照.任务管理步骤位 = 自我线程快照.任务管理步骤位;
    快照.任务管理步骤语义 = 自我线程快照.任务管理步骤语义;
    快照.任务管理当前步骤标题 = 自我线程快照.任务管理当前步骤标题;
    快照.任务管理最近结果标题 = 自我线程快照.任务管理最近结果标题;
    快照.任务管理目标结果摘要 = 自我线程快照.任务管理目标结果摘要;
    快照.主循环归并来源 = 自我线程快照.主循环归并来源;
    快照.主循环归并摘要 = 自我线程快照.主循环归并摘要;
    快照.主消息心跳车道状态 = 自我线程快照.主消息心跳车道状态;
    快照.任务治理车道状态 = 自我线程快照.任务治理车道状态;
    快照.学习车道状态 = 自我线程快照.学习车道状态;
    快照.延迟事实车道状态 = 自我线程快照.延迟事实车道状态;
    快照.根任务结构调整车道状态 = 自我线程快照.根任务结构调整车道状态;
    快照.运行时车道摘要 = 自我线程快照.运行时车道摘要;
    快照.自我线程本次启动来自故障恢复 = 自我线程快照.本次启动来自故障恢复;
    快照.自我线程累计故障次数 = 自我线程快照.累计故障次数;
    快照.自我线程累计恢复次数 = 自我线程快照.累计恢复次数;
    快照.自我线程上次故障时间 = 自我线程快照.上次故障时间;
    快照.自我线程上次故障摘要 = 自我线程快照.上次故障摘要;
    快照.自我线程最近恢复摘要 = 自我线程快照.最近恢复摘要;
    快照.学习调度摘要 = 自我线程快照.学习调度摘要;
    快照.学习当前阶段 = 自我线程快照.学习当前阶段;
    快照.学习当前状态 = 自我线程快照.学习当前状态;
    快照.学习当前任务标题 = 自我线程快照.学习当前任务标题;
    快照.学习当前方法标题 = 自我线程快照.学习当前方法标题;
    快照.学习最近摘要 = 自我线程快照.学习最近摘要;
    快照.学习最近失败摘要 = 自我线程快照.学习最近失败摘要;
    快照.学习最近回流摘要 = 自我线程快照.学习最近回流摘要;
    快照.学习就绪队列数 = 自我线程快照.学习任务就绪数;
    快照.学习等待表数 = 自我线程快照.学习任务等待数;
    快照.学习任务执行中数 = 自我线程快照.学习任务执行中数;

    {
        结构_学习账本快照 学习快照{};
        if (学习任务模块::读取学习账本快照(&学习快照)) {
            快照.学习任务总数 = 学习快照.总数;
            快照.学习任务活跃数 = 学习快照.活跃数;
            快照.学习任务挂起数 = 学习快照.挂起数;
            快照.学习任务可调度数 = 学习快照.可调度数;
            快照.学习任务执行中数 = 学习快照.执行中数;
            快照.学习任务已完成数 = 学习快照.已完成数;
            快照.学习账本摘要 = 学习快照.最近摘要;
        }
    }

    if (自我.已初始化()) {
        快照.自我安全值 = 自我.获取安全值();
        快照.自我服务值 = 自我.获取服务值();
        快照.自我物理安全值 = 自我.获取物理安全值();
        快照.自我风险安全值 = 自我.获取风险安全值();
        (void)自我.读取自我I64特征当前值(
            自我特征定义类::类型_自我_待学习方法数量(),
            快照.自我待学习方法数量,
            "控制面板::读取控制面板快照/待学习方法数量");
        快照.自我安全根方向差值 = 自我.安全根方向差值();
        快照.自我时序正向步长 = 自我.时序正向步长();
        快照.自我时序反向步长 = 自我.时序反向步长();
        快照.自我服务时序衰减步长 = 自我.服务时序衰减步长();
    }

    if (const auto* 场景主信息 = 世界树.场景().取场景主信息(自我现实场景)) {
        快照.自我现实场景名称 = 私有_安全文本(私有_安全词(场景主信息->名称), "未命名现实场景");
    }
    if (const auto* 场景主信息 = 世界树.场景().取场景主信息(自我内部世界)) {
        快照.自我内部世界名称 = 私有_安全文本(私有_安全词(场景主信息->名称), "未命名内部世界");
    }

    {
        std::ostringstream 输出;
        输出 << "初始化=" << (快照.自我已初始化 ? "是" : "否")
            << " | 存在=" << (快照.自我存在已建立 ? "已建立" : "未建立")
            << " | 待机=" << (快照.自我待机状态 ? "是" : "否")
            << " | 安全=" << 快照.自我安全值
            << " | 服务=" << 快照.自我服务值
            << " | 风险安全=" << 快照.自我风险安全值
            << " | 待学习方法=" << 快照.自我待学习方法数量;
        快照.自我摘要 = 输出.str();
    }

    {
        std::ostringstream 输出;
        输出 << "生命周期=" << 私有_线程生命周期文本(快照.自我线程生命周期)
            << " | 健康=" << (快照.自我线程健康运行 ? "是" : "否")
            << " | 首轮=" << (快照.自我线程首轮运行已完成 ? "是" : "否")
            << " | Tick=" << 快照.自我Tick计数
            << " | 当前阶段=" << 私有_安全文本(快照.自我线程当前阶段, "无")
            << " | 最终去向=" << 私有_安全文本(快照.自我线程当前最终去向, "无");
        if (快照.自我线程本次启动来自故障恢复) {
            输出 << " | 恢复启动=是";
        }
        if (快照.自我线程累计故障次数 > 0 || 快照.自我线程累计恢复次数 > 0) {
            输出 << " | 故障次数=" << 快照.自我线程累计故障次数
                << " | 恢复次数=" << 快照.自我线程累计恢复次数;
        }
        if (!快照.主循环归并摘要.empty()) {
            输出 << " | 归并=" << 快照.主循环归并摘要;
        }
        if (!快照.运行时车道摘要.empty()) {
            输出 << " | 车道=" << 快照.运行时车道摘要;
        }
        if (!快照.自我线程最近恢复摘要.empty()) {
            输出 << " | 恢复=" << 快照.自我线程最近恢复摘要;
        }
        if (!自我线程快照.最近运行摘要.empty()) {
            输出 << " | 摘要=" << 自我线程快照.最近运行摘要;
        }
        if (!快照.自我线程最近故障摘要.empty()) {
            输出 << " | 故障=" << 快照.自我线程最近故障摘要;
        }
        快照.自我线程摘要 = 输出.str();
    }

    {
        std::ostringstream 输出;
        输出 << "主需求="
            << (当前主需求 ? 私有_需求标题(当前主需求) : std::string("空"))
            << " | 主任务=" << (当前主任务 ? 私有_任务标题(当前主任务) : std::string("空"))
            << " | 主方法=" << (当前主方法 ? 私有_方法标题(当前主方法) : std::string("空"))
            << " | 根层重判=" << 私有_安全文本(快照.任务管理根层重判结果, "无")
            << " | 门控=" << 私有_安全文本(快照.任务管理执行前门控结果, "无")
            << " | 缺口=" << 私有_安全文本(快照.任务管理当前缺口, "无")
            << " | 去向=" << 私有_安全文本(快照.任务管理当前去向, "无")
            << " | 总控=" << 私有_安全文本(快照.任务管理总控结果, "无")
            << " | 反馈=" << 私有_安全文本(快照.任务管理反馈类型, "无")
            << " | 方法来源=" << 私有_安全文本(快照.任务管理方法来源, "无")
            << " | 步骤位=" << 私有_安全文本(快照.任务管理步骤位, "无")
            << " | 语义=" << 私有_安全文本(快照.任务管理步骤语义, "无")
            << " | 当前步骤=" << 私有_安全文本(快照.任务管理当前步骤标题, "空")
            << " | 最近结果=" << 私有_安全文本(快照.任务管理最近结果标题, "空")
            << " | 最终去向=" << 私有_安全文本(快照.自我线程当前最终去向, "无")
            << " | 生命周期=" << 私有_线程生命周期文本(快照.自我线程生命周期)
            << " | Tick=" << 快照.自我Tick计数
            << " | 正向步长=" << 快照.自我时序正向步长
            << " | 反向步长=" << 快照.自我时序反向步长
            << " | 服务衰减=" << 快照.自我服务时序衰减步长
            << " | 待学习方法=" << 快照.自我待学习方法数量
            << " | 学习账本=" << 快照.学习任务总数 << "/" << 快照.学习任务挂起数 << "/" << 快照.学习任务可调度数;
        if (!快照.学习当前阶段.empty()) {
            输出 << " | 学习阶段=" << 快照.学习当前阶段;
        }
        if (!快照.学习当前状态.empty()) {
            输出 << " | 学习状态=" << 快照.学习当前状态;
        }
        if (!快照.任务管理上层反馈摘要.empty()) {
            输出 << " | 上层反馈=" << 快照.任务管理上层反馈摘要;
        }
        if (!快照.任务管理反馈摘要.empty()) {
            输出 << " | 反馈摘要=" << 快照.任务管理反馈摘要;
        }
        if (!快照.任务管理目标结果摘要.empty()) {
            输出 << " | 双面=" << 快照.任务管理目标结果摘要;
        }
        if (!快照.主循环归并摘要.empty()) {
            输出 << " | 归并=" << 快照.主循环归并摘要;
        }
        if (!快照.运行时车道摘要.empty()) {
            输出 << " | 车道=" << 快照.运行时车道摘要;
        }
        快照.运行时摘要 = 输出.str();
    }

    私有_构建任务管理专项快照(&快照);
    私有_构建学习专项快照(&快照);

    for (auto* 世界节点 : 世界树.获取子节点(nullptr)) {
        结构_控制面板列表项 项{};
        项.节点指针 = reinterpret_cast<std::uintptr_t>(世界节点);
        项.标题 = 私有_基础节点标题(世界节点);
        项.摘要 = "子节点数 " + std::to_string(世界树.获取子节点(世界节点).size());
        快照.世界列表.push_back(std::move(项));
    }

    for (auto* 场景节点 : 世界树.场景().枚举全部场景()) {
        const auto* 场景主信息 = 世界树.场景().取场景主信息(场景节点);
        if (!场景主信息) continue;

        结构_控制面板列表项 项{};
        项.节点指针 = reinterpret_cast<std::uintptr_t>(场景节点);
        项.标题 = 私有_安全文本(私有_安全词(场景主信息->名称), "未命名场景");
        项.摘要 = 私有_世界类型文本(场景主信息->世界类型)
            + " | 状态索引 " + std::to_string(场景主信息->状态索引.size())
            + " | 动态索引 " + std::to_string(场景主信息->动态索引.size())
            + " | 因果索引 " + std::to_string(场景主信息->实例因果索引.size());
        快照.场景列表.push_back(std::move(项));
    }

    for (auto* 存在节点 : 世界树.存在().枚举全部存在()) {
        const auto* 存在主信息 = 世界树.存在().取存在主信息(存在节点);
        if (!存在主信息) continue;

        结构_控制面板列表项 项{};
        项.节点指针 = reinterpret_cast<std::uintptr_t>(存在节点);
        项.标题 = 私有_安全文本(私有_安全词(存在主信息->名称), "未命名存在");
        const auto 类型 = 私有_安全词(存在主信息->类型);
        项.摘要 = (类型.empty() ? std::string("无类型") : 类型)
            + " | 需求根=" + (存在主信息->需求根节点 ? std::string("是") : "否")
            + " | 任务根=" + (存在主信息->任务根节点 ? std::string("是") : "否")
            + " | 方法根=" + (存在主信息->方法根节点 ? std::string("是") : "否");
        快照.存在列表.push_back(std::move(项));
    }

    const auto* 需求根节点 = 自我存在 ? 世界树.存在().获取需求根节点(自我存在) : nullptr;
    const auto* 任务根节点 = 自我存在 ? 世界树.存在().获取任务根节点(自我存在) : nullptr;
    const auto* 方法根节点 = 自我存在 ? 世界树.存在().获取方法根节点(自我存在) : nullptr;

    快照.需求数 = 私有_统计独立树节点数(需求根节点);
    快照.任务数 = 私有_统计独立树节点数(任务根节点);
    快照.方法数 = 私有_统计独立树节点数(方法根节点);

    私有_采集独立树列表(
        需求根节点,
        &快照.需求列表,
        [](const auto* 节点) { return 私有_需求标题(节点); },
        [](const auto* 节点) { return 私有_需求摘要(节点); });
    私有_采集独立树列表(
        任务根节点,
        &快照.任务列表,
        [](const auto* 节点) { return 私有_任务标题(节点); },
        [](const auto* 节点) { return 私有_任务摘要(节点); });
    私有_采集独立树列表(
        方法根节点,
        &快照.方法列表,
        [](const auto* 节点) { return 私有_方法标题(节点); },
        [](const auto* 节点) { return 私有_方法摘要(节点); });

    快照.世界树根.文本 = "世界树";
    私有_构建世界树节点(
        世界树.世界根(),
        &快照.世界树根,
        0,
        树深度上限,
        树广度上限);

    快照.需求树根.文本 = "需求树 (" + std::to_string(快照.需求数) + ")";
    私有_构建独立树节点(
        需求根节点,
        &快照.需求树根,
        0,
        树深度上限,
        树广度上限,
        [](const auto* 节点) { return 私有_需求标题(节点) + " | " + 私有_需求摘要(节点); });

    快照.任务树根.文本 = "任务树 (" + std::to_string(快照.任务数) + ")";
    私有_构建独立树节点(
        任务根节点,
        &快照.任务树根,
        0,
        树深度上限,
        树广度上限,
        [](const auto* 节点) { return 私有_任务标题(节点) + " | " + 私有_任务摘要(节点); });

    快照.方法树根.文本 = "方法树 (" + std::to_string(快照.方法数) + ")";
    私有_构建独立树节点(
        方法根节点,
        &快照.方法树根,
        0,
        树深度上限,
        树广度上限,
        [](const auto* 节点) { return 私有_方法标题(节点) + " | " + 私有_方法摘要(节点); });

    私有_替换快照内指针文本(&快照);

    return 快照;
}

std::string 渲染控制面板摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 树层数上限,
    std::size_t 列表预览上限)
{
    std::ostringstream 输出;
    输出 << "控制面板摘要\n";
    输出 << "世界树: 初始化=" << (快照.世界树已初始化 ? "是" : "否")
        << " | 世界根子项=" << 快照.世界根子节点数
        << " | 基础信息=" << 快照.基础信息节点数
        << " | 场景=" << 快照.场景数
        << " | 存在=" << 快照.存在数
        << " | 特征=" << 快照.特征数
        << " | 状态=" << 快照.状态数
        << " | 动态=" << 快照.动态数
        << " | 因果实例=" << 快照.因果实例数
        << " | 因果模板=" << 快照.因果模板数 << '\n';
    输出 << "自我: " << 快照.自我摘要 << '\n';
    输出 << "自我线程: " << 快照.自我线程摘要 << '\n';
    输出 << "运行时: " << 快照.运行时摘要 << '\n';
    输出 << "任务管理: " << 私有_安全文本(快照.任务管理输出摘要, "暂无最近治理结果缓存") << '\n';
    输出 << "学习账本: 总=" << 快照.学习任务总数
        << " | 活跃=" << 快照.学习任务活跃数
        << " | 挂起=" << 快照.学习任务挂起数
        << " | 可调度=" << 快照.学习任务可调度数
        << " | 执行中=" << 快照.学习任务执行中数
        << " | 已完成=" << 快照.学习任务已完成数;
    if (!快照.学习账本摘要.empty()) {
        输出 << " | 最近=" << 快照.学习账本摘要;
    }
    输出 << '\n';
    输出 << "场景: 现实="
        << 私有_安全文本(快照.自我现实场景名称, "无")
        << " | 内部="
        << 私有_安全文本(快照.自我内部世界名称, "无") << '\n';
    输出 << "三棵树: 需求=" << 快照.需求数
        << " | 任务=" << 快照.任务数
        << " | 方法=" << 快照.方法数 << '\n';
    输出 << "先天动作: 动态=" << 快照.先天动作动态数
        << " | 因果=" << 快照.先天动作因果数 << '\n';

    const auto 渲染列表 = [&](std::string_view 标题, const std::vector<结构_控制面板列表项>& 列表) {
        输出 << 标题 << ":\n";
        if (列表.empty()) {
            输出 << "  - 暂无数据\n";
            return;
        }
        const auto 上限 = 列表.size();
        for (std::size_t 索引 = 0; 索引 < 上限; ++索引) {
            const auto& 项 = 列表[索引];
            输出 << "  - " << 项.标题;
            if (!项.摘要.empty()) {
                输出 << " | " << 项.摘要;
            }
            输出 << '\n';
        }
    };

    渲染列表("需求列表", 快照.需求列表);
    渲染列表("任务列表", 快照.任务列表);
    渲染列表("方法列表", 快照.方法列表);
    渲染列表("先天动作动态", 快照.先天动作动态列表);
    渲染列表("先天动作因果", 快照.先天动作因果列表);
    私有_追加阶段事件预览(输出, 快照.自我运行阶段事件, 列表预览上限);

    输出 << "世界树预览:\n";
    std::size_t 已输出世界树行数 = 0;
    私有_渲染树前几行(
        快照.世界树根,
        输出,
        树层数上限,
        列表预览上限,
        0,
        已输出世界树行数);

    输出 << "需求树预览:\n";
    std::size_t 已输出需求树行数 = 0;
    私有_渲染树前几行(
        快照.需求树根,
        输出,
        树层数上限,
        列表预览上限,
        0,
        已输出需求树行数);

    输出 << "任务树预览:\n";
    std::size_t 已输出任务树行数 = 0;
    私有_渲染树前几行(
        快照.任务树根,
        输出,
        树层数上限,
        列表预览上限,
        0,
        已输出任务树行数);

    输出 << "方法树预览:\n";
    std::size_t 已输出方法树行数 = 0;
    私有_渲染树前几行(
        快照.方法树根,
        输出,
        树层数上限,
        列表预览上限,
        0,
        已输出方法树行数);

    return 输出.str();
}

std::string 渲染任务管理摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限)
{
    (void)列表预览上限;
    std::ostringstream 输出;
    输出 << "任务管理专项摘要\n";
    输出 << "专项总览: " << 私有_安全文本(快照.任务管理专项摘要, "暂无任务管理专项数据") << '\n';
    输出 << "上下文镜像: " << 私有_安全文本(快照.任务管理上下文摘要, "暂无上下文镜像") << '\n';
    输出 << "本轮输出: " << 私有_安全文本(快照.任务管理输出摘要, "暂无最近治理结果缓存") << '\n';

    const auto 渲染列表 = [&](std::string_view 标题, const std::vector<结构_控制面板列表项>& 列表) {
        输出 << 标题 << ":\n";
        if (列表.empty()) {
            输出 << "  - 暂无数据\n";
            return;
        }
        const auto 上限 = (std::min)(列表预览上限, 列表.size());
        for (std::size_t 索引 = 0; 索引 < 上限; ++索引) {
            const auto& 项 = 列表[索引];
            输出 << "  - " << 项.标题;
            if (!项.摘要.empty()) {
                输出 << " | " << 项.摘要;
            }
            输出 << '\n';
        }
        if (列表.size() > 上限) {
            输出 << "  - ... 省略 " << (列表.size() - 上限) << " 项\n";
        }
    };

    渲染列表("输入面", 快照.任务管理输入列表);
    渲染列表("输出面", 快照.任务管理输出列表);
    渲染列表("触发环节", 快照.任务管理触发列表);
    return 输出.str();
}

std::string 渲染学习摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限)
{
    std::ostringstream 输出;
    输出 << "学习专项摘要\n";
    输出 << "专项总览: " << 私有_安全文本(快照.学习专项摘要, "暂无学习专项数据") << '\n';
    输出 << "调度摘要: " << 私有_安全文本(快照.学习调度摘要, "暂无学习调度数据") << '\n';
    输出 << "当前阶段: " << 私有_安全文本(快照.学习当前阶段, "空")
        << " | 当前状态: " << 私有_安全文本(快照.学习当前状态, "空")
        << " | 当前任务: " << 私有_安全文本(快照.学习当前任务标题, "空")
        << " | 当前方法: " << 私有_安全文本(快照.学习当前方法标题, "空") << '\n';
    if (!快照.学习最近摘要.empty()) {
        输出 << "最近摘要: " << 快照.学习最近摘要 << '\n';
    }
    if (!快照.学习最近失败摘要.empty()) {
        输出 << "最近失败: " << 快照.学习最近失败摘要 << '\n';
    }
    if (!快照.学习最近回流摘要.empty()) {
        输出 << "最近回流: " << 快照.学习最近回流摘要 << '\n';
    }

    const auto 渲染列表 = [&](std::string_view 标题, const std::vector<结构_控制面板列表项>& 列表) {
        输出 << 标题 << ":\n";
        if (列表.empty()) {
            输出 << "  - 暂无数据\n";
            return;
        }
        const auto 上限 = (std::min)(列表预览上限, 列表.size());
        for (std::size_t 索引 = 0; 索引 < 上限; ++索引) {
            const auto& 项 = 列表[索引];
            输出 << "  - " << 项.标题;
            if (!项.摘要.empty()) {
                输出 << " | " << 项.摘要;
            }
            输出 << '\n';
        }
        if (列表.size() > 上限) {
            输出 << "  - ... 省略 " << (列表.size() - 上限) << " 项\n";
        }
    };

    渲染列表("学习任务表", 快照.学习任务列表);
    渲染列表("就绪队列", 快照.学习就绪列表);
    渲染列表("等待表", 快照.学习等待列表);
    return 输出.str();
}

std::string 生成控制面板HTML(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限)
{
    std::ostringstream 输出;
    输出 << R"(<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>鱼巢控制面板</title>
  <style>
    :root{
      --bg:#f4efe6;
      --ink:#1f2a2c;
      --muted:#5b6b6e;
      --line:#d7c7ae;
      --card:#fffaf2;
      --accent:#b85c38;
      --accent-soft:#f0d7c2;
      --shadow:0 16px 32px rgba(76, 47, 32, .08);
    }
    *{box-sizing:border-box}
    ::selection{
      background:rgba(184,92,56,.24);
      color:var(--ink);
    }
    body{
      margin:0;
      font-family:"Microsoft YaHei UI","Source Han Sans SC","PingFang SC",sans-serif;
      color:var(--ink);
      background:
        radial-gradient(circle at top right, rgba(184,92,56,.18), transparent 24%),
        linear-gradient(180deg, #fff8ed 0%, var(--bg) 100%);
      -webkit-user-select:text;
      user-select:text;
    }
    h1,.panel h2,.panel h3,.lead,.label,.value,.sub,.summary,.toolbar-title,.toolbar-sub,
    .data-table th,.data-table td,.tree-leaf,details>summary,li,.footer{
      -webkit-user-select:text;
      user-select:text;
    }
    .shell{max-width:1680px;margin:0 auto;padding:24px 20px 42px}
    .layout{
      display:grid;
      grid-template-columns:240px minmax(0,1fr);
      gap:18px;
      align-items:start;
    }
    .rail{
      position:sticky;
      top:16px;
      background:rgba(255,250,242,.92);
      border:1px solid rgba(91,107,110,.12);
      border-radius:24px;
      box-shadow:var(--shadow);
      padding:18px 16px;
      backdrop-filter:blur(8px);
    }
    .brand{
      font-size:13px;
      letter-spacing:.08em;
      color:var(--muted);
      text-transform:uppercase;
    }
    .rail-title{
      margin:10px 0 0;
      font-size:24px;
      font-weight:700;
      line-height:1.3;
    }
    .rail-sub{
      margin:10px 0 0;
      color:var(--muted);
      font-size:13px;
      line-height:1.7;
    }
    .nav{
      margin-top:18px;
      display:grid;
      gap:8px;
    }
    .menu-item,.menu-refresh,.toolbar-btn{
      border:none;
      border-radius:14px;
      padding:11px 12px;
      font-size:14px;
      cursor:pointer;
      transition:transform .12s ease, background .12s ease, color .12s ease;
      -webkit-user-select:none;
      user-select:none;
    }
    .menu-item{
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:10px;
      width:100%;
      background:rgba(184,92,56,.08);
      color:var(--ink);
      text-align:left;
    }
    .menu-item:hover,.menu-refresh:hover,.toolbar-btn:hover{
      transform:translateY(-1px);
    }
    .menu-item:hover{
      background:rgba(184,92,56,.16);
    }
    .menu-item.active{
      background:var(--accent);
      color:#fffaf2;
      font-weight:700;
      box-shadow:0 10px 22px rgba(184,92,56,.18);
    }
    .menu-label{
      min-width:0;
      flex:1 1 auto;
      overflow:hidden;
      text-overflow:ellipsis;
      white-space:nowrap;
    }
    .menu-badge{
      flex:0 0 auto;
      border-radius:999px;
      padding:3px 9px;
      font-size:12px;
      line-height:1.4;
      background:rgba(31,42,44,.08);
      color:var(--muted);
      white-space:nowrap;
    }
    .menu-item.active .menu-badge{
      background:rgba(255,250,242,.2);
      color:#fffaf2;
    }
    .rail-actions{
      margin-top:16px;
      padding-top:16px;
      border-top:1px solid rgba(91,107,110,.12);
    }
    .menu-refresh{
      width:100%;
      background:linear-gradient(135deg, var(--accent), #cb7a58);
      color:#fffaf2;
      font-weight:700;
    }
    .content{min-width:0}
    .toolbar{
      position:sticky;
      top:16px;
      z-index:3;
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:16px;
      padding:18px 20px;
      background:rgba(255,250,242,.9);
      border:1px solid rgba(91,107,110,.12);
      border-radius:22px;
      box-shadow:var(--shadow);
      backdrop-filter:blur(10px);
    }
    .toolbar-kicker{
      font-size:12px;
      letter-spacing:.08em;
      color:var(--muted);
      text-transform:uppercase;
    }
    .toolbar-title{
      margin-top:6px;
      font-size:28px;
      font-weight:700;
      line-height:1.2;
    }
    .toolbar-sub{
      margin-top:8px;
      color:var(--muted);
      font-size:14px;
      line-height:1.6;
    }
    .toolbar-actions{
      display:flex;
      align-items:center;
      gap:10px;
      flex-shrink:0;
    }
    .toolbar-btn{
      min-width:112px;
      background:linear-gradient(135deg, var(--accent), #cb7a58);
      color:#fffaf2;
      font-weight:700;
    }
    .toolbar-btn.secondary{
      background:rgba(184,92,56,.08);
      border:1px solid rgba(184,92,56,.18);
      color:var(--ink);
      font-weight:600;
    }
    .page-stack{margin-top:18px}
    .page{
      display:none;
      animation:panel-in .18s ease;
    }
    .page.active{display:block}
    @keyframes panel-in{
      from{opacity:0;transform:translateY(6px)}
      to{opacity:1;transform:translateY(0)}
    }
    .hero{
      background:linear-gradient(135deg, rgba(184,92,56,.12), rgba(59,85,88,.08));
      border:1px solid rgba(184,92,56,.18);
      border-radius:28px;
      padding:28px;
      box-shadow:var(--shadow);
    }
    h1{margin:0;font-size:36px;letter-spacing:.04em}
    .lead{margin:12px 0 0;color:var(--muted);font-size:15px;line-height:1.7}
    .cards{
      margin-top:20px;
      display:grid;
      grid-template-columns:repeat(auto-fit,minmax(180px,1fr));
      gap:14px;
    }
    .card,.panel{
      background:var(--card);
      border:1px solid rgba(91,107,110,.12);
      border-radius:22px;
      box-shadow:var(--shadow);
    }
    .card{padding:18px}
    .label{font-size:12px;letter-spacing:.08em;color:var(--muted);text-transform:uppercase}
    .value{margin-top:8px;font-size:26px;font-weight:700}
    .sub{margin-top:8px;color:var(--muted);font-size:13px;line-height:1.6}
    .grid{
      display:grid;
      grid-template-columns:1.15fr 1fr;
      gap:18px;
      margin-top:18px;
    }
    .triple{
      display:grid;
      grid-template-columns:repeat(3,1fr);
      gap:18px;
      margin-top:18px;
    }
    .panel{padding:20px}
    .panel h2{margin:0 0 14px;font-size:22px}
    .panel h3{margin:0 0 12px;font-size:16px}
    .summary{
      white-space:pre-wrap;
      font-size:14px;
      line-height:1.8;
      color:var(--ink);
      background:rgba(184,92,56,.06);
      border:1px dashed rgba(184,92,56,.28);
      border-radius:16px;
      padding:14px 16px;
    }
    .muted{color:var(--muted)}
    .data-table{
      width:100%;
      border-collapse:collapse;
      font-size:14px;
    }
    .data-table th,.data-table td{
      text-align:left;
      vertical-align:top;
      border-top:1px solid rgba(91,107,110,.12);
      padding:10px 8px;
    }
    .data-table th{
      font-size:12px;
      text-transform:uppercase;
      letter-spacing:.06em;
      color:var(--muted);
      border-top:none;
      padding-top:0;
    }
    ul{margin:0;padding-left:18px}
    li{margin:4px 0}
    details>summary{cursor:pointer;color:var(--accent);font-weight:600}
    .tree-root,.tree-root ul{list-style:none;padding-left:16px}
    .tree-root{padding-left:0}
    .tree-leaf{padding:4px 0;line-height:1.5}
    .footer{
      margin-top:18px;
      color:var(--muted);
      font-size:13px;
      text-align:right;
    }
    @media (max-width: 1100px){
      .layout{grid-template-columns:1fr}
      .rail{position:static}
      .toolbar{position:static}
      .grid,.triple{grid-template-columns:1fr}
      h1{font-size:30px}
    }
    @media (max-width: 720px){
      .toolbar{
        flex-direction:column;
        align-items:flex-start;
      }
      .toolbar-actions{
        width:100%;
      }
      .toolbar-btn{
        width:100%;
      }
    }
  </style>
</head>
<body>
  <div class="shell">
    <div class="layout">
      <aside class="rail">
        <div class="brand">WebView2 控制面板</div>
        <div class="rail-title">鱼巢</div>
        <div class="rail-sub">左侧为菜单栏，中间为单页内容区；每次刷新都直接重新抓取当前工程快照。</div>
        <nav class="nav" aria-label="控制面板菜单">
)";

    const auto 追加菜单项 = [&](std::string_view 页面键, std::string_view 标签, std::string_view 徽标, bool 当前页 = false) {
        输出 << "<button class='menu-item";
        if (当前页) {
            输出 << " active";
        }
        输出 << "' type='button' data-page='" << 私有_转义HTML(页面键) << "'>"
            << "<span class='menu-label'>" << 私有_转义HTML(标签) << "</span>";
        if (!徽标.empty()) {
            输出 << "<span class='menu-badge'>" << 私有_转义HTML(徽标) << "</span>";
        }
        输出 << "</button>";
    };

    追加菜单项("overview", "概览", "状态 " + std::to_string(快照.状态数), true);
    追加菜单项("task-mgmt", "任务管理", std::to_string(快照.任务管理输出列表.size()));
    追加菜单项("learning", "学习任务",
        std::to_string(快照.学习任务总数) + "/" + std::to_string(快照.学习就绪队列数));
    追加菜单项("innate-actions", "先天动作",
        std::to_string(快照.先天动作动态数) + "/" + std::to_string(快照.先天动作因果数));
    追加菜单项("need-tree", "需求树", std::to_string(快照.需求数));
    追加菜单项("task-tree", "任务树", std::to_string(快照.任务数));
    追加菜单项("method-tree", "方法树", std::to_string(快照.方法数));
    追加菜单项("world-tree", "世界树", std::to_string(快照.基础信息节点数));
    追加菜单项("scene-entity", "场景与存在",
        std::to_string(快照.场景数) + "/" + std::to_string(快照.存在数));
    追加菜单项("thread-events", "线程事件", std::to_string(快照.自我运行阶段事件.size()));

    输出 << R"(
        </nav>
        <div class="rail-actions">
          <button class="menu-refresh js-refresh" type="button">刷新当前快照</button>
        </div>
      </aside>
      <main class="content">
        <div class="toolbar">
          <div class="toolbar-meta">
            <div class="toolbar-kicker">控制面板</div>
            <div class="toolbar-title" id="page-title">概览</div>
            <div class="toolbar-sub" id="page-subtitle">自我总览、运行时与主要统计。</div>
          </div>
          <div class="toolbar-actions">
            <button class="toolbar-btn secondary js-copy-page" type="button">复制当前页</button>
            <button class="toolbar-btn js-refresh" type="button">刷新</button>
          </div>
        </div>
        <div class="page-stack">
          <section class="page active" data-page="overview" data-title="概览" data-subtitle="自我总览、运行时与主要统计。">
            <section class="hero">
              <h1>鱼巢控制面板</h1>
              <div class="lead">当前工程参照旧版控制面板迁移了最小显示链，优先展示这套代码真实存在的世界树、自我核心值和三棵独立信息树。</div>
              <div class="cards">
)";

    const auto 追加卡片 = [&](std::string_view 标签, std::string_view 值, std::string_view 补充 = {}) {
        输出 << "<div class='card'><div class='label'>" << 私有_转义HTML(标签)
            << "</div><div class='value'>" << 私有_转义HTML(值)
            << "</div>";
        if (!补充.empty()) {
            输出 << "<div class='sub'>" << 私有_转义HTML(补充) << "</div>";
        }
        输出 << "</div>";
    };

    追加卡片("自我状态", 快照.自我待机状态 ? "待机" : "活动", 快照.自我摘要);
    追加卡片("自我线程", 私有_线程生命周期文本(快照.自我线程生命周期), 快照.自我线程摘要);
    追加卡片("线程恢复", 快照.自我线程本次启动来自故障恢复 ? "恢复启动" : "普通启动",
        "故障 " + std::to_string(快照.自我线程累计故障次数) + " / 恢复 " + std::to_string(快照.自我线程累计恢复次数));
    追加卡片("安全值", std::to_string(快照.自我安全值), "物理安全 " + std::to_string(快照.自我物理安全值));
    追加卡片("服务值", std::to_string(快照.自我服务值), "风险安全 " + std::to_string(快照.自我风险安全值));
    追加卡片("安全根方向差值", std::to_string(快照.自我安全根方向差值), "越大说明越偏离根安全目标");
    追加卡片("先天动作", std::to_string(快照.先天动作动态数) + " 动态 / " + std::to_string(快照.先天动作因果数) + " 因果",
        "线程每周期被动触发的本能动作事实与因果");
    追加卡片("世界树节点", std::to_string(快照.基础信息节点数), "场景 " + std::to_string(快照.场景数) + " / 存在 " + std::to_string(快照.存在数));
    追加卡片("三棵树", "需求 " + std::to_string(快照.需求数) + " / 任务 " + std::to_string(快照.任务数) + " / 方法 " + std::to_string(快照.方法数),
        "当前主需求指针 " + std::to_string(快照.当前主需求指针));

    输出 << R"(
              </div>
            </section>

            <div class="grid">
              <section class="panel">
                <h2>自我与运行时</h2>
        <div class="summary">)"
        << 私有_转义HTML(快照.自我线程摘要 + "\n" + 快照.运行时摘要) << R"(</div>
        <div class="cards" style="margin-top:16px">
)";
    追加卡片("现实场景", 私有_安全文本(快照.自我现实场景名称, "无"), "指针 " + std::to_string(快照.自我现实场景指针));
    追加卡片("内部世界", 私有_安全文本(快照.自我内部世界名称, "无"), "指针 " + std::to_string(快照.自我内部世界指针));
    追加卡片("待学习方法", std::to_string(快照.自我待学习方法数量), "需求 " + std::to_string(快照.需求数));
    追加卡片("学习账本", std::to_string(快照.学习任务总数) + " 总 / " + std::to_string(快照.学习任务可调度数) + " 可调度",
        "挂起 " + std::to_string(快照.学习任务挂起数));
    追加卡片("正向步长", std::to_string(快照.自我时序正向步长), "反向步长 " + std::to_string(快照.自我时序反向步长));
    追加卡片("服务衰减", std::to_string(快照.自我服务时序衰减步长), "存在指针 " + std::to_string(快照.自我存在指针));
    追加卡片("线程Tick", std::to_string(快照.自我Tick计数), "最近Tick " + std::to_string(快照.自我最近Tick时间));
    输出 << R"(
                </div>
              </section>

              <section class="panel">
                <h2>仓库统计</h2>
        <div class="cards" style="margin-top:0">
)";
    追加卡片("世界根子项", std::to_string(快照.世界根子节点数));
    追加卡片("特征", std::to_string(快照.特征数));
    追加卡片("状态", std::to_string(快照.状态数));
    追加卡片("动态", std::to_string(快照.动态数));
    追加卡片("因果实例", std::to_string(快照.因果实例数));
    追加卡片("因果模板", std::to_string(快照.因果模板数));
    追加卡片("先天动作动态", std::to_string(快照.先天动作动态数));
    追加卡片("先天动作因果", std::to_string(快照.先天动作因果数));
    输出 << R"(
                </div>
              </section>
            </div>
          </section>

          <section class="page" data-page="task-mgmt" data-title="任务管理" data-subtitle="查看任务管理一步治理的上下文镜像、结果面和触发环节。">
            <section class="panel">
              <h2>任务管理专项摘要</h2>
      <div class="summary">)"
        << 私有_转义HTML(快照.任务管理专项摘要.empty() ? std::string("暂无任务管理专项数据") : 快照.任务管理专项摘要)
        << R"(</div>
      <div class="triple" style="margin-top:16px">
        <div>
          <h3>输入面</h3>
)"
        << 私有_列表HTML(快照.任务管理输入列表, 列表预览上限)
        << R"(        </div>
        <div>
          <h3>输出面</h3>
)"
        << 私有_列表HTML(快照.任务管理输出列表, 列表预览上限)
        << R"(        </div>
        <div>
          <h3>触发环节</h3>
)"
        << 私有_列表HTML(快照.任务管理触发列表, 列表预览上限)
        << R"(        </div>
      </div>
            </section>
          </section>

          <section class="page" data-page="learning" data-title="学习任务" data-subtitle="查看学习账本、就绪队列、等待表与最近学习阶段。">
            <section class="panel">
              <h2>学习专项摘要</h2>
      <div class="summary">)"
        << 私有_转义HTML(快照.学习专项摘要.empty() ? std::string("暂无学习专项数据") : 快照.学习专项摘要)
        << R"(</div>
      <div class="triple" style="margin-top:16px">
        <div>
          <h3>学习任务表</h3>
)"
        << 私有_列表HTML(快照.学习任务列表, 列表预览上限)
        << R"(        </div>
        <div>
          <h3>就绪队列</h3>
)"
        << 私有_列表HTML(快照.学习就绪列表, 列表预览上限)
        << R"(        </div>
        <div>
          <h3>等待表</h3>
)"
        << 私有_列表HTML(快照.学习等待列表, 列表预览上限)
        << R"(        </div>
      </div>
            </section>
          </section>

          <section class="page" data-page="innate-actions" data-title="先天动作" data-subtitle="检查自我线程每周期自动触发的先天动作动态与因果。">
            <section class="panel">
              <h2>先天动作检查</h2>
      <div class="summary">)"
        << 私有_转义HTML(
            "先天动作动态 " + std::to_string(快照.先天动作动态数)
            + " | 先天动作因果 " + std::to_string(快照.先天动作因果数)
            + " | 总动态 " + std::to_string(快照.动态数)
            + " | 总因果实例 " + std::to_string(快照.因果实例数))
        << R"(</div>
      <div class="grid" style="margin-top:16px">
        <div>
          <h3>先天动作动态</h3>
)"
        << 私有_列表HTML(快照.先天动作动态列表, 列表预览上限)
        << R"(        </div>
        <div>
          <h3>先天动作因果</h3>
)"
        << 私有_列表HTML(快照.先天动作因果列表, 列表预览上限)
        << R"(        </div>
      </div>
            </section>
          </section>

          <section class="page" data-page="need-tree" data-title="需求树" data-subtitle="查看自我的需求树与节点摘要。">
            <section class="panel">
              <h2>需求树</h2>
)";
    输出 << 私有_树HTML(快照.需求树根);
    输出 << "<div style='margin-top:16px'>" << 私有_列表HTML(快照.需求列表, 列表预览上限) << "</div>";
    输出 << R"(
            </section>
          </section>

          <section class="page" data-page="task-tree" data-title="任务树" data-subtitle="查看自我的任务树与治理链。">
            <section class="panel">
              <h2>任务树</h2>
)";
    输出 << 私有_树HTML(快照.任务树根);
    输出 << "<div style='margin-top:16px'>" << 私有_列表HTML(快照.任务列表, 列表预览上限) << "</div>";
    输出 << R"(
            </section>
          </section>

          <section class="page" data-page="method-tree" data-title="方法树" data-subtitle="查看本能方法与当前方法树结构。">
            <section class="panel">
              <h2>方法树</h2>
)";
    输出 << 私有_树HTML(快照.方法树根);
    输出 << "<div style='margin-top:16px'>" << 私有_列表HTML(快照.方法列表, 列表预览上限) << "</div>";
    输出 << R"(
            </section>
          </section>

          <section class="page" data-page="world-tree" data-title="世界树" data-subtitle="查看当前世界树的层级结构。">
            <section class="panel">
              <h2>世界树</h2>
)";
    输出 << 私有_树HTML(快照.世界树根);
    输出 << R"(
            </section>
          </section>

          <section class="page" data-page="scene-entity" data-title="场景与存在" data-subtitle="查看世界、场景和存在列表摘要。">
            <section class="panel">
              <h2>场景与存在</h2>
        <div class="summary">)"
        << 私有_转义HTML("世界列表 " + std::to_string(快照.世界列表.size())
            + " | 场景列表 " + std::to_string(快照.场景列表.size())
            + " | 存在列表 " + std::to_string(快照.存在列表.size()))
        << R"(</div>
        <div style="margin-top:16px"><h3>场景列表</h3>)"
        << 私有_列表HTML(快照.场景列表, 列表预览上限)
        << R"(</div>
        <div style="margin-top:16px"><h3>存在列表</h3>)"
        << 私有_列表HTML(快照.存在列表, 列表预览上限)
        << R"(</div>
            </section>
          </section>

          <section class="page" data-page="thread-events" data-title="线程事件" data-subtitle="查看自我线程阶段事件与最近摘要。">
            <section class="panel">
              <h2>自我线程阶段事件</h2>
      <div class="summary">)";
    if (快照.自我运行阶段事件.empty()) {
        输出 << "暂无事件";
    }
    else {
        const std::size_t 起点 = 快照.自我运行阶段事件.size() > 列表预览上限
            ? (快照.自我运行阶段事件.size() - 列表预览上限)
            : 0;
        for (std::size_t 索引 = 起点; 索引 < 快照.自我运行阶段事件.size(); ++索引) {
            const auto& 事件 = 快照.自我运行阶段事件[索引];
            输出 << 私有_转义HTML(
                "[" + std::to_string(事件.时间) + "] "
                + 私有_安全文本(事件.阶段键, "未命名阶段")
                + (事件.摘要.empty() ? std::string{} : std::string(" | ") + 事件.摘要));
            if (索引 + 1 < 快照.自我运行阶段事件.size()) {
                输出 << "\n";
            }
        }
    }
    输出 << R"(</div>
            </section>
          </section>
        </div>

        <div class="footer">当前页面由现工程快照直接生成；左侧菜单切换内容页，刷新按钮会重新抓取最新运行快照。</div>
      </main>
    </div>
  </div>
  <script>
    (() => {
      const 菜单项列表 = Array.from(document.querySelectorAll('.menu-item'));
      const 页面列表 = Array.from(document.querySelectorAll('.page'));
      const 标题节点 = document.getElementById('page-title');
      const 副标题节点 = document.getElementById('page-subtitle');
      const 页面别名 = {
        'runtime': 'overview',
        'stats': 'overview'
      };

      const 刷新页面 = () => {
        if (window.chrome && window.chrome.webview) {
          window.chrome.webview.postMessage('refresh');
        } else {
          location.reload();
        }
      };

      const 复制按钮列表 = Array.from(document.querySelectorAll('.js-copy-page'));
      let 复制反馈计时器 = 0;

      const 设置复制按钮文本 = (文本) => {
        复制按钮列表.forEach((按钮) => {
          按钮.textContent = 文本;
        });
      };

      const 读取当前页文本 = () => {
        const 当前页面 = 页面列表.find((页面) => 页面.classList.contains('active')) || 页面列表[0];
        if (!当前页面) return '';

        const 标题 = 当前页面.dataset.title || '';
        const 副标题 = 当前页面.dataset.subtitle || '';
        const 正文 = (当前页面.innerText || '').trim();
        return [标题, 副标题, 正文].filter(Boolean).join('\n\n').trim();
      };

      const 回退复制文本 = (文本) => {
        const 文本框 = document.createElement('textarea');
        文本框.value = 文本;
        文本框.setAttribute('readonly', 'readonly');
        文本框.style.position = 'fixed';
        文本框.style.opacity = '0';
        文本框.style.pointerEvents = 'none';
        document.body.appendChild(文本框);
        文本框.focus();
        文本框.select();
        let 成功 = false;
        try {
          成功 = document.execCommand('copy');
        } catch (_) {
          成功 = false;
        }
        文本框.remove();
        return 成功;
      };

      const 复制当前页 = async () => {
        const 文本 = 读取当前页文本();
        if (!文本) {
          设置复制按钮文本('无可复制内容');
          clearTimeout(复制反馈计时器);
          复制反馈计时器 = window.setTimeout(() => 设置复制按钮文本('复制当前页'), 1200);
          return;
        }

        let 成功 = false;
        if (navigator.clipboard && navigator.clipboard.writeText) {
          try {
            await navigator.clipboard.writeText(文本);
            成功 = true;
          } catch (_) {
            成功 = false;
          }
        }
        if (!成功) {
          成功 = 回退复制文本(文本);
        }

        设置复制按钮文本(成功 ? '已复制当前页' : '复制失败');
        clearTimeout(复制反馈计时器);
        复制反馈计时器 = window.setTimeout(() => 设置复制按钮文本('复制当前页'), 1400);
      };

      const 归一化页面键 = (键) => {
        if (!键) return 'overview';
        return 页面别名[键] || 键;
      };

      const 切换页面 = (键, 是否同步地址栏) => {
        const 目标键 = 归一化页面键(键);
        const 目标页面 = 页面列表.find((页面) => 页面.dataset.page === 目标键) || 页面列表[0];
        if (!目标页面) return;

        页面列表.forEach((页面) => {
          页面.classList.toggle('active', 页面 === 目标页面);
        });
        菜单项列表.forEach((菜单项) => {
          菜单项.classList.toggle('active', 菜单项.dataset.page === 目标页面.dataset.page);
        });

        if (标题节点) {
          标题节点.textContent = 目标页面.dataset.title || '控制面板';
        }
        if (副标题节点) {
          副标题节点.textContent = 目标页面.dataset.subtitle || '';
        }

        if (是否同步地址栏) {
          const 新哈希 = '#' + 目标页面.dataset.page;
          if (location.hash !== 新哈希) {
            history.replaceState(null, '', 新哈希);
          }
        }

        window.scrollTo(0, 0);
      };

      菜单项列表.forEach((菜单项) => {
        菜单项.addEventListener('click', () => 切换页面(菜单项.dataset.page, true));
      });
      document.querySelectorAll('.js-refresh').forEach((按钮) => {
        按钮.addEventListener('click', 刷新页面);
      });
      复制按钮列表.forEach((按钮) => {
        按钮.addEventListener('click', () => {
          void 复制当前页();
        });
      });
      window.addEventListener('hashchange', () => {
        切换页面(location.hash.slice(1), false);
      });

      切换页面(location.hash.slice(1), false);
    })();
  </script>
</body>
</html>)";

    return 输出.str();
}

bool 保存控制面板HTML(
    const std::filesystem::path& 输出路径,
    const 结构_控制面板快照& 快照)
{
    std::ofstream 输出文件(输出路径, std::ios::binary);
    if (!输出文件) return false;

    const auto HTML = 生成控制面板HTML(快照);
    输出文件.write(HTML.data(), static_cast<std::streamsize>(HTML.size()));
    return 输出文件.good();
}

bool 启动控制面板窗口() noexcept
{
    return 启动控制面板WebView2窗口();
}

void 等待控制面板窗口关闭() noexcept
{
    等待控制面板WebView2窗口关闭();
}

int 获取控制面板启动诊断码() noexcept
{
    return 获取控制面板WebView2启动诊断码();
}

std::filesystem::path 默认控制面板HTML路径()
{
    return std::filesystem::absolute(std::filesystem::path(L"控制面板.html"));
}

枚举_控制面板命令 解析控制面板命令行(int argc, char** argv) noexcept
{
    枚举_控制面板命令 输出 = 枚举_控制面板命令::无;
    for (int i = 1; i < argc; ++i) {
        const std::string_view 参数 = argv[i] ? std::string_view(argv[i]) : std::string_view{};
        if (参数 == "--panel-text" || 参数 == "--summary") {
            输出 = 枚举_控制面板命令::输出摘要;
        }
        else if (参数 == "--task-mgmt-summary" || 参数 == "--task-summary" || 参数 == "--task-management-summary") {
            输出 = 枚举_控制面板命令::输出任务管理摘要;
        }
        else if (参数 == "--learning-summary" || 参数 == "--study-summary") {
            输出 = 枚举_控制面板命令::输出学习摘要;
        }
        else if (参数 == "--panel-html") {
            输出 = 枚举_控制面板命令::生成HTML;
        }
        else if (参数 == "--panel" || 参数 == "--panel-open") {
            输出 = 枚举_控制面板命令::打开窗口;
        }
    }
    return 输出;
}
