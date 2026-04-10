#include "基础信息类.h"

#include <sstream>

#include "世界树类.h"
#include "因果类.h"
#include "自我类.h"
#include "语素类.h"
#include "需求类.h"
#include "任务类.h"
#include "方法类.h"
#include "语言类.h"

namespace {
    std::string 私有_基础节点短名(const 基础信息类* 基础信息, const 基础信息节点类* 节点);

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

    template<class T节点>
    std::vector<const T节点*> 私有_枚举独立树直接子节点(const T节点* 父节点)
    {
        std::vector<const T节点*> out;
        if (!父节点 || !父节点->子) return out;

        auto* 起点 = static_cast<const T节点*>(父节点->子);
        auto* 当前 = 起点;
        do {
            out.push_back(当前);
            当前 = static_cast<const T节点*>(当前->下);
        } while (当前 && 当前 != 起点);
        return out;
    }

    template<class T容器>
    void 私有_追加去重项(T容器& 列表, const std::string& 文本, std::size_t 上限)
    {
        if (文本.empty()) return;
        if (上限 > 0 && 列表.size() >= 上限) return;
        if (std::find(列表.begin(), 列表.end(), 文本) != 列表.end()) return;
        列表.push_back(文本);
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

    std::string 私有_任务节点名称(const 任务类::节点类* 节点)
    {
        if (!节点) return {};
        return 私有_安全文本(私有_安全词(节点->主信息.名称), std::string("任务#") + 节点->获取主键());
    }

    std::string 私有_方法节点名称(const 方法类::节点类* 节点)
    {
        if (!节点) return {};
        return 私有_安全文本(私有_安全词(节点->主信息.动作名), std::string("方法#") + 节点->获取主键());
    }

    std::string 私有_需求节点名称(const 需求类::节点类* 节点)
    {
        if (!节点) return {};
        return 私有_安全文本(私有_安全词(节点->主信息.类型), std::string("需求#") + 节点->获取主键());
    }

    std::string 私有_存在称呼(const 存在节点主信息类* 主信息)
    {
        if (!主信息) return "该存在";
        const auto 名称 = 私有_安全词(主信息->名称);
        if (!名称.empty()) return 名称;
        const auto 类型 = 私有_安全词(主信息->类型);
        if (!类型.empty()) return 类型;
        return "该存在";
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

        const auto 比较对象 = 私有_二次特征比较对象名称(基础信息, 主信息);
        if (!比较对象.empty()) {
            短语.push_back("相对于" + 比较对象);
        }

        if (短语.empty() && 主信息->所属场景.指针) {
            短语.push_back("在" + 私有_基础节点短名(基础信息, 主信息->所属场景.指针) + "中");
        }

        return 私有_连接短语(短语, "");
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

        if (const auto* 二次特征主信息 = 基础信息->取主信息<二次特征主信息类>(节点)) {
            const auto 概念名称 = 私有_安全词(二次特征主信息->概念名称);
            if (!概念名称.empty()) return 概念名称;
            return 私有_二次特征种类文本(二次特征主信息->种类);
        }

        if (const auto* 因果主信息 = 基础信息->取主信息<因果实例主信息类>(节点)) {
            if (!因果主信息->动作语义键.empty()) return 因果主信息->动作语义键;
            const auto 名称 = 私有_安全词(因果主信息->名称);
            return 名称.empty() ? "因果实例" : 名称;
        }

        if (const auto* 因果模板主信息 = 基础信息->取主信息<因果模板主信息类>(节点)) {
            const auto 名称 = 私有_安全词(因果模板主信息->名称);
            return 名称.empty() ? "抽象因果" : 名称;
        }

        const auto 名称 = 私有_安全词(节点->主信息->名称);
        if (!名称.empty()) return 名称;
        const auto 类型 = 私有_安全词(节点->主信息->类型);
        return 类型.empty() ? std::string("节点#") + 节点->获取主键() : 类型;
    }

    std::string 私有_指针句柄文本(const 基础信息类* 基础信息, std::uintptr_t 指针值)
    {
        if (指针值 == 0) return "空";
        if (!基础信息) return std::string("ptr#") + std::to_string(指针值);

        for (auto* 节点 : 基础信息->枚举全部节点()) {
            if (reinterpret_cast<std::uintptr_t>(节点) != 指针值) {
                continue;
            }

            const auto 名称 = 私有_基础节点短名(基础信息, 节点);
            return 名称.empty() ? std::string("节点#") + 节点->获取主键() : 名称;
        }

        const auto* 自我存在 = 自我.获取自我存在();
        if (自我存在) {
            if (const auto* 需求根节点 = 世界树.存在().获取需求根节点(自我存在)) {
                std::string 命中名称{};
                私有_遍历独立树节点(需求根节点, [&](const 需求类::节点类* 节点) {
                    if (命中名称.empty() && reinterpret_cast<std::uintptr_t>(节点) == 指针值) {
                        命中名称 = 私有_需求节点名称(节点);
                    }
                });
                if (!命中名称.empty()) return 命中名称;
            }

            if (const auto* 任务根节点 = 世界树.存在().获取任务根节点(自我存在)) {
                std::string 命中名称{};
                私有_遍历独立树节点(任务根节点, [&](const 任务类::节点类* 节点) {
                    if (命中名称.empty() && reinterpret_cast<std::uintptr_t>(节点) == 指针值) {
                        命中名称 = 私有_任务节点名称(节点);
                    }
                });
                if (!命中名称.empty()) return 命中名称;
            }

            if (const auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在)) {
                std::string 命中名称{};
                私有_遍历独立树节点(方法根节点, [&](const 方法类::节点类* 节点) {
                    if (命中名称.empty() && reinterpret_cast<std::uintptr_t>(节点) == 指针值) {
                        命中名称 = 私有_方法节点名称(节点);
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
}

基础信息类::基础信息类() = default;

基础信息类::~基础信息类()
{
    auto lk = 获取锁();
    私有_清理主信息_已加锁();
}

void 基础信息类::初始化()
{
}

基础信息类::节点类* 基础信息类::世界根() const noexcept
{
    return static_cast<节点类*>(根指针);
}

基础信息类::节点类* 基础信息类::添加节点(节点类* 位置节点, 基础信息基类* 主信息)
{
    if (!主信息) return nullptr;
    auto* pos = 位置节点 ? 位置节点 : 世界根();
    return static_cast<节点类*>(基类::添加节点(pos, 主信息));
}

基础信息类::节点类* 基础信息类::添加子节点(节点类* 父节点, 基础信息基类* 主信息)
{
    if (!主信息) return nullptr;
    auto* parent = 父节点 ? 父节点 : 世界根();
    return static_cast<节点类*>(基类::添加子节点(parent, 主信息));
}

bool 基础信息类::删除节点(节点类* 节点)
{
    if (!节点 || 节点 == 世界根()) return false;
    基类::删除节点(节点);
    return true;
}

bool 基础信息类::更新主信息(节点类* 节点, 基础信息基类* 主信息)
{
    if (!节点 || 节点 == 世界根() || !主信息) return false;

    auto lk = 获取锁();
    if (节点->主信息 == 主信息) return true;

    delete 节点->主信息;
    节点->主信息 = 主信息;
    return true;
}

std::vector<基础信息类::节点类*> 基础信息类::枚举全部节点() const
{
    std::vector<节点类*> out;
    auto lk = 获取读锁();
    if (!根指针) return out;

    for (auto* it = 根指针->链下; it && it != 根指针; it = it->链下) {
        out.push_back(static_cast<节点类*>(it));
    }
    return out;
}

std::vector<基础信息类::节点类*> 基础信息类::枚举子节点(const 节点类* 父节点) const
{
    std::vector<节点类*> out;
    auto lk = 获取读锁();
    auto* parent = 父节点 ? const_cast<节点类*>(父节点) : 世界根();
    if (!parent || !parent->子) return out;

    auto* first = static_cast<节点类*>(parent->子);
    auto* it = first;
    do {
        out.push_back(it);
        it = static_cast<节点类*>(it->下);
    } while (it && it != first);

    return out;
}

std::string 语言类::获取基础信息名称(const 基础信息节点类* 节点) const
{
    const auto& 基础信息 = 世界树.基础信息();
    if (!节点 || !节点->主信息) return "世界根";

    if (const auto* 指代主信息 = 基础信息.取主信息<指代节点主信息类>(节点)) {
        const auto 代词 = 私有_安全词(指代主信息->代词);
        if (!代词.empty() && 指代主信息->指代对象.指针) {
            return 代词 + "（指向" + 私有_基础节点短名(&基础信息, 指代主信息->指代对象.指针) + "）";
        }
        if (!代词.empty()) return 代词;
        if (指代主信息->指代对象.指针) {
            return "指代" + 私有_基础节点短名(&基础信息, 指代主信息->指代对象.指针);
        }
        return "未命名指代";
    }

    if (const auto* 场景主信息 = 基础信息.取主信息<场景节点主信息类>(节点)) {
        const auto 名称 = 私有_安全词(场景主信息->名称);
        const auto 世界类型 = 私有_世界类型文本(场景主信息->世界类型);
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
            return 私有_安全文本(类型, "特征") + "为" + 私有_特征值文本(&基础信息, 特征主信息->当前值);
        }
        return 私有_安全文本(类型, "未命名特征");
    }

    if (const auto* 状态主信息 = 基础信息.取主信息<状态节点主信息类>(节点)) {
        std::string 主体短名{};
        if (状态主信息->状态主体.指针) {
            主体短名 = 私有_基础节点短名(&基础信息, 状态主信息->状态主体.指针);
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
            值文本 = 私有_特征值文本(&基础信息, 状态主信息->状态值);
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
            主体短名 = 私有_基础节点短名(&基础信息, 动态主信息->动态主体.指针);
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

    if (const auto* 二次特征主信息 = 基础信息.取主信息<二次特征主信息类>(节点)) {
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

std::string 语言类::生成二次特征反义词结果(const 二次特征节点类* 节点) const
{
    const auto* 主信息 = 节点 ? dynamic_cast<const 二次特征主信息类*>(节点->主信息) : nullptr;
    return 私有_二次特征反义词结果_按主信息(主信息);
}

std::string 语言类::生成二次特征介词短语(const 二次特征节点类* 节点) const
{
    const auto* 主信息 = 节点 ? dynamic_cast<const 二次特征主信息类*>(节点->主信息) : nullptr;
    return 私有_二次特征介词短语_按主信息(&世界树.基础信息(), 主信息);
}

std::string 语言类::生成二次特征自然语言(const 二次特征节点类* 节点) const
{
    const auto& 基础信息 = 世界树.基础信息();
    const auto* 主信息 = 节点 ? dynamic_cast<const 二次特征主信息类*>(节点->主信息) : nullptr;
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

std::string 语言类::生成存在有什么表达(const 存在节点类* 节点, std::size_t 上限) const
{
    const auto* 主信息 = 节点 ? dynamic_cast<const 存在节点主信息类*>(节点->主信息) : nullptr;
    if (!主信息) return "该存在当前没有可描述的拥有项";

    const auto 称呼 = 私有_存在称呼(主信息);
    std::vector<std::string> 项目{};

    if (主信息->内部世界.指针) {
        const auto 内部世界名 = 私有_基础节点短名(&世界树.基础信息(), 主信息->内部世界.指针);
        私有_追加去重项(项目, 内部世界名.empty() ? "内部世界" : "内部世界" + 内部世界名, 上限);
    }

    for (const auto* 子存在 : 世界树.存在().获取子存在(节点)) {
        私有_追加去重项(项目, 私有_基础节点短名(&世界树.基础信息(), 子存在), 上限);
    }

    for (const auto* 子特征 : 世界树.存在().获取子特征(节点)) {
        私有_追加去重项(项目, 私有_基础节点短名(&世界树.基础信息(), 子特征), 上限);
    }

    if (世界树.存在().获取需求根节点(节点)) {
        私有_追加去重项(项目, "需求树", 上限);
    }
    if (世界树.存在().获取任务根节点(节点)) {
        私有_追加去重项(项目, "任务树", 上限);
    }
    if (世界树.存在().获取方法根节点(节点)) {
        私有_追加去重项(项目, "方法树", 上限);
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

    const auto 称呼 = 私有_存在称呼(主信息);
    const auto 类型 = 私有_安全词(主信息->类型);
    auto 表达 = 称呼 + "是一个" + (类型.empty() ? std::string("存在") : 类型);

    if (主信息->概念模板.指针) {
        const auto 模板名 = 私有_基础节点短名(&世界树.基础信息(), 主信息->概念模板.指针);
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

    const auto 称呼 = 私有_存在称呼(主信息);
    std::vector<std::string> 方法项{};
    const auto* 方法根节点 = 世界树.存在().获取方法根节点(节点);

    if (方法根节点) {
        auto 直接子项 = 私有_枚举独立树直接子节点(方法根节点);
        if (直接子项.empty()) {
            私有_追加去重项(方法项, 私有_方法节点名称(方法根节点), 上限);
        }
        else {
            for (const auto* 方法节点 : 直接子项) {
                私有_追加去重项(方法项, 私有_方法节点名称(方法节点), 上限);
            }
        }
    }

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
        if (!因果引用.指针) continue;
        私有_追加去重项(因果短语, 世界树.因果().生成实例因果自然语言(因果引用.指针), 上限);
    }

    if (因果短语.empty()) {
        return 私有_存在称呼(主信息) + "当前还没有明确的趋向依据";
    }
    return "从已有因果看，" + 私有_连接短语(因果短语, "；");
}

std::string 语言类::生成存在需要什么表达(const 存在节点类* 节点, std::size_t 上限) const
{
    const auto* 主信息 = 节点 ? dynamic_cast<const 存在节点主信息类*>(节点->主信息) : nullptr;
    if (!主信息) return "该存在当前没有明确需求";

    const auto 称呼 = 私有_存在称呼(主信息);
    std::vector<std::string> 需求项{};
    const auto* 需求根节点 = 世界树.存在().获取需求根节点(节点);

    if (需求根节点) {
        auto 直接子项 = 私有_枚举独立树直接子节点(需求根节点);
        if (直接子项.empty()) {
            私有_追加去重项(需求项, 私有_需求节点名称(需求根节点), 上限);
        }
        else {
            for (const auto* 需求节点 : 直接子项) {
                私有_追加去重项(需求项, 私有_需求节点名称(需求节点), 上限);
            }
        }
    }

    if (需求项.empty()) {
        return 称呼 + "当前没有明确需求";
    }
    return 称呼 + "当前需要" + 私有_连接短语(需求项);
}

std::string 基础信息类::获取名称(const 节点类* 节点) const
{
    return 语言集.获取基础信息名称(节点);
}

void 基础信息类::私有_清理主信息_已加锁() noexcept
{
    if (!根指针) return;

    for (auto* it = 根指针->链下; it && it != 根指针; it = it->链下) {
        delete it->主信息;
        it->主信息 = nullptr;
    }
}
