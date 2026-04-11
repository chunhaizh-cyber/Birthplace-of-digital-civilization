#include "语言类.h"

#include <algorithm>
#include <vector>

#include "世界树类.h"

namespace {
    void 私有_追加去重项(std::vector<std::string>* 输出, const std::string& 项, std::size_t 上限)
    {
        if (!输出 || 项.empty()) {
            return;
        }
        if (std::find(输出->begin(), 输出->end(), 项) != 输出->end()) {
            return;
        }
        if (上限 > 0 && 输出->size() >= 上限) {
            return;
        }
        输出->push_back(项);
    }

    std::string 私有_连接短语(const std::vector<std::string>& 短语列表, const char* 分隔符 = "、")
    {
        std::string 输出{};
        for (const auto& 短语 : 短语列表) {
            if (短语.empty()) {
                continue;
            }
            if (!输出.empty()) {
                输出 += 分隔符 ? 分隔符 : "";
            }
            输出 += 短语;
        }
        return 输出;
    }

    std::string 私有_安全词(const 词性节点类* 词)
    {
        return 词 ? 词->获取主键() : std::string{};
    }

    std::string 私有_存在称呼(const 语言类& 语言, const 存在节点类* 节点, const 存在节点主信息类* 主信息)
    {
        const auto 节点名称 = 语言.获取基础信息名称(节点);
        if (!节点名称.empty()) {
            return 节点名称;
        }
        const auto 名称 = 私有_安全词(主信息 ? 主信息->名称 : nullptr);
        if (!名称.empty()) {
            return 名称;
        }
        const auto 类型 = 私有_安全词(主信息 ? 主信息->类型 : nullptr);
        return 类型.empty() ? std::string("该存在") : 类型;
    }

    void 私有_收集树根子项名称(
        const 语言类& 语言,
        const 基础信息节点类* 树根节点,
        std::size_t 上限,
        std::vector<std::string>* 输出)
    {
        if (!树根节点 || !输出) {
            return;
        }

        const auto 直接子项 = 世界树.基础信息().枚举子节点(树根节点);
        if (直接子项.empty()) {
            私有_追加去重项(输出, 语言.获取基础信息名称(树根节点), 上限);
            return;
        }

        for (const auto* 子节点 : 直接子项) {
            私有_追加去重项(输出, 语言.获取基础信息名称(子节点), 上限);
        }
    }
}

std::string 语言类::生成存在有什么表达(const 存在节点类* 节点, std::size_t 上限) const
{
    const auto* 主信息 = 节点 ? dynamic_cast<const 存在节点主信息类*>(节点->主信息) : nullptr;
    if (!主信息) return "该存在当前没有可描述的拥有项";

    const auto 称呼 = 私有_存在称呼(*this, 节点, 主信息);
    std::vector<std::string> 项目{};

    if (主信息->内部世界.指针) {
        const auto 内部世界名 = 获取基础信息名称(主信息->内部世界.指针);
        私有_追加去重项(&项目, 内部世界名.empty() ? "内部世界" : "内部世界" + 内部世界名, 上限);
    }

    for (const auto* 子存在 : 世界树.存在().获取子存在(节点)) {
        私有_追加去重项(&项目, 获取基础信息名称(子存在), 上限);
    }

    for (const auto* 子特征 : 世界树.存在().获取子特征(节点)) {
        私有_追加去重项(&项目, 获取基础信息名称(子特征), 上限);
    }

    if (世界树.存在().获取需求根节点(节点)) {
        私有_追加去重项(&项目, "需求树", 上限);
    }
    if (世界树.存在().获取任务根节点(节点)) {
        私有_追加去重项(&项目, "任务树", 上限);
    }
    if (世界树.存在().获取方法根节点(节点)) {
        私有_追加去重项(&项目, "方法树", 上限);
    }

    if (项目.empty()) {
        return 称呼 + "当前没有明确可列出的内容";
    }
    return 称呼 + "有" + 私有_连接短语(项目);
}

std::string 语言类::生成存在是什么表达(const 存在节点类* 节点) const
{
    const auto* 主信息 = 节点 ? dynamic_cast<const 存在节点主信息类*>(节点->主信息) : nullptr;
    if (!主信息) return "该对象是一个存在";

    const auto 称呼 = 私有_存在称呼(*this, 节点, 主信息);
    const auto 类型 = 私有_安全词(主信息->类型);
    auto 表达 = 称呼 + "是一个" + (类型.empty() ? std::string("存在") : 类型);

    if (主信息->概念模板.指针) {
        const auto 模板名 = 获取基础信息名称(主信息->概念模板.指针);
        if (!模板名.empty()) {
            表达 += "，概念模板为" + 模板名;
        }
    }

    return 表达;
}

std::string 语言类::生成存在能什么表达(const 存在节点类* 节点, std::size_t 上限) const
{
    const auto* 主信息 = 节点 ? dynamic_cast<const 存在节点主信息类*>(节点->主信息) : nullptr;
    if (!主信息) return "该存在当前还没有明确的方法能力";

    const auto 称呼 = 私有_存在称呼(*this, 节点, 主信息);
    std::vector<std::string> 方法项{};
    私有_收集树根子项名称(*this, 世界树.存在().获取方法根节点(节点), 上限, &方法项);

    if (方法项.empty()) {
        return 称呼 + "当前还没有明确的方法能力";
    }
    return 称呼 + "当前具备" + 私有_连接短语(方法项) + "这些方法能力";
}

std::string 语言类::生成存在会什么表达(const 存在节点类* 节点, std::size_t 上限) const
{
    const auto* 主信息 = 节点 ? dynamic_cast<const 存在节点主信息类*>(节点->主信息) : nullptr;
    if (!主信息) return "该存在当前还没有明确的趋向依据";

    std::vector<std::string> 因果短语{};
    for (const auto& 因果引用 : 主信息->实例因果) {
        if (!因果引用.指针) {
            continue;
        }
        私有_追加去重项(&因果短语, 世界树.因果().生成实例因果自然语言(因果引用.指针), 上限);
    }

    if (因果短语.empty()) {
        return 私有_存在称呼(*this, 节点, 主信息) + "当前还没有明确的趋向依据";
    }
    return "从已有因果看，" + 私有_连接短语(因果短语, "；");
}

std::string 语言类::生成存在需要什么表达(const 存在节点类* 节点, std::size_t 上限) const
{
    const auto* 主信息 = 节点 ? dynamic_cast<const 存在节点主信息类*>(节点->主信息) : nullptr;
    if (!主信息) return "该存在当前没有明确需求";

    const auto 称呼 = 私有_存在称呼(*this, 节点, 主信息);
    std::vector<std::string> 需求项{};
    私有_收集树根子项名称(*this, 世界树.存在().获取需求根节点(节点), 上限, &需求项);

    if (需求项.empty()) {
        return 称呼 + "当前没有明确需求";
    }
    return 称呼 + "当前需要" + 私有_连接短语(需求项);
}
