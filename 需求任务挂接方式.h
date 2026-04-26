#pragma once

#include <cstdint>

enum class 枚举_需求任务挂接方式 : std::uint8_t {
    未定义 = 0,
    增补为新步骤 = 1,
    挂到当前步骤前置任务 = 2,
};
