module;

// 文件头部规则注释模块：
// 1. 本模块是外设观察材料的运行期中间层，只保存内存队列和短期材料页，不把中间层材料写入数据库或世界树事实。
// 2. 扫描、观察、跟踪等上层入口只能消费本模块暴露的最新/最近同类型报告组，或已绑定显式等待项的短期可回查报告；不得跨帧回退旧材料伪造队列非空。
// 3. 过期报告和短期材料页必须在读写路径清理；容量裁剪不得抢先删除未过期的同外设同类型最新报告。

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <deque>
#include <limits>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "双目相机本能适配器.h"
#include "特征类.h"

export module 外设观察报告队列;

import 场景体素模块;

export enum class 枚举_外设观察运行模式 : std::uint8_t {
    未指定 = 0,
    陌生环境逐簇识别 = 1,
    已识别场景扫描变化 = 2,
    目标跟踪观察 = 3,
    识别 = 陌生环境逐簇识别,
    扫描 = 已识别场景扫描变化,
    跟踪 = 目标跟踪观察,
};

export enum class 枚举_外设分割处理模式 : std::uint8_t {
    未指定 = 0,
    全局观察 = 1,
    高价值待识别 = 2,
    扫描候选 = 3,
    目标跟踪 = 4,
    静止低频维护 = 5,
    低价值 = 6,
};

export enum class 枚举_外设观察报告类型 : std::uint8_t {
    未指定 = 0,
    逐簇识别报告 = 1,
    扫描变化报告 = 2,
    跟踪报告 = 3,
};

export enum class 枚举_扫描变化基准状态 : std::uint8_t {
    未定义 = 0,
    缺少逐簇识别基准 = 1,
    有可比历史 = 2,
};

export enum class 枚举_外设提交包类型 : std::uint8_t {
    未指定 = 0,
    识别 = 1,
    扫描 = 2,
    跟踪 = 3,
};

export enum class 枚举_外设触发来源 : std::uint8_t {
    未指定 = 0,
    默认维护 = 1,
    需求等待项 = 2,
    指定目标跟踪 = 3,
    诊断入口 = 4,
};

export enum class 枚举_外设特征值形态 : std::uint8_t {
    未指定 = 0,
    I64 = 1,
    F64 = 2,
    文本 = 3,
    句柄 = 4,
};

export enum class 枚举_目标特征当前值类型 : std::uint8_t {
    未指定 = 0,
    I64 = 1,
    VecU句柄 = 2,
    指针句柄 = 3,
    文本句柄 = 4,
};

export enum class 枚举_目标观察约束状态 : std::uint8_t {
    未定义 = 0,
    可用 = 1,
    已过期 = 2,
    已撤销 = 3,
    材料不可回查 = 4,
};

export enum class 枚举_目标观察约束可用性 : std::uint8_t {
    未定义 = 0,
    可用 = 1,
    可降级使用 = 2,
    已过期 = 3,
    目标已替代 = 5,
    材料不可回查 = 6,
    缺目标存在 = 7,
    缺目标特征 = 8,
    缺当前值 = 9,
    缺定位材料 = 10,
    已撤销 = 11,
};

export enum class 枚举_外设观察先验发布缺口 : std::uint8_t {
    无 = 0,
    缺场景体素先验快照 = 1,
    先验坐标系版本缺失 = 2,
    快照过期 = 3,
    目标观察约束过期 = 4,
    外设位姿缺失 = 5,
    投影材料不可生成 = 6,
    先验图层缺失 = 7,
    降级策略触发 = 8,
};

export enum class 枚举_观察材料等级 : std::uint8_t {
    未定义 = 0,
    不合格 = 1,
    可用 = 2,
    优质 = 3,
};

export enum class 枚举_外设材料能力等级 : std::uint8_t {
    未定义 = 0,
    候选或缺口 = 1,
    平面轮廓 = 2,
    深度锚定 = 3,
    空间可用 = 4,
};

export enum class 枚举_外设坐标对齐状态 : std::uint8_t {
    未确定 = 0,
    已对齐 = 1,
    未对齐 = 2,
    降级使用 = 3,
    未参与 = 4,
};

export enum class 枚举_外设坐标系一致性状态 : std::uint8_t {
    未确定 = 0,
    一致 = 1,
    降级 = 2,
    不一致 = 3,
};

export enum class 枚举_外设特征宿主类型 : std::uint8_t {
    未指定 = 0,
    区域 = 1,
    候选 = 2,
    簇 = 3,
    目标 = 4,
    已确认存在 = 5,
};

export enum class 枚举_外设区域归属状态 : std::uint8_t {
    未指定 = 0,
    已识别 = 1,
    未识别 = 2,
    候选 = 3,
    冲突 = 4,
    背景 = 5,
    无效 = 6,
    需重识别 = 7,
};

export enum class 枚举_扫描匹配状态 : std::uint8_t {
    未指定 = 0,
    匹配成功 = 1,
    匹配失败 = 2,
    遮挡 = 3,
    丢失 = 4,
    需重识别 = 5,
};

export enum class 枚举_跟踪状态 : std::uint8_t {
    未指定 = 0,
    稳定 = 1,
    遮挡 = 2,
    丢失 = 3,
    冲突 = 4,
    证据不足 = 5,
};

export enum class 枚举_外设Tracker轨迹状态 : std::uint8_t {
    未指定 = 0,
    新建 = 1,
    候选 = 2,
    稳定 = 3,
    短时丢失 = 4,
    已丢失 = 5,
    遮挡 = 6,
    冲突 = 7,
};

export enum class 枚举_外设提交缺口码 : std::int64_t {
    无 = 0,
    缺报告 = 1,
    缺原始观察帧 = 2,
    缺深度帧 = 3,
    缺彩色帧 = 4,
    缺对齐帧 = 5,
    缺空间候选 = 6,
    缺像素归属账本 = 7,
    缺材料句柄 = 8,
    材料过期 = 9,
    低质量 = 10,
    深度无效 = 11,
    空间坐标无效 = 12,
    缺可比历史 = 13,
    缺已确认存在ID = 14,
    缺目标种子 = 15,
    外设采集失败 = 16,
    报告类型不匹配 = 17,
};

export enum class 枚举_视觉清晰度等级状态 : std::int64_t {
    未评估 = 0,
    不可用 = 1,
    低清晰 = 2,
    可用 = 3,
    高清晰 = 4,
    局部可用 = 5,
    需复验 = 6,
    冲突 = 7,
};

export enum class 枚举_观察帧清晰度阻断原因 : std::int64_t {
    无 = 0,
    缺报告 = 1,
    采集失败 = 2,
    材料不可回查 = 4,
    帧质量不足 = 8,
    深度不足 = 16,
    空间不足 = 32,
    低质量区域 = 64,
    未解释区域偏高 = 128,
    坐标冲突 = 256,
    材料需复验 = 512,
};

export enum class 枚举_观察候选清晰度阻断原因 : std::int64_t {
    无 = 0,
    缺候选 = 1,
    缺像素 = 2,
    深度不足 = 4,
    空间不足 = 8,
    空间连续性不足 = 16,
    范围稳定性不足 = 32,
    轮廓置信度不足 = 64,
    缺稳定锚点 = 128,
    缺像素归属材料 = 256,
    缺局部图材料 = 512,
    候选不可提交 = 1024,
};

export enum class 枚举_像素观察归属状态 : std::int64_t {
    未评估 = 0,
    无效像素 = 1,
    未归属 = 2,
    承载面候选 = 3,
    普通观察候选 = 4,
    遮挡边界 = 5,
    争议区域 = 6,
    深度缺失 = 7,
    图像边界不可信区域 = 8,
    场景体素先验投影区域 = 9,
    场景体素先验遮挡区域 = 10,
    自由空间冲突区域 = 11,
    新占据候选区域 = 12,
    已知空闲命中区域 = 13,
    低质量区域 = 14,
    保留 = 15,
};

export enum class 枚举_外设特征类型ID : std::uint64_t {
    未指定 = 0,
    像素数量 = 1001,
    投影中心X = 1002,
    投影中心Y = 1003,
    空间中心Z = 1004,
    空间范围最小X = 1005,
    空间范围最大X = 1006,
    空间范围最小Y = 1007,
    空间范围最大Y = 1008,
    空间范围最小Z = 1009,
    空间范围最大Z = 1010,
    深度有效率 = 1011,
    空间坐标有效率 = 1012,
    空间连续性评分 = 1013,
    范围稳定性评分 = 1014,
    轮廓置信度 = 1015,
    跨帧匹配状态 = 1016,
    跨帧连续命中次数 = 1017,
    跨帧丢失次数 = 1018,
    新增观察像素簇数量 = 2001,
    消失观察像素簇数量 = 2002,
    变化观察像素簇数量 = 2003,
    高风险未知区域数量 = 2004,
    跟踪状态值 = 3001,
    丢失状态值 = 3002,
    最近观测年龄毫秒 = 3003,
    距离复现离散度 = 3004,
    空间坐标复现离散度 = 3005,
    连续成功次数 = 3006,
    失败次数 = 3007,
    重捕获候选数量 = 3008,
    稳定特征样本数量 = 3009,
    像素观察归属图句柄 = 4001,
    像素观察归属图版本 = 4002,
    像素观察归属图状态值域版本 = 4003,
    像素观察归属图生成方法编号 = 4004,
    像素观察归属图来源报告编号 = 4005,
    像素观察归属图来源帧编号 = 4006,
    像素观察归属图坐标系版本 = 4007,
    像素观察归属图使用先验版本 = 4008,
    像素总数 = 4009,
    有效像素数量 = 4010,
    未归属像素数量 = 4011,
    争议像素数量 = 4012,
    深度缺失像素数量 = 4013,
    遮挡边界像素数量 = 4014,
    自由空间冲突像素数量 = 4015,
    低质量像素数量 = 4016,
    像素归属质量评分Q10000 = 4017,
    候选有效深度点数量 = 4101,
    候选像素覆盖率Q10000 = 4102,
    候选争议像素数量 = 4103,
    候选遮挡边界像素数量 = 4104,
    候选深度缺失像素数量 = 4105,
    候选像素归属质量评分Q10000 = 4106,
    候选轮廓掩码句柄 = 4107,
    候选点集句柄 = 4108,
    候选彩色轮廓局部图句柄 = 4109,
    候选深度轮廓局部图句柄 = 4110,
    候选使用场景体素版本 = 4111,
    候选匹配先验短编号 = 4112,
    候选先验投影重叠率Q10000 = 4113,
    候选先验深度一致评分Q10000 = 4114,
    候选自由空间冲突数量 = 4115,
};

export struct 结构_外设观察像素簇摘要 {
    std::int64_t 外设内部簇ID = 0;
    std::int64_t 来源空间候选ID = 0;
    std::int64_t 外设跨帧簇ID = -1;
    std::int64_t 跨帧匹配状态 = 0;
    std::int64_t 跨帧连续命中次数 = 0;
    std::int64_t 跨帧丢失次数 = 0;
    std::int64_t 跨帧复现离散度 = 0;
    std::int64_t 跨帧连续稳定命中次数 = 0;
    std::int64_t 跨帧连续发布次数 = 0;
    std::int64_t 跨帧轨迹稳定状态 = 0;
    std::int64_t 跨帧轨迹待发布状态 = 0;
    std::int64_t 跨帧丢失宽限状态 = 0;
    枚举_外设Tracker轨迹状态 轨迹状态 = 枚举_外设Tracker轨迹状态::未指定;
    std::int64_t 轨迹状态连续帧数 = 0;
    std::int64_t 平滑中心X = 0;
    std::int64_t 平滑中心Y = 0;
    std::int64_t 平滑中心Z = 0;
    std::int64_t 平滑投影最小X = 0;
    std::int64_t 平滑投影最大X = 0;
    std::int64_t 平滑投影最小Y = 0;
    std::int64_t 平滑投影最大Y = 0;
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
    std::int64_t 稳定锚点数量 = 0;
    std::int64_t 稳定锚点密度 = 0;
    std::int64_t 稳定锚点覆盖率 = 0;
    std::int64_t 边界锚点覆盖率 = 0;
    bool 可提交观察区域 = false;
    std::int64_t 像素集合掩码状态 = 0;
    std::int64_t 彩色轮廓局部图状态 = 0;
    std::int64_t 深度轮廓局部图状态 = 0;
    std::int64_t 局部图宽度 = 0;
    std::int64_t 局部图高度 = 0;
    std::string 像素集合掩码句柄{};
    std::string 彩色轮廓局部图句柄{};
    std::string 深度轮廓局部图句柄{};
    std::string ROI句柄{};
    std::string 点集句柄{};
    std::string 空间候选句柄{};
    std::string 像素归属验证材料句柄{};
    std::string 失败原因{};
};

export struct 结构_外设空间中心 {
    std::int64_t X = 0;
    std::int64_t Y = 0;
    std::int64_t Z = 0;
};

export struct 结构_外设空间范围AABB {
    std::int64_t 最小X = 0;
    std::int64_t 最大X = 0;
    std::int64_t 最小Y = 0;
    std::int64_t 最大Y = 0;
    std::int64_t 最小Z = 0;
    std::int64_t 最大Z = 0;
};

export struct 结构_外设向量3D {
    std::int64_t X = 0;
    std::int64_t Y = 0;
    std::int64_t Z = 0;
};

export struct 结构_外设提交包头 {
    std::uint64_t 包ID = 0;
    std::uint64_t 报告ID = 0;
    std::uint64_t 帧ID = 0;
    std::uint64_t 场景ID = 0;
    枚举_外设提交包类型 包类型 = 枚举_外设提交包类型::未指定;
    枚举_外设触发来源 触发来源 = 枚举_外设触发来源::未指定;
    std::int64_t 采集时间毫秒 = 0;
    std::int64_t 提交时间毫秒 = 0;
    std::string 外设ID{};
    std::string 材料页ID{};
    std::string 坐标系ID{};
    std::int64_t 有效截止时间毫秒 = 0;
    std::vector<std::string> 材料句柄集{};
    std::vector<枚举_外设提交缺口码> 缺失材料{};
    std::vector<枚举_外设提交缺口码> 质量问题{};
};

export struct 结构_外设特征值条目 {
    枚举_外设特征类型ID 特征类型 = 枚举_外设特征类型ID::未指定;
    枚举_外设特征值形态 值形态 = 枚举_外设特征值形态::未指定;
    std::int64_t I64值 = 0;
    double F64值 = 0.0;
    std::uint64_t 宿主局部ID = 0;
    枚举_外设特征宿主类型 宿主类型 = 枚举_外设特征宿主类型::未指定;
    std::string 坐标系ID{};
    std::int64_t 时间戳毫秒 = 0;
    double 置信度 = 0.0;
    double 质量分 = 0.0;
    std::vector<std::string> 来源材料句柄集{};
    bool 可比较 = false;
    std::string 比较器ID{};
    std::vector<枚举_外设提交缺口码> 不可比较原因{};
};

export struct 结构_外设识别候选 {
    std::uint64_t 候选ID = 0;
    std::uint64_t 来源区域ID = 0;
    std::uint64_t 匹配存在ID = 0;
    枚举_外设区域归属状态 归属状态 = 枚举_外设区域归属状态::候选;
    double 匹配置信度 = 0.0;
    std::vector<std::string> 材料句柄集{};
    std::vector<结构_外设特征值条目> 候选特征值{};
    std::vector<枚举_外设提交缺口码> 证据不足原因{};
};

export struct 结构_外设识别提交包 {
    结构_外设提交包头 包头{};
    std::vector<std::uint64_t> 有效观察区域{};
    std::vector<std::uint64_t> 未识别区域{};
    std::vector<std::uint64_t> 冲突区域{};
    std::vector<std::uint64_t> 背景区域{};
    std::vector<std::uint64_t> 无效区域{};
    std::vector<结构_外设识别候选> 新存在候选{};
    std::vector<结构_外设识别候选> 已有存在匹配候选{};
    std::string 像素归属账本句柄{};
    std::vector<std::string> ROI句柄集{};
    std::vector<std::string> 掩码句柄集{};
    std::vector<std::string> 点集句柄集{};
    std::vector<std::string> 深度切片句柄集{};
    std::vector<结构_外设特征值条目> 候选特征值{};
    std::vector<枚举_外设提交缺口码> 证据不足原因{};
};

export struct 结构_外设已知存在扫描结果 {
    std::uint64_t 已确认存在ID = 0;
    std::string 当前ROI句柄{};
    std::string 当前掩码句柄{};
    std::string 当前点集句柄{};
    std::string 当前深度切片句柄{};
    结构_外设空间中心 当前中心{};
    结构_外设空间范围AABB 当前空间范围{};
    结构_外设空间中心 上一中心{};
    结构_外设空间范围AABB 上一空间范围{};
    结构_外设向量3D 位置变化{};
    double 距离变化 = 0.0;
    double 深度变化 = 0.0;
    枚举_扫描匹配状态 匹配状态 = 枚举_扫描匹配状态::未指定;
    double 匹配置信度 = 0.0;
    std::int64_t 候选像素数量 = 0;
    std::int64_t 候选有效深度点数量 = 0;
    std::int64_t 候选争议像素数量 = 0;
    std::int64_t 候选遮挡边界像素数量 = 0;
    std::int64_t 候选深度缺失像素数量 = 0;
    std::int64_t 候选低质量像素数量 = 0;
    std::int64_t 候选像素归属质量评分Q10000 = 0;
    std::vector<枚举_外设提交缺口码> 原因{};
};

export struct 结构_外设扫描提交包 {
    结构_外设提交包头 包头{};
    std::uint64_t 场景基准ID = 0;
    std::uint64_t 当前帧ID = 0;
    std::uint64_t 上一帧ID = 0;
    std::vector<std::uint64_t> 已识别区域{};
    std::vector<std::uint64_t> 未解释变化区域{};
    std::vector<std::uint64_t> 需重识别区域{};
    std::vector<std::uint64_t> 冲突区域{};
    std::vector<结构_外设已知存在扫描结果> 已知存在结果{};
    double 扫描覆盖率 = 0.0;
    double 像素归属闭合率 = 0.0;
    double 深度有效率 = 0.0;
    double 空间坐标有效率 = 0.0;
    std::vector<结构_外设特征值条目> 当前特征值{};
    std::vector<结构_外设特征值条目> 特征变化值{};
    std::vector<枚举_外设提交缺口码> 缺口原因{};
};

export struct 结构_外设跟踪提交包 {
    结构_外设提交包头 包头{};
    std::uint64_t 目标存在ID = 0;
    std::uint64_t 目标种子ID = 0;
    std::uint64_t 跟踪窗口ID = 0;
    std::uint64_t 当前帧ID = 0;
    std::uint64_t 上一帧ID = 0;
    std::string 当前ROI句柄{};
    std::string 当前掩码句柄{};
    std::string 当前点集句柄{};
    std::string 当前深度切片句柄{};
    结构_外设空间中心 当前中心{};
    结构_外设空间范围AABB 当前空间范围{};
    结构_外设空间中心 上一中心{};
    结构_外设空间范围AABB 上一空间范围{};
    结构_外设向量3D 目标位移{};
    double 目标距离变化 = 0.0;
    double 靠近远离趋势值 = 0.0;
    枚举_跟踪状态 跟踪状态 = 枚举_跟踪状态::未指定;
    double 跟踪置信度 = 0.0;
    std::vector<结构_外设特征值条目> 目标特征值{};
    std::vector<结构_外设特征值条目> 目标特征变化值{};
    std::vector<枚举_外设提交缺口码> 证据不足原因{};
};

export struct 结构_外设观察报告队列项 {
    std::uint64_t 报告ID = 0;
    std::uint64_t 对应命令ID = 0;
    std::uint64_t 匹配等待项ID = 0;
    std::string 来源外设ID{};
    枚举_外设观察运行模式 观察运行模式 = 枚举_外设观察运行模式::未指定;
    枚举_外设观察报告类型 报告类型 = 枚举_外设观察报告类型::未指定;
    std::int64_t 时间戳毫秒 = 0;
    bool 采集成功 = false;
    std::string 失败原因{};

    std::int64_t 帧宽度 = 0;
    std::int64_t 帧高度 = 0;
    std::int64_t 深度帧号 = 0;
    std::int64_t 彩色帧号 = 0;
    std::string 原始观察帧句柄{};
    std::string 深度帧句柄{};
    std::string 彩色帧句柄{};
    std::string 对齐帧句柄{};
    std::string 空间候选队列句柄{};
    std::string 像素归属账本句柄{};
    std::string 相机内参版本ID{};
    std::string 相机外参版本ID{};
    std::string 坐标系ID{};
    std::string 主分割坐标系ID{};
    std::string 深度材料坐标系ID{};
    std::string 左红外边缘坐标系ID{};
    std::string RGB裁剪材料坐标系ID{};
    std::string 像素掩码坐标系ID{};
    枚举_外设坐标对齐状态 深度材料坐标对齐状态 = 枚举_外设坐标对齐状态::未确定;
    枚举_外设坐标对齐状态 左红外边缘坐标对齐状态 = 枚举_外设坐标对齐状态::未确定;
    枚举_外设坐标对齐状态 RGB裁剪材料坐标对齐状态 = 枚举_外设坐标对齐状态::未确定;
    枚举_外设坐标对齐状态 像素掩码坐标对齐状态 = 枚举_外设坐标对齐状态::未确定;
    枚举_外设坐标系一致性状态 坐标系一致性状态 = 枚举_外设坐标系一致性状态::未确定;
    std::string 报告生成方法ID{};
    std::string 报告生成方法版本ID{};
    std::int64_t 报告置信度 = 0;
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
    枚举_扫描变化基准状态 扫描变化基准状态 = 枚举_扫描变化基准状态::未定义;
    std::int64_t 跨帧采集状态 = 0;
    std::int64_t 跨帧匹配观察像素簇数量 = 0;
    std::int64_t 跨帧新建观察像素簇数量 = 0;
    std::int64_t 跨帧稳定观察像素簇数量 = 0;
    std::int64_t 跨帧丢失观察像素簇数量 = 0;
    std::uint64_t 跟踪ID = 0;
    std::int64_t 跟踪状态值 = 0;
    std::int64_t 丢失状态值 = 0;
    std::int64_t 最近观测年龄毫秒 = 0;
    std::int64_t 距离复现离散度 = 0;
    std::int64_t 空间坐标复现离散度 = 0;
    std::int64_t 连续成功次数 = 0;
    std::int64_t 失败次数 = 0;
    std::int64_t 重捕获候选数量 = 0;
    std::int64_t 稳定特征样本数量 = 0;
    枚举_外设Tracker轨迹状态 轨迹状态 = 枚举_外设Tracker轨迹状态::未指定;
    std::int64_t 轨迹状态连续帧数 = 0;

    std::uint64_t 目标观察约束ID = 0;
    枚举_外设分割处理模式 本帧处理模式 = 枚举_外设分割处理模式::未指定;
    std::int64_t 本帧处理优先级 = 50;
    std::string 本帧处理掩码句柄{};
    std::int64_t 目标约束使用状态 = 0; // 0 未使用；1 强匹配；2 降级；3+ 为拒绝或缺口原因。
    std::int64_t 报告层级 = 0;         // L0/L1/L2，由外设侧按本次材料成本和完整度填写。
    std::int64_t 平面轮廓稳定状态 = 0;
    std::int64_t 深度锚定状态 = 0;
    std::int64_t 空间可用状态 = 0;
    std::int64_t 外设材料可承接状态 = 0;
    枚举_外设材料能力等级 可承接能力等级 = 枚举_外设材料能力等级::未定义;
    std::int64_t 稳定锚点数量 = 0;
    std::int64_t 稳定锚点密度 = 0;
    std::int64_t 稳定锚点覆盖率 = 0;
    std::int64_t 边界锚点覆盖率 = 0;
    std::uint64_t 来源原始报告ID = 0;  // L1 稳定子集报告回查其来源 L0 原始报告。
    std::uint64_t 来源外设帧ID = 0;    // L1 稳定子集报告回查其来源外设帧。
    std::int64_t 目标特征当前观察值 = 0;
    std::int64_t 目标特征误差值 = 0;
    bool 已消费 = false;
    std::uint64_t 消费等待项ID = 0;
    std::int64_t 消费时间毫秒 = 0;

    std::vector<结构_外设观察像素簇摘要> 观察像素簇集合{};
};

export struct 结构_外设观察等待项 {
    std::uint64_t 等待项ID = 0;
    std::string 来源需求ID{};
    std::string 目标外设ID{};
    枚举_外设观察运行模式 观察运行模式 = 枚举_外设观察运行模式::未指定;
    枚举_外设观察报告类型 期望报告类型 = 枚举_外设观察报告类型::未指定;
    std::int64_t 最大允许报告年龄毫秒 = 0;
    std::int64_t 最小稳定性要求 = 0;
    bool 是否允许缓存报告 = true;
    std::int64_t 目标观察簇ID = -1;
    std::string 幂等键{};

    std::uint64_t 目标观察约束ID = 0;
    std::string 目标存在ID{};
    std::string 目标特征类型ID{};
    std::int64_t 目标特征当前值 = 0;
    枚举_目标特征当前值类型 目标特征当前值类型 = 枚举_目标特征当前值类型::未指定;
    std::string 目标特征当前值句柄{};
    枚举_外设分割处理模式 本帧处理模式 = 枚举_外设分割处理模式::全局观察;
    std::int64_t 本帧处理优先级 = 50;
    std::string 本帧处理掩码句柄{};
    std::int64_t 目标约束有效截止时间毫秒 = 0;
    std::int64_t 允许降级匹配 = 1;
};

export struct 结构_目标观察约束特征组 {
    std::uint64_t 约束ID = 0;
    std::string 幂等键{};
    std::string 来源需求ID{};
    std::string 来源任务ID{};
    std::uint64_t 来源等待项ID = 0;

    std::string 目标存在ID{};
    std::string 目标特征类型ID{};
    std::int64_t 目标特征当前值 = 0;
    枚举_目标特征当前值类型 目标特征当前值类型 = 枚举_目标特征当前值类型::未指定;
    std::string 目标特征当前值句柄{};

    std::int64_t 比较符 = 0;
    std::int64_t 目标特征允许误差 = 0;
    std::int64_t 目标特征稳定阈值 = 0;

    std::string ROI句柄{};
    std::string 像素集合掩码句柄{};
    std::string 点集句柄{};

    std::int64_t 二维最小X = 0;
    std::int64_t 二维最大X = 0;
    std::int64_t 二维最小Y = 0;
    std::int64_t 二维最大Y = 0;

    std::int64_t 空间范围最小X = 0;
    std::int64_t 空间范围最大X = 0;
    std::int64_t 空间范围最小Y = 0;
    std::int64_t 空间范围最大Y = 0;
    std::int64_t 空间范围最小Z = 0;
    std::int64_t 空间范围最大Z = 0;

    std::uint64_t 来源报告ID = 0;
    std::uint64_t 来源帧ID = 0;

    std::int64_t 写入时间毫秒 = 0;
    std::int64_t TTL毫秒 = 0;
    std::int64_t 最大允许报告年龄毫秒 = 0;
    std::int64_t 最大允许观测年龄毫秒 = 0;
    std::int64_t 材料页TTL毫秒 = 0;

    std::int64_t 最低深度有效率 = 0;
    std::int64_t 最低空间坐标有效率 = 0;
    std::int64_t 最低掩码重合率 = 0;
    std::int64_t 最大预测误差 = 0;

    bool 允许降级 = true; // 兼容运行期协议位；机器判断读取应使用可用性判定状态值域。
    bool 允许缓存 = true; // 兼容运行期协议位；不得扩张为项目一阶事实。
    枚举_目标观察约束状态 状态 = 枚举_目标观察约束状态::未定义;
};

export struct 结构_目标观察约束可用性判定 {
    枚举_目标观察约束可用性 状态 = 枚举_目标观察约束可用性::未定义;
    std::int64_t 强匹配条件状态 = 0;
    std::int64_t 降级匹配条件状态 = 0;
    std::int64_t 先验可用性收束状态 = 0;
    bool 可强匹配 = false; // 兼容运行期协议位；新读取方不得作为机器判断依据。
    bool 可降级匹配 = false; // 兼容运行期协议位；新读取方不得作为机器判断依据。
};

export struct 结构_观察材料质量判定 {
    枚举_观察材料等级 等级 = 枚举_观察材料等级::未定义;
    bool 可进入验证方法 = false;
    bool 可进入识别方法 = false;
    bool 可生成扫描变化候选 = false;
    bool 可生成跟踪动态候选 = false;
    bool 可交付观察方法材料 = false;
    bool 平面轮廓稳定状态 = false;
    bool 深度锚定状态 = false;
    bool 空间可用状态 = false;
    bool 外设材料可承接状态 = false;
    枚举_外设材料能力等级 可承接能力等级 = 枚举_外设材料能力等级::未定义;
    枚举_外设坐标系一致性状态 坐标系一致性状态 = 枚举_外设坐标系一致性状态::未确定;
    std::int64_t 稳定锚点数量 = 0;
    std::int64_t 稳定锚点密度 = 0;
    std::int64_t 稳定锚点覆盖率 = 0;
    std::int64_t 边界锚点覆盖率 = 0;
    bool 目标区域优质 = false;
};

export struct 结构_观察帧清晰度评分 {
    std::uint64_t 来源报告ID = 0;
    std::uint64_t 来源帧ID = 0;
    std::int64_t 评分Q10000 = 0;
    std::int64_t 状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::未评估);
    std::int64_t 阻断原因掩码 = 0;
    std::int64_t 深度有效率Q10000 = 0;
    std::int64_t 空间坐标有效率Q10000 = 0;
    std::int64_t 帧质量评分Q10000 = 0;
    std::int64_t 未解释区域比例Q10000 = 0;
    std::int64_t 低质量区域数量 = 0;
};

export struct 结构_观察候选清晰度评分 {
    std::int64_t 外设内部簇ID = 0;
    std::int64_t 来源空间候选ID = 0;
    std::int64_t 评分Q10000 = 0;
    std::int64_t 状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::未评估);
    std::int64_t 阻断原因掩码 = 0;
    std::int64_t 像素数量 = 0;
    std::int64_t 深度有效率Q10000 = 0;
    std::int64_t 空间坐标有效率Q10000 = 0;
    std::int64_t 空间连续性评分Q10000 = 0;
    std::int64_t 范围稳定性评分Q10000 = 0;
    std::int64_t 轮廓置信度Q10000 = 0;
    std::int64_t 稳定锚点评分Q10000 = 0;
    std::int64_t 像素归属材料评分Q10000 = 0;
};

export struct 结构_外设观察队列状态 {
    std::size_t 队列容量 = 0;
    std::size_t 队列数量 = 0;
    std::size_t 等待项数量 = 0;
    std::uint64_t 最新报告ID = 0;
    std::size_t 目标观察约束数量 = 0;
    std::uint64_t 最新目标观察约束ID = 0;
};

export enum class 枚举_D455观察材料读取状态 : std::uint8_t {
    成功 = 0,
    空句柄 = 1,
    句柄格式错误 = 2,
    未找到 = 3,
    已过期 = 4,
    材料未就绪 = 5,
    材料类型不匹配 = 6,
    簇缺来源空间候选 = 7,
    簇投影非法 = 8,
    簇材料不可重建 = 9,
};

export struct 结构_D455观察材料句柄摘要 {
    枚举_D455观察材料读取状态 状态 = 枚举_D455观察材料读取状态::未找到;
    bool 成功 = false;
    std::uint64_t 报告ID = 0;
    std::string 资源类型ID{};
    std::int64_t 创建时间毫秒 = 0;
    std::int64_t 过期时间毫秒 = 0;
    std::int64_t 帧宽度 = 0;
    std::int64_t 帧高度 = 0;
    std::int64_t 深度帧号 = 0;
    std::int64_t 彩色帧号 = 0;
    std::int64_t 候选编号 = -1;
    std::int64_t 像素数量 = 0;
    std::int64_t 数据元素数量 = 0;
};

export struct 结构_D455控制面板视频快照 {
    bool 成功 = false;
    std::string 失败原因{};
    std::uint64_t 报告ID = 0;
    std::int64_t 帧宽度 = 0;
    std::int64_t 帧高度 = 0;
    std::int64_t 深度帧号 = 0;
    std::int64_t 彩色帧号 = 0;
    std::size_t 分割簇数量 = 0;
    std::int64_t 分割像素数量 = 0;
    std::int64_t 轮廓线像素数量 = 0;
    std::vector<std::uint8_t> 颜色RGB{};
    std::vector<std::uint8_t> 分割RGB{};
};

export struct 结构_D455三维体素轮廓视角读取结果 {
    bool 成功 = false;
    std::string 失败原因{};
    std::uint64_t 报告ID = 0;
    std::int64_t 候选编号 = -1;
    std::int64_t 有效像素数量 = 0;
    结构_三维体素轮廓图视角 视角{};
};

export struct 结构_像素观察归属图材料页读取结果 {
    枚举_D455观察材料读取状态 状态 = 枚举_D455观察材料读取状态::未找到;
    bool 成功 = false;
    std::string 失败原因{};
    std::uint64_t 报告ID = 0;
    std::string 资源类型ID{};
    std::int64_t 创建时间毫秒 = 0;
    std::int64_t 过期时间毫秒 = 0;
    std::int64_t 图宽 = 0;
    std::int64_t 图高 = 0;
    std::int64_t 来源报告编号 = 0;
    std::int64_t 来源帧编号 = 0;
    std::string 坐标系ID{};
    std::int64_t 坐标系版本 = 0;
    std::int64_t 状态值域版本 = 0;
    std::int64_t 像素总数 = 0;
    std::int64_t 归属像素数量 = 0;
    std::vector<std::int64_t> 每像素观察候选局部编号{};
    std::vector<std::int64_t> 每像素状态值{};
    std::vector<std::int64_t> 每像素可选先验短编号{};
    std::vector<std::int64_t> 每像素可选先验评分Q10000{};
};

export const char* 外设观察运行模式文本(枚举_外设观察运行模式 类型) noexcept;
export const char* 外设分割处理模式文本(枚举_外设分割处理模式 类型) noexcept;
export const char* 外设Tracker轨迹状态文本(枚举_外设Tracker轨迹状态 类型) noexcept;
export const char* 外设观察报告类型文本(枚举_外设观察报告类型 类型) noexcept;
export const char* 外设提交包类型文本(枚举_外设提交包类型 类型) noexcept;
export 枚举_外设提交包类型 外设提交包类型_由报告类型(枚举_外设观察报告类型 类型) noexcept;
export 结构_外设提交包头 构造外设提交包头_由观察报告(const 结构_外设观察报告队列项& 报告项);
export std::optional<结构_外设识别提交包> 构造外设识别提交包_由观察报告(
    const 结构_外设观察报告队列项& 报告项);
export std::optional<结构_外设扫描提交包> 构造外设扫描提交包_由观察报告(
    const 结构_外设观察报告队列项& 报告项);
export std::optional<结构_外设跟踪提交包> 构造外设跟踪提交包_由观察报告(
    const 结构_外设观察报告队列项& 报告项);
export std::int64_t 读取像素观察归属状态值域版本() noexcept;
export const char* 像素观察归属状态文本(枚举_像素观察归属状态 状态) noexcept;
export std::uint64_t 提交外设观察报告(结构_外设观察报告队列项 报告项);
export bool 提交D455短期观察材料页(
    std::uint64_t 报告ID,
    双目相机本能适配器::调用结果 来源,
    const 结构_外设观察报告队列项& 报告项);
export bool 提交D455控制面板视频帧(
    双目相机本能适配器::彩色视频帧 来源);
export 结构_D455观察材料句柄摘要 解析D455观察材料句柄(const std::string& 句柄);
export 结构_像素观察归属图材料页读取结果 读取像素观察归属图材料页(const std::string& 句柄);
export 结构_D455三维体素轮廓视角读取结果 读取D455簇三维体素轮廓视角(
    const 结构_外设观察像素簇摘要& 簇);
export 结构_D455控制面板视频快照 读取最新D455控制面板视频快照();
export std::uint64_t 提交外设观察等待项(结构_外设观察等待项 等待项);
export bool 完成外设观察等待项(std::uint64_t 等待项ID);
export const char* 观察材料等级文本(枚举_观察材料等级 类型) noexcept;
export const char* 外设材料能力等级文本(枚举_外设材料能力等级 类型) noexcept;
export const char* 外设坐标对齐状态文本(枚举_外设坐标对齐状态 类型) noexcept;
export const char* 外设坐标系一致性状态文本(枚举_外设坐标系一致性状态 类型) noexcept;
export 结构_观察材料质量判定 判断外设观察材料质量(
    const 结构_外设观察报告队列项& 报告项,
    const 结构_外设观察等待项* 等待项);
export std::int64_t 计算观察材料质量收束状态(
    const 结构_外设观察报告队列项& 报告项,
    const 结构_观察材料质量判定& 判定,
    const 结构_外设观察等待项* 等待项);
export 结构_观察帧清晰度评分 计算观察帧清晰度评分(
    const 结构_外设观察报告队列项& 报告项,
    const 结构_观察材料质量判定& 判定,
    const 结构_外设观察等待项* 等待项);
export 结构_观察帧清晰度评分 计算观察帧清晰度评分(
    const 结构_外设观察报告队列项& 报告项,
    const 结构_外设观察等待项* 等待项);
export 结构_观察候选清晰度评分 计算观察候选清晰度评分(
    const 结构_外设观察像素簇摘要& 簇);
export std::vector<枚举_外设提交缺口码> 收集观察材料质量缺口码(
    const 结构_外设观察报告队列项& 报告项,
    const 结构_观察材料质量判定& 判定,
    const 结构_外设观察等待项* 等待项);
export std::int64_t 外设提交缺口_结构化缺口大类(枚举_外设提交缺口码 缺口) noexcept;
export std::int64_t 外设提交缺口_结构化阻断阶段(枚举_外设提交缺口码 缺口) noexcept;
export std::int64_t 外设提交缺口_需求化处理建议(枚举_外设提交缺口码 缺口) noexcept;
export std::int64_t 外设观察先验发布缺口_结构化缺口大类(枚举_外设观察先验发布缺口 缺口) noexcept;
export std::int64_t 外设观察先验发布缺口_结构化阻断阶段(枚举_外设观察先验发布缺口 缺口) noexcept;
export std::int64_t 外设观察先验发布缺口_需求化处理建议(枚举_外设观察先验发布缺口 缺口) noexcept;
export 枚举_外设观察先验发布缺口 目标观察约束可用性_外设先验发布缺口(
    枚举_目标观察约束可用性 状态) noexcept;
export std::string 构造观察材料质量判定摘要(const 结构_观察材料质量判定& 判定);
export const char* 目标观察约束可用性文本(枚举_目标观察约束可用性 类型) noexcept;
export std::uint64_t 提交目标观察约束特征组(结构_目标观察约束特征组 约束);
export std::optional<结构_目标观察约束特征组> 读取目标观察约束_按等待项(
    const 结构_外设观察等待项& 等待项);
export std::optional<结构_目标观察约束特征组> 读取目标观察约束_按目标特征(
    const std::string& 目标存在ID,
    const std::string& 目标特征类型ID);
export std::optional<结构_目标观察约束特征组> 选择最新可用目标观察约束(
    const std::string& 目标存在ID,
    const std::string& 目标特征类型ID,
    std::int64_t 当前时间毫秒);
export std::vector<结构_目标观察约束特征组> 读取可用目标观察约束集合_按目标特征(
    const std::string& 目标特征类型ID,
    std::size_t 最大数量,
    std::int64_t 当前时间毫秒);
export 结构_目标观察约束可用性判定 判断目标观察约束可用性(
    const 结构_目标观察约束特征组& 约束,
    std::int64_t 当前时间毫秒,
    const 结构_外设观察等待项* 等待项);
export std::uint64_t 发布外设观察先验_由场景体素先验快照(
    const 场景体素模块::结构_SceneVoxelPrior& 先验快照,
    const std::string& 目标外设编号,
    const std::string& 目标存在ID,
    const std::string& 目标特征类型ID,
    std::int64_t 当前时间毫秒,
    std::int64_t 最大允许先验年龄毫秒);
export std::uint64_t 生成目标观察约束_由场景体素先验快照(
    const 场景体素模块::结构_SceneVoxelPrior& 先验快照,
    const std::string& 目标外设编号,
    const std::string& 目标存在ID,
    const std::string& 目标特征类型ID,
    std::int64_t 当前时间毫秒,
    std::int64_t 最大允许先验年龄毫秒);
export std::optional<结构_目标观察约束特征组> 读取最新外设处理先验_按目标(
    const std::string& 目标存在ID,
    const std::string& 目标特征类型ID,
    std::int64_t 当前时间毫秒);
export std::int64_t 计算外设处理先验优先级_由场景体素先验快照(
    const 场景体素模块::结构_SceneVoxelPrior& 先验快照,
    std::int64_t 当前时间毫秒,
    std::int64_t 最大允许先验年龄毫秒) noexcept;
export bool 撤销目标观察约束特征组(std::uint64_t 约束ID);
export std::size_t 清理目标观察约束特征组(std::int64_t 当前时间毫秒);
export std::string 构造目标观察约束摘要(const 结构_目标观察约束特征组& 约束);
export std::optional<结构_外设观察报告队列项> 读取最新外设观察报告(
    const std::string& 外设ID,
    枚举_外设观察报告类型 报告类型);
export std::optional<结构_外设观察报告队列项> 读取最新外设观察报告(
    const std::string& 外设ID,
    枚举_外设观察报告类型 报告类型,
    bool 包含已消费);
export std::optional<结构_外设观察报告队列项> 读取外设观察报告_按ID(
    std::uint64_t 报告ID);
export std::optional<结构_外设观察报告队列项> 读取外设观察报告_按ID(
    std::uint64_t 报告ID,
    bool 包含已消费);
export std::vector<结构_外设观察等待项> 读取外设观察等待项集合(
    const std::string& 外设ID,
    枚举_外设观察运行模式 观察运行模式);
export std::vector<结构_外设观察报告队列项> 读取外设观察报告队列视图(
    const std::string& 外设ID,
    枚举_外设观察报告类型 报告类型,
    bool 包含已消费);
export std::optional<结构_外设观察报告队列项> 按等待项匹配外设观察报告(
    const 结构_外设观察等待项& 等待项);
export bool 标记外设观察报告已消费(
    std::uint64_t 报告ID,
    std::uint64_t 等待项ID);
export std::size_t 清理外设观察报告队列(std::int64_t 当前时间毫秒);
export 结构_外设观察队列状态 读取外设观察队列状态();
export void 清空外设观察报告队列();
export std::string 构造外设观察报告摘要(const 结构_外设观察报告队列项& 报告项);
export std::string 构造D455材料包证据卡摘要(const 结构_外设观察报告队列项& 报告项);
export std::string 构造D455逐簇识别样本卡摘要(const 结构_外设观察报告队列项& 报告项);
export std::string 构造D455扫描变化样本卡摘要(const 结构_外设观察报告队列项& 报告项);
export std::string 构造外设观察等待项摘要(const 结构_外设观察等待项& 等待项);

namespace {
    constexpr std::int64_t 像素观察归属状态值域版本_第一版 = 1;
    constexpr const char* D455资源类型_像素观察归属图_第一版 = "像素观察归属图_第一版";

    constexpr std::int64_t 外设质量状态_未评估 = 0;
    constexpr std::int64_t 外设质量状态_可用 = 1;
    constexpr std::int64_t 外设质量状态_低质量 = 2;
    constexpr std::int64_t 外设质量状态_不可用 = 3;
    constexpr std::int64_t 外设质量状态_部分可用 = 4;
    constexpr std::int64_t 外设质量状态_需复验 = 5;

    constexpr std::int64_t 结构化缺口大类_未定义 = 0;
    constexpr std::int64_t 结构化缺口大类_外设材料缺口 = 1;
    constexpr std::int64_t 结构化缺口大类_观察域材料缺口 = 2;
    constexpr std::int64_t 结构化缺口大类_质量收束缺口 = 3;
    constexpr std::int64_t 结构化缺口大类_坐标基准缺口 = 4;
    constexpr std::int64_t 结构化缺口大类_像素归属缺口 = 5;
    constexpr std::int64_t 结构化缺口大类_场景体素先验缺口 = 7;
    constexpr std::int64_t 结构化缺口大类_识别归属缺口 = 10;

    constexpr std::int64_t 结构化阻断阶段_未定义 = 0;
    constexpr std::int64_t 结构化阻断阶段_材料取得 = 1;
    constexpr std::int64_t 结构化阻断阶段_材料质量收束 = 2;
    constexpr std::int64_t 结构化阻断阶段_坐标基准收束 = 3;
    constexpr std::int64_t 结构化阻断阶段_像素归属 = 4;
    constexpr std::int64_t 结构化阻断阶段_先验融合 = 7;
    constexpr std::int64_t 结构化阻断阶段_观察提交 = 8;
    constexpr std::int64_t 结构化阻断阶段_识别归属 = 9;
    constexpr std::int64_t 结构化阻断阶段_扫描比较 = 11;

    constexpr std::int64_t 需求化建议_未评估 = 0;
    constexpr std::int64_t 需求化建议_只记录过程证据 = 1;
    constexpr std::int64_t 需求化建议_转外设材料闭环 = 6;
    constexpr std::int64_t 需求化建议_转观察验证闭环 = 7;
    constexpr std::int64_t 需求化建议_等待复验 = 11;

    constexpr std::int64_t 目标约束条件状态_未满足 = 0;
    constexpr std::int64_t 目标约束条件状态_满足 = 1;
    constexpr std::int64_t 先验可用性收束状态_未定义 = 0;
    constexpr std::int64_t 先验可用性收束状态_强匹配 = 1;
    constexpr std::int64_t 先验可用性收束状态_降级匹配 = 2;
    constexpr std::int64_t 先验可用性收束状态_不可用 = 3;

    struct 结构_D455短期观察材料页 {
        std::uint64_t 报告ID = 0;
        std::int64_t 创建时间毫秒 = 0;
        std::int64_t 过期时间毫秒 = 0;
        双目相机本能适配器::调用结果 来源{};
        结构_外设观察报告队列项 报告{};
    };

    struct 结构_D455控制面板视频帧页 {
        std::uint64_t 视频帧ID = 0;
        std::int64_t 创建时间毫秒 = 0;
        std::int64_t 过期时间毫秒 = 0;
        双目相机本能适配器::彩色视频帧 来源{};
    };

    struct 结构_外设观察队列共享状态 {
        std::mutex 互斥{};
        std::deque<结构_外设观察报告队列项> 报告队列{};
        std::deque<结构_D455短期观察材料页> D455材料页队列{};
        std::deque<结构_D455控制面板视频帧页> D455控制面板视频帧队列{};
        std::vector<结构_外设观察等待项> 等待项集合{};
        std::deque<结构_目标观察约束特征组> 目标观察约束缓冲区{};
        std::uint64_t 最新报告ID = 0;
        std::uint64_t 最新D455控制面板视频帧ID = 0;
        std::uint64_t 最新目标观察约束ID = 0;
        std::size_t 队列容量 = 256;
        std::size_t D455材料页容量 = 6;
        std::size_t D455控制面板视频帧容量 = 12;
        std::size_t 目标观察约束缓冲区容量 = 128;
        std::int64_t D455材料页保留毫秒 = 60000;
        std::int64_t D455控制面板视频帧保留毫秒 = 3000;
    };

    // 功能：按函数名执行对应处理。
    结构_外设观察队列共享状态& 外设观察队列状态() noexcept
    {
        static 结构_外设观察队列共享状态 状态;
        return 状态;
    }

    // 功能：按函数名执行对应处理。
    std::atomic<std::uint64_t>& 外设观察报告ID计数器() noexcept
    {
        static std::atomic<std::uint64_t> 计数器{0};
        return 计数器;
    }

    // 功能：等待线程、任务、外设或条件变化。
    std::atomic<std::uint64_t>& 外设观察等待项ID计数器() noexcept
    {
        static std::atomic<std::uint64_t> 计数器{0};
        return 计数器;
    }

    // 功能：按函数名执行对应处理。
    std::atomic<std::uint64_t>& 目标观察约束ID计数器() noexcept
    {
        static std::atomic<std::uint64_t> 计数器{0};
        return 计数器;
    }

    // 功能：按函数名执行对应处理。
    std::int64_t 外设观察当前时间毫秒() noexcept
    {
        const auto 现在 = std::chrono::steady_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(现在).count();
    }

    constexpr std::int64_t 外设提交包默认有效毫秒 = 60000;
    constexpr std::int64_t 目标观察约束默认TTL毫秒 = 60000;

    // 功能：提交事实、动态、任务状态或运行回执。
    std::uint64_t 外设提交_正局部ID(
        std::int64_t 候选ID,
        std::uint64_t 回退ID) noexcept
    {
        return 候选ID > 0
            ? static_cast<std::uint64_t>(候选ID)
            : 回退ID;
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    double 外设提交_百万分值(std::int64_t 值) noexcept
    {
        return 值 <= 0 ? 0.0 : static_cast<double>(值) / 1000000.0;
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    void 外设提交_追加缺口(
        std::vector<枚举_外设提交缺口码>& 集合,
        枚举_外设提交缺口码 缺口)
    {
        if (缺口 == 枚举_外设提交缺口码::无) {
            return;
        }
        if (std::find(集合.begin(), 集合.end(), 缺口) == 集合.end()) {
            集合.push_back(缺口);
        }
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    void 外设提交_追加非空句柄(
        std::vector<std::string>& 集合,
        const std::string& 句柄)
    {
        if (句柄.empty()) {
            return;
        }
        if (std::find(集合.begin(), 集合.end(), 句柄) == 集合.end()) {
            集合.push_back(句柄);
        }
    }

    // 功能：按材料页解析结果判断句柄是否可回查。
    bool 外设提交_材料句柄可回查(const std::string& 句柄)
    {
        return !句柄.empty() && 解析D455观察材料句柄(句柄).成功;
    }

    // 功能：只把可回查材料句柄追加到提交材料集合。
    void 外设提交_追加可回查句柄(
        std::vector<std::string>& 集合,
        const std::string& 句柄)
    {
        if (!外设提交_材料句柄可回查(句柄)) {
            return;
        }
        外设提交_追加非空句柄(集合, 句柄);
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    std::uint64_t 外设提交_帧ID(const 结构_外设观察报告队列项& 报告项) noexcept
    {
        if (报告项.来源外设帧ID > 0) {
            return 报告项.来源外设帧ID;
        }
        if (报告项.深度帧号 > 0) {
            return static_cast<std::uint64_t>(报告项.深度帧号);
        }
        if (报告项.彩色帧号 > 0) {
            return static_cast<std::uint64_t>(报告项.彩色帧号);
        }
        return 报告项.报告ID;
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    std::string 外设提交_材料页ID(const 结构_外设观察报告队列项& 报告项)
    {
        const auto 材料来源报告ID = 报告项.来源原始报告ID != 0
            ? 报告项.来源原始报告ID
            : 报告项.报告ID;
        return 材料来源报告ID == 0
            ? std::string{}
            : "D455材料页#报告" + std::to_string(材料来源报告ID);
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    std::vector<std::string> 外设提交_簇材料句柄集(const 结构_外设观察像素簇摘要& 簇)
    {
        std::vector<std::string> 输出{};
        外设提交_追加非空句柄(输出, 簇.ROI句柄);
        外设提交_追加非空句柄(输出, 簇.点集句柄);
        外设提交_追加非空句柄(输出, 簇.空间候选句柄);
        外设提交_追加非空句柄(输出, 簇.像素归属验证材料句柄);
        外设提交_追加非空句柄(输出, 簇.像素集合掩码句柄);
        外设提交_追加非空句柄(输出, 簇.彩色轮廓局部图句柄);
        外设提交_追加非空句柄(输出, 簇.深度轮廓局部图句柄);
        return 输出;
    }

    // 功能：收集簇内当前仍可回查的材料句柄。
    std::vector<std::string> 外设提交_可回查簇材料句柄集(const 结构_外设观察像素簇摘要& 簇)
    {
        std::vector<std::string> 输出{};
        外设提交_追加可回查句柄(输出, 簇.ROI句柄);
        外设提交_追加可回查句柄(输出, 簇.点集句柄);
        外设提交_追加可回查句柄(输出, 簇.空间候选句柄);
        外设提交_追加可回查句柄(输出, 簇.像素归属验证材料句柄);
        外设提交_追加可回查句柄(输出, 簇.像素集合掩码句柄);
        外设提交_追加可回查句柄(输出, 簇.彩色轮廓局部图句柄);
        外设提交_追加可回查句柄(输出, 簇.深度轮廓局部图句柄);
        return 输出;
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    void 外设提交_汇总报告材料(
        结构_外设提交包头& 包头,
        const 结构_外设观察报告队列项& 报告项)
    {
        外设提交_追加可回查句柄(包头.材料句柄集, 报告项.原始观察帧句柄);
        外设提交_追加可回查句柄(包头.材料句柄集, 报告项.深度帧句柄);
        外设提交_追加可回查句柄(包头.材料句柄集, 报告项.彩色帧句柄);
        外设提交_追加可回查句柄(包头.材料句柄集, 报告项.对齐帧句柄);
        外设提交_追加可回查句柄(包头.材料句柄集, 报告项.空间候选队列句柄);
        外设提交_追加可回查句柄(包头.材料句柄集, 报告项.像素归属账本句柄);
        for (const auto& 簇 : 报告项.观察像素簇集合) {
            for (const auto& 句柄 : 外设提交_可回查簇材料句柄集(簇)) {
                外设提交_追加非空句柄(包头.材料句柄集, 句柄);
            }
        }
    }

    // 功能：按缺口类别写入提交包头的材料缺失或质量问题集合。
    void 外设提交_追加包头质量缺口(
        结构_外设提交包头& 包头,
        枚举_外设提交缺口码 缺口)
    {
        switch (缺口) {
        case 枚举_外设提交缺口码::外设采集失败:
        case 枚举_外设提交缺口码::材料过期:
        case 枚举_外设提交缺口码::低质量:
        case 枚举_外设提交缺口码::深度无效:
        case 枚举_外设提交缺口码::空间坐标无效:
        case 枚举_外设提交缺口码::报告类型不匹配:
            外设提交_追加缺口(包头.质量问题, 缺口);
            break;
        default:
            外设提交_追加缺口(包头.缺失材料, 缺口);
            break;
        }
    }

    // 功能：把质量缺口集合合并到提交包体缺口向量。
    void 外设提交_合并缺口集合(
        std::vector<枚举_外设提交缺口码>& 目标,
        const std::vector<枚举_外设提交缺口码>& 来源)
    {
        for (const auto 缺口 : 来源) {
            外设提交_追加缺口(目标, 缺口);
        }
    }

    // 功能：比较外设材料能力等级是否达到指定最低层级。
    bool 外设提交_能力等级达到(
        枚举_外设材料能力等级 当前,
        枚举_外设材料能力等级 最低) noexcept
    {
        return static_cast<std::uint8_t>(当前) >= static_cast<std::uint8_t>(最低);
    }

    // 功能：按六态质量状态和能力等级判断提交包是否可继续承接材料。
    bool 外设提交_质量状态允许承接(
        std::int64_t 质量状态,
        枚举_外设材料能力等级 能力等级) noexcept
    {
        return (质量状态 == 外设质量状态_可用 || 质量状态 == 外设质量状态_部分可用)
            && 外设提交_能力等级达到(能力等级, 枚举_外设材料能力等级::平面轮廓);
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    void 外设提交_填充报告缺口(
        结构_外设提交包头& 包头,
        const 结构_外设观察报告队列项& 报告项)
    {
        if (!报告项.采集成功) {
            外设提交_追加缺口(包头.质量问题, 枚举_外设提交缺口码::外设采集失败);
        }
        if (报告项.原始观察帧句柄.empty()) {
            外设提交_追加缺口(包头.缺失材料, 枚举_外设提交缺口码::缺原始观察帧);
        }
        if (报告项.深度帧句柄.empty()) {
            外设提交_追加缺口(包头.缺失材料, 枚举_外设提交缺口码::缺深度帧);
        }
        if (报告项.彩色帧句柄.empty()) {
            外设提交_追加缺口(包头.缺失材料, 枚举_外设提交缺口码::缺彩色帧);
        }
        if (报告项.对齐帧句柄.empty()) {
            外设提交_追加缺口(包头.缺失材料, 枚举_外设提交缺口码::缺对齐帧);
        }
        if (报告项.空间候选队列句柄.empty()) {
            外设提交_追加缺口(包头.缺失材料, 枚举_外设提交缺口码::缺空间候选);
        }
        if (报告项.像素归属账本句柄.empty()) {
            外设提交_追加缺口(包头.缺失材料, 枚举_外设提交缺口码::缺像素归属账本);
        }
        if (报告项.帧质量评分 <= 0 || 报告项.低质量区域数量 > 0) {
            外设提交_追加缺口(包头.质量问题, 枚举_外设提交缺口码::低质量);
        }
        if (报告项.深度有效率 <= 0) {
            外设提交_追加缺口(包头.质量问题, 枚举_外设提交缺口码::深度无效);
        }
        if (报告项.空间坐标有效率 <= 0) {
            外设提交_追加缺口(包头.质量问题, 枚举_外设提交缺口码::空间坐标无效);
        }
        const auto 判定 = 判断外设观察材料质量(报告项, nullptr);
        for (const auto 缺口 : 收集观察材料质量缺口码(报告项, 判定, nullptr)) {
            外设提交_追加包头质量缺口(包头, 缺口);
        }
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    结构_外设空间中心 外设提交_簇中心(const 结构_外设观察像素簇摘要& 簇) noexcept
    {
        return 结构_外设空间中心{ 簇.中心X, 簇.中心Y, 簇.中心Z };
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    结构_外设空间范围AABB 外设提交_簇范围(const 结构_外设观察像素簇摘要& 簇) noexcept
    {
        return 结构_外设空间范围AABB{
            簇.空间范围最小X,
            簇.空间范围最大X,
            簇.空间范围最小Y,
            簇.空间范围最大Y,
            簇.空间范围最小Z,
            簇.空间范围最大Z,
        };
    }

    结构_外设特征值条目 外设提交_构造I64特征(
        枚举_外设特征类型ID 特征类型,
        std::int64_t 值,
        std::uint64_t 宿主局部ID,
        枚举_外设特征宿主类型 宿主类型,
        const std::string& 坐标系ID,
        std::int64_t 时间戳毫秒,
        double 置信度,
        double 质量分,
        const std::vector<std::string>& 来源材料句柄集,
        bool 可比较,
        const std::string& 比较器ID = {})
    {
        结构_外设特征值条目 条目{};
        条目.特征类型 = 特征类型;
        条目.值形态 = 枚举_外设特征值形态::I64;
        条目.I64值 = 值;
        条目.宿主局部ID = 宿主局部ID;
        条目.宿主类型 = 宿主类型;
        条目.坐标系ID = 坐标系ID;
        条目.时间戳毫秒 = 时间戳毫秒;
        条目.置信度 = 置信度;
        条目.质量分 = 质量分;
        条目.来源材料句柄集 = 来源材料句柄集;
        条目.可比较 = 可比较;
        条目.比较器ID = 比较器ID;
        if (!可比较) {
            条目.不可比较原因.push_back(枚举_外设提交缺口码::缺可比历史);
        }
        return 条目;
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    void 外设提交_追加簇基础特征(
        std::vector<结构_外设特征值条目>& 输出,
        const 结构_外设观察报告队列项& 报告项,
        const 结构_外设观察像素簇摘要& 簇,
        std::uint64_t 宿主局部ID,
        枚举_外设特征宿主类型 宿主类型,
        bool 可比较)
    {
        const auto 材料句柄集 = 外设提交_簇材料句柄集(簇);
        const auto 置信度 = 外设提交_百万分值(报告项.报告置信度);
        const auto 质量分 = 外设提交_百万分值(报告项.帧质量评分);
        const auto& 坐标系ID = 报告项.坐标系ID;
        const auto 时间戳 = 报告项.时间戳毫秒;
        const std::string 比较器ID = 可比较 ? "外设同源跨帧I64比较器" : std::string{};
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::像素数量, 簇.像素数量, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::投影中心X, 簇.中心X, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::投影中心Y, 簇.中心Y, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间中心Z, 簇.中心Z, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最小X, 簇.空间范围最小X, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最大X, 簇.空间范围最大X, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最小Y, 簇.空间范围最小Y, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最大Y, 簇.空间范围最大Y, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最小Z, 簇.空间范围最小Z, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最大Z, 簇.空间范围最大Z, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::深度有效率, 簇.深度有效率, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间坐标有效率, 簇.空间坐标有效率, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间连续性评分, 簇.空间连续性评分, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::范围稳定性评分, 簇.范围稳定性评分, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::轮廓置信度, 簇.轮廓置信度, 宿主局部ID, 宿主类型,
            坐标系ID, 时间戳, 置信度, 质量分, 材料句柄集, 可比较, 比较器ID));
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    枚举_扫描匹配状态 外设提交_扫描匹配状态_由簇(
        const 结构_外设观察像素簇摘要& 簇) noexcept
    {
        if (簇.跨帧丢失次数 > 0 && 簇.跨帧连续命中次数 <= 0) {
            return 枚举_扫描匹配状态::丢失;
        }
        if (簇.跨帧匹配状态 > 0 || 簇.外设跨帧簇ID >= 0) {
            return 枚举_扫描匹配状态::匹配成功;
        }
        return 枚举_扫描匹配状态::需重识别;
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    枚举_跟踪状态 外设提交_跟踪状态_由报告(
        const 结构_外设观察报告队列项& 报告项) noexcept
    {
        switch (报告项.轨迹状态) {
        case 枚举_外设Tracker轨迹状态::稳定:
            return 枚举_跟踪状态::稳定;
        case 枚举_外设Tracker轨迹状态::短时丢失:
        case 枚举_外设Tracker轨迹状态::已丢失:
            return 枚举_跟踪状态::丢失;
        case 枚举_外设Tracker轨迹状态::遮挡:
            return 枚举_跟踪状态::遮挡;
        case 枚举_外设Tracker轨迹状态::冲突:
            return 枚举_跟踪状态::冲突;
        case 枚举_外设Tracker轨迹状态::新建:
        case 枚举_外设Tracker轨迹状态::候选:
            return 枚举_跟踪状态::证据不足;
        case 枚举_外设Tracker轨迹状态::未指定:
        default:
            break;
        }
        if (报告项.丢失状态值 > 0) {
            return 枚举_跟踪状态::丢失;
        }
        if (!报告项.采集成功 || 报告项.稳定特征样本数量 <= 0) {
            return 枚举_跟踪状态::证据不足;
        }
        if (报告项.失败次数 > 0 && 报告项.连续成功次数 <= 0) {
            return 枚举_跟踪状态::证据不足;
        }
        return 枚举_跟踪状态::稳定;
    }

    // 功能：清理缓存、临时状态或运行资源。
    void D455_清理短期材料页_已加锁(
        结构_外设观察队列共享状态& 状态,
        std::int64_t 当前时间毫秒)
    {
        while (!状态.D455材料页队列.empty()) {
            const auto& 页 = 状态.D455材料页队列.front();
            const bool 已过期 = 页.过期时间毫秒 > 0 && 当前时间毫秒 > 页.过期时间毫秒;
            const bool 超容量 = 状态.D455材料页队列.size() > 状态.D455材料页容量;
            if (!已过期 && !超容量) {
                break;
            }
            状态.D455材料页队列.pop_front();
        }
    }

    // 功能：清理缓存、临时状态或运行资源。
    void D455_清理控制面板视频帧_已加锁(
        结构_外设观察队列共享状态& 状态,
        std::int64_t 当前时间毫秒)
    {
        while (!状态.D455控制面板视频帧队列.empty()) {
            const auto& 页 = 状态.D455控制面板视频帧队列.front();
            const bool 已过期 = 页.过期时间毫秒 > 0 && 当前时间毫秒 > 页.过期时间毫秒;
            const bool 超容量 = 状态.D455控制面板视频帧队列.size() > 状态.D455控制面板视频帧容量;
            if (!已过期 && !超容量) {
                break;
            }
            状态.D455控制面板视频帧队列.pop_front();
        }
    }

    // 功能：清理缓存、临时状态或运行资源。
    bool 外设观察报告_应清理(
        const 结构_外设观察报告队列项& 报告项,
        std::int64_t 当前时间毫秒) noexcept
    {
        return 报告项.时间戳毫秒 > 0
            && 当前时间毫秒 - 报告项.时间戳毫秒 > 外设提交包默认有效毫秒;
    }

    // 功能：按函数名执行对应处理。
    bool 外设观察报告_容量裁剪保护(
        const 结构_外设观察报告队列项& 报告项,
        std::int64_t 当前时间毫秒) noexcept
    {
        if (外设观察报告_应清理(报告项, 当前时间毫秒)) {
            return false;
        }
        if (报告项.匹配等待项ID != 0 || 报告项.消费等待项ID != 0) {
            return true;
        }
        return 报告项.报告类型 == 枚举_外设观察报告类型::逐簇识别报告
            && 报告项.报告层级 == 1;
    }

    // 功能：按函数名执行对应处理。
    std::size_t 外设观察报告_裁剪容量已加锁(
        结构_外设观察队列共享状态& 状态,
        std::int64_t 当前时间毫秒)
    {
        std::size_t 裁剪数量 = 0;
        auto 是未过期同类型最新报告 = [&](const 结构_外设观察报告队列项& 报告项) noexcept {
            if (报告项.报告类型 == 枚举_外设观察报告类型::未指定
                || 外设观察报告_应清理(报告项, 当前时间毫秒)) {
                return false;
            }
            for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
                if (外设观察报告_应清理(*迭代器, 当前时间毫秒)) {
                    continue;
                }
                if (迭代器->报告类型 != 报告项.报告类型) {
                    continue;
                }
                if (迭代器->来源外设ID != 报告项.来源外设ID) {
                    continue;
                }
                return &(*迭代器) == &报告项;
            }
            return false;
        };
        while (状态.报告队列.size() > 状态.队列容量) {
            auto 可裁剪项 = std::find_if(
                状态.报告队列.begin(),
                状态.报告队列.end(),
                [&](const 结构_外设观察报告队列项& 报告项) noexcept {
                    return !外设观察报告_容量裁剪保护(报告项, 当前时间毫秒)
                        && !是未过期同类型最新报告(报告项);
                });
            if (可裁剪项 == 状态.报告队列.end()) {
                auto 普通L1缓存项 = std::find_if(
                    状态.报告队列.begin(),
                    状态.报告队列.end(),
                    [&](const 结构_外设观察报告队列项& 报告项) noexcept {
                        return !外设观察报告_应清理(报告项, 当前时间毫秒)
                            && !是未过期同类型最新报告(报告项)
                            && 报告项.匹配等待项ID == 0
                            && 报告项.消费等待项ID == 0
                            && 报告项.报告类型 == 枚举_外设观察报告类型::逐簇识别报告
                            && 报告项.报告层级 == 1;
                    });
                if (普通L1缓存项 != 状态.报告队列.end()) {
                    状态.报告队列.erase(普通L1缓存项);
                } else {
                    break;
                }
            } else {
                状态.报告队列.erase(可裁剪项);
            }
            ++裁剪数量;
        }
        return 裁剪数量;
    }

    // 功能：清理缓存、临时状态或运行资源。
    std::size_t 外设观察报告_清理已加锁(
        结构_外设观察队列共享状态& 状态,
        std::int64_t 当前时间毫秒)
    {
        const auto 原数量 = 状态.报告队列.size();
        状态.报告队列.erase(
            std::remove_if(
                状态.报告队列.begin(),
                状态.报告队列.end(),
                [当前时间毫秒](const 结构_外设观察报告队列项& 报告项) noexcept {
                    return 外设观察报告_应清理(报告项, 当前时间毫秒);
                }),
            状态.报告队列.end());
        (void)外设观察报告_裁剪容量已加锁(状态, 当前时间毫秒);
        return 原数量 > 状态.报告队列.size()
            ? 原数量 - 状态.报告队列.size()
            : 0;
    }

    // 功能：按函数名执行对应处理。
    bool D455_句柄前缀有效(const std::string& 句柄) noexcept
    {
        return 句柄.rfind("d455://", 0) == 0;
    }

    // 功能：按函数名执行对应处理。
    std::string D455_句柄资源类型(const std::string& 句柄)
    {
        constexpr std::size_t 前缀长度 = 7;
        if (句柄.size() <= 前缀长度 || !D455_句柄前缀有效(句柄)) {
            return {};
        }
        const auto 斜杠 = 句柄.find('/', 前缀长度);
        if (斜杠 == std::string::npos) {
            return 句柄.substr(前缀长度);
        }
        return 句柄.substr(前缀长度, 斜杠 - 前缀长度);
    }

    // 功能：按函数名执行对应处理。
    bool D455_帧像素数量有效(const 双目相机本能适配器::调用结果& 来源) noexcept
    {
        return 来源.成功
            && 来源.宽度 > 0
            && 来源.高度 > 0
            && 来源.预期像素数量 == static_cast<std::int64_t>(来源.宽度) * static_cast<std::int64_t>(来源.高度)
            && 来源.预期像素数量 > 0;
    }

    // 功能：按函数名执行对应处理。
    bool D455_颜色帧材料有效(const 双目相机本能适配器::调用结果& 来源) noexcept
    {
        return D455_帧像素数量有效(来源)
            && 来源.颜色RGB.size() == static_cast<std::size_t>(来源.预期像素数量);
    }

    // 功能：按函数名执行对应处理。
    bool D455_控制面板视频帧有效(const 双目相机本能适配器::彩色视频帧& 来源) noexcept
    {
        return 来源.成功
            && 来源.宽度 > 0
            && 来源.高度 > 0
            && 来源.预期像素数量 == static_cast<std::int64_t>(来源.宽度) * static_cast<std::int64_t>(来源.高度)
            && 来源.预期像素数量 > 0
            && 来源.颜色RGB.size() == static_cast<std::size_t>(来源.预期像素数量) * 3;
    }

    // 功能：按函数名执行对应处理。
    bool D455_深度帧材料有效(const 双目相机本能适配器::调用结果& 来源) noexcept
    {
        return D455_帧像素数量有效(来源)
            && (来源.原始深度毫米.size() == static_cast<std::size_t>(来源.预期像素数量)
                || 来源.滤波深度毫米.size() == static_cast<std::size_t>(来源.预期像素数量)
                || 来源.补全深度毫米.size() == static_cast<std::size_t>(来源.预期像素数量)
                || 来源.融合深度毫米.size() == static_cast<std::size_t>(来源.预期像素数量));
    }

    // 功能：按函数名执行对应处理。
    bool D455_空间坐标材料有效(const 双目相机本能适配器::调用结果& 来源) noexcept
    {
        return D455_帧像素数量有效(来源)
            && 来源.空间坐标毫米XYZ.size() == static_cast<std::size_t>(来源.预期像素数量);
    }

    // 功能：按条件查找目标对象、方法或事实。
    const 双目相机本能适配器::空间候选摘要* D455_查找材料页空间候选(
        const 结构_D455短期观察材料页& 页,
        std::int64_t 候选编号) noexcept
    {
        for (const auto& 候选 : 页.来源.空间候选列表) {
            if (候选.候选编号 == 候选编号) {
                return &候选;
            }
        }
        return nullptr;
    }

    // 功能：按函数名执行对应处理。
    void D455_填充材料页通用摘要(
        const 结构_D455短期观察材料页& 页,
        const std::string& 资源类型,
        结构_D455观察材料句柄摘要* 摘要) noexcept
    {
        if (!摘要) {
            return;
        }
        摘要->报告ID = 页.报告ID;
        摘要->资源类型ID = 资源类型;
        摘要->创建时间毫秒 = 页.创建时间毫秒;
        摘要->过期时间毫秒 = 页.过期时间毫秒;
        摘要->帧宽度 = 页.来源.宽度;
        摘要->帧高度 = 页.来源.高度;
        摘要->深度帧号 = 页.来源.深度帧号;
        摘要->彩色帧号 = 页.来源.彩色帧号;
    }

    // 功能：按函数名执行对应处理。
    void D455_标记材料摘要成功(结构_D455观察材料句柄摘要* 摘要) noexcept
    {
        if (!摘要) {
            return;
        }
        摘要->状态 = 枚举_D455观察材料读取状态::成功;
        摘要->成功 = true;
    }

    // 功能：判断 D455 像素是否有可用深度材料。
    bool D455_像素有深度材料(
        const 双目相机本能适配器::调用结果& 来源,
        std::size_t 像素索引) noexcept
    {
        if (像素索引 >= static_cast<std::size_t>((std::max<std::int64_t>)(0, 来源.预期像素数量))) {
            return false;
        }
        if (像素索引 < 来源.融合深度有效性.size()) {
            return 来源.融合深度有效性[像素索引] != 0;
        }
        if (像素索引 < 来源.深度有效性Mask.size()) {
            return 来源.深度有效性Mask[像素索引] != 0;
        }
        if (像素索引 < 来源.融合深度毫米.size()) {
            return 来源.融合深度毫米[像素索引] > 0;
        }
        if (像素索引 < 来源.原始深度毫米.size()) {
            return 来源.原始深度毫米[像素索引] > 0;
        }
        if (像素索引 < 来源.滤波深度毫米.size()) {
            return 来源.滤波深度毫米[像素索引] > 0;
        }
        if (像素索引 < 来源.补全深度毫米.size()) {
            return 来源.补全深度毫米[像素索引] > 0;
        }
        return false;
    }

    // 功能：从 D455 短期材料页只读重建像素观察归属图材料页。
    结构_像素观察归属图材料页读取结果 D455_构造像素观察归属图材料页(
        const 结构_D455短期观察材料页& 页)
    {
        结构_像素观察归属图材料页读取结果 输出{};
        输出.报告ID = 页.报告ID;
        输出.资源类型ID = D455资源类型_像素观察归属图_第一版;
        输出.创建时间毫秒 = 页.创建时间毫秒;
        输出.过期时间毫秒 = 页.过期时间毫秒;
        输出.图宽 = 页.来源.宽度;
        输出.图高 = 页.来源.高度;
        输出.来源报告编号 = static_cast<std::int64_t>(页.报告ID);
        输出.来源帧编号 = static_cast<std::int64_t>(
            页.来源.彩色帧号 != 0 ? 页.来源.彩色帧号 : 页.来源.深度帧号);
        输出.坐标系ID = 页.报告.坐标系ID;
        输出.状态值域版本 = 像素观察归属状态值域版本_第一版;
        输出.像素总数 = 页.来源.预期像素数量;

        auto 失败 = [&](枚举_D455观察材料读取状态 状态, std::string 原因) {
            输出.状态 = 状态;
            输出.成功 = false;
            输出.失败原因 = std::move(原因);
            return 输出;
        };

        if (!D455_帧像素数量有效(页.来源)) {
            return 失败(枚举_D455观察材料读取状态::材料未就绪, "D455帧像素数量无效");
        }
        const auto 像素总数 = static_cast<std::size_t>(页.来源.预期像素数量);
        输出.每像素观察候选局部编号.assign(像素总数, -1);
        输出.每像素状态值.resize(像素总数);
        输出.每像素可选先验短编号.assign(像素总数, 0);
        输出.每像素可选先验评分Q10000.assign(像素总数, 0);
        for (std::size_t 索引 = 0; 索引 < 像素总数; ++索引) {
            输出.每像素状态值[索引] = D455_像素有深度材料(页.来源, 索引)
                ? static_cast<std::int64_t>(枚举_像素观察归属状态::未归属)
                : static_cast<std::int64_t>(枚举_像素观察归属状态::深度缺失);
        }

        for (const auto& 候选 : 页.来源.空间候选列表) {
            for (const auto 像素索引 : 候选.像素索引集合) {
                const auto 索引 = static_cast<std::size_t>(像素索引);
                if (索引 >= 像素总数) {
                    continue;
                }
                if (输出.每像素观察候选局部编号[索引] >= 0
                    && 输出.每像素观察候选局部编号[索引] != 候选.候选编号) {
                    输出.每像素状态值[索引] =
                        static_cast<std::int64_t>(枚举_像素观察归属状态::争议区域);
                    continue;
                }
                if (输出.每像素观察候选局部编号[索引] < 0) {
                    ++输出.归属像素数量;
                }
                输出.每像素观察候选局部编号[索引] = 候选.候选编号;
                输出.每像素状态值[索引] =
                    static_cast<std::int64_t>(枚举_像素观察归属状态::普通观察候选);
            }
        }
        if (页.来源.空间候选列表.empty() || 输出.归属像素数量 <= 0) {
            return 失败(枚举_D455观察材料读取状态::材料未就绪, "缺像素观察候选归属索引");
        }

        输出.状态 = 枚举_D455观察材料读取状态::成功;
        输出.成功 = true;
        return 输出;
    }

    // 功能：按函数名执行对应处理。
    bool D455_尝试匹配帧句柄(
        const 结构_D455短期观察材料页& 页,
        const std::string& 句柄,
        const std::string& 资源类型,
        结构_D455观察材料句柄摘要* 摘要)
    {
        if (句柄 == 页.报告.原始观察帧句柄) {
            D455_填充材料页通用摘要(页, 资源类型, 摘要);
            摘要->数据元素数量 = 页.来源.预期像素数量;
            if (D455_帧像素数量有效(页.来源)) {
                D455_标记材料摘要成功(摘要);
            } else {
                摘要->状态 = 枚举_D455观察材料读取状态::材料未就绪;
            }
            return true;
        }
        if (句柄 == 页.报告.深度帧句柄) {
            D455_填充材料页通用摘要(页, 资源类型, 摘要);
            摘要->数据元素数量 = static_cast<std::int64_t>(
                std::max({
                    页.来源.原始深度毫米.size(),
                    页.来源.滤波深度毫米.size(),
                    页.来源.补全深度毫米.size(),
                    页.来源.融合深度毫米.size(),
                }));
            if (D455_深度帧材料有效(页.来源)) {
                D455_标记材料摘要成功(摘要);
            } else {
                摘要->状态 = 枚举_D455观察材料读取状态::材料未就绪;
            }
            return true;
        }
        if (句柄 == 页.报告.彩色帧句柄) {
            D455_填充材料页通用摘要(页, 资源类型, 摘要);
            摘要->数据元素数量 = static_cast<std::int64_t>(页.来源.颜色RGB.size());
            if (D455_颜色帧材料有效(页.来源)) {
                D455_标记材料摘要成功(摘要);
            } else {
                摘要->状态 = 枚举_D455观察材料读取状态::材料未就绪;
            }
            return true;
        }
        if (句柄 == 页.报告.对齐帧句柄) {
            D455_填充材料页通用摘要(页, 资源类型, 摘要);
            摘要->数据元素数量 = 页.来源.预期像素数量;
            if (D455_颜色帧材料有效(页.来源)
                && D455_深度帧材料有效(页.来源)
                && 页.来源.彩色深度已对齐 > 0) {
                D455_标记材料摘要成功(摘要);
            } else {
                摘要->状态 = 枚举_D455观察材料读取状态::材料未就绪;
            }
            return true;
        }
        if (句柄 == 页.报告.空间候选队列句柄) {
            D455_填充材料页通用摘要(页, 资源类型, 摘要);
            摘要->数据元素数量 = static_cast<std::int64_t>(页.来源.空间候选列表.size());
            if (页.来源.成功) {
                D455_标记材料摘要成功(摘要);
            } else {
                摘要->状态 = 枚举_D455观察材料读取状态::材料未就绪;
            }
            return true;
        }
        if (句柄 == 页.报告.像素归属账本句柄) {
            D455_填充材料页通用摘要(页, D455资源类型_像素观察归属图_第一版, 摘要);
            std::int64_t 归属像素数 = 0;
            bool 每簇有像素集合 = !页.来源.空间候选列表.empty();
            for (const auto& 候选 : 页.来源.空间候选列表) {
                归属像素数 += static_cast<std::int64_t>(候选.像素索引集合.size());
                if (候选.像素索引集合.empty()) {
                    每簇有像素集合 = false;
                }
            }
            摘要->数据元素数量 = 归属像素数;
            if (D455_帧像素数量有效(页.来源) && 每簇有像素集合) {
                D455_标记材料摘要成功(摘要);
            } else {
                摘要->状态 = 枚举_D455观察材料读取状态::材料未就绪;
            }
            return true;
        }
        return false;
    }

    // 功能：按函数名执行对应处理。
    bool D455_候选投影有效(
        const 结构_外设观察像素簇摘要& 簇,
        const 结构_D455短期观察材料页& 页) noexcept
    {
        return 页.来源.宽度 > 0
            && 页.来源.高度 > 0
            && 簇.投影最小X >= 0
            && 簇.投影最小Y >= 0
            && 簇.投影最大X >= 簇.投影最小X
            && 簇.投影最大Y >= 簇.投影最小Y
            && 簇.投影最大X < 页.来源.宽度
            && 簇.投影最大Y < 页.来源.高度;
    }

    // 功能：按函数名执行对应处理。
    bool D455_尝试匹配簇句柄(
        const 结构_D455短期观察材料页& 页,
        const std::string& 句柄,
        const std::string& 资源类型,
        结构_D455观察材料句柄摘要* 摘要)
    {
        for (const auto& 簇 : 页.报告.观察像素簇集合) {
            const bool 命中ROI = 句柄 == 簇.ROI句柄;
            const bool 命中点集 = 句柄 == 簇.点集句柄;
            const bool 命中空间候选 = 句柄 == 簇.空间候选句柄;
            const bool 命中像素归属材料 = 句柄 == 簇.像素归属验证材料句柄;
            const bool 命中掩码 = 句柄 == 簇.像素集合掩码句柄;
            const bool 命中彩色局部图 = 句柄 == 簇.彩色轮廓局部图句柄;
            const bool 命中深度局部图 = 句柄 == 簇.深度轮廓局部图句柄;
            if (!命中ROI
                && !命中点集
                && !命中空间候选
                && !命中像素归属材料
                && !命中掩码
                && !命中彩色局部图
                && !命中深度局部图) {
                continue;
            }

            const auto* 候选 = D455_查找材料页空间候选(页, 簇.来源空间候选ID);
            D455_填充材料页通用摘要(页, 资源类型, 摘要);
            摘要->候选编号 = 簇.来源空间候选ID;
            摘要->像素数量 = 簇.像素数量;
            摘要->数据元素数量 = 候选
                ? static_cast<std::int64_t>(候选->像素索引集合.size())
                : 0;

            const bool 有候选 = 候选 != nullptr;
            const bool 有像素集合 = 有候选 && !候选->像素索引集合.empty();
            const bool 投影有效 = D455_候选投影有效(簇, 页);
            bool 可用 = 有候选 && 投影有效;
            if (命中掩码 || 命中点集 || 命中像素归属材料) {
                可用 = 可用 && 有像素集合;
            }
            if (命中点集) {
                可用 = 可用 && D455_空间坐标材料有效(页.来源);
            }
            if (命中彩色局部图) {
                可用 = 可用 && D455_颜色帧材料有效(页.来源);
            }
            if (命中深度局部图) {
                可用 = 可用 && D455_深度帧材料有效(页.来源);
            }

            if (可用) {
                D455_标记材料摘要成功(摘要);
            } else {
                摘要->状态 = !有候选
                    ? 枚举_D455观察材料读取状态::簇缺来源空间候选
                    : (!投影有效
                        ? 枚举_D455观察材料读取状态::簇投影非法
                        : 枚举_D455观察材料读取状态::簇材料不可重建);
            }
            return true;
        }
        return false;
    }

    // 功能：按函数名执行对应处理。
    void D455_控制面板分割颜色(
        std::size_t 顺序,
        std::uint8_t& R,
        std::uint8_t& G,
        std::uint8_t& B) noexcept
    {
        switch (顺序 % 8) {
        case 0: R = 34; G = 211; B = 238; return;
        case 1: R = 251; G = 113; B = 133; return;
        case 2: R = 132; G = 204; B = 22; return;
        case 3: R = 250; G = 204; B = 21; return;
        case 4: R = 96; G = 165; B = 250; return;
        case 5: R = 244; G = 114; B = 182; return;
        case 6: R = 45; G = 212; B = 191; return;
        default: R = 167; G = 139; B = 250; return;
        }
    }

    // 功能：按函数名执行对应处理。
    结构_D455控制面板视频快照 D455_构造控制面板视频快照_已加锁(
        const 结构_D455短期观察材料页& 页)
    {
        结构_D455控制面板视频快照 输出{};
        输出.报告ID = 页.报告ID;
        输出.帧宽度 = 页.来源.宽度;
        输出.帧高度 = 页.来源.高度;
        输出.深度帧号 = 页.来源.深度帧号;
        输出.彩色帧号 = 页.来源.彩色帧号;
        if (!D455_颜色帧材料有效(页.来源)) {
            输出.失败原因 = "D455外设线程最新材料缺少可绘制彩色帧";
            return 输出;
        }

        const auto 像素数 = static_cast<std::size_t>(页.来源.预期像素数量);
        输出.成功 = true;
        输出.颜色RGB.reserve(像素数 * 3);
        输出.分割RGB.resize(像素数 * 3, 0);
        std::vector<std::int32_t> 分割归属(像素数, -1);
        for (std::size_t i = 0; i < 像素数; ++i) {
            const auto& 像素 = 页.来源.颜色RGB[i];
            输出.颜色RGB.push_back(像素.R);
            输出.颜色RGB.push_back(像素.G);
            输出.颜色RGB.push_back(像素.B);
            const auto 目标 = i * 3;
            输出.分割RGB[目标] = static_cast<std::uint8_t>(像素.R / 4);
            输出.分割RGB[目标 + 1] = static_cast<std::uint8_t>(像素.G / 4);
            输出.分割RGB[目标 + 2] = static_cast<std::uint8_t>(像素.B / 4);
        }

        std::size_t 候选顺序 = 0;
        for (const auto& 候选 : 页.来源.空间候选列表) {
            if (候选.像素索引集合.empty()) {
                continue;
            }
            const auto 候选标签 = static_cast<std::int32_t>(候选顺序);
            std::uint8_t 标记R = 0;
            std::uint8_t 标记G = 0;
            std::uint8_t 标记B = 0;
            D455_控制面板分割颜色(候选顺序++, 标记R, 标记G, 标记B);
            bool 候选有有效像素 = false;
            for (const auto 像素索引 : 候选.像素索引集合) {
                const auto 索引 = static_cast<std::size_t>(像素索引);
                if (像素索引 >= 像素数) {
                    continue;
                }
                const auto 目标 = 索引 * 3;
                const auto 原R = 输出.颜色RGB[目标];
                const auto 原G = 输出.颜色RGB[目标 + 1];
                const auto 原B = 输出.颜色RGB[目标 + 2];
                输出.分割RGB[目标] = static_cast<std::uint8_t>((原R * 35 + 标记R * 65) / 100);
                输出.分割RGB[目标 + 1] = static_cast<std::uint8_t>((原G * 35 + 标记G * 65) / 100);
                输出.分割RGB[目标 + 2] = static_cast<std::uint8_t>((原B * 35 + 标记B * 65) / 100);
                分割归属[索引] = 候选标签;
                ++输出.分割像素数量;
                候选有有效像素 = true;
            }
            if (候选有有效像素) {
                ++输出.分割簇数量;
            }
        }

        std::vector<std::uint8_t> 轮廓线Mask(像素数, 0);
        auto 标记轮廓线像素 = [&](int x, int y) noexcept {
            if (x < 0 || y < 0 || x >= 页.来源.宽度 || y >= 页.来源.高度) {
                return;
            }
            const auto 索引 = static_cast<std::size_t>(y) * static_cast<std::size_t>(页.来源.宽度)
                + static_cast<std::size_t>(x);
            if (索引 >= 轮廓线Mask.size() || 轮廓线Mask[索引]) {
                return;
            }
            轮廓线Mask[索引] = 1;
            ++输出.轮廓线像素数量;
        };

        for (int y = 0; y < 页.来源.高度; ++y) {
            for (int x = 0; x < 页.来源.宽度; ++x) {
                const auto 索引 = static_cast<std::size_t>(y) * static_cast<std::size_t>(页.来源.宽度)
                    + static_cast<std::size_t>(x);
                const auto 当前归属 = 分割归属[索引];
                if (当前归属 < 0) {
                    continue;
                }
                const bool 是边界 =
                    x == 0
                    || y == 0
                    || x + 1 >= 页.来源.宽度
                    || y + 1 >= 页.来源.高度
                    || 分割归属[索引 - 1] != 当前归属
                    || 分割归属[索引 + 1] != 当前归属
                    || 分割归属[索引 - static_cast<std::size_t>(页.来源.宽度)] != 当前归属
                    || 分割归属[索引 + static_cast<std::size_t>(页.来源.宽度)] != 当前归属;
                if (是边界) {
                    标记轮廓线像素(x, y);
                    标记轮廓线像素(x + 1, y);
                    标记轮廓线像素(x, y + 1);
                }
            }
        }

        for (std::size_t i = 0; i < 轮廓线Mask.size(); ++i) {
            if (!轮廓线Mask[i]) {
                continue;
            }
            const auto 目标 = i * 3;
            输出.分割RGB[目标] = 255;
            输出.分割RGB[目标 + 1] = 255;
            输出.分割RGB[目标 + 2] = 255;
        }
        return 输出;
    }

    // 功能：构建运行所需的数据结构或中间结果。
    结构_D455控制面板视频快照 D455_构造控制面板视频快照_由视频帧_已加锁(
        const 结构_D455控制面板视频帧页& 页)
    {
        结构_D455控制面板视频快照 输出{};
        输出.报告ID = 页.视频帧ID;
        输出.帧宽度 = 页.来源.宽度;
        输出.帧高度 = 页.来源.高度;
        输出.深度帧号 = 页.来源.深度帧号;
        输出.彩色帧号 = 页.来源.彩色帧号;
        if (!D455_控制面板视频帧有效(页.来源)) {
            输出.失败原因 = "D455外设线程最新视频帧缺少可绘制彩色帧";
            return 输出;
        }

        输出.成功 = true;
        输出.颜色RGB = 页.来源.颜色RGB;
        输出.分割RGB.assign(页.来源.颜色RGB.size(), 0);
        return 输出;
    }

    // 功能：等待线程、任务、外设或条件变化。
    bool 外设观察_报告满足等待项(
        const 结构_外设观察报告队列项& 报告项,
        const 结构_外设观察等待项& 等待项,
        std::int64_t 当前时间毫秒) noexcept
    {
        if (!等待项.目标外设ID.empty() && 报告项.来源外设ID != 等待项.目标外设ID) {
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
        if (等待项.等待项ID != 0
            && 报告项.匹配等待项ID != 0
            && 报告项.匹配等待项ID != 等待项.等待项ID) {
            return false;
        }
        if (等待项.目标观察约束ID != 0
            && 报告项.目标观察约束ID != 0
            && 报告项.目标观察约束ID != 等待项.目标观察约束ID) {
            return false;
        }
        const bool 逐簇识别等待 =
            等待项.期望报告类型 == 枚举_外设观察报告类型::逐簇识别报告
            || (等待项.期望报告类型 == 枚举_外设观察报告类型::未指定
                && 等待项.观察运行模式 == 枚举_外设观察运行模式::陌生环境逐簇识别);
        if (逐簇识别等待
            && 报告项.报告类型 == 枚举_外设观察报告类型::逐簇识别报告
            && 报告项.报告层级 < 1) {
            return false;
        }
        if (等待项.最大允许报告年龄毫秒 > 0
            && 报告项.时间戳毫秒 > 0
            && 当前时间毫秒 - 报告项.时间戳毫秒 > 等待项.最大允许报告年龄毫秒) {
            return false;
        }
        return true;
    }

    // 功能：按函数名执行对应处理。
    bool 目标观察约束_有当前值(const 结构_目标观察约束特征组& 约束) noexcept
    {
        switch (约束.目标特征当前值类型) {
        case 枚举_目标特征当前值类型::I64:
            return true;
        case 枚举_目标特征当前值类型::VecU句柄:
        case 枚举_目标特征当前值类型::指针句柄:
        case 枚举_目标特征当前值类型::文本句柄:
            return !约束.目标特征当前值句柄.empty();
        case 枚举_目标特征当前值类型::未指定:
        default:
            return false;
        }
    }

    // 功能：按函数名执行对应处理。
    bool 目标观察约束_二维范围有效(const 结构_目标观察约束特征组& 约束) noexcept
    {
        return 约束.二维最大X > 约束.二维最小X
            && 约束.二维最大Y > 约束.二维最小Y;
    }

    // 功能：按函数名执行对应处理。
    bool 目标观察约束_空间范围有效(const 结构_目标观察约束特征组& 约束) noexcept
    {
        return 约束.空间范围最大X > 约束.空间范围最小X
            && 约束.空间范围最大Y > 约束.空间范围最小Y
            && 约束.空间范围最大Z > 约束.空间范围最小Z;
    }

    // 功能：按函数名执行对应处理。
    bool 目标观察约束_有定位材料(const 结构_目标观察约束特征组& 约束) noexcept
    {
        return !约束.ROI句柄.empty()
            || !约束.像素集合掩码句柄.empty()
            || !约束.点集句柄.empty()
            || 目标观察约束_二维范围有效(约束)
            || 目标观察约束_空间范围有效(约束);
    }

    // 功能：按函数名执行对应处理。
    bool 目标观察约束_有精细材料(const 结构_目标观察约束特征组& 约束) noexcept
    {
        return !约束.像素集合掩码句柄.empty()
            || !约束.点集句柄.empty();
    }

    // 功能：把 VecU 运行期句柄转成现有目标观察约束可回查句柄槽使用的不透明锚点。
    std::string 外设先验_VecU不透明句柄(const char* 前缀, VecU句柄 句柄)
    {
        if (!句柄.有效()) return {};
        return std::string(前缀) + std::to_string(static_cast<std::uint64_t>(句柄.主信息指针));
    }

    // 功能：把场景体素快照生成时间与外设队列毫秒时间统一成快照年龄材料。
    std::int64_t 外设先验_快照年龄毫秒(
        const 场景体素模块::结构_SceneVoxelPrior& 先验快照,
        std::int64_t 当前时间毫秒) noexcept
    {
        if (先验快照.生成时间 <= 0) return 0;
        if (当前时间毫秒 <= 0) {
            当前时间毫秒 = 外设观察当前时间毫秒();
        }
        if (当前时间毫秒 <= 0) return 0;
        const auto 当前时间毫秒U64 = static_cast<std::uint64_t>(当前时间毫秒);
        const auto 当前时间微秒 = 当前时间毫秒U64 > (std::numeric_limits<std::uint64_t>::max)() / 1000
            ? (std::numeric_limits<std::uint64_t>::max)()
            : 当前时间毫秒U64 * 1000;
        if (当前时间微秒 <= 先验快照.生成时间) return 0;
        const auto 年龄毫秒 = (当前时间微秒 - 先验快照.生成时间) / 1000;
        return 年龄毫秒 > static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)())
            ? (std::numeric_limits<std::int64_t>::max)()
            : static_cast<std::int64_t>(年龄毫秒);
    }

    // 功能：按只读快照存在项汇总第一版粗空间范围。
    bool 外设先验_填充粗空间范围(
        结构_目标观察约束特征组& 约束,
        const 场景体素模块::结构_SceneVoxelPrior& 先验快照) noexcept
    {
        bool 已填充 = false;
        for (const auto& 项 : 先验快照.存在项集合) {
            if (!项.场景AABB_mm.有效()) continue;
            if (!已填充) {
                约束.空间范围最小X = 项.场景AABB_mm.最小X_mm;
                约束.空间范围最大X = 项.场景AABB_mm.最大X_mm;
                约束.空间范围最小Y = 项.场景AABB_mm.最小Y_mm;
                约束.空间范围最大Y = 项.场景AABB_mm.最大Y_mm;
                约束.空间范围最小Z = 项.场景AABB_mm.最小Z_mm;
                约束.空间范围最大Z = 项.场景AABB_mm.最大Z_mm;
                已填充 = true;
                continue;
            }
            约束.空间范围最小X = std::min(约束.空间范围最小X, 项.场景AABB_mm.最小X_mm);
            约束.空间范围最大X = std::max(约束.空间范围最大X, 项.场景AABB_mm.最大X_mm);
            约束.空间范围最小Y = std::min(约束.空间范围最小Y, 项.场景AABB_mm.最小Y_mm);
            约束.空间范围最大Y = std::max(约束.空间范围最大Y, 项.场景AABB_mm.最大Y_mm);
            约束.空间范围最小Z = std::min(约束.空间范围最小Z, 项.场景AABB_mm.最小Z_mm);
            约束.空间范围最大Z = std::max(约束.空间范围最大Z, 项.场景AABB_mm.最大Z_mm);
        }
        return 已填充;
    }

    // 功能：把场景体素先验句柄和粗空间范围写入目标观察约束材料槽。
    bool 外设先验_填充先验材料(
        结构_目标观察约束特征组& 约束,
        const 场景体素模块::结构_SceneVoxelPrior& 先验快照)
    {
        约束.ROI句柄 = 外设先验_VecU不透明句柄(
            "vecu://scene-prior/roi/",
            先验快照.先验感兴趣区域集合句柄);
        约束.像素集合掩码句柄 = 外设先验_VecU不透明句柄(
            "vecu://scene-prior/free-space-mask/",
            先验快照.已知空闲区句柄);
        约束.点集句柄 = 外设先验_VecU不透明句柄(
            "vecu://scene-prior/depth-range/",
            先验快照.预计深度范围图句柄);

        const bool 有粗空间范围 = 外设先验_填充粗空间范围(约束, 先验快照);
        return 有粗空间范围
            || !约束.ROI句柄.empty()
            || !约束.像素集合掩码句柄.empty()
            || !约束.点集句柄.empty();
    }

    // 功能：等待线程、任务、外设或条件变化。
    bool 目标观察约束_匹配等待项(
        const 结构_目标观察约束特征组& 约束,
        const 结构_外设观察等待项& 等待项) noexcept
    {
        if (等待项.目标观察约束ID != 0) {
            return 约束.约束ID == 等待项.目标观察约束ID;
        }
        if (等待项.等待项ID != 0 && 约束.来源等待项ID == 等待项.等待项ID) {
            return true;
        }
        if (!等待项.目标存在ID.empty() && 约束.目标存在ID == 等待项.目标存在ID) {
            if (等待项.目标特征类型ID.empty()
                || 约束.目标特征类型ID == 等待项.目标特征类型ID) {
                return true;
            }
        }
        return false;
    }

    // 功能：按函数名执行对应处理。
    bool 目标观察约束_匹配目标特征(
        const 结构_目标观察约束特征组& 约束,
        const std::string& 目标存在ID,
        const std::string& 目标特征类型ID) noexcept
    {
        if (!目标存在ID.empty() && 约束.目标存在ID != 目标存在ID) {
            return false;
        }
        if (!目标特征类型ID.empty() && 约束.目标特征类型ID != 目标特征类型ID) {
            return false;
        }
        return true;
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    void 目标观察约束_修正提交默认值(结构_目标观察约束特征组& 约束)
    {
        if (约束.约束ID == 0) {
            约束.约束ID = ++目标观察约束ID计数器();
        }
        if (约束.写入时间毫秒 == 0) {
            约束.写入时间毫秒 = 外设观察当前时间毫秒();
        }
        if (约束.TTL毫秒 <= 0) {
            约束.TTL毫秒 = 目标观察约束默认TTL毫秒;
        }
        if (约束.状态 == 枚举_目标观察约束状态::未定义) {
            约束.状态 = 枚举_目标观察约束状态::可用;
        }
    }

    // 功能：清理缓存、临时状态或运行资源。
    bool 目标观察约束_应清理(
        const 结构_目标观察约束特征组& 约束,
        std::int64_t 当前时间毫秒) noexcept
    {
        if (约束.状态 == 枚举_目标观察约束状态::已撤销) {
            return true;
        }
        return 约束.TTL毫秒 > 0
            && 约束.写入时间毫秒 > 0
            && 当前时间毫秒 - 约束.写入时间毫秒 > 约束.TTL毫秒;
    }

    // 功能：清理缓存、临时状态或运行资源。
    std::size_t 目标观察约束_清理已加锁(
        结构_外设观察队列共享状态& 状态,
        std::int64_t 当前时间毫秒)
    {
        const auto 原数量 = 状态.目标观察约束缓冲区.size();
        状态.目标观察约束缓冲区.erase(
            std::remove_if(
                状态.目标观察约束缓冲区.begin(),
                状态.目标观察约束缓冲区.end(),
                [当前时间毫秒](const 结构_目标观察约束特征组& 约束) noexcept {
                    return 目标观察约束_应清理(约束, 当前时间毫秒);
                }),
            状态.目标观察约束缓冲区.end());
        while (状态.目标观察约束缓冲区.size() > 状态.目标观察约束缓冲区容量) {
            状态.目标观察约束缓冲区.pop_front();
        }
        return 原数量 > 状态.目标观察约束缓冲区.size()
            ? 原数量 - 状态.目标观察约束缓冲区.size()
            : 0;
    }
}

// 功能：执行对应模块、线程或方法的运行逻辑。
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

// 功能：按函数名执行对应处理。
const char* 外设分割处理模式文本(枚举_外设分割处理模式 类型) noexcept
{
    switch (类型) {
    case 枚举_外设分割处理模式::全局观察: return "全局观察";
    case 枚举_外设分割处理模式::高价值待识别: return "高价值待识别";
    case 枚举_外设分割处理模式::扫描候选: return "扫描候选";
    case 枚举_外设分割处理模式::目标跟踪: return "目标跟踪";
    case 枚举_外设分割处理模式::静止低频维护: return "静止低频维护";
    case 枚举_外设分割处理模式::低价值: return "低价值";
    case 枚举_外设分割处理模式::未指定: return "未指定";
    default: return "未知";
    }
}

// 功能：按函数名执行对应处理。
const char* 外设Tracker轨迹状态文本(枚举_外设Tracker轨迹状态 类型) noexcept
{
    switch (类型) {
    case 枚举_外设Tracker轨迹状态::新建: return "新建";
    case 枚举_外设Tracker轨迹状态::候选: return "候选";
    case 枚举_外设Tracker轨迹状态::稳定: return "稳定";
    case 枚举_外设Tracker轨迹状态::短时丢失: return "短时丢失";
    case 枚举_外设Tracker轨迹状态::已丢失: return "已丢失";
    case 枚举_外设Tracker轨迹状态::遮挡: return "遮挡";
    case 枚举_外设Tracker轨迹状态::冲突: return "冲突";
    case 枚举_外设Tracker轨迹状态::未指定: return "未指定";
    default: return "未知";
    }
}

// 功能：按函数名执行对应处理。
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

// 功能：读取像素观察归属状态值域当前代码登记版本。
std::int64_t 读取像素观察归属状态值域版本() noexcept
{
    return 像素观察归属状态值域版本_第一版;
}

// 功能：把像素观察归属状态值转为只读审计文本。
const char* 像素观察归属状态文本(枚举_像素观察归属状态 状态) noexcept
{
    switch (状态) {
    case 枚举_像素观察归属状态::未评估: return "未评估";
    case 枚举_像素观察归属状态::无效像素: return "无效像素";
    case 枚举_像素观察归属状态::未归属: return "未归属";
    case 枚举_像素观察归属状态::承载面候选: return "承载面候选";
    case 枚举_像素观察归属状态::普通观察候选: return "普通观察候选";
    case 枚举_像素观察归属状态::遮挡边界: return "遮挡边界";
    case 枚举_像素观察归属状态::争议区域: return "争议区域";
    case 枚举_像素观察归属状态::深度缺失: return "深度缺失";
    case 枚举_像素观察归属状态::图像边界不可信区域: return "图像边界/不可信区域";
    case 枚举_像素观察归属状态::场景体素先验投影区域: return "场景体素先验投影区域";
    case 枚举_像素观察归属状态::场景体素先验遮挡区域: return "场景体素先验遮挡区域";
    case 枚举_像素观察归属状态::自由空间冲突区域: return "自由空间冲突区域";
    case 枚举_像素观察归属状态::新占据候选区域: return "新占据候选区域";
    case 枚举_像素观察归属状态::已知空闲命中区域: return "已知空闲命中区域";
    case 枚举_像素观察归属状态::低质量区域: return "低质量区域";
    case 枚举_像素观察归属状态::保留: return "保留";
    default: return "未知";
    }
}

// 功能：按函数名执行对应处理。
const char* 观察材料等级文本(枚举_观察材料等级 类型) noexcept
{
    switch (类型) {
    case 枚举_观察材料等级::不合格: return "不合格";
    case 枚举_观察材料等级::可用: return "可用";
    case 枚举_观察材料等级::优质: return "优质";
    case 枚举_观察材料等级::未定义: return "未定义";
    default: return "未知";
    }
}

// 功能：按函数名执行对应处理。
const char* 外设材料能力等级文本(枚举_外设材料能力等级 类型) noexcept
{
    switch (类型) {
    case 枚举_外设材料能力等级::候选或缺口: return "候选或缺口";
    case 枚举_外设材料能力等级::平面轮廓: return "平面轮廓";
    case 枚举_外设材料能力等级::深度锚定: return "深度锚定";
    case 枚举_外设材料能力等级::空间可用: return "空间可用";
    case 枚举_外设材料能力等级::未定义: return "未定义";
    default: return "未知";
    }
}

// 功能：按函数名执行对应处理。
const char* 外设坐标对齐状态文本(枚举_外设坐标对齐状态 类型) noexcept
{
    switch (类型) {
    case 枚举_外设坐标对齐状态::已对齐: return "已对齐";
    case 枚举_外设坐标对齐状态::未对齐: return "未对齐";
    case 枚举_外设坐标对齐状态::降级使用: return "降级使用";
    case 枚举_外设坐标对齐状态::未参与: return "未参与";
    case 枚举_外设坐标对齐状态::未确定: return "未确定";
    default: return "未知";
    }
}

// 功能：按函数名执行对应处理。
const char* 外设坐标系一致性状态文本(枚举_外设坐标系一致性状态 类型) noexcept
{
    switch (类型) {
    case 枚举_外设坐标系一致性状态::一致: return "一致";
    case 枚举_外设坐标系一致性状态::降级: return "降级";
    case 枚举_外设坐标系一致性状态::不一致: return "不一致";
    case 枚举_外设坐标系一致性状态::未确定: return "未确定";
    default: return "未知";
    }
}

// 功能：提交事实、动态、任务状态或运行回执。
const char* 外设提交包类型文本(枚举_外设提交包类型 类型) noexcept
{
    switch (类型) {
    case 枚举_外设提交包类型::识别: return "识别提交包";
    case 枚举_外设提交包类型::扫描: return "扫描提交包";
    case 枚举_外设提交包类型::跟踪: return "跟踪提交包";
    case 枚举_外设提交包类型::未指定: return "未指定";
    default: return "未知";
    }
}

// 功能：按函数名执行对应处理。
const char* 目标观察约束可用性文本(枚举_目标观察约束可用性 类型) noexcept
{
    switch (类型) {
    case 枚举_目标观察约束可用性::可用: return "可用";
    case 枚举_目标观察约束可用性::可降级使用: return "可降级使用";
    case 枚举_目标观察约束可用性::已过期: return "已过期";
    case 枚举_目标观察约束可用性::目标已替代: return "目标已替代";
    case 枚举_目标观察约束可用性::材料不可回查: return "材料不可回查";
    case 枚举_目标观察约束可用性::缺目标存在: return "缺目标存在";
    case 枚举_目标观察约束可用性::缺目标特征: return "缺目标特征";
    case 枚举_目标观察约束可用性::缺当前值: return "缺当前值";
    case 枚举_目标观察约束可用性::缺定位材料: return "缺定位材料";
    case 枚举_目标观察约束可用性::已撤销: return "已撤销";
    case 枚举_目标观察约束可用性::未定义: return "未定义";
    default: return "未知";
    }
}

// 功能：判断条件、状态或业务结果是否成立。
结构_观察材料质量判定 判断外设观察材料质量(
    const 结构_外设观察报告队列项& 报告项,
    const 结构_外设观察等待项* 等待项)
{
    结构_观察材料质量判定 判定{};
    constexpr std::int64_t 最低可用有效率 = 1;
    constexpr std::int64_t 优质最低有效率 = 5000;
    constexpr std::int64_t 优质最低帧质量 = 1;
    constexpr std::int64_t 逐簇稳定最低连续命中 = 3;
    constexpr std::int64_t 逐簇稳定最大复现离散度 = 2200;

    const auto 设置不合格 = [&]() {
        判定.等级 = 枚举_观察材料等级::不合格;
        return 判定;
    };

    if (报告项.报告类型 == 枚举_外设观察报告类型::未指定) {
        return 设置不合格();
    }
    if (!报告项.采集成功) {
        return 设置不合格();
    }

    const auto 当前时间 = 外设观察当前时间毫秒();
    if (等待项
        && 等待项->最大允许报告年龄毫秒 > 0
        && 报告项.时间戳毫秒 > 0
        && 当前时间 - 报告项.时间戳毫秒 > 等待项->最大允许报告年龄毫秒) {
        return 设置不合格();
    }

    const bool 有报告材料 =
        !报告项.原始观察帧句柄.empty()
        || !报告项.深度帧句柄.empty()
        || !报告项.彩色帧句柄.empty()
        || !报告项.对齐帧句柄.empty()
        || !报告项.空间候选队列句柄.empty()
        || !报告项.像素归属账本句柄.empty();
    const bool 是逐簇识别报告 =
        报告项.报告类型 == 枚举_外设观察报告类型::逐簇识别报告;
    bool 有可回查簇材料 = false;
    std::int64_t 目标区域深度有效率 = 0;
    std::int64_t 目标区域空间有效率 = 0;
    std::int64_t 逐簇稳定硬门簇数量 = 0;
    std::int64_t 平面轮廓稳定簇数量 = 0;
    std::int64_t 深度锚定簇数量 = 0;
    std::int64_t 空间可用簇数量 = 0;
    std::int64_t 稳定锚点数量 = 0;
    std::int64_t 边界锚点覆盖率累计 = 0;
    std::int64_t 边界锚点覆盖率样本 = 0;
    for (const auto& 簇 : 报告项.观察像素簇集合) {
        if (!簇.ROI句柄.empty()
            || !簇.像素集合掩码句柄.empty()
            || !簇.点集句柄.empty()
            || !簇.空间候选句柄.empty()
            || !簇.像素归属验证材料句柄.empty()) {
            有可回查簇材料 = true;
        }
        目标区域深度有效率 = (std::max<std::int64_t>)(目标区域深度有效率, 簇.深度有效率);
        目标区域空间有效率 = (std::max<std::int64_t>)(目标区域空间有效率, 簇.空间坐标有效率);
        const bool 簇跨帧稳定 =
            簇.跨帧轨迹稳定状态 > 0
            && 簇.跨帧连续稳定命中次数 >= 逐簇稳定最低连续命中
            && 簇.跨帧复现离散度 <= 逐簇稳定最大复现离散度;
        const bool 簇有平面轮廓材料 =
            簇.像素数量 > 0
            || !簇.ROI句柄.empty()
            || !簇.像素集合掩码句柄.empty()
            || !簇.彩色轮廓局部图句柄.empty();
        const bool 簇深度锚定 =
            簇.深度有效率 >= 优质最低有效率
            || 簇.深度轮廓局部图状态 > 0
            || !簇.深度轮廓局部图句柄.empty();
        const bool 簇空间可用 =
            簇.空间坐标有效率 >= 优质最低有效率
            || !簇.空间候选句柄.empty()
            || !簇.点集句柄.empty();
        const auto 簇估算锚点数量 = 簇.稳定锚点数量 > 0
            ? 簇.稳定锚点数量
            : (簇.像素数量 > 0
                ? (簇.像素数量 * (std::max<std::int64_t>)(0, 簇.深度有效率)) / 10000
                : 0);
        稳定锚点数量 += (std::max<std::int64_t>)(0, 簇估算锚点数量);
        const auto 簇边界锚点覆盖率 = 簇.边界锚点覆盖率 > 0
            ? 簇.边界锚点覆盖率
            : 簇.深度有效率;
        if (簇边界锚点覆盖率 > 0) {
            边界锚点覆盖率累计 += 簇边界锚点覆盖率;
            ++边界锚点覆盖率样本;
        }
        if (簇跨帧稳定) {
            ++逐簇稳定硬门簇数量;
            if (簇有平面轮廓材料) {
                ++平面轮廓稳定簇数量;
            }
            if (簇有平面轮廓材料 && 簇深度锚定) {
                ++深度锚定簇数量;
            }
            if (簇有平面轮廓材料 && 簇深度锚定 && 簇空间可用) {
                ++空间可用簇数量;
            }
        }
    }

    const bool 需要簇材料 =
        是逐簇识别报告
        || 报告项.报告类型 == 枚举_外设观察报告类型::扫描变化报告
        || 报告项.报告类型 == 枚举_外设观察报告类型::跟踪报告;
    if (!有报告材料 && !有可回查簇材料) {
        return 设置不合格();
    }
    if (需要簇材料 && 报告项.观察像素簇集合.empty()) {
        return 设置不合格();
    }

    const bool 全局基础可用 =
        报告项.深度有效率 >= 最低可用有效率
        || 报告项.空间坐标有效率 >= 最低可用有效率
        || 报告项.帧质量评分 >= 优质最低帧质量;
    const bool 目标区域基础可用 =
        目标区域深度有效率 >= 最低可用有效率
        || 目标区域空间有效率 >= 最低可用有效率;
    const bool 逐簇稳定子集硬门达标 =
        !是逐簇识别报告
        || (报告项.报告层级 >= 1
            && 报告项.跨帧采集状态 > 0
            && 报告项.跨帧稳定观察像素簇数量 > 0
            && !报告项.观察像素簇集合.empty()
            && 逐簇稳定硬门簇数量 == static_cast<std::int64_t>(报告项.观察像素簇集合.size()));
    const auto 簇数量 = static_cast<std::int64_t>(报告项.观察像素簇集合.size());
    const std::int64_t 总像素数量 = [&]() {
        std::int64_t 总量 = 0;
        for (const auto& 簇 : 报告项.观察像素簇集合) {
            总量 += (std::max<std::int64_t>)(0, 簇.像素数量);
        }
        return 总量;
    }();
    if (报告项.稳定锚点数量 > 0) {
        稳定锚点数量 = 报告项.稳定锚点数量;
    }
    const auto 稳定锚点密度 = 报告项.稳定锚点密度 > 0
        ? 报告项.稳定锚点密度
        : (总像素数量 > 0 ? (稳定锚点数量 * 10000) / 总像素数量 : 0);
    const auto 稳定锚点覆盖率 = 报告项.稳定锚点覆盖率 > 0
        ? 报告项.稳定锚点覆盖率
        : 稳定锚点密度;
    const auto 边界锚点覆盖率 = 报告项.边界锚点覆盖率 > 0
        ? 报告项.边界锚点覆盖率
        : (边界锚点覆盖率样本 > 0 ? 边界锚点覆盖率累计 / 边界锚点覆盖率样本 : 0);
    const bool anchor支持达标 =
        稳定锚点数量 >= 16
        && 稳定锚点密度 >= 3000
        && 稳定锚点覆盖率 >= 3000;
    const bool 坐标系一致 =
        报告项.坐标系一致性状态 == 枚举_外设坐标系一致性状态::一致;
    const bool 平面轮廓稳定 =
        报告项.平面轮廓稳定状态 > 0
        || (是逐簇识别报告
            && 逐簇稳定子集硬门达标
            && 簇数量 > 0
            && 平面轮廓稳定簇数量 == 簇数量);
    const bool 深度锚定 =
        报告项.深度锚定状态 > 0
        || (平面轮廓稳定
            && anchor支持达标
            && (报告项.深度有效率 >= 优质最低有效率
                || 目标区域深度有效率 >= 优质最低有效率
                || (簇数量 > 0 && 深度锚定簇数量 == 簇数量)));
    const bool 空间可用 =
        坐标系一致
        && (报告项.空间可用状态 > 0
            || (深度锚定
                && (报告项.空间坐标有效率 >= 优质最低有效率
                    || 目标区域空间有效率 >= 优质最低有效率
                    || (簇数量 > 0 && 空间可用簇数量 == 簇数量))));
    if (!全局基础可用 && !目标区域基础可用 && !平面轮廓稳定) {
        return 设置不合格();
    }

    const bool 全局优质 =
        报告项.帧质量评分 >= 优质最低帧质量
        && 报告项.深度有效率 >= 优质最低有效率
        && 报告项.空间坐标有效率 >= 优质最低有效率;
    const bool 约束匹配材料 =
        报告项.目标约束使用状态 == 1
        || 报告项.目标约束使用状态 == 2;
    const bool 目标区域优质 =
        约束匹配材料
        && !报告项.观察像素簇集合.empty()
        && 目标区域深度有效率 >= 优质最低有效率
        && 目标区域空间有效率 >= 优质最低有效率;

    判定.目标区域优质 = 目标区域优质;
    判定.等级 = (全局优质 || 目标区域优质)
        ? 枚举_观察材料等级::优质
        : 枚举_观察材料等级::可用;
    判定.可进入验证方法 = 判定.等级 != 枚举_观察材料等级::不合格;
    判定.可进入识别方法 =
        判定.可进入验证方法
        && 是逐簇识别报告
        && 逐簇稳定子集硬门达标;

    const bool 强约束命中 = 报告项.目标约束使用状态 == 1;
    const bool 非首帧扫描 =
        报告项.报告类型 == 枚举_外设观察报告类型::扫描变化报告
        && 报告项.扫描变化基准状态 == 枚举_扫描变化基准状态::有可比历史;
    const bool 存在扫描变化 =
        报告项.新增观察像素簇数量 > 0
        || 报告项.消失观察像素簇数量 > 0
        || 报告项.变化观察像素簇数量 > 0
        || 报告项.目标特征误差值 > 0;
    判定.可生成扫描变化候选 =
        判定.等级 == 枚举_观察材料等级::优质
        && 报告项.报告类型 == 枚举_外设观察报告类型::扫描变化报告
        && 强约束命中
        && 非首帧扫描
        && 存在扫描变化;
    判定.可生成跟踪动态候选 =
        判定.等级 == 枚举_观察材料等级::优质
        && 报告项.报告类型 == 枚举_外设观察报告类型::跟踪报告
        && 强约束命中
        && 报告项.跟踪状态值 > 0
        && 报告项.丢失状态值 == 0;
    判定.平面轮廓稳定状态 = 平面轮廓稳定;
    判定.深度锚定状态 = 深度锚定;
    判定.空间可用状态 = 空间可用;
    判定.坐标系一致性状态 = 报告项.坐标系一致性状态;
    判定.稳定锚点数量 = 稳定锚点数量;
    判定.稳定锚点密度 = 稳定锚点密度;
    判定.稳定锚点覆盖率 = 稳定锚点覆盖率;
    判定.边界锚点覆盖率 = 边界锚点覆盖率;
    判定.可承接能力等级 = 空间可用
        ? 枚举_外设材料能力等级::空间可用
        : (深度锚定
            ? 枚举_外设材料能力等级::深度锚定
            : (平面轮廓稳定
                ? 枚举_外设材料能力等级::平面轮廓
                : (判定.可进入验证方法
                    ? 枚举_外设材料能力等级::候选或缺口
                    : 枚举_外设材料能力等级::未定义)));
    判定.外设材料可承接状态 =
        平面轮廓稳定
        || 判定.可生成扫描变化候选
        || 判定.可生成跟踪动态候选;
    判定.可交付观察方法材料 =
        判定.可生成扫描变化候选
        || 判定.可生成跟踪动态候选
        || (判定.可进入识别方法 && 判定.外设材料可承接状态);
    return 判定;
}

// 功能：按既有报告证据和兼容质量判定计算 1175 约束下的 I64 质量收束状态。
std::int64_t 计算观察材料质量收束状态(
    const 结构_外设观察报告队列项& 报告项,
    const 结构_观察材料质量判定& 判定,
    const 结构_外设观察等待项* 等待项)
{
    if (报告项.报告ID == 0
        && 报告项.报告类型 == 枚举_外设观察报告类型::未指定
        && !报告项.采集成功) {
        return 外设质量状态_未评估;
    }

    const auto 当前时间 = 外设观察当前时间毫秒();
    if (等待项
        && 等待项->最大允许报告年龄毫秒 > 0
        && 报告项.时间戳毫秒 > 0
        && 当前时间 - 报告项.时间戳毫秒 > 等待项->最大允许报告年龄毫秒) {
        return 外设质量状态_需复验;
    }
    if (等待项
        && 等待项->目标约束有效截止时间毫秒 > 0
        && 当前时间 > 等待项->目标约束有效截止时间毫秒) {
        return 外设质量状态_需复验;
    }
    if (报告项.坐标系一致性状态 == 枚举_外设坐标系一致性状态::不一致) {
        return 外设质量状态_需复验;
    }
    if (报告项.目标约束使用状态 >= 3) {
        return 外设质量状态_需复验;
    }

    if (报告项.报告类型 == 枚举_外设观察报告类型::未指定) {
        return 外设质量状态_未评估;
    }
    if (!报告项.采集成功) {
        return 外设质量状态_不可用;
    }

    const auto 句柄可回查 = [](const std::string& 句柄) {
        return !句柄.empty() && 解析D455观察材料句柄(句柄).成功;
    };
    const bool 有报告材料 =
        句柄可回查(报告项.原始观察帧句柄)
        || 句柄可回查(报告项.深度帧句柄)
        || 句柄可回查(报告项.彩色帧句柄)
        || 句柄可回查(报告项.对齐帧句柄)
        || 句柄可回查(报告项.空间候选队列句柄)
        || 句柄可回查(报告项.像素归属账本句柄);
    bool 有簇材料 = false;
    for (const auto& 簇 : 报告项.观察像素簇集合) {
        if (句柄可回查(簇.ROI句柄)
            || 句柄可回查(簇.像素集合掩码句柄)
            || 句柄可回查(簇.点集句柄)
            || 句柄可回查(簇.空间候选句柄)
            || 句柄可回查(簇.像素归属验证材料句柄)
            || 句柄可回查(簇.彩色轮廓局部图句柄)
            || 句柄可回查(簇.深度轮廓局部图句柄)) {
            有簇材料 = true;
            break;
        }
    }

    const bool 需要簇材料 =
        报告项.报告类型 == 枚举_外设观察报告类型::逐簇识别报告
        || 报告项.报告类型 == 枚举_外设观察报告类型::扫描变化报告
        || 报告项.报告类型 == 枚举_外设观察报告类型::跟踪报告;
    if ((!有报告材料 && !有簇材料)
        || (需要簇材料 && 报告项.观察像素簇集合.empty())) {
        return 外设质量状态_不可用;
    }

    const bool 基础质量不足 =
        报告项.低质量区域数量 > 0
        || 报告项.帧质量评分 <= 0
        || (报告项.深度有效率 <= 0 && 报告项.空间坐标有效率 <= 0);
    if (判定.等级 == 枚举_观察材料等级::未定义) {
        return 基础质量不足 ? 外设质量状态_低质量 : 外设质量状态_未评估;
    }
    if (判定.等级 == 枚举_观察材料等级::不合格) {
        return 基础质量不足 ? 外设质量状态_低质量 : 外设质量状态_不可用;
    }
    if (判定.空间可用状态
        || 判定.可承接能力等级 == 枚举_外设材料能力等级::空间可用) {
        return 外设质量状态_可用;
    }
    if (判定.深度锚定状态
        || 判定.平面轮廓稳定状态
        || 判定.可承接能力等级 == 枚举_外设材料能力等级::深度锚定
        || 判定.可承接能力等级 == 枚举_外设材料能力等级::平面轮廓) {
        return 外设质量状态_部分可用;
    }
    if (基础质量不足
        || 判定.可承接能力等级 == 枚举_外设材料能力等级::候选或缺口) {
        return 外设质量状态_低质量;
    }
    return 外设质量状态_未评估;
}

// 功能：按 D9 F 层输入材料收束观察帧清晰度评分，不写世界树事实或日志状态。
结构_观察帧清晰度评分 计算观察帧清晰度评分(
    const 结构_外设观察报告队列项& 报告项,
    const 结构_观察材料质量判定& 判定,
    const 结构_外设观察等待项* 等待项)
{
    auto 限制Q10000 = [](std::int64_t 值) noexcept {
        return (std::min<std::int64_t>)(10000, (std::max<std::int64_t>)(0, 值));
    };
    auto 追加阻断 = [](std::int64_t& 掩码, 枚举_观察帧清晰度阻断原因 原因) noexcept {
        掩码 |= static_cast<std::int64_t>(原因);
    };

    结构_观察帧清晰度评分 输出{};
    输出.来源报告ID = 报告项.报告ID;
    输出.来源帧ID = 外设提交_帧ID(报告项);
    输出.深度有效率Q10000 = 限制Q10000(报告项.深度有效率);
    输出.空间坐标有效率Q10000 = 限制Q10000(报告项.空间坐标有效率);
    输出.帧质量评分Q10000 = 限制Q10000(报告项.帧质量评分);
    输出.未解释区域比例Q10000 = 限制Q10000(报告项.未解释区域比例);
    输出.低质量区域数量 = (std::max<std::int64_t>)(0, 报告项.低质量区域数量);

    const auto 质量状态 = 计算观察材料质量收束状态(报告项, 判定, 等待项);
    if (报告项.报告ID == 0 && 报告项.报告类型 == 枚举_外设观察报告类型::未指定) {
        追加阻断(输出.阻断原因掩码, 枚举_观察帧清晰度阻断原因::缺报告);
    }
    if (!报告项.采集成功) {
        追加阻断(输出.阻断原因掩码, 枚举_观察帧清晰度阻断原因::采集失败);
    }
    if (质量状态 == 外设质量状态_不可用) {
        追加阻断(输出.阻断原因掩码, 枚举_观察帧清晰度阻断原因::材料不可回查);
    } else if (质量状态 == 外设质量状态_需复验) {
        追加阻断(输出.阻断原因掩码, 枚举_观察帧清晰度阻断原因::材料需复验);
    }
    if (报告项.坐标系一致性状态 == 枚举_外设坐标系一致性状态::不一致) {
        追加阻断(输出.阻断原因掩码, 枚举_观察帧清晰度阻断原因::坐标冲突);
    }
    if (输出.帧质量评分Q10000 <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察帧清晰度阻断原因::帧质量不足);
    }
    if (输出.深度有效率Q10000 <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察帧清晰度阻断原因::深度不足);
    }
    if (输出.空间坐标有效率Q10000 <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察帧清晰度阻断原因::空间不足);
    }
    if (输出.低质量区域数量 > 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察帧清晰度阻断原因::低质量区域);
    }
    if (输出.未解释区域比例Q10000 >= 5000) {
        追加阻断(输出.阻断原因掩码, 枚举_观察帧清晰度阻断原因::未解释区域偏高);
    }

    const auto 质量项数量 =
        (输出.帧质量评分Q10000 > 0 ? 1 : 0)
        + (输出.深度有效率Q10000 > 0 ? 1 : 0)
        + (输出.空间坐标有效率Q10000 > 0 ? 1 : 0);
    if (质量项数量 > 0) {
        const auto 基础平均 =
            (输出.帧质量评分Q10000
                + 输出.深度有效率Q10000
                + 输出.空间坐标有效率Q10000) / 3;
        const auto 未解释扣分 = 输出.未解释区域比例Q10000 / 2;
        const auto 低质量扣分 = (std::min<std::int64_t>)(3000, 输出.低质量区域数量 * 500);
        输出.评分Q10000 = 限制Q10000(基础平均 - 未解释扣分 - 低质量扣分);
    }

    if ((输出.阻断原因掩码 & static_cast<std::int64_t>(枚举_观察帧清晰度阻断原因::坐标冲突)) != 0) {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::冲突);
    } else if ((输出.阻断原因掩码 & static_cast<std::int64_t>(枚举_观察帧清晰度阻断原因::材料需复验)) != 0) {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::需复验);
    } else if (输出.评分Q10000 <= 0
        || (输出.阻断原因掩码 & (static_cast<std::int64_t>(枚举_观察帧清晰度阻断原因::缺报告)
            | static_cast<std::int64_t>(枚举_观察帧清晰度阻断原因::采集失败)
            | static_cast<std::int64_t>(枚举_观察帧清晰度阻断原因::材料不可回查))) != 0) {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::不可用);
    } else if (输出.评分Q10000 >= 8000 && 输出.阻断原因掩码 == 0) {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::高清晰);
    } else if (输出.评分Q10000 >= 5000
        && (输出.阻断原因掩码 & (static_cast<std::int64_t>(枚举_观察帧清晰度阻断原因::深度不足)
            | static_cast<std::int64_t>(枚举_观察帧清晰度阻断原因::空间不足))) == 0) {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::可用);
    } else if (输出.评分Q10000 > 0
        && (判定.平面轮廓稳定状态 || 判定.深度锚定状态 || 判定.空间可用状态)) {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::局部可用);
    } else {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::低清晰);
    }
    return 输出;
}

// 功能：先复用观察材料质量收束，再计算 D9 F 层观察帧清晰度评分。
结构_观察帧清晰度评分 计算观察帧清晰度评分(
    const 结构_外设观察报告队列项& 报告项,
    const 结构_外设观察等待项* 等待项)
{
    const auto 判定 = 判断外设观察材料质量(报告项, 等待项);
    return 计算观察帧清晰度评分(报告项, 判定, 等待项);
}

// 功能：按 D9 C 层输入材料收束观察候选清晰度评分，不写世界树事实或日志状态。
结构_观察候选清晰度评分 计算观察候选清晰度评分(
    const 结构_外设观察像素簇摘要& 簇)
{
    auto 限制Q10000 = [](std::int64_t 值) noexcept {
        return (std::min<std::int64_t>)(10000, (std::max<std::int64_t>)(0, 值));
    };
    auto 追加阻断 = [](std::int64_t& 掩码, 枚举_观察候选清晰度阻断原因 原因) noexcept {
        掩码 |= static_cast<std::int64_t>(原因);
    };

    结构_观察候选清晰度评分 输出{};
    输出.外设内部簇ID = 簇.外设内部簇ID;
    输出.来源空间候选ID = 簇.来源空间候选ID;
    输出.像素数量 = (std::max<std::int64_t>)(0, 簇.像素数量);
    输出.深度有效率Q10000 = 限制Q10000(簇.深度有效率);
    输出.空间坐标有效率Q10000 = 限制Q10000(簇.空间坐标有效率);
    输出.空间连续性评分Q10000 = 限制Q10000(簇.空间连续性评分);
    输出.范围稳定性评分Q10000 = 限制Q10000(簇.范围稳定性评分);
    输出.轮廓置信度Q10000 = 限制Q10000(簇.轮廓置信度);
    输出.稳定锚点评分Q10000 = 限制Q10000((std::max<std::int64_t>)(
        簇.稳定锚点密度,
        簇.稳定锚点覆盖率));

    const bool 有像素归属材料 = !簇.像素归属验证材料句柄.empty();
    const bool 有局部图材料 =
        !簇.像素集合掩码句柄.empty()
        || !簇.彩色轮廓局部图句柄.empty()
        || !簇.深度轮廓局部图句柄.empty()
        || !簇.ROI句柄.empty()
        || !簇.点集句柄.empty()
        || !簇.空间候选句柄.empty();
    输出.像素归属材料评分Q10000 = 有像素归属材料 ? 10000 : 0;

    if (簇.外设内部簇ID <= 0 && 簇.来源空间候选ID <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::缺候选);
    }
    if (输出.像素数量 <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::缺像素);
    }
    if (输出.深度有效率Q10000 <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::深度不足);
    }
    if (输出.空间坐标有效率Q10000 <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::空间不足);
    }
    if (输出.空间连续性评分Q10000 <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::空间连续性不足);
    }
    if (输出.范围稳定性评分Q10000 <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::范围稳定性不足);
    }
    if (输出.轮廓置信度Q10000 <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::轮廓置信度不足);
    }
    if (簇.稳定锚点数量 <= 0 && 输出.稳定锚点评分Q10000 <= 0) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::缺稳定锚点);
    }
    if (!有像素归属材料) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::缺像素归属材料);
    }
    if (!有局部图材料) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::缺局部图材料);
    }
    if (!簇.可提交观察区域) {
        追加阻断(输出.阻断原因掩码, 枚举_观察候选清晰度阻断原因::候选不可提交);
    }

    const auto 基础平均 =
        (输出.深度有效率Q10000
            + 输出.空间坐标有效率Q10000
            + 输出.空间连续性评分Q10000
            + 输出.范围稳定性评分Q10000
            + 输出.轮廓置信度Q10000
            + 输出.稳定锚点评分Q10000
            + 输出.像素归属材料评分Q10000) / 7;
    const auto 缺像素扣分 = 输出.像素数量 <= 0 ? 3000 : 0;
    const auto 缺局部图扣分 = 有局部图材料 ? 0 : 1000;
    输出.评分Q10000 = 限制Q10000(基础平均 - 缺像素扣分 - 缺局部图扣分);

    const auto 不可用掩码 =
        static_cast<std::int64_t>(枚举_观察候选清晰度阻断原因::缺候选)
        | static_cast<std::int64_t>(枚举_观察候选清晰度阻断原因::缺像素)
        | static_cast<std::int64_t>(枚举_观察候选清晰度阻断原因::候选不可提交);
    const auto 核心材料不足掩码 =
        static_cast<std::int64_t>(枚举_观察候选清晰度阻断原因::深度不足)
        | static_cast<std::int64_t>(枚举_观察候选清晰度阻断原因::空间不足)
        | static_cast<std::int64_t>(枚举_观察候选清晰度阻断原因::缺像素归属材料);
    if ((输出.阻断原因掩码 & 不可用掩码) != 0 || 输出.评分Q10000 <= 0) {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::不可用);
    } else if (输出.评分Q10000 >= 8000 && 输出.阻断原因掩码 == 0) {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::高清晰);
    } else if (输出.评分Q10000 >= 5000 && (输出.阻断原因掩码 & 核心材料不足掩码) == 0) {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::可用);
    } else if (输出.评分Q10000 > 0 && 有局部图材料) {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::局部可用);
    } else {
        输出.状态值 = static_cast<std::int64_t>(枚举_视觉清晰度等级状态::低清晰);
    }
    return 输出;
}

// 功能：把外设提交缺口码映射为 D5 结构化缺口大类 I64 值域。
std::int64_t 外设提交缺口_结构化缺口大类(枚举_外设提交缺口码 缺口) noexcept
{
    switch (缺口) {
    case 枚举_外设提交缺口码::无:
        return 结构化缺口大类_未定义;
    case 枚举_外设提交缺口码::缺像素归属账本:
        return 结构化缺口大类_像素归属缺口;
    case 枚举_外设提交缺口码::低质量:
    case 枚举_外设提交缺口码::深度无效:
        return 结构化缺口大类_质量收束缺口;
    case 枚举_外设提交缺口码::空间坐标无效:
        return 结构化缺口大类_坐标基准缺口;
    case 枚举_外设提交缺口码::缺可比历史:
        return 结构化缺口大类_观察域材料缺口;
    case 枚举_外设提交缺口码::缺已确认存在ID:
    case 枚举_外设提交缺口码::缺目标种子:
        return 结构化缺口大类_识别归属缺口;
    default:
        return 结构化缺口大类_外设材料缺口;
    }
}

// 功能：把外设提交缺口码映射为 D5 结构化阻断阶段 I64 值域。
std::int64_t 外设提交缺口_结构化阻断阶段(枚举_外设提交缺口码 缺口) noexcept
{
    switch (缺口) {
    case 枚举_外设提交缺口码::无:
        return 结构化阻断阶段_未定义;
    case 枚举_外设提交缺口码::缺像素归属账本:
        return 结构化阻断阶段_像素归属;
    case 枚举_外设提交缺口码::低质量:
    case 枚举_外设提交缺口码::深度无效:
        return 结构化阻断阶段_材料质量收束;
    case 枚举_外设提交缺口码::空间坐标无效:
        return 结构化阻断阶段_坐标基准收束;
    case 枚举_外设提交缺口码::缺可比历史:
        return 结构化阻断阶段_扫描比较;
    case 枚举_外设提交缺口码::缺已确认存在ID:
    case 枚举_外设提交缺口码::缺目标种子:
        return 结构化阻断阶段_识别归属;
    default:
        return 结构化阻断阶段_材料取得;
    }
}

// 功能：把外设提交缺口码映射为 D5 需求化处理建议 I64 值域，不直接创建需求。
std::int64_t 外设提交缺口_需求化处理建议(枚举_外设提交缺口码 缺口) noexcept
{
    switch (缺口) {
    case 枚举_外设提交缺口码::无:
        return 需求化建议_未评估;
    case 枚举_外设提交缺口码::缺报告:
    case 枚举_外设提交缺口码::外设采集失败:
    case 枚举_外设提交缺口码::报告类型不匹配:
    case 枚举_外设提交缺口码::低质量:
    case 枚举_外设提交缺口码::深度无效:
        return 需求化建议_只记录过程证据;
    case 枚举_外设提交缺口码::缺原始观察帧:
    case 枚举_外设提交缺口码::缺深度帧:
    case 枚举_外设提交缺口码::缺彩色帧:
    case 枚举_外设提交缺口码::缺对齐帧:
    case 枚举_外设提交缺口码::缺空间候选:
    case 枚举_外设提交缺口码::缺材料句柄:
    case 枚举_外设提交缺口码::材料过期:
        return 需求化建议_转外设材料闭环;
    case 枚举_外设提交缺口码::缺像素归属账本:
    case 枚举_外设提交缺口码::空间坐标无效:
    case 枚举_外设提交缺口码::缺已确认存在ID:
    case 枚举_外设提交缺口码::缺目标种子:
        return 需求化建议_转观察验证闭环;
    case 枚举_外设提交缺口码::缺可比历史:
        return 需求化建议_等待复验;
    default:
        return 需求化建议_未评估;
    }
}

// 功能：把外设观察先验发布缺口映射为 D5 结构化缺口大类 I64 值域。
std::int64_t 外设观察先验发布缺口_结构化缺口大类(枚举_外设观察先验发布缺口 缺口) noexcept
{
    switch (缺口) {
    case 枚举_外设观察先验发布缺口::无:
        return 结构化缺口大类_未定义;
    case 枚举_外设观察先验发布缺口::先验坐标系版本缺失:
    case 枚举_外设观察先验发布缺口::外设位姿缺失:
    case 枚举_外设观察先验发布缺口::投影材料不可生成:
        return 结构化缺口大类_坐标基准缺口;
    case 枚举_外设观察先验发布缺口::目标观察约束过期:
        return 结构化缺口大类_观察域材料缺口;
    case 枚举_外设观察先验发布缺口::缺场景体素先验快照:
    case 枚举_外设观察先验发布缺口::快照过期:
    case 枚举_外设观察先验发布缺口::先验图层缺失:
    case 枚举_外设观察先验发布缺口::降级策略触发:
        return 结构化缺口大类_场景体素先验缺口;
    default:
        return 结构化缺口大类_未定义;
    }
}

// 功能：把外设观察先验发布缺口映射为 D5 结构化阻断阶段 I64 值域。
std::int64_t 外设观察先验发布缺口_结构化阻断阶段(枚举_外设观察先验发布缺口 缺口) noexcept
{
    switch (缺口) {
    case 枚举_外设观察先验发布缺口::无:
        return 结构化阻断阶段_未定义;
    case 枚举_外设观察先验发布缺口::先验坐标系版本缺失:
    case 枚举_外设观察先验发布缺口::外设位姿缺失:
    case 枚举_外设观察先验发布缺口::投影材料不可生成:
        return 结构化阻断阶段_坐标基准收束;
    case 枚举_外设观察先验发布缺口::目标观察约束过期:
        return 结构化阻断阶段_观察提交;
    case 枚举_外设观察先验发布缺口::缺场景体素先验快照:
    case 枚举_外设观察先验发布缺口::快照过期:
    case 枚举_外设观察先验发布缺口::先验图层缺失:
    case 枚举_外设观察先验发布缺口::降级策略触发:
        return 结构化阻断阶段_先验融合;
    default:
        return 结构化阻断阶段_未定义;
    }
}

// 功能：把外设观察先验发布缺口映射为 D5 需求化处理建议 I64 值域，不直接创建需求。
std::int64_t 外设观察先验发布缺口_需求化处理建议(枚举_外设观察先验发布缺口 缺口) noexcept
{
    switch (缺口) {
    case 枚举_外设观察先验发布缺口::无:
        return 需求化建议_未评估;
    case 枚举_外设观察先验发布缺口::降级策略触发:
        return 需求化建议_只记录过程证据;
    case 枚举_外设观察先验发布缺口::缺场景体素先验快照:
    case 枚举_外设观察先验发布缺口::快照过期:
    case 枚举_外设观察先验发布缺口::目标观察约束过期:
    case 枚举_外设观察先验发布缺口::先验图层缺失:
        return 需求化建议_等待复验;
    case 枚举_外设观察先验发布缺口::先验坐标系版本缺失:
    case 枚举_外设观察先验发布缺口::外设位姿缺失:
    case 枚举_外设观察先验发布缺口::投影材料不可生成:
        return 需求化建议_转观察验证闭环;
    default:
        return 需求化建议_未评估;
    }
}

// 功能：把目标观察约束消费端可用性标准化为外设先验发布缺口。
枚举_外设观察先验发布缺口 目标观察约束可用性_外设先验发布缺口(
    枚举_目标观察约束可用性 状态) noexcept
{
    switch (状态) {
    case 枚举_目标观察约束可用性::可用:
        return 枚举_外设观察先验发布缺口::无;
    case 枚举_目标观察约束可用性::可降级使用:
        return 枚举_外设观察先验发布缺口::降级策略触发;
    case 枚举_目标观察约束可用性::已过期:
    case 枚举_目标观察约束可用性::目标已替代:
    case 枚举_目标观察约束可用性::已撤销:
        return 枚举_外设观察先验发布缺口::目标观察约束过期;
    case 枚举_目标观察约束可用性::缺定位材料:
        return 枚举_外设观察先验发布缺口::外设位姿缺失;
    case 枚举_目标观察约束可用性::材料不可回查:
    case 枚举_目标观察约束可用性::缺目标存在:
    case 枚举_目标观察约束可用性::缺目标特征:
    case 枚举_目标观察约束可用性::缺当前值:
        return 枚举_外设观察先验发布缺口::投影材料不可生成;
    case 枚举_目标观察约束可用性::未定义:
    default:
        return 枚举_外设观察先验发布缺口::缺场景体素先验快照;
    }
}

// 功能：按既有质量收束状态和材料证据收集外设提交缺口码。
std::vector<枚举_外设提交缺口码> 收集观察材料质量缺口码(
    const 结构_外设观察报告队列项& 报告项,
    const 结构_观察材料质量判定& 判定,
    const 结构_外设观察等待项* 等待项)
{
    std::vector<枚举_外设提交缺口码> 缺口集合{};
    auto 追加缺口 = [&](枚举_外设提交缺口码 缺口) {
        外设提交_追加缺口(缺口集合, 缺口);
    };
    auto 检查句柄 = [&](const std::string& 句柄) {
        if (句柄.empty()) {
            return;
        }
        const auto 摘要 = 解析D455观察材料句柄(句柄);
        if (摘要.成功) {
            return;
        }
        if (摘要.状态 == 枚举_D455观察材料读取状态::已过期) {
            追加缺口(枚举_外设提交缺口码::材料过期);
        } else {
            追加缺口(枚举_外设提交缺口码::缺材料句柄);
        }
    };

    if (报告项.报告ID == 0
        && 报告项.报告类型 == 枚举_外设观察报告类型::未指定
        && !报告项.采集成功) {
        追加缺口(枚举_外设提交缺口码::缺报告);
        return 缺口集合;
    }

    if (报告项.报告类型 == 枚举_外设观察报告类型::未指定) {
        追加缺口(枚举_外设提交缺口码::缺报告);
    }
    if (!报告项.采集成功) {
        追加缺口(枚举_外设提交缺口码::外设采集失败);
    }

    const auto 当前时间 = 外设观察当前时间毫秒();
    if (等待项
        && 等待项->最大允许报告年龄毫秒 > 0
        && 报告项.时间戳毫秒 > 0
        && 当前时间 - 报告项.时间戳毫秒 > 等待项->最大允许报告年龄毫秒) {
        追加缺口(枚举_外设提交缺口码::材料过期);
    }
    if (等待项
        && 等待项->目标约束有效截止时间毫秒 > 0
        && 当前时间 > 等待项->目标约束有效截止时间毫秒) {
        追加缺口(枚举_外设提交缺口码::材料过期);
    }
    if (等待项
        && 等待项->期望报告类型 != 枚举_外设观察报告类型::未指定
        && 报告项.报告类型 != 枚举_外设观察报告类型::未指定
        && 报告项.报告类型 != 等待项->期望报告类型) {
        追加缺口(枚举_外设提交缺口码::报告类型不匹配);
    }
    if (报告项.坐标系一致性状态 == 枚举_外设坐标系一致性状态::不一致
        || 判定.坐标系一致性状态 == 枚举_外设坐标系一致性状态::不一致) {
        追加缺口(枚举_外设提交缺口码::空间坐标无效);
    }
    if (报告项.目标约束使用状态 >= 3) {
        追加缺口(枚举_外设提交缺口码::低质量);
    }

    if (报告项.原始观察帧句柄.empty()) {
        追加缺口(枚举_外设提交缺口码::缺原始观察帧);
    } else {
        检查句柄(报告项.原始观察帧句柄);
    }
    if (报告项.深度帧句柄.empty()) {
        追加缺口(枚举_外设提交缺口码::缺深度帧);
    } else {
        检查句柄(报告项.深度帧句柄);
    }
    if (报告项.彩色帧句柄.empty()) {
        追加缺口(枚举_外设提交缺口码::缺彩色帧);
    } else {
        检查句柄(报告项.彩色帧句柄);
    }
    if (报告项.对齐帧句柄.empty()) {
        追加缺口(枚举_外设提交缺口码::缺对齐帧);
    } else {
        检查句柄(报告项.对齐帧句柄);
    }
    if (报告项.空间候选队列句柄.empty()) {
        追加缺口(枚举_外设提交缺口码::缺空间候选);
    } else {
        检查句柄(报告项.空间候选队列句柄);
    }
    if (报告项.像素归属账本句柄.empty()) {
        追加缺口(枚举_外设提交缺口码::缺像素归属账本);
    } else {
        检查句柄(报告项.像素归属账本句柄);
    }

    const bool 需要簇材料 =
        报告项.报告类型 == 枚举_外设观察报告类型::逐簇识别报告
        || 报告项.报告类型 == 枚举_外设观察报告类型::扫描变化报告
        || 报告项.报告类型 == 枚举_外设观察报告类型::跟踪报告;
    if (需要簇材料 && 报告项.观察像素簇集合.empty()) {
        追加缺口(枚举_外设提交缺口码::缺材料句柄);
    }
    for (const auto& 簇 : 报告项.观察像素簇集合) {
        const auto 簇材料句柄集 = 外设提交_簇材料句柄集(簇);
        if (簇材料句柄集.empty()) {
            追加缺口(枚举_外设提交缺口码::缺材料句柄);
        }
        for (const auto& 句柄 : 簇材料句柄集) {
            检查句柄(句柄);
        }
        if (!簇.可提交观察区域) {
            追加缺口(枚举_外设提交缺口码::低质量);
        }
    }

    if (报告项.帧质量评分 <= 0
        || 报告项.低质量区域数量 > 0
        || 判定.等级 == 枚举_观察材料等级::不合格
        || 判定.可承接能力等级 == 枚举_外设材料能力等级::候选或缺口) {
        追加缺口(枚举_外设提交缺口码::低质量);
    }
    if (报告项.深度有效率 <= 0) {
        追加缺口(枚举_外设提交缺口码::深度无效);
    }
    if (报告项.空间坐标有效率 <= 0) {
        追加缺口(枚举_外设提交缺口码::空间坐标无效);
    }

    const auto 质量状态 = 计算观察材料质量收束状态(报告项, 判定, 等待项);
    if (质量状态 == 外设质量状态_低质量) {
        追加缺口(枚举_外设提交缺口码::低质量);
    } else if (质量状态 == 外设质量状态_不可用 && 缺口集合.empty()) {
        追加缺口(枚举_外设提交缺口码::缺材料句柄);
    } else if (质量状态 == 外设质量状态_部分可用) {
        if (!判定.深度锚定状态 && 报告项.深度有效率 <= 0) {
            追加缺口(枚举_外设提交缺口码::深度无效);
        }
        if (!判定.空间可用状态 && 报告项.空间坐标有效率 <= 0) {
            追加缺口(枚举_外设提交缺口码::空间坐标无效);
        }
    } else if (质量状态 == 外设质量状态_需复验 && 缺口集合.empty()) {
        追加缺口(枚举_外设提交缺口码::材料过期);
    }

    return 缺口集合;
}

// 功能：按函数名执行对应处理。
std::string 构造观察材料质量判定摘要(const 结构_观察材料质量判定& 判定)
{
    std::ostringstream 输出;
    输出 << "观察材料质量判定"
        << " | 等级=" << 观察材料等级文本(判定.等级)
        << " | 可进入验证方法=" << (判定.可进入验证方法 ? 1 : 0)
        << " | 可进入识别方法=" << (判定.可进入识别方法 ? 1 : 0)
        << " | 可生成扫描变化候选=" << (判定.可生成扫描变化候选 ? 1 : 0)
        << " | 可生成跟踪动态候选=" << (判定.可生成跟踪动态候选 ? 1 : 0)
        << " | 可交付观察方法材料=" << (判定.可交付观察方法材料 ? 1 : 0)
        << " | 外设材料可承接状态=" << (判定.外设材料可承接状态 ? 1 : 0)
        << " | 可承接能力等级=" << 外设材料能力等级文本(判定.可承接能力等级)
        << " | 坐标系一致性状态=" << 外设坐标系一致性状态文本(判定.坐标系一致性状态)
        << " | 稳定锚点数量=" << 判定.稳定锚点数量
        << " | 稳定锚点密度=" << 判定.稳定锚点密度
        << " | 稳定锚点覆盖率=" << 判定.稳定锚点覆盖率
        << " | 边界锚点覆盖率=" << 判定.边界锚点覆盖率
        << " | 平面轮廓稳定状态=" << (判定.平面轮廓稳定状态 ? 1 : 0)
        << " | 深度锚定状态=" << (判定.深度锚定状态 ? 1 : 0)
        << " | 空间可用状态=" << (判定.空间可用状态 ? 1 : 0)
        << " | 目标区域优质=" << (判定.目标区域优质 ? 1 : 0);
    return 输出.str();
}

// 功能：提交事实、动态、任务状态或运行回执。
枚举_外设提交包类型 外设提交包类型_由报告类型(枚举_外设观察报告类型 类型) noexcept
{
    switch (类型) {
    case 枚举_外设观察报告类型::逐簇识别报告:
        return 枚举_外设提交包类型::识别;
    case 枚举_外设观察报告类型::扫描变化报告:
        return 枚举_外设提交包类型::扫描;
    case 枚举_外设观察报告类型::跟踪报告:
        return 枚举_外设提交包类型::跟踪;
    case 枚举_外设观察报告类型::未指定:
    default:
        return 枚举_外设提交包类型::未指定;
    }
}

// 功能：提交事实、动态、任务状态或运行回执。
结构_外设提交包头 构造外设提交包头_由观察报告(const 结构_外设观察报告队列项& 报告项)
{
    结构_外设提交包头 包头{};
    包头.报告ID = 报告项.报告ID;
    包头.帧ID = 外设提交_帧ID(报告项);
    包头.包类型 = 外设提交包类型_由报告类型(报告项.报告类型);
    包头.包ID = 报告项.报告ID == 0
        ? 0
        : 报告项.报告ID * 10 + static_cast<std::uint64_t>(包头.包类型);
    包头.触发来源 = 报告项.匹配等待项ID == 0
        ? 枚举_外设触发来源::默认维护
        : 枚举_外设触发来源::需求等待项;
    if (包头.包类型 == 枚举_外设提交包类型::跟踪) {
        包头.触发来源 = 枚举_外设触发来源::指定目标跟踪;
    }
    包头.采集时间毫秒 = 报告项.时间戳毫秒;
    包头.提交时间毫秒 = 外设观察当前时间毫秒();
    包头.外设ID = 报告项.来源外设ID;
    包头.材料页ID = 外设提交_材料页ID(报告项);
    包头.坐标系ID = 报告项.坐标系ID;
    包头.有效截止时间毫秒 = 报告项.时间戳毫秒 > 0
        ? 报告项.时间戳毫秒 + 外设提交包默认有效毫秒
        : 0;
    外设提交_汇总报告材料(包头, 报告项);
    外设提交_填充报告缺口(包头, 报告项);
    return 包头;
}

// 功能：提交事实、动态、任务状态或运行回执。
std::optional<结构_外设识别提交包> 构造外设识别提交包_由观察报告(
    const 结构_外设观察报告队列项& 报告项)
{
    if (报告项.报告类型 != 枚举_外设观察报告类型::逐簇识别报告) {
        return std::nullopt;
    }

    结构_外设识别提交包 包{};
    包.包头 = 构造外设提交包头_由观察报告(报告项);
    const auto 判定 = 判断外设观察材料质量(报告项, nullptr);
    const auto 质量状态 = 计算观察材料质量收束状态(报告项, 判定, nullptr);
    const auto 质量缺口集合 = 收集观察材料质量缺口码(报告项, 判定, nullptr);
    const bool 材料允许识别承接 = 外设提交_质量状态允许承接(质量状态, 判定.可承接能力等级);
    外设提交_合并缺口集合(包.证据不足原因, 质量缺口集合);
    包.像素归属账本句柄 = 报告项.像素归属账本句柄;
    std::uint64_t 序号 = 0;
    for (const auto& 簇 : 报告项.观察像素簇集合) {
        ++序号;
        const auto 区域ID = 外设提交_正局部ID(簇.外设内部簇ID, 序号);
        const bool 有效 = 簇.可提交观察区域 && 材料允许识别承接;
        if (有效) {
            包.有效观察区域.push_back(区域ID);
        } else {
            包.无效区域.push_back(区域ID);
        }

        外设提交_追加可回查句柄(包.ROI句柄集, 簇.ROI句柄);
        外设提交_追加可回查句柄(包.掩码句柄集, 簇.像素集合掩码句柄);
        外设提交_追加可回查句柄(包.点集句柄集, 簇.点集句柄);
        外设提交_追加可回查句柄(包.深度切片句柄集, 簇.深度轮廓局部图句柄);

        结构_外设识别候选 候选{};
        候选.候选ID = 区域ID;
        候选.来源区域ID = 区域ID;
        候选.归属状态 = 有效
            ? 枚举_外设区域归属状态::候选
            : 枚举_外设区域归属状态::无效;
        候选.匹配置信度 = 外设提交_百万分值(簇.轮廓置信度);
        候选.材料句柄集 = 外设提交_可回查簇材料句柄集(簇);
        if (候选.材料句柄集.empty()) {
            外设提交_追加缺口(候选.证据不足原因, 枚举_外设提交缺口码::缺材料句柄);
        }
        if (!材料允许识别承接) {
            外设提交_合并缺口集合(候选.证据不足原因, 质量缺口集合);
        }
        外设提交_追加簇基础特征(
            候选.候选特征值,
            报告项,
            簇,
            区域ID,
            枚举_外设特征宿主类型::候选,
            false);
        包.候选特征值.insert(
            包.候选特征值.end(),
            候选.候选特征值.begin(),
            候选.候选特征值.end());
        if (有效) {
            包.新存在候选.push_back(std::move(候选));
        } else {
            外设提交_追加缺口(包.证据不足原因, 枚举_外设提交缺口码::低质量);
        }
    }
    if (报告项.未归簇区域数量 > 0 || 报告项.不可观测区域数量 > 0) {
        外设提交_追加缺口(包.证据不足原因, 枚举_外设提交缺口码::缺材料句柄);
    }
    return 包;
}

// 功能：提交事实、动态、任务状态或运行回执。
std::optional<结构_外设扫描提交包> 构造外设扫描提交包_由观察报告(
    const 结构_外设观察报告队列项& 报告项)
{
    if (报告项.报告类型 != 枚举_外设观察报告类型::扫描变化报告) {
        return std::nullopt;
    }

    结构_外设扫描提交包 包{};
    包.包头 = 构造外设提交包头_由观察报告(报告项);
    const auto 判定 = 判断外设观察材料质量(报告项, nullptr);
    const auto 质量状态 = 计算观察材料质量收束状态(报告项, 判定, nullptr);
    const auto 质量缺口集合 = 收集观察材料质量缺口码(报告项, 判定, nullptr);
    const bool 材料允许扫描承接 = 外设提交_质量状态允许承接(质量状态, 判定.可承接能力等级);
    外设提交_合并缺口集合(包.缺口原因, 质量缺口集合);
    包.当前帧ID = 包.包头.帧ID;
    const bool 缺少逐簇识别基准 =
        报告项.扫描变化基准状态 == 枚举_扫描变化基准状态::缺少逐簇识别基准;
    包.上一帧ID = 缺少逐簇识别基准 || 包.当前帧ID == 0 ? 0 : 包.当前帧ID - 1;
    包.扫描覆盖率 = 报告项.预期像素数量 > 0
        ? static_cast<double>(报告项.像素特征数量) / static_cast<double>(报告项.预期像素数量)
        : 0.0;
    包.像素归属闭合率 = 报告项.预期像素数量 > 0
        ? static_cast<double>((std::max<std::int64_t>)(0, 报告项.预期像素数量 - 报告项.未归簇区域数量))
            / static_cast<double>(报告项.预期像素数量)
        : 0.0;
    包.深度有效率 = 外设提交_百万分值(报告项.深度有效率);
    包.空间坐标有效率 = 外设提交_百万分值(报告项.空间坐标有效率);

    const auto 报告宿主ID = 报告项.报告ID;
    const std::vector<std::string> 报告材料句柄集 = 包.包头.材料句柄集;
    包.特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::新增观察像素簇数量,
        报告项.新增观察像素簇数量,
        报告宿主ID,
        枚举_外设特征宿主类型::区域,
        报告项.坐标系ID,
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料句柄集,
        true,
        "外设扫描报告计数比较器"));
    包.特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::消失观察像素簇数量,
        报告项.消失观察像素簇数量,
        报告宿主ID,
        枚举_外设特征宿主类型::区域,
        报告项.坐标系ID,
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料句柄集,
        true,
        "外设扫描报告计数比较器"));
    包.特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::变化观察像素簇数量,
        报告项.变化观察像素簇数量,
        报告宿主ID,
        枚举_外设特征宿主类型::区域,
        报告项.坐标系ID,
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料句柄集,
        true,
        "外设扫描报告计数比较器"));

    std::uint64_t 序号 = 0;
    for (const auto& 簇 : 报告项.观察像素簇集合) {
        ++序号;
        const auto 区域ID = 外设提交_正局部ID(簇.外设内部簇ID, 序号);
        const auto 匹配状态 = 外设提交_扫描匹配状态_由簇(簇);
        const auto 输出匹配状态 = 材料允许扫描承接
            ? 匹配状态
            : 枚举_扫描匹配状态::需重识别;
        if (输出匹配状态 == 枚举_扫描匹配状态::需重识别) {
            包.需重识别区域.push_back(区域ID);
        } else {
            包.已识别区域.push_back(区域ID);
        }

        外设提交_追加簇基础特征(
            包.当前特征值,
            报告项,
            簇,
            区域ID,
            枚举_外设特征宿主类型::区域,
            true);
        包.特征变化值.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::跨帧匹配状态,
            簇.跨帧匹配状态,
            区域ID,
            枚举_外设特征宿主类型::区域,
            报告项.坐标系ID,
            报告项.时间戳毫秒,
            外设提交_百万分值(报告项.报告置信度),
            外设提交_百万分值(报告项.帧质量评分),
            外设提交_簇材料句柄集(簇),
            true,
            "外设同源跨帧I64比较器"));

        结构_外设已知存在扫描结果 结果{};
        结果.当前ROI句柄 = 外设提交_材料句柄可回查(簇.ROI句柄) ? 簇.ROI句柄 : std::string{};
        结果.当前掩码句柄 = 外设提交_材料句柄可回查(簇.像素集合掩码句柄) ? 簇.像素集合掩码句柄 : std::string{};
        结果.当前点集句柄 = 外设提交_材料句柄可回查(簇.点集句柄) ? 簇.点集句柄 : std::string{};
        结果.当前深度切片句柄 = 外设提交_材料句柄可回查(簇.深度轮廓局部图句柄) ? 簇.深度轮廓局部图句柄 : std::string{};
        结果.当前中心 = 外设提交_簇中心(簇);
        结果.当前空间范围 = 外设提交_簇范围(簇);
        结果.匹配状态 = 输出匹配状态;
        结果.匹配置信度 = 外设提交_百万分值(簇.轮廓置信度);
        结果.候选像素数量 = (std::max<std::int64_t>)(0, 簇.像素数量);
        const auto 候选深度有效率 = (std::min<std::int64_t>)(
            10000,
            (std::max<std::int64_t>)(0, 簇.深度有效率));
        const auto 候选空间有效率 = (std::min<std::int64_t>)(
            10000,
            (std::max<std::int64_t>)(0, 簇.空间坐标有效率));
        const auto 候选轮廓置信度 = (std::min<std::int64_t>)(
            10000,
            (std::max<std::int64_t>)(0, 簇.轮廓置信度));
        结果.候选有效深度点数量 = (结果.候选像素数量 * 候选深度有效率) / 10000;
        结果.候选深度缺失像素数量 = (std::max<std::int64_t>)(
            0,
            结果.候选像素数量 - 结果.候选有效深度点数量);
        结果.候选低质量像素数量 = 簇.可提交观察区域 ? 0 : 结果.候选像素数量;
        结果.候选遮挡边界像素数量 =
            簇.轨迹状态 == 枚举_外设Tracker轨迹状态::遮挡 ? 结果.候选像素数量 : 0;
        结果.候选像素归属质量评分Q10000 =
            (候选深度有效率 + 候选空间有效率 + 候选轮廓置信度) / 3;
        if (!材料允许扫描承接) {
            外设提交_合并缺口集合(结果.原因, 质量缺口集合);
        }
        外设提交_追加缺口(结果.原因, 枚举_外设提交缺口码::缺已确认存在ID);
        包.已知存在结果.push_back(std::move(结果));
    }

    if (包.已知存在结果.empty()) {
        外设提交_追加缺口(包.缺口原因, 枚举_外设提交缺口码::缺已确认存在ID);
    }
    if (报告项.新增观察像素簇数量 > 0 || 报告项.高风险未知区域数量 > 0) {
        外设提交_追加缺口(包.缺口原因, 枚举_外设提交缺口码::缺已确认存在ID);
    }
    return 包;
}

// 功能：提交事实、动态、任务状态或运行回执。
std::optional<结构_外设跟踪提交包> 构造外设跟踪提交包_由观察报告(
    const 结构_外设观察报告队列项& 报告项)
{
    if (报告项.报告类型 != 枚举_外设观察报告类型::跟踪报告) {
        return std::nullopt;
    }

    结构_外设跟踪提交包 包{};
    包.包头 = 构造外设提交包头_由观察报告(报告项);
    const auto 判定 = 判断外设观察材料质量(报告项, nullptr);
    const auto 质量状态 = 计算观察材料质量收束状态(报告项, 判定, nullptr);
    const auto 质量缺口集合 = 收集观察材料质量缺口码(报告项, 判定, nullptr);
    const bool 材料允许跟踪承接 = 外设提交_质量状态允许承接(质量状态, 判定.可承接能力等级);
    外设提交_合并缺口集合(包.证据不足原因, 质量缺口集合);
    包.目标种子ID = 报告项.跟踪ID;
    包.跟踪窗口ID = 报告项.匹配等待项ID;
    包.当前帧ID = 包.包头.帧ID;
    包.上一帧ID = 包.当前帧ID > 0 ? 包.当前帧ID - 1 : 0;
    包.跟踪状态 = 外设提交_跟踪状态_由报告(报告项);
    if (!材料允许跟踪承接) {
        包.跟踪状态 = 枚举_跟踪状态::证据不足;
    }
    包.跟踪置信度 = 外设提交_百万分值(报告项.报告置信度);
    if (包.目标种子ID == 0
        && 报告项.目标观察约束ID == 0
        && 报告项.匹配等待项ID == 0) {
        外设提交_追加缺口(包.证据不足原因, 枚举_外设提交缺口码::缺目标种子);
    }
    外设提交_追加缺口(包.证据不足原因, 枚举_外设提交缺口码::缺已确认存在ID);

    if (!报告项.观察像素簇集合.empty()) {
        const auto& 簇 = 报告项.观察像素簇集合.front();
        const auto 目标局部ID = 外设提交_正局部ID(
            簇.外设内部簇ID,
            包.目标种子ID == 0 ? 1 : 包.目标种子ID);
        包.当前ROI句柄 = 外设提交_材料句柄可回查(簇.ROI句柄) ? 簇.ROI句柄 : std::string{};
        包.当前掩码句柄 = 外设提交_材料句柄可回查(簇.像素集合掩码句柄) ? 簇.像素集合掩码句柄 : std::string{};
        包.当前点集句柄 = 外设提交_材料句柄可回查(簇.点集句柄) ? 簇.点集句柄 : std::string{};
        包.当前深度切片句柄 = 外设提交_材料句柄可回查(簇.深度轮廓局部图句柄) ? 簇.深度轮廓局部图句柄 : std::string{};
        包.当前中心 = 外设提交_簇中心(簇);
        包.当前空间范围 = 外设提交_簇范围(簇);
        外设提交_追加簇基础特征(
            包.目标特征值,
            报告项,
            簇,
            目标局部ID,
            枚举_外设特征宿主类型::目标,
            true);
    } else {
        外设提交_追加缺口(包.证据不足原因, 枚举_外设提交缺口码::缺材料句柄);
    }

    const std::vector<std::string> 报告材料句柄集 = 包.包头.材料句柄集;
    const auto 目标宿主ID = 包.目标种子ID;
    包.目标特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::跟踪状态值,
        报告项.跟踪状态值,
        目标宿主ID,
        枚举_外设特征宿主类型::目标,
        报告项.坐标系ID,
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料句柄集,
        true,
        "外设目标跟踪I64比较器"));
    包.目标特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::丢失状态值,
        报告项.丢失状态值,
        目标宿主ID,
        枚举_外设特征宿主类型::目标,
        报告项.坐标系ID,
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料句柄集,
        true,
        "外设目标跟踪I64比较器"));
    包.目标特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::稳定特征样本数量,
        报告项.稳定特征样本数量,
        目标宿主ID,
        枚举_外设特征宿主类型::目标,
        报告项.坐标系ID,
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料句柄集,
        true,
        "外设目标跟踪I64比较器"));
    return 包;
}

// 功能：提交事实、动态、任务状态或运行回执。
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
    外设观察报告_清理已加锁(状态, 报告项.时间戳毫秒);
    D455_清理短期材料页_已加锁(状态, 报告项.时间戳毫秒);
    状态.报告队列.push_back(报告项);
    状态.最新报告ID = 报告项.报告ID;
    外设观察报告_清理已加锁(状态, 报告项.时间戳毫秒);
    D455_清理短期材料页_已加锁(状态, 报告项.时间戳毫秒);
    return 报告项.报告ID;
}

// 功能：提交事实、动态、任务状态或运行回执。
bool 提交D455短期观察材料页(
    std::uint64_t 报告ID,
    双目相机本能适配器::调用结果 来源,
    const 结构_外设观察报告队列项& 报告项)
{
    if (报告ID == 0 || !来源.成功) {
        return false;
    }
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    结构_D455短期观察材料页 页{};
    页.报告ID = 报告ID;
    页.创建时间毫秒 = 当前时间;
    页.过期时间毫秒 = 当前时间 + 状态.D455材料页保留毫秒;
    页.来源 = std::move(来源);
    页.报告 = 报告项;
    页.报告.报告ID = 报告ID;

    std::lock_guard<std::mutex> 锁(状态.互斥);
    for (auto& 已有 : 状态.D455材料页队列) {
        if (已有.报告ID == 报告ID) {
            已有 = std::move(页);
            D455_清理短期材料页_已加锁(状态, 当前时间);
            return true;
        }
    }
    状态.D455材料页队列.push_back(std::move(页));
    D455_清理短期材料页_已加锁(状态, 当前时间);
    return true;
}

// 功能：提交运行期控制面板视频帧。
bool 提交D455控制面板视频帧(
    双目相机本能适配器::彩色视频帧 来源)
{
    if (!D455_控制面板视频帧有效(来源)) {
        return false;
    }
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    结构_D455控制面板视频帧页 页{};
    页.创建时间毫秒 = 当前时间;
    页.过期时间毫秒 = 当前时间 + 状态.D455控制面板视频帧保留毫秒;
    页.来源 = std::move(来源);

    std::lock_guard<std::mutex> 锁(状态.互斥);
    页.视频帧ID = ++状态.最新D455控制面板视频帧ID;
    状态.D455控制面板视频帧队列.push_back(std::move(页));
    D455_清理控制面板视频帧_已加锁(状态, 当前时间);
    return true;
}

// 功能：解析输入文本、消息、场景或运行包。
结构_D455观察材料句柄摘要 解析D455观察材料句柄(const std::string& 句柄)
{
    结构_D455观察材料句柄摘要 摘要{};
    if (句柄.empty()) {
        摘要.状态 = 枚举_D455观察材料读取状态::空句柄;
        return 摘要;
    }
    if (!D455_句柄前缀有效(句柄)) {
        摘要.状态 = 枚举_D455观察材料读取状态::句柄格式错误;
        return 摘要;
    }

    const auto 资源类型 = D455_句柄资源类型(句柄);
    if (资源类型.empty()) {
        摘要.状态 = 枚举_D455观察材料读取状态::句柄格式错误;
        return 摘要;
    }

    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    D455_清理短期材料页_已加锁(状态, 当前时间);
    for (auto 迭代器 = 状态.D455材料页队列.rbegin();
        迭代器 != 状态.D455材料页队列.rend();
        ++迭代器) {
        if (迭代器->过期时间毫秒 > 0 && 当前时间 > 迭代器->过期时间毫秒) {
            if (D455_尝试匹配帧句柄(*迭代器, 句柄, 资源类型, &摘要)
                || D455_尝试匹配簇句柄(*迭代器, 句柄, 资源类型, &摘要)) {
                摘要.状态 = 枚举_D455观察材料读取状态::已过期;
                摘要.成功 = false;
                return 摘要;
            }
            continue;
        }
        if (D455_尝试匹配帧句柄(*迭代器, 句柄, 资源类型, &摘要)
            || D455_尝试匹配簇句柄(*迭代器, 句柄, 资源类型, &摘要)) {
            return 摘要;
        }
    }

    摘要.状态 = 枚举_D455观察材料读取状态::未找到;
    摘要.资源类型ID = 资源类型;
    return 摘要;
}

// 功能：按像素归属账本句柄只读回查像素观察归属图材料页。
结构_像素观察归属图材料页读取结果 读取像素观察归属图材料页(const std::string& 句柄)
{
    结构_像素观察归属图材料页读取结果 输出{};
    输出.资源类型ID = D455资源类型_像素观察归属图_第一版;
    auto 失败 = [&](枚举_D455观察材料读取状态 状态, std::string 原因) {
        输出.状态 = 状态;
        输出.成功 = false;
        输出.失败原因 = std::move(原因);
        return 输出;
    };

    if (句柄.empty()) {
        return 失败(枚举_D455观察材料读取状态::空句柄, "像素观察归属图句柄为空");
    }
    if (!D455_句柄前缀有效(句柄)) {
        return 失败(枚举_D455观察材料读取状态::句柄格式错误, "像素观察归属图句柄格式错误");
    }

    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    D455_清理短期材料页_已加锁(状态, 当前时间);
    for (auto 迭代器 = 状态.D455材料页队列.rbegin();
        迭代器 != 状态.D455材料页队列.rend();
        ++迭代器) {
        if (句柄 != 迭代器->报告.像素归属账本句柄) {
            continue;
        }
        if (迭代器->过期时间毫秒 > 0 && 当前时间 > 迭代器->过期时间毫秒) {
            auto 过期结果 = D455_构造像素观察归属图材料页(*迭代器);
            过期结果.状态 = 枚举_D455观察材料读取状态::已过期;
            过期结果.成功 = false;
            过期结果.失败原因 = "像素观察归属图材料页已过期";
            return 过期结果;
        }
        return D455_构造像素观察归属图材料页(*迭代器);
    }

    return 失败(枚举_D455观察材料读取状态::未找到, "未找到像素观察归属图材料页");
}

// 功能：从 D455 簇句柄对应短期材料页只读重建三维体素轮廓图视角。
结构_D455三维体素轮廓视角读取结果 读取D455簇三维体素轮廓视角(
    const 结构_外设观察像素簇摘要& 簇)
{
    结构_D455三维体素轮廓视角读取结果 输出{};

    auto 失败 = [&](std::string 原因) {
        输出.成功 = false;
        输出.失败原因 = std::move(原因);
        return 输出;
    };
    auto 材料读取状态文本 = [](枚举_D455观察材料读取状态 读取状态) noexcept -> const char* {
        switch (读取状态) {
        case 枚举_D455观察材料读取状态::成功: return "成功";
        case 枚举_D455观察材料读取状态::空句柄: return "空句柄";
        case 枚举_D455观察材料读取状态::句柄格式错误: return "句柄格式错误";
        case 枚举_D455观察材料读取状态::未找到: return "未找到";
        case 枚举_D455观察材料读取状态::已过期: return "已过期";
        case 枚举_D455观察材料读取状态::材料未就绪: return "材料未就绪";
        case 枚举_D455观察材料读取状态::材料类型不匹配: return "材料类型不匹配";
        case 枚举_D455观察材料读取状态::簇缺来源空间候选: return "簇缺来源空间候选";
        case 枚举_D455观察材料读取状态::簇投影非法: return "簇投影非法";
        case 枚举_D455观察材料读取状态::簇材料不可重建: return "簇材料不可重建";
        default: return "未知";
        }
    };
    auto 材料读取失败说明 = [&](const 结构_D455观察材料句柄摘要& 摘要) {
        std::ostringstream 输出说明;
        输出说明 << "D455簇材料读取失败"
            << " | 状态=" << 材料读取状态文本(摘要.状态);
        if (!摘要.资源类型ID.empty()) {
            输出说明 << " | 资源类型ID=" << 摘要.资源类型ID;
        }
        if (摘要.报告ID > 0) {
            输出说明 << " | 报告ID=" << 摘要.报告ID;
        }
        if (摘要.候选编号 >= 0) {
            输出说明 << " | 候选编号=" << 摘要.候选编号;
        }
        return 输出说明.str();
    };

    std::vector<std::string> 句柄集合{};
    auto 追加句柄 = [&](const std::string& 句柄) {
        if (!句柄.empty()) {
            句柄集合.push_back(句柄);
        }
    };
    追加句柄(簇.点集句柄);
    追加句柄(簇.深度轮廓局部图句柄);
    追加句柄(簇.像素集合掩码句柄);
    追加句柄(簇.空间候选句柄);
    追加句柄(簇.彩色轮廓局部图句柄);
    if (句柄集合.empty()) {
        return 失败("D455簇缺少可回查局部体素材料句柄");
    }

    auto 构造视角 = [&](const 结构_D455短期观察材料页& 页,
        const 双目相机本能适配器::空间候选摘要& 候选,
        const 结构_D455观察材料句柄摘要& 摘要) {
        if (!D455_帧像素数量有效(页.来源)) {
            return 失败("D455帧像素材料未就绪");
        }
        if (!D455_深度帧材料有效(页.来源)) {
            return 失败("D455深度材料未就绪");
        }
        if (!D455_空间坐标材料有效(页.来源)) {
            return 失败("D455空间点材料未就绪");
        }
        if (候选.像素索引集合.empty()) {
            return 失败("D455簇缺少像素索引集合");
        }
        if (候选.投影最大X < 候选.投影最小X || 候选.投影最大Y < 候选.投影最小Y) {
            return 失败("D455簇投影范围非法");
        }
        const auto 局部宽64 = static_cast<std::uint64_t>(候选.投影最大X - 候选.投影最小X + 1);
        const auto 局部高64 = static_cast<std::uint64_t>(候选.投影最大Y - 候选.投影最小Y + 1);
        if (局部宽64 == 0
            || 局部高64 == 0
            || 局部宽64 > static_cast<std::uint64_t>((std::numeric_limits<std::uint32_t>::max)())
            || 局部高64 > static_cast<std::uint64_t>((std::numeric_limits<std::uint32_t>::max)())
            || 局部宽64 > static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)()) / 局部高64) {
            return 失败("D455簇局部图尺寸超出平台限制");
        }

        const auto 局部像素数64 = 局部宽64 * 局部高64;
        if (局部像素数64 > static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)())) {
            return 失败("D455簇局部图像素数超出平台限制");
        }
        const auto 局部像素数 = static_cast<std::size_t>(局部像素数64);
        const auto 帧像素数 = static_cast<std::size_t>(页.来源.预期像素数量);

        const std::vector<std::int64_t>* 深度_mm = nullptr;
        if (页.来源.融合深度毫米.size() == 帧像素数) {
            深度_mm = &页.来源.融合深度毫米;
        } else if (页.来源.补全深度毫米.size() == 帧像素数) {
            深度_mm = &页.来源.补全深度毫米;
        } else if (页.来源.滤波深度毫米.size() == 帧像素数) {
            深度_mm = &页.来源.滤波深度毫米;
        } else if (页.来源.原始深度毫米.size() == 帧像素数) {
            深度_mm = &页.来源.原始深度毫米;
        }
        if (!深度_mm) {
            return 失败("D455深度数组尺寸与帧尺寸不一致");
        }

        输出 = {};
        输出.报告ID = 摘要.报告ID;
        输出.候选编号 = 摘要.候选编号;
        输出.视角.宽度 = static_cast<std::uint32_t>(局部宽64);
        输出.视角.高度 = static_cast<std::uint32_t>(局部高64);
        输出.视角.视角序号 = 0;
        输出.视角.轮廓掩码.assign(局部像素数, 0);
        输出.视角.深度_mm.assign(局部像素数, 0);
        输出.视角.空间点_mm.assign(局部像素数, {});
        const bool 有颜色 = D455_颜色帧材料有效(页.来源);
        if (有颜色) {
            输出.视角.颜色_RGBA.assign(局部像素数, 0);
        }

        const auto 帧宽 = static_cast<std::size_t>(页.来源.宽度);
        std::int64_t 有效像素数 = 0;
        for (const auto 像素索引U32 : 候选.像素索引集合) {
            const auto 像素索引 = static_cast<std::size_t>(像素索引U32);
            if (像素索引 >= 帧像素数 || 帧宽 == 0) {
                continue;
            }
            const auto 全局X = static_cast<std::int64_t>(像素索引 % 帧宽);
            const auto 全局Y = static_cast<std::int64_t>(像素索引 / 帧宽);
            if (全局X < 候选.投影最小X
                || 全局X > 候选.投影最大X
                || 全局Y < 候选.投影最小Y
                || 全局Y > 候选.投影最大Y) {
                continue;
            }

            const auto 深度值 = (*深度_mm)[像素索引];
            const auto& 点 = 页.来源.空间坐标毫米XYZ[像素索引];
            if (深度值 <= 0 || 点.Z <= 0) {
                continue;
            }

            const auto 局部X = static_cast<std::size_t>(全局X - 候选.投影最小X);
            const auto 局部Y = static_cast<std::size_t>(全局Y - 候选.投影最小Y);
            const auto 局部索引 = 局部Y * static_cast<std::size_t>(输出.视角.宽度) + 局部X;
            if (局部索引 >= 局部像素数) {
                continue;
            }
            if (输出.视角.轮廓掩码[局部索引] == 0) {
                ++有效像素数;
            }
            输出.视角.轮廓掩码[局部索引] = 1;
            输出.视角.深度_mm[局部索引] = 深度值;
            输出.视角.空间点_mm[局部索引] = 结构_三维体素空间点毫米{ 点.X, 点.Y, 点.Z };
            if (有颜色) {
                const auto& 颜色 = 页.来源.颜色RGB[像素索引];
                输出.视角.颜色_RGBA[局部索引] =
                    0xff000000u
                    | (static_cast<std::uint32_t>(颜色.R) << 16)
                    | (static_cast<std::uint32_t>(颜色.G) << 8)
                    | static_cast<std::uint32_t>(颜色.B);
            }
        }

        if (有效像素数 <= 0) {
            return 失败("D455簇局部图内没有有效深度空间点");
        }
        输出.成功 = true;
        输出.失败原因.clear();
        输出.有效像素数量 = 有效像素数;
        return 输出;
    };

    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    D455_清理短期材料页_已加锁(状态, 当前时间);

    std::string 最近失败说明{};
    for (auto 迭代器 = 状态.D455材料页队列.rbegin();
        迭代器 != 状态.D455材料页队列.rend();
        ++迭代器) {
        for (const auto& 句柄 : 句柄集合) {
            const auto 资源类型 = D455_句柄资源类型(句柄);
            if (资源类型.empty()) {
                continue;
            }
            结构_D455观察材料句柄摘要 摘要{};
            if (!D455_尝试匹配簇句柄(*迭代器, 句柄, 资源类型, &摘要)) {
                continue;
            }
            if (!摘要.成功) {
                最近失败说明 = 材料读取失败说明(摘要);
                continue;
            }
            const auto* 候选 = D455_查找材料页空间候选(*迭代器, 摘要.候选编号);
            if (!候选) {
                最近失败说明 = "D455簇句柄命中但未找到来源空间候选";
                continue;
            }
            return 构造视角(*迭代器, *候选, 摘要);
        }
    }

    return 失败(最近失败说明.empty()
        ? "未找到D455簇三维体素轮廓材料页"
        : 最近失败说明);
}

// 功能：从指定来源读取数据或状态。
结构_D455控制面板视频快照 读取最新D455控制面板视频快照()
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    D455_清理控制面板视频帧_已加锁(状态, 当前时间);
    D455_清理短期材料页_已加锁(状态, 当前时间);
    for (auto 迭代器 = 状态.D455材料页队列.rbegin();
        迭代器 != 状态.D455材料页队列.rend();
        ++迭代器) {
        if (!D455_颜色帧材料有效(迭代器->来源)) {
            continue;
        }
        return D455_构造控制面板视频快照_已加锁(*迭代器);
    }
    for (auto 迭代器 = 状态.D455控制面板视频帧队列.rbegin();
        迭代器 != 状态.D455控制面板视频帧队列.rend();
        ++迭代器) {
        if (!D455_控制面板视频帧有效(迭代器->来源)) {
            continue;
        }
        return D455_构造控制面板视频快照_由视频帧_已加锁(*迭代器);
    }

    结构_D455控制面板视频快照 输出{};
    输出.失败原因 = "未找到D455外设线程短期视频材料";
    return 输出;
}

// 功能：提交事实、动态、任务状态或运行回执。
std::uint64_t 提交外设观察等待项(结构_外设观察等待项 等待项)
{
    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    if (!等待项.幂等键.empty()) {
        for (const auto& 已有 : 状态.等待项集合) {
            if (已有.幂等键 == 等待项.幂等键) {
                return 已有.等待项ID;
            }
        }
    }
    if (等待项.等待项ID == 0) {
        等待项.等待项ID = ++外设观察等待项ID计数器();
    }
    状态.等待项集合.push_back(等待项);
    return 等待项.等待项ID;
}

// 功能：等待线程、任务、外设或条件变化。
bool 完成外设观察等待项(std::uint64_t 等待项ID)
{
    if (等待项ID == 0) {
        return false;
    }

    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    const auto 旧数量 = 状态.等待项集合.size();
    状态.等待项集合.erase(
        std::remove_if(
            状态.等待项集合.begin(),
            状态.等待项集合.end(),
            [等待项ID](const 结构_外设观察等待项& 等待项) noexcept {
                return 等待项.等待项ID == 等待项ID;
            }),
        状态.等待项集合.end());
    return 状态.等待项集合.size() != 旧数量;
}

// 功能：判断条件、状态或业务结果是否成立。
结构_目标观察约束可用性判定 判断目标观察约束可用性(
    const 结构_目标观察约束特征组& 约束,
    std::int64_t 当前时间毫秒,
    const 结构_外设观察等待项* 等待项)
{
    结构_目标观察约束可用性判定 判定{};
    const auto 设置 = [&判定](
        枚举_目标观察约束可用性 状态,
        bool 强匹配,
        bool 降级匹配) {
        判定.状态 = 状态;
        判定.强匹配条件状态 = 强匹配
            ? 目标约束条件状态_满足
            : 目标约束条件状态_未满足;
        判定.降级匹配条件状态 = 降级匹配
            ? 目标约束条件状态_满足
            : 目标约束条件状态_未满足;
        判定.先验可用性收束状态 = 强匹配
            ? 先验可用性收束状态_强匹配
            : (降级匹配
                ? 先验可用性收束状态_降级匹配
                : (状态 == 枚举_目标观察约束可用性::未定义
                    ? 先验可用性收束状态_未定义
                    : 先验可用性收束状态_不可用));
        判定.可强匹配 = 强匹配;
        判定.可降级匹配 = 降级匹配;
        return 判定;
    };

    if (约束.状态 == 枚举_目标观察约束状态::已撤销) {
        return 设置(枚举_目标观察约束可用性::已撤销, false, false);
    }
    if (约束.状态 == 枚举_目标观察约束状态::已过期
        || (约束.TTL毫秒 > 0
            && 约束.写入时间毫秒 > 0
            && 当前时间毫秒 > 0
            && 当前时间毫秒 - 约束.写入时间毫秒 > 约束.TTL毫秒)) {
        return 设置(枚举_目标观察约束可用性::已过期, false, false);
    }
    if (约束.目标存在ID.empty()) {
        return 设置(枚举_目标观察约束可用性::缺目标存在, false, false);
    }
    if (约束.目标特征类型ID.empty()) {
        return 设置(枚举_目标观察约束可用性::缺目标特征, false, false);
    }
    if (!目标观察约束_有当前值(约束)) {
        return 设置(枚举_目标观察约束可用性::缺当前值, false, false);
    }
    if (!目标观察约束_有定位材料(约束)) {
        return 设置(枚举_目标观察约束可用性::缺定位材料, false, false);
    }
    if (约束.状态 == 枚举_目标观察约束状态::材料不可回查) {
        return 设置(枚举_目标观察约束可用性::材料不可回查, false, false);
    }
    const std::string* 等待项目标存在 = nullptr;
    if (等待项 && !等待项->目标存在ID.empty()) {
        等待项目标存在 = &等待项->目标存在ID;
    }
    if (等待项目标存在
        && *等待项目标存在 != 约束.目标存在ID
        && (等待项->等待项ID == 0 || 等待项->等待项ID != 约束.来源等待项ID)) {
        return 设置(枚举_目标观察约束可用性::目标已替代, false, false);
    }

    const bool 缺精细材料 = !目标观察约束_有精细材料(约束);
    if (缺精细材料) {
        if (约束.允许降级) {
            return 设置(枚举_目标观察约束可用性::可降级使用, false, true);
        }
        return 设置(枚举_目标观察约束可用性::材料不可回查, false, false);
    }

    return 设置(枚举_目标观察约束可用性::可用, true, false);
}

// 功能：提交事实、动态、任务状态或运行回执。
std::uint64_t 提交目标观察约束特征组(结构_目标观察约束特征组 约束)
{
    目标观察约束_修正提交默认值(约束);
    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    if (!约束.幂等键.empty()) {
        for (auto& 已有 : 状态.目标观察约束缓冲区) {
            if (已有.幂等键 == 约束.幂等键) {
                const auto 已有ID = 已有.约束ID;
                约束.约束ID = 已有ID;
                已有 = std::move(约束);
                状态.最新目标观察约束ID = 已有ID;
                目标观察约束_清理已加锁(状态, 外设观察当前时间毫秒());
                return 已有ID;
            }
        }
    }
    状态.目标观察约束缓冲区.push_back(std::move(约束));
    状态.最新目标观察约束ID = 状态.目标观察约束缓冲区.back().约束ID;
    目标观察约束_清理已加锁(状态, 外设观察当前时间毫秒());
    return 状态.最新目标观察约束ID;
}

// 功能：从指定来源读取数据或状态。
std::optional<结构_目标观察约束特征组> 读取目标观察约束_按等待项(
    const 结构_外设观察等待项& 等待项)
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    目标观察约束_清理已加锁(状态, 当前时间);
    for (auto 迭代器 = 状态.目标观察约束缓冲区.rbegin();
        迭代器 != 状态.目标观察约束缓冲区.rend();
        ++迭代器) {
        if (!目标观察约束_匹配等待项(*迭代器, 等待项)) {
            continue;
        }
        const auto 判定 = 判断目标观察约束可用性(*迭代器, 当前时间, &等待项);
        if (判定.强匹配条件状态 == 目标约束条件状态_满足
            || 判定.降级匹配条件状态 == 目标约束条件状态_满足) {
            return *迭代器;
        }
    }
    return std::nullopt;
}

// 功能：从指定来源读取数据或状态。
std::optional<结构_目标观察约束特征组> 读取目标观察约束_按目标特征(
    const std::string& 目标存在ID,
    const std::string& 目标特征类型ID)
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    目标观察约束_清理已加锁(状态, 当前时间);
    for (auto 迭代器 = 状态.目标观察约束缓冲区.rbegin();
        迭代器 != 状态.目标观察约束缓冲区.rend();
        ++迭代器) {
        if (目标观察约束_匹配目标特征(*迭代器, 目标存在ID, 目标特征类型ID)) {
            return *迭代器;
        }
    }
    return std::nullopt;
}

// 功能：从候选集中选择当前使用对象或方法。
std::optional<结构_目标观察约束特征组> 选择最新可用目标观察约束(
    const std::string& 目标存在ID,
    const std::string& 目标特征类型ID,
    std::int64_t 当前时间毫秒)
{
    auto& 状态 = 外设观察队列状态();
    if (当前时间毫秒 <= 0) {
        当前时间毫秒 = 外设观察当前时间毫秒();
    }
    std::lock_guard<std::mutex> 锁(状态.互斥);
    目标观察约束_清理已加锁(状态, 当前时间毫秒);
    for (auto 迭代器 = 状态.目标观察约束缓冲区.rbegin();
        迭代器 != 状态.目标观察约束缓冲区.rend();
        ++迭代器) {
        if (!目标观察约束_匹配目标特征(*迭代器, 目标存在ID, 目标特征类型ID)) {
            continue;
        }
        const auto 判定 = 判断目标观察约束可用性(*迭代器, 当前时间毫秒, nullptr);
        if (判定.强匹配条件状态 == 目标约束条件状态_满足
            || 判定.降级匹配条件状态 == 目标约束条件状态_满足) {
            return *迭代器;
        }
    }
    return std::nullopt;
}

// 功能：从指定来源读取数据或状态。
std::vector<结构_目标观察约束特征组> 读取可用目标观察约束集合_按目标特征(
    const std::string& 目标特征类型ID,
    std::size_t 最大数量,
    std::int64_t 当前时间毫秒)
{
    std::vector<结构_目标观察约束特征组> 结果{};
    if (目标特征类型ID.empty()) {
        return 结果;
    }
    auto& 状态 = 外设观察队列状态();
    if (当前时间毫秒 <= 0) {
        当前时间毫秒 = 外设观察当前时间毫秒();
    }
    std::lock_guard<std::mutex> 锁(状态.互斥);
    目标观察约束_清理已加锁(状态, 当前时间毫秒);
    std::vector<std::string> 已取目标存在{};
    for (auto 迭代器 = 状态.目标观察约束缓冲区.rbegin();
        迭代器 != 状态.目标观察约束缓冲区.rend();
        ++迭代器) {
        if (迭代器->目标特征类型ID != 目标特征类型ID
            || 迭代器->目标存在ID.empty()) {
            continue;
        }
        if (std::find(
                已取目标存在.begin(),
                已取目标存在.end(),
                迭代器->目标存在ID)
            != 已取目标存在.end()) {
            continue;
        }
        const auto 判定 = 判断目标观察约束可用性(*迭代器, 当前时间毫秒, nullptr);
        if (判定.强匹配条件状态 != 目标约束条件状态_满足
            && 判定.降级匹配条件状态 != 目标约束条件状态_满足) {
            continue;
        }
        结果.push_back(*迭代器);
        已取目标存在.push_back(迭代器->目标存在ID);
        if (最大数量 > 0 && 结果.size() >= 最大数量) {
            break;
        }
    }
    return 结果;
}

// 功能：发布场景体素先验到既有目标观察约束材料槽，不写世界树事实或场景体素缓存。
std::uint64_t 发布外设观察先验_由场景体素先验快照(
    const 场景体素模块::结构_SceneVoxelPrior& 先验快照,
    const std::string& 目标外设编号,
    const std::string& 目标存在ID,
    const std::string& 目标特征类型ID,
    std::int64_t 当前时间毫秒,
    std::int64_t 最大允许先验年龄毫秒)
{
    return 生成目标观察约束_由场景体素先验快照(
        先验快照,
        目标外设编号,
        目标存在ID,
        目标特征类型ID,
        当前时间毫秒,
        最大允许先验年龄毫秒);
}

// 功能：按 SceneVoxelPrior 只读材料计算外设本帧处理优先级，不写入队列或事实。
std::int64_t 计算外设处理先验优先级_由场景体素先验快照(
    const 场景体素模块::结构_SceneVoxelPrior& 先验快照,
    std::int64_t 当前时间毫秒,
    std::int64_t 最大允许先验年龄毫秒) noexcept
{
    if (!先验快照.只读状态 || 先验快照.版本 <= 0) {
        return 0;
    }

    std::int64_t 优先级 = 20;
    if (先验快照.坐标系版本 <= 0) {
        return 优先级;
    }

    const auto 先验年龄毫秒 = 外设先验_快照年龄毫秒(先验快照, 当前时间毫秒);
    const bool 已过期 = 最大允许先验年龄毫秒 > 0
        && 先验年龄毫秒 > 最大允许先验年龄毫秒;
    const bool 有图层句柄 =
        先验快照.先验感兴趣区域集合句柄.有效()
        || 先验快照.预计深度范围图句柄.有效()
        || 先验快照.已知空闲区句柄.有效()
        || 先验快照.未知区句柄.有效()
        || 先验快照.自由空间冲突候选区句柄.有效()
        || 先验快照.可见性预计算结果句柄.有效();

    bool 有粗空间范围 = false;
    for (const auto& 项 : 先验快照.存在项集合) {
        if (项.场景AABB_mm.有效()) {
            有粗空间范围 = true;
            break;
        }
    }

    if (有图层句柄) {
        优先级 = 80;
    } else if (有粗空间范围) {
        优先级 = 60;
    } else if (!先验快照.存在项集合.empty()) {
        优先级 = 40;
    }

    if (先验快照.统计.先验图层生成失败数量 > 0) {
        优先级 -= 10;
    }
    if (先验快照.统计.严重缺口数量 > 0) {
        const auto 严重缺口惩罚 = std::min<std::int64_t>(3, 先验快照.统计.严重缺口数量) * 10;
        优先级 -= 严重缺口惩罚;
    } else if (先验快照.统计.缺口数量 > 0) {
        优先级 -= 10;
    }
    if (已过期) {
        优先级 = std::min<std::int64_t>(优先级, 30);
    }

    return std::clamp<std::int64_t>(优先级, 0, 100);
}

// 功能：由场景体素只读快照生成目标观察约束，失败时返回 0 作为缺口待承接信号。
std::uint64_t 生成目标观察约束_由场景体素先验快照(
    const 场景体素模块::结构_SceneVoxelPrior& 先验快照,
    const std::string& 目标外设编号,
    const std::string& 目标存在ID,
    const std::string& 目标特征类型ID,
    std::int64_t 当前时间毫秒,
    std::int64_t 最大允许先验年龄毫秒)
{
    if (目标存在ID.empty() || 目标特征类型ID.empty()) {
        return 0;
    }
    if (!先验快照.只读状态 || 先验快照.版本 <= 0 || 先验快照.坐标系版本 <= 0) {
        return 0;
    }
    if (当前时间毫秒 <= 0) {
        当前时间毫秒 = 外设观察当前时间毫秒();
    }
    const auto 先验年龄毫秒 = 外设先验_快照年龄毫秒(先验快照, 当前时间毫秒);
    if (最大允许先验年龄毫秒 > 0 && 先验年龄毫秒 > 最大允许先验年龄毫秒) {
        return 0;
    }
    if (const auto 既有 = 选择最新可用目标观察约束(
        目标存在ID,
        目标特征类型ID,
        当前时间毫秒)) {
        return 既有->约束ID;
    }

    结构_目标观察约束特征组 约束{};
    约束.幂等键 =
        "scene-prior|device=" + 目标外设编号
        + "|target=" + 目标存在ID
        + "|feature=" + 目标特征类型ID
        + "|snapshot=" + std::to_string(先验快照.版本);
    约束.目标存在ID = 目标存在ID;
    约束.目标特征类型ID = 目标特征类型ID;
    约束.目标特征当前值 = 先验快照.版本;
    约束.目标特征当前值类型 = 枚举_目标特征当前值类型::I64;
    约束.目标特征允许误差 = 先验快照.统计.缺口数量;
    约束.目标特征稳定阈值 = 先验快照.统计.严重缺口数量;
    约束.写入时间毫秒 = 当前时间毫秒;
    约束.TTL毫秒 = 最大允许先验年龄毫秒 > 0
        ? 最大允许先验年龄毫秒
        : 目标观察约束默认TTL毫秒;
    约束.最大允许观测年龄毫秒 = 最大允许先验年龄毫秒;
    约束.最大允许报告年龄毫秒 = 最大允许先验年龄毫秒;
    约束.最大预测误差 = 先验年龄毫秒;
    约束.允许降级 = true;
    约束.允许缓存 = true;
    约束.状态 = 枚举_目标观察约束状态::可用;

    if (!外设先验_填充先验材料(约束, 先验快照)) {
        return 0;
    }
    return 提交目标观察约束特征组(std::move(约束));
}

// 功能：按目标读取当前可用外设处理先验，本质上复用目标观察约束缓冲区。
std::optional<结构_目标观察约束特征组> 读取最新外设处理先验_按目标(
    const std::string& 目标存在ID,
    const std::string& 目标特征类型ID,
    std::int64_t 当前时间毫秒)
{
    if (目标存在ID.empty() || 目标特征类型ID.empty()) {
        return std::nullopt;
    }
    return 选择最新可用目标观察约束(
        目标存在ID,
        目标特征类型ID,
        当前时间毫秒);
}

// 功能：按函数名执行对应处理。
bool 撤销目标观察约束特征组(std::uint64_t 约束ID)
{
    if (约束ID == 0) {
        return false;
    }
    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    for (auto& 约束 : 状态.目标观察约束缓冲区) {
        if (约束.约束ID != 约束ID) {
            continue;
        }
        约束.状态 = 枚举_目标观察约束状态::已撤销;
        return true;
    }
    return false;
}

// 功能：清理缓存、临时状态或运行资源。
std::size_t 清理目标观察约束特征组(std::int64_t 当前时间毫秒)
{
    auto& 状态 = 外设观察队列状态();
    if (当前时间毫秒 <= 0) {
        当前时间毫秒 = 外设观察当前时间毫秒();
    }
    std::lock_guard<std::mutex> 锁(状态.互斥);
    return 目标观察约束_清理已加锁(状态, 当前时间毫秒);
}

// 功能：按函数名执行对应处理。
std::string 构造目标观察约束摘要(const 结构_目标观察约束特征组& 约束)
{
    const auto 判定 = 判断目标观察约束可用性(
        约束,
        外设观察当前时间毫秒(),
        nullptr);
    std::ostringstream 输出;
    输出
        << "约束ID=" << 约束.约束ID
        << " | 目标存在=" << 约束.目标存在ID
        << " | 目标特征=" << 约束.目标特征类型ID
        << " | 当前值类型=" << static_cast<int>(约束.目标特征当前值类型)
        << " | 当前值=" << 约束.目标特征当前值
        << " | 当前值句柄=" << 约束.目标特征当前值句柄
        << " | 来源报告ID=" << 约束.来源报告ID
        << " | 来源帧ID=" << 约束.来源帧ID
        << " | 来源等待项ID=" << 约束.来源等待项ID
        << " | TTL毫秒=" << 约束.TTL毫秒
        << " | ROI=" << 约束.ROI句柄
        << " | 掩码=" << 约束.像素集合掩码句柄
        << " | 点集=" << 约束.点集句柄
        << " | 可用性=" << 目标观察约束可用性文本(判定.状态)
        << " | 先验可用性收束状态=" << 判定.先验可用性收束状态
        << " | 强匹配条件状态=" << 判定.强匹配条件状态
        << " | 降级匹配条件状态=" << 判定.降级匹配条件状态;
    return 输出.str();
}

// 功能：从指定来源读取数据或状态。
std::optional<结构_外设观察报告队列项> 读取最新外设观察报告(
    const std::string& 外设ID,
    枚举_外设观察报告类型 报告类型)
{
    return 读取最新外设观察报告(外设ID, 报告类型, false);
}

// 功能：从指定来源读取数据或状态。
std::optional<结构_外设观察报告队列项> 读取最新外设观察报告(
    const std::string& 外设ID,
    枚举_外设观察报告类型 报告类型,
    bool 包含已消费)
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    外设观察报告_清理已加锁(状态, 当前时间);
    D455_清理短期材料页_已加锁(状态, 当前时间);
    const 结构_外设观察报告队列项* 近组锚点 = nullptr;
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
        if (!包含已消费 && 迭代器->已消费) {
            continue;
        }
        if (!外设ID.empty() && 迭代器->来源外设ID != 外设ID) {
            continue;
        }
        if (报告类型 != 枚举_外设观察报告类型::未指定 && 迭代器->报告类型 != 报告类型) {
            continue;
        }
        近组锚点 = &(*迭代器);
        break;
    }
    if (!近组锚点) {
        return std::nullopt;
    }
    const auto 近组帧ID = 外设提交_帧ID(*近组锚点);
    const auto 近组外设ID = 近组锚点->来源外设ID;
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
        if (!包含已消费 && 迭代器->已消费) {
            continue;
        }
        if (!外设ID.empty() && 迭代器->来源外设ID != 外设ID) {
            continue;
        }
        if (!近组外设ID.empty() && 迭代器->来源外设ID != 近组外设ID) {
            continue;
        }
        if (外设提交_帧ID(*迭代器) != 近组帧ID) {
            continue;
        }
        if (报告类型 != 枚举_外设观察报告类型::未指定 && 迭代器->报告类型 != 报告类型) {
            continue;
        }
        return *迭代器;
    }
    return std::nullopt;
}

// 功能：从指定来源读取数据或状态。
std::optional<结构_外设观察报告队列项> 读取外设观察报告_按ID(
    std::uint64_t 报告ID)
{
    return 读取外设观察报告_按ID(报告ID, true);
}

// 功能：从指定来源读取数据或状态。
std::optional<结构_外设观察报告队列项> 读取外设观察报告_按ID(
    std::uint64_t 报告ID,
    bool 包含已消费)
{
    if (报告ID == 0) {
        return std::nullopt;
    }
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    外设观察报告_清理已加锁(状态, 当前时间);
    D455_清理短期材料页_已加锁(状态, 当前时间);
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
        if (!包含已消费 && 迭代器->已消费) {
            continue;
        }
        if (迭代器->报告ID == 报告ID) {
            return *迭代器;
        }
    }
    return std::nullopt;
}

// 功能：从指定来源读取数据或状态。
std::vector<结构_外设观察报告队列项> 读取外设观察报告队列视图(
    const std::string& 外设ID,
    枚举_外设观察报告类型 报告类型,
    bool 包含已消费)
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    外设观察报告_清理已加锁(状态, 当前时间);
    D455_清理短期材料页_已加锁(状态, 当前时间);
    const 结构_外设观察报告队列项* 近组锚点 = nullptr;
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
        if (!外设ID.empty() && 迭代器->来源外设ID != 外设ID) {
            continue;
        }
        if (报告类型 != 枚举_外设观察报告类型::未指定
            && 迭代器->报告类型 != 报告类型) {
            continue;
        }
        近组锚点 = &(*迭代器);
        break;
    }
    if (!近组锚点) {
        return {};
    }
    const auto 近组帧ID = 外设提交_帧ID(*近组锚点);
    const auto 近组外设ID = 近组锚点->来源外设ID;
    std::vector<结构_外设观察报告队列项> 输出{};
    输出.reserve(状态.报告队列.size());
    for (const auto& 报告项 : 状态.报告队列) {
        if (!包含已消费 && 报告项.已消费) {
            continue;
        }
        if (!外设ID.empty() && 报告项.来源外设ID != 外设ID) {
            continue;
        }
        if (!近组外设ID.empty() && 报告项.来源外设ID != 近组外设ID) {
            continue;
        }
        if (外设提交_帧ID(报告项) != 近组帧ID) {
            continue;
        }
        if (报告类型 != 枚举_外设观察报告类型::未指定
            && 报告项.报告类型 != 报告类型) {
            continue;
        }
        输出.push_back(报告项);
    }
    return 输出;
}

// 功能：从指定来源读取数据或状态。
std::vector<结构_外设观察等待项> 读取外设观察等待项集合(
    const std::string& 外设ID,
    枚举_外设观察运行模式 观察运行模式)
{
    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    std::vector<结构_外设观察等待项> 输出{};
    输出.reserve(状态.等待项集合.size());
    for (const auto& 等待项 : 状态.等待项集合) {
        if (!外设ID.empty() && 等待项.目标外设ID != 外设ID) {
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

// 功能：等待线程、任务、外设或条件变化。
std::optional<结构_外设观察报告队列项> 按等待项匹配外设观察报告(
    const 结构_外设观察等待项& 等待项)
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    外设观察报告_清理已加锁(状态, 当前时间);
    D455_清理短期材料页_已加锁(状态, 当前时间);

    if (等待项.等待项ID != 0) {
        for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
            if (迭代器->已消费) {
                continue;
            }
            if (迭代器->匹配等待项ID != 等待项.等待项ID) {
                continue;
            }
            if (外设观察_报告满足等待项(*迭代器, 等待项, 当前时间)) {
                auto 匹配报告 = *迭代器;
                匹配报告.匹配等待项ID = 等待项.等待项ID;
                return 匹配报告;
            }
        }
    }

    const 结构_外设观察报告队列项* 近组锚点 = nullptr;
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
        if (迭代器->已消费) {
            continue;
        }
        if (!等待项.目标外设ID.empty() && 迭代器->来源外设ID != 等待项.目标外设ID) {
            continue;
        }
        if (!外设观察_报告满足等待项(*迭代器, 等待项, 当前时间)) {
            continue;
        }
        近组锚点 = &(*迭代器);
        break;
    }
    if (!近组锚点) {
        return std::nullopt;
    }
    const auto 近组帧ID = 外设提交_帧ID(*近组锚点);
    const auto 近组外设ID = 近组锚点->来源外设ID;
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
        if (!近组外设ID.empty() && 迭代器->来源外设ID != 近组外设ID) {
            continue;
        }
        if (外设提交_帧ID(*迭代器) != 近组帧ID) {
            continue;
        }
        if (迭代器->已消费) {
            continue;
        }
        if (外设观察_报告满足等待项(*迭代器, 等待项, 当前时间)) {
            auto 匹配报告 = *迭代器;
            匹配报告.匹配等待项ID = 等待项.等待项ID;
            return 匹配报告;
        }
    }
    return std::nullopt;
}

// 功能：按函数名执行对应处理。
bool 标记外设观察报告已消费(
    std::uint64_t 报告ID,
    std::uint64_t 等待项ID)
{
    if (报告ID == 0) {
        return false;
    }
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    for (auto& 报告项 : 状态.报告队列) {
        if (报告项.报告ID != 报告ID) {
            continue;
        }
        报告项.已消费 = true;
        报告项.消费等待项ID = 等待项ID;
        报告项.消费时间毫秒 = 当前时间;
        return true;
    }
    return false;
}

// 功能：清理缓存、临时状态或运行资源。
std::size_t 清理外设观察报告队列(std::int64_t 当前时间毫秒)
{
    auto& 状态 = 外设观察队列状态();
    if (当前时间毫秒 <= 0) {
        当前时间毫秒 = 外设观察当前时间毫秒();
    }
    std::lock_guard<std::mutex> 锁(状态.互斥);
    const auto 报告清理数量 = 外设观察报告_清理已加锁(状态, 当前时间毫秒);
    const auto 原材料页数量 = 状态.D455材料页队列.size();
    D455_清理短期材料页_已加锁(状态, 当前时间毫秒);
    const auto 材料页清理数量 = 原材料页数量 > 状态.D455材料页队列.size()
        ? 原材料页数量 - 状态.D455材料页队列.size()
        : 0;
    const auto 原视频帧数量 = 状态.D455控制面板视频帧队列.size();
    D455_清理控制面板视频帧_已加锁(状态, 当前时间毫秒);
    const auto 视频帧清理数量 = 原视频帧数量 > 状态.D455控制面板视频帧队列.size()
        ? 原视频帧数量 - 状态.D455控制面板视频帧队列.size()
        : 0;
    return 报告清理数量 + 材料页清理数量 + 视频帧清理数量;
}

// 功能：从指定来源读取数据或状态。
结构_外设观察队列状态 读取外设观察队列状态()
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    外设观察报告_清理已加锁(状态, 当前时间);
    D455_清理短期材料页_已加锁(状态, 当前时间);
    D455_清理控制面板视频帧_已加锁(状态, 当前时间);
    return 结构_外设观察队列状态{
        状态.队列容量,
        状态.报告队列.size(),
        状态.等待项集合.size(),
        状态.最新报告ID,
        状态.目标观察约束缓冲区.size(),
        状态.最新目标观察约束ID,
    };
}

// 功能：按函数名执行对应处理。
void 清空外设观察报告队列()
{
    auto& 状态 = 外设观察队列状态();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    状态.报告队列.clear();
    状态.D455材料页队列.clear();
    状态.D455控制面板视频帧队列.clear();
    状态.等待项集合.clear();
    状态.目标观察约束缓冲区.clear();
    状态.最新报告ID = 0;
    状态.最新D455控制面板视频帧ID = 0;
    状态.最新目标观察约束ID = 0;
}

// 功能：按函数名执行对应处理。
std::string 构造外设观察报告摘要(const 结构_外设观察报告队列项& 报告项)
{
    std::ostringstream 输出;
    std::int64_t 掩码簇数量 = 0;
    std::int64_t 彩色局部图簇数量 = 0;
    std::int64_t 深度局部图簇数量 = 0;
    std::int64_t 可回查材料簇数量 = 0;
    std::int64_t 轨迹稳定簇数量 = 0;
    std::int64_t 待发布簇数量 = 0;
    std::int64_t 丢失宽限簇数量 = 0;
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
        if (!簇.ROI句柄.empty()
            || !簇.点集句柄.empty()
            || !簇.空间候选句柄.empty()
            || !簇.像素归属验证材料句柄.empty()) {
            ++可回查材料簇数量;
        }
        if (簇.跨帧轨迹稳定状态 > 0) {
            ++轨迹稳定簇数量;
        }
        if (簇.跨帧轨迹待发布状态 > 0) {
            ++待发布簇数量;
        }
        if (簇.跨帧丢失宽限状态 > 0) {
            ++丢失宽限簇数量;
        }
    }
    输出 << "外设观察报告"
        << " | 报告ID=" << 报告项.报告ID
        << " | 来源外设ID=" << (报告项.来源外设ID.empty() ? "空" : 报告项.来源外设ID)
        << " | 观察运行模式=" << 外设观察运行模式文本(报告项.观察运行模式)
        << " | 本帧处理模式=" << 外设分割处理模式文本(报告项.本帧处理模式)
        << " | 本帧处理优先级=" << 报告项.本帧处理优先级
        << " | 本帧处理掩码=" << (报告项.本帧处理掩码句柄.empty() ? "空" : 报告项.本帧处理掩码句柄)
        << " | 类型=" << 外设观察报告类型文本(报告项.报告类型)
        << " | 时间戳毫秒=" << 报告项.时间戳毫秒
        << " | 成功=" << (报告项.采集成功 ? 1 : 0)
        << " | 帧=" << 报告项.帧宽度 << 'x' << 报告项.帧高度
        << " | 深度帧号=" << 报告项.深度帧号
        << " | 彩色帧号=" << 报告项.彩色帧号
        << " | 原始帧句柄=" << (报告项.原始观察帧句柄.empty() ? 0 : 1)
        << " | 深度帧句柄=" << (报告项.深度帧句柄.empty() ? 0 : 1)
        << " | 彩色帧句柄=" << (报告项.彩色帧句柄.empty() ? 0 : 1)
        << " | 对齐帧句柄=" << (报告项.对齐帧句柄.empty() ? 0 : 1)
        << " | 空间候选队列句柄=" << (报告项.空间候选队列句柄.empty() ? 0 : 1)
        << " | 像素归属账本句柄=" << (报告项.像素归属账本句柄.empty() ? 0 : 1)
        << " | 像素簇=" << 报告项.观察像素簇集合.size()
        << " | 掩码簇=" << 掩码簇数量
        << " | 彩色局部图簇=" << 彩色局部图簇数量
        << " | 深度局部图簇=" << 深度局部图簇数量
        << " | 可回查材料簇=" << 可回查材料簇数量
        << " | 已消费=" << (报告项.已消费 ? 1 : 0)
        << " | 消费等待项ID=" << 报告项.消费等待项ID
        << " | 深度有效率=" << 报告项.深度有效率
        << " | 空间坐标有效率=" << 报告项.空间坐标有效率
        << " | 质量=" << 报告项.帧质量评分
        << " | 报告生成方法=" << (报告项.报告生成方法ID.empty() ? "空" : 报告项.报告生成方法ID)
        << " | 报告置信度=" << 报告项.报告置信度
        << " | 报告层级=" << 报告项.报告层级
        << " | 主分割坐标系ID=" << (报告项.主分割坐标系ID.empty() ? "空" : 报告项.主分割坐标系ID)
        << " | 坐标系一致性状态=" << 外设坐标系一致性状态文本(报告项.坐标系一致性状态)
        << " | 深度坐标对齐=" << 外设坐标对齐状态文本(报告项.深度材料坐标对齐状态)
        << " | IR坐标对齐=" << 外设坐标对齐状态文本(报告项.左红外边缘坐标对齐状态)
        << " | RGB坐标对齐=" << 外设坐标对齐状态文本(报告项.RGB裁剪材料坐标对齐状态)
        << " | 掩码坐标对齐=" << 外设坐标对齐状态文本(报告项.像素掩码坐标对齐状态)
        << " | 外设材料可承接状态=" << 报告项.外设材料可承接状态
        << " | 可承接能力等级=" << 外设材料能力等级文本(报告项.可承接能力等级)
        << " | 稳定锚点数量=" << 报告项.稳定锚点数量
        << " | 稳定锚点密度=" << 报告项.稳定锚点密度
        << " | 稳定锚点覆盖率=" << 报告项.稳定锚点覆盖率
        << " | 边界锚点覆盖率=" << 报告项.边界锚点覆盖率
        << " | 平面轮廓稳定状态=" << 报告项.平面轮廓稳定状态
        << " | 深度锚定状态=" << 报告项.深度锚定状态
        << " | 空间可用状态=" << 报告项.空间可用状态;
    if (报告项.来源原始报告ID != 0 || 报告项.来源外设帧ID != 0) {
        输出 << " | 来源原始报告ID=" << 报告项.来源原始报告ID
            << " | 来源外设帧ID=" << 报告项.来源外设帧ID;
    }
    if (报告项.目标观察约束ID != 0 || 报告项.目标约束使用状态 != 0) {
        输出 << " | 目标观察约束ID=" << 报告项.目标观察约束ID
            << " | 目标约束使用状态=" << 报告项.目标约束使用状态
            << " | 目标特征当前观察值=" << 报告项.目标特征当前观察值
            << " | 目标特征误差值=" << 报告项.目标特征误差值;
    }
    if (报告项.跨帧采集状态 > 0) {
        输出 << " | 跨帧匹配簇=" << 报告项.跨帧匹配观察像素簇数量
            << " | 跨帧新建簇=" << 报告项.跨帧新建观察像素簇数量
            << " | 跨帧稳定簇=" << 报告项.跨帧稳定观察像素簇数量
            << " | 跨帧丢失簇=" << 报告项.跨帧丢失观察像素簇数量
            << " | 轨迹状态=" << 外设Tracker轨迹状态文本(报告项.轨迹状态)
            << " | 轨迹连续帧数=" << 报告项.轨迹状态连续帧数
            << " | 轨迹稳定簇=" << 轨迹稳定簇数量
            << " | 待发布簇=" << 待发布簇数量
            << " | 丢失宽限簇=" << 丢失宽限簇数量;
    }
    if (报告项.报告类型 == 枚举_外设观察报告类型::扫描变化报告) {
        输出 << " | 新增簇=" << 报告项.新增观察像素簇数量
            << " | 消失簇=" << 报告项.消失观察像素簇数量
            << " | 变化簇=" << 报告项.变化观察像素簇数量
            << " | 高风险未知=" << 报告项.高风险未知区域数量;
    }
    if (报告项.报告类型 == 枚举_外设观察报告类型::跟踪报告) {
        输出 << " | 跟踪ID=" << 报告项.跟踪ID
            << " | 跟踪状态=" << 报告项.跟踪状态值
            << " | 轨迹状态=" << 外设Tracker轨迹状态文本(报告项.轨迹状态)
            << " | 轨迹连续帧数=" << 报告项.轨迹状态连续帧数
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

// 功能：按函数名执行对应处理。
std::string 构造D455材料包证据卡摘要(const 结构_外设观察报告队列项& 报告项)
{
    const auto 包头 = 构造外设提交包头_由观察报告(报告项);
    std::int64_t ROI句柄数量 = 0;
    std::int64_t 掩码句柄数量 = 0;
    std::int64_t 点集句柄数量 = 0;
    std::int64_t 深度切片句柄数量 = 0;
    std::int64_t 失败簇数量 = 0;
    std::string 首个失败原因{};
    for (const auto& 簇 : 报告项.观察像素簇集合) {
        if (!簇.ROI句柄.empty()) {
            ++ROI句柄数量;
        }
        if (!簇.像素集合掩码句柄.empty()) {
            ++掩码句柄数量;
        }
        if (!簇.点集句柄.empty()) {
            ++点集句柄数量;
        }
        if (!簇.深度轮廓局部图句柄.empty()) {
            ++深度切片句柄数量;
        }
        if (!簇.失败原因.empty()) {
            ++失败簇数量;
            if (首个失败原因.empty()) {
                首个失败原因 = 簇.失败原因;
            }
        }
    }

    std::ostringstream 输出;
    输出 << "D455材料包证据卡"
        << " | 报告ID=" << 报告项.报告ID
        << " | 来源外设ID=" << (报告项.来源外设ID.empty() ? "空" : 报告项.来源外设ID)
        << " | 报告类型=" << 外设观察报告类型文本(报告项.报告类型)
        << " | 材料页=" << (包头.材料页ID.empty() ? "空" : 包头.材料页ID)
        << " | 帧ID=" << 包头.帧ID
        << " | 深度帧号=" << 报告项.深度帧号
        << " | 彩色帧号=" << 报告项.彩色帧号
        << " | 时间戳毫秒=" << 报告项.时间戳毫秒
        << " | 尺寸=" << 报告项.帧宽度 << 'x' << 报告项.帧高度
        << " | 原始帧句柄=" << (报告项.原始观察帧句柄.empty() ? "空" : 报告项.原始观察帧句柄)
        << " | 深度帧句柄=" << (报告项.深度帧句柄.empty() ? "空" : 报告项.深度帧句柄)
        << " | 彩色帧句柄=" << (报告项.彩色帧句柄.empty() ? "空" : 报告项.彩色帧句柄)
        << " | 对齐帧句柄=" << (报告项.对齐帧句柄.empty() ? "空" : 报告项.对齐帧句柄)
        << " | 空间候选队列=" << (报告项.空间候选队列句柄.empty() ? "空" : 报告项.空间候选队列句柄)
        << " | 像素归属账本=" << (报告项.像素归属账本句柄.empty() ? "空" : 报告项.像素归属账本句柄)
        << " | 观察簇=" << 报告项.观察像素簇集合.size()
        << " | ROI句柄=" << ROI句柄数量
        << " | 掩码句柄=" << 掩码句柄数量
        << " | 点集句柄=" << 点集句柄数量
        << " | 深度切片句柄=" << 深度切片句柄数量
        << " | 材料句柄总数=" << 包头.材料句柄集.size()
        << " | 深度有效率=" << 报告项.深度有效率
        << " | 空间坐标有效率=" << 报告项.空间坐标有效率
        << " | 帧质量=" << 报告项.帧质量评分
        << " | 主分割坐标系ID=" << (报告项.主分割坐标系ID.empty() ? "空" : 报告项.主分割坐标系ID)
        << " | 坐标系一致性状态=" << 外设坐标系一致性状态文本(报告项.坐标系一致性状态)
        << " | 可承接能力等级=" << 外设材料能力等级文本(报告项.可承接能力等级)
        << " | 稳定锚点数量=" << 报告项.稳定锚点数量
        << " | 稳定锚点密度=" << 报告项.稳定锚点密度
        << " | 稳定锚点覆盖率=" << 报告项.稳定锚点覆盖率
        << " | 边界锚点覆盖率=" << 报告项.边界锚点覆盖率
        << " | 平面轮廓稳定状态=" << 报告项.平面轮廓稳定状态
        << " | 深度锚定状态=" << 报告项.深度锚定状态
        << " | 空间可用状态=" << 报告项.空间可用状态
        << " | 缺失材料=" << 包头.缺失材料.size()
        << " | 质量问题=" << 包头.质量问题.size()
        << " | 失败簇=" << 失败簇数量
        << " | 采集成功=" << (报告项.采集成功 ? 1 : 0);
    if (!报告项.失败原因.empty()) {
        输出 << " | 失败原因=" << 报告项.失败原因;
    }
    if (!首个失败原因.empty()) {
        输出 << " | 首个簇失败原因=" << 首个失败原因;
    }
    if (!包头.材料句柄集.empty()) {
        输出 << " | 材料句柄样本=";
        const std::size_t 上限 = (std::min)(包头.材料句柄集.size(), std::size_t{6});
        for (std::size_t 索引 = 0; 索引 < 上限; ++索引) {
            if (索引 > 0) {
                输出 << ';';
            }
            输出 << 包头.材料句柄集[索引];
        }
        if (包头.材料句柄集.size() > 上限) {
            输出 << ";...";
        }
    }
    return 输出.str();
}

// 功能：执行识别相关匹配、确认或结果输出逻辑。
std::string 构造D455逐簇识别样本卡摘要(const 结构_外设观察报告队列项& 报告项)
{
    if (报告项.报告类型 != 枚举_外设观察报告类型::逐簇识别报告) {
        std::ostringstream 输出;
        输出 << "D455逐簇识别样本卡"
            << " | 报告ID=" << 报告项.报告ID
            << " | 报告类型=" << 外设观察报告类型文本(报告项.报告类型)
            << " | 状态=非逐簇识别报告";
        return 输出.str();
    }

    const auto 包 = 构造外设识别提交包_由观察报告(报告项);
    if (!包.has_value()) {
        std::ostringstream 输出;
        输出 << "D455逐簇识别样本卡"
            << " | 报告ID=" << 报告项.报告ID
            << " | 状态=识别提交包不可构造";
        return 输出.str();
    }

    std::size_t 失败簇数量 = 0;
    std::size_t 空间候选句柄数量 = 0;
    std::size_t 轨迹稳定簇数量 = 0;
    std::size_t 待发布簇数量 = 0;
    std::string 首个失败原因{};
    for (const auto& 簇 : 报告项.观察像素簇集合) {
        if (!簇.失败原因.empty()) {
            ++失败簇数量;
            if (首个失败原因.empty()) {
                首个失败原因 = 簇.失败原因;
            }
        }
        if (!簇.空间候选句柄.empty()) {
            ++空间候选句柄数量;
        }
        if (簇.跨帧轨迹稳定状态 > 0) {
            ++轨迹稳定簇数量;
        }
        if (簇.跨帧轨迹待发布状态 > 0) {
            ++待发布簇数量;
        }
    }

    std::size_t 候选材料句柄数量 = 0;
    std::size_t 候选证据不足数量 = 0;
    for (const auto& 候选 : 包->新存在候选) {
        候选材料句柄数量 += 候选.材料句柄集.size();
        候选证据不足数量 += 候选.证据不足原因.size();
    }
    for (const auto& 候选 : 包->已有存在匹配候选) {
        候选材料句柄数量 += 候选.材料句柄集.size();
        候选证据不足数量 += 候选.证据不足原因.size();
    }

    const auto 候选存在数量 = 包->新存在候选.size() + 包->已有存在匹配候选.size();
    const auto 未识别区域数量 = 报告项.未归簇区域数量 > 0
        ? static_cast<std::uint64_t>(报告项.未归簇区域数量)
        : std::uint64_t{0};
    const auto 不可观测区域数量 = 报告项.不可观测区域数量 > 0
        ? static_cast<std::uint64_t>(报告项.不可观测区域数量)
        : std::uint64_t{0};

    std::ostringstream 输出;
    输出 << "D455逐簇识别样本卡"
        << " | 报告ID=" << 报告项.报告ID
        << " | 来源外设ID=" << (报告项.来源外设ID.empty() ? "空" : 报告项.来源外设ID)
        << " | 观察像素簇=" << 报告项.观察像素簇集合.size()
        << " | 有效观察区域=" << 包->有效观察区域.size()
        << " | 无效区域=" << 包->无效区域.size()
        << " | 未识别区域=" << 未识别区域数量
        << " | 冲突区域=" << 包->冲突区域.size()
        << " | 背景区域=" << 包->背景区域.size()
        << " | 不可观测区域=" << 不可观测区域数量
        << " | 候选存在=" << 候选存在数量
        << " | 新候选=" << 包->新存在候选.size()
        << " | 已有匹配候选=" << 包->已有存在匹配候选.size()
        << " | ROI句柄=" << 包->ROI句柄集.size()
        << " | 掩码句柄=" << 包->掩码句柄集.size()
        << " | 点集句柄=" << 包->点集句柄集.size()
        << " | 深度切片句柄=" << 包->深度切片句柄集.size()
        << " | 候选材料句柄=" << 候选材料句柄数量
        << " | 空间候选句柄=" << 空间候选句柄数量
        << " | 跨帧稳定簇=" << 报告项.跨帧稳定观察像素簇数量
        << " | 轨迹稳定簇=" << 轨迹稳定簇数量
        << " | 待发布簇=" << 待发布簇数量
        << " | 像素归属账本句柄=" << (包->像素归属账本句柄.empty() ? "空" : 包->像素归属账本句柄)
        << " | 报告证据不足=" << 包->证据不足原因.size()
        << " | 候选证据不足=" << 候选证据不足数量
        << " | 失败簇=" << 失败簇数量
        << " | 只读边界=材料归属和存在候选,不确认世界存在";
    if (!首个失败原因.empty()) {
        输出 << " | 首个簇失败原因=" << 首个失败原因;
    }
    return 输出.str();
}

// 功能：执行扫描相关材料获取、比较或提交逻辑。
std::string 构造D455扫描变化样本卡摘要(const 结构_外设观察报告队列项& 报告项)
{
    if (报告项.报告类型 != 枚举_外设观察报告类型::扫描变化报告) {
        std::ostringstream 输出;
        输出 << "D455扫描变化样本卡"
            << " | 报告ID=" << 报告项.报告ID
            << " | 报告类型=" << 外设观察报告类型文本(报告项.报告类型)
            << " | 状态=非扫描变化报告";
        return 输出.str();
    }

    const auto 包 = 构造外设扫描提交包_由观察报告(报告项);
    if (!包.has_value()) {
        std::ostringstream 输出;
        输出 << "D455扫描变化样本卡"
            << " | 报告ID=" << 报告项.报告ID
            << " | 状态=扫描提交包不可构造";
        return 输出.str();
    }

    std::size_t ROI句柄数量 = 0;
    std::size_t 掩码句柄数量 = 0;
    std::size_t 点集句柄数量 = 0;
    std::size_t 深度切片句柄数量 = 0;
    std::size_t 已知存在结果缺口数量 = 0;
    for (const auto& 结果 : 包->已知存在结果) {
        if (!结果.当前ROI句柄.empty()) {
            ++ROI句柄数量;
        }
        if (!结果.当前掩码句柄.empty()) {
            ++掩码句柄数量;
        }
        if (!结果.当前点集句柄.empty()) {
            ++点集句柄数量;
        }
        if (!结果.当前深度切片句柄.empty()) {
            ++深度切片句柄数量;
        }
        已知存在结果缺口数量 += 结果.原因.size();
    }

    const bool 首帧基准 =
        报告项.扫描变化基准状态 == 枚举_扫描变化基准状态::缺少逐簇识别基准;
    const auto 发现候选簇数量 = 首帧基准 ? 报告项.观察像素簇集合.size() : std::size_t{0};

    std::ostringstream 输出;
    输出 << "D455扫描变化样本卡"
        << " | 报告ID=" << 报告项.报告ID
        << " | 来源外设ID=" << (报告项.来源外设ID.empty() ? "空" : 报告项.来源外设ID)
        << " | 当前帧=" << 包->当前帧ID
        << " | 上一帧=" << 包->上一帧ID
        << " | 场景基准=" << 包->场景基准ID
        << " | 状态=" << (首帧基准 ? "首帧基准" : "有可比历史")
        << " | 上一状态=" << (首帧基准 ? "缺可比历史" : "上一逐簇识别基准可比")
        << " | 当前状态=当前逐簇识别报告"
        << " | 当前观察簇=" << 报告项.观察像素簇集合.size()
        << " | 发现候选=" << 发现候选簇数量
        << " | 已识别区域=" << 包->已识别区域.size()
        << " | 已知存在结果=" << 包->已知存在结果.size()
        << " | 新增簇=" << 报告项.新增观察像素簇数量
        << " | 消失簇=" << 报告项.消失观察像素簇数量
        << " | 变化簇=" << 报告项.变化观察像素簇数量
        << " | 高风险未知=" << 报告项.高风险未知区域数量
        << " | 未解释变化=" << 包->未解释变化区域.size()
        << " | 需重识别区域=" << 包->需重识别区域.size()
        << " | 冲突区域=" << 包->冲突区域.size()
        << " | 当前特征值=" << 包->当前特征值.size()
        << " | 特征变化值=" << 包->特征变化值.size()
        << " | ROI句柄=" << ROI句柄数量
        << " | 掩码句柄=" << 掩码句柄数量
        << " | 点集句柄=" << 点集句柄数量
        << " | 深度切片句柄=" << 深度切片句柄数量
        << " | 扫描覆盖率=" << 包->扫描覆盖率
        << " | 像素归属闭合率=" << 包->像素归属闭合率
        << " | 深度有效率=" << 包->深度有效率
        << " | 空间坐标有效率=" << 包->空间坐标有效率
        << " | 缺口原因=" << 包->缺口原因.size()
        << " | 已知存在结果缺口=" << 已知存在结果缺口数量
        << " | 首帧基准=" << (首帧基准 ? 1 : 0)
        << " | 只读边界=扫描变化材料,不确认全场景明确,不写安全结论";
    if (首帧基准) {
        输出 << " | 事实口径=首次扫描仅可形成发现存在或发现特征候选";
    } else {
        输出 << " | 事实口径=已有可比历史,变化计数仅作为扫描变化候选材料";
    }
    if (!报告项.失败原因.empty()) {
        输出 << " | 失败原因=" << 报告项.失败原因;
    }
    return 输出.str();
}

// 功能：等待线程、任务、外设或条件变化。
std::string 构造外设观察等待项摘要(const 结构_外设观察等待项& 等待项)
{
    std::ostringstream 输出;
    输出 << "外设观察等待项"
        << " | 等待项ID=" << 等待项.等待项ID
        << " | 来源需求=" << (等待项.来源需求ID.empty() ? "空" : 等待项.来源需求ID)
        << " | 目标外设ID=" << (等待项.目标外设ID.empty() ? "空" : 等待项.目标外设ID)
        << " | 观察运行模式=" << 外设观察运行模式文本(等待项.观察运行模式)
        << " | 本帧处理模式=" << 外设分割处理模式文本(等待项.本帧处理模式)
        << " | 本帧处理优先级=" << 等待项.本帧处理优先级
        << " | 本帧处理掩码=" << (等待项.本帧处理掩码句柄.empty() ? "空" : 等待项.本帧处理掩码句柄)
        << " | 期望报告=" << 外设观察报告类型文本(等待项.期望报告类型)
        << " | 最大年龄毫秒=" << 等待项.最大允许报告年龄毫秒
        << " | 最小稳定性=" << 等待项.最小稳定性要求
        << " | 允许缓存=" << (等待项.是否允许缓存报告 ? 1 : 0);
    if (等待项.目标观察簇ID >= 0) {
        输出 << " | 目标观察簇ID=" << 等待项.目标观察簇ID;
    }
    输出
        << " | 目标观察约束ID=" << 等待项.目标观察约束ID
        << " | 目标存在ID=" << (等待项.目标存在ID.empty() ? "空" : 等待项.目标存在ID)
        << " | 目标特征类型ID=" << (等待项.目标特征类型ID.empty() ? "空" : 等待项.目标特征类型ID)
        << " | 目标特征当前值类型=" << static_cast<int>(等待项.目标特征当前值类型)
        << " | 目标特征当前值=" << 等待项.目标特征当前值
        << " | 目标特征当前值句柄=" << 等待项.目标特征当前值句柄
        << " | 目标约束有效截止时间毫秒=" << 等待项.目标约束有效截止时间毫秒
        << " | 允许降级匹配=" << 等待项.允许降级匹配;
    if (!等待项.幂等键.empty()) {
        输出 << " | 幂等键=" << 等待项.幂等键;
    }
    return 输出.str();
}
