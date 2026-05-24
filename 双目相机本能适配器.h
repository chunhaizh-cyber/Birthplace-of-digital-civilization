#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

namespace 双目相机本能适配器 {
    enum class 失败原因 : std::uint8_t {
        无 = 0,
        运行时缺失,
        打开失败,
        不可用,
        采集失败,
    };

    struct 调用结果 {
        bool 成功 = false;
        bool 相机已打开 = false;
        失败原因 原因 = 失败原因::无;
        std::string 消息{};
        int 宽度 = 0;
        int 高度 = 0;
        std::uint32_t 深度帧号 = 0;
        std::uint32_t 彩色帧号 = 0;
        std::size_t 轮廓数量 = 0;
    };

    调用结果 打开() noexcept;
    调用结果 释放() noexcept;
    调用结果 检查() noexcept;
    调用结果 采集一帧() noexcept;
}
