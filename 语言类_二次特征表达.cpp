#include "语言类.h"

#include <sstream>
#include <vector>

#include "世界树类.h"
#include "语素类.h"
#include "方法类.h"

import 二次特征应用模块;

namespace {
    std::string 私有_基础节点短名(const 基础信息类* 基础信息, const 基础信息节点类* 节点);

    std::string 私有_安全词(const 语素入口节点类* 词) noexcept
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

    std::string 私有_世界类型文本(枚举_世界类型 世界类型)
    {
        switch (世界类型) {
        case 枚举_世界类型::现实世界: return "现实世界";
        case 枚举_世界类型::内部世界: return "内部世界";
        case 枚举_世界类型::文本世界: return "文本世界";
        case 枚举_世界类型::想象世界: return "想象世界";
        case 枚举_世界类型::记忆世界: return "记忆世界";
        case 枚举_世界类型::推理世界: return "推理世界";
        case 枚举_世界类型::虚拟世界: return "虚拟世界";
        default: return "场景";
        }
    }

    std::string 私有_二次特征种类文本(枚举_二次特征种类 种类)
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

    std::string 私有_连接短语(const std::vector<std::string>& 短语列表, const char* 分隔符 = "、")
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

    std::string 私有_方法节点名称(const 方法类::节点类* 节点)
    {
        if (!节点) return {};
        return 私有_安全文本(私有_安全词(节点->主信息.首节点信息().动作名), std::string("方法#") + 节点->获取主键());
    }

    struct 结构_二次特征方向模板 {
        std::vector<std::string> 关键词{};
        std::string 正向结果{};
        std::string 反向结果{};
        std::string 中性结果{};
        bool 是方位关系 = false;
    };

    const 结构_二次特征方向模板* 私有_匹配二次特征方向模板(const std::string& 轴键) noexcept
    {
        static const std::vector<结构_二次特征方向模板> 模板表{
            { {"左右", "左侧", "右侧", "重合"}, "在左侧", "在右侧", "重合", true },
            { {"右左", "右侧", "左侧"}, "在右侧", "在左侧", "重合", true },
            { {"前后", "前方", "后方"}, "在前方", "在后方", "重合", true },
            { {"上下", "上方", "下方"}, "在上方", "在下方", "重合", true },
            { {"内外", "内部", "外部"}, "在内部", "在外部", "重合", true },
            { {"远近", "距离", "远离", "接近"}, "更远", "更近", "同距", false },
            { {"高低", "高度", "高", "低"}, "更高", "更低", "相当", false },
            { {"大小", "规模"}, "更大", "更小", "相当", false },
            { {"多少", "数量"}, "更多", "更少", "相当", false },
            { {"强弱", "强度"}, "更强", "更弱", "相当", false },
            { {"快慢", "速度"}, "更快", "更慢", "相当", false },
            { {"长短", "长度"}, "更长", "更短", "相当", false }
        };

        for (const auto& 模板 : 模板表) {
            for (const auto& 关键词 : 模板.关键词) {
                if (!关键词.empty() && 轴键.find(关键词) != std::string::npos) {
                    return &模板;
                }
            }
        }
        return nullptr;
    }

    std::string 私有_二次特征轴键(const 二次特征主信息类* 主信息)
    {
        if (!主信息) return {};
        const auto 概念名称 = 私有_安全词(主信息->概念名称);
        if (!概念名称.empty()) return 概念名称;
        if (!主信息->度量签名_链键.empty()) return 主信息->度量签名_链键;
        return 私有_二次特征种类文本(主信息->种类);
    }

    const 结构_二次特征方向模板* 私有_二次特征方向模板_按主信息(const 二次特征主信息类* 主信息) noexcept
    {
        if (!主信息) return nullptr;
        return 私有_匹配二次特征方向模板(私有_二次特征轴键(主信息));
    }

    std::string 私有_二次特征比较对象名称(const 基础信息类* 基础信息, const 二次特征主信息类* 主信息)
    {
        if (!基础信息 || !主信息) return {};
        if (主信息->右对象.指针) return 私有_基础节点短名(基础信息, 主信息->右对象.指针);
        if (主信息->客体.指针) return 私有_基础节点短名(基础信息, 主信息->客体.指针);
        return {};
    }

    std::string 私有_二次特征主语名称(const 基础信息类* 基础信息, const 二次特征主信息类* 主信息)
    {
        if (!基础信息 || !主信息) return {};
        if (主信息->左对象.指针) return 私有_基础节点短名(基础信息, 主信息->左对象.指针);
        if (主信息->主体.指针) return 私有_基础节点短名(基础信息, 主信息->主体.指针);
        return {};
    }

    std::string 私有_二次特征维度短语(const 二次特征主信息类* 主信息)
    {
        const auto 轴键 = 私有_二次特征轴键(主信息);
        if (轴键.empty()) return {};
        if (轴键 == 私有_二次特征种类文本(主信息->种类)) return {};
        return "在" + 轴键 + "维度上";
    }

    std::string 私有_二次特征粒度短语(const 二次特征主信息类* 主信息)
    {
        if (!主信息) return {};
        switch (主信息->粒度) {
        case 枚举_二次特征粒度::当前场景: return "按当前场景";
        case 枚举_二次特征粒度::相邻分步: return "按相邻分步";
        case 枚举_二次特征粒度::首尾整体: return "按首尾整体";
        case 枚举_二次特征粒度::完整路径: return "按完整路径";
        case 枚举_二次特征粒度::聚合段: return "按聚合段";
        default: return {};
        }
    }

    std::string 私有_二次特征基准短语(const 二次特征主信息类* 主信息)
    {
        if (!主信息) return {};
        switch (主信息->基准类型) {
        case 枚举_二次特征基准类型::相邻前态: return "相对前一状态";
        case 枚举_二次特征基准类型::起始态: return "相对起始态";
        case 枚举_二次特征基准类型::稳态: return "相对稳态";
        case 枚举_二次特征基准类型::目标态: return "相对目标态";
        case 枚举_二次特征基准类型::上边界: return "相对上边界";
        case 枚举_二次特征基准类型::下边界: return "相对下边界";
        default: return {};
        }
    }

    std::string 私有_二次特征时间归一短语(const 二次特征主信息类* 主信息)
    {
        if (!主信息) return {};
        switch (主信息->时间归一方式) {
        case 枚举_时间归一方式::分步时长: return "按分步时长归一";
        case 枚举_时间归一方式::总时长: return "按总时长归一";
        case 枚举_时间归一方式::频率窗口: return "按频率窗口归一";
        default: return {};
        }
    }

    std::string 私有_连接限定短语(const std::vector<std::string>& 短语列表)
    {
        std::ostringstream 输出;
        bool 首项 = true;
        for (const auto& 短语 : 短语列表) {
            if (短语.empty()) continue;
            if (!首项) 输出 << "，";
            输出 << 短语;
            首项 = false;
        }
        return 输出.str();
    }

    std::string 私有_二次特征单体方向结果(std::string 结果)
    {
        if (结果.rfind("更", 0) == 0 && 结果.size() > std::string("更").size()) {
            return "偏" + 结果.substr(std::string("更").size());
        }
        if (结果 == "相当") return "持平";
        if (结果 == "同距") return "距离相当";
        return 结果;
    }

    std::string 私有_二次特征空间关系结果(const std::string& 结果, const std::string& 比较对象)
    {
        if (结果.empty()) return {};
        if (比较对象.empty()) return 结果;
        if (结果 == "重合") {
            return "与" + 比较对象 + "重合";
        }
        if (结果.rfind("在", 0) == 0 && 结果.size() > std::string("在").size()) {
            return "在" + 比较对象 + "的" + 结果.substr(std::string("在").size());
        }
        return "相对于" + 比较对象 + 结果;
    }

    std::string 私有_二次特征比较结果(
        const std::string& 结果,
        const std::string& 比较对象,
        const std::string& 维度短语)
    {
        if (结果.empty()) return {};
        if (!比较对象.empty()) {
            if (结果 == "相当" || 结果 == "同距") {
                return "与" + 比较对象 + 结果;
            }
            return "相对于" + 比较对象 + 结果;
        }

        const auto 单体结果 = 私有_二次特征单体方向结果(结果);
        if (!维度短语.empty()) {
            return 维度短语 + 单体结果;
        }
        return 单体结果;
    }

    std::string 私有_二次特征反义词结果_按主信息(const 二次特征主信息类* 主信息)
    {
        if (!主信息) return {};
        if (主信息->种类 == 枚举_二次特征种类::观察关系) {
            return "观察到";
        }

        const auto 轴键 = 私有_二次特征轴键(主信息);
        if (const auto* 模板 = 私有_匹配二次特征方向模板(轴键)) {
            if (主信息->标量值 > 0) return 模板->正向结果;
            if (主信息->标量值 < 0) return 模板->反向结果;
            return 模板->中性结果;
        }

        if (主信息->是否满足) {
            if (主信息->标量值 > 0) return "偏正向";
            if (主信息->标量值 < 0) return "偏反向";
            return "成立";
        }
        if (主信息->标量值 > 0) return "更明显";
        if (主信息->标量值 < 0) return "更不明显";
        return "相当";
    }

    std::string 私有_二次特征介词短语_按主信息(const 基础信息类* 基础信息, const 二次特征主信息类* 主信息)
    {
        if (!基础信息 || !主信息) return {};

        if (主信息->种类 == 枚举_二次特征种类::观察关系) {
            if (主信息->所属场景.指针) {
                return "在" + 私有_基础节点短名(基础信息, 主信息->所属场景.指针) + "中";
            }
            return {};
        }

        std::vector<std::string> 短语{};
        const auto 维度短语 = 私有_二次特征维度短语(主信息);
        if (!维度短语.empty()) {
            短语.push_back(维度短语);
        }
        const auto 粒度短语 = 私有_二次特征粒度短语(主信息);
        if (!粒度短语.empty()) {
            短语.push_back(粒度短语);
        }
        const auto 基准短语 = 私有_二次特征基准短语(主信息);
        if (!基准短语.empty()) {
            短语.push_back(基准短语);
        }
        const auto 时间归一短语 = 私有_二次特征时间归一短语(主信息);
        if (!时间归一短语.empty()) {
            短语.push_back(时间归一短语);
        }

        const auto 比较对象 = 私有_二次特征比较对象名称(基础信息, 主信息);
        if (!比较对象.empty()) {
            短语.push_back("相对于" + 比较对象);
        }

        if (短语.empty() && 主信息->所属场景.指针) {
            短语.push_back("在" + 私有_基础节点短名(基础信息, 主信息->所属场景.指针) + "中");
        }

        if (短语.empty()) return {};
        return "（" + 私有_连接限定短语(短语) + "）";
    }

    std::string 私有_基础节点短名(const 基础信息类* 基础信息, const 基础信息节点类* 节点)
    {
        if (!基础信息 || !节点 || !节点->主信息) return {};

        if (const auto* 指代主信息 = 基础信息->取主信息<指代节点主信息类>(节点)) {
            const auto 代词 = 私有_安全词(指代主信息->代词);
            if (!代词.empty()) return 代词;
            if (指代主信息->指代对象.指针) {
                return 私有_安全文本(
                    私有_安全词(指代主信息->指代对象.指针->主信息 ? 指代主信息->指代对象.指针->主信息->名称 : nullptr),
                    "指代");
            }
            return "指代";
        }

        if (const auto* 场景主信息 = 基础信息->取主信息<场景节点主信息类>(节点)) {
            const auto 名称 = 私有_安全词(场景主信息->名称);
            return 名称.empty() ? 私有_世界类型文本(场景主信息->世界类型) : 名称;
        }

        if (const auto* 存在主信息 = 基础信息->取主信息<存在节点主信息类>(节点)) {
            const auto 名称 = 私有_安全词(存在主信息->名称);
            if (!名称.empty()) return 名称;
            const auto 类型 = 私有_安全词(存在主信息->类型);
            return 类型.empty() ? "存在" : 类型;
        }

        if (const auto* 特征主信息 = 基础信息->取主信息<特征节点主信息类>(节点)) {
            const auto 类型 = 私有_安全词(特征主信息->类型);
            if (!类型.empty()) return 类型;
            const auto 名称 = 私有_安全词(特征主信息->名称);
            return 名称.empty() ? "特征" : 名称;
        }

        if (const auto* 状态主信息 = 基础信息->取主信息<状态节点主信息类>(节点)) {
            std::string 主体短名{};
            if (状态主信息->状态主体.指针) {
                主体短名 = 私有_基础节点短名(基础信息, 状态主信息->状态主体.指针);
            }
            std::string 特征短名{};
            if (const auto* 特征主信息 = 基础信息->取主信息<特征节点主信息类>(状态主信息->状态特征.指针)) {
                特征短名 = 私有_安全文本(私有_安全词(特征主信息->类型), 私有_安全词(特征主信息->名称));
            }
            if (!主体短名.empty() && !特征短名.empty()) {
                return 主体短名 + "的" + 特征短名;
            }
            if (!特征短名.empty()) return 特征短名;
            return "状态";
        }

        if (const auto* 动态主信息 = 基础信息->取主信息<动态节点主信息类>(节点)) {
            if (const auto* 来源方法 = reinterpret_cast<const 方法类::节点类*>(动态主信息->来源动作.指针)) {
                const auto 动作名 = 私有_方法节点名称(来源方法);
                if (!动作名.empty()) return 动作名;
            }
            const auto 动作名 = 私有_安全词(动态主信息->来源动作名);
            if (!动作名.empty()) return 动作名;
            return "动态";
        }

        if (const auto* 二次特征主信息 = 二次特征主信息_桥接(节点)) {
            const auto 概念名称 = 私有_安全词(二次特征主信息->概念名称);
            if (!概念名称.empty()) return 概念名称;
            return 私有_二次特征种类文本(二次特征主信息->种类);
        }

        if (const auto* 因果主信息 = 世界树.因果().取因果主信息(static_cast<const 因果节点类*>(节点))) {
            if (因果主信息->是实例因果()) {
                if (!因果主信息->动作名称.empty()) return 因果主信息->动作名称;
                if (!因果主信息->动作语义键.empty()) return 因果主信息->动作语义键;
                const auto 名称 = 私有_安全词(因果主信息->名称);
                return 名称.empty() ? "因果实例" : 名称;
            }
            if (因果主信息->是抽象因果()) {
                const auto 名称 = 私有_安全词(因果主信息->名称);
                return 名称.empty() ? "抽象因果" : 名称;
            }
        }

        const auto 名称 = 私有_安全词(节点->主信息->名称);
        if (!名称.empty()) return 名称;
        const auto 类型 = 私有_安全词(节点->主信息->类型);
        return 类型.empty() ? std::string("节点#") + 节点->获取主键() : 类型;
    }
}

std::string 语言类::生成二次特征反义词结果(const 二次特征节点类* 节点) const
{
    const auto* 主信息 = 二次特征主信息_桥接(节点);
    return 私有_二次特征反义词结果_按主信息(主信息);
}

std::string 语言类::生成二次特征介词短语(const 二次特征节点类* 节点) const
{
    const auto* 主信息 = 二次特征主信息_桥接(节点);
    return 私有_二次特征介词短语_按主信息(&世界树.基础信息(), 主信息);
}

std::string 语言类::生成二次特征自然语言(const 二次特征节点类* 节点) const
{
    const auto& 基础信息 = 世界树.基础信息();
    const auto* 主信息 = 二次特征主信息_桥接(节点);
    if (!主信息) return {};

    const auto 主语 = 私有_二次特征主语名称(&基础信息, 主信息);
    const auto 比较对象 = 私有_二次特征比较对象名称(&基础信息, 主信息);
    const auto 维度短语 = 私有_二次特征维度短语(主信息);
    const auto 介词短语 = 私有_二次特征介词短语_按主信息(&基础信息, 主信息);
    const auto 反义词结果 = 私有_二次特征反义词结果_按主信息(主信息);
    const auto* 方向模板 = 私有_二次特征方向模板_按主信息(主信息);

    if (主信息->种类 == 枚举_二次特征种类::观察关系) {
        const auto 客体 = 私有_基础节点短名(&基础信息, 主信息->客体.指针);
        if (!主语.empty() && !客体.empty()) {
            if (!介词短语.empty()) {
                return 主语 + 介词短语 + 反义词结果 + 客体;
            }
            return 主语 + 反义词结果 + 客体;
        }
    }

    if (!主语.empty() && !反义词结果.empty()) {
        if (方向模板 && 方向模板->是方位关系) {
            const auto 关系结果 = 私有_二次特征空间关系结果(反义词结果, 比较对象);
            if (!关系结果.empty()) {
                return 主语 + 关系结果;
            }
        }

        const auto 比较结果 = 私有_二次特征比较结果(反义词结果, 比较对象, 维度短语);
        if (!比较结果.empty()) {
            return 主语 + 比较结果;
        }
    }

    if (!主语.empty() && !介词短语.empty() && !反义词结果.empty()) {
        return 主语 + 介词短语 + 反义词结果;
    }
    if (!介词短语.empty() && !反义词结果.empty()) {
        return 介词短语 + 反义词结果;
    }
    if (!反义词结果.empty()) return 反义词结果;

    const auto 概念名称 = 私有_安全词(主信息->概念名称);
    if (!概念名称.empty()) return 概念名称;
    return 私有_二次特征种类文本(主信息->种类);
}
