#pragma once

#include <mutex>

// 场景主信息里的状态 / 动态 / 二次特征索引是事实目录缓存，
// 会被自我线程、自检线程和控制面板读取路径同时触达。
// 统一保护这些 vector，避免扩容 / 排序期间并发读写破坏引用对象。
inline std::recursive_mutex& 借用场景索引全局互斥() noexcept
{
    static std::recursive_mutex 互斥{};
    return 互斥;
}
