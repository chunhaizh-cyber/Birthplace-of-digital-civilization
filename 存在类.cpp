#include "存在类.h"

#include "需求类.h"
#include "任务类.h"
#include "方法类.h"

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

    void 私有_递归释放基础信息主信息(基础信息节点类* 节点) noexcept
    {
        if (!节点) return;

        auto* 子节点头 = static_cast<基础信息节点类*>(节点->子);
        if (子节点头) {
            auto* 当前 = 子节点头;
            do {
                auto* 下一个 = static_cast<基础信息节点类*>(当前->下);
                私有_递归释放基础信息主信息(当前);
                当前 = 下一个;
            } while (当前 && 当前 != 子节点头);
        }

        delete 节点->主信息;
        节点->主信息 = nullptr;
    }
}

存在类::存在类(基础信息类* 基础信息) noexcept
    : 基础信息_(基础信息)
{
}

void 存在类::绑定基础信息(基础信息类* 基础信息) noexcept
{
    基础信息_ = 基础信息;
}

存在节点主信息类* 存在类::取存在主信息(存在节点类* 节点) const noexcept
{
    auto* 主信息 = 基础信息_ ? 基础信息_->取主信息<存在节点主信息类>(节点) : nullptr;
    if (主信息) {
        (void)私有_确保存在具备三类信息根节点(*主信息);
    }
    return 主信息;
}

const 存在节点主信息类* 存在类::取存在主信息(const 存在节点类* 节点) const noexcept
{
    const auto* 主信息 = 基础信息_ ? 基础信息_->取主信息<存在节点主信息类>(节点) : nullptr;
    if (主信息) {
        auto* 可补齐主信息 = const_cast<存在节点主信息类*>(主信息);
        (void)私有_确保存在具备三类信息根节点(*可补齐主信息);
    }
    return 主信息;
}

需求类::节点类* 存在类::获取需求根节点(存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<需求类::节点类*>(主信息->需求根节点) : nullptr;
}

const 需求类::节点类* 存在类::获取需求根节点(const 存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<const 需求类::节点类*>(主信息->需求根节点) : nullptr;
}

任务类::节点类* 存在类::获取任务根节点(存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<任务类::节点类*>(主信息->任务根节点) : nullptr;
}

const 任务类::节点类* 存在类::获取任务根节点(const 存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<const 任务类::节点类*>(主信息->任务根节点) : nullptr;
}

方法类::节点类* 存在类::获取方法根节点(存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<方法类::节点类*>(主信息->方法根节点) : nullptr;
}

const 方法类::节点类* 存在类::获取方法根节点(const 存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? reinterpret_cast<const 方法类::节点类*>(主信息->方法根节点) : nullptr;
}

存在节点类* 存在类::创建存在(基础信息节点类* 父节点, 存在节点主信息类* 主信息)
{
    if (!基础信息_ || !主信息) return nullptr;
    (void)私有_确保存在具备三类信息根节点(*主信息);
    return static_cast<存在节点类*>(基础信息_->添加子节点(父节点, 主信息));
}

存在节点类* 存在类::创建存在_按名称类型(
    基础信息节点类* 父节点,
    const 词性节点类* 名称,
    const 词性节点类* 类型)
{
    auto* 主信息 = new 存在节点主信息类{};
    主信息->名称 = 名称;
    主信息->类型 = 类型;
    return 创建存在(父节点, 主信息);
}

存在节点类* 存在类::取或创建子存在_按名称类型(
    基础信息节点类* 父节点,
    const 词性节点类* 名称,
    const 词性节点类* 类型)
{
    for (auto* 节点 : 获取子存在(父节点)) {
        const auto* 主信息 = 取存在主信息(节点);
        if (!主信息) continue;

        const bool 名称匹配 = !名称 || 私有_词性相同(主信息->名称, 名称);
        const bool 类型匹配 = !类型 || 私有_词性相同(主信息->类型, 类型);
        if (名称匹配 && 类型匹配) {
            return 节点;
        }
    }

    return 创建存在_按名称类型(父节点, 名称, 类型);
}

bool 存在类::删除存在(存在节点类* 节点)
{
    if (!基础信息_ || !节点) return false;

    私有_递归释放基础信息主信息(节点);
    return 基础信息_->删除节点(节点);
}

std::vector<存在节点类*> 存在类::枚举全部存在() const
{
    std::vector<存在节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举节点_按类型<存在节点主信息类>()) {
        out.push_back(static_cast<存在节点类*>(节点));
    }
    return out;
}

std::vector<存在节点类*> 存在类::获取子存在(const 基础信息节点类* 父节点) const
{
    std::vector<存在节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举子节点_按类型<存在节点主信息类>(父节点)) {
        out.push_back(static_cast<存在节点类*>(节点));
    }
    return out;
}

存在节点类* 存在类::查找子存在_按名称(const 基础信息节点类* 父节点, const 词性节点类* 名称) const
{
    if (!名称) return nullptr;

    for (auto* 节点 : 获取子存在(父节点)) {
        const auto* 主信息 = 取存在主信息(节点);
        if (主信息 && 私有_词性相同(主信息->名称, 名称)) {
            return 节点;
        }
    }
    return nullptr;
}

存在节点类* 存在类::查找子存在_按类型(const 基础信息节点类* 父节点, const 词性节点类* 类型) const
{
    if (!类型) return nullptr;

    for (auto* 节点 : 获取子存在(父节点)) {
        const auto* 主信息 = 取存在主信息(节点);
        if (主信息 && 私有_词性相同(主信息->类型, 类型)) {
            return 节点;
        }
    }
    return nullptr;
}

特征节点类* 存在类::创建特征_按类型(
    基础信息节点类* 宿主,
    const 词性节点类* 特征类型,
    const 特征值& 当前值)
{
    if (!基础信息_ || !特征类型) return nullptr;

    auto* 主信息 = new 特征节点主信息类{};
    主信息->类型 = 特征类型;
    主信息->当前值 = 当前值;
    return static_cast<特征节点类*>(基础信息_->添加子节点(宿主, 主信息));
}

std::vector<特征节点类*> 存在类::获取子特征(const 基础信息节点类* 宿主) const
{
    std::vector<特征节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举子节点_按类型<特征节点主信息类>(宿主)) {
        out.push_back(static_cast<特征节点类*>(节点));
    }
    return out;
}

特征节点类* 存在类::查找子特征_按类型(const 基础信息节点类* 宿主, const 词性节点类* 特征类型) const
{
    if (!特征类型) return nullptr;

    for (auto* 节点 : 获取子特征(宿主)) {
        const auto* 主信息 = 基础信息_->取主信息<特征节点主信息类>(节点);
        if (主信息 && 私有_词性相同(主信息->类型, 特征类型)) {
            return 节点;
        }
    }
    return nullptr;
}

bool 存在类::写入名称(存在节点类* 节点, const 词性节点类* 名称)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    主信息->名称 = 名称;
    return true;
}

bool 存在类::写入类型(存在节点类* 节点, const 词性节点类* 类型)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    主信息->类型 = 类型;
    return true;
}

bool 存在类::绑定概念模板(存在节点类* 节点, 存在节点类* 概念模板)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    主信息->概念模板 = 私有_生成引用(概念模板);
    return true;
}

bool 存在类::绑定内部世界(存在节点类* 节点, 场景节点类* 内部世界)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    主信息->内部世界 = 私有_生成引用(内部世界);
    return true;
}

bool 存在类::追加实例因果(存在节点类* 节点, 因果实例节点类* 因果实例)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息 || !因果实例) return false;
    if (!私有_引用已存在(主信息->实例因果, 因果实例)) {
        主信息->实例因果.push_back(私有_生成引用(因果实例));
    }
    return true;
}
