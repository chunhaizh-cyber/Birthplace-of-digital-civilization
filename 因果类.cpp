#include "因果类.h"
#include "动态类.h"
#include "方法类.h"
#include "语素类.h"

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

import 自我类.特征定义;

namespace {

template<class T节点>
static std::string 私有_节点键(const T节点* 节点)
{
    if (!节点) return {};
    return 节点->获取主键();
}

template<class T节点>
static std::string 私有_引用键(const 可解析引用<T节点>& 引用)
{
    if (!引用.主键.empty()) return 引用.主键;
    return 引用.指针 ? 引用.指针->获取主键() : std::string{};
}

template<class T节点>
static bool 私有_引用等于节点(const 可解析引用<T节点>& 引用, const T节点* 节点)
{
    if (!节点) return false;
    const auto k = 私有_引用键(引用);
    return !k.empty() && k == 节点->获取主键();
}

template<class T节点>
static 可解析引用<T节点> 私有_制引用(T节点* 节点)
{
    可解析引用<T节点> r{};
    r.指针 = 节点;
    if (节点) r.主键 = 节点->获取主键();
    return r;
}

static std::string 私有_指针键(const void* p)
{
    if (!p) return {};
    std::ostringstream os;
    os << "ptr:" << reinterpret_cast<std::uintptr_t>(p);
    return os.str();
}

static std::string 私有_引用键(const 可解析引用<方法节点类>& 引用)
{
    if (!引用.主键.empty()) return 引用.主键;
    // 方法节点类在因果主信息层以弱引用出现；这里不强制包含其完整定义，
    // 避免把因果沉淀工具耦合到方法树节点实现。
    return 私有_指针键(引用.指针);
}

static void 私有_追加片段(std::string& out, const std::string& s)
{
    if (!out.empty()) out.push_back('|');
    out += s.empty() ? "_" : s;
}

static std::string 私有_状态签名(const 状态节点类* 状态)
{
    if (!状态) return {};
    auto* m = dynamic_cast<const 状态节点主信息类*>(状态->主信息);
    if (!m) return 私有_节点键(状态);

    std::string out;
    私有_追加片段(out, 私有_引用键(m->状态主体));
    私有_追加片段(out, 私有_引用键(m->状态特征));
    // 状态值的内部表达可能继续演化；第一版用状态节点主键保持证据可追溯。
    私有_追加片段(out, 私有_节点键(状态));
    return out;
}

static std::string 私有_动作名键(const 语素入口节点类* 名)
{
    return 私有_指针键(名);
}

static bool 私有_向量含动态(const std::vector<可解析引用<动态节点类>>& v, const 动态节点类* d)
{
    return std::any_of(v.begin(), v.end(), [&](const auto& r) { return 私有_引用等于节点(r, d); });
}

static bool 私有_向量含二次特征(const std::vector<可解析引用<二次特征节点类>>& v, const 二次特征节点类* n)
{
    return std::any_of(v.begin(), v.end(), [&](const auto& r) { return 私有_引用等于节点(r, n); });
}

static bool 私有_向量含特征(const std::vector<可解析引用<特征节点类>>& v, const 特征节点类* n)
{
    return std::any_of(v.begin(), v.end(), [&](const auto& r) { return 私有_引用等于节点(r, n); });
}

static const char* 私有_形态文本(const 因果主信息类* m)
{
    if (!m) return "未知因果";
    switch (m->形态) {
    case 枚举_因果形态::实例: return "实例因果";
    case 枚举_因果形态::抽象: return "抽象因果";
    default: return "未知因果";
    }
}

static bool 私有_因果模板有动作(const 因果主信息类* m) noexcept
{
    return m
        && (m->因方法模板.有效()
            || !m->因动作语义键.empty()
            || !m->因动作名称.empty());
}

// 因果主信息不再持久化置信度；需要排序或投影时由成立/失败基础计数临时派生。
static std::int64_t 私有_派生置信分(const 因果主信息类* m) noexcept
{
    if (!m) return 0;
    const auto 稳定度 = m->计算稳定度();
    return 稳定度 > 0 ? 稳定度 : 0;
}

static bool 私有_动态结果命中状态(const 动态节点类* 动态, const 状态节点类* 目标状态) noexcept
{
    auto* m = 动态 ? dynamic_cast<const 动态节点主信息类*>(动态->主信息) : nullptr;
    return m && 私有_引用等于节点(m->结果状态, 目标状态);
}

static bool 私有_状态迁移结果命中状态(const 二次特征节点类* 迁移, const 状态节点类* 目标状态) noexcept
{
    auto* m = 迁移 ? dynamic_cast<const 二次特征主信息类*>(迁移->主信息) : nullptr;
    return m && 私有_引用等于节点(m->来源结果状态, 目标状态);
}

static bool 私有_因果模板结果命中状态(
    const 因果主信息类* 模板,
    const 状态节点类* 目标状态) noexcept
{
    if (!模板 || !目标状态) return false;

    for (const auto& 证据 : 模板->证据动态样本) {
        if (私有_动态结果命中状态(证据.指针, 目标状态)) {
            return true;
        }
    }

    if (私有_状态迁移结果命中状态(模板->主果比较模板.指针, 目标状态)) {
        return true;
    }
    for (const auto& 迁移 : 模板->果比较模板) {
        if (私有_状态迁移结果命中状态(迁移.指针, 目标状态)) {
            return true;
        }
    }
    return false;
}

static I64 私有_方法本能ID(const 方法类::节点类* 方法) noexcept
{
    auto* 首 = 方法 ? 方法->主信息.取首节点信息() : nullptr;
    if (!首 || 首->动作句柄.类型 != 枚举_动作句柄类型::本能函数ID) {
        return 0;
    }
    return 首->动作句柄.本能ID;
}

static bool 私有_方法动作匹配因果模板(
    const 方法类::节点类* 方法,
    const 因果主信息类* 模板) noexcept
{
    auto* 首 = 方法 ? 方法->主信息.取首节点信息() : nullptr;
    if (!首 || !模板) return false;

    if (模板->因方法模板.有效()) {
        if (模板->因方法模板.指针 == reinterpret_cast<const 方法节点类*>(方法)) {
            return true;
        }
        if (!模板->因方法模板.主键.empty()
            && 模板->因方法模板.主键 == 方法->获取主键()) {
            return true;
        }
    }

    if (!模板->因动作名称.empty()
        && 首->动作名
        && 私有_动作名键(首->动作名) == 模板->因动作名称) {
        return true;
    }

    if (!模板->因动作语义键.empty()) {
        if (方法->获取主键() == 模板->因动作语义键) {
            return true;
        }
        if (首->动作名 && 首->动作名->获取主键() == 模板->因动作语义键) {
            return true;
        }
        if (私有_方法本能ID(方法) != 0
            && std::to_string(私有_方法本能ID(方法)) == 模板->因动作语义键) {
            return true;
        }
    }

    return false;
}

static std::vector<方法类::节点类*> 私有_查找可映射动作方法(
    方法类::节点类* 方法根节点,
    const 因果主信息类* 模板)
{
    std::vector<方法类::节点类*> out;
    if (!方法根节点 || !方法根节点->子 || !模板) return out;

    std::unordered_set<std::string> 已加入{};
    auto* 首节点 = static_cast<方法类::节点类*>(方法根节点->子);
    auto* 当前 = 首节点;
    std::size_t 保护计数 = 0;
    do {
        if (当前
            && 当前->主信息.节点种类() == 枚举_方法节点种类::方法首节点
            && 私有_方法动作匹配因果模板(当前, 模板)) {
            const auto 主键 = 当前->获取主键();
            if (已加入.insert(主键).second) {
                out.push_back(当前);
            }
        }

        当前 = static_cast<方法类::节点类*>(当前->下);
        ++保护计数;
    } while (当前 && 当前 != 首节点 && 保护计数 < 100000);

    return out;
}

static bool 私有_目标特征是安全账务值(const 特征节点类* 特征) noexcept;
static bool 私有_目标特征是服务账务值(const 特征节点类* 特征) noexcept;
static bool 私有_目标特征是任务层状态(const 特征节点类* 特征) noexcept;
static bool 私有_目标特征是方法层状态(const 特征节点类* 特征) noexcept;
static bool 私有_目标特征是学习层状态(const 特征节点类* 特征) noexcept;

static const 特征节点类* 私有_动态目标特征(const 动态节点主信息类* 动态) noexcept
{
    if (!动态) return nullptr;
    if (动态->动态特征.指针) return 动态->动态特征.指针;

    auto* 结果状态信息 = 动态->结果状态.指针
        ? dynamic_cast<const 状态节点主信息类*>(动态->结果状态.指针->主信息)
        : nullptr;
    if (结果状态信息 && 结果状态信息->状态特征.指针) {
        return 结果状态信息->状态特征.指针;
    }

    auto* 初始状态信息 = 动态->初始状态.指针
        ? dynamic_cast<const 状态节点主信息类*>(动态->初始状态.指针->主信息)
        : nullptr;
    return 初始状态信息 ? 初始状态信息->状态特征.指针 : nullptr;
}

static bool 私有_动态有状态转换事实(const 动态节点主信息类* 动态) noexcept
{
    return 动态 && (动态->初始状态.指针 || 动态->结果状态.指针);
}

static bool 私有_动态有动作来源(const 动态节点主信息类* 动态) noexcept
{
    return 动态
        && (动态->来源动作.有效()
            || 动态->来源动作名
            || 动态->来源类型 == 枚举_动态来源类型::方法调用);
}

static 枚举_状态转换因果边类型 私有_推断边类型(const 动态节点主信息类* 动态) noexcept
{
    if (!动态) return 枚举_状态转换因果边类型::未定义;

    const auto* 目标特征 = 私有_动态目标特征(动态);
    if (私有_目标特征是安全账务值(目标特征)
        || 私有_目标特征是服务账务值(目标特征)) {
        return 枚举_状态转换因果边类型::账务因果;
    }

    if (动态->来源类型 == 枚举_动态来源类型::内部机制动作
        || 私有_目标特征是任务层状态(目标特征)
        || 私有_目标特征是方法层状态(目标特征)
        || 私有_目标特征是学习层状态(目标特征)) {
        return 枚举_状态转换因果边类型::治理因果;
    }

    if (私有_动态有状态转换事实(动态)
        && !私有_动态有动作来源(动态)) {
        return 枚举_状态转换因果边类型::观测因果;
    }

    if (私有_动态有动作来源(动态)) {
        return 枚举_状态转换因果边类型::物理因果;
    }
    return 枚举_状态转换因果边类型::引用关系;
}

static 结构_状态转换因果节点 私有_构造状态转换因果节点(
    状态节点类* 状态,
    动态节点类* 来源动态,
    因果模板节点类* 来源模板,
    因果实例节点类* 来源实例)
{
    结构_状态转换因果节点 节点{};
    节点.状态节点 = 状态;
    节点.来源动态 = 来源动态;
    节点.来源因果模板 = 来源模板;
    节点.来源因果实例 = 来源实例;

    auto* 状态信息 = 状态 ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
    if (状态信息) {
        节点.宿主存在 = 状态信息->状态主体.指针;
        节点.特征类型 = 状态信息->状态特征.指针;
        节点.状态时间 = 状态信息->发生时间 ? 状态信息->发生时间 : 状态信息->收到时间;
    }

    auto* 动态信息 = 来源动态 ? dynamic_cast<动态节点主信息类*>(来源动态->主信息) : nullptr;
    if (动态信息) {
        节点.来源场景 = 动态信息->来源输出场景.指针
            ? 动态信息->来源输出场景.指针
            : 动态信息->来源输入场景.指针;
    }

    std::string 签名 = "NODE";
    私有_追加片段(签名, 私有_节点键(节点.宿主存在));
    私有_追加片段(签名, 私有_节点键(节点.特征类型));
    私有_追加片段(签名, 私有_状态签名(状态));
    节点.节点签名 = 签名;
    return 节点;
}

static 结构_状态转换因果边 私有_构造动态状态转换边(动态节点类* 动态)
{
    结构_状态转换因果边 边{};
    auto* 动态信息 = 动态 ? dynamic_cast<动态节点主信息类*>(动态->主信息) : nullptr;
    if (!动态信息) return 边;

    边.原因状态 = 动态信息->初始状态.指针;
    边.结果状态 = 动态信息->结果状态.指针;
    边.动作动态 = 动态;
    边.边类型 = 私有_推断边类型(动态信息);
    边.置信度 = 1;
    边.稳定度 = 动态信息->来源执行成功 ? 1 : 0;

    std::string 签名 = "EDGE";
    私有_追加片段(签名, 私有_状态签名(边.原因状态));
    私有_追加片段(签名, 私有_节点键(动态));
    私有_追加片段(签名, 私有_状态签名(边.结果状态));
    边.边签名 = 签名;
    return 边;
}

static bool 私有_语素入口同一(const 语素入口节点类* 左, const 语素入口节点类* 右) noexcept
{
    if (左 == 右) return true;
    if (!左 || !右) return false;
    return 左->获取主键() == 右->获取主键();
}

static const 语素入口节点类* 私有_只读查找特征类型词(const char* 词) noexcept
{
    if (!词 || !*词) return nullptr;
    try {
        auto* 词节点 = 语素集.获取词节点指针(词);
        return 词节点 ? 语素集.查找信息入口节点(词节点, 枚举_信息入口类型::特征模板入口) : nullptr;
    } catch (...) {
        return nullptr;
    }
}

static const 语素入口节点类* 私有_特征语义类型(const 特征节点类* 特征) noexcept
{
    auto* 主信息 = 特征 ? dynamic_cast<const 特征节点主信息类*>(特征->主信息) : nullptr;
    return 主信息 ? 主信息->类型 : nullptr;
}

static bool 私有_特征类型同一(const 特征节点类* 特征, const 语素入口节点类* 期望类型) noexcept
{
    return 私有_语素入口同一(私有_特征语义类型(特征), 期望类型);
}

static bool 私有_特征类型同一_只读词(const 特征节点类* 特征, const char* 期望词) noexcept
{
    return 私有_特征类型同一(特征, 私有_只读查找特征类型词(期望词));
}

static bool 私有_目标特征是安全账务值(const 特征节点类* 特征) noexcept
{
    return 私有_特征类型同一(特征, 自我特征定义类::类型_自我_安全值());
}

static bool 私有_目标特征是服务账务值(const 特征节点类* 特征) noexcept
{
    return 私有_特征类型同一(特征, 自我特征定义类::类型_自我_服务值());
}

static bool 私有_目标特征是直接安全目标(const 特征节点类* 特征) noexcept
{
    return 私有_特征类型同一(特征, 自我特征定义类::类型_自我_物理安全())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_自我_风险安全());
}

static bool 私有_目标特征是任务层状态(const 特征节点类* 特征) noexcept
{
    return 私有_特征类型同一(特征, 自我特征定义类::类型_任务_状态())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_任务_局部完成度())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_任务_局部运行状态())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_任务管理_结果状态())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_任务管理_任务执行记账结果());
}

static bool 私有_目标特征是方法层状态(const 特征节点类* 特征) noexcept
{
    return 私有_特征类型同一_只读词(特征, "方法状态")
        || 私有_特征类型同一_只读词(特征, "方法动作状态")
        || 私有_特征类型同一_只读词(特征, "方法动作数量")
        || 私有_特征类型同一_只读词(特征, "方法条件结果配对状态")
        || 私有_特征类型同一_只读词(特征, "方法稳定状态")
        || 私有_特征类型同一_只读词(特征, "方法运行账完整性")
        || 私有_特征类型同一_只读词(特征, "条件结果对状态")
        || 私有_特征类型同一_只读词(特征, "条件结果对数量")
        || 私有_特征类型同一_只读词(特征, "方法成熟度阶段");
}

static bool 私有_目标特征是学习层状态(const 特征节点类* 特征) noexcept
{
    return 私有_特征类型同一(特征, 自我特征定义类::类型_学习_目标状态())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_学习_信息补齐状态())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_学习_因果验证状态())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_学习_方法可靠度状态())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_学习_风险评估状态());
}

static 枚举_目标投影维度 私有_按特征语义推断目标维度(const 特征节点类* 特征)
{
    if (私有_目标特征是安全账务值(特征)
        || 私有_目标特征是直接安全目标(特征)) {
        return 枚举_目标投影维度::安全;
    }
    if (私有_目标特征是服务账务值(特征)) return 枚举_目标投影维度::服务;
    if (私有_目标特征是学习层状态(特征)) return 枚举_目标投影维度::学习;
    if (私有_目标特征是方法层状态(特征)) return 枚举_目标投影维度::方法维护;
    if (私有_特征类型同一(特征, 自我特征定义类::类型_自我_待处理方法数量())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_自我_可用方法数量())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_自我_当前主需求())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_自我_当前主任务())
        || 私有_特征类型同一(特征, 自我特征定义类::类型_自我_当前主方法())) {
        return 枚举_目标投影维度::自维护;
    }
    return 枚举_目标投影维度::其他;
}

static 枚举_目标权重类型 私有_目标维度到权重类型(枚举_目标投影维度 维度) noexcept
{
    switch (维度) {
    case 枚举_目标投影维度::安全: return 枚举_目标权重类型::安全权重;
    case 枚举_目标投影维度::服务: return 枚举_目标权重类型::服务权重;
    case 枚举_目标投影维度::学习: return 枚举_目标权重类型::学习权重;
    case 枚举_目标投影维度::方法维护: return 枚举_目标权重类型::方法维护权重;
    case 枚举_目标投影维度::自维护: return 枚举_目标权重类型::自维护权重;
    case 枚举_目标投影维度::其他: return 枚举_目标权重类型::其他;
    default: return 枚举_目标权重类型::未定义;
    }
}

static std::uint32_t 私有_按特征语义推断结算贡献层级(
    枚举_目标投影维度 维度,
    const 特征节点类* 特征)
{
    if (维度 == 枚举_目标投影维度::安全 || 维度 == 枚举_目标投影维度::服务) {
        if (私有_目标特征是安全账务值(特征) || 私有_目标特征是服务账务值(特征)) return 0;
        if (私有_目标特征是直接安全目标(特征)) return 1;
        if (私有_目标特征是任务层状态(特征)) return 2;
        if (私有_目标特征是方法层状态(特征) || 私有_目标特征是学习层状态(特征)) return 3;
    }
    if (维度 == 枚举_目标投影维度::学习
        || 维度 == 枚举_目标投影维度::方法维护) {
        return 1;
    }
    return 0;
}

static bool 私有_来源需求有安全方向(const 需求节点类* 需求) noexcept
{
    return 需求 && 需求->主信息.安全权重 > 0;
}

static bool 私有_来源需求有服务方向(const 需求节点类* 需求) noexcept
{
    return 需求 && 需求->主信息.服务权重 > 0;
}

static 结构_目标投影路径 私有_构造规则投影路径(
    基础信息节点类* 目标宿主,
    特征节点类* 目标特征,
    枚举_目标投影维度 维度,
    枚举_目标权重类型 权重类型,
    std::uint32_t 结算贡献层级)
{
    结构_目标投影路径 路径{};
    路径.目标维度 = 维度;
    路径.权重类型 = 权重类型;
    路径.目标宿主 = 目标宿主;
    路径.目标特征类型 = 目标特征;
    路径.结算贡献层级 = 结算贡献层级;
    路径.因果距离层级 = 0;
    路径.投影显示层级 = 1;
    路径.是否仅为候选 = true;
    路径.是否可结算 = false;

    if (!目标宿主) 路径.缺失证据.push_back("缺目标宿主");
    if (!目标特征) 路径.缺失证据.push_back("缺目标特征类型");
    if (结算贡献层级 == 0) 路径.缺失证据.push_back("缺结算贡献层级判定规则");
    路径.缺失证据.push_back("缺D0事实状态转换证据");
    路径.缺失证据.push_back("缺D1需求满足证据");
    路径.缺失证据.push_back("缺actual实际达成证据");
    return 路径;
}

static std::vector<结构_目标投影路径> 私有_按目标特征生成规则投影路径(
    基础信息节点类* 目标宿主,
    特征节点类* 目标特征,
    const 需求节点类* 来源需求)
{
    std::vector<结构_目标投影路径> out;
    auto 追加 = [&](枚举_目标投影维度 维度, 枚举_目标权重类型 权重类型, std::uint32_t 层级) {
        out.push_back(私有_构造规则投影路径(目标宿主, 目标特征, 维度, 权重类型, 层级));
    };

    if (!目标特征) return out;

    if (私有_目标特征是安全账务值(目标特征)) {
        auto 路径 = 私有_构造规则投影路径(
            目标宿主,
            目标特征,
            枚举_目标投影维度::安全,
            枚举_目标权重类型::安全权重,
            0);
        路径.缺失证据.push_back("目标特征为安全账务值，需先证明安全目标特征实际变化");
        out.push_back(std::move(路径));
        return out;
    }
    if (私有_目标特征是服务账务值(目标特征)) {
        auto 路径 = 私有_构造规则投影路径(
            目标宿主,
            目标特征,
            枚举_目标投影维度::服务,
            枚举_目标权重类型::服务权重,
            0);
        路径.缺失证据.push_back("目标特征为服务账务值，需先证明服务目标特征实际变化");
        out.push_back(std::move(路径));
        return out;
    }

    if (私有_目标特征是直接安全目标(目标特征)) {
        追加(枚举_目标投影维度::安全, 枚举_目标权重类型::安全权重, 1);
        return out;
    }

    if (私有_目标特征是任务层状态(目标特征)) {
        if (私有_来源需求有安全方向(来源需求)) {
            追加(枚举_目标投影维度::安全, 枚举_目标权重类型::安全权重, 2);
        }
        if (私有_来源需求有服务方向(来源需求)) {
            追加(枚举_目标投影维度::服务, 枚举_目标权重类型::服务权重, 2);
        }
        if (out.empty()) {
            追加(枚举_目标投影维度::其他, 枚举_目标权重类型::其他, 0);
        }
        return out;
    }

    if (私有_目标特征是方法层状态(目标特征) || 私有_目标特征是学习层状态(目标特征)) {
        if (私有_来源需求有安全方向(来源需求)) {
            追加(枚举_目标投影维度::安全, 枚举_目标权重类型::安全权重, 3);
        }
        if (私有_来源需求有服务方向(来源需求)) {
            追加(枚举_目标投影维度::服务, 枚举_目标权重类型::服务权重, 3);
        }
        追加(
            私有_目标特征是学习层状态(目标特征) ? 枚举_目标投影维度::学习 : 枚举_目标投影维度::方法维护,
            私有_目标特征是学习层状态(目标特征) ? 枚举_目标权重类型::学习权重 : 枚举_目标权重类型::方法维护权重,
            1);
        return out;
    }

    const auto 维度 = 私有_按特征语义推断目标维度(目标特征);
    追加(维度, 私有_目标维度到权重类型(维度), 私有_按特征语义推断结算贡献层级(维度, 目标特征));
    return out;
}

static 结构_目标投影路径 私有_从状态转换边生成目标投影路径(
    const 结构_状态转换因果边& 边)
{
    结构_目标投影路径 路径{};
    auto* 结果信息 = 边.结果状态
        ? dynamic_cast<状态节点主信息类*>(边.结果状态->主信息)
        : nullptr;
    if (!结果信息) {
        路径.缺失证据.push_back("缺结果状态主信息");
        return 路径;
    }

    路径.目标宿主 = 结果信息->状态主体.指针;
    路径.目标特征类型 = 结果信息->状态特征.指针;
    路径.目标维度 = 私有_按特征语义推断目标维度(路径.目标特征类型);
    路径.权重类型 = 私有_目标维度到权重类型(路径.目标维度);
    路径.证据路径.push_back(边);
    路径.因果距离层级 = 1;
    路径.投影显示层级 = 1;
    路径.结算贡献层级 = 私有_按特征语义推断结算贡献层级(路径.目标维度, 路径.目标特征类型);
    路径.是否仅为候选 = true;
    路径.是否可结算 = false;

    if (!路径.目标特征类型) {
        路径.缺失证据.push_back("缺目标特征类型");
    }
    if (路径.结算贡献层级 == 0) {
        路径.缺失证据.push_back("缺结算贡献层级判定规则");
    }
    if (边.边类型 == 枚举_状态转换因果边类型::引用关系
        || 边.边类型 == 枚举_状态转换因果边类型::未定义) {
        路径.缺失证据.push_back("缺明确因果边类型");
    }
    return 路径;
}

} // namespace

因果类::因果类(基础信息类* 基础信息) noexcept
    : 基础信息_(基础信息)
{
}

void 因果类::绑定基础信息(基础信息类* 基础信息) noexcept
{
    基础信息_ = 基础信息;
}

因果主信息类* 因果类::取因果主信息(因果节点类* 节点) const noexcept
{
    return 节点 ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
}

const 因果主信息类* 因果类::取因果主信息(const 因果节点类* 节点) const noexcept
{
    return 节点 ? dynamic_cast<const 因果主信息类*>(节点->主信息) : nullptr;
}

因果实例主信息类* 因果类::取实例主信息(因果实例节点类* 节点) const noexcept
{
    auto* m = 取因果主信息(reinterpret_cast<因果节点类*>(节点));
    return (m && m->形态 == 枚举_因果形态::实例) ? m : nullptr;
}

const 因果实例主信息类* 因果类::取实例主信息(const 因果实例节点类* 节点) const noexcept
{
    auto* m = 取因果主信息(reinterpret_cast<const 因果节点类*>(节点));
    return (m && m->形态 == 枚举_因果形态::实例) ? m : nullptr;
}

因果模板主信息类* 因果类::取模板主信息(因果模板节点类* 节点) const noexcept
{
    auto* m = 取因果主信息(reinterpret_cast<因果节点类*>(节点));
    return (m && m->形态 == 枚举_因果形态::抽象) ? m : nullptr;
}

const 因果模板主信息类* 因果类::取模板主信息(const 因果模板节点类* 节点) const noexcept
{
    auto* m = 取因果主信息(reinterpret_cast<const 因果节点类*>(节点));
    return (m && m->形态 == 枚举_因果形态::抽象) ? m : nullptr;
}

因果模板节点类* 因果类::创建因果模板(基础信息节点类* 父节点, 因果主信息类* 主信息)
{
    auto* base = 基础信息_ ? 基础信息_ : &获取基础信息集();
    if (!base || !主信息) return nullptr;
    if (主信息->形态 == 枚举_因果形态::未定义) 主信息->形态 = 枚举_因果形态::抽象;
    auto* parent = 父节点 ? 父节点 : base->世界根();
    return reinterpret_cast<因果模板节点类*>(base->添加子节点(parent, 主信息));
}

因果实例节点类* 因果类::创建因果实例(基础信息节点类* 父节点, 因果主信息类* 主信息)
{
    auto* base = 基础信息_ ? 基础信息_ : &获取基础信息集();
    if (!base || !主信息) return nullptr;
    if (主信息->形态 == 枚举_因果形态::未定义) 主信息->形态 = 枚举_因果形态::实例;
    auto* parent = 父节点 ? 父节点 : base->世界根();
    return reinterpret_cast<因果实例节点类*>(base->添加子节点(parent, 主信息));
}

std::string 因果类::计算动态状态变化签名(const 动态节点类* 动态节点) const
{
    auto* d = 动态节点 ? dynamic_cast<const 动态节点主信息类*>(动态节点->主信息) : nullptr;
    if (!d) return {};

    std::string out = "STATE";
    私有_追加片段(out, 私有_引用键(d->动态主体));
    私有_追加片段(out, 私有_引用键(d->动态特征));
    私有_追加片段(out, 私有_引用键(d->初始状态));
    私有_追加片段(out, 私有_引用键(d->结果状态));
    if (d->动态路径签名 != 0) 私有_追加片段(out, std::to_string(d->动态路径签名));
    return out;
}

std::string 因果类::计算动态动作致变签名(
    const 动态节点类* 动态节点,
    基础信息节点类* 动作主体,
    基础信息节点类* 动作语义,
    const std::string& 因动作语义键) const
{
    auto* d = 动态节点 ? dynamic_cast<const 动态节点主信息类*>(动态节点->主信息) : nullptr;
    if (!d) return {};

    std::string out = "ACTION";
    私有_追加片段(out, 计算动态状态变化签名(动态节点));
    私有_追加片段(out, 因动作语义键.empty() ? std::string{} :因动作语义键);
    私有_追加片段(out, 私有_节点键(动作主体));
    私有_追加片段(out, 私有_节点键(动作语义));
    私有_追加片段(out, 私有_引用键(d->来源动作));
    私有_追加片段(out, 私有_动作名键(d->来源动作名));
    私有_追加片段(out, std::to_string(static_cast<int>(d->来源动作相位)));
    return out;
}

std::string 因果类::计算模板状态变化签名(const 因果模板节点类* 节点) const
{
    auto* m = 取模板主信息(节点);
    if (!m) return {};

    // 优先使用证据动态反推签名；这样抽象主信息不需要把实例状态嵌入自身。
    if (!m->证据动态样本.empty()) {
        auto* d = m->证据动态样本.front().指针;
        if (d) return 计算动态状态变化签名(d);
    }

    std::string out = "STATE_TEMPLATE";
    for (const auto& r : m->果比较模板) 私有_追加片段(out, 私有_引用键(r));
    for (const auto& r : m->条件比较模板) 私有_追加片段(out, 私有_引用键(r));
    return out;
}

std::string 因果类::计算模板动作致变签名(const 因果模板节点类* 节点) const
{
    auto* m = 取模板主信息(节点);
    if (!m) return {};

    if (!m->证据动态样本.empty()) {
        auto* d = m->证据动态样本.front().指针;
        if (d) return 计算动态动作致变签名(d, nullptr, nullptr, m->因动作语义键);
    }

    std::string out = "ACTION_TEMPLATE";
    私有_追加片段(out, 计算模板状态变化签名(节点));
    私有_追加片段(out, 私有_引用键(m->因方法模板));
    私有_追加片段(out, m->因动作语义键);
    私有_追加片段(out, m->因动作名称);
    return out;
}

std::vector<因果模板节点类*> 因果类::查找状态变化关系模板(const std::string& 状态变化签名) const
{
    std::vector<因果模板节点类*> out;
    if (状态变化签名.empty()) return out;
    for (auto* n : 枚举因果模板()) {
        auto* m = 取模板主信息(n);
        if (!m) continue;
        if (m->因方法模板.有效() || !m->因动作语义键.empty() || !m->因动作名称.empty()) continue;
        if (计算模板状态变化签名(n) == 状态变化签名) out.push_back(n);
    }
    return out;
}

std::vector<因果模板节点类*> 因果类::查找动作致变关系模板(const std::string& 动作致变签名) const
{
    std::vector<因果模板节点类*> out;
    if (动作致变签名.empty()) return out;
    for (auto* n : 枚举因果模板()) {
        auto* m = 取模板主信息(n);
        if (!m) continue;
        if (!(m->因方法模板.有效() || !m->因动作语义键.empty() || !m->因动作名称.empty())) continue;
        if (计算模板动作致变签名(n) == 动作致变签名) out.push_back(n);
    }
    return out;
}

std::vector<因果模板节点类*> 因果类::查找导致状态变化的动作关系(const std::string& 状态变化签名) const
{
    std::vector<因果模板节点类*> out;
    if (状态变化签名.empty()) return out;
    for (auto* n : 枚举因果模板()) {
        auto* m = 取模板主信息(n);
        if (!m) continue;
        if (!(m->因方法模板.有效() || !m->因动作语义键.empty() || !m->因动作名称.empty())) continue;
        if (计算模板状态变化签名(n) == 状态变化签名) out.push_back(n);
    }
    return out;
}

结构_自我动作组合反推结果 因果类::反推自我动作组合(
    const 结构_自我动作组合反推输入& 输入) const
{
    结构_自我动作组合反推结果 out{};
    if (!输入.目标结果状态) {
        out.状态 = 枚举_自我动作组合反推状态::无目标结果;
        return out;
    }

    const std::uint32_t 最大候选数 = 输入.最大候选数 == 0 ? 16 : 输入.最大候选数;
    for (auto* 模板节点 : 枚举因果模板()) {
        auto* 模板 = 取模板主信息(模板节点);
        if (!模板 || !私有_因果模板有动作(模板)) continue;
        if (输入.只接受已验证因果 && !模板->已验证) continue;
        if (模板->计算稳定度() < 输入.最低稳定度) continue;
        if (!私有_因果模板结果命中状态(模板, 输入.目标结果状态)) continue;

        auto 映射方法 = 私有_查找可映射动作方法(输入.方法根节点, 模板);
        if (映射方法.empty()) {
            结构_自我动作组合候选 候选{};
            候选.来源因果模板 = 模板节点;
            候选.动作语义键 = 模板->因动作语义键;
            候选.稳定度 = 模板->计算稳定度();
            out.候选.push_back(std::move(候选));
        } else {
            for (auto* 方法 : 映射方法) {
                结构_自我动作组合候选 候选{};
                候选.来源因果模板 = 模板节点;
                候选.候选方法 = 方法;
                候选.动作语义键 = 模板->因动作语义键;
                候选.本能方法ID = 私有_方法本能ID(方法);
                候选.稳定度 = 模板->计算稳定度();
                out.候选.push_back(std::move(候选));
                if (out.候选.size() >= 最大候选数) break;
            }
        }

        if (out.候选.size() >= 最大候选数) break;
    }

    std::sort(out.候选.begin(), out.候选.end(), [](const auto& a, const auto& b) {
        if (a.可直接绑定单步本能() != b.可直接绑定单步本能()) {
            return a.可直接绑定单步本能();
        }
        if ((a.候选方法 != nullptr) != (b.候选方法 != nullptr)) {
            return a.候选方法 != nullptr;
        }
        if (a.稳定度 != b.稳定度) {
            return a.稳定度 > b.稳定度;
        }
        return a.动作语义键 < b.动作语义键;
    });

    if (out.候选.empty()) {
        out.状态 = 枚举_自我动作组合反推状态::无匹配因果;
        return out;
    }

    std::size_t 直接本能数 = 0;
    std::size_t 已映射方法数 = 0;
    for (const auto& 候选 : out.候选) {
        if (候选.候选方法) ++已映射方法数;
        if (候选.可直接绑定单步本能()) ++直接本能数;
    }

    if (直接本能数 == 1) {
        out.状态 = 枚举_自我动作组合反推状态::单步本能方法;
    } else if (直接本能数 > 1) {
        out.状态 = 枚举_自我动作组合反推状态::多候选方法;
    } else if (已映射方法数 > 0) {
        out.状态 = 枚举_自我动作组合反推状态::动作序列待定义;
    } else {
        out.状态 = 枚举_自我动作组合反推状态::有因果但无可映射方法;
    }
    return out;
}

结构_状态转换因果投影 因果类::生成状态转换因果投影(
    const 结构_状态转换因果投影输入& 输入) const
{
    结构_状态转换因果投影 out{};
    auto* base = 基础信息_ ? 基础信息_ : &获取基础信息集();
    if (!base) {
        out.缺失证据.push_back("缺基础信息集");
        return out;
    }

    std::unordered_set<std::string> 已加入节点{};
    std::unordered_set<std::string> 已加入边{};

    auto 追加节点 = [&](状态节点类* 状态, 动态节点类* 动态, 因果模板节点类* 模板, 因果实例节点类* 实例) {
        auto 节点 = 私有_构造状态转换因果节点(状态, 动态, 模板, 实例);
        if (节点.节点签名.empty()) return;
        if (已加入节点.insert(节点.节点签名).second) {
            out.节点.push_back(std::move(节点));
        }
    };

    auto 追加边 = [&](结构_状态转换因果边 边) {
        if (!边.原因状态 || !边.结果状态) return;
        if (边.边签名.empty()) return;
        if (!已加入边.insert(边.边签名).second) return;

        追加节点(边.原因状态, 边.动作动态, 边.因果模板, 边.因果实例);
        追加节点(边.结果状态, 边.动作动态, 边.因果模板, 边.因果实例);
        auto 路径 = 私有_从状态转换边生成目标投影路径(边);
        out.目标投影路径.push_back(std::move(路径));
        out.边.push_back(std::move(边));
    };

    for (auto* n0 : base->枚举节点_按类型<动态节点主信息类>()) {
        auto* 动态 = reinterpret_cast<动态节点类*>(n0);
        auto* 动态信息 = dynamic_cast<动态节点主信息类*>(动态 ? 动态->主信息 : nullptr);
        if (!动态信息 || !动态信息->初始状态.指针 || !动态信息->结果状态.指针) continue;
        追加边(私有_构造动态状态转换边(动态));
    }

    for (auto* 模板节点 : 枚举因果模板()) {
        auto* 模板信息 = 取模板主信息(模板节点);
        if (!模板信息) continue;
        if (!输入.包含未验证路径 && !模板信息->已验证) continue;

        for (const auto& 证据 : 模板信息->证据动态样本) {
            if (!证据.指针) continue;
            auto 边 = 私有_构造动态状态转换边(证据.指针);
            if (!边.原因状态 || !边.结果状态) continue;
            边.因果模板 = 模板节点;
            边.置信度 = 私有_派生置信分(模板信息);
            边.稳定度 = 模板信息->计算稳定度();
            if (!边.边签名.empty()) {
                边.边签名 += "|T:";
                边.边签名 += 私有_节点键(模板节点);
            }
            追加边(std::move(边));
        }

        for (const auto& 迁移引用 : 模板信息->果比较模板) {
            auto* 迁移 = 迁移引用.指针;
            auto* 迁移信息 = 迁移 ? dynamic_cast<二次特征主信息类*>(迁移->主信息) : nullptr;
            if (!迁移信息
                || !迁移信息->来源起始状态.指针
                || !迁移信息->来源结果状态.指针) {
                continue;
            }

            结构_状态转换因果边 边{};
            边.原因状态 = 迁移信息->来源起始状态.指针;
            边.结果状态 = 迁移信息->来源结果状态.指针;
            边.动作动态 = 迁移信息->来源动态.指针;
            边.因果模板 = 模板节点;
            边.边类型 = 边.动作动态
                ? 私有_推断边类型(dynamic_cast<动态节点主信息类*>(边.动作动态->主信息))
                : 枚举_状态转换因果边类型::引用关系;
            边.置信度 = 私有_派生置信分(模板信息);
            边.稳定度 = 模板信息->计算稳定度();

            std::string 签名 = "TEMPLATE_EDGE";
            私有_追加片段(签名, 私有_状态签名(边.原因状态));
            私有_追加片段(签名, 私有_节点键(迁移));
            私有_追加片段(签名, 私有_状态签名(边.结果状态));
            私有_追加片段(签名, 私有_节点键(模板节点));
            边.边签名 = 签名;
            追加边(std::move(边));
        }
    }

    if (out.边.empty()) {
        out.缺失证据.push_back("未找到可投影状态转换边");
    }
    (void)输入.最大深度;
    return out;
}

结构_叶子任务因果投影预判 因果类::查询叶子任务权重类型与层级(
    const 结构_叶子任务因果投影输入& 输入) const
{
    结构_叶子任务因果投影预判 out{};
    out.叶子任务 = 输入.叶子任务;
    out.来源需求 = 输入.来源需求;
    out.目标宿主 = 输入.目标宿主;
    out.目标特征类型 = 输入.目标特征类型;

    if (!输入.叶子任务) {
        out.缺失证据.push_back("缺叶子任务");
    }
    if (!输入.来源需求) {
        out.缺失证据.push_back("缺来源需求");
    }
    if (!输入.目标宿主) {
        out.缺失证据.push_back("缺目标宿主");
    }
    if (!输入.目标特征类型) {
        out.缺失证据.push_back("缺目标特征类型");
    }

    out.是否允许形成D0 = 输入.目标宿主 != nullptr && 输入.目标特征类型 != nullptr;

    结构_状态转换因果投影输入 投影输入{};
    投影输入.最大深度 = 输入.最大深度;
    投影输入.包含未验证路径 = 输入.允许未验证路径;
    auto 投影 = 生成状态转换因果投影(投影输入);

    auto 追加权重类型 = [&](枚举_目标权重类型 权重类型) {
        if (权重类型 == 枚举_目标权重类型::未定义) return;
        if (std::find(out.候选权重类型集.begin(), out.候选权重类型集.end(), 权重类型)
            == out.候选权重类型集.end()) {
            out.候选权重类型集.push_back(权重类型);
        }
    };

    for (auto 路径 : 投影.目标投影路径) {
        if (输入.目标特征类型 && 路径.目标特征类型 != 输入.目标特征类型) {
            continue;
        }
        if (输入.目标宿主 && 路径.目标宿主 && 路径.目标宿主 != 输入.目标宿主) {
            continue;
        }
        追加权重类型(路径.权重类型);
        out.候选投影路径.push_back(std::move(路径));
    }

    if (out.候选投影路径.empty()) {
        // 因果本体里尚未沉淀到对应 D0 证据时，仍按目标特征语义生成
        // 只读规则候选路径。候选路径只说明“该特征可投影到哪类权重、
        // 哪个结算贡献层级”，不允许直接结算。
        auto 规则路径 = 私有_按目标特征生成规则投影路径(
            输入.目标宿主,
            输入.目标特征类型,
            输入.来源需求);
        for (auto& 路径 : 规则路径) {
            追加权重类型(路径.权重类型);
            out.候选投影路径.push_back(std::move(路径));
        }
    }

    if (out.候选投影路径.empty()) {
        out.缺失证据.push_back("缺目标投影路径");
        for (const auto& 缺失 : 投影.缺失证据) {
            out.缺失证据.push_back(缺失);
        }
    }

    for (const auto& 路径 : out.候选投影路径) {
        if (路径.是否可结算 && 路径.结算贡献层级 != 0 && 路径.缺失证据.empty()) {
            out.是否允许进入结算候选 = true;
            break;
        }
    }

    if (!out.是否允许进入结算候选 && !out.候选投影路径.empty()) {
        auto 追加缺失证据 = [&](const std::string& 证据) {
            if (证据.empty()) return;
            if (std::find(out.缺失证据.begin(), out.缺失证据.end(), 证据)
                == out.缺失证据.end()) {
                out.缺失证据.push_back(证据);
            }
        };
        for (const auto& 路径 : out.候选投影路径) {
            for (const auto& 证据 : 路径.缺失证据) {
                追加缺失证据(证据);
            }
        }
        out.缺失证据.push_back("投影路径仍缺结算闸门证据");
    }

    (void)输入.候选方法;
    return out;
}

static std::vector<二次特征节点类*> 私有_查找动态果状态迁移模板(基础信息类* base, 动态节点类* d)
{
    std::vector<二次特征节点类*> out;
    if (!base || !d) return out;
    auto* dm = dynamic_cast<动态节点主信息类*>(d->主信息);
    if (!dm) return out;

    for (auto* n0 : base->枚举节点_按类型<二次特征主信息类>()) {
        auto* n = reinterpret_cast<二次特征节点类*>(n0);
        auto* m = dynamic_cast<二次特征主信息类*>(n->主信息);
        if (!m) continue;
        if (m->形态 != 二次特征主信息类::枚举_形态::状态迁移) continue;

        bool hit = false;
        if (私有_引用等于节点(m->来源动态, d)) hit = true;
        if (!hit && dm->初始状态.指针 && dm->结果状态.指针) {
            hit = 私有_引用等于节点(m->来源起始状态, dm->初始状态.指针)
                && 私有_引用等于节点(m->来源结果状态, dm->结果状态.指针);
        }
        if (hit) out.push_back(n);
    }
    return out;
}

结构_两层因果构建结果 因果类::按动态补全两层因果(
    场景节点类* 场景,
    动态节点类* 动态节点,
    枚举_因果来源类型 来源类型,
    基础信息节点类* 动作主体,
    基础信息节点类* 动作语义,
    const std::string& 因动作语义键,
    bool 设为主结果)
{
    结构_两层因果构建结果 result{};
    auto* base = 基础信息_ ? 基础信息_ : &获取基础信息集();
    auto* dm = 动态节点 ? dynamic_cast<动态节点主信息类*>(动态节点->主信息) : nullptr;
    if (!base || !dm || !动态节点) return result;

    result.状态变化签名 = 计算动态状态变化签名(动态节点);
    result.动作致变签名 = 计算动态动作致变签名(动态节点, 动作主体, 动作语义, 因动作语义键);

    auto 状态候选 = 查找状态变化关系模板(result.状态变化签名);
    if (!状态候选.empty()) {
        result.状态变化关系 = 状态候选.front();
    } else {
        auto* info = new 因果主信息类(枚举_因果形态::抽象);
        info->适用锚点类型 = 场景 ? 枚举_因果锚点类型::场景 : 枚举_因果锚点类型::未定义;
        info->适用层级 = dm->动态层级;
        info->来源类型 = 来源类型;
        for (auto* t : 私有_查找动态果状态迁移模板(base, 动态节点)) {
            info->果比较模板.push_back(私有_制引用(t));
            if (!info->主果比较模板.有效()) info->主果比较模板 = 私有_制引用(t);
        }
        result.状态变化关系 = 创建因果模板(reinterpret_cast<基础信息节点类*>(场景), info);
        result.创建了状态变化关系 = result.状态变化关系 != nullptr;
    }
    if (result.状态变化关系) {
        追加证据动态样本(result.状态变化关系, 动态节点);
    }

    auto* instInfo = new 因果主信息类(枚举_因果形态::实例);
    instInfo->适用锚点类型 = 场景 ? 枚举_因果锚点类型::场景 : 枚举_因果锚点类型::未定义;
    instInfo->适用层级 = dm->动态层级;
    instInfo->来源类型 = 来源类型;
    instInfo->证据动态样本.push_back(私有_制引用(动态节点));
    instInfo->观察次数 = 1;
    instInfo->果出现次数 = dm->结果状态.有效() ? 1 : 0;
    if (dm->来源动作.有效()) instInfo->因方法模板 = dm->来源动作;
    if (!因动作语义键.empty()) instInfo->因动作语义键 = 因动作语义键;
    else if (动作语义 || 动作主体) instInfo->因动作语义键 = 私有_节点键(动作语义 ? 动作语义 : 动作主体);
    if (dm->来源动作名) instInfo->因动作名称 = 私有_动作名键(dm->来源动作名);
    if (dm->来源动作.有效() || !instInfo->因动作语义键.empty() || !instInfo->因动作名称.empty()) {
        instInfo->因出现次数 = 1;
        if (instInfo->果出现次数 != 0) instInfo->成立次数 = 1;
    }
    result.证据实例 = 创建因果实例(reinterpret_cast<基础信息节点类*>(场景), instInfo);
    result.创建了证据实例 = result.证据实例 != nullptr;

    const bool 有动作 = 动作主体 || 动作语义 || !因动作语义键.empty() || dm->来源动作.有效() || dm->来源动作名 != nullptr;
    if (有动作) {
        auto 动作候选 = 查找动作致变关系模板(result.动作致变签名);
        if (!动作候选.empty()) {
            result.动作致变关系 = 动作候选.front();
        } else {
            auto* info = new 因果主信息类(枚举_因果形态::抽象);
            info->适用锚点类型 = 场景 ? 枚举_因果锚点类型::场景 : 枚举_因果锚点类型::未定义;
            info->适用层级 = dm->动态层级 + 1;
            info->来源类型 = 来源类型;
            if (dm->来源动作.有效()) info->因方法模板 = dm->来源动作;
            if (!因动作语义键.empty()) info->因动作语义键 = 因动作语义键;
            else if (动作语义 || 动作主体) info->因动作语义键 = 私有_节点键(动作语义 ? 动作语义 : 动作主体);
            if (dm->来源动作名) info->因动作名称 = 私有_动作名键(dm->来源动作名);
            for (auto* t : 私有_查找动态果状态迁移模板(base, 动态节点)) {
                info->果比较模板.push_back(私有_制引用(t));
                if (!info->主果比较模板.有效()) info->主果比较模板 = 私有_制引用(t);
            }
            result.动作致变关系 = 创建因果模板(reinterpret_cast<基础信息节点类*>(场景), info);
            result.创建了动作致变关系 = result.动作致变关系 != nullptr;
        }
        if (result.动作致变关系) {
            追加证据动态样本(result.动作致变关系, 动态节点);
            if (result.证据实例) 追加证据实例(result.动作致变关系, result.证据实例);
        }
    }

    if (result.状态变化关系 && result.证据实例) {
        追加证据实例(result.状态变化关系, result.证据实例);
    }

    if (设为主结果 && result.状态变化关系) {
        按动态桥接结果状态(reinterpret_cast<因果节点类*>(result.状态变化关系), 动态节点, true);
    }
    if (设为主结果 && result.动作致变关系) {
        按动态桥接结果状态(reinterpret_cast<因果节点类*>(result.动作致变关系), 动态节点, true);
    }

    return result;
}

因果模板节点类* 因果类::按动态创建因果信息(
    场景节点类* 场景,
    动态节点类* 动态节点,
    枚举_因果来源类型 来源类型,
    基础信息节点类* 动作主体,
    基础信息节点类* 动作语义,
    const std::string& 因动作语义键,
    bool 设为主结果)
{
    auto r = 按动态补全两层因果(场景, 动态节点, 来源类型, 动作主体, 动作语义, 因动作语义键, 设为主结果);
    return r.动作致变关系 ? r.动作致变关系 : r.状态变化关系;
}

std::vector<结构_两层因果构建结果> 因果类::按动态集合补全两层因果(
    场景节点类* 场景,
    const std::vector<动态节点类*>& 动态集合,
    枚举_因果来源类型 来源类型)
{
    std::vector<结构_两层因果构建结果> out;
    out.reserve(动态集合.size());
    for (auto* d : 动态集合) {
        auto r = 按动态补全两层因果(场景, d, 来源类型);
        if (r.成功()) out.push_back(r);
    }
    return out;
}

bool 因果类::删除因果(因果节点类* 节点)
{
    auto* base = 基础信息_ ? 基础信息_ : &获取基础信息集();
    return base && 节点 ? base->删除节点(reinterpret_cast<基础信息节点类*>(节点)) : false;
}

std::vector<因果节点类*> 因果类::枚举全部因果() const
{
    std::vector<因果节点类*> out;
    auto* base = 基础信息_ ? 基础信息_ : const_cast<基础信息类*>(&获取基础信息集());
    if (!base) return out;
    for (auto* n : base->枚举节点_按类型<因果主信息类>()) {
        out.push_back(reinterpret_cast<因果节点类*>(n));
    }
    return out;
}

std::vector<因果实例节点类*> 因果类::枚举因果实例() const
{
    std::vector<因果实例节点类*> out;
    for (auto* n : 枚举全部因果()) {
        auto* m = 取因果主信息(n);
        if (m && m->形态 == 枚举_因果形态::实例) out.push_back(reinterpret_cast<因果实例节点类*>(n));
    }
    return out;
}

std::vector<因果模板节点类*> 因果类::枚举因果模板() const
{
    std::vector<因果模板节点类*> out;
    for (auto* n : 枚举全部因果()) {
        auto* m = 取因果主信息(n);
        if (m && m->形态 == 枚举_因果形态::抽象) out.push_back(reinterpret_cast<因果模板节点类*>(n));
    }
    return out;
}

bool 因果类::按动态桥接结果状态(因果节点类* 节点, 动态节点类* 动态节点, bool 设为主结果)
{
    auto* m = 取因果主信息(节点);
    auto* d = 动态节点 ? dynamic_cast<动态节点主信息类*>(动态节点->主信息) : nullptr;
    if (!m || !d || !d->结果状态.有效()) return false;

    // 当前云端因果主信息以“果比较模板 + 证据动态”为主；结果状态仍通过证据动态保留。
    if (!私有_向量含动态(m->证据动态样本, 动态节点)) {
        m->证据动态样本.push_back(私有_制引用(动态节点));
    }
    if (设为主结果) {
        for (auto* t : 私有_查找动态果状态迁移模板(基础信息_ ? 基础信息_ : &获取基础信息集(), 动态节点)) {
            if (!m->主果比较模板.有效()) m->主果比较模板 = 私有_制引用(t);
            if (!私有_向量含二次特征(m->果比较模板, t)) m->果比较模板.push_back(私有_制引用(t));
        }
    }
    return true;
}

bool 因果类::追加条件模板(因果节点类* 节点, 特征节点类* 条件模板)
{
    auto* m = 取因果主信息(节点);
    if (!m || !条件模板) return false;
    if (!私有_向量含特征(m->条件比较模板, 条件模板)) m->条件比较模板.push_back(私有_制引用(条件模板));
    return true;
}

bool 因果类::追加状态迁移模板(因果节点类* 节点, 二次特征节点类* 果状态迁移模板, bool 设为主迁移)
{
    auto* m = 取因果主信息(节点);
    if (!m || !果状态迁移模板) return false;
    if (!私有_向量含二次特征(m->果比较模板, 果状态迁移模板)) m->果比较模板.push_back(私有_制引用(果状态迁移模板));
    if (设为主迁移 || !m->主果比较模板.有效()) m->主果比较模板 = 私有_制引用(果状态迁移模板);
    return true;
}

bool 因果类::追加结果状态(因果节点类* 节点, 状态节点类* 结果状态, bool 设为主结果)
{
    auto* m = 取因果主信息(节点);
    if (!m || !结果状态) return false;

    // 新口径不直接把实例结果状态嵌入抽象因果；尝试寻找以该结果状态为终点的果比较模板。
    auto* base = 基础信息_ ? 基础信息_ : &获取基础信息集();
    bool changed = false;
    if (base) {
        for (auto* n0 : base->枚举节点_按类型<二次特征主信息类>()) {
            auto* n = reinterpret_cast<二次特征节点类*>(n0);
            auto* x = dynamic_cast<二次特征主信息类*>(n->主信息);
            if (!x) continue;
            if (x->形态 != 二次特征主信息类::枚举_形态::状态迁移) continue;
            if (!私有_引用等于节点(x->来源结果状态, 结果状态)) continue;
            追加状态迁移模板(节点, n, 设为主结果);
            changed = true;
        }
    }
    return changed;
}

bool 因果类::绑定动作主体语义(
    因果节点类* 节点,
    基础信息节点类* 动作主体,
    基础信息节点类* 动作语义,
    const std::string& 因动作语义键)
{
    auto* m = 取因果主信息(节点);
    if (!m) return false;
    if (!因动作语义键.empty()) {
        m->因动作语义键 = 因动作语义键;
    } else if (动作语义 || 动作主体) {
        m->因动作语义键 = 私有_节点键(动作语义 ? 动作语义 : 动作主体);
    }
    return true;
}

bool 因果类::追加证据动态样本(因果模板节点类* 节点, 动态节点类* 证据动态)
{
    auto* m = 取模板主信息(节点);
    if (!m || !证据动态) return false;
    if (!私有_向量含动态(m->证据动态样本, 证据动态)) {
        m->证据动态样本.push_back(私有_制引用(证据动态));
        ++m->观察次数;
        ++m->因出现次数;
        ++m->果出现次数;
        ++m->成立次数;
        m->最近命中时间 = 0;
    }
    return true;
}

bool 因果类::追加证据实例(因果模板节点类* 节点, 因果实例节点类* 证据实例)
{
    auto* m = 取模板主信息(节点);
    auto* e = 取实例主信息(证据实例);
    if (!m || !e) return false;
    // 当前云端统一因果主信息没有“证据实例”字段，实例证据通过实例持有的动态样本合并到模板。
    bool changed = false;
    for (auto& d : e->证据动态样本) {
        if (d.指针 && !私有_向量含动态(m->证据动态样本, d.指针)) {
            m->证据动态样本.push_back(d);
            changed = true;
        }
    }
    if (changed) {
        ++m->观察次数;
        ++m->因出现次数;
        ++m->果出现次数;
        ++m->成立次数;
    }
    return true;
}

bool 因果类::二次特征匹配模板(const 二次特征节点类* 二次特征, const 二次特征节点类* 模板) const noexcept
{
    auto* x = 二次特征 ? dynamic_cast<const 二次特征主信息类*>(二次特征->主信息) : nullptr;
    auto* t = 模板 ? dynamic_cast<const 二次特征主信息类*>(模板->主信息) : nullptr;
    if (!x || !t) return false;
    return x->模板匹配(*t);
}

bool 因果类::因果模板匹配二次特征(
    const 因果模板节点类* 模板,
    const std::vector<二次特征节点类*>&,
    const std::vector<二次特征节点类*>& 结果候选) const noexcept
{
    auto* m = 取模板主信息(模板);
    if (!m) return false;
    if (m->果比较模板.empty()) return true;

    for (const auto& rt : m->果比较模板) {
        bool oneHit = false;
        for (auto* c : 结果候选) {
            if (rt.指针 && 二次特征匹配模板(c, rt.指针)) {
                oneHit = true;
                break;
            }
        }
        if (!oneHit) return false;
    }
    return true;
}

std::vector<因果模板节点类*> 因果类::查找匹配因果模板(
    枚举_因果锚点类型 锚点类型,
    std::uint32_t 状态层级,
    const std::vector<二次特征节点类*>& 条件候选,
    const std::vector<二次特征节点类*>& 结果候选) const
{
    std::vector<因果模板节点类*> out;
    for (auto* n : 枚举因果模板()) {
        auto* m = 取模板主信息(n);
        if (!m) continue;
        if (锚点类型 != 枚举_因果锚点类型::未定义 && m->适用锚点类型 != 锚点类型) continue;
        if (状态层级 != 0 && m->适用层级 != 状态层级) continue;
        if (因果模板匹配二次特征(n, 条件候选, 结果候选)) out.push_back(n);
    }
    return out;
}

bool 因果类::生成实例因果表达单元(const 因果实例节点类* 节点, 结构_因果表达单元& 输出) const
{
    auto* m = 取实例主信息(节点);
    if (!m) return false;
    输出 = {};
    输出.是抽象因果 = false;
    输出.锚点名称 = 私有_形态文本(m);
    输出.动作短语 = !m->因动作语义键.empty() ? m->因动作语义键 : m->因动作名称;
    输出.成立次数 = m->成立次数;
    输出.失败次数 = m->失败次数;
    for (const auto& d : m->证据动态样本) {
        if (!d.指针) continue;
        auto* dm = dynamic_cast<动态节点主信息类*>(d.指针->主信息);
        if (!dm) continue;
        if (dm->初始状态.指针) 输出.条件短语.push_back("状态:" + 私有_节点键(dm->初始状态.指针));
        if (dm->结果状态.指针) {
            const auto s = "状态:" + 私有_节点键(dm->结果状态.指针);
            输出.结果短语.push_back(s);
            if (输出.主结果短语.empty()) 输出.主结果短语 = s;
        }
    }
    return true;
}

bool 因果类::生成抽象因果表达单元(const 因果模板节点类* 节点, 结构_因果表达单元& 输出) const
{
    auto* m = 取模板主信息(节点);
    if (!m) return false;
    输出 = {};
    输出.是抽象因果 = true;
    输出.锚点名称 = 私有_形态文本(m);
    输出.动作短语 = !m->因动作语义键.empty() ? m->因动作语义键 : m->因动作名称;
    输出.成立次数 = m->成立次数;
    输出.失败次数 = m->失败次数;
    for (const auto& c : m->条件比较模板) 输出.条件短语.push_back("条件比较模板:" + 私有_引用键(c));
    for (const auto& r : m->果比较模板) {
        const auto s = "果比较模板:" + 私有_引用键(r);
        输出.结果短语.push_back(s);
        if (!m->主果比较模板.有效() || 私有_引用键(m->主果比较模板) == 私有_引用键(r)) 输出.主结果短语 = s;
    }
    if (输出.结果短语.empty() && !m->证据动态样本.empty()) {
        auto* d = m->证据动态样本.front().指针;
        auto* dm = d ? dynamic_cast<动态节点主信息类*>(d->主信息) : nullptr;
        if (dm && dm->结果状态.指针) {
            const auto s = "状态:" + 私有_节点键(dm->结果状态.指针);
            输出.结果短语.push_back(s);
            输出.主结果短语 = s;
        }
    }
    return true;
}

std::string 因果类::生成实例因果自然语言(const 因果实例节点类* 节点) const
{
    结构_因果表达单元 e{};
    if (!生成实例因果表达单元(节点, e)) return {};
    std::ostringstream os;
    if (!e.条件短语.empty()) {
        os << "因为";
        for (std::size_t i = 0; i < e.条件短语.size(); ++i) {
            if (i) os << "、";
            os << e.条件短语[i];
        }
        os << "，";
    }
    if (!e.动作短语.empty()) os << "在动作[" << e.动作短语 << "]作用下，";
    os << "所以发生" << (e.主结果短语.empty() ? "状态变化" : e.主结果短语) << "。";
    return os.str();
}

std::string 因果类::生成抽象因果自然语言(const 因果模板节点类* 节点) const
{
    结构_因果表达单元 e{};
    if (!生成抽象因果表达单元(节点, e)) return {};
    std::ostringstream os;
    if (!e.条件短语.empty()) {
        os << "若";
        for (std::size_t i = 0; i < e.条件短语.size(); ++i) {
            if (i) os << "、";
            os << e.条件短语[i];
        }
        os << "，";
    }
    if (!e.动作短语.empty()) os << "动作[" << e.动作短语 << "]会导致";
    else os << "会发生";
    os << (e.主结果短语.empty() ? "状态变化" : e.主结果短语);
    os << "；证据成立" << e.成立次数 << "次，失败" << e.失败次数 << "次。";
    return os.str();
}


