#pragma once

#include <functional>
#include <string>
#include <vector>

#include "基础信息类.h"

struct 特征比较明细 {
    const 词性节点类* 类型 = nullptr;
    I64 差异度 = 0;
    I64 权重 = 10000;
    bool 缺失A = false;
    bool 缺失B = false;
};

struct 存在比较结果 {
    I64 总差异度 = 0;
    I64 归一化差异度 = 0;
    int 命中特征数 = 0;
    std::vector<特征比较明细> 明细{};
};

struct 稳态策略 {
    int 连续N = 5;
    I64 阈值_判稳 = 0;
    I64 阈值_命中原型 = 0;
    int Kmax_points = 32;
    时间戳 淘汰_超时 = 0;
};

struct 写入参数 {
    时间戳 ts = 0;
    bool 仅更新当前值 = false;
    bool 允许创建稳态 = true;
    bool 允许淘汰 = true;
    稳态策略 稳态{};
};

struct 特征写入结果 {
    特征节点类* 特征 = nullptr;
    bool 通过候选门 = false;
    bool 稳态发生变化 = false;
};

struct 集合比较参数 {
    bool 输出明细 = true;
    I64 缺失惩罚 = 10000;
    bool 归一化 = true;
};

struct 特征比较方法结果 {
    bool 可比较 = false;
    枚举_特征值比较模式 比较方式 = 枚举_特征值比较模式::相等;
    I64 左比较值 = 0;
    I64 右比较值 = 0;
    I64 差值 = 0;
};

class 特征类 {
public:
    using 特征集合 = std::vector<特征节点类*>;
    using 差异度函数 = std::function<I64(const 词性节点类*, const 特征值&, const 特征值&)>;

    explicit 特征类(基础信息类* 基础信息 = nullptr) noexcept;

    void 绑定基础信息(基础信息类* 基础信息) noexcept;
    void 设置差异度函数(差异度函数 函数) noexcept;

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

    特征写入结果 写入特征值_按参数(特征节点类* 节点, const 特征值& 值, const 写入参数& 参数 = {});
    bool 写入特征值(特征节点类* 节点, const 特征值& 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征值_I64(特征节点类* 节点, I64 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征值_VecU句柄(特征节点类* 节点, VecU句柄 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征值_指针句柄(特征节点类* 节点, 指针句柄 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    特征值 读取特征值(const 特征节点类* 节点) const;
    枚举_特征值比较模式 获取比较方式(
        const 词性节点类* 特征类型,
        const 特征值& 左值,
        const 特征值& 右值) const noexcept;
    特征比较方法结果 获取比较方法结果(
        const 词性节点类* 特征类型,
        const 特征值& 左值,
        const 特征值& 右值,
        const 特征节点主信息类* 左特征主信息 = nullptr,
        const 特征节点主信息类* 右特征主信息 = nullptr) const;
    存在比较结果 比较特征集合(const 特征集合& A, const 特征集合& B, const 集合比较参数& 参数 = {}) const;
    std::string 生成度量签名(const 特征节点类* 节点) const;

private:
    基础信息类* 基础信息_ = nullptr;
    差异度函数 差异度函数_{};
};
