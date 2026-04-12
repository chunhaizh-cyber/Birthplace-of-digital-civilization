#pragma once

#include <string>

#include "本能方法类.h"
#include "任务管理任务模块.h"

namespace 任务管理最小原语模块 {

using 类型_任务管理最小原语执行体 = bool (*)(void*) noexcept;

struct 结构_任务管理最小原语执行请求 {
    枚举_任务管理最小原语ID 原语ID = 枚举_任务管理最小原语ID::未定义;
    枚举_本能方法ID 本能ID = 枚举_本能方法ID::未定义;
    void* 执行上下文 = nullptr;
    类型_任务管理最小原语执行体 执行体 = nullptr;
    std::string 调用点{};
};

枚举_本能方法ID 查询对应本能方法ID(枚举_任务管理最小原语ID 原语ID) noexcept;
枚举_任务管理最小原语ID 查询对应最小原语ID(枚举_本能方法ID 本能ID) noexcept;

bool 执行任务管理最小原语(const 结构_任务管理最小原语执行请求& 请求) noexcept;

bool 按本能方法ID执行任务管理最小原语(
    枚举_本能方法ID 本能ID,
    void* 执行上下文,
    类型_任务管理最小原语执行体 执行体,
    const std::string& 调用点 = {}) noexcept;

bool 执行确保单承载存在(
    void* 执行上下文,
    类型_任务管理最小原语执行体 执行体,
    const std::string& 调用点 = {}) noexcept;

bool 执行读取单特征当前值(
    void* 执行上下文,
    类型_任务管理最小原语执行体 执行体,
    const std::string& 调用点 = {}) noexcept;

bool 执行写入单特征当前值(
    void* 执行上下文,
    类型_任务管理最小原语执行体 执行体,
    const std::string& 调用点 = {}) noexcept;

bool 执行写入单挂点指针(
    void* 执行上下文,
    类型_任务管理最小原语执行体 执行体,
    const std::string& 调用点 = {}) noexcept;

bool 执行记录动作开始(
    void* 执行上下文,
    类型_任务管理最小原语执行体 执行体,
    const std::string& 调用点 = {}) noexcept;

bool 执行记录动作结束(
    void* 执行上下文,
    类型_任务管理最小原语执行体 执行体,
    const std::string& 调用点 = {}) noexcept;

} // namespace 任务管理最小原语模块
