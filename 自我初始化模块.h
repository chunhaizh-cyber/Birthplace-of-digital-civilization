#pragma once

#include <string>

class 自我类;

namespace 自我初始化模块 {

struct 结构_自我初始化结果 {
    bool 数据层已初始化 = false;
    bool 当前活动自我已就绪 = false;
    bool 自我存在已就绪 = false;
    bool 自我现实场景已就绪 = false;
    bool 自我内部世界已就绪 = false;
    bool 自我锚点已初始化 = false;

    bool 核心特征承载已就绪 = false;
    bool 主链镜像承载已就绪 = false;
    bool 运行控制承载已就绪 = false;
    bool 默认本能方法已注册 = false;
    bool 根需求已就绪 = false;
    bool 根任务已就绪 = false;
    bool 治理任务已就绪 = false;
    bool 方法根已就绪 = false;
    bool 当前主链已回填 = false;
    bool 自我运行环境已初始化 = false;

    bool 初始化完成 = false;
    std::string 摘要{};
};

bool 执行自我初始化(
    自我类& 自我对象,
    const std::string& 调用点 = "自我初始化模块::执行自我初始化",
    结构_自我初始化结果* 输出 = nullptr) noexcept;

结构_自我初始化结果 读取自我初始化结果(const 自我类& 自我对象) noexcept;

} // namespace 自我初始化模块
