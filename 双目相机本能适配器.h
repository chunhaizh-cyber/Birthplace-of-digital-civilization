#pragma once

// 文件头部规则注释模块：
// 1. 本头文件只声明双目相机外设适配器对外调用结果和控制入口，不承载世界树真值、需求或价值结算。
// 2. 调用结果中的文本只用于错误和诊断说明；业务判断必须使用结构化数值、状态和项目承载结构。
// 3. 高频视频入队可使用单帧采集参数，旧单帧诊断入口默认保留多帧融合口径。

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

namespace 双目相机本能适配器 {
    enum class 失败原因 : std::uint8_t {
        无 = 0,
        运行时缺失,
        打开失败,
        不可用,
        采集失败,
    };

    struct RGB像素 {
        std::uint8_t R = 0;
        std::uint8_t G = 0;
        std::uint8_t B = 0;
    };

    struct 彩色视频帧 {
        bool 成功 = false;
        bool 相机已打开 = false;
        失败原因 原因 = 失败原因::无;
        std::string 消息{};
        int 宽度 = 0;
        int 高度 = 0;
        std::uint64_t 系统到达时间_us = 0;
        std::uint64_t 设备时间_us = 0;
        std::uint8_t 时间域 = 0;
        std::uint32_t 深度帧号 = 0;
        std::uint32_t 彩色帧号 = 0;
        std::int64_t 预期像素数量 = 0;
        std::vector<std::uint8_t> 颜色RGB{};
    };

    struct 空间坐标毫米 {
        std::int64_t X = 0;
        std::int64_t Y = 0;
        std::int64_t Z = 0;
    };

    struct 空间候选摘要 {
        std::int64_t 候选编号 = 0;
        std::int64_t 像素数量 = 0;
        std::int64_t 有效点比例 = 0;
        std::int64_t 中心X = 0;
        std::int64_t 中心Y = 0;
        std::int64_t 中心Z = 0;
        std::int64_t 范围最小X = 0;
        std::int64_t 范围最大X = 0;
        std::int64_t 范围最小Y = 0;
        std::int64_t 范围最大Y = 0;
        std::int64_t 范围最小Z = 0;
        std::int64_t 范围最大Z = 0;
        std::int64_t 投影最小X = 0;
        std::int64_t 投影最大X = 0;
        std::int64_t 投影最小Y = 0;
        std::int64_t 投影最大Y = 0;
        std::int64_t 平均深度 = 0;
        std::int64_t 最近深度 = 0;
        std::int64_t 最远深度 = 0;
        std::int64_t 空间连续性评分 = 0;
        std::int64_t 范围稳定性评分 = 0;
        std::int64_t 空间覆盖像素数 = 0;
        std::int64_t 空间匹配像素数 = 0;
        std::int64_t 空间冲突像素数 = 0;
        std::int64_t 轮廓支持像素数 = 0;
        std::int64_t 轮廓冲突像素数 = 0;
        std::int64_t 深度一致率 = 0;
        std::int64_t 范围内点比例 = 0;
        std::int64_t 投影覆盖率 = 0;
        std::int64_t 轮廓吻合率 = 0;
        std::int64_t 轮廓来源类型 = 0;
        std::int64_t 轮廓闭合度 = 0;
        std::int64_t 轮廓深度支持率 = 0;
        std::int64_t 轮廓颜色支持率 = 0;
        std::int64_t 轮廓空间支持率 = 0;
        std::int64_t 轮廓跨越深度断裂 = 0;
        std::int64_t 轮廓穿过深度无效区域 = 0;
        std::int64_t 轮廓依赖补全深度 = 0;
        std::int64_t 轮廓置信度 = 0;
        std::int64_t 遮挡像素数 = 0;
        std::int64_t 未解释像素数 = 0;
        std::int64_t 像素归属验证状态 = 0;
        std::int64_t 观察确认补偿状态 = 0;
        std::int64_t 缺失原因 = 0;
        std::int64_t 未解释区域状态 = 0;
        std::int64_t 待补观察区域数量 = 0;
        std::int64_t 补全候选状态 = 0;
        std::int64_t 低置信轮廓状态 = 0;
        std::int64_t 补观察需求建议 = 0;
        std::vector<std::uint32_t> 像素索引集合{};
    };

    struct 诊断区域摘要 {
        std::int64_t 区域编号 = 0;
        std::int64_t 图层类型 = 0;
        std::int64_t 来源候选编号 = -1;
        std::int64_t 投影最小X = 0;
        std::int64_t 投影最大X = 0;
        std::int64_t 投影最小Y = 0;
        std::int64_t 投影最大Y = 0;
        std::int64_t 像素数量 = 0;
        std::int64_t 置信度 = 0;
        std::int64_t 掩码状态 = 0;
    };

    struct 调用结果 {
        bool 成功 = false;
        bool 相机已打开 = false;
        失败原因 原因 = 失败原因::无;
        std::string 消息{};
        int 宽度 = 0;
        int 高度 = 0;
        std::uint64_t 系统到达时间_us = 0;
        std::uint64_t 设备时间_us = 0;
        std::uint8_t 时间域 = 0;
        std::uint32_t 深度帧号 = 0;
        std::uint32_t 彩色帧号 = 0;
        std::int64_t 预期像素数量 = 0;
        std::int64_t 像素特征数量 = 0;
        std::int64_t 颜色像素数量 = 0;
        std::int64_t 深度有效像素数量 = 0;
        std::int64_t 点云有效像素数量 = 0;
        std::int64_t 像素覆盖状态 = 0;
        std::int64_t 空间坐标单位毫米 = 1;
        std::vector<RGB像素> 颜色RGB{};
        std::vector<std::int64_t> 原始深度毫米{};
        std::vector<std::int64_t> 滤波深度毫米{};
        std::vector<std::int64_t> 补全深度毫米{};
        std::vector<std::int64_t> 融合深度毫米{};
        std::vector<std::uint8_t> 深度有效性Mask{};
        std::vector<std::uint8_t> 融合深度有效性{};
        // 0=无有效深度，1=原始有效深度，2=滤波深度，3=补全深度。
        std::vector<std::uint8_t> 深度来源{};
        std::vector<std::int64_t> 深度稳定性{};
        std::vector<std::int64_t> 深度邻域一致性{};
        std::vector<std::int64_t> 多帧深度稳定性{};
        std::vector<std::int64_t> 每像素深度方差{};
        std::vector<空间坐标毫米> 空间坐标毫米XYZ{};
        std::int64_t 颜色RGB结构状态 = 0;
        std::int64_t 原始深度毫米结构状态 = 0;
        std::int64_t 滤波深度毫米结构状态 = 0;
        std::int64_t 补全深度毫米结构状态 = 0;
        std::int64_t 融合深度毫米结构状态 = 0;
        std::int64_t 深度有效性Mask状态 = 0;
        std::int64_t 融合深度有效性结构状态 = 0;
        std::int64_t 深度来源结构状态 = 0;
        std::int64_t 深度稳定性结构状态 = 0;
        std::int64_t 深度邻域一致性结构状态 = 0;
        std::int64_t 多帧深度稳定性结构状态 = 0;
        std::int64_t 每像素深度方差结构状态 = 0;
        std::int64_t 空间坐标毫米XYZ结构状态 = 0;
        std::int64_t 原始深度来源像素数量 = 0;
        std::int64_t 滤波深度来源像素数量 = 0;
        std::int64_t 补全深度来源像素数量 = 0;
        std::int64_t 无有效深度来源像素数量 = 0;
        std::int64_t 补全深度低置信像素数量 = 0;
        std::int64_t 深度稳定性平均值 = 0;
        std::int64_t 深度邻域一致性平均值 = 0;
        std::int64_t 观察帧组状态 = 0;
        std::int64_t 观察帧组帧数 = 0;
        std::int64_t 融合依据帧集合状态 = 0;
        std::int64_t 融合深度有效像素数量 = 0;
        std::int64_t 融合深度有效率 = 0;
        std::int64_t 融合补偿深度空洞像素数量 = 0;
        std::int64_t 融合低稳定像素数量 = 0;
        std::int64_t 融合深度稳定性平均值 = 0;
        std::int64_t 融合深度平均方差 = 0;
        std::int64_t 彩色深度已对齐 = 0;
        std::int64_t 对齐目标 = 0;
        std::int64_t 深度有效率 = 0;
        std::int64_t 空间坐标有效率 = 0;
        std::int64_t 帧质量评分 = 0;
        std::int64_t 深度空洞数量 = 0;
        std::int64_t 深度空洞区域数量 = 0;
        std::int64_t 深度空洞区域集合状态 = 0;
        std::int64_t 彩色深度对齐状态 = 0;
        std::int64_t 轮廓闭合率 = 0;
        std::int64_t 轮廓断裂数量 = 0;
        std::int64_t 彩图轮廓数量 = 0;
        std::int64_t 深度轮廓数量 = 0;
        std::int64_t 空间投影轮廓数量 = 0;
        std::int64_t 融合轮廓数量 = 0;
        std::int64_t 轮廓来源分层状态 = 0;
        std::int64_t 轮廓深度支持率 = 0;
        std::int64_t 轮廓颜色支持率 = 0;
        std::int64_t 轮廓空间支持率 = 0;
        std::int64_t 融合轮廓置信度 = 0;
        std::int64_t 跨越深度断裂轮廓数量 = 0;
        std::int64_t 穿过深度无效区域轮廓数量 = 0;
        std::int64_t 依赖补全深度轮廓数量 = 0;
        std::int64_t 空间点有效率 = 0;
        std::int64_t 边界深度稳定率 = 0;
        std::int64_t 候选存在验证通过率 = 0;
        std::int64_t 未解释区域比例 = 0;
        std::int64_t 补观察缺口状态 = 0;
        std::int64_t 补观察缺失原因 = 0;
        std::int64_t 待补观察区域数量 = 0;
        std::int64_t 补全候选数量 = 0;
        std::int64_t 低置信轮廓数量 = 0;
        std::int64_t 条件不足候选数量 = 0;
        std::int64_t 待验证候选数量 = 0;
        std::int64_t 部分确认候选数量 = 0;
        std::int64_t 补观察需求建议 = 0;
        std::int64_t 诊断区域数量 = 0;
        std::int64_t 诊断区域集合状态 = 0;
        std::int64_t 诊断区域掩码状态 = 0;
        std::int64_t 空间候选数量 = 0;
        std::int64_t 空间候选有效点数量 = 0;
        std::vector<空间候选摘要> 空间候选列表{};
        std::vector<诊断区域摘要> 诊断区域列表{};
        std::size_t 轮廓数量 = 0;
    };

    调用结果 打开(bool 使用低延迟观察配置 = false) noexcept;
    调用结果 释放() noexcept;
    调用结果 检查() noexcept;
    彩色视频帧 采集彩色视频帧() noexcept;
    调用结果 采集一帧轻量报告() noexcept;
    调用结果 采集一帧(
        std::uint32_t 融合目标帧数 = 3,
        bool 提取空间候选 = true) noexcept;
}
