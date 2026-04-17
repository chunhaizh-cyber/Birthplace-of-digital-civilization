module;

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

#include "基础信息主信息类.h"

export module 二次特征匹配模块;

namespace 二次特征匹配模块_detail {
    template<class T节点>
    std::string 引用键(const 可解析引用<T节点>& 引用)
    {
        if (!引用.主键.empty()) return 引用.主键;
        return 引用.指针 ? 引用.指针->获取主键() : std::string{};
    }

    inline bool 区间有效(const std::optional<I64区间>& 区间值) noexcept
    {
        return 区间值.has_value() && 区间值->有效();
    }

    inline bool 词性主键相等(const 词性节点类* 左, const 词性节点类* 右) noexcept
    {
        if (左 == 右) return true;
        if (!左 || !右) return false;
        const auto 左主键 = 左->获取主键();
        const auto 右主键 = 右->获取主键();
        return !左主键.empty() && 左主键 == 右主键;
    }

    inline const 二次特征主信息类* 主信息(const 二次特征节点类* 节点) noexcept
    {
        return 节点 ? dynamic_cast<const 二次特征主信息类*>(节点->主信息) : nullptr;
    }

    inline I64 点值(const 二次特征主信息类& 主信息) noexcept
    {
        switch (主信息.值形态) {
        case 枚举_二次特征值形态::布尔:
            return 主信息.是否满足 ? 1 : 0;
        case 枚举_二次特征值形态::枚举:
        case 枚举_二次特征值形态::签名:
            return static_cast<I64>(主信息.离散编码);
        default:
            return 主信息.标量值;
        }
    }

    inline bool 对象约束匹配(
        const 基础信息节点类* 条件侧,
        const 基础信息节点类* 状态侧,
        bool 条件空视为通配) noexcept
    {
        if (!条件侧) return 条件空视为通配;
        return 条件侧 == 状态侧;
    }

    inline bool 概念模板_弱相等(
        const 可解析引用<二次特征节点类>& 左,
        const 可解析引用<二次特征节点类>& 右) noexcept
    {
        if (引用键(左).empty() || 引用键(右).empty()) return false;
        if (引用键(左) == 引用键(右)) return true;

        const auto* 左主信息 = 主信息(左.指针);
        const auto* 右主信息 = 主信息(右.指针);
        if (!左主信息 || !右主信息) return false;

        if (左主信息->概念名称 && 右主信息->概念名称
            && 词性主键相等(左主信息->概念名称, 右主信息->概念名称)) {
            return true;
        }

        return 左主信息->度量签名_链键 == 右主信息->度量签名_链键
            && 左主信息->区间层级 == 右主信息->区间层级
            && 左主信息->标量区间.has_value() == 右主信息->标量区间.has_value()
            && (!左主信息->标量区间.has_value()
                || (左主信息->标量区间->低值 == 右主信息->标量区间->低值
                    && 左主信息->标量区间->高值 == 右主信息->标量区间->高值));
    }
}

export namespace 二次特征匹配模块 {

struct 结构_匹配配置 {
    bool 使用对象绑定 = true;
    bool 条件空指针视为通配 = true;
    bool 允许用概念名称弱匹配 = true;
};

struct 结构_匹配结果 {
    I64 命中数 = 0;
    I64 摘要数 = 0;
    I64 目标数 = 0;
    I64 精确率_Q10000 = 0;
    I64 召回率_Q10000 = 0;
    I64 得分_Q10000 = 0;
};

export inline bool 二次特征_满足(
    const 二次特征主信息类* 条件,
    const 二次特征主信息类* 状态,
    const 结构_匹配配置& 配置 = {}) noexcept
{
    using namespace 二次特征匹配模块_detail;

    if (!条件 || !状态) return false;
    if (条件->度量签名_链键.empty() || 状态->度量签名_链键.empty()) return false;
    if (条件->度量签名_链键 != 状态->度量签名_链键) return false;

    if (配置.使用对象绑定) {
        if (!对象约束匹配(条件->主体.指针, 状态->主体.指针, 配置.条件空指针视为通配)) return false;
        if (!对象约束匹配(条件->客体.指针, 状态->客体.指针, 配置.条件空指针视为通配)) return false;
        if (!对象约束匹配(条件->左对象.指针, 状态->左对象.指针, 配置.条件空指针视为通配)) return false;
        if (!对象约束匹配(条件->右对象.指针, 状态->右对象.指针, 配置.条件空指针视为通配)) return false;
    }

    if (条件->概念模板.有效()) {
        if (引用键(条件->概念模板) == 引用键(状态->概念模板)) {
            return true;
        }
        if (配置.允许用概念名称弱匹配
            && 概念模板_弱相等(条件->概念模板, 状态->概念模板)) {
            return true;
        }
        return false;
    }

    if (条件->概念名称) {
        if (状态->概念名称 && 词性主键相等(条件->概念名称, 状态->概念名称)) {
            return true;
        }
        if (状态->概念模板.指针) {
            const auto* 概念主信息 = 主信息(状态->概念模板.指针);
            if (概念主信息 && 概念主信息->概念名称
                && 词性主键相等(条件->概念名称, 概念主信息->概念名称)) {
                return true;
            }
        }
        return false;
    }

    if (区间有效(条件->标量区间)) {
        if (区间有效(状态->标量区间)) {
            const I64 低值 = std::max<I64>(条件->标量区间->低值, 状态->标量区间->低值);
            const I64 高值 = std::min<I64>(条件->标量区间->高值, 状态->标量区间->高值);
            return 高值 >= 低值;
        }
        const auto 当前点值 = 点值(*状态);
        return 当前点值 >= 条件->标量区间->低值 && 当前点值 <= 条件->标量区间->高值;
    }

    if (!条件->离散语义键.empty() && 状态->离散语义键 != 条件->离散语义键) return false;
    if (条件->离散编码 != 0 && 状态->离散编码 != 条件->离散编码) return false;

    if (条件->值形态 == 枚举_二次特征值形态::布尔) {
        return 状态->是否满足 == 条件->是否满足;
    }

    return true;
}

export inline bool 条件索引_满足(
    const std::vector<const 二次特征节点类*>& 条件索引,
    const std::vector<const 二次特征节点类*>& 当前判定集,
    I64* 输出满足数 = nullptr,
    const 结构_匹配配置& 配置 = {}) noexcept
{
    using namespace 二次特征匹配模块_detail;

    if (输出满足数) *输出满足数 = 0;
    if (条件索引.empty()) return true;
    if (当前判定集.empty()) return false;

    I64 命中 = 0;
    for (const auto* 条件节点 : 条件索引) {
        const auto* 条件主信息 = 主信息(条件节点);
        if (!条件主信息) continue;

        bool 当前命中 = false;
        for (const auto* 状态节点 : 当前判定集) {
            const auto* 状态主信息 = 主信息(状态节点);
            if (!状态主信息) continue;
            if (二次特征_满足(条件主信息, 状态主信息, 配置)) {
                当前命中 = true;
                break;
            }
        }
        if (当前命中) ++命中;
    }

    if (输出满足数) *输出满足数 = 命中;
    return 命中 >= static_cast<I64>(条件索引.size());
}

export inline 结构_匹配结果 结果摘要_匹配目标(
    const std::vector<const 二次特征节点类*>& 摘要索引,
    const std::vector<const 二次特征节点类*>& 目标判定集,
    const 结构_匹配配置& 配置 = {}) noexcept
{
    using namespace 二次特征匹配模块_detail;

    结构_匹配结果 输出{};
    输出.摘要数 = static_cast<I64>(摘要索引.size());
    输出.目标数 = static_cast<I64>(目标判定集.size());
    if (摘要索引.empty() || 目标判定集.empty()) {
        return 输出;
    }

    std::vector<bool> 已用(目标判定集.size(), false);
    for (const auto* 摘要节点 : 摘要索引) {
        const auto* 摘要主信息 = 主信息(摘要节点);
        if (!摘要主信息) continue;

        for (std::size_t i = 0; i < 目标判定集.size(); ++i) {
            if (已用[i]) continue;
            const auto* 目标主信息 = 主信息(目标判定集[i]);
            if (!目标主信息) continue;
            if (二次特征_满足(摘要主信息, 目标主信息, 配置)) {
                已用[i] = true;
                ++输出.命中数;
                break;
            }
        }
    }

    const auto 计算比率Q10000 = [](I64 分子, I64 分母) noexcept -> I64 {
        if (分母 <= 0 || 分子 <= 0) return 0;
        const auto 值 = static_cast<I64>((static_cast<long long>(分子) * 10000LL) / static_cast<long long>(分母));
        return std::clamp<I64>(值, 0, 10000);
    };

    输出.精确率_Q10000 = 计算比率Q10000(输出.命中数, 输出.摘要数);
    输出.召回率_Q10000 = 计算比率Q10000(输出.命中数, 输出.目标数);
    输出.得分_Q10000 = 计算比率Q10000(2 * 输出.命中数, 输出.摘要数 + 输出.目标数);
    return 输出;
}

} // namespace 二次特征匹配模块
