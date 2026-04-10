#include "特征类.h"

namespace {
    bool 私有_词性相同(const 词性节点类* a, const 词性节点类* b) noexcept
    {
        if (a == b) return true;
        if (!a || !b) return false;
        return a->获取主键() == b->获取主键();
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

bool 特征类::写入特征值(特征节点类* 节点, const 特征值& 值, 时间戳 now)
{
    auto* 主信息 = 取特征主信息(节点);
    if (!主信息) return false;

    主信息->当前值 = 值;
    主信息->统计.记录观测(now);
    return true;
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
