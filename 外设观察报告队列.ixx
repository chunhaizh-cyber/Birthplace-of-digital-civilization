module;

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <deque>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

export module 外设观察报告队列;

export enum class 枚举_外设观察运行模式 : std::uint8_t {
    未指定 = 0,
    陌生环境逐簇识别 = 1,
    已识别场景扫描变化 = 2,
    目标跟踪观察 = 3,
    扫描 = 陌生环境逐簇识别,
    跟踪 = 目标跟踪观察,
};

export enum class 枚举_外设观察报告类型 : std::uint8_t {
    未指定 = 0,
    逐簇识别报告 = 1,
    扫描变化报告 = 2,
    跟踪报告 = 3,
};

export struct 结构_外设观察像素簇摘要 {
    std::int64_t 外设内部簇ID = 0;
    std::int64_t 来源空间候选ID = 0;
    std::int64_t 外设跨帧簇ID = -1;
    std::int64_t 跨帧匹配状态 = 0;
    std::int64_t 跨帧连续命中次数 = 0;
    std::int64_t 跨帧丢失次数 = 0;
    std::int64_t 跨帧复现离散度 = 0;
    std::int64_t 像素数量 = 0;
    std::int64_t 投影最小X = 0;
    std::int64_t 投影最大X = 0;
    std::int64_t 投影最小Y = 0;
    std::int64_t 投影最大Y = 0;
    std::int64_t 中心X = 0;
    std::int64_t 中心Y = 0;
    std::int64_t 中心Z = 0;
    std::int64_t 空间范围最小X = 0;
    std::int64_t 空间范围最大X = 0;
    std::int64_t 空间范围最小Y = 0;
    std::int64_t 空间范围最大Y = 0;
    std::int64_t 空间范围最小Z = 0;
    std::int64_t 空间范围最大Z = 0;
    std::int64_t 深度有效率 = 0;
    std::int64_t 空间坐标有效率 = 0;
    std::int64_t 空间连续性评分 = 0;
    std::int64_t 范围稳定性评分 = 0;
    std::int64_t 轮廓置信度 = 0;
    std::int64_t 像素集合掩码状态 = 0;
    std::int64_t 彩色轮廓局部图状态 = 0;
    std::int64_t 深度轮廓局部图状态 = 0;
    std::int64_t 局部图宽度 = 0;
    std::int64_t 局部图高度 = 0;
    std::string 像素集合掩码句柄{};
    std::string 彩色轮廓局部图句柄{};
    std::string 深度轮廓局部图句柄{};
    std::string 失败原因{};
};

export struct 结构_外设观察报告队列项 {
    std::uint64_t 报告ID = 0;
    std::uint64_t 对应命令ID = 0;
    std::uint64_t 匹配等待项ID = 0;
    std::string 来源外设{};
    枚举_外设观察运行模式 观察运行模式 = 枚举_外设观察运行模式::未指定;
    枚举_外设观察报告类型 报告类型 = 枚举_外设观察报告类型::未指定;
    std::int64_t 时间戳毫秒 = 0;
    bool 采集成功 = false;
    std::string 配置摘要{};
    std::string 错误摘要{};
    std::string 失败原因{};

    std::int64_t 帧宽度 = 0;
    std::int64_t 帧高度 = 0;
    std::int64_t 深度帧号 = 0;
    std::int64_t 彩色帧号 = 0;
    std::int64_t 预期像素数量 = 0;
    std::int64_t 像素特征数量 = 0;
    std::int64_t 深度有效率 = 0;
    std::int64_t 空间坐标有效率 = 0;
    std::int64_t 帧质量评分 = 0;
    std::int64_t 低质量区域数量 = 0;
    std::int64_t 未归簇区域数量 = 0;
    std::int64_t 不可观测区域数量 = 0;
    std::int64_t 未解释区域比例 = 0;
    std::int64_t 新增观察像素簇数量 = 0;
    std::int64_t 消失观察像素簇数量 = 0;
    std::int64_t 变化观察像素簇数量 = 0;
    std::int64_t 高风险未知区域数量 = 0;
    std::string 差异摘要{};
    std::int64_t 跨帧采集状态 = 0;
    std::int64_t 跨帧匹配观察像素簇数量 = 0;
    std::int64_t 跨帧新建观察像素簇数量 = 0;
    std::int64_t 跨帧稳定观察像素簇数量 = 0;
    std::int64_t 跨帧丢失观察像素簇数量 = 0;
    std::string 跨帧采集摘要{};

    std::uint64_t 跟踪ID = 0;
    std::string 跟踪种子{};
    std::int64_t 跟踪状态值 = 0;
    std::int64_t 丢失状态值 = 0;
    std::int64_t 最近观测年龄毫秒 = 0;
    std::int64_t 距离复现离散度 = 0;
    std::int64_t 空间坐标复现离散度 = 0;
    std::int64_t 连续成功次数 = 0;
    std::int64_t 失败次数 = 0;
    std::int64_t 重捕获候选数量 = 0;
    std::int64_t 稳定特征样本数量 = 0;

    std::vector<结构_外设观察像素簇摘要> 观察像素簇集合{};
};

export struct 结构_外设观察等待项 {
    std::uint64_t 等待项ID = 0;
    std::string 来源需求ID{};
    std::string 目标外设{};
    枚举_外设观察运行模式 观察运行模式 = 枚举_外设观察运行模式::未指定;
    枚举_外设观察报告类型 期望报告类型 = 枚举_外设观察报告类型::未指定;
    std::int64_t 最大允许报告年龄毫秒 = 0;
    bool 是否允许缓存报告 = true;
    std::string 目标区域或目标簇{};
    std::string 质量要求摘要{};
};

export struct 结构_外设观察队列状态 {
    std::size_t 队列容量 = 0;
    std::size_t 队列数量 = 0;
    std::size_t 等待项数量 = 0;
    std::uint64_t 最新报告ID = 0;
};

export const char* 外设观察运行模式文本(枚举_外设观察运行模式 类型) noexcept;
export const char* 外设观察报告类型文本(枚举_外设观察报告类型 类型) noexcept;
export std::uint64_t 提交外设观察报告(结构_外设观察报告队列项 报告项);
export std::uint64_t 提交外设观察等待项(结构_外设观察等待项 等待项);
export std::optional<结构_外设观察报告队列项> 读取最新外设观察报告(
    const std::string& 外设ID,
    枚举_外设观察报告类型 报告类型);
export std::optional<结构_外设观察报告队列项> 读取外设观察报告_按ID(
    std::uint64_t 报告ID);
export std::vector<结构_外设观察等待项> 读取外设观察等待项集合(
    const std::string& 外设ID,
    枚举_外设观察运行模式 观察运行模式);
export std::optional<结构_外设观察报告队列项> 按等待项匹配外设观察报告(
    const 结构_外设观察等待项& 等待项);
export 结构_外设观察队列状态 读取外设观察队列状态();
export void 清空外设观察报告队列();
export std::string 构造外设观察报告摘要(const 结构_外设观察报告队列项& 报告项);
export std::string 构造外设观察等待项摘要(const 结构_外设观察等待项& 等待项);

namespace {
    struct 结构_外设观察队列共享状态 {
        std::mutex 互斥{};
        std::deque<结构_外设观察报告队列项> 报告队列{};
        std::vector<结构_外设观察等待项> 等待项集合{};
        std::uint64_t 最新报告ID = 0;
        std::size_t 队列容量 = 64;
    };

    结构_外设观察队列共享状态& 外设观察队列状态() noexcept
    {
        static 结构_外设观察队列共享状态 状态;
        return 状态;
    }

    std::atomic<std::uint64_t>& 外设观察报告ID计数器() noexcept
    {
        static std::atomic<std::uint64_t> 计数器{0};
        return 计数器;
    }

    std::atomic<std::uint64_t>& 外设观察等待项ID计数器() noexcept
    {
        static std::atomic<std::uint64_t> 计数器{0};
        return 计数器;
    }

    std::int64_t 外设观察当前时间毫秒() noexcept
    {
        const auto 现在 = std::chrono::steady_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(现在).count();
    }

    bool 外设观察_报告满足等待项(
        const 结构_外设观察报告队列项& 报告项,
        const 结构_外设观察等待项& 等待项,
        std::int64_t 当前时间毫秒) noexcept
    {
        if (!等待项.目标外设.empty() && 报告项.来源外设 != 等待项.目标外设) {
            return false;
        }
        if (等待项.观察运行模式 != 枚举_外设观察运行模式::未指定
            && 报告项.观察运行模式 != 等待项.观察运行模式) {
            return false;
        }
        if (等待项.期望报告类型 != 枚举_外设观察报告类型::未指定
            && 报告项.报告类型 != 等待项.期望报告类型) {
            return false;
        }
        if (等待项.最大允许报告年龄毫秒 > 0
            && 报告项.时间戳毫秒 > 0
            && 当前时间毫秒 - 报告项.时间戳毫秒 > 等待项.最大允许报告年龄毫秒) {
            return false;
        }
        return true;
    }
}

const char* 外设观察运行模式文本(枚举_外设观察运行模式 类型) noexcept
{
    switch (类型) {
    case 枚举_外设观察运行模式::陌生环境逐簇识别: return "陌生环境逐簇识别";
    case 枚举_外设观察运行模式::已识别场景扫描变化: return "已识别场景扫描变化";
    case 枚举_外设观察运行模式::目标跟踪观察: return "目标跟踪观察";
    case 枚举_外设观察运行模式::未指定: return "未指定";
    default: return "未知";
    }
}

const char* 外设观察报告类型文本(枚举_外设观察报告类型 类型) noexcept
{
    switch (类型) {
    case 枚举_外设观察报告类型::逐簇识别报告: return "逐簇识别报告";
    case 枚举_外设观察报告类型::扫描变化报告: return "扫描变化报告";
    case 枚举_外设观察报告类型::跟踪报告: return "跟踪报告";
    case 枚举_外设观察报告类型::未指定: return "未指定";
    default: return "未知";
    }
}

std::uint64_t 提交外设观察报告(结构_外设观察报告队列项 报告项)
{
    auto& 状态 = 外设观察队列状态();
    if (报告项.报告ID == 0) {
        报告项.报告ID = ++外设观察报告ID计数器();
    }
    if (报告项.时间戳毫秒 == 0) {
        报告项.时间戳毫秒 = 外设观察当前时间毫秒();
    }

    std::lock_guard<std::mutex> 锁(状态.互斥);
    状态.报告队列.push_back(报告项);
    状态.最新报告ID = 报告项.报告ID;
    while (状态.报告队列.size() > 状态.队列容量) {
        状态.报告队列.pop_front();
    }
    return 报告项.报告ID;
}

std::uint64_t 提交外设观察等待项(结构_外设观察等待项 等待项)
{
    auto& 状态 = 外设观察队列状态();
    if (等待项.等待项ID == 0) {
        等待项.等待项ID = ++外设观察等待项ID计数器();
    }

    std::lock_guard<std::mutex> 锁(状态.互斥);
    状态.等待项集合.push_back(等待项);
    return 等待项.等待项ID;
}

std::optional<结构_外设观察报告队列项> 读取最新外设观察报告(
    const std::string& 外设ID,
    枚举_外设观察报告类型 报告类型)
{
    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
        if (!外设ID.empty() && 迭代器->来源外设 != 外设ID) {
            continue;
        }
        if (报告类型 != 枚举_外设观察报告类型::未指定 && 迭代器->报告类型 != 报告类型) {
            continue;
        }
        return *迭代器;
    }
    return std::nullopt;
}

std::optional<结构_外设观察报告队列项> 读取外设观察报告_按ID(
    std::uint64_t 报告ID)
{
    if (报告ID == 0) {
        return std::nullopt;
    }
    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
        if (迭代器->报告ID == 报告ID) {
            return *迭代器;
        }
    }
    return std::nullopt;
}

std::vector<结构_外设观察等待项> 读取外设观察等待项集合(
    const std::string& 外设ID,
    枚举_外设观察运行模式 观察运行模式)
{
    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    std::vector<结构_外设观察等待项> 输出{};
    输出.reserve(状态.等待项集合.size());
    for (const auto& 等待项 : 状态.等待项集合) {
        if (!外设ID.empty() && 等待项.目标外设 != 外设ID) {
            continue;
        }
        if (观察运行模式 != 枚举_外设观察运行模式::未指定
            && 等待项.观察运行模式 != 观察运行模式) {
            continue;
        }
        输出.push_back(等待项);
    }
    return 输出;
}

std::optional<结构_外设观察报告队列项> 按等待项匹配外设观察报告(
    const 结构_外设观察等待项& 等待项)
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
        if (外设观察_报告满足等待项(*迭代器, 等待项, 当前时间)) {
            auto 匹配报告 = *迭代器;
            匹配报告.匹配等待项ID = 等待项.等待项ID;
            return 匹配报告;
        }
    }
    return std::nullopt;
}

结构_外设观察队列状态 读取外设观察队列状态()
{
    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    return 结构_外设观察队列状态{
        状态.队列容量,
        状态.报告队列.size(),
        状态.等待项集合.size(),
        状态.最新报告ID,
    };
}

void 清空外设观察报告队列()
{
    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    状态.报告队列.clear();
    状态.等待项集合.clear();
    状态.最新报告ID = 0;
}

std::string 构造外设观察报告摘要(const 结构_外设观察报告队列项& 报告项)
{
    std::ostringstream 输出;
    std::int64_t 掩码簇数量 = 0;
    std::int64_t 彩色局部图簇数量 = 0;
    std::int64_t 深度局部图簇数量 = 0;
    for (const auto& 簇 : 报告项.观察像素簇集合) {
        if (簇.像素集合掩码状态 > 0 && !簇.像素集合掩码句柄.empty()) {
            ++掩码簇数量;
        }
        if (簇.彩色轮廓局部图状态 > 0 && !簇.彩色轮廓局部图句柄.empty()) {
            ++彩色局部图簇数量;
        }
        if (簇.深度轮廓局部图状态 > 0 && !簇.深度轮廓局部图句柄.empty()) {
            ++深度局部图簇数量;
        }
    }
    输出 << "外设观察报告"
        << " | 报告ID=" << 报告项.报告ID
        << " | 来源外设=" << (报告项.来源外设.empty() ? "空" : 报告项.来源外设)
        << " | 观察运行模式=" << 外设观察运行模式文本(报告项.观察运行模式)
        << " | 类型=" << 外设观察报告类型文本(报告项.报告类型)
        << " | 时间戳毫秒=" << 报告项.时间戳毫秒
        << " | 成功=" << (报告项.采集成功 ? 1 : 0)
        << " | 帧=" << 报告项.帧宽度 << 'x' << 报告项.帧高度
        << " | 深度帧号=" << 报告项.深度帧号
        << " | 彩色帧号=" << 报告项.彩色帧号
        << " | 像素簇=" << 报告项.观察像素簇集合.size()
        << " | 掩码簇=" << 掩码簇数量
        << " | 彩色局部图簇=" << 彩色局部图簇数量
        << " | 深度局部图簇=" << 深度局部图簇数量
        << " | 深度有效率=" << 报告项.深度有效率
        << " | 空间坐标有效率=" << 报告项.空间坐标有效率
        << " | 质量=" << 报告项.帧质量评分;
    if (报告项.跨帧采集状态 > 0) {
        输出 << " | 跨帧匹配簇=" << 报告项.跨帧匹配观察像素簇数量
            << " | 跨帧新建簇=" << 报告项.跨帧新建观察像素簇数量
            << " | 跨帧稳定簇=" << 报告项.跨帧稳定观察像素簇数量
            << " | 跨帧丢失簇=" << 报告项.跨帧丢失观察像素簇数量;
        if (!报告项.跨帧采集摘要.empty()) {
            输出 << " | 跨帧摘要=" << 报告项.跨帧采集摘要;
        }
    }
    if (报告项.报告类型 == 枚举_外设观察报告类型::扫描变化报告) {
        输出 << " | 新增簇=" << 报告项.新增观察像素簇数量
            << " | 消失簇=" << 报告项.消失观察像素簇数量
            << " | 变化簇=" << 报告项.变化观察像素簇数量
            << " | 高风险未知=" << 报告项.高风险未知区域数量;
        if (!报告项.差异摘要.empty()) {
            输出 << " | 差异摘要=" << 报告项.差异摘要;
        }
    }
    if (报告项.报告类型 == 枚举_外设观察报告类型::跟踪报告) {
        输出 << " | 跟踪ID=" << 报告项.跟踪ID
            << " | 跟踪种子=" << (报告项.跟踪种子.empty() ? "空" : 报告项.跟踪种子)
            << " | 跟踪状态=" << 报告项.跟踪状态值
            << " | 丢失状态=" << 报告项.丢失状态值
            << " | 连续成功=" << 报告项.连续成功次数
            << " | 失败次数=" << 报告项.失败次数
            << " | 稳定特征样本=" << 报告项.稳定特征样本数量;
    }
    if (!报告项.失败原因.empty()) {
        输出 << " | 失败原因=" << 报告项.失败原因;
    }
    return 输出.str();
}

std::string 构造外设观察等待项摘要(const 结构_外设观察等待项& 等待项)
{
    std::ostringstream 输出;
    输出 << "外设观察等待项"
        << " | 等待项ID=" << 等待项.等待项ID
        << " | 来源需求=" << (等待项.来源需求ID.empty() ? "空" : 等待项.来源需求ID)
        << " | 目标外设=" << (等待项.目标外设.empty() ? "空" : 等待项.目标外设)
        << " | 观察运行模式=" << 外设观察运行模式文本(等待项.观察运行模式)
        << " | 期望报告=" << 外设观察报告类型文本(等待项.期望报告类型)
        << " | 最大年龄毫秒=" << 等待项.最大允许报告年龄毫秒
        << " | 允许缓存=" << (等待项.是否允许缓存报告 ? 1 : 0);
    return 输出.str();
}
