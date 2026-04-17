#include "场景类.h"

#include <limits>

#include "语素类.h"

namespace {
    bool 私有_词性相同(const 词性节点类* a, const 词性节点类* b) noexcept
    {
        if (a == b) return true;
        if (!a || !b) return false;
        return a->获取主键() == b->获取主键();
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

    时间戳 私有_取状态排序时间(const 状态节点主信息类& 主信息) noexcept
    {
        return 主信息.发生时间 != 0 ? 主信息.发生时间 : 主信息.收到时间;
    }

    void 私有_追加二次特征引用(场景节点主信息类* 主信息, 二次特征节点类* 二次特征节点)
    {
        if (!主信息 || !二次特征节点) return;
        const auto 引用 = 私有_生成引用(二次特征节点);
        if (!私有_引用已存在(主信息->关系索引, 二次特征节点)) {
            主信息->关系索引.push_back(引用);
        }
        if (!私有_引用已存在(主信息->二次特征索引, 二次特征节点)) {
            主信息->二次特征索引.push_back(引用);
        }
    }

    特征节点类* 私有_查找子特征_按类型(
        基础信息类* 基础信息,
        const 基础信息节点类* 宿主,
        const 词性节点类* 特征类型)
    {
        if (!基础信息 || !宿主 || !特征类型) return nullptr;

        for (auto* 节点 : 基础信息->枚举子节点_按类型<特征节点主信息类>(宿主)) {
            const auto* 主信息 = 基础信息->取主信息<特征节点主信息类>(节点);
            if (主信息 && 私有_词性相同(主信息->类型, 特征类型)) {
                return static_cast<特征节点类*>(节点);
            }
        }
        return nullptr;
    }

    void 私有_确保场景默认特征已初始化(基础信息类* 基础信息, 场景节点类* 场景)
    {
        if (!基础信息 || !场景) return;

        const auto* 安全度特征 = 语素集.添加词性词("安全度", "名词");
        if (!安全度特征) return;

        auto* 特征节点 = 私有_查找子特征_按类型(基础信息, 场景, 安全度特征);
        auto* 特征主信息 = 特征节点 ? 基础信息->取主信息<特征节点主信息类>(特征节点) : nullptr;
        if (!特征主信息) {
            auto* 新主信息 = new 特征节点主信息类{};
            新主信息->类型 = 安全度特征;
            新主信息->当前值 = I64{ (std::numeric_limits<I64>::max)() / 2 };
            特征节点 = static_cast<特征节点类*>(基础信息->添加子节点(场景, 新主信息));
            特征主信息 = 特征节点 ? 基础信息->取主信息<特征节点主信息类>(特征节点) : nullptr;
        }

        if (!特征主信息) return;
        特征主信息->类型 = 安全度特征;
        if (!特征主信息->有当前值()) {
            特征主信息->当前值 = I64{ (std::numeric_limits<I64>::max)() / 2 };
        }
    }
}

场景类::场景类(基础信息类* 基础信息) noexcept
    : 基础信息_(基础信息)
{
}

void 场景类::绑定基础信息(基础信息类* 基础信息) noexcept
{
    基础信息_ = 基础信息;
}

场景节点主信息类* 场景类::取场景主信息(场景节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<场景节点主信息类>(节点) : nullptr;
}

const 场景节点主信息类* 场景类::取场景主信息(const 场景节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<场景节点主信息类>(节点) : nullptr;
}

场景节点类* 场景类::创建场景(基础信息节点类* 父节点, 场景节点主信息类* 主信息)
{
    if (!基础信息_ || !主信息) return nullptr;
    auto* 场景节点 = static_cast<场景节点类*>(基础信息_->添加子节点(父节点, 主信息));
    私有_确保场景默认特征已初始化(基础信息_, 场景节点);
    return 场景节点;
}

场景节点类* 场景类::创建场景_按名称(
    基础信息节点类* 父节点,
    const 词性节点类* 名称,
    枚举_世界类型 世界类型)
{
    auto* 主信息 = new 场景节点主信息类{};
    主信息->名称 = 名称;
    主信息->世界类型 = 世界类型;
    return 创建场景(父节点, 主信息);
}

场景节点类* 场景类::取或创建子场景_按名称(
    基础信息节点类* 父节点,
    const 词性节点类* 名称,
    枚举_世界类型 世界类型)
{
    if (auto* 命中 = 查找子场景_按名称(父节点, 名称)) {
        if (auto* 主信息 = 取场景主信息(命中)) {
            主信息->世界类型 = 世界类型;
        }
        return 命中;
    }
    return 创建场景_按名称(父节点, 名称, 世界类型);
}

bool 场景类::删除场景(场景节点类* 节点)
{
    return 基础信息_ ? 基础信息_->删除节点(节点) : false;
}

std::vector<场景节点类*> 场景类::枚举全部场景() const
{
    std::vector<场景节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举节点_按类型<场景节点主信息类>()) {
        out.push_back(static_cast<场景节点类*>(节点));
    }
    return out;
}

std::vector<场景节点类*> 场景类::获取子场景(const 基础信息节点类* 父节点) const
{
    std::vector<场景节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举子节点_按类型<场景节点主信息类>(父节点)) {
        out.push_back(static_cast<场景节点类*>(节点));
    }
    return out;
}

std::vector<存在节点类*> 场景类::获取子存在(const 基础信息节点类* 父节点) const
{
    std::vector<存在节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举子节点_按类型<存在节点主信息类>(父节点)) {
        out.push_back(static_cast<存在节点类*>(节点));
    }
    return out;
}

std::vector<特征节点类*> 场景类::获取子特征(const 基础信息节点类* 父节点) const
{
    std::vector<特征节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举子节点_按类型<特征节点主信息类>(父节点)) {
        out.push_back(static_cast<特征节点类*>(节点));
    }
    return out;
}

场景节点类* 场景类::查找子场景_按名称(const 基础信息节点类* 父节点, const 词性节点类* 名称) const
{
    if (!名称) return nullptr;

    for (auto* 节点 : 获取子场景(父节点)) {
        const auto* 主信息 = 取场景主信息(节点);
        if (主信息 && 私有_词性相同(主信息->名称, 名称)) {
            return 节点;
        }
    }
    return nullptr;
}

bool 场景类::绑定宿主(场景节点类* 节点, 存在节点类* 宿主存在)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息) return false;
    主信息->宿主存在 = 私有_生成引用(宿主存在);
    return true;
}

bool 场景类::绑定来源文章(场景节点类* 节点, 文章节点类* 来源文章)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息) return false;
    主信息->来源文章 = 私有_生成引用(来源文章);
    return true;
}

bool 场景类::追加状态(场景节点类* 节点, 状态节点类* 状态节点)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息 || !状态节点) return false;
    if (!私有_引用已存在(主信息->状态索引, 状态节点)) {
        主信息->状态索引.push_back(私有_生成引用(状态节点));
    }
    std::stable_sort(主信息->状态索引.begin(), 主信息->状态索引.end(), [this](const auto& 左, const auto& 右) {
        const auto* 左主信息 = 左.指针 ? 基础信息_->取主信息<状态节点主信息类>(左.指针) : nullptr;
        const auto* 右主信息 = 右.指针 ? 基础信息_->取主信息<状态节点主信息类>(右.指针) : nullptr;
        const auto 左时间 = 左主信息 ? 私有_取状态排序时间(*左主信息) : 0;
        const auto 右时间 = 右主信息 ? 私有_取状态排序时间(*右主信息) : 0;
        if (左时间 != 右时间) return 左时间 < 右时间;
        return 左.主键 < 右.主键;
    });
    return true;
}

bool 场景类::追加动态(场景节点类* 节点, 动态节点类* 动态节点)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息 || !动态节点) return false;
    if (!私有_引用已存在(主信息->动态索引, 动态节点)) {
        主信息->动态索引.push_back(私有_生成引用(动态节点));
    }
    return true;
}

bool 场景类::追加关系(场景节点类* 节点, 二次特征节点类* 二次特征节点)
{
    return 追加二次特征(节点, 二次特征节点);
}

bool 场景类::追加二次特征(场景节点类* 节点, 二次特征节点类* 二次特征节点)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息 || !二次特征节点) return false;
    私有_追加二次特征引用(主信息, 二次特征节点);
    return true;
}

bool 场景类::追加实例因果(场景节点类* 节点, 因果实例节点类* 因果实例节点)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息 || !因果实例节点) return false;
    if (!私有_引用已存在(主信息->实例因果索引, 因果实例节点)) {
        主信息->实例因果索引.push_back(私有_生成引用(因果实例节点));
    }
    return true;
}
