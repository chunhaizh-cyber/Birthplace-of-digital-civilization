#include <exception>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "双目相机本能适配器.h"

import D455相机模块;
import 基础数据类型模块;

namespace {
    HMODULE g_RealSense运行时模块 = nullptr;
    std::mutex g_互斥;
    std::unique_ptr<D455_相机实现> g_相机;
    bool g_相机已打开 = false;

    std::string 路径UTF8(const std::filesystem::path& 路径)
    {
        const auto 文本 = 路径.u8string();
        return std::string(
            reinterpret_cast<const char*>(文本.data()),
            文本.size());
    }

    std::filesystem::path 模块目录() noexcept
    {
        wchar_t 缓冲区[MAX_PATH]{};
        const DWORD 长度 = GetModuleFileNameW(nullptr, 缓冲区, MAX_PATH);
        if (长度 == 0 || 长度 >= MAX_PATH) {
            return {};
        }
        return std::filesystem::path(缓冲区).parent_path();
    }

    bool 确保RealSense运行时(std::string& 错误消息) noexcept
    {
        if (g_RealSense运行时模块) {
            return true;
        }

        try {
            const auto 根目录 = 模块目录();
            const auto 主DLL = (根目录 / L"realsense2.dll").lexically_normal();
            const auto LZ4DLL = (根目录 / L"lz4.dll").lexically_normal();

            if (!std::filesystem::exists(主DLL)) {
                错误消息 = "RealSense 运行时缺失: " + 路径UTF8(主DLL);
                return false;
            }
            if (!std::filesystem::exists(LZ4DLL)) {
                错误消息 = "RealSense 依赖缺失: " + 路径UTF8(LZ4DLL);
                return false;
            }

            g_RealSense运行时模块 = LoadLibraryExW(
                主DLL.c_str(),
                nullptr,
                LOAD_WITH_ALTERED_SEARCH_PATH);
            if (!g_RealSense运行时模块) {
                错误消息 = "RealSense 运行时加载失败 | Win32="
                    + std::to_string(GetLastError())
                    + " | 路径="
                    + 路径UTF8(主DLL);
                return false;
            }
            return true;
        }
        catch (const std::exception& e) {
            错误消息 = std::string("RealSense 运行时检查异常: ") + e.what();
            return false;
        }
        catch (...) {
            错误消息 = "RealSense 运行时检查发生未知异常";
            return false;
        }
    }

    双目相机本能适配器::调用结果 从帧生成结果(const 结构体_原始场景帧& 帧)
    {
        双目相机本能适配器::调用结果 结果{};
        结果.成功 = true;
        结果.相机已打开 = true;
        结果.消息 = "D455 采集一帧完成";
        结果.宽度 = 帧.宽度;
        结果.高度 = 帧.高度;
        结果.深度帧号 = 帧.时间戳.深度帧号;
        结果.彩色帧号 = 帧.时间戳.彩色帧号;
        结果.轮廓数量 = 帧.轮廓观测列表.size();
        return 结果;
    }
}

namespace 双目相机本能适配器 {
    调用结果 打开() noexcept
    {
        std::lock_guard<std::mutex> 锁(g_互斥);
        调用结果 结果{};
        std::string 运行时错误{};
        if (!确保RealSense运行时(运行时错误)) {
            结果.原因 = 失败原因::运行时缺失;
            结果.消息 = 运行时错误;
            return 结果;
        }

        try {
            if (!g_相机) {
                D455_相机实现::配置项 配置{};
                配置.启用轮廓提取 = true;
                配置.轮廓_输出原始掩膜 = true;
                g_相机 = std::make_unique<D455_相机实现>(配置);
                g_相机已打开 = false;
            }
            if (!g_相机已打开) {
                if (!g_相机->打开()) {
                    g_相机.reset();
                    g_相机已打开 = false;
                    结果.原因 = 失败原因::打开失败;
                    结果.消息 = "D455 打开失败";
                    return 结果;
                }
                g_相机已打开 = true;
            }
            结果.成功 = true;
            结果.相机已打开 = true;
            结果.消息 = "D455 已可用";
            return 结果;
        }
        catch (const std::exception& e) {
            g_相机.reset();
            g_相机已打开 = false;
            结果.原因 = 失败原因::打开失败;
            结果.消息 = std::string("D455 打开异常: ") + e.what();
            return 结果;
        }
        catch (...) {
            g_相机.reset();
            g_相机已打开 = false;
            结果.原因 = 失败原因::打开失败;
            结果.消息 = "D455 打开未知异常";
            return 结果;
        }
    }

    调用结果 释放() noexcept
    {
        std::lock_guard<std::mutex> 锁(g_互斥);
        调用结果 结果{};
        try {
            if (g_相机) {
                g_相机->关闭();
            }
            g_相机.reset();
            g_相机已打开 = false;
            结果.成功 = true;
            结果.消息 = "D455 已释放";
            return 结果;
        }
        catch (...) {
            g_相机.reset();
            g_相机已打开 = false;
            结果.成功 = true;
            结果.消息 = "D455 释放时出现异常，已清理本地持有状态";
            return 结果;
        }
    }

    调用结果 检查() noexcept
    {
        std::lock_guard<std::mutex> 锁(g_互斥);
        调用结果 结果{};
        std::string 运行时错误{};
        if (!确保RealSense运行时(运行时错误)) {
            结果.成功 = true;
            结果.相机已打开 = false;
            结果.原因 = 失败原因::运行时缺失;
            结果.消息 = 运行时错误;
            return 结果;
        }
        结果.成功 = true;
        结果.相机已打开 = g_相机 && g_相机已打开;
        结果.消息 = 结果.相机已打开 ? "D455 正在运行" : "D455 未打开";
        return 结果;
    }

    调用结果 采集一帧() noexcept
    {
        std::lock_guard<std::mutex> 锁(g_互斥);
        调用结果 结果{};
        if (!g_相机 || !g_相机已打开) {
            结果.原因 = 失败原因::不可用;
            结果.消息 = "D455 未达到可用状态";
            return 结果;
        }

        try {
            结构体_原始场景帧 帧{};
            if (!g_相机->采集一帧(帧)) {
                结果.原因 = 失败原因::采集失败;
                结果.消息 = "D455 采集一帧失败";
                return 结果;
            }
            return 从帧生成结果(帧);
        }
        catch (const std::exception& e) {
            结果.原因 = 失败原因::采集失败;
            结果.消息 = std::string("D455 采集异常: ") + e.what();
            return 结果;
        }
        catch (...) {
            结果.原因 = 失败原因::采集失败;
            结果.消息 = "D455 采集未知异常";
            return 结果;
        }
    }
}
