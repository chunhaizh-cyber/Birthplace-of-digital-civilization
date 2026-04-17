#include "度量模板注册表桥接.h"

import 度量模板注册表模块;

void 初始化度量模板注册表() noexcept
{
    度量模板注册表类::实例().注册默认模板();
}

std::vector<结构_度量刻度片_桥接> 获取度量默认刻度_由链键(
    const std::string& 链键,
    const std::string& 特征类型主键)
{
    std::vector<结构_度量刻度片_桥接> 输出{};
    const auto 刻度列表 = 度量模板注册表类::实例().默认刻度_由链键(链键, 特征类型主键);
    输出.reserve(刻度列表.size());
    for (const auto& 刻度 : 刻度列表) {
        输出.push_back({
            刻度.区间.低值,
            刻度.区间.高值,
            刻度.名称,
            刻度.是小区间,
        });
    }
    return 输出;
}
