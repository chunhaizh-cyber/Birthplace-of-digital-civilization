#pragma once

#include <vector>

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
    bool 是存在节点(const 基础信息节点类* 节点) const noexcept;
    const 语素入口节点类* 读取存在名称(const 存在节点类* 节点) const noexcept;
    const 语素入口节点类* 读取存在类型(const 存在节点类* 节点) const noexcept;
    场景节点类* 读取内部世界(const 存在节点类* 节点) const noexcept;
    std::vector<可解析引用<存在节点类>> 读取概念集快照(const 存在节点类* 节点) const;
    可解析引用<存在节点类> 读取概念模板引用(const 存在节点类* 节点) const noexcept;
    bool 读取最近观测位置缓存(const 存在节点类* 节点, Vector3D& 输出位置_mm) const noexcept;
    bool 读取上次观测位置缓存(const 存在节点类* 节点, Vector3D& 输出位置_mm) const noexcept;
    bool 确保存在三类根节点(存在节点类* 节点) const noexcept;
    需求类::节点类* 获取需求根节点(存在节点类* 节点) const noexcept;
    const 需求类::节点类* 获取需求根节点(const 存在节点类* 节点) const noexcept;
    任务类::节点类* 获取任务根节点(存在节点类* 节点) const noexcept;
    const 任务类::节点类* 获取任务根节点(const 存在节点类* 节点) const noexcept;
    方法类::节点类* 获取方法根节点(存在节点类* 节点) const noexcept;
    const 方法类::节点类* 获取方法根节点(const 存在节点类* 节点) const noexcept;

    存在节点类* 创建存在(基础信息节点类* 父节点, 存在节点主信息类* 主信息);
    存在节点类* 创建存在_按名称类型(
        基础信息节点类* 父节点,
        const 语素入口节点类* 名称 = nullptr,
        const 语素入口节点类* 类型 = nullptr
    );
    存在节点类* 取或创建子存在_按名称类型(
        基础信息节点类* 父节点,
        const 语素入口节点类* 名称 = nullptr,
        const 语素入口节点类* 类型 = nullptr
    );
    存在节点类* 取或创建子存在_按类型(
        基础信息节点类* 父节点,
        const 语素入口节点类* 类型
    );
    存在节点类* 取或创建子存在_按类型并按特征I64(
        基础信息节点类* 父节点,
        const 语素入口节点类* 类型,
        const 语素入口节点类* 唯一键特征类型,
        I64 唯一键特征值
    );
    存在节点类* 确保存在概念(
        基础信息节点类* 概念父节点,
        const 语素入口节点类* 名称
    );
    存在节点类* 创建存在实例(
        基础信息节点类* 父节点,
        const 语素入口节点类* 名称,
        const std::vector<存在节点类*>& 概念集
    );
    bool 删除存在(存在节点类* 节点);

    std::vector<存在节点类*> 枚举全部存在() const;
    std::vector<存在节点类*> 获取子存在(const 基础信息节点类* 父节点) const;
    存在节点类* 查找子存在_按名称(const 基础信息节点类* 父节点, const 语素入口节点类* 名称) const;
    存在节点类* 查找子存在_按类型(const 基础信息节点类* 父节点, const 语素入口节点类* 类型) const;
    std::vector<存在节点类*> 获取概念集(const 存在节点类* 节点) const;
    bool 存在包含概念(const 存在节点类* 实例, const 存在节点类* 概念) const noexcept;
    std::vector<存在节点类*> 查找实例_按概念(
        const 存在节点类* 概念,
        const 基础信息节点类* 范围根 = nullptr
    ) const;

    特征节点类* 创建特征_按类型(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        const 特征值& 当前值 = {}
    );
    std::vector<特征节点类*> 获取子特征(const 基础信息节点类* 宿主) const;
    特征节点类* 查找子特征_按类型(const 基础信息节点类* 宿主, const 语素入口节点类* 特征类型) const;

    bool 写入名称(存在节点类* 节点, const 语素入口节点类* 名称);
    bool 写入类型(存在节点类* 节点, const 语素入口节点类* 类型);
    bool 绑定概念模板(存在节点类* 节点, 存在节点类* 概念模板);
    bool 添加实例概念(
        存在节点类* 实例,
        存在节点类* 概念,
        bool 是否扩充记忆 = true
    );
    void 根据概念扩充实例记忆(
        存在节点类* 实例,
        const std::vector<存在节点类*>& 概念集
    );
    bool 绑定内部世界(存在节点类* 节点, 场景节点类* 内部世界);
    // 仅更新存在主信息中的最近观测缓存；正式治理判断应读取空间特征 / 二次特征 / 动态证据。
    bool 写入观测位置(存在节点类* 节点, const Vector3D& 位置_mm);
    bool 标记观测未命中(存在节点类* 节点);

private:
    基础信息类* 基础信息_ = nullptr;
};
