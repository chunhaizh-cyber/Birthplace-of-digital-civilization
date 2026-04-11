#pragma once

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

struct 结构_动作执行回执 {
    std::vector<std::string> 动作名称列表{};
    std::string 动作分类{};
    bool 成功 = false;
    std::string 错误码{};
    std::string 诊断{};
    std::int64_t 开始时间 = 0;
    std::int64_t 结束时间 = 0;
    std::string 摘要{};
};

inline std::string 构造动作执行回执摘要(const 结构_动作执行回执& 回执)
{
    std::ostringstream 输出;
    输出 << "成功=" << (回执.成功 ? "是" : "否");
    if (!回执.动作分类.empty()) {
        输出 << " | 分类=" << 回执.动作分类;
    }
    if (!回执.动作名称列表.empty()) {
        输出 << " | 动作=";
        for (std::size_t i = 0; i < 回执.动作名称列表.size(); ++i) {
            if (i > 0) {
                输出 << ",";
            }
            输出 << 回执.动作名称列表[i];
        }
    }
    if (!回执.错误码.empty()) {
        输出 << " | 错误码=" << 回执.错误码;
    }
    if (回执.开始时间 > 0 || 回执.结束时间 > 0) {
        输出 << " | 时间=" << 回执.开始时间 << "->" << 回执.结束时间;
    }
    if (!回执.诊断.empty()) {
        输出 << " | 诊断=" << 回执.诊断;
    }
    return 输出.str();
}
