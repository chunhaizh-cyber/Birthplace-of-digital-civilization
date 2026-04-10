#pragma once

#include "任务主信息类.h"

class 方法信息基类 {
public:
    const 自然句节点类* 描述信息 = nullptr;
    枚举_方法节点种类 节点种类 = 枚举_方法节点种类::未定义;
    可解析引用<存在节点类> 方法虚拟存在{};
    可解析引用<场景节点类> 条件场景{};
    可解析引用<场景节点类> 结果场景{};
    const 词性节点类* 动作名 = nullptr;
    结构体_动作句柄 动作句柄{};
    结构_统计 统计{};

    virtual ~方法信息基类() = default;
    virtual std::int64_t 比较(方法信息基类* 对象, 枚举_比较字段) const {
        return 对象 ? 0 : -1;
    }
};

class 方法主信息类 : public 方法信息基类 {
public:
    可解析引用<任务节点类> 来源任务{};
    可解析引用<方法节点类> 父方法{};
    可解析引用<方法节点类> 前置方法{};
    可解析引用<方法节点类> 后续方法{};
    可解析引用<状态节点类> 期望结果状态{};
    时间戳 最近执行时间 = 0;
    时间戳 最近完成时间 = 0;

    std::int64_t 比较(方法信息基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<方法主信息类*>(对象);
        if (!右) return -1;

        switch (字段) {
        case 枚举_比较字段::名称:
            return 动作名 == 右->动作名 ? 1 : 0;
        case 枚举_比较字段::类型:
            return 节点种类 == 右->节点种类 ? 1 : 0;
        case 枚举_比较字段::值:
            return 私有_动作句柄相等(动作句柄, 右->动作句柄) ? 1 : 0;
        default:
            return 0;
        }
    }

private:
    static bool 私有_动作句柄相等(const 结构体_动作句柄& a, const 结构体_动作句柄& b) noexcept {
        return a.类型 == b.类型
            && a.本能ID == b.本能ID
            && a.外部实现主键 == b.外部实现主键
            && a.动作序列主键 == b.动作序列主键;
    }
};
