#include "因果类.h"

#include <algorithm>
#include <limits>
#include <sstream>

#include "自我类.h"
#include "语素类.h"

namespace {
    std::string 私有_状态短语(const 基础信息类* 基础信息, const 可解析引用<状态节点类>& 状态引用);

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

    const 词性节点类* 私有_取动作键词(const std::string& 动作语义键)
    {
        if (动作语义键.empty()) return nullptr;
        return 语素集.添加词性词(动作语义键, "名词");
    }

    const 词性节点类* 私有_取因果信息根名称词()
    {
        return 语素集.添加词性词("因果信息", "名词");
    }

    std::string 私有_安全词(const 词性节点类* 词) noexcept
    {
        if (!词) return {};
        try {
            return 语素集.获取词(词);
        }
        catch (...) {
            return {};
        }
    }

    std::string 私有_安全文本(std::string 文本, const std::string& 回退)
    {
        return 文本.empty() ? 回退 : std::move(文本);
    }

    constexpr I64 私有_百分比值(I64 基准, I64 百分比) noexcept
    {
        return (基准 / 100) * 百分比 + ((基准 % 100) * 百分比) / 100;
    }

    bool 私有_文本包含(const std::string& 文本, const char* 片段) noexcept
    {
        return 片段 && 文本.find(片段) != std::string::npos;
    }

    std::string 私有_基础节点短名(const 基础信息类* 基础信息, const 基础信息节点类* 节点)
    {
        if (!基础信息 || !节点 || !节点->主信息) return {};

        if (const auto* 场景主信息 = 基础信息->取主信息<场景节点主信息类>(节点)) {
            return 私有_安全文本(私有_安全词(场景主信息->名称), "场景");
        }
        if (const auto* 存在主信息 = 基础信息->取主信息<存在节点主信息类>(节点)) {
            auto 名称 = 私有_安全词(存在主信息->名称);
            if (!名称.empty()) return 名称;
            return 私有_安全文本(私有_安全词(存在主信息->类型), "存在");
        }
        if (const auto* 特征主信息 = 基础信息->取主信息<特征节点主信息类>(节点)) {
            return 私有_安全文本(私有_安全词(特征主信息->类型), "特征");
        }
        return 私有_安全文本(私有_安全词(节点->主信息->名称), 节点->获取主键());
    }

    template<class T节点, class T回调>
    void 私有_遍历独立树节点(const T节点* 节点, T回调&& 回调)
    {
        if (!节点) return;
        回调(节点);
        if (!节点->子) return;

        auto* 起点 = static_cast<const T节点*>(节点->子);
        auto* 当前 = 起点;
        do {
            私有_遍历独立树节点(当前, 回调);
            当前 = static_cast<const T节点*>(当前->下);
        } while (当前 && 当前 != 起点);
    }

    std::string 私有_任务节点短名(const 任务类::节点类* 节点)
    {
        if (!节点) return {};
        return 私有_安全文本(私有_安全词(节点->主信息.名称), std::string("任务#") + 节点->获取主键());
    }

    std::string 私有_方法节点短名(const 方法类::节点类* 节点)
    {
        if (!节点) return {};
        return 私有_安全文本(私有_安全词(节点->主信息.动作名), std::string("方法#") + 节点->获取主键());
    }

    std::string 私有_需求节点短名(const 需求类::节点类* 节点)
    {
        if (!节点) return {};
        return 私有_安全文本(私有_安全词(节点->主信息.类型), std::string("需求#") + 节点->获取主键());
    }

    std::string 私有_指针句柄文本(const 基础信息类* 基础信息, std::uintptr_t 指针值)
    {
        if (指针值 == 0) return "ptr#空";
        if (!基础信息) return std::string("ptr#") + std::to_string(指针值);

        for (auto* 节点 : 基础信息->枚举全部节点()) {
            if (reinterpret_cast<std::uintptr_t>(节点) != 指针值) {
                continue;
            }

            auto 节点短名 = 私有_基础节点短名(基础信息, 节点);
            if (!节点短名.empty()) {
                return 节点短名;
            }
            return std::string("节点#") + 节点->获取主键();
        }

        const auto* 自我存在 = 自我.获取自我存在();
        if (自我存在) {
            if (const auto* 需求根节点 = 世界树.存在().获取需求根节点(自我存在)) {
                std::string 命中名称{};
                私有_遍历独立树节点(需求根节点, [&](const 需求类::节点类* 节点) {
                    if (命中名称.empty() && reinterpret_cast<std::uintptr_t>(节点) == 指针值) {
                        命中名称 = 私有_需求节点短名(节点);
                    }
                });
                if (!命中名称.empty()) return 命中名称;
            }

            if (const auto* 任务根节点 = 世界树.存在().获取任务根节点(自我存在)) {
                std::string 命中名称{};
                私有_遍历独立树节点(任务根节点, [&](const 任务类::节点类* 节点) {
                    if (命中名称.empty() && reinterpret_cast<std::uintptr_t>(节点) == 指针值) {
                        命中名称 = 私有_任务节点短名(节点);
                    }
                });
                if (!命中名称.empty()) return 命中名称;
            }

            if (const auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在)) {
                std::string 命中名称{};
                私有_遍历独立树节点(方法根节点, [&](const 方法类::节点类* 节点) {
                    if (命中名称.empty() && reinterpret_cast<std::uintptr_t>(节点) == 指针值) {
                        命中名称 = 私有_方法节点短名(节点);
                    }
                });
                if (!命中名称.empty()) return 命中名称;
            }
        }

        return std::string("ptr#") + std::to_string(指针值);
    }

    std::string 私有_特征值文本(const 基础信息类* 基础信息, const 特征值& 值)
    {
        if (const auto* 标量 = std::get_if<I64>(&值)) {
            return std::to_string(*标量);
        }
        if (const auto* 向量句柄 = std::get_if<VecU句柄>(&值)) {
            return 向量句柄->有效()
                ? std::string("VecU#") + std::to_string(向量句柄->主信息指针)
                : "VecU#空";
        }
        if (const auto* 指针 = std::get_if<指针句柄>(&值)) {
            return 私有_指针句柄文本(基础信息, 指针->指针);
        }
        return "空";
    }

    std::string 私有_状态前缀(const std::string& 主体短名, const std::string& 特征短名)
    {
        if (!主体短名.empty() && !特征短名.empty()) {
            return 主体短名 + "的" + 特征短名;
        }
        if (!特征短名.empty()) return 特征短名;
        if (!主体短名.empty()) return 主体短名 + "状态";
        return "状态";
    }

    const 特征节点主信息类* 私有_取状态特征主信息(const 基础信息类* 基础信息, const 状态节点主信息类* 状态主信息)
    {
        if (!基础信息 || !状态主信息 || !状态主信息->状态特征.指针) return nullptr;
        return 基础信息->取主信息<特征节点主信息类>(状态主信息->状态特征.指针);
    }

    std::string 私有_取状态特征短名(const 基础信息类* 基础信息, const 状态节点主信息类* 状态主信息)
    {
        const auto* 特征主信息 = 私有_取状态特征主信息(基础信息, 状态主信息);
        if (!特征主信息) return {};
        return 私有_安全文本(私有_安全词(特征主信息->类型), 私有_安全词(特征主信息->名称));
    }

    std::string 私有_取状态主体短名(const 基础信息类* 基础信息, const 状态节点主信息类* 状态主信息)
    {
        if (!基础信息 || !状态主信息) return {};
        return 私有_基础节点短名(基础信息, 状态主信息->状态主体.指针);
    }

    bool 私有_是自我服务值特征(const 词性节点类* 特征类型) noexcept
    {
        return 私有_词性相同(特征类型, 自我特征定义类::类型_自我_服务值());
    }

    bool 私有_是自我安全类特征(const 词性节点类* 特征类型) noexcept
    {
        return 私有_词性相同(特征类型, 自我特征定义类::类型_自我_安全值())
            || 私有_词性相同(特征类型, 自我特征定义类::类型_自我_风险安全())
            || 私有_词性相同(特征类型, 自我特征定义类::类型_自我_物理安全());
    }

    bool 私有_是数量型特征名称(const std::string& 特征短名) noexcept
    {
        return 私有_文本包含(特征短名, "步长")
            || 私有_文本包含(特征短名, "数量")
            || 私有_文本包含(特征短名, "强度")
            || 私有_文本包含(特征短名, "权重");
    }

    const 状态节点主信息类* 私有_查找对应条件状态(
        const 基础信息类* 基础信息,
        const 因果实例主信息类* 实例主信息,
        const 状态节点主信息类* 结果主信息)
    {
        if (!基础信息 || !实例主信息 || !结果主信息) return nullptr;

        for (const auto& 条件引用 : 实例主信息->条件状态) {
            const auto* 条件主信息 = 基础信息->取主信息<状态节点主信息类>(条件引用.指针);
            if (!条件主信息) continue;
            if (条件主信息->状态主体.指针 != 结果主信息->状态主体.指针) continue;
            if (条件主信息->状态特征.指针 != 结果主信息->状态特征.指针) continue;
            return 条件主信息;
        }
        return nullptr;
    }

    std::string 私有_抽象数值条件短语(
        const 基础信息类* 基础信息,
        const 状态节点主信息类* 状态主信息,
        I64 值)
    {
        const auto 主体短名 = 私有_取状态主体短名(基础信息, 状态主信息);
        const auto 特征短名 = 私有_取状态特征短名(基础信息, 状态主信息);
        const auto 前缀 = 私有_状态前缀(主体短名, 特征短名);
        const auto* 特征主信息 = 私有_取状态特征主信息(基础信息, 状态主信息);
        const auto* 特征类型 = 特征主信息 ? 特征主信息->类型 : nullptr;

        constexpr I64 上限 = (std::numeric_limits<I64>::max)();
        const I64 低位阈值 = 私有_百分比值(上限, 50);
        const I64 高位阈值 = 私有_百分比值(上限, 80);

        if (私有_是自我安全类特征(特征类型)) {
            if (值 < 低位阈值) return 前缀 + "处于低位";
            if (值 > 高位阈值) return 前缀 + "处于高位";
            return 前缀 + "处于稳定带";
        }

        if (私有_是自我服务值特征(特征类型)) {
            if (值 < 低位阈值) return 前缀 + "处于低位";
            if (值 > 高位阈值) return 前缀 + "处于高位";
            return 前缀 + "处于中位区";
        }

        return 前缀 + "处于某一数值状态";
    }

    std::string 私有_从动作语义推断方向(
        const 因果实例主信息类* 实例主信息,
        const std::string& 特征短名,
        bool 数量型)
    {
        const auto 动作语义键 = 实例主信息 ? 实例主信息->动作语义键 : std::string{};

        if (私有_文本包含(动作语义键, "回升")) return "回升";
        if (私有_文本包含(动作语义键, "回落")) return "回落";
        if (私有_文本包含(动作语义键, "归零")) return "归零";
        if (私有_文本包含(动作语义键, "切换")) return "发生切换";
        if (私有_文本包含(动作语义键, "迁移")) return "发生迁移";
        if (私有_文本包含(动作语义键, "推进")) return "发生推进";
        if (私有_文本包含(动作语义键, "挂点")) return "发生切换";
        if (私有_文本包含(动作语义键, "衰减")) return 数量型 ? "减小" : "下降";
        if (私有_文本包含(动作语义键, "减少")) return 数量型 ? "减小" : "下降";
        if (私有_文本包含(动作语义键, "下降")) return 数量型 ? "减小" : "下降";
        if (私有_文本包含(动作语义键, "增加")) return 数量型 ? "增大" : "上升";
        if (私有_文本包含(动作语义键, "上升")) return 数量型 ? "增大" : "上升";

        if (私有_文本包含(特征短名, "指针")) return "发生切换";
        return "发生变化";
    }

    std::string 私有_抽象结果短语(
        const 基础信息类* 基础信息,
        const 因果实例主信息类* 实例主信息,
        const 可解析引用<状态节点类>& 结果引用)
    {
        if (!基础信息 || !结果引用.指针) return {};

        const auto* 结果主信息 = 基础信息->取主信息<状态节点主信息类>(结果引用.指针);
        if (!结果主信息) return {};

        const auto 主体短名 = 私有_取状态主体短名(基础信息, 结果主信息);
        const auto 特征短名 = 私有_取状态特征短名(基础信息, 结果主信息);
        const auto 前缀 = 私有_状态前缀(主体短名, 特征短名);
        const auto* 条件主信息 = 私有_查找对应条件状态(基础信息, 实例主信息, 结果主信息);

        if (const auto* 新值 = std::get_if<I64>(&结果主信息->状态值)) {
            if (条件主信息) {
                if (const auto* 旧值 = std::get_if<I64>(&条件主信息->状态值)) {
                    if (*新值 > *旧值) {
                        if (私有_是自我安全类特征(私有_取状态特征主信息(基础信息, 结果主信息)
                            ? 私有_取状态特征主信息(基础信息, 结果主信息)->类型
                            : nullptr) && *旧值 < 私有_百分比值((std::numeric_limits<I64>::max)(), 50)) {
                            return 前缀 + "回升";
                        }
                        return 前缀 + (私有_是数量型特征名称(特征短名) ? "增大" : "上升");
                    }
                    if (*新值 < *旧值) {
                        if (私有_是自我安全类特征(私有_取状态特征主信息(基础信息, 结果主信息)
                            ? 私有_取状态特征主信息(基础信息, 结果主信息)->类型
                            : nullptr) && *旧值 > 私有_百分比值((std::numeric_limits<I64>::max)(), 80)) {
                            return 前缀 + "回落";
                        }
                        return 前缀 + (私有_是数量型特征名称(特征短名) ? "减小" : "下降");
                    }
                    return 前缀 + "保持稳定";
                }
            }

            return 前缀 + 私有_从动作语义推断方向(
                实例主信息,
                特征短名,
                私有_是数量型特征名称(特征短名));
        }

        if (std::holds_alternative<指针句柄>(结果主信息->状态值)) {
            return 前缀 + "发生切换";
        }

        return 前缀 + "发生变化";
    }

    std::string 私有_抽象条件短语(
        const 基础信息类* 基础信息,
        const 可解析引用<状态节点类>& 条件引用)
    {
        if (!基础信息 || !条件引用.指针) return {};

        const auto* 状态主信息 = 基础信息->取主信息<状态节点主信息类>(条件引用.指针);
        if (!状态主信息) return {};

        if (const auto* 值 = std::get_if<I64>(&状态主信息->状态值)) {
            return 私有_抽象数值条件短语(基础信息, 状态主信息, *值);
        }
        return 私有_状态短语(基础信息, 条件引用);
    }

    std::string 私有_状态短语(const 基础信息类* 基础信息, const 可解析引用<状态节点类>& 状态引用)
    {
        if (!基础信息) return {};
        if (!状态引用.指针) {
            return 状态引用.主键.empty() ? std::string{} : std::string("状态#") + 状态引用.主键;
        }

        const auto* 状态主信息 = 基础信息->取主信息<状态节点主信息类>(状态引用.指针);
        if (!状态主信息) return std::string("状态#") + 状态引用.指针->获取主键();

        std::string 主体短名 = 私有_基础节点短名(基础信息, 状态主信息->状态主体.指针);
        std::string 特征短名{};
        if (const auto* 特征主信息 = 基础信息->取主信息<特征节点主信息类>(状态主信息->状态特征.指针)) {
            特征短名 = 私有_安全词(特征主信息->类型);
        }
        std::string 值文本{};
        if (std::holds_alternative<指针句柄>(状态主信息->状态值)
            && !状态主信息->变化原因说明.empty()
            && 状态主信息->变化原因说明 != "治理前镜像"
            && 状态主信息->变化原因说明 != "治理后镜像") {
            值文本 = 状态主信息->变化原因说明;
        }
        else {
            值文本 = 私有_特征值文本(基础信息, 状态主信息->状态值);
        }

        if (!主体短名.empty() && !特征短名.empty()) {
            return 主体短名 + "的" + 特征短名 + "为" + 值文本;
        }
        if (!特征短名.empty()) {
            return 特征短名 + "为" + 值文本;
        }
        if (!主体短名.empty()) {
            return 主体短名 + "状态为" + 值文本;
        }
        return std::string("状态为") + 值文本;
    }

    std::string 私有_连接短语(const std::vector<std::string>& 短语列表, const char* 分隔符)
    {
        std::ostringstream 输出;
        bool 首项 = true;
        for (const auto& 短语 : 短语列表) {
            if (短语.empty()) continue;
            if (!首项) 输出 << 分隔符;
            输出 << 短语;
            首项 = false;
        }
        return 输出.str();
    }

    std::string 私有_动作短语(const 基础信息类* 基础信息, const 因果实例主信息类& 主信息)
    {
        if (!主信息.动作语义键.empty()) {
            return 主信息.动作语义键;
        }
        auto 动作短名 = 私有_基础节点短名(基础信息, 主信息.动作语义.指针);
        if (!动作短名.empty()) return 动作短名;
        动作短名 = 私有_基础节点短名(基础信息, 主信息.动作主体.指针);
        return 动作短名.empty() ? std::string("该动作") : 动作短名;
    }

    const 词性节点类* 私有_取主结果特征类型(基础信息类* 基础信息, const 因果实例主信息类* 实例主信息)
    {
        if (!实例主信息) return nullptr;
        if (实例主信息->类型) return 实例主信息->类型;
        if (!基础信息 || !实例主信息->主结果状态.指针) return nullptr;

        const auto* 状态主信息 = 基础信息->取主信息<状态节点主信息类>(实例主信息->主结果状态.指针);
        if (!状态主信息 || !状态主信息->状态特征.指针) return nullptr;

        const auto* 特征主信息 = 基础信息->取主信息<特征节点主信息类>(状态主信息->状态特征.指针);
        return 特征主信息 ? 特征主信息->类型 : nullptr;
    }

    基础信息节点类* 私有_取动作模板节点(const 因果实例主信息类* 实例主信息)
    {
        if (!实例主信息) return nullptr;
        if (实例主信息->动作语义.指针) return 实例主信息->动作语义.指针;
        return 实例主信息->动作主体.指针;
    }

    时间戳 私有_取模板命中时间(const 因果实例主信息类* 实例主信息)
    {
        if (!实例主信息) return 0;
        if (实例主信息->结果时间 != 0) return 实例主信息->结果时间;
        return 实例主信息->动作时间;
    }

    bool 私有_是抽象因果信息根节点(const 基础信息类* 基础信息, const 因果模板节点类* 节点)
    {
        if (!基础信息 || !节点) return false;

        const auto* 主信息 = 基础信息->取主信息<因果模板主信息类>(节点);
        if (!主信息) return false;
        if (!私有_词性相同(主信息->名称, 私有_取因果信息根名称词())) return false;
        return 主信息->证据实例.empty() && 主信息->结果模板.empty();
    }

    因果模板节点类* 私有_取抽象因果信息根节点(基础信息类* 基础信息)
    {
        if (!基础信息) return nullptr;

        for (auto* 节点 : 基础信息->枚举子节点_按类型<因果模板主信息类>(nullptr)) {
            auto* 因果节点 = static_cast<因果模板节点类*>(节点);
            if (私有_是抽象因果信息根节点(基础信息, 因果节点)) {
                return 因果节点;
            }
        }
        return nullptr;
    }

    因果模板节点类* 私有_取或创建抽象因果信息根节点(基础信息类* 基础信息)
    {
        if (!基础信息) return nullptr;
        if (auto* 已有 = 私有_取抽象因果信息根节点(基础信息)) {
            return 已有;
        }

        auto* 新主信息 = new 因果模板主信息类();
        新主信息->名称 = 私有_取因果信息根名称词();
        新主信息->类型 = 私有_取因果信息根名称词();
        return static_cast<因果模板节点类*>(基础信息->添加子节点(nullptr, 新主信息));
    }

    因果模板节点类* 私有_查找抽象因果模板(
        基础信息类* 基础信息,
        const 词性节点类* 动作键词,
        const 词性节点类* 结果特征类型)
    {
        if (!基础信息 || !动作键词 || !结果特征类型) return nullptr;

        auto* 因果信息根 = 私有_取抽象因果信息根节点(基础信息);
        if (!因果信息根) return nullptr;

        for (auto* 节点 : 基础信息->枚举子节点_按类型<因果模板主信息类>(因果信息根)) {
            auto* 模板节点 = static_cast<因果模板节点类*>(节点);
            auto* 模板主信息 = 基础信息->取主信息<因果模板主信息类>(模板节点);
            if (!模板主信息) continue;
            if (模板主信息->名称 != 动作键词) continue;
            if (模板主信息->类型 != 结果特征类型) continue;
            return 模板节点;
        }

        return nullptr;
    }

    因果模板节点类* 私有_取或创建抽象因果模板(
        基础信息类* 基础信息,
        const 因果实例主信息类* 实例主信息)
    {
        if (!基础信息 || !实例主信息) return nullptr;

        const auto* 动作键词 = 私有_取动作键词(实例主信息->动作语义键);
        const auto* 结果特征类型 = 私有_取主结果特征类型(基础信息, 实例主信息);
        if (!动作键词 || !结果特征类型) return nullptr;

        if (auto* 已有模板 = 私有_查找抽象因果模板(
            基础信息,
            动作键词,
            结果特征类型)) {
            return 已有模板;
        }

        auto* 新主信息 = new 因果模板主信息类();
        新主信息->名称 = 动作键词;
        新主信息->类型 = 结果特征类型;
        新主信息->动作模板 = 私有_生成引用(私有_取动作模板节点(实例主信息));
        auto* 因果信息根 = 私有_取或创建抽象因果信息根节点(基础信息);
        return 因果信息根
            ? static_cast<因果模板节点类*>(基础信息->添加子节点(因果信息根, 新主信息))
            : nullptr;
    }

    void 私有_同步抽象因果信息(基础信息类* 基础信息, 因果实例节点类* 实例节点)
    {
        if (!基础信息 || !实例节点) return;

        auto* 实例主信息 = 基础信息->取主信息<因果实例主信息类>(实例节点);
        if (!实例主信息) return;
        if (实例主信息->动作语义键.empty()) return;
        if (!实例主信息->主结果状态.指针) return;

        auto* 模板节点 = 私有_取或创建抽象因果模板(基础信息, 实例主信息);
        auto* 模板主信息 = 模板节点 ? 基础信息->取主信息<因果模板主信息类>(模板节点) : nullptr;
        if (!模板主信息) return;

        if (!模板主信息->动作模板.指针) {
            模板主信息->动作模板 = 私有_生成引用(私有_取动作模板节点(实例主信息));
        }

        const bool 已有证据 = 私有_引用已存在(模板主信息->证据实例, 实例节点);
        if (!已有证据) {
            模板主信息->证据实例.push_back(私有_生成引用(实例节点));
            ++模板主信息->成立次数;
        }

        const auto 命中时间 = 私有_取模板命中时间(实例主信息);
        if (命中时间 > 模板主信息->最近命中时间) {
            模板主信息->最近命中时间 = 命中时间;
        }

        模板主信息->稳定度 = static_cast<std::int64_t>(模板主信息->成立次数)
            - static_cast<std::int64_t>(模板主信息->失败次数);
    }
}

因果类::因果类(基础信息类* 基础信息) noexcept
    : 基础信息_(基础信息)
{
}

void 因果类::绑定基础信息(基础信息类* 基础信息) noexcept
{
    基础信息_ = 基础信息;
}

因果实例主信息类* 因果类::取实例主信息(因果实例节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<因果实例主信息类>(节点) : nullptr;
}

const 因果实例主信息类* 因果类::取实例主信息(const 因果实例节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<因果实例主信息类>(节点) : nullptr;
}

因果模板主信息类* 因果类::取模板主信息(因果模板节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<因果模板主信息类>(节点) : nullptr;
}

const 因果模板主信息类* 因果类::取模板主信息(const 因果模板节点类* 节点) const noexcept
{
    return 基础信息_ ? 基础信息_->取主信息<因果模板主信息类>(节点) : nullptr;
}

因果实例节点类* 因果类::创建因果实例(场景节点类* 场景, 因果实例主信息类* 主信息)
{
    if (!基础信息_ || !主信息) return nullptr;

    auto* 节点 = static_cast<因果实例节点类*>(基础信息_->添加子节点(场景, 主信息));
    auto* 场景主信息 = 基础信息_->取主信息<场景节点主信息类>(场景);
    if (节点 && 场景主信息 && !私有_引用已存在(场景主信息->实例因果索引, 节点)) {
        场景主信息->实例因果索引.push_back(私有_生成引用(节点));
    }
    return 节点;
}

因果模板节点类* 因果类::创建因果模板(基础信息节点类* 父节点, 因果模板主信息类* 主信息)
{
    if (!基础信息_ || !主信息) return nullptr;
    return static_cast<因果模板节点类*>(基础信息_->添加子节点(父节点, 主信息));
}

bool 因果类::删除因果(因果节点类* 节点)
{
    if (!基础信息_ || !节点) return false;

    auto* 父节点 = static_cast<基础信息节点类*>(节点->父);
    auto* 场景主信息 = 基础信息_->取主信息<场景节点主信息类>(父节点);
    if (场景主信息) {
        场景主信息->实例因果索引.erase(
            std::remove_if(
                场景主信息->实例因果索引.begin(),
                场景主信息->实例因果索引.end(),
                [&](const auto& 项) { return 项.指针 == 节点 || 项.主键 == 节点->获取主键(); }),
            场景主信息->实例因果索引.end());
    }

    return 基础信息_->删除节点(节点);
}

std::vector<因果节点类*> 因果类::枚举全部因果() const
{
    std::vector<因果节点类*> out;
    for (auto* 节点 : 枚举因果实例()) out.push_back(节点);
    for (auto* 节点 : 枚举因果模板()) out.push_back(节点);
    return out;
}

std::vector<因果实例节点类*> 因果类::枚举因果实例() const
{
    std::vector<因果实例节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举节点_按类型<因果实例主信息类>()) {
        out.push_back(static_cast<因果实例节点类*>(节点));
    }
    return out;
}

std::vector<因果模板节点类*> 因果类::枚举因果模板() const
{
    std::vector<因果模板节点类*> out;
    if (!基础信息_) return out;

    for (auto* 节点 : 基础信息_->枚举节点_按类型<因果模板主信息类>()) {
        auto* 模板节点 = static_cast<因果模板节点类*>(节点);
        if (私有_是抽象因果信息根节点(基础信息_, 模板节点)) {
            continue;
        }
        out.push_back(模板节点);
    }
    return out;
}

std::vector<因果实例节点类*> 因果类::获取场景因果实例(const 场景节点类* 场景) const
{
    std::vector<因果实例节点类*> out;
    const auto* 场景主信息 = 基础信息_ ? 基础信息_->取主信息<场景节点主信息类>(场景) : nullptr;
    if (!场景主信息) return out;

    for (const auto& 项 : 场景主信息->实例因果索引) {
        if (项.指针) out.push_back(项.指针);
    }
    return out;
}

bool 因果类::追加条件状态(因果实例节点类* 节点, 状态节点类* 条件状态)
{
    auto* 主信息 = 取实例主信息(节点);
    if (!主信息 || !条件状态) return false;
    if (!私有_引用已存在(主信息->条件状态, 条件状态)) {
        主信息->条件状态.push_back(私有_生成引用(条件状态));
    }
    return true;
}

bool 因果类::追加结果状态(因果实例节点类* 节点, 状态节点类* 结果状态, bool 设为主结果)
{
    auto* 主信息 = 取实例主信息(节点);
    if (!主信息 || !结果状态) return false;
    if (!私有_引用已存在(主信息->结果状态, 结果状态)) {
        主信息->结果状态.push_back(私有_生成引用(结果状态));
    }
    if (设为主结果) {
        主信息->主结果状态 = 私有_生成引用(结果状态);
        私有_同步抽象因果信息(基础信息_, 节点);
    }
    return true;
}

bool 因果类::绑定动作主体语义(
    因果实例节点类* 节点,
    基础信息节点类* 动作主体,
    基础信息节点类* 动作语义,
    const std::string& 动作语义键)
{
    auto* 主信息 = 取实例主信息(节点);
    if (!主信息) return false;

    主信息->动作主体 = 私有_生成引用(动作主体);
    主信息->动作语义 = 私有_生成引用(动作语义);
    主信息->动作语义键 = 动作语义键;
    return true;
}

bool 因果类::追加证据实例(因果模板节点类* 节点, 因果实例节点类* 证据实例)
{
    auto* 主信息 = 取模板主信息(节点);
    if (!主信息 || !证据实例) return false;
    if (!私有_引用已存在(主信息->证据实例, 证据实例)) {
        主信息->证据实例.push_back(私有_生成引用(证据实例));
    }
    return true;
}

bool 因果类::追加条件模板(因果模板节点类* 节点, 二次特征节点类* 条件模板)
{
    auto* 主信息 = 取模板主信息(节点);
    if (!主信息 || !条件模板) return false;
    if (!私有_引用已存在(主信息->条件模板, 条件模板)) {
        主信息->条件模板.push_back(私有_生成引用(条件模板));
    }
    return true;
}

bool 因果类::追加结果模板(因果模板节点类* 节点, 二次特征节点类* 结果模板)
{
    auto* 主信息 = 取模板主信息(节点);
    if (!主信息 || !结果模板) return false;
    if (!私有_引用已存在(主信息->结果模板, 结果模板)) {
        主信息->结果模板.push_back(私有_生成引用(结果模板));
    }
    return true;
}

bool 因果类::生成实例因果表达单元(const 因果实例节点类* 节点, 结构_因果表达单元& 输出) const
{
    输出 = {};
    const auto* 主信息 = 取实例主信息(节点);
    if (!基础信息_ || !主信息) return false;

    输出.是抽象因果 = false;
    输出.锚点名称 = 私有_基础节点短名(基础信息_, 主信息->锚点信息.指针);
    输出.动作短语 = 私有_动作短语(基础信息_, *主信息);

    for (const auto& 条件状态 : 主信息->条件状态) {
        auto 条件短语 = 私有_状态短语(基础信息_, 条件状态);
        if (!条件短语.empty()) {
            输出.条件短语.push_back(std::move(条件短语));
        }
    }

    for (const auto& 结果状态 : 主信息->结果状态) {
        auto 结果短语 = 私有_状态短语(基础信息_, 结果状态);
        if (!结果短语.empty()) {
            输出.结果短语.push_back(std::move(结果短语));
        }
    }

    输出.主结果短语 = 私有_状态短语(基础信息_, 主信息->主结果状态);
    if (输出.主结果短语.empty() && !输出.结果短语.empty()) {
        输出.主结果短语 = 输出.结果短语.front();
    }

    return !输出.动作短语.empty()
        || !输出.主结果短语.empty()
        || !输出.条件短语.empty();
}

bool 因果类::生成抽象因果表达单元(const 因果模板节点类* 节点, 结构_因果表达单元& 输出) const
{
    输出 = {};
    if (!基础信息_ || !节点 || 私有_是抽象因果信息根节点(基础信息_, 节点)) return false;
    const auto* 主信息 = 取模板主信息(节点);
    if (!主信息) return false;

    输出.是抽象因果 = true;
    输出.成立次数 = 主信息->成立次数;
    输出.失败次数 = 主信息->失败次数;

    const 因果实例节点类* 证据节点 = nullptr;
    for (auto it = 主信息->证据实例.rbegin(); it != 主信息->证据实例.rend(); ++it) {
        if (it->指针) {
            证据节点 = it->指针;
            break;
        }
    }

    if (证据节点 && 生成实例因果表达单元(证据节点, 输出)) {
        输出 = {};
        输出.是抽象因果 = true;
        输出.成立次数 = 主信息->成立次数;
        输出.失败次数 = 主信息->失败次数;

        const auto* 实例主信息 = 取实例主信息(证据节点);
        if (实例主信息) {
            输出.动作短语 = 私有_动作短语(基础信息_, *实例主信息);
            for (const auto& 条件状态 : 实例主信息->条件状态) {
                auto 条件短语 = 私有_抽象条件短语(基础信息_, 条件状态);
                if (!条件短语.empty()) {
                    输出.条件短语.push_back(std::move(条件短语));
                }
            }
            for (const auto& 结果状态 : 实例主信息->结果状态) {
                auto 结果短语 = 私有_抽象结果短语(基础信息_, 实例主信息, 结果状态);
                if (!结果短语.empty()) {
                    输出.结果短语.push_back(std::move(结果短语));
                }
            }
            输出.主结果短语 = 私有_抽象结果短语(基础信息_, 实例主信息, 实例主信息->主结果状态);
            if (输出.主结果短语.empty() && !输出.结果短语.empty()) {
                输出.主结果短语 = 输出.结果短语.front();
            }
            return !输出.动作短语.empty()
                || !输出.主结果短语.empty()
                || !输出.条件短语.empty();
        }
    }

    输出.动作短语 = 私有_安全文本(私有_安全词(主信息->名称), "该动作");
    const auto 结果类型 = 私有_安全词(主信息->类型);
    if (!结果类型.empty()) {
        输出.主结果短语 = 结果类型 + "沿某个方向发生变化";
    }

    return !输出.动作短语.empty() || !输出.主结果短语.empty();
}

std::string 因果类::生成实例因果自然语言(const 因果实例节点类* 节点) const
{
    结构_因果表达单元 表达单元{};
    if (!生成实例因果表达单元(节点, 表达单元)) return {};

    const auto 条件文本 = 私有_连接短语(表达单元.条件短语, "，且");
    const auto 结果文本 = !表达单元.主结果短语.empty()
        ? 表达单元.主结果短语
        : 私有_连接短语(表达单元.结果短语, "，并且");

    if (!条件文本.empty() && !表达单元.动作短语.empty() && !结果文本.empty()) {
        return "因为" + 条件文本 + "，所以" + 表达单元.动作短语 + "产生了" + 结果文本;
    }
    if (!条件文本.empty() && !结果文本.empty()) {
        return "在" + 条件文本 + "下，结果变为" + 结果文本;
    }
    if (!表达单元.动作短语.empty() && !结果文本.empty()) {
        return 表达单元.动作短语 + "使" + 结果文本 + "成立";
    }
    if (!表达单元.动作短语.empty()) {
        return 表达单元.动作短语;
    }
    return 结果文本;
}

std::string 因果类::生成抽象因果自然语言(const 因果模板节点类* 节点) const
{
    结构_因果表达单元 表达单元{};
    if (!生成抽象因果表达单元(节点, 表达单元)) return {};

    const auto 条件文本 = 私有_连接短语(表达单元.条件短语, "，且");
    const auto 结果文本 = !表达单元.主结果短语.empty()
        ? 表达单元.主结果短语
        : 私有_连接短语(表达单元.结果短语, "，并且");

    std::ostringstream 输出;
    if (!条件文本.empty() && !表达单元.动作短语.empty() && !结果文本.empty()) {
        输出 << "通常在" << 条件文本 << "时，" << 表达单元.动作短语 << "会导致" << 结果文本;
    }
    else if (!表达单元.动作短语.empty() && !结果文本.empty()) {
        输出 << "通常" << 表达单元.动作短语 << "会使" << 结果文本 << "成立";
    }
    else if (!结果文本.empty()) {
        输出 << "这类情况下通常会出现" << 结果文本;
    }
    else {
        输出 << "这是一条抽象因果信息";
    }

    输出 << "（成立 " << 表达单元.成立次数 << " 次";
    if (表达单元.失败次数 > 0) {
        输出 << "，失败 " << 表达单元.失败次数 << " 次";
    }
    输出 << "）";
    return 输出.str();
}
