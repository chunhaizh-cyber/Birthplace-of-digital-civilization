#pragma once

#include "语素信息类型.h"

class 自我特征定义类 {
public:
    static void 初始化依赖语素() noexcept;

    static const 词性节点类* 类型_自我_安全值() noexcept;
    static const 词性节点类* 类型_自我_服务值() noexcept;
    static const 词性节点类* 类型_自我_物理安全() noexcept;
    static const 词性节点类* 类型_自我_风险安全() noexcept;
    static const 词性节点类* 类型_自我_情绪ID() noexcept;
    static const 词性节点类* 类型_自我_情绪强度() noexcept;
    static const 词性节点类* 类型_自我_外显情绪ID() noexcept;
    static const 词性节点类* 类型_自我_外显情绪强度() noexcept;
    static const 词性节点类* 类型_自我_待学习方法数量() noexcept;
    static const 词性节点类* 类型_自我_当前主需求() noexcept;
    static const 词性节点类* 类型_自我_当前主任务() noexcept;
    static const 词性节点类* 类型_自我_当前主方法() noexcept;
    static const 词性节点类* 类型_自我_外设可用性() noexcept;
    static const 词性节点类* 类型_自我_尝试学习状态() noexcept;
    static const 词性节点类* 类型_自我_待机状态() noexcept;
    static const 词性节点类* 类型_自我_时序正向步长() noexcept;
    static const 词性节点类* 类型_自我_时序反向步长() noexcept;
    static const 词性节点类* 类型_自我_服务时序衰减步长() noexcept;
};
