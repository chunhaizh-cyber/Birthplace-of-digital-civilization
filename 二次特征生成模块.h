#pragma once

#include <cstdint>
#include <vector>

#include "基础信息类.h"

class 世界树类;

class 二次特征生成模块 {
public:
    explicit 二次特征生成模块(世界树类* 世界树 = nullptr) noexcept;

    void 绑定世界树(世界树类* 世界树) noexcept;

    std::vector<二次特征节点类*> 生成动态型基础二次特征(场景节点类* 场景, 动态节点类* 动态);
    动态节点类* 尝试生成连续同向聚合动态(场景节点类* 场景, 动态节点类* 动态);
    动态节点类* 尝试生成同类事件段聚合动态(场景节点类* 场景, 动态节点类* 动态);

    std::vector<二次特征节点类*> 刷新场景存在型基础二次特征(
        场景节点类* 场景,
        std::int64_t 接近阈值_mm = 1000,
        std::int64_t 冲突阈值_mm = 200
    );

private:
    世界树类* 世界树_ = nullptr;
};
