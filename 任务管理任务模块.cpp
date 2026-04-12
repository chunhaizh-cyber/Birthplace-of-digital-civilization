#include "任务管理任务模块.h"

#include <cassert>
#include <functional>
#include <mutex>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "本能方法类.h"
#include "学习任务模块.h"
#include "自我初始化模块.h"
#include "自我类.h"
#include "任务管理最小原语模块.h"
#include "语素类.h"
#include "任务管理任务模块_治理快照.cpp"

namespace 任务管理任务模块_治理缓存 {
    void 缓存最近治理结果(const 结构_任务管理结果& 结果) noexcept;
}

namespace {
    using 任务节点 = 任务类::节点类;
    using 方法节点 = 方法类::节点类;
    using 需求节点 = 需求类::节点类;

    enum class 枚举_任务管理特征目标宿主 {
        未定义 = 0,
        宿主任务治理,
        主体桥接,
        任务账本,
        方法资产,
    };

    struct 结构_M2治理阶段状态 {
        任务节点* 当前管理任务 = nullptr;
        任务节点* 宿主任务 = nullptr;
        任务节点* 待回接业务子任务 = nullptr;
        bool 已受上位输入约束 = false;
        bool 桥接补层允许继续 = false;
    };

    struct 结构_M9请求触发态 {
        bool 命中显式事件 = false;
        结构_任务管理显式事件控制 显式事件{};
        bool 命中恢复重建 = false;
        结构_治理恢复快照 恢复快照{};
        结构_任务管理半白盒控制 半白盒控制{};
    };

    enum class 枚举_最小原语写回目标宿主 : std::uint8_t {
        按语义宿主 = 0,
        宿主任务治理 = 1,
        主体桥接 = 2,
        方法资产 = 3,
    };

    struct 结构_最小原语边界执行上下文 {
        自我类* 自我对象 = nullptr;
        任务节点* 宿主任务 = nullptr;
        时间戳 当前时间 = 0;
        const std::string* 调用点 = nullptr;
        结构_任务管理结果* 结果 = nullptr;
        枚举_任务管理最小原语ID 原语ID = 枚举_任务管理最小原语ID::未定义;
        枚举_动作事件相位 相位 = 枚举_动作事件相位::未定义;
    };

    struct 结构_最小原语I64写回执行上下文 {
        自我类* 自我对象 = nullptr;
        枚举_最小原语写回目标宿主 目标宿主 = 枚举_最小原语写回目标宿主::按语义宿主;
        const 词性节点类* 特征类型 = nullptr;
        I64 新值 = 0;
        const std::string* 动作语义键 = nullptr;
        方法节点* 来源方法 = nullptr;
        任务节点* 来源任务 = nullptr;
        时间戳 当前时间 = 0;
        const std::string* 调用点 = nullptr;
        结构_任务管理结果* 结果 = nullptr;
    };

    struct 结构_最小原语指针写回执行上下文 {
        自我类* 自我对象 = nullptr;
        枚举_最小原语写回目标宿主 目标宿主 = 枚举_最小原语写回目标宿主::按语义宿主;
        const 词性节点类* 特征类型 = nullptr;
        const void* 新指针值 = nullptr;
        const std::string* 新值标题 = nullptr;
        const std::string* 动作语义键 = nullptr;
        方法节点* 来源方法 = nullptr;
        任务节点* 来源任务 = nullptr;
        时间戳 当前时间 = 0;
        const std::string* 调用点 = nullptr;
        结构_任务管理结果* 结果 = nullptr;
        const 词性节点类* 来源动作名覆盖 = nullptr;
        枚举_动作事件相位 来源相位 = 枚举_动作事件相位::完成;
    };

    void 私有_追加最近反馈片段(
        结构_任务管理结果& 结果,
        const std::string& 片段) noexcept;
    void 私有_M9_装配请求基本骨架(
        const 结构_任务管理上下文& 上下文,
        时间戳 当前时间,
        std::uint32_t 提交序号,
        结构_任务管理请求& 输出) noexcept;
    结构_M9请求触发态 私有_M9_消费请求触发态() noexcept;
    void 私有_M9_装配请求触发信息(
        结构_任务管理请求& 输出,
        const 结构_M9请求触发态& 触发态) noexcept;
    void 私有_M9_生成恢复重建请求摘要(
        结构_任务管理请求& 输出,
        const 结构_M9请求触发态& 触发态) noexcept;
    void 私有_M9_生成请求补充材料摘要(
        const 结构_任务管理上下文& 上下文,
        结构_任务管理请求& 输出) noexcept;
    std::string 私有_M10_主体长期策略摘要(
        const 结构_任务管理主体虚拟存在& 主体) noexcept;
    std::string 私有_M10_任务状态签名(const 状态节点类* 状态) noexcept;
    std::string 私有_M10_业务差额签名(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept;
    std::string 私有_M10_当前条件签名(const 结构_任务管理结果& 结果) noexcept;
    std::string 私有_M10_有效正式方法标题(const 结构_任务管理结果& 结果) noexcept;
    std::string 私有_M10_长期策略摘要版本(const 结构_任务管理结果& 结果) noexcept;
    bool 私有_M10_存在业务推进压力(const 结构_任务管理结果& 结果) noexcept;
    bool 私有_M10_当前方法命中主体长期策略(const 结构_任务管理结果& 结果) noexcept;
    bool 私有_M10_长期策略驱动业务子任务派生(const 结构_任务管理结果& 结果) noexcept;
    bool 私有_M10_长期策略可靠度允许回填(const 结构_任务管理主体虚拟存在& 主体) noexcept;
    bool 私有_M10_主体长期策略允许直放后段(const 结构_任务管理结果& 结果) noexcept;
    void 私有_M10_刷新长期策略权重(结构_任务管理主体虚拟存在& 主体) noexcept;
    void 私有_M10_合并长期策略范围签名(
        std::string& 范围摘要,
        const std::string& 新签名) noexcept;
    bool 私有_M10_当前轮形成有效长期策略推进(const 结构_任务管理结果& 结果) noexcept;
    std::string 私有_M10_当前轮长期策略失效原因(const 结构_任务管理结果& 结果) noexcept;
    void 私有_M10_回写长期策略可靠度(
        任务节点* 宿主任务,
        时间戳 当前时间,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_M10_沉淀正式学习回流到主体(
        自我类& 自我对象,
        const 结构_学习方法资产提交输入& 输入,
        const 结构_学习方法资产提交结果& 资产结果,
        时间戳 当前时间,
        结构_任务管理主体虚拟存在* 输出主体 = nullptr,
        std::string* 输出摘要 = nullptr) noexcept;
    bool 私有_M9_执行前段治理阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept;
    bool 私有_M9_事件轮禁止业务扩张(const 结构_任务管理结果& 结果) noexcept;
    void 私有_M9_执行后段治理动作阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept;
    void 私有_M9_执行学习承接与业务派生阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept;

    bool 私有_读取状态节点任务状态(const 状态节点类* 状态节点, 枚举_任务状态& 输出) noexcept;
    bool 私有_任务节点归属宿主任务(const 任务节点* 节点, const 任务节点* 宿主任务) noexcept;
    bool 私有_方法属于宿主任务(const 方法节点* 方法, const 任务节点* 宿主任务) noexcept;
    void 私有_按宿主清洗治理上下文镜像(结构_任务管理上下文& 上下文) noexcept;
    bool 私有_任务节点目标结果已对齐(const 任务节点* 任务) noexcept;
    bool 私有_任务节点目标结果待推进(const 任务节点* 任务) noexcept;
    枚举_任务管理本能能力缺口类型 私有_P0_当前本能缺口(const 结构_任务管理结果& 结果) noexcept;
    void 私有_设定治理结论(
        结构_任务管理结果& 结果,
        枚举_任务管理功能域 功能域,
        枚举_任务管理缺口类型 缺口类型,
        枚举_任务管理下一步去向 下一步去向,
        枚举_任务管理治理状态迁移 状态迁移,
        枚举_任务管理总控结果 总控结果,
        枚举_任务管理反馈类型 反馈类型,
        std::string 反馈摘要) noexcept;
    void 私有_刷新分身继承位图(
        结构_任务管理分身继承面& 分身,
        const 结构_任务管理上下文& 上下文) noexcept;
    std::string 私有_主体化输入条件主键(
        const 结构_任务管理上下文& 上下文,
        const 结构_任务管理分身继承面& 分身) noexcept;
    std::string 私有_主体化结果主键(
        const 结构_任务管理分身继承面& 分身,
        const 结构_任务管理上下文& 上下文) noexcept;
    枚举_任务管理特征目标宿主 私有_查询任务管理特征目标宿主(
        const 词性节点类* 特征类型) noexcept;
    bool 私有_校验任务管理特征目标宿主(
        const 词性节点类* 特征类型,
        枚举_任务管理特征目标宿主 期望宿主) noexcept;
    const 特征节点类* 私有_查找宿主任务治理特征节点(
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型) noexcept;
    特征节点类* 私有_确保宿主任务治理特征节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept;
    bool 私有_读取宿主任务治理I64特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64& 输出值,
        const std::string& 调用点) noexcept;
    bool 私有_读取宿主任务治理指针特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        std::uintptr_t& 输出值,
        const std::string& 调用点) noexcept;
    const 特征节点类* 私有_查找主体桥接特征节点(
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型) noexcept;
    特征节点类* 私有_确保主体桥接特征节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept;
    const 特征节点类* 私有_查找方法资产特征节点(
        const 方法节点* 方法头,
        const 词性节点类* 特征类型) noexcept;
    特征节点类* 私有_确保方法资产特征节点(
        方法节点* 方法头,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept;
    bool 私有_读取主体桥接I64特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64& 输出值,
        const std::string& 调用点) noexcept;
    bool 私有_读取主体桥接指针特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        std::uintptr_t& 输出值,
        const std::string& 调用点) noexcept;
    场景节点类* 私有_任务场景(自我类& 自我对象, const 任务节点* 节点) noexcept;
    存在节点类* 私有_确保任务虚拟存在(
        自我类& 自我对象,
        任务节点* 任务,
        const std::string& 调用点) noexcept;
    bool 私有_记录宿主任务治理证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_记录宿主任务治理证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖 = nullptr,
        枚举_动作事件相位 来源相位 = 枚举_动作事件相位::完成) noexcept;
    bool 私有_记录主体桥接证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_记录方法资产证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_记录主体桥接证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖 = nullptr,
        枚举_动作事件相位 来源相位 = 枚举_动作事件相位::完成) noexcept;
    bool 私有_按语义宿主分派记录治理证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_按语义宿主分派记录治理证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖 = nullptr,
        枚举_动作事件相位 来源相位 = 枚举_动作事件相位::完成) noexcept;
    bool 私有_写入宿主任务治理当前值_I64(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64 新值,
        时间戳 now,
        const std::string& 调用点) noexcept;
    bool 私有_写入宿主任务治理当前值_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        时间戳 now,
        const std::string& 调用点) noexcept;
    bool 私有_写入宿主任务治理当前值_指针(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        时间戳 now,
        const std::string& 调用点) noexcept;
    bool 私有_写入宿主任务治理当前值_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        时间戳 now,
        const std::string& 调用点) noexcept;
    bool 私有_记录任务管理方法位专项动态(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        枚举_动作事件相位 相位) noexcept;
    bool 私有_执行最小原语边界记录体(void* 上下文) noexcept;
    bool 私有_执行最小原语I64写回体(void* 上下文) noexcept;
    bool 私有_执行最小原语指针写回体(void* 上下文) noexcept;
    bool 私有_通过执行壳写回I64(
        自我类& 自我对象,
        枚举_最小原语写回目标宿主 目标宿主,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_通过执行壳写回指针(
        自我类& 自我对象,
        枚举_最小原语写回目标宿主 目标宿主,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖 = nullptr,
        枚举_动作事件相位 来源相位 = 枚举_动作事件相位::完成) noexcept;
    void 私有_写回治理稳定字段(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;

    std::mutex 私有_任务管理请求序号锁{};
    std::uint32_t 私有_任务管理请求序号 = 0;
    std::mutex 私有_任务管理主体表锁{};
    std::unordered_map<std::uint64_t, 结构_任务管理主体虚拟存在> 私有_任务管理主体表{};
    std::mutex 私有_半白盒治理控制锁{};
    结构_任务管理半白盒控制 私有_半白盒治理控制{};
    std::mutex 私有_治理显式事件控制锁{};
    结构_任务管理显式事件控制 私有_治理显式事件控制{};
    struct 结构_任务管理恢复重建控制 {
        bool 启用 = false;
        结构_治理恢复快照 快照{};
    };
    std::mutex 私有_治理恢复重建控制锁{};
    结构_任务管理恢复重建控制 私有_治理恢复重建控制{};

    void 私有_写入引用(可解析引用<方法节点类>& 输出, 方法节点* 节点) noexcept
    {
        输出.指针 = reinterpret_cast<方法节点类*>(节点);
        输出.主键 = 节点 ? 节点->获取主键() : std::string{};
    }

    void 私有_写入引用(可解析引用<任务节点类>& 输出, 任务节点* 节点) noexcept
    {
        输出.指针 = reinterpret_cast<任务节点类*>(节点);
        输出.主键 = 节点 ? 节点->获取主键() : std::string{};
    }

    template<class T节点>
    void 私有_写入引用(可解析引用<T节点>& 输出, T节点* 节点) noexcept
    {
        输出.指针 = 节点;
        输出.主键 = 节点 ? 节点->获取主键() : std::string{};
    }

    template<class T节点, class T谓词>
    T节点* 私有_查找直接子节点_按谓词(T节点* 父节点, T谓词&& 谓词) noexcept
    {
        if (!父节点 || !父节点->子) return nullptr;

        auto* 首节点 = static_cast<T节点*>(父节点->子);
        auto* 当前 = 首节点;
        do {
            if (谓词(当前)) return 当前;
            当前 = static_cast<T节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    template<class T节点, class T回调>
    void 私有_枚举直接子节点(const T节点* 父节点, T回调&& 回调) noexcept
    {
        if (!父节点 || !父节点->子) {
            return;
        }

        auto* 首节点 = static_cast<const T节点*>(父节点->子);
        auto* 当前 = 首节点;
        do {
            回调(当前);
            当前 = static_cast<const T节点*>(当前->下);
        } while (当前 && 当前 != 首节点);
    }

    bool 私有_词性相同(const 词性节点类* a, const 词性节点类* b) noexcept
    {
        if (a == b) return true;
        if (!a || !b) return false;
        return a->获取主键() == b->获取主键();
    }

    时间戳 私有_归一化时间(时间戳 now) noexcept
    {
        return now != 0 ? now : 结构体_时间戳::当前_微秒();
    }

    const 词性节点类* 私有_管理特征_当前功能域() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前功能域", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前缺口类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前缺口类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前下一步去向() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前下一步去向", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前治理状态迁移() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前治理状态迁移", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近功能域() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近功能域", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近总控结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近总控结果", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近反馈类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近反馈类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近反馈摘要() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近反馈摘要", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近根层重判结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近根层重判结果", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近执行前门控结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近执行前门控结果", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近上层反馈摘要() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近上层反馈摘要", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前方法来源() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前方法来源", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前步骤位类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前步骤位类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前宿主目标结果比较() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前宿主目标结果比较", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前后果治理() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前后果治理", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前主体ID() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前主体ID", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前分身ID() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前分身ID", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前方法需求位() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前方法需求位", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前本能能力缺口类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前本能能力缺口类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前输入条件主键() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前输入条件主键", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前治理态型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前治理态型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前最小原语动态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前最小原语动态", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前宿主任务指针() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前宿主任务指针", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前方法指针() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前方法指针", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前方法位专项动态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前方法位专项动态", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前步骤指针() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前步骤指针", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近结果指针() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近结果指针", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_宿主任务状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_宿主任务状态", "名词");
        return s_词;
    }

    const 词性节点类* 私有_任务特征_目标状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务目标状态", "名词");
        return s_词;
    }

    const 词性节点类* 私有_任务特征_结果状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务结果状态", "名词");
        return s_词;
    }

    const 词性节点类* 私有_方法特征_方法类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法类型", "名词");
        return s_词;
    }

    枚举_任务管理特征目标宿主 私有_查询任务管理特征目标宿主(
        const 词性节点类* 特征类型) noexcept
    {
        if (!特征类型) {
            return 枚举_任务管理特征目标宿主::未定义;
        }

        if (特征类型 == 私有_任务特征_目标状态()
            || 特征类型 == 私有_任务特征_结果状态()) {
            return 枚举_任务管理特征目标宿主::任务账本;
        }

        if (特征类型 == 私有_方法特征_方法类型()) {
            return 枚举_任务管理特征目标宿主::方法资产;
        }

        if (特征类型 == 私有_管理特征_当前主体ID()
            || 特征类型 == 私有_管理特征_当前分身ID()
            || 特征类型 == 私有_管理特征_当前方法需求位()
            || 特征类型 == 私有_管理特征_当前本能能力缺口类型()
            || 特征类型 == 私有_管理特征_当前输入条件主键()
            || 特征类型 == 私有_管理特征_当前治理态型()
            || 特征类型 == 私有_管理特征_当前方法位专项动态()
            || 特征类型 == 私有_管理特征_当前最小原语动态()) {
            return 枚举_任务管理特征目标宿主::主体桥接;
        }

        if (特征类型 == 私有_管理特征_当前功能域()
            || 特征类型 == 私有_管理特征_当前缺口类型()
            || 特征类型 == 私有_管理特征_当前下一步去向()
            || 特征类型 == 私有_管理特征_当前治理状态迁移()
            || 特征类型 == 私有_管理特征_最近功能域()
            || 特征类型 == 私有_管理特征_最近总控结果()
            || 特征类型 == 私有_管理特征_最近反馈类型()
            || 特征类型 == 私有_管理特征_最近反馈摘要()
            || 特征类型 == 私有_管理特征_最近根层重判结果()
            || 特征类型 == 私有_管理特征_最近执行前门控结果()
            || 特征类型 == 私有_管理特征_最近上层反馈摘要()
            || 特征类型 == 私有_管理特征_当前方法来源()
            || 特征类型 == 私有_管理特征_当前步骤位类型()
            || 特征类型 == 私有_管理特征_当前宿主目标结果比较()
            || 特征类型 == 私有_管理特征_当前后果治理()
            || 特征类型 == 私有_管理特征_当前宿主任务指针()
            || 特征类型 == 私有_管理特征_当前方法指针()
            || 特征类型 == 私有_管理特征_当前步骤指针()
            || 特征类型 == 私有_管理特征_最近结果指针()
            || 特征类型 == 私有_管理特征_宿主任务状态()) {
            return 枚举_任务管理特征目标宿主::宿主任务治理;
        }

        return 枚举_任务管理特征目标宿主::未定义;
    }

    bool 私有_校验任务管理特征目标宿主(
        const 词性节点类* 特征类型,
        枚举_任务管理特征目标宿主 期望宿主) noexcept
    {
#ifndef NDEBUG
        const auto 实际宿主 = 私有_查询任务管理特征目标宿主(特征类型);
        assert(
            实际宿主 == 期望宿主
            && "任务管理特征宿主映射与显式写入口不一致");
#else
        (void)特征类型;
        (void)期望宿主;
#endif
        return true;
    }

    const 词性节点类* 私有_任务虚拟存在类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务虚拟存在", "名词");
        return s_词;
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

    std::string 私有_拼接片段(const std::vector<std::string>& 片段)
    {
        std::ostringstream 输出;
        bool 首段 = true;
        for (const auto& 片段文本 : 片段) {
            if (片段文本.empty()) {
                continue;
            }
            if (!首段) {
                输出 << "；";
            }
            输出 << 片段文本;
            首段 = false;
        }
        return 输出.str();
    }

    std::string 私有_任务标题(const 任务节点* 节点) noexcept
    {
        if (!节点) return "空";
        const auto 标题 = 私有_安全词(节点->主信息.名称);
        return 标题.empty() ? std::string("任务#") + 节点->获取主键() : 标题;
    }

    const 词性节点类* 私有_任务虚拟存在名称词(const 任务节点* 节点) noexcept
    {
        return 语素集.添加词性词("任务账本_" + 私有_任务标题(节点), "专有名词");
    }

    std::string 私有_方法标题(const 方法节点* 节点) noexcept
    {
        if (!节点) return "空";
        const auto 标题 = 私有_安全词(节点->主信息.动作名);
        return 标题.empty() ? std::string("方法#") + 节点->获取主键() : 标题;
    }

    template<class T节点>
    T节点* 私有_获取独立树根(T节点* 节点) noexcept
    {
        while (节点 && 节点->父) {
            节点 = static_cast<T节点*>(节点->父);
        }
        return 节点;
    }

    方法节点* 私有_独立树添加方法子节点(方法节点* 父节点, const 方法主信息类& 主信息)
    {
        if (!父节点) return nullptr;

        auto* 独立根 = 私有_获取独立树根(父节点);
        if (!独立根) return nullptr;

        方法类 临时树{};
        auto* 临时根 = 临时树.根指针;
        临时树.根指针 = 独立根;

        auto* 新节点 = static_cast<方法节点*>(临时树.添加子节点(父节点, 主信息));

        临时树.根指针 = 临时根;
        return 新节点;
    }

    template<class T树, class T节点, class T主信息>
    T节点* 私有_独立树添加子节点(T节点* 父节点, const T主信息& 主信息)
    {
        if (!父节点) return nullptr;

        auto* 独立根 = 私有_获取独立树根(父节点);
        if (!独立根) return nullptr;

        T树 临时树{};
        auto* 临时根 = 临时树.根指针;
        临时树.根指针 = 独立根;

        auto* 新节点 = static_cast<T节点*>(临时树.添加子节点(父节点, 主信息));

        临时树.根指针 = 临时根;
        return 新节点;
    }

    template<class T节点>
    bool 私有_是祖先节点(const T节点* 祖先节点, const T节点* 当前节点) noexcept
    {
        auto* 游标 = 当前节点;
        while (游标) {
            if (游标 == 祖先节点) {
                return true;
            }
            游标 = static_cast<const T节点*>(游标->父);
        }
        return false;
    }

    std::string 私有_任务状态文本(枚举_任务状态 状态) noexcept
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

    std::string 私有_功能域文本(枚举_任务管理功能域 域) noexcept
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

    std::string 私有_缺口文本(枚举_任务管理缺口类型 缺口) noexcept
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

    std::string 私有_去向文本(枚举_任务管理下一步去向 去向) noexcept
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

    std::string 私有_治理状态迁移文本(枚举_任务管理治理状态迁移 迁移) noexcept
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

    std::string 私有_总控结果文本(枚举_任务管理总控结果 结果) noexcept
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

    std::string 私有_根层重判结果文本(枚举_任务管理根层重判结果 结果) noexcept
    {
        switch (结果) {
        case 枚举_任务管理根层重判结果::允许普通推进: return "允许普通推进";
        case 枚举_任务管理根层重判结果::待机保持: return "待机保持";
        case 枚举_任务管理根层重判结果::收束优先: return "收束优先";
        case 枚举_任务管理根层重判结果::停止退出边界: return "停止退出边界";
        default: return "未定义";
        }
    }

    std::string 私有_执行前门控结果文本(枚举_任务管理执行前门控结果 结果) noexcept
    {
        switch (结果) {
        case 枚举_任务管理执行前门控结果::允许普通执行: return "允许普通执行";
        case 枚举_任务管理执行前门控结果::仅允许等待: return "仅允许等待";
        case 枚举_任务管理执行前门控结果::仅允许收束: return "仅允许收束";
        case 枚举_任务管理执行前门控结果::禁止继续: return "禁止继续";
        default: return "未定义";
        }
    }

    std::string 私有_反馈类型文本(枚举_任务管理反馈类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_任务管理反馈类型::治理推进: return "治理推进";
        case 枚举_任务管理反馈类型::等待维持: return "等待维持";
        case 枚举_任务管理反馈类型::收束回执: return "收束回执";
        case 枚举_任务管理反馈类型::重筹办提示: return "重筹办提示";
        default: return "未定义";
        }
    }

    std::string 私有_方法来源文本(枚举_任务管理方法来源 来源) noexcept
    {
        switch (来源) {
        case 枚举_任务管理方法来源::已挂方法复用: return "已挂方法复用";
        case 枚举_任务管理方法来源::当前主方法回填: return "当前主方法回填";
        case 枚举_任务管理方法来源::最近结果回看: return "最近结果回看";
        case 枚举_任务管理方法来源::主体长期策略回填: return "主体长期策略回填";
        case 枚举_任务管理方法来源::方法树首节点候选: return "方法树首节点候选";
        case 枚举_任务管理方法来源::条件节点筛选通过: return "条件节点筛选通过";
        case 枚举_任务管理方法来源::默认本能兜底: return "默认本能兜底";
        default: return "未定义";
        }
    }

    std::string 私有_步骤位类型文本(枚举_任务管理步骤位类型 位类型) noexcept
    {
        switch (位类型) {
        case 枚举_任务管理步骤位类型::执行位: return "执行位";
        case 枚举_任务管理步骤位类型::等待位: return "等待位";
        case 枚举_任务管理步骤位类型::回接位: return "回接位";
        case 枚举_任务管理步骤位类型::补条件位: return "补条件位";
        default: return "未定义";
        }
    }

    std::string 私有_步骤语义类型文本(枚举_任务管理步骤语义类型 类型) noexcept
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

    std::string 私有_双面比较结果文本(枚举_任务管理双面比较结果 结果) noexcept
    {
        switch (结果) {
        case 枚举_任务管理双面比较结果::待补齐: return "待补齐";
        case 枚举_任务管理双面比较结果::已对齐: return "已对齐";
        case 枚举_任务管理双面比较结果::存在差额: return "存在差额";
        default: return "未定义";
        }
    }

    std::string 私有_后果治理文本(枚举_任务管理后果治理 结果) noexcept
    {
        switch (结果) {
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

    std::uint64_t 私有_安全解析U64(const std::string& 文本) noexcept
    {
        if (文本.empty()) {
            return 0;
        }

        try {
            return static_cast<std::uint64_t>(std::stoull(文本));
        }
        catch (...) {
            return 0;
        }
    }

    constexpr std::uint64_t 私有_原语位(枚举_任务管理最小原语ID 原语ID) noexcept
    {
        switch (原语ID) {
        case 枚举_任务管理最小原语ID::确保单承载存在: return 1ULL << 0;
        case 枚举_任务管理最小原语ID::读取单特征当前值: return 1ULL << 1;
        case 枚举_任务管理最小原语ID::写入单特征当前值: return 1ULL << 2;
        case 枚举_任务管理最小原语ID::写入单挂点指针: return 1ULL << 3;
        case 枚举_任务管理最小原语ID::记录动作开始: return 1ULL << 4;
        case 枚举_任务管理最小原语ID::记录动作结束: return 1ULL << 5;
        default: return 0;
        }
    }

    枚举_本能方法ID 私有_最小原语对应本能方法ID(
        枚举_任务管理最小原语ID 原语ID) noexcept
    {
        return 任务管理最小原语模块::查询对应本能方法ID(原语ID);
    }

    constexpr std::uint64_t 私有_派生能力位_状态差分取证 = 1ULL << 0;
    constexpr std::uint64_t 私有_派生能力位_实例因果派生 = 1ULL << 1;
    constexpr std::uint64_t 私有_派生能力位_轻量因果链组织 = 1ULL << 2;

    const 结构_任务管理最小原语签名* 私有_最小原语签名表() noexcept
    {
        static const 结构_任务管理最小原语签名 s_表[] = {
            { 枚举_任务管理最小原语ID::确保单承载存在, "确保单承载存在", "存在类型特征, 归属指针特征", "存在指针特征", "自我存在/任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在/场景存在" },
            { 枚举_任务管理最小原语ID::读取单特征当前值, "读取单特征当前值", "主体指针特征, 目标特征类型", "同类型带值特征", "任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在/场景存在" },
            { 枚举_任务管理最小原语ID::写入单特征当前值, "写入单特征当前值", "主体指针特征, 目标特征类型, 候选值特征", "同类型带值特征", "任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在/场景存在" },
            { 枚举_任务管理最小原语ID::写入单挂点指针, "写入单挂点指针", "挂点主体指针特征, 挂点类型特征, 目标节点指针特征", "同类型指针特征", "任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在" },
            { 枚举_任务管理最小原语ID::记录动作开始, "记录动作开始", "动作名特征, 动作主体指针特征, 输入场景指针特征", "动作事实动态(开始相位)", "任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在/场景存在" },
            { 枚举_任务管理最小原语ID::记录动作结束, "记录动作结束", "动作名特征, 动作主体指针特征, 输出场景指针特征, 错误码特征", "动作事实动态(完成或失败相位)", "任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在/场景存在" },
        };
        return s_表;
    }

    std::size_t 私有_最小原语签名数量() noexcept
    {
        return 6;
    }

    const 结构_任务管理最小原语签名* 私有_查找最小原语签名(
        枚举_任务管理最小原语ID 原语ID) noexcept
    {
        const auto* 表 = 私有_最小原语签名表();
        for (std::size_t i = 0; i < 私有_最小原语签名数量(); ++i) {
            if (表[i].原语ID == 原语ID) {
                return &表[i];
            }
        }
        return nullptr;
    }

    std::string 私有_原语位图摘要(std::uint64_t 位图) noexcept
    {
        static const 枚举_任务管理最小原语ID s_顺序[] = {
            枚举_任务管理最小原语ID::确保单承载存在,
            枚举_任务管理最小原语ID::读取单特征当前值,
            枚举_任务管理最小原语ID::写入单特征当前值,
            枚举_任务管理最小原语ID::写入单挂点指针,
            枚举_任务管理最小原语ID::记录动作开始,
            枚举_任务管理最小原语ID::记录动作结束,
        };

        std::vector<std::string> 片段{};
        for (const auto 原语ID : s_顺序) {
            if ((位图 & 私有_原语位(原语ID)) == 0) {
                continue;
            }
            if (const auto* 签名 = 私有_查找最小原语签名(原语ID)) {
                片段.emplace_back(签名->原语名);
            }
        }
        return 私有_拼接片段(片段);
    }

    std::string 私有_派生能力位图摘要(std::uint64_t 位图) noexcept
    {
        std::vector<std::string> 片段{};
        if ((位图 & 私有_派生能力位_状态差分取证) != 0) {
            片段.emplace_back("状态差分取证");
        }
        if ((位图 & 私有_派生能力位_实例因果派生) != 0) {
            片段.emplace_back("实例因果派生");
        }
        if ((位图 & 私有_派生能力位_轻量因果链组织) != 0) {
            片段.emplace_back("轻量因果链组织");
        }
        return 私有_拼接片段(片段);
    }

    std::string 私有_方法需求位文本(枚举_任务管理方法需求位 需求位) noexcept
    {
        switch (需求位) {
        case 枚举_任务管理方法需求位::补结构: return "补结构";
        case 枚举_任务管理方法需求位::选择推进: return "选择推进";
        case 枚举_任务管理方法需求位::分解补条件: return "分解补条件";
        case 枚举_任务管理方法需求位::求证判定: return "求证判定";
        case 枚举_任务管理方法需求位::门控转域: return "门控转域";
        case 枚举_任务管理方法需求位::失败策略: return "失败策略";
        case 枚举_任务管理方法需求位::收束复盘: return "收束复盘";
        default: return "未定义";
        }
    }

    std::string 私有_本能能力缺口文本(枚举_任务管理本能能力缺口类型 缺口) noexcept
    {
        switch (缺口) {
        case 枚举_任务管理本能能力缺口类型::无缺口: return "无缺口";
        case 枚举_任务管理本能能力缺口类型::缺观察原语: return "缺观察原语";
        case 枚举_任务管理本能能力缺口类型::缺写回原语: return "缺写回原语";
        case 枚举_任务管理本能能力缺口类型::缺挂点原语: return "缺挂点原语";
        case 枚举_任务管理本能能力缺口类型::缺动作边界: return "缺动作边界";
        case 枚举_任务管理本能能力缺口类型::缺状态差分取证: return "缺状态差分取证";
        case 枚举_任务管理本能能力缺口类型::缺实例因果取证: return "缺实例因果取证";
        case 枚举_任务管理本能能力缺口类型::缺轻量因果链组织: return "缺轻量因果链组织";
        case 枚举_任务管理本能能力缺口类型::需学习补齐: return "需学习补齐";
        default: return "未定义";
        }
    }

    const char* 私有_触发来源文本(枚举_任务管理触发来源 来源) noexcept
    {
        switch (来源) {
        case 枚举_任务管理触发来源::自然运行态: return "自然运行态";
        case 枚举_任务管理触发来源::半白盒: return "半白盒";
        case 枚举_任务管理触发来源::白盒: return "白盒";
        default: return "未定义";
        }
    }

    const char* 私有_触发事件类型文本(枚举_任务管理触发事件类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_任务管理触发事件类型::普通推进: return "普通推进";
        case 枚举_任务管理触发事件类型::执行回流: return "执行回流";
        case 枚举_任务管理触发事件类型::学习回流: return "学习回流";
        case 枚举_任务管理触发事件类型::外部反馈: return "外部反馈";
        case 枚举_任务管理触发事件类型::恢复重建: return "恢复重建";
        default: return "未定义";
        }
    }

    const char* 私有_学习承接类型文本(枚举_任务管理学习承接类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_任务管理学习承接类型::定向学习: return "定向学习";
        case 枚举_任务管理学习承接类型::广泛学习: return "广泛学习";
        default: return "未定义";
        }
    }

    结构_任务管理半白盒控制 私有_读取半白盒治理控制() noexcept
    {
        std::scoped_lock 锁(私有_半白盒治理控制锁);
        return 私有_半白盒治理控制;
    }

    bool 私有_尝试消费治理恢复重建控制(结构_治理恢复快照* 输出) noexcept
    {
        std::scoped_lock 锁(私有_治理恢复重建控制锁);
        if (!私有_治理恢复重建控制.启用) {
            return false;
        }
        if (输出) {
            *输出 = 私有_治理恢复重建控制.快照;
        }
        私有_治理恢复重建控制 = {};
        return true;
    }

    bool 私有_尝试消费治理显式事件控制(结构_任务管理显式事件控制* 输出) noexcept
    {
        std::scoped_lock 锁(私有_治理显式事件控制锁);
        if (!私有_治理显式事件控制.启用
            || 私有_治理显式事件控制.事件类型 == 枚举_任务管理触发事件类型::未定义) {
            return false;
        }
        if (输出) {
            *输出 = 私有_治理显式事件控制;
        }
        私有_治理显式事件控制 = {};
        return true;
    }

    bool 私有_本能缺口属于原语或治理补齐(
        枚举_任务管理本能能力缺口类型 缺口类型) noexcept
    {
        switch (缺口类型) {
        case 枚举_任务管理本能能力缺口类型::缺观察原语:
        case 枚举_任务管理本能能力缺口类型::缺写回原语:
        case 枚举_任务管理本能能力缺口类型::缺挂点原语:
        case 枚举_任务管理本能能力缺口类型::缺动作边界:
        case 枚举_任务管理本能能力缺口类型::缺状态差分取证:
        case 枚举_任务管理本能能力缺口类型::缺实例因果取证:
        case 枚举_任务管理本能能力缺口类型::缺轻量因果链组织:
            return true;
        default:
            return false;
        }
    }

    const char* 私有_学习通道文本(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_本能缺口属于原语或治理补齐(私有_P0_当前本能缺口(结果))
            ? "本能补齐"
            : "方法补齐";
    }

    template <typename T>
    T 私有_P0_选用非默认值(T 第一, T 第二, T 第三, T 默认值) noexcept
    {
        if (第一 != 默认值) {
            return 第一;
        }
        if (第二 != 默认值) {
            return 第二;
        }
        return 第三;
    }

    template <typename T>
    void 私有_P0_覆盖非默认值(T& 目标, T 来源, T 默认值) noexcept
    {
        if (来源 != 默认值) {
            目标 = 来源;
        }
    }

    template <typename T>
    T* 私有_P0_选用非空指针(T* 第一, T* 第二 = nullptr, T* 第三 = nullptr) noexcept
    {
        if (第一) {
            return 第一;
        }
        if (第二) {
            return 第二;
        }
        return 第三;
    }

    template <typename T>
    void 私有_P0_覆盖非空指针(T*& 目标, T* 来源) noexcept
    {
        if (来源) {
            目标 = 来源;
        }
    }

    const std::string& 私有_P0_选用非空文本(
        const std::string& 第一,
        const std::string& 第二,
        const std::string& 第三) noexcept
    {
        if (!第一.empty()) {
            return 第一;
        }
        if (!第二.empty()) {
            return 第二;
        }
        return 第三;
    }

    void 私有_P0_覆盖非空文本(
        std::string& 目标,
        const std::string& 来源) noexcept
    {
        if (!来源.empty()) {
            目标 = 来源;
        }
    }

    枚举_任务管理功能域 私有_P0_当前功能域(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前功能域,
            结果.当前单步决策.当前功能域,
            私有_P0_选用非默认值(
                结果.当前功能域,
                结果.上下文.最近功能域,
                枚举_任务管理功能域::未定义,
                枚举_任务管理功能域::未定义),
            枚举_任务管理功能域::未定义);
    }

    枚举_任务管理缺口类型 私有_P0_当前缺口类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前缺口类型,
            结果.当前单步决策.当前缺口类型,
            私有_P0_选用非默认值(
                结果.当前缺口类型,
                结果.上下文.当前缺口类型,
                枚举_任务管理缺口类型::未定义,
                枚举_任务管理缺口类型::未定义),
            枚举_任务管理缺口类型::未定义);
    }

    枚举_任务管理下一步去向 私有_P0_当前下一步去向(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前下一步去向,
            结果.当前单步决策.当前下一步去向,
            私有_P0_选用非默认值(
                结果.当前下一步去向,
                结果.上下文.当前下一步去向,
                枚举_任务管理下一步去向::未定义,
                枚举_任务管理下一步去向::未定义),
            枚举_任务管理下一步去向::未定义);
    }

    枚举_任务管理治理状态迁移 私有_P0_当前治理状态迁移(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前治理状态迁移,
            结果.当前单步决策.当前治理状态迁移,
            结果.当前治理状态迁移,
            枚举_任务管理治理状态迁移::未定义);
    }

    枚举_任务管理方法来源 私有_P0_当前方法来源(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前单步决策.当前方法来源,
            结果.当前方法来源,
            枚举_任务管理方法来源::未定义,
            枚举_任务管理方法来源::未定义);
    }

    枚举_任务管理方法需求位 私有_P0_当前方法需求位(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前方法需求位,
            结果.当前单步决策.当前方法需求位,
            私有_P0_选用非默认值(
                结果.当前需求位特征.当前需求位,
                结果.当前主方法需求位,
                枚举_任务管理方法需求位::未定义,
                枚举_任务管理方法需求位::未定义),
            枚举_任务管理方法需求位::未定义);
    }

    枚举_任务管理本能能力缺口类型 私有_P0_当前本能缺口(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前本能能力缺口类型,
            结果.当前单步决策.当前本能能力缺口类型,
            私有_P0_选用非默认值(
                结果.当前本能能力缺口.当前缺口类型,
                结果.当前本能能力缺口类型,
                枚举_任务管理本能能力缺口类型::未定义,
                枚举_任务管理本能能力缺口类型::未定义),
            枚举_任务管理本能能力缺口类型::未定义);
    }

    枚举_任务管理步骤位类型 私有_P0_当前步骤位类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前单步决策.当前步骤位类型,
            结果.当前步骤位类型,
            枚举_任务管理步骤位类型::未定义,
            枚举_任务管理步骤位类型::未定义);
    }

    枚举_任务管理步骤语义类型 私有_P0_当前步骤语义类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前单步决策.当前步骤语义类型,
            结果.当前步骤语义类型,
            枚举_任务管理步骤语义类型::未定义,
            枚举_任务管理步骤语义类型::未定义);
    }

    枚举_任务管理后果治理 私有_P0_当前后果治理(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前单步决策.当前后果治理,
            结果.当前后果治理,
            枚举_任务管理后果治理::未定义,
            枚举_任务管理后果治理::未定义);
    }

    任务节点* 私有_P0_当前宿主任务(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空指针(
            结果.上下文.当前宿主任务,
            static_cast<任务节点*>(结果.当前一步结果.宿主任务),
            static_cast<任务节点*>(结果.当前请求.宿主任务));
    }

    方法节点* 私有_P0_当前方法(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空指针(
            结果.上下文.当前方法,
            static_cast<方法节点*>(结果.当前一步结果.当前方法节点));
    }

    任务节点* 私有_P0_当前步骤节点(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空指针(
            结果.当前步骤节点,
            static_cast<任务节点*>(结果.当前一步结果.当前步骤节点),
            static_cast<任务节点*>(结果.当前写回结果.当前步骤节点));
    }

    任务节点* 私有_P0_最新结果节点(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空指针(
            结果.最新结果节点,
            static_cast<任务节点*>(结果.当前一步结果.最新结果节点),
            static_cast<任务节点*>(结果.当前写回结果.最新结果节点));
    }

    任务节点* 私有_P0_当前子任务节点(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空指针(
            结果.当前子任务节点,
            static_cast<任务节点*>(结果.当前写回结果.当前学习子任务),
            私有_P0_选用非空指针(
                static_cast<任务节点*>(结果.当前写回结果.当前补条件子任务),
                static_cast<任务节点*>(结果.当前写回结果.当前业务子任务),
                static_cast<任务节点*>(nullptr)));
    }

    枚举_任务管理总控结果 私有_P0_最近总控结果(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.最近总控结果,
            结果.最近总控结果,
            枚举_任务管理总控结果::未定义,
            枚举_任务管理总控结果::未定义);
    }

    枚举_任务管理反馈类型 私有_P0_最近反馈类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.最近反馈类型,
            结果.最近反馈类型,
            枚举_任务管理反馈类型::未定义,
            枚举_任务管理反馈类型::未定义);
    }

    const std::string& 私有_P0_最近反馈摘要(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空文本(
            结果.当前一步结果.最近反馈摘要,
            结果.当前单步决策.决策摘要,
            结果.最近反馈摘要);
    }

    const std::string& 私有_P0_双面摘要(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空文本(
            结果.目标结果差额摘要,
            结果.当前写回结果.双面摘要,
            结果.目标结果差额摘要);
    }

    枚举_任务管理功能域 私有_P0_阶段当前功能域(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前功能域,
            结果.当前单步决策.当前功能域,
            结果.当前功能域,
            枚举_任务管理功能域::未定义);
    }

    枚举_任务管理缺口类型 私有_P0_阶段当前缺口类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前缺口类型,
            结果.当前单步决策.当前缺口类型,
            结果.当前缺口类型,
            枚举_任务管理缺口类型::未定义);
    }

    枚举_任务管理下一步去向 私有_P0_阶段当前下一步去向(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前下一步去向,
            结果.当前单步决策.当前下一步去向,
            结果.当前下一步去向,
            枚举_任务管理下一步去向::未定义);
    }

    枚举_任务管理总控结果 私有_P0_阶段最近总控结果(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.最近总控结果,
            结果.最近总控结果,
            枚举_任务管理总控结果::未定义,
            枚举_任务管理总控结果::未定义);
    }

    枚举_任务管理反馈类型 私有_P0_阶段最近反馈类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.最近反馈类型,
            结果.最近反馈类型,
            枚举_任务管理反馈类型::未定义,
            枚举_任务管理反馈类型::未定义);
    }

    const std::string& 私有_P0_阶段最近反馈摘要(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空文本(
            结果.当前一步结果.最近反馈摘要,
            结果.最近反馈摘要,
            结果.当前单步决策.决策摘要);
    }

    void 私有_P0_覆盖决策字段(
        结构_任务管理单步决策& 目标,
        const 结构_任务管理单步决策& 来源) noexcept
    {
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前治理状态迁移, 来源.当前治理状态迁移, 枚举_任务管理治理状态迁移::未定义);
        私有_P0_覆盖非默认值(目标.当前方法来源, 来源.当前方法来源, 枚举_任务管理方法来源::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤位类型, 来源.当前步骤位类型, 枚举_任务管理步骤位类型::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤语义类型, 来源.当前步骤语义类型, 枚举_任务管理步骤语义类型::未定义);
        私有_P0_覆盖非默认值(目标.当前后果治理, 来源.当前后果治理, 枚举_任务管理后果治理::未定义);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.影子验证状态, 来源.影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        私有_P0_覆盖非空文本(目标.决策摘要, 来源.决策摘要);
    }

    void 私有_P0_覆盖兼容决策字段(
        结构_任务管理单步决策& 目标,
        const 结构_任务管理结果& 来源) noexcept
    {
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前治理状态迁移, 来源.当前治理状态迁移, 枚举_任务管理治理状态迁移::未定义);
        私有_P0_覆盖非默认值(目标.当前方法来源, 来源.当前方法来源, 枚举_任务管理方法来源::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前主方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤位类型, 来源.当前步骤位类型, 枚举_任务管理步骤位类型::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤语义类型, 来源.当前步骤语义类型, 枚举_任务管理步骤语义类型::未定义);
        私有_P0_覆盖非默认值(目标.当前后果治理, 来源.当前后果治理, 枚举_任务管理后果治理::未定义);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.影子验证状态, 来源.当前影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
    }

    void 私有_P0_覆盖决策到兼容字段(
        结构_任务管理结果& 目标,
        const 结构_任务管理单步决策& 来源) noexcept
    {
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前治理状态迁移, 来源.当前治理状态迁移, 枚举_任务管理治理状态迁移::未定义);
        私有_P0_覆盖非默认值(目标.当前方法来源, 来源.当前方法来源, 枚举_任务管理方法来源::未定义);
        私有_P0_覆盖非默认值(目标.当前主方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤位类型, 来源.当前步骤位类型, 枚举_任务管理步骤位类型::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤语义类型, 来源.当前步骤语义类型, 枚举_任务管理步骤语义类型::未定义);
        私有_P0_覆盖非默认值(目标.当前后果治理, 来源.当前后果治理, 枚举_任务管理后果治理::未定义);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.当前影子验证状态, 来源.影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
    }

    void 私有_P0_覆盖写回字段(
        结构_任务管理写回结果& 目标,
        const 结构_任务管理写回结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.当前步骤节点, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.最新结果节点, static_cast<任务节点*>(来源.最新结果节点));
        私有_P0_覆盖非空指针(目标.当前业务子任务, static_cast<任务节点*>(来源.当前业务子任务));
        私有_P0_覆盖非空指针(目标.当前补条件子任务, static_cast<任务节点*>(来源.当前补条件子任务));
        私有_P0_覆盖非空指针(目标.当前学习子任务, static_cast<任务节点*>(来源.当前学习子任务));
        私有_P0_覆盖非空指针(目标.当前学习方法, static_cast<方法节点*>(来源.当前学习方法));
        私有_P0_覆盖非空指针(目标.宿主目标状态, 来源.宿主目标状态);
        私有_P0_覆盖非空指针(目标.宿主结果状态, 来源.宿主结果状态);
        私有_P0_覆盖非空文本(目标.双面摘要, 来源.双面摘要);
        私有_P0_覆盖非空文本(目标.结构动作摘要, 来源.结构动作摘要);
        私有_P0_覆盖非空文本(目标.主体回并摘要, 来源.主体回并摘要);
    }

    void 私有_P0_覆盖兼容写回字段(
        结构_任务管理写回结果& 目标,
        const 结构_任务管理结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.当前步骤节点, 来源.当前步骤节点);
        私有_P0_覆盖非空指针(目标.最新结果节点, 来源.最新结果节点);
        if (来源.已创建业务子任务 || 来源.已复用业务子任务) {
            私有_P0_覆盖非空指针(目标.当前业务子任务, 来源.当前子任务节点);
        }
        if (来源.已创建补条件子任务 || 来源.已复用补条件子任务) {
            私有_P0_覆盖非空指针(目标.当前补条件子任务, 来源.当前子任务节点);
        }
        if (来源.已创建学习子任务 || 来源.已复用学习子任务) {
            私有_P0_覆盖非空指针(目标.当前学习子任务, 来源.当前子任务节点);
        }
        私有_P0_覆盖非空指针(目标.当前学习方法, 来源.当前学习方法节点);
        私有_P0_覆盖非空指针(目标.宿主目标状态, 来源.宿主目标状态);
        私有_P0_覆盖非空指针(目标.宿主结果状态, 来源.宿主结果状态);
        私有_P0_覆盖非空文本(目标.双面摘要, 来源.目标结果差额摘要);
        私有_P0_覆盖非空文本(目标.结构动作摘要, 来源.最近反馈摘要);
        私有_P0_覆盖非空文本(目标.主体回并摘要, 来源.当前写回结果.主体回并摘要);
    }

    任务节点* 私有_P0_选用写回子任务节点(
        const 结构_任务管理写回结果& 写回) noexcept
    {
        return 私有_P0_选用非空指针(
            static_cast<任务节点*>(写回.当前学习子任务),
            static_cast<任务节点*>(写回.当前补条件子任务),
            static_cast<任务节点*>(写回.当前业务子任务));
    }

    void 私有_P0_覆盖写回到兼容字段(
        结构_任务管理结果& 目标,
        const 结构_任务管理写回结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.当前步骤节点, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.最新结果节点, static_cast<任务节点*>(来源.最新结果节点));
        私有_P0_覆盖非空指针(目标.宿主目标状态, 来源.宿主目标状态);
        私有_P0_覆盖非空指针(目标.宿主结果状态, 来源.宿主结果状态);
        私有_P0_覆盖非空指针(目标.当前学习方法节点, static_cast<方法节点*>(来源.当前学习方法));
        if (auto* 当前子任务 = 私有_P0_选用写回子任务节点(来源)) {
            私有_P0_覆盖非空指针(目标.当前子任务节点, 当前子任务);
        }
        私有_P0_覆盖非空文本(目标.目标结果差额摘要, 来源.双面摘要);
    }

    void 私有_P0_覆盖一步结果字段(
        结构_任务管理一步结果& 目标,
        const 结构_任务管理一步结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.宿主任务, static_cast<任务节点*>(来源.宿主任务));
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前治理状态迁移, 来源.当前治理状态迁移, 枚举_任务管理治理状态迁移::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非空指针(目标.当前步骤节点, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.当前方法节点, static_cast<方法节点*>(来源.当前方法节点));
        私有_P0_覆盖非空指针(目标.最新结果节点, static_cast<任务节点*>(来源.最新结果节点));
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        私有_P0_覆盖非空文本(目标.当前输入条件主键, 来源.当前输入条件主键);
        私有_P0_覆盖非空文本(目标.当前治理态型, 来源.当前治理态型);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.影子验证状态, 来源.影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        if (来源.已触发学习承接) {
            目标.已触发学习承接 = true;
        }
        私有_P0_覆盖非默认值(
            目标.当前学习承接类型,
            来源.当前学习承接类型,
            枚举_任务管理学习承接类型::未定义);
        if (来源.当前学习为本能补齐) {
            目标.当前学习为本能补齐 = true;
        }
        私有_P0_覆盖非空文本(目标.学习承接摘要, 来源.学习承接摘要);
        私有_P0_覆盖非空文本(目标.主体回并摘要, 来源.主体回并摘要);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    void 私有_P0_覆盖兼容一步结果字段(
        结构_任务管理一步结果& 目标,
        const 结构_任务管理结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.宿主任务, 来源.上下文.当前宿主任务);
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前治理状态迁移, 来源.当前治理状态迁移, 枚举_任务管理治理状态迁移::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前主方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非空指针(目标.当前步骤节点, 来源.当前步骤节点);
        if (auto* 当前方法 = 私有_P0_当前方法(来源)) {
            私有_P0_覆盖非空指针(目标.当前方法节点, 当前方法);
        }
        私有_P0_覆盖非空指针(目标.最新结果节点, 来源.最新结果节点);
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        私有_P0_覆盖非空文本(目标.当前输入条件主键, 来源.上下文.当前输入条件主键);
        私有_P0_覆盖非空文本(目标.当前治理态型, 来源.上下文.当前治理态型);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.影子验证状态, 来源.当前影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        if (来源.当前学习承接决策.是否触发学习) {
            目标.已触发学习承接 = true;
        }
        私有_P0_覆盖非默认值(
            目标.当前学习承接类型,
            来源.当前学习承接决策.学习类型,
            枚举_任务管理学习承接类型::未定义);
        if (来源.当前学习承接决策.补原语或治理本能) {
            目标.当前学习为本能补齐 = true;
        }
        私有_P0_覆盖非空文本(目标.学习承接摘要, 来源.当前学习承接决策.决策摘要);
        私有_P0_覆盖非空文本(目标.主体回并摘要, 来源.当前写回结果.主体回并摘要);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    void 私有_P0_覆盖一步结果到兼容字段(
        结构_任务管理结果& 目标,
        const 结构_任务管理一步结果& 来源) noexcept
    {
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前主方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非空指针(目标.当前步骤节点, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.最新结果节点, static_cast<任务节点*>(来源.最新结果节点));
        私有_P0_覆盖非默认值(目标.最近功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.当前影子验证状态, 来源.影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        if (来源.已触发学习承接) {
            目标.当前学习承接决策.是否触发学习 = true;
        }
        私有_P0_覆盖非默认值(
            目标.当前学习承接决策.学习类型,
            来源.当前学习承接类型,
            枚举_任务管理学习承接类型::未定义);
        if (来源.当前学习为本能补齐) {
            目标.当前学习承接决策.补原语或治理本能 = true;
        }
        私有_P0_覆盖非空文本(目标.当前学习承接决策.决策摘要, 来源.学习承接摘要);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    void 私有_P0_覆盖上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理上下文& 来源) noexcept
    {
        if (来源.当前时间 != 0) {
            目标.当前时间 = 来源.当前时间;
        }
        私有_P0_覆盖非空指针(目标.当前主需求, 来源.当前主需求);
        私有_P0_覆盖非空指针(目标.当前管理任务, 来源.当前管理任务);
        私有_P0_覆盖非空指针(目标.当前宿主任务, 来源.当前宿主任务);
        私有_P0_覆盖非空指针(目标.当前方法, 来源.当前方法);
        私有_P0_覆盖非空指针(目标.当前步骤, 来源.当前步骤);
        私有_P0_覆盖非空指针(目标.最近结果, 来源.最近结果);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.最近根层重判结果, 来源.最近根层重判结果, 枚举_任务管理根层重判结果::未定义);
        私有_P0_覆盖非默认值(目标.最近执行前门控结果, 来源.最近执行前门控结果, 枚举_任务管理执行前门控结果::未定义);
        私有_P0_覆盖非空文本(目标.最近上层反馈摘要, 来源.最近上层反馈摘要);
        私有_P0_覆盖非默认值(目标.最近功能域, 来源.最近功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        私有_P0_覆盖非默认值(目标.最近宿主目标结果比较, 来源.最近宿主目标结果比较, 枚举_任务管理双面比较结果::未定义);
        私有_P0_覆盖非默认值(目标.最近后果治理, 来源.最近后果治理, 枚举_任务管理后果治理::未定义);
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非空文本(目标.当前输入条件主键, 来源.当前输入条件主键);
        私有_P0_覆盖非空文本(目标.当前治理态型, 来源.当前治理态型);
        私有_P0_覆盖非空指针(目标.当前方法位专项动态, 来源.当前方法位专项动态);
        私有_P0_覆盖非空文本(目标.当前方法位专项动态标题, 来源.当前方法位专项动态标题);
        私有_P0_覆盖非空指针(目标.当前最小原语动态, 来源.当前最小原语动态);
        私有_P0_覆盖非空文本(目标.当前最小原语动态标题, 来源.当前最小原语动态标题);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    void 私有_P0_覆盖请求字段(
        结构_任务管理请求& 目标,
        const 结构_任务管理请求& 来源) noexcept
    {
        if (来源.请求ID != 0) {
            目标.请求ID = 来源.请求ID;
        }
        if (来源.提交时间 != 0) {
            目标.提交时间 = 来源.提交时间;
        }
        if (来源.提交序号 != 0) {
            目标.提交序号 = 来源.提交序号;
        }
        私有_P0_覆盖非空指针(目标.宿主任务, 来源.宿主任务);
        私有_P0_覆盖上下文字段(目标.上下文, 来源.上下文);
        私有_P0_覆盖非默认值(目标.触发事件类型, 来源.触发事件类型, 枚举_任务管理触发事件类型::未定义);
        私有_P0_覆盖非默认值(目标.触发来源, 来源.触发来源, 枚举_任务管理触发来源::未定义);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.影子验证状态, 来源.影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        私有_P0_覆盖非空文本(目标.触发事件摘要, 来源.触发事件摘要);
        私有_P0_覆盖非空文本(目标.当前恢复点类型, 来源.当前恢复点类型);
        if (来源.存在待消费学习回流) {
            目标.存在待消费学习回流 = true;
        }
        if (来源.存在待消费外部反馈) {
            目标.存在待消费外部反馈 = true;
        }
        私有_P0_覆盖非空文本(目标.恢复重建摘要, 来源.恢复重建摘要);
        私有_P0_覆盖非空文本(目标.补充材料摘要, 来源.补充材料摘要);
    }

    void 私有_P0_覆盖请求到上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理请求& 来源) noexcept
    {
        私有_P0_覆盖上下文字段(目标, 来源.上下文);
        if (来源.提交时间 != 0) {
            目标.当前时间 = 来源.提交时间;
        }
        私有_P0_覆盖非空指针(目标.当前宿主任务, static_cast<任务节点*>(来源.宿主任务));
    }

    void 私有_P0_覆盖决策到上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理单步决策& 来源) noexcept
    {
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.最近功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.最近后果治理, 来源.当前后果治理, 枚举_任务管理后果治理::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.决策摘要);
    }

    void 私有_P0_覆盖写回到上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理写回结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.当前步骤, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.最近结果, static_cast<任务节点*>(来源.最新结果节点));
    }

    void 私有_P0_覆盖一步结果到上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理一步结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.当前宿主任务, static_cast<任务节点*>(来源.宿主任务));
        私有_P0_覆盖非空指针(目标.当前方法, static_cast<方法节点*>(来源.当前方法节点));
        私有_P0_覆盖非空指针(目标.当前步骤, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.最近结果, static_cast<任务节点*>(来源.最新结果节点));
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.最近功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非空文本(目标.当前输入条件主键, 来源.当前输入条件主键);
        私有_P0_覆盖非空文本(目标.当前治理态型, 来源.当前治理态型);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    void 私有_P0_覆盖兼容到上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理结果& 来源) noexcept
    {
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.最近功能域, 来源.最近功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        私有_P0_覆盖非默认值(目标.最近宿主目标结果比较, 来源.最近宿主目标结果比较, 枚举_任务管理双面比较结果::未定义);
        私有_P0_覆盖非默认值(目标.最近后果治理, 来源.当前后果治理, 枚举_任务管理后果治理::未定义);
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前主方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非空文本(目标.当前输入条件主键, 来源.当前需求位特征.输入条件主键);
        私有_P0_覆盖非空文本(目标.当前治理态型, 来源.当前一步结果.当前治理态型);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    std::string 私有_治理态型文本(const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << "域=" << 私有_功能域文本(私有_P0_当前功能域(结果))
            << "|步骤位=" << 私有_步骤位类型文本(私有_P0_当前步骤位类型(结果))
            << "|需求位=" << 私有_方法需求位文本(私有_P0_当前方法需求位(结果));
        return 输出.str();
    }

    std::string 私有_任务管理动态聚合方式文本(枚举_动态聚合方式 聚合方式) noexcept
    {
        switch (聚合方式) {
        case 枚举_动态聚合方式::原子相邻: return "原子相邻";
        case 枚举_动态聚合方式::连续同向段: return "连续同向段";
        case 枚举_动态聚合方式::同类事件段: return "同类事件段";
        case 枚举_动态聚合方式::窗口聚合: return "窗口聚合";
        case 枚举_动态聚合方式::路径压缩: return "路径压缩";
        default: return "未定义";
        }
    }

    bool 私有_动态来源链包含目标(
        动态节点类* 候选动态,
        动态节点类* 目标动态,
        std::uint32_t 深度 = 0) noexcept
    {
        if (!候选动态 || !目标动态 || 深度 > 8) {
            return false;
        }

        const auto* 候选主信息 = 世界树.动态().取动态主信息(候选动态);
        if (!候选主信息 || 候选主信息->来源低层动态.empty()) {
            return false;
        }

        const auto 目标主键 = 目标动态->获取主键();
        for (const auto& 引用 : 候选主信息->来源低层动态) {
            auto* 来源动态 = 引用.指针;
            const auto 来源主键 =
                !引用.主键.empty()
                ? 引用.主键
                : (来源动态 ? 来源动态->获取主键() : std::string{});
            if (来源主键 == 目标主键) {
                return true;
            }
            if (来源动态 && 私有_动态来源链包含目标(来源动态, 目标动态, 深度 + 1)) {
                return true;
            }
        }
        return false;
    }

    动态节点类* 私有_选择任务管理桥接动态(动态节点类* 原始动态) noexcept
    {
        if (!原始动态) {
            return nullptr;
        }

        auto* 最佳动态 = 原始动态;
        auto* 父节点 = static_cast<基础信息节点类*>(原始动态->父);
        auto* 场景 = static_cast<场景节点类*>(父节点);
        if (!场景) {
            return 最佳动态;
        }

        for (auto* 候选动态 : 世界树.动态().获取场景动态(场景)) {
            if (!候选动态 || 候选动态 == 原始动态) {
                continue;
            }
            const auto* 候选主信息 = 世界树.动态().取动态主信息(候选动态);
            const auto* 最佳主信息 = 世界树.动态().取动态主信息(最佳动态);
            if (!候选主信息 || !最佳主信息 || 候选主信息->来源低层动态.empty()) {
                continue;
            }
            if (!私有_动态来源链包含目标(候选动态, 原始动态)) {
                continue;
            }

            const bool 层级更高 = 候选主信息->动态层级 > 最佳主信息->动态层级;
            const bool 同层但覆盖更宽 =
                候选主信息->动态层级 == 最佳主信息->动态层级
                && 候选主信息->来源低层动态.size() > 最佳主信息->来源低层动态.size();
            if (层级更高 || 同层但覆盖更宽) {
                最佳动态 = 候选动态;
            }
        }

        return 最佳动态;
    }

    std::string 私有_任务管理桥接动态标题(
        动态节点类* 动态,
        const std::string& 回退标题) noexcept
    {
        auto* 桥接动态 = 私有_选择任务管理桥接动态(动态);
        if (!桥接动态) {
            return 回退标题;
        }

        const auto* 动态主信息 = 世界树.动态().取动态主信息(桥接动态);
        if (!动态主信息) {
            return 回退标题;
        }

        std::ostringstream 输出;
        if (!回退标题.empty()) {
            输出 << 回退标题;
        }
        else if (动态主信息->来源动作名) {
            输出 << 动态主信息->来源动作名->获取主键();
        }
        else {
            输出 << "动态";
        }

        if (动态主信息->动态层级 != 0
            || 动态主信息->聚合方式 != 枚举_动态聚合方式::原子相邻
            || !动态主信息->来源低层动态.empty()) {
            输出 << " [层级=" << 动态主信息->动态层级
                << " 聚合=" << 私有_任务管理动态聚合方式文本(动态主信息->聚合方式);
            if (!动态主信息->来源低层动态.empty()) {
                输出 << " 段数=" << 动态主信息->来源低层动态.size();
            }
            输出 << "]";
        }
        return 输出.str();
    }

    void 私有_P0_刷新主体桥接字段(结构_任务管理结果& 结果) noexcept
    {
        const auto 当前方法需求位 = 私有_P0_当前方法需求位(结果);
        const auto 当前本能能力缺口类型 = 私有_P0_当前本能缺口(结果);
        const auto 当前输入条件主键 =
            !结果.当前分身.本轮输入条件主键.empty()
            ? 结果.当前分身.本轮输入条件主键
            : 结果.当前需求位特征.输入条件主键;
        const auto 当前治理态型 = 私有_治理态型文本(结果);
        const auto 已选方法位专项动态标题 = 私有_P0_选用非空文本(
            结果.当前需求位特征.当前方法位专项动态标题,
            结果.当前分身.当前方法位专项动态标题,
            私有_P0_选用非空文本(
                结果.当前主体.最近方法位专项动态标题,
                结果.上下文.当前方法位专项动态标题,
                std::string{}));
        const auto 已选最小原语动态标题 = 私有_P0_选用非空文本(
            结果.当前本能能力缺口.当前最小原语动态标题,
            结果.当前分身.当前最小原语动态标题,
            私有_P0_选用非空文本(
                结果.当前主体.最近最小原语动态标题,
                结果.上下文.当前最小原语动态标题,
                std::string{}));
        auto* 当前方法位专项动态 =
            结果.当前需求位特征.当前方法位专项动态
            ? 结果.当前需求位特征.当前方法位专项动态
            : (
                结果.当前分身.当前方法位专项动态
                ? 结果.当前分身.当前方法位专项动态
                : (
                    结果.当前主体.最近方法位专项动态
                    ? 结果.当前主体.最近方法位专项动态
                    : 结果.上下文.当前方法位专项动态
                )
            );
        auto* 当前最小原语动态 =
            结果.当前本能能力缺口.当前最小原语动态
            ? 结果.当前本能能力缺口.当前最小原语动态
            : (
                结果.当前分身.当前最小原语动态
                ? 结果.当前分身.当前最小原语动态
                : (
                    结果.当前主体.最近最小原语动态
                    ? 结果.当前主体.最近最小原语动态
                    : 结果.上下文.当前最小原语动态
                )
            );
        auto* 当前桥接方法位专项动态 = 私有_选择任务管理桥接动态(当前方法位专项动态);
        if (!当前桥接方法位专项动态) {
            当前桥接方法位专项动态 = 当前方法位专项动态;
        }
        auto* 当前桥接最小原语动态 = 私有_选择任务管理桥接动态(当前最小原语动态);
        if (!当前桥接最小原语动态) {
            当前桥接最小原语动态 = 当前最小原语动态;
        }
        const auto 当前方法位专项动态标题 = 私有_任务管理桥接动态标题(
            当前桥接方法位专项动态,
            已选方法位专项动态标题);
        const auto 当前最小原语动态标题 = 私有_任务管理桥接动态标题(
            当前桥接最小原语动态,
            已选最小原语动态标题);

        if (结果.当前主体.主体ID != 0) {
            结果.当前主体ID = 结果.当前主体.主体ID;
        }
        if (结果.当前分身.分身ID != 0) {
            结果.当前分身ID = 结果.当前分身.分身ID;
        }
        if (结果.当前触发来源 == 枚举_任务管理触发来源::未定义) {
            结果.当前触发来源 = 枚举_任务管理触发来源::自然运行态;
        }
        if (结果.来源最小原语位图 == 0 && 结果.当前本能能力缺口.所需原语位图 != 0) {
            结果.来源最小原语位图 = 结果.当前本能能力缺口.所需原语位图;
        }
        if (结果.来源主观察特征.empty()) {
            结果.来源主观察特征 = 结果.当前本能能力缺口.主观察特征键;
        }
        if (结果.当前影子验证状态.empty()) {
            结果.当前影子验证状态 = "未触发";
        }
        if (结果.已初始化分身继承面) {
            结果.当前分身.当前本能能力缺口类型 = 当前本能能力缺口类型;
            结果.当前分身.当前治理态型 = 当前治理态型;
            结果.当前分身.当前方法位专项动态 = 当前桥接方法位专项动态;
            结果.当前分身.当前方法位专项动态标题 = 当前方法位专项动态标题;
            结果.当前分身.当前最小原语动态 = 当前桥接最小原语动态;
            结果.当前分身.当前最小原语动态标题 = 当前最小原语动态标题;
        }
        if (结果.已确保任务管理主体虚拟存在) {
            私有_P0_覆盖非默认值(
                结果.当前主体.最近本能能力缺口类型,
                当前本能能力缺口类型,
                枚举_任务管理本能能力缺口类型::未定义);
            私有_P0_覆盖非空文本(结果.当前主体.最近治理态型, 当前治理态型);
            结果.当前主体.最近方法位专项动态 = 当前桥接方法位专项动态;
            结果.当前主体.最近方法位专项动态标题 = 当前方法位专项动态标题;
            结果.当前主体.最近最小原语动态 = 当前桥接最小原语动态;
            结果.当前主体.最近最小原语动态标题 = 当前最小原语动态标题;
        }

        结果.上下文.当前主体ID = 结果.当前主体ID;
        结果.上下文.当前分身ID = 结果.当前分身ID;
        结果.上下文.当前方法需求位 = 当前方法需求位;
        结果.上下文.当前本能能力缺口类型 = 当前本能能力缺口类型;
        结果.上下文.当前输入条件主键 = 当前输入条件主键;
        结果.上下文.当前治理态型 = 当前治理态型;
        结果.上下文.当前方法位专项动态 = 当前桥接方法位专项动态;
        结果.上下文.当前方法位专项动态标题 = 当前方法位专项动态标题;
        结果.上下文.当前最小原语动态 = 当前桥接最小原语动态;
        结果.上下文.当前最小原语动态标题 = 当前最小原语动态标题;

        私有_P0_覆盖非空指针(结果.当前请求.宿主任务, 结果.上下文.当前宿主任务);
        私有_P0_覆盖上下文字段(结果.当前请求.上下文, 结果.上下文);
        结果.当前请求.触发来源 = 结果.当前触发来源;
        结果.当前请求.来源最小原语位图 = 结果.来源最小原语位图;
        结果.当前请求.来源主观察特征 = 结果.来源主观察特征;
        结果.当前请求.影子验证状态 = 结果.当前影子验证状态;
        结果.当前请求.允许正式资产提交 = 结果.允许正式资产提交;
        if (结果.当前请求.触发事件摘要.empty()
            && !结果.当前请求.恢复重建摘要.empty()) {
            结果.当前请求.触发事件摘要 = 结果.当前请求.恢复重建摘要;
        }
        {
            std::ostringstream 请求摘要;
            请求摘要 << "宿主=" << 私有_任务标题(结果.上下文.当前宿主任务)
                << " | 方法=" << 私有_方法标题(结果.上下文.当前方法)
                << " | 事件=" << 私有_触发事件类型文本(结果.当前请求.触发事件类型)
                << " | 需求位=" << 私有_方法需求位文本(当前方法需求位)
                << " | 本能缺口=" << 私有_本能能力缺口文本(当前本能能力缺口类型)
                << " | 触发来源=" << 私有_触发来源文本(结果.当前触发来源)
                << " | 本轮原语命中=" << (结果.来源最小原语位图 == 0 ? std::string("空") : 私有_原语位图摘要(结果.来源最小原语位图))
                << " | 正式已具备=" << (结果.当前本能能力缺口.原语账本摘要.empty() ? std::string("空") : 结果.当前本能能力缺口.原语账本摘要)
                << " | 主观察=" << (结果.来源主观察特征.empty() ? "空" : 结果.来源主观察特征)
                << " | 影子验证=" << (结果.当前影子验证状态.empty() ? "未触发" : 结果.当前影子验证状态)
                << " | 正式提交=" << (结果.允许正式资产提交 ? "允许" : "禁止")
                << " | 输入条件=" << (当前输入条件主键.empty() ? "空" : 当前输入条件主键)
                << " | 态型=" << 当前治理态型;
            if (!当前方法位专项动态标题.empty()) {
                请求摘要 << " | 方法位动态=" << 当前方法位专项动态标题;
            }
            if (!当前最小原语动态标题.empty()) {
                请求摘要 << " | 原语动态=" << 当前最小原语动态标题;
            }
            if (!结果.当前请求.触发事件摘要.empty()) {
                请求摘要 << " | mailbox=" << 结果.当前请求.触发事件摘要;
            }
            if (!结果.当前请求.当前恢复点类型.empty()) {
                请求摘要 << " | 恢复点=" << 结果.当前请求.当前恢复点类型;
            }
            if (结果.当前请求.存在待消费学习回流 || 结果.当前请求.存在待消费外部反馈) {
                请求摘要 << " | 待消费="
                    << "学习回流=" << (结果.当前请求.存在待消费学习回流 ? "是" : "否")
                    << "/外部反馈=" << (结果.当前请求.存在待消费外部反馈 ? "是" : "否");
            }
            if (!结果.当前请求.恢复重建摘要.empty()) {
                请求摘要 << " | 恢复重建=" << 结果.当前请求.恢复重建摘要;
            }
            结果.当前请求.补充材料摘要 = 请求摘要.str();
        }

        结果.当前单步决策.当前方法需求位 = 当前方法需求位;
        结果.当前单步决策.当前本能能力缺口类型 = 当前本能能力缺口类型;
        结果.当前单步决策.当前触发来源 = 结果.当前触发来源;
        结果.当前单步决策.来源最小原语位图 = 结果.来源最小原语位图;
        结果.当前单步决策.来源主观察特征 = 结果.来源主观察特征;
        结果.当前单步决策.影子验证状态 = 结果.当前影子验证状态;
        结果.当前单步决策.允许正式资产提交 = 结果.允许正式资产提交;

        私有_P0_覆盖非空指针(结果.当前一步结果.宿主任务, 结果.上下文.当前宿主任务);
        结果.当前一步结果.当前主体ID = 结果.当前主体ID;
        结果.当前一步结果.当前分身ID = 结果.当前分身ID;
        结果.当前一步结果.当前方法需求位 = 当前方法需求位;
        结果.当前一步结果.当前本能能力缺口类型 = 当前本能能力缺口类型;
        结果.当前一步结果.当前触发来源 = 结果.当前触发来源;
        私有_P0_覆盖非空指针(结果.当前一步结果.当前方法节点, 结果.上下文.当前方法);
        私有_P0_覆盖非空指针(结果.当前一步结果.当前步骤节点, 私有_P0_当前步骤节点(结果));
        私有_P0_覆盖非空指针(结果.当前一步结果.最新结果节点, 私有_P0_最新结果节点(结果));
        结果.当前一步结果.当前输入条件主键 = 当前输入条件主键;
        结果.当前一步结果.当前治理态型 = 当前治理态型;
        结果.当前一步结果.来源最小原语位图 = 结果.来源最小原语位图;
        结果.当前一步结果.来源主观察特征 = 结果.来源主观察特征;
        结果.当前一步结果.影子验证状态 = 结果.当前影子验证状态;
        结果.当前一步结果.允许正式资产提交 = 结果.允许正式资产提交;
    }

    void 私有_应用半白盒桥接屏蔽(结构_任务管理结果& 结果) noexcept
    {
        const auto 控制 = 私有_读取半白盒治理控制();
        if (!控制.启用) {
            if (结果.当前触发来源 == 枚举_任务管理触发来源::未定义) {
                结果.当前触发来源 = 枚举_任务管理触发来源::自然运行态;
            }
            if (结果.当前影子验证状态.empty()) {
                结果.当前影子验证状态 = "未触发";
            }
            私有_P0_刷新主体桥接字段(结果);
            return;
        }

        结果.当前触发来源 = 枚举_任务管理触发来源::半白盒;
        结果.当前影子验证状态 =
            控制.影子验证状态.empty() ? std::string("待影子验证") : 控制.影子验证状态;
        结果.允许正式资产提交 = 控制.允许正式资产提交;
        if (!控制.来源主观察特征.empty()) {
            结果.来源主观察特征 = 控制.来源主观察特征;
        }

        switch (控制.屏蔽类型) {
        case 枚举_任务管理半白盒屏蔽类型::缺状态差分取证:
            结果.当前需求位特征.需要新学习 = false;
            结果.当前需求位特征.最新目标状态 = nullptr;
            结果.当前需求位特征.最新结果状态 = nullptr;
            if (结果.当前需求位特征.需求摘要.empty()) {
                结果.当前需求位特征.需求摘要 = "半白盒=屏蔽状态差分派生";
            }
            else {
                结果.当前需求位特征.需求摘要 += " | 半白盒=屏蔽状态差分派生";
            }
            if (结果.来源主观察特征.empty()) {
                结果.来源主观察特征 = "半白盒/主观察/状态差分";
            }
            break;
        case 枚举_任务管理半白盒屏蔽类型::缺轻量因果链组织:
            结果.当前需求位特征.需要新学习 = false;
            if (!结果.当前需求位特征.最新目标状态) {
                结果.当前需求位特征.最新目标状态 =
                    控制.借用目标状态
                    ? 控制.借用目标状态
                    : (
                        结果.宿主目标状态
                        ? 结果.宿主目标状态
                        : (结果.当前分身.宿主任务 ? 结果.当前分身.宿主任务->主信息.目标状态.指针 : nullptr)
                    );
            }
            if (!结果.当前需求位特征.最新结果状态) {
                结果.当前需求位特征.最新结果状态 =
                    控制.借用结果状态
                    ? 控制.借用结果状态
                    : (
                        结果.宿主结果状态
                        ? 结果.宿主结果状态
                        : (结果.当前分身.宿主任务 ? 结果.当前分身.宿主任务->主信息.结果状态信息.指针 : nullptr)
                    );
            }
            if (!结果.当前需求位特征.最近实例因果 && 控制.借用实例因果) {
                结果.当前需求位特征.最近实例因果 = 控制.借用实例因果;
            }
            结果.当前需求位特征.最近轻量因果链摘要.clear();
            if (结果.当前需求位特征.需求摘要.empty()) {
                结果.当前需求位特征.需求摘要 = "半白盒=屏蔽轻量因果链组装";
            }
            else {
                结果.当前需求位特征.需求摘要 += " | 半白盒=屏蔽轻量因果链组装";
            }
            if (结果.来源主观察特征.empty()) {
                结果.来源主观察特征 = "半白盒/主观察/轻量因果链";
            }
            break;
        default:
            break;
        }

        私有_P0_刷新主体桥接字段(结果);
    }

    bool 私有_形成治理特征面(结构_任务管理结果& 结果) noexcept
    {
        if (!结果.已初始化分身继承面) {
            return false;
        }

        auto* 宿主任务 = 私有_P0_当前宿主任务(结果);
        auto* 当前方法 = 私有_P0_当前方法(结果);
        auto* 当前步骤 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果 = 私有_P0_最新结果节点(结果);

        结果.当前分身.宿主任务 = 宿主任务;
        结果.当前分身.当前管理方法 = 当前方法;
        结果.当前分身.当前步骤 = 当前步骤;
        结果.当前分身.最近结果 = 最新结果;
        if (结果.当前分身.本轮输入条件主键.empty()) {
            结果.当前分身.本轮输入条件主键 = 私有_主体化输入条件主键(结果.上下文, 结果.当前分身);
        }
        if (结果.当前分身.本轮结果主键.empty()) {
            结果.当前分身.本轮结果主键 = 私有_主体化结果主键(结果.当前分身, 结果.上下文);
        }

        结果.当前需求位特征.当前功能域 = 私有_P0_当前功能域(结果);
        结果.当前需求位特征.当前缺口类型 = 私有_P0_当前缺口类型(结果);
        结果.当前需求位特征.当前下一步去向 = 私有_P0_当前下一步去向(结果);
        if (结果.当前需求位特征.输入条件主键.empty()) {
            结果.当前需求位特征.输入条件主键 = 结果.当前分身.本轮输入条件主键;
        }
        if (结果.当前需求位特征.结果主键.empty()) {
            结果.当前需求位特征.结果主键 = 结果.当前分身.本轮结果主键;
        }
        if (结果.最新目标状态) {
            结果.当前需求位特征.最新目标状态 = 结果.最新目标状态;
        }
        if (结果.最新状态) {
            结果.当前需求位特征.最新结果状态 = 结果.最新状态;
        }
        if (结果.最新动态) {
            结果.当前需求位特征.最近开始动态 = 结果.最新动态;
            结果.当前需求位特征.最近结束动态 = 结果.最新动态;
        }
        if (结果.最新因果) {
            结果.当前需求位特征.最近实例因果 = 结果.最新因果;
        }

        const auto& 最近轻量因果链摘要 = 私有_P0_选用非空文本(
            结果.当前需求位特征.最近轻量因果链摘要,
            结果.当前分身.继承摘要,
            私有_P0_最近反馈摘要(结果));
        if (!最近轻量因果链摘要.empty()) {
            结果.当前需求位特征.最近轻量因果链摘要 = 最近轻量因果链摘要;
        }

        私有_P0_刷新主体桥接字段(结果);
        return true;
    }

    bool 私有_命中方法需求位(结构_任务管理结果& 结果) noexcept
    {
        if (!结果.已初始化分身继承面) {
            return false;
        }

        const auto 已有输入条件主键 = 结果.当前需求位特征.输入条件主键;
        const auto 已有结果主键 = 结果.当前需求位特征.结果主键;
        const auto 已有轻量因果链摘要 = 结果.当前需求位特征.最近轻量因果链摘要;
        auto* 已有目标状态 = 结果.当前需求位特征.最新目标状态;
        auto* 已有结果状态 = 结果.当前需求位特征.最新结果状态;
        auto* 已有开始动态 = 结果.当前需求位特征.最近开始动态;
        auto* 已有结束动态 = 结果.当前需求位特征.最近结束动态;
        auto* 已有方法位专项动态 = 结果.当前需求位特征.当前方法位专项动态;
        auto* 已有实例因果 = 结果.当前需求位特征.最近实例因果;
        const auto 已有方法位专项动态标题 = 结果.当前需求位特征.当前方法位专项动态标题;

        结构_任务管理方法需求位特征面 新特征{};
        const bool 已命中 =
            任务管理任务模块::刷新任务管理方法需求位(
                结果.上下文,
                结果.当前分身,
                &新特征);
        if (!已命中) {
            return false;
        }

        if (新特征.输入条件主键.empty()) {
            新特征.输入条件主键 = 已有输入条件主键;
        }
        if (新特征.结果主键.empty()) {
            新特征.结果主键 = 已有结果主键;
        }
        if (新特征.最近轻量因果链摘要.empty()) {
            新特征.最近轻量因果链摘要 = 已有轻量因果链摘要;
        }
        if (!新特征.最新目标状态) {
            新特征.最新目标状态 = 已有目标状态;
        }
        if (!新特征.最新结果状态) {
            新特征.最新结果状态 = 已有结果状态;
        }
        if (!新特征.最近开始动态) {
            新特征.最近开始动态 = 已有开始动态;
        }
        if (!新特征.最近结束动态) {
            新特征.最近结束动态 = 已有结束动态;
        }
        if (!新特征.当前方法位专项动态) {
            新特征.当前方法位专项动态 = 已有方法位专项动态;
        }
        if (!新特征.最近实例因果) {
            新特征.最近实例因果 = 已有实例因果;
        }
        if (新特征.当前方法位专项动态标题.empty()) {
            新特征.当前方法位专项动态标题 = 已有方法位专项动态标题;
        }

        结果.当前需求位特征 = std::move(新特征);
        结果.已刷新方法需求位 = true;
        结果.当前主方法需求位 = 结果.当前需求位特征.当前需求位;
        结果.当前分身.当前主方法需求位 = 结果.当前主方法需求位;
        结果.当前单步决策.当前方法需求位 = 结果.当前主方法需求位;
        结果.当前一步结果.当前方法需求位 = 结果.当前主方法需求位;
        私有_P0_刷新主体桥接字段(结果);
        return true;
    }

    bool 私有_评估本能能力缺口(自我类& 自我对象, 结构_任务管理结果& 结果) noexcept
    {
        auto* 已有最小原语动态 = 结果.当前本能能力缺口.当前最小原语动态;
        const auto 已有最小原语动态标题 = 结果.当前本能能力缺口.当前最小原语动态标题;
        结构_任务管理本能能力缺口面 当前缺口{};
        const bool 已评估 =
            任务管理任务模块::评估任务管理本能能力缺口(
                自我对象,
                结果.当前需求位特征,
                &当前缺口);
        if (!已评估) {
            return false;
        }

        if (!当前缺口.当前最小原语动态) {
            当前缺口.当前最小原语动态 = 已有最小原语动态;
        }
        if (当前缺口.当前最小原语动态标题.empty()) {
            当前缺口.当前最小原语动态标题 = 已有最小原语动态标题;
        }
        if (!当前缺口.当前最小原语动态) {
            当前缺口.当前最小原语动态 = 结果.当前分身.当前最小原语动态;
        }
        if (当前缺口.当前最小原语动态标题.empty()) {
            当前缺口.当前最小原语动态标题 = 结果.当前分身.当前最小原语动态标题;
        }
        结果.当前本能能力缺口 = std::move(当前缺口);
        结果.已评估本能能力缺口 = true;
        结果.当前本能能力缺口类型 = 结果.当前本能能力缺口.当前缺口类型;
        结果.来源最小原语位图 = 结果.当前本能能力缺口.所需原语位图;
        if (结果.来源主观察特征.empty()) {
            结果.来源主观察特征 = 结果.当前本能能力缺口.主观察特征键;
        }
        结果.当前单步决策.当前本能能力缺口类型 = 结果.当前本能能力缺口类型;
        结果.当前一步结果.当前本能能力缺口类型 = 结果.当前本能能力缺口类型;
        私有_P0_刷新主体桥接字段(结果);
        return true;
    }

    bool 私有_桥接补层裁决(结构_任务管理结果& 结果) noexcept
    {
        const auto 当前需求位 = 私有_P0_当前方法需求位(结果);
        auto 当前本能缺口 = 私有_P0_当前本能缺口(结果);

        std::ostringstream 摘要;
        摘要 << "桥接补层裁决"
            << " | 需求位=" << 私有_方法需求位文本(当前需求位)
            << " | 本能缺口=" << 私有_本能能力缺口文本(当前本能缺口)
            << " | 本轮原语命中=" << (结果.来源最小原语位图 == 0 ? "空" : 私有_原语位图摘要(结果.来源最小原语位图))
            << " | 正式已具备=" << (结果.当前本能能力缺口.原语账本摘要.empty() ? "空" : 结果.当前本能能力缺口.原语账本摘要)
            << " | 派生摘要=" << (结果.当前本能能力缺口.派生规则摘要.empty() ? "空" : 结果.当前本能能力缺口.派生规则摘要);

        if (私有_M10_主体长期策略允许直放后段(结果)) {
            结果.当前本能能力缺口.当前缺口类型 = 枚举_任务管理本能能力缺口类型::无缺口;
            结果.当前本能能力缺口.需要学习补齐 = false;
            结果.当前本能能力缺口.允许直接执行 = true;
            结果.当前本能能力缺口类型 = 枚举_任务管理本能能力缺口类型::无缺口;
            结果.当前单步决策.当前本能能力缺口类型 = 结果.当前本能能力缺口类型;
            结果.当前一步结果.当前本能能力缺口类型 = 结果.当前本能能力缺口类型;
            结果.当前写回结果.结构动作摘要 =
                摘要.str() + " | 结论=主体长期策略正式资产已验证，宿主存在目标差额，放行后段";
            私有_P0_刷新主体桥接字段(结果);
            return true;
        }

        if (当前本能缺口 == 枚举_任务管理本能能力缺口类型::无缺口) {
            结果.当前写回结果.结构动作摘要 = 摘要.str() + " | 结论=放行后段";
            私有_P0_刷新主体桥接字段(结果);
            return true;
        }

        if (当前本能缺口 == 枚举_任务管理本能能力缺口类型::缺挂点原语) {
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::筹办,
                枚举_任务管理缺口类型::方法挂点缺失,
                枚举_任务管理下一步去向::绑定方法,
                枚举_任务管理治理状态迁移::进入挂起,
                枚举_任务管理总控结果::宿主已挂起,
                枚举_任务管理反馈类型::重筹办提示,
                摘要.str() + " | 结论=退回补结构/绑定方法");
        }
        else if (结果.当前本能能力缺口.需要退回补结构) {
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::筹办,
                枚举_任务管理缺口类型::任务待启动,
                枚举_任务管理下一步去向::回到重筹办,
                枚举_任务管理治理状态迁移::回到重筹办,
                枚举_任务管理总控结果::宿主待重筹办,
                枚举_任务管理反馈类型::重筹办提示,
                摘要.str() + " | 结论=退回补结构");
        }
        else if (结果.当前本能能力缺口.需要学习补齐
            || 当前本能缺口 == 枚举_任务管理本能能力缺口类型::需学习补齐) {
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::等待,
                枚举_任务管理缺口类型::任务运行中,
                枚举_任务管理下一步去向::保持等待,
                枚举_任务管理治理状态迁移::保持等待,
                枚举_任务管理总控结果::宿主保持等待,
                枚举_任务管理反馈类型::等待维持,
                摘要.str() + " | 结论=建议学习补齐，本轮只输出显式建议");
        }
        else {
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::等待,
                枚举_任务管理缺口类型::任务运行中,
                枚举_任务管理下一步去向::保持等待,
                枚举_任务管理治理状态迁移::保持等待,
                枚举_任务管理总控结果::宿主保持等待,
                枚举_任务管理反馈类型::等待维持,
                摘要.str() + " | 结论=等待原语/派生证据补齐");
        }

        结果.当前写回结果.结构动作摘要 = 私有_P0_最近反馈摘要(结果);
        私有_P0_刷新主体桥接字段(结果);
        return false;
    }

    std::string 私有_任务管理方法位标题(const 结构_任务管理结果& 结果) noexcept
    {
        switch (私有_P0_当前方法需求位(结果)) {
        case 枚举_任务管理方法需求位::收束复盘:
            return "管理_复盘器";
        case 枚举_任务管理方法需求位::失败策略:
            return "管理_失败策略器";
        case 枚举_任务管理方法需求位::门控转域:
            return "管理_门控器";
        case 枚举_任务管理方法需求位::分解补条件:
            return "管理_分解器";
        case 枚举_任务管理方法需求位::求证判定:
            return "管理_求证器";
        case 枚举_任务管理方法需求位::补结构:
            return "管理_筹办器";
        case 枚举_任务管理方法需求位::选择推进:
            return "管理_选择器";
        default:
            break;
        }

        const auto 当前功能域 = 私有_P0_当前功能域(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前步骤位类型 = 私有_P0_当前步骤位类型(结果);
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);

        if (当前功能域 == 枚举_任务管理功能域::收束
            || 当前下一步去向 == 枚举_任务管理下一步去向::进入收束) {
            return "管理_复盘器";
        }
        if (当前功能域 == 枚举_任务管理功能域::纠偏) {
            return "管理_失败策略器";
        }
        if (结果.最近执行前门控结果 != 枚举_任务管理执行前门控结果::允许普通执行
            || 当前功能域 == 枚举_任务管理功能域::等待) {
            return "管理_门控器";
        }
        if (结果.已创建补条件子任务
            || 结果.已复用补条件子任务
            || 结果.已创建业务子任务
            || 结果.已复用业务子任务
            || 当前步骤位类型 == 枚举_任务管理步骤位类型::补条件位) {
            return "管理_分解器";
        }
        if (结果.宿主目标结果已对齐
            || 结果.宿主目标结果待推进
            || 结果.当前宿主目标结果比较 != 枚举_任务管理双面比较结果::未定义
            || 结果.当前步骤目标结果比较 != 枚举_任务管理双面比较结果::未定义
            || 结果.当前结果目标结果比较 != 枚举_任务管理双面比较结果::未定义) {
            return "管理_求证器";
        }
        if (当前功能域 == 枚举_任务管理功能域::筹办
            || 当前缺口类型 == 枚举_任务管理缺口类型::治理任务缺失
            || 当前缺口类型 == 枚举_任务管理缺口类型::宿主任务缺失
            || 当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失) {
            return "管理_筹办器";
        }
        return "管理_选择器";
    }

    void 私有_设定治理结论(
        结构_任务管理结果& 结果,
        枚举_任务管理功能域 功能域,
        枚举_任务管理缺口类型 缺口类型,
        枚举_任务管理下一步去向 下一步去向,
        枚举_任务管理治理状态迁移 状态迁移,
        枚举_任务管理总控结果 总控结果,
        枚举_任务管理反馈类型 反馈类型,
        std::string 反馈摘要) noexcept
    {
        结构_任务管理单步决策 当前决策{};
        当前决策.当前功能域 = 功能域;
        当前决策.当前缺口类型 = 缺口类型;
        当前决策.当前下一步去向 = 下一步去向;
        当前决策.当前治理状态迁移 = 状态迁移;
        当前决策.决策摘要 = 反馈摘要;
        私有_P0_覆盖决策字段(结果.当前单步决策, 当前决策);

        结构_任务管理一步结果 当前一步结果{};
        当前一步结果.当前功能域 = 功能域;
        当前一步结果.当前缺口类型 = 缺口类型;
        当前一步结果.当前下一步去向 = 下一步去向;
        当前一步结果.当前治理状态迁移 = 状态迁移;
        当前一步结果.最近总控结果 = 总控结果;
        当前一步结果.最近反馈类型 = 反馈类型;
        当前一步结果.最近反馈摘要 = 反馈摘要;
        私有_P0_覆盖一步结果字段(结果.当前一步结果, 当前一步结果);

        结果.当前功能域 = 功能域;
        结果.当前缺口类型 = 缺口类型;
        结果.当前下一步去向 = 下一步去向;
        结果.当前治理状态迁移 = 状态迁移;
        结果.最近功能域 = 功能域;
        结果.最近总控结果 = 总控结果;
        结果.最近反馈类型 = 反馈类型;
        结果.最近反馈摘要 = std::move(反馈摘要);
    }

    void 私有_同步治理上下文镜像(结构_任务管理结果& 结果) noexcept
    {
        私有_P0_刷新主体桥接字段(结果);
        auto 阶段上下文 = 结果.上下文;
        私有_P0_覆盖请求到上下文字段(阶段上下文, 结果.当前请求);
        私有_P0_覆盖兼容到上下文字段(阶段上下文, 结果);
        私有_P0_覆盖决策到上下文字段(阶段上下文, 结果.当前单步决策);
        私有_P0_覆盖写回到上下文字段(阶段上下文, 结果.当前写回结果);
        私有_P0_覆盖一步结果到上下文字段(阶段上下文, 结果.当前一步结果);
        阶段上下文.最近反馈摘要 = 私有_P0_选用非空文本(
            结果.当前一步结果.最近反馈摘要,
            结果.最近反馈摘要,
            结果.当前单步决策.决策摘要);
        私有_按宿主清洗治理上下文镜像(阶段上下文);
        结果.上下文 = std::move(阶段上下文);
    }

    void 私有_完成治理轮收口(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        结果.宿主任务新状态 = 宿主任务 ? 宿主任务->主信息.状态 : 枚举_任务状态::未定义;
        if (!结果.当前步骤节点) {
            结果.当前步骤节点 = 结果.上下文.当前步骤;
        }
        if (!结果.最新结果节点) {
            结果.最新结果节点 = 结果.上下文.最近结果;
        }

        私有_P0_覆盖非空指针(结果.当前写回结果.当前步骤节点, 私有_P0_当前步骤节点(结果));
        私有_P0_覆盖非空指针(结果.当前写回结果.最新结果节点, 私有_P0_最新结果节点(结果));
        私有_P0_覆盖非空指针(结果.当前一步结果.当前方法节点, 私有_P0_当前方法(结果));

        if (结果.已初始化分身继承面) {
            结果.当前分身.最近推进时间 = 当前时间;
            结果.当前分身.最近回并时间 = 当前时间;
            结果.当前分身.回并序号 = 结果.当前主体.最近回并序号 + 1;
            结果.当前分身.宿主任务 = 宿主任务;
            结果.当前分身.当前步骤 = 私有_P0_当前步骤节点(结果);
            结果.当前分身.最近结果 = 私有_P0_最新结果节点(结果);
            结果.当前分身.当前管理方法 = 私有_P0_当前方法(结果);
            结果.当前分身.当前主方法需求位 = 私有_P0_当前方法需求位(结果);
            结果.当前分身.当前本能能力缺口类型 = 私有_P0_当前本能缺口(结果);
            结果.当前分身.当前治理态型 = 私有_治理态型文本(结果);
            结果.当前分身.本轮输入条件主键 = 私有_主体化输入条件主键(结果.上下文, 结果.当前分身);
            结果.当前分身.本轮结果主键 = 私有_主体化结果主键(结果.当前分身, 结果.上下文);
            结果.当前分身.本地推进次数 = 结果.已推进任务状态 ? 1U : 0U;
            结果.当前分身.本地完成次数 =
                结果.宿主任务新状态 == 枚举_任务状态::完成 ? 1U : 0U;
            结果.当前分身.本地失败次数 =
                (结果.宿主任务新状态 == 枚举_任务状态::失败
                    || 结果.宿主任务新状态 == 枚举_任务状态::取消)
                ? 1U
                : 0U;
            结果.当前分身.本地弱出生证据次数 = 结果.已创建学习方法头 ? 1U : 0U;
            结果.当前分身.本地验证证据次数 = 结果.已生成治理证据 ? 1U : 0U;
            结果.当前分身.本地升格证据次数 = 结果.已创建学习方法结果节点 ? 1U : 0U;
            结果.当前分身.本地价值证据次数 = 结果.已写回目标状态 ? 1U : 0U;
            结果.当前分身.当前状态 = 枚举_任务管理分身状态::等待回并;
            私有_刷新分身继承位图(结果.当前分身, 结果.上下文);

            (void)私有_形成治理特征面(结果);
            (void)私有_命中方法需求位(结果);
            if (!结果.最近反馈摘要.empty()) {
                结果.当前需求位特征.最近轻量因果链摘要 = 结果.最近反馈摘要;
            }
            私有_应用半白盒桥接屏蔽(结果);
            (void)私有_评估本能能力缺口(自我对象, 结果);
        }

        if (结果.已确保任务管理主体虚拟存在) {
            结果.当前主体.最近功能域 = 私有_P0_当前功能域(结果);
            结果.当前主体.最近主缺口类型 = 私有_P0_当前缺口类型(结果);
            结果.当前主体.最近下一步去向 = 私有_P0_当前下一步去向(结果);
            结果.当前主体.最近总控结果 = 私有_P0_最近总控结果(结果);
            结果.当前主体.最近主方法需求位 = 私有_P0_当前方法需求位(结果);
            私有_P0_覆盖非默认值(
                结果.当前主体.最近本能能力缺口类型,
                私有_P0_当前本能缺口(结果),
                枚举_任务管理本能能力缺口类型::未定义);
            私有_P0_覆盖非空文本(结果.当前主体.最近治理态型, 私有_治理态型文本(结果));
            结果.当前主体.最近输入条件主键 = 结果.当前分身.本轮输入条件主键;
            结果.当前主体.最近结果主键 = 结果.当前分身.本轮结果主键;
            结果.当前主体.最近原因摘要 = 私有_P0_最近反馈摘要(结果);
        }

        if (结果.已初始化分身继承面 && 结果.已确保任务管理主体虚拟存在) {
            结果.已回并分身增量 =
                任务管理任务模块::回并分身增量到任务管理主体(
                    结果.当前分身,
                    &结果.当前主体);
            if (结果.已回并分身增量) {
                std::ostringstream 主体回并摘要;
                主体回并摘要 << "主体ID=" << 结果.当前主体.主体ID
                    << " | 分身ID=" << 结果.当前分身.分身ID
                    << " | 需求位=" << 私有_方法需求位文本(结果.当前分身.当前主方法需求位)
                    << " | 输入条件=" << (结果.当前分身.本轮输入条件主键.empty() ? "空" : 结果.当前分身.本轮输入条件主键);
                结果.当前写回结果.主体回并摘要 = 主体回并摘要.str();
                结果.当前一步结果.主体回并摘要 = 结果.当前写回结果.主体回并摘要;
            }
        }

        私有_M10_回写长期策略可靠度(宿主任务, 当前时间, 结果);
        私有_P0_刷新主体桥接字段(结果);
        私有_同步治理上下文镜像(结果);

        (void)私有_记录任务管理方法位专项动态(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/方法位动态/开始",
            结果,
            枚举_动作事件相位::开始运行);

        私有_写回治理稳定字段(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/证据/稳定字段",
            结果);

        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前步骤指针(),
            私有_P0_当前步骤节点(结果),
            私有_任务标题(私有_P0_当前步骤节点(结果)),
            "任务管理_当前步骤推进",
            私有_P0_当前方法(结果),
            宿主任务,
            当前时间,
            调用点 + "/证据/当前步骤",
            结果);

        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近结果指针(),
            私有_P0_最新结果节点(结果),
            私有_任务标题(私有_P0_最新结果节点(结果)),
            "任务管理_最近结果推进",
            私有_P0_当前方法(结果),
            宿主任务,
            当前时间,
            调用点 + "/证据/最近结果",
            结果);

        (void)私有_记录任务管理方法位专项动态(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/方法位动态/完成",
            结果,
            枚举_动作事件相位::完成);
    }

    bool 私有_应用上位输入约束(结构_任务管理结果& 结果) noexcept
    {
        结果.最近根层重判结果 = 结果.上下文.最近根层重判结果;
        结果.最近执行前门控结果 = 结果.上下文.最近执行前门控结果;
        结果.最近上层反馈摘要 = 结果.上下文.最近上层反馈摘要;

        const auto 根层结果 = 结果.上下文.最近根层重判结果;
        const auto 门控结果 = 结果.上下文.最近执行前门控结果;

        if (门控结果 == 枚举_任务管理执行前门控结果::禁止继续
            || 根层结果 == 枚举_任务管理根层重判结果::停止退出边界) {
            结果.已受上位输入约束 = true;
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::收束,
                枚举_任务管理缺口类型::上位停止约束,
                枚举_任务管理下一步去向::进入收束,
                枚举_任务管理治理状态迁移::进入收束,
                枚举_任务管理总控结果::宿主进入收束,
                枚举_任务管理反馈类型::收束回执,
                "上位输入命中停止退出边界，任务治理只做收口");
            return true;
        }

        if (门控结果 == 枚举_任务管理执行前门控结果::仅允许收束
            || 根层结果 == 枚举_任务管理根层重判结果::收束优先) {
            结果.已受上位输入约束 = true;
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::收束,
                枚举_任务管理缺口类型::上位收束约束,
                枚举_任务管理下一步去向::进入收束,
                枚举_任务管理治理状态迁移::进入收束,
                枚举_任务管理总控结果::宿主进入收束,
                枚举_任务管理反馈类型::收束回执,
                "上位输入要求优先收束，本轮不继续普通推进");
            return true;
        }

        if (门控结果 == 枚举_任务管理执行前门控结果::仅允许等待
            || 根层结果 == 枚举_任务管理根层重判结果::待机保持) {
            结果.已受上位输入约束 = true;
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::等待,
                枚举_任务管理缺口类型::上位等待约束,
                枚举_任务管理下一步去向::保持等待,
                枚举_任务管理治理状态迁移::保持等待,
                枚举_任务管理总控结果::宿主保持等待,
                枚举_任务管理反馈类型::等待维持,
                "上位输入要求保持等待，本轮不进入普通执行链");
            return true;
        }

        return false;
    }

    任务节点* 私有_读取任务节点指针特征(
        const 自我类& 自我对象,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!特征类型) {
            return nullptr;
        }
        if (!自我对象.读取自我指针特征当前值(特征类型, 指针值, 调用点)) {
            return nullptr;
        }
        return reinterpret_cast<任务节点*>(指针值);
    }

    template<class T枚举>
    T枚举 私有_读取I64枚举特征(
        const 自我类& 自我对象,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        I64 当前值 = 0;
        if (!特征类型 || !自我对象.读取自我I64特征当前值(特征类型, 当前值, 调用点)) {
            return T枚举::未定义;
        }
        return static_cast<T枚举>(当前值);
    }

    std::string 私有_读取反馈摘要特征(
        const 自我类& 自我对象,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!特征类型 || !自我对象.读取自我指针特征当前值(特征类型, 指针值, 调用点)) {
            return {};
        }

        return 私有_安全词(reinterpret_cast<const 词性节点类*>(指针值));
    }

    std::uint64_t 私有_读取U64文本特征(
        const 自我类& 自我对象,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        return 私有_安全解析U64(
            私有_读取反馈摘要特征(自我对象, 特征类型, 调用点));
    }

    bool 私有_读取指针特征(const 特征节点类* 特征, std::uintptr_t& 输出值) noexcept
    {
        if (!特征) return false;
        const auto 快照 = 世界树.读取特征快照(特征);
        const auto* 指针 = std::get_if<指针句柄>(&快照);
        if (!指针) return false;
        输出值 = 指针->指针;
        return true;
    }

    bool 私有_读取I64特征(const 特征节点类* 特征, I64& 输出值) noexcept
    {
        return 特征 && 世界树.读取特征_I64(特征, 输出值);
    }

    bool 私有_读取宿主任务治理I64特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64& 输出值,
        const std::string& 调用点) noexcept
    {
        if (!特征类型) {
            return false;
        }
        if (const auto* 特征节点 = 私有_查找宿主任务治理特征节点(宿主任务, 特征类型);
            私有_读取I64特征(特征节点, 输出值)) {
            return true;
        }
        (void)自我对象;
        (void)调用点;
        return false;
    }

    bool 私有_读取宿主任务治理指针特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        std::uintptr_t& 输出值,
        const std::string& 调用点) noexcept
    {
        if (!特征类型) {
            return false;
        }
        if (const auto* 特征节点 = 私有_查找宿主任务治理特征节点(宿主任务, 特征类型);
            私有_读取指针特征(特征节点, 输出值)) {
            return true;
        }
        (void)自我对象;
        (void)调用点;
        return false;
    }

    任务节点* 私有_读取宿主任务治理任务节点指针特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!私有_读取宿主任务治理指针特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            指针值,
            调用点)) {
            return nullptr;
        }
        return reinterpret_cast<任务节点*>(指针值);
    }

    方法节点* 私有_读取宿主任务治理方法节点指针特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!私有_读取宿主任务治理指针特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            指针值,
            调用点)) {
            return nullptr;
        }
        return reinterpret_cast<方法节点*>(指针值);
    }

    template<class T枚举>
    T枚举 私有_读取宿主任务治理I64枚举特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        I64 当前值 = 0;
        if (!私有_读取宿主任务治理I64特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            当前值,
            调用点)) {
            return T枚举::未定义;
        }
        return static_cast<T枚举>(当前值);
    }

    std::string 私有_读取宿主任务治理反馈摘要特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!私有_读取宿主任务治理指针特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            指针值,
            调用点)) {
            return {};
        }
        return 私有_安全词(reinterpret_cast<const 词性节点类*>(指针值));
    }

    bool 私有_读取主体桥接I64特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64& 输出值,
        const std::string& 调用点) noexcept
    {
        if (!特征类型) {
            return false;
        }
        if (const auto* 特征节点 = 私有_查找主体桥接特征节点(宿主任务, 特征类型);
            私有_读取I64特征(特征节点, 输出值)) {
            return true;
        }
        (void)自我对象;
        (void)调用点;
        return false;
    }

    bool 私有_读取主体桥接指针特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        std::uintptr_t& 输出值,
        const std::string& 调用点) noexcept
    {
        if (!特征类型) {
            return false;
        }
        if (const auto* 特征节点 = 私有_查找主体桥接特征节点(宿主任务, 特征类型);
            私有_读取指针特征(特征节点, 输出值)) {
            return true;
        }
        (void)自我对象;
        (void)调用点;
        return false;
    }

    template<class T枚举>
    T枚举 私有_读取主体桥接I64枚举特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        I64 当前值 = 0;
        if (!私有_读取主体桥接I64特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            当前值,
            调用点)) {
            return T枚举::未定义;
        }
        return static_cast<T枚举>(当前值);
    }

    std::string 私有_读取主体桥接反馈摘要特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!私有_读取主体桥接指针特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            指针值,
            调用点)) {
            return {};
        }
        return 私有_安全词(reinterpret_cast<const 词性节点类*>(指针值));
    }

    std::uint64_t 私有_读取主体桥接U64文本特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        return 私有_安全解析U64(
            私有_读取主体桥接反馈摘要特征(
                自我对象,
                宿主任务,
                特征类型,
                调用点));
    }

    bool 私有_任务引用匹配(
        const 任务节点* 宿主任务,
        const 可解析引用<任务节点类>& 宿主引用) noexcept
    {
        if (!宿主任务) {
            return false;
        }
        if (宿主引用.指针 == reinterpret_cast<const 任务节点类*>(宿主任务)) {
            return true;
        }
        return !宿主引用.主键.empty()
            && 宿主引用.主键 == 宿主任务->获取主键();
    }

    bool 私有_桥接上下文处于恢复稀疏态(const 结构_任务管理上下文& 上下文) noexcept
    {
        return !上下文.当前方法
            && !上下文.当前步骤
            && !上下文.最近结果;
    }

    void 私有_从最近治理结果补齐主体桥接字段(结构_任务管理上下文& 上下文) noexcept
    {
        if (!上下文.当前宿主任务) {
            return;
        }

        结构_任务管理结果 最近治理结果{};
        if (!任务管理任务模块::读取最近治理结果(&最近治理结果)
            || 最近治理结果.上下文.当前宿主任务 != 上下文.当前宿主任务) {
            return;
        }

        const auto 最近主体ID = 最近治理结果.当前主体ID != 0
            ? 最近治理结果.当前主体ID
            : 最近治理结果.上下文.当前主体ID;
        if (上下文.当前主体ID == 0 && 最近主体ID != 0) {
            上下文.当前主体ID = 最近主体ID;
        }

        const auto* 最近方法 = 私有_P0_当前方法(最近治理结果);
        if (上下文.当前方法 && 最近方法 && 最近方法 != 上下文.当前方法) {
            return;
        }

        const auto 最近分身ID = 最近治理结果.当前分身ID != 0
            ? 最近治理结果.当前分身ID
            : 最近治理结果.上下文.当前分身ID;
        if (上下文.当前分身ID == 0 && 最近分身ID != 0) {
            上下文.当前分身ID = 最近分身ID;
        }

        const auto 最近方法需求位 = 私有_P0_当前方法需求位(最近治理结果);
        if (上下文.当前方法需求位 == 枚举_任务管理方法需求位::未定义
            && 最近方法需求位 != 枚举_任务管理方法需求位::未定义) {
            上下文.当前方法需求位 = 最近方法需求位;
        }

        const auto 最近本能缺口 = 私有_P0_当前本能缺口(最近治理结果);
        if (上下文.当前本能能力缺口类型 == 枚举_任务管理本能能力缺口类型::未定义
            && 最近本能缺口 != 枚举_任务管理本能能力缺口类型::未定义) {
            上下文.当前本能能力缺口类型 = 最近本能缺口;
        }
        if (!上下文.当前方法位专项动态 && 最近治理结果.当前需求位特征.当前方法位专项动态) {
            上下文.当前方法位专项动态 = 最近治理结果.当前需求位特征.当前方法位专项动态;
        }
        if (上下文.当前方法位专项动态标题.empty()) {
            上下文.当前方法位专项动态标题 = 私有_P0_选用非空文本(
                最近治理结果.当前需求位特征.当前方法位专项动态标题,
                最近治理结果.上下文.当前方法位专项动态标题,
                std::string{});
        }
        if (!上下文.当前最小原语动态 && 最近治理结果.当前本能能力缺口.当前最小原语动态) {
            上下文.当前最小原语动态 = 最近治理结果.当前本能能力缺口.当前最小原语动态;
        }
        if (上下文.当前最小原语动态标题.empty()) {
            上下文.当前最小原语动态标题 = 私有_P0_选用非空文本(
                最近治理结果.当前本能能力缺口.当前最小原语动态标题,
                最近治理结果.上下文.当前最小原语动态标题,
                std::string{});
        }

        if (上下文.当前输入条件主键.empty()) {
            上下文.当前输入条件主键 = 私有_P0_选用非空文本(
                最近治理结果.当前一步结果.当前输入条件主键,
                最近治理结果.上下文.当前输入条件主键,
                私有_P0_选用非空文本(
                    最近治理结果.当前分身.本轮输入条件主键,
                    最近治理结果.当前需求位特征.输入条件主键,
                    std::string{}));
        }

        if (上下文.当前治理态型.empty()) {
            上下文.当前治理态型 = 私有_P0_选用非空文本(
                最近治理结果.当前一步结果.当前治理态型,
                最近治理结果.上下文.当前治理态型,
                std::string{});
        }
    }

    void 私有_从治理恢复快照补齐主体桥接字段(结构_任务管理上下文& 上下文) noexcept
    {
        if (!上下文.当前宿主任务 || !私有_桥接上下文处于恢复稀疏态(上下文)) {
            return;
        }

        结构_治理恢复快照 恢复快照{};
        if (!任务管理任务模块::读取治理恢复快照(&恢复快照)
            || !私有_任务引用匹配(上下文.当前宿主任务, 恢复快照.宿主任务)) {
            return;
        }

        if (上下文.当前主体ID == 0 && 恢复快照.当前主体ID != 0) {
            上下文.当前主体ID = 恢复快照.当前主体ID;
        }
        if (上下文.当前分身ID == 0 && 恢复快照.当前分身ID != 0) {
            上下文.当前分身ID = 恢复快照.当前分身ID;
        }
        if (上下文.当前方法需求位 == 枚举_任务管理方法需求位::未定义
            && 恢复快照.最近方法需求位 != 枚举_任务管理方法需求位::未定义) {
            上下文.当前方法需求位 = 恢复快照.最近方法需求位;
        }
        if (上下文.当前本能能力缺口类型 == 枚举_任务管理本能能力缺口类型::未定义
            && 恢复快照.最近本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义) {
            上下文.当前本能能力缺口类型 = 恢复快照.最近本能能力缺口类型;
        }
        if (上下文.当前输入条件主键.empty()) {
            上下文.当前输入条件主键 = 恢复快照.最近条件主键;
        }
        if (上下文.当前治理态型.empty()) {
            上下文.当前治理态型 = 恢复快照.最近治理态型;
        }
        if (上下文.当前方法位专项动态标题.empty()) {
            上下文.当前方法位专项动态标题 = 恢复快照.最近方法位专项动态标题;
        }
        if (上下文.当前最小原语动态标题.empty()) {
            上下文.当前最小原语动态标题 = 恢复快照.最近最小原语动态标题;
        }
    }

    void 私有_从任务管理主体虚拟存在补齐主体ID(结构_任务管理上下文& 上下文) noexcept
    {
        if (!上下文.当前宿主任务) {
            return;
        }

        结构_任务管理主体虚拟存在 主体{};
        if (任务管理任务模块::读取任务管理主体虚拟存在(
            上下文.当前宿主任务,
            &主体) && 主体.主体ID != 0) {
            if (上下文.当前主体ID == 0) {
                上下文.当前主体ID = 主体.主体ID;
            }
            if (上下文.当前方法需求位 == 枚举_任务管理方法需求位::未定义
                && 主体.最近主方法需求位 != 枚举_任务管理方法需求位::未定义) {
                上下文.当前方法需求位 = 主体.最近主方法需求位;
            }
            if (上下文.当前本能能力缺口类型 == 枚举_任务管理本能能力缺口类型::未定义
                && 主体.最近本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义) {
                上下文.当前本能能力缺口类型 = 主体.最近本能能力缺口类型;
            }
            if (上下文.当前输入条件主键.empty()) {
                上下文.当前输入条件主键 = 主体.最近输入条件主键;
            }
            if (上下文.当前治理态型.empty()) {
                上下文.当前治理态型 = 主体.最近治理态型;
            }
            if (!上下文.当前方法位专项动态 && 主体.最近方法位专项动态) {
                上下文.当前方法位专项动态 = 主体.最近方法位专项动态;
            }
            if (上下文.当前方法位专项动态标题.empty()) {
                上下文.当前方法位专项动态标题 = 主体.最近方法位专项动态标题;
            }
            if (!上下文.当前最小原语动态 && 主体.最近最小原语动态) {
                上下文.当前最小原语动态 = 主体.最近最小原语动态;
            }
            if (上下文.当前最小原语动态标题.empty()) {
                上下文.当前最小原语动态标题 = 主体.最近最小原语动态标题;
            }
        }
    }

    void 私有_按正式主体桥接结构补齐上下文(结构_任务管理上下文& 上下文) noexcept
    {
        私有_从最近治理结果补齐主体桥接字段(上下文);
        私有_从治理恢复快照补齐主体桥接字段(上下文);
        私有_从任务管理主体虚拟存在补齐主体ID(上下文);
    }

    枚举_方法类型 私有_推导动作句柄方法类型(const 结构体_动作句柄& 动作句柄) noexcept
    {
        switch (动作句柄.类型) {
        case 枚举_动作句柄类型::外部实现主键:
            return 动作句柄.外部实现主键.empty()
                ? 枚举_方法类型::未定义
                : 枚举_方法类型::外部方法;
        case 枚举_动作句柄类型::动作序列主键:
            return 动作句柄.动作序列主键.empty()
                ? 枚举_方法类型::未定义
                : 枚举_方法类型::内部方法;
        case 枚举_动作句柄类型::本能函数ID: {
            if (动作句柄.本能ID == 0) {
                return 枚举_方法类型::未定义;
            }
            const auto* 元信息 = 本能方法类::查询元信息(
                static_cast<枚举_本能方法ID>(动作句柄.本能ID));
            if (元信息 && 元信息->分类 == 枚举_本能方法分类::外部动作) {
                return 枚举_方法类型::外部方法;
            }
            return 枚举_方法类型::内部方法;
        }
        default:
            return 枚举_方法类型::未定义;
        }
    }

    枚举_方法类型 私有_读取方法类型特征(const 方法节点* 方法头) noexcept
    {
        auto* 方法虚拟存在 = 方法头 ? 方法头->主信息.方法虚拟存在.指针 : nullptr;
        auto* 宿主基础节点 = reinterpret_cast<const 基础信息节点类*>(方法虚拟存在);
        auto* 特征节点 = 宿主基础节点
            ? 世界树.存在().查找子特征_按类型(宿主基础节点, 私有_方法特征_方法类型())
            : nullptr;
        I64 当前值 = 0;
        if (!私有_读取I64特征(特征节点, 当前值)) {
            return 枚举_方法类型::未定义;
        }
        return static_cast<枚举_方法类型>(当前值);
    }

    枚举_方法类型 私有_推导方法类型(
        const 方法节点* 方法头,
        const 结构_任务管理结果* 结果 = nullptr) noexcept
    {
        if (const auto 类型 = 私有_读取方法类型特征(方法头);
            类型 != 枚举_方法类型::未定义) {
            return 类型;
        }

        if (方法头) {
            if (const auto 类型 = 私有_推导动作句柄方法类型(方法头->主信息.动作句柄);
                类型 != 枚举_方法类型::未定义) {
                return 类型;
            }
        }

        const auto* 来源方法 = 结果 ? 结果->上下文.当前方法 : nullptr;
        if (来源方法 && 来源方法 != 方法头) {
            if (const auto 类型 = 私有_读取方法类型特征(来源方法);
                类型 != 枚举_方法类型::未定义) {
                return 类型;
            }
            return 私有_推导动作句柄方法类型(来源方法->主信息.动作句柄);
        }

        return 枚举_方法类型::未定义;
    }

    std::uint64_t 私有_节点主键摘要(const auto* 节点) noexcept
    {
        if (!节点) return 0;
        return static_cast<std::uint64_t>(std::hash<std::string>{}(节点->获取主键()));
    }

    任务节点* 私有_主体锚点任务(任务节点* 宿主任务) noexcept
    {
        auto* 当前 = 宿主任务;
        while (当前) {
            auto* 父任务 = 当前->主信息.父任务.指针
                ? reinterpret_cast<任务节点*>(当前->主信息.父任务.指针)
                : nullptr;
            if (!父任务 || 父任务 == 当前) {
                break;
            }
            当前 = 父任务;
        }
        return 当前 ? 当前 : 宿主任务;
    }

    std::uint64_t 私有_主体ID_按宿主任务(任务节点* 宿主任务) noexcept
    {
        auto* 锚点任务 = 私有_主体锚点任务(宿主任务);
        const auto 锚点ID = 私有_节点主键摘要(锚点任务 ? 锚点任务 : 宿主任务);
        if (锚点ID != 0) {
            return 锚点ID;
        }

        const auto* 回退节点 = 锚点任务 ? 锚点任务 : 宿主任务;
        return 回退节点
            ? static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(回退节点))
            : 0;
    }

    std::int32_t 私有_Q10000比率(I64 当前值, I64 目标值) noexcept
    {
        if (目标值 <= 0) {
            return 0;
        }

        auto 比率 = static_cast<long long>(当前值) * 10000LL / static_cast<long long>(目标值);
        if (比率 < 0) 比率 = 0;
        if (比率 > 20000) 比率 = 20000;
        return static_cast<std::int32_t>(比率);
    }

    void 私有_刷新分身继承位图(
        结构_任务管理分身继承面& 分身,
        const 结构_任务管理上下文& 上下文) noexcept
    {
        分身.继承任务特征位图 = 0;
        if (分身.宿主任务) 分身.继承任务特征位图 |= (1ULL << 0);
        if (分身.当前步骤) 分身.继承任务特征位图 |= (1ULL << 1);
        if (分身.最近结果) 分身.继承任务特征位图 |= (1ULL << 2);
        if (上下文.最近功能域 != 枚举_任务管理功能域::未定义) 分身.继承任务特征位图 |= (1ULL << 3);
        if (上下文.当前缺口类型 != 枚举_任务管理缺口类型::未定义) 分身.继承任务特征位图 |= (1ULL << 4);
        if (上下文.当前下一步去向 != 枚举_任务管理下一步去向::未定义) 分身.继承任务特征位图 |= (1ULL << 5);
        if (上下文.最近根层重判结果 != 枚举_任务管理根层重判结果::未定义) 分身.继承任务特征位图 |= (1ULL << 6);
        if (上下文.最近执行前门控结果 != 枚举_任务管理执行前门控结果::未定义) 分身.继承任务特征位图 |= (1ULL << 7);

        分身.继承方法特征位图 = 0;
        if (分身.当前管理方法) 分身.继承方法特征位图 |= (1ULL << 0);
        if (私有_推导方法类型(分身.当前管理方法) != 枚举_方法类型::未定义) 分身.继承方法特征位图 |= (1ULL << 1);
        if (上下文.最近反馈类型 != 枚举_任务管理反馈类型::未定义) 分身.继承方法特征位图 |= (1ULL << 2);
    }

    std::string 私有_主体化输入条件主键(
        const 结构_任务管理上下文& 上下文,
        const 结构_任务管理分身继承面& 分身) noexcept
    {
        std::ostringstream 输出;
        输出 << "宿主=" << 私有_任务标题(分身.宿主任务 ? 分身.宿主任务 : 上下文.当前宿主任务)
            << "|方法=" << 私有_方法标题(分身.当前管理方法 ? 分身.当前管理方法 : 上下文.当前方法)
            << "|缺口=" << 私有_缺口文本(上下文.当前缺口类型)
            << "|去向=" << 私有_去向文本(上下文.当前下一步去向)
            << "|域=" << 私有_功能域文本(上下文.最近功能域)
            << "|门控=" << 私有_执行前门控结果文本(上下文.最近执行前门控结果)
            << "|根层=" << 私有_根层重判结果文本(上下文.最近根层重判结果);
        return 输出.str();
    }

    std::string 私有_主体化结果主键(
        const 结构_任务管理分身继承面& 分身,
        const 结构_任务管理上下文& 上下文) noexcept
    {
        if (分身.最近结果) {
            return 分身.最近结果->获取主键();
        }
        if (上下文.最近结果) {
            return 上下文.最近结果->获取主键();
        }
        if (分身.当前步骤) {
            return "步骤:" + 分身.当前步骤->获取主键();
        }
        if (分身.当前管理方法) {
            return "方法:" + 分身.当前管理方法->获取主键();
        }
        return {};
    }

    枚举_任务管理方法需求位 私有_推导任务管理方法需求位(
        const 结构_任务管理上下文& 上下文,
        const 结构_任务管理分身继承面& 分身) noexcept
    {
        const auto* 宿主任务 = 分身.宿主任务 ? 分身.宿主任务 : 上下文.当前宿主任务;
        const auto 宿主状态 = 宿主任务 ? 宿主任务->主信息.状态 : 枚举_任务状态::未定义;
        const bool 宿主已完成 = 宿主状态 == 枚举_任务状态::完成;
        const bool 宿主已失败 = 宿主状态 == 枚举_任务状态::失败 || 宿主状态 == 枚举_任务状态::取消;
        const bool 已满足完成判据 = 宿主已完成 || 私有_任务节点目标结果已对齐(宿主任务);
        const bool 已确定无法继续 =
            宿主已失败
            || 上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::禁止继续
            || 上下文.最近根层重判结果 == 枚举_任务管理根层重判结果::停止退出边界;
        const bool 需要补结构 =
            !宿主任务
            || !分身.当前步骤
            || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::治理任务缺失
            || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::宿主任务缺失
            || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::任务待启动;
        const bool 需要选方法 =
            宿主任务
            && (!分身.当前管理方法
                || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
                || 上下文.当前下一步去向 == 枚举_任务管理下一步去向::绑定方法);
        const bool 需要门控转域 =
            上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::仅允许等待
            || 上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::仅允许收束
            || 上下文.最近根层重判结果 == 枚举_任务管理根层重判结果::待机保持
            || 上下文.当前下一步去向 == 枚举_任务管理下一步去向::回到重筹办
            || 上下文.当前下一步去向 == 枚举_任务管理下一步去向::保持等待;
        const bool 需要收束复盘 =
            上下文.最近根层重判结果 == 枚举_任务管理根层重判结果::收束优先
            || 上下文.当前下一步去向 == 枚举_任务管理下一步去向::进入收束
            || 宿主已完成;

        if (需要收束复盘) {
            return 枚举_任务管理方法需求位::收束复盘;
        }
        if (已确定无法继续) {
            return 宿主已失败
                ? 枚举_任务管理方法需求位::失败策略
                : 枚举_任务管理方法需求位::门控转域;
        }
        if (需要门控转域) {
            return 枚举_任务管理方法需求位::门控转域;
        }
        if (需要补结构) {
            return 枚举_任务管理方法需求位::补结构;
        }
        if (需要选方法) {
            return 枚举_任务管理方法需求位::选择推进;
        }
        if (已满足完成判据) {
            return 枚举_任务管理方法需求位::求证判定;
        }
        if (私有_任务节点目标结果待推进(宿主任务) && !分身.最近结果) {
            return 枚举_任务管理方法需求位::分解补条件;
        }
        if (私有_任务节点目标结果待推进(宿主任务)) {
            return 枚举_任务管理方法需求位::选择推进;
        }
        return 枚举_任务管理方法需求位::求证判定;
    }

    枚举_任务管理步骤位类型 私有_P0_推导步骤位类型(
        const 结构_任务管理上下文& 上下文) noexcept
    {
        if (上下文.当前下一步去向 == 枚举_任务管理下一步去向::保持等待) {
            return 枚举_任务管理步骤位类型::等待位;
        }
        if (上下文.当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
            || 上下文.当前下一步去向 == 枚举_任务管理下一步去向::绑定方法) {
            return 枚举_任务管理步骤位类型::补条件位;
        }
        if (上下文.最近结果) {
            return 枚举_任务管理步骤位类型::回接位;
        }
        return 枚举_任务管理步骤位类型::执行位;
    }

    枚举_任务管理步骤语义类型 私有_P0_推导步骤语义类型(
        const 结构_任务管理上下文& 上下文,
        枚举_任务管理步骤位类型 步骤位类型) noexcept
    {
        switch (步骤位类型) {
        case 枚举_任务管理步骤位类型::等待位:
            return 枚举_任务管理步骤语义类型::等待维持步骤;
        case 枚举_任务管理步骤位类型::回接位:
            return 枚举_任务管理步骤语义类型::回接派生步骤;
        case 枚举_任务管理步骤位类型::补条件位:
            return 枚举_任务管理步骤语义类型::补条件步骤;
        case 枚举_任务管理步骤位类型::执行位:
            return 私有_任务节点目标结果待推进(上下文.当前宿主任务)
                ? 枚举_任务管理步骤语义类型::业务推进步骤
                : 枚举_任务管理步骤语义类型::治理步骤;
        default:
            return 枚举_任务管理步骤语义类型::未定义;
        }
    }

    枚举_任务管理本能能力缺口类型 私有_P0_推导本能能力缺口(
        const 结构_任务管理上下文& 上下文,
        枚举_任务管理方法需求位 需求位) noexcept
    {
        if (!上下文.当前宿主任务) {
            return 枚举_任务管理本能能力缺口类型::缺观察原语;
        }
        if (!上下文.当前方法) {
            return 枚举_任务管理本能能力缺口类型::缺挂点原语;
        }
        if (!上下文.最近结果) {
            return 需求位 == 枚举_任务管理方法需求位::分解补条件
                ? 枚举_任务管理本能能力缺口类型::缺状态差分取证
                : 枚举_任务管理本能能力缺口类型::缺动作边界;
        }
        return 枚举_任务管理本能能力缺口类型::无缺口;
    }

    bool 私有_P0_存在本地上下文(const 结构_任务管理上下文& 上下文) noexcept
    {
        return 上下文.当前时间 != 0
            || 上下文.当前主需求 != nullptr
            || 上下文.当前管理任务 != nullptr
            || 上下文.当前宿主任务 != nullptr
            || 上下文.当前方法 != nullptr
            || 上下文.当前步骤 != nullptr
            || 上下文.最近结果 != nullptr
            || 上下文.当前缺口类型 != 枚举_任务管理缺口类型::未定义
            || 上下文.当前下一步去向 != 枚举_任务管理下一步去向::未定义
            || 上下文.最近根层重判结果 != 枚举_任务管理根层重判结果::未定义
            || 上下文.最近执行前门控结果 != 枚举_任务管理执行前门控结果::未定义
            || !上下文.最近上层反馈摘要.empty()
            || 上下文.最近功能域 != 枚举_任务管理功能域::未定义
            || 上下文.最近总控结果 != 枚举_任务管理总控结果::未定义
            || 上下文.最近反馈类型 != 枚举_任务管理反馈类型::未定义
            || 上下文.最近宿主目标结果比较 != 枚举_任务管理双面比较结果::未定义
            || 上下文.最近后果治理 != 枚举_任务管理后果治理::未定义
            || 上下文.当前主体ID != 0
            || 上下文.当前分身ID != 0
            || 上下文.当前方法需求位 != 枚举_任务管理方法需求位::未定义
            || 上下文.当前本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义
            || !上下文.当前输入条件主键.empty()
            || !上下文.当前治理态型.empty()
            || !上下文.最近反馈摘要.empty();
    }

    结构_任务管理请求 私有_P0_合成阶段请求(
        自我类& 自我对象,
        时间戳 当前时间,
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_任务管理请求 阶段请求{};
        (void)任务管理任务模块::构造任务管理请求(自我对象, 当前时间, &阶段请求);
        私有_P0_覆盖请求字段(阶段请求, 结果.当前请求);
        if (私有_P0_存在本地上下文(结果.上下文)) {
            私有_P0_覆盖上下文字段(阶段请求.上下文, 结果.上下文);
            私有_P0_覆盖非空指针(阶段请求.宿主任务, 结果.上下文.当前宿主任务);
        }
        return 阶段请求;
    }

    结构_任务管理单步决策 私有_P0_合成单步决策(
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_任务管理单步决策 阶段决策{};
        (void)任务管理任务模块::形成任务管理单步决策(结果.当前请求, &阶段决策);
        私有_P0_覆盖决策字段(阶段决策, 结果.当前单步决策);
        私有_P0_覆盖兼容决策字段(阶段决策, 结果);
        阶段决策.决策摘要 = 私有_P0_选用非空文本(
            结果.最近反馈摘要,
            结果.摘要,
            阶段决策.决策摘要);
        return 阶段决策;
    }

    结构_任务管理写回结果 私有_P0_合成写回结果(
        自我类& 自我对象,
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_任务管理写回结果 阶段写回{};
        (void)任务管理任务模块::形成任务管理写回结果(
            自我对象,
            结果.当前请求,
            结果.当前单步决策,
            &阶段写回);
        私有_P0_覆盖写回字段(阶段写回, 结果.当前写回结果);
        私有_P0_覆盖兼容写回字段(阶段写回, 结果);
        return 阶段写回;
    }

    结构_任务管理一步结果 私有_P0_合成一步结果(
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_任务管理一步结果 阶段一步结果{};
        (void)任务管理任务模块::组装任务管理一步结果(
            结果.当前请求,
            结果.当前单步决策,
            结果.当前写回结果,
            &阶段一步结果);
        私有_P0_覆盖一步结果字段(阶段一步结果, 结果.当前一步结果);
        私有_P0_覆盖兼容一步结果字段(阶段一步结果, 结果);
        return 阶段一步结果;
    }

    void 私有_P0_回填兼容字段(结构_任务管理结果& 结果) noexcept
    {
        const auto 阶段当前功能域 = 私有_P0_阶段当前功能域(结果);
        const auto 阶段当前缺口类型 = 私有_P0_阶段当前缺口类型(结果);
        const auto 阶段当前下一步去向 = 私有_P0_阶段当前下一步去向(结果);
        const auto 阶段最近总控结果 = 私有_P0_阶段最近总控结果(结果);
        const auto 阶段最近反馈类型 = 私有_P0_阶段最近反馈类型(结果);

        结果.当前功能域 = 阶段当前功能域;
        结果.当前缺口类型 = 阶段当前缺口类型;
        结果.当前下一步去向 = 阶段当前下一步去向;
        私有_P0_覆盖决策到兼容字段(结果, 结果.当前单步决策);
        私有_P0_覆盖写回到兼容字段(结果, 结果.当前写回结果);
        结果.最近功能域 = 阶段当前功能域;
        结果.最近总控结果 = 阶段最近总控结果;
        结果.最近反馈类型 = 阶段最近反馈类型;
        私有_P0_覆盖一步结果到兼容字段(结果, 结果.当前一步结果);
        私有_P0_覆盖非空文本(结果.最近反馈摘要, 私有_P0_阶段最近反馈摘要(结果));
    }

    void 私有_刷新P0分阶段骨架(
        自我类& 自我对象,
        时间戳 当前时间,
        结构_任务管理结果& 结果) noexcept
    {
        结果.当前请求 = 私有_P0_合成阶段请求(自我对象, 当前时间, 结果);
        结果.上下文 = 结果.当前请求.上下文;
        结果.当前单步决策 = 私有_P0_合成单步决策(结果);
        结果.当前写回结果 = 私有_P0_合成写回结果(自我对象, 结果);
        结果.当前一步结果 = 私有_P0_合成一步结果(结果);
        私有_P0_回填兼容字段(结果);
        私有_同步治理上下文镜像(结果);
    }

    方法节点* 私有_查找本能方法首节点(
        const 自我类& 自我对象,
        枚举_本能方法ID 本能ID) noexcept
    {
        auto* 自我存在 = const_cast<存在节点类*>(自我对象.获取自我存在());
        if (!自我存在) return nullptr;

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        if (!方法根节点) return nullptr;

        return 私有_查找直接子节点_按谓词<方法节点>(方法根节点, [&](const 方法节点* 节点) noexcept {
            if (!节点) return false;
            const auto& 主信息 = 节点->主信息;
            return 主信息.节点种类 == 枚举_方法节点种类::方法首节点
                && 主信息.动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
                && 主信息.动作句柄.本能ID == static_cast<I64>(本能ID);
        });
    }

    bool 私有_任务管理最小原语已正式注册(
        const 自我类& 自我对象,
        枚举_任务管理最小原语ID 原语ID,
        const std::string& 调用点) noexcept
    {
        const auto 本能ID = 私有_最小原语对应本能方法ID(原语ID);
        if (本能ID == 枚举_本能方法ID::未定义) {
            return false;
        }
        if (!本能方法类::查询元信息(本能ID)) {
            return false;
        }

        (void)调用点;
        return 私有_查找本能方法首节点(自我对象, 本能ID) != nullptr;
    }

    bool 私有_任务状态已终结(枚举_任务状态 状态) noexcept;
    const 词性节点类* 私有_补条件子任务类型词() noexcept;
    const 词性节点类* 私有_业务子任务类型词() noexcept;
    const 词性节点类* 私有_学习子任务类型词() noexcept;

    任务节点* 私有_查找直接补条件子任务(const 任务节点* 宿主任务) noexcept
    {
        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(const_cast<任务节点*>(宿主任务), [&](const 任务节点* 节点) noexcept {
            return 节点
                && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                && 私有_词性相同(节点->主信息.类型, 私有_补条件子任务类型词())
                && !私有_任务状态已终结(节点->主信息.状态);
        }));
    }

    任务节点* 私有_查找直接业务子任务(const 任务节点* 宿主任务) noexcept
    {
        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(const_cast<任务节点*>(宿主任务), [&](const 任务节点* 节点) noexcept {
            return 节点
                && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                && 私有_词性相同(节点->主信息.类型, 私有_业务子任务类型词())
                && !私有_任务状态已终结(节点->主信息.状态);
        }));
    }

    任务节点* 私有_查找任务管理学习树根(const 任务节点* 管理任务) noexcept
    {
        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(
            const_cast<任务节点*>(管理任务),
            [&](const 任务节点* 节点) noexcept {
                return 节点
                    && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                    && 私有_词性相同(节点->主信息.类型, 语素集.添加词性词("学习树根节点", "名词"));
            }));
    }

    任务节点* 私有_查找定向学习分支(const 任务节点* 学习树根) noexcept
    {
        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(
            const_cast<任务节点*>(学习树根),
            [&](const 任务节点* 节点) noexcept {
                return 节点
                    && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                    && 私有_词性相同(节点->主信息.类型, 语素集.添加词性词("学习分支", "名词"))
                    && 私有_词性相同(节点->主信息.名称, 语素集.添加词性词("定向学习分支", "专有名词"));
            }));
    }

    任务节点* 私有_查找直接派生子任务(const 任务节点* 宿主任务) noexcept
    {
        if (auto* 业务子任务 = 私有_查找直接业务子任务(宿主任务)) {
            return 业务子任务;
        }
        return 私有_查找直接补条件子任务(宿主任务);
    }

    任务节点* 私有_查找最深派生子任务(任务节点* 起点) noexcept
    {
        auto* 当前 = 起点;
        auto* 命中 = static_cast<任务节点*>(nullptr);
        while (当前) {
            auto* 下级 = 私有_查找直接派生子任务(当前);
            if (!下级) {
                break;
            }
            命中 = 下级;
            当前 = 下级;
        }
        return 命中;
    }

    任务节点* 私有_选择终结后回接宿主(任务节点* 起点) noexcept
    {
        auto* 当前 = 起点;
        while (当前 && 私有_任务状态已终结(当前->主信息.状态)) {
            auto* 父任务 = 当前->主信息.父任务.指针
                ? reinterpret_cast<任务节点*>(当前->主信息.父任务.指针)
                : nullptr;
            if (!父任务) {
                return nullptr;
            }
            if (auto* 子任务 = 私有_查找最深派生子任务(父任务)) {
                return 子任务;
            }
            当前 = 父任务;
        }
        return 当前;
    }

    bool 私有_允许默认本能兜底(const 自我类& 自我对象, const 任务节点* 宿主任务) noexcept
    {
        if (!宿主任务) {
            return false;
        }

        if (宿主任务 == 自我对象.获取安全根任务()
            || 宿主任务 == 自我对象.获取服务根任务()) {
            return true;
        }

        return 私有_任务标题(宿主任务) == "任务管理任务";
    }

    任务节点* 私有_展开锚点宿主候选(任务节点* 根任务) noexcept
    {
        if (!根任务) {
            return nullptr;
        }
        if (auto* 子任务 = 私有_查找最深派生子任务(根任务)) {
            return 子任务;
        }
        return 根任务;
    }

    任务节点* 私有_选择锚点驱动根任务(
        const 自我类& 自我对象,
        const 结构_任务管理上下文& 上下文,
        std::string* 锚点裁决摘要) noexcept
    {
        const auto 写摘要 = [&](const std::string& 文本) {
            if (锚点裁决摘要) {
                *锚点裁决摘要 = 文本;
            }
        };

        auto* 当前主需求 = 上下文.当前主需求;
        if (当前主需求 && 当前主需求 == 自我对象.获取安全根需求()) {
            auto* 根任务 = 自我对象.获取安全根任务();
            写摘要(
                "锚点=安全根需求 | 根任务=" + 私有_任务标题(根任务)
                + " | 宿主候选=" + 私有_任务标题(私有_展开锚点宿主候选(根任务)));
            return 根任务;
        }
        if (当前主需求 && 当前主需求 == 自我对象.获取服务根需求()) {
            auto* 根任务 = 自我对象.获取服务根任务();
            写摘要(
                "锚点=服务根需求 | 根任务=" + 私有_任务标题(根任务)
                + " | 宿主候选=" + 私有_任务标题(私有_展开锚点宿主候选(根任务)));
            return 根任务;
        }

        if (auto* 服务根任务 = 自我对象.获取服务根任务()) {
            写摘要(
                "锚点=主需求未显式命中 | 回退根任务=" + 私有_任务标题(服务根任务)
                + " | 宿主候选=" + 私有_任务标题(私有_展开锚点宿主候选(服务根任务)));
            return 服务根任务;
        }
        if (auto* 安全根任务 = 自我对象.获取安全根任务()) {
            写摘要(
                "锚点=主需求未显式命中 | 回退根任务=" + 私有_任务标题(安全根任务)
                + " | 宿主候选=" + 私有_任务标题(私有_展开锚点宿主候选(安全根任务)));
            return 安全根任务;
        }

        写摘要("锚点=空 | 根任务=空");
        return nullptr;
    }

    任务节点* 私有_选择宿主任务(
        const 自我类& 自我对象,
        const 结构_任务管理上下文& 上下文,
        std::string* 锚点裁决摘要) noexcept
    {
        std::string 锚点摘要{};
        auto* 锚点根任务 = 私有_选择锚点驱动根任务(自我对象, 上下文, &锚点摘要);
        auto* 锚点宿主候选 = 私有_展开锚点宿主候选(锚点根任务);

        if (上下文.当前宿主任务 && 上下文.当前宿主任务 != 上下文.当前管理任务) {
            auto* 当前候选 = 上下文.当前宿主任务;
            if (私有_任务状态已终结(上下文.当前宿主任务->主信息.状态)) {
                if (auto* 回接宿主 = 私有_选择终结后回接宿主(上下文.当前宿主任务)) {
                    当前候选 = 回接宿主;
                }
                else {
                    当前候选 = nullptr;
                }
            }
            else if (auto* 子任务 = 私有_查找最深派生子任务(上下文.当前宿主任务)) {
                当前候选 = 子任务;
            }

            const bool 当前候选仍在锚点分支 =
                !锚点根任务
                || !当前候选
                || 当前候选 == 锚点根任务
                || 私有_是祖先节点(锚点根任务, 当前候选);
            if (锚点宿主候选 && !当前候选仍在锚点分支) {
                if (锚点裁决摘要) {
                    *锚点裁决摘要 = 锚点摘要
                        + " | 当前宿主已偏离锚点分支，切换到=" + 私有_任务标题(锚点宿主候选);
                }
                return 锚点宿主候选;
            }
            if (当前候选) {
                if (锚点裁决摘要) {
                    *锚点裁决摘要 = 锚点摘要.empty()
                        ? "锚点=空 | 沿用当前宿主=" + 私有_任务标题(当前候选)
                        : (锚点摘要 + " | 当前宿主仍在锚点分支，沿用=" + 私有_任务标题(当前候选));
                }
                return 当前候选;
            }
        }

        if (锚点宿主候选) {
            if (锚点裁决摘要) {
                *锚点裁决摘要 = 锚点摘要 + " | 按锚点选中宿主=" + 私有_任务标题(锚点宿主候选);
            }
            return 锚点宿主候选;
        }

        if (锚点裁决摘要) {
            *锚点裁决摘要 = 锚点摘要.empty() ? "锚点=空 | 未选中宿主任务" : (锚点摘要 + " | 未选中宿主任务");
        }
        return nullptr;
    }

    枚举_本能方法ID 私有_选择本能方法ID(const 自我类& 自我对象, 任务节点* 宿主任务) noexcept
    {
        if (!宿主任务) {
            return 枚举_本能方法ID::未定义;
        }

        if (宿主任务 == 自我对象.获取安全根任务()) {
            return 枚举_本能方法ID::自我_停止当前试探并进入保守态;
        }

        if (宿主任务 == 自我对象.获取服务根任务()) {
            return 枚举_本能方法ID::自我_读取核心状态;
        }

        const auto 标题 = 私有_任务标题(宿主任务);
        if (标题 == "任务管理任务") {
            return 枚举_本能方法ID::自我_读取核心状态;
        }

        return 枚举_本能方法ID::自我_读取核心状态;
    }

    bool 私有_任务节点归属宿主任务(const 任务节点* 节点, const 任务节点* 宿主任务) noexcept;

    bool 私有_方法属于宿主任务(const 方法节点* 方法, const 任务节点* 宿主任务) noexcept
    {
        if (!方法 || !宿主任务) {
            return false;
        }
        const auto* 来源任务 = 方法->主信息.来源任务.指针
            ? reinterpret_cast<const 任务节点*>(方法->主信息.来源任务.指针)
            : nullptr;
        return 来源任务 == 宿主任务;
    }

    bool 私有_方法条件筛选通过(
        const 方法节点* 方法,
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果,
        bool& 使用了条件节点) noexcept
    {
        使用了条件节点 = false;
        if (!方法 || !宿主任务 || !方法->子) {
            return true;
        }

        const 状态节点类* 候选状态[] = {
            结果.最新状态,
            结果.最新目标状态,
            宿主任务->主信息.结果状态信息.指针,
            宿主任务->主信息.目标状态.指针,
        };

        bool 有条件节点 = false;
        auto* 首节点 = static_cast<const 方法节点*>(方法->子);
        auto* 当前 = 首节点;
        do {
            if (!当前 || 当前->主信息.节点种类 != 枚举_方法节点种类::方法条件节点) {
                当前 = 当前 ? static_cast<const 方法节点*>(当前->下) : nullptr;
                continue;
            }

            有条件节点 = true;
            使用了条件节点 = true;
            const auto* 期望状态 = 当前->主信息.期望结果状态.指针;
            if (!期望状态) {
                return true;
            }

            for (const auto* 状态节点 : 候选状态) {
                if (状态节点 && 状态节点 == 期望状态) {
                    return true;
                }
            }

            当前 = static_cast<const 方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return !有条件节点;
    }

    方法节点* 私有_从最近结果回看方法(
        const 结构_任务管理结果& 结果,
        const 任务节点* 宿主任务) noexcept
    {
        const auto* 最近结果 = 结果.上下文.最近结果;
        if (!最近结果
            || 最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
            || !私有_任务节点归属宿主任务(最近结果, 宿主任务)) {
            return nullptr;
        }

        return 最近结果->主信息.执行方法.指针
            ? reinterpret_cast<方法节点*>(最近结果->主信息.执行方法.指针)
            : nullptr;
    }

    方法节点* 私有_查找宿主任务方法首节点候选(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果,
        bool& 使用了条件节点) noexcept
    {
        使用了条件节点 = false;
        auto* 自我存在 = const_cast<存在节点类*>(自我对象.获取自我存在());
        if (!自我存在 || !宿主任务) {
            return nullptr;
        }

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        if (!方法根节点 || !方法根节点->子) {
            return nullptr;
        }

        auto* 首节点 = static_cast<方法节点*>(方法根节点->子);
        auto* 当前 = 首节点;
        do {
            if (!当前 || 当前->主信息.节点种类 != 枚举_方法节点种类::方法首节点) {
                当前 = 当前 ? static_cast<方法节点*>(当前->下) : nullptr;
                continue;
            }
            if (!私有_方法属于宿主任务(当前, 宿主任务)) {
                当前 = static_cast<方法节点*>(当前->下);
                continue;
            }

            bool 当前方法命中条件 = false;
            if (私有_方法条件筛选通过(当前, 宿主任务, 结果, 当前方法命中条件)) {
                使用了条件节点 = 当前方法命中条件;
                return 当前;
            }

            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    方法节点* 私有_查找主体长期策略方法候选(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        const auto& 主体 = 结果.当前主体;
        if (!宿主任务
            || 主体.累计正式方法资产提交次数 == 0
            || 主体.最近正式方法标题.empty()
            || !私有_M10_长期策略可靠度允许回填(主体)) {
            return nullptr;
        }

        auto* 自我存在 = const_cast<存在节点类*>(自我对象.获取自我存在());
        if (!自我存在) {
            return nullptr;
        }

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        if (!方法根节点 || !方法根节点->子) {
            return nullptr;
        }

        auto* 首节点 = static_cast<方法节点*>(方法根节点->子);
        auto* 当前 = 首节点;
        do {
            if (!当前 || 当前->主信息.节点种类 != 枚举_方法节点种类::方法首节点) {
                当前 = 当前 ? static_cast<方法节点*>(当前->下) : nullptr;
                continue;
            }
            if (私有_方法标题(当前) != 主体.最近正式方法标题) {
                当前 = static_cast<方法节点*>(当前->下);
                continue;
            }

            bool 当前方法命中条件 = false;
            if (私有_方法条件筛选通过(当前, 宿主任务, 结果, 当前方法命中条件)) {
                return 当前;
            }

            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    bool 私有_M10_主体长期策略允许直放后段(const 结构_任务管理结果& 结果) noexcept
    {
        auto* 宿主任务 = 私有_P0_当前宿主任务(结果);
        const auto 当前本能缺口 = 私有_P0_当前本能缺口(结果);
        return 宿主任务
            && 私有_M10_当前方法命中主体长期策略(结果)
            && 当前本能缺口 == 枚举_任务管理本能能力缺口类型::需学习补齐
            && 私有_M10_长期策略可靠度允许回填(结果.当前主体)
            && 结果.当前主体.累计正式方法资产提交次数 > 0
            && !结果.当前主体.最近正式方法标题.empty()
            && 私有_M10_存在业务推进压力(结果);
    }

    void 私有_完成方法挂接(
        自我类& 自我对象,
        任务节点* 宿主任务,
        方法节点* 方法,
        时间戳 now,
        const std::string& 调用点,
        枚举_任务管理方法来源 来源,
        结构_任务管理结果& 结果,
        方法节点*& 输出方法) noexcept
    {
        输出方法 = 方法;
        结果.当前方法来源 = 来源;
        if (!宿主任务 || !方法) {
            return;
        }

        auto& 任务主信息 = 宿主任务->主信息;
        私有_写入引用(任务主信息.执行方法, 方法);
        任务主信息.动作句柄 = 方法->主信息.动作句柄;
        if (!方法->主信息.来源任务.指针) {
            私有_写入引用(方法->主信息.来源任务, 宿主任务);
        }
        (void)自我对象.设置当前主方法(方法, now, 调用点 + "/设置当前主方法");
    }

    bool 私有_迁移任务状态(任务节点* 节点, 枚举_任务状态 新状态, 时间戳 now) noexcept
    {
        if (!节点) return false;

        auto& 主信息 = 节点->主信息;
        if (主信息.状态 == 新状态) {
            return false;
        }

        if ((新状态 == 枚举_任务状态::运行中) && 主信息.启动时间 == 0) {
            主信息.启动时间 = now;
        }
        if ((新状态 == 枚举_任务状态::完成
            || 新状态 == 枚举_任务状态::失败
            || 新状态 == 枚举_任务状态::取消)
            && 主信息.完成时间 == 0) {
            主信息.完成时间 = now;
        }

        主信息.状态 = 新状态;
        return true;
    }

    状态节点类* 私有_创建状态节点(
        场景节点类* 场景,
        基础信息节点类* 主体,
        特征节点类* 特征,
        const 特征值& 值,
        时间戳 now,
        bool 是否变化,
        const std::string& 原因类别,
        const std::string& 原因说明) noexcept
    {
        auto* 状态节点 = 世界树.状态().创建状态(场景, 主体, 特征, 值, now, now);
        if (!状态节点) return nullptr;

        auto* 主信息 = 世界树.状态().取状态主信息(状态节点);
        if (主信息) {
            const auto* 特征类型词 = (特征 && 特征->主信息) ? 特征->主信息->类型 : nullptr;
            主信息->名称 = 特征类型词;
            主信息->类型 = 特征类型词;
        }
        (void)世界树.状态().标记变化(状态节点, 是否变化, 原因类别, 原因说明);
        return 状态节点;
    }

    bool 私有_记录宿主任务治理证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::宿主任务治理);
        auto* 宿主任务 = 来源任务 ? 来源任务 : 自我对象.获取当前主任务();
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(
            自我对象,
            宿主任务,
            调用点 + "/任务虚拟存在");
        auto* 场景 = 私有_任务场景(自我对象, 宿主任务);
        auto* 特征节点 = 私有_确保宿主任务治理特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/特征");
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        if (!宿主任务 || !任务虚拟存在 || !场景 || !宿主基础节点 || !特征节点) {
            return false;
        }

        I64 旧值 = 0;
        const bool 有旧值 = 世界树.读取特征_I64(特征节点, 旧值);
        if (有旧值 && 旧值 == 新值) {
            return false;
        }

        if (!世界树.写入特征_I64(特征节点, 新值, now)) {
            return false;
        }

        状态节点类* 条件状态 = nullptr;
        if (有旧值) {
            条件状态 = 私有_创建状态节点(
                场景,
                宿主基础节点,
                特征节点,
                特征值{ 旧值 },
                now,
                false,
                动作语义键,
                "治理前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ 新值 },
            now,
            true,
            动作语义键,
            "治理后镜像");
        if (!结果状态) {
            return false;
        }

        auto* 动态节点 = 世界树.动态().创建动态(场景, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 任务虚拟存在, 特征节点);
            if (来源方法) {
                (void)世界树.动态().绑定来源方法(
                    动态节点,
                    reinterpret_cast<方法节点类*>(来源方法),
                    来源方法->主信息.动作名,
                    枚举_动作事件相位::完成);
            }
            (void)世界树.动态().绑定来源场景(动态节点, 场景, 场景);
        }

        auto* 因果主信息 = new 因果实例主信息类{};
        因果主信息->名称 = 特征类型;
        因果主信息->类型 = 特征类型;
        因果主信息->锚点类型 = 枚举_因果锚点类型::存在;
        私有_写入引用(因果主信息->锚点信息, 任务虚拟存在);
        因果主信息->动作时间 = now;
        因果主信息->结果时间 = now;
        因果主信息->条件时间段 = 结构_时间段{ now, now };
        因果主信息->来源类型 = 枚举_因果来源类型::执行生成;
        因果主信息->来源任务主键 = 私有_节点主键摘要(宿主任务);
        因果主信息->来源方法主键 = 私有_节点主键摘要(来源方法);
        因果主信息->置信度 = 10000;
        因果主信息->动作语义键 = 动作语义键;

        auto* 因果节点 = 世界树.创建因果实例(场景, 因果主信息);
        if (因果节点) {
            (void)世界树.因果().绑定动作主体语义(因果节点, 任务虚拟存在, 特征节点, 动作语义键);
            const bool 已从动态回带主结果 =
                世界树.因果().按动态桥接结果状态(因果节点, 动态节点, true);
            if (条件状态) {
                (void)世界树.因果().追加条件状态(因果节点, 条件状态);
            }
            (void)世界树.因果().追加结果状态(因果节点, 结果状态, !已从动态回带主结果);
        }

        结果.已生成治理证据 = true;
        结果.最新状态 = 结果状态;
        结果.最新动态 = 动态节点;
        结果.最新因果 = 因果节点;
        return true;
    }

    bool 私有_记录方法资产证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::方法资产);
        auto* 方法头 = 来源方法;
        auto* 方法虚拟存在 = 方法头 ? 方法头->主信息.方法虚拟存在.指针 : nullptr;
        auto* 场景 = 来源任务 ? 私有_任务场景(自我对象, 来源任务) : nullptr;
        if (!场景) {
            场景 = 自我对象.确保自我内部世界(调用点 + "/自我内部世界");
        }
        auto* 特征节点 = 私有_确保方法资产特征节点(
            方法头,
            特征类型,
            调用点 + "/特征");
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(方法虚拟存在);
        if (!方法头 || !方法虚拟存在 || !场景 || !宿主基础节点 || !特征节点) {
            return false;
        }

        I64 旧值 = 0;
        const bool 有旧值 = 世界树.读取特征_I64(特征节点, 旧值);
        if (有旧值 && 旧值 == 新值) {
            return false;
        }

        if (!世界树.写入特征_I64(特征节点, 新值, now)) {
            return false;
        }

        状态节点类* 条件状态 = nullptr;
        if (有旧值) {
            条件状态 = 私有_创建状态节点(
                场景,
                宿主基础节点,
                特征节点,
                特征值{ 旧值 },
                now,
                false,
                动作语义键,
                "治理前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ 新值 },
            now,
            true,
            动作语义键,
            "治理后镜像");
        if (!结果状态) {
            return false;
        }

        auto* 动态节点 = 世界树.动态().创建动态(场景, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 方法虚拟存在, 特征节点);
            if (来源方法) {
                (void)世界树.动态().绑定来源方法(
                    动态节点,
                    reinterpret_cast<方法节点类*>(来源方法),
                    来源方法->主信息.动作名,
                    枚举_动作事件相位::完成);
            }
            (void)世界树.动态().绑定来源场景(动态节点, 场景, 场景);
        }

        auto* 因果主信息 = new 因果实例主信息类{};
        因果主信息->名称 = 特征类型;
        因果主信息->类型 = 特征类型;
        因果主信息->锚点类型 = 枚举_因果锚点类型::存在;
        私有_写入引用(因果主信息->锚点信息, 方法虚拟存在);
        因果主信息->动作时间 = now;
        因果主信息->结果时间 = now;
        因果主信息->条件时间段 = 结构_时间段{ now, now };
        因果主信息->来源类型 = 枚举_因果来源类型::执行生成;
        因果主信息->来源任务主键 = 私有_节点主键摘要(来源任务);
        因果主信息->来源方法主键 = 私有_节点主键摘要(来源方法);
        因果主信息->置信度 = 10000;
        因果主信息->动作语义键 = 动作语义键;

        auto* 因果节点 = 世界树.创建因果实例(场景, 因果主信息);
        if (因果节点) {
            (void)世界树.因果().绑定动作主体语义(因果节点, 方法虚拟存在, 特征节点, 动作语义键);
            const bool 已从动态回带主结果 =
                世界树.因果().按动态桥接结果状态(因果节点, 动态节点, true);
            if (条件状态) {
                (void)世界树.因果().追加条件状态(因果节点, 条件状态);
            }
            (void)世界树.因果().追加结果状态(因果节点, 结果状态, !已从动态回带主结果);
        }

        结果.已生成治理证据 = true;
        结果.最新状态 = 结果状态;
        结果.最新动态 = 动态节点;
        结果.最新因果 = 因果节点;
        return true;
    }

    bool 私有_记录宿主任务治理证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖,
        枚举_动作事件相位 来源相位) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::宿主任务治理);
        auto* 宿主任务 = 来源任务 ? 来源任务 : 自我对象.获取当前主任务();
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(
            自我对象,
            宿主任务,
            调用点 + "/任务虚拟存在");
        auto* 场景 = 私有_任务场景(自我对象, 宿主任务);
        auto* 特征节点 = 私有_确保宿主任务治理特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/特征");
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        if (!宿主任务 || !任务虚拟存在 || !场景 || !宿主基础节点 || !特征节点) {
            return false;
        }

        std::uintptr_t 旧值 = 0;
        const bool 有旧值 = 私有_读取指针特征(特征节点, 旧值);
        const std::uintptr_t 新值 = reinterpret_cast<std::uintptr_t>(新指针值);
        if (有旧值 && 旧值 == 新值) {
            return false;
        }

        if (!世界树.写入特征_指针(特征节点, 新指针值, now)) {
            return false;
        }

        状态节点类* 条件状态 = nullptr;
        if (有旧值) {
            条件状态 = 私有_创建状态节点(
                场景,
                宿主基础节点,
                特征节点,
                特征值{ 指针句柄{ 旧值 } },
                now,
                false,
                动作语义键,
                "治理前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ 指针句柄{ 新值 } },
            now,
            true,
            动作语义键,
            "治理后镜像");
        if (!结果状态) {
            return false;
        }
        if (!新值标题.empty()) {
            (void)世界树.状态().标记变化(结果状态, true, 动作语义键, 新值标题);
        }

        auto* 动态节点 = 世界树.动态().创建动态(场景, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 任务虚拟存在, 特征节点);
            if (来源方法 || 来源动作名覆盖) {
                const auto* 来源动作名 = 来源动作名覆盖
                    ? 来源动作名覆盖
                    : 来源方法->主信息.动作名;
                (void)世界树.动态().绑定来源方法(
                    动态节点,
                    reinterpret_cast<方法节点类*>(来源方法),
                    来源动作名,
                    来源相位);
            }
            (void)世界树.动态().绑定来源场景(动态节点, 场景, 场景);
        }

        auto* 因果主信息 = new 因果实例主信息类{};
        因果主信息->名称 = 特征类型;
        因果主信息->类型 = 特征类型;
        因果主信息->锚点类型 = 枚举_因果锚点类型::存在;
        私有_写入引用(因果主信息->锚点信息, 任务虚拟存在);
        因果主信息->动作时间 = now;
        因果主信息->结果时间 = now;
        因果主信息->条件时间段 = 结构_时间段{ now, now };
        因果主信息->来源类型 = 枚举_因果来源类型::执行生成;
        因果主信息->来源任务主键 = 私有_节点主键摘要(宿主任务);
        因果主信息->来源方法主键 = 私有_节点主键摘要(来源方法);
        因果主信息->置信度 = 10000;
        因果主信息->动作语义键 = 动作语义键;

        auto* 因果节点 = 世界树.创建因果实例(场景, 因果主信息);
        if (因果节点) {
            (void)世界树.因果().绑定动作主体语义(因果节点, 任务虚拟存在, 特征节点, 动作语义键);
            const bool 已从动态回带主结果 =
                世界树.因果().按动态桥接结果状态(因果节点, 动态节点, true);
            if (条件状态) {
                (void)世界树.因果().追加条件状态(因果节点, 条件状态);
            }
            (void)世界树.因果().追加结果状态(因果节点, 结果状态, !已从动态回带主结果);
        }

        结果.已生成治理证据 = true;
        结果.最新状态 = 结果状态;
        结果.最新动态 = 动态节点;
        结果.最新因果 = 因果节点;
        return true;
    }

    bool 私有_记录主体桥接证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::主体桥接);
        auto* 宿主任务 = 来源任务 ? 来源任务 : 自我对象.获取当前主任务();
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(
            自我对象,
            宿主任务,
            调用点 + "/任务虚拟存在");
        auto* 场景 = 私有_任务场景(自我对象, 宿主任务);
        auto* 特征节点 = 私有_确保主体桥接特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/特征");
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        if (!宿主任务 || !任务虚拟存在 || !场景 || !宿主基础节点 || !特征节点) {
            return false;
        }

        I64 旧值 = 0;
        const bool 有旧值 = 世界树.读取特征_I64(特征节点, 旧值);
        if (有旧值 && 旧值 == 新值) {
            return false;
        }

        if (!世界树.写入特征_I64(特征节点, 新值, now)) {
            return false;
        }

        状态节点类* 条件状态 = nullptr;
        if (有旧值) {
            条件状态 = 私有_创建状态节点(
                场景,
                宿主基础节点,
                特征节点,
                特征值{ 旧值 },
                now,
                false,
                动作语义键,
                "治理前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ 新值 },
            now,
            true,
            动作语义键,
            "治理后镜像");
        if (!结果状态) {
            return false;
        }

        auto* 动态节点 = 世界树.动态().创建动态(场景, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 任务虚拟存在, 特征节点);
            if (来源方法) {
                (void)世界树.动态().绑定来源方法(
                    动态节点,
                    reinterpret_cast<方法节点类*>(来源方法),
                    来源方法->主信息.动作名,
                    枚举_动作事件相位::完成);
            }
            (void)世界树.动态().绑定来源场景(动态节点, 场景, 场景);
        }

        auto* 因果主信息 = new 因果实例主信息类{};
        因果主信息->名称 = 特征类型;
        因果主信息->类型 = 特征类型;
        因果主信息->锚点类型 = 枚举_因果锚点类型::存在;
        私有_写入引用(因果主信息->锚点信息, 任务虚拟存在);
        因果主信息->动作时间 = now;
        因果主信息->结果时间 = now;
        因果主信息->条件时间段 = 结构_时间段{ now, now };
        因果主信息->来源类型 = 枚举_因果来源类型::执行生成;
        因果主信息->来源任务主键 = 私有_节点主键摘要(宿主任务);
        因果主信息->来源方法主键 = 私有_节点主键摘要(来源方法);
        因果主信息->置信度 = 10000;
        因果主信息->动作语义键 = 动作语义键;

        auto* 因果节点 = 世界树.创建因果实例(场景, 因果主信息);
        if (因果节点) {
            (void)世界树.因果().绑定动作主体语义(因果节点, 任务虚拟存在, 特征节点, 动作语义键);
            const bool 已从动态回带主结果 =
                世界树.因果().按动态桥接结果状态(因果节点, 动态节点, true);
            if (条件状态) {
                (void)世界树.因果().追加条件状态(因果节点, 条件状态);
            }
            (void)世界树.因果().追加结果状态(因果节点, 结果状态, !已从动态回带主结果);
        }

        结果.已生成治理证据 = true;
        结果.最新状态 = 结果状态;
        结果.最新动态 = 动态节点;
        结果.最新因果 = 因果节点;
        return true;
    }

    bool 私有_记录主体桥接证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖,
        枚举_动作事件相位 来源相位) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::主体桥接);
        auto* 宿主任务 = 来源任务 ? 来源任务 : 自我对象.获取当前主任务();
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(
            自我对象,
            宿主任务,
            调用点 + "/任务虚拟存在");
        auto* 场景 = 私有_任务场景(自我对象, 宿主任务);
        auto* 特征节点 = 私有_确保主体桥接特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/特征");
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        if (!宿主任务 || !任务虚拟存在 || !场景 || !宿主基础节点 || !特征节点) {
            return false;
        }

        std::uintptr_t 旧值 = 0;
        const bool 有旧值 = 私有_读取指针特征(特征节点, 旧值);
        const std::uintptr_t 新值 = reinterpret_cast<std::uintptr_t>(新指针值);
        if (有旧值 && 旧值 == 新值) {
            return false;
        }

        if (!世界树.写入特征_指针(特征节点, 新指针值, now)) {
            return false;
        }

        状态节点类* 条件状态 = nullptr;
        if (有旧值) {
            条件状态 = 私有_创建状态节点(
                场景,
                宿主基础节点,
                特征节点,
                特征值{ 指针句柄{ 旧值 } },
                now,
                false,
                动作语义键,
                "治理前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ 指针句柄{ 新值 } },
            now,
            true,
            动作语义键,
            "治理后镜像");
        if (!结果状态) {
            return false;
        }
        if (!新值标题.empty()) {
            (void)世界树.状态().标记变化(结果状态, true, 动作语义键, 新值标题);
        }

        auto* 动态节点 = 世界树.动态().创建动态(场景, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 任务虚拟存在, 特征节点);
            if (来源方法 || 来源动作名覆盖) {
                const auto* 来源动作名 = 来源动作名覆盖
                    ? 来源动作名覆盖
                    : 来源方法->主信息.动作名;
                (void)世界树.动态().绑定来源方法(
                    动态节点,
                    reinterpret_cast<方法节点类*>(来源方法),
                    来源动作名,
                    来源相位);
            }
            (void)世界树.动态().绑定来源场景(动态节点, 场景, 场景);
        }

        auto* 因果主信息 = new 因果实例主信息类{};
        因果主信息->名称 = 特征类型;
        因果主信息->类型 = 特征类型;
        因果主信息->锚点类型 = 枚举_因果锚点类型::存在;
        私有_写入引用(因果主信息->锚点信息, 任务虚拟存在);
        因果主信息->动作时间 = now;
        因果主信息->结果时间 = now;
        因果主信息->条件时间段 = 结构_时间段{ now, now };
        因果主信息->来源类型 = 枚举_因果来源类型::执行生成;
        因果主信息->来源任务主键 = 私有_节点主键摘要(宿主任务);
        因果主信息->来源方法主键 = 私有_节点主键摘要(来源方法);
        因果主信息->置信度 = 10000;
        因果主信息->动作语义键 = 动作语义键;

        auto* 因果节点 = 世界树.创建因果实例(场景, 因果主信息);
        if (因果节点) {
            (void)世界树.因果().绑定动作主体语义(因果节点, 任务虚拟存在, 特征节点, 动作语义键);
            const bool 已从动态回带主结果 =
                世界树.因果().按动态桥接结果状态(因果节点, 动态节点, true);
            if (条件状态) {
                (void)世界树.因果().追加条件状态(因果节点, 条件状态);
            }
            (void)世界树.因果().追加结果状态(因果节点, 结果状态, !已从动态回带主结果);
        }

        结果.已生成治理证据 = true;
        结果.最新状态 = 结果状态;
        结果.最新动态 = 动态节点;
        结果.最新因果 = 因果节点;
        return true;
    }

    bool 私有_按语义宿主分派记录治理证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        switch (私有_查询任务管理特征目标宿主(特征类型)) {
        case 枚举_任务管理特征目标宿主::宿主任务治理:
            return 私有_记录宿主任务治理证据_I64(
                自我对象, 特征类型, 新值, 动作语义键, 来源方法, 来源任务, now, 调用点, 结果);
        case 枚举_任务管理特征目标宿主::主体桥接:
            return 私有_记录主体桥接证据_I64(
                自我对象, 特征类型, 新值, 动作语义键, 来源方法, 来源任务, now, 调用点, 结果);
        case 枚举_任务管理特征目标宿主::方法资产:
            return 私有_记录方法资产证据_I64(
                自我对象, 特征类型, 新值, 动作语义键, 来源方法, 来源任务, now, 调用点, 结果);
        default:
            return false;
        }
    }

    bool 私有_按语义宿主分派记录治理证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖,
        枚举_动作事件相位 来源相位) noexcept
    {
        switch (私有_查询任务管理特征目标宿主(特征类型)) {
        case 枚举_任务管理特征目标宿主::宿主任务治理:
            return 私有_记录宿主任务治理证据_指针(
                自我对象,
                特征类型,
                新指针值,
                新值标题,
                动作语义键,
                来源方法,
                来源任务,
                now,
                调用点,
                结果,
                来源动作名覆盖,
                来源相位);
        case 枚举_任务管理特征目标宿主::主体桥接:
            return 私有_记录主体桥接证据_指针(
                自我对象,
                特征类型,
                新指针值,
                新值标题,
                动作语义键,
                来源方法,
                来源任务,
                now,
                调用点,
                结果,
                来源动作名覆盖,
                来源相位);
        default:
            return false;
        }
    }

    bool 私有_记录任务管理方法位专项动态(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        枚举_动作事件相位 相位) noexcept
    {
        const auto 方法位标题 = 私有_任务管理方法位标题(结果);
        if (方法位标题.empty()) {
            return false;
        }

        const std::string 事件标题 =
            方法位标题 + (相位 == 枚举_动作事件相位::开始运行 ? "开始" : "完成");
        const auto* 事件词 = 语素集.添加词性词(事件标题, "短语");
        if (!事件词) {
            return false;
        }

        auto* 原最新状态 = 结果.最新状态;
        auto* 原最新动态 = 结果.最新动态;
        auto* 原最新因果 = 结果.最新因果;
        const bool 原已生成治理证据 = 结果.已生成治理证据;
        auto* 当前方法 = 私有_P0_当前方法(结果);

        const bool 已记录 = 私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前方法位专项动态(),
            事件词,
            事件标题,
            "任务管理_方法位专项动态",
            当前方法,
            宿主任务,
            now,
            调用点,
            结果,
            事件词,
            相位);
        if (!已记录) {
            return false;
        }

        结果.当前需求位特征.当前方法位专项动态 = 结果.最新动态;
        结果.当前需求位特征.当前方法位专项动态标题 = 事件标题;
        私有_P0_刷新主体桥接字段(结果);
        结果.已生成治理证据 = 原已生成治理证据 || 已记录;
        结果.最新状态 = 原最新状态;
        结果.最新动态 = 原最新动态;
        结果.最新因果 = 原最新因果;
        return true;
    }

    bool 私有_记录最小原语边界动态(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        枚举_任务管理最小原语ID 原语ID,
        枚举_动作事件相位 相位) noexcept
    {
        const auto* 签名 = 私有_查找最小原语签名(原语ID);
        if (!签名) {
            return false;
        }

        const std::string 事件标题 = std::string("原语_")
            + 签名->原语名
            + (相位 == 枚举_动作事件相位::开始运行 ? "_开始" : "_结束");
        const auto* 事件词 = 语素集.添加词性词(事件标题, "短语");
        if (!事件词) {
            return false;
        }

        auto* 原最新状态 = 结果.最新状态;
        auto* 原最新动态 = 结果.最新动态;
        auto* 原最新因果 = 结果.最新因果;
        const bool 原已生成治理证据 = 结果.已生成治理证据;
        auto* 当前方法 = 私有_P0_当前方法(结果);

        const bool 已记录 = 私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前最小原语动态(),
            事件词,
            事件标题,
            "任务管理_最小原语动态",
            当前方法,
            宿主任务,
            now,
            调用点,
            结果,
            事件词,
            相位);
        if (!已记录) {
            return false;
        }

        结果.当前本能能力缺口.当前最小原语动态 = 结果.最新动态;
        结果.当前本能能力缺口.当前最小原语动态标题 = 事件标题;
        私有_P0_刷新主体桥接字段(结果);
        结果.已生成治理证据 = 原已生成治理证据 || 已记录;
        结果.最新状态 = 原最新状态;
        结果.最新动态 = 原最新动态;
        结果.最新因果 = 原最新因果;
        return true;
    }

    bool 私有_执行最小原语边界记录体(void* 上下文) noexcept
    {
        auto* 执行上下文 = static_cast<结构_最小原语边界执行上下文*>(上下文);
        if (!执行上下文
            || !执行上下文->自我对象
            || !执行上下文->调用点
            || !执行上下文->结果) {
            return false;
        }

        return 私有_记录最小原语边界动态(
            *执行上下文->自我对象,
            执行上下文->宿主任务,
            执行上下文->当前时间,
            *执行上下文->调用点,
            *执行上下文->结果,
            执行上下文->原语ID,
            执行上下文->相位);
    }

    bool 私有_执行最小原语I64写回体(void* 上下文) noexcept
    {
        auto* 执行上下文 = static_cast<结构_最小原语I64写回执行上下文*>(上下文);
        if (!执行上下文
            || !执行上下文->自我对象
            || !执行上下文->动作语义键
            || !执行上下文->调用点
            || !执行上下文->结果) {
            return false;
        }

        switch (执行上下文->目标宿主) {
        case 枚举_最小原语写回目标宿主::宿主任务治理:
            return 私有_记录宿主任务治理证据_I64(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新值,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果);
        case 枚举_最小原语写回目标宿主::主体桥接:
            return 私有_记录主体桥接证据_I64(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新值,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果);
        case 枚举_最小原语写回目标宿主::方法资产:
            return 私有_记录方法资产证据_I64(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新值,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果);
        case 枚举_最小原语写回目标宿主::按语义宿主:
        default:
            return 私有_按语义宿主分派记录治理证据_I64(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新值,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果);
        }
    }

    bool 私有_执行最小原语指针写回体(void* 上下文) noexcept
    {
        auto* 执行上下文 = static_cast<结构_最小原语指针写回执行上下文*>(上下文);
        if (!执行上下文
            || !执行上下文->自我对象
            || !执行上下文->新值标题
            || !执行上下文->动作语义键
            || !执行上下文->调用点
            || !执行上下文->结果) {
            return false;
        }

        switch (执行上下文->目标宿主) {
        case 枚举_最小原语写回目标宿主::宿主任务治理:
            return 私有_记录宿主任务治理证据_指针(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新指针值,
                *执行上下文->新值标题,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果,
                执行上下文->来源动作名覆盖,
                执行上下文->来源相位);
        case 枚举_最小原语写回目标宿主::主体桥接:
            return 私有_记录主体桥接证据_指针(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新指针值,
                *执行上下文->新值标题,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果,
                执行上下文->来源动作名覆盖,
                执行上下文->来源相位);
        case 枚举_最小原语写回目标宿主::按语义宿主:
        default:
            return 私有_按语义宿主分派记录治理证据_指针(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新指针值,
                *执行上下文->新值标题,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果,
                执行上下文->来源动作名覆盖,
                执行上下文->来源相位);
        }
    }

    bool 私有_通过执行壳写回I64(
        自我类& 自我对象,
        枚举_最小原语写回目标宿主 目标宿主,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        结构_最小原语I64写回执行上下文 写回执行上下文{};
        写回执行上下文.自我对象 = &自我对象;
        写回执行上下文.目标宿主 = 目标宿主;
        写回执行上下文.特征类型 = 特征类型;
        写回执行上下文.新值 = 新值;
        写回执行上下文.动作语义键 = &动作语义键;
        写回执行上下文.来源方法 = 来源方法;
        写回执行上下文.来源任务 = 来源任务;
        写回执行上下文.当前时间 = now;
        写回执行上下文.调用点 = &调用点;
        写回执行上下文.结果 = &结果;
        return 任务管理最小原语模块::执行写入单特征当前值(
            &写回执行上下文,
            私有_执行最小原语I64写回体,
            调用点);
    }

    bool 私有_通过执行壳写回指针(
        自我类& 自我对象,
        枚举_最小原语写回目标宿主 目标宿主,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖,
        枚举_动作事件相位 来源相位) noexcept
    {
        结构_最小原语指针写回执行上下文 写回执行上下文{};
        写回执行上下文.自我对象 = &自我对象;
        写回执行上下文.目标宿主 = 目标宿主;
        写回执行上下文.特征类型 = 特征类型;
        写回执行上下文.新指针值 = 新指针值;
        写回执行上下文.新值标题 = &新值标题;
        写回执行上下文.动作语义键 = &动作语义键;
        写回执行上下文.来源方法 = 来源方法;
        写回执行上下文.来源任务 = 来源任务;
        写回执行上下文.当前时间 = now;
        写回执行上下文.调用点 = &调用点;
        写回执行上下文.结果 = &结果;
        写回执行上下文.来源动作名覆盖 = 来源动作名覆盖;
        写回执行上下文.来源相位 = 来源相位;
        return 任务管理最小原语模块::执行写入单挂点指针(
            &写回执行上下文,
            私有_执行最小原语指针写回体,
            调用点);
    }

    bool 私有_执行固定派生规则_I64写回(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        const std::string 开始调用点 = 调用点 + "/开始";
        const std::string 写回调用点 = 调用点 + "/写回";
        const std::string 结束调用点 = 调用点 + "/结束";

        结构_最小原语边界执行上下文 开始执行上下文{};
        开始执行上下文.自我对象 = &自我对象;
        开始执行上下文.宿主任务 = 来源任务;
        开始执行上下文.当前时间 = now;
        开始执行上下文.调用点 = &开始调用点;
        开始执行上下文.结果 = &结果;
        开始执行上下文.原语ID = 枚举_任务管理最小原语ID::记录动作开始;
        开始执行上下文.相位 = 枚举_动作事件相位::开始运行;
        (void)任务管理最小原语模块::执行记录动作开始(
            &开始执行上下文,
            私有_执行最小原语边界记录体,
            开始调用点);

        结构_最小原语I64写回执行上下文 写回执行上下文{};
        写回执行上下文.自我对象 = &自我对象;
        写回执行上下文.特征类型 = 特征类型;
        写回执行上下文.新值 = 新值;
        写回执行上下文.动作语义键 = &动作语义键;
        写回执行上下文.来源方法 = 来源方法;
        写回执行上下文.来源任务 = 来源任务;
        写回执行上下文.当前时间 = now;
        写回执行上下文.调用点 = &写回调用点;
        写回执行上下文.结果 = &结果;
        const bool 已变化 = 任务管理最小原语模块::执行写入单特征当前值(
            &写回执行上下文,
            私有_执行最小原语I64写回体,
            写回调用点);

        结构_最小原语边界执行上下文 结束执行上下文{};
        结束执行上下文.自我对象 = &自我对象;
        结束执行上下文.宿主任务 = 来源任务;
        结束执行上下文.当前时间 = now;
        结束执行上下文.调用点 = &结束调用点;
        结束执行上下文.结果 = &结果;
        结束执行上下文.原语ID = 枚举_任务管理最小原语ID::记录动作结束;
        结束执行上下文.相位 = 枚举_动作事件相位::完成;
        (void)任务管理最小原语模块::执行记录动作结束(
            &结束执行上下文,
            私有_执行最小原语边界记录体,
            结束调用点);
        return 已变化;
    }

    bool 私有_执行固定派生规则_指针写回(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        const std::string 开始调用点 = 调用点 + "/开始";
        const std::string 写回调用点 = 调用点 + "/写回";
        const std::string 结束调用点 = 调用点 + "/结束";

        结构_最小原语边界执行上下文 开始执行上下文{};
        开始执行上下文.自我对象 = &自我对象;
        开始执行上下文.宿主任务 = 来源任务;
        开始执行上下文.当前时间 = now;
        开始执行上下文.调用点 = &开始调用点;
        开始执行上下文.结果 = &结果;
        开始执行上下文.原语ID = 枚举_任务管理最小原语ID::记录动作开始;
        开始执行上下文.相位 = 枚举_动作事件相位::开始运行;
        (void)任务管理最小原语模块::执行记录动作开始(
            &开始执行上下文,
            私有_执行最小原语边界记录体,
            开始调用点);

        结构_最小原语指针写回执行上下文 写回执行上下文{};
        写回执行上下文.自我对象 = &自我对象;
        写回执行上下文.特征类型 = 特征类型;
        写回执行上下文.新指针值 = 新指针值;
        写回执行上下文.新值标题 = &新值标题;
        写回执行上下文.动作语义键 = &动作语义键;
        写回执行上下文.来源方法 = 来源方法;
        写回执行上下文.来源任务 = 来源任务;
        写回执行上下文.当前时间 = now;
        写回执行上下文.调用点 = &写回调用点;
        写回执行上下文.结果 = &结果;
        const bool 已变化 = 任务管理最小原语模块::执行写入单挂点指针(
            &写回执行上下文,
            私有_执行最小原语指针写回体,
            写回调用点);

        结构_最小原语边界执行上下文 结束执行上下文{};
        结束执行上下文.自我对象 = &自我对象;
        结束执行上下文.宿主任务 = 来源任务;
        结束执行上下文.当前时间 = now;
        结束执行上下文.调用点 = &结束调用点;
        结束执行上下文.结果 = &结果;
        结束执行上下文.原语ID = 枚举_任务管理最小原语ID::记录动作结束;
        结束执行上下文.相位 = 枚举_动作事件相位::完成;
        (void)任务管理最小原语模块::执行记录动作结束(
            &结束执行上下文,
            私有_执行最小原语边界记录体,
            结束调用点);
        return 已变化;
    }

    void 私有_写回治理稳定字段(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        私有_P0_刷新主体桥接字段(结果);
        const auto 当前功能域 = 私有_P0_当前功能域(结果);
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前治理状态迁移 = 私有_P0_当前治理状态迁移(结果);
        const auto 当前方法来源 = 私有_P0_当前方法来源(结果);
        const auto 当前方法需求位 = 私有_P0_当前方法需求位(结果);
        const auto 当前本能能力缺口类型 = 私有_P0_当前本能缺口(结果);
        const auto 当前步骤位类型 = 私有_P0_当前步骤位类型(结果);
        const auto 当前后果治理 = 私有_P0_当前后果治理(结果);
        const auto 最近功能域 =
            结果.最近功能域 != 枚举_任务管理功能域::未定义
            ? 结果.最近功能域
            : 当前功能域;
        const auto 最近总控结果 = 私有_P0_最近总控结果(结果);
        const auto 最近反馈类型 = 私有_P0_最近反馈类型(结果);
        const auto& 最近反馈摘要 = 私有_P0_最近反馈摘要(结果);
        const auto& 当前输入条件主键 = 结果.上下文.当前输入条件主键;
        const auto& 当前治理态型 = 结果.上下文.当前治理态型;
        auto* 当前步骤节点 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果节点 = 私有_P0_最新结果节点(结果);
        auto* 当前方法 = 私有_P0_当前方法(结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前功能域(),
            static_cast<I64>(当前功能域),
            "任务管理_当前功能域",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前功能域",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前缺口类型(),
            static_cast<I64>(当前缺口类型),
            "任务管理_当前缺口类型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前缺口类型",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前下一步去向(),
            static_cast<I64>(当前下一步去向),
            "任务管理_当前下一步去向",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前下一步去向",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前治理状态迁移(),
            static_cast<I64>(当前治理状态迁移),
            "任务管理_当前治理状态迁移",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前治理状态迁移",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前方法来源(),
            static_cast<I64>(当前方法来源),
            "任务管理_当前方法来源",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前方法来源",
            结果);

        (void)私有_执行固定派生规则_I64写回(
            自我对象,
            私有_管理特征_当前方法需求位(),
            static_cast<I64>(当前方法需求位),
            "任务管理_当前方法需求位",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前方法需求位",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前本能能力缺口类型(),
            static_cast<I64>(当前本能能力缺口类型),
            "任务管理_当前本能能力缺口类型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前本能能力缺口类型",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前步骤位类型(),
            static_cast<I64>(当前步骤位类型),
            "任务管理_当前步骤位类型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前步骤位类型",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前宿主目标结果比较(),
            static_cast<I64>(结果.当前宿主目标结果比较),
            "任务管理_当前宿主目标结果比较",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前宿主目标结果比较",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前后果治理(),
            static_cast<I64>(当前后果治理),
            "任务管理_当前后果治理",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前后果治理",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近根层重判结果(),
            static_cast<I64>(结果.最近根层重判结果),
            "任务管理_最近根层重判结果",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近根层重判结果",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近执行前门控结果(),
            static_cast<I64>(结果.最近执行前门控结果),
            "任务管理_最近执行前门控结果",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近执行前门控结果",
            结果);

        const auto* 上层反馈词 = 结果.最近上层反馈摘要.empty()
            ? nullptr
            : 语素集.添加词性词(结果.最近上层反馈摘要, "短语");
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近上层反馈摘要(),
            上层反馈词,
            结果.最近上层反馈摘要,
            "任务管理_最近上层反馈摘要",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近上层反馈摘要",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近功能域(),
            static_cast<I64>(最近功能域),
            "任务管理_最近功能域",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近功能域",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近总控结果(),
            static_cast<I64>(最近总控结果),
            "任务管理_最近总控结果",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近总控结果",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近反馈类型(),
            static_cast<I64>(最近反馈类型),
            "任务管理_最近反馈类型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近反馈类型",
            结果);

        const auto* 主体ID词 = 结果.当前主体ID == 0
            ? nullptr
            : 语素集.添加词性词(std::to_string(结果.当前主体ID), "数词");
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前主体ID(),
            主体ID词,
            结果.当前主体ID == 0 ? std::string{} : std::to_string(结果.当前主体ID),
            "任务管理_当前主体ID",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前主体ID",
            结果);

        const auto* 分身ID词 = 结果.当前分身ID == 0
            ? nullptr
            : 语素集.添加词性词(std::to_string(结果.当前分身ID), "数词");
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前分身ID(),
            分身ID词,
            结果.当前分身ID == 0 ? std::string{} : std::to_string(结果.当前分身ID),
            "任务管理_当前分身ID",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前分身ID",
            结果);

        const auto* 摘要词 = 最近反馈摘要.empty()
            ? nullptr
            : 语素集.添加词性词(最近反馈摘要, "短语");
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近反馈摘要(),
            摘要词,
            最近反馈摘要,
            "任务管理_最近反馈摘要",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近反馈摘要",
            结果);

        const auto* 输入条件词 = 当前输入条件主键.empty()
            ? nullptr
            : 语素集.添加词性词(当前输入条件主键, "短语");
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前输入条件主键(),
            输入条件词,
            当前输入条件主键,
            "任务管理_当前输入条件主键",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前输入条件主键",
            结果);

        const auto* 治理态型词 = 当前治理态型.empty()
            ? nullptr
            : 语素集.添加词性词(当前治理态型, "短语");
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前治理态型(),
            治理态型词,
            当前治理态型,
            "任务管理_当前治理态型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前治理态型",
            结果);

        (void)私有_执行固定派生规则_指针写回(
            自我对象,
            私有_管理特征_当前步骤指针(),
            当前步骤节点,
            私有_任务标题(当前步骤节点),
            "任务管理_当前步骤指针",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前步骤指针",
            结果);

        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近结果指针(),
            最新结果节点,
            私有_任务标题(最新结果节点),
            "任务管理_最近结果指针",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近结果指针",
            结果);
    }

    std::string 私有_治理摘要文本(const 结构_任务管理结果& 结果) noexcept
    {
        const auto 当前宿主任务 = 私有_P0_当前宿主任务(结果);
        const auto 当前子任务 = 私有_P0_当前子任务节点(结果);
        const auto 当前方法 = 私有_P0_当前方法(结果);
        const auto 当前步骤 = 私有_P0_当前步骤节点(结果);
        const auto 最新结果 = 私有_P0_最新结果节点(结果);
        const auto 当前功能域 = 私有_P0_当前功能域(结果);
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前治理状态迁移 = 私有_P0_当前治理状态迁移(结果);
        const auto 当前方法来源 = 私有_P0_当前方法来源(结果);
        const auto 当前步骤位类型 = 私有_P0_当前步骤位类型(结果);
        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        const auto 当前后果治理 = 私有_P0_当前后果治理(结果);
        const auto 当前方法需求位 = 私有_P0_当前方法需求位(结果);
        const auto 当前本能缺口 = 私有_P0_当前本能缺口(结果);
        const auto 最近总控结果 = 私有_P0_最近总控结果(结果);
        const auto 最近反馈类型 = 私有_P0_最近反馈类型(结果);
        const auto& 最近反馈摘要 = 私有_P0_最近反馈摘要(结果);
        const auto& 双面摘要 = 私有_P0_双面摘要(结果);

        std::ostringstream 摘要;
        摘要 << "宿主=" << 私有_任务标题(当前宿主任务)
            << " | 子任务=" << 私有_任务标题(当前子任务)
            << " | 方法=" << 私有_方法标题(当前方法)
            << " | 学习方法=" << 私有_方法标题(结果.当前学习方法节点)
            << " | 学习承接=" << (结果.当前学习承接决策.决策摘要.empty() ? "未决策" : 结果.当前学习承接决策.决策摘要)
            << " | 事件=" << 私有_触发事件类型文本(结果.当前请求.触发事件类型)
            << " | 主体ID=" << 结果.当前主体ID
            << " | 分身ID=" << 结果.当前分身ID
            << " | 方法来源=" << 私有_方法来源文本(当前方法来源)
            << " | 步骤=" << 私有_任务标题(当前步骤)
            << " | 结果=" << 私有_任务标题(最新结果)
            << " | 步骤位=" << 私有_步骤位类型文本(当前步骤位类型)
            << " | 语义=" << 私有_步骤语义类型文本(当前步骤语义类型)
            << " | 宿主比较=" << 私有_双面比较结果文本(结果.当前宿主目标结果比较)
            << " | 后果治理=" << 私有_后果治理文本(当前后果治理)
            << " | 域=" << 私有_功能域文本(当前功能域)
            << " | 缺口=" << 私有_缺口文本(当前缺口类型)
            << " | 去向=" << 私有_去向文本(当前下一步去向)
            << " | 锚点裁决=" << (结果.锚点裁决摘要.empty() ? "空" : 结果.锚点裁决摘要)
            << " | 根层重判=" << 私有_根层重判结果文本(结果.最近根层重判结果)
            << " | 门控=" << 私有_执行前门控结果文本(结果.最近执行前门控结果)
            << " | 迁移=" << 私有_治理状态迁移文本(当前治理状态迁移)
            << " | 总控=" << 私有_总控结果文本(最近总控结果)
            << " | 需求位=" << 私有_方法需求位文本(当前方法需求位)
            << " | 本能缺口=" << 私有_本能能力缺口文本(当前本能缺口)
            << " | 触发来源=" << 私有_触发来源文本(结果.当前触发来源)
            << " | 本轮原语命中=" << (结果.来源最小原语位图 == 0 ? std::string("空") : 私有_原语位图摘要(结果.来源最小原语位图))
            << " | 正式已具备=" << (结果.当前本能能力缺口.原语账本摘要.empty() ? std::string("空") : 结果.当前本能能力缺口.原语账本摘要)
            << " | 主观察=" << (结果.来源主观察特征.empty() ? "空" : 结果.来源主观察特征)
            << " | 影子验证=" << (结果.当前影子验证状态.empty() ? "未触发" : 结果.当前影子验证状态)
            << " | 正式提交=" << (结果.允许正式资产提交 ? "允许" : "禁止")
            << " | 输入条件=" << (结果.上下文.当前输入条件主键.empty() ? "空" : 结果.上下文.当前输入条件主键)
            << " | 治理态型=" << (结果.上下文.当前治理态型.empty() ? "空" : 结果.上下文.当前治理态型)
            << " | 反馈=" << 私有_反馈类型文本(最近反馈类型)
            << " | 摘要=" << (最近反馈摘要.empty() ? "空" : 最近反馈摘要)
            << " | 上层反馈=" << (结果.最近上层反馈摘要.empty() ? "空" : 结果.最近上层反馈摘要)
            << " | 状态=" << 私有_任务状态文本(结果.宿主任务新状态)
            << " | 双面=" << (双面摘要.empty() ? "未整理" : 双面摘要);
        if (!结果.当前主体.最近长期策略摘要.empty()) {
            摘要 << " | 长期策略=" << 结果.当前主体.最近长期策略摘要;
        }
        if (!结果.当前请求.当前恢复点类型.empty()) {
            摘要 << " | 恢复点=" << 结果.当前请求.当前恢复点类型;
        }
        if (结果.当前请求.存在待消费学习回流 || 结果.当前请求.存在待消费外部反馈) {
            摘要 << " | 恢复待消费="
                << "学习回流=" << (结果.当前请求.存在待消费学习回流 ? "是" : "否")
                << "/外部反馈=" << (结果.当前请求.存在待消费外部反馈 ? "是" : "否");
        }
        if (!结果.当前请求.恢复重建摘要.empty()) {
            摘要 << " | 恢复重建=" << 结果.当前请求.恢复重建摘要;
        }
        return 摘要.str();
    }

    bool 私有_绑定宿主任务方法(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点,
        方法节点*& 输出方法) noexcept
    {
        输出方法 = nullptr;
        结果.当前方法来源 = 枚举_任务管理方法来源::未定义;
        if (!宿主任务) {
            return false;
        }

        auto& 任务主信息 = 宿主任务->主信息;
        if (任务主信息.执行方法.指针) {
            输出方法 = reinterpret_cast<方法节点*>(任务主信息.执行方法.指针);
            结果.当前方法来源 = 枚举_任务管理方法来源::已挂方法复用;
            if (!自我对象.获取当前主方法()) {
                (void)自我对象.设置当前主方法(输出方法, now, 调用点 + "/回填当前主方法");
            }
            return false;
        }

        if (auto* 当前方法 = 私有_P0_当前方法(结果);
            当前方法 && 私有_方法属于宿主任务(当前方法, 宿主任务)) {
            私有_完成方法挂接(
                自我对象,
                宿主任务,
                当前方法,
                now,
                调用点 + "/当前主方法回填",
                枚举_任务管理方法来源::当前主方法回填,
                结果,
                输出方法);
            return 输出方法 != nullptr;
        }

        if (auto* 长期策略方法 = 私有_查找主体长期策略方法候选(
            自我对象,
            宿主任务,
            结果)) {
            私有_完成方法挂接(
                自我对象,
                宿主任务,
                长期策略方法,
                now,
                调用点 + "/主体长期策略回填",
                枚举_任务管理方法来源::主体长期策略回填,
                结果,
                输出方法);
            return 输出方法 != nullptr;
        }

        if (auto* 回看方法 = 私有_从最近结果回看方法(结果, 宿主任务)) {
            私有_完成方法挂接(
                自我对象,
                宿主任务,
                回看方法,
                now,
                调用点 + "/最近结果回看",
                枚举_任务管理方法来源::最近结果回看,
                结果,
                输出方法);
            return 输出方法 != nullptr;
        }

        bool 使用了条件节点 = false;
        if (auto* 首节点候选 = 私有_查找宿主任务方法首节点候选(
            自我对象,
            宿主任务,
            结果,
            使用了条件节点)) {
            私有_完成方法挂接(
                自我对象,
                宿主任务,
                首节点候选,
                now,
                调用点 + "/方法树首节点候选",
                使用了条件节点
                    ? 枚举_任务管理方法来源::条件节点筛选通过
                    : 枚举_任务管理方法来源::方法树首节点候选,
                结果,
                输出方法);
            return 输出方法 != nullptr;
        }

        if (!私有_允许默认本能兜底(自我对象, 宿主任务)) {
            return false;
        }

        (void)本能方法类::注册默认自我本能方法集(自我对象, 调用点 + "/默认本能方法");
        const auto 本能ID = 私有_选择本能方法ID(自我对象, 宿主任务);
        auto* 方法节点 = 私有_查找本能方法首节点(自我对象, 本能ID);
        if (!方法节点) {
            return false;
        }

        私有_完成方法挂接(
            自我对象,
            宿主任务,
            方法节点,
            now,
            调用点 + "/默认本能兜底",
            枚举_任务管理方法来源::默认本能兜底,
            结果,
            输出方法);
        return 输出方法 != nullptr;
    }

    bool 私有_任务节点归属宿主任务(const 任务节点* 节点, const 任务节点* 宿主任务) noexcept
    {
        return 节点 && 宿主任务 && 私有_是祖先节点(宿主任务, 节点);
    }

    void 私有_按宿主清洗治理上下文镜像(结构_任务管理上下文& 上下文) noexcept
    {
        auto* 宿主任务 = 上下文.当前宿主任务;
        if (!宿主任务) {
            上下文.当前方法 = nullptr;
            上下文.当前步骤 = nullptr;
            上下文.最近结果 = nullptr;
            上下文.当前方法位专项动态 = nullptr;
            上下文.当前方法位专项动态标题.clear();
            上下文.当前最小原语动态 = nullptr;
            上下文.当前最小原语动态标题.clear();
            return;
        }

        auto* 已挂方法 = 宿主任务->主信息.执行方法.指针
            ? reinterpret_cast<方法节点*>(宿主任务->主信息.执行方法.指针)
            : nullptr;
        if (上下文.当前方法
            && !私有_方法属于宿主任务(上下文.当前方法, 宿主任务)
            && 上下文.当前方法 != 已挂方法) {
            上下文.当前方法 = nullptr;
        }

        if (上下文.当前步骤
            && (上下文.当前步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点
                || !私有_任务节点归属宿主任务(上下文.当前步骤, 宿主任务))) {
            上下文.当前步骤 = nullptr;
        }

        if (上下文.最近结果
            && (上下文.最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
                || !私有_任务节点归属宿主任务(上下文.最近结果, 宿主任务))) {
            上下文.最近结果 = nullptr;
        }

        auto* 最近结果父步骤 = 上下文.最近结果 && 上下文.最近结果->父
            ? static_cast<任务节点*>(上下文.最近结果->父)
            : nullptr;
        if (最近结果父步骤
            && (最近结果父步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点
                || !私有_任务节点归属宿主任务(最近结果父步骤, 宿主任务))) {
            上下文.最近结果 = nullptr;
            最近结果父步骤 = nullptr;
        }

        if (上下文.最近结果 && 最近结果父步骤) {
            if (!上下文.当前步骤 || 上下文.当前步骤 != 最近结果父步骤) {
                上下文.当前步骤 = 最近结果父步骤;
            }
        }

        if (!上下文.当前方法
            || !私有_方法属于宿主任务(上下文.当前方法, 宿主任务)
            || 上下文.当前方法需求位 == 枚举_任务管理方法需求位::未定义) {
            上下文.当前方法位专项动态 = nullptr;
            上下文.当前方法位专项动态标题.clear();
        }

        if (上下文.当前本能能力缺口类型 == 枚举_任务管理本能能力缺口类型::未定义) {
            上下文.当前最小原语动态 = nullptr;
            上下文.当前最小原语动态标题.clear();
        }
    }

    枚举_任务管理步骤位类型 私有_推导步骤位类型(
        const 结构_任务管理结果& 结果,
        bool 从最近结果回接) noexcept
    {
        const auto 当前功能域 = 私有_P0_当前功能域(结果);
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);

        if (当前功能域 == 枚举_任务管理功能域::等待
            || 当前下一步去向 == 枚举_任务管理下一步去向::保持等待) {
            return 枚举_任务管理步骤位类型::等待位;
        }
        if (当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
            || 当前下一步去向 == 枚举_任务管理下一步去向::绑定方法) {
            return 枚举_任务管理步骤位类型::补条件位;
        }
        if (从最近结果回接) {
            return 枚举_任务管理步骤位类型::回接位;
        }
        return 枚举_任务管理步骤位类型::执行位;
    }

    std::string 私有_步骤名称前缀文本(const 结构_任务管理结果& 结果) noexcept
    {
        switch (私有_P0_当前步骤语义类型(结果)) {
        case 枚举_任务管理步骤语义类型::等待维持步骤: return "等待位";
        case 枚举_任务管理步骤语义类型::回接派生步骤: return "业务回接位";
        case 枚举_任务管理步骤语义类型::补条件步骤: return "补条件位";
        case 枚举_任务管理步骤语义类型::业务推进步骤: return "业务执行位";
        case 枚举_任务管理步骤语义类型::治理步骤:
        default:
            return "治理步骤";
        }
    }

    std::string 私有_结果名称前缀文本(const 结构_任务管理结果& 结果) noexcept
    {
        switch (私有_P0_当前步骤语义类型(结果)) {
        case 枚举_任务管理步骤语义类型::等待维持步骤: return "等待结果";
        case 枚举_任务管理步骤语义类型::回接派生步骤: return "业务回接结果";
        case 枚举_任务管理步骤语义类型::补条件步骤: return "补条件结果";
        case 枚举_任务管理步骤语义类型::业务推进步骤: return "业务执行结果";
        case 枚举_任务管理步骤语义类型::治理步骤:
        default:
            return "治理结果";
        }
    }

    bool 私有_任务状态已终结(枚举_任务状态 状态) noexcept
    {
        switch (状态) {
        case 枚举_任务状态::完成:
        case 枚举_任务状态::失败:
        case 枚举_任务状态::取消:
            return true;
        default:
            return false;
        }
    }

    const 词性节点类* 私有_治理步骤类型词(const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(私有_步骤名称前缀文本(结果), "名词");
    }

    const 词性节点类* 私有_治理结果类型词(const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(私有_结果名称前缀文本(结果), "名词");
    }

    std::string 私有_治理步骤键(const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << 私有_功能域文本(私有_P0_当前功能域(结果))
            << "_" << 私有_任务状态文本(结果.宿主任务新状态)
            << "_" << 私有_方法标题(私有_P0_当前方法(结果));
        return 输出.str();
    }

    std::string 私有_治理结果键(const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << 私有_功能域文本(私有_P0_当前功能域(结果))
            << "_" << 私有_缺口文本(私有_P0_当前缺口类型(结果))
            << "_" << 私有_去向文本(私有_P0_当前下一步去向(结果))
            << "_" << 私有_任务状态文本(结果.宿主任务新状态)
            << "_" << 私有_方法标题(私有_P0_当前方法(结果));
        return 输出.str();
    }

    const 词性节点类* 私有_治理步骤名称词(const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            私有_步骤名称前缀文本(结果) + "_" + 私有_治理步骤键(结果),
            "专有名词");
    }

    const 词性节点类* 私有_治理结果名称词(const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            私有_结果名称前缀文本(结果) + "_" + 私有_治理结果键(结果),
            "专有名词");
    }

    std::string 私有_补条件子任务键(const 任务节点* 宿主任务, const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << 私有_任务标题(宿主任务)
            << "_" << 私有_缺口文本(私有_P0_当前缺口类型(结果))
            << "_" << 私有_去向文本(私有_P0_当前下一步去向(结果));
        return 输出.str();
    }

    std::string 私有_M10_任务状态签名(const 状态节点类* 状态) noexcept
    {
        枚举_任务状态 状态值 = 枚举_任务状态::未定义;
        return 私有_读取状态节点任务状态(状态, 状态值)
            ? 私有_任务状态文本(状态值)
            : std::string("缺失");
    }

    std::string 私有_M10_业务差额签名(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        const auto* 宿主目标状态 = 宿主任务 ? 宿主任务->主信息.目标状态.指针 : 结果.宿主目标状态;
        const auto* 宿主结果状态 = 宿主任务 ? 宿主任务->主信息.结果状态信息.指针 : 结果.宿主结果状态;

        枚举_任务状态 目标值 = 枚举_任务状态::未定义;
        枚举_任务状态 结果值 = 枚举_任务状态::未定义;
        const bool 有目标 = 私有_读取状态节点任务状态(宿主目标状态, 目标值);
        const bool 有结果 = 私有_读取状态节点任务状态(宿主结果状态, 结果值);

        std::string 比较文本 = "待补齐";
        if (有目标 && 有结果) {
            比较文本 = (目标值 == 结果值) ? "已对齐" : "存在差额";
        }

        std::ostringstream 输出;
        输出 << 比较文本
            << "|目标=" << (有目标 ? 私有_任务状态文本(目标值) : std::string("缺失"))
            << "|结果=" << (有结果 ? 私有_任务状态文本(结果值) : std::string("缺失"));
        return 输出.str();
    }

    std::string 私有_M10_当前条件签名(const 结构_任务管理结果& 结果) noexcept
    {
        const auto& 签名 = !结果.当前一步结果.当前输入条件主键.empty()
            ? 结果.当前一步结果.当前输入条件主键
            : (
                !结果.上下文.当前输入条件主键.empty()
                ? 结果.上下文.当前输入条件主键
                : (
                    !结果.当前单步决策.来源主观察特征.empty()
                    ? 结果.当前单步决策.来源主观察特征
                    : (
                        !结果.当前请求.来源主观察特征.empty()
                        ? 结果.当前请求.来源主观察特征
                        : 结果.来源主观察特征
                    )
                )
            );
        return 签名.empty() ? std::string("未定义条件") : 签名;
    }

    std::string 私有_M10_有效正式方法标题(const 结构_任务管理结果& 结果) noexcept
    {
        if (!结果.当前主体.最近正式方法标题.empty()) {
            return 结果.当前主体.最近正式方法标题;
        }
        return 私有_方法标题(私有_P0_当前方法(结果));
    }

    std::string 私有_M10_长期策略摘要版本(const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 原始;
        原始 << 结果.当前主体.最近正式方法标题
            << "|" << 结果.当前主体.最近长期策略摘要
            << "|" << 结果.当前主体.累计正式方法资产提交次数
            << "|" << 结果.当前主体.当前长期策略权重_Q10000
            << "|" << 结果.当前主体.当前长期策略降权级别;
        const auto 哈希值 = static_cast<unsigned long long>(std::hash<std::string>{}(原始.str()));
        return "v" + std::to_string(哈希值);
    }

    bool 私有_M10_存在业务推进压力(const 结构_任务管理结果& 结果) noexcept
    {
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        return 结果.宿主目标结果待推进
            || 当前缺口类型 == 枚举_任务管理缺口类型::任务运行中
            || 当前下一步去向 == 枚举_任务管理下一步去向::保持运行
            || 当前步骤语义类型 == 枚举_任务管理步骤语义类型::业务推进步骤
            || 当前步骤语义类型 == 枚举_任务管理步骤语义类型::回接派生步骤;
    }

    void 私有_M10_合并长期策略范围签名(
        std::string& 范围摘要,
        const std::string& 新签名) noexcept
    {
        if (新签名.empty() || 新签名 == "未定义条件" || 新签名 == "待补齐|目标=缺失|结果=缺失") {
            return;
        }

        std::vector<std::string> 片段{};
        std::string 当前{};
        for (char 字符 : 范围摘要) {
            if (字符 == ';') {
                if (!当前.empty()) {
                    片段.push_back(std::move(当前));
                    当前.clear();
                }
                continue;
            }
            当前.push_back(字符);
        }
        if (!当前.empty()) {
            片段.push_back(std::move(当前));
        }

        for (const auto& 片段值 : 片段) {
            if (片段值 == 新签名) {
                return;
            }
        }

        片段.push_back(新签名);
        while (片段.size() > 4) {
            片段.erase(片段.begin());
        }

        std::ostringstream 输出;
        for (std::size_t i = 0; i < 片段.size(); ++i) {
            if (i != 0) {
                输出 << ';';
            }
            输出 << 片段[i];
        }
        范围摘要 = 输出.str();
    }

    void 私有_M10_刷新长期策略权重(结构_任务管理主体虚拟存在& 主体) noexcept
    {
        if (主体.累计正式方法资产提交次数 == 0 || 主体.最近正式方法标题.empty()) {
            主体.当前长期策略权重_Q10000 = 0;
            主体.当前长期策略降权级别 = 0;
            return;
        }

        const auto 成功次数 = 主体.累计长期策略成功次数;
        const auto 失败次数 = 主体.累计长期策略失败次数;
        const auto 正式资产次数 = 主体.累计正式方法资产提交次数;

        int 权重 = 6000;
        权重 += static_cast<int>((成功次数 > 8 ? 8 : 成功次数) * 450);
        权重 += static_cast<int>((正式资产次数 > 8 ? 8 : 正式资产次数) * 180);
        权重 -= static_cast<int>((失败次数 > 8 ? 8 : 失败次数) * 700);
        if (失败次数 > 成功次数) {
            权重 -= static_cast<int>((失败次数 - 成功次数) * 300);
        }

        if (权重 < 1000) {
            权重 = 1000;
        }
        if (权重 > 10000) {
            权重 = 10000;
        }

        主体.当前长期策略权重_Q10000 = static_cast<std::uint32_t>(权重);
        主体.当前长期策略降权级别 =
            失败次数 == 0
            ? 0U
            : static_cast<std::uint32_t>(失败次数 > 9 ? 9 : 失败次数);
    }

    bool 私有_M10_当前方法命中主体长期策略(const 结构_任务管理结果& 结果) noexcept
    {
        if (结果.当前主体.累计正式方法资产提交次数 == 0
            || 结果.当前主体.最近正式方法标题.empty()) {
            return false;
        }

        const auto 目标正式方法标题 = 结果.当前主体.最近正式方法标题;
        if (auto* 当前方法 = 私有_P0_当前方法(结果);
            当前方法 && 私有_方法标题(当前方法) == 目标正式方法标题) {
            return true;
        }

        auto* 宿主任务 = 私有_P0_当前宿主任务(结果);
        auto* 已挂方法 =
            宿主任务 && 宿主任务->主信息.执行方法.指针
            ? reinterpret_cast<方法节点*>(宿主任务->主信息.执行方法.指针)
            : nullptr;
        return 已挂方法 && 私有_方法标题(已挂方法) == 目标正式方法标题;
    }

    bool 私有_M10_长期策略可靠度允许回填(const 结构_任务管理主体虚拟存在& 主体) noexcept
    {
        if (主体.累计正式方法资产提交次数 == 0 || 主体.最近正式方法标题.empty()) {
            return false;
        }
        const auto 权重 = 主体.当前长期策略权重_Q10000 == 0
            ? 6000U
            : 主体.当前长期策略权重_Q10000;
        return 权重 >= 2500U;
    }

    bool 私有_M10_当前轮形成有效长期策略推进(const 结构_任务管理结果& 结果) noexcept
    {
        if (私有_M9_事件轮禁止业务扩张(结果)) {
            return false;
        }
        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        return 结果.已创建业务子任务
            || 结果.已复用业务子任务
            || 当前步骤语义类型 == 枚举_任务管理步骤语义类型::业务推进步骤
            || 当前步骤语义类型 == 枚举_任务管理步骤语义类型::回接派生步骤
            || 当前下一步去向 == 枚举_任务管理下一步去向::保持运行
            || 当前下一步去向 == 枚举_任务管理下一步去向::进入运行;
    }

    std::string 私有_M10_当前轮长期策略失效原因(const 结构_任务管理结果& 结果) noexcept
    {
        if (私有_M9_事件轮禁止业务扩张(结果)) {
            return "事件轮禁止扩张";
        }
        if (私有_P0_当前本能缺口(结果) != 枚举_任务管理本能能力缺口类型::无缺口) {
            return "桥接缺口未满足";
        }
        if (!私有_M10_存在业务推进压力(结果)) {
            return "宿主缺少业务推进压力";
        }
        if (私有_P0_当前下一步去向(结果) == 枚举_任务管理下一步去向::保持等待) {
            return "未形成有效推进";
        }
        return "未命中稳定业务推进";
    }

    bool 私有_M10_长期策略驱动业务子任务派生(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_M10_存在业务推进压力(结果)
            && 私有_M10_当前方法命中主体长期策略(结果)
            && !私有_M10_有效正式方法标题(结果).empty();
    }

    std::string 私有_业务子任务键(const 任务节点* 宿主任务, const 结构_任务管理结果& 结果) noexcept
    {
        const auto 正式方法标题 = 私有_M10_有效正式方法标题(结果);
        const auto 业务差额签名 = 私有_M10_业务差额签名(宿主任务, 结果);
        const auto 目标状态签名 = 私有_M10_任务状态签名(
            宿主任务 ? 宿主任务->主信息.目标状态.指针 : 结果.宿主目标状态);
        const auto 长期策略摘要版本 = 私有_M10_长期策略摘要版本(结果);

        std::ostringstream 原始;
        原始 << (宿主任务 ? 宿主任务->获取主键() : std::string("空宿主"))
            << "|差额=" << 业务差额签名
            << "|方法=" << 正式方法标题
            << "|目标=" << 目标状态签名
            << "|策略=" << 长期策略摘要版本;

        std::ostringstream 输出;
        输出 << (宿主任务 ? 宿主任务->获取主键() : std::string("空宿主"))
            << "_"
            << static_cast<unsigned long long>(std::hash<std::string>{}(原始.str()));
        return 输出.str();
    }

    const 词性节点类* 私有_补条件子任务名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "补条件子任务_" + 私有_补条件子任务键(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_补条件子任务类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("补条件子任务", "名词");
        return s_词;
    }

    const 词性节点类* 私有_业务子任务名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "业务子任务_" + 私有_业务子任务键(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_业务子任务类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("业务子任务", "名词");
        return s_词;
    }

    const 词性节点类* 私有_任务管理学习树名称词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理学习树", "专有名词");
        return s_词;
    }

    const 词性节点类* 私有_任务管理学习树类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("学习树根节点", "名词");
        return s_词;
    }

    const 词性节点类* 私有_定向学习分支名称词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("定向学习分支", "专有名词");
        return s_词;
    }

    const 词性节点类* 私有_学习分支类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("学习分支", "名词");
        return s_词;
    }

    std::string 私有_学习子任务键(const 任务节点* 宿主任务, const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << 私有_任务标题(宿主任务)
            << "_" << 私有_学习通道文本(结果)
            << "_" << 私有_缺口文本(私有_P0_当前缺口类型(结果))
            << "_" << 私有_去向文本(私有_P0_当前下一步去向(结果))
            << "_" << 私有_方法需求位文本(私有_P0_当前方法需求位(结果))
            << "_" << 私有_本能能力缺口文本(私有_P0_当前本能缺口(结果))
            << "_" << 私有_方法标题(私有_P0_当前方法(结果));
        return 输出.str();
    }

    const 词性节点类* 私有_学习子任务名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习子任务_" + 私有_学习子任务键(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_学习子任务类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("学习子任务", "名词");
        return s_词;
    }

    const 词性节点类* 私有_方法虚拟存在类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法虚拟存在", "名词");
        return s_词;
    }

    bool 私有_应新生学习方法头(const 结构_任务管理结果& 结果) noexcept
    {
        const auto* 当前方法 = 私有_P0_当前方法(结果);
        return !当前方法
            || 私有_P0_当前方法来源(结果) == 枚举_任务管理方法来源::默认本能兜底;
    }

    std::string 私有_学习方法动作名文本(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        const auto* 当前方法 = 私有_P0_当前方法(结果);
        if (私有_应新生学习方法头(结果)) {
            if (当前方法) {
                return "候选_" + 私有_方法标题(当前方法) + "_" + 私有_任务标题(宿主任务);
            }
            return "待学习动作_" + 私有_学习子任务键(宿主任务, 结果);
        }
        return 私有_方法标题(当前方法);
    }

    const 词性节点类* 私有_学习方法动作名词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        const auto* 当前方法 = 私有_P0_当前方法(结果);
        if (!私有_应新生学习方法头(结果) && 当前方法 && 当前方法->主信息.动作名) {
            return 当前方法->主信息.动作名;
        }
        return 语素集.添加词性词(私有_学习方法动作名文本(宿主任务, 结果), "动词");
    }

    const 词性节点类* 私有_学习方法虚拟存在名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习方法_" + 私有_学习方法动作名文本(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_学习方法条件场景名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习方法条件场景_" + 私有_学习方法动作名文本(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_学习方法结果场景名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习方法结果场景_" + 私有_学习方法动作名文本(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_学习方法条件节点动作名词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习条件_" + 私有_任务标题(宿主任务) + "_" + 私有_缺口文本(私有_P0_当前缺口类型(结果)),
            "动词");
    }

    const 词性节点类* 私有_学习方法结果节点动作名词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习结果_" + 私有_任务标题(宿主任务) + "_" + 私有_去向文本(私有_P0_当前下一步去向(结果)),
            "动词");
    }

    bool 私有_任务为业务子任务(const 任务节点* 任务) noexcept
    {
        return 任务 && 私有_词性相同(任务->主信息.类型, 私有_业务子任务类型词());
    }

    枚举_任务状态 私有_治理结果状态(const 结构_任务管理结果& 结果) noexcept
    {
        if (!私有_P0_当前宿主任务(结果)) {
            return 枚举_任务状态::失败;
        }
        if (!私有_P0_当前方法(结果)) {
            return 枚举_任务状态::挂起;
        }

        switch (结果.宿主任务新状态) {
        case 枚举_任务状态::完成:
            return 枚举_任务状态::完成;
        case 枚举_任务状态::失败:
            return 枚举_任务状态::失败;
        case 枚举_任务状态::取消:
            return 枚举_任务状态::取消;
        case 枚举_任务状态::挂起:
            return 枚举_任务状态::挂起;
        default:
            return 枚举_任务状态::完成;
        }
    }

    场景节点类* 私有_任务场景(自我类& 自我对象, const 任务节点* 节点) noexcept
    {
        if (节点 && 节点->主信息.场景.指针) {
            return 节点->主信息.场景.指针;
        }
        if (auto* 现实场景 = 自我对象.获取自我现实场景()) {
            return 现实场景;
        }
        return 自我对象.获取自我内部世界();
    }

    存在节点类* 私有_确保任务虚拟存在(
        自我类& 自我对象,
        任务节点* 任务,
        const std::string& 调用点) noexcept
    {
        (void)调用点;
        if (!任务) {
            return nullptr;
        }

        if (任务->主信息.任务虚拟存在.指针) {
            return 任务->主信息.任务虚拟存在.指针;
        }

        auto* 宿主场景 = 私有_任务场景(自我对象, 任务);
        if (!宿主场景) {
            return nullptr;
        }

        auto* 任务虚拟存在 = 世界树.存在().取或创建子存在_按名称类型(
            宿主场景,
            私有_任务虚拟存在名称词(任务),
            私有_任务虚拟存在类型词());
        if (!任务虚拟存在) {
            return nullptr;
        }

        私有_写入引用(任务->主信息.任务虚拟存在, 任务虚拟存在);
        return 任务虚拟存在;
    }

    const 特征节点类* 私有_查找宿主任务治理特征节点(
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型) noexcept
    {
        if (!宿主任务 || !特征类型 || !宿主任务->主信息.任务虚拟存在.指针) {
            return nullptr;
        }
        const auto* 宿主基础节点 =
            reinterpret_cast<const 基础信息节点类*>(宿主任务->主信息.任务虚拟存在.指针);
        return 宿主基础节点
            ? 世界树.存在().查找子特征_按类型(宿主基础节点, 特征类型)
            : nullptr;
    }

    特征节点类* 私有_确保宿主任务治理特征节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务 || !特征类型) {
            return nullptr;
        }
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(自我对象, 宿主任务, 调用点);
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        return 宿主基础节点
            ? 世界树.取或创建子特征_按类型(宿主基础节点, 特征类型)
            : nullptr;
    }

    const 特征节点类* 私有_查找主体桥接特征节点(
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型) noexcept
    {
        return 私有_查找宿主任务治理特征节点(宿主任务, 特征类型);
    }

    特征节点类* 私有_确保主体桥接特征节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        // P3 过渡期先复用宿主任务虚拟存在承载桥接字段，P4 再正式拆出主体/分身存在。
        return 私有_确保宿主任务治理特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点);
    }

    const 特征节点类* 私有_查找方法资产特征节点(
        const 方法节点* 方法头,
        const 词性节点类* 特征类型) noexcept
    {
        if (!方法头 || !特征类型 || !方法头->主信息.方法虚拟存在.指针) {
            return nullptr;
        }
        const auto* 宿主基础节点 =
            reinterpret_cast<const 基础信息节点类*>(方法头->主信息.方法虚拟存在.指针);
        return 宿主基础节点
            ? 世界树.存在().查找子特征_按类型(宿主基础节点, 特征类型)
            : nullptr;
    }

    特征节点类* 私有_确保方法资产特征节点(
        方法节点* 方法头,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        (void)调用点;
        if (!方法头 || !特征类型 || !方法头->主信息.方法虚拟存在.指针) {
            return nullptr;
        }
        auto* 宿主基础节点 =
            reinterpret_cast<基础信息节点类*>(方法头->主信息.方法虚拟存在.指针);
        return 宿主基础节点
            ? 世界树.取或创建子特征_按类型(宿主基础节点, 特征类型)
            : nullptr;
    }

    bool 私有_状态节点值匹配任务状态(const 状态节点类* 状态节点, 枚举_任务状态 状态) noexcept
    {
        const auto* 主信息 = 世界树.状态().取状态主信息(状态节点);
        if (!主信息) {
            return false;
        }

        const auto* 当前值 = std::get_if<I64>(&主信息->状态值);
        return 当前值 && (*当前值 == static_cast<I64>(状态));
    }

    bool 私有_读取状态节点任务状态(const 状态节点类* 状态节点, 枚举_任务状态& 输出) noexcept
    {
        const auto* 主信息 = 世界树.状态().取状态主信息(状态节点);
        if (!主信息) {
            return false;
        }

        const auto* 当前值 = std::get_if<I64>(&主信息->状态值);
        if (!当前值) {
            return false;
        }

        输出 = static_cast<枚举_任务状态>(*当前值);
        return true;
    }

    bool 私有_任务节点目标结果已对齐(const 任务节点* 任务) noexcept
    {
        if (!任务) {
            return false;
        }

        枚举_任务状态 目标值 = 枚举_任务状态::未定义;
        枚举_任务状态 结果值 = 枚举_任务状态::未定义;
        return 私有_读取状态节点任务状态(任务->主信息.目标状态.指针, 目标值)
            && 私有_读取状态节点任务状态(任务->主信息.结果状态信息.指针, 结果值)
            && 目标值 == 结果值;
    }

    bool 私有_任务节点目标结果待推进(const 任务节点* 任务) noexcept
    {
        if (!任务) {
            return false;
        }

        枚举_任务状态 目标值 = 枚举_任务状态::未定义;
        枚举_任务状态 结果值 = 枚举_任务状态::未定义;
        return 私有_读取状态节点任务状态(任务->主信息.目标状态.指针, 目标值)
            && 私有_读取状态节点任务状态(任务->主信息.结果状态信息.指针, 结果值)
            && 目标值 != 结果值;
    }

    状态节点类* 私有_确保任务状态镜像(
        自我类& 自我对象,
        任务节点* 任务,
        可解析引用<状态节点类>& 现有引用,
        const 词性节点类* 特征类型,
        枚举_任务状态 状态值,
        时间戳 now,
        const std::string& 原因类别,
        const std::string& 原因说明) noexcept
    {
        if (!任务 || !特征类型) {
            return nullptr;
        }

        if (私有_状态节点值匹配任务状态(现有引用.指针, 状态值)) {
            return 现有引用.指针;
        }

        auto* 场景 = 私有_任务场景(自我对象, 任务);
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(自我对象, 任务, 原因类别);
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        auto* 特征节点 = 宿主基础节点
            ? 世界树.取或创建子特征_按类型(宿主基础节点, 特征类型)
            : nullptr;
        if (!场景 || !宿主基础节点 || !特征节点) {
            return nullptr;
        }

        auto* 状态节点 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ static_cast<I64>(状态值) },
            now,
            true,
            原因类别,
            原因说明);
        if (!状态节点) {
            return nullptr;
        }

        私有_写入引用(现有引用, 状态节点);
        return 状态节点;
    }

    枚举_任务状态 私有_宿主任务目标状态(const 结构_任务管理结果& 结果) noexcept
    {
        switch (私有_P0_当前治理状态迁移(结果)) {
        case 枚举_任务管理治理状态迁移::进入挂起:
            return 枚举_任务状态::挂起;
        case 枚举_任务管理治理状态迁移::进入运行:
        case 枚举_任务管理治理状态迁移::保持运行:
        case 枚举_任务管理治理状态迁移::保持等待:
            return 枚举_任务状态::运行中;
        case 枚举_任务管理治理状态迁移::进入收束:
            return 结果.宿主任务新状态 != 枚举_任务状态::未定义
                ? 结果.宿主任务新状态
                : 枚举_任务状态::完成;
        case 枚举_任务管理治理状态迁移::回到重筹办:
            return 枚举_任务状态::未启动;
        default:
            return 结果.宿主任务新状态 != 枚举_任务状态::未定义
                ? 结果.宿主任务新状态
                : 枚举_任务状态::未启动;
        }
    }

    状态节点类* 私有_确保任务目标状态(
        自我类& 自我对象,
        任务节点* 任务,
        枚举_任务状态 目标状态,
        时间戳 now,
        const std::string& 原因类别,
        const std::string& 原因说明) noexcept
    {
        return 私有_确保任务状态镜像(
            自我对象,
            任务,
            任务->主信息.目标状态,
            私有_任务特征_目标状态(),
            目标状态,
            now,
            原因类别,
            原因说明);
    }

    状态节点类* 私有_确保任务结果状态(
        自我类& 自我对象,
        任务节点* 任务,
        枚举_任务状态 结果状态,
        时间戳 now,
        const std::string& 原因类别,
        const std::string& 原因说明) noexcept
    {
        if (!任务) {
            return nullptr;
        }

        return 私有_确保任务状态镜像(
            自我对象,
            任务,
            任务->主信息.结果状态信息,
            私有_任务特征_结果状态(),
            结果状态,
            now,
            原因类别,
            原因说明);
    }

    bool 私有_任务节点方法匹配(const 任务节点* 任务, const 方法节点* 方法) noexcept
    {
        const auto* 已挂方法 = 任务 && 任务->主信息.执行方法.指针
            ? reinterpret_cast<const 方法节点*>(任务->主信息.执行方法.指针)
            : nullptr;
        return 已挂方法 == 方法;
    }

    bool 私有_可进入等待分支(const 任务节点* 宿主任务, const 结构_任务管理结果& 结果) noexcept
    {
        const auto* 当前方法 = 私有_P0_当前方法(结果);
        const auto* 当前步骤 = 私有_P0_当前步骤节点(结果);
        const auto* 最近结果 = 私有_P0_最新结果节点(结果);
        if (!宿主任务 || !当前方法) {
            return false;
        }
        if (结果.已绑定方法挂点 || 结果.已推进任务状态) {
            return false;
        }
        if (宿主任务->主信息.状态 != 枚举_任务状态::运行中) {
            return false;
        }
        if (私有_任务节点目标结果待推进(宿主任务)) {
            return false;
        }

        if (!当前步骤 || !最近结果) {
            return false;
        }
        if (当前步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点
            || 最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点) {
            return false;
        }
        if (!私有_任务节点归属宿主任务(当前步骤, 宿主任务)
            || !私有_任务节点归属宿主任务(最近结果, 宿主任务)) {
            return false;
        }
        if (最近结果->主信息.状态 != 枚举_任务状态::完成) {
            return false;
        }
        return 私有_任务节点方法匹配(当前步骤, 当前方法)
            && 私有_任务节点方法匹配(最近结果, 当前方法);
    }

    枚举_任务管理步骤语义类型 私有_推导步骤语义类型(
        const 结构_任务管理结果& 结果,
        const 任务节点* 宿主任务) noexcept
    {
        switch (私有_P0_当前步骤位类型(结果)) {
        case 枚举_任务管理步骤位类型::等待位:
            return 枚举_任务管理步骤语义类型::等待维持步骤;
        case 枚举_任务管理步骤位类型::回接位:
            return 枚举_任务管理步骤语义类型::回接派生步骤;
        case 枚举_任务管理步骤位类型::补条件位:
            return 枚举_任务管理步骤语义类型::补条件步骤;
        case 枚举_任务管理步骤位类型::执行位:
            return 私有_任务节点目标结果待推进(宿主任务)
                ? 枚举_任务管理步骤语义类型::业务推进步骤
                : 枚举_任务管理步骤语义类型::治理步骤;
        default:
            return 枚举_任务管理步骤语义类型::未定义;
        }
    }

    bool 私有_业务子任务应转入执行位(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        const auto* 当前方法 = 私有_P0_当前方法(结果);
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto* 最近结果 = 私有_P0_最新结果节点(结果);
        if (!私有_任务为业务子任务(宿主任务)) {
            return false;
        }
        if (!当前方法) {
            return false;
        }
        if (当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
            || 当前下一步去向 == 枚举_任务管理下一步去向::绑定方法) {
            return false;
        }
        if (私有_查找直接派生子任务(宿主任务)) {
            return false;
        }
        if (!最近结果
            || 最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
            || !私有_任务节点归属宿主任务(最近结果, 宿主任务)) {
            return false;
        }

        const auto* 最近步骤 = 最近结果->父
            ? static_cast<const 任务节点*>(最近结果->父)
            : nullptr;
        if (!最近步骤 || 最近步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点) {
            return false;
        }

        static const 词性节点类* s_业务回接位类型 = 语素集.添加词性词("业务回接位", "名词");
        return 私有_词性相同(最近步骤->主信息.类型, s_业务回接位类型);
    }

    bool 私有_业务子任务执行已完成(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        const auto* 当前方法 = 私有_P0_当前方法(结果);
        const auto* 最近结果 = 私有_P0_最新结果节点(结果);
        if (!私有_任务为业务子任务(宿主任务)) {
            return false;
        }
        if (私有_查找直接派生子任务(宿主任务)) {
            return false;
        }
        if (!最近结果
            || 最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
            || 最近结果->主信息.状态 != 枚举_任务状态::完成
            || !私有_任务节点归属宿主任务(最近结果, 宿主任务)) {
            return false;
        }

        const auto* 最近步骤 = 最近结果->父
            ? static_cast<const 任务节点*>(最近结果->父)
            : nullptr;
        if (!最近步骤
            || 最近步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点
            || !私有_任务节点归属宿主任务(最近步骤, 宿主任务)
            || 最近步骤->主信息.状态 != 枚举_任务状态::完成) {
            return false;
        }

        static const 词性节点类* s_业务执行位类型 = 语素集.添加词性词("业务执行位", "名词");
        static const 词性节点类* s_业务执行结果类型 = 语素集.添加词性词("业务执行结果", "名词");
        if (!私有_词性相同(最近步骤->主信息.类型, s_业务执行位类型)
            || !私有_词性相同(最近结果->主信息.类型, s_业务执行结果类型)) {
            return false;
        }

        if (当前方法) {
            return 私有_任务节点方法匹配(最近步骤, 当前方法)
                && 私有_任务节点方法匹配(最近结果, 当前方法);
        }
        return true;
    }

    任务节点* 私有_查找待回接的已完成业务子任务(
        const 任务节点* 宿主任务,
        const 结构_任务管理上下文& 上下文) noexcept
    {
        if (!宿主任务 || !上下文.最近结果) {
            return nullptr;
        }

        const auto* 最近结果 = 上下文.最近结果;
        if (最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
            || 最近结果->主信息.状态 != 枚举_任务状态::完成) {
            return nullptr;
        }

        const auto* 最近步骤 = 最近结果->父
            ? static_cast<const 任务节点*>(最近结果->父)
            : nullptr;
        if (!最近步骤
            || 最近步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点
            || 最近步骤->主信息.状态 != 枚举_任务状态::完成) {
            return nullptr;
        }

        static const 词性节点类* s_业务执行位类型 = 语素集.添加词性词("业务执行位", "名词");
        static const 词性节点类* s_业务执行结果类型 = 语素集.添加词性词("业务执行结果", "名词");
        if (!私有_词性相同(最近步骤->主信息.类型, s_业务执行位类型)
            || !私有_词性相同(最近结果->主信息.类型, s_业务执行结果类型)) {
            return nullptr;
        }

        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(
            const_cast<任务节点*>(宿主任务),
            [&](const 任务节点* 节点) noexcept {
                return 节点
                    && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                    && 私有_词性相同(节点->主信息.类型, 私有_业务子任务类型词())
                    && 节点->主信息.状态 == 枚举_任务状态::完成
                    && 私有_任务节点归属宿主任务(最近结果, 节点)
                    && 私有_任务节点归属宿主任务(最近步骤, 节点);
            }));
    }

    bool 私有_写回任务目标状态(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        bool 已写回 = false;
        auto* 当前步骤节点 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果节点 = 私有_P0_最新结果节点(结果);
        auto* 当前方法 = 私有_P0_当前方法(结果);

        auto* 宿主目标状态 = 私有_确保任务目标状态(
            自我对象,
            宿主任务,
            私有_宿主任务目标状态(结果),
            now,
            "任务管理_宿主目标状态",
            私有_治理状态迁移文本(结果.当前治理状态迁移));
        if (宿主目标状态) {
            结果.宿主目标状态 = 宿主目标状态;
            结果.最新目标状态 = 宿主目标状态;
            已写回 = true;
        }

        auto* 步骤目标状态 = 私有_确保任务目标状态(
            自我对象,
            当前步骤节点,
            当前方法 ? 枚举_任务状态::完成 : 枚举_任务状态::挂起,
            now,
            "任务管理_步骤目标状态",
            调用点);
        if (步骤目标状态) {
            结果.步骤目标状态 = 步骤目标状态;
            结果.最新目标状态 = 步骤目标状态;
            已写回 = true;
        }

        auto* 结果目标状态 = 私有_确保任务目标状态(
            自我对象,
            最新结果节点,
            私有_治理结果状态(结果),
            now,
            "任务管理_结果目标状态",
            调用点);
        if (结果目标状态) {
            结果.结果目标状态 = 结果目标状态;
            结果.最新目标状态 = 结果目标状态;
            已写回 = true;
        }

        结果.已写回目标状态 = 已写回;
        return 已写回;
    }

    void 私有_同步任务结果状态引用(
        任务节点* 宿主任务,
        结构_任务管理结果& 结果) noexcept
    {
        auto* 当前步骤节点 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果节点 = 私有_P0_最新结果节点(结果);
        结果.宿主结果状态 = 宿主任务 ? 宿主任务->主信息.结果状态信息.指针 : nullptr;
        结果.步骤结果状态 = 当前步骤节点 ? 当前步骤节点->主信息.结果状态信息.指针 : nullptr;
        结果.结果结果状态 = 最新结果节点 ? 最新结果节点->主信息.结果状态信息.指针 : nullptr;

        if (结果.结果结果状态) {
            if (当前步骤节点
                && 当前步骤节点->主信息.结果状态信息.指针 != 结果.结果结果状态) {
                私有_写入引用(当前步骤节点->主信息.结果状态信息, 结果.结果结果状态);
                结果.步骤结果状态 = 结果.结果结果状态;
            }
        }
    }

    bool 私有_写回任务结果状态(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        bool 已写回 = false;
        auto* 当前步骤节点 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果节点 = 私有_P0_最新结果节点(结果);

        auto* 宿主结果状态 = 私有_确保任务结果状态(
            自我对象,
            宿主任务,
            结果.宿主任务新状态 != 枚举_任务状态::未定义
                ? 结果.宿主任务新状态
                : (宿主任务 ? 宿主任务->主信息.状态 : 枚举_任务状态::未定义),
            now,
            "任务管理_宿主结果状态",
            调用点);
        if (宿主结果状态) {
            结果.宿主结果状态 = 宿主结果状态;
            结果.最新状态 = 宿主结果状态;
            已写回 = true;
        }

        auto* 步骤结果状态 = 私有_确保任务结果状态(
            自我对象,
            当前步骤节点,
            当前步骤节点 ? 当前步骤节点->主信息.状态 : 枚举_任务状态::未定义,
            now,
            "任务管理_步骤结果状态",
            调用点);
        if (步骤结果状态) {
            结果.步骤结果状态 = 步骤结果状态;
            结果.最新状态 = 步骤结果状态;
            已写回 = true;
        }

        auto* 结果结果状态 = 私有_确保任务结果状态(
            自我对象,
            最新结果节点,
            最新结果节点 ? 最新结果节点->主信息.状态 : 枚举_任务状态::未定义,
            now,
            "任务管理_结果结果状态",
            调用点);
        if (结果结果状态) {
            结果.结果结果状态 = 结果结果状态;
            结果.最新状态 = 结果结果状态;
            已写回 = true;
        }

        私有_同步任务结果状态引用(宿主任务, 结果);
        return 已写回;
    }

    std::string 私有_任务双面摘要文本(
        const char* 标签,
        const 状态节点类* 目标状态,
        const 状态节点类* 结果状态) noexcept
    {
        枚举_任务状态 目标值 = 枚举_任务状态::未定义;
        枚举_任务状态 结果值 = 枚举_任务状态::未定义;
        const bool 有目标 = 私有_读取状态节点任务状态(目标状态, 目标值);
        const bool 有结果 = 私有_读取状态节点任务状态(结果状态, 结果值);

        std::ostringstream 输出;
        输出 << 标签
            << "目标=" << (有目标 ? 私有_任务状态文本(目标值) : std::string("缺失"))
            << "/结果=" << (有结果 ? 私有_任务状态文本(结果值) : std::string("缺失"))
            << "/差额=";
        if (有目标 && 有结果 && 目标值 == 结果值) {
            输出 << "已对齐";
        }
        else if (有目标 && 有结果) {
            输出 << "待推进";
        }
        else {
            输出 << "待补齐";
        }
        return 输出.str();
    }

    枚举_任务管理双面比较结果 私有_比较任务双面(
        const 状态节点类* 目标状态,
        const 状态节点类* 结果状态) noexcept
    {
        枚举_任务状态 目标值 = 枚举_任务状态::未定义;
        枚举_任务状态 结果值 = 枚举_任务状态::未定义;
        const bool 有目标 = 私有_读取状态节点任务状态(目标状态, 目标值);
        const bool 有结果 = 私有_读取状态节点任务状态(结果状态, 结果值);
        if (!有目标 || !有结果) {
            return 枚举_任务管理双面比较结果::待补齐;
        }
        if (目标值 == 结果值) {
            return 枚举_任务管理双面比较结果::已对齐;
        }
        return 枚举_任务管理双面比较结果::存在差额;
    }

    void 私有_整理后果治理(
        结构_任务管理结果& 结果,
        bool 存在待回接业务子任务) noexcept
    {
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        if (存在待回接业务子任务) {
            if (结果.当前宿主目标结果比较 == 枚举_任务管理双面比较结果::已对齐) {
                结果.当前后果治理 = 枚举_任务管理后果治理::回接后保持等待;
                return;
            }
            if (结果.当前宿主目标结果比较 == 枚举_任务管理双面比较结果::存在差额) {
                结果.当前后果治理 = 枚举_任务管理后果治理::回接后继续派生;
                return;
            }
        }

        switch (当前下一步去向) {
        case 枚举_任务管理下一步去向::进入收束:
            结果.当前后果治理 = 枚举_任务管理后果治理::进入收束;
            return;
        case 枚举_任务管理下一步去向::保持等待:
            结果.当前后果治理 = 枚举_任务管理后果治理::保持等待;
            return;
        default:
            break;
        }

        switch (结果.当前宿主目标结果比较) {
        case 枚举_任务管理双面比较结果::待补齐:
            结果.当前后果治理 = 枚举_任务管理后果治理::补齐双面;
            return;
        case 枚举_任务管理双面比较结果::存在差额:
            结果.当前后果治理 = 枚举_任务管理后果治理::继续派生业务步骤;
            return;
        case 枚举_任务管理双面比较结果::已对齐:
            结果.当前后果治理 = 枚举_任务管理后果治理::无额外后果;
            return;
        default:
            结果.当前后果治理 = 枚举_任务管理后果治理::未定义;
            return;
        }
    }

    void 私有_整理任务双面摘要(
        任务节点* 宿主任务,
        结构_任务管理结果& 结果) noexcept
    {
        (void)宿主任务;
        结果.宿主目标状态 = 宿主任务 ? 宿主任务->主信息.目标状态.指针 : nullptr;
        结果.步骤目标状态 = 结果.当前步骤节点 ? 结果.当前步骤节点->主信息.目标状态.指针 : nullptr;
        结果.结果目标状态 = 结果.最新结果节点 ? 结果.最新结果节点->主信息.目标状态.指针 : nullptr;
        私有_同步任务结果状态引用(宿主任务, 结果);

        结果.当前宿主目标结果比较 = 私有_比较任务双面(结果.宿主目标状态, 结果.宿主结果状态);
        结果.当前步骤目标结果比较 = 私有_比较任务双面(结果.步骤目标状态, 结果.步骤结果状态);
        结果.当前结果目标结果比较 = 私有_比较任务双面(结果.结果目标状态, 结果.结果结果状态);
        结果.最近宿主目标结果比较 = 结果.当前宿主目标结果比较;
        结果.宿主目标结果已对齐 = 结果.当前宿主目标结果比较 == 枚举_任务管理双面比较结果::已对齐;
        结果.宿主目标结果待推进 = 结果.当前宿主目标结果比较 == 枚举_任务管理双面比较结果::存在差额;
        结果.已由目标差额驱动 = 结果.宿主目标结果待推进;

        std::vector<std::string> 片段{};
        片段.emplace_back(私有_任务双面摘要文本("宿主", 结果.宿主目标状态, 结果.宿主结果状态));
        if (结果.当前步骤节点) {
            片段.emplace_back(私有_任务双面摘要文本("步骤", 结果.步骤目标状态, 结果.步骤结果状态));
        }
        if (结果.最新结果节点) {
            片段.emplace_back(私有_任务双面摘要文本("结果", 结果.结果目标状态, 结果.结果结果状态));
        }
        片段.emplace_back("宿主比较=" + 私有_双面比较结果文本(结果.当前宿主目标结果比较));
        if (结果.宿主目标结果待推进) {
            片段.emplace_back("宿主目标与结果存在差额，下一轮继续派生业务步骤");
        }
        else if (结果.当前宿主目标结果比较 == 枚举_任务管理双面比较结果::待补齐) {
            片段.emplace_back("宿主目标与结果仍待补齐，当前只补账本双面");
        }
        结果.目标结果差额摘要 = 私有_拼接片段(片段);
    }

    void 私有_同步治理步骤节点(
        任务节点* 步骤节点,
        任务节点* 父节点,
        任务节点* 宿主任务,
        const 结构_任务管理结果& 结果,
        场景节点类* 场景,
        时间戳 now) noexcept
    {
        if (!步骤节点 || !宿主任务) {
            return;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);
        auto& 主信息 = 步骤节点->主信息;
        主信息.名称 = 私有_治理步骤名称词(结果);
        主信息.类型 = 私有_治理步骤类型词(结果);
        主信息.节点种类 = 枚举_任务节点种类::步骤节点;
        主信息.状态 = 当前方法 ? 枚举_任务状态::运行中 : 枚举_任务状态::挂起;
        if (主信息.启动时间 == 0) {
            主信息.启动时间 = now;
        }
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 宿主任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 宿主任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 宿主任务->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 父节点);
        私有_写入引用(主信息.场景, 场景);
        私有_写入引用(主信息.执行方法, 当前方法);
        if (当前方法) {
            主信息.动作句柄 = 当前方法->主信息.动作句柄;
        }
        else {
            主信息.动作句柄 = 结构体_动作句柄{};
        }
        if (结果.最新状态) {
            私有_写入引用(主信息.结果状态信息, 结果.最新状态);
        }
    }

    bool 私有_结果节点匹配当前治理(
        const 任务节点* 结果节点,
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        if (!结果节点 || !宿主任务) {
            return false;
        }
        if (结果节点->主信息.节点种类 != 枚举_任务节点种类::结果节点) {
            return false;
        }
        if (!私有_任务节点归属宿主任务(结果节点, 宿主任务)) {
            return false;
        }
        if (!私有_词性相同(结果节点->主信息.名称, 私有_治理结果名称词(结果))) {
            return false;
        }
        if (结果节点->主信息.状态 != 私有_治理结果状态(结果)) {
            return false;
        }

        const auto* 当前方法 = 私有_P0_当前方法(结果);
        const auto* 已挂方法 = 结果节点->主信息.执行方法.指针
            ? reinterpret_cast<const 方法节点*>(结果节点->主信息.执行方法.指针)
            : nullptr;
        return 当前方法 == 已挂方法;
    }

    任务节点* 私有_创建治理步骤节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务) {
            return nullptr;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);
        auto* 挂接父节点 = 宿主任务;
        if (结果.上下文.最近结果
            && 结果.上下文.最近结果->主信息.节点种类 == 枚举_任务节点种类::结果节点
            && 私有_任务节点归属宿主任务(结果.上下文.最近结果, 宿主任务)) {
            挂接父节点 = 结果.上下文.最近结果;
        }

        auto* 场景 = 宿主任务->主信息.场景.指针
            ? 宿主任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.启动时间 = now;

        主信息.名称 = 私有_治理步骤名称词(结果);
        主信息.类型 = 私有_治理步骤类型词(结果);
        主信息.节点种类 = 枚举_任务节点种类::步骤节点;
        主信息.状态 = 当前方法 ? 枚举_任务状态::运行中 : 枚举_任务状态::挂起;
        主信息.基准优先级 = 宿主任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 宿主任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 宿主任务->主信息.调度优先级;
        主信息.统计.记录观测(now);
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 挂接父节点);
        私有_写入引用(主信息.前置任务, 挂接父节点);
        私有_写入引用(主信息.场景, 场景);
        私有_写入引用(主信息.执行方法, 当前方法);
        if (当前方法) {
            主信息.动作句柄 = 当前方法->主信息.动作句柄;
        }
        if (结果.最新状态) {
            私有_写入引用(主信息.结果状态信息, 结果.最新状态);
        }

        auto* 步骤节点 = 私有_独立树添加子节点<任务类>(挂接父节点, 主信息);
        if (!步骤节点) {
            return nullptr;
        }

        私有_写入引用(挂接父节点->主信息.后续任务, 步骤节点);
        结果.当前步骤节点 = 步骤节点;
        结果.上下文.当前步骤 = 步骤节点;
        结果.已创建步骤节点 = true;
        return 步骤节点;
    }

    任务节点* 私有_创建治理结果节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        任务节点* 步骤节点,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务 || !步骤节点) {
            return nullptr;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);
        auto* 场景 = 宿主任务->主信息.场景.指针
            ? 宿主任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.名称 = 私有_治理结果名称词(结果);
        主信息.类型 = 私有_治理结果类型词(结果);
        主信息.节点种类 = 枚举_任务节点种类::结果节点;
        主信息.状态 = 私有_治理结果状态(结果);
        主信息.启动时间 = 步骤节点->主信息.启动时间 != 0 ? 步骤节点->主信息.启动时间 : now;
        主信息.完成时间 = now;
        主信息.基准优先级 = 宿主任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 宿主任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 宿主任务->主信息.调度优先级;
        主信息.统计.记录观测(now);
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 步骤节点);
        私有_写入引用(主信息.前置任务, 步骤节点);
        私有_写入引用(主信息.场景, 场景);
        私有_写入引用(主信息.执行方法, 当前方法);
        if (当前方法) {
            主信息.动作句柄 = 当前方法->主信息.动作句柄;
        }
        if (结果.最新状态) {
            私有_写入引用(主信息.结果状态信息, 结果.最新状态);
        }

        auto* 结果节点 = 私有_独立树添加子节点<任务类>(步骤节点, 主信息);
        if (!结果节点) {
            return nullptr;
        }

        步骤节点->主信息.状态 = 枚举_任务状态::完成;
        if (步骤节点->主信息.启动时间 == 0) {
            步骤节点->主信息.启动时间 = now;
        }
        步骤节点->主信息.完成时间 = now;
        步骤节点->主信息.统计.记录观测(now);
        if (结果.最新状态) {
            私有_写入引用(步骤节点->主信息.结果状态信息, 结果.最新状态);
            私有_写入引用(宿主任务->主信息.结果状态信息, 结果.最新状态);
        }
        私有_写入引用(步骤节点->主信息.后续任务, 结果节点);

        结果.最新结果节点 = 结果节点;
        结果.上下文.最近结果 = 结果节点;
        结果.已创建结果节点 = true;
        return 结果节点;
    }

    任务节点* 私有_创建或复用补条件子任务头(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务) {
            return nullptr;
        }

        if (私有_P0_当前步骤语义类型(结果) != 枚举_任务管理步骤语义类型::补条件步骤) {
            return nullptr;
        }

        if (私有_词性相同(宿主任务->主信息.类型, 私有_补条件子任务类型词())) {
            return nullptr;
        }

        auto* 现有子任务 = 私有_查找直接子节点_按谓词<任务节点>(宿主任务, [&](const 任务节点* 节点) noexcept {
            return 节点
                && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                && 私有_词性相同(节点->主信息.名称, 私有_补条件子任务名称词(宿主任务, 结果))
                && !私有_任务状态已终结(节点->主信息.状态);
        });

        if (现有子任务) {
            结果.当前子任务节点 = 现有子任务;
            结果.已复用补条件子任务 = true;
            return 现有子任务;
        }

        auto* 场景 = 宿主任务->主信息.场景.指针
            ? 宿主任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.名称 = 私有_补条件子任务名称词(宿主任务, 结果);
        主信息.类型 = 私有_补条件子任务类型词();
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::未启动;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 宿主任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 宿主任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 宿主任务->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 宿主任务);
        私有_写入引用(主信息.前置任务, 宿主任务);
        私有_写入引用(主信息.场景, 场景);

        auto* 子任务头 = 私有_独立树添加子节点<任务类>(宿主任务, 主信息);
        if (!子任务头) {
            return nullptr;
        }

        结果.当前子任务节点 = 子任务头;
        结果.已创建补条件子任务 = true;
        return 子任务头;
    }

    任务节点* 私有_创建或复用业务子任务头(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        const bool 长期策略驱动业务派生 = 私有_M10_长期策略驱动业务子任务派生(结果);
        if (!宿主任务
            || (!结果.宿主目标结果待推进
                && !(长期策略驱动业务派生 && 私有_M10_存在业务推进压力(结果)))) {
            return nullptr;
        }

        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        if (当前步骤语义类型 != 枚举_任务管理步骤语义类型::回接派生步骤
            && 当前步骤语义类型 != 枚举_任务管理步骤语义类型::业务推进步骤
            && !长期策略驱动业务派生) {
            return nullptr;
        }

        if (长期策略驱动业务派生
            && 当前步骤语义类型 != 枚举_任务管理步骤语义类型::回接派生步骤
            && 当前步骤语义类型 != 枚举_任务管理步骤语义类型::业务推进步骤) {
            结果.当前步骤语义类型 = 枚举_任务管理步骤语义类型::业务推进步骤;
        }

        if (私有_任务为业务子任务(宿主任务)) {
            return nullptr;
        }

        auto* 现有子任务 = 私有_查找直接子节点_按谓词<任务节点>(宿主任务, [&](const 任务节点* 节点) noexcept {
            return 节点
                && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                && 私有_词性相同(节点->主信息.名称, 私有_业务子任务名称词(宿主任务, 结果))
                && !私有_任务状态已终结(节点->主信息.状态);
        });

        if (现有子任务) {
            结果.当前子任务节点 = 现有子任务;
            结果.已复用业务子任务 = true;
            return 现有子任务;
        }

        auto* 场景 = 宿主任务->主信息.场景.指针
            ? 宿主任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());
        auto* 当前方法 = 私有_P0_当前方法(结果);

        任务主信息类 主信息{};
        主信息.名称 = 私有_业务子任务名称词(宿主任务, 结果);
        主信息.类型 = 私有_业务子任务类型词();
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::未启动;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 宿主任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 宿主任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 宿主任务->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 宿主任务);
        私有_写入引用(主信息.前置任务, 宿主任务);
        私有_写入引用(主信息.场景, 场景);
        私有_写入引用(主信息.执行方法, 当前方法);
        if (当前方法) {
            主信息.动作句柄 = 当前方法->主信息.动作句柄;
        }

        auto* 子任务头 = 私有_独立树添加子节点<任务类>(宿主任务, 主信息);
        if (!子任务头) {
            return nullptr;
        }

        结果.当前子任务节点 = 子任务头;
        结果.已创建业务子任务 = true;
        return 子任务头;
    }

    任务节点* 私有_确保任务管理学习树根(
        自我类& 自我对象,
        任务节点* 管理任务,
        const 结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        (void)结果;
        if (!管理任务) {
            return nullptr;
        }

        if (auto* 现有根 = 私有_查找任务管理学习树根(管理任务)) {
            return 现有根;
        }

        auto* 场景 = 管理任务->主信息.场景.指针
            ? 管理任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.名称 = 私有_任务管理学习树名称词();
        主信息.类型 = 私有_任务管理学习树类型词();
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::运行中;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 管理任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 管理任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 管理任务->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 管理任务);
        私有_写入引用(主信息.前置任务, 管理任务);
        私有_写入引用(主信息.场景, 场景);

        return 私有_独立树添加子节点<任务类>(管理任务, 主信息);
    }

    任务节点* 私有_确保定向学习分支(
        自我类& 自我对象,
        任务节点* 学习树根,
        const 结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        (void)结果;
        if (!学习树根) {
            return nullptr;
        }

        if (auto* 现有分支 = 私有_查找定向学习分支(学习树根)) {
            return 现有分支;
        }

        auto* 场景 = 学习树根->主信息.场景.指针
            ? 学习树根->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.名称 = 私有_定向学习分支名称词();
        主信息.类型 = 私有_学习分支类型词();
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::运行中;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 学习树根->主信息.基准优先级;
        主信息.局部优先级偏移 = 学习树根->主信息.局部优先级偏移;
        主信息.调度优先级 = 学习树根->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 学习树根);
        私有_写入引用(主信息.前置任务, 学习树根);
        私有_写入引用(主信息.场景, 场景);

        return 私有_独立树添加子节点<任务类>(学习树根, 主信息);
    }

    bool 私有_应创建学习子任务(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        if (!宿主任务) {
            return false;
        }
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前方法来源 = 私有_P0_当前方法来源(结果);
        const auto 当前本能能力缺口 = 私有_P0_当前本能缺口(结果);
        const auto* 当前方法 = 私有_P0_当前方法(结果);
        const bool 稳定等待态复盘学习 =
            !结果.宿主目标结果待推进
            && 当前缺口类型 == 枚举_任务管理缺口类型::无缺口
            && 当前下一步去向 == 枚举_任务管理下一步去向::保持等待
            && 当前方法来源 == 枚举_任务管理方法来源::已挂方法复用
            && 当前方法;
        if (私有_本能缺口属于原语或治理补齐(当前本能能力缺口)
            || 当前本能能力缺口 == 枚举_任务管理本能能力缺口类型::需学习补齐) {
            return true;
        }
        if (当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
            || 当前下一步去向 == 枚举_任务管理下一步去向::绑定方法) {
            return true;
        }
        if (稳定等待态复盘学习) {
            return true;
        }
        return 结果.宿主目标结果待推进
            && 当前方法来源 == 枚举_任务管理方法来源::默认本能兜底;
    }

    const char* 私有_学习通道文本(const 结构_学习承接决策& 决策) noexcept
    {
        return 决策.补原语或治理本能 ? "补原语/治理本能" : "补方法";
    }

    结构_学习承接决策 私有_形成学习承接决策(
        任务节点* 当前管理任务,
        任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_学习承接决策 决策{};
        决策.来源根任务 = 宿主任务 ? 私有_主体锚点任务(宿主任务) : nullptr;
        决策.来源父任务 = 宿主任务;
        决策.来源功能域 = 私有_P0_当前功能域(结果);
        决策.来源缺口类型 = 私有_P0_当前缺口类型(结果);
        决策.来源方法需求位 = 私有_P0_当前方法需求位(结果);
        决策.来源本能能力缺口类型 = 私有_P0_当前本能缺口(结果);
        决策.来源最小原语位图 =
            结果.当前本能能力缺口.所需原语位图 != 0
            ? 结果.当前本能能力缺口.所需原语位图
            : 结果.来源最小原语位图;
        决策.来源正式原语摘要 =
            !结果.当前本能能力缺口.原语账本摘要.empty()
            ? 结果.当前本能能力缺口.原语账本摘要
            : std::string{};
        决策.来源主观察特征 =
            !结果.当前本能能力缺口.主观察特征键.empty()
            ? 结果.当前本能能力缺口.主观察特征键
            : 结果.来源主观察特征;
        决策.回流目标方法位 = 决策.来源方法需求位;
        决策.回流目标摘要 =
            !结果.当前写回结果.主体回并摘要.empty()
            ? 结果.当前写回结果.主体回并摘要
            : (
                !结果.当前需求位特征.需求摘要.empty()
                ? 结果.当前需求位特征.需求摘要
                : 私有_P0_最近反馈摘要(结果)
            );
        决策.补原语或治理本能 =
            私有_本能缺口属于原语或治理补齐(决策.来源本能能力缺口类型);
        决策.是否影子验证 = !结果.允许正式资产提交;
        决策.允许正式资产提交 = 结果.允许正式资产提交;

        if (!当前管理任务 || !宿主任务) {
            决策.决策摘要 = "学习承接=未放行 | 原因=缺管理任务或宿主";
            return 决策;
        }

        决策.学习类型 = 枚举_任务管理学习承接类型::定向学习;
        决策.是否触发学习 = 私有_应创建学习子任务(宿主任务, 结果);

        std::ostringstream 摘要;
        摘要 << "学习承接="
            << (决策.是否触发学习 ? "已放行" : "未放行")
            << " | 类型=" << 私有_学习承接类型文本(决策.学习类型)
            << " | 通道=" << 私有_学习通道文本(决策)
            << " | 来源根任务=" << 私有_任务标题(决策.来源根任务)
            << " | 来源父任务=" << 私有_任务标题(决策.来源父任务)
            << " | 来源功能域=" << 私有_功能域文本(决策.来源功能域)
            << " | 来源缺口=" << 私有_缺口文本(决策.来源缺口类型)
            << " | 来源需求位=" << 私有_方法需求位文本(决策.来源方法需求位)
            << " | 来源本能缺口=" << 私有_本能能力缺口文本(决策.来源本能能力缺口类型)
            << " | 本轮原语命中=" << (决策.来源最小原语位图 == 0 ? std::string("空") : 私有_原语位图摘要(决策.来源最小原语位图))
            << " | 正式已具备=" << (决策.来源正式原语摘要.empty() ? std::string("空") : 决策.来源正式原语摘要)
            << " | 主观察=" << (决策.来源主观察特征.empty() ? "空" : 决策.来源主观察特征)
            << " | 回流目标=" << (决策.回流目标摘要.empty() ? "空" : 决策.回流目标摘要)
            << " | 影子验证=" << (决策.是否影子验证 ? "是" : "否")
            << " | 正式提交=" << (决策.允许正式资产提交 ? "允许" : "禁止");
        决策.决策摘要 = 摘要.str();
        return 决策;
    }

    void 私有_同步学习承接决策(
        结构_任务管理结果& 结果,
        const 结构_学习承接决策& 决策) noexcept
    {
        结果.当前学习承接决策 = 决策;
        结果.当前一步结果.已触发学习承接 = 决策.是否触发学习;
        结果.当前一步结果.当前学习承接类型 = 决策.学习类型;
        结果.当前一步结果.当前学习为本能补齐 = 决策.补原语或治理本能;
        结果.当前一步结果.学习承接摘要 = 决策.决策摘要;
    }

    任务节点* 私有_创建或复用学习子任务头(
        自我类& 自我对象,
        任务节点* 管理任务,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!私有_应创建学习子任务(宿主任务, 结果)) {
            return nullptr;
        }

        auto* 学习树根 = 私有_确保任务管理学习树根(
            自我对象,
            管理任务,
            结果,
            now,
            调用点 + "/学习树根");
        auto* 定向分支 = 私有_确保定向学习分支(
            自我对象,
            学习树根,
            结果,
            now,
            调用点 + "/定向学习分支");
        if (!定向分支) {
            return nullptr;
        }

        auto* 现有子任务 = 私有_查找直接子节点_按谓词<任务节点>(定向分支, [&](const 任务节点* 节点) noexcept {
            return 节点
                && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                && 私有_词性相同(节点->主信息.类型, 私有_学习子任务类型词())
                && 私有_词性相同(节点->主信息.名称, 私有_学习子任务名称词(宿主任务, 结果))
                && !私有_任务状态已终结(节点->主信息.状态);
        });
        if (现有子任务) {
            if (!结果.当前子任务节点) {
                结果.当前子任务节点 = 现有子任务;
            }
            结果.已复用学习子任务 = true;
            return 现有子任务;
        }

        auto* 场景 = 宿主任务 && 宿主任务->主信息.场景.指针
            ? 宿主任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.名称 = 私有_学习子任务名称词(宿主任务, 结果);
        主信息.类型 = 私有_学习子任务类型词();
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::挂起;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 宿主任务 ? 宿主任务->主信息.基准优先级 : 0;
        主信息.局部优先级偏移 = 宿主任务 ? 宿主任务->主信息.局部优先级偏移 : 0;
        主信息.调度优先级 = 宿主任务 ? 宿主任务->主信息.调度优先级 : 0;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 宿主任务);
        私有_写入引用(主信息.前置任务, 宿主任务);
        私有_写入引用(主信息.场景, 场景);

        auto* 子任务头 = 私有_独立树添加子节点<任务类>(定向分支, 主信息);
        if (!子任务头) {
            return nullptr;
        }

        if (!结果.当前子任务节点) {
            结果.当前子任务节点 = 子任务头;
        }
        结果.已创建学习子任务 = true;
        return 子任务头;
    }

    I64 私有_统计待学习方法数量(const 任务节点* 管理任务) noexcept
    {
        const auto* 学习树根 = 私有_查找任务管理学习树根(管理任务);
        if (!学习树根) {
            return 0;
        }

        I64 总数 = 0;
        私有_枚举直接子节点<任务节点>(学习树根, [&](const 任务节点* 分支节点) {
            if (!分支节点) {
                return;
            }
            私有_枚举直接子节点<任务节点>(分支节点, [&](const 任务节点* 学习节点) {
                if (学习节点
                    && 学习节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                    && 私有_词性相同(学习节点->主信息.类型, 私有_学习子任务类型词())
                    && !私有_任务状态已终结(学习节点->主信息.状态)) {
                    ++总数;
                }
            });
        });
        return 总数;
    }

    bool 私有_学习方法有条件侧(const 方法节点* 方法头) noexcept
    {
        if (!方法头) {
            return false;
        }
        if (方法头->主信息.条件场景.指针) {
            return true;
        }

        auto* 首节点 = static_cast<const 方法节点*>(方法头->子);
        if (!首节点) {
            return false;
        }

        auto* 当前 = 首节点;
        do {
            if (当前 && 当前->主信息.节点种类 == 枚举_方法节点种类::方法条件节点) {
                return true;
            }
            当前 = static_cast<const 方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return false;
    }

    bool 私有_学习方法有动作骨架(const 方法节点* 方法头) noexcept
    {
        return 方法头
            && 方法头->主信息.动作句柄.类型 != 枚举_动作句柄类型::未设置;
    }

    bool 私有_学习方法仅需求位(const 方法节点* 方法头) noexcept
    {
        return 方法头
            && !(私有_学习方法有动作骨架(方法头) && 私有_学习方法有条件侧(方法头));
    }

    bool 私有_学习方法可直接进入学习(const 方法节点* 方法头) noexcept
    {
        return 方法头
            && 私有_学习方法有动作骨架(方法头)
            && 私有_学习方法有条件侧(方法头);
    }

    状态节点类* 私有_学习方法条件目标状态(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        if (结果.宿主目标状态) {
            return 结果.宿主目标状态;
        }
        if (结果.最新目标状态) {
            return 结果.最新目标状态;
        }
        return 宿主任务 ? 宿主任务->主信息.目标状态.指针 : nullptr;
    }

    状态节点类* 私有_学习方法结果目标状态(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        if (结果.宿主结果状态) {
            return 结果.宿主结果状态;
        }
        if (结果.最新状态) {
            return 结果.最新状态;
        }
        return 宿主任务 ? 宿主任务->主信息.结果状态信息.指针 : nullptr;
    }

    方法节点* 私有_查找学习方法头候选(
        const 自我类& 自我对象,
        const 任务节点* 学习子任务,
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        auto* 自我存在 = const_cast<存在节点类*>(自我对象.获取自我存在());
        if (!自我存在 || !学习子任务) {
            return nullptr;
        }

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        auto* 动作名 = 私有_学习方法动作名词(宿主任务, 结果);
        if (!方法根节点 || !动作名) {
            return nullptr;
        }

        auto* 首节点 = static_cast<方法节点*>(方法根节点->子);
        if (!首节点) {
            return nullptr;
        }

        auto* 当前 = 首节点;
        do {
            if (当前
                && 当前->主信息.节点种类 == 枚举_方法节点种类::方法首节点
                && 私有_词性相同(当前->主信息.动作名, 动作名)
                && reinterpret_cast<const void*>(当前->主信息.来源任务.指针)
                    == reinterpret_cast<const void*>(学习子任务)) {
                return 当前;
            }

            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    void 私有_补齐学习方法结构(
        自我类& 自我对象,
        任务节点* 学习子任务,
        任务节点* 宿主任务,
        方法节点* 方法头,
        结构_任务管理结果& 结果,
        const std::string& 调用点) noexcept
    {
        if (!方法头) {
            return;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);

        auto& 主信息 = 方法头->主信息;
        主信息.节点种类 = 枚举_方法节点种类::方法首节点;
        if (!主信息.动作名) {
            主信息.动作名 = 私有_学习方法动作名词(宿主任务, 结果);
        }
        if (学习子任务 && !主信息.来源任务.指针) {
            私有_写入引用(主信息.来源任务, 学习子任务);
        }
        if (主信息.动作句柄.类型 == 枚举_动作句柄类型::未设置
            && 当前方法
            && 当前方法->主信息.动作句柄.类型 != 枚举_动作句柄类型::未设置) {
            主信息.动作句柄 = 当前方法->主信息.动作句柄;
        }
        if (!主信息.期望结果状态.指针) {
            if (auto* 期望状态 = 私有_学习方法条件目标状态(宿主任务, 结果)) {
                私有_写入引用(主信息.期望结果状态, 期望状态);
            }
        }

        auto* 自我内部世界 = 自我对象.确保自我内部世界(调用点 + "/自我内部世界");
        if (!自我内部世界) {
            return;
        }

        auto* 方法虚拟存在 = 主信息.方法虚拟存在.指针;
        if (!方法虚拟存在) {
            方法虚拟存在 = 世界树.存在().取或创建子存在_按名称类型(
                自我内部世界,
                私有_学习方法虚拟存在名称词(宿主任务, 结果),
                私有_方法虚拟存在类型词());
            if (方法虚拟存在) {
                私有_写入引用(主信息.方法虚拟存在, 方法虚拟存在);
            }
        }

        if (方法虚拟存在) {
            const auto 方法类型 = 私有_推导方法类型(方法头, &结果);
            if (方法类型 != 枚举_方法类型::未定义) {
                (void)私有_通过执行壳写回I64(
                    自我对象,
                    枚举_最小原语写回目标宿主::方法资产,
                    私有_方法特征_方法类型(),
                    static_cast<I64>(方法类型),
                    "方法_方法类型写回",
                    方法头,
                    宿主任务,
                    学习子任务 && 学习子任务->主信息.启动时间 != 0
                        ? 学习子任务->主信息.启动时间
                        : (宿主任务 ? 宿主任务->主信息.启动时间 : 0),
                    调用点 + "/方法类型",
                    结果);
            }
        }

        if (方法虚拟存在 && !主信息.条件场景.指针) {
            if (auto* 条件场景 = 世界树.场景().取或创建子场景_按名称(
                方法虚拟存在,
                私有_学习方法条件场景名称词(宿主任务, 结果),
                枚举_世界类型::内部世界)) {
                私有_写入引用(主信息.条件场景, 条件场景);
            }
        }

        if (方法虚拟存在 && !主信息.结果场景.指针) {
            if (auto* 结果场景 = 世界树.场景().取或创建子场景_按名称(
                方法虚拟存在,
                私有_学习方法结果场景名称词(宿主任务, 结果),
                枚举_世界类型::内部世界)) {
                私有_写入引用(主信息.结果场景, 结果场景);
            }
        }
    }

    方法节点* 私有_创建或复用学习方法头(
        自我类& 自我对象,
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!学习子任务) {
            return nullptr;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);

        if (!私有_应新生学习方法头(结果) && 当前方法) {
            结果.当前学习方法节点 = 当前方法;
            结果.已复用学习方法头 = true;
            私有_写入引用(学习子任务->主信息.执行方法, 当前方法);
            学习子任务->主信息.动作句柄 = 当前方法->主信息.动作句柄;
            return 结果.当前学习方法节点;
        }

        auto* 自我存在 = 自我对象.确保自我存在(调用点 + "/自我存在");
        if (!自我存在) {
            return nullptr;
        }

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        if (!方法根节点) {
            return nullptr;
        }

        auto* 方法头 = 私有_查找学习方法头候选(自我对象, 学习子任务, 宿主任务, 结果);
        if (方法头) {
            结果.已复用学习方法头 = true;
        }
        else {
            方法主信息类 主信息{};
            主信息.节点种类 = 枚举_方法节点种类::方法首节点;
            主信息.动作名 = 私有_学习方法动作名词(宿主任务, 结果);
            if (当前方法
                && 当前方法->主信息.动作句柄.类型 != 枚举_动作句柄类型::未设置) {
                主信息.动作句柄 = 当前方法->主信息.动作句柄;
            }
            if (auto* 期望状态 = 私有_学习方法条件目标状态(宿主任务, 结果)) {
                私有_写入引用(主信息.期望结果状态, 期望状态);
            }
            私有_写入引用(主信息.来源任务, 学习子任务);
            方法头 = 私有_独立树添加方法子节点(方法根节点, 主信息);
            结果.已创建学习方法头 = 方法头 != nullptr;
        }

        私有_补齐学习方法结构(
            自我对象,
            学习子任务,
            宿主任务,
            方法头,
            结果,
            调用点 + "/补齐结构");

        if (方法头) {
            私有_写入引用(学习子任务->主信息.执行方法, 方法头);
            学习子任务->主信息.动作句柄 = 方法头->主信息.动作句柄;
            结果.当前学习方法节点 = 方法头;
        }

        (void)now;
        return 方法头;
    }

    方法节点* 私有_查找学习方法直接子节点(
        const 方法节点* 方法头,
        枚举_方法节点种类 节点种类,
        const 词性节点类* 动作名,
        const 任务节点* 学习子任务) noexcept
    {
        if (!方法头 || !动作名) {
            return nullptr;
        }

        auto* 首节点 = static_cast<方法节点*>(方法头->子);
        if (!首节点) {
            return nullptr;
        }

        auto* 当前 = 首节点;
        do {
            if (当前
                && 当前->主信息.节点种类 == 节点种类
                && 私有_词性相同(当前->主信息.动作名, 动作名)
                && (!学习子任务
                    || reinterpret_cast<const void*>(当前->主信息.来源任务.指针)
                        == reinterpret_cast<const void*>(学习子任务))) {
                return 当前;
            }

            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    方法节点* 私有_创建或复用学习方法条件节点(
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        方法节点* 方法头,
        结构_任务管理结果& 结果) noexcept
    {
        if (!方法头 || !学习子任务) {
            return nullptr;
        }

        auto* 动作名 = 私有_学习方法条件节点动作名词(宿主任务, 结果);
        auto* 条件节点 = 私有_查找学习方法直接子节点(
            方法头,
            枚举_方法节点种类::方法条件节点,
            动作名,
            学习子任务);
        if (条件节点) {
            return 条件节点;
        }

        方法主信息类 主信息{};
        主信息.节点种类 = 枚举_方法节点种类::方法条件节点;
        主信息.动作名 = 动作名;
        主信息.条件场景 = 方法头->主信息.条件场景;
        私有_写入引用(主信息.来源任务, 学习子任务);
        私有_写入引用(主信息.父方法, 方法头);
        if (auto* 期望状态 = 私有_学习方法条件目标状态(宿主任务, 结果)) {
            私有_写入引用(主信息.期望结果状态, 期望状态);
        }
        条件节点 = 私有_独立树添加方法子节点(方法头, 主信息);
        结果.已创建学习方法条件节点 = 条件节点 != nullptr;
        return 条件节点;
    }

    方法节点* 私有_创建或复用学习方法结果节点(
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        方法节点* 方法头,
        结构_任务管理结果& 结果) noexcept
    {
        if (!方法头 || !学习子任务) {
            return nullptr;
        }

        auto* 动作名 = 私有_学习方法结果节点动作名词(宿主任务, 结果);
        auto* 结果节点 = 私有_查找学习方法直接子节点(
            方法头,
            枚举_方法节点种类::方法结果节点,
            动作名,
            学习子任务);
        if (结果节点) {
            return 结果节点;
        }

        方法主信息类 主信息{};
        主信息.节点种类 = 枚举_方法节点种类::方法结果节点;
        主信息.动作名 = 动作名;
        主信息.结果场景 = 方法头->主信息.结果场景;
        私有_写入引用(主信息.来源任务, 学习子任务);
        私有_写入引用(主信息.父方法, 方法头);
        if (auto* 期望状态 = 私有_学习方法结果目标状态(宿主任务, 结果)) {
            私有_写入引用(主信息.期望结果状态, 期望状态);
        }
        结果节点 = 私有_独立树添加方法子节点(方法头, 主信息);
        结果.已创建学习方法结果节点 = 结果节点 != nullptr;
        return 结果节点;
    }

    方法节点* 私有_写回学习方法骨架(
        自我类& 自我对象,
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        auto* 方法头 = 私有_创建或复用学习方法头(
            自我对象,
            宿主任务,
            学习子任务,
            结果,
            now,
            调用点 + "/方法头");
        if (!方法头) {
            return nullptr;
        }

        (void)私有_创建或复用学习方法条件节点(
            宿主任务,
            学习子任务,
            方法头,
            结果);
        (void)私有_创建或复用学习方法结果节点(
            宿主任务,
            学习子任务,
            方法头,
            结果);
        return 方法头;
    }

    bool 私有_同步学习任务实体(
        自我类& 自我对象,
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        方法节点* 学习方法,
        const 结构_学习承接决策& 学习决策,
        const 结构_任务管理结果& 结果,
        时间戳 now) noexcept
    {
        if (!学习子任务) {
            return false;
        }

        auto* 当前步骤 = 私有_P0_当前步骤节点(结果);
        auto* 最近结果 = 私有_P0_最新结果节点(结果);
        auto* 当前方法 = 私有_P0_当前方法(结果);
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前方法来源 = 私有_P0_当前方法来源(结果);
        const auto& 最近反馈摘要 = 私有_P0_最近反馈摘要(结果);

        结构_学习任务同步输入 输入{};
        输入.当前时间 = now;
        输入.去重键 = 学习子任务->获取主键();
        输入.学习子任务 = 学习子任务;
        输入.学习方法 = 学习方法;
        输入.来源宿主任务 = 宿主任务;
        输入.来源步骤 = 当前步骤;
        输入.来源结果 = 最近结果;
        输入.来源方法 = 当前方法;
        输入.最近根层重判结果 = 结果.最近根层重判结果;
        输入.当前缺口类型 = 当前缺口类型;
        输入.当前下一步去向 = 当前下一步去向;
        输入.当前方法来源 = 当前方法来源;
        输入.当前触发来源 = 结果.当前触发来源;
        输入.当前学习承接类型 = 学习决策.学习类型;
        输入.来源方法需求位 = 学习决策.来源方法需求位;
        输入.来源本能能力缺口类型 = 学习决策.来源本能能力缺口类型;
        输入.回流目标方法位 = 学习决策.回流目标方法位;
        const bool 本能补齐学习 = 学习决策.补原语或治理本能;
        输入.仅需求位方法头 = 本能补齐学习 ? false : 私有_学习方法仅需求位(学习方法);
        输入.当前安全值 = 自我对象.获取安全值();
        输入.当前服务值 = 自我对象.获取服务值();
        输入.当前风险安全 = 自我对象.获取风险安全值();
        输入.来源最小原语位图 = 学习决策.来源最小原语位图;
        输入.来源正式原语摘要 = 学习决策.来源正式原语摘要;
        输入.来源主观察特征 = 学习决策.来源主观察特征;
        输入.当前影子验证状态 = 结果.当前影子验证状态;
        输入.允许正式资产提交 = 学习决策.允许正式资产提交;
        输入.学习承接摘要 = 学习决策.决策摘要;
        输入.回流目标摘要 = 学习决策.回流目标摘要;
        输入.可直接进入学习 = 本能补齐学习
            ? (!输入.来源主观察特征.empty()
                || 输入.来源最小原语位图 != 0
                || !输入.回流目标摘要.empty())
            : 私有_学习方法可直接进入学习(学习方法);
        输入.最近反馈摘要 = 最近反馈摘要;
        return 学习任务模块::创建或更新学习任务(输入, nullptr);
    }

    bool 私有_推进步骤结果子链(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务) {
            return false;
        }

        const bool 有可回接最近结果 =
            结果.上下文.最近结果
            && 结果.上下文.最近结果->主信息.节点种类 == 枚举_任务节点种类::结果节点
            && 私有_任务节点归属宿主任务(结果.上下文.最近结果, 宿主任务);
        const bool 业务子任务转执行 = 私有_业务子任务应转入执行位(宿主任务, 结果);
        结果.当前步骤位类型 = 业务子任务转执行
            ? 枚举_任务管理步骤位类型::执行位
            : 私有_推导步骤位类型(结果, 有可回接最近结果);
        结果.当前步骤语义类型 = 业务子任务转执行
            ? 枚举_任务管理步骤语义类型::业务推进步骤
            : 私有_推导步骤语义类型(结果, 宿主任务);
        (void)私有_创建或复用补条件子任务头(
            自我对象,
            宿主任务,
            结果,
            now,
            调用点 + "/补条件子任务");

        auto* 最近结果 = 结果.上下文.最近结果;
        if (!业务子任务转执行 && 私有_结果节点匹配当前治理(最近结果, 宿主任务, 结果)) {
            结果.已命中最近结果 = true;
            结果.当前步骤位类型 = 私有_推导步骤位类型(结果, true);
            结果.当前步骤语义类型 = 私有_推导步骤语义类型(结果, 宿主任务);
            结果.最新结果节点 = 最近结果;
            auto* 当前步骤 = 最近结果 && 最近结果->父
                ? static_cast<任务节点*>(最近结果->父)
                : nullptr;
            if (当前步骤
                && 当前步骤->主信息.节点种类 == 枚举_任务节点种类::步骤节点
                && 私有_任务节点归属宿主任务(当前步骤, 宿主任务)) {
                结果.当前步骤节点 = 当前步骤;
                结果.上下文.当前步骤 = 当前步骤;
            }
            return false;
        }

        auto* 当前步骤 = 结果.上下文.当前步骤;
        const bool 可复用当前步骤 =
            当前步骤
            && 当前步骤->主信息.节点种类 == 枚举_任务节点种类::步骤节点
            && 私有_任务节点归属宿主任务(当前步骤, 宿主任务)
            && 当前步骤->子 == nullptr;

        if (可复用当前步骤) {
            结果.已复用当前步骤 = true;
            auto* 场景 = 宿主任务->主信息.场景.指针
                ? 宿主任务->主信息.场景.指针
                : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());
            auto* 父节点 = 当前步骤->父
                ? static_cast<任务节点*>(当前步骤->父)
                : 宿主任务;
            const bool 当前步骤回接最近结果 =
                父节点
                && 父节点->主信息.节点种类 == 枚举_任务节点种类::结果节点
                && 私有_任务节点归属宿主任务(父节点, 宿主任务);
            结果.当前步骤位类型 = 业务子任务转执行
                ? 枚举_任务管理步骤位类型::执行位
                : 私有_推导步骤位类型(结果, 当前步骤回接最近结果);
            结果.当前步骤语义类型 = 业务子任务转执行
                ? 枚举_任务管理步骤语义类型::业务推进步骤
                : 私有_推导步骤语义类型(结果, 宿主任务);
            私有_同步治理步骤节点(当前步骤, 父节点, 宿主任务, 结果, 场景, now);
            结果.当前步骤节点 = 当前步骤;
            结果.上下文.当前步骤 = 当前步骤;
        }
        else {
            结果.当前步骤位类型 = 业务子任务转执行
                ? 枚举_任务管理步骤位类型::执行位
                : 私有_推导步骤位类型(结果, 有可回接最近结果);
            结果.当前步骤语义类型 = 业务子任务转执行
                ? 枚举_任务管理步骤语义类型::业务推进步骤
                : 私有_推导步骤语义类型(结果, 宿主任务);
            当前步骤 = 私有_创建治理步骤节点(
                自我对象,
                宿主任务,
                结果,
                now,
                调用点 + "/创建步骤");
        }

        if (!当前步骤) {
            return 结果.已创建步骤节点;
        }

        auto* 结果节点 = 私有_创建治理结果节点(
            自我对象,
            宿主任务,
            当前步骤,
            结果,
            now,
            调用点 + "/创建结果");
        return 结果.已创建步骤节点 || 结果节点 != nullptr;
    }
}

namespace {
    bool 私有_写入宿主任务治理当前值_I64(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64 新值,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::宿主任务治理);
        auto* 特征节点 = 私有_确保宿主任务治理特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/任务虚拟存在");
        if (!特征节点) {
            return false;
        }
        I64 当前值 = 0;
        if (世界树.读取特征_I64(特征节点, 当前值) && 当前值 == 新值) {
            return false;
        }
        return 世界树.写入特征_I64(特征节点, 新值, now);
    }

    bool 私有_写入宿主任务治理当前值_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        return 私有_写入宿主任务治理当前值_I64(
            自我对象,
            自我对象.获取当前主任务(),
            特征类型,
            新值,
            now,
            调用点);
    }

    bool 私有_写入宿主任务治理当前值_指针(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::宿主任务治理);
        auto* 特征节点 = 私有_确保宿主任务治理特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/任务虚拟存在");
        if (!特征节点) {
            return false;
        }
        std::uintptr_t 当前值 = 0;
        const auto 新值 = reinterpret_cast<std::uintptr_t>(新指针值);
        if (私有_读取指针特征(特征节点, 当前值) && 当前值 == 新值) {
            return false;
        }
        return 世界树.写入特征_指针(特征节点, 新指针值, now);
    }

    bool 私有_写入宿主任务治理当前值_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        return 私有_写入宿主任务治理当前值_指针(
            自我对象,
            自我对象.获取当前主任务(),
            特征类型,
            新指针值,
            now,
            调用点);
    }
}

bool 任务管理任务模块::写入上位输入镜像(
    自我类& 自我对象,
    枚举_任务管理根层重判结果 根层重判结果,
    枚举_任务管理执行前门控结果 执行前门控结果,
    const std::string& 上层反馈摘要,
    时间戳 now,
    const std::string& 调用点) noexcept
{
    const auto 当前时间 = 私有_归一化时间(now);
    bool 已变化 = false;

    已变化 = 私有_写入宿主任务治理当前值_I64(
        自我对象,
        私有_管理特征_最近根层重判结果(),
        static_cast<I64>(根层重判结果),
        当前时间,
        调用点 + "/最近根层重判结果") || 已变化;

    已变化 = 私有_写入宿主任务治理当前值_I64(
        自我对象,
        私有_管理特征_最近执行前门控结果(),
        static_cast<I64>(执行前门控结果),
        当前时间,
        调用点 + "/最近执行前门控结果") || 已变化;

    const auto* 摘要词 = 上层反馈摘要.empty()
        ? nullptr
        : 语素集.添加词性词(上层反馈摘要, "短语");
    已变化 = 私有_写入宿主任务治理当前值_指针(
        自我对象,
        私有_管理特征_最近上层反馈摘要(),
        摘要词,
        当前时间,
        调用点 + "/最近上层反馈摘要") || 已变化;

    return 已变化;
}

结构_任务管理上下文 任务管理任务模块::读取任务管理上下文(
    const 自我类& 自我对象,
    时间戳 now) noexcept
{
    结构_任务管理上下文 上下文{};
    上下文.当前时间 = 私有_归一化时间(now);
    上下文.当前主需求 = 自我对象.获取当前主需求();
    上下文.当前管理任务 = 自我对象.获取任务管理任务();
    上下文.当前宿主任务 = 自我对象.获取当前主任务();
    上下文.当前方法 = 自我对象.获取当前主方法();
    上下文.当前步骤 = 私有_读取宿主任务治理任务节点指针特征(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_当前步骤指针(),
        "任务管理任务模块::读取任务管理上下文/当前步骤");
    上下文.最近结果 = 私有_读取宿主任务治理任务节点指针特征(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近结果指针(),
        "任务管理任务模块::读取任务管理上下文/最近结果");
    上下文.当前缺口类型 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理缺口类型>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_当前缺口类型(),
        "任务管理任务模块::读取任务管理上下文/当前缺口类型");
    上下文.当前下一步去向 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理下一步去向>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_当前下一步去向(),
        "任务管理任务模块::读取任务管理上下文/当前下一步去向");
    上下文.最近根层重判结果 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理根层重判结果>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近根层重判结果(),
        "任务管理任务模块::读取任务管理上下文/最近根层重判结果");
    上下文.最近执行前门控结果 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理执行前门控结果>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近执行前门控结果(),
        "任务管理任务模块::读取任务管理上下文/最近执行前门控结果");
    上下文.最近上层反馈摘要 = 私有_读取宿主任务治理反馈摘要特征(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近上层反馈摘要(),
        "任务管理任务模块::读取任务管理上下文/最近上层反馈摘要");
    上下文.最近功能域 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理功能域>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近功能域(),
        "任务管理任务模块::读取任务管理上下文/最近功能域");
    上下文.最近总控结果 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理总控结果>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近总控结果(),
        "任务管理任务模块::读取任务管理上下文/最近总控结果");
    上下文.最近反馈类型 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理反馈类型>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近反馈类型(),
        "任务管理任务模块::读取任务管理上下文/最近反馈类型");
    私有_按正式主体桥接结构补齐上下文(上下文);
    结构_任务管理主体虚拟存在 主体正式承载{};
    const bool 存在主体正式承载 =
        上下文.当前宿主任务
        && 任务管理任务模块::读取任务管理主体虚拟存在(
            上下文.当前宿主任务,
            &主体正式承载);
    const bool 主体正式承载已有本能缺口 =
        存在主体正式承载
        && 主体正式承载.最近本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义;
    const bool 主体正式承载已有治理态型 =
        存在主体正式承载
        && !主体正式承载.最近治理态型.empty();
    if (上下文.当前主体ID == 0) {
        上下文.当前主体ID = 私有_读取主体桥接U64文本特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前主体ID(),
            "任务管理任务模块::读取任务管理上下文/当前主体ID");
    }
    if (上下文.当前分身ID == 0) {
        上下文.当前分身ID = 私有_读取主体桥接U64文本特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前分身ID(),
            "任务管理任务模块::读取任务管理上下文/当前分身ID");
    }
    if (上下文.当前方法需求位 == 枚举_任务管理方法需求位::未定义
        && !存在主体正式承载) {
        上下文.当前方法需求位 = 私有_读取主体桥接I64枚举特征<枚举_任务管理方法需求位>(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前方法需求位(),
            "任务管理任务模块::读取任务管理上下文/当前方法需求位");
    }
    if (上下文.当前本能能力缺口类型 == 枚举_任务管理本能能力缺口类型::未定义
        && !主体正式承载已有本能缺口) {
        上下文.当前本能能力缺口类型 = 私有_读取主体桥接I64枚举特征<枚举_任务管理本能能力缺口类型>(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前本能能力缺口类型(),
            "任务管理任务模块::读取任务管理上下文/当前本能能力缺口类型");
    }
    if (上下文.当前输入条件主键.empty() && !存在主体正式承载) {
        上下文.当前输入条件主键 = 私有_读取主体桥接反馈摘要特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前输入条件主键(),
            "任务管理任务模块::读取任务管理上下文/当前输入条件主键");
    }
    if (上下文.当前治理态型.empty() && !主体正式承载已有治理态型) {
        上下文.当前治理态型 = 私有_读取主体桥接反馈摘要特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前治理态型(),
            "任务管理任务模块::读取任务管理上下文/当前治理态型");
    }
    if (上下文.当前方法位专项动态标题.empty() && !存在主体正式承载) {
        上下文.当前方法位专项动态标题 = 私有_读取主体桥接反馈摘要特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前方法位专项动态(),
            "任务管理任务模块::读取任务管理上下文/当前方法位专项动态");
    }
    if (上下文.当前最小原语动态标题.empty() && !存在主体正式承载) {
        上下文.当前最小原语动态标题 = 私有_读取主体桥接反馈摘要特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前最小原语动态(),
            "任务管理任务模块::读取任务管理上下文/当前最小原语动态");
    }
    上下文.最近反馈摘要 = 私有_读取宿主任务治理反馈摘要特征(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近反馈摘要(),
        "任务管理任务模块::读取任务管理上下文/最近反馈摘要");
    私有_按宿主清洗治理上下文镜像(上下文);
    return 上下文;
}

namespace {
    bool 私有_治理轮返回值(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.已补齐治理任务
            || 结果.已选中宿主任务
            || 结果.已绑定方法挂点
            || 结果.已推进任务状态
            || 结果.已创建步骤节点
            || 结果.已创建结果节点
            || 结果.已创建业务子任务
            || 结果.已创建学习子任务
            || 结果.已创建学习方法头
            || 结果.已创建学习方法条件节点
            || 结果.已创建学习方法结果节点
            || 结果.已确保任务管理主体虚拟存在
            || 结果.已初始化分身继承面
            || 结果.已刷新方法需求位
            || 结果.已评估本能能力缺口
            || 结果.已回并分身增量
            || 结果.已写回目标状态
            || 结果.已生成治理证据;
    }

    bool 私有_M2_提前结束治理轮(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果) noexcept
    {
        (void)私有_记录任务管理方法位专项动态(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/方法位动态/开始",
            结果,
            枚举_动作事件相位::开始运行);
        私有_写回治理稳定字段(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/稳定字段",
            结果);
        (void)私有_记录任务管理方法位专项动态(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/方法位动态/完成",
            结果,
            枚举_动作事件相位::完成);
        私有_刷新P0分阶段骨架(自我对象, 当前时间, 结果);
        结果.摘要 = 私有_治理摘要文本(结果);
        任务管理任务模块_治理缓存::缓存最近治理结果(结果);
        return false;
    }

    bool 私有_M2_受理请求阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        私有_刷新P0分阶段骨架(自我对象, 当前时间, 结果);
        结果.最近根层重判结果 = 结果.上下文.最近根层重判结果;
        结果.最近执行前门控结果 = 结果.上下文.最近执行前门控结果;
        结果.最近上层反馈摘要 = 结果.上下文.最近上层反馈摘要;

        if (!结果.上下文.当前管理任务) {
            (void)自我初始化模块::执行自我初始化(自我对象, 调用点 + "/补齐治理任务", nullptr);
            结果.当前请求 = {};
            私有_刷新P0分阶段骨架(自我对象, 当前时间, 结果);
            结果.已补齐治理任务 = 结果.上下文.当前管理任务 != nullptr;
            结果.最近根层重判结果 = 结果.上下文.最近根层重判结果;
            结果.最近执行前门控结果 = 结果.上下文.最近执行前门控结果;
            结果.最近上层反馈摘要 = 结果.上下文.最近上层反馈摘要;
        }

        阶段状态.当前管理任务 = 结果.上下文.当前管理任务;
        if (!阶段状态.当前管理任务) {
            结果.上下文.当前管理任务 = nullptr;
            结果.上下文.当前宿主任务 = nullptr;
            结果.上下文.当前方法 = nullptr;
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::筹办,
                枚举_任务管理缺口类型::治理任务缺失,
                枚举_任务管理下一步去向::回到重筹办,
                枚举_任务管理治理状态迁移::回到重筹办,
                枚举_任务管理总控结果::宿主待重筹办,
                枚举_任务管理反馈类型::重筹办提示,
                "任务管理任务缺失");
            return 私有_M2_提前结束治理轮(
                自我对象,
                当前时间,
                调用点 + "/受理请求",
                nullptr,
                结果);
        }

        if (阶段状态.当前管理任务->主信息.来源需求.指针 != 结果.上下文.当前主需求) {
            私有_写入引用(阶段状态.当前管理任务->主信息.来源需求, 结果.上下文.当前主需求);
        }
        (void)私有_迁移任务状态(阶段状态.当前管理任务, 枚举_任务状态::运行中, 当前时间);
        return true;
    }

    bool 私有_M2_宿主锚点确认阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        std::string 锚点裁决摘要{};
        阶段状态.宿主任务 = 私有_选择宿主任务(自我对象, 结果.上下文, &锚点裁决摘要);
        结果.锚点裁决摘要 = std::move(锚点裁决摘要);
        if (!阶段状态.宿主任务) {
            结果.上下文.当前宿主任务 = nullptr;
        }
        else if (阶段状态.宿主任务 != 结果.上下文.当前宿主任务) {
            结果.已选中宿主任务 = 自我对象.设置当前主任务(
                阶段状态.宿主任务,
                当前时间,
                调用点 + "/设置当前宿主任务");
            结果.上下文.当前宿主任务 = 阶段状态.宿主任务;
        }
        私有_按宿主清洗治理上下文镜像(结果.上下文);

        if (!阶段状态.宿主任务) {
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::筹办,
                枚举_任务管理缺口类型::宿主任务缺失,
                枚举_任务管理下一步去向::回到重筹办,
                枚举_任务管理治理状态迁移::回到重筹办,
                枚举_任务管理总控结果::宿主待重筹办,
                枚举_任务管理反馈类型::重筹办提示,
                "未选中宿主任务");
            return 私有_M2_提前结束治理轮(
                自我对象,
                当前时间,
                调用点 + "/宿主锚点确认",
                nullptr,
                结果);
        }

        结果.已确保任务管理主体虚拟存在 =
            任务管理任务模块::确保任务管理主体虚拟存在(
                自我对象,
                阶段状态.宿主任务,
                当前时间,
                &结果.当前主体);
        if (结果.已确保任务管理主体虚拟存在) {
            结果.当前主体ID = 结果.当前主体.主体ID;
            结果.上下文.当前主体ID = 结果.当前主体ID;
        }

        结果.宿主任务旧状态 = 阶段状态.宿主任务->主信息.状态;
        阶段状态.待回接业务子任务 =
            私有_查找待回接的已完成业务子任务(阶段状态.宿主任务, 结果.上下文);
        if (阶段状态.待回接业务子任务) {
            结果.当前子任务节点 = 阶段状态.待回接业务子任务;
        }
        return true;
    }

    void 私有_M2_上位输入约束阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        方法节点* 当前方法 = 私有_P0_当前方法(结果);
        阶段状态.已受上位输入约束 = 私有_应用上位输入约束(结果);
        if (!阶段状态.已受上位输入约束) {
            const bool 已绑定方法 = 私有_绑定宿主任务方法(
                自我对象,
                阶段状态.宿主任务,
                结果,
                当前时间,
                调用点 + "/绑定宿主方法",
                当前方法);
            结果.已绑定方法挂点 = 已绑定方法;
            结果.上下文.当前方法 = 当前方法;

            if (!当前方法 && 阶段状态.宿主任务->主信息.执行方法.指针) {
                当前方法 = reinterpret_cast<方法节点*>(阶段状态.宿主任务->主信息.执行方法.指针);
                结果.上下文.当前方法 = 当前方法;
            }

            if (当前方法 && 当前方法 != 自我对象.获取当前主方法()) {
                (void)自我对象.设置当前主方法(当前方法, 当前时间, 调用点 + "/同步当前主方法");
            }
        }
    }

    void 私有_M2_形成单步决策阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        if (结果.已确保任务管理主体虚拟存在) {
            结果.已初始化分身继承面 =
                任务管理任务模块::初始化任务管理分身继承面(
                    结果.上下文,
                    结果.当前主体,
                    &结果.当前分身);
            if (结果.已初始化分身继承面) {
                结果.当前分身.宿主任务 = 阶段状态.宿主任务;
                结果.当前分身.当前管理方法 = 私有_P0_当前方法(结果);
                结果.当前分身.当前步骤 = 结果.上下文.当前步骤;
                结果.当前分身.最近结果 = 结果.上下文.最近结果;
                结果.当前分身ID = 结果.当前分身.分身ID;
                结果.上下文.当前分身ID = 结果.当前分身ID;
                (void)私有_形成治理特征面(结果);
                (void)私有_命中方法需求位(结果);
                私有_应用半白盒桥接屏蔽(结果);
                (void)私有_评估本能能力缺口(自我对象, 结果);
            }
        }

        阶段状态.桥接补层允许继续 =
            阶段状态.已受上位输入约束
            || !结果.已初始化分身继承面
            || 私有_桥接补层裁决(结果);

        if (!阶段状态.桥接补层允许继续) {
            结果.当前写回结果.结构动作摘要 = 私有_P0_最近反馈摘要(结果);
        }

        if (阶段状态.桥接补层允许继续 && !阶段状态.已受上位输入约束 && !私有_P0_当前方法(结果)) {
            if (阶段状态.宿主任务->主信息.状态 != 枚举_任务状态::挂起) {
                结果.已推进任务状态 = 私有_迁移任务状态(阶段状态.宿主任务, 枚举_任务状态::挂起, 当前时间);
            }
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::筹办,
                枚举_任务管理缺口类型::方法挂点缺失,
                枚举_任务管理下一步去向::绑定方法,
                枚举_任务管理治理状态迁移::进入挂起,
                枚举_任务管理总控结果::宿主已挂起,
                枚举_任务管理反馈类型::重筹办提示,
                "宿主任务缺少方法挂点");
        }
        else if (阶段状态.桥接补层允许继续 && !阶段状态.已受上位输入约束) {
            switch (阶段状态.宿主任务->主信息.状态) {
            case 枚举_任务状态::完成:
            case 枚举_任务状态::失败:
            case 枚举_任务状态::取消:
                私有_设定治理结论(
                    结果,
                    枚举_任务管理功能域::收束,
                    枚举_任务管理缺口类型::任务已终结,
                    枚举_任务管理下一步去向::进入收束,
                    枚举_任务管理治理状态迁移::进入收束,
                    枚举_任务管理总控结果::宿主进入收束,
                    枚举_任务管理反馈类型::收束回执,
                    "宿主任务已终结，进入收束");
                break;
            case 枚举_任务状态::运行中:
                if (私有_业务子任务执行已完成(阶段状态.宿主任务, 结果)) {
                    结果.已推进任务状态 = 私有_迁移任务状态(阶段状态.宿主任务, 枚举_任务状态::完成, 当前时间)
                        || 结果.已推进任务状态;
                    私有_设定治理结论(
                        结果,
                        枚举_任务管理功能域::收束,
                        枚举_任务管理缺口类型::任务已终结,
                        枚举_任务管理下一步去向::进入收束,
                        枚举_任务管理治理状态迁移::进入收束,
                        枚举_任务管理总控结果::宿主进入收束,
                        枚举_任务管理反馈类型::收束回执,
                        "业务子任务已完成业务执行，进入收束");
                }
                else if (阶段状态.待回接业务子任务) {
                    私有_设定治理结论(
                        结果,
                        枚举_任务管理功能域::维护,
                        枚举_任务管理缺口类型::任务运行中,
                        枚举_任务管理下一步去向::保持运行,
                        枚举_任务管理治理状态迁移::保持运行,
                        枚举_任务管理总控结果::宿主保持运行,
                        枚举_任务管理反馈类型::治理推进,
                        "直接业务子任务已完成，回接父任务结果");
                }
                else if (私有_可进入等待分支(阶段状态.宿主任务, 结果)) {
                    私有_设定治理结论(
                        结果,
                        枚举_任务管理功能域::等待,
                        枚举_任务管理缺口类型::无缺口,
                        枚举_任务管理下一步去向::保持等待,
                        枚举_任务管理治理状态迁移::保持等待,
                        枚举_任务管理总控结果::宿主保持等待,
                        枚举_任务管理反馈类型::等待维持,
                        "当前无新增缺口，保持等待");
                }
                else {
                    私有_设定治理结论(
                        结果,
                        枚举_任务管理功能域::维护,
                        枚举_任务管理缺口类型::任务运行中,
                        枚举_任务管理下一步去向::保持运行,
                        枚举_任务管理治理状态迁移::保持运行,
                        枚举_任务管理总控结果::宿主保持运行,
                        枚举_任务管理反馈类型::治理推进,
                        "宿主任务运行中");
                }
                break;
            case 枚举_任务状态::挂起:
                结果.已推进任务状态 = 私有_迁移任务状态(阶段状态.宿主任务, 枚举_任务状态::运行中, 当前时间);
                私有_设定治理结论(
                    结果,
                    枚举_任务管理功能域::纠偏,
                    枚举_任务管理缺口类型::任务待启动,
                    枚举_任务管理下一步去向::进入运行,
                    枚举_任务管理治理状态迁移::进入运行,
                    枚举_任务管理总控结果::宿主已进入运行,
                    枚举_任务管理反馈类型::治理推进,
                    "宿主任务已推进到运行中");
                break;
            case 枚举_任务状态::未定义:
            case 枚举_任务状态::未启动:
            default:
                结果.已推进任务状态 = 私有_迁移任务状态(阶段状态.宿主任务, 枚举_任务状态::运行中, 当前时间);
                私有_设定治理结论(
                    结果,
                    枚举_任务管理功能域::维护,
                    枚举_任务管理缺口类型::任务待启动,
                    枚举_任务管理下一步去向::进入运行,
                    枚举_任务管理治理状态迁移::进入运行,
                    枚举_任务管理总控结果::宿主已进入运行,
                    枚举_任务管理反馈类型::治理推进,
                    "宿主任务已推进到运行中");
                break;
            }
        }

        结果.宿主任务新状态 = 阶段状态.宿主任务->主信息.状态;
        私有_同步学习承接决策(
            结果,
            私有_形成学习承接决策(
                阶段状态.当前管理任务,
                阶段状态.宿主任务,
                结果));
        私有_同步治理上下文镜像(结果);
    }

    void 私有_M2_形成写回结果阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept
    {
        auto* 当前证据方法 = 私有_P0_当前方法(结果);
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前宿主任务指针(),
            阶段状态.宿主任务,
            私有_任务标题(阶段状态.宿主任务),
            "任务管理_宿主任务切换",
            当前证据方法,
            阶段状态.宿主任务,
            当前时间,
            调用点 + "/证据/宿主任务",
            结果);
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前方法指针(),
            当前证据方法,
            私有_方法标题(当前证据方法),
            "任务管理_当前方法挂点",
            当前证据方法,
            阶段状态.宿主任务,
            当前时间,
            调用点 + "/证据/当前方法",
            结果);
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_宿主任务状态(),
            static_cast<I64>(结果.宿主任务新状态),
            "任务管理_宿主任务状态迁移",
            当前证据方法,
            阶段状态.宿主任务,
            当前时间,
            调用点 + "/证据/宿主任务状态",
            结果);
    }

    void 私有_M2_组装一步结果阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept
    {
        私有_完成治理轮收口(
            自我对象,
            阶段状态.宿主任务,
            当前时间,
            调用点,
            结果);
        私有_刷新P0分阶段骨架(自我对象, 当前时间, 结果);
        结果.摘要 = 私有_治理摘要文本(结果);
        任务管理任务模块_治理缓存::缓存最近治理结果(结果);
    }

    void 私有_M9_装配请求基本骨架(
        const 结构_任务管理上下文& 上下文,
        时间戳 当前时间,
        std::uint32_t 提交序号,
        结构_任务管理请求& 输出) noexcept
    {
        输出.宿主任务 = 上下文.当前宿主任务;
        输出.提交时间 = 当前时间;
        输出.提交序号 = 提交序号;
        输出.上下文 = 上下文;
        输出.请求ID = static_cast<std::uint64_t>(std::hash<std::string>{}(
            std::to_string(当前时间)
            + "|" + std::to_string(提交序号)
            + "|" + (上下文.当前宿主任务 ? 上下文.当前宿主任务->获取主键() : std::string("空宿主"))
            + "|" + (上下文.当前方法 ? 上下文.当前方法->获取主键() : std::string("空方法"))));
    }

    结构_M9请求触发态 私有_M9_消费请求触发态() noexcept
    {
        结构_M9请求触发态 触发态{};
        触发态.命中显式事件 = 私有_尝试消费治理显式事件控制(&触发态.显式事件);
        触发态.命中恢复重建 = 私有_尝试消费治理恢复重建控制(&触发态.恢复快照);
        触发态.半白盒控制 = 私有_读取半白盒治理控制();
        return 触发态;
    }

    void 私有_M9_装配请求触发信息(
        结构_任务管理请求& 输出,
        const 结构_M9请求触发态& 触发态) noexcept
    {
        if (触发态.命中恢复重建) {
            输出.触发事件类型 = 枚举_任务管理触发事件类型::恢复重建;
        }
        else if (触发态.命中显式事件) {
            输出.触发事件类型 = 触发态.显式事件.事件类型;
        }
        else {
            输出.触发事件类型 = 枚举_任务管理触发事件类型::普通推进;
        }

        输出.触发来源 =
            触发态.半白盒控制.启用
            ? 枚举_任务管理触发来源::半白盒
            : (
                触发态.命中显式事件
                ? 触发态.显式事件.触发来源
                : 枚举_任务管理触发来源::自然运行态
            );
        输出.来源最小原语位图 = 触发态.命中显式事件 ? 触发态.显式事件.来源最小原语位图 : 0;
        输出.来源主观察特征 =
            触发态.半白盒控制.启用
            ? 触发态.半白盒控制.来源主观察特征
            : 触发态.显式事件.来源主观察特征;
        输出.影子验证状态 =
            触发态.半白盒控制.启用
            ? (触发态.半白盒控制.影子验证状态.empty()
                ? std::string("未触发")
                : 触发态.半白盒控制.影子验证状态)
            : (触发态.显式事件.影子验证状态.empty()
                ? std::string("未触发")
                : 触发态.显式事件.影子验证状态);
        输出.允许正式资产提交 =
            触发态.半白盒控制.启用
            ? 触发态.半白盒控制.允许正式资产提交
            : 触发态.显式事件.允许正式资产提交;
        输出.触发事件摘要 = 触发态.命中显式事件 ? 触发态.显式事件.事件摘要 : std::string{};
        输出.当前恢复点类型.clear();
        输出.存在待消费学习回流 = false;
        输出.存在待消费外部反馈 = false;
        输出.恢复重建摘要.clear();
    }

    void 私有_M9_生成恢复重建请求摘要(
        结构_任务管理请求& 输出,
        const 结构_M9请求触发态& 触发态) noexcept
    {
        if (!触发态.命中恢复重建) {
            return;
        }

        const auto& 恢复快照 = 触发态.恢复快照;
        if (恢复快照.最近触发来源 != 枚举_任务管理触发来源::未定义) {
            输出.触发来源 = 恢复快照.最近触发来源;
        }
        if (输出.来源最小原语位图 == 0 && 恢复快照.最近来源最小原语位图 != 0) {
            输出.来源最小原语位图 = 恢复快照.最近来源最小原语位图;
        }
        if (输出.来源主观察特征.empty()) {
            输出.来源主观察特征 = 恢复快照.最近来源主观察特征;
        }
        输出.影子验证状态 =
            恢复快照.最近影子验证状态.empty()
            ? "未触发"
            : 恢复快照.最近影子验证状态;
        输出.允许正式资产提交 = 恢复快照.允许正式资产提交;
        输出.当前恢复点类型 = 恢复快照.当前恢复点类型;
        输出.存在待消费学习回流 = 恢复快照.存在待消费学习回流;
        输出.存在待消费外部反馈 = 恢复快照.存在待消费外部反馈;

        std::ostringstream 恢复摘要;
        恢复摘要 << "恢复点=" << (恢复快照.当前恢复点类型.empty() ? "未定义" : 恢复快照.当前恢复点类型)
            << " | 学习回流待消费=" << (恢复快照.存在待消费学习回流 ? "是" : "否")
            << " | 外部反馈待消费=" << (恢复快照.存在待消费外部反馈 ? "是" : "否")
            << " | 待回判事件=" << 恢复快照.待恢复治理事件数;
        if (!恢复快照.最近学习等待摘要.empty()) {
            恢复摘要 << " | 学习等待=" << 恢复快照.最近学习等待摘要;
        }
        else if (!恢复快照.最近学习承接摘要.empty()) {
            恢复摘要 << " | 学习承接=" << 恢复快照.最近学习承接摘要;
        }
        if (!恢复快照.最近学习回流摘要.empty()) {
            恢复摘要 << " | 学习回流=" << 恢复快照.最近学习回流摘要;
        }
        if (!恢复快照.待恢复治理事件摘要.empty()) {
            恢复摘要 << " | 待回判摘要=" << 恢复快照.待恢复治理事件摘要;
        }
        输出.恢复重建摘要 = 恢复摘要.str();
        if (输出.触发事件摘要.empty()) {
            输出.触发事件摘要 = 输出.恢复重建摘要;
        }
    }

    void 私有_M9_生成请求补充材料摘要(
        const 结构_任务管理上下文& 上下文,
        结构_任务管理请求& 输出) noexcept
    {
        std::ostringstream 摘要;
        摘要 << "宿主=" << 私有_任务标题(上下文.当前宿主任务)
            << " | 方法=" << 私有_方法标题(上下文.当前方法)
            << " | 事件=" << 私有_触发事件类型文本(输出.触发事件类型)
            << " | 根层=" << 私有_根层重判结果文本(上下文.最近根层重判结果)
            << " | 门控=" << 私有_执行前门控结果文本(上下文.最近执行前门控结果)
            << " | 缺口=" << 私有_缺口文本(上下文.当前缺口类型)
            << " | 去向=" << 私有_去向文本(上下文.当前下一步去向)
            << " | 主体ID=" << 上下文.当前主体ID
            << " | 分身ID=" << 上下文.当前分身ID
            << " | 需求位=" << 私有_方法需求位文本(上下文.当前方法需求位)
            << " | 本能缺口=" << 私有_本能能力缺口文本(上下文.当前本能能力缺口类型)
            << " | 触发来源=" << 私有_触发来源文本(输出.触发来源)
            << " | 主观察=" << (输出.来源主观察特征.empty() ? "空" : 输出.来源主观察特征)
            << " | 影子验证=" << (输出.影子验证状态.empty() ? "未触发" : 输出.影子验证状态)
            << " | 正式提交=" << (输出.允许正式资产提交 ? "允许" : "禁止");
        if (!输出.触发事件摘要.empty()) {
            摘要 << " | mailbox=" << 输出.触发事件摘要;
        }
        if (!输出.当前恢复点类型.empty()) {
            摘要 << " | 恢复点=" << 输出.当前恢复点类型;
        }
        if (输出.存在待消费学习回流 || 输出.存在待消费外部反馈) {
            摘要 << " | 待消费="
                << "学习回流=" << (输出.存在待消费学习回流 ? "是" : "否")
                << "/外部反馈=" << (输出.存在待消费外部反馈 ? "是" : "否");
        }
        if (!输出.恢复重建摘要.empty()) {
            摘要 << " | 恢复重建=" << 输出.恢复重建摘要;
        }
        输出.补充材料摘要 = 摘要.str();
    }

    std::string 私有_M10_主体长期策略摘要(
        const 结构_任务管理主体虚拟存在& 主体) noexcept
    {
        const auto 需求位索引 = static_cast<std::size_t>(主体.最近学习回流方法位);
        const auto 需求位学习回流次数 =
            需求位索引 < std::size(主体.需求位学习回流次数)
            ? 主体.需求位学习回流次数[需求位索引]
            : 0U;
        const auto 需求位正式资产次数 =
            需求位索引 < std::size(主体.需求位正式资产提交次数)
            ? 主体.需求位正式资产提交次数[需求位索引]
            : 0U;

        std::string 策略结论 = "继续等待更多回流证据";
        if (!私有_M10_长期策略可靠度允许回填(主体)) {
            策略结论 = "降低长期策略权重，等待新成功证据";
        }
        else if (需求位正式资产次数 >= 2 || 主体.累计正式方法资产提交次数 >= 3) {
            策略结论 = "优先复用已验证正式方法资产";
        }
        else if (主体.累计正式方法资产提交次数 > 0) {
            策略结论 = "优先沿用最近正式回流方法";
        }
        else if (主体.累计学习回流次数 > 0) {
            策略结论 = "保留回流候选，等待主循环继续复判";
        }

        const bool 最近失败更新更晚 =
            主体.最近长期策略失败时间 != 0
            && 主体.最近长期策略失败时间 >= 主体.最近长期策略成功时间;
        std::ostringstream 摘要;
        摘要 << "需求位=" << 私有_方法需求位文本(主体.最近学习回流方法位)
            << " | 学习回流=" << 主体.累计学习回流次数
            << " | 正式资产=" << 主体.累计正式方法资产提交次数
            << " | 需求位回流=" << 需求位学习回流次数
            << " | 需求位正式资产=" << 需求位正式资产次数
            << " | 稳定复现=" << 主体.累计稳定复现次数
            << " | 价值成立=" << 主体.累计价值成立次数
            << " | 成功=" << 主体.累计长期策略成功次数
            << " | 失败=" << 主体.累计长期策略失败次数
            << " | 权重=" << 主体.当前长期策略权重_Q10000
            << " | 降权=" << 主体.当前长期策略降权级别
            << " | 策略=" << 策略结论;
        if (!主体.最近正式方法标题.empty()) {
            摘要 << " | 最近方法=" << 主体.最近正式方法标题;
        }
        if (!主体.最近长期策略条件签名.empty()) {
            摘要 << " | 条件=" << 主体.最近长期策略条件签名;
        }
        if (!主体.长期策略条件签名范围摘要.empty()) {
            摘要 << " | 条件范围=" << 主体.长期策略条件签名范围摘要;
        }
        if (!主体.最近长期策略业务差额签名.empty()) {
            摘要 << " | 差额=" << 主体.最近长期策略业务差额签名;
        }
        if (!主体.长期策略业务差额范围摘要.empty()) {
            摘要 << " | 差额范围=" << 主体.长期策略业务差额范围摘要;
        }
        if (最近失败更新更晚 && !主体.最近长期策略失效原因.empty()) {
            摘要 << " | 最近失效=" << 主体.最近长期策略失效原因;
        }
        return 摘要.str();
    }

    void 私有_M10_回写长期策略可靠度(
        任务节点* 宿主任务,
        时间戳 当前时间,
        结构_任务管理结果& 结果) noexcept
    {
        if (!宿主任务) {
            return;
        }

        结构_任务管理主体虚拟存在 主体 = 结果.当前主体;
        if (主体.主体ID == 0) {
            if (!任务管理任务模块::读取任务管理主体虚拟存在(
                reinterpret_cast<任务类::节点类*>(宿主任务),
                &主体)) {
                return;
            }
        }

        const bool 当前轮命中长期策略 =
            结果.当前方法来源 == 枚举_任务管理方法来源::主体长期策略回填
            || 私有_M10_当前方法命中主体长期策略(结果)
            || 结果.最近反馈摘要.find("长期策略驱动业务子任务=是") != std::string::npos;
        if (!当前轮命中长期策略 || 主体.累计正式方法资产提交次数 == 0) {
            return;
        }

        const auto 条件签名 = 私有_M10_当前条件签名(结果);
        const auto 业务差额签名 = 私有_M10_业务差额签名(宿主任务, 结果);
        主体.最近长期策略条件签名 = 条件签名;
        主体.最近长期策略业务差额签名 = 业务差额签名;
        私有_M10_合并长期策略范围签名(主体.长期策略条件签名范围摘要, 条件签名);
        私有_M10_合并长期策略范围签名(主体.长期策略业务差额范围摘要, 业务差额签名);

        if (私有_M10_当前轮形成有效长期策略推进(结果)) {
            主体.累计长期策略成功次数 += 1;
            主体.最近长期策略成功时间 = 当前时间;
        }
        else {
            主体.累计长期策略失败次数 += 1;
            主体.最近长期策略失败时间 = 当前时间;
            主体.最近长期策略失效原因 = 私有_M10_当前轮长期策略失效原因(结果);
        }

        私有_M10_刷新长期策略权重(主体);
        主体.最近长期策略摘要 = 私有_M10_主体长期策略摘要(主体);

        {
            std::scoped_lock 锁(私有_任务管理主体表锁);
            私有_任务管理主体表[主体.主体ID] = 主体;
        }

        结果.当前主体 = 主体;
        if (!主体.最近长期策略摘要.empty()) {
            结果.当前写回结果.主体回并摘要 = 主体.最近长期策略摘要;
            结果.当前一步结果.主体回并摘要 = 主体.最近长期策略摘要;
        }
        if (!主体.最近长期策略失效原因.empty()
            && 主体.最近长期策略失败时间 >= 主体.最近长期策略成功时间) {
            私有_追加最近反馈片段(
                结果,
                "长期策略失效原因=" + 主体.最近长期策略失效原因);
        }
    }

    bool 私有_M10_沉淀正式学习回流到主体(
        自我类& 自我对象,
        const 结构_学习方法资产提交输入& 输入,
        const 结构_学习方法资产提交结果& 资产结果,
        时间戳 当前时间,
        结构_任务管理主体虚拟存在* 输出主体,
        std::string* 输出摘要) noexcept
    {
        if (!输入.来源宿主任务 || !资产结果.已写回正式方法资产) {
            return false;
        }

        结构_任务管理主体虚拟存在 主体{};
        if (!任务管理任务模块::确保任务管理主体虚拟存在(
            自我对象,
            输入.来源宿主任务,
            当前时间,
            &主体)) {
            return false;
        }

        const auto 需求位索引 = static_cast<std::size_t>(输入.来源方法需求位);
        主体.最近学习回流时间 = 当前时间;
        主体.最近学习回流方法位 = 输入.来源方法需求位;
        主体.最近主方法需求位 = 输入.来源方法需求位;
        主体.最近宿主任务 = 输入.来源宿主任务;
        主体.最近管理方法 =
            资产结果.当前学习方法节点
            ? 资产结果.当前学习方法节点
            : 输入.来源方法;
        主体.最近原因摘要 =
            !输入.学习承接摘要.empty()
            ? 输入.学习承接摘要
            : 资产结果.摘要;
        主体.最近学习回流摘要 =
            !输入.回流目标摘要.empty()
            ? 输入.回流目标摘要
            : 资产结果.摘要;
        主体.最近正式方法标题 = 资产结果.正式方法标题;
        主体.最近长期策略条件签名 =
            !输入.来源主观察特征.empty()
            ? 输入.来源主观察特征
            : 主体.最近输入条件主键;
        {
            结构_任务管理结果 临时结果{};
            主体.最近长期策略业务差额签名 =
                私有_M10_业务差额签名(
                    reinterpret_cast<任务节点*>(输入.来源宿主任务),
                    临时结果);
        }
        私有_M10_合并长期策略范围签名(
            主体.长期策略条件签名范围摘要,
            主体.最近长期策略条件签名);
        私有_M10_合并长期策略范围签名(
            主体.长期策略业务差额范围摘要,
            主体.最近长期策略业务差额签名);
        主体.累计学习回流次数 += 1;
        主体.累计正式方法资产提交次数 += 1;
        主体.累计稳定复现次数 += 1;
        if (资产结果.已创建学习方法结果节点) {
            主体.累计价值成立次数 += 1;
        }
        if (需求位索引 < std::size(主体.需求位学习回流次数)) {
            主体.需求位学习回流次数[需求位索引] += 1;
            主体.需求位正式资产提交次数[需求位索引] += 1;
            主体.需求位验证证据次数[需求位索引] += 1;
            主体.需求位升格证据次数[需求位索引] += 1;
        }
        私有_M10_刷新长期策略权重(主体);
        主体.最近长期策略摘要 = 私有_M10_主体长期策略摘要(主体);

        {
            std::scoped_lock 锁(私有_任务管理主体表锁);
            私有_任务管理主体表[主体.主体ID] = 主体;
        }

        if (输出主体) {
            *输出主体 = 主体;
        }
        if (输出摘要) {
            *输出摘要 = 主体.最近长期策略摘要;
        }
        return true;
    }

    bool 私有_M9_执行前段治理阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        if (!私有_M2_受理请求阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态)) {
            return false;
        }
        if (!私有_M2_宿主锚点确认阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态)) {
            return false;
        }
        私有_M2_上位输入约束阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态);
        私有_M2_形成单步决策阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态);
        私有_M2_形成写回结果阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态);
        return true;
    }

    void 私有_追加最近反馈片段(
        结构_任务管理结果& 结果,
        const std::string& 片段) noexcept
    {
        if (片段.empty()) {
            return;
        }
        if (!结果.最近反馈摘要.empty()) {
            结果.最近反馈摘要 += "；";
        }
        结果.最近反馈摘要 += 片段;
    }

    bool 私有_M9_事件轮禁止业务扩张(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前请求.触发事件类型 == 枚举_任务管理触发事件类型::学习回流
            || 结果.当前请求.触发事件类型 == 枚举_任务管理触发事件类型::外部反馈
            || 结果.当前请求.触发事件类型 == 枚举_任务管理触发事件类型::恢复重建
            || 结果.当前下一步去向 == 枚举_任务管理下一步去向::保持等待
            || 结果.当前下一步去向 == 枚举_任务管理下一步去向::进入收束;
    }

    void 私有_M9_执行后段治理动作阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept
    {
        auto* 宿主任务 = 阶段状态.宿主任务;
        auto* 待回接业务子任务 = 阶段状态.待回接业务子任务;
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前功能域 = 私有_P0_当前功能域(结果);
        const bool 允许推进步骤结果子链 =
            当前下一步去向 != 枚举_任务管理下一步去向::进入收束
            || 当前功能域 == 枚举_任务管理功能域::等待;
        if (允许推进步骤结果子链) {
            (void)私有_推进步骤结果子链(
                自我对象,
                宿主任务,
                结果,
                当前时间,
                调用点 + "/步骤结果子链");
        }

        if (!结果.当前步骤节点) {
            结果.当前步骤节点 = 结果.上下文.当前步骤;
        }
        if (!结果.最新结果节点) {
            结果.最新结果节点 = 结果.上下文.最近结果;
        }
        if (结果.已创建补条件子任务) {
            私有_追加最近反馈片段(结果, "已创建补条件子任务=" + 私有_任务标题(结果.当前子任务节点));
        }
        if (结果.已复用补条件子任务) {
            私有_追加最近反馈片段(结果, "已复用补条件子任务=" + 私有_任务标题(结果.当前子任务节点));
        }

        const auto 当前方法来源 = 私有_P0_当前方法来源(结果);
        const auto 当前步骤位类型 = 私有_P0_当前步骤位类型(结果);
        if (当前方法来源 != 枚举_任务管理方法来源::未定义) {
            私有_追加最近反馈片段(结果, "方法来源=" + std::string(私有_方法来源文本(当前方法来源)));
        }
        if (当前步骤位类型 != 枚举_任务管理步骤位类型::未定义) {
            私有_追加最近反馈片段(结果, "步骤位=" + std::string(私有_步骤位类型文本(当前步骤位类型)));
        }

        (void)私有_写回任务目标状态(
            自我对象,
            宿主任务,
            结果,
            当前时间,
            调用点 + "/目标状态");

        (void)私有_写回任务结果状态(
            自我对象,
            宿主任务,
            结果,
            当前时间,
            调用点 + "/结果状态");

        私有_整理任务双面摘要(宿主任务, 结果);
        if (待回接业务子任务) {
            if (结果.宿主目标结果已对齐) {
                私有_设定治理结论(
                    结果,
                    枚举_任务管理功能域::等待,
                    枚举_任务管理缺口类型::无缺口,
                    枚举_任务管理下一步去向::保持等待,
                    枚举_任务管理治理状态迁移::保持等待,
                    枚举_任务管理总控结果::宿主保持等待,
                    枚举_任务管理反馈类型::等待维持,
                    "直接业务子任务已完成，父任务结果已对齐，进入等待");
            }
            else if (结果.宿主目标结果待推进) {
                私有_设定治理结论(
                    结果,
                    枚举_任务管理功能域::维护,
                    枚举_任务管理缺口类型::任务运行中,
                    枚举_任务管理下一步去向::保持运行,
                    枚举_任务管理治理状态迁移::保持运行,
                    枚举_任务管理总控结果::宿主保持运行,
                    枚举_任务管理反馈类型::治理推进,
                    "直接业务子任务已完成，但父任务仍有差额，继续派生下一段业务任务");
            }
            私有_同步治理上下文镜像(结果);
        }
        私有_整理后果治理(结果, 待回接业务子任务 != nullptr);
        私有_同步治理上下文镜像(结果);
        if (结果.已由目标差额驱动 && 结果.当前步骤语义类型 == 枚举_任务管理步骤语义类型::治理步骤) {
            结果.当前步骤语义类型 = 枚举_任务管理步骤语义类型::业务推进步骤;
        }
    }

    void 私有_M9_执行学习承接与业务派生阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept
    {
        auto* 当前管理任务 = 阶段状态.当前管理任务;
        auto* 宿主任务 = 阶段状态.宿主任务;
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const bool 事件轮禁止业务扩张 = 私有_M9_事件轮禁止业务扩张(结果);
        const bool 长期策略驱动业务派生 = 私有_M10_长期策略驱动业务子任务派生(结果);
        const auto 业务子任务复用键 = 私有_业务子任务键(宿主任务, 结果);
        if (长期策略驱动业务派生) {
            私有_追加最近反馈片段(结果, "长期策略驱动业务子任务=是");
            私有_追加最近反馈片段(结果, "业务子任务复用键=" + 业务子任务复用键);
        }
        if (事件轮禁止业务扩张) {
            私有_追加最近反馈片段(结果, "事件轮禁止继续扩张业务子任务");
            if (长期策略驱动业务派生) {
                私有_追加最近反馈片段(结果, "业务子任务决策=禁止扩张");
            }
        }
        else {
            auto* 业务子任务 = 私有_创建或复用业务子任务头(
                自我对象,
                宿主任务,
                结果,
                当前时间,
                调用点 + "/业务子任务");
            if (业务子任务 && (结果.已创建业务子任务 || 结果.已复用业务子任务)) {
                结果.当前写回结果.当前业务子任务 = 业务子任务;
            }
            if (长期策略驱动业务派生) {
                if (结果.已复用业务子任务) {
                    私有_追加最近反馈片段(结果, "业务子任务决策=复用");
                }
                else if (结果.已创建业务子任务) {
                    私有_追加最近反馈片段(结果, "业务子任务决策=新建");
                }
                else {
                    私有_追加最近反馈片段(结果, "业务子任务决策=未命中");
                }
            }
        }

        const auto 学习承接决策 = 私有_形成学习承接决策(
            当前管理任务,
            宿主任务,
            结果);
        私有_同步学习承接决策(结果, 学习承接决策);
        auto* 学习子任务 = 学习承接决策.是否触发学习
            ? 私有_创建或复用学习子任务头(
                自我对象,
                当前管理任务,
                宿主任务,
                结果,
                当前时间,
                调用点 + "/学习子任务")
            : nullptr;
        auto* 学习方法 = (学习承接决策.是否触发学习 && !学习承接决策.补原语或治理本能)
            ? 私有_写回学习方法骨架(
                自我对象,
                宿主任务,
                学习子任务,
                结果,
                当前时间,
                调用点 + "/学习方法")
            : nullptr;
        if (学习承接决策.是否触发学习) {
            (void)私有_同步学习任务实体(
                自我对象,
                宿主任务,
                学习子任务,
                学习方法,
                学习承接决策,
                结果,
                当前时间);
        }
        结果.当前写回结果.当前学习子任务 = 学习子任务;
        结果.当前写回结果.当前学习方法 = 学习方法;
        I64 待学习方法数量 = 学习任务模块::统计活跃学习任务数量();
        if (待学习方法数量 <= 0) {
            待学习方法数量 = 私有_统计待学习方法数量(当前管理任务);
        }
        (void)自我对象.更新待学习方法数量(
            待学习方法数量,
            当前时间,
            调用点 + "/学习计数");
        (void)自我对象.更新尝试学习状态(
            学习任务模块::统计活跃学习任务数量() > 0,
            当前时间,
            调用点 + "/学习状态");
        if (!结果.目标结果差额摘要.empty()) {
            私有_追加最近反馈片段(结果, 结果.目标结果差额摘要);
        }
        if (结果.已创建业务子任务) {
            私有_追加最近反馈片段(结果, "已创建业务子任务=" + 私有_任务标题(结果.当前子任务节点));
        }
        if (结果.已复用业务子任务) {
            私有_追加最近反馈片段(结果, "已复用业务子任务=" + 私有_任务标题(结果.当前子任务节点));
        }
        if (结果.已创建学习子任务) {
            私有_追加最近反馈片段(结果, "已创建学习子任务=" + 私有_任务标题(学习子任务));
        }
        if (结果.已复用学习子任务) {
            私有_追加最近反馈片段(结果, "已复用学习子任务=" + 私有_任务标题(学习子任务));
        }
        if (!学习承接决策.决策摘要.empty()) {
            私有_追加最近反馈片段(结果, 学习承接决策.决策摘要);
        }
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        if ((结果.已创建学习子任务 || 结果.已复用学习子任务)
            && 当前缺口类型 == 枚举_任务管理缺口类型::无缺口
            && 当前下一步去向 == 枚举_任务管理下一步去向::保持等待) {
            私有_追加最近反馈片段(结果, "稳定等待态已触发方法复盘学习");
        }
        if ((结果.已创建学习子任务 || 结果.已复用学习子任务) && 学习承接决策.补原语或治理本能) {
            私有_追加最近反馈片段(结果, "当前学习通道=本能补齐/治理本能组合");
        }
        if (结果.已创建学习方法头) {
            私有_追加最近反馈片段(结果, "已创建学习方法头=" + 私有_方法标题(学习方法));
        }
        if (结果.已复用学习方法头) {
            私有_追加最近反馈片段(结果, "已复用学习方法头=" + 私有_方法标题(学习方法));
        }
        if (结果.已创建学习方法条件节点) {
            私有_追加最近反馈片段(结果, "已创建学习方法条件节点");
        }
        if (结果.已创建学习方法结果节点) {
            私有_追加最近反馈片段(结果, "已创建学习方法结果节点");
        }
        if (!结果.最近上层反馈摘要.empty()) {
            私有_追加最近反馈片段(结果, "上层反馈=" + 结果.最近上层反馈摘要);
        }
    }
}

bool 任务管理任务模块::设置半白盒治理控制(
    const 结构_任务管理半白盒控制& 控制) noexcept
{
    std::scoped_lock 锁(私有_半白盒治理控制锁);
    私有_半白盒治理控制 = 控制;
    私有_半白盒治理控制.启用 =
        控制.启用
        && 控制.屏蔽类型 != 枚举_任务管理半白盒屏蔽类型::未定义;
    return true;
}

void 任务管理任务模块::清除半白盒治理控制() noexcept
{
    std::scoped_lock 锁(私有_半白盒治理控制锁);
    私有_半白盒治理控制 = {};
}

bool 任务管理任务模块::设置治理显式事件控制(
    const 结构_任务管理显式事件控制& 控制) noexcept
{
    std::scoped_lock 锁(私有_治理显式事件控制锁);
    私有_治理显式事件控制 = 控制;
    私有_治理显式事件控制.启用 =
        控制.启用
        && 控制.事件类型 != 枚举_任务管理触发事件类型::未定义;
    return true;
}

void 任务管理任务模块::清除治理显式事件控制() noexcept
{
    std::scoped_lock 锁(私有_治理显式事件控制锁);
    私有_治理显式事件控制 = {};
}

bool 任务管理任务模块::设置治理恢复重建控制(
    const 结构_治理恢复快照& 快照,
    const std::string& 调用点) noexcept
{
    (void)调用点;
    std::scoped_lock 锁(私有_治理恢复重建控制锁);
    私有_治理恢复重建控制 = {};
    私有_治理恢复重建控制.启用 = true;
    私有_治理恢复重建控制.快照 = 快照;
    return true;
}

void 任务管理任务模块::清除治理恢复重建控制() noexcept
{
    std::scoped_lock 锁(私有_治理恢复重建控制锁);
    私有_治理恢复重建控制 = {};
}

bool 任务管理任务模块::构造任务管理请求(
    const 自我类& 自我对象,
    时间戳 now,
    结构_任务管理请求* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    const auto 当前时间 = 私有_归一化时间(now);
    const auto 上下文 = 读取任务管理上下文(自我对象, 当前时间);

    std::uint32_t 提交序号 = 0;
    {
        std::scoped_lock 锁(私有_任务管理请求序号锁);
        提交序号 = ++私有_任务管理请求序号;
    }

    私有_M9_装配请求基本骨架(上下文, 当前时间, 提交序号, *输出);
    const auto 触发态 = 私有_M9_消费请求触发态();
    私有_M9_装配请求触发信息(*输出, 触发态);
    私有_M9_生成恢复重建请求摘要(*输出, 触发态);
    私有_M9_生成请求补充材料摘要(上下文, *输出);
    return true;
}

bool 任务管理任务模块::形成任务管理单步决策(
    const 结构_任务管理请求& 请求,
    结构_任务管理单步决策* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    结构_任务管理分身继承面 临时分身{};
    临时分身.宿主任务 = 请求.上下文.当前宿主任务;
    临时分身.当前步骤 = 请求.上下文.当前步骤;
    临时分身.最近结果 = 请求.上下文.最近结果;
    临时分身.当前管理方法 = 请求.上下文.当前方法;
    临时分身.本轮输入条件主键 = 私有_主体化输入条件主键(请求.上下文, 临时分身);
    临时分身.本轮结果主键 = 私有_主体化结果主键(临时分身, 请求.上下文);

    输出->当前功能域 = 请求.上下文.最近功能域;
    输出->当前缺口类型 = 请求.上下文.当前缺口类型;
    输出->当前下一步去向 = 请求.上下文.当前下一步去向;
    输出->当前治理状态迁移 = 枚举_任务管理治理状态迁移::未定义;
    输出->当前方法来源 = 枚举_任务管理方法来源::未定义;
    输出->当前方法需求位 = 私有_推导任务管理方法需求位(请求.上下文, 临时分身);
    输出->当前本能能力缺口类型 = 私有_P0_推导本能能力缺口(请求.上下文, 输出->当前方法需求位);
    输出->当前触发来源 = 请求.触发来源;
    输出->当前步骤位类型 = 私有_P0_推导步骤位类型(请求.上下文);
    输出->当前步骤语义类型 = 私有_P0_推导步骤语义类型(请求.上下文, 输出->当前步骤位类型);
    输出->当前后果治理 =
        请求.上下文.当前下一步去向 == 枚举_任务管理下一步去向::进入收束
        ? 枚举_任务管理后果治理::进入收束
        : (
            请求.上下文.当前下一步去向 == 枚举_任务管理下一步去向::保持等待
            ? 枚举_任务管理后果治理::保持等待
            : 枚举_任务管理后果治理::无额外后果
        );
    输出->来源最小原语位图 = 请求.来源最小原语位图;
    输出->来源主观察特征 = 请求.来源主观察特征;
    输出->影子验证状态 = 请求.影子验证状态;
    输出->允许正式资产提交 = 请求.允许正式资产提交;

    std::ostringstream 摘要;
    摘要 << "功能域=" << 私有_功能域文本(输出->当前功能域)
        << " | 缺口=" << 私有_缺口文本(输出->当前缺口类型)
        << " | 去向=" << 私有_去向文本(输出->当前下一步去向)
        << " | 需求位=" << 私有_方法需求位文本(输出->当前方法需求位)
        << " | 本能缺口=" << 私有_本能能力缺口文本(输出->当前本能能力缺口类型)
        << " | 触发来源=" << 私有_触发来源文本(输出->当前触发来源)
        << " | 本轮原语命中=" << (输出->来源最小原语位图 == 0 ? std::string("空") : 私有_原语位图摘要(输出->来源最小原语位图))
        << " | 主观察=" << (输出->来源主观察特征.empty() ? "空" : 输出->来源主观察特征)
        << " | 影子验证=" << (输出->影子验证状态.empty() ? "未触发" : 输出->影子验证状态)
        << " | 正式提交=" << (输出->允许正式资产提交 ? "允许" : "禁止");
    输出->决策摘要 = 摘要.str();
    return true;
}

bool 任务管理任务模块::形成任务管理写回结果(
    自我类&,
    const 结构_任务管理请求& 请求,
    const 结构_任务管理单步决策& 决策,
    结构_任务管理写回结果* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    输出->当前步骤节点 = 请求.上下文.当前步骤;
    输出->最新结果节点 = 请求.上下文.最近结果;
    输出->当前业务子任务 = nullptr;
    输出->当前补条件子任务 = nullptr;
    输出->当前学习子任务 = nullptr;
    输出->当前学习方法 = nullptr;
    输出->宿主目标状态 = 请求.上下文.当前宿主任务 ? 请求.上下文.当前宿主任务->主信息.目标状态.指针 : nullptr;
    输出->宿主结果状态 = 请求.上下文.当前宿主任务 ? 请求.上下文.当前宿主任务->主信息.结果状态信息.指针 : nullptr;
    输出->双面摘要 = {};
    输出->结构动作摘要 =
        "桥接补层写回="
        + 私有_去向文本(决策.当前下一步去向)
        + " | 需求位="
        + 私有_方法需求位文本(决策.当前方法需求位)
        + " | 本能缺口="
        + 私有_本能能力缺口文本(决策.当前本能能力缺口类型);
    输出->主体回并摘要 = {};
    return true;
}

bool 任务管理任务模块::组装任务管理一步结果(
    const 结构_任务管理请求& 请求,
    const 结构_任务管理单步决策& 决策,
    const 结构_任务管理写回结果& 写回,
    结构_任务管理一步结果* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    输出->宿主任务 = 请求.宿主任务;
    输出->当前主体ID = 请求.上下文.当前主体ID;
    输出->当前分身ID = 请求.上下文.当前分身ID;
    输出->当前功能域 = 决策.当前功能域;
    输出->当前缺口类型 = 决策.当前缺口类型;
    输出->当前下一步去向 = 决策.当前下一步去向;
    输出->当前治理状态迁移 = 决策.当前治理状态迁移;
    输出->当前方法需求位 = 决策.当前方法需求位;
    输出->当前本能能力缺口类型 = 决策.当前本能能力缺口类型;
    输出->当前触发来源 = 决策.当前触发来源;
    输出->当前步骤节点 = 写回.当前步骤节点;
    输出->当前方法节点 = 请求.上下文.当前方法;
    输出->最新结果节点 = 写回.最新结果节点;
    输出->最近总控结果 = 请求.上下文.最近总控结果;
    输出->最近反馈类型 = 请求.上下文.最近反馈类型;
    输出->当前输入条件主键 = 请求.上下文.当前输入条件主键;
    输出->当前治理态型 = 请求.上下文.当前治理态型;
    输出->来源最小原语位图 = 决策.来源最小原语位图;
    输出->来源主观察特征 = 决策.来源主观察特征;
    输出->影子验证状态 = 决策.影子验证状态;
    输出->允许正式资产提交 = 决策.允许正式资产提交;
    输出->已触发学习承接 =
        写回.当前学习子任务 != nullptr || 写回.当前学习方法 != nullptr;
    输出->当前学习承接类型 = 输出->已触发学习承接
        ? 枚举_任务管理学习承接类型::定向学习
        : 枚举_任务管理学习承接类型::未定义;
    输出->当前学习为本能补齐 =
        输出->已触发学习承接
        && 私有_本能缺口属于原语或治理补齐(决策.当前本能能力缺口类型);
    if (输出->已触发学习承接) {
        std::ostringstream 学习摘要;
        学习摘要 << "学习承接=已放行"
            << " | 类型=" << 私有_学习承接类型文本(输出->当前学习承接类型)
            << " | 通道=" << (输出->当前学习为本能补齐 ? "补原语/治理本能" : "补方法");
        输出->学习承接摘要 = 学习摘要.str();
    }
    输出->主体回并摘要 = 写回.主体回并摘要;
    输出->最近反馈摘要 =
        !写回.结构动作摘要.empty() ? 写回.结构动作摘要 : 请求.上下文.最近反馈摘要;
    return true;
}

bool 任务管理任务模块::确保任务管理主体虚拟存在(
    自我类& 自我对象,
    任务类::节点类* 宿主任务,
    时间戳 now,
    结构_任务管理主体虚拟存在* 输出) noexcept
{
    auto* 锚点宿主 = 宿主任务 ? reinterpret_cast<任务节点*>(宿主任务) : 自我对象.获取当前主任务();
    if (!锚点宿主) {
        return false;
    }

    const auto 当前时间 = 私有_归一化时间(now);
    const auto 主体ID = 私有_主体ID_按宿主任务(锚点宿主);
    if (主体ID == 0) {
        return false;
    }

    std::scoped_lock 锁(私有_任务管理主体表锁);
    auto& 主体 = 私有_任务管理主体表[主体ID];
    if (主体.主体ID == 0) {
        主体 = {};
        主体.创建时间 = 当前时间;
        主体.主体ID = 主体ID;
        主体.版本号 = 1;
    }

    主体.最近激活时间 = 当前时间;
    主体.最近宿主任务 = 锚点宿主;
    主体.安全取向_Q10000 = 私有_Q10000比率(自我对象.获取安全值(), 自我对象.根目标安全值());
    主体.服务取向_Q10000 = 私有_Q10000比率(自我对象.获取服务值(), 自我对象.根目标服务值());
    主体.当前预算_Q10000 = 主体.服务取向_Q10000;
    主体.风险偏保守 =
        自我对象.是否待机状态()
        || 自我对象.获取安全值() <= (自我对象.根目标安全值() / 2);

    if (输出) {
        *输出 = 主体;
    }
    return true;
}

bool 任务管理任务模块::读取任务管理主体虚拟存在(
    任务类::节点类* 宿主任务,
    结构_任务管理主体虚拟存在* 输出) noexcept
{
    if (!宿主任务 || !输出) {
        return false;
    }

    const auto 主体ID = 私有_主体ID_按宿主任务(reinterpret_cast<任务节点*>(宿主任务));
    if (主体ID == 0) {
        return false;
    }

    std::scoped_lock 锁(私有_任务管理主体表锁);
    const auto it = 私有_任务管理主体表.find(主体ID);
    if (it == 私有_任务管理主体表.end()) {
        return false;
    }
    *输出 = it->second;
    return true;
}

bool 任务管理任务模块::初始化任务管理分身继承面(
    const 结构_任务管理上下文& 上下文,
    const 结构_任务管理主体虚拟存在& 主体,
    结构_任务管理分身继承面* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    输出->创建时间 = 上下文.当前时间;
    输出->最近推进时间 = 上下文.当前时间;
    输出->最近回并时间 = 0;
    输出->主体ID = 主体.主体ID != 0
        ? 主体.主体ID
        : (
            上下文.当前宿主任务
            ? 私有_主体ID_按宿主任务(上下文.当前宿主任务)
            : 私有_节点主键摘要(上下文.当前管理任务)
        );
    输出->分身ID = static_cast<std::uint64_t>(std::hash<std::string>{}(
        std::to_string(输出->主体ID)
        + "|" + (上下文.当前宿主任务 ? 上下文.当前宿主任务->获取主键() : std::string("空宿主"))
        + "|" + (上下文.当前方法 ? 上下文.当前方法->获取主键() : std::string("空方法"))
        + "|" + std::to_string(上下文.当前时间)));
    输出->回并序号 = 0;
    输出->当前状态 = 上下文.当前宿主任务
        ? 枚举_任务管理分身状态::可推进
        : 枚举_任务管理分身状态::待继承;
    输出->汇总策略 =
        上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::允许普通执行
        ? 枚举_任务管理主体汇总策略::高置信优先
        : 枚举_任务管理主体汇总策略::最近优先;
    输出->当前主方法需求位 = 枚举_任务管理方法需求位::未定义;
    输出->当前本能能力缺口类型 =
        上下文.当前本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义
        ? 上下文.当前本能能力缺口类型
        : 主体.最近本能能力缺口类型;
    输出->宿主任务 = 上下文.当前宿主任务;
    输出->当前步骤 = 上下文.当前步骤;
    输出->最近结果 = 上下文.最近结果;
    输出->当前管理方法 = 上下文.当前方法;
    输出->当前方法位专项动态 =
        上下文.当前方法位专项动态
        ? 上下文.当前方法位专项动态
        : 主体.最近方法位专项动态;
    输出->当前最小原语动态 =
        上下文.当前最小原语动态
        ? 上下文.当前最小原语动态
        : 主体.最近最小原语动态;
    输出->已继承宿主任务虚拟存在特征 = 输出->宿主任务 != nullptr;
    输出->已继承方法虚拟存在特征 = 输出->当前管理方法 != nullptr;
    私有_刷新分身继承位图(*输出, 上下文);
    输出->本轮输入条件主键 = 私有_主体化输入条件主键(上下文, *输出);
    输出->本轮结果主键 = 私有_主体化结果主键(*输出, 上下文);
    输出->当前治理态型 =
        !上下文.当前治理态型.empty()
        ? 上下文.当前治理态型
        : 主体.最近治理态型;
    输出->当前方法位专项动态标题 =
        !上下文.当前方法位专项动态标题.empty()
        ? 上下文.当前方法位专项动态标题
        : 主体.最近方法位专项动态标题;
    输出->当前最小原语动态标题 =
        !上下文.当前最小原语动态标题.empty()
        ? 上下文.当前最小原语动态标题
        : 主体.最近最小原语动态标题;

    std::ostringstream 摘要;
    摘要 << "宿主=" << 私有_任务标题(输出->宿主任务)
        << " | 方法=" << 私有_方法标题(输出->当前管理方法)
        << " | 域=" << 私有_功能域文本(上下文.最近功能域)
        << " | 缺口=" << 私有_缺口文本(上下文.当前缺口类型)
        << " | 去向=" << 私有_去向文本(上下文.当前下一步去向);
    输出->继承摘要 = 摘要.str();
    return true;
}

bool 任务管理任务模块::刷新任务管理方法需求位(
    const 结构_任务管理上下文& 上下文,
    const 结构_任务管理分身继承面& 分身,
    结构_任务管理方法需求位特征面* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    输出->当前功能域 = 上下文.最近功能域;
    输出->当前缺口类型 = 上下文.当前缺口类型;
    输出->当前下一步去向 = 上下文.当前下一步去向;
    输出->当前需求位 = 私有_推导任务管理方法需求位(上下文, 分身);
    输出->已命中需求位 = 输出->当前需求位 != 枚举_任务管理方法需求位::未定义;
    输出->需要新结构 =
        !分身.宿主任务
        || !分身.当前步骤
        || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::治理任务缺失
        || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::宿主任务缺失
        || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::任务待启动;
    输出->需要新观察 = 分身.最近结果 == nullptr;
    输出->需要新因果 = 输出->最近实例因果 == nullptr;
    输出->已满足完成判据 =
        (分身.宿主任务 && 分身.宿主任务->主信息.状态 == 枚举_任务状态::完成)
        || 私有_任务节点目标结果已对齐(分身.宿主任务);
    输出->已确定无法继续 =
        (分身.宿主任务
            && (分身.宿主任务->主信息.状态 == 枚举_任务状态::失败
                || 分身.宿主任务->主信息.状态 == 枚举_任务状态::取消))
        || 上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::禁止继续
        || 上下文.最近根层重判结果 == 枚举_任务管理根层重判结果::停止退出边界;
    输出->需要新学习 =
        输出->当前需求位 == 枚举_任务管理方法需求位::失败策略
        || 输出->需要新因果;
    输出->输入条件主键 =
        !分身.本轮输入条件主键.empty()
        ? 分身.本轮输入条件主键
        : 私有_主体化输入条件主键(上下文, 分身);
    输出->结果主键 =
        !分身.本轮结果主键.empty()
        ? 分身.本轮结果主键
        : 私有_主体化结果主键(分身, 上下文);
    输出->最新目标状态 = 分身.宿主任务 ? 分身.宿主任务->主信息.目标状态.指针 : nullptr;
    输出->最新结果状态 = 分身.宿主任务 ? 分身.宿主任务->主信息.结果状态信息.指针 : nullptr;
    输出->当前方法位专项动态 = 分身.当前方法位专项动态;
    输出->当前方法位专项动态标题 = 分身.当前方法位专项动态标题;
    输出->最近轻量因果链摘要 = 分身.继承摘要;

    std::ostringstream 摘要;
    摘要 << "需求位=" << 私有_方法需求位文本(输出->当前需求位)
        << " | 宿主=" << 私有_任务标题(分身.宿主任务)
        << " | 缺口=" << 私有_缺口文本(上下文.当前缺口类型)
        << " | 去向=" << 私有_去向文本(上下文.当前下一步去向);
    输出->需求摘要 = 摘要.str();
    return 输出->已命中需求位;
}

bool 任务管理任务模块::评估任务管理本能能力缺口(
    自我类& 自我对象,
    const 结构_任务管理方法需求位特征面& 需求位特征,
    结构_任务管理本能能力缺口面* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    输出->对应需求位 = 需求位特征.当前需求位;
    输出->所需原语位图 =
        私有_原语位(枚举_任务管理最小原语ID::确保单承载存在)
        | 私有_原语位(枚举_任务管理最小原语ID::读取单特征当前值)
        | 私有_原语位(枚举_任务管理最小原语ID::写入单特征当前值)
        | 私有_原语位(枚举_任务管理最小原语ID::写入单挂点指针)
        | 私有_原语位(枚举_任务管理最小原语ID::记录动作开始)
        | 私有_原语位(枚举_任务管理最小原语ID::记录动作结束);
    输出->所需派生能力位图 =
        私有_派生能力位_状态差分取证
        | 私有_派生能力位_实例因果派生
        | 私有_派生能力位_轻量因果链组织;

    (void)本能方法类::注册默认自我本能方法集(
        自我对象,
        "任务管理任务模块::评估任务管理本能能力缺口/注册默认本能方法");

    // 这里先按“正式动作资产已存在”判定六原语是否具备，
    // 不再直接把工程 helper 视为默认全具备。
    输出->已具备确保单承载存在 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::确保单承载存在,
        "任务管理任务模块::评估任务管理本能能力缺口/确保单承载存在");
    输出->已具备读取单特征当前值 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::读取单特征当前值,
        "任务管理任务模块::评估任务管理本能能力缺口/读取单特征当前值");
    输出->已具备写入单特征当前值 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::写入单特征当前值,
        "任务管理任务模块::评估任务管理本能能力缺口/写入单特征当前值");
    输出->已具备写入单挂点指针 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::写入单挂点指针,
        "任务管理任务模块::评估任务管理本能能力缺口/写入单挂点指针");
    输出->已具备记录动作开始 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::记录动作开始,
        "任务管理任务模块::评估任务管理本能能力缺口/记录动作开始");
    输出->已具备记录动作结束 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::记录动作结束,
        "任务管理任务模块::评估任务管理本能能力缺口/记录动作结束");
    输出->已完成固定派生规则 =
        输出->已具备记录动作开始 && 输出->已具备记录动作结束;
    输出->已具备状态差分取证 =
        需求位特征.最新目标状态 != nullptr && 需求位特征.最新结果状态 != nullptr;
    输出->已具备实例因果派生 = 需求位特征.最近实例因果 != nullptr;
    输出->已具备轻量因果链组织 = !需求位特征.最近轻量因果链摘要.empty();

    输出->已具备原语位图 = 0;
    if (输出->已具备确保单承载存在) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::确保单承载存在);
    if (输出->已具备读取单特征当前值) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::读取单特征当前值);
    if (输出->已具备写入单特征当前值) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::写入单特征当前值);
    if (输出->已具备写入单挂点指针) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::写入单挂点指针);
    if (输出->已具备记录动作开始) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::记录动作开始);
    if (输出->已具备记录动作结束) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::记录动作结束);

    输出->已具备派生能力位图 = 0;
    if (输出->已具备状态差分取证) 输出->已具备派生能力位图 |= 私有_派生能力位_状态差分取证;
    if (输出->已具备实例因果派生) 输出->已具备派生能力位图 |= 私有_派生能力位_实例因果派生;
    if (输出->已具备轻量因果链组织) 输出->已具备派生能力位图 |= 私有_派生能力位_轻量因果链组织;

    输出->主观察特征键 =
        !需求位特征.结果主键.empty() ? 需求位特征.结果主键 : 需求位特征.输入条件主键;
    输出->原语账本摘要 = 私有_原语位图摘要(输出->已具备原语位图);
    输出->派生规则摘要 =
        "固定派生规则="
        + std::string(输出->已完成固定派生规则 ? "已接线" : "未接线")
        + " | 派生能力="
        + 私有_派生能力位图摘要(输出->已具备派生能力位图);
    输出->允许直接执行 = false;
    输出->需要学习补齐 = false;
    输出->需要等待外部条件 = false;
    输出->需要退回补结构 = false;

    if (需求位特征.需要新学习) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::需学习补齐;
        输出->需要学习补齐 = true;
    }
    else if (!输出->已具备确保单承载存在 || !输出->已具备读取单特征当前值) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺观察原语;
    }
    else if (!输出->已具备写入单特征当前值) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺写回原语;
    }
    else if (!输出->已具备写入单挂点指针) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺挂点原语;
    }
    else if (!输出->已完成固定派生规则) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺动作边界;
    }
    else if (!输出->已具备状态差分取证) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺状态差分取证;
        输出->需要退回补结构 = true;
    }
    else if (!输出->已具备实例因果派生) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺实例因果取证;
    }
    else if (!输出->已具备轻量因果链组织) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺轻量因果链组织;
    }
    else {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::无缺口;
        输出->允许直接执行 = true;
    }

    std::ostringstream 摘要;
    摘要 << "需求位=" << 私有_方法需求位文本(需求位特征.当前需求位)
        << " | 缺口=" << 私有_本能能力缺口文本(输出->当前缺口类型)
        << " | 主观察=" << (输出->主观察特征键.empty() ? "空" : 输出->主观察特征键)
        << " | 正式已具备=" << (输出->原语账本摘要.empty() ? "空" : 输出->原语账本摘要)
        << " | 派生摘要=" << (输出->派生规则摘要.empty() ? "空" : 输出->派生规则摘要);
    输出->缺口摘要 = 摘要.str();
    return true;
}

bool 任务管理任务模块::读取任务管理正式原语资产概览(
    const 自我类& 自我对象,
    结构_任务管理正式原语资产概览* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    *输出 = {};
    输出->所需原语位图 =
        私有_原语位(枚举_任务管理最小原语ID::确保单承载存在)
        | 私有_原语位(枚举_任务管理最小原语ID::读取单特征当前值)
        | 私有_原语位(枚举_任务管理最小原语ID::写入单特征当前值)
        | 私有_原语位(枚举_任务管理最小原语ID::写入单挂点指针)
        | 私有_原语位(枚举_任务管理最小原语ID::记录动作开始)
        | 私有_原语位(枚举_任务管理最小原语ID::记录动作结束);

    输出->已具备确保单承载存在 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::确保单承载存在,
        "任务管理任务模块::读取任务管理正式原语资产概览/确保单承载存在");
    输出->已具备读取单特征当前值 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::读取单特征当前值,
        "任务管理任务模块::读取任务管理正式原语资产概览/读取单特征当前值");
    输出->已具备写入单特征当前值 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::写入单特征当前值,
        "任务管理任务模块::读取任务管理正式原语资产概览/写入单特征当前值");
    输出->已具备写入单挂点指针 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::写入单挂点指针,
        "任务管理任务模块::读取任务管理正式原语资产概览/写入单挂点指针");
    输出->已具备记录动作开始 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::记录动作开始,
        "任务管理任务模块::读取任务管理正式原语资产概览/记录动作开始");
    输出->已具备记录动作结束 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::记录动作结束,
        "任务管理任务模块::读取任务管理正式原语资产概览/记录动作结束");

    if (输出->已具备确保单承载存在) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::确保单承载存在);
    if (输出->已具备读取单特征当前值) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::读取单特征当前值);
    if (输出->已具备写入单特征当前值) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::写入单特征当前值);
    if (输出->已具备写入单挂点指针) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::写入单挂点指针);
    if (输出->已具备记录动作开始) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::记录动作开始);
    if (输出->已具备记录动作结束) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::记录动作结束);

    输出->缺失原语位图 = 输出->所需原语位图 & ~输出->已具备原语位图;
    输出->已全部具备 = 输出->缺失原语位图 == 0;
    输出->已具备摘要 = 私有_原语位图摘要(输出->已具备原语位图);
    输出->缺失摘要 = 输出->缺失原语位图 == 0
        ? "无"
        : 私有_原语位图摘要(输出->缺失原语位图);

    std::ostringstream 摘要;
    摘要 << "状态=" << (输出->已全部具备 ? "齐全" : "缺失")
        << " | 正式已具备=" << (输出->已具备摘要.empty() ? std::string("空") : 输出->已具备摘要)
        << " | 正式缺失=" << 输出->缺失摘要;
    输出->摘要 = 摘要.str();
    return true;
}

std::string 任务管理任务模块::摘要任务管理最小原语位图(std::uint64_t 位图) noexcept
{
    return 私有_原语位图摘要(位图);
}

bool 任务管理任务模块::回并分身增量到任务管理主体(
    const 结构_任务管理分身继承面& 分身,
    结构_任务管理主体虚拟存在* 主体) noexcept
{
    if (!主体 || 主体->主体ID == 0) {
        return false;
    }

    const auto 需求位索引 = static_cast<std::size_t>(分身.当前主方法需求位);
    主体->最近回并时间 = 分身.最近回并时间 != 0 ? 分身.最近回并时间 : 分身.最近推进时间;
    主体->最近回并序号 = 分身.回并序号;
    主体->最近输入条件主键 = 分身.本轮输入条件主键;
    主体->最近结果主键 = 分身.本轮结果主键;
    主体->最近原因摘要 = 分身.继承摘要;
    主体->最近宿主任务 = 分身.宿主任务;
    主体->最近管理方法 = 分身.当前管理方法;
    私有_P0_覆盖非默认值(
        主体->最近本能能力缺口类型,
        分身.当前本能能力缺口类型,
        枚举_任务管理本能能力缺口类型::未定义);
    私有_P0_覆盖非空文本(主体->最近治理态型, 分身.当前治理态型);
    主体->最近方法位专项动态 = 分身.当前方法位专项动态;
    主体->最近最小原语动态 = 分身.当前最小原语动态;
    主体->最近方法位专项动态标题 = 分身.当前方法位专项动态标题;
    主体->最近最小原语动态标题 = 分身.当前最小原语动态标题;
    主体->累计回并分身数 += 1;
    主体->累计推进次数 += 分身.本地推进次数;
    主体->累计完成次数 += 分身.本地完成次数;
    主体->累计失败次数 += 分身.本地失败次数;
    主体->累计稳定复现次数 += 分身.本地验证证据次数;
    主体->累计价值成立次数 += 分身.本地价值证据次数;
    if (主体->当前活跃分身数 > 0) {
        主体->当前活跃分身数 -= 1;
    }
    if (需求位索引 < std::size(主体->需求位命中次数)) {
        主体->最近主方法需求位 = 分身.当前主方法需求位;
        主体->需求位命中次数[需求位索引] += 1;
        主体->需求位成功次数[需求位索引] += 分身.本地推进次数 > 0 ? 1U : 0U;
        主体->需求位弱出生证据次数[需求位索引] += 分身.本地弱出生证据次数;
        主体->需求位验证证据次数[需求位索引] += 分身.本地验证证据次数;
        主体->需求位升格证据次数[需求位索引] += 分身.本地升格证据次数;
    }

    std::scoped_lock 锁(私有_任务管理主体表锁);
    私有_任务管理主体表[主体->主体ID] = *主体;
    return true;
}

bool 任务管理任务模块::提交学习方法资产(
    自我类& 自我对象,
    const 结构_学习方法资产提交输入& 输入,
    时间戳 now,
    const std::string& 调用点,
    结构_学习方法资产提交结果* 输出) noexcept
{
    if (!输入.来源宿主任务) {
        return false;
    }

    结构_任务管理结果 结果{};
    结构_任务管理上下文 提交上下文{};
    提交上下文.当前管理任务 = 自我对象.获取任务管理任务();
    提交上下文.当前宿主任务 = 输入.来源宿主任务;
    提交上下文.当前方法 = 输入.来源方法;
    提交上下文.当前步骤 = 输入.来源步骤;
    提交上下文.最近结果 = 输入.来源结果;
    提交上下文.当前缺口类型 = 输入.当前缺口类型;
    提交上下文.当前下一步去向 = 输入.当前下一步去向;
    提交上下文.当前方法需求位 = 输入.来源方法需求位;
    提交上下文.当前本能能力缺口类型 = 输入.来源本能能力缺口类型;
    提交上下文.当前输入条件主键 = 输入.来源主观察特征;
    提交上下文.当前治理态型 = 输入.回流目标摘要;

    结构_任务管理请求 提交请求{};
    提交请求.宿主任务 = 输入.来源宿主任务;
    提交请求.上下文 = 提交上下文;
    提交请求.触发来源 = 输入.当前触发来源;
    提交请求.来源最小原语位图 = 输入.来源最小原语位图;
    提交请求.来源主观察特征 = 输入.来源主观察特征;
    提交请求.影子验证状态 = 输入.当前影子验证状态;
    提交请求.允许正式资产提交 = 输入.允许正式资产提交;
    提交请求.补充材料摘要 = 输入.学习承接摘要;
    私有_P0_覆盖请求字段(结果.当前请求, 提交请求);
    私有_P0_覆盖上下文字段(结果.上下文, 提交上下文);

    结果.当前学习方法节点 = 输入.学习方法;
    结果.当前子任务节点 = 输入.学习子任务;
    结果.当前步骤节点 = 输入.来源步骤;
    结果.最新结果节点 = 输入.来源结果;
    结果.当前缺口类型 = 输入.当前缺口类型;
    结果.当前下一步去向 = 输入.当前下一步去向;
    结果.当前方法来源 = 输入.当前方法来源;
    结果.当前主方法需求位 = 输入.来源方法需求位;
    结果.当前本能能力缺口类型 = 输入.来源本能能力缺口类型;

    结构_任务管理单步决策 提交决策{};
    提交决策.当前缺口类型 = 输入.当前缺口类型;
    提交决策.当前下一步去向 = 输入.当前下一步去向;
    提交决策.当前方法来源 = 输入.当前方法来源;
    提交决策.当前触发来源 = 输入.当前触发来源;
    提交决策.当前方法需求位 = 输入.来源方法需求位;
    提交决策.当前本能能力缺口类型 = 输入.来源本能能力缺口类型;
    提交决策.来源最小原语位图 = 输入.来源最小原语位图;
    提交决策.来源主观察特征 = 输入.来源主观察特征;
    提交决策.影子验证状态 = 输入.当前影子验证状态;
    提交决策.允许正式资产提交 = 输入.允许正式资产提交;
    提交决策.决策摘要 = 输入.学习承接摘要;
    私有_P0_覆盖决策字段(结果.当前单步决策, 提交决策);

    结构_任务管理写回结果 提交写回{};
    提交写回.当前学习方法 = 输入.学习方法;
    提交写回.当前学习子任务 = 输入.学习子任务;
    提交写回.当前步骤节点 = 输入.来源步骤;
    提交写回.最新结果节点 = 输入.来源结果;
    提交写回.主体回并摘要 = 输入.回流目标摘要;
    私有_P0_覆盖写回字段(结果.当前写回结果, 提交写回);

    结构_任务管理一步结果 提交一步结果{};
    提交一步结果.宿主任务 = 输入.来源宿主任务;
    提交一步结果.当前方法节点 = 输入.来源方法;
    提交一步结果.当前步骤节点 = 输入.来源步骤;
    提交一步结果.最新结果节点 = 输入.来源结果;
    提交一步结果.当前缺口类型 = 输入.当前缺口类型;
    提交一步结果.当前下一步去向 = 输入.当前下一步去向;
    提交一步结果.当前方法需求位 = 输入.来源方法需求位;
    提交一步结果.当前本能能力缺口类型 = 输入.来源本能能力缺口类型;
    提交一步结果.当前触发来源 = 输入.当前触发来源;
    提交一步结果.当前输入条件主键 = 输入.来源主观察特征;
    提交一步结果.来源最小原语位图 = 输入.来源最小原语位图;
    提交一步结果.来源主观察特征 = 输入.来源主观察特征;
    提交一步结果.影子验证状态 = 输入.当前影子验证状态;
    提交一步结果.允许正式资产提交 = 输入.允许正式资产提交;
    提交一步结果.已触发学习承接 = true;
    提交一步结果.当前学习承接类型 = 输入.当前学习承接类型;
    提交一步结果.当前学习为本能补齐 =
        私有_本能缺口属于原语或治理补齐(输入.来源本能能力缺口类型);
    提交一步结果.学习承接摘要 = 输入.学习承接摘要;
    提交一步结果.主体回并摘要 = 输入.回流目标摘要;
    私有_P0_覆盖一步结果字段(结果.当前一步结果, 提交一步结果);

    结果.当前学习承接决策.是否触发学习 = true;
    结果.当前学习承接决策.学习类型 = 输入.当前学习承接类型;
    结果.当前学习承接决策.补原语或治理本能 =
        私有_本能缺口属于原语或治理补齐(输入.来源本能能力缺口类型);
    结果.当前学习承接决策.来源根任务 =
        输入.来源宿主任务 ? 私有_主体锚点任务(输入.来源宿主任务) : nullptr;
    结果.当前学习承接决策.来源父任务 = 输入.来源宿主任务;
    结果.当前学习承接决策.来源功能域 = 结果.当前功能域;
    结果.当前学习承接决策.来源缺口类型 = 输入.当前缺口类型;
    结果.当前学习承接决策.来源方法需求位 = 输入.来源方法需求位;
    结果.当前学习承接决策.来源本能能力缺口类型 = 输入.来源本能能力缺口类型;
    结果.当前学习承接决策.来源最小原语位图 = 输入.来源最小原语位图;
    结果.当前学习承接决策.来源主观察特征 = 输入.来源主观察特征;
    结果.当前学习承接决策.回流目标方法位 = 输入.回流目标方法位;
    结果.当前学习承接决策.回流目标摘要 = 输入.回流目标摘要;
    结果.当前学习承接决策.是否影子验证 = !输入.允许正式资产提交;
    结果.当前学习承接决策.允许正式资产提交 = 输入.允许正式资产提交;
    结果.当前学习承接决策.决策摘要 = 输入.学习承接摘要;

    auto* 学习子任务 = 输入.学习子任务;
    if (!学习子任务) {
        auto* 当前管理任务 = 自我对象.获取任务管理任务();
        结果.上下文.当前管理任务 = 当前管理任务;
        学习子任务 = 私有_创建或复用学习子任务头(
            自我对象,
            reinterpret_cast<任务节点*>(当前管理任务),
            reinterpret_cast<任务节点*>(输入.来源宿主任务),
            结果,
            now,
            调用点 + "/补齐学习子任务");
        if (!学习子任务) {
            return false;
        }
        结果.当前子任务节点 = 学习子任务;
        结果.当前写回结果.当前学习子任务 = 学习子任务;
    }

    auto* 方法头 = 私有_写回学习方法骨架(
        自我对象,
        输入.来源宿主任务,
        学习子任务,
        结果,
        now,
        调用点 + "/正式入账");
    if (!方法头) {
        return false;
    }

    结构_学习方法资产提交结果 本地结果{};
    本地结果.已写回正式方法资产 = true;
    本地结果.已创建学习方法头 = 结果.已创建学习方法头;
    本地结果.已复用学习方法头 = 结果.已复用学习方法头;
    本地结果.已创建学习方法条件节点 = 结果.已创建学习方法条件节点;
    本地结果.已创建学习方法结果节点 = 结果.已创建学习方法结果节点;
    本地结果.当前学习方法节点 = 方法头;
    本地结果.正式方法标题 = 私有_方法标题(方法头);

    const bool 复用现有方法头 =
        !结果.已创建学习方法头
        && 输入.来源方法
        && 方法头
        && 输入.来源方法->获取主键() == 方法头->获取主键();
    const bool 视为新生正式方法资产 =
        结果.已创建学习方法头
        || !输入.来源方法
        || 输入.当前方法来源 == 枚举_任务管理方法来源::默认本能兜底;
    本地结果.资产类型 = 视为新生正式方法资产
        ? "新生正式方法资产"
        : (复用现有方法头 ? "复用现有方法头" : "沿用学习方法资产");

    std::ostringstream 摘要;
    摘要 << 本地结果.资产类型 << "="
        << (本地结果.正式方法标题.empty() ? std::string("空方法") : 本地结果.正式方法标题);
    if (结果.已创建学习方法条件节点) {
        摘要 << " | 已补条件节点";
    }
    if (结果.已创建学习方法结果节点) {
        摘要 << " | 已补结果节点";
    }
    std::string 主体长期策略摘要{};
    if (私有_M10_沉淀正式学习回流到主体(
        自我对象,
        输入,
        本地结果,
        now,
        &结果.当前主体,
        &主体长期策略摘要)) {
        结果.已确保任务管理主体虚拟存在 = true;
        结果.当前主体ID = 结果.当前主体.主体ID;
        结果.上下文.当前主体ID = 结果.当前主体ID;
        结果.当前写回结果.主体回并摘要 = 主体长期策略摘要;
        结果.当前一步结果.主体回并摘要 = 主体长期策略摘要;
        本地结果.主体长期策略摘要 = 主体长期策略摘要;
        if (!主体长期策略摘要.empty()) {
            摘要 << " | 主体策略=" << 主体长期策略摘要;
        }
    }
    本地结果.摘要 = 摘要.str();

    if (输出) {
        *输出 = std::move(本地结果);
    }
    return true;
}

bool 任务管理任务模块::执行一步治理(
    自我类& 自我对象,
    时间戳 now,
    const std::string& 调用点,
    结构_任务管理结果* 输出) noexcept
{
    const auto 当前时间 = 私有_归一化时间(now);
    结构_任务管理结果 结果{};
    结构_M2治理阶段状态 阶段状态{};
    if (!私有_M9_执行前段治理阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态)) {
        if (输出) {
            *输出 = 结果;
        }
        return 私有_治理轮返回值(结果);
    }

    const bool 桥接补层允许继续 = 阶段状态.桥接补层允许继续;

    if (桥接补层允许继续) {
        私有_M9_执行后段治理动作阶段(
            自我对象,
            当前时间,
            调用点,
            结果,
            阶段状态);
        私有_M9_执行学习承接与业务派生阶段(
            自我对象,
            当前时间,
            调用点,
            结果,
            阶段状态);
    }

    私有_M2_组装一步结果阶段(
        自我对象,
        当前时间,
        调用点,
        结果,
        阶段状态);

    if (输出) {
        *输出 = 结果;
    }

    return 私有_治理轮返回值(结果);
}
