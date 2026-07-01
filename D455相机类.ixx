module;

// 文件头部规则注释模块：
// 1. 本模块只封装 D455 相机采集能力，不承载世界树真值、需求树或价值结算。
// 2. 轻量彩色视频帧只作为外设中间层供料和控制面板显示材料，不替代观察事实提交链。
// 3. 完整观察帧采集仍负责深度、点云和轮廓材料；不得把视频快照当作空间候选或场景事实。

#include <cstdint>
#include <memory>
#include <vector>

#include "基础数据类型.h"

export module D455相机模块;

import 相机接口模块;

export struct 结构体_D455彩色视频帧 {
    int 宽度 = 0;
    int 高度 = 0;
    std::uint64_t 系统到达时间_us = 0;
    std::uint64_t 设备时间_us = 0;
    std::uint8_t 时间域 = 0;
    std::uint32_t 深度帧号 = 0;
    std::uint32_t 彩色帧号 = 0;
    std::vector<std::uint8_t> 颜色RGB{};
};

export class D455_相机实现 : public 抽象深度相机接口 {
public:
    struct 配置项 {
        int 深度宽 = 640;
        int 深度高 = 480;
        int 彩色宽 = 640;
        int 彩色高 = 480;
        int 帧率 = 30;

        bool 启用红外双目 = false;
        int 红外宽 = 640;
        int 红外高 = 480;

        bool 彩色_自动曝光 = true;
        bool 彩色_自动白平衡 = true;
        double 彩色_曝光 = 8000.0;
        double 彩色_增益 = 64.0;
        double 彩色_白平衡 = 4500.0;

        bool 深度_启用发射器 = true;
        double 深度_激光功率 = 150.0;

        bool 启用视差域处理 = true;
        bool 启用空间滤波 = true;
        double 空间_平滑系数 = 0.5;
        double 空间_平滑阈值 = 20.0;
        double 空间_孔洞填充 = 0.0;

        bool 启用时间滤波 = true;
        double 时间_平滑系数 = 0.4;
        double 时间_平滑阈值 = 20.0;
        double 时间_持久性 = 3.0;

        bool 启用填洞滤波 = true;
        double 填洞_模式 = 1.0;

        bool 启用降采样 = false;
        double 降采样_倍率 = 1.0;

        bool 启用轮廓提取 = true;

        int 背景学习帧数 = 30;
        double 背景_更新系数 = 0.01;

        // 配置沿用 RealSense 常用米制口径，输出帧统一转换为当前工程约定的毫米。
        double 前景_深度差阈值_m = 0.06;
        double 前景_最小深度_m = 0.15;
        double 前景_最大深度_m = 4.0;

        int 轮廓_最小像素数 = 400;
        int 轮廓_最大数量 = 32;
        int 轮廓_形态学半径 = 1;

        bool 轮廓_输出边界点 = false;
        bool 轮廓_输出裁剪颜色 = true;
        bool 轮廓_输出原始掩膜 = true;
    };

    explicit D455_相机实现(配置项 cfg = {});
    ~D455_相机实现() override;

    D455_相机实现(D455_相机实现&&) noexcept;
    D455_相机实现& operator=(D455_相机实现&&) noexcept;
    D455_相机实现(const D455_相机实现&) = delete;
    D455_相机实现& operator=(const D455_相机实现&) = delete;

    bool 打开() override;
    void 关闭() override;
    bool 采集一帧(结构体_原始场景帧& 输出) override;
    bool 采集彩色视频帧(结构体_D455彩色视频帧& 输出);

    const 配置项& 获取配置() const;
    bool 应用配置(const 配置项& 新配置);
    bool 采集一帧并提取轮廓(结构体_原始场景帧& 输出, std::vector<结构体_轮廓观测>& out轮廓);
    const std::vector<结构体_轮廓观测>& 获取最近轮廓观测() const;

private:
    class 实现;
    std::unique_ptr<实现> 实现指针;
};
