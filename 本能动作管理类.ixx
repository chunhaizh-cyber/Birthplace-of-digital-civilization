module;

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "基础信息节点类型.h"
#include "本能方法类.h"
#include "方法类.h"
#include "世界树类.h"
#include "语素类.h"
#include "语素节点类型.h"

export module 本能动作管理模块;

namespace 本能动作管理模块_detail {
    enum class 枚举_本能运行状态 : I64 {
        未启动 = 0,
        运行中 = 1,
        成功 = 2,
        失败 = 3,
        异常 = 4,
        超时 = 5,
        中断 = 6,
    };

    enum class 枚举_本能运行阶段 : I64 {
        未定义 = 0,
        确保 = 1,
        解析绑定 = 2,
        执行 = 3,
        输出 = 4,
        完成 = 5,
    };

    inline std::string 默认函数名称(枚举_本能方法ID 动作ID)
    {
        const auto* 元信息 = 本能方法类::查询元信息(动作ID);
        if (!元信息 || !元信息->动作名称) {
            return {};
        }
        return 元信息->动作名称;
    }

    inline std::vector<枚举_本能方法ID> 默认动作ID列表()
    {
        return 本能方法类::枚举默认自我本能方法ID();
    }

    inline std::uint64_t 下一个请求ID() noexcept
    {
        static std::atomic<std::uint64_t> s_下一个请求ID{ 1 };
        return s_下一个请求ID.fetch_add(1, std::memory_order_relaxed);
    }

    inline const 词性节点类* 本能特征词(const char* 名称) noexcept
    {
        return 语素集.添加词性词(名称 ? 名称 : "", "名词");
    }

    inline const 词性节点类* 特征_运行状态() noexcept { return 本能特征词("运行状态"); }
    inline const 词性节点类* 特征_运行阶段() noexcept { return 本能特征词("运行阶段"); }
    inline const 词性节点类* 特征_最近请求ID() noexcept { return 本能特征词("最近请求ID"); }
    inline const 词性节点类* 特征_最近开始时间() noexcept { return 本能特征词("最近开始时间"); }
    inline const 词性节点类* 特征_最近结束时间() noexcept { return 本能特征词("最近结束时间"); }
    inline const 词性节点类* 特征_最近耗时() noexcept { return 本能特征词("最近耗时"); }
    inline const 词性节点类* 特征_最近错误码() noexcept { return 本能特征词("最近错误码"); }
    inline const 词性节点类* 特征_最近调用桥接码() noexcept { return 本能特征词("最近调用桥接码"); }
    inline const 词性节点类* 特征_最近质量() noexcept { return 本能特征词("最近质量"); }
    inline const 词性节点类* 特征_最近成本Q() noexcept { return 本能特征词("最近成本"); }
    inline const 词性节点类* 特征_最近成本Us() noexcept { return 本能特征词("最近耗时成本"); }
    inline const 词性节点类* 特征_最近结果场景() noexcept { return 本能特征词("最近结果场景"); }
    inline const 词性节点类* 特征_最近输入场景() noexcept { return 本能特征词("最近输入场景"); }
    inline const 词性节点类* 特征_最近目标存在() noexcept { return 本能特征词("最近目标存在"); }
    inline const 词性节点类* 特征_最近宿主任务() noexcept { return 本能特征词("最近宿主任务"); }
    inline const 词性节点类* 特征_确保阶段状态() noexcept { return 本能特征词("确保阶段状态"); }
    inline const 词性节点类* 特征_解析绑定阶段状态() noexcept { return 本能特征词("解析绑定阶段状态"); }
    inline const 词性节点类* 特征_执行阶段状态() noexcept { return 本能特征词("执行阶段状态"); }
    inline const 词性节点类* 特征_输出阶段状态() noexcept { return 本能特征词("输出阶段状态"); }
    inline const 词性节点类* 特征_确保阶段耗时() noexcept { return 本能特征词("确保阶段耗时"); }
    inline const 词性节点类* 特征_解析绑定阶段耗时() noexcept { return 本能特征词("解析绑定阶段耗时"); }
    inline const 词性节点类* 特征_执行阶段耗时() noexcept { return 本能特征词("执行阶段耗时"); }
    inline const 词性节点类* 特征_输出阶段耗时() noexcept { return 本能特征词("输出阶段耗时"); }
    inline const 词性节点类* 特征_最近失败阶段() noexcept { return 本能特征词("最近失败阶段"); }
    inline const 词性节点类* 特征_运行次数() noexcept { return 本能特征词("运行次数"); }
    inline const 词性节点类* 特征_成功次数() noexcept { return 本能特征词("成功次数"); }
    inline const 词性节点类* 特征_失败次数() noexcept { return 本能特征词("失败次数"); }
    inline const 词性节点类* 特征_异常次数() noexcept { return 本能特征词("异常次数"); }
    inline const 词性节点类* 特征_超时次数() noexcept { return 本能特征词("超时次数"); }
    inline const 词性节点类* 特征_连续失败次数() noexcept { return 本能特征词("连续失败次数"); }
    inline const 词性节点类* 特征_最近成功时间() noexcept { return 本能特征词("最近成功时间"); }
    inline const 词性节点类* 特征_最近失败时间() noexcept { return 本能特征词("最近失败时间"); }
    inline const 词性节点类* 特征_累计成本Us() noexcept { return 本能特征词("累计耗时成本"); }
    inline const 词性节点类* 特征_累计成功耗时Us() noexcept { return 本能特征词("累计成功耗时"); }
    inline const 词性节点类* 特征_缺条件结束次数() noexcept { return 本能特征词("缺条件结束次数"); }
    inline const 词性节点类* 特征_缺方法结束次数() noexcept { return 本能特征词("缺方法结束次数"); }
    inline const 词性节点类* 特征_结果场景变更次数() noexcept { return 本能特征词("结果场景变更次数"); }
    inline const 词性节点类* 特征_功能动作ID() noexcept { return 本能特征词("动作ID"); }
    inline const 词性节点类* 特征_功能缺口类型() noexcept { return 本能特征词("缺口类型"); }
    inline const 词性节点类* 特征_功能需求当前状态() noexcept { return 本能特征词("需求当前状态"); }
    inline const 词性节点类* 特征_功能需求目标状态() noexcept { return 本能特征词("需求目标状态"); }
    inline const 词性节点类* 特征_功能需求方向() noexcept { return 本能特征词("需求方向"); }
    inline const 词性节点类* 特征_功能目标存在() noexcept { return 本能特征词("目标存在"); }
    inline const 词性节点类* 特征_功能宿主任务() noexcept { return 本能特征词("宿主任务"); }
    inline const 词性节点类* 特征_功能方法虚拟存在() noexcept { return 本能特征词("方法虚拟存在"); }

    inline 方法类::节点类* 方法节点(void* 指针) noexcept
    {
        return reinterpret_cast<方法类::节点类*>(指针);
    }

    template<class T节点>
    inline 可解析引用<T节点> 引用(T节点* 节点) noexcept
    {
        可解析引用<T节点> 输出{};
        输出.指针 = 节点;
        if (节点) {
            try {
                输出.主键 = 节点->获取主键();
            }
            catch (...) {
                输出.主键.clear();
            }
        }
        return 输出;
    }

    inline std::string 节点主键(const 基础信息节点类* 节点) noexcept
    {
        if (!节点) {
            return {};
        }
        try {
            return 节点->获取主键();
        }
        catch (...) {
            return {};
        }
    }

    inline std::string 指针键(const void* 指针) noexcept
    {
        return 指针
            ? std::to_string(reinterpret_cast<std::uintptr_t>(指针))
            : std::string{};
    }

    inline std::string 方向安全片段(std::string 文本)
    {
        if (文本.empty()) {
            return "未明";
        }
        for (auto& 字符 : 文本) {
            if (字符 == '\r' || 字符 == '\n' || 字符 == '\t') {
                字符 = ' ';
            }
        }
        if (文本.size() > 160) {
            文本.resize(160);
        }
        return 文本;
    }

    inline std::string 二次特征方向文本(const 二次特征节点类* 节点) noexcept
    {
        if (!节点) {
            return {};
        }
        try {
            const auto* 主信息 = 世界树.二次特征().取二次特征主信息(节点);
            if (!主信息) {
                return 节点主键(reinterpret_cast<const 基础信息节点类*>(节点));
            }
            if (!主信息->离散语义键.empty()) {
                return 主信息->离散语义键;
            }
            if (主信息->方向区间 != 二次特征主信息类::枚举_方向区间::未定义) {
                return 二次特征主信息类::方向区间文本(主信息->方向区间);
            }
            if (!主信息->度量签名_链键.empty()) {
                return 主信息->度量签名_链键;
            }
        }
        catch (...) {
        }
        return 节点主键(reinterpret_cast<const 基础信息节点类*>(节点));
    }

    inline 场景节点类* 取或创建方法样本场景(
        存在节点类* 方法虚拟存在,
        const std::string& 前缀,
        const std::string& 方向签名) noexcept
    {
        if (!方法虚拟存在 || 前缀.empty()) {
            return nullptr;
        }
        try {
            const auto 名称 = 前缀 + "|" + 方向安全片段(方向签名);
            return 世界树.取或创建子场景_按名称(
                reinterpret_cast<基础信息节点类*>(方法虚拟存在),
                语素集.添加词性词(名称, "名词"));
        }
        catch (...) {
            return nullptr;
        }
    }

    inline 二次特征节点类* 取或创建方向二次特征(
        场景节点类* 场景,
        二次特征主信息类::枚举_形态 形态,
        const std::string& 度量签名,
        const std::string& 概念名称,
        std::int32_t 离散编码,
        const std::string& 语义键,
        状态节点类* 当前状态 = nullptr,
        状态节点类* 目标状态 = nullptr) noexcept
    {
        if (!场景 || 度量签名.empty()) {
            return nullptr;
        }
        try {
            二次特征主信息类 模板{};
            模板.形态 = 形态;
            模板.种类 = 形态 == 二次特征主信息类::枚举_形态::结果
                ? 枚举_二次特征种类::状态比较
                : 枚举_二次特征种类::观察关系;
            模板.域 = 枚举_二次特征域::存在;
            模板.粒度 = 枚举_二次特征粒度::当前场景;
            模板.基准类型 = 形态 == 二次特征主信息类::枚举_形态::结果
                ? 枚举_二次特征基准类型::目标态
                : 枚举_二次特征基准类型::无;
            模板.时间归一方式 = 枚举_时间归一方式::无;
            模板.值形态 = 枚举_二次特征值形态::枚举;
            模板.度量签名_链键 = 度量签名;
            模板.概念名称 = 语素集.添加词性词(
                概念名称.empty() ? 语义键 : 概念名称,
                "状态词");
            模板.名称 = 模板.概念名称;
            模板.类型 = 模板.概念名称;
            模板.所属场景 = 引用(场景);
            模板.来源场景 = 引用(场景);
            模板.来源类型 = (当前状态 || 目标状态)
                ? 枚举_二次特征来源类型::状态
                : 枚举_二次特征来源类型::未定义;
            模板.来源起始状态 = 引用(当前状态);
            模板.来源结果状态 = 引用(目标状态);
            模板.左对象 = 引用(reinterpret_cast<基础信息节点类*>(当前状态));
            模板.右对象 = 引用(reinterpret_cast<基础信息节点类*>(目标状态));
            模板.离散编码 = 离散编码;
            模板.离散语义键 = 语义键.empty() ? "未明" : 语义键;
            模板.是否满足 = true;
            模板.置信度 = 10000;
            auto* 节点 = 世界树.二次特征().获取或创建子二次特征_按签名(
                reinterpret_cast<基础信息节点类*>(场景),
                模板);
            if (节点) {
                (void)世界树.二次特征().绑定场景(节点, 场景);
            }
            return 节点;
        }
        catch (...) {
            return nullptr;
        }
    }

    inline void 追加状态引用(
        std::vector<可解析引用<状态节点类>>& 列表,
        状态节点类* 状态) noexcept
    {
        if (!状态) {
            return;
        }
        const auto 键 = 节点主键(reinterpret_cast<const 基础信息节点类*>(状态));
        for (const auto& 已有 : 列表) {
            if ((已有.指针 && 已有.指针 == 状态)
                || (!键.empty() && 已有.主键 == 键)) {
                return;
            }
        }
        auto 新引用 = 引用(状态);
        if (新引用.指针 || !新引用.主键.empty()) {
            列表.push_back(std::move(新引用));
        }
    }

}


export using 枚举_本能动作ID = 枚举_本能方法ID;

export {

inline constexpr std::int64_t 本能动作错误_未注册 = -20001;
inline constexpr std::int64_t 本能动作错误_执行失败 = -20002;
inline constexpr std::int64_t 本能动作错误_动作异常 = -20003;
inline constexpr std::int64_t 本能动作错误_动作未知异常 = -20004;

struct 结构_本能动作执行上下文 {
    void* 世界树 = nullptr;
    void* 场景管理 = nullptr;

    void* 任务 = nullptr;
    void* 任务节点 = nullptr;

    场景节点类* 场景 = nullptr;
    场景节点类* 输入场景 = nullptr;
    场景节点类* 输出场景 = nullptr;
    存在节点类* 目标存在 = nullptr;

    void* 方法根 = nullptr;
    std::uint64_t 请求ID = 0;
};

struct 结构_本能动作步骤结果 {
    枚举_本能动作ID 动作ID = 枚举_本能动作ID::未定义;

    bool 成功 = false;
    std::int64_t 成功码 = 0;
    std::int64_t 置信度 = 0;
    std::int64_t 质量_Q10000 = 0;
    std::uint64_t 成本_us = 0;
    std::int64_t 成本_Q10000 = 0;
    场景节点类* 结果存在集场景 = nullptr;
    场景节点类* 结果事件集场景 = nullptr;
    存在节点类* 方法虚拟存在 = nullptr;
    动态节点类* 动作动态 = nullptr;

    状态节点类* 需求当前状态 = nullptr;
    状态节点类* 需求目标状态 = nullptr;
    二次特征节点类* 需求方向 = nullptr;
    std::string 需求方向说明{};

    std::string 备注{};
};

using 本能动作函数 = std::function<结构_本能动作步骤结果(结构_本能动作执行上下文&)>;
using 兼容场景本能函数 = std::function<bool(场景节点类* 输入场景, 场景节点类* 输出场景)>;
using 本能函数 = 本能动作函数;

struct 结构体_本能动作上下文 {
    本能动作函数 调用点{};
    void* 方法信息首节点 = nullptr;
    std::string 函数名称{};
};

namespace 本能动作管理模块_detail {
    inline bool 写I64(
        方法类::节点类* 方法,
        const 词性节点类* 特征类型,
        I64 值,
        时间戳 now) noexcept
    {
        if (!方法 || !特征类型) {
            return false;
        }
        try {
            return 方法类::记录方法虚拟存在特征(
                方法,
                特征类型,
                特征值{ 值 },
                nullptr,
                now);
        }
        catch (...) {
            return false;
        }
    }

    inline bool 写指针(
        方法类::节点类* 方法,
        const 词性节点类* 特征类型,
        const void* 指针,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!方法 || !特征类型 || !指针) {
            return false;
        }
        try {
            return 方法类::记录方法虚拟存在特征(
                方法,
                特征类型,
                特征值{ 指针句柄{ reinterpret_cast<std::uintptr_t>(指针) } },
                nullptr,
                now,
                调用点);
        }
        catch (...) {
            return false;
        }
    }

    inline I64 读取I64(
        方法类::节点类* 方法,
        const 词性节点类* 特征类型,
        I64 默认值 = 0) noexcept
    {
        if (!方法 || !特征类型) {
            return 默认值;
        }
        try {
            auto* 方法虚拟存在 = 方法->主信息.方法虚拟存在.指针
                ? 方法->主信息.方法虚拟存在.指针
                : 方法类::取或创建_方法虚拟存在(
                    方法,
                    nullptr,
                    结构体_时间戳::当前_微秒(),
                    "本能动作管理类::读取I64/虚拟存在");
            I64 当前值 = 默认值;
            if (方法虚拟存在
                && 世界树.读取特征_I64(
                    reinterpret_cast<const 基础信息节点类*>(方法虚拟存在),
                    特征类型,
                    当前值)) {
                return 当前值;
            }
        }
        catch (...) {
        }
        return 默认值;
    }

    inline void 增量I64(
        方法类::节点类* 方法,
        const 词性节点类* 特征类型,
        I64 增量,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        const auto 当前值 = 读取I64(方法, 特征类型, 0);
        (void)写I64(方法, 特征类型, 当前值 + 增量,now);
    }

    inline bool 写场景I64(
        场景节点类* 场景,
        const 词性节点类* 特征类型,
        I64 值,
        时间戳 now) noexcept
    {
        if (!场景 || !特征类型) {
            return false;
        }
        try {
            return 世界树.写入特征_I64(
                reinterpret_cast<基础信息节点类*>(场景),
                特征类型,
                值,
                now);
        }
        catch (...) {
            return false;
        }
    }

    inline bool 写场景指针(
        场景节点类* 场景,
        const 词性节点类* 特征类型,
        const void* 指针,
        时间戳 now) noexcept
    {
        if (!场景 || !特征类型 || !指针) {
            return false;
        }
        try {
            return 世界树.写入特征_指针(
                reinterpret_cast<基础信息节点类*>(场景),
                特征类型,
                指针,
                now);
        }
        catch (...) {
            return false;
        }
    }

    inline I64 功能缺口类型(const 结构_本能动作步骤结果& 结果) noexcept
    {
        if (结果.成功码 == 本能方法结果_任务缺条件需自我线程) {
            return 1;
        }
        if (结果.成功码 == 本能方法结果_任务缺方法需任务管理任务
            || 结果.成功码 == 本能方法结果_任务筹办已激活学习任务
            || 结果.成功码 == 本能方法结果_任务执行一步已激活学习任务) {
            return 2;
        }
        if (结果.成功码 == 本能方法结果_任务筹办已建立方法头待重入
            || 结果.成功码 == 本能方法结果_任务执行一步已建立方法头待重入
            || 结果.成功码 == 本能方法结果_学习任务已建立任务头待重入) {
            return 3;
        }
        return 0;
    }

    inline void 记录功能结果到结果场景(
        结构_本能动作执行上下文& 上下文,
        const 结构_本能动作步骤结果& 结果,
        时间戳 now) noexcept
    {
        auto* 结果场景 = 结果.结果存在集场景
            ? 结果.结果存在集场景
            : (上下文.输出场景 ? 上下文.输出场景 : 上下文.场景);
        if (!结果场景) {
            return;
        }

        (void)写场景I64(结果场景, 特征_功能动作ID(), static_cast<I64>(结果.动作ID), now);
        (void)写场景I64(结果场景, 特征_功能缺口类型(), 功能缺口类型(结果), now);
        (void)写场景指针(结果场景, 特征_功能需求当前状态(), 结果.需求当前状态, now);
        (void)写场景指针(结果场景, 特征_功能需求目标状态(), 结果.需求目标状态, now);
        (void)写场景指针(结果场景, 特征_功能需求方向(), 结果.需求方向, now);
        (void)写场景指针(结果场景, 特征_功能目标存在(), 上下文.目标存在, now);
        (void)写场景指针(
            结果场景,
            特征_功能宿主任务(),
            上下文.任务节点 ? 上下文.任务节点 : 上下文.任务,
            now);
        (void)写场景指针(结果场景, 特征_功能方法虚拟存在(), 结果.方法虚拟存在, now);
    }

    inline const 词性节点类* 阶段状态特征(枚举_本能运行阶段 阶段) noexcept
    {
        switch (阶段) {
        case 枚举_本能运行阶段::确保: return 特征_确保阶段状态();
        case 枚举_本能运行阶段::解析绑定: return 特征_解析绑定阶段状态();
        case 枚举_本能运行阶段::执行: return 特征_执行阶段状态();
        case 枚举_本能运行阶段::输出: return 特征_输出阶段状态();
        default: return nullptr;
        }
    }

    inline const 词性节点类* 阶段耗时特征(枚举_本能运行阶段 阶段) noexcept
    {
        switch (阶段) {
        case 枚举_本能运行阶段::确保: return 特征_确保阶段耗时();
        case 枚举_本能运行阶段::解析绑定: return 特征_解析绑定阶段耗时();
        case 枚举_本能运行阶段::执行: return 特征_执行阶段耗时();
        case 枚举_本能运行阶段::输出: return 特征_输出阶段耗时();
        default: return nullptr;
        }
    }

    inline void 记录阶段(
        方法类::节点类* 方法,
        枚举_本能运行阶段 阶段,
        枚举_本能运行状态 状态,
        时间戳 开始,
        时间戳 结束,
        const std::string& 调用点) noexcept
    {
        const auto now = 结束 != 0 ? 结束 : 结构体_时间戳::当前_微秒();
        (void)写I64(方法, 特征_运行阶段(), static_cast<I64>(阶段),开始);
        if (const auto* 状态特征 = 阶段状态特征(阶段)) {
            (void)写I64(方法, 状态特征, static_cast<I64>(状态),开始);
        }
        if (const auto* 耗时特征 = 阶段耗时特征(阶段)) {
            const auto 耗时 = 开始 != 0 && now >= 开始 ? static_cast<I64>(now - 开始) : 0;
            (void)写I64(方法, 耗时特征, 耗时,结束);
        }
    }

    inline 方法类::节点类* 解析方法节点(
        const 结构_本能动作执行上下文& 上下文,
        const 结构体_本能动作上下文* 注册上下文 = nullptr) noexcept
    {
        if (auto* 方法 = 方法节点(上下文.方法根)) {
            return 方法;
        }
        return 注册上下文 ? 方法节点(注册上下文->方法信息首节点) : nullptr;
    }

    inline 存在节点类* 确保方法虚拟存在(
        方法类::节点类* 方法,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!方法) {
            return nullptr;
        }
        try {
            (void)方法类::初始化方法虚拟存在信息(
                方法,
                nullptr,
                now);
            return 方法类::取或创建_方法虚拟存在(
                方法,
                nullptr,
                now);
        }
        catch (...) {
            return nullptr;
        }
    }


} // namespace 本能动作管理模块_detail


class 本能动作管理类 {
public:
    bool 注册(
        枚举_本能动作ID 动作ID,
        本能动作函数 动作函数,
        std::string 函数名称 = {},
        void* 方法首节点 = nullptr)
    {
        if (动作ID == 枚举_本能动作ID::未定义) return false;
        if (!动作函数) return false;
        if (函数名称.empty()) {
            函数名称 = 本能动作管理模块_detail::默认函数名称(动作ID);
        }
        if (函数名称.empty()) return false;

        结构体_本能动作上下文 上下文{};
        上下文.调用点 = std::move(动作函数);
        上下文.方法信息首节点 = 方法首节点;
        上下文.函数名称 = std::move(函数名称);

        std::lock_guard 锁(mu_);
        表_[动作ID] = std::move(上下文);
        return true;
    }

    bool 注册_兼容(
        枚举_本能动作ID 动作ID,
        兼容场景本能函数 动作函数,
        std::string 函数名称 = {},
        void* 方法首节点 = nullptr)
    {
        if (!动作函数) return false;

        return 注册(
            动作ID,
            [动作函数 = std::move(动作函数), 动作ID](结构_本能动作执行上下文& 上下文) mutable {
                结构_本能动作步骤结果 结果{};
                结果.动作ID = 动作ID;
                结果.结果存在集场景 = 上下文.输出场景 ? 上下文.输出场景 : 上下文.场景;
                结果.结果事件集场景 = 结果.结果存在集场景;
                if (动作函数(上下文.输入场景, 上下文.输出场景)) {
                    return 结果;
                }

                结果.成功码 = 本能动作错误_执行失败;
                结果.备注 = "兼容场景本能函数返回失败";
                return 结果;
            },
            std::move(函数名称),
            方法首节点);
    }

    bool 补全方法信息(枚举_本能动作ID 动作ID, void* 方法首节点)
    {
        std::lock_guard 锁(mu_);
        const auto it = 表_.find(动作ID);
        if (it == 表_.end()) return false;
        it->second.方法信息首节点 = 方法首节点;
        return true;
    }

    bool 注销(枚举_本能动作ID 动作ID)
    {
        std::lock_guard 锁(mu_);
        return 表_.erase(动作ID) > 0;
    }

    bool 有(枚举_本能动作ID 动作ID) const
    {
        std::lock_guard 锁(mu_);
        return 表_.find(动作ID) != 表_.end();
    }

    std::optional<结构体_本能动作上下文> 查询(枚举_本能动作ID 动作ID) const
    {
        std::lock_guard 锁(mu_);
        const auto it = 表_.find(动作ID);
        if (it == 表_.end()) return std::nullopt;
        return it->second;
    }

    std::optional<std::pair<枚举_本能动作ID, 结构体_本能动作上下文>> 查询_按函数名(const std::string& 函数名称) const
    {
        if (函数名称.empty()) return std::nullopt;

        std::lock_guard 锁(mu_);
        for (const auto& [动作ID, 上下文] : 表_) {
            if (上下文.函数名称 == 函数名称) {
                return std::make_pair(动作ID, 上下文);
            }
        }
        return std::nullopt;
    }

    std::string 查询函数名称(枚举_本能动作ID 动作ID) const
    {
        {
            std::lock_guard 锁(mu_);
            const auto it = 表_.find(动作ID);
            if (it != 表_.end() && !it->second.函数名称.empty()) {
                return it->second.函数名称;
            }
        }
        return 本能动作管理模块_detail::默认函数名称(动作ID);
    }

    枚举_本能动作ID 查询ID(const std::string& 函数名称) const
    {
        if (函数名称.empty()) return 枚举_本能动作ID::未定义;

        {
            std::lock_guard 锁(mu_);
            for (const auto& [动作ID, 上下文] : 表_) {
                if (上下文.函数名称 == 函数名称) {
                    return 动作ID;
                }
            }
        }

        for (const auto 动作ID : 本能动作管理模块_detail::默认动作ID列表()) {
            if (本能动作管理模块_detail::默认函数名称(动作ID) == 函数名称) {
                return 动作ID;
            }
        }
        return 枚举_本能动作ID::未定义;
    }

    std::vector<枚举_本能动作ID> 枚举已注册ID() const
    {
        std::vector<枚举_本能动作ID> 输出{};
        std::lock_guard 锁(mu_);
        输出.reserve(表_.size());
        for (const auto& [动作ID, _] : 表_) {
            输出.push_back(动作ID);
        }
        std::sort(输出.begin(), 输出.end(), [](const auto 左, const auto 右) {
            return static_cast<std::uint32_t>(左) < static_cast<std::uint32_t>(右);
        });
        return 输出;
    }

    bool 运行(
        枚举_本能动作ID 动作ID,
        结构_本能动作执行上下文& 上下文,
        结构_本能动作步骤结果* 输出 = nullptr) const noexcept
    {
        auto 已注册动作 = 查询(动作ID);
        auto* 方法 = 本能动作管理模块_detail::解析方法节点(
            上下文,
            已注册动作.has_value() ? &(*已注册动作) : nullptr);
        const auto 开始 = 结构体_时间戳::当前_微秒();
       
        if (!已注册动作.has_value() || !已注册动作->调用点) {
            结构_本能动作步骤结果 结果{};
            结果.动作ID = 动作ID;
            结果.成功码 = 本能动作错误_未注册;
            结果.备注 = "本能动作未注册";
           
            if (输出) {
                *输出 = std::move(结果);
            }
            return false;
        }

        try {
            auto 结果 = 已注册动作->调用点(上下文);
            if (结果.动作ID == 枚举_本能动作ID::未定义) {
                结果.动作ID = 动作ID;
            }
           
            const bool 成功 = 结果.成功码 == 0;
            if (输出) {
                *输出 = std::move(结果);
            }
            return 成功;
        }
        catch (const std::exception& 异常) {
            结构_本能动作步骤结果 结果{};
            结果.动作ID = 动作ID;
            结果.成功码 = 本能动作错误_动作异常;
            结果.备注 = 异常.what();
           
            if (输出) {
                *输出 = std::move(结果);
            }
            return false;
        }
        catch (...) {
            结构_本能动作步骤结果 结果{};
            结果.动作ID = 动作ID;
            结果.成功码 = 本能动作错误_动作未知异常;
            结果.备注 = "本能动作抛出未知异常";
          
            if (输出) {
                *输出 = std::move(结果);
            }
            return false;
        }
    }

    bool 运行_兼容(
        枚举_本能动作ID 动作ID,
        场景节点类* 输入场景,
        场景节点类* 输出场景,
        结构_本能动作步骤结果* 输出 = nullptr) const noexcept
    {
        结构_本能动作执行上下文 上下文{};
        上下文.输入场景 = 输入场景;
        上下文.输出场景 = 输出场景 ? 输出场景 : 输入场景;
        上下文.场景 = 上下文.输出场景 ? 上下文.输出场景 : 上下文.输入场景;
        return 运行(动作ID, 上下文, 输出);
    }

    void 调试清空运行期缓存() noexcept
    {
        std::lock_guard 锁(mu_);
        表_.clear();
    }

private:
    mutable std::mutex mu_{};
    std::unordered_map<枚举_本能动作ID, 结构体_本能动作上下文> 表_{};
};

inline const 结构_本能方法元信息* 查询本能动作元信息(枚举_本能动作ID 动作ID) noexcept
{
    return 本能方法类::查询元信息(动作ID);
}

inline std::vector<枚举_本能动作ID> 枚举默认本能动作ID()
{
    return 本能方法类::枚举默认自我本能方法ID();
}

inline 本能动作管理类& 取本能动作集() noexcept
{
    static 本能动作管理类 s_实例{};
    return s_实例;
}

} // export
