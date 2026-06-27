module;

// 文件头部规则注释模块：
// 1. 修改本文件前先阅读本模块；控制面板时序、禁忌和数据加载边界先写在这里。
// 2. 每次启动前都需要先清数据库，所以控制面板数据需要后加载，等自我运行获取数据后再加载。
// 3. WebView2 控制面板启动页不得同步强读 SQL 投影；启动页先显示加载提示，后续按区段刷新。
// 4. 本模块只供人读，不参与机器判断；业务状态仍必须由项目结构或 SQL 投影承载。

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "世界树类.h"
#include "动态类.h"
#include "语素类.h"
#include "语言类.h"
#include "需求类.h"
#include "任务类.h"
#include "方法类.h"
#include "因果类.h"
#include "场景索引同步.h"

module 控制面板类;

import 全局共享函数类;
import 控制面板WebView2;
import 自我类;
import 自我类.特征定义;
import 自我线程模块;
import 控制面板摘要线程模块;
import 线程生命周期消息模块;
import 任务模块.管理工作线程;
import 任务模块.管理界面线程;
import 任务模块.治理协议;
import 数据库ADO模块;
import 日志模块;

namespace {
    using 需求节点 = 需求类::节点类;
    using 任务节点 = 任务类::节点类;
    using 方法节点 = 方法类::节点类;
    using 路径集合 = std::unordered_set<std::uintptr_t>;

    std::string 私有_需求显示名_控制面板(const 需求节点* 节点);

    template<class T节点>
    std::vector<T节点*> 私有_枚举子节点(const T节点* 父节点, const std::size_t 上限);

    constexpr std::uintptr_t 私有_线程详情_自我 = 1;
    constexpr std::uintptr_t 私有_线程详情_工作 = 2;
    constexpr std::uintptr_t 私有_线程详情_摘要 = 4;
    constexpr std::uintptr_t 私有_线程详情_任务界面 = 5;
    constexpr std::size_t 私有_列表分页大小 = 100;
    constexpr const char* 私有_SQL控制面板HTML头部规则备注 =
        "<!-- 规则备注：每次启动前都需要先清数据库，所以控制面板数据需要后加载，等自我运行获取数据后再加载。不要在 WebView2 控制面板启动页同步强读 SQL 投影。 -->\n";

    enum class 枚举_控制面板HTML用途 {
        自我场景窗口,
    };

    using 线程池参数保存状态 =
        任务管理线程协议::枚举_任务管理工作线程池参数保存状态;
    using 线程池参数错误状态 =
        任务管理线程协议::枚举_任务管理工作线程池参数错误状态;
    using 线程池参数应用状态 =
        任务管理线程协议::枚举_任务管理工作线程池参数应用状态;

    // 功能：把任务工作线程池参数保存状态转换为控制面板显示文本。
    std::string 私有_任务工作线程池参数保存状态文本(
        const 线程池参数保存状态 状态,
        const std::uint64_t worker数量)
    {
        switch (状态) {
        case 线程池参数保存状态::保存成功:
            return "保存成功 | worker pool 大小=" + std::to_string(worker数量);
        case 线程池参数保存状态::保存失败_非法大小:
            return "保存失败";
        case 线程池参数保存状态::启动拒绝_非法大小:
            return "拒绝非法 worker pool 大小";
        case 线程池参数保存状态::未保存:
        default:
            return "尚未保存";
        }
    }

    // 功能：把任务工作线程池参数错误状态转换为控制面板显示文本。
    std::string 私有_任务工作线程池参数错误状态文本(
        const 线程池参数错误状态 状态,
        const std::uint64_t worker数量,
        const std::uint64_t 最小值,
        const std::uint64_t 最大值)
    {
        switch (状态) {
        case 线程池参数错误状态::大小为零:
            return "任务管理工作线程池大小不能为 0";
        case 线程池参数错误状态::大小越界:
            return "任务管理工作线程池大小越界 | worker数="
                + std::to_string(worker数量)
                + " | 有效范围="
                + std::to_string(最小值)
                + "-"
                + std::to_string(最大值);
        case 线程池参数错误状态::重建失败:
            return "保存成功但 worker pool 重建失败";
        case 线程池参数错误状态::无:
        default:
            return "";
        }
    }

    // 功能：把任务工作线程池参数应用状态转换为控制面板显示文本。
    std::string 私有_任务工作线程池参数应用状态文本(
        const 线程池参数应用状态 状态,
        const std::uint64_t worker数量)
    {
        switch (状态) {
        case 线程池参数应用状态::未启动:
            return "未启动";
        case 线程池参数应用状态::已按当前大小启动:
            return "已按 worker pool 大小 "
                + std::to_string(worker数量)
                + " 启动";
        case 线程池参数应用状态::等待重建:
            return "等待重建 worker pool";
        case 线程池参数应用状态::已保存等待下次启动:
            return "已保存，worker pool 未运行，等待下次启动应用";
        case 线程池参数应用状态::保存后已安全重建:
            return "保存后已安全重建 worker pool";
        case 线程池参数应用状态::保存成功但重建失败:
            return "保存成功但 worker pool 重建失败";
        case 线程池参数应用状态::未应用:
        default:
            return "尚未应用";
        }
    }

    std::string 私有_任务工作线程池最近参数保存文本(
        const 任务管理线程协议::结构_任务界面线程快照& 快照)
    {
        return 私有_任务工作线程池参数保存状态文本(
            快照.最近参数保存状态,
            快照.任务管理工作线程池当前有效大小);
    }

    std::string 私有_任务工作线程池最近参数错误文本(
        const 任务管理线程协议::结构_任务界面线程快照& 快照)
    {
        return 私有_任务工作线程池参数错误状态文本(
            快照.最近参数错误状态,
            快照.任务管理工作线程池待保存大小,
            快照.任务管理工作线程池最小大小,
            快照.任务管理工作线程池最大大小);
    }

    std::string 私有_任务工作线程池最近参数应用文本(
        const 任务管理线程协议::结构_任务界面线程快照& 快照)
    {
        return 私有_任务工作线程池参数应用状态文本(
            快照.最近参数应用状态,
            快照.任务管理工作线程池当前有效大小);
    }

    std::string 私有_任务工作线程池最近参数保存文本(
        const 结构_控制面板快照& 快照)
    {
        return 私有_任务工作线程池参数保存状态文本(
            static_cast<线程池参数保存状态>(
                快照.任务管理工作线程池最近参数保存状态值),
            快照.任务管理工作线程池当前有效大小);
    }

    std::string 私有_任务工作线程池最近参数错误文本(
        const 结构_控制面板快照& 快照)
    {
        return 私有_任务工作线程池参数错误状态文本(
            static_cast<线程池参数错误状态>(
                快照.任务管理工作线程池最近参数错误状态值),
            快照.任务管理工作线程池待保存大小,
            快照.任务管理工作线程池最小大小,
            快照.任务管理工作线程池最大大小);
    }

    std::string 私有_任务工作线程池最近参数应用文本(
        const 结构_控制面板快照& 快照)
    {
        return 私有_任务工作线程池参数应用状态文本(
            static_cast<线程池参数应用状态>(
                快照.任务管理工作线程池最近参数应用状态值),
            快照.任务管理工作线程池当前有效大小);
    }

    // 功能：读取自我待处理方法数量特征。
    I64 私有_读取自我待处理方法数量_控制面板(const 自我类& 自我) noexcept
    {
        I64 待处理方法数量 = 0;
        (void)自我.读取自我I64特征当前值(
            自我特征定义类::类型_自我_待处理方法数量(),
            待处理方法数量);
        return 待处理方法数量;
    }

    // 功能：读取缺口恢复接口反馈摘要显示文本。
    std::string 私有_缺口恢复反馈摘要_控制面板(
        const 自我线程类::结构_缺口恢复接口快照& 恢复快照)
    {
        return 恢复快照.摘要;
    }

    // 功能：执行任务、方法或动作的主体逻辑。
    const 语素入口节点类* 私有_特征_最近执行时间_控制面板() noexcept
    {
        static const 语素入口节点类* s_词 =
            语素集.添加信息入口词("最近执行时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_最近完成时间_控制面板() noexcept
    {
        static const 语素入口节点类* s_词 =
            语素集.添加信息入口词("最近完成时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    存在节点类* 私有_解析方法虚拟存在_控制面板(const 方法节点* 方法) noexcept
    {
        return 方法类::方法虚拟存在只读(方法);
    }

    // 功能：构造方法虚拟存在的显示引用，不创建新结构。
    可解析引用<存在节点类> 私有_方法虚拟存在引用_控制面板(const 方法节点* 方法) noexcept
    {
        return 方法类::方法虚拟存在引用(方法);
    }

    // 功能：从指定来源读取数据或状态。
    bool 私有_读取方法I64特征_控制面板(
        const 方法节点* 方法,
        const 语素入口节点类* 特征类型,
        I64& 输出) noexcept
    {
        auto* 方法虚拟存在 = 私有_解析方法虚拟存在_控制面板(方法);
        return 方法虚拟存在
            && 特征类型
            && 世界树.读取特征_I64(
                reinterpret_cast<const 基础信息节点类*>(方法虚拟存在),
                特征类型,
                输出);
    }

    // 功能：从指定来源读取数据或状态。
    时间戳 私有_读取方法运行账时间_控制面板(
        const 方法节点* 方法,
        const 语素入口节点类* 特征类型) noexcept
    {
        if (!方法 || !特征类型) {
            return 0;
        }
        I64 值 = 0;
        if (私有_读取方法I64特征_控制面板(方法, 特征类型, 值) && 值 > 0) {
            return static_cast<时间戳>(值);
        }
        return 0;
    }

    struct 结构_构建上下文 {
        std::size_t 树广度上限 = 0;
        std::uintptr_t 自我存在指针 = 0;
        std::uintptr_t 自我现实场景指针 = 0;
        std::uintptr_t 自我内部世界指针 = 0;
        std::uintptr_t 当前主需求指针 = 0;
        std::uintptr_t 当前主任务指针 = 0;
        std::uintptr_t 当前主方法指针 = 0;
    };

    结构_控制面板树节点 私有_构建基础信息树节点(
        基础信息节点类* 节点,
        const 结构_构建上下文& 上下文,
        std::size_t 剩余深度,
        路径集合 路径);
    结构_控制面板树节点 私有_构建需求树节点(
        需求节点* 节点,
        const 结构_构建上下文& 上下文,
        std::size_t 剩余深度,
        路径集合 路径,
        bool 列表模式 = false);
    结构_控制面板树节点 私有_构建任务树节点(
        任务节点* 节点,
        const 结构_构建上下文& 上下文,
        std::size_t 剩余深度,
        路径集合 路径);
    结构_控制面板树节点 私有_构建方法树节点(
        方法节点* 节点,
        const 结构_构建上下文& 上下文,
        std::size_t 剩余深度,
        路径集合 路径);
    结构_控制面板树节点 私有_构建基础信息根链骨架(
        基础信息节点类* 节点,
        const 结构_构建上下文& 上下文);
    结构_控制面板树节点 私有_构建需求根链骨架(
        需求节点* 节点,
        const 结构_构建上下文& 上下文);
    结构_控制面板树节点 私有_构建任务根链骨架(
        任务节点* 节点,
        const 结构_构建上下文& 上下文);
    结构_控制面板树节点 私有_构建方法根链骨架(
        方法节点* 节点,
        const 结构_构建上下文& 上下文);
    std::vector<结构_控制面板树节点> 私有_构建需求主信息详情(
        需求节点* 节点,
        const 结构_构建上下文& 上下文);
    std::string 私有_截断文本(const std::string& 文本, std::size_t 上限);

    template<class T节点>
    std::uintptr_t 私有_地址(const T节点* 节点) noexcept
    {
        return reinterpret_cast<std::uintptr_t>(节点);
    }

    // 功能：建立对象、任务、方法或因果之间的绑定关系。
    bool 私有_任务头绑定需求(const 任务节点* 节点) noexcept
    {
        return 节点
            && 任务类::读取任务节点种类(节点) == 枚举_任务节点种类::头结点
            && (节点->主信息.对应需求.指针
                || !节点->主信息.对应需求.主键.empty());
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_任务节点属于需求任务链(const 任务节点* 节点) noexcept
    {
        std::size_t 保护 = 0;
        for (auto* 当前 = 节点;
             当前 && 保护 < 128;
             当前 = reinterpret_cast<const 任务节点*>(当前->父), ++保护) {
            if (任务类::读取任务节点种类(当前) == 枚举_任务节点种类::头结点) {
                return 私有_任务头绑定需求(当前);
            }
            if (当前 == reinterpret_cast<const 任务节点*>(当前->父)) {
                break;
            }
        }
        return false;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_任务节点可在普通任务树显示(const 任务节点* 节点) noexcept
    {
        return 节点
            && (!节点->父 || 私有_任务节点属于需求任务链(节点));
    }

    template<class T节点>
    struct 私有_规范节点类型 {
        using 类型 = std::remove_cv_t<T节点>;
    };

    template<>
    struct 私有_规范节点类型<需求节点类> {
        using 类型 = 需求节点;
    };

    template<>
    struct 私有_规范节点类型<任务节点类> {
        using 类型 = 任务节点;
    };

    template<>
    struct 私有_规范节点类型<方法节点类> {
        using 类型 = 方法节点;
    };

    template<class T节点>
    using 私有_规范节点类型_t = typename 私有_规范节点类型<std::remove_cv_t<T节点>>::类型;

    template<class T节点>
    const 私有_规范节点类型_t<T节点>* 私有_转为规范节点(const T节点* 节点) noexcept
    {
        return reinterpret_cast<const 私有_规范节点类型_t<T节点>*>(节点);
    }

    template<class T节点>
    T节点* 私有_转回原节点(私有_规范节点类型_t<T节点>* 节点) noexcept
    {
        return reinterpret_cast<T节点*>(节点);
    }

    template<class T节点>
    bool 私有_规范节点属于独立树(const T节点* 根节点, const T节点* 候选节点) noexcept
    {
        if (!根节点 || !候选节点) {
            return false;
        }
        if (根节点 == 候选节点) {
            return true;
        }
        for (auto* 当前节点 = reinterpret_cast<const T节点*>(根节点->链下);
             当前节点 && 当前节点 != 根节点;
             当前节点 = reinterpret_cast<const T节点*>(当前节点->链下)) {
            if (当前节点 == 候选节点) {
                return true;
            }
        }
        return false;
    }

    template<class T节点>
    const 私有_规范节点类型_t<T节点>* 私有_当前树根节点() noexcept
    {
        using 节点类型 = 私有_规范节点类型_t<T节点>;
        auto* 自我存在 = 自我.获取自我存在();

        if constexpr (std::is_same_v<节点类型, 基础信息节点类>) {
            return 世界树.世界根();
        }
        else if constexpr (std::is_same_v<节点类型, 需求节点>) {
            return 自我存在 ? 世界树.获取需求根节点(自我存在) : nullptr;
        }
        else if constexpr (std::is_same_v<节点类型, 任务节点>) {
            return 自我存在
                ? reinterpret_cast<const 节点类型*>(世界树.获取任务根节点(自我存在))
                : nullptr;
        }
        else if constexpr (std::is_same_v<节点类型, 方法节点>) {
            return 自我存在
                ? reinterpret_cast<const 节点类型*>(世界树.获取方法根节点(自我存在))
                : nullptr;
        }
        else if constexpr (std::is_same_v<节点类型, 自然句节点类>) {
            return reinterpret_cast<const 节点类型*>(语言集.根指针);
        }
        else if constexpr (std::is_same_v<节点类型, 语素入口节点类>) {
            return 语素集.获取语素树根节点();
        }
        else {
            return nullptr;
        }
    }

    template<class T节点>
    bool 私有_节点属于当前树(const T节点* 候选节点) noexcept
    {
        return 私有_规范节点属于独立树(
            私有_当前树根节点<T节点>(),
            私有_转为规范节点(候选节点));
    }

    template<class T节点>
    const T节点* 私有_按主键解析当前树节点(const std::string& 主键) noexcept
    {
        using 节点类型 = 私有_规范节点类型_t<T节点>;
        if (主键.empty()) {
            return nullptr;
        }

        const auto* 根节点 = 私有_当前树根节点<T节点>();
        if (!根节点) {
            return nullptr;
        }

        if constexpr (requires(const 节点类型* 值) { 值->获取主键(); }) {
            if (根节点->获取主键() == 主键) {
                return 私有_转回原节点<T节点>(const_cast<节点类型*>(根节点));
            }
            for (auto* 当前节点 = reinterpret_cast<const 节点类型*>(根节点->链下);
                 当前节点 && 当前节点 != 根节点;
                 当前节点 = reinterpret_cast<const 节点类型*>(当前节点->链下)) {
                if (当前节点->获取主键() == 主键) {
                    return 私有_转回原节点<T节点>(const_cast<节点类型*>(当前节点));
                }
            }
        }
        return nullptr;
    }

    template<class T节点>
    const T节点* 私有_解析当前树节点(
        const T节点* 指针,
        const std::string& 主键 = {}) noexcept
    {
        if (私有_节点属于当前树(指针)) {
            return 指针;
        }
        return 私有_按主键解析当前树节点<T节点>(主键);
    }

    template<class T节点>
    T节点* 私有_解析当前树节点_可写(
        T节点* 指针,
        const std::string& 主键 = {}) noexcept
    {
        return const_cast<T节点*>(私有_解析当前树节点(
            static_cast<const T节点*>(指针),
            主键));
    }

    std::string 私有_文本列表摘要(
        const std::vector<std::string>& 列表,
        const std::size_t 上限 = 6)
    {
        if (列表.empty()) {
            return {};
        }
        std::ostringstream 输出;
        const std::size_t 数量 = (std::min)(列表.size(), 上限);
        for (std::size_t i = 0; i < 数量; ++i) {
            if (i > 0) {
                输出 << "、";
            }
            输出 << 列表[i];
        }
        if (列表.size() > 数量) {
            输出 << " 等" << 列表.size() << "项";
        }
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_十六进制指针(const std::uintptr_t 值)
    {
        std::ostringstream 输出;
        输出 << "0x" << std::hex << std::uppercase << 值;
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_时间文本(const 时间戳 时间)
    {
        return 时间 == 0 ? "0" : std::to_string(时间);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_时长文本_微秒(const 时间戳 时长)
    {
        std::ostringstream 输出;
        输出 << 时长 << "us";
        if (时长 >= 1000000) {
            输出 << " (" << std::fixed << std::setprecision(3)
                << static_cast<double>(时长) / 1000000.0 << "s)";
        }
        else if (时长 >= 1000) {
            输出 << " (" << std::fixed << std::setprecision(3)
                << static_cast<double>(时长) / 1000.0 << "ms)";
        }
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_起点时长文本(const 时间戳 起点, const 时间戳 终点)
    {
        if (起点 == 0 || 终点 == 0 || 终点 < 起点) {
            return "未记录";
        }
        return 私有_时长文本_微秒(终点 - 起点);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_区间文本(const I64区间& 区间)
    {
        return "[" + std::to_string(区间.低值) + ", " + std::to_string(区间.高值) + "]";
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_可选区间文本(const std::optional<I64区间>& 区间)
    {
        return 区间.has_value() ? 私有_区间文本(*区间) : "空";
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_抽象特征形态文本(const 枚举_抽象特征形态 形态) noexcept
    {
        switch (形态) {
        case 枚举_抽象特征形态::叶子: return "叶子";
        case 枚举_抽象特征形态::复合: return "复合";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_I64值域文本(const I64值域& 值域)
    {
        if (值域.项集.empty()) {
            return "空项集";
        }

        std::ostringstream 输出;
        for (std::size_t i = 0; i < 值域.项集.size(); ++i) {
            if (i != 0) 输出 << " ∪ ";
            const auto& 项 = 值域.项集[i];
            if (const auto* 值 = std::get_if<I64>(&项)) {
                输出 << *值;
            }
            else if (const auto* 区间 = std::get_if<I64区间>(&项)) {
                输出 << 私有_区间文本(*区间);
            }
            else {
                输出 << "未知项";
            }
        }
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_特征值域文本(const std::optional<特征值域>& 值域)
    {
        if (!值域.has_value()) {
            return "空";
        }

        const auto& v = *值域;
        if (std::holds_alternative<空值值域>(v)) {
            return "空值";
        }
        if (std::holds_alternative<空集合值域>(v)) {
            return "空集合";
        }
        if (const auto* i64 = std::get_if<I64值域>(&v)) {
            return "I64{" + 私有_I64值域文本(*i64) + "}";
        }
        if (const auto* vec = std::get_if<VecU句柄值域>(&v)) {
            return "VecU句柄{数量=" + std::to_string(vec->允许句柄.size()) + "}";
        }
        if (const auto* ptr = std::get_if<指针句柄值域>(&v)) {
            return "指针句柄{允许目标抽象数=" + std::to_string(ptr->允许目标抽象.size()) + "}";
        }
        return "未知值域";
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_时间段文本(const 结构_时间段& 时间段)
    {
        return "起=" + 私有_时间文本(时间段.起) + " | 止=" + 私有_时间文本(时间段.止);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_向量文本(const Vector3D& 向量)
    {
        std::ostringstream 输出;
        输出 << "(" << 向量.x << ", " << 向量.y << ", " << 向量.z << ")";
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_词文本(const 语素入口节点类* 词) noexcept
    {
        const auto* 安全词节点 = 私有_解析当前树节点(词);
        if (!安全词节点) {
            return {};
        }
        return 语素集.安全获取词(安全词节点);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_自然句文本(const 自然句节点类* 句子) noexcept
    {
        const auto* 安全句子节点 = 私有_解析当前树节点(句子);
        if (!安全句子节点) {
            return {};
        }
        return "自然句#" + 安全句子节点->获取主键();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_特征值文本(const 特征值& 值)
    {
        if (std::holds_alternative<std::monostate>(值)) {
            return "空";
        }
        if (const auto* 标量 = std::get_if<I64>(&值)) {
            return std::to_string(*标量);
        }
        if (const auto* 向量 = std::get_if<VecU句柄>(&值)) {
            return std::string("VecU@") + 私有_十六进制指针(向量->主信息指针);
        }
        if (const auto* 指针 = std::get_if<指针句柄>(&值)) {
            return std::string("指针@") + 私有_十六进制指针(指针->指针);
        }
        return "未定义";
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_线程生命周期文本(const 枚举_线程生命周期状态 状态) noexcept
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

    // 功能：执行对应模块、线程或方法的运行逻辑。
    const char* 私有_线程运行阶段文本(const 枚举_自我线程运行阶段 阶段) noexcept
    {
        switch (阶段) {
        case 枚举_自我线程运行阶段::自我初始化: return "自我初始化";
        case 枚举_自我线程运行阶段::线程启动中: return "线程启动中";
        case 枚举_自我线程运行阶段::运行态建立: return "运行态建立";
        case 枚举_自我线程运行阶段::主循环消息归并: return "主循环消息归并";
        case 枚举_自我线程运行阶段::主循环初始化补齐: return "主循环初始化补齐";
        case 枚举_自我线程运行阶段::主循环主链回填: return "主循环主链回填";
        case 枚举_自我线程运行阶段::主循环根层裁决: return "主循环根层裁决";
        case 枚举_自我线程运行阶段::主循环任务治理: return "主循环任务治理";
        case 枚举_自我线程运行阶段::主循环结果回流: return "主循环结果回流";
        case 枚举_自我线程运行阶段::主循环心跳巡检: return "主循环心跳巡检";
        case 枚举_自我线程运行阶段::停止请求中: return "停止请求中";
        case 枚举_自我线程运行阶段::线程收尾: return "线程收尾";
        case 枚举_自我线程运行阶段::线程停止: return "线程停止";
        case 枚举_自我线程运行阶段::线程故障: return "线程故障";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_线程最终去向文本(const 枚举_自我线程最终去向 去向) noexcept
    {
        switch (去向) {
        case 枚举_自我线程最终去向::继续提交: return "继续提交";
        case 枚举_自我线程最终去向::回到重筹办: return "回到重筹办";
        case 枚举_自我线程最终去向::转入缺口承接: return "转入缺口承接";
        case 枚举_自我线程最终去向::进入收束: return "进入收束";
        case 枚举_自我线程最终去向::待机: return "待机";
        case 枚举_自我线程最终去向::停止: return "停止";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_任务节点种类文本(const 枚举_任务节点种类 种类) noexcept
    {
        switch (种类) {
        case 枚举_任务节点种类::头结点: return "头结点";
        case 枚举_任务节点种类::步骤节点: return "步骤节点";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_任务状态文本(const 枚举_任务状态 状态) noexcept
    {
        switch (状态) {
        case 枚举_任务状态::未启动: return "未启动";
        case 枚举_任务状态::运行中: return "运行中";
        case 枚举_任务状态::挂起: return "挂起";
        case 枚举_任务状态::完成: return "完成";
        case 枚举_任务状态::已结算: return "已结算";
        case 枚举_任务状态::失败: return "失败";
        case 枚举_任务状态::取消: return "取消";
        case 枚举_任务状态::超时: return "超时";
        case 枚举_任务状态::就绪: return "就绪";
        case 枚举_任务状态::执行中: return "执行中";
        case 枚举_任务状态::筹办中: return "筹办中";
        case 枚举_任务状态::排队中: return "排队中";
        case 枚举_任务状态::等待中: return "等待中";
        case 枚举_任务状态::无法执行: return "无法执行";
        case 枚举_任务状态::待重筹办: return "待重筹办";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_需求状态已达(const 需求节点* 节点) noexcept
    {
        if (!节点) {
            return false;
        }
        const auto* 当前状态 = 节点->主信息.被需求当前状态.指针;
        const auto* 目标状态 = 节点->主信息.需求状态.指针;
        if (当前状态 && 目标状态) {
            const auto 比较 = 世界树.比较状态(当前状态, 目标状态);
            return 比较.可比较
                && 三向关系被接受(比较.关系, 节点->主信息.满足关系);
        }
        const auto& 当前主键 = 节点->主信息.被需求当前状态.主键;
        const auto& 目标主键 = 节点->主信息.需求状态.主键;
        return !当前主键.empty()
            && 当前主键 == 目标主键
            && 三向关系被接受(枚举_三向关系::等于, 节点->主信息.满足关系);
    }

    template<class T节点>
    std::string 私有_引用主键_控制面板(const 可解析引用<T节点>& 引用)
    {
        if (!引用.主键.empty()) {
            return 引用.主键;
        }
        return 引用.指针 ? 引用.指针->获取主键() : std::string{};
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求节点主键探测_无日志(const 需求节点* 节点) noexcept
    {
        if (!节点) {
            return "空";
        }

        try {
            auto 主键 = 节点->获取主键();
            return 主键.empty() ? std::string("空主键") : std::move(主键);
        }
        catch (const std::exception& 异常) {
            return std::string("不可读(") + 异常.what() + ")";
        }
        catch (...) {
            return "不可读(未知异常)";
        }
    }

    // 功能：记录日志、动态、证据或运行痕迹。
    void 私有_记录需求节点子链数量不一致_控制面板(const 需求节点* 节点) noexcept
    {
        if (!节点 || !节点->子 || 节点->子节点数量 > 0) {
            return;
        }

        static std::mutex 互斥{};
        static std::unordered_set<std::uintptr_t> 已记录地址{};

        const auto 地址 = 私有_地址(节点);
        {
            std::lock_guard<std::mutex> 锁{ 互斥 };
            if (!已记录地址.insert(地址).second) {
                return;
            }
        }

        auto* 子 = reinterpret_cast<const 需求节点*>(节点->子);
        std::ostringstream 日志;
        日志 << "控制面板/需求子链数量不一致"
            << " | 地址=" << 私有_十六进制指针(地址)
            << " | 主键=" << 私有_需求节点主键探测_无日志(节点)
            << " | 子节点数量=" << 节点->子节点数量
            << " | 子=" << 私有_十六进制指针(私有_地址(子))
            << " | 子主键=" << 私有_需求节点主键探测_无日志(子)
            << " | 动作=展示层停止枚举该子链";
        项目运行错误日志(日志.str());
    }

    // 功能：记录日志、动态、证据或运行痕迹。
    void 私有_记录异常需求节点链路_控制面板(const 需求节点* 节点) noexcept
    {
        if (!节点) {
            return;
        }

        static std::mutex 互斥{};
        static std::unordered_set<std::uintptr_t> 已记录地址{};

        const auto 地址 = 私有_地址(节点);
        {
            std::lock_guard<std::mutex> 锁{ 互斥 };
            if (!已记录地址.insert(地址).second) {
                return;
            }
        }

        // 这里只做一次性链路诊断，不尝试修复需求树。异常节点很可能来自
        // 写侧挂接/删除或读侧并发刷新，保留父/兄弟指针能帮助定位污染入口。
        auto* 父 = reinterpret_cast<const 需求节点*>(节点->父);
        auto* 上 = reinterpret_cast<const 需求节点*>(节点->上);
        auto* 下 = reinterpret_cast<const 需求节点*>(节点->下);
        auto* 子 = reinterpret_cast<const 需求节点*>(节点->子);

        std::ostringstream 日志;
        日志 << "控制面板/异常需求节点链路"
            << " | 地址=" << 私有_十六进制指针(地址)
            << " | 主键=" << 私有_需求节点主键探测_无日志(节点)
            << " | 父=" << 私有_十六进制指针(私有_地址(父))
            << " | 父主键=" << 私有_需求节点主键探测_无日志(父)
            << " | 上=" << 私有_十六进制指针(私有_地址(上))
            << " | 上主键=" << 私有_需求节点主键探测_无日志(上)
            << " | 下=" << 私有_十六进制指针(私有_地址(下))
            << " | 下主键=" << 私有_需求节点主键探测_无日志(下)
            << " | 子=" << 私有_十六进制指针(私有_地址(子))
            << " | 子主键=" << 私有_需求节点主键探测_无日志(子)
            << " | 子节点数量=" << 节点->子节点数量;
        项目运行错误日志(日志.str());
    }

    bool 私有_尝试读取需求节点主键_控制面板(
        const 需求节点* 节点,
        std::string* 输出主键 = nullptr) noexcept
    {
        if (!节点) {
            return false;
        }
        try {
            auto 主键 = 节点->获取主键();
            if (主键.empty()) {
                std::ostringstream 日志;
                日志 << "控制面板/需求主键为空"
                    << " | 地址=" << 私有_十六进制指针(私有_地址(节点));
                项目运行错误日志(日志.str());
                私有_记录异常需求节点链路_控制面板(节点);
                return false;
            }
            if (输出主键) {
                *输出主键 = std::move(主键);
            }
            return true;
        }
        catch (const std::exception& 异常) {
            std::ostringstream 日志;
            日志 << "控制面板/需求主键读取失败"
                << " | 地址=" << 私有_十六进制指针(私有_地址(节点))
                << " | 异常=" << 异常.what();
            项目运行错误日志(日志.str());
            私有_记录异常需求节点链路_控制面板(节点);
        }
        catch (...) {
            std::ostringstream 日志;
            日志 << "控制面板/需求主键读取失败"
                << " | 地址=" << 私有_十六进制指针(私有_地址(节点))
                << " | 异常=未知异常";
            项目运行错误日志(日志.str());
            私有_记录异常需求节点链路_控制面板(节点);
        }
        return false;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_节点主键_控制面板(const 需求节点* 节点)
    {
        std::string 主键{};
        if (私有_尝试读取需求节点主键_控制面板(节点, &主键)) {
            return 主键;
        }
        return {};
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_节点主键_控制面板(const 语素入口节点类* 节点)
    {
        return 节点 ? 节点->获取主键() : std::string{};
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_任务节点主键_控制面板(const 任务节点* 节点)
    {
        return 节点 ? 节点->获取主键() : std::string{};
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_累计任务状态(结构_控制面板快照& 快照, const 枚举_任务状态 状态) noexcept
    {
        switch (状态) {
        case 枚举_任务状态::未启动: ++快照.任务状态未启动数; break;
        case 枚举_任务状态::运行中: ++快照.任务状态运行中数; break;
        case 枚举_任务状态::挂起: ++快照.任务状态挂起数; break;
        case 枚举_任务状态::完成: ++快照.任务状态完成数; break;
        case 枚举_任务状态::已结算: ++快照.任务状态已结算数; break;
        case 枚举_任务状态::失败: ++快照.任务状态失败数; break;
        case 枚举_任务状态::取消: ++快照.任务状态取消数; break;
        case 枚举_任务状态::超时: ++快照.任务状态超时数; break;
        case 枚举_任务状态::就绪: ++快照.任务状态就绪数; break;
        case 枚举_任务状态::执行中: ++快照.任务状态执行中数; break;
        case 枚举_任务状态::筹办中: ++快照.任务状态筹办中数; break;
        case 枚举_任务状态::排队中: ++快照.任务状态排队中数; break;
        case 枚举_任务状态::等待中: ++快照.任务状态等待中数; break;
        case 枚举_任务状态::无法执行: ++快照.任务状态无法执行数; break;
        case 枚举_任务状态::待重筹办: ++快照.任务状态待重筹办数; break;
        case 枚举_任务状态::未定义:
        default:
            ++快照.任务状态未定义数;
            break;
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加非零计数(std::ostringstream& 输出, bool& 已写, const char* 名称, std::size_t 数量)
    {
        if (数量 == 0) {
            return;
        }
        if (已写) {
            输出 << " | ";
        }
        输出 << 名称 << "=" << 数量;
        已写 = true;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加计数(std::ostringstream& 输出, bool& 已写, const char* 名称, std::size_t 数量)
    {
        if (已写) {
            输出 << " | ";
        }
        输出 << 名称 << "=" << 数量;
        已写 = true;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_任务状态数量摘要(const 结构_控制面板快照& 快照)
    {
        std::ostringstream 输出;
        bool 已写 = false;
        私有_追加非零计数(输出, 已写, "未定义", 快照.任务状态未定义数);
        私有_追加非零计数(输出, 已写, "未启动", 快照.任务状态未启动数);
        私有_追加计数(输出, 已写, "运行中", 快照.任务状态运行中数);
        私有_追加非零计数(输出, 已写, "挂起", 快照.任务状态挂起数);
        if (快照.任务数 > 0) {
            私有_追加计数(输出, 已写, "完成", 快照.任务状态完成数);
        }
        else {
            私有_追加非零计数(输出, 已写, "完成", 快照.任务状态完成数);
        }
        私有_追加非零计数(输出, 已写, "已结算", 快照.任务状态已结算数);
        私有_追加非零计数(输出, 已写, "失败", 快照.任务状态失败数);
        私有_追加非零计数(输出, 已写, "取消", 快照.任务状态取消数);
        私有_追加非零计数(输出, 已写, "超时", 快照.任务状态超时数);
        私有_追加非零计数(输出, 已写, "就绪", 快照.任务状态就绪数);
        私有_追加非零计数(输出, 已写, "执行中", 快照.任务状态执行中数);
        私有_追加非零计数(输出, 已写, "筹办中", 快照.任务状态筹办中数);
        私有_追加非零计数(输出, 已写, "排队中", 快照.任务状态排队中数);
        私有_追加非零计数(输出, 已写, "等待中", 快照.任务状态等待中数);
        私有_追加非零计数(输出, 已写, "无法执行", 快照.任务状态无法执行数);
        私有_追加非零计数(输出, 已写, "待重筹办", 快照.任务状态待重筹办数);
        return 已写 ? 输出.str() : "暂无任务状态";
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求满足数量摘要(const 结构_控制面板快照& 快照)
    {
        std::ostringstream 输出;
        输出 << "已满足=" << 快照.需求已满足数
            << " | 状态已达未截止=" << 快照.需求状态已达未截止数
            << " | 未满足=" << 快照.需求未满足数;
        return 输出.str();
    }

    // 功能：统计方法首节点下的直接条件和结果结构。
    void 私有_统计方法直接条件结果_控制面板(
        const 方法节点* 方法首节点,
        std::size_t& 条件数,
        std::size_t& 直接结果数,
        std::size_t& 条件下结果数,
        std::size_t& 条件无结果数) noexcept
    {
        条件数 = 0;
        直接结果数 = 0;
        条件下结果数 = 0;
        条件无结果数 = 0;
        if (!方法首节点 || !方法首节点->子) {
            return;
        }
        auto* 首子 = reinterpret_cast<const 方法节点*>(方法首节点->子);
        auto* 当前 = 首子;
        std::size_t 保护 = 0;
        do {
            if (方法类::方法节点种类(当前) == 枚举_方法节点种类::方法条件节点) {
                ++条件数;
                std::size_t 当前条件结果数 = 0;
                if (当前->子) {
                    auto* 首结果 = reinterpret_cast<const 方法节点*>(当前->子);
                    auto* 结果 = 首结果;
                    std::size_t 结果保护 = 0;
                    do {
                        if (方法类::方法节点种类(结果) == 枚举_方法节点种类::方法结果节点) {
                            ++当前条件结果数;
                        }
                        结果 = 结果 ? reinterpret_cast<const 方法节点*>(结果->下) : nullptr;
                        ++结果保护;
                    } while (结果 && 结果 != 首结果 && 结果保护 < 1024);
                }
                条件下结果数 += 当前条件结果数;
                if (当前条件结果数 == 0) {
                    ++条件无结果数;
                }
            }
            else if (方法类::方法节点种类(当前) == 枚举_方法节点种类::方法结果节点) {
                ++直接结果数;
            }
            当前 = 当前 ? reinterpret_cast<const 方法节点*>(当前->下) : nullptr;
            ++保护;
        } while (当前 && 当前 != 首子 && 保护 < 2048);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_方法结构诊断摘要(const 结构_控制面板快照& 快照)
    {
        std::ostringstream 输出;
        输出 << "首节点=" << 快照.方法首节点数
            << " | 条件无结果=" << 快照.方法条件无结果数
            << " | 结果无条件=" << 快照.方法结果无条件数
            << " | 条件结果不成对=" << 快照.方法条件结果不成对数;
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_方法节点种类文本(const 枚举_方法节点种类 种类) noexcept
    {
        switch (种类) {
        case 枚举_方法节点种类::方法首节点: return "方法首节点";
        case 枚举_方法节点种类::方法条件节点: return "方法条件节点";
        case 枚举_方法节点种类::方法结果节点: return "方法结果节点";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_方法来源文本(const 枚举_方法来源 来源) noexcept
    {
        switch (来源) {
        case 枚举_方法来源::本能: return "本能";
        case 枚举_方法来源::外部导入: return "外部导入";
        case 枚举_方法来源::因果抽象: return "因果抽象";
        case 枚举_方法来源::观察抽象: return "观察抽象";
        case 枚举_方法来源::路径提炼: return "路径提炼";
        case 枚举_方法来源::运行时方法沉淀: return "运行时方法沉淀";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_动作句柄类型文本(const 枚举_动作句柄类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_动作句柄类型::本能函数ID: return "本能函数ID";
        case 枚举_动作句柄类型::外部实现主键: return "外部实现主键";
        case 枚举_动作句柄类型::动作序列主键: return "动作序列主键";
        default: return "未设置";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_世界类型文本(const 枚举_世界类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_世界类型::现实世界: return "现实世界";
        case 枚举_世界类型::文本世界: return "文本世界";
        case 枚举_世界类型::想象世界: return "想象世界";
        case 枚举_世界类型::记忆世界: return "记忆世界";
        case 枚举_世界类型::推理世界: return "推理世界";
        case 枚举_世界类型::内部世界: return "内部世界";
        case 枚举_世界类型::虚拟世界: return "虚拟世界";
        default: return "其它";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_动态聚合方式文本(const 枚举_动态聚合方式 方式) noexcept
    {
        switch (方式) {
        case 枚举_动态聚合方式::原子相邻: return "原子相邻";
        case 枚举_动态聚合方式::连续同向段: return "连续同向段";
        case 枚举_动态聚合方式::同类事件段: return "同类事件段";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_因果锚点类型文本(const 枚举_因果锚点类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_因果锚点类型::场景: return "场景";
        case 枚举_因果锚点类型::存在: return "存在";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_任务管理总控结果文本(const 枚举_任务管理总控结果 结果) noexcept
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

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_主信息类型文本(const 枚举_主信息类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_主信息类型::特征: return "特征";
        case 枚举_主信息类型::抽象特征: return "抽象特征";
        case 枚举_主信息类型::存在: return "存在";
        case 枚举_主信息类型::场景: return "场景";
        case 枚举_主信息类型::状态: return "状态";
        case 枚举_主信息类型::动态: return "动态";
        case 枚举_主信息类型::因果: return "因果";
        case 枚举_主信息类型::二次特征_修饰: return "二次特征";
        case 枚举_主信息类型::指代: return "指代";
        default: return "基础信息";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_信息存储树文本(const 枚举_信息存储树 存储树) noexcept
    {
        switch (存储树) {
        case 枚举_信息存储树::基础信息模板树: return "基础信息模板树";
        case 枚举_信息存储树::现实世界树: return "现实世界树";
        case 枚举_信息存储树::内部世界树: return "内部世界树";
        case 枚举_信息存储树::需求根树: return "需求根树";
        case 枚举_信息存储树::任务根树: return "任务根树";
        case 枚举_信息存储树::方法根树: return "方法根树";
        case 枚举_信息存储树::语言记录树: return "语言记录树";
        case 枚举_信息存储树::不入树: return "不入树";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_信息入口类型文本(const 枚举_信息入口类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_信息入口类型::存在概念入口: return "存在概念入口";
        case 枚举_信息入口类型::特征模板入口: return "特征模板入口";
        case 枚举_信息入口类型::状态模板入口: return "状态模板入口";
        case 枚举_信息入口类型::动态模板入口: return "动态模板入口";
        case 枚举_信息入口类型::关系模板入口: return "关系模板入口";
        case 枚举_信息入口类型::因果模板入口: return "因果模板入口";
        case 枚举_信息入口类型::场景模板入口: return "场景模板入口";
        case 枚举_信息入口类型::存在实例入口: return "存在实例入口";
        case 枚举_信息入口类型::特征实例入口: return "特征实例入口";
        case 枚举_信息入口类型::状态实例入口: return "状态实例入口";
        case 枚举_信息入口类型::动态实例入口: return "动态实例入口";
        case 枚举_信息入口类型::关系实例入口: return "关系实例入口";
        case 枚举_信息入口类型::场景实例入口: return "场景实例入口";
        case 枚举_信息入口类型::需求概念入口: return "需求概念入口";
        case 枚举_信息入口类型::任务概念入口: return "任务概念入口";
        case 枚举_信息入口类型::方法概念入口: return "方法概念入口";
        case 枚举_信息入口类型::需求节点入口: return "需求节点入口";
        case 枚举_信息入口类型::任务信息节点入口: return "任务信息节点入口";
        case 枚举_信息入口类型::任务虚拟存在入口: return "任务虚拟存在入口";
        case 枚举_信息入口类型::方法信息节点入口: return "方法信息节点入口";
        case 枚举_信息入口类型::方法虚拟存在入口: return "方法虚拟存在入口";
        case 枚举_信息入口类型::数值入口: return "数值入口";
        case 枚举_信息入口类型::单位入口: return "单位入口";
        case 枚举_信息入口类型::比较入口: return "比较入口";
        case 枚举_信息入口类型::逻辑连接入口: return "逻辑连接入口";
        case 枚举_信息入口类型::语言记录入口: return "语言记录入口";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_语素入口诊断文本(const 语素入口节点类* 入口节点)
    {
        if (!入口节点 || !入口节点->主信息) {
            return "空";
        }

        if (!语素集.是语素入口节点(入口节点)) {
            return "非语素入口节点";
        }

        const auto 人类词性 = 语素集.获取语素入口人类词性(入口节点);
        const auto 信息入口类型 = 语素集.获取语素入口信息入口类型(入口节点);
        const auto 显式主信息类型 = 语素集.获取语素入口显式对应基础信息类型(入口节点);
        const auto 主信息类型 = 显式主信息类型 != 枚举_主信息类型::未定义
            ? 显式主信息类型
            : 语素类::读取信息入口主信息类型(信息入口类型);
        const auto* 默认根名 = 语素类::读取信息入口默认根名(信息入口类型);

        std::ostringstream 输出;
        输出 << "人类词性=" << 枚举_词性_工厂::根据枚举类型获取文本(人类词性)
            << " | 信息入口=" << 私有_信息入口类型文本(信息入口类型)
            << " | 主信息类型=" << 私有_主信息类型文本(主信息类型)
            << " | 存储树=" << 私有_信息存储树文本(语素类::读取信息入口存储树(信息入口类型))
            << " | 默认根=" << (默认根名 ? 默认根名 : "空")
            << " | 需上下文=" << (语素类::信息入口需要上下文限定(信息入口类型) ? "是" : "否")
            << " | 裸词模板=" << (语素类::信息入口裸词默认模板入口(信息入口类型) ? "是" : "否");
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_动作句柄文本(const 结构体_动作句柄& 句柄)
    {
        std::ostringstream 输出;
        输出 << 私有_动作句柄类型文本(句柄.类型);
        switch (句柄.类型) {
        case 枚举_动作句柄类型::本能函数ID:
            输出 << " | 本能ID=" << 句柄.本能ID;
            break;
        case 枚举_动作句柄类型::外部实现主键:
            输出 << " | 外部实现主键=" << (句柄.外部实现主键.empty() ? "空" : 句柄.外部实现主键);
            break;
        case 枚举_动作句柄类型::动作序列主键:
            输出 << " | 动作序列主键=" << (句柄.动作序列主键.empty() ? "空" : 句柄.动作序列主键);
            break;
        default:
            break;
        }
        return 输出.str();
    }

    std::string 私有_需求标题(const 需求节点* 节点);
    std::string 私有_任务标题(const 任务节点* 节点);
    std::string 私有_方法标题(const 方法节点* 节点);
    std::string 私有_基础信息标题(const 基础信息节点类* 节点, const 结构_构建上下文& 上下文);

    template<class T节点>
    std::string 私有_对象摘要(const T节点* 节点)
    {
        if (!节点) {
            return "空";
        }

        if constexpr (requires(const T节点* 值) { 值->获取主键(); }) {
            std::ostringstream 输出;
            输出 << "节点#" << 节点->获取主键();
            return 输出.str();
        }
        else {
            return "引用节点";
        }
    }

    template<class T节点>
    std::string 私有_安全节点摘要(const T节点* 节点, const char* 前缀)
    {
        if (!节点) {
            return "空";
        }

        std::ostringstream 输出;
        输出 << 前缀;
        if constexpr (requires(const T节点* 值) { 值->获取主键(); }) {
            输出 << "#" << 节点->获取主键();
        }
        输出 << " | 地址=" << 私有_十六进制指针(私有_地址(节点));
        return 输出.str();
    }

    template<>
    std::string 私有_对象摘要<基础信息节点类>(const 基础信息节点类* 节点)
    {
        if (!节点) {
            return "空";
        }
        结构_构建上下文 空上下文{};
        return 私有_基础信息标题(节点, 空上下文);
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_场景复现特征词(const char* 名称) noexcept
    {
        try {
            if (!名称 || !*名称) {
                return nullptr;
            }
            return 语素集.查找信息入口节点(名称, 枚举_信息入口类型::特征模板入口);
        }
        catch (...) {
            return nullptr;
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_场景复现存在概念词(const char* 名称) noexcept
    {
        try {
            if (!名称 || !*名称) {
                return nullptr;
            }
            return 语素集.查找信息入口节点(名称, 枚举_信息入口类型::存在概念入口);
        }
        catch (...) {
            return nullptr;
        }
    }

    // 功能：从指定来源读取数据或状态。
    bool 私有_读取场景复现I64(
        const 基础信息节点类* 宿主,
        const char* 特征名,
        I64& 输出值) noexcept
    {
        输出值 = 0;
        const auto* 特征类型 = 私有_场景复现特征词(特征名);
        return 宿主
            && 特征类型
            && 世界树.读取特征_I64(宿主, 特征类型, 输出值);
    }

    // 功能：从指定来源读取数据或状态。
    bool 私有_读取场景复现I64_保留默认(
        const 基础信息节点类* 宿主,
        const char* 特征名,
        I64& 输出值) noexcept
    {
        I64 临时值 = 0;
        if (!私有_读取场景复现I64(宿主, 特征名, 临时值)) {
            return false;
        }
        输出值 = 临时值;
        return true;
    }

    // 功能：从指定来源读取数据或状态。
    bool 私有_读取场景复现VecI64(
        const 基础信息节点类* 宿主,
        const char* 特征名,
        VecI64& 输出值) noexcept
    {
        输出值.clear();
        const auto* 特征类型 = 私有_场景复现特征词(特征名);
        return 宿主
            && 特征类型
            && 世界树.读取特征VecI64(宿主, 特征类型, 输出值);
    }

    // 功能：服务所在模块的内部辅助流程。
    int 私有_场景复现宿主评分(const 基础信息节点类* 宿主) noexcept
    {
        if (!宿主) {
            return 0;
        }

        int 评分 = 0;
        I64 值 = 0;
        const auto 读非零加分 = [&](const char* 特征名, int 加分) noexcept {
            if (私有_读取场景复现I64(宿主, 特征名, 值) && 值 != 0) {
                评分 += 加分;
            }
        };

        读非零加分("当前观察帧", 16);
        读非零加分("当前帧像素特征索引", 12);
        读非零加分("空间候选集合", 16);
        读非零加分("观察存在假设", 12);
        读非零加分("像素归属验证结果", 12);
        读非零加分("已验证观察存在", 12);
        if (私有_读取场景复现I64(宿主, "相机帧宽度", 值) && 值 > 0) {
            评分 += 4;
        }
        if (私有_读取场景复现I64(宿主, "相机帧高度", 值) && 值 > 0) {
            评分 += 4;
        }
        if (私有_读取场景复现I64(宿主, "空间候选数量", 值) && 值 > 0) {
            评分 += 20;
        }
        if (私有_读取场景复现I64(宿主, "诊断区域数量", 值) && 值 > 0) {
            评分 += 10;
        }
        if (私有_读取场景复现I64(宿主, "已验证观察存在数量", 值) && 值 > 0) {
            评分 += 24;
        }
        if (私有_读取场景复现I64(宿主, "深度帧号", 值) && 值 > 0) {
            评分 += 1;
        }
        return 评分;
    }

    // 功能：从候选集中选择当前使用对象或方法。
    基础信息节点类* 私有_选择自我场景复现宿主(场景节点类* 自我所在场景) noexcept
    {
        auto* 场景宿主 = reinterpret_cast<基础信息节点类*>(自我所在场景);
        if (!场景宿主) {
            return nullptr;
        }

        基础信息节点类* 最佳宿主 = nullptr;
        int 最佳评分 = 0;
        I64 最佳深度帧号 = -1;
        std::vector<基础信息节点类*> 栈{};
        栈.push_back(场景宿主);

        std::size_t 已扫描 = 0;
        while (!栈.empty() && 已扫描 < 4096) {
            auto* 当前 = 栈.back();
            栈.pop_back();
            ++已扫描;
            if (!当前) {
                continue;
            }

            const int 当前评分 = 私有_场景复现宿主评分(当前);
            I64 当前深度帧号 = -1;
            (void)私有_读取场景复现I64(当前, "深度帧号", 当前深度帧号);
            if (当前评分 > 最佳评分
                || (当前评分 == 最佳评分 && 当前评分 > 0 && 当前深度帧号 > 最佳深度帧号)) {
                最佳宿主 = 当前;
                最佳评分 = 当前评分;
                最佳深度帧号 = 当前深度帧号;
            }

            for (auto* 子节点 : 私有_枚举子节点(当前, (std::numeric_limits<std::size_t>::max)())) {
                if (子节点) {
                    栈.push_back(子节点);
                }
            }
        }

        return 最佳宿主 ? 最佳宿主 : 场景宿主;
    }

    // 功能：评估状态、风险、质量、权重或完成度。
    int 私有_场景安全评估宿主评分(const 基础信息节点类* 宿主) noexcept
    {
        if (!宿主) {
            return 0;
        }

        int 评分 = 0;
        I64 值 = 0;
        const auto 读到加分 = [&](const char* 特征名, int 加分) noexcept {
            if (私有_读取场景复现I64(宿主, 特征名, 值)) {
                评分 += 加分;
            }
        };

        读到加分("基础观察事实可用状态", 16);
        读到加分("基础风险判断状态", 16);
        读到加分("风险状态明确状态", 16);
        读到加分("基础风险值计算状态", 10);
        读到加分("当前场景评估安全值候选", 12);
        读到加分("当前场景安全度候选", 12);
        读到加分("当前场景安全判定状态", 10);
        读到加分("安全评估证据不足原因", 10);
        读到加分("风险安全_场景影响部分候选", 8);
        读到加分("风险安全_场景影响部分可结算状态", 8);
        读到加分("风险安全_场景影响部分不可结算原因", 6);
        读到加分("风险安全_场景影响部分已入账状态", 6);
        读到加分("风险安全层候选", 8);
        读到加分("风险安全层投影候选", 8);
        读到加分("风险安全层缺失因素数量", 4);
        读到加分("风险安全层明确状态", 6);
        读到加分("风险安全层可聚合状态", 6);
        读到加分("风险安全层已入账状态", 4);
        读到加分("风险因素未搜索掩码", 4);
        读到加分("风险因素搜索缺口掩码", 4);
        读到加分("风险因素负向证据掩码", 4);
        读到加分("风险因素默认满足掩码", 4);
        读到加分("风险因素已入账掩码", 4);
        读到加分("风险因素证据搜索未完成数量", 4);
        读到加分("风险因素默认满足数量", 4);
        读到加分("当前场景已确认存在数量", 4);
        return 评分;
    }

    // 功能：从候选集中选择当前使用对象或方法。
    基础信息节点类* 私有_选择自我场景安全评估宿主(场景节点类* 自我所在场景) noexcept
    {
        auto* 场景宿主 = reinterpret_cast<基础信息节点类*>(自我所在场景);
        if (!场景宿主) {
            return nullptr;
        }

        基础信息节点类* 最佳宿主 = nullptr;
        int 最佳评分 = 0;
        I64 最佳深度帧号 = -1;
        I64 最佳观察帧 = -1;
        std::vector<基础信息节点类*> 栈{};
        栈.push_back(场景宿主);

        std::size_t 已扫描 = 0;
        while (!栈.empty() && 已扫描 < 4096) {
            auto* 当前 = 栈.back();
            栈.pop_back();
            ++已扫描;
            if (!当前) {
                continue;
            }

            const int 当前评分 = 私有_场景安全评估宿主评分(当前);
            I64 当前深度帧号 = -1;
            I64 当前观察帧 = -1;
            (void)私有_读取场景复现I64(当前, "深度帧号", 当前深度帧号);
            (void)私有_读取场景复现I64(当前, "当前观察帧", 当前观察帧);
            if (当前评分 > 最佳评分
                || (当前评分 == 最佳评分
                    && 当前评分 > 0
                    && (当前深度帧号 > 最佳深度帧号
                        || (当前深度帧号 == 最佳深度帧号 && 当前观察帧 > 最佳观察帧)))) {
                最佳宿主 = 当前;
                最佳评分 = 当前评分;
                最佳深度帧号 = 当前深度帧号;
                最佳观察帧 = 当前观察帧;
            }

            for (auto* 子节点 : 私有_枚举子节点(当前, (std::numeric_limits<std::size_t>::max)())) {
                if (子节点) {
                    栈.push_back(子节点);
                }
            }
        }

        return 最佳宿主;
    }

    struct 结构_自我场景内容统计 {
        static constexpr std::size_t 扫描上限 = 4096;
        I64 节点数 = 0;
        I64 存在数 = 0;
        I64 场景数 = 0;
        I64 特征数 = 0;
        I64 状态数 = 0;
        I64 动态数 = 0;
        I64 二次特征数 = 0;
        bool 扫描达到上限 = false;
        std::vector<std::string> 存在样例{};
    };

    // 功能：服务所在模块的内部辅助流程。
    void 私有_累计自我场景内容节点(
        结构_自我场景内容统计& 统计,
        const 基础信息节点类* 节点,
        const bool 采集样例) noexcept
    {
        if (!节点) {
            return;
        }
        ++统计.节点数;
        const auto* 主信息 = 节点->主信息;
        if (dynamic_cast<const 存在节点主信息类*>(主信息)) {
            ++统计.存在数;
            if (采集样例 && 统计.存在样例.size() < 6) {
                统计.存在样例.push_back(私有_对象摘要(节点));
            }
        }
        else if (dynamic_cast<const 场景节点主信息类*>(主信息)) {
            ++统计.场景数;
        }
        else if (dynamic_cast<const 特征节点主信息类*>(主信息)) {
            ++统计.特征数;
        }
        else if (dynamic_cast<const 状态节点主信息类*>(主信息)) {
            ++统计.状态数;
        }
        else if (dynamic_cast<const 动态节点主信息类*>(主信息)) {
            ++统计.动态数;
        }
        else if (dynamic_cast<const 二次特征主信息类*>(主信息)) {
            ++统计.二次特征数;
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    结构_自我场景内容统计 私有_统计自我场景子树内容(
        const 基础信息节点类* 根,
        const bool 采集样例) noexcept
    {
        结构_自我场景内容统计 统计{};
        if (!根) {
            return 统计;
        }

        std::vector<const 基础信息节点类*> 栈{};
        for (auto* 子节点 : 私有_枚举子节点(根, (std::numeric_limits<std::size_t>::max)())) {
            if (子节点) {
                栈.push_back(子节点);
            }
        }

        路径集合 已访问{};
        std::size_t 已扫描 = 0;
        while (!栈.empty() && 已扫描 < 结构_自我场景内容统计::扫描上限) {
            const auto* 当前 = 栈.back();
            栈.pop_back();
            if (!当前) {
                continue;
            }
            const auto 地址 = 私有_地址(当前);
            if (!已访问.insert(地址).second) {
                continue;
            }
            ++已扫描;
            私有_累计自我场景内容节点(统计, 当前, 采集样例);
            for (auto* 子节点 : 私有_枚举子节点(当前, (std::numeric_limits<std::size_t>::max)())) {
                if (子节点) {
                    栈.push_back(子节点);
                }
            }
        }
        统计.扫描达到上限 = !栈.empty();
        return 统计;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_自我场景内容类型摘要(const 结构_自我场景内容统计& 统计)
    {
        std::ostringstream 输出;
        std::size_t 已写入 = 0;
        const auto 追加 = [&](const char* 名称, const I64 数量) {
            if (数量 <= 0) {
                return;
            }
            if (已写入 > 0) {
                输出 << " / ";
            }
            输出 << 名称 << 数量;
            ++已写入;
        };
        追加("存在", 统计.存在数);
        追加("场景", 统计.场景数);
        追加("特征", 统计.特征数);
        追加("状态", 统计.状态数);
        追加("动态", 统计.动态数);
        追加("二次特征", 统计.二次特征数);
        return 已写入 > 0 ? 输出.str() : "空";
    }

    // 功能：从指定来源读取数据或状态。
    void 私有_读取自我场景内容统计(
        结构_控制面板快照& 快照,
        场景节点类* 自我所在场景,
        基础信息节点类* 复现宿主) noexcept
    {
        auto* 场景宿主 = reinterpret_cast<基础信息节点类*>(自我所在场景);
        if (!场景宿主) {
            快照.自我场景直接子层摘要 = "空";
            快照.自我场景子树类型摘要 = "空";
            快照.自我场景存在样例摘要 = "空";
            快照.自我场景复现宿主存在样例摘要 = "空";
            return;
        }

        const auto 直接子节点 = 私有_枚举子节点(场景宿主, (std::numeric_limits<std::size_t>::max)());
        结构_自我场景内容统计 直接统计{};
        for (auto* 子节点 : 直接子节点) {
            私有_累计自我场景内容节点(直接统计, 子节点, false);
        }

        const auto 场景子树统计 = 私有_统计自我场景子树内容(场景宿主, true);
        const auto 宿主子树统计 = 私有_统计自我场景子树内容(复现宿主, true);

        快照.自我场景直接子节点数量 = static_cast<I64>(直接子节点.size());
        快照.自我场景子树节点数量 = 场景子树统计.节点数;
        快照.自我场景直接存在数量 = 直接统计.存在数;
        快照.自我场景子树存在数量 = 场景子树统计.存在数;
        快照.自我场景子树场景数量 = 场景子树统计.场景数;
        快照.自我场景子树特征数量 = 场景子树统计.特征数;
        快照.自我场景子树状态数量 = 场景子树统计.状态数;
        快照.自我场景子树动态数量 = 场景子树统计.动态数;
        快照.自我场景子树二次特征数量 = 场景子树统计.二次特征数;
        快照.自我场景复现宿主子树节点数量 = 宿主子树统计.节点数;
        快照.自我场景复现宿主子树存在数量 = 宿主子树统计.存在数;
        快照.自我场景子树扫描上限 = static_cast<I64>(结构_自我场景内容统计::扫描上限);
        快照.自我场景子树扫描达到上限 = 场景子树统计.扫描达到上限 ? 1 : 0;
        快照.自我场景复现宿主子树扫描达到上限 = 宿主子树统计.扫描达到上限 ? 1 : 0;
        快照.自我场景直接子层摘要 = 私有_自我场景内容类型摘要(直接统计);
        快照.自我场景子树类型摘要 = 私有_自我场景内容类型摘要(场景子树统计);
        快照.自我场景存在样例摘要 = 场景子树统计.存在样例.empty()
            ? std::string("空")
            : 私有_文本列表摘要(场景子树统计.存在样例, 6);
        快照.自我场景复现宿主存在样例摘要 = 宿主子树统计.存在样例.empty()
            ? std::string("空")
            : 私有_文本列表摘要(宿主子树统计.存在样例, 6);
    }

    // 功能：从指定来源读取数据或状态。
    void 私有_读取自我场景诊断区域列表(
        基础信息节点类* 宿主,
        结构_控制面板快照& 快照) noexcept
    {
        快照.自我场景诊断区域列表.clear();
        const auto* 区域类型 = 私有_场景复现存在概念词("观察诊断区域");
        if (!宿主 || !区域类型) {
            return;
        }

        const auto 读区域值 = [](基础信息节点类* 节点, const char* 特征名, std::int64_t& 字段) noexcept {
            I64 值 = static_cast<I64>(字段);
            if (私有_读取场景复现I64_保留默认(节点, 特征名, 值)) {
                字段 = static_cast<std::int64_t>(值);
            }
        };

        std::size_t 已扫描 = 0;
        for (auto* 子存在 : 世界树.获取子存在(宿主)) {
            if (++已扫描 > 1024) {
                break;
            }
            const auto* 主信息 = 世界树.取存在主信息(子存在);
            if (!主信息 || 主信息->类型 != 区域类型) {
                continue;
            }
            auto* 节点 = reinterpret_cast<基础信息节点类*>(子存在);
            if (!节点) {
                continue;
            }

            结构_控制面板诊断区域 区域{};
            读区域值(节点, "诊断区域编号", 区域.区域编号);
            读区域值(节点, "诊断区域图层类型", 区域.图层类型);
            读区域值(节点, "诊断区域来源候选编号", 区域.来源候选编号);
            读区域值(节点, "诊断区域投影最小X", 区域.投影最小X);
            读区域值(节点, "诊断区域投影最大X", 区域.投影最大X);
            读区域值(节点, "诊断区域投影最小Y", 区域.投影最小Y);
            读区域值(节点, "诊断区域投影最大Y", 区域.投影最大Y);
            读区域值(节点, "诊断区域像素数量", 区域.像素数量);
            读区域值(节点, "诊断区域置信度", 区域.置信度);
            读区域值(节点, "诊断区域掩码状态", 区域.掩码状态);
            if (区域.图层类型 <= 0
                || 区域.投影最大X < 区域.投影最小X
                || 区域.投影最大Y < 区域.投影最小Y) {
                continue;
            }
            快照.自我场景诊断区域列表.push_back(区域);
        }

        std::sort(
            快照.自我场景诊断区域列表.begin(),
            快照.自我场景诊断区域列表.end(),
            [](const auto& a, const auto& b) noexcept {
                if (a.图层类型 != b.图层类型) {
                    return a.图层类型 < b.图层类型;
                }
                return a.区域编号 < b.区域编号;
            });
        if (快照.自我场景诊断区域列表.size() > 256) {
            快照.自我场景诊断区域列表.resize(256);
        }
        if (!快照.自我场景诊断区域列表.empty()) {
            if (快照.自我场景诊断区域数量 <= 0) {
                快照.自我场景诊断区域数量 = static_cast<std::int64_t>(快照.自我场景诊断区域列表.size());
            }
            快照.自我场景诊断区域集合状态 = 1;
            快照.自我场景诊断区域掩码状态 = 1;
        }
    }

    // 功能：从指定来源读取数据或状态。
    bool 私有_读取自我场景存在三元特征(
        const 基础信息节点类* 节点,
        const char* x名,
        const char* y名,
        const char* z名,
        std::int64_t& x,
        std::int64_t& y,
        std::int64_t& z) noexcept
    {
        I64 vx = 0;
        I64 vy = 0;
        I64 vz = 0;
        if (!私有_读取场景复现I64(节点, x名, vx)
            || !私有_读取场景复现I64(节点, y名, vy)
            || !私有_读取场景复现I64(节点, z名, vz)) {
            return false;
        }
        x = static_cast<std::int64_t>(vx);
        y = static_cast<std::int64_t>(vy);
        z = static_cast<std::int64_t>(vz);
        return true;
    }

    // 功能：从指定来源读取数据或状态。
    bool 私有_读取自我场景存在AABB特征(
        const 基础信息节点类* 节点,
        结构_控制面板自我场景存在复现项& 项) noexcept
    {
        I64 minX = 0;
        I64 maxX = 0;
        I64 minY = 0;
        I64 maxY = 0;
        I64 minZ = 0;
        I64 maxZ = 0;
        if (!私有_读取场景复现I64(节点, "范围坐标AABB最小X", minX)
            || !私有_读取场景复现I64(节点, "范围坐标AABB最大X", maxX)
            || !私有_读取场景复现I64(节点, "范围坐标AABB最小Y", minY)
            || !私有_读取场景复现I64(节点, "范围坐标AABB最大Y", maxY)
            || !私有_读取场景复现I64(节点, "范围坐标AABB最小Z", minZ)
            || !私有_读取场景复现I64(节点, "范围坐标AABB最大Z", maxZ)) {
            return false;
        }
        if (maxX < minX || maxY < minY || maxZ < minZ) {
            return false;
        }
        项.AABB最小X = static_cast<std::int64_t>(minX);
        项.AABB最大X = static_cast<std::int64_t>(maxX);
        项.AABB最小Y = static_cast<std::int64_t>(minY);
        项.AABB最大Y = static_cast<std::int64_t>(maxY);
        项.AABB最小Z = static_cast<std::int64_t>(minZ);
        项.AABB最大Z = static_cast<std::int64_t>(maxZ);
        return true;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_填充自我场景存在复现项(
        const 基础信息节点类* 节点,
        const std::int64_t 来源范围,
        结构_控制面板自我场景存在复现项& 项) noexcept
    {
        if (!节点 || !dynamic_cast<const 存在节点主信息类*>(节点->主信息)) {
            return false;
        }

        项 = {};
        项.节点指针 = 私有_地址(节点);
        项.来源范围 = 来源范围;
        项.标题 = 私有_对象摘要(节点);
        if (const auto* 存在主信息 = 世界树.取存在主信息(reinterpret_cast<const 存在节点类*>(节点))) {
            项.类型 = 存在主信息->类型 ? 存在主信息->类型->获取主键() : std::string("存在");
        } else {
            项.类型 = "存在";
        }

        const auto 读 = [&](const char* 特征名, std::int64_t& 字段) noexcept {
            I64 值 = static_cast<I64>(字段);
            if (私有_读取场景复现I64_保留默认(节点, 特征名, 值)) {
                字段 = static_cast<std::int64_t>(值);
                return true;
            }
            return false;
        };

        const bool 有中心 = 私有_读取自我场景存在三元特征(
            节点,
            "中心空间坐标X",
            "中心空间坐标Y",
            "中心空间坐标Z",
            项.中心X,
            项.中心Y,
            项.中心Z);
        I64 绝对明确 = 0;
        const bool 有存在绝对坐标 = !有中心
            && 私有_读取场景复现I64(节点, "存在_场景绝对坐标明确状态", 绝对明确)
            && 绝对明确 > 0
            && 私有_读取自我场景存在三元特征(
                节点,
                "存在_场景绝对坐标X",
                "存在_场景绝对坐标Y",
                "存在_场景绝对坐标Z",
                项.中心X,
                项.中心Y,
                项.中心Z);
        I64 自我绝对明确 = 0;
        const bool 有自我绝对坐标 = !有中心
            && !有存在绝对坐标
            && 私有_读取场景复现I64(节点, "自我_场景绝对坐标明确状态", 自我绝对明确)
            && 自我绝对明确 > 0
            && 私有_读取自我场景存在三元特征(
                节点,
                "自我_场景绝对坐标X",
                "自我_场景绝对坐标Y",
                "自我_场景绝对坐标Z",
                项.中心X,
                项.中心Y,
                项.中心Z);
        bool 中心可用 = 有中心 || 有存在绝对坐标 || 有自我绝对坐标;

        bool AABB可用 = 私有_读取自我场景存在AABB特征(节点, 项);
        bool 尺寸可用 = 私有_读取自我场景存在三元特征(
            节点,
            "假设尺寸X",
            "假设尺寸Y",
            "假设尺寸Z",
            项.尺寸X,
            项.尺寸Y,
            项.尺寸Z);
        if (!尺寸可用 && AABB可用) {
            项.尺寸X = std::max<std::int64_t>(1, 项.AABB最大X - 项.AABB最小X);
            项.尺寸Y = std::max<std::int64_t>(1, 项.AABB最大Y - 项.AABB最小Y);
            项.尺寸Z = std::max<std::int64_t>(1, 项.AABB最大Z - 项.AABB最小Z);
            尺寸可用 = true;
        }
        if (!AABB可用 && 中心可用 && 尺寸可用) {
            const auto 推导范围 = [](std::int64_t 中心, std::int64_t 尺寸, bool 最小) noexcept {
                const auto 安全尺寸 = std::max<std::int64_t>(1, 尺寸);
                const auto 半 = 安全尺寸 / 2;
                return 最小 ? 中心 - 半 : 中心 + (安全尺寸 - 半);
            };
            项.AABB最小X = 推导范围(项.中心X, 项.尺寸X, true);
            项.AABB最大X = 推导范围(项.中心X, 项.尺寸X, false);
            项.AABB最小Y = 推导范围(项.中心Y, 项.尺寸Y, true);
            项.AABB最大Y = 推导范围(项.中心Y, 项.尺寸Y, false);
            项.AABB最小Z = 推导范围(项.中心Z, 项.尺寸Z, true);
            项.AABB最大Z = 推导范围(项.中心Z, 项.尺寸Z, false);
            AABB可用 = true;
        }
        if (!中心可用 && AABB可用) {
            项.中心X = (项.AABB最小X + 项.AABB最大X) / 2;
            项.中心Y = (项.AABB最小Y + 项.AABB最大Y) / 2;
            项.中心Z = (项.AABB最小Z + 项.AABB最大Z) / 2;
            中心可用 = true;
        }

        bool 投影可用 = true;
        投影可用 = 读("投影范围最小X", 项.投影最小X) && 投影可用;
        投影可用 = 读("投影范围最大X", 项.投影最大X) && 投影可用;
        投影可用 = 读("投影范围最小Y", 项.投影最小Y) && 投影可用;
        投影可用 = 读("投影范围最大Y", 项.投影最大Y) && 投影可用;
        if (项.投影最大X < 项.投影最小X || 项.投影最大Y < 项.投影最小Y) {
            投影可用 = false;
        }

        (void)读("来源空间候选编号", 项.来源空间候选编号);
        (void)读("外设观察报告ID", 项.外设观察报告ID);
        (void)读("外设观察像素簇ID", 项.外设观察像素簇ID);
        (void)读("观察存在确认状态", 项.观察存在确认状态);
        (void)读("像素归属验证状态", 项.像素归属验证状态);
        (void)读("可绘制状态", 项.可绘制状态);
        (void)读("颜色RGB结构状态", 项.颜色RGB结构状态);
        (void)读("像素颜色层", 项.像素颜色层状态);
        (void)读("颜色缓冲", 项.颜色缓冲状态);
        (void)读("彩图轮廓数量", 项.彩图轮廓数量);
        (void)读("轮廓颜色支持率", 项.轮廓颜色支持率);
        (void)读("内部世界局部轮廓材料可回查状态", 项.局部轮廓材料可回查状态);
        (void)读("平面轮廓状态", 项.平面轮廓状态);
        (void)读("空间极值轮廓状态", 项.空间极值轮廓状态);

        VecI64 轮廓{};
        if (私有_读取场景复现VecI64(节点, "平面轮廓", 轮廓)) {
            项.平面轮廓点数 = static_cast<std::int64_t>(轮廓.size() / 2);
            if (项.平面轮廓点数 > 0 && 项.平面轮廓状态 <= 0) {
                项.平面轮廓状态 = 1;
            }
        }
        if (私有_读取场景复现VecI64(节点, "空间极值轮廓", 轮廓)) {
            项.空间极值轮廓点数 = static_cast<std::int64_t>(轮廓.size() / 3);
            if (项.空间极值轮廓点数 > 0 && 项.空间极值轮廓状态 <= 0) {
                项.空间极值轮廓状态 = 1;
            }
        }

        if (中心可用) 项.几何状态 |= 1;
        if (AABB可用) 项.几何状态 |= 2;
        if (尺寸可用) 项.几何状态 |= 4;
        if (投影可用) 项.几何状态 |= 8;
        if (项.颜色RGB结构状态 > 0
            || 项.像素颜色层状态 > 0
            || 项.颜色缓冲状态 > 0
            || 项.彩图轮廓数量 > 0
            || 项.轮廓颜色支持率 > 0) {
            项.几何状态 |= 16;
        }
        if (项.局部轮廓材料可回查状态 > 0) 项.几何状态 |= 32;
        if (项.平面轮廓状态 > 0 || 项.平面轮廓点数 > 0) 项.几何状态 |= 64;
        if (项.空间极值轮廓状态 > 0 || 项.空间极值轮廓点数 > 0) 项.几何状态 |= 128;
        return true;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_收集自我场景存在复现项(
        const 基础信息节点类* 根,
        const std::int64_t 来源范围,
        结构_控制面板快照& 快照,
        路径集合& 已访问) noexcept
    {
        if (!根 || 快照.自我场景存在复现项列表.size() >= 1024) {
            return;
        }

        std::vector<const 基础信息节点类*> 栈{};
        栈.push_back(根);
        std::size_t 已扫描 = 0;
        while (!栈.empty()
            && 已扫描 < 结构_自我场景内容统计::扫描上限
            && 快照.自我场景存在复现项列表.size() < 1024) {
            const auto* 当前 = 栈.back();
            栈.pop_back();
            if (!当前) {
                continue;
            }
            const auto 地址 = 私有_地址(当前);
            if (!已访问.insert(地址).second) {
                continue;
            }
            ++已扫描;

            if (dynamic_cast<const 存在节点主信息类*>(当前->主信息)) {
                结构_控制面板自我场景存在复现项 项{};
                if (私有_填充自我场景存在复现项(当前, 来源范围, 项)) {
                    if (项.几何状态 & 1) {
                        ++快照.自我场景真实几何存在数量;
                    }
                    if (项.几何状态 & 1) {
                        ++快照.自我场景真实可绘制存在数量;
                    }
                    if (项.几何状态 & 16) {
                        ++快照.自我场景真实颜色状态存在数量;
                    }
                    if (项.局部轮廓材料可回查状态 > 0) {
                        ++快照.自我场景真实彩图材料可回查存在数量;
                    }
                    快照.自我场景存在复现项列表.push_back(std::move(项));
                }
            }

            for (auto* 子节点 : 私有_枚举子节点(当前, (std::numeric_limits<std::size_t>::max)())) {
                if (子节点
                    && (dynamic_cast<const 存在节点主信息类*>(子节点->主信息)
                        || dynamic_cast<const 场景节点主信息类*>(子节点->主信息))) {
                    栈.push_back(子节点);
                }
            }
        }
    }

    // 功能：从指定来源读取数据或状态。
    void 私有_读取自我场景存在复现项列表(
        场景节点类* 自我所在场景,
        基础信息节点类* 复现宿主,
        结构_控制面板快照& 快照) noexcept
    {
        快照.自我场景存在复现项列表.clear();
        快照.自我场景真实复现存在数量 = 0;
        快照.自我场景真实几何存在数量 = 0;
        快照.自我场景真实可绘制存在数量 = 0;
        快照.自我场景真实颜色状态存在数量 = 0;
        快照.自我场景真实彩图材料可回查存在数量 = 0;

        路径集合 已访问{};
        私有_收集自我场景存在复现项(
            reinterpret_cast<const 基础信息节点类*>(自我所在场景),
            1,
            快照,
            已访问);
        私有_收集自我场景存在复现项(
            复现宿主,
            2,
            快照,
            已访问);

        std::sort(
            快照.自我场景存在复现项列表.begin(),
            快照.自我场景存在复现项列表.end(),
            [](const auto& a, const auto& b) noexcept {
                const bool a有范围 = (a.几何状态 & 1) != 0
                    && ((a.几何状态 & 2) != 0 || (a.几何状态 & 4) != 0);
                const bool b有范围 = (b.几何状态 & 1) != 0
                    && ((b.几何状态 & 2) != 0 || (b.几何状态 & 4) != 0);
                if (a有范围 != b有范围) return a有范围 > b有范围;
                const bool a可绘制 = (a.几何状态 & 1) != 0;
                const bool b可绘制 = (b.几何状态 & 1) != 0;
                if (a可绘制 != b可绘制) return a可绘制 > b可绘制;
                if (a.观察存在确认状态 != b.观察存在确认状态) {
                    return a.观察存在确认状态 > b.观察存在确认状态;
                }
                if (a.来源空间候选编号 != b.来源空间候选编号) {
                    return a.来源空间候选编号 < b.来源空间候选编号;
                }
                return a.节点指针 < b.节点指针;
            });
        快照.自我场景真实复现存在数量 =
            static_cast<std::int64_t>(快照.自我场景存在复现项列表.size());
    }

    // 功能：从指定来源读取数据或状态。
    void 私有_读取自我场景复现快照(结构_控制面板快照& 快照) noexcept
    {
        auto* 自我所在场景 = 自我.获取自我现实场景();
        快照.自我所在场景指针 = 私有_地址(自我所在场景);
        快照.自我所在场景标题 = 自我所在场景
            ? 私有_安全节点摘要(自我所在场景, "自我所在场景")
            : std::string("空");

        auto* 宿主 = 私有_选择自我场景复现宿主(自我所在场景);
        快照.自我场景复现宿主指针 = 私有_地址(宿主);
        快照.自我场景复现宿主标题 = 宿主
            ? 私有_安全节点摘要(宿主, "复现宿主")
            : std::string("空");
        auto* 安全宿主 = 私有_选择自我场景安全评估宿主(自我所在场景);
        快照.自我场景安全评估宿主指针 = 私有_地址(安全宿主);
        快照.自我场景安全评估宿主标题 = 安全宿主
            ? 私有_安全节点摘要(安全宿主, "安全评估宿主")
            : std::string("空");
        私有_读取自我场景内容统计(快照, 自我所在场景, 宿主);
        私有_读取自我场景存在复现项列表(自我所在场景, 宿主, 快照);
        if (!宿主) {
            return;
        }

        bool 有快照 = false;
        const auto 读 = [&](const char* 特征名, I64& 字段) noexcept {
            const bool 命中 = 私有_读取场景复现I64_保留默认(宿主, 特征名, 字段);
            有快照 = 命中 || 有快照;
        };
        const auto 读安全 = [&](const char* 特征名, I64& 字段) noexcept {
            (void)私有_读取场景复现I64_保留默认(安全宿主, 特征名, 字段);
        };

        读("当前观察帧", 快照.自我场景当前观察帧);
        读("相机帧宽度", 快照.自我场景相机帧宽度);
        读("相机帧高度", 快照.自我场景相机帧高度);
        读("深度帧号", 快照.自我场景深度帧号);
        读("彩色帧号", 快照.自我场景彩色帧号);
        读("预期像素数量", 快照.自我场景预期像素数量);
        读("像素特征数量", 快照.自我场景像素特征数量);
        读("深度有效像素数量", 快照.自我场景深度有效像素数量);
        读("点云有效像素数量", 快照.自我场景点云有效像素数量);
        读("像素覆盖状态", 快照.自我场景像素覆盖状态);
        读("空间坐标单位毫米", 快照.自我场景空间坐标单位毫米);
        读("颜色RGB结构状态", 快照.自我场景颜色RGB结构状态);
        读("原始深度毫米结构状态", 快照.自我场景原始深度毫米结构状态);
        读("滤波深度毫米结构状态", 快照.自我场景滤波深度毫米结构状态);
        读("补全深度毫米结构状态", 快照.自我场景补全深度毫米结构状态);
        读("深度有效性Mask状态", 快照.自我场景深度有效性Mask状态);
        读("深度来源结构状态", 快照.自我场景深度来源结构状态);
        读("深度稳定性结构状态", 快照.自我场景深度稳定性结构状态);
        读("深度邻域一致性结构状态", 快照.自我场景深度邻域一致性结构状态);
        读("融合深度毫米结构状态", 快照.自我场景融合深度毫米结构状态);
        读("融合深度有效性结构状态", 快照.自我场景融合深度有效性结构状态);
        读("多帧深度稳定性结构状态", 快照.自我场景多帧深度稳定性结构状态);
        读("每像素深度方差结构状态", 快照.自我场景每像素深度方差结构状态);
        读("空间坐标毫米XYZ结构状态", 快照.自我场景空间坐标毫米XYZ结构状态);
        读("原始深度来源像素数量", 快照.自我场景原始深度来源像素数量);
        读("滤波深度来源像素数量", 快照.自我场景滤波深度来源像素数量);
        读("补全深度来源像素数量", 快照.自我场景补全深度来源像素数量);
        读("无有效深度来源像素数量", 快照.自我场景无有效深度来源像素数量);
        读("补全深度低置信像素数量", 快照.自我场景补全深度低置信像素数量);
        读("深度稳定性平均值", 快照.自我场景深度稳定性平均值);
        读("深度邻域一致性平均值", 快照.自我场景深度邻域一致性平均值);
        读("观察帧组状态", 快照.自我场景观察帧组状态);
        读("观察帧组帧数", 快照.自我场景观察帧组帧数);
        读("融合依据帧集合状态", 快照.自我场景融合依据帧集合状态);
        读("融合深度有效像素数量", 快照.自我场景融合深度有效像素数量);
        读("融合深度有效率", 快照.自我场景融合深度有效率);
        读("融合补偿深度空洞像素数量", 快照.自我场景融合补偿深度空洞像素数量);
        读("融合低稳定像素数量", 快照.自我场景融合低稳定像素数量);
        读("融合深度稳定性平均值", 快照.自我场景融合深度稳定性平均值);
        读("融合深度平均方差", 快照.自我场景融合深度平均方差);
        读("彩色深度已对齐", 快照.自我场景彩色深度已对齐);
        读("深度有效率", 快照.自我场景深度有效率);
        读("空间坐标有效率", 快照.自我场景空间坐标有效率);
        读("帧质量评分", 快照.自我场景帧质量评分);
        读("深度空洞数量", 快照.自我场景深度空洞数量);
        读("深度空洞区域数量", 快照.自我场景深度空洞区域数量);
        读("轮廓闭合率", 快照.自我场景轮廓闭合率);
        读("轮廓断裂数量", 快照.自我场景轮廓断裂数量);
        读("彩图轮廓数量", 快照.自我场景彩图轮廓数量);
        读("深度轮廓数量", 快照.自我场景深度轮廓数量);
        读("空间投影轮廓数量", 快照.自我场景空间投影轮廓数量);
        读("融合轮廓数量", 快照.自我场景融合轮廓数量);
        读("轮廓来源分层状态", 快照.自我场景轮廓来源分层状态);
        读("轮廓深度支持率", 快照.自我场景轮廓深度支持率);
        读("轮廓颜色支持率", 快照.自我场景轮廓颜色支持率);
        读("轮廓空间支持率", 快照.自我场景轮廓空间支持率);
        读("融合轮廓置信度", 快照.自我场景融合轮廓置信度);
        读("跨越深度断裂轮廓数量", 快照.自我场景跨越深度断裂轮廓数量);
        读("穿过深度无效区域轮廓数量", 快照.自我场景穿过深度无效区域轮廓数量);
        读("依赖补全深度轮廓数量", 快照.自我场景依赖补全深度轮廓数量);
        读("边界深度稳定率", 快照.自我场景边界深度稳定率);
        读("候选存在验证通过率", 快照.自我场景候选存在验证通过率);
        读("未解释区域比例", 快照.自我场景未解释区域比例);
        读("补观察缺口状态", 快照.自我场景补观察缺口状态);
        读("补观察缺失原因", 快照.自我场景补观察缺失原因);
        读("待补观察区域数量", 快照.自我场景待补观察区域数量);
        读("补全候选数量", 快照.自我场景补全候选数量);
        读("低置信轮廓数量", 快照.自我场景低置信轮廓数量);
        读("条件不足候选数量", 快照.自我场景条件不足候选数量);
        读("待验证候选数量", 快照.自我场景待验证候选数量);
        读("部分确认候选数量", 快照.自我场景部分确认候选数量);
        读("补观察需求建议", 快照.自我场景补观察需求建议);
        读("诊断区域数量", 快照.自我场景诊断区域数量);
        读("诊断区域集合状态", 快照.自我场景诊断区域集合状态);
        读("诊断区域掩码状态", 快照.自我场景诊断区域掩码状态);
        私有_读取自我场景诊断区域列表(宿主, 快照);
        读("空间候选数量", 快照.自我场景空间候选数量);
        读("空间候选有效点数量", 快照.自我场景空间候选有效点数量);
        读("主空间候选编号", 快照.自我场景主空间候选编号);
        读("主空间候选像素数量", 快照.自我场景主空间候选像素数量);
        读("中心空间坐标X", 快照.自我场景中心空间坐标X);
        读("中心空间坐标Y", 快照.自我场景中心空间坐标Y);
        读("中心空间坐标Z", 快照.自我场景中心空间坐标Z);
        读("范围坐标AABB最小X", 快照.自我场景范围坐标AABB最小X);
        读("范围坐标AABB最大X", 快照.自我场景范围坐标AABB最大X);
        读("范围坐标AABB最小Y", 快照.自我场景范围坐标AABB最小Y);
        读("范围坐标AABB最大Y", 快照.自我场景范围坐标AABB最大Y);
        读("范围坐标AABB最小Z", 快照.自我场景范围坐标AABB最小Z);
        读("范围坐标AABB最大Z", 快照.自我场景范围坐标AABB最大Z);
        读("空间连续性评分", 快照.自我场景空间连续性评分);
        读("范围稳定性评分", 快照.自我场景范围稳定性评分);
        读("观察存在假设", 快照.自我场景观察存在假设);
        读("来源空间候选编号", 快照.自我场景来源空间候选编号);
        读("存在假设验证状态", 快照.自我场景存在假设验证状态);
        读("假设距离", 快照.自我场景假设距离);
        读("假设尺寸X", 快照.自我场景假设尺寸X);
        读("假设尺寸Y", 快照.自我场景假设尺寸Y);
        读("假设尺寸Z", 快照.自我场景假设尺寸Z);
        读("假设有效点比例", 快照.自我场景假设有效点比例);
        读("像素归属验证状态", 快照.自我场景像素归属验证状态);
        读("全帧像素归属账状态", 快照.自我场景全帧像素归属账状态);
        读("已归属像素数", 快照.自我场景已归属像素数);
        读("未解释像素数", 快照.自我场景未解释像素数);
        读("归属冲突像素数", 快照.自我场景归属冲突像素数);
        读("像素归属率", 快照.自我场景像素归属率);
        读("深度一致率", 快照.自我场景深度一致率);
        读("范围内点比例", 快照.自我场景范围内点比例);
        读("投影覆盖率", 快照.自我场景投影覆盖率);
        读("轮廓吻合率", 快照.自我场景轮廓吻合率);
        读("已验证观察存在", 快照.自我场景已验证观察存在);
        读("观察存在确认状态", 快照.自我场景观察存在确认状态);
        读("已验证观察存在数量", 快照.自我场景已验证观察存在数量);
        读("帧解释状态", 快照.自我场景帧解释状态);
        读安全("基础观察事实可用状态", 快照.自我场景基础观察事实可用状态);
        读安全("基础风险判断状态", 快照.自我场景基础风险判断状态);
        读安全("风险状态明确状态", 快照.自我场景风险状态明确状态);
        读安全("基础风险值计算状态", 快照.自我场景基础风险值计算状态);
        读安全("当前场景评估安全值候选", 快照.自我场景当前场景评估安全值候选);
        读安全("当前场景安全度候选", 快照.自我场景当前场景安全度候选);
        读安全("当前场景安全判定状态", 快照.自我场景当前场景安全判定状态);
        读安全("安全评估证据不足原因", 快照.自我场景安全评估证据不足原因);
        读安全("风险安全_场景影响部分候选", 快照.自我场景风险安全_场景影响部分候选);
        读安全("风险安全_场景影响部分可结算状态", 快照.自我场景风险安全_场景影响部分可结算状态);
        读安全("风险安全_场景影响部分不可结算原因", 快照.自我场景风险安全_场景影响部分不可结算原因);
        读安全("风险安全_场景影响部分已入账状态", 快照.自我场景风险安全_场景影响部分已入账状态);
        读安全("风险安全层候选", 快照.自我场景风险安全层候选);
        读安全("风险安全层投影候选", 快照.自我场景风险安全层投影候选);
        读安全("风险安全层缺失因素数量", 快照.自我场景风险安全层缺失因素数量);
        读安全("风险安全层明确状态", 快照.自我场景风险安全层明确状态);
        读安全("风险安全层可聚合状态", 快照.自我场景风险安全层可聚合状态);
        读安全("风险安全层已入账状态", 快照.自我场景风险安全层已入账状态);
        读安全("风险因素未搜索掩码", 快照.自我场景风险因素未搜索掩码);
        读安全("风险因素搜索缺口掩码", 快照.自我场景风险因素搜索缺口掩码);
        读安全("风险因素负向证据掩码", 快照.自我场景风险因素负向证据掩码);
        读安全("风险因素默认满足掩码", 快照.自我场景风险因素默认满足掩码);
        读安全("风险因素已入账掩码", 快照.自我场景风险因素已入账掩码);
        读安全("风险因素证据搜索未完成数量", 快照.自我场景风险因素证据搜索未完成数量);
        读安全("风险因素默认满足数量", 快照.自我场景风险因素默认满足数量);

        快照.自我场景复现有快照 = 有快照
            && (快照.自我场景当前观察帧 != 0
                || 快照.自我场景相机帧宽度 > 0
                || 快照.自我场景空间候选数量 > 0
                || 快照.自我场景诊断区域数量 > 0
                || 快照.自我场景观察存在假设 != 0
                || 快照.自我场景已验证观察存在数量 > 0
                || 快照.自我场景真实复现存在数量 > 0);
    }

    template<>
    std::string 私有_对象摘要<需求节点>(const 需求节点* 节点)
    {
        return 私有_需求标题(节点);
    }

    template<>
    std::string 私有_对象摘要<任务节点>(const 任务节点* 节点)
    {
        return 私有_任务标题(节点);
    }

    template<>
    std::string 私有_对象摘要<方法节点>(const 方法节点* 节点)
    {
        return 私有_方法标题(节点);
    }

    template<class T节点>
    std::string 私有_引用主键文本(T节点* 指针, const std::string& 主键 = {})
    {
        using 节点类型 = 私有_规范节点类型_t<T节点>;
        if (!主键.empty()) {
            return 主键;
        }

        const auto* 当前树节点 = 私有_解析当前树节点(static_cast<const T节点*>(指针), 主键);
        const auto* 规范当前树节点 = 私有_转为规范节点(当前树节点);
        if constexpr (std::is_same_v<节点类型, 需求节点>) {
            if (规范当前树节点) {
                return 私有_节点主键_控制面板(规范当前树节点);
            }
        }
        else if constexpr (std::is_same_v<节点类型, 任务节点>) {
            if (规范当前树节点) {
                return 私有_任务节点主键_控制面板(规范当前树节点);
            }
        }
        else if constexpr (requires(const 节点类型* 值) { 值->获取主键(); }) {
            if (规范当前树节点) {
                return 规范当前树节点->获取主键();
            }
        }

        if (指针) {
            return "未解析主键";
        }
        return "空";
    }

    template<class T节点>
    std::string 私有_引用文本(T节点* 指针, const std::string& 主键 = {})
    {
        using 节点类型 = 私有_规范节点类型_t<T节点>;
        const auto* 当前树节点 = 私有_解析当前树节点(static_cast<const T节点*>(指针), 主键);
        const auto* 规范当前树节点 = 私有_转为规范节点(当前树节点);
        if (!当前树节点 && !指针 && 主键.empty()) {
            return "空";
        }

        std::ostringstream 输出;
        if (规范当前树节点) {
            输出 << 私有_对象摘要(规范当前树节点);
        }
        else if (!主键.empty()) {
            输出 << "主键=" << 主键;
        }
        else {
            输出 << "引用已失效";
        }

        bool 主键已体现在对象摘要中 = false;
        if constexpr (requires(const 节点类型* 值) { 值->获取主键(); }) {
            主键已体现在对象摘要中 = 规范当前树节点 && 主键 == 规范当前树节点->获取主键();
        }

        if (!主键.empty() && !主键已体现在对象摘要中) {
            输出 << " | 主键=" << 主键;
        }
        if (指针 && 当前树节点 && 当前树节点 != 指针) {
            输出 << " | 原地址=" << 私有_十六进制指针(私有_地址(指针));
            输出 << " | 当前地址=" << 私有_十六进制指针(私有_地址(当前树节点));
            输出 << " | 指针状态=已按主键重定位";
        }
        else if (当前树节点) {
            输出 << " | 地址=" << 私有_十六进制指针(私有_地址(当前树节点));
        }
        else if (指针) {
            输出 << " | 地址=" << 私有_十六进制指针(私有_地址(指针));
            输出 << " | 指针状态=不在当前树";
        }
        return 输出.str();
    }

    template<class T节点>
    constexpr std::string_view 私有_引用展开类型() noexcept
    {
        using 节点类型 = std::remove_cv_t<T节点>;
        if constexpr (std::is_same_v<节点类型, 基础信息节点类>) {
            return "base-ref";
        }
        else if constexpr (std::is_same_v<节点类型, 需求节点> || std::is_same_v<节点类型, 需求节点类>) {
            return "need-ref";
        }
        else if constexpr (std::is_same_v<节点类型, 任务节点> || std::is_same_v<节点类型, 任务节点类>) {
            return "task-ref";
        }
        else if constexpr (std::is_same_v<节点类型, 方法节点> || std::is_same_v<节点类型, 方法节点类>) {
            return "method-ref";
        }
        else {
            return "";
        }
    }

    template<class T节点>
    constexpr std::string_view 私有_引用字段类型文本() noexcept
    {
        using 节点类型 = std::remove_cv_t<T节点>;
        if constexpr (std::is_same_v<节点类型, 基础信息节点类>) {
            return "基础信息指针";
        }
        else if constexpr (std::is_same_v<节点类型, 需求节点> || std::is_same_v<节点类型, 需求节点类>) {
            return "需求指针";
        }
        else if constexpr (std::is_same_v<节点类型, 任务节点> || std::is_same_v<节点类型, 任务节点类>) {
            return "任务指针";
        }
        else if constexpr (std::is_same_v<节点类型, 方法节点> || std::is_same_v<节点类型, 方法节点类>) {
            return "方法指针";
        }
        else {
            return "引用";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_字段显示文本(
        const std::string& 名称,
        std::string_view 类型,
        const std::string& 值)
    {
        return 名称 + " | 类型=" + std::string(类型) + " | 值=" + (值.empty() ? "空" : 值);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_等号字段显示文本(
        const std::string& 名称,
        std::string_view 类型,
        const std::string& 值)
    {
        return 名称 + "=" + (值.empty() ? "空" : 值) + " | 数据类型=" + std::string(类型);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_等号字段显示文本(
        const std::string& 名称,
        const std::string& 值)
    {
        return 私有_等号字段显示文本(名称, "文本", 值);
    }

    结构_控制面板树节点 私有_新节点(
        std::string 文本,
        const std::uintptr_t 节点指针 = 0,
        const bool 默认展开 = false,
        const bool 可延迟展开 = false,
        std::string 展开类型 = {},
        const std::uintptr_t 附加参数 = 0)
    {
        结构_控制面板树节点 节点{};
        节点.节点指针 = 节点指针;
        节点.附加参数 = 附加参数;
        节点.文本 = std::move(文本);
        节点.默认展开 = 默认展开;
        节点.可延迟展开 = 可延迟展开;
        节点.展开类型 = std::move(展开类型);
        return 节点;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const std::string& 值)
    {
        父节点.子项.push_back(私有_新节点(私有_字段显示文本(名称, "文本", 值)));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const char* 值)
    {
        私有_追加叶字段(父节点, 名称, 值 ? std::string(值) : std::string{});
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const std::int64_t 值)
    {
        父节点.子项.push_back(私有_新节点(私有_字段显示文本(名称, "i64", std::to_string(值))));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const std::uint64_t 值)
    {
        父节点.子项.push_back(私有_新节点(私有_字段显示文本(名称, "u64", std::to_string(值))));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const bool 值)
    {
        父节点.子项.push_back(私有_新节点(私有_字段显示文本(名称, "bool", 布尔文本_是或否(值))));
    }

    template<class T>
    requires (std::is_arithmetic_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>)
    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const T 值)
    {
        父节点.子项.push_back(私有_新节点(私有_字段显示文本(名称, "数值", std::to_string(值))));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加统计字段(结构_控制面板树节点& 父节点, const 结构_统计& 统计)
    {
        auto 统计节点 = 私有_新节点("统计");
        私有_追加叶字段(统计节点, "创建时间", 私有_时间文本(统计.创建时间));
        私有_追加叶字段(统计节点, "最后观测时间", 私有_时间文本(统计.最后观测时间));
        私有_追加叶字段(统计节点, "命中次数", 统计.命中次数);
        父节点.子项.push_back(std::move(统计节点));
    }

    template<class T节点, class T构建器>
    void 私有_追加指针字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        T节点* 指针,
        const std::string& 主键,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径,
        T构建器&& 构建器)
    {
        (void)上下文;
        (void)剩余深度;
        (void)构建器;

        const auto 展开类型 = std::string(私有_引用展开类型<T节点>());
        auto* 可展开指针 = 私有_解析当前树节点_可写(指针, 主键);
        auto 字段节点 = 私有_新节点(
            私有_字段显示文本(名称, 私有_引用字段类型文本<T节点>(), 私有_引用文本(指针, 主键)),
            私有_地址(可展开指针),
            false,
            false,
            展开类型);
        (void)路径;
        父节点.子项.push_back(std::move(字段节点));
    }

    template<class T节点, class T构建器>
    void 私有_追加引用字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const 可解析引用<T节点>& 引用,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径,
        T构建器&& 构建器)
    {
        私有_追加指针字段(
            父节点,
            名称,
            引用.指针,
            引用.主键,
            上下文,
            剩余深度,
            路径,
            std::forward<T构建器>(构建器));
    }

    template<class T节点, class T构建器>
    void 私有_追加引用列表字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const std::vector<可解析引用<T节点>>& 列表,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径,
        T构建器&& 构建器,
        const std::size_t 上限)
    {
        auto 列表节点 = 私有_新节点(名称 + " (" + std::to_string(列表.size()) + ")");
        const auto 实际上限 = (std::min)(上限, 列表.size());
        for (std::size_t 索引 = 0; 索引 < 实际上限; ++索引) {
            私有_追加引用字段(
                列表节点,
                std::to_string(索引),
                列表[索引],
                上下文,
                剩余深度,
                路径,
                std::forward<T构建器>(构建器));
        }
        if (列表.size() > 实际上限) {
            列表节点.子项.push_back(
                私有_新节点("... 省略 " + std::to_string(列表.size() - 实际上限) + " 项"));
        }
        父节点.子项.push_back(std::move(列表节点));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加等号字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const std::string& 值)
    {
        父节点.子项.push_back(私有_新节点(私有_等号字段显示文本(名称, "文本", 值)));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加等号字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const char* 值)
    {
        私有_追加等号字段(父节点, 名称, 值 ? std::string(值) : std::string{});
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加等号字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const bool 值)
    {
        父节点.子项.push_back(私有_新节点(私有_等号字段显示文本(名称, "bool", 布尔文本_是或否(值))));
    }

    template<class T>
    requires (std::is_arithmetic_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>)
    void 私有_追加等号字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const T 值)
    {
        constexpr auto 类型 = std::is_integral_v<T>
            ? (std::is_signed_v<T> ? std::string_view{"i64"} : std::string_view{"u64"})
            : std::string_view{"数值"};
        父节点.子项.push_back(私有_新节点(私有_等号字段显示文本(名称, 类型, std::to_string(值))));
    }

    template<class T节点>
    void 私有_追加等号指针字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        T节点* 指针,
        const std::string& 主键,
        const 路径集合& 路径)
    {
        const auto 展开类型 = std::string(私有_引用展开类型<T节点>());
        auto* 可展开指针 = 私有_解析当前树节点_可写(指针, 主键);
        auto 字段节点 = 私有_新节点(
            私有_等号字段显示文本(名称, 私有_引用字段类型文本<T节点>(), 私有_引用文本(指针, 主键)),
            私有_地址(可展开指针),
            false,
            false,
            展开类型);
        (void)路径;

        父节点.子项.push_back(std::move(字段节点));
    }

    template<class T节点>
    void 私有_追加等号引用字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const 可解析引用<T节点>& 引用,
        const 路径集合& 路径)
    {
        私有_追加等号指针字段(父节点, 名称, 引用.指针, 引用.主键, 路径);
    }

    template<class T节点>
    std::vector<T节点*> 私有_枚举子节点(const T节点* 父节点, const std::size_t 上限)
    {
        std::vector<T节点*> 结果{};
        if constexpr (std::is_same_v<std::remove_cv_t<T节点>, 基础信息节点类>) {
            std::size_t 已收集 = 0;
            for (auto* 子节点 : 世界树.获取子节点(父节点)) {
                if (!子节点) {
                    continue;
                }
                结果.push_back(static_cast<T节点*>(子节点));
                ++已收集;
                if (已收集 >= 上限) {
                    break;
                }
            }
            return 结果;
        }

        if (!父节点 || 父节点->子节点数量 <= 0 || !父节点->子) {
            if constexpr (std::is_same_v<std::remove_cv_t<T节点>, 需求节点>) {
                私有_记录需求节点子链数量不一致_控制面板(父节点);
            }
            return 结果;
        }

        auto* 首节点 = reinterpret_cast<T节点*>(父节点->子);
        auto* 当前 = 首节点;
        std::size_t 已收集 = 0;
        do {
            结果.push_back(当前);
            ++已收集;
            当前 = reinterpret_cast<T节点*>(当前->下);
        } while (当前 && 当前 != 首节点 && 已收集 < 上限);
        return 结果;
    }

    template<class T节点, class T回调>
    void 私有_遍历全部子节点(const T节点* 父节点, T回调&& 回调)
    {
        if constexpr (std::is_same_v<std::remove_cv_t<T节点>, 基础信息节点类>) {
            for (auto* 子节点 : 世界树.获取子节点(父节点)) {
                if (子节点) {
                    回调(static_cast<T节点*>(子节点));
                }
            }
            return;
        }

        if (!父节点 || 父节点->子节点数量 <= 0 || !父节点->子) {
            if constexpr (std::is_same_v<std::remove_cv_t<T节点>, 需求节点>) {
                私有_记录需求节点子链数量不一致_控制面板(父节点);
            }
            return;
        }

        auto* 当前 = reinterpret_cast<T节点*>(父节点->子);
        路径集合 已访问{};
        while (当前) {
            const auto 地址 = 私有_地址(当前);
            if (!已访问.insert(地址).second) {
                break;
            }
            回调(当前);
            当前 = reinterpret_cast<T节点*>(当前->下);
        }
    }

    template<class T节点, class T回调>
    void 私有_遍历子树节点(const T节点* 父节点, T回调& 回调)
    {
        私有_遍历全部子节点(父节点, [&](const T节点* 子节点) {
            回调(子节点);
            私有_遍历子树节点(子节点, 回调);
        });
    }

    template<class T节点>
    std::vector<T节点*> 私有_枚举全部子节点(const T节点* 父节点)
    {
        std::vector<T节点*> 结果{};
        私有_遍历全部子节点(父节点, [&](T节点* 子节点) {
            结果.push_back(子节点);
        });
        return 结果;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::vector<任务节点*> 私有_筛选需求任务链节点(
        const std::vector<任务节点*>& 节点集)
    {
        std::vector<任务节点*> 输出{};
        for (auto* 节点 : 节点集) {
            if (私有_任务节点可在普通任务树显示(节点)) {
                输出.push_back(节点);
            }
        }
        return 输出;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_累计文本计数(
        std::vector<std::pair<std::string, std::size_t>>& 统计,
        std::string 键)
    {
        if (键.empty()) {
            键 = "空";
        }

        for (auto& [已有键, 数量] : 统计) {
            if (已有键 == 键) {
                ++数量;
                return;
            }
        }
        统计.emplace_back(std::move(键), 1);
    }

    std::string 私有_文本计数摘要(
        std::vector<std::pair<std::string, std::size_t>> 统计,
        const std::size_t 展示上限 = 4)
    {
        if (统计.empty()) {
            return {};
        }

        std::sort(
            统计.begin(),
            统计.end(),
            [](const auto& 左, const auto& 右) {
                if (左.second != 右.second) {
                    return 左.second > 右.second;
                }
                return 左.first < 右.first;
            });

        std::ostringstream 输出;
        std::size_t 已写入 = 0;
        std::size_t 其余数量 = 0;
        for (const auto& [键, 数量] : 统计) {
            if (已写入 < 展示上限) {
                if (已写入 > 0) {
                    输出 << "/";
                }
                输出 << 键 << 数量;
                ++已写入;
            }
            else {
                其余数量 += 数量;
            }
        }
        if (其余数量 > 0) {
            输出 << "/其余" << 其余数量;
        }
        return 输出.str();
    }

    // 功能：创建并返回或登记对应对象。
    结构_控制面板树节点 私有_创建结构省略节点(
        const std::string& 展开类型,
        const std::uintptr_t 父节点指针,
        const std::size_t 起始偏移,
        const std::size_t 剩余数量)
    {
        return 私有_新节点(
            "... 省略 " + std::to_string(剩余数量) + " 个结构子节点",
            父节点指针,
            false,
            true,
            展开类型,
            static_cast<std::uintptr_t>(起始偏移));
    }

    template<class T节点>
    std::size_t 私有_计数子树节点(const T节点* 根节点)
    {
        if (!根节点) {
            return 0;
        }

        std::size_t 总数 = 1;
        if constexpr (std::is_same_v<std::remove_cv_t<T节点>, 基础信息节点类>) {
            for (auto* 子节点 : 世界树.获取子节点(根节点)) {
                总数 += 私有_计数子树节点(static_cast<const T节点*>(子节点));
            }
            return 总数;
        }

        if (根节点->子节点数量 <= 0 || !根节点->子) {
            if constexpr (std::is_same_v<std::remove_cv_t<T节点>, 需求节点>) {
                私有_记录需求节点子链数量不一致_控制面板(根节点);
            }
            return 总数;
        }

        auto* 首节点 = reinterpret_cast<const T节点*>(根节点->子);
        auto* 当前 = 首节点;
        do {
            总数 += 私有_计数子树节点(当前);
            当前 = reinterpret_cast<const T节点*>(当前->下);
        } while (当前 && 当前 != 首节点);
        return 总数;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_基础信息类别文本(const 基础信息基类* 主信息)
    {
        if (!主信息) {
            return "空";
        }
        if (dynamic_cast<const 抽象特征主信息类*>(主信息)) return "抽象特征";
        if (dynamic_cast<const 特征节点主信息类*>(主信息)) return "特征";
        if (dynamic_cast<const 存在节点主信息类*>(主信息)) return "存在";
        if (dynamic_cast<const 场景节点主信息类*>(主信息)) return "场景";
        if (dynamic_cast<const 状态节点主信息类*>(主信息)) return "状态";
        if (dynamic_cast<const 动态节点主信息类*>(主信息)) return "动态";
        if (dynamic_cast<const 二次特征主信息类*>(主信息)) return "二次特征";
        if (dynamic_cast<const 因果主信息类*>(主信息)) return "因果";
        if (dynamic_cast<const 指代节点主信息类*>(主信息)) return "指代";
        return 私有_主信息类型文本(主信息->主信息类型);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_基础信息子层摘要(const 基础信息节点类* 节点)
    {
        std::vector<std::pair<std::string, std::size_t>> 统计{};
        私有_遍历全部子节点(节点, [&](const 基础信息节点类* 子节点) {
            私有_累计文本计数(统计, 私有_基础信息类别文本(子节点 ? 子节点->主信息 : nullptr));
        });
        return 私有_文本计数摘要(std::move(统计));
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求子层摘要(const 需求节点* 节点)
    {
        std::vector<std::pair<std::string, std::size_t>> 统计{};
        私有_遍历全部子节点(节点, [&](const 需求节点* 子节点) {
            auto 标签 = 私有_需求显示名_控制面板(子节点);
            if (标签.empty()) {
                标签 = "空目标特征";
            }
            私有_累计文本计数(统计, std::move(标签));
        });
        return 私有_文本计数摘要(std::move(统计));
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_任务子层摘要(const 任务节点* 节点)
    {
        std::vector<std::pair<std::string, std::size_t>> 统计{};
        私有_遍历全部子节点(节点, [&](const 任务节点* 子节点) {
            私有_累计文本计数(统计, 私有_任务节点种类文本(任务类::读取任务节点种类(子节点)));
        });
        return 私有_文本计数摘要(std::move(统计));
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_方法子层摘要(const 方法节点* 节点)
    {
        std::vector<std::pair<std::string, std::size_t>> 统计{};
        私有_遍历全部子节点(节点, [&](const 方法节点* 子节点) {
            私有_累计文本计数(统计, 私有_方法节点种类文本(方法类::方法节点种类(子节点)));
        });
        return 私有_文本计数摘要(std::move(统计));
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_基础信息标题(const 基础信息节点类* 节点, const 结构_构建上下文& 上下文)
    {
        if (!节点) {
            return "空节点";
        }

        const auto* 主信息 = 节点->主信息;
        if (!主信息 && !节点->父) {
            return "基础信息仓库根 | 主信息=空";
        }

        const auto 名称 = 主信息 ? 私有_词文本(主信息->名称) : std::string{};
        const auto 类型 = 主信息 ? 私有_词文本(主信息->类型) : std::string{};

        std::ostringstream 输出;
        输出 << "主信息=" << 私有_基础信息类别文本(主信息)
            << " | "
            << (!名称.empty() ? 名称 : 节点->获取主键())
            << " | 子=" << static_cast<std::size_t>(节点->子节点数量)
            << " | 主键=" << 节点->获取主键();
        if (!类型.empty()) {
            输出 << " | 类型=" << 类型;
        }
        if (const auto 子层摘要 = 私有_基础信息子层摘要(节点); !子层摘要.empty()) {
            输出 << " | 子主信息=" << 子层摘要;
        }
        if (私有_地址(节点) == 上下文.自我存在指针) {
            输出 << " | 当前自我存在";
        }
        if (私有_地址(节点) == 上下文.自我现实场景指针) {
            输出 << " | 当前现实场景";
        }
        if (私有_地址(节点) == 上下文.自我内部世界指针) {
            输出 << " | 当前内部世界";
        }
        return 输出.str();
    }

    enum class 枚举_需求方法可用性状态 : std::uint8_t {
        无需求状态 = 0,
        目标特征未解析 = 1,
        已满足 = 2,
        无候选方法 = 3,
        候选不可用 = 4,
        有可用方法 = 5,
        需求方向未解析 = 6,
        无可用同向方法 = 7,
        有可用同向方法 = 8,
    };

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_需求方法可用性状态文本(const 枚举_需求方法可用性状态 状态) noexcept
    {
        switch (状态) {
        case 枚举_需求方法可用性状态::无需求状态: return "无需求状态";
        case 枚举_需求方法可用性状态::目标特征未解析: return "目标特征未解析";
        case 枚举_需求方法可用性状态::已满足: return "已满足";
        case 枚举_需求方法可用性状态::无候选方法: return "无候选方法";
        case 枚举_需求方法可用性状态::候选不可用: return "候选不可用";
        case 枚举_需求方法可用性状态::有可用方法: return "有可用方法";
        case 枚举_需求方法可用性状态::需求方向未解析: return "需求方向未解析";
        case 枚举_需求方法可用性状态::无可用同向方法: return "无可用同向方法";
        case 枚举_需求方法可用性状态::有可用同向方法: return "有可用同向方法";
        default: return "未定义";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_方向文本_控制面板(const int 方向) noexcept
    {
        if (方向 > 0 && 方向 != 2) {
            return "增加";
        }
        if (方向 < 0) {
            return "减少";
        }
        if (方向 == 0) {
            return "不变";
        }
        return "未解析";
    }

    // 功能：按条件查找目标对象、方法或事实。
    const 语素入口节点类* 私有_查找特征模板入口_控制面板(const char* 名称) noexcept
    {
        try {
            if (!名称 || !*名称) {
                return nullptr;
            }
            return 语素集.查找信息入口节点(名称, 枚举_信息入口类型::特征模板入口);
        }
        catch (...) {
            return nullptr;
        }
    }

    using 枚举_需求目标语义_控制面板 = 需求类::枚举_需求目标语义;
    using 结构_需求目标语义视图_控制面板 = 需求类::结构_需求目标语义视图;

    // 功能：服务所在模块的内部辅助流程。
    结构_需求目标语义视图_控制面板 私有_需求目标语义视图_控制面板(
        const 语素入口节点类* 目标特征类型) noexcept
    {
        return 需求类::需求目标语义视图(目标特征类型);
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_需求结构形态文本_控制面板(const 需求节点* 节点) noexcept
    {
        return 需求类::需求结构形态文本(节点);
    }

    // 功能：推进任务筹办，形成候选、缺口或就绪回执。
    std::string 私有_普通筹办不可用原因_控制面板(
        const 需求节点* 节点,
        const 结构_需求目标语义视图_控制面板& 视图)
    {
        (void)节点;
        return 需求类::普通候选方法筹办禁止原因(视图);
    }

    // 功能：记录日志、动态、证据或运行痕迹。
    I64 私有_OR组令牌记录键_控制面板(const 需求节点* OR组) noexcept
    {
        std::uint64_t 哈希 = 1469598103934665603ull;
        const auto 主键 = OR组 ? 私有_节点主键_控制面板(OR组) : std::string{};
        for (const unsigned char 字符 : 主键) {
            哈希 ^= static_cast<std::uint64_t>(字符);
            哈希 *= 1099511628211ull;
        }
        if (主键.empty()) {
            哈希 ^= static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(OR组));
            哈希 *= 1099511628211ull;
        }
        const auto 正值 = static_cast<I64>(哈希 & 0x7fffffffffffffffULL);
        return 正值 != 0 ? 正值 : 1;
    }

    // 功能：记录日志、动态、证据或运行痕迹。
    存在节点类* 私有_查找OR组令牌记录_控制面板(const 需求节点* OR组) noexcept
    {
        auto* 自我存在 = 自我.获取自我存在();
        const auto* 记录类型 = 私有_查找特征模板入口_控制面板("OR组结算令牌记录");
        const auto* 记录键特征 = 私有_查找特征模板入口_控制面板("OR组令牌记录键");
        if (!OR组 || !自我存在 || !记录类型 || !记录键特征) {
            return nullptr;
        }

        const auto 目标键 = 私有_OR组令牌记录键_控制面板(OR组);
        for (auto* 子存在 : 世界树.获取子存在(
                 reinterpret_cast<基础信息节点类*>(自我存在))) {
            const auto* 主信息 = 世界树.取存在主信息(子存在);
            if (!主信息 || !语素入口同一(主信息->类型, 记录类型)) {
                continue;
            }
            I64 当前键 = 0;
            if (世界树.读取特征_I64(
                    reinterpret_cast<const 基础信息节点类*>(子存在),
                    记录键特征,
                    当前键)
                && 当前键 == 目标键) {
                return 子存在;
            }
        }
        return nullptr;
    }

    // 功能：读取 OR 组令牌状态显示文本。
    std::string 私有_OR组令牌状态文本_控制面板(
        const 需求节点* 节点,
        const 结构_需求目标语义视图_控制面板& 目标语义,
        const 存在节点类* 记录) noexcept
    {
        if (!节点 || !目标语义.是OR组目标) {
            return "不适用";
        }
        if (!自我.获取自我存在()) {
            return "自我存在空";
        }
        if (!记录) {
            return "无令牌记录";
        }

        const auto* 状态特征 = 私有_查找特征模板入口_控制面板("OR组结算令牌状态");
        I64 状态 = 0;
        if (状态特征
            && 世界树.读取特征_I64(
                reinterpret_cast<const 基础信息节点类*>(记录),
                状态特征,
                状态)) {
            if (状态 == 1) {
                return "已领取";
            }
            else if (状态 == 0) {
                return "未领取";
            }
            else {
                return "未知值=" + std::to_string(状态);
            }
        }
        return "有令牌记录但无状态";
    }

    // 功能：读取 OR 组令牌当前激活路径显示文本。
    std::string 私有_OR组令牌当前激活路径文本_控制面板(
        const 需求节点* 节点,
        const 结构_需求目标语义视图_控制面板& 目标语义,
        const 存在节点类* 记录) noexcept
    {
        if (!节点 || !目标语义.是OR组目标) {
            return "不适用";
        }
        if (!自我.获取自我存在() || !记录) {
            return "未触发";
        }

        const auto* 路径特征 = 私有_查找特征模板入口_控制面板("OR组领取路径");
        void* 路径指针 = nullptr;
        if (路径特征
            && 世界树.读取特征_指针(
                reinterpret_cast<const 基础信息节点类*>(记录),
                路径特征,
                路径指针)
            && 路径指针) {
            auto* 路径需求 = reinterpret_cast<const 需求节点*>(路径指针);
            const auto 路径主键 = 私有_节点主键_控制面板(路径需求);
            return 路径主键.empty()
                ? 私有_十六进制指针(私有_地址(路径需求))
                : 路径主键;
        }
        return "未写入";
    }

    // 功能：按条件查找目标对象、方法或事实。
    const 语素入口节点类* 私有_查找人类词性入口_控制面板(
        const std::string& 词值,
        const std::string& 词性值) noexcept
    {
        if (词值.empty() || 词性值.empty()) {
            return nullptr;
        }
        return 语素集.查找人类词性入口节点(词值, 词性值);
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_方法是否可用特征类型_控制面板() noexcept
    {
        return 私有_查找人类词性入口_控制面板("方法是否可用", "名词");
    }

    template<class T节点>
    T节点* 私有_解析基础信息引用_控制面板(const 可解析引用<T节点>& 引用) noexcept
    {
        if (引用.指针) {
            return reinterpret_cast<T节点*>(引用.指针);
        }
        if (!引用.主键.empty()) {
            return reinterpret_cast<T节点*>(世界树.按主键解析节点(引用.主键));
        }
        return nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_状态特征类型_控制面板(const 状态节点类* 状态节点) noexcept
    {
        const auto* 状态主信息 = 状态节点 ? 世界树.取状态主信息(状态节点) : nullptr;
        if (!状态主信息) {
            return nullptr;
        }

        auto* 特征节点 = 状态主信息->状态特征.指针;
        if (!特征节点 && !状态主信息->状态特征.主键.empty()) {
            特征节点 = 世界树.按主键解析特征节点(状态主信息->状态特征.主键);
        }
        const auto* 特征主信息 = 特征节点 ? 世界树.取特征主信息(特征节点) : nullptr;
        return 特征主信息 ? 特征主信息->类型 : nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_需求目标特征类型_控制面板(const 需求节点* 节点) noexcept
    {
        return 需求类::读取需求目标特征类型(节点);
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_需求是根层节点_控制面板(const 需求节点* 节点) noexcept
    {
        return 节点 && 节点->父 && 节点->父->父 == nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求显示名_控制面板(const 需求节点* 节点)
    {
        const auto* 目标特征类型 = 私有_需求目标特征类型_控制面板(节点);
        if (私有_需求是根层节点_控制面板(节点)
            && 语素入口同一(目标特征类型, 自我特征定义类::类型_自我_安全值())) {
            return "安全根需求";
        }
        if (私有_需求是根层节点_控制面板(节点)
            && 语素入口同一(目标特征类型, 自我特征定义类::类型_自我_服务值())) {
            return "服务根需求";
        }
        auto 文本 = 私有_词文本(目标特征类型);
        return 文本.empty() ? std::string("空目标特征") : std::move(文本);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_状态主体主键_控制面板(const 状态节点类* 状态节点) noexcept
    {
        const auto* 状态主信息 = 状态节点 ? 世界树.取状态主信息(状态节点) : nullptr;
        if (!状态主信息) {
            return {};
        }
        return 私有_引用主键_控制面板(状态主信息->状态主体);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求目标宿主主键_控制面板(const 需求节点* 节点) noexcept
    {
        if (!节点) {
            return {};
        }
        auto* 目标状态 = 私有_解析基础信息引用_控制面板(节点->主信息.需求状态);
        auto 目标宿主 = 私有_状态主体主键_控制面板(目标状态);
        if (!目标宿主.empty()) {
            return 目标宿主;
        }

        auto* 当前状态 = 私有_解析基础信息引用_控制面板(节点->主信息.被需求当前状态);
        目标宿主 = 私有_状态主体主键_控制面板(当前状态);
        if (!目标宿主.empty()) {
            return 目标宿主;
        }

        return 私有_引用主键_控制面板(节点->主信息.被需求存在);
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_基础主键是存在_控制面板(const std::string& 主键) noexcept
    {
        if (主键.empty()) {
            return false;
        }
        return 世界树.按主键解析存在节点(主键) != nullptr;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    const 任务节点* 私有_解析任务引用_控制面板(
        const 可解析引用<任务节点类>& 引用) noexcept
    {
        // 需求 / 任务树不是基础信息仓库节点；这里不能按基础信息主键兜底查找。
        // 诊断只认当前运行态中已经绑定的指针，避免只读路径触发隐式创建。
        return reinterpret_cast<const 任务节点*>(引用.指针);
    }

    // 功能：解析输入文本、消息、场景或运行包。
    const 需求节点* 私有_解析需求引用_控制面板(
        const 可解析引用<需求节点类>& 引用) noexcept
    {
        // 同上：控制面板只读取已绑定需求指针，不通过主键猜测需求树节点。
        return reinterpret_cast<const 需求节点*>(引用.指针);
    }

    bool 私有_需求存在完成任务但仍活动_控制面板(
        const 需求节点* 节点,
        std::string* 输出任务主键 = nullptr) noexcept
    {
        if (!节点 || 节点->主信息.需求有效截止 != 0) {
            return false;
        }

        // 有子需求和根需求允许长期保留历史完成任务；这里的异常只用于捕捉
        // “叶子需求已经由当前任务完成，但需求仍未截止”的真实闭环断点。
        if (节点->子) {
            return false;
        }

        const auto 当前任务完成 = [&](const 可解析引用<任务节点类>& 任务引用) noexcept {
            const auto* 任务 = 私有_解析任务引用_控制面板(任务引用);
            if (!任务
                || 任务类::读取任务节点种类(任务) != 枚举_任务节点种类::头结点
                || (任务类::读取任务状态(任务) != 枚举_任务状态::完成
                    && 任务类::读取任务状态(任务) != 枚举_任务状态::已结算)) {
                return false;
            }

            if (输出任务主键) {
                *输出任务主键 = 私有_任务节点主键_控制面板(任务);
            }
            return true;
        };

        if (节点->主信息.对应任务.有效()) {
            return 当前任务完成(节点->主信息.对应任务);
        }

        for (const auto& 任务引用 : 节点->主信息.任务列表) {
            if (当前任务完成(任务引用)) {
                return true;
            }
        }
        return false;
    }

    // 功能：检查状态、条件、缺口或运行约束。
    void 私有_检查需求父子权重_控制面板(
        const 需求节点* 父需求,
        结构_控制面板快照& 快照) noexcept
    {
        if (!父需求 || !父需求->子 || 父需求->子节点数量 <= 0) {
            return;
        }

        auto* 首子节点 = reinterpret_cast<const 需求节点*>(父需求->子);
        auto* 当前子节点 = 首子节点;
        std::int64_t 活跃子需求数量 = 0;
        do {
            if (当前子节点
                && 当前子节点->主信息.需求有效截止 == 0
                && 当前子节点->主信息.是否阻塞父任务执行) {
                ++活跃子需求数量;
            }
            当前子节点 = reinterpret_cast<const 需求节点*>(当前子节点->下);
        } while (当前子节点 && 当前子节点 != 首子节点);

        ++快照.需求树父子权重检查父需求数;
        当前子节点 = 首子节点;
        std::int64_t 活跃序号 = 0;
        const bool OR关系 = 需求类::子需求权重采用OR关系(父需求);
        do {
            if (!当前子节点) {
                break;
            }

            const bool 是活跃阻塞子需求 =
                当前子节点->主信息.需求有效截止 == 0
                && 当前子节点->主信息.是否阻塞父任务执行
                && 活跃子需求数量 > 0;
            const auto 期望权重 = 是活跃阻塞子需求
                ? 需求类::计算子需求权重(
                    父需求->主信息.安全权重,
                    父需求->主信息.服务权重,
                    活跃子需求数量,
                    活跃序号,
                    OR关系)
                : 需求类::结构_子需求平分权重{};
            if (是活跃阻塞子需求) {
                ++活跃序号;
            }
            const bool 权重符合 =
                当前子节点->主信息.安全权重 == 期望权重.安全权重
                && 当前子节点->主信息.服务权重 == 期望权重.服务权重;
            if (!权重符合) {
                std::ostringstream 日志;
                日志 << "控制面板/需求父子权重异常"
                    << " | 父需求=" << 私有_节点主键_控制面板(父需求)
                    << " | 子需求=" << 私有_节点主键_控制面板(当前子节点)
                    << " | 活跃子数=" << 活跃子需求数量
                    << " | 子活跃=" << (是活跃阻塞子需求 ? "是" : "否")
                    << " | OR关系=" << (OR关系 ? "是" : "否")
                    << " | 父安全=" << 父需求->主信息.安全权重
                    << " | 父服务=" << 父需求->主信息.服务权重
                    << " | 子安全=" << 当前子节点->主信息.安全权重
                    << " | 子服务=" << 当前子节点->主信息.服务权重
                    << " | 期望安全=" << 期望权重.安全权重
                    << " | 期望服务=" << 期望权重.服务权重;
                项目运行错误日志(日志.str());
                ++快照.需求树父子权重异常需求数;
            }

            当前子节点 = reinterpret_cast<const 需求节点*>(当前子节点->下);
        } while (当前子节点 && 当前子节点 != 首子节点);
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_状态I64值_控制面板(const 状态节点类* 状态节点, I64& 输出值) noexcept
    {
        const auto* 状态主信息 = 状态节点 ? 世界树.取状态主信息(状态节点) : nullptr;
        if (!状态主信息) {
            return false;
        }
        if (const auto* 标量 = std::get_if<I64>(&状态主信息->状态值)) {
            输出值 = *标量;
            return true;
        }
        return false;
    }

    // 功能：服务所在模块的内部辅助流程。
    int 私有_需求方向_控制面板(const 需求节点* 节点) noexcept
    {
        if (!节点) {
            return 2;
        }
        auto* 当前状态 = 私有_解析基础信息引用_控制面板(节点->主信息.被需求当前状态);
        auto* 目标状态 = 私有_解析基础信息引用_控制面板(节点->主信息.需求状态);
        const auto* 当前特征类型 = 私有_状态特征类型_控制面板(当前状态);
        const auto* 目标特征类型 = 私有_状态特征类型_控制面板(目标状态);
        if (!当前特征类型
            || !目标特征类型
            || !语素入口同一(当前特征类型, 目标特征类型)) {
            return 2;
        }

        I64 当前值 = 0;
        I64 目标值 = 0;
        if (!私有_状态I64值_控制面板(当前状态, 当前值)
            || !私有_状态I64值_控制面板(目标状态, 目标值)) {
            return 2;
        }
        if (目标值 > 当前值) {
            return 1;
        }
        if (目标值 < 当前值) {
            return -1;
        }
        return 0;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加唯一方法_控制面板(std::vector<方法节点*>& 列表, 方法节点* 方法) noexcept
    {
        if (!方法) {
            return;
        }
        if (std::find(列表.begin(), 列表.end(), 方法) == 列表.end()) {
            列表.push_back(方法);
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_收集候选方法_按结果特征_控制面板(
        方法节点* 当前方法节点,
        const 语素入口节点类* 目标特征类型,
        std::vector<方法节点*>& 输出,
        路径集合& 已访问) noexcept
    {
        if (!当前方法节点 || !目标特征类型) {
            return;
        }
        const auto 地址 = 私有_地址(当前方法节点);
        if (!已访问.insert(地址).second) {
            return;
        }

        const auto* 结果项集 = 方法类::方法结果项集(当前方法节点);
        if (方法类::方法是首节点(当前方法节点)
            && 方法类::方法有动作(当前方法节点)
            && 方法类::方法结果能力可被查找命中(当前方法节点)
            && !方法类::方法结果能力是根写入原语(当前方法节点)
            && 结果项集
            && std::any_of(
                结果项集->begin(),
                结果项集->end(),
                [&](const 结构_方法结果项& 结果项) {
                    return 语素入口同一(结果项.关系目标特征类型(), 目标特征类型);
                })) {
            私有_追加唯一方法_控制面板(输出, 当前方法节点);
        }

        if (!当前方法节点->子) {
            return;
        }
        auto* 首子 = reinterpret_cast<方法节点*>(当前方法节点->子);
        auto* 当前 = 首子;
        std::size_t 保护计数 = 0;
        do {
            私有_收集候选方法_按结果特征_控制面板(当前, 目标特征类型, 输出, 已访问);
            当前 = 当前 ? reinterpret_cast<方法节点*>(当前->下) : nullptr;
            ++保护计数;
        } while (当前 && 当前 != 首子 && 保护计数 < 4096);
    }

    // 功能：按条件查找目标对象、方法或事实。
    std::vector<方法节点*> 私有_查找需求候选方法_控制面板(
        const 需求节点* 需求,
        const 语素入口节点类* 目标特征类型) noexcept
    {
        std::vector<方法节点*> 候选方法{};
        if (!需求 || !目标特征类型) {
            return 候选方法;
        }

        std::vector<方法节点*> 方法根集{};
        auto* 被需求存在 = 私有_解析基础信息引用_控制面板(需求->主信息.被需求存在);
        if (auto* 方法根 = 被需求存在 ? 世界树.获取方法根节点(被需求存在) : nullptr) {
            私有_追加唯一方法_控制面板(方法根集, reinterpret_cast<方法节点*>(方法根));
        }
        if (auto* 自我存在 = 自我.获取自我存在()) {
            if (auto* 方法根 = 世界树.获取方法根节点(自我存在)) {
                私有_追加唯一方法_控制面板(方法根集, reinterpret_cast<方法节点*>(方法根));
            }
        }

        路径集合 已访问{};
        for (auto* 方法根 : 方法根集) {
            私有_收集候选方法_按结果特征_控制面板(
                方法根,
                目标特征类型,
                候选方法,
                已访问);
        }
        return 候选方法;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::optional<int> 私有_允许方向_控制面板(方向掩码 允许方向) noexcept
    {
        if (允许方向 == 方向_大于0) return 1;
        if (允许方向 == 方向_小于0) return -1;
        if (允许方向 == 方向_等于0) return 0;
        return std::nullopt;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_收集方法结果方向_控制面板(
        const 方法节点* 方法,
        std::vector<int>& 输出,
        路径集合& 已访问) noexcept
    {
        if (!方法) {
            return;
        }
        const auto 地址 = 私有_地址(方法);
        if (!已访问.insert(地址).second) {
            return;
        }

        if (!方法类::方法结果能力可被查找命中(方法)
            || 方法类::方法结果能力是根写入原语(方法)) {
            return;
        }
        const auto* 结果项集 = 方法类::方法结果项集(方法);
        if (!结果项集) {
            return;
        }
        for (const auto& 结果项 : *结果项集) {
            auto 方向 = 私有_允许方向_控制面板(结果项.关系允许方向());
            if (方向) {
                输出.push_back(*方向);
            }
        }

        if (!方法->子) {
            return;
        }
        auto* 首子 = reinterpret_cast<const 方法节点*>(方法->子);
        auto* 当前 = 首子;
        std::size_t 保护计数 = 0;
        do {
            私有_收集方法结果方向_控制面板(当前, 输出, 已访问);
            当前 = 当前 ? reinterpret_cast<const 方法节点*>(当前->下) : nullptr;
            ++保护计数;
        } while (当前 && 当前 != 首子 && 保护计数 < 4096);
    }

    // 功能：服务所在模块的内部辅助流程。
    int 私有_方法相对需求方向_控制面板(const 方法节点* 方法, const int 需求方向) noexcept
    {
        if (!方法 || 需求方向 == 2) {
            return 2;
        }

        std::vector<int> 方法方向集{};
        路径集合 已访问{};
        私有_收集方法结果方向_控制面板(方法, 方法方向集, 已访问);
        if (方法方向集.empty()) {
            return 2;
        }
        if (std::find(方法方向集.begin(), 方法方向集.end(), 需求方向) != 方法方向集.end()) {
            return 1;
        }
        return -1;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_方法当前可用_控制面板(const 方法节点* 方法) noexcept
    {
        if (!方法) {
            return false;
        }
        const auto* 方法是否可用 = 私有_方法是否可用特征类型_控制面板();
        if (!方法是否可用) {
            return false;
        }

        auto* 方法虚拟存在 = 方法类::方法虚拟存在只读(方法);
        I64 可用值 = 0;
        return 方法虚拟存在
            && 世界树.读取特征_I64(
                reinterpret_cast<const 基础信息节点类*>(方法虚拟存在),
                方法是否可用,
                可用值)
            && 可用值 != 0;
    }

    // 功能：评估状态、风险、质量、权重或完成度。
    void 私有_评估需求方法可用性_控制面板(
        const 需求节点* 节点,
        const 语素入口节点类*& 输出目标特征类型,
        std::vector<方法节点*>& 输出候选方法,
        std::size_t& 输出可用方法数,
        std::size_t& 输出同向候选方法数,
        std::size_t& 输出可用同向方法数,
        std::size_t& 输出有方法动作候选数,
        std::size_t& 输出方向未明候选数,
        std::size_t& 输出反向候选数,
        bool& 输出有需求状态引用,
        bool& 输出需求状态已满足,
        int& 输出需求方向,
        枚举_需求方法可用性状态& 输出状态) noexcept
    {
        输出目标特征类型 = nullptr;
        输出候选方法.clear();
        输出可用方法数 = 0;
        输出同向候选方法数 = 0;
        输出可用同向方法数 = 0;
        输出有方法动作候选数 = 0;
        输出方向未明候选数 = 0;
        输出反向候选数 = 0;
        输出有需求状态引用 = false;
        输出需求状态已满足 = false;
        输出需求方向 = 2;
        输出状态 = 枚举_需求方法可用性状态::无需求状态;
        if (!节点) {
            return;
        }

        输出有需求状态引用 = 节点->主信息.需求状态.指针
            || !节点->主信息.需求状态.主键.empty();
        auto* 当前状态 = reinterpret_cast<状态节点类*>(
            私有_解析基础信息引用_控制面板(节点->主信息.被需求当前状态));
        auto* 目标状态 = reinterpret_cast<状态节点类*>(
            私有_解析基础信息引用_控制面板(节点->主信息.需求状态));
        if (当前状态 && 目标状态) {
            const auto 比较 = 世界树.比较状态(
                当前状态,
                目标状态);
            输出需求状态已满足 = 比较.可比较
                && 三向关系被接受(比较.关系, 节点->主信息.满足关系);
        }
        输出目标特征类型 = 私有_需求目标特征类型_控制面板(节点);
        输出需求方向 = 私有_需求方向_控制面板(节点);

        if (!输出有需求状态引用) {
            输出状态 = 枚举_需求方法可用性状态::无需求状态;
            return;
        }
        if (!输出目标特征类型) {
            输出状态 = 枚举_需求方法可用性状态::目标特征未解析;
            return;
        }
        if (输出需求状态已满足) {
            输出状态 = 枚举_需求方法可用性状态::已满足;
            return;
        }
        if (输出需求方向 == 2) {
            输出状态 = 枚举_需求方法可用性状态::需求方向未解析;
        }

        输出候选方法 = 私有_查找需求候选方法_控制面板(节点, 输出目标特征类型);
        for (auto* 方法 : 输出候选方法) {
            if (方法类::方法有动作(方法)) {
                ++输出有方法动作候选数;
            }
            const auto 方向关系 = 私有_方法相对需求方向_控制面板(方法, 输出需求方向);
            if (方向关系 > 0) {
                ++输出同向候选方法数;
            }
            else if (方向关系 < 0) {
                ++输出反向候选数;
            }
            else {
                ++输出方向未明候选数;
            }
            if (私有_方法当前可用_控制面板(方法)) {
                ++输出可用方法数;
                if (方向关系 > 0) {
                    ++输出可用同向方法数;
                }
            }
        }

        if (输出候选方法.empty()) {
            输出状态 = 枚举_需求方法可用性状态::无候选方法;
        }
        else if (输出可用方法数 == 0) {
            输出状态 = 枚举_需求方法可用性状态::候选不可用;
        }
        else if (输出需求方向 != 2 && 输出可用同向方法数 == 0) {
            输出状态 = 枚举_需求方法可用性状态::无可用同向方法;
        }
        else if (输出需求方向 != 2 && 输出可用同向方法数 > 0) {
            输出状态 = 枚举_需求方法可用性状态::有可用同向方法;
        }
        else if (输出需求方向 == 2) {
            输出状态 = 枚举_需求方法可用性状态::需求方向未解析;
        }
        else {
            输出状态 = 枚举_需求方法可用性状态::有可用方法;
        }
    }

    enum class 枚举_需求满足显示状态_控制面板 {
        未满足,
        部分满足,
        全部满足
    };

    const char* 私有_需求满足显示状态文本_控制面板(
        const 枚举_需求满足显示状态_控制面板 状态) noexcept
    {
        switch (状态) {
        case 枚举_需求满足显示状态_控制面板::全部满足:
            return "全部满足";
        case 枚举_需求满足显示状态_控制面板::部分满足:
            return "部分满足";
        default:
            return "未满足";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_状态值文本_控制面板(const 状态节点类* 状态节点)
    {
        const auto* 状态主信息 = 状态节点
            ? 世界树.取状态主信息(状态节点)
            : nullptr;
        return 状态主信息 ? 私有_特征值文本(状态主信息->状态值) : std::string("空");
    }

    bool 私有_需求自身状态已满足_控制面板(
        const 需求节点* 节点,
        bool* 输出可比较 = nullptr,
        枚举_三向关系* 输出关系 = nullptr) noexcept
    {
        if (输出可比较) {
            *输出可比较 = false;
        }
        if (输出关系) {
            *输出关系 = 枚举_三向关系::等于;
        }
        if (!节点) {
            return false;
        }

        auto* 当前状态 = 私有_解析基础信息引用_控制面板(
            节点->主信息.被需求当前状态);
        auto* 目标状态 = 私有_解析基础信息引用_控制面板(
            节点->主信息.需求状态);
        if (当前状态 && 目标状态) {
            const auto 比较 = 世界树.比较状态(当前状态, 目标状态);
            if (输出可比较) {
                *输出可比较 = 比较.可比较;
            }
            if (输出关系) {
                *输出关系 = 比较.关系;
            }
            return 比较.可比较
                && 三向关系被接受(比较.关系, 节点->主信息.满足关系);
        }

        const auto 当前主键 = 私有_引用主键_控制面板(
            节点->主信息.被需求当前状态);
        const auto 目标主键 = 私有_引用主键_控制面板(
            节点->主信息.需求状态);
        if (!当前主键.empty() && 当前主键 == 目标主键) {
            if (输出可比较) {
                *输出可比较 = true;
            }
            if (输出关系) {
                *输出关系 = 枚举_三向关系::等于;
            }
            return 三向关系被接受(
                枚举_三向关系::等于,
                节点->主信息.满足关系);
        }
        return false;
    }

    枚举_需求满足显示状态_控制面板 私有_评估需求满足情况_控制面板(
        const 需求节点* 节点,
        bool& 当前已截止,
        bool& 自身状态可比较,
        bool& 自身状态已满足,
        枚举_三向关系& 自身比较关系,
        std::size_t& 直接子总数,
        std::size_t& 活动阻塞子数,
        std::size_t& 全部满足子数,
        std::size_t& 部分满足子数,
        std::size_t& 未满足子数,
        std::size_t& 已截止子数,
        const std::size_t 深度 = 0) noexcept
    {
        当前已截止 = false;
        自身状态可比较 = false;
        自身状态已满足 = false;
        自身比较关系 = 枚举_三向关系::等于;
        直接子总数 = 0;
        活动阻塞子数 = 0;
        全部满足子数 = 0;
        部分满足子数 = 0;
        未满足子数 = 0;
        已截止子数 = 0;
        if (!节点) {
            return 枚举_需求满足显示状态_控制面板::未满足;
        }

        当前已截止 = 节点->主信息.需求有效截止 != 0;
        自身状态已满足 = 私有_需求自身状态已满足_控制面板(
            节点,
            &自身状态可比较,
            &自身比较关系);

        if (节点->子 && 深度 < 32) {
            auto* 首子节点 = reinterpret_cast<const 需求节点*>(节点->子);
            auto* 当前子节点 = 首子节点;
            std::size_t 保护计数 = 0;
            do {
                if (!当前子节点) {
                    break;
                }
                ++直接子总数;
                if (当前子节点->主信息.需求有效截止 != 0) {
                    ++已截止子数;
                    ++全部满足子数;
                }
                else if (当前子节点->主信息.是否阻塞父任务执行) {
                    ++活动阻塞子数;
                    bool 子当前已截止 = false;
                    bool 子自身状态可比较 = false;
                    bool 子自身状态已满足 = false;
                    枚举_三向关系 子自身比较关系 = 枚举_三向关系::等于;
                    std::size_t 子直接子总数 = 0;
                    std::size_t 子活动阻塞子数 = 0;
                    std::size_t 子全部满足子数 = 0;
                    std::size_t 子部分满足子数 = 0;
                    std::size_t 子未满足子数 = 0;
                    std::size_t 子已截止子数 = 0;
                    const auto 子满足状态 = 私有_评估需求满足情况_控制面板(
                        当前子节点,
                        子当前已截止,
                        子自身状态可比较,
                        子自身状态已满足,
                        子自身比较关系,
                        子直接子总数,
                        子活动阻塞子数,
                        子全部满足子数,
                        子部分满足子数,
                        子未满足子数,
                        子已截止子数,
                        深度 + 1);
                    if (子满足状态 == 枚举_需求满足显示状态_控制面板::全部满足) {
                        ++全部满足子数;
                    }
                    else if (子满足状态 == 枚举_需求满足显示状态_控制面板::部分满足) {
                        ++部分满足子数;
                    }
                    else {
                        ++未满足子数;
                    }
                }
                当前子节点 = reinterpret_cast<const 需求节点*>(当前子节点->下);
                ++保护计数;
            } while (当前子节点 && 当前子节点 != 首子节点 && 保护计数 < 2048);
        }

        if (当前已截止 || 自身状态已满足) {
            return 枚举_需求满足显示状态_控制面板::全部满足;
        }
        if (活动阻塞子数 > 0
            && 未满足子数 == 0
            && 部分满足子数 == 0
            && 全部满足子数 >= 活动阻塞子数) {
            return 枚举_需求满足显示状态_控制面板::全部满足;
        }
        if (全部满足子数 > 0 || 部分满足子数 > 0) {
            return 枚举_需求满足显示状态_控制面板::部分满足;
        }
        return 枚举_需求满足显示状态_控制面板::未满足;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求满足情况说明_控制面板(
        const 枚举_需求满足显示状态_控制面板 满足状态,
        const bool 当前已截止,
        const bool 自身状态可比较,
        const bool 自身状态已满足,
        const 枚举_三向关系 自身比较关系,
        const std::size_t 直接子总数,
        const std::size_t 活动阻塞子数,
        const std::size_t 全部满足子数,
        const std::size_t 部分满足子数,
        const std::size_t 未满足子数,
        const std::size_t 已截止子数)
    {
        std::ostringstream 输出;
        输出 << "自身=" << (自身状态已满足 ? "满足" : "未满足")
            << " | 可比较=" << 布尔文本_是或否(自身状态可比较)
            << " | 关系=" << static_cast<I64>(自身比较关系)
            << " | 已截止=" << 布尔文本_是或否(当前已截止)
            << " | 直接子=" << 直接子总数
            << " | 活动阻塞子=" << 活动阻塞子数
            << " | 全部满足子=" << 全部满足子数
            << " | 部分满足子=" << 部分满足子数
            << " | 未满足子=" << 未满足子数
            << " | 已截止子=" << 已截止子数
            << " | 显示状态=" << 私有_需求满足显示状态文本_控制面板(满足状态);
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求标题_核心(const 需求节点* 节点)
    {
        if (!节点) {
            return "空需求";
        }

        std::string 主键{};
        if (!私有_尝试读取需求节点主键_控制面板(节点, &主键)) {
            return std::string("异常需求节点 | 主键不可读 | 地址=")
                + 私有_十六进制指针(私有_地址(节点));
        }

        const auto 显示名 = 私有_需求显示名_控制面板(节点);
        std::ostringstream 输出;
        输出 << "需求"
            << " | 目标特征=" << 显示名
            << " | 安全=" << 节点->主信息.安全权重
            << " | 服务=" << 节点->主信息.服务权重
            << " | 子=" << static_cast<std::size_t>(节点->子节点数量)
            << " | 主键=" << 主键;
        if (const auto 子层摘要 = 私有_需求子层摘要(节点); !子层摘要.empty()) {
            输出 << " | 子目标=" << 子层摘要;
        }
        const 语素入口节点类* 方法评估目标特征类型 = nullptr;
        std::vector<方法节点*> 方法评估候选方法{};
        std::size_t 方法评估可用方法数 = 0;
        std::size_t 方法评估同向候选方法数 = 0;
        std::size_t 方法评估可用同向方法数 = 0;
        std::size_t 方法评估有方法动作候选数 = 0;
        std::size_t 方法评估方向未明候选数 = 0;
        std::size_t 方法评估反向候选数 = 0;
        bool 方法评估有需求状态引用 = false;
        bool 方法评估需求状态已满足 = false;
        int 方法评估需求方向 = 2;
        枚举_需求方法可用性状态 方法评估状态 = 枚举_需求方法可用性状态::无需求状态;
        私有_评估需求方法可用性_控制面板(
            节点,
            方法评估目标特征类型,
            方法评估候选方法,
            方法评估可用方法数,
            方法评估同向候选方法数,
            方法评估可用同向方法数,
            方法评估有方法动作候选数,
            方法评估方向未明候选数,
            方法评估反向候选数,
            方法评估有需求状态引用,
            方法评估需求状态已满足,
            方法评估需求方向,
            方法评估状态);
        if (方法评估有需求状态引用 || 方法评估目标特征类型) {
            输出 << " | 方法=" << 私有_需求方法可用性状态文本(方法评估状态);
            if (方法评估目标特征类型) {
                输出 << " | 目标特征=" << 私有_词文本(方法评估目标特征类型);
            }
            输出 << " | 方向=" << 私有_方向文本_控制面板(方法评估需求方向);
            if (!方法评估候选方法.empty() || 方法评估可用方法数 > 0) {
                输出 << " | 候选=" << 方法评估候选方法.size()
                    << " | 可用=" << 方法评估可用方法数
                    << " | 可用同向=" << 方法评估可用同向方法数;
            }
        }
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求标题(const 需求节点* 节点)
    {
        try {
            return 私有_需求标题_核心(节点);
        }
        catch (const std::exception& 异常) {
            std::ostringstream 日志;
            日志 << "控制面板/需求标题读取失败"
                << " | 地址=" << 私有_十六进制指针(私有_地址(节点))
                << " | 异常=" << 异常.what();
            项目运行错误日志(日志.str());
        }
        catch (...) {
            std::ostringstream 日志;
            日志 << "控制面板/需求标题读取失败"
                << " | 地址=" << 私有_十六进制指针(私有_地址(节点))
                << " | 异常=未知异常";
            项目运行错误日志(日志.str());
        }
        return std::string("需求标题读取失败 | 地址=")
            + 私有_十六进制指针(私有_地址(节点));
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求结构标题_核心(const 需求节点* 节点)
    {
        if (!节点) {
            return "空需求";
        }

        std::string 主键{};
        if (!私有_尝试读取需求节点主键_控制面板(节点, &主键)) {
            return std::string("异常需求节点 | 主键不可读 | 地址=")
                + 私有_十六进制指针(私有_地址(节点));
        }

        const bool 是结构根 = 节点->父 == nullptr;
        const bool 是根需求 = 节点->父 != nullptr && 节点->父->父 == nullptr;
        const auto 显示名 = 私有_需求显示名_控制面板(节点);
        std::ostringstream 输出;
        输出 << (是结构根 ? "需求树根" : (是根需求 ? "根需求" : "子需求"))
            << " | " << 显示名
            << " | 子=" << static_cast<std::size_t>(节点->子节点数量)
            << " | 主键=" << 主键;
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求结构标题(const 需求节点* 节点)
    {
        try {
            return 私有_需求结构标题_核心(节点);
        }
        catch (const std::exception& 异常) {
            std::ostringstream 日志;
            日志 << "控制面板/需求结构标题读取失败"
                << " | 地址=" << 私有_十六进制指针(私有_地址(节点))
                << " | 异常=" << 异常.what();
            项目运行错误日志(日志.str());
        }
        catch (...) {
            std::ostringstream 日志;
            日志 << "控制面板/需求结构标题读取失败"
                << " | 地址=" << 私有_十六进制指针(私有_地址(节点))
                << " | 异常=未知异常";
            项目运行错误日志(日志.str());
        }
        return std::string("需求结构标题读取失败 | 地址=")
            + 私有_十六进制指针(私有_地址(节点));
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_任务标题(const 任务节点* 节点)
    {
        if (!节点) {
            return "空任务";
        }

        const auto 名称 = 私有_词文本(节点->主信息.名称);
        std::ostringstream 输出;
        输出 << 私有_任务节点种类文本(任务类::读取任务节点种类(节点))
            << " | " << (名称.empty() ? 节点->获取主键() : 名称)
            << " | 状态=" << 私有_任务状态文本(任务类::读取任务状态(节点))
            << " | 子=" << static_cast<std::size_t>(节点->子节点数量)
            << " | 主键=" << 节点->获取主键();
        if (const auto 子层摘要 = 私有_任务子层摘要(节点); !子层摘要.empty()) {
            输出 << " | 子种类=" << 子层摘要;
        }
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_任务结构标题(const 任务节点* 节点)
    {
        if (!节点) {
            return "空任务";
        }

        const auto 名称 = 私有_词文本(节点->主信息.名称);
        const bool 是结构根 = 节点->父 == nullptr;
        const auto 标识 = 名称.empty() ? 节点->获取主键() : 名称;
        std::ostringstream 输出;
        输出 << (是结构根 ? "根节点" : 私有_任务节点种类文本(任务类::读取任务节点种类(节点)));
        if (!标识.empty()) {
            输出 << " | " << 标识;
        }
        输出 << " | 状态=" << 私有_任务状态文本(任务类::读取任务状态(节点))
            << " | 子=" << static_cast<std::size_t>(节点->子节点数量)
            << " | 主键=" << 节点->获取主键();
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_方法标题(const 方法节点* 节点)
    {
        if (!节点) {
            return "空方法";
        }

        std::string 标题 = 私有_词文本(方法类::方法动作名(节点));
        if (标题.empty()) {
            标题 = 私有_词文本(方法类::方法首个结果能力特征类型(节点));
        }
        if (标题.empty()) {
            标题 = 节点->获取主键();
        }

        std::ostringstream 输出;
        输出 << 私有_方法节点种类文本(方法类::方法节点种类(节点))
            << " | " << 标题
            << " | 来源=" << 私有_方法来源文本(方法类::方法来源(节点))
            << " | 子=" << static_cast<std::size_t>(节点->子节点数量)
            << " | 主键=" << 节点->获取主键();
        if (const auto 子层摘要 = 私有_方法子层摘要(节点); !子层摘要.empty()) {
            输出 << " | 子种类=" << 子层摘要;
        }
        return 输出.str();
    }

    结构_控制面板树节点 私有_基础信息骨架节点(
        基础信息节点类* 节点,
        const 结构_构建上下文& 上下文,
        const bool 默认展开 = false)
    {
        return 私有_新节点(
            私有_基础信息标题(节点, 上下文),
            私有_地址(节点),
            默认展开,
            节点 != nullptr,
            "base-node");
    }

    结构_控制面板树节点 私有_需求骨架节点(
        需求节点* 节点,
        const 结构_构建上下文& 上下文,
        const bool 默认展开 = false)
    {
        if (节点 && !私有_尝试读取需求节点主键_控制面板(节点, nullptr)) {
            // 需求树可能正在由自我线程更新；展示层遇到主键不可读的节点时
            // 不继续展开它，避免把坏节点伪装成“空类型/多子节点”的正常需求。
            return 私有_新节点(
                std::string("异常需求节点 | 已停止展开 | 地址=")
                    + 私有_十六进制指针(私有_地址(节点)),
                私有_地址(节点),
                false,
                false);
        }
        return 私有_新节点(
            私有_需求标题(节点),
            私有_地址(节点),
            默认展开,
            节点 != nullptr,
            "need-node");
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_标记详情默认展开(结构_控制面板树节点& 节点)
    {
        if (!节点.子项.empty() && !节点.可延迟展开) {
            节点.默认展开 = true;
        }
        for (auto& 子项 : 节点.子项) {
            私有_标记详情默认展开(子项);
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_需求列表摘要文本(需求节点* 节点)
    {
        auto* 目标状态 = 私有_解析基础信息引用_控制面板(节点->主信息.需求状态);
        const auto* 目标特征类型 = 私有_需求目标特征类型_控制面板(节点);

        std::ostringstream 输出;
        输出 << "主键=" << 私有_节点主键_控制面板(节点)
            << " | 子节点数量=" << static_cast<std::size_t>(节点->子节点数量)
            << " | 目标特征类型=" << 私有_词文本(目标特征类型)
            << " | 目标特征值=" << 私有_状态值文本_控制面板(目标状态);
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    结构_控制面板树节点 私有_需求列表详情节点(
        需求节点* 节点,
        const 结构_构建上下文& 上下文)
    {
        if (!节点) {
            return 私有_新节点("空需求");
        }
        if (!私有_尝试读取需求节点主键_控制面板(节点, nullptr)) {
            return 私有_新节点(
                std::string("异常需求 | 已停止读取详情 | 地址=")
                    + 私有_十六进制指针(私有_地址(节点)),
                私有_地址(节点),
                false,
                false);
        }

        auto 列表项 = 私有_新节点(
            私有_需求列表摘要文本(节点),
            私有_地址(节点),
            false,
            false);
        try {
            auto 详情列表 = 私有_构建需求主信息详情(节点, 上下文);
            for (auto& 详情 : 详情列表) {
                私有_标记详情默认展开(详情);
                列表项.详情.push_back(std::move(详情));
            }
        }
        catch (const std::exception& 异常) {
            std::ostringstream 日志;
            日志 << "控制面板/需求列表详情读取失败"
                << " | 地址=" << 私有_十六进制指针(私有_地址(节点))
                << " | 异常=" << 异常.what();
            项目运行错误日志(日志.str());
            列表项.详情.push_back(私有_新节点(
                std::string("需求详情读取失败 | 异常=") + 异常.what()));
        }
        catch (...) {
            std::ostringstream 日志;
            日志 << "控制面板/需求列表详情读取失败"
                << " | 地址=" << 私有_十六进制指针(私有_地址(节点))
                << " | 异常=未知异常";
            项目运行错误日志(日志.str());
            列表项.详情.push_back(私有_新节点("需求详情读取失败 | 异常=未知异常"));
        }
        if (列表项.详情.empty()) {
            列表项.详情.push_back(私有_新节点("需求详情为空"));
        }
        return 列表项;
    }

    结构_控制面板树节点 私有_需求结构骨架节点(
        需求节点* 节点,
        const 结构_构建上下文& 上下文,
        const bool 默认展开 = false)
    {
        if (节点 && !私有_尝试读取需求节点主键_控制面板(节点, nullptr)) {
            // 结构树只负责展示需求链，不在这里修复链表；异常节点需要
            // 保留地址与日志，供后续沿入树/删除路径继续排查。
            return 私有_新节点(
                std::string("异常需求节点 | 已停止展开 | 地址=")
                    + 私有_十六进制指针(私有_地址(节点)),
                私有_地址(节点),
                false,
                false);
        }
        return 私有_新节点(
            私有_需求结构标题(节点),
            私有_地址(节点),
            默认展开,
            节点 != nullptr,
            "need-node");
    }

    结构_控制面板树节点 私有_任务骨架节点(
        任务节点* 节点,
        const 结构_构建上下文& 上下文,
        const bool 默认展开 = false)
    {
        return 私有_新节点(
            私有_任务结构标题(节点),
            私有_地址(节点),
            默认展开,
            节点 != nullptr,
            "task-node");
    }

    结构_控制面板树节点 私有_方法骨架节点(
        方法节点* 节点,
        const 结构_构建上下文& 上下文,
        const bool 默认展开 = false)
    {
        auto 文本 = 私有_方法标题(节点);
        if (方法类::方法是首节点(节点)) {
            std::size_t 条件数 = 0;
            std::size_t 直接结果数 = 0;
            std::size_t 条件下结果数 = 0;
            std::size_t 条件无结果数 = 0;
            私有_统计方法直接条件结果_控制面板(
                节点,
                条件数,
                直接结果数,
                条件下结果数,
                条件无结果数);
            if (直接结果数 > 0) {
                文本 += " | 结构异常=结果直挂首节点";
            }
            else if (条件数 > 0 && 条件下结果数 == 0) {
                文本 += " | 结构异常=有条件无结果";
            }
            else if (条件无结果数 > 0) {
                文本 += " | 结构异常=条件结果不成对";
            }
        }
        return 私有_新节点(
            文本,
            私有_地址(节点),
            默认展开,
            节点 != nullptr,
            "method-node");
    }

    // 功能：构建运行所需的数据结构或中间结果。
    std::vector<结构_控制面板树节点> 私有_构建方法主信息详情(方法节点* 节点)
    {
        if (!节点) {
            return { 私有_新节点("空方法节点") };
        }

        路径集合 路径{};
        路径.insert(私有_地址(节点));

        auto 字段节点 = 私有_新节点("方法主信息");
        私有_追加等号字段(字段节点, "主键", 节点->获取主键());
        私有_追加等号字段(字段节点, "公共.描述信息", 私有_自然句文本(节点->主信息.公共.描述信息));
        私有_追加等号字段(字段节点, "公共.节点种类", 私有_方法节点种类文本(方法类::方法节点种类(节点)));
        私有_追加等号字段(字段节点, "公共.统计.创建时间", 私有_时间文本(节点->主信息.公共.统计.创建时间));
        私有_追加等号字段(字段节点, "公共.统计.最后观测时间", 私有_时间文本(节点->主信息.公共.统计.最后观测时间));
        私有_追加等号字段(字段节点, "公共.统计.命中次数", 节点->主信息.公共.统计.命中次数);

        const auto* 条件信息 = 节点->主信息.取条件节点信息();
        const auto* 结果信息 = 节点->主信息.取结果节点信息();

        if (方法类::方法是首节点(节点)) {
            私有_追加等号引用字段(字段节点, "角色.首节点.方法虚拟存在", 私有_方法虚拟存在引用_控制面板(节点), 路径);
            私有_追加等号字段(字段节点, "角色.首节点.动作名", 私有_词文本(方法类::方法动作名(节点)));
            私有_追加等号字段(字段节点, "角色.首节点.动作句柄", 私有_动作句柄文本(方法类::方法动作句柄(节点)));
            私有_追加等号字段(字段节点, "角色.首节点.来源", 私有_方法来源文本(方法类::方法来源(节点)));
            私有_追加等号引用字段(字段节点, "角色.首节点.来源任务", 方法类::方法来源任务引用(节点), 路径);
            私有_追加等号引用字段(字段节点, "角色.首节点.父方法", 方法类::方法父方法引用(节点), 路径);
            私有_追加等号引用字段(字段节点, "角色.首节点.前置方法", 方法类::方法前置方法引用(节点), 路径);
            私有_追加等号引用字段(字段节点, "角色.首节点.后续方法", 方法类::方法后续方法引用(节点), 路径);
            私有_追加等号字段(
                字段节点,
                "角色.首节点.能力.结果包.结果项数",
                static_cast<std::uint64_t>(
                    方法类::方法结果项集(节点)
                        ? 方法类::方法结果项集(节点)->size()
                        : 0));
            私有_追加等号字段(
                字段节点,
                "角色.首节点.能力.可被方法查找命中",
                方法类::方法结果能力可被查找命中(节点));
            私有_追加等号字段(
                字段节点,
                "角色.首节点.能力.是否根写入原语",
                方法类::方法结果能力是根写入原语(节点));
            私有_追加等号字段(字段节点, "角色.首节点.成熟度阶段", 方法类::方法成熟度阶段(节点));
            私有_追加等号字段(字段节点, "角色.首节点.是否允许自动查找", 方法类::方法允许自动查找(节点));
        }
        else if (条件信息) {
            私有_追加等号引用字段(字段节点, "角色.条件节点.条件场景", 条件信息->条件场景, 路径);
            私有_追加等号字段(字段节点, "角色.条件节点.条件主键", 条件信息->条件主键);
            私有_追加等号字段(字段节点, "角色.条件节点.条件场景模板主键", 条件信息->条件场景模板主键);
            私有_追加等号字段(
                字段节点,
                "角色.条件节点.条件判定索引数",
                static_cast<std::uint64_t>(条件信息->条件判定索引.size()));
            私有_追加等号字段(字段节点, "角色.条件节点.时间起", 私有_时间文本(条件信息->时间起));
            私有_追加等号字段(字段节点, "角色.条件节点.时间止", 私有_时间文本(条件信息->时间止));
        }
        else if (结果信息) {
            私有_追加等号引用字段(字段节点, "角色.结果节点.结果场景", 结果信息->结果场景, 路径);
            私有_追加等号字段(字段节点, "角色.结果节点.结果主键", 结果信息->结果主键);
            私有_追加等号字段(
                字段节点,
                "角色.结果节点.结果包.结果项数",
                static_cast<std::uint64_t>(结果信息->结果包.结果项集.size()));
            私有_追加等号字段(
                字段节点,
                "角色.结果节点.结果初始状态数",
                static_cast<std::uint64_t>(结果信息->结果初始状态.size()));
        }
        else {
            私有_追加等号字段(字段节点, "角色", "未识别方法角色信息");
        }

        return { std::move(字段节点) };
    }

    // 功能：构建运行所需的数据结构或中间结果。
    std::vector<结构_控制面板树节点> 私有_构建任务主信息详情(任务节点* 节点)
    {
        if (!节点) {
            return { 私有_新节点("空任务节点") };
        }

        路径集合 路径{};
        路径.insert(私有_地址(节点));

        auto 字段节点 = 私有_新节点("任务主信息");
        私有_追加等号字段(字段节点, "主键", 节点->获取主键());
        私有_追加等号字段(字段节点, "名称", 私有_词文本(节点->主信息.名称));
        私有_追加等号字段(字段节点, "类型", 私有_词文本(节点->主信息.类型));
        私有_追加等号字段(字段节点, "创建时间", 私有_时间文本(节点->主信息.创建时间));
        私有_追加等号字段(字段节点, "启动时间", 私有_时间文本(节点->主信息.启动时间));
        私有_追加等号字段(字段节点, "完成时间", 私有_时间文本(节点->主信息.完成时间));

        私有_追加等号引用字段(字段节点, "对应需求", 节点->主信息.对应需求, 路径);
        私有_追加等号引用字段(字段节点, "任务虚拟存在", 节点->主信息.任务虚拟存在, 路径);
        私有_追加等号引用字段(字段节点, "场景", 节点->主信息.场景, 路径);
        私有_追加等号引用字段(字段节点, "目标状态", 节点->主信息.目标状态, 路径);
        私有_追加等号引用字段(字段节点, "实际结果状态", 节点->主信息.实际结果状态, 路径);

        return { std::move(字段节点) };
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_三向关系掩码文本(const 三向关系掩码 掩码)
    {
        std::vector<std::string> 片段{};
        if ((掩码 & 关系_小于) != 0) {
            片段.push_back("小于");
        }
        if ((掩码 & 关系_等于) != 0) {
            片段.push_back("等于");
        }
        if ((掩码 & 关系_大于) != 0) {
            片段.push_back("大于");
        }
        std::ostringstream 输出;
        if (片段.empty()) {
            输出 << "未设置";
        }
        else {
            for (std::size_t i = 0; i < 片段.size(); ++i) {
                if (i > 0) {
                    输出 << "/";
                }
                输出 << 片段[i];
            }
        }
        输出 << " (" << static_cast<unsigned>(掩码) << ")";
        return 输出.str();
    }

    std::string 私有_拼接片段_控制面板(
        const std::vector<std::string>& 片段集,
        const std::size_t 上限 = 8)
    {
        if (片段集.empty()) {
            return "无";
        }
        std::ostringstream 输出;
        const auto 数量 = (std::min)(片段集.size(), 上限);
        for (std::size_t i = 0; i < 数量; ++i) {
            if (i > 0) {
                输出 << ";";
            }
            输出 << 片段集[i];
        }
        if (片段集.size() > 数量) {
            输出 << ";...";
        }
        return 输出.str();
    }

    // 功能：构建运行所需的数据结构或中间结果。
    std::vector<结构_控制面板树节点> 私有_构建需求主信息详情(
        需求节点* 节点,
        const 结构_构建上下文& 上下文)
    {
        if (!节点) {
            return { 私有_新节点("空需求节点") };
        }

        路径集合 路径{};
        路径.insert(私有_地址(节点));

        auto 字段节点 = 私有_新节点("需求主信息");
        私有_追加等号字段(字段节点, "主键", 私有_节点主键_控制面板(节点));
        私有_追加等号字段(字段节点, "节点地址", 私有_十六进制指针(私有_地址(节点)));
        私有_追加等号字段(字段节点, "描述信息", 私有_自然句文本(节点->主信息.描述信息));
        私有_追加等号字段(字段节点, "显示名", 私有_需求显示名_控制面板(节点));
        私有_追加等号字段(字段节点, "需求有效截止", 私有_时间文本(节点->主信息.需求有效截止));
        私有_追加等号字段(字段节点, "是否阻塞父任务执行", 节点->主信息.是否阻塞父任务执行);
        私有_追加等号字段(字段节点, "安全权重", 节点->主信息.安全权重);
        私有_追加等号字段(字段节点, "服务权重", 节点->主信息.服务权重);
        私有_追加等号字段(字段节点, "满足关系", 私有_三向关系掩码文本(节点->主信息.满足关系));
        auto* 当前状态 = 私有_解析基础信息引用_控制面板(节点->主信息.被需求当前状态);
        auto* 目标状态 = 私有_解析基础信息引用_控制面板(节点->主信息.需求状态);
        const auto* 目标特征类型 = 私有_需求目标特征类型_控制面板(节点);
        bool 当前已截止 = false;
        bool 自身状态可比较 = false;
        bool 自身状态已满足 = false;
        枚举_三向关系 自身比较关系 = 枚举_三向关系::等于;
        std::size_t 直接子总数 = 0;
        std::size_t 活动阻塞子数 = 0;
        std::size_t 全部满足子数 = 0;
        std::size_t 部分满足子数 = 0;
        std::size_t 未满足子数 = 0;
        std::size_t 已截止子数 = 0;
        const auto 满足状态 = 私有_评估需求满足情况_控制面板(
            节点,
            当前已截止,
            自身状态可比较,
            自身状态已满足,
            自身比较关系,
            直接子总数,
            活动阻塞子数,
            全部满足子数,
            部分满足子数,
            未满足子数,
            已截止子数);
        私有_追加等号字段(字段节点, "目标特征类型", 私有_词文本(目标特征类型));
        私有_追加等号字段(字段节点, "目标特征值", 私有_状态值文本_控制面板(目标状态));
        私有_追加等号字段(字段节点, "当前特征值", 私有_状态值文本_控制面板(当前状态));
        私有_追加等号字段(
            字段节点,
            "满足情况",
            私有_需求满足显示状态文本_控制面板(满足状态));
        私有_追加等号字段(
            字段节点,
            "满足情况说明",
            私有_需求满足情况说明_控制面板(
                满足状态,
                当前已截止,
                自身状态可比较,
                自身状态已满足,
                自身比较关系,
                直接子总数,
                活动阻塞子数,
                全部满足子数,
                部分满足子数,
                未满足子数,
                已截止子数));
        私有_追加等号字段(字段节点, "目标特征类型缓存", 私有_词文本(节点->主信息.目标特征类型缓存));
        const auto 目标语义视图 = 私有_需求目标语义视图_控制面板(目标特征类型);
        const auto* 令牌记录 = 私有_查找OR组令牌记录_控制面板(节点);
        const auto 令牌状态文本 = 私有_OR组令牌状态文本_控制面板(节点, 目标语义视图, 令牌记录);
        const auto 令牌激活路径文本 = 私有_OR组令牌当前激活路径文本_控制面板(节点, 目标语义视图, 令牌记录);
        私有_追加等号字段(字段节点, "结构形态", 私有_需求结构形态文本_控制面板(节点));
        私有_追加等号字段(
            字段节点,
            "可进入普通候选方法筹办",
            目标语义视图.可进入普通候选方法筹办);
        私有_追加等号字段(
            字段节点,
            "不可进入普通筹办原因",
            私有_普通筹办不可用原因_控制面板(节点, 目标语义视图));
        私有_追加等号字段(字段节点, "OR组令牌状态", 令牌状态文本);
        私有_追加等号字段(字段节点, "OR组当前激活路径", 令牌激活路径文本);

        私有_追加等号引用字段(字段节点, "需求主体", 节点->主信息.需求主体, 路径);
        私有_追加等号引用字段(字段节点, "需求场景", 节点->主信息.需求场景, 路径);
        私有_追加等号引用字段(字段节点, "被需求存在", 节点->主信息.被需求存在, 路径);
        私有_追加等号引用字段(字段节点, "被需求当前状态", 节点->主信息.被需求当前状态, 路径);
        私有_追加等号引用字段(字段节点, "需求状态", 节点->主信息.需求状态, 路径);
        私有_追加等号引用字段(字段节点, "对应任务", 节点->主信息.对应任务, 路径);

        auto 任务列表节点 = 私有_新节点(
            私有_等号字段显示文本(
                "任务列表",
                "数量 " + std::to_string(节点->主信息.任务列表.size())));
        const auto 任务列表上限 = (std::min)(上下文.树广度上限, 节点->主信息.任务列表.size());
        for (std::size_t 索引 = 0; 索引 < 任务列表上限; ++索引) {
            私有_追加等号引用字段(
                任务列表节点,
                "任务列表[" + std::to_string(索引) + "]",
                节点->主信息.任务列表[索引],
                路径);
        }
        if (节点->主信息.任务列表.size() > 任务列表上限) {
            任务列表节点.子项.push_back(
                私有_新节点("... 省略 " + std::to_string(节点->主信息.任务列表.size() - 任务列表上限) + " 项"));
        }
        字段节点.子项.push_back(std::move(任务列表节点));

        私有_追加等号字段(字段节点, "累计安全结算", 节点->主信息.累计安全结算);
        私有_追加等号字段(字段节点, "累计服务结算", 节点->主信息.累计服务结算);
        私有_追加等号字段(字段节点, "最近结算任务主键", 节点->主信息.最近结算任务主键);
        私有_追加等号字段(字段节点, "最近结算时间", 私有_时间文本(节点->主信息.最近结算时间));

        私有_追加等号字段(字段节点, "统计.创建时间", 私有_时间文本(节点->主信息.统计.创建时间));
        私有_追加等号字段(字段节点, "统计.最后观测时间", 私有_时间文本(节点->主信息.统计.最后观测时间));
        私有_追加等号字段(字段节点, "统计.命中次数", 节点->主信息.统计.命中次数);

        return { std::move(字段节点) };
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加基础信息通用字段(
        结构_控制面板树节点& 字段节点,
        基础信息节点类* 节点,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径)
    {
        if (!节点) {
            return;
        }

        私有_追加叶字段(字段节点, "主键", 节点->获取主键());
        私有_追加叶字段(字段节点, "节点地址", 私有_十六进制指针(私有_地址(节点)));
        (void)上下文;
        (void)剩余深度;
        (void)路径;

        if (const auto* 主信息 = 节点->主信息) {
            私有_追加叶字段(字段节点, "节点类别", 私有_基础信息类别文本(主信息));
            私有_追加叶字段(字段节点, "主信息类型", 私有_主信息类型文本(主信息->主信息类型));
            私有_追加叶字段(字段节点, "主信息运行时基类", "基础信息基类派生");
            私有_追加叶字段(字段节点, "名称", 私有_词文本(主信息->名称));
            私有_追加叶字段(字段节点, "名称入口诊断", 私有_语素入口诊断文本(主信息->名称));
            私有_追加叶字段(字段节点, "类型", 私有_词文本(主信息->类型));
            私有_追加叶字段(字段节点, "类型入口诊断", 私有_语素入口诊断文本(主信息->类型));
            私有_追加统计字段(字段节点, 主信息->统计);
        }
        else {
            私有_追加叶字段(字段节点, "节点类别", "仓库根");
            私有_追加叶字段(字段节点, "主信息类型", "空");
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加特征节点字段(
        结构_控制面板树节点& 字段节点,
        const 特征节点主信息类& 主信息)
    {
        私有_追加叶字段(
            字段节点,
            "抽象特征",
            私有_引用文本(主信息.抽象特征.指针, 主信息.抽象特征.主键));
        私有_追加叶字段(
            字段节点,
            "当前命中抽象特征",
            私有_引用文本(主信息.当前命中抽象特征.指针, 主信息.当前命中抽象特征.主键));
        私有_追加叶字段(字段节点, "当前值", 私有_特征值文本(主信息.当前值));
        私有_追加叶字段(字段节点, "当前稳态索引", 主信息.当前稳态索引);
        私有_追加叶字段(字段节点, "区间", 私有_可选区间文本(主信息.区间));
        私有_追加叶字段(字段节点, "局部区间覆盖", 私有_可选区间文本(主信息.局部区间覆盖));
        私有_追加叶字段(字段节点, "稳态数量", static_cast<std::uint64_t>(主信息.稳态列表.size()));
        私有_追加叶字段(
            字段节点,
            "连续测量误差区间",
            私有_区间文本(主信息.连续测量误差区间));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加抽象特征节点字段(
        结构_控制面板树节点& 字段节点,
        const 抽象特征主信息类& 主信息)
    {
        私有_追加叶字段(字段节点, "形态", 私有_抽象特征形态文本(主信息.形态));
        私有_追加叶字段(字段节点, "值域", 私有_特征值域文本(主信息.值域));
        私有_追加叶字段(字段节点, "兼容区间", 私有_可选区间文本(主信息.区间));
        私有_追加叶字段(字段节点, "来源实例特征数量", static_cast<std::uint64_t>(主信息.来源实例特征.size()));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加存在节点字段(
        结构_控制面板树节点& 字段节点,
        const 存在节点主信息类& 主信息,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径)
    {
        私有_追加引用字段(
            字段节点,
            "概念模板",
            主信息.概念模板,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用列表字段(
            字段节点,
            "概念集",
            主信息.概念集,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            32);
        私有_追加引用字段(
            字段节点,
            "内部世界",
            主信息.内部世界,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加指针字段(
            字段节点,
            "需求根节点",
            主信息.需求根节点,
            {},
            上下文,
            剩余深度,
            路径,
            [](需求节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建需求树节点(reinterpret_cast<需求节点*>(目标), 局部上下文, 深度, std::move(路径集), false);
            });
        私有_追加指针字段(
            字段节点,
            "任务根节点",
            主信息.任务根节点,
            {},
            上下文,
            剩余深度,
            路径,
            [](任务节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建任务树节点(reinterpret_cast<任务节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加指针字段(
            字段节点,
            "方法根节点",
            主信息.方法根节点,
            {},
            上下文,
            剩余深度,
            路径,
            [](方法节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建方法树节点(reinterpret_cast<方法节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加叶字段(字段节点, "有位置历史缓存", 主信息.有位置历史);
        私有_追加叶字段(字段节点, "上次观测位置缓存", 私有_向量文本(主信息.上次观测位置_mm));
        私有_追加叶字段(字段节点, "最近观测位置缓存", 私有_向量文本(主信息.最近观测位置_mm));
        私有_追加叶字段(字段节点, "最近位移缓存_mm", 主信息.最近位移_mm);
        私有_追加叶字段(字段节点, "连续静止帧缓存", static_cast<std::uint64_t>(主信息.连续静止帧));
        私有_追加叶字段(字段节点, "连续未命中帧缓存", static_cast<std::uint64_t>(主信息.连续未命中帧));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加场景节点字段(
        结构_控制面板树节点& 字段节点,
        const 场景节点主信息类& 主信息,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径)
    {
        私有_追加引用字段(
            字段节点,
            "宿主存在",
            主信息.宿主存在,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加叶字段(字段节点, "世界类型", 私有_世界类型文本(主信息.世界类型));
        私有_追加叶字段(字段节点, "来源文章", 主信息.来源文章 ? 主信息.来源文章->获取主键() : "空");
        std::vector<可解析引用<状态节点类>> 状态索引快照;
        std::vector<可解析引用<动态节点类>> 动态索引快照;
        std::vector<可解析引用<二次特征节点类>> 二次特征索引快照;
        {
            std::lock_guard<std::recursive_mutex> 锁(借用场景索引全局互斥());
            状态索引快照 = 主信息.状态索引;
            动态索引快照 = 主信息.动态索引;
            二次特征索引快照 = 主信息.二次特征索引;
        }
        私有_追加引用列表字段(
            字段节点,
            "状态索引",
            状态索引快照,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        私有_追加引用列表字段(
            字段节点,
            "动态索引",
            动态索引快照,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        私有_追加引用列表字段(
            字段节点,
            "二次特征索引",
            二次特征索引快照,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加状态节点字段(
        结构_控制面板树节点& 字段节点,
        const 状态节点主信息类& 主信息,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径)
    {
        私有_追加叶字段(字段节点, "收到时间", 私有_时间文本(主信息.收到时间));
        私有_追加叶字段(字段节点, "发生时间", 私有_时间文本(主信息.发生时间));
        私有_追加叶字段(字段节点, "状态值", 私有_特征值文本(主信息.状态值));
        私有_追加叶字段(字段节点, "是否变化缓存", 主信息.是否变化);
        私有_追加引用字段(
            字段节点,
            "状态主体",
            主信息.状态主体,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "状态特征",
            主信息.状态特征,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "对应信息节点",
            主信息.对应信息节点,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加动态节点字段(
        结构_控制面板树节点& 字段节点,
        const 动态节点主信息类& 主信息,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径)
    {
        私有_追加叶字段(字段节点, "时间段", 私有_时间段文本(主信息.时间));
        私有_追加叶字段(
            字段节点,
            "动态类型",
            私有_词文本(主信息.类型 ? 主信息.类型 : 动态类::推导动态类型(主信息)));
        私有_追加叶字段(字段节点, "来源动作名", 私有_词文本(主信息.来源动作名));
        私有_追加叶字段(字段节点, "来源执行成功", 主信息.来源执行成功);
        私有_追加叶字段(字段节点, "来源错误码", 主信息.来源错误码);
        私有_追加叶字段(字段节点, "动态路径签名", 主信息.动态路径签名);
        私有_追加叶字段(字段节点, "动态层级", static_cast<std::uint64_t>(主信息.动态层级));
        私有_追加叶字段(字段节点, "聚合方式", 私有_动态聚合方式文本(主信息.聚合方式));
        私有_追加引用字段(
            字段节点,
            "初始状态",
            主信息.初始状态,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "结果状态",
            主信息.结果状态,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "动态主体",
            主信息.动态主体,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "动态特征",
            主信息.动态特征,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "来源动作",
            主信息.来源动作,
            上下文,
            剩余深度,
            路径,
            [](方法节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建方法树节点(reinterpret_cast<方法节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "来源输入场景",
            主信息.来源输入场景,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "来源输出场景",
            主信息.来源输出场景,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用列表字段(
            字段节点,
            "来源低层动态",
            主信息.来源低层动态,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加二次特征字段(
        结构_控制面板树节点& 字段节点,
        const 二次特征主信息类& 主信息,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径)
    {
        私有_追加叶字段(字段节点, "度量签名_链键", 主信息.度量签名_链键);
        私有_追加叶字段(字段节点, "概念名称", 私有_词文本(主信息.概念名称));
        私有_追加叶字段(字段节点, "标量区间", 私有_可选区间文本(主信息.标量区间));
        私有_追加叶字段(字段节点, "标量值", 主信息.标量值);
        私有_追加叶字段(字段节点, "离散编码", static_cast<std::int64_t>(主信息.离散编码));
        私有_追加叶字段(字段节点, "离散语义键", 主信息.离散语义键);
        私有_追加叶字段(字段节点, "是否满足", 主信息.是否满足);
        私有_追加叶字段(字段节点, "分步时长", 私有_时间文本(主信息.分步时长));
        私有_追加叶字段(字段节点, "总时长", 私有_时间文本(主信息.总时长));
        私有_追加叶字段(字段节点, "窗口时长", 私有_时间文本(主信息.窗口时长));
        私有_追加叶字段(字段节点, "来源动态层级", static_cast<std::uint64_t>(主信息.来源动态层级));
        私有_追加叶字段(字段节点, "来源动态路径签名", 主信息.来源动态路径签名);
        私有_追加叶字段(字段节点, "来源时间段", 私有_时间段文本(主信息.来源时间段));
        私有_追加引用字段(
            字段节点,
            "概念模板",
            主信息.概念模板,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "所属场景",
            主信息.所属场景,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "来源起始状态",
            主信息.来源起始状态,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "来源结果状态",
            主信息.来源结果状态,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "来源动态",
            主信息.来源动态,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "来源场景",
            主信息.来源场景,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "主体",
            主信息.主体,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "客体",
            主信息.客体,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "左对象",
            主信息.左对象,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "右对象",
            主信息.右对象,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加因果字段(
        结构_控制面板树节点& 字段节点,
        const 因果主信息类& 主信息,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径)
    {
        私有_追加叶字段(字段节点, "适用锚点类型", 私有_因果锚点类型文本(主信息.适用锚点类型));
        私有_追加叶字段(字段节点, "适用层级", static_cast<std::uint64_t>(主信息.适用层级));
        私有_追加叶字段(字段节点, "因动作名称", 主信息.因动作名称);
        私有_追加叶字段(字段节点, "派生稳定度", 主信息.计算稳定度());
        私有_追加叶字段(字段节点, "已验证", 主信息.已验证);
        私有_追加叶字段(字段节点, "观察次数", 主信息.观察次数);
        私有_追加叶字段(字段节点, "条件命中次数", 主信息.条件命中次数);
        私有_追加叶字段(字段节点, "因出现次数", 主信息.因出现次数);
        私有_追加叶字段(字段节点, "果出现次数", 主信息.果出现次数);
        私有_追加叶字段(字段节点, "成立次数", 主信息.成立次数);
        私有_追加叶字段(字段节点, "失败次数", 主信息.失败次数);
        私有_追加叶字段(字段节点, "最近命中时间", 私有_时间文本(主信息.最近命中时间));
        私有_追加叶字段(字段节点, "最近失败时间", 私有_时间文本(主信息.最近失败时间));
        私有_追加引用字段(
            字段节点,
            "因方法模板",
            主信息.因方法模板,
            上下文,
            剩余深度,
            路径,
            [](方法节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建方法树节点(reinterpret_cast<方法节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用列表字段(
            字段节点,
            "条件比较模板",
            主信息.条件比较模板,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        私有_追加引用列表字段(
            字段节点,
            "果比较模板",
            主信息.果比较模板,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        私有_追加引用字段(
            字段节点,
            "主果比较模板",
            主信息.主果比较模板,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用列表字段(
            字段节点,
            "证据动态样本",
            主信息.证据动态样本,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
    }

    // 功能：构建运行所需的数据结构或中间结果。
    结构_控制面板树节点 私有_构建基础信息树节点(
        基础信息节点类* 节点,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        路径集合 路径)
    {
        if (!节点) {
            return 私有_新节点("空基础信息节点");
        }

        const auto 当前地址 = 私有_地址(节点);
        if (路径.contains(当前地址)) {
            return 私有_新节点("基础信息节点 | 路径=已在上层路径中", 当前地址, false);
        }
        路径.insert(当前地址);

        auto 树节点 = 私有_基础信息骨架节点(节点, 上下文, false);

        if (剩余深度 == 0) {
            return 树节点;
        }

        auto 字段节点 = 私有_新节点("节点字段");
        字段节点.是字段分组 = true;
        私有_追加基础信息通用字段(字段节点, 节点, 上下文, 剩余深度, 路径);

        if (const auto* 主信息 = 节点->主信息) {
            if (const auto* 抽象特征 = dynamic_cast<const 抽象特征主信息类*>(主信息)) {
                私有_追加抽象特征节点字段(字段节点, *抽象特征);
            }
            else if (const auto* 特征 = dynamic_cast<const 特征节点主信息类*>(主信息)) {
                私有_追加特征节点字段(字段节点, *特征);
            }
            else if (const auto* 存在 = dynamic_cast<const 存在节点主信息类*>(主信息)) {
                私有_追加存在节点字段(字段节点, *存在, 上下文, 剩余深度, 路径);
            }
            else if (const auto* 场景 = dynamic_cast<const 场景节点主信息类*>(主信息)) {
                私有_追加场景节点字段(字段节点, *场景, 上下文, 剩余深度, 路径);
            }
            else if (const auto* 状态 = dynamic_cast<const 状态节点主信息类*>(主信息)) {
                私有_追加状态节点字段(字段节点, *状态, 上下文, 剩余深度, 路径);
            }
            else if (const auto* 动态 = dynamic_cast<const 动态节点主信息类*>(主信息)) {
                私有_追加动态节点字段(字段节点, *动态, 上下文, 剩余深度, 路径);
            }
            else if (const auto* 二次特征 = dynamic_cast<const 二次特征主信息类*>(主信息)) {
                私有_追加二次特征字段(字段节点, *二次特征, 上下文, 剩余深度, 路径);
            }
            else if (const auto* 因果 = dynamic_cast<const 因果主信息类*>(主信息)) {
                私有_追加因果字段(字段节点, *因果, 上下文, 剩余深度, 路径);
            }
        }
        树节点.子项.push_back(std::move(字段节点));

        const auto 子节点集 = 私有_枚举子节点(节点, (std::numeric_limits<std::size_t>::max)());
        const auto 实际上限 = (std::min)(上下文.树广度上限, 子节点集.size());
        for (std::size_t 索引 = 0; 索引 < 实际上限; ++索引) {
            树节点.子项.push_back(私有_基础信息骨架节点(子节点集[索引], 上下文, false));
        }
        if (子节点集.size() > 实际上限) {
            树节点.子项.push_back(
                私有_创建结构省略节点(
                    "base-node-more",
                    私有_地址(节点),
                    实际上限,
                    子节点集.size() - 实际上限));
        }

        return 树节点;
    }

    // 功能：构建运行所需的数据结构或中间结果。
    结构_控制面板树节点 私有_构建因果信息树(
        const 结构_构建上下文& 上下文,
        const std::size_t 因果模板数,
        const std::size_t 证据动态样本数)
    {
        auto 根节点 = 私有_新节点(
            "因果信息 | 因果模板=" + std::to_string(因果模板数)
                + " | 证据动态样本=" + std::to_string(证据动态样本数),
            0,
            true);

        std::vector<基础信息节点类*> 因果根节点{};
        auto* 世界根节点 = 世界树.世界根();
        for (auto* 因果节点 : 世界树.枚举节点_按主信息类型<因果主信息类>()) {
            bool 有因果父祖先 = false;
            std::size_t 保护计数 = 0;
            for (auto* 父节点 = 世界树.获取父节点(因果节点);
                 父节点 && 父节点 != 世界根节点 && 保护计数 < 4096;
                 父节点 = 世界树.获取父节点(父节点), ++保护计数) {
                if (世界树.是因果节点(父节点)) {
                    有因果父祖先 = true;
                    break;
                }
                if (父节点 == 世界树.获取父节点(父节点)) {
                    break;
                }
            }
            if (!有因果父祖先) {
                因果根节点.push_back(因果节点);
            }
        }

        const auto 实际上限 = (std::min)(上下文.树广度上限, 因果根节点.size());
        for (std::size_t 索引 = 0; 索引 < 实际上限; ++索引) {
            根节点.子项.push_back(
                私有_基础信息骨架节点(因果根节点[索引], 上下文, false));
        }
        if (因果根节点.size() > 实际上限) {
            根节点.子项.push_back(
                私有_新节点("... 省略 " + std::to_string(因果根节点.size() - 实际上限) + " 项"));
        }

        if (因果根节点.empty()) {
            根节点.子项.clear();
            根节点.子项.push_back(私有_新节点("当前世界树根链未记录因果根节点"));
        }

        return 根节点;
    }

    // 功能：构建运行所需的数据结构或中间结果。
    结构_控制面板树节点 私有_构建需求树节点(
        需求节点* 节点,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        路径集合 路径,
        const bool 列表模式)
    {
        if (!节点) {
            return 私有_新节点("空需求节点");
        }

        const auto 当前地址 = 私有_地址(节点);
        if (路径.contains(当前地址)) {
            return 私有_新节点("需求节点 | 路径=已在上层路径中", 当前地址, false);
        }
        路径.insert(当前地址);

        auto 树节点 = 私有_需求结构骨架节点(节点, 上下文, false);

        if (剩余深度 == 0) {
            return 树节点;
        }

        auto 字段节点 = 私有_新节点("节点字段");
        字段节点.是字段分组 = true;
        私有_追加叶字段(字段节点, "主键", 私有_节点主键_控制面板(节点));
        私有_追加叶字段(字段节点, "节点地址", 私有_十六进制指针(私有_地址(节点)));
        私有_追加叶字段(字段节点, "描述信息", 私有_自然句文本(节点->主信息.描述信息));
        私有_追加叶字段(
            字段节点,
            "目标特征类型",
            私有_词文本(私有_需求目标特征类型_控制面板(节点)));
        私有_追加叶字段(字段节点, "需求有效截止", 私有_时间文本(节点->主信息.需求有效截止));
        私有_追加叶字段(字段节点, "安全权重", 节点->主信息.安全权重);
        私有_追加叶字段(字段节点, "服务权重", 节点->主信息.服务权重);
        const 语素入口节点类* 方法评估目标特征类型 = nullptr;
        std::vector<方法节点*> 方法评估候选方法{};
        std::size_t 方法评估可用方法数 = 0;
        std::size_t 方法评估同向候选方法数 = 0;
        std::size_t 方法评估可用同向方法数 = 0;
        std::size_t 方法评估有方法动作候选数 = 0;
        std::size_t 方法评估方向未明候选数 = 0;
        std::size_t 方法评估反向候选数 = 0;
        bool 方法评估有需求状态引用 = false;
        bool 方法评估需求状态已满足 = false;
        int 方法评估需求方向 = 2;
        枚举_需求方法可用性状态 方法评估状态 = 枚举_需求方法可用性状态::无需求状态;
        私有_评估需求方法可用性_控制面板(
            节点,
            方法评估目标特征类型,
            方法评估候选方法,
            方法评估可用方法数,
            方法评估同向候选方法数,
            方法评估可用同向方法数,
            方法评估有方法动作候选数,
            方法评估方向未明候选数,
            方法评估反向候选数,
            方法评估有需求状态引用,
            方法评估需求状态已满足,
            方法评估需求方向,
            方法评估状态);
        const auto 目标语义视图 = 私有_需求目标语义视图_控制面板(方法评估目标特征类型);
        const auto* 令牌记录 = 私有_查找OR组令牌记录_控制面板(节点);
        const auto 令牌状态文本 = 私有_OR组令牌状态文本_控制面板(节点, 目标语义视图, 令牌记录);
        const auto 令牌激活路径文本 = 私有_OR组令牌当前激活路径文本_控制面板(节点, 目标语义视图, 令牌记录);
        私有_追加叶字段(字段节点, "结构形态", 私有_需求结构形态文本_控制面板(节点));
        私有_追加叶字段(字段节点, "可进入普通候选方法筹办", 目标语义视图.可进入普通候选方法筹办);
        私有_追加叶字段(
            字段节点,
            "不可进入普通筹办原因",
            私有_普通筹办不可用原因_控制面板(节点, 目标语义视图));
        私有_追加叶字段(字段节点, "OR组令牌状态", 令牌状态文本);
        私有_追加叶字段(字段节点, "OR组当前激活路径", 令牌激活路径文本);
        私有_追加叶字段(字段节点, "方法可用性", 私有_需求方法可用性状态文本(方法评估状态));
        私有_追加叶字段(字段节点, "目标特征类型", 私有_词文本(方法评估目标特征类型));
        私有_追加叶字段(字段节点, "需求方向", 私有_方向文本_控制面板(方法评估需求方向));
        私有_追加叶字段(字段节点, "候选方法数量", static_cast<std::uint64_t>(方法评估候选方法.size()));
        私有_追加叶字段(字段节点, "可用方法数量", static_cast<std::uint64_t>(方法评估可用方法数));
        私有_追加叶字段(字段节点, "同向候选方法数量", static_cast<std::uint64_t>(方法评估同向候选方法数));
        私有_追加叶字段(字段节点, "可用同向方法数量", static_cast<std::uint64_t>(方法评估可用同向方法数));
        私有_追加叶字段(字段节点, "方向未明候选方法数量", static_cast<std::uint64_t>(方法评估方向未明候选数));
        私有_追加叶字段(字段节点, "反向候选方法数量", static_cast<std::uint64_t>(方法评估反向候选数));
        私有_追加叶字段(字段节点, "有方法动作候选方法数量", static_cast<std::uint64_t>(方法评估有方法动作候选数));
        私有_追加叶字段(字段节点, "需求状态已满足", 方法评估需求状态已满足);
        auto 候选方法列表节点 = 私有_新节点("候选方法 (" + std::to_string(方法评估候选方法.size()) + ")");
        const auto 候选方法实际上限 = (std::min)(上下文.树广度上限, 方法评估候选方法.size());
        for (std::size_t 索引 = 0; 索引 < 候选方法实际上限; ++索引) {
            auto* 方法 = 方法评估候选方法[索引];
            私有_追加指针字段(
                候选方法列表节点,
                std::to_string(索引),
                方法,
                方法 ? 方法->获取主键() : std::string{},
                上下文,
                剩余深度,
                路径,
                [](方法节点* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                    return 私有_构建方法树节点(目标, 局部上下文, 深度, std::move(路径集));
                });
        }
        if (方法评估候选方法.size() > 候选方法实际上限) {
            候选方法列表节点.子项.push_back(
                私有_新节点("... 省略 " + std::to_string(方法评估候选方法.size() - 候选方法实际上限) + " 项"));
        }
        字段节点.子项.push_back(std::move(候选方法列表节点));
        私有_追加统计字段(字段节点, 节点->主信息.统计);
        私有_追加指针字段(
            字段节点,
            "父需求",
            reinterpret_cast<需求节点*>(节点->父),
            {},
            上下文,
            剩余深度,
            路径,
            [列表模式](需求节点* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建需求树节点(目标, 局部上下文, 深度, std::move(路径集), 列表模式);
            });
        私有_追加引用字段(
            字段节点,
            "需求主体",
            节点->主信息.需求主体,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "需求场景",
            节点->主信息.需求场景,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "被需求存在",
            节点->主信息.被需求存在,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "需求状态",
            节点->主信息.需求状态,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用列表字段(
            字段节点,
            "任务列表",
            节点->主信息.任务列表,
            上下文,
            剩余深度,
            路径,
            [](任务节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建任务树节点(reinterpret_cast<任务节点*>(目标), 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        私有_追加引用字段(
            字段节点,
            "被需求当前状态",
            节点->主信息.被需求当前状态,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        树节点.子项.push_back(std::move(字段节点));

        if (!列表模式) {
            const auto 子节点集 = 私有_枚举子节点(节点, 上下文.树广度上限);
            for (auto* 子节点 : 子节点集) {
                树节点.子项.push_back(私有_需求结构骨架节点(子节点, 上下文, false));
            }
            if (static_cast<std::size_t>(节点->子节点数量) > 子节点集.size()) {
                树节点.子项.push_back(
                    私有_创建结构省略节点(
                        "need-node-more",
                        私有_地址(节点),
                        子节点集.size(),
                        static_cast<std::size_t>(节点->子节点数量) - 子节点集.size()));
            }
        }

        return 树节点;
    }

    // 功能：构建运行所需的数据结构或中间结果。
    结构_控制面板树节点 私有_构建任务树节点(
        任务节点* 节点,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        路径集合 路径)
    {
        if (!节点) {
            return 私有_新节点("空任务节点");
        }

        const auto 当前地址 = 私有_地址(节点);
        if (路径.contains(当前地址)) {
            return 私有_新节点("任务节点 | 路径=已在上层路径中", 当前地址, false);
        }
        路径.insert(当前地址);

        auto 树节点 = 私有_任务骨架节点(节点, 上下文, false);

        if (剩余深度 == 0) {
            return 树节点;
        }

        auto 字段节点 = 私有_新节点("节点字段");
        字段节点.是字段分组 = true;
        私有_追加叶字段(字段节点, "主键", 节点->获取主键());
        私有_追加叶字段(字段节点, "节点地址", 私有_十六进制指针(私有_地址(节点)));
        私有_追加叶字段(字段节点, "名称", 私有_词文本(节点->主信息.名称));
        私有_追加叶字段(字段节点, "类型", 私有_词文本(节点->主信息.类型));
        私有_追加叶字段(字段节点, "节点种类", 私有_任务节点种类文本(任务类::读取任务节点种类(节点)));
        私有_追加叶字段(字段节点, "状态", 私有_任务状态文本(任务类::读取任务状态(节点)));
        私有_追加叶字段(字段节点, "创建时间", 私有_时间文本(节点->主信息.创建时间));
        私有_追加叶字段(字段节点, "基准优先级", 任务类::读取任务基准优先级(节点));
        私有_追加叶字段(字段节点, "局部优先级偏移", 任务类::读取任务局部优先级偏移(节点));
        私有_追加叶字段(字段节点, "调度优先级", 任务类::读取任务调度优先级(节点));
        私有_追加叶字段(字段节点, "运行次数", 任务类::读取任务运行次数(节点));
        私有_追加叶字段(字段节点, "成功次数", 任务类::读取任务成功次数(节点));
        私有_追加叶字段(字段节点, "失败次数", 任务类::读取任务失败次数(节点));
        私有_追加叶字段(字段节点, "连续失败次数", 任务类::读取任务连续失败次数(节点));
        私有_追加叶字段(字段节点, "转入缺口需求次数", 任务类::读取任务转入缺口需求次数(节点));
        私有_追加叶字段(字段节点, "最近错误码", 任务类::读取任务最近错误码(节点));
        私有_追加叶字段(字段节点, "启动时间", 私有_时间文本(节点->主信息.启动时间));
        私有_追加叶字段(字段节点, "完成时间", 私有_时间文本(节点->主信息.完成时间));

        私有_追加指针字段(
            字段节点,
            "父任务结构父节点",
            reinterpret_cast<任务节点*>(节点->父),
            {},
            上下文,
            剩余深度,
            路径,
            [](任务节点* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建任务树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "场景",
            节点->主信息.场景,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "任务虚拟存在",
            节点->主信息.任务虚拟存在,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "来源需求",
            节点->主信息.对应需求,
            上下文,
            剩余深度,
            路径,
            [](需求节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建需求树节点(reinterpret_cast<需求节点*>(目标), 局部上下文, 深度, std::move(路径集), false);
            });
        可解析引用<方法节点类> 执行方法引用{};
        if (auto* 执行方法 = 任务类::读取任务执行方法(节点)) {
            执行方法引用.绑定(执行方法);
        }
        私有_追加引用字段(
            字段节点,
            "执行方法",
            执行方法引用,
            上下文,
            剩余深度,
            路径,
            [](方法节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建方法树节点(reinterpret_cast<方法节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "目标状态",
            节点->主信息.目标状态,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "实际结果状态",
            节点->主信息.实际结果状态,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });

        私有_追加叶字段(字段节点, "等待缺口唤醒", 任务类::读取任务等待缺口唤醒(节点));
        树节点.子项.push_back(std::move(字段节点));

        const auto 子节点全集 = 私有_筛选需求任务链节点(
            私有_枚举子节点(节点, (std::numeric_limits<std::size_t>::max)()));
        const auto 子节点数量 = (std::min)(子节点全集.size(), 上下文.树广度上限);
        std::vector<任务节点*> 子节点集{};
        子节点集.reserve(子节点数量);
        for (std::size_t i = 0; i < 子节点数量; ++i) {
            子节点集.push_back(子节点全集[i]);
        }
        for (auto* 子节点 : 子节点集) {
            树节点.子项.push_back(私有_任务骨架节点(子节点, 上下文, false));
        }
        if (子节点全集.size() > 子节点集.size()) {
            树节点.子项.push_back(
                私有_创建结构省略节点(
                    "task-node-more",
                    私有_地址(节点),
                    子节点集.size(),
                    子节点全集.size() - 子节点集.size()));
        }

        return 树节点;
    }

    // 功能：构建运行所需的数据结构或中间结果。
    结构_控制面板树节点 私有_构建方法树节点(
        方法节点* 节点,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        路径集合 路径)
    {
        if (!节点) {
            return 私有_新节点("空方法节点");
        }

        const auto 当前地址 = 私有_地址(节点);
        if (路径.contains(当前地址)) {
            return 私有_新节点("方法节点 | 路径=已在上层路径中", 当前地址, false);
        }
        路径.insert(当前地址);

        auto 树节点 = 私有_方法骨架节点(节点, 上下文, false);

        if (剩余深度 == 0) {
            return 树节点;
        }

        auto 字段节点 = 私有_新节点("节点字段");
        字段节点.是字段分组 = true;
        私有_追加叶字段(字段节点, "主键", 节点->获取主键());
        私有_追加叶字段(字段节点, "节点地址", 私有_十六进制指针(私有_地址(节点)));
        if (方法类::方法是首节点(节点)) {
            std::size_t 条件数 = 0;
            std::size_t 直接结果数 = 0;
            std::size_t 条件下结果数 = 0;
            std::size_t 条件无结果数 = 0;
            私有_统计方法直接条件结果_控制面板(
                节点,
                条件数,
                直接结果数,
                条件下结果数,
                条件无结果数);
            私有_追加叶字段(字段节点, "直接条件节点数量", static_cast<I64>(条件数));
            私有_追加叶字段(字段节点, "条件下结果节点数量", static_cast<I64>(条件下结果数));
            私有_追加叶字段(字段节点, "直接结果节点数量", static_cast<I64>(直接结果数));
            私有_追加叶字段(
                字段节点,
                "条件结果是否成对",
                直接结果数 == 0 && 条件无结果数 == 0);
        }
        const auto* 条件信息 = 节点->主信息.取条件节点信息();
        const auto* 结果信息 = 节点->主信息.取结果节点信息();
        私有_追加叶字段(字段节点, "描述信息", 私有_自然句文本(节点->主信息.公共.描述信息));
        私有_追加叶字段(字段节点, "节点种类", 私有_方法节点种类文本(方法类::方法节点种类(节点)));
        私有_追加叶字段(字段节点, "动作名", 私有_词文本(方法类::方法动作名(节点)));
        私有_追加叶字段(字段节点, "动作句柄", 私有_动作句柄文本(方法类::方法动作句柄(节点)));
        私有_追加叶字段(字段节点, "来源", 私有_方法来源文本(方法类::方法来源(节点)));
        私有_追加叶字段(
            字段节点,
            "最近执行时间",
            私有_时间文本(私有_读取方法运行账时间_控制面板(
                节点,
                私有_特征_最近执行时间_控制面板())));
        私有_追加叶字段(
            字段节点,
            "最近完成时间",
            私有_时间文本(私有_读取方法运行账时间_控制面板(
                节点,
                私有_特征_最近完成时间_控制面板())));
        私有_追加叶字段(字段节点, "条件主键", 条件信息 ? 条件信息->条件主键 : std::string{});
        私有_追加叶字段(字段节点, "时间起", 私有_时间文本(条件信息 ? 条件信息->时间起 : 0));
        私有_追加叶字段(字段节点, "时间止", 私有_时间文本(条件信息 ? 条件信息->时间止 : 0));
        私有_追加叶字段(字段节点, "条件场景模板主键", 条件信息 ? 条件信息->条件场景模板主键 : std::string{});
        私有_追加叶字段(字段节点, "首个结果能力特征类型", 私有_词文本(方法类::方法首个结果能力特征类型(节点)));
        私有_追加叶字段(字段节点, "结果主键", 结果信息 ? 结果信息->结果主键 : std::string{});
        私有_追加统计字段(字段节点, 节点->主信息.公共.统计);

        私有_追加指针字段(
            字段节点,
            "父方法结构父节点",
            reinterpret_cast<方法节点*>(节点->父),
            {},
            上下文,
            剩余深度,
            路径,
            [](方法节点* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建方法树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "方法虚拟存在",
            私有_方法虚拟存在引用_控制面板(节点),
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "条件场景",
            条件信息 ? 条件信息->条件场景 : 可解析引用<场景节点类>{},
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "结果场景",
            结果信息 ? 结果信息->结果场景 : 可解析引用<场景节点类>{},
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "来源任务",
            方法类::方法来源任务引用(节点),
            上下文,
            剩余深度,
            路径,
            [](任务节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建任务树节点(reinterpret_cast<任务节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "父方法",
            方法类::方法父方法引用(节点),
            上下文,
            剩余深度,
            路径,
            [](方法节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建方法树节点(reinterpret_cast<方法节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "前置方法",
            方法类::方法前置方法引用(节点),
            上下文,
            剩余深度,
            路径,
            [](方法节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建方法树节点(reinterpret_cast<方法节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "后续方法",
            方法类::方法后续方法引用(节点),
            上下文,
            剩余深度,
            路径,
            [](方法节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建方法树节点(reinterpret_cast<方法节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用列表字段(
            字段节点,
            "结果初始状态",
            结果信息 ? 结果信息->结果初始状态 : std::vector<可解析引用<状态节点类>>{},
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        私有_追加引用列表字段(
            字段节点,
            "条件判定索引",
            条件信息 ? 条件信息->条件判定索引 : std::vector<可解析引用<二次特征节点类>>{},
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        树节点.子项.push_back(std::move(字段节点));

        const auto 子节点集 = 私有_枚举子节点(节点, 上下文.树广度上限);
        for (auto* 子节点 : 子节点集) {
            树节点.子项.push_back(私有_方法骨架节点(子节点, 上下文, false));
        }
        if (static_cast<std::size_t>(节点->子节点数量) > 子节点集.size()) {
            树节点.子项.push_back(
                私有_创建结构省略节点(
                    "method-node-more",
                    私有_地址(节点),
                    子节点集.size(),
                    static_cast<std::size_t>(节点->子节点数量) - 子节点集.size()));
        }

        return 树节点;
    }

    // 功能：构建基础信息树根层骨架，只预置一层直接子节点。
    结构_控制面板树节点 私有_构建基础信息根链骨架(
        基础信息节点类* 节点,
        const 结构_构建上下文& 上下文)
    {
        if (!节点) {
            return 私有_新节点("空基础信息节点");
        }

        auto 树节点 = 私有_基础信息骨架节点(
            节点,
            上下文,
            !节点->父);

        const auto 子节点集 = 私有_枚举子节点(节点, (std::numeric_limits<std::size_t>::max)());
        const auto 实际上限 = (std::min)(上下文.树广度上限, 子节点集.size());
        for (std::size_t 索引 = 0; 索引 < 实际上限; ++索引) {
            auto* 子节点 = 子节点集[索引];
            if (!子节点) {
                continue;
            }
            树节点.子项.push_back(私有_基础信息骨架节点(子节点, 上下文, false));
        }
        if (子节点集.size() > 实际上限) {
            树节点.子项.push_back(
                私有_创建结构省略节点(
                    "base-node-more",
                    私有_地址(节点),
                    实际上限,
                    子节点集.size() - 实际上限));
        }
        return 树节点;
    }

    // 功能：构建需求树根层骨架，只预置一层直接子节点。
    结构_控制面板树节点 私有_构建需求根链骨架(
        需求节点* 节点,
        const 结构_构建上下文& 上下文)
    {
        if (!节点) {
            return 私有_新节点("空需求节点");
        }

        auto 树节点 = 私有_需求结构骨架节点(
            节点,
            上下文,
            !节点->父);

        const auto 子节点集 = 私有_枚举子节点(节点, 上下文.树广度上限);
        for (auto* 子节点 : 子节点集) {
            if (!子节点) {
                continue;
            }
            树节点.子项.push_back(私有_需求结构骨架节点(子节点, 上下文, false));
        }
        if (static_cast<std::size_t>(节点->子节点数量) > 子节点集.size()) {
            树节点.子项.push_back(
                私有_创建结构省略节点(
                    "need-node-more",
                    私有_地址(节点),
                    子节点集.size(),
                    static_cast<std::size_t>(节点->子节点数量) - 子节点集.size()));
        }
        return 树节点;
    }

    // 功能：构建任务树根层骨架，只预置一层直接子节点。
    结构_控制面板树节点 私有_构建任务根链骨架(
        任务节点* 节点,
        const 结构_构建上下文& 上下文)
    {
        if (!节点) {
            return 私有_新节点("空任务节点");
        }

        auto 树节点 = 私有_任务骨架节点(
            节点,
            上下文,
            !节点->父);

        const auto 子节点全集 = 私有_筛选需求任务链节点(
            私有_枚举子节点(节点, (std::numeric_limits<std::size_t>::max)()));
        const auto 子节点数量 = (std::min)(子节点全集.size(), 上下文.树广度上限);
        std::vector<任务节点*> 子节点集{};
        子节点集.reserve(子节点数量);
        for (std::size_t i = 0; i < 子节点数量; ++i) {
            子节点集.push_back(子节点全集[i]);
        }
        for (auto* 子节点 : 子节点集) {
            if (!子节点) {
                continue;
            }
            树节点.子项.push_back(私有_任务骨架节点(子节点, 上下文, false));
        }
        if (子节点全集.size() > 子节点集.size()) {
            树节点.子项.push_back(
                私有_创建结构省略节点(
                    "task-node-more",
                    私有_地址(节点),
                    子节点集.size(),
                    子节点全集.size() - 子节点集.size()));
        }
        return 树节点;
    }

    // 功能：构建方法树根层骨架，只预置一层直接子节点。
    结构_控制面板树节点 私有_构建方法根链骨架(
        方法节点* 节点,
        const 结构_构建上下文& 上下文)
    {
        if (!节点) {
            return 私有_新节点("空方法节点");
        }

        auto 树节点 = 私有_方法骨架节点(
            节点,
            上下文,
            !节点->父);

        const auto 子节点集 = 私有_枚举子节点(节点, 上下文.树广度上限);
        for (auto* 子节点 : 子节点集) {
            if (!子节点) {
                continue;
            }
            树节点.子项.push_back(私有_方法骨架节点(子节点, 上下文, false));
        }
        if (static_cast<std::size_t>(节点->子节点数量) > 子节点集.size()) {
            树节点.子项.push_back(
                私有_创建结构省略节点(
                    "method-node-more",
                    私有_地址(节点),
                    子节点集.size(),
                    static_cast<std::size_t>(节点->子节点数量) - 子节点集.size()));
        }
        return 树节点;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_计入线程生命周期统计(
        std::size_t& 已知逻辑线程数,
        std::size_t& 当前运行数,
        std::size_t& 未启动数,
        std::size_t& 已结束数,
        std::size_t& 故障数,
        const 枚举_线程生命周期状态 状态) noexcept
    {
        ++已知逻辑线程数;
        switch (状态) {
        case 枚举_线程生命周期状态::未启动:
            ++未启动数;
            break;
        case 枚举_线程生命周期状态::启动中:
        case 枚举_线程生命周期状态::运行中:
        case 枚举_线程生命周期状态::停止请求中:
        case 枚举_线程生命周期状态::收尾中:
            ++当前运行数;
            break;
        case 枚举_线程生命周期状态::已停止:
            ++已结束数;
            break;
        case 枚举_线程生命周期状态::故障:
            ++故障数;
            break;
        default:
            ++未启动数;
            break;
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_计入摘要线程生命周期统计(
        std::size_t& 已知逻辑线程数,
        std::size_t& 当前运行数,
        std::size_t& 未启动数,
        std::size_t& 已结束数,
        std::size_t& 故障数,
        const 枚举_控制面板摘要线程生命周期状态 状态) noexcept
    {
        ++已知逻辑线程数;
        switch (状态) {
        case 枚举_控制面板摘要线程生命周期状态::未启动:
            ++未启动数;
            break;
        case 枚举_控制面板摘要线程生命周期状态::启动中:
        case 枚举_控制面板摘要线程生命周期状态::运行中:
        case 枚举_控制面板摘要线程生命周期状态::停止请求中:
            ++当前运行数;
            break;
        case 枚举_控制面板摘要线程生命周期状态::已停止:
            ++已结束数;
            break;
        case 枚举_控制面板摘要线程生命周期状态::故障:
            ++故障数;
            break;
        default:
            ++未启动数;
            break;
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_计入布尔线程统计(
        std::size_t& 已知逻辑线程数,
        std::size_t& 当前运行数,
        std::size_t& 未启动数,
        std::size_t& 故障数,
        const bool 已启动,
        const bool 正在运行,
        const bool 故障) noexcept
    {
        ++已知逻辑线程数;
        if (故障) {
            ++故障数;
        }
        else if (已启动 || 正在运行) {
            ++当前运行数;
        }
        else {
            ++未启动数;
        }
    }

    // 功能：计算权重、状态、差值或派生结果。
    void 私有_计算线程状态页统计(
        const 结构_控制面板快照& 快照,
        std::size_t& 已知逻辑线程数,
        std::size_t& 当前运行数,
        std::size_t& 未启动数,
        std::size_t& 已结束数,
        std::size_t& 故障数) noexcept
    {
        私有_计入线程生命周期统计(
            已知逻辑线程数,
            当前运行数,
            未启动数,
            已结束数,
            故障数,
            static_cast<枚举_线程生命周期状态>(快照.自我线程生命周期));
        私有_计入布尔线程统计(
            已知逻辑线程数,
            当前运行数,
            未启动数,
            故障数,
            快照.任务管理界面线程已启动,
            快照.任务管理界面线程正在处理,
            快照.任务管理界面线程故障);
        私有_计入布尔线程统计(
            已知逻辑线程数,
            当前运行数,
            未启动数,
            故障数,
            快照.任务管理工作线程已启动,
            快照.任务管理工作线程正在执行,
            false);
        私有_计入摘要线程生命周期统计(
            已知逻辑线程数,
            当前运行数,
            未启动数,
            已结束数,
            故障数,
            static_cast<枚举_控制面板摘要线程生命周期状态>(
                快照.控制面板摘要线程生命周期));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_计入线程信息项统计(
        std::size_t& 已知逻辑线程数,
        std::size_t& 当前运行数,
        std::size_t& 未启动数,
        std::size_t& 已结束数,
        std::size_t& 故障数,
        const 结构_控制面板线程信息项& 线程) noexcept
    {
        ++已知逻辑线程数;
        if (线程.是否故障) {
            ++故障数;
            return;
        }
        if (线程.是否已退出) {
            ++已结束数;
            return;
        }

        if (线程.生命周期状态值 != 线程生命周期状态值_未设置) {
            switch (static_cast<枚举_线程生命周期状态>(线程.生命周期状态值)) {
            case 枚举_线程生命周期状态::未启动:
                ++未启动数;
                break;
            case 枚举_线程生命周期状态::启动中:
            case 枚举_线程生命周期状态::运行中:
            case 枚举_线程生命周期状态::停止请求中:
            case 枚举_线程生命周期状态::收尾中:
                ++当前运行数;
                break;
            case 枚举_线程生命周期状态::已停止:
                ++已结束数;
                break;
            case 枚举_线程生命周期状态::故障:
                ++故障数;
                break;
            default:
                ++当前运行数;
                break;
            }
            return;
        }

        if (!线程.创建消息已到) {
            ++未启动数;
        }
        else {
            ++当前运行数;
        }
    }

    // 功能：计算权重、状态、差值或派生结果。
    void 私有_计算线程信息表统计(
        const std::vector<结构_控制面板线程信息项>& 线程信息项集,
        std::size_t& 已知逻辑线程数,
        std::size_t& 当前运行数,
        std::size_t& 未启动数,
        std::size_t& 已结束数,
        std::size_t& 故障数) noexcept
    {
        for (const auto& 线程 : 线程信息项集) {
            私有_计入线程信息项统计(
                已知逻辑线程数,
                当前运行数,
                未启动数,
                已结束数,
                故障数,
                线程);
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_线程状态统计文本(
        const std::size_t 已知逻辑线程数,
        const std::size_t 当前运行数,
        const std::size_t 未启动数,
        const std::size_t 已结束数,
        const std::size_t 故障数)
    {
        std::ostringstream 输出;
        输出 << "已知项目线程=" << 已知逻辑线程数
            << " | 当前运行=" << 当前运行数
            << " | 未启动=" << 未启动数
            << " | 已结束=" << 已结束数
            << " | 故障=" << 故障数;
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_线程信息项系统线程ID文本(
        const 结构_控制面板线程信息项& 线程)
    {
        return 线程.系统线程ID == 0
            ? std::string("待绑定")
            : std::to_string(线程.系统线程ID);
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_线程信息项状态文本(
        const 结构_控制面板线程信息项& 线程)
    {
        if (!线程.生命周期状态.empty()) {
            return 线程.生命周期状态;
        }
        return 线程.创建消息已到 ? std::string("创建消息已到") : std::string("未记录");
    }

    // 功能：执行对应模块、线程或方法的运行逻辑。
    std::string 私有_线程信息项运行状态文本(
        const 结构_控制面板线程信息项& 线程)
    {
        if (!线程.运行状态.empty()) {
            return 线程.运行状态;
        }
        if (线程.是否故障) {
            return "故障";
        }
        if (线程.是否已退出) {
            return "已退出";
        }
        if (线程.是否堵塞) {
            return "堵塞/等待";
        }
        if (线程.是否暂停) {
            return "暂停";
        }
        return 线程.是否健康 ? std::string("健康") : std::string("异常或未记录");
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_控制面板任务工作线程显示文本(std::string 文本)
    {
        const auto 替换全部 = [&文本](const std::string& 原文, const std::string& 替换) {
            std::size_t 位置 = 0;
            while ((位置 = 文本.find(原文, 位置)) != std::string::npos) {
                文本.replace(位置, 原文.size(), 替换);
                位置 += 替换.size();
            }
        };

        替换全部("创建和管理任务管理后台 worker 线程", "创建和管理任务工作线程");
        替换全部("任务管理后台 worker 线程", "任务工作线程");
        替换全部("任务管理后台worker", "任务工作线程");
        替换全部("后台 worker", "任务工作线程");
        替换全部("后台worker", "任务工作线程");
        替换全部("worker池", "任务工作线程池");
        return 文本;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_线程信息项最近事件文本(
        const 结构_控制面板线程信息项& 线程)
    {
        std::ostringstream 输出;
        输出 << (线程.最近事件类型.empty() ? "线程生命周期消息" : 线程.最近事件类型);
        if (!线程.最近事件键.empty()) {
            输出 << " | 事件键=" << 线程.最近事件键;
        }
        if (!线程.最近事件摘要.empty()) {
            输出 << " | " << 线程.最近事件摘要;
        }
        if (线程.迟到消息数 > 0) {
            输出 << " | 迟到消息=" << 线程.迟到消息数;
        }
        return 私有_控制面板任务工作线程显示文本(输出.str());
    }

    void 私有_追加线程显示详情(
        结构_控制面板树节点& 节点,
        const std::string& 线程名称,
        const std::string& 线程用途,
        const std::string& 线程类别,
        const std::string& 逻辑ID,
        const std::string& 系统线程ID,
        const std::string& 状态,
        const std::string& 运行状态,
        const std::string& 加载时间,
        const std::string& 运行时长,
        时间戳 详情加载时间,
        const std::string& 最近事件,
        const std::string& 说明);

    // 功能：构建运行所需的数据结构或中间结果。
    结构_控制面板树节点 私有_构建线程信息项节点(
        const 结构_控制面板线程信息项& 线程,
        const 时间戳 详情加载时间)
    {
        const auto 线程名称 = 私有_控制面板任务工作线程显示文本(
            线程.线程名称.empty() ? 线程.线程逻辑ID : 线程.线程名称);
        const auto 生命周期 = 私有_线程信息项状态文本(线程);
        const auto 运行状态 = 私有_线程信息项运行状态文本(线程);
        auto 节点 = 私有_新节点(
            线程名称
                + " | 生命周期=" + 生命周期
                + " | 运行=" + 运行状态
                + " | 更新=" + (线程.最近更新时间 == 0
                    ? std::string("未记录")
                    : 私有_时间文本(线程.最近更新时间)),
            0,
            false);

        const auto 结束时间 =
            (线程.是否已退出 || 线程.是否故障) && 线程.退出时间 != 0
                ? 线程.退出时间
                : 详情加载时间;
        私有_追加线程显示详情(
            节点,
            线程名称,
            私有_控制面板任务工作线程显示文本(
                线程.线程用途.empty() ? std::string("未记录") : 线程.线程用途),
            线程.线程类别.empty() ? std::string("项目线程") : 线程.线程类别,
            线程.线程逻辑ID,
            私有_线程信息项系统线程ID文本(线程),
            生命周期,
            运行状态,
            线程.创建时间 == 0 ? std::string("未记录") : 私有_时间文本(线程.创建时间),
            私有_起点时长文本(线程.创建时间, 结束时间),
            详情加载时间,
            私有_截断文本(私有_线程信息项最近事件文本(线程), 180),
            "来源=消息中间件/控制面板线程生命周期消息表；该表只服务显示和诊断，不作为任务状态、需求、世界事实或动作动态。");
        if (!节点.详情.empty()) {
            auto& 字段节点 = 节点.详情.back();
            私有_追加叶字段(字段节点, "所属模块", 线程.所属模块.empty() ? std::string("未记录") : 线程.所属模块);
            私有_追加叶字段(字段节点, "创建者逻辑ID", 线程.创建者逻辑ID.empty() ? std::string("未记录") : 线程.创建者逻辑ID);
            私有_追加叶字段(
                字段节点,
                "创建者名称",
                私有_控制面板任务工作线程显示文本(
                    线程.创建者名称.empty() ? std::string("未记录") : 线程.创建者名称));
            私有_追加叶字段(字段节点, "所属线程池ID", 线程.所属线程池ID.empty() ? std::string("未记录") : 线程.所属线程池ID);
            私有_追加叶字段(
                字段节点,
                "所属线程池名称",
                私有_控制面板任务工作线程显示文本(
                    线程.所属线程池名称.empty() ? std::string("未记录") : 线程.所属线程池名称));
            私有_追加叶字段(字段节点, "版本", std::to_string(线程.版本));
        }
        return 节点;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加线程显示详情(
        结构_控制面板树节点& 节点,
        const std::string& 线程名称,
        const std::string& 线程用途,
        const std::string& 线程类别,
        const std::string& 逻辑ID,
        const std::string& 系统线程ID,
        const std::string& 状态,
        const std::string& 运行状态,
        const std::string& 加载时间,
        const std::string& 运行时长,
        const 时间戳 详情加载时间,
        const std::string& 最近事件,
        const std::string& 说明)
    {
        auto 字段节点 = 私有_新节点("线程详细信息");
        私有_追加叶字段(字段节点, "线程名称", 私有_控制面板任务工作线程显示文本(线程名称));
        私有_追加叶字段(字段节点, "线程用途", 私有_控制面板任务工作线程显示文本(线程用途));
        私有_追加叶字段(字段节点, "线程类别", 线程类别);
        私有_追加叶字段(字段节点, "逻辑ID", 逻辑ID);
        私有_追加叶字段(字段节点, "系统线程ID", 系统线程ID);
        私有_追加叶字段(字段节点, "状态", 状态);
        私有_追加叶字段(字段节点, "运行状态", 运行状态);
        私有_追加叶字段(字段节点, "加载时间", 加载时间);
        私有_追加叶字段(字段节点, "运行时长", 运行时长);
        私有_追加叶字段(字段节点, "详情加载时间", 私有_时间文本(详情加载时间));
        私有_追加叶字段(字段节点, "最近事件", 私有_控制面板任务工作线程显示文本(最近事件));
        私有_追加叶字段(字段节点, "说明", 私有_控制面板任务工作线程显示文本(说明));
        节点.详情.push_back(std::move(字段节点));
    }

    // 功能：构建运行所需的数据结构或中间结果。
    结构_控制面板树节点 私有_构建线程状态树(
        const 结构_控制面板快照& 快照,
        const 任务管理线程协议::结构_任务界面线程快照& 界面线程快照,
        const 任务管理工作线程::结构_工作线程实例快照& 工作线程快照)
    {
        const auto 详情加载时间 = 结构体_时间戳::当前_微秒();
        const auto 线程信息项集 = 消费并读取控制面板线程信息表();
        const bool 使用消息线程信息表 = !线程信息项集.empty();
        std::size_t 线程已知逻辑线程数 = 0;
        std::size_t 线程当前运行数 = 0;
        std::size_t 线程未启动数 = 0;
        std::size_t 线程已结束数 = 0;
        std::size_t 线程故障数 = 0;
        if (使用消息线程信息表) {
            私有_计算线程信息表统计(
                线程信息项集,
                线程已知逻辑线程数,
                线程当前运行数,
                线程未启动数,
                线程已结束数,
                线程故障数);
        }
        else {
            私有_计算线程状态页统计(
                快照,
                线程已知逻辑线程数,
                线程当前运行数,
                线程未启动数,
                线程已结束数,
                线程故障数);
        }
        const auto 统计文本 = 私有_线程状态统计文本(
            线程已知逻辑线程数,
            线程当前运行数,
            线程未启动数,
            线程已结束数,
            线程故障数);
        auto 根节点 = 私有_新节点(
            "线程列表 | " + 统计文本,
            0,
            true);
        私有_追加线程显示详情(
            根节点,
            "项目线程状态统计",
            "控制面板线程状态页上部统计，仅统计项目自身创建并登记的线程",
            "统计",
            "thread-status-summary",
            "不适用",
            "统计",
            统计文本,
            私有_时间文本(详情加载时间),
            "不适用",
            详情加载时间,
            "只显示项目线程，不显示系统线程",
            使用消息线程信息表
                ? "中间栏来自 消息中间件 消费后的控制面板线程生命周期消息表；不显示系统线程枚举。"
                : "尚未消费到线程生命周期消息，暂用项目线程快照占位；不显示系统线程枚举。");

        if (使用消息线程信息表) {
            for (const auto& 线程 : 线程信息项集) {
                根节点.子项.push_back(私有_构建线程信息项节点(线程, 详情加载时间));
            }
            return 根节点;
        }

        auto 自我线程节点 = 私有_新节点(
            std::string("自我线程 | 生命周期=")
                + 私有_线程生命周期文本(static_cast<枚举_线程生命周期状态>(快照.自我线程生命周期))
                + " | 阶段=" + (快照.自我线程当前阶段.empty() ? std::string("暂无数据") : 快照.自我线程当前阶段)
                + " | Tick=" + std::to_string(快照.自我Tick计数),
            私有_线程详情_自我,
            false,
            false,
            "thread-self");
        私有_追加线程显示详情(
            自我线程节点,
            "自我线程",
            "自我主循环、需求树治理和任务发起意图",
            "逻辑线程",
            "self-main",
            "待绑定",
            私有_线程生命周期文本(static_cast<枚举_线程生命周期状态>(快照.自我线程生命周期)),
            快照.自我线程当前阶段.empty() ? std::string("暂无数据") : 快照.自我线程当前阶段,
            "待线程生命周期消息接入",
            "待线程生命周期消息接入",
            详情加载时间,
            std::string("Tick=") + std::to_string(快照.自我Tick计数),
            "当前快照只能提供生命周期、阶段和 Tick；真实加载时间 / 连续运行时长由线程信息消息表接入后显示。");
        根节点.子项.push_back(std::move(自我线程节点));

        auto 界面线程节点 = 私有_新节点(
            std::string("任务管理界面线程 | 启动=")
                + 布尔文本_是或否(界面线程快照.已启动)
                + " | 处理=" + std::string(布尔文本_是或否(界面线程快照.正在处理))
                + " | 请求=" + std::to_string(界面线程快照.累计接收请求数)
                + " | 巡检=" + std::to_string(界面线程快照.最近巡检任务数)
                + "/" + std::to_string(界面线程快照.最近巡检入队任务数)
                + " | 等待工作项=" + std::to_string(界面线程快照.当前等待工作项数)
                + " | 上行=" + std::to_string(界面线程快照.当前上行队列长度),
            私有_线程详情_任务界面,
            false,
            false,
            "thread-task-interface");
        私有_追加线程显示详情(
            界面线程节点,
            "任务管理界面线程",
            "任务创建承接、任务虚拟存在维护、工作项调度和任务治理上行",
            "逻辑线程",
            "task-interface",
            "待绑定",
            界面线程快照.故障
                ? std::string("故障")
                : (界面线程快照.已启动 ? std::string("运行中") : std::string("未启动")),
            界面线程快照.正在处理 ? std::string("正在处理") : std::string("空闲或未启动"),
            "待线程生命周期消息接入",
            "待线程生命周期消息接入",
            详情加载时间,
            std::string("请求=") + std::to_string(界面线程快照.累计接收请求数),
            "当前快照提供队列、巡检和上行计数；真实线程创建时间由控制面板线程信息模块补齐。");
        根节点.子项.push_back(std::move(界面线程节点));

        auto 工作线程节点 = 私有_新节点(
            std::string("任务管理工作线程 | 启动=")
                + 布尔文本_是或否(工作线程快照.已启动)
                + " | 执行=" + std::string(布尔文本_是或否(工作线程快照.正在执行))
                + " | 排队=" + std::to_string(工作线程快照.当前排队数)
                + " | 推进=" + std::to_string(工作线程快照.累计推进次数),
            私有_线程详情_工作,
            false,
            false,
            "thread-worker");
        const auto 工作线程结束时间 =
            工作线程快照.正在执行 ? 详情加载时间 : 工作线程快照.最近结束时间;
        私有_追加线程显示详情(
            工作线程节点,
            "任务管理工作线程",
            "按任务工作包推进任务筹办和任务执行生命周期",
            "逻辑线程",
            "task-worker-main",
            "待绑定",
            工作线程快照.故障
                ? std::string("故障")
                : (工作线程快照.已启动 ? std::string("运行中") : std::string("未启动")),
            工作线程快照.正在执行 ? std::string("正在执行") : std::string("空闲或未启动"),
            工作线程快照.最近开始时间 == 0
                ? std::string("未记录")
                : 私有_时间文本(工作线程快照.最近开始时间),
            私有_起点时长文本(工作线程快照.最近开始时间, 工作线程结束时间),
            详情加载时间,
            工作线程快照.最近推进阶段.empty()
                ? std::string("推进=") + std::to_string(工作线程快照.累计推进次数)
                : 私有_截断文本(工作线程快照.最近推进阶段 + " / " + 工作线程快照.最近推进状况, 160),
            "当前按项目工作线程快照显示；线程本体加载时间和运行时长由 002002 消息表接入后替换。");
        根节点.子项.push_back(std::move(工作线程节点));

        auto 摘要线程节点 = 私有_新节点(
            std::string("控制面板摘要线程 | 生命周期=")
                + 控制面板摘要线程生命周期文本(
                    static_cast<枚举_控制面板摘要线程生命周期状态>(
                        快照.控制面板摘要线程生命周期))
                + " | 健康=" + std::string(布尔文本_是或否(快照.控制面板摘要线程健康运行))
                + " | 快照=" + std::to_string(快照.控制面板摘要快照序号),
            私有_线程详情_摘要,
            false,
            false,
            "thread-summary");
        私有_追加线程显示详情(
            摘要线程节点,
            "控制面板摘要线程",
            "后台采集控制面板摘要和轻量运行事实镜像",
            "逻辑线程",
            "control-panel-summary",
            "待绑定",
            控制面板摘要线程生命周期文本(
                static_cast<枚举_控制面板摘要线程生命周期状态>(
                    快照.控制面板摘要线程生命周期)),
            快照.控制面板摘要线程健康运行 ? std::string("健康") : std::string("异常或未启动"),
            "待线程生命周期消息接入",
            "待线程生命周期消息接入",
            详情加载时间,
            std::string("自我生命周期=")
                + 私有_线程生命周期文本(static_cast<枚举_线程生命周期状态>(快照.自我线程生命周期))
                + " | 阶段=" + 快照.自我线程当前阶段
                + " | 去向=" + 快照.自我线程当前最终去向
                + " | Tick=" + std::to_string(快照.自我Tick计数),
            "当前快照提供生命周期、健康状态和摘要序号；真实加载时间 / 连续运行时长由线程信息消息表接入后显示。");
        根节点.子项.push_back(std::move(摘要线程节点));

        return 根节点;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_转义HTML(std::string_view 文本)
    {
        std::string 输出;
        输出.reserve(文本.size() + 16);
        for (const char 字符 : 文本) {
            switch (字符) {
            case '&': 输出 += "&amp;"; break;
            case '<': 输出 += "&lt;"; break;
            case '>': 输出 += "&gt;"; break;
            case '"': 输出 += "&quot;"; break;
            case '\'': 输出 += "&#39;"; break;
            default: 输出.push_back(字符); break;
            }
        }
        return 输出;
    }

    struct 结构_SQL控制面板数据 {
        std::vector<std::string> 批次{};
        std::vector<std::vector<std::string>> 运行批次{};
        std::vector<std::vector<std::string>> 同步状态{};
        std::vector<std::vector<std::string>> 事件流水{};
        std::vector<std::vector<std::string>> 当前任务{};
        std::vector<std::vector<std::string>> 当前需求{};
        std::vector<std::vector<std::string>> 当前方法{};
        std::vector<std::vector<std::string>> 当前世界{};
        std::vector<std::vector<std::string>> 当前动态{};
        std::vector<std::vector<std::string>> 指标{};
        std::vector<std::vector<std::string>> 线程{};
        std::vector<std::vector<std::string>> 线程事件{};
        std::vector<std::vector<std::string>> 动作动态{};
        std::vector<std::vector<std::string>> 因果信息{};
        std::vector<std::vector<std::string>> 因果信息关系{};
        std::vector<std::vector<std::string>> 特征{};
        std::vector<std::vector<std::string>> 字段目录{};
        std::vector<std::vector<std::string>> 需求树{};
        std::vector<std::vector<std::string>> 任务树{};
        std::vector<std::vector<std::string>> 方法树{};
        std::vector<std::vector<std::string>> 世界树{};
        std::vector<std::vector<std::string>> 世界树关系{};
        std::vector<std::vector<std::string>> 语素树{};
        std::string 投影读取错误{};
    };

    using SQL控制面板目标行集 = std::vector<std::vector<std::string>> 结构_SQL控制面板数据::*;

    // 功能：生成控制面板 SQL 读模型 ADO 连接串。
    std::string 私有_SQL控制面板ADO连接串()
    {
        return 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "鱼巢投影库");
    }

    // 功能：通过 ADO 执行控制面板 SQL 查询。
    bool 私有_执行ADO控制面板查询(
        const std::string& 连接串,
        std::string_view 名称,
        std::string_view SQL,
        结构_ADO查询结果& 结果,
        std::string& 错误)
    {
        std::string ADO错误{};
        if (!执行ADO查询(连接串, std::string(SQL), 结果, ADO错误)) {
            错误 = std::string(名称) + " 查询失败 | " + ADO错误;
            return false;
        }
        return true;
    }

    // 功能：从 SQL Server 最新投影视图读取控制面板数据。
    bool 私有_读取SQL控制面板数据(结构_SQL控制面板数据& 数据, std::string& 错误)
    {
        数据 = {};
        const auto 连接串 = 私有_SQL控制面板ADO连接串();

        const std::vector<std::tuple<std::string_view, std::string_view, SQL控制面板目标行集>> 运行态查询集{
            {
                "控制面板运行批次",
                R"SQL(
SELECT TOP (1)
    CONVERT(nvarchar(36), [运行ID]) AS [运行ID],
    CONVERT(nvarchar(19), [启动时间], 120) AS [启动时间],
    COALESCE([来源标记], N'') AS [来源标记],
    CONVERT(nvarchar(20), COALESCE([进程ID], 0)) AS [进程ID],
    CONVERT(nvarchar(5), COALESCE([同步开关], 0)) AS [同步开关],
    COALESCE([状态], N'') AS [状态]
FROM [鱼巢].[控制面板运行批次]
ORDER BY [启动时间] DESC;
)SQL",
                &结构_SQL控制面板数据::运行批次
            },
            {
                "SQL投影同步状态",
                R"SQL(
SELECT
    COALESCE([同步域], N'') AS [同步域],
    COALESCE([最近阶段], N'') AS [最近阶段],
    COALESCE([最近状态], N'') AS [最近状态],
    CONVERT(nvarchar(19), [更新时间], 120) AS [更新时间],
    COALESCE([最近错误], N'') AS [最近错误]
FROM [鱼巢].[SQL投影同步状态]
ORDER BY [更新时间] DESC, [同步域];
)SQL",
                &结构_SQL控制面板数据::同步状态
            },
            {
                "控制面板事件流水",
                R"SQL(
SELECT TOP (300)
    CONVERT(nvarchar(19), [发生时间], 120) AS [发生时间],
    COALESCE([领域], N'') AS [领域],
    COALESCE([操作], N'') AS [操作],
    COALESCE([对象主键], N'') AS [对象主键],
    COALESCE([显示摘要], N'') AS [显示摘要]
FROM [鱼巢].[控制面板事件流水]
ORDER BY [发生时间] DESC, [事件ID] DESC;
)SQL",
                &结构_SQL控制面板数据::事件流水
            },
            {
                "当前任务显示项",
                R"SQL(
SELECT
    COALESCE([任务主键], N'') AS [任务主键],
    COALESCE([父任务主键], N'') AS [父任务主键],
    CONVERT(nvarchar(20), COALESCE([状态值], 0)) AS [状态值],
    COALESCE([状态文本], N'') AS [状态文本],
    CONVERT(nvarchar(19), [更新时间], 120) AS [更新时间],
    COALESCE([显示摘要], N'') AS [显示摘要]
FROM [鱼巢].[当前任务显示项]
ORDER BY [更新时间] DESC, [任务主键];
)SQL",
                &结构_SQL控制面板数据::当前任务
            },
            {
                "当前需求显示项",
                R"SQL(
SELECT
    COALESCE([需求主键], N'') AS [需求主键],
    COALESCE([父需求主键], N'') AS [父需求主键],
    CONVERT(nvarchar(20), COALESCE([满足关系掩码], 0)) AS [满足关系掩码],
    CONVERT(nvarchar(1), COALESCE([已满足], 0)) AS [已满足],
    CONVERT(nvarchar(19), [更新时间], 120) AS [更新时间],
    COALESCE([显示摘要], N'') AS [显示摘要]
FROM [鱼巢].[当前需求显示项]
ORDER BY [更新时间] DESC, [需求主键];
)SQL",
                &结构_SQL控制面板数据::当前需求
            },
            {
                "当前方法显示项",
                R"SQL(
SELECT
    COALESCE([方法主键], N'') AS [方法主键],
    COALESCE([父方法主键], N'') AS [父方法主键],
    CONVERT(nvarchar(20), COALESCE([状态值], 0)) AS [状态值],
    CONVERT(nvarchar(19), [更新时间], 120) AS [更新时间],
    COALESCE([显示摘要], N'') AS [显示摘要]
FROM [鱼巢].[当前方法显示项]
ORDER BY [更新时间] DESC, [方法主键];
)SQL",
                &结构_SQL控制面板数据::当前方法
            },
            {
                "当前世界显示项",
                R"SQL(
SELECT
    COALESCE([对象主键], N'') AS [对象主键],
    COALESCE([父对象主键], N'') AS [父对象主键],
    COALESCE([对象类型], N'') AS [对象类型],
    CONVERT(nvarchar(19), [更新时间], 120) AS [更新时间],
    COALESCE([显示摘要], N'') AS [显示摘要]
FROM [鱼巢].[当前世界显示项]
ORDER BY [更新时间] DESC, [对象主键];
)SQL",
                &结构_SQL控制面板数据::当前世界
            },
            {
                "当前动态显示项",
                R"SQL(
SELECT
    COALESCE([动态主键], N'') AS [动态主键],
    COALESCE([来源主键], N'') AS [来源主键],
    COALESCE([动态类型], N'') AS [动态类型],
    CONVERT(nvarchar(19), [更新时间], 120) AS [更新时间],
    COALESCE([显示摘要], N'') AS [显示摘要]
FROM [鱼巢].[当前动态显示项]
ORDER BY [更新时间] DESC, [动态主键];
)SQL",
                &结构_SQL控制面板数据::当前动态
            },
        };

        for (const auto& [名称, SQL, 目标行集] : 运行态查询集) {
            结构_ADO查询结果 查询结果{};
            if (!私有_执行ADO控制面板查询(
                连接串,
                名称,
                SQL,
                查询结果,
                错误)) {
                return false;
            }
            (数据.*目标行集) = std::move(查询结果.行集);
        }
        if (数据.运行批次.empty()) {
            错误 = "SQL 控制面板运行批次为空";
            return false;
        }
        const auto 读取行字段 = [](const std::vector<std::string>& 行, const std::size_t 索引) {
            return 索引 < 行.size() ? 行[索引] : std::string{};
        };
        数据.批次 = {
            读取行字段(数据.运行批次.front(), 0),
            读取行字段(数据.运行批次.front(), 1),
            读取行字段(数据.运行批次.front(), 2),
            std::string("运行态SQL显示镜像 | 状态=") + 读取行字段(数据.运行批次.front(), 5)
        };

        const std::vector<std::tuple<std::string_view, std::string_view, SQL控制面板目标行集>> 查询集{
            {
                "面板指标",
                R"SQL(
SELECT TOP (80)
    COALESCE([指标键], N'') AS [指标键],
    COALESCE([指标分组], N'') AS [指标分组],
    COALESCE([值文本], N'') AS [值文本],
    COALESCE([来源类型], N'') AS [来源类型],
    COALESCE([来源路径], N'') AS [来源路径]
FROM [鱼巢].[最新控制面板运行指标]
ORDER BY [记录标识];
)SQL",
                &结构_SQL控制面板数据::指标
            },
            {
                "线程信息",
                R"SQL(
SELECT TOP (120)
    COALESCE([逻辑标识], N'') AS [逻辑标识],
    COALESCE([线程名称], N'') AS [线程名称],
    COALESCE([生命周期状态], N'') AS [生命周期状态],
    COALESCE([运行状态], N'') AS [运行状态],
    CONVERT(nvarchar(10), COALESCE([是否健康], 0)) AS [是否健康],
    COALESCE([模块名称], N'') AS [模块名称],
    COALESCE([最近原因键], N'') AS [最近原因键]
FROM [鱼巢].[最新控制面板线程信息]
ORDER BY [行号];
)SQL",
                &结构_SQL控制面板数据::线程
            },
            {
                "线程生命周期事件",
                R"SQL(
SELECT TOP (120)
    CONVERT(nvarchar(40), [消息标识]) AS [消息标识],
    COALESCE([事件类型], N'') AS [事件类型],
    COALESCE([逻辑标识], N'') AS [逻辑标识],
    COALESCE([旧生命周期状态], N'') AS [旧生命周期状态],
    COALESCE([新生命周期状态], N'') AS [新生命周期状态],
    COALESCE([原因键], N'') AS [原因键],
    COALESCE([显示摘要], N'') AS [显示摘要]
FROM [鱼巢].[最新控制面板线程生命周期事件]
ORDER BY [发生时间微秒] DESC, [消息标识] DESC;
)SQL",
                &结构_SQL控制面板数据::线程事件
            },
            {
                "动作动态",
                R"SQL(
SELECT TOP (120)
    CONVERT(nvarchar(23), [日志时间], 121) AS [日志时间],
    COALESCE([事件类别], N'') AS [事件类别],
    COALESCE([事件名称], N'') AS [事件名称],
    COALESCE([方法名称], N'') AS [方法名称],
    COALESCE([特征主键], N'') AS [特征主键],
    COALESCE([动作动态], N'') AS [动作动态],
    COALESCE([来源动作动态], N'') AS [来源动作动态]
FROM [鱼巢].[最新动作动态]
ORDER BY [日志时间] DESC, [事件序号] DESC;
)SQL",
                &结构_SQL控制面板数据::动作动态
            },
            {
                "因果信息",
                R"SQL(
SELECT
    COALESCE([节点主键], N'') AS [节点主键],
    COALESCE([父节点主键], N'') AS [父节点主键],
    CONVERT(nvarchar(20), COALESCE([深度], 0)) AS [深度],
    COALESCE([节点类型], N'') AS [节点类型],
    COALESCE([显示文本], N'') AS [显示文本],
    COALESCE([类型文本], N'') AS [类型文本],
    COALESCE([值类别], N'') AS [值类别],
    COALESCE([值文本], N'') AS [值文本],
    COALESCE([辅助文本], N'') AS [辅助文本]
FROM [鱼巢].[当前世界树节点] n
WHERE n.[节点类型] = N'因果'
  AND NOT EXISTS (
      SELECT 1
      FROM [鱼巢].[当前世界树节点] ancestor
      WHERE ancestor.[节点类型] = N'因果'
        AND ancestor.[节点主键] <> n.[节点主键]
        AND n.[路径文本] LIKE ancestor.[路径文本] + N'/%'
  )
ORDER BY [行号];
)SQL",
                &结构_SQL控制面板数据::因果信息
            },
            {
                "因果信息关系",
                R"SQL(
SELECT
    COALESCE([宿主主键], N'') AS [宿主主键],
    COALESCE([关系名], N'') AS [关系名],
    COALESCE([目标类别], N'') AS [目标类别],
    COALESCE([目标主键], N'') AS [目标主键],
    COALESCE([目标文本], N'') AS [目标文本],
    CONVERT(nvarchar(20), [序号]) AS [序号]
FROM [鱼巢].[当前世界树关系]
WHERE [宿主主键] IN (
    SELECT [节点主键]
    FROM [鱼巢].[当前世界树节点] n
    WHERE n.[节点类型] = N'因果'
      AND NOT EXISTS (
          SELECT 1
          FROM [鱼巢].[当前世界树节点] ancestor
          WHERE ancestor.[节点类型] = N'因果'
            AND ancestor.[节点主键] <> n.[节点主键]
            AND n.[路径文本] LIKE ancestor.[路径文本] + N'/%'
      )
)
ORDER BY [宿主主键], [序号], [关系名];
)SQL",
                &结构_SQL控制面板数据::因果信息关系
            },
            {
                "特征类型",
                R"SQL(
SELECT TOP (120)
    COALESCE([特征名称], N'') AS [特征名称],
    COALESCE([来源类型], N'') AS [来源类型],
    COALESCE([符号名称], N'') AS [符号名称],
    COALESCE([来源路径], N'') AS [来源路径],
    CONVERT(nvarchar(20), COALESCE([来源行号], 0)) AS [来源行号]
FROM [鱼巢].[最新特征类型]
ORDER BY [特征名称];
)SQL",
                &结构_SQL控制面板数据::特征
            },
            {
                "控制面板字段目录",
                R"SQL(
SELECT TOP (160)
    COALESCE([指标键], N'') AS [指标键],
    COALESCE([数据分组], N'') AS [数据分组],
    COALESCE([代码类型], N'') AS [代码类型],
    COALESCE([控制面板结构], N'') AS [控制面板结构],
    COALESCE([来源路径], N'') AS [来源路径],
    CONVERT(nvarchar(20), COALESCE([来源行号], 0)) AS [来源行号]
FROM [鱼巢].[最新控制面板字段目录]
ORDER BY [数据分组], [指标键];
)SQL",
                &结构_SQL控制面板数据::字段目录
            },
            {
                "需求树",
                R"SQL(
SELECT TOP (400)
    COALESCE([显示节点标识], N'') AS [显示节点标识],
    COALESCE([显示父节点标识], N'') AS [显示父节点标识],
    CONVERT(nvarchar(20), COALESCE([显示深度], 0)) AS [深度],
    COALESCE([节点名称], N'') AS [节点名称],
    COALESCE([树形摘要], N'') AS [树形摘要],
    COALESCE([目标语义], N'') AS [目标语义],
    COALESCE([目标特征显示], N'') AS [目标特征显示],
    COALESCE([任务显示], N'') AS [任务显示],
    COALESCE([节点主键], N'') AS [节点主键],
    COALESCE([逻辑组织类型], N'') AS [逻辑组织类型],
    COALESCE([目标特征类型], N'') AS [目标特征类型],
    COALESCE([目标特征主键], N'') AS [目标特征主键],
    COALESCE([任务类型], N'') AS [任务类型],
    COALESCE([任务主键], N'') AS [任务主键],
    COALESCE([主体显示名], N'') AS [主体显示名],
    COALESCE([主体类型名], N'') AS [主体类型名],
    COALESCE([主体主键], N'') AS [主体主键],
    COALESCE([场景显示名], N'') AS [场景显示名],
    COALESCE([场景类型名], N'') AS [场景类型名],
    COALESCE([场景主键], N'') AS [场景主键],
    COALESCE([目标宿主显示名], N'') AS [目标宿主显示名],
    COALESCE([目标宿主类型名], N'') AS [目标宿主类型名],
    COALESCE([目标宿主主键], N'') AS [目标宿主主键],
    COALESCE([当前状态显示名], N'') AS [当前状态显示名],
    COALESCE([当前状态类型名], N'') AS [当前状态类型名],
    COALESCE([当前状态主键], N'') AS [当前状态主键],
    COALESCE([目标状态显示名], N'') AS [目标状态显示名],
    COALESCE([目标状态类型名], N'') AS [目标状态类型名],
    COALESCE([目标状态主键], N'') AS [目标状态主键],
    CONVERT(nvarchar(20), COALESCE([满足关系掩码], 0)) AS [满足关系掩码],
    CONVERT(nvarchar(20), COALESCE([安全权重], 0)) AS [安全权重],
    CONVERT(nvarchar(20), COALESCE([服务权重], 0)) AS [服务权重],
    CONVERT(nvarchar(20), COALESCE([累计安全结算], 0)) AS [累计安全结算],
    CONVERT(nvarchar(20), COALESCE([累计服务结算], 0)) AS [累计服务结算],
    CONVERT(nvarchar(20), COALESCE([有效截止微秒], 0)) AS [有效截止微秒],
    COALESCE([最近结算任务显示名], N'') AS [最近结算任务显示名],
    COALESCE([最近结算任务类型名], N'') AS [最近结算任务类型名],
    COALESCE([最近结算任务主键], N'') AS [最近结算任务主键],
    CONVERT(nvarchar(20), COALESCE([最近结算时间微秒], 0)) AS [最近结算时间微秒],
    COALESCE([描述主键], N'') AS [描述主键],
    CONVERT(nvarchar(20), COALESCE([统计创建时间微秒], 0)) AS [统计创建时间微秒],
    CONVERT(nvarchar(20), COALESCE([统计最后观测时间微秒], 0)) AS [统计最后观测时间微秒],
    CONVERT(nvarchar(20), COALESCE([统计命中次数], 0)) AS [统计命中次数],
    CONVERT(nvarchar(1), COALESCE([已截止], 0)) AS [已截止],
    CONVERT(nvarchar(1), COALESCE([阻塞父任务], 0)) AS [阻塞父任务]
FROM [鱼巢].[当前需求面板节点]
WHERE COALESCE([显示父节点标识], N'') = N''
ORDER BY [行号];
)SQL",
                &结构_SQL控制面板数据::需求树
            },
            {
                "任务树",
                R"SQL(
SELECT
    COALESCE([节点主键], N'') AS [节点主键],
    COALESCE([父节点主键], N'') AS [父节点主键],
    CONVERT(nvarchar(20), COALESCE([深度], 0)) AS [深度],
    COALESCE([节点种类文本], N'') AS [节点种类文本],
    COALESCE([任务状态文本], N'') AS [任务状态文本],
    COALESCE([需求主键], N'') AS [需求主键],
    COALESCE([目标状态主键], N'') AS [目标状态主键],
    COALESCE([结果状态主键], N'') AS [结果状态主键]
FROM [鱼巢].[当前任务树节点]
WHERE [深度] = 1
ORDER BY [行号];
)SQL",
                &结构_SQL控制面板数据::任务树
            },
            {
                "方法树",
                R"SQL(
SELECT
    COALESCE([节点主键], N'') AS [节点主键],
    COALESCE([父节点主键], N'') AS [父节点主键],
    CONVERT(nvarchar(20), COALESCE([深度], 0)) AS [深度],
    COALESCE([节点种类文本], N'') AS [节点种类文本],
    COALESCE([动作名称], N'') AS [动作名称],
    COALESCE([动作句柄], N'') AS [动作句柄],
    COALESCE([来源文本], N'') AS [来源文本],
    COALESCE([主结果特征主键], N'') AS [主结果特征主键],
    CONVERT(nvarchar(20), COALESCE([结果项数量], 0)) AS [结果项数量]
FROM [鱼巢].[当前方法树节点]
WHERE [深度] = 1
ORDER BY [行号];
)SQL",
                &结构_SQL控制面板数据::方法树
            },
            {
                "世界树",
                R"SQL(
SELECT
    COALESCE([节点主键], N'') AS [节点主键],
    COALESCE([父节点主键], N'') AS [父节点主键],
    CONVERT(nvarchar(20), COALESCE([深度], 0)) AS [深度],
    COALESCE([节点类型], N'') AS [节点类型],
    COALESCE([显示文本], N'') AS [显示文本],
    COALESCE([类型文本], N'') AS [类型文本],
    COALESCE([值类别], N'') AS [值类别],
    COALESCE([值文本], N'') AS [值文本],
    COALESCE([辅助文本], N'') AS [辅助文本]
FROM [鱼巢].[当前世界树节点]
WHERE COALESCE([父节点主键], N'') = N''
ORDER BY [行号];
)SQL",
                &结构_SQL控制面板数据::世界树
            },
            {
                "世界树关系",
                R"SQL(
SELECT TOP (2000)
    COALESCE([宿主主键], N'') AS [宿主主键],
    COALESCE([关系名], N'') AS [关系名],
    COALESCE([目标类别], N'') AS [目标类别],
    COALESCE([目标主键], N'') AS [目标主键],
    COALESCE([目标文本], N'') AS [目标文本],
    CONVERT(nvarchar(20), COALESCE([序号], 0)) AS [序号]
FROM [鱼巢].[当前世界树关系]
ORDER BY [行号];
)SQL",
                &结构_SQL控制面板数据::世界树关系
            },
            {
                "语素树",
                R"SQL(
SELECT
    COALESCE([节点主键], N'') AS [节点主键],
    COALESCE([父节点主键], N'') AS [父节点主键],
    CONVERT(nvarchar(20), COALESCE([深度], 0)) AS [深度],
    COALESCE([节点类型], N'') AS [节点类型],
    COALESCE([词面文本], N'') AS [词面文本],
    COALESCE([信息入口类型文本], N'') AS [信息入口类型文本],
    COALESCE([映射主信息类型文本], N'') AS [映射主信息类型文本],
    COALESCE([绑定基础信息主键], N'') AS [绑定基础信息主键]
FROM [鱼巢].[当前语素节点]
WHERE COALESCE([父节点主键], N'') = N''
ORDER BY [行号];
)SQL",
                &结构_SQL控制面板数据::语素树
            },
        };

        for (const auto& [名称, SQL, 目标行集] : 查询集) {
            结构_ADO查询结果 查询结果{};
            if (!私有_执行ADO控制面板查询(
                连接串,
                名称,
                SQL,
                查询结果,
                错误)) {
                if (数据.投影读取错误.empty()) {
                    数据.投影读取错误 = 错误;
                }
                continue;
            }
            (数据.*目标行集) = std::move(查询结果.行集);
        }

        std::vector<std::vector<std::string>> 运行态指标{};
        const auto 添加运行态指标 = [&](std::string 名称, const std::size_t 数量, std::string 来源) {
            运行态指标.push_back({
                std::move(名称),
                "运行态SQL",
                std::to_string(数量),
                std::move(来源),
                "控制面板运行态表"
            });
        };
        添加运行态指标("运行批次", 数据.运行批次.size(), "[鱼巢].[控制面板运行批次]");
        添加运行态指标("同步状态", 数据.同步状态.size(), "[鱼巢].[SQL投影同步状态]");
        添加运行态指标("事件流水", 数据.事件流水.size(), "[鱼巢].[控制面板事件流水]");
        添加运行态指标("当前任务", 数据.当前任务.size(), "[鱼巢].[当前任务显示项]");
        添加运行态指标("当前需求", 数据.当前需求.size(), "[鱼巢].[当前需求显示项]");
        添加运行态指标("当前方法", 数据.当前方法.size(), "[鱼巢].[当前方法显示项]");
        添加运行态指标("当前世界", 数据.当前世界.size(), "[鱼巢].[当前世界显示项]");
        添加运行态指标("当前动态", 数据.当前动态.size(), "[鱼巢].[当前动态显示项]");
        if (!数据.投影读取错误.empty()) {
            运行态指标.push_back({
                "旧全量投影读取",
                "校准路径",
                "失败",
                "可选读模型",
                数据.投影读取错误
            });
        }
        else {
            运行态指标.push_back({
                "旧全量投影读取",
                "校准路径",
                "可用",
                "可选读模型",
                "旧投影视图已读取"
            });
        }
        运行态指标.insert(
            运行态指标.end(),
            std::make_move_iterator(数据.指标.begin()),
            std::make_move_iterator(数据.指标.end()));
        数据.指标 = std::move(运行态指标);
        return true;
    }

    // 功能：读取 SQL 控制面板结果行中的指定字段。
    std::string 私有_SQL字段(
        const std::vector<std::string>& 行,
        const std::size_t 索引)
    {
        return 索引 < 行.size() ? 行[索引] : std::string{};
    }

    // 功能：生成 SQL Server Unicode 字符串字面量。
    std::string 私有_SQL字符串字面量(std::string_view 文本)
    {
        std::string 输出 = "N'";
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

    // 功能：向 SQL 控制面板 HTML 追加一个数据表区段。
    void 私有_追加SQL控制面板表(
        std::ostringstream& 输出,
        std::string_view 标题,
        std::string_view 区段ID,
        const std::vector<std::string_view>& 表头,
        const std::vector<std::vector<std::string>>& 行集)
    {
        输出 << "<section id=\"" << 私有_转义HTML(区段ID)
            << "\" data-section-title=\"" << 私有_转义HTML(标题) << "\">\n"
            << "<h2>" << 私有_转义HTML(标题) << "</h2>\n"
            << "<div class=\"table-wrap\"><table data-filterable><thead><tr>";
        for (const auto 表头项 : 表头) {
            输出 << "<th>" << 私有_转义HTML(表头项) << "</th>";
        }
        输出 << "</tr></thead><tbody>\n";
        for (const auto& 行 : 行集) {
            输出 << "<tr>";
            for (std::size_t i = 0; i < 表头.size(); ++i) {
                输出 << "<td>" << 私有_转义HTML(私有_SQL字段(行, i)) << "</td>";
            }
            输出 << "</tr>\n";
        }
        输出 << "</tbody></table></div></section>\n";
    }

    // 功能：向 SQL 控制面板 HTML 追加 JSON 字符串。
    void 私有_追加SQL控制面板JSON字符串(std::ostringstream& 输出, std::string_view 文本)
    {
        输出 << '"';
        for (const char 字符 : 文本) {
            switch (字符) {
            case '\\': 输出 << "\\\\"; break;
            case '"': 输出 << "\\\""; break;
            case '\n': 输出 << "\\n"; break;
            case '\r': 输出 << "\\r"; break;
            case '\t': 输出 << "\\t"; break;
            default: 输出 << 字符; break;
            }
        }
        输出 << '"';
    }

    // 功能：把 SQL 控制面板表头输出为 JSON 字符串数组。
    void 私有_追加SQL控制面板表头JSON(
        std::ostringstream& 输出,
        std::initializer_list<std::string_view> 表头)
    {
        输出 << "[";
        std::size_t 索引 = 0;
        for (const auto 字段 : 表头) {
            if (索引++ > 0) {
                输出 << ",";
            }
            私有_追加SQL控制面板JSON字符串(输出, 字段);
        }
        输出 << "]";
    }

    // 功能：把 SQL 控制面板行集输出为 JSON 二维数组。
    void 私有_追加SQL控制面板行集JSON(
        std::ostringstream& 输出,
        const std::vector<std::vector<std::string>>& 行集,
        const std::size_t 列数)
    {
        输出 << "[";
        for (std::size_t 行索引 = 0; 行索引 < 行集.size(); ++行索引) {
            if (行索引 > 0) {
                输出 << ",";
            }
            输出 << "[";
            for (std::size_t 列索引 = 0; 列索引 < 列数; ++列索引) {
                if (列索引 > 0) {
                    输出 << ",";
                }
                私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行集[行索引], 列索引));
            }
            输出 << "]";
        }
        输出 << "]";
    }

    // 功能：生成 SQL 控制面板普通表格区段刷新 JSON。
    std::string 私有_SQL控制面板表格区段刷新JSON(
        std::string_view 区段ID,
        std::string_view 标题,
        std::initializer_list<std::string_view> 表头,
        const std::vector<std::vector<std::string>>& 行集)
    {
        std::ostringstream 输出;
        输出 << "{\"ok\":true,\"kind\":\"sql-table\",\"page\":";
        私有_追加SQL控制面板JSON字符串(输出, 区段ID);
        输出 << ",\"title\":";
        私有_追加SQL控制面板JSON字符串(输出, 标题);
        输出 << ",\"headers\":";
        私有_追加SQL控制面板表头JSON(输出, 表头);
        输出 << ",\"rows\":";
        私有_追加SQL控制面板行集JSON(输出, 行集, 表头.size());
        输出 << "}";
        return 输出.str();
    }

    // 功能：生成 SQL 控制面板因果信息区段刷新 JSON。
    std::string 私有_SQL控制面板因果信息刷新JSON(
        std::string_view 区段ID,
        const 结构_SQL控制面板数据& 数据)
    {
        std::ostringstream 输出;
        输出 << "{\"ok\":true,\"kind\":\"sql-causal-info\",\"page\":";
        私有_追加SQL控制面板JSON字符串(输出, 区段ID);
        输出 << ",\"rows\":";
        私有_追加SQL控制面板行集JSON(输出, 数据.因果信息, 9);
        输出 << ",\"relations\":";
        私有_追加SQL控制面板行集JSON(输出, 数据.因果信息关系, 6);
        输出 << "}";
        return 输出.str();
    }

    // 功能：生成 SQL 控制面板因果链查询区段刷新 JSON。
    std::string 私有_SQL控制面板因果链刷新JSON(
        std::string_view 区段ID,
        const 结构_SQL控制面板数据& 数据)
    {
        std::ostringstream 输出;
        输出 << "{\"ok\":true,\"kind\":\"sql-causal-chain\",\"page\":";
        私有_追加SQL控制面板JSON字符串(输出, 区段ID);
        输出 << ",\"relations\":";
        私有_追加SQL控制面板行集JSON(输出, 数据.因果信息关系, 6);
        输出 << "}";
        return 输出.str();
    }

    // 功能：生成 SQL 控制面板世界树区段刷新 JSON。
    std::string 私有_SQL控制面板世界树刷新JSON(
        std::string_view 区段ID,
        const 结构_SQL控制面板数据& 数据)
    {
        std::ostringstream 输出;
        输出 << "{\"ok\":true,\"kind\":\"sql-world-tree\",\"page\":";
        私有_追加SQL控制面板JSON字符串(输出, 区段ID);
        输出 << ",\"headers\":";
        私有_追加SQL控制面板表头JSON(输出, { "节点", "父节点", "深度", "类别", "显示", "类型", "值类", "值", "辅助" });
        输出 << ",\"rows\":";
        私有_追加SQL控制面板行集JSON(输出, 数据.世界树, 9);
        输出 << ",\"relations\":";
        私有_追加SQL控制面板行集JSON(输出, 数据.世界树关系, 6);
        输出 << "}";
        return 输出.str();
    }

    // 功能：生成 SQL 控制面板子链读取失败 JSON。
    std::string 私有_SQL控制面板子链错误JSON(
        std::string_view 区段ID,
        std::string_view 节点键,
        std::string_view 错误)
    {
        std::ostringstream 输出;
        输出 << "{\"ok\":false,\"kind\":\"sql-subtree\",\"section\":";
        私有_追加SQL控制面板JSON字符串(输出, 区段ID);
        输出 << ",\"nodeKey\":";
        私有_追加SQL控制面板JSON字符串(输出, 节点键);
        输出 << ",\"error\":";
        私有_追加SQL控制面板JSON字符串(输出, 错误);
        输出 << "}";
        return 输出.str();
    }

    // 功能：执行 SQL 控制面板子链查询。
    bool 私有_读取SQL控制面板子链行集(
        std::string_view 名称,
        const std::string& SQL,
        std::vector<std::vector<std::string>>& 行集,
        std::string& 错误)
    {
        结构_ADO查询结果 查询结果{};
        if (!私有_执行ADO控制面板查询(
            私有_SQL控制面板ADO连接串(),
            名称,
            SQL,
            查询结果,
            错误)) {
            return false;
        }
        行集 = std::move(查询结果.行集);
        return true;
    }

    // 功能：生成 SQL 控制面板普通树页子链刷新 JSON。
    std::string 私有_SQL控制面板普通树子链JSON(
        std::string_view 区段ID,
        std::string_view 节点键,
        const std::vector<std::vector<std::string>>& 行集,
        const std::size_t 列数)
    {
        std::ostringstream 输出;
        输出 << "{\"ok\":true,\"kind\":\"sql-subtree\",\"section\":";
        私有_追加SQL控制面板JSON字符串(输出, 区段ID);
        输出 << ",\"nodeKey\":";
        私有_追加SQL控制面板JSON字符串(输出, 节点键);
        输出 << ",\"rows\":";
        私有_追加SQL控制面板行集JSON(输出, 行集, 列数);
        输出 << "}";
        return 输出.str();
    }

    // 功能：生成 SQL IN 条件中的字符串列表。
    std::string 私有_SQL字符串IN列表(
        const std::vector<std::string>& 键列表,
        const std::size_t 起点,
        const std::size_t 数量)
    {
        std::string 输出;
        const std::size_t 终点 = std::min(键列表.size(), 起点 + 数量);
        for (std::size_t 索引 = 起点; 索引 < 终点; ++索引) {
            if (!输出.empty()) {
                输出 += ",";
            }
            输出 += 私有_SQL字符串字面量(键列表[索引]);
        }
        return 输出;
    }

    // 功能：按父节点键读取 SQL 树直接子节点层。
    bool 私有_读取SQL树直接子层(
        std::string_view 查询名,
        std::string_view 来源视图,
        std::string_view 父键字段,
        std::string_view 选择字段SQL,
        std::string_view 父节点键,
        std::vector<std::vector<std::string>>& 行集,
        std::string& 错误)
    {
        行集.clear();
        std::string SQL;
        SQL += "SELECT ";
        SQL += 选择字段SQL;
        SQL += " FROM ";
        SQL += 来源视图;
        SQL += " WHERE COALESCE(";
        SQL += 父键字段;
        SQL += ", N'') = ";
        SQL += 私有_SQL字符串字面量(父节点键);
        SQL += " ORDER BY [行号];";

        return 私有_读取SQL控制面板子链行集(查询名, SQL, 行集, 错误);
    }

    // 功能：按世界树节点键集合读取关系行。
    bool 私有_读取SQL世界树关系行集(
        const std::vector<std::vector<std::string>>& 节点行集,
        std::vector<std::vector<std::string>>& 关系行集,
        std::string& 错误)
    {
        关系行集.clear();
        std::vector<std::string> 节点键集{};
        节点键集.reserve(节点行集.size());
        for (const auto& 行 : 节点行集) {
            const auto 键 = 私有_SQL字段(行, 0);
            if (!键.empty()) {
                节点键集.push_back(键);
            }
        }

        constexpr std::size_t 单批键数量 = 80;
        for (std::size_t 起点 = 0; 起点 < 节点键集.size(); 起点 += 单批键数量) {
            const auto IN列表 = 私有_SQL字符串IN列表(节点键集, 起点, 单批键数量);
            if (IN列表.empty()) {
                continue;
            }
            std::string SQL = R"SQL(
SELECT
    COALESCE([宿主主键], N'') AS [宿主主键],
    COALESCE([关系名], N'') AS [关系名],
    COALESCE([目标类别], N'') AS [目标类别],
    COALESCE([目标主键], N'') AS [目标主键],
    COALESCE([目标文本], N'') AS [目标文本],
    CONVERT(nvarchar(20), COALESCE([序号], 0)) AS [序号]
FROM [鱼巢].[当前世界树关系]
WHERE [宿主主键] IN ()SQL";
            SQL += IN列表;
            SQL += ") ORDER BY [行号];";

            std::vector<std::vector<std::string>> 本批行集{};
            if (!私有_读取SQL控制面板子链行集("世界树子链关系", SQL, 本批行集, 错误)) {
                return false;
            }
            for (auto& 行 : 本批行集) {
                关系行集.push_back(std::move(行));
            }
        }
        return true;
    }

    // 功能：生成 SQL 控制面板世界树子链刷新 JSON。
    std::string 私有_SQL控制面板世界树子链JSON(
        std::string_view 区段ID,
        std::string_view 节点键,
        const std::vector<std::vector<std::string>>& 节点行集,
        const std::vector<std::vector<std::string>>& 关系行集)
    {
        std::ostringstream 输出;
        输出 << "{\"ok\":true,\"kind\":\"sql-world-subtree\",\"section\":";
        私有_追加SQL控制面板JSON字符串(输出, 区段ID);
        输出 << ",\"nodeKey\":";
        私有_追加SQL控制面板JSON字符串(输出, 节点键);
        输出 << ",\"rows\":";
        私有_追加SQL控制面板行集JSON(输出, 节点行集, 9);
        输出 << ",\"relations\":";
        私有_追加SQL控制面板行集JSON(输出, 关系行集, 6);
        输出 << "}";
        return 输出.str();
    }

    // 功能：按 SQL 控制面板树节点读取一层直接子节点。
    std::string 私有_SQL控制面板子链JSON(
        std::string_view 区段ID,
        std::string_view 节点键)
    {
        if (节点键.empty()) {
            return 私有_SQL控制面板子链错误JSON(区段ID, 节点键, "节点键为空");
        }

        std::vector<std::vector<std::string>> 行集{};
        std::string 错误{};
        if (区段ID == "demandTree") {
            constexpr std::string_view 字段 = R"SQL(
COALESCE([显示节点标识], N'') AS [显示节点标识],
COALESCE([显示父节点标识], N'') AS [显示父节点标识],
CONVERT(nvarchar(20), COALESCE([显示深度], 0)) AS [深度],
COALESCE([节点名称], N'') AS [节点名称],
COALESCE([树形摘要], N'') AS [树形摘要],
COALESCE([目标语义], N'') AS [目标语义],
COALESCE([目标特征显示], N'') AS [目标特征显示],
COALESCE([任务显示], N'') AS [任务显示],
COALESCE([节点主键], N'') AS [节点主键],
COALESCE([逻辑组织类型], N'') AS [逻辑组织类型],
COALESCE([目标特征类型], N'') AS [目标特征类型],
COALESCE([目标特征主键], N'') AS [目标特征主键],
COALESCE([任务类型], N'') AS [任务类型],
COALESCE([任务主键], N'') AS [任务主键],
COALESCE([主体显示名], N'') AS [主体显示名],
COALESCE([主体类型名], N'') AS [主体类型名],
COALESCE([主体主键], N'') AS [主体主键],
COALESCE([场景显示名], N'') AS [场景显示名],
COALESCE([场景类型名], N'') AS [场景类型名],
COALESCE([场景主键], N'') AS [场景主键],
COALESCE([目标宿主显示名], N'') AS [目标宿主显示名],
COALESCE([目标宿主类型名], N'') AS [目标宿主类型名],
COALESCE([目标宿主主键], N'') AS [目标宿主主键],
COALESCE([当前状态显示名], N'') AS [当前状态显示名],
COALESCE([当前状态类型名], N'') AS [当前状态类型名],
COALESCE([当前状态主键], N'') AS [当前状态主键],
COALESCE([目标状态显示名], N'') AS [目标状态显示名],
COALESCE([目标状态类型名], N'') AS [目标状态类型名],
COALESCE([目标状态主键], N'') AS [目标状态主键],
CONVERT(nvarchar(20), COALESCE([满足关系掩码], 0)) AS [满足关系掩码],
CONVERT(nvarchar(20), COALESCE([安全权重], 0)) AS [安全权重],
CONVERT(nvarchar(20), COALESCE([服务权重], 0)) AS [服务权重],
CONVERT(nvarchar(20), COALESCE([累计安全结算], 0)) AS [累计安全结算],
CONVERT(nvarchar(20), COALESCE([累计服务结算], 0)) AS [累计服务结算],
CONVERT(nvarchar(20), COALESCE([有效截止微秒], 0)) AS [有效截止微秒],
COALESCE([最近结算任务显示名], N'') AS [最近结算任务显示名],
COALESCE([最近结算任务类型名], N'') AS [最近结算任务类型名],
COALESCE([最近结算任务主键], N'') AS [最近结算任务主键],
CONVERT(nvarchar(20), COALESCE([最近结算时间微秒], 0)) AS [最近结算时间微秒],
COALESCE([描述主键], N'') AS [描述主键],
CONVERT(nvarchar(20), COALESCE([统计创建时间微秒], 0)) AS [统计创建时间微秒],
CONVERT(nvarchar(20), COALESCE([统计最后观测时间微秒], 0)) AS [统计最后观测时间微秒],
CONVERT(nvarchar(20), COALESCE([统计命中次数], 0)) AS [统计命中次数],
CONVERT(nvarchar(1), COALESCE([已截止], 0)) AS [已截止],
CONVERT(nvarchar(1), COALESCE([阻塞父任务], 0)) AS [阻塞父任务])SQL";
            if (!私有_读取SQL树直接子层("需求树子层", "[鱼巢].[当前需求面板节点]", "[显示父节点标识]", 字段, 节点键, 行集, 错误)) {
                return 私有_SQL控制面板子链错误JSON(区段ID, 节点键, 错误);
            }
            return 私有_SQL控制面板普通树子链JSON(区段ID, 节点键, 行集, 46);
        }
        if (区段ID == "taskTree") {
            constexpr std::string_view 字段 = R"SQL(
COALESCE([节点主键], N'') AS [节点主键],
COALESCE([父节点主键], N'') AS [父节点主键],
CONVERT(nvarchar(20), COALESCE([深度], 0)) AS [深度],
COALESCE([节点种类文本], N'') AS [节点种类文本],
COALESCE([任务状态文本], N'') AS [任务状态文本],
COALESCE([需求主键], N'') AS [需求主键],
COALESCE([目标状态主键], N'') AS [目标状态主键],
COALESCE([结果状态主键], N'') AS [结果状态主键])SQL";
            if (!私有_读取SQL树直接子层("任务树子层", "[鱼巢].[当前任务树节点]", "[父节点主键]", 字段, 节点键, 行集, 错误)) {
                return 私有_SQL控制面板子链错误JSON(区段ID, 节点键, 错误);
            }
            return 私有_SQL控制面板普通树子链JSON(区段ID, 节点键, 行集, 8);
        }
        if (区段ID == "methodTree") {
            constexpr std::string_view 字段 = R"SQL(
COALESCE([节点主键], N'') AS [节点主键],
COALESCE([父节点主键], N'') AS [父节点主键],
CONVERT(nvarchar(20), COALESCE([深度], 0)) AS [深度],
COALESCE([节点种类文本], N'') AS [节点种类文本],
COALESCE([动作名称], N'') AS [动作名称],
COALESCE([动作句柄], N'') AS [动作句柄],
COALESCE([来源文本], N'') AS [来源文本],
COALESCE([主结果特征主键], N'') AS [主结果特征主键],
CONVERT(nvarchar(20), COALESCE([结果项数量], 0)) AS [结果项数量])SQL";
            if (!私有_读取SQL树直接子层("方法树子层", "[鱼巢].[当前方法树节点]", "[父节点主键]", 字段, 节点键, 行集, 错误)) {
                return 私有_SQL控制面板子链错误JSON(区段ID, 节点键, 错误);
            }
            return 私有_SQL控制面板普通树子链JSON(区段ID, 节点键, 行集, 9);
        }
        if (区段ID == "lexemeTree") {
            constexpr std::string_view 字段 = R"SQL(
COALESCE([节点主键], N'') AS [节点主键],
COALESCE([父节点主键], N'') AS [父节点主键],
CONVERT(nvarchar(20), COALESCE([深度], 0)) AS [深度],
COALESCE([节点类型], N'') AS [节点类型],
COALESCE([词面文本], N'') AS [词面文本],
COALESCE([信息入口类型文本], N'') AS [信息入口类型文本],
COALESCE([映射主信息类型文本], N'') AS [映射主信息类型文本],
COALESCE([绑定基础信息主键], N'') AS [绑定基础信息主键])SQL";
            if (!私有_读取SQL树直接子层("语素树子层", "[鱼巢].[当前语素节点]", "[父节点主键]", 字段, 节点键, 行集, 错误)) {
                return 私有_SQL控制面板子链错误JSON(区段ID, 节点键, 错误);
            }
            return 私有_SQL控制面板普通树子链JSON(区段ID, 节点键, 行集, 8);
        }
        if (区段ID == "worldTree") {
            constexpr std::string_view 字段 = R"SQL(
COALESCE([节点主键], N'') AS [节点主键],
COALESCE([父节点主键], N'') AS [父节点主键],
CONVERT(nvarchar(20), COALESCE([深度], 0)) AS [深度],
COALESCE([节点类型], N'') AS [节点类型],
COALESCE([显示文本], N'') AS [显示文本],
COALESCE([类型文本], N'') AS [类型文本],
COALESCE([值类别], N'') AS [值类别],
COALESCE([值文本], N'') AS [值文本],
COALESCE([辅助文本], N'') AS [辅助文本])SQL";
            if (!私有_读取SQL树直接子层("世界树子层", "[鱼巢].[当前世界树节点]", "[父节点主键]", 字段, 节点键, 行集, 错误)) {
                return 私有_SQL控制面板子链错误JSON(区段ID, 节点键, 错误);
            }
            std::vector<std::vector<std::string>> 关系行集{};
            if (!私有_读取SQL世界树关系行集(行集, 关系行集, 错误)) {
                return 私有_SQL控制面板子链错误JSON(区段ID, 节点键, 错误);
            }
            return 私有_SQL控制面板世界树子链JSON(区段ID, 节点键, 行集, 关系行集);
        }
        if (区段ID == "causalInfo") {
            constexpr std::string_view 字段 = R"SQL(
COALESCE([节点主键], N'') AS [节点主键],
COALESCE([父节点主键], N'') AS [父节点主键],
CONVERT(nvarchar(20), COALESCE([深度], 0)) AS [深度],
COALESCE([节点类型], N'') AS [节点类型],
COALESCE([显示文本], N'') AS [显示文本],
COALESCE([类型文本], N'') AS [类型文本],
COALESCE([值类别], N'') AS [值类别],
COALESCE([值文本], N'') AS [值文本],
COALESCE([辅助文本], N'') AS [辅助文本])SQL";
            if (!私有_读取SQL树直接子层("因果信息子层", "[鱼巢].[当前世界树节点]", "[父节点主键]", 字段, 节点键, 行集, 错误)) {
                return 私有_SQL控制面板子链错误JSON(区段ID, 节点键, 错误);
            }
            std::vector<std::vector<std::string>> 关系行集{};
            if (!私有_读取SQL世界树关系行集(行集, 关系行集, 错误)) {
                return 私有_SQL控制面板子链错误JSON(区段ID, 节点键, 错误);
            }
            return 私有_SQL控制面板世界树子链JSON(区段ID, 节点键, 行集, 关系行集);
        }
        return 私有_SQL控制面板子链错误JSON(区段ID, 节点键, "该区段不是按需树");
    }

    // 功能：按 SQL 控制面板菜单区段生成局部刷新 JSON。
    std::string 私有_SQL控制面板区段刷新JSON(std::string_view 区段ID)
    {
        结构_SQL控制面板数据 数据{};
        std::string 错误{};
        if (!私有_读取SQL控制面板数据(数据, 错误)) {
            std::ostringstream 输出;
            输出 << "{\"ok\":false,\"kind\":\"sql-section\",\"page\":";
            私有_追加SQL控制面板JSON字符串(输出, 区段ID);
            输出 << ",\"error\":";
            私有_追加SQL控制面板JSON字符串(输出, 错误);
            输出 << "}";
            return 输出.str();
        }

        if (区段ID == "metrics") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "面板指标", { "指标", "分组", "值", "来源", "文件" }, 数据.指标);
        }
        if (区段ID == "runtimeBatch") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "运行批次", { "运行ID", "启动时间", "来源", "进程ID", "同步开关", "状态" }, 数据.运行批次);
        }
        if (区段ID == "syncStatus") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "同步状态", { "同步域", "最近阶段", "状态", "更新时间", "最近错误" }, 数据.同步状态);
        }
        if (区段ID == "eventFlow") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "事件流水", { "时间", "领域", "操作", "对象", "摘要" }, 数据.事件流水);
        }
        if (区段ID == "currentTasks") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "当前任务", { "任务", "父任务", "状态值", "状态文本", "更新时间", "摘要" }, 数据.当前任务);
        }
        if (区段ID == "currentDemands") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "当前需求", { "需求", "父需求", "满足掩码", "已满足", "更新时间", "摘要" }, 数据.当前需求);
        }
        if (区段ID == "currentMethods") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "当前方法", { "方法", "父方法", "状态值", "更新时间", "摘要" }, 数据.当前方法);
        }
        if (区段ID == "currentWorld") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "当前世界", { "对象", "父对象", "类型", "更新时间", "摘要" }, 数据.当前世界);
        }
        if (区段ID == "currentDynamics") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "当前动态", { "动态", "来源", "类型", "更新时间", "摘要" }, 数据.当前动态);
        }
        if (区段ID == "threads") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "线程信息", { "逻辑ID", "线程名", "生命周期", "运行状态", "健康", "模块", "最近事件" }, 数据.线程);
        }
        if (区段ID == "threadEvents") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "线程生命周期事件", { "消息ID", "事件", "线程", "旧生命周期", "新生命周期", "原因", "摘要" }, 数据.线程事件);
        }
        if (区段ID == "actions") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "动作动态", { "时间", "类", "事件", "方法", "特征", "动作动态", "来源动态" }, 数据.动作动态);
        }
        if (区段ID == "causalInfo") {
            return 私有_SQL控制面板因果信息刷新JSON(区段ID, 数据);
        }
        if (区段ID == "causalChain") {
            return 私有_SQL控制面板因果链刷新JSON(区段ID, 数据);
        }
        if (区段ID == "demandTree") {
            return 私有_SQL控制面板表格区段刷新JSON(
                区段ID,
                "需求树",
                {
                    "节点序号", "父序号", "深度", "名称", "树形态", "目标语义", "目标特征", "任务",
                    "需求节点主键", "逻辑组织类型", "目标特征类型", "目标特征主键", "任务类型", "任务主键",
                    "需求主体名称", "需求主体类型", "需求主体主键",
                    "需求场景名称", "需求场景类型", "需求场景主键",
                    "目标宿主名称", "目标宿主类型", "目标宿主主键",
                    "当前状态名称", "当前状态类型", "当前状态主键",
                    "目标状态名称", "目标状态类型", "目标状态主键",
                    "满足关系掩码", "安全权重", "服务权重", "累计安全结算", "累计服务结算", "有效截止",
                    "最近结算任务名称", "最近结算任务类型", "最近结算任务主键", "最近结算时间",
                    "描述信息主键", "统计创建时间", "统计最后观测时间", "统计命中次数", "已截止", "阻塞父任务"
                },
                数据.需求树);
        }
        if (区段ID == "taskTree") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "任务树", { "节点", "父节点", "深度", "节点种类", "任务状态", "需求", "目标状态", "结果状态" }, 数据.任务树);
        }
        if (区段ID == "methodTree") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "方法树", { "节点", "父节点", "深度", "节点种类", "动作名", "动作句柄", "来源", "主结果特征", "结果数" }, 数据.方法树);
        }
        if (区段ID == "worldTree") {
            return 私有_SQL控制面板世界树刷新JSON(区段ID, 数据);
        }
        if (区段ID == "worldRelations") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "世界树关系", { "宿主", "关系", "目标类", "目标", "目标显示", "序号" }, 数据.世界树关系);
        }
        if (区段ID == "lexemeTree") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "语素树", { "节点", "父节点", "深度", "类别", "词面", "入口类型", "主信息类型", "基础信息" }, 数据.语素树);
        }
        if (区段ID == "features") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "特征类型", { "特征", "来源", "符号", "文件", "行" }, 数据.特征);
        }
        if (区段ID == "catalog") {
            return 私有_SQL控制面板表格区段刷新JSON(区段ID, "控制面板字段目录", { "字段", "分组", "C++类型", "结构", "文件", "行" }, 数据.字段目录);
        }

        std::ostringstream 输出;
        输出 << "{\"ok\":false,\"kind\":\"sql-section\",\"page\":";
        私有_追加SQL控制面板JSON字符串(输出, 区段ID);
        输出 << ",\"error\":\"未知 SQL 控制面板区段\"}";
        return 输出.str();
    }

    // 功能：判断页面名是否对应 SQL 控制面板菜单区段。
    bool 私有_是SQL控制面板区段(std::string_view 区段ID) noexcept
    {
        return 区段ID == "metrics"
            || 区段ID == "runtimeBatch"
            || 区段ID == "syncStatus"
            || 区段ID == "eventFlow"
            || 区段ID == "currentTasks"
            || 区段ID == "currentDemands"
            || 区段ID == "currentMethods"
            || 区段ID == "currentWorld"
            || 区段ID == "currentDynamics"
            || 区段ID == "threads"
            || 区段ID == "threadEvents"
            || 区段ID == "actions"
            || 区段ID == "causalInfo"
            || 区段ID == "causalChain"
            || 区段ID == "demandTree"
            || 区段ID == "taskTree"
            || 区段ID == "methodTree"
            || 区段ID == "worldTree"
            || 区段ID == "worldRelations"
            || 区段ID == "lexemeTree"
            || 区段ID == "features"
            || 区段ID == "catalog";
    }

    // 功能：根据 SQL 控制面板数据生成静态 HTML。
    std::string 私有_生成SQL控制面板HTML(const 结构_SQL控制面板数据& 数据)
    {
        const auto 批次ID = 私有_SQL字段(数据.批次, 0);
        const auto 创建时间 = 私有_SQL字段(数据.批次, 1);
        const auto 工作区 = 私有_SQL字段(数据.批次, 2);
        const auto 来源说明 = 私有_SQL字段(数据.批次, 3);
        const bool 初始待加载 = 批次ID == "待加载";
        std::ostringstream 输出;
        输出 << "<!doctype html>\n"
            << 私有_SQL控制面板HTML头部规则备注
            << R"HTML(<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>鱼巢控制面板</title>
  <style>
    :root{color-scheme:light;font-family:"Microsoft YaHei UI","Segoe UI",sans-serif;--bg:#f4f6f8;--surface:#fff;--line:#d8dee7;--ink:#172026;--muted:#5d6977;--accent:#0f766e;--blue:#2563eb}
    *{box-sizing:border-box} html,body{height:100%} body{margin:0;background:var(--bg);color:var(--ink);display:flex;flex-direction:column;overflow:hidden}
    header{flex:0 0 auto;padding:10px 24px 12px;background:#18212f;color:#fff} h1{margin:0 0 4px;font-size:20px;letter-spacing:0} header p{margin:2px 0;color:#d7deea}
    .top-actions{display:flex;flex-wrap:wrap;gap:8px;margin-top:10px}.top-actions button{border:1px solid rgba(255,255,255,.35);background:#fff;color:#172026;border-radius:6px;padding:7px 11px;cursor:pointer}.top-actions button.secondary{background:#263244;color:#fff}.top-actions span{align-self:center;color:#d7deea;font-size:13px}
    .panel-shell{flex:1 1 auto;min-height:0;display:grid;grid-template-columns:220px minmax(360px,.9fr) minmax(380px,1.1fr);gap:0}
    .menu-bar{background:#fff;border-right:1px solid var(--line);padding:14px 12px;min-height:0;overflow:auto}
    .menu-group{display:grid;gap:6px}.menu-title{font-size:12px;color:var(--muted);margin:10px 8px 4px}.menu-bar button{width:100%;border:1px solid transparent;background:#fff;border-radius:6px;padding:9px 10px;text-align:left;color:var(--ink);cursor:pointer}.menu-bar button:hover{background:#eef6f5}.menu-bar button.active{background:#0f766e;border-color:#0f766e;color:#fff}
    main.content{padding:18px 18px 36px;min-width:0;min-height:0;overflow:auto}.cards{display:grid;grid-template-columns:repeat(auto-fit,minmax(180px,1fr));gap:10px;margin:0 0 16px}
    .node-detail-pane{background:#fff;border-left:1px solid var(--line);padding:18px 18px 36px;min-height:0;overflow:auto}
    .card{background:var(--surface);border:1px solid var(--line);border-radius:8px;padding:12px}.card b{display:block;font-size:22px;margin-bottom:4px}
    input{width:min(720px,100%);padding:10px 12px;border:1px solid #cbd5e1;border-radius:6px;margin-bottom:12px}
    section{display:none;background:var(--surface);border:1px solid var(--line);border-radius:8px;padding:14px}section.active{display:block}h2{font-size:18px;margin:0 0 12px}
    .table-wrap{overflow:auto;max-height:68vh;border:1px solid #e5e7eb}table{border-collapse:collapse;width:100%;font-size:13px}th,td{border-bottom:1px solid #e5e7eb;padding:8px 10px;text-align:left;vertical-align:top;white-space:nowrap}th{position:sticky;top:0;background:#f8fafc;z-index:1}
    .table-wrap[hidden]{display:none}.world-tree-grid,.sql-section-tree{display:block}.tree-panel{border:1px solid #e5e7eb;border-radius:8px;background:#fbfdff;overflow:hidden}.tree-toolbar{display:flex;gap:8px;align-items:center;padding:9px;border-bottom:1px solid #e5e7eb;background:#f8fafc}.tree-toolbar button{border:1px solid #cbd5e1;background:#fff;border-radius:6px;padding:7px 10px;cursor:pointer}.tree-view{max-height:68vh;overflow:auto;padding:10px;font-size:13px}.tree-node{margin:2px 0}.tree-node summary{cursor:pointer;list-style:none}.tree-node summary::-webkit-details-marker{display:none}.tree-node summary::before{content:"▸";display:inline-block;width:16px;color:#64748b}.tree-node[open]>summary::before{content:"▾"}.tree-leaf{padding-left:16px}.tree-line{display:flex;gap:8px;align-items:center;min-height:26px;padding:3px 6px;border-radius:5px;white-space:nowrap;overflow:hidden}.tree-line:hover{background:#eef6f5}.tree-line.selected{background:#dff2ef;outline:1px solid #8fc9c1}.tree-line span{min-width:0}.tree-key{flex:0 0 auto;font-family:Consolas,monospace;color:#1d4ed8}.tree-kind{flex:0 0 auto;color:#0f766e}.tree-title,.tree-muted{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.tree-title{color:var(--ink)}.tree-muted{color:#64748b}.tree-children{margin-left:18px;border-left:1px solid #dbe4ee;padding-left:8px}
    .causal-detail{border:1px solid #e5e7eb;border-radius:8px;background:#fbfdff;min-height:420px;max-height:68vh;overflow:auto;padding:12px}.causal-detail-empty{color:var(--muted);font-size:14px}.detail-head{display:grid;gap:4px;border-bottom:1px solid #e5e7eb;padding-bottom:10px;margin-bottom:10px}.detail-head strong{font-size:16px}.detail-block{border:1px solid #dbe4ee;border-radius:8px;background:#fff;margin:10px 0;padding:10px}.detail-block h3{margin:0 0 8px;font-size:15px}.detail-item{border-top:1px solid #edf2f7;padding:9px 0}.detail-item:first-of-type{border-top:0}.detail-item-title{font-weight:600;margin-bottom:6px}.detail-row{display:grid;grid-template-columns:120px minmax(0,1fr);gap:8px;font-size:13px;line-height:1.5}.detail-row span{color:var(--muted)}.detail-row b{font-weight:400;word-break:break-all}.detail-sub{margin:8px 0 0 10px;border-left:2px solid #bfdcd8;padding-left:10px}.detail-sub-title{font-size:12px;color:var(--muted);margin-bottom:5px}.detail-sub-row{display:grid;grid-template-columns:120px minmax(0,1fr);gap:8px;font-size:12px;line-height:1.5;padding:3px 0}.detail-sub-row b{font-weight:500}.detail-sub-row span{word-break:break-all}
    .chain-grid{display:grid;grid-template-columns:1fr 1fr auto;gap:10px;align-items:start;margin-bottom:12px}.chain-grid label{display:grid;gap:5px;font-size:13px;color:var(--muted)}.chain-grid input{width:100%;margin:0}.chain-grid button{height:39px;border:1px solid var(--blue);background:var(--blue);color:#fff;border-radius:6px;padding:0 14px;cursor:pointer}.chain-result{margin:8px 0 12px;color:var(--muted)}
    @media(max-width:1100px){body{overflow:auto}.panel-shell{display:block;overflow:visible}.menu-bar,.node-detail-pane,main.content{overflow:visible}.menu-bar,.node-detail-pane{border-right:0;border-left:0;border-bottom:1px solid var(--line)}.menu-group{grid-template-columns:repeat(auto-fit,minmax(130px,1fr))}.menu-title{grid-column:1/-1}}
    @media(max-width:760px){main.content{padding:14px}.chain-grid{grid-template-columns:1fr}.chain-grid button{width:100%}}
    .note{color:var(--muted);font-size:13px;line-height:1.6}code{background:#eef2f7;color:#172026;padding:1px 4px;border-radius:4px}
  </style>
</head>
<body>
)HTML";
        输出 << "<header><h1>鱼巢控制面板</h1>"
            << "<p>数据来源：ADO / SQL Server <code>.\\SQLEXPRESS</code> / <code>鱼巢投影库</code> / <code>[鱼巢]</code> SQL 投影视图</p>"
            << "<p>批次：<code>" << 私有_转义HTML(批次ID) << "</code>；创建时间：" << 私有_转义HTML(创建时间)
            << "；工作区：" << 私有_转义HTML(工作区) << "</p>"
            << "<div class=\"top-actions\">"
            << "<button id=\"openCameraWindow\" class=\"secondary\" type=\"button\">打开相机窗口</button>"
            << "<span id=\"panelStatus\">"
            << (初始待加载 ? "数据加载中，等待自我运行写入 SQL 投影" : "点击菜单只刷新当前页面数据")
            << "</span></div></header>\n"
            << "<div class=\"panel-shell\"><aside class=\"menu-bar\"><nav class=\"menu-group\" aria-label=\"控制面板菜单\">"
            << "<div class=\"menu-title\">运行</div>"
            << "<button class=\"active\" data-menu-index=\"1\" data-target=\"metrics\">1. 面板指标</button>"
            << "<button data-menu-index=\"2\" data-target=\"runtimeBatch\">2. 运行批次</button>"
            << "<button data-menu-index=\"3\" data-target=\"syncStatus\">3. 同步状态</button>"
            << "<button data-menu-index=\"4\" data-target=\"eventFlow\">4. 事件流水</button>"
            << "<button data-menu-index=\"5\" data-target=\"currentTasks\">5. 当前任务</button>"
            << "<button data-menu-index=\"6\" data-target=\"currentDemands\">6. 当前需求</button>"
            << "<button data-menu-index=\"7\" data-target=\"currentMethods\">7. 当前方法</button>"
            << "<button data-menu-index=\"8\" data-target=\"currentWorld\">8. 当前世界</button>"
            << "<button data-menu-index=\"9\" data-target=\"currentDynamics\">9. 当前动态</button>"
            << "<button data-menu-index=\"10\" data-target=\"threads\">10. 线程信息</button>"
            << "<button data-menu-index=\"11\" data-target=\"threadEvents\">11. 线程事件</button>"
            << "<button data-menu-index=\"12\" data-target=\"actions\">12. 动作动态</button>"
            << "<div class=\"menu-title\">治理</div>"
            << "<button data-menu-index=\"13\" data-target=\"causalInfo\">13. 因果信息</button>"
            << "<button data-menu-index=\"14\" data-target=\"causalChain\">14. 因果链查询</button>"
            << "<button data-menu-index=\"15\" data-target=\"demandTree\">15. 需求树</button>"
            << "<button data-menu-index=\"16\" data-target=\"taskTree\">16. 任务树</button>"
            << "<button data-menu-index=\"17\" data-target=\"methodTree\">17. 方法树</button>"
            << "<button data-menu-index=\"18\" data-target=\"worldTree\">18. 世界树</button>"
            << "<button data-menu-index=\"19\" data-target=\"worldRelations\">19. 世界关系</button>"
            << "<button data-menu-index=\"20\" data-target=\"lexemeTree\">20. 语素树</button>"
            << "<div class=\"menu-title\">基础</div>"
            << "<button data-menu-index=\"21\" data-target=\"features\">21. 特征类型</button>"
            << "<button data-menu-index=\"22\" data-target=\"catalog\">22. 字段目录</button>"
            << "</nav></aside><main class=\"content\">\n"
            << "<input id=\"filter\" type=\"search\" placeholder=\"过滤当前页表格和世界树文本\">\n"
            << "<section id=\"metrics\" class=\"active\" data-section-title=\"面板指标\"><h2>面板指标</h2>\n"
            << "<p class=\"note\">本页面通过 ADO 读取 SQL 投影，不读取 live 控制面板快照，也不是世界树、任务树或动作动态链的写入口。"
            << 私有_转义HTML(来源说明) << "</p>\n"
            << "<div class=\"cards\">";
        for (const auto& 行 : 数据.指标) {
            输出 << "<div class=\"card\"><b>" << 私有_转义HTML(私有_SQL字段(行, 2))
                << "</b><span>" << 私有_转义HTML(私有_SQL字段(行, 0))
                << "</span><div class=\"note\">" << 私有_转义HTML(私有_SQL字段(行, 1))
                << "</div></div>";
        }
        输出 << "</div><div class=\"table-wrap\"><table data-filterable><thead><tr>"
            << "<th>指标</th><th>分组</th><th>值</th><th>来源</th><th>文件</th>"
            << "</tr></thead><tbody>\n";
        for (const auto& 行 : 数据.指标) {
            输出 << "<tr>";
            for (std::size_t i = 0; i < 5; ++i) {
                输出 << "<td>" << 私有_转义HTML(私有_SQL字段(行, i)) << "</td>";
            }
            输出 << "</tr>\n";
        }
        输出 << "</tbody></table></div></section>\n";

        私有_追加SQL控制面板表(输出, "运行批次", "runtimeBatch", { "运行ID", "启动时间", "来源", "进程ID", "同步开关", "状态" }, 数据.运行批次);
        私有_追加SQL控制面板表(输出, "同步状态", "syncStatus", { "同步域", "最近阶段", "状态", "更新时间", "最近错误" }, 数据.同步状态);
        私有_追加SQL控制面板表(输出, "事件流水", "eventFlow", { "时间", "领域", "操作", "对象", "摘要" }, 数据.事件流水);
        私有_追加SQL控制面板表(输出, "当前任务", "currentTasks", { "任务", "父任务", "状态值", "状态文本", "更新时间", "摘要" }, 数据.当前任务);
        私有_追加SQL控制面板表(输出, "当前需求", "currentDemands", { "需求", "父需求", "满足掩码", "已满足", "更新时间", "摘要" }, 数据.当前需求);
        私有_追加SQL控制面板表(输出, "当前方法", "currentMethods", { "方法", "父方法", "状态值", "更新时间", "摘要" }, 数据.当前方法);
        私有_追加SQL控制面板表(输出, "当前世界", "currentWorld", { "对象", "父对象", "类型", "更新时间", "摘要" }, 数据.当前世界);
        私有_追加SQL控制面板表(输出, "当前动态", "currentDynamics", { "动态", "来源", "类型", "更新时间", "摘要" }, 数据.当前动态);
        私有_追加SQL控制面板表(输出, "线程信息", "threads", { "逻辑ID", "线程名", "生命周期", "运行状态", "健康", "模块", "最近事件" }, 数据.线程);
        私有_追加SQL控制面板表(输出, "线程生命周期事件", "threadEvents", { "消息ID", "事件", "线程", "旧生命周期", "新生命周期", "原因", "摘要" }, 数据.线程事件);
        私有_追加SQL控制面板表(输出, "动作动态", "actions", { "时间", "类", "事件", "方法", "特征", "动作动态", "来源动态" }, 数据.动作动态);
        输出 << "<section id=\"causalChain\"><h2>因果链查询</h2>"
            << "<div class=\"chain-grid\">"
            << "<label>因<input id=\"causeInput\" type=\"search\" placeholder=\"输入因果节点或目标主键\"></label>"
            << "<label>果<input id=\"effectInput\" type=\"search\" placeholder=\"输入目标主键或类别\"></label>"
            << "<button id=\"chainQuery\" type=\"button\">查询</button>"
            << "</div><div id=\"chainResult\" class=\"chain-result\">输入因和果后查询世界树因果关系链接。</div>"
            << "<div class=\"table-wrap\"><table><thead><tr><th>#</th><th>因</th><th>关系</th><th>果</th><th>证据</th></tr></thead><tbody id=\"chainRows\"></tbody></table></div></section>\n";
        输出 << "<section id=\"causalInfo\"><h2>因果信息</h2>"
            << "<div class=\"world-tree-grid\"><div class=\"tree-panel\">"
            << "<div class=\"tree-toolbar\"><button id=\"causalInfoExpand\" type=\"button\">展开两层</button>"
            << "<button id=\"causalInfoCollapse\" type=\"button\">收起</button>"
            << "<span class=\"note\">SQL 世界树根链因果根节点：" << 数据.因果信息.size()
            << "；组成关系：" << 数据.因果信息关系.size() << "</span></div>"
            << "<div id=\"causalInfoTreeView\" class=\"tree-view\"></div></div></div></section>\n";
        私有_追加SQL控制面板表(
            输出,
            "需求树",
            "demandTree",
            {
                "节点序号", "父序号", "深度", "名称", "树形态", "目标语义", "目标特征", "任务",
                "需求节点主键", "逻辑组织类型", "目标特征类型", "目标特征主键", "任务类型", "任务主键",
                "需求主体名称", "需求主体类型", "需求主体主键",
                "需求场景名称", "需求场景类型", "需求场景主键",
                "目标宿主名称", "目标宿主类型", "目标宿主主键",
                "当前状态名称", "当前状态类型", "当前状态主键",
                "目标状态名称", "目标状态类型", "目标状态主键",
                "满足关系掩码", "安全权重", "服务权重", "累计安全结算", "累计服务结算", "有效截止",
                "最近结算任务名称", "最近结算任务类型", "最近结算任务主键", "最近结算时间",
                "描述信息主键", "统计创建时间", "统计最后观测时间", "统计命中次数", "已截止", "阻塞父任务"
            },
            数据.需求树);
        私有_追加SQL控制面板表(输出, "任务树", "taskTree", { "节点", "父节点", "深度", "节点种类", "任务状态", "需求", "目标状态", "结果状态" }, 数据.任务树);
        私有_追加SQL控制面板表(输出, "方法树", "methodTree", { "节点", "父节点", "深度", "节点种类", "动作名", "动作句柄", "来源", "主结果特征", "结果数" }, 数据.方法树);
        std::size_t 世界树因果节点数 = 0;
        for (const auto& 行 : 数据.世界树) {
            if (私有_SQL字段(行, 3) == "因果") {
                ++世界树因果节点数;
            }
        }
        输出 << "<section id=\"worldTree\"><h2>世界树</h2>"
            << "<div class=\"world-tree-grid\"><div class=\"tree-panel\">"
            << "<div class=\"tree-toolbar\"><button id=\"worldTreeExpand\" type=\"button\">展开三层</button>"
            << "<button id=\"worldTreeCollapse\" type=\"button\">收起</button>"
            << "<button id=\"worldTreeShowCausal\" type=\"button\">显示因果</button>"
            << "<span class=\"note\">SQL 当前节点数：" << 数据.世界树.size()
            << "；因果节点：" << 世界树因果节点数 << "</span></div>"
            << "<div id=\"worldTreeView\" class=\"tree-view\"></div></div>"
            << "<div class=\"table-wrap\" hidden><table data-filterable><thead><tr>"
            << "<th>节点</th><th>父节点</th><th>深度</th><th>类别</th><th>显示</th><th>类型</th><th>值类</th><th>值</th><th>辅助</th>"
            << "</tr></thead><tbody>\n";
        for (const auto& 行 : 数据.世界树) {
            输出 << "<tr>";
            for (std::size_t i = 0; i < 9; ++i) {
                输出 << "<td>" << 私有_转义HTML(私有_SQL字段(行, i)) << "</td>";
            }
            输出 << "</tr>\n";
        }
        输出 << "</tbody></table></div></div></section>\n";
        私有_追加SQL控制面板表(输出, "世界树关系", "worldRelations", { "宿主", "关系", "目标类", "目标", "目标显示", "序号" }, 数据.世界树关系);
        私有_追加SQL控制面板表(输出, "语素树", "lexemeTree", { "节点", "父节点", "深度", "类别", "词面", "入口类型", "主信息类型", "基础信息" }, 数据.语素树);
        私有_追加SQL控制面板表(输出, "特征类型", "features", { "特征", "来源", "符号", "文件", "行" }, 数据.特征);
        私有_追加SQL控制面板表(输出, "控制面板字段目录", "catalog", { "字段", "分组", "C++类型", "结构", "文件", "行" }, 数据.字段目录);

        输出 << "</main><aside class=\"node-detail-pane\" aria-label=\"节点信息\">"
            << "<div id=\"sqlNodeDetail\" class=\"causal-detail\"><div class=\"causal-detail-empty\">请选择中间树节点查看信息</div></div>"
            << "</aside></div>\n<script>\nconst causalInfoRows=[";
        for (std::size_t i = 0; i < 数据.因果信息.size(); ++i) {
            if (i > 0) {
                输出 << ',';
            }
            const auto& 行 = 数据.因果信息[i];
            输出 << "{\"key\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 0));
            输出 << ",\"parent\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 1));
            输出 << ",\"depth\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 2));
            输出 << ",\"kind\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 3));
            输出 << ",\"display\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 4));
            输出 << ",\"type\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 5));
            输出 << ",\"valueKind\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 6));
            输出 << ",\"value\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 7));
            输出 << ",\"aux\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 8));
            输出 << '}';
        }
        输出 << "];\nconst causalInfoRelations=[";
        for (std::size_t i = 0; i < 数据.因果信息关系.size(); ++i) {
            if (i > 0) {
                输出 << ',';
            }
            const auto& 行 = 数据.因果信息关系[i];
            输出 << "{\"owner\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 0));
            输出 << ",\"relation\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 1));
            输出 << ",\"targetKind\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 2));
            输出 << ",\"targetKey\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 3));
            输出 << ",\"targetText\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 4));
            输出 << ",\"ordinal\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 5));
            输出 << '}';
        }
        输出 << "];\nconst worldTreeRows=[";
        for (std::size_t i = 0; i < 数据.世界树.size(); ++i) {
            if (i > 0) {
                输出 << ',';
            }
            const auto& 行 = 数据.世界树[i];
            输出 << "{\"key\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 0));
            输出 << ",\"parent\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 1));
            输出 << ",\"depth\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 2));
            输出 << ",\"kind\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 3));
            输出 << ",\"display\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 4));
            输出 << ",\"type\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 5));
            输出 << ",\"valueKind\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 6));
            输出 << ",\"value\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 7));
            输出 << ",\"aux\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 8));
            输出 << '}';
        }
        输出 << "];\nconst worldTreeRelations=[";
        for (std::size_t i = 0; i < 数据.世界树关系.size(); ++i) {
            if (i > 0) {
                输出 << ',';
            }
            const auto& 行 = 数据.世界树关系[i];
            输出 << "{\"owner\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 0));
            输出 << ",\"relation\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 1));
            输出 << ",\"targetKind\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 2));
            输出 << ",\"targetKey\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 3));
            输出 << ",\"targetText\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 4));
            输出 << ",\"ordinal\":";
            私有_追加SQL控制面板JSON字符串(输出, 私有_SQL字段(行, 5));
            输出 << '}';
        }
        输出 << "];\n";
        输出 << R"HTML(
const buttons=Array.from(document.querySelectorAll('button[data-target]'));
const sections=Array.from(document.querySelectorAll('section'));
const filter=document.getElementById('filter');
const causeInput=document.getElementById('causeInput');
const effectInput=document.getElementById('effectInput');
const chainRows=document.getElementById('chainRows');
const chainResult=document.getElementById('chainResult');
const causalInfoHost=document.getElementById('causalInfoTreeView');
const nodeDetail=document.getElementById('sqlNodeDetail');
const causalInfoDetail=nodeDetail;
const worldTreeHost=document.getElementById('worldTreeView');
const openCameraButton=document.getElementById('openCameraWindow');
const panelStatus=document.getElementById('panelStatus');
let causalInfoRoots=[];
let worldTreeRoots=[];
const genericTreeRootsBySection=new Map();
let selectedCausalInfoKey='';
let selectedCausalInfoNode=null;
let selectedWorldTreeNode=null;
let selectedGenericNode=null;
let menuNumberBuffer='';
let menuNumberTimer=0;
let sqlSubtreeRequestSeq=0;
let sqlRefreshRetryTimer=0;
const sqlSubtreeRequests=new Map();
const sqlSectionFingerprints=new Map();
const causalInfoByKey=new Map(causalInfoRows.map(row=>[row.key,row]));
const causalRelationsByOwner=new Map();
causalInfoRelations.forEach(rel=>{
  if(!causalRelationsByOwner.has(rel.owner))causalRelationsByOwner.set(rel.owner,[]);
  causalRelationsByOwner.get(rel.owner).push(rel);
});
const causalRelationEdges=causalInfoRelations.map(rel=>({
  sourceKind:'因果',
  sourceKey:rel.owner,
  targetKind:rel.targetKind,
  targetKey:rel.targetKey,
  relation:rel.relation,
  evidence:rel.targetText||`序号=${rel.ordinal||0}`
}));
const worldNodeByKey=new Map(worldTreeRows.map(row=>[row.key,row]));
const worldRelationsByOwner=new Map();
worldTreeRelations.forEach(rel=>{
  if(!worldRelationsByOwner.has(rel.owner))worldRelationsByOwner.set(rel.owner,[]);
  worldRelationsByOwner.get(rel.owner).push(rel);
});
function escapeHtml(text){
  return String(text??'').replace(/[&<>"']/g,ch=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[ch]));
}
function applyFilter(){
  const query=filter.value.trim().toLowerCase();
  const active=document.querySelector('section.active table[data-filterable]');
  if(active){
    for(const row of active.tBodies[0].rows){row.style.display=!query||row.textContent.toLowerCase().includes(query)?'':'none';}
  }
  filterCausalInfo(query);
  filterWorldTree(query);
  filterGenericTrees(query);
}
function fieldText(value){
  const text=String(value??'').trim();
  return text||'未记录';
}
function featureValueText(node){
  if(!node)return '未记录';
  const kind=String(node.valueKind??'').trim();
  const value=String(node.value??'').trim();
  if(!kind&&!value)return '未记录';
  return `${kind} ${value}`.trim();
}
function parseAuxFields(text){
  const fields=new Map();
  String(text??'').split(/\s*[|；;]\s*/).forEach(part=>{
    const index=part.indexOf('=');
    if(index<0)return;
    const key=part.slice(0,index).trim();
    const value=part.slice(index+1).trim();
    if(key)fields.set(key,value);
  });
  return fields;
}
function isConditionRelation(name){
  return /条件|因比较/.test(String(name??''));
}
function isResultRelation(name){
  return /主果|果比较/.test(String(name??''));
}
function isActionRelation(name){
  return /因动作|动作|因方法|方法模板/.test(String(name??''));
}
function renderDetailRows(rows){
  return rows.map(([label,value])=>`<div class="detail-row"><span>${escapeHtml(label)}</span><b>${escapeHtml(fieldText(value))}</b></div>`).join('');
}
function showNodeDetail(title,subtitle,rows,extraHtml=''){
  if(!nodeDetail)return;
  nodeDetail.innerHTML=`<div class="detail-head"><strong>${escapeHtml(fieldText(title))}</strong><span>${escapeHtml(fieldText(subtitle))}</span></div><div class="detail-block"><h3>节点信息</h3>${renderDetailRows(rows)}</div>${extraHtml}`;
}
function sectionTitle(target){
  const section=document.getElementById(target||'');
  return section?.dataset.sectionTitle||section?.querySelector('h2')?.textContent||target||'当前页面';
}
function activeSectionId(){
  return buttons.find(item=>item.classList.contains('active'))?.dataset.target||'';
}
function isSectionActive(target){
  return activeSectionId()===(target||'');
}
function stableDataFingerprint(value){
  try{return JSON.stringify(value??null);}
  catch(_){return String(value??'');}
}
function isUnchangedSection(page,fingerprint){
  const key=String(page||'');
  const unchanged=sqlSectionFingerprints.has(key)&&sqlSectionFingerprints.get(key)===fingerprint;
  if(!unchanged)sqlSectionFingerprints.set(key,fingerprint);
  return unchanged;
}
function isUnchangedSubtree(row,fingerprint){
  if(!row)return false;
  const unchanged=!!row.__sqlSubtreeLoaded&&row.__sqlSubtreeFingerprint===fingerprint;
  if(!unchanged)row.__sqlSubtreeFingerprint=fingerprint;
  return unchanged;
}
function clearSelectionForSection(target){
  if(target==='causalInfo'){
    selectedCausalInfoKey='';
    selectedCausalInfoNode=null;
    updateCausalSelection();
    return;
  }
  if(target==='worldTree'){
    selectedWorldTreeNode=null;
    updateWorldSelection();
    return;
  }
  if(genericTreeRootsBySection.has(target)){
    selectedGenericNode=null;
    updateGenericSelection();
  }
}
function showSectionState(target,status,detail){
  const title=sectionTitle(target);
  showNodeDetail(title,status||'SQL 投影视图',[
    ['页面',title],
    ['状态',detail||status||'等待刷新'],
    ['数据源','ADO / SQL Server']
  ]);
}
function renderOneLayerChildren(ownerKey){
  const relations=worldRelationsByOwner.get(ownerKey)||[];
  if(!relations.length)return '';
  const visible=relations.slice(0,24).map(rel=>{
    const node=worldNodeByKey.get(rel.targetKey);
    const target=node?(node.display||node.type||node.value||node.key):(rel.targetText||rel.targetKey);
    const detail=node?`特征类型=${fieldText(node.type)}；特征值范围/具体值=${featureValueText(node)}；类别=${fieldText(node.kind)}`:fieldText(rel.targetText);
    return `<div class="detail-sub-row"><b>${escapeHtml(fieldText(rel.relation))}</b><span>${escapeHtml(fieldText(rel.targetKind))}:${escapeHtml(fieldText(rel.targetKey))} ${escapeHtml(target)} | ${escapeHtml(detail)}</span></div>`;
  }).join('');
  const more=relations.length>24?`<div class="detail-sub-row"><b>其余</b><span>${relations.length-24} 条</span></div>`:'';
  return `<div class="detail-sub"><div class="detail-sub-title">子特征一层 ${relations.length}</div>${visible}${more}</div>`;
}
function renderFeatureReference(rel){
  const node=worldNodeByKey.get(rel.targetKey);
  const title=node?(node.display||node.type||node.value||node.key):(rel.targetText||rel.targetKey);
  return `<div class="detail-item"><div class="detail-item-title">${escapeHtml(fieldText(rel.relation))} | ${escapeHtml(title)}</div>${renderDetailRows([
    ['目标', `${rel.targetKind||''}:${rel.targetKey||''}`],
    ['特征类型', node?node.type:rel.targetText],
    ['特征值范围/具体值', node?featureValueText(node):'未记录'],
    ['节点类别', node?node.kind:rel.targetKind],
    ['显示', node?node.display:rel.targetText],
    ['辅助', node?node.aux:'']
  ])}${node?renderOneLayerChildren(node.key):''}</div>`;
}
function renderReferenceBlock(title,relations){
  const body=relations.length?relations.map(renderFeatureReference).join(''):'<div class="causal-detail-empty">未记录</div>';
  return `<div class="detail-block"><h3>${escapeHtml(title)}</h3>${body}</div>`;
}
function renderActionBlock(row,relations){
  const fields=parseAuxFields(row.aux);
  const actionName=fields.get('因动作名称')||fields.get('动作名称')||'';
  const methodTemplate=fields.get('因方法模板')||'';
  const relationHtml=relations.length?relations.map(rel=>`<div class="detail-item">${renderDetailRows([
    ['关系', rel.relation],
    ['目标', `${rel.targetKind||''}:${rel.targetKey||''}`],
    ['名称', rel.targetText],
    ['序号', rel.ordinal]
  ])}</div>`).join(''):'';
  return `<div class="detail-block"><h3>动作</h3><div class="detail-item">${renderDetailRows([
    ['动作名称', actionName],
    ['因方法模板', methodTemplate]
  ])}</div>${relationHtml}</div>`;
}
function visitTreeRows(rows,visitor){
  rows.forEach(row=>{
    visitor(row);
    if(row.children)visitTreeRows(row.children,visitor);
  });
}
function updateCausalSelection(){
  visitTreeRows(causalInfoRoots,row=>{
    if(row.lineEl)row.lineEl.classList.toggle('selected',row===selectedCausalInfoNode);
  });
}
function selectCausalInfo(key,node=null){
  selectedCausalInfoKey=key;
  selectedCausalInfoNode=node;
  const row=causalInfoByKey.get(key);
  if(!row||!causalInfoDetail){
    if(causalInfoDetail)causalInfoDetail.innerHTML='<div class="causal-detail-empty">未选择因果信息</div>';
    updateCausalSelection();
    return;
  }
  const relations=causalRelationsByOwner.get(key)||[];
  const conditionRelations=relations.filter(rel=>isConditionRelation(rel.relation));
  const resultRelations=relations.filter(rel=>isResultRelation(rel.relation));
  const actionRelations=relations.filter(rel=>isActionRelation(rel.relation)&&!isConditionRelation(rel.relation)&&!isResultRelation(rel.relation));
  causalInfoDetail.innerHTML=`<div class="detail-head"><strong>${escapeHtml(row.display||row.type||row.key)}</strong><span><span class="tree-key">${escapeHtml(row.key)}</span> ${escapeHtml(row.kind||'因果')}</span><span class="tree-muted">${escapeHtml(row.aux||'')}</span></div>${renderReferenceBlock('条件',conditionRelations)}${renderActionBlock(row,actionRelations)}${renderReferenceBlock('结果',resultRelations)}`;
  updateCausalSelection();
}
function selectCausalInfoNode(node,loadSubtree=false){
  if(!node)return;
  if(node.relationRow){
    selectedCausalInfoNode=node;
    const rel=node.relationRow;
    showNodeDetail(`${rel.relation||'因果关系'} -> ${rel.targetKey||rel.targetText||'未记录'}`,`宿主因果 ${node.ownerKey||''}`,[
      ['宿主因果',node.ownerKey],
      ['关系',rel.relation],
      ['目标类别',rel.targetKind],
      ['目标主键',rel.targetKey],
      ['目标显示',rel.targetText],
      ['序号',rel.ordinal]
    ]);
    updateCausalSelection();
    return;
  }
  if(node.kind&&node.kind!=='因果'){
    selectedCausalInfoKey=node.key;
    selectedCausalInfoNode=node;
    const relations=causalRelationsByOwner.get(node.key)||[];
    const relationHtml=relations.length?`<div class="detail-block"><h3>子关系 ${relations.length}</h3>${relations.slice(0,48).map(rel=>`<div class="detail-item">${renderDetailRows([
      ['关系',rel.relation],
      ['目标',`${rel.targetKind||''}:${rel.targetKey||''}`],
      ['显示',rel.targetText],
      ['序号',rel.ordinal]
    ])}</div>`).join('')}${relations.length>48?`<div class="detail-sub-row"><b>其余</b><span>${relations.length-48} 条</span></div>`:''}</div>`:'';
    showNodeDetail(node.display||node.type||node.key,`${node.kind||'世界树节点'} | ${node.key}`,[
      ['节点主键',node.key],
      ['父节点',node.parent],
      ['深度',node.depth],
      ['类别',node.kind],
      ['显示',node.display],
      ['类型',node.type],
      ['值类',node.valueKind],
      ['值',node.value],
      ['辅助',node.aux]
    ],relationHtml);
    updateCausalSelection();
    if(loadSubtree)requestSQLCausalInfoSubtree(node);
    return;
  }
  selectCausalInfo(node.key,node);
  if(loadSubtree)requestSQLCausalInfoSubtree(node);
}
function treeLabel(row){
  const parts=[`<span class="tree-key">${escapeHtml(row.key)}</span>`];
  const display=String(row.display||'').trim();
  const type=String(row.type||'').trim();
  const value=String(row.value||'').trim();
  const valueKind=String(row.valueKind||'').trim();
  if(row.kind)parts.push(`<span class="tree-kind">${escapeHtml(row.kind)}</span>`);
  if(display)parts.push(`<span>${escapeHtml(display)}</span>`);
  if(type&&type!==display)parts.push(`<span class="tree-muted">类型=${escapeHtml(type)}</span>`);
  if(valueKind||value)parts.push(`<span class="tree-muted">${escapeHtml(valueKind)} ${escapeHtml(value)}</span>`);
  if(row.aux)parts.push(`<span class="tree-muted">${escapeHtml(row.aux)}</span>`);
  return parts.join('');
}
function renderTreeNode(row, labelFn){
  const hasChildren=Array.isArray(row.children)&&row.children.length>0;
  const wrapper=document.createElement(hasChildren?'details':'div');
  wrapper.className=hasChildren?'tree-node':'tree-node tree-leaf';
  wrapper.dataset.search=row.searchText||'';
  row.el=wrapper;
  if(hasChildren&&(row.open||Number(row.depth||0)<2))wrapper.open=true;
  if(hasChildren)wrapper.addEventListener('toggle',()=>{row.open=wrapper.open;});
  const lineTag=hasChildren?'summary':'div';
  const line=document.createElement(lineTag);
  line.className='tree-line';
  line.innerHTML=labelFn(row);
  row.lineEl=line;
  if(row.onSelect){
    line.tabIndex=0;
    line.addEventListener('click',event=>{event.stopPropagation();row.onSelect(row);});
    line.addEventListener('keydown',event=>{if(event.key==='Enter'||event.key===' '){event.preventDefault();row.onSelect(row);}});
  }
  wrapper.appendChild(line);
  if(hasChildren){
    const children=document.createElement('div');
    children.className='tree-children';
    row.children.forEach(child=>children.appendChild(renderTreeNode(child,labelFn)));
    wrapper.appendChild(children);
  }
  return wrapper;
}
)HTML";
        输出 << R"HTML(
function genericNodeLabel(row){
  const parts=[`<span class="tree-key">${escapeHtml(row.key||'节点')}</span>`];
  if(row.kind)parts.push(`<span class="tree-kind">${escapeHtml(row.kind)}</span>`);
  if(row.display)parts.push(`<span>${escapeHtml(row.display)}</span>`);
  if(row.summary)parts.push(`<span class="tree-muted">${escapeHtml(row.summary)}</span>`);
  return parts.join('');
}
function updateGenericSelection(){
  genericTreeRootsBySection.forEach(roots=>{
    visitTreeRows(roots,row=>{
      if(row.lineEl)row.lineEl.classList.toggle('selected',row===selectedGenericNode);
    });
  });
}
function countGenericNodes(roots){
  let count=0;
  visitTreeRows(roots||[],()=>{count+=1;});
  return count;
}
function findGenericNode(sectionId,key){
  let found=null;
  visitTreeRows(genericTreeRootsBySection.get(sectionId)||[],row=>{
    if(!found&&row.key===key)found=row;
  });
  return found;
}
function renderGenericTreePanel(section){
  if(!section)return;
  const roots=genericTreeRootsBySection.get(section.id)||[];
  const panel=section.querySelector('.sql-section-tree');
  if(!panel)return;
  const note=panel.querySelector('.note');
  if(note)note.textContent=`SQL 节点：${countGenericNodes(roots)}`;
  const view=panel.querySelector('.tree-view');
  if(!view)return;
  view.innerHTML='';
  roots.forEach(root=>view.appendChild(renderTreeNode(root,genericNodeLabel)));
  updateGenericSelection();
}
function requestSQLGenericSubtree(row){
  if(!row||!row.lazySubtree||row.__sqlSubtreeLoading)return;
  if(!(window.chrome&&window.chrome.webview)){
    if(panelStatus)panelStatus.textContent='静态 HTML 预览未连接子链加载接口。';
    return;
  }
  row.__sqlSubtreeLoading=true;
  row.open=true;
  const requestId=++sqlSubtreeRequestSeq;
  sqlSubtreeRequests.set(requestId,{kind:'generic',sectionId:row.sectionId,nodeKey:row.key});
  if(panelStatus)panelStatus.textContent=`正在加载子节点：${row.key}`;
  window.chrome.webview.postMessage(`sql-subtree:${requestId}:${row.sectionId}:${encodeURIComponent(row.key||'')}`);
}
function selectGenericNode(row){
  if(!row)return;
  selectedGenericNode=row;
  const rows=row.headers.map((header,index)=>[header||`字段${index+1}`,row.cells[index]||'']);
  showNodeDetail(row.title||row.key,row.subtitle||row.kind||'SQL 行节点',rows);
  updateGenericSelection();
  requestSQLGenericSubtree(row);
}
function createGenericRowNode(sectionId,sectionTitle,headers,cells,index,treeLike){
  const key=fieldText(cells[0]||`${sectionTitle}#${index+1}`);
  let kind=treeLike?fieldText(cells[3]):fieldText(cells[1]||sectionTitle);
  let display=treeLike?fieldText(cells[4]||cells[5]||cells[6]||''):fieldText(cells[2]||cells[1]||'');
  let summary=headers.slice(1,4).map((header,i)=>`${header}=${fieldText(cells[i+1])}`).join(' | ');
  let title=key;
  let subtitle=sectionTitle;
  if(sectionId==='demandTree'){
    const name=fieldText(cells[3]||cells[6]||key);
    const shape=fieldText(cells[4]||'');
    const targetFeature=fieldText(cells[6]||'');
    const task=fieldText(cells[7]||'');
    kind=name;
    display=shape;
    summary=[targetFeature&&`目标特征=${targetFeature}`,task&&`任务=${task}`].filter(Boolean).join(' | ');
    title=name||key;
    subtitle=`${sectionTitle} | 节点=${key}`;
  }
  const node={sectionId,key,parent:treeLike?String(cells[1]||''):'',depth:treeLike?String(cells[2]||'1'):'1',kind,display,summary,headers,cells,children:[],title,subtitle,lazySubtree:treeLike};
  node.searchText=[sectionTitle,key,kind,display,summary,...cells].join(' ').toLowerCase();
  node.onSelect=()=>selectGenericNode(node);
  return node;
}
function buildGenericHierarchy(nodes){
  const roots=[];
  const byKey=new Map();
  nodes.forEach(node=>{
    node.children=[];
    const depth=Number.parseInt(node.depth||'0',10);
    node.depthValue=Number.isFinite(depth)&&depth>=0?depth:0;
    if(node.key)byKey.set(String(node.key),node);
  });
  let linkedByParent=false;
  nodes.forEach(node=>{
    const parentKey=String(node.parent||'').trim();
    const parent=parentKey?byKey.get(parentKey):null;
    if(parent&&parent!==node){
      parent.children.push(node);
      linkedByParent=true;
    }else{
      roots.push(node);
    }
  });
  if(linkedByParent)return roots.length?roots:nodes;
  roots.length=0;
  nodes.forEach(node=>{node.children=[];});
  const stack=[];
  nodes.forEach(node=>{
    while(stack.length&&stack[stack.length-1].depthValue>=node.depthValue)stack.pop();
    const parent=stack.length?stack[stack.length-1]:null;
    if(parent)parent.children.push(node);else roots.push(node);
    stack.push(node);
  });
  return roots.length?roots:nodes;
}
const treeLikeSections=new Set(['demandTree','taskTree','methodTree','lexemeTree']);
function buildTableTreeSection(section){
  if(!section||['causalInfo','worldTree','causalChain'].includes(section.id))return;
  const table=section.querySelector('table[data-filterable]');
  if(!table||section.querySelector('.sql-section-tree'))return;
  const headers=Array.from(table.tHead?.rows?.[0]?.cells||[]).map(cell=>cell.textContent.trim());
  const rows=Array.from(table.tBodies?.[0]?.rows||[]).map((tr,index)=>{
    const cells=Array.from(tr.cells).map(cell=>cell.textContent.trim());
    return createGenericRowNode(section.id,section.dataset.sectionTitle||section.querySelector('h2')?.textContent||section.id,headers,cells,index,treeLikeSections.has(section.id));
  });
  const roots=treeLikeSections.has(section.id)?buildGenericHierarchy(rows):[{key:section.dataset.sectionTitle||section.id,depth:'0',kind:'页面',display:`${section.dataset.sectionTitle||section.id} ${rows.length}`,summary:'SQL 行节点',headers:['页面','数量'],cells:[section.dataset.sectionTitle||section.id,String(rows.length)],children:rows,searchText:rows.map(row=>row.searchText).join(' '),onSelect:null}];
  genericTreeRootsBySection.set(section.id,roots);
  const panel=document.createElement('div');
  panel.className='tree-panel sql-section-tree';
  panel.innerHTML=`<div class="tree-toolbar"><span class="note">SQL 节点：${rows.length}</span></div><div class="tree-view"></div>`;
  const view=panel.querySelector('.tree-view');
  roots.forEach(root=>view.appendChild(renderTreeNode(root,genericNodeLabel)));
  const wrap=table.closest('.table-wrap');
  if(wrap){
    section.insertBefore(panel,wrap);
    wrap.hidden=true;
  }else{
    section.appendChild(panel);
  }
}
function rebuildTableTreeSection(section){
  if(!section)return;
  section.querySelectorAll('.sql-section-tree').forEach(panel=>panel.remove());
  const wrap=section.querySelector('.table-wrap');
  if(wrap)wrap.hidden=false;
  genericTreeRootsBySection.delete(section.id);
  buildTableTreeSection(section);
}
function buildTableTreeSections(){
  sections.forEach(buildTableTreeSection);
}
)HTML";
        输出 << R"HTML(
function sqlCell(row,index){
  return Array.isArray(row)&&row[index]!=null?String(row[index]):'';
}
function sqlRowsToTreeRows(rows){
  return (Array.isArray(rows)?rows:[]).map(row=>({
    key:sqlCell(row,0),
    parent:sqlCell(row,1),
    depth:sqlCell(row,2),
    kind:sqlCell(row,3),
    display:sqlCell(row,4),
    type:sqlCell(row,5),
    valueKind:sqlCell(row,6),
    value:sqlCell(row,7),
    aux:sqlCell(row,8)
  }));
}
function sqlRowsToRelations(rows){
  return (Array.isArray(rows)?rows:[]).map(row=>({
    owner:sqlCell(row,0),
    relation:sqlCell(row,1),
    targetKind:sqlCell(row,2),
    targetKey:sqlCell(row,3),
    targetText:sqlCell(row,4),
    ordinal:sqlCell(row,5)
  }));
}
function resetCausalRelationIndexes(relations){
  causalInfoRelations.splice(0,causalInfoRelations.length,...relations);
  causalRelationsByOwner.clear();
  causalInfoRelations.forEach(rel=>{
    if(!causalRelationsByOwner.has(rel.owner))causalRelationsByOwner.set(rel.owner,[]);
    causalRelationsByOwner.get(rel.owner).push(rel);
  });
  causalRelationEdges.splice(0,causalRelationEdges.length,...causalInfoRelations.map(rel=>({
    sourceKind:'因果',
    sourceKey:rel.owner,
    targetKind:rel.targetKind,
    targetKey:rel.targetKey,
    relation:rel.relation,
    evidence:rel.targetText||`序号=${rel.ordinal||0}`
  })));
}
function mergeCausalInfoRelations(relations){
  const known=new Set(causalInfoRelations.map(rel=>`${rel.owner}|${rel.relation}|${rel.targetKey}|${rel.ordinal}`));
  relations.forEach(rel=>{
    const key=`${rel.owner}|${rel.relation}|${rel.targetKey}|${rel.ordinal}`;
    if(!known.has(key)){
      known.add(key);
      causalInfoRelations.push(rel);
    }
  });
}
function resetCausalInfoIndexes(rows,relations){
  causalInfoRows.splice(0,causalInfoRows.length,...rows);
  causalInfoByKey.clear();
  causalInfoRows.forEach(row=>causalInfoByKey.set(row.key,row));
  resetCausalRelationIndexes(relations);
}
function resetWorldTreeIndexes(rows,relations){
  worldTreeRows.splice(0,worldTreeRows.length,...rows);
  worldTreeRelations.splice(0,worldTreeRelations.length,...relations);
  worldNodeByKey.clear();
  worldTreeRows.forEach(row=>worldNodeByKey.set(row.key,row));
  worldRelationsByOwner.clear();
  worldTreeRelations.forEach(rel=>{
    if(!worldRelationsByOwner.has(rel.owner))worldRelationsByOwner.set(rel.owner,[]);
    worldRelationsByOwner.get(rel.owner).push(rel);
  });
}
function renderSQLTableSection(data){
  const section=document.getElementById(data.page||'');
  if(!section)return;
  const table=section.querySelector('table[data-filterable]');
  if(!table)return;
  const headers=Array.isArray(data.headers)?data.headers:[];
  const rows=Array.isArray(data.rows)?data.rows:[];
  if(isUnchangedSection(data.page,stableDataFingerprint([headers,rows]))){
    if(isSectionActive(data.page)&&!rows.length)安排SQL区段重试(data.page);
    return false;
  }
  const headRow=table.tHead?.rows?.[0];
  if(headRow)headRow.innerHTML=headers.map(header=>`<th>${escapeHtml(header)}</th>`).join('');
  const body=table.tBodies?.[0]||table.createTBody();
  body.innerHTML=rows.map(row=>`<tr>${headers.map((_,index)=>`<td>${escapeHtml(sqlCell(row,index))}</td>`).join('')}</tr>`).join('');
  if(data.page==='metrics'){
    const cards=section.querySelector('.cards');
    if(cards){
      cards.innerHTML=rows.map(row=>`<div class="card"><b>${escapeHtml(sqlCell(row,2))}</b><span>${escapeHtml(sqlCell(row,0))}</span><div class="note">${escapeHtml(sqlCell(row,1))}</div></div>`).join('');
    }
  }
  rebuildTableTreeSection(section);
  applyFilter();
  if(isSectionActive(data.page)){
    if(rows.length){
      selectDefaultForSection(data.page);
    }else{
      clearSelectionForSection(data.page);
      showSectionState(data.page,'数据加载中','当前 SQL 区段暂无行，继续等待投影写入。');
      安排SQL区段重试(data.page);
    }
  }
  return true;
}
function renderSQLCausalInfoSection(data){
  const rows=sqlRowsToTreeRows(data.rows);
  const relations=sqlRowsToRelations(data.relations);
  if(isUnchangedSection('causalInfo',stableDataFingerprint([data.rows||[],data.relations||[]]))){
    if(isSectionActive('causalInfo')&&!rows.length)安排SQL区段重试('causalInfo');
    return false;
  }
  resetCausalInfoIndexes(rows,relations);
  const note=document.querySelector('#causalInfo .tree-toolbar .note');
  if(note)note.textContent=`SQL 世界树根链因果根节点：${rows.length}；组成关系：${relations.length}`;
  buildCausalInfoTree();
  applyFilter();
  if(isSectionActive('causalInfo')){
    if(rows.length)selectDefaultForSection('causalInfo');
    else{
      selectedCausalInfoKey='';
      selectedCausalInfoNode=null;
      showSectionState('causalInfo','数据加载中','当前 SQL 因果信息暂无节点，继续等待投影写入。');
      安排SQL区段重试('causalInfo');
    }
  }
  return true;
}
function renderSQLCausalChainSection(data){
  const relations=sqlRowsToRelations(data.relations);
  if(isUnchangedSection('causalChain',stableDataFingerprint(data.relations||[])))return false;
  resetCausalRelationIndexes(relations);
  chainRows.innerHTML='';
  chainResult.textContent=`已刷新因果链关系 ${causalRelationEdges.length} 条。`;
  return true;
}
function renderSQLWorldTreeSection(data){
  const rows=sqlRowsToTreeRows(data.rows);
  const relations=sqlRowsToRelations(data.relations);
  if(isUnchangedSection('worldTree',stableDataFingerprint([data.rows||[],data.relations||[]]))){
    if(isSectionActive('worldTree')&&!rows.length)安排SQL区段重试('worldTree');
    return false;
  }
  resetWorldTreeIndexes(rows,relations);
  const section=document.getElementById('worldTree');
  const table=section?.querySelector('table[data-filterable]');
  const headers=Array.isArray(data.headers)?data.headers:[];
  if(table){
    const headRow=table.tHead?.rows?.[0];
    if(headRow)headRow.innerHTML=headers.map(header=>`<th>${escapeHtml(header)}</th>`).join('');
    const body=table.tBodies?.[0]||table.createTBody();
    body.innerHTML=(Array.isArray(data.rows)?data.rows:[]).map(row=>`<tr>${headers.map((_,index)=>`<td>${escapeHtml(sqlCell(row,index))}</td>`).join('')}</tr>`).join('');
  }
  const note=document.querySelector('#worldTree .tree-toolbar .note');
  if(note){
    const causalCount=rows.filter(row=>row.kind==='因果').length;
    note.textContent=`SQL 当前节点数：${rows.length}；因果节点：${causalCount}`;
  }
  buildWorldTree();
  applyFilter();
  if(isSectionActive('worldTree')){
    if(rows.length)selectDefaultForSection('worldTree');
    else{
      selectedWorldTreeNode=null;
      showSectionState('worldTree','数据加载中','当前 SQL 世界树暂无节点，继续等待投影写入。');
      安排SQL区段重试('worldTree');
    }
  }
  return true;
}
function applySQLGenericSubtree(data){
  const sectionId=data.section||data.page||'';
  const nodeKey=data.nodeKey||'';
  const section=document.getElementById(sectionId);
  const node=findGenericNode(sectionId,nodeKey);
  if(!section||!node)return;
  node.__sqlSubtreeLoading=false;
  if(!data.ok){
    if(panelStatus)panelStatus.textContent=data.error?`子节点加载失败：${data.error}`:'子节点加载失败。';
    return;
  }
  const headers=node.headers||[];
  const title=section.dataset.sectionTitle||section.querySelector('h2')?.textContent||sectionId;
  const fingerprint=stableDataFingerprint(data.rows||[]);
  if(isUnchangedSubtree(node,fingerprint)){
    if(panelStatus)panelStatus.textContent=`子节点无变化：${nodeKey}`;
    return;
  }
  const childRows=(Array.isArray(data.rows)?data.rows:[]).map((row,index)=>
    createGenericRowNode(sectionId,title,headers,row,index,true));
  node.children=buildGenericHierarchy(childRows);
  node.__sqlSubtreeLoaded=true;
  renderGenericTreePanel(section);
  applyFilter();
  if(panelStatus)panelStatus.textContent=`已加载子节点：${nodeKey}，节点 ${childRows.length}`;
}
function mergeWorldRelations(relations){
  const known=new Set(worldTreeRelations.map(rel=>`${rel.owner}|${rel.relation}|${rel.targetKey}|${rel.ordinal}`));
  relations.forEach(rel=>{
    const key=`${rel.owner}|${rel.relation}|${rel.targetKey}|${rel.ordinal}`;
    if(!known.has(key)){
      known.add(key);
      worldTreeRelations.push(rel);
    }
  });
}
function applySQLWorldSubtree(data){
  const nodeKey=data.nodeKey||'';
  const parent=worldNodeByKey.get(nodeKey);
  if(parent)parent.__sqlSubtreeLoading=false;
  if(!data.ok){
    if(panelStatus)panelStatus.textContent=data.error?`子节点加载失败：${data.error}`:'子节点加载失败。';
    return;
  }
  const fingerprint=stableDataFingerprint([data.rows||[],data.relations||[]]);
  if(parent&&isUnchangedSubtree(parent,fingerprint)){
    if(panelStatus)panelStatus.textContent=`世界树子节点无变化：${nodeKey}`;
    return;
  }
  const rows=sqlRowsToTreeRows(data.rows);
  const known=new Set(worldTreeRows.map(row=>row.key));
  rows.forEach(row=>{
    if(!known.has(row.key)){
      known.add(row.key);
      worldTreeRows.push(row);
    }
  });
  if(parent){
    parent.__sqlSubtreeLoaded=true;
    parent.open=true;
  }
  mergeWorldRelations(sqlRowsToRelations(data.relations));
  resetWorldTreeIndexes([...worldTreeRows],[...worldTreeRelations]);
  buildWorldTree();
  applyFilter();
  if(panelStatus)panelStatus.textContent=`已加载世界树子节点：${nodeKey}，节点 ${rows.length}`;
}
function applySQLCausalInfoSubtree(data){
  const nodeKey=data.nodeKey||'';
  const parent=causalInfoByKey.get(nodeKey);
  if(parent)parent.__sqlSubtreeLoading=false;
  if(!data.ok){
    if(panelStatus)panelStatus.textContent=data.error?`子节点加载失败：${data.error}`:'子节点加载失败。';
    return;
  }
  const fingerprint=stableDataFingerprint([data.rows||[],data.relations||[]]);
  if(parent&&isUnchangedSubtree(parent,fingerprint)){
    if(panelStatus)panelStatus.textContent=`因果信息子节点无变化：${nodeKey}`;
    return;
  }
  const rows=sqlRowsToTreeRows(data.rows);
  const known=new Set(causalInfoRows.map(row=>row.key));
  rows.forEach(row=>{
    if(!known.has(row.key)){
      known.add(row.key);
      causalInfoRows.push(row);
    }
  });
  if(parent){
    parent.__sqlSubtreeLoaded=true;
    parent.open=true;
  }
  mergeCausalInfoRelations(sqlRowsToRelations(data.relations));
  resetCausalInfoIndexes([...causalInfoRows],[...causalInfoRelations]);
  buildCausalInfoTree();
  applyFilter();
  if(panelStatus)panelStatus.textContent=`已加载因果信息子节点：${nodeKey}，节点 ${rows.length}`;
}
function 应用SQL子链刷新(requestId,data){
  const request=sqlSubtreeRequests.get(Number(requestId));
  sqlSubtreeRequests.delete(Number(requestId));
  if(data?.kind==='sql-world-subtree'&&data?.section==='causalInfo')applySQLCausalInfoSubtree(data);
  else if(data?.kind==='sql-world-subtree')applySQLWorldSubtree(data);
  else applySQLGenericSubtree(data||{ok:false,section:request?.sectionId,nodeKey:request?.nodeKey,error:'子链响应为空'});
}
function 请求刷新当前SQL区段(target){
  if(!target)return false;
  if(window.chrome&&window.chrome.webview){
    if(panelStatus)panelStatus.textContent=`正在刷新：${target}`;
    window.chrome.webview.postMessage(`refresh-page:${target}`);
    return true;
  }
  if(panelStatus)panelStatus.textContent='静态 HTML 预览未连接实时刷新接口。';
  return false;
}
function 当前SQL区段(){
  const activeButton=buttons.find(item=>item.classList.contains('active'));
  return activeButton?.dataset.target||buttons[0]?.dataset.target||'metrics';
}
function 安排SQL区段重试(target){
  clearTimeout(sqlRefreshRetryTimer);
  sqlRefreshRetryTimer=window.setTimeout(()=>请求刷新当前SQL区段(target||当前SQL区段()),2000);
}
function 应用SQL区段刷新(data){
  if(!data||typeof data!=='object')return;
  if(!data.ok){
    if(panelStatus)panelStatus.textContent=data.error?`数据加载中：${data.error}`:'数据加载中，等待 SQL 投影写入。';
    安排SQL区段重试(data.page||当前SQL区段());
    return;
  }
  const activeResponse=isSectionActive(data.page||'');
  const rowsArray=Array.isArray(data.rows)?data.rows:[];
  const emptyActiveResult=activeResponse
    && ['sql-table','sql-causal-info','sql-world-tree'].includes(data.kind)
    && rowsArray.length===0;
  if(activeResponse)clearTimeout(sqlRefreshRetryTimer);
  let changed=false;
  if(data.kind==='sql-table')changed=renderSQLTableSection(data);
  else if(data.kind==='sql-causal-info')changed=renderSQLCausalInfoSection(data);
  else if(data.kind==='sql-causal-chain')changed=renderSQLCausalChainSection(data);
  else if(data.kind==='sql-world-tree')changed=renderSQLWorldTreeSection(data);
  else return;
  if(panelStatus)panelStatus.textContent=emptyActiveResult
    ? `数据加载中：${sectionTitle(data.page||'当前页面')} 暂无 SQL 行，继续重试。`
    : (changed ? `已刷新当前页面：${data.page||''}` : `当前页面无变化：${data.page||''}`);
}
)HTML";
        输出 << R"HTML(
function filterTreeRows(roots,query){
  if(!roots.length)return;
  function visit(row){
    const selfMatch=!query||(row.searchText||'').includes(query);
    let childMatch=false;
    (row.children||[]).forEach(child=>{childMatch=visit(child)||childMatch;});
    const visible=selfMatch||childMatch;
    if(row.el){row.el.style.display=visible?'':'none';if(query&&childMatch&&row.el.tagName==='DETAILS')row.el.open=true;}
    return visible;
  }
  roots.forEach(visit);
}
function setTreeDepth(rows,depth){
  rows.forEach(row=>{if(row.el&&row.el.tagName==='DETAILS')row.el.open=Number(row.depth||0)<depth;});
}
function worldTreeLabel(row){
  return treeLabel(row);
}
function causalInfoLabel(row){
  return treeLabel(row);
}
function requestSQLCausalInfoSubtree(row){
  if(!row||row.__sqlSubtreeLoading)return;
  if(!(window.chrome&&window.chrome.webview)){
    if(panelStatus)panelStatus.textContent='静态 HTML 预览未连接子链加载接口。';
    return;
  }
  row.__sqlSubtreeLoading=true;
  row.open=true;
  const requestId=++sqlSubtreeRequestSeq;
  sqlSubtreeRequests.set(requestId,{kind:'causalInfo',sectionId:'causalInfo',nodeKey:row.key});
  if(panelStatus)panelStatus.textContent=`正在加载因果信息子节点：${row.key}`;
  window.chrome.webview.postMessage(`sql-subtree:${requestId}:causalInfo:${encodeURIComponent(row.key||'')}`);
}
function expandCausalInfoTwoLayers(){
  if(!causalInfoRoots.length){
    if(panelStatus)panelStatus.textContent='当前因果信息没有可展开的根节点。';
    return;
  }
  causalInfoRoots.forEach(row=>{
    row.open=true;
    requestSQLCausalInfoSubtree(row);
  });
  setTreeDepth(causalInfoRoots,3);
}
function updateWorldSelection(){
  worldTreeRows.forEach(row=>{
    if(row.lineEl)row.lineEl.classList.toggle('selected',row===selectedWorldTreeNode);
  });
}
function requestSQLWorldSubtree(row){
  if(!row||row.__sqlSubtreeLoading)return;
  if(!(window.chrome&&window.chrome.webview)){
    if(panelStatus)panelStatus.textContent='静态 HTML 预览未连接子链加载接口。';
    return;
  }
  row.__sqlSubtreeLoading=true;
  row.open=true;
  const requestId=++sqlSubtreeRequestSeq;
  sqlSubtreeRequests.set(requestId,{kind:'world',sectionId:'worldTree',nodeKey:row.key});
  if(panelStatus)panelStatus.textContent=`正在加载世界树子节点：${row.key}`;
  window.chrome.webview.postMessage(`sql-subtree:${requestId}:worldTree:${encodeURIComponent(row.key||'')}`);
}
function selectWorldTreeNode(row){
  if(!row)return;
  selectedWorldTreeNode=row;
  const relations=worldRelationsByOwner.get(row.key)||[];
  const relationHtml=relations.length?`<div class="detail-block"><h3>子关系 ${relations.length}</h3>${relations.slice(0,48).map(rel=>`<div class="detail-item">${renderDetailRows([
    ['关系',rel.relation],
    ['目标',`${rel.targetKind||''}:${rel.targetKey||''}`],
    ['显示',rel.targetText],
    ['序号',rel.ordinal]
  ])}</div>`).join('')}${relations.length>48?`<div class="detail-sub-row"><b>其余</b><span>${relations.length-48} 条</span></div>`:''}</div>`:'';
  showNodeDetail(row.display||row.type||row.key,`${row.kind||'世界树节点'} | ${row.key}`,[
    ['节点主键',row.key],
    ['父节点',row.parent],
    ['深度',row.depth],
    ['类别',row.kind],
    ['显示',row.display],
    ['类型',row.type],
    ['值类',row.valueKind],
    ['值',row.value],
    ['辅助',row.aux]
  ],relationHtml);
  updateWorldSelection();
  requestSQLWorldSubtree(row);
}
function buildCausalInfoTree(){
  if(!causalInfoHost)return;
  const map=new Map();
  causalInfoRows.forEach(row=>{
    row.children=[];
    row.selectKey=row.key;
    row.onSelect=()=>selectCausalInfoNode(row,true);
    row.searchText=[row.key,row.parent,row.kind,row.display,row.type,row.valueKind,row.value,row.aux].join(' ').toLowerCase();
    if(row.key)map.set(row.key,row);
  });
  causalInfoRoots=[];
  causalInfoRows.forEach(row=>{
    const parent=map.get(row.parent);
    if(parent&&parent!==row)parent.children.push(row);else causalInfoRoots.push(row);
  });
  causalInfoHost.innerHTML='';
  const root={key:'CAUSAL_INFO_ROOT',depth:'0',kind:'因果信息',display:`因果信息 ${causalInfoRoots.length}`,children:causalInfoRoots};
  root.searchText='因果信息 '+causalInfoRoots.map(row=>row.searchText).join(' ');
  root.onSelect=()=>{
    showNodeDetail('因果信息','世界树因果根节点',[
      ['当前根节点数',String(causalInfoRoots.length)],
      ['加载口径','无因果父祖先的因果节点'],
      ['展开方式','点击因果根节点加载一层子链']
    ]);
    if(!causalInfoRoots.length&&panelStatus)panelStatus.textContent='当前因果信息没有可展开的根节点。';
  };
  causalInfoHost.appendChild(renderTreeNode(root,causalInfoLabel));
  updateCausalSelection();
}
function filterCausalInfo(query){
  if(!causalInfoHost||!causalInfoRoots.length)return;
  filterTreeRows(causalInfoRoots,query);
}
function buildWorldTree(){
  if(!worldTreeHost)return;
  const map=new Map();
  worldTreeRows.forEach(row=>{row.children=[];row.searchText=[row.key,row.parent,row.kind,row.display,row.type,row.valueKind,row.value,row.aux].join(' ').toLowerCase();row.onSelect=()=>selectWorldTreeNode(row);map.set(row.key,row);});
  worldTreeRoots=[];
  worldTreeRows.forEach(row=>{
    const parent=map.get(row.parent);
    if(parent&&parent!==row)parent.children.push(row);else worldTreeRoots.push(row);
  });
  worldTreeHost.innerHTML='';
  const fragment=document.createDocumentFragment();
  worldTreeRoots.forEach(row=>fragment.appendChild(renderTreeNode(row,worldTreeLabel)));
  worldTreeHost.appendChild(fragment);
  updateWorldSelection();
}
function filterWorldTree(query){
  if(!worldTreeHost||!worldTreeRoots.length)return;
  filterTreeRows(worldTreeRoots,query);
}
function filterGenericTrees(query){
  genericTreeRootsBySection.forEach(roots=>filterTreeRows(roots,query));
}
function setWorldTreeDepth(depth){
  if(!worldTreeHost)return;
  setTreeDepth(worldTreeRows,depth);
}
function nodeId(kind,key){return `${kind||''}|${key||''}`;}
function nodeText(kind,key){return `${kind||''}:${key||''}`;}
function nodeMatches(kind,key,query){
  if(!query)return false;
  return nodeText(kind,key).toLowerCase().includes(query)||String(key||'').toLowerCase().includes(query);
}
function renderChain(path,message){
  chainRows.innerHTML='';
  chainResult.textContent=message;
  path.forEach((edge,index)=>{
    const tr=document.createElement('tr');
    tr.innerHTML=`<td>${index+1}</td><td>${escapeHtml(nodeText(edge.sourceKind,edge.sourceKey))}</td><td>${escapeHtml(edge.relation)}</td><td>${escapeHtml(nodeText(edge.targetKind,edge.targetKey))}</td><td>${escapeHtml(edge.evidence)}</td>`;
    chainRows.appendChild(tr);
  });
}
function queryCausalChain(){
  const cause=(causeInput.value||'').trim().toLowerCase();
  const effect=(effectInput.value||'').trim().toLowerCase();
  if(!cause||!effect){renderChain([],'请同时输入因和果。');return;}
  const adjacency=new Map();
  const starts=new Set();
  const targetMatches=new Set();
  for(const edge of causalRelationEdges){
    const src=nodeId(edge.sourceKind,edge.sourceKey);
    const dst=nodeId(edge.targetKind,edge.targetKey);
    if(!adjacency.has(src))adjacency.set(src,[]);
    adjacency.get(src).push({edge,dst});
    if(nodeMatches(edge.sourceKind,edge.sourceKey,cause))starts.add(src);
    if(nodeMatches(edge.targetKind,edge.targetKey,effect))targetMatches.add(dst);
  }
  const queue=[];
  const visited=new Set();
  for(const start of starts){queue.push({node:start,path:[]});visited.add(start);}
  while(queue.length){
    const current=queue.shift();
    if(targetMatches.has(current.node)&&current.path.length>0){
      renderChain(current.path,`已生成 ${current.path.length} 条中间链接。`);
      return;
    }
    for(const next of adjacency.get(current.node)||[]){
      if(visited.has(next.dst))continue;
      const path=current.path.concat(next.edge);
      if(targetMatches.has(next.dst)){
        renderChain(path,`已生成 ${path.length} 条中间链接。`);
        return;
      }
      visited.add(next.dst);
      if(path.length<64)queue.push({node:next.dst,path});
    }
  }
  renderChain([],`没有在 ${causalRelationEdges.length} 条世界树因果关系内找到路径。`);
}
function firstSelectableNode(roots){
  let found=null;
  visitTreeRows(roots,row=>{
    if(!found&&row.onSelect)found=row;
  });
  return found;
}
function selectDefaultForSection(target){
  if(target==='causalInfo'){
    const node=firstSelectableNode(causalInfoRoots);
    if(node)selectCausalInfoNode(node);
    else{
      selectedCausalInfoKey='';
      selectedCausalInfoNode=null;
      showSectionState(target,'数据加载中','当前 SQL 因果信息暂无节点。');
    }
    return;
  }
  if(target==='worldTree'){
    const node=firstSelectableNode(worldTreeRoots);
    if(node)selectWorldTreeNode(node);
    else{
      selectedWorldTreeNode=null;
      showSectionState(target,'数据加载中','当前 SQL 世界树暂无节点。');
    }
    return;
  }
  const genericRoots=genericTreeRootsBySection.get(target);
  if(genericRoots){
    const node=firstSelectableNode(genericRoots);
    if(node)selectGenericNode(node);
    else{
      selectedGenericNode=null;
      showSectionState(target,'数据加载中','当前 SQL 区段暂无节点。');
    }
    return;
  }
  if(target==='causalChain'){
    showNodeDetail('因果链查询','输入因和果后生成世界树因果关系链接',[
      ['因',causeInput?.value||''],
      ['果',effectInput?.value||''],
      ['SQL 因果关系',String(causalRelationEdges.length)]
    ]);
  }
}
)HTML";
        输出 << R"HTML(
function activateMenuButton(button,refreshCurrent=false){
  if(!button)return;
  buttons.forEach(item=>item.classList.toggle('active',item===button));
  sections.forEach(section=>section.classList.toggle('active',section.id===button.dataset.target));
  try{localStorage.setItem('fishnest.panel.activeTarget',button.dataset.target||'');}catch(_){}
  applyFilter();
  showSectionState(button.dataset.target,'数据加载中','正在读取当前 SQL 区段。');
  selectDefaultForSection(button.dataset.target);
  if(refreshCurrent)请求刷新当前SQL区段(button.dataset.target);
}
function selectMenuByNumber(text){
  const index=Number(text);
  if(!Number.isInteger(index)||index<=0)return false;
  const button=buttons.find(item=>Number(item.dataset.menuIndex||0)===index);
  if(!button)return false;
  activateMenuButton(button,true);
  return true;
}
function openCameraWindow(){
  if(window.chrome&&window.chrome.webview){
    window.chrome.webview.postMessage('camera:open-window');
  }else if(panelStatus){
    panelStatus.textContent='静态 HTML 预览不能打开相机窗口。';
  }
}
buttons.forEach(button=>button.addEventListener('click',()=>activateMenuButton(button,true)));
filter.addEventListener('input',applyFilter);
if(openCameraButton)openCameraButton.addEventListener('click',openCameraWindow);
document.addEventListener('keydown',event=>{
  if(event.target&&['INPUT','TEXTAREA','SELECT'].includes(event.target.tagName))return;
  if(/^[0-9]$/.test(event.key)){
    menuNumberBuffer+=event.key;
    clearTimeout(menuNumberTimer);
    if(menuNumberBuffer==='1'){
      menuNumberTimer=setTimeout(()=>{selectMenuByNumber(menuNumberBuffer);menuNumberBuffer='';},650);
      return;
    }
    if(selectMenuByNumber(menuNumberBuffer)){
      menuNumberBuffer='';
      return;
    }
    menuNumberTimer=setTimeout(()=>{selectMenuByNumber(menuNumberBuffer);menuNumberBuffer='';},650);
  }
});
document.getElementById('chainQuery').addEventListener('click',queryCausalChain);
causeInput.addEventListener('keydown',event=>{if(event.key==='Enter')queryCausalChain();});
effectInput.addEventListener('keydown',event=>{if(event.key==='Enter')queryCausalChain();});
document.getElementById('causalInfoExpand').addEventListener('click',expandCausalInfoTwoLayers);
document.getElementById('causalInfoCollapse').addEventListener('click',()=>setTreeDepth(causalInfoRoots,1));
document.getElementById('worldTreeExpand').addEventListener('click',()=>setWorldTreeDepth(3));
document.getElementById('worldTreeCollapse').addEventListener('click',()=>setWorldTreeDepth(1));
document.getElementById('worldTreeShowCausal').addEventListener('click',()=>{filter.value='因果';applyFilter();});
buildTableTreeSections();
buildCausalInfoTree();
buildWorldTree();
try{
  const savedTarget=localStorage.getItem('fishnest.panel.activeTarget');
  const savedButton=savedTarget?buttons.find(item=>item.dataset.target===savedTarget):null;
  if(savedButton)activateMenuButton(savedButton);else activateMenuButton(buttons[0]);
}catch(_){activateMenuButton(buttons[0]);}
window.setTimeout(()=>{if(window.chrome&&window.chrome.webview)请求刷新当前SQL区段(当前SQL区段());},300);
window.__panelApplyCameraWindowState=function(data){
  if(panelStatus&&data&&typeof data==='object'){
    panelStatus.textContent=data.message||(data.ok?'相机窗口已打开。':'相机窗口打开失败。');
  }
};
window.__panelApplyPageRefresh=应用SQL区段刷新;
window.__panelApplySQLSubtree=应用SQL子链刷新;
window.__panelApplyExpand=function(){};
window.__panelApplyDetail=function(){};
</script>
</body>
</html>
)HTML";
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加树节点JSON(std::ostringstream& 输出, const 结构_控制面板树节点& 节点)
    {
        输出 << "{";
        输出 << "\"ptr\":" << 节点.节点指针 << ",";
        输出 << "\"arg\":" << 节点.附加参数 << ",";
        输出 << "\"text\":";
        追加JSON字符串(输出, 节点.文本);
        输出 << ",";
        输出 << "\"open\":" << (节点.默认展开 ? "true" : "false") << ",";
        输出 << "\"lazy\":" << (节点.可延迟展开 ? "true" : "false") << ",";
        输出 << "\"expandType\":";
        追加JSON字符串(输出, 节点.展开类型);
        输出 << ",";
        if (!节点.详情.empty()) {
            输出 << "\"__detailLoaded\":true,";
            输出 << "\"details\":[";
            for (std::size_t 索引 = 0; 索引 < 节点.详情.size(); ++索引) {
                if (索引 > 0) {
                    输出 << ",";
                }
                私有_追加树节点JSON(输出, 节点.详情[索引]);
            }
            输出 << "],";
        }
        输出 << "\"children\":[";
        for (std::size_t 索引 = 0; 索引 < 节点.子项.size(); ++索引) {
            if (索引 > 0) {
                输出 << ",";
            }
            私有_追加树节点JSON(输出, 节点.子项[索引]);
        }
        输出 << "]";
        输出 << "}";
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_树节点JSON(const 结构_控制面板树节点& 节点)
    {
        std::ostringstream 输出;
        私有_追加树节点JSON(输出, 节点);
        return 输出.str();
    }

    std::string 私有_页面刷新JSON(
        std::string_view 页面,
        const 结构_控制面板树节点* 根节点,
        std::string_view 错误 = {})
    {
        std::ostringstream 输出;
        输出 << "{\"ok\":" << (根节点 && 错误.empty() ? "true" : "false");
        输出 << ",\"page\":";
        追加JSON字符串(输出, 页面);
        if (根节点) {
            输出 << ",\"root\":";
            私有_追加树节点JSON(输出, *根节点);
        }
        if (!错误.empty()) {
            输出 << ",\"error\":";
            追加JSON字符串(输出, 错误);
        }
        输出 << "}";
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加自我场景诊断区域数组JSON(
        std::ostringstream& 输出,
        const std::vector<结构_控制面板诊断区域>& 区域列表)
    {
        输出 << "[";
        for (std::size_t 索引 = 0; 索引 < 区域列表.size(); ++索引) {
            if (索引 > 0) {
                输出 << ",";
            }
            const auto& 区域 = 区域列表[索引];
            输出 << "{";
            输出 << "\"id\":" << 区域.区域编号;
            输出 << ",\"layer\":" << 区域.图层类型;
            输出 << ",\"source\":" << 区域.来源候选编号;
            输出 << ",\"rect\":["
                << 区域.投影最小X << ","
                << 区域.投影最小Y << ","
                << 区域.投影最大X << ","
                << 区域.投影最大Y << "]";
            输出 << ",\"pixels\":" << 区域.像素数量;
            输出 << ",\"confidence\":" << 区域.置信度;
            输出 << ",\"mask\":" << 区域.掩码状态;
            输出 << "}";
        }
        输出 << "]";
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加自我场景存在复现项数组JSON(
        std::ostringstream& 输出,
        const std::vector<结构_控制面板自我场景存在复现项>& 存在列表)
    {
        输出 << "[";
        for (std::size_t 索引 = 0; 索引 < 存在列表.size(); ++索引) {
            if (索引 > 0) {
                输出 << ",";
            }
            const auto& 项 = 存在列表[索引];
            const bool 可绘制 = (项.几何状态 & 1) != 0;
            输出 << "{";
            输出 << "\"ptr\":" << 项.节点指针;
            输出 << ",\"title\":";
            追加JSON字符串(输出, 项.标题);
            输出 << ",\"type\":";
            追加JSON字符串(输出, 项.类型);
            输出 << ",\"sourceScope\":" << 项.来源范围;
            输出 << ",\"sourceCandidate\":" << 项.来源空间候选编号;
            输出 << ",\"reportId\":" << 项.外设观察报告ID;
            输出 << ",\"clusterId\":" << 项.外设观察像素簇ID;
            输出 << ",\"geometryState\":" << 项.几何状态;
            输出 << ",\"renderable\":" << (可绘制 ? "true" : "false");
            输出 << ",\"center\":";
            追加JSON_I64数组3(输出, 项.中心X, 项.中心Y, 项.中心Z);
            输出 << ",\"min\":";
            追加JSON_I64数组3(输出, 项.AABB最小X, 项.AABB最小Y, 项.AABB最小Z);
            输出 << ",\"max\":";
            追加JSON_I64数组3(输出, 项.AABB最大X, 项.AABB最大Y, 项.AABB最大Z);
            输出 << ",\"size\":";
            追加JSON_I64数组3(输出, 项.尺寸X, 项.尺寸Y, 项.尺寸Z);
            输出 << ",\"projection\":["
                << 项.投影最小X << ","
                << 项.投影最小Y << ","
                << 项.投影最大X << ","
                << 项.投影最大Y << "]";
            输出 << ",\"confirmState\":" << 项.观察存在确认状态;
            输出 << ",\"ownershipState\":" << 项.像素归属验证状态;
            输出 << ",\"drawableState\":" << 项.可绘制状态;
            输出 << ",\"rgbStructure\":" << 项.颜色RGB结构状态;
            输出 << ",\"pixelColorLayer\":" << 项.像素颜色层状态;
            输出 << ",\"colorBuffer\":" << 项.颜色缓冲状态;
            输出 << ",\"colorContourCount\":" << 项.彩图轮廓数量;
            输出 << ",\"contourColorSupport\":" << 项.轮廓颜色支持率;
            输出 << ",\"textureReady\":" << 项.局部轮廓材料可回查状态;
            输出 << ",\"flatContourState\":" << 项.平面轮廓状态;
            输出 << ",\"spaceContourState\":" << 项.空间极值轮廓状态;
            输出 << ",\"flatContourPoints\":" << 项.平面轮廓点数;
            输出 << ",\"spaceContourPoints\":" << 项.空间极值轮廓点数;
            输出 << "}";
        }
        输出 << "]";
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_自我场景复现JSON(const 结构_控制面板快照& 快照)
    {
        const bool 候选范围有效 =
            快照.自我场景空间候选数量 > 0
            && 快照.自我场景范围坐标AABB最大X >= 快照.自我场景范围坐标AABB最小X
            && 快照.自我场景范围坐标AABB最大Y >= 快照.自我场景范围坐标AABB最小Y
            && 快照.自我场景范围坐标AABB最大Z >= 快照.自我场景范围坐标AABB最小Z;
        const I64 绘制候选编号 = 快照.自我场景观察存在假设 != 0
            ? 快照.自我场景来源空间候选编号
            : 快照.自我场景主空间候选编号;
        const I64 绘制候选像素数量 = 快照.自我场景观察存在假设 != 0
            ? std::max<I64>(
                0,
                快照.自我场景假设有效点比例 * 快照.自我场景点云有效像素数量 / 10000)
            : 快照.自我场景主空间候选像素数量;
        I64 窗口像素数量 = 快照.自我场景预期像素数量;
        if (窗口像素数量 <= 0
            && 快照.自我场景相机帧宽度 > 0
            && 快照.自我场景相机帧高度 > 0) {
            窗口像素数量 =
                快照.自我场景相机帧宽度 * 快照.自我场景相机帧高度;
        }
        const I64 未解释像素数 = std::max<I64>(0, 快照.自我场景未解释像素数);
        const I64 未知区域窗口占比 = 窗口像素数量 > 0
            ? std::clamp<I64>(未解释像素数 * 10000 / 窗口像素数量, 0, 10000)
            : std::clamp<I64>(快照.自我场景未解释区域比例, 0, 10000);

        std::ostringstream 输出;
        输出 << "{";
        输出 << "\"ok\":" << (快照.自我场景复现有快照 ? "true" : "false") << ",";
        输出 << "\"scenePtr\":" << 快照.自我所在场景指针 << ",";
        输出 << "\"hostPtr\":" << 快照.自我场景复现宿主指针 << ",";
        输出 << "\"sceneTitle\":";
        追加JSON字符串(输出, 快照.自我所在场景标题);
        输出 << ",\"hostTitle\":";
        追加JSON字符串(输出, 快照.自我场景复现宿主标题);
        输出 << ",\"safetyHostPtr\":" << 快照.自我场景安全评估宿主指针;
        输出 << ",\"safetyHostTitle\":";
        追加JSON字符串(输出, 快照.自我场景安全评估宿主标题);
        输出 << ",\"sceneDirectChildren\":" << 快照.自我场景直接子节点数量;
        输出 << ",\"sceneSubtreeNodes\":" << 快照.自我场景子树节点数量;
        输出 << ",\"sceneDirectExistences\":" << 快照.自我场景直接存在数量;
        输出 << ",\"sceneSubtreeExistences\":" << 快照.自我场景子树存在数量;
        输出 << ",\"sceneSubtreeScenes\":" << 快照.自我场景子树场景数量;
        输出 << ",\"sceneSubtreeFeatures\":" << 快照.自我场景子树特征数量;
        输出 << ",\"sceneSubtreeStates\":" << 快照.自我场景子树状态数量;
        输出 << ",\"sceneSubtreeDynamics\":" << 快照.自我场景子树动态数量;
        输出 << ",\"sceneSubtreeRelations\":" << 快照.自我场景子树二次特征数量;
        输出 << ",\"hostSubtreeNodes\":" << 快照.自我场景复现宿主子树节点数量;
        输出 << ",\"hostSubtreeExistences\":" << 快照.自我场景复现宿主子树存在数量;
        输出 << ",\"sceneSubtreeScanLimit\":" << 快照.自我场景子树扫描上限;
        输出 << ",\"sceneSubtreeScanLimitHit\":" << 快照.自我场景子树扫描达到上限;
        输出 << ",\"hostSubtreeScanLimitHit\":" << 快照.自我场景复现宿主子树扫描达到上限;
        输出 << ",\"sceneDirectSummary\":";
        追加JSON字符串(输出, 快照.自我场景直接子层摘要);
        输出 << ",\"sceneSubtreeSummary\":";
        追加JSON字符串(输出, 快照.自我场景子树类型摘要);
        输出 << ",\"sceneExistenceSamples\":";
        追加JSON字符串(输出, 快照.自我场景存在样例摘要);
        输出 << ",\"hostExistenceSamples\":";
        追加JSON字符串(输出, 快照.自我场景复现宿主存在样例摘要);
        输出 << ",\"width\":" << 快照.自我场景相机帧宽度;
        输出 << ",\"height\":" << 快照.自我场景相机帧高度;
        输出 << ",\"depthFrame\":" << 快照.自我场景深度帧号;
        输出 << ",\"colorFrame\":" << 快照.自我场景彩色帧号;
        输出 << ",\"currentFrame\":" << 快照.自我场景当前观察帧;
        输出 << ",\"expectedPixels\":" << 快照.自我场景预期像素数量;
        输出 << ",\"windowPixels\":" << 窗口像素数量;
        输出 << ",\"pixelFeatures\":" << 快照.自我场景像素特征数量;
        输出 << ",\"depthValidPixels\":" << 快照.自我场景深度有效像素数量;
        输出 << ",\"pointCloudPixels\":" << 快照.自我场景点云有效像素数量;
        输出 << ",\"pixelCoverage\":" << 快照.自我场景像素覆盖状态;
        输出 << ",\"unitMm\":" << 快照.自我场景空间坐标单位毫米;
        输出 << ",\"rgbStructure\":" << 快照.自我场景颜色RGB结构状态;
        输出 << ",\"rawDepthStructure\":" << 快照.自我场景原始深度毫米结构状态;
        输出 << ",\"filteredDepthStructure\":" << 快照.自我场景滤波深度毫米结构状态;
        输出 << ",\"filledDepthStructure\":" << 快照.自我场景补全深度毫米结构状态;
        输出 << ",\"depthMaskStructure\":" << 快照.自我场景深度有效性Mask状态;
        输出 << ",\"depthSourceStructure\":" << 快照.自我场景深度来源结构状态;
        输出 << ",\"depthStabilityStructure\":" << 快照.自我场景深度稳定性结构状态;
        输出 << ",\"depthNeighborhoodStructure\":" << 快照.自我场景深度邻域一致性结构状态;
        输出 << ",\"fusedDepthStructure\":" << 快照.自我场景融合深度毫米结构状态;
        输出 << ",\"fusedDepthValidityStructure\":" << 快照.自我场景融合深度有效性结构状态;
        输出 << ",\"multiFrameStabilityStructure\":" << 快照.自我场景多帧深度稳定性结构状态;
        输出 << ",\"perPixelDepthVarianceStructure\":" << 快照.自我场景每像素深度方差结构状态;
        输出 << ",\"xyzStructure\":" << 快照.自我场景空间坐标毫米XYZ结构状态;
        输出 << ",\"rawDepthSourcePixels\":" << 快照.自我场景原始深度来源像素数量;
        输出 << ",\"filteredDepthSourcePixels\":" << 快照.自我场景滤波深度来源像素数量;
        输出 << ",\"filledDepthSourcePixels\":" << 快照.自我场景补全深度来源像素数量;
        输出 << ",\"noDepthSourcePixels\":" << 快照.自我场景无有效深度来源像素数量;
        输出 << ",\"filledDepthLowConfidencePixels\":" << 快照.自我场景补全深度低置信像素数量;
        输出 << ",\"depthStabilityAverage\":" << 快照.自我场景深度稳定性平均值;
        输出 << ",\"depthNeighborhoodAverage\":" << 快照.自我场景深度邻域一致性平均值;
        输出 << ",\"observationFrameGroupState\":" << 快照.自我场景观察帧组状态;
        输出 << ",\"observationFrameGroupCount\":" << 快照.自我场景观察帧组帧数;
        输出 << ",\"fusedBasisFrameSetState\":" << 快照.自我场景融合依据帧集合状态;
        输出 << ",\"fusedDepthValidPixels\":" << 快照.自我场景融合深度有效像素数量;
        输出 << ",\"fusedDepthValidRatio\":" << 快照.自我场景融合深度有效率;
        输出 << ",\"fusedDepthHoleCompensationPixels\":" << 快照.自我场景融合补偿深度空洞像素数量;
        输出 << ",\"fusedDepthLowStabilityPixels\":" << 快照.自我场景融合低稳定像素数量;
        输出 << ",\"fusedDepthStabilityAverage\":" << 快照.自我场景融合深度稳定性平均值;
        输出 << ",\"fusedDepthAverageVariance\":" << 快照.自我场景融合深度平均方差;
        输出 << ",\"colorDepthAligned\":" << 快照.自我场景彩色深度已对齐;
        输出 << ",\"depthValidRatio\":" << 快照.自我场景深度有效率;
        输出 << ",\"spaceValidRatio\":" << 快照.自我场景空间坐标有效率;
        输出 << ",\"frameQualityScore\":" << 快照.自我场景帧质量评分;
        输出 << ",\"depthHolePixels\":" << 快照.自我场景深度空洞数量;
        输出 << ",\"depthHoleRegions\":" << 快照.自我场景深度空洞区域数量;
        输出 << ",\"contourClosureRatio\":" << 快照.自我场景轮廓闭合率;
        输出 << ",\"contourBreakCount\":" << 快照.自我场景轮廓断裂数量;
        输出 << ",\"colorContourCount\":" << 快照.自我场景彩图轮廓数量;
        输出 << ",\"depthContourCount\":" << 快照.自我场景深度轮廓数量;
        输出 << ",\"spaceProjectionContourCount\":" << 快照.自我场景空间投影轮廓数量;
        输出 << ",\"fusedContourCount\":" << 快照.自我场景融合轮廓数量;
        输出 << ",\"contourLayerState\":" << 快照.自我场景轮廓来源分层状态;
        输出 << ",\"contourDepthSupport\":" << 快照.自我场景轮廓深度支持率;
        输出 << ",\"contourColorSupport\":" << 快照.自我场景轮廓颜色支持率;
        输出 << ",\"contourSpaceSupport\":" << 快照.自我场景轮廓空间支持率;
        输出 << ",\"fusedContourConfidence\":" << 快照.自我场景融合轮廓置信度;
        输出 << ",\"depthBreakContourCount\":" << 快照.自我场景跨越深度断裂轮廓数量;
        输出 << ",\"invalidDepthContourCount\":" << 快照.自我场景穿过深度无效区域轮廓数量;
        输出 << ",\"filledDepthContourCount\":" << 快照.自我场景依赖补全深度轮廓数量;
        输出 << ",\"boundaryDepthStability\":" << 快照.自我场景边界深度稳定率;
        输出 << ",\"candidateVerifyRatio\":" << 快照.自我场景候选存在验证通过率;
        输出 << ",\"unexplainedRatio\":" << 快照.自我场景未解释区域比例;
        输出 << ",\"observationGapState\":" << 快照.自我场景补观察缺口状态;
        输出 << ",\"observationMissingReason\":" << 快照.自我场景补观察缺失原因;
        输出 << ",\"refillObservationRegionCount\":" << 快照.自我场景待补观察区域数量;
        输出 << ",\"completionCandidateCount\":" << 快照.自我场景补全候选数量;
        输出 << ",\"lowConfidenceContourCount\":" << 快照.自我场景低置信轮廓数量;
        输出 << ",\"insufficientCandidateCount\":" << 快照.自我场景条件不足候选数量;
        输出 << ",\"pendingVerifyCandidateCount\":" << 快照.自我场景待验证候选数量;
        输出 << ",\"partialConfirmCandidateCount\":" << 快照.自我场景部分确认候选数量;
        输出 << ",\"observationSuggestion\":" << 快照.自我场景补观察需求建议;
        输出 << ",\"diagnosticRegionCount\":" << 快照.自我场景诊断区域数量;
        输出 << ",\"diagnosticRegionSetState\":" << 快照.自我场景诊断区域集合状态;
        输出 << ",\"diagnosticRegionMaskState\":" << 快照.自我场景诊断区域掩码状态;
        输出 << ",\"diagnosticRegions\":";
        私有_追加自我场景诊断区域数组JSON(输出, 快照.自我场景诊断区域列表);
        输出 << ",\"realExistenceCount\":" << 快照.自我场景真实复现存在数量;
        输出 << ",\"realGeometryExistenceCount\":" << 快照.自我场景真实几何存在数量;
        输出 << ",\"realRenderableExistenceCount\":" << 快照.自我场景真实可绘制存在数量;
        输出 << ",\"realColorStateExistenceCount\":" << 快照.自我场景真实颜色状态存在数量;
        输出 << ",\"realTextureExistenceCount\":" << 快照.自我场景真实彩图材料可回查存在数量;
        输出 << ",\"existences\":";
        私有_追加自我场景存在复现项数组JSON(输出, 快照.自我场景存在复现项列表);
        输出 << ",\"candidateCount\":" << 快照.自我场景空间候选数量;
        输出 << ",\"candidateValidPixels\":" << 快照.自我场景空间候选有效点数量;
        输出 << ",\"hypothesisState\":" << 快照.自我场景观察存在假设;
        输出 << ",\"hypothesisSource\":" << 快照.自我场景来源空间候选编号;
        输出 << ",\"hypothesisVerifyState\":" << 快照.自我场景存在假设验证状态;
        输出 << ",\"hypothesisDistance\":" << 快照.自我场景假设距离;
        输出 << ",\"hypothesisSize\":";
        追加JSON_I64数组3(
            输出,
            快照.自我场景假设尺寸X,
            快照.自我场景假设尺寸Y,
            快照.自我场景假设尺寸Z);
        输出 << ",\"hypothesisValidRatio\":" << 快照.自我场景假设有效点比例;
        输出 << ",\"pixelOwnershipState\":" << 快照.自我场景像素归属验证状态;
        输出 << ",\"fullFrameOwnershipState\":" << 快照.自我场景全帧像素归属账状态;
        输出 << ",\"assignedPixels\":" << 快照.自我场景已归属像素数;
        输出 << ",\"unexplainedPixels\":" << 快照.自我场景未解释像素数;
        输出 << ",\"ownershipConflictPixels\":" << 快照.自我场景归属冲突像素数;
        输出 << ",\"ownershipRatio\":" << 快照.自我场景像素归属率;
        输出 << ",\"depthConsistency\":" << 快照.自我场景深度一致率;
        输出 << ",\"insideRangeRatio\":" << 快照.自我场景范围内点比例;
        输出 << ",\"projectionCoverage\":" << 快照.自我场景投影覆盖率;
        输出 << ",\"contourMatch\":" << 快照.自我场景轮廓吻合率;
        输出 << ",\"verifiedFlag\":" << 快照.自我场景已验证观察存在;
        输出 << ",\"confirmState\":" << 快照.自我场景观察存在确认状态;
        输出 << ",\"verifiedCount\":" << 快照.自我场景已验证观察存在数量;
        输出 << ",\"knownExistenceCount\":"
            << std::max<I64>(
                快照.自我场景直接存在数量,
                std::max<I64>(快照.自我场景子树存在数量, 快照.自我场景已验证观察存在数量));
        输出 << ",\"unknownWindowPixels\":" << 未解释像素数;
        输出 << ",\"unknownWindowPixelTotal\":" << 窗口像素数量;
        输出 << ",\"unknownWindowPixelRatio\":" << 未知区域窗口占比;
        输出 << ",\"frameExplainState\":" << 快照.自我场景帧解释状态;
        输出 << ",\"basicObservationFactUsableState\":" << 快照.自我场景基础观察事实可用状态;
        输出 << ",\"basicRiskJudgementState\":" << 快照.自我场景基础风险判断状态;
        输出 << ",\"riskStateExplicitState\":" << 快照.自我场景风险状态明确状态;
        输出 << ",\"basicRiskValueCalculatedState\":" << 快照.自我场景基础风险值计算状态;
        输出 << ",\"sceneAssessmentSafetyValue\":" << 快照.自我场景当前场景评估安全值候选;
        输出 << ",\"sceneSafetyDegree\":" << 快照.自我场景当前场景安全度候选;
        输出 << ",\"sceneSafetyJudgementState\":" << 快照.自我场景当前场景安全判定状态;
        输出 << ",\"safetyEvidenceInsufficientReason\":" << 快照.自我场景安全评估证据不足原因;
        输出 << ",\"riskSafetySceneImpactCandidate\":" << 快照.自我场景风险安全_场景影响部分候选;
        输出 << ",\"riskSafetySceneImpactSettleableState\":" << 快照.自我场景风险安全_场景影响部分可结算状态;
        输出 << ",\"riskSafetySceneImpactUnsettleableReason\":" << 快照.自我场景风险安全_场景影响部分不可结算原因;
        输出 << ",\"riskSafetySceneImpactBookedState\":" << 快照.自我场景风险安全_场景影响部分已入账状态;
        输出 << ",\"riskSafetyLayerCandidate\":" << 快照.自我场景风险安全层候选;
        输出 << ",\"riskSafetyLayerProjectionCandidate\":" << 快照.自我场景风险安全层投影候选;
        输出 << ",\"riskSafetyLayerMissingFactorCount\":" << 快照.自我场景风险安全层缺失因素数量;
        输出 << ",\"riskSafetyLayerClearState\":" << 快照.自我场景风险安全层明确状态;
        输出 << ",\"riskSafetyLayerAggregatableState\":" << 快照.自我场景风险安全层可聚合状态;
        输出 << ",\"riskSafetyLayerBookedState\":" << 快照.自我场景风险安全层已入账状态;
        输出 << ",\"riskFactorUnsearchedMask\":" << 快照.自我场景风险因素未搜索掩码;
        输出 << ",\"riskFactorSearchGapMask\":" << 快照.自我场景风险因素搜索缺口掩码;
        输出 << ",\"riskFactorNegativeEvidenceMask\":" << 快照.自我场景风险因素负向证据掩码;
        输出 << ",\"riskFactorDefaultSatisfiedMask\":" << 快照.自我场景风险因素默认满足掩码;
        输出 << ",\"riskFactorBookedMask\":" << 快照.自我场景风险因素已入账掩码;
        输出 << ",\"riskFactorSearchIncompleteCount\":" << 快照.自我场景风险因素证据搜索未完成数量;
        输出 << ",\"riskFactorDefaultSatisfiedCount\":" << 快照.自我场景风险因素默认满足数量;
        输出 << ",\"candidate\":{";
        输出 << "\"valid\":" << (候选范围有效 ? "true" : "false") << ",";
        输出 << "\"id\":" << 绘制候选编号 << ",";
        输出 << "\"pixels\":" << 绘制候选像素数量 << ",";
        输出 << "\"center\":";
        追加JSON_I64数组3(
            输出,
            快照.自我场景中心空间坐标X,
            快照.自我场景中心空间坐标Y,
            快照.自我场景中心空间坐标Z);
        输出 << ",\"min\":";
        追加JSON_I64数组3(
            输出,
            快照.自我场景范围坐标AABB最小X,
            快照.自我场景范围坐标AABB最小Y,
            快照.自我场景范围坐标AABB最小Z);
        输出 << ",\"max\":";
        追加JSON_I64数组3(
            输出,
            快照.自我场景范围坐标AABB最大X,
            快照.自我场景范围坐标AABB最大Y,
            快照.自我场景范围坐标AABB最大Z);
        输出 << ",\"continuity\":" << 快照.自我场景空间连续性评分;
        输出 << ",\"stability\":" << 快照.自我场景范围稳定性评分;
        输出 << "}}";
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_树节点子项JSON(const 结构_控制面板树节点& 节点)
    {
        std::ostringstream 输出;
        输出 << "[";
        for (std::size_t 索引 = 0; 索引 < 节点.子项.size(); ++索引) {
            if (索引 > 0) {
                输出 << ",";
            }
            私有_追加树节点JSON(输出, 节点.子项[索引]);
        }
        输出 << "]";
        return 输出.str();
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_树节点列表JSON(const std::vector<结构_控制面板树节点>& 节点列表)
    {
        std::ostringstream 输出;
        输出 << "[";
        for (std::size_t 索引 = 0; 索引 < 节点列表.size(); ++索引) {
            if (索引 > 0) {
                输出 << ",";
            }
            私有_追加树节点JSON(输出, 节点列表[索引]);
        }
        输出 << "]";
        return 输出.str();
    }

    std::string 私有_失效节点JSON(const std::string& 文本 = "节点已失效或已移出当前树")
    {
        return 私有_树节点列表JSON({ 私有_新节点(文本) });
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_移除节点字段子项(结构_控制面板树节点& 节点)
    {
        节点.子项.erase(
            std::remove_if(
                节点.子项.begin(),
                节点.子项.end(),
                [](const 结构_控制面板树节点& 子项) {
                    return 子项.是字段分组;
                }),
            节点.子项.end());
    }

    // 功能：服务所在模块的内部辅助流程。
    std::vector<结构_控制面板树节点> 私有_提取节点字段详情(const 结构_控制面板树节点& 节点)
    {
        std::vector<结构_控制面板树节点> 结果{};
        for (const auto& 子项 : 节点.子项) {
            if (子项.是字段分组) {
                结果.push_back(子项);
            }
        }
        return 结果;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_页面摘要(const std::string& 文本)
    {
        return 文本.empty() ? "暂无数据" : 文本;
    }

    std::string 私有_截断文本(const std::string& 文本, const std::size_t 上限 = 160)
    {
        if (文本.size() <= 上限) {
            return 文本;
        }
        return 文本.substr(0, 上限) + "...";
    }

    // 功能：创建并返回或登记对应对象。
    结构_构建上下文 私有_创建构建上下文(const std::size_t 树广度上限)
    {
        结构_构建上下文 上下文{};
        上下文.树广度上限 = (std::min)((std::max<std::size_t>)(1, 树广度上限), std::size_t{16});
        上下文.自我存在指针 = 私有_地址(自我.获取自我存在());
        上下文.自我现实场景指针 = 私有_地址(自我.获取自我现实场景());
        上下文.自我内部世界指针 = 私有_地址(自我.获取自我内部世界());
        return 上下文;
    }

    // 功能：从指定来源读取数据或状态。
    void 私有_读取需求列表分页(
        需求节点* 根节点,
        const std::size_t 起始偏移,
        const std::size_t 每页数量,
        std::vector<需求节点*>& 节点集,
        bool& 还有更多,
        std::size_t& 已遍历数量)
    {
        if (!根节点 || 每页数量 == 0) {
            return;
        }

        std::vector<需求节点*> 栈{};
        栈.push_back(根节点);
        while (!栈.empty()) {
            auto* 当前 = 栈.back();
            栈.pop_back();

            if (当前 != 根节点) {
                if (已遍历数量 >= 起始偏移 && 节点集.size() < 每页数量) {
                    节点集.push_back(当前);
                }
                ++已遍历数量;
            }

            const auto 子节点集 = 私有_枚举全部子节点(当前);
            for (auto it = 子节点集.rbegin(); it != 子节点集.rend(); ++it) {
                栈.push_back(*it);
            }

            if (节点集.size() >= 每页数量 && !栈.empty()) {
                还有更多 = true;
                break;
            }
        }

        if (!还有更多) {
            还有更多 = !栈.empty();
        }
    }

    // 功能：创建并返回或登记对应对象。
    结构_控制面板树节点 私有_创建需求列表加载更多节点(const std::size_t 下一个偏移)
    {
        return 私有_新节点(
            "继续加载需求列表 | 下一段起点=" + std::to_string(下一个偏移)
                + " | 每次=" + std::to_string(私有_列表分页大小) + " 项",
            下一个偏移,
            false,
            true,
            "need-list-more");
    }
}

// 功能：从指定来源读取数据或状态。
结构_控制面板快照 私有_读取控制面板快照(
    const std::size_t 树深度上限,
    const std::size_t 树广度上限,
    const bool 读取自我场景快照)
{
    结构_控制面板快照 快照{};
    const bool 仅标量摘要 = 树深度上限 == 0 && 树广度上限 == 0;
    const auto 记录快照阶段 = [](std::string_view 阶段) noexcept {
        项目运行日志(std::string("控制面板快照阶段 | ") + std::string(阶段));
    };

    记录快照阶段("开始");
    快照.世界树已初始化 = 世界树.世界根() != nullptr;
    快照.自我已初始化 = 自我.已初始化();
    auto* 原始自我存在 = 自我.获取自我存在();
    快照.自我存在已建立 = 原始自我存在 != nullptr;
    if (const auto* 自我主信息 = 世界树.取存在主信息(原始自我存在)) {
        快照.自我需求根字段已建立 = 自我主信息->需求根节点 != nullptr;
        快照.自我任务根字段已建立 = 自我主信息->任务根节点 != nullptr;
        快照.自我方法根字段已建立 = 自我主信息->方法根节点 != nullptr;
    }
    快照.自我内部世界已建立 = 自我.获取自我内部世界() != nullptr;
    快照.自我待机状态 = 自我.是否待机状态();
    快照.自我安全值 = 自我.获取安全值();
    快照.自我服务值 = 自我.获取服务值();
    快照.自我风险安全值 = 自我.获取风险安全值();

    auto& 自我线程 = 获取全局自我线程();
    (void)启动控制面板摘要线程();
    (void)请求刷新控制面板摘要();
    const auto 摘要线程生命周期 = 获取全局控制面板摘要线程().读取生命周期状态();
    auto 摘要线程快照 = 读取最新控制面板摘要快照();
    if (摘要线程快照.快照序号 == 0) {
        const auto 等待截止 =
            std::chrono::steady_clock::now() + std::chrono::milliseconds(100);
        do {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            摘要线程快照 = 读取最新控制面板摘要快照();
        } while (
            摘要线程快照.快照序号 == 0
            && std::chrono::steady_clock::now() < 等待截止);
    }
    const auto 自我线程最小快照 = 读取自我线程最小状态快照();
    const auto 生命周期 =
        static_cast<枚举_线程生命周期状态>(自我线程最小快照.生命周期值);
    const auto 当前阶段 =
        static_cast<枚举_自我线程运行阶段>(自我线程最小快照.最近阶段值);
    const auto 当前去向 =
        static_cast<枚举_自我线程最终去向>(自我线程最小快照.最近去向值);
    const auto 自检报告修复快照 = 自我线程.读取最近自检报告修复治理快照();

    快照.自我线程已初始化 = true;
    快照.自我线程运行中 =
        生命周期 == 枚举_线程生命周期状态::启动中
        || 生命周期 == 枚举_线程生命周期状态::运行中
        || 生命周期 == 枚举_线程生命周期状态::停止请求中
        || 生命周期 == 枚举_线程生命周期状态::收尾中;
    快照.自我线程健康运行 = 生命周期 != 枚举_线程生命周期状态::故障;
    快照.自我线程生命周期 = static_cast<std::uint8_t>(生命周期);
    快照.自我Tick计数 =
        自我线程最小快照.Tick计数值 > 0
            ? static_cast<std::uint64_t>(自我线程最小快照.Tick计数值)
            : 0;
    快照.自我线程累计故障次数 = 自我线程.读取累计故障次数();
    快照.自我线程累计恢复次数 = 自我线程.读取累计恢复次数();
    快照.自我线程当前阶段 = 私有_线程运行阶段文本(当前阶段);
    快照.自我线程当前最终去向 = 私有_线程最终去向文本(当前去向);

    快照.控制面板摘要线程已启动 =
        摘要线程生命周期 != 枚举_控制面板摘要线程生命周期状态::未启动
        && 摘要线程生命周期 != 枚举_控制面板摘要线程生命周期状态::已停止;
    快照.控制面板摘要线程健康运行 =
        获取全局控制面板摘要线程().是否健康运行();
    快照.控制面板摘要线程生命周期 =
        static_cast<std::uint8_t>(摘要线程生命周期);
    快照.控制面板摘要快照序号 = 摘要线程快照.快照序号;
    快照.控制面板摘要_运行证据方法数 = static_cast<std::uint64_t>(摘要线程快照.运行事实有运行证据方法数);
    快照.控制面板摘要_失败任务数 = static_cast<std::uint64_t>(摘要线程快照.运行事实失败任务数);
    快照.控制面板摘要_等待任务数 = static_cast<std::uint64_t>(摘要线程快照.运行事实等待任务数);
    快照.控制面板摘要_最近方法主键 = 摘要线程快照.运行事实最近方法主键;
    快照.控制面板摘要_最近方法时间 = static_cast<std::uint64_t>(摘要线程快照.运行事实最近方法时间);
    快照.控制面板摘要_最近任务时间 = static_cast<std::uint64_t>(摘要线程快照.运行事实最近任务时间);
    快照.控制面板自检生命周期值 = 摘要线程快照.自检线程生命周期值;
    快照.控制面板自检最近模式值 = 摘要线程快照.自检线程最近模式值;
    快照.控制面板自检健康状态值 = 摘要线程快照.自检线程健康状态值;
    快照.控制面板自检健康运行 = 摘要线程快照.自检线程健康运行;
    快照.控制面板自检Tick计数 = 摘要线程快照.自检线程Tick计数;
    快照.控制面板自检累计发现问题数 = 摘要线程快照.自检线程累计发现问题数;
    快照.控制面板自检累计提交需求数 = 摘要线程快照.自检线程累计提交需求数;
    快照.控制面板自检累计提交报告数 = 摘要线程快照.自检线程累计提交报告数;
    快照.控制面板自检累计需求化失败数 = 摘要线程快照.自检线程累计需求化失败数;
    快照.控制面板自检最近检查时间 = static_cast<std::uint64_t>(摘要线程快照.自检线程最近检查时间);
    快照.控制面板自检最近提交时间 = static_cast<std::uint64_t>(摘要线程快照.自检线程最近提交时间);
    快照.自检报告待处理数 = 自检报告修复快照.待处理报告数;
    快照.自检报告待休眠修复数 = 自检报告修复快照.待休眠修复报告数;
    快照.自检报告待映射确认数 = 自检报告修复快照.待映射确认报告数;
    快照.自检报告仅报告保留数 = 自检报告修复快照.仅报告保留数;
    快照.自检报告休眠期评估数 = 自检报告修复快照.休眠期评估报告数;
    快照.自检报告越界需求化拒绝数 = 自检报告修复快照.累计越界需求化拒绝数;
    快照.自检报告最近休眠期门控数 = 自检报告修复快照.最近休眠期门控报告数;
    快照.自检报告最近处置 = 自检报告修复快照.最近报告处置;
    快照.自检报告修复门控摘要 = 自检报告修复快照.摘要;
    快照.自我存在指针 = 私有_地址(自我.获取自我存在());
    if (仅标量摘要) {
        记录快照阶段("轻量摘要跳过场景复现");
        记录快照阶段("轻量摘要跳过基础节点全量计数");
    }
    else {
        if (读取自我场景快照) {
            私有_读取自我场景复现快照(快照);
        }
        else {
            记录快照阶段("主控制面板跳过场景复现");
        }

        快照.基础信息节点数 = 世界树.统计全部节点();
        快照.场景数 = 世界树.统计节点_按主信息类型<场景节点主信息类>();
        快照.存在数 = 世界树.统计节点_按主信息类型<存在节点主信息类>();
        快照.特征数 = 世界树.统计节点_按主信息类型<特征节点主信息类>();
        快照.状态数 = 世界树.统计节点_按主信息类型<状态节点主信息类>();
        快照.动态数 = 世界树.统计节点_按主信息类型<动态节点主信息类>();
        记录快照阶段("基础节点计数完成");

        const auto 因果节点集 = 世界树.枚举节点_按主信息类型<因果主信息类>();
        std::size_t 因果模板数 = 0;
        std::size_t 因果证据动态样本数 = 0;
        for (auto* 因果节点 : 因果节点集) {
            const auto* 主信息 = 世界树.取因果主信息(reinterpret_cast<const 因果节点类*>(因果节点));
            if (!主信息) {
                continue;
            }
            ++因果模板数;
            因果证据动态样本数 += 主信息->证据动态样本.size();
        }
        快照.因果模板数 = 因果模板数;
        快照.因果证据动态样本数 = 因果证据动态样本数;
    }

    auto* 自我存在 = 自我.获取自我存在();
    auto* 需求根节点 = 自我存在 ? 世界树.获取需求根节点(自我存在) : nullptr;
    auto* 任务根节点 = 自我存在 ? 世界树.获取任务根节点(自我存在) : nullptr;
    auto* 方法根节点 = 自我存在 ? 世界树.获取方法根节点(自我存在) : nullptr;
    记录快照阶段("自我根节点读取完成");

    快照.需求数 = 需求根节点 ? (私有_计数子树节点(需求根节点) - 1) : 0;
    快照.方法数 = 方法根节点 ? (私有_计数子树节点(方法根节点) - 1) : 0;
    记录快照阶段("需求方法数量统计完成");
    if (方法根节点) {
        auto 统计方法结构 = [&](const 方法节点* 节点) {
            if (!节点 || 节点 == 方法根节点 || !方法类::方法是首节点(节点)) {
                return;
            }
            ++快照.方法首节点数;
            std::size_t 条件数 = 0;
            std::size_t 直接结果数 = 0;
            std::size_t 条件下结果数 = 0;
            std::size_t 条件无结果数 = 0;
            私有_统计方法直接条件结果_控制面板(
                节点,
                条件数,
                直接结果数,
                条件下结果数,
                条件无结果数);
            if (条件无结果数 > 0) {
                ++快照.方法条件无结果数;
            }
            if (直接结果数 > 0) {
                ++快照.方法结果无条件数;
            }
            if (条件无结果数 > 0 || 直接结果数 > 0) {
                ++快照.方法条件结果不成对数;
            }
        };
        私有_遍历子树节点(reinterpret_cast<方法节点*>(方法根节点), 统计方法结构);
    }
    if (需求根节点) {
        std::lock_guard<std::recursive_mutex> 需求树锁{
            需求类::借用需求树全局互斥()
        };
        std::unordered_map<std::string, std::size_t> 活动目标计数{};
        std::unordered_map<std::string, std::vector<std::string>> 活动目标样本{};
        路径集合 已统计需求地址{};
        std::size_t 重复需求地址数 = 0;
        std::vector<std::string> 重复需求地址样本{};
        auto 统计需求满足 = [&](const 需求节点* 节点) {
            if (!节点 || 节点 == 需求根节点) {
                return;
            }
            const auto 节点地址 = 私有_地址(节点);
            if (!已统计需求地址.insert(节点地址).second) {
                ++重复需求地址数;
                if (重复需求地址样本.size() < 5) {
                    std::ostringstream 样本;
                    样本 << 私有_节点主键_控制面板(节点)
                        << "@"
                        << 私有_十六进制指针(节点地址);
                    重复需求地址样本.push_back(样本.str());
                }
            }
            if (节点->主信息.需求有效截止 != 0) {
                ++快照.需求已满足数;
            }
            else if (私有_需求状态已达(节点)) {
                ++快照.需求状态已达未截止数;
            }
            else {
                ++快照.需求未满足数;
            }

            if (节点->子) {
                ++快照.需求树有子节点数;
            }
            else {
                ++快照.需求树叶子节点数;
            }

            if (节点->主信息.需求有效截止 != 0) {
                ++快照.需求树已截止需求数;
            }
            else if (节点->主信息.是否阻塞父任务执行) {
                ++快照.需求树活动阻塞需求数;
            }

            if (节点->主信息.需求有效截止 == 0
                && !节点->子) {
                ++快照.需求树活动叶子需求数;
                if (!节点->主信息.对应任务.有效()) {
                    ++快照.需求树活动叶子需求未任务化数;
                    if (快照.需求树首个未任务化叶子需求主键.empty()) {
                        快照.需求树首个未任务化叶子需求主键 =
                            私有_节点主键_控制面板(节点);
                        快照.需求树首个未任务化叶子需求父主键 =
                            私有_节点主键_控制面板(reinterpret_cast<需求节点*>(节点->父));
                        快照.需求树首个未任务化叶子需求目标主体主键 =
                            私有_需求目标宿主主键_控制面板(节点);
                        const auto* 目标特征类型 = 节点->主信息.目标特征类型缓存
                            ? 节点->主信息.目标特征类型缓存
                            : 私有_需求目标特征类型_控制面板(节点);
                        快照.需求树首个未任务化叶子需求目标特征主键 =
                            私有_节点主键_控制面板(目标特征类型);
                    }
                }
            }

            auto* 目标状态 = 私有_解析基础信息引用_控制面板(
                节点->主信息.需求状态);
            const auto* 目标状态主信息 = 目标状态
                ? 世界树.取状态主信息(目标状态)
                : nullptr;
            const auto 目标状态主键 = 私有_引用主键_控制面板(
                节点->主信息.需求状态);
            const auto 显式目标宿主主键 = 私有_引用主键_控制面板(
                节点->主信息.被需求存在);
            const auto 目标状态宿主主键 = 私有_状态主体主键_控制面板(
                目标状态);
            const auto 目标宿主主键 = 私有_需求目标宿主主键_控制面板(节点);
            const auto* 目标特征类型 = 节点->主信息.目标特征类型缓存
                ? 节点->主信息.目标特征类型缓存
                : 私有_需求目标特征类型_控制面板(节点);
            const auto 目标特征类型主键 =
                私有_节点主键_控制面板(目标特征类型);
            const auto 目标语义视图 = 私有_需求目标语义视图_控制面板(目标特征类型);
            if (目标语义视图.是逻辑组织目标) {
                ++快照.需求树逻辑组织需求数;
                switch (目标语义视图.语义) {
                case 枚举_需求目标语义_控制面板::AND组满足:
                    ++快照.需求树AND组需求数;
                    break;
                case 枚举_需求目标语义_控制面板::OR组满足:
                    ++快照.需求树OR组需求数;
                    break;
                case 枚举_需求目标语义_控制面板::方法路径完成:
                    ++快照.需求树方法路径组需求数;
                    break;
                case 枚举_需求目标语义_控制面板::因果子链支撑:
                    ++快照.需求树因果子链支撑需求数;
                    break;
                default:
                    break;
                }
            }
            if (目标语义视图.是结算令牌目标) {
                ++快照.需求树OR组结算令牌需求数;
            }

            auto* 目标特征节点 = 目标状态主信息
                ? 目标状态主信息->状态特征.指针
                : nullptr;
            if (!目标特征节点
                && 目标状态主信息
                && !目标状态主信息->状态特征.主键.empty()) {
                目标特征节点 = 世界树.按主键解析特征节点(
                    目标状态主信息->状态特征.主键);
            }

            bool 目标绑定异常 = false;
            auto 标记目标绑定异常 = [&]() {
                目标绑定异常 = true;
            };

            if (!目标状态主信息) {
                ++快照.需求树缺目标状态需求数;
                标记目标绑定异常();
            }
            if (目标宿主主键.empty()) {
                ++快照.需求树缺目标宿主需求数;
                标记目标绑定异常();
            }
            if (目标特征类型主键.empty()) {
                ++快照.需求树缺目标特征类型需求数;
                标记目标绑定异常();
            }
            if (!显式目标宿主主键.empty()
                && !目标状态宿主主键.empty()
                && 显式目标宿主主键 != 目标状态宿主主键
                && 私有_基础主键是存在_控制面板(目标状态宿主主键)) {
                ++快照.需求树目标状态宿主不一致需求数;
                标记目标绑定异常();
            }
            if (!目标宿主主键.empty() && !目标特征类型主键.empty()) {
                std::string 目标特征归属主键{};
                if (目标特征节点 && 目标特征节点->父) {
                    目标特征归属主键 =
                        reinterpret_cast<const 基础信息节点类*>(
                            目标特征节点->父)->获取主键();
                }
                if (目标特征归属主键.empty()
                    || 目标特征归属主键 != 目标宿主主键) {
                    ++快照.需求树目标特征未归属宿主需求数;
                    标记目标绑定异常();
                }
            }

            if (目标绑定异常) {
                ++快照.需求树目标绑定异常需求数;
                if (快照.需求树首个目标绑定异常需求主键.empty()) {
                    快照.需求树首个目标绑定异常需求主键 =
                        私有_节点主键_控制面板(节点);
                    快照.需求树首个目标绑定异常父主键 =
                        私有_节点主键_控制面板(reinterpret_cast<需求节点*>(节点->父));
                    快照.需求树首个目标绑定异常目标状态主键 = 目标状态主键;
                    快照.需求树首个目标绑定异常目标宿主主键 = 目标宿主主键;
                    快照.需求树首个目标绑定异常目标特征主键 = 目标特征类型主键;
                }
            }
            else {
                ++快照.需求树目标绑定完整需求数;
            }

            std::string 已完成任务主键{};
            if (私有_需求存在完成任务但仍活动_控制面板(节点, &已完成任务主键)) {
                ++快照.需求树完成任务仍活动需求数;
                if (快照.需求树首个完成任务仍活动需求主键.empty()) {
                    快照.需求树首个完成任务仍活动需求主键 =
                        私有_节点主键_控制面板(节点);
                    快照.需求树首个完成任务仍活动任务主键 =
                        std::move(已完成任务主键);
                }
            }

            if (!节点->主信息.是否阻塞父任务执行) {
                ++快照.需求树非阻塞需求数;
            }

            if (节点->主信息.需求有效截止 == 0) {
                const auto 父主键 =
                    私有_节点主键_控制面板(reinterpret_cast<需求节点*>(节点->父));
                const auto 目标宿主主键 = 私有_需求目标宿主主键_控制面板(节点);
                const auto* 目标特征类型 = 节点->主信息.目标特征类型缓存
                    ? 节点->主信息.目标特征类型缓存
                    : 私有_需求目标特征类型_控制面板(节点);
                const auto 目标特征主键 = 私有_节点主键_控制面板(目标特征类型);
                if (!父主键.empty()
                    && !目标宿主主键.empty()
                    && !目标特征主键.empty()) {
                    const auto 方向 = 私有_需求方向_控制面板(节点);
                    const auto 键 = 父主键
                        + "|宿主=" + 目标宿主主键
                        + "|特征=" + 目标特征主键
                        + "|方向=" + std::to_string(方向);
                    ++活动目标计数[键];
                    auto& 样本需求主键 = 活动目标样本[键];
                    if (样本需求主键.size() < 5) {
                        样本需求主键.push_back(
                            私有_节点主键_控制面板(节点));
                    }
                }
            }
        };
        私有_遍历子树节点(需求根节点, 统计需求满足);
        记录快照阶段("需求满足统计完成");
        auto 检查父子权重 = [&](const 需求节点* 节点) {
            if (!节点 || 节点 == 需求根节点) {
                return;
            }
            私有_检查需求父子权重_控制面板(节点, 快照);
        };
        私有_遍历子树节点(需求根节点, 检查父子权重);
        if (重复需求地址数 > 0) {
            std::ostringstream 日志;
            日志 << "控制面板/需求树重复节点诊断"
                << " | 重复地址数=" << 重复需求地址数
                << " | 样本=";
            for (std::size_t 索引 = 0; 索引 < 重复需求地址样本.size(); ++索引) {
                if (索引 > 0) {
                    日志 << ",";
                }
                日志 << 重复需求地址样本[索引];
            }
            项目运行日志(日志.str());
        }

        std::size_t 已输出重复目标样本数 = 0;
        for (const auto& 项 : 活动目标计数) {
            if (项.second > 1) {
                ++快照.需求树重复目标组数;
                快照.需求树重复目标需求数 += 项.second;
                if (已输出重复目标样本数 < 8) {
                    const auto 样本迭代 = 活动目标样本.find(项.first);
                    const auto* 样本需求主键 = 样本迭代 != 活动目标样本.end()
                        ? &样本迭代->second
                        : nullptr;
                    std::ostringstream 日志;
                    日志 << "控制面板/需求树重复目标诊断"
                        << " | 键=" << 项.first
                        << " | 数量=" << 项.second
                        << " | 样本需求=";
                    const auto 样本数量 = 样本需求主键 ? 样本需求主键->size() : 0;
                    for (std::size_t 索引 = 0; 索引 < 样本数量; ++索引) {
                        if (索引 > 0) {
                            日志 << ",";
                        }
                        日志 << (*样本需求主键)[索引];
                    }
                    项目运行日志(日志.str());
                    ++已输出重复目标样本数;
                }
            }
        }
        if (快照.需求树重复目标组数 > 已输出重复目标样本数) {
            项目运行日志(
                "控制面板/需求树重复目标诊断 | 省略组数="
                + std::to_string(
                    快照.需求树重复目标组数 - 已输出重复目标样本数));
        }
        快照.需求树存在重复目标风险 = 快照.需求树重复目标组数 > 0;
        快照.需求树父子权重异常 = 快照.需求树父子权重异常需求数 > 0;
        记录快照阶段("需求树诊断统计完成");
    }
    if (auto* 当前主需求节点 = 私有_解析当前树节点_可写(自我.获取当前主需求())) {
        快照.需求树当前主需求主键 =
            私有_节点主键_控制面板(当前主需求节点);
        快照.需求树当前主需求父主键 =
            私有_节点主键_控制面板(reinterpret_cast<需求节点*>(当前主需求节点->父));
        快照.需求树当前主需求目标主体主键 =
            私有_引用主键_控制面板(当前主需求节点->主信息.被需求存在);
        const auto* 目标特征类型 = 当前主需求节点->主信息.目标特征类型缓存
            ? 当前主需求节点->主信息.目标特征类型缓存
            : 私有_需求目标特征类型_控制面板(当前主需求节点);
        快照.需求树当前主需求目标特征主键 =
            私有_节点主键_控制面板(目标特征类型);
        快照.需求树当前主需求有任务 =
            当前主需求节点->主信息.对应任务.有效();
        const auto 目标语义视图 = 私有_需求目标语义视图_控制面板(目标特征类型);
        const auto* 令牌记录 = 私有_查找OR组令牌记录_控制面板(当前主需求节点);
        const auto 令牌状态文本 = 私有_OR组令牌状态文本_控制面板(
            当前主需求节点,
            目标语义视图,
            令牌记录);
        const auto 令牌激活路径文本 = 私有_OR组令牌当前激活路径文本_控制面板(
            当前主需求节点,
            目标语义视图,
            令牌记录);
        快照.需求树当前主需求结构形态 =
            私有_需求结构形态文本_控制面板(当前主需求节点);
        快照.需求树当前主需求可普通任务化 =
            目标语义视图.可进入普通候选方法筹办;
        快照.需求树当前主需求OR令牌状态 = 令牌状态文本;
        快照.需求树当前主需求当前激活路径 = 令牌激活路径文本;
    }
    if (任务根节点) {
        auto 统计任务节点 = [&](const 任务节点* 节点) {
            if (!节点) {
                return;
            }
            if (!私有_任务节点属于需求任务链(节点)) {
                return;
            }
            ++快照.任务数;
            switch (任务类::读取任务节点种类(节点)) {
            case 枚举_任务节点种类::头结点:
                ++快照.任务头节点数;
                break;
            case 枚举_任务节点种类::步骤节点:
                ++快照.任务步骤节点数;
                break;
            case 枚举_任务节点种类::未定义:
            default:
                ++快照.任务其他节点数;
                break;
            }
            if (节点 != 任务根节点) {
                私有_累计任务状态(快照, 任务类::读取任务状态(节点));
                if (任务类::读取任务节点种类(节点) == 枚举_任务节点种类::头结点
                    && (任务类::读取任务状态(节点) == 枚举_任务状态::完成
                        || 任务类::读取任务状态(节点) == 枚举_任务状态::已结算)
                    && !私有_解析需求引用_控制面板(节点->主信息.对应需求)) {
                    ++快照.任务完成但缺对应需求数;
                    if (快照.首个完成但缺对应需求任务主键.empty()) {
                        快照.首个完成但缺对应需求任务主键 =
                            私有_任务节点主键_控制面板(节点);
                    }
                }
            }
        };
        私有_遍历子树节点(任务根节点, 统计任务节点);
        记录快照阶段("任务树统计完成");
    }

    快照.自我待处理方法数量 = 私有_读取自我待处理方法数量_控制面板(自我);
    const auto 缺口恢复接口快照 = 自我线程.读取最近缺口恢复接口快照();
    (void)自我.读取自我I64特征当前值(
        自我特征定义类::类型_自我_可用方法数量(),
        快照.自我可用方法数量);
    快照.缺口需求总数 = 0;
    快照.缺口需求活跃数 = 0;
    快照.缺口需求可调度数 = 0;
    快照.缺口恢复请求数 = 缺口恢复接口快照.恢复请求列表.size();
    快照.缺口因兜底切换进入就绪数 = 0;
    快照.缺口因兜底切换进入采样数 = 0;
    快照.应触发缺口承接 = 缺口恢复接口快照.应触发缺口承接;
    快照.缺口应申请重试恢复 = 缺口恢复接口快照.应申请重试恢复;
    快照.缺口应申请收束恢复 = 缺口恢复接口快照.应申请收束恢复;
    快照.缺口最近反馈摘要 = 私有_缺口恢复反馈摘要_控制面板(缺口恢复接口快照);

    任务管理工作线程::结构_工作线程实例快照 工作线程快照{};
    (void)任务管理工作线程::读取任务管理工作线程快照(&工作线程快照);
    任务管理线程协议::结构_任务界面线程快照 界面线程快照{};
    (void)任务管理界面线程::读取任务管理界面线程快照(&界面线程快照);

    快照.任务管理界面线程已启动 = 界面线程快照.已启动;
    快照.任务管理界面线程正在处理 = 界面线程快照.正在处理;
    快照.任务管理界面线程故障 = 界面线程快照.故障;
    快照.任务管理界面线程累计接收请求数 = 界面线程快照.累计接收请求数;
    快照.任务管理界面线程累计绑定任务虚拟存在数 = 界面线程快照.累计绑定任务虚拟存在数;
    快照.任务管理界面线程累计派发工作项数 = 界面线程快照.累计派发工作项数;
    快照.任务管理界面线程累计收到工作结果数 = 界面线程快照.累计收到工作结果数;
    快照.任务管理界面线程累计上行消息数 = 界面线程快照.累计上行消息数;
    快照.任务管理界面线程累计调度事件数 = 界面线程快照.累计调度事件数;
    快照.任务管理界面线程累计消费工作项数 = 界面线程快照.累计消费工作项数;
    快照.任务管理界面线程累计巡检任务数 = 界面线程快照.累计巡检任务数;
    快照.任务管理界面线程累计巡检入队任务数 = 界面线程快照.累计巡检入队任务数;
    快照.任务管理界面线程累计后台worker取项数 = 界面线程快照.累计后台worker取项数;
    快照.任务管理界面线程累计后台worker派发数 = 界面线程快照.累计后台worker派发数;
    快照.任务管理界面线程当前请求队列长度 = 界面线程快照.当前请求队列长度;
    快照.任务管理界面线程当前等待工作项数 = 界面线程快照.当前等待工作项数;
    快照.任务管理界面线程当前待筹办工作项数 = 界面线程快照.当前待筹办工作项数;
    快照.任务管理界面线程当前待执行工作项数 = 界面线程快照.当前待执行工作项数;
    快照.任务管理界面线程当前完成工作项数 = 界面线程快照.当前完成工作项数;
    快照.任务管理界面线程当前死信工作项数 = 界面线程快照.当前死信工作项数;
    快照.任务管理界面线程当前在途工作项数 = 界面线程快照.当前在途工作项数;
    快照.任务管理界面线程当前任务状态快照数 = 界面线程快照.当前任务状态快照数;
    快照.任务管理界面线程当前等待任务索引数 = 界面线程快照.当前等待任务索引数;
    快照.任务管理界面线程当前挂起任务索引数 = 界面线程快照.当前挂起任务索引数;
    快照.任务管理界面线程当前上行队列长度 = 界面线程快照.当前上行队列长度;
    快照.任务管理界面线程后台worker池运行 = 界面线程快照.后台worker池运行;
    快照.任务管理界面线程后台worker线程数 = 界面线程快照.后台worker线程数;
    快照.任务管理工作线程池默认大小 = 界面线程快照.任务管理工作线程池默认大小;
    快照.任务管理工作线程池最小大小 = 界面线程快照.任务管理工作线程池最小大小;
    快照.任务管理工作线程池最大大小 = 界面线程快照.任务管理工作线程池最大大小;
    快照.任务管理工作线程池当前有效大小 =
        界面线程快照.任务管理工作线程池当前有效大小;
    快照.任务管理工作线程池待保存大小 =
        界面线程快照.任务管理工作线程池待保存大小;
    快照.任务管理工作线程池参数版本 =
        界面线程快照.任务管理工作线程池参数版本;
    快照.任务管理界面线程最近请求ID = 界面线程快照.最近请求ID;
    快照.任务管理界面线程最近工作项ID = 界面线程快照.最近工作项ID;
    快照.任务管理界面线程最近调度事件ID = 界面线程快照.最近调度事件ID;
    快照.任务管理界面线程最近后台worker序号 = 界面线程快照.最近后台worker序号;
    快照.任务管理界面线程最近后台worker工作项ID = 界面线程快照.最近后台worker工作项ID;
    快照.任务管理界面线程最近工作项类型 = 界面线程快照.最近工作项类型;
    快照.任务管理界面线程最近调度事件类型 = 界面线程快照.最近调度事件类型;
    快照.任务管理界面线程最近任务主键 = 界面线程快照.最近任务主键;
    快照.任务管理界面线程最近任务状态版本 = 界面线程快照.最近任务状态版本;
    快照.任务管理界面线程最近任务状态 = 界面线程快照.最近任务状态;
    快照.任务管理界面线程最近任务虚拟存在主键 = 界面线程快照.最近任务虚拟存在主键;
    快照.任务管理界面线程最近需求主键 = 界面线程快照.最近需求主键;
    快照.任务管理界面线程最近调度动作 = 界面线程快照.最近调度动作;
    快照.任务管理界面线程最近后台worker任务主键 = 界面线程快照.最近后台worker任务主键;
    快照.任务管理界面线程最近后台worker状态 = 界面线程快照.最近后台worker状态;
    快照.任务管理工作线程池最近参数保存状态值 =
        static_cast<std::uint8_t>(界面线程快照.最近参数保存状态);
    快照.任务管理工作线程池最近参数错误状态值 =
        static_cast<std::uint8_t>(界面线程快照.最近参数错误状态);
    快照.任务管理工作线程池最近参数应用状态值 =
        static_cast<std::uint8_t>(界面线程快照.最近参数应用状态);
    快照.任务管理界面线程待筹办队列头部任务主键 =
        界面线程快照.待筹办队列头部任务主键;
    快照.任务管理界面线程待执行队列头部任务主键 =
        界面线程快照.待执行队列头部任务主键;
    快照.任务管理界面线程待筹办队列头部优先级 =
        界面线程快照.待筹办队列头部优先级;
    快照.任务管理界面线程待执行队列头部优先级 =
        界面线程快照.待执行队列头部优先级;
    快照.任务管理界面线程待筹办队列头部排序兜底键 =
        界面线程快照.待筹办队列头部排序兜底键;
    快照.任务管理界面线程待执行队列头部排序兜底键 =
        界面线程快照.待执行队列头部排序兜底键;
    快照.任务管理界面线程最近巡检任务数 =
        界面线程快照.最近巡检任务数;
    快照.任务管理界面线程最近巡检入队任务数 =
        界面线程快照.最近巡检入队任务数;
    快照.任务管理界面线程最近巡检耗时微秒 =
        界面线程快照.最近巡检耗时微秒;
    快照.任务管理界面线程最近巡检可推进任务数 =
        界面线程快照.最近巡检可推进任务数;
    快照.任务管理界面线程最近巡检等待任务数 =
        界面线程快照.最近巡检等待任务数;
    快照.任务管理界面线程最近巡检挂起任务数 =
        界面线程快照.最近巡检挂起任务数;
    快照.任务管理界面线程最近巡检终结任务数 =
        界面线程快照.最近巡检终结任务数;
    快照.任务管理界面线程最近巡检跳过已有工作项数 =
        界面线程快照.最近巡检跳过已有工作项数;
    快照.任务管理界面线程最近巡检跳过缺结构数 =
        界面线程快照.最近巡检跳过缺结构数;
    快照.任务管理界面线程最近巡检跳过不可推进数 =
        界面线程快照.最近巡检跳过不可推进数;
    快照.任务管理界面线程最近巡检跳过入队预算数 =
        界面线程快照.最近巡检跳过入队预算数;
    快照.任务管理工作线程已启动 = 工作线程快照.已启动;
    快照.任务管理工作线程正在执行 = 工作线程快照.正在执行;
    快照.任务管理工作线程已收到请求 = 工作线程快照.已收到请求;
    快照.任务管理工作线程累计推进次数 = 工作线程快照.累计推进次数;
    快照.任务管理工作线程当前排队数 = 工作线程快照.当前排队数;
    快照.任务管理工作线程最近缺口归类 = 工作线程快照.最近缺口归类;
    快照.任务管理工作线程最近特征变化数 = 工作线程快照.最近特征变化数;

    std::optional<结构_任务管理结果> 最近治理结果{};
    std::optional<结构_治理恢复快照> 最近恢复快照{};

    if (仅标量摘要) {
        记录快照阶段("轻量摘要跳过树骨架构建");
        return 快照;
    }

    结构_构建上下文 上下文{};
    上下文.树广度上限 = (std::min)((std::max<std::size_t>)(1, 树广度上限), std::size_t{16});
    上下文.自我存在指针 = 私有_地址(自我存在);
    上下文.自我现实场景指针 = 私有_地址(自我.获取自我现实场景());
    上下文.自我内部世界指针 = 私有_地址(自我.获取自我内部世界());

    快照.线程状态树根 = 私有_构建线程状态树(快照, 界面线程快照, 工作线程快照);
    记录快照阶段("线程状态树构建完成");

    auto 世界树根 = 私有_新节点(
        std::string("世界树 | 节点仓库=基础信息类")
            + " | 基础节点=" + std::to_string(快照.基础信息节点数)
            + " | 场景=" + std::to_string(快照.场景数)
            + " | 存在=" + std::to_string(快照.存在数),
        0,
        true);
    if (auto* 世界根节点 = 世界树.世界根()) {
        世界树根.子项.push_back(
            私有_构建基础信息根链骨架(世界根节点, 上下文));
    }
    else {
        世界树根.子项.push_back(私有_新节点("基础信息仓库根为空"));
    }
    快照.世界树根 = std::move(世界树根);
    记录快照阶段("世界树骨架构建完成");

    快照.因果信息根 = 私有_构建因果信息树(
        上下文,
        快照.因果模板数,
        快照.因果证据动态样本数);
    记录快照阶段("因果信息树构建完成");

    auto 需求树根 = 私有_新节点(
        "需求树 | 需求数=" + std::to_string(快照.需求数)
            + " | " + 私有_需求满足数量摘要(快照),
        0,
        true);
    if (需求根节点) {
        需求树根.子项.push_back(
            私有_构建需求根链骨架(需求根节点, 上下文));
    }
    else {
        需求树根.子项.push_back(私有_新节点("需求根为空"));
    }
    快照.需求树根 = std::move(需求树根);
    记录快照阶段("需求树骨架构建完成");

    auto 需求列表树根 = 私有_新节点(
        "需求列表 | 需求数=" + std::to_string(快照.需求数),
        0,
        true);
    if (需求根节点) {
        记录快照阶段("需求列表分页读取开始");
        std::vector<需求节点*> 分页节点集{};
        bool 分页还有更多 = false;
        std::size_t 分页已遍历数量 = 0;
        私有_读取需求列表分页(
            需求根节点,
            0,
            私有_列表分页大小,
            分页节点集,
            分页还有更多,
            分页已遍历数量);
        记录快照阶段("需求列表分页读取完成");
        for (auto* 节点 : 分页节点集) {
            需求列表树根.子项.push_back(私有_需求列表详情节点(节点, 上下文));
        }
        if (分页还有更多) {
            需求列表树根.子项.push_back(私有_创建需求列表加载更多节点(分页已遍历数量));
        }
    }
    if (需求列表树根.子项.empty()) {
        需求列表树根.子项.push_back(私有_新节点("暂无需求"));
    }
    快照.需求列表树根 = std::move(需求列表树根);
    记录快照阶段("需求列表骨架构建完成");

    auto 任务树根 = 私有_新节点(
        "任务树 | 任务节点=" + std::to_string(快照.任务数)
            + " | 头=" + std::to_string(快照.任务头节点数)
            + " | 步骤=" + std::to_string(快照.任务步骤节点数)
            + " | 状态={" + 私有_任务状态数量摘要(快照) + "}",
        0,
        true);
    if (任务根节点) {
        任务树根.子项.push_back(
            私有_构建任务根链骨架(任务根节点, 上下文));
    }
    else {
        任务树根.子项.push_back(私有_新节点("任务根为空"));
    }
    快照.任务树根 = std::move(任务树根);
    记录快照阶段("任务树骨架构建完成");

    auto 方法树根 = 私有_新节点(
        "方法树 | 方法数=" + std::to_string(快照.方法数)
            + " | " + 私有_方法结构诊断摘要(快照),
        0,
        true);
    if (方法根节点) {
        方法树根.子项.push_back(
            私有_构建方法根链骨架(方法根节点, 上下文));
    }
    else {
        方法树根.子项.push_back(私有_新节点("方法根为空"));
    }
    快照.方法树根 = std::move(方法树根);
    记录快照阶段("方法树骨架构建完成");

    return 快照;
}

// 功能：从指定来源读取数据或状态。
结构_控制面板快照 读取控制面板快照(
    const std::size_t 树深度上限,
    const std::size_t 树广度上限)
{
    return 私有_读取控制面板快照(
        树深度上限,
        树广度上限,
        true);
}

// 功能：从指定来源读取数据或状态。
结构_控制面板快照 读取控制面板轻量摘要快照()
{
    return 私有_读取控制面板快照(0, 0, false);
}

// 功能：从指定来源读取数据或状态。
std::string 读取控制面板页面刷新JSON(std::string_view 页面)
{
    if (私有_是SQL控制面板区段(页面)) {
        return 私有_SQL控制面板区段刷新JSON(页面);
    }

    const auto 快照 = 读取控制面板快照(0, 0);
    auto* 自我存在 = 自我.获取自我存在();
    auto* 需求根节点 = 自我存在 ? 世界树.获取需求根节点(自我存在) : nullptr;
    auto* 任务根节点 = 自我存在 ? 世界树.获取任务根节点(自我存在) : nullptr;
    auto* 方法根节点 = 自我存在 ? 世界树.获取方法根节点(自我存在) : nullptr;
    const auto 上下文 = 私有_创建构建上下文(16);

    if (页面 == "thread-status") {
        任务管理工作线程::结构_工作线程实例快照 工作线程快照{};
        (void)任务管理工作线程::读取任务管理工作线程快照(&工作线程快照);
        任务管理线程协议::结构_任务界面线程快照 界面线程快照{};
        (void)任务管理界面线程::读取任务管理界面线程快照(&界面线程快照);
        auto 根 = 私有_构建线程状态树(快照, 界面线程快照, 工作线程快照);
        return 私有_页面刷新JSON(页面, &根);
    }

    if (页面 == "world-tree") {
        auto 局部快照 = 快照;
        局部快照.基础信息节点数 = 世界树.统计全部节点();
        局部快照.场景数 = 世界树.统计节点_按主信息类型<场景节点主信息类>();
        局部快照.存在数 = 世界树.统计节点_按主信息类型<存在节点主信息类>();
        auto 根 = 私有_新节点(
            std::string("世界树 | 节点仓库=基础信息类")
                + " | 基础节点=" + std::to_string(局部快照.基础信息节点数)
                + " | 场景=" + std::to_string(局部快照.场景数)
                + " | 存在=" + std::to_string(局部快照.存在数),
            0,
            true);
        if (auto* 世界根节点 = 世界树.世界根()) {
            根.子项.push_back(私有_构建基础信息根链骨架(世界根节点, 上下文));
        }
        else {
            根.子项.push_back(私有_新节点("基础信息仓库根为空"));
        }
        return 私有_页面刷新JSON(页面, &根);
    }

    if (页面 == "causal-info") {
        std::size_t 因果模板数 = 0;
        std::size_t 因果证据动态样本数 = 0;
        for (auto* 因果节点 : 世界树.枚举节点_按主信息类型<因果主信息类>()) {
            const auto* 主信息 = 世界树.取因果主信息(reinterpret_cast<const 因果节点类*>(因果节点));
            if (!主信息) {
                continue;
            }
            ++因果模板数;
            因果证据动态样本数 += 主信息->证据动态样本.size();
        }
        auto 根 = 私有_构建因果信息树(
            上下文,
            因果模板数,
            因果证据动态样本数);
        return 私有_页面刷新JSON(页面, &根);
    }

    if (页面 == "need-tree") {
        auto 根 = 私有_新节点(
            "需求树 | 需求数=" + std::to_string(快照.需求数)
                + " | " + 私有_需求满足数量摘要(快照),
            0,
            true);
        if (需求根节点) {
            根.子项.push_back(私有_构建需求根链骨架(需求根节点, 上下文));
        }
        else {
            根.子项.push_back(私有_新节点("需求根为空"));
        }
        return 私有_页面刷新JSON(页面, &根);
    }

    if (页面 == "need-list") {
        auto 根 = 私有_新节点(
            "需求列表 | 需求数=" + std::to_string(快照.需求数),
            0,
            true);
        if (需求根节点) {
            std::vector<需求节点*> 分页节点集{};
            bool 分页还有更多 = false;
            std::size_t 分页已遍历数量 = 0;
            私有_读取需求列表分页(
                需求根节点,
                0,
                私有_列表分页大小,
                分页节点集,
                分页还有更多,
                分页已遍历数量);
            for (auto* 节点 : 分页节点集) {
                根.子项.push_back(私有_需求列表详情节点(节点, 上下文));
            }
            if (分页还有更多) {
                根.子项.push_back(私有_创建需求列表加载更多节点(分页已遍历数量));
            }
        }
        if (根.子项.empty()) {
            根.子项.push_back(私有_新节点("暂无需求"));
        }
        return 私有_页面刷新JSON(页面, &根);
    }

    if (页面 == "task-tree") {
        auto 根 = 私有_新节点(
            "任务树 | 任务节点=" + std::to_string(快照.任务数)
                + " | 头=" + std::to_string(快照.任务头节点数)
                + " | 步骤=" + std::to_string(快照.任务步骤节点数)
                + " | 状态={" + 私有_任务状态数量摘要(快照) + "}",
            0,
            true);
        if (任务根节点) {
            根.子项.push_back(私有_构建任务根链骨架(任务根节点, 上下文));
        }
        else {
            根.子项.push_back(私有_新节点("任务根为空"));
        }
        return 私有_页面刷新JSON(页面, &根);
    }

    if (页面 == "method-tree") {
        auto 根 = 私有_新节点(
            "方法树 | 方法数=" + std::to_string(快照.方法数)
                + " | " + 私有_方法结构诊断摘要(快照),
            0,
            true);
        if (方法根节点) {
            根.子项.push_back(私有_构建方法根链骨架(方法根节点, 上下文));
        }
        else {
            根.子项.push_back(私有_新节点("方法根为空"));
        }
        return 私有_页面刷新JSON(页面, &根);
    }

    if (页面 == "settings") {
        auto 根 = 私有_新节点(
            "参数设定 | 任务管理工作线程池大小="
            + std::to_string(快照.任务管理工作线程池当前有效大小)
            + " | 默认="
            + std::to_string(快照.任务管理工作线程池默认大小),
            0,
            true);
        根.子项.push_back(私有_新节点(
            "任务管理工作线程池大小 | 当前有效="
            + std::to_string(快照.任务管理工作线程池当前有效大小)
            + " | 待保存="
            + std::to_string(快照.任务管理工作线程池待保存大小)
            + " | 范围="
            + std::to_string(快照.任务管理工作线程池最小大小)
            + "-"
            + std::to_string(快照.任务管理工作线程池最大大小),
            0,
            false));
        根.子项.push_back(私有_新节点(
            "参数保存 | 最近保存="
            + 私有_页面摘要(私有_任务工作线程池最近参数保存文本(快照))
            + " | 最近错误="
            + 私有_页面摘要(私有_任务工作线程池最近参数错误文本(快照)),
            0,
            false));
        根.子项.push_back(私有_新节点(
            "参数应用 | 最近应用="
            + 私有_页面摘要(私有_任务工作线程池最近参数应用文本(快照))
            + " | 参数版本="
            + std::to_string(快照.任务管理工作线程池参数版本),
            0,
            false));
        return 私有_页面刷新JSON(页面, &根);
    }

    if (页面 == "camera-view") {
        auto 根 = 私有_新节点("该页面只保留独立相机播放窗口", 0, false);
        return 私有_页面刷新JSON(页面, &根);
    }

    if (页面 == "self-scene") {
        auto 根 = 私有_新节点("该页面只保留独立窗口启动控制", 0, false);
        return 私有_页面刷新JSON(页面, &根);
    }

    return 私有_页面刷新JSON(页面, nullptr, "未知控制面板页面");
}

// 功能：读取 SQL 控制面板树节点的一层直接子节点 JSON。
std::string 读取SQL控制面板子链JSON(
    std::string_view 区段ID,
    std::string_view 节点键)
{
    return 私有_SQL控制面板子链JSON(区段ID, 节点键);
}

// 功能：从指定来源读取数据或状态。
std::string 读取控制面板节点子项JSON(
    std::string_view 展开类型,
    const std::uintptr_t 节点指针,
    const std::size_t 树广度上限,
    const std::uintptr_t 附加参数)
{
    const auto 上下文 = 私有_创建构建上下文(树广度上限);

    if (展开类型 == "base-node") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<基础信息节点类*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON();
        }
        auto 节点 = 私有_构建基础信息树节点(
            已解析节点,
            上下文,
            1,
            {});
        私有_移除节点字段子项(节点);
        return 私有_树节点子项JSON(节点);
    }
    if (展开类型 == "need-node") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<需求节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON();
        }
        auto 节点 = 私有_构建需求树节点(
            已解析节点,
            上下文,
            1,
            {},
            false);
        私有_移除节点字段子项(节点);
        return 私有_树节点子项JSON(节点);
    }
    if (展开类型 == "task-node") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<任务节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON();
        }
        auto 节点 = 私有_构建任务树节点(
            已解析节点,
            上下文,
            1,
            {});
        私有_移除节点字段子项(节点);
        return 私有_树节点子项JSON(节点);
    }
    if (展开类型 == "method-node") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<方法节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON();
        }
        auto 节点 = 私有_构建方法树节点(
            已解析节点,
            上下文,
            1,
            {});
        私有_移除节点字段子项(节点);
        return 私有_树节点子项JSON(节点);
    }
    if (展开类型 == "base-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<基础信息节点类*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        auto 目标节点 = 私有_构建基础信息树节点(
            已解析节点,
            上下文,
            1,
            {});
        return 私有_树节点列表JSON(私有_提取节点字段详情(目标节点));
    }
    if (展开类型 == "need-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<需求节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        return 私有_树节点列表JSON(私有_构建需求主信息详情(已解析节点, 上下文));
    }
    if (展开类型 == "task-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<任务节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        return 私有_树节点列表JSON(私有_构建任务主信息详情(已解析节点));
    }
    if (展开类型 == "method-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<方法节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        return 私有_树节点列表JSON(私有_构建方法主信息详情(已解析节点));
    }
    if (展开类型 == "base-node-more") {
        结构_控制面板树节点 容器 = 私有_新节点("继续加载基础信息结构子节点");
        auto* 父节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<基础信息节点类*>(节点指针));
        if (!父节点) {
            return 私有_失效节点JSON("父节点已失效或已移出当前树");
        }
        const auto 子节点集 = 父节点 ? 私有_枚举子节点(父节点, (std::numeric_limits<std::size_t>::max)()) : std::vector<基础信息节点类*>{};
        const auto 起始偏移 = (std::min)(static_cast<std::size_t>(附加参数), 子节点集.size());
        const auto 结束偏移 = (std::min)(起始偏移 + 上下文.树广度上限, 子节点集.size());
        for (std::size_t 索引 = 起始偏移; 索引 < 结束偏移; ++索引) {
            容器.子项.push_back(私有_基础信息骨架节点(子节点集[索引], 上下文, false));
        }
        if (子节点集.size() > 结束偏移) {
            容器.子项.push_back(
                私有_创建结构省略节点(
                    "base-node-more",
                    私有_地址(父节点),
                    结束偏移,
                    子节点集.size() - 结束偏移));
        }
        return 私有_树节点子项JSON(容器);
    }
    if (展开类型 == "need-node-more") {
        结构_控制面板树节点 容器 = 私有_新节点("继续加载需求结构子节点");
        auto* 父节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<需求节点*>(节点指针));
        if (!父节点) {
            return 私有_失效节点JSON("父节点已失效或已移出当前树");
        }
        const auto 子节点集 = 私有_枚举子节点(
            父节点,
            (std::numeric_limits<std::size_t>::max)());
        const auto 起始偏移 = (std::min)(static_cast<std::size_t>(附加参数), 子节点集.size());
        const auto 结束偏移 = (std::min)(起始偏移 + 上下文.树广度上限, 子节点集.size());
        for (std::size_t 索引 = 起始偏移; 索引 < 结束偏移; ++索引) {
            容器.子项.push_back(私有_需求结构骨架节点(子节点集[索引], 上下文, false));
        }
        if (子节点集.size() > 结束偏移) {
            容器.子项.push_back(
                私有_创建结构省略节点(
                    "need-node-more",
                    私有_地址(父节点),
                    结束偏移,
                    子节点集.size() - 结束偏移));
        }
        return 私有_树节点子项JSON(容器);
    }
    if (展开类型 == "task-node-more") {
        结构_控制面板树节点 容器 = 私有_新节点("继续加载任务结构子节点");
        auto* 父节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<任务节点*>(节点指针));
        if (!父节点) {
            return 私有_失效节点JSON("父节点已失效或已移出当前树");
        }
        const auto 子节点集 = 私有_筛选需求任务链节点(
            私有_枚举子节点(父节点, (std::numeric_limits<std::size_t>::max)()));
        const auto 起始偏移 = (std::min)(static_cast<std::size_t>(附加参数), 子节点集.size());
        const auto 结束偏移 = (std::min)(起始偏移 + 上下文.树广度上限, 子节点集.size());
        for (std::size_t 索引 = 起始偏移; 索引 < 结束偏移; ++索引) {
            容器.子项.push_back(私有_任务骨架节点(子节点集[索引], 上下文, false));
        }
        if (子节点集.size() > 结束偏移) {
            容器.子项.push_back(
                私有_创建结构省略节点(
                    "task-node-more",
                    私有_地址(父节点),
                    结束偏移,
                    子节点集.size() - 结束偏移));
        }
        return 私有_树节点子项JSON(容器);
    }
    if (展开类型 == "method-node-more") {
        结构_控制面板树节点 容器 = 私有_新节点("继续加载方法结构子节点");
        auto* 父节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<方法节点*>(节点指针));
        if (!父节点) {
            return 私有_失效节点JSON("父节点已失效或已移出当前树");
        }
        const auto 子节点集 = 私有_枚举子节点(
            父节点,
            (std::numeric_limits<std::size_t>::max)());
        const auto 起始偏移 = (std::min)(static_cast<std::size_t>(附加参数), 子节点集.size());
        const auto 结束偏移 = (std::min)(起始偏移 + 上下文.树广度上限, 子节点集.size());
        for (std::size_t 索引 = 起始偏移; 索引 < 结束偏移; ++索引) {
            容器.子项.push_back(私有_方法骨架节点(子节点集[索引], 上下文, false));
        }
        if (子节点集.size() > 结束偏移) {
            容器.子项.push_back(
                私有_创建结构省略节点(
                    "method-node-more",
                    私有_地址(父节点),
                    结束偏移,
                    子节点集.size() - 结束偏移));
        }
        return 私有_树节点子项JSON(容器);
    }
    if (展开类型 == "need-list-more") {
        结构_控制面板树节点 容器 = 私有_新节点("需求列表分页");
        auto* 自我存在 = 自我.获取自我存在();
        if (auto* 需求根节点 = 自我存在 ? 世界树.获取需求根节点(自我存在) : nullptr) {
            std::vector<需求节点*> 分页节点集{};
            bool 分页还有更多 = false;
            std::size_t 分页已遍历数量 = 0;
            私有_读取需求列表分页(
                需求根节点,
                static_cast<std::size_t>(节点指针),
                私有_列表分页大小,
                分页节点集,
                分页还有更多,
                分页已遍历数量);
            for (auto* 节点 : 分页节点集) {
                容器.子项.push_back(私有_需求列表详情节点(节点, 上下文));
            }
            if (分页还有更多) {
                容器.子项.push_back(私有_创建需求列表加载更多节点(分页已遍历数量));
            }
        }
        return 私有_树节点子项JSON(容器);
    }

    return "[]";
}

// 功能：从指定来源读取数据或状态。
std::string 读取控制面板节点详情JSON(
    std::string_view 展开类型,
    const std::uintptr_t 节点指针,
    const std::size_t 树广度上限)
{
    const auto 上下文 = 私有_创建构建上下文(树广度上限);

    if (展开类型 == "base-node") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<基础信息节点类*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON();
        }
        const auto 节点 = 私有_构建基础信息树节点(
            已解析节点,
            上下文,
            1,
            {});
        return 私有_树节点列表JSON(私有_提取节点字段详情(节点));
    }
    if (展开类型 == "need-node") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<需求节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON();
        }
        return 私有_树节点列表JSON(私有_构建需求主信息详情(已解析节点, 上下文));
    }
    if (展开类型 == "task-node") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<任务节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON();
        }
        return 私有_树节点列表JSON(私有_构建任务主信息详情(已解析节点));
    }
    if (展开类型 == "method-node") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<方法节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON();
        }
        return 私有_树节点列表JSON(私有_构建方法主信息详情(已解析节点));
    }
    if (展开类型 == "base-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<基础信息节点类*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        const auto 节点 = 私有_构建基础信息树节点(
            已解析节点,
            上下文,
            1,
            {});
        return 私有_树节点列表JSON(私有_提取节点字段详情(节点));
    }
    if (展开类型 == "need-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<需求节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        return 私有_树节点列表JSON(私有_构建需求主信息详情(已解析节点, 上下文));
    }
    if (展开类型 == "task-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<任务节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        return 私有_树节点列表JSON(私有_构建任务主信息详情(已解析节点));
    }
    if (展开类型 == "method-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<方法节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        return 私有_树节点列表JSON(私有_构建方法主信息详情(已解析节点));
    }
    if (展开类型 == "thread-self"
        || 展开类型 == "thread-task-interface"
        || 展开类型 == "thread-worker"
        || 展开类型 == "thread-summary") {
        const auto 快照 = 读取控制面板快照(1, 16);
        任务管理工作线程::结构_工作线程实例快照 工作线程快照{};
        (void)任务管理工作线程::读取任务管理工作线程快照(&工作线程快照);
        任务管理线程协议::结构_任务界面线程快照 界面线程快照{};
        (void)任务管理界面线程::读取任务管理界面线程快照(&界面线程快照);

        auto 字段节点 = 私有_新节点("节点字段");
        字段节点.是字段分组 = true;
        if (展开类型 == "thread-self") {
            私有_追加叶字段(字段节点, "生命周期", 私有_线程生命周期文本(static_cast<枚举_线程生命周期状态>(快照.自我线程生命周期)));
            私有_追加叶字段(字段节点, "当前阶段", 快照.自我线程当前阶段);
            私有_追加叶字段(字段节点, "当前最终去向", 快照.自我线程当前最终去向);
            私有_追加叶字段(字段节点, "Tick计数", 快照.自我Tick计数);
            私有_追加叶字段(字段节点, "累计故障次数", 快照.自我线程累计故障次数);
            私有_追加叶字段(字段节点, "累计恢复次数", 快照.自我线程累计恢复次数);
        }
        else if (展开类型 == "thread-worker") {
            私有_追加叶字段(字段节点, "已启动", 工作线程快照.已启动);
            私有_追加叶字段(字段节点, "正在执行", 工作线程快照.正在执行);
            私有_追加叶字段(字段节点, "已收到请求", 工作线程快照.已收到请求);
            私有_追加叶字段(字段节点, "最近任务根ID", 工作线程快照.最近任务根ID);
            私有_追加叶字段(字段节点, "最近请求ID", 工作线程快照.最近请求ID);
            私有_追加叶字段(字段节点, "最近宿主任务主键", 工作线程快照.最近宿主任务主键);
            私有_追加叶字段(字段节点, "累计推进次数", 工作线程快照.累计推进次数);
            私有_追加叶字段(字段节点, "当前排队数", 工作线程快照.当前排队数);
            私有_追加叶字段(字段节点, "最近推进时间", 私有_时间文本(工作线程快照.最近推进时间));
            私有_追加叶字段(字段节点, "最近步骤主键", 工作线程快照.最近步骤主键);
            私有_追加叶字段(字段节点, "最近方法主键", 工作线程快照.最近方法主键);
            私有_追加叶字段(字段节点, "最近下一步去向", 工作线程快照.最近下一步去向);
            私有_追加叶字段(字段节点, "最近总控结果", 工作线程快照.最近总控结果);
            私有_追加叶字段(字段节点, "最近缺口归类", 工作线程快照.最近缺口归类);
            私有_追加叶字段(字段节点, "最近推进阶段", 工作线程快照.最近推进阶段);
            私有_追加叶字段(字段节点, "最近推进状况", 工作线程快照.最近推进状况);
            私有_追加叶字段(字段节点, "最近筹办承接", 工作线程快照.最近筹办承接);
            私有_追加叶字段(字段节点, "最近特征变化数", 工作线程快照.最近特征变化数);
        }
        else if (展开类型 == "thread-task-interface") {
            私有_追加叶字段(字段节点, "已启动", 界面线程快照.已启动);
            私有_追加叶字段(字段节点, "正在处理", 界面线程快照.正在处理);
            私有_追加叶字段(字段节点, "故障", 界面线程快照.故障);
            私有_追加叶字段(字段节点, "累计接收请求数", 界面线程快照.累计接收请求数);
            私有_追加叶字段(字段节点, "累计绑定任务虚拟存在数", 界面线程快照.累计绑定任务虚拟存在数);
            私有_追加叶字段(字段节点, "累计派发工作项数", 界面线程快照.累计派发工作项数);
            私有_追加叶字段(字段节点, "累计收到工作结果数", 界面线程快照.累计收到工作结果数);
            私有_追加叶字段(字段节点, "累计上行消息数", 界面线程快照.累计上行消息数);
            私有_追加叶字段(字段节点, "累计调度事件数", 界面线程快照.累计调度事件数);
            私有_追加叶字段(字段节点, "累计消费工作项数", 界面线程快照.累计消费工作项数);
            私有_追加叶字段(字段节点, "累计巡检任务数", 界面线程快照.累计巡检任务数);
            私有_追加叶字段(字段节点, "累计巡检入队任务数", 界面线程快照.累计巡检入队任务数);
            私有_追加叶字段(字段节点, "累计任务工作线程取项数", 界面线程快照.累计后台worker取项数);
            私有_追加叶字段(字段节点, "累计任务工作线程派发数", 界面线程快照.累计后台worker派发数);
            私有_追加叶字段(字段节点, "当前请求队列长度", 界面线程快照.当前请求队列长度);
            私有_追加叶字段(字段节点, "当前等待工作项数", 界面线程快照.当前等待工作项数);
            私有_追加叶字段(字段节点, "当前待筹办工作项数", 界面线程快照.当前待筹办工作项数);
            私有_追加叶字段(字段节点, "当前待执行工作项数", 界面线程快照.当前待执行工作项数);
            私有_追加叶字段(字段节点, "当前完成工作项数", 界面线程快照.当前完成工作项数);
            私有_追加叶字段(字段节点, "当前死信工作项数", 界面线程快照.当前死信工作项数);
            私有_追加叶字段(字段节点, "当前在途工作项数", 界面线程快照.当前在途工作项数);
            私有_追加叶字段(字段节点, "当前任务状态快照数", 界面线程快照.当前任务状态快照数);
            私有_追加叶字段(字段节点, "当前等待任务索引数", 界面线程快照.当前等待任务索引数);
            私有_追加叶字段(字段节点, "当前挂起任务索引数", 界面线程快照.当前挂起任务索引数);
            私有_追加叶字段(字段节点, "当前上行队列长度", 界面线程快照.当前上行队列长度);
            私有_追加叶字段(字段节点, "任务工作线程池运行", 界面线程快照.后台worker池运行);
            私有_追加叶字段(字段节点, "任务工作线程数", 界面线程快照.后台worker线程数);
            私有_追加叶字段(字段节点, "任务工作线程池默认大小", 界面线程快照.任务管理工作线程池默认大小);
            私有_追加叶字段(字段节点, "任务工作线程池最小大小", 界面线程快照.任务管理工作线程池最小大小);
            私有_追加叶字段(字段节点, "任务工作线程池最大大小", 界面线程快照.任务管理工作线程池最大大小);
            私有_追加叶字段(字段节点, "任务工作线程池当前有效大小", 界面线程快照.任务管理工作线程池当前有效大小);
            私有_追加叶字段(字段节点, "任务工作线程池待保存大小", 界面线程快照.任务管理工作线程池待保存大小);
            私有_追加叶字段(字段节点, "任务工作线程池参数版本", 界面线程快照.任务管理工作线程池参数版本);
            私有_追加叶字段(字段节点, "最近参数保存结果", 私有_任务工作线程池最近参数保存文本(界面线程快照));
            私有_追加叶字段(字段节点, "最近参数错误", 私有_截断文本(私有_任务工作线程池最近参数错误文本(界面线程快照), 200));
            私有_追加叶字段(字段节点, "最近参数应用结果", 私有_任务工作线程池最近参数应用文本(界面线程快照));
            私有_追加叶字段(字段节点, "待筹办队列头部任务", 界面线程快照.待筹办队列头部任务主键);
            私有_追加叶字段(字段节点, "待筹办队列头部优先级", 界面线程快照.待筹办队列头部优先级);
            私有_追加叶字段(字段节点, "待筹办队列头部兜底键", 私有_截断文本(界面线程快照.待筹办队列头部排序兜底键, 200));
            私有_追加叶字段(字段节点, "待执行队列头部任务", 界面线程快照.待执行队列头部任务主键);
            私有_追加叶字段(字段节点, "待执行队列头部优先级", 界面线程快照.待执行队列头部优先级);
            私有_追加叶字段(字段节点, "待执行队列头部兜底键", 私有_截断文本(界面线程快照.待执行队列头部排序兜底键, 200));
            私有_追加叶字段(字段节点, "最近巡检任务数", 界面线程快照.最近巡检任务数);
            私有_追加叶字段(字段节点, "最近巡检入队任务数", 界面线程快照.最近巡检入队任务数);
            私有_追加叶字段(字段节点, "最近巡检耗时微秒", 界面线程快照.最近巡检耗时微秒);
            私有_追加叶字段(字段节点, "最近巡检可推进任务数", 界面线程快照.最近巡检可推进任务数);
            私有_追加叶字段(字段节点, "最近巡检等待任务数", 界面线程快照.最近巡检等待任务数);
            私有_追加叶字段(字段节点, "最近巡检挂起任务数", 界面线程快照.最近巡检挂起任务数);
            私有_追加叶字段(字段节点, "最近巡检终结任务数", 界面线程快照.最近巡检终结任务数);
            私有_追加叶字段(字段节点, "最近巡检跳过已有工作项数", 界面线程快照.最近巡检跳过已有工作项数);
            私有_追加叶字段(字段节点, "最近巡检跳过缺结构数", 界面线程快照.最近巡检跳过缺结构数);
            私有_追加叶字段(字段节点, "最近巡检跳过不可推进数", 界面线程快照.最近巡检跳过不可推进数);
            私有_追加叶字段(字段节点, "最近巡检跳过入队预算数", 界面线程快照.最近巡检跳过入队预算数);
            私有_追加叶字段(字段节点, "最近请求ID", 界面线程快照.最近请求ID);
            私有_追加叶字段(字段节点, "最近工作项ID", 界面线程快照.最近工作项ID);
            私有_追加叶字段(字段节点, "最近调度事件ID", 界面线程快照.最近调度事件ID);
            私有_追加叶字段(字段节点, "最近调度事件类型", 界面线程快照.最近调度事件类型);
            私有_追加叶字段(字段节点, "最近工作项类型", 界面线程快照.最近工作项类型);
            私有_追加叶字段(字段节点, "最近任务工作线程序号", 界面线程快照.最近后台worker序号);
            私有_追加叶字段(字段节点, "最近任务工作线程工作项ID", 界面线程快照.最近后台worker工作项ID);
            私有_追加叶字段(字段节点, "最近任务工作线程任务主键", 界面线程快照.最近后台worker任务主键);
            私有_追加叶字段(字段节点, "最近任务工作线程工作项类型", 界面线程快照.最近后台worker工作项类型);
            私有_追加叶字段(字段节点, "最近任务工作线程状态", 界面线程快照.最近后台worker状态);
            私有_追加叶字段(字段节点, "最近任务主键", 界面线程快照.最近任务主键);
            私有_追加叶字段(字段节点, "最近任务状态", 界面线程快照.最近任务状态);
            私有_追加叶字段(字段节点, "最近任务状态版本", 界面线程快照.最近任务状态版本);
            私有_追加叶字段(字段节点, "最近任务虚拟存在主键", 界面线程快照.最近任务虚拟存在主键);
            私有_追加叶字段(字段节点, "最近需求主键", 界面线程快照.最近需求主键);
            私有_追加叶字段(字段节点, "最近调度动作", 界面线程快照.最近调度动作);
        }
        else if (展开类型 == "thread-summary") {
            私有_追加叶字段(
                字段节点,
                "生命周期",
                控制面板摘要线程生命周期文本(
                    static_cast<枚举_控制面板摘要线程生命周期状态>(
                        快照.控制面板摘要线程生命周期)));
            私有_追加叶字段(字段节点, "已启动", 快照.控制面板摘要线程已启动);
            私有_追加叶字段(字段节点, "健康运行", 快照.控制面板摘要线程健康运行);
            私有_追加叶字段(字段节点, "快照序号", 快照.控制面板摘要快照序号);
            私有_追加叶字段(
                字段节点,
                "自我线程生命周期",
                私有_线程生命周期文本(static_cast<枚举_线程生命周期状态>(快照.自我线程生命周期)));
            私有_追加叶字段(字段节点, "自我Tick计数", 快照.自我Tick计数);
            私有_追加叶字段(字段节点, "需求数", 快照.需求数);
            私有_追加叶字段(字段节点, "任务数", 快照.任务数);
            私有_追加叶字段(字段节点, "方法数", 快照.方法数);
            私有_追加叶字段(字段节点, "运行证据方法数", 快照.控制面板摘要_运行证据方法数);
            私有_追加叶字段(字段节点, "失败任务数", 快照.控制面板摘要_失败任务数);
            私有_追加叶字段(字段节点, "等待任务数", 快照.控制面板摘要_等待任务数);
            私有_追加叶字段(字段节点, "最近方法主键", 快照.控制面板摘要_最近方法主键);
            私有_追加叶字段(字段节点, "最近方法时间", 快照.控制面板摘要_最近方法时间);
            私有_追加叶字段(字段节点, "最近任务时间", 快照.控制面板摘要_最近任务时间);
            私有_追加叶字段(字段节点, "自检生命周期值", 快照.控制面板自检生命周期值);
            私有_追加叶字段(字段节点, "自检模式值", 快照.控制面板自检最近模式值);
            私有_追加叶字段(字段节点, "自检健康运行", 快照.控制面板自检健康运行);
            私有_追加叶字段(字段节点, "自检Tick计数", 快照.控制面板自检Tick计数);
            私有_追加叶字段(字段节点, "自检累计发现问题数", 快照.控制面板自检累计发现问题数);
            私有_追加叶字段(字段节点, "自检累计提交需求数", 快照.控制面板自检累计提交需求数);
            私有_追加叶字段(字段节点, "自检累计提交报告数", 快照.控制面板自检累计提交报告数);
            私有_追加叶字段(字段节点, "自检累计需求化失败数", 快照.控制面板自检累计需求化失败数);
            私有_追加叶字段(字段节点, "自检最近检查时间", 快照.控制面板自检最近检查时间);
            私有_追加叶字段(字段节点, "自检最近提交时间", 快照.控制面板自检最近提交时间);
            私有_追加叶字段(字段节点, "自检报告待处理数", 快照.自检报告待处理数);
            私有_追加叶字段(字段节点, "待休眠评估报告数", 快照.自检报告休眠期评估数);
            私有_追加叶字段(字段节点, "待映射确认报告数", 快照.自检报告待映射确认数);
            私有_追加叶字段(字段节点, "仅报告保留数", 快照.自检报告仅报告保留数);
            私有_追加叶字段(字段节点, "越界需求化拒绝数", 快照.自检报告越界需求化拒绝数);
            私有_追加叶字段(字段节点, "最近休眠期门控报告数", 快照.自检报告最近休眠期门控数);
            私有_追加叶字段(字段节点, "最近报告处置", 私有_截断文本(快照.自检报告最近处置, 200));
            私有_追加叶字段(字段节点, "自检报告修复门控", 私有_截断文本(快照.自检报告修复门控摘要, 200));
        }
        else {
            私有_追加叶字段(字段节点, "项目线程信息来源", "消息中间件 / 控制面板线程生命周期消息表");
            私有_追加叶字段(字段节点, "说明", "未匹配到项目线程详情类型；线程状态页不显示系统线程枚举。");
        }
        return 私有_树节点列表JSON({ std::move(字段节点) });
    }

    return "[]";
}

// 功能：生成展示用文本、界面片段或可视化内容。
std::string 渲染控制面板摘要(
    const 结构_控制面板快照& 快照,
    std::size_t,
    std::size_t)
{
    std::ostringstream 输出;
    输出 << "控制面板摘要\n"
        << "  - 自我: 初始化=" << 布尔文本_是或否(快照.自我已初始化)
        << " | 存在=" << 布尔文本_是或否(快照.自我存在已建立)
        << " | 内部世界=" << 布尔文本_是或否(快照.自我内部世界已建立)
        << " | 根字段=需" << 布尔文本_是或否(快照.自我需求根字段已建立)
        << "/任" << 布尔文本_是或否(快照.自我任务根字段已建立)
        << "/方" << 布尔文本_是或否(快照.自我方法根字段已建立)
        << " | 待机=" << 布尔文本_是或否(快照.自我待机状态)
        << " | 安全=" << 快照.自我安全值
        << " | 服务=" << 快照.自我服务值
        << " | 风险安全=" << 快照.自我风险安全值
        << '\n'
        << "  - 自我线程: 生命周期="
        << 私有_线程生命周期文本(static_cast<枚举_线程生命周期状态>(快照.自我线程生命周期))
        << " | 阶段=" << 私有_页面摘要(快照.自我线程当前阶段)
        << " | 去向=" << 私有_页面摘要(快照.自我线程当前最终去向)
        << " | Tick=" << 快照.自我Tick计数
        << " | 故障=" << 快照.自我线程累计故障次数
        << " | 恢复=" << 快照.自我线程累计恢复次数
        << '\n'
        << "  - 摘要线程: 生命周期="
        << 控制面板摘要线程生命周期文本(
            static_cast<枚举_控制面板摘要线程生命周期状态>(
                快照.控制面板摘要线程生命周期))
        << " | 健康=" << 布尔文本_是或否(快照.控制面板摘要线程健康运行)
        << " | 快照=" << 快照.控制面板摘要快照序号
        << " | 需求=" << 快照.需求数
        << " | 任务=" << 快照.任务数
        << " | 方法=" << 快照.方法数
        << '\n'
        << "  - 需求树: 总数=" << 快照.需求数
        << " | 已满足=" << 快照.需求已满足数
        << " | 未满足=" << 快照.需求未满足数
        << " | 有子=" << 快照.需求树有子节点数
        << " | 叶子=" << 快照.需求树叶子节点数
        << " | 活动阻塞=" << 快照.需求树活动阻塞需求数
        << " | 逻辑组织=" << 快照.需求树逻辑组织需求数
        << " | OR令牌目标=" << 快照.需求树OR组结算令牌需求数
        << " | 当前主需求=" << 私有_页面摘要(快照.需求树当前主需求主键.empty()
            ? std::string("空")
            : 快照.需求树当前主需求主键)
        << '\n'
        << "  - 场景安全评估: 基础观察="
        << 快照.自我场景基础观察事实可用状态
        << " | 基础风险判断=" << 快照.自我场景基础风险判断状态
        << " | 风险明确=" << 快照.自我场景风险状态明确状态
        << " | 风险值计算=" << 快照.自我场景基础风险值计算状态
        << " | 评估安全值=" << 快照.自我场景当前场景评估安全值候选
        << " | 安全度=" << 快照.自我场景当前场景安全度候选
        << " | 判定=" << 快照.自我场景当前场景安全判定状态
        << " | 场景影响=" << 快照.自我场景风险安全_场景影响部分候选
        << " | 可结算=" << 快照.自我场景风险安全_场景影响部分可结算状态
        << " | 入账=" << 快照.自我场景风险安全_场景影响部分已入账状态
        << " | 不可结算原因=" << 快照.自我场景风险安全_场景影响部分不可结算原因
        << " | 风险安全层候选=" << 快照.自我场景风险安全层候选
        << " | 风险安全投影候选=" << 快照.自我场景风险安全层投影候选
        << " | 风险安全层明确=" << 快照.自我场景风险安全层明确状态
        << " | 风险安全层可聚合=" << 快照.自我场景风险安全层可聚合状态
        << " | 风险安全层入账=" << 快照.自我场景风险安全层已入账状态
        << " | 未搜索掩码=" << 快照.自我场景风险因素未搜索掩码
        << " | 默认满足掩码=" << 快照.自我场景风险因素默认满足掩码
        << " | 已入账掩码=" << 快照.自我场景风险因素已入账掩码
        << " | 未明确原因=" << 快照.自我场景安全评估证据不足原因
        << '\n'
        << "  - 树规模: 基础节点=" << 快照.基础信息节点数
        << " | 场景=" << 快照.场景数
        << " | 存在=" << 快照.存在数
        << " | 状态=" << 快照.状态数
        << " | 动态=" << 快照.动态数
        << " | 需求=" << 快照.需求数
        << " | 任务节点=" << 快照.任务数
        << " (头=" << 快照.任务头节点数
        << ", 步骤=" << 快照.任务步骤节点数
        << ", 其他=" << 快照.任务其他节点数 << ")"
        << " | 方法=" << 快照.方法数
        << '\n'
        << "  - 补方法: 待处理方法=" << 快照.自我待处理方法数量
        << " | 可用方法=" << 快照.自我可用方法数量
        << " | 缺口承接=已接管"
        << '\n'
        << "  - 任务管理: 工作线程="
        << 布尔文本_是或否(快照.任务管理工作线程已启动)
        << " | 界面线程=" << 布尔文本_是或否(快照.任务管理界面线程已启动)
        << " | 请求=" << 快照.任务管理界面线程累计接收请求数
        << " | 上行=" << 快照.任务管理界面线程当前上行队列长度
        << " | 执行=" << 布尔文本_是或否(快照.任务管理工作线程正在执行)
        << " | 排队=" << 快照.任务管理工作线程当前排队数
        << " | 任务工作线程池=" << (快照.任务管理界面线程后台worker池运行 ? "运行" : "停止")
        << "/" << 快照.任务管理界面线程后台worker线程数
        << " | 最近巡检=" << 快照.任务管理界面线程最近巡检任务数
        << "/" << 快照.任务管理界面线程最近巡检入队任务数
        << " | 不可推进=" << 快照.任务管理界面线程最近巡检跳过不可推进数
        << " | 缺口=" << 私有_页面摘要(快照.任务管理工作线程最近缺口归类)
        << " | 特征变化=" << 快照.任务管理工作线程最近特征变化数
        << '\n';
    return 输出.str();
}

// 功能：生成展示用文本、界面片段或可视化内容。
std::string 渲染任务管理摘要(
    const 结构_控制面板快照& 快照,
    std::size_t)
{
    std::ostringstream 输出;
    输出 << "任务管理摘要\n"
        << "  - 自我线程: 阶段=" << 私有_页面摘要(快照.自我线程当前阶段)
        << " | 去向=" << 私有_页面摘要(快照.自我线程当前最终去向)
        << " | Tick=" << 快照.自我Tick计数
        << '\n'
        << "  - 工作线程: 已启动=" << 布尔文本_是或否(快照.任务管理工作线程已启动)
        << " | 正在执行=" << 布尔文本_是或否(快照.任务管理工作线程正在执行)
        << " | 已收到请求=" << 布尔文本_是或否(快照.任务管理工作线程已收到请求)
        << " | 推进次数=" << 快照.任务管理工作线程累计推进次数
        << " | 当前排队=" << 快照.任务管理工作线程当前排队数
        << '\n'
        << "  - 界面线程: 已启动=" << 布尔文本_是或否(快照.任务管理界面线程已启动)
        << " | 正在处理=" << 布尔文本_是或否(快照.任务管理界面线程正在处理)
        << " | 故障=" << 布尔文本_是或否(快照.任务管理界面线程故障)
        << " | 请求=" << 快照.任务管理界面线程累计接收请求数
        << " | 调度事件=" << 快照.任务管理界面线程累计调度事件数
        << " | 消费工作项=" << 快照.任务管理界面线程累计消费工作项数
        << " | 派发工作项=" << 快照.任务管理界面线程累计派发工作项数
        << " | 巡检累计=" << 快照.任务管理界面线程累计巡检任务数
        << "/" << 快照.任务管理界面线程累计巡检入队任务数
        << " | 最近巡检=" << 快照.任务管理界面线程最近巡检任务数
        << "/" << 快照.任务管理界面线程最近巡检入队任务数
        << " | 巡检耗时微秒=" << 快照.任务管理界面线程最近巡检耗时微秒
        << " | 可推进=" << 快照.任务管理界面线程最近巡检可推进任务数
        << " | 等待=" << 快照.任务管理界面线程最近巡检等待任务数
        << " | 挂起=" << 快照.任务管理界面线程最近巡检挂起任务数
        << " | 终结=" << 快照.任务管理界面线程最近巡检终结任务数
        << " | 跳过已有=" << 快照.任务管理界面线程最近巡检跳过已有工作项数
        << " | 缺结构=" << 快照.任务管理界面线程最近巡检跳过缺结构数
        << " | 不可推进=" << 快照.任务管理界面线程最近巡检跳过不可推进数
        << " | 预算跳过=" << 快照.任务管理界面线程最近巡检跳过入队预算数
        << " | 待筹办=" << 快照.任务管理界面线程当前待筹办工作项数
        << " | 待执行=" << 快照.任务管理界面线程当前待执行工作项数
        << " | 完成队列=" << 快照.任务管理界面线程当前完成工作项数
        << " | 在途=" << 快照.任务管理界面线程当前在途工作项数
        << " | 任务工作线程池=" << (快照.任务管理界面线程后台worker池运行 ? "运行" : "停止")
        << "/" << 快照.任务管理界面线程后台worker线程数
        << " | 任务工作线程池参数=" << 快照.任务管理工作线程池当前有效大小
        << " (默认" << 快照.任务管理工作线程池默认大小 << ")"
        << " | 任务工作线程取项=" << 快照.任务管理界面线程累计后台worker取项数
        << " | 任务工作线程派发=" << 快照.任务管理界面线程累计后台worker派发数
        << " | 等待索引=" << 快照.任务管理界面线程当前等待任务索引数
        << " | 挂起索引=" << 快照.任务管理界面线程当前挂起任务索引数
        << " | 死信=" << 快照.任务管理界面线程当前死信工作项数
        << " | 上行队列=" << 快照.任务管理界面线程当前上行队列长度
        << " | 最近调度=" << 私有_页面摘要(快照.任务管理界面线程最近调度动作)
        << " | 最近事件=" << 私有_页面摘要(快照.任务管理界面线程最近调度事件类型)
        << "/" << 快照.任务管理界面线程最近调度事件ID
        << " | 最近工作项=" << 私有_页面摘要(快照.任务管理界面线程最近工作项类型)
        << " | 筹办头=" << 私有_页面摘要(快照.任务管理界面线程待筹办队列头部任务主键)
        << "/" << 快照.任务管理界面线程待筹办队列头部优先级
        << " | 执行头=" << 私有_页面摘要(快照.任务管理界面线程待执行队列头部任务主键)
        << "/" << 快照.任务管理界面线程待执行队列头部优先级
        << " | 最近任务工作线程=" << 快照.任务管理界面线程最近后台worker序号
        << "/" << 私有_页面摘要(快照.任务管理界面线程最近后台worker状态)
        << " | 参数应用=" << 私有_页面摘要(私有_任务工作线程池最近参数应用文本(快照))
        << " | 参数错误=" << 私有_页面摘要(私有_任务工作线程池最近参数错误文本(快照))
        << " | 最近状态=" << 私有_页面摘要(快照.任务管理界面线程最近任务状态)
        << " | 最近任务=" << 私有_页面摘要(快照.任务管理界面线程最近任务主键)
        << " | 任务虚拟存在=" << 私有_页面摘要(快照.任务管理界面线程最近任务虚拟存在主键)
        << " | 巡检跳过不可推进=" << 快照.任务管理界面线程最近巡检跳过不可推进数
        << '\n'
        << "  - 树规模: 需求=" << 快照.需求数
        << " | 任务节点=" << 快照.任务数
        << " (头=" << 快照.任务头节点数
        << ", 步骤=" << 快照.任务步骤节点数
        << ", 其他=" << 快照.任务其他节点数 << ")"
        << " | 方法=" << 快照.方法数
        << " | 缺口承接=已接管"
        << '\n';
    return 输出.str();
}

// 功能：生成展示用文本、界面片段或可视化内容。
std::string 渲染缺口摘要(
    const 结构_控制面板快照& 快照,
    std::size_t)
{
    std::ostringstream 输出;
    输出 << "补方法摘要\n"
        << "  - 缺口承接=已接管"
        << " | 待处理方法=" << 快照.自我待处理方法数量
        << " | 可用方法=" << 快照.自我可用方法数量
        << " | 恢复请求=" << 快照.缺口恢复请求数
        << '\n'
        << "  - 判定: 应触发缺口承接=" << 布尔文本_是或否(快照.应触发缺口承接)
        << " | 应申请重试恢复=" << 布尔文本_是或否(快照.缺口应申请重试恢复)
        << " | 应申请收束恢复=" << 布尔文本_是或否(快照.缺口应申请收束恢复)
        << '\n'
        << "  - 最近反馈=" << 私有_页面摘要(快照.缺口最近反馈摘要)
        << '\n';
    return 输出.str();
}

// 功能：生成展示用文本、界面片段或可视化内容。
std::string 渲染需求树生长摘要(const 结构_控制面板快照& 快照)
{
    std::ostringstream 输出;
    输出 << "需求树生长摘要\n"
        << "  - 规模: 总数=" << 快照.需求数
        << " | 已满足=" << 快照.需求已满足数
        << " | 状态已达未截止=" << 快照.需求状态已达未截止数
        << " | 未满足=" << 快照.需求未满足数
        << '\n'
        << "  - 结构: 有子=" << 快照.需求树有子节点数
        << " | 叶子=" << 快照.需求树叶子节点数
        << " | 活动阻塞=" << 快照.需求树活动阻塞需求数
        << " | 活动叶子=" << 快照.需求树活动叶子需求数
        << " | 未任务化叶子=" << 快照.需求树活动叶子需求未任务化数
        << " | 非阻塞=" << 快照.需求树非阻塞需求数
        << " | 已截止=" << 快照.需求树已截止需求数
        << " | 重复目标组=" << 快照.需求树重复目标组数
        << " | 重复目标需求=" << 快照.需求树重复目标需求数
        << " | 权重检查父=" << 快照.需求树父子权重检查父需求数
        << " | 权重异常=" << 快照.需求树父子权重异常需求数
        << " | 完成任务仍活动需求=" << 快照.需求树完成任务仍活动需求数
        << '\n'
        << "  - 逻辑组织: 总数=" << 快照.需求树逻辑组织需求数
        << " | AND=" << 快照.需求树AND组需求数
        << " | OR=" << 快照.需求树OR组需求数
        << " | 方法路径=" << 快照.需求树方法路径组需求数
        << " | 因果子链=" << 快照.需求树因果子链支撑需求数
        << " | OR令牌目标=" << 快照.需求树OR组结算令牌需求数
        << '\n'
        << "  - 目标绑定: 完整=" << 快照.需求树目标绑定完整需求数
        << " | 异常=" << 快照.需求树目标绑定异常需求数
        << " | 缺目标状态=" << 快照.需求树缺目标状态需求数
        << " | 缺目标宿主=" << 快照.需求树缺目标宿主需求数
        << " | 缺目标特征类型=" << 快照.需求树缺目标特征类型需求数
        << " | 宿主不一致=" << 快照.需求树目标状态宿主不一致需求数
        << " | 特征未归属宿主=" << 快照.需求树目标特征未归属宿主需求数
        << '\n'
        << "  - 首个目标绑定异常: 主键=" << 私有_页面摘要(
            快照.需求树首个目标绑定异常需求主键.empty()
                ? std::string("无")
                : 快照.需求树首个目标绑定异常需求主键)
        << " | 父=" << 私有_页面摘要(
            快照.需求树首个目标绑定异常父主键.empty()
                ? std::string("空")
                : 快照.需求树首个目标绑定异常父主键)
        << " | 目标状态=" << 私有_页面摘要(
            快照.需求树首个目标绑定异常目标状态主键.empty()
                ? std::string("空")
                : 快照.需求树首个目标绑定异常目标状态主键)
        << " | 目标宿主=" << 私有_页面摘要(
            快照.需求树首个目标绑定异常目标宿主主键.empty()
                ? std::string("空")
                : 快照.需求树首个目标绑定异常目标宿主主键)
        << " | 目标特征=" << 私有_页面摘要(
            快照.需求树首个目标绑定异常目标特征主键.empty()
                ? std::string("空")
                : 快照.需求树首个目标绑定异常目标特征主键)
        << '\n'
        << "  - 首个未任务化叶子需求: 主键=" << 私有_页面摘要(
            快照.需求树首个未任务化叶子需求主键.empty()
                ? std::string("无")
                : 快照.需求树首个未任务化叶子需求主键)
        << " | 父=" << 私有_页面摘要(
            快照.需求树首个未任务化叶子需求父主键.empty()
                ? std::string("空")
                : 快照.需求树首个未任务化叶子需求父主键)
        << " | 目标宿主=" << 私有_页面摘要(
            快照.需求树首个未任务化叶子需求目标主体主键.empty()
                ? std::string("空")
                : 快照.需求树首个未任务化叶子需求目标主体主键)
        << " | 目标特征=" << 私有_页面摘要(
            快照.需求树首个未任务化叶子需求目标特征主键.empty()
                ? std::string("空")
                : 快照.需求树首个未任务化叶子需求目标特征主键)
        << '\n'
        << "  - 首个完成任务仍活动需求: 主键=" << 私有_页面摘要(
            快照.需求树首个完成任务仍活动需求主键.empty()
                ? std::string("无")
                : 快照.需求树首个完成任务仍活动需求主键)
        << " | 任务=" << 私有_页面摘要(
            快照.需求树首个完成任务仍活动任务主键.empty()
                ? std::string("空")
                : 快照.需求树首个完成任务仍活动任务主键)
        << '\n'
        << "  - 当前主需求: 主键=" << 私有_页面摘要(
            快照.需求树当前主需求主键.empty()
                ? std::string("空")
                : 快照.需求树当前主需求主键)
        << " | 父=" << 私有_页面摘要(
            快照.需求树当前主需求父主键.empty()
                ? std::string("空")
                : 快照.需求树当前主需求父主键)
        << " | 目标宿主=" << 私有_页面摘要(
            快照.需求树当前主需求目标主体主键.empty()
                ? std::string("空")
                : 快照.需求树当前主需求目标主体主键)
        << " | 目标特征=" << 私有_页面摘要(
            快照.需求树当前主需求目标特征主键.empty()
                ? std::string("空")
                : 快照.需求树当前主需求目标特征主键)
        << " | 有任务=" << 布尔文本_是或否(快照.需求树当前主需求有任务)
        << " | 结构形态=" << 私有_页面摘要(
            快照.需求树当前主需求结构形态.empty()
                ? std::string("空")
                : 快照.需求树当前主需求结构形态)
        << " | 普通筹办=" << 布尔文本_是或否(快照.需求树当前主需求可普通任务化)
        << " | OR令牌=" << 私有_页面摘要(
            快照.需求树当前主需求OR令牌状态.empty()
                ? std::string("不适用")
                : 快照.需求树当前主需求OR令牌状态)
        << " | 激活路径=" << 私有_页面摘要(
            快照.需求树当前主需求当前激活路径.empty()
                ? std::string("不适用")
                : 快照.需求树当前主需求当前激活路径)
        << '\n'
        << "  - 任务承接: 任务数=" << 快照.任务数
        << " | 任务头=" << 快照.任务头节点数
        << " | 未启动=" << 快照.任务状态未启动数
        << " | 运行中=" << 快照.任务状态运行中数
        << " | 就绪=" << 快照.任务状态就绪数
        << " | 等待=" << 快照.任务状态等待中数
        << " | 待重筹办=" << 快照.任务状态待重筹办数
        << " | 完成=" << 快照.任务状态完成数
        << " | 已结算=" << 快照.任务状态已结算数
        << " | 完成缺需求=" << 快照.任务完成但缺对应需求数
        << '\n'
        << "  - 任务管理: 请求=" << 快照.任务管理界面线程累计接收请求数
        << " | 绑定任务虚拟存在=" << 快照.任务管理界面线程累计绑定任务虚拟存在数
        << " | 工作项=" << 快照.任务管理界面线程累计派发工作项数
        << " | 工作结果=" << 快照.任务管理界面线程累计收到工作结果数
        << " | 上行=" << 快照.任务管理界面线程累计上行消息数
        << " | 当前等待工作项=" << 快照.任务管理界面线程当前等待工作项数
        << " | 待筹办=" << 快照.任务管理界面线程当前待筹办工作项数
        << " | 待执行=" << 快照.任务管理界面线程当前待执行工作项数
        << " | 完成队列=" << 快照.任务管理界面线程当前完成工作项数
        << " | 在途=" << 快照.任务管理界面线程当前在途工作项数
        << " | 等待索引=" << 快照.任务管理界面线程当前等待任务索引数
        << " | 挂起索引=" << 快照.任务管理界面线程当前挂起任务索引数
        << " | 死信=" << 快照.任务管理界面线程当前死信工作项数
        << '\n';
    return 输出.str();
}

// 功能：根据当前输入生成目标数据、场景、动态或回执。
std::string 私有_生成控制面板HTML(
    const 结构_控制面板快照& 快照,
    std::size_t,
    枚举_控制面板HTML用途 用途)
{
    const bool 自我场景窗口模式 = 用途 == 枚举_控制面板HTML用途::自我场景窗口;
    const auto 线程树JSON = 私有_树节点JSON(快照.线程状态树根);
    const auto 世界树JSON = 私有_树节点JSON(快照.世界树根);
    const auto 因果信息JSON = 私有_树节点JSON(快照.因果信息根);
    const auto 需求树JSON = 私有_树节点JSON(快照.需求树根);
    const auto 需求列表JSON = 私有_树节点JSON(快照.需求列表树根);
    const auto 任务树JSON = 私有_树节点JSON(快照.任务树根);
    const auto 方法树JSON = 私有_树节点JSON(快照.方法树根);
    const auto 自我场景复现JSON = 私有_自我场景复现JSON(快照);
    auto 参数设定根 = 私有_新节点(
        "参数设定 | 任务管理工作线程池大小="
        + std::to_string(快照.任务管理工作线程池当前有效大小)
        + " | 默认="
        + std::to_string(快照.任务管理工作线程池默认大小),
        0,
        true);
    参数设定根.子项.push_back(私有_新节点(
        "任务管理工作线程池大小 | 当前有效="
        + std::to_string(快照.任务管理工作线程池当前有效大小)
        + " | 待保存="
        + std::to_string(快照.任务管理工作线程池待保存大小)
        + " | 范围="
        + std::to_string(快照.任务管理工作线程池最小大小)
        + "-"
        + std::to_string(快照.任务管理工作线程池最大大小),
        0,
        false));
    参数设定根.子项.push_back(私有_新节点(
        "参数保存 | 最近保存="
        + 私有_页面摘要(私有_任务工作线程池最近参数保存文本(快照))
        + " | 最近错误="
        + 私有_页面摘要(私有_任务工作线程池最近参数错误文本(快照)),
        0,
        false));
    参数设定根.子项.push_back(私有_新节点(
        "参数应用 | 最近应用="
        + 私有_页面摘要(私有_任务工作线程池最近参数应用文本(快照))
        + " | 参数版本="
        + std::to_string(快照.任务管理工作线程池参数版本),
        0,
        false));
    const auto 参数设定JSON = 私有_树节点JSON(参数设定根);

    const auto 树交互提示 = 私有_转义HTML("结构节点与字段摘要");
    const auto 线程树交互提示 = 私有_转义HTML("完整项目线程列表，不分页、不省略；点击项目线程查看右侧详情");
    const auto 任务树交互提示 = 私有_转义HTML("任务根节点结构与运行壳摘要");
    std::size_t 线程已知逻辑线程数 = 0;
    std::size_t 线程当前运行数 = 0;
    std::size_t 线程未启动数 = 0;
    std::size_t 线程已结束数 = 0;
    std::size_t 线程故障数 = 0;
    私有_计算线程状态页统计(
        快照,
        线程已知逻辑线程数,
        线程当前运行数,
        线程未启动数,
        线程已结束数,
        线程故障数);
    const auto 线程树标题前缀 = std::string("线程列表 | ");
    const auto 线程统计文本 = 快照.线程状态树根.文本.rfind(线程树标题前缀, 0) == 0
        ? 快照.线程状态树根.文本.substr(线程树标题前缀.size())
        : 私有_线程状态统计文本(
            线程已知逻辑线程数,
            线程当前运行数,
            线程未启动数,
            线程已结束数,
            线程故障数);
    const auto 线程徽标数量 = 快照.线程状态树根.子项.empty()
        ? 线程已知逻辑线程数
        : 快照.线程状态树根.子项.size();
    const auto 线程页摘要 = 私有_转义HTML(
        线程统计文本
        + " | 自我线程阶段=" + 私有_页面摘要(快照.自我线程当前阶段)
        + " | 去向=" + 私有_页面摘要(快照.自我线程当前最终去向)
        + " | 任务界面线程=" + std::string(布尔文本_是或否(快照.任务管理界面线程已启动))
        + " | 最近巡检=" + std::to_string(快照.任务管理界面线程最近巡检任务数)
        + "/" + std::to_string(快照.任务管理界面线程最近巡检入队任务数)
        + " | 工作线程=" + std::string(布尔文本_是或否(快照.任务管理工作线程已启动))
        + " | 来源=消息中间件/项目线程快照");
    const auto 世界页摘要 = 私有_转义HTML(
        "节点仓库=基础信息类 | 基础节点=" + std::to_string(快照.基础信息节点数)
        + " | 场景=" + std::to_string(快照.场景数)
        + " | 存在=" + std::to_string(快照.存在数)
        + " | 状态=" + std::to_string(快照.状态数)
        + " | 动态=" + std::to_string(快照.动态数)
        + " | 因果模板=" + std::to_string(快照.因果模板数));
    const auto 因果信息摘要 = 私有_转义HTML(
        "因果模板=" + std::to_string(快照.因果模板数)
        + " | 证据动态样本=" + std::to_string(快照.因果证据动态样本数)
        + " | 来源=世界树因果主信息");
    const auto 需求树摘要 = 私有_转义HTML(
        "需求数=" + std::to_string(快照.需求数)
        + " | " + 私有_需求满足数量摘要(快照)
        + " | 有子=" + std::to_string(快照.需求树有子节点数)
        + " | 叶子=" + std::to_string(快照.需求树叶子节点数)
        + " | 活动阻塞=" + std::to_string(快照.需求树活动阻塞需求数));
    const auto 需求列表摘要 = 私有_转义HTML(
        "需求数=" + std::to_string(快照.需求数)
        + " | 列表窗口=" + std::to_string(私有_列表分页大小));
    const auto 任务树摘要 = 私有_转义HTML(
        "任务节点=" + std::to_string(快照.任务数)
            + " | 头=" + std::to_string(快照.任务头节点数)
            + " | 步骤=" + std::to_string(快照.任务步骤节点数)
        + " | 状态={" + 私有_任务状态数量摘要(快照) + "}");
    const auto 方法树摘要 = 私有_转义HTML(
        "方法数=" + std::to_string(快照.方法数)
        + " | " + 私有_方法结构诊断摘要(快照)
        + " | 缺口承接=已接管");
    const auto 自我场景复现菜单标记 = 私有_转义HTML(
        快照.自我场景复现有快照
            ? ("候选 " + std::to_string(快照.自我场景空间候选数量))
            : "OpenGL");
    I64 自我场景窗口像素数量 = 快照.自我场景预期像素数量;
    if (自我场景窗口像素数量 <= 0
        && 快照.自我场景相机帧宽度 > 0
        && 快照.自我场景相机帧高度 > 0) {
        自我场景窗口像素数量 =
            快照.自我场景相机帧宽度 * 快照.自我场景相机帧高度;
    }
    const I64 自我场景已知存在数量 =
        std::max<I64>(
            快照.自我场景直接存在数量,
            std::max<I64>(快照.自我场景子树存在数量, 快照.自我场景已验证观察存在数量));
    const auto 自我场景复现摘要 = 私有_转义HTML(
        "场景=" + 私有_页面摘要(快照.自我所在场景标题)
        + " | 宿主=" + 私有_页面摘要(快照.自我场景复现宿主标题)
        + " | 观察帧=" + std::to_string(快照.自我场景当前观察帧)
        + " | 帧=" + std::to_string(快照.自我场景相机帧宽度)
        + "x" + std::to_string(快照.自我场景相机帧高度)
        + " | 像素特征=" + std::to_string(快照.自我场景像素特征数量)
        + " | 质量=" + std::to_string(快照.自我场景帧质量评分)
        + " | 空间候选=" + std::to_string(快照.自我场景空间候选数量)
        + " | 已验证观察存在=" + std::to_string(快照.自我场景已验证观察存在数量)
        + " | 已知存在=" + std::to_string(自我场景已知存在数量)
        + " | 场景子树存在=" + std::to_string(快照.自我场景子树存在数量)
        + " | 直接存在=" + std::to_string(快照.自我场景直接存在数量)
        + " | 未知区域=" + std::to_string(std::max<I64>(0, 快照.自我场景未解释像素数))
        + "/" + std::to_string(std::max<I64>(0, 自我场景窗口像素数量)) + "像素"
        + " | 基础观察=" + std::to_string(快照.自我场景基础观察事实可用状态)
        + " | 基础风险判断=" + std::to_string(快照.自我场景基础风险判断状态)
        + " | 风险明确=" + std::to_string(快照.自我场景风险状态明确状态)
        + " | 评估安全值=" + std::to_string(快照.自我场景当前场景评估安全值候选)
        + " | 风险安全投影候选=" + std::to_string(快照.自我场景风险安全层投影候选)
        + " | 风险安全层明确=" + std::to_string(快照.自我场景风险安全层明确状态)
        + " | 补观察缺口=" + std::to_string(快照.自我场景补观察缺口状态)
        + " | 待补区域=" + std::to_string(快照.自我场景待补观察区域数量)
        + " | 帧解释=" + std::to_string(快照.自我场景帧解释状态));
    const auto 自我场景存在样例摘要 = 私有_转义HTML(
        "存在样例=" + 私有_页面摘要(快照.自我场景存在样例摘要));
    const auto 参数设定摘要 = 私有_转义HTML(
        "任务管理工作线程池大小="
        + std::to_string(快照.任务管理工作线程池当前有效大小)
        + " | 默认=" + std::to_string(快照.任务管理工作线程池默认大小)
        + " | 待保存=" + std::to_string(快照.任务管理工作线程池待保存大小)
        + " | 范围=" + std::to_string(快照.任务管理工作线程池最小大小)
        + "-" + std::to_string(快照.任务管理工作线程池最大大小)
        + " | 最近保存=" + 私有_页面摘要(私有_任务工作线程池最近参数保存文本(快照))
        + " | 最近参数错误=" + 私有_页面摘要(私有_任务工作线程池最近参数错误文本(快照))
        + " | 最近应用=" + 私有_页面摘要(私有_任务工作线程池最近参数应用文本(快照)));
    const auto 自我卡片值 = 私有_转义HTML(快照.自我存在已建立 ? "已建立" : "未建立");
    const auto 自我卡片说明 = 私有_转义HTML(
        "安全=" + std::to_string(快照.自我安全值)
        + " | 服务=" + std::to_string(快照.自我服务值)
        + " | 风险安全=" + std::to_string(快照.自我风险安全值)
        + " | 待处理方法=" + std::to_string(快照.自我待处理方法数量)
        + " | 可用方法=" + std::to_string(快照.自我可用方法数量));
    const auto 线程卡片值 = 私有_转义HTML(快照.自我线程当前阶段.empty() ? "未启动" : 快照.自我线程当前阶段);
    const auto 线程卡片说明 = 私有_转义HTML(
        std::string("生命周期=")
        + 私有_线程生命周期文本(static_cast<枚举_线程生命周期状态>(快照.自我线程生命周期))
        + " | 去向=" + 快照.自我线程当前最终去向
        + " | Tick=" + std::to_string(快照.自我Tick计数));
    const auto 界面线程卡片值 = 私有_转义HTML(快照.任务管理界面线程已启动 ? "已启动" : "未启动");
    const auto 界面线程卡片说明 = 私有_转义HTML(
        "请求=" + std::to_string(快照.任务管理界面线程累计接收请求数)
        + " | 调度事件=" + std::to_string(快照.任务管理界面线程累计调度事件数)
        + " | 最近巡检=" + std::to_string(快照.任务管理界面线程最近巡检任务数)
        + "/" + std::to_string(快照.任务管理界面线程最近巡检入队任务数)
        + " | 工作项=" + std::to_string(快照.任务管理界面线程累计派发工作项数)
        + " | 待筹办=" + std::to_string(快照.任务管理界面线程当前待筹办工作项数)
        + " | 待执行=" + std::to_string(快照.任务管理界面线程当前待执行工作项数)
        + " | 在途=" + std::to_string(快照.任务管理界面线程当前在途工作项数)
        + " | 任务工作线程池=" + std::string(快照.任务管理界面线程后台worker池运行 ? "运行" : "停止")
        + "/" + std::to_string(快照.任务管理界面线程后台worker线程数)
        + " | 任务工作线程池大小=" + std::to_string(快照.任务管理工作线程池当前有效大小)
        + " | 等待索引=" + std::to_string(快照.任务管理界面线程当前等待任务索引数)
        + " | 挂起索引=" + std::to_string(快照.任务管理界面线程当前挂起任务索引数)
        + " | 死信=" + std::to_string(快照.任务管理界面线程当前死信工作项数)
        + " | 上行=" + std::to_string(快照.任务管理界面线程当前上行队列长度)
        + " | 调度=" + 私有_页面摘要(快照.任务管理界面线程最近调度动作)
        + " | 最近事件=" + 私有_页面摘要(快照.任务管理界面线程最近调度事件类型));
    const auto 工作线程卡片值 = 私有_转义HTML(快照.任务管理工作线程已启动 ? "已启动" : "未启动");
    const auto 工作线程卡片说明 = 私有_转义HTML(
        "执行=" + std::string(布尔文本_是或否(快照.任务管理工作线程正在执行))
        + " | 排队=" + std::to_string(快照.任务管理工作线程当前排队数)
        + " | 推进=" + std::to_string(快照.任务管理工作线程累计推进次数));
    std::ostringstream 输出;
    输出 << R"HTML(<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>鱼巢控制面板</title>
  <style>
    :root{
      --bg:#f4f6f8;
      --surface:#ffffff;
      --surface-2:#f8fafc;
      --line:#d8dee7;
      --ink:#172026;
      --muted:#5d6977;
      --accent:#0f766e;
      --blue:#2563eb;
      --amber:#b45309;
      --danger:#be123c;
      --shadow:0 10px 24px rgba(15,23,42,.08);
    }
    *{box-sizing:border-box}
    body{
      margin:0;
      color:var(--ink);
      font-family:"Microsoft YaHei UI","PingFang SC","Source Han Sans SC",sans-serif;
      background:var(--bg);
    }
    .shell{max-width:1680px;margin:0 auto;padding:24px 20px 36px}
    .layout{
      display:grid;
      grid-template-columns:260px minmax(0,1fr);
      gap:18px;
      align-items:start;
    }
    .rail{
      position:sticky;
      top:16px;
      padding:18px 16px;
      border:1px solid rgba(15,23,42,.08);
      border-radius:8px;
      background:rgba(255,255,255,.94);
      box-shadow:var(--shadow);
      backdrop-filter:blur(10px);
    }
    .brand{font-size:12px;letter-spacing:.1em;text-transform:uppercase;color:var(--muted)}
    .rail h1{margin:10px 0 0;font-size:24px;line-height:1.2}
    .rail p{margin:10px 0 0;color:var(--muted);font-size:13px;line-height:1.6}
    .nav{margin-top:18px;display:grid;gap:8px}
    .menu-item,.toolbar-btn{
      border:none;
      border-radius:8px;
      cursor:pointer;
      transition:transform .12s ease,background .12s ease,color .12s ease;
      -webkit-user-select:none;
      user-select:none;
    }
    .menu-item{
      width:100%;
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:10px;
      padding:11px 12px;
      background:#eef3f7;
      color:var(--ink);
      text-align:left;
    }
    .menu-item.active{
      background:var(--accent);
      color:#fff;
      font-weight:700;
      box-shadow:0 10px 20px rgba(15,118,110,.18);
    }
    .menu-item:hover,.toolbar-btn:hover{transform:translateY(-1px)}
    .menu-badge{
      border-radius:8px;
      padding:3px 9px;
      font-size:12px;
      background:rgba(31,43,47,.08);
      color:var(--muted);
      white-space:nowrap;
    }
    .menu-item.active .menu-badge{
      background:rgba(255,255,255,.22);
      color:#fff;
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
      border:1px solid rgba(15,23,42,.08);
      border-radius:8px;
      background:rgba(255,255,255,.94);
      box-shadow:var(--shadow);
      backdrop-filter:blur(10px);
    }
    .toolbar-kicker{font-size:12px;letter-spacing:.08em;text-transform:uppercase;color:var(--muted)}
    .toolbar-title{margin-top:6px;font-size:30px;font-weight:700;line-height:1.2}
    .toolbar-sub{margin-top:8px;color:var(--muted);font-size:14px;line-height:1.6}
    .toolbar-actions{display:flex;gap:10px}
    .toolbar-btn{
      min-width:120px;
      padding:12px 16px;
      background:var(--accent);
      color:#fff;
      font-weight:700;
    }
    .toolbar-btn.secondary{
      background:#eef3f7;
      border:1px solid rgba(15,118,110,.18);
      color:var(--ink);
    }
    .overview{
      margin-top:18px;
      display:grid;
      grid-template-columns:repeat(4,minmax(0,1fr));
      gap:12px;
    }
    .status-card{
      min-width:0;
      padding:14px;
      border:1px solid rgba(15,23,42,.08);
      border-radius:8px;
      background:var(--surface);
      box-shadow:var(--shadow);
    }
    .status-head{
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:8px;
      color:var(--muted);
      font-size:12px;
    }
    .status-value{margin-top:8px;font-size:22px;font-weight:700;line-height:1.2}
    .status-meta{margin-top:8px;color:var(--muted);font-size:12px;line-height:1.6}
    .status-dot{width:9px;height:9px;border-radius:50%;background:var(--muted);flex:0 0 auto}
    .status-dot.ok{background:var(--accent)}
    .status-dot.warn{background:var(--amber)}
    .status-dot.bad{background:var(--danger)}
    .card,.panel{
      border:1px solid rgba(15,23,42,.08);
      border-radius:8px;
      background:var(--surface);
      box-shadow:var(--shadow);
    }
    .card{padding:14px}
    .label{font-size:12px;letter-spacing:.08em;text-transform:uppercase;color:var(--muted)}
    .value{margin-top:8px;font-size:24px;font-weight:700}
    .sub{margin-top:8px;color:var(--muted);font-size:13px;line-height:1.7}
    .page{display:none;margin-top:18px}
    .page.active{display:block}
    .panel{padding:20px}
    .panel h3{margin:0 0 10px;font-size:22px}
    .panel-topline{
      font-size:12px;
      letter-spacing:.08em;
      text-transform:uppercase;
      color:var(--muted);
    }
    .summary{
      margin-top:12px;
      padding:14px 16px;
      border:1px dashed rgba(15,118,110,.28);
      border-radius:8px;
      background:rgba(15,118,110,.05);
      color:var(--ink);
      font-size:14px;
      line-height:1.8;
      white-space:pre-wrap;
    }
    .settings-layout{
      display:grid;
      grid-template-columns:minmax(0,1fr) minmax(300px,360px);
      gap:18px;
      align-items:start;
    }
    .settings-main,.settings-side{min-width:0}
    .settings-list{
      margin-top:16px;
      display:grid;
      gap:14px;
    }
    .settings-row{
      display:grid;
      grid-template-columns:minmax(0,1fr) minmax(260px,340px);
      gap:16px;
      align-items:center;
      padding:16px;
      border-radius:8px;
      border:1px solid rgba(15,23,42,.08);
      background:var(--surface-2);
    }
    .settings-name{
      font-size:16px;
      font-weight:700;
      line-height:1.5;
      word-break:break-word;
    }
    .settings-meta{
      margin-top:8px;
      color:var(--muted);
      font-size:13px;
      line-height:1.7;
      word-break:break-word;
    }
    .settings-editor{
      display:grid;
      grid-template-columns:minmax(96px,1fr) auto;
      gap:10px;
      align-items:center;
    }
    .settings-number{
      width:100%;
      min-height:44px;
      padding:10px 12px;
      border:1px solid rgba(15,23,42,.16);
      border-radius:8px;
      background:#fff;
      color:var(--ink);
      font-size:16px;
      line-height:1.4;
    }
    .settings-number:focus{
      outline:2px solid rgba(15,118,110,.28);
      outline-offset:2px;
    }
    .settings-save-btn{
      min-height:44px;
      padding:0 16px;
      border:none;
      border-radius:8px;
      background:var(--accent);
      color:#fff;
      cursor:pointer;
      font-weight:700;
      white-space:nowrap;
    }
    .settings-save-btn:hover{transform:translateY(-1px)}
    .settings-status{
      min-height:24px;
      color:var(--muted);
      font-size:13px;
      line-height:1.6;
      word-break:break-word;
    }
    .settings-status.ok{color:var(--accent);font-weight:700}
    .settings-status.error{color:var(--danger);font-weight:700}
    .settings-result-grid{
      display:grid;
      grid-template-columns:repeat(3,minmax(0,1fr));
      gap:10px;
    }
    .settings-result,.settings-slot{
      min-width:0;
      padding:13px 14px;
      border-radius:8px;
      border:1px solid rgba(15,23,42,.08);
      background:var(--surface);
    }
    .settings-result-label,.settings-side-label{
      color:var(--muted);
      font-size:12px;
      line-height:1.5;
    }
    .settings-result-value{
      margin-top:7px;
      font-size:14px;
      line-height:1.7;
      word-break:break-word;
    }
    .settings-side-list{
      margin-top:16px;
      display:grid;
      gap:10px;
    }
    .settings-side-value{
      margin-top:7px;
      font-size:15px;
      font-weight:700;
      line-height:1.6;
      word-break:break-word;
    }
    .settings-hidden-tree[hidden]{display:none}
)HTML";
    输出 << R"HTML(
    .camera-control{
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:16px;
      flex-wrap:wrap;
      margin-bottom:18px;
    }
    .camera-actions{
      display:flex;
      gap:10px;
      flex-wrap:wrap;
    }
    .camera-btn{
      min-height:44px;
      padding:0 16px;
      border:none;
      border-radius:8px;
      background:var(--accent);
      color:#fff;
      cursor:pointer;
      font-weight:700;
      white-space:nowrap;
    }
    .camera-btn.secondary{
      background:#eef3f7;
      border:1px solid rgba(15,118,110,.18);
      color:var(--ink);
    }
    .camera-btn.danger{
      background:var(--danger);
    }
    .camera-status{
      color:var(--muted);
      font-size:13px;
      line-height:1.7;
      word-break:break-word;
    }
    .camera-status.ok{color:var(--accent);font-weight:700}
    .camera-status.error{color:var(--danger);font-weight:700}
    .scene-control{
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:16px;
      flex-wrap:wrap;
      margin-bottom:18px;
    }
    .scene-status{
      color:var(--muted);
      font-size:13px;
      line-height:1.7;
      word-break:break-word;
    }
    .scene-status.ok{color:var(--accent);font-weight:700}
    .scene-status.error{color:var(--danger);font-weight:700}
    .scene-actions{
      display:flex;
      gap:10px;
      flex-wrap:wrap;
    }
    .scene-btn{
      min-height:44px;
      padding:0 16px;
      border:none;
      border-radius:8px;
      background:var(--accent);
      color:#fff;
      cursor:pointer;
      font-weight:700;
      white-space:nowrap;
    }
    .scene-btn.secondary{
      background:#eef3f7;
      border:1px solid rgba(15,118,110,.18);
      color:var(--ink);
    }
    .scene-layout{
      display:grid;
      grid-template-columns:minmax(0,1.4fr) minmax(320px,.6fr);
      gap:18px;
      align-items:start;
    }
    .scene-side{
      min-width:0;
      display:grid;
      gap:18px;
    }
    .scene-viewer{min-width:0}
    .scene-canvas-shell{
      position:relative;
      aspect-ratio:16/9;
      min-height:360px;
      border-radius:8px;
      border:1px solid rgba(15,23,42,.12);
      background:#080d12;
      overflow:hidden;
    }
    .scene-canvas{
      width:100%;
      height:100%;
      display:block;
      background:#080d12;
    }
    .scene-layer-overlay{
      position:absolute;
      inset:0;
      width:100%;
      height:100%;
      pointer-events:none;
    }
    .scene-legend{
      margin-top:14px;
      display:flex;
      flex-wrap:wrap;
      gap:10px;
      color:var(--muted);
      font-size:12px;
      line-height:1.5;
    }
    .scene-legend span{
      display:inline-flex;
      align-items:center;
      gap:6px;
    }
    .scene-legend i{
      width:10px;
      height:10px;
      border-radius:50%;
      display:inline-block;
      background:var(--accent);
    }
    .scene-legend .x i{background:#f87171}
    .scene-legend .y i{background:#34d399}
    .scene-legend .z i{background:#60a5fa}
    .scene-legend .box i{background:#f59e0b}
    .scene-legend .depth-hole i{background:#ef4444}
    .scene-legend .filled-depth i{background:#facc15}
    .scene-legend .unexplained i{background:#a78bfa}
    .scene-legend .contour-risk i{background:#fb7185}
    .scene-stat-grid{
      display:grid;
      grid-template-columns:repeat(2,minmax(0,1fr));
      gap:10px;
    }
    .scene-stat{
      min-width:0;
      padding:12px 14px;
      border-radius:8px;
      border:1px solid rgba(15,23,42,.08);
      background:var(--surface-2);
    }
    .scene-stat-label{
      color:var(--muted);
      font-size:12px;
      line-height:1.5;
    }
    .scene-stat-value{
      margin-top:6px;
      font-size:15px;
      font-weight:800;
      line-height:1.5;
      word-break:break-word;
    }
    .scene-diagnostic-grid{
      display:grid;
      grid-template-columns:repeat(2,minmax(0,1fr));
      gap:10px;
    }
    .scene-diagnostic-row{
      min-width:0;
      padding:10px 12px;
      border-radius:8px;
      border:1px solid rgba(15,23,42,.08);
      background:var(--surface-2);
    }
    .scene-diagnostic-label{
      color:var(--muted);
      font-size:12px;
      line-height:1.5;
    }
    .scene-diagnostic-value{
      margin-top:6px;
      font-size:14px;
      font-weight:800;
      line-height:1.55;
      word-break:break-word;
    }
    .scene-layer-controls{
      display:grid;
      grid-template-columns:repeat(2,minmax(0,1fr));
      gap:10px;
    }
    .scene-layer-toggle{
      min-width:0;
      display:flex;
      align-items:center;
      gap:8px;
      padding:10px 12px;
      border-radius:8px;
      border:1px solid rgba(15,23,42,.08);
      background:var(--surface-2);
      color:var(--ink);
      font-size:13px;
      font-weight:700;
      line-height:1.45;
    }
    .scene-layer-toggle input{accent-color:var(--accent)}
    .scene-layer-status{
      margin-top:12px;
      color:var(--muted);
      font-size:12px;
      line-height:1.6;
      word-break:break-word;
    }
    .scene-layer-grid{
      margin-top:12px;
      display:grid;
      gap:10px;
    }
    .scene-layer-row{
      min-width:0;
      padding:10px 12px;
      border-radius:8px;
      border:1px solid rgba(15,23,42,.08);
      background:var(--surface-2);
    }
    .scene-layer-head{
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:8px;
      color:var(--muted);
      font-size:12px;
      line-height:1.5;
    }
    .scene-layer-value{
      color:var(--ink);
      font-weight:800;
      word-break:break-word;
    }
    .scene-layer-meter{
      margin-top:8px;
      height:7px;
      border-radius:999px;
      overflow:hidden;
      background:rgba(15,23,42,.08);
    }
    .scene-layer-meter span{
      display:block;
      width:0%;
      height:100%;
      border-radius:999px;
      background:var(--accent);
    }
    .scene-layer-row.depth-hole .scene-layer-meter span{background:#ef4444}
    .scene-layer-row.filled-depth .scene-layer-meter span{background:#facc15}
    .scene-layer-row.unexplained .scene-layer-meter span{background:#a78bfa}
    .scene-layer-row.contour-risk .scene-layer-meter span{background:#fb7185}
)HTML";
    输出 << R"HTML(
    .workspace{
      display:grid;
      grid-template-columns:minmax(0,1.2fr) minmax(320px,420px);
      gap:18px;
      align-items:start;
    }
    .tree-panel,.detail-panel{
      min-width:0;
      height:100%;
    }
    .tree-toolbar{
      margin-top:14px;
      display:flex;
      align-items:flex-start;
      justify-content:space-between;
      gap:12px;
      flex-wrap:wrap;
    }
    .tree-hint{
      color:var(--muted);
      font-size:13px;
      line-height:1.7;
    }
    .tree-shell{
      margin-top:14px;
      padding:18px;
      border-radius:8px;
      background:var(--surface-2);
      border:1px solid rgba(15,23,42,.08);
      min-height:440px;
      max-height:calc(100vh - 240px);
      overflow:auto;
    }
    .tree-root,.tree-root ul{list-style:none;margin:0;padding-left:20px}
    .tree-root{padding-left:0}
    .tree-row{
      position:relative;
      display:flex;
      align-items:flex-start;
      gap:10px;
      margin:4px 0;
      padding:8px 10px 8px 26px;
      border-radius:8px;
      line-height:1.6;
      cursor:default;
      user-select:text;
      transition:background .12s ease, box-shadow .12s ease;
    }
    .tree-row:hover{background:rgba(15,118,110,.05)}
    .tree-row.selected{
      background:rgba(15,118,110,.14);
      box-shadow:inset 0 0 0 1px rgba(15,118,110,.18);
    }
    .tree-row.branch{cursor:pointer}
    .tree-row.branch::before{
      content:"▸";
      position:absolute;
      left:8px;
      top:8px;
      color:var(--accent);
      transition:transform .12s ease;
    }
    .tree-row.branch.open::before{transform:rotate(90deg)}
    .tree-row.loading{opacity:.65}
    .tree-row-text{min-width:0;overflow-wrap:anywhere}
    .task-state-pill{
      flex:0 0 auto;
      padding:2px 8px;
      border-radius:8px;
      font-size:12px;
      line-height:1.5;
      font-weight:700;
      background:#eef3f7;
      color:var(--ink);
      border:1px solid rgba(15,23,42,.08);
    }
    .task-state-pill.running{
      background:#e8f7ef;
      color:#166534;
      border-color:rgba(22,101,52,.18);
    }
    .tree-row.task-state-running{
      background:rgba(22,101,52,.06);
      box-shadow:inset 3px 0 0 rgba(22,101,52,.72);
    }
    .tree-row.task-state-running.selected{
      background:rgba(22,101,52,.12);
      box-shadow:inset 3px 0 0 rgba(22,101,52,.82), inset 0 0 0 1px rgba(22,101,52,.16);
    }
    .tree-children[hidden]{display:none}
    .detail-panel{
      position:sticky;
      top:110px;
    }
    .detail-kicker{
      font-size:12px;
      letter-spacing:.08em;
      text-transform:uppercase;
      color:var(--muted);
    }
    .detail-title{
      margin-top:8px;
      font-size:24px;
      font-weight:700;
      line-height:1.4;
      word-break:break-word;
    }
    .detail-note{
      margin-top:10px;
      color:var(--muted);
      font-size:13px;
      line-height:1.7;
    }
    .breadcrumbs{
      margin-top:14px;
      display:flex;
      flex-wrap:wrap;
      gap:8px;
    }
    .breadcrumb{
      border:none;
      border-radius:8px;
      padding:6px 12px;
      background:#eef3f7;
      color:var(--ink);
      font-size:12px;
      cursor:pointer;
    }
    .breadcrumb.current{
      background:var(--accent);
      color:#fff;
      cursor:default;
    }
    .chips{
      margin-top:14px;
      display:flex;
      flex-wrap:wrap;
      gap:8px;
    }
    .chip{
      padding:5px 10px;
      border-radius:8px;
      background:#eef3f7;
      color:var(--ink);
      font-size:12px;
      line-height:1.4;
    }
    .detail-grid{
      margin-top:18px;
      display:grid;
      grid-template-columns:108px minmax(0,1fr);
      gap:12px 14px;
    }
    .detail-key{
      color:var(--muted);
      font-size:12px;
      letter-spacing:.08em;
      text-transform:uppercase;
    }
    .detail-value{
      font-size:14px;
      line-height:1.7;
      word-break:break-word;
    }
    .detail-actions{
      margin-top:18px;
      display:flex;
      gap:10px;
      flex-wrap:wrap;
    }
    .detail-btn{
      border:none;
      border-radius:8px;
      padding:11px 14px;
      cursor:pointer;
      font-weight:700;
      background:var(--accent);
      color:#fff;
    }
    .detail-btn.secondary{
      background:#eef3f7;
      border:1px solid rgba(15,118,110,.18);
      color:var(--ink);
    }
    .detail-list{
      margin-top:16px;
      display:grid;
      gap:10px;
    }
    .detail-tree{
      margin-top:18px;
      display:grid;
      gap:12px;
    }
    .detail-node{
      border:1px solid rgba(15,23,42,.08);
      border-radius:8px;
      background:var(--surface);
      padding:12px 14px;
    }
    .detail-group-head,.detail-field-row{
      display:grid;
      gap:10px;
      align-items:start;
    }
    .detail-group-head{
      grid-template-columns:minmax(0,1fr) auto;
    }
    .detail-group-title{
      font-size:14px;
      font-weight:700;
      line-height:1.6;
      word-break:break-word;
    }
    .detail-group-meta{
      margin-top:8px;
      color:var(--muted);
      font-size:12px;
      line-height:1.7;
      word-break:break-word;
    }
    .detail-field-row{
      grid-template-columns:minmax(120px,1fr) minmax(80px,.6fr) minmax(0,1.6fr) auto;
    }
    .detail-field-name{
      font-weight:700;
      line-height:1.6;
      word-break:break-word;
    }
    .detail-field-type{
      color:var(--muted);
      font-size:12px;
      line-height:1.8;
      word-break:break-word;
    }
    .detail-field-value{
      font-size:13px;
      line-height:1.7;
      word-break:break-word;
    }
    .detail-field-row.pointer-field{
      cursor:pointer;
    }
    .detail-field-row.pointer-field .detail-field-value{
      color:var(--accent);
      font-weight:700;
    }
    .detail-inline-actions{
      display:flex;
      justify-content:flex-end;
    }
    .detail-mini-btn{
      border:none;
      border-radius:8px;
      padding:7px 10px;
      cursor:pointer;
      font-size:12px;
      font-weight:700;
      background:rgba(15,118,110,.1);
      color:var(--ink);
    }
    .detail-mini-btn:disabled{
      cursor:default;
      opacity:.65;
    }
    .detail-node-children{
      margin-top:10px;
      padding-left:14px;
      border-left:2px solid rgba(15,118,110,.14);
      display:grid;
      gap:10px;
    }
    .detail-item{
      padding:12px 14px;
      border-radius:8px;
      background:var(--surface);
      border:1px solid rgba(15,23,42,.08);
      font-size:13px;
      line-height:1.7;
      word-break:break-word;
    }
    .detail-raw{
      margin-top:18px;
      padding:14px 16px;
      border:1px dashed rgba(15,118,110,.28);
      border-radius:8px;
      background:rgba(15,118,110,.05);
      font-size:13px;
      line-height:1.8;
      white-space:pre-wrap;
      word-break:break-word;
    }
    .pointer-modal-backdrop{
      position:fixed;
      inset:0;
      z-index:50;
      display:none;
      align-items:center;
      justify-content:center;
      padding:28px;
      background:rgba(15,23,42,.36);
    }
    .pointer-modal-backdrop.show{
      display:flex;
    }
    .pointer-modal{
      width:min(760px,100%);
      max-height:min(82vh,760px);
      overflow:auto;
      border-radius:8px;
      border:1px solid rgba(15,23,42,.12);
      background:#fff;
      box-shadow:0 24px 80px rgba(15,23,42,.24);
      padding:18px;
    }
    .pointer-modal-head{
      display:flex;
      align-items:flex-start;
      justify-content:space-between;
      gap:16px;
      margin-bottom:14px;
    }
    .pointer-modal-title{
      margin-top:4px;
      font-size:18px;
      font-weight:800;
      line-height:1.4;
      color:var(--ink);
      word-break:break-word;
    }
    .pointer-modal-close{
      border:none;
      border-radius:8px;
      padding:9px 12px;
      cursor:pointer;
      font-weight:700;
      background:#eef3f7;
      color:var(--ink);
    }
    .pointer-modal-body{
      display:grid;
      gap:12px;
    }
    .detail-empty{
      margin-top:14px;
      color:var(--muted);
      font-size:14px;
      line-height:1.8;
    }
    .footer{
      margin-top:18px;
      color:var(--muted);
      font-size:13px;
      text-align:right;
    }
    @media (max-width:1350px){
      .overview{grid-template-columns:repeat(2,minmax(0,1fr))}
      .workspace{grid-template-columns:1fr}
      .scene-layout{grid-template-columns:1fr}
      .settings-layout{grid-template-columns:1fr}
      .detail-panel{position:static}
      .tree-shell{max-height:none}
    }
    @media (max-width:1100px){
      .layout{grid-template-columns:1fr}
      .rail,.toolbar{position:static}
    }
    @media (max-width:720px){
      .overview{grid-template-columns:1fr}
      .toolbar{flex-direction:column;align-items:flex-start}
      .toolbar-actions{width:100%}
      .toolbar-btn{width:100%}
      .detail-grid{grid-template-columns:1fr}
      .detail-field-row{grid-template-columns:1fr}
      .detail-inline-actions{justify-content:flex-start}
      .settings-row{grid-template-columns:1fr}
      .settings-editor{grid-template-columns:minmax(0,1fr) auto}
      .settings-result-grid{grid-template-columns:1fr}
      .camera-control{align-items:flex-start}
      .camera-actions{width:100%}
      .camera-btn{flex:1 1 140px}
      .scene-control{align-items:flex-start}
      .scene-actions{width:100%}
      .scene-btn{flex:1 1 140px}
      .scene-stat-grid{grid-template-columns:1fr}
      .scene-diagnostic-grid{grid-template-columns:1fr}
      .scene-layer-controls{grid-template-columns:1fr}
      .scene-canvas-shell{min-height:280px}
    }
  </style>
</head>)HTML";
    输出 << R"HTML(<body>
)HTML";
    if (自我场景窗口模式) {
        输出 << R"HTML(
  <style>
    .shell{max-width:none;padding:16px}
    .layout{display:block}
    .rail,.toolbar,.overview,.footer{display:none!important}
    .content{min-width:0}
    .page{display:none;margin-top:0}
    .page[data-page="self-scene"]{display:block}
    .scene-window-control{display:none}
    .scene-layout{grid-template-columns:minmax(0,1fr) minmax(340px,460px)}
    .scene-canvas-shell{height:calc(100vh - 230px);min-height:460px;aspect-ratio:auto}
    @media (max-width:1180px){
      .scene-layout{grid-template-columns:1fr}
      .scene-canvas-shell{height:58vh;min-height:360px}
    }
  </style>
)HTML";
    }
    else {
        输出 << R"HTML(
  <style>
    .page[data-page="self-scene"] .scene-control,
    .page[data-page="self-scene"] .scene-layout{display:none}
  </style>
)HTML";
    }
    输出 << R"HTML(
  <div class="shell">
    <div class="layout">
      <aside class="rail">
        <div class="brand">WebView2 控制面板</div>
        <h1>鱼巢</h1>
        <p>需求、任务、方法和线程运行总览。</p>
        <nav class="nav" aria-label="控制面板菜单">
          <button class="menu-item active" type="button" data-page="thread-status"><span>线程状态</span><span class="menu-badge">)HTML"
        << 线程徽标数量
        << R"HTML(</span></button>
          <button class="menu-item" type="button" data-page="world-tree"><span>世界树</span><span class="menu-badge">)HTML"
        << 快照.基础信息节点数
        << R"HTML(</span></button>
          <button class="menu-item" type="button" data-page="causal-info"><span>因果信息</span><span class="menu-badge">)HTML"
        << 快照.因果模板数
        << R"HTML(</span></button>
          <button class="menu-item" type="button" data-page="need-tree"><span>需求树</span><span class="menu-badge">)HTML"
        << 快照.需求数
        << R"HTML(</span></button>
          <button class="menu-item" type="button" data-page="need-list"><span>需求列表</span><span class="menu-badge">)HTML"
        << 快照.需求数
        << R"HTML(</span></button>
          <button class="menu-item" type="button" data-page="task-tree"><span>任务树</span><span class="menu-badge">)HTML"
        << 快照.任务数
        << R"HTML(</span></button>
          <button class="menu-item" type="button" data-page="method-tree"><span>方法树</span><span class="menu-badge">)HTML"
        << 快照.方法数
        << R"HTML(</span></button>
          <button class="menu-item" type="button" data-page="self-scene"><span>自我所在场景</span><span class="menu-badge">)HTML"
        << 自我场景复现菜单标记
        << R"HTML(</span></button>
          <button class="menu-item" type="button" data-page="camera-view"><span>相机画面</span><span class="menu-badge">D455</span></button>
          <button class="menu-item" type="button" data-page="settings"><span>参数设定</span><span class="menu-badge">)HTML"
        << 快照.任务管理工作线程池当前有效大小
        << R"HTML(</span></button>
        </nav>
      </aside>
      <main class="content">
        <div class="toolbar">
          <div>
            <div class="toolbar-kicker">控制面板</div>
            <div class="toolbar-title" id="page-title">线程状态</div>
            <div class="toolbar-sub" id="page-subtitle">自我线程、任务管理界面线程、任务管理工作线程和进程内线程状态。</div>
          </div>
          <div class="toolbar-actions">
            <button class="toolbar-btn secondary" type="button" id="copy-page">复制当前页</button>
          </div>
        </div>

        <section class="overview">
          <div class="status-card">
            <div class="status-head"><span>自我</span><span class="status-dot )HTML"
        << (快照.自我存在已建立 ? "ok" : "warn")
        << R"HTML("></span></div>
            <div class="status-value">)HTML"
        << 自我卡片值
        << R"HTML(</div>
            <div class="status-meta">)HTML"
        << 自我卡片说明
        << R"HTML(</div>
          </div>
          <div class="status-card">
            <div class="status-head"><span>自我线程</span><span class="status-dot )HTML"
        << (快照.自我线程健康运行 ? "ok" : "warn")
        << R"HTML("></span></div>
            <div class="status-value">)HTML"
        << 线程卡片值
        << R"HTML(</div>
            <div class="status-meta">)HTML"
        << 线程卡片说明
        << R"HTML(</div>
          </div>
          <div class="status-card">
            <div class="status-head"><span>任务界面线程</span><span class="status-dot )HTML"
        << (快照.任务管理界面线程已启动 && !快照.任务管理界面线程故障 ? "ok" : "warn")
        << R"HTML("></span></div>
            <div class="status-value">)HTML"
        << 界面线程卡片值
        << R"HTML(</div>
            <div class="status-meta">)HTML"
        << 界面线程卡片说明
        << R"HTML(</div>
          </div>
          <div class="status-card">
            <div class="status-head"><span>任务工作线程</span><span class="status-dot )HTML"
        << (快照.任务管理工作线程已启动 ? "ok" : "warn")
        << R"HTML("></span></div>
            <div class="status-value">)HTML"
        << 工作线程卡片值
        << R"HTML(</div>
            <div class="status-meta">)HTML"
        << 工作线程卡片说明
        << R"HTML(</div>
          </div>
        </section>

        <section class="page active" data-page="thread-status" data-title="线程状态" data-subtitle="上部显示项目线程统计，中间栏显示完整项目线程列表，右侧栏显示选中线程详情。">
          <div class="workspace">
            <section class="panel tree-panel">
              <div class="panel-topline">线程统计</div>
              <h3>项目线程列表</h3>
              <div class="summary">)HTML"
        << 线程页摘要
        << R"HTML(</div>
              <div class="tree-toolbar"><div class="tree-hint">)HTML"
        << 线程树交互提示
        << R"HTML(</div></div>
              <div class="tree-shell"><div id="tree-thread-status"></div></div>
            </section>
            <aside class="panel detail-panel"><div id="detail-thread-status" class="detail-host"></div></aside>
          </div>
        </section>

        <section class="page" data-page="world-tree" data-title="世界树" data-subtitle="基础信息类仓库中的世界结构；树节点统一是基础信息节点类，主信息承载派生类型。">
          <div class="workspace">
            <section class="panel tree-panel">
              <div class="panel-topline">原始树视图</div>
              <h3>世界树</h3>
              <div class="summary">)HTML"
        << 世界页摘要
        << R"HTML(</div>
              <div class="tree-toolbar"><div class="tree-hint">)HTML"
        << 树交互提示
        << R"HTML(</div></div>
              <div class="tree-shell"><div id="tree-world-tree"></div></div>
            </section>
            <aside class="panel detail-panel"><div id="detail-world-tree" class="detail-host"></div></aside>
          </div>
        </section>

        <section class="page" data-page="causal-info" data-title="因果信息" data-subtitle="当前世界树中记录的因果模板和证据动态样本。">
          <div class="workspace">
            <section class="panel tree-panel">
              <div class="panel-topline">因果账本</div>
              <h3>因果信息</h3>
              <div class="summary">)HTML"
        << 因果信息摘要
        << R"HTML(</div>
              <div class="tree-toolbar"><div class="tree-hint">)HTML"
        << 树交互提示
        << R"HTML(</div></div>
              <div class="tree-shell"><div id="tree-causal-info"></div></div>
            </section>
            <aside class="panel detail-panel"><div id="detail-causal-info" class="detail-host"></div></aside>
          </div>
        </section>

        <section class="page" data-page="need-tree" data-title="需求树" data-subtitle="真实父子关系、树形结构、目标状态与任务绑定。">
          <div class="workspace">
            <section class="panel tree-panel">
              <div class="panel-topline">原始树视图</div>
              <h3>需求树</h3>
              <div class="summary">)HTML"
        << 需求树摘要
        << R"HTML(</div>
              <div class="tree-toolbar"><div class="tree-hint">)HTML"
        << 树交互提示
        << R"HTML(</div></div>
              <div class="tree-shell"><div id="tree-need-tree"></div></div>
            </section>
            <aside class="panel detail-panel"><div id="detail-need-tree" class="detail-host"></div></aside>
          </div>
        </section>

        <section class="page" data-page="need-list" data-title="需求列表" data-subtitle="按前序顺序平铺需求列表；列表窗口 100 项。">
          <div class="workspace">
            <section class="panel tree-panel">
              <div class="panel-topline">原始树视图</div>
              <h3>需求列表</h3>
              <div class="summary">)HTML"
        << 需求列表摘要
        << R"HTML(</div>
              <div class="tree-toolbar"><div class="tree-hint">)HTML"
        << 树交互提示
        << R"HTML(</div></div>
              <div class="tree-shell"><div id="tree-need-list"></div></div>
            </section>
            <aside class="panel detail-panel"><div id="detail-need-list" class="detail-host"></div></aside>
          </div>
        </section>

        <section class="page" data-page="task-tree" data-title="任务列表" data-subtitle="每一条任务是一棵树；中间栏只看结构，字段在右侧显示。">
          <div class="workspace">
            <section class="panel tree-panel">
              <div class="panel-topline">任务列表</div>
              <h3>任务列表</h3>
              <div class="summary">)HTML"
        << 任务树摘要
        << R"HTML(</div>
              <div class="tree-toolbar"><div class="tree-hint">)HTML"
        << 任务树交互提示
        << R"HTML(</div></div>
              <div class="tree-shell"><div id="tree-task-tree"></div></div>
            </section>
            <aside class="panel detail-panel"><div id="detail-task-tree" class="detail-host"></div></aside>
          </div>
        </section>

        <section class="page" data-page="method-tree" data-title="方法树" data-subtitle="真实方法树关系、方法首节点、条件节点与结果节点。">
          <div class="workspace">
            <section class="panel tree-panel">
              <div class="panel-topline">原始树视图</div>
              <h3>方法树</h3>
              <div class="summary">)HTML"
        << 方法树摘要
        << R"HTML(</div>
              <div class="tree-toolbar"><div class="tree-hint">)HTML"
        << 树交互提示
        << R"HTML(</div></div>
              <div class="tree-shell"><div id="tree-method-tree"></div></div>
            </section>
            <aside class="panel detail-panel"><div id="detail-method-tree" class="detail-host"></div></aside>
          </div>
        </section>

        <section class="page" data-page="self-scene" data-title="自我所在场景" data-subtitle="启动独立自我场景窗口；控制面板不承载场景渲染。">
          <section class="panel scene-window-control">
            <div>
              <div class="panel-topline">OpenGL</div>
              <h3>独立自我场景窗口</h3>
              <div class="summary">)HTML"
        << 自我场景复现摘要
        << R"HTML(</div>
              <div class="summary">)HTML"
        << 自我场景存在样例摘要
        << R"HTML(</div>
              <div id="self-scene-window-status" class="scene-status" role="status">点击按钮启动独立场景窗口。</div>
            </div>
            <div class="scene-actions">
              <button id="self-scene-open-window" class="scene-btn" type="button">打开独立窗口</button>
            </div>
          </section>
          <section class="panel scene-control">
            <div>
              <div class="panel-topline">OpenGL</div>
              <h3>自我所在场景</h3>
              <div class="summary">)HTML"
        << 自我场景复现摘要
        << R"HTML(</div>
              <div id="self-scene-status" class="scene-status" role="status">等待渲染</div>
            </div>
            <div class="scene-actions">
              <button id="scene-reset-view" class="scene-btn secondary" type="button">重置视角</button>
              <button id="scene-refresh-window" class="scene-btn secondary" type="button">刷新快照</button>
              <button id="scene-toggle-rotation" class="scene-btn" type="button">暂停旋转</button>
            </div>
          </section>
          <div class="scene-layout">
            <section class="panel scene-viewer">
              <div class="scene-canvas-shell">
                <canvas id="self-scene-gl" class="scene-canvas" width="960" height="540"></canvas>
                <canvas id="self-scene-layer-overlay" class="scene-layer-overlay" width="960" height="540"></canvas>
              </div>
              <div class="scene-legend" aria-label="场景复现图例">
                <span class="x"><i></i>X</span>
                <span class="y"><i></i>Y</span>
                <span class="z"><i></i>Z</span>
                <span class="box"><i></i>AABB</span>
                <span class="depth-hole"><i></i>深度空洞</span>
                <span class="filled-depth"><i></i>补全深度</span>
                <span class="unexplained"><i></i>未解释</span>
                <span class="contour-risk"><i></i>轮廓风险</span>
              </div>
            </section>
            <div class="scene-side">
              <aside class="panel">
                <div class="panel-topline">场景快照</div>
                <h3>空间解释</h3>
                <div class="scene-stat-grid" aria-label="自我所在场景复现状态">
                  <div class="scene-stat">
                    <div class="scene-stat-label">帧尺寸</div>
                    <div id="scene-frame-size-stat" class="scene-stat-value">--</div>
                  </div>
                  <div class="scene-stat">
                    <div class="scene-stat-label">帧号</div>
                    <div id="scene-frame-stat" class="scene-stat-value">--</div>
                  </div>
                  <div class="scene-stat">
                    <div class="scene-stat-label">像素特征</div>
                    <div id="scene-pixel-stat" class="scene-stat-value">--</div>
                  </div>
                  <div class="scene-stat">
                    <div class="scene-stat-label">空间候选</div>
                    <div id="scene-candidate-stat" class="scene-stat-value">--</div>
                  </div>
                  <div class="scene-stat">
                    <div class="scene-stat-label">场景存在</div>
                    <div id="scene-known-existence-stat" class="scene-stat-value">--</div>
                  </div>
                  <div class="scene-stat">
                    <div class="scene-stat-label">未知区域占比</div>
                    <div id="scene-unknown-region-stat" class="scene-stat-value">--</div>
                  </div>
                  <div class="scene-stat">
                    <div class="scene-stat-label">中心坐标</div>
                    <div id="scene-center-stat" class="scene-stat-value">--</div>
                  </div>
                  <div class="scene-stat">
                    <div class="scene-stat-label">范围 AABB</div>
                    <div id="scene-aabb-stat" class="scene-stat-value">--</div>
                  </div>
                  <div class="scene-stat">
                    <div class="scene-stat-label">连续 / 稳定</div>
                    <div id="scene-score-stat" class="scene-stat-value">--</div>
                  </div>
                  <div class="scene-stat">
                    <div class="scene-stat-label">验证状态</div>
                    <div id="scene-verify-stat" class="scene-stat-value">--</div>
                  </div>
                </div>
              </aside>
              <aside class="panel">
                <div class="panel-topline">场景内容</div>
                <h3>自我场景信息</h3>
                <div class="scene-diagnostic-grid" aria-label="自我所在场景内容摘要">
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">场景节点</div>
                    <div id="scene-info-root-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">复现宿主</div>
                    <div id="scene-info-host-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">直接子层</div>
                    <div id="scene-info-direct-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">子树总量</div>
                    <div id="scene-info-subtree-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">直接类型</div>
                    <div id="scene-info-direct-summary-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">子树类型</div>
                    <div id="scene-info-subtree-summary-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">存在样例</div>
                    <div id="scene-info-existence-samples-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">宿主存在样例</div>
                    <div id="scene-info-host-existence-samples-stat" class="scene-diagnostic-value">--</div>
                  </div>
                </div>
              </aside>
              <aside class="panel">
                <div class="panel-topline">区域图层</div>
                <h3>诊断区域</h3>
                <div class="scene-layer-controls" aria-label="诊断区域图层开关">
                  <label class="scene-layer-toggle"><input id="scene-layer-depth-hole" type="checkbox" checked>深度空洞</label>
                  <label class="scene-layer-toggle"><input id="scene-layer-filled-depth" type="checkbox" checked>补全深度</label>
                  <label class="scene-layer-toggle"><input id="scene-layer-unexplained" type="checkbox" checked>未解释</label>
                  <label class="scene-layer-toggle"><input id="scene-layer-contour-risk" type="checkbox" checked>轮廓风险</label>
                </div>
                <div id="scene-layer-status" class="scene-layer-status">等待图层数据</div>
                <div class="scene-layer-grid" aria-label="诊断区域图层摘要">
                  <div class="scene-layer-row depth-hole">
                    <div class="scene-layer-head"><span>深度空洞区域</span><span id="scene-layer-depth-hole-value" class="scene-layer-value">--</span></div>
                    <div class="scene-layer-meter"><span id="scene-layer-depth-hole-meter"></span></div>
                  </div>
                  <div class="scene-layer-row filled-depth">
                    <div class="scene-layer-head"><span>补全深度区域</span><span id="scene-layer-filled-depth-value" class="scene-layer-value">--</span></div>
                    <div class="scene-layer-meter"><span id="scene-layer-filled-depth-meter"></span></div>
                  </div>
                  <div class="scene-layer-row unexplained">
                    <div class="scene-layer-head"><span>未解释区域</span><span id="scene-layer-unexplained-value" class="scene-layer-value">--</span></div>
                    <div class="scene-layer-meter"><span id="scene-layer-unexplained-meter"></span></div>
                  </div>
                  <div class="scene-layer-row contour-risk">
                    <div class="scene-layer-head"><span>轮廓风险区域</span><span id="scene-layer-contour-risk-value" class="scene-layer-value">--</span></div>
                    <div class="scene-layer-meter"><span id="scene-layer-contour-risk-meter"></span></div>
                  </div>
                </div>
              </aside>
              <aside class="panel">
                <div class="panel-topline">诊断摘要</div>
                <h3>观察质量</h3>
                <div class="scene-diagnostic-grid" aria-label="观察质量诊断摘要">
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">结构状态</div>
                    <div id="scene-quality-structure-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">质量比例</div>
                    <div id="scene-quality-ratio-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">深度来源</div>
                    <div id="scene-depth-source-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">深度稳定</div>
                    <div id="scene-depth-stability-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">多帧融合</div>
                    <div id="scene-fusion-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">轮廓证据</div>
                    <div id="scene-contour-layer-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">轮廓风险</div>
                    <div id="scene-contour-risk-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">补观察缺口</div>
                    <div id="scene-gap-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">补偿候选</div>
                    <div id="scene-gap-candidate-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">安全评估宿主</div>
                    <div id="scene-safety-host-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">基础风险判断</div>
                    <div id="scene-basic-risk-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">场景安全值</div>
                    <div id="scene-safety-value-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">风险安全投影</div>
                    <div id="scene-risk-safety-projection-stat" class="scene-diagnostic-value">--</div>
                  </div>
                  <div class="scene-diagnostic-row">
                    <div class="scene-diagnostic-label">未明确原因</div>
                    <div id="scene-safety-gap-stat" class="scene-diagnostic-value">--</div>
                  </div>
                </div>
              </aside>
            </div>
          </div>
        </section>

        <section class="page" data-page="camera-view" data-title="相机画面" data-subtitle="启动独立相机播放窗口；控制面板不承载视频播放。">
          <section class="panel camera-control">
            <div>
              <div class="panel-topline">D455</div>
              <h3>独立相机播放窗口</h3>
              <div id="camera-status" class="camera-status" role="status">点击按钮启动独立相机播放窗口。</div>
            </div>
            <div class="camera-actions">
              <button id="camera-open-window" class="camera-btn" type="button">打开独立窗口</button>
            </div>
          </section>
        </section>

        <section class="page" data-page="settings" data-title="参数设定" data-subtitle="运行配置只影响调度资源，不写入需求、任务或动作动态。">
          <div class="settings-layout">
            <section class="panel settings-main">
              <div class="panel-topline">运行参数</div>
              <h3>参数设定</h3>
              <div class="summary">)HTML"
        << 参数设定摘要
        << R"HTML(</div>
              <div class="settings-list">
                <div class="settings-row" data-setting="worker-pool-size">
                  <div>
                    <div class="settings-name">任务管理工作线程池大小</div>
                    <div class="settings-meta">当前有效=)HTML"
        << 快照.任务管理工作线程池当前有效大小
        << R"HTML( | 待保存=)HTML"
        << 快照.任务管理工作线程池待保存大小
        << R"HTML( | 默认=)HTML"
        << 快照.任务管理工作线程池默认大小
        << R"HTML( | 范围=)HTML"
        << 快照.任务管理工作线程池最小大小
        << R"HTML(-)HTML"
        << 快照.任务管理工作线程池最大大小
        << R"HTML(</div>
                  </div>
                  <div>
                    <div class="settings-editor">
                      <input id="worker-pool-size-input" class="settings-number" type="number" min=")HTML"
        << 快照.任务管理工作线程池最小大小
        << R"HTML(" max=")HTML"
        << 快照.任务管理工作线程池最大大小
        << R"HTML(" step="1" value=")HTML"
        << 快照.任务管理工作线程池待保存大小
        << R"HTML(" inputmode="numeric" aria-label="任务管理工作线程池大小">
                      <button id="save-worker-pool-size" class="settings-save-btn" type="button">保存</button>
                    </div>
                    <div id="worker-pool-size-status" class="settings-status" role="status"></div>
                  </div>
                </div>
                <div class="settings-result-grid" aria-label="参数保存状态">
                  <div class="settings-result">
                    <div class="settings-result-label">最近保存</div>
                    <div class="settings-result-value">)HTML"
        << 私有_转义HTML(私有_页面摘要(私有_任务工作线程池最近参数保存文本(快照)))
        << R"HTML(</div>
                  </div>
                  <div class="settings-result">
                    <div class="settings-result-label">最近应用</div>
                    <div class="settings-result-value">)HTML"
        << 私有_转义HTML(私有_页面摘要(私有_任务工作线程池最近参数应用文本(快照)))
        << R"HTML(</div>
                  </div>
                  <div class="settings-result">
                    <div class="settings-result-label">最近参数错误</div>
                    <div class="settings-result-value">)HTML"
        << 私有_转义HTML(私有_页面摘要(私有_任务工作线程池最近参数错误文本(快照)))
        << R"HTML(</div>
                  </div>
                </div>
              </div>
            </section>
            <aside class="panel settings-side">
              <div class="panel-topline">任务管理</div>
              <h3>线程池参数</h3>
              <div class="settings-side-list">
                <div class="settings-slot">
                  <div class="settings-side-label">参数版本</div>
                  <div class="settings-side-value">)HTML"
        << 快照.任务管理工作线程池参数版本
        << R"HTML(</div>
                </div>
                <div class="settings-slot">
                  <div class="settings-side-label">任务工作线程池</div>
                  <div class="settings-side-value">)HTML"
        << (快照.任务管理界面线程后台worker池运行 ? "运行中" : "未运行")
        << R"HTML( | 线程数=)HTML"
        << 快照.任务管理界面线程后台worker线程数
        << R"HTML(</div>
                </div>
                <div class="settings-slot">
                  <div class="settings-side-label">扩展参数</div>
                  <div class="settings-side-value">暂无新增参数</div>
                </div>
              </div>
            </aside>
          </div>
          <div class="settings-hidden-tree" hidden>
            <div id="tree-settings"></div>
            <div id="detail-settings"></div>
          </div>
        </section>

        <div class="footer">鱼巢控制面板 · 运行快照</div>
      </main>
    </div>
  </div>
  <div id="pointer-detail-modal" class="pointer-modal-backdrop" hidden>
    <div class="pointer-modal" role="dialog" aria-modal="true" aria-labelledby="pointer-detail-title">
      <div class="pointer-modal-head">
        <div>
          <div class="detail-kicker">指针主信息</div>
          <div id="pointer-detail-title" class="pointer-modal-title">引用目标</div>
        </div>
        <button id="pointer-detail-close" class="pointer-modal-close" type="button">关闭</button>
      </div>
      <div id="pointer-detail-body" class="pointer-modal-body"></div>
    </div>
  </div>)HTML";
    输出 << R"HTML(
  <script>
    const 展开类型标签 = {
      'base-node': '基础信息节点',
      'need-node': '需求节点',
      'task-node': '任务节点',
      'method-node': '方法节点',
      'base-node-more': '更多基础信息子节点',
      'need-node-more': '更多需求子节点',
      'task-node-more': '更多任务子节点',
      'method-node-more': '更多方法子节点',
      'thread-self': '自我线程',
      'thread-task-interface': '任务管理界面线程',
      'thread-worker': '任务管理工作线程',
      'thread-summary': '控制面板摘要线程',
      'need-list-more': '需求列表分页',
      'base-ref': '基础信息节点引用',
      'need-ref': '需求节点引用',
      'task-ref': '任务节点引用',
      'method-ref': '方法节点引用'
    };

    const 页面配置 = {
      'thread-status': {
        treeHost: 'tree-thread-status',
        detailHost: 'detail-thread-status',
        detailHint: '线程详情字段：加载时间、运行时长、状态、用途、ID 和最近事件'
      },
      'world-tree': {
        treeHost: 'tree-world-tree',
        detailHost: 'detail-world-tree',
        detailHint: '世界树页展示基础信息类仓库；右侧只展示当前节点主信息字段，父子上下结构以中间树为准。'
      },
      'causal-info': {
        treeHost: 'tree-causal-info',
        detailHost: 'detail-causal-info',
        detailHint: '因果主信息字段；因果模板和证据动态样本均来自当前世界树。'
      },
      'need-tree': {
        treeHost: 'tree-need-tree',
        detailHost: 'detail-need-tree',
        detailHint: '需求节点字段'
      },
      'need-list': {
        treeHost: 'tree-need-list',
        detailHost: 'detail-need-list',
        detailHint: '需求列表字段'
      },
      'task-tree': {
        treeHost: 'tree-task-tree',
        detailHost: 'detail-task-tree',
        detailHint: '任务主信息字段'
      },
      'method-tree': {
        treeHost: 'tree-method-tree',
        detailHost: 'detail-method-tree',
        detailHint: '方法主信息字段'
      },
      'settings': {
        treeHost: 'tree-settings',
        detailHost: 'detail-settings',
        detailHint: '运行参数字段'
      }
    };

    const 页面树数据 = {
      "thread-status": )HTML"
        << 线程树JSON
        << R"HTML(,
      "world-tree": )HTML"
        << 世界树JSON
        << R"HTML(,
      "causal-info": )HTML"
        << 因果信息JSON
        << R"HTML(,
      "need-tree": )HTML"
        << 需求树JSON
        << R"HTML(,
      "need-list": )HTML"
        << 需求列表JSON
        << R"HTML(,
      "task-tree": )HTML"
        << 任务树JSON
        << R"HTML(,
      "method-tree": )HTML"
        << 方法树JSON
        << R"HTML(,
      "settings": )HTML"
        << 参数设定JSON
        << R"HTML(
    };
    const 自我场景复现数据 = )HTML"
        << 自我场景复现JSON
        << R"HTML(;
    const 自我场景窗口模式 = )HTML"
        << (自我场景窗口模式 ? "true" : "false")
        << R"HTML(;
)HTML";
    输出 << R"HTML(
    
    const 菜单项列表 = Array.from(document.querySelectorAll('.menu-item'));
    const 页面列表 = Array.from(document.querySelectorAll('.page'));
    const 页面标题节点 = document.getElementById('page-title');
    const 页面副标题节点 = document.getElementById('page-subtitle');
    const 页面选中节点 = new Map();
    const 挂起展开 = new Map();
    const 挂起详情 = new Map();
    let 展开请求号 = 1;
    let 详情请求号 = 1;
    let 节点流水号 = 1;
    const 指针弹窗 = document.getElementById('pointer-detail-modal');
    const 指针弹窗标题 = document.getElementById('pointer-detail-title');
    const 指针弹窗正文 = document.getElementById('pointer-detail-body');
    const 指针弹窗关闭 = document.getElementById('pointer-detail-close');

    function 规范化节点(page, node, parent = null) {
      if (!node || typeof node !== 'object') return;
      if (!node.__id) {
        node.__id = `${page}-node-${节点流水号++}`;
      }
      node.__page = page;
      node.__parent = parent;
      node.text = typeof node.text === 'string' ? node.text : '';
      node.open = !!node.open;
      node.lazy = !!node.lazy;
      node.expandType = typeof node.expandType === 'string' ? node.expandType : '';
      node.ptr = Number(node.ptr || 0);
      node.arg = Number(node.arg || 0);
      node.__loaded = !!node.__loaded;
      node.__loading = !!node.__loading;
      node.__detailLoaded = !!node.__detailLoaded;
      node.__detailLoading = !!node.__detailLoading;
      if (!Array.isArray(node.details)) {
        node.details = [];
      }
      if (!Array.isArray(node.children)) {
        node.children = [];
      }
      node.children.forEach((child) => 规范化节点(page, child, node));
    }

    function 查找节点(node, id) {
      if (!node) return null;
      if (node.__id === id) return node;
      for (const child of Array.isArray(node.children) ? node.children : []) {
        const 命中 = 查找节点(child, id);
        if (命中) return 命中;
      }
      return null;
    }

    function 节点稳定键(node) {
      if (!node) return '';
      const 指针 = Number(node.ptr || 0);
      const 附加 = Number(node.arg || 0);
      const 展开 = String(node.expandType || '');
      if (指针) {
        return `${展开}|${指针}|${附加}`;
      }
      const 文本 = String(node.text || '').trim();
      return 文本 ? `text|${文本}` : '';
    }

    function 查找稳定节点(node, key) {
      if (!node || !key) return null;
      if (节点稳定键(node) === key) return node;
      for (const child of Array.isArray(node.children) ? node.children : []) {
        const 命中 = 查找稳定节点(child, key);
        if (命中) return 命中;
      }
      return null;
    }

    function 收集展开节点键(node, keys) {
      if (!node || !keys) return;
      const key = 节点稳定键(node);
      if (node.open && key) keys.add(key);
      (Array.isArray(node.children) ? node.children : []).forEach((child) => 收集展开节点键(child, keys));
    }

    function 恢复展开节点(node, keys) {
      if (!node || !keys) return;
      const key = 节点稳定键(node);
      if (key && keys.has(key)) {
        node.open = true;
      }
      (Array.isArray(node.children) ? node.children : []).forEach((child) => 恢复展开节点(child, keys));
    }

    function 取页面根(page) {
      return 页面树数据[page] || null;
    }

    function 取当前页面节点(page) {
      const 根节点 = 取页面根(page);
      if (!根节点) return null;
      const 选中ID = 页面选中节点.get(page) || 根节点.__id;
      const 节点 = 查找节点(根节点, 选中ID) || 根节点;
      页面选中节点.set(page, 节点.__id);
      return 节点;
    }

    function 转义HTML(text) {
      return String(text ?? '').replace(/[&<>"]/g, (ch) => (
        ch === '&' ? '&amp;'
          : ch === '<' ? '&lt;'
            : ch === '>' ? '&gt;'
              : '&quot;'
      ));
    }

    function 解析节点文本(text) {
      const 原始文本 = String(text || '');
      const 类型标记 = ' | 类型=';
      const 值标记 = ' | 值=';
      const 类型位置 = 原始文本.indexOf(类型标记);
      const 值位置 = 类型位置 >= 0 ? 原始文本.indexOf(值标记, 类型位置 + 类型标记.length) : -1;
      if (类型位置 >= 0 && 值位置 >= 0) {
        return {
          标题: 原始文本.slice(0, 类型位置),
          类型: 原始文本.slice(类型位置 + 类型标记.length, 值位置),
          值: 原始文本.slice(值位置 + 值标记.length),
          附加: []
        };
      }

      const 首个分隔 = 原始文本.indexOf(' | ');
      const 等号位置 = 原始文本.indexOf('=');
      if (等号位置 > 0 && (首个分隔 < 0 || 等号位置 < 首个分隔)) {
        const 数据类型标记 = ' | 数据类型=';
        let 字段值 = 原始文本.slice(等号位置 + 1);
        let 字段类型 = '文本';
        const 数据类型位置 = 字段值.lastIndexOf(数据类型标记);
        if (数据类型位置 >= 0) {
          字段类型 = 字段值.slice(数据类型位置 + 数据类型标记.length) || '文本';
          字段值 = 字段值.slice(0, 数据类型位置);
        }
        return {
          标题: 原始文本.slice(0, 等号位置),
          类型: 字段类型,
          值: 字段值,
          附加: []
        };
      }

      const 片段 = 原始文本.split(' | ');
      return {
        标题: 片段[0] || 原始文本,
        类型: '',
        值: '',
        附加: 片段.slice(1)
      };
    }

    function 取节点标题(node) {
      const 解析结果 = 解析节点文本(node ? node.text : '');
      return 解析结果.标题 || (node ? node.text : '') || '未命名节点';
    }

    function 节点显示文本(node) {
      const 解析结果 = 解析节点文本(node ? node.text : '');
      const parts = [];
      if (解析结果.标题) parts.push(解析结果.标题);
      if (解析结果.类型) parts.push(`类型=${解析结果.类型}`);
      if (解析结果.值) parts.push(`值=${解析结果.值}`);
      解析结果.附加.filter(Boolean).forEach((item) => parts.push(item));
      return parts.join(' | ') || (node ? node.text : '') || '未命名节点';
    }

    function 构建节点路径(node) {
      const 路径 = [];
      let 当前 = node || null;
      while (当前) {
        路径.push(当前);
        当前 = 当前.__parent || null;
      }
      return 路径.reverse();
    }

    function 格式化指针(ptr) {
      const 数值 = Number(ptr || 0);
      if (!数值) {
        return '空';
      }
      return '0x' + 数值.toString(16).toUpperCase();
    }

    function 是否可展开(node) {
      return !!(node && (node.lazy || (Array.isArray(node.children) && node.children.length > 0)));
    }

    function 是否引用节点(node) {
      return !!(node && typeof node.expandType === 'string' && node.expandType.endsWith('-ref'));
    }

    function 推断节点角色(node) {
      if (!node) return '空';
      if (node.expandType && 展开类型标签[node.expandType]) {
        return 展开类型标签[node.expandType];
      }
      const 解析结果 = 解析节点文本(node.text);
      if (解析结果.类型) {
        return 是否引用节点(node) ? '字段引用' : '原始字段';
      }
      return 是否可展开(node) ? '结构节点' : '摘要节点';
    }

    function 推断字段类型(node) {
      const 解析结果 = 解析节点文本(node ? node.text : '');
      if (解析结果.类型) {
        return 解析结果.类型;
      }
      return 是否可展开(node) ? '结构节点' : '文本';
    }

    function 推断字段值(node) {
      const 解析结果 = 解析节点文本(node ? node.text : '');
      if (解析结果.值) {
        return 解析结果.值;
      }
      if (解析结果.附加.length > 0) {
        return 解析结果.附加.join(' | ');
      }
      if (Array.isArray(node?.children) && node.children.length > 0) {
        return `已挂接 ${node.children.length} 个子项`;
      }
      return '空';
    }

    function 节点状态文本(node) {
      if (!node) return '空';
      if (node.__loading) return '正在加载';
      if (node.lazy && !node.__loaded) return '待延迟展开';
      if (node.open) return '已展开';
      if (是否可展开(node)) return '可展开';
      return '静态';
    }

    function 子项摘要(node) {
      if (!node) return '空';
      const 数量 = Array.isArray(node.children) ? node.children.length : 0;
      if (node.lazy && !node.__loaded) {
        return 数量 > 0 ? `${数量} 个预置结构子项，展开时加载下一层子节点` : '展开时加载下一层子节点';
      }
      return `${数量} 个已挂接子项`;
    }

    function 更新选中样式(page, row) {
      const 配置 = 页面配置[page];
      const host = 配置 ? document.getElementById(配置.treeHost) : null;
      if (!host) return;
      host.querySelectorAll('.tree-row.selected').forEach((item) => item.classList.remove('selected'));
      if (row) {
        row.classList.add('selected');
        return;
      }
      const 当前节点 = 取当前页面节点(page);
      if (!当前节点) return;
      const 目标 = host.querySelector(`.tree-row[data-node-id="${当前节点.__id}"]`);
      if (目标) {
        目标.classList.add('selected');
      }
    }

    function 添加详情行(container, key, value) {
      const keyNode = document.createElement('div');
      keyNode.className = 'detail-key';
      keyNode.textContent = key;
      const valueNode = document.createElement('div');
      valueNode.className = 'detail-value';
      valueNode.textContent = value;
      container.appendChild(keyNode);
      container.appendChild(valueNode);
    }

    function 是否可请求详情(node) {
      return !!(
        node
        && node.ptr
        && node.expandType
        && !node.expandType.endsWith('-more')
        && !node.expandType.endsWith('-ref')
      );
    }

    function 是否指针引用字段(node) {
      return !!(
        node
        && node.ptr
        && typeof node.expandType === 'string'
        && node.expandType.endsWith('-ref')
      );
    }

    function 是否仅显示主信息详情页(page) {
      return page === 'method-tree' || page === 'task-tree';
    }

    function 关闭指针弹窗() {
      if (!指针弹窗) return;
      指针弹窗.classList.remove('show');
      指针弹窗.hidden = true;
      if (指针弹窗正文) {
        指针弹窗正文.replaceChildren();
      }
    }

    function 显示指针弹窗加载(node) {
      if (!指针弹窗 || !指针弹窗标题 || !指针弹窗正文) return;
      const 解析结果 = 解析节点文本(node?.text || '');
      指针弹窗标题.textContent = `${解析结果.标题 || '引用目标'} = ${解析结果.值 || '空'}`;
      指针弹窗正文.replaceChildren();
      const loading = document.createElement('div');
      loading.className = 'detail-empty';
      loading.textContent = '正在读取指针目标主信息...';
      指针弹窗正文.appendChild(loading);
      指针弹窗.hidden = false;
      指针弹窗.classList.add('show');
    }

    function 渲染指针弹窗详情(page, node, 详情列表) {
      if (!指针弹窗 || !指针弹窗标题 || !指针弹窗正文) return;
      const 解析结果 = 解析节点文本(node?.text || '');
      指针弹窗标题.textContent = `${解析结果.标题 || '引用目标'} = ${解析结果.值 || '空'}`;
      指针弹窗正文.replaceChildren();
      const 列表 = Array.isArray(详情列表) ? 详情列表 : [];
      if (列表.length === 0) {
        const empty = document.createElement('div');
        empty.className = 'detail-empty';
        empty.textContent = '未读取到指针目标主信息。';
        指针弹窗正文.appendChild(empty);
      } else {
        列表.forEach((item) => {
          指针弹窗正文.appendChild(创建详情节点(page, item));
        });
      }
      指针弹窗.hidden = false;
      指针弹窗.classList.add('show');
    }

    function 规范化详情节点(page, node, parent = null) {
      if (!node || typeof node !== 'object') return;
      node.__page = page;
      node.__parent = parent;
      node.text = typeof node.text === 'string' ? node.text : '';
      node.open = !!node.open;
      node.lazy = !!node.lazy;
      node.expandType = typeof node.expandType === 'string' ? node.expandType : '';
      node.ptr = Number(node.ptr || 0);
      node.arg = Number(node.arg || 0);
      node.__loaded = !!node.__loaded;
      node.__loading = !!node.__loading;
      if (!Array.isArray(node.children)) {
        node.children = [];
      }
      node.children.forEach((child) => 规范化详情节点(page, child, node));
    }

    function 替换分页节点(node, 子项列表) {
      const parent = node ? node.__parent : null;
      if (!parent || !Array.isArray(parent.children)) {
        return false;
      }
      const 索引 = parent.children.findIndex((child) => child && child.__id === node.__id);
      if (索引 < 0) {
        return false;
      }
      const 新子项 = Array.isArray(子项列表) ? 子项列表 : [];
      新子项.forEach((child) => 规范化节点(node.__page, child, parent));
      parent.children.splice(索引, 1, ...新子项);
      if (新子项.length > 0 && node.__page) {
        页面选中节点.set(node.__page, 新子项[0].__id);
      }
      return true;
    }

    function 请求节点详情(page, node, 完成回调) {
      if (!(window.chrome && window.chrome.webview) || !page || !是否可请求详情(node)) {
        return false;
      }
      if (node.__detailLoading || node.__detailLoaded) {
        return true;
      }

      const 请求号 = 详情请求号++;
      node.__detailLoading = true;
      挂起详情.set(请求号, { page, node, 完成回调 });
      window.chrome.webview.postMessage(`detail:${请求号}:${node.expandType}:${node.ptr}`);
      更新详情面板(page);
      return true;
    }

    function 请求指针主信息弹窗(page, node) {
      if (!(window.chrome && window.chrome.webview) || !page || !是否指针引用字段(node)) {
        return false;
      }
      const 请求号 = 详情请求号++;
      显示指针弹窗加载(node);
      挂起详情.set(请求号, { page, node, 弹窗: true });
      window.chrome.webview.postMessage(`detail:${请求号}:${node.expandType}:${node.ptr}`);
      return true;
    }
)HTML";
    输出 << R"HTML(

    function 请求节点展开(node, 完成回调) {
      if (!(window.chrome && window.chrome.webview) || !node || !node.lazy || !node.ptr || !node.expandType) {
        return false;
      }
      if (node.__loading) {
        return true;
      }

      const 请求号 = 展开请求号++;
      node.__loading = true;
      挂起展开.set(请求号, { page: node.__page, node, 完成回调 });
      window.chrome.webview.postMessage(`expand:${请求号}:${node.expandType}:${node.ptr}:${Number(node.arg || 0)}`);
      if (node.__page) {
        更新详情面板(node.__page);
      }
      return true;
    }

    window.__panelApplyDetail = function(请求号, 详情列表) {
      const 记录 = 挂起详情.get(Number(请求号));
      if (!记录) return;
      if (记录.弹窗) {
        const 列表 = Array.isArray(详情列表) ? 详情列表 : [];
        列表.forEach((item) => 规范化详情节点(记录.page, item, 记录.node));
        挂起详情.delete(Number(请求号));
        渲染指针弹窗详情(记录.page, 记录.node, 列表);
        return;
      }
      记录.node.details = Array.isArray(详情列表) ? 详情列表 : [];
      记录.node.__detailLoaded = true;
      记录.node.__detailLoading = false;
      记录.node.details.forEach((item) => 规范化详情节点(记录.page, item, 记录.node));
      挂起详情.delete(Number(请求号));
      if (typeof 记录.完成回调 === 'function') {
        记录.完成回调(记录.node);
      }
      更新详情面板(记录.page);
    };

    window.__panelApplyExpand = function(请求号, 子项列表) {
      const 记录 = 挂起展开.get(Number(请求号));
      if (!记录) return;
      if (记录.node.expandType.endsWith('-more') && 替换分页节点(记录.node, 子项列表)) {
        记录.node.__loading = false;
        挂起展开.delete(Number(请求号));
        if (typeof 记录.完成回调 === 'function') {
          记录.完成回调(记录.node.__parent || null);
        }
        渲染页面树(记录.page);
        return;
      }
      记录.node.children = Array.isArray(子项列表) ? 子项列表 : [];
      记录.node.__loaded = true;
      记录.node.__loading = false;
      记录.node.children.forEach((child) => 规范化节点(记录.page, child, 记录.node));
      挂起展开.delete(Number(请求号));
      if (typeof 记录.完成回调 === 'function') {
        记录.完成回调(记录.node);
      }
      渲染页面树(记录.page);
    };

    window.__panelApplyPageRefresh = function(data) {
      if (!data || typeof data !== 'object') return;
      const page = data.page || '';
      const config = 页面配置[page];
      if (!config) return;
      if (!data.ok || !data.root) {
        更新详情面板(page);
        return;
      }
      const 原根节点 = 页面树数据[page] || null;
      const 旧选中键 = 节点稳定键(取当前页面节点(page));
      const 展开键集 = new Set();
      收集展开节点键(原根节点, 展开键集);
      const root = data.root;
      页面树数据[page] = root;
      挂起展开.clear();
      挂起详情.clear();
      规范化节点(page, root);
      恢复展开节点(root, 展开键集);
      const 新选中节点 = 查找稳定节点(root, 旧选中键) || root;
      页面选中节点.set(page, 新选中节点.__id);
      更新页面摘要(page, root);
      渲染页面树(page);
      请求节点详情(page, 新选中节点);
      更新详情面板(page);
    };

    function 更新页面摘要(page, root) {
      if (!root || typeof root.text !== 'string') return;
      const 页面 = 页面列表.find((item) => item.dataset.page === page);
      const 摘要节点 = 页面 ? 页面.querySelector('.summary') : null;
      if (!摘要节点) return;
      const 分隔位置 = root.text.indexOf(' | ');
      摘要节点.textContent = 分隔位置 >= 0 ? root.text.slice(分隔位置 + 3) : root.text;
    }

    function 选择节点(page, node, row) {
      if (!page || !node) return;
      页面选中节点.set(page, node.__id);
      更新选中样式(page, row);
      请求节点详情(page, node);
      更新详情面板(page);
    }

    function 切换节点展开(page, node) {
      if (!page || !node) return;
      if (node.open) {
        node.open = false;
        渲染页面树(page);
        return;
      }

      if (node.lazy && !node.__loaded) {
        if (请求节点展开(node, (目标节点) => {
          目标节点.open = true;
        })) {
          更新详情面板(page);
          return;
        }
      }

      node.open = true;
      渲染页面树(page);
    }

    function 跳到引用目标(page, node) {
      if (!page || !node) return;

      const 完成 = (当前节点) => {
        当前节点.open = true;
        if (Array.isArray(当前节点.children) && 当前节点.children.length > 0) {
          页面选中节点.set(page, 当前节点.children[0].__id);
        } else {
          页面选中节点.set(page, 当前节点.__id);
        }
      };

      if (node.lazy && !node.__loaded) {
        if (请求节点展开(node, 完成)) {
          更新详情面板(page);
          return;
        }
      }

      完成(node);
      渲染页面树(page);
      const host = document.getElementById(页面配置[page].treeHost);
      const 当前行 = host ? host.querySelector('.tree-row.selected') : null;
      if (当前行) {
        当前行.scrollIntoView({ block: 'nearest' });
      }
    }

    function 提取任务状态文本(node) {
      const 文本 = String(node?.text || '');
      const 命中 = 文本.match(/(?:^|\s\|\s)状态=([^|]+)/);
      return 命中 ? 命中[1].trim() : '';
    }

    function 任务状态样式名(状态文本) {
      if (状态文本 === '运行中') return 'running';
      return '';
    }

    function 填充树行文本(page, row, node) {
      const 文本 = 节点显示文本(node);
      if (page !== 'task-tree') {
        row.textContent = 文本;
        return;
      }

      const 状态文本 = 提取任务状态文本(node);
      const 状态样式 = 任务状态样式名(状态文本);
      const 正文 = document.createElement('span');
      正文.className = 'tree-row-text';
      正文.textContent = 文本;
      row.appendChild(正文);

      if (!状态样式) {
        return;
      }
      row.classList.add(`task-state-${状态样式}`);
      const 状态徽标 = document.createElement('span');
      状态徽标.className = `task-state-pill ${状态样式}`;
      状态徽标.textContent = 状态文本;
      row.appendChild(状态徽标);
    }

    function 创建树节点(page, node) {
      const li = document.createElement('li');
      const row = document.createElement('div');
      row.className = 'tree-row';
      row.dataset.nodeId = node.__id;
      填充树行文本(page, row, node);
      if (页面选中节点.get(page) === node.__id) {
        row.classList.add('selected');
      }

      const 有子节点 = Array.isArray(node.children) && node.children.length > 0;
      row.classList.toggle('branch', !!node.lazy || 有子节点);
      row.classList.toggle('loading', !!node.__loading);
      row.classList.toggle('open', !!node.open);

      row.addEventListener('click', (event) => {
        event.stopPropagation();
        选择节点(page, node, row);
      });

      row.addEventListener('dblclick', (event) => {
        event.stopPropagation();
        选择节点(page, node, row);
        切换节点展开(page, node);
      });

      li.appendChild(row);
      if (node.open) {
        const children = document.createElement('ul');
        children.className = 'tree-children';
        (Array.isArray(node.children) ? node.children : []).forEach((child) => {
          children.appendChild(创建树节点(page, child));
        });
        li.appendChild(children);
      }
      return li;
    }

    function 渲染页面树(page) {
      const 配置 = 页面配置[page];
      const 根节点 = 取页面根(page);
      const host = 配置 ? document.getElementById(配置.treeHost) : null;
      if (!host || !根节点) return;
      if (!页面选中节点.has(page)) {
        页面选中节点.set(page, 根节点.__id);
      }
      const root = document.createElement('ul');
      root.className = 'tree-root';
      root.appendChild(创建树节点(page, 根节点));
      host.replaceChildren(root);
      更新选中样式(page);
      const 当前节点 = 取当前页面节点(page);
      if (当前节点) {
        请求节点详情(page, 当前节点);
      }
      更新详情面板(page);
    }

    function 切换页面(page) {
      const 目标页面 = 页面列表.find((item) => item.dataset.page === page) || 页面列表[0];
      页面列表.forEach((item) => item.classList.toggle('active', item === 目标页面));
      菜单项列表.forEach((item) => item.classList.toggle('active', item.dataset.page === 目标页面.dataset.page));
      页面标题节点.textContent = 目标页面.dataset.title || '控制面板';
      页面副标题节点.textContent = 目标页面.dataset.subtitle || '';
      渲染页面树(目标页面.dataset.page);
      if (目标页面.dataset.page === 'self-scene') {
        if (自我场景窗口模式) {
          请求绘制自我场景();
        }
      }
      if (location.hash !== '#' + 目标页面.dataset.page) {
        history.replaceState(null, '', '#' + 目标页面.dataset.page);
      }
      window.scrollTo(0, 0);
    }
)HTML";
    输出 << R"HTML(

    function 创建详情节点(page, node) {
      const 解析结果 = 解析节点文本(node?.text || '');
      const 容器 = document.createElement('div');
      容器.className = 解析结果.类型 ? 'detail-node detail-field-node' : 'detail-node detail-group-node';

      if (解析结果.类型) {
        const row = document.createElement('div');
        row.className = 'detail-field-row';
        if (是否指针引用字段(node)) {
          row.classList.add('pointer-field');
          row.title = '双击查看指针目标主信息';
        }

        const name = document.createElement('div');
        name.className = 'detail-field-name';
        name.textContent = 解析结果.标题 || '未命名字段';
        row.appendChild(name);

        const type = document.createElement('div');
        type.className = 'detail-field-type';
        type.textContent = 解析结果.类型 || '文本';
        row.appendChild(type);

        const value = document.createElement('div');
        value.className = 'detail-field-value';
        value.textContent = 解析结果.值 || (解析结果.附加.length > 0 ? 解析结果.附加.join(' | ') : '空');
        row.appendChild(value);

        row.addEventListener('dblclick', (event) => {
          if (是否指针引用字段(node)) {
            event.stopPropagation();
            请求指针主信息弹窗(page, node);
            return;
          }
        });

        容器.appendChild(row);
      } else {
        const head = document.createElement('div');
        head.className = 'detail-group-head';

        const title = document.createElement('div');
        title.className = 'detail-group-title';
        title.textContent = 解析结果.标题 || node.text || '未命名分组';
        head.appendChild(title);

        容器.appendChild(head);

        if (解析结果.附加.length > 0) {
          const meta = document.createElement('div');
          meta.className = 'detail-group-meta';
          meta.textContent = 解析结果.附加.join(' | ');
          容器.appendChild(meta);
        }
      }

      const 显示子项 = Array.isArray(node?.children) && node.children.length > 0 && (!node.lazy || node.open);
      if (显示子项) {
        const children = document.createElement('div');
        children.className = 'detail-node-children';
        node.children.forEach((child) => {
          children.appendChild(创建详情节点(page, child));
        });
        容器.appendChild(children);
      }

      return 容器;
    }

    function 更新详情面板(page) {
      const 配置 = 页面配置[page];
      const 当前页面 = 页面列表.find((item) => item.dataset.page === page) || null;
      const host = 配置 ? document.getElementById(配置.detailHost) : null;
      const 节点 = 取当前页面节点(page);
      if (!host) return;

      host.replaceChildren();
      if (!节点) {
        const 空状态 = document.createElement('div');
        空状态.className = 'detail-empty';
        空状态.textContent = '当前页面没有节点可显示。';
        host.appendChild(空状态);
        return;
      }

      const 解析结果 = 解析节点文本(节点.text);
      const 仅显示主信息 = 是否仅显示主信息详情页(page);
      if (!仅显示主信息) {
        const kicker = document.createElement('div');
        kicker.className = 'detail-kicker';
        kicker.textContent = (当前页面?.dataset.title || '当前页面') + ' / 节点详情';
        host.appendChild(kicker);

        const title = document.createElement('div');
        title.className = 'detail-title';
        title.textContent = 节点显示文本(节点);
        host.appendChild(title);

        const note = document.createElement('div');
        note.className = 'detail-note';
        note.textContent = 配置?.detailHint || '节点主信息与字段详情';
        host.appendChild(note);

        const chips = document.createElement('div');
        chips.className = 'chips';
        [
          推断节点角色(节点),
          '状态=' + 节点状态文本(节点),
          '地址=' + 格式化指针(节点.ptr)
        ].forEach((text) => {
          const chip = document.createElement('div');
          chip.className = 'chip';
          chip.textContent = text;
          chips.appendChild(chip);
        });
        host.appendChild(chips);

        const actions = document.createElement('div');
        actions.className = 'detail-actions';
        if (是否引用节点(节点)) {
          const follow = document.createElement('button');
          follow.type = 'button';
          follow.className = 'detail-btn secondary';
          follow.textContent = '跳到引用目标';
          follow.addEventListener('click', () => 跳到引用目标(page, 节点));
          actions.appendChild(follow);
        }
        if (actions.childNodes.length > 0) {
          host.appendChild(actions);
        }

        const grid = document.createElement('div');
        grid.className = 'detail-grid';
        添加详情行(grid, '页面', 当前页面?.dataset.title || '控制面板');
        添加详情行(grid, '节点角色', 推断节点角色(节点));
        添加详情行(grid, '树节点类型', 推断字段类型(节点));
        添加详情行(grid, '树节点摘要', 推断字段值(节点));
        添加详情行(grid, '展开器', 节点.expandType ? (展开类型标签[节点.expandType] || 节点.expandType) : '静态');
        添加详情行(grid, '展开状态', 节点状态文本(节点));
        添加详情行(
          grid,
          '详情状态',
          节点.__detailLoading
            ? '正在加载'
            : (节点.__detailLoaded ? `已加载 ${Array.isArray(节点.details) ? 节点.details.length : 0} 组` : '未加载'));
        host.appendChild(grid);
      }

      const 需求详情页 = page === 'need-tree' || page === 'need-list';
      if (!仅显示主信息 && !需求详情页 && 解析结果.附加.length > 0) {
        const list = document.createElement('div');
        list.className = 'detail-list';
        解析结果.附加.forEach((item, index) => {
          const card = document.createElement('div');
          card.className = 'detail-item';
          card.innerHTML = `<strong>附加信息 ${index + 1}</strong><br>${转义HTML(item)}`;
          list.appendChild(card);
        });
        host.appendChild(list);
      }

      if (节点.__detailLoading) {
        const loading = document.createElement('div');
        loading.className = 'detail-empty';
        loading.textContent = '正在按需加载当前节点的主信息详细信息...';
        host.appendChild(loading);
      } else if (Array.isArray(节点.details) && 节点.details.length > 0) {
        const detailTree = document.createElement('div');
        detailTree.className = 'detail-tree';
        节点.details.forEach((item) => {
          detailTree.appendChild(创建详情节点(page, item));
        });
        host.appendChild(detailTree);
      } else if (!是否可请求详情(节点)) {
        const empty = document.createElement('div');
        empty.className = 'detail-empty';
        empty.textContent = '当前节点没有单独的主信息字段，右侧保留节点摘要。';
        host.appendChild(empty);
      } else if (!(window.chrome && window.chrome.webview)) {
        const empty = document.createElement('div');
        empty.className = 'detail-empty';
        empty.textContent = '静态 HTML 预览未连接详情接口。';
        host.appendChild(empty);
      }

      if (!仅显示主信息) {
        const raw = document.createElement('div');
        raw.className = 'detail-raw';
        raw.innerHTML = `<strong>原始文本</strong><br>${转义HTML(节点.text || '空')}`;
        host.appendChild(raw);
      }
    }

)HTML";
    输出 << R"HTML(
    let 自我场景GL状态 = null;

    function 设置自我场景状态(text, kind = '') {
      const status = document.getElementById('self-scene-status');
      if (!status) return;
      status.textContent = text || '';
      status.classList.toggle('ok', kind === 'ok');
      status.classList.toggle('error', kind === 'error');
    }

    function 设置自我场景窗口状态(text, kind = '') {
      const status = document.getElementById('self-scene-window-status');
      if (!status) return;
      status.textContent = text || '';
      status.classList.toggle('ok', kind === 'ok');
      status.classList.toggle('error', kind === 'error');
    }

    function 打开自我场景独立窗口() {
      if (!(window.chrome && window.chrome.webview)) {
        设置自我场景窗口状态('静态 HTML 预览未连接窗口接口。', 'error');
        return;
      }
      设置自我场景窗口状态('正在打开独立场景窗口...');
      window.chrome.webview.postMessage('scene:open-window');
    }

    window.__panelApplySceneWindowState = function(data) {
      if (!data || typeof data !== 'object') {
        设置自我场景窗口状态('独立场景窗口状态返回无效。', 'error');
        return;
      }
      设置自我场景窗口状态(data.message || (data.ok ? '独立场景窗口已打开。' : '独立场景窗口打开失败。'), data.ok ? 'ok' : 'error');
    };

    function 设置自我场景文本(id, text) {
      const node = document.getElementById(id);
      if (node) node.textContent = text;
    }

    function 格式化场景数值(value) {
      const n = Number(value || 0);
      if (!Number.isFinite(n)) return '0';
      return String(Math.round(n * 1000) / 1000);
    }

    function 格式化场景三元组(value) {
      const arr = Array.isArray(value) ? value : [0, 0, 0];
      return `${格式化场景数值(arr[0])}, ${格式化场景数值(arr[1])}, ${格式化场景数值(arr[2])}`;
    }

    function 格式化场景比例(value) {
      return `${Number(value || 0)}/10000`;
    }

    function 场景万分比(value) {
      const n = Number(value || 0);
      if (!Number.isFinite(n)) return 0;
      return Math.max(0, Math.min(10000, n));
    }

    function 场景像素比例(pixels, total) {
      const p = Number(pixels || 0);
      const t = Number(total || 0);
      if (!Number.isFinite(p) || !Number.isFinite(t) || t <= 0) return 0;
      return 场景万分比(Math.round(p * 10000 / t));
    }

    function 读取自我场景窗口像素总数(data) {
      const explicitTotal = Number(data?.unknownWindowPixelTotal || data?.windowPixels || data?.expectedPixels || 0);
      if (Number.isFinite(explicitTotal) && explicitTotal > 0) return explicitTotal;
      const width = Number(data?.width || 0);
      const height = Number(data?.height || 0);
      if (Number.isFinite(width) && Number.isFinite(height) && width > 0 && height > 0) {
        return width * height;
      }
      const features = Number(data?.pixelFeatures || 0);
      return Number.isFinite(features) && features > 0 ? features : 0;
    }

    function 读取自我场景未知像素数(data) {
      const value = data?.unknownWindowPixels !== undefined
        ? data.unknownWindowPixels
        : data?.unexplainedPixels;
      const pixels = Number(value || 0);
      return Number.isFinite(pixels) ? Math.max(0, pixels) : 0;
    }

    function 读取自我场景未知区域占比(data, total) {
      const explicitRatio = Number(data?.unknownWindowPixelRatio || 0);
      if (Number.isFinite(explicitRatio) && explicitRatio > 0) return 场景万分比(explicitRatio);
      const pixels = 读取自我场景未知像素数(data);
      return pixels > 0
        ? 场景像素比例(pixels, total)
        : 场景万分比(data?.unexplainedRatio || 0);
    }

    function 设置自我场景图层值(kind, text, ratio) {
      设置自我场景文本(`scene-layer-${kind}-value`, text);
      const meter = document.getElementById(`scene-layer-${kind}-meter`);
      if (meter) {
        meter.style.width = `${场景万分比(ratio) / 100}%`;
      }
    }

    function 自我场景图层启用(id) {
      const node = document.getElementById(`scene-layer-${id}`);
      return !node || !!node.checked;
    }

    function 自我场景区域图层ID(layer) {
      switch (Number(layer || 0)) {
        case 1: return 'depth-hole';
        case 2: return 'filled-depth';
        case 3: return 'unexplained';
        case 4: return 'contour-risk';
        default: return '';
      }
    }

    function 自我场景区域颜色(layer) {
      switch (Number(layer || 0)) {
        case 1: return '#ef4444';
        case 2: return '#facc15';
        case 3: return '#a78bfa';
        case 4: return '#fb7185';
        default: return '#94a3b8';
      }
    }

    function 读取自我场景诊断区域(data) {
      return Array.isArray(data?.diagnosticRegions)
        ? data.diagnosticRegions.filter((region) => {
            const rect = region?.rect || [];
            return 自我场景区域图层ID(region?.layer)
              && rect.length === 4
              && Number(rect[2]) >= Number(rect[0])
              && Number(rect[3]) >= Number(rect[1]);
          })
        : [];
    }

    function 读取自我场景图层摘要(data) {
      const expected = 读取自我场景窗口像素总数(data);
      const unknownPixels = 读取自我场景未知像素数(data);
      const regions = 读取自我场景诊断区域(data);
      const regionCounts = { 'depth-hole': 0, 'filled-depth': 0, unexplained: 0, 'contour-risk': 0 };
      const regionPixels = { 'depth-hole': 0, 'filled-depth': 0, unexplained: 0, 'contour-risk': 0 };
      regions.forEach((region) => {
        const id = 自我场景区域图层ID(region.layer);
        if (!id) return;
        regionCounts[id] += 1;
        regionPixels[id] += Math.max(0, Number(region.pixels || 0));
      });
      const depthHoleRatio = data.depthHolePixels
        ? 场景像素比例(data.depthHolePixels, expected)
        : 场景万分比(10000 - Number(data.depthValidRatio || 0));
      const filledPixels = Number(data.filledDepthSourcePixels || data.fusedDepthHoleCompensationPixels || 0);
      const filledRatio = 场景像素比例(filledPixels, expected);
      const unexplainedRatio = 读取自我场景未知区域占比(data, expected);
      const contourTotal =
        Number(data.colorContourCount || 0)
        + Number(data.depthContourCount || 0)
        + Number(data.spaceProjectionContourCount || 0)
        + Number(data.fusedContourCount || 0);
      const contourRiskCount =
        Number(data.depthBreakContourCount || 0)
        + Number(data.invalidDepthContourCount || 0)
        + Number(data.filledDepthContourCount || 0)
        + Number(data.lowConfidenceContourCount || 0);
      const contourRiskRatio = contourTotal > 0
        ? 场景万分比(Math.round(contourRiskCount * 10000 / contourTotal))
        : 0;
      return {
        depthHoleRatio,
        filledRatio,
        unexplainedRatio,
        contourRiskRatio,
        filledPixels,
        contourRiskCount,
        contourTotal,
        regionCounts,
        regionPixels,
        expectedPixels: expected,
        unknownPixels,
        regionTotal: regions.length
      };
    }

    function 更新自我场景图层摘要() {
      const data = 自我场景复现数据 || {};
      const summary = 读取自我场景图层摘要(data);
      设置自我场景图层值(
        'depth-hole',
        `${data.depthHolePixels || 0} 像素 / ${data.depthHoleRegions || 0} 区 / 矩形 ${summary.regionCounts['depth-hole'] || 0} / ${格式化场景比例(summary.depthHoleRatio)}`,
        summary.depthHoleRatio);
      设置自我场景图层值(
        'filled-depth',
        `${summary.filledPixels || 0} 像素 / 候选 ${data.completionCandidateCount || 0} / 矩形 ${summary.regionCounts['filled-depth'] || 0} / ${格式化场景比例(summary.filledRatio)}`,
        summary.filledRatio);
      设置自我场景图层值(
        'unexplained',
        `${data.unexplainedPixels || 0} 像素 / 待补 ${data.refillObservationRegionCount || 0} / 矩形 ${summary.regionCounts.unexplained || 0} / ${格式化场景比例(summary.unexplainedRatio)}`,
        summary.unexplainedRatio);
      设置自我场景图层值(
        'contour-risk',
        `${summary.contourRiskCount || 0}/${summary.contourTotal || 0} / 矩形 ${summary.regionCounts['contour-risk'] || 0} / ${格式化场景比例(summary.contourRiskRatio)}`,
        summary.contourRiskRatio);
      const status = document.getElementById('scene-layer-status');
      if (status) {
        status.textContent = data.ok
          ? (summary.regionTotal > 0
              ? `区域矩形已落账 ${summary.regionTotal}/${data.diagnosticRegionCount || summary.regionTotal}；掩码状态 ${data.diagnosticRegionMaskState || 0}。`
              : '当前为自我所在场景快照的摘要覆盖层；逐区域几何/掩码尚未落账。')
          : '当前没有可复现快照，区域图层只保留占位状态。';
      }
    }

)HTML";
    输出 << R"HTML(
    function 更新自我场景统计() {
      const data = 自我场景复现数据 || {};
      const candidate = data.candidate || {};
      const summary = 读取自我场景图层摘要(data);
      const knownExistenceCount = 读取自我场景存在总量(data);
      const verifiedExistenceCount = Number(data.verifiedCount || 0);
      const sceneLimitText = data.sceneSubtreeScanLimitHit ? ` / 扫描达上限 ${data.sceneSubtreeScanLimit || 4096}` : '';
      const hostLimitText = data.hostSubtreeScanLimitHit ? ` / 扫描达上限 ${data.sceneSubtreeScanLimit || 4096}` : '';
      设置自我场景文本('scene-frame-size-stat', `${data.width || 0} x ${data.height || 0}`);
      设置自我场景文本('scene-frame-stat', `观察 ${data.currentFrame || 0} / D${data.depthFrame || 0} / C${data.colorFrame || 0}`);
      设置自我场景文本('scene-pixel-stat', `${data.pixelFeatures || 0} / 深度 ${data.depthValidPixels || 0}(${data.depthValidRatio || 0}) / 融合 ${data.fusedDepthValidPixels || 0}(${data.fusedDepthValidRatio || 0}) / 帧组 ${data.observationFrameGroupCount || 0} / 轮廓 ${data.colorContourCount || 0}/${data.depthContourCount || 0}/${data.spaceProjectionContourCount || 0}/${data.fusedContourCount || 0}`);
      设置自我场景文本('scene-candidate-stat', `${data.candidateCount || 0} / 有效点 ${data.candidateValidPixels || 0}`);
      设置自我场景文本('scene-known-existence-stat', `${knownExistenceCount} 个 / 直接 ${data.sceneDirectExistences || 0} / 观察已验证 ${verifiedExistenceCount} / 特征复现 ${data.realRenderableExistenceCount || 0}/${data.realExistenceCount || 0}`);
      设置自我场景文本('scene-unknown-region-stat', `${summary.unknownPixels || 0} / ${summary.expectedPixels || 0} 像素点 (${格式化场景比例(summary.unexplainedRatio)})`);
      设置自我场景文本('scene-center-stat', candidate.valid ? 格式化场景三元组(candidate.center) : '--');
      设置自我场景文本('scene-aabb-stat', candidate.valid ? `${格式化场景三元组(candidate.min)} -> ${格式化场景三元组(candidate.max)}` : '--');
      设置自我场景文本('scene-score-stat', `${data.frameQualityScore || 0} / ${candidate.continuity || 0} / ${candidate.stability || 0}`);
      设置自我场景文本('scene-verify-stat', `假设 ${data.hypothesisVerifyState || 0} / 像素 ${data.pixelOwnershipState || 0}:${data.fullFrameOwnershipState || 0} / 已归属 ${data.assignedPixels || 0}(${data.ownershipRatio || 0}) / 未解释 ${data.unexplainedPixels || 0} / 冲突 ${data.ownershipConflictPixels || 0} / 确认 ${data.confirmState || 0} / 已验证 ${data.verifiedCount || 0} / 补观察 ${data.observationGapState || 0}:${data.refillObservationRegionCount || 0}:${data.observationMissingReason || 0}`);
      设置自我场景文本('scene-info-root-stat', data.sceneTitle || '空');
      设置自我场景文本('scene-info-host-stat', `${data.hostTitle || '空'} / 宿主子树 ${data.hostSubtreeNodes || 0} / 存在 ${data.hostSubtreeExistences || 0}${hostLimitText}`);
      设置自我场景文本('scene-info-direct-stat', `直接子节点 ${data.sceneDirectChildren || 0} / 直接存在 ${data.sceneDirectExistences || 0}`);
      设置自我场景文本('scene-info-subtree-stat', `子树节点 ${data.sceneSubtreeNodes || 0}${sceneLimitText} / 存在 ${data.sceneSubtreeExistences || 0} / 场景 ${data.sceneSubtreeScenes || 0} / 特征 ${data.sceneSubtreeFeatures || 0} / 状态 ${data.sceneSubtreeStates || 0} / 动态 ${data.sceneSubtreeDynamics || 0} / 二次特征 ${data.sceneSubtreeRelations || 0}`);
      设置自我场景文本('scene-info-direct-summary-stat', data.sceneDirectSummary || '空');
      设置自我场景文本('scene-info-subtree-summary-stat', data.sceneSubtreeSummary || '空');
      设置自我场景文本('scene-info-existence-samples-stat', data.sceneExistenceSamples || '空');
      设置自我场景文本('scene-info-host-existence-samples-stat', data.hostExistenceSamples || '空');
      设置自我场景文本('scene-quality-structure-stat', `RGB ${data.rgbStructure || 0} / 原始 ${data.rawDepthStructure || 0} / 滤波 ${data.filteredDepthStructure || 0} / 补全 ${data.filledDepthStructure || 0} / Mask ${data.depthMaskStructure || 0} / XYZ ${data.xyzStructure || 0} / 对齐 ${data.colorDepthAligned || 0} / 存在颜色状态 ${data.realColorStateExistenceCount || 0} / 彩图可回查 ${data.realTextureExistenceCount || 0}`);
      设置自我场景文本('scene-quality-ratio-stat', `质量 ${data.frameQualityScore || 0} / 深度 ${格式化场景比例(data.depthValidRatio)} / 空间 ${格式化场景比例(data.spaceValidRatio)} / 候选 ${格式化场景比例(data.candidateVerifyRatio)} / 未解释 ${格式化场景比例(data.unexplainedRatio)}`);
      设置自我场景文本('scene-depth-source-stat', `原始 ${data.rawDepthSourcePixels || 0} / 滤波 ${data.filteredDepthSourcePixels || 0} / 补全 ${data.filledDepthSourcePixels || 0} / 无效 ${data.noDepthSourcePixels || 0} / 低置信 ${data.filledDepthLowConfidencePixels || 0}`);
      设置自我场景文本('scene-depth-stability-stat', `结构 ${data.depthSourceStructure || 0}:${data.depthStabilityStructure || 0}:${data.depthNeighborhoodStructure || 0} / 稳定 ${data.depthStabilityAverage || 0} / 邻域 ${data.depthNeighborhoodAverage || 0} / 边界 ${data.boundaryDepthStability || 0}`);
      设置自我场景文本('scene-fusion-stat', `状态 ${data.observationFrameGroupState || 0}:${data.fusedBasisFrameSetState || 0} / 帧 ${data.observationFrameGroupCount || 0} / 有效 ${data.fusedDepthValidPixels || 0}(${格式化场景比例(data.fusedDepthValidRatio)}) / 补洞 ${data.fusedDepthHoleCompensationPixels || 0} / 低稳 ${data.fusedDepthLowStabilityPixels || 0}`);
      设置自我场景文本('scene-contour-layer-stat', `分层 ${data.contourLayerState || 0} / 彩 ${data.colorContourCount || 0} / 深 ${data.depthContourCount || 0} / 空间 ${data.spaceProjectionContourCount || 0} / 融合 ${data.fusedContourCount || 0} / 置信 ${data.fusedContourConfidence || 0}`);
      设置自我场景文本('scene-contour-risk-stat', `深度支持 ${格式化场景比例(data.contourDepthSupport)} / 颜色 ${格式化场景比例(data.contourColorSupport)} / 空间 ${格式化场景比例(data.contourSpaceSupport)} / 断裂 ${data.depthBreakContourCount || 0} / 无效 ${data.invalidDepthContourCount || 0} / 补全 ${data.filledDepthContourCount || 0} / 低置信 ${data.lowConfidenceContourCount || 0}`);
      设置自我场景文本('scene-gap-stat', `状态 ${data.observationGapState || 0} / 原因 ${data.observationMissingReason || 0} / 待补区域 ${data.refillObservationRegionCount || 0} / 诊断矩形 ${data.diagnosticRegionCount || 0}:${data.diagnosticRegionSetState || 0}:${data.diagnosticRegionMaskState || 0} / 建议 ${data.observationSuggestion || 0}`);
      设置自我场景文本('scene-gap-candidate-stat', `补全 ${data.completionCandidateCount || 0} / 条件不足 ${data.insufficientCandidateCount || 0} / 待验证 ${data.pendingVerifyCandidateCount || 0} / 部分确认 ${data.partialConfirmCandidateCount || 0}`);
      设置自我场景文本('scene-safety-host-stat', data.safetyHostTitle || '空');
      设置自我场景文本('scene-basic-risk-stat', `基础观察 ${data.basicObservationFactUsableState || 0} / 判断 ${data.basicRiskJudgementState || 0} / 风险明确 ${data.riskStateExplicitState || 0} / 风险值计算 ${data.basicRiskValueCalculatedState || 0}`);
      设置自我场景文本('scene-safety-value-stat', `评估安全值 ${data.sceneAssessmentSafetyValue || 0} / 安全度 ${data.sceneSafetyDegree || 0} / 判定 ${data.sceneSafetyJudgementState || 0}`);
      设置自我场景文本('scene-risk-safety-projection-stat', `场景影响 ${data.riskSafetySceneImpactCandidate || 0} / 可结算 ${data.riskSafetySceneImpactSettleableState || 0} / 入账 ${data.riskSafetySceneImpactBookedState || 0} / 原因 ${data.riskSafetySceneImpactUnsettleableReason || 0} / 层候选 ${data.riskSafetyLayerCandidate || 0} / 投影 ${data.riskSafetyLayerProjectionCandidate || 0} / 缺失因素 ${data.riskSafetyLayerMissingFactorCount || 0}`);
      设置自我场景文本('scene-safety-gap-stat', `安全评估证据不足原因 ${data.safetyEvidenceInsufficientReason || 0}`);
      更新自我场景图层摘要();
    }

)HTML";
    输出 << R"HTML(
    function 场景向量减(a, b) {
      return [a[0] - b[0], a[1] - b[1], a[2] - b[2]];
    }

    function 场景向量叉乘(a, b) {
      return [
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
      ];
    }

    function 场景向量点乘(a, b) {
      return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }

    function 场景向量归一(v) {
      const len = Math.hypot(v[0], v[1], v[2]) || 1;
      return [v[0] / len, v[1] / len, v[2] / len];
    }

    function 场景矩阵相乘(a, b) {
      const out = new Float32Array(16);
      for (let col = 0; col < 4; ++col) {
        for (let row = 0; row < 4; ++row) {
          out[col * 4 + row] =
            a[0 * 4 + row] * b[col * 4 + 0]
            + a[1 * 4 + row] * b[col * 4 + 1]
            + a[2 * 4 + row] * b[col * 4 + 2]
            + a[3 * 4 + row] * b[col * 4 + 3];
        }
      }
      return out;
    }

    function 场景透视矩阵(fovy, aspect, near, far) {
      const f = 1 / Math.tan(fovy / 2);
      const range = 1 / (near - far);
      return new Float32Array([
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far + near) * range, -1,
        0, 0, 2 * far * near * range, 0
      ]);
    }

    function 场景视图矩阵(eye, center, up) {
      const f = 场景向量归一(场景向量减(center, eye));
      const s = 场景向量归一(场景向量叉乘(f, up));
      const u = 场景向量叉乘(s, f);
      return new Float32Array([
        s[0], u[0], -f[0], 0,
        s[1], u[1], -f[1], 0,
        s[2], u[2], -f[2], 0,
        -场景向量点乘(s, eye), -场景向量点乘(u, eye), 场景向量点乘(f, eye), 1
      ]);
    }

    function 编译自我场景着色器(gl, type, source) {
      const shader = gl.createShader(type);
      gl.shaderSource(shader, source);
      gl.compileShader(shader);
      if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        const log = gl.getShaderInfoLog(shader) || 'shader compile failed';
        gl.deleteShader(shader);
        throw new Error(log);
      }
      return shader;
    }

    function 创建自我场景程序(gl) {
      const vertex = 编译自我场景着色器(gl, gl.VERTEX_SHADER, `
        attribute vec3 a_position;
        attribute vec3 a_color;
        uniform mat4 u_matrix;
        varying vec3 v_color;
        void main() {
          gl_Position = u_matrix * vec4(a_position, 1.0);
          gl_PointSize = 9.0;
          v_color = a_color;
        }
      `);
      const fragment = 编译自我场景着色器(gl, gl.FRAGMENT_SHADER, `
        precision mediump float;
        varying vec3 v_color;
        void main() {
          gl_FragColor = vec4(v_color, 1.0);
        }
      `);
      const program = gl.createProgram();
      gl.attachShader(program, vertex);
      gl.attachShader(program, fragment);
      gl.linkProgram(program);
      gl.deleteShader(vertex);
      gl.deleteShader(fragment);
      if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        const log = gl.getProgramInfoLog(program) || 'program link failed';
        gl.deleteProgram(program);
        throw new Error(log);
      }
      return program;
    }

)HTML";
    输出 << R"HTML(
    function 读取自我场景真实存在列表(data) {
      return Array.isArray(data?.existences) ? data.existences : [];
    }

    function 自我场景数组3(value) {
      return Array.isArray(value) && value.length >= 3
        ? [Number(value[0] || 0), Number(value[1] || 0), Number(value[2] || 0)]
        : [0, 0, 0];
    }

    function 自我场景存在具备状态(item, bit) {
      return (Number(item?.geometryState || 0) & bit) !== 0;
    }

    function 自我场景存在可取中心(item) {
      return 自我场景存在具备状态(item, 1);
    }

    function 自我场景存在可取范围(item) {
      if (!自我场景存在具备状态(item, 2)) return false;
      const mn = 自我场景数组3(item?.min);
      const mx = 自我场景数组3(item?.max);
      return mx[0] >= mn[0] && mx[1] >= mn[1] && mx[2] >= mn[2];
    }

    function 自我场景纳入范围(min, max, point) {
      for (let i = 0; i < 3; ++i) {
        const v = Number(point[i] || 0);
        min[i] = Math.min(min[i], v);
        max[i] = Math.max(max[i], v);
      }
    }

    function 取自我场景范围(data) {
      const min = [Infinity, Infinity, Infinity];
      const max = [-Infinity, -Infinity, -Infinity];
      自我场景纳入范围(min, max, [0, 0, 0]);
      const candidate = data?.candidate || {};
      if (candidate.valid) {
        自我场景纳入范围(min, max, 自我场景数组3(candidate.center));
        自我场景纳入范围(min, max, 自我场景数组3(candidate.min));
        自我场景纳入范围(min, max, 自我场景数组3(candidate.max));
      }
      读取自我场景真实存在列表(data).forEach((item) => {
        if (自我场景存在可取中心(item)) {
          自我场景纳入范围(min, max, 自我场景数组3(item.center));
        }
        if (自我场景存在可取范围(item)) {
          自我场景纳入范围(min, max, 自我场景数组3(item.min));
          自我场景纳入范围(min, max, 自我场景数组3(item.max));
        }
      });
      for (let i = 0; i < 3; ++i) {
        if (!Number.isFinite(min[i]) || !Number.isFinite(max[i])) {
          min[i] = [-1000, -100, -1000][i];
          max[i] = [1000, 900, 1000][i];
        }
      }
      const pad = Math.max(100, Math.max(max[0] - min[0], max[1] - min[1], max[2] - min[2]) * 0.18);
      for (let i = 0; i < 3; ++i) {
        min[i] -= pad;
        max[i] += pad;
      }
      const center = [
        (min[0] + max[0]) / 2,
        (min[1] + max[1]) / 2,
        (min[2] + max[2]) / 2
      ];
      const extent = Math.max(max[0] - min[0], max[1] - min[1], max[2] - min[2], 1);
      return { min, max, center, extent, scale: 4 / extent };
    }

    function 读取自我场景存在总量(data) {
      const known = Number(data?.knownExistenceCount || 0);
      const direct = Number(data?.sceneDirectExistences || 0);
      const subtree = Number(data?.sceneSubtreeExistences || 0);
      const verified = Number(data?.verifiedCount || 0);
      const real = Number(data?.realExistenceCount || 0);
      return Math.max(0, known, direct, subtree, verified, real);
    }

    function 自我场景存在显示颜色(item) {
      if (Number(item?.textureReady || 0) > 0) return [0.94, 0.72, 0.22];
      if (Number(item?.colorBuffer || 0) > 0 || Number(item?.pixelColorLayer || 0) > 0) return [0.42, 0.70, 0.96];
      if (Number(item?.confirmState || 0) > 0) return [0.12, 0.80, 0.72];
      return [0.74, 0.80, 0.86];
    }

    function 自我场景存在盒颜色(item) {
      if (Number(item?.textureReady || 0) > 0) return [0.78, 0.52, 0.10];
      if (Number(item?.colorBuffer || 0) > 0 || Number(item?.pixelColorLayer || 0) > 0) return [0.18, 0.38, 0.70];
      if (Number(item?.confirmState || 0) > 0) return [0.08, 0.34, 0.32];
      return [0.30, 0.36, 0.42];
    }

    function 构建自我场景真实存在视图(data) {
      const items = 读取自我场景真实存在列表(data);
      const nodes = [];
      items.forEach((item) => {
        if (!自我场景存在可取中心(item)) return;
        nodes.push({
          item,
          point: 自我场景数组3(item.center),
          color: 自我场景存在显示颜色(item),
          lineColor: 自我场景存在盒颜色(item)
        });
      });
      const renderLimit = 512;
      return {
        nodes: nodes.slice(0, renderLimit),
        total: items.length,
        rendered: Math.min(nodes.length, renderLimit),
        truncated: nodes.length > renderLimit
      };
    }

    function 构建自我场景几何(data) {
      const range = 取自我场景范围(data || {});
      const lines = [];
      const points = [];
      const transform = (p) => [
        (Number(p[0] || 0) - range.center[0]) * range.scale,
        (Number(p[1] || 0) - range.center[1]) * range.scale,
        (Number(p[2] || 0) - range.center[2]) * range.scale
      ];
      const pushVertex = (target, p, color) => {
        const q = transform(p);
        target.push(q[0], q[1], q[2], color[0], color[1], color[2]);
      };
      const pushLine = (a, b, color) => {
        pushVertex(lines, a, color);
        pushVertex(lines, b, color);
      };
      const pushPoint = (p, color) => pushVertex(points, p, color);

      const gridColor = [0.17, 0.26, 0.29];
      const y = Math.min(0, range.min[1]);
      const steps = 10;
      for (let i = 0; i <= steps; ++i) {
        const t = i / steps;
        const x = range.min[0] + (range.max[0] - range.min[0]) * t;
        const z = range.min[2] + (range.max[2] - range.min[2]) * t;
        pushLine([x, y, range.min[2]], [x, y, range.max[2]], gridColor);
        pushLine([range.min[0], y, z], [range.max[0], y, z], gridColor);
      }

      const axisLen = range.extent * 0.45;
      pushLine([0, 0, 0], [axisLen, 0, 0], [0.95, 0.38, 0.38]);
      pushLine([0, 0, 0], [0, axisLen, 0], [0.2, 0.83, 0.52]);
      pushLine([0, 0, 0], [0, 0, axisLen], [0.38, 0.65, 0.98]);
      pushPoint([0, 0, 0], [1.0, 1.0, 1.0]);

      const pushBox = (mn, mx, color) => {
        const corners = [
          [mn[0], mn[1], mn[2]], [mx[0], mn[1], mn[2]], [mx[0], mx[1], mn[2]], [mn[0], mx[1], mn[2]],
          [mn[0], mn[1], mx[2]], [mx[0], mn[1], mx[2]], [mx[0], mx[1], mx[2]], [mn[0], mx[1], mx[2]]
        ];
        [[0,1],[1,2],[2,3],[3,0],[4,5],[5,6],[6,7],[7,4],[0,4],[1,5],[2,6],[3,7]].forEach((edge) => {
          pushLine(corners[edge[0]], corners[edge[1]], color);
        });
      };

      const existenceView = 构建自我场景真实存在视图(data || {});
      existenceView.nodes.forEach((node) => {
        const item = node.item || {};
        if (自我场景存在可取范围(item)) {
          pushBox(自我场景数组3(item.min), 自我场景数组3(item.max), node.lineColor);
        }
        pushPoint(node.point, node.color);
      });

      const candidate = data?.candidate || {};
      if (candidate.valid) {
        const mn = candidate.min || [0, 0, 0];
        const mx = candidate.max || [0, 0, 0];
        const boxColor = data?.verifiedFlag ? [0.06, 0.84, 0.56] : [0.96, 0.58, 0.12];
        const pointColor = data?.verifiedFlag ? [0.78, 1.0, 0.72] : [0.06, 0.84, 0.56];
        pushBox(mn, mx, boxColor);
        pushPoint(candidate.center || [0, 0, 0], pointColor);
      }

      return {
        lineData: new Float32Array(lines),
        pointData: new Float32Array(points),
        lineCount: lines.length / 6,
        pointCount: points.length / 6,
        existenceNodeCount: existenceView.rendered,
        existenceTotalCount: existenceView.total
      };
    }

)HTML";
    输出 << R"HTML(
    function 调整自我场景画布尺寸(gl, canvas) {
      const rect = canvas.getBoundingClientRect();
      const width = Math.max(1, Math.floor((rect.width || canvas.clientWidth || 960) * window.devicePixelRatio));
      const height = Math.max(1, Math.floor((rect.height || canvas.clientHeight || 540) * window.devicePixelRatio));
      if (canvas.width !== width || canvas.height !== height) {
        canvas.width = width;
        canvas.height = height;
      }
      gl.viewport(0, 0, canvas.width, canvas.height);
    }

    function 绘制自我场景诊断覆盖层() {
      const overlay = document.getElementById('self-scene-layer-overlay');
      const source = document.getElementById('self-scene-gl');
      if (!overlay || !source) return;
      const rect = source.getBoundingClientRect();
      const dpr = window.devicePixelRatio || 1;
      const width = Math.max(1, Math.floor((rect.width || source.clientWidth || 960) * dpr));
      const height = Math.max(1, Math.floor((rect.height || source.clientHeight || 540) * dpr));
      if (overlay.width !== width || overlay.height !== height) {
        overlay.width = width;
        overlay.height = height;
      }
      const ctx = overlay.getContext('2d');
      if (!ctx) return;
      ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
      const cssWidth = width / dpr;
      const cssHeight = height / dpr;
      ctx.clearRect(0, 0, cssWidth, cssHeight);

      const data = 自我场景复现数据 || {};
      if (!data.ok) return;
      const summary = 读取自我场景图层摘要(data);
      const regions = 读取自我场景诊断区域(data).filter((region) => {
        const layerId = 自我场景区域图层ID(region.layer);
        return layerId && 自我场景图层启用(layerId);
      });
      const layers = [
        {
          id: 'depth-hole',
          name: '深度空洞',
          value: summary.depthHoleRatio,
          color: '#ef4444',
          text: `${data.depthHoleRegions || 0}区`
        },
        {
          id: 'filled-depth',
          name: '补全深度',
          value: summary.filledRatio,
          color: '#facc15',
          text: `${data.completionCandidateCount || 0}候选`
        },
        {
          id: 'unexplained',
          name: '未解释',
          value: summary.unexplainedRatio,
          color: '#a78bfa',
          text: `${data.refillObservationRegionCount || 0}待补`
        },
        {
          id: 'contour-risk',
          name: '轮廓风险',
          value: summary.contourRiskRatio,
          color: '#fb7185',
          text: `${summary.contourRiskCount || 0}项`
        }
      ].filter((item) => 自我场景图层启用(item.id));

      if (layers.length === 0) return;
      const panelWidth = Math.min(430, Math.max(280, cssWidth * 0.46));
      const rowHeight = 26;
      const panelHeight = 44 + layers.length * rowHeight;
      const x = 18;
      const y = 18;
      ctx.fillStyle = 'rgba(8, 13, 18, 0.72)';
      ctx.strokeStyle = 'rgba(255,255,255,0.14)';
      ctx.lineWidth = 1;
      ctx.beginPath();
      if (ctx.roundRect) {
        ctx.roundRect(x, y, panelWidth, panelHeight, 8);
      } else {
        ctx.rect(x, y, panelWidth, panelHeight);
      }
      ctx.fill();
      ctx.stroke();

      ctx.fillStyle = 'rgba(255,255,255,0.86)';
      ctx.font = '700 12px system-ui, sans-serif';
      ctx.fillText(regions.length > 0 ? '诊断区域矩形' : '诊断区域摘要覆盖', x + 12, y + 22);
      ctx.font = '12px system-ui, sans-serif';
      layers.forEach((layer, index) => {
        const rowY = y + 42 + index * rowHeight;
        const barX = x + 82;
        const barWidth = panelWidth - 170;
        const activeWidth = Math.max(2, barWidth * 场景万分比(layer.value) / 10000);
        ctx.fillStyle = 'rgba(255,255,255,0.68)';
        ctx.fillText(layer.name, x + 12, rowY + 12);
        ctx.fillStyle = 'rgba(255,255,255,0.14)';
        ctx.fillRect(barX, rowY, barWidth, 12);
        ctx.fillStyle = layer.color;
        ctx.globalAlpha = 0.86;
        ctx.fillRect(barX, rowY, activeWidth, 12);
        ctx.globalAlpha = 1;
        ctx.fillStyle = 'rgba(255,255,255,0.78)';
        ctx.fillText(`${layer.value}/10000 ${layer.text}`, barX + barWidth + 10, rowY + 11);
      });

      if (regions.length === 0) {
        ctx.fillStyle = 'rgba(255,255,255,0.55)';
        ctx.font = '12px system-ui, sans-serif';
        ctx.fillText('几何待落账', Math.max(18, cssWidth - 98), Math.max(32, cssHeight - 18));
        return;
      }

      const frameW = Math.max(1, Number(data.width || 0));
      const frameH = Math.max(1, Number(data.height || 0));
      const mapMaxWidth = Math.min(430, Math.max(260, cssWidth * 0.44));
      const mapWidth = Math.max(220, Math.min(mapMaxWidth, cssWidth - 36));
      const mapHeight = Math.max(140, Math.min(260, mapWidth * frameH / frameW + 36));
      const mapX = 18;
      const mapY = y + panelHeight + 14;
      if (mapY + mapHeight + 10 > cssHeight) {
        ctx.fillStyle = 'rgba(255,255,255,0.62)';
        ctx.font = '12px system-ui, sans-serif';
        ctx.fillText(`${regions.length} 矩形`, Math.max(18, cssWidth - 88), Math.max(32, cssHeight - 18));
        return;
      }

      ctx.fillStyle = 'rgba(8, 13, 18, 0.66)';
      ctx.strokeStyle = 'rgba(255,255,255,0.14)';
      ctx.beginPath();
      if (ctx.roundRect) {
        ctx.roundRect(mapX, mapY, mapWidth, mapHeight, 8);
      } else {
        ctx.rect(mapX, mapY, mapWidth, mapHeight);
      }
      ctx.fill();
      ctx.stroke();

      ctx.fillStyle = 'rgba(255,255,255,0.82)';
      ctx.font = '700 12px system-ui, sans-serif';
      ctx.fillText(`${regions.length}/${data.diagnosticRegionCount || regions.length} 矩形`, mapX + 12, mapY + 21);

      const innerX = mapX + 12;
      const innerY = mapY + 32;
      const innerW = mapWidth - 24;
      const innerH = mapHeight - 44;
      const scale = Math.min(innerW / frameW, innerH / frameH);
      const drawW = frameW * scale;
      const drawH = frameH * scale;
      const originX = innerX + (innerW - drawW) / 2;
      const originY = innerY + (innerH - drawH) / 2;
      ctx.strokeStyle = 'rgba(255,255,255,0.22)';
      ctx.strokeRect(originX, originY, drawW, drawH);

      regions.slice(0, 180).forEach((region) => {
        const rect = region.rect || [0, 0, 0, 0];
        const minX = Math.max(0, Math.min(frameW - 1, Number(rect[0] || 0)));
        const minY = Math.max(0, Math.min(frameH - 1, Number(rect[1] || 0)));
        const maxX = Math.max(0, Math.min(frameW - 1, Number(rect[2] || 0)));
        const maxY = Math.max(0, Math.min(frameH - 1, Number(rect[3] || 0)));
        const rx = originX + Math.min(minX, maxX) * scale;
        const ry = originY + Math.min(minY, maxY) * scale;
        const rw = Math.max(2, (Math.abs(maxX - minX) + 1) * scale);
        const rh = Math.max(2, (Math.abs(maxY - minY) + 1) * scale);
        const color = 自我场景区域颜色(region.layer);
        ctx.globalAlpha = region.layer === 1 ? 0.24 : 0.18;
        ctx.fillStyle = color;
        ctx.fillRect(rx, ry, rw, rh);
        ctx.globalAlpha = 0.88;
        ctx.strokeStyle = color;
        ctx.strokeRect(rx, ry, rw, rh);
      });
      ctx.globalAlpha = 1;
    }

)HTML";
    输出 << R"HTML(
    function 初始化自我场景WebGL() {
      if (自我场景GL状态) return 自我场景GL状态;
      更新自我场景统计();
      const canvas = document.getElementById('self-scene-gl');
      if (!canvas) return null;
      const gl = canvas.getContext('webgl', { antialias: true, alpha: false }) || canvas.getContext('experimental-webgl');
      if (!gl) {
        设置自我场景状态('OpenGL(WebGL) 初始化失败。', 'error');
        return null;
      }
      try {
        const program = 创建自我场景程序(gl);
        const geometry = 构建自我场景几何(自我场景复现数据 || {});
        const lineBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, lineBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, geometry.lineData, gl.STATIC_DRAW);
        const pointBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, pointBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, geometry.pointData, gl.STATIC_DRAW);
        自我场景GL状态 = {
          gl,
          canvas,
          program,
          lineBuffer,
          pointBuffer,
          lineCount: geometry.lineCount,
          pointCount: geometry.pointCount,
          position: gl.getAttribLocation(program, 'a_position'),
          color: gl.getAttribLocation(program, 'a_color'),
          matrix: gl.getUniformLocation(program, 'u_matrix'),
          angle: 0.72,
          rotating: true,
          frame: 0
        };
        const existenceStatus = geometry.existenceTotalCount > 0
          ? `已绘制 ${geometry.existenceNodeCount}/${geometry.existenceTotalCount} 个存在特征复现项。`
          : '';
        设置自我场景状态(
          (自我场景复现数据 && 自我场景复现数据.ok)
            ? `已用自我所在场景快照完成 OpenGL 复现。${existenceStatus}`
            : `自我所在场景暂无可复现快照，已显示坐标基准。${existenceStatus}`,
          (自我场景复现数据 && 自我场景复现数据.ok) ? 'ok' : '');
      } catch (error) {
        设置自我场景状态(`OpenGL(WebGL) 渲染准备失败：${error.message || error}`, 'error');
        自我场景GL状态 = null;
      }
      return 自我场景GL状态;
    }

    function 绘制自我场景() {
      const state = 初始化自我场景WebGL();
      if (!state) return;
      const { gl, canvas, program } = state;
      调整自我场景画布尺寸(gl, canvas);
      gl.clearColor(0.03, 0.05, 0.07, 1);
      gl.clearDepth(1);
      gl.enable(gl.DEPTH_TEST);
      gl.depthFunc(gl.LEQUAL);
      gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
      gl.useProgram(program);

      const aspect = Math.max(1, canvas.width) / Math.max(1, canvas.height);
      const angle = state.angle;
      const eye = [Math.sin(angle) * 6.4, 3.7, Math.cos(angle) * 6.4];
      const projection = 场景透视矩阵(Math.PI / 4.5, aspect, 0.1, 100);
      const view = 场景视图矩阵(eye, [0, 0, 0], [0, 1, 0]);
      const matrix = 场景矩阵相乘(projection, view);
      gl.uniformMatrix4fv(state.matrix, false, matrix);

      const bindGeometry = (buffer) => {
        gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
        gl.enableVertexAttribArray(state.position);
        gl.vertexAttribPointer(state.position, 3, gl.FLOAT, false, 24, 0);
        gl.enableVertexAttribArray(state.color);
        gl.vertexAttribPointer(state.color, 3, gl.FLOAT, false, 24, 12);
      };

      bindGeometry(state.lineBuffer);
      if (state.lineCount > 0) {
        gl.drawArrays(gl.LINES, 0, state.lineCount);
      }
      bindGeometry(state.pointBuffer);
      if (state.pointCount > 0) {
        gl.drawArrays(gl.POINTS, 0, state.pointCount);
      }
      绘制自我场景诊断覆盖层();

      if (state.rotating && (自我场景窗口模式 || document.querySelector('.page.active')?.dataset.page === 'self-scene')) {
        state.angle += 0.006;
      }
      state.frame = window.requestAnimationFrame(绘制自我场景);
    }

    function 请求绘制自我场景() {
      const state = 初始化自我场景WebGL();
      if (!state) return;
      if (!state.frame) {
        绘制自我场景();
      }
    }

    function 重置自我场景视角() {
      const state = 初始化自我场景WebGL();
      if (!state) return;
      state.angle = 0.72;
      请求绘制自我场景();
    }

    function 切换自我场景旋转() {
      const state = 初始化自我场景WebGL();
      if (!state) return;
      state.rotating = !state.rotating;
      const button = document.getElementById('scene-toggle-rotation');
      if (button) {
        button.textContent = state.rotating ? '暂停旋转' : '继续旋转';
      }
      请求绘制自我场景();
    }

)HTML";
    输出 << R"HTML(
    function 设置相机状态(text, kind = '') {
      const status = document.getElementById('camera-status');
      if (!status) return;
      status.textContent = text || '';
      status.classList.toggle('ok', kind === 'ok');
      status.classList.toggle('error', kind === 'error');
    }

    function 打开相机独立窗口() {
      if (!(window.chrome && window.chrome.webview)) {
        设置相机状态('静态 HTML 预览未连接窗口接口。', 'error');
        return;
      }
      设置相机状态('正在打开独立播放窗口...');
      window.chrome.webview.postMessage('camera:open-window');
    }

    window.__panelApplyCameraWindowState = function(data) {
      if (!data || typeof data !== 'object') {
        设置相机状态('独立播放窗口状态返回无效。', 'error');
        return;
      }
      设置相机状态(data.message || (data.ok ? '独立播放窗口已打开。' : '独立播放窗口打开失败。'), data.ok ? 'ok' : 'error');
    };

)HTML";
    输出 << R"HTML(
    菜单项列表.forEach((item) => {
      item.addEventListener('click', () => {
        const 目标页 = item.dataset.page;
        切换页面(目标页);
        请求刷新控制面板页(目标页);
      });
    });

    if (指针弹窗关闭) {
      指针弹窗关闭.addEventListener('click', 关闭指针弹窗);
    }
    if (指针弹窗) {
      指针弹窗.addEventListener('click', (event) => {
        if (event.target === 指针弹窗) {
          关闭指针弹窗();
        }
      });
    }
    document.addEventListener('keydown', (event) => {
      if (event.key === 'Escape') {
        关闭指针弹窗();
      }
    });

    function 设置线程池保存状态(text, kind = '') {
      const status = document.getElementById('worker-pool-size-status');
      if (!status) return;
      status.textContent = text || '';
      status.classList.toggle('ok', kind === 'ok');
      status.classList.toggle('error', kind === 'error');
    }

    function 提交线程池大小设置() {
      const input = document.getElementById('worker-pool-size-input');
      if (!input) return;
      const min = Number(input.min || 1);
      const max = Number(input.max || 128);
      const value = Number(input.value);
      if (!Number.isInteger(value) || value < min || value > max) {
        设置线程池保存状态(`请输入 ${min}-${max} 的整数。`, 'error');
        return;
      }
      if (!(window.chrome && window.chrome.webview)) {
        设置线程池保存状态('静态 HTML 预览未连接参数保存接口。', 'error');
        return;
      }
      设置线程池保存状态('已提交保存请求。', 'ok');
      window.chrome.webview.postMessage(`settings:worker-pool-size:${value}`);
    }

    const 线程池大小输入 = document.getElementById('worker-pool-size-input');
    const 线程池大小保存按钮 = document.getElementById('save-worker-pool-size');
    if (线程池大小保存按钮) {
      线程池大小保存按钮.addEventListener('click', 提交线程池大小设置);
    }
    if (线程池大小输入) {
      线程池大小输入.addEventListener('keydown', (event) => {
        if (event.key === 'Enter') {
          event.preventDefault();
          提交线程池大小设置();
        }
      });
    }

    const 相机独立窗口按钮 = document.getElementById('camera-open-window');
    if (相机独立窗口按钮) {
      相机独立窗口按钮.addEventListener('click', 打开相机独立窗口);
    }

    const 自我场景独立窗口按钮 = document.getElementById('self-scene-open-window');
    if (自我场景独立窗口按钮) {
      自我场景独立窗口按钮.addEventListener('click', 打开自我场景独立窗口);
    }

    const 自我场景重置按钮 = document.getElementById('scene-reset-view');
    const 自我场景刷新按钮 = document.getElementById('scene-refresh-window');
    const 自我场景旋转按钮 = document.getElementById('scene-toggle-rotation');
    if (自我场景重置按钮) {
      自我场景重置按钮.addEventListener('click', 重置自我场景视角);
    }
    if (自我场景刷新按钮) {
      自我场景刷新按钮.addEventListener('click', () => {
        if (window.chrome && window.chrome.webview) {
          window.chrome.webview.postMessage('refresh');
        } else {
          location.reload();
        }
      });
    }
    if (自我场景旋转按钮) {
      自我场景旋转按钮.addEventListener('click', 切换自我场景旋转);
    }
    ['depth-hole', 'filled-depth', 'unexplained', 'contour-risk'].forEach((layerId) => {
      const checkbox = document.getElementById(`scene-layer-${layerId}`);
      if (checkbox) {
        checkbox.addEventListener('change', () => {
          绘制自我场景诊断覆盖层();
          请求绘制自我场景();
        });
      }
    });
    window.addEventListener('resize', () => {
      if (自我场景窗口模式) {
        请求绘制自我场景();
      }
    });

    function 请求刷新控制面板页(page) {
      if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage(`refresh-page:${page || 'thread-status'}`);
        return true;
      }
      return false;
    }

    document.getElementById('copy-page').addEventListener('click', async () => {
      const 当前页面 = 页面列表.find((item) => item.classList.contains('active')) || 页面列表[0];
      const 文本 = 当前页面 ? 当前页面.innerText.trim() : '';
      if (!文本) return;
      try {
        await navigator.clipboard.writeText(文本);
      } catch (_) {
        const area = document.createElement('textarea');
        area.value = 文本;
        area.style.position = 'fixed';
        area.style.opacity = '0';
        document.body.appendChild(area);
        area.focus();
        area.select();
        document.execCommand('copy');
        area.remove();
      }
    });

    window.addEventListener('hashchange', () => {
      切换页面(location.hash.slice(1));
    });

    Object.entries(页面树数据).forEach(([page, root]) => {
      规范化节点(page, root);
      if (root) {
        页面选中节点.set(page, root.__id);
      }
    });

    Object.keys(页面配置).forEach((page) => 渲染页面树(page));
    切换页面(location.hash.slice(1) || (自我场景窗口模式 ? 'self-scene' : 'thread-status'));
  </script>
</body>
</html>)HTML";

    return 输出.str();
}

// 功能：根据当前输入生成目标数据、场景、动态或回执。
std::string 生成自我场景独立窗口HTML(
    const 结构_控制面板快照& 快照)
{
    return 私有_生成控制面板HTML(
        快照,
        24,
        枚举_控制面板HTML用途::自我场景窗口);
}

// 功能：从 SQL Server 读模型读取数据并生成控制面板 HTML。
std::string 生成SQL控制面板HTML()
{
    结构_SQL控制面板数据 数据{};
    std::string 错误{};
    if (!私有_读取SQL控制面板数据(数据, 错误)) {
        项目运行错误日志("控制面板HTML生成失败 | 来源=ADO-SQL | 原因=" + 错误);
        std::ostringstream 输出;
        输出 << "<!doctype html>\n"
            << 私有_SQL控制面板HTML头部规则备注
            << "<html lang=\"zh-CN\"><head><meta charset=\"utf-8\">"
            << "<title>鱼巢控制面板</title></head><body>"
            << "<h1>鱼巢控制面板</h1><p>SQL 投影读取失败。</p><pre>"
            << 私有_转义HTML(错误)
            << "</pre></body></html>";
        return 输出.str();
    }
    return 私有_生成SQL控制面板HTML(数据);
}

// 功能：生成不预读 SQL 数据的控制面板启动页，后续由页面按区段加载投影数据。
std::string 生成SQL控制面板启动HTML()
{
    结构_SQL控制面板数据 数据{};
    数据.批次 = {
        "待加载",
        "",
        "",
        "数据加载中，等待自我运行写入 SQL 投影后再刷新当前页。"
    };
    return 私有_生成SQL控制面板HTML(数据);
}

// 功能：从 SQL Server 读模型读取数据并保存控制面板 HTML。
bool 保存控制面板HTML(const std::filesystem::path& 输出路径)
{
    const auto HTML = 生成SQL控制面板HTML();
    if (HTML.empty()) {
        项目运行错误日志("控制面板HTML保存失败 | 来源=ADO-SQL | 原因=生成HTML为空");
        return false;
    }

    std::ofstream 输出文件(输出路径, std::ios::binary);
    if (!输出文件) {
        项目运行错误日志("控制面板HTML保存失败 | 来源=ADO-SQL | 原因=无法打开输出文件 | 路径=" + 输出路径.string());
        return false;
    }

    输出文件.write(HTML.data(), static_cast<std::streamsize>(HTML.size()));
    return 输出文件.good();
}

// 功能：启动线程、模块或运行流程。
bool 启动控制面板窗口() noexcept
{
    return 启动控制面板WebView2窗口();
}

// 功能：启动控制面板相机播放独立窗口。
bool 启动控制面板相机窗口() noexcept
{
    return 启动控制面板相机播放窗口();
}

// 功能：请求控制面板相关窗口关闭并等待窗口线程收束。
void 请求关闭控制面板窗口() noexcept
{
    请求关闭控制面板WebView2窗口();
}

// 功能：等待线程、任务、外设或条件变化。
void 等待控制面板窗口关闭() noexcept
{
    等待控制面板WebView2窗口关闭();
}

// 功能：读取并返回指定对象、状态或运行材料。
int 获取控制面板启动诊断码() noexcept
{
    return 获取控制面板WebView2启动诊断码();
}

// 功能：按函数名执行对应处理。
std::filesystem::path 默认控制面板HTML路径()
{
    return std::filesystem::absolute(std::filesystem::path(L"控制面板.html"));
}

// 功能：解析输入文本、消息、场景或运行包。
枚举_控制面板命令 解析控制面板命令行(const int argc, char** argv) noexcept
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
        else if (参数 == "--gap-summary" || 参数 == "--method-gap-summary") {
            输出 = 枚举_控制面板命令::输出缺口摘要;
        }
        else if (参数 == "--demand-tree-growth-summary"
            || 参数 == "--demand-growth-summary"
            || 参数 == "--need-tree-growth-summary") {
            输出 = 枚举_控制面板命令::输出需求树生长摘要;
        }
        else if (参数 == "--completed-task-change-trace"
            || 参数 == "--completed-task-trace"
            || 参数 == "--task-completion-change-trace") {
            输出 = 枚举_控制面板命令::输出完成任务变化跟踪;
        }
        else if (参数 == "--panel-html") {
            输出 = 枚举_控制面板命令::生成HTML;
        }
        else if (参数 == "--panel" || 参数 == "--panel-open") {
            输出 = 枚举_控制面板命令::打开窗口;
        }
        else if (参数 == "--panel-camera" || 参数 == "--camera-window" || 参数 == "--d455-window") {
            输出 = 枚举_控制面板命令::打开相机窗口;
        }
    }
    return 输出;
}
