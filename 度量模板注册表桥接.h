#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class 枚举_度量刻度语义代码 : std::uint8_t {
    未定义 = 0,
    低 = 1,
    中 = 2,
    高 = 3,
    小差异 = 4,
    中差异 = 5,
    大差异 = 6,
    低相似 = 7,
    中相似 = 8,
    高相似 = 9,
    覆盖低 = 10,
    覆盖中 = 11,
    覆盖高 = 12,
    齐全 = 13,
    缺A = 14,
    缺B = 15,
    都缺 = 16,
};

struct 结构_度量刻度片_桥接 {
    std::int64_t 低值 = 0;
    std::int64_t 高值 = -1;
    枚举_度量刻度语义代码 语义 = 枚举_度量刻度语义代码::未定义;
    bool 是小区间 = false;
};

const char* 度量刻度语义显示文本(枚举_度量刻度语义代码 语义) noexcept;
void 初始化度量模板注册表() noexcept;
std::vector<结构_度量刻度片_桥接> 获取度量默认刻度_由链键(
    const std::string& 链键,
    const std::string& 特征类型主键 = {});
