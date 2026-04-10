#pragma once

#include "基础信息类.h"
#include "需求类.h"
#include "任务类.h"
#include "方法类.h"

class 存在类 {
public:
    explicit 存在类(基础信息类* 基础信息 = nullptr) noexcept;

    void 绑定基础信息(基础信息类* 基础信息) noexcept;

    存在节点主信息类* 取存在主信息(存在节点类* 节点) const noexcept;
    const 存在节点主信息类* 取存在主信息(const 存在节点类* 节点) const noexcept;
    需求类::节点类* 获取需求根节点(存在节点类* 节点) const noexcept;
    const 需求类::节点类* 获取需求根节点(const 存在节点类* 节点) const noexcept;
    任务类::节点类* 获取任务根节点(存在节点类* 节点) const noexcept;
    const 任务类::节点类* 获取任务根节点(const 存在节点类* 节点) const noexcept;
    方法类::节点类* 获取方法根节点(存在节点类* 节点) const noexcept;
    const 方法类::节点类* 获取方法根节点(const 存在节点类* 节点) const noexcept;

    存在节点类* 创建存在(基础信息节点类* 父节点, 存在节点主信息类* 主信息);
    存在节点类* 创建存在_按名称类型(
        基础信息节点类* 父节点,
        const 词性节点类* 名称 = nullptr,
        const 词性节点类* 类型 = nullptr
    );
    存在节点类* 取或创建子存在_按名称类型(
        基础信息节点类* 父节点,
        const 词性节点类* 名称 = nullptr,
        const 词性节点类* 类型 = nullptr
    );
    bool 删除存在(存在节点类* 节点);

    std::vector<存在节点类*> 枚举全部存在() const;
    std::vector<存在节点类*> 获取子存在(const 基础信息节点类* 父节点) const;
    存在节点类* 查找子存在_按名称(const 基础信息节点类* 父节点, const 词性节点类* 名称) const;
    存在节点类* 查找子存在_按类型(const 基础信息节点类* 父节点, const 词性节点类* 类型) const;

    特征节点类* 创建特征_按类型(
        基础信息节点类* 宿主,
        const 词性节点类* 特征类型,
        const 特征值& 当前值 = {}
    );
    std::vector<特征节点类*> 获取子特征(const 基础信息节点类* 宿主) const;
    特征节点类* 查找子特征_按类型(const 基础信息节点类* 宿主, const 词性节点类* 特征类型) const;

    bool 写入名称(存在节点类* 节点, const 词性节点类* 名称);
    bool 写入类型(存在节点类* 节点, const 词性节点类* 类型);
    bool 绑定概念模板(存在节点类* 节点, 存在节点类* 概念模板);
    bool 绑定内部世界(存在节点类* 节点, 场景节点类* 内部世界);
    bool 追加实例因果(存在节点类* 节点, 因果实例节点类* 因果实例);

private:
    基础信息类* 基础信息_ = nullptr;
};
