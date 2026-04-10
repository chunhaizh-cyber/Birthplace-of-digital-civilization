#pragma once

#include "基础信息类.h"

class 状态类 {
public:
    explicit 状态类(基础信息类* 基础信息 = nullptr) noexcept;

    void 绑定基础信息(基础信息类* 基础信息) noexcept;

    状态节点主信息类* 取状态主信息(状态节点类* 节点) const noexcept;
    const 状态节点主信息类* 取状态主信息(const 状态节点类* 节点) const noexcept;

    状态节点类* 创建状态(场景节点类* 场景, 状态节点主信息类* 主信息);
    状态节点类* 创建状态(
        场景节点类* 场景,
        基础信息节点类* 主体,
        特征节点类* 特征,
        const 特征值& 状态值,
        时间戳 发生时间 = 结构体_时间戳::当前_微秒(),
        时间戳 收到时间 = 结构体_时间戳::当前_微秒()
    );
    bool 删除状态(状态节点类* 节点);

    std::vector<状态节点类*> 枚举全部状态() const;
    std::vector<状态节点类*> 获取场景状态(const 场景节点类* 场景) const;

    bool 绑定主体特征(状态节点类* 节点, 基础信息节点类* 主体, 特征节点类* 特征);
    bool 写入状态值(状态节点类* 节点, const 特征值& 值, 时间戳 发生时间 = 0, 时间戳 收到时间 = 0);
    bool 标记变化(
        状态节点类* 节点,
        bool 是否变化,
        const std::string& 原因类别 = {},
        const std::string& 原因说明 = {}
    );

private:
    static 枚举_状态域 私有_推断状态域(const 基础信息节点类* 主体) noexcept;

private:
    基础信息类* 基础信息_ = nullptr;
};
