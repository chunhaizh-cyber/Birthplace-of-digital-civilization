module;

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <TlHelp32.h>

#include "世界树类.h"
#include "语素类.h"
#include "语言类.h"
#include "需求类.h"
#include "任务类.h"
#include "方法类.h"

module 控制面板类;

import 控制面板WebView2;
import 自我类;
import 自我类.特征定义;
import 自我线程模块;
import 控制面板摘要线程模块;
import 任务模块.管理工作线程;
import 任务模块.管理界面线程;
import 任务模块.治理协议;

namespace {
    using 需求节点 = 需求类::节点类;
    using 任务节点 = 任务类::节点类;
    using 方法节点 = 方法类::节点类;
    using 路径集合 = std::unordered_set<std::uintptr_t>;

    constexpr std::uintptr_t 私有_线程详情_自我 = 1;
    constexpr std::uintptr_t 私有_线程详情_工作 = 2;
    constexpr std::uintptr_t 私有_线程详情_系统 = 3;
    constexpr std::uintptr_t 私有_线程详情_摘要 = 4;
    constexpr std::uintptr_t 私有_线程详情_任务界面 = 5;
    constexpr std::size_t 私有_列表分页大小 = 100;

    struct 结构_控制面板缺口承接快照 {
        I64 待处理方法数量 = 0;
        bool 应触发缺口承接 = false;
        bool 应申请重试恢复 = false;
        bool 应申请收束恢复 = false;
        std::size_t 恢复请求数 = 0;
        std::string 最近缺口反馈摘要{};
    };

    结构_控制面板缺口承接快照 私有_读取缺口承接快照_控制面板(
        const 自我类& 自我,
        const 自我线程类& 自我线程) noexcept
    {
        结构_控制面板缺口承接快照 输出{};
        (void)自我.读取自我I64特征当前值(
            自我特征定义类::类型_自我_待处理方法数量(),
            输出.待处理方法数量);

        const auto 恢复快照 = 自我线程.读取最近缺口恢复接口快照();
        输出.应触发缺口承接 = 恢复快照.应触发缺口承接;
        输出.应申请重试恢复 = 恢复快照.应申请重试恢复;
        输出.应申请收束恢复 = 恢复快照.应申请收束恢复;
        输出.恢复请求数 = 恢复快照.恢复请求列表.size();
        输出.最近缺口反馈摘要 = !恢复快照.摘要.empty()
            ? 恢复快照.摘要
            : 恢复快照.恢复请求摘要;
        return 输出;
    }

    const 语素入口节点类* 私有_特征_最近执行时间_控制面板() noexcept
    {
        static const 语素入口节点类* s_词 =
            语素集.添加信息入口词("最近执行时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    const 语素入口节点类* 私有_特征_最近完成时间_控制面板() noexcept
    {
        static const 语素入口节点类* s_词 =
            语素集.添加信息入口词("最近完成时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    时间戳 私有_读取方法运行账时间_控制面板(
        const 方法节点* 方法,
        const 语素入口节点类* 特征类型) noexcept
    {
        if (!方法 || !特征类型) {
            return 0;
        }
        auto* 方法虚拟存在 = 方法->主信息.首节点信息().方法虚拟存在.指针;
        if (!方法虚拟存在 && !方法->主信息.首节点信息().方法虚拟存在.主键.empty()) {
            方法虚拟存在 = reinterpret_cast<存在节点类*>(
                世界树.基础信息().查找主键(方法->主信息.首节点信息().方法虚拟存在.主键));
        }
        I64 值 = 0;
        if (方法虚拟存在
            && 世界树.读取特征_I64(
                reinterpret_cast<const 基础信息节点类*>(方法虚拟存在),
                特征类型,
                值)
            && 值 > 0) {
            return static_cast<时间戳>(值);
        }
        return 0;
    }

    struct 结构_构建上下文 {
        std::size_t 树深度上限 = 0;
        std::size_t 树广度上限 = 0;
        std::size_t 引用展开深度上限 = 2;
        路径集合 世界默认展开路径{};
        路径集合 需求默认展开路径{};
        路径集合 任务默认展开路径{};
        路径集合 方法默认展开路径{};
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
    std::string 私有_截断文本(const std::string& 文本, std::size_t 上限);

    template<class T节点>
    std::uintptr_t 私有_地址(const T节点* 节点) noexcept
    {
        return reinterpret_cast<std::uintptr_t>(节点);
    }

    bool 私有_任务头绑定需求(const 任务节点* 节点) noexcept
    {
        return 节点
            && 任务类::读取任务节点种类(节点) == 枚举_任务节点种类::头结点
            && (节点->主信息.对应需求.指针
                || !节点->主信息.对应需求.主键.empty());
    }

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
            return 世界树.基础信息().世界根();
        }
        else if constexpr (std::is_same_v<节点类型, 需求节点>) {
            return 自我存在 ? 世界树.存在().获取需求根节点(自我存在) : nullptr;
        }
        else if constexpr (std::is_same_v<节点类型, 任务节点>) {
            return 自我存在
                ? reinterpret_cast<const 节点类型*>(世界树.存在().获取任务根节点(自我存在))
                : nullptr;
        }
        else if constexpr (std::is_same_v<节点类型, 方法节点>) {
            return 自我存在
                ? reinterpret_cast<const 节点类型*>(世界树.存在().获取方法根节点(自我存在))
                : nullptr;
        }
        else if constexpr (std::is_same_v<节点类型, 自然句节点类>) {
            return reinterpret_cast<const 节点类型*>(语言集.根指针);
        }
        else if constexpr (std::is_same_v<节点类型, 语素入口节点类>) {
            return reinterpret_cast<const 节点类型*>(语素集.根指针);
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

    const char* 私有_布尔文本(const bool 值) noexcept
    {
        return 值 ? "是" : "否";
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

    std::string 私有_十六进制指针(const std::uintptr_t 值)
    {
        std::ostringstream 输出;
        输出 << "0x" << std::hex << std::uppercase << 值;
        return 输出.str();
    }

    std::string 私有_时间文本(const 时间戳 时间)
    {
        return 时间 == 0 ? "0" : std::to_string(时间);
    }

    std::string 私有_区间文本(const I64区间& 区间)
    {
        return "[" + std::to_string(区间.低值) + ", " + std::to_string(区间.高值) + "]";
    }

    std::string 私有_可选区间文本(const std::optional<I64区间>& 区间)
    {
        return 区间.has_value() ? 私有_区间文本(*区间) : "空";
    }

    std::string 私有_时间段文本(const 结构_时间段& 时间段)
    {
        return "起=" + 私有_时间文本(时间段.起) + " | 止=" + 私有_时间文本(时间段.止);
    }

    std::string 私有_向量文本(const Vector3D& 向量)
    {
        std::ostringstream 输出;
        输出 << "(" << 向量.x << ", " << 向量.y << ", " << 向量.z << ")";
        return 输出.str();
    }

    std::string 私有_词文本(const 语素入口节点类* 词) noexcept
    {
        const auto* 安全词节点 = 私有_解析当前树节点(词);
        if (!安全词节点) {
            return {};
        }
        try {
            return 语素集.获取词(安全词节点);
        }
        catch (...) {
            return {};
        }
    }

    std::string 私有_自然句文本(const 自然句节点类* 句子) noexcept
    {
        const auto* 安全句子节点 = 私有_解析当前树节点(句子);
        if (!安全句子节点) {
            return {};
        }
        return "自然句#" + 安全句子节点->获取主键();
    }

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

    const char* 私有_特征值类型文本(const 特征值& 值) noexcept
    {
        if (std::holds_alternative<std::monostate>(值)) {
            return "空";
        }
        if (std::holds_alternative<I64>(值)) {
            return "i64";
        }
        if (std::holds_alternative<VecU句柄>(值)) {
            return "VecU句柄";
        }
        if (std::holds_alternative<指针句柄>(值)) {
            return "指针句柄";
        }
        return "未定义";
    }

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

    const char* 私有_任务节点种类文本(const 枚举_任务节点种类 种类) noexcept
    {
        switch (种类) {
        case 枚举_任务节点种类::头结点: return "头结点";
        case 枚举_任务节点种类::步骤节点: return "步骤节点";
        default: return "未定义";
        }
    }

    const char* 私有_任务状态文本(const 枚举_任务状态 状态) noexcept
    {
        switch (状态) {
        case 枚举_任务状态::未启动: return "未启动";
        case 枚举_任务状态::运行中: return "运行中";
        case 枚举_任务状态::挂起: return "挂起";
        case 枚举_任务状态::完成: return "完成";
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

    bool 私有_需求状态已达(const 需求节点* 节点) noexcept
    {
        if (!节点) {
            return false;
        }
        const auto* 当前状态 = 节点->主信息.被需求当前状态.指针;
        const auto* 目标状态 = 节点->主信息.需求状态.指针;
        if (当前状态 && 目标状态) {
            const auto 比较 = 世界树.特征().比较状态(当前状态, 目标状态);
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

    std::string 私有_节点主键_控制面板(const 需求节点* 节点)
    {
        return 节点 ? 节点->获取主键() : std::string{};
    }

    std::string 私有_节点主键_控制面板(const 语素入口节点类* 节点)
    {
        return 节点 ? 节点->获取主键() : std::string{};
    }

    void 私有_累计任务状态(结构_控制面板快照& 快照, const 枚举_任务状态 状态) noexcept
    {
        switch (状态) {
        case 枚举_任务状态::未启动: ++快照.任务状态未启动数; break;
        case 枚举_任务状态::运行中: ++快照.任务状态运行中数; break;
        case 枚举_任务状态::挂起: ++快照.任务状态挂起数; break;
        case 枚举_任务状态::完成: ++快照.任务状态完成数; break;
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

    std::string 私有_任务状态数量摘要(const 结构_控制面板快照& 快照)
    {
        std::ostringstream 输出;
        bool 已写 = false;
        私有_追加非零计数(输出, 已写, "未定义", 快照.任务状态未定义数);
        私有_追加非零计数(输出, 已写, "未启动", 快照.任务状态未启动数);
        私有_追加非零计数(输出, 已写, "运行中", 快照.任务状态运行中数);
        私有_追加非零计数(输出, 已写, "挂起", 快照.任务状态挂起数);
        私有_追加非零计数(输出, 已写, "完成", 快照.任务状态完成数);
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

    std::string 私有_需求满足数量摘要(const 结构_控制面板快照& 快照)
    {
        std::ostringstream 输出;
        输出 << "已满足=" << 快照.需求已满足数
            << " | 状态已达未截止=" << 快照.需求状态已达未截止数
            << " | 未满足=" << 快照.需求未满足数;
        return 输出.str();
    }

    struct 结构_方法直接结构计数 {
        std::size_t 条件数 = 0;
        std::size_t 结果数 = 0;
    };

    结构_方法直接结构计数 私有_统计方法直接条件结果(const 方法节点* 方法首节点) noexcept
    {
        结构_方法直接结构计数 计数{};
        if (!方法首节点 || !方法首节点->子) {
            return 计数;
        }
        auto* 首子 = reinterpret_cast<const 方法节点*>(方法首节点->子);
        auto* 当前 = 首子;
        std::size_t 保护 = 0;
        do {
            if (当前->主信息.公共.节点种类 == 枚举_方法节点种类::方法条件节点) {
                ++计数.条件数;
            }
            else if (当前->主信息.公共.节点种类 == 枚举_方法节点种类::方法结果节点) {
                ++计数.结果数;
            }
            当前 = 当前 ? reinterpret_cast<const 方法节点*>(当前->下) : nullptr;
            ++保护;
        } while (当前 && 当前 != 首子 && 保护 < 2048);
        return 计数;
    }

    std::string 私有_方法结构诊断摘要(const 结构_控制面板快照& 快照)
    {
        std::ostringstream 输出;
        输出 << "首节点=" << 快照.方法首节点数
            << " | 条件无结果=" << 快照.方法条件无结果数
            << " | 结果无条件=" << 快照.方法结果无条件数
            << " | 条件结果不成对=" << 快照.方法条件结果不成对数;
        return 输出.str();
    }

    const char* 私有_方法节点种类文本(const 枚举_方法节点种类 种类) noexcept
    {
        switch (种类) {
        case 枚举_方法节点种类::方法首节点: return "方法首节点";
        case 枚举_方法节点种类::方法条件节点: return "方法条件节点";
        case 枚举_方法节点种类::方法结果节点: return "方法结果节点";
        default: return "未定义";
        }
    }

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

    const char* 私有_动作句柄类型文本(const 枚举_动作句柄类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_动作句柄类型::本能函数ID: return "本能函数ID";
        case 枚举_动作句柄类型::外部实现主键: return "外部实现主键";
        case 枚举_动作句柄类型::动作序列主键: return "动作序列主键";
        default: return "未设置";
        }
    }

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

    const char* 私有_状态域文本(const 枚举_状态域 域) noexcept
    {
        switch (域) {
        case 枚举_状态域::内部状态: return "内部状态";
        case 枚举_状态域::世界状态: return "世界状态";
        default: return "未定义";
        }
    }

    const char* 私有_动态来源类型文本(const 枚举_动态来源类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_动态来源类型::方法调用: return "方法调用";
        case 枚举_动态来源类型::内部机制动作: return "内部机制动作";
        default: return "未定义";
        }
    }

    const char* 私有_动态业务语义类型文本(const 枚举_动态业务语义类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_动态业务语义类型::动作事实动态: return "动作事实动态";
        case 枚举_动态业务语义类型::状态变化动态: return "状态变化动态";
        case 枚举_动态业务语义类型::被动记账动态: return "被动记账动态";
        case 枚举_动态业务语义类型::治理过程动态: return "治理过程动态";
        case 枚举_动态业务语义类型::边界结束动态: return "边界结束动态";
        default: return "未定义";
        }
    }

    const char* 私有_动作事件相位文本(const 枚举_动作事件相位 相位) noexcept
    {
        switch (相位) {
        case 枚举_动作事件相位::开始运行: return "开始运行";
        case 枚举_动作事件相位::完成: return "完成";
        case 枚举_动作事件相位::失败: return "失败";
        default: return "未定义";
        }
    }

    const char* 私有_动态聚合方式文本(const 枚举_动态聚合方式 方式) noexcept
    {
        switch (方式) {
        case 枚举_动态聚合方式::原子相邻: return "原子相邻";
        case 枚举_动态聚合方式::连续同向段: return "连续同向段";
        case 枚举_动态聚合方式::同类事件段: return "同类事件段";
        case 枚举_动态聚合方式::窗口聚合: return "窗口聚合";
        case 枚举_动态聚合方式::路径压缩: return "路径压缩";
        default: return "未定义";
        }
    }

    const char* 私有_因果形态文本(const 枚举_因果形态 形态) noexcept
    {
        switch (形态) {
        case 枚举_因果形态::实例: return "实例";
        case 枚举_因果形态::抽象: return "抽象";
        default: return "未定义";
        }
    }

    const char* 私有_因果锚点类型文本(const 枚举_因果锚点类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_因果锚点类型::场景: return "场景";
        case 枚举_因果锚点类型::存在: return "存在";
        default: return "未定义";
        }
    }

    const char* 私有_因果来源类型文本(const 枚举_因果来源类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_因果来源类型::观察生成: return "观察生成";
        case 枚举_因果来源类型::执行生成: return "执行生成";
        case 枚举_因果来源类型::推断生成: return "推断生成";
        default: return "未定义";
        }
    }

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
        case 枚举_信息入口类型::因果实例入口: return "因果实例入口";
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

    std::string 私有_语素入口诊断文本(const 语素入口节点类* 入口节点)
    {
        if (!入口节点 || !入口节点->主信息) {
            return "空";
        }

        const auto* 主信息 = dynamic_cast<const 语素入口主信息类*>(入口节点->主信息);
        if (!主信息) {
            return "非语素入口节点";
        }

        const auto 人类词性 = 主信息->人类词性 != 枚举_词性::未定义
            ? 主信息->人类词性
            : 主信息->词性;
        const auto 信息入口类型 = 主信息->信息入口类型;
        const auto 主信息类型 = 主信息->对应基础信息类型 != 枚举_主信息类型::未定义
            ? 主信息->对应基础信息类型
            : 语素_取信息入口主信息类型(信息入口类型);
        const auto* 默认根名 = 语素_取信息入口默认根名(信息入口类型);

        std::ostringstream 输出;
        输出 << "人类词性=" << 枚举_词性_工厂::根据枚举类型获取文本(人类词性)
            << " | 信息入口=" << 私有_信息入口类型文本(信息入口类型)
            << " | 主信息类型=" << 私有_主信息类型文本(主信息类型)
            << " | 存储树=" << 私有_信息存储树文本(语素_取信息入口存储树(信息入口类型))
            << " | 默认根=" << (默认根名 ? 默认根名 : "空")
            << " | 需上下文=" << (语素_信息入口需要上下文限定(信息入口类型) ? "是" : "否")
            << " | 裸词模板=" << (语素_信息入口裸词默认模板入口(信息入口类型) ? "是" : "否");
        return 输出.str();
    }

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

    std::string 私有_轻引用文本(const 结构_基础信息轻引用& 引用)
    {
        auto* 指针 = 引用.指针 != 0
            ? reinterpret_cast<基础信息节点类*>(引用.指针)
            : nullptr;
        return 私有_引用文本(指针, 引用.主键);
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

    std::string 私有_字段显示文本(
        const std::string& 名称,
        std::string_view 类型,
        const std::string& 值)
    {
        return 名称 + " | 类型=" + std::string(类型) + " | 值=" + (值.empty() ? "空" : 值);
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

    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const std::string& 值)
    {
        父节点.子项.push_back(私有_新节点(私有_字段显示文本(名称, "文本", 值)));
    }

    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const char* 值)
    {
        私有_追加叶字段(父节点, 名称, 值 ? std::string(值) : std::string{});
    }

    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const std::int64_t 值)
    {
        父节点.子项.push_back(私有_新节点(私有_字段显示文本(名称, "i64", std::to_string(值))));
    }

    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const std::uint64_t 值)
    {
        父节点.子项.push_back(私有_新节点(私有_字段显示文本(名称, "u64", std::to_string(值))));
    }

    void 私有_追加叶字段(
        结构_控制面板树节点& 父节点,
        const std::string& 名称,
        const bool 值)
    {
        父节点.子项.push_back(私有_新节点(私有_字段显示文本(名称, "bool", 私有_布尔文本(值))));
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

    void 私有_追加统计字段(结构_控制面板树节点& 父节点, const 结构_统计& 统计)
    {
        auto 统计节点 = 私有_新节点("统计");
        私有_追加叶字段(统计节点, "创建时间", 私有_时间文本(统计.创建时间));
        私有_追加叶字段(统计节点, "最后观测时间", 私有_时间文本(统计.最后观测时间));
        私有_追加叶字段(统计节点, "命中次数", 统计.命中次数);
        私有_追加叶字段(统计节点, "可信度", 统计.可信度);
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
            可展开指针 != nullptr && !展开类型.empty(),
            展开类型);

        if (可展开指针) {
            const auto 地址 = 私有_地址(可展开指针);
            if (路径.contains(地址)) {
                字段节点.可延迟展开 = false;
                字段节点.展开类型.clear();
                字段节点.子项.push_back(私有_新节点("路径=已在上层路径中"));
            }
        }
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

    template<class T节点>
    std::vector<T节点*> 私有_枚举子节点(const T节点* 父节点, const std::size_t 上限)
    {
        std::vector<T节点*> 结果{};
        if (!父节点 || !父节点->子) {
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
        if (!父节点 || !父节点->子) {
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
        if (!根节点->子) {
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

    template<class T节点>
    路径集合 私有_构建父链路径(T节点* 节点)
    {
        路径集合 路径{};
        auto* 当前 = 节点;
        while (当前) {
            const auto 地址 = 私有_地址(当前);
            if (!路径.insert(地址).second) {
                break;
            }
            当前 = reinterpret_cast<T节点*>(当前->父);
        }
        return 路径;
    }

    路径集合 私有_合并路径集合(路径集合 左, const 路径集合& 右)
    {
        左.insert(右.begin(), 右.end());
        return 左;
    }

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

    std::string 私有_基础信息子层摘要(const 基础信息节点类* 节点)
    {
        std::vector<std::pair<std::string, std::size_t>> 统计{};
        私有_遍历全部子节点(节点, [&](const 基础信息节点类* 子节点) {
            私有_累计文本计数(统计, 私有_基础信息类别文本(子节点 ? 子节点->主信息 : nullptr));
        });
        return 私有_文本计数摘要(std::move(统计));
    }

    std::string 私有_需求子层摘要(const 需求节点* 节点)
    {
        std::vector<std::pair<std::string, std::size_t>> 统计{};
        私有_遍历全部子节点(节点, [&](const 需求节点* 子节点) {
            auto 标签 = 私有_词文本(子节点->主信息.类型);
            if (标签.empty()) {
                标签 = "空类型";
            }
            私有_累计文本计数(统计, std::move(标签));
        });
        return 私有_文本计数摘要(std::move(统计));
    }

    std::string 私有_任务子层摘要(const 任务节点* 节点)
    {
        std::vector<std::pair<std::string, std::size_t>> 统计{};
        私有_遍历全部子节点(节点, [&](const 任务节点* 子节点) {
            私有_累计文本计数(统计, 私有_任务节点种类文本(任务类::读取任务节点种类(子节点)));
        });
        return 私有_文本计数摘要(std::move(统计));
    }

    std::string 私有_方法子层摘要(const 方法节点* 节点)
    {
        std::vector<std::pair<std::string, std::size_t>> 统计{};
        私有_遍历全部子节点(节点, [&](const 方法节点* 子节点) {
            私有_累计文本计数(统计, 私有_方法节点种类文本(子节点->主信息.节点种类()));
        });
        return 私有_文本计数摘要(std::move(统计));
    }

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

    struct 结构_需求方法可用性评估 {
        const 语素入口节点类* 目标特征类型 = nullptr;
        std::vector<方法节点*> 候选方法{};
        std::vector<方法节点*> 可用方法{};
        std::vector<方法节点*> 同向候选方法{};
        std::vector<方法节点*> 可用同向方法{};
        std::size_t 有动作骨架候选数 = 0;
        std::size_t 方向未明候选数 = 0;
        std::size_t 反向候选数 = 0;
        bool 有需求状态引用 = false;
        bool 需求状态已满足 = false;
        int 需求方向 = 2;
        枚举_需求方法可用性状态 状态 = 枚举_需求方法可用性状态::无需求状态;
    };

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

    bool 私有_语素入口相同_控制面板(const 语素入口节点类* 左, const 语素入口节点类* 右) noexcept
    {
        if (左 == 右) {
            return true;
        }
        if (!左 || !右) {
            return false;
        }
        return 左->获取主键() == 右->获取主键();
    }

    const 语素入口节点类* 私有_查找人类词性入口_控制面板(
        const std::string& 词值,
        const std::string& 词性值) noexcept
    {
        if (词值.empty() || 词性值.empty()) {
            return nullptr;
        }
        auto* 词节点 = 语素集.查找词节点(词值);
        return 词节点 ? 语素集.查找人类词性入口节点(词节点, 词性值) : nullptr;
    }

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
            return reinterpret_cast<T节点*>(世界树.基础信息().查找主键(引用.主键));
        }
        return nullptr;
    }

    const 语素入口节点类* 私有_状态特征类型_控制面板(const 状态节点类* 状态节点) noexcept
    {
        const auto* 状态主信息 = 状态节点 ? 世界树.状态().取状态主信息(状态节点) : nullptr;
        if (!状态主信息) {
            return nullptr;
        }

        auto* 特征节点 = 状态主信息->状态特征.指针;
        if (!特征节点 && !状态主信息->状态特征.主键.empty()) {
            特征节点 = reinterpret_cast<特征节点类*>(
                世界树.基础信息().查找主键(状态主信息->状态特征.主键));
        }
        const auto* 特征主信息 = 特征节点 ? 世界树.特征().取特征主信息(特征节点) : nullptr;
        return 特征主信息 ? 特征主信息->类型 : nullptr;
    }

    const 语素入口节点类* 私有_需求目标特征类型_控制面板(const 需求节点* 节点) noexcept
    {
        if (!节点) {
            return nullptr;
        }
        auto* 目标状态 = 私有_解析基础信息引用_控制面板(节点->主信息.需求状态);
        return 私有_状态特征类型_控制面板(目标状态);
    }

    bool 私有_状态I64值_控制面板(const 状态节点类* 状态节点, I64& 输出值) noexcept
    {
        const auto* 状态主信息 = 状态节点 ? 世界树.状态().取状态主信息(状态节点) : nullptr;
        if (!状态主信息) {
            return false;
        }
        if (const auto* 标量 = std::get_if<I64>(&状态主信息->状态值)) {
            输出值 = *标量;
            return true;
        }
        return false;
    }

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
            || !私有_语素入口相同_控制面板(当前特征类型, 目标特征类型)) {
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

    void 私有_追加唯一方法_控制面板(std::vector<方法节点*>& 列表, 方法节点* 方法) noexcept
    {
        if (!方法) {
            return;
        }
        if (std::find(列表.begin(), 列表.end(), 方法) == 列表.end()) {
            列表.push_back(方法);
        }
    }

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

        if (当前方法节点->主信息.公共.节点种类 == 枚举_方法节点种类::方法首节点
            && 当前方法节点->主信息.有动作骨架()
            && 当前方法节点->主信息.首节点信息().能力.可被方法查找命中
            && !当前方法节点->主信息.首节点信息().能力.是否根写入原语
            && std::any_of(
                当前方法节点->主信息.首节点信息().能力.结果包.结果项集.begin(),
                当前方法节点->主信息.首节点信息().能力.结果包.结果项集.end(),
                [&](const 结构_方法结果项& 结果项) {
                    return 私有_语素入口相同_控制面板(结果项.特征类型, 目标特征类型);
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
        if (auto* 方法根 = 被需求存在 ? 世界树.存在().获取方法根节点(被需求存在) : nullptr) {
            私有_追加唯一方法_控制面板(方法根集, reinterpret_cast<方法节点*>(方法根));
        }
        if (auto* 自我存在 = 自我.获取自我存在()) {
            if (auto* 方法根 = 世界树.存在().获取方法根节点(自我存在)) {
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

    std::optional<int> 私有_二次特征方向_控制面板(const 二次特征节点类* 二次特征节点) noexcept
    {
        const auto* 主信息 = 二次特征节点
            ? 世界树.基础信息().取主信息<二次特征主信息类>(二次特征节点)
            : nullptr;
        if (!主信息) {
            return std::nullopt;
        }
        if (主信息->差值.has_value()) {
            if (*主信息->差值 > 0) {
                return 1;
            }
            if (*主信息->差值 < 0) {
                return -1;
            }
            return 0;
        }
        switch (主信息->方向区间) {
        case 二次特征主信息类::枚举_方向区间::正区间:
            return 1;
        case 二次特征主信息类::枚举_方向区间::负区间:
            return -1;
        case 二次特征主信息类::枚举_方向区间::零区间:
            return 0;
        default:
            break;
        }
        return std::nullopt;
    }

    std::optional<int> 私有_结果变化方向_控制面板(枚举_结果变化方向 方向) noexcept
    {
        switch (方向) {
        case 枚举_结果变化方向::增加:
        case 枚举_结果变化方向::趋向目标:
        case 枚举_结果变化方向::从无到有:
        case 枚举_结果变化方向::从未完成到完成:
        case 枚举_结果变化方向::已形成:
        case 枚举_结果变化方向::已识别:
        case 枚举_结果变化方向::已配对:
        case 枚举_结果变化方向::趋向可用:
        case 枚举_结果变化方向::更完整:
        case 枚举_结果变化方向::更稳定:
        case 枚举_结果变化方向::上升:
            return 1;
        case 枚举_结果变化方向::减少:
        case 枚举_结果变化方向::从有到无:
            return -1;
        case 枚举_结果变化方向::保持:
            return 0;
        default:
            return std::nullopt;
        }
    }

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

        const auto& 能力 = 方法->主信息.首节点信息().能力;
        if (!能力.可被方法查找命中 || 能力.是否根写入原语) {
            return;
        }
        for (const auto& 结果项 : 能力.结果包.结果项集) {
            if (const auto 方向 = 私有_结果变化方向_控制面板(结果项.方向)) {
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

    bool 私有_方法当前可用_控制面板(const 方法节点* 方法) noexcept
    {
        if (!方法) {
            return false;
        }
        const auto* 方法是否可用 = 私有_方法是否可用特征类型_控制面板();
        if (!方法是否可用) {
            return false;
        }

        auto* 方法虚拟存在 = 方法->主信息.首节点信息().方法虚拟存在.指针;
        if (!方法虚拟存在 && !方法->主信息.首节点信息().方法虚拟存在.主键.empty()) {
            方法虚拟存在 = reinterpret_cast<存在节点类*>(
                世界树.基础信息().查找主键(方法->主信息.首节点信息().方法虚拟存在.主键));
        }
        I64 可用值 = 0;
        return 方法虚拟存在
            && 世界树.读取特征_I64(
                reinterpret_cast<const 基础信息节点类*>(方法虚拟存在),
                方法是否可用,
                可用值)
            && 可用值 != 0;
    }

    结构_需求方法可用性评估 私有_评估需求方法可用性_控制面板(const 需求节点* 节点) noexcept
    {
        结构_需求方法可用性评估 评估{};
        if (!节点) {
            return 评估;
        }

        评估.有需求状态引用 = 节点->主信息.需求状态.指针
            || !节点->主信息.需求状态.主键.empty();
        auto* 当前状态 = reinterpret_cast<状态节点类*>(
            私有_解析基础信息引用_控制面板(节点->主信息.被需求当前状态));
        auto* 目标状态 = reinterpret_cast<状态节点类*>(
            私有_解析基础信息引用_控制面板(节点->主信息.需求状态));
        if (当前状态 && 目标状态) {
            const auto 比较 = 世界树.特征().比较状态(
                当前状态,
                目标状态);
            评估.需求状态已满足 = 比较.可比较
                && 三向关系被接受(比较.关系, 节点->主信息.满足关系);
        }
        评估.目标特征类型 = 私有_需求目标特征类型_控制面板(节点);
        评估.需求方向 = 私有_需求方向_控制面板(节点);

        if (!评估.有需求状态引用) {
            评估.状态 = 枚举_需求方法可用性状态::无需求状态;
            return 评估;
        }
        if (!评估.目标特征类型) {
            评估.状态 = 枚举_需求方法可用性状态::目标特征未解析;
            return 评估;
        }
        if (评估.需求状态已满足) {
            评估.状态 = 枚举_需求方法可用性状态::已满足;
            return 评估;
        }
        if (评估.需求方向 == 2) {
            评估.状态 = 枚举_需求方法可用性状态::需求方向未解析;
        }

        评估.候选方法 = 私有_查找需求候选方法_控制面板(节点, 评估.目标特征类型);
        for (auto* 方法 : 评估.候选方法) {
            if (方法 && 方法->主信息.有动作骨架()) {
                ++评估.有动作骨架候选数;
            }
            const auto 方向关系 = 私有_方法相对需求方向_控制面板(方法, 评估.需求方向);
            if (方向关系 > 0) {
                评估.同向候选方法.push_back(方法);
            }
            else if (方向关系 < 0) {
                ++评估.反向候选数;
            }
            else {
                ++评估.方向未明候选数;
            }
            if (私有_方法当前可用_控制面板(方法)) {
                评估.可用方法.push_back(方法);
                if (方向关系 > 0) {
                    评估.可用同向方法.push_back(方法);
                }
            }
        }

        if (评估.候选方法.empty()) {
            评估.状态 = 枚举_需求方法可用性状态::无候选方法;
        }
        else if (评估.可用方法.empty()) {
            评估.状态 = 枚举_需求方法可用性状态::候选不可用;
        }
        else if (评估.需求方向 != 2 && 评估.可用同向方法.empty()) {
            评估.状态 = 枚举_需求方法可用性状态::无可用同向方法;
        }
        else if (评估.需求方向 != 2 && !评估.可用同向方法.empty()) {
            评估.状态 = 枚举_需求方法可用性状态::有可用同向方法;
        }
        else if (评估.需求方向 == 2) {
            评估.状态 = 枚举_需求方法可用性状态::需求方向未解析;
        }
        else {
            评估.状态 = 枚举_需求方法可用性状态::有可用方法;
        }
        return 评估;
    }

    std::string 私有_需求标题(const 需求节点* 节点)
    {
        if (!节点) {
            return "空需求";
        }

        const auto 类型文本 = 私有_词文本(节点->主信息.类型);
        std::ostringstream 输出;
        输出 << "需求"
            << " | 类型=" << (类型文本.empty() ? "空" : 类型文本)
            << " | 安全=" << 节点->主信息.安全权重
            << " | 服务=" << 节点->主信息.服务权重
            << " | 子=" << static_cast<std::size_t>(节点->子节点数量)
            << " | 主键=" << 节点->获取主键();
        if (const auto 子层摘要 = 私有_需求子层摘要(节点); !子层摘要.empty()) {
            输出 << " | 子类型=" << 子层摘要;
        }
        const auto 方法评估 = 私有_评估需求方法可用性_控制面板(节点);
        if (方法评估.有需求状态引用 || 方法评估.目标特征类型) {
            输出 << " | 方法=" << 私有_需求方法可用性状态文本(方法评估.状态);
            if (方法评估.目标特征类型) {
                输出 << " | 目标特征=" << 私有_词文本(方法评估.目标特征类型);
            }
            输出 << " | 方向=" << 私有_方向文本_控制面板(方法评估.需求方向);
            if (!方法评估.候选方法.empty() || !方法评估.可用方法.empty()) {
                输出 << " | 候选=" << 方法评估.候选方法.size()
                    << " | 可用=" << 方法评估.可用方法.size()
                    << " | 可用同向=" << 方法评估.可用同向方法.size();
            }
        }
        return 输出.str();
    }

    std::string 私有_需求结构标题(const 需求节点* 节点)
    {
        if (!节点) {
            return "空需求";
        }

        const auto 类型文本 = 私有_词文本(节点->主信息.类型);
        const bool 是结构根 = 节点->父 == nullptr;
        const bool 是根需求 = 节点->父 != nullptr && 节点->父->父 == nullptr;
        std::ostringstream 输出;
        输出 << (是结构根 ? "需求树根" : (是根需求 ? "根需求" : "子需求"))
            << " | " << (类型文本.empty() ? "空" : 类型文本)
            << " | 子=" << static_cast<std::size_t>(节点->子节点数量)
            << " | 主键=" << 节点->获取主键();
        return 输出.str();
    }

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

    std::string 私有_任务结构标题(const 任务节点* 节点)
    {
        if (!节点) {
            return "空任务";
        }

        const auto 名称 = 私有_词文本(节点->主信息.名称);
        const bool 是结构根 = 节点->父 == nullptr;
        std::ostringstream 输出;
        输出 << (是结构根 ? "任务根节点" : "任务")
            << " | " << (名称.empty() ? 节点->获取主键() : 名称)
            << " | 子=" << static_cast<std::size_t>(节点->子节点数量)
            << " | 主键=" << 节点->获取主键();
        return 输出.str();
    }

    std::string 私有_方法标题(const 方法节点* 节点)
    {
        if (!节点) {
            return "空方法";
        }

        std::string 标题 = 私有_词文本(节点->主信息.首节点信息().动作名);
        if (标题.empty()) {
            标题 = 私有_词文本(节点->主信息.首个结果能力特征类型());
        }
        if (标题.empty()) {
            标题 = 节点->获取主键();
        }

        std::ostringstream 输出;
        输出 << 私有_方法节点种类文本(节点->主信息.节点种类())
            << " | " << 标题
            << " | 来源=" << 私有_方法来源文本(节点->主信息.首节点信息().来源)
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
        return 私有_新节点(
            私有_需求标题(节点),
            私有_地址(节点),
            默认展开,
            节点 != nullptr,
            "need-node");
    }

    结构_控制面板树节点 私有_需求结构骨架节点(
        需求节点* 节点,
        const 结构_构建上下文& 上下文,
        const bool 默认展开 = false)
    {
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
        if (节点 && 节点->主信息.公共.节点种类 == 枚举_方法节点种类::方法首节点) {
            const auto 计数 = 私有_统计方法直接条件结果(节点);
            if (计数.条件数 > 0 && 计数.结果数 == 0) {
                文本 += " | 结构异常=有条件无结果";
            }
            else if (计数.结果数 > 0 && 计数.条件数 == 0) {
                文本 += " | 结构异常=有结果无条件";
            }
            else if (计数.条件数 != 计数.结果数) {
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
        私有_追加叶字段(字段节点, "子节点数量", 节点->子节点数量);
        私有_追加指针字段(
            字段节点,
            "父节点（上级节点）",
            reinterpret_cast<基础信息节点类*>(节点->父),
            {},
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加指针字段(
            字段节点,
            "首子节点",
            reinterpret_cast<基础信息节点类*>(节点->子),
            {},
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加指针字段(
            字段节点,
            "上同层节点",
            reinterpret_cast<基础信息节点类*>(节点->上),
            {},
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加指针字段(
            字段节点,
            "下同层节点",
            reinterpret_cast<基础信息节点类*>(节点->下),
            {},
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });

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

    void 私有_追加抽象特征节点字段(
        结构_控制面板树节点& 字段节点,
        const 抽象特征主信息类& 主信息)
    {
        私有_追加叶字段(字段节点, "区间", 私有_可选区间文本(主信息.区间));
        私有_追加叶字段(字段节点, "组合规则数量", static_cast<std::uint64_t>(主信息.组合规则.size()));
        私有_追加叶字段(字段节点, "生命周期", static_cast<std::uint64_t>(主信息.生命周期));
        私有_追加叶字段(字段节点, "重叠命中次数", 主信息.重叠命中次数);
        私有_追加叶字段(字段节点, "最近重叠命中时间", 私有_时间文本(主信息.最近重叠命中时间));
        私有_追加叶字段(
            字段节点,
            "来源实例特征",
            私有_引用文本(主信息.来源实例特征.指针, 主信息.来源实例特征.主键));
        私有_追加叶字段(字段节点, "升格时间", 私有_时间文本(主信息.升格时间));

        auto 规则节点 = 私有_新节点("组合规则");
        for (std::size_t i = 0; i < 主信息.组合规则.size(); ++i) {
            const auto& 规则 = 主信息.组合规则[i];
            auto 单规则节点 = 私有_新节点(
                std::to_string(i) + ": " + (规则.规则名称.empty() ? "未命名规则" : 规则.规则名称));
            私有_追加叶字段(单规则节点, "规则主键", 规则.规则主键);
            私有_追加叶字段(单规则节点, "输入数量", static_cast<std::uint64_t>(规则.输入.size()));
            私有_追加叶字段(单规则节点, "组合方式", static_cast<std::uint64_t>(规则.组合方式));
            私有_追加叶字段(单规则节点, "最少命中数", static_cast<std::uint64_t>(规则.最少命中数));
            私有_追加叶字段(单规则节点, "规则版本", static_cast<std::uint64_t>(规则.规则版本));
            私有_追加叶字段(单规则节点, "优先级", static_cast<std::int64_t>(规则.优先级));
            私有_追加叶字段(单规则节点, "已启用", 规则.已启用);
            私有_追加叶字段(单规则节点, "命中次数", 规则.命中次数);
            私有_追加叶字段(单规则节点, "失败次数", 规则.失败次数);
            私有_追加叶字段(单规则节点, "最近命中时间", 私有_时间文本(规则.最近命中时间));
            私有_追加叶字段(单规则节点, "最近失败时间", 私有_时间文本(规则.最近失败时间));

            auto 输入节点 = 私有_新节点("输入");
            for (std::size_t 输入索引 = 0; 输入索引 < 规则.输入.size(); ++输入索引) {
                const auto& 输入 = 规则.输入[输入索引];
                auto 单输入节点 = 私有_新节点(
                    std::to_string(输入索引) + ": " + (输入.角色键.empty() ? "未命名输入" : 输入.角色键));
                私有_追加叶字段(单输入节点, "来源抽象特征", 私有_轻引用文本(输入.来源抽象特征));
                私有_追加叶字段(单输入节点, "期望抽象状态", 私有_轻引用文本(输入.期望抽象状态));
                私有_追加叶字段(单输入节点, "期望区间", 私有_可选区间文本(输入.期望区间));
                私有_追加叶字段(单输入节点, "角色", static_cast<std::uint64_t>(输入.角色));
                私有_追加叶字段(单输入节点, "缺失策略", static_cast<std::uint64_t>(输入.缺失策略));
                私有_追加叶字段(单输入节点, "默认值", 输入.默认值);
                私有_追加叶字段(单输入节点, "权重", 输入.权重);
                私有_追加叶字段(单输入节点, "方向", static_cast<std::int64_t>(输入.方向));
                私有_追加叶字段(单输入节点, "输入序号", static_cast<std::uint64_t>(输入.输入序号));
                输入节点.子项.push_back(std::move(单输入节点));
            }
            单规则节点.子项.push_back(std::move(输入节点));

            auto 输出节点 = 私有_新节点("输出");
            私有_追加叶字段(输出节点, "输出策略", static_cast<std::uint64_t>(规则.输出.输出策略));
            私有_追加叶字段(输出节点, "成立区间", 私有_可选区间文本(规则.输出.成立区间));
            私有_追加叶字段(输出节点, "成立输出值", 规则.输出.成立输出值);
            私有_追加叶字段(输出节点, "不成立输出值", 规则.输出.不成立输出值);
            私有_追加叶字段(输出节点, "输出值限制", 私有_可选区间文本(规则.输出.输出值限制));
            单规则节点.子项.push_back(std::move(输出节点));
            规则节点.子项.push_back(std::move(单规则节点));
        }
        字段节点.子项.push_back(std::move(规则节点));
    }

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
        私有_追加叶字段(字段节点, "有位置历史", 主信息.有位置历史);
        私有_追加叶字段(字段节点, "上次观测位置", 私有_向量文本(主信息.上次观测位置_mm));
        私有_追加叶字段(字段节点, "最近观测位置", 私有_向量文本(主信息.最近观测位置_mm));
        私有_追加叶字段(字段节点, "最近位移_mm", 主信息.最近位移_mm);
        私有_追加叶字段(字段节点, "连续静止帧", static_cast<std::uint64_t>(主信息.连续静止帧));
        私有_追加叶字段(字段节点, "连续未命中帧", static_cast<std::uint64_t>(主信息.连续未命中帧));
        私有_追加引用列表字段(
            字段节点,
            "实例因果",
            主信息.实例因果,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
    }

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
        私有_追加引用列表字段(
            字段节点,
            "状态索引",
            主信息.状态索引,
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
            主信息.动态索引,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        私有_追加引用列表字段(
            字段节点,
            "关系索引",
            主信息.关系索引,
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
            主信息.二次特征索引,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        私有_追加引用列表字段(
            字段节点,
            "实例因果索引",
            主信息.实例因果索引,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
    }

    void 私有_追加状态节点字段(
        结构_控制面板树节点& 字段节点,
        const 状态节点主信息类& 主信息,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径)
    {
        私有_追加叶字段(字段节点, "状态域", 私有_状态域文本(主信息.状态域));
        私有_追加叶字段(字段节点, "收到时间", 私有_时间文本(主信息.收到时间));
        私有_追加叶字段(字段节点, "发生时间", 私有_时间文本(主信息.发生时间));
        私有_追加叶字段(字段节点, "状态值", 私有_特征值文本(主信息.状态值));
        私有_追加叶字段(字段节点, "是否变化", 主信息.是否变化);
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

    void 私有_追加动态节点字段(
        结构_控制面板树节点& 字段节点,
        const 动态节点主信息类& 主信息,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径)
    {
        私有_追加叶字段(字段节点, "时间段", 私有_时间段文本(主信息.时间));
        私有_追加叶字段(字段节点, "来源类型", 私有_动态来源类型文本(主信息.来源类型));
        私有_追加叶字段(字段节点, "业务语义类型", 私有_动态业务语义类型文本(主信息.业务语义类型));
        私有_追加叶字段(字段节点, "来源动作名", 私有_词文本(主信息.来源动作名));
        私有_追加叶字段(字段节点, "来源动作相位", 私有_动作事件相位文本(主信息.来源动作相位));
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
        私有_追加叶字段(字段节点, "置信度", 主信息.置信度);
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

    void 私有_追加因果字段(
        结构_控制面板树节点& 字段节点,
        const 因果主信息类& 主信息,
        const 结构_构建上下文& 上下文,
        const std::size_t 剩余深度,
        const 路径集合& 路径)
    {
        私有_追加叶字段(字段节点, "形态", 私有_因果形态文本(主信息.形态));
        私有_追加叶字段(字段节点, "适用锚点类型", 私有_因果锚点类型文本(主信息.适用锚点类型));
        私有_追加叶字段(字段节点, "适用层级", static_cast<std::uint64_t>(主信息.适用层级));
        私有_追加叶字段(字段节点, "动作名称", 主信息.动作名称);
        私有_追加叶字段(字段节点, "动作语义键", 主信息.动作语义键);
        私有_追加叶字段(字段节点, "来源类型", 私有_因果来源类型文本(主信息.来源类型));
        私有_追加叶字段(字段节点, "置信度", 主信息.置信度);
        私有_追加叶字段(字段节点, "已验证", 主信息.已验证);
        私有_追加叶字段(字段节点, "成立次数", 主信息.成立次数);
        私有_追加叶字段(字段节点, "失败次数", 主信息.失败次数);
        私有_追加叶字段(字段节点, "最近命中时间", 私有_时间文本(主信息.最近命中时间));
        私有_追加叶字段(字段节点, "最近失败时间", 私有_时间文本(主信息.最近失败时间));
        私有_追加引用字段(
            字段节点,
            "动作模板",
            主信息.动作模板,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用列表字段(
            字段节点,
            "条件模板",
            主信息.条件模板,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        私有_追加引用列表字段(
            字段节点,
            "状态迁移模板",
            主信息.状态迁移模板,
            上下文,
            剩余深度,
            路径,
            [](基础信息节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建基础信息树节点(目标, 局部上下文, 深度, std::move(路径集));
            },
            上下文.树广度上限);
        私有_追加引用字段(
            字段节点,
            "主状态迁移",
            主信息.主状态迁移,
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

        const auto 子节点集 = 世界树.基础信息().枚举子节点(节点);
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
        私有_追加叶字段(字段节点, "主键", 节点->获取主键());
        私有_追加叶字段(字段节点, "节点地址", 私有_十六进制指针(私有_地址(节点)));
        私有_追加叶字段(字段节点, "子节点数量", 节点->子节点数量);
        私有_追加叶字段(字段节点, "描述信息", 私有_自然句文本(节点->主信息.描述信息));
        私有_追加叶字段(字段节点, "类型", 私有_词文本(节点->主信息.类型));
        私有_追加叶字段(字段节点, "需求有效截止", 私有_时间文本(节点->主信息.需求有效截止));
        私有_追加叶字段(字段节点, "安全权重", 节点->主信息.安全权重);
        私有_追加叶字段(字段节点, "服务权重", 节点->主信息.服务权重);
        私有_追加叶字段(字段节点, "派生来源方法主键", 节点->主信息.派生来源方法主键);
        私有_追加叶字段(字段节点, "派生需求类型抽象特征指针", 私有_十六进制指针(节点->主信息.派生需求类型抽象特征指针));
        私有_追加叶字段(字段节点, "派生需求类型值", 节点->主信息.派生需求类型值);
        私有_追加叶字段(字段节点, "派生方法需求位抽象特征指针", 私有_十六进制指针(节点->主信息.派生方法需求位抽象特征指针));
        私有_追加叶字段(字段节点, "派生方法需求位值", 节点->主信息.派生方法需求位值);
        私有_追加叶字段(字段节点, "派生本能能力缺口类型抽象特征指针", 私有_十六进制指针(节点->主信息.派生本能能力缺口类型抽象特征指针));
        私有_追加叶字段(字段节点, "派生本能能力缺口类型值", 节点->主信息.派生本能能力缺口类型值);
        const auto 方法评估 = 私有_评估需求方法可用性_控制面板(节点);
        私有_追加叶字段(字段节点, "方法可用性", 私有_需求方法可用性状态文本(方法评估.状态));
        私有_追加叶字段(字段节点, "目标特征类型", 私有_词文本(方法评估.目标特征类型));
        私有_追加叶字段(字段节点, "需求方向", 私有_方向文本_控制面板(方法评估.需求方向));
        私有_追加叶字段(字段节点, "候选方法数量", static_cast<std::uint64_t>(方法评估.候选方法.size()));
        私有_追加叶字段(字段节点, "可用方法数量", static_cast<std::uint64_t>(方法评估.可用方法.size()));
        私有_追加叶字段(字段节点, "同向候选方法数量", static_cast<std::uint64_t>(方法评估.同向候选方法.size()));
        私有_追加叶字段(字段节点, "可用同向方法数量", static_cast<std::uint64_t>(方法评估.可用同向方法.size()));
        私有_追加叶字段(字段节点, "方向未明候选方法数量", static_cast<std::uint64_t>(方法评估.方向未明候选数));
        私有_追加叶字段(字段节点, "反向候选方法数量", static_cast<std::uint64_t>(方法评估.反向候选数));
        私有_追加叶字段(字段节点, "有动作骨架候选方法数量", static_cast<std::uint64_t>(方法评估.有动作骨架候选数));
        私有_追加叶字段(字段节点, "需求状态已满足", 方法评估.需求状态已满足);
        auto 候选方法列表节点 = 私有_新节点("候选方法 (" + std::to_string(方法评估.候选方法.size()) + ")");
        const auto 候选方法实际上限 = (std::min)(上下文.树广度上限, 方法评估.候选方法.size());
        for (std::size_t 索引 = 0; 索引 < 候选方法实际上限; ++索引) {
            auto* 方法 = 方法评估.候选方法[索引];
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
        if (方法评估.候选方法.size() > 候选方法实际上限) {
            候选方法列表节点.子项.push_back(
                私有_新节点("... 省略 " + std::to_string(方法评估.候选方法.size() - 候选方法实际上限) + " 项"));
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
        私有_追加叶字段(字段节点, "主键", 节点->获取主键());
        私有_追加叶字段(字段节点, "节点地址", 私有_十六进制指针(私有_地址(节点)));
        私有_追加叶字段(字段节点, "子节点数量", 节点->子节点数量);
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
            "结果状态信息",
            节点->主信息.结果状态信息,
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
        私有_追加叶字段(字段节点, "主键", 节点->获取主键());
        私有_追加叶字段(字段节点, "节点地址", 私有_十六进制指针(私有_地址(节点)));
        私有_追加叶字段(字段节点, "子节点数量", 节点->子节点数量);
        if (节点->主信息.公共.节点种类 == 枚举_方法节点种类::方法首节点) {
            const auto 计数 = 私有_统计方法直接条件结果(节点);
            私有_追加叶字段(字段节点, "直接条件节点数量", static_cast<I64>(计数.条件数));
            私有_追加叶字段(字段节点, "直接结果节点数量", static_cast<I64>(计数.结果数));
            私有_追加叶字段(
                字段节点,
                "条件结果是否成对",
                计数.条件数 == 计数.结果数);
        }
        const auto* 首信息 = 节点->主信息.取首节点信息();
        const auto* 条件信息 = 节点->主信息.取条件节点信息();
        const auto* 结果信息 = 节点->主信息.取结果节点信息();
        私有_追加叶字段(字段节点, "描述信息", 私有_自然句文本(节点->主信息.公共.描述信息));
        私有_追加叶字段(字段节点, "节点种类", 私有_方法节点种类文本(节点->主信息.节点种类()));
        私有_追加叶字段(字段节点, "动作名", 私有_词文本(首信息 ? 首信息->动作名 : nullptr));
        私有_追加叶字段(字段节点, "动作句柄", 首信息 ? 私有_动作句柄文本(首信息->动作句柄) : std::string{});
        私有_追加叶字段(字段节点, "来源", 首信息 ? 私有_方法来源文本(首信息->来源) : std::string{});
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
        私有_追加叶字段(字段节点, "首个结果能力特征类型", 私有_词文本(节点->主信息.首个结果能力特征类型()));
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
            首信息 ? 首信息->方法虚拟存在 : 可解析引用<存在节点类>{},
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
            首信息 ? 首信息->来源任务 : 可解析引用<任务节点类>{},
            上下文,
            剩余深度,
            路径,
            [](任务节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建任务树节点(reinterpret_cast<任务节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "父方法",
            首信息 ? 首信息->父方法 : 可解析引用<方法节点类>{},
            上下文,
            剩余深度,
            路径,
            [](方法节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建方法树节点(reinterpret_cast<方法节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "前置方法",
            首信息 ? 首信息->前置方法 : 可解析引用<方法节点类>{},
            上下文,
            剩余深度,
            路径,
            [](方法节点类* 目标, const 结构_构建上下文& 局部上下文, std::size_t 深度, 路径集合 路径集) {
                return 私有_构建方法树节点(reinterpret_cast<方法节点*>(目标), 局部上下文, 深度, std::move(路径集));
            });
        私有_追加引用字段(
            字段节点,
            "后续方法",
            首信息 ? 首信息->后续方法 : 可解析引用<方法节点类>{},
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
            !节点->父 || 上下文.世界默认展开路径.contains(私有_地址(节点)));

        const auto 子节点集 = 世界树.基础信息().枚举子节点(节点);
        const auto 实际上限 = (std::min)(上下文.树广度上限, 子节点集.size());
        for (std::size_t 索引 = 0; 索引 < 实际上限; ++索引) {
            auto* 子节点 = 子节点集[索引];
            if (!子节点 || !上下文.世界默认展开路径.contains(私有_地址(子节点))) {
                continue;
            }
            树节点.子项.push_back(私有_构建基础信息根链骨架(子节点, 上下文));
        }
        return 树节点;
    }

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
            !节点->父 || 节点->父->父 == nullptr || 上下文.需求默认展开路径.contains(私有_地址(节点)));

        const auto 子节点集 = 私有_枚举子节点(节点, 上下文.树广度上限);
        for (auto* 子节点 : 子节点集) {
            if (!子节点) {
                continue;
            }
            if (!节点->父 || 上下文.需求默认展开路径.contains(私有_地址(子节点))) {
                树节点.子项.push_back(私有_构建需求根链骨架(子节点, 上下文));
            }
            else {
                树节点.子项.push_back(私有_需求结构骨架节点(子节点, 上下文, false));
            }
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
            !节点->父 || 上下文.任务默认展开路径.contains(私有_地址(节点)));

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
            if (!节点->父 || 上下文.任务默认展开路径.contains(私有_地址(子节点))) {
                树节点.子项.push_back(私有_构建任务根链骨架(子节点, 上下文));
            }
            else {
                树节点.子项.push_back(私有_任务骨架节点(子节点, 上下文, false));
            }
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
            !节点->父 || 上下文.方法默认展开路径.contains(私有_地址(节点)));

        const auto 子节点集 = 私有_枚举子节点(节点, 上下文.树广度上限);
        for (auto* 子节点 : 子节点集) {
            if (!子节点) {
                continue;
            }
            if (!节点->父 || 上下文.方法默认展开路径.contains(私有_地址(子节点))) {
                树节点.子项.push_back(私有_构建方法根链骨架(子节点, 上下文));
            }
            else {
                树节点.子项.push_back(私有_方法骨架节点(子节点, 上下文, false));
            }
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

    std::vector<THREADENTRY32> 私有_枚举当前进程线程()
    {
        std::vector<THREADENTRY32> 线程集{};
        HANDLE 快照 = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (快照 == INVALID_HANDLE_VALUE) {
            return 线程集;
        }

        const DWORD 进程ID = GetCurrentProcessId();
        THREADENTRY32 条目{};
        条目.dwSize = sizeof(条目);
        if (Thread32First(快照, &条目)) {
            do {
                if (条目.th32OwnerProcessID == 进程ID) {
                    线程集.push_back(条目);
                }
                条目.dwSize = sizeof(条目);
            } while (Thread32Next(快照, &条目));
        }
        CloseHandle(快照);
        return 线程集;
    }

    结构_控制面板树节点 私有_构建线程状态树(
        const 结构_控制面板快照& 快照,
        const 任务管理线程协议::结构_任务界面线程快照& 界面线程快照,
        const 任务管理工作线程::结构_工作线程实例快照& 工作线程快照)
    {
        auto 根节点 = 私有_新节点(
            "线程状态 | 系统线程=" + std::to_string(快照.线程数)
                + " | 自我线程=" + 快照.自我线程当前阶段,
            0,
            true);

        auto 自我线程节点 = 私有_新节点(
            std::string("自我线程 | 生命周期=")
                + 私有_线程生命周期文本(static_cast<枚举_线程生命周期状态>(快照.自我线程生命周期))
                + " | 阶段=" + (快照.自我线程当前阶段.empty() ? std::string("暂无数据") : 快照.自我线程当前阶段)
                + " | Tick=" + std::to_string(快照.自我Tick计数),
            私有_线程详情_自我,
            false,
            false,
            "thread-self");
        根节点.子项.push_back(std::move(自我线程节点));

        auto 界面线程节点 = 私有_新节点(
            std::string("任务管理界面线程 | 启动=")
                + 私有_布尔文本(界面线程快照.已启动)
                + " | 处理=" + std::string(私有_布尔文本(界面线程快照.正在处理))
                + " | 请求=" + std::to_string(界面线程快照.累计接收请求数)
                + " | 等待包=" + std::to_string(界面线程快照.当前等待工作包数)
                + " | 上行=" + std::to_string(界面线程快照.当前上行队列长度),
            私有_线程详情_任务界面,
            false,
            false,
            "thread-task-interface");
        根节点.子项.push_back(std::move(界面线程节点));

        auto 工作线程节点 = 私有_新节点(
            std::string("任务管理工作线程 | 启动=")
                + 私有_布尔文本(工作线程快照.已启动)
                + " | 执行=" + std::string(私有_布尔文本(工作线程快照.正在执行))
                + " | 排队=" + std::to_string(工作线程快照.当前排队数)
                + " | 推进=" + std::to_string(工作线程快照.累计推进次数),
            私有_线程详情_工作,
            false,
            false,
            "thread-worker");
        根节点.子项.push_back(std::move(工作线程节点));

        auto 摘要线程节点 = 私有_新节点(
            std::string("控制面板摘要线程 | 生命周期=")
                + 控制面板摘要线程生命周期文本(
                    static_cast<枚举_控制面板摘要线程生命周期状态>(
                        快照.控制面板摘要线程生命周期))
                + " | 健康=" + std::string(私有_布尔文本(快照.控制面板摘要线程健康运行))
                + " | 快照=" + std::to_string(快照.控制面板摘要快照序号),
            私有_线程详情_摘要,
            false,
            false,
            "thread-summary");
        根节点.子项.push_back(std::move(摘要线程节点));

        auto 系统线程节点 = 私有_新节点(
            std::string("系统线程摘要 | 当前进程线程数=") + std::to_string(快照.线程数),
            私有_线程详情_系统,
            false,
            false,
            "thread-system");
        根节点.子项.push_back(std::move(系统线程节点));

        return 根节点;
    }

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
                    输出 << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                        << static_cast<int>(static_cast<unsigned char>(字符))
                        << std::dec << std::setfill(' ');
                }
                else {
                    输出 << 字符;
                }
                break;
            }
        }
        输出 << '"';
    }

    void 私有_追加树节点JSON(std::ostringstream& 输出, const 结构_控制面板树节点& 节点)
    {
        输出 << "{";
        输出 << "\"ptr\":" << 节点.节点指针 << ",";
        输出 << "\"arg\":" << 节点.附加参数 << ",";
        输出 << "\"text\":";
        私有_追加JSON字符串(输出, 节点.文本);
        输出 << ",";
        输出 << "\"open\":" << (节点.默认展开 ? "true" : "false") << ",";
        输出 << "\"lazy\":" << (节点.可延迟展开 ? "true" : "false") << ",";
        输出 << "\"expandType\":";
        私有_追加JSON字符串(输出, 节点.展开类型);
        输出 << ",";
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

    std::string 私有_树节点JSON(const 结构_控制面板树节点& 节点)
    {
        std::ostringstream 输出;
        私有_追加树节点JSON(输出, 节点);
        return 输出.str();
    }

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

    void 私有_移除节点字段子项(结构_控制面板树节点& 节点)
    {
        节点.子项.erase(
            std::remove_if(
                节点.子项.begin(),
                节点.子项.end(),
                [](const 结构_控制面板树节点& 子项) {
                    return 子项.文本 == "节点字段";
                }),
            节点.子项.end());
    }

    std::vector<结构_控制面板树节点> 私有_提取节点字段详情(const 结构_控制面板树节点& 节点)
    {
        std::vector<结构_控制面板树节点> 结果{};
        for (const auto& 子项 : 节点.子项) {
            if (子项.文本 == "节点字段") {
                结果.push_back(子项);
            }
        }
        return 结果;
    }

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

    结构_构建上下文 私有_创建构建上下文(const std::size_t 树广度上限)
    {
        结构_构建上下文 上下文{};
        上下文.树深度上限 = 1;
        上下文.树广度上限 = (std::min)((std::max<std::size_t>)(1, 树广度上限), std::size_t{16});
        上下文.引用展开深度上限 = 1;
        上下文.自我存在指针 = 私有_地址(自我.获取自我存在());
        上下文.自我现实场景指针 = 私有_地址(自我.获取自我现实场景());
        上下文.自我内部世界指针 = 私有_地址(自我.获取自我内部世界());
        上下文.世界默认展开路径 = 私有_合并路径集合(
            私有_构建父链路径(自我.获取自我现实场景()),
            私有_构建父链路径(自我.获取自我内部世界()));
        上下文.世界默认展开路径 = 私有_合并路径集合(
            std::move(上下文.世界默认展开路径),
            私有_构建父链路径(自我.获取自我存在()));
        return 上下文;
    }

    struct 结构_需求列表分页结果 {
        std::vector<需求节点*> 节点集{};
        bool 还有更多 = false;
        std::size_t 已遍历数量 = 0;
    };

    结构_需求列表分页结果 私有_读取需求列表分页(
        需求节点* 根节点,
        const std::size_t 起始偏移,
        const std::size_t 每页数量)
    {
        结构_需求列表分页结果 结果{};
        if (!根节点 || 每页数量 == 0) {
            return 结果;
        }

        std::vector<需求节点*> 栈{};
        栈.push_back(根节点);
        while (!栈.empty()) {
            auto* 当前 = 栈.back();
            栈.pop_back();

            if (当前 != 根节点) {
                if (结果.已遍历数量 >= 起始偏移 && 结果.节点集.size() < 每页数量) {
                    结果.节点集.push_back(当前);
                }
                ++结果.已遍历数量;
            }

            const auto 子节点集 = 私有_枚举全部子节点(当前);
            for (auto it = 子节点集.rbegin(); it != 子节点集.rend(); ++it) {
                栈.push_back(*it);
            }

            if (结果.节点集.size() >= 每页数量 && !栈.empty()) {
                结果.还有更多 = true;
                break;
            }
        }

        if (!结果.还有更多) {
            结果.还有更多 = !栈.empty();
        }
        return 结果;
    }

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

结构_控制面板快照 读取控制面板快照(
    const std::size_t 树深度上限,
    const std::size_t 树广度上限)
{
    结构_控制面板快照 快照{};

    快照.世界树已初始化 = 世界树.基础信息().世界根() != nullptr;
    快照.自我已初始化 = 自我.已初始化();
    auto* 原始自我存在 = 自我.获取自我存在();
    快照.自我存在已建立 = 原始自我存在 != nullptr;
    if (const auto* 自我主信息 = 世界树.基础信息().取主信息<存在节点主信息类>(原始自我存在)) {
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
    快照.自我线程最近运行摘要 = 自我线程.读取最近运行摘要();
    快照.自我线程最近恢复摘要 = 自我线程.读取最近恢复摘要();
    快照.自我线程最近故障摘要 = 自我线程.读取最近故障摘要();

    快照.控制面板摘要线程已启动 =
        摘要线程生命周期 != 枚举_控制面板摘要线程生命周期状态::未启动
        && 摘要线程生命周期 != 枚举_控制面板摘要线程生命周期状态::已停止;
    快照.控制面板摘要线程健康运行 =
        获取全局控制面板摘要线程().是否健康运行();
    快照.控制面板摘要线程生命周期 =
        static_cast<std::uint8_t>(摘要线程生命周期);
    快照.控制面板摘要快照序号 = 摘要线程快照.快照序号;
    快照.控制面板摘要线程摘要 = 摘要线程快照.自我线程摘要;
    快照.控制面板摘要_需求树 = 摘要线程快照.需求树摘要;
    快照.控制面板摘要_任务树 = 摘要线程快照.任务树摘要;
    快照.控制面板摘要_方法树 = 摘要线程快照.方法树摘要;
    快照.控制面板摘要_运行事实 = 摘要线程快照.运行事实摘要;
    快照.控制面板摘要_自检 = 摘要线程快照.自检摘要;
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
    快照.自我存在标题 = 私有_安全节点摘要(自我.获取自我存在(), "自我存在");

    const auto 全部基础节点 = 世界树.基础信息().枚举全部节点();
    快照.基础信息节点数 = 全部基础节点.size();
    快照.场景数 = 世界树.基础信息().枚举节点_按类型<场景节点主信息类>().size();
    快照.存在数 = 世界树.基础信息().枚举节点_按类型<存在节点主信息类>().size();
    快照.特征数 = 世界树.基础信息().枚举节点_按类型<特征节点主信息类>().size();
    快照.状态数 = 世界树.基础信息().枚举节点_按类型<状态节点主信息类>().size();
    快照.动态数 = 世界树.基础信息().枚举节点_按类型<动态节点主信息类>().size();

    const auto 因果节点集 = 世界树.基础信息().枚举节点_按类型<因果主信息类>();
    std::size_t 因果模板数 = 0;
    std::size_t 因果证据动态样本数 = 0;
    for (auto* 因果节点 : 因果节点集) {
        const auto* 主信息 = 世界树.基础信息().取主信息<因果主信息类>(因果节点);
        if (!主信息) {
            continue;
        }
        if (主信息->是抽象因果()) {
            ++因果模板数;
        }
        因果证据动态样本数 += 主信息->证据动态样本.size();
    }
    快照.因果模板数 = 因果模板数;
    快照.因果证据动态样本数 = 因果证据动态样本数;

    auto* 自我存在 = 自我.获取自我存在();
    auto* 需求根节点 = 自我存在 ? 世界树.存在().获取需求根节点(自我存在) : nullptr;
    auto* 任务根节点 = 自我存在 ? 世界树.存在().获取任务根节点(自我存在) : nullptr;
    auto* 方法根节点 = 自我存在 ? 世界树.存在().获取方法根节点(自我存在) : nullptr;

    快照.需求数 = 需求根节点 ? (私有_计数子树节点(需求根节点) - 1) : 0;
    快照.方法数 = 方法根节点 ? (私有_计数子树节点(方法根节点) - 1) : 0;
    if (方法根节点) {
        auto 统计方法结构 = [&](const 方法节点* 节点) {
            if (!节点 || 节点 == 方法根节点
                || 节点->主信息.节点种类() != 枚举_方法节点种类::方法首节点) {
                return;
            }
            ++快照.方法首节点数;
            const auto 计数 = 私有_统计方法直接条件结果(节点);
            if (计数.条件数 > 0 && 计数.结果数 == 0) {
                ++快照.方法条件无结果数;
            }
            if (计数.结果数 > 0 && 计数.条件数 == 0) {
                ++快照.方法结果无条件数;
            }
            if (计数.条件数 != 计数.结果数) {
                ++快照.方法条件结果不成对数;
            }
        };
        私有_遍历子树节点(reinterpret_cast<方法节点*>(方法根节点), 统计方法结构);
    }
    if (需求根节点) {
        auto 统计需求满足 = [&](const 需求节点* 节点) {
            if (!节点 || 节点 == 需求根节点) {
                return;
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

            switch (节点->主信息.结构角色) {
            case 枚举_需求结构角色::管理需求:
                ++快照.需求树管理需求数;
                break;
            case 枚举_需求结构角色::执行需求:
                ++快照.需求树执行需求数;
                break;
            case 枚举_需求结构角色::未定义:
            default:
                ++快照.需求树角色未定义数;
                break;
            }

            if (节点->主信息.需求有效截止 != 0) {
                ++快照.需求树已截止需求数;
            }
            else if (节点->主信息.是否阻塞父任务执行) {
                ++快照.需求树活动阻塞需求数;
            }

            if (!节点->主信息.是否阻塞父任务执行) {
                ++快照.需求树非阻塞需求数;
            }

            if (!节点->主信息.派生来源方法主键.empty()
                || 节点->主信息.派生需求类型抽象特征指针 != 0
                || 节点->主信息.派生需求类型值 != 0
                || 节点->主信息.派生方法需求位抽象特征指针 != 0
                || 节点->主信息.派生方法需求位值 != 0
                || 节点->主信息.派生本能能力缺口类型抽象特征指针 != 0
                || 节点->主信息.派生本能能力缺口类型值 != 0) {
                ++快照.需求树派生归因需求数;
            }
        };
        私有_遍历子树节点(需求根节点, 统计需求满足);
    }
    if (auto* 当前主需求节点 = 自我.获取当前主需求()) {
        快照.需求树当前主需求主键 = 当前主需求节点->获取主键();
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
            }
        };
        私有_遍历子树节点(任务根节点, 统计任务节点);
    }

    const auto 缺口承接快照 = 私有_读取缺口承接快照_控制面板(自我, 自我线程);
    快照.自我待处理方法数量 = 缺口承接快照.待处理方法数量;
    (void)自我.读取自我I64特征当前值(
        自我特征定义类::类型_自我_可用方法数量(),
        快照.自我可用方法数量);
    快照.缺口需求总数 = 0;
    快照.缺口需求活跃数 = 0;
    快照.缺口需求可调度数 = 0;
    快照.缺口恢复请求数 = 缺口承接快照.恢复请求数;
    快照.缺口因兜底切换进入就绪数 = 0;
    快照.缺口因兜底切换进入采样数 = 0;
    快照.应触发缺口承接 = 缺口承接快照.应触发缺口承接;
    快照.缺口应申请重试恢复 = 缺口承接快照.应申请重试恢复;
    快照.缺口应申请收束恢复 = 缺口承接快照.应申请收束恢复;
    快照.缺口调度摘要 = "缺口承接已接管";
    快照.缺口最近摘要.clear();
    快照.缺口最近失败摘要.clear();
    快照.缺口最近反馈摘要 = 缺口承接快照.最近缺口反馈摘要;
    快照.缺口最近回流摘要.clear();
    快照.缺口首个兜底切换就绪摘要.clear();
    快照.缺口首个兜底切换采样摘要.clear();
    快照.缺口当前阶段.clear();
    快照.缺口当前状态.clear();
    快照.缺口当前任务标题.clear();
    快照.缺口当前方法标题.clear();

    任务管理工作线程::结构_工作线程实例快照 工作线程快照{};
    (void)任务管理工作线程::读取任务管理工作线程快照(&工作线程快照);
    任务管理线程协议::结构_任务界面线程快照 界面线程快照{};
    (void)任务管理界面线程::读取任务管理界面线程快照(&界面线程快照);

    快照.任务管理界面线程已启动 = 界面线程快照.已启动;
    快照.任务管理界面线程正在处理 = 界面线程快照.正在处理;
    快照.任务管理界面线程故障 = 界面线程快照.故障;
    快照.任务管理界面线程累计接收请求数 = 界面线程快照.累计接收请求数;
    快照.任务管理界面线程累计绑定任务虚拟存在数 = 界面线程快照.累计绑定任务虚拟存在数;
    快照.任务管理界面线程累计派发工作包数 = 界面线程快照.累计派发工作包数;
    快照.任务管理界面线程累计收到工作结果数 = 界面线程快照.累计收到工作结果数;
    快照.任务管理界面线程累计上行消息数 = 界面线程快照.累计上行消息数;
    快照.任务管理界面线程当前请求队列长度 = 界面线程快照.当前请求队列长度;
    快照.任务管理界面线程当前等待工作包数 = 界面线程快照.当前等待工作包数;
    快照.任务管理界面线程当前上行队列长度 = 界面线程快照.当前上行队列长度;
    快照.任务管理界面线程最近请求ID = 界面线程快照.最近请求ID;
    快照.任务管理界面线程最近工作包ID = 界面线程快照.最近工作包ID;
    快照.任务管理界面线程最近任务主键 = 界面线程快照.最近任务主键;
    快照.任务管理界面线程最近任务虚拟存在主键 = 界面线程快照.最近任务虚拟存在主键;
    快照.任务管理界面线程最近需求主键 = 界面线程快照.最近需求主键;
    快照.任务管理界面线程最近调度动作 = 界面线程快照.最近调度动作;
    快照.任务管理界面线程最近说明 = 界面线程快照.最近说明;

    快照.任务管理工作线程已启动 = 工作线程快照.已启动;
    快照.任务管理工作线程正在执行 = 工作线程快照.正在执行;
    快照.任务管理工作线程已收到请求 = 工作线程快照.已收到请求;
    快照.任务管理工作线程累计推进次数 = 工作线程快照.累计推进次数;
    快照.任务管理工作线程当前排队数 = 工作线程快照.当前排队数;
    快照.任务管理工作线程最近因果链状态 = 工作线程快照.最近因果链状态;
    快照.任务管理工作线程最近缺口归类 = 工作线程快照.最近缺口归类;
    快照.任务管理工作线程最近特征变化数 = 工作线程快照.最近特征变化数;

    std::optional<结构_任务管理结果> 最近治理结果{};
    std::optional<结构_治理恢复快照> 最近恢复快照{};

    快照.线程数 = 私有_枚举当前进程线程().size();

    结构_构建上下文 上下文{};
    上下文.树深度上限 = (std::min)((std::max<std::size_t>)(1, 树深度上限), std::size_t{6});
    上下文.树广度上限 = (std::min)((std::max<std::size_t>)(1, 树广度上限), std::size_t{16});
    上下文.引用展开深度上限 = 2;
    上下文.自我存在指针 = 私有_地址(自我存在);
    上下文.自我现实场景指针 = 私有_地址(自我.获取自我现实场景());
    上下文.自我内部世界指针 = 私有_地址(自我.获取自我内部世界());
    上下文.世界默认展开路径 = 私有_合并路径集合(
        私有_构建父链路径(自我.获取自我现实场景()),
        私有_构建父链路径(自我.获取自我内部世界()));
    上下文.世界默认展开路径 = 私有_合并路径集合(
        std::move(上下文.世界默认展开路径),
        私有_构建父链路径(自我存在));

    快照.线程状态树根 = 私有_构建线程状态树(快照, 界面线程快照, 工作线程快照);

    auto 世界树根 = 私有_新节点(
        std::string("世界树 | 节点仓库=基础信息类")
            + " | 基础节点=" + std::to_string(快照.基础信息节点数)
            + " | 场景=" + std::to_string(快照.场景数)
            + " | 存在=" + std::to_string(快照.存在数),
        0,
        true);
    if (auto* 世界根节点 = 世界树.基础信息().世界根()) {
        世界树根.子项.push_back(
            私有_构建基础信息根链骨架(世界根节点, 上下文));
    }
    else {
        世界树根.子项.push_back(私有_新节点("基础信息仓库根为空"));
    }
    快照.世界树根 = std::move(世界树根);

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

    auto 需求列表树根 = 私有_新节点(
        "需求列表 | 需求数=" + std::to_string(快照.需求数),
        0,
        true);
    if (需求根节点) {
        const auto 分页结果 = 私有_读取需求列表分页(需求根节点, 0, 私有_列表分页大小);
        for (auto* 节点 : 分页结果.节点集) {
            需求列表树根.子项.push_back(私有_需求骨架节点(节点, 上下文, false));
        }
        if (分页结果.还有更多) {
            需求列表树根.子项.push_back(私有_创建需求列表加载更多节点(分页结果.已遍历数量));
        }
    }
    if (需求列表树根.子项.empty()) {
        需求列表树根.子项.push_back(私有_新节点("暂无需求列表项"));
    }
    快照.需求列表树根 = std::move(需求列表树根);

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

    return 快照;
}

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
        结构_控制面板树节点 容器 = 私有_新节点("引用展开");
        auto 目标节点 = 私有_构建基础信息树节点(
            已解析节点,
            上下文,
            1,
            {});
        私有_移除节点字段子项(目标节点);
        容器.子项.push_back(std::move(目标节点));
        return 私有_树节点子项JSON(容器);
    }
    if (展开类型 == "need-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<需求节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        结构_控制面板树节点 容器 = 私有_新节点("引用展开");
        auto 目标节点 = 私有_构建需求树节点(
            已解析节点,
            上下文,
            1,
            {},
            false);
        私有_移除节点字段子项(目标节点);
        容器.子项.push_back(std::move(目标节点));
        return 私有_树节点子项JSON(容器);
    }
    if (展开类型 == "task-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<任务节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        结构_控制面板树节点 容器 = 私有_新节点("引用展开");
        auto 目标节点 = 私有_构建任务树节点(
            已解析节点,
            上下文,
            1,
            {});
        私有_移除节点字段子项(目标节点);
        容器.子项.push_back(std::move(目标节点));
        return 私有_树节点子项JSON(容器);
    }
    if (展开类型 == "method-ref") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<方法节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON("引用目标已失效或已移出当前树");
        }
        结构_控制面板树节点 容器 = 私有_新节点("引用展开");
        auto 目标节点 = 私有_构建方法树节点(
            已解析节点,
            上下文,
            1,
            {});
        私有_移除节点字段子项(目标节点);
        容器.子项.push_back(std::move(目标节点));
        return 私有_树节点子项JSON(容器);
    }
    if (展开类型 == "base-node-more") {
        结构_控制面板树节点 容器 = 私有_新节点("继续加载基础信息结构子节点");
        auto* 父节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<基础信息节点类*>(节点指针));
        if (!父节点) {
            return 私有_失效节点JSON("父节点已失效或已移出当前树");
        }
        const auto 子节点集 = 父节点 ? 世界树.基础信息().枚举子节点(父节点) : std::vector<基础信息节点类*>{};
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
        if (auto* 需求根节点 = 自我存在 ? 世界树.存在().获取需求根节点(自我存在) : nullptr) {
            const auto 分页结果 = 私有_读取需求列表分页(
                需求根节点,
                static_cast<std::size_t>(节点指针),
                私有_列表分页大小);
            for (auto* 节点 : 分页结果.节点集) {
                容器.子项.push_back(私有_需求骨架节点(节点, 上下文, false));
            }
            if (分页结果.还有更多) {
                容器.子项.push_back(私有_创建需求列表加载更多节点(分页结果.已遍历数量));
            }
        }
        return 私有_树节点子项JSON(容器);
    }

    return "[]";
}

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
        const auto 节点 = 私有_构建需求树节点(
            已解析节点,
            上下文,
            1,
            {},
            false);
        return 私有_树节点列表JSON(私有_提取节点字段详情(节点));
    }
    if (展开类型 == "task-node") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<任务节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON();
        }
        const auto 节点 = 私有_构建任务树节点(
            已解析节点,
            上下文,
            1,
            {});
        return 私有_树节点列表JSON(私有_提取节点字段详情(节点));
    }
    if (展开类型 == "method-node") {
        auto* 已解析节点 = 私有_解析当前树节点_可写(
            reinterpret_cast<方法节点*>(节点指针));
        if (!已解析节点) {
            return 私有_失效节点JSON();
        }
        const auto 节点 = 私有_构建方法树节点(
            已解析节点,
            上下文,
            1,
            {});
        return 私有_树节点列表JSON(私有_提取节点字段详情(节点));
    }
    if (展开类型 == "thread-self"
        || 展开类型 == "thread-task-interface"
        || 展开类型 == "thread-worker"
        || 展开类型 == "thread-summary"
        || 展开类型 == "thread-system") {
        const auto 快照 = 读取控制面板快照(1, 16);
        任务管理工作线程::结构_工作线程实例快照 工作线程快照{};
        (void)任务管理工作线程::读取任务管理工作线程快照(&工作线程快照);
        任务管理线程协议::结构_任务界面线程快照 界面线程快照{};
        (void)任务管理界面线程::读取任务管理界面线程快照(&界面线程快照);

        auto 字段节点 = 私有_新节点("节点字段");
        if (展开类型 == "thread-self") {
            私有_追加叶字段(字段节点, "生命周期", 私有_线程生命周期文本(static_cast<枚举_线程生命周期状态>(快照.自我线程生命周期)));
            私有_追加叶字段(字段节点, "当前阶段", 快照.自我线程当前阶段);
            私有_追加叶字段(字段节点, "当前最终去向", 快照.自我线程当前最终去向);
            私有_追加叶字段(字段节点, "Tick计数", 快照.自我Tick计数);
            私有_追加叶字段(字段节点, "累计故障次数", 快照.自我线程累计故障次数);
            私有_追加叶字段(字段节点, "累计恢复次数", 快照.自我线程累计恢复次数);
            私有_追加叶字段(字段节点, "最近运行摘要", 私有_截断文本(快照.自我线程最近运行摘要, 160));
            私有_追加叶字段(字段节点, "最近恢复摘要", 私有_截断文本(快照.自我线程最近恢复摘要, 160));
            私有_追加叶字段(字段节点, "最近故障摘要", 私有_截断文本(快照.自我线程最近故障摘要, 160));
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
            私有_追加叶字段(字段节点, "最近因果链状态", 工作线程快照.最近因果链状态);
            私有_追加叶字段(字段节点, "最近缺口归类", 工作线程快照.最近缺口归类);
            私有_追加叶字段(字段节点, "最近推进阶段", 工作线程快照.最近推进阶段);
            私有_追加叶字段(字段节点, "最近推进状况", 工作线程快照.最近推进状况);
            私有_追加叶字段(字段节点, "最近筹办承接", 工作线程快照.最近筹办承接);
            私有_追加叶字段(字段节点, "最近等待原因", 工作线程快照.最近等待原因);
            私有_追加叶字段(字段节点, "最近特征变化数", 工作线程快照.最近特征变化数);
        }
        else if (展开类型 == "thread-task-interface") {
            私有_追加叶字段(字段节点, "已启动", 界面线程快照.已启动);
            私有_追加叶字段(字段节点, "正在处理", 界面线程快照.正在处理);
            私有_追加叶字段(字段节点, "故障", 界面线程快照.故障);
            私有_追加叶字段(字段节点, "累计接收请求数", 界面线程快照.累计接收请求数);
            私有_追加叶字段(字段节点, "累计绑定任务虚拟存在数", 界面线程快照.累计绑定任务虚拟存在数);
            私有_追加叶字段(字段节点, "累计派发工作包数", 界面线程快照.累计派发工作包数);
            私有_追加叶字段(字段节点, "累计收到工作结果数", 界面线程快照.累计收到工作结果数);
            私有_追加叶字段(字段节点, "累计上行消息数", 界面线程快照.累计上行消息数);
            私有_追加叶字段(字段节点, "当前请求队列长度", 界面线程快照.当前请求队列长度);
            私有_追加叶字段(字段节点, "当前等待工作包数", 界面线程快照.当前等待工作包数);
            私有_追加叶字段(字段节点, "当前上行队列长度", 界面线程快照.当前上行队列长度);
            私有_追加叶字段(字段节点, "最近请求ID", 界面线程快照.最近请求ID);
            私有_追加叶字段(字段节点, "最近工作包ID", 界面线程快照.最近工作包ID);
            私有_追加叶字段(字段节点, "最近任务主键", 界面线程快照.最近任务主键);
            私有_追加叶字段(字段节点, "最近任务虚拟存在主键", 界面线程快照.最近任务虚拟存在主键);
            私有_追加叶字段(字段节点, "最近需求主键", 界面线程快照.最近需求主键);
            私有_追加叶字段(字段节点, "最近调度动作", 界面线程快照.最近调度动作);
            私有_追加叶字段(字段节点, "最近说明", 私有_截断文本(界面线程快照.最近说明, 200));
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
            私有_追加叶字段(字段节点, "线程摘要", 私有_截断文本(快照.控制面板摘要线程摘要, 200));
            私有_追加叶字段(字段节点, "需求树摘要", 私有_截断文本(快照.控制面板摘要_需求树, 200));
            私有_追加叶字段(字段节点, "任务树摘要", 私有_截断文本(快照.控制面板摘要_任务树, 200));
            私有_追加叶字段(字段节点, "方法树摘要", 私有_截断文本(快照.控制面板摘要_方法树, 200));
            私有_追加叶字段(字段节点, "运行事实摘要", 私有_截断文本(快照.控制面板摘要_运行事实, 200));
            私有_追加叶字段(字段节点, "自检摘要", 私有_截断文本(快照.控制面板摘要_自检, 200));
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
            私有_追加叶字段(字段节点, "当前进程线程数", 快照.线程数);
            私有_追加叶字段(字段节点, "说明", "重构版先保留稳定摘要，逐线程探测后续再补。");
        }
        return 私有_树节点列表JSON({ std::move(字段节点) });
    }

    return "[]";
}

std::string 渲染控制面板摘要(
    const 结构_控制面板快照& 快照,
    std::size_t,
    std::size_t)
{
    std::ostringstream 输出;
    输出 << "控制面板摘要\n"
        << "  - 自我: 初始化=" << 私有_布尔文本(快照.自我已初始化)
        << " | 存在=" << 私有_布尔文本(快照.自我存在已建立)
        << " | 内部世界=" << 私有_布尔文本(快照.自我内部世界已建立)
        << " | 根字段=需" << 私有_布尔文本(快照.自我需求根字段已建立)
        << "/任" << 私有_布尔文本(快照.自我任务根字段已建立)
        << "/方" << 私有_布尔文本(快照.自我方法根字段已建立)
        << " | 待机=" << 私有_布尔文本(快照.自我待机状态)
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
        << " | 健康=" << 私有_布尔文本(快照.控制面板摘要线程健康运行)
        << " | 快照=" << 快照.控制面板摘要快照序号
        << " | 需求=" << 私有_页面摘要(快照.控制面板摘要_需求树)
        << " | 任务=" << 私有_页面摘要(快照.控制面板摘要_任务树)
        << " | 方法=" << 私有_页面摘要(快照.控制面板摘要_方法树)
        << '\n'
        << "  - 需求树: 总数=" << 快照.需求数
        << " | 已满足=" << 快照.需求已满足数
        << " | 未满足=" << 快照.需求未满足数
        << " | 管理=" << 快照.需求树管理需求数
        << " | 执行=" << 快照.需求树执行需求数
        << " | 活动阻塞=" << 快照.需求树活动阻塞需求数
        << " | 派生归因=" << 快照.需求树派生归因需求数
        << " | 当前主需求=" << 私有_页面摘要(快照.需求树当前主需求主键.empty()
            ? std::string("空")
            : 快照.需求树当前主需求主键)
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
        << 私有_布尔文本(快照.任务管理工作线程已启动)
        << " | 界面线程=" << 私有_布尔文本(快照.任务管理界面线程已启动)
        << " | 请求=" << 快照.任务管理界面线程累计接收请求数
        << " | 上行=" << 快照.任务管理界面线程当前上行队列长度
        << " | 执行=" << 私有_布尔文本(快照.任务管理工作线程正在执行)
        << " | 排队=" << 快照.任务管理工作线程当前排队数
        << " | 因果链=" << 私有_页面摘要(快照.任务管理工作线程最近因果链状态)
        << " | 缺口=" << 私有_页面摘要(快照.任务管理工作线程最近缺口归类)
        << " | 特征变化=" << 快照.任务管理工作线程最近特征变化数
        << '\n';
    return 输出.str();
}
std::string 渲染任务管理摘要(
    const 结构_控制面板快照& 快照,
    std::size_t)
{
    std::ostringstream 输出;
    输出 << "任务管理摘要\n"
        << "  - 自我线程: 阶段=" << 私有_页面摘要(快照.自我线程当前阶段)
        << " | 去向=" << 私有_页面摘要(快照.自我线程当前最终去向)
        << " | 最近运行=" << 私有_页面摘要(快照.自我线程最近运行摘要)
        << '\n'
        << "  - 工作线程: 已启动=" << 私有_布尔文本(快照.任务管理工作线程已启动)
        << " | 正在执行=" << 私有_布尔文本(快照.任务管理工作线程正在执行)
        << " | 已收到请求=" << 私有_布尔文本(快照.任务管理工作线程已收到请求)
        << " | 推进次数=" << 快照.任务管理工作线程累计推进次数
        << " | 当前排队=" << 快照.任务管理工作线程当前排队数
        << '\n'
        << "  - 界面线程: 已启动=" << 私有_布尔文本(快照.任务管理界面线程已启动)
        << " | 正在处理=" << 私有_布尔文本(快照.任务管理界面线程正在处理)
        << " | 故障=" << 私有_布尔文本(快照.任务管理界面线程故障)
        << " | 请求=" << 快照.任务管理界面线程累计接收请求数
        << " | 派发工作包=" << 快照.任务管理界面线程累计派发工作包数
        << " | 上行队列=" << 快照.任务管理界面线程当前上行队列长度
        << " | 最近调度=" << 私有_页面摘要(快照.任务管理界面线程最近调度动作)
        << " | 最近任务=" << 私有_页面摘要(快照.任务管理界面线程最近任务主键)
        << " | 任务虚拟存在=" << 私有_页面摘要(快照.任务管理界面线程最近任务虚拟存在主键)
        << " | 最近说明=" << 私有_页面摘要(快照.任务管理界面线程最近说明)
        << '\n'
        << "  - 树规模: 需求=" << 快照.需求数
        << " | 任务节点=" << 快照.任务数
        << " (头=" << 快照.任务头节点数
        << ", 步骤=" << 快照.任务步骤节点数
        << ", 其他=" << 快照.任务其他节点数 << ")"
        << " | 方法=" << 快照.方法数
        << " | 缺口承接=已接管"
        << '\n'
        << "  - 最近治理: 功能域=" << 私有_页面摘要(快照.任务管理最近功能域)
        << " | 缺口=" << 私有_页面摘要(快照.任务管理最近缺口类型)
        << " | 去向=" << 私有_页面摘要(快照.任务管理最近下一步去向)
        << " | 总控=" << 私有_页面摘要(快照.任务管理最近总控结果)
        << '\n'
        << "  - 恢复快照=" << 私有_页面摘要(快照.任务管理最近恢复摘要)
        << '\n';
    return 输出.str();
}

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
        << "  - 判定: 应触发缺口承接=" << 私有_布尔文本(快照.应触发缺口承接)
        << " | 应申请重试恢复=" << 私有_布尔文本(快照.缺口应申请重试恢复)
        << " | 应申请收束恢复=" << 私有_布尔文本(快照.缺口应申请收束恢复)
        << '\n'
        << "  - 当前: 阶段=" << 私有_页面摘要(快照.缺口当前阶段)
        << " | 状态=" << 私有_页面摘要(快照.缺口当前状态)
        << " | 任务=" << 私有_页面摘要(快照.缺口当前任务标题)
        << " | 方法=" << 私有_页面摘要(快照.缺口当前方法标题)
        << '\n'
        << "  - 缺口调度=" << 私有_页面摘要(快照.缺口调度摘要)
        << '\n'
        << "  - 最近摘要=" << 私有_页面摘要(快照.缺口最近摘要)
        << '\n'
        << "  - 最近反馈=" << 私有_页面摘要(快照.缺口最近反馈摘要)
        << '\n'
        << "  - 最近回流=" << 私有_页面摘要(快照.缺口最近回流摘要)
        << '\n';
    return 输出.str();
}

std::string 生成控制面板HTML(
    const 结构_控制面板快照& 快照,
    std::size_t)
{
    const auto 线程树JSON = 私有_树节点JSON(快照.线程状态树根);
    const auto 世界树JSON = 私有_树节点JSON(快照.世界树根);
    const auto 需求树JSON = 私有_树节点JSON(快照.需求树根);
    const auto 需求列表JSON = 私有_树节点JSON(快照.需求列表树根);
    const auto 任务树JSON = 私有_树节点JSON(快照.任务树根);
    const auto 方法树JSON = 私有_树节点JSON(快照.方法树根);

    const auto 树交互提示 = 私有_转义HTML("结构节点与字段摘要");
    const auto 任务树交互提示 = 私有_转义HTML("任务根节点结构与运行壳摘要");
    const auto 线程页摘要 = 私有_转义HTML(
        "自我线程阶段=" + 私有_页面摘要(快照.自我线程当前阶段)
        + " | 去向=" + 私有_页面摘要(快照.自我线程当前最终去向)
        + " | 任务界面线程=" + std::string(私有_布尔文本(快照.任务管理界面线程已启动))
        + " | 工作线程=" + std::string(私有_布尔文本(快照.任务管理工作线程已启动))
        + " | 系统线程数=" + std::to_string(快照.线程数));
    const auto 世界页摘要 = 私有_转义HTML(
        "节点仓库=基础信息类 | 基础节点=" + std::to_string(快照.基础信息节点数)
        + " | 场景=" + std::to_string(快照.场景数)
        + " | 存在=" + std::to_string(快照.存在数)
        + " | 状态=" + std::to_string(快照.状态数)
        + " | 动态=" + std::to_string(快照.动态数)
        + " | 因果模板=" + std::to_string(快照.因果模板数));
    const auto 需求树摘要 = 私有_转义HTML(
        "需求数=" + std::to_string(快照.需求数)
        + " | " + 私有_需求满足数量摘要(快照)
        + " | 管理=" + std::to_string(快照.需求树管理需求数)
        + " | 执行=" + std::to_string(快照.需求树执行需求数)
        + " | 活动阻塞=" + std::to_string(快照.需求树活动阻塞需求数));
    const auto 需求列表摘要 = 私有_转义HTML(
        "需求数=" + std::to_string(快照.需求数)
        + " | 列表窗口=" + std::to_string(私有_列表分页大小));
    const auto 任务树摘要 = 私有_转义HTML(
        "任务节点=" + std::to_string(快照.任务数)
            + " | 头=" + std::to_string(快照.任务头节点数)
            + " | 步骤=" + std::to_string(快照.任务步骤节点数)
        + " | 状态={" + 私有_任务状态数量摘要(快照) + "}"
        + " | 功能域=" + 私有_页面摘要(快照.任务管理最近功能域)
        + " | 去向=" + 私有_页面摘要(快照.任务管理最近下一步去向));
    const auto 方法树摘要 = 私有_转义HTML(
        "方法数=" + std::to_string(快照.方法数)
        + " | " + 私有_方法结构诊断摘要(快照)
        + " | 缺口承接=已接管");
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
        + " | 工作包=" + std::to_string(快照.任务管理界面线程累计派发工作包数)
        + " | 上行=" + std::to_string(快照.任务管理界面线程当前上行队列长度)
        + " | 调度=" + 私有_页面摘要(快照.任务管理界面线程最近调度动作));
    const auto 工作线程卡片值 = 私有_转义HTML(快照.任务管理工作线程已启动 ? "已启动" : "未启动");
    const auto 工作线程卡片说明 = 私有_转义HTML(
        "执行=" + std::string(私有_布尔文本(快照.任务管理工作线程正在执行))
        + " | 排队=" + std::to_string(快照.任务管理工作线程当前排队数)
        + " | 推进=" + std::to_string(快照.任务管理工作线程累计推进次数)
        + " | 去向=" + 私有_页面摘要(快照.任务管理最近下一步去向));
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
    }
  </style>
</head>)HTML";
    输出 << R"HTML(<body>
  <div class="shell">
    <div class="layout">
      <aside class="rail">
        <div class="brand">WebView2 控制面板</div>
        <h1>鱼巢</h1>
        <p>需求、任务、方法和线程运行总览。</p>
        <nav class="nav" aria-label="控制面板菜单">
          <button class="menu-item active" type="button" data-page="thread-status"><span>线程状态</span><span class="menu-badge">)HTML"
        << 快照.线程数
        << R"HTML(</span></button>
          <button class="menu-item" type="button" data-page="world-tree"><span>世界树</span><span class="menu-badge">)HTML"
        << 快照.基础信息节点数
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
            <button class="toolbar-btn" type="button" id="refresh-page">刷新</button>
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

        <section class="page active" data-page="thread-status" data-title="线程状态" data-subtitle="自我线程、任务管理界面线程、任务管理工作线程和进程内线程状态。">
          <div class="workspace">
            <section class="panel tree-panel">
              <div class="panel-topline">原始树视图</div>
              <h3>线程状态</h3>
              <div class="summary">)HTML"
        << 线程页摘要
        << R"HTML(</div>
              <div class="tree-toolbar"><div class="tree-hint">)HTML"
        << 树交互提示
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

        <section class="page" data-page="need-tree" data-title="需求树" data-subtitle="真实父子关系、结构角色、目标状态与任务绑定。">
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

        <div class="footer">鱼巢控制面板 · 运行快照</div>
      </main>
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
      'thread-system': '系统线程摘要',
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
        detailHint: '线程运行字段'
      },
      'world-tree': {
        treeHost: 'tree-world-tree',
        detailHost: 'detail-world-tree',
        detailHint: '世界树页展示的是基础信息类仓库；树上只看节点摘要和子层统计，主信息详细信息与父/子/上/下字段都放到右侧。'
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
        detailHint: '任务运行壳字段'
      },
      'method-tree': {
        treeHost: 'tree-method-tree',
        detailHost: 'detail-method-tree',
        detailHint: '方法节点字段'
      }
    };

    const 页面树数据 = {
      "thread-status": )HTML"
        << 线程树JSON
        << R"HTML(,
      "world-tree": )HTML"
        << 世界树JSON
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
        << R"HTML(
    };
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
        return 数量 > 0 ? `${数量} 个预置结构子项，展开时会补齐完整子节点` : '展开时加载结构子项';
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

    function 创建树节点(page, node) {
      const li = document.createElement('li');
      const row = document.createElement('div');
      row.className = 'tree-row';
      row.dataset.nodeId = node.__id;
      row.textContent = node.text || '';
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

        if (是否可展开(node)) {
          const actions = document.createElement('div');
          actions.className = 'detail-inline-actions';
          const toggle = document.createElement('button');
          toggle.type = 'button';
          toggle.className = 'detail-mini-btn';
          toggle.disabled = !!node.__loading;
          toggle.textContent = node.__loading
            ? '加载中'
            : (node.open ? '收起引用' : (node.lazy && !node.__loaded ? '展开引用' : '展开子项'));
          toggle.addEventListener('click', () => 切换节点展开(page, node));
          actions.appendChild(toggle);
          row.appendChild(actions);
        }

        容器.appendChild(row);
      } else {
        const head = document.createElement('div');
        head.className = 'detail-group-head';

        const title = document.createElement('div');
        title.className = 'detail-group-title';
        title.textContent = 解析结果.标题 || node.text || '未命名分组';
        head.appendChild(title);

        if (是否可展开(node)) {
          const toggle = document.createElement('button');
          toggle.type = 'button';
          toggle.className = 'detail-mini-btn';
          toggle.disabled = !!node.__loading;
          toggle.textContent = node.__loading
            ? '加载中'
            : (node.open ? '收起子节点' : (node.lazy && !node.__loaded ? '加载子节点' : '展开子节点'));
          toggle.addEventListener('click', () => 切换节点展开(page, node));
          head.appendChild(toggle);
        }

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
      const 路径 = 构建节点路径(节点);
      const kicker = document.createElement('div');
      kicker.className = 'detail-kicker';
      kicker.textContent = (当前页面?.dataset.title || '当前页面') + ' / 节点详情';
      host.appendChild(kicker);

      const title = document.createElement('div');
      title.className = 'detail-title';
      title.textContent = 解析结果.标题 || 节点.text || '未命名节点';
      host.appendChild(title);

      const note = document.createElement('div');
      note.className = 'detail-note';
      note.textContent = 配置?.detailHint || '节点主信息与字段详情';
      host.appendChild(note);

      const breadcrumbs = document.createElement('div');
      breadcrumbs.className = 'breadcrumbs';
      路径.forEach((item, index) => {
        const isCurrent = index === 路径.length - 1;
        const node = document.createElement(isCurrent ? 'span' : 'button');
        node.className = isCurrent ? 'breadcrumb current' : 'breadcrumb';
        node.textContent = 取节点标题(item);
        if (!isCurrent) {
          node.type = 'button';
          node.addEventListener('click', () => {
            页面选中节点.set(page, item.__id);
            更新选中样式(page);
            请求节点详情(page, item);
            更新详情面板(page);
          });
        }
        breadcrumbs.appendChild(node);
      });
      host.appendChild(breadcrumbs);

      const chips = document.createElement('div');
      chips.className = 'chips';
      [
        推断节点角色(节点),
        '状态=' + 节点状态文本(节点),
        '子项=' + 子项摘要(节点),
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
      if (是否可展开(节点)) {
        const toggle = document.createElement('button');
        toggle.type = 'button';
        toggle.className = 'detail-btn';
        toggle.textContent = 节点.open ? '折叠当前节点' : (节点.lazy && !节点.__loaded ? '加载并展开当前节点' : '展开当前节点');
        toggle.addEventListener('click', () => 切换节点展开(page, 节点));
        actions.appendChild(toggle);
      }
      if (是否引用节点(节点)) {
        const follow = document.createElement('button');
        follow.type = 'button';
        follow.className = 'detail-btn secondary';
        follow.textContent = '跳到引用目标';
        follow.addEventListener('click', () => 跳到引用目标(page, 节点));
        actions.appendChild(follow);
      }
      if (节点.__parent) {
        const up = document.createElement('button');
        up.type = 'button';
        up.className = 'detail-btn secondary';
        up.textContent = '回到父节点';
        up.addEventListener('click', () => {
          页面选中节点.set(page, 节点.__parent.__id);
          更新选中样式(page);
          请求节点详情(page, 节点.__parent);
          更新详情面板(page);
        });
        actions.appendChild(up);
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

      if (解析结果.附加.length > 0) {
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

      const raw = document.createElement('div');
      raw.className = 'detail-raw';
      raw.innerHTML = `<strong>原始文本</strong><br>${转义HTML(节点.text || '空')}`;
      host.appendChild(raw);
    }

    菜单项列表.forEach((item) => {
      item.addEventListener('click', () => 切换页面(item.dataset.page));
    });

    document.getElementById('refresh-page').addEventListener('click', () => {
      if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage('refresh');
      } else {
        location.reload();
      }
    });

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
    切换页面(location.hash.slice(1) || 'thread-status');
  </script>
</body>
</html>)HTML";

    return 输出.str();
}

bool 保存控制面板HTML(
    const std::filesystem::path& 输出路径,
    const 结构_控制面板快照& 快照)
{
    std::ofstream 输出文件(输出路径, std::ios::binary);
    if (!输出文件) {
        return false;
    }

    const auto HTML = 生成控制面板HTML(快照, 24);
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
        else if (参数 == "--panel-html") {
            输出 = 枚举_控制面板命令::生成HTML;
        }
        else if (参数 == "--panel" || 参数 == "--panel-open") {
            输出 = 枚举_控制面板命令::打开窗口;
        }
    }
    return 输出;
}
