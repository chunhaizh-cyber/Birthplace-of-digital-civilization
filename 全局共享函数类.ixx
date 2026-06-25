module;

#include "基础数据类型.h"

#include <bit>
#include <cmath>
#include <string>
#include <string_view>

export module 全局共享函数类;

// 文件头部规则注释模块：
// 1. 没有领域自定义主信息 / 领域类型参与的通用函数统一放在本模块。
// 2. 新增前必须先扫描 说明书/函数功能说明文档.md，确认没有同名或同功能函数。
// 3. 本模块可直接包含不含项目自定义数据结构的标准库或纯工具头。
// 4. 必要的非主信息类通用自定义数据结构应先迁移到 基础数据类型.h，再供本模块使用。
// 5. 不直接包含含项目自定义数据结构的领域头。
// 6. 涉及语素、基础信息、主信息类、存在、场景、需求、任务、方法等领域类型的共享函数，
//    放入对应功能类、专用类或对应功能头文件，不进入本模块。
// 7. 本模块只供通用计算和格式化复用，不承载机器业务状态。

// 功能：对 I64 执行带上下限保护的饱和加法。
export inline I64 饱和增加(I64 左值, I64 右值) noexcept
{
    if (右值 > 0 && 左值 > (std::numeric_limits<I64>::max)() - 右值) {
        return (std::numeric_limits<I64>::max)();
    }
    if (右值 < 0 && 左值 < (std::numeric_limits<I64>::min)() - 右值) {
        return (std::numeric_limits<I64>::min)();
    }
    return 左值 + 右值;
}

// 功能：对 I64 执行带上下限保护的饱和减法。
export inline I64 饱和减少(I64 左值, I64 右值) noexcept
{
    if (右值 == (std::numeric_limits<I64>::min)()) {
        return (std::numeric_limits<I64>::max)();
    }
    return 饱和增加(左值, -右值);
}

// 功能：返回 I64 的饱和绝对值，I64 最小值返回 I64 最大值。
export inline I64 饱和绝对值I64(I64 值) noexcept
{
    if (值 == (std::numeric_limits<I64>::min)()) {
        return (std::numeric_limits<I64>::max)();
    }
    return 值 < 0 ? -值 : 值;
}

// 功能：返回两个 I64 值的非饱和绝对差，保持原调用点既有算术语义。
export inline I64 绝对差I64(I64 左值, I64 右值) noexcept
{
    return 左值 >= 右值 ? 左值 - 右值 : 右值 - 左值;
}

// 功能：返回两个 I64 边界值的非负正向差，反向时返回 0。
export inline I64 正差I64(I64 最大值, I64 最小值) noexcept
{
    return 最大值 >= 最小值 ? 最大值 - 最小值 : 0;
}

// 功能：返回两个闭区间之间的 I64 间隔，区间重叠时返回 0。
export inline I64 区间间隔I64(I64 第一最小值, I64 第一最大值, I64 第二最小值, I64 第二最大值) noexcept
{
    if (第一最大值 < 第二最小值) return 第二最小值 - 第一最大值;
    if (第二最大值 < 第一最小值) return 第一最小值 - 第二最大值;
    return 0;
}

// 功能：计算两个 I64 边界的中点，避免直接相加导致溢出。
export inline I64 区间中点(I64 下界, I64 上界) noexcept
{
    return 下界 / 2 + 上界 / 2 + (下界 % 2 + 上界 % 2) / 2;
}

// 功能：把 I64 映射为保持有序关系的 U64 编码。
export inline std::uint64_t I64有序编码(I64 值) noexcept
{
    return static_cast<std::uint64_t>(值) ^ (std::uint64_t{ 1 } << 63);
}

// 功能：把保持有序关系的 U64 编码还原为 I64。
export inline I64 I64由有序编码(std::uint64_t 编码) noexcept
{
    return static_cast<I64>(编码 ^ (std::uint64_t{ 1 } << 63));
}

// 功能：把候选数量限制为可枚举的安全预算。
export inline std::uint64_t 有效候选预算(I64 最大候选数量) noexcept
{
    if (最大候选数量 <= 0) return 64;
    constexpr std::uint64_t 上限 = 4096;
    const auto 值 = static_cast<std::uint64_t>(最大候选数量);
    return 值 > 上限 ? 上限 : 值;
}

// 功能：当错误输出指针有效时写入可读错误文本；空文本指针写为空串。
export inline void 写入可选错误文本(std::string* 错误, const char* 文本) noexcept
{
    if (错误) {
        *错误 = 文本 ? 文本 : "";
    }
}

// 功能：把稳定主键文本折叠为 U64；仅用于主键/标识压缩，不用于自由文本业务判断。
export inline std::uint64_t 折叠字符串主键(std::string_view 主键) noexcept
{
    std::uint64_t 值 = 1469598103934665603ull;
    for (const unsigned char 字节 : 主键) {
        值 ^= static_cast<std::uint64_t>(字节);
        值 *= 1099511628211ull;
    }
    return 值;
}

// 功能：计算左右扩展数量合并后的安全域大小，溢出时返回 U64 最大值。
export inline std::uint64_t 安全域大小(std::uint64_t 左侧数量, std::uint64_t 右侧数量) noexcept
{
    constexpr auto 最大值 = (std::numeric_limits<std::uint64_t>::max)();
    if (最大值 - 左侧数量 <= 右侧数量) {
        return 最大值;
    }
    return 左侧数量 + 右侧数量 + 1;
}

// 功能：把 I64 值限制到闭区间范围内。
export inline I64 限制I64(I64 值, I64 下界, I64 上界) noexcept
{
    if (值 < 下界) return 下界;
    if (值 > 上界) return 上界;
    return 值;
}

// 功能：计算 I64 比例的万分比，非法分子或分母返回 0。
export inline I64 计算比例万分比I64(I64 分子, I64 分母) noexcept
{
    if (分子 <= 0 || 分母 <= 0) return 0;
    return std::clamp<I64>(分子 * 10000 / 分母, 0, 10000);
}

// 功能：按整数百分比计算 I64 基准值的百分比结果，避免直接相乘先溢出。
export constexpr I64 百分比值(I64 基准, I64 百分比) noexcept
{
    return (基准 / 100) * 百分比 + ((基准 % 100) * 百分比) / 100;
}

// 功能：合并基础匹配分数与 VecI64 轮廓匹配分数，结果限制在 Q10000 区间。
export inline I64 合并基础匹配与VecI64轮廓评分(I64 基础评分, I64 轮廓评分) noexcept
{
    基础评分 = std::clamp<I64>(基础评分, 0, 10000);
    轮廓评分 = std::clamp<I64>(轮廓评分, 0, 10000);
    if (轮廓评分 <= 0) {
        return 基础评分;
    }
    return std::clamp<I64>(基础评分 * 70 / 100 + 轮廓评分 * 30 / 100, 0, 10000);
}

// 功能：计算闭合 ROI 的像素面积，范围反向时返回 0。
export inline I64 ROI面积(I64 最小X, I64 最大X, I64 最小Y, I64 最大Y) noexcept
{
    if (最大X < 最小X || 最大Y < 最小Y) return 0;
    return (最大X - 最小X + 1) * (最大Y - 最小Y + 1);
}

// 功能：计算两个闭合 ROI 的重叠面积。
export inline I64 ROI重叠面积(
    I64 A最小X,
    I64 A最大X,
    I64 A最小Y,
    I64 A最大Y,
    I64 B最小X,
    I64 B最大X,
    I64 B最小Y,
    I64 B最大Y) noexcept
{
    const I64 交最小X = std::max<I64>(A最小X, B最小X);
    const I64 交最大X = std::min<I64>(A最大X, B最大X);
    const I64 交最小Y = std::max<I64>(A最小Y, B最小Y);
    const I64 交最大Y = std::min<I64>(A最大Y, B最大Y);
    return ROI面积(交最小X, 交最大X, 交最小Y, 交最大Y);
}

// 功能：计算 I64 闭区间长度，范围反向时返回 0。
export inline I64 正范围(I64 最小值, I64 最大值) noexcept
{
    return 最大值 >= 最小值 ? (最大值 - 最小值) : 0;
}

// 功能：计算投影闭区间尺寸，范围缺失时使用兜底值且最小为 1。
export inline I64 投影尺寸(I64 最小值, I64 最大值, I64 兜底) noexcept
{
    if (最大值 >= 最小值) {
        return std::max<I64>(1, 最大值 - 最小值 + 1);
    }
    return std::max<I64>(1, 兜底);
}

// 功能：返回三个 I64 值中的最大值。
export inline I64 最大3(I64 a, I64 b, I64 c) noexcept
{
    return std::max(a, std::max(b, c));
}

// 功能：计算两个一维闭区间的重叠率，输出万分比。
export inline I64 范围轴重叠率(I64 aMin, I64 aMax, I64 bMin, I64 bMax) noexcept
{
    const I64 aLen = std::max<I64>(1, 正范围(aMin, aMax));
    const I64 bLen = std::max<I64>(1, 正范围(bMin, bMax));
    const I64 重叠 = std::max<I64>(0, std::min(aMax, bMax) - std::max(aMin, bMin));
    return std::min<I64>(10000, 重叠 * 10000 / std::max(aLen, bLen));
}

// 功能：只累加正数 I64 增量，超过 I64 上限时饱和到最大值。
export inline void 累加非负I64(I64& 累计, I64 值) noexcept
{
    if (值 <= 0) return;
    const I64 上限 = std::numeric_limits<I64>::max();
    累计 = 累计 > 上限 - 值 ? 上限 : 累计 + 值;
}

// 功能：把 size_t 计数转换为 I64，超过 I64 上限时饱和。
export inline I64 转换像素计数(std::size_t 值) noexcept
{
    constexpr auto 上限 = static_cast<std::size_t>(std::numeric_limits<I64>::max());
    return 值 > 上限 ? std::numeric_limits<I64>::max() : static_cast<I64>(值);
}

// 功能：把浮点毫米值四舍五入转换为 I64，非有限值返回 0，越界时饱和。
export inline I64 转换毫米(double 值) noexcept
{
    if (!std::isfinite(值)) {
        return 0;
    }
    const double 下限 = static_cast<double>(std::numeric_limits<I64>::min());
    const double 上限 = static_cast<double>(std::numeric_limits<I64>::max());
    if (值 <= 下限) {
        return std::numeric_limits<I64>::min();
    }
    if (值 >= 上限) {
        return std::numeric_limits<I64>::max();
    }
    return static_cast<I64>(std::llround(值));
}

// 功能：计算 size_t 比例的万分比，分母为 0 时返回 0，保持原有未截断比例语义。
export inline I64 比例万分比(std::size_t 分子, std::size_t 分母) noexcept
{
    if (分母 == 0) {
        return 0;
    }
    return static_cast<I64>(
        (static_cast<unsigned long long>(分子) * 10000ULL)
        / static_cast<unsigned long long>(分母));
}

// 功能：计算 double 与 I64 的非饱和绝对差，返回 I64 截断结果。
export inline I64 绝对差F64I64(double 左值, I64 右值) noexcept
{
    const double 差 = 左值 - static_cast<double>(右值);
    return static_cast<I64>(差 >= 0.0 ? 差 : -差);
}

// 功能：计算当前时间到有效截止时间的剩余毫秒数，过期时返回 0。
export inline I64 计算TTL毫秒(I64 当前时间毫秒, I64 有效截止时间毫秒) noexcept
{
    return 有效截止时间毫秒 > 当前时间毫秒
        ? 有效截止时间毫秒 - 当前时间毫秒
        : 0;
}

// 功能：把非负 long double 四舍五入转换为 I64，负数返回 0，超过 I64 上限时饱和。
export inline I64 长双非负转I64(long double 值) noexcept
{
    if (值 <= 0.0L) return 0;
    if (值 >= 9223372036854775807.0L) return 9223372036854775807LL;
    return static_cast<I64>(值 + 0.5L);
}

// 功能：把有限的坐标分量四舍五入为 I64，越界或非有限值返回 false。
export inline bool 坐标分量转I64(double 值, I64& 输出) noexcept
{
    if (!std::isfinite(值)) return false;
    const long double 下界 = static_cast<long double>((std::numeric_limits<I64>::min)());
    const long double 上界 = static_cast<long double>((std::numeric_limits<I64>::max)());
    const long double 待写 = static_cast<long double>(值);
    if (待写 < 下界 || 待写 > 上界) return false;
    输出 = static_cast<I64>(std::llround(值));
    return true;
}

// 功能：把 Vector3D 三个坐标分量四舍五入为 I64 三元组。
export inline bool 坐标转I64三元(const Vector3D& 坐标, I64& x, I64& y, I64& z) noexcept
{
    return 坐标分量转I64(坐标.x, x)
        && 坐标分量转I64(坐标.y, y)
        && 坐标分量转I64(坐标.z, z);
}

// 功能：计算边长立方体的三维体素总数，溢出或边长为 0 时返回 0。
export inline std::uint64_t 三维体素总数(std::uint64_t 边长) noexcept
{
    if (边长 == 0 || 边长 > (std::numeric_limits<std::uint64_t>::max)() / 边长) return 0;
    const auto 面 = 边长 * 边长;
    if (边长 > (std::numeric_limits<std::uint64_t>::max)() / 面) return 0;
    return 面 * 边长;
}

// 功能：计算三维长方体体素总数，任一维为 0 或乘法溢出时返回 0。
export inline std::uint64_t 三维体素长方体总数(
    const std::uint64_t 宽度,
    const std::uint64_t 高度,
    const std::uint64_t 深度) noexcept
{
    if (宽度 == 0 || 高度 == 0 || 深度 == 0) return 0;
    if (宽度 > (std::numeric_limits<std::uint64_t>::max)() / 高度) return 0;
    const auto 面 = 宽度 * 高度;
    if (深度 > (std::numeric_limits<std::uint64_t>::max)() / 面) return 0;
    return 面 * 深度;
}

// 功能：统计 VecIU64 位块中的三维体素占据位数量。
export inline std::uint64_t 统计三维体素占据数(const VecIU64& 占据位块) noexcept
{
    std::uint64_t 数量 = 0;
    for (const auto 块 : 占据位块) {
        数量 += static_cast<std::uint64_t>(std::popcount(块));
    }
    return 数量;
}

// 功能：把一个 U64 值混入累计版本号，混合结果为 0 时返回 1。
export inline std::uint64_t 混合U64版本(std::uint64_t 累积, std::uint64_t 值) noexcept
{
    累积 ^= 值 + 0x9e3779b97f4a7c15ull + (累积 << 6) + (累积 >> 2);
    return 累积 == 0 ? 1 : 累积;
}

// 功能：把方向值归一为 -1、0、1 三种练习差值。
export inline I64 练习方向转差值(I64 方向) noexcept
{
    if (方向 > 0) return 1;
    if (方向 < 0) return -1;
    return 0;
}

// 功能：根据已生成数量生成无目标 I64 候选值。
export inline I64 生成无目的未出现I64参数(I64 已生成数量) noexcept
{
    auto x = static_cast<std::uint64_t>(已生成数量) + 0x9E3779B97F4A7C15ull;
    x ^= x >> 30;
    x *= 0xBF58476D1CE4E5B9ull;
    x ^= x >> 27;
    x *= 0x94D049BB133111EBull;
    x ^= x >> 31;
    return static_cast<I64>(x);
}

// 功能：从区间中点向两侧扩展生成 I64 候选值。
export inline I64 生成区间扩展覆盖I64参数(
    I64 已生成数量,
    I64 下界,
    I64 上界,
    I64 中点,
    I64 最大候选数量,
    I64& 输出策略) noexcept
{
    输出策略 = 2;
    const auto 下界码 = I64有序编码(下界);
    const auto 上界码 = I64有序编码(上界);
    const auto 中点码 = I64有序编码(中点);
    const auto 左侧数量 = 中点码 - 下界码;
    const auto 右侧数量 = 上界码 - 中点码;

    auto 序号 = 已生成数量 < 0
        ? std::uint64_t{ 0 }
        : static_cast<std::uint64_t>(已生成数量);
    const auto 总数 = 安全域大小(左侧数量, 右侧数量);
    const auto 预算 = 有效候选预算(最大候选数量);
    const bool 值域过大 = 总数 > 预算;
    if (值域过大) {
        序号 %= 预算;
        const auto 步幅 = 总数 / 预算 + ((总数 % 预算) ? 1 : 0);
        if (序号 > 0 && 步幅 > 1) {
            序号 *= 步幅;
            if (序号 >= 总数) {
                序号 = 总数 - 1;
            }
        }
        输出策略 = 4;
    } else if (序号 >= 总数) {
        序号 %= 总数;
    }
    if (序号 == 0) {
        return 中点;
    }

    const auto 公共侧数量 = (左侧数量 < 右侧数量)
        ? 左侧数量
        : 右侧数量;
    const auto 公共覆盖数量 = std::uint64_t{ 1 } + 公共侧数量 * std::uint64_t{ 2 };
    if (序号 < 公共覆盖数量) {
        const auto 步长 = (序号 + 1) / 2;
        if ((序号 % 2) == 1) {
            return I64由有序编码(中点码 - 步长);
        }
        return I64由有序编码(中点码 + 步长);
    }

    const auto 剩余序号 = 序号 - 公共覆盖数量;
    const auto 步长 = 公共侧数量 + std::uint64_t{ 1 } + 剩余序号;
    if (左侧数量 > 右侧数量) {
        return I64由有序编码(中点码 - 步长);
    }
    return I64由有序编码(中点码 + 步长);
}

// 功能：在指定区间内生成 I64 候选值，优先中点并按两侧扩展覆盖。
export inline I64 生成区间内I64参数(
    I64 已生成数量,
    I64 下界,
    I64 上界,
    bool 有历史值,
    I64 历史值,
    I64 最大候选数量,
    I64& 输出策略) noexcept
{
    输出策略 = 0;
    if (下界 > 上界) {
        下界 = 上界 = 0;
    }
    if (有历史值 && 下界 <= 历史值 && 历史值 <= 上界
        && 已生成数量 > 0
        && 已生成数量 % 17 == 0) {
        输出策略 = 1;
        return 历史值;
    }
    const I64 中点 = 区间中点(下界, 上界);
    return 生成区间扩展覆盖I64参数(
        已生成数量,
        下界,
        上界,
        中点,
        最大候选数量,
        输出策略);
}

// 功能：从离散集合中按中点向两侧扩展选择候选索引。
export inline std::size_t 离散集合扩展索引(std::size_t 数量, I64 已生成数量) noexcept
{
    if (数量 == 0) return 0;
    auto 序号 = 已生成数量 < 0
        ? std::size_t{ 0 }
        : static_cast<std::size_t>(已生成数量);
    序号 %= 数量;
    const auto 中点 = (数量 - 1) / 2;
    if (序号 == 0) return 中点;

    const auto 左侧数量 = 中点;
    const auto 右侧数量 = 数量 - 1 - 中点;
    const auto 公共侧数量 = (左侧数量 < 右侧数量) ? 左侧数量 : 右侧数量;
    const auto 公共覆盖数量 = std::size_t{ 1 } + 公共侧数量 * std::size_t{ 2 };
    if (序号 < 公共覆盖数量) {
        const auto 步长 = (序号 + 1) / 2;
        return (序号 % 2) == 1 ? 中点 - 步长 : 中点 + 步长;
    }

    const auto 剩余序号 = 序号 - 公共覆盖数量;
    if (左侧数量 > 右侧数量) {
        return 中点 - (公共侧数量 + std::size_t{ 1 } + 剩余序号);
    }
    return 中点 + (公共侧数量 + std::size_t{ 1 } + 剩余序号);
}

// 功能：把 U64 转换为 I64，超过 I64 上限时饱和。
export inline I64 U64转I64饱和(std::uint64_t 值) noexcept
{
    constexpr auto 最大值 = static_cast<std::uint64_t>((std::numeric_limits<I64>::max)());
    if (值 > 最大值) {
        return (std::numeric_limits<I64>::max)();
    }
    return static_cast<I64>(值);
}

// 功能：将布尔值格式化为中文显示文本。
export constexpr const char* 布尔文本_是或否(bool 值) noexcept
{
    return 值 ? "是" : "否";
}

// 功能：将布尔值格式化为数字协议文本。
export constexpr const char* 布尔文本_一或零(bool 值) noexcept
{
    return 值 ? "1" : "0";
}

// 功能：将布尔值格式化为英文调试文本。
export inline std::string 布尔文本_true或false(bool 值)
{
    return 值 ? "true" : "false";
}

// 功能：将整数格式化为 SQL 字段恢复比对文本。
export inline std::string SQL整数文本(int 值)
{
    return std::to_string(值);
}

// 功能：将可空整数格式化为 SQL 写入文本；无值时返回 NULL。
export inline std::string SQL可空整数文本(bool 有值, int 值)
{
    return 有值 ? std::to_string(值) : "NULL";
}

// 功能：将可空整数格式化为 SQL 字段恢复比对文本；无值时返回空文本。
export inline std::string SQL字段可空整数文本(bool 有值, int 值)
{
    return 有值 ? std::to_string(值) : std::string{};
}

// 功能：把指针地址格式化为日志文本。
export inline std::string 指针日志文本(const void* 指针) noexcept
{
    return std::to_string(reinterpret_cast<std::uintptr_t>(指针));
}
