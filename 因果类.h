#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "基础信息类.h"
#include "方法类.h"

// 因果类的口径：
// 1) 因果主体是抽象因果，即“状态变化关系”和“动作导致状态变化关系”。
// 2) 动态 / 运行结果 / 场景中的具体发生，只作为证据样本。
// 3) 因果树不强绑特征树；它只解释状态变化，并可投影为需求候选。

struct 结构_因果表达单元 {
    bool 是抽象因果 = false;
    std::string 锚点名称{};
    std::vector<std::string> 条件短语{};
    std::string 动作短语{};
    std::vector<std::string> 结果短语{};
    std::string 主结果短语{};
    std::uint64_t 成立次数 = 0;
    std::uint64_t 失败次数 = 0;
};

enum class 枚举_因果关系层 : std::uint8_t {
    未定义 = 0,
    状态变化关系 = 1,       // 第一层：取得“状态 A -> 状态 B”的关系。
    动作致变关系 = 2,       // 第二层：取得“动作 X -> 状态变化”的关系。
};

struct 结构_两层因果构建结果 {
    因果模板节点类* 状态变化关系 = nullptr;
    因果模板节点类* 动作致变关系 = nullptr;
    因果实例节点类* 证据实例 = nullptr;

    bool 创建了状态变化关系 = false;
    bool 创建了动作致变关系 = false;
    bool 创建了证据实例 = false;

    std::string 状态变化签名{};
    std::string 动作致变签名{};

    bool 成功() const noexcept {
        return 状态变化关系 != nullptr || 动作致变关系 != nullptr || 证据实例 != nullptr;
    }
};

struct 结构_因果候选边 {
    因果模板节点类* 原因 = nullptr;
    因果模板节点类* 结果 = nullptr;
    std::string 原因签名{};
    std::string 结果签名{};
    std::uint32_t 因果距离 = 0;

    // 投影 / 查询输出中的派生分值，不是因果主信息持久字段。
    std::int64_t 置信度 = 0;
};

enum class 枚举_自我动作组合反推状态 : std::uint8_t {
    未定义 = 0,
    无目标结果 = 1,
    无匹配因果 = 2,
    有因果但无可映射方法 = 3,
    单步本能方法 = 4,
    多候选方法 = 5,
    动作序列待定义 = 6,
};

struct 结构_自我动作组合反推输入 {
    状态节点类* 目标结果状态 = nullptr;
    const 语素入口节点类* 目标特征类型 = nullptr;
    方法类::节点类* 方法根节点 = nullptr;
    std::int64_t 最低稳定度 = 0;
    std::uint32_t 最大候选数 = 16;
    bool 只接受已验证因果 = false;
};

struct 结构_自我动作组合候选 {
    因果模板节点类* 来源因果模板 = nullptr;
    方法类::节点类* 候选方法 = nullptr;
    std::string 动作语义键{};
    std::int64_t 本能方法ID = 0;
    std::int64_t 稳定度 = 0;

    bool 可直接绑定单步本能() const noexcept
    {
        return 候选方法 != nullptr && 本能方法ID != 0;
    }
};

struct 结构_自我动作组合反推结果 {
    枚举_自我动作组合反推状态 状态 =
        枚举_自我动作组合反推状态::未定义;
    std::vector<结构_自我动作组合候选> 候选{};

    bool 有可直接绑定单步本能() const noexcept
    {
        for (const auto& 项 : 候选) {
            if (项.可直接绑定单步本能()) return true;
        }
        return false;
    }
};

enum class 枚举_状态转换因果边类型 : std::uint8_t {
    未定义 = 0,
    物理因果 = 1,
    观测因果 = 2,
    治理因果 = 3,
    账务因果 = 4,
    引用关系 = 5,
};

enum class 枚举_目标投影维度 : std::uint8_t {
    未定义 = 0,
    安全 = 1,
    服务 = 2,
    学习 = 3,
    方法维护 = 4,
    自维护 = 5,
    其他 = 6,
};

enum class 枚举_目标权重类型 : std::uint8_t {
    未定义 = 0,
    安全权重 = 1,
    服务权重 = 2,
    学习权重 = 3,
    方法维护权重 = 4,
    自维护权重 = 5,
    其他 = 6,
};

struct 结构_状态转换因果节点 {
    基础信息节点类* 宿主存在 = nullptr;
    特征节点类* 特征类型 = nullptr;
    状态节点类* 状态节点 = nullptr;
    动态节点类* 来源动态 = nullptr;
    因果模板节点类* 来源因果模板 = nullptr;
    因果实例节点类* 来源因果实例 = nullptr;
    场景节点类* 来源场景 = nullptr;
    时间戳 状态时间 = 0;
    std::string 节点签名{};
};

struct 结构_状态转换因果边 {
    状态节点类* 原因状态 = nullptr;
    状态节点类* 结果状态 = nullptr;
    动态节点类* 动作动态 = nullptr;
    因果模板节点类* 因果模板 = nullptr;
    因果实例节点类* 因果实例 = nullptr;
    枚举_状态转换因果边类型 边类型 = 枚举_状态转换因果边类型::未定义;

    // 由因果主信息的基础计数临时派生，供投影排序和诊断展示使用。
    std::int64_t 置信度 = 0;
    std::int64_t 稳定度 = 0;
    std::vector<动态节点类*> 副作用集合{};
    std::string 边签名{};
};

struct 结构_目标投影路径 {
    枚举_目标投影维度 目标维度 = 枚举_目标投影维度::未定义;
    枚举_目标权重类型 权重类型 = 枚举_目标权重类型::未定义;
    基础信息节点类* 目标宿主 = nullptr;
    特征节点类* 目标特征类型 = nullptr;
    const 语素入口节点类* 目标特征类型词 = nullptr;
    std::vector<结构_状态转换因果边> 证据路径{};
    std::vector<std::string> 缺失证据{};
    std::uint32_t 结算贡献层级 = 0;
    std::uint32_t 因果距离层级 = 0;
    std::uint32_t 投影显示层级 = 0;
    bool 是否可结算 = false;
    bool 是否仅为候选 = true;
};

struct 结构_状态转换因果投影输入 {
    std::uint32_t 最大深度 = 8;
    bool 包含未验证路径 = true;
};

struct 结构_状态转换因果投影 {
    std::vector<结构_状态转换因果节点> 节点{};
    std::vector<结构_状态转换因果边> 边{};
    std::vector<结构_目标投影路径> 目标投影路径{};
    std::vector<std::string> 缺失证据{};
};

struct 结构_叶子任务因果投影输入 {
    任务节点类* 叶子任务 = nullptr;
    需求节点类* 来源需求 = nullptr;
    基础信息节点类* 目标宿主 = nullptr;
    特征节点类* 目标特征类型 = nullptr;
    方法类::节点类* 候选方法 = nullptr;
    std::uint32_t 最大深度 = 8;
    bool 允许未验证路径 = true;
};

struct 结构_叶子任务因果投影预判 {
    任务节点类* 叶子任务 = nullptr;
    需求节点类* 来源需求 = nullptr;
    基础信息节点类* 目标宿主 = nullptr;
    特征节点类* 目标特征类型 = nullptr;
    std::vector<结构_目标投影路径> 候选投影路径{};
    std::vector<枚举_目标权重类型> 候选权重类型集{};
    std::vector<std::string> 缺失证据{};
    bool 是否允许形成D0 = false;
    bool 是否允许进入结算候选 = false;
};

struct 结构_自检原子目标因果投影输入 {
    任务节点类* 固定来源任务 = nullptr;
    需求节点类* 固定来源需求 = nullptr;
    基础信息节点类* 目标宿主 = nullptr;
    特征节点类* 目标特征类型 = nullptr;
    const 语素入口节点类* 目标特征类型词 = nullptr;
    std::uint32_t 最大深度 = 8;
    bool 允许未验证路径 = true;
};

struct 结构_自检原子目标因果投影结果 {
    任务节点类* 固定来源任务 = nullptr;
    需求节点类* 固定来源需求 = nullptr;
    基础信息节点类* 目标宿主 = nullptr;
    特征节点类* 目标特征类型 = nullptr;
    const 语素入口节点类* 目标特征类型词 = nullptr;
    std::vector<结构_目标投影路径> 候选投影路径{};
    std::vector<枚举_目标投影维度> 目标维度集{};
    std::vector<枚举_目标权重类型> 候选权重类型集{};
    std::vector<std::string> 来源因果主键集{};
    std::vector<std::string> 缺失证据{};
    bool 是否有安全投影 = false;
    bool 是否有服务投影 = false;
    bool 是否仅维护投影 = false;
    bool 是否只有候选规则投影 = false;
};

class 因果类 {
public:
    explicit 因果类(基础信息类* 基础信息 = nullptr) noexcept;

    void 绑定基础信息(基础信息类* 基础信息) noexcept;

    因果主信息类* 取因果主信息(因果节点类* 节点) const noexcept;
    const 因果主信息类* 取因果主信息(const 因果节点类* 节点) const noexcept;

    因果实例主信息类* 取实例主信息(因果实例节点类* 节点) const noexcept;
    const 因果实例主信息类* 取实例主信息(const 因果实例节点类* 节点) const noexcept;

    因果模板主信息类* 取模板主信息(因果模板节点类* 节点) const noexcept;
    const 因果模板主信息类* 取模板主信息(const 因果模板节点类* 节点) const noexcept;

    因果模板节点类* 创建因果模板(基础信息节点类* 父节点, 因果主信息类* 主信息);
    因果实例节点类* 创建因果实例(基础信息节点类* 父节点, 因果主信息类* 主信息);

    // 兼容旧入口：按一个动态补全两层因果，并返回“动作致变关系”；若无动作，则返回“状态变化关系”。
    因果模板节点类* 按动态创建因果信息(
        场景节点类* 场景,
        动态节点类* 动态节点,
        基础信息节点类* 动作主体 = nullptr,
        基础信息节点类* 动作语义 = nullptr,
        const std::string& 动作语义键 = {},
        bool 设为主结果 = true);

    // 新主入口：同一条动态会生成 / 命中两种抽象因果，并把动态挂作证据样本。
    结构_两层因果构建结果 按动态补全两层因果(
        场景节点类* 场景,
        动态节点类* 动态节点,
        基础信息节点类* 动作主体 = nullptr,
        基础信息节点类* 动作语义 = nullptr,
        const std::string& 动作语义键 = {},
        bool 设为主结果 = true);

    std::vector<结构_两层因果构建结果> 按动态集合补全两层因果(
        场景节点类* 场景,
        const std::vector<动态节点类*>& 动态集合);

    bool 删除因果(因果节点类* 节点);

    std::vector<因果节点类*> 枚举全部因果() const;
    std::vector<因果实例节点类*> 枚举因果实例() const;
    std::vector<因果模板节点类*> 枚举因果模板() const;

    bool 按动态桥接结果状态(
        因果节点类* 节点,
        动态节点类* 动态节点,
        bool 设为主结果 = true);

    bool 追加条件模板(因果节点类* 节点, 特征节点类* 条件模板);
    bool 追加状态迁移模板(因果节点类* 节点, 二次特征节点类* 状态迁移模板, bool 设为主迁移 = false);
    bool 追加结果状态(因果节点类* 节点, 状态节点类* 结果状态, bool 设为主结果 = false);

    bool 绑定动作主体语义(
        因果节点类* 节点,
        基础信息节点类* 动作主体,
        基础信息节点类* 动作语义,
        const std::string& 动作语义键 = {}
    );

    bool 追加证据动态样本(因果模板节点类* 节点, 动态节点类* 证据动态);
    std::size_t 批量追加证据动态样本(
        const std::vector<因果模板节点类*>& 节点集合,
        动态节点类* 证据动态);
    bool 追加证据实例(因果模板节点类* 节点, 因果实例节点类* 证据实例);

    std::string 计算动态状态变化签名(const 动态节点类* 动态节点) const;
    std::string 计算动态动作致变签名(
        const 动态节点类* 动态节点,
        基础信息节点类* 动作主体 = nullptr,
        基础信息节点类* 动作语义 = nullptr,
        const std::string& 动作语义键 = {}) const;

    std::string 计算模板状态变化签名(const 因果模板节点类* 节点) const;
    std::string 计算模板动作致变签名(const 因果模板节点类* 节点) const;

    std::vector<因果模板节点类*> 查找状态变化关系模板(const std::string& 状态变化签名) const;
    std::vector<因果模板节点类*> 查找动作致变关系模板(const std::string& 动作致变签名) const;
    std::vector<因果模板节点类*> 查找导致状态变化的动作关系(const std::string& 状态变化签名) const;

    // 只读反推接口：
    // 根据目标结果状态，从已沉淀的“动作 + 状态变化”因果模板中查找自我可能使用的动作组合。
    // 本函数只返回候选，不写方法树、任务树或世界树；若候选可唯一映射到单步本能方法，
    // 后续仍必须由方法域提交逻辑决定是否把该本能 ID 写入目标方法动作句柄。
    结构_自我动作组合反推结果 反推自我动作组合(
        const 结构_自我动作组合反推输入& 输入) const;

    // 只读投影接口：
    // 从世界树根链上已有的动态、状态和因果模板中提取状态转换因果视图。
    // 本函数只返回普通结构体，不创建因果节点、不写任务树 / 需求树 / 世界树。
    结构_状态转换因果投影 生成状态转换因果投影(
        const 结构_状态转换因果投影输入& 输入 = {}) const;

    // 只读预判接口：
    // 在叶子任务业务动作 D0 生成前，尝试判断目标特征可能落入哪些目标投影、
    // 对应哪个权重类型和结算贡献层级。本函数只给出候选和缺失证据，不执行结算。
    结构_叶子任务因果投影预判 查询叶子任务权重类型与层级(
        const 结构_叶子任务因果投影输入& 输入) const;

    // 只读自检投影接口：
    // 自检固定来源任务是安全根任务；本函数不要求叶子任务存在，
    // 只按固定来源和原子目标查询状态转换因果投影、来源因果和缺失证据。
    结构_自检原子目标因果投影结果 查询自检原子目标投影(
        const 结构_自检原子目标因果投影输入& 输入) const;

    bool 二次特征匹配模板(const 二次特征节点类* 二次特征, const 二次特征节点类* 模板) const noexcept;

    bool 因果模板匹配二次特征(
        const 因果模板节点类* 模板,
        const std::vector<二次特征节点类*>& 条件候选,
        const std::vector<二次特征节点类*>& 结果候选) const noexcept;

    std::vector<因果模板节点类*> 查找匹配因果模板(
        枚举_因果锚点类型 锚点类型,
        std::uint32_t 状态层级,
        const std::vector<二次特征节点类*>& 条件候选,
        const std::vector<二次特征节点类*>& 结果候选) const;

    bool 生成实例因果表达单元(const 因果实例节点类* 节点, 结构_因果表达单元& 输出) const;
    bool 生成抽象因果表达单元(const 因果模板节点类* 节点, 结构_因果表达单元& 输出) const;

    std::string 生成实例因果自然语言(const 因果实例节点类* 节点) const;
    std::string 生成抽象因果自然语言(const 因果模板节点类* 节点) const;

private:
    基础信息类* 基础信息_ = nullptr;
};
