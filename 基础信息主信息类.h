#pragma once

#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <array>
#include <cmath>

#include "语言节点类型.h"
#include "语素信息类型.h"
#include "基础信息节点类型.h"
#include "方法信息类型.h"
#include "任务信息类型.h"
#include "需求信息类型.h"

import 基础数据类型模块;
//class 方法节点类;
//class 任务节点类;
//class 需求节点类;

enum class 枚举_状态域 : std::uint8_t {
    未定义 = 0,
    内部状态 = 1,
    世界状态 = 2,
};

enum class 枚举_动态来源类型 : std::uint8_t {
    未定义 = 0,
    方法调用 = 1,
    内部机制动作 = 2,
};

enum class 枚举_动态业务语义类型 : std::uint8_t {
    未定义 = 0,
    动作事实动态 = 1,
    状态变化动态 = 2,
    被动记账动态 = 3,
    治理过程动态 = 4,
    边界结束动态 = 5,
};

enum class 枚举_动作事件相位 : std::uint8_t {
    未定义 = 0,
    开始运行 = 1,
    完成 = 2,
    失败 = 3,
};

enum class 枚举_动态聚合方式 : std::uint8_t {
    未定义 = 0,
    原子相邻 = 1,
    连续同向段 = 2,
    同类事件段 = 3,
    窗口聚合 = 4,
    路径压缩 = 5,
};

enum class 枚举_二次特征域 : std::uint8_t {
    未定义 = 0,
    存在 = 1,
    动态 = 2,
};

enum class 枚举_二次特征主母式 : std::uint8_t {
    未定义 = 0,
    存在关系 = 1,
    差分 = 2,
    时间归一 = 3,
    路径 = 4,
    参照偏离 = 5,
    周期相位 = 6,
    状态转移 = 7,
};

enum class 枚举_二次特征粒度 : std::uint8_t {
    未定义 = 0,
    当前场景 = 1,
    相邻分步 = 2,
    首尾整体 = 3,
    完整路径 = 4,
    聚合段 = 5,
};

enum class 枚举_二次特征基准类型 : std::uint8_t {
    未定义 = 0,
    无 = 1,
    相邻前态 = 2,
    起始态 = 3,
    稳态 = 4,
    目标态 = 5,
    上边界 = 6,
    下边界 = 7,
};

enum class 枚举_时间归一方式 : std::uint8_t {
    无 = 0,
    分步时长 = 1,
    总时长 = 2,
    频率窗口 = 3,
};

enum class 枚举_二次特征值形态 : std::uint8_t {
    未定义 = 0,
    布尔 = 1,
    标量 = 2,
    区间 = 3,
    枚举 = 4,
    签名 = 5,
};

enum class 枚举_因果锚点类型 : std::uint8_t {
    未定义 = 0,
    场景 = 1,
    存在 = 2,
};

enum class 枚举_因果来源类型 : std::uint8_t {
    未定义 = 0,
    观察生成 = 1,
    执行生成 = 2,
    推断生成 = 3,
};
// ============================================================================
// 主信息基类 / 基础信息基类
// ============================================================================
class 主信息基类 {
public:
    const 词性节点类* 名称 = nullptr;
    const 词性节点类* 类型 = nullptr;
    结构_统计 统计{};

    virtual ~主信息基类() = default;
};

class 基础信息基类 : public 主信息基类 {
public:
    枚举_主信息类型 主信息类型 = 枚举_主信息类型::基础信息基类;

    基础信息基类() = default;
    explicit 基础信息基类(枚举_主信息类型 t) { 主信息类型 = t; }
    基础信息基类(const 词性节点类* 名, const 词性节点类* 型, 枚举_主信息类型 t) {
        名称 = 名;
        类型 = 型;
        主信息类型 = t;
    }

    virtual std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段) const = 0;
};



template<class T, std::size_t N>
struct 环形缓冲 {
     static_assert(N >= 2);
     T data[N]{};
     std::size_t head = 0;
     std::size_t size = 0;

     void clear() { head = 0; size = 0; }

     void push(const T& v)
     {
         data[head] = v;
         head = (head + 1) % N;
         if (size < N) ++size;
     }

     // 0 表示最新，1 表示前一项...
     const T* get_from_latest(std::size_t i) const
     {
         if (i >= size) return nullptr;
         std::size_t idx = (head + N - 1 - i) % N;
         return &data[idx];
     }
 };

 // ========= 稳态原型：点 / 区间 =========
 // - 点：用于 VecU（轮廓/颜色/签名）等不可值域聚合的情况，或 I64 早期尚未形成区间。
 // - 区间：统一用“1 维标量区间”表达：
 //     * 值域（I64 标量）：box=[lo,hi] 表示真实取值范围。
 //     * 距离到原型（VecU / 不可排序值）：box=[lo,hi] 表示 diff(rep,v) 的允许范围（rep 必须存在）。
 //     * 相似度阈值（VecU / 不可排序值）：box=[lo,hi] 表示 sim(rep,v) 的允许范围（rep 必须存在）。
 //   说明：多维连续量若要走区间，请优先拆成多个 I64 特征；
 //         不可排序多维量（轮廓/颜色/签名等）请编码为 VecIU64，再用“距离/相似度阈值”表达稳态。

struct 稳态点原型
 {
     特征快照值 点{};                    // I64 或 VecU句柄
     std::uint64_t 命中次数 = 0;
     时间戳 最后命中 = 0;

     bool 有效() const noexcept { return 特征快照有值(点); }
 };

struct 稳态多维区间原型
 {
     I64多维区间 box{};                  // 每个维度一个 [lo,hi]
     枚举_区间语义 语义 = 枚举_区间语义::值域;

     std::uint64_t 命中次数 = 0;
     时间戳 最后命中 = 0;

     // 可选：用于解释/回溯/做 medoid（I64 或 VecU句柄）
     std::optional<特征快照值> 代表点{};

     bool 有效() const noexcept {
         if (!box.有效()) return false;
         if (box.维度.size() != 1) return false; // 当前区间统一为 1 维标量区间
         if (语义 == 枚举_区间语义::值域) return true;
         // 距离/相似度阈值：必须有代表点
         return 代表点.has_value() && 特征快照有值(*代表点);
     }
 };

using 稳态原型 = std::variant<稳态点原型, 稳态多维区间原型>;

class 指代节点主信息类 : public 基础信息基类 {
public:
    const 词性节点类* 代词 = nullptr;
    可解析引用<存在节点类> 指代对象{};

    指代节点主信息类() : 基础信息基类(枚举_主信息类型::指代) {}
    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段) const override {
        return dynamic_cast<指代节点主信息类*>(对象) ? 0 : -1;
    }
};

class 特征节点主信息类 : public 基础信息基类 {
public:
    特征值 当前值{};
    std::optional<I64区间> 区间{};
    std::vector<特征稳态> 稳态列表{};
    I64区间 连续测量误差区间{ 0, -1 };

    特征节点主信息类() : 基础信息基类(枚举_主信息类型::特征) {}
    explicit 特征节点主信息类(const 词性节点类* 特征类型)
        : 基础信息基类(nullptr, 特征类型, 枚举_主信息类型::特征) {
    }
    特征节点主信息类(const 词性节点类* 特征类型, const 特征值& v)
        : 基础信息基类(nullptr, 特征类型, 枚举_主信息类型::特征), 当前值(v) {
    }

    bool 有当前值() const noexcept { return 特征值有效(当前值); }
    bool 有区间() const noexcept { return 区间.has_value() && 区间->有效(); }
    void 设置当前值(const 特征值& v) { 当前值 = v; }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段) const override {
        return dynamic_cast<特征节点主信息类*>(对象) ? 0 : -1;
    }
};

class 存在节点主信息类 : public 基础信息基类 {
public:
    可解析引用<存在节点类> 概念模板{};
    可解析引用<场景节点类> 内部世界{};
    需求节点类* 需求根节点 = nullptr;
    任务节点类* 任务根节点 = nullptr;
    方法节点类* 方法根节点 = nullptr;

    bool 有位置历史 = false;
    Vector3D 上次观测位置_mm{ 0, 0, 0 };
    Vector3D 最近观测位置_mm{ 0, 0, 0 };
    std::int64_t 最近位移_mm = 0;
    std::uint32_t 连续静止帧 = 0;
    std::uint32_t 连续未命中帧 = 0;

    std::vector<可解析引用<因果实例节点类>> 实例因果{};

    存在节点主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::存在) {}
    存在节点主信息类(const 词性节点类* 名, const 词性节点类* 型 = nullptr)
        : 基础信息基类(名, 型, 枚举_主信息类型::存在) {
    }
    ~存在节点主信息类() override;

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<存在节点主信息类*>(对象);
        if (!右) return -1;
        switch (字段) {
        case 枚举_比较字段::基础信息基类_名称:
        case 枚举_比较字段::名称:
            return 名称 == 右->名称;
        case 枚举_比较字段::基础信息基类_类型:
        case 枚举_比较字段::类型:
            return 类型 == 右->类型;
        case 枚举_比较字段::评估_全局置信度:
            return 统计.可信度 - 右->统计.可信度;
        default:
            return -1;
        }
    }
};

class 场景节点主信息类 : public 基础信息基类 {
public:
    可解析引用<存在节点类> 宿主存在{};
    枚举_世界类型 世界类型 = 枚举_世界类型::现实世界;
    可解析引用<文章节点类> 来源文章{};

    // 只保留“索引引用”，不在场景里嵌入大对象副本。
    std::vector<可解析引用<状态节点类>> 状态索引{};
    std::vector<可解析引用<动态节点类>> 动态索引{};
    std::vector<可解析引用<二次特征节点类>> 关系索引{};
    std::vector<可解析引用<二次特征节点类>> 二次特征索引{};
    std::vector<可解析引用<因果实例节点类>> 实例因果索引{};

    场景节点主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::场景) {}
    explicit 场景节点主信息类(const 词性节点类* 名, 枚举_世界类型 t)
        : 基础信息基类(名, nullptr, 枚举_主信息类型::场景) {
        世界类型 = t;
    }

    bool 是否内部世界() const noexcept { return 宿主存在.有效(); }
    bool 是否现实世界() const noexcept { return 世界类型 == 枚举_世界类型::现实世界; }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<场景节点主信息类*>(对象);
        if (!右) return -1;
        switch (字段) {
        case 枚举_比较字段::基础信息基类_名称:
        case 枚举_比较字段::名称:
            return 名称 == 右->名称;
        case 枚举_比较字段::基础信息基类_类型:
        case 枚举_比较字段::类型:
            return 类型 == 右->类型;
        default:
            return -1;
        }
    }
};

class 状态节点主信息类 : public 基础信息基类 {
public:
    枚举_状态域 状态域 = 枚举_状态域::未定义;
    时间戳 收到时间 = 0;
    时间戳 发生时间 = 0;
    可解析引用<基础信息节点类> 状态主体{};
    可解析引用<特征节点类> 状态特征{};
    特征值 状态值{};
    可解析引用<基础信息节点类> 对应信息节点{};
    bool 是否变化 = false;
    std::string 变化原因类别{};
    std::string 变化原因说明{};

    状态节点主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::状态) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段) const override {
        return dynamic_cast<状态节点主信息类*>(对象) ? 0 : -1;
    }
};

class 动态节点主信息类 : public 基础信息基类 {
public:
    可解析引用<状态节点类> 初始状态{};
    可解析引用<状态节点类> 结果状态{};
    可解析引用<基础信息节点类> 动态主体{};
    可解析引用<特征节点类> 动态特征{};
    结构_时间段 时间{};

    枚举_动态来源类型 来源类型 = 枚举_动态来源类型::未定义;
    枚举_动态业务语义类型 业务语义类型 = 枚举_动态业务语义类型::未定义;
    可解析引用<方法节点类> 来源动作{};
    const 词性节点类* 来源动作名 = nullptr;
    枚举_动作事件相位 来源动作相位 = 枚举_动作事件相位::未定义;
    可解析引用<场景节点类> 来源输入场景{};
    可解析引用<场景节点类> 来源输出场景{};
    bool 来源执行成功 = false;
    I64 来源错误码 = 0;
    std::uint64_t 动态路径签名 = 0;
    std::uint32_t 动态层级 = 0;
    枚举_动态聚合方式 聚合方式 = 枚举_动态聚合方式::未定义;
    std::vector<可解析引用<动态节点类>> 来源低层动态{};

    动态节点主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::动态) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段) const override {
        return dynamic_cast<动态节点主信息类*>(对象) ? 0 : -1;
    }
};

class 二次特征主信息类 : public 基础信息基类 {
public:
    enum class 枚举_形态 : std::uint8_t {
        未定义 = 0,
        概念模板 = 1,
        条件 = 2,
        结果 = 3,
    };

    枚举_形态 形态 = 枚举_形态::未定义;
    枚举_二次特征种类 种类 = 枚举_二次特征种类::未定义;
    枚举_二次特征域 域 = 枚举_二次特征域::未定义;
    枚举_二次特征主母式 主母式 = 枚举_二次特征主母式::未定义;
    枚举_二次特征粒度 粒度 = 枚举_二次特征粒度::未定义;
    枚举_二次特征基准类型 基准类型 = 枚举_二次特征基准类型::未定义;
    枚举_时间归一方式 时间归一方式 = 枚举_时间归一方式::无;
    枚举_二次特征值形态 值形态 = 枚举_二次特征值形态::未定义;
    std::string 度量签名_链键{};

    const 词性节点类* 概念名称 = nullptr;
    std::optional<I64区间> 标量区间{};

    可解析引用<场景节点类> 所属场景{};
    可解析引用<状态节点类> 来源起始状态{};
    可解析引用<状态节点类> 来源结果状态{};
    可解析引用<动态节点类> 来源动态{};
    可解析引用<场景节点类> 来源场景{};
    std::uint32_t 来源动态层级 = 0;
    std::uint64_t 来源动态路径签名 = 0;
    可解析引用<基础信息节点类> 主体{};
    可解析引用<基础信息节点类> 客体{};
    可解析引用<基础信息节点类> 左对象{};
    可解析引用<基础信息节点类> 右对象{};

    std::int64_t 标量值 = 0;
    std::int32_t 离散编码 = 0;
    std::string 离散语义键{};
    bool 是否满足 = false;
    std::int64_t 置信度 = 0;
    std::uint32_t 规则版本 = 1;

    二次特征主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::二次特征_修饰) {}

    void 刷新度量签名_链键()
    {
        度量签名_链键 = 私有_生成度量签名_链键();
    }

    bool 语义相同(const 二次特征主信息类& 右) const
    {
        const bool 基础语义相同 = 种类 == 右.种类
            && 域 == 右.域
            && 主母式 == 右.主母式
            && 粒度 == 右.粒度
            && 基准类型 == 右.基准类型
            && 时间归一方式 == 右.时间归一方式
            && 值形态 == 右.值形态
            && 来源动态层级 == 右.来源动态层级
            && 来源动态路径签名 == 右.来源动态路径签名
            && 私有_引用键(主体) == 私有_引用键(右.主体)
            && 私有_引用键(客体) == 私有_引用键(右.客体)
            && 私有_引用键(左对象) == 私有_引用键(右.左对象)
            && 私有_引用键(右对象) == 私有_引用键(右.右对象);
        if (!基础语义相同) return false;

        switch (值形态) {
        case 枚举_二次特征值形态::布尔:
            return 是否满足 == 右.是否满足;
        case 枚举_二次特征值形态::区间:
            return 标量区间.has_value() == 右.标量区间.has_value()
                && (!标量区间.has_value()
                    || (标量区间->低值 == 右.标量区间->低值
                        && 标量区间->高值 == 右.标量区间->高值));
        case 枚举_二次特征值形态::枚举:
        case 枚举_二次特征值形态::签名:
            return 离散编码 == 右.离散编码 && 离散语义键 == 右.离散语义键;
        default:
            return 标量值 == 右.标量值;
        }
    }

    bool 模板匹配(const 二次特征主信息类& 模板) const
    {
        if (模板.种类 != 枚举_二次特征种类::未定义 && 种类 != 模板.种类) return false;
        if (模板.域 != 枚举_二次特征域::未定义 && 域 != 模板.域) return false;
        if (模板.主母式 != 枚举_二次特征主母式::未定义 && 主母式 != 模板.主母式) return false;
        if (模板.粒度 != 枚举_二次特征粒度::未定义 && 粒度 != 模板.粒度) return false;
        if (模板.基准类型 != 枚举_二次特征基准类型::未定义 && 基准类型 != 模板.基准类型) return false;
        if (模板.时间归一方式 != 枚举_时间归一方式::无 && 时间归一方式 != 模板.时间归一方式) return false;
        if (模板.值形态 != 枚举_二次特征值形态::未定义 && 值形态 != 模板.值形态) return false;

        if (!私有_引用匹配(主体, 模板.主体)) return false;
        if (!私有_引用匹配(客体, 模板.客体)) return false;
        if (!私有_引用匹配(左对象, 模板.左对象)) return false;
        if (!私有_引用匹配(右对象, 模板.右对象)) return false;

        if (模板.来源动态层级 != 0 && 来源动态层级 != 模板.来源动态层级) return false;
        if (模板.来源动态路径签名 != 0 && 来源动态路径签名 != 模板.来源动态路径签名) return false;
        if (!模板.离散语义键.empty() && 离散语义键 != 模板.离散语义键) return false;
        if (模板.离散编码 != 0 && 离散编码 != 模板.离散编码) return false;
        if (!模板.度量签名_链键.empty() && 度量签名_链键 != 模板.度量签名_链键) return false;

        if (模板.标量区间.has_value() && 模板.标量区间->有效()) {
            if (标量值 < 模板.标量区间->低值 || 标量值 > 模板.标量区间->高值) {
                return false;
            }
        }

        if (模板.值形态 == 枚举_二次特征值形态::布尔 && 是否满足 != 模板.是否满足) {
            return false;
        }

        return true;
    }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段) const override {
        auto* 右 = dynamic_cast<二次特征主信息类*>(对象);
        if (!右) return -1;
        const auto 左签名 = 度量签名_链键.empty() ? 私有_生成度量签名_链键() : 度量签名_链键;
        const auto 右签名 = 右->度量签名_链键.empty() ? 右->私有_生成度量签名_链键() : 右->度量签名_链键;
        if (左签名 != 右签名)
            return 左签名 < 右签名 ? -1 : 1;

        switch (值形态) {
        case 枚举_二次特征值形态::布尔:
            if (是否满足 == 右->是否满足) return 0;
            return 是否满足 ? 1 : -1;
        case 枚举_二次特征值形态::区间:
            if (标量区间.has_value() != 右->标量区间.has_value()) {
                return 标量区间.has_value() ? 1 : -1;
            }
            if (!标量区间.has_value()) return 0;
            if (标量区间->低值 != 右->标量区间->低值) {
                return 标量区间->低值 < 右->标量区间->低值 ? -1 : 1;
            }
            if (标量区间->高值 != 右->标量区间->高值) {
                return 标量区间->高值 < 右->标量区间->高值 ? -1 : 1;
            }
            return 0;
        case 枚举_二次特征值形态::枚举:
        case 枚举_二次特征值形态::签名:
            if (离散编码 != 右->离散编码)
                return 离散编码 < 右->离散编码 ? -1 : 1;
            if (离散语义键 == 右->离散语义键) return 0;
            return 离散语义键 < 右->离散语义键 ? -1 : 1;
        default:
            if (标量值 == 右->标量值) return 0;
            return 标量值 < 右->标量值 ? -1 : 1;
        }
    }

private:
    template<class T节点>
    static std::string 私有_引用键(const 可解析引用<T节点>& 引用)
    {
        if (!引用.主键.empty()) return 引用.主键;
        return 引用.指针 ? 引用.指针->获取主键() : std::string{};
    }

    template<class T节点>
    static bool 私有_引用匹配(const 可解析引用<T节点>& 当前值, const 可解析引用<T节点>& 模板值)
    {
        const auto 模板键 = 私有_引用键(模板值);
        if (模板键.empty()) return true;
        return 私有_引用键(当前值) == 模板键;
    }

    std::string 私有_生成度量签名_链键() const
    {
        std::string out;
        out.reserve(256);

        const auto 追加片段 = [&](const std::string& 片段) {
            if (!out.empty()) out.push_back('|');
            out += 片段;
        };

        追加片段(std::to_string(static_cast<int>(域)));
        追加片段(std::to_string(static_cast<int>(种类)));
        追加片段(std::to_string(static_cast<int>(主母式)));
        追加片段(std::to_string(static_cast<int>(粒度)));
        追加片段(std::to_string(static_cast<int>(基准类型)));
        追加片段(std::to_string(static_cast<int>(时间归一方式)));
        追加片段(std::to_string(static_cast<int>(值形态)));
        追加片段(私有_引用键(主体));
        追加片段(私有_引用键(客体));
        追加片段(私有_引用键(左对象));
        追加片段(私有_引用键(右对象));
        追加片段(私有_引用键(来源起始状态));
        追加片段(私有_引用键(来源结果状态));
        追加片段(私有_引用键(来源动态));
        追加片段(私有_引用键(来源场景));
        追加片段(std::to_string(来源动态层级));
        追加片段(std::to_string(来源动态路径签名));
        return out;
    }
};

class 因果实例主信息类 : public 基础信息基类 {
public:
    枚举_因果锚点类型 锚点类型 = 枚举_因果锚点类型::未定义;
    可解析引用<基础信息节点类> 锚点信息{};
    std::uint32_t 状态层级 = 0;

    结构_时间段 条件时间段{};
    时间戳 动作时间 = 0;
    时间戳 结果时间 = 0;

    std::vector<可解析引用<状态节点类>> 条件状态{};
    可解析引用<基础信息节点类> 动作主体{};
    可解析引用<基础信息节点类> 动作语义{};
    std::string 动作语义键{};
    std::vector<可解析引用<状态节点类>> 结果状态{};
    可解析引用<状态节点类> 主结果状态{};

    枚举_因果来源类型 来源类型 = 枚举_因果来源类型::未定义;
    std::uint64_t 来源任务主键 = 0;
    std::uint64_t 来源方法主键 = 0;
    std::int64_t 置信度 = 0;
    bool 已验证 = false;

    因果实例主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::因果) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段) const override {
        return dynamic_cast<因果实例主信息类*>(对象) ? 0 : -1;
    }
};

class 因果模板主信息类 : public 基础信息基类 {
public:
    枚举_因果锚点类型 适用锚点类型 = 枚举_因果锚点类型::未定义;
    std::uint32_t 适用层级 = 0;

    std::vector<可解析引用<二次特征节点类>> 条件模板{};
    可解析引用<基础信息节点类> 动作模板{};
    std::vector<可解析引用<二次特征节点类>> 结果模板{};
    std::vector<可解析引用<因果实例节点类>> 证据实例{};

    std::uint64_t 成立次数 = 0;
    std::uint64_t 失败次数 = 0;
    时间戳 最近命中时间 = 0;
    时间戳 最近失败时间 = 0;
    std::int64_t 稳定度 = 0;

    因果模板主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::因果) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段) const override {
        return dynamic_cast<因果模板主信息类*>(对象) ? 0 : -1;
    }
};

