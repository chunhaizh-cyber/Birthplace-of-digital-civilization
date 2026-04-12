#include "自我类.h"

#include <algorithm>
#include <functional>
#include <limits>

#include "本能方法类.h"
#include "自我初始化模块.h"
#include "需求类.h"
#include "任务类.h"
#include "方法类.h"
#include "语素类.h"

namespace {
    using 方法节点 = 方法类::节点类;

    I64 私有_饱和加(I64 a, I64 b) noexcept
    {
        if (b > 0 && a > (std::numeric_limits<I64>::max)() - b) return (std::numeric_limits<I64>::max)();
        if (b < 0 && a < (std::numeric_limits<I64>::min)() - b) return (std::numeric_limits<I64>::min)();
        return a + b;
    }

    I64 私有_饱和减(I64 a, I64 b) noexcept
    {
        if (b == (std::numeric_limits<I64>::min)()) return (std::numeric_limits<I64>::max)();
        return 私有_饱和加(a, -b);
    }

    bool 私有_词性相同(const 词性节点类* a, const 词性节点类* b) noexcept
    {
        if (a == b) return true;
        if (!a || !b) return false;
        return a->获取主键() == b->获取主键();
    }

    template<class T节点>
    void 私有_写入引用(可解析引用<T节点>& 输出, T节点* 节点) noexcept
    {
        输出.指针 = 节点;
        输出.主键 = 节点 ? 节点->获取主键() : std::string{};
    }

    template<class T节点>
    T节点* 私有_获取独立树根(T节点* 节点) noexcept
    {
        while (节点 && 节点->父) {
            节点 = static_cast<T节点*>(节点->父);
        }
        return 节点;
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

    constexpr I64 私有_百分比值(I64 基准, I64 百分比) noexcept
    {
        return (基准 / 100) * 百分比 + ((基准 % 100) * 百分比) / 100;
    }

    template<class T节点>
    std::uint64_t 私有_节点主键摘要(const T节点* 节点) noexcept
    {
        if (!节点) return 0;
        return static_cast<std::uint64_t>(std::hash<std::string>{}(节点->获取主键()));
    }

    const 词性节点类* 私有_本能动作名称词(枚举_本能方法ID 本能ID) noexcept
    {
        const auto* 元信息 = 本能方法类::查询元信息(本能ID);
        if (!元信息 || !元信息->动作名称 || 元信息->动作名称[0] == '\0') {
            return nullptr;
        }
        return 语素集.添加词性词(元信息->动作名称, "动词");
    }

    方法节点* 私有_查找本能方法首节点(const 自我类& 自我对象, 枚举_本能方法ID 本能ID) noexcept
    {
        auto* 自我存在 = const_cast<存在节点类*>(自我对象.获取自我存在());
        if (!自我存在) return nullptr;

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        const auto* 动作名称 = 私有_本能动作名称词(本能ID);
        if (!方法根节点) return nullptr;

        return 私有_查找直接子节点_按谓词<方法节点>(方法根节点, [&](const 方法节点* 节点) noexcept {
            if (!节点) return false;
            const auto& 主信息 = 节点->主信息;
            if (主信息.节点种类 != 枚举_方法节点种类::方法首节点) return false;
            if (主信息.动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
                && 主信息.动作句柄.本能ID == static_cast<I64>(本能ID)) {
                return true;
            }
            return 动作名称 && 私有_词性相同(主信息.动作名, 动作名称);
        });
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

    动态节点类* 私有_创建本能动作动态(
        自我类& 自我对象,
        特征节点类* 特征节点,
        状态节点类* 条件状态,
        状态节点类* 结果状态,
        枚举_本能方法ID 本能ID,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        auto* 自我存在 = 自我对象.确保自我存在(调用点 + "/自我存在");
        auto* 自我内部世界 = 自我对象.确保自我内部世界(调用点 + "/自我内部世界");
        if (!自我存在 || !自我内部世界) {
            return nullptr;
        }

        auto* 动态节点 = 世界树.动态().创建动态(自我内部世界, 条件状态, 结果状态, now, now);
        if (!动态节点) {
            return nullptr;
        }

        auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
        const auto* 动作名称 = 私有_本能动作名称词(本能ID);
        if (动态主信息) {
            动态主信息->名称 = 动作名称;
            动态主信息->类型 = 动作名称;
            动态主信息->来源执行成功 = true;
        }
        (void)世界树.动态().设置主体特征(动态节点, 自我存在, 特征节点);

        if (auto* 来源方法 = 私有_查找本能方法首节点(自我对象, 本能ID)) {
            (void)世界树.动态().绑定来源方法(
                动态节点,
                reinterpret_cast<方法节点类*>(来源方法),
                来源方法->主信息.动作名 ? 来源方法->主信息.动作名 : 动作名称,
                枚举_动作事件相位::完成);
        }
        (void)世界树.动态().绑定来源场景(动态节点, 自我内部世界, 自我内部世界);
        return 动态节点;
    }

    void 私有_记录本能动作_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 旧值,
        I64 新值,
        bool 是否变化,
        枚举_本能方法ID 本能ID,
        const std::string& 动作语义键,
        时间戳 now,
        const std::vector<状态节点类*>* 附加结果状态,
        const std::string& 调用点) noexcept
    {
        auto* 自我存在 = 自我对象.确保自我存在(调用点 + "/自我存在");
        auto* 自我内部世界 = 自我对象.确保自我内部世界(调用点 + "/自我内部世界");
        if (!自我存在 || !自我内部世界 || !特征类型) {
            return;
        }

        auto* 特征节点 = 世界树.取或创建子特征_按类型(自我存在, 特征类型);
        if (!特征节点) {
            return;
        }

        状态节点类* 条件状态 = nullptr;
        状态节点类* 结果状态 = nullptr;
        if (是否变化) {
            条件状态 = 私有_创建状态节点(
                自我内部世界,
                自我存在,
                特征节点,
                特征值{ 旧值 },
                now,
                false,
                动作语义键,
                "动作前镜像");
            结果状态 = 私有_创建状态节点(
                自我内部世界,
                自我存在,
                特征节点,
                特征值{ 新值 },
                now,
                true,
                动作语义键,
                "动作后镜像");
        }

        auto* 动态节点 = 私有_创建本能动作动态(
            自我对象,
            特征节点,
            条件状态,
            结果状态,
            本能ID,
            now,
            调用点 + "/动作动态");
        if (!是否变化 || !结果状态) {
            (void)动态节点;
            return;
        }

        auto* 因果主信息 = new 因果实例主信息类{};
        const auto* 动作名称 = 私有_本能动作名称词(本能ID);
        因果主信息->名称 = 动作名称;
        因果主信息->类型 = 动作名称;
        因果主信息->锚点类型 = 枚举_因果锚点类型::存在;
        私有_写入引用(因果主信息->锚点信息, 自我存在);
        因果主信息->动作时间 = now;
        因果主信息->结果时间 = now;
        因果主信息->条件时间段 = 结构_时间段{ now, now };
        因果主信息->来源类型 = 枚举_因果来源类型::执行生成;
        因果主信息->来源方法主键 = 私有_节点主键摘要(私有_查找本能方法首节点(自我对象, 本能ID));
        因果主信息->置信度 = 10000;
        因果主信息->动作语义键 = 动作语义键;

        auto* 因果节点 = 世界树.创建因果实例(自我内部世界, 因果主信息);
        if (!因果节点) {
            return;
        }

        (void)世界树.因果().绑定动作主体语义(因果节点, 自我存在, 特征节点, 动作语义键);
        const bool 已从动态回带主结果 =
            世界树.因果().按动态桥接结果状态(因果节点, 动态节点, true);
        if (条件状态) {
            (void)世界树.因果().追加条件状态(因果节点, 条件状态);
        }
        (void)世界树.因果().追加结果状态(因果节点, 结果状态, !已从动态回带主结果);
        if (附加结果状态) {
            for (auto* 附加状态 : *附加结果状态) {
                if (附加状态 && 附加状态 != 结果状态) {
                    (void)世界树.因果().追加结果状态(因果节点, 附加状态, false);
                }
            }
        }
    }

    bool 私有_记录自我指针状态(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        auto* 自我存在 = 自我对象.确保自我存在(调用点 + "/自我存在");
        auto* 自我内部世界 = 自我对象.确保自我内部世界(调用点 + "/自我内部世界");
        if (!自我存在 || !自我内部世界 || !特征类型) {
            return false;
        }

        auto* 特征节点 = 世界树.取或创建子特征_按类型(自我存在, 特征类型);
        if (!特征节点) {
            return false;
        }

        std::uintptr_t 旧值 = 0;
        bool 有旧值 = false;
        const auto 快照 = 世界树.读取特征快照(特征节点);
        if (const auto* 句柄 = std::get_if<指针句柄>(&快照)) {
            旧值 = 句柄->指针;
            有旧值 = true;
        }

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
                自我内部世界,
                自我存在,
                特征节点,
                特征值{ 指针句柄{ 旧值 } },
                now,
                false,
                动作语义键,
                "动作前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            自我内部世界,
            自我存在,
            特征节点,
            特征值{ 指针句柄{ 新值 } },
            now,
            true,
            动作语义键,
            "动作后镜像");
        if (!结果状态) {
            return false;
        }
        if (!新值标题.empty()) {
            (void)世界树.状态().标记变化(结果状态, true, 动作语义键, 新值标题);
        }

        auto* 动态节点 = 世界树.动态().创建动态(自我内部世界, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 自我存在, 特征节点);
            (void)世界树.动态().绑定来源场景(动态节点, 自我内部世界, 自我内部世界);
        }

        auto* 因果主信息 = new 因果实例主信息类{};
        因果主信息->名称 = 特征类型;
        因果主信息->类型 = 特征类型;
        因果主信息->锚点类型 = 枚举_因果锚点类型::存在;
        私有_写入引用(因果主信息->锚点信息, 自我存在);
        因果主信息->动作时间 = now;
        因果主信息->结果时间 = now;
        因果主信息->条件时间段 = 结构_时间段{ now, now };
        因果主信息->来源类型 = 枚举_因果来源类型::执行生成;
        因果主信息->置信度 = 10000;
        因果主信息->动作语义键 = 动作语义键;

        auto* 因果节点 = 世界树.创建因果实例(自我内部世界, 因果主信息);
        if (因果节点) {
            (void)世界树.因果().绑定动作主体语义(因果节点, 自我存在, 特征节点, 动作语义键);
            const bool 已从动态回带主结果 =
                世界树.因果().按动态桥接结果状态(因果节点, 动态节点, true);
            if (条件状态) {
                (void)世界树.因果().追加条件状态(因果节点, 条件状态);
            }
            (void)世界树.因果().追加结果状态(因果节点, 结果状态, !已从动态回带主结果);
        }
        return true;
    }

    bool 私有_记录自我I64状态(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        auto* 自我存在 = 自我对象.确保自我存在(调用点 + "/自我存在");
        auto* 自我内部世界 = 自我对象.确保自我内部世界(调用点 + "/自我内部世界");
        if (!自我存在 || !自我内部世界 || !特征类型) {
            return false;
        }

        auto* 特征节点 = 世界树.取或创建子特征_按类型(自我存在, 特征类型);
        if (!特征节点) {
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
                自我内部世界,
                自我存在,
                特征节点,
                特征值{ 旧值 },
                now,
                false,
                动作语义键,
                "动作前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            自我内部世界,
            自我存在,
            特征节点,
            特征值{ 新值 },
            now,
            true,
            动作语义键,
            "动作后镜像");
        if (!结果状态) {
            return false;
        }

        auto* 动态节点 = 世界树.动态().创建动态(自我内部世界, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 自我存在, 特征节点);
            (void)世界树.动态().绑定来源场景(动态节点, 自我内部世界, 自我内部世界);
        }

        auto* 因果主信息 = new 因果实例主信息类{};
        因果主信息->名称 = 特征类型;
        因果主信息->类型 = 特征类型;
        因果主信息->锚点类型 = 枚举_因果锚点类型::存在;
        私有_写入引用(因果主信息->锚点信息, 自我存在);
        因果主信息->动作时间 = now;
        因果主信息->结果时间 = now;
        因果主信息->条件时间段 = 结构_时间段{ now, now };
        因果主信息->来源类型 = 枚举_因果来源类型::执行生成;
        因果主信息->置信度 = 10000;
        因果主信息->动作语义键 = 动作语义键;

        auto* 因果节点 = 世界树.创建因果实例(自我内部世界, 因果主信息);
        if (因果节点) {
            (void)世界树.因果().绑定动作主体语义(因果节点, 自我存在, 特征节点, 动作语义键);
            const bool 已从动态回带主结果 =
                世界树.因果().按动态桥接结果状态(因果节点, 动态节点, true);
            if (条件状态) {
                (void)世界树.因果().追加条件状态(因果节点, 条件状态);
            }
            (void)世界树.因果().追加结果状态(因果节点, 结果状态, !已从动态回带主结果);
        }
        return true;
    }
}

自我类 自我{};

自我类::自我类() = default;

自我类::~自我类()
{
    if (当前活动自我_.load(std::memory_order_acquire) == this) {
        当前活动自我_.store(nullptr, std::memory_order_release);
    }
}

void 自我类::初始化自我(const std::string& 调用点)
{
    (void)自我初始化模块::执行自我初始化(*this, 调用点, nullptr);
}

bool 自我类::已初始化() const noexcept
{
    return 已初始化_.load(std::memory_order_acquire);
}

场景节点类* 自我类::获取自我现实场景() const noexcept
{
    return 世界树.自我所在场景;
}

场景节点类* 自我类::获取自我内部世界() const noexcept
{
    return 世界树.取存在内部世界(世界树.自我指针);
}

存在节点类* 自我类::获取自我存在() const noexcept
{
    return 世界树.自我指针;
}

场景节点类* 自我类::确保自我现实场景(const std::string& 调用点)
{
    return 确保自我现实场景_(调用点);
}

场景节点类* 自我类::确保自我内部世界(const std::string& 调用点)
{
    return 确保自我内部世界_(调用点);
}

存在节点类* 自我类::确保自我存在(const std::string& 调用点)
{
    return 确保自我存在_(调用点);
}

I64 自我类::获取安全值() const noexcept
{
    return 读取自我I64特征当前值_(
        自我特征定义类::类型_自我_安全值(),
        生命值上限_() / 4,
        "自我类::获取安全值");
}

I64 自我类::获取服务值() const noexcept
{
    return 读取自我I64特征当前值_(
        自我特征定义类::类型_自我_服务值(),
        生命值上限_() / 2,
        "自我类::获取服务值");
}

I64 自我类::获取物理安全值() const noexcept
{
    return 读取自我I64特征当前值_(
        自我特征定义类::类型_自我_物理安全(),
        生命值上限_(),
        "自我类::获取物理安全值");
}

I64 自我类::获取风险安全值() const noexcept
{
    return 读取自我I64特征当前值_(
        自我特征定义类::类型_自我_风险安全(),
        生命值上限_(),
        "自我类::获取风险安全值");
}

bool 自我类::设置物理安全值(I64 值, 时间戳 now, const std::string& 调用点) noexcept
{
    return 私有_记录自我I64状态(
        *this,
        自我特征定义类::类型_自我_物理安全(),
        值,
        "自我_物理安全更新",
        now,
        调用点);
}

bool 自我类::是否已消亡() const noexcept
{
    return 获取安全值() == 0;
}

bool 自我类::是否待机状态() const noexcept
{
    return standby_mode_.load(std::memory_order_acquire);
}

I64 自我类::根目标安全值() const noexcept
{
    return 生命值上限_();
}

I64 自我类::根目标服务值() const noexcept
{
    return 生命值上限_();
}

I64 自我类::安全根方向差值() const noexcept
{
    return 私有_饱和减(根目标安全值(), 获取安全值());
}

I64 自我类::时序正向步长() const noexcept
{
    return temporal_forward_step_.load(std::memory_order_acquire);
}

I64 自我类::时序反向步长() const noexcept
{
    return temporal_backward_step_.load(std::memory_order_acquire);
}

I64 自我类::服务时序衰减步长() const noexcept
{
    return (std::max<I64>)(1, service_decay_step_.load(std::memory_order_acquire));
}

void 自我类::设置服务时序衰减步长(I64 步长) noexcept
{
    const I64 新步长 = (std::max<I64>)(1, 步长);
    service_decay_step_.store(新步长, std::memory_order_release);
    (void)写入自我I64特征当前值(
        自我特征定义类::类型_自我_服务时序衰减步长(),
        新步长,
        结构体_时间戳::当前_微秒(),
        "自我类::设置服务时序衰减步长");
}

bool 自我类::读取自我I64特征当前值(
    const 词性节点类* 特征类型,
    I64& 输出值,
    const std::string& 调用点) const noexcept
{
    (void)调用点;
    if (!特征类型) return false;

    auto* 自我存在 = 世界树.自我指针;
    if (!自我存在) return false;

    auto* 特征节点 = 世界树.存在().查找子特征_按类型(自我存在, 特征类型);
    if (!特征节点) return false;

    return 世界树.读取特征_I64(特征节点, 输出值);
}

bool 自我类::写入自我I64特征当前值(
    const 词性节点类* 特征类型,
    I64 值,
    时间戳 now,
    const std::string& 调用点) noexcept
{
    auto* 特征节点 = 确保自我特征_(特征类型, 调用点);
    if (!特征节点) return false;
    return 世界树.写入特征_I64(特征节点, 值, now);
}

bool 自我类::读取自我指针特征当前值(
    const 词性节点类* 特征类型,
    std::uintptr_t& 输出值,
    const std::string& 调用点) const noexcept
{
    (void)调用点;
    if (!特征类型) return false;

    auto* 自我存在 = 世界树.自我指针;
    if (!自我存在) return false;

    auto* 特征节点 = 世界树.存在().查找子特征_按类型(自我存在, 特征类型);
    if (!特征节点) return false;

    const auto 快照 = 世界树.读取特征快照(特征节点);
    const auto* 句柄 = std::get_if<指针句柄>(&快照);
    if (!句柄) return false;

    输出值 = 句柄->指针;
    return true;
}

bool 自我类::写入自我指针特征当前值(
    const 词性节点类* 特征类型,
    const void* 指针值,
    时间戳 now,
    const std::string& 调用点) noexcept
{
    auto* 特征节点 = 确保自我特征_(特征类型, 调用点);
    if (!特征节点) return false;
    return 世界树.写入特征_指针(特征节点, 指针值, now);
}

需求类::节点类* 自我类::获取安全根需求() const noexcept
{
    auto* 自我存在 = 世界树.自我指针;
    if (!自我存在) return nullptr;

    auto* 需求根节点 = 世界树.存在().获取需求根节点(自我存在);
    const auto* 类型词 = 语素集.添加词性词("安全根需求", "名词");
    return 私有_查找直接子节点_按谓词<需求类::节点类>(需求根节点, [&](const 需求类::节点类* 节点) noexcept {
        return 节点 && 私有_词性相同(节点->主信息.类型, 类型词);
    });
}

需求类::节点类* 自我类::获取服务根需求() const noexcept
{
    auto* 自我存在 = 世界树.自我指针;
    if (!自我存在) return nullptr;

    auto* 需求根节点 = 世界树.存在().获取需求根节点(自我存在);
    const auto* 类型词 = 语素集.添加词性词("服务根需求", "名词");
    return 私有_查找直接子节点_按谓词<需求类::节点类>(需求根节点, [&](const 需求类::节点类* 节点) noexcept {
        return 节点 && 私有_词性相同(节点->主信息.类型, 类型词);
    });
}

任务类::节点类* 自我类::获取安全根任务() const noexcept
{
    auto* 自我存在 = 世界树.自我指针;
    if (!自我存在) return nullptr;

    auto* 任务根节点 = 世界树.存在().获取任务根节点(自我存在);
    const auto* 名称词 = 语素集.添加词性词("安全根任务", "专有名词");
    return 私有_查找直接子节点_按谓词<任务类::节点类>(任务根节点, [&](const 任务类::节点类* 节点) noexcept {
        return 节点 && 私有_词性相同(节点->主信息.名称, 名称词);
    });
}

任务类::节点类* 自我类::获取服务根任务() const noexcept
{
    auto* 自我存在 = 世界树.自我指针;
    if (!自我存在) return nullptr;

    auto* 任务根节点 = 世界树.存在().获取任务根节点(自我存在);
    const auto* 名称词 = 语素集.添加词性词("服务根任务", "专有名词");
    return 私有_查找直接子节点_按谓词<任务类::节点类>(任务根节点, [&](const 任务类::节点类* 节点) noexcept {
        return 节点 && 私有_词性相同(节点->主信息.名称, 名称词);
    });
}

任务类::节点类* 自我类::获取任务管理任务() const noexcept
{
    auto* 自我存在 = 世界树.自我指针;
    if (!自我存在) return nullptr;

    auto* 任务根节点 = 世界树.存在().获取任务根节点(自我存在);
    const auto* 名称词 = 语素集.添加词性词("任务管理任务", "专有名词");
    return 私有_查找直接子节点_按谓词<任务类::节点类>(任务根节点, [&](const 任务类::节点类* 节点) noexcept {
        return 节点 && 私有_词性相同(节点->主信息.名称, 名称词);
    });
}

需求类::节点类* 自我类::获取当前主需求() const noexcept
{
    std::uintptr_t 指针值 = 0;
    if (!读取自我指针特征当前值(自我特征定义类::类型_自我_当前主需求(), 指针值, "自我类::获取当前主需求")) {
        return nullptr;
    }
    return reinterpret_cast<需求类::节点类*>(指针值);
}

任务类::节点类* 自我类::获取当前主任务() const noexcept
{
    std::uintptr_t 指针值 = 0;
    if (!读取自我指针特征当前值(自我特征定义类::类型_自我_当前主任务(), 指针值, "自我类::获取当前主任务")) {
        return nullptr;
    }
    return reinterpret_cast<任务类::节点类*>(指针值);
}

方法类::节点类* 自我类::获取当前主方法() const noexcept
{
    std::uintptr_t 指针值 = 0;
    if (!读取自我指针特征当前值(自我特征定义类::类型_自我_当前主方法(), 指针值, "自我类::获取当前主方法")) {
        return nullptr;
    }
    return reinterpret_cast<方法类::节点类*>(指针值);
}

bool 自我类::设置当前主需求(需求类::节点类* 节点, 时间戳 now, const std::string& 调用点) noexcept
{
    return 私有_记录自我指针状态(
        *this,
        自我特征定义类::类型_自我_当前主需求(),
        节点,
        节点 ? 节点->获取主键() : std::string{},
        "自我_当前主需求切换",
        now,
        调用点);
}

bool 自我类::设置当前主任务(任务类::节点类* 节点, 时间戳 now, const std::string& 调用点) noexcept
{
    return 私有_记录自我指针状态(
        *this,
        自我特征定义类::类型_自我_当前主任务(),
        节点,
        节点 ? 节点->获取主键() : std::string{},
        "自我_当前主任务切换",
        now,
        调用点);
}

bool 自我类::设置当前主方法(方法类::节点类* 节点, 时间戳 now, const std::string& 调用点) noexcept
{
    return 私有_记录自我指针状态(
        *this,
        自我特征定义类::类型_自我_当前主方法(),
        节点,
        节点 ? 节点->获取主键() : std::string{},
        "自我_当前主方法切换",
        now,
        调用点);
}

void 自我类::安全值增加(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept
{
    if (delta > 0) 安全值_上升_(delta, 原因类别, 原因说明);
    else if (delta < 0) 安全值_下降_(-delta, 原因类别, 原因说明);
}

void 自我类::安全值减少(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept
{
    if (delta > 0) 安全值_下降_(delta, 原因类别, 原因说明);
    else if (delta < 0) 安全值_上升_(-delta, 原因类别, 原因说明);
}

void 自我类::服务值增加(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept
{
    if (delta > 0) 服务值_上升_(delta, 原因类别, 原因说明);
    else if (delta < 0) 服务值_下降_(-delta, 原因类别, 原因说明);
}

void 自我类::服务值减少(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept
{
    if (delta > 0) 服务值_下降_(delta, 原因类别, 原因说明);
    else if (delta < 0) 服务值_上升_(-delta, 原因类别, 原因说明);
}

bool 自我类::应用服务归零待机保护(时间戳 now, const std::string& 调用点) noexcept
{
    const bool 旧待机状态 = standby_mode_.load(std::memory_order_acquire);
    const I64 旧待机值 = 旧待机状态 ? 1 : 0;
    const I64 当前服务值 = 获取服务值();
    const I64 旧安全值 = 获取安全值();
    constexpr I64 最小非零安全值 = 1;

    bool 新待机状态 = false;
    I64 新安全值 = 旧安全值;
    bool 待机状态已变化 = false;
    bool 安全值已变化 = false;

    if (当前服务值 != 0) {
        新待机状态 = false;
        standby_mode_.store(false, std::memory_order_release);
        待机状态已变化 = (旧待机状态 != 新待机状态);
    }
    else {
        新待机状态 = true;
        standby_mode_.store(true, std::memory_order_release);
        待机状态已变化 = (旧待机状态 != 新待机状态);
        if (旧安全值 != 最小非零安全值) {
            安全值已变化 = 写入自我I64特征当前值(
                自我特征定义类::类型_自我_安全值(),
                最小非零安全值,
                now,
                调用点 + "/写安全值");
            if (安全值已变化) {
                新安全值 = 最小非零安全值;
            }
        }
    }

    std::vector<状态节点类*> 附加结果状态{};
    if (待机状态已变化 && 安全值已变化) {
        auto* 自我存在 = 确保自我存在(调用点 + "/自我存在");
        auto* 自我内部世界 = 确保自我内部世界(调用点 + "/自我内部世界");
        if (自我存在 && 自我内部世界) {
            auto* 安全特征节点 = 世界树.取或创建子特征_按类型(
                自我存在,
                自我特征定义类::类型_自我_安全值());
            if (安全特征节点) {
                if (auto* 安全结果状态 = 私有_创建状态节点(
                    自我内部世界,
                    自我存在,
                    安全特征节点,
                    特征值{ 新安全值 },
                    now,
                    true,
                    "服务归零待机保护",
                    "动作后镜像/安全值")) {
                    附加结果状态.push_back(安全结果状态);
                }
            }
        }
    }

    同步运行态特征_(调用点 + "/同步运行态");

    if (待机状态已变化 || (!待机状态已变化 && 安全值已变化)) {
        const bool 以待机状态为主结果 = 待机状态已变化;
        私有_记录本能动作_I64(
            *this,
            以待机状态为主结果 ? 自我特征定义类::类型_自我_待机状态() : 自我特征定义类::类型_自我_安全值(),
            以待机状态为主结果 ? 旧待机值 : 旧安全值,
            以待机状态为主结果 ? (新待机状态 ? 1 : 0) : 新安全值,
            true,
            枚举_本能方法ID::自我_服务归零待机保护,
            "服务归零待机保护",
            now,
            附加结果状态.empty() ? nullptr : &附加结果状态,
            调用点);
    }
    else {
        私有_记录本能动作_I64(
            *this,
            自我特征定义类::类型_自我_待机状态(),
            旧待机值,
            旧待机值,
            false,
            枚举_本能方法ID::自我_服务归零待机保护,
            "服务归零待机保护",
            now,
            nullptr,
            调用点);
    }
    return 待机状态已变化 || 安全值已变化;
}

bool 自我类::应用定时衰减服务值(I64 delta, 时间戳 now, const std::string& 调用点) noexcept
{
    if (delta <= 0) delta = 服务时序衰减步长();
    delta = (std::max<I64>)(1, delta);

    const I64 旧服务值 = 获取服务值();
    const I64 候选服务值 = 私有_饱和减(旧服务值, delta);

    bool 已变化 = false;
    I64 新服务值 = 旧服务值;
    if (候选服务值 != 旧服务值) {
        已变化 = 写入自我I64特征当前值(
            自我特征定义类::类型_自我_服务值(),
            候选服务值,
            now,
            调用点 + "/写服务值");
        if (已变化) {
            新服务值 = 候选服务值;
        }
    }

    私有_记录本能动作_I64(
        *this,
        自我特征定义类::类型_自我_服务值(),
        旧服务值,
        新服务值,
        已变化,
        枚举_本能方法ID::自我_定时衰减服务值,
        "定时衰减服务值",
        now,
        nullptr,
        调用点);
    return 已变化;
}

bool 自我类::应用按双值重算服务衰减步长(时间戳 now, const std::string& 调用点) noexcept
{
    const I64 旧步长 = 服务时序衰减步长();
    const I64 新步长 = 计算服务衰减步长_按双值_(获取服务值(), 获取安全值());

    const bool 已变化 = 新步长 != 旧步长;
    if (已变化) {
        service_decay_step_.store(新步长, std::memory_order_release);
        (void)写入自我I64特征当前值(
            自我特征定义类::类型_自我_服务时序衰减步长(),
            新步长,
            now,
            调用点 + "/写服务衰减步长");
    }

    私有_记录本能动作_I64(
        *this,
        自我特征定义类::类型_自我_服务时序衰减步长(),
        旧步长,
        新步长,
        已变化,
        枚举_本能方法ID::自我_按双值重算服务衰减步长,
        "按双值重算服务衰减步长",
        now,
        nullptr,
        调用点);
    return 已变化;
}

bool 自我类::应用按服务值重算安全增加步长(时间戳 now, const std::string& 调用点) noexcept
{
    const I64 旧步长 = 时序正向步长();
    const I64 新步长 = 计算安全增加步长_按服务值_(获取服务值());

    const bool 已变化 = 新步长 != 旧步长;
    if (已变化) {
        temporal_forward_step_.store(新步长, std::memory_order_release);
        (void)写入自我I64特征当前值(
            自我特征定义类::类型_自我_时序正向步长(),
            新步长,
            now,
            调用点 + "/写安全增加步长");
    }

    私有_记录本能动作_I64(
        *this,
        自我特征定义类::类型_自我_时序正向步长(),
        旧步长,
        新步长,
        已变化,
        枚举_本能方法ID::自我_按服务值重算安全增加步长,
        "按服务值重算安全增加步长",
        now,
        nullptr,
        调用点);
    return 已变化;
}

bool 自我类::应用按服务值重算安全减少步长(时间戳 now, const std::string& 调用点) noexcept
{
    const I64 旧步长 = 时序反向步长();
    const I64 新步长 = 计算安全减少步长_按服务值_(获取服务值());

    const bool 已变化 = 新步长 != 旧步长;
    if (已变化) {
        temporal_backward_step_.store(新步长, std::memory_order_release);
        (void)写入自我I64特征当前值(
            自我特征定义类::类型_自我_时序反向步长(),
            新步长,
            now,
            调用点 + "/写安全减少步长");
    }

    私有_记录本能动作_I64(
        *this,
        自我特征定义类::类型_自我_时序反向步长(),
        旧步长,
        新步长,
        已变化,
        枚举_本能方法ID::自我_按服务值重算安全减少步长,
        "按服务值重算安全减少步长",
        now,
        nullptr,
        调用点);
    return 已变化;
}

bool 自我类::应用安全低位回升(I64 delta, 时间戳 now, const std::string& 调用点) noexcept
{
    const I64 旧安全值 = 获取安全值();
    const I64 目标安全值 = 安全稳定带下限值_();

    if (delta <= 0) delta = 时序正向步长();
    delta = (std::max<I64>)(1, delta);

    I64 新安全值 = 旧安全值;
    bool 已变化 = false;
    if (旧安全值 < 目标安全值) {
        const I64 候选值 = 私有_饱和加(旧安全值, delta);
        新安全值 = (std::min<I64>)(候选值, 目标安全值);
        if (新安全值 != 旧安全值) {
            已变化 = 写入自我I64特征当前值(
                自我特征定义类::类型_自我_安全值(),
                新安全值,
                now,
                调用点 + "/写安全值");
            if (!已变化) {
                新安全值 = 旧安全值;
            }
        }
    }

    私有_记录本能动作_I64(
        *this,
        自我特征定义类::类型_自我_安全值(),
        旧安全值,
        新安全值,
        已变化,
        枚举_本能方法ID::自我_安全低位回升,
        "安全低位回升",
        now,
        nullptr,
        调用点);
    return 已变化;
}

bool 自我类::应用安全高位回落(I64 delta, 时间戳 now, const std::string& 调用点) noexcept
{
    const I64 旧安全值 = 获取安全值();
    const I64 目标安全值 = 安全稳定带上限值_();

    if (delta <= 0) delta = 时序反向步长();
    delta = (std::max<I64>)(1, delta);

    I64 新安全值 = 旧安全值;
    bool 已变化 = false;
    if (旧安全值 > 目标安全值) {
        const I64 候选值 = 私有_饱和减(旧安全值, delta);
        新安全值 = (std::max<I64>)(候选值, 目标安全值);
        if (新安全值 != 旧安全值) {
            已变化 = 写入自我I64特征当前值(
                自我特征定义类::类型_自我_安全值(),
                新安全值,
                now,
                调用点 + "/写安全值");
            if (!已变化) {
                新安全值 = 旧安全值;
            }
        }
    }

    私有_记录本能动作_I64(
        *this,
        自我特征定义类::类型_自我_安全值(),
        旧安全值,
        新安全值,
        已变化,
        枚举_本能方法ID::自我_安全高位回落,
        "安全高位回落",
        now,
        nullptr,
        调用点);
    return 已变化;
}

bool 自我类::应用风险安全回归(I64 delta, 时间戳 now, const std::string& 调用点) noexcept
{
    delta = (std::max<I64>)(1, delta);

    const I64 旧风险安全值 = 获取风险安全值();
    const I64 目标风险安全值 = 风险安全回归目标值_();
    I64 新风险安全值 = 旧风险安全值;
    bool 已变化 = false;

    if (旧风险安全值 > 目标风险安全值) {
        const I64 候选值 = 私有_饱和减(旧风险安全值, delta);
        新风险安全值 = (std::max<I64>)(候选值, 目标风险安全值);
        if (新风险安全值 != 旧风险安全值) {
            已变化 = 写入自我I64特征当前值(
                自我特征定义类::类型_自我_风险安全(),
                新风险安全值,
                now,
                调用点 + "/写风险安全");
            if (!已变化) {
                新风险安全值 = 旧风险安全值;
            }
        }
    }

    私有_记录本能动作_I64(
        *this,
        自我特征定义类::类型_自我_风险安全(),
        旧风险安全值,
        新风险安全值,
        已变化,
        枚举_本能方法ID::自我_风险安全回归,
        "风险安全回归",
        now,
        nullptr,
        调用点);
    return 已变化;
}

bool 自我类::更新待学习方法数量(I64 数量, 时间戳 now, const std::string& 调用点) noexcept
{
    if (数量 < 0) {
        数量 = 0;
    }

    I64 旧值 = 0;
    (void)读取自我I64特征当前值(
        自我特征定义类::类型_自我_待学习方法数量(),
        旧值,
        调用点 + "/读取待学习方法数量");
    const bool 已变化 = 数量 != 旧值;
    if (已变化) {
        (void)写入自我I64特征当前值(
            自我特征定义类::类型_自我_待学习方法数量(),
            数量,
            now,
            调用点 + "/写待学习方法数量");
    }

    私有_记录本能动作_I64(
        *this,
        自我特征定义类::类型_自我_待学习方法数量(),
        旧值,
        已变化 ? 数量 : 旧值,
        已变化,
        枚举_本能方法ID::自我_更新待学习方法数量,
        "更新待学习方法数量",
        now,
        nullptr,
        调用点);
    return 已变化;
}

bool 自我类::更新尝试学习状态(bool 正在尝试学习, 时间戳 now, const std::string& 调用点) noexcept
{
    I64 旧值 = 0;
    (void)读取自我I64特征当前值(
        自我特征定义类::类型_自我_尝试学习状态(),
        旧值,
        调用点 + "/读取尝试学习状态");
    const I64 新值 = 正在尝试学习 ? 1 : 0;
    const bool 已变化 = 新值 != 旧值;
    if (已变化) {
        (void)写入自我I64特征当前值(
            自我特征定义类::类型_自我_尝试学习状态(),
            新值,
            now,
            调用点 + "/写尝试学习状态");
    }

    私有_记录本能动作_I64(
        *this,
        自我特征定义类::类型_自我_尝试学习状态(),
        旧值,
        已变化 ? 新值 : 旧值,
        已变化,
        枚举_本能方法ID::自我_更新尝试学习状态,
        "更新尝试学习状态",
        now,
        nullptr,
        调用点);
    return 已变化;
}

自我类::结构_根任务权重& 自我类::根任务权重() noexcept
{
    return roots_;
}

const 自我类::结构_根任务权重& 自我类::根任务权重() const noexcept
{
    return roots_;
}

自我类* 自我类::当前活动自我() noexcept
{
    return 当前活动自我_.load(std::memory_order_acquire);
}

constexpr I64 自我类::生命值上限_() noexcept
{
    return (std::numeric_limits<I64>::max)();
}

constexpr I64 自我类::服务低位阈值_() noexcept
{
    return 私有_百分比值(生命值上限_(), 50);
}

constexpr I64 自我类::服务高位阈值_() noexcept
{
    return 私有_百分比值(生命值上限_(), 80);
}

constexpr I64 自我类::安全稳定带下限值_() noexcept
{
    return 私有_百分比值(生命值上限_(), 50);
}

constexpr I64 自我类::安全稳定带上限值_() noexcept
{
    return 私有_百分比值(生命值上限_(), 80);
}

constexpr I64 自我类::风险安全回归目标值_() noexcept
{
    return ((生命值上限_() / 5) * 4) + (((生命值上限_() % 5) * 4) / 5);
}

场景节点类* 自我类::确保自我现实场景_(const std::string& 调用点) noexcept
{
    (void)调用点;
    return 世界树.取或创建自我现实场景();
}

场景节点类* 自我类::确保自我内部世界_(const std::string& 调用点) noexcept
{
    auto* 自我存在 = 确保自我存在_(调用点 + "/宿主存在");
    if (!自我存在) return nullptr;
    const auto* 名称词 = 语素集.添加词性词("自我内部世界", "专有名词");
    return 世界树.取或创建存在内部世界(自我存在, 名称词);
}

存在节点类* 自我类::确保自我存在_(const std::string& 调用点) noexcept
{
    (void)调用点;
    return 世界树.取或创建自我存在();
}

特征节点类* 自我类::确保自我特征_(const 词性节点类* 特征类型, const std::string& 调用点) noexcept
{
    if (!特征类型) return nullptr;

    auto* 自我存在 = 确保自我存在_(调用点 + "/自我存在");
    if (!自我存在) return nullptr;

    auto* 特征节点 = 世界树.取或创建子特征_按类型(自我存在, 特征类型);
    auto* 特征主信息 = 世界树.特征().取特征主信息(特征节点);
    if (特征主信息) {
        if (!特征主信息->名称) 特征主信息->名称 = 特征类型;
        if (!特征主信息->类型) 特征主信息->类型 = 特征类型;
    }
    return 特征节点;
}

void 自我类::确保自我I64特征已初始化_(
    存在节点类* 自我存在,
    const 词性节点类* 特征类型,
    I64 默认值,
    const std::string& 调用点) noexcept
{
    (void)自我存在;
    I64 当前值 = 0;
    if (读取自我I64特征当前值(特征类型, 当前值, 调用点 + "/读取当前值")) {
        return;
    }
    (void)写入自我I64特征当前值(特征类型, 默认值, 结构体_时间戳::当前_微秒(), 调用点 + "/写默认值");
}

void 自我类::确保自我指针特征已初始化_(
    存在节点类* 自我存在,
    const 词性节点类* 特征类型,
    std::uintptr_t 默认值,
    const std::string& 调用点) noexcept
{
    (void)自我存在;
    std::uintptr_t 当前值 = 0;
    if (读取自我指针特征当前值(特征类型, 当前值, 调用点 + "/读取当前值")) {
        return;
    }
    (void)写入自我指针特征当前值(
        特征类型,
        reinterpret_cast<const void*>(默认值),
        结构体_时间戳::当前_微秒(),
        调用点 + "/写默认值");
}

void 自我类::初始化自我特征与默认值_(const std::string& 调用点) noexcept
{
    auto* 自我存在 = 确保自我存在_(调用点 + "/自我存在");
    if (!自我存在) return;
    const 时间戳 now = 结构体_时间戳::当前_微秒();

    const I64 默认最大安全值 = 生命值上限_();
    const I64 默认安全值 = 生命值上限_() / 4;
    const I64 默认服务值 = 生命值上限_() / 2;

    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_安全值(), 默认安全值, 调用点 + "/安全值");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_服务值(), 默认服务值, 调用点 + "/服务值");
    I64 当前物理安全值 = 0;
    if (!读取自我I64特征当前值(自我特征定义类::类型_自我_物理安全(), 当前物理安全值, 调用点 + "/读取物理安全")) {
        (void)设置物理安全值(默认最大安全值, now, 调用点 + "/物理安全");
    }
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_风险安全(), 默认最大安全值, 调用点 + "/风险安全");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_情绪ID(), 0, 调用点 + "/情绪ID");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_情绪强度(), 0, 调用点 + "/情绪强度");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_外显情绪ID(), 0, 调用点 + "/外显情绪ID");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_外显情绪强度(), 0, 调用点 + "/外显情绪强度");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_待学习方法数量(), 0, 调用点 + "/待学习方法数量");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_外设可用性(), 0, 调用点 + "/外设可用性");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_尝试学习状态(), 0, 调用点 + "/尝试学习状态");

    确保自我指针特征已初始化_(自我存在, 自我特征定义类::类型_自我_当前主需求(), 0, 调用点 + "/当前主需求");
    确保自我指针特征已初始化_(自我存在, 自我特征定义类::类型_自我_当前主任务(), 0, 调用点 + "/当前主任务");
    确保自我指针特征已初始化_(自我存在, 自我特征定义类::类型_自我_当前主方法(), 0, 调用点 + "/当前主方法");

    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_待机状态(), 0, 调用点 + "/待机状态");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_时序正向步长(), 1, 调用点 + "/时序正向步长");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_时序反向步长(), 1, 调用点 + "/时序反向步长");
    确保自我I64特征已初始化_(自我存在, 自我特征定义类::类型_自我_服务时序衰减步长(), 1, 调用点 + "/服务时序衰减步长");
}

void 自我类::从特征恢复运行态_(const std::string& 调用点) noexcept
{
    I64 当前值 = 0;

    if (读取自我I64特征当前值(自我特征定义类::类型_自我_待机状态(), 当前值, 调用点 + "/待机状态")) {
        standby_mode_.store(当前值 != 0, std::memory_order_release);
    }
    else {
        standby_mode_.store(false, std::memory_order_release);
    }

    if (读取自我I64特征当前值(自我特征定义类::类型_自我_时序正向步长(), 当前值, 调用点 + "/时序正向步长")) {
        temporal_forward_step_.store((std::max<I64>)(1, 当前值), std::memory_order_release);
    }
    else {
        temporal_forward_step_.store(1, std::memory_order_release);
    }

    if (读取自我I64特征当前值(自我特征定义类::类型_自我_时序反向步长(), 当前值, 调用点 + "/时序反向步长")) {
        temporal_backward_step_.store((std::max<I64>)(1, 当前值), std::memory_order_release);
    }
    else {
        temporal_backward_step_.store(1, std::memory_order_release);
    }

    if (读取自我I64特征当前值(自我特征定义类::类型_自我_服务时序衰减步长(), 当前值, 调用点 + "/服务时序衰减步长")) {
        service_decay_step_.store((std::max<I64>)(1, 当前值), std::memory_order_release);
    }
    else {
        service_decay_step_.store(1, std::memory_order_release);
    }
}

void 自我类::确保自我根需求与根任务已初始化_(const std::string& 调用点) noexcept
{
    auto* 自我存在 = 确保自我存在_(调用点 + "/自我存在");
    if (!自我存在) return;

    (void)世界树.存在().获取方法根节点(自我存在);

    auto* 安全根需求 = 确保根需求_(
        自我存在,
        语素集.添加词性词("安全根需求", "名词"),
        roots_.安全,
        调用点 + "/安全根需求");
    auto* 服务根需求 = 确保根需求_(
        自我存在,
        语素集.添加词性词("服务根需求", "名词"),
        roots_.服务,
        调用点 + "/服务根需求");

    auto* 安全根任务 = 确保根任务_(
        自我存在,
        语素集.添加词性词("安全根任务", "专有名词"),
        语素集.添加词性词("根任务", "名词"),
        安全根需求,
        调用点 + "/安全根任务");
    auto* 服务根任务 = 确保根任务_(
        自我存在,
        语素集.添加词性词("服务根任务", "专有名词"),
        语素集.添加词性词("根任务", "名词"),
        服务根需求,
        调用点 + "/服务根任务");

    if (安全根需求) {
        安全根需求->主信息.相关任务.指针 = reinterpret_cast<任务节点类*>(安全根任务);
        安全根需求->主信息.相关任务.主键 = 安全根任务 ? 安全根任务->获取主键() : std::string{};
    }
    if (服务根需求) {
        服务根需求->主信息.相关任务.指针 = reinterpret_cast<任务节点类*>(服务根任务);
        服务根需求->主信息.相关任务.主键 = 服务根任务 ? 服务根任务->获取主键() : std::string{};
    }

    auto* 当前主需求 = 获取当前主需求();
    if (!当前主需求) {
        当前主需求 = 选择当前主需求候选_();
    }
    (void)确保任务管理任务_(自我存在, 当前主需求, 调用点 + "/任务管理任务");
}

void 自我类::确保主链镜像已初始化_(const std::string& 调用点) noexcept
{
    const 时间戳 now = 结构体_时间戳::当前_微秒();

    auto* 当前主需求 = 获取当前主需求();
    if (!当前主需求) {
        当前主需求 = 选择当前主需求候选_();
        if (当前主需求) {
            (void)设置当前主需求(当前主需求, now, 调用点 + "/设置当前主需求");
        }
    }

    auto* 当前主任务 = 获取当前主任务();
    if (!当前主任务) {
        当前主任务 = 选择当前主任务候选_(当前主需求);
        if (当前主任务) {
            (void)设置当前主任务(当前主任务, now, 调用点 + "/设置当前主任务");
        }
    }

    auto* 自我存在 = 获取自我存在();
    if (自我存在) {
        (void)确保任务管理任务_(自我存在, 当前主需求, 调用点 + "/回填任务管理任务");
    }
}

需求类::节点类* 自我类::确保根需求_(
    存在节点类* 自我存在,
    const 词性节点类* 需求类型,
    I64 默认权重,
    const std::string& 调用点) noexcept
{
    (void)调用点;
    if (!自我存在 || !需求类型) return nullptr;

    auto* 需求根节点 = 世界树.存在().获取需求根节点(自我存在);
    if (!需求根节点) return nullptr;

    auto* 需求节点 = 私有_查找直接子节点_按谓词<需求类::节点类>(需求根节点, [&](const 需求类::节点类* 节点) noexcept {
        return 节点 && 私有_词性相同(节点->主信息.类型, 需求类型);
    });

    if (!需求节点) {
        需求主信息类 主信息{};
        主信息.类型 = 需求类型;
        主信息.权重 = 默认权重;
        私有_写入引用(主信息.需求主体, 自我存在);
        私有_写入引用(主信息.被需求存在, 自我存在);
        私有_写入引用(主信息.需求场景, 世界树.自我所在场景);
        需求节点 = 私有_独立树添加子节点<需求类>(需求根节点, 主信息);
    }

    if (!需求节点) return nullptr;

    auto& 主信息 = 需求节点->主信息;
    主信息.类型 = 需求类型;
    if (主信息.权重 == 0) {
        主信息.权重 = 默认权重;
    }
    私有_写入引用(主信息.需求主体, 自我存在);
    私有_写入引用(主信息.被需求存在, 自我存在);
    私有_写入引用(主信息.需求场景, 世界树.自我所在场景);
    return 需求节点;
}

任务类::节点类* 自我类::确保根任务_(
    存在节点类* 自我存在,
    const 词性节点类* 任务名称,
    const 词性节点类* 任务类型,
    需求类::节点类* 来源需求,
    const std::string& 调用点) noexcept
{
    (void)调用点;
    if (!自我存在 || !任务名称) return nullptr;

    auto* 任务根节点 = 世界树.存在().获取任务根节点(自我存在);
    if (!任务根节点) return nullptr;

    auto* 任务节点 = 私有_查找直接子节点_按谓词<任务类::节点类>(任务根节点, [&](const 任务类::节点类* 节点) noexcept {
        return 节点 && 私有_词性相同(节点->主信息.名称, 任务名称);
    });

    if (!任务节点) {
        任务主信息类 主信息{};
        主信息.名称 = 任务名称;
        主信息.类型 = 任务类型;
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::未启动;
        私有_写入引用(主信息.来源需求, 来源需求);
        私有_写入引用(主信息.场景, 世界树.自我所在场景);
        if (来源需求) {
            主信息.基准优先级 = 来源需求->主信息.权重;
            主信息.调度优先级 = 来源需求->主信息.权重;
        }
        任务节点 = 私有_独立树添加子节点<任务类>(任务根节点, 主信息);
    }

    if (!任务节点) return nullptr;

    auto& 主信息 = 任务节点->主信息;
    主信息.名称 = 任务名称;
    if (!主信息.类型) {
        主信息.类型 = 任务类型;
    }
    if (主信息.节点种类 == 枚举_任务节点种类::未定义) {
        主信息.节点种类 = 枚举_任务节点种类::头结点;
    }
    if (主信息.状态 == 枚举_任务状态::未定义) {
        主信息.状态 = 枚举_任务状态::未启动;
    }
    私有_写入引用(主信息.来源需求, 来源需求);
    私有_写入引用(主信息.场景, 世界树.自我所在场景);
    if (来源需求 && 主信息.基准优先级 == 0) {
        主信息.基准优先级 = 来源需求->主信息.权重;
    }
    if (来源需求 && 主信息.调度优先级 == 0) {
        主信息.调度优先级 = 来源需求->主信息.权重;
    }
    return 任务节点;
}

任务类::节点类* 自我类::确保任务管理任务_(
    存在节点类* 自我存在,
    需求类::节点类* 当前主需求,
    const std::string& 调用点) noexcept
{
    auto* 治理任务 = 确保根任务_(
        自我存在,
        语素集.添加词性词("任务管理任务", "专有名词"),
        语素集.添加词性词("治理任务", "名词"),
        当前主需求,
        调用点);
    if (!治理任务) return nullptr;

    auto& 主信息 = 治理任务->主信息;
    私有_写入引用(主信息.来源需求, 当前主需求);
    if (当前主需求 && 主信息.调度优先级 == 0) {
        主信息.调度优先级 = 当前主需求->主信息.权重;
    }
    return 治理任务;
}

需求类::节点类* 自我类::选择当前主需求候选_() const noexcept
{
    auto* 安全根需求 = 获取安全根需求();
    auto* 服务根需求 = 获取服务根需求();

    if (!安全根需求) return 服务根需求;
    if (!服务根需求) return 安全根需求;

    const long double 安全压力 =
        static_cast<long double>(安全根方向差值()) *
        static_cast<long double>((std::max<I64>)(roots_.安全, 1));
    const long double 服务压力 =
        static_cast<long double>(私有_饱和减(根目标服务值(), 获取服务值())) *
        static_cast<long double>((std::max<I64>)(roots_.服务, 1));

    return 安全压力 >= 服务压力 ? 安全根需求 : 服务根需求;
}

任务类::节点类* 自我类::选择当前主任务候选_(需求类::节点类* 当前主需求) const noexcept
{
    auto* 安全根需求 = 获取安全根需求();
    auto* 服务根需求 = 获取服务根需求();
    if (当前主需求 && 当前主需求 == 服务根需求) {
        return 获取服务根任务();
    }
    if (当前主需求 && 当前主需求 == 安全根需求) {
        return 获取安全根任务();
    }

    if (auto* 治理任务 = 获取任务管理任务()) {
        return 治理任务;
    }
    if (auto* 安全根任务 = 获取安全根任务()) {
        return 安全根任务;
    }
    return 获取服务根任务();
}

void 自我类::同步运行态特征_(const std::string& 调用点) noexcept
{
    const 时间戳 now = 结构体_时间戳::当前_微秒();
    (void)写入自我I64特征当前值(
        自我特征定义类::类型_自我_待机状态(),
        standby_mode_.load(std::memory_order_acquire) ? 1 : 0,
        now,
        调用点 + "/待机状态");
    (void)写入自我I64特征当前值(
        自我特征定义类::类型_自我_时序正向步长(),
        temporal_forward_step_.load(std::memory_order_acquire),
        now,
        调用点 + "/时序正向步长");
    (void)写入自我I64特征当前值(
        自我特征定义类::类型_自我_时序反向步长(),
        temporal_backward_step_.load(std::memory_order_acquire),
        now,
        调用点 + "/时序反向步长");
    (void)写入自我I64特征当前值(
        自我特征定义类::类型_自我_服务时序衰减步长(),
        service_decay_step_.load(std::memory_order_acquire),
        now,
        调用点 + "/服务时序衰减步长");
}

void 自我类::重置时序步长为默认值_() noexcept
{
    temporal_forward_step_.store(1, std::memory_order_release);
    temporal_backward_step_.store(1, std::memory_order_release);
    service_decay_step_.store(1, std::memory_order_release);
    standby_mode_.store(false, std::memory_order_release);
}

void 自我类::重算时序回归步长_按当前双值_() noexcept
{
    const I64 当前服务值 = 获取服务值();
    const I64 当前安全值 = 获取安全值();
    temporal_forward_step_.store(计算安全增加步长_按服务值_(当前服务值), std::memory_order_release);
    temporal_backward_step_.store(计算安全减少步长_按服务值_(当前服务值), std::memory_order_release);
    service_decay_step_.store(计算服务衰减步长_按双值_(当前服务值, 当前安全值), std::memory_order_release);
    同步运行态特征_("自我类::重算时序回归步长_按当前双值");
}

I64 自我类::计算服务衰减步长_按双值_(I64 服务值, I64 安全值) const noexcept
{
    I64 步长 = 1;
    const I64 服务高位阈值 = 服务高位阈值_();
    if (服务值 < 服务高位阈值) {
        步长 = 私有_饱和加(步长, (服务高位阈值 - 服务值) / 100);
    }

    const I64 安全下沿 = 安全稳定带下限值_();
    if (安全值 < 安全下沿) {
        const I64 安全压制值 = 安全下沿 - 安全值;
        步长 = 步长 > 安全压制值 ? (步长 - 安全压制值) : 1;
    }

    return (std::max<I64>)(1, 步长);
}

I64 自我类::计算安全增加步长_按服务值_(I64 服务值) const noexcept
{
    I64 步长 = 1;
    const I64 服务高位阈值 = 服务高位阈值_();
    if (服务值 > 服务高位阈值) {
        步长 = 私有_饱和加(步长, 服务值 - 服务高位阈值);
    }
    else if (服务值 < 服务低位阈值_()) {
        const I64 压制值 = 服务低位阈值_() - 服务值;
        步长 = 步长 > 压制值 ? (步长 - 压制值) : 1;
    }
    return (std::max<I64>)(1, 步长);
}

I64 自我类::计算安全减少步长_按服务值_(I64 服务值) const noexcept
{
    I64 步长 = 1;
    if (服务值 < 服务低位阈值_()) {
        步长 = 私有_饱和加(步长, 服务低位阈值_() - 服务值);
    }
    else if (服务值 > 服务高位阈值_()) {
        const I64 压制值 = 服务值 - 服务高位阈值_();
        步长 = 步长 > 压制值 ? (步长 - 压制值) : 1;
    }
    return (std::max<I64>)(1, 步长);
}

I64 自我类::读取自我I64特征当前值_(
    const 词性节点类* 特征类型,
    I64 缺省值,
    const std::string& 调用点) const noexcept
{
    I64 当前值 = 0;
    if (读取自我I64特征当前值(特征类型, 当前值, 调用点)) {
        return 当前值;
    }
    return 缺省值;
}

void 自我类::安全值_上升_(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept
{
    const I64 新值 = 私有_饱和加(获取安全值(), delta);
    (void)写入自我I64特征当前值(
        自我特征定义类::类型_自我_安全值(),
        新值,
        结构体_时间戳::当前_微秒(),
        "自我类::安全值_上升_");
    重算时序回归步长_按当前双值_();
    (void)原因类别;
    (void)原因说明;
}

void 自我类::安全值_下降_(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept
{
    const I64 新值 = 私有_饱和减(获取安全值(), delta);
    (void)写入自我I64特征当前值(
        自我特征定义类::类型_自我_安全值(),
        新值,
        结构体_时间戳::当前_微秒(),
        "自我类::安全值_下降_");
    重算时序回归步长_按当前双值_();
    (void)原因类别;
    (void)原因说明;
}

void 自我类::服务值_上升_(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept
{
    const I64 新值 = 私有_饱和加(获取服务值(), delta);
    (void)写入自我I64特征当前值(
        自我特征定义类::类型_自我_服务值(),
        新值,
        结构体_时间戳::当前_微秒(),
        "自我类::服务值_上升_");
    重算时序回归步长_按当前双值_();
    (void)原因类别;
    (void)原因说明;
}

void 自我类::服务值_下降_(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept
{
    const I64 新值 = 私有_饱和减(获取服务值(), delta);
    (void)写入自我I64特征当前值(
        自我特征定义类::类型_自我_服务值(),
        新值,
        结构体_时间戳::当前_微秒(),
        "自我类::服务值_下降_");
    重算时序回归步长_按当前双值_();
    (void)原因类别;
    (void)原因说明;
}
