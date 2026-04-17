#pragma once

#include <cstdint>
#include <vector>

#include "基础信息主信息类.h"

struct 二次特征匹配配置_桥接 {
    bool 使用对象绑定 = true;
    bool 条件空指针视为通配 = true;
    bool 允许用概念名称弱匹配 = true;
};

struct 二次特征匹配结果_桥接 {
    std::int64_t 命中数 = 0;
    std::int64_t 摘要数 = 0;
    std::int64_t 目标数 = 0;
    std::int64_t 精确率_Q10000 = 0;
    std::int64_t 召回率_Q10000 = 0;
    std::int64_t 得分_Q10000 = 0;
};

bool 二次特征满足_桥接(
    const 二次特征主信息类* 条件,
    const 二次特征主信息类* 状态,
    const 二次特征匹配配置_桥接& 配置 = {}) noexcept;

bool 条件索引满足_桥接(
    const std::vector<const 二次特征节点类*>& 条件索引,
    const std::vector<const 二次特征节点类*>& 当前判定集,
    std::int64_t* 输出满足数 = nullptr,
    const 二次特征匹配配置_桥接& 配置 = {}) noexcept;

二次特征匹配结果_桥接 结果摘要匹配目标_桥接(
    const std::vector<const 二次特征节点类*>& 摘要索引,
    const std::vector<const 二次特征节点类*>& 目标判定集,
    const 二次特征匹配配置_桥接& 配置 = {}) noexcept;
