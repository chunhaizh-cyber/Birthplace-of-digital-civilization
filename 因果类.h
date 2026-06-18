#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "基础信息类.h"
#include "方法类.h"

// 因果类的口径：
// 1) 因果主体是模板关系，即“状态变化关系”和“动作导致状态变化关系”。
// 2) 动态 / 运行结果 / 场景中的具体发生，只作为证据样本。
// 3) 因果树不强绑特征树；它只解释状态变化，并可投影为需求候选。

struct 结构_状态转换因果节点 {
    基础信息节点类* 宿主存在 = nullptr;
    特征节点类* 特征类型 = nullptr;
    状态节点类* 状态节点 = nullptr;
    动态节点类* 来源动态 = nullptr;
    因果模板节点类* 来源因果模板 = nullptr;
    场景节点类* 来源场景 = nullptr;
    时间戳 状态时间 = 0;
};

struct 结构_因果链查询输入 {
    状态节点类* 目标结果状态 = nullptr;
    特征节点类* 目标结果特征 = nullptr;
    std::uint32_t 最大深度 = 8;
    bool 包含未验证路径 = true;
};

struct 结构_因果链查询结果 {
    std::vector<结构_状态转换因果节点> 节点{};
    std::size_t 边数量 = 0;
    std::vector<因果模板节点类*> 来源因果模板{};
    std::vector<std::string> 缺失证据{};
};

struct 结构_叶子任务因果投影输入 {
    任务节点类* 叶子任务 = nullptr;
    需求节点类* 来源需求 = nullptr;
    基础信息节点类* 目标宿主 = nullptr;
    特征节点类* 目标特征类型 = nullptr;
    std::uint32_t 最大深度 = 8;
    bool 允许未验证路径 = true;
};

struct 结构_叶子任务因果投影预判 {
    std::size_t 候选投影数量 = 0;
    std::vector<std::uint32_t> 结算贡献层级集{};
    std::vector<std::uint32_t> 因果距离层级集{};
    std::vector<std::string> 缺失证据{};
    bool 是否允许形成D0 = false;
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
    std::size_t 候选投影数量 = 0;
    std::vector<std::uint32_t> 结算贡献层级集{};
    std::vector<std::uint32_t> 因果距离层级集{};
    std::vector<std::string> 来源因果主键集{};
    std::vector<std::string> 缺失证据{};
};

class 因果类 {
public:
    explicit 因果类(基础信息类* 基础信息 = nullptr) noexcept;

    void 绑定基础信息(基础信息类* 基础信息) noexcept;

    因果主信息类* 取因果主信息(因果节点类* 节点) const noexcept;
    const 因果主信息类* 取因果主信息(const 因果节点类* 节点) const noexcept;

    因果模板主信息类* 取模板主信息(因果模板节点类* 节点) const noexcept;
    const 因果模板主信息类* 取模板主信息(const 因果模板节点类* 节点) const noexcept;

    // 功能：按动态信息生成或命中因果信息；已有相同条件、动作和结果时只追加证据动态并返回现有因果。
    因果模板节点类* 生成因果信息(
        动态节点类* 动态信息,
        基础信息节点类* 动作主体 = nullptr,
        基础信息节点类* 动作语义 = nullptr,
        const std::string& 动作语义键 = {},
        bool 设为主结果 = true);

    // 功能：按动态信息和指定场景生成或命中因果信息；已有相同条件、动作和结果时只追加证据动态并返回现有因果。
    因果模板节点类* 生成因果信息(
        场景节点类* 场景,
        动态节点类* 动态信息,
        基础信息节点类* 动作主体 = nullptr,
        基础信息节点类* 动作语义 = nullptr,
        const std::string& 动作语义键 = {},
        bool 设为主结果 = true);

    std::vector<因果节点类*> 枚举全部因果() const;
    std::vector<因果模板节点类*> 枚举因果模板() const;

    // 功能：按条件特征查询抽象因果模板，不创建或修改因果节点。
    std::vector<因果模板节点类*> 查询条件相关因果(特征节点类* 条件特征) const;

    // 功能：按结果特征或结果状态查询抽象因果模板，不创建或修改因果节点。
    std::vector<因果模板节点类*> 查询结果相关因果(
        特征节点类* 结果特征,
        状态节点类* 结果状态 = nullptr) const;

    // 功能：按方法模板或动作语义键查询抽象因果模板，不创建或修改因果节点。
    std::vector<因果模板节点类*> 查询动作相关因果(
        const 方法节点类* 因方法,
        const std::string& 动作语义键 = {}) const;

    // 只读链路查询接口：
    // 从目标结果状态或目标结果特征回溯现有状态转换因果边，只返回查询视图，不写世界树。
    结构_因果链查询结果 查询因果链(
        const 结构_因果链查询输入& 输入) const;

    // 只读预判接口：
    // 在叶子任务业务动作 D0 生成前，尝试判断目标特征可能落入哪些目标投影、
    // 对应哪个结算贡献层级。本函数只给出候选数量、层级集和缺失证据，不执行结算。
    结构_叶子任务因果投影预判 查询叶子任务目标投影层级(
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

    // 功能：把因果模板转换为基础命名显示使用的自然语言。
    std::string 生成抽象因果自然语言(const 因果模板节点类* 节点) const;

private:
    // 功能：创建内部因果模板节点；外部新增因果信息统一使用生成因果信息。
    因果模板节点类* 创建因果模板(基础信息节点类* 父节点, 因果主信息类* 主信息);

    // 功能：为生成因果信息命中的现有模板追加证据动态。
    bool 追加证据动态样本(因果模板节点类* 节点, 动态节点类* 证据动态);

    // 功能：内部生成或命中因果模板，并返回公开入口使用的首选模板。
    因果模板节点类* 私有_生成因果信息并返回模板(
        场景节点类* 场景,
        动态节点类* 动态节点,
        基础信息节点类* 动作主体 = nullptr,
        基础信息节点类* 动作语义 = nullptr,
        const std::string& 动作语义键 = {},
        bool 设为主结果 = true);

    基础信息类* 基础信息_ = nullptr;
};
