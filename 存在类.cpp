#include "存在类.h"

#include <cmath>

#include "需求类.h"
#include "任务类.h"
#include "方法类.h"

namespace {
    std::int64_t 私有_计算距离_mm(const Vector3D& 左位置, const Vector3D& 右位置) noexcept
    {
        const long double dx = static_cast<long double>(左位置.x) - static_cast<long double>(右位置.x);
        const long double dy = static_cast<long double>(左位置.y) - static_cast<long double>(右位置.y);
        const long double dz = static_cast<long double>(左位置.z) - static_cast<long double>(右位置.z);
        return static_cast<std::int64_t>(std::llround(std::sqrt(dx * dx + dy * dy + dz * dz)));
    }

    bool 私有_语素入口相同(const 语素入口节点类* a, const 语素入口节点类* b) noexcept
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

bool 存在类::确保存在三类根节点(存在节点类* 节点) const noexcept
{
    auto* 主信息 = 取存在主信息(节点);
    return 主信息 ? 私有_确保存在具备三类信息根节点(*主信息) : false;
}

bool 存在类::确保存在三类根节点(const 存在节点类* 节点) const noexcept
{
    const auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    auto* 可补齐主信息 = const_cast<存在节点主信息类*>(主信息);
    return 私有_确保存在具备三类信息根节点(*可补齐主信息);
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
    const 语素入口节点类* 名称,
    const 语素入口节点类* 类型)
{
    auto* 主信息 = new 存在节点主信息类{};
    主信息->名称 = 名称;
    主信息->类型 = 类型;
    return 创建存在(父节点, 主信息);
}

存在节点类* 存在类::取或创建子存在_按名称类型(
    基础信息节点类* 父节点,
    const 语素入口节点类* 名称,
    const 语素入口节点类* 类型)
{
    for (auto* 节点 : 获取子存在(父节点)) {
        const auto* 主信息 = 取存在主信息(节点);
        if (!主信息) continue;

        const bool 名称匹配 = !名称 || 私有_语素入口相同(主信息->名称, 名称);
        const bool 类型匹配 = !类型 || 私有_语素入口相同(主信息->类型, 类型);
        if (名称匹配 && 类型匹配) {
            return 节点;
        }
    }

    return 创建存在_按名称类型(父节点, 名称, 类型);
}

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

存在节点类* 存在类::取或创建子存在_按类型并按特征I64(
    基础信息节点类* 父节点,
    const 语素入口节点类* 类型,
    const 语素入口节点类* 唯一键特征类型,
    I64 唯一键特征值)
{
    if (!类型 || !唯一键特征类型) return nullptr;

    for (auto* 存在节点 : 获取子存在(父节点)) {
        const auto* 主信息 = 取存在主信息(存在节点);
        if (!主信息 || !私有_语素入口相同(主信息->类型, 类型)) {
            continue;
        }

        auto* 特征节点 = 查找子特征_按类型(存在节点, 唯一键特征类型);
        const auto* 特征主信息 = 基础信息_ ? 基础信息_->取主信息<特征节点主信息类>(特征节点) : nullptr;
        if (!特征主信息) continue;

        const auto* 标量值 = std::get_if<I64>(&特征主信息->当前值);
        if (标量值 && (*标量值 == 唯一键特征值)) {
            return 存在节点;
        }
    }

    auto* 新存在 = 创建存在_按名称类型(父节点, nullptr, 类型);
    if (!新存在) return nullptr;

    auto* 特征节点 = 创建特征_按类型(新存在, 唯一键特征类型, 特征值{ 唯一键特征值 });
    (void)特征节点;
    return 新存在;
}

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

    std::vector<存在节点类*> 待扩充概念{};
    for (auto* 概念 : 输入概念集) {
        if (!概念 || 概念 == 实例) continue;
        (void)添加实例概念(实例, 概念, false);
        私有_追加唯一裸指针(待扩充概念, 概念);
    }
    根据概念扩充实例记忆(实例, 待扩充概念);
    return 实例;
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

存在节点类* 存在类::查找子存在_按名称(const 基础信息节点类* 父节点, const 语素入口节点类* 名称) const
{
    if (!名称) return nullptr;

    for (auto* 节点 : 获取子存在(父节点)) {
        const auto* 主信息 = 取存在主信息(节点);
        if (主信息 && 私有_语素入口相同(主信息->名称, 名称)) {
            return 节点;
        }
    }
    return nullptr;
}

存在节点类* 存在类::查找子存在_按类型(const 基础信息节点类* 父节点, const 语素入口节点类* 类型) const
{
    if (!类型) return nullptr;

    for (auto* 节点 : 获取子存在(父节点)) {
        const auto* 主信息 = 取存在主信息(节点);
        if (主信息 && 私有_语素入口相同(主信息->类型, 类型)) {
            return 节点;
        }
    }
    return nullptr;
}

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

特征节点类* 存在类::创建特征_按类型(
    基础信息节点类* 宿主,
    const 语素入口节点类* 特征类型,
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

特征节点类* 存在类::查找子特征_按类型(const 基础信息节点类* 宿主, const 语素入口节点类* 特征类型) const
{
    if (!特征类型) return nullptr;

    for (auto* 节点 : 获取子特征(宿主)) {
        const auto* 主信息 = 基础信息_->取主信息<特征节点主信息类>(节点);
        if (主信息 && 私有_语素入口相同(主信息->类型, 特征类型)) {
            return 节点;
        }
    }
    return nullptr;
}

bool 存在类::写入名称(存在节点类* 节点, const 语素入口节点类* 名称)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    主信息->名称 = 名称;
    return true;
}

bool 存在类::写入类型(存在节点类* 节点, const 语素入口节点类* 类型)
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
    if (概念模板 && !私有_引用已存在(主信息->概念集, 概念模板)) {
        主信息->概念集.push_back(私有_生成引用(概念模板));
    }
    return true;
}

bool 存在类::添加实例概念(
    存在节点类* 实例,
    存在节点类* 概念,
    bool 是否扩充记忆)
{
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
    if (是否扩充记忆) {
        根据概念扩充实例记忆(实例, std::vector<存在节点类*>{ 概念 });
    }
    return 已添加;
}

void 存在类::根据概念扩充实例记忆(
    存在节点类* 实例,
    const std::vector<存在节点类*>& 输入概念集)
{
    if (!基础信息_ || !实例) return;

    std::vector<存在节点类*> 已收集{};
    std::vector<存在节点类*> 扩充顺序{};
    const auto 收集概念 = [&](auto&& self, 存在节点类* 概念) -> void {
        if (!概念 || 概念 == 实例) return;
        const auto 已有数量 = 已收集.size();
        私有_追加唯一裸指针(已收集, 概念);
        if (已收集.size() == 已有数量) return;

        for (auto* 上位概念 : 获取概念集(概念)) {
            self(self, 上位概念);
        }
        扩充顺序.push_back(概念);
    };

    for (auto* 概念 : 输入概念集) {
        收集概念(收集概念, 概念);
    }

    for (auto* 概念 : 扩充顺序) {
        (void)添加实例概念(实例, 概念, false);
        for (auto* 概念特征 : 获取子特征(概念)) {
            const auto* 特征主信息 = 基础信息_->取主信息<特征节点主信息类>(概念特征);
            if (!特征主信息 || !特征主信息->类型) continue;
            if (查找子特征_按类型(实例, 特征主信息->类型)) continue;
            (void)创建特征_按类型(
                实例,
                特征主信息->类型,
                特征主信息->当前值);
        }
    }
}

bool 存在类::绑定内部世界(存在节点类* 节点, 场景节点类* 内部世界)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;
    主信息->内部世界 = 私有_生成引用(内部世界);
    return true;
}

bool 存在类::写入观测位置(存在节点类* 节点, const Vector3D& 位置_mm)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;

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

bool 存在类::标记观测未命中(存在节点类* 节点)
{
    auto* 主信息 = 取存在主信息(节点);
    if (!主信息) return false;

    ++主信息->连续未命中帧;
    主信息->连续静止帧 = 0;
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
