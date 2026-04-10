#pragma once

#include "基础信息类.h"

class 特征类 {
public:
    explicit 特征类(基础信息类* 基础信息 = nullptr) noexcept;

    void 绑定基础信息(基础信息类* 基础信息) noexcept;

    特征节点主信息类* 取特征主信息(特征节点类* 节点) const noexcept;
    const 特征节点主信息类* 取特征主信息(const 特征节点类* 节点) const noexcept;

    特征节点类* 创建特征(基础信息节点类* 宿主, 特征节点主信息类* 主信息);
    特征节点类* 创建特征_按类型(
        基础信息节点类* 宿主,
        const 词性节点类* 特征类型,
        const 特征值& 当前值 = {}
    );
    特征节点类* 取或创建子特征_按类型(基础信息节点类* 宿主, const 词性节点类* 特征类型);
    bool 删除特征(特征节点类* 节点);

    std::vector<特征节点类*> 枚举全部特征() const;
    std::vector<特征节点类*> 获取子特征(const 基础信息节点类* 宿主) const;
    特征节点类* 查找子特征_按类型(const 基础信息节点类* 宿主, const 词性节点类* 特征类型) const;

    bool 写入特征值(特征节点类* 节点, const 特征值& 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征值_I64(特征节点类* 节点, I64 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征值_VecU句柄(特征节点类* 节点, VecU句柄 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征值_指针句柄(特征节点类* 节点, 指针句柄 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    特征值 读取特征值(const 特征节点类* 节点) const;

private:
    基础信息类* 基础信息_ = nullptr;
};
