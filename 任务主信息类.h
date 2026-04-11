#pragma once

#include "语言节点类型.h"
#include "语素信息类型.h"
#include "基础信息节点类型.h"
#include "需求节点类型.h"
#include "双向链表模板核心.h"

class 任务节点类;
class 方法节点类;

enum class 枚举_任务节点种类 : std::uint8_t {
    未定义 = 0,
    头结点 = 1,
    步骤节点 = 2,
    结果节点 = 3,
};

enum class 枚举_任务状态 : std::uint8_t {
    未定义 = 0,
    未启动 = 1,
    运行中 = 2,
    挂起 = 3,
    完成 = 4,
    失败 = 5,
    取消 = 6,
};

class 任务信息基类 {
public:
    const 词性节点类* 名称 = nullptr;
    const 自然句节点类* 描述信息 = nullptr;
    const 词性节点类* 类型 = nullptr;
    枚举_任务节点种类 节点种类 = 枚举_任务节点种类::未定义;
    枚举_任务状态 状态 = 枚举_任务状态::未定义;

    结构_统计 统计{};
    std::int64_t 基准优先级 = 0;
    std::int64_t 局部优先级偏移 = 0;
    std::int64_t 调度优先级 = 0;

    可解析引用<场景节点类> 场景{};
    可解析引用<存在节点类> 任务虚拟存在{};

    virtual ~任务信息基类() = default;
};

enum class 枚举_方法节点种类 : std::uint8_t {
    未定义 = 0,
    方法首节点 = 1,
    方法条件节点 = 2,
    方法结果节点 = 3,
};

enum class 枚举_动作句柄类型 : std::uint8_t {
    未设置 = 0,
    本能函数ID = 1,
    外部实现主键 = 2,
    动作序列主键 = 3,
};

enum class 枚举_方法类型 : std::uint8_t {
    未定义 = 0,
    内部方法 = 1,
    外部方法 = 2,
};

struct 结构体_动作句柄 {
    枚举_动作句柄类型 类型 = 枚举_动作句柄类型::未设置;
    I64 本能ID = 0;
    std::string 外部实现主键{};
    std::string 动作序列主键{};

    bool 有效() const noexcept {
        switch (类型) {
        case 枚举_动作句柄类型::本能函数ID: return 本能ID != 0;
        case 枚举_动作句柄类型::外部实现主键: return !外部实现主键.empty();
        case 枚举_动作句柄类型::动作序列主键: return !动作序列主键.empty();
        default: return false;
        }
    }
};

class 任务主信息类 : public 任务信息基类 {
public:
    可解析引用<需求节点类> 来源需求{};
    可解析引用<任务节点类> 父任务{};
    可解析引用<任务节点类> 前置任务{};
    可解析引用<任务节点类> 后续任务{};
    可解析引用<方法节点类> 执行方法{};
    可解析引用<状态节点类> 目标状态{};
    可解析引用<状态节点类> 结果状态信息{};
    结构体_动作句柄 动作句柄{};
    时间戳 启动时间 = 0;
    时间戳 完成时间 = 0;

    std::int64_t 比较(const 任务主信息类* 对象, 枚举_比较字段 字段) const {
        return 对象 ? 比较(*对象, 字段) : -1;
    }

    std::int64_t 比较(const 任务主信息类& 对象, 枚举_比较字段 字段) const {
        switch (字段) {
        case 枚举_比较字段::名称:
            return 名称 == 对象.名称 ? 1 : 0;
        case 枚举_比较字段::类型:
            return 类型 == 对象.类型 ? 1 : 0;
        case 枚举_比较字段::值:
            return 状态 == 对象.状态 ? 1 : 0;
        default:
            return -1;
        }
    }
};
