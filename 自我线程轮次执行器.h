#pragma once

#include <string>

#include "任务管理任务模块.h"
#include "自我线程最小回写器.h"
#include "自我线程运行包.h"

class 自我类;

namespace 自我线程轮次执行器 {

struct 结构_任务治理桥接结果 {
    bool 任务管理上位输入已刷新 = false;
    枚举_任务管理根层重判结果 根层重判结果 = 枚举_任务管理根层重判结果::未定义;
    枚举_任务管理执行前门控结果 执行前门控结果 = 枚举_任务管理执行前门控结果::未定义;
    std::string 上层反馈摘要{};
    bool 任务管理治理触发 = false;
    bool 已生成一步治理任务包 = false;
    bool 已生成回流结算包 = false;
    bool 已写入动态节点 = false;
    bool 已写入因果候选 = false;
    结构_任务管理结果 治理结果{};
    自我线程运行包::结构_一步治理任务包 一步治理任务包{};
    自我线程最小回写器::结构_最小回写结果 最小回写结果{};
};

bool 执行一轮最小任务治理(
    自我类& 自我对象,
    时间戳 now,
    枚举_任务管理根层重判结果 根层重判结果,
    枚举_任务管理执行前门控结果 执行前门控结果,
    const std::string& 上层反馈摘要,
    const std::string& 调用点 = "自我线程轮次执行器::执行一轮最小任务治理",
    结构_任务治理桥接结果* 输出 = nullptr) noexcept;

} // namespace 自我线程轮次执行器
