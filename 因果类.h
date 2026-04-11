#pragma once

#include <string>
#include <vector>

#include "基础信息类.h"

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

class 因果类 {
public:
    explicit 因果类(基础信息类* 基础信息 = nullptr) noexcept;

    void 绑定基础信息(基础信息类* 基础信息) noexcept;

    因果实例主信息类* 取实例主信息(因果实例节点类* 节点) const noexcept;
    const 因果实例主信息类* 取实例主信息(const 因果实例节点类* 节点) const noexcept;
    因果模板主信息类* 取模板主信息(因果模板节点类* 节点) const noexcept;
    const 因果模板主信息类* 取模板主信息(const 因果模板节点类* 节点) const noexcept;

    因果实例节点类* 创建因果实例(场景节点类* 场景, 因果实例主信息类* 主信息);
    因果模板节点类* 创建因果模板(基础信息节点类* 父节点, 因果模板主信息类* 主信息);
    bool 删除因果(因果节点类* 节点);

    std::vector<因果节点类*> 枚举全部因果() const;
    std::vector<因果实例节点类*> 枚举因果实例() const;
    std::vector<因果模板节点类*> 枚举因果模板() const;
    std::vector<因果实例节点类*> 获取场景因果实例(const 场景节点类* 场景) const;

    bool 按动态桥接结果状态(
        因果实例节点类* 节点,
        动态节点类* 动态节点,
        bool 设为主结果 = true);
    bool 追加条件状态(因果实例节点类* 节点, 状态节点类* 条件状态);
    bool 追加结果状态(因果实例节点类* 节点, 状态节点类* 结果状态, bool 设为主结果 = false);
    bool 绑定动作主体语义(
        因果实例节点类* 节点,
        基础信息节点类* 动作主体,
        基础信息节点类* 动作语义,
        const std::string& 动作语义键 = {}
    );
    bool 追加证据实例(因果模板节点类* 节点, 因果实例节点类* 证据实例);
    bool 追加条件模板(因果模板节点类* 节点, 二次特征节点类* 条件模板);
    bool 追加结果模板(因果模板节点类* 节点, 二次特征节点类* 结果模板);

    bool 生成实例因果表达单元(const 因果实例节点类* 节点, 结构_因果表达单元& 输出) const;
    bool 生成抽象因果表达单元(const 因果模板节点类* 节点, 结构_因果表达单元& 输出) const;
    std::string 生成实例因果自然语言(const 因果实例节点类* 节点) const;
    std::string 生成抽象因果自然语言(const 因果模板节点类* 节点) const;

private:
    基础信息类* 基础信息_ = nullptr;
};
