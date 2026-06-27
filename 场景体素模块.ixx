// 文件头部规则注释模块：
// 1. 本模块只提供当前场景体素只读快照和查询服务，不写世界树事实、需求满足、价值结算或动作动态。
// 2. 场景体素先验只能辅助观察域候选、评分、解释和缺口，不得替代合法外设材料和提交入口裁决。
// 3. 新增业务接入前必须回读 `规范/场景体素服务与视觉先验融合规范20260625.md` 和计划当前切片。

module;

#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <deque>
#include <limits>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "基础信息类.h"
#include "场景类.h"
#include "存在类.h"
#include "特征类.h"

export module 场景体素模块;

import 全局共享函数类;

export namespace 场景体素模块 {

struct 结构_场景体素同步参数 {
    const 语素入口节点类* 三维体素特征类型 = nullptr;
    I64 场景体素坐标系版本 = 0;
    std::uint32_t 默认存在最小体素边长_mm = 0;
    时间戳 快照时间 = 0;
};

struct 结构_场景体素存在项 {
    可解析引用<存在节点类> 存在{};
    可解析引用<特征节点类> 三维体素特征{};
    VecU句柄 三维体素根句柄{};
    Vector3D 场景绝对坐标_mm{};
    I64 原点X_mm = 0;
    I64 原点Y_mm = 0;
    I64 原点Z_mm = 0;
    std::uint32_t 最小体素边长_mm = 0;
};

enum class 枚举_场景体素缺口类型 : I64 {
    无 = 0,
    缺体素特征类型 = 1,
    缺存在场景绝对坐标 = 2,
    缺三维体素特征 = 3,
    缺三维体素句柄 = 4,
    缺坐标系版本 = 5,
};

struct 结构_场景体素缺口项 {
    枚举_场景体素缺口类型 类型 = 枚举_场景体素缺口类型::无;
    I64 快照版本 = 0;
    I64 坐标系版本 = 0;
    可解析引用<场景节点类> 场景{};
    可解析引用<存在节点类> 存在{};
    可解析引用<特征节点类> 特征{};
};

struct 结构_场景体素快照统计 {
    I64 枚举存在数量 = 0;
    I64 有坐标存在数量 = 0;
    I64 有体素存在数量 = 0;
    I64 入选存在数量 = 0;
    I64 缺口数量 = 0;
    I64 缺体素特征类型数量 = 0;
    I64 缺坐标存在数量 = 0;
    I64 缺体素存在数量 = 0;
    I64 缺坐标系版本数量 = 0;
};

struct 结构_场景体素只读快照 {
    bool 只读状态 = true;
    I64 版本 = 0;
    I64 坐标系版本 = 0;
    时间戳 生成时间 = 0;
    可解析引用<场景节点类> 场景{};
    std::vector<结构_场景体素存在项> 存在项集合{};
    std::vector<结构_场景体素缺口项> 缺口集合{};
    结构_场景体素快照统计 统计{};
};

using 结构_SceneVoxelPrior = 结构_场景体素只读快照;

struct 结构_场景体素AABB {
    I64 最小X_mm = 0;
    I64 最小Y_mm = 0;
    I64 最小Z_mm = 0;
    I64 最大X_mm = 0;
    I64 最大Y_mm = 0;
    I64 最大Z_mm = 0;

    // 功能：判断 AABB 六个端点是否形成有效闭区间。
    bool 有效() const noexcept
    {
        return 最小X_mm <= 最大X_mm
            && 最小Y_mm <= 最大Y_mm
            && 最小Z_mm <= 最大Z_mm;
    }
};

struct 结构_场景体素AABB统计 {
    I64 查询存在数量 = 0;
    I64 命中存在数量 = 0;
};

struct 结构_场景体素射线粗查询参数 {
    I64 起点X_mm = 0;
    I64 起点Y_mm = 0;
    I64 起点Z_mm = 0;
    I64 方向X = 0;
    I64 方向Y = 0;
    I64 方向Z = 0;
    I64 最大距离_mm = 0;
    I64 最大垂距_mm = 0;
    I64 最大返回数量 = 0;
    bool 只取前向 = true;
};

struct 结构_场景体素射线粗命中项 {
    结构_场景体素存在项 存在项{};
    I64 投影距离_mm = 0;
    I64 垂距_mm = 0;
};

enum class 枚举_场景体素维护事件类型 : I64 {
    无 = 0,
    同步当前场景快照 = 1,
};

enum class 枚举_场景体素维护线程生命周期状态 : I64 {
    未启动 = 0,
    启动中 = 1,
    运行中 = 2,
    停止请求中 = 3,
    已停止 = 4,
    故障 = 5,
};

enum class 枚举_场景体素维护事件结果 : I64 {
    无 = 0,
    已同步快照 = 1,
    缺基础信息 = 2,
    缺场景 = 3,
    未知事件类型 = 4,
};

struct 结构_场景体素维护事件 {
    枚举_场景体素维护事件类型 类型 = 枚举_场景体素维护事件类型::无;
    基础信息类* 基础信息 = nullptr;
    const 场景节点类* 场景 = nullptr;
    结构_场景体素同步参数 同步参数{};
    时间戳 提交时间 = 0;
};

struct 结构_场景体素维护线程配置 {
    std::size_t 最大队列长度 = 64;
};

struct 结构_场景体素维护线程摘要 {
    枚举_场景体素维护线程生命周期状态 生命周期 =
        枚举_场景体素维护线程生命周期状态::未启动;
    bool 已启动 = false;
    bool 停止请求 = false;
    std::size_t 队列长度 = 0;
    std::uint64_t 已接收事件数量 = 0;
    std::uint64_t 已丢弃事件数量 = 0;
    std::uint64_t 已处理事件数量 = 0;
    std::uint64_t 成功同步次数 = 0;
    std::uint64_t 失败事件次数 = 0;
    枚举_场景体素维护事件结果 最近事件结果 = 枚举_场景体素维护事件结果::无;
    时间戳 最近事件时间 = 0;
    I64 最新快照版本 = 0;
    I64 最新快照坐标系版本 = 0;
};

namespace detail {
    // 功能：生成同步快照版本号，达到 I64 最大值后保持不再递增。
    inline I64 私有_下一个版本(I64& 当前版本) noexcept
    {
        if (当前版本 <= 0) 当前版本 = 1;
        const I64 输出版本 = 当前版本;
        if (当前版本 < (std::numeric_limits<I64>::max)()) {
            ++当前版本;
        }
        return 输出版本;
    }

    // 功能：使用调用方传入时间，未传入时使用当前微秒时间。
    inline 时间戳 私有_有效快照时间(时间戳 输入时间) noexcept
    {
        return 输入时间 != 0 ? 输入时间 : 结构体_时间戳::当前_微秒();
    }

    // 功能：判断存在原点是否落在 AABB 闭区间内。
    inline bool 私有_点在AABB内(
        const 结构_场景体素存在项& 项,
        const 结构_场景体素AABB& 范围) noexcept
    {
        return 范围.有效()
            && 项.原点X_mm >= 范围.最小X_mm
            && 项.原点X_mm <= 范围.最大X_mm
            && 项.原点Y_mm >= 范围.最小Y_mm
            && 项.原点Y_mm <= 范围.最大Y_mm
            && 项.原点Z_mm >= 范围.最小Z_mm
            && 项.原点Z_mm <= 范围.最大Z_mm;
    }

    // 功能：把非负双精度距离安全转换为 I64。
    inline bool 私有_非负双精度转I64(double 值, I64& 输出) noexcept
    {
        if (!std::isfinite(值) || 值 < 0.0) return false;
        return 坐标分量转I64(值, 输出);
    }

    // 功能：按调用方最大返回数量计算安全 resize 上限。
    inline std::size_t 私有_返回上限(I64 最大返回数量, std::size_t 当前数量) noexcept
    {
        if (最大返回数量 <= 0) return 当前数量;
        const auto 请求上限 = static_cast<std::uint64_t>(最大返回数量);
        const auto size上限 = static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)());
        const auto 安全上限 = static_cast<std::size_t>(std::min(请求上限, size上限));
        return std::min(安全上限, 当前数量);
    }
}

class 场景体素缓存类 {
public:
    // 功能：创建同步版场景体素缓存服务，可延后绑定基础信息。
    explicit 场景体素缓存类(基础信息类* 基础信息 = nullptr) noexcept
        : 基础信息_(基础信息)
    {
    }

    // 功能：绑定基础信息并清空当前运行期快照缓存。
    void 绑定基础信息(基础信息类* 基础信息) noexcept
    {
        std::lock_guard<std::mutex> 锁(快照互斥_);
        基础信息_ = 基础信息;
        最新快照_ = {};
        下一版本_ = 1;
    }

    // 功能：从当前场景子存在只读生成 SceneVoxelPrior 快照，不创建或修改世界树事实。
    结构_场景体素只读快照 同步当前场景快照(
        const 场景节点类* 场景,
        const 结构_场景体素同步参数& 参数)
    {
        结构_场景体素只读快照 快照{};
        基础信息类* 当前基础信息 = nullptr;
        {
            std::lock_guard<std::mutex> 锁(快照互斥_);
            当前基础信息 = 基础信息_;
            快照.版本 = detail::私有_下一个版本(下一版本_);
        }
        快照.坐标系版本 = 参数.场景体素坐标系版本;
        快照.生成时间 = detail::私有_有效快照时间(参数.快照时间);
        快照.场景 = const_cast<场景节点类*>(场景);

        if (!当前基础信息 || !场景) {
            std::lock_guard<std::mutex> 锁(快照互斥_);
            if (快照.版本 >= 最新快照_.版本) {
                最新快照_ = std::move(快照);
            }
            return 最新快照_;
        }

        auto 追加缺口 = [&](枚举_场景体素缺口类型 类型, 存在节点类* 存在, 特征节点类* 特征) {
            结构_场景体素缺口项 缺口{};
            缺口.类型 = 类型;
            缺口.快照版本 = 快照.版本;
            缺口.坐标系版本 = 快照.坐标系版本;
            缺口.场景 = const_cast<场景节点类*>(场景);
            缺口.存在 = 存在;
            缺口.特征 = 特征;
            快照.缺口集合.push_back(std::move(缺口));
        };

        if (参数.场景体素坐标系版本 <= 0) {
            ++快照.统计.缺坐标系版本数量;
            追加缺口(枚举_场景体素缺口类型::缺坐标系版本, nullptr, nullptr);
        }

        场景类 场景服务(当前基础信息);
        存在类 存在服务(当前基础信息);
        特征类 特征服务(当前基础信息);
        const auto 子存在集合 = 场景服务.获取子存在(场景);
        快照.统计.枚举存在数量 = static_cast<I64>(std::min<std::size_t>(
            子存在集合.size(),
            static_cast<std::size_t>((std::numeric_limits<I64>::max)())));

        for (auto* 子存在 : 子存在集合) {
            if (!子存在) continue;

            bool 有体素 = false;
            特征节点类* 三维体素特征 = nullptr;
            VecU句柄 三维体素根句柄{};
            if (!参数.三维体素特征类型) {
                ++快照.统计.缺体素特征类型数量;
                ++快照.统计.缺体素存在数量;
                追加缺口(枚举_场景体素缺口类型::缺体素特征类型, 子存在, nullptr);
            } else {
                三维体素特征 = 特征服务.查找子特征_按类型(
                    子存在,
                    参数.三维体素特征类型);
                有体素 = 特征服务.读取三维体素特征值(三维体素特征, 三维体素根句柄);
                if (有体素) {
                    ++快照.统计.有体素存在数量;
                } else {
                    ++快照.统计.缺体素存在数量;
                    追加缺口(
                        三维体素特征
                            ? 枚举_场景体素缺口类型::缺三维体素句柄
                            : 枚举_场景体素缺口类型::缺三维体素特征,
                        子存在,
                        三维体素特征);
                }
            }

            Vector3D 坐标{};
            I64 x = 0;
            I64 y = 0;
            I64 z = 0;
            const bool 有坐标 = 存在服务.读取存在场景绝对坐标(子存在, 坐标)
                && 坐标转I64三元(坐标, x, y, z);
            if (有坐标) {
                ++快照.统计.有坐标存在数量;
            } else {
                ++快照.统计.缺坐标存在数量;
                追加缺口(枚举_场景体素缺口类型::缺存在场景绝对坐标, 子存在, nullptr);
            }

            if (!有体素 || !有坐标) continue;

            结构_场景体素存在项 项{};
            项.存在 = 子存在;
            项.三维体素特征 = 三维体素特征;
            项.三维体素根句柄 = 三维体素根句柄;
            项.场景绝对坐标_mm = 坐标;
            项.原点X_mm = x;
            项.原点Y_mm = y;
            项.原点Z_mm = z;
            项.最小体素边长_mm = 参数.默认存在最小体素边长_mm;
            快照.存在项集合.push_back(std::move(项));
        }

        快照.统计.入选存在数量 = static_cast<I64>(std::min<std::size_t>(
            快照.存在项集合.size(),
            static_cast<std::size_t>((std::numeric_limits<I64>::max)())));
        快照.统计.缺口数量 = static_cast<I64>(std::min<std::size_t>(
            快照.缺口集合.size(),
            static_cast<std::size_t>((std::numeric_limits<I64>::max)())));
        std::lock_guard<std::mutex> 锁(快照互斥_);
        if (快照.版本 >= 最新快照_.版本) {
            最新快照_ = std::move(快照);
        }
        return 最新快照_;
    }

    // 功能：读取最近一次同步生成的只读快照副本，不返回可写内部缓存引用。
    结构_场景体素只读快照 读取最新快照() const
    {
        std::lock_guard<std::mutex> 锁(快照互斥_);
        return 最新快照_;
    }

    // 功能：查询快照中位于 AABB 内的存在原点，不读取或修改世界树事实。
    static std::vector<结构_场景体素存在项> 查询AABB(
        const 结构_场景体素只读快照& 快照,
        const 结构_场景体素AABB& 范围,
        I64 最大返回数量 = 0)
    {
        std::vector<结构_场景体素存在项> 结果{};
        if (!范围.有效()) return 结果;
        for (const auto& 项 : 快照.存在项集合) {
            if (detail::私有_点在AABB内(项, 范围)) {
                结果.push_back(项);
            }
        }
        const auto 上限 = detail::私有_返回上限(最大返回数量, 结果.size());
        if (结果.size() > 上限) 结果.resize(上限);
        return 结果;
    }

    // 功能：在最新快照中查询 AABB 内存在原点，不返回可写内部缓存引用。
    std::vector<结构_场景体素存在项> 查询AABB(
        const 结构_场景体素AABB& 范围,
        I64 最大返回数量 = 0) const
    {
        return 查询AABB(读取最新快照(), 范围, 最大返回数量);
    }

    // 功能：统计快照中位于 AABB 内的存在原点数量，不创建任何缺口节点。
    static 结构_场景体素AABB统计 统计AABB(
        const 结构_场景体素只读快照& 快照,
        const 结构_场景体素AABB& 范围)
    {
        结构_场景体素AABB统计 统计{};
        统计.查询存在数量 = static_cast<I64>(std::min<std::size_t>(
            快照.存在项集合.size(),
            static_cast<std::size_t>((std::numeric_limits<I64>::max)())));
        if (!范围.有效()) return 统计;
        for (const auto& 项 : 快照.存在项集合) {
            if (detail::私有_点在AABB内(项, 范围)) {
                ++统计.命中存在数量;
            }
        }
        return 统计;
    }

    // 功能：统计最新快照中位于 AABB 内的存在原点数量。
    结构_场景体素AABB统计 统计AABB(const 结构_场景体素AABB& 范围) const
    {
        return 统计AABB(读取最新快照(), 范围);
    }

    // 功能：按点附近立方范围查询存在原点，只作为视觉先验粗过滤。
    static std::vector<结构_场景体素存在项> 查询点附近(
        const 结构_场景体素只读快照& 快照,
        I64 x_mm,
        I64 y_mm,
        I64 z_mm,
        I64 半径_mm,
        I64 最大返回数量 = 0)
    {
        const I64 半径 = std::max<I64>(0, 半径_mm);
        结构_场景体素AABB 范围{};
        范围.最小X_mm = 饱和减少(x_mm, 半径);
        范围.最大X_mm = 饱和增加(x_mm, 半径);
        范围.最小Y_mm = 饱和减少(y_mm, 半径);
        范围.最大Y_mm = 饱和增加(y_mm, 半径);
        范围.最小Z_mm = 饱和减少(z_mm, 半径);
        范围.最大Z_mm = 饱和增加(z_mm, 半径);
        return 查询AABB(快照, 范围, 最大返回数量);
    }

    // 功能：在最新快照中按点附近立方范围查询存在原点。
    std::vector<结构_场景体素存在项> 查询点附近(
        I64 x_mm,
        I64 y_mm,
        I64 z_mm,
        I64 半径_mm,
        I64 最大返回数量 = 0) const
    {
        return 查询点附近(读取最新快照(), x_mm, y_mm, z_mm, 半径_mm, 最大返回数量);
    }

    // 功能：用存在原点到射线的垂距做粗查询，只作为先验过滤，不证明遮挡或命中事实。
    static std::vector<结构_场景体素射线粗命中项> 射线粗查询(
        const 结构_场景体素只读快照& 快照,
        const 结构_场景体素射线粗查询参数& 参数)
    {
        std::vector<结构_场景体素射线粗命中项> 结果{};
        const double dx = static_cast<double>(参数.方向X);
        const double dy = static_cast<double>(参数.方向Y);
        const double dz = static_cast<double>(参数.方向Z);
        const double 长度平方 = dx * dx + dy * dy + dz * dz;
        if (长度平方 <= 0.0 || !std::isfinite(长度平方)) return 结果;
        const double 长度 = std::sqrt(长度平方);
        const double 最大垂距 = static_cast<double>(std::max<I64>(0, 参数.最大垂距_mm));
        const double 最大距离 = static_cast<double>(std::max<I64>(0, 参数.最大距离_mm));

        for (const auto& 项 : 快照.存在项集合) {
            const double px = static_cast<double>(项.原点X_mm) - static_cast<double>(参数.起点X_mm);
            const double py = static_cast<double>(项.原点Y_mm) - static_cast<double>(参数.起点Y_mm);
            const double pz = static_cast<double>(项.原点Z_mm) - static_cast<double>(参数.起点Z_mm);
            const double 投影参数 = (px * dx + py * dy + pz * dz) / 长度平方;
            if (参数.只取前向 && 投影参数 < 0.0) continue;

            const double 投影距离 = 投影参数 * 长度;
            if (最大距离 > 0.0 && 投影距离 > 最大距离) continue;

            const double 最近X = 投影参数 * dx;
            const double 最近Y = 投影参数 * dy;
            const double 最近Z = 投影参数 * dz;
            const double 垂距 = std::sqrt(
                (px - 最近X) * (px - 最近X)
                + (py - 最近Y) * (py - 最近Y)
                + (pz - 最近Z) * (pz - 最近Z));
            if (最大垂距 > 0.0 && 垂距 > 最大垂距) continue;

            I64 投影距离I64 = 0;
            I64 垂距I64 = 0;
            if (!detail::私有_非负双精度转I64(std::max(0.0, 投影距离), 投影距离I64)
                || !detail::私有_非负双精度转I64(垂距, 垂距I64)) {
                continue;
            }

            结构_场景体素射线粗命中项 命中{};
            命中.存在项 = 项;
            命中.投影距离_mm = 投影距离I64;
            命中.垂距_mm = 垂距I64;
            结果.push_back(std::move(命中));
        }

        std::stable_sort(
            结果.begin(),
            结果.end(),
            [](const 结构_场景体素射线粗命中项& 左,
               const 结构_场景体素射线粗命中项& 右) {
                if (左.投影距离_mm != 右.投影距离_mm) {
                    return 左.投影距离_mm < 右.投影距离_mm;
                }
                if (左.垂距_mm != 右.垂距_mm) {
                    return 左.垂距_mm < 右.垂距_mm;
                }
                const auto 左键 = 左.存在项.存在.主键;
                const auto 右键 = 右.存在项.存在.主键;
                return 左键 < 右键;
            });

        const auto 上限 = detail::私有_返回上限(参数.最大返回数量, 结果.size());
        if (结果.size() > 上限) 结果.resize(上限);
        return 结果;
    }

    // 功能：在最新快照中执行射线粗查询，只返回运行期先验候选。
    std::vector<结构_场景体素射线粗命中项> 射线粗查询(
        const 结构_场景体素射线粗查询参数& 参数) const
    {
        return 射线粗查询(读取最新快照(), 参数);
    }

private:
    基础信息类* 基础信息_ = nullptr;
    I64 下一版本_ = 1;
    结构_场景体素只读快照 最新快照_{};
    mutable std::mutex 快照互斥_{};
};

class 场景体素维护线程类 {
public:
    场景体素维护线程类() = default;

    // 功能：析构时请求维护线程停止并等待线程收口。
    ~场景体素维护线程类()
    {
        请求停止();
        等待停止();
    }

    场景体素维护线程类(const 场景体素维护线程类&) = delete;
    场景体素维护线程类& operator=(const 场景体素维护线程类&) = delete;

    // 功能：启动场景体素维护线程，只维护运行期缓存和快照。
    bool 启动(const 结构_场景体素维护线程配置& 配置 = {})
    {
        {
            std::lock_guard<std::mutex> 锁(状态互斥_);
            if (生命周期_ == 枚举_场景体素维护线程生命周期状态::启动中
                || 生命周期_ == 枚举_场景体素维护线程生命周期状态::运行中) {
                return true;
            }
            if (工作线程_.joinable()) {
                return false;
            }

            配置_ = 配置;
            停止请求_ = false;
            生命周期_ = 枚举_场景体素维护线程生命周期状态::启动中;
        }

        try {
            工作线程_ = std::thread(&场景体素维护线程类::主循环_, this);
            return true;
        } catch (...) {
            std::lock_guard<std::mutex> 锁(状态互斥_);
            生命周期_ = 枚举_场景体素维护线程生命周期状态::故障;
            停止请求_ = true;
            return false;
        }
    }

    // 功能：请求场景体素维护线程停止，已入队事件由线程自然收口。
    void 请求停止()
    {
        {
            std::lock_guard<std::mutex> 锁(状态互斥_);
            if (生命周期_ == 枚举_场景体素维护线程生命周期状态::未启动
                || 生命周期_ == 枚举_场景体素维护线程生命周期状态::已停止) {
                return;
            }
            停止请求_ = true;
            生命周期_ = 枚举_场景体素维护线程生命周期状态::停止请求中;
        }
        条件_.notify_all();
    }

    // 功能：等待场景体素维护线程退出。
    void 等待停止()
    {
        if (工作线程_.joinable()) {
            工作线程_.join();
        }
    }

    // 功能：提交维护事件，只进入运行期事件队列，不写世界树事实。
    bool 提交事件(结构_场景体素维护事件 事件)
    {
        {
            std::lock_guard<std::mutex> 锁(状态互斥_);
            if (生命周期_ != 枚举_场景体素维护线程生命周期状态::启动中
                && 生命周期_ != 枚举_场景体素维护线程生命周期状态::运行中) {
                return false;
            }
            if (事件.提交时间 == 0) {
                事件.提交时间 = 结构体_时间戳::当前_微秒();
            }

            const std::size_t 队列上限 = std::max<std::size_t>(1, 配置_.最大队列长度);
            while (事件队列_.size() >= 队列上限) {
                事件队列_.pop_front();
                ++已丢弃事件数量_;
            }
            事件队列_.push_back(std::move(事件));
            ++已接收事件数量_;
        }
        条件_.notify_one();
        return true;
    }

    // 功能：读取维护线程生命周期状态。
    枚举_场景体素维护线程生命周期状态 读取生命周期状态() const
    {
        std::lock_guard<std::mutex> 锁(状态互斥_);
        return 生命周期_;
    }

    // 功能：读取维护线程摘要和最新快照版本，不返回内部队列引用。
    结构_场景体素维护线程摘要 读取摘要() const
    {
        结构_场景体素维护线程摘要 摘要{};
        {
            std::lock_guard<std::mutex> 锁(状态互斥_);
            摘要.生命周期 = 生命周期_;
            摘要.已启动 =
                生命周期_ == 枚举_场景体素维护线程生命周期状态::启动中
                || 生命周期_ == 枚举_场景体素维护线程生命周期状态::运行中
                || 生命周期_ == 枚举_场景体素维护线程生命周期状态::停止请求中;
            摘要.停止请求 = 停止请求_;
            摘要.队列长度 = 事件队列_.size();
            摘要.已接收事件数量 = 已接收事件数量_;
            摘要.已丢弃事件数量 = 已丢弃事件数量_;
            摘要.已处理事件数量 = 已处理事件数量_;
            摘要.成功同步次数 = 成功同步次数_;
            摘要.失败事件次数 = 失败事件次数_;
            摘要.最近事件结果 = 最近事件结果_;
            摘要.最近事件时间 = 最近事件时间_;
        }
        const auto 快照 = 缓存_.读取最新快照();
        摘要.最新快照版本 = 快照.版本;
        摘要.最新快照坐标系版本 = 快照.坐标系版本;
        return 摘要;
    }

    // 功能：读取维护线程维护的最新快照副本。
    结构_场景体素只读快照 读取最新快照() const
    {
        return 缓存_.读取最新快照();
    }

private:
    // 功能：处理单个维护事件，只调用只读快照同步逻辑。
    枚举_场景体素维护事件结果 处理事件_(结构_场景体素维护事件& 事件)
    {
        if (事件.类型 != 枚举_场景体素维护事件类型::同步当前场景快照) {
            return 枚举_场景体素维护事件结果::未知事件类型;
        }
        if (!事件.基础信息) {
            return 枚举_场景体素维护事件结果::缺基础信息;
        }
        if (!事件.场景) {
            return 枚举_场景体素维护事件结果::缺场景;
        }

        if (当前基础信息_ != 事件.基础信息) {
            缓存_.绑定基础信息(事件.基础信息);
            当前基础信息_ = 事件.基础信息;
        }

        if (事件.同步参数.快照时间 == 0) {
            事件.同步参数.快照时间 = 结构体_时间戳::当前_微秒();
        }
        (void)缓存_.同步当前场景快照(事件.场景, 事件.同步参数);
        return 枚举_场景体素维护事件结果::已同步快照;
    }

    // 功能：维护线程主循环，串行消费快照维护事件。
    void 主循环_()
    {
        {
            std::lock_guard<std::mutex> 锁(状态互斥_);
            if (生命周期_ == 枚举_场景体素维护线程生命周期状态::启动中) {
                生命周期_ = 枚举_场景体素维护线程生命周期状态::运行中;
            }
        }

        for (;;) {
            结构_场景体素维护事件 事件{};
            {
                std::unique_lock<std::mutex> 锁(状态互斥_);
                条件_.wait(锁, [&]() {
                    return 停止请求_ || !事件队列_.empty();
                });
                if (事件队列_.empty()) {
                    if (停止请求_) {
                        break;
                    }
                    continue;
                }
                事件 = std::move(事件队列_.front());
                事件队列_.pop_front();
            }

            const auto 结果 = 处理事件_(事件);
            const bool 成功 = 结果 == 枚举_场景体素维护事件结果::已同步快照;
            {
                std::lock_guard<std::mutex> 锁(状态互斥_);
                ++已处理事件数量_;
                if (成功) {
                    ++成功同步次数_;
                } else {
                    ++失败事件次数_;
                }
                最近事件结果_ = 结果;
                最近事件时间_ = 事件.提交时间 != 0
                    ? 事件.提交时间
                    : 结构体_时间戳::当前_微秒();
            }
        }

        {
            std::lock_guard<std::mutex> 锁(状态互斥_);
            停止请求_ = false;
            生命周期_ = 枚举_场景体素维护线程生命周期状态::已停止;
        }
    }

private:
    mutable std::mutex 状态互斥_{};
    std::condition_variable 条件_{};
    std::thread 工作线程_{};
    结构_场景体素维护线程配置 配置_{};
    bool 停止请求_ = false;
    枚举_场景体素维护线程生命周期状态 生命周期_ =
        枚举_场景体素维护线程生命周期状态::未启动;
    std::deque<结构_场景体素维护事件> 事件队列_{};
    std::uint64_t 已接收事件数量_ = 0;
    std::uint64_t 已丢弃事件数量_ = 0;
    std::uint64_t 已处理事件数量_ = 0;
    std::uint64_t 成功同步次数_ = 0;
    std::uint64_t 失败事件次数_ = 0;
    枚举_场景体素维护事件结果 最近事件结果_ = 枚举_场景体素维护事件结果::无;
    时间戳 最近事件时间_ = 0;
    基础信息类* 当前基础信息_ = nullptr;
    场景体素缓存类 缓存_{};
};

// 功能：读取全局场景体素维护线程单例。
inline 场景体素维护线程类& 获取全局场景体素维护线程() noexcept
{
    static 场景体素维护线程类 线程;
    return 线程;
}

// 功能：启动全局场景体素维护线程。
inline bool 启动场景体素维护线程(const 结构_场景体素维护线程配置& 配置 = {})
{
    return 获取全局场景体素维护线程().启动(配置);
}

// 功能：停止全局场景体素维护线程。
inline void 停止场景体素维护线程()
{
    auto& 线程 = 获取全局场景体素维护线程();
    线程.请求停止();
    线程.等待停止();
}

// 功能：提交场景体素维护事件到全局线程队列。
inline bool 提交场景体素维护事件(结构_场景体素维护事件 事件)
{
    return 获取全局场景体素维护线程().提交事件(std::move(事件));
}

// 功能：读取全局场景体素维护线程摘要。
inline 结构_场景体素维护线程摘要 读取场景体素维护线程摘要()
{
    return 获取全局场景体素维护线程().读取摘要();
}

// 功能：读取全局场景体素维护线程维护的最新只读快照。
inline 结构_场景体素只读快照 读取场景体素维护线程最新快照()
{
    return 获取全局场景体素维护线程().读取最新快照();
}

} // namespace 场景体素模块
