#include "场景类.h"

#include <algorithm>
#include <mutex>

#include "场景索引同步.h"
#include "语素类.h"


namespace {

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

    template<class T主信息>
    std::vector<基础信息节点类*> 私有_直接基础信息子节点_按类型(
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
            if (dynamic_cast<T主信息*>(当前->主信息)) {
                结果.push_back(当前);
            }
            当前 = static_cast<基础信息节点类*>(当前->下);
        } while (当前 && 当前 != 首节点);
        return 结果;
    }

    // 功能：服务所在模块的内部辅助流程。
    时间戳 私有_取状态排序时间(const 状态节点主信息类& 主信息) noexcept
    {
        return 主信息.发生时间 != 0 ? 主信息.发生时间 : 主信息.收到时间;
    }

    // 功能：服务所在模块的内部辅助流程。
    时间戳 私有_取动态排序时间(const 动态节点主信息类& 主信息) noexcept
    {
        return 主信息.时间.止 != 0 ? 主信息.时间.止 : 主信息.时间.起;
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_追加二次特征引用(场景节点主信息类* 主信息, 二次特征节点类* 二次特征节点)
    {
        if (!主信息 || !二次特征节点) return;
        std::lock_guard<std::recursive_mutex> 锁(借用场景索引全局互斥());
        const auto 引用 = 私有_生成引用(二次特征节点);
        if (!私有_引用已存在(主信息->二次特征索引, 二次特征节点)) {
            主信息->二次特征索引.push_back(引用);
        }
    }

}

// 功能：按函数名执行对应处理。
场景类::场景类(基础信息类* 基础信息) noexcept
    : 基础信息_(基础信息)
{
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
void 场景类::绑定基础信息(基础信息类* 基础信息) noexcept
{
    基础信息_ = 基础信息;
}

// 功能：按函数名执行对应处理。
场景节点主信息类* 场景类::取场景主信息(场景节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<场景节点主信息类>(节点) : nullptr;
}

// 功能：按函数名执行对应处理。
const 场景节点主信息类* 场景类::取场景主信息(const 场景节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<场景节点主信息类>(节点) : nullptr;
}

// 功能：判断基础信息节点是否为场景节点，不创建或修改节点。
bool 场景类::是场景节点(const 基础信息节点类* 节点) const noexcept
{
    return 基础信息_ && 基础信息_->取主信息<场景节点主信息类>(节点) != nullptr;
}

// 功能：按主键查找并解析场景节点，不创建或修改节点。
场景节点类* 场景类::按主键解析场景节点(const std::string& 主键) const noexcept
{
    if (!基础信息_ || 主键.empty()) {
        return nullptr;
    }
    auto* 节点 = 基础信息_->查找主键(主键);
    return 基础信息_->取主信息<场景节点主信息类>(节点)
        ? static_cast<场景节点类*>(节点)
        : nullptr;
}

// 功能：创建并返回或登记对应对象。
场景节点类* 场景类::创建场景(基础信息节点类* 父节点, 场景节点主信息类* 主信息)
{
    if (!基础信息_ || !主信息) return nullptr;
    return static_cast<场景节点类*>(基础信息_->添加子节点(父节点, 主信息));
}

// 功能：创建并返回或登记对应对象。
场景节点类* 场景类::创建场景_按名称(
    基础信息节点类* 父节点,
    const 语素入口节点类* 名称,
    枚举_世界类型 世界类型)
{
    auto* 主信息 = new 场景节点主信息类{};
    主信息->名称 = 名称;
    主信息->世界类型 = 世界类型;
    return 创建场景(父节点, 主信息);
}

// 功能：创建并返回或登记对应对象。
场景节点类* 场景类::取或创建子场景_按名称(
    基础信息节点类* 父节点,
    const 语素入口节点类* 名称,
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

// 功能：删除指定对象、状态或登记项。
bool 场景类::删除场景(场景节点类* 节点)
{
    return 基础信息_ ? 基础信息_->删除节点(节点) : false;
}

// 功能：按函数名执行对应处理。
std::vector<场景节点类*> 场景类::枚举全部场景() const
{
    std::vector<场景节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举节点_按类型<场景节点主信息类>()) {
        out.push_back(static_cast<场景节点类*>(节点));
    }
    return out;
}

// 功能：读取并返回指定对象、状态或运行材料。
std::vector<场景节点类*> 场景类::获取子场景(const 基础信息节点类* 父节点) const
{
    std::vector<场景节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 私有_直接基础信息子节点_按类型<场景节点主信息类>(*基础信息_, 父节点)) {
        out.push_back(static_cast<场景节点类*>(节点));
    }
    return out;
}

// 功能：读取并返回指定对象、状态或运行材料。
std::vector<存在节点类*> 场景类::获取子存在(const 基础信息节点类* 父节点) const
{
    std::vector<存在节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 私有_直接基础信息子节点_按类型<存在节点主信息类>(*基础信息_, 父节点)) {
        out.push_back(static_cast<存在节点类*>(节点));
    }
    return out;
}

// 功能：读取并返回指定对象、状态或运行材料。
std::vector<特征节点类*> 场景类::获取子特征(const 基础信息节点类* 父节点) const
{
    std::vector<特征节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 私有_直接基础信息子节点_按类型<特征节点主信息类>(*基础信息_, 父节点)) {
        out.push_back(static_cast<特征节点类*>(节点));
    }
    return out;
}

// 功能：按条件查找目标对象、方法或事实。
场景节点类* 场景类::查找子场景_按名称(const 基础信息节点类* 父节点, const 语素入口节点类* 名称) const
{
    if (!名称) return nullptr;

    for (auto* 节点 : 获取子场景(父节点)) {
        const auto* 主信息 = 取场景主信息(节点);
        if (主信息 && 语素入口同一(主信息->名称, 名称)) {
            return 节点;
        }
    }
    return nullptr;
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
bool 场景类::绑定宿主(场景节点类* 节点, 存在节点类* 宿主存在)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息) return false;
    主信息->宿主存在 = 私有_生成引用(宿主存在);
    return true;
}

// 功能：读取场景宿主存在引用，优先返回缓存指针，缺指针时按主键只读解析。
存在节点类* 场景类::读取场景宿主存在(const 场景节点类* 场景) const noexcept
{
    const auto* 主信息 = 取场景主信息(场景);
    if (!主信息) return nullptr;
    if (主信息->宿主存在.指针) return 主信息->宿主存在.指针;
    if (!基础信息_ || 主信息->宿主存在.主键.empty()) return nullptr;

    return static_cast<存在节点类*>(基础信息_->查找主键(主信息->宿主存在.主键));
}

// 功能：读取场景状态事实目录的引用快照，不解释状态值业务含义。
std::vector<可解析引用<状态节点类>> 场景类::读取场景状态索引快照(
    const 场景节点类* 场景) const
{
    std::vector<可解析引用<状态节点类>> 快照;
    const auto* 主信息 = 取场景主信息(场景);
    if (!主信息) return 快照;

    std::lock_guard<std::recursive_mutex> 锁(借用场景索引全局互斥());
    快照 = 主信息->状态索引;
    return 快照;
}

// 功能：读取场景动态事实目录的引用快照，不解释动态语义。
std::vector<可解析引用<动态节点类>> 场景类::读取场景动态索引快照(
    const 场景节点类* 场景) const
{
    std::vector<可解析引用<动态节点类>> 快照;
    const auto* 主信息 = 取场景主信息(场景);
    if (!主信息) return 快照;

    std::lock_guard<std::recursive_mutex> 锁(借用场景索引全局互斥());
    快照 = 主信息->动态索引;
    return 快照;
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
bool 场景类::绑定来源文章(场景节点类* 节点, 文章节点类* 来源文章)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息) return false;
    主信息->来源文章 = 私有_生成引用(来源文章);
    return true;
}

// 功能：按函数名执行对应处理。
bool 场景类::追加状态(场景节点类* 节点, 状态节点类* 状态节点)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息 || !状态节点) return false;
    // 场景只登记事实索引；不在场景层沉淀任务状态、需求满足或结算判断。
    std::lock_guard<std::recursive_mutex> 锁(借用场景索引全局互斥());
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

// 功能：从场景状态事实目录移除指定状态引用，不删除状态节点。
bool 场景类::移除场景状态索引(场景节点类* 场景, 状态节点类* 状态节点)
{
    if (!状态节点) return false;

    可解析引用<状态节点类> 状态引用{};
    状态引用.指针 = 状态节点;
    状态引用.主键 = 状态节点->获取主键();
    return 移除场景状态索引引用(场景, 状态引用);
}

// 功能：按状态引用从场景状态事实目录移除状态引用，不删除状态节点。
bool 场景类::移除场景状态索引引用(场景节点类* 场景, const 可解析引用<状态节点类>& 状态引用)
{
    auto* 主信息 = 取场景主信息(场景);
    if (!主信息 || (!状态引用.指针 && 状态引用.主键.empty())) return false;

    std::lock_guard<std::recursive_mutex> 锁(借用场景索引全局互斥());
    auto& 索引 = 主信息->状态索引;
    const auto 原数量 = 索引.size();
    索引.erase(
        std::remove_if(
            索引.begin(),
            索引.end(),
            [&](const auto& 项) {
                return (状态引用.指针 && 项.指针 == 状态引用.指针)
                    || (!状态引用.主键.empty() && 项.主键 == 状态引用.主键);
            }),
        索引.end());
    return 索引.size() != 原数量;
}

// 功能：按函数名执行对应处理。
bool 场景类::追加动态(场景节点类* 节点, 动态节点类* 动态节点)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息 || !动态节点) return false;
    // 动态语义由动态节点来源动作、动态特征和状态比较推导；场景仅保存引用。
    std::lock_guard<std::recursive_mutex> 锁(借用场景索引全局互斥());
    if (!私有_引用已存在(主信息->动态索引, 动态节点)) {
        主信息->动态索引.push_back(私有_生成引用(动态节点));
    }
    std::stable_sort(主信息->动态索引.begin(), 主信息->动态索引.end(), [this](const auto& 左, const auto& 右) {
        const auto* 左主信息 = (基础信息_ && 左.指针) ? 基础信息_->取主信息<动态节点主信息类>(左.指针) : nullptr;
        const auto* 右主信息 = (基础信息_ && 右.指针) ? 基础信息_->取主信息<动态节点主信息类>(右.指针) : nullptr;
        const auto 左时间 = 左主信息 ? 私有_取动态排序时间(*左主信息) : 0;
        const auto 右时间 = 右主信息 ? 私有_取动态排序时间(*右主信息) : 0;
        if (左时间 != 右时间) return 左时间 < 右时间;
        return 左.主键 < 右.主键;
    });
    return true;
}

// 功能：从场景动态事实目录移除指定动态引用，不删除动态节点。
bool 场景类::移除场景动态索引(场景节点类* 场景, 动态节点类* 动态节点)
{
    auto* 主信息 = 取场景主信息(场景);
    if (!主信息 || !动态节点) return false;

    const auto 动态主键 = 动态节点->获取主键();
    std::lock_guard<std::recursive_mutex> 锁(借用场景索引全局互斥());
    auto& 索引 = 主信息->动态索引;
    const auto 原数量 = 索引.size();
    索引.erase(
        std::remove_if(
            索引.begin(),
            索引.end(),
            [&](const auto& 项) {
                return 项.指针 == 动态节点 || (!动态主键.empty() && 项.主键 == 动态主键);
            }),
        索引.end());
    return 索引.size() != 原数量;
}

// 功能：按函数名执行对应处理。
bool 场景类::追加关系(场景节点类* 节点, 二次特征节点类* 二次特征节点)
{
    return 追加二次特征(节点, 二次特征节点);
}

// 功能：按函数名执行对应处理。
bool 场景类::追加二次特征(场景节点类* 节点, 二次特征节点类* 二次特征节点)
{
    auto* 主信息 = 取场景主信息(节点);
    if (!主信息 || !二次特征节点) return false;
    // 二次特征是场景内关系事实，场景本身不解释其业务含义。
    私有_追加二次特征引用(主信息, 二次特征节点);
    return true;
}

// 功能：从场景二次特征事实目录移除指定二次特征引用，不删除二次特征节点。
bool 场景类::移除场景二次特征索引(场景节点类* 场景, 二次特征节点类* 二次特征节点)
{
    auto* 主信息 = 取场景主信息(场景);
    if (!主信息 || !二次特征节点) return false;

    const auto 二次特征主键 = 二次特征节点->获取主键();
    std::lock_guard<std::recursive_mutex> 锁(借用场景索引全局互斥());
    auto& 索引 = 主信息->二次特征索引;
    const auto 原数量 = 索引.size();
    索引.erase(
        std::remove_if(
            索引.begin(),
            索引.end(),
            [&](const auto& 项) {
                return 项.指针 == 二次特征节点
                    || (!二次特征主键.empty() && 项.主键 == 二次特征主键);
            }),
        索引.end());
    return 索引.size() != 原数量;
}

// 功能：读取场景二次特征事实目录的引用快照，不解释二次特征业务含义。
std::vector<可解析引用<二次特征节点类>> 场景类::读取场景二次特征索引快照(
    const 场景节点类* 场景) const
{
    std::vector<可解析引用<二次特征节点类>> 快照;
    const auto* 主信息 = 取场景主信息(场景);
    if (!主信息) return 快照;

    std::lock_guard<std::recursive_mutex> 锁(借用场景索引全局互斥());
    快照 = 主信息->二次特征索引;
    return 快照;
}
