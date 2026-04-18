#include "语言类.h"

#include <string>
#include <variant>

#include "世界树类.h"

import 二次特征应用模块;

namespace {
    std::string 私有_安全词(const 词性节点类* 词)
    {
        return 词 ? 词->获取主键() : std::string{};
    }

    std::string 私有_安全文本(const std::string& 文本, const std::string& 默认值)
    {
        return 文本.empty() ? 默认值 : 文本;
    }

    const char* 私有_世界类型文本(枚举_世界类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_世界类型::现实世界: return "现实世界";
        case 枚举_世界类型::文本世界: return "文本世界";
        case 枚举_世界类型::想象世界: return "想象世界";
        case 枚举_世界类型::记忆世界: return "记忆世界";
        case 枚举_世界类型::推理世界: return "推理世界";
        case 枚举_世界类型::内部世界: return "内部世界";
        case 枚举_世界类型::虚拟世界: return "虚拟世界";
        default: return "其它世界";
        }
    }

    std::string 私有_指针句柄文本(std::uintptr_t 指针值)
    {
        if (指针值 == 0) {
            return "空指针";
        }
        return "指针#" + std::to_string(指针值);
    }

    std::string 私有_特征值文本(const 特征值& 值)
    {
        if (std::holds_alternative<I64>(值)) {
            return std::to_string(std::get<I64>(值));
        }
        if (std::holds_alternative<指针句柄>(值)) {
            return 私有_指针句柄文本(std::get<指针句柄>(值).指针);
        }
        if (std::holds_alternative<VecU句柄>(值)) {
            return "VecU#" + std::to_string(std::get<VecU句柄>(值).主信息指针);
        }
        return "未定义";
    }

    std::string 私有_基础节点短名(const 基础信息节点类* 节点)
    {
        if (!节点) {
            return {};
        }
        const auto 名称 = 语言集.获取基础信息名称(节点);
        return 名称.empty() ? std::string("节点#") + 节点->获取主键() : 名称;
    }

    std::string 私有_方法节点名称(const 方法类::节点类* 节点)
    {
        if (!节点) {
            return {};
        }
        return 私有_安全文本(私有_安全词(节点->主信息.动作名), std::string("方法#") + 节点->获取主键());
    }

    std::string 私有_二次特征种类文本(枚举_二次特征种类 种类) noexcept
    {
        switch (种类) {
        case 枚举_二次特征种类::存在比较: return "存在比较";
        case 枚举_二次特征种类::特征比较: return "特征比较";
        case 枚举_二次特征种类::状态比较: return "状态比较";
        case 枚举_二次特征种类::动态比较: return "动态比较";
        case 枚举_二次特征种类::因果强度比较: return "因果强度比较";
        case 枚举_二次特征种类::观察关系: return "观察关系";
        default: return "二次特征";
        }
    }
}

std::string 语言类::获取基础信息名称(const 基础信息节点类* 节点) const
{
    const auto& 基础信息 = 世界树.基础信息();
    if (!节点 || !节点->主信息) return "世界根";

    if (const auto* 指代主信息 = 基础信息.取主信息<指代节点主信息类>(节点)) {
        const auto 代词 = 私有_安全词(指代主信息->代词);
        if (!代词.empty() && 指代主信息->指代对象.指针) {
            return 代词 + "（指向" + 私有_基础节点短名(指代主信息->指代对象.指针) + "）";
        }
        if (!代词.empty()) return 代词;
        if (指代主信息->指代对象.指针) {
            return "指代" + 私有_基础节点短名(指代主信息->指代对象.指针);
        }
        return "未命名指代";
    }

    if (const auto* 场景主信息 = 基础信息.取主信息<场景节点主信息类>(节点)) {
        const auto 名称 = 私有_安全词(场景主信息->名称);
        const auto 世界类型 = std::string(私有_世界类型文本(场景主信息->世界类型));
        if (!名称.empty() && 名称 != 世界类型) {
            return 名称 + "（" + 世界类型 + "）";
        }
        return 名称.empty() ? 世界类型 : 名称;
    }

    if (const auto* 存在主信息 = 基础信息.取主信息<存在节点主信息类>(节点)) {
        const auto 名称 = 私有_安全词(存在主信息->名称);
        const auto 类型 = 私有_安全词(存在主信息->类型);
        if (!名称.empty() && !类型.empty() && 名称 != 类型) {
            return 名称 + "（" + 类型 + "）";
        }
        if (!名称.empty()) return 名称;
        return 类型.empty() ? "未命名存在" : 类型;
    }

    if (const auto* 特征主信息 = 基础信息.取主信息<特征节点主信息类>(节点)) {
        const auto 类型 = 私有_安全文本(私有_安全词(特征主信息->类型), 私有_安全词(特征主信息->名称));
        if (特征值有效(特征主信息->当前值)) {
            return 私有_安全文本(类型, "特征") + "为" + 私有_特征值文本(特征主信息->当前值);
        }
        return 私有_安全文本(类型, "未命名特征");
    }

    if (const auto* 状态主信息 = 基础信息.取主信息<状态节点主信息类>(节点)) {
        std::string 主体短名{};
        if (状态主信息->状态主体.指针) {
            主体短名 = 私有_基础节点短名(状态主信息->状态主体.指针);
        }
        std::string 特征短名{};
        if (const auto* 特征主信息 = 基础信息.取主信息<特征节点主信息类>(状态主信息->状态特征.指针)) {
            特征短名 = 私有_安全文本(私有_安全词(特征主信息->类型), 私有_安全词(特征主信息->名称));
        }

        std::string 值文本{};
        if (std::holds_alternative<指针句柄>(状态主信息->状态值)
            && !状态主信息->变化原因说明.empty()
            && 状态主信息->变化原因说明 != "治理前镜像"
            && 状态主信息->变化原因说明 != "治理后镜像") {
            值文本 = 状态主信息->变化原因说明;
        }
        else {
            值文本 = 私有_特征值文本(状态主信息->状态值);
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
        return "状态为" + 值文本;
    }

    if (const auto* 动态主信息 = 基础信息.取主信息<动态节点主信息类>(节点)) {
        std::string 主体短名{};
        if (动态主信息->动态主体.指针) {
            主体短名 = 私有_基础节点短名(动态主信息->动态主体.指针);
        }

        std::string 动作名{};
        if (const auto* 来源方法 = reinterpret_cast<const 方法类::节点类*>(动态主信息->来源动作.指针)) {
            动作名 = 私有_方法节点名称(来源方法);
        }
        if (动作名.empty()) {
            动作名 = 私有_安全词(动态主信息->来源动作名);
        }
        if (动作名.empty()) {
            动作名 = 私有_安全词(动态主信息->名称);
        }

        if (!主体短名.empty() && !动作名.empty()) {
            return 主体短名 + "执行了" + 动作名;
        }
        if (!动作名.empty()) return 动作名;
        return "动态";
    }

    if (const auto* 二次特征主信息 = 二次特征主信息_桥接(节点)) {
        const auto 自然语言 = 生成二次特征自然语言(static_cast<const 二次特征节点类*>(节点));
        if (!自然语言.empty()) return 自然语言;

        const auto 概念名称 = 私有_安全词(二次特征主信息->概念名称);
        if (!概念名称.empty()) return 概念名称;
        return 私有_二次特征种类文本(二次特征主信息->种类);
    }

    if (const auto* 因果主信息 = 基础信息.取主信息<因果实例主信息类>(节点)) {
        const auto 自然语言 = 世界树.因果().生成实例因果自然语言(static_cast<const 因果实例节点类*>(节点));
        if (!自然语言.empty()) return 自然语言;
        if (!因果主信息->动作语义键.empty()) return 因果主信息->动作语义键;
        return "因果实例";
    }

    if (const auto* 因果模板主信息 = 基础信息.取主信息<因果模板主信息类>(节点)) {
        const auto 名称 = 私有_安全词(因果模板主信息->名称);
        if (名称 == "因果信息") return 名称;

        const auto 自然语言 = 世界树.因果().生成抽象因果自然语言(static_cast<const 因果模板节点类*>(节点));
        if (!自然语言.empty()) return 自然语言;
        if (!名称.empty()) return 名称;
        return "抽象因果";
    }

    const auto 名称 = 私有_安全词(节点->主信息->名称);
    if (!名称.empty()) return 名称;
    const auto 类型 = 私有_安全词(节点->主信息->类型);
    return 类型.empty() ? std::string("节点#") + 节点->获取主键() : 类型;
}
