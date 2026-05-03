module;

#include <cstddef>
#include <cstdint>
#include <string>

export module 学习对象适配器;

export import 学习任务模块;

export {

enum class 枚举_学习对象来源类型 : std::uint8_t {
    未定义 = 0,
    当前方法 = 1,
    因果信息 = 2,
    最近结果 = 3,
    成功执行片段 = 4,
};

struct 结构_学习对象原料 {
    枚举_学习对象来源类型 来源类型 = 枚举_学习对象来源类型::未定义;
    std::uintptr_t 来源类型抽象特征指针 = 0;
    I64 来源类型值 = 0;
    bool 使用当前方法 = false;
    bool 使用因果信息 = false;
    bool 使用最近结果 = false;
    bool 使用成功执行片段 = false;
    std::size_t 因果动态证据数 = 0;
    std::size_t 因果模板数 = 0;
    std::string 来源摘要{};
    std::string 动作摘要{};
    std::string 条件摘要{};
    std::string 结果摘要{};
};

struct 结构_待学习方法骨架 {
    std::string 动作标题{};
    std::string 条件摘要{};
    std::string 结果摘要{};
    bool 具备动作骨架 = false;
    bool 具备条件侧 = false;
    bool 具备结果侧 = false;
};

struct 结构_学习对象适配结果 {
    结构_学习对象原料 原料{};
    结构_待学习方法骨架 待学习方法骨架{};
    bool 仅需求位方法头 = false;
    bool 可直接进入学习 = false;
    std::string 摘要{};
};

namespace 学习对象适配器 {

结构_学习对象适配结果 适配学习对象(
    const 结构_学习任务实体& 学习任务) noexcept;

} // namespace 学习对象适配器

} // export
