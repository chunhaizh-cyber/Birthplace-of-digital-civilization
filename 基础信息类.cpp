#include "基础信息类.h"

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
    if (!parent || !parent->子) return out;

    auto* first = static_cast<节点类*>(parent->子);
    auto* it = first;
    do {
        out.push_back(it);
        it = static_cast<节点类*>(it->下);
    } while (it && it != first);

    return out;
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
