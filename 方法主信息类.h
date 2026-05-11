#pragma once

#include <cstdlib>
#include <cstdint>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "语言节点类型.h"
#include "语素节点类型.h"
#include "基础信息节点类型.h"
#include "双向链表模板核心.h"

class 任务节点类;
class 方法节点类;

enum class 枚举_方法节点种类 : std::uint8_t {
    未定义 = 0,
    方法首节点 = 1,
    方法条件节点 = 2,
    方法结果节点 = 3,
};

enum class 枚举_动作句柄类型 : std::uint8_t {
    未设置 = 0,
    本能函数ID = 1,
    外部实现主键 = 2,
    动作序列主键 = 3,
};

enum class 枚举_方法类型 : std::uint8_t {
    未定义 = 0,
    内部方法 = 1,
    外部方法 = 2,
};

enum class 枚举_方法来源 : std::uint8_t {
    未定义 = 0,
    本能 = 1,
    外部导入 = 2,
    因果抽象 = 3,
    观察抽象 = 4,
    路径提炼 = 5,
    运行时方法沉淀 = 6,
};

enum class 枚举_方法作用域 : std::uint8_t {
    未定义 = 0,
    任务局部 = 1,
    主体局部 = 2,
    通用复用 = 3,
    跨域复用 = 4,
};

enum class 枚举_方法作用对象 : std::uint8_t {
    任意 = 0,
    执行者 = 1,
    目标 = 2,
    环境 = 3,
    第三方 = 4,
    方法 = 5,
    任务 = 6,
    内部因果样本 = 7,
    外界信息上下文 = 8,
};

enum class 枚举_条件比较方式 : std::uint8_t {
    未定义 = 0,
    等于 = 1,
    不等于 = 2,
    大于 = 3,
    大于等于 = 4,
    小于 = 5,
    小于等于 = 6,
    区间内 = 7,
    存在 = 8,
    不存在 = 9,
    方向一致 = 10,
};

enum class 枚举_结果变化方向 : std::uint8_t {
    未定义 = 0,
    增加 = 1,
    减少 = 2,
    保持 = 3,
    趋向目标 = 4,
    从无到有 = 5,
    从有到无 = 6,
    从未完成到完成 = 7,
    已形成 = 8,
    已识别 = 9,
    已配对 = 10,
    趋向可用 = 11,
    更完整 = 12,
    更稳定 = 13,
    上升 = 14,
};

struct 结构体_动作句柄 {
    枚举_动作句柄类型 类型 = 枚举_动作句柄类型::未设置;

    I64 本能ID = 0;
    std::string 外部实现主键{};
    std::string 动作序列主键{};

    bool 有效() const noexcept {
        switch (类型) {
        case 枚举_动作句柄类型::本能函数ID: return 本能ID != 0;
        case 枚举_动作句柄类型::外部实现主键: return !外部实现主键.empty();
        case 枚举_动作句柄类型::动作序列主键: return !动作序列主键.empty();
        default: return false;
        }
    }
};

struct 结构_方法条件项 {
    const 语素入口节点类* 特征类型 = nullptr;
    枚举_方法作用对象 作用对象 = 枚举_方法作用对象::任意;
    std::string 绑定名{};
    枚举_条件比较方式 比较方式 = 枚举_条件比较方式::未定义;
    特征值 期望值{};
    I64 区间下界 = 0;
    I64 区间上界 = 0;
    bool 是否必需 = true;
};

struct 结构_方法条件组 {
    std::vector<结构_方法条件项> 条件项集{};
};

struct 结构_方法条件面 {
    std::vector<结构_方法条件组> 可用条件组集{};
};

struct 结构_方法形参项 {
    const 语素入口节点类* 参数特征类型 = nullptr;
    const 语素入口节点类* 参数值类型 = nullptr;
    const 语素入口节点类* 参数来源 = nullptr;
    const 语素入口节点类* 读取特征类型 = nullptr;
    枚举_方法作用对象 作用对象 = 枚举_方法作用对象::任意;
    std::string 绑定名{};
    bool 是否必需 = true;
};

struct 结构_方法形参表 {
    std::vector<结构_方法形参项> 形参项集{};
};

struct 结构_方法结果项 {
    const 语素入口节点类* 特征类型 = nullptr;
    std::string 绑定名{};
    枚举_结果变化方向 方向 = 枚举_结果变化方向::未定义;
    特征值 目标值{};
    I64 最小变化量 = 0;
    I64 最大变化量 = 0;
    bool 是否主结果 = false;
    bool 是否必要结果 = true;
    bool 是否副结果 = false;
    bool 是否风险结果 = false;
    I64 置信度_Q10000 = 0;
};

struct 结构_方法结果包 {
    std::vector<结构_方法结果项> 结果项集{};
};

struct 结构_方法能力签名 {
    std::string 能力键{};
    结构_方法条件面 条件面{};
    结构_方法形参表 形参表{};
    结构_方法结果包 结果包{};
    I64 成熟度阶段 = 0;
    bool 可被方法查找命中 = true;
    bool 是否根写入原语 = false;
};

struct 结构_方法能力面 {
    std::vector<结构_方法能力签名> 能力签名集{};
};

struct 结构_方法首节点能力面 {
    // 权威结构：一个签名表示“若干条件组成立时，产生一包结果变化”。
    结构_方法能力面 能力面{};
};

struct 结构_方法节点公共信息 {
    const 自然句节点类* 描述信息 = nullptr;
    枚举_方法节点种类 节点种类 = 枚举_方法节点种类::未定义;
    结构_统计 统计{};
};

struct 结构_方法首节点信息 {
    可解析引用<存在节点类> 方法虚拟存在{};
    const 语素入口节点类* 动作名 = nullptr;
    结构体_动作句柄 动作句柄{};
    枚举_方法来源 来源 = 枚举_方法来源::未定义;
    枚举_方法作用域 作用域 = 枚举_方法作用域::未定义;
    可解析引用<任务节点类> 来源任务{};
    可解析引用<方法节点类> 父方法{};
    可解析引用<方法节点类> 前置方法{};
    可解析引用<方法节点类> 后续方法{};
    结构_方法首节点能力面 能力{};
    I64 成熟度阶段 = 0;
    bool 是否允许自动查找 = true;
};

struct 结构_方法条件节点信息 {
    可解析引用<场景节点类> 条件场景{};
    std::string 条件主键{};
    std::string 条件场景模板主键{};
    结构_方法条件面 条件面{};
    std::vector<可解析引用<二次特征节点类>> 条件判定索引{};
    时间戳 时间起 = 0;
    时间戳 时间止 = 0;
};

struct 结构_方法结果节点信息 {
    可解析引用<场景节点类> 结果场景{};
    std::string 结果主键{};
    结构_方法结果包 结果包{};
    std::vector<可解析引用<状态节点类>> 结果初始状态{};
};

using 方法角色信息 = std::variant<
    std::monostate,
    结构_方法首节点信息,
    结构_方法条件节点信息,
    结构_方法结果节点信息>;

class 方法主信息类 {
public:
    结构_方法节点公共信息 公共{};
    方法角色信息 角色{};

    方法主信息类() = default;
    explicit 方法主信息类(const 自然句节点类* 描述) {
        公共.描述信息 = 描述;
    }

    static 方法主信息类 构造首节点(
        const 语素入口节点类* 动作 = nullptr,
        结构体_动作句柄 动作句柄 = {}) {
        方法主信息类 信息{};
        信息.公共.节点种类 = 枚举_方法节点种类::方法首节点;
        结构_方法首节点信息 首节点{};
        首节点.动作名 = 动作;
        首节点.动作句柄 = 动作句柄;
        信息.角色 = std::move(首节点);
        return 信息;
    }

    static 方法主信息类 构造条件节点() {
        方法主信息类 信息{};
        信息.公共.节点种类 = 枚举_方法节点种类::方法条件节点;
        信息.角色 = 结构_方法条件节点信息{};
        return 信息;
    }

    static 方法主信息类 构造结果节点() {
        方法主信息类 信息{};
        信息.公共.节点种类 = 枚举_方法节点种类::方法结果节点;
        信息.角色 = 结构_方法结果节点信息{};
        return 信息;
    }

    枚举_方法节点种类 节点种类() const noexcept {
        return 公共.节点种类;
    }

    bool 是否方法首节点() const noexcept {
        return 公共.节点种类 == 枚举_方法节点种类::方法首节点;
    }

    bool 是否方法条件节点() const noexcept {
        return 公共.节点种类 == 枚举_方法节点种类::方法条件节点;
    }

    bool 是否方法结果节点() const noexcept {
        return 公共.节点种类 == 枚举_方法节点种类::方法结果节点;
    }

    结构_方法首节点信息* 取首节点信息() noexcept {
        if (!是否方法首节点()) return nullptr;
        return std::get_if<结构_方法首节点信息>(&角色);
    }

    const 结构_方法首节点信息* 取首节点信息() const noexcept {
        if (!是否方法首节点()) return nullptr;
        return std::get_if<结构_方法首节点信息>(&角色);
    }

    结构_方法条件节点信息* 取条件节点信息() noexcept {
        if (!是否方法条件节点()) return nullptr;
        return std::get_if<结构_方法条件节点信息>(&角色);
    }

    const 结构_方法条件节点信息* 取条件节点信息() const noexcept {
        if (!是否方法条件节点()) return nullptr;
        return std::get_if<结构_方法条件节点信息>(&角色);
    }

    结构_方法结果节点信息* 取结果节点信息() noexcept {
        if (!是否方法结果节点()) return nullptr;
        return std::get_if<结构_方法结果节点信息>(&角色);
    }

    const 结构_方法结果节点信息* 取结果节点信息() const noexcept {
        if (!是否方法结果节点()) return nullptr;
        return std::get_if<结构_方法结果节点信息>(&角色);
    }

    结构_方法首节点信息& 首节点信息() noexcept {
        auto* 信息 = 取首节点信息();
        if (信息) return *信息;
        std::abort();
    }

    const 结构_方法首节点信息& 首节点信息() const noexcept {
        const auto* 信息 = 取首节点信息();
        if (信息) return *信息;
        static const 结构_方法首节点信息 空{};
        return 空;
    }

    const 结构_方法条件节点信息& 条件节点信息() const noexcept {
        const auto* 信息 = 取条件节点信息();
        if (信息) return *信息;
        static const 结构_方法条件节点信息 空{};
        return 空;
    }

    const 结构_方法结果节点信息& 结果节点信息() const noexcept {
        const auto* 信息 = 取结果节点信息();
        if (信息) return *信息;
        static const 结构_方法结果节点信息 空{};
        return 空;
    }

    bool 有动作骨架() const noexcept {
        const auto* 首节点 = 取首节点信息();
        return 首节点 && (首节点->动作名 || 首节点->动作句柄.有效());
    }

    bool 有结果能力() const noexcept {
        const auto* 首节点 = 取首节点信息();
        if (!首节点) return false;
        for (const auto& 签名 : 首节点->能力.能力面.能力签名集) {
            if (!签名.结果包.结果项集.empty()) {
                return true;
            }
        }
        return false;
    }

    const 语素入口节点类* 首个结果能力特征类型() const noexcept {
        const auto* 首节点 = 取首节点信息();
        if (!首节点) return nullptr;
        for (const auto& 签名 : 首节点->能力.能力面.能力签名集) {
            for (const auto& 结果项 : 签名.结果包.结果项集) {
                if (结果项.特征类型) {
                    return 结果项.特征类型;
                }
            }
        }
        return nullptr;
    }

    const std::vector<结构_方法能力签名>* 取能力签名集() const noexcept {
        const auto* 首节点 = 取首节点信息();
        return 首节点 ? &首节点->能力.能力面.能力签名集 : nullptr;
    }

    bool 有最小出生锚点() const noexcept {
        return 有动作骨架() || 有结果能力();
    }

    bool 是仅结果特征需求位() const noexcept {
        return 有结果能力() && !有动作骨架();
    }

    std::int64_t 比较(const 方法主信息类* 对象, 枚举_比较字段 字段) const {
        return 对象 ? 比较(*对象, 字段) : -1;
    }

    std::int64_t 比较(const 方法主信息类& 对象, 枚举_比较字段 字段) const {
        const auto* 左首节点 = 取首节点信息();
        const auto* 右首节点 = 对象.取首节点信息();
        switch (字段) {
        case 枚举_比较字段::名称:
            return 左首节点 && 右首节点 && 左首节点->动作名 == 右首节点->动作名 ? 1 : 0;
        case 枚举_比较字段::类型:
            return 公共.节点种类 == 对象.公共.节点种类 ? 1 : 0;
        case 枚举_比较字段::值:
            return 左首节点 && 右首节点 && 私有_动作句柄相等(左首节点->动作句柄, 右首节点->动作句柄) ? 1 : 0;
        default:
            return 0;
        }
    }

protected:
    static bool 私有_动作句柄相等(const 结构体_动作句柄& a, const 结构体_动作句柄& b) noexcept {
        return a.类型 == b.类型
            && a.本能ID == b.本能ID
            && a.外部实现主键 == b.外部实现主键
            && a.动作序列主键 == b.动作序列主键;
    }
};
