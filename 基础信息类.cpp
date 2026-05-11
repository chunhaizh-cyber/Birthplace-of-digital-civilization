#include "基础信息类.h"
#include "语素主信息类.h"
#include "语素类.h"

#include <stdexcept>

#include "语言类.h"
#include "语言类_基础信息命名.cpp"
#define 私有_安全词 语言类二次_私有_安全词
#define 私有_安全文本 语言类二次_私有_安全文本
#define 私有_世界类型文本 语言类二次_私有_世界类型文本
#define 私有_二次特征种类文本 语言类二次_私有_二次特征种类文本
#define 私有_方法节点名称 语言类二次_私有_方法节点名称
#include "语言类_二次特征表达.cpp"
#undef 私有_方法节点名称
#undef 私有_二次特征种类文本
#undef 私有_世界类型文本
#undef 私有_安全文本
#undef 私有_安全词

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
    if (!parent || !私有_节点属于当前树_已加锁(parent) || !parent->子) return out;

    auto* first = static_cast<节点类*>(parent->子);
    auto* it = first;
    do {
        out.push_back(it);
        it = static_cast<节点类*>(it->下);
    } while (it && it != first);

    return out;
}


基础信息类::节点类* 基础信息类::查找语素入口模板(const 语素入口节点类* 入口节点, 枚举_主信息类型 类型) const
{
    if (!入口节点 || 类型 == 枚举_主信息类型::未定义) return nullptr;

    auto lk = 获取读锁();
    if (!根指针) return nullptr;

    for (auto* it = 根指针->链下; it && it != 根指针; it = it->链下) {
        auto* 节点 = static_cast<节点类*>(it);
        if (!基础信息节点匹配类型(节点, 类型)) continue;
        if (私有_主信息引用语素入口节点(节点->主信息, 入口节点)) {
            return 节点;
        }
    }
    return nullptr;
}

基础信息类::节点类* 基础信息类::确保语素入口模板(const 语素入口节点类* 入口节点, 枚举_主信息类型 类型, 节点类* 父节点)
{
    if (!入口节点) {
        throw std::invalid_argument("基础信息类::确保语素入口模板 - 语素入口节点为空");
    }
    if (类型 == 枚举_主信息类型::未定义) {
        return nullptr;
    }
    枚举_信息入口类型 信息入口类型 = 枚举_信息入口类型::未定义;
    if (auto* 入口主信息 = dynamic_cast<const 语素入口主信息类*>(入口节点->主信息)) {
        信息入口类型 = 入口主信息->信息入口类型;
    }

    const auto 存储树 = 语素_取信息入口存储树(信息入口类型);
    auto* 父 = 父节点 ? 父节点 : 选择信息入口默认根(信息入口类型);
    if (!父 && 存储树 == 枚举_信息存储树::不入树) {
        return nullptr;
    }
    if (!父 && 语素_信息入口需要上下文限定(信息入口类型)) {
        return nullptr;
    }
    if (!父) {
        父 = (类型 == 枚举_主信息类型::存在 ? 取或创建存在概念根() : 世界根());
    }
    if (类型 == 枚举_主信息类型::存在 && !父节点) {
        for (auto* 已有 : 查找引用语素入口节点的基础信息(入口节点, 父)) {
            if (基础信息节点匹配类型(已有, 类型)) {
                return 已有;
            }
        }
    } else if (auto* 已有 = 查找语素入口模板(入口节点, 类型)) {
        return 已有;
    }
    if (!主信息类型是否可由语素入口模板创建(类型)) {
        return nullptr;
    }

    auto* 新主信息 = 私有_创建语素入口模板主信息(入口节点, 类型);
    if (!新主信息) return nullptr;

    auto* 新节点 = 添加子节点(父, 新主信息);
    if (!新节点) {
        delete 新主信息;
        return nullptr;
    }
    return 新节点;
}

基础信息类::节点类* 基础信息类::选择信息入口默认根(
    枚举_信息入口类型 信息入口类型,
    节点类* 上下文节点)
{
    if (信息入口类型 == 枚举_信息入口类型::未定义) {
        return nullptr;
    }
    if (!上下文节点 && 语素_信息入口需要上下文限定(信息入口类型)) {
        return nullptr;
    }

    switch (语素_取信息入口存储树(信息入口类型)) {
    case 枚举_信息存储树::基础信息模板树:
        switch (信息入口类型) {
        case 枚举_信息入口类型::存在概念入口:
        case 枚举_信息入口类型::需求概念入口:
        case 枚举_信息入口类型::任务概念入口:
        case 枚举_信息入口类型::方法概念入口:
            return 取或创建存在概念根();
        default:
            return 世界根();
        }

    case 枚举_信息存储树::现实世界树:
    case 枚举_信息存储树::内部世界树:
    case 枚举_信息存储树::语言记录树:
        return 上下文节点 ? 上下文节点 : 世界根();

    case 枚举_信息存储树::需求根树:
    case 枚举_信息存储树::任务根树:
    case 枚举_信息存储树::方法根树:
        return 上下文节点;

    case 枚举_信息存储树::不入树:
    case 枚举_信息存储树::未定义:
    default:
        return nullptr;
    }
}

std::vector<基础信息类::节点类*> 基础信息类::查找引用语素入口节点的基础信息(
    const 语素入口节点类* 入口节点,
    const 节点类* 范围根
) const
{
    std::vector<节点类*> out;
    if (!入口节点) return out;

    auto lk = 获取读锁();
    if (!根指针) return out;

    for (auto* it = 根指针->链下; it && it != 根指针; it = it->链下) {
        auto* 节点 = static_cast<节点类*>(it);
        if (!私有_节点在范围内(范围根, 节点)) continue;
        if (私有_主信息引用语素入口节点(节点->主信息, 入口节点)) {
            out.push_back(节点);
        }
    }
    return out;
}

bool 基础信息类::主信息类型是否可由语素入口模板创建(枚举_主信息类型 类型) noexcept
{
    return 语素_基础信息类型可由语素入口模板创建(类型);
}

bool 基础信息类::基础信息节点匹配类型(const 节点类* 节点, 枚举_主信息类型 类型) noexcept
{
    if (!节点 || !节点->主信息) return false;
    if (类型 == 枚举_主信息类型::未定义) return true;
    return 节点->主信息->主信息类型 == 类型;
}

bool 基础信息类::基础信息节点引用语素入口节点(const 节点类* 节点, const 语素入口节点类* 入口节点) noexcept
{
    return 节点 && 私有_主信息引用语素入口节点(节点->主信息, 入口节点);
}

基础信息基类* 基础信息类::私有_创建语素入口模板主信息(const 语素入口节点类* 入口节点, 枚举_主信息类型 类型)
{
    switch (类型) {
    case 枚举_主信息类型::指代: {
        auto* 主信息 = new 指代节点主信息类{};
        主信息->名称 = 入口节点;
        主信息->代词 = 入口节点;
        return 主信息;
    }
    case 枚举_主信息类型::存在:
        return new 存在节点主信息类(入口节点, nullptr);
    case 枚举_主信息类型::抽象特征:
        return new 抽象特征主信息类(入口节点);
    case 枚举_主信息类型::特征:
        return new 特征节点主信息类(入口节点);
    case 枚举_主信息类型::状态: {
        auto* 主信息 = new 状态节点主信息类{};
        主信息->名称 = 入口节点;
        return 主信息;
    }
    case 枚举_主信息类型::场景:
        return new 场景节点主信息类(入口节点, 枚举_世界类型::现实世界);
    case 枚举_主信息类型::动态: {
        auto* 主信息 = new 动态节点主信息类{};
        主信息->名称 = 入口节点;
        主信息->来源动作名 = 入口节点;
        主信息->业务语义类型 = 枚举_动态业务语义类型::动作事实动态;
        return 主信息;
    }
    case 枚举_主信息类型::二次特征_修饰: {
        auto* 主信息 = new 二次特征主信息类{};
        主信息->名称 = 入口节点;
        主信息->概念名称 = 入口节点;
        主信息->形态 = 二次特征主信息类::枚举_形态::概念模板;
        return 主信息;
    }
    case 枚举_主信息类型::因果: {
        auto* 主信息 = new 因果主信息类(枚举_因果形态::抽象);
        主信息->名称 = 入口节点;
        return 主信息;
    }
    case 枚举_主信息类型::语言信息_仅记录:
        return new 语言记录主信息类(入口节点);
    default:
        return nullptr;
    }
}

bool 基础信息类::私有_主信息引用语素入口节点(const 基础信息基类* 主信息, const 语素入口节点类* 入口节点) noexcept
{
    if (!主信息 || !入口节点) return false;
    if (主信息->名称 == 入口节点 || 主信息->类型 == 入口节点) return true;

    if (auto* 指代 = dynamic_cast<const 指代节点主信息类*>(主信息)) {
        if (指代->代词 == 入口节点) return true;
    }
    if (auto* 动态 = dynamic_cast<const 动态节点主信息类*>(主信息)) {
        if (动态->来源动作名 == 入口节点) return true;
    }
    if (auto* 二次 = dynamic_cast<const 二次特征主信息类*>(主信息)) {
        if (二次->概念名称 == 入口节点) return true;
    }
    if (auto* 语言 = dynamic_cast<const 语言记录主信息类*>(主信息)) {
        if (语言->语言词性 == 入口节点) return true;
    }
    return false;
}

bool 基础信息类::私有_节点在范围内(const 节点类* 范围根, const 节点类* 候选节点) noexcept
{
    if (!范围根) return true;
    for (auto* 游标 = 候选节点; 游标; 游标 = static_cast<const 节点类*>(游标->父)) {
        if (游标 == 范围根) return true;
    }
    return false;
}

std::string 基础信息类::获取名称(const 节点类* 节点) const
{
    return 语言集.获取基础信息名称(节点);
}

存在节点类* 基础信息类::取或创建存在概念根()
{
    if (存在概念根_ && 取主信息<存在节点主信息类>(存在概念根_)) {
        return static_cast<存在节点类*>(存在概念根_);
    }

    const auto* 根名 = 语素集.添加信息入口词("存在概念根", 枚举_信息入口类型::存在概念入口);
    const auto* 根型 = 语素集.添加信息入口词("概念根", 枚举_信息入口类型::存在概念入口);
    for (auto* 节点 : 枚举子节点_按类型<存在节点主信息类>(世界根())) {
        const auto* 主信息 = 取主信息<存在节点主信息类>(节点);
        if (主信息 && 主信息->名称 == 根名) {
            存在概念根_ = 节点;
            return static_cast<存在节点类*>(存在概念根_);
        }
    }

    auto* 主信息 = new 存在节点主信息类(根名, 根型);
    auto* 根节点 = 添加子节点(世界根(), 主信息);
    if (!根节点) {
        delete 主信息;
        return nullptr;
    }
    存在概念根_ = 根节点;
    return static_cast<存在节点类*>(存在概念根_);
}

void 基础信息类::私有_清理主信息_已加锁() noexcept
{
    if (!根指针) return;

    for (auto* it = 根指针->链下; it && it != 根指针; it = it->链下) {
        delete it->主信息;
        it->主信息 = nullptr;
    }
}


基础信息类& 获取基础信息集()
{
    static 基础信息类 全局基础信息集{};
    return 全局基础信息集;
}
