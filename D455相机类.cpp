module;

// 文件头部规则注释模块：
// 1. 本文件只实现 D455 相机采集和本地帧处理，不写世界树真值、需求树或价值结算。
// 2. 轻量彩色视频采集只输出 RGB 帧和帧号时间戳，用于外设中间层视频供料。
// 3. 完整观察采集负责深度、点云和轮廓材料；两条路径不得互相冒充业务事实。

#include <librealsense2/rs2.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "基础数据类型.h"
#include "预处理开关变量.h"

module D455相机模块;

import 相机接口模块;

#if 鱼巢_开关_启用控制台输出
#define 鱼巢_D455控制台输出(表达式) do { 表达式; } while (false)
#else
#define 鱼巢_D455控制台输出(表达式) do { } while (false)
#endif

#pragma pack(push, 16)
class D455_相机实现::实现 {
public:
    using 配置项 = D455_相机实现::配置项;

    explicit 实现(配置项 cfg = {})
        : cfg(cfg),
        对齐器(RS2_STREAM_COLOR),
        深度到视差(true),
        视差到深度(false) {
    }

    // 功能：释放或收束对象资源。
    ~实现() {
        关闭();
    }

    // 功能：按函数名执行对应处理。
    bool 打开() {
        try {
            rs2::config c;
            c.enable_stream(RS2_STREAM_DEPTH, cfg.深度宽, cfg.深度高, RS2_FORMAT_Z16, cfg.帧率);
            if (cfg.启用彩色流) {
                c.enable_stream(RS2_STREAM_COLOR, cfg.彩色宽, cfg.彩色高, RS2_FORMAT_ANY, cfg.帧率);
            }
            if (cfg.启用红外双目) {
                c.enable_stream(RS2_STREAM_INFRARED, 1, cfg.红外宽, cfg.红外高, RS2_FORMAT_Y8, cfg.帧率);
                c.enable_stream(RS2_STREAM_INFRARED, 2, cfg.红外宽, cfg.红外高, RS2_FORMAT_Y8, cfg.帧率);
            }

            profile = 管道.start(c);
            已打开 = true;

            auto 深度profile = profile.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>();
            深度内参 = 深度profile.get_intrinsics();

            rs2::device dev = profile.get_device();
            if (auto ds = dev.first<rs2::depth_sensor>(); ds) {
                深度尺度 = ds.get_depth_scale();
            }

            配置传感器(dev);
            配置滤波链();
            return true;
        }
        catch (const rs2::error& e) {
            鱼巢_D455控制台输出(std::cerr << "D455 打开失败: " << e.what() << std::endl);
            已打开 = false;
            return false;
        }
    }

    // 功能：按函数名执行对应处理。
    void 关闭() {
        if (!已打开) return;

        try {
            管道.stop();
        }
        catch (...) {
        }

        已打开 = false;
    }

    // 功能：按函数名执行对应处理。
    bool 采集一帧(结构体_原始场景帧& 输出) {
        if (!已打开) return false;
        if (!cfg.启用彩色流) return false;

        try {
            rs2::frameset frames = 管道.wait_for_frames();
            rs2::frameset aligned = 对齐器.process(frames);

            rs2::depth_frame rawDepth = aligned.get_depth_frame();
            rs2::video_frame color = aligned.get_color_frame();
            if (!rawDepth || !color) return false;

            rs2::frame filtered = rawDepth;
            if (cfg.启用降采样) {
                filtered = 降采样滤波.process(filtered);
            }
            if (cfg.启用视差域处理) {
                filtered = 深度到视差.process(filtered);
            }
            if (cfg.启用空间滤波) {
                filtered = 空间滤波.process(filtered);
            }
            if (cfg.启用时间滤波) {
                filtered = 时间滤波.process(filtered);
            }
            if (cfg.启用视差域处理) {
                filtered = 视差到深度.process(filtered);
            }

            rs2::depth_frame filteredDepth = filtered.as<rs2::depth_frame>();
            rs2::depth_frame filledDepth = filteredDepth;
            if (cfg.启用填洞滤波) {
                filledDepth = 填洞滤波.process(filtered).as<rs2::depth_frame>();
            }

            const bool 有非填洞滤波 = cfg.启用降采样 || cfg.启用空间滤波 || cfg.启用时间滤波;

            const int w = rawDepth.get_width();
            const int h = rawDepth.get_height();
            const std::size_t N = static_cast<std::size_t>(w) * static_cast<std::size_t>(h);

            输出 = {};
            输出.时间戳.系统到达时间_us = 结构体_时间戳::当前_微秒();
            输出.时间戳.设备时间_us = static_cast<std::uint64_t>(std::max(0.0, rawDepth.get_timestamp()) * 1000.0);
            输出.时间戳.域 = 转换时间域(rawDepth.get_frame_timestamp_domain());
            输出.时间戳.深度帧号 = static_cast<std::uint32_t>(rawDepth.get_frame_number());
            输出.时间戳.彩色帧号 = static_cast<std::uint32_t>(color.get_frame_number());
            输出.宽度 = w;
            输出.高度 = h;
            const auto 当前深度Profile = rawDepth.get_profile().as<rs2::video_stream_profile>();
            深度内参 = 当前深度Profile.get_intrinsics();
            输出.深度内参 = 结构体_相机内参{ 深度内参.fx, 深度内参.fy, 深度内参.ppx, 深度内参.ppy, w, h, true };
            输出.深度已对齐到彩色 = color.get_width() == w && color.get_height() == h;
            输出.深度单位_mm = 深度尺度 * 1000.0;

            输出.点云.assign(N, Vector3D{ 0, 0, 0 });

            if (!复制深度帧毫米(rawDepth, w, h, 输出.原始深度, 输出.原始深度有效)) {
                return false;
            }
            输出.深度 = 输出.原始深度;
            输出.深度有效 = 输出.原始深度有效;
            if (有非填洞滤波) {
                (void)复制深度帧毫米(filteredDepth, w, h, 输出.滤波深度, 输出.滤波深度有效);
            }
            if (cfg.启用填洞滤波) {
                (void)复制深度帧毫米(filledDepth, w, h, 输出.补全深度, 输出.补全深度有效);
            }

            if (!读取对齐彩色(color, 输出)) {
                return false;
            }
            if (cfg.启用红外双目) {
                读取双目红外(frames, 输出);
            }
            生成点云(输出);

            最近轮廓.clear();
            if (cfg.启用轮廓提取) {
                提取轮廓(输出, 最近轮廓);
                输出.轮廓观测列表 = 最近轮廓;
            }

            return true;
        }
        catch (const rs2::error& e) {
            鱼巢_D455控制台输出(std::cerr << "采集一帧失败: " << e.what() << std::endl);
            return false;
        }
    }

    // 功能：读取 D455 当前彩色视频帧。
    bool 采集彩色视频帧(结构体_D455彩色视频帧& 输出) {
        if (!已打开) return false;
        if (!cfg.启用彩色流) return false;

        try {
            rs2::frameset frames = 管道.wait_for_frames();
            rs2::video_frame color = frames.get_color_frame();
            if (!color) return false;

            输出 = {};
            输出.宽度 = color.get_width();
            输出.高度 = color.get_height();
            输出.系统到达时间_us = 结构体_时间戳::当前_微秒();
            输出.设备时间_us = static_cast<std::uint64_t>(std::max(0.0, color.get_timestamp()) * 1000.0);
            输出.时间域 = static_cast<std::uint8_t>(转换时间域(color.get_frame_timestamp_domain()));
            输出.彩色帧号 = static_cast<std::uint32_t>(color.get_frame_number());
            if (auto depth = frames.get_depth_frame(); depth) {
                输出.深度帧号 = static_cast<std::uint32_t>(depth.get_frame_number());
            }

            return 复制彩色帧RGB(color, 输出.宽度, 输出.高度, 输出.颜色RGB);
        }
        catch (const rs2::error& e) {
            鱼巢_D455控制台输出(std::cerr << "采集彩色视频帧失败: " << e.what() << std::endl);
            return false;
        }
    }

    // 功能：采集一帧队列高频观察材料，保留彩色对齐和深度滤波链；队列所需空间坐标与像素簇由报告构建阶段生成。
    bool 采集轻量观察帧(结构体_原始场景帧& 输出) {
        if (!已打开) return false;

        try {
            结构体_D455轻量观察帧阶段耗时 本次耗时{};
            const auto 计算耗时 = [](auto 起点, auto 终点) noexcept -> std::uint64_t {
                return static_cast<std::uint64_t>(
                    std::chrono::duration_cast<std::chrono::microseconds>(终点 - 起点).count());
            };
            const auto 总开始 = std::chrono::steady_clock::now();
            auto 记录并失败 = [&]() noexcept {
                本次耗时.总耗时微秒 = 计算耗时(总开始, std::chrono::steady_clock::now());
                最近轻量观察帧阶段耗时 = 本次耗时;
                return false;
            };

            const auto 等帧开始 = std::chrono::steady_clock::now();
            rs2::frameset frames = 管道.wait_for_frames();
            const auto 等帧结束 = std::chrono::steady_clock::now();
            本次耗时.等帧耗时微秒 = 计算耗时(等帧开始, 等帧结束);

            const auto 对齐开始 = std::chrono::steady_clock::now();
            rs2::frameset processedFrames = cfg.启用彩色流
                ? 对齐器.process(frames)
                : frames;
            const auto 对齐结束 = std::chrono::steady_clock::now();
            本次耗时.对齐耗时微秒 = 计算耗时(对齐开始, 对齐结束);

            const auto 取帧开始 = std::chrono::steady_clock::now();
            rs2::depth_frame rawDepth = processedFrames.get_depth_frame();
            rs2::video_frame color = processedFrames.get_color_frame();
            const auto 取帧结束 = std::chrono::steady_clock::now();
            本次耗时.取帧耗时微秒 = 计算耗时(取帧开始, 取帧结束);
            if (!rawDepth) return 记录并失败();
            if (cfg.启用彩色流 && !color) return 记录并失败();

            const auto 滤波开始 = std::chrono::steady_clock::now();
            rs2::frame filtered = rawDepth;
            if (cfg.启用降采样) {
                filtered = 降采样滤波.process(filtered);
            }
            if (cfg.启用视差域处理) {
                filtered = 深度到视差.process(filtered);
            }
            if (cfg.启用空间滤波) {
                filtered = 空间滤波.process(filtered);
            }
            if (cfg.启用时间滤波) {
                filtered = 时间滤波.process(filtered);
            }
            if (cfg.启用视差域处理) {
                filtered = 视差到深度.process(filtered);
            }

            rs2::depth_frame filteredDepth = filtered.as<rs2::depth_frame>();
            rs2::depth_frame 输出深度 = filteredDepth;
            if (cfg.启用填洞滤波) {
                输出深度 = 填洞滤波.process(filtered).as<rs2::depth_frame>();
            }
            const auto 滤波结束 = std::chrono::steady_clock::now();
            本次耗时.滤波耗时微秒 = 计算耗时(滤波开始, 滤波结束);

            const int w = 输出深度.get_width();
            const int h = 输出深度.get_height();
            if (w <= 0 || h <= 0) return 记录并失败();

            const auto 元数据开始 = std::chrono::steady_clock::now();
            输出 = {};
            输出.时间戳.系统到达时间_us = 结构体_时间戳::当前_微秒();
            输出.时间戳.设备时间_us = static_cast<std::uint64_t>(
                std::max(0.0, 输出深度.get_timestamp()) * 1000.0);
            输出.时间戳.域 = 转换时间域(输出深度.get_frame_timestamp_domain());
            输出.时间戳.深度帧号 = static_cast<std::uint32_t>(输出深度.get_frame_number());
            if (color) {
                输出.时间戳.彩色帧号 = static_cast<std::uint32_t>(color.get_frame_number());
            }
            输出.宽度 = w;
            输出.高度 = h;

            const auto 当前深度Profile = 输出深度.get_profile().as<rs2::video_stream_profile>();
            深度内参 = 当前深度Profile.get_intrinsics();
            输出.深度内参 = 结构体_相机内参{
                深度内参.fx,
                深度内参.fy,
                深度内参.ppx,
                深度内参.ppy,
                w,
                h,
                true
            };
            输出.深度已对齐到彩色 = color && color.get_width() == w && color.get_height() == h;
            输出.深度单位_mm = 深度尺度 * 1000.0;
            const auto 元数据结束 = std::chrono::steady_clock::now();
            本次耗时.元数据耗时微秒 = 计算耗时(元数据开始, 元数据结束);

            const auto 彩色复制开始 = std::chrono::steady_clock::now();
            if (cfg.启用彩色流 && color) {
                if (!读取对齐彩色(color, 输出)) {
                    return 记录并失败();
                }
            }
            const auto 彩色复制结束 = std::chrono::steady_clock::now();
            本次耗时.彩色复制耗时微秒 = 计算耗时(彩色复制开始, 彩色复制结束);

            const auto 复制开始 = std::chrono::steady_clock::now();
            const bool 成功 = 复制深度帧毫米(输出深度, w, h, 输出.深度, 输出.深度有效);
            const auto 复制结束 = std::chrono::steady_clock::now();
            本次耗时.深度复制耗时微秒 = 计算耗时(复制开始, 复制结束);
            本次耗时.总耗时微秒 = 计算耗时(总开始, 复制结束);
            本次耗时.有效 = 成功;
            最近轻量观察帧阶段耗时 = 本次耗时;
            return 成功;
        }
        catch (const rs2::error& e) {
            鱼巢_D455控制台输出(std::cerr << "采集轻量观察帧失败: " << e.what() << std::endl);
            最近轻量观察帧阶段耗时 = {};
            return false;
        }
    }

    // 功能：按函数名执行对应处理。
    bool 采集一帧并提取轮廓(结构体_原始场景帧& 输出, std::vector<结构体_轮廓观测>& out轮廓) {
        if (!采集一帧(输出)) return false;
        out轮廓 = 最近轮廓;
        return true;
    }

    // 功能：读取并返回指定对象、状态或运行材料。
    const 配置项& 获取配置() const {
        return cfg;
    }

    // 功能：按函数名执行对应处理。
    bool 应用配置(const 配置项& 新配置) {
        const bool 需要重启 =
            cfg.深度宽 != 新配置.深度宽 ||
            cfg.深度高 != 新配置.深度高 ||
            cfg.彩色宽 != 新配置.彩色宽 ||
            cfg.彩色高 != 新配置.彩色高 ||
            cfg.启用彩色流 != 新配置.启用彩色流 ||
            cfg.启用红外双目 != 新配置.启用红外双目 ||
            cfg.红外宽 != 新配置.红外宽 ||
            cfg.红外高 != 新配置.红外高 ||
            cfg.帧率 != 新配置.帧率;

        if (!已打开) {
            cfg = 新配置;
            return true;
        }

        if (需要重启) {
            关闭();
            cfg = 新配置;
            return 打开();
        }

        cfg = 新配置;
        try {
            配置传感器(profile.get_device());
            配置滤波链();
            return true;
        }
        catch (const rs2::error& e) {
            鱼巢_D455控制台输出(std::cerr << "D455 应用配置失败: " << e.what() << std::endl);
            return false;
        }
    }

    // 功能：读取并返回指定对象、状态或运行材料。
    const std::vector<结构体_轮廓观测>& 获取最近轮廓观测() const {
        return 最近轮廓;
    }

    // 功能：读取最近一次轻量观察帧采集的诊断阶段耗时。
    结构体_D455轻量观察帧阶段耗时 读取最近轻量观察帧阶段耗时() const {
        return 最近轻量观察帧阶段耗时;
    }

private:
    bool 已打开 = false;
    配置项 cfg;

    rs2::pipeline 管道;
    rs2::pipeline_profile profile;
    rs2::align 对齐器;

    rs2_intrinsics 深度内参{};
    double 深度尺度 = 0.001;

    rs2::decimation_filter 降采样滤波;
    rs2::disparity_transform 深度到视差;
    rs2::spatial_filter 空间滤波;
    rs2::temporal_filter 时间滤波;
    rs2::disparity_transform 视差到深度;
    rs2::hole_filling_filter 填洞滤波;
    结构体_D455轻量观察帧阶段耗时 最近轻量观察帧阶段耗时{};

    std::vector<double> 背景深度;
    std::vector<double> 背景权重;
    bool 背景已建立 = false;
    int 背景累计帧 = 0;

    std::vector<double> 上一帧深度;
    std::vector<结构体_轮廓观测> 最近轮廓;

private:
    // 功能：按函数名执行对应处理。
    static inline std::size_t 索引(int u, int v, int w) {
        return static_cast<std::size_t>(v) * static_cast<std::size_t>(w) + static_cast<std::size_t>(u);
    }

    // 功能：按函数名执行对应处理。
    static inline bool 在范围内(int x, int a, int b) {
        return x >= a && x <= b;
    }

    // 功能：按函数名执行对应处理。
    bool 复制深度帧毫米(
        const rs2::depth_frame& depth,
        int expectedW,
        int expectedH,
        std::vector<double>& 深度输出,
        std::vector<std::uint8_t>& 有效输出) const {
        深度输出.clear();
        有效输出.clear();
        if (!depth || depth.get_width() != expectedW || depth.get_height() != expectedH) {
            return false;
        }

        const std::uint16_t* dp = static_cast<const std::uint16_t*>(depth.get_data());
        if (!dp) {
            return false;
        }

        const std::size_t N = static_cast<std::size_t>(expectedW) * static_cast<std::size_t>(expectedH);
        深度输出.assign(N, 0.0);
        有效输出.assign(N, 0);
        for (std::size_t i = 0; i < N; ++i) {
            const double mm = static_cast<double>(dp[i]) * 深度尺度 * 1000.0;
            深度输出[i] = mm;
            有效输出[i] = dp[i] != 0 ? 1 : 0;
        }
        return true;
    }

    // 功能：按函数名执行对应处理。
    static inline std::uint8_t clamp_u8(int x) {
        if (x < 0) return 0;
        if (x > 255) return 255;
        return static_cast<std::uint8_t>(x);
    }

    // 功能：把输入数据转换为目标类型、语义或结构。
    static 枚举_时间域 转换时间域(rs2_timestamp_domain domain) {
        switch (domain) {
        case RS2_TIMESTAMP_DOMAIN_HARDWARE_CLOCK:
            return 枚举_时间域::设备硬件时钟;
        case RS2_TIMESTAMP_DOMAIN_SYSTEM_TIME:
            return 枚举_时间域::系统时钟;
        case RS2_TIMESTAMP_DOMAIN_GLOBAL_TIME:
            return 枚举_时间域::全局时间;
        default:
            return 枚举_时间域::未知;
        }
    }

    // 功能：按函数名执行对应处理。
    static inline void yuv_to_rgb(std::uint8_t Y, std::uint8_t U, std::uint8_t V, std::uint8_t& R, std::uint8_t& G, std::uint8_t& B) {
        const int C = static_cast<int>(Y) - 16;
        const int D = static_cast<int>(U) - 128;
        const int E = static_cast<int>(V) - 128;

        const int r = (298 * C + 409 * E + 128) >> 8;
        const int g = (298 * C - 100 * D - 208 * E + 128) >> 8;
        const int b = (298 * C + 516 * D + 128) >> 8;

        R = clamp_u8(r);
        G = clamp_u8(g);
        B = clamp_u8(b);
    }

    // 功能：按函数名执行对应处理。
    void 配置传感器(rs2::device dev) {
        try {
            for (auto&& s : dev.query_sensors()) {
                bool isColor = false;
                try {
                    const char* name = s.get_info(RS2_CAMERA_INFO_NAME);
                    if (name && std::string(name).find("RGB") != std::string::npos) {
                        isColor = true;
                    }
                }
                catch (...) {
                }

                if (isColor && cfg.启用彩色流) {
                    if (s.supports(RS2_OPTION_ENABLE_AUTO_EXPOSURE)) {
                        s.set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE, cfg.彩色_自动曝光 ? 1.0f : 0.0f);
                    }
                    if (s.supports(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE)) {
                        s.set_option(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE, cfg.彩色_自动白平衡 ? 1.0f : 0.0f);
                    }
                    if (!cfg.彩色_自动曝光) {
                        if (s.supports(RS2_OPTION_EXPOSURE)) s.set_option(RS2_OPTION_EXPOSURE, static_cast<float>(cfg.彩色_曝光));
                        if (s.supports(RS2_OPTION_GAIN)) s.set_option(RS2_OPTION_GAIN, static_cast<float>(cfg.彩色_增益));
                    }
                    if (!cfg.彩色_自动白平衡 && s.supports(RS2_OPTION_WHITE_BALANCE)) {
                        s.set_option(RS2_OPTION_WHITE_BALANCE, static_cast<float>(cfg.彩色_白平衡));
                    }
                }

                if (s.is<rs2::depth_sensor>()) {
                    if (s.supports(RS2_OPTION_EMITTER_ENABLED)) {
                        s.set_option(RS2_OPTION_EMITTER_ENABLED, cfg.深度_启用发射器 ? 1.0f : 0.0f);
                    }
                    if (s.supports(RS2_OPTION_LASER_POWER)) {
                        s.set_option(RS2_OPTION_LASER_POWER, static_cast<float>(cfg.深度_激光功率));
                    }
                }
            }
        }
        catch (...) {
            // 设备选项差异不阻断采集，打开失败会在主路径返回。
        }
    }

    // 功能：按函数名执行对应处理。
    void 配置滤波链() {
        auto try_set = [](auto& f, rs2_option opt, double val) {
            try {
                if (f.supports(opt)) f.set_option(opt, static_cast<float>(val));
            }
            catch (...) {
            }
            };

        try_set(降采样滤波, RS2_OPTION_FILTER_MAGNITUDE, cfg.降采样_倍率);
        try_set(空间滤波, RS2_OPTION_FILTER_SMOOTH_ALPHA, cfg.空间_平滑系数);
        try_set(空间滤波, RS2_OPTION_FILTER_SMOOTH_DELTA, cfg.空间_平滑阈值);
        try_set(空间滤波, RS2_OPTION_HOLES_FILL, cfg.空间_孔洞填充);
        try_set(时间滤波, RS2_OPTION_FILTER_SMOOTH_ALPHA, cfg.时间_平滑系数);
        try_set(时间滤波, RS2_OPTION_FILTER_SMOOTH_DELTA, cfg.时间_平滑阈值);
        try_set(时间滤波, RS2_OPTION_HOLES_FILL, cfg.时间_持久性);
        try_set(填洞滤波, RS2_OPTION_HOLES_FILL, cfg.填洞_模式);
    }

    // 功能：读取 D455 彩色帧并转换为连续 RGB 字节。
    bool 复制彩色帧RGB(
        const rs2::video_frame& color,
        int w,
        int h,
        std::vector<std::uint8_t>& 输出RGB) const {
        const int cw = color.get_width();
        const int ch = color.get_height();
        输出RGB.clear();
        if (cw != w || ch != h || w <= 0 || h <= 0) return false;

        const rs2_format fmt = color.get_profile().format();
        const int bpp = color.get_bytes_per_pixel();
        const int stride = color.get_stride_in_bytes();
        const std::uint8_t* base = static_cast<const std::uint8_t*>(color.get_data());
        if (!base) return false;
        if (stride < w * bpp) return false;

        const std::size_t 像素数 = static_cast<std::size_t>(w) * static_cast<std::size_t>(h);
        输出RGB.assign(像素数 * 3, 0);

        auto write_rgb = [&](int u, int v, std::uint8_t R, std::uint8_t G, std::uint8_t B) {
            const auto 目标 = 索引(u, v, w) * 3;
            输出RGB[目标] = R;
            输出RGB[目标 + 1] = G;
            输出RGB[目标 + 2] = B;
            };

        if (fmt == RS2_FORMAT_RGB8 || fmt == RS2_FORMAT_BGR8 || fmt == RS2_FORMAT_RGBA8 || fmt == RS2_FORMAT_BGRA8) {
            for (int v = 0; v < h; ++v) {
                const std::uint8_t* row = base + static_cast<std::size_t>(v) * static_cast<std::size_t>(stride);
                for (int u = 0; u < w; ++u) {
                    const std::uint8_t* px = row + static_cast<std::size_t>(u) * static_cast<std::size_t>(bpp);
                    switch (fmt) {
                    case RS2_FORMAT_RGB8:  write_rgb(u, v, px[0], px[1], px[2]); break;
                    case RS2_FORMAT_BGR8:  write_rgb(u, v, px[2], px[1], px[0]); break;
                    case RS2_FORMAT_RGBA8: write_rgb(u, v, px[0], px[1], px[2]); break;
                    case RS2_FORMAT_BGRA8: write_rgb(u, v, px[2], px[1], px[0]); break;
                    default: break;
                    }
                }
            }
            return true;
        }

        if (fmt == RS2_FORMAT_YUYV && bpp == 2) {
            for (int v = 0; v < h; ++v) {
                const std::uint8_t* row = base + static_cast<std::size_t>(v) * static_cast<std::size_t>(stride);
                for (int u = 0; u < w; u += 2) {
                    if (u + 1 >= w) {
                        break;
                    }
                    const std::uint8_t* p = row + static_cast<std::size_t>(u) * 2;
                    const std::uint8_t Y0 = p[0], U = p[1], Y1 = p[2], V = p[3];
                    std::uint8_t R{}, G{}, B{};
                    yuv_to_rgb(Y0, U, V, R, G, B);
                    write_rgb(u, v, R, G, B);
                    yuv_to_rgb(Y1, U, V, R, G, B);
                    write_rgb(u + 1, v, R, G, B);
                }
            }
            return true;
        }

        if (fmt == RS2_FORMAT_UYVY && bpp == 2) {
            for (int v = 0; v < h; ++v) {
                const std::uint8_t* row = base + static_cast<std::size_t>(v) * static_cast<std::size_t>(stride);
                for (int u = 0; u < w; u += 2) {
                    if (u + 1 >= w) {
                        break;
                    }
                    const std::uint8_t* p = row + static_cast<std::size_t>(u) * 2;
                    const std::uint8_t U = p[0], Y0 = p[1], V = p[2], Y1 = p[3];
                    std::uint8_t R{}, G{}, B{};
                    yuv_to_rgb(Y0, U, V, R, G, B);
                    write_rgb(u, v, R, G, B);
                    yuv_to_rgb(Y1, U, V, R, G, B);
                    write_rgb(u + 1, v, R, G, B);
                }
            }
            return true;
        }

        输出RGB.clear();
        return false;
    }

    // 功能：读取 D455 彩色帧并直接转换为工程 Color 数组。
    bool 复制彩色帧Color(
        const rs2::video_frame& color,
        int w,
        int h,
        std::vector<Color>& 输出颜色) const {
        static_assert(sizeof(Color) == 3);
        const int cw = color.get_width();
        const int ch = color.get_height();
        输出颜色.clear();
        if (cw != w || ch != h || w <= 0 || h <= 0) return false;

        const rs2_format fmt = color.get_profile().format();
        const int bpp = color.get_bytes_per_pixel();
        const int stride = color.get_stride_in_bytes();
        const std::uint8_t* base = static_cast<const std::uint8_t*>(color.get_data());
        if (!base) return false;

        const std::size_t 像素数 = static_cast<std::size_t>(w) * static_cast<std::size_t>(h);
        输出颜色.resize(像素数);

        auto write_color = [&](int u, int v, std::uint8_t R, std::uint8_t G, std::uint8_t B) {
            输出颜色[索引(u, v, w)] = Color{ R, G, B };
            };

        if (fmt == RS2_FORMAT_RGB8 && bpp == 3) {
            for (int v = 0; v < h; ++v) {
                const std::uint8_t* row = base + static_cast<std::size_t>(v) * static_cast<std::size_t>(stride);
                std::memcpy(
                    输出颜色.data() + static_cast<std::size_t>(v) * static_cast<std::size_t>(w),
                    row,
                    static_cast<std::size_t>(w) * sizeof(Color));
            }
            return true;
        }

        if ((fmt == RS2_FORMAT_BGR8 && bpp == 3)
            || (fmt == RS2_FORMAT_RGBA8 && bpp == 4)
            || (fmt == RS2_FORMAT_BGRA8 && bpp == 4)) {
            for (int v = 0; v < h; ++v) {
                const std::uint8_t* row = base + static_cast<std::size_t>(v) * static_cast<std::size_t>(stride);
                for (int u = 0; u < w; ++u) {
                    const std::uint8_t* px = row + static_cast<std::size_t>(u) * static_cast<std::size_t>(bpp);
                    switch (fmt) {
                    case RS2_FORMAT_BGR8:  write_color(u, v, px[2], px[1], px[0]); break;
                    case RS2_FORMAT_RGBA8: write_color(u, v, px[0], px[1], px[2]); break;
                    case RS2_FORMAT_BGRA8: write_color(u, v, px[2], px[1], px[0]); break;
                    default: break;
                    }
                }
            }
            return true;
        }

        if (fmt == RS2_FORMAT_YUYV && bpp == 2) {
            for (int v = 0; v < h; ++v) {
                const std::uint8_t* row = base + static_cast<std::size_t>(v) * static_cast<std::size_t>(stride);
                for (int u = 0; u < w; u += 2) {
                    if (u + 1 >= w) {
                        break;
                    }
                    const std::uint8_t* p = row + static_cast<std::size_t>(u) * 2;
                    const std::uint8_t Y0 = p[0], U = p[1], Y1 = p[2], V = p[3];
                    std::uint8_t R{}, G{}, B{};
                    yuv_to_rgb(Y0, U, V, R, G, B);
                    write_color(u, v, R, G, B);
                    yuv_to_rgb(Y1, U, V, R, G, B);
                    write_color(u + 1, v, R, G, B);
                }
            }
            return true;
        }

        if (fmt == RS2_FORMAT_UYVY && bpp == 2) {
            for (int v = 0; v < h; ++v) {
                const std::uint8_t* row = base + static_cast<std::size_t>(v) * static_cast<std::size_t>(stride);
                for (int u = 0; u < w; u += 2) {
                    if (u + 1 >= w) {
                        break;
                    }
                    const std::uint8_t* p = row + static_cast<std::size_t>(u) * 2;
                    const std::uint8_t U = p[0], Y0 = p[1], V = p[2], Y1 = p[3];
                    std::uint8_t R{}, G{}, B{};
                    yuv_to_rgb(Y0, U, V, R, G, B);
                    write_color(u, v, R, G, B);
                    yuv_to_rgb(Y1, U, V, R, G, B);
                    write_color(u + 1, v, R, G, B);
                }
            }
            return true;
        }

        输出颜色.clear();
        return false;
    }

    // 功能：从指定来源读取已对齐彩色帧。
    bool 读取对齐彩色(const rs2::video_frame& color, 结构体_原始场景帧& out) {
        if (!复制彩色帧Color(color, out.宽度, out.高度, out.颜色)) {
            out.颜色.clear();
            return false;
        }
        const std::size_t 预期像素数 = out.宽度 > 0 && out.高度 > 0
            ? static_cast<std::size_t>(out.宽度) * static_cast<std::size_t>(out.高度)
            : 0;
        return out.颜色.size() == 预期像素数;
    }

    // 功能：按函数名执行对应处理。
    static void 复制红外Y8(const rs2::video_frame& ir, std::vector<std::uint8_t>& out, int expectedW, int expectedH) {
        out.clear();
        if (ir.get_width() != expectedW || ir.get_height() != expectedH) return;
        if (ir.get_profile().format() != RS2_FORMAT_Y8 || ir.get_bytes_per_pixel() != 1) return;

        const std::uint8_t* base = static_cast<const std::uint8_t*>(ir.get_data());
        if (!base) return;

        const int w = ir.get_width();
        const int h = ir.get_height();
        const int stride = ir.get_stride_in_bytes();
        out.resize(static_cast<std::size_t>(w) * static_cast<std::size_t>(h));
        for (int v = 0; v < h; ++v) {
            const std::uint8_t* row = base + static_cast<std::size_t>(v) * static_cast<std::size_t>(stride);
            std::memcpy(out.data() + static_cast<std::size_t>(v) * static_cast<std::size_t>(w), row, static_cast<std::size_t>(w));
        }
    }

    // 功能：从指定来源读取数据或状态。
    void 读取双目红外(const rs2::frameset& frames, 结构体_原始场景帧& out) {
        out.红外1.clear();
        out.红外2.clear();

        for (auto&& frame : frames) {
            rs2::video_frame vf = frame.as<rs2::video_frame>();
            if (!vf) continue;

            rs2::stream_profile sp = vf.get_profile();
            if (sp.stream_type() != RS2_STREAM_INFRARED) continue;

            const int streamIndex = sp.stream_index();
            if (streamIndex == 1) {
                复制红外Y8(vf, out.红外1, out.宽度, out.高度);
            }
            else if (streamIndex == 2) {
                复制红外Y8(vf, out.红外2, out.宽度, out.高度);
            }
        }
    }

    // 功能：根据当前输入生成目标数据、场景、动态或回执。
    void 生成点云(结构体_原始场景帧& 帧) {
        const double fx = 深度内参.fx;
        const double fy = 深度内参.fy;
        const double cx = 深度内参.ppx;
        const double cy = 深度内参.ppy;

        const int w = 帧.宽度;
        const int h = 帧.高度;
        if (fx == 0.0 || fy == 0.0) return;

        for (int v = 0; v < h; ++v) {
            for (int u = 0; u < w; ++u) {
                const std::size_t i = 索引(u, v, w);
                const double z = 帧.深度[i];
                if (z <= 0.0) {
                    帧.点云[i] = Vector3D{ 0, 0, 0 };
                    continue;
                }

                const double X = (static_cast<double>(u) - cx) * z / fx;
                const double Y = (static_cast<double>(v) - cy) * z / fy;
                帧.点云[i] = Vector3D{ X, Y, z };
            }
        }
    }

    // 功能：按函数名执行对应处理。
    static void 形态学_闭运算(std::vector<std::uint8_t>& m, int w, int h, int r) {
        if (r <= 0) return;

        std::vector<std::uint8_t> tmp(static_cast<std::size_t>(w) * static_cast<std::size_t>(h), 0);
        auto idx = [&](int u, int v) {
            return static_cast<std::size_t>(v) * static_cast<std::size_t>(w) + static_cast<std::size_t>(u);
            };

        for (int v = 0; v < h; ++v) {
            for (int u = 0; u < w; ++u) {
                bool on = false;
                for (int dv = -r; dv <= r && !on; ++dv) {
                    const int y = v + dv;
                    if (!在范围内(y, 0, h - 1)) continue;
                    for (int du = -r; du <= r; ++du) {
                        const int x = u + du;
                        if (!在范围内(x, 0, w - 1)) continue;
                        if (m[idx(x, y)] != 0) {
                            on = true;
                            break;
                        }
                    }
                }
                tmp[idx(u, v)] = on ? 1 : 0;
            }
        }

        for (int v = 0; v < h; ++v) {
            for (int u = 0; u < w; ++u) {
                bool on = true;
                for (int dv = -r; dv <= r && on; ++dv) {
                    const int y = v + dv;
                    if (!在范围内(y, 0, h - 1)) {
                        on = false;
                        break;
                    }
                    for (int du = -r; du <= r; ++du) {
                        const int x = u + du;
                        if (!在范围内(x, 0, w - 1) || tmp[idx(x, y)] == 0) {
                            on = false;
                            break;
                        }
                    }
                }
                m[idx(u, v)] = on ? 1 : 0;
            }
        }
    }

    // 功能：更新已有对象、状态、索引或缓存。
    void 更新背景模型(const 结构体_原始场景帧& 帧, const std::vector<std::uint8_t>* 前景掩膜) {
        const int w = 帧.宽度;
        const int h = 帧.高度;
        const std::size_t N = static_cast<std::size_t>(w) * static_cast<std::size_t>(h);

        if (背景深度.size() != N) {
            背景深度.assign(N, 0.0);
            背景权重.assign(N, 0.0);
            背景已建立 = false;
            背景累计帧 = 0;
        }

        if (!背景已建立) {
            for (std::size_t i = 0; i < N; ++i) {
                const double z = 帧.深度[i];
                if (z <= 0.0) continue;
                const double wgt = 背景权重[i];
                背景深度[i] = (背景深度[i] * wgt + z) / (wgt + 1.0);
                背景权重[i] = wgt + 1.0;
            }
            ++背景累计帧;
            if (背景累计帧 >= std::max(1, cfg.背景学习帧数)) {
                背景已建立 = true;
            }
            return;
        }

        const double a = std::max(0.0, std::min(1.0, cfg.背景_更新系数));
        if (a <= 0.0) return;

        for (std::size_t i = 0; i < N; ++i) {
            if (前景掩膜 && (*前景掩膜)[i]) continue;

            const double z = 帧.深度[i];
            if (z <= 0.0) continue;

            const double bg = 背景深度[i];
            背景深度[i] = bg <= 0.0 ? z : bg * (1.0 - a) + z * a;
        }
    }

    // 功能：根据当前输入生成目标数据、场景、动态或回执。
    void 生成前景掩膜(const 结构体_原始场景帧& 帧, std::vector<std::uint8_t>& outMask) {
        const int w = 帧.宽度;
        const int h = 帧.高度;
        const std::size_t N = static_cast<std::size_t>(w) * static_cast<std::size_t>(h);
        outMask.assign(N, 0);

        const double minZ = cfg.前景_最小深度_m * 1000.0;
        const double maxZ = cfg.前景_最大深度_m * 1000.0;
        const double th = cfg.前景_深度差阈值_m * 1000.0;

        const bool canUseBG = 背景已建立 && 背景深度.size() == N;
        const bool canUsePrev = !canUseBG && 上一帧深度.size() == N;

        for (std::size_t i = 0; i < N; ++i) {
            const double z = 帧.深度[i];
            if (!(z > 0.0 && z >= minZ && z <= maxZ)) continue;

            double ref = 0.0;
            if (canUseBG) ref = 背景深度[i];
            else if (canUsePrev) ref = 上一帧深度[i];

            bool fg = false;
            if (ref <= 0.0) {
                fg = canUseBG;
            }
            else {
                fg = std::fabs(z - ref) > th;
            }

            outMask[i] = fg ? 1 : 0;
        }

        形态学_闭运算(outMask, w, h, cfg.轮廓_形态学半径);

        if (canUseBG) {
            更新背景模型(帧, &outMask);
        }
        else {
            更新背景模型(帧, nullptr);
        }

        上一帧深度 = 帧.深度;
    }

    // 功能：按函数名执行对应处理。
    void 提取轮廓(结构体_原始场景帧& 帧, std::vector<结构体_轮廓观测>& out) {
        out.clear();

        const int w = 帧.宽度;
        const int h = 帧.高度;
        if (w <= 0 || h <= 0) return;

        std::vector<std::uint8_t> mask;
        生成前景掩膜(帧, mask);
        帧.前景提示 = mask;

        const std::size_t N = static_cast<std::size_t>(w) * static_cast<std::size_t>(h);
        std::vector<std::uint8_t> vis(N, 0);

        auto idx = [&](int u, int v) {
            return static_cast<std::size_t>(v) * static_cast<std::size_t>(w) + static_cast<std::size_t>(u);
            };

        std::queue<int> q;
        int found = 0;
        int seq = 0;

        for (int v0 = 0; v0 < h; ++v0) {
            for (int u0 = 0; u0 < w; ++u0) {
                const std::size_t i0 = idx(u0, v0);
                if (mask[i0] == 0 || vis[i0]) continue;

                int minx = u0, maxx = u0, miny = v0, maxy = v0;
                int count = 0;

                double minX = std::numeric_limits<double>::infinity();
                double minY = std::numeric_limits<double>::infinity();
                double minZ = std::numeric_limits<double>::infinity();
                double maxX = -std::numeric_limits<double>::infinity();
                double maxY = -std::numeric_limits<double>::infinity();
                double maxZ = -std::numeric_limits<double>::infinity();

                std::uint64_t sumR = 0, sumG = 0, sumB = 0;
                std::vector<int> pixels;
                pixels.reserve(2048);

                q.push(static_cast<int>(i0));
                vis[i0] = 1;

                while (!q.empty()) {
                    const int i = q.front();
                    q.pop();
                    const int u = i % w;
                    const int v = i / w;

                    ++count;
                    pixels.push_back(i);

                    minx = std::min(minx, u);
                    maxx = std::max(maxx, u);
                    miny = std::min(miny, v);
                    maxy = std::max(maxy, v);

                    const auto& P = 帧.点云[static_cast<std::size_t>(i)];
                    if (P.z > 0.0) {
                        minX = std::min(minX, P.x);
                        minY = std::min(minY, P.y);
                        minZ = std::min(minZ, P.z);
                        maxX = std::max(maxX, P.x);
                        maxY = std::max(maxY, P.y);
                        maxZ = std::max(maxZ, P.z);
                    }

                    const auto& C = 帧.颜色[static_cast<std::size_t>(i)];
                    sumR += C.r;
                    sumG += C.g;
                    sumB += C.b;

                    auto try_push = [&](int nu, int nv) {
                        if (!在范围内(nu, 0, w - 1) || !在范围内(nv, 0, h - 1)) return;
                        const std::size_t ni = idx(nu, nv);
                        if (vis[ni] || mask[ni] == 0) return;
                        vis[ni] = 1;
                        q.push(static_cast<int>(ni));
                        };

                    try_push(u - 1, v);
                    try_push(u + 1, v);
                    try_push(u, v - 1);
                    try_push(u, v + 1);
                }

                if (count < std::max(1, cfg.轮廓_最小像素数)) continue;
                if (!(std::isfinite(minX) && std::isfinite(maxX) && std::isfinite(minZ) && std::isfinite(maxZ))) {
                    continue;
                }

                结构体_轮廓观测 obs;
                obs.帧内序号 = seq++;
                obs.x = minx;
                obs.y = miny;
                obs.w = maxx - minx + 1;
                obs.h = maxy - miny + 1;
                obs.像素数 = count;
                obs.有效 = true;

                obs.中心 = Vector3D{ (minX + maxX) * 0.5, (minY + maxY) * 0.5, (minZ + maxZ) * 0.5 };
                obs.尺寸 = Vector3D{ std::max(0.0, maxX - minX), std::max(0.0, maxY - minY), std::max(0.0, maxZ - minZ) };

                const int denom = std::max(1, count);
                obs.平均颜色 = Color{
                    static_cast<std::uint8_t>(std::min<std::uint64_t>(255, sumR / static_cast<std::uint64_t>(denom))),
                    static_cast<std::uint8_t>(std::min<std::uint64_t>(255, sumG / static_cast<std::uint64_t>(denom))),
                    static_cast<std::uint8_t>(std::min<std::uint64_t>(255, sumB / static_cast<std::uint64_t>(denom)))
                };

                obs.点云簇.reserve(static_cast<std::size_t>(count));
                for (int pi : pixels) {
                    const auto& P = 帧.点云[static_cast<std::size_t>(pi)];
                    if (P.z > 0.0) {
                        obs.点云簇.push_back(P);
                    }
                }

                const int rw = obs.w;
                const int rh = obs.h;

                if (cfg.轮廓_输出原始掩膜) {
                    obs.掩膜.assign(static_cast<std::size_t>(rw) * static_cast<std::size_t>(rh), 0);
                    for (int pi : pixels) {
                        const int u = pi % w;
                        const int v = pi / w;
                        const int ru = u - obs.x;
                        const int rv = v - obs.y;
                        obs.掩膜[static_cast<std::size_t>(rv) * static_cast<std::size_t>(rw) + static_cast<std::size_t>(ru)] = 1;
                    }
                }

                if (cfg.轮廓_输出裁剪颜色) {
                    obs.裁剪颜色.assign(static_cast<std::size_t>(rw) * static_cast<std::size_t>(rh), Color{ 0, 0, 0 });
                    for (int rv = 0; rv < rh; ++rv) {
                        const int v = obs.y + rv;
                        for (int ru = 0; ru < rw; ++ru) {
                            const int u = obs.x + ru;
                            obs.裁剪颜色[static_cast<std::size_t>(rv) * static_cast<std::size_t>(rw) + static_cast<std::size_t>(ru)] =
                                帧.颜色[static_cast<std::size_t>(v) * static_cast<std::size_t>(w) + static_cast<std::size_t>(u)];
                        }
                    }
                }

                if (cfg.轮廓_输出边界点 && cfg.轮廓_输出原始掩膜) {
                    auto ridx = [&](int ru, int rv) {
                        return static_cast<std::size_t>(rv) * static_cast<std::size_t>(rw) + static_cast<std::size_t>(ru);
                        };

                    for (int rv = 0; rv < rh; ++rv) {
                        for (int ru = 0; ru < rw; ++ru) {
                            if (obs.掩膜[ridx(ru, rv)] == 0) continue;

                            const bool edge =
                                (ru == 0 || obs.掩膜[ridx(ru - 1, rv)] == 0) ||
                                (ru == rw - 1 || obs.掩膜[ridx(ru + 1, rv)] == 0) ||
                                (rv == 0 || obs.掩膜[ridx(ru, rv - 1)] == 0) ||
                                (rv == rh - 1 || obs.掩膜[ridx(ru, rv + 1)] == 0);

                            if (edge) {
                                const int u = obs.x + ru;
                                const int v = obs.y + rv;
                                obs.边界点.push_back(static_cast<std::int64_t>((static_cast<std::uint64_t>(static_cast<std::uint32_t>(v)) << 32) | static_cast<std::uint32_t>(u)));
                            }
                        }
                    }
                }

                out.push_back(std::move(obs));
                ++found;
                if (found >= std::max(1, cfg.轮廓_最大数量)) {
                    return;
                }
            }
        }
    }
};
#pragma pack(pop)

// 功能：初始化对象实例。
D455_相机实现::D455_相机实现(配置项 cfg)
    : 实现指针(std::make_unique<实现>(cfg)) {
}

D455_相机实现::~D455_相机实现() = default;
D455_相机实现::D455_相机实现(D455_相机实现&&) noexcept = default;
D455_相机实现& D455_相机实现::operator=(D455_相机实现&&) noexcept = default;

// 功能：按函数名执行对应处理。
bool D455_相机实现::打开() {
    return 实现指针 && 实现指针->打开();
}

// 功能：按函数名执行对应处理。
void D455_相机实现::关闭() {
    if (实现指针) {
        实现指针->关闭();
    }
}

// 功能：按函数名执行对应处理。
bool D455_相机实现::采集一帧(结构体_原始场景帧& 输出) {
    return 实现指针 && 实现指针->采集一帧(输出);
}

// 功能：读取 D455 当前彩色视频帧。
bool D455_相机实现::采集彩色视频帧(结构体_D455彩色视频帧& 输出) {
    return 实现指针 && 实现指针->采集彩色视频帧(输出);
}

// 功能：采集一帧队列高频观察材料，保留报告队列所需彩色、对齐深度、内参和帧元数据。
bool D455_相机实现::采集轻量观察帧(结构体_原始场景帧& 输出) {
    return 实现指针 && 实现指针->采集轻量观察帧(输出);
}

// 功能：读取最近一次轻量观察帧采集的诊断阶段耗时。
结构体_D455轻量观察帧阶段耗时 D455_相机实现::读取最近轻量观察帧阶段耗时() const {
    return 实现指针 ? 实现指针->读取最近轻量观察帧阶段耗时() : 结构体_D455轻量观察帧阶段耗时{};
}

// 功能：读取并返回指定对象、状态或运行材料。
const D455_相机实现::配置项& D455_相机实现::获取配置() const {
    static const 配置项 默认配置{};
    return 实现指针 ? 实现指针->获取配置() : 默认配置;
}

// 功能：按函数名执行对应处理。
bool D455_相机实现::应用配置(const 配置项& 新配置) {
    return 实现指针 && 实现指针->应用配置(新配置);
}

// 功能：按函数名执行对应处理。
bool D455_相机实现::采集一帧并提取轮廓(结构体_原始场景帧& 输出, std::vector<结构体_轮廓观测>& out轮廓) {
    return 实现指针 && 实现指针->采集一帧并提取轮廓(输出, out轮廓);
}

// 功能：读取并返回指定对象、状态或运行材料。
const std::vector<结构体_轮廓观测>& D455_相机实现::获取最近轮廓观测() const {
    static const std::vector<结构体_轮廓观测> 空轮廓;
    return 实现指针 ? 实现指针->获取最近轮廓观测() : 空轮廓;
}
