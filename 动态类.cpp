#include "动态类.h"

#include <algorithm>
#include <functional>
#include <string_view>

#include "方法类.h"
#include "世界树类.h"
#include "语素类.h"

import 二次特征应用模块;

namespace {
    template<class T节点>
    可解析引用<T节点> 私有_生成引用(T节点* 节点)
    {
        可解析引用<T节点> out{};
        out.指针 = 节点;
        if (节点) out.主键 = 节点->获取主键();
        return out;
    }

    std::string 私有_词标题(const 词性节点类* 词)
    {
        return 词 ? 词->获取主键() : std::string{};
    }

    bool 私有_包含片段(const std::string& 文本, const char* 片段)
    {
        return !文本.empty() && 片段 && 文本.find(片段) != std::string::npos;
    }

    std::string 私有_动态动作标题(const 动态节点主信息类& 主信息)
    {
        if (主信息.来源动作名) {
            return 私有_词标题(主信息.来源动作名);
        }
        const auto* 来源动作 = reinterpret_cast<const 方法类::节点类*>(主信息.来源动作.指针);
        return 来源动作 && 来源动作->主信息.动作名
            ? 私有_词标题(来源动作->主信息.动作名)
            : std::string{};
    }

    std::string 私有_动态特征标题(const 动态节点主信息类& 主信息)
    {
        return 主信息.动态特征.指针 && 主信息.动态特征.指针->主信息
            ? 私有_词标题(主信息.动态特征.指针->主信息->类型)
            : std::string{};
    }

    枚举_动态业务语义类型 私有_推导动态业务语义类型(const 动态节点主信息类& 主信息)
    {
        const auto 动作标题 = 私有_动态动作标题(主信息);
        const auto 特征标题 = 私有_动态特征标题(主信息);

        if (私有_包含片段(动作标题, "管理_")
            || 私有_包含片段(特征标题, "任务管理_")) {
            return 枚举_动态业务语义类型::治理过程动态;
        }

        if (主信息.来源动作相位 == 枚举_动作事件相位::失败
            || 私有_包含片段(动作标题, "进入收束")
            || 私有_包含片段(动作标题, "进入挂起")
            || 私有_包含片段(动作标题, "进入完成")
            || 私有_包含片段(动作标题, "进入失败")
            || 私有_包含片段(动作标题, "进入等待")
            || 私有_包含片段(动作标题, "停止")
            || 私有_包含片段(动作标题, "待机")
            || 私有_包含片段(动作标题, "封闭")) {
            return 枚举_动态业务语义类型::边界结束动态;
        }

        if (主信息.来源类型 == 枚举_动态来源类型::内部机制动作
            || 私有_包含片段(动作标题, "衰减")
            || 私有_包含片段(动作标题, "回归")
            || 私有_包含片段(动作标题, "回升")
            || 私有_包含片段(动作标题, "修复")
            || 私有_包含片段(动作标题, "保护")
            || 私有_包含片段(动作标题, "重算")
            || 私有_包含片段(动作标题, "校正")
            || 私有_包含片段(动作标题, "锚点")) {
            return 枚举_动态业务语义类型::被动记账动态;
        }

        if (!动作标题.empty() || 主信息.来源动作.指针) {
            return 枚举_动态业务语义类型::动作事实动态;
        }

        if (!特征标题.empty()
            || 主信息.初始状态.指针
            || 主信息.结果状态.指针) {
            return 枚举_动态业务语义类型::状态变化动态;
        }

        return 枚举_动态业务语义类型::未定义;
    }

    void 私有_刷新动态业务语义类型(动态节点主信息类* 主信息)
    {
        if (!主信息) {
            return;
        }
        主信息->业务语义类型 = 私有_推导动态业务语义类型(*主信息);
    }

    const 词性节点类* 私有_推导动态名称类型(const 动态节点主信息类& 主信息) noexcept
    {
        if (主信息.来源动作名) {
            return 主信息.来源动作名;
        }
        if (主信息.动态特征.指针 && 主信息.动态特征.指针->主信息) {
            if (const auto* 特征类型 = 主信息.动态特征.指针->主信息->类型) {
                return 特征类型;
            }
        }
        return 语素集.添加词性词("动态", "名词");
    }

    void 私有_补齐动态名称类型(动态节点主信息类* 主信息) noexcept
    {
        if (!主信息) {
            return;
        }
        const auto* 通用名称 = 私有_推导动态名称类型(*主信息);
        if (!主信息->名称) {
            主信息->名称 = 通用名称;
        }
        if (!主信息->类型) {
            主信息->类型 = 通用名称;
        }
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
    bool 私有_引用匹配节点(const 可解析引用<T节点>& 引用, const T节点* 节点) noexcept
    {
        if (!节点) return false;
        if (引用.指针 == 节点) return true;
        return !引用.主键.empty() && 引用.主键 == 节点->获取主键();
    }

    bool 私有_特征值相等(const 特征值& 左, const 特征值& 右) noexcept
    {
        if (左.index() != 右.index()) return false;
        if (const auto* 左标量 = std::get_if<I64>(&左)) {
            return *左标量 == std::get<I64>(右);
        }
        if (const auto* 左向量 = std::get_if<VecU句柄>(&左)) {
            return *左向量 == std::get<VecU句柄>(右);
        }
        if (const auto* 左指针 = std::get_if<指针句柄>(&左)) {
            return *左指针 == std::get<指针句柄>(右);
        }
        return true;
    }

    时间戳 私有_取状态时间(const 状态节点主信息类& 主信息) noexcept
    {
        return 主信息.发生时间 != 0 ? 主信息.发生时间 : 主信息.收到时间;
    }

    时间戳 私有_取动态时间(const 动态节点主信息类& 主信息) noexcept
    {
        return 主信息.时间.止 != 0 ? 主信息.时间.止 : 主信息.时间.起;
    }

    bool 私有_可形成基础动态(
        const 状态节点主信息类* 前状态,
        const 状态节点主信息类* 后状态) noexcept
    {
        if (!前状态 || !后状态) return false;
        if (!前状态->状态主体.有效() || !后状态->状态主体.有效()) return false;
        if (!前状态->状态特征.有效() || !后状态->状态特征.有效()) return false;
        const auto 前主体 = 前状态->状态主体.指针;
        const auto 后主体 = 后状态->状态主体.指针;
        const auto 前特征 = 前状态->状态特征.指针;
        const auto 后特征 = 后状态->状态特征.指针;
        if (!前主体 || !后主体 || !前特征 || !后特征) return false;
        if (前主体 != 后主体) return false;
        if (前特征 != 后特征) return false;
        return 私有_取状态时间(*前状态) <= 私有_取状态时间(*后状态);
    }

    std::uint64_t 私有_计算动态路径签名(
        const 状态节点主信息类* 初始状态,
        const 状态节点主信息类* 结果状态,
        const 基础信息节点类* 动态主体,
        const 特征节点类* 动态特征) noexcept
    {
        const std::uint64_t pack[6] = {
            static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(动态主体)),
            static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(动态特征)),
            static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(初始状态 ? 初始状态->状态主体.指针 : nullptr)),
            static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(结果状态 ? 结果状态->状态主体.指针 : nullptr)),
            static_cast<std::uint64_t>(初始状态 ? 私有_取状态时间(*初始状态) : 0),
            static_cast<std::uint64_t>(结果状态 ? 私有_取状态时间(*结果状态) : 0),
        };
        return static_cast<std::uint64_t>(std::hash<std::string_view>{}(
            std::string_view(reinterpret_cast<const char*>(pack), sizeof(pack))));
    }

    std::uint64_t 私有_计算动作动态路径签名(
        const 基础信息节点类* 动态主体,
        const 特征节点类* 动态特征,
        时间戳 开始时间,
        时间戳 结束时间) noexcept
    {
        const std::uint64_t pack[5] = {
            static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(动态主体)),
            static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(动态特征)),
            static_cast<std::uint64_t>(开始时间),
            static_cast<std::uint64_t>(结束时间),
            0xAC7100D1ULL,
        };
        return static_cast<std::uint64_t>(std::hash<std::string_view>{}(
            std::string_view(reinterpret_cast<const char*>(pack), sizeof(pack))));
    }

动态节点类* 私有_解析动态引用(基础信息类* 基础信息, const 可解析引用<动态节点类>& 引用)
    {
        if (!基础信息) {
            return nullptr;
        }

        if (!引用.主键.empty()) {
            auto* 基础节点 = 基础信息->查找主键(引用.主键);
            auto* 动态节点 = 基础节点 ? static_cast<动态节点类*>(基础节点) : nullptr;
            if (动态节点 && 基础信息->取主信息<动态节点主信息类>(动态节点)) {
                return 动态节点;
            }
        }

        if (引用.指针) {
            const auto* 目标地址 = reinterpret_cast<const 基础信息节点类*>(引用.指针);
            for (auto* 基础节点 : 基础信息->枚举节点_按类型<动态节点主信息类>()) {
                if (基础节点 == 目标地址) {
                    return static_cast<动态节点类*>(基础节点);
                }
            }
        }

        return nullptr;
    }

    状态节点类* 私有_解析状态引用(基础信息类* 基础信息, const 可解析引用<状态节点类>& 引用)
    {
        if (!基础信息) {
            return nullptr;
        }

        if (!引用.主键.empty()) {
            auto* 基础节点 = 基础信息->查找主键(引用.主键);
            auto* 状态节点 = 基础节点 ? static_cast<状态节点类*>(基础节点) : nullptr;
            if (状态节点 && 基础信息->取主信息<状态节点主信息类>(状态节点)) {
                return 状态节点;
            }
        }

        if (引用.指针) {
            const auto* 目标地址 = reinterpret_cast<const 基础信息节点类*>(引用.指针);
            for (auto* 基础节点 : 基础信息->枚举节点_按类型<状态节点主信息类>()) {
                if (基础节点 == 目标地址) {
                    return static_cast<状态节点类*>(基础节点);
                }
            }
        }

        return nullptr;
    }

    void 私有_按时间追加动态索引(
        基础信息类* 基础信息,
        场景节点主信息类* 场景主信息,
        动态节点类* 节点)
    {
        if (!场景主信息 || !节点) return;
        if (!私有_引用已存在(场景主信息->动态索引, 节点)) {
            场景主信息->动态索引.push_back(私有_生成引用(节点));
        }
        std::stable_sort(场景主信息->动态索引.begin(), 场景主信息->动态索引.end(), [基础信息](const auto& 左, const auto& 右) {
            const auto* 左主信息 = (基础信息 && 左.指针) ? 基础信息->取主信息<动态节点主信息类>(左.指针) : nullptr;
            const auto* 右主信息 = (基础信息 && 右.指针) ? 基础信息->取主信息<动态节点主信息类>(右.指针) : nullptr;
            const auto 左时间 = 左主信息 ? 私有_取动态时间(*左主信息) : 0;
            const auto 右时间 = 右主信息 ? 私有_取动态时间(*右主信息) : 0;
            if (左时间 != 右时间) return 左时间 < 右时间;
            return 左.主键 < 右.主键;
        });
    }
}

动态类::动态类(基础信息类* 基础信息) noexcept
    : 基础信息_(基础信息)
{
}

void 动态类::绑定基础信息(基础信息类* 基础信息) noexcept
{
    基础信息_ = 基础信息;
}

动态节点主信息类* 动态类::取动态主信息(动态节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<动态节点主信息类>(节点) : nullptr;
}

const 动态节点主信息类* 动态类::取动态主信息(const 动态节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<动态节点主信息类>(节点) : nullptr;
}

动态节点类* 动态类::创建动态(场景节点类* 场景, 动态节点主信息类* 主信息)
{
    if (!基础信息_ || !主信息) return nullptr;

    私有_补齐动态名称类型(主信息);
    私有_刷新动态业务语义类型(主信息);
    auto* 节点 = static_cast<动态节点类*>(基础信息_->添加子节点(场景, 主信息));
    auto* 场景主信息 = 基础信息_->取主信息<场景节点主信息类>(场景);
    if (节点 && 场景主信息) {
        私有_按时间追加动态索引(基础信息_, 场景主信息, 节点);
    }
    if (节点 && 场景) {
        (void)世界树.二次特征生成().获取动态二次特征(*节点);
        (void)世界树.二次特征生成().尝试生成连续同向聚合动态(场景, 节点);
        (void)世界树.二次特征生成().尝试生成同类事件段聚合动态(场景, 节点);
    }
    return 节点;
}

动态节点类* 动态类::创建动态(
    场景节点类* 场景,
    状态节点类* 初始状态,
    状态节点类* 结果状态,
    时间戳 起,
    时间戳 止)
{
    const auto* 初始状态主信息 = 基础信息_ ? 基础信息_->取主信息<状态节点主信息类>(初始状态) : nullptr;
    const auto* 结果状态主信息 = 基础信息_ ? 基础信息_->取主信息<状态节点主信息类>(结果状态) : nullptr;
    if (!私有_可形成基础动态(初始状态主信息, 结果状态主信息)) {
        return nullptr;
    }

    if (起 == 0) 起 = 初始状态主信息 ? 私有_取状态时间(*初始状态主信息) : 0;
    if (止 == 0) 止 = 结果状态主信息 ? 私有_取状态时间(*结果状态主信息) : 起;
    if (止 != 0 && 起 != 0 && 止 < 起) {
        std::swap(起, 止);
    }

    auto* 主信息 = new 动态节点主信息类{};
    主信息->初始状态 = 私有_生成引用(初始状态);
    主信息->结果状态 = 私有_生成引用(结果状态);
    主信息->动态主体 = 结果状态主信息 ? 结果状态主信息->状态主体 : 私有_生成引用(static_cast<基础信息节点类*>(nullptr));
    主信息->动态特征 = 结果状态主信息 ? 结果状态主信息->状态特征 : 私有_生成引用(static_cast<特征节点类*>(nullptr));
    主信息->时间 = 结构_时间段{ 起, 止 };
    主信息->动态路径签名 = 私有_计算动态路径签名(
        初始状态主信息,
        结果状态主信息,
        主信息->动态主体.指针,
        主信息->动态特征.指针);
    主信息->动态层级 = 0;
    主信息->聚合方式 = 枚举_动态聚合方式::原子相邻;
    return 创建动态(场景, 主信息);
}

动态节点类* 动态类::创建叶子动态(
    场景节点类* 场景,
    状态节点类* 初始状态,
    状态节点类* 结果状态,
    时间戳 now)
{
    return 创建动态(场景, 初始状态, 结果状态, 0, now);
}

动态节点类* 动态类::创建动作动态(
    场景节点类* 场景,
    基础信息节点类* 动态主体,
    特征节点类* 动态特征,
    时间戳 开始时间,
    时间戳 结束时间)
{
    if (!场景 || !动态主体 || !动态特征) return nullptr;

    if (开始时间 == 0 && 结束时间 == 0) {
        结束时间 = 结构体_时间戳::当前_微秒();
        开始时间 = 结束时间;
    } else if (开始时间 == 0) {
        开始时间 = 结束时间;
    } else if (结束时间 == 0) {
        结束时间 = 开始时间;
    }
    if (结束时间 < 开始时间) {
        std::swap(开始时间, 结束时间);
    }

    auto* 主信息 = new 动态节点主信息类{};
    主信息->动态主体 = 私有_生成引用(动态主体);
    主信息->动态特征 = 私有_生成引用(动态特征);
    主信息->时间 = 结构_时间段{ 开始时间, 结束时间 };
    主信息->动态路径签名 = 私有_计算动作动态路径签名(动态主体, 动态特征, 开始时间, 结束时间);
    return 创建动态(场景, 主信息);
}

bool 动态类::删除动态(动态节点类* 节点)
{
    if (!基础信息_ || !节点) return false;

    auto* 父节点 = static_cast<基础信息节点类*>(节点->父);
    auto* 场景主信息 = 基础信息_->取主信息<场景节点主信息类>(父节点);
    if (场景主信息) {
        场景主信息->动态索引.erase(
            std::remove_if(
                场景主信息->动态索引.begin(),
                场景主信息->动态索引.end(),
                [&](const auto& 项) { return 项.指针 == 节点 || 项.主键 == 节点->获取主键(); }),
            场景主信息->动态索引.end());
    }

    return 基础信息_->删除节点(节点);
}

std::vector<动态节点类*> 动态类::枚举全部动态() const
{
    std::vector<动态节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举节点_按类型<动态节点主信息类>()) {
        out.push_back(static_cast<动态节点类*>(节点));
    }
    return out;
}

std::vector<动态节点类*> 动态类::获取场景动态(const 场景节点类* 场景) const
{
    std::vector<动态节点类*> out;
    const auto* 场景主信息 = 基础信息_ ? 基础信息_->取主信息<场景节点主信息类>(场景) : nullptr;
    if (!场景主信息) return out;

    for (const auto& 项 : 场景主信息->动态索引) {
        if (auto* 动态节点 = 私有_解析动态引用(基础信息_, 项)) {
            out.push_back(动态节点);
        }
    }
    return out;
}

状态节点类* 动态类::查找相邻前状态(场景节点类* 场景, 状态节点类* 新状态) const
{
    if (!基础信息_ || !场景 || !新状态) return nullptr;

    const auto* 新状态主信息 = 基础信息_->取主信息<状态节点主信息类>(新状态);
    if (!新状态主信息 || !新状态主信息->状态主体.有效() || !新状态主信息->状态特征.有效()) {
        return nullptr;
    }

    const auto* 场景主信息 = 基础信息_->取主信息<场景节点主信息类>(场景);
    if (!场景主信息) {
        return nullptr;
    }

    bool 已跳过当前状态 = false;
    for (auto it = 场景主信息->状态索引.rbegin(); it != 场景主信息->状态索引.rend(); ++it) {
        auto* 候选节点 = 私有_解析状态引用(基础信息_, *it);
        const auto* 候选主信息 = 基础信息_->取主信息<状态节点主信息类>(候选节点);
        if (!候选主信息) continue;

        const bool 是当前状态 =
            候选节点 == 新状态
            || (候选节点 && 新状态 && 候选节点->获取主键() == 新状态->获取主键())
            || (私有_引用匹配节点(候选主信息->状态主体, 新状态主信息->状态主体.指针)
                && 私有_引用匹配节点(候选主信息->状态特征, 新状态主信息->状态特征.指针)
                && 候选主信息->发生时间 == 新状态主信息->发生时间
                && 候选主信息->收到时间 == 新状态主信息->收到时间
                && 私有_特征值相等(候选主信息->状态值, 新状态主信息->状态值));
        if (!已跳过当前状态 && 是当前状态) {
            已跳过当前状态 = true;
            continue;
        }

        if (!私有_可形成基础动态(候选主信息, 新状态主信息)) continue;
        return 候选节点;
    }

    return nullptr;
}

动态节点类* 动态类::状态记录后刷新动态(
    场景节点类* 场景,
    状态节点类* 新状态,
    时间戳 now,
    const std::string& 调用点)
{
    (void)调用点;
    auto* 前状态 = 查找相邻前状态(场景, 新状态);
    if (!前状态) return nullptr;
    return 创建叶子动态(场景, 前状态, 新状态, now);
}

bool 动态类::设置业务语义类型(动态节点类* 节点, 枚举_动态业务语义类型 类型)
{
    auto* 主信息 = 取动态主信息(节点);
    if (!主信息) return false;

    主信息->业务语义类型 = 类型;
    return true;
}

bool 动态类::设置主体特征(动态节点类* 节点, 基础信息节点类* 主体, 特征节点类* 特征)
{
    auto* 主信息 = 取动态主信息(节点);
    if (!主信息) return false;

    主信息->动态主体 = 私有_生成引用(主体);
    主信息->动态特征 = 私有_生成引用(特征);
    私有_刷新动态业务语义类型(主信息);
    return true;
}

bool 动态类::设置聚合信息(动态节点类* 节点, std::uint32_t 动态层级, 枚举_动态聚合方式 聚合方式)
{
    auto* 主信息 = 取动态主信息(节点);
    if (!主信息) return false;

    主信息->动态层级 = 动态层级;
    主信息->聚合方式 = 聚合方式;
    return true;
}

bool 动态类::追加来源低层动态(动态节点类* 节点, 动态节点类* 来源低层动态)
{
    auto* 主信息 = 取动态主信息(节点);
    if (!主信息 || !来源低层动态) return false;

    if (!私有_引用已存在(主信息->来源低层动态, 来源低层动态)) {
        主信息->来源低层动态.push_back(私有_生成引用(来源低层动态));
    }
    return true;
}

bool 动态类::绑定来源方法(
    动态节点类* 节点,
    方法节点类* 来源动作,
    const 词性节点类* 来源动作名,
    枚举_动作事件相位 相位)
{
    auto* 主信息 = 取动态主信息(节点);
    if (!主信息) return false;

    主信息->来源类型 = 枚举_动态来源类型::方法调用;
    主信息->来源动作.指针 = 来源动作;
    主信息->来源动作名 = 来源动作名;
    主信息->来源动作相位 = 相位;
    私有_刷新动态业务语义类型(主信息);
    return true;
}

bool 动态类::绑定来源场景(动态节点类* 节点, 场景节点类* 输入场景, 场景节点类* 输出场景)
{
    auto* 主信息 = 取动态主信息(节点);
    if (!主信息) return false;

    主信息->来源输入场景 = 私有_生成引用(输入场景);
    主信息->来源输出场景 = 私有_生成引用(输出场景);
    return true;
}
