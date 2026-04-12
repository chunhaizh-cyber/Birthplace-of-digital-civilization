#pragma once

#include <string>

#include "基础信息类.h"
#include "特征值类.h"
#include "特征类.h"
#include "存在类.h"
#include "场景类.h"
#include "二次特征类.h"
#include "二次特征生成模块.h"
#include "状态类.h"
#include "动态类.h"
#include "因果类.h"

class 世界树类 {
public:
    世界树类();
    ~世界树类();

    世界树类(const 世界树类&) = delete;
    世界树类& operator=(const 世界树类&) = delete;
    世界树类(世界树类&&) = delete;
    世界树类& operator=(世界树类&&) = delete;

    void 初始化();
    void 初始化默认世界();

    基础信息类& 基础信息() noexcept;
    const 基础信息类& 基础信息() const noexcept;
    特征值类& 值池() noexcept;
    const 特征值类& 值池() const noexcept;
    特征类& 特征() noexcept;
    const 特征类& 特征() const noexcept;
    存在类& 存在() noexcept;
    const 存在类& 存在() const noexcept;
    场景类& 场景() noexcept;
    const 场景类& 场景() const noexcept;
    二次特征类& 二次特征() noexcept;
    const 二次特征类& 二次特征() const noexcept;
    二次特征生成模块& 二次特征生成() noexcept;
    const 二次特征生成模块& 二次特征生成() const noexcept;
    状态类& 状态() noexcept;
    const 状态类& 状态() const noexcept;
    动态类& 动态() noexcept;
    const 动态类& 动态() const noexcept;
    因果类& 因果() noexcept;
    const 因果类& 因果() const noexcept;

    基础信息节点类* 世界根() const noexcept;
    std::vector<基础信息节点类*> 获取子节点(const 基础信息节点类* 父节点) const;
    std::string 获取名称(const 基础信息节点类* 节点) const;

    特征节点类* 创建特征(基础信息节点类* 宿主, 特征节点主信息类* 主信息);
    特征节点类* 取或创建子特征_按类型(基础信息节点类* 宿主, const 词性节点类* 特征类型);
    存在节点类* 创建存在(基础信息节点类* 父节点, 存在节点主信息类* 主信息);
    场景节点类* 创建场景(基础信息节点类* 父节点, 场景节点主信息类* 主信息);
    二次特征节点类* 创建二次特征(基础信息节点类* 父节点, 二次特征主信息类* 主信息);
    状态节点类* 创建状态(场景节点类* 场景, 状态节点主信息类* 主信息);
    动态节点类* 创建动态(场景节点类* 场景, 动态节点主信息类* 主信息);
    因果实例节点类* 创建因果实例(场景节点类* 场景, 因果实例主信息类* 主信息);
    因果模板节点类* 创建因果模板(基础信息节点类* 父节点, 因果模板主信息类* 主信息);

    bool 写入特征_I64(特征节点类* 节点, I64 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征_指针(特征节点类* 节点, const void* 指针, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征_VecU(
        特征节点类* 节点,
        VecIU64 值,
        枚举_轮廓维度 维度 = 枚举_轮廓维度::未定义,
        时间戳 now = 结构体_时间戳::当前_微秒()
    );
    特征值 读取特征快照(const 特征节点类* 节点) const;
    bool 读取特征_I64(const 特征节点类* 节点, I64& 输出值) const;
    const VecIU64* 读取特征VecU(const 特征节点类* 节点) const;

    场景节点类* 取或创建默认世界(const std::string& 名称, 枚举_世界类型 世界类型);
    场景节点类* 取或创建自我现实场景();
    存在节点类* 取或创建自我存在();
    场景节点类* 取存在内部世界(const 存在节点类* 宿主存在) const noexcept;
    场景节点类* 取或创建存在内部世界(存在节点类* 宿主存在, const 词性节点类* 场景名称 = nullptr);
    bool 写入存在观测位置(存在节点类* 节点, const Vector3D& 位置_mm);

public:
    场景节点类* 现实世界 = nullptr;
    场景节点类* 文本世界 = nullptr;
    场景节点类* 想象世界 = nullptr;
    场景节点类* 记忆世界 = nullptr;
    场景节点类* 推理世界 = nullptr;
    场景节点类* 虚拟世界 = nullptr;
    存在节点类* 自我指针 = nullptr;
    场景节点类* 自我所在场景 = nullptr;

private:
    基础信息类 基础信息链_{};
    特征值类 值池_{};
    特征类 特征服务_;
    存在类 存在服务_;
    场景类 场景服务_;
    二次特征类 二次特征服务_;
    二次特征生成模块 二次特征生成服务_;
    状态类 状态服务_;
    动态类 动态服务_;
    因果类 因果服务_;
};

extern 世界树类 世界树;

