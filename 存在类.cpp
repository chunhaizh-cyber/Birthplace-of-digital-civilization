#include "存在类.h"

#include <cmath>
#include <limits>

#include "需求类.h"
#include "任务类.h"
#include "方法类.h"
#include "特征类.h"
#include "语素类.h"


namespace {

    // 功能：计算权重、状态、差值或派生结果。
    std::int64_t 私有_计算距离_mm(const Vector3D& 左位置, const Vector3D& 右位置) noexcept
    {
        const long double dx = static_cast<long double>(左位置.x) - static_cast<long double>(右位置.x);
        const long double dy = static_cast<long double>(左位置.y) - static_cast<long double>(右位置.y);
        const long double dz = static_cast<long double>(左位置.z) - static_cast<long double>(右位置.z);
        return static_cast<std::int64_t>(std::llround(std::sqrt(dx * dx + dy * dy + dz * dz)));
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征模板入口性(const char* 名称) noexcept
    {
        return 名称 && *名称
            ? 语素集.添加信息入口词(名称, 枚举_信息入口类型::特征模板入口)
            : nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在场景绝对坐标X() noexcept
    {
        return 私有_特征模板入口性("存在_场景绝对坐标X");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在场景绝对坐标Y() noexcept
    {
        return 私有_特征模板入口性("存在_场景绝对坐标Y");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在场景绝对坐标Z() noexcept
    {
        return 私有_特征模板入口性("存在_场景绝对坐标Z");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在场景绝对坐标明确状态() noexcept
    {
        return 私有_特征模板入口性("存在_场景绝对坐标明确状态");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在坐标来源() noexcept
    {
        return 私有_特征模板入口性("存在_坐标来源");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在坐标时间戳() noexcept
    {
        return 私有_特征模板入口性("存在_坐标时间戳");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在坐标置信度() noexcept
    {
        return 私有_特征模板入口性("存在_坐标置信度");
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_坐标分量转I64(double 值, I64& 输出) noexcept
    {
        if (!std::isfinite(值)) return false;
        const long double 下界 = static_cast<long double>((std::numeric_limits<I64>::min)());
        const long double 上界 = static_cast<long double>((std::numeric_limits<I64>::max)());
        const long double 待写 = static_cast<long double>(值);
        if (待写 < 下界 || 待写 > 上界) return false;
        输出 = static_cast<I64>(std::llround(值));
        return true;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_坐标转I64三元(const Vector3D& 坐标, I64& x, I64& y, I64& z) noexcept
    {
        return 私有_坐标分量转I64(坐标.x, x)
            && 私有_坐标分量转I64(坐标.y, y)
            && 私有_坐标分量转I64(坐标.z, z);
    }

    // 功能：服务所在模块的内部辅助流程。
    场景节点类* 私有_取存在所在场景(
        const 基础信息类& 基础信息,
        const 存在节点类* 存在) noexcept
    {
        auto* 父节点 = 存在 ? static_cast<基础信息节点类*>(存在->父) : nullptr;
        return 父节点 && 基础信息.取主信息<场景节点主信息类>(父节点)
            ? static_cast<场景节点类*>(父节点)
            : nullptr;
    }

    // 功能：从指定来源读取数据或状态。
    bool 私有_读取子特征I64(
        const 特征类& 特征服务,
        const 基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        I64& 输出值)
    {
        const auto 值 = 特征服务.读取子特征值_按类型(宿主, 特征类型);
        if (const auto* 标量 = std::get_if<I64>(&值)) {
            输出值 = *标量;
            return true;
        }
        return false;
    }

    // 功能：把处理结果写入指定对象、场景或日志。
    bool 私有_写入子特征I64(
        特征类& 特征服务,
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        I64 值,
        时间戳 now)
    {
        auto* 特征节点 = 特征服务.取或创建子特征_按类型(宿主, 特征类型);
        return 特征节点 ? 特征服务.写入特征值_I64(特征节点, 值, now) : false;
    }

    // 功能：把处理结果写入指定对象、场景或日志。
    bool 私有_写入子特征指针(
        特征类& 特征服务,
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        const void* 指针,
        时间戳 now)
    {
        auto* 特征节点 = 特征服务.取或创建子特征_按类型(宿主, 特征类型);
        if (!特征节点) return false;
        指针句柄 句柄{};
        句柄.指针 = reinterpret_cast<std::uintptr_t>(指针);
        return 特征服务.写入特征值_指针句柄(特征节点, 句柄, now);
    }

    template<class T节点>
    可解析引用<T节点> 私有_生成引用(T节点* 节点)
    {
        可解析引用<T节点> out{};
        out.指针 = 节点;
        if (节点) out.主键 = 节点->获取主键();
        return out;
    }

    template<class T节点>
    bool 私有_引用已存在(const std::vector<可解析引用<T节点>>& 列表, T节点* 节点)
    {
        for (const auto& 项 : 列表) {
            if (项.指针 == 节点) return true;
            if (节点 && !项.主键.empty() && 项.主键 == 节点->获取主键()) return true;
        }
        return false;
    }

    template<class T节点>
    void 私有_追加唯一裸指针(std::vector<T节点*>& 列表, T节点* 节点)
    {
        if (!节点) return;
        const auto 主键 = 节点->获取主键();
        for (auto* 已有 : 列表) {
            if (已有 == 节点) return;
            if (已有 && !主键.empty() && 已有->获取主键() == 主键) return;
        }
        列表.push_back(节点);
    }

    std::vector<基础信息节点类*> 私有_直接存在子节点(
        const 基础信息类& 基础信息,
        const 基础信息节点类* 父节点)
    {
        std::vector<基础信息节点类*> 结果{};
        auto lk = 基础信息.获取读锁();
        auto* 父 = 父节点 ? const_cast<基础信息节点类*>(父节点) : 基础信息.世界根();
        if (!父 || !父->子) return 结果;

        auto* 首节点 = static_cast<基础信息节点类*>(父->子);
        auto* 当前 = 首节点;
        do {
            if (基础信息.取主信息<存在节点主信息类>(当前)) {
                结果.push_back(当前);
            }
            当前 = static_cast<基础信息节点类*>(当前->下);
        } while (当前 && 当前 != 首节点);
        return 结果;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_节点在范围内(const 基础信息节点类* 范围根, const 基础信息节点类* 候选节点) noexcept
    {
        if (!范围根) return true;
        std::size_t 保护计数 = 0;
        auto* 当前 = 候选节点;
        while (当前 && ++保护计数 < 4096) {
            if (当前 == 范围根) return true;
            当前 = static_cast<const 基础信息节点类*>(当前->父);
        }
        return false;
    }

    template<class T节点>
    T节点* 私有_创建独立信息根节点()
    {
        auto* 根节点 = new T节点{};
        根节点->初始化为孤立节点();
        根节点->父 = nullptr;
        根节点->子 = nullptr;
        根节点->同层环头 = 根节点;
        return 根节点;
    }

    // 功能：确保目标结构、状态或前置条件存在并可用。
    bool 私有_确保存在具备三类信息根节点(存在节点主信息类& 主信息)
    {
        bool 已补齐 = false;

        if (!主信息.需求根节点) {
            auto* 根节点 = 私有_创建独立信息根节点<需求类::节点类>();
            主信息.需求根节点 = reinterpret_cast<需求节点类*>(根节点);
            已补齐 = true;
        }
        if (!主信息.任务根节点) {
            auto* 根节点 = 私有_创建独立信息根节点<任务类::节点类>();
            主信息.任务根节点 = reinterpret_cast<任务节点类*>(根节点);
            已补齐 = true;
        }
        if (!主信息.方法根节点) {
            auto* 根节点 = 私有_创建独立信息根节点<方法类::节点类>();
            主信息.方法根节点 = reinterpret_cast<方法节点类*>(根节点);
            已补齐 = true;
        }

        return 已补齐;
    }

}

// 功能：按函数名执行对应处理。
存在类::存在类(基础信息类* 基础信息) noexcept
    : 基础信息_(基础信息)
{
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
void 存在类::绑定基础信息(基础信息类* 基础信息) noexcept
{
    基础信息_ = 基础信息;
}

// 功能：按函数名执行对应处理。
存在节点主信息类* 存在类::取存在主信息(存在节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<存在节点主信息类>(节点) : nullptr;
}

// 功能：按函数名执行对应处理。
const 存在节点主信息类* 存在类::取存在主信息(const 存在节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<存在节点主信息类>(节点) : nullptr;
}

// 功能：判断基础信息节点是否承载存在主信息。
bool 存在类::是存在节点(const 基础信息节点类* 节点) const noexcept
{
    return 节点 && 基础信息_ && 基础信息_->取主信息<存在节点主信息类>(节点);
}

// 功能：读取存在名称入口，不创建或补齐任何结构。
const 语素入口节点类* 存在类::读取存在名称(const 存在节点类* 节点) const noexcept
{
    const auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? 主信息->名称 : nullptr;
}

// 功能：读取存在类型入口，不创建或补齐任何结构。
const 语素入口节点类* 存在类::读取存在类型(const 存在节点类* 节点) const noexcept
{
    const auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? 主信息->类型 : nullptr;
}

// 功能：读取存在内部世界引用，不创建内部世界。
场景节点类* 存在类::读取内部世界(const 存在节点类* 节点) const noexcept
{
    const auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? 主信息->内部世界.指针 : nullptr;
}

// 功能：读取存在概念集引用快照，不解析或扩充概念。
std::vector<可解析引用<存在节点类>> 存在类::读取概念集快照(const 存在节点类* 节点) const
{
    const auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? 主信息->概念集 : std::vector<可解析引用<存在节点类>>{};
}

// 功能：读取存在概念模板引用，不解析或扩充概念。
可解析引用<存在节点类> 存在类::读取概念模板引用(const 存在节点类* 节点) const noexcept
{
    const auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? 主信息->概念模板 : 可解析引用<存在节点类>{};
}

// 功能：从存在特征结构读取场景绝对坐标，不创建特征节点。
bool 存在类::读取存在场景绝对坐标(const 存在节点类* 节点, Vector3D& 输出坐标_mm) const
{
    输出坐标_mm = {};
    if (!基础信息_ || !是存在节点(节点)) return false;

    auto* 宿主 = reinterpret_cast<const 基础信息节点类*>(节点);
    特征类 特征服务(基础信息_);

    I64 明确状态 = 0;
    if (!私有_读取子特征I64(特征服务, 宿主, 私有_特征_存在场景绝对坐标明确状态(), 明确状态)
        || 明确状态 <= 0) {
        return false;
    }

    I64 x = 0;
    I64 y = 0;
    I64 z = 0;
    if (!私有_读取子特征I64(特征服务, 宿主, 私有_特征_存在场景绝对坐标X(), x)
        || !私有_读取子特征I64(特征服务, 宿主, 私有_特征_存在场景绝对坐标Y(), y)
        || !私有_读取子特征I64(特征服务, 宿主, 私有_特征_存在场景绝对坐标Z(), z)) {
        return false;
    }

    输出坐标_mm = Vector3D{
        static_cast<double>(x),
        static_cast<double>(y),
        static_cast<double>(z),
    };
    return true;
}

// 功能：按参考存在绝对坐标和相对坐标计算同场景绝对坐标。
bool 存在类::计算场景绝对坐标_由参考存在相对坐标(
    const 存在节点类* 参考存在,
    const Vector3D& 相对坐标_mm,
    Vector3D& 输出绝对坐标_mm) const
{
    输出绝对坐标_mm = {};
    if (!基础信息_ || !参考存在 || !私有_取存在所在场景(*基础信息_, 参考存在)) return false;

    Vector3D 参考绝对坐标{};
    if (!读取存在场景绝对坐标(参考存在, 参考绝对坐标)) {
        return false;
    }

    const Vector3D 候选{
        参考绝对坐标.x + 相对坐标_mm.x,
        参考绝对坐标.y + 相对坐标_mm.y,
        参考绝对坐标.z + 相对坐标_mm.z,
    };
    I64 x = 0;
    I64 y = 0;
    I64 z = 0;
    if (!私有_坐标转I64三元(候选, x, y, z)) {
        return false;
    }

    输出绝对坐标_mm = Vector3D{
        static_cast<double>(x),
        static_cast<double>(y),
        static_cast<double>(z),
    };
    return true;
}

// 功能：读取最近观测位置缓存；缓存只作为特征化或显示输入。
bool 存在类::读取最近观测位置缓存(const 存在节点类* 节点, Vector3D& 输出位置_mm) const noexcept
{
    const auto* 主信息 = 取存在主信息(节点);
    if (!主信息 || !主信息->有位置历史) return false;
    输出位置_mm = 主信息->最近观测位置_mm;
    return true;
}

// 功能：读取上次观测位置缓存；缓存只作为特征化或显示输入。
bool 存在类::读取上次观测位置缓存(const 存在节点类* 节点, Vector3D& 输出位置_mm) const noexcept
{
    const auto* 主信息 = 取存在主信息(节点);
    if (!主信息 || !主信息->有位置历史) return false;
    输出位置_mm = 主信息->上次观测位置_mm;
    return true;
}

// 功能：确保目标结构、状态或前置条件存在并可用。
bool 存在类::确保存在三类根节点(存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? 私有_确保存在具备三类信息根节点(*主信息) : false;
}

// 功能：读取并返回指定对象、状态或运行材料。
需求类::节点类* 存在类::获取需求根节点(存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<需求类::节点类*>(主信息->需求根节点) : nullptr;
}

// 功能：读取并返回指定对象、状态或运行材料。
const 需求类::节点类* 存在类::获取需求根节点(const 存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<const 需求类::节点类*>(主信息->需求根节点) : nullptr;
}

// 功能：读取并返回指定对象、状态或运行材料。
任务类::节点类* 存在类::获取任务根节点(存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<任务类::节点类*>(主信息->任务根节点) : nullptr;
}

// 功能：读取并返回指定对象、状态或运行材料。
const 任务类::节点类* 存在类::获取任务根节点(const 存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<const 任务类::节点类*>(主信息->任务根节点) : nullptr;
}

// 功能：读取并返回指定对象、状态或运行材料。
方法类::节点类* 存在类::获取方法根节点(存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<方法类::节点类*>(主信息->方法根节点) : nullptr;
}

// 功能：读取并返回指定对象、状态或运行材料。
const 方法类::节点类* 存在类::获取方法根节点(const 存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<const 方法类::节点类*>(主信息->方法根节点) : nullptr;
}

// 功能：创建并返回或登记对应对象。
存在节点类* 存在类::创建存在(基础信息节点类* 父节点, 存在节点主信息类* 主信息)
{
    if (!基础信息_ || !主信息) return nullptr;
    (void)私有_确保存在具备三类信息根节点(*主信息);
    return static_cast<存在节点类*>(基础信息_->添加子节点(父节点, 主信息));
}

// 功能：创建并返回或登记对应对象。
存在节点类* 存在类::创建存在_按名称类型(
    基础信息节点类* 父节点,
    const 语素入口节点类* 名称,
    const 语素入口节点类* 类型)
{
    auto* 主信息 = new 存在节点主信息类{};
    主信息->名称 = 名称;
    主信息->类型 = 类型;
    return 创建存在(父节点, 主信息);
}

// 功能：创建并返回或登记对应对象。
存在节点类* 存在类::取或创建子存在_按名称类型(
    基础信息节点类* 父节点,
    const 语素入口节点类* 名称,
    const 语素入口节点类* 类型)
{
    for (auto* 节点 : 获取子存在(父节点)) {
        const auto* 主信息 = 取存在主信息(节点);
        if (!主信息) continue;

        const bool 名称匹配 = !名称 || 语素入口同一(主信息->名称, 名称);
        const bool 类型匹配 = !类型 || 语素入口同一(主信息->类型, 类型);
        if (名称匹配 && 类型匹配) {
            return 节点;
        }
    }

    return 创建存在_按名称类型(父节点, 名称, 类型);
}

// 功能：创建并返回或登记对应对象。
存在节点类* 存在类::取或创建子存在_按类型(
    基础信息节点类* 父节点,
    const 语素入口节点类* 类型)
{
    if (!类型) return nullptr;
    if (auto* 命中 = 查找子存在_按类型(父节点, 类型)) {
        return 命中;
    }
    return 创建存在_按名称类型(父节点, nullptr, 类型);
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 存在类::确保存在概念(
    基础信息节点类* 概念父节点,
    const 语素入口节点类* 名称)
{
    if (!基础信息_ || !概念父节点 || !名称) return nullptr;
    if (auto* 命中 = 查找子存在_按名称(概念父节点, 名称)) {
        return 命中;
    }
    return 创建存在_按名称类型(概念父节点, 名称, nullptr);
}

// 功能：创建并返回或登记对应对象。
存在节点类* 存在类::创建存在实例(
    基础信息节点类* 父节点,
    const 语素入口节点类* 名称,
    const std::vector<存在节点类*>& 输入概念集)
{
    if (!基础信息_) return nullptr;
    auto* 实例 = 创建存在_按名称类型(
        父节点 ? 父节点 : 基础信息_->世界根(),
        名称,
        nullptr);
    if (!实例) return nullptr;

    for (auto* 概念 : 输入概念集) {
        if (!概念 || 概念 == 实例) continue;
        (void)添加实例概念(实例, 概念, false);
    }
    return 实例;
}

// 功能：删除指定对象、状态或登记项。
bool 存在类::删除存在(存在节点类* 节点)
{
    if (!基础信息_ || !节点) return false;

    基础信息_->释放节点主信息递归(节点);
    return 基础信息_->删除节点(节点);
}

// 功能：按函数名执行对应处理。
std::vector<存在节点类*> 存在类::枚举全部存在() const
{
    std::vector<存在节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举节点_按类型<存在节点主信息类>()) {
        out.push_back(static_cast<存在节点类*>(节点));
    }
    return out;
}

// 功能：读取并返回指定对象、状态或运行材料。
std::vector<存在节点类*> 存在类::获取子存在(const 基础信息节点类* 父节点) const
{
    std::vector<存在节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 私有_直接存在子节点(*基础信息_, 父节点)) {
        out.push_back(static_cast<存在节点类*>(节点));
    }
    return out;
}

// 功能：按条件查找目标对象、方法或事实。
存在节点类* 存在类::查找子存在_按名称(const 基础信息节点类* 父节点, const 语素入口节点类* 名称) const
{
    if (!名称) return nullptr;

    for (auto* 节点 : 获取子存在(父节点)) {
        const auto* 主信息 = 取存在主信息(节点);
        if (主信息 && 语素入口同一(主信息->名称, 名称)) {
            return 节点;
        }
    }
    return nullptr;
}

// 功能：按条件查找目标对象、方法或事实。
存在节点类* 存在类::查找子存在_按类型(const 基础信息节点类* 父节点, const 语素入口节点类* 类型) const
{
    if (!类型) return nullptr;

    for (auto* 节点 : 获取子存在(父节点)) {
        const auto* 主信息 = 取存在主信息(节点);
        if (主信息 && 语素入口同一(主信息->类型, 类型)) {
            return 节点;
        }
    }
    return nullptr;
}

// 功能：读取并返回指定对象、状态或运行材料。
std::vector<存在节点类*> 存在类::获取概念集(const 存在节点类* 节点) const
{
    std::vector<存在节点类*> out;
    const auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return out;

    const auto 解析概念引用 = [this](const 可解析引用<存在节点类>& 引用) -> 存在节点类* {
        if (引用.指针) return 引用.指针;
        if (!基础信息_ || 引用.主键.empty()) return nullptr;
        return dynamic_cast<存在节点类*>(基础信息_->查找主键(引用.主键));
    };

    for (const auto& 概念引用 : 主信息->概念集) {
        私有_追加唯一裸指针(out, 解析概念引用(概念引用));
    }
    私有_追加唯一裸指针(out, 解析概念引用(主信息->概念模板));
    return out;
}

// 功能：按函数名执行对应处理。
bool 存在类::存在包含概念(const 存在节点类* 实例, const 存在节点类* 概念) const noexcept
{
    if (!实例 || !概念) return false;
    const auto* 主信息 = 取存在主信息(实例);
    if (!主信息) return false;
    const auto 概念主键 = 概念->获取主键();
    const auto 引用匹配 = [&](const 可解析引用<存在节点类>& 引用) noexcept {
        if (引用.指针 == 概念) return true;
        return !概念主键.empty() && !引用.主键.empty() && 引用.主键 == 概念主键;
    };
    if (引用匹配(主信息->概念模板)) return true;
    for (const auto& 引用 : 主信息->概念集) {
        if (引用匹配(引用)) return true;
    }
    return false;
}

// 功能：按条件查找目标对象、方法或事实。
std::vector<存在节点类*> 存在类::查找实例_按概念(
    const 存在节点类* 概念,
    const 基础信息节点类* 范围根) const
{
    std::vector<存在节点类*> out;
    if (!概念) return out;
    for (auto* 候选 : 枚举全部存在()) {
        if (!候选 || 候选 == 概念 || !私有_节点在范围内(范围根, 候选)) {
            continue;
        }
        if (存在包含概念(候选, 概念)) {
            out.push_back(候选);
        }
    }
    return out;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 存在类::写入名称(存在节点类* 节点, const 语素入口节点类* 名称)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    主信息->名称 = 名称;
    return true;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 存在类::写入类型(存在节点类* 节点, const 语素入口节点类* 类型)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    主信息->类型 = 类型;
    return true;
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
bool 存在类::绑定概念模板(存在节点类* 节点, 存在节点类* 概念模板)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    主信息->概念模板 = 私有_生成引用(概念模板);
    if (概念模板 && !私有_引用已存在(主信息->概念集, 概念模板)) {
        主信息->概念集.push_back(私有_生成引用(概念模板));
    }
    return true;
}

// 功能：按函数名执行对应处理。
bool 存在类::添加实例概念(
    存在节点类* 实例,
    存在节点类* 概念,
    bool 是否扩充记忆)
{
    (void)是否扩充记忆;
    if (!实例 || !概念 || 实例 == 概念) return false;
    auto* 主信息 = 取存在主信息(实例);
    if (!主信息) return false;

    bool 已添加 = false;
    if (!私有_引用已存在(主信息->概念集, 概念)) {
        主信息->概念集.push_back(私有_生成引用(概念));
        已添加 = true;
    }
    if (!主信息->概念模板.有效()) {
        主信息->概念模板 = 私有_生成引用(概念);
    }
    return 已添加;
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
bool 存在类::绑定内部世界(存在节点类* 节点, 场景节点类* 内部世界)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    主信息->内部世界 = 私有_生成引用(内部世界);
    return true;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 存在类::写入观测位置(存在节点类* 节点, const Vector3D& 位置_mm)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;

    // 位置历史是最近观测缓存；不要从这些字段直接派生任务 / 需求 / 结算事实。
    const bool 已有历史 = 主信息->有位置历史;
    const auto 原最近位置 = 主信息->最近观测位置_mm;
    if (已有历史) {
        主信息->上次观测位置_mm = 原最近位置;
        主信息->最近位移_mm = 私有_计算距离_mm(原最近位置, 位置_mm);
    } else {
        主信息->上次观测位置_mm = 位置_mm;
        主信息->最近位移_mm = 0;
    }

    主信息->最近观测位置_mm = 位置_mm;
    主信息->有位置历史 = true;
    主信息->连续未命中帧 = 0;
    if (已有历史 && 主信息->最近位移_mm == 0) {
        ++主信息->连续静止帧;
    } else {
        主信息->连续静止帧 = 0;
    }
    return true;
}

// 功能：把存在场景绝对坐标写入特征结构，并同步最近观测缓存。
bool 存在类::写入存在场景绝对坐标(
    存在节点类* 节点,
    const Vector3D& 坐标_mm,
    时间戳 now,
    I64 置信度)
{
    if (!基础信息_ || !是存在节点(节点)) return false;

    I64 x = 0;
    I64 y = 0;
    I64 z = 0;
    if (!私有_坐标转I64三元(坐标_mm, x, y, z)) {
        return false;
    }

    auto* 宿主 = reinterpret_cast<基础信息节点类*>(节点);
    特征类 特征服务(基础信息_);

    bool 全部成功 = true;
    全部成功 = 私有_写入子特征I64(特征服务, 宿主, 私有_特征_存在场景绝对坐标X(), x, now) && 全部成功;
    全部成功 = 私有_写入子特征I64(特征服务, 宿主, 私有_特征_存在场景绝对坐标Y(), y, now) && 全部成功;
    全部成功 = 私有_写入子特征I64(特征服务, 宿主, 私有_特征_存在场景绝对坐标Z(), z, now) && 全部成功;
    全部成功 = 私有_写入子特征I64(特征服务, 宿主, 私有_特征_存在场景绝对坐标明确状态(), 1, now) && 全部成功;
    全部成功 = 私有_写入子特征I64(特征服务, 宿主, 私有_特征_存在坐标时间戳(), static_cast<I64>(now), now) && 全部成功;
    全部成功 = 私有_写入子特征I64(特征服务, 宿主, 私有_特征_存在坐标置信度(), 置信度, now) && 全部成功;

    if (全部成功) {
        (void)写入观测位置(节点, Vector3D{
            static_cast<double>(x),
            static_cast<double>(y),
            static_cast<double>(z),
        });
    }
    return 全部成功;
}

// 功能：按参考存在相对坐标写入目标存在场景绝对坐标和坐标来源。
bool 存在类::写入存在场景绝对坐标_由参考存在相对坐标(
    存在节点类* 目标存在,
    const 存在节点类* 参考存在,
    const Vector3D& 相对坐标_mm,
    时间戳 now,
    I64 置信度)
{
    if (!基础信息_ || !目标存在 || !参考存在) return false;

    auto* 目标场景 = 私有_取存在所在场景(*基础信息_, 目标存在);
    auto* 参考场景 = 私有_取存在所在场景(*基础信息_, 参考存在);
    if (!目标场景 || !参考场景 || 目标场景 != 参考场景) {
        return false;
    }

    Vector3D 绝对坐标{};
    if (!计算场景绝对坐标_由参考存在相对坐标(参考存在, 相对坐标_mm, 绝对坐标)) {
        return false;
    }

    if (!写入存在场景绝对坐标(目标存在, 绝对坐标, now, 置信度)) {
        return false;
    }

    特征类 特征服务(基础信息_);
    return 私有_写入子特征指针(
        特征服务,
        reinterpret_cast<基础信息节点类*>(目标存在),
        私有_特征_存在坐标来源(),
        参考存在,
        now);
}

// 功能：按函数名执行对应处理。
bool 存在类::标记观测未命中(存在节点类* 节点)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;

    // 未命中计数是观测统计缓存；权威缺失事实应由特征或二次特征沉淀。
    ++主信息->连续未命中帧;
    主信息->连续静止帧 = 0;
    return true;
}
