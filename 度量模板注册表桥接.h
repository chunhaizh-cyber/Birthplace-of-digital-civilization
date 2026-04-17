#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct 结构_度量刻度片_桥接 {
    std::int64_t 低值 = 0;
    std::int64_t 高值 = -1;
    std::string 名称{};
    bool 是小区间 = false;
};

void 初始化度量模板注册表() noexcept;
std::vector<结构_度量刻度片_桥接> 获取度量默认刻度_由链键(
    const std::string& 链键,
    const std::string& 特征类型主键 = {});
