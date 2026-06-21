#pragma once

#include "方法主信息类.h"

class 基础信息类;
enum class 枚举_本能方法ID : std::uint32_t;

class 方法类 : public 链表模板<方法主信息类> {
    friend class 本能方法类;
    friend class 方法虚拟存在服务类;

public:
    using 基类 = 链表模板<方法主信息类>;
    using 节点类 = 基类::节点类;

public:
    // 功能：按主键查找并解析方法节点，不创建或修改节点。
    方法节点类* 按主键解析方法节点(const std::string& 主键) noexcept;
    const 方法节点类* 按主键解析方法节点(const std::string& 主键) const noexcept;
    static 方法节点类* 按主键解析方法节点(基础信息类* 基础信息, const std::string& 主键) noexcept;

    static 节点类* 查找方法首节点_按动作句柄(
        节点类* 方法根节点,
        const 结构体_动作句柄& 动作句柄) noexcept;
    static const 节点类* 查找方法首节点_按动作句柄(
        const 节点类* 方法根节点,
        const 结构体_动作句柄& 动作句柄) noexcept;

    static 节点类* 查找方法首节点_按动作名(
        节点类* 方法根节点,
        const 语素入口节点类* 动作名) noexcept;
    static const 节点类* 查找方法首节点_按动作名(
        const 节点类* 方法根节点,
        const 语素入口节点类* 动作名) noexcept;

    static 节点类* 注册方法首节点(
        节点类* 方法根节点,
        const 语素入口节点类* 动作名,
        const 结构体_动作句柄& 动作句柄,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const 语素入口节点类* 主结果特征类型 = nullptr);

    static 节点类* 注册需求位方法首节点(
        节点类* 方法根节点,
        const 语素入口节点类* 主结果特征类型,
        枚举_方法来源 来源 = 枚举_方法来源::未定义,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static 节点类* 注册需求方向方法首节点(
        节点类* 方法根节点,
        const 语素入口节点类* 主结果特征类型,
        二次特征节点类* 结果方向,
        枚举_方法来源 来源 = 枚举_方法来源::未定义,
        时间戳 now = 结构体_时间戳::当前_微秒());
    // 功能：把当前方法树本体重写到 SQL Server 查询投影。
    static bool 重写方法树SQL投影(
        const 节点类* 方法根节点,
        const char* 来源原因) noexcept;

    static 场景节点类* 取或创建_方法条件场景(
        节点类* 方法首节点,
        存在节点类* 宿主存在 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static 场景节点类* 取或创建_方法结果场景(
        节点类* 方法首节点,
        存在节点类* 宿主存在 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static 节点类* 创建条件节点(
        节点类* 首节点,
        场景节点类* 条件场景模板 = nullptr,
        const std::vector<可解析引用<二次特征节点类>>& 条件判定索引 = {},
        std::string 条件模板主键 = {},
        时间戳 时间起 = 0,
        时间戳 时间止 = 0);

    static 节点类* 创建结果节点(
        节点类* 父节点,
        场景节点类* 结果场景模板 = nullptr,
        const std::vector<可解析引用<二次特征节点类>>& 结果方向索引 = {});

    // 功能：判断方法首节点是否明确生产目标特征类型。
    static bool 方法首节点生产目标特征(
        const 方法节点类* 方法,
        const 语素入口节点类* 目标特征类型) noexcept;

    // 功能：判断方法树中是否存在明确生产目标特征类型的方法首节点。
    static bool 方法树存在生产目标特征(
        const 节点类* 方法根节点,
        const 语素入口节点类* 目标特征类型) noexcept;

    // 功能：判断节点是否为方法首节点。
    static bool 方法是首节点(const 节点类* 方法节点) noexcept;

    // 功能：判断方法首节点是否声明了动作入口。
    static bool 方法有动作(const 节点类* 方法首节点) noexcept;

    // 功能：读取方法首节点来源枚举。
    static 枚举_方法来源 方法来源(const 节点类* 方法首节点) noexcept;

    // 功能：读取方法首节点成熟度阶段。
    static I64 方法成熟度阶段(const 节点类* 方法首节点) noexcept;

    // 功能：读取方法首节点的动作名入口。
    static const 语素入口节点类* 方法动作名(const 节点类* 方法首节点) noexcept;

    // 功能：读取方法首节点的动作句柄。
    static 结构体_动作句柄 方法动作句柄(const 节点类* 方法首节点) noexcept;

    // 功能：读取方法首节点绑定的本能方法 ID。
    static I64 方法本能ID(const 节点类* 方法首节点) noexcept;

    // 功能：判断方法首节点是否绑定了有效本能方法。
    static bool 方法指定本能方法(const 节点类* 方法首节点) noexcept;

    // 功能：统计方法首节点的动作数量。
    static I64 方法动作数量(const 节点类* 方法首节点) noexcept;

    // 功能：统计方法首节点的结构化可执行入口数量。
    static I64 方法可执行入口数量(const 节点类* 方法首节点) noexcept;

    // 功能：统计方法首节点的条件节点数量。
    static I64 方法条件节点数量(const 节点类* 方法首节点) noexcept;

    // 功能：统计方法首节点的结果节点数量。
    static I64 方法结果节点数量(const 节点类* 方法首节点) noexcept;

    // 功能：统计方法首节点的条件结果对数量。
    static I64 方法条件结果对数量(const 节点类* 方法首节点) noexcept;

    // 功能：读取方法首节点的第一个条件场景。
    static 场景节点类* 方法首个条件场景(
        const 节点类* 方法首节点,
        场景节点类* 默认场景 = nullptr) noexcept;

    // 功能：计算方法首节点当前最低结构状态值。
    static I64 方法最低结构状态值(const 节点类* 方法首节点) noexcept;

    // 功能：判断方法首节点是否满足可用最低结构判据。
    static bool 方法满足可用最低判据(const 节点类* 方法首节点) noexcept;

    // 功能：比较两个场景的状态语义是否等价。
    static bool 场景状态语义等价(
        const 场景节点类* 左,
        const 场景节点类* 右) noexcept;

    // 功能：在方法条件集合中声明条件特征并记录是否必需。
    static bool 声明条件特征(
        节点类* 方法首节点,
        const 语素入口节点类* 特征类型,
        bool 是否必需,
        时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;

    // 功能：在方法条件集合中声明必需条件特征。
    static bool 声明条件特征(
        节点类* 方法首节点,
        const 语素入口节点类* 特征类型,
        时间戳 now = 结构体_时间戳::当前_微秒()) noexcept;

private:
    static 节点类* 查找或创建_本能方法首节点(
        存在节点类* 宿主存在,
        枚举_本能方法ID 本能ID,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 是有效本能方法能力值(
        I64 自我能力值) noexcept;

    static 节点类* 取或确保本能方法首节点_按能力值(
        存在节点类* 宿主存在,
        I64 自我能力值,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 补齐本能方法首节点(
        节点类* 方法首节点,
        存在节点类* 宿主存在 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static 存在节点类* 取或创建_方法虚拟存在(
        节点类* 方法首节点,
        存在节点类* 宿主存在 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 记录方法虚拟存在特征(
        节点类* 方法首节点,
        const 语素入口节点类* 特征类型,
        const 特征值& 值,
        存在节点类* 宿主存在 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());

    static bool 初始化方法虚拟存在信息(
        节点类* 方法首节点,
        存在节点类* 宿主存在 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒());
};
