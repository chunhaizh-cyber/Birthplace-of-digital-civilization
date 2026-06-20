#pragma once

#include <vector>

#include "基础信息类.h"
#include "需求类.h"
#include "任务类.h"
#include "方法类.h"

struct 结构_存在空间相对坐标项 {
    const 存在节点类* 参考存在 = nullptr;
    Vector3D 相对坐标_mm{};
    I64 置信度 = 10000;
};

struct 结构_存在空间绝对坐标换算结果 {
    bool 成功 = false;
    Vector3D 绝对坐标_mm{};
    const 存在节点类* 参考存在 = nullptr;
    I64 使用相对坐标项数量 = 0;
    I64 置信度 = 0;
};

class 存在类 {
public:
    explicit 存在类(基础信息类* 基础信息 = nullptr) noexcept;

    void 绑定基础信息(基础信息类* 基础信息) noexcept;

    存在节点主信息类* 取存在主信息(存在节点类* 节点) const noexcept;
    const 存在节点主信息类* 取存在主信息(const 存在节点类* 节点) const noexcept;
    bool 是存在节点(const 基础信息节点类* 节点) const noexcept;
    bool 是存在概念(const 存在节点类* 节点, const 存在节点类* 存在概念根) const noexcept;
    bool 是实例存在(const 存在节点类* 节点, const 存在节点类* 存在概念根) const noexcept;
    const 语素入口节点类* 读取存在名称(const 存在节点类* 节点) const noexcept;
    const 语素入口节点类* 读取存在类型(const 存在节点类* 节点) const noexcept;
    场景节点类* 读取内部世界(const 存在节点类* 节点) const noexcept;
    std::vector<可解析引用<存在节点类>> 读取概念集快照(const 存在节点类* 节点) const;
    可解析引用<存在节点类> 读取概念模板引用(const 存在节点类* 节点) const noexcept;
    bool 读取存在场景绝对坐标(const 存在节点类* 节点, Vector3D& 输出坐标_mm) const;
    bool 计算场景绝对坐标_由参考存在相对坐标(
        const 存在节点类* 参考存在,
        const Vector3D& 相对坐标_mm,
        Vector3D& 输出绝对坐标_mm) const;
    结构_存在空间绝对坐标换算结果 计算场景绝对坐标_由相对坐标表(
        const std::vector<结构_存在空间相对坐标项>& 相对坐标表) const;
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

    bool 写入名称(存在节点类* 节点, const 语素入口节点类* 名称);
    bool 写入类型(存在节点类* 节点, const 语素入口节点类* 类型);
    bool 绑定概念模板(存在节点类* 节点, 存在节点类* 概念模板);
    bool 添加实例概念(
        存在节点类* 实例,
        存在节点类* 概念,
        bool 是否扩充记忆 = true
    );
    bool 绑定内部世界(存在节点类* 节点, 场景节点类* 内部世界);
    // 仅更新存在主信息中的最近观测缓存；正式治理判断应读取空间特征 / 二次特征 / 动态证据。
    bool 写入观测位置(存在节点类* 节点, const Vector3D& 位置_mm);
    bool 写入存在场景绝对坐标(
        存在节点类* 节点,
        const Vector3D& 坐标_mm,
        时间戳 now,
        I64 置信度);
    bool 写入存在场景绝对坐标_由参考存在相对坐标(
        存在节点类* 目标存在,
        const 存在节点类* 参考存在,
        const Vector3D& 相对坐标_mm,
        时间戳 now,
        I64 置信度);
    bool 写入存在场景绝对坐标_由相对坐标表(
        存在节点类* 目标存在,
        const std::vector<结构_存在空间相对坐标项>& 相对坐标表,
        时间戳 now);
    bool 标记观测未命中(存在节点类* 节点);

private:
    基础信息类* 基础信息_ = nullptr;
};
