#include "特征类.h"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <utility>

namespace {
    constexpr I64 私有_默认不同值 = 10000;

    bool 私有_词性相同(const 词性节点类* a, const 词性节点类* b) noexcept
    {
        if (a == b) return true;
        if (!a || !b) return false;
        return a->获取主键() == b->获取主键();
    }

    I64 私有_饱和加(I64 a, I64 b) noexcept
    {
        if (b > 0 && a > I64_MAX - b) return I64_MAX;
        if (b < 0 && a < I64_MIN - b) return I64_MIN;
        return a + b;
    }

    I64 私有_饱和减(I64 a, I64 b) noexcept
    {
        if (b == I64_MIN) return I64_MAX;
        return 私有_饱和加(a, -b);
    }

    I64 私有_绝对值饱和(I64 值) noexcept
    {
        if (值 == I64_MIN) return I64_MAX;
        return 值 < 0 ? -值 : 值;
    }

    I64 私有_点到区间距离(I64 x, const I64区间& 区间) noexcept
    {
        if (!区间.有效()) return 0;
        if (x < 区间.低值) return 区间.低值 - x;
        if (x > 区间.高值) return x - 区间.高值;
        return 0;
    }

    I64 私有_区间外有符号差值(I64 x, const I64区间& 区间) noexcept
    {
        if (!区间.有效()) return x;
        if (x < 区间.低值) return 私有_饱和减(x, 区间.低值);
        if (x > 区间.高值) return 私有_饱和减(x, 区间.高值);
        return 0;
    }

    bool 私有_特征值严格相等(const 特征值& 左值, const 特征值& 右值) noexcept
    {
        if (左值.index() != 右值.index()) return false;

        if (const auto* 左标量 = std::get_if<I64>(&左值)) {
            const auto* 右标量 = std::get_if<I64>(&右值);
            return 右标量 && (*左标量 == *右标量);
        }
        if (const auto* 左句柄 = std::get_if<VecU句柄>(&左值)) {
            const auto* 右句柄 = std::get_if<VecU句柄>(&右值);
            return 右句柄 && (*左句柄 == *右句柄);
        }
        if (const auto* 左指针 = std::get_if<指针句柄>(&左值)) {
            const auto* 右指针 = std::get_if<指针句柄>(&右值);
            return 右指针 && (*左指针 == *右指针);
        }
        return true;
    }
}

特征类::特征类(基础信息类* 基础信息) noexcept
    : 基础信息_(基础信息)
{
}

void 特征类::绑定基础信息(基础信息类* 基础信息) noexcept
{
    基础信息_ = 基础信息;
}

void 特征类::设置差异度函数(差异度函数 函数) noexcept
{
    差异度函数_ = std::move(函数);
}

特征节点主信息类* 特征类::取特征主信息(特征节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<特征节点主信息类>(节点) : nullptr;
}

const 特征节点主信息类* 特征类::取特征主信息(const 特征节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<特征节点主信息类>(节点) : nullptr;
}

特征节点类* 特征类::创建特征(基础信息节点类* 宿主, 特征节点主信息类* 主信息)
{
    if (!基础信息_ || !主信息) return nullptr;
    return static_cast<特征节点类*>(基础信息_->添加子节点(宿主, 主信息));
}

特征节点类* 特征类::创建特征_按类型(
    基础信息节点类* 宿主,
    const 词性节点类* 特征类型,
    const 特征值& 当前值)
{
    if (!特征类型) return nullptr;

    auto* 主信息 = new 特征节点主信息类{};
    主信息->类型 = 特征类型;
    主信息->当前值 = 当前值;
    return 创建特征(宿主, 主信息);
}

特征节点类* 特征类::取或创建子特征_按类型(基础信息节点类* 宿主, const 词性节点类* 特征类型)
{
    if (auto* 命中 = 查找子特征_按类型(宿主, 特征类型)) {
        return 命中;
    }
    return 创建特征_按类型(宿主, 特征类型);
}

bool 特征类::删除特征(特征节点类* 节点)
{
    return 基础信息_ ? 基础信息_->删除节点(节点) : false;
}

std::vector<特征节点类*> 特征类::枚举全部特征() const
{
    std::vector<特征节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举节点_按类型<特征节点主信息类>()) {
        out.push_back(static_cast<特征节点类*>(节点));
    }
    return out;
}

std::vector<特征节点类*> 特征类::获取子特征(const 基础信息节点类* 宿主) const
{
    std::vector<特征节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举子节点_按类型<特征节点主信息类>(宿主)) {
        out.push_back(static_cast<特征节点类*>(节点));
    }
    return out;
}

特征节点类* 特征类::查找子特征_按类型(const 基础信息节点类* 宿主, const 词性节点类* 特征类型) const
{
    if (!特征类型) return nullptr;

    for (auto* 节点 : 获取子特征(宿主)) {
        const auto* 主信息 = 取特征主信息(节点);
        if (主信息 && 私有_词性相同(主信息->类型, 特征类型)) {
            return 节点;
        }
    }
    return nullptr;
}

特征写入结果 特征类::写入特征值_按参数(特征节点类* 节点, const 特征值& 值, const 写入参数& 参数)
{
    特征写入结果 结果{};
    结果.特征 = 节点;

    auto* 主信息 = 取特征主信息(节点);
    if (!主信息 || !特征值有效(值)) return 结果;

    const auto 现在 = 参数.ts != 0 ? 参数.ts : 结构体_时间戳::当前_微秒();
    主信息->当前值 = 值;
    主信息->统计.记录观测(现在);
    主信息->近帧候选.push(值);
    主信息->近帧时间.push(现在);

    if (参数.仅更新当前值) {
        return 结果;
    }

    const auto 计算差异度 = [&](const 特征值& 左值, const 特征值& 右值) noexcept -> I64 {
        I64 差异度 = 私有_默认不同值;
        if (差异度函数_) {
            差异度 = 差异度函数_(主信息->类型, 左值, 右值);
        } else if (const auto* 左标量 = std::get_if<I64>(&左值)) {
            if (const auto* 右标量 = std::get_if<I64>(&右值)) {
                差异度 = 私有_绝对值饱和(私有_饱和减(*左标量, *右标量));
            } else {
                差异度 = 私有_默认不同值;
            }
        } else {
            差异度 = 私有_特征值严格相等(左值, 右值) ? 0 : 私有_默认不同值;
        }

        if (const auto* 左标量 = std::get_if<I64>(&左值)) {
            const auto* 右标量 = std::get_if<I64>(&右值);
            if (右标量 && 主信息->连续测量误差区间.有效()) {
                const I64 delta = 私有_饱和减(*左标量, *右标量);
                return 私有_点到区间距离(delta, 主信息->连续测量误差区间);
            }
        }
        return 差异度;
    };

    const auto& 策略 = 参数.稳态;
    const int 连续帧要求 = std::max(2, 策略.连续N);
    if (主信息->近帧候选.size < static_cast<std::size_t>(连续帧要求)) {
        return 结果;
    }

    bool 稳定 = true;
    for (int i = 1; i < 连续帧要求; ++i) {
        const auto* 历史值 = 主信息->近帧候选.get_from_latest(static_cast<std::size_t>(i));
        if (!历史值 || 计算差异度(值, *历史值) > 策略.阈值_判稳) {
            稳定 = false;
            break;
        }
    }

    结果.通过候选门 = 稳定;
    if (!稳定) {
        return 结果;
    }

    const int 旧稳态索引 = 主信息->当前稳态索引;
    int 命中稳态索引 = -1;
    for (std::size_t i = 0; i < 主信息->稳态列表.size(); ++i) {
        auto& 稳态 = 主信息->稳态列表[i];
        if (!特征值有效(稳态.值)) continue;
        if (计算差异度(值, 稳态.值) <= 策略.阈值_命中原型) {
            命中稳态索引 = static_cast<int>(i);
            稳态.命中次数 += 1;
            稳态.最后命中 = 现在;
            if (const auto* 标量 = std::get_if<I64>(&值)) {
                稳态.区间 = I64区间::点(*标量);
            }
            break;
        }
    }

    if (命中稳态索引 < 0 && 参数.允许创建稳态) {
        特征稳态 新稳态{};
        新稳态.值 = 值;
        新稳态.命中次数 = 1;
        新稳态.最后命中 = 现在;
        if (const auto* 标量 = std::get_if<I64>(&值)) {
            新稳态.区间 = I64区间::点(*标量);
        }
        主信息->稳态列表.push_back(新稳态);
        命中稳态索引 = static_cast<int>(主信息->稳态列表.size()) - 1;
    }

    主信息->当前稳态索引 = 命中稳态索引;
    结果.稳态发生变化 = (命中稳态索引 >= 0 && 命中稳态索引 != 旧稳态索引);

    if (参数.允许淘汰 && 策略.淘汰_超时 > 0) {
        auto& 稳态列表 = 主信息->稳态列表;
        int 新当前稳态索引 = 主信息->当前稳态索引;
        for (std::size_t i = 0; i < 稳态列表.size();) {
            const auto& 稳态 = 稳态列表[i];
            const bool 超时 = 稳态.最后命中 > 0
                && 现在 > 稳态.最后命中
                && (现在 - 稳态.最后命中) > 策略.淘汰_超时;
            if (!超时) {
                ++i;
                continue;
            }

            if (新当前稳态索引 == static_cast<int>(i)) {
                新当前稳态索引 = -1;
            } else if (新当前稳态索引 > static_cast<int>(i)) {
                --新当前稳态索引;
            }
            稳态列表.erase(稳态列表.begin() + static_cast<std::ptrdiff_t>(i));
        }
        主信息->当前稳态索引 = 新当前稳态索引;
    }

    if (策略.Kmax_points > 0 && static_cast<int>(主信息->稳态列表.size()) > 策略.Kmax_points) {
        auto& 稳态列表 = 主信息->稳态列表;
        while (static_cast<int>(稳态列表.size()) > 策略.Kmax_points) {
            std::size_t 最弱索引 = 0;
            for (std::size_t i = 1; i < 稳态列表.size(); ++i) {
                const auto& 候选 = 稳态列表[i];
                const auto& 当前最弱 = 稳态列表[最弱索引];
                if (候选.命中次数 < 当前最弱.命中次数
                    || (候选.命中次数 == 当前最弱.命中次数 && 候选.最后命中 < 当前最弱.最后命中)) {
                    最弱索引 = i;
                }
            }

            if (主信息->当前稳态索引 == static_cast<int>(最弱索引)) {
                主信息->当前稳态索引 = -1;
            } else if (主信息->当前稳态索引 > static_cast<int>(最弱索引)) {
                --主信息->当前稳态索引;
            }

            稳态列表.erase(稳态列表.begin() + static_cast<std::ptrdiff_t>(最弱索引));
        }
    }

    return 结果;
}

bool 特征类::写入特征值(特征节点类* 节点, const 特征值& 值, 时间戳 now)
{
    写入参数 参数{};
    参数.ts = now;
    return 写入特征值_按参数(节点, 值, 参数).特征 != nullptr && 特征值有效(值);
}

bool 特征类::写入特征值_I64(特征节点类* 节点, I64 值, 时间戳 now)
{
    return 写入特征值(节点, 特征值{ 值 }, now);
}

bool 特征类::写入特征值_VecU句柄(特征节点类* 节点, VecU句柄 值, 时间戳 now)
{
    return 写入特征值(节点, 特征值{ 值 }, now);
}

bool 特征类::写入特征值_指针句柄(特征节点类* 节点, 指针句柄 值, 时间戳 now)
{
    return 写入特征值(节点, 特征值{ 值 }, now);
}

特征值 特征类::读取特征值(const 特征节点类* 节点) const
{
    const auto* 主信息 = 取特征主信息(节点);
    return 主信息 ? 主信息->当前值 : 特征值{};
}

枚举_特征值比较模式 特征类::获取比较方式(
    const 词性节点类*,
    const 特征值& 左值,
    const 特征值& 右值) const noexcept
{
    if (!特征值有效(左值) || !特征值有效(右值) || 左值.index() != 右值.index()) {
        return 枚举_特征值比较模式::相等;
    }

    if (std::holds_alternative<I64>(左值)) {
        return 枚举_特征值比较模式::有序标量;
    }
    if (std::holds_alternative<指针句柄>(左值)) {
        return 枚举_特征值比较模式::相等;
    }
    if (std::holds_alternative<VecU句柄>(左值)) {
        return 枚举_特征值比较模式::相等或相似度;
    }
    return 枚举_特征值比较模式::相等;
}

特征比较方法结果 特征类::获取比较方法结果(
    const 词性节点类* 特征类型,
    const 特征值& 左值,
    const 特征值& 右值,
    const 特征节点主信息类* 左特征主信息,
    const 特征节点主信息类* 右特征主信息) const
{
    特征比较方法结果 结果{};
    if (!特征值有效(左值) || !特征值有效(右值) || 左值.index() != 右值.index()) {
        return 结果;
    }

    结果.比较方式 = 获取比较方式(特征类型, 左值, 右值);

    switch (结果.比较方式) {
    case 枚举_特征值比较模式::有序标量: {
        const auto* 左标量 = std::get_if<I64>(&左值);
        const auto* 右标量 = std::get_if<I64>(&右值);
        if (!左标量 || !右标量) return 结果;

        const I64 原始差值 = 私有_饱和减(*右标量, *左标量);
        const I64区间 左误差区间 = 左特征主信息 ? 左特征主信息->连续测量误差区间 : I64区间{ 0, -1 };
        const I64区间 右误差区间 = 右特征主信息 ? 右特征主信息->连续测量误差区间 : I64区间{ 0, -1 };

        I64 调整后差值 = 原始差值;
        if (左误差区间.有效() && 右误差区间.有效()) {
            const I64 下界 = std::min(左误差区间.低值, 右误差区间.低值);
            const I64 上界 = std::max(左误差区间.高值, 右误差区间.高值);
            调整后差值 = 私有_区间外有符号差值(原始差值, I64区间{ 下界, 上界 });
        } else if (左误差区间.有效()) {
            调整后差值 = 私有_区间外有符号差值(原始差值, 左误差区间);
        } else if (右误差区间.有效()) {
            调整后差值 = 私有_区间外有符号差值(原始差值, 右误差区间);
        }

        结果.可比较 = true;
        结果.左比较值 = *左标量;
        结果.右比较值 = 私有_饱和加(*左标量, 调整后差值);
        结果.差值 = 调整后差值;
        return 结果;
    }
    case 枚举_特征值比较模式::相等: {
        结果.可比较 = true;
        结果.左比较值 = 0;
        结果.右比较值 = 私有_特征值严格相等(左值, 右值) ? 0 : 1;
        结果.差值 = 结果.右比较值;
        return 结果;
    }
    case 枚举_特征值比较模式::相等或相似度: {
        I64 差异度 = 10000;
        if (差异度函数_) {
            差异度 = 差异度函数_(特征类型, 左值, 右值);
        } else if (私有_特征值严格相等(左值, 右值)) {
            差异度 = 0;
        }
        结果.可比较 = true;
        结果.左比较值 = 0;
        结果.右比较值 = 差异度;
        结果.差值 = 差异度;
        return 结果;
    }
    default:
        return 结果;
    }
}

存在比较结果 特征类::比较特征集合(const 特征集合& A, const 特征集合& B, const 集合比较参数& 参数) const
{
    存在比较结果 结果{};

    std::unordered_map<const 词性节点类*, const 特征节点类*> 左表{};
    std::unordered_map<const 词性节点类*, const 特征节点类*> 右表{};

    const auto 构建索引 = [](const 特征集合& 集合, auto& 输出表) {
        for (const auto* 节点 : 集合) {
            if (!节点 || !节点->主信息) continue;
            const auto* 主信息 = dynamic_cast<const 特征节点主信息类*>(节点->主信息);
            if (!主信息 || !主信息->类型 || 输出表.contains(主信息->类型)) continue;
            输出表.emplace(主信息->类型, 节点);
        }
    };

    构建索引(A, 左表);
    构建索引(B, 右表);

    std::unordered_map<const 词性节点类*, bool> 全类型{};
    for (const auto& [类型, _] : 左表) 全类型.emplace(类型, true);
    for (const auto& [类型, _] : 右表) 全类型.emplace(类型, true);

    I64 加权差异和 = 0;
    I64 权重和 = 0;

    for (const auto& [类型, _] : 全类型) {
        特征比较明细 明细{};
        明细.类型 = 类型;

        const auto 左迭代器 = 左表.find(类型);
        const auto 右迭代器 = 右表.find(类型);
        if (左迭代器 == 左表.end()) {
            明细.缺失A = true;
            明细.差异度 = 参数.缺失惩罚;
        } else if (右迭代器 == 右表.end()) {
            明细.缺失B = true;
            明细.差异度 = 参数.缺失惩罚;
        } else {
            const auto* 左主信息 = 取特征主信息(左迭代器->second);
            const auto* 右主信息 = 取特征主信息(右迭代器->second);
            if (!左主信息 || !右主信息 || !特征值有效(左主信息->当前值) || !特征值有效(右主信息->当前值)) {
                明细.差异度 = 参数.缺失惩罚;
            } else if (差异度函数_) {
                明细.差异度 = 差异度函数_(类型, 左主信息->当前值, 右主信息->当前值);
            } else if (const auto* 左标量 = std::get_if<I64>(&左主信息->当前值)) {
                if (const auto* 右标量 = std::get_if<I64>(&右主信息->当前值)) {
                    明细.差异度 = 私有_绝对值饱和(私有_饱和减(*左标量, *右标量));
                } else {
                    明细.差异度 = 参数.缺失惩罚;
                }
            } else {
                明细.差异度 = 私有_特征值严格相等(左主信息->当前值, 右主信息->当前值)
                    ? 0
                    : 参数.缺失惩罚;
            }
        }

        if (!明细.缺失A && !明细.缺失B && 明细.差异度 == 0) {
            ++结果.命中特征数;
        }

        权重和 = 私有_饱和加(权重和, 明细.权重);
        if (明细.权重 > 0) {
            if (明细.差异度 > 0 && 明细.差异度 > I64_MAX / 明细.权重) {
                加权差异和 = I64_MAX;
            } else {
                加权差异和 = 私有_饱和加(加权差异和, 明细.差异度 * 明细.权重);
            }
        }

        if (参数.输出明细) {
            结果.明细.push_back(明细);
        }
    }

    结果.总差异度 = 权重和 > 0 ? (加权差异和 / 10000) : 0;
    结果.归一化差异度 = (参数.归一化 && 权重和 > 0)
        ? (加权差异和 / 权重和)
        : 结果.总差异度;
    return 结果;
}

std::string 特征类::生成度量签名(const 特征节点类* 节点) const
{
    const auto* 主信息 = 取特征主信息(节点);
    std::ostringstream 输出;
    输出 << "Feature";
    if (主信息 && 主信息->类型) {
        输出 << "|T=" << 主信息->类型->获取主键();
    }
    if (主信息 && 主信息->名称) {
        输出 << "|N=" << 主信息->名称->获取主键();
    }

    if (主信息) {
        if (std::holds_alternative<I64>(主信息->当前值)) {
            输出 << "|V=I64";
        } else if (std::holds_alternative<VecU句柄>(主信息->当前值)) {
            输出 << "|V=VecU";
        } else if (std::holds_alternative<指针句柄>(主信息->当前值)) {
            输出 << "|V=Ptr";
        } else {
            输出 << "|V=None";
        }
    }
    return 输出.str();
}
