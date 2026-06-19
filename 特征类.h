#pragma once

#include <functional>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "基础信息类.h"

class 特征值类;

struct 存在比较结果 {
    I64 总差异度 = 0;
    I64 归一化差异度 = 0;
    int 命中特征数 = 0;
    int 比较特征数 = 0;
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
    VecIU64_三维体素_4x4x4顶层二分层 = 4,
};

struct 结构_VecU特征解释规则 {
    枚举_VecU解释规则 规则 = 枚举_VecU解释规则::未定义;
    std::uint32_t 坐标维度 = 0;
    bool 需要ZigZag解码 = false;

    // 功能：按函数名执行对应处理。
    bool 有效() const noexcept { return 规则 != 枚举_VecU解释规则::未定义; }

    // 功能：按函数名执行对应处理。
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

    // 功能：比较当前值、目标值或历史基准。
    bool 可比较() const noexcept { return 状态 == 枚举_轮廓比较状态::可比较; }
};

struct 结构_三维体素二分层摘要 {
    bool 有效 = false;
    std::uint32_t 顶层边长 = 4;
    std::uint32_t 细分层数 = 0;
    std::uint64_t 最终边长 = 0;
    std::uint32_t 原始宽度 = 0;
    std::uint32_t 原始高度 = 0;
    std::uint32_t 原始深度 = 0;
    bool 来源为非立方体 = false;
    std::uint64_t 最小体素边长_mm = 0;
    std::uint64_t 节点数量 = 0;
    std::uint64_t 占据体素数量 = 0;
};

enum class 枚举_三维体素链节点状态 : std::uint8_t {
    空 = 0,
    满 = 1,
    混合 = 2,
};

struct 结构_三维体素链节点信息 {
    bool 有效 = false;
    bool 是根 = false;
    std::uint32_t 层级 = 0;
    std::uint32_t 最大层级 = 0;
    std::uint32_t x = 0;
    std::uint32_t y = 0;
    std::uint32_t z = 0;
    std::uint32_t 边长 = 0;
    枚举_三维体素链节点状态 状态 = 枚举_三维体素链节点状态::空;
    std::uint64_t 占据体素数量 = 0;
    std::uint32_t 最小体素边长_mm = 0;
    std::uint32_t 原始宽度 = 0;
    std::uint32_t 原始高度 = 0;
    std::uint32_t 原始深度 = 0;
};

struct 结构_三维体素链写入结果 {
    bool 成功 = false;
    VecU句柄 根句柄{};
    std::vector<VecU句柄> 叶子句柄{};
    结构_三维体素二分层摘要 摘要{};
};

struct 结构_三维体素链查询结果 {
    bool 命中 = false;
    VecU句柄 节点句柄{};
    结构_三维体素链节点信息 节点{};
};

struct 结构_三维体素空间点毫米 {
    I64 x = 0;
    I64 y = 0;
    I64 z = 0;
};

struct 结构_三维体素轮廓图视角 {
    std::uint32_t 宽度 = 0;
    std::uint32_t 高度 = 0;
    std::uint32_t 视角序号 = 0;
    std::vector<std::uint8_t> 轮廓掩码{};
    std::vector<I64> 深度_mm{};
    std::vector<结构_三维体素空间点毫米> 空间点_mm{};
    std::vector<std::uint32_t> 颜色_RGBA{};
};

struct 结构_三维体素轮廓融合参数 {
    std::uint32_t 体素边长_mm = 0;
    std::uint32_t 互补最大间隔体素 = 1;
    std::uint32_t 最小确认视角数 = 1;
    std::uint64_t 最大输出体素数 = 2000000;
};

struct 结构_三维体素颜色项 {
    std::uint32_t x = 0;
    std::uint32_t y = 0;
    std::uint32_t z = 0;
    std::uint32_t 颜色_RGBA = 0;
};

struct 结构_三维体素轮廓融合结果 {
    bool 成功 = false;
    std::string 失败原因{};
    VecIU64 占据位块{};
    std::vector<结构_三维体素颜色项> 体素颜色{};
    std::uint32_t 宽度 = 0;
    std::uint32_t 高度 = 0;
    std::uint32_t 深度 = 0;
    std::uint32_t 体素边长_mm = 0;
    I64 原点X_mm = 0;
    I64 原点Y_mm = 0;
    I64 原点Z_mm = 0;
    std::uint64_t 输入视角数量 = 0;
    std::uint64_t 有效像素数量 = 0;
    std::uint64_t 原始命中体素数量 = 0;
    std::uint64_t 互补补全体素数量 = 0;
    std::uint64_t 占据体素数量 = 0;
    std::uint64_t 颜色体素数量 = 0;
};

struct 结构_三维体素存在空间绑定参数 {
    I64 原点X_mm = 0;
    I64 原点Y_mm = 0;
    I64 原点Z_mm = 0;
    std::uint32_t 最小体素边长_mm = 0;
};

struct 结构_三维体素局部轮廓相似度结果 {
    bool 可比较 = false;
    std::string 不可比较原因{};
    I64 相似度Q10000 = 0;
    I64 命中率Q10000 = 0;
    I64 越界惩罚Q10000 = 0;
    std::uint32_t 请求查询层级 = 0;
    std::uint32_t 实际查询层级 = 0;
    std::uint32_t 存在体素网格边长 = 0;
    std::uint32_t 存在最小体素边长_mm = 0;
    std::uint32_t 局部体素边长_mm = 0;
    std::uint64_t 局部占据体素数量 = 0;
    std::uint64_t 命中体素数量 = 0;
    std::uint64_t 满节点命中数量 = 0;
    std::uint64_t 混合节点命中数量 = 0;
    std::uint64_t 空体素冲突数量 = 0;
    std::uint64_t 隐式空冲突数量 = 0;
    std::uint64_t 越界体素数量 = 0;
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

    // 功能：按主键查找并解析特征节点，不创建或修改节点。
    特征节点类* 按主键解析特征节点(const std::string& 主键) const noexcept;

    // 功能：读取特征语义类型，不创建或修改节点。
    const 语素入口节点类* 读取特征语义类型(const 特征节点类* 特征) const noexcept;

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
    bool 写入三维体素特征值(特征节点类* 节点, VecU句柄 值, 时间戳 now = 结构体_时间戳::当前_微秒());
    bool 读取三维体素特征值(const 特征节点类* 节点, VecU句柄& 输出值) const;
    bool 清空三维体素特征值(特征节点类* 节点, 时间戳 now = 结构体_时间戳::当前_微秒());
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
    static std::uint64_t 三维体素最终边长(std::uint32_t 细分层数) noexcept;
    static std::optional<std::uint32_t> 三维体素包围立方体细分层数(
        std::uint32_t 宽度,
        std::uint32_t 高度,
        std::uint32_t 深度) noexcept;
    static std::uint64_t 三维体素占据位块数量(std::uint32_t 细分层数) noexcept;
    static std::uint64_t 三维体素长方体占据位块数量(
        std::uint32_t 宽度,
        std::uint32_t 高度,
        std::uint32_t 深度) noexcept;
    static VecIU64 创建三维体素占据位块(std::uint32_t 细分层数);
    static VecIU64 创建三维体素长方体占据位块(
        std::uint32_t 宽度,
        std::uint32_t 高度,
        std::uint32_t 深度);
    static bool 读取三维体素占据位(
        const VecIU64& 占据位块,
        std::uint32_t 细分层数,
        std::uint32_t x,
        std::uint32_t y,
        std::uint32_t z) noexcept;
    static bool 写入三维体素占据位(
        VecIU64& 占据位块,
        std::uint32_t 细分层数,
        std::uint32_t x,
        std::uint32_t y,
        std::uint32_t z,
        bool 有存在) noexcept;
    static bool 读取三维体素长方体占据位(
        const VecIU64& 占据位块,
        std::uint32_t 宽度,
        std::uint32_t 高度,
        std::uint32_t 深度,
        std::uint32_t x,
        std::uint32_t y,
        std::uint32_t z) noexcept;
    static bool 写入三维体素长方体占据位(
        VecIU64& 占据位块,
        std::uint32_t 宽度,
        std::uint32_t 高度,
        std::uint32_t 深度,
        std::uint32_t x,
        std::uint32_t y,
        std::uint32_t z,
        bool 有存在) noexcept;
    static VecIU64 归一化三维体素长方体占据位块(
        const VecIU64& 长方体占据位块,
        std::uint32_t 宽度,
        std::uint32_t 高度,
        std::uint32_t 深度,
        std::uint32_t& 输出细分层数);
    static VecIU64 创建三维体素根节点VecU(
        std::uint32_t 细分层数,
        std::uint32_t 最小体素边长_mm,
        std::uint64_t 占据体素数量,
        std::uint32_t 原始宽度 = 0,
        std::uint32_t 原始高度 = 0,
        std::uint32_t 原始深度 = 0);
    static VecIU64 创建三维体素链节点VecU(
        std::uint32_t 层级,
        std::uint32_t 最大层级,
        std::uint32_t x,
        std::uint32_t y,
        std::uint32_t z,
        std::uint32_t 边长,
        枚举_三维体素链节点状态 状态,
        std::uint64_t 占据体素数量,
        std::uint32_t 最小体素边长_mm);
    static bool 解析三维体素链节点VecU(
        const VecIU64& 值,
        结构_三维体素链节点信息& 输出信息);
    结构_三维体素链写入结果 写入三维体素二分层链(
        特征值类& 值池,
        特征节点类* 节点,
        const VecIU64& 占据位块,
        std::uint32_t 细分层数,
        std::uint32_t 最小体素边长_mm = 0,
        时间戳 now = 结构体_时间戳::当前_微秒());
    结构_三维体素链写入结果 写入三维体素二分层链(
        特征值类& 值池,
        特征节点类* 节点,
        const VecIU64& 占据位块,
        std::uint32_t 细分层数,
        std::uint32_t 原始宽度,
        std::uint32_t 原始高度,
        std::uint32_t 原始深度,
        std::uint32_t 最小体素边长_mm = 0,
        时间戳 now = 结构体_时间戳::当前_微秒());
    结构_三维体素链写入结果 写入三维体素长方体二分层链(
        特征值类& 值池,
        特征节点类* 节点,
        const VecIU64& 长方体占据位块,
        std::uint32_t 宽度,
        std::uint32_t 高度,
        std::uint32_t 深度,
        std::uint32_t 最小体素边长_mm = 0,
        时间戳 now = 结构体_时间戳::当前_微秒());
    static 结构_三维体素链查询结果 查询三维体素二分层链(
        const 特征值类& 值池,
        VecU句柄 根句柄,
        std::uint32_t x,
        std::uint32_t y,
        std::uint32_t z,
        std::uint32_t 查询层级);
    static 结构_三维体素轮廓融合结果 从多视角轮廓图生成三维体素(
        const std::vector<结构_三维体素轮廓图视角>& 视角集合,
        const 结构_三维体素轮廓融合参数& 参数);
    // 根据已生成的局部三维体素，和已存存在体素链做只读相似度比较。
    static 结构_三维体素局部轮廓相似度结果 比较局部三维体素与三维体素链(
        const 特征值类& 值池,
        VecU句柄 存在体素根句柄,
        const 结构_三维体素存在空间绑定参数& 存在空间,
        const 结构_三维体素轮廓融合结果& 局部体素,
        std::uint32_t 查询层级);
    // 根据局部深度轮廓生成的体素，和已存存在体素链做只读相似度比较。
    static 结构_三维体素局部轮廓相似度结果 比较局部轮廓深度图与三维体素链(
        const 特征值类& 值池,
        VecU句柄 存在体素根句柄,
        const 结构_三维体素存在空间绑定参数& 存在空间,
        const std::vector<结构_三维体素轮廓图视角>& 局部视角集合,
        const 结构_三维体素轮廓融合参数& 局部融合参数,
        std::uint32_t 查询层级);
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
