module;

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
    generation失配 = 4,
    目标已替代 = 5,
    材料不可回查 = 6,
    缺目标存在 = 7,
    缺目标特征 = 8,
    缺当前值 = 9,
    缺定位材料 = 10,
    已撤销 = 11,
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
    std::string 轨迹状态generation{};
    std::int64_t 轨迹状态连续帧数 = 0;
    std::string 轨迹状态变化原因{};
    std::string 轨迹状态变化动态候选{};
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
    std::int64_t 像素集合掩码状态 = 0;
    std::int64_t 彩色轮廓局部图状态 = 0;
    std::int64_t 深度轮廓局部图状态 = 0;
    std::int64_t 局部图宽度 = 0;
    std::int64_t 局部图高度 = 0;
    std::string 像素集合掩码句柄{};
    std::string 彩色轮廓局部图句柄{};
    std::string 深度轮廓局部图句柄{};
    std::string ROI引用{};
    std::string 点集引用{};
    std::string 空间候选引用{};
    std::string 像素归属验证材料引用{};
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
    std::string 标定版本{};
    std::string generation{};
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
    std::string 文本值{};
    std::uint64_t 宿主局部ID = 0;
    枚举_外设特征宿主类型 宿主类型 = 枚举_外设特征宿主类型::未指定;
    std::string 坐标系ID{};
    std::string 单位{};
    std::int64_t 时间戳毫秒 = 0;
    double 置信度 = 0.0;
    double 质量分 = 0.0;
    std::vector<std::string> 来源材料{};
    std::vector<std::string> 来源动态{};
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
    std::string 像素归属账本{};
    std::vector<std::string> ROI句柄集{};
    std::vector<std::string> 掩码句柄集{};
    std::vector<std::string> 点集句柄集{};
    std::vector<std::string> 深度切片句柄集{};
    std::vector<结构_外设特征值条目> 候选特征值{};
    std::vector<枚举_外设提交缺口码> 证据不足原因{};
};

export struct 结构_外设已知存在扫描结果 {
    std::uint64_t 已确认存在ID = 0;
    std::string 当前ROI{};
    std::string 当前掩码{};
    std::string 当前点集{};
    std::string 当前深度切片{};
    结构_外设空间中心 当前中心{};
    结构_外设空间范围AABB 当前空间范围{};
    结构_外设空间中心 上一中心{};
    结构_外设空间范围AABB 上一空间范围{};
    结构_外设向量3D 位置变化{};
    double 距离变化 = 0.0;
    double 深度变化 = 0.0;
    枚举_扫描匹配状态 匹配状态 = 枚举_扫描匹配状态::未指定;
    double 匹配置信度 = 0.0;
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
    std::string 当前ROI{};
    std::string 当前掩码{};
    std::string 当前点集{};
    std::string 当前深度切片{};
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
    std::string 原始观察帧句柄{};
    std::string 深度帧句柄{};
    std::string 彩色帧句柄{};
    std::string 对齐帧句柄{};
    std::string 空间候选队列引用{};
    std::string 像素归属账本引用{};
    std::string 相机内参版本{};
    std::string 相机外参版本{};
    std::string 坐标系版本{};
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
    std::string 报告生成方法版本{};
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
    枚举_外设Tracker轨迹状态 轨迹状态 = 枚举_外设Tracker轨迹状态::未指定;
    std::string 轨迹状态generation{};
    std::int64_t 轨迹状态连续帧数 = 0;
    std::string 轨迹状态变化原因{};
    std::string 轨迹状态变化动态候选{};

    std::uint64_t 目标观察约束ID = 0;
    std::string 约束generation{};
    std::string 目标特征generation{};
    std::string 来源材料generation{};
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
    std::string 目标外设{};
    枚举_外设观察运行模式 观察运行模式 = 枚举_外设观察运行模式::未指定;
    枚举_外设观察报告类型 期望报告类型 = 枚举_外设观察报告类型::未指定;
    std::int64_t 最大允许报告年龄毫秒 = 0;
    std::int64_t 最小稳定性要求 = 0;
    bool 是否允许缓存报告 = true;
    std::string 目标区域或目标簇{};
    std::string 幂等键{};
    std::string 质量要求摘要{};

    std::uint64_t 目标观察约束ID = 0;
    std::string 目标存在ID{};
    std::string 目标特征类型ID{};
    std::int64_t 目标特征当前值 = 0;
    枚举_目标特征当前值类型 目标特征当前值类型 = 枚举_目标特征当前值类型::未指定;
    std::string 目标特征当前值句柄{};
    std::string 约束generation{};
    std::string 目标特征generation{};
    std::string 来源材料generation{};
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

    std::string ROI引用{};
    std::string 像素集合掩码句柄{};
    std::string 点集引用{};

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

    std::string 约束generation{};
    std::string 目标特征generation{};
    std::string 来源材料generation{};

    std::int64_t 写入时间毫秒 = 0;
    std::int64_t TTL毫秒 = 0;
    std::int64_t 最大允许报告年龄毫秒 = 0;
    std::int64_t 最大允许观测年龄毫秒 = 0;
    std::int64_t 材料页TTL毫秒 = 0;

    std::int64_t 最低深度有效率 = 0;
    std::int64_t 最低空间坐标有效率 = 0;
    std::int64_t 最低掩码重合率 = 0;
    std::int64_t 最大预测误差 = 0;

    bool 允许降级 = true;
    bool 允许缓存 = true;
    枚举_目标观察约束状态 状态 = 枚举_目标观察约束状态::未定义;
    std::string 失效原因{};
};

export struct 结构_目标观察约束可用性判定 {
    枚举_目标观察约束可用性 状态 = 枚举_目标观察约束可用性::未定义;
    bool 可强匹配 = false;
    bool 可降级匹配 = false;
    std::string 原因{};
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
    std::string 依据{};
    std::string 需补观察原因{};
    std::string 建议外设动作{};
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
};

export struct 结构_D455观察材料句柄摘要 {
    枚举_D455观察材料读取状态 状态 = 枚举_D455观察材料读取状态::未找到;
    bool 成功 = false;
    std::string 失败原因{};
    std::uint64_t 报告ID = 0;
    std::string 资源类型{};
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
export std::uint64_t 提交外设观察报告(结构_外设观察报告队列项 报告项);
export bool 提交D455短期观察材料页(
    std::uint64_t 报告ID,
    const 双目相机本能适配器::调用结果& 来源,
    const 结构_外设观察报告队列项& 报告项);
export 结构_D455观察材料句柄摘要 解析D455观察材料句柄(const std::string& 句柄);
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
export bool 撤销目标观察约束特征组(
    std::uint64_t 约束ID,
    const std::string& 原因);
export std::size_t 清理目标观察约束特征组(std::int64_t 当前时间毫秒);
export std::string 构造目标观察约束摘要(const 结构_目标观察约束特征组& 约束);
export std::optional<结构_外设观察报告队列项> 读取最新外设观察报告(
    const std::string& 外设ID,
    枚举_外设观察报告类型 报告类型);
export std::optional<结构_外设观察报告队列项> 读取外设观察报告_按ID(
    std::uint64_t 报告ID);
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
    struct 结构_D455短期观察材料页 {
        std::uint64_t 报告ID = 0;
        std::int64_t 创建时间毫秒 = 0;
        std::int64_t 过期时间毫秒 = 0;
        双目相机本能适配器::调用结果 来源{};
        结构_外设观察报告队列项 报告{};
    };

    struct 结构_外设观察队列共享状态 {
        std::mutex 互斥{};
        std::deque<结构_外设观察报告队列项> 报告队列{};
        std::deque<结构_D455短期观察材料页> D455材料页队列{};
        std::vector<结构_外设观察等待项> 等待项集合{};
        std::deque<结构_目标观察约束特征组> 目标观察约束缓冲区{};
        std::uint64_t 最新报告ID = 0;
        std::uint64_t 最新目标观察约束ID = 0;
        std::size_t 队列容量 = 256;
        std::size_t D455材料页容量 = 6;
        std::size_t 目标观察约束缓冲区容量 = 128;
        std::int64_t D455材料页保留毫秒 = 60000;
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
    std::string 外设提交_标定版本(const 结构_外设观察报告队列项& 报告项)
    {
        std::ostringstream 输出;
        if (!报告项.相机内参版本.empty()) {
            输出 << "内参=" << 报告项.相机内参版本;
        }
        if (!报告项.相机外参版本.empty()) {
            if (输出.tellp() > 0) {
                输出 << ';';
            }
            输出 << "外参=" << 报告项.相机外参版本;
        }
        return 输出.str();
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    std::string 外设提交_generation(const 结构_外设观察报告队列项& 报告项)
    {
        if (报告项.报告生成方法ID.empty() && 报告项.报告生成方法版本.empty()) {
            return {};
        }
        return 报告项.报告生成方法ID + "#" + 报告项.报告生成方法版本;
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    std::vector<std::string> 外设提交_簇材料句柄集(const 结构_外设观察像素簇摘要& 簇)
    {
        std::vector<std::string> 输出{};
        外设提交_追加非空句柄(输出, 簇.ROI引用);
        外设提交_追加非空句柄(输出, 簇.点集引用);
        外设提交_追加非空句柄(输出, 簇.空间候选引用);
        外设提交_追加非空句柄(输出, 簇.像素归属验证材料引用);
        外设提交_追加非空句柄(输出, 簇.像素集合掩码句柄);
        外设提交_追加非空句柄(输出, 簇.彩色轮廓局部图句柄);
        外设提交_追加非空句柄(输出, 簇.深度轮廓局部图句柄);
        return 输出;
    }

    // 功能：提交事实、动态、任务状态或运行回执。
    void 外设提交_汇总报告材料(
        结构_外设提交包头& 包头,
        const 结构_外设观察报告队列项& 报告项)
    {
        外设提交_追加非空句柄(包头.材料句柄集, 报告项.原始观察帧句柄);
        外设提交_追加非空句柄(包头.材料句柄集, 报告项.深度帧句柄);
        外设提交_追加非空句柄(包头.材料句柄集, 报告项.彩色帧句柄);
        外设提交_追加非空句柄(包头.材料句柄集, 报告项.对齐帧句柄);
        外设提交_追加非空句柄(包头.材料句柄集, 报告项.空间候选队列引用);
        外设提交_追加非空句柄(包头.材料句柄集, 报告项.像素归属账本引用);
        for (const auto& 簇 : 报告项.观察像素簇集合) {
            for (const auto& 句柄 : 外设提交_簇材料句柄集(簇)) {
                外设提交_追加非空句柄(包头.材料句柄集, 句柄);
            }
        }
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
        if (报告项.空间候选队列引用.empty()) {
            外设提交_追加缺口(包头.缺失材料, 枚举_外设提交缺口码::缺空间候选);
        }
        if (报告项.像素归属账本引用.empty()) {
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
        const std::string& 单位,
        std::int64_t 时间戳毫秒,
        double 置信度,
        double 质量分,
        const std::vector<std::string>& 来源材料,
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
        条目.单位 = 单位;
        条目.时间戳毫秒 = 时间戳毫秒;
        条目.置信度 = 置信度;
        条目.质量分 = 质量分;
        条目.来源材料 = 来源材料;
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
        const auto 材料 = 外设提交_簇材料句柄集(簇);
        const auto 置信度 = 外设提交_百万分值(报告项.报告置信度);
        const auto 质量分 = 外设提交_百万分值(报告项.帧质量评分);
        const auto& 坐标系ID = 报告项.坐标系版本;
        const auto 时间戳 = 报告项.时间戳毫秒;
        const std::string 比较器ID = 可比较 ? "外设同源跨帧I64比较器" : std::string{};
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::像素数量, 簇.像素数量, 宿主局部ID, 宿主类型,
            坐标系ID, "像素", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::投影中心X, 簇.中心X, 宿主局部ID, 宿主类型,
            坐标系ID, "像素", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::投影中心Y, 簇.中心Y, 宿主局部ID, 宿主类型,
            坐标系ID, "像素", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间中心Z, 簇.中心Z, 宿主局部ID, 宿主类型,
            坐标系ID, "毫米", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最小X, 簇.空间范围最小X, 宿主局部ID, 宿主类型,
            坐标系ID, "毫米", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最大X, 簇.空间范围最大X, 宿主局部ID, 宿主类型,
            坐标系ID, "毫米", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最小Y, 簇.空间范围最小Y, 宿主局部ID, 宿主类型,
            坐标系ID, "毫米", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最大Y, 簇.空间范围最大Y, 宿主局部ID, 宿主类型,
            坐标系ID, "毫米", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最小Z, 簇.空间范围最小Z, 宿主局部ID, 宿主类型,
            坐标系ID, "毫米", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间范围最大Z, 簇.空间范围最大Z, 宿主局部ID, 宿主类型,
            坐标系ID, "毫米", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::深度有效率, 簇.深度有效率, 宿主局部ID, 宿主类型,
            坐标系ID, "百万分比", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间坐标有效率, 簇.空间坐标有效率, 宿主局部ID, 宿主类型,
            坐标系ID, "百万分比", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::空间连续性评分, 簇.空间连续性评分, 宿主局部ID, 宿主类型,
            坐标系ID, "百万分比", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::范围稳定性评分, 簇.范围稳定性评分, 宿主局部ID, 宿主类型,
            坐标系ID, "百万分比", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
        输出.push_back(外设提交_构造I64特征(
            枚举_外设特征类型ID::轮廓置信度, 簇.轮廓置信度, 宿主局部ID, 宿主类型,
            坐标系ID, "百万分比", 时间戳, 置信度, 质量分, 材料, 可比较, 比较器ID));
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
        return 报告项.报告类型 == 枚举_外设观察报告类型::逐簇识别报告
            && 报告项.报告层级 == 1;
    }

    // 功能：按函数名执行对应处理。
    std::size_t 外设观察报告_裁剪容量已加锁(
        结构_外设观察队列共享状态& 状态,
        std::int64_t 当前时间毫秒)
    {
        std::size_t 裁剪数量 = 0;
        while (状态.报告队列.size() > 状态.队列容量) {
            auto 可裁剪项 = std::find_if(
                状态.报告队列.begin(),
                状态.报告队列.end(),
                [当前时间毫秒](const 结构_外设观察报告队列项& 报告项) noexcept {
                    return !外设观察报告_容量裁剪保护(报告项, 当前时间毫秒);
                });
            if (可裁剪项 == 状态.报告队列.end()) {
                状态.报告队列.pop_front();
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
        摘要->资源类型 = 资源类型;
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
        摘要->失败原因.clear();
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
                摘要->失败原因 = "原始观察帧材料未就绪";
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
                摘要->失败原因 = "深度帧材料未就绪";
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
                摘要->失败原因 = "彩色帧材料未就绪";
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
                摘要->失败原因 = "对齐帧材料未就绪";
            }
            return true;
        }
        if (句柄 == 页.报告.空间候选队列引用) {
            D455_填充材料页通用摘要(页, 资源类型, 摘要);
            摘要->数据元素数量 = static_cast<std::int64_t>(页.来源.空间候选列表.size());
            if (页.来源.成功) {
                D455_标记材料摘要成功(摘要);
            } else {
                摘要->状态 = 枚举_D455观察材料读取状态::材料未就绪;
                摘要->失败原因 = "空间候选队列材料未就绪";
            }
            return true;
        }
        if (句柄 == 页.报告.像素归属账本引用) {
            D455_填充材料页通用摘要(页, 资源类型, 摘要);
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
                摘要->失败原因 = "像素归属账本输入材料未就绪";
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
            const bool 命中ROI = 句柄 == 簇.ROI引用;
            const bool 命中点集 = 句柄 == 簇.点集引用;
            const bool 命中空间候选 = 句柄 == 簇.空间候选引用;
            const bool 命中像素归属材料 = 句柄 == 簇.像素归属验证材料引用;
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
                摘要->状态 = 枚举_D455观察材料读取状态::材料未就绪;
                摘要->失败原因 = !有候选
                    ? "簇句柄未命中来源空间候选"
                    : (!投影有效
                        ? "簇投影不在当前帧内"
                        : "簇材料不可由当前短期材料页重建");
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

    // 功能：等待线程、任务、外设或条件变化。
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
        if (!等待项.约束generation.empty()
            && !报告项.约束generation.empty()
            && 报告项.约束generation != 等待项.约束generation) {
            return false;
        }
        if (!等待项.目标特征generation.empty()
            && !报告项.目标特征generation.empty()
            && 报告项.目标特征generation != 等待项.目标特征generation) {
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
        return !约束.ROI引用.empty()
            || !约束.像素集合掩码句柄.empty()
            || !约束.点集引用.empty()
            || 目标观察约束_二维范围有效(约束)
            || 目标观察约束_空间范围有效(约束);
    }

    // 功能：按函数名执行对应处理。
    bool 目标观察约束_有精细材料(const 结构_目标观察约束特征组& 约束) noexcept
    {
        return !约束.像素集合掩码句柄.empty()
            || !约束.点集引用.empty()
            || !约束.来源材料generation.empty();
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
        if (!等待项.目标区域或目标簇.empty()
            && 约束.目标存在ID == 等待项.目标区域或目标簇) {
            return true;
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
        if (约束.约束generation.empty()) {
            约束.约束generation = "目标观察约束#" + std::to_string(约束.约束ID);
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
    case 枚举_目标观察约束可用性::generation失配: return "generation失配";
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

    const auto 设置不合格 = [&](const std::string& 原因, const std::string& 建议) {
        判定.等级 = 枚举_观察材料等级::不合格;
        判定.依据 = 原因;
        判定.需补观察原因 = 原因;
        判定.建议外设动作 = 建议;
        return 判定;
    };

    if (报告项.报告类型 == 枚举_外设观察报告类型::未指定) {
        return 设置不合格("报告类型未指定", "重新观察并生成明确报告类型");
    }
    if (!报告项.采集成功) {
        return 设置不合格(
            报告项.失败原因.empty() ? "外设采集失败" : 报告项.失败原因,
            "重新取得观察材料");
    }

    const auto 当前时间 = 外设观察当前时间毫秒();
    if (等待项
        && 等待项->最大允许报告年龄毫秒 > 0
        && 报告项.时间戳毫秒 > 0
        && 当前时间 - 报告项.时间戳毫秒 > 等待项->最大允许报告年龄毫秒) {
        return 设置不合格("报告年龄超过等待项最大允许报告年龄", "重新观察并刷新报告");
    }

    const bool 有报告材料 =
        !报告项.原始观察帧句柄.empty()
        || !报告项.深度帧句柄.empty()
        || !报告项.彩色帧句柄.empty()
        || !报告项.对齐帧句柄.empty()
        || !报告项.空间候选队列引用.empty()
        || !报告项.像素归属账本引用.empty();
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
        if (!簇.ROI引用.empty()
            || !簇.像素集合掩码句柄.empty()
            || !簇.点集引用.empty()
            || !簇.空间候选引用.empty()
            || !簇.像素归属验证材料引用.empty()) {
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
            || !簇.ROI引用.empty()
            || !簇.像素集合掩码句柄.empty()
            || !簇.彩色轮廓局部图句柄.empty();
        const bool 簇深度锚定 =
            簇.深度有效率 >= 优质最低有效率
            || 簇.深度轮廓局部图状态 > 0
            || !簇.深度轮廓局部图句柄.empty();
        const bool 簇空间可用 =
            簇.空间坐标有效率 >= 优质最低有效率
            || !簇.空间候选引用.empty()
            || !簇.点集引用.empty();
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
        return 设置不合格("缺少可回查观察材料句柄", "重新提交材料页或重新观察");
    }
    if (需要簇材料 && 报告项.观察像素簇集合.empty()) {
        return 设置不合格("报告缺少观察像素簇集合", "重新观察或扩大重捕获范围");
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
        return 设置不合格("全局和目标区域均缺少基础深度/空间质量，且未形成平面轮廓稳定材料", "重新观察或调整相机工作模式");
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
        && 报告项.差异摘要.find("首帧基准") == std::string::npos
        && 报告项.差异摘要.find("缺少上一帧逐簇识别基准") == std::string::npos;
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

    std::ostringstream 依据;
    依据 << "等级=" << 观察材料等级文本(判定.等级)
        << ";全局优质=" << (全局优质 ? 1 : 0)
        << ";目标区域优质=" << (目标区域优质 ? 1 : 0)
        << ";目标约束使用状态=" << 报告项.目标约束使用状态
        << ";报告层级=" << 报告项.报告层级
        << ";能力等级=" << 外设材料能力等级文本(判定.可承接能力等级)
        << ";坐标系一致性=" << 外设坐标系一致性状态文本(判定.坐标系一致性状态)
        << ";anchor数量=" << 判定.稳定锚点数量
        << ";anchor密度=" << 判定.稳定锚点密度
        << ";anchor覆盖率=" << 判定.稳定锚点覆盖率
        << ";边界anchor覆盖率=" << 判定.边界锚点覆盖率
        << ";平面轮廓稳定=" << (判定.平面轮廓稳定状态 ? 1 : 0)
        << ";深度锚定=" << (判定.深度锚定状态 ? 1 : 0)
        << ";空间可用=" << (判定.空间可用状态 ? 1 : 0)
        << ";逐簇稳定硬门=" << (逐簇稳定子集硬门达标 ? 1 : 0)
        << ";逐簇稳定硬门簇=" << 逐簇稳定硬门簇数量
        << ";平面轮廓稳定簇=" << 平面轮廓稳定簇数量
        << ";深度锚定簇=" << 深度锚定簇数量
        << ";空间可用簇=" << 空间可用簇数量
        << ";跨帧稳定簇=" << 报告项.跨帧稳定观察像素簇数量
        << ";来源原始报告ID=" << 报告项.来源原始报告ID
        << ";深度有效率=" << 报告项.深度有效率
        << ";空间坐标有效率=" << 报告项.空间坐标有效率
        << ";目标区域深度有效率=" << 目标区域深度有效率
        << ";目标区域空间有效率=" << 目标区域空间有效率;
    判定.依据 = 依据.str();
    if (判定.等级 == 枚举_观察材料等级::可用) {
        判定.需补观察原因 = 平面轮廓稳定 && !深度锚定
            ? "平面轮廓稳定，深度锚定或空间能力不足"
            : "材料可用但未达到优质门槛或缺少强目标约束";
        判定.建议外设动作 = 报告项.目标约束使用状态 == 2
            ? "补齐掩码/点集/generation后重观察"
            : "继续观察并提升深度/空间有效率";
    } else {
        判定.需补观察原因 = "无";
        判定.建议外设动作 = "无需补观察";
    }
    return 判定;
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
        << " | 目标区域优质=" << (判定.目标区域优质 ? 1 : 0)
        << " | 依据=" << 判定.依据
        << " | 需补观察原因=" << 判定.需补观察原因
        << " | 建议外设动作=" << 判定.建议外设动作;
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
    包头.外设ID = 报告项.来源外设;
    包头.材料页ID = 外设提交_材料页ID(报告项);
    包头.坐标系ID = 报告项.坐标系版本;
    包头.标定版本 = 外设提交_标定版本(报告项);
    包头.generation = 外设提交_generation(报告项);
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
    包.像素归属账本 = 报告项.像素归属账本引用;
    std::uint64_t 序号 = 0;
    for (const auto& 簇 : 报告项.观察像素簇集合) {
        ++序号;
        const auto 区域ID = 外设提交_正局部ID(簇.外设内部簇ID, 序号);
        const bool 有效 = 簇.像素数量 > 0 && 簇.失败原因.empty();
        if (有效) {
            包.有效观察区域.push_back(区域ID);
        } else {
            包.无效区域.push_back(区域ID);
        }

        外设提交_追加非空句柄(包.ROI句柄集, 簇.ROI引用);
        外设提交_追加非空句柄(包.掩码句柄集, 簇.像素集合掩码句柄);
        外设提交_追加非空句柄(包.点集句柄集, 簇.点集引用);
        外设提交_追加非空句柄(包.深度切片句柄集, 簇.深度轮廓局部图句柄);

        结构_外设识别候选 候选{};
        候选.候选ID = 区域ID;
        候选.来源区域ID = 区域ID;
        候选.归属状态 = 有效
            ? 枚举_外设区域归属状态::候选
            : 枚举_外设区域归属状态::无效;
        候选.匹配置信度 = 外设提交_百万分值(簇.轮廓置信度);
        候选.材料句柄集 = 外设提交_簇材料句柄集(簇);
        if (候选.材料句柄集.empty()) {
            外设提交_追加缺口(候选.证据不足原因, 枚举_外设提交缺口码::缺材料句柄);
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
    包.当前帧ID = 包.包头.帧ID;
    const bool 缺少逐簇识别基准 =
        报告项.差异摘要.find("缺少上一帧逐簇识别基准") != std::string::npos;
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
    const std::vector<std::string> 报告材料 = 包.包头.材料句柄集;
    包.特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::新增观察像素簇数量,
        报告项.新增观察像素簇数量,
        报告宿主ID,
        枚举_外设特征宿主类型::区域,
        报告项.坐标系版本,
        "数量",
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料,
        true,
        "外设扫描报告计数比较器"));
    包.特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::消失观察像素簇数量,
        报告项.消失观察像素簇数量,
        报告宿主ID,
        枚举_外设特征宿主类型::区域,
        报告项.坐标系版本,
        "数量",
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料,
        true,
        "外设扫描报告计数比较器"));
    包.特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::变化观察像素簇数量,
        报告项.变化观察像素簇数量,
        报告宿主ID,
        枚举_外设特征宿主类型::区域,
        报告项.坐标系版本,
        "数量",
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料,
        true,
        "外设扫描报告计数比较器"));

    std::uint64_t 序号 = 0;
    for (const auto& 簇 : 报告项.观察像素簇集合) {
        ++序号;
        const auto 区域ID = 外设提交_正局部ID(簇.外设内部簇ID, 序号);
        const auto 匹配状态 = 外设提交_扫描匹配状态_由簇(簇);
        if (匹配状态 == 枚举_扫描匹配状态::需重识别) {
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
            报告项.坐标系版本,
            "枚举值",
            报告项.时间戳毫秒,
            外设提交_百万分值(报告项.报告置信度),
            外设提交_百万分值(报告项.帧质量评分),
            外设提交_簇材料句柄集(簇),
            true,
            "外设同源跨帧I64比较器"));

        结构_外设已知存在扫描结果 结果{};
        结果.当前ROI = 簇.ROI引用;
        结果.当前掩码 = 簇.像素集合掩码句柄;
        结果.当前点集 = 簇.点集引用;
        结果.当前深度切片 = 簇.深度轮廓局部图句柄;
        结果.当前中心 = 外设提交_簇中心(簇);
        结果.当前空间范围 = 外设提交_簇范围(簇);
        结果.匹配状态 = 匹配状态;
        结果.匹配置信度 = 外设提交_百万分值(簇.轮廓置信度);
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
    包.目标种子ID = 报告项.跟踪ID;
    包.跟踪窗口ID = 报告项.匹配等待项ID;
    包.当前帧ID = 包.包头.帧ID;
    包.上一帧ID = 包.当前帧ID > 0 ? 包.当前帧ID - 1 : 0;
    包.跟踪状态 = 外设提交_跟踪状态_由报告(报告项);
    包.跟踪置信度 = 外设提交_百万分值(报告项.报告置信度);
    if (包.目标种子ID == 0 && 报告项.跟踪种子.empty()) {
        外设提交_追加缺口(包.证据不足原因, 枚举_外设提交缺口码::缺目标种子);
    }
    外设提交_追加缺口(包.证据不足原因, 枚举_外设提交缺口码::缺已确认存在ID);

    if (!报告项.观察像素簇集合.empty()) {
        const auto& 簇 = 报告项.观察像素簇集合.front();
        const auto 目标局部ID = 外设提交_正局部ID(
            簇.外设内部簇ID,
            包.目标种子ID == 0 ? 1 : 包.目标种子ID);
        包.当前ROI = 簇.ROI引用;
        包.当前掩码 = 簇.像素集合掩码句柄;
        包.当前点集 = 簇.点集引用;
        包.当前深度切片 = 簇.深度轮廓局部图句柄;
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

    const std::vector<std::string> 报告材料 = 包.包头.材料句柄集;
    const auto 目标宿主ID = 包.目标种子ID;
    包.目标特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::跟踪状态值,
        报告项.跟踪状态值,
        目标宿主ID,
        枚举_外设特征宿主类型::目标,
        报告项.坐标系版本,
        "枚举值",
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料,
        true,
        "外设目标跟踪I64比较器"));
    包.目标特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::丢失状态值,
        报告项.丢失状态值,
        目标宿主ID,
        枚举_外设特征宿主类型::目标,
        报告项.坐标系版本,
        "枚举值",
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料,
        true,
        "外设目标跟踪I64比较器"));
    包.目标特征变化值.push_back(外设提交_构造I64特征(
        枚举_外设特征类型ID::稳定特征样本数量,
        报告项.稳定特征样本数量,
        目标宿主ID,
        枚举_外设特征宿主类型::目标,
        报告项.坐标系版本,
        "数量",
        报告项.时间戳毫秒,
        外设提交_百万分值(报告项.报告置信度),
        外设提交_百万分值(报告项.帧质量评分),
        报告材料,
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
    状态.报告队列.push_back(报告项);
    状态.最新报告ID = 报告项.报告ID;
    外设观察报告_清理已加锁(状态, 报告项.时间戳毫秒);
    return 报告项.报告ID;
}

// 功能：提交事实、动态、任务状态或运行回执。
bool 提交D455短期观察材料页(
    std::uint64_t 报告ID,
    const 双目相机本能适配器::调用结果& 来源,
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
    页.来源 = 来源;
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

// 功能：解析输入文本、消息、场景或运行包。
结构_D455观察材料句柄摘要 解析D455观察材料句柄(const std::string& 句柄)
{
    结构_D455观察材料句柄摘要 摘要{};
    if (句柄.empty()) {
        摘要.状态 = 枚举_D455观察材料读取状态::空句柄;
        摘要.失败原因 = "D455材料句柄为空";
        return 摘要;
    }
    if (!D455_句柄前缀有效(句柄)) {
        摘要.状态 = 枚举_D455观察材料读取状态::句柄格式错误;
        摘要.失败原因 = "D455材料句柄格式错误";
        return 摘要;
    }

    const auto 资源类型 = D455_句柄资源类型(句柄);
    if (资源类型.empty()) {
        摘要.状态 = 枚举_D455观察材料读取状态::句柄格式错误;
        摘要.失败原因 = "D455材料句柄缺少资源类型";
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
                摘要.失败原因 = "D455短期观察材料已过期";
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
    摘要.资源类型 = 资源类型;
    摘要.失败原因 = "未找到D455短期观察材料句柄";
    return 摘要;
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

    std::vector<std::string> 句柄集合{};
    auto 追加句柄 = [&](const std::string& 句柄) {
        if (!句柄.empty()) {
            句柄集合.push_back(句柄);
        }
    };
    追加句柄(簇.点集引用);
    追加句柄(簇.深度轮廓局部图句柄);
    追加句柄(簇.像素集合掩码句柄);
    追加句柄(簇.空间候选引用);
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

    std::string 最近失败原因{};
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
                最近失败原因 = 摘要.失败原因;
                continue;
            }
            const auto* 候选 = D455_查找材料页空间候选(*迭代器, 摘要.候选编号);
            if (!候选) {
                最近失败原因 = "D455簇句柄命中但未找到来源空间候选";
                continue;
            }
            return 构造视角(*迭代器, *候选, 摘要);
        }
    }

    return 失败(最近失败原因.empty()
        ? "未找到D455簇三维体素轮廓材料页"
        : 最近失败原因);
}

// 功能：从指定来源读取数据或状态。
结构_D455控制面板视频快照 读取最新D455控制面板视频快照()
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    D455_清理短期材料页_已加锁(状态, 当前时间);
    for (auto 迭代器 = 状态.D455材料页队列.rbegin();
        迭代器 != 状态.D455材料页队列.rend();
        ++迭代器) {
        if (!D455_颜色帧材料有效(迭代器->来源)) {
            continue;
        }
        return D455_构造控制面板视频快照_已加锁(*迭代器);
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
        bool 降级匹配,
        std::string 原因) {
        判定.状态 = 状态;
        判定.可强匹配 = 强匹配;
        判定.可降级匹配 = 降级匹配;
        判定.原因 = std::move(原因);
        return 判定;
    };

    if (约束.状态 == 枚举_目标观察约束状态::已撤销) {
        return 设置(枚举_目标观察约束可用性::已撤销, false, false,
            约束.失效原因.empty() ? "目标观察约束已撤销" : 约束.失效原因);
    }
    if (约束.状态 == 枚举_目标观察约束状态::已过期
        || (约束.TTL毫秒 > 0
            && 约束.写入时间毫秒 > 0
            && 当前时间毫秒 > 0
            && 当前时间毫秒 - 约束.写入时间毫秒 > 约束.TTL毫秒)) {
        return 设置(枚举_目标观察约束可用性::已过期, false, false,
            "目标观察约束TTL已过期");
    }
    if (约束.目标存在ID.empty()) {
        return 设置(枚举_目标观察约束可用性::缺目标存在, false, false,
            "目标观察约束缺目标存在ID");
    }
    if (约束.目标特征类型ID.empty()) {
        return 设置(枚举_目标观察约束可用性::缺目标特征, false, false,
            "目标观察约束缺目标特征类型ID");
    }
    if (!目标观察约束_有当前值(约束)) {
        return 设置(枚举_目标观察约束可用性::缺当前值, false, false,
            "目标观察约束缺目标特征当前值或句柄");
    }
    if (!目标观察约束_有定位材料(约束)) {
        return 设置(枚举_目标观察约束可用性::缺定位材料, false, false,
            "目标观察约束缺ROI、掩码、点集或AABB");
    }
    if (约束.状态 == 枚举_目标观察约束状态::材料不可回查) {
        return 设置(枚举_目标观察约束可用性::材料不可回查, false, false,
            约束.失效原因.empty() ? "目标观察约束材料不可回查" : 约束.失效原因);
    }
    const std::string* 等待项目标存在 = nullptr;
    if (等待项 && !等待项->目标存在ID.empty()) {
        等待项目标存在 = &等待项->目标存在ID;
    } else if (等待项 && !等待项->目标区域或目标簇.empty()) {
        等待项目标存在 = &等待项->目标区域或目标簇;
    }
    if (等待项目标存在
        && *等待项目标存在 != 约束.目标存在ID
        && (等待项->等待项ID == 0 || 等待项->等待项ID != 约束.来源等待项ID)) {
        return 设置(枚举_目标观察约束可用性::目标已替代, false, false,
            "等待项目标与目标观察约束目标存在ID不一致");
    }

    const bool 缺目标特征generation = 约束.目标特征generation.empty();
    const bool 缺精细材料 = !目标观察约束_有精细材料(约束);
    if (缺目标特征generation || 缺精细材料) {
        if (约束.允许降级) {
            std::string 原因{};
            if (缺目标特征generation) {
                原因 += "缺目标特征generation";
            }
            if (缺精细材料) {
                if (!原因.empty()) {
                    原因 += ";";
                }
                原因 += "缺精细材料或来源材料generation";
            }
            return 设置(枚举_目标观察约束可用性::可降级使用, false, true, 原因);
        }
        return 设置(
            缺目标特征generation
                ? 枚举_目标观察约束可用性::generation失配
                : 枚举_目标观察约束可用性::材料不可回查,
            false,
            false,
            缺目标特征generation
                ? "目标观察约束缺目标特征generation且不允许降级"
                : "目标观察约束缺精细材料且不允许降级");
    }

    return 设置(枚举_目标观察约束可用性::可用, true, false,
        "目标观察约束可强匹配使用");
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
        if (判定.可强匹配 || 判定.可降级匹配) {
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
        if (判定.可强匹配 || 判定.可降级匹配) {
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
        if (!判定.可强匹配 && !判定.可降级匹配) {
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

// 功能：按函数名执行对应处理。
bool 撤销目标观察约束特征组(
    std::uint64_t 约束ID,
    const std::string& 原因)
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
        约束.失效原因 = 原因.empty() ? "目标观察约束被撤销" : 原因;
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
        << " | 约束generation=" << 约束.约束generation
        << " | 目标特征generation=" << 约束.目标特征generation
        << " | 来源材料generation=" << 约束.来源材料generation
        << " | 来源等待项ID=" << 约束.来源等待项ID
        << " | TTL毫秒=" << 约束.TTL毫秒
        << " | ROI=" << 约束.ROI引用
        << " | 掩码=" << 约束.像素集合掩码句柄
        << " | 点集=" << 约束.点集引用
        << " | 可用性=" << 目标观察约束可用性文本(判定.状态)
        << " | 可强匹配=" << (判定.可强匹配 ? 1 : 0)
        << " | 可降级匹配=" << (判定.可降级匹配 ? 1 : 0)
        << " | 原因=" << 判定.原因;
    return 输出.str();
}

// 功能：从指定来源读取数据或状态。
std::optional<结构_外设观察报告队列项> 读取最新外设观察报告(
    const std::string& 外设ID,
    枚举_外设观察报告类型 报告类型)
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    外设观察报告_清理已加锁(状态, 当前时间);
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

// 功能：从指定来源读取数据或状态。
std::optional<结构_外设观察报告队列项> 读取外设观察报告_按ID(
    std::uint64_t 报告ID)
{
    if (报告ID == 0) {
        return std::nullopt;
    }
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    外设观察报告_清理已加锁(状态, 当前时间);
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
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
    std::vector<结构_外设观察报告队列项> 输出{};
    输出.reserve(状态.报告队列.size());
    for (const auto& 报告项 : 状态.报告队列) {
        if (!包含已消费 && 报告项.已消费) {
            continue;
        }
        if (!外设ID.empty() && 报告项.来源外设 != 外设ID) {
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

// 功能：等待线程、任务、外设或条件变化。
std::optional<结构_外设观察报告队列项> 按等待项匹配外设观察报告(
    const 结构_外设观察等待项& 等待项)
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    外设观察报告_清理已加锁(状态, 当前时间);
    for (auto 迭代器 = 状态.报告队列.rbegin(); 迭代器 != 状态.报告队列.rend(); ++迭代器) {
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
    return 外设观察报告_清理已加锁(状态, 当前时间毫秒);
}

// 功能：从指定来源读取数据或状态。
结构_外设观察队列状态 读取外设观察队列状态()
{
    auto& 状态 = 外设观察队列状态();
    const auto 当前时间 = 外设观察当前时间毫秒();
    std::lock_guard<std::mutex> 锁(状态.互斥);
    外设观察报告_清理已加锁(状态, 当前时间);
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
    状态.等待项集合.clear();
    状态.目标观察约束缓冲区.clear();
    状态.最新报告ID = 0;
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
        if (!簇.ROI引用.empty()
            || !簇.点集引用.empty()
            || !簇.空间候选引用.empty()
            || !簇.像素归属验证材料引用.empty()) {
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
        << " | 来源外设=" << (报告项.来源外设.empty() ? "空" : 报告项.来源外设)
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
        << " | 空间候选队列引用=" << (报告项.空间候选队列引用.empty() ? 0 : 1)
        << " | 像素归属账本引用=" << (报告项.像素归属账本引用.empty() ? 0 : 1)
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
    if (报告项.目标观察约束ID != 0
        || !报告项.约束generation.empty()
        || 报告项.目标约束使用状态 != 0) {
        输出 << " | 目标观察约束ID=" << 报告项.目标观察约束ID
            << " | 目标约束使用状态=" << 报告项.目标约束使用状态
            << " | 约束generation=" << (报告项.约束generation.empty() ? "空" : 报告项.约束generation)
            << " | 目标特征generation=" << (报告项.目标特征generation.empty() ? "空" : 报告项.目标特征generation)
            << " | 来源材料generation=" << (报告项.来源材料generation.empty() ? "空" : 报告项.来源材料generation)
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
            << " | 轨迹状态=" << 外设Tracker轨迹状态文本(报告项.轨迹状态)
            << " | 轨迹连续帧数=" << 报告项.轨迹状态连续帧数
            << " | 轨迹状态generation=" << (报告项.轨迹状态generation.empty() ? "空" : 报告项.轨迹状态generation)
            << " | 轨迹变化原因=" << (报告项.轨迹状态变化原因.empty() ? "空" : 报告项.轨迹状态变化原因)
            << " | 轨迹动态候选=" << (报告项.轨迹状态变化动态候选.empty() ? "空" : 报告项.轨迹状态变化动态候选)
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
        if (!簇.ROI引用.empty()) {
            ++ROI句柄数量;
        }
        if (!簇.像素集合掩码句柄.empty()) {
            ++掩码句柄数量;
        }
        if (!簇.点集引用.empty()) {
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
        << " | 来源外设=" << (报告项.来源外设.empty() ? "空" : 报告项.来源外设)
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
        << " | 空间候选队列=" << (报告项.空间候选队列引用.empty() ? "空" : 报告项.空间候选队列引用)
        << " | 像素归属账本=" << (报告项.像素归属账本引用.empty() ? "空" : 报告项.像素归属账本引用)
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
    std::size_t 空间候选引用数量 = 0;
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
        if (!簇.空间候选引用.empty()) {
            ++空间候选引用数量;
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
        << " | 来源外设=" << (报告项.来源外设.empty() ? "空" : 报告项.来源外设)
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
        << " | 空间候选引用=" << 空间候选引用数量
        << " | 跨帧稳定簇=" << 报告项.跨帧稳定观察像素簇数量
        << " | 轨迹稳定簇=" << 轨迹稳定簇数量
        << " | 待发布簇=" << 待发布簇数量
        << " | 像素归属账本=" << (包->像素归属账本.empty() ? "空" : 包->像素归属账本)
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
        if (!结果.当前ROI.empty()) {
            ++ROI句柄数量;
        }
        if (!结果.当前掩码.empty()) {
            ++掩码句柄数量;
        }
        if (!结果.当前点集.empty()) {
            ++点集句柄数量;
        }
        if (!结果.当前深度切片.empty()) {
            ++深度切片句柄数量;
        }
        已知存在结果缺口数量 += 结果.原因.size();
    }

    const bool 首帧基准 =
        报告项.差异摘要.find("缺少上一帧逐簇识别基准") != std::string::npos;
    const auto 发现候选簇数量 = 首帧基准 ? 报告项.观察像素簇集合.size() : std::size_t{0};

    std::ostringstream 输出;
    输出 << "D455扫描变化样本卡"
        << " | 报告ID=" << 报告项.报告ID
        << " | 来源外设=" << (报告项.来源外设.empty() ? "空" : 报告项.来源外设)
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
    if (!报告项.差异摘要.empty()) {
        输出 << " | 变化摘要=" << 报告项.差异摘要;
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
        << " | 目标外设=" << (等待项.目标外设.empty() ? "空" : 等待项.目标外设)
        << " | 观察运行模式=" << 外设观察运行模式文本(等待项.观察运行模式)
        << " | 本帧处理模式=" << 外设分割处理模式文本(等待项.本帧处理模式)
        << " | 本帧处理优先级=" << 等待项.本帧处理优先级
        << " | 本帧处理掩码=" << (等待项.本帧处理掩码句柄.empty() ? "空" : 等待项.本帧处理掩码句柄)
        << " | 期望报告=" << 外设观察报告类型文本(等待项.期望报告类型)
        << " | 最大年龄毫秒=" << 等待项.最大允许报告年龄毫秒
        << " | 最小稳定性=" << 等待项.最小稳定性要求
        << " | 允许缓存=" << (等待项.是否允许缓存报告 ? 1 : 0);
    if (!等待项.目标区域或目标簇.empty()) {
        输出 << " | 目标区域或目标簇=" << 等待项.目标区域或目标簇;
    }
    输出
        << " | 目标观察约束ID=" << 等待项.目标观察约束ID
        << " | 目标存在ID=" << (等待项.目标存在ID.empty() ? "空" : 等待项.目标存在ID)
        << " | 目标特征类型ID=" << (等待项.目标特征类型ID.empty() ? "空" : 等待项.目标特征类型ID)
        << " | 目标特征当前值类型=" << static_cast<int>(等待项.目标特征当前值类型)
        << " | 目标特征当前值=" << 等待项.目标特征当前值
        << " | 目标特征当前值句柄=" << 等待项.目标特征当前值句柄
        << " | 约束generation=" << 等待项.约束generation
        << " | 目标特征generation=" << 等待项.目标特征generation
        << " | 来源材料generation=" << 等待项.来源材料generation
        << " | 目标约束有效截止时间毫秒=" << 等待项.目标约束有效截止时间毫秒
        << " | 允许降级匹配=" << 等待项.允许降级匹配;
    if (!等待项.幂等键.empty()) {
        输出 << " | 幂等键=" << 等待项.幂等键;
    }
    if (!等待项.质量要求摘要.empty()) {
        输出 << " | 质量要求=" << 等待项.质量要求摘要;
    }
    return 输出.str();
}
