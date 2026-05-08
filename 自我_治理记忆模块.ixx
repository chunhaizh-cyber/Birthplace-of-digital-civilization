module;

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

#include "需求类.h"
#include "任务类.h"
#include "方法类.h"

export module 自我_治理记忆模块;

import 自我_缺口承接模块;

export {

enum class 枚举_治理记忆类型_最小 : std::uint8_t {
    未定义 = 0,
    缺口回流 = 1,
    执行失败 = 2,
    禁止命中 = 3,
    缺口压力 = 4,
    风险记忆 = 5,
    正向推进 = 6,
    方法谨慎 = 7,
    执行冻结 = 8,
    边界优先 = 9,
    重试压力 = 10,
};

enum class 枚举_治理记忆命中用途_最小 : std::uint8_t {
    全部 = 0,
    主需求竞争 = 1,
    去向判定 = 2,
    重复缺口承接门控 = 3,
};

struct 结构_治理临时记忆项_最小 {
    bool 有效 = false;
    枚举_治理记忆类型_最小 类型 = 枚举_治理记忆类型_最小::未定义;
    需求类::节点类* 来源需求 = nullptr;
    任务类::节点类* 来源任务头结点 = nullptr;
    方法类::节点类* 来源方法首节点 = nullptr;
    I64 缺口类型 = 0;
    I64 反馈类型 = 0;
    I64 缺口状态 = 0;
    I64 等待原因 = 0;
    I64 回流策略 = 0;
    I64 记忆强度 = 0;
    I64 缺口压力值 = 0;
    I64 重试压力值 = 0;
    I64 风险值 = 0;
    I64 正向推进值 = 0;
    I64 边界优先建议值 = 0;
    时间戳 记录时间 = 0;
    时间戳 失效时间 = 0;
    时间戳 最近衰减时间 = 0;
    bool 阻止重复缺口承接 = false;
    bool 暂缓主需求竞争 = false;
    bool 影响去向判定 = false;
    std::string 摘要{};
};

} // export
