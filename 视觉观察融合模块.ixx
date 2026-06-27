// 文件头部规则注释模块：
// 1. 本模块只把合法外设材料和 SceneVoxelPrior 融合为观察域候选、评分、解释和缺口。
// 2. 本模块不得创建存在、确认消失、写一阶特征、写需求满足、写价值结算或生成动作动态。
// 3. 业务入口接入前必须经 6145 / 6146 / 6147 / 6150 自身切片裁决，不得绕过质量门和提交入口。

module;

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "基础信息类.h"

export module 视觉观察融合模块;

import 全局共享函数类;
import 外设观察报告队列;
import 场景体素模块;

export namespace 视觉观察融合模块 {

enum class 枚举_视觉观察解释类型 : I64 {
    未定义 = 0,
    先验匹配 = 1,
    视野外 = 2,
    遮挡候选 = 3,
    深度缺失 = 4,
    自由空间冲突 = 5,
    证据不足 = 6,
};

enum class 枚举_视觉信息缺口类型 : I64 {
    无 = 0,
    缺SceneVoxelPrior = 1,
    缺合法外设材料 = 2,
    材料空间范围无效 = 3,
    材料不可比较 = 4,
    深度缺失 = 5,
    先验坐标系版本缺失 = 6,
    先验结构缺口 = 7,
    证据不足 = 8,
};

struct 结构_视觉融合目标约束摘要 {
    std::uint64_t 约束ID = 0;
    std::uint64_t 来源报告ID = 0;
    std::uint64_t 来源帧ID = 0;
    I64 最低先验匹配评分Q10000 = 0;
    I64 最低深度一致评分Q10000 = 0;
    I64 最低材料质量Q10000 = 0;
    I64 最大先验年龄毫秒 = 0;
};

struct 结构_视觉融合合法外设材料项 {
    std::uint64_t 数据项ID = 0;
    std::uint64_t 来源报告ID = 0;
    std::uint64_t 来源帧ID = 0;
    std::uint64_t 已确认存在ID = 0;
    std::uint64_t 约束ID = 0;
    结构_外设空间中心 当前中心{};
    结构_外设空间范围AABB 当前空间范围{};
    bool 空间中心有效 = false;
    bool 空间范围有效 = false;
    bool 可回查 = false;
    bool 可比较 = false;
    bool 可生成候选 = false;
    I64 材料质量Q10000 = 0;
    I64 深度有效率Q10000 = 0;
    I64 空间坐标有效率Q10000 = 0;
};

struct 结构_视觉先验匹配评分 {
    std::uint64_t 数据项ID = 0;
    I64 先验版本 = 0;
    I64 先验坐标系版本 = 0;
    I64 AABB命中数量 = 0;
    I64 最近先验距离_mm = 0;
    I64 空间重叠评分Q10000 = 0;
    I64 深度一致评分Q10000 = 0;
    I64 材料质量评分Q10000 = 0;
    I64 综合评分Q10000 = 0;
    可解析引用<存在节点类> 最佳先验存在{};
};

struct 结构_视觉自由空间冲突候选 {
    std::uint64_t 数据项ID = 0;
    I64 先验版本 = 0;
    I64 冲突数量 = 0;
    I64 材料质量Q10000 = 0;
    结构_外设空间范围AABB 当前空间范围{};
};

struct 结构_视觉信息缺口 {
    枚举_视觉信息缺口类型 类型 = 枚举_视觉信息缺口类型::无;
    std::uint64_t 数据项ID = 0;
    std::uint64_t 来源报告ID = 0;
    std::uint64_t 来源帧ID = 0;
    I64 先验版本 = 0;
    I64 证据值 = 0;
    可解析引用<存在节点类> 相关存在{};
};

struct 结构_视觉观察候选 {
    std::uint64_t 数据项ID = 0;
    枚举_视觉观察解释类型 解释类型 = 枚举_视觉观察解释类型::未定义;
    结构_视觉先验匹配评分 先验评分{};
};

struct 结构_视觉观察域结果包 {
    I64 先验版本 = 0;
    I64 先验坐标系版本 = 0;
    I64 合法材料数量 = 0;
    I64 候选数量 = 0;
    I64 先验匹配数量 = 0;
    I64 自由空间冲突候选数量 = 0;
    I64 信息缺口数量 = 0;
    I64 遮挡候选数量 = 0;
    I64 深度缺失数量 = 0;
    std::vector<结构_视觉先验匹配评分> 先验匹配评分集合{};
    std::vector<结构_视觉自由空间冲突候选> 自由空间冲突候选集合{};
    std::vector<结构_视觉信息缺口> 信息缺口集合{};
    std::vector<结构_视觉观察候选> 候选集合{};
};

struct 结构_视觉融合输入 {
    结构_视觉融合目标约束摘要 目标约束{};
    std::vector<结构_视觉融合合法外设材料项> 合法外设材料集合{};
    场景体素模块::结构_SceneVoxelPrior SceneVoxelPrior{};
};

namespace detail {
    // 功能：判断外设空间 AABB 是否为有效闭区间。
    inline bool 外设AABB有效(const 结构_外设空间范围AABB& 范围) noexcept
    {
        return 范围.最小X <= 范围.最大X
            && 范围.最小Y <= 范围.最大Y
            && 范围.最小Z <= 范围.最大Z;
    }

    // 功能：把外设 AABB 转为场景体素 AABB 查询结构。
    inline 场景体素模块::结构_场景体素AABB 转场景体素AABB(
        const 结构_外设空间范围AABB& 范围) noexcept
    {
        return 场景体素模块::结构_场景体素AABB{
            范围.最小X,
            范围.最小Y,
            范围.最小Z,
            范围.最大X,
            范围.最大Y,
            范围.最大Z,
        };
    }

    // 功能：把外设置信度比例转换为 Q10000 评分。
    inline I64 比例转Q10000(double 比例) noexcept
    {
        if (!std::isfinite(比例) || 比例 <= 0.0) return 0;
        if (比例 > 1.0) 比例 = 1.0;
        return 限制I64(static_cast<I64>(std::llround(比例 * 10000.0)), 0, 10000);
    }

    // 功能：按材料中心和先验存在原点计算 L1 距离。
    inline I64 材料到先验L1距离(
        const 结构_视觉融合合法外设材料项& 材料,
        const 场景体素模块::结构_场景体素存在项& 先验项) noexcept
    {
        const auto dx = 饱和绝对差I64(材料.当前中心.X, 先验项.原点X_mm);
        const auto dy = 饱和绝对差I64(材料.当前中心.Y, 先验项.原点Y_mm);
        const auto dz = 饱和绝对差I64(材料.当前中心.Z, 先验项.原点Z_mm);
        return 饱和增加(饱和增加(dx, dy), dz);
    }

    // 功能：选择离材料中心最近的先验存在项。
    inline const 场景体素模块::结构_场景体素存在项* 选择最近先验项(
        const 结构_视觉融合合法外设材料项& 材料,
        const std::vector<场景体素模块::结构_场景体素存在项>& 命中集合,
        I64& 输出距离)
    {
        输出距离 = 0;
        if (命中集合.empty()) return nullptr;
        const 场景体素模块::结构_场景体素存在项* 最佳 = nullptr;
        I64 最佳距离 = (std::numeric_limits<I64>::max)();
        for (const auto& 项 : 命中集合) {
            const I64 距离 = 材料到先验L1距离(材料, 项);
            if (!最佳 || 距离 < 最佳距离) {
                最佳 = &项;
                最佳距离 = 距离;
            }
        }
        输出距离 = 最佳 ? 最佳距离 : 0;
        return 最佳;
    }

    // 功能：追加结构化视觉信息缺口。
    inline void 追加信息缺口(
        结构_视觉观察域结果包& 输出,
        枚举_视觉信息缺口类型 类型,
        const 结构_视觉融合合法外设材料项* 材料,
        I64 先验版本,
        I64 证据值,
        存在节点类* 相关存在 = nullptr)
    {
        结构_视觉信息缺口 缺口{};
        缺口.类型 = 类型;
        缺口.先验版本 = 先验版本;
        缺口.证据值 = 证据值;
        缺口.相关存在 = 相关存在;
        if (材料) {
            缺口.数据项ID = 材料->数据项ID;
            缺口.来源报告ID = 材料->来源报告ID;
            缺口.来源帧ID = 材料->来源帧ID;
        }
        输出.信息缺口集合.push_back(std::move(缺口));
    }

    // 功能：按材料、先验命中和阈值生成观察解释类型。
    inline 枚举_视觉观察解释类型 判断解释类型(
        const 结构_视觉融合合法外设材料项& 材料,
        const 结构_视觉先验匹配评分& 评分,
        const 结构_视觉融合目标约束摘要& 约束) noexcept
    {
        if (材料.深度有效率Q10000 <= 0) return 枚举_视觉观察解释类型::深度缺失;
        if (评分.AABB命中数量 <= 0) return 枚举_视觉观察解释类型::自由空间冲突;
        if (评分.综合评分Q10000 >= std::max<I64>(0, 约束.最低先验匹配评分Q10000)) {
            return 枚举_视觉观察解释类型::先验匹配;
        }
        if (材料.材料质量Q10000 < 5000) return 枚举_视觉观察解释类型::遮挡候选;
        return 枚举_视觉观察解释类型::证据不足;
    }
}

class 视觉观察融合类 {
public:
    // 功能：从扫描提交包提取公开合法外设材料项，供融合模块运行期使用。
    static std::vector<结构_视觉融合合法外设材料项> 从扫描提交包提取合法外设材料(
        const 结构_外设扫描提交包& 扫描包)
    {
        std::vector<结构_视觉融合合法外设材料项> 输出{};
        std::uint64_t 序号 = 0;
        for (const auto& 已知 : 扫描包.已知存在结果) {
            结构_视觉融合合法外设材料项 项{};
            项.数据项ID = ++序号;
            项.来源报告ID = 扫描包.包头.报告ID;
            项.来源帧ID = 扫描包.包头.帧ID != 0 ? 扫描包.包头.帧ID : 扫描包.当前帧ID;
            项.已确认存在ID = 已知.已确认存在ID;
            项.当前中心 = 已知.当前中心;
            项.当前空间范围 = 已知.当前空间范围;
            项.空间中心有效 = 已知.当前中心.X != 0 || 已知.当前中心.Y != 0 || 已知.当前中心.Z != 0;
            项.空间范围有效 = detail::外设AABB有效(已知.当前空间范围);
            项.可回查 = !已知.当前点集句柄.empty() || !已知.当前深度切片句柄.empty();
            项.可比较 = 项.空间范围有效 && 项.可回查;
            项.可生成候选 = 项.可比较;
            项.材料质量Q10000 = detail::比例转Q10000(已知.匹配置信度);
            项.深度有效率Q10000 = 扫描包.深度有效率 > 0.0
                ? detail::比例转Q10000(扫描包.深度有效率)
                : (已知.当前深度切片句柄.empty() ? 0 : 10000);
            项.空间坐标有效率Q10000 = 扫描包.空间坐标有效率 > 0.0
                ? detail::比例转Q10000(扫描包.空间坐标有效率)
                : (项.空间范围有效 ? 10000 : 0);
            输出.push_back(std::move(项));
        }
        return 输出;
    }

    // 功能：融合合法外设材料和 SceneVoxelPrior，输出观察域候选、评分、解释和缺口。
    static 结构_视觉观察域结果包 融合观察域结果(const 结构_视觉融合输入& 输入)
    {
        结构_视觉观察域结果包 输出{};
        输出.先验版本 = 输入.SceneVoxelPrior.版本;
        输出.先验坐标系版本 = 输入.SceneVoxelPrior.坐标系版本;
        输出.合法材料数量 = static_cast<I64>(std::min<std::size_t>(
            输入.合法外设材料集合.size(),
            static_cast<std::size_t>((std::numeric_limits<I64>::max)())));

        if (输入.SceneVoxelPrior.版本 <= 0) {
            detail::追加信息缺口(
                输出,
                枚举_视觉信息缺口类型::缺SceneVoxelPrior,
                nullptr,
                输入.SceneVoxelPrior.版本,
                输入.SceneVoxelPrior.版本);
        }
        if (输入.SceneVoxelPrior.坐标系版本 <= 0) {
            detail::追加信息缺口(
                输出,
                枚举_视觉信息缺口类型::先验坐标系版本缺失,
                nullptr,
                输入.SceneVoxelPrior.版本,
                输入.SceneVoxelPrior.坐标系版本);
        }
        for (const auto& 先验缺口 : 输入.SceneVoxelPrior.缺口集合) {
            detail::追加信息缺口(
                输出,
                枚举_视觉信息缺口类型::先验结构缺口,
                nullptr,
                输入.SceneVoxelPrior.版本,
                static_cast<I64>(先验缺口.类型),
                先验缺口.存在.指针);
        }
        if (输入.合法外设材料集合.empty()) {
            detail::追加信息缺口(
                输出,
                枚举_视觉信息缺口类型::缺合法外设材料,
                nullptr,
                输入.SceneVoxelPrior.版本,
                0);
        }

        for (const auto& 材料 : 输入.合法外设材料集合) {
            if (!材料.空间范围有效 || !detail::外设AABB有效(材料.当前空间范围)) {
                detail::追加信息缺口(
                    输出,
                    枚举_视觉信息缺口类型::材料空间范围无效,
                    &材料,
                    输入.SceneVoxelPrior.版本,
                    0);
                continue;
            }
            if (!材料.可比较) {
                detail::追加信息缺口(
                    输出,
                    枚举_视觉信息缺口类型::材料不可比较,
                    &材料,
                    输入.SceneVoxelPrior.版本,
                    0);
                continue;
            }
            if (材料.深度有效率Q10000 <= 0) {
                ++输出.深度缺失数量;
                detail::追加信息缺口(
                    输出,
                    枚举_视觉信息缺口类型::深度缺失,
                    &材料,
                    输入.SceneVoxelPrior.版本,
                    材料.深度有效率Q10000);
            }

            const auto 查询范围 = detail::转场景体素AABB(材料.当前空间范围);
            const auto 命中集合 = 场景体素模块::场景体素缓存类::查询AABB(
                输入.SceneVoxelPrior,
                查询范围);

            I64 最近距离 = 0;
            const auto* 最佳先验 = detail::选择最近先验项(材料, 命中集合, 最近距离);

            结构_视觉先验匹配评分 评分{};
            评分.数据项ID = 材料.数据项ID;
            评分.先验版本 = 输入.SceneVoxelPrior.版本;
            评分.先验坐标系版本 = 输入.SceneVoxelPrior.坐标系版本;
            评分.AABB命中数量 = static_cast<I64>(std::min<std::size_t>(
                命中集合.size(),
                static_cast<std::size_t>((std::numeric_limits<I64>::max)())));
            评分.最近先验距离_mm = 最近距离;
            评分.空间重叠评分Q10000 = 评分.AABB命中数量 > 0 ? 10000 : 0;
            评分.深度一致评分Q10000 = 最佳先验
                ? 深度差异评分(饱和绝对差I64(材料.当前中心.Z, 最佳先验->原点Z_mm))
                : 0;
            评分.材料质量评分Q10000 = 限制I64(材料.材料质量Q10000, 0, 10000);
            评分.综合评分Q10000 = 限制I64(
                (评分.空间重叠评分Q10000 + 评分.深度一致评分Q10000 + 评分.材料质量评分Q10000) / 3,
                0,
                10000);
            if (最佳先验) 评分.最佳先验存在 = 最佳先验->存在.指针;

            const auto 解释类型 = detail::判断解释类型(材料, 评分, 输入.目标约束);
            结构_视觉观察候选 候选{};
            候选.数据项ID = 材料.数据项ID;
            候选.解释类型 = 解释类型;
            候选.先验评分 = 评分;
            输出.候选集合.push_back(std::move(候选));
            输出.先验匹配评分集合.push_back(评分);

            if (评分.AABB命中数量 > 0) {
                ++输出.先验匹配数量;
            }
            if (解释类型 == 枚举_视觉观察解释类型::遮挡候选) {
                ++输出.遮挡候选数量;
            }
            if (解释类型 == 枚举_视觉观察解释类型::自由空间冲突) {
                结构_视觉自由空间冲突候选 冲突{};
                冲突.数据项ID = 材料.数据项ID;
                冲突.先验版本 = 输入.SceneVoxelPrior.版本;
                冲突.冲突数量 = 1;
                冲突.材料质量Q10000 = 材料.材料质量Q10000;
                冲突.当前空间范围 = 材料.当前空间范围;
                输出.自由空间冲突候选集合.push_back(std::move(冲突));
            }
        }

        输出.候选数量 = static_cast<I64>(std::min<std::size_t>(
            输出.候选集合.size(),
            static_cast<std::size_t>((std::numeric_limits<I64>::max)())));
        输出.自由空间冲突候选数量 = static_cast<I64>(std::min<std::size_t>(
            输出.自由空间冲突候选集合.size(),
            static_cast<std::size_t>((std::numeric_limits<I64>::max)())));
        输出.信息缺口数量 = static_cast<I64>(std::min<std::size_t>(
            输出.信息缺口集合.size(),
            static_cast<std::size_t>((std::numeric_limits<I64>::max)())));
        return 输出;
    }
};

} // namespace 视觉观察融合模块
