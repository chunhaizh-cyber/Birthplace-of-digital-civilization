#pragma once

#include <functional>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "基础信息类.h"

struct 特征比较明细 {
    const 语素入口节点类* 类型 = nullptr;
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

struct 结构_特征状态比较结果 {
    bool 可比较 = false;
    枚举_三向关系 关系 = 枚举_三向关系::等于;
    const 语素入口节点类* 特征类型 = nullptr;
    枚举_特征值比较模式 比较方式 = 枚举_特征值比较模式::相等;
    I64 差值 = 0;
    std::string 说明{};
};

enum class 枚举_VecU解释规则 : std::uint8_t {
    未定义 = 0,
    原始VecIU64 = 1,
    VecIU64_ZigZag_I64_二维坐标链 = 2,
    VecIU64_ZigZag_I64_三维坐标链 = 3,
};

struct 结构_VecU特征解释规则 {
    枚举_VecU解释规则 规则 = 枚举_VecU解释规则::未定义;
    std::uint32_t 坐标维度 = 0;
    bool 需要ZigZag解码 = false;

    bool 有效() const noexcept { return 规则 != 枚举_VecU解释规则::未定义; }
    bool 是坐标链() const noexcept { return 坐标维度 == 2 || 坐标维度 == 3; }
};

enum class 枚举_轮廓比较状态 : std::uint8_t {
    未定义 = 0,
    可比较 = 1,
    格式非法 = 2,
    维度不支持 = 3,
    点数量不一致 = 4,
    不可对齐 = 5,
};

struct 结构_轮廓比较结果 {
    枚举_轮廓比较状态 状态 = 枚举_轮廓比较状态::未定义;
    std::uint32_t 坐标维度 = 0;
    I64 左点数量 = 0;
    I64 右点数量 = 0;
    I64 点数量差异 = 0;
    I64 中心L1误差 = 0;
    I64 点链平均L1误差 = 0;
    I64 点链最大L1误差 = 0;
    I64 匹配评分Q10000 = 0;

    bool 可比较() const noexcept { return 状态 == 枚举_轮廓比较状态::可比较; }
};

struct 常用抽象特征初始化结果 {
    std::uint32_t 抽象特征数 = 0;
    std::uint32_t 区间状态数 = 0;
    bool 成功 = false;
};

class 特征类 {
public:
    using 特征集合 = std::vector<特征节点类*>;
    using 抽象特征集合 = std::vector<抽象特征节点类*>;
    using 差异度函数 = std::function<I64(const 语素入口节点类*, const 特征值&, const 特征值&)>;

    explicit 特征类(基础信息类* 基础信息 = nullptr) noexcept;

    void 绑定基础信息(基础信息类* 基础信息) noexcept;
    void 设置差异度函数(差异度函数 函数) noexcept;

    特征节点主信息类* 取特征主信息(特征节点类* 节点) const noexcept;
    const 特征节点主信息类* 取特征主信息(const 特征节点类* 节点) const noexcept;
    抽象特征主信息类* 取抽象特征主信息(抽象特征节点类* 节点) const noexcept;
    const 抽象特征主信息类* 取抽象特征主信息(const 抽象特征节点类* 节点) const noexcept;

    抽象特征节点类* 创建抽象特征(基础信息节点类* 父抽象特征, 抽象特征主信息类* 主信息);
    抽象特征节点类* 创建叶子抽象特征(
        基础信息节点类* 父抽象特征,
        const 语素入口节点类* 名称,
        const 语素入口节点类* 类型,
        特征值域 值域);
    抽象特征节点类* 创建复合抽象特征(
        基础信息节点类* 父抽象特征,
        const 语素入口节点类* 名称,
        const 语素入口节点类* 类型);
    抽象特征节点类* 创建抽象特征_按类型(
        基础信息节点类* 父抽象特征,
        const 语素入口节点类* 特征类型,
        std::optional<I64区间> 区间 = std::nullopt
    );
    抽象特征节点类* 查找子抽象特征_按类型(
        const 基础信息节点类* 父抽象特征,
        const 语素入口节点类* 特征类型) const;
    抽象特征节点类* 查找子抽象特征_按名称(
        const 基础信息节点类* 父抽象特征,
        const 语素入口节点类* 名称) const;
    bool 校验抽象特征子链(const 基础信息节点类* 抽象特征节点) const;
    抽象特征节点类* 取或创建子抽象特征_按类型(
        基础信息节点类* 父抽象特征,
        const 语素入口节点类* 特征类型);
    常用抽象特征初始化结果 初始化常用抽象特征(基础信息节点类* 抽象特征根);
    bool 常用抽象特征已初始化(const 基础信息节点类* 抽象特征根) const;

    特征节点类* 创建特征(基础信息节点类* 宿主, 特征节点主信息类* 主信息);
    特征节点类* 创建特征_按类型(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        const 特征值& 当前值 = {}
    );
    特征节点类* 取或创建子特征_按类型(基础信息节点类* 宿主, const 语素入口节点类* 特征类型);
    bool 删除特征(特征节点类* 节点);

    std::vector<特征节点类*> 枚举全部特征() const;
    std::vector<特征节点类*> 获取子特征(const 基础信息节点类* 宿主) const;
    特征节点类* 查找子特征_按类型(const 基础信息节点类* 宿主, const 语素入口节点类* 特征类型) const;
    特征节点类* 查找子特征_按名称(const 基础信息节点类* 宿主, const 语素入口节点类* 名称) const;
    特征值 读取子特征值_按类型(const 基础信息节点类* 宿主, const 语素入口节点类* 特征类型) const;
    bool 特征值命中值域(const 特征值& 值, const 特征值域& 值域) const;
    bool 匹配抽象特征(const 抽象特征节点类* 抽象特征, const 特征节点类* 实例特征) const;
    特征节点类* 查找实例特征_按抽象特征(
        const 基础信息节点类* 宿主,
        const 抽象特征节点类* 抽象特征) const;
    特征节点类* 取或创建实例特征_按抽象特征(
        基础信息节点类* 宿主,
        抽象特征节点类* 抽象特征);

    特征写入结果 写入特征值_按参数(特征节点类* 节点, const 特征值& 值, const 写入参数& 参数 = {});
    bool 写入特征值(特征节点类* 节点, const 特征值& 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征值_I64(特征节点类* 节点, I64 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征值_VecU句柄(特征节点类* 节点, VecU句柄 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 写入特征值_指针句柄(特征节点类* 节点, 指针句柄 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    特征值 读取特征值(const 特征节点类* 节点) const;
    std::vector<抽象特征节点类*> 枚举命中抽象状态(const 特征节点类* 实例特征) const;
    抽象特征节点类* 解析实例特征命中抽象特征(特征节点类* 实例特征);
    bool 设置实例特征局部区间覆盖(特征节点类* 实例特征, I64区间 区间);
    bool 清除实例特征局部区间覆盖(特征节点类* 实例特征);
    抽象特征节点类* 升格当前稳态为抽象区间(特征节点类* 实例特征);
    bool 合并抽象区间(抽象特征节点类* 目标抽象特征, const 抽象特征节点类* 来源抽象特征);
    bool 求值组合抽象特征(
        基础信息节点类* 宿主,
        抽象特征节点类* 组合抽象特征,
        时间戳 now = 结构体_时间戳::当前_微秒());
    枚举_特征值比较模式 获取比较方式(
        const 语素入口节点类* 特征类型,
        const 特征值& 左值,
        const 特征值& 右值) const noexcept;
    特征比较方法结果 获取比较方法结果(
        const 语素入口节点类* 特征类型,
        const 特征值& 左值,
        const 特征值& 右值,
        const 特征节点主信息类* 左特征主信息 = nullptr,
        const 特征节点主信息类* 右特征主信息 = nullptr) const;
    static 结构_VecU特征解释规则 VecU解释规则_按特征类型(const 语素入口节点类* 特征类型);
    static std::optional<std::uint32_t> 轮廓坐标维度_按特征类型(const 语素入口节点类* 特征类型);
    static bool 校验坐标链VecI64(const VecI64& 值, std::uint32_t 坐标维度) noexcept;
    static bool 校验平面轮廓VecI64(const VecI64& 值) noexcept;
    static bool 校验空间极值轮廓VecI64(const VecI64& 值) noexcept;
    static bool 校验坐标类VecI64_按特征类型(const 语素入口节点类* 特征类型, const VecI64& 值);
    static 结构_轮廓比较结果 比较坐标轮廓VecI64(
        const VecI64& 左值,
        const VecI64& 右值,
        std::uint32_t 坐标维度) noexcept;
    static 结构_轮廓比较结果 比较平面轮廓VecI64(const VecI64& 左值, const VecI64& 右值) noexcept;
    static 结构_轮廓比较结果 比较空间极值轮廓VecI64(const VecI64& 左值, const VecI64& 右值) noexcept;
    // 按特征类型选择平面或空间轮廓比较规则，未知类型返回不可比较状态。
    static 结构_轮廓比较结果 比较轮廓VecI64_按特征类型(
        const 语素入口节点类* 特征类型,
        const VecI64& 左值,
        const VecI64& 右值) noexcept;
    结构_特征状态比较结果 比较状态(
        const 状态节点类* 当前状态,
        const 状态节点类* 目标状态) const;
    存在比较结果 比较特征集合(const 特征集合& A, const 特征集合& B, const 集合比较参数& 参数 = {}) const;
    std::string 生成度量签名(const 特征节点类* 节点) const;

private:
    基础信息类* 基础信息_ = nullptr;
    差异度函数 差异度函数_{};
};
