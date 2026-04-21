module;

#include <cassert>
#include <functional>
#include <mutex>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "日志接入.h"
#include "本能方法类.h"
#include "世界树类.h"
#include "需求类.h"
#include "任务管理最小原语模块.h"
#include "语素类.h"

module 任务管理任务模块;

import 二次特征应用模块;
import 自我模块;
import 自我_执行桥模块;
import 自我模块.初始化;
import 学习调度模块;
import 学习任务模块;
import 学习执行模块;
import 任务模块.运行包;

#include "任务管理任务模块_治理快照.cpp"

namespace 任务管理任务模块_治理缓存 {
    void 缓存最近治理结果(const 结构_任务管理结果& 结果) noexcept;
}

namespace {
    using 任务节点 = 任务类::节点类;
    using 方法节点 = 方法类::节点类;
    using 需求节点 = 需求类::节点类;

    enum class 枚举_任务管理特征目标宿主 {
        未定义 = 0,
        宿主任务治理,
        主体桥接,
        任务账本,
        方法资产,
    };

    struct 结构_M2治理阶段状态 {
        任务节点* 当前管理任务 = nullptr;
        任务节点* 宿主任务 = nullptr;
        任务节点* 待回接业务子任务 = nullptr;
        bool 已受上位输入约束 = false;
        bool 桥接补层允许继续 = false;
    };

    struct 结构_M9请求触发态 {
        bool 命中显式事件 = false;
        结构_任务管理显式事件控制 显式事件{};
        bool 命中控制请求 = false;
        结构_任务管理控制请求 控制请求{};
        bool 命中恢复重建 = false;
        结构_治理恢复快照 恢复快照{};
    };

    enum class 枚举_最小原语写回目标宿主 : std::uint8_t {
        按语义宿主 = 0,
        宿主任务治理 = 1,
        主体桥接 = 2,
        方法资产 = 3,
    };

    struct 结构_最小原语边界执行上下文 {
        自我类* 自我对象 = nullptr;
        任务节点* 宿主任务 = nullptr;
        时间戳 当前时间 = 0;
        const std::string* 调用点 = nullptr;
        结构_任务管理结果* 结果 = nullptr;
        枚举_任务管理最小原语ID 原语ID = 枚举_任务管理最小原语ID::未定义;
        枚举_动作事件相位 相位 = 枚举_动作事件相位::未定义;
    };

    struct 结构_最小原语I64写回执行上下文 {
        自我类* 自我对象 = nullptr;
        枚举_最小原语写回目标宿主 目标宿主 = 枚举_最小原语写回目标宿主::按语义宿主;
        const 词性节点类* 特征类型 = nullptr;
        I64 新值 = 0;
        const std::string* 动作语义键 = nullptr;
        方法节点* 来源方法 = nullptr;
        任务节点* 来源任务 = nullptr;
        时间戳 当前时间 = 0;
        const std::string* 调用点 = nullptr;
        结构_任务管理结果* 结果 = nullptr;
    };

    struct 结构_最小原语指针写回执行上下文 {
        自我类* 自我对象 = nullptr;
        枚举_最小原语写回目标宿主 目标宿主 = 枚举_最小原语写回目标宿主::按语义宿主;
        const 词性节点类* 特征类型 = nullptr;
        const void* 新指针值 = nullptr;
        const std::string* 新值标题 = nullptr;
        const std::string* 动作语义键 = nullptr;
        方法节点* 来源方法 = nullptr;
        任务节点* 来源任务 = nullptr;
        时间戳 当前时间 = 0;
        const std::string* 调用点 = nullptr;
        结构_任务管理结果* 结果 = nullptr;
        const 词性节点类* 来源动作名覆盖 = nullptr;
        枚举_动作事件相位 来源相位 = 枚举_动作事件相位::完成;
    };

    void 私有_追加最近反馈片段(
        结构_任务管理结果& 结果,
        const std::string& 片段) noexcept;
    void 私有_M9_装配请求基本骨架(
        const 结构_任务管理上下文& 上下文,
        时间戳 当前时间,
        std::uint32_t 提交序号,
        结构_任务管理请求& 输出) noexcept;
    结构_M9请求触发态 私有_M9_消费请求触发态() noexcept;
    void 私有_M9_装配请求触发信息(
        结构_任务管理请求& 输出,
        const 结构_M9请求触发态& 触发态) noexcept;
    void 私有_M9_生成恢复重建请求摘要(
        结构_任务管理请求& 输出,
        const 结构_M9请求触发态& 触发态) noexcept;
    void 私有_M9_生成请求补充材料摘要(
        const 结构_任务管理上下文& 上下文,
        结构_任务管理请求& 输出) noexcept;
    bool 私有_P0_控制请求有效(const 结构_任务管理控制请求& 请求) noexcept;
    bool 私有_P0_控制承接有效(const 结构_任务管理控制承接记录& 记录) noexcept;
    void 私有_P0_覆盖控制请求(
        结构_任务管理控制请求& 目标,
        const 结构_任务管理控制请求& 来源) noexcept;
    void 私有_P0_覆盖控制承接(
        结构_任务管理控制承接记录& 目标,
        const 结构_任务管理控制承接记录& 来源) noexcept;
    std::string 私有_P0_控制请求摘要(const 结构_任务管理控制请求& 请求) noexcept;
    std::string 私有_P0_控制承接摘要(const 结构_任务管理控制承接记录& 记录) noexcept;
    std::string 私有_M10_主体长期策略摘要(
        const 结构_任务管理主体虚拟存在& 主体) noexcept;
    std::string 私有_M10_任务状态签名(const 状态节点类* 状态) noexcept;
    std::string 私有_M10_业务差额签名(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept;
    std::string 私有_M10_当前条件签名(const 结构_任务管理结果& 结果) noexcept;
    std::string 私有_M10_有效正式方法标题(const 结构_任务管理结果& 结果) noexcept;
    std::string 私有_M10_长期策略摘要版本(const 结构_任务管理结果& 结果) noexcept;
    bool 私有_M10_存在业务推进压力(const 结构_任务管理结果& 结果) noexcept;
    bool 私有_M10_当前方法命中主体长期策略(const 结构_任务管理结果& 结果) noexcept;
    bool 私有_M10_长期策略驱动业务子任务派生(const 结构_任务管理结果& 结果) noexcept;
    bool 私有_M10_长期策略可靠度允许回填(const 结构_任务管理主体虚拟存在& 主体) noexcept;
    bool 私有_M10_主体长期策略允许直放后段(const 结构_任务管理结果& 结果) noexcept;
    void 私有_M10_刷新长期策略权重(结构_任务管理主体虚拟存在& 主体) noexcept;
    void 私有_M10_合并长期策略范围签名(
        std::string& 范围摘要,
        const std::string& 新签名) noexcept;
    bool 私有_M10_当前轮形成有效长期策略推进(const 结构_任务管理结果& 结果) noexcept;
    std::string 私有_M10_当前轮长期策略失效原因(const 结构_任务管理结果& 结果) noexcept;
    void 私有_M10_回写长期策略可靠度(
        任务节点* 宿主任务,
        时间戳 当前时间,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_M10_沉淀正式学习反馈到主体(
        自我类& 自我对象,
        const 结构_学习方法资产提交输入& 输入,
        const 结构_学习方法资产提交结果& 资产结果,
        时间戳 当前时间,
        结构_任务管理主体虚拟存在* 输出主体 = nullptr,
        std::string* 输出摘要 = nullptr) noexcept;
    bool 私有_执行筹办模块(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept;
    void 私有_执行可执行判断模块(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept;
    void 私有_执行允许执行判断模块(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept;
    bool 私有_学习方法有动作骨架(const 方法节点* 方法头) noexcept;
    bool 私有_学习方法仅需求位(const 方法节点* 方法头) noexcept;
    bool 私有_学习方法可直接进入学习(const 方法节点* 方法头) noexcept;
    I64 私有_学习方法条件数量(const 方法节点* 方法头) noexcept;
    I64 私有_学习方法结果数量(const 方法节点* 方法头) noexcept;
    bool 私有_学习方法结果已具备(const 方法节点* 方法头) noexcept;
    bool 私有_学习方法已具备可用骨架(const 方法节点* 方法头) noexcept;
    void 私有_执行执行模块(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept;
    void 私有_执行反馈模块(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept;
    bool 私有_上下文允许普通执行前筹办(const 结构_任务管理上下文& 上下文) noexcept;
    void 私有_M2_绑定宿主方法阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept;
    void 私有_M2_刷新局部可执行判断阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept;
    void 私有_M2_应用允许执行判断阶段(
        时间戳 当前时间,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept;
    bool 私有_M9_执行前段治理阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept;
    bool 私有_M9_事件轮禁止业务扩张(const 结构_任务管理结果& 结果) noexcept;
    bool 私有_M9_步骤语义允许直接执行(枚举_任务管理步骤语义类型 类型) noexcept;
    bool 私有_M9_结果节点已是当前步骤实际结果(
        const 任务节点* 当前步骤,
        const 任务节点* 最新结果) noexcept;
    bool 私有_M9_执行当前任务步骤(
        任务节点* 宿主任务,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_M9_执行后段治理动作阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept;
    void 私有_M9_执行学习承接与业务派生阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态,
        bool 允许业务派生) noexcept;

    bool 私有_读取状态节点任务状态(const 状态节点类* 状态节点, 枚举_任务状态& 输出) noexcept;
    bool 私有_任务节点归属宿主任务(const 任务节点* 节点, const 任务节点* 宿主任务) noexcept;
    bool 私有_方法属于宿主任务(const 方法节点* 方法, const 任务节点* 宿主任务) noexcept;
    void 私有_按宿主清洗治理上下文镜像(结构_任务管理上下文& 上下文) noexcept;
    bool 私有_任务节点目标结果已对齐(const 任务节点* 任务) noexcept;
    bool 私有_任务节点目标结果待推进(const 任务节点* 任务) noexcept;
    struct 结构_P0_升阶候选分析 {
        std::size_t 同轴同向候选数 = 0;
        bool 存在可比对 = false;
        bool 存在同轴反向候选 = false;
        std::string 可比基准摘要{};
        std::string 排序原因{};
        std::string 未决原因{};
        std::vector<const 结构_原语候选项*> 入选候选{};
        std::vector<const 结构_原语候选项*> 可比候选{};
        const 结构_原语候选项* 首可比候选 = nullptr;
        const 结构_原语候选项* 次可比候选 = nullptr;
        bool 已完成高阶排序 = false;
        bool 是否等效 = false;
        bool 是否过头 = false;
        std::int64_t 排序置信度_Q10000 = 0;
        std::string 胜出候选主键{};
        std::string 排序关系{};
    };

    bool 私有_P0_分析升阶候选(
        const 结构_任务管理请求& 请求,
        结构_P0_升阶候选分析& 输出) noexcept;
    std::size_t 私有_P0_统计升阶同向候选数(const 结构_任务管理请求& 请求) noexcept;
    std::string 私有_P0_推导需求轴键(const 结构_任务管理请求& 请求) noexcept;
    void 私有_P0_刷新请求原语锚点(结构_任务管理请求& 请求) noexcept;
    bool 私有_P0_生成高阶排序请求(
        const 结构_任务管理请求& 请求,
        结构_高阶排序请求& 输出) noexcept;
    bool 私有_P0_生成高阶排序结果(
        const 结构_任务管理请求& 请求,
        结构_高阶排序结果& 输出) noexcept;
    std::string 私有_P0_升阶候选摘要(const 结构_任务管理请求& 请求) noexcept;
    std::string 私有_P0_高阶排序请求摘要(const 结构_高阶排序请求& 请求) noexcept;
    std::string 私有_P0_高阶排序结果摘要(const 结构_高阶排序结果& 结果) noexcept;
    std::string 私有_P0_高阶胜出绑定摘要(const 结构_高阶胜出绑定& 绑定) noexcept;
    std::string 私有_原语类型文本(枚举_原语类型 类型) noexcept;
    const char* 私有_P0_原语候选比较基准类型文本(枚举_原语候选比较基准类型 类型) noexcept;
    const char* 私有_P0_原语候选来源锚点类型文本(枚举_原语候选来源锚点类型 类型) noexcept;
    void 私有_P1_覆盖任务治理二次特征证据(
        结构_任务治理二次特征证据& 目标,
        const 结构_任务治理二次特征证据& 来源) noexcept;
    void 私有_P0_覆盖高阶排序请求(
        结构_高阶排序请求& 目标,
        const 结构_高阶排序请求& 来源) noexcept;
    void 私有_P0_覆盖高阶排序结果(
        结构_高阶排序结果& 目标,
        const 结构_高阶排序结果& 来源) noexcept;
    void 私有_P0_覆盖高阶胜出绑定(
        结构_高阶胜出绑定& 目标,
        const 结构_高阶胜出绑定& 来源) noexcept;
    void 私有_P0_覆盖原语判定汇总(
        结构_原语判定汇总& 目标,
        const 结构_原语判定汇总& 来源) noexcept;
    方法节点* 私有_P0_归一到方法首节点(方法节点* 节点) noexcept;
    const 结构_原语候选项* 私有_P0_查找升阶候选_按主键(
        const 结构_任务管理请求& 请求,
        const std::string& 候选主键) noexcept;
    bool 私有_P0_解析高阶胜出绑定(
        const 结构_任务管理请求& 请求,
        const 结构_高阶排序结果& 高阶排序结果,
        结构_高阶胜出绑定& 输出) noexcept;
    bool 私有_P0_执行原语(
        const 结构_任务管理请求& 请求,
        枚举_原语类型 类型,
        结构_原语判定结果& 输出) noexcept;
    bool 私有_P0_汇总原语(
        const std::vector<结构_原语判定结果>& 结果集,
        结构_原语判定汇总& 输出) noexcept;
    bool 私有_P0_生成原语判定汇总(
        const 结构_任务管理请求& 请求,
        结构_原语判定汇总& 输出) noexcept;
    bool 私有_P0_选择下一步(
        const 结构_任务管理请求& 请求,
        const 结构_原语判定汇总& 汇总,
        结构_任务管理单步决策& 输出) noexcept;
    枚举_任务管理本能能力缺口类型 私有_P0_当前本能缺口(const 结构_任务管理结果& 结果) noexcept;
    void 私有_设定治理结论(
        结构_任务管理结果& 结果,
        枚举_任务管理功能域 功能域,
        枚举_任务管理缺口类型 缺口类型,
        枚举_任务管理下一步去向 下一步去向,
        枚举_任务管理治理状态迁移 状态迁移,
        枚举_任务管理总控结果 总控结果,
        枚举_任务管理反馈类型 反馈类型,
        std::string 反馈摘要) noexcept;
    void 私有_刷新分身继承位图(
        结构_任务管理分身继承面& 分身,
        const 结构_任务管理上下文& 上下文) noexcept;
    std::string 私有_主体化输入条件主键(
        const 结构_任务管理上下文& 上下文,
        const 结构_任务管理分身继承面& 分身) noexcept;
    std::string 私有_主体化结果主键(
        const 结构_任务管理分身继承面& 分身,
        const 结构_任务管理上下文& 上下文) noexcept;
    枚举_任务管理特征目标宿主 私有_查询任务管理特征目标宿主(
        const 词性节点类* 特征类型) noexcept;
    bool 私有_校验任务管理特征目标宿主(
        const 词性节点类* 特征类型,
        枚举_任务管理特征目标宿主 期望宿主) noexcept;
    const 特征节点类* 私有_查找宿主任务治理特征节点(
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型) noexcept;
    特征节点类* 私有_确保宿主任务治理特征节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept;
    bool 私有_读取宿主任务治理I64特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64& 输出值,
        const std::string& 调用点) noexcept;
    bool 私有_读取宿主任务治理指针特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        std::uintptr_t& 输出值,
        const std::string& 调用点) noexcept;
    const 特征节点类* 私有_查找主体桥接特征节点(
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型) noexcept;
    特征节点类* 私有_确保主体桥接特征节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept;
    const 特征节点类* 私有_查找方法资产特征节点(
        const 方法节点* 方法头,
        const 词性节点类* 特征类型) noexcept;
    特征节点类* 私有_确保方法资产特征节点(
        方法节点* 方法头,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept;
    bool 私有_读取主体桥接I64特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64& 输出值,
        const std::string& 调用点) noexcept;
    bool 私有_读取主体桥接指针特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        std::uintptr_t& 输出值,
        const std::string& 调用点) noexcept;
    场景节点类* 私有_任务场景(自我类& 自我对象, const 任务节点* 节点) noexcept;
    存在节点类* 私有_确保任务虚拟存在(
        自我类& 自我对象,
        任务节点* 任务,
        const std::string& 调用点) noexcept;
    bool 私有_P1_刷新任务治理二次特征证据(
        自我类& 自我对象,
        结构_任务管理请求& 请求,
        const std::string& 调用点) noexcept;
    bool 私有_记录宿主任务治理证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_记录宿主任务治理证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖 = nullptr,
        枚举_动作事件相位 来源相位 = 枚举_动作事件相位::完成) noexcept;
    bool 私有_记录主体桥接证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_记录方法资产证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_记录主体桥接证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖 = nullptr,
        枚举_动作事件相位 来源相位 = 枚举_动作事件相位::完成) noexcept;
    bool 私有_按语义宿主分派记录治理证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_按语义宿主分派记录治理证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖 = nullptr,
        枚举_动作事件相位 来源相位 = 枚举_动作事件相位::完成) noexcept;
    bool 私有_写入宿主任务治理当前值_I64(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64 新值,
        时间戳 now,
        const std::string& 调用点) noexcept;
    bool 私有_写入宿主任务治理当前值_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        时间戳 now,
        const std::string& 调用点) noexcept;
    bool 私有_写入宿主任务治理当前值_指针(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        时间戳 now,
        const std::string& 调用点) noexcept;
    bool 私有_写入宿主任务治理当前值_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        时间戳 now,
        const std::string& 调用点) noexcept;
    bool 私有_记录任务管理方法位专项动态(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        枚举_动作事件相位 相位) noexcept;
    bool 私有_执行最小原语边界记录体(void* 上下文) noexcept;
    bool 私有_执行最小原语I64写回体(void* 上下文) noexcept;
    bool 私有_执行最小原语指针写回体(void* 上下文) noexcept;
    bool 私有_通过执行壳写回I64(
        自我类& 自我对象,
        枚举_最小原语写回目标宿主 目标宿主,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;
    bool 私有_通过执行壳写回指针(
        自我类& 自我对象,
        枚举_最小原语写回目标宿主 目标宿主,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖 = nullptr,
        枚举_动作事件相位 来源相位 = 枚举_动作事件相位::完成) noexcept;
    void 私有_写回治理稳定字段(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept;

    std::mutex 私有_任务管理请求序号锁{};
    std::uint32_t 私有_任务管理请求序号 = 0;
    std::mutex 私有_任务管理主体表锁{};
    std::unordered_map<std::uint64_t, 结构_任务管理主体虚拟存在> 私有_任务管理主体表{};
    std::mutex 私有_治理显式事件控制锁{};
    结构_任务管理显式事件控制 私有_治理显式事件控制{};
    std::mutex 私有_治理控制请求锁{};
    结构_任务管理控制请求 私有_治理控制请求{};
    std::mutex 私有_最近治理控制承接锁{};
    结构_任务管理控制承接记录 私有_最近治理控制承接{};
    struct 结构_任务管理恢复重建控制 {
        bool 启用 = false;
        结构_治理恢复快照 快照{};
    };
    std::mutex 私有_治理恢复重建控制锁{};
    结构_任务管理恢复重建控制 私有_治理恢复重建控制{};

    void 私有_写入引用(可解析引用<方法节点类>& 输出, 方法节点* 节点) noexcept
    {
        输出.指针 = reinterpret_cast<方法节点类*>(节点);
        输出.主键 = 节点 ? 节点->获取主键() : std::string{};
    }

    void 私有_写入引用(可解析引用<任务节点类>& 输出, 任务节点* 节点) noexcept
    {
        输出.指针 = reinterpret_cast<任务节点类*>(节点);
        输出.主键 = 节点 ? 节点->获取主键() : std::string{};
    }

    template<class T节点>
    void 私有_写入引用(可解析引用<T节点>& 输出, T节点* 节点) noexcept
    {
        输出.指针 = 节点;
        输出.主键 = 节点 ? 节点->获取主键() : std::string{};
    }

    状态节点类* 私有_解析状态引用(const 可解析引用<状态节点类>& 引用) noexcept
    {
        if (引用.指针) {
            return 引用.指针;
        }
        if (引用.主键.empty()) {
            return nullptr;
        }
        if (auto* 基础节点 = 世界树.基础信息().查找主键(引用.主键)) {
            return static_cast<状态节点类*>(基础节点);
        }
        return nullptr;
    }

    状态节点类* 私有_取首个有效状态(
        const std::vector<可解析引用<状态节点类>>& 列表) noexcept
    {
        for (const auto& 引用 : 列表) {
            if (auto* 状态节点 = 私有_解析状态引用(引用)) {
                return 状态节点;
            }
        }
        return nullptr;
    }

    void 私有_追加状态引用(
        std::vector<可解析引用<状态节点类>>& 输出,
        状态节点类* 状态节点) noexcept
    {
        if (!状态节点) {
            return;
        }

        const auto 主键 = 状态节点->获取主键();
        const bool 已存在 = std::any_of(输出.begin(), 输出.end(), [&](const auto& 已有) {
            return 已有.指针 == 状态节点
                || (!主键.empty() && 已有.主键 == 主键);
        });
        if (已存在) {
            return;
        }

        可解析引用<状态节点类> 引用{};
        私有_写入引用(引用, 状态节点);
        输出.push_back(std::move(引用));
    }

    void 私有_P2_追加二次特征引用(
        std::vector<可解析引用<二次特征节点类>>& 输出,
        二次特征节点类* 二次特征) noexcept
    {
        if (!二次特征) {
            return;
        }

        const auto 主键 = 二次特征->获取主键();
        const bool 已存在 = std::any_of(输出.begin(), 输出.end(), [&](const auto& 已有) {
            return 已有.指针 == 二次特征
                || (!主键.empty() && 已有.主键 == 主键);
        });
        if (已存在) {
            return;
        }

        可解析引用<二次特征节点类> 引用{};
        私有_写入引用(引用, 二次特征);
        输出.push_back(std::move(引用));
    }

    void 私有_P2_吸收二次特征引用列表(
        std::vector<可解析引用<二次特征节点类>>& 目标,
        const std::vector<可解析引用<二次特征节点类>>& 来源) noexcept
    {
        for (const auto& 引用 : 来源) {
            if (引用.指针) {
                私有_P2_追加二次特征引用(目标, 引用.指针);
                continue;
            }
            if (引用.主键.empty()) {
                continue;
            }

            const bool 已存在 = std::any_of(目标.begin(), 目标.end(), [&](const auto& 已有) {
                return (!已有.主键.empty() && 已有.主键 == 引用.主键);
            });
            if (!已存在) {
                目标.push_back(引用);
            }
        }
    }

    template<class T节点, class T谓词>
    T节点* 私有_查找直接子节点_按谓词(T节点* 父节点, T谓词&& 谓词) noexcept
    {
        if (!父节点 || !父节点->子) return nullptr;

        auto* 首节点 = static_cast<T节点*>(父节点->子);
        auto* 当前 = 首节点;
        do {
            if (谓词(当前)) return 当前;
            当前 = static_cast<T节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    template<class T节点, class T回调>
    void 私有_枚举直接子节点(const T节点* 父节点, T回调&& 回调) noexcept
    {
        if (!父节点 || !父节点->子) {
            return;
        }

        auto* 首节点 = static_cast<const T节点*>(父节点->子);
        auto* 当前 = 首节点;
        do {
            回调(当前);
            当前 = static_cast<const T节点*>(当前->下);
        } while (当前 && 当前 != 首节点);
    }

    bool 私有_词性相同(const 词性节点类* a, const 词性节点类* b) noexcept
    {
        if (a == b) return true;
        if (!a || !b) return false;
        return a->获取主键() == b->获取主键();
    }

    时间戳 私有_归一化时间(时间戳 now) noexcept
    {
        return now != 0 ? now : 结构体_时间戳::当前_微秒();
    }

    const 词性节点类* 私有_管理特征_当前功能域() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前功能域", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前缺口类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前缺口类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前下一步去向() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前下一步去向", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前治理状态迁移() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前治理状态迁移", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近功能域() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近功能域", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近总控结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近总控结果", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近反馈类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近反馈类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近根层重判结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近根层重判结果", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近执行前门控结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近执行前门控结果", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前方法来源() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前方法来源", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前步骤位类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前步骤位类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前步骤语义类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前步骤语义类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前步骤可复用() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前步骤可复用", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近结果命中当前治理() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近结果命中当前治理", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前动作长时间无推进() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前动作长时间无推进", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前动作需重试() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前动作需重试", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前需等待() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前需等待", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前需收束() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前需收束", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_可继续推进() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_可继续推进", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前可回接最近结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前可回接最近结果", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_稳定等待态复盘学习() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_稳定等待态复盘学习", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_业务子任务已完成() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_业务子任务已完成", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_业务子任务应转执行() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_业务子任务应转执行", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前宿主目标结果比较() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前宿主目标结果比较", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前后果治理() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前后果治理", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前主体ID() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前主体ID", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前分身ID() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前分身ID", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前方法需求位() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前方法需求位", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前学习承接类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前学习承接类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前待学习方法头() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前待学习方法头", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前学习兜底方法() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前学习兜底方法", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前学习目标类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前学习目标类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前学习需求数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前学习需求数量", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前学习动作数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前学习动作数量", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前学习程序裁决() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前学习程序裁决", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前学习方法有动作骨架() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前学习方法有动作骨架", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前学习方法条件数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前学习方法条件数量", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前学习方法结果数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前学习方法结果数量", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前学习方法可用() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前学习方法可用", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前已切换到兜底可执行方法() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前已切换到兜底可执行方法", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前本能能力缺口类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前本能能力缺口类型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前输入条件主键() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前输入条件主键", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前治理态型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前治理态型", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前最小原语动态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前最小原语动态", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前宿主任务指针() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前宿主任务指针", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前方法指针() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前方法指针", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前方法位专项动态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前方法位专项动态", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_当前步骤指针() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_当前步骤指针", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_最近结果指针() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_最近结果指针", "名词");
        return s_词;
    }

    const 词性节点类* 私有_管理特征_宿主任务状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理_宿主任务状态", "名词");
        return s_词;
    }

    const 词性节点类* 私有_任务特征_目标状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务目标状态", "名词");
        return s_词;
    }

    const 词性节点类* 私有_任务特征_结果状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务结果状态", "名词");
        return s_词;
    }

    const 词性节点类* 私有_方法特征_方法类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法类型", "名词");
        return s_词;
    }

    枚举_任务管理特征目标宿主 私有_查询任务管理特征目标宿主(
        const 词性节点类* 特征类型) noexcept
    {
        if (!特征类型) {
            return 枚举_任务管理特征目标宿主::未定义;
        }

        if (特征类型 == 私有_任务特征_目标状态()
            || 特征类型 == 私有_任务特征_结果状态()) {
            return 枚举_任务管理特征目标宿主::任务账本;
        }

        if (特征类型 == 私有_方法特征_方法类型()) {
            return 枚举_任务管理特征目标宿主::方法资产;
        }

        if (特征类型 == 私有_管理特征_当前主体ID()
            || 特征类型 == 私有_管理特征_当前分身ID()
            || 特征类型 == 私有_管理特征_当前方法需求位()
            || 特征类型 == 私有_管理特征_当前本能能力缺口类型()
            || 特征类型 == 私有_管理特征_当前输入条件主键()
            || 特征类型 == 私有_管理特征_当前治理态型()
            || 特征类型 == 私有_管理特征_当前方法位专项动态()
            || 特征类型 == 私有_管理特征_当前最小原语动态()) {
            return 枚举_任务管理特征目标宿主::主体桥接;
        }

        if (特征类型 == 私有_管理特征_当前功能域()
            || 特征类型 == 私有_管理特征_当前缺口类型()
            || 特征类型 == 私有_管理特征_当前下一步去向()
            || 特征类型 == 私有_管理特征_当前治理状态迁移()
            || 特征类型 == 私有_管理特征_最近功能域()
            || 特征类型 == 私有_管理特征_最近总控结果()
            || 特征类型 == 私有_管理特征_最近反馈类型()
            || 特征类型 == 私有_管理特征_最近根层重判结果()
            || 特征类型 == 私有_管理特征_最近执行前门控结果()
            || 特征类型 == 私有_管理特征_当前方法来源()
            || 特征类型 == 私有_管理特征_当前步骤位类型()
            || 特征类型 == 私有_管理特征_当前步骤语义类型()
            || 特征类型 == 私有_管理特征_当前步骤可复用()
            || 特征类型 == 私有_管理特征_最近结果命中当前治理()
            || 特征类型 == 私有_管理特征_当前动作长时间无推进()
            || 特征类型 == 私有_管理特征_当前动作需重试()
            || 特征类型 == 私有_管理特征_当前需等待()
            || 特征类型 == 私有_管理特征_当前需收束()
            || 特征类型 == 私有_管理特征_可继续推进()
            || 特征类型 == 私有_管理特征_当前可回接最近结果()
            || 特征类型 == 私有_管理特征_稳定等待态复盘学习()
            || 特征类型 == 私有_管理特征_业务子任务已完成()
            || 特征类型 == 私有_管理特征_业务子任务应转执行()
            || 特征类型 == 私有_管理特征_当前宿主目标结果比较()
            || 特征类型 == 私有_管理特征_当前后果治理()
            || 特征类型 == 私有_管理特征_当前学习承接类型()
            || 特征类型 == 私有_管理特征_当前待学习方法头()
            || 特征类型 == 私有_管理特征_当前学习兜底方法()
            || 特征类型 == 私有_管理特征_当前学习目标类型()
            || 特征类型 == 私有_管理特征_当前学习需求数量()
            || 特征类型 == 私有_管理特征_当前学习动作数量()
            || 特征类型 == 私有_管理特征_当前学习程序裁决()
            || 特征类型 == 私有_管理特征_当前学习方法有动作骨架()
            || 特征类型 == 私有_管理特征_当前学习方法条件数量()
            || 特征类型 == 私有_管理特征_当前学习方法结果数量()
            || 特征类型 == 私有_管理特征_当前学习方法可用()
            || 特征类型 == 私有_管理特征_当前已切换到兜底可执行方法()
            || 特征类型 == 私有_管理特征_当前宿主任务指针()
            || 特征类型 == 私有_管理特征_当前方法指针()
            || 特征类型 == 私有_管理特征_当前步骤指针()
            || 特征类型 == 私有_管理特征_最近结果指针()
            || 特征类型 == 私有_管理特征_宿主任务状态()) {
            return 枚举_任务管理特征目标宿主::宿主任务治理;
        }

        return 枚举_任务管理特征目标宿主::未定义;
    }

    bool 私有_校验任务管理特征目标宿主(
        const 词性节点类* 特征类型,
        枚举_任务管理特征目标宿主 期望宿主) noexcept
    {
#ifndef NDEBUG
        const auto 实际宿主 = 私有_查询任务管理特征目标宿主(特征类型);
        assert(
            实际宿主 == 期望宿主
            && "任务管理特征宿主映射与显式写入口不一致");
#else
        (void)特征类型;
        (void)期望宿主;
#endif
        return true;
    }

    const 词性节点类* 私有_任务虚拟存在类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务虚拟存在", "名词");
        return s_词;
    }

    std::string 私有_安全词(const 词性节点类* 词) noexcept
    {
        if (!词) return {};

        try {
            return 语素集.获取词(词);
        }
        catch (...) {
            return {};
        }
    }

    std::string 私有_拼接片段(const std::vector<std::string>& 片段)
    {
        std::ostringstream 输出;
        bool 首段 = true;
        for (const auto& 片段文本 : 片段) {
            if (片段文本.empty()) {
                continue;
            }
            if (!首段) {
                输出 << "；";
            }
            输出 << 片段文本;
            首段 = false;
        }
        return 输出.str();
    }

    std::string 私有_任务标题(const 任务节点* 节点) noexcept
    {
        if (!节点) return "空";
        const auto 标题 = 私有_安全词(节点->主信息.名称);
        return 标题.empty() ? std::string("任务#") + 节点->获取主键() : 标题;
    }

    const 词性节点类* 私有_任务虚拟存在名称词(const 任务节点* 节点) noexcept
    {
        return 语素集.添加词性词("任务账本_" + 私有_任务标题(节点), "专有名词");
    }

    std::string 私有_方法标题(const 方法节点* 节点) noexcept
    {
        if (!节点) return "空";
        const auto 标题 = 私有_安全词(节点->主信息.动作名);
        return 标题.empty() ? std::string("方法#") + 节点->获取主键() : 标题;
    }

    template<class T节点>
    T节点* 私有_获取独立树根(T节点* 节点) noexcept
    {
        while (节点 && 节点->父) {
            节点 = static_cast<T节点*>(节点->父);
        }
        return 节点;
    }

    方法节点* 私有_独立树添加方法子节点(方法节点* 父节点, const 方法信息基类& 主信息)
    {
        if (!父节点) return nullptr;

        auto* 独立根 = 私有_获取独立树根(父节点);
        if (!独立根) return nullptr;

        方法类 临时树{};
        auto* 临时根 = 临时树.根指针;
        临时树.根指针 = 独立根;

        auto* 新节点 = static_cast<方法节点*>(临时树.添加子节点(父节点, 主信息));

        临时树.根指针 = 临时根;
        return 新节点;
    }

    template<class T树, class T节点, class T主信息>
    T节点* 私有_独立树添加子节点(T节点* 父节点, const T主信息& 主信息)
    {
        if (!父节点) return nullptr;

        auto* 独立根 = 私有_获取独立树根(父节点);
        if (!独立根) return nullptr;

        T树 临时树{};
        auto* 临时根 = 临时树.根指针;
        临时树.根指针 = 独立根;

        auto* 新节点 = static_cast<T节点*>(临时树.添加子节点(父节点, 主信息));

        临时树.根指针 = 临时根;
        return 新节点;
    }

    template<class T节点>
    bool 私有_是祖先节点(const T节点* 祖先节点, const T节点* 当前节点) noexcept
    {
        auto* 游标 = 当前节点;
        while (游标) {
            if (游标 == 祖先节点) {
                return true;
            }
            游标 = static_cast<const T节点*>(游标->父);
        }
        return false;
    }

    std::string 私有_任务状态文本(枚举_任务状态 状态) noexcept
    {
        switch (状态) {
        case 枚举_任务状态::未启动: return "未启动";
        case 枚举_任务状态::运行中: return "运行中";
        case 枚举_任务状态::挂起: return "挂起";
        case 枚举_任务状态::完成: return "完成";
        case 枚举_任务状态::失败: return "失败";
        case 枚举_任务状态::取消: return "取消";
        case 枚举_任务状态::超时: return "超时";
        case 枚举_任务状态::就绪: return "就绪";
        case 枚举_任务状态::执行中: return "执行中";
        case 枚举_任务状态::筹办中: return "筹办中";
        case 枚举_任务状态::排队中: return "排队中";
        case 枚举_任务状态::等待中: return "等待中";
        case 枚举_任务状态::无法执行: return "无法执行";
        case 枚举_任务状态::待重筹办: return "待重筹办";
        default: return "未定义";
        }
    }

    std::string 私有_功能域文本(枚举_任务管理功能域 域) noexcept
    {
        switch (域) {
        case 枚举_任务管理功能域::筹办: return "筹办";
        case 枚举_任务管理功能域::维护: return "维护";
        case 枚举_任务管理功能域::纠偏: return "纠偏";
        case 枚举_任务管理功能域::收束: return "收束";
        case 枚举_任务管理功能域::等待: return "等待";
        default: return "未定义";
        }
    }

    std::string 私有_缺口文本(枚举_任务管理缺口类型 缺口) noexcept
    {
        switch (缺口) {
        case 枚举_任务管理缺口类型::无缺口: return "无缺口";
        case 枚举_任务管理缺口类型::治理任务缺失: return "治理任务缺失";
        case 枚举_任务管理缺口类型::宿主任务缺失: return "宿主任务缺失";
        case 枚举_任务管理缺口类型::方法挂点缺失: return "方法挂点缺失";
        case 枚举_任务管理缺口类型::任务待启动: return "任务待启动";
        case 枚举_任务管理缺口类型::任务运行中: return "任务运行中";
        case 枚举_任务管理缺口类型::任务已终结: return "任务已终结";
        case 枚举_任务管理缺口类型::上位等待约束: return "上位等待约束";
        case 枚举_任务管理缺口类型::上位收束约束: return "上位收束约束";
        case 枚举_任务管理缺口类型::上位停止约束: return "上位停止约束";
        default: return "未定义";
        }
    }

    std::string 私有_去向文本(枚举_任务管理下一步去向 去向) noexcept
    {
        switch (去向) {
        case 枚举_任务管理下一步去向::回到重筹办: return "回到重筹办";
        case 枚举_任务管理下一步去向::绑定方法: return "绑定方法";
        case 枚举_任务管理下一步去向::进入运行: return "进入运行";
        case 枚举_任务管理下一步去向::保持运行: return "保持运行";
        case 枚举_任务管理下一步去向::保持等待: return "保持等待";
        case 枚举_任务管理下一步去向::进入收束: return "进入收束";
        default: return "未定义";
        }
    }

    std::string 私有_治理状态迁移文本(枚举_任务管理治理状态迁移 迁移) noexcept
    {
        switch (迁移) {
        case 枚举_任务管理治理状态迁移::回到重筹办: return "回到重筹办";
        case 枚举_任务管理治理状态迁移::进入挂起: return "进入挂起";
        case 枚举_任务管理治理状态迁移::进入运行: return "进入运行";
        case 枚举_任务管理治理状态迁移::保持运行: return "保持运行";
        case 枚举_任务管理治理状态迁移::保持等待: return "保持等待";
        case 枚举_任务管理治理状态迁移::进入收束: return "进入收束";
        default: return "未定义";
        }
    }

    std::string 私有_总控结果文本(枚举_任务管理总控结果 结果) noexcept
    {
        switch (结果) {
        case 枚举_任务管理总控结果::治理任务已补齐: return "治理任务已补齐";
        case 枚举_任务管理总控结果::宿主待重筹办: return "宿主待重筹办";
        case 枚举_任务管理总控结果::宿主已挂起: return "宿主已挂起";
        case 枚举_任务管理总控结果::宿主已进入运行: return "宿主已进入运行";
        case 枚举_任务管理总控结果::宿主保持运行: return "宿主保持运行";
        case 枚举_任务管理总控结果::宿主保持等待: return "宿主保持等待";
        case 枚举_任务管理总控结果::宿主进入收束: return "宿主进入收束";
        default: return "未定义";
        }
    }

    std::string 私有_根层重判结果文本(枚举_任务管理根层重判结果 结果) noexcept
    {
        switch (结果) {
        case 枚举_任务管理根层重判结果::允许普通推进: return "允许普通推进";
        case 枚举_任务管理根层重判结果::待机保持: return "待机保持";
        case 枚举_任务管理根层重判结果::收束优先: return "收束优先";
        case 枚举_任务管理根层重判结果::停止退出边界: return "停止退出边界";
        default: return "未定义";
        }
    }

    std::string 私有_执行前门控结果文本(枚举_任务管理执行前门控结果 结果) noexcept
    {
        switch (结果) {
        case 枚举_任务管理执行前门控结果::允许普通执行: return "允许普通执行";
        case 枚举_任务管理执行前门控结果::仅允许等待: return "仅允许等待";
        case 枚举_任务管理执行前门控结果::仅允许收束: return "仅允许收束";
        case 枚举_任务管理执行前门控结果::禁止继续: return "禁止继续";
        default: return "未定义";
        }
    }

    std::string 私有_反馈类型文本(枚举_任务管理反馈类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_任务管理反馈类型::治理推进: return "治理推进";
        case 枚举_任务管理反馈类型::等待维持: return "等待维持";
        case 枚举_任务管理反馈类型::收束回执: return "收束回执";
        case 枚举_任务管理反馈类型::重筹办提示: return "重筹办提示";
        default: return "未定义";
        }
    }

    std::string 私有_方法来源文本(枚举_任务管理方法来源 来源) noexcept
    {
        switch (来源) {
        case 枚举_任务管理方法来源::已挂方法复用: return "已挂方法复用";
        case 枚举_任务管理方法来源::当前主方法回填: return "当前主方法回填";
        case 枚举_任务管理方法来源::最近结果回看: return "最近结果回看";
        case 枚举_任务管理方法来源::主体长期策略回填: return "主体长期策略回填";
        case 枚举_任务管理方法来源::方法树首节点候选: return "方法树首节点候选";
        case 枚举_任务管理方法来源::条件节点筛选通过: return "条件节点筛选通过";
        case 枚举_任务管理方法来源::默认本能兜底: return "默认本能兜底";
        default: return "未定义";
        }
    }

    std::string 私有_步骤位类型文本(枚举_任务管理步骤位类型 位类型) noexcept
    {
        switch (位类型) {
        case 枚举_任务管理步骤位类型::执行位: return "执行位";
        case 枚举_任务管理步骤位类型::等待位: return "等待位";
        case 枚举_任务管理步骤位类型::回接位: return "回接位";
        case 枚举_任务管理步骤位类型::补条件位: return "补条件位";
        case 枚举_任务管理步骤位类型::补结构位: return "补结构位";
        default: return "未定义";
        }
    }

    std::string 私有_步骤语义类型文本(枚举_任务管理步骤语义类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_任务管理步骤语义类型::治理步骤: return "治理步骤";
        case 枚举_任务管理步骤语义类型::业务推进步骤: return "业务推进步骤";
        case 枚举_任务管理步骤语义类型::等待维持步骤: return "等待维持步骤";
        case 枚举_任务管理步骤语义类型::回接派生步骤: return "回接派生步骤";
        case 枚举_任务管理步骤语义类型::补条件步骤: return "补条件步骤";
        case 枚举_任务管理步骤语义类型::补结构步骤: return "补结构步骤";
        default: return "未定义";
        }
    }

    std::string 私有_双面比较结果文本(枚举_任务管理双面比较结果 结果) noexcept
    {
        switch (结果) {
        case 枚举_任务管理双面比较结果::待补齐: return "待补齐";
        case 枚举_任务管理双面比较结果::已对齐: return "已对齐";
        case 枚举_任务管理双面比较结果::存在差额: return "存在差额";
        default: return "未定义";
        }
    }

    std::string 私有_后果治理文本(枚举_任务管理后果治理 结果) noexcept
    {
        switch (结果) {
        case 枚举_任务管理后果治理::无额外后果: return "无额外后果";
        case 枚举_任务管理后果治理::补齐双面: return "补齐双面";
        case 枚举_任务管理后果治理::继续派生业务步骤: return "继续派生业务步骤";
        case 枚举_任务管理后果治理::保持等待: return "保持等待";
        case 枚举_任务管理后果治理::回接后保持等待: return "回接后保持等待";
        case 枚举_任务管理后果治理::回接后继续派生: return "回接后继续派生";
        case 枚举_任务管理后果治理::进入收束: return "进入收束";
        default: return "未定义";
        }
    }

    std::uint64_t 私有_安全解析U64(const std::string& 文本) noexcept
    {
        if (文本.empty()) {
            return 0;
        }

        try {
            return static_cast<std::uint64_t>(std::stoull(文本));
        }
        catch (...) {
            return 0;
        }
    }

    constexpr std::uint64_t 私有_原语位(枚举_任务管理最小原语ID 原语ID) noexcept
    {
        switch (原语ID) {
        case 枚举_任务管理最小原语ID::确保单承载存在: return 1ULL << 0;
        case 枚举_任务管理最小原语ID::读取单特征当前值: return 1ULL << 1;
        case 枚举_任务管理最小原语ID::写入单特征当前值: return 1ULL << 2;
        case 枚举_任务管理最小原语ID::写入单挂点指针: return 1ULL << 3;
        case 枚举_任务管理最小原语ID::记录动作开始: return 1ULL << 4;
        case 枚举_任务管理最小原语ID::记录动作结束: return 1ULL << 5;
        default: return 0;
        }
    }

    枚举_本能方法ID 私有_最小原语对应本能方法ID(
        枚举_任务管理最小原语ID 原语ID) noexcept
    {
        return 任务管理最小原语模块::查询对应本能方法ID(原语ID);
    }

    constexpr std::uint64_t 私有_派生能力位_状态差分取证 = 1ULL << 0;
    constexpr std::uint64_t 私有_派生能力位_实例因果派生 = 1ULL << 1;
    constexpr std::uint64_t 私有_派生能力位_抽象因果归纳 = 1ULL << 2;

    const 结构_任务管理最小原语签名* 私有_最小原语签名表() noexcept
    {
        static const 结构_任务管理最小原语签名 s_表[] = {
            { 枚举_任务管理最小原语ID::确保单承载存在, "确保单承载存在", "存在类型特征, 归属指针特征", "存在指针特征", "自我存在/任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在/场景存在" },
            { 枚举_任务管理最小原语ID::读取单特征当前值, "读取单特征当前值", "主体指针特征, 目标特征类型", "同类型带值特征", "任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在/场景存在" },
            { 枚举_任务管理最小原语ID::写入单特征当前值, "写入单特征当前值", "主体指针特征, 目标特征类型, 候选值特征", "同类型带值特征", "任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在/场景存在" },
            { 枚举_任务管理最小原语ID::写入单挂点指针, "写入单挂点指针", "挂点主体指针特征, 挂点类型特征, 目标节点指针特征", "同类型指针特征", "任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在" },
            { 枚举_任务管理最小原语ID::记录动作开始, "记录动作开始", "动作名特征, 动作主体指针特征, 输入场景指针特征", "动作事实动态(开始相位)", "任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在/场景存在" },
            { 枚举_任务管理最小原语ID::记录动作结束, "记录动作结束", "动作名特征, 动作主体指针特征, 输出场景指针特征, 错误码特征", "动作事实动态(完成或失败相位)", "任务虚拟存在/任务管理主体虚拟存在/方法虚拟存在/场景存在" },
        };
        return s_表;
    }

    std::size_t 私有_最小原语签名数量() noexcept
    {
        return 6;
    }

    const 结构_任务管理最小原语签名* 私有_查找最小原语签名(
        枚举_任务管理最小原语ID 原语ID) noexcept
    {
        const auto* 表 = 私有_最小原语签名表();
        for (std::size_t i = 0; i < 私有_最小原语签名数量(); ++i) {
            if (表[i].原语ID == 原语ID) {
                return &表[i];
            }
        }
        return nullptr;
    }

    std::string 私有_原语位图摘要(std::uint64_t 位图) noexcept
    {
        static const 枚举_任务管理最小原语ID s_顺序[] = {
            枚举_任务管理最小原语ID::确保单承载存在,
            枚举_任务管理最小原语ID::读取单特征当前值,
            枚举_任务管理最小原语ID::写入单特征当前值,
            枚举_任务管理最小原语ID::写入单挂点指针,
            枚举_任务管理最小原语ID::记录动作开始,
            枚举_任务管理最小原语ID::记录动作结束,
        };

        std::vector<std::string> 片段{};
        for (const auto 原语ID : s_顺序) {
            if ((位图 & 私有_原语位(原语ID)) == 0) {
                continue;
            }
            if (const auto* 签名 = 私有_查找最小原语签名(原语ID)) {
                片段.emplace_back(签名->原语名);
            }
        }
        return 私有_拼接片段(片段);
    }

    std::string 私有_派生能力位图摘要(std::uint64_t 位图) noexcept
    {
        std::vector<std::string> 片段{};
        if ((位图 & 私有_派生能力位_状态差分取证) != 0) {
            片段.emplace_back("状态差分取证");
        }
        if ((位图 & 私有_派生能力位_实例因果派生) != 0) {
            片段.emplace_back("实例因果派生");
        }
        if ((位图 & 私有_派生能力位_抽象因果归纳) != 0) {
            片段.emplace_back("抽象因果归纳");
        }
        return 私有_拼接片段(片段);
    }

    std::string 私有_方法需求位文本(枚举_任务管理方法需求位 需求位) noexcept
    {
        switch (需求位) {
        case 枚举_任务管理方法需求位::补结构: return "补结构";
        case 枚举_任务管理方法需求位::选择推进: return "选择推进";
        case 枚举_任务管理方法需求位::分解补条件: return "分解补条件";
        case 枚举_任务管理方法需求位::求证判定: return "求证判定";
        case 枚举_任务管理方法需求位::门控转域: return "门控转域";
        case 枚举_任务管理方法需求位::失败策略: return "失败策略";
        case 枚举_任务管理方法需求位::收束复盘: return "收束复盘";
        default: return "未定义";
        }
    }

    std::string 私有_本能能力缺口文本(枚举_任务管理本能能力缺口类型 缺口) noexcept
    {
        switch (缺口) {
        case 枚举_任务管理本能能力缺口类型::无缺口: return "无缺口";
        case 枚举_任务管理本能能力缺口类型::缺观察原语: return "缺观察原语";
        case 枚举_任务管理本能能力缺口类型::缺写回原语: return "缺写回原语";
        case 枚举_任务管理本能能力缺口类型::缺挂点原语: return "缺挂点原语";
        case 枚举_任务管理本能能力缺口类型::缺动作边界: return "缺动作边界";
        case 枚举_任务管理本能能力缺口类型::缺状态差分取证: return "缺状态差分取证";
        case 枚举_任务管理本能能力缺口类型::缺实例因果取证: return "缺实例因果取证";
        case 枚举_任务管理本能能力缺口类型::缺抽象因果归纳: return "缺抽象因果归纳";
        case 枚举_任务管理本能能力缺口类型::需学习补齐: return "需学习补齐";
        default: return "未定义";
        }
    }

    const char* 私有_触发来源文本(枚举_任务管理触发来源 来源) noexcept
    {
        switch (来源) {
        case 枚举_任务管理触发来源::自然运行态: return "自然运行态";
        case 枚举_任务管理触发来源::自我线程控制意图: return "自我线程控制意图";
        default: return "未定义";
        }
    }

    std::string 私有_原语类型文本(枚举_原语类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_原语类型::条件满足: return "条件满足";
        case 枚举_原语类型::方向一致: return "方向一致";
        case 枚举_原语类型::结果符合: return "结果符合";
        case 枚举_原语类型::冲突判定: return "冲突判定";
        case 枚举_原语类型::因果稳定: return "因果稳定";
        case 枚举_原语类型::升阶判定: return "升阶判定";
        case 枚举_原语类型::可等待: return "可等待";
        case 枚举_原语类型::已完成: return "已完成";
        case 枚举_原语类型::需收束: return "需收束";
        case 枚举_原语类型::可回接: return "可回接";
        case 枚举_原语类型::可继续推进: return "可继续推进";
        default: return "未定义";
        }
    }

    const char* 私有_触发事件类型文本(枚举_任务管理触发事件类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_任务管理触发事件类型::普通推进: return "普通推进";
        case 枚举_任务管理触发事件类型::控制请求: return "控制请求";
        case 枚举_任务管理触发事件类型::执行回流: return "执行回流";
        case 枚举_任务管理触发事件类型::学习反馈: return "学习反馈";
        case 枚举_任务管理触发事件类型::学习回流: return "学习回流";
        case 枚举_任务管理触发事件类型::外部反馈: return "外部反馈";
        case 枚举_任务管理触发事件类型::恢复重建: return "恢复重建";
        default: return "未定义";
        }
    }

    const char* 私有_学习承接类型文本(枚举_任务管理学习承接类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_任务管理学习承接类型::定向学习: return "定向学习";
        case 枚举_任务管理学习承接类型::广泛学习: return "广泛学习";
        default: return "未定义";
        }
    }

    const char* 私有_学习反馈消费策略文本(
        枚举_任务管理学习反馈消费策略 策略) noexcept
    {
        switch (策略) {
        case 枚举_任务管理学习反馈消费策略::继续学习: return "继续学习";
        case 枚举_任务管理学习反馈消费策略::挂起观察: return "挂起观察";
        case 枚举_任务管理学习反馈消费策略::使用已完善方法: return "使用已完善方法";
        case 枚举_任务管理学习反馈消费策略::再派生学习任务: return "再派生学习任务";
        default: return "未定义";
        }
    }

    bool 私有_尝试消费治理恢复重建控制(结构_治理恢复快照* 输出) noexcept
    {
        std::scoped_lock 锁(私有_治理恢复重建控制锁);
        if (!私有_治理恢复重建控制.启用) {
            return false;
        }
        if (输出) {
            *输出 = 私有_治理恢复重建控制.快照;
        }
        私有_治理恢复重建控制 = {};
        return true;
    }

    bool 私有_尝试消费治理显式事件控制(结构_任务管理显式事件控制* 输出) noexcept
    {
        std::scoped_lock 锁(私有_治理显式事件控制锁);
        if (!私有_治理显式事件控制.启用
            || 私有_治理显式事件控制.事件类型 == 枚举_任务管理触发事件类型::未定义) {
            return false;
        }
        if (输出) {
            *输出 = 私有_治理显式事件控制;
        }
        私有_治理显式事件控制 = {};
        return true;
    }

    bool 私有_尝试消费治理控制请求(结构_任务管理控制请求* 输出) noexcept
    {
        std::scoped_lock 锁(私有_治理控制请求锁);
        if (!私有_P0_控制请求有效(私有_治理控制请求)) {
            return false;
        }
        if (输出) {
            *输出 = 私有_治理控制请求;
        }
        私有_治理控制请求 = {};
        return true;
    }

    bool 私有_本能缺口属于原语或治理补齐(
        枚举_任务管理本能能力缺口类型 缺口类型) noexcept
    {
        switch (缺口类型) {
        case 枚举_任务管理本能能力缺口类型::缺观察原语:
        case 枚举_任务管理本能能力缺口类型::缺写回原语:
        case 枚举_任务管理本能能力缺口类型::缺挂点原语:
        case 枚举_任务管理本能能力缺口类型::缺动作边界:
        case 枚举_任务管理本能能力缺口类型::缺状态差分取证:
        case 枚举_任务管理本能能力缺口类型::缺实例因果取证:
        case 枚举_任务管理本能能力缺口类型::缺抽象因果归纳:
            return true;
        default:
            return false;
        }
    }

    const char* 私有_学习通道文本(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_本能缺口属于原语或治理补齐(私有_P0_当前本能缺口(结果))
            ? "本能补齐"
            : "方法补齐";
    }

    template <typename T>
    T 私有_P0_选用非默认值(T 第一, T 第二, T 第三, T 默认值) noexcept
    {
        if (第一 != 默认值) {
            return 第一;
        }
        if (第二 != 默认值) {
            return 第二;
        }
        return 第三;
    }

    template <typename T>
    void 私有_P0_覆盖非默认值(T& 目标, T 来源, T 默认值) noexcept
    {
        if (来源 != 默认值) {
            目标 = 来源;
        }
    }

    template <typename T>
    T* 私有_P0_选用非空指针(T* 第一, T* 第二 = nullptr, T* 第三 = nullptr) noexcept
    {
        if (第一) {
            return 第一;
        }
        if (第二) {
            return 第二;
        }
        return 第三;
    }

    template <typename T>
    void 私有_P0_覆盖非空指针(T*& 目标, T* 来源) noexcept
    {
        if (来源) {
            目标 = 来源;
        }
    }

    const std::string& 私有_P0_选用非空文本(
        const std::string& 第一,
        const std::string& 第二,
        const std::string& 第三) noexcept
    {
        if (!第一.empty()) {
            return 第一;
        }
        if (!第二.empty()) {
            return 第二;
        }
        return 第三;
    }

    void 私有_P0_覆盖非空文本(
        std::string& 目标,
        const std::string& 来源) noexcept
    {
        if (!来源.empty()) {
            目标 = 来源;
        }
    }

    bool 私有_P0_控制请求有效(const 结构_任务管理控制请求& 请求) noexcept
    {
        return !请求.请求主键.empty()
            || !请求.目标线程主键.empty()
            || !请求.目标任务管理任务主键.empty()
            || !请求.目标任务主键.empty()
            || 请求.控制意图值 != 枚举_任务管理控制意图值::未定义
            || !请求.发起者主键.empty()
            || !请求.发起者类型摘要.empty()
            || 请求.发生时间 != 0
            || !请求.请求原因.empty()
            || 请求.是否需要确认
            || 请求.请求预算 != 0
            || 请求.请求优先级 != 0
            || !请求.请求摘要.empty();
    }

    bool 私有_P0_控制承接有效(const 结构_任务管理控制承接记录& 记录) noexcept
    {
        return !记录.控制请求主键.empty()
            || !记录.目标线程主键.empty()
            || !记录.目标任务管理任务主键.empty()
            || !记录.目标任务主键.empty()
            || !记录.对应执行尝试主键.empty()
            || 记录.当前控制意图 != 枚举_任务管理控制意图值::未定义
            || 记录.当前响应状态 != 枚举_任务管理控制响应状态::未定义
            || 记录.是否已下发
            || 记录.是否已响应
            || 记录.是否已执行完成
            || 记录.是否超时
            || 记录.最近回报时间 != 0
            || !记录.拒绝原因.empty()
            || !记录.摘要.empty();
    }

    void 私有_P0_覆盖控制请求(
        结构_任务管理控制请求& 目标,
        const 结构_任务管理控制请求& 来源) noexcept
    {
        if (!私有_P0_控制请求有效(来源)) {
            return;
        }

        私有_P0_覆盖非空文本(目标.请求主键, 来源.请求主键);
        私有_P0_覆盖非空文本(目标.目标线程主键, 来源.目标线程主键);
        私有_P0_覆盖非空文本(目标.目标任务管理任务主键, 来源.目标任务管理任务主键);
        私有_P0_覆盖非空文本(目标.目标任务主键, 来源.目标任务主键);
        if (来源.控制意图值 != 枚举_任务管理控制意图值::未定义) {
            目标.控制意图值 = 来源.控制意图值;
        }
        私有_P0_覆盖非空文本(目标.发起者主键, 来源.发起者主键);
        私有_P0_覆盖非空文本(目标.发起者类型摘要, 来源.发起者类型摘要);
        if (来源.发生时间 != 0) {
            目标.发生时间 = 来源.发生时间;
        }
        私有_P0_覆盖非空文本(目标.请求原因, 来源.请求原因);
        if (来源.是否需要确认) {
            目标.是否需要确认 = true;
        }
        if (来源.请求预算 != 0) {
            目标.请求预算 = 来源.请求预算;
        }
        if (来源.请求优先级 != 0) {
            目标.请求优先级 = 来源.请求优先级;
        }
        私有_P0_覆盖非空文本(目标.请求摘要, 来源.请求摘要);
    }

    void 私有_P0_覆盖控制承接(
        结构_任务管理控制承接记录& 目标,
        const 结构_任务管理控制承接记录& 来源) noexcept
    {
        if (!私有_P0_控制承接有效(来源)) {
            return;
        }

        私有_P0_覆盖非空文本(目标.控制请求主键, 来源.控制请求主键);
        私有_P0_覆盖非空文本(目标.目标线程主键, 来源.目标线程主键);
        私有_P0_覆盖非空文本(目标.目标任务管理任务主键, 来源.目标任务管理任务主键);
        私有_P0_覆盖非空文本(目标.目标任务主键, 来源.目标任务主键);
        私有_P0_覆盖非空文本(目标.对应执行尝试主键, 来源.对应执行尝试主键);
        if (来源.当前控制意图 != 枚举_任务管理控制意图值::未定义) {
            目标.当前控制意图 = 来源.当前控制意图;
        }
        if (来源.当前响应状态 != 枚举_任务管理控制响应状态::未定义) {
            目标.当前响应状态 = 来源.当前响应状态;
        }
        if (来源.是否已下发) {
            目标.是否已下发 = true;
        }
        if (来源.是否已响应) {
            目标.是否已响应 = true;
        }
        if (来源.是否已执行完成) {
            目标.是否已执行完成 = true;
        }
        if (来源.是否超时) {
            目标.是否超时 = true;
        }
        if (来源.最近回报时间 != 0) {
            目标.最近回报时间 = 来源.最近回报时间;
        }
        私有_P0_覆盖非空文本(目标.拒绝原因, 来源.拒绝原因);
        私有_P0_覆盖非空文本(目标.摘要, 来源.摘要);
    }

    std::string 私有_P0_控制请求摘要(const 结构_任务管理控制请求& 请求) noexcept
    {
        (void)请求;
        return {};
    }

    std::string 私有_P0_控制承接摘要(const 结构_任务管理控制承接记录& 记录) noexcept
    {
        (void)记录;
        return {};
    }

    bool 私有_P0_高阶排序请求有效(const 结构_高阶排序请求& 请求) noexcept
    {
        return !请求.需求轴键.empty()
            || !请求.候选主键列表.empty()
            || !请求.候选摘要.empty()
            || !请求.首候选主键.empty()
            || !请求.次候选主键.empty()
            || !请求.比较特征键.empty()
            || 请求.比较基准类型 != 枚举_原语候选比较基准类型::未定义
            || !请求.比较基准摘要.empty()
            || 请求.存在可比对
            || !请求.未决原因.empty();
    }

    bool 私有_P0_高阶排序结果有效(const 结构_高阶排序结果& 结果) noexcept
    {
        return !结果.需求轴键.empty()
            || !结果.候选主键列表.empty()
            || !结果.候选摘要.empty()
            || !结果.排序关系.empty()
            || !结果.胜出候选主键.empty()
            || 结果.是否等效
            || 结果.是否过头
            || 结果.置信度_Q10000 != 0
            || !结果.原因说明.empty();
    }

    bool 私有_P0_高阶胜出绑定有效(const 结构_高阶胜出绑定& 绑定) noexcept
    {
        return !绑定.需求轴键.empty()
            || !绑定.胜出候选主键.empty()
            || !绑定.胜出方法主键.empty()
            || 绑定.胜出任务结果节点 != nullptr
            || 绑定.胜出方法结果节点 != nullptr
            || 绑定.胜出方法节点 != nullptr
            || 绑定.来源锚点类型 != 枚举_原语候选来源锚点类型::未定义
            || 绑定.已绑定执行方法
            || !绑定.绑定摘要.empty()
            || !绑定.未绑定原因.empty();
    }

    void 私有_P0_覆盖高阶排序请求(
        结构_高阶排序请求& 目标,
        const 结构_高阶排序请求& 来源) noexcept
    {
        if (!私有_P0_高阶排序请求有效(来源)) {
            return;
        }

        私有_P0_覆盖非空文本(目标.需求轴键, 来源.需求轴键);
        if (!来源.候选主键列表.empty()) {
            目标.候选主键列表 = 来源.候选主键列表;
        }
        私有_P0_覆盖非空文本(目标.候选摘要, 来源.候选摘要);
        私有_P0_覆盖非空文本(目标.首候选主键, 来源.首候选主键);
        私有_P0_覆盖非空文本(目标.次候选主键, 来源.次候选主键);
        私有_P0_覆盖非空文本(目标.比较特征键, 来源.比较特征键);
        if (来源.比较基准类型 != 枚举_原语候选比较基准类型::未定义) {
            目标.比较基准类型 = 来源.比较基准类型;
        }
        私有_P0_覆盖非空文本(目标.比较基准摘要, 来源.比较基准摘要);
        if (来源.存在可比对) {
            目标.存在可比对 = true;
        }
        私有_P0_覆盖非空文本(目标.未决原因, 来源.未决原因);
    }

    void 私有_P0_覆盖高阶排序结果(
        结构_高阶排序结果& 目标,
        const 结构_高阶排序结果& 来源) noexcept
    {
        if (!私有_P0_高阶排序结果有效(来源)) {
            return;
        }

        私有_P0_覆盖非空文本(目标.需求轴键, 来源.需求轴键);
        if (!来源.候选主键列表.empty()) {
            目标.候选主键列表 = 来源.候选主键列表;
        }
        私有_P0_覆盖非空文本(目标.候选摘要, 来源.候选摘要);
        私有_P0_覆盖非空文本(目标.排序关系, 来源.排序关系);
        私有_P0_覆盖非空文本(目标.胜出候选主键, 来源.胜出候选主键);
        目标.是否等效 = 来源.是否等效;
        目标.是否过头 = 来源.是否过头;
        if (来源.置信度_Q10000 != 0) {
            目标.置信度_Q10000 = 来源.置信度_Q10000;
        }
        私有_P0_覆盖非空文本(目标.原因说明, 来源.原因说明);
    }

    void 私有_P0_覆盖高阶胜出绑定(
        结构_高阶胜出绑定& 目标,
        const 结构_高阶胜出绑定& 来源) noexcept
    {
        if (!私有_P0_高阶胜出绑定有效(来源)) {
            return;
        }

        私有_P0_覆盖非空文本(目标.需求轴键, 来源.需求轴键);
        私有_P0_覆盖非空文本(目标.胜出候选主键, 来源.胜出候选主键);
        私有_P0_覆盖非空文本(目标.胜出方法主键, 来源.胜出方法主键);
        私有_P0_覆盖非空指针(目标.胜出任务结果节点, static_cast<任务节点*>(来源.胜出任务结果节点));
        私有_P0_覆盖非空指针(目标.胜出方法结果节点, static_cast<方法节点*>(来源.胜出方法结果节点));
        私有_P0_覆盖非空指针(目标.胜出方法节点, static_cast<方法节点*>(来源.胜出方法节点));
        if (来源.来源锚点类型 != 枚举_原语候选来源锚点类型::未定义) {
            目标.来源锚点类型 = 来源.来源锚点类型;
        }
        if (来源.已绑定执行方法) {
            目标.已绑定执行方法 = true;
        }
        私有_P0_覆盖非空文本(目标.绑定摘要, 来源.绑定摘要);
        私有_P0_覆盖非空文本(目标.未绑定原因, 来源.未绑定原因);
    }

    std::string 私有_P0_高阶排序请求摘要(const 结构_高阶排序请求& 请求) noexcept
    {
        (void)请求;
        return {};
    }

    std::string 私有_P0_高阶排序结果摘要(const 结构_高阶排序结果& 结果) noexcept
    {
        (void)结果;
        return {};
    }

    std::string 私有_P0_高阶胜出绑定摘要(const 结构_高阶胜出绑定& 绑定) noexcept
    {
        (void)绑定;
        return {};
    }

    void 私有_P1_覆盖任务治理二次特征证据(
        结构_任务治理二次特征证据& 目标,
        const 结构_任务治理二次特征证据& 来源) noexcept
    {
        if (!来源.有证据) {
            return;
        }

        目标.有证据 = true;
        目标.任务条件满足 = 来源.任务条件满足;
        目标.目标结果方向一致 = 来源.目标结果方向一致;
        目标.目标结果已落范围 = 来源.目标结果已落范围;
        目标.当前存在显式冲突 = 来源.当前存在显式冲突;
        目标.当前动作已启动 = 来源.当前动作已启动;
        目标.当前动作已完成 = 来源.当前动作已完成;
        目标.当前动作失败退出 = 来源.当前动作失败退出;
        目标.当前处于方法位推进中 = 来源.当前处于方法位推进中;
        目标.当前处于原语边界执行中 = 来源.当前处于原语边界执行中;
        目标.当前动作已形成稳定结果 = 来源.当前动作已形成稳定结果;
        目标.当前动作长时间无推进 = 来源.当前动作长时间无推进;
        目标.当前动作需重试 = 来源.当前动作需重试;
        目标.当前动作被中断 = 来源.当前动作被中断;
        目标.影子验证稳定 = 来源.影子验证稳定;
        目标.控制请求待确认 = 来源.控制请求待确认;
        目标.控制请求已拒绝 = 来源.控制请求已拒绝;
        目标.高阶排序已完成 = 来源.高阶排序已完成;
        目标.高阶排序胜出已绑定 = 来源.高阶排序胜出已绑定;
        目标.当前需等待 = 来源.当前需等待;
        目标.当前需收束 = 来源.当前需收束;
        目标.当前需重筹办 = 来源.当前需重筹办;
        目标.当前可回接最近结果 = 来源.当前可回接最近结果;
        目标.稳定等待态复盘学习 = 来源.稳定等待态复盘学习;
        目标.业务子任务已完成 = 来源.业务子任务已完成;
        目标.业务子任务应转执行 = 来源.业务子任务应转执行;
        目标.最近结果命中当前治理 = 来源.最近结果命中当前治理;
        目标.当前步骤可复用 = 来源.当前步骤可复用;
        目标.可继续推进 = 来源.可继续推进;
        if (来源.综合置信度_Q10000 != 0) {
            目标.综合置信度_Q10000 = 来源.综合置信度_Q10000;
        }
        私有_P0_覆盖非空文本(目标.摘要, 来源.摘要);
    }

    void 私有_P0_覆盖原语判定汇总(
        结构_原语判定汇总& 目标,
        const 结构_原语判定汇总& 来源) noexcept
    {
        if (来源.条件满足) {
            目标.条件满足 = true;
        }
        if (来源.方向一致) {
            目标.方向一致 = true;
        }
        if (来源.结果符合) {
            目标.结果符合 = true;
        }
        if (来源.出现冲突) {
            目标.出现冲突 = true;
        }
        if (来源.因果稳定) {
            目标.因果稳定 = true;
        }
        if (来源.需要升阶) {
            目标.需要升阶 = true;
        }
        if (来源.可等待) {
            目标.可等待 = true;
        }
        if (来源.已完成) {
            目标.已完成 = true;
        }
        if (来源.需收束) {
            目标.需收束 = true;
        }
        if (来源.可回接) {
            目标.可回接 = true;
        }
        if (来源.可继续推进) {
            目标.可继续推进 = true;
        }
        if (来源.综合满足度_Q10000 != 0) {
            目标.综合满足度_Q10000 = 来源.综合满足度_Q10000;
        }
        if (来源.综合置信度_Q10000 != 0) {
            目标.综合置信度_Q10000 = 来源.综合置信度_Q10000;
        }
        私有_P0_覆盖非空文本(目标.升阶原因, 来源.升阶原因);
        if (!来源.明细.empty()) {
            目标.明细 = 来源.明细;
        }
    }

    struct 结构_P0_原语证据视图 {
        bool 有宿主 = false;
        bool 有治理证据 = false;
        bool 任务条件满足 = false;
        bool 方向一致 = false;
        bool 结果符合 = false;
        bool 存在显式冲突 = false;
        bool 当前动作已形成稳定结果 = false;
        bool 当前动作失败退出 = false;
        bool 当前动作长时间无推进 = false;
        bool 当前动作需重试 = false;
        bool 当前动作被中断 = false;
        bool 影子验证稳定 = false;
        bool 可等待 = false;
        bool 已完成 = false;
        bool 需收束 = false;
        bool 可回接 = false;
        bool 可继续推进 = false;
        I64 综合置信度_Q10000 = 0;
    };

    结构_P0_原语证据视图 私有_P0_构建原语证据视图(
        const 结构_任务管理请求& 请求,
        const 任务节点* 宿主任务) noexcept
    {
        结构_P0_原语证据视图 输出{};
        const auto& 证据 = 请求.当前治理二次特征证据;
        输出.有宿主 = 宿主任务 != nullptr;
        输出.有治理证据 = 证据.有证据;
        输出.任务条件满足 = 输出.有治理证据 && 证据.任务条件满足;
        输出.方向一致 = 输出.有治理证据 && 证据.目标结果方向一致;
        输出.结果符合 = 输出.有治理证据 && 证据.目标结果已落范围;
        输出.存在显式冲突 = 输出.有治理证据 && 证据.当前存在显式冲突;
        输出.当前动作已形成稳定结果 = 输出.有治理证据 && 证据.当前动作已形成稳定结果;
        输出.当前动作失败退出 = 输出.有治理证据 && 证据.当前动作失败退出;
        输出.当前动作长时间无推进 = 输出.有治理证据 && 证据.当前动作长时间无推进;
        输出.当前动作需重试 = 输出.有治理证据 && 证据.当前动作需重试;
        输出.当前动作被中断 = 输出.有治理证据 && 证据.当前动作被中断;
        输出.影子验证稳定 =
            输出.有治理证据
            && (证据.影子验证稳定 || 证据.当前动作已形成稳定结果);
        输出.可等待 =
            请求.上下文.当前需等待
            || (输出.有治理证据 && 证据.当前需等待);
        输出.已完成 =
            请求.上下文.业务子任务已完成
            || (输出.有治理证据 && (证据.业务子任务已完成 || 证据.目标结果已落范围));
        输出.需收束 =
            请求.上下文.当前需收束
            || (输出.有治理证据 && 证据.当前需收束);
        输出.可回接 =
            请求.上下文.当前可回接最近结果
            || (输出.有治理证据 && (证据.当前可回接最近结果 || 证据.最近结果命中当前治理));
        输出.可继续推进 =
            (请求.上下文.可继续推进
                || (输出.有治理证据 && 证据.可继续推进))
            && !输出.当前动作失败退出
            && !输出.当前动作长时间无推进
            && !输出.当前动作被中断
            && !输出.当前动作需重试;
        输出.综合置信度_Q10000 = 证据.综合置信度_Q10000;
        return 输出;
    }

    bool 私有_P0_执行原语(
        const 结构_任务管理请求& 请求,
        枚举_原语类型 类型,
        结构_原语判定结果& 输出) noexcept
    {
        输出 = {};
        输出.类型 = 类型;

        const auto* 宿主任务 = 私有_P0_选用非空指针(
            static_cast<任务节点*>(请求.宿主任务),
            请求.上下文.当前宿主任务);
        const auto 视图 = 私有_P0_构建原语证据视图(请求, 宿主任务);

        switch (类型) {
        case 枚举_原语类型::条件满足: {
            输出.主域 = "任务总控";
            输出.小域 = "前提";
            if (!视图.有宿主) {
                输出.原因说明 = "宿主任务缺失";
                输出.下一步建议 = "先回到重筹办";
                return true;
            }
            if (!视图.有治理证据) {
                输出.满足度_Q10000 = 0;
                输出.置信度_Q10000 = 0;
                输出.原因说明 = "当前治理二次特征证据缺失，暂不直接判前提";
                输出.下一步建议 = "先生成治理证据";
                return true;
            }
            switch (请求.上下文.当前缺口类型) {
            case 枚举_任务管理缺口类型::宿主任务缺失:
            case 枚举_任务管理缺口类型::治理任务缺失:
            case 枚举_任务管理缺口类型::方法挂点缺失:
                输出.原因说明 = "当前仍存在结构性缺口";
                输出.下一步建议 = "先补齐结构或挂点";
                return true;
            case 枚举_任务管理缺口类型::上位等待约束:
                输出.满足度_Q10000 = 3000;
                输出.置信度_Q10000 = 8500;
                输出.原因说明 = "当前受上位等待约束";
                输出.下一步建议 = "维持等待";
                return true;
            case 枚举_任务管理缺口类型::上位收束约束:
                输出.满足度_Q10000 = 3000;
                输出.置信度_Q10000 = 8500;
                输出.原因说明 = "当前受上位收束约束";
                输出.下一步建议 = "进入收束";
                return true;
            case 枚举_任务管理缺口类型::上位停止约束:
                输出.满足度_Q10000 = 1000;
                输出.置信度_Q10000 = 9000;
                输出.原因说明 = "当前受上位停止约束";
                输出.下一步建议 = "停止继续";
                return true;
            default:
                break;
            }
            输出.是否满足 = 视图.任务条件满足;
            输出.满足度_Q10000 = 10000;
            输出.置信度_Q10000 = 视图.综合置信度_Q10000;
            输出.原因说明 = 视图.任务条件满足
                ? "当前已具备进入四域选择的最小前提"
                : "当前仍缺少进入四域选择的前提";
            输出.下一步建议 = 视图.任务条件满足 ? "进入选择函数" : "先补齐结构与约束";
            return true;
        }
        case 枚举_原语类型::方向一致: {
            输出.主域 = "目标结果";
            输出.小域 = "方向";
            if (!视图.有宿主) {
                输出.原因说明 = "缺少宿主任务，无法判断方向";
                输出.下一步建议 = "先补齐宿主";
                return true;
            }
            if (!视图.有治理证据) {
                输出.满足度_Q10000 = 0;
                输出.置信度_Q10000 = 0;
                输出.原因说明 = "当前治理二次特征证据缺失，暂不直接判方向";
                输出.下一步建议 = "先生成目标结果方向证据";
                return true;
            }
            if (视图.结果符合) {
                输出.是否满足 = true;
                输出.满足度_Q10000 = 10000;
                输出.置信度_Q10000 = 视图.综合置信度_Q10000;
                输出.原因说明 = "宿主目标与结果已对齐";
                输出.下一步建议 = "可进入结果符合或收束判定";
                return true;
            }
            if (视图.方向一致) {
                输出.是否满足 = true;
                输出.满足度_Q10000 = 7000;
                输出.置信度_Q10000 = 视图.综合置信度_Q10000;
                输出.原因说明 = "当前仅能确认仍朝目标方向推进";
                输出.下一步建议 = "继续结合结果符合与升阶原语";
                return true;
            }
            输出.满足度_Q10000 = 2000;
            输出.置信度_Q10000 = 6500;
            输出.原因说明 = "当前目标-结果双面仍待补齐";
            输出.下一步建议 = "先补齐双面";
            return true;
        }
        case 枚举_原语类型::结果符合: {
            输出.主域 = "目标结果";
            输出.小域 = "范围";
            if (!视图.有治理证据) {
                输出.满足度_Q10000 = 0;
                输出.置信度_Q10000 = 0;
                输出.原因说明 = "当前治理二次特征证据缺失，暂不直接判结果范围";
                输出.下一步建议 = "先生成结果范围证据";
                return true;
            }
            if (视图.结果符合) {
                输出.是否满足 = true;
                输出.满足度_Q10000 = 10000;
                输出.置信度_Q10000 = 视图.综合置信度_Q10000;
                输出.原因说明 = "结果已落入当前目标范围";
                输出.下一步建议 = "可进入收束或维持";
                return true;
            }
            if (视图.方向一致) {
                输出.满足度_Q10000 = 4000;
                输出.置信度_Q10000 = 视图.综合置信度_Q10000;
                输出.原因说明 = "结果仍未落入目标范围";
                输出.下一步建议 = "继续推进";
                return true;
            }
            输出.满足度_Q10000 = 1000;
            输出.置信度_Q10000 = 6000;
            输出.原因说明 = "结果范围仍不可判";
            输出.下一步建议 = "先补齐双面";
            return true;
        }
        case 枚举_原语类型::冲突判定: {
            输出.主域 = "任务总控";
            输出.小域 = "冲突";
            if (!视图.有治理证据) {
                输出.满足度_Q10000 = 0;
                输出.置信度_Q10000 = 0;
                输出.原因说明 = "当前治理二次特征证据缺失，暂不直接判冲突";
                输出.下一步建议 = "先生成冲突证据";
                return true;
            }
            输出.是否冲突 = 视图.存在显式冲突;
            输出.是否满足 = 视图.存在显式冲突;
            输出.满足度_Q10000 = 视图.存在显式冲突 ? 10000 : 3000;
            输出.置信度_Q10000 = 视图.存在显式冲突 ? 9000 : 7000;
            输出.原因说明 = 视图.存在显式冲突
                ? "当前已进入纠偏/禁止继续类控制边界"
                : "当前未发现显式反向控制信号";
            输出.下一步建议 = 视图.存在显式冲突 ? "转入纠偏或重筹办" : "维持当前选择链";
            return true;
        }
        case 枚举_原语类型::因果稳定: {
            输出.主域 = "验证";
            输出.小域 = "因果";
            if (!视图.有治理证据) {
                输出.满足度_Q10000 = 0;
                输出.置信度_Q10000 = 0;
                输出.原因说明 = "当前治理二次特征证据缺失，暂不直接判因果稳定";
                输出.下一步建议 = "先生成动作与验证证据";
                return true;
            }
            输出.是否满足 = 视图.影子验证稳定;
            输出.满足度_Q10000 = 视图.影子验证稳定 ? 8000 : 2000;
            输出.置信度_Q10000 = 视图.影子验证稳定 ? 7000 : 5000;
            输出.原因说明 = 视图.影子验证稳定
                ? (视图.当前动作已形成稳定结果
                    ? "当前动作动态已形成稳定结果，可视为高可信因果候选"
                    : "当前影子验证已给出稳定/通过类信号")
                : "当前尚无稳定因果验证信号";
            输出.下一步建议 = 视图.影子验证稳定 ? "可结合结果符合进入收束" : "继续保留验证";
            return true;
        }
        case 枚举_原语类型::可等待: {
            输出.主域 = "任务总控";
            输出.小域 = "等待";
            if (!视图.有治理证据) {
                输出.满足度_Q10000 = 0;
                输出.置信度_Q10000 = 0;
                输出.原因说明 = "当前治理二次特征证据缺失，暂不直接判等待";
                输出.下一步建议 = "先生成等待证据";
                return true;
            }
            输出.是否满足 = 视图.可等待;
            输出.满足度_Q10000 = 视图.可等待 ? 9000 : 2000;
            输出.置信度_Q10000 = 视图.综合置信度_Q10000;
            输出.原因说明 = 视图.可等待
                ? "当前等待判据已满足"
                : (视图.当前动作长时间无推进
                    ? "当前动作已长时间无推进，但尚未进入正式等待位"
                    : "当前等待判据未满足");
            输出.下一步建议 = 视图.可等待 ? "维持等待" : "继续结合推进/收束判据";
            return true;
        }
        case 枚举_原语类型::已完成: {
            输出.主域 = "任务总控";
            输出.小域 = "完成";
            if (!视图.有治理证据) {
                输出.满足度_Q10000 = 0;
                输出.置信度_Q10000 = 0;
                输出.原因说明 = "当前治理二次特征证据缺失，暂不直接判完成";
                输出.下一步建议 = "先生成完成证据";
                return true;
            }
            输出.是否满足 = 视图.已完成;
            输出.满足度_Q10000 = 视图.已完成 ? 10000 : 2000;
            输出.置信度_Q10000 = 视图.综合置信度_Q10000;
            输出.原因说明 = 视图.已完成
                ? "当前已命中完成判据"
                : (视图.当前动作需重试
                    ? "当前动作失败后仍需重试，暂不判完成"
                    : "当前尚未命中完成判据");
            输出.下一步建议 = 视图.已完成 ? "进入收束" : "继续等待结果或推进";
            return true;
        }
        case 枚举_原语类型::需收束: {
            输出.主域 = "任务总控";
            输出.小域 = "收束";
            if (!视图.有治理证据) {
                输出.满足度_Q10000 = 0;
                输出.置信度_Q10000 = 0;
                输出.原因说明 = "当前治理二次特征证据缺失，暂不直接判收束";
                输出.下一步建议 = "先生成收束证据";
                return true;
            }
            输出.是否满足 = 视图.需收束;
            输出.满足度_Q10000 = 视图.需收束 ? 9500 : 1500;
            输出.置信度_Q10000 = 视图.综合置信度_Q10000;
            输出.原因说明 = 视图.需收束 ? "当前已命中收束判据" : "当前尚未命中收束判据";
            输出.下一步建议 = 视图.需收束 ? "进入收束" : "继续结合等待/推进判据";
            return true;
        }
        case 枚举_原语类型::可回接: {
            输出.主域 = "任务总控";
            输出.小域 = "回接";
            if (!视图.有治理证据) {
                输出.满足度_Q10000 = 0;
                输出.置信度_Q10000 = 0;
                输出.原因说明 = "当前治理二次特征证据缺失，暂不直接判回接";
                输出.下一步建议 = "先生成回接证据";
                return true;
            }
            输出.是否满足 = 视图.可回接;
            输出.满足度_Q10000 = 视图.可回接 ? 9000 : 2000;
            输出.置信度_Q10000 = 视图.综合置信度_Q10000;
            输出.原因说明 = 视图.可回接 ? "当前存在可回接结果证据" : "当前尚无可回接结果证据";
            输出.下一步建议 = 视图.可回接 ? "优先回接最近结果" : "继续观察或创建新结果节点";
            return true;
        }
        case 枚举_原语类型::可继续推进: {
            输出.主域 = "任务总控";
            输出.小域 = "推进";
            if (!视图.有治理证据) {
                输出.满足度_Q10000 = 0;
                输出.置信度_Q10000 = 0;
                输出.原因说明 = "当前治理二次特征证据缺失，暂不直接判推进";
                输出.下一步建议 = "先生成推进证据";
                return true;
            }
            输出.是否满足 = 视图.可继续推进;
            输出.满足度_Q10000 = 视图.可继续推进 ? 9000 : 2000;
            输出.置信度_Q10000 = 视图.综合置信度_Q10000;
            输出.原因说明 = 视图.可继续推进
                ? "当前具备继续推进条件"
                : (视图.当前动作需重试
                    ? "当前动作失败后仍需重试，暂不继续前推"
                    : (视图.当前动作被中断
                        ? "当前动作已被中断，不宜继续推进"
                        : (视图.当前动作长时间无推进
                            ? "当前动作已长时间无推进，不宜继续推进"
                            : (视图.当前动作失败退出
                                ? "当前动作已失败退出，不宜继续推进"
                                : "当前不宜继续推进"))));
            输出.下一步建议 = 视图.可继续推进 ? "继续推进" : "转入等待/重筹办/收束";
            return true;
        }
        case 枚举_原语类型::升阶判定: {
            输出.主域 = "细化";
            输出.小域 = "升阶";
            结构_P0_升阶候选分析 候选分析{};
            (void)私有_P0_分析升阶候选(请求, 候选分析);
            结构_高阶排序结果 高阶排序结果{};
            (void)私有_P0_生成高阶排序结果(请求, 高阶排序结果);
            const auto 同向候选数 = 候选分析.同轴同向候选数;
            const auto 需求轴 =
                请求.当前需求轴键.empty()
                ? std::string("未定义需求轴")
                : 请求.当前需求轴键;
            const bool 需升阶 =
                !候选分析.已完成高阶排序
                &&
                同向候选数 >= 2
                && 候选分析.存在可比对
                && !视图.存在显式冲突
                && !候选分析.存在同轴反向候选;
            const bool 已完成高阶排序 = 候选分析.已完成高阶排序;
            输出.需升阶 = 需升阶;
            输出.是否满足 = 已完成高阶排序;
            输出.满足度_Q10000 = 已完成高阶排序 ? 10000 : (需升阶 ? 4000 : 0);
            输出.置信度_Q10000 =
                高阶排序结果.置信度_Q10000 != 0
                ? 高阶排序结果.置信度_Q10000
                : (需升阶 ? 6500 : 6000);
            if (已完成高阶排序) {
                输出.原因说明 =
                    "需求轴[" + 需求轴 + "]下高阶排序已完成；"
                    + (高阶排序结果.原因说明.empty() ? 候选分析.排序原因 : 高阶排序结果.原因说明);
                if (高阶排序结果.是否等效) {
                    输出.下一步建议 = "高阶排序等效，保持当前或按次级策略选";
                }
                else if (!高阶排序结果.胜出候选主键.empty()) {
                    输出.下一步建议 = 高阶排序结果.是否过头
                        ? "高阶排序已完成，但胜出候选已过头，优先转入更稳妥候选或纠偏"
                        : ("按胜出候选继续推进=" + 高阶排序结果.胜出候选主键);
                }
                else {
                    输出.下一步建议 = "高阶排序已完成，等待选择函数按排序关系路由";
                }
            }
            else if (需升阶) {
                输出.原因说明 =
                    "需求轴[" + 需求轴 + "]下已形成同轴、同向且可比的候选组，需继续排序谁更接近需求；"
                    + (高阶排序结果.原因说明.empty() ? 候选分析.排序原因 : 高阶排序结果.原因说明);
                输出.下一步建议 = "触发高阶排序请求";
            }
            else if (视图.存在显式冲突 || 候选分析.存在同轴反向候选) {
                输出.原因说明 =
                    "需求轴[" + 需求轴 + "]下存在反向候选，当前交回二次层处理冲突，不在此处硬判高阶排序";
                输出.下一步建议 = "交回二次层处理冲突";
            }
            else if (同向候选数 < 2) {
                输出.原因说明 =
                    "需求轴[" + 需求轴 + "]下当前同向候选不足，暂不触发高阶排序；"
                    + (候选分析.未决原因.empty() ? std::string("需继续积累同轴候选") : 候选分析.未决原因);
                输出.下一步建议 = "继续积累同轴候选";
            }
            else {
                输出.原因说明 =
                    "需求轴[" + 需求轴 + "]下已出现多个同向候选，但尚缺可比基准；"
                    + (候选分析.未决原因.empty() ? std::string("当前只记录高阶排序请求，不直接生成三次特征实体") : 候选分析.未决原因);
                输出.下一步建议 = "补齐比较基准后再评估高阶排序";
            }
            return true;
        }
        default:
            return false;
        }
    }

    bool 私有_P0_汇总原语(
        const std::vector<结构_原语判定结果>& 结果集,
        结构_原语判定汇总& 输出) noexcept
    {
        输出 = {};
        if (结果集.empty()) {
            return true;
        }

        std::int64_t 满足度总和 = 0;
        std::int64_t 置信度总和 = 0;
        std::size_t 计数 = 0;
        for (const auto& 结果 : 结果集) {
            输出.明细.emplace_back(结果);
            满足度总和 += 结果.满足度_Q10000;
            置信度总和 += 结果.置信度_Q10000;
            ++计数;

            switch (结果.类型) {
            case 枚举_原语类型::条件满足:
                输出.条件满足 = 结果.是否满足;
                break;
            case 枚举_原语类型::方向一致:
                输出.方向一致 = 结果.是否满足;
                break;
            case 枚举_原语类型::结果符合:
                输出.结果符合 = 结果.是否满足;
                break;
            case 枚举_原语类型::冲突判定:
                输出.出现冲突 = 结果.是否冲突 || 结果.是否满足;
                break;
            case 枚举_原语类型::因果稳定:
                输出.因果稳定 = 结果.是否满足;
                break;
            case 枚举_原语类型::升阶判定:
                输出.需要升阶 = 输出.需要升阶 || 结果.需升阶;
                if (输出.升阶原因.empty() && !结果.原因说明.empty()) {
                    输出.升阶原因 = 结果.原因说明;
                }
                break;
            case 枚举_原语类型::可等待:
                输出.可等待 = 结果.是否满足;
                break;
            case 枚举_原语类型::已完成:
                输出.已完成 = 结果.是否满足;
                break;
            case 枚举_原语类型::需收束:
                输出.需收束 = 结果.是否满足;
                break;
            case 枚举_原语类型::可回接:
                输出.可回接 = 结果.是否满足;
                break;
            case 枚举_原语类型::可继续推进:
                输出.可继续推进 = 结果.是否满足;
                break;
            default:
                break;
            }
        }

        if (计数 != 0) {
            输出.综合满足度_Q10000 = 满足度总和 / static_cast<std::int64_t>(计数);
            输出.综合置信度_Q10000 = 置信度总和 / static_cast<std::int64_t>(计数);
        }
        return true;
    }

    bool 私有_P0_生成原语判定汇总(
        const 结构_任务管理请求& 请求,
        结构_原语判定汇总& 输出) noexcept
    {
        static constexpr 枚举_原语类型 s_顺序[] = {
            枚举_原语类型::条件满足,
            枚举_原语类型::方向一致,
            枚举_原语类型::结果符合,
            枚举_原语类型::冲突判定,
            枚举_原语类型::因果稳定,
            枚举_原语类型::升阶判定,
            枚举_原语类型::可等待,
            枚举_原语类型::已完成,
            枚举_原语类型::需收束,
            枚举_原语类型::可回接,
            枚举_原语类型::可继续推进,
        };

        std::vector<结构_原语判定结果> 结果集{};
        结果集.reserve(sizeof(s_顺序) / sizeof(s_顺序[0]));
        for (const auto 类型 : s_顺序) {
            结构_原语判定结果 当前{};
            if (!私有_P0_执行原语(请求, 类型, 当前)) {
                return false;
            }
            结果集.emplace_back(std::move(当前));
        }
        return 私有_P0_汇总原语(结果集, 输出);
    }

    bool 私有_P0_选择下一步(
        const 结构_任务管理请求& 请求,
        const 结构_原语判定汇总& 汇总,
        结构_任务管理单步决策& 输出) noexcept
    {
        结构_高阶排序请求 高阶排序请求 = 请求.当前高阶排序请求;
        if (!私有_P0_高阶排序请求有效(高阶排序请求)) {
            (void)私有_P0_生成高阶排序请求(请求, 高阶排序请求);
        }
        结构_高阶排序结果 高阶排序结果{};
        (void)私有_P0_生成高阶排序结果(请求, 高阶排序结果);
        结构_高阶胜出绑定 高阶胜出绑定{};
        (void)私有_P0_解析高阶胜出绑定(请求, 高阶排序结果, 高阶胜出绑定);

        输出 = {};
        输出.当前功能域 = 请求.上下文.最近功能域;
        输出.当前缺口类型 = 请求.上下文.当前缺口类型;
        输出.当前下一步去向 = 请求.上下文.当前下一步去向;
        输出.当前触发来源 = 请求.触发来源;
        输出.当前原语判定汇总 = 汇总;
        输出.当前高阶排序请求 = 高阶排序请求;
        输出.当前高阶排序结果 = 高阶排序结果;
        输出.当前高阶胜出绑定 = 高阶胜出绑定;
        输出.当前控制承接 = 请求.当前控制承接;

        if (!汇总.条件满足) {
            if (请求.上下文.当前缺口类型 == 枚举_任务管理缺口类型::上位等待约束
                || 请求.上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::仅允许等待) {
                输出.当前功能域 = 枚举_任务管理功能域::等待;
                输出.当前下一步去向 = 枚举_任务管理下一步去向::保持等待;
                输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::保持等待;
                输出.当前后果治理 = 枚举_任务管理后果治理::保持等待;
                return true;
            }
            if (请求.上下文.当前缺口类型 == 枚举_任务管理缺口类型::上位收束约束
                || 请求.上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::仅允许收束
                || 请求.上下文.最近根层重判结果 == 枚举_任务管理根层重判结果::收束优先) {
                输出.当前功能域 = 枚举_任务管理功能域::收束;
                输出.当前下一步去向 = 枚举_任务管理下一步去向::进入收束;
                输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::进入收束;
                输出.当前后果治理 = 枚举_任务管理后果治理::进入收束;
                return true;
            }

            输出.当前下一步去向 = 枚举_任务管理下一步去向::回到重筹办;
            输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::回到重筹办;
            return true;
        }

        if (汇总.出现冲突) {
            输出.当前功能域 = 枚举_任务管理功能域::纠偏;
            输出.当前下一步去向 = 枚举_任务管理下一步去向::回到重筹办;
            输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::回到重筹办;
            输出.当前后果治理 = 枚举_任务管理后果治理::补齐双面;
            return true;
        }

        if (汇总.需收束 || 汇总.已完成) {
            输出.当前功能域 = 枚举_任务管理功能域::收束;
            输出.当前下一步去向 = 枚举_任务管理下一步去向::进入收束;
            输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::进入收束;
            输出.当前后果治理 = 枚举_任务管理后果治理::进入收束;
            return true;
        }

        if (汇总.可等待 && !汇总.可继续推进) {
            输出.当前功能域 = 枚举_任务管理功能域::等待;
            输出.当前下一步去向 = 枚举_任务管理下一步去向::保持等待;
            输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::保持等待;
            输出.当前后果治理 = 汇总.可回接
                ? 枚举_任务管理后果治理::回接后保持等待
                : 枚举_任务管理后果治理::保持等待;
            return true;
        }

        if (汇总.结果符合 && 汇总.因果稳定) {
            输出.当前功能域 = 枚举_任务管理功能域::收束;
            输出.当前下一步去向 = 枚举_任务管理下一步去向::进入收束;
            输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::进入收束;
            输出.当前后果治理 = 枚举_任务管理后果治理::进入收束;
            return true;
        }

        const bool 已完成高阶排序 =
            私有_P0_高阶排序结果有效(高阶排序结果)
            && (
                高阶排序结果.是否等效
                || !高阶排序结果.胜出候选主键.empty()
            );
        if (已完成高阶排序) {
            if (高阶排序结果.是否等效) {
                if (输出.当前功能域 == 枚举_任务管理功能域::未定义) {
                    输出.当前功能域 = 枚举_任务管理功能域::维护;
                }
                输出.当前下一步去向 = 汇总.方向一致
                    ? 枚举_任务管理下一步去向::保持运行
                    : 枚举_任务管理下一步去向::保持等待;
                输出.当前治理状态迁移 = 汇总.方向一致
                    ? 枚举_任务管理治理状态迁移::保持运行
                    : 枚举_任务管理治理状态迁移::保持等待;
                输出.当前后果治理 = 汇总.方向一致
                    ? 枚举_任务管理后果治理::无额外后果
                    : 枚举_任务管理后果治理::保持等待;
                return true;
            }
            if (高阶排序结果.是否过头) {
                输出.当前功能域 = 枚举_任务管理功能域::纠偏;
                输出.当前下一步去向 = 枚举_任务管理下一步去向::回到重筹办;
                输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::回到重筹办;
                输出.当前后果治理 = 枚举_任务管理后果治理::补齐双面;
                return true;
            }
            if (!高阶胜出绑定.已绑定执行方法) {
                if (输出.当前功能域 == 枚举_任务管理功能域::未定义) {
                    输出.当前功能域 = 枚举_任务管理功能域::等待;
                }
                输出.当前下一步去向 = 枚举_任务管理下一步去向::保持等待;
                输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::保持等待;
                输出.当前后果治理 = 枚举_任务管理后果治理::保持等待;
                return true;
            }
            输出.当前功能域 = 枚举_任务管理功能域::维护;
            输出.当前下一步去向 = 枚举_任务管理下一步去向::保持运行;
            输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::保持运行;
            输出.当前后果治理 = 枚举_任务管理后果治理::无额外后果;
            return true;
        }

        if (汇总.需要升阶) {
            if (输出.当前功能域 == 枚举_任务管理功能域::未定义) {
                输出.当前功能域 = 枚举_任务管理功能域::维护;
            }
            输出.当前下一步去向 = 枚举_任务管理下一步去向::保持等待;
            输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::保持等待;
            输出.当前后果治理 = 枚举_任务管理后果治理::保持等待;
            return true;
        }

        if (私有_P0_高阶排序请求有效(高阶排序请求)
            && !高阶排序请求.存在可比对
            && !高阶排序请求.未决原因.empty()) {
            if (输出.当前功能域 == 枚举_任务管理功能域::未定义) {
                输出.当前功能域 = 枚举_任务管理功能域::等待;
            }
            输出.当前下一步去向 = 枚举_任务管理下一步去向::保持等待;
            输出.当前治理状态迁移 = 枚举_任务管理治理状态迁移::保持等待;
            输出.当前后果治理 = 枚举_任务管理后果治理::保持等待;
            return true;
        }

        if (汇总.方向一致) {
            if (输出.当前功能域 == 枚举_任务管理功能域::未定义) {
                输出.当前功能域 = 枚举_任务管理功能域::维护;
            }
            if (输出.当前下一步去向 == 枚举_任务管理下一步去向::未定义
                || 输出.当前下一步去向 == 枚举_任务管理下一步去向::回到重筹办) {
                输出.当前下一步去向 = 汇总.可继续推进
                    ? 枚举_任务管理下一步去向::保持运行
                    : 枚举_任务管理下一步去向::保持等待;
            }
            if (输出.当前治理状态迁移 == 枚举_任务管理治理状态迁移::未定义) {
                输出.当前治理状态迁移 = 汇总.可继续推进
                    ? 枚举_任务管理治理状态迁移::保持运行
                    : 枚举_任务管理治理状态迁移::保持等待;
            }
            if (输出.当前后果治理 == 枚举_任务管理后果治理::未定义) {
                输出.当前后果治理 = 汇总.可继续推进
                    ? 枚举_任务管理后果治理::无额外后果
                    : 枚举_任务管理后果治理::保持等待;
            }
            return true;
        }

        return true;
    }

    枚举_任务管理功能域 私有_P0_当前功能域(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前功能域,
            结果.当前单步决策.当前功能域,
            私有_P0_选用非默认值(
                结果.当前功能域,
                结果.上下文.最近功能域,
                枚举_任务管理功能域::未定义,
                枚举_任务管理功能域::未定义),
            枚举_任务管理功能域::未定义);
    }

    枚举_任务管理缺口类型 私有_P0_当前缺口类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前缺口类型,
            结果.当前单步决策.当前缺口类型,
            私有_P0_选用非默认值(
                结果.当前缺口类型,
                结果.上下文.当前缺口类型,
                枚举_任务管理缺口类型::未定义,
                枚举_任务管理缺口类型::未定义),
            枚举_任务管理缺口类型::未定义);
    }

    枚举_任务管理下一步去向 私有_P0_当前下一步去向(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前下一步去向,
            结果.当前单步决策.当前下一步去向,
            私有_P0_选用非默认值(
                结果.当前下一步去向,
                结果.上下文.当前下一步去向,
                枚举_任务管理下一步去向::未定义,
                枚举_任务管理下一步去向::未定义),
            枚举_任务管理下一步去向::未定义);
    }

    枚举_任务管理治理状态迁移 私有_P0_当前治理状态迁移(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前治理状态迁移,
            结果.当前单步决策.当前治理状态迁移,
            结果.当前治理状态迁移,
            枚举_任务管理治理状态迁移::未定义);
    }

    枚举_任务管理方法来源 私有_P0_当前方法来源(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前单步决策.当前方法来源,
            结果.当前方法来源,
            枚举_任务管理方法来源::未定义,
            枚举_任务管理方法来源::未定义);
    }

    枚举_任务管理方法需求位 私有_P0_当前方法需求位(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前方法需求位,
            结果.当前单步决策.当前方法需求位,
            私有_P0_选用非默认值(
                结果.当前需求位特征.当前需求位,
                结果.当前主方法需求位,
                枚举_任务管理方法需求位::未定义,
                枚举_任务管理方法需求位::未定义),
            枚举_任务管理方法需求位::未定义);
    }

    枚举_任务管理本能能力缺口类型 私有_P0_当前本能缺口(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前本能能力缺口类型,
            结果.当前单步决策.当前本能能力缺口类型,
            私有_P0_选用非默认值(
                结果.当前本能能力缺口.当前缺口类型,
                结果.当前本能能力缺口类型,
                枚举_任务管理本能能力缺口类型::未定义,
                枚举_任务管理本能能力缺口类型::未定义),
            枚举_任务管理本能能力缺口类型::未定义);
    }

    枚举_任务管理步骤位类型 私有_P0_当前步骤位类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前单步决策.当前步骤位类型,
            私有_P0_选用非默认值(
                结果.当前需求位特征.当前步骤位类型,
                结果.当前步骤位类型,
                枚举_任务管理步骤位类型::未定义,
                枚举_任务管理步骤位类型::未定义),
            枚举_任务管理步骤位类型::未定义,
            枚举_任务管理步骤位类型::未定义);
    }

    枚举_任务管理步骤语义类型 私有_P0_当前步骤语义类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前单步决策.当前步骤语义类型,
            私有_P0_选用非默认值(
                结果.当前需求位特征.当前步骤语义类型,
                结果.当前步骤语义类型,
                枚举_任务管理步骤语义类型::未定义,
                枚举_任务管理步骤语义类型::未定义),
            枚举_任务管理步骤语义类型::未定义,
            枚举_任务管理步骤语义类型::未定义);
    }

    枚举_任务管理学习承接类型 私有_P0_当前学习承接类型(const 结构_任务管理结果& 结果) noexcept
    {
        if (结果.当前一步结果.当前学习承接类型 != 枚举_任务管理学习承接类型::未定义) {
            return 结果.当前一步结果.当前学习承接类型;
        }
        if (结果.当前学习承接决策.学习类型 != 枚举_任务管理学习承接类型::未定义) {
            return 结果.当前学习承接决策.学习类型;
        }
        return 结果.当前需求位特征.当前学习承接类型;
    }

    bool 私有_P0_已触发学习承接(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前一步结果.已触发学习承接
            || 结果.当前学习承接决策.是否触发学习
            || 结果.当前需求位特征.已触发学习承接;
    }

    const std::string& 私有_P0_当前学习承接摘要(const 结构_任务管理结果& 结果) noexcept
    {
        (void)结果;
        static const std::string 空摘要{};
        return 空摘要;
    }

    bool 私有_P0_当前步骤可复用(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前需求位特征.当前步骤可复用
            || 结果.上下文.当前步骤可复用
            || (结果.当前治理二次特征证据.有证据
                && 结果.当前治理二次特征证据.当前步骤可复用);
    }

    bool 私有_P0_最近结果命中当前治理(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前需求位特征.最近结果命中当前治理
            || 结果.上下文.最近结果命中当前治理
            || (结果.当前治理二次特征证据.有证据
                && 结果.当前治理二次特征证据.最近结果命中当前治理);
    }

    bool 私有_P0_当前动作长时间无推进(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前需求位特征.当前动作长时间无推进
            || 结果.上下文.当前动作长时间无推进
            || (结果.当前治理二次特征证据.有证据
                && 结果.当前治理二次特征证据.当前动作长时间无推进);
    }

    bool 私有_P0_当前动作需重试(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前需求位特征.当前动作需重试
            || 结果.上下文.当前动作需重试
            || (结果.当前治理二次特征证据.有证据
                && 结果.当前治理二次特征证据.当前动作需重试);
    }

    bool 私有_P0_当前需等待(const 结构_任务管理结果& 结果) noexcept
    {
        if (结果.当前原语判定汇总.可等待) {
            return true;
        }
        if (结果.当前需求位特征.当前需等待 || 结果.上下文.当前需等待) {
            return true;
        }
        if (结果.当前治理二次特征证据.有证据
            && 结果.当前治理二次特征证据.当前需等待) {
            return true;
        }
        return 私有_P0_当前下一步去向(结果) == 枚举_任务管理下一步去向::保持等待
            || 私有_P0_当前步骤语义类型(结果) == 枚举_任务管理步骤语义类型::等待维持步骤;
    }

    bool 私有_P0_当前需收束(const 结构_任务管理结果& 结果) noexcept
    {
        if (结果.当前需求位特征.当前需收束 || 结果.上下文.当前需收束) {
            return true;
        }
        if (结果.当前治理二次特征证据.有证据
            && 结果.当前治理二次特征证据.当前需收束) {
            return true;
        }
        return 私有_P0_当前下一步去向(结果) == 枚举_任务管理下一步去向::进入收束;
    }

    bool 私有_P0_可继续推进(const 结构_任务管理结果& 结果) noexcept
    {
        if (结果.当前原语判定汇总.可继续推进) {
            return true;
        }
        if (结果.当前需求位特征.可继续推进 || 结果.上下文.可继续推进) {
            return true;
        }
        return 结果.当前治理二次特征证据.有证据
            && 结果.当前治理二次特征证据.可继续推进;
    }

    bool 私有_P0_当前可回接最近结果(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前需求位特征.当前可回接最近结果
            || 结果.上下文.当前可回接最近结果
            || (结果.当前治理二次特征证据.有证据
                && 结果.当前治理二次特征证据.当前可回接最近结果);
    }

    bool 私有_P0_稳定等待态复盘学习(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前需求位特征.稳定等待态复盘学习
            || 结果.上下文.稳定等待态复盘学习
            || (结果.当前治理二次特征证据.有证据
                && 结果.当前治理二次特征证据.稳定等待态复盘学习);
    }

    bool 私有_P0_业务子任务已完成(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前需求位特征.业务子任务已完成
            || 结果.上下文.业务子任务已完成
            || (结果.当前治理二次特征证据.有证据
                && 结果.当前治理二次特征证据.业务子任务已完成);
    }

    bool 私有_P0_业务子任务应转执行(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前需求位特征.业务子任务应转执行
            || 结果.上下文.业务子任务应转执行
            || (结果.当前治理二次特征证据.有证据
                && 结果.当前治理二次特征证据.业务子任务应转执行);
    }

    void 私有_P0_刷新步骤与学习结构字段(结构_任务管理结果& 结果) noexcept
    {
        const auto 当前步骤位类型 = 私有_P0_当前步骤位类型(结果);
        if (当前步骤位类型 != 枚举_任务管理步骤位类型::未定义) {
            结果.当前需求位特征.当前步骤位类型 = 当前步骤位类型;
            结果.上下文.当前步骤位类型 = 当前步骤位类型;
        }

        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        if (当前步骤语义类型 != 枚举_任务管理步骤语义类型::未定义) {
            结果.当前需求位特征.当前步骤语义类型 = 当前步骤语义类型;
            结果.上下文.当前步骤语义类型 = 当前步骤语义类型;
        }

        const bool 已触发学习承接 = 私有_P0_已触发学习承接(结果);
        if (已触发学习承接) {
            结果.当前需求位特征.已触发学习承接 = true;
            结果.上下文.已触发学习承接 = true;
        }

        const auto 当前学习承接类型 = 私有_P0_当前学习承接类型(结果);
        if (当前学习承接类型 != 枚举_任务管理学习承接类型::未定义) {
            结果.当前需求位特征.当前学习承接类型 = 当前学习承接类型;
            结果.上下文.当前学习承接类型 = 当前学习承接类型;
        }

        结果.当前需求位特征.学习承接摘要.clear();
        结果.上下文.当前学习承接摘要.clear();

        const bool 当前需等待 = 私有_P0_当前需等待(结果);
        结果.当前需求位特征.当前需等待 = 当前需等待;
        结果.上下文.当前需等待 = 当前需等待;

        const bool 当前需收束 = 私有_P0_当前需收束(结果);
        结果.当前需求位特征.当前需收束 = 当前需收束;
        结果.上下文.当前需收束 = 当前需收束;

        const bool 可继续推进 = 私有_P0_可继续推进(结果);
        结果.当前需求位特征.可继续推进 = 可继续推进;
        结果.上下文.可继续推进 = 可继续推进;

        const bool 当前可回接最近结果 = 私有_P0_当前可回接最近结果(结果);
        结果.当前需求位特征.当前可回接最近结果 = 当前可回接最近结果;
        结果.上下文.当前可回接最近结果 = 当前可回接最近结果;

        const bool 稳定等待态复盘学习 = 私有_P0_稳定等待态复盘学习(结果);
        结果.当前需求位特征.稳定等待态复盘学习 = 稳定等待态复盘学习;
        结果.上下文.稳定等待态复盘学习 = 稳定等待态复盘学习;

        const bool 业务子任务已完成 = 私有_P0_业务子任务已完成(结果);
        结果.当前需求位特征.业务子任务已完成 = 业务子任务已完成;
        结果.上下文.业务子任务已完成 = 业务子任务已完成;

        const bool 业务子任务应转执行 = 私有_P0_业务子任务应转执行(结果);
        结果.当前需求位特征.业务子任务应转执行 = 业务子任务应转执行;
        结果.上下文.业务子任务应转执行 = 业务子任务应转执行;

        if (结果.当前治理二次特征证据.有证据) {
            结果.当前需求位特征.当前步骤可复用 =
                结果.当前治理二次特征证据.当前步骤可复用;
            结果.当前需求位特征.最近结果命中当前治理 =
                结果.当前治理二次特征证据.最近结果命中当前治理;
            结果.当前需求位特征.当前动作长时间无推进 =
                结果.当前治理二次特征证据.当前动作长时间无推进;
            结果.当前需求位特征.当前动作需重试 =
                结果.当前治理二次特征证据.当前动作需重试;

            结果.上下文.当前步骤可复用 =
                结果.当前治理二次特征证据.当前步骤可复用;
            结果.上下文.最近结果命中当前治理 =
                结果.当前治理二次特征证据.最近结果命中当前治理;
            结果.上下文.当前动作长时间无推进 =
                结果.当前治理二次特征证据.当前动作长时间无推进;
            结果.上下文.当前动作需重试 =
                结果.当前治理二次特征证据.当前动作需重试;
            结果.当前需求位特征.当前需等待 =
                结果.当前治理二次特征证据.当前需等待;
            结果.当前需求位特征.当前需收束 =
                结果.当前治理二次特征证据.当前需收束;
            结果.当前需求位特征.可继续推进 =
                结果.当前治理二次特征证据.可继续推进;
            结果.当前需求位特征.当前可回接最近结果 =
                结果.当前治理二次特征证据.当前可回接最近结果;
            结果.当前需求位特征.稳定等待态复盘学习 =
                结果.当前治理二次特征证据.稳定等待态复盘学习;
            结果.当前需求位特征.业务子任务已完成 =
                结果.当前治理二次特征证据.业务子任务已完成;
            结果.当前需求位特征.业务子任务应转执行 =
                结果.当前治理二次特征证据.业务子任务应转执行;

            结果.上下文.当前需等待 =
                结果.当前治理二次特征证据.当前需等待;
            结果.上下文.当前需收束 =
                结果.当前治理二次特征证据.当前需收束;
            结果.上下文.可继续推进 =
                结果.当前治理二次特征证据.可继续推进;
            结果.上下文.当前可回接最近结果 =
                结果.当前治理二次特征证据.当前可回接最近结果;
            结果.上下文.稳定等待态复盘学习 =
                结果.当前治理二次特征证据.稳定等待态复盘学习;
            结果.上下文.业务子任务已完成 =
                结果.当前治理二次特征证据.业务子任务已完成;
            结果.上下文.业务子任务应转执行 =
                结果.当前治理二次特征证据.业务子任务应转执行;
        }
    }

    枚举_任务管理后果治理 私有_P0_当前后果治理(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前单步决策.当前后果治理,
            结果.当前后果治理,
            枚举_任务管理后果治理::未定义,
            枚举_任务管理后果治理::未定义);
    }

    任务节点* 私有_P0_当前宿主任务(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空指针(
            结果.上下文.当前宿主任务,
            static_cast<任务节点*>(结果.当前一步结果.宿主任务),
            static_cast<任务节点*>(结果.当前请求.宿主任务));
    }

    方法节点* 私有_P0_当前方法(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空指针(
            结果.上下文.当前方法,
            static_cast<方法节点*>(结果.当前一步结果.当前方法节点));
    }

    任务节点* 私有_P0_当前步骤节点(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空指针(
            结果.当前步骤节点,
            static_cast<任务节点*>(结果.当前一步结果.当前步骤节点),
            static_cast<任务节点*>(结果.当前写回结果.当前步骤节点));
    }

    任务节点* 私有_P0_最新结果节点(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空指针(
            结果.最新结果节点,
            static_cast<任务节点*>(结果.当前一步结果.最新结果节点),
            static_cast<任务节点*>(结果.当前写回结果.最新结果节点));
    }

    任务节点* 私有_P0_当前子任务节点(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空指针(
            结果.当前子任务节点,
            static_cast<任务节点*>(结果.当前写回结果.当前学习子任务),
            私有_P0_选用非空指针(
                static_cast<任务节点*>(结果.当前写回结果.当前补条件子任务),
                static_cast<任务节点*>(结果.当前写回结果.当前业务子任务),
                static_cast<任务节点*>(nullptr)));
    }

    枚举_任务管理总控结果 私有_P0_最近总控结果(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.最近总控结果,
            结果.最近总控结果,
            枚举_任务管理总控结果::未定义,
            枚举_任务管理总控结果::未定义);
    }

    枚举_任务管理反馈类型 私有_P0_最近反馈类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.最近反馈类型,
            结果.最近反馈类型,
            枚举_任务管理反馈类型::未定义,
            枚举_任务管理反馈类型::未定义);
    }

    const std::string& 私有_P0_最近反馈摘要(const 结构_任务管理结果& 结果) noexcept
    {
        (void)结果;
        static const std::string 空摘要{};
        return 空摘要;
    }

    const std::string& 私有_P0_双面摘要(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非空文本(
            结果.目标结果差额摘要,
            结果.当前写回结果.双面摘要,
            结果.目标结果差额摘要);
    }

    枚举_任务管理功能域 私有_P0_阶段当前功能域(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前功能域,
            结果.当前单步决策.当前功能域,
            结果.当前功能域,
            枚举_任务管理功能域::未定义);
    }

    枚举_任务管理缺口类型 私有_P0_阶段当前缺口类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前缺口类型,
            结果.当前单步决策.当前缺口类型,
            结果.当前缺口类型,
            枚举_任务管理缺口类型::未定义);
    }

    枚举_任务管理下一步去向 私有_P0_阶段当前下一步去向(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.当前下一步去向,
            结果.当前单步决策.当前下一步去向,
            结果.当前下一步去向,
            枚举_任务管理下一步去向::未定义);
    }

    枚举_任务管理总控结果 私有_P0_阶段最近总控结果(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.最近总控结果,
            结果.最近总控结果,
            枚举_任务管理总控结果::未定义,
            枚举_任务管理总控结果::未定义);
    }

    枚举_任务管理反馈类型 私有_P0_阶段最近反馈类型(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_选用非默认值(
            结果.当前一步结果.最近反馈类型,
            结果.最近反馈类型,
            枚举_任务管理反馈类型::未定义,
            枚举_任务管理反馈类型::未定义);
    }

    const std::string& 私有_P0_阶段最近反馈摘要(const 结构_任务管理结果& 结果) noexcept
    {
        (void)结果;
        static const std::string 空摘要{};
        return 空摘要;
    }

    void 私有_P0_覆盖决策字段(
        结构_任务管理单步决策& 目标,
        const 结构_任务管理单步决策& 来源) noexcept
    {
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前治理状态迁移, 来源.当前治理状态迁移, 枚举_任务管理治理状态迁移::未定义);
        私有_P0_覆盖非默认值(目标.当前方法来源, 来源.当前方法来源, 枚举_任务管理方法来源::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤位类型, 来源.当前步骤位类型, 枚举_任务管理步骤位类型::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤语义类型, 来源.当前步骤语义类型, 枚举_任务管理步骤语义类型::未定义);
        私有_P0_覆盖非默认值(目标.当前后果治理, 来源.当前后果治理, 枚举_任务管理后果治理::未定义);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.影子验证状态, 来源.影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        私有_P1_覆盖任务治理二次特征证据(目标.当前治理二次特征证据, 来源.当前治理二次特征证据);
        私有_P0_覆盖原语判定汇总(目标.当前原语判定汇总, 来源.当前原语判定汇总);
        私有_P0_覆盖高阶排序请求(目标.当前高阶排序请求, 来源.当前高阶排序请求);
        私有_P0_覆盖高阶排序结果(目标.当前高阶排序结果, 来源.当前高阶排序结果);
        私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前高阶胜出绑定);
        私有_P0_覆盖控制承接(目标.当前控制承接, 来源.当前控制承接);
        私有_P0_覆盖非空文本(目标.决策摘要, 来源.决策摘要);
    }

    void 私有_P0_覆盖兼容决策字段(
        结构_任务管理单步决策& 目标,
        const 结构_任务管理结果& 来源) noexcept
    {
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前治理状态迁移, 来源.当前治理状态迁移, 枚举_任务管理治理状态迁移::未定义);
        私有_P0_覆盖非默认值(目标.当前方法来源, 来源.当前方法来源, 枚举_任务管理方法来源::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前主方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤位类型, 来源.当前步骤位类型, 枚举_任务管理步骤位类型::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤语义类型, 来源.当前步骤语义类型, 枚举_任务管理步骤语义类型::未定义);
        私有_P0_覆盖非默认值(目标.当前后果治理, 来源.当前后果治理, 枚举_任务管理后果治理::未定义);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.影子验证状态, 来源.当前影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        私有_P1_覆盖任务治理二次特征证据(目标.当前治理二次特征证据, 来源.当前治理二次特征证据);
        私有_P0_覆盖原语判定汇总(目标.当前原语判定汇总, 来源.当前原语判定汇总);
        私有_P0_覆盖高阶排序请求(目标.当前高阶排序请求, 来源.当前高阶排序请求);
        if (!私有_P0_高阶排序请求有效(目标.当前高阶排序请求)) {
            私有_P0_覆盖高阶排序请求(目标.当前高阶排序请求, 来源.当前请求.当前高阶排序请求);
        }
        私有_P0_覆盖高阶排序结果(目标.当前高阶排序结果, 来源.当前高阶排序结果);
        私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前高阶胜出绑定);
        if (!私有_P0_高阶胜出绑定有效(目标.当前高阶胜出绑定)) {
            私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前一步结果.当前高阶胜出绑定);
        }
        if (!私有_P0_高阶胜出绑定有效(目标.当前高阶胜出绑定)) {
            私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前请求.当前高阶胜出绑定);
        }
        私有_P0_覆盖控制承接(目标.当前控制承接, 来源.当前控制承接);
    }

    void 私有_P0_覆盖决策到兼容字段(
        结构_任务管理结果& 目标,
        const 结构_任务管理单步决策& 来源) noexcept
    {
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前治理状态迁移, 来源.当前治理状态迁移, 枚举_任务管理治理状态迁移::未定义);
        私有_P0_覆盖非默认值(目标.当前方法来源, 来源.当前方法来源, 枚举_任务管理方法来源::未定义);
        私有_P0_覆盖非默认值(目标.当前主方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤位类型, 来源.当前步骤位类型, 枚举_任务管理步骤位类型::未定义);
        私有_P0_覆盖非默认值(目标.当前步骤语义类型, 来源.当前步骤语义类型, 枚举_任务管理步骤语义类型::未定义);
        私有_P0_覆盖非默认值(目标.当前后果治理, 来源.当前后果治理, 枚举_任务管理后果治理::未定义);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.当前影子验证状态, 来源.影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        私有_P1_覆盖任务治理二次特征证据(目标.当前治理二次特征证据, 来源.当前治理二次特征证据);
        私有_P0_覆盖原语判定汇总(目标.当前原语判定汇总, 来源.当前原语判定汇总);
        私有_P0_覆盖高阶排序请求(目标.当前高阶排序请求, 来源.当前高阶排序请求);
        私有_P0_覆盖高阶排序结果(目标.当前高阶排序结果, 来源.当前高阶排序结果);
        私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前高阶胜出绑定);
    }

    void 私有_P0_覆盖写回字段(
        结构_任务管理写回结果& 目标,
        const 结构_任务管理写回结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.当前步骤节点, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.最新结果节点, static_cast<任务节点*>(来源.最新结果节点));
        私有_P0_覆盖非空指针(目标.当前业务子任务, static_cast<任务节点*>(来源.当前业务子任务));
        私有_P0_覆盖非空指针(目标.当前补条件子任务, static_cast<任务节点*>(来源.当前补条件子任务));
        私有_P0_覆盖非空指针(目标.当前学习子任务, static_cast<任务节点*>(来源.当前学习子任务));
        私有_P0_覆盖非空指针(目标.当前学习方法, static_cast<方法节点*>(来源.当前学习方法));
        私有_P0_覆盖非空指针(目标.宿主目标状态, 来源.宿主目标状态);
        私有_P0_覆盖非空指针(目标.宿主结果状态, 来源.宿主结果状态);
        私有_P0_覆盖非空文本(目标.双面摘要, 来源.双面摘要);
        私有_P0_覆盖非空文本(目标.结构动作摘要, 来源.结构动作摘要);
        私有_P0_覆盖非空文本(目标.主体回并摘要, 来源.主体回并摘要);
    }

    void 私有_P0_覆盖兼容写回字段(
        结构_任务管理写回结果& 目标,
        const 结构_任务管理结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.当前步骤节点, 来源.当前步骤节点);
        私有_P0_覆盖非空指针(目标.最新结果节点, 来源.最新结果节点);
        if (来源.已创建业务子任务 || 来源.已复用业务子任务) {
            私有_P0_覆盖非空指针(目标.当前业务子任务, 来源.当前子任务节点);
        }
        if (来源.已创建补条件子任务 || 来源.已复用补条件子任务) {
            私有_P0_覆盖非空指针(目标.当前补条件子任务, 来源.当前子任务节点);
        }
        if (来源.已创建学习子任务 || 来源.已复用学习子任务) {
            私有_P0_覆盖非空指针(目标.当前学习子任务, 来源.当前子任务节点);
        }
        私有_P0_覆盖非空指针(目标.当前学习方法, 来源.当前学习方法节点);
        私有_P0_覆盖非空指针(目标.宿主目标状态, 来源.宿主目标状态);
        私有_P0_覆盖非空指针(目标.宿主结果状态, 来源.宿主结果状态);
        私有_P0_覆盖非空文本(目标.双面摘要, 来源.目标结果差额摘要);
        私有_P0_覆盖非空文本(目标.结构动作摘要, 来源.最近反馈摘要);
        私有_P0_覆盖非空文本(目标.主体回并摘要, 来源.当前写回结果.主体回并摘要);
    }

    任务节点* 私有_P0_选用写回子任务节点(
        const 结构_任务管理写回结果& 写回) noexcept
    {
        return 私有_P0_选用非空指针(
            static_cast<任务节点*>(写回.当前学习子任务),
            static_cast<任务节点*>(写回.当前补条件子任务),
            static_cast<任务节点*>(写回.当前业务子任务));
    }

    void 私有_P0_覆盖写回到兼容字段(
        结构_任务管理结果& 目标,
        const 结构_任务管理写回结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.当前步骤节点, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.最新结果节点, static_cast<任务节点*>(来源.最新结果节点));
        私有_P0_覆盖非空指针(目标.宿主目标状态, 来源.宿主目标状态);
        私有_P0_覆盖非空指针(目标.宿主结果状态, 来源.宿主结果状态);
        私有_P0_覆盖非空指针(目标.当前学习方法节点, static_cast<方法节点*>(来源.当前学习方法));
        if (auto* 当前子任务 = 私有_P0_选用写回子任务节点(来源)) {
            私有_P0_覆盖非空指针(目标.当前子任务节点, 当前子任务);
        }
        私有_P0_覆盖非空文本(目标.目标结果差额摘要, 来源.双面摘要);
    }

    void 私有_P0_覆盖一步结果字段(
        结构_任务管理一步结果& 目标,
        const 结构_任务管理一步结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.宿主任务, static_cast<任务节点*>(来源.宿主任务));
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前治理状态迁移, 来源.当前治理状态迁移, 枚举_任务管理治理状态迁移::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非空指针(目标.当前步骤节点, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.当前方法节点, static_cast<方法节点*>(来源.当前方法节点));
        私有_P0_覆盖非空指针(目标.最新结果节点, static_cast<任务节点*>(来源.最新结果节点));
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        私有_P0_覆盖非空文本(目标.当前输入条件主键, 来源.当前输入条件主键);
        私有_P0_覆盖非空文本(目标.当前治理态型, 来源.当前治理态型);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.影子验证状态, 来源.影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        私有_P1_覆盖任务治理二次特征证据(目标.当前治理二次特征证据, 来源.当前治理二次特征证据);
        私有_P0_覆盖原语判定汇总(目标.当前原语判定汇总, 来源.当前原语判定汇总);
        私有_P0_覆盖高阶排序请求(目标.当前高阶排序请求, 来源.当前高阶排序请求);
        私有_P0_覆盖高阶排序结果(目标.当前高阶排序结果, 来源.当前高阶排序结果);
        私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前高阶胜出绑定);
        私有_P0_覆盖控制承接(目标.当前控制承接, 来源.当前控制承接);
        if (来源.已触发学习承接) {
            目标.已触发学习承接 = true;
        }
        私有_P0_覆盖非默认值(
            目标.当前学习承接类型,
            来源.当前学习承接类型,
            枚举_任务管理学习承接类型::未定义);
        if (来源.当前学习为本能补齐) {
            目标.当前学习为本能补齐 = true;
        }
        私有_P0_覆盖非空文本(目标.学习承接摘要, 来源.学习承接摘要);
        私有_P0_覆盖非空文本(目标.主体回并摘要, 来源.主体回并摘要);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    void 私有_P0_覆盖兼容一步结果字段(
        结构_任务管理一步结果& 目标,
        const 结构_任务管理结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.宿主任务, 来源.上下文.当前宿主任务);
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前治理状态迁移, 来源.当前治理状态迁移, 枚举_任务管理治理状态迁移::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前主方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非空指针(目标.当前步骤节点, 来源.当前步骤节点);
        if (auto* 当前方法 = 私有_P0_当前方法(来源)) {
            私有_P0_覆盖非空指针(目标.当前方法节点, 当前方法);
        }
        私有_P0_覆盖非空指针(目标.最新结果节点, 来源.最新结果节点);
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        私有_P0_覆盖非空文本(目标.当前输入条件主键, 来源.上下文.当前输入条件主键);
        私有_P0_覆盖非空文本(目标.当前治理态型, 来源.上下文.当前治理态型);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.影子验证状态, 来源.当前影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        私有_P1_覆盖任务治理二次特征证据(目标.当前治理二次特征证据, 来源.当前治理二次特征证据);
        私有_P0_覆盖原语判定汇总(目标.当前原语判定汇总, 来源.当前原语判定汇总);
        私有_P0_覆盖高阶排序请求(目标.当前高阶排序请求, 来源.当前请求.当前高阶排序请求);
        私有_P0_覆盖高阶排序结果(目标.当前高阶排序结果, 来源.当前高阶排序结果);
        私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前高阶胜出绑定);
        if (!私有_P0_高阶胜出绑定有效(目标.当前高阶胜出绑定)) {
            私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前单步决策.当前高阶胜出绑定);
        }
        if (!私有_P0_高阶胜出绑定有效(目标.当前高阶胜出绑定)) {
            私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前请求.当前高阶胜出绑定);
        }
        私有_P0_覆盖控制承接(目标.当前控制承接, 来源.当前控制承接);
        if (来源.当前学习承接决策.是否触发学习) {
            目标.已触发学习承接 = true;
        }
        私有_P0_覆盖非默认值(
            目标.当前学习承接类型,
            来源.当前学习承接决策.学习类型,
            枚举_任务管理学习承接类型::未定义);
        if (来源.当前学习承接决策.补原语或治理本能) {
            目标.当前学习为本能补齐 = true;
        }
        私有_P0_覆盖非空文本(目标.学习承接摘要, 来源.当前学习承接决策.决策摘要);
        私有_P0_覆盖非空文本(目标.主体回并摘要, 来源.当前写回结果.主体回并摘要);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    void 私有_P0_覆盖一步结果到兼容字段(
        结构_任务管理结果& 目标,
        const 结构_任务管理一步结果& 来源) noexcept
    {
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.当前主方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前触发来源, 来源.当前触发来源, 枚举_任务管理触发来源::未定义);
        私有_P0_覆盖非空指针(目标.当前步骤节点, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.最新结果节点, static_cast<任务节点*>(来源.最新结果节点));
        私有_P0_覆盖非默认值(目标.最近功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.当前影子验证状态, 来源.影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        私有_P1_覆盖任务治理二次特征证据(目标.当前治理二次特征证据, 来源.当前治理二次特征证据);
        私有_P0_覆盖原语判定汇总(目标.当前原语判定汇总, 来源.当前原语判定汇总);
        私有_P0_覆盖高阶排序请求(目标.当前高阶排序请求, 来源.当前高阶排序请求);
        私有_P0_覆盖高阶排序请求(目标.当前请求.当前高阶排序请求, 来源.当前高阶排序请求);
        私有_P0_覆盖高阶排序结果(目标.当前高阶排序结果, 来源.当前高阶排序结果);
        私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前高阶胜出绑定);
        私有_P0_覆盖高阶胜出绑定(目标.当前请求.当前高阶胜出绑定, 来源.当前高阶胜出绑定);
        私有_P0_覆盖控制承接(目标.当前控制承接, 来源.当前控制承接);
        私有_P0_覆盖控制承接(目标.当前请求.当前控制承接, 来源.当前控制承接);
        if (来源.已触发学习承接) {
            目标.当前学习承接决策.是否触发学习 = true;
        }
        私有_P0_覆盖非默认值(
            目标.当前学习承接决策.学习类型,
            来源.当前学习承接类型,
            枚举_任务管理学习承接类型::未定义);
        if (来源.当前学习为本能补齐) {
            目标.当前学习承接决策.补原语或治理本能 = true;
        }
        私有_P0_覆盖非空文本(目标.当前学习承接决策.决策摘要, 来源.学习承接摘要);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    void 私有_P0_覆盖上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理上下文& 来源) noexcept
    {
        if (来源.当前时间 != 0) {
            目标.当前时间 = 来源.当前时间;
        }
        私有_P0_覆盖非空指针(目标.当前主需求, 来源.当前主需求);
        私有_P0_覆盖非空指针(目标.当前管理任务, 来源.当前管理任务);
        私有_P0_覆盖非空指针(目标.当前宿主任务, 来源.当前宿主任务);
        私有_P0_覆盖非空指针(目标.当前方法, 来源.当前方法);
        私有_P0_覆盖非空指针(目标.当前步骤, 来源.当前步骤);
        私有_P0_覆盖非空指针(目标.最近结果, 来源.最近结果);
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        if (来源.当前步骤可复用) {
            目标.当前步骤可复用 = true;
        }
        if (来源.最近结果命中当前治理) {
            目标.最近结果命中当前治理 = true;
        }
        if (来源.当前动作长时间无推进) {
            目标.当前动作长时间无推进 = true;
        }
        if (来源.当前动作需重试) {
            目标.当前动作需重试 = true;
        }
        if (来源.当前需等待) {
            目标.当前需等待 = true;
        }
        if (来源.当前需收束) {
            目标.当前需收束 = true;
        }
        if (来源.可继续推进) {
            目标.可继续推进 = true;
        }
        if (来源.当前可回接最近结果) {
            目标.当前可回接最近结果 = true;
        }
        if (来源.稳定等待态复盘学习) {
            目标.稳定等待态复盘学习 = true;
        }
        if (来源.业务子任务已完成) {
            目标.业务子任务已完成 = true;
        }
        if (来源.业务子任务应转执行) {
            目标.业务子任务应转执行 = true;
        }
        私有_P0_覆盖非默认值(目标.最近根层重判结果, 来源.最近根层重判结果, 枚举_任务管理根层重判结果::未定义);
        私有_P0_覆盖非默认值(目标.最近执行前门控结果, 来源.最近执行前门控结果, 枚举_任务管理执行前门控结果::未定义);
        私有_P0_覆盖非空文本(目标.最近上层反馈摘要, 来源.最近上层反馈摘要);
        私有_P0_覆盖非默认值(目标.最近功能域, 来源.最近功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        私有_P0_覆盖非默认值(目标.最近宿主目标结果比较, 来源.最近宿主目标结果比较, 枚举_任务管理双面比较结果::未定义);
        私有_P0_覆盖非默认值(目标.最近后果治理, 来源.最近后果治理, 枚举_任务管理后果治理::未定义);
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非空文本(目标.当前输入条件主键, 来源.当前输入条件主键);
        私有_P0_覆盖非空文本(目标.当前治理态型, 来源.当前治理态型);
        私有_P0_覆盖非空指针(目标.当前方法位专项动态, 来源.当前方法位专项动态);
        私有_P0_覆盖非空文本(目标.当前方法位专项动态标题, 来源.当前方法位专项动态标题);
        私有_P0_覆盖非空指针(目标.当前最小原语动态, 来源.当前最小原语动态);
        私有_P0_覆盖非空文本(目标.当前最小原语动态标题, 来源.当前最小原语动态标题);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    void 私有_P0_覆盖请求字段(
        结构_任务管理请求& 目标,
        const 结构_任务管理请求& 来源) noexcept
    {
        if (来源.请求ID != 0) {
            目标.请求ID = 来源.请求ID;
        }
        if (来源.提交时间 != 0) {
            目标.提交时间 = 来源.提交时间;
        }
        if (来源.提交序号 != 0) {
            目标.提交序号 = 来源.提交序号;
        }
        私有_P0_覆盖非空指针(目标.宿主任务, 来源.宿主任务);
        私有_P0_覆盖上下文字段(目标.上下文, 来源.上下文);
        私有_P0_覆盖非默认值(目标.触发事件类型, 来源.触发事件类型, 枚举_任务管理触发事件类型::未定义);
        私有_P0_覆盖非默认值(目标.触发来源, 来源.触发来源, 枚举_任务管理触发来源::未定义);
        if (来源.来源最小原语位图 != 0) {
            目标.来源最小原语位图 = 来源.来源最小原语位图;
        }
        私有_P0_覆盖非空文本(目标.来源主观察特征, 来源.来源主观察特征);
        私有_P0_覆盖非空文本(目标.影子验证状态, 来源.影子验证状态);
        if (来源.允许正式资产提交) {
            目标.允许正式资产提交 = true;
        }
        私有_P1_覆盖任务治理二次特征证据(目标.当前治理二次特征证据, 来源.当前治理二次特征证据);
        私有_P0_覆盖原语判定汇总(目标.当前原语判定汇总, 来源.当前原语判定汇总);
        私有_P0_覆盖非空文本(目标.当前需求轴键, 来源.当前需求轴键);
        if (!来源.当前升阶候选组.empty()) {
            目标.当前升阶候选组 = 来源.当前升阶候选组;
        }
        私有_P0_覆盖高阶排序请求(目标.当前高阶排序请求, 来源.当前高阶排序请求);
        私有_P0_覆盖高阶胜出绑定(目标.当前高阶胜出绑定, 来源.当前高阶胜出绑定);
        if (来源.存在控制请求) {
            目标.存在控制请求 = true;
        }
        私有_P0_覆盖控制请求(目标.当前控制请求, 来源.当前控制请求);
        私有_P0_覆盖控制承接(目标.当前控制承接, 来源.当前控制承接);
        私有_P0_覆盖非空文本(目标.触发事件摘要, 来源.触发事件摘要);
        私有_P0_覆盖非空文本(目标.当前恢复点类型, 来源.当前恢复点类型);
        if (来源.存在待消费学习反馈) {
            目标.存在待消费学习反馈 = true;
        }
        if (来源.存在待消费学习回流) {
            目标.存在待消费学习回流 = true;
        }
        if (来源.存在待消费外部反馈) {
            目标.存在待消费外部反馈 = true;
        }
        私有_P0_覆盖非空文本(目标.恢复重建摘要, 来源.恢复重建摘要);
        私有_P0_覆盖非空文本(目标.补充材料摘要, 来源.补充材料摘要);
    }

    void 私有_P0_覆盖请求到上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理请求& 来源) noexcept
    {
        私有_P0_覆盖上下文字段(目标, 来源.上下文);
        if (来源.提交时间 != 0) {
            目标.当前时间 = 来源.提交时间;
        }
        私有_P0_覆盖非空指针(目标.当前宿主任务, static_cast<任务节点*>(来源.宿主任务));
    }

    void 私有_P0_覆盖决策到上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理单步决策& 来源) noexcept
    {
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.最近功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.最近后果治理, 来源.当前后果治理, 枚举_任务管理后果治理::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.决策摘要);
    }

    void 私有_P0_覆盖写回到上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理写回结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.当前步骤, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.最近结果, static_cast<任务节点*>(来源.最新结果节点));
    }

    void 私有_P0_覆盖一步结果到上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理一步结果& 来源) noexcept
    {
        私有_P0_覆盖非空指针(目标.当前宿主任务, static_cast<任务节点*>(来源.宿主任务));
        私有_P0_覆盖非空指针(目标.当前方法, static_cast<方法节点*>(来源.当前方法节点));
        私有_P0_覆盖非空指针(目标.当前步骤, static_cast<任务节点*>(来源.当前步骤节点));
        私有_P0_覆盖非空指针(目标.最近结果, static_cast<任务节点*>(来源.最新结果节点));
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.最近功能域, 来源.当前功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非空文本(目标.当前输入条件主键, 来源.当前输入条件主键);
        私有_P0_覆盖非空文本(目标.当前治理态型, 来源.当前治理态型);
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    void 私有_P0_覆盖兼容到上下文字段(
        结构_任务管理上下文& 目标,
        const 结构_任务管理结果& 来源) noexcept
    {
        私有_P0_覆盖非默认值(目标.当前缺口类型, 来源.当前缺口类型, 枚举_任务管理缺口类型::未定义);
        私有_P0_覆盖非默认值(目标.当前下一步去向, 来源.当前下一步去向, 枚举_任务管理下一步去向::未定义);
        私有_P0_覆盖非默认值(目标.最近功能域, 来源.最近功能域, 枚举_任务管理功能域::未定义);
        私有_P0_覆盖非默认值(目标.最近总控结果, 来源.最近总控结果, 枚举_任务管理总控结果::未定义);
        私有_P0_覆盖非默认值(目标.最近反馈类型, 来源.最近反馈类型, 枚举_任务管理反馈类型::未定义);
        私有_P0_覆盖非默认值(目标.最近宿主目标结果比较, 来源.最近宿主目标结果比较, 枚举_任务管理双面比较结果::未定义);
        私有_P0_覆盖非默认值(目标.最近后果治理, 来源.当前后果治理, 枚举_任务管理后果治理::未定义);
        if (来源.当前主体ID != 0) {
            目标.当前主体ID = 来源.当前主体ID;
        }
        if (来源.当前分身ID != 0) {
            目标.当前分身ID = 来源.当前分身ID;
        }
        私有_P0_覆盖非默认值(目标.当前方法需求位, 来源.当前主方法需求位, 枚举_任务管理方法需求位::未定义);
        私有_P0_覆盖非默认值(目标.当前本能能力缺口类型, 来源.当前本能能力缺口类型, 枚举_任务管理本能能力缺口类型::未定义);
        私有_P0_覆盖非空文本(目标.当前输入条件主键, 来源.当前需求位特征.输入条件主键);
        私有_P0_覆盖非空文本(目标.当前治理态型, 来源.当前一步结果.当前治理态型);
        if (来源.当前需求位特征.当前步骤可复用) {
            目标.当前步骤可复用 = true;
        }
        if (来源.当前需求位特征.最近结果命中当前治理) {
            目标.最近结果命中当前治理 = true;
        }
        if (来源.当前需求位特征.当前动作长时间无推进) {
            目标.当前动作长时间无推进 = true;
        }
        if (来源.当前需求位特征.当前动作需重试) {
            目标.当前动作需重试 = true;
        }
        if (来源.当前需求位特征.当前需等待) {
            目标.当前需等待 = true;
        }
        if (来源.当前需求位特征.当前需收束) {
            目标.当前需收束 = true;
        }
        if (来源.当前需求位特征.可继续推进) {
            目标.可继续推进 = true;
        }
        if (来源.当前需求位特征.当前可回接最近结果) {
            目标.当前可回接最近结果 = true;
        }
        if (来源.当前需求位特征.稳定等待态复盘学习) {
            目标.稳定等待态复盘学习 = true;
        }
        if (来源.当前需求位特征.业务子任务已完成) {
            目标.业务子任务已完成 = true;
        }
        if (来源.当前需求位特征.业务子任务应转执行) {
            目标.业务子任务应转执行 = true;
        }
        私有_P0_覆盖非空文本(目标.最近反馈摘要, 来源.最近反馈摘要);
    }

    std::string 私有_治理态型文本(const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << "域=" << 私有_功能域文本(私有_P0_当前功能域(结果))
            << "|步骤位=" << 私有_步骤位类型文本(私有_P0_当前步骤位类型(结果))
            << "|需求位=" << 私有_方法需求位文本(私有_P0_当前方法需求位(结果));
        return 输出.str();
    }

    std::string 私有_任务管理动态聚合方式文本(枚举_动态聚合方式 聚合方式) noexcept
    {
        switch (聚合方式) {
        case 枚举_动态聚合方式::原子相邻: return "原子相邻";
        case 枚举_动态聚合方式::连续同向段: return "连续同向段";
        case 枚举_动态聚合方式::同类事件段: return "同类事件段";
        case 枚举_动态聚合方式::窗口聚合: return "窗口聚合";
        case 枚举_动态聚合方式::路径压缩: return "路径压缩";
        default: return "未定义";
        }
    }

    bool 私有_动态来源链包含目标(
        动态节点类* 候选动态,
        动态节点类* 目标动态,
        std::uint32_t 深度 = 0) noexcept
    {
        if (!候选动态 || !目标动态 || 深度 > 8) {
            return false;
        }

        const auto* 候选主信息 = 世界树.动态().取动态主信息(候选动态);
        if (!候选主信息 || 候选主信息->来源低层动态.empty()) {
            return false;
        }

        const auto 目标主键 = 目标动态->获取主键();
        for (const auto& 引用 : 候选主信息->来源低层动态) {
            auto* 来源动态 = 引用.指针;
            const auto 来源主键 =
                !引用.主键.empty()
                ? 引用.主键
                : (来源动态 ? 来源动态->获取主键() : std::string{});
            if (来源主键 == 目标主键) {
                return true;
            }
            if (来源动态 && 私有_动态来源链包含目标(来源动态, 目标动态, 深度 + 1)) {
                return true;
            }
        }
        return false;
    }

    动态节点类* 私有_选择任务管理桥接动态(动态节点类* 原始动态) noexcept
    {
        if (!原始动态) {
            return nullptr;
        }

        auto* 最佳动态 = 原始动态;
        auto* 父节点 = static_cast<基础信息节点类*>(原始动态->父);
        auto* 场景 = static_cast<场景节点类*>(父节点);
        if (!场景) {
            return 最佳动态;
        }

        for (auto* 候选动态 : 世界树.动态().获取场景动态(场景)) {
            if (!候选动态 || 候选动态 == 原始动态) {
                continue;
            }
            const auto* 候选主信息 = 世界树.动态().取动态主信息(候选动态);
            const auto* 最佳主信息 = 世界树.动态().取动态主信息(最佳动态);
            if (!候选主信息 || !最佳主信息 || 候选主信息->来源低层动态.empty()) {
                continue;
            }
            if (!私有_动态来源链包含目标(候选动态, 原始动态)) {
                continue;
            }

            const bool 层级更高 = 候选主信息->动态层级 > 最佳主信息->动态层级;
            const bool 同层但覆盖更宽 =
                候选主信息->动态层级 == 最佳主信息->动态层级
                && 候选主信息->来源低层动态.size() > 最佳主信息->来源低层动态.size();
            if (层级更高 || 同层但覆盖更宽) {
                最佳动态 = 候选动态;
            }
        }

        return 最佳动态;
    }

    std::string 私有_任务管理桥接动态标题(
        动态节点类* 动态,
        const std::string& 回退标题) noexcept
    {
        auto* 桥接动态 = 私有_选择任务管理桥接动态(动态);
        if (!桥接动态) {
            return 回退标题;
        }

        const auto* 动态主信息 = 世界树.动态().取动态主信息(桥接动态);
        if (!动态主信息) {
            return 回退标题;
        }

        std::ostringstream 输出;
        if (!回退标题.empty()) {
            输出 << 回退标题;
        }
        else if (动态主信息->来源动作名) {
            输出 << 动态主信息->来源动作名->获取主键();
        }
        else {
            输出 << "动态";
        }

        if (动态主信息->动态层级 != 0
            || 动态主信息->聚合方式 != 枚举_动态聚合方式::原子相邻
            || !动态主信息->来源低层动态.empty()) {
            输出 << " [层级=" << 动态主信息->动态层级
                << " 聚合=" << 私有_任务管理动态聚合方式文本(动态主信息->聚合方式);
            if (!动态主信息->来源低层动态.empty()) {
                输出 << " 段数=" << 动态主信息->来源低层动态.size();
            }
            输出 << "]";
        }
        return 输出.str();
    }

    const 动态节点主信息类* 私有_读取任务管理桥接动态主信息(
        动态节点类* 动态) noexcept
    {
        auto* 桥接动态 = 私有_选择任务管理桥接动态(动态);
        if (!桥接动态) {
            桥接动态 = 动态;
        }
        return 桥接动态 ? 世界树.动态().取动态主信息(桥接动态) : nullptr;
    }

    bool 私有_任务管理动态标题含片段(
        const std::string& 标题,
        const char* 片段) noexcept
    {
        return !标题.empty() && 片段 && 标题.find(片段) != std::string::npos;
    }

    bool 私有_任务管理动态已开始(
        const 动态节点主信息类* 动态主信息,
        const std::string& 标题) noexcept
    {
        if (!动态主信息 && 标题.empty()) {
            return false;
        }
        if (动态主信息
            && 动态主信息->来源动作相位 == 枚举_动作事件相位::开始运行) {
            return true;
        }
        return 私有_任务管理动态标题含片段(标题, "开始")
            || 私有_任务管理动态标题含片段(标题, "启动")
            || 私有_任务管理动态标题含片段(标题, "运行中");
    }

    bool 私有_任务管理动态失败退出(
        const 动态节点主信息类* 动态主信息,
        const std::string& 标题) noexcept
    {
        if (!动态主信息 && 标题.empty()) {
            return false;
        }
        if (动态主信息
            && 动态主信息->来源动作相位 == 枚举_动作事件相位::失败) {
            return true;
        }
        if (动态主信息
            && 动态主信息->来源类型 != 枚举_动态来源类型::未定义
            && !动态主信息->来源执行成功) {
            return true;
        }
        return 私有_任务管理动态标题含片段(标题, "失败")
            || 私有_任务管理动态标题含片段(标题, "中断")
            || 私有_任务管理动态标题含片段(标题, "拒绝")
            || 私有_任务管理动态标题含片段(标题, "停止");
    }

    bool 私有_任务管理动态已完成(
        const 动态节点主信息类* 动态主信息,
        const std::string& 标题) noexcept
    {
        if (!动态主信息 && 标题.empty()) {
            return false;
        }
        if (私有_任务管理动态失败退出(动态主信息, 标题)) {
            return false;
        }
        if (动态主信息
            && 动态主信息->来源动作相位 == 枚举_动作事件相位::完成) {
            return true;
        }
        if (动态主信息
            && 动态主信息->业务语义类型 == 枚举_动态业务语义类型::边界结束动态) {
            return true;
        }
        return 私有_任务管理动态标题含片段(标题, "完成")
            || 私有_任务管理动态标题含片段(标题, "结束")
            || 私有_任务管理动态标题含片段(标题, "收束");
    }

    bool 私有_任务管理动态处于方法位推进中(
        const 动态节点主信息类* 动态主信息,
        const std::string& 标题) noexcept
    {
        if (!动态主信息 && 标题.empty()) {
            return false;
        }
        if (私有_任务管理动态失败退出(动态主信息, 标题)
            || 私有_任务管理动态已完成(动态主信息, 标题)) {
            return false;
        }
        if (动态主信息
            && (动态主信息->来源动作相位 == 枚举_动作事件相位::开始运行
                || 动态主信息->业务语义类型 == 枚举_动态业务语义类型::治理过程动态
                || 动态主信息->业务语义类型 == 枚举_动态业务语义类型::动作事实动态)) {
            return true;
        }
        return 私有_任务管理动态标题含片段(标题, "推进")
            || 私有_任务管理动态标题含片段(标题, "治理")
            || 私有_任务管理动态标题含片段(标题, "开始");
    }

    bool 私有_任务管理动态处于原语边界执行中(
        const 动态节点主信息类* 动态主信息,
        const std::string& 标题) noexcept
    {
        if (!动态主信息 && 标题.empty()) {
            return false;
        }
        if (私有_任务管理动态失败退出(动态主信息, 标题)
            || 私有_任务管理动态已完成(动态主信息, 标题)) {
            return false;
        }
        if (动态主信息
            && 动态主信息->来源动作相位 == 枚举_动作事件相位::开始运行) {
            return true;
        }
        return 私有_任务管理动态标题含片段(标题, "原语_")
            && 私有_任务管理动态标题含片段(标题, "开始");
    }

    void 私有_P0_刷新主体桥接字段(结构_任务管理结果& 结果) noexcept
    {
        const auto 当前方法需求位 = 私有_P0_当前方法需求位(结果);
        const auto 当前本能能力缺口类型 = 私有_P0_当前本能缺口(结果);
        const auto 当前输入条件主键 =
            !结果.当前分身.本轮输入条件主键.empty()
            ? 结果.当前分身.本轮输入条件主键
            : 结果.当前需求位特征.输入条件主键;
        const auto 当前治理态型 = 私有_治理态型文本(结果);
        const auto 已选方法位专项动态标题 = 私有_P0_选用非空文本(
            结果.当前需求位特征.当前方法位专项动态标题,
            结果.当前分身.当前方法位专项动态标题,
            私有_P0_选用非空文本(
                结果.当前主体.最近方法位专项动态标题,
                结果.上下文.当前方法位专项动态标题,
                std::string{}));
        const auto 已选最小原语动态标题 = 私有_P0_选用非空文本(
            结果.当前本能能力缺口.当前最小原语动态标题,
            结果.当前分身.当前最小原语动态标题,
            私有_P0_选用非空文本(
                结果.当前主体.最近最小原语动态标题,
                结果.上下文.当前最小原语动态标题,
                std::string{}));
        auto* 当前方法位专项动态 =
            结果.当前需求位特征.当前方法位专项动态
            ? 结果.当前需求位特征.当前方法位专项动态
            : (
                结果.当前分身.当前方法位专项动态
                ? 结果.当前分身.当前方法位专项动态
                : (
                    结果.当前主体.最近方法位专项动态
                    ? 结果.当前主体.最近方法位专项动态
                    : 结果.上下文.当前方法位专项动态
                )
            );
        auto* 当前最小原语动态 =
            结果.当前本能能力缺口.当前最小原语动态
            ? 结果.当前本能能力缺口.当前最小原语动态
            : (
                结果.当前分身.当前最小原语动态
                ? 结果.当前分身.当前最小原语动态
                : (
                    结果.当前主体.最近最小原语动态
                    ? 结果.当前主体.最近最小原语动态
                    : 结果.上下文.当前最小原语动态
                )
            );
        auto* 当前桥接方法位专项动态 = 私有_选择任务管理桥接动态(当前方法位专项动态);
        if (!当前桥接方法位专项动态) {
            当前桥接方法位专项动态 = 当前方法位专项动态;
        }
        auto* 当前桥接最小原语动态 = 私有_选择任务管理桥接动态(当前最小原语动态);
        if (!当前桥接最小原语动态) {
            当前桥接最小原语动态 = 当前最小原语动态;
        }
        const auto 当前方法位专项动态标题 = 私有_任务管理桥接动态标题(
            当前桥接方法位专项动态,
            已选方法位专项动态标题);
        const auto 当前最小原语动态标题 = 私有_任务管理桥接动态标题(
            当前桥接最小原语动态,
            已选最小原语动态标题);

        if (结果.当前主体.主体ID != 0) {
            结果.当前主体ID = 结果.当前主体.主体ID;
        }
        if (结果.当前分身.分身ID != 0) {
            结果.当前分身ID = 结果.当前分身.分身ID;
        }
        if (结果.当前触发来源 == 枚举_任务管理触发来源::未定义) {
            结果.当前触发来源 = 枚举_任务管理触发来源::自然运行态;
        }
        if (结果.来源最小原语位图 == 0 && 结果.当前本能能力缺口.所需原语位图 != 0) {
            结果.来源最小原语位图 = 结果.当前本能能力缺口.所需原语位图;
        }
        if (结果.来源主观察特征.empty()) {
            结果.来源主观察特征 = 结果.当前本能能力缺口.主观察特征键;
        }
        if (结果.当前影子验证状态.empty()) {
            结果.当前影子验证状态 = "未触发";
        }
        if (结果.已初始化分身继承面) {
            结果.当前分身.当前本能能力缺口类型 = 当前本能能力缺口类型;
            结果.当前分身.当前治理态型 = 当前治理态型;
            结果.当前分身.当前方法位专项动态 = 当前桥接方法位专项动态;
            结果.当前分身.当前方法位专项动态标题 = 当前方法位专项动态标题;
            结果.当前分身.当前最小原语动态 = 当前桥接最小原语动态;
            结果.当前分身.当前最小原语动态标题 = 当前最小原语动态标题;
        }
        if (结果.已确保任务管理主体虚拟存在) {
            私有_P0_覆盖非默认值(
                结果.当前主体.最近本能能力缺口类型,
                当前本能能力缺口类型,
                枚举_任务管理本能能力缺口类型::未定义);
            私有_P0_覆盖非空文本(结果.当前主体.最近治理态型, 当前治理态型);
            结果.当前主体.最近方法位专项动态 = 当前桥接方法位专项动态;
            结果.当前主体.最近方法位专项动态标题 = 当前方法位专项动态标题;
            结果.当前主体.最近最小原语动态 = 当前桥接最小原语动态;
            结果.当前主体.最近最小原语动态标题 = 当前最小原语动态标题;
        }

        结果.上下文.当前主体ID = 结果.当前主体ID;
        结果.上下文.当前分身ID = 结果.当前分身ID;
        结果.上下文.当前方法需求位 = 当前方法需求位;
        结果.上下文.当前本能能力缺口类型 = 当前本能能力缺口类型;
        结果.上下文.当前输入条件主键 = 当前输入条件主键;
        结果.上下文.当前治理态型 = 当前治理态型;
        结果.上下文.当前方法位专项动态 = 当前桥接方法位专项动态;
        结果.上下文.当前方法位专项动态标题 = 当前方法位专项动态标题;
        结果.上下文.当前最小原语动态 = 当前桥接最小原语动态;
        结果.上下文.当前最小原语动态标题 = 当前最小原语动态标题;

        私有_P0_覆盖非空指针(结果.当前请求.宿主任务, 结果.上下文.当前宿主任务);
        私有_P0_覆盖上下文字段(结果.当前请求.上下文, 结果.上下文);
        结果.当前请求.触发来源 = 结果.当前触发来源;
        结果.当前请求.来源最小原语位图 = 结果.来源最小原语位图;
        结果.当前请求.来源主观察特征 = 结果.来源主观察特征;
        结果.当前请求.影子验证状态 = 结果.当前影子验证状态;
        结果.当前请求.允许正式资产提交 = 结果.允许正式资产提交;
        私有_P1_覆盖任务治理二次特征证据(结果.当前请求.当前治理二次特征证据, 结果.当前治理二次特征证据);
        if (结果.当前原语判定汇总.明细.empty()) {
            私有_P0_刷新请求原语锚点(结果.当前请求);
            (void)私有_P0_生成原语判定汇总(结果.当前请求, 结果.当前原语判定汇总);
        }
        else {
            私有_P0_刷新请求原语锚点(结果.当前请求);
        }
        私有_P0_覆盖原语判定汇总(结果.当前请求.当前原语判定汇总, 结果.当前原语判定汇总);
        if (结果.当前请求.存在控制请求) {
            结果.当前请求.存在控制请求 = true;
        }
        私有_P0_覆盖控制请求(结果.当前请求.当前控制请求, 结果.当前请求.当前控制请求);
        私有_P0_覆盖控制承接(结果.当前请求.当前控制承接, 结果.当前控制承接);
        if (!私有_P0_高阶排序请求有效(结果.当前高阶排序请求)) {
            (void)私有_P0_生成高阶排序请求(结果.当前请求, 结果.当前高阶排序请求);
        }
        私有_P0_覆盖高阶排序请求(结果.当前请求.当前高阶排序请求, 结果.当前高阶排序请求);
        if (!私有_P0_高阶排序结果有效(结果.当前高阶排序结果)) {
            (void)私有_P0_生成高阶排序结果(结果.当前请求, 结果.当前高阶排序结果);
        }
        if (!私有_P0_高阶胜出绑定有效(结果.当前高阶胜出绑定)) {
            (void)私有_P0_解析高阶胜出绑定(
                结果.当前请求,
                结果.当前高阶排序结果,
                结果.当前高阶胜出绑定);
        }
        私有_P0_覆盖高阶胜出绑定(结果.当前请求.当前高阶胜出绑定, 结果.当前高阶胜出绑定);
        if (结果.当前请求.触发事件摘要.empty()
            && !结果.当前请求.恢复重建摘要.empty()) {
            结果.当前请求.触发事件摘要 = 结果.当前请求.恢复重建摘要;
        }
        {
            std::ostringstream 请求摘要;
            请求摘要 << "宿主=" << 私有_任务标题(结果.上下文.当前宿主任务)
                << " | 方法=" << 私有_方法标题(结果.上下文.当前方法)
                << " | 事件=" << 私有_触发事件类型文本(结果.当前请求.触发事件类型)
                << " | 需求位=" << 私有_方法需求位文本(当前方法需求位)
                << " | 本能缺口=" << 私有_本能能力缺口文本(当前本能能力缺口类型)
                << " | 触发来源=" << 私有_触发来源文本(结果.当前触发来源)
                << " | 本轮原语命中=" << (结果.来源最小原语位图 == 0 ? std::string("空") : 私有_原语位图摘要(结果.来源最小原语位图))
                << " | 正式已具备=" << (结果.当前本能能力缺口.原语账本摘要.empty() ? std::string("空") : 结果.当前本能能力缺口.原语账本摘要)
                << " | 治理证据=" << (结果.当前请求.当前治理二次特征证据.摘要.empty() ? std::string("空") : 结果.当前请求.当前治理二次特征证据.摘要)
                << " | 主观察=" << (结果.来源主观察特征.empty() ? "空" : 结果.来源主观察特征)
                << " | 影子验证=" << (结果.当前影子验证状态.empty() ? "未触发" : 结果.当前影子验证状态)
                << " | 正式提交=" << (结果.允许正式资产提交 ? "允许" : "禁止")
                << " | 需求轴=" << (结果.当前请求.当前需求轴键.empty() ? "空" : 结果.当前请求.当前需求轴键)
                << " | 升阶候选=" << 私有_P0_升阶候选摘要(结果.当前请求)
                << " | 高阶请求=" << 私有_P0_高阶排序请求摘要(结果.当前请求.当前高阶排序请求)
                << " | 高阶排序=" << 私有_P0_高阶排序结果摘要(结果.当前高阶排序结果)
                << " | 高阶绑定=" << 私有_P0_高阶胜出绑定摘要(结果.当前高阶胜出绑定)
                << " | 控制请求=" << (结果.当前请求.存在控制请求 ? 私有_P0_控制请求摘要(结果.当前请求.当前控制请求) : std::string("空"))
                << " | 控制承接=" << (私有_P0_控制承接有效(结果.当前请求.当前控制承接) ? 私有_P0_控制承接摘要(结果.当前请求.当前控制承接) : std::string("空"))
                << " | 输入条件=" << (当前输入条件主键.empty() ? "空" : 当前输入条件主键)
                << " | 态型=" << 当前治理态型;
            if (!当前方法位专项动态标题.empty()) {
                请求摘要 << " | 方法位动态=" << 当前方法位专项动态标题;
            }
            if (!当前最小原语动态标题.empty()) {
                请求摘要 << " | 原语动态=" << 当前最小原语动态标题;
            }
            if (!结果.当前请求.触发事件摘要.empty()) {
                请求摘要 << " | mailbox=" << 结果.当前请求.触发事件摘要;
            }
            if (!结果.当前请求.当前恢复点类型.empty()) {
                请求摘要 << " | 恢复点=" << 结果.当前请求.当前恢复点类型;
            }
            if (结果.当前请求.存在待消费学习反馈
                || 结果.当前请求.存在待消费学习回流
                || 结果.当前请求.存在待消费外部反馈) {
                请求摘要 << " | 待消费="
                    << "学习反馈=" << (结果.当前请求.存在待消费学习反馈 ? "是" : "否")
                    << "/"
                    << "兼容回流=" << (结果.当前请求.存在待消费学习回流 ? "是" : "否")
                    << "/外部反馈=" << (结果.当前请求.存在待消费外部反馈 ? "是" : "否");
            }
            if (!结果.当前请求.恢复重建摘要.empty()) {
                请求摘要 << " | 恢复重建=" << 结果.当前请求.恢复重建摘要;
            }
            结果.当前请求.补充材料摘要.clear();
        }

        结果.当前单步决策.当前方法需求位 = 当前方法需求位;
        结果.当前单步决策.当前本能能力缺口类型 = 当前本能能力缺口类型;
        结果.当前单步决策.当前触发来源 = 结果.当前触发来源;
        结果.当前单步决策.来源最小原语位图 = 结果.来源最小原语位图;
        结果.当前单步决策.来源主观察特征 = 结果.来源主观察特征;
        结果.当前单步决策.影子验证状态 = 结果.当前影子验证状态;
        结果.当前单步决策.允许正式资产提交 = 结果.允许正式资产提交;
        私有_P1_覆盖任务治理二次特征证据(结果.当前单步决策.当前治理二次特征证据, 结果.当前治理二次特征证据);
        私有_P0_覆盖原语判定汇总(结果.当前单步决策.当前原语判定汇总, 结果.当前原语判定汇总);
        私有_P0_覆盖高阶排序请求(结果.当前单步决策.当前高阶排序请求, 结果.当前高阶排序请求);
        私有_P0_覆盖高阶排序结果(结果.当前单步决策.当前高阶排序结果, 结果.当前高阶排序结果);
        私有_P0_覆盖控制承接(结果.当前单步决策.当前控制承接, 结果.当前控制承接);

        私有_P0_覆盖非空指针(结果.当前一步结果.宿主任务, 结果.上下文.当前宿主任务);
        结果.当前一步结果.当前主体ID = 结果.当前主体ID;
        结果.当前一步结果.当前分身ID = 结果.当前分身ID;
        结果.当前一步结果.当前方法需求位 = 当前方法需求位;
        结果.当前一步结果.当前本能能力缺口类型 = 当前本能能力缺口类型;
        结果.当前一步结果.当前触发来源 = 结果.当前触发来源;
        私有_P0_覆盖非空指针(结果.当前一步结果.当前方法节点, 结果.上下文.当前方法);
        私有_P0_覆盖非空指针(结果.当前一步结果.当前步骤节点, 私有_P0_当前步骤节点(结果));
        私有_P0_覆盖非空指针(结果.当前一步结果.最新结果节点, 私有_P0_最新结果节点(结果));
        结果.当前一步结果.当前输入条件主键 = 当前输入条件主键;
        结果.当前一步结果.当前治理态型 = 当前治理态型;
        结果.当前一步结果.来源最小原语位图 = 结果.来源最小原语位图;
        结果.当前一步结果.来源主观察特征 = 结果.来源主观察特征;
        结果.当前一步结果.影子验证状态 = 结果.当前影子验证状态;
        结果.当前一步结果.允许正式资产提交 = 结果.允许正式资产提交;
        私有_P1_覆盖任务治理二次特征证据(结果.当前一步结果.当前治理二次特征证据, 结果.当前治理二次特征证据);
        私有_P0_覆盖原语判定汇总(结果.当前一步结果.当前原语判定汇总, 结果.当前原语判定汇总);
        私有_P0_覆盖高阶排序请求(结果.当前一步结果.当前高阶排序请求, 结果.当前高阶排序请求);
        私有_P0_覆盖高阶排序结果(结果.当前一步结果.当前高阶排序结果, 结果.当前高阶排序结果);
        私有_P0_覆盖控制承接(结果.当前一步结果.当前控制承接, 结果.当前控制承接);
    }

    void 私有_收口触发信息(结构_任务管理结果& 结果) noexcept
    {
        if (结果.当前触发来源 == 枚举_任务管理触发来源::未定义) {
            结果.当前触发来源 = 枚举_任务管理触发来源::自然运行态;
        }
        if (结果.当前影子验证状态.empty()) {
            结果.当前影子验证状态 = "未触发";
        }
        私有_P0_刷新主体桥接字段(结果);
    }

    std::string 私有_P0_抽象因果摘要(因果模板节点类* 模板节点) noexcept
    {
        (void)模板节点;
        return {};
    }

    bool 私有_形成治理特征面(结构_任务管理结果& 结果) noexcept
    {
        if (!结果.已初始化分身继承面) {
            return false;
        }

        auto* 宿主任务 = 私有_P0_当前宿主任务(结果);
        auto* 当前方法 = 私有_P0_当前方法(结果);
        auto* 当前步骤 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果 = 私有_P0_最新结果节点(结果);

        结果.当前分身.宿主任务 = 宿主任务;
        结果.当前分身.当前管理方法 = 当前方法;
        结果.当前分身.当前步骤 = 当前步骤;
        结果.当前分身.最近结果 = 最新结果;
        if (结果.当前分身.本轮输入条件主键.empty()) {
            结果.当前分身.本轮输入条件主键 = 私有_主体化输入条件主键(结果.上下文, 结果.当前分身);
        }
        if (结果.当前分身.本轮结果主键.empty()) {
            结果.当前分身.本轮结果主键 = 私有_主体化结果主键(结果.当前分身, 结果.上下文);
        }

        结果.当前需求位特征.当前功能域 = 私有_P0_当前功能域(结果);
        结果.当前需求位特征.当前缺口类型 = 私有_P0_当前缺口类型(结果);
        结果.当前需求位特征.当前下一步去向 = 私有_P0_当前下一步去向(结果);
        结果.当前需求位特征.当前步骤位类型 = 私有_P0_当前步骤位类型(结果);
        结果.当前需求位特征.当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        结果.当前需求位特征.当前步骤可复用 = 结果.上下文.当前步骤可复用;
        结果.当前需求位特征.最近结果命中当前治理 = 结果.上下文.最近结果命中当前治理;
        结果.当前需求位特征.当前动作长时间无推进 = 结果.上下文.当前动作长时间无推进;
        结果.当前需求位特征.当前动作需重试 = 结果.上下文.当前动作需重试;
        结果.当前需求位特征.当前需等待 = 结果.上下文.当前需等待;
        结果.当前需求位特征.当前需收束 = 结果.上下文.当前需收束;
        结果.当前需求位特征.可继续推进 = 结果.上下文.可继续推进;
        结果.当前需求位特征.当前可回接最近结果 = 结果.上下文.当前可回接最近结果;
        结果.当前需求位特征.稳定等待态复盘学习 = 结果.上下文.稳定等待态复盘学习;
        结果.当前需求位特征.业务子任务已完成 = 结果.上下文.业务子任务已完成;
        结果.当前需求位特征.业务子任务应转执行 = 结果.上下文.业务子任务应转执行;
        结果.当前需求位特征.已触发学习承接 = 私有_P0_已触发学习承接(结果);
        结果.当前需求位特征.当前学习承接类型 = 私有_P0_当前学习承接类型(结果);
        结果.当前需求位特征.学习承接摘要.clear();
        if (结果.当前需求位特征.输入条件主键.empty()) {
            结果.当前需求位特征.输入条件主键 = 结果.当前分身.本轮输入条件主键;
        }
        if (结果.当前需求位特征.结果主键.empty()) {
            结果.当前需求位特征.结果主键 = 结果.当前分身.本轮结果主键;
        }
        if (结果.最新目标状态) {
            结果.当前需求位特征.最新目标状态 = 结果.最新目标状态;
        }
        if (结果.最新状态) {
            结果.当前需求位特征.最新结果状态 = 结果.最新状态;
        }
        if (结果.最新动态) {
            结果.当前需求位特征.最近开始动态 = 结果.最新动态;
            结果.当前需求位特征.最近结束动态 = 结果.最新动态;
        }
        if (结果.最新抽象因果) {
            结果.当前需求位特征.最近抽象因果 = 结果.最新抽象因果;
        }
        if (结果.当前需求位特征.最近抽象因果) {
            const auto 抽象因果摘要 =
                私有_P0_抽象因果摘要(结果.当前需求位特征.最近抽象因果);
            if (!抽象因果摘要.empty()) {
                结果.当前需求位特征.最近抽象因果摘要 = 抽象因果摘要;
            }
        }

        私有_P0_刷新步骤与学习结构字段(结果);
        私有_P0_刷新主体桥接字段(结果);
        return true;
    }

    bool 私有_命中方法需求位(结构_任务管理结果& 结果) noexcept
    {
        if (!结果.已初始化分身继承面) {
            return false;
        }

        const auto 已有输入条件主键 = 结果.当前需求位特征.输入条件主键;
        const auto 已有结果主键 = 结果.当前需求位特征.结果主键;
        const auto 已有抽象因果摘要 = 结果.当前需求位特征.最近抽象因果摘要;
        const auto 已有步骤位类型 = 结果.当前需求位特征.当前步骤位类型;
        const auto 已有步骤语义类型 = 结果.当前需求位特征.当前步骤语义类型;
        const bool 已有当前步骤可复用 = 结果.当前需求位特征.当前步骤可复用;
        const bool 已有最近结果命中当前治理 = 结果.当前需求位特征.最近结果命中当前治理;
        const bool 已有当前动作长时间无推进 = 结果.当前需求位特征.当前动作长时间无推进;
        const bool 已有当前动作需重试 = 结果.当前需求位特征.当前动作需重试;
        const bool 已有当前需等待 = 结果.当前需求位特征.当前需等待;
        const bool 已有当前需收束 = 结果.当前需求位特征.当前需收束;
        const bool 已有可继续推进 = 结果.当前需求位特征.可继续推进;
        const bool 已有当前可回接最近结果 = 结果.当前需求位特征.当前可回接最近结果;
        const bool 已有稳定等待态复盘学习 = 结果.当前需求位特征.稳定等待态复盘学习;
        const bool 已有业务子任务已完成 = 结果.当前需求位特征.业务子任务已完成;
        const bool 已有业务子任务应转执行 = 结果.当前需求位特征.业务子任务应转执行;
        const bool 已有已触发学习承接 = 结果.当前需求位特征.已触发学习承接;
        const auto 已有学习承接类型 = 结果.当前需求位特征.当前学习承接类型;
        auto* 已有目标状态 = 结果.当前需求位特征.最新目标状态;
        auto* 已有结果状态 = 结果.当前需求位特征.最新结果状态;
        auto* 已有开始动态 = 结果.当前需求位特征.最近开始动态;
        auto* 已有结束动态 = 结果.当前需求位特征.最近结束动态;
        auto* 已有方法位专项动态 = 结果.当前需求位特征.当前方法位专项动态;
        auto* 已有抽象因果 = 结果.当前需求位特征.最近抽象因果;
        const auto 已有方法位专项动态标题 = 结果.当前需求位特征.当前方法位专项动态标题;

        结构_任务管理方法需求位特征面 新特征{};
        const bool 已命中 =
            任务管理任务模块::刷新任务管理方法需求位(
                结果.上下文,
                结果.当前分身,
                &新特征);
        if (!已命中) {
            return false;
        }

        if (新特征.输入条件主键.empty()) {
            新特征.输入条件主键 = 已有输入条件主键;
        }
        if (新特征.结果主键.empty()) {
            新特征.结果主键 = 已有结果主键;
        }
        if (新特征.最近抽象因果摘要.empty()) {
            新特征.最近抽象因果摘要 = 已有抽象因果摘要;
        }
        if (新特征.当前步骤位类型 == 枚举_任务管理步骤位类型::未定义) {
            新特征.当前步骤位类型 = 已有步骤位类型;
        }
        if (新特征.当前步骤语义类型 == 枚举_任务管理步骤语义类型::未定义) {
            新特征.当前步骤语义类型 = 已有步骤语义类型;
        }
        if (!新特征.当前步骤可复用 && 已有当前步骤可复用) {
            新特征.当前步骤可复用 = true;
        }
        if (!新特征.最近结果命中当前治理 && 已有最近结果命中当前治理) {
            新特征.最近结果命中当前治理 = true;
        }
        if (!新特征.当前动作长时间无推进 && 已有当前动作长时间无推进) {
            新特征.当前动作长时间无推进 = true;
        }
        if (!新特征.当前动作需重试 && 已有当前动作需重试) {
            新特征.当前动作需重试 = true;
        }
        if (!新特征.当前需等待 && 已有当前需等待) {
            新特征.当前需等待 = true;
        }
        if (!新特征.当前需收束 && 已有当前需收束) {
            新特征.当前需收束 = true;
        }
        if (!新特征.可继续推进 && 已有可继续推进) {
            新特征.可继续推进 = true;
        }
        if (!新特征.当前可回接最近结果 && 已有当前可回接最近结果) {
            新特征.当前可回接最近结果 = true;
        }
        if (!新特征.稳定等待态复盘学习 && 已有稳定等待态复盘学习) {
            新特征.稳定等待态复盘学习 = true;
        }
        if (!新特征.业务子任务已完成 && 已有业务子任务已完成) {
            新特征.业务子任务已完成 = true;
        }
        if (!新特征.业务子任务应转执行 && 已有业务子任务应转执行) {
            新特征.业务子任务应转执行 = true;
        }
        if (!新特征.已触发学习承接 && 已有已触发学习承接) {
            新特征.已触发学习承接 = true;
        }
        if (新特征.当前学习承接类型 == 枚举_任务管理学习承接类型::未定义) {
            新特征.当前学习承接类型 = 已有学习承接类型;
        }
        if (!新特征.最新目标状态) {
            新特征.最新目标状态 = 已有目标状态;
        }
        if (!新特征.最新结果状态) {
            新特征.最新结果状态 = 已有结果状态;
        }
        if (!新特征.最近开始动态) {
            新特征.最近开始动态 = 已有开始动态;
        }
        if (!新特征.最近结束动态) {
            新特征.最近结束动态 = 已有结束动态;
        }
        if (!新特征.当前方法位专项动态) {
            新特征.当前方法位专项动态 = 已有方法位专项动态;
        }
        if (!新特征.最近抽象因果) {
            新特征.最近抽象因果 = 已有抽象因果;
        }
        if (新特征.最近抽象因果摘要.empty() && 新特征.最近抽象因果) {
            新特征.最近抽象因果摘要 = 私有_P0_抽象因果摘要(新特征.最近抽象因果);
        }
        if (新特征.当前方法位专项动态标题.empty()) {
            新特征.当前方法位专项动态标题 = 已有方法位专项动态标题;
        }

        结果.当前需求位特征 = std::move(新特征);
        结果.已刷新方法需求位 = true;
        结果.当前主方法需求位 = 结果.当前需求位特征.当前需求位;
        结果.当前分身.当前主方法需求位 = 结果.当前主方法需求位;
        结果.当前单步决策.当前方法需求位 = 结果.当前主方法需求位;
        结果.当前一步结果.当前方法需求位 = 结果.当前主方法需求位;
        私有_P0_刷新主体桥接字段(结果);
        return true;
    }

    bool 私有_评估本能能力缺口(自我类& 自我对象, 结构_任务管理结果& 结果) noexcept
    {
        auto* 已有最小原语动态 = 结果.当前本能能力缺口.当前最小原语动态;
        const auto 已有最小原语动态标题 = 结果.当前本能能力缺口.当前最小原语动态标题;
        结构_任务管理本能能力缺口面 当前缺口{};
        const bool 已评估 =
            任务管理任务模块::评估任务管理本能能力缺口(
                自我对象,
                结果.当前需求位特征,
                &当前缺口);
        if (!已评估) {
            return false;
        }

        if (!当前缺口.当前最小原语动态) {
            当前缺口.当前最小原语动态 = 已有最小原语动态;
        }
        if (当前缺口.当前最小原语动态标题.empty()) {
            当前缺口.当前最小原语动态标题 = 已有最小原语动态标题;
        }
        if (!当前缺口.当前最小原语动态) {
            当前缺口.当前最小原语动态 = 结果.当前分身.当前最小原语动态;
        }
        if (当前缺口.当前最小原语动态标题.empty()) {
            当前缺口.当前最小原语动态标题 = 结果.当前分身.当前最小原语动态标题;
        }
        结果.当前本能能力缺口 = std::move(当前缺口);
        结果.已评估本能能力缺口 = true;
        结果.当前本能能力缺口类型 = 结果.当前本能能力缺口.当前缺口类型;
        结果.来源最小原语位图 = 结果.当前本能能力缺口.所需原语位图;
        if (结果.来源主观察特征.empty()) {
            结果.来源主观察特征 = 结果.当前本能能力缺口.主观察特征键;
        }
        结果.当前单步决策.当前本能能力缺口类型 = 结果.当前本能能力缺口类型;
        结果.当前一步结果.当前本能能力缺口类型 = 结果.当前本能能力缺口类型;
        私有_P0_刷新主体桥接字段(结果);
        return true;
    }

    bool 私有_桥接补层裁决(结构_任务管理结果& 结果) noexcept
    {
        const auto 当前需求位 = 私有_P0_当前方法需求位(结果);
        auto 当前本能缺口 = 私有_P0_当前本能缺口(结果);

        std::ostringstream 摘要;
        摘要 << "桥接补层裁决"
            << " | 需求位=" << 私有_方法需求位文本(当前需求位)
            << " | 本能缺口=" << 私有_本能能力缺口文本(当前本能缺口)
            << " | 本轮原语命中=" << (结果.来源最小原语位图 == 0 ? "空" : 私有_原语位图摘要(结果.来源最小原语位图))
            << " | 正式已具备=" << (结果.当前本能能力缺口.原语账本摘要.empty() ? "空" : 结果.当前本能能力缺口.原语账本摘要)
            << " | 派生摘要=" << (结果.当前本能能力缺口.派生规则摘要.empty() ? "空" : 结果.当前本能能力缺口.派生规则摘要);

        if (私有_M10_主体长期策略允许直放后段(结果)) {
            结果.当前本能能力缺口.当前缺口类型 = 枚举_任务管理本能能力缺口类型::无缺口;
            结果.当前本能能力缺口.需要学习补齐 = false;
            结果.当前本能能力缺口.允许直接执行 = true;
            结果.当前本能能力缺口类型 = 枚举_任务管理本能能力缺口类型::无缺口;
            结果.当前单步决策.当前本能能力缺口类型 = 结果.当前本能能力缺口类型;
            结果.当前一步结果.当前本能能力缺口类型 = 结果.当前本能能力缺口类型;
            结果.当前写回结果.结构动作摘要 =
                摘要.str() + " | 结论=主体长期策略正式资产已验证，宿主存在目标差额，放行后段";
            私有_P0_刷新主体桥接字段(结果);
            return true;
        }

        if (当前本能缺口 == 枚举_任务管理本能能力缺口类型::无缺口) {
            结果.当前写回结果.结构动作摘要 = 摘要.str() + " | 结论=放行后段";
            私有_P0_刷新主体桥接字段(结果);
            return true;
        }

        if (当前本能缺口 == 枚举_任务管理本能能力缺口类型::缺挂点原语) {
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::筹办,
                枚举_任务管理缺口类型::方法挂点缺失,
                枚举_任务管理下一步去向::绑定方法,
                枚举_任务管理治理状态迁移::进入挂起,
                枚举_任务管理总控结果::宿主已挂起,
                枚举_任务管理反馈类型::重筹办提示,
                摘要.str() + " | 结论=退回补结构/绑定方法");
        }
        else if (结果.当前本能能力缺口.需要退回补结构) {
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::筹办,
                枚举_任务管理缺口类型::任务待启动,
                枚举_任务管理下一步去向::回到重筹办,
                枚举_任务管理治理状态迁移::回到重筹办,
                枚举_任务管理总控结果::宿主待重筹办,
                枚举_任务管理反馈类型::重筹办提示,
                摘要.str() + " | 结论=退回补结构");
        }
        else if (结果.当前本能能力缺口.需要学习补齐
            || 当前本能缺口 == 枚举_任务管理本能能力缺口类型::需学习补齐) {
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::等待,
                枚举_任务管理缺口类型::任务运行中,
                枚举_任务管理下一步去向::保持等待,
                枚举_任务管理治理状态迁移::保持等待,
                枚举_任务管理总控结果::宿主保持等待,
                枚举_任务管理反馈类型::等待维持,
                摘要.str() + " | 结论=建议学习补齐，本轮只输出显式建议");
        }
        else {
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::等待,
                枚举_任务管理缺口类型::任务运行中,
                枚举_任务管理下一步去向::保持等待,
                枚举_任务管理治理状态迁移::保持等待,
                枚举_任务管理总控结果::宿主保持等待,
                枚举_任务管理反馈类型::等待维持,
                摘要.str() + " | 结论=等待原语/派生证据补齐");
        }

        结果.当前写回结果.结构动作摘要 = 私有_P0_最近反馈摘要(结果);
        私有_P0_刷新主体桥接字段(结果);
        return false;
    }

    std::string 私有_任务管理方法位标题(const 结构_任务管理结果& 结果) noexcept
    {
        switch (私有_P0_当前方法需求位(结果)) {
        case 枚举_任务管理方法需求位::收束复盘:
            return "管理_复盘器";
        case 枚举_任务管理方法需求位::失败策略:
            return "管理_失败策略器";
        case 枚举_任务管理方法需求位::门控转域:
            return "管理_门控器";
        case 枚举_任务管理方法需求位::分解补条件:
            return "管理_分解器";
        case 枚举_任务管理方法需求位::求证判定:
            return "管理_求证器";
        case 枚举_任务管理方法需求位::补结构:
            return "管理_筹办器";
        case 枚举_任务管理方法需求位::选择推进:
            return "管理_选择器";
        default:
            break;
        }

        const auto 当前功能域 = 私有_P0_当前功能域(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前步骤位类型 = 私有_P0_当前步骤位类型(结果);
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);

        if (当前功能域 == 枚举_任务管理功能域::收束
            || 当前下一步去向 == 枚举_任务管理下一步去向::进入收束) {
            return "管理_复盘器";
        }
        if (当前功能域 == 枚举_任务管理功能域::纠偏) {
            return "管理_失败策略器";
        }
        if (结果.最近执行前门控结果 != 枚举_任务管理执行前门控结果::允许普通执行
            || 当前功能域 == 枚举_任务管理功能域::等待) {
            return "管理_门控器";
        }
        if (结果.已创建补条件子任务
            || 结果.已复用补条件子任务
            || 结果.已创建业务子任务
            || 结果.已复用业务子任务
            || 当前步骤位类型 == 枚举_任务管理步骤位类型::补条件位) {
            return "管理_分解器";
        }
        if (结果.宿主目标结果已对齐
            || 结果.宿主目标结果待推进
            || 结果.当前宿主目标结果比较 != 枚举_任务管理双面比较结果::未定义
            || 结果.当前步骤目标结果比较 != 枚举_任务管理双面比较结果::未定义
            || 结果.当前结果目标结果比较 != 枚举_任务管理双面比较结果::未定义) {
            return "管理_求证器";
        }
        if (当前功能域 == 枚举_任务管理功能域::筹办
            || 当前缺口类型 == 枚举_任务管理缺口类型::治理任务缺失
            || 当前缺口类型 == 枚举_任务管理缺口类型::宿主任务缺失
            || 当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失) {
            return "管理_筹办器";
        }
        return "管理_选择器";
    }

    void 私有_设定治理结论(
        结构_任务管理结果& 结果,
        枚举_任务管理功能域 功能域,
        枚举_任务管理缺口类型 缺口类型,
        枚举_任务管理下一步去向 下一步去向,
        枚举_任务管理治理状态迁移 状态迁移,
        枚举_任务管理总控结果 总控结果,
        枚举_任务管理反馈类型 反馈类型,
        std::string 反馈摘要) noexcept
    {
        结构_任务管理单步决策 当前决策{};
        当前决策.当前功能域 = 功能域;
        当前决策.当前缺口类型 = 缺口类型;
        当前决策.当前下一步去向 = 下一步去向;
        当前决策.当前治理状态迁移 = 状态迁移;
        当前决策.决策摘要.clear();
        私有_P0_覆盖决策字段(结果.当前单步决策, 当前决策);

        结构_任务管理一步结果 当前一步结果{};
        当前一步结果.当前功能域 = 功能域;
        当前一步结果.当前缺口类型 = 缺口类型;
        当前一步结果.当前下一步去向 = 下一步去向;
        当前一步结果.当前治理状态迁移 = 状态迁移;
        当前一步结果.最近总控结果 = 总控结果;
        当前一步结果.最近反馈类型 = 反馈类型;
        当前一步结果.最近反馈摘要.clear();
        私有_P0_覆盖一步结果字段(结果.当前一步结果, 当前一步结果);

        结果.当前功能域 = 功能域;
        结果.当前缺口类型 = 缺口类型;
        结果.当前下一步去向 = 下一步去向;
        结果.当前治理状态迁移 = 状态迁移;
        结果.最近功能域 = 功能域;
        结果.最近总控结果 = 总控结果;
        结果.最近反馈类型 = 反馈类型;
        结果.最近反馈摘要.clear();
    }

    void 私有_同步治理上下文镜像(结构_任务管理结果& 结果) noexcept
    {
        私有_P0_刷新主体桥接字段(结果);
        auto 阶段上下文 = 结果.上下文;
        私有_P0_覆盖请求到上下文字段(阶段上下文, 结果.当前请求);
        私有_P0_覆盖兼容到上下文字段(阶段上下文, 结果);
        私有_P0_覆盖决策到上下文字段(阶段上下文, 结果.当前单步决策);
        私有_P0_覆盖写回到上下文字段(阶段上下文, 结果.当前写回结果);
        私有_P0_覆盖一步结果到上下文字段(阶段上下文, 结果.当前一步结果);
        阶段上下文.最近反馈摘要.clear();
        私有_按宿主清洗治理上下文镜像(阶段上下文);
        结果.上下文 = std::move(阶段上下文);
    }

    void 私有_完成治理轮收口(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        const bool 调试M10回填 = 调用点.find("M10长期策略驱动回填") != std::string::npos;
        if (调试M10回填) {
            std::ostringstream 收口入口摘要;
            收口入口摘要 << "任务管理/M10回填 | 收口-进入函数"
                << " | 宿主指针=" << static_cast<const void*>(宿主任务)
                << " | 上下文宿主指针=" << static_cast<const void*>(结果.上下文.当前宿主任务)
                << " | 当前步骤指针=" << static_cast<const void*>(结果.当前步骤节点)
                << " | 最近结果指针=" << static_cast<const void*>(结果.最新结果节点)
                << " | 当前方法指针=" << static_cast<const void*>(结果.上下文.当前方法);
            项目运行日志(收口入口摘要.str());
        }
        结果.宿主任务新状态 = 宿主任务 ? 宿主任务->主信息.状态 : 枚举_任务状态::未定义;
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 收口-宿主状态读取完成");
        }
        if (!结果.当前步骤节点) {
            结果.当前步骤节点 = 结果.上下文.当前步骤;
        }
        if (!结果.最新结果节点) {
            结果.最新结果节点 = 结果.上下文.最近结果;
        }

        私有_P0_覆盖非空指针(结果.当前写回结果.当前步骤节点, 私有_P0_当前步骤节点(结果));
        私有_P0_覆盖非空指针(结果.当前写回结果.最新结果节点, 私有_P0_最新结果节点(结果));
        私有_P0_覆盖非空指针(结果.当前一步结果.当前方法节点, 私有_P0_当前方法(结果));
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 收口-步骤结果方法指针整理完成");
        }

        if (结果.已初始化分身继承面) {
            结果.当前分身.最近推进时间 = 当前时间;
            结果.当前分身.最近回并时间 = 当前时间;
            结果.当前分身.回并序号 = 结果.当前主体.最近回并序号 + 1;
            结果.当前分身.宿主任务 = 宿主任务;
            结果.当前分身.当前步骤 = 私有_P0_当前步骤节点(结果);
            结果.当前分身.最近结果 = 私有_P0_最新结果节点(结果);
            结果.当前分身.当前管理方法 = 私有_P0_当前方法(结果);
            结果.当前分身.当前主方法需求位 = 私有_P0_当前方法需求位(结果);
            结果.当前分身.当前本能能力缺口类型 = 私有_P0_当前本能缺口(结果);
            结果.当前分身.当前治理态型 = 私有_治理态型文本(结果);
            结果.当前分身.本轮输入条件主键 = 私有_主体化输入条件主键(结果.上下文, 结果.当前分身);
            结果.当前分身.本轮结果主键 = 私有_主体化结果主键(结果.当前分身, 结果.上下文);
            结果.当前分身.本地推进次数 = 结果.已推进任务状态 ? 1U : 0U;
            结果.当前分身.本地完成次数 =
                结果.宿主任务新状态 == 枚举_任务状态::完成 ? 1U : 0U;
            结果.当前分身.本地失败次数 =
                (结果.宿主任务新状态 == 枚举_任务状态::失败
                    || 结果.宿主任务新状态 == 枚举_任务状态::取消)
                ? 1U
                : 0U;
            结果.当前分身.本地弱出生证据次数 = 结果.已创建学习方法头 ? 1U : 0U;
            结果.当前分身.本地验证证据次数 = 结果.已生成治理证据 ? 1U : 0U;
            结果.当前分身.本地升格证据次数 = 结果.已创建学习方法结果节点 ? 1U : 0U;
            结果.当前分身.本地价值证据次数 = 结果.已写回目标状态 ? 1U : 0U;
            结果.当前分身.当前状态 = 枚举_任务管理分身状态::等待回并;
            if (调试M10回填) {
                项目运行日志("任务管理/M10回填 | 收口-准备刷新分身继承位图");
            }
            私有_刷新分身继承位图(结果.当前分身, 结果.上下文);
            if (调试M10回填) {
                项目运行日志("任务管理/M10回填 | 收口-分身继承位图刷新完成");
                项目运行日志("任务管理/M10回填 | 收口-准备形成治理特征面");
            }

            (void)私有_形成治理特征面(结果);
            if (调试M10回填) {
                项目运行日志("任务管理/M10回填 | 收口-治理特征面形成完成");
                项目运行日志("任务管理/M10回填 | 收口-准备命中方法需求位");
            }
            (void)私有_命中方法需求位(结果);
            if (调试M10回填) {
                项目运行日志("任务管理/M10回填 | 收口-方法需求位命中完成");
                项目运行日志("任务管理/M10回填 | 收口-准备收口触发信息");
            }
            私有_收口触发信息(结果);
            if (调试M10回填) {
                项目运行日志("任务管理/M10回填 | 收口-触发信息收口完成");
                项目运行日志("任务管理/M10回填 | 收口-准备评估本能能力缺口");
            }
            (void)私有_评估本能能力缺口(自我对象, 结果);
            if (调试M10回填) {
                项目运行日志("任务管理/M10回填 | 收口-本能能力缺口评估完成");
            }
        }
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 收口-分身继承面处理完成");
        }

        if (结果.已确保任务管理主体虚拟存在) {
            结果.当前主体.最近功能域 = 私有_P0_当前功能域(结果);
            结果.当前主体.最近主缺口类型 = 私有_P0_当前缺口类型(结果);
            结果.当前主体.最近下一步去向 = 私有_P0_当前下一步去向(结果);
            结果.当前主体.最近总控结果 = 私有_P0_最近总控结果(结果);
            结果.当前主体.最近主方法需求位 = 私有_P0_当前方法需求位(结果);
            私有_P0_覆盖非默认值(
                结果.当前主体.最近本能能力缺口类型,
                私有_P0_当前本能缺口(结果),
                枚举_任务管理本能能力缺口类型::未定义);
            私有_P0_覆盖非空文本(结果.当前主体.最近治理态型, 私有_治理态型文本(结果));
            结果.当前主体.最近输入条件主键 = 结果.当前分身.本轮输入条件主键;
            结果.当前主体.最近结果主键 = 结果.当前分身.本轮结果主键;
            结果.当前主体.最近原因摘要 = 私有_P0_最近反馈摘要(结果);
        }
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 收口-主体最近字段刷新完成");
        }

        if (结果.已初始化分身继承面 && 结果.已确保任务管理主体虚拟存在) {
            结果.已回并分身增量 =
                任务管理任务模块::回并分身增量到任务管理主体(
                    结果.当前分身,
                    &结果.当前主体);
            if (结果.已回并分身增量) {
                std::ostringstream 主体回并摘要;
                主体回并摘要 << "主体ID=" << 结果.当前主体.主体ID
                    << " | 分身ID=" << 结果.当前分身.分身ID
                    << " | 需求位=" << 私有_方法需求位文本(结果.当前分身.当前主方法需求位)
                    << " | 输入条件=" << (结果.当前分身.本轮输入条件主键.empty() ? "空" : 结果.当前分身.本轮输入条件主键);
                结果.当前写回结果.主体回并摘要 = 主体回并摘要.str();
                结果.当前一步结果.主体回并摘要 = 结果.当前写回结果.主体回并摘要;
            }
        }
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 收口-分身增量回并完成");
            项目运行日志("任务管理/M10回填 | 收口-准备回写长期策略可靠度");
        }

        私有_M10_回写长期策略可靠度(宿主任务, 当前时间, 结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 收口-长期策略可靠度回写完成");
            项目运行日志("任务管理/M10回填 | 收口-准备刷新主体桥接字段");
        }
        私有_P0_刷新主体桥接字段(结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 收口-主体桥接字段刷新完成");
            项目运行日志("任务管理/M10回填 | 收口-准备同步治理上下文镜像");
        }
        私有_同步治理上下文镜像(结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 收口-治理上下文镜像同步完成");
            项目运行日志("任务管理/M10回填 | 收口-准备记录方法位专项动态");
        }

        (void)私有_记录任务管理方法位专项动态(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/方法位动态/开始",
            结果,
            枚举_动作事件相位::开始运行);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 收口-方法位专项动态记录完成");
            项目运行日志("任务管理/M10回填 | 收口-准备写回治理稳定字段");
        }

        私有_写回治理稳定字段(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/证据/稳定字段",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 收口-治理稳定字段写回完成");
        }

        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前步骤指针(),
            私有_P0_当前步骤节点(结果),
            私有_任务标题(私有_P0_当前步骤节点(结果)),
            "任务管理_当前步骤推进",
            私有_P0_当前方法(结果),
            宿主任务,
            当前时间,
            调用点 + "/证据/当前步骤",
            结果);

        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近结果指针(),
            私有_P0_最新结果节点(结果),
            私有_任务标题(私有_P0_最新结果节点(结果)),
            "任务管理_最近结果推进",
            私有_P0_当前方法(结果),
            宿主任务,
            当前时间,
            调用点 + "/证据/最近结果",
            结果);

        (void)私有_记录任务管理方法位专项动态(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/方法位动态/完成",
            结果,
            枚举_动作事件相位::完成);
    }

    bool 私有_应用上位输入约束(结构_任务管理结果& 结果) noexcept
    {
        结果.最近根层重判结果 = 结果.上下文.最近根层重判结果;
        结果.最近执行前门控结果 = 结果.上下文.最近执行前门控结果;
        结果.最近上层反馈摘要.clear();

        const auto 根层结果 = 结果.上下文.最近根层重判结果;
        const auto 门控结果 = 结果.上下文.最近执行前门控结果;

        if (门控结果 == 枚举_任务管理执行前门控结果::禁止继续
            || 根层结果 == 枚举_任务管理根层重判结果::停止退出边界) {
            结果.已受上位输入约束 = true;
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::收束,
                枚举_任务管理缺口类型::上位停止约束,
                枚举_任务管理下一步去向::进入收束,
                枚举_任务管理治理状态迁移::进入收束,
                枚举_任务管理总控结果::宿主进入收束,
                枚举_任务管理反馈类型::收束回执,
                "上位输入命中停止退出边界，任务治理只做收口");
            return true;
        }

        if (门控结果 == 枚举_任务管理执行前门控结果::仅允许收束
            || 根层结果 == 枚举_任务管理根层重判结果::收束优先) {
            结果.已受上位输入约束 = true;
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::收束,
                枚举_任务管理缺口类型::上位收束约束,
                枚举_任务管理下一步去向::进入收束,
                枚举_任务管理治理状态迁移::进入收束,
                枚举_任务管理总控结果::宿主进入收束,
                枚举_任务管理反馈类型::收束回执,
                "上位输入要求优先收束，本轮不继续普通推进");
            return true;
        }

        if (门控结果 == 枚举_任务管理执行前门控结果::仅允许等待
            || 根层结果 == 枚举_任务管理根层重判结果::待机保持) {
            结果.已受上位输入约束 = true;
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::等待,
                枚举_任务管理缺口类型::上位等待约束,
                枚举_任务管理下一步去向::保持等待,
                枚举_任务管理治理状态迁移::保持等待,
                枚举_任务管理总控结果::宿主保持等待,
                枚举_任务管理反馈类型::等待维持,
                "上位输入要求保持等待，本轮不进入普通执行链");
            return true;
        }

        return false;
    }

    bool 私有_上下文允许普通执行前筹办(const 结构_任务管理上下文& 上下文) noexcept
    {
        const auto 根层结果 = 上下文.最近根层重判结果;
        const auto 门控结果 = 上下文.最近执行前门控结果;
        return 门控结果 != 枚举_任务管理执行前门控结果::禁止继续
            && 门控结果 != 枚举_任务管理执行前门控结果::仅允许收束
            && 门控结果 != 枚举_任务管理执行前门控结果::仅允许等待
            && 根层结果 != 枚举_任务管理根层重判结果::停止退出边界
            && 根层结果 != 枚举_任务管理根层重判结果::收束优先
            && 根层结果 != 枚举_任务管理根层重判结果::待机保持;
    }

    任务节点* 私有_读取任务节点指针特征(
        const 自我类& 自我对象,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!特征类型) {
            return nullptr;
        }
        if (!自我对象.读取自我指针特征当前值(特征类型, 指针值, 调用点)) {
            return nullptr;
        }
        return reinterpret_cast<任务节点*>(指针值);
    }

    template<class T枚举>
    T枚举 私有_读取I64枚举特征(
        const 自我类& 自我对象,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        I64 当前值 = 0;
        if (!特征类型 || !自我对象.读取自我I64特征当前值(特征类型, 当前值, 调用点)) {
            return T枚举::未定义;
        }
        return static_cast<T枚举>(当前值);
    }

    std::string 私有_读取反馈摘要特征(
        const 自我类& 自我对象,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!特征类型 || !自我对象.读取自我指针特征当前值(特征类型, 指针值, 调用点)) {
            return {};
        }

        return 私有_安全词(reinterpret_cast<const 词性节点类*>(指针值));
    }

    std::uint64_t 私有_读取U64文本特征(
        const 自我类& 自我对象,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        return 私有_安全解析U64(
            私有_读取反馈摘要特征(自我对象, 特征类型, 调用点));
    }

    bool 私有_读取指针特征(const 特征节点类* 特征, std::uintptr_t& 输出值) noexcept
    {
        if (!特征) return false;
        const auto 快照 = 世界树.读取特征快照(特征);
        const auto* 指针 = std::get_if<指针句柄>(&快照);
        if (!指针) return false;
        输出值 = 指针->指针;
        return true;
    }

    bool 私有_读取I64特征(const 特征节点类* 特征, I64& 输出值) noexcept
    {
        return 特征 && 世界树.读取特征_I64(特征, 输出值);
    }

    bool 私有_读取宿主任务治理I64特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64& 输出值,
        const std::string& 调用点) noexcept
    {
        if (!特征类型) {
            return false;
        }
        if (const auto* 特征节点 = 私有_查找宿主任务治理特征节点(宿主任务, 特征类型);
            私有_读取I64特征(特征节点, 输出值)) {
            return true;
        }
        (void)自我对象;
        (void)调用点;
        return false;
    }

    bool 私有_读取宿主任务治理指针特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        std::uintptr_t& 输出值,
        const std::string& 调用点) noexcept
    {
        if (!特征类型) {
            return false;
        }
        if (const auto* 特征节点 = 私有_查找宿主任务治理特征节点(宿主任务, 特征类型);
            私有_读取指针特征(特征节点, 输出值)) {
            return true;
        }
        (void)自我对象;
        (void)调用点;
        return false;
    }

    任务节点* 私有_读取宿主任务治理任务节点指针特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!私有_读取宿主任务治理指针特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            指针值,
            调用点)) {
            return nullptr;
        }
        return reinterpret_cast<任务节点*>(指针值);
    }

    方法节点* 私有_读取宿主任务治理方法节点指针特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!私有_读取宿主任务治理指针特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            指针值,
            调用点)) {
            return nullptr;
        }
        return reinterpret_cast<方法节点*>(指针值);
    }

    template<class T枚举>
    T枚举 私有_读取宿主任务治理I64枚举特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        I64 当前值 = 0;
        if (!私有_读取宿主任务治理I64特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            当前值,
            调用点)) {
            return T枚举::未定义;
        }
        return static_cast<T枚举>(当前值);
    }

    std::string 私有_读取宿主任务治理反馈摘要特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!私有_读取宿主任务治理指针特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            指针值,
            调用点)) {
            return {};
        }
        return 私有_安全词(reinterpret_cast<const 词性节点类*>(指针值));
    }

    bool 私有_读取主体桥接I64特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64& 输出值,
        const std::string& 调用点) noexcept
    {
        if (!特征类型) {
            return false;
        }
        if (const auto* 特征节点 = 私有_查找主体桥接特征节点(宿主任务, 特征类型);
            私有_读取I64特征(特征节点, 输出值)) {
            return true;
        }
        (void)自我对象;
        (void)调用点;
        return false;
    }

    bool 私有_读取主体桥接指针特征当前值(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        std::uintptr_t& 输出值,
        const std::string& 调用点) noexcept
    {
        if (!特征类型) {
            return false;
        }
        if (const auto* 特征节点 = 私有_查找主体桥接特征节点(宿主任务, 特征类型);
            私有_读取指针特征(特征节点, 输出值)) {
            return true;
        }
        (void)自我对象;
        (void)调用点;
        return false;
    }

    template<class T枚举>
    T枚举 私有_读取主体桥接I64枚举特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        I64 当前值 = 0;
        if (!私有_读取主体桥接I64特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            当前值,
            调用点)) {
            return T枚举::未定义;
        }
        return static_cast<T枚举>(当前值);
    }

    std::string 私有_读取主体桥接反馈摘要特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        std::uintptr_t 指针值 = 0;
        if (!私有_读取主体桥接指针特征当前值(
            自我对象,
            宿主任务,
            特征类型,
            指针值,
            调用点)) {
            return {};
        }
        return 私有_安全词(reinterpret_cast<const 词性节点类*>(指针值));
    }

    std::uint64_t 私有_读取主体桥接U64文本特征(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        return 私有_安全解析U64(
            私有_读取主体桥接反馈摘要特征(
                自我对象,
                宿主任务,
                特征类型,
                调用点));
    }

    bool 私有_任务引用匹配(
        const 任务节点* 宿主任务,
        const 可解析引用<任务节点类>& 宿主引用) noexcept
    {
        if (!宿主任务) {
            return false;
        }
        if (宿主引用.指针 == reinterpret_cast<const 任务节点类*>(宿主任务)) {
            return true;
        }
        return !宿主引用.主键.empty()
            && 宿主引用.主键 == 宿主任务->获取主键();
    }

    bool 私有_桥接上下文处于恢复稀疏态(const 结构_任务管理上下文& 上下文) noexcept
    {
        return !上下文.当前方法
            && !上下文.当前步骤
            && !上下文.最近结果;
    }

    void 私有_从最近治理结果补齐主体桥接字段(结构_任务管理上下文& 上下文) noexcept
    {
        if (!上下文.当前宿主任务) {
            return;
        }

        结构_任务管理结果 最近治理结果{};
        if (!任务管理任务模块::读取最近治理结果(&最近治理结果)
            || 最近治理结果.上下文.当前宿主任务 != 上下文.当前宿主任务) {
            return;
        }

        const auto 最近主体ID = 最近治理结果.当前主体ID != 0
            ? 最近治理结果.当前主体ID
            : 最近治理结果.上下文.当前主体ID;
        if (上下文.当前主体ID == 0 && 最近主体ID != 0) {
            上下文.当前主体ID = 最近主体ID;
        }

        const auto* 最近方法 = 私有_P0_当前方法(最近治理结果);
        if (上下文.当前方法 && 最近方法 && 最近方法 != 上下文.当前方法) {
            return;
        }

        const auto 最近分身ID = 最近治理结果.当前分身ID != 0
            ? 最近治理结果.当前分身ID
            : 最近治理结果.上下文.当前分身ID;
        if (上下文.当前分身ID == 0 && 最近分身ID != 0) {
            上下文.当前分身ID = 最近分身ID;
        }

        const auto 最近方法需求位 = 私有_P0_当前方法需求位(最近治理结果);
        if (上下文.当前方法需求位 == 枚举_任务管理方法需求位::未定义
            && 最近方法需求位 != 枚举_任务管理方法需求位::未定义) {
            上下文.当前方法需求位 = 最近方法需求位;
        }
        const auto 最近步骤位类型 = 私有_P0_当前步骤位类型(最近治理结果);
        if (上下文.当前步骤位类型 == 枚举_任务管理步骤位类型::未定义
            && 最近步骤位类型 != 枚举_任务管理步骤位类型::未定义) {
            上下文.当前步骤位类型 = 最近步骤位类型;
        }
        const auto 最近步骤语义类型 = 私有_P0_当前步骤语义类型(最近治理结果);
        if (上下文.当前步骤语义类型 == 枚举_任务管理步骤语义类型::未定义
            && 最近步骤语义类型 != 枚举_任务管理步骤语义类型::未定义) {
            上下文.当前步骤语义类型 = 最近步骤语义类型;
        }
        if (!上下文.已触发学习承接 && 私有_P0_已触发学习承接(最近治理结果)) {
            上下文.已触发学习承接 = true;
        }
        const auto 最近学习承接类型 = 私有_P0_当前学习承接类型(最近治理结果);
        if (上下文.当前学习承接类型 == 枚举_任务管理学习承接类型::未定义
            && 最近学习承接类型 != 枚举_任务管理学习承接类型::未定义) {
            上下文.当前学习承接类型 = 最近学习承接类型;
        }
        const auto 最近本能缺口 = 私有_P0_当前本能缺口(最近治理结果);
        if (上下文.当前本能能力缺口类型 == 枚举_任务管理本能能力缺口类型::未定义
            && 最近本能缺口 != 枚举_任务管理本能能力缺口类型::未定义) {
            上下文.当前本能能力缺口类型 = 最近本能缺口;
        }
        if (!上下文.当前方法位专项动态 && 最近治理结果.当前需求位特征.当前方法位专项动态) {
            上下文.当前方法位专项动态 = 最近治理结果.当前需求位特征.当前方法位专项动态;
        }
        if (上下文.当前方法位专项动态标题.empty()) {
            上下文.当前方法位专项动态标题 = 私有_P0_选用非空文本(
                最近治理结果.当前需求位特征.当前方法位专项动态标题,
                最近治理结果.上下文.当前方法位专项动态标题,
                std::string{});
        }
        if (!上下文.当前最小原语动态 && 最近治理结果.当前本能能力缺口.当前最小原语动态) {
            上下文.当前最小原语动态 = 最近治理结果.当前本能能力缺口.当前最小原语动态;
        }
        if (上下文.当前最小原语动态标题.empty()) {
            上下文.当前最小原语动态标题 = 私有_P0_选用非空文本(
                最近治理结果.当前本能能力缺口.当前最小原语动态标题,
                最近治理结果.上下文.当前最小原语动态标题,
                std::string{});
        }

        if (上下文.当前输入条件主键.empty()) {
            上下文.当前输入条件主键 = 私有_P0_选用非空文本(
                最近治理结果.当前一步结果.当前输入条件主键,
                最近治理结果.上下文.当前输入条件主键,
                私有_P0_选用非空文本(
                    最近治理结果.当前分身.本轮输入条件主键,
                    最近治理结果.当前需求位特征.输入条件主键,
                    std::string{}));
        }

        if (上下文.当前治理态型.empty()) {
            上下文.当前治理态型 = 私有_P0_选用非空文本(
                最近治理结果.当前一步结果.当前治理态型,
                最近治理结果.上下文.当前治理态型,
                std::string{});
        }
    }

    void 私有_从治理恢复快照补齐主体桥接字段(结构_任务管理上下文& 上下文) noexcept
    {
        if (!上下文.当前宿主任务 || !私有_桥接上下文处于恢复稀疏态(上下文)) {
            return;
        }

        结构_治理恢复快照 恢复快照{};
        if (!任务管理任务模块::读取治理恢复快照(&恢复快照)
            || !私有_任务引用匹配(上下文.当前宿主任务, 恢复快照.宿主任务)) {
            return;
        }

        if (上下文.当前主体ID == 0 && 恢复快照.当前主体ID != 0) {
            上下文.当前主体ID = 恢复快照.当前主体ID;
        }
        if (上下文.当前分身ID == 0 && 恢复快照.当前分身ID != 0) {
            上下文.当前分身ID = 恢复快照.当前分身ID;
        }
        if (上下文.当前方法需求位 == 枚举_任务管理方法需求位::未定义
            && 恢复快照.最近方法需求位 != 枚举_任务管理方法需求位::未定义) {
            上下文.当前方法需求位 = 恢复快照.最近方法需求位;
        }
        if (上下文.当前本能能力缺口类型 == 枚举_任务管理本能能力缺口类型::未定义
            && 恢复快照.最近本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义) {
            上下文.当前本能能力缺口类型 = 恢复快照.最近本能能力缺口类型;
        }
        if (上下文.当前输入条件主键.empty()) {
            上下文.当前输入条件主键 = 恢复快照.最近条件主键;
        }
        if (上下文.当前治理态型.empty()) {
            上下文.当前治理态型 = 恢复快照.最近治理态型;
        }
        if (上下文.当前方法位专项动态标题.empty()) {
            上下文.当前方法位专项动态标题 = 恢复快照.最近方法位专项动态标题;
        }
        if (上下文.当前最小原语动态标题.empty()) {
            上下文.当前最小原语动态标题 = 恢复快照.最近最小原语动态标题;
        }
    }

    void 私有_从任务管理主体虚拟存在补齐主体ID(结构_任务管理上下文& 上下文) noexcept
    {
        if (!上下文.当前宿主任务) {
            return;
        }

        结构_任务管理主体虚拟存在 主体{};
        if (任务管理任务模块::读取任务管理主体虚拟存在(
            上下文.当前宿主任务,
            &主体) && 主体.主体ID != 0) {
            if (上下文.当前主体ID == 0) {
                上下文.当前主体ID = 主体.主体ID;
            }
            if (上下文.当前方法需求位 == 枚举_任务管理方法需求位::未定义
                && 主体.最近主方法需求位 != 枚举_任务管理方法需求位::未定义) {
                上下文.当前方法需求位 = 主体.最近主方法需求位;
            }
            if (上下文.当前本能能力缺口类型 == 枚举_任务管理本能能力缺口类型::未定义
                && 主体.最近本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义) {
                上下文.当前本能能力缺口类型 = 主体.最近本能能力缺口类型;
            }
            if (上下文.当前输入条件主键.empty()) {
                上下文.当前输入条件主键 = 主体.最近输入条件主键;
            }
            if (上下文.当前治理态型.empty()) {
                上下文.当前治理态型 = 主体.最近治理态型;
            }
            if (!上下文.当前方法位专项动态 && 主体.最近方法位专项动态) {
                上下文.当前方法位专项动态 = 主体.最近方法位专项动态;
            }
            if (上下文.当前方法位专项动态标题.empty()) {
                上下文.当前方法位专项动态标题 = 主体.最近方法位专项动态标题;
            }
            if (!上下文.当前最小原语动态 && 主体.最近最小原语动态) {
                上下文.当前最小原语动态 = 主体.最近最小原语动态;
            }
            if (上下文.当前最小原语动态标题.empty()) {
                上下文.当前最小原语动态标题 = 主体.最近最小原语动态标题;
            }
        }
    }

    void 私有_按正式主体桥接结构补齐上下文(结构_任务管理上下文& 上下文) noexcept
    {
        私有_从最近治理结果补齐主体桥接字段(上下文);
        私有_从治理恢复快照补齐主体桥接字段(上下文);
        私有_从任务管理主体虚拟存在补齐主体ID(上下文);
    }

    枚举_方法类型 私有_推导动作句柄方法类型(const 结构体_动作句柄& 动作句柄) noexcept
    {
        switch (动作句柄.类型) {
        case 枚举_动作句柄类型::外部实现主键:
            return 动作句柄.外部实现主键.empty()
                ? 枚举_方法类型::未定义
                : 枚举_方法类型::外部方法;
        case 枚举_动作句柄类型::动作序列主键:
            return 动作句柄.动作序列主键.empty()
                ? 枚举_方法类型::未定义
                : 枚举_方法类型::内部方法;
        case 枚举_动作句柄类型::本能函数ID: {
            if (动作句柄.本能ID == 0) {
                return 枚举_方法类型::未定义;
            }
            const auto* 元信息 = 本能方法类::查询元信息(
                static_cast<枚举_本能方法ID>(动作句柄.本能ID));
            if (元信息 && 元信息->分类 == 枚举_本能方法分类::外部动作) {
                return 枚举_方法类型::外部方法;
            }
            return 枚举_方法类型::内部方法;
        }
        default:
            return 枚举_方法类型::未定义;
        }
    }

    枚举_方法类型 私有_读取方法类型特征(const 方法节点* 方法头) noexcept
    {
        auto* 方法虚拟存在 = 方法头 ? 方法头->主信息.方法虚拟存在.指针 : nullptr;
        auto* 宿主基础节点 = reinterpret_cast<const 基础信息节点类*>(方法虚拟存在);
        auto* 特征节点 = 宿主基础节点
            ? 世界树.存在().查找子特征_按类型(宿主基础节点, 私有_方法特征_方法类型())
            : nullptr;
        I64 当前值 = 0;
        if (!私有_读取I64特征(特征节点, 当前值)) {
            return 枚举_方法类型::未定义;
        }
        return static_cast<枚举_方法类型>(当前值);
    }

    枚举_方法类型 私有_推导方法类型(
        const 方法节点* 方法头,
        const 结构_任务管理结果* 结果 = nullptr) noexcept
    {
        if (const auto 类型 = 私有_读取方法类型特征(方法头);
            类型 != 枚举_方法类型::未定义) {
            return 类型;
        }

        if (方法头) {
            if (const auto 类型 = 私有_推导动作句柄方法类型(方法头->主信息.动作句柄);
                类型 != 枚举_方法类型::未定义) {
                return 类型;
            }
        }

        const auto* 来源方法 = 结果 ? 结果->上下文.当前方法 : nullptr;
        if (来源方法 && 来源方法 != 方法头) {
            if (const auto 类型 = 私有_读取方法类型特征(来源方法);
                类型 != 枚举_方法类型::未定义) {
                return 类型;
            }
            return 私有_推导动作句柄方法类型(来源方法->主信息.动作句柄);
        }

        return 枚举_方法类型::未定义;
    }

    std::uint64_t 私有_节点主键摘要(const auto* 节点) noexcept
    {
        if (!节点) return 0;
        return static_cast<std::uint64_t>(std::hash<std::string>{}(节点->获取主键()));
    }

    任务节点* 私有_主体锚点任务(任务节点* 宿主任务) noexcept
    {
        auto* 当前 = 宿主任务;
        while (当前) {
            auto* 父任务 = 当前->主信息.父任务.指针
                ? reinterpret_cast<任务节点*>(当前->主信息.父任务.指针)
                : nullptr;
            if (!父任务 || 父任务 == 当前) {
                break;
            }
            当前 = 父任务;
        }
        return 当前 ? 当前 : 宿主任务;
    }

    std::uint64_t 私有_主体ID_按宿主任务(任务节点* 宿主任务) noexcept
    {
        auto* 锚点任务 = 私有_主体锚点任务(宿主任务);
        const auto 锚点ID = 私有_节点主键摘要(锚点任务 ? 锚点任务 : 宿主任务);
        if (锚点ID != 0) {
            return 锚点ID;
        }

        const auto* 回退节点 = 锚点任务 ? 锚点任务 : 宿主任务;
        return 回退节点
            ? static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(回退节点))
            : 0;
    }

    std::int32_t 私有_Q10000比率(I64 当前值, I64 目标值) noexcept
    {
        if (目标值 <= 0) {
            return 0;
        }

        auto 比率 = static_cast<long long>(当前值) * 10000LL / static_cast<long long>(目标值);
        if (比率 < 0) 比率 = 0;
        if (比率 > 20000) 比率 = 20000;
        return static_cast<std::int32_t>(比率);
    }

    void 私有_刷新分身继承位图(
        结构_任务管理分身继承面& 分身,
        const 结构_任务管理上下文& 上下文) noexcept
    {
        分身.继承任务特征位图 = 0;
        if (分身.宿主任务) 分身.继承任务特征位图 |= (1ULL << 0);
        if (分身.当前步骤) 分身.继承任务特征位图 |= (1ULL << 1);
        if (分身.最近结果) 分身.继承任务特征位图 |= (1ULL << 2);
        if (上下文.最近功能域 != 枚举_任务管理功能域::未定义) 分身.继承任务特征位图 |= (1ULL << 3);
        if (上下文.当前缺口类型 != 枚举_任务管理缺口类型::未定义) 分身.继承任务特征位图 |= (1ULL << 4);
        if (上下文.当前下一步去向 != 枚举_任务管理下一步去向::未定义) 分身.继承任务特征位图 |= (1ULL << 5);
        if (上下文.最近根层重判结果 != 枚举_任务管理根层重判结果::未定义) 分身.继承任务特征位图 |= (1ULL << 6);
        if (上下文.最近执行前门控结果 != 枚举_任务管理执行前门控结果::未定义) 分身.继承任务特征位图 |= (1ULL << 7);

        分身.继承方法特征位图 = 0;
        if (分身.当前管理方法) 分身.继承方法特征位图 |= (1ULL << 0);
        if (私有_推导方法类型(分身.当前管理方法) != 枚举_方法类型::未定义) 分身.继承方法特征位图 |= (1ULL << 1);
        if (上下文.最近反馈类型 != 枚举_任务管理反馈类型::未定义) 分身.继承方法特征位图 |= (1ULL << 2);
    }

    std::string 私有_主体化输入条件主键(
        const 结构_任务管理上下文& 上下文,
        const 结构_任务管理分身继承面& 分身) noexcept
    {
        std::ostringstream 输出;
        输出 << "宿主=" << 私有_任务标题(分身.宿主任务 ? 分身.宿主任务 : 上下文.当前宿主任务)
            << "|方法=" << 私有_方法标题(分身.当前管理方法 ? 分身.当前管理方法 : 上下文.当前方法)
            << "|缺口=" << 私有_缺口文本(上下文.当前缺口类型)
            << "|去向=" << 私有_去向文本(上下文.当前下一步去向)
            << "|域=" << 私有_功能域文本(上下文.最近功能域)
            << "|门控=" << 私有_执行前门控结果文本(上下文.最近执行前门控结果)
            << "|根层=" << 私有_根层重判结果文本(上下文.最近根层重判结果);
        return 输出.str();
    }

    std::string 私有_主体化结果主键(
        const 结构_任务管理分身继承面& 分身,
        const 结构_任务管理上下文& 上下文) noexcept
    {
        if (分身.最近结果) {
            return 分身.最近结果->获取主键();
        }
        if (上下文.最近结果) {
            return 上下文.最近结果->获取主键();
        }
        if (分身.当前步骤) {
            return "步骤:" + 分身.当前步骤->获取主键();
        }
        if (分身.当前管理方法) {
            return "方法:" + 分身.当前管理方法->获取主键();
        }
        return {};
    }

    枚举_任务管理方法需求位 私有_推导任务管理方法需求位(
        const 结构_任务管理上下文& 上下文,
        const 结构_任务管理分身继承面& 分身) noexcept
    {
        const auto* 宿主任务 = 分身.宿主任务 ? 分身.宿主任务 : 上下文.当前宿主任务;
        const auto 宿主状态 = 宿主任务 ? 宿主任务->主信息.状态 : 枚举_任务状态::未定义;
        const bool 宿主已完成 = 宿主状态 == 枚举_任务状态::完成;
        const bool 宿主已失败 = 宿主状态 == 枚举_任务状态::失败 || 宿主状态 == 枚举_任务状态::取消;
        const bool 目标结果已满足 = 私有_任务节点目标结果已对齐(宿主任务);
        const bool 已满足完成判据 = 宿主已完成;
        const bool 已确定无法继续 =
            宿主已失败
            || 上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::禁止继续
            || 上下文.最近根层重判结果 == 枚举_任务管理根层重判结果::停止退出边界;
        const bool 需要补结构 =
            !宿主任务
            || !分身.当前步骤
            || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::治理任务缺失
            || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::宿主任务缺失
            || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::任务待启动;
        const bool 需要选方法 =
            宿主任务
            && (!分身.当前管理方法
                || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
                || 上下文.当前下一步去向 == 枚举_任务管理下一步去向::绑定方法);
        const bool 需要门控转域 =
            上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::仅允许等待
            || 上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::仅允许收束
            || 上下文.最近根层重判结果 == 枚举_任务管理根层重判结果::待机保持
            || 上下文.当前下一步去向 == 枚举_任务管理下一步去向::回到重筹办
            || 上下文.当前下一步去向 == 枚举_任务管理下一步去向::保持等待;
        const bool 需要收束复盘 =
            上下文.最近根层重判结果 == 枚举_任务管理根层重判结果::收束优先
            || 上下文.当前下一步去向 == 枚举_任务管理下一步去向::进入收束
            || 宿主已完成;

        if (需要收束复盘) {
            return 枚举_任务管理方法需求位::收束复盘;
        }
        if (已确定无法继续) {
            return 宿主已失败
                ? 枚举_任务管理方法需求位::失败策略
                : 枚举_任务管理方法需求位::门控转域;
        }
        if (需要门控转域) {
            return 枚举_任务管理方法需求位::门控转域;
        }
        if (需要补结构) {
            return 枚举_任务管理方法需求位::补结构;
        }
        if (需要选方法) {
            return 枚举_任务管理方法需求位::选择推进;
        }
        if (已满足完成判据 || 目标结果已满足) {
            return 枚举_任务管理方法需求位::求证判定;
        }
        if (私有_任务节点目标结果待推进(宿主任务) && !分身.最近结果) {
            return 枚举_任务管理方法需求位::分解补条件;
        }
        if (私有_任务节点目标结果待推进(宿主任务)) {
            return 枚举_任务管理方法需求位::选择推进;
        }
        return 枚举_任务管理方法需求位::求证判定;
    }

    枚举_任务管理步骤位类型 私有_P0_推导步骤位类型(
        const 结构_任务管理上下文& 上下文) noexcept
    {
        if (上下文.当前下一步去向 == 枚举_任务管理下一步去向::保持等待) {
            return 枚举_任务管理步骤位类型::等待位;
        }
        if (上下文.当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
            || 上下文.当前下一步去向 == 枚举_任务管理下一步去向::绑定方法) {
            return 枚举_任务管理步骤位类型::补结构位;
        }
        if (上下文.最近结果) {
            return 枚举_任务管理步骤位类型::回接位;
        }
        return 枚举_任务管理步骤位类型::执行位;
    }

    枚举_任务管理步骤语义类型 私有_P0_推导步骤语义类型(
        const 结构_任务管理上下文& 上下文,
        枚举_任务管理步骤位类型 步骤位类型) noexcept
    {
        switch (步骤位类型) {
        case 枚举_任务管理步骤位类型::等待位:
            return 枚举_任务管理步骤语义类型::等待维持步骤;
        case 枚举_任务管理步骤位类型::回接位:
            return 枚举_任务管理步骤语义类型::回接派生步骤;
        case 枚举_任务管理步骤位类型::补条件位:
            return 枚举_任务管理步骤语义类型::补条件步骤;
        case 枚举_任务管理步骤位类型::补结构位:
            return 枚举_任务管理步骤语义类型::补结构步骤;
        case 枚举_任务管理步骤位类型::执行位:
            return 私有_任务节点目标结果待推进(上下文.当前宿主任务)
                ? 枚举_任务管理步骤语义类型::业务推进步骤
                : 枚举_任务管理步骤语义类型::治理步骤;
        default:
            return 枚举_任务管理步骤语义类型::未定义;
        }
    }

    枚举_任务管理本能能力缺口类型 私有_P0_推导本能能力缺口(
        const 结构_任务管理上下文& 上下文,
        枚举_任务管理方法需求位 需求位) noexcept
    {
        if (!上下文.当前宿主任务) {
            return 枚举_任务管理本能能力缺口类型::缺观察原语;
        }
        if (!上下文.当前方法) {
            return 枚举_任务管理本能能力缺口类型::缺挂点原语;
        }
        if (!上下文.最近结果) {
            return 需求位 == 枚举_任务管理方法需求位::分解补条件
                ? 枚举_任务管理本能能力缺口类型::缺状态差分取证
                : 枚举_任务管理本能能力缺口类型::缺动作边界;
        }
        return 枚举_任务管理本能能力缺口类型::无缺口;
    }

    bool 私有_P0_存在本地上下文(const 结构_任务管理上下文& 上下文) noexcept
    {
        return 上下文.当前时间 != 0
            || 上下文.当前主需求 != nullptr
            || 上下文.当前管理任务 != nullptr
            || 上下文.当前宿主任务 != nullptr
            || 上下文.当前方法 != nullptr
            || 上下文.当前步骤 != nullptr
            || 上下文.最近结果 != nullptr
            || 上下文.当前缺口类型 != 枚举_任务管理缺口类型::未定义
            || 上下文.当前下一步去向 != 枚举_任务管理下一步去向::未定义
            || 上下文.当前步骤位类型 != 枚举_任务管理步骤位类型::未定义
            || 上下文.当前步骤语义类型 != 枚举_任务管理步骤语义类型::未定义
            || 上下文.最近根层重判结果 != 枚举_任务管理根层重判结果::未定义
            || 上下文.最近执行前门控结果 != 枚举_任务管理执行前门控结果::未定义
            || 上下文.最近功能域 != 枚举_任务管理功能域::未定义
            || 上下文.最近总控结果 != 枚举_任务管理总控结果::未定义
            || 上下文.最近反馈类型 != 枚举_任务管理反馈类型::未定义
            || 上下文.最近宿主目标结果比较 != 枚举_任务管理双面比较结果::未定义
            || 上下文.最近后果治理 != 枚举_任务管理后果治理::未定义
            || 上下文.当前主体ID != 0
            || 上下文.当前分身ID != 0
            || 上下文.当前方法需求位 != 枚举_任务管理方法需求位::未定义
            || 上下文.当前本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义
            || !上下文.当前输入条件主键.empty()
            || !上下文.当前治理态型.empty()
            || 上下文.已触发学习承接
            || 上下文.当前学习承接类型 != 枚举_任务管理学习承接类型::未定义;
    }

    结构_任务管理请求 私有_P0_合成阶段请求(
        自我类& 自我对象,
        时间戳 当前时间,
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_任务管理请求 阶段请求{};
        (void)任务管理任务模块::构造任务管理请求(自我对象, 当前时间, &阶段请求);
        私有_P0_覆盖请求字段(阶段请求, 结果.当前请求);
        if (私有_P0_存在本地上下文(结果.上下文)) {
            私有_P0_覆盖上下文字段(阶段请求.上下文, 结果.上下文);
            私有_P0_覆盖非空指针(阶段请求.宿主任务, 结果.上下文.当前宿主任务);
        }
        私有_P0_刷新请求原语锚点(阶段请求);
        (void)私有_P1_刷新任务治理二次特征证据(
            自我对象,
            阶段请求,
            "任务管理任务模块::私有_P0_合成阶段请求");
        (void)私有_P0_生成原语判定汇总(阶段请求, 阶段请求.当前原语判定汇总);
        return 阶段请求;
    }

    结构_任务管理单步决策 私有_P0_合成单步决策(
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_任务管理单步决策 阶段决策{};
        (void)任务管理任务模块::形成任务管理单步决策(结果, &阶段决策);
        return 阶段决策;
    }

    结构_任务管理写回结果 私有_P0_合成写回结果(
        自我类& 自我对象,
        const 结构_任务管理结果& 结果) noexcept
    {
        (void)自我对象;
        结构_任务管理写回结果 阶段写回{};
        (void)任务管理任务模块::形成任务管理写回结果(结果, &阶段写回);
        return 阶段写回;
    }

    结构_任务管理一步结果 私有_P0_合成一步结果(
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_任务管理一步结果 阶段一步结果{};
        (void)任务管理任务模块::组装任务管理一步结果(结果, &阶段一步结果);
        return 阶段一步结果;
    }

    void 私有_P0_回填兼容字段(结构_任务管理结果& 结果) noexcept
    {
        const auto 阶段当前功能域 = 私有_P0_阶段当前功能域(结果);
        const auto 阶段当前缺口类型 = 私有_P0_阶段当前缺口类型(结果);
        const auto 阶段当前下一步去向 = 私有_P0_阶段当前下一步去向(结果);
        const auto 阶段最近总控结果 = 私有_P0_阶段最近总控结果(结果);
        const auto 阶段最近反馈类型 = 私有_P0_阶段最近反馈类型(结果);

        结果.当前功能域 = 阶段当前功能域;
        结果.当前缺口类型 = 阶段当前缺口类型;
        结果.当前下一步去向 = 阶段当前下一步去向;
        私有_P0_覆盖决策到兼容字段(结果, 结果.当前单步决策);
        私有_P0_覆盖写回到兼容字段(结果, 结果.当前写回结果);
        结果.最近功能域 = 阶段当前功能域;
        结果.最近总控结果 = 阶段最近总控结果;
        结果.最近反馈类型 = 阶段最近反馈类型;
        私有_P0_覆盖一步结果到兼容字段(结果, 结果.当前一步结果);
    }

    void 私有_刷新P0分阶段骨架(
        自我类& 自我对象,
        时间戳 当前时间,
        结构_任务管理结果& 结果) noexcept
    {
        结果.当前请求 = 私有_P0_合成阶段请求(自我对象, 当前时间, 结果);
        结果.上下文 = 结果.当前请求.上下文;
        结果.当前单步决策 = 私有_P0_合成单步决策(结果);
        结果.当前写回结果 = 私有_P0_合成写回结果(自我对象, 结果);
        结果.当前一步结果 = 私有_P0_合成一步结果(结果);
        私有_P0_回填兼容字段(结果);
        私有_同步治理上下文镜像(结果);
    }

    方法节点* 私有_查找本能方法首节点(
        const 自我类& 自我对象,
        枚举_本能方法ID 本能ID) noexcept
    {
        auto* 自我存在 = const_cast<存在节点类*>(自我对象.获取自我存在());
        if (!自我存在) return nullptr;

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        if (!方法根节点) return nullptr;

        return 私有_查找直接子节点_按谓词<方法节点>(方法根节点, [&](const 方法节点* 节点) noexcept {
            if (!节点) return false;
            const auto& 主信息 = 节点->主信息;
            return 主信息.节点种类 == 枚举_方法节点种类::方法首节点
                && 主信息.动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
                && 主信息.动作句柄.本能ID == static_cast<I64>(本能ID);
        });
    }

    bool 私有_任务管理最小原语已正式注册(
        const 自我类& 自我对象,
        枚举_任务管理最小原语ID 原语ID,
        const std::string& 调用点) noexcept
    {
        const auto 兼容本能ID = 私有_最小原语对应本能方法ID(原语ID);
        if (兼容本能ID == 枚举_本能方法ID::未定义) {
            return false;
        }

        (void)调用点;
        return 私有_查找本能方法首节点(自我对象, 兼容本能ID) != nullptr;
    }

    bool 私有_任务状态已终结(枚举_任务状态 状态) noexcept;
    bool 私有_稳定等待态复盘学习(
        const 结构_任务管理结果& 结果) noexcept;
    const 词性节点类* 私有_补条件子任务类型词() noexcept;
    const 词性节点类* 私有_业务子任务类型词() noexcept;
    const 词性节点类* 私有_学习子任务类型词(枚举_任务管理学习承接类型 类型) noexcept;
    bool 私有_学习子任务类型匹配(const 词性节点类* 类型词) noexcept;

    任务节点* 私有_查找直接补条件子任务(const 任务节点* 宿主任务) noexcept
    {
        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(const_cast<任务节点*>(宿主任务), [&](const 任务节点* 节点) noexcept {
            return 节点
                && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                && 私有_词性相同(节点->主信息.类型, 私有_补条件子任务类型词())
                && !私有_任务状态已终结(节点->主信息.状态);
        }));
    }

    任务节点* 私有_查找直接业务子任务(const 任务节点* 宿主任务) noexcept
    {
        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(const_cast<任务节点*>(宿主任务), [&](const 任务节点* 节点) noexcept {
            return 节点
                && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                && 私有_词性相同(节点->主信息.类型, 私有_业务子任务类型词())
                && !私有_任务状态已终结(节点->主信息.状态);
        }));
    }

    任务节点* 私有_查找任务管理学习树根(const 任务节点* 管理任务) noexcept
    {
        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(
            const_cast<任务节点*>(管理任务),
            [&](const 任务节点* 节点) noexcept {
                return 节点
                    && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                    && 私有_词性相同(节点->主信息.类型, 语素集.添加词性词("学习树根节点", "名词"));
            }));
    }

    任务节点* 私有_查找定向学习分支(const 任务节点* 学习树根) noexcept
    {
        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(
            const_cast<任务节点*>(学习树根),
            [&](const 任务节点* 节点) noexcept {
                return 节点
                    && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                    && 私有_词性相同(节点->主信息.类型, 语素集.添加词性词("学习分支", "名词"))
                    && 私有_词性相同(节点->主信息.名称, 语素集.添加词性词("定向学习分支", "专有名词"));
            }));
    }

    任务节点* 私有_查找广泛学习分支(const 任务节点* 学习树根) noexcept
    {
        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(
            const_cast<任务节点*>(学习树根),
            [&](const 任务节点* 节点) noexcept {
                return 节点
                    && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                    && 私有_词性相同(节点->主信息.类型, 语素集.添加词性词("学习分支", "名词"))
                    && 私有_词性相同(节点->主信息.名称, 语素集.添加词性词("广泛学习分支", "专有名词"));
            }));
    }

    任务节点* 私有_查找直接派生子任务(const 任务节点* 宿主任务) noexcept
    {
        if (auto* 业务子任务 = 私有_查找直接业务子任务(宿主任务)) {
            return 业务子任务;
        }
        return 私有_查找直接补条件子任务(宿主任务);
    }

    任务节点* 私有_查找最深派生子任务(任务节点* 起点) noexcept
    {
        auto* 当前 = 起点;
        auto* 命中 = static_cast<任务节点*>(nullptr);
        while (当前) {
            auto* 下级 = 私有_查找直接派生子任务(当前);
            if (!下级) {
                break;
            }
            命中 = 下级;
            当前 = 下级;
        }
        return 命中;
    }

    任务节点* 私有_选择终结后回接宿主(任务节点* 起点) noexcept
    {
        auto* 当前 = 起点;
        while (当前 && 私有_任务状态已终结(当前->主信息.状态)) {
            auto* 父任务 = 当前->主信息.父任务.指针
                ? reinterpret_cast<任务节点*>(当前->主信息.父任务.指针)
                : nullptr;
            if (!父任务) {
                return nullptr;
            }
            if (auto* 子任务 = 私有_查找最深派生子任务(父任务)) {
                return 子任务;
            }
            当前 = 父任务;
        }
        return 当前;
    }

    bool 私有_允许默认本能兜底(const 自我类& 自我对象, const 任务节点* 宿主任务) noexcept
    {
        if (!宿主任务) {
            return false;
        }

        if (宿主任务 == 自我对象.获取安全根任务()
            || 宿主任务 == 自我对象.获取服务根任务()) {
            return true;
        }

        return 私有_任务标题(宿主任务) == "任务管理任务";
    }

    任务节点* 私有_展开锚点宿主候选(任务节点* 根任务) noexcept
    {
        if (!根任务) {
            return nullptr;
        }
        if (auto* 子任务 = 私有_查找最深派生子任务(根任务)) {
            return 子任务;
        }
        return 根任务;
    }

    任务节点* 私有_选择锚点驱动根任务(
        const 自我类& 自我对象,
        const 结构_任务管理上下文& 上下文,
        std::string* 锚点裁决摘要) noexcept
    {
        const auto 写摘要 = [&](const std::string& 文本) {
            if (锚点裁决摘要) {
                *锚点裁决摘要 = 文本;
            }
        };

        auto* 当前主需求 = 上下文.当前主需求;
        if (当前主需求 && 当前主需求 == 自我对象.获取安全根需求()) {
            auto* 根任务 = 自我对象.获取安全根任务();
            写摘要(
                "锚点=安全根需求 | 根任务=" + 私有_任务标题(根任务)
                + " | 宿主候选=" + 私有_任务标题(私有_展开锚点宿主候选(根任务)));
            return 根任务;
        }
        if (当前主需求 && 当前主需求 == 自我对象.获取服务根需求()) {
            auto* 根任务 = 自我对象.获取服务根任务();
            写摘要(
                "锚点=服务根需求 | 根任务=" + 私有_任务标题(根任务)
                + " | 宿主候选=" + 私有_任务标题(私有_展开锚点宿主候选(根任务)));
            return 根任务;
        }

        if (auto* 服务根任务 = 自我对象.获取服务根任务()) {
            写摘要(
                "锚点=主需求未显式命中 | 回退根任务=" + 私有_任务标题(服务根任务)
                + " | 宿主候选=" + 私有_任务标题(私有_展开锚点宿主候选(服务根任务)));
            return 服务根任务;
        }
        if (auto* 安全根任务 = 自我对象.获取安全根任务()) {
            写摘要(
                "锚点=主需求未显式命中 | 回退根任务=" + 私有_任务标题(安全根任务)
                + " | 宿主候选=" + 私有_任务标题(私有_展开锚点宿主候选(安全根任务)));
            return 安全根任务;
        }

        写摘要("锚点=空 | 根任务=空");
        return nullptr;
    }

    任务节点* 私有_选择宿主任务(
        const 自我类& 自我对象,
        const 结构_任务管理上下文& 上下文,
        std::string* 锚点裁决摘要) noexcept
    {
        std::string 锚点摘要{};
        auto* 锚点根任务 = 私有_选择锚点驱动根任务(自我对象, 上下文, &锚点摘要);
        auto* 锚点宿主候选 = 私有_展开锚点宿主候选(锚点根任务);

        if (上下文.当前宿主任务 && 上下文.当前宿主任务 != 上下文.当前管理任务) {
            auto* 当前候选 = 上下文.当前宿主任务;
            if (私有_任务状态已终结(上下文.当前宿主任务->主信息.状态)) {
                if (auto* 回接宿主 = 私有_选择终结后回接宿主(上下文.当前宿主任务)) {
                    当前候选 = 回接宿主;
                }
                else {
                    当前候选 = nullptr;
                }
            }
            else if (auto* 子任务 = 私有_查找最深派生子任务(上下文.当前宿主任务)) {
                当前候选 = 子任务;
            }

            const bool 当前候选仍在锚点分支 =
                !锚点根任务
                || !当前候选
                || 当前候选 == 锚点根任务
                || 私有_是祖先节点(锚点根任务, 当前候选);
            if (锚点宿主候选 && !当前候选仍在锚点分支) {
                if (锚点裁决摘要) {
                    *锚点裁决摘要 = 锚点摘要
                        + " | 当前宿主已偏离锚点分支，切换到=" + 私有_任务标题(锚点宿主候选);
                }
                return 锚点宿主候选;
            }
            if (当前候选) {
                if (锚点裁决摘要) {
                    *锚点裁决摘要 = 锚点摘要.empty()
                        ? "锚点=空 | 沿用当前宿主=" + 私有_任务标题(当前候选)
                        : (锚点摘要 + " | 当前宿主仍在锚点分支，沿用=" + 私有_任务标题(当前候选));
                }
                return 当前候选;
            }
        }

        if (锚点宿主候选) {
            if (锚点裁决摘要) {
                *锚点裁决摘要 = 锚点摘要 + " | 按锚点选中宿主=" + 私有_任务标题(锚点宿主候选);
            }
            return 锚点宿主候选;
        }

        if (锚点裁决摘要) {
            *锚点裁决摘要 = 锚点摘要.empty() ? "锚点=空 | 未选中宿主任务" : (锚点摘要 + " | 未选中宿主任务");
        }
        return nullptr;
    }

    枚举_本能方法ID 私有_选择本能方法ID(const 自我类& 自我对象, 任务节点* 宿主任务) noexcept
    {
        if (!宿主任务) {
            return 枚举_本能方法ID::未定义;
        }

        if (宿主任务 == 自我对象.获取安全根任务()) {
            return 枚举_本能方法ID::自我_停止当前试探并进入保守态;
        }

        if (宿主任务 == 自我对象.获取服务根任务()) {
            return 枚举_本能方法ID::自我_读取核心状态;
        }

        const auto 标题 = 私有_任务标题(宿主任务);
        if (标题 == "任务管理任务") {
            return 枚举_本能方法ID::自我_读取核心状态;
        }

        return 枚举_本能方法ID::自我_读取核心状态;
    }

    bool 私有_任务节点归属宿主任务(const 任务节点* 节点, const 任务节点* 宿主任务) noexcept;

    bool 私有_方法属于宿主任务(const 方法节点* 方法, const 任务节点* 宿主任务) noexcept
    {
        if (!方法 || !宿主任务) {
            return false;
        }
        const auto* 来源任务 = 方法->主信息.来源任务.指针
            ? reinterpret_cast<const 任务节点*>(方法->主信息.来源任务.指针)
            : nullptr;
        return 来源任务 == 宿主任务;
    }

    bool 私有_方法条件筛选通过(
        const 方法节点* 方法,
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果,
        bool& 使用了条件节点) noexcept
    {
        使用了条件节点 = false;
        if (!方法 || !宿主任务 || !方法->子) {
            return true;
        }

        const 状态节点类* 候选状态[] = {
            结果.最新状态,
            结果.最新目标状态,
            宿主任务->主信息.结果状态信息.指针,
            宿主任务->主信息.目标状态.指针,
        };

        bool 有条件节点 = false;
        auto* 首节点 = static_cast<const 方法节点*>(方法->子);
        auto* 当前 = 首节点;
        do {
            if (!当前 || 当前->主信息.节点种类 != 枚举_方法节点种类::方法条件节点) {
                当前 = 当前 ? static_cast<const 方法节点*>(当前->下) : nullptr;
                continue;
            }

            有条件节点 = true;
            使用了条件节点 = true;
            const auto* 初始状态 = 私有_取首个有效状态(当前->主信息.结果初始状态);
            if (!初始状态) {
                return true;
            }

            std::int32_t 命中结果 = 0;
            if (二次特征本能判断模块::判断状态是否命中候选集(
                &世界树,
                初始状态,
                候选状态,
                候选状态 + std::size(候选状态),
                命中结果)
                && 命中结果 > 0) {
                return true;
            }

            当前 = static_cast<const 方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return !有条件节点;
    }

    方法节点* 私有_从最近结果回看方法(
        const 结构_任务管理结果& 结果,
        const 任务节点* 宿主任务) noexcept
    {
        const auto* 最近结果 = 结果.上下文.最近结果;
        if (!最近结果
            || 最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
            || !私有_任务节点归属宿主任务(最近结果, 宿主任务)) {
            return nullptr;
        }

        return 最近结果->主信息.执行方法.指针
            ? reinterpret_cast<方法节点*>(最近结果->主信息.执行方法.指针)
            : nullptr;
    }

    方法节点* 私有_查找宿主任务方法首节点候选(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果,
        bool& 使用了条件节点) noexcept
    {
        使用了条件节点 = false;
        auto* 自我存在 = const_cast<存在节点类*>(自我对象.获取自我存在());
        if (!自我存在 || !宿主任务) {
            return nullptr;
        }

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        if (!方法根节点 || !方法根节点->子) {
            return nullptr;
        }

        auto* 首节点 = static_cast<方法节点*>(方法根节点->子);
        auto* 当前 = 首节点;
        do {
            if (!当前 || 当前->主信息.节点种类 != 枚举_方法节点种类::方法首节点) {
                当前 = 当前 ? static_cast<方法节点*>(当前->下) : nullptr;
                continue;
            }
            if (!私有_方法属于宿主任务(当前, 宿主任务)) {
                当前 = static_cast<方法节点*>(当前->下);
                continue;
            }

            bool 当前方法命中条件 = false;
            if (私有_方法条件筛选通过(当前, 宿主任务, 结果, 当前方法命中条件)) {
                使用了条件节点 = 当前方法命中条件;
                return 当前;
            }

            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    方法节点* 私有_查找主体长期策略方法候选(
        const 自我类& 自我对象,
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        const auto& 主体 = 结果.当前主体;
        if (!宿主任务
            || 主体.累计正式方法资产提交次数 == 0
            || 主体.最近正式方法标题.empty()
            || !私有_M10_长期策略可靠度允许回填(主体)) {
            return nullptr;
        }

        auto* 自我存在 = const_cast<存在节点类*>(自我对象.获取自我存在());
        if (!自我存在) {
            return nullptr;
        }

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        if (!方法根节点 || !方法根节点->子) {
            return nullptr;
        }

        auto* 首节点 = static_cast<方法节点*>(方法根节点->子);
        auto* 当前 = 首节点;
        do {
            if (!当前 || 当前->主信息.节点种类 != 枚举_方法节点种类::方法首节点) {
                当前 = 当前 ? static_cast<方法节点*>(当前->下) : nullptr;
                continue;
            }
            if (私有_方法标题(当前) != 主体.最近正式方法标题) {
                当前 = static_cast<方法节点*>(当前->下);
                continue;
            }

            bool 当前方法命中条件 = false;
            if (私有_方法条件筛选通过(当前, 宿主任务, 结果, 当前方法命中条件)) {
                return 当前;
            }

            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    bool 私有_M10_主体长期策略允许直放后段(const 结构_任务管理结果& 结果) noexcept
    {
        auto* 宿主任务 = 私有_P0_当前宿主任务(结果);
        const auto 当前本能缺口 = 私有_P0_当前本能缺口(结果);
        return 宿主任务
            && 私有_M10_当前方法命中主体长期策略(结果)
            && 当前本能缺口 == 枚举_任务管理本能能力缺口类型::需学习补齐
            && 私有_M10_长期策略可靠度允许回填(结果.当前主体)
            && 结果.当前主体.累计正式方法资产提交次数 > 0
            && !结果.当前主体.最近正式方法标题.empty()
            && 私有_M10_存在业务推进压力(结果);
    }

    void 私有_完成方法挂接(
        自我类& 自我对象,
        任务节点* 宿主任务,
        方法节点* 方法,
        时间戳 now,
        const std::string& 调用点,
        枚举_任务管理方法来源 来源,
        结构_任务管理结果& 结果,
        方法节点*& 输出方法) noexcept
    {
        输出方法 = 方法;
        结果.当前方法来源 = 来源;
        if (!宿主任务 || !方法) {
            return;
        }

        auto& 任务主信息 = 宿主任务->主信息;
        私有_写入引用(任务主信息.执行方法, 方法);
        任务主信息.动作句柄 = 方法->主信息.动作句柄;
        if (!方法->主信息.来源任务.指针) {
            私有_写入引用(方法->主信息.来源任务, 宿主任务);
        }
        (void)自我对象.设置当前主方法(方法, now, 调用点 + "/设置当前主方法");
    }

    bool 私有_迁移任务状态(任务节点* 节点, 枚举_任务状态 新状态, 时间戳 now) noexcept
    {
        if (!节点) return false;

        auto& 主信息 = 节点->主信息;
        if (主信息.状态 == 新状态) {
            return false;
        }

        if ((新状态 == 枚举_任务状态::运行中) && 主信息.启动时间 == 0) {
            主信息.启动时间 = now;
        }
        if ((新状态 == 枚举_任务状态::完成
            || 新状态 == 枚举_任务状态::失败
            || 新状态 == 枚举_任务状态::取消)
            && 主信息.完成时间 == 0) {
            主信息.完成时间 = now;
        }

        主信息.状态 = 新状态;
        return true;
    }

    状态节点类* 私有_创建状态节点(
        场景节点类* 场景,
        基础信息节点类* 主体,
        特征节点类* 特征,
        const 特征值& 值,
        时间戳 now,
        bool 是否变化,
        const std::string& 原因类别,
        const std::string& 原因说明) noexcept
    {
        auto* 状态节点 = 世界树.状态().创建状态(场景, 主体, 特征, 值, now, now);
        if (!状态节点) return nullptr;

        auto* 主信息 = 世界树.状态().取状态主信息(状态节点);
        if (主信息) {
            const auto* 特征类型词 = (特征 && 特征->主信息) ? 特征->主信息->类型 : nullptr;
            主信息->名称 = 特征类型词;
            主信息->类型 = 特征类型词;
        }
        (void)世界树.状态().标记变化(状态节点, 是否变化, 原因类别, 原因说明);
        return 状态节点;
    }

    bool 私有_记录宿主任务治理证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::宿主任务治理);
        auto* 宿主任务 = 来源任务 ? 来源任务 : 自我对象.获取当前主任务();
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(
            自我对象,
            宿主任务,
            调用点 + "/任务虚拟存在");
        auto* 场景 = 私有_任务场景(自我对象, 宿主任务);
        auto* 特征节点 = 私有_确保宿主任务治理特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/特征");
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        if (!宿主任务 || !任务虚拟存在 || !场景 || !宿主基础节点 || !特征节点) {
            return false;
        }

        I64 旧值 = 0;
        const bool 有旧值 = 世界树.读取特征_I64(特征节点, 旧值);
        if (有旧值 && 旧值 == 新值) {
            return false;
        }

        if (!世界树.写入特征_I64(特征节点, 新值, now)) {
            return false;
        }

        状态节点类* 条件状态 = nullptr;
        if (有旧值) {
            条件状态 = 私有_创建状态节点(
                场景,
                宿主基础节点,
                特征节点,
                特征值{ 旧值 },
                now,
                false,
                动作语义键,
                "治理前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ 新值 },
            now,
            true,
            动作语义键,
            "治理后镜像");
        if (!结果状态) {
            return false;
        }

        auto* 动态节点 = 世界树.动态().创建动态(场景, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 任务虚拟存在, 特征节点);
            if (来源方法) {
                (void)世界树.动态().绑定来源方法(
                    动态节点,
                    reinterpret_cast<方法节点类*>(来源方法),
                    来源方法->主信息.动作名,
                    枚举_动作事件相位::完成);
            }
            (void)世界树.动态().绑定来源场景(动态节点, 场景, 场景);
        }

        auto* 因果节点 = 世界树.按动态创建因果信息(
            场景,
            动态节点,
            枚举_因果来源类型::执行生成,
            任务虚拟存在,
            特征节点,
            动作语义键,
            true);
        if (因果节点) {
            if (auto* 因果主信息 = 世界树.因果().取模板主信息(因果节点)) {
                因果主信息->名称 = 特征类型;
                因果主信息->类型 = 特征类型;
                因果主信息->置信度 = 10000;
            }
        }

        结果.已生成治理证据 = true;
        结果.最新状态 = 结果状态;
        结果.最新动态 = 动态节点;
        结果.最新抽象因果 = 因果节点;
        return true;
    }

    bool 私有_记录方法资产证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::方法资产);
        auto* 方法头 = 来源方法;
        auto* 方法虚拟存在 = 方法头 ? 方法头->主信息.方法虚拟存在.指针 : nullptr;
        auto* 场景 = 来源任务 ? 私有_任务场景(自我对象, 来源任务) : nullptr;
        if (!场景) {
            场景 = 自我对象.确保自我内部世界(调用点 + "/自我内部世界");
        }
        auto* 特征节点 = 私有_确保方法资产特征节点(
            方法头,
            特征类型,
            调用点 + "/特征");
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(方法虚拟存在);
        if (!方法头 || !方法虚拟存在 || !场景 || !宿主基础节点 || !特征节点) {
            return false;
        }

        I64 旧值 = 0;
        const bool 有旧值 = 世界树.读取特征_I64(特征节点, 旧值);
        if (有旧值 && 旧值 == 新值) {
            return false;
        }

        if (!世界树.写入特征_I64(特征节点, 新值, now)) {
            return false;
        }

        状态节点类* 条件状态 = nullptr;
        if (有旧值) {
            条件状态 = 私有_创建状态节点(
                场景,
                宿主基础节点,
                特征节点,
                特征值{ 旧值 },
                now,
                false,
                动作语义键,
                "治理前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ 新值 },
            now,
            true,
            动作语义键,
            "治理后镜像");
        if (!结果状态) {
            return false;
        }

        auto* 动态节点 = 世界树.动态().创建动态(场景, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 方法虚拟存在, 特征节点);
            if (来源方法) {
                (void)世界树.动态().绑定来源方法(
                    动态节点,
                    reinterpret_cast<方法节点类*>(来源方法),
                    来源方法->主信息.动作名,
                    枚举_动作事件相位::完成);
            }
            (void)世界树.动态().绑定来源场景(动态节点, 场景, 场景);
        }

        auto* 因果节点 = 世界树.按动态创建因果信息(
            场景,
            动态节点,
            枚举_因果来源类型::执行生成,
            方法虚拟存在,
            特征节点,
            动作语义键,
            true);
        if (因果节点) {
            if (auto* 因果主信息 = 世界树.因果().取模板主信息(因果节点)) {
                因果主信息->名称 = 特征类型;
                因果主信息->类型 = 特征类型;
                因果主信息->置信度 = 10000;
            }
        }

        结果.已生成治理证据 = true;
        结果.最新状态 = 结果状态;
        结果.最新动态 = 动态节点;
        结果.最新抽象因果 = 因果节点;
        return true;
    }

    bool 私有_记录宿主任务治理证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖,
        枚举_动作事件相位 来源相位) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::宿主任务治理);
        auto* 宿主任务 = 来源任务 ? 来源任务 : 自我对象.获取当前主任务();
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(
            自我对象,
            宿主任务,
            调用点 + "/任务虚拟存在");
        auto* 场景 = 私有_任务场景(自我对象, 宿主任务);
        auto* 特征节点 = 私有_确保宿主任务治理特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/特征");
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        if (!宿主任务 || !任务虚拟存在 || !场景 || !宿主基础节点 || !特征节点) {
            return false;
        }

        std::uintptr_t 旧值 = 0;
        const bool 有旧值 = 私有_读取指针特征(特征节点, 旧值);
        const std::uintptr_t 新值 = reinterpret_cast<std::uintptr_t>(新指针值);
        if (有旧值 && 旧值 == 新值) {
            return false;
        }

        if (!世界树.写入特征_指针(特征节点, 新指针值, now)) {
            return false;
        }

        状态节点类* 条件状态 = nullptr;
        if (有旧值) {
            条件状态 = 私有_创建状态节点(
                场景,
                宿主基础节点,
                特征节点,
                特征值{ 指针句柄{ 旧值 } },
                now,
                false,
                动作语义键,
                "治理前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ 指针句柄{ 新值 } },
            now,
            true,
            动作语义键,
            "治理后镜像");
        if (!结果状态) {
            return false;
        }
        if (!新值标题.empty()) {
            (void)世界树.状态().标记变化(结果状态, true, 动作语义键, 新值标题);
        }

        auto* 动态节点 = 世界树.动态().创建动态(场景, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 任务虚拟存在, 特征节点);
            if (来源方法 || 来源动作名覆盖) {
                const auto* 来源动作名 = 来源动作名覆盖
                    ? 来源动作名覆盖
                    : 来源方法->主信息.动作名;
                (void)世界树.动态().绑定来源方法(
                    动态节点,
                    reinterpret_cast<方法节点类*>(来源方法),
                    来源动作名,
                    来源相位);
            }
            (void)世界树.动态().绑定来源场景(动态节点, 场景, 场景);
        }

        auto* 因果节点 = 世界树.按动态创建因果信息(
            场景,
            动态节点,
            枚举_因果来源类型::执行生成,
            任务虚拟存在,
            特征节点,
            动作语义键,
            true);
        if (因果节点) {
            if (auto* 因果主信息 = 世界树.因果().取模板主信息(因果节点)) {
                因果主信息->名称 = 特征类型;
                因果主信息->类型 = 特征类型;
                因果主信息->置信度 = 10000;
            }
        }

        结果.已生成治理证据 = true;
        结果.最新状态 = 结果状态;
        结果.最新动态 = 动态节点;
        结果.最新抽象因果 = 因果节点;
        return true;
    }

    bool 私有_记录主体桥接证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::主体桥接);
        auto* 宿主任务 = 来源任务 ? 来源任务 : 自我对象.获取当前主任务();
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(
            自我对象,
            宿主任务,
            调用点 + "/任务虚拟存在");
        auto* 场景 = 私有_任务场景(自我对象, 宿主任务);
        auto* 特征节点 = 私有_确保主体桥接特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/特征");
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        if (!宿主任务 || !任务虚拟存在 || !场景 || !宿主基础节点 || !特征节点) {
            return false;
        }

        I64 旧值 = 0;
        const bool 有旧值 = 世界树.读取特征_I64(特征节点, 旧值);
        if (有旧值 && 旧值 == 新值) {
            return false;
        }

        if (!世界树.写入特征_I64(特征节点, 新值, now)) {
            return false;
        }

        状态节点类* 条件状态 = nullptr;
        if (有旧值) {
            条件状态 = 私有_创建状态节点(
                场景,
                宿主基础节点,
                特征节点,
                特征值{ 旧值 },
                now,
                false,
                动作语义键,
                "治理前镜像");
        }

        auto* 结果状态 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ 新值 },
            now,
            true,
            动作语义键,
            "治理后镜像");
        if (!结果状态) {
            return false;
        }

        auto* 动态节点 = 世界树.动态().创建动态(场景, 条件状态, 结果状态, now, now);
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 任务虚拟存在, 特征节点);
            if (来源方法) {
                (void)世界树.动态().绑定来源方法(
                    动态节点,
                    reinterpret_cast<方法节点类*>(来源方法),
                    来源方法->主信息.动作名,
                    枚举_动作事件相位::完成);
            }
            (void)世界树.动态().绑定来源场景(动态节点, 场景, 场景);
        }

        auto* 因果节点 = 世界树.按动态创建因果信息(
            场景,
            动态节点,
            枚举_因果来源类型::执行生成,
            任务虚拟存在,
            特征节点,
            动作语义键,
            true);
        if (因果节点) {
            if (auto* 因果主信息 = 世界树.因果().取模板主信息(因果节点)) {
                因果主信息->名称 = 特征类型;
                因果主信息->类型 = 特征类型;
                因果主信息->置信度 = 10000;
            }
        }

        结果.已生成治理证据 = true;
        结果.最新状态 = 结果状态;
        结果.最新动态 = 动态节点;
        结果.最新抽象因果 = 因果节点;
        return true;
    }

    bool 私有_记录主体桥接证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖,
        枚举_动作事件相位 来源相位) noexcept
    {
        const bool 调试M10回填 = 调用点.find("M10长期策略驱动回填") != std::string::npos;
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::主体桥接);
        auto* 宿主任务 = 来源任务 ? 来源任务 : 自我对象.获取当前主任务();
        if (调试M10回填) {
            std::ostringstream 主体桥接入口摘要;
            主体桥接入口摘要 << "任务管理/M10回填 | 主体桥接指针-入口"
                << " | 动作键=" << 动作语义键
                << " | 新值标题=" << 新值标题
                << " | 特征=" << (特征类型 ? 特征类型->获取主键() : std::string("空"))
                << " | 宿主指针=" << static_cast<const void*>(宿主任务)
                << " | 来源方法指针=" << static_cast<const void*>(来源方法)
                << " | 相位=" << static_cast<int>(来源相位);
            项目运行日志(主体桥接入口摘要.str());
        }
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(
            自我对象,
            宿主任务,
            调用点 + "/任务虚拟存在");
        auto* 场景 = 私有_任务场景(自我对象, 宿主任务);
        auto* 特征节点 = 私有_确保主体桥接特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/特征");
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 主体桥接指针-宿主与特征承载就绪");
        }
        if (!宿主任务 || !任务虚拟存在 || !场景 || !宿主基础节点 || !特征节点) {
            return false;
        }

        std::uintptr_t 旧值 = 0;
        const bool 有旧值 = 私有_读取指针特征(特征节点, 旧值);
        const std::uintptr_t 新值 = reinterpret_cast<std::uintptr_t>(新指针值);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 主体桥接指针-旧值读取完成");
        }
        if (有旧值 && 旧值 == 新值) {
            return false;
        }

        if (!世界树.写入特征_指针(特征节点, 新指针值, now)) {
            return false;
        }
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 主体桥接指针-特征写入完成");
        }

        状态节点类* 条件状态 = nullptr;
        if (有旧值) {
            条件状态 = 私有_创建状态节点(
                场景,
                宿主基础节点,
                特征节点,
                特征值{ 指针句柄{ 旧值 } },
                now,
                false,
                动作语义键,
                "治理前镜像");
        }
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 主体桥接指针-条件状态处理完成");
        }

        auto* 结果状态 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ 指针句柄{ 新值 } },
            now,
            true,
            动作语义键,
            "治理后镜像");
        if (!结果状态) {
            return false;
        }
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 主体桥接指针-结果状态创建完成");
        }
        if (!新值标题.empty()) {
            (void)世界树.状态().标记变化(结果状态, true, 动作语义键, 新值标题);
        }
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 主体桥接指针-结果状态标记完成");
        }

        auto* 动态节点 = 世界树.动态().创建动态(场景, 条件状态, 结果状态, now, now);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 主体桥接指针-动态创建完成");
        }
        if (动态节点) {
            auto* 动态主信息 = 世界树.动态().取动态主信息(动态节点);
            if (动态主信息) {
                动态主信息->名称 = 特征类型;
                动态主信息->类型 = 特征类型;
            }
            (void)世界树.动态().设置主体特征(动态节点, 任务虚拟存在, 特征节点);
            if (调试M10回填) {
                项目运行日志("任务管理/M10回填 | 主体桥接指针-动态主体特征绑定完成");
            }
            if (来源方法 || 来源动作名覆盖) {
                const auto* 来源动作名 = 来源动作名覆盖
                    ? 来源动作名覆盖
                    : 来源方法->主信息.动作名;
                if (调试M10回填) {
                    项目运行日志("任务管理/M10回填 | 主体桥接指针-准备绑定来源方法");
                }
                (void)世界树.动态().绑定来源方法(
                    动态节点,
                    reinterpret_cast<方法节点类*>(来源方法),
                    来源动作名,
                    来源相位);
                if (调试M10回填) {
                    项目运行日志("任务管理/M10回填 | 主体桥接指针-来源方法绑定完成");
                }
            }
            (void)世界树.动态().绑定来源场景(动态节点, 场景, 场景);
            if (调试M10回填) {
                项目运行日志("任务管理/M10回填 | 主体桥接指针-来源场景绑定完成");
            }
        }

        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 主体桥接指针-因果主信息组装完成");
        }

        auto* 因果节点 = 世界树.按动态创建因果信息(
            场景,
            动态节点,
            枚举_因果来源类型::执行生成,
            任务虚拟存在,
            特征节点,
            动作语义键,
            true);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 主体桥接指针-因果信息创建完成");
        }
        if (因果节点) {
            if (auto* 因果主信息 = 世界树.因果().取模板主信息(因果节点)) {
                因果主信息->名称 = 特征类型;
                因果主信息->类型 = 特征类型;
                因果主信息->置信度 = 10000;
            }
            if (调试M10回填) {
                项目运行日志("任务管理/M10回填 | 主体桥接指针-因果绑定完成");
            }
        }

        结果.已生成治理证据 = true;
        结果.最新状态 = 结果状态;
        结果.最新动态 = 动态节点;
        结果.最新抽象因果 = 因果节点;
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 主体桥接指针-写回完成");
        }
        return true;
    }

    bool 私有_按语义宿主分派记录治理证据_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        switch (私有_查询任务管理特征目标宿主(特征类型)) {
        case 枚举_任务管理特征目标宿主::宿主任务治理:
            return 私有_记录宿主任务治理证据_I64(
                自我对象, 特征类型, 新值, 动作语义键, 来源方法, 来源任务, now, 调用点, 结果);
        case 枚举_任务管理特征目标宿主::主体桥接:
            return 私有_记录主体桥接证据_I64(
                自我对象, 特征类型, 新值, 动作语义键, 来源方法, 来源任务, now, 调用点, 结果);
        case 枚举_任务管理特征目标宿主::方法资产:
            return 私有_记录方法资产证据_I64(
                自我对象, 特征类型, 新值, 动作语义键, 来源方法, 来源任务, now, 调用点, 结果);
        default:
            return false;
        }
    }

    bool 私有_按语义宿主分派记录治理证据_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖,
        枚举_动作事件相位 来源相位) noexcept
    {
        switch (私有_查询任务管理特征目标宿主(特征类型)) {
        case 枚举_任务管理特征目标宿主::宿主任务治理:
            return 私有_记录宿主任务治理证据_指针(
                自我对象,
                特征类型,
                新指针值,
                新值标题,
                动作语义键,
                来源方法,
                来源任务,
                now,
                调用点,
                结果,
                来源动作名覆盖,
                来源相位);
        case 枚举_任务管理特征目标宿主::主体桥接:
            return 私有_记录主体桥接证据_指针(
                自我对象,
                特征类型,
                新指针值,
                新值标题,
                动作语义键,
                来源方法,
                来源任务,
                now,
                调用点,
                结果,
                来源动作名覆盖,
                来源相位);
        default:
            return false;
        }
    }

    bool 私有_记录任务管理方法位专项动态(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        枚举_动作事件相位 相位) noexcept
    {
        const auto 方法位标题 = 私有_任务管理方法位标题(结果);
        if (方法位标题.empty()) {
            return false;
        }

        const std::string 事件标题 =
            方法位标题 + (相位 == 枚举_动作事件相位::开始运行 ? "开始" : "完成");
        const auto* 事件词 = 语素集.添加词性词(事件标题, "短语");
        if (!事件词) {
            return false;
        }

        auto* 原最新状态 = 结果.最新状态;
        auto* 原最新动态 = 结果.最新动态;
        auto* 原最新抽象因果 = 结果.最新抽象因果;
        const bool 原已生成治理证据 = 结果.已生成治理证据;
        auto* 当前方法 = 私有_P0_当前方法(结果);

        const bool 已记录 = 私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前方法位专项动态(),
            事件词,
            事件标题,
            "任务管理_方法位专项动态",
            当前方法,
            宿主任务,
            now,
            调用点,
            结果,
            事件词,
            相位);
        if (!已记录) {
            return false;
        }

        结果.当前需求位特征.当前方法位专项动态 = 结果.最新动态;
        结果.当前需求位特征.当前方法位专项动态标题 = 事件标题;
        私有_P0_刷新主体桥接字段(结果);
        结果.已生成治理证据 = 原已生成治理证据 || 已记录;
        结果.最新状态 = 原最新状态;
        结果.最新动态 = 原最新动态;
        结果.最新抽象因果 = 原最新抽象因果;
        return true;
    }

    bool 私有_记录最小原语边界动态(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        枚举_任务管理最小原语ID 原语ID,
        枚举_动作事件相位 相位) noexcept
    {
        const bool 调试M10回填 = 调用点.find("M10长期策略驱动回填") != std::string::npos;
        const auto* 签名 = 私有_查找最小原语签名(原语ID);
        if (!签名) {
            return false;
        }

        const std::string 事件标题 = std::string("原语_")
            + 签名->原语名
            + (相位 == 枚举_动作事件相位::开始运行 ? "_开始" : "_结束");
        const auto* 事件词 = 语素集.添加词性词(事件标题, "短语");
        if (!事件词) {
            return false;
        }
        if (调试M10回填) {
            std::ostringstream 原语边界摘要;
            原语边界摘要 << "任务管理/M10回填 | 原语边界-入口"
                << " | 原语ID=" << static_cast<int>(原语ID)
                << " | 相位=" << static_cast<int>(相位)
                << " | 标题=" << 事件标题;
            项目运行日志(原语边界摘要.str());
        }

        auto* 原最新状态 = 结果.最新状态;
        auto* 原最新动态 = 结果.最新动态;
        auto* 原最新抽象因果 = 结果.最新抽象因果;
        const bool 原已生成治理证据 = 结果.已生成治理证据;
        auto* 当前方法 = 私有_P0_当前方法(结果);

        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 原语边界-准备主体桥接指针写回");
        }
        const bool 已记录 = 私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前最小原语动态(),
            事件词,
            事件标题,
            "任务管理_最小原语动态",
            当前方法,
            宿主任务,
            now,
            调用点,
            结果,
            事件词,
            相位);
        if (!已记录) {
            return false;
        }
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 原语边界-主体桥接指针写回完成");
        }

        结果.当前本能能力缺口.当前最小原语动态 = 结果.最新动态;
        结果.当前本能能力缺口.当前最小原语动态标题 = 事件标题;
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 原语边界-准备刷新主体桥接字段");
        }
        私有_P0_刷新主体桥接字段(结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 原语边界-主体桥接字段刷新完成");
        }
        结果.已生成治理证据 = 原已生成治理证据 || 已记录;
        结果.最新状态 = 原最新状态;
        结果.最新动态 = 原最新动态;
        结果.最新抽象因果 = 原最新抽象因果;
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 原语边界-恢复最新证据完成");
        }
        return true;
    }

    bool 私有_执行最小原语边界记录体(void* 上下文) noexcept
    {
        auto* 执行上下文 = static_cast<结构_最小原语边界执行上下文*>(上下文);
        if (!执行上下文
            || !执行上下文->自我对象
            || !执行上下文->调用点
            || !执行上下文->结果) {
            return false;
        }

        return 私有_记录最小原语边界动态(
            *执行上下文->自我对象,
            执行上下文->宿主任务,
            执行上下文->当前时间,
            *执行上下文->调用点,
            *执行上下文->结果,
            执行上下文->原语ID,
            执行上下文->相位);
    }

    bool 私有_执行最小原语I64写回体(void* 上下文) noexcept
    {
        auto* 执行上下文 = static_cast<结构_最小原语I64写回执行上下文*>(上下文);
        if (!执行上下文
            || !执行上下文->自我对象
            || !执行上下文->动作语义键
            || !执行上下文->调用点
            || !执行上下文->结果) {
            return false;
        }

        switch (执行上下文->目标宿主) {
        case 枚举_最小原语写回目标宿主::宿主任务治理:
            return 私有_记录宿主任务治理证据_I64(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新值,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果);
        case 枚举_最小原语写回目标宿主::主体桥接:
            return 私有_记录主体桥接证据_I64(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新值,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果);
        case 枚举_最小原语写回目标宿主::方法资产:
            return 私有_记录方法资产证据_I64(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新值,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果);
        case 枚举_最小原语写回目标宿主::按语义宿主:
        default:
            return 私有_按语义宿主分派记录治理证据_I64(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新值,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果);
        }
    }

    bool 私有_执行最小原语指针写回体(void* 上下文) noexcept
    {
        auto* 执行上下文 = static_cast<结构_最小原语指针写回执行上下文*>(上下文);
        if (!执行上下文
            || !执行上下文->自我对象
            || !执行上下文->新值标题
            || !执行上下文->动作语义键
            || !执行上下文->调用点
            || !执行上下文->结果) {
            return false;
        }

        switch (执行上下文->目标宿主) {
        case 枚举_最小原语写回目标宿主::宿主任务治理:
            return 私有_记录宿主任务治理证据_指针(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新指针值,
                *执行上下文->新值标题,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果,
                执行上下文->来源动作名覆盖,
                执行上下文->来源相位);
        case 枚举_最小原语写回目标宿主::主体桥接:
            return 私有_记录主体桥接证据_指针(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新指针值,
                *执行上下文->新值标题,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果,
                执行上下文->来源动作名覆盖,
                执行上下文->来源相位);
        case 枚举_最小原语写回目标宿主::按语义宿主:
        default:
            return 私有_按语义宿主分派记录治理证据_指针(
                *执行上下文->自我对象,
                执行上下文->特征类型,
                执行上下文->新指针值,
                *执行上下文->新值标题,
                *执行上下文->动作语义键,
                执行上下文->来源方法,
                执行上下文->来源任务,
                执行上下文->当前时间,
                *执行上下文->调用点,
                *执行上下文->结果,
                执行上下文->来源动作名覆盖,
                执行上下文->来源相位);
        }
    }

    bool 私有_通过执行壳写回I64(
        自我类& 自我对象,
        枚举_最小原语写回目标宿主 目标宿主,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        结构_最小原语I64写回执行上下文 写回执行上下文{};
        写回执行上下文.自我对象 = &自我对象;
        写回执行上下文.目标宿主 = 目标宿主;
        写回执行上下文.特征类型 = 特征类型;
        写回执行上下文.新值 = 新值;
        写回执行上下文.动作语义键 = &动作语义键;
        写回执行上下文.来源方法 = 来源方法;
        写回执行上下文.来源任务 = 来源任务;
        写回执行上下文.当前时间 = now;
        写回执行上下文.调用点 = &调用点;
        写回执行上下文.结果 = &结果;
        return 任务管理最小原语模块::执行写入单特征当前值(
            &写回执行上下文,
            私有_执行最小原语I64写回体,
            调用点);
    }

    bool 私有_通过执行壳写回指针(
        自我类& 自我对象,
        枚举_最小原语写回目标宿主 目标宿主,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 词性节点类* 来源动作名覆盖,
        枚举_动作事件相位 来源相位) noexcept
    {
        结构_最小原语指针写回执行上下文 写回执行上下文{};
        写回执行上下文.自我对象 = &自我对象;
        写回执行上下文.目标宿主 = 目标宿主;
        写回执行上下文.特征类型 = 特征类型;
        写回执行上下文.新指针值 = 新指针值;
        写回执行上下文.新值标题 = &新值标题;
        写回执行上下文.动作语义键 = &动作语义键;
        写回执行上下文.来源方法 = 来源方法;
        写回执行上下文.来源任务 = 来源任务;
        写回执行上下文.当前时间 = now;
        写回执行上下文.调用点 = &调用点;
        写回执行上下文.结果 = &结果;
        写回执行上下文.来源动作名覆盖 = 来源动作名覆盖;
        写回执行上下文.来源相位 = 来源相位;
        return 任务管理最小原语模块::执行写入单挂点指针(
            &写回执行上下文,
            私有_执行最小原语指针写回体,
            调用点);
    }

    bool 私有_执行固定派生规则_I64写回(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        const bool 调试M10回填 = 调用点.find("M10长期策略驱动回填") != std::string::npos;
        const std::string 开始调用点 = 调用点 + "/开始";
        const std::string 写回调用点 = 调用点 + "/写回";
        const std::string 结束调用点 = 调用点 + "/结束";

        结构_最小原语边界执行上下文 开始执行上下文{};
        开始执行上下文.自我对象 = &自我对象;
        开始执行上下文.宿主任务 = 来源任务;
        开始执行上下文.当前时间 = now;
        开始执行上下文.调用点 = &开始调用点;
        开始执行上下文.结果 = &结果;
        开始执行上下文.原语ID = 枚举_任务管理最小原语ID::记录动作开始;
        开始执行上下文.相位 = 枚举_动作事件相位::开始运行;
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 固定派生I64-准备记录开始");
        }
        (void)任务管理最小原语模块::执行记录动作开始(
            &开始执行上下文,
            私有_执行最小原语边界记录体,
            开始调用点);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 固定派生I64-记录开始完成");
        }

        结构_最小原语I64写回执行上下文 写回执行上下文{};
        写回执行上下文.自我对象 = &自我对象;
        写回执行上下文.特征类型 = 特征类型;
        写回执行上下文.新值 = 新值;
        写回执行上下文.动作语义键 = &动作语义键;
        写回执行上下文.来源方法 = 来源方法;
        写回执行上下文.来源任务 = 来源任务;
        写回执行上下文.当前时间 = now;
        写回执行上下文.调用点 = &写回调用点;
        写回执行上下文.结果 = &结果;
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 固定派生I64-准备写值");
        }
        const bool 已变化 = 任务管理最小原语模块::执行写入单特征当前值(
            &写回执行上下文,
            私有_执行最小原语I64写回体,
            写回调用点);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 固定派生I64-写值完成");
        }

        结构_最小原语边界执行上下文 结束执行上下文{};
        结束执行上下文.自我对象 = &自我对象;
        结束执行上下文.宿主任务 = 来源任务;
        结束执行上下文.当前时间 = now;
        结束执行上下文.调用点 = &结束调用点;
        结束执行上下文.结果 = &结果;
        结束执行上下文.原语ID = 枚举_任务管理最小原语ID::记录动作结束;
        结束执行上下文.相位 = 枚举_动作事件相位::完成;
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 固定派生I64-准备记录结束");
        }
        (void)任务管理最小原语模块::执行记录动作结束(
            &结束执行上下文,
            私有_执行最小原语边界记录体,
            结束调用点);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 固定派生I64-记录结束完成");
        }
        return 已变化;
    }

    bool 私有_执行固定派生规则_指针写回(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        const std::string& 新值标题,
        const std::string& 动作语义键,
        方法节点* 来源方法,
        任务节点* 来源任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        const std::string 开始调用点 = 调用点 + "/开始";
        const std::string 写回调用点 = 调用点 + "/写回";
        const std::string 结束调用点 = 调用点 + "/结束";

        结构_最小原语边界执行上下文 开始执行上下文{};
        开始执行上下文.自我对象 = &自我对象;
        开始执行上下文.宿主任务 = 来源任务;
        开始执行上下文.当前时间 = now;
        开始执行上下文.调用点 = &开始调用点;
        开始执行上下文.结果 = &结果;
        开始执行上下文.原语ID = 枚举_任务管理最小原语ID::记录动作开始;
        开始执行上下文.相位 = 枚举_动作事件相位::开始运行;
        (void)任务管理最小原语模块::执行记录动作开始(
            &开始执行上下文,
            私有_执行最小原语边界记录体,
            开始调用点);

        结构_最小原语指针写回执行上下文 写回执行上下文{};
        写回执行上下文.自我对象 = &自我对象;
        写回执行上下文.特征类型 = 特征类型;
        写回执行上下文.新指针值 = 新指针值;
        写回执行上下文.新值标题 = &新值标题;
        写回执行上下文.动作语义键 = &动作语义键;
        写回执行上下文.来源方法 = 来源方法;
        写回执行上下文.来源任务 = 来源任务;
        写回执行上下文.当前时间 = now;
        写回执行上下文.调用点 = &写回调用点;
        写回执行上下文.结果 = &结果;
        const bool 已变化 = 任务管理最小原语模块::执行写入单挂点指针(
            &写回执行上下文,
            私有_执行最小原语指针写回体,
            写回调用点);

        结构_最小原语边界执行上下文 结束执行上下文{};
        结束执行上下文.自我对象 = &自我对象;
        结束执行上下文.宿主任务 = 来源任务;
        结束执行上下文.当前时间 = now;
        结束执行上下文.调用点 = &结束调用点;
        结束执行上下文.结果 = &结果;
        结束执行上下文.原语ID = 枚举_任务管理最小原语ID::记录动作结束;
        结束执行上下文.相位 = 枚举_动作事件相位::完成;
        (void)任务管理最小原语模块::执行记录动作结束(
            &结束执行上下文,
            私有_执行最小原语边界记录体,
            结束调用点);
        return 已变化;
    }

    void 私有_写回治理稳定字段(
        自我类& 自我对象,
        任务节点* 宿主任务,
        时间戳 now,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        const bool 调试M10回填 = 调用点.find("M10长期策略驱动回填") != std::string::npos;
        私有_P0_刷新主体桥接字段(结果);
        const auto 当前功能域 = 私有_P0_当前功能域(结果);
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前治理状态迁移 = 私有_P0_当前治理状态迁移(结果);
        const auto 当前方法来源 = 私有_P0_当前方法来源(结果);
        const auto 当前方法需求位 = 私有_P0_当前方法需求位(结果);
        const auto 当前本能能力缺口类型 = 私有_P0_当前本能缺口(结果);
        const auto 当前步骤位类型 = 私有_P0_当前步骤位类型(结果);
        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        const auto 当前后果治理 = 私有_P0_当前后果治理(结果);
        const auto 当前学习承接类型 = 私有_P0_当前学习承接类型(结果);
        const auto 最近功能域 =
            结果.最近功能域 != 枚举_任务管理功能域::未定义
            ? 结果.最近功能域
            : 当前功能域;
        const auto 最近总控结果 = 私有_P0_最近总控结果(结果);
        const auto 最近反馈类型 = 私有_P0_最近反馈类型(结果);
        const auto& 当前输入条件主键 = 结果.上下文.当前输入条件主键;
        const auto& 当前治理态型 = 结果.上下文.当前治理态型;
        auto* 当前步骤节点 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果节点 = 私有_P0_最新结果节点(结果);
        auto* 当前方法 = 私有_P0_当前方法(结果);
        if (调试M10回填) {
            std::ostringstream 稳定字段入口摘要;
            稳定字段入口摘要 << "任务管理/M10回填 | 稳定字段-入口"
                << " | 宿主指针=" << static_cast<const void*>(宿主任务)
                << " | 当前方法指针=" << static_cast<const void*>(当前方法)
                << " | 当前步骤指针=" << static_cast<const void*>(当前步骤节点)
                << " | 最近结果指针=" << static_cast<const void*>(最新结果节点)
                << " | 功能域=" << static_cast<int>(当前功能域)
                << " | 缺口=" << static_cast<int>(当前缺口类型)
                << " | 去向=" << static_cast<int>(当前下一步去向);
            项目运行日志(稳定字段入口摘要.str());
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前功能域(),
            static_cast<I64>(当前功能域),
            "任务管理_当前功能域",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前功能域",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前功能域完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前缺口类型(),
            static_cast<I64>(当前缺口类型),
            "任务管理_当前缺口类型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前缺口类型",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前缺口完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前下一步去向(),
            static_cast<I64>(当前下一步去向),
            "任务管理_当前下一步去向",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前下一步去向",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前去向完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前治理状态迁移(),
            static_cast<I64>(当前治理状态迁移),
            "任务管理_当前治理状态迁移",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前治理状态迁移",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前迁移完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前方法来源(),
            static_cast<I64>(当前方法来源),
            "任务管理_当前方法来源",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前方法来源",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前方法来源完成");
        }

        (void)私有_执行固定派生规则_I64写回(
            自我对象,
            私有_管理特征_当前方法需求位(),
            static_cast<I64>(当前方法需求位),
            "任务管理_当前方法需求位",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前方法需求位",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前方法需求位完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前本能能力缺口类型(),
            static_cast<I64>(当前本能能力缺口类型),
            "任务管理_当前本能能力缺口类型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前本能能力缺口类型",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前本能缺口完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前步骤位类型(),
            static_cast<I64>(当前步骤位类型),
            "任务管理_当前步骤位类型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前步骤位类型",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前步骤位完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前步骤语义类型(),
            static_cast<I64>(当前步骤语义类型),
            "任务管理_当前步骤语义类型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前步骤语义类型",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前步骤语义完成");
        }

        const bool 当前步骤可复用 =
            结果.当前需求位特征.当前步骤可复用
            || 结果.当前治理二次特征证据.当前步骤可复用;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前步骤可复用(),
            当前步骤可复用 ? 1 : 0,
            "任务管理_当前步骤可复用",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前步骤可复用",
            结果);

        const bool 最近结果命中当前治理 =
            结果.当前需求位特征.最近结果命中当前治理
            || 结果.当前治理二次特征证据.最近结果命中当前治理;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近结果命中当前治理(),
            最近结果命中当前治理 ? 1 : 0,
            "任务管理_最近结果命中当前治理",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近结果命中当前治理",
            结果);

        const bool 当前动作长时间无推进 =
            结果.当前需求位特征.当前动作长时间无推进
            || 结果.当前治理二次特征证据.当前动作长时间无推进;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前动作长时间无推进(),
            当前动作长时间无推进 ? 1 : 0,
            "任务管理_当前动作长时间无推进",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前动作长时间无推进",
            结果);

        const bool 当前动作需重试 =
            结果.当前需求位特征.当前动作需重试
            || 结果.当前治理二次特征证据.当前动作需重试;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前动作需重试(),
            当前动作需重试 ? 1 : 0,
            "任务管理_当前动作需重试",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前动作需重试",
            结果);

        const bool 当前需等待 =
            结果.当前需求位特征.当前需等待
            || 结果.当前治理二次特征证据.当前需等待;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前需等待(),
            当前需等待 ? 1 : 0,
            "任务管理_当前需等待",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前需等待",
            结果);

        const bool 当前需收束 =
            结果.当前需求位特征.当前需收束
            || 结果.当前治理二次特征证据.当前需收束;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前需收束(),
            当前需收束 ? 1 : 0,
            "任务管理_当前需收束",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前需收束",
            结果);

        const bool 可继续推进 =
            结果.当前需求位特征.可继续推进
            || 结果.当前治理二次特征证据.可继续推进;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_可继续推进(),
            可继续推进 ? 1 : 0,
            "任务管理_可继续推进",
            当前方法,
            宿主任务,
            now,
            调用点 + "/可继续推进",
            结果);

        const bool 当前可回接最近结果 =
            结果.当前需求位特征.当前可回接最近结果
            || 结果.当前治理二次特征证据.当前可回接最近结果;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前可回接最近结果(),
            当前可回接最近结果 ? 1 : 0,
            "任务管理_当前可回接最近结果",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前可回接最近结果",
            结果);

        const bool 稳定等待态复盘学习 =
            结果.当前需求位特征.稳定等待态复盘学习
            || 结果.当前治理二次特征证据.稳定等待态复盘学习;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_稳定等待态复盘学习(),
            稳定等待态复盘学习 ? 1 : 0,
            "任务管理_稳定等待态复盘学习",
            当前方法,
            宿主任务,
            now,
            调用点 + "/稳定等待态复盘学习",
            结果);

        const bool 业务子任务已完成 =
            结果.当前需求位特征.业务子任务已完成
            || 结果.当前治理二次特征证据.业务子任务已完成;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_业务子任务已完成(),
            业务子任务已完成 ? 1 : 0,
            "任务管理_业务子任务已完成",
            当前方法,
            宿主任务,
            now,
            调用点 + "/业务子任务已完成",
            结果);

        const bool 业务子任务应转执行 =
            结果.当前需求位特征.业务子任务应转执行
            || 结果.当前治理二次特征证据.业务子任务应转执行;
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_业务子任务应转执行(),
            业务子任务应转执行 ? 1 : 0,
            "任务管理_业务子任务应转执行",
            当前方法,
            宿主任务,
            now,
            调用点 + "/业务子任务应转执行",
            结果);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前宿主目标结果比较(),
            static_cast<I64>(结果.当前宿主目标结果比较),
            "任务管理_当前宿主目标结果比较",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前宿主目标结果比较",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-宿主目标结果比较完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前后果治理(),
            static_cast<I64>(当前后果治理),
            "任务管理_当前后果治理",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前后果治理",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前后果治理完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近根层重判结果(),
            static_cast<I64>(结果.最近根层重判结果),
            "任务管理_最近根层重判结果",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近根层重判结果",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-最近根层重判完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近执行前门控结果(),
            static_cast<I64>(结果.最近执行前门控结果),
            "任务管理_最近执行前门控结果",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近执行前门控结果",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-最近执行前门控完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近功能域(),
            static_cast<I64>(最近功能域),
            "任务管理_最近功能域",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近功能域",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-最近功能域完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近总控结果(),
            static_cast<I64>(最近总控结果),
            "任务管理_最近总控结果",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近总控结果",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-最近总控结果完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近反馈类型(),
            static_cast<I64>(最近反馈类型),
            "任务管理_最近反馈类型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近反馈类型",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-最近反馈类型完成");
        }

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前学习承接类型(),
            static_cast<I64>(当前学习承接类型),
            "任务管理_当前学习承接类型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前学习承接类型",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前学习承接类型完成");
        }

        const auto* 主体ID词 = 结果.当前主体ID == 0
            ? nullptr
            : 语素集.添加词性词(std::to_string(结果.当前主体ID), "数词");
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前主体ID(),
            主体ID词,
            结果.当前主体ID == 0 ? std::string{} : std::to_string(结果.当前主体ID),
            "任务管理_当前主体ID",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前主体ID",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前主体ID完成");
        }

        const auto* 分身ID词 = 结果.当前分身ID == 0
            ? nullptr
            : 语素集.添加词性词(std::to_string(结果.当前分身ID), "数词");
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前分身ID(),
            分身ID词,
            结果.当前分身ID == 0 ? std::string{} : std::to_string(结果.当前分身ID),
            "任务管理_当前分身ID",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前分身ID",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前分身ID完成");
        }

        const auto* 输入条件词 = 当前输入条件主键.empty()
            ? nullptr
            : 语素集.添加词性词(当前输入条件主键, "短语");
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前输入条件主键(),
            输入条件词,
            当前输入条件主键,
            "任务管理_当前输入条件主键",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前输入条件主键",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前输入条件主键完成");
        }

        const auto* 治理态型词 = 当前治理态型.empty()
            ? nullptr
            : 语素集.添加词性词(当前治理态型, "短语");
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::主体桥接,
            私有_管理特征_当前治理态型(),
            治理态型词,
            当前治理态型,
            "任务管理_当前治理态型",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前治理态型",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前治理态型完成");
        }

        (void)私有_执行固定派生规则_指针写回(
            自我对象,
            私有_管理特征_当前步骤指针(),
            当前步骤节点,
            私有_任务标题(当前步骤节点),
            "任务管理_当前步骤指针",
            当前方法,
            宿主任务,
            now,
            调用点 + "/当前步骤指针",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-当前步骤指针完成");
        }

        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_最近结果指针(),
            最新结果节点,
            私有_任务标题(最新结果节点),
            "任务管理_最近结果指针",
            当前方法,
            宿主任务,
            now,
            调用点 + "/最近结果指针",
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 稳定字段-最近结果指针完成");
        }
    }

    std::string 私有_治理摘要文本(const 结构_任务管理结果& 结果) noexcept
    {
        (void)结果;
        return {};
    }

    bool 私有_绑定宿主任务方法(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点,
        方法节点*& 输出方法) noexcept
    {
        输出方法 = nullptr;
        结果.当前方法来源 = 枚举_任务管理方法来源::未定义;
        if (!宿主任务) {
            return false;
        }

        auto& 任务主信息 = 宿主任务->主信息;
        if (任务主信息.执行方法.指针) {
            auto* 已挂方法 = reinterpret_cast<方法节点*>(任务主信息.执行方法.指针);
            if (已挂方法 && 私有_方法属于宿主任务(已挂方法, 宿主任务)) {
                输出方法 = 已挂方法;
                结果.当前方法来源 = 枚举_任务管理方法来源::已挂方法复用;
                if (私有_学习方法已具备可用骨架(已挂方法)) {
                    if (!自我对象.获取当前主方法()) {
                        (void)自我对象.设置当前主方法(输出方法, now, 调用点 + "/回填当前主方法");
                    }
                }
                return false;
            }

            任务主信息.执行方法 = {};
            任务主信息.动作句柄 = {};
        }

        if (auto* 当前方法 = 私有_P0_当前方法(结果);
            当前方法 && 私有_方法属于宿主任务(当前方法, 宿主任务)) {
            输出方法 = 当前方法;
            结果.当前方法来源 = 枚举_任务管理方法来源::当前主方法回填;
            if (私有_学习方法已具备可用骨架(当前方法)) {
                私有_完成方法挂接(
                    自我对象,
                    宿主任务,
                    当前方法,
                    now,
                    调用点 + "/当前主方法回填",
                    枚举_任务管理方法来源::当前主方法回填,
                    结果,
                    输出方法);
                return 输出方法 != nullptr;
            }
            return false;
        }

        if (auto* 长期策略方法 = 私有_查找主体长期策略方法候选(
            自我对象,
            宿主任务,
            结果)) {
            输出方法 = 长期策略方法;
            结果.当前方法来源 = 枚举_任务管理方法来源::主体长期策略回填;
            if (私有_学习方法已具备可用骨架(长期策略方法)) {
                私有_完成方法挂接(
                    自我对象,
                    宿主任务,
                    长期策略方法,
                    now,
                    调用点 + "/主体长期策略回填",
                    枚举_任务管理方法来源::主体长期策略回填,
                    结果,
                    输出方法);
                return 输出方法 != nullptr;
            }
            return false;
        }

        if (auto* 回看方法 = 私有_从最近结果回看方法(结果, 宿主任务)) {
            输出方法 = 回看方法;
            结果.当前方法来源 = 枚举_任务管理方法来源::最近结果回看;
            if (私有_学习方法已具备可用骨架(回看方法)) {
                私有_完成方法挂接(
                    自我对象,
                    宿主任务,
                    回看方法,
                    now,
                    调用点 + "/最近结果回看",
                    枚举_任务管理方法来源::最近结果回看,
                    结果,
                    输出方法);
                return 输出方法 != nullptr;
            }
            return false;
        }

        bool 使用了条件节点 = false;
        if (auto* 首节点候选 = 私有_查找宿主任务方法首节点候选(
            自我对象,
            宿主任务,
            结果,
            使用了条件节点)) {
            输出方法 = 首节点候选;
            结果.当前方法来源 =
                使用了条件节点
                ? 枚举_任务管理方法来源::条件节点筛选通过
                : 枚举_任务管理方法来源::方法树首节点候选;
            if (私有_学习方法已具备可用骨架(首节点候选)) {
                私有_完成方法挂接(
                    自我对象,
                    宿主任务,
                    首节点候选,
                    now,
                    调用点 + "/方法树首节点候选",
                    结果.当前方法来源,
                    结果,
                    输出方法);
                return 输出方法 != nullptr;
            }
            return false;
        }
        return false;
    }

    bool 私有_任务节点归属宿主任务(const 任务节点* 节点, const 任务节点* 宿主任务) noexcept
    {
        return 节点 && 宿主任务 && 私有_是祖先节点(宿主任务, 节点);
    }

    void 私有_按宿主清洗治理上下文镜像(结构_任务管理上下文& 上下文) noexcept
    {
        auto* 宿主任务 = 上下文.当前宿主任务;
        if (!宿主任务) {
            上下文.当前方法 = nullptr;
            上下文.当前步骤 = nullptr;
            上下文.最近结果 = nullptr;
            上下文.当前方法位专项动态 = nullptr;
            上下文.当前方法位专项动态标题.clear();
            上下文.当前最小原语动态 = nullptr;
            上下文.当前最小原语动态标题.clear();
            return;
        }

        auto* 已挂方法 = 宿主任务->主信息.执行方法.指针
            ? reinterpret_cast<方法节点*>(宿主任务->主信息.执行方法.指针)
            : nullptr;
        if (上下文.当前方法
            && !私有_方法属于宿主任务(上下文.当前方法, 宿主任务)
            && 上下文.当前方法 != 已挂方法) {
            上下文.当前方法 = nullptr;
        }

        if (上下文.当前步骤
            && (上下文.当前步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点
                || !私有_任务节点归属宿主任务(上下文.当前步骤, 宿主任务))) {
            上下文.当前步骤 = nullptr;
        }

        if (上下文.最近结果
            && (上下文.最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
                || !私有_任务节点归属宿主任务(上下文.最近结果, 宿主任务))) {
            上下文.最近结果 = nullptr;
        }

        auto* 最近结果父步骤 = 上下文.最近结果 && 上下文.最近结果->父
            ? static_cast<任务节点*>(上下文.最近结果->父)
            : nullptr;
        if (最近结果父步骤
            && (最近结果父步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点
                || !私有_任务节点归属宿主任务(最近结果父步骤, 宿主任务))) {
            上下文.最近结果 = nullptr;
            最近结果父步骤 = nullptr;
        }

        if (上下文.最近结果 && 最近结果父步骤) {
            if (!上下文.当前步骤 || 上下文.当前步骤 != 最近结果父步骤) {
                上下文.当前步骤 = 最近结果父步骤;
            }
        }

        if (!上下文.当前方法
            || !私有_方法属于宿主任务(上下文.当前方法, 宿主任务)
            || 上下文.当前方法需求位 == 枚举_任务管理方法需求位::未定义) {
            上下文.当前方法位专项动态 = nullptr;
            上下文.当前方法位专项动态标题.clear();
        }

        if (上下文.当前本能能力缺口类型 == 枚举_任务管理本能能力缺口类型::未定义) {
            上下文.当前最小原语动态 = nullptr;
            上下文.当前最小原语动态标题.clear();
        }
    }

    枚举_任务管理步骤位类型 私有_推导步骤位类型(
        const 结构_任务管理结果& 结果,
        bool 从最近结果回接) noexcept
    {
        const auto 当前功能域 = 私有_P0_当前功能域(结果);
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);

        if (当前功能域 == 枚举_任务管理功能域::等待
            || 当前下一步去向 == 枚举_任务管理下一步去向::保持等待) {
            return 枚举_任务管理步骤位类型::等待位;
        }
        if (当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
            || 当前下一步去向 == 枚举_任务管理下一步去向::绑定方法) {
            return 枚举_任务管理步骤位类型::补结构位;
        }
        if (从最近结果回接) {
            return 枚举_任务管理步骤位类型::回接位;
        }
        return 枚举_任务管理步骤位类型::执行位;
    }

    std::string 私有_步骤名称前缀文本(const 结构_任务管理结果& 结果) noexcept
    {
        switch (私有_P0_当前步骤语义类型(结果)) {
        case 枚举_任务管理步骤语义类型::等待维持步骤: return "等待位";
        case 枚举_任务管理步骤语义类型::回接派生步骤: return "业务回接位";
        case 枚举_任务管理步骤语义类型::补条件步骤: return "补条件位";
        case 枚举_任务管理步骤语义类型::补结构步骤: return "补结构位";
        case 枚举_任务管理步骤语义类型::业务推进步骤: return "业务执行位";
        case 枚举_任务管理步骤语义类型::治理步骤:
        default:
            return "治理步骤";
        }
    }

    std::string 私有_结果名称前缀文本(const 结构_任务管理结果& 结果) noexcept
    {
        switch (私有_P0_当前步骤语义类型(结果)) {
        case 枚举_任务管理步骤语义类型::等待维持步骤: return "等待结果";
        case 枚举_任务管理步骤语义类型::回接派生步骤: return "业务回接结果";
        case 枚举_任务管理步骤语义类型::补条件步骤: return "补条件结果";
        case 枚举_任务管理步骤语义类型::补结构步骤: return "补结构结果";
        case 枚举_任务管理步骤语义类型::业务推进步骤: return "业务执行结果";
        case 枚举_任务管理步骤语义类型::治理步骤:
        default:
            return "治理结果";
        }
    }

    bool 私有_任务状态已终结(枚举_任务状态 状态) noexcept
    {
        switch (状态) {
        case 枚举_任务状态::完成:
        case 枚举_任务状态::失败:
        case 枚举_任务状态::取消:
            return true;
        default:
            return false;
        }
    }

    const 词性节点类* 私有_治理步骤类型词(const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(私有_步骤名称前缀文本(结果), "名词");
    }

    const 词性节点类* 私有_治理结果类型词(const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(私有_结果名称前缀文本(结果), "名词");
    }

    std::string 私有_治理步骤键(const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << 私有_功能域文本(私有_P0_当前功能域(结果))
            << "_" << 私有_任务状态文本(结果.宿主任务新状态)
            << "_" << 私有_方法标题(私有_P0_当前方法(结果));
        return 输出.str();
    }

    std::string 私有_治理结果键(const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << 私有_功能域文本(私有_P0_当前功能域(结果))
            << "_" << 私有_缺口文本(私有_P0_当前缺口类型(结果))
            << "_" << 私有_去向文本(私有_P0_当前下一步去向(结果))
            << "_" << 私有_任务状态文本(结果.宿主任务新状态)
            << "_" << 私有_方法标题(私有_P0_当前方法(结果));
        return 输出.str();
    }

    const 词性节点类* 私有_治理步骤名称词(const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            私有_步骤名称前缀文本(结果) + "_" + 私有_治理步骤键(结果),
            "专有名词");
    }

    const 词性节点类* 私有_治理结果名称词(const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            私有_结果名称前缀文本(结果) + "_" + 私有_治理结果键(结果),
            "专有名词");
    }

    std::string 私有_补条件子任务键(const 任务节点* 宿主任务, const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << 私有_任务标题(宿主任务)
            << "_" << 私有_缺口文本(私有_P0_当前缺口类型(结果))
            << "_" << 私有_去向文本(私有_P0_当前下一步去向(结果));
        return 输出.str();
    }

    std::string 私有_M10_任务状态签名(const 状态节点类* 状态) noexcept
    {
        枚举_任务状态 状态值 = 枚举_任务状态::未定义;
        return 私有_读取状态节点任务状态(状态, 状态值)
            ? 私有_任务状态文本(状态值)
            : std::string("缺失");
    }

    std::string 私有_M10_业务差额签名(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        const auto* 宿主目标状态 = 宿主任务 ? 宿主任务->主信息.目标状态.指针 : 结果.宿主目标状态;
        const auto* 宿主结果状态 = 宿主任务 ? 宿主任务->主信息.结果状态信息.指针 : 结果.宿主结果状态;

        枚举_任务状态 目标值 = 枚举_任务状态::未定义;
        枚举_任务状态 结果值 = 枚举_任务状态::未定义;
        const bool 有目标 = 私有_读取状态节点任务状态(宿主目标状态, 目标值);
        const bool 有结果 = 私有_读取状态节点任务状态(宿主结果状态, 结果值);

        std::string 比较文本 = "待补齐";
        std::int32_t 对齐结果 = 0;
        if (二次特征本能判断模块::判断状态是否对齐(
                &世界树,
                宿主目标状态,
                宿主结果状态,
                对齐结果)) {
            比较文本 = 对齐结果 > 0 ? "已对齐" : "存在差额";
        }
        else if (有目标 && 有结果) {
            比较文本 = (目标值 == 结果值) ? "已对齐" : "存在差额";
        }

        std::ostringstream 输出;
        输出 << 比较文本
            << "|目标=" << (有目标 ? 私有_任务状态文本(目标值) : std::string("缺失"))
            << "|结果=" << (有结果 ? 私有_任务状态文本(结果值) : std::string("缺失"));
        return 输出.str();
    }

    std::string 私有_M10_当前条件签名(const 结构_任务管理结果& 结果) noexcept
    {
        const auto& 签名 = !结果.当前一步结果.当前输入条件主键.empty()
            ? 结果.当前一步结果.当前输入条件主键
            : (
                !结果.上下文.当前输入条件主键.empty()
                ? 结果.上下文.当前输入条件主键
                : (
                    !结果.当前单步决策.来源主观察特征.empty()
                    ? 结果.当前单步决策.来源主观察特征
                    : (
                        !结果.当前请求.来源主观察特征.empty()
                        ? 结果.当前请求.来源主观察特征
                        : 结果.来源主观察特征
                    )
                )
            );
        return 签名.empty() ? std::string("未定义条件") : 签名;
    }

    std::string 私有_M10_有效正式方法标题(const 结构_任务管理结果& 结果) noexcept
    {
        if (!结果.当前主体.最近正式方法标题.empty()) {
            return 结果.当前主体.最近正式方法标题;
        }
        return 私有_方法标题(私有_P0_当前方法(结果));
    }

    std::string 私有_M10_长期策略摘要版本(const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 原始;
        原始 << 结果.当前主体.最近正式方法标题
            << "|" << 结果.当前主体.最近长期策略摘要
            << "|" << 结果.当前主体.累计正式方法资产提交次数
            << "|" << 结果.当前主体.当前长期策略权重_Q10000
            << "|" << 结果.当前主体.当前长期策略降权级别;
        const auto 哈希值 = static_cast<unsigned long long>(std::hash<std::string>{}(原始.str()));
        return "v" + std::to_string(哈希值);
    }

    bool 私有_M10_存在业务推进压力(const 结构_任务管理结果& 结果) noexcept
    {
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        return 结果.宿主目标结果待推进
            || 当前缺口类型 == 枚举_任务管理缺口类型::任务运行中
            || 当前下一步去向 == 枚举_任务管理下一步去向::保持运行
            || 当前步骤语义类型 == 枚举_任务管理步骤语义类型::业务推进步骤
            || 当前步骤语义类型 == 枚举_任务管理步骤语义类型::回接派生步骤;
    }

    void 私有_M10_合并长期策略范围签名(
        std::string& 范围摘要,
        const std::string& 新签名) noexcept
    {
        if (新签名.empty() || 新签名 == "未定义条件" || 新签名 == "待补齐|目标=缺失|结果=缺失") {
            return;
        }

        std::vector<std::string> 片段{};
        std::string 当前{};
        for (char 字符 : 范围摘要) {
            if (字符 == ';') {
                if (!当前.empty()) {
                    片段.push_back(std::move(当前));
                    当前.clear();
                }
                continue;
            }
            当前.push_back(字符);
        }
        if (!当前.empty()) {
            片段.push_back(std::move(当前));
        }

        for (const auto& 片段值 : 片段) {
            if (片段值 == 新签名) {
                return;
            }
        }

        片段.push_back(新签名);
        while (片段.size() > 4) {
            片段.erase(片段.begin());
        }

        std::ostringstream 输出;
        for (std::size_t i = 0; i < 片段.size(); ++i) {
            if (i != 0) {
                输出 << ';';
            }
            输出 << 片段[i];
        }
        范围摘要 = 输出.str();
    }

    void 私有_M10_刷新长期策略权重(结构_任务管理主体虚拟存在& 主体) noexcept
    {
        if (主体.累计正式方法资产提交次数 == 0 || 主体.最近正式方法标题.empty()) {
            主体.当前长期策略权重_Q10000 = 0;
            主体.当前长期策略降权级别 = 0;
            return;
        }

        const auto 成功次数 = 主体.累计长期策略成功次数;
        const auto 失败次数 = 主体.累计长期策略失败次数;
        const auto 正式资产次数 = 主体.累计正式方法资产提交次数;

        int 权重 = 6000;
        权重 += static_cast<int>((成功次数 > 8 ? 8 : 成功次数) * 450);
        权重 += static_cast<int>((正式资产次数 > 8 ? 8 : 正式资产次数) * 180);
        权重 -= static_cast<int>((失败次数 > 8 ? 8 : 失败次数) * 700);
        if (失败次数 > 成功次数) {
            权重 -= static_cast<int>((失败次数 - 成功次数) * 300);
        }

        if (权重 < 1000) {
            权重 = 1000;
        }
        if (权重 > 10000) {
            权重 = 10000;
        }

        主体.当前长期策略权重_Q10000 = static_cast<std::uint32_t>(权重);
        主体.当前长期策略降权级别 =
            失败次数 == 0
            ? 0U
            : static_cast<std::uint32_t>(失败次数 > 9 ? 9 : 失败次数);
    }

    bool 私有_M10_当前方法命中主体长期策略(const 结构_任务管理结果& 结果) noexcept
    {
        if (结果.当前主体.累计正式方法资产提交次数 == 0
            || 结果.当前主体.最近正式方法标题.empty()) {
            return false;
        }

        const auto 目标正式方法标题 = 结果.当前主体.最近正式方法标题;
        if (auto* 当前方法 = 私有_P0_当前方法(结果);
            当前方法 && 私有_方法标题(当前方法) == 目标正式方法标题) {
            return true;
        }

        return false;
    }

    bool 私有_M10_长期策略可靠度允许回填(const 结构_任务管理主体虚拟存在& 主体) noexcept
    {
        if (主体.累计正式方法资产提交次数 == 0 || 主体.最近正式方法标题.empty()) {
            return false;
        }
        const auto 权重 = 主体.当前长期策略权重_Q10000 == 0
            ? 6000U
            : 主体.当前长期策略权重_Q10000;
        return 权重 >= 2500U;
    }

    bool 私有_M10_当前轮形成有效长期策略推进(const 结构_任务管理结果& 结果) noexcept
    {
        if (私有_M9_事件轮禁止业务扩张(结果)) {
            return false;
        }
        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        return 结果.已创建业务子任务
            || 结果.已复用业务子任务
            || 当前步骤语义类型 == 枚举_任务管理步骤语义类型::业务推进步骤
            || 当前步骤语义类型 == 枚举_任务管理步骤语义类型::回接派生步骤
            || 当前下一步去向 == 枚举_任务管理下一步去向::保持运行
            || 当前下一步去向 == 枚举_任务管理下一步去向::进入运行;
    }

    std::string 私有_M10_当前轮长期策略失效原因(const 结构_任务管理结果& 结果) noexcept
    {
        if (私有_M9_事件轮禁止业务扩张(结果)) {
            return "事件轮禁止扩张";
        }
        if (私有_P0_当前本能缺口(结果) != 枚举_任务管理本能能力缺口类型::无缺口) {
            return "桥接缺口未满足";
        }
        if (!私有_M10_存在业务推进压力(结果)) {
            return "宿主缺少业务推进压力";
        }
        if (私有_P0_当前下一步去向(结果) == 枚举_任务管理下一步去向::保持等待) {
            return "未形成有效推进";
        }
        return "未命中稳定业务推进";
    }

    bool 私有_M10_长期策略驱动业务子任务派生(const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_M10_存在业务推进压力(结果)
            && 私有_M10_当前方法命中主体长期策略(结果)
            && !私有_M10_有效正式方法标题(结果).empty();
    }

    std::string 私有_业务子任务键(const 任务节点* 宿主任务, const 结构_任务管理结果& 结果) noexcept
    {
        const auto 正式方法标题 = 私有_M10_有效正式方法标题(结果);
        const auto 业务差额签名 = 私有_M10_业务差额签名(宿主任务, 结果);
        const auto 目标状态签名 = 私有_M10_任务状态签名(
            宿主任务 ? 宿主任务->主信息.目标状态.指针 : 结果.宿主目标状态);
        const auto 长期策略摘要版本 = 私有_M10_长期策略摘要版本(结果);

        std::ostringstream 原始;
        原始 << (宿主任务 ? 宿主任务->获取主键() : std::string("空宿主"))
            << "|差额=" << 业务差额签名
            << "|方法=" << 正式方法标题
            << "|目标=" << 目标状态签名
            << "|策略=" << 长期策略摘要版本;

        std::ostringstream 输出;
        输出 << (宿主任务 ? 宿主任务->获取主键() : std::string("空宿主"))
            << "_"
            << static_cast<unsigned long long>(std::hash<std::string>{}(原始.str()));
        return 输出.str();
    }

    const 词性节点类* 私有_补条件子任务名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "补条件子任务_" + 私有_补条件子任务键(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_补条件子任务类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("补条件子任务", "名词");
        return s_词;
    }

    const 词性节点类* 私有_业务子任务名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "业务子任务_" + 私有_业务子任务键(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_业务子任务类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("业务子任务", "名词");
        return s_词;
    }

    const 词性节点类* 私有_任务管理学习树名称词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务管理学习树", "专有名词");
        return s_词;
    }

    const 词性节点类* 私有_任务管理学习树类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("学习树根节点", "名词");
        return s_词;
    }

    const 词性节点类* 私有_定向学习分支名称词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("定向学习分支", "专有名词");
        return s_词;
    }

    const 词性节点类* 私有_广泛学习分支名称词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("广泛学习分支", "专有名词");
        return s_词;
    }

    const 词性节点类* 私有_学习分支类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("学习分支", "名词");
        return s_词;
    }

    std::string 私有_学习目标方法标题(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        auto* 当前方法 = 私有_P0_当前方法(结果);
        if (当前方法
            && 当前方法->主信息.动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && 当前方法->主信息.动作句柄.本能ID == static_cast<I64>(枚举_本能方法ID::学习_尝试学习)) {
            当前方法 = nullptr;
        }

        if (当前方法) {
            const auto 标题 = 私有_方法标题(当前方法);
            if (!标题.empty() && 标题 != "空") {
                return 标题;
            }
        }

        if (auto* 当前学习方法 = 结果.当前学习方法节点) {
            const auto 标题 = 私有_方法标题(当前学习方法);
            if (!标题.empty() && 标题 != "空") {
                return 标题;
            }
        }

        std::ostringstream 输出;
        输出 << "待学习方法";
        if (宿主任务) {
            输出 << "_" << 私有_任务标题(宿主任务);
        }
        const auto 需求位 = 私有_P0_当前方法需求位(结果);
        if (需求位 != 枚举_任务管理方法需求位::未定义) {
            输出 << "_" << 私有_方法需求位文本(需求位);
        }
        const auto 本能缺口 = 私有_P0_当前本能缺口(结果);
        if (本能缺口 != 枚举_任务管理本能能力缺口类型::未定义) {
            输出 << "_" << 私有_本能能力缺口文本(本能缺口);
        }
        return 输出.str();
    }

    std::string 私有_学习子任务键(const 任务节点* 宿主任务, const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << 私有_任务标题(宿主任务)
            << "_" << 私有_学习通道文本(结果)
            << "_" << 私有_缺口文本(私有_P0_当前缺口类型(结果))
            << "_" << 私有_去向文本(私有_P0_当前下一步去向(结果))
            << "_" << 私有_方法需求位文本(私有_P0_当前方法需求位(结果))
            << "_" << 私有_本能能力缺口文本(私有_P0_当前本能缺口(结果))
            << "_" << 私有_学习目标方法标题(宿主任务, 结果);
        return 输出.str();
    }

    std::string 私有_广泛学习子任务键(
        const 结构_学习承接决策& 决策,
        const 结构_任务管理结果& 结果) noexcept
    {
        std::ostringstream 输出;
        输出 << 私有_任务标题(决策.来源根任务)
            << "_" << 私有_学习承接类型文本(决策.学习类型)
            << "_" << 私有_功能域文本(决策.来源功能域)
            << "_" << 私有_方法需求位文本(决策.来源方法需求位)
            << "_" << 私有_学习通道文本(结果)
            << "_" << 私有_学习目标方法标题(决策.来源父任务, 结果);
        return 输出.str();
    }

    const 词性节点类* 私有_学习子任务名称词(
        枚举_任务管理学习承接类型 学习类型,
        const 任务节点* 宿主任务,
        const 结构_学习承接决策& 决策,
        const 结构_任务管理结果& 结果) noexcept
    {
        if (学习类型 == 枚举_任务管理学习承接类型::广泛学习) {
            return 语素集.添加词性词(
                "任务管理广泛学习子任务_" + 私有_广泛学习子任务键(决策, 结果),
                "专有名词");
        }
        return 语素集.添加词性词(
            "学习子任务_" + 私有_学习子任务键(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_学习子任务类型词(
        枚举_任务管理学习承接类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_任务管理学习承接类型::广泛学习: {
            static const 词性节点类* s_词 = 语素集.添加词性词("任务管理广泛学习子任务", "名词");
            return s_词;
        }
        case 枚举_任务管理学习承接类型::定向学习:
        case 枚举_任务管理学习承接类型::未定义:
        default: {
            static const 词性节点类* s_词 = 语素集.添加词性词("学习子任务", "名词");
            return s_词;
        }
        }
    }

    bool 私有_学习子任务类型匹配(const 词性节点类* 类型词) noexcept
    {
        return 私有_词性相同(类型词, 私有_学习子任务类型词(枚举_任务管理学习承接类型::定向学习))
            || 私有_词性相同(类型词, 私有_学习子任务类型词(枚举_任务管理学习承接类型::广泛学习));
    }

    const 词性节点类* 私有_方法虚拟存在类型词() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法虚拟存在", "名词");
        return s_词;
    }

    bool 私有_应新生学习方法头(const 结构_任务管理结果& 结果) noexcept
    {
        const auto* 当前方法 = 私有_P0_当前方法(结果);
        return !当前方法
            || 私有_P0_当前方法来源(结果) == 枚举_任务管理方法来源::默认本能兜底;
    }

    std::string 私有_学习方法动作名文本(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        auto* 当前方法 = 私有_P0_当前方法(结果);
        if (当前方法
            && 当前方法->主信息.动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && 当前方法->主信息.动作句柄.本能ID == static_cast<I64>(枚举_本能方法ID::学习_尝试学习)) {
            当前方法 = nullptr;
        }
        if (私有_应新生学习方法头(结果)) {
            if (当前方法) {
                return "候选_" + 私有_方法标题(当前方法) + "_" + 私有_任务标题(宿主任务);
            }
            return "待学习动作_" + 私有_学习目标方法标题(宿主任务, 结果);
        }
        return 当前方法
            ? 私有_方法标题(当前方法)
            : ("待学习动作_" + 私有_学习目标方法标题(宿主任务, 结果));
    }

    const 词性节点类* 私有_学习方法动作名词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        auto* 当前方法 = 私有_P0_当前方法(结果);
        if (当前方法
            && 当前方法->主信息.动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && 当前方法->主信息.动作句柄.本能ID == static_cast<I64>(枚举_本能方法ID::学习_尝试学习)) {
            当前方法 = nullptr;
        }
        if (!私有_应新生学习方法头(结果) && 当前方法 && 当前方法->主信息.动作名) {
            return 当前方法->主信息.动作名;
        }
        return 语素集.添加词性词(私有_学习方法动作名文本(宿主任务, 结果), "动词");
    }

    const 词性节点类* 私有_学习方法虚拟存在名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习方法_" + 私有_学习方法动作名文本(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_学习方法条件场景名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习方法条件场景_" + 私有_学习方法动作名文本(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_学习方法结果场景名称词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习方法结果场景_" + 私有_学习方法动作名文本(宿主任务, 结果),
            "专有名词");
    }

    const 词性节点类* 私有_学习方法条件节点动作名词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习条件_" + 私有_任务标题(宿主任务) + "_" + 私有_缺口文本(私有_P0_当前缺口类型(结果)),
            "动词");
    }

    const 词性节点类* 私有_学习方法结果节点动作名词(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 语素集.添加词性词(
            "学习结果_" + 私有_任务标题(宿主任务) + "_" + 私有_去向文本(私有_P0_当前下一步去向(结果)),
            "动词");
    }

    bool 私有_任务为业务子任务(const 任务节点* 任务) noexcept
    {
        return 任务 && 私有_词性相同(任务->主信息.类型, 私有_业务子任务类型词());
    }

    枚举_任务状态 私有_治理结果状态(const 结构_任务管理结果& 结果) noexcept
    {
        if (!私有_P0_当前宿主任务(结果)) {
            return 枚举_任务状态::失败;
        }
        if (!私有_P0_当前方法(结果)) {
            return 枚举_任务状态::挂起;
        }

        switch (结果.宿主任务新状态) {
        case 枚举_任务状态::完成:
            return 枚举_任务状态::完成;
        case 枚举_任务状态::失败:
            return 枚举_任务状态::失败;
        case 枚举_任务状态::取消:
            return 枚举_任务状态::取消;
        case 枚举_任务状态::挂起:
            return 枚举_任务状态::挂起;
        default:
            return 枚举_任务状态::完成;
        }
    }

    场景节点类* 私有_任务场景(自我类& 自我对象, const 任务节点* 节点) noexcept
    {
        if (节点 && 节点->主信息.场景.指针) {
            return 节点->主信息.场景.指针;
        }
        if (auto* 现实场景 = 自我对象.获取自我现实场景()) {
            return 现实场景;
        }
        return 自我对象.获取自我内部世界();
    }

    存在节点类* 私有_确保任务虚拟存在(
        自我类& 自我对象,
        任务节点* 任务,
        const std::string& 调用点) noexcept
    {
        (void)调用点;
        if (!任务) {
            return nullptr;
        }

        if (任务->主信息.任务虚拟存在.指针) {
            return 任务->主信息.任务虚拟存在.指针;
        }

        auto* 宿主场景 = 私有_任务场景(自我对象, 任务);
        if (!宿主场景) {
            return nullptr;
        }

        auto* 任务虚拟存在 = 世界树.存在().取或创建子存在_按名称类型(
            宿主场景,
            私有_任务虚拟存在名称词(任务),
            私有_任务虚拟存在类型词());
        if (!任务虚拟存在) {
            return nullptr;
        }

        私有_写入引用(任务->主信息.任务虚拟存在, 任务虚拟存在);
        return 任务虚拟存在;
    }

    const 特征节点类* 私有_查找宿主任务治理特征节点(
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型) noexcept
    {
        if (!宿主任务 || !特征类型 || !宿主任务->主信息.任务虚拟存在.指针) {
            return nullptr;
        }
        const auto* 宿主基础节点 =
            reinterpret_cast<const 基础信息节点类*>(宿主任务->主信息.任务虚拟存在.指针);
        return 宿主基础节点
            ? 世界树.存在().查找子特征_按类型(宿主基础节点, 特征类型)
            : nullptr;
    }

    特征节点类* 私有_确保宿主任务治理特征节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务 || !特征类型) {
            return nullptr;
        }
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(自我对象, 宿主任务, 调用点);
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        return 宿主基础节点
            ? 世界树.取或创建子特征_按类型(宿主基础节点, 特征类型)
            : nullptr;
    }

    const 特征节点类* 私有_查找主体桥接特征节点(
        const 任务节点* 宿主任务,
        const 词性节点类* 特征类型) noexcept
    {
        return 私有_查找宿主任务治理特征节点(宿主任务, 特征类型);
    }

    特征节点类* 私有_确保主体桥接特征节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        // P3 过渡期先复用宿主任务虚拟存在承载桥接字段，P4 再正式拆出主体/分身存在。
        return 私有_确保宿主任务治理特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点);
    }

    const 特征节点类* 私有_查找方法资产特征节点(
        const 方法节点* 方法头,
        const 词性节点类* 特征类型) noexcept
    {
        if (!方法头 || !特征类型 || !方法头->主信息.方法虚拟存在.指针) {
            return nullptr;
        }
        const auto* 宿主基础节点 =
            reinterpret_cast<const 基础信息节点类*>(方法头->主信息.方法虚拟存在.指针);
        return 宿主基础节点
            ? 世界树.存在().查找子特征_按类型(宿主基础节点, 特征类型)
            : nullptr;
    }

    特征节点类* 私有_确保方法资产特征节点(
        方法节点* 方法头,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        (void)调用点;
        if (!方法头 || !特征类型 || !方法头->主信息.方法虚拟存在.指针) {
            return nullptr;
        }
        auto* 宿主基础节点 =
            reinterpret_cast<基础信息节点类*>(方法头->主信息.方法虚拟存在.指针);
        return 宿主基础节点
            ? 世界树.取或创建子特征_按类型(宿主基础节点, 特征类型)
            : nullptr;
    }

    bool 私有_状态节点值匹配任务状态(const 状态节点类* 状态节点, 枚举_任务状态 状态) noexcept
    {
        const auto* 主信息 = 世界树.状态().取状态主信息(状态节点);
        if (!主信息) {
            return false;
        }

        const auto* 当前值 = std::get_if<I64>(&主信息->状态值);
        return 当前值 && (*当前值 == static_cast<I64>(状态));
    }

    bool 私有_读取状态节点任务状态(const 状态节点类* 状态节点, 枚举_任务状态& 输出) noexcept
    {
        const auto* 主信息 = 世界树.状态().取状态主信息(状态节点);
        if (!主信息) {
            return false;
        }

        const auto* 当前值 = std::get_if<I64>(&主信息->状态值);
        if (!当前值) {
            return false;
        }

        输出 = static_cast<枚举_任务状态>(*当前值);
        return true;
    }

    bool 私有_任务节点目标结果已对齐(const 任务节点* 任务) noexcept
    {
        if (!任务) {
            return false;
        }

        std::int32_t 对齐结果 = 0;
        return 二次特征本能判断模块::判断状态是否对齐(
            &世界树,
            任务->主信息.目标状态.指针,
            任务->主信息.结果状态信息.指针,
            对齐结果)
            && 对齐结果 > 0;
    }

    bool 私有_任务节点目标结果待推进(const 任务节点* 任务) noexcept
    {
        if (!任务) {
            return false;
        }

        std::int32_t 对齐结果 = 0;
        return 二次特征本能判断模块::判断状态是否对齐(
            &世界树,
            任务->主信息.目标状态.指针,
            任务->主信息.结果状态信息.指针,
            对齐结果)
            && 对齐结果 < 0;
    }

    const char* 私有_P0_原语候选比较基准类型文本(枚举_原语候选比较基准类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_原语候选比较基准类型::状态结果特征: return "状态结果特征";
        case 枚举_原语候选比较基准类型::方法结果特征: return "方法结果特征";
        case 枚举_原语候选比较基准类型::结果方向二次特征: return "结果方向二次特征";
        default: return "未定义";
        }
    }

    const char* 私有_P0_原语候选来源锚点类型文本(枚举_原语候选来源锚点类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_原语候选来源锚点类型::步骤结果锚点: return "步骤结果锚点";
        case 枚举_原语候选来源锚点类型::宿主最近实际结果锚点: return "宿主最近实际结果锚点";
        case 枚举_原语候选来源锚点类型::方法结果锚点: return "方法结果锚点";
        case 枚举_原语候选来源锚点类型::当前最近结果锚点: return "当前最近结果锚点";
        case 枚举_原语候选来源锚点类型::命中预测结果锚点: return "命中预测结果锚点";
        case 枚举_原语候选来源锚点类型::方法结果链锚点: return "方法结果链锚点";
        default: return "未定义";
        }
    }

    std::string 私有_P0_原语候选标题(const 结构_原语候选项& 候选) noexcept
    {
        if (候选.节点) {
            return 私有_任务标题(候选.节点);
        }
        if (候选.方法节点) {
            return 私有_方法标题(候选.方法节点);
        }
        return 候选.候选主键.empty() ? std::string("空") : 候选.候选主键;
    }

    方法节点* 私有_P0_归一到方法首节点(方法节点* 节点) noexcept
    {
        auto* 当前 = 节点;
        while (当前) {
            if (当前->主信息.节点种类 == 枚举_方法节点种类::方法首节点) {
                return 当前;
            }
            当前 = 当前->主信息.父方法.指针
                ? reinterpret_cast<方法节点*>(当前->主信息.父方法.指针)
                : nullptr;
        }
        return nullptr;
    }

    const 结构_原语候选项* 私有_P0_查找升阶候选_按主键(
        const 结构_任务管理请求& 请求,
        const std::string& 候选主键) noexcept
    {
        if (候选主键.empty()) {
            return nullptr;
        }

        const auto 已有 = std::find_if(
            请求.当前升阶候选组.begin(),
            请求.当前升阶候选组.end(),
            [&](const 结构_原语候选项& 候选) noexcept {
                return !候选.候选主键.empty() && 候选.候选主键 == 候选主键;
            });
        return 已有 == 请求.当前升阶候选组.end() ? nullptr : &(*已有);
    }

    bool 私有_P0_解析高阶胜出绑定(
        const 结构_任务管理请求& 请求,
        const 结构_高阶排序结果& 高阶排序结果,
        结构_高阶胜出绑定& 输出) noexcept
    {
        输出 = {};
        输出.需求轴键 = !高阶排序结果.需求轴键.empty()
            ? 高阶排序结果.需求轴键
            : 请求.当前需求轴键;
        输出.胜出候选主键 = 高阶排序结果.胜出候选主键;

        if (高阶排序结果.胜出候选主键.empty()) {
            输出.未绑定原因 = "高阶排序尚未产出胜出候选";
            输出.绑定摘要 = "高阶排序未形成显式胜出绑定";
            return false;
        }

        const auto* 候选 = 私有_P0_查找升阶候选_按主键(请求, 高阶排序结果.胜出候选主键);
        if (!候选) {
            输出.未绑定原因 = "胜出候选未命中当前请求候选集";
            输出.绑定摘要 = "高阶胜出候选缺少请求内锚点，暂未绑定执行方法";
            return false;
        }

        输出.来源锚点类型 = 候选->来源锚点类型;
        输出.胜出任务结果节点 = 候选->节点;
        输出.胜出方法结果节点 = 候选->方法节点;

        if (!输出.胜出方法结果节点
            && 候选->节点
            && 候选->节点->主信息.对应方法结果节点.指针) {
            输出.胜出方法结果节点 =
                reinterpret_cast<方法节点*>(候选->节点->主信息.对应方法结果节点.指针);
        }

        if (候选->节点 && 候选->节点->主信息.执行方法.指针) {
            输出.胜出方法节点 = 私有_P0_归一到方法首节点(
                reinterpret_cast<方法节点*>(候选->节点->主信息.执行方法.指针));
        }
        if (!输出.胜出方法节点 && 输出.胜出方法结果节点) {
            输出.胜出方法节点 = 私有_P0_归一到方法首节点(输出.胜出方法结果节点);
        }

        if (输出.胜出方法节点) {
            输出.胜出方法主键 = 输出.胜出方法节点->获取主键();
            输出.已绑定执行方法 = true;
            std::ostringstream 摘要;
            摘要 << 私有_P0_原语候选标题(*候选)
                << " 胜出并绑定到执行方法 "
                << 私有_方法标题(输出.胜出方法节点);
            输出.绑定摘要 = 摘要.str();
            return true;
        }

        输出.未绑定原因 = "胜出候选未解析出可执行方法";
        std::ostringstream 摘要;
        摘要 << 私有_P0_原语候选标题(*候选)
            << " 胜出，但当前缺少可执行方法锚点";
        输出.绑定摘要 = 摘要.str();
        return false;
    }

    std::string 私有_P0_取状态比较特征键(const 状态节点类* 状态节点) noexcept
    {
        if (!状态节点) {
            return {};
        }

        const auto* 主信息 = 世界树.状态().取状态主信息(状态节点);
        if (!主信息) {
            return {};
        }

        const auto* 状态特征 = 主信息->状态特征.指针;
        const auto* 特征主信息 = 状态特征
            ? dynamic_cast<const 特征节点主信息类*>(状态特征->主信息)
            : nullptr;
        if (特征主信息 && 特征主信息->类型 && !特征主信息->类型->获取主键().empty()) {
            return 特征主信息->类型->获取主键();
        }
        return 状态特征 ? 状态特征->获取主键() : std::string{};
    }

    std::string 私有_P0_取方法结果比较特征键(const 方法节点* 方法结果节点) noexcept
    {
        if (!方法结果节点) {
            return {};
        }

        if (const auto* 初始状态 = 私有_取首个有效状态(方法结果节点->主信息.结果初始状态)) {
            const auto 状态特征键 = 私有_P0_取状态比较特征键(初始状态);
            if (!状态特征键.empty()) {
                return 状态特征键;
            }
        }

        if (方法结果节点->主信息.主结果特征类型 && !方法结果节点->主信息.主结果特征类型->获取主键().empty()) {
            return 方法结果节点->主信息.主结果特征类型->获取主键();
        }

        return 方法结果节点->主信息.结果主键;
    }

    时间戳 私有_P0_取任务结果时间戳(const 任务节点* 节点) noexcept
    {
        if (!节点) {
            return 0;
        }
        if (节点->主信息.完成时间 != 0) {
            return 节点->主信息.完成时间;
        }
        if (节点->主信息.启动时间 != 0) {
            return 节点->主信息.启动时间;
        }
        return 节点->主信息.创建时间;
    }

    时间戳 私有_P0_取方法结果时间戳(const 方法节点* 节点) noexcept
    {
        if (!节点) {
            return 0;
        }
        if (节点->主信息.最近完成时间 != 0) {
            return 节点->主信息.最近完成时间;
        }
        if (节点->主信息.最近执行时间 != 0) {
            return 节点->主信息.最近执行时间;
        }
        if (节点->主信息.时间止 != 0) {
            return 节点->主信息.时间止;
        }
        return 节点->主信息.时间起;
    }

    任务节点* 私有_P0_取步骤结果锚点(const 结构_任务管理请求& 请求) noexcept
    {
        auto* 当前步骤 = 请求.上下文.当前步骤;
        if (!当前步骤) {
            return nullptr;
        }

        if (auto* 步骤结果 = reinterpret_cast<任务节点*>(当前步骤->主信息.后续任务.指针)) {
            return 步骤结果;
        }

        auto* 最近结果 = 请求.上下文.最近结果;
        if (最近结果
            && reinterpret_cast<任务节点*>(最近结果->主信息.来源步骤节点.指针) == 当前步骤) {
            return 最近结果;
        }
        return nullptr;
    }

    任务节点* 私有_P0_取当前最近结果锚点(const 结构_任务管理请求& 请求) noexcept
    {
        auto* 最近结果 = 请求.上下文.最近结果;
        if (!最近结果 || 最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点) {
            return nullptr;
        }
        return 最近结果;
    }

    任务节点* 私有_P0_取宿主最近实际结果锚点(const 结构_任务管理请求& 请求) noexcept
    {
        if (!请求.宿主任务) {
            return nullptr;
        }
        return reinterpret_cast<任务节点*>(请求.宿主任务->主信息.最近实际结果节点.指针);
    }

    任务节点* 私有_P0_取命中预测结果锚点(const 结构_任务管理请求& 请求) noexcept
    {
        auto* 最近结果 = 私有_P0_取当前最近结果锚点(请求);
        if (!最近结果) {
            return nullptr;
        }

        if (auto* 命中预测 = reinterpret_cast<任务节点*>(最近结果->主信息.命中预测结果节点.指针)) {
            return 命中预测;
        }

        auto* 来源步骤 = reinterpret_cast<任务节点*>(最近结果->主信息.来源步骤节点.指针);
        if (!来源步骤) {
            return nullptr;
        }

        if (auto* 步骤结果 = reinterpret_cast<任务节点*>(来源步骤->主信息.后续任务.指针)) {
            return 步骤结果;
        }
        return nullptr;
    }

    方法节点* 私有_P0_取方法结果锚点(const 结构_任务管理请求& 请求) noexcept
    {
        const auto 取结果方法 = [](const 任务节点* 结果节点) noexcept -> 方法节点* {
            return 结果节点
                ? reinterpret_cast<方法节点*>(结果节点->主信息.对应方法结果节点.指针)
                : nullptr;
        };

        if (auto* 方法结果 = 取结果方法(私有_P0_取步骤结果锚点(请求))) {
            return 方法结果;
        }
        if (auto* 方法结果 = 取结果方法(私有_P0_取宿主最近实际结果锚点(请求))) {
            return 方法结果;
        }
        if (auto* 方法结果 = 取结果方法(请求.上下文.最近结果)) {
            return 方法结果;
        }

        auto* 当前方法 = 请求.上下文.当前方法;
        if (!当前方法) {
            return nullptr;
        }
        if (当前方法->主信息.节点种类 == 枚举_方法节点种类::方法结果节点) {
            return 当前方法;
        }

        return 私有_查找直接子节点_按谓词<方法节点>(当前方法, [&](const 方法节点* 节点) noexcept {
            return 节点 && 节点->主信息.节点种类 == 枚举_方法节点种类::方法结果节点;
        });
    }

    状态节点类* 私有_P0_取候选结果状态(const 结构_原语候选项& 候选) noexcept
    {
        if (候选.节点) {
            return 候选.节点->主信息.结果状态信息.指针;
        }
        if (候选.方法节点) {
            return 私有_取首个有效状态(候选.方法节点->主信息.结果初始状态);
        }
        return nullptr;
    }

    bool 私有_P0_判断候选相对需求轴方向(
        const 结构_任务管理请求& 请求,
        const 状态节点类* 候选结果状态,
        std::int32_t& 输出关系) noexcept
    {
        输出关系 = 0;
        auto* 宿主任务 = 请求.宿主任务 ? 请求.宿主任务 : 请求.上下文.当前宿主任务;
        if (!宿主任务 || !候选结果状态) {
            return false;
        }

        const auto* 当前状态 = 宿主任务->主信息.结果状态信息.指针;
        const auto* 目标状态 = 宿主任务->主信息.目标状态.指针;
        if (!当前状态 || !目标状态) {
            return false;
        }

        std::int32_t 需求方向 = 0;
        std::int32_t 候选方向 = 0;
        if (!二次特征本能判断模块::判断状态结果方向(
                &世界树,
                当前状态,
                目标状态,
                需求方向)
            || !二次特征本能判断模块::判断状态结果方向(
                &世界树,
                当前状态,
                候选结果状态,
                候选方向)) {
            return false;
        }

        if (需求方向 == 0 || 候选方向 == 0) {
            输出关系 = 0;
            return true;
        }

        输出关系 = 需求方向 == 候选方向 ? 1 : -1;
        return true;
    }

    bool 私有_P0_读取候选相对目标距离(
        const 结构_任务管理请求& 请求,
        const 结构_原语候选项& 候选,
        std::int64_t& 输出距离,
        bool& 输出是否过头) noexcept
    {
        输出距离 = 0;
        输出是否过头 = false;

        auto* 宿主任务 = 请求.宿主任务 ? 请求.宿主任务 : 请求.上下文.当前宿主任务;
        const auto* 候选结果状态 = 私有_P0_取候选结果状态(候选);
        if (!宿主任务 || !候选结果状态) {
            return false;
        }

        const auto* 当前状态 = 宿主任务->主信息.结果状态信息.指针;
        const auto* 目标状态 = 宿主任务->主信息.目标状态.指针;
        if (!当前状态 || !目标状态) {
            return false;
        }

        const auto* 目标主信息 = 世界树.状态().取状态主信息(目标状态);
        const auto* 候选主信息 = 世界树.状态().取状态主信息(候选结果状态);
        if (!目标主信息 || !候选主信息) {
            return false;
        }

        const auto* 目标特征主信息 = 目标主信息->状态特征.指针
            ? dynamic_cast<const 特征节点主信息类*>(目标主信息->状态特征.指针->主信息)
            : nullptr;
        const auto* 候选特征主信息 = 候选主信息->状态特征.指针
            ? dynamic_cast<const 特征节点主信息类*>(候选主信息->状态特征.指针->主信息)
            : nullptr;
        const auto* 目标特征类型 = 目标特征主信息 ? 目标特征主信息->类型 : nullptr;
        const auto* 候选特征类型 = 候选特征主信息 ? 候选特征主信息->类型 : nullptr;
        const bool 特征类型一致 =
            目标特征类型 && 候选特征类型
            && (目标特征类型 == 候选特征类型
                || (!目标特征类型->获取主键().empty()
                    && 目标特征类型->获取主键() == 候选特征类型->获取主键()));
        if (!特征类型一致) {
            return false;
        }

        const auto 比较方法结果 = 世界树.特征().获取比较方法结果(
            目标特征类型,
            目标主信息->状态值,
            候选主信息->状态值,
            目标特征主信息,
            候选特征主信息);
        if (!比较方法结果.可比较) {
            return false;
        }

        std::int32_t 需求方向 = 0;
        if (!二次特征本能判断模块::判断状态结果方向(
                &世界树,
                当前状态,
                目标状态,
                需求方向)) {
            return false;
        }

        输出距离 =
            比较方法结果.差值 < 0
            ? -比较方法结果.差值
            : 比较方法结果.差值;
        if (需求方向 > 0) {
            输出是否过头 = 比较方法结果.差值 > 0;
        }
        else if (需求方向 < 0) {
            输出是否过头 = 比较方法结果.差值 < 0;
        }
        return true;
    }

    std::string 私有_P0_推导需求轴键(const 结构_任务管理请求& 请求) noexcept
    {
        const auto& 上下文 = 请求.上下文;
        if (!上下文.当前输入条件主键.empty()) {
            return 上下文.当前输入条件主键;
        }
        if (!请求.来源主观察特征.empty()) {
            return 请求.来源主观察特征;
        }
        if (上下文.当前主需求) {
            const auto 需求主键 = 上下文.当前主需求->获取主键();
            if (!需求主键.empty()) {
                return "需求:" + 需求主键;
            }
            return "需求:未命名需求";
        }
        if (上下文.当前方法需求位 != 枚举_任务管理方法需求位::未定义) {
            return "需求位:" + 私有_方法需求位文本(上下文.当前方法需求位);
        }
        return {};
    }

    void 私有_P0_刷新请求原语锚点(结构_任务管理请求& 请求) noexcept
    {
        请求.当前需求轴键 = 私有_P0_推导需求轴键(请求);
        请求.当前升阶候选组.clear();

        std::vector<任务节点*> 任务结果候选{};
        std::vector<方法节点*> 方法结果候选{};

        const auto 追加任务结果节点到集合 = [&](任务节点* 节点) noexcept {
            if (!节点 || 节点->主信息.节点种类 != 枚举_任务节点种类::结果节点) {
                return;
            }
            if (std::find(任务结果候选.begin(), 任务结果候选.end(), 节点) == 任务结果候选.end()) {
                任务结果候选.emplace_back(节点);
            }
        };

        const auto 追加方法结果节点到集合 = [&](方法节点* 节点) noexcept {
            if (!节点 || 节点->主信息.节点种类 != 枚举_方法节点种类::方法结果节点) {
                return;
            }
            if (std::find(方法结果候选.begin(), 方法结果候选.end(), 节点) == 方法结果候选.end()) {
                方法结果候选.emplace_back(节点);
            }
        };

        const auto 收集步骤结果集合 = [&](任务节点* 步骤节点) noexcept {
            if (!步骤节点 || 步骤节点->主信息.节点种类 != 枚举_任务节点种类::步骤节点) {
                return;
            }

            if (auto* 后续结果 = reinterpret_cast<任务节点*>(步骤节点->主信息.后续任务.指针)) {
                追加任务结果节点到集合(后续结果);
            }

            if (!步骤节点->子) {
                return;
            }

            auto* 起点 = static_cast<任务节点*>(步骤节点->子);
            auto* 当前 = 起点;
            do {
                if (当前 && 当前->主信息.节点种类 == 枚举_任务节点种类::结果节点) {
                    追加任务结果节点到集合(当前);
                }
                当前 = 当前 ? static_cast<任务节点*>(当前->下) : nullptr;
            } while (当前 && 当前 != 起点);
        };

        const auto 收集方法结果集合 = [&](方法节点* 方法结果父节点) noexcept {
            if (!方法结果父节点) {
                return;
            }

            if (方法结果父节点->主信息.节点种类 == 枚举_方法节点种类::方法结果节点) {
                追加方法结果节点到集合(方法结果父节点);
            }

            if (!方法结果父节点->子) {
                return;
            }

            auto* 起点 = static_cast<方法节点*>(方法结果父节点->子);
            auto* 当前 = 起点;
            do {
                if (当前 && 当前->主信息.节点种类 == 枚举_方法节点种类::方法结果节点) {
                    追加方法结果节点到集合(当前);
                }
                当前 = 当前 ? static_cast<方法节点*>(当前->下) : nullptr;
            } while (当前 && 当前 != 起点);
        };

        const auto 合并角色 = [](std::string& 目标, const std::string& 角色文本) noexcept {
            if (角色文本.empty() || 目标.find(角色文本) != std::string::npos) {
                return;
            }
            if (!目标.empty()) {
                目标 += "/";
            }
            目标 += 角色文本;
        };

        const auto 追加任务候选 = [&](任务节点* 节点, 枚举_原语候选来源锚点类型 来源锚点类型, const char* 角色) noexcept {
            if (!节点) {
                return;
            }

            auto 已有 = std::find_if(
                请求.当前升阶候选组.begin(),
                请求.当前升阶候选组.end(),
                [&](const 结构_原语候选项& 项) noexcept {
                    return (项.节点 && 项.节点 == 节点)
                        || (!项.候选主键.empty() && 项.候选主键 == 节点->获取主键());
                });

            std::int32_t 方向关系 = 0;
            const bool 可判断方向 = 私有_P0_判断候选相对需求轴方向(
                请求,
                节点->主信息.结果状态信息.指针,
                方向关系);

            const auto 填充字段 = [&](结构_原语候选项& 候选) noexcept {
                候选.节点 = 节点;
                候选.候选主键 = 节点->获取主键();
                候选.需求轴键 = 请求.当前需求轴键;
                候选.结果时间戳 = 私有_P0_取任务结果时间戳(节点);
                候选.结果序号 = 节点->主信息.步骤序号;
                if (候选.来源锚点类型 == 枚举_原语候选来源锚点类型::未定义) {
                    候选.来源锚点类型 = 来源锚点类型;
                }
                if (可判断方向) {
                    候选.是否同向 = 候选.是否同向 || 方向关系 > 0;
                }
                if (候选.比较特征键.empty()) {
                    候选.比较特征键 = 私有_P0_取状态比较特征键(节点->主信息.结果状态信息.指针);
                    if (!候选.比较特征键.empty()) {
                        候选.比较基准类型 = 枚举_原语候选比较基准类型::状态结果特征;
                    }
                }
                if (候选.比较特征键.empty()) {
                    if (auto* 方法结果节点 = reinterpret_cast<方法节点*>(节点->主信息.对应方法结果节点.指针)) {
                        候选.比较特征键 = 私有_P0_取方法结果比较特征键(方法结果节点);
                        if (!候选.比较特征键.empty()) {
                            候选.比较基准类型 = 枚举_原语候选比较基准类型::方法结果特征;
                        }
                    }
                }
                if (候选.比较特征键.empty() && 节点->主信息.结果方向二次特征.指针) {
                    候选.比较特征键 = 节点->主信息.结果方向二次特征.指针->获取主键();
                    if (!候选.比较特征键.empty()) {
                        候选.比较基准类型 = 枚举_原语候选比较基准类型::结果方向二次特征;
                    }
                }
            };

            if (已有 != 请求.当前升阶候选组.end()) {
                合并角色(已有->候选角色, 角色 ? std::string(角色) : std::string{});
                填充字段(*已有);
                return;
            }

            结构_原语候选项 候选{};
            候选.候选角色 = 角色 ? 角色 : "";
            填充字段(候选);
            请求.当前升阶候选组.emplace_back(std::move(候选));
        };

        const auto 追加方法候选 = [&](方法节点* 方法结果节点, 枚举_原语候选来源锚点类型 来源锚点类型, const char* 角色) noexcept {
            if (!方法结果节点) {
                return;
            }

            auto 已有 = std::find_if(
                请求.当前升阶候选组.begin(),
                请求.当前升阶候选组.end(),
                [&](const 结构_原语候选项& 项) noexcept {
                    return (项.方法节点 && 项.方法节点 == 方法结果节点)
                        || (!项.候选主键.empty()
                            && 项.候选主键 == (
                                !方法结果节点->主信息.结果主键.empty()
                                ? 方法结果节点->主信息.结果主键
                                : 方法结果节点->获取主键()));
                });

            std::int32_t 方向关系 = 0;
            const auto* 结果状态 = 私有_取首个有效状态(方法结果节点->主信息.结果初始状态);
            const bool 可判断方向 = 私有_P0_判断候选相对需求轴方向(
                请求,
                结果状态,
                方向关系);

            const auto 填充字段 = [&](结构_原语候选项& 候选) noexcept {
                候选.方法节点 = 方法结果节点;
                候选.候选主键 =
                    !方法结果节点->主信息.结果主键.empty()
                    ? 方法结果节点->主信息.结果主键
                    : 方法结果节点->获取主键();
                候选.需求轴键 = 请求.当前需求轴键;
                候选.结果时间戳 = 私有_P0_取方法结果时间戳(方法结果节点);
                候选.来源锚点类型 = 来源锚点类型;
                if (可判断方向) {
                    候选.是否同向 = 候选.是否同向 || 方向关系 > 0;
                }
                if (候选.比较特征键.empty()) {
                    候选.比较特征键 = 私有_P0_取状态比较特征键(结果状态);
                    if (!候选.比较特征键.empty()) {
                        候选.比较基准类型 = 枚举_原语候选比较基准类型::状态结果特征;
                    }
                }
                if (候选.比较特征键.empty()) {
                    候选.比较特征键 = 私有_P0_取方法结果比较特征键(方法结果节点);
                    if (!候选.比较特征键.empty()) {
                        候选.比较基准类型 = 枚举_原语候选比较基准类型::方法结果特征;
                    }
                }
            };

            if (已有 != 请求.当前升阶候选组.end()) {
                合并角色(已有->候选角色, 角色 ? std::string(角色) : std::string{});
                填充字段(*已有);
                return;
            }

            结构_原语候选项 候选{};
            候选.候选角色 = 角色 ? 角色 : "";
            填充字段(候选);
            请求.当前升阶候选组.emplace_back(std::move(候选));
        };

        收集步骤结果集合(请求.上下文.当前步骤);
        if (auto* 当前宿主步骤 = 请求.宿主任务
            ? reinterpret_cast<任务节点*>(请求.宿主任务->主信息.当前步骤节点.指针)
            : nullptr) {
            收集步骤结果集合(当前宿主步骤);
        }
        追加任务结果节点到集合(私有_P0_取当前最近结果锚点(请求));
        追加任务结果节点到集合(私有_P0_取步骤结果锚点(请求));
        追加任务结果节点到集合(私有_P0_取命中预测结果锚点(请求));
        追加任务结果节点到集合(私有_P0_取宿主最近实际结果锚点(请求));

        for (auto* 任务结果节点 : 任务结果候选) {
            if (!任务结果节点) {
                continue;
            }

            枚举_原语候选来源锚点类型 来源类型 = 枚举_原语候选来源锚点类型::步骤结果锚点;
            const char* 角色 = "步骤结果锚点";

            if (任务结果节点 == 私有_P0_取当前最近结果锚点(请求)) {
                来源类型 = 枚举_原语候选来源锚点类型::当前最近结果锚点;
                角色 = "当前最近结果锚点";
            }
            else if (任务结果节点 == 私有_P0_取命中预测结果锚点(请求)) {
                来源类型 = 枚举_原语候选来源锚点类型::命中预测结果锚点;
                角色 = "命中预测结果锚点";
            }
            else if (任务结果节点 == 私有_P0_取宿主最近实际结果锚点(请求)) {
                来源类型 = 枚举_原语候选来源锚点类型::宿主最近实际结果锚点;
                角色 = "宿主最近实际结果锚点";
            }

            追加任务候选(任务结果节点, 来源类型, 角色);

            if (auto* 对应方法结果 = reinterpret_cast<方法节点*>(任务结果节点->主信息.对应方法结果节点.指针)) {
                追加方法结果节点到集合(对应方法结果);
            }
        }

        收集方法结果集合(请求.上下文.当前方法);
        收集方法结果集合(私有_P0_取方法结果锚点(请求));

        for (auto* 方法结果节点 : 方法结果候选) {
            if (!方法结果节点) {
                continue;
            }

            const bool 为直接方法锚点 = 方法结果节点 == 私有_P0_取方法结果锚点(请求);
            追加方法候选(
                方法结果节点,
                为直接方法锚点
                    ? 枚举_原语候选来源锚点类型::方法结果锚点
                    : 枚举_原语候选来源锚点类型::方法结果链锚点,
                为直接方法锚点 ? "方法结果锚点" : "方法结果链锚点");
        }
    }

    bool 私有_P0_分析升阶候选(
        const 结构_任务管理请求& 请求,
        结构_P0_升阶候选分析& 输出) noexcept
    {
        输出 = {};
        if (请求.当前需求轴键.empty()) {
            输出.未决原因 = "缺需求轴键";
            return true;
        }

        for (const auto& 项 : 请求.当前升阶候选组) {
            if (项.需求轴键.empty() || 项.需求轴键 != 请求.当前需求轴键) {
                continue;
            }

            std::int32_t 方向关系 = 0;
            const auto* 结果状态 = 私有_P0_取候选结果状态(项);
            const bool 可判断方向 = 私有_P0_判断候选相对需求轴方向(请求, 结果状态, 方向关系);
            if (可判断方向 && 方向关系 < 0) {
                输出.存在同轴反向候选 = true;
                continue;
            }
            if (!项.是否同向) {
                continue;
            }

            输出.入选候选.emplace_back(&项);
        }

        输出.同轴同向候选数 = 输出.入选候选.size();
        if (输出.同轴同向候选数 < 2) {
            输出.未决原因 = 输出.同轴同向候选数 == 0
                ? "当前没有形成同轴同向候选"
                : "当前仅有单个同轴同向候选";
            return true;
        }

        struct 结构_可比候选组 {
            std::string 基准摘要{};
            std::vector<const 结构_原语候选项*> 成员{};
        };

        std::vector<结构_可比候选组> 候选组{};
        for (const auto* 项 : 输出.入选候选) {
            if (!项
                || 项->比较基准类型 == 枚举_原语候选比较基准类型::未定义
                || 项->比较特征键.empty()) {
                continue;
            }

            const auto 基准摘要 =
                std::string(私有_P0_原语候选比较基准类型文本(项->比较基准类型))
                + ":" + 项->比较特征键;
            auto 已有 = std::find_if(
                候选组.begin(),
                候选组.end(),
                [&](const 结构_可比候选组& 组) noexcept {
                    return 组.基准摘要 == 基准摘要;
                });
            if (已有 == 候选组.end()) {
                候选组.push_back({ 基准摘要, { 项 } });
            }
            else {
                已有->成员.push_back(项);
            }
        }

        const 结构_可比候选组* 最佳候选组 = nullptr;
        for (const auto& 候选组项 : 候选组) {
            if (候选组项.成员.size() < 2) {
                continue;
            }
            if (!最佳候选组 || 候选组项.成员.size() > 最佳候选组->成员.size()) {
                最佳候选组 = &候选组项;
            }
        }

        if (最佳候选组) {
            输出.存在可比对 = true;
            输出.可比基准摘要 = 最佳候选组->基准摘要;
            输出.可比候选 = 最佳候选组->成员;
            if (!输出.可比候选.empty()) {
                输出.首可比候选 = 输出.可比候选.front();
            }
            if (输出.可比候选.size() > 1) {
                输出.次可比候选 = 输出.可比候选[1];
            }
        }
        else {
            std::ostringstream 未决原因;
            未决原因 << "缺可比基准：";
            bool 首个 = true;
            for (const auto* 项 : 输出.入选候选) {
                if (!项) {
                    continue;
                }
                if (!首个) {
                    未决原因 << "、";
                }
                首个 = false;
                未决原因
                    << 私有_P0_原语候选标题(*项)
                    << "("
                    << (项->比较特征键.empty()
                        ? std::string("缺比较特征键")
                        : std::string(私有_P0_原语候选比较基准类型文本(项->比较基准类型)) + ":" + 项->比较特征键)
                    << ")";
            }
            输出.未决原因 = 首个 ? std::string("缺可比基准") : 未决原因.str();
            return true;
        }

        struct 结构_排序观测 {
            const 结构_原语候选项* 候选 = nullptr;
            std::int64_t 相对目标距离 = 0;
            bool 是否过头 = false;
        };

        std::vector<结构_排序观测> 排序观测集{};
        排序观测集.reserve(输出.可比候选.size());
        for (const auto* 项 : 输出.可比候选) {
            if (!项) {
                continue;
            }
            结构_排序观测 观测{};
            观测.候选 = 项;
            if (!私有_P0_读取候选相对目标距离(
                    请求,
                    *项,
                    观测.相对目标距离,
                    观测.是否过头)) {
                continue;
            }
            排序观测集.emplace_back(观测);
        }

        if (排序观测集.size() < 2) {
            输出.未决原因 =
                "比较基准[" + 输出.可比基准摘要 + "]下已形成同向候选，但缺目标距离比较值";
            return true;
        }

        const auto 更优于 = [](const 结构_排序观测& 左, const 结构_排序观测& 右) noexcept {
            if (左.是否过头 != 右.是否过头) {
                return !左.是否过头 && 右.是否过头;
            }
            if (左.相对目标距离 != 右.相对目标距离) {
                return 左.相对目标距离 < 右.相对目标距离;
            }
            return false;
        };

        const 结构_排序观测* 最优 = nullptr;
        const 结构_排序观测* 次优 = nullptr;
        for (const auto& 观测 : 排序观测集) {
            if (!最优 || 更优于(观测, *最优)) {
                次优 = 最优;
                最优 = &观测;
            }
            else if (!次优 || 更优于(观测, *次优)) {
                次优 = &观测;
            }
        }

        if (!最优 || !最优->候选) {
            输出.未决原因 = "高阶排序未能确定有效候选";
            return true;
        }

        输出.已完成高阶排序 = true;
        输出.是否过头 = 最优->是否过头;
        输出.排序置信度_Q10000 = 最优->是否过头 ? 7200 : 8200;

        if (次优
            && 次优->候选
            && 最优->相对目标距离 == 次优->相对目标距离
            && 最优->是否过头 == 次优->是否过头) {
            输出.是否等效 = true;
            输出.排序置信度_Q10000 = 6800;
            输出.排序关系 =
                私有_P0_原语候选标题(*最优->候选)
                + " 与 "
                + 私有_P0_原语候选标题(*次优->候选)
                + " 等效";
            输出.排序原因 =
                "比较基准=" + 输出.可比基准摘要
                + " | 两者相对目标距离一致，暂按等效返回";
            return true;
        }

        输出.胜出候选主键 = 最优->候选->候选主键;
        if (次优 && 次优->候选) {
            if (!最优->是否过头 && 次优->是否过头) {
                输出.排序关系 =
                    私有_P0_原语候选标题(*最优->候选)
                    + " 更合适，"
                    + 私有_P0_原语候选标题(*次优->候选)
                    + " 已过头";
            }
            else {
                输出.排序关系 =
                    私有_P0_原语候选标题(*最优->候选)
                    + " 比 "
                    + 私有_P0_原语候选标题(*次优->候选)
                    + " 更接近需求";
            }
        }
        else {
            输出.排序关系 = 私有_P0_原语候选标题(*最优->候选) + " 当前更接近需求";
        }

        std::ostringstream 排序原因;
        排序原因
            << "比较基准=" << 输出.可比基准摘要
            << " | 胜出候选=" << 私有_P0_原语候选标题(*最优->候选)
            << " | 相对目标距离=" << 最优->相对目标距离
            << " | 过头=" << (最优->是否过头 ? "是" : "否");
        if (次优 && 次优->候选) {
            排序原因
                << " | 对比候选=" << 私有_P0_原语候选标题(*次优->候选)
                << " | 对比距离=" << 次优->相对目标距离
                << " | 对比过头=" << (次优->是否过头 ? "是" : "否");
        }
        输出.排序原因 = 排序原因.str();
        return true;
    }

    bool 私有_P0_生成高阶排序请求(
        const 结构_任务管理请求& 请求,
        结构_高阶排序请求& 输出) noexcept
    {
        输出 = {};
        输出.需求轴键 = 请求.当前需求轴键;

        结构_P0_升阶候选分析 候选分析{};
        (void)私有_P0_分析升阶候选(请求, 候选分析);

        for (const auto* 候选 : 候选分析.入选候选) {
            if (候选 && !候选->候选主键.empty()) {
                输出.候选主键列表.push_back(候选->候选主键);
            }
        }
        if (输出.候选主键列表.empty()) {
            for (const auto& 候选 : 请求.当前升阶候选组) {
                if (!请求.当前需求轴键.empty()
                    && !候选.需求轴键.empty()
                    && 候选.需求轴键 != 请求.当前需求轴键) {
                    continue;
                }
                if (!候选.候选主键.empty()) {
                    输出.候选主键列表.push_back(候选.候选主键);
                }
            }
        }

        输出.候选摘要 = 私有_P0_升阶候选摘要(请求);
        输出.存在可比对 = 候选分析.存在可比对;
        输出.比较基准摘要 = 候选分析.可比基准摘要;
        输出.未决原因 = 候选分析.未决原因;

        const auto* 首候选 =
            候选分析.首可比候选 ? 候选分析.首可比候选
            : (!候选分析.入选候选.empty() ? 候选分析.入选候选.front() : nullptr);
        const auto* 次候选 =
            候选分析.次可比候选 ? 候选分析.次可比候选
            : (候选分析.入选候选.size() > 1 ? 候选分析.入选候选[1] : nullptr);

        if (首候选) {
            输出.首候选主键 = 首候选->候选主键;
            输出.比较特征键 = 首候选->比较特征键;
            输出.比较基准类型 = 首候选->比较基准类型;
        }
        if (次候选) {
            输出.次候选主键 = 次候选->候选主键;
            if (输出.比较特征键.empty()) {
                输出.比较特征键 = 次候选->比较特征键;
            }
            if (输出.比较基准类型 == 枚举_原语候选比较基准类型::未定义) {
                输出.比较基准类型 = 次候选->比较基准类型;
            }
        }
        if (输出.比较基准摘要.empty()
            && 输出.比较基准类型 != 枚举_原语候选比较基准类型::未定义
            && !输出.比较特征键.empty()) {
            输出.比较基准摘要 =
                std::string(私有_P0_原语候选比较基准类型文本(输出.比较基准类型))
                + ":" + 输出.比较特征键;
        }
        return true;
    }

    bool 私有_P0_生成高阶排序结果(
        const 结构_任务管理请求& 请求,
        结构_高阶排序结果& 输出) noexcept
    {
        输出 = {};
        输出.需求轴键 = 请求.当前需求轴键;
        for (const auto& 候选 : 请求.当前升阶候选组) {
            if (!请求.当前需求轴键.empty()
                && !候选.需求轴键.empty()
                && 候选.需求轴键 != 请求.当前需求轴键) {
                continue;
            }
            if (!候选.候选主键.empty()) {
                输出.候选主键列表.push_back(候选.候选主键);
            }
        }
        输出.候选摘要 = 私有_P0_升阶候选摘要(请求);

        结构_P0_升阶候选分析 候选分析{};
        (void)私有_P0_分析升阶候选(请求, 候选分析);
        if (!候选分析.已完成高阶排序) {
            输出.排序关系 =
                候选分析.存在可比对
                ? std::string("需要升阶但尚未排完")
                : std::string("当前不可比");
            输出.置信度_Q10000 = 候选分析.存在可比对 ? 5800 : 4200;
            输出.原因说明 =
                !候选分析.未决原因.empty()
                ? 候选分析.未决原因
                : (候选分析.存在可比对
                    ? "已形成同轴同向可比候选，等待完成高阶排序"
                    : "当前尚未形成可稳定排序的候选组");
            return true;
        }

        输出.排序关系 = 候选分析.排序关系;
        输出.胜出候选主键 = 候选分析.胜出候选主键;
        输出.是否等效 = 候选分析.是否等效;
        输出.是否过头 = 候选分析.是否过头;
        输出.置信度_Q10000 = 候选分析.排序置信度_Q10000;
        输出.原因说明 =
            !候选分析.排序原因.empty()
            ? 候选分析.排序原因
            : 候选分析.未决原因;
        return true;
    }

    std::size_t 私有_P0_统计升阶同向候选数(const 结构_任务管理请求& 请求) noexcept
    {
        结构_P0_升阶候选分析 候选分析{};
        (void)私有_P0_分析升阶候选(请求, 候选分析);
        return 候选分析.同轴同向候选数;
    }

    std::string 私有_P0_升阶候选摘要(const 结构_任务管理请求& 请求) noexcept
    {
        if (请求.当前升阶候选组.empty()) {
            return "空";
        }

        结构_P0_升阶候选分析 候选分析{};
        (void)私有_P0_分析升阶候选(请求, 候选分析);

        std::ostringstream 输出;
        bool 首个 = true;
        for (const auto& 项 : 请求.当前升阶候选组) {
            if (!首个) {
                输出 << "; ";
            }
            首个 = false;
            std::int32_t 方向关系 = 0;
            const bool 可判断方向 = 私有_P0_判断候选相对需求轴方向(
                请求,
                私有_P0_取候选结果状态(项),
                方向关系);
            const auto 方向文本 =
                项.是否同向
                ? "同向"
                : (可判断方向 && 方向关系 < 0 ? "反向" : "未明");

            输出 << (项.候选角色.empty() ? "候选" : 项.候选角色)
                << "=" << 私有_P0_原语候选标题(项)
                << "["
                << 方向文本
                << "|轴=" << (项.需求轴键.empty() ? "空" : 项.需求轴键)
                << "|基准=";
            if (项.比较特征键.empty()) {
                输出 << "缺失";
            }
            else {
                输出 << 私有_P0_原语候选比较基准类型文本(项.比较基准类型)
                    << ":" << 项.比较特征键;
            }
            输出 << "|锚点=" << 私有_P0_原语候选来源锚点类型文本(项.来源锚点类型);
            if (项.结果序号 != 0) {
                输出 << "|序=" << 项.结果序号;
            }
            else if (项.结果时间戳 != 0) {
                输出 << "|时=" << 项.结果时间戳;
            }
            输出 << "]";
        }
        if (候选分析.存在可比对 && !候选分析.可比基准摘要.empty()) {
            输出 << " -> 比较基准=" << 候选分析.可比基准摘要;
        }
        if (候选分析.已完成高阶排序) {
            if (!候选分析.排序关系.empty()) {
                输出 << " -> 排序=" << 候选分析.排序关系;
            }
            if (!候选分析.胜出候选主键.empty()) {
                输出 << " -> 胜出=" << 候选分析.胜出候选主键;
            }
        }
        else if (!候选分析.未决原因.empty()) {
            输出 << " -> 未决=" << 候选分析.未决原因;
        }
        return 首个 ? std::string("空") : 输出.str();
    }

    状态节点类* 私有_确保任务状态镜像(
        自我类& 自我对象,
        任务节点* 任务,
        可解析引用<状态节点类>& 现有引用,
        const 词性节点类* 特征类型,
        枚举_任务状态 状态值,
        时间戳 now,
        const std::string& 原因类别,
        const std::string& 原因说明) noexcept
    {
        if (!任务 || !特征类型) {
            return nullptr;
        }

        if (私有_状态节点值匹配任务状态(现有引用.指针, 状态值)) {
            return 现有引用.指针;
        }

        auto* 场景 = 私有_任务场景(自我对象, 任务);
        auto* 任务虚拟存在 = 私有_确保任务虚拟存在(自我对象, 任务, 原因类别);
        auto* 宿主基础节点 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        auto* 特征节点 = 宿主基础节点
            ? 世界树.取或创建子特征_按类型(宿主基础节点, 特征类型)
            : nullptr;
        if (!场景 || !宿主基础节点 || !特征节点) {
            return nullptr;
        }

        auto* 状态节点 = 私有_创建状态节点(
            场景,
            宿主基础节点,
            特征节点,
            特征值{ static_cast<I64>(状态值) },
            now,
            true,
            原因类别,
            原因说明);
        if (!状态节点) {
            return nullptr;
        }

        私有_写入引用(现有引用, 状态节点);
        return 状态节点;
    }

    枚举_任务状态 私有_宿主任务目标状态(const 结构_任务管理结果& 结果) noexcept
    {
        switch (私有_P0_当前治理状态迁移(结果)) {
        case 枚举_任务管理治理状态迁移::进入挂起:
            return 枚举_任务状态::挂起;
        case 枚举_任务管理治理状态迁移::进入运行:
        case 枚举_任务管理治理状态迁移::保持运行:
        case 枚举_任务管理治理状态迁移::保持等待:
            return 枚举_任务状态::运行中;
        case 枚举_任务管理治理状态迁移::进入收束:
            return 结果.宿主任务新状态 != 枚举_任务状态::未定义
                ? 结果.宿主任务新状态
                : 枚举_任务状态::完成;
        case 枚举_任务管理治理状态迁移::回到重筹办:
            return 枚举_任务状态::未启动;
        default:
            return 结果.宿主任务新状态 != 枚举_任务状态::未定义
                ? 结果.宿主任务新状态
                : 枚举_任务状态::未启动;
        }
    }

    状态节点类* 私有_确保任务目标状态(
        自我类& 自我对象,
        任务节点* 任务,
        枚举_任务状态 目标状态,
        时间戳 now,
        const std::string& 原因类别,
        const std::string& 原因说明) noexcept
    {
        return 私有_确保任务状态镜像(
            自我对象,
            任务,
            任务->主信息.目标状态,
            私有_任务特征_目标状态(),
            目标状态,
            now,
            原因类别,
            原因说明);
    }

    状态节点类* 私有_确保任务结果状态(
        自我类& 自我对象,
        任务节点* 任务,
        枚举_任务状态 结果状态,
        时间戳 now,
        const std::string& 原因类别,
        const std::string& 原因说明) noexcept
    {
        if (!任务) {
            return nullptr;
        }

        return 私有_确保任务状态镜像(
            自我对象,
            任务,
            任务->主信息.结果状态信息,
            私有_任务特征_结果状态(),
            结果状态,
            now,
            原因类别,
            原因说明);
    }

    bool 私有_任务节点方法匹配(const 任务节点* 任务, const 方法节点* 方法) noexcept
    {
        const auto* 已挂方法 = 任务 && 任务->主信息.执行方法.指针
            ? reinterpret_cast<const 方法节点*>(任务->主信息.执行方法.指针)
            : nullptr;
        return 已挂方法 == 方法;
    }

    bool 私有_可进入等待分支(const 任务节点* 宿主任务, const 结构_任务管理结果& 结果) noexcept
    {
        return 宿主任务 && 私有_P0_当前需等待(结果);
    }

    枚举_任务管理步骤语义类型 私有_推导步骤语义类型(
        const 结构_任务管理结果& 结果,
        const 任务节点* 宿主任务) noexcept
    {
        (void)宿主任务;
        switch (私有_P0_当前步骤位类型(结果)) {
        case 枚举_任务管理步骤位类型::等待位:
            return 枚举_任务管理步骤语义类型::等待维持步骤;
        case 枚举_任务管理步骤位类型::回接位:
            return 枚举_任务管理步骤语义类型::回接派生步骤;
        case 枚举_任务管理步骤位类型::补条件位:
            return 枚举_任务管理步骤语义类型::补条件步骤;
        case 枚举_任务管理步骤位类型::补结构位:
            return 枚举_任务管理步骤语义类型::补结构步骤;
        case 枚举_任务管理步骤位类型::执行位:
            return 结果.宿主目标结果待推进
                ? 枚举_任务管理步骤语义类型::业务推进步骤
                : 枚举_任务管理步骤语义类型::治理步骤;
        default:
            return 枚举_任务管理步骤语义类型::未定义;
        }
    }

    bool 私有_业务子任务应转入执行位(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 宿主任务
            && 私有_P0_业务子任务应转执行(结果);
    }

    bool 私有_业务子任务执行已完成(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 宿主任务
            && 私有_P0_业务子任务已完成(结果);
    }

    任务节点* 私有_查找待回接的已完成业务子任务(
        const 任务节点* 宿主任务,
        const 结构_任务管理上下文& 上下文) noexcept
    {
        if (!宿主任务 || !上下文.最近结果) {
            return nullptr;
        }

        const auto* 最近结果 = 上下文.最近结果;
        if (最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
            || 最近结果->主信息.状态 != 枚举_任务状态::完成) {
            return nullptr;
        }

        const auto* 最近步骤 = 最近结果->父
            ? static_cast<const 任务节点*>(最近结果->父)
            : nullptr;
        if (!最近步骤
            || 最近步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点
            || 最近步骤->主信息.状态 != 枚举_任务状态::完成) {
            return nullptr;
        }

        static const 词性节点类* s_业务执行位类型 = 语素集.添加词性词("业务执行位", "名词");
        static const 词性节点类* s_业务执行结果类型 = 语素集.添加词性词("业务执行结果", "名词");
        if (!私有_词性相同(最近步骤->主信息.类型, s_业务执行位类型)
            || !私有_词性相同(最近结果->主信息.类型, s_业务执行结果类型)) {
            return nullptr;
        }

        return const_cast<任务节点*>(私有_查找直接子节点_按谓词<任务节点>(
            const_cast<任务节点*>(宿主任务),
            [&](const 任务节点* 节点) noexcept {
                return 节点
                    && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                    && 私有_词性相同(节点->主信息.类型, 私有_业务子任务类型词())
                    && 节点->主信息.状态 == 枚举_任务状态::完成
                    && 私有_任务节点归属宿主任务(最近结果, 节点)
                    && 私有_任务节点归属宿主任务(最近步骤, 节点);
            }));
    }

    bool 私有_写回任务目标状态(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        bool 已写回 = false;
        auto* 当前步骤节点 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果节点 = 私有_P0_最新结果节点(结果);
        auto* 当前方法 = 私有_P0_当前方法(结果);

        auto* 宿主目标状态 = 私有_确保任务目标状态(
            自我对象,
            宿主任务,
            私有_宿主任务目标状态(结果),
            now,
            "任务管理_宿主目标状态",
            私有_治理状态迁移文本(结果.当前治理状态迁移));
        if (宿主目标状态) {
            结果.宿主目标状态 = 宿主目标状态;
            结果.最新目标状态 = 宿主目标状态;
            已写回 = true;
        }

        auto* 步骤目标状态 = 私有_确保任务目标状态(
            自我对象,
            当前步骤节点,
            当前方法 ? 枚举_任务状态::完成 : 枚举_任务状态::挂起,
            now,
            "任务管理_步骤目标状态",
            调用点);
        if (步骤目标状态) {
            结果.步骤目标状态 = 步骤目标状态;
            结果.最新目标状态 = 步骤目标状态;
            已写回 = true;
        }

        auto* 结果目标状态 = 私有_确保任务目标状态(
            自我对象,
            最新结果节点,
            私有_治理结果状态(结果),
            now,
            "任务管理_结果目标状态",
            调用点);
        if (结果目标状态) {
            结果.结果目标状态 = 结果目标状态;
            结果.最新目标状态 = 结果目标状态;
            已写回 = true;
        }

        结果.已写回目标状态 = 已写回;
        return 已写回;
    }

    void 私有_同步任务结果状态引用(
        任务节点* 宿主任务,
        结构_任务管理结果& 结果) noexcept
    {
        auto* 当前步骤节点 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果节点 = 私有_P0_最新结果节点(结果);
        结果.宿主结果状态 = 宿主任务 ? 宿主任务->主信息.结果状态信息.指针 : nullptr;
        结果.步骤结果状态 = 当前步骤节点 ? 当前步骤节点->主信息.结果状态信息.指针 : nullptr;
        结果.结果结果状态 = 最新结果节点 ? 最新结果节点->主信息.结果状态信息.指针 : nullptr;

        if (结果.结果结果状态) {
            if (当前步骤节点
                && 当前步骤节点->主信息.结果状态信息.指针 != 结果.结果结果状态) {
                私有_写入引用(当前步骤节点->主信息.结果状态信息, 结果.结果结果状态);
                结果.步骤结果状态 = 结果.结果结果状态;
            }
        }
    }

    bool 私有_写回任务结果状态(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        bool 已写回 = false;
        auto* 当前步骤节点 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果节点 = 私有_P0_最新结果节点(结果);

        auto* 宿主结果状态 = 私有_确保任务结果状态(
            自我对象,
            宿主任务,
            结果.宿主任务新状态 != 枚举_任务状态::未定义
                ? 结果.宿主任务新状态
                : (宿主任务 ? 宿主任务->主信息.状态 : 枚举_任务状态::未定义),
            now,
            "任务管理_宿主结果状态",
            调用点);
        if (宿主结果状态) {
            结果.宿主结果状态 = 宿主结果状态;
            结果.最新状态 = 宿主结果状态;
            已写回 = true;
        }

        auto* 步骤结果状态 = 私有_确保任务结果状态(
            自我对象,
            当前步骤节点,
            当前步骤节点 ? 当前步骤节点->主信息.状态 : 枚举_任务状态::未定义,
            now,
            "任务管理_步骤结果状态",
            调用点);
        if (步骤结果状态) {
            结果.步骤结果状态 = 步骤结果状态;
            结果.最新状态 = 步骤结果状态;
            已写回 = true;
        }

        auto* 结果结果状态 = 私有_确保任务结果状态(
            自我对象,
            最新结果节点,
            最新结果节点 ? 最新结果节点->主信息.状态 : 枚举_任务状态::未定义,
            now,
            "任务管理_结果结果状态",
            调用点);
        if (结果结果状态) {
            结果.结果结果状态 = 结果结果状态;
            结果.最新状态 = 结果结果状态;
            已写回 = true;
        }

        私有_同步任务结果状态引用(宿主任务, 结果);
        return 已写回;
    }

    std::string 私有_任务双面摘要文本(
        const char* 标签,
        const 状态节点类* 目标状态,
        const 状态节点类* 结果状态) noexcept
    {
        枚举_任务状态 目标值 = 枚举_任务状态::未定义;
        枚举_任务状态 结果值 = 枚举_任务状态::未定义;
        const bool 有目标 = 私有_读取状态节点任务状态(目标状态, 目标值);
        const bool 有结果 = 私有_读取状态节点任务状态(结果状态, 结果值);

        std::ostringstream 输出;
        输出 << 标签
            << "目标=" << (有目标 ? 私有_任务状态文本(目标值) : std::string("缺失"))
            << "/结果=" << (有结果 ? 私有_任务状态文本(结果值) : std::string("缺失"))
            << "/差额=";
        std::int32_t 对齐结果 = 0;
        if (二次特征本能判断模块::判断状态是否对齐(
                &世界树,
                目标状态,
                结果状态,
                对齐结果)) {
            输出 << (对齐结果 > 0 ? "已对齐" : "待推进");
        }
        else if (有目标 && 有结果 && 目标值 == 结果值) {
            输出 << "已对齐";
        }
        else if (有目标 && 有结果) {
            输出 << "待推进";
        }
        else {
            输出 << "待补齐";
        }
        return 输出.str();
    }

    枚举_任务管理双面比较结果 私有_比较任务双面(
        const 状态节点类* 目标状态,
        const 状态节点类* 结果状态) noexcept
    {
        std::int32_t 对齐结果 = 0;
        if (二次特征本能判断模块::判断状态是否对齐(
                &世界树,
                目标状态,
                结果状态,
                对齐结果)) {
            return 对齐结果 > 0
                ? 枚举_任务管理双面比较结果::已对齐
                : 枚举_任务管理双面比较结果::存在差额;
        }
        return 枚举_任务管理双面比较结果::待补齐;
    }

    void 私有_整理后果治理(
        结构_任务管理结果& 结果,
        bool 存在待回接业务子任务) noexcept
    {
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        if (存在待回接业务子任务) {
            if (结果.当前宿主目标结果比较 == 枚举_任务管理双面比较结果::已对齐) {
                结果.当前后果治理 = 枚举_任务管理后果治理::回接后保持等待;
                return;
            }
            if (结果.当前宿主目标结果比较 == 枚举_任务管理双面比较结果::存在差额) {
                结果.当前后果治理 = 枚举_任务管理后果治理::回接后继续派生;
                return;
            }
        }

        switch (当前下一步去向) {
        case 枚举_任务管理下一步去向::进入收束:
            结果.当前后果治理 = 枚举_任务管理后果治理::进入收束;
            return;
        case 枚举_任务管理下一步去向::保持等待:
            结果.当前后果治理 = 枚举_任务管理后果治理::保持等待;
            return;
        default:
            break;
        }

        switch (结果.当前宿主目标结果比较) {
        case 枚举_任务管理双面比较结果::待补齐:
            结果.当前后果治理 = 枚举_任务管理后果治理::补齐双面;
            return;
        case 枚举_任务管理双面比较结果::存在差额:
            结果.当前后果治理 = 枚举_任务管理后果治理::继续派生业务步骤;
            return;
        case 枚举_任务管理双面比较结果::已对齐:
            结果.当前后果治理 = 枚举_任务管理后果治理::无额外后果;
            return;
        default:
            结果.当前后果治理 = 枚举_任务管理后果治理::未定义;
            return;
        }
    }

    void 私有_整理任务双面摘要(
        任务节点* 宿主任务,
        结构_任务管理结果& 结果) noexcept
    {
        (void)宿主任务;
        结果.宿主目标状态 = 宿主任务 ? 宿主任务->主信息.目标状态.指针 : nullptr;
        结果.步骤目标状态 = 结果.当前步骤节点 ? 结果.当前步骤节点->主信息.目标状态.指针 : nullptr;
        结果.结果目标状态 = 结果.最新结果节点 ? 结果.最新结果节点->主信息.目标状态.指针 : nullptr;
        私有_同步任务结果状态引用(宿主任务, 结果);

        结果.当前宿主目标结果比较 = 私有_比较任务双面(结果.宿主目标状态, 结果.宿主结果状态);
        结果.当前步骤目标结果比较 = 私有_比较任务双面(结果.步骤目标状态, 结果.步骤结果状态);
        结果.当前结果目标结果比较 = 私有_比较任务双面(结果.结果目标状态, 结果.结果结果状态);
        结果.最近宿主目标结果比较 = 结果.当前宿主目标结果比较;
        结果.宿主目标结果已对齐 = 结果.当前宿主目标结果比较 == 枚举_任务管理双面比较结果::已对齐;
        结果.宿主目标结果待推进 = 结果.当前宿主目标结果比较 == 枚举_任务管理双面比较结果::存在差额;
        结果.已由目标差额驱动 = 结果.宿主目标结果待推进;

        std::vector<std::string> 片段{};
        片段.emplace_back(私有_任务双面摘要文本("宿主", 结果.宿主目标状态, 结果.宿主结果状态));
        if (结果.当前步骤节点) {
            片段.emplace_back(私有_任务双面摘要文本("步骤", 结果.步骤目标状态, 结果.步骤结果状态));
        }
        if (结果.最新结果节点) {
            片段.emplace_back(私有_任务双面摘要文本("结果", 结果.结果目标状态, 结果.结果结果状态));
        }
        片段.emplace_back("宿主比较=" + 私有_双面比较结果文本(结果.当前宿主目标结果比较));
        if (结果.宿主目标结果待推进) {
            片段.emplace_back("宿主目标与结果存在差额，下一轮继续派生业务步骤");
        }
        else if (结果.当前宿主目标结果比较 == 枚举_任务管理双面比较结果::待补齐) {
            片段.emplace_back("宿主目标与结果仍待补齐，当前只补账本双面");
        }
        结果.目标结果差额摘要 = 私有_拼接片段(片段);
    }

    void 私有_同步治理步骤节点(
        任务节点* 步骤节点,
        任务节点* 父节点,
        任务节点* 宿主任务,
        const 结构_任务管理结果& 结果,
        场景节点类* 场景,
        时间戳 now) noexcept
    {
        if (!步骤节点 || !宿主任务) {
            return;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);
        auto& 主信息 = 步骤节点->主信息;
        主信息.名称 = 私有_治理步骤名称词(结果);
        主信息.类型 = 私有_治理步骤类型词(结果);
        主信息.节点种类 = 枚举_任务节点种类::步骤节点;
        主信息.状态 = 当前方法 ? 枚举_任务状态::运行中 : 枚举_任务状态::挂起;
        if (主信息.启动时间 == 0) {
            主信息.启动时间 = now;
        }
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 宿主任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 宿主任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 宿主任务->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 父节点);
        私有_写入引用(主信息.场景, 场景);
        私有_写入引用(主信息.执行方法, 当前方法);
        if (当前方法) {
            主信息.动作句柄 = 当前方法->主信息.动作句柄;
        }
        else {
            主信息.动作句柄 = 结构体_动作句柄{};
        }
        if (结果.最新状态) {
            私有_写入引用(主信息.结果状态信息, 结果.最新状态);
        }
    }

    bool 私有_结果节点匹配当前治理(
        const 任务节点* 结果节点,
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        return 结果节点
            && 宿主任务
            && 私有_P0_最近结果命中当前治理(结果);
    }

    bool 私有_P1_写任务治理二次特征_布尔(
        自我类& 自我对象,
        const 任务节点* 宿主任务,
        const std::string& 概念名称,
        const bool 是否满足,
        const std::string& 原因摘要,
        const I64 置信度,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务 || 概念名称.empty()) {
            return false;
        }

        auto* 场景 = 私有_任务场景(自我对象, 宿主任务);
        auto* 主体 = 私有_确保任务虚拟存在(
            自我对象,
            const_cast<任务节点*>(宿主任务),
            调用点 + "/任务虚拟存在");
        if (!场景 || !主体) {
            return false;
        }

        结构_场景二次特征写入请求 请求{};
        请求.概念名称 = 概念名称;
        请求.概念词性 = "名词";
        请求.同步名称和类型 = true;
        请求.种类 = 枚举_二次特征种类::状态比较;
        请求.域 = 枚举_二次特征域::存在;
        请求.粒度 = 枚举_二次特征粒度::当前场景;
        请求.基准类型 = 枚举_二次特征基准类型::无;
        请求.时间归一方式 = 枚举_时间归一方式::无;
        请求.值形态 = 枚举_二次特征值形态::布尔;
        请求.主体 = 主体;
        请求.离散编码 = 是否满足 ? 1 : 0;
        请求.离散语义键 = 原因摘要.empty() ? (是否满足 ? "满足" : "未满足") : 原因摘要;
        请求.标量值 = 是否满足 ? 1 : 0;
        请求.是否满足 = 是否满足;
        请求.置信度 = 置信度;
        return 世界树.二次特征生成().写入场景二次特征(场景, 请求, nullptr);
    }

    bool 私有_P1_刷新任务治理二次特征证据(
        自我类& 自我对象,
        结构_任务管理请求& 请求,
        const std::string& 调用点) noexcept
    {
        请求.当前治理二次特征证据 = {};

        auto* 宿主任务 = 私有_P0_选用非空指针(
            static_cast<任务节点*>(请求.宿主任务),
            请求.上下文.当前宿主任务);
        if (!宿主任务) {
            return false;
        }

        const auto* 当前方法 = 请求.上下文.当前方法;
        const auto* 当前步骤 = 请求.上下文.当前步骤;
        const auto* 最近结果 = 请求.上下文.最近结果;
        const auto* 最近结果方法 = 最近结果 && 最近结果->主信息.执行方法.指针
            ? reinterpret_cast<const 方法节点*>(最近结果->主信息.执行方法.指针)
            : nullptr;
        const auto* 当前步骤方法 = 当前步骤 && 当前步骤->主信息.执行方法.指针
            ? reinterpret_cast<const 方法节点*>(当前步骤->主信息.执行方法.指针)
            : nullptr;
        const auto 当前方法位专项动态标题 = 私有_任务管理桥接动态标题(
            请求.上下文.当前方法位专项动态,
            请求.上下文.当前方法位专项动态标题);
        const auto 当前最小原语动态标题 = 私有_任务管理桥接动态标题(
            请求.上下文.当前最小原语动态,
            请求.上下文.当前最小原语动态标题);
        const auto* 当前方法位专项动态主信息 =
            私有_读取任务管理桥接动态主信息(请求.上下文.当前方法位专项动态);
        const auto* 当前最小原语动态主信息 =
            私有_读取任务管理桥接动态主信息(请求.上下文.当前最小原语动态);
        const bool 当前动作已启动 =
            私有_任务管理动态已开始(当前方法位专项动态主信息, 当前方法位专项动态标题)
            || 私有_任务管理动态已开始(当前最小原语动态主信息, 当前最小原语动态标题);
        const bool 当前动作已完成 =
            私有_任务管理动态已完成(当前方法位专项动态主信息, 当前方法位专项动态标题)
            || 私有_任务管理动态已完成(当前最小原语动态主信息, 当前最小原语动态标题);
        const bool 当前动作失败退出 =
            私有_任务管理动态失败退出(当前方法位专项动态主信息, 当前方法位专项动态标题)
            || 私有_任务管理动态失败退出(当前最小原语动态主信息, 当前最小原语动态标题);
        const bool 当前处于方法位推进中 =
            私有_任务管理动态处于方法位推进中(当前方法位专项动态主信息, 当前方法位专项动态标题);
        const bool 当前处于原语边界执行中 =
            私有_任务管理动态处于原语边界执行中(当前最小原语动态主信息, 当前最小原语动态标题);
        const auto 当前时间 =
            请求.上下文.当前时间 != 0
            ? 请求.上下文.当前时间
            : 请求.提交时间;
        const 时间戳 最近动作推进时间 = [&]() noexcept {
            if (请求.上下文.最近结果) {
                return 私有_P0_取任务结果时间戳(请求.上下文.最近结果);
            }
            if (当前步骤) {
                return 私有_P0_取任务结果时间戳(当前步骤);
            }
            if (宿主任务) {
                return 私有_P0_取任务结果时间戳(宿主任务);
            }
            return static_cast<时间戳>(0);
        }();
        const bool 控制响应超时 =
            请求.当前控制承接.是否超时
            || 请求.当前控制承接.当前响应状态 == 枚举_任务管理控制响应状态::已超时;
        const bool 当前动作被中断 =
            (请求.当前控制承接.当前响应状态 == 枚举_任务管理控制响应状态::已执行完成
                || 请求.当前控制承接.当前响应状态 == 枚举_任务管理控制响应状态::已响应)
            && (
                请求.当前控制承接.当前控制意图 == 枚举_任务管理控制意图值::请求暂停
                || 请求.当前控制承接.当前控制意图 == 枚举_任务管理控制意图值::请求打断
                || 请求.当前控制承接.当前控制意图 == 枚举_任务管理控制意图值::请求停止
            );

        const bool 已对齐 = 私有_任务节点目标结果已对齐(宿主任务);
        const bool 待推进 = 私有_任务节点目标结果待推进(宿主任务);
        const bool 影子验证稳定 =
            请求.影子验证状态.find("稳定") != std::string::npos
            || 请求.影子验证状态.find("通过") != std::string::npos
            || 请求.影子验证状态.find("已验证") != std::string::npos;
        const bool 当前存在显式冲突 =
            请求.上下文.最近功能域 == 枚举_任务管理功能域::纠偏
            || 请求.上下文.最近根层重判结果 == 枚举_任务管理根层重判结果::停止退出边界
            || 请求.上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::禁止继续
            || 当前动作失败退出;
        const bool 控制请求待确认 =
            请求.存在控制请求
            || (
                私有_P0_控制承接有效(请求.当前控制承接)
                && !请求.当前控制承接.是否已响应
                && 请求.当前控制承接.当前响应状态 != 枚举_任务管理控制响应状态::已拒绝
                && 请求.当前控制承接.当前响应状态 != 枚举_任务管理控制响应状态::已执行完成
            );
        const bool 控制请求已拒绝 =
            请求.当前控制承接.当前响应状态 == 枚举_任务管理控制响应状态::已拒绝
            || !请求.当前控制承接.拒绝原因.empty();
        const bool 当前动作长时间无推进 =
            请求.上下文.当前动作长时间无推进
            || (
                !当前动作已完成
                && !当前动作失败退出
                && (
                    控制响应超时
                    || (
                        当前步骤
                        && 当前步骤->主信息.超时截止时间 != 0
                        && 当前时间 != 0
                        && 当前时间 >= 当前步骤->主信息.超时截止时间
                    )
                    || (
                        最近动作推进时间 != 0
                        && 请求.当前控制承接.最近回报时间 != 0
                        && 请求.当前控制承接.最近回报时间 <= 最近动作推进时间
                        && 控制响应超时
                    )
                )
            );

        结构_高阶排序结果 高阶排序结果 = {};
        if (私有_P0_高阶排序请求有效(请求.当前高阶排序请求)
            || !请求.当前升阶候选组.empty()) {
            (void)私有_P0_生成高阶排序结果(请求, 高阶排序结果);
        }
        const bool 高阶排序已完成 =
            私有_P0_高阶排序结果有效(高阶排序结果)
            && (高阶排序结果.是否等效 || !高阶排序结果.胜出候选主键.empty());

        结构_高阶胜出绑定 高阶胜出绑定 = 请求.当前高阶胜出绑定;
        if (!私有_P0_高阶胜出绑定有效(高阶胜出绑定)
            && 私有_P0_高阶排序结果有效(高阶排序结果)) {
            (void)私有_P0_解析高阶胜出绑定(请求, 高阶排序结果, 高阶胜出绑定);
        }
        const bool 高阶排序胜出已绑定 =
            私有_P0_高阶胜出绑定有效(高阶胜出绑定)
            && 高阶胜出绑定.已绑定执行方法;

        const bool 当前步骤可复用 =
            请求.上下文.当前步骤可复用
            || (
                当前步骤
                && 当前步骤->主信息.节点种类 == 枚举_任务节点种类::步骤节点
                && 私有_任务节点归属宿主任务(当前步骤, 宿主任务)
                && 当前步骤->子 == nullptr
            );

        const bool 业务子任务应转执行 = [&]() noexcept {
            if (请求.上下文.业务子任务应转执行) {
                return true;
            }
            if (!私有_任务为业务子任务(宿主任务)) {
                return false;
            }
            if (!当前方法) {
                return false;
            }
            if (请求.上下文.当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
                || 请求.上下文.当前下一步去向 == 枚举_任务管理下一步去向::绑定方法) {
                return false;
            }
            if (私有_查找直接派生子任务(宿主任务)) {
                return false;
            }
            if (!最近结果
                || 最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
                || !私有_任务节点归属宿主任务(最近结果, 宿主任务)) {
                return false;
            }

            const auto* 最近步骤 = 最近结果->父
                ? static_cast<const 任务节点*>(最近结果->父)
                : nullptr;
            if (!最近步骤 || 最近步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点) {
                return false;
            }

            static const 词性节点类* s_业务回接位类型 = 语素集.添加词性词("业务回接位", "名词");
            return 私有_词性相同(最近步骤->主信息.类型, s_业务回接位类型);
        }();

        const bool 当前可回接最近结果 =
            请求.上下文.当前可回接最近结果
            || (
                最近结果
                && 最近结果->主信息.节点种类 == 枚举_任务节点种类::结果节点
                && 最近结果->主信息.状态 == 枚举_任务状态::完成
                && 私有_任务节点归属宿主任务(最近结果, 宿主任务)
                && (!当前方法 || 最近结果方法 == 当前方法)
            );

        const bool 业务子任务已完成 = [&]() noexcept {
            if (请求.上下文.业务子任务已完成) {
                return true;
            }
            if (!私有_任务为业务子任务(宿主任务)) {
                return false;
            }
            if (私有_查找直接派生子任务(宿主任务)) {
                return false;
            }
            if (!最近结果
                || 最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
                || 最近结果->主信息.状态 != 枚举_任务状态::完成
                || !私有_任务节点归属宿主任务(最近结果, 宿主任务)) {
                return false;
            }

            const auto* 最近步骤 = 最近结果->父
                ? static_cast<const 任务节点*>(最近结果->父)
                : nullptr;
            if (!最近步骤
                || 最近步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点
                || 最近步骤->主信息.状态 != 枚举_任务状态::完成
                || !私有_任务节点归属宿主任务(最近步骤, 宿主任务)) {
                return false;
            }

            static const 词性节点类* s_业务执行位类型 = 语素集.添加词性词("业务执行位", "名词");
            static const 词性节点类* s_业务执行结果类型 = 语素集.添加词性词("业务执行结果", "名词");
            if (!私有_词性相同(最近步骤->主信息.类型, s_业务执行位类型)
                || !私有_词性相同(最近结果->主信息.类型, s_业务执行结果类型)) {
                return false;
            }

            return !当前方法
                || (当前步骤方法 == 当前方法 && 最近结果方法 == 当前方法);
        }();

        const bool 最近结果命中当前治理 =
            请求.上下文.最近结果命中当前治理
            || [&]() noexcept {
                if (!最近结果
                    || 最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点
                    || !私有_任务节点归属宿主任务(最近结果, 宿主任务)) {
                    return false;
                }

                结构_任务管理结果 临时治理{};
                临时治理.上下文 = 请求.上下文;
                临时治理.当前功能域 = 请求.上下文.最近功能域;
                临时治理.当前缺口类型 = 请求.上下文.当前缺口类型;
                临时治理.当前下一步去向 = 请求.上下文.当前下一步去向;
                临时治理.当前方法来源 = 当前方法
                    ? 枚举_任务管理方法来源::已挂方法复用
                    : 枚举_任务管理方法来源::未定义;
                临时治理.当前步骤位类型 =
                    请求.上下文.当前下一步去向 == 枚举_任务管理下一步去向::保持等待
                    ? 枚举_任务管理步骤位类型::等待位
                    : 枚举_任务管理步骤位类型::执行位;
                临时治理.当前步骤语义类型 =
                    临时治理.当前步骤位类型 == 枚举_任务管理步骤位类型::等待位
                    ? 枚举_任务管理步骤语义类型::等待维持步骤
                    : 枚举_任务管理步骤语义类型::治理步骤;
                临时治理.宿主任务新状态 = 宿主任务->主信息.状态;
                临时治理.当前步骤节点 = const_cast<任务节点*>(当前步骤);
                临时治理.最新结果节点 = const_cast<任务节点*>(最近结果);

                if (!私有_词性相同(最近结果->主信息.名称, 私有_治理结果名称词(临时治理))) {
                    return false;
                }
                if (最近结果->主信息.状态 != 私有_治理结果状态(临时治理)) {
                    return false;
                }
                return 当前方法 == 最近结果方法;
            }();

        const bool 当前需等待 = [&]() noexcept {
            if (请求.上下文.当前需等待) {
                return true;
            }
            if (请求.上下文.当前下一步去向 == 枚举_任务管理下一步去向::保持等待) {
                return true;
            }
            if (!宿主任务 || !当前方法) {
                return false;
            }
            if (宿主任务->主信息.状态 != 枚举_任务状态::运行中) {
                return false;
            }
            if (待推进) {
                return false;
            }
            if (!当前步骤 || !最近结果) {
                return false;
            }
            if (当前步骤->主信息.节点种类 != 枚举_任务节点种类::步骤节点
                || 最近结果->主信息.节点种类 != 枚举_任务节点种类::结果节点) {
                return false;
            }
            if (!私有_任务节点归属宿主任务(当前步骤, 宿主任务)
                || !私有_任务节点归属宿主任务(最近结果, 宿主任务)) {
                return false;
            }
            if (最近结果->主信息.状态 != 枚举_任务状态::完成) {
                return false;
            }
            return 当前步骤方法 == 当前方法 && 最近结果方法 == 当前方法;
        }();

        const bool 当前需收束 =
            请求.上下文.当前需收束
            || 请求.上下文.当前下一步去向 == 枚举_任务管理下一步去向::进入收束
            || 私有_任务状态已终结(宿主任务->主信息.状态)
            || 业务子任务已完成
            || (已对齐 && 影子验证稳定);
        const bool 当前动作已形成稳定结果 =
            当前动作已完成
            && !当前动作失败退出
            && (影子验证稳定 || 最近结果命中当前治理 || 业务子任务已完成 || 已对齐);
        const bool 当前动作需重试 =
            请求.上下文.当前动作需重试
            || (
                当前动作失败退出
                && 当前步骤
                && 当前步骤->主信息.允许重试次数 > 当前步骤->主信息.已重试次数
                && !当前动作被中断
                && !当前需收束
            );

        const bool 当前需重筹办 =
            请求.上下文.当前下一步去向 == 枚举_任务管理下一步去向::回到重筹办
            || 请求.上下文.当前缺口类型 == 枚举_任务管理缺口类型::治理任务缺失
            || 请求.上下文.当前缺口类型 == 枚举_任务管理缺口类型::宿主任务缺失
            || 请求.上下文.当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
            || 当前存在显式冲突
            || 控制请求已拒绝;

        const bool 当前已具备复用方法 =
            当前方法 != nullptr
            && 请求.上下文.当前缺口类型 != 枚举_任务管理缺口类型::方法挂点缺失
            && 请求.上下文.当前下一步去向 != 枚举_任务管理下一步去向::绑定方法;
        const bool 稳定等待态复盘学习 =
            宿主任务
            && !待推进
            && 请求.上下文.当前缺口类型 == 枚举_任务管理缺口类型::无缺口
            && 当前需等待
            && !当前动作长时间无推进
            && !当前动作需重试
            && 当前已具备复用方法;

        const bool 任务条件满足 =
            请求.上下文.当前缺口类型 != 枚举_任务管理缺口类型::治理任务缺失
            && 请求.上下文.当前缺口类型 != 枚举_任务管理缺口类型::宿主任务缺失
            && 请求.上下文.当前缺口类型 != 枚举_任务管理缺口类型::方法挂点缺失;

        const bool 可继续推进 =
            请求.上下文.可继续推进
            || (
                任务条件满足
                && !当前存在显式冲突
                && !当前需等待
                && !当前需收束
                && !当前需重筹办
                && (待推进 || 最近结果命中当前治理 || 高阶排序胜出已绑定)
            );

        auto& 证据 = 请求.当前治理二次特征证据;
        证据.有证据 = true;
        证据.任务条件满足 = 任务条件满足;
        证据.目标结果方向一致 = 已对齐 || 待推进;
        证据.目标结果已落范围 = 已对齐;
        证据.当前存在显式冲突 = 当前存在显式冲突;
        证据.当前动作已启动 = 当前动作已启动;
        证据.当前动作已完成 = 当前动作已完成;
        证据.当前动作失败退出 = 当前动作失败退出;
        证据.当前处于方法位推进中 = 当前处于方法位推进中;
        证据.当前处于原语边界执行中 = 当前处于原语边界执行中;
        证据.当前动作已形成稳定结果 = 当前动作已形成稳定结果;
        证据.当前动作长时间无推进 = 当前动作长时间无推进;
        证据.当前动作需重试 = 当前动作需重试;
        证据.当前动作被中断 = 当前动作被中断;
        证据.影子验证稳定 = 影子验证稳定;
        证据.控制请求待确认 = 控制请求待确认;
        证据.控制请求已拒绝 = 控制请求已拒绝;
        证据.高阶排序已完成 = 高阶排序已完成;
        证据.高阶排序胜出已绑定 = 高阶排序胜出已绑定;
        证据.当前需等待 = 当前需等待;
        证据.当前需收束 = 当前需收束;
        证据.当前需重筹办 = 当前需重筹办;
        证据.当前可回接最近结果 = 当前可回接最近结果;
        证据.稳定等待态复盘学习 = 稳定等待态复盘学习;
        证据.业务子任务已完成 = 业务子任务已完成;
        证据.业务子任务应转执行 = 业务子任务应转执行;
        证据.最近结果命中当前治理 = 最近结果命中当前治理;
        证据.当前步骤可复用 = 当前步骤可复用;
        证据.可继续推进 = 可继续推进;
        证据.综合置信度_Q10000 = 8000;

        std::vector<std::string> 片段{};
        if (证据.任务条件满足) 片段.emplace_back("条件满足");
        if (证据.目标结果方向一致) 片段.emplace_back("方向一致");
        if (证据.目标结果已落范围) 片段.emplace_back("结果已落范围");
        if (证据.当前存在显式冲突) 片段.emplace_back("显式冲突");
        if (证据.当前动作已启动) 片段.emplace_back("动作已启动");
        if (证据.当前动作已完成) 片段.emplace_back("动作已完成");
        if (证据.当前动作失败退出) 片段.emplace_back("动作失败退出");
        if (证据.当前处于方法位推进中) 片段.emplace_back("方法位推进中");
        if (证据.当前处于原语边界执行中) 片段.emplace_back("原语边界执行中");
        if (证据.当前动作已形成稳定结果) 片段.emplace_back("动作形成稳定结果");
        if (证据.当前动作长时间无推进) 片段.emplace_back("动作长时间无推进");
        if (证据.当前动作需重试) 片段.emplace_back("动作需重试");
        if (证据.当前动作被中断) 片段.emplace_back("动作被中断");
        if (证据.影子验证稳定) 片段.emplace_back("影子验证稳定");
        if (证据.控制请求待确认) 片段.emplace_back("控制待确认");
        if (证据.控制请求已拒绝) 片段.emplace_back("控制已拒绝");
        if (证据.高阶排序已完成) 片段.emplace_back("高阶排序已完成");
        if (证据.高阶排序胜出已绑定) 片段.emplace_back("高阶胜出已绑定");
        if (证据.当前需等待) 片段.emplace_back("需等待");
        if (证据.当前需收束) 片段.emplace_back("需收束");
        if (证据.当前需重筹办) 片段.emplace_back("需重筹办");
        if (证据.当前可回接最近结果) 片段.emplace_back("可回接最近结果");
        if (证据.稳定等待态复盘学习) 片段.emplace_back("稳定等待复盘学习");
        if (证据.业务子任务已完成) 片段.emplace_back("业务子任务已完成");
        if (证据.业务子任务应转执行) 片段.emplace_back("业务子任务应转执行");
        if (证据.最近结果命中当前治理) 片段.emplace_back("最近结果命中当前治理");
        if (证据.当前步骤可复用) 片段.emplace_back("当前步骤可复用");
        if (证据.可继续推进) 片段.emplace_back("可继续推进");
        证据.摘要 = 片段.empty() ? "无治理证据" : 私有_拼接片段(片段);

        const auto 写布尔证据 = [&](const char* 概念名称, const bool 值, const char* 原因摘要) noexcept {
            return 私有_P1_写任务治理二次特征_布尔(
                自我对象,
                宿主任务,
                概念名称,
                值,
                原因摘要,
                证据.综合置信度_Q10000,
                调用点 + "/" + 概念名称);
        };

        (void)写布尔证据("任务条件满足", 证据.任务条件满足, 证据.任务条件满足 ? "已具备最小前提" : "仍存在结构性缺口");
        (void)写布尔证据("目标结果方向一致", 证据.目标结果方向一致, 证据.目标结果方向一致 ? "目标结果仍同向" : "目标结果方向未明");
        (void)写布尔证据("目标结果已落范围", 证据.目标结果已落范围, 证据.目标结果已落范围 ? "结果已落入目标范围" : "结果尚未落入目标范围");
        (void)写布尔证据("当前存在显式冲突", 证据.当前存在显式冲突, 证据.当前存在显式冲突 ? "当前命中纠偏/禁止继续边界" : "当前未发现显式冲突");
        (void)写布尔证据("当前动作已启动", 证据.当前动作已启动, 证据.当前动作已启动 ? "当前动作边界已启动" : "当前动作尚未进入启动边界");
        (void)写布尔证据("当前动作已完成", 证据.当前动作已完成, 证据.当前动作已完成 ? "当前动作边界已完成" : "当前动作尚未进入完成边界");
        (void)写布尔证据("当前动作失败退出", 证据.当前动作失败退出, 证据.当前动作失败退出 ? "当前动作已失败退出" : "当前动作未命中失败退出");
        (void)写布尔证据("当前处于方法位推进中", 证据.当前处于方法位推进中, 证据.当前处于方法位推进中 ? "当前方法位正在推进" : "当前方法位未处于推进态");
        (void)写布尔证据("当前处于原语边界执行中", 证据.当前处于原语边界执行中, 证据.当前处于原语边界执行中 ? "当前仍处于原语边界执行中" : "当前未处于原语边界执行中");
        (void)写布尔证据("当前动作已形成稳定结果", 证据.当前动作已形成稳定结果, 证据.当前动作已形成稳定结果 ? "当前动作已形成稳定结果" : "当前动作尚未形成稳定结果");
        (void)写布尔证据("当前动作长时间无推进", 证据.当前动作长时间无推进, 证据.当前动作长时间无推进 ? "当前动作已命中超时/长时间无推进" : "当前动作未命中超时或停滞");
        (void)写布尔证据("当前动作需重试", 证据.当前动作需重试, 证据.当前动作需重试 ? "当前动作失败但仍具备重试空间" : "当前动作当前不需要重试");
        (void)写布尔证据("当前动作被中断", 证据.当前动作被中断, 证据.当前动作被中断 ? "当前动作已被控制链打断" : "当前动作未被控制链打断");
        (void)写布尔证据("影子验证稳定", 证据.影子验证稳定, 证据.影子验证稳定 ? "影子验证已稳定" : "影子验证尚未稳定");
        (void)写布尔证据("控制请求待确认", 证据.控制请求待确认, 证据.控制请求待确认 ? "控制请求仍待确认" : "当前无待确认控制请求");
        (void)写布尔证据("控制请求已拒绝", 证据.控制请求已拒绝, 证据.控制请求已拒绝 ? "控制请求已被拒绝" : "当前无被拒绝控制请求");
        (void)写布尔证据("高阶排序已完成", 证据.高阶排序已完成, 证据.高阶排序已完成 ? "同轴候选已完成高阶排序" : "当前未完成高阶排序");
        (void)写布尔证据("高阶排序胜出已绑定", 证据.高阶排序胜出已绑定, 证据.高阶排序胜出已绑定 ? "胜出候选已落到执行绑定" : "胜出候选尚未绑定执行");
        (void)写布尔证据("当前需等待", 证据.当前需等待, 证据.当前需等待 ? "当前应保持等待" : "当前不需要进入等待");
        (void)写布尔证据("当前需收束", 证据.当前需收束, 证据.当前需收束 ? "当前应进入收束" : "当前未命中收束判据");
        (void)写布尔证据("当前需重筹办", 证据.当前需重筹办, 证据.当前需重筹办 ? "当前应回到重筹办" : "当前不需要重筹办");
        (void)写布尔证据("当前可回接最近结果", 证据.当前可回接最近结果, 证据.当前可回接最近结果 ? "最近结果可回接" : "最近结果当前不可回接");
        (void)写布尔证据("稳定等待态复盘学习", 证据.稳定等待态复盘学习, 证据.稳定等待态复盘学习 ? "当前进入稳定等待，可触发复盘学习" : "当前未命中稳定等待复盘学习");
        (void)写布尔证据("业务子任务已完成", 证据.业务子任务已完成, 证据.业务子任务已完成 ? "业务子任务已完成执行" : "业务子任务尚未完成执行");
        (void)写布尔证据("业务子任务应转执行", 证据.业务子任务应转执行, 证据.业务子任务应转执行 ? "业务子任务当前应从回接位转入执行位" : "业务子任务当前不应转入执行位");
        (void)写布尔证据("最近结果命中当前治理", 证据.最近结果命中当前治理, 证据.最近结果命中当前治理 ? "最近结果命中当前治理结果" : "最近结果未命中当前治理结果");
        (void)写布尔证据("当前步骤可复用", 证据.当前步骤可复用, 证据.当前步骤可复用 ? "当前步骤可复用" : "当前步骤不可复用");
        (void)写布尔证据("可继续推进", 证据.可继续推进, 证据.可继续推进 ? "当前允许继续推进" : "当前不宜继续推进");
        return true;
    }

    const 词性节点类* 私有_任务方法实例场景名称词(
        const 词性节点类* 基础名称,
        const char* 后缀,
        时间戳 now) noexcept
    {
        auto 标题 = 私有_安全词(基础名称);
        if (标题.empty()) {
            标题 = "任务方法实例";
        }
        标题 += "_";
        标题 += (后缀 && *后缀) ? 后缀 : "场景";
        if (now != 0) {
            标题 += "_";
            标题 += std::to_string(now);
        }
        return 语素集.添加词性词(标题, "专有名词");
    }

    void 私有_按模板场景初始化任务实例场景(
        场景节点类* 实例场景,
        场景节点类* 模板场景) noexcept
    {
        if (!实例场景 || !模板场景) {
            return;
        }

        const auto* 模板主信息 = 世界树.场景().取场景主信息(模板场景);
        if (!模板主信息) {
            return;
        }

        for (const auto& 二次特征引用 : 模板主信息->关系索引) {
            auto* 节点 = 二次特征引用.指针;
            if (!节点 && !二次特征引用.主键.empty()) {
                节点 = static_cast<二次特征节点类*>(世界树.基础信息().查找主键(二次特征引用.主键));
            }
            if (节点) {
                (void)世界树.场景().追加关系(实例场景, 节点);
            }
        }

        for (const auto& 二次特征引用 : 模板主信息->二次特征索引) {
            auto* 节点 = 二次特征引用.指针;
            if (!节点 && !二次特征引用.主键.empty()) {
                节点 = static_cast<二次特征节点类*>(世界树.基础信息().查找主键(二次特征引用.主键));
            }
            if (节点) {
                (void)世界树.场景().追加二次特征(实例场景, 节点);
            }
        }
    }

    方法节点* 私有_查找方法模板直接子节点(
        方法节点* 方法头,
        枚举_方法节点种类 节点种类) noexcept
    {
        if (!方法头 || !方法头->子) {
            return nullptr;
        }

        auto* 起点 = static_cast<方法节点*>(方法头->子);
        auto* 当前 = 起点;
        do {
            if (当前
                && 当前->主信息.节点种类 == 节点种类
                && !当前->主信息.来源任务.有效()) {
                return 当前;
            }
            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 起点);

        当前 = 起点;
        do {
            if (当前 && 当前->主信息.节点种类 == 节点种类) {
                return 当前;
            }
            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 起点);

        return nullptr;
    }

    场景节点类* 私有_创建任务方法实例场景(
        任务节点* 宿主任务,
        const 词性节点类* 基础名称,
        场景节点类* 模板场景,
        const char* 后缀,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        (void)调用点;
        if (!宿主任务) {
            return nullptr;
        }
        if (now == 0) {
            now = 结构体_时间戳::当前_微秒();
        }

        auto* 承载世界 = 宿主任务->主信息.任务虚拟存在.指针
            ? 世界树.取或创建存在内部世界(宿主任务->主信息.任务虚拟存在.指针)
            : nullptr;
        auto* 场景父锚点 = 承载世界
            ? reinterpret_cast<基础信息节点类*>(承载世界)
            : reinterpret_cast<基础信息节点类*>(宿主任务->主信息.场景.指针);
        if (!场景父锚点) {
            return nullptr;
        }

        auto* 场景 = 世界树.取或创建子场景_按名称(
            场景父锚点,
            私有_任务方法实例场景名称词(基础名称, 后缀, now),
            枚举_世界类型::内部世界);
        if (场景 && 宿主任务->主信息.任务虚拟存在.指针) {
            (void)世界树.场景().绑定宿主(场景, 宿主任务->主信息.任务虚拟存在.指针);
        }
        私有_按模板场景初始化任务实例场景(场景, 模板场景);
        return 场景;
    }

    任务节点* 私有_创建治理步骤节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务) {
            return nullptr;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);
        auto* 挂接父节点 = 宿主任务;
        if (结果.上下文.最近结果
            && 结果.上下文.最近结果->主信息.节点种类 == 枚举_任务节点种类::结果节点
            && 私有_任务节点归属宿主任务(结果.上下文.最近结果, 宿主任务)) {
            挂接父节点 = 结果.上下文.最近结果;
        }

        auto* 场景 = 宿主任务->主信息.场景.指针
            ? 宿主任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());
        auto* 方法条件模板节点 = 当前方法
            ? 私有_查找方法模板直接子节点(当前方法, 枚举_方法节点种类::方法条件节点)
            : nullptr;
        auto* 方法条件模板场景 = 方法条件模板节点 && 方法条件模板节点->主信息.条件场景.指针
            ? 方法条件模板节点->主信息.条件场景.指针
            : (当前方法 ? 当前方法->主信息.条件场景.指针 : nullptr);

        任务主信息类 主信息{};
        主信息.启动时间 = now;

        主信息.名称 = 私有_治理步骤名称词(结果);
        主信息.类型 = 私有_治理步骤类型词(结果);
        主信息.节点种类 = 枚举_任务节点种类::步骤节点;
        主信息.状态 = 当前方法 ? 枚举_任务状态::运行中 : 枚举_任务状态::挂起;
        主信息.基准优先级 = 宿主任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 宿主任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 宿主任务->主信息.调度优先级;
        主信息.统计.记录观测(now);
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 挂接父节点);
        私有_写入引用(主信息.场景, 场景);
        私有_写入引用(主信息.执行方法, 当前方法);
        if (当前方法) {
            主信息.动作句柄 = 当前方法->主信息.动作句柄;
        }
        if (结果.最新状态) {
            私有_写入引用(主信息.结果状态信息, 结果.最新状态);
        }
        if (当前方法) {
            if (auto* 方法条件场景 = 私有_创建任务方法实例场景(
                    宿主任务,
                    主信息.名称,
                    方法条件模板场景,
                    "方法条件",
                    now,
                    调用点 + "/方法条件节点")) {
                私有_写入引用(主信息.方法条件节点, 方法条件场景);
            }
        }

        auto* 步骤节点 = 私有_独立树添加子节点<任务类>(挂接父节点, 主信息);
        if (!步骤节点) {
            return nullptr;
        }

        私有_写入引用(挂接父节点->主信息.后续任务, 步骤节点);
        结果.当前步骤节点 = 步骤节点;
        结果.上下文.当前步骤 = 步骤节点;
        结果.已创建步骤节点 = true;
        return 步骤节点;
    }

    任务节点* 私有_创建治理结果节点(
        自我类& 自我对象,
        任务节点* 宿主任务,
        任务节点* 步骤节点,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务 || !步骤节点) {
            return nullptr;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);
        auto* 场景 = 宿主任务->主信息.场景.指针
            ? 宿主任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());
        auto* 方法结果模板节点 = 当前方法
            ? 私有_查找方法模板直接子节点(当前方法, 枚举_方法节点种类::方法结果节点)
            : nullptr;
        auto* 方法结果模板场景 = 方法结果模板节点 && 方法结果模板节点->主信息.结果场景.指针
            ? 方法结果模板节点->主信息.结果场景.指针
            : (当前方法 ? 当前方法->主信息.结果场景.指针 : nullptr);

        任务主信息类 主信息{};
        主信息.名称 = 私有_治理结果名称词(结果);
        主信息.类型 = 私有_治理结果类型词(结果);
        主信息.节点种类 = 枚举_任务节点种类::结果节点;
        主信息.状态 = 私有_治理结果状态(结果);
        主信息.启动时间 = 步骤节点->主信息.启动时间 != 0 ? 步骤节点->主信息.启动时间 : now;
        主信息.完成时间 = now;
        主信息.基准优先级 = 宿主任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 宿主任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 宿主任务->主信息.调度优先级;
        主信息.统计.记录观测(now);
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 步骤节点);
        私有_写入引用(主信息.场景, 场景);
        私有_写入引用(主信息.执行方法, 当前方法);
        if (当前方法) {
            主信息.动作句柄 = 当前方法->主信息.动作句柄;
        }
        if (结果.最新状态) {
            私有_写入引用(主信息.结果状态信息, 结果.最新状态);
        }
        if (当前方法) {
            if (auto* 方法结果场景 = 私有_创建任务方法实例场景(
                    宿主任务,
                    主信息.名称,
                    方法结果模板场景,
                    "方法结果",
                    now,
                    调用点 + "/方法结果节点")) {
                私有_写入引用(主信息.方法结果节点, 方法结果场景);
            }
        }

        auto* 结果节点 = 私有_独立树添加子节点<任务类>(步骤节点, 主信息);
        if (!结果节点) {
            return nullptr;
        }

        步骤节点->主信息.状态 = 枚举_任务状态::完成;
        if (步骤节点->主信息.启动时间 == 0) {
            步骤节点->主信息.启动时间 = now;
        }
        步骤节点->主信息.完成时间 = now;
        步骤节点->主信息.统计.记录观测(now);
        if (结果.最新状态) {
            私有_写入引用(步骤节点->主信息.结果状态信息, 结果.最新状态);
            私有_写入引用(宿主任务->主信息.结果状态信息, 结果.最新状态);
        }
        私有_写入引用(步骤节点->主信息.后续任务, 结果节点);

        结果.最新结果节点 = 结果节点;
        结果.上下文.最近结果 = 结果节点;
        结果.已创建结果节点 = true;
        return 结果节点;
    }

    任务节点* 私有_创建或复用补条件子任务头(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务) {
            return nullptr;
        }

        if (私有_P0_当前步骤语义类型(结果) != 枚举_任务管理步骤语义类型::补条件步骤) {
            return nullptr;
        }

        if (私有_词性相同(宿主任务->主信息.类型, 私有_补条件子任务类型词())) {
            return nullptr;
        }

        auto* 现有子任务 = 私有_查找直接子节点_按谓词<任务节点>(宿主任务, [&](const 任务节点* 节点) noexcept {
            return 节点
                && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                && 私有_词性相同(节点->主信息.名称, 私有_补条件子任务名称词(宿主任务, 结果))
                && !私有_任务状态已终结(节点->主信息.状态);
        });

        if (现有子任务) {
            结果.当前子任务节点 = 现有子任务;
            结果.已复用补条件子任务 = true;
            return 现有子任务;
        }

        auto* 场景 = 宿主任务->主信息.场景.指针
            ? 宿主任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.名称 = 私有_补条件子任务名称词(宿主任务, 结果);
        主信息.类型 = 私有_补条件子任务类型词();
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::未启动;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 宿主任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 宿主任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 宿主任务->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 宿主任务);
        私有_写入引用(主信息.场景, 场景);

        auto* 子任务头 = 私有_独立树添加子节点<任务类>(宿主任务, 主信息);
        if (!子任务头) {
            return nullptr;
        }

        结果.当前子任务节点 = 子任务头;
        结果.已创建补条件子任务 = true;
        return 子任务头;
    }

    任务节点* 私有_创建或复用业务子任务头(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        const bool 长期策略驱动业务派生 = 私有_M10_长期策略驱动业务子任务派生(结果);
        if (!宿主任务
            || (!结果.宿主目标结果待推进
                && !(长期策略驱动业务派生 && 私有_M10_存在业务推进压力(结果)))) {
            return nullptr;
        }

        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        if (当前步骤语义类型 != 枚举_任务管理步骤语义类型::回接派生步骤
            && 当前步骤语义类型 != 枚举_任务管理步骤语义类型::业务推进步骤
            && !长期策略驱动业务派生) {
            return nullptr;
        }

        if (长期策略驱动业务派生
            && 当前步骤语义类型 != 枚举_任务管理步骤语义类型::回接派生步骤
            && 当前步骤语义类型 != 枚举_任务管理步骤语义类型::业务推进步骤) {
            结果.当前步骤语义类型 = 枚举_任务管理步骤语义类型::业务推进步骤;
        }

        if (私有_任务为业务子任务(宿主任务)) {
            return nullptr;
        }

        auto* 现有子任务 = 私有_查找直接子节点_按谓词<任务节点>(宿主任务, [&](const 任务节点* 节点) noexcept {
            return 节点
                && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                && 私有_词性相同(节点->主信息.名称, 私有_业务子任务名称词(宿主任务, 结果))
                && !私有_任务状态已终结(节点->主信息.状态);
        });

        if (现有子任务) {
            结果.当前子任务节点 = 现有子任务;
            结果.已复用业务子任务 = true;
            return 现有子任务;
        }

        auto* 场景 = 宿主任务->主信息.场景.指针
            ? 宿主任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());
        auto* 当前方法 = 私有_P0_当前方法(结果);

        任务主信息类 主信息{};
        主信息.名称 = 私有_业务子任务名称词(宿主任务, 结果);
        主信息.类型 = 私有_业务子任务类型词();
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::未启动;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 宿主任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 宿主任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 宿主任务->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 宿主任务);
        私有_写入引用(主信息.场景, 场景);
        私有_写入引用(主信息.执行方法, 当前方法);
        if (当前方法) {
            主信息.动作句柄 = 当前方法->主信息.动作句柄;
        }

        auto* 子任务头 = 私有_独立树添加子节点<任务类>(宿主任务, 主信息);
        if (!子任务头) {
            return nullptr;
        }

        结果.当前子任务节点 = 子任务头;
        结果.已创建业务子任务 = true;
        return 子任务头;
    }

    任务节点* 私有_确保任务管理学习树根(
        自我类& 自我对象,
        任务节点* 管理任务,
        const 结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        (void)结果;
        if (!管理任务) {
            return nullptr;
        }

        if (auto* 现有根 = 私有_查找任务管理学习树根(管理任务)) {
            return 现有根;
        }

        auto* 场景 = 管理任务->主信息.场景.指针
            ? 管理任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.名称 = 私有_任务管理学习树名称词();
        主信息.类型 = 私有_任务管理学习树类型词();
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::运行中;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 管理任务->主信息.基准优先级;
        主信息.局部优先级偏移 = 管理任务->主信息.局部优先级偏移;
        主信息.调度优先级 = 管理任务->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 管理任务);
        私有_写入引用(主信息.场景, 场景);

        return 私有_独立树添加子节点<任务类>(管理任务, 主信息);
    }

    bool 私有_稳定等待态复盘学习(
        const 结构_任务管理结果& 结果) noexcept
    {
        return 私有_P0_稳定等待态复盘学习(结果);
    }

    任务节点* 私有_确保定向学习分支(
        自我类& 自我对象,
        任务节点* 学习树根,
        const 结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        (void)结果;
        if (!学习树根) {
            return nullptr;
        }

        if (auto* 现有分支 = 私有_查找定向学习分支(学习树根)) {
            return 现有分支;
        }

        auto* 场景 = 学习树根->主信息.场景.指针
            ? 学习树根->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.名称 = 私有_定向学习分支名称词();
        主信息.类型 = 私有_学习分支类型词();
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::运行中;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 学习树根->主信息.基准优先级;
        主信息.局部优先级偏移 = 学习树根->主信息.局部优先级偏移;
        主信息.调度优先级 = 学习树根->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 学习树根);
        私有_写入引用(主信息.场景, 场景);

        return 私有_独立树添加子节点<任务类>(学习树根, 主信息);
    }

    任务节点* 私有_确保广泛学习分支(
        自我类& 自我对象,
        任务节点* 学习树根,
        const 结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        (void)结果;
        if (!学习树根) {
            return nullptr;
        }

        if (auto* 现有分支 = 私有_查找广泛学习分支(学习树根)) {
            return 现有分支;
        }

        auto* 场景 = 学习树根->主信息.场景.指针
            ? 学习树根->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.名称 = 私有_广泛学习分支名称词();
        主信息.类型 = 私有_学习分支类型词();
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::运行中;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 学习树根->主信息.基准优先级;
        主信息.局部优先级偏移 = 学习树根->主信息.局部优先级偏移;
        主信息.调度优先级 = 学习树根->主信息.调度优先级;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        私有_写入引用(主信息.父任务, 学习树根);
        私有_写入引用(主信息.场景, 场景);

        return 私有_独立树添加子节点<任务类>(学习树根, 主信息);
    }

    bool 私有_应创建学习子任务(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        if (!宿主任务) {
            return false;
        }
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前方法来源 = 私有_P0_当前方法来源(结果);
        const auto 当前本能能力缺口 = 私有_P0_当前本能缺口(结果);
        const bool 稳定等待态复盘学习 = 私有_稳定等待态复盘学习(结果);
        if (私有_本能缺口属于原语或治理补齐(当前本能能力缺口)
            || 当前本能能力缺口 == 枚举_任务管理本能能力缺口类型::需学习补齐) {
            return true;
        }
        if (当前缺口类型 == 枚举_任务管理缺口类型::方法挂点缺失
            || 当前下一步去向 == 枚举_任务管理下一步去向::绑定方法) {
            return true;
        }
        if (稳定等待态复盘学习) {
            return true;
        }
        return 结果.宿主目标结果待推进
            && 当前方法来源 == 枚举_任务管理方法来源::默认本能兜底;
    }

    const char* 私有_学习通道文本(const 结构_学习承接决策& 决策) noexcept
    {
        return 决策.补原语或治理本能 ? "补原语/治理本能" : "补方法";
    }

    const char* 私有_学习目标类型文本(枚举_学习目标类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_学习目标类型::让方法可用: return "让方法可用";
        case 枚举_学习目标类型::修正失败方法: return "修正失败方法";
        case 枚举_学习目标类型::迁移已有方法: return "迁移已有方法";
        case 枚举_学习目标类型::压缩相似方法: return "压缩相似方法";
        case 枚举_学习目标类型::分化高冲突方法: return "分化高冲突方法";
        case 枚举_学习目标类型::提升方法优选级: return "提升方法优选级";
        default: return "未定义";
        }
    }

    const char* 私有_学习需求类型文本(枚举_学习需求类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_学习需求类型::方法可用需求: return "方法可用需求";
        case 枚举_学习需求类型::方法修正需求: return "方法修正需求";
        case 枚举_学习需求类型::方法迁移需求: return "方法迁移需求";
        case 枚举_学习需求类型::方法优选需求: return "方法优选需求";
        default: return "未定义";
        }
    }

    const char* 私有_学习动作类型文本(枚举_学习动作类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_学习动作类型::样本收集: return "样本收集";
        case 枚举_学习动作类型::差异比较: return "差异比较";
        case 枚举_学习动作类型::共同条件提取: return "共同条件提取";
        case 枚举_学习动作类型::候选动作生成: return "候选动作生成";
        case 枚举_学习动作类型::结果预测: return "结果预测";
        case 枚举_学习动作类型::沙箱验证: return "沙箱验证";
        case 枚举_学习动作类型::风险评估: return "风险评估";
        case 枚举_学习动作类型::边界标记: return "边界标记";
        case 枚举_学习动作类型::可用层进入判断: return "可用层进入判断";
        case 枚举_学习动作类型::优选层进入判断: return "优选层进入判断";
        default: return "未定义";
        }
    }

    const char* 私有_学习原函数类型文本(枚举_学习原函数类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_学习原函数类型::读取特征: return "读取特征";
        case 枚举_学习原函数类型::收集样本: return "收集样本";
        case 枚举_学习原函数类型::比较样本: return "比较样本";
        case 枚举_学习原函数类型::提取共同点: return "提取共同点";
        case 枚举_学习原函数类型::提取差异点: return "提取差异点";
        case 枚举_学习原函数类型::生成候选条件: return "生成候选条件";
        case 枚举_学习原函数类型::生成候选动作: return "生成候选动作";
        case 枚举_学习原函数类型::生成候选结果: return "生成候选结果";
        case 枚举_学习原函数类型::执行沙箱验证: return "执行沙箱验证";
        case 枚举_学习原函数类型::计算二次特征: return "计算二次特征";
        case 枚举_学习原函数类型::标记边界: return "标记边界";
        case 枚举_学习原函数类型::回滚候选: return "回滚候选";
        case 枚举_学习原函数类型::注册方法: return "注册方法";
        default: return "未定义";
        }
    }

    const char* 私有_学习程序决策文本(枚举_学习程序决策 决策) noexcept
    {
        switch (决策) {
        case 枚举_学习程序决策::继续执行: return "继续执行";
        case 枚举_学习程序决策::补充样本: return "补充样本";
        case 枚举_学习程序决策::拆分候选方法: return "拆分候选方法";
        case 枚举_学习程序决策::合并候选方法: return "合并候选方法";
        case 枚举_学习程序决策::降权观察: return "降权观察";
        case 枚举_学习程序决策::试运行: return "试运行";
        case 枚举_学习程序决策::转正: return "转正";
        case 枚举_学习程序决策::暂停: return "暂停";
        case 枚举_学习程序决策::废弃: return "废弃";
        default: return "未定义";
        }
    }

    结构_学习目标描述 私有_构建学习目标描述(
        const 结构_学习承接决策& 决策,
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_学习目标描述 输出{};
        auto* 当前方法 = 私有_P0_当前方法(结果);
        输出.目标方法头节点 = 当前方法;
        输出.目标学习方法节点 = 当前方法;
        const auto 条件数量 = 私有_学习方法条件数量(当前方法);
        const auto 结果数量 = 私有_学习方法结果数量(当前方法);
        const bool 有动作骨架 = 私有_学习方法有动作骨架(当前方法);
        const bool 已具备可用骨架 = 私有_学习方法已具备可用骨架(当前方法);
        if (已具备可用骨架 && 决策.允许正式资产提交) {
            输出.类型 = 枚举_学习目标类型::提升方法优选级;
        }
        else if (结果.当前治理二次特征证据.当前存在显式冲突) {
            输出.类型 = 枚举_学习目标类型::修正失败方法;
        }
        else {
            输出.类型 = 枚举_学习目标类型::让方法可用;
        }
        输出.最大风险值_Q10000 = 结果.允许正式资产提交 ? 2000 : 4500;
        输出.最大资源消耗_Q10000 = 5000;
        std::ostringstream 摘要;
        摘要 << "让方法可用"
            << " | 目标方法=" << 私有_方法标题(输出.目标方法头节点)
            << " | 有动作骨架=" << (有动作骨架 ? "是" : "否")
            << " | 条件数量=" << 条件数量
            << " | 结果数量=" << 结果数量
            << " | 当前可用骨架=" << (已具备可用骨架 ? "是" : "否")
            << " | 最大风险=" << 输出.最大风险值_Q10000;
        输出.摘要 = 摘要.str();
        return 输出;
    }

    std::vector<结构_学习需求描述> 私有_构建学习需求集合(
        const 结构_学习承接决策& 决策,
        const 结构_任务管理结果& 结果) noexcept
    {
        std::vector<结构_学习需求描述> 输出{};
        auto* 当前方法 = 决策.学习目标.目标方法头节点
            ? 决策.学习目标.目标方法头节点
            : 私有_P0_当前方法(结果);
        const auto 条件数量 = 私有_学习方法条件数量(当前方法);
        const auto 结果数量 = 私有_学习方法结果数量(当前方法);
        const bool 有动作骨架 = 私有_学习方法有动作骨架(当前方法);
        const bool 已具备可用骨架 = 私有_学习方法已具备可用骨架(当前方法);
        const bool 有抽象因果 = 结果.当前需求位特征.最近抽象因果 != nullptr
            || !结果.当前需求位特征.最近抽象因果摘要.empty();
        auto 添加需求 = [&](枚举_学习需求类型 类型,
                           std::int64_t 优先级,
                           std::int64_t 完成度_Q10000,
                           std::int64_t 风险上限_Q10000,
                           bool 必须完成,
                           bool 可延迟,
                           const std::string& 原因) {
            结构_学习需求描述 需求{};
            需求.类型 = 类型;
            需求.优先级 = 优先级;
            需求.完成度_Q10000 = 完成度_Q10000;
            需求.风险上限_Q10000 = 风险上限_Q10000;
            需求.必须完成 = 必须完成;
            需求.可延迟 = 可延迟;
            std::ostringstream 摘要;
            摘要 << 私有_学习需求类型文本(类型)
                << " | 优先级=" << 优先级
                << " | 完成度=" << 完成度_Q10000
                << " | 风险上限=" << 风险上限_Q10000
                << " | 原因=" << 原因;
            需求.摘要 = 摘要.str();
            输出.push_back(std::move(需求));
        };

        if (结果.当前治理二次特征证据.当前存在显式冲突) {
            添加需求(
                枚举_学习需求类型::方法修正需求,
                96,
                0,
                2600,
                true,
                false,
                "当前方法在运行中出现显式冲突，需要修正已有方法");
        }
        if (!已具备可用骨架) {
            添加需求(
                枚举_学习需求类型::方法可用需求,
                98,
                0,
                3200,
                true,
                false,
                "当前待学习方法头仍未达到可用，需要继续补齐条件、动作、结果与验证");
        }
        if (有抽象因果
            && 有动作骨架
            && 条件数量 > 0
            && 结果数量 > 0
            && !结果.允许正式资产提交
            && 结果.当前需求位特征.当前学习承接类型 == 枚举_任务管理学习承接类型::广泛学习) {
            添加需求(
                枚举_学习需求类型::方法迁移需求,
                72,
                3500,
                2200,
                false,
                true,
                "当前方法已有抽象因果与骨架，可继续迁移到更广的任务场景");
        }
        if (已具备可用骨架 && 决策.允许正式资产提交) {
            添加需求(
                枚举_学习需求类型::方法优选需求,
                80,
                5000,
                1200,
                false,
                true,
                "当前方法已可用，可继续进入优选与正式使用判断");
        }
        if (输出.empty()) {
            添加需求(
                枚举_学习需求类型::方法可用需求,
                60,
                0,
                3000,
                false,
                true,
                "默认先围绕让当前方法可用继续学习");
        }
        return 输出;
    }

    std::vector<结构_学习动作描述> 私有_构建学习动作集合(
        const 结构_学习承接决策& 决策,
        const 结构_任务管理结果& 结果) noexcept
    {
        std::vector<结构_学习动作描述> 输出{};
        auto* 当前方法 = 决策.学习目标.目标方法头节点
            ? 决策.学习目标.目标方法头节点
            : 私有_P0_当前方法(结果);
        const auto 条件数量 = 私有_学习方法条件数量(当前方法);
        const auto 结果数量 = 私有_学习方法结果数量(当前方法);
        const bool 有动作骨架 = 私有_学习方法有动作骨架(当前方法);
        const bool 已具备可用骨架 = 私有_学习方法已具备可用骨架(当前方法);
        const bool 有抽象因果 = 结果.当前需求位特征.最近抽象因果 != nullptr
            || !结果.当前需求位特征.最近抽象因果摘要.empty();
        auto 添加动作 = [&](枚举_学习动作类型 类型,
                           枚举_学习需求类型 来源需求类型,
                           I64 优先级,
                           bool 必须执行,
                           const std::string& 原因) {
            for (const auto& 已有 : 输出) {
                if (已有.类型 == 类型 && 已有.来源需求类型 == 来源需求类型) {
                    return;
                }
            }
            结构_学习动作描述 动作{};
            动作.类型 = 类型;
            动作.来源需求类型 = 来源需求类型;
            动作.优先级 = 优先级;
            动作.必须执行 = 必须执行;
            std::ostringstream 摘要;
            摘要 << 私有_学习动作类型文本(类型)
                << " | 来源需求=" << 私有_学习需求类型文本(来源需求类型)
                << " | 优先级=" << 优先级
                << " | 必须执行=" << (必须执行 ? "是" : "否")
                << " | 原因=" << 原因;
            动作.摘要 = 摘要.str();
            输出.push_back(std::move(动作));
        };

        if (!有抽象因果) {
            添加动作(
                枚举_学习动作类型::样本收集,
                枚举_学习需求类型::方法可用需求,
                96,
                true,
                "当前方法缺少可复用抽象因果，先补样本与基础证据");
        }
        if (结果.当前治理二次特征证据.当前存在显式冲突) {
            添加动作(
                枚举_学习动作类型::差异比较,
                枚举_学习需求类型::方法修正需求,
                98,
                true,
                "当前方法存在显式冲突，需要先比较正反样本差异");
            添加动作(
                枚举_学习动作类型::风险评估,
                枚举_学习需求类型::方法修正需求,
                92,
                true,
                "当前方法存在冲突，需要重评估风险与回退边界");
        }
        if (条件数量 <= 0) {
            添加动作(
                枚举_学习动作类型::共同条件提取,
                枚举_学习需求类型::方法可用需求,
                97,
                true,
                "当前方法缺条件模板，需要先补条件");
        }
        if (!有动作骨架) {
            添加动作(
                枚举_学习动作类型::候选动作生成,
                枚举_学习需求类型::方法可用需求,
                99,
                true,
                "当前方法缺动作骨架，需要先补动作");
        }
        if (结果数量 <= 0) {
            添加动作(
                枚举_学习动作类型::结果预测,
                枚举_学习需求类型::方法可用需求,
                97,
                true,
                "当前方法缺结果侧，需要先补结果迁移");
        }
        if (有动作骨架 && 条件数量 > 0 && 结果数量 > 0 && !已具备可用骨架) {
            添加动作(
                枚举_学习动作类型::沙箱验证,
                枚举_学习需求类型::方法可用需求,
                95,
                true,
                "当前方法已有基本结构，需要验证是否达到可用");
        }
        if (已具备可用骨架 && !决策.允许正式资产提交) {
            添加动作(
                枚举_学习动作类型::边界标记,
                枚举_学习需求类型::方法可用需求,
                82,
                false,
                "当前方法接近可用，但还需补适用边界");
            添加动作(
                枚举_学习动作类型::可用层进入判断,
                枚举_学习需求类型::方法可用需求,
                84,
                false,
                "当前方法已有基本结构，需要判断是否进入可用层");
        }
        if (决策.允许正式资产提交) {
            添加动作(
                枚举_学习动作类型::优选层进入判断,
                枚举_学习需求类型::方法优选需求,
                76,
                false,
                "当前方法已允许正式提交，可进入优选层判断");
        }
        if (输出.empty()) {
            添加动作(
                枚举_学习动作类型::沙箱验证,
                枚举_学习需求类型::方法可用需求,
                60,
                false,
                "默认先验证当前方法是否已达到可用");
        }
        return 输出;
    }

    std::vector<结构_学习原函数需求项> 私有_构建原函数需求集合(
        const std::vector<结构_学习动作描述>& 学习动作集合,
        const 结构_学习承接决策& 决策,
        const 结构_任务管理结果& 结果) noexcept
    {
        std::vector<结构_学习原函数需求项> 输出{};
        auto 添加需求 = [&](枚举_学习原函数类型 类型,
                           枚举_学习需求类型 来源需求类型,
                           枚举_学习动作类型 来源动作类型,
                           bool 必须满足,
                           bool 已满足,
                           const std::string& 原因) {
            for (const auto& 已有 : 输出) {
                if (已有.类型 == 类型 && 已有.来源动作类型 == 来源动作类型) {
                    return;
                }
            }
            结构_学习原函数需求项 需求{};
            需求.类型 = 类型;
            需求.来源需求类型 = 来源需求类型;
            需求.来源动作类型 = 来源动作类型;
            需求.必须满足 = 必须满足;
            需求.已满足 = 已满足;
            std::ostringstream 摘要;
            摘要 << 私有_学习原函数类型文本(类型)
                << " | 来源需求=" << 私有_学习需求类型文本(来源需求类型)
                << " | 来源动作=" << 私有_学习动作类型文本(来源动作类型)
                << " | 必须=" << (必须满足 ? "是" : "否")
                << " | 已满足=" << (已满足 ? "是" : "否")
                << " | 原因=" << 原因;
            需求.摘要 = 摘要.str();
            输出.push_back(std::move(需求));
        };

        for (const auto& 学习动作 : 学习动作集合) {
            const bool 当前方法已可用骨架 = 私有_学习方法已具备可用骨架(私有_P0_当前方法(结果));
            switch (学习动作.类型) {
            case 枚举_学习动作类型::样本收集:
                添加需求(
                    枚举_学习原函数类型::读取特征,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    !决策.来源主观察特征.empty(),
                    "先读取当前待学习方法与主观察上下文");
                添加需求(
                    枚举_学习原函数类型::收集样本,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    结果.当前需求位特征.最近抽象因果 != nullptr
                        || 结果.当前治理二次特征证据.当前可回接最近结果,
                    "补齐当前方法缺动作时需要的基础证据");
                break;
            case 枚举_学习动作类型::差异比较:
                添加需求(
                    枚举_学习原函数类型::比较样本,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    false,
                    "比较冲突样本的关键差异");
                添加需求(
                    枚举_学习原函数类型::提取差异点,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    false,
                    "提取导致失败或冲突的差异点");
                break;
            case 枚举_学习动作类型::共同条件提取:
                添加需求(
                    枚举_学习原函数类型::提取共同点,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    私有_学习方法条件数量(私有_P0_当前方法(结果)) > 0,
                    "从当前证据中提取可复用条件");
                添加需求(
                    枚举_学习原函数类型::生成候选条件,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    私有_学习方法条件数量(私有_P0_当前方法(结果)) > 0,
                    "补齐方法头的条件节点");
                break;
            case 枚举_学习动作类型::候选动作生成:
                添加需求(
                    枚举_学习原函数类型::读取特征,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    !决策.来源主观察特征.empty(),
                    "先读取当前方法目标结果与主观察");
                添加需求(
                    枚举_学习原函数类型::生成候选动作,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    私有_学习方法有动作骨架(私有_P0_当前方法(结果)),
                    "补齐当前方法头的动作骨架");
                break;
            case 枚举_学习动作类型::结果预测:
                添加需求(
                    枚举_学习原函数类型::生成候选结果,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    私有_学习方法结果数量(私有_P0_当前方法(结果)) > 0,
                    "补齐当前方法头的结果侧");
                break;
            case 枚举_学习动作类型::沙箱验证:
                添加需求(
                    枚举_学习原函数类型::执行沙箱验证,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    当前方法已可用骨架,
                    "验证当前方法头是否已经可用");
                添加需求(
                    枚举_学习原函数类型::计算二次特征,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    当前方法已可用骨架,
                    "把验证结果压成方法可用判据");
                break;
            case 枚举_学习动作类型::风险评估:
                添加需求(
                    枚举_学习原函数类型::计算二次特征,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    true,
                    false,
                    "重新计算风险相关二次特征");
                break;
            case 枚举_学习动作类型::边界标记:
                添加需求(
                    枚举_学习原函数类型::标记边界,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    false,
                    false,
                    "补齐当前方法的适用边界");
                break;
            case 枚举_学习动作类型::可用层进入判断:
            case 枚举_学习动作类型::优选层进入判断:
                添加需求(
                    枚举_学习原函数类型::注册方法,
                    学习动作.来源需求类型,
                    学习动作.类型,
                    false,
                    决策.允许正式资产提交,
                    "把当前可用方法写回正式方法节点");
                break;
            default:
                break;
            }
        }
        return 输出;
    }

    结构_候选学习程序摘要 私有_构建候选学习程序摘要(
        const 结构_学习承接决策& 决策,
        const std::vector<结构_学习原函数需求项>& 原函数需求集合,
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_候选学习程序摘要 输出{};
        输出.当前执行方法节点 = 私有_P0_当前方法(结果);
        输出.候选学习方法节点 = 输出.当前执行方法节点;
        输出.已形成候选学习方法节点 = 输出.候选学习方法节点 != nullptr;
        输出.已允许转正 = 决策.允许正式资产提交;
        std::ostringstream 程序主键;
        程序主键 << (决策.来源根任务 ? 决策.来源根任务->获取主键() : "空根任务")
               << "|" << static_cast<int>(决策.学习目标.类型)
               << "|" << static_cast<int>(决策.来源方法需求位)
               << "|" << 私有_方法标题(输出.当前执行方法节点);
        输出.程序主键 = 程序主键.str();
        for (const auto& 项 : 原函数需求集合) {
            输出.原函数序列.push_back(项.类型);
        }
        const bool 需要补样本 =
            std::ranges::find(输出.原函数序列, 枚举_学习原函数类型::收集样本) != 输出.原函数序列.end();
        const bool 需要补动作 =
            std::ranges::find(输出.原函数序列, 枚举_学习原函数类型::生成候选动作) != 输出.原函数序列.end();
        const bool 需要补条件 =
            std::ranges::find(输出.原函数序列, 枚举_学习原函数类型::生成候选条件) != 输出.原函数序列.end();
        const bool 需要补结果 =
            std::ranges::find(输出.原函数序列, 枚举_学习原函数类型::生成候选结果) != 输出.原函数序列.end();
        const bool 需要验证 =
            std::ranges::find(输出.原函数序列, 枚举_学习原函数类型::执行沙箱验证) != 输出.原函数序列.end();
        const bool 有抽象因果 =
            结果.当前需求位特征.最近抽象因果 != nullptr
            || !结果.当前需求位特征.最近抽象因果摘要.empty();
        const bool 当前方法已可用骨架 =
            私有_学习方法已具备可用骨架(输出.当前执行方法节点);
        if (决策.允许正式资产提交
            && 当前方法已可用骨架) {
            输出.当前裁决 = 枚举_学习程序决策::转正;
        }
        else if (需要补样本 && !有抽象因果) {
            输出.当前裁决 = 枚举_学习程序决策::补充样本;
        }
        else if (需要验证) {
            输出.当前裁决 = 枚举_学习程序决策::试运行;
        }
        else if (需要补动作 || 需要补条件 || 需要补结果 || 需要补样本) {
            输出.当前裁决 = 枚举_学习程序决策::继续执行;
        }
        else if (结果.当前治理二次特征证据.当前需收束) {
            输出.当前裁决 = 枚举_学习程序决策::暂停;
        }
        else {
            输出.当前裁决 = 枚举_学习程序决策::继续执行;
        }
        std::ostringstream 摘要;
        摘要 << "程序键=" << 输出.程序主键
            << " | 当前方法=" << 私有_方法标题(输出.当前执行方法节点)
            << " | 当前裁决=" << 私有_学习程序决策文本(输出.当前裁决)
            << " | 原函数数=" << 输出.原函数序列.size()
            << " | 可改写=" << (输出.可动态改写 ? "是" : "否")
            << " | 候选方法节点=" << (输出.已形成候选学习方法节点 ? "已形成" : "未形成")
            << " | 允许转正=" << (输出.已允许转正 ? "是" : "否");
        输出.摘要 = 摘要.str();
        return 输出;
    }

    枚举_任务管理学习承接类型 私有_推导学习承接类型(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        (void)宿主任务;
        if (结果.当前需求位特征.当前学习承接类型 != 枚举_任务管理学习承接类型::未定义) {
            return 结果.当前需求位特征.当前学习承接类型;
        }
        if (结果.当前学习承接决策.学习类型 != 枚举_任务管理学习承接类型::未定义) {
            return 结果.当前学习承接决策.学习类型;
        }
        if (私有_P0_稳定等待态复盘学习(结果)) {
            return 枚举_任务管理学习承接类型::广泛学习;
        }
        return 枚举_任务管理学习承接类型::定向学习;
    }

    结构_学习承接决策 私有_形成学习承接决策(
        任务节点* 当前管理任务,
        任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        结构_学习承接决策 决策{};
        结构_高阶排序请求 高阶排序请求 = 结果.当前高阶排序请求;
        if (!私有_P0_高阶排序请求有效(高阶排序请求)) {
            高阶排序请求 = 结果.当前请求.当前高阶排序请求;
        }
        结构_高阶排序结果 高阶排序结果 = 结果.当前高阶排序结果;
        if (!私有_P0_高阶排序结果有效(高阶排序结果)) {
            高阶排序结果 = 结果.当前一步结果.当前高阶排序结果;
        }
        结构_高阶胜出绑定 高阶胜出绑定 = 结果.当前高阶胜出绑定;
        if (!私有_P0_高阶胜出绑定有效(高阶胜出绑定)) {
            高阶胜出绑定 = 结果.当前一步结果.当前高阶胜出绑定;
        }
        if (!私有_P0_高阶胜出绑定有效(高阶胜出绑定)) {
            高阶胜出绑定 = 结果.当前单步决策.当前高阶胜出绑定;
        }
        if (!私有_P0_高阶胜出绑定有效(高阶胜出绑定)
            && (私有_P0_高阶排序结果有效(高阶排序结果) || 私有_P0_高阶排序请求有效(高阶排序请求))) {
            (void)私有_P0_解析高阶胜出绑定(结果.当前请求, 高阶排序结果, 高阶胜出绑定);
        }
        决策.来源根任务 = 宿主任务 ? 私有_主体锚点任务(宿主任务) : nullptr;
        决策.来源父任务 = 宿主任务;
        决策.来源功能域 = 私有_P0_当前功能域(结果);
        决策.来源缺口类型 = 私有_P0_当前缺口类型(结果);
        决策.来源方法需求位 = 私有_P0_当前方法需求位(结果);
        决策.来源本能能力缺口类型 = 私有_P0_当前本能缺口(结果);
        决策.来源最小原语位图 =
            结果.当前本能能力缺口.所需原语位图 != 0
            ? 结果.当前本能能力缺口.所需原语位图
            : 结果.来源最小原语位图;
        决策.来源正式原语摘要 =
            !结果.当前本能能力缺口.原语账本摘要.empty()
            ? 结果.当前本能能力缺口.原语账本摘要
            : std::string{};
        决策.来源主观察特征 =
            !结果.当前本能能力缺口.主观察特征键.empty()
            ? 结果.当前本能能力缺口.主观察特征键
            : 结果.来源主观察特征;
        决策.方法服务目标 = 决策.来源方法需求位;
        决策.方法服务范围 =
            决策.方法服务目标 == 枚举_任务管理方法需求位::未定义
            ? "围绕学习对象方法完善"
            : ("围绕" + std::string(私有_方法需求位文本(决策.方法服务目标)) + "完善学习对象方法");
        决策.回流目标方法位 = 决策.来源方法需求位;
        决策.回流目标摘要 =
            !结果.当前写回结果.主体回并摘要.empty()
            ? 结果.当前写回结果.主体回并摘要
            : 结果.当前需求位特征.需求摘要;
        决策.补原语或治理本能 =
            私有_本能缺口属于原语或治理补齐(决策.来源本能能力缺口类型);
        决策.来源高阶排序需求轴键 = !高阶排序请求.需求轴键.empty()
            ? 高阶排序请求.需求轴键
            : 高阶排序结果.需求轴键;
        决策.来源高阶排序候选主键列表 = !高阶排序请求.候选主键列表.empty()
            ? 高阶排序请求.候选主键列表
            : 高阶排序结果.候选主键列表;
        决策.来源高阶排序比较基准 = !高阶排序请求.比较基准摘要.empty()
            ? 高阶排序请求.比较基准摘要
            : 高阶排序请求.比较特征键;
        决策.来源高阶排序胜出候选主键 = 高阶排序结果.胜出候选主键;
        决策.来源高阶排序胜出方法主键 = 高阶胜出绑定.胜出方法主键;
        决策.来源高阶排序已显式绑定 = 高阶胜出绑定.已绑定执行方法;
        决策.来源高阶排序绑定摘要.clear();
        决策.来源高阶排序已验证 =
            结果.当前原语判定汇总.结果符合 || 结果.当前原语判定汇总.因果稳定;
        if (私有_P0_高阶排序结果有效(高阶排序结果) || 私有_P0_高阶排序请求有效(高阶排序请求)) {
            std::ostringstream 高阶验证摘要;
            高阶验证摘要 << "高阶请求=" << 私有_P0_高阶排序请求摘要(高阶排序请求)
                << " | 高阶排序=" << 私有_P0_高阶排序结果摘要(高阶排序结果)
                << " | 高阶绑定=" << 私有_P0_高阶胜出绑定摘要(高阶胜出绑定)
                << " | 后验验证=" << (决策.来源高阶排序已验证 ? "已验证" : "待验证");
            决策.来源高阶排序验证摘要 = 高阶验证摘要.str();
        }
        决策.是否影子验证 = !结果.允许正式资产提交;
        决策.允许正式资产提交 = 结果.允许正式资产提交;
        决策.默认消费策略 =
            !决策.允许正式资产提交
            ? 枚举_任务管理学习反馈消费策略::继续学习
            : 枚举_任务管理学习反馈消费策略::使用已完善方法;
        决策.学习目标 = 私有_构建学习目标描述(决策, 结果);
        决策.学习需求集合 = 私有_构建学习需求集合(决策, 结果);
        决策.学习动作集合 = 私有_构建学习动作集合(决策, 结果);
        决策.原函数需求集合 = 私有_构建原函数需求集合(决策.学习动作集合, 决策, 结果);
        决策.候选学习程序 = 私有_构建候选学习程序摘要(决策, 决策.原函数需求集合, 结果);

        if (!当前管理任务 || !宿主任务) {
            决策.决策摘要.clear();
            return 决策;
        }

        决策.学习类型 = 私有_推导学习承接类型(宿主任务, 结果);
        决策.是否触发学习 = 私有_应创建学习子任务(宿主任务, 结果);
        if (决策.允许正式资产提交
            && 决策.学习类型 == 枚举_任务管理学习承接类型::广泛学习) {
            决策.默认消费策略 = 枚举_任务管理学习反馈消费策略::挂起观察;
        }

        决策.决策摘要.clear();
        return 决策;
    }

    void 私有_同步学习承接决策(
        结构_任务管理结果& 结果,
        const 结构_学习承接决策& 决策) noexcept
    {
        结果.当前学习承接决策 = 决策;
        结果.当前需求位特征.已触发学习承接 = 决策.是否触发学习;
        结果.当前需求位特征.当前学习承接类型 = 决策.学习类型;
        结果.当前需求位特征.学习承接摘要.clear();
        结果.当前一步结果.已触发学习承接 = 决策.是否触发学习;
        结果.当前一步结果.当前学习承接类型 = 决策.学习类型;
        结果.当前一步结果.当前学习为本能补齐 = 决策.补原语或治理本能;
        结果.当前一步结果.学习承接摘要.clear();
        私有_P0_刷新步骤与学习结构字段(结果);
    }

    任务节点* 私有_创建或复用学习子任务头(
        自我类& 自我对象,
        任务节点* 管理任务,
        任务节点* 宿主任务,
        const 结构_学习承接决策& 学习决策,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!私有_应创建学习子任务(宿主任务, 结果)) {
            return nullptr;
        }

        auto* 学习树根 = 私有_确保任务管理学习树根(
            自我对象,
            管理任务,
            结果,
            now,
            调用点 + "/学习树根");
        auto* 学习分支 = 学习决策.学习类型 == 枚举_任务管理学习承接类型::广泛学习
            ? 私有_确保广泛学习分支(
                自我对象,
                学习树根,
                结果,
                now,
                调用点 + "/广泛学习分支")
            : 私有_确保定向学习分支(
                自我对象,
                学习树根,
                结果,
                now,
                调用点 + "/定向学习分支");
        if (!学习分支) {
            return nullptr;
        }

        auto* 现有子任务 = 私有_查找直接子节点_按谓词<任务节点>(学习分支, [&](const 任务节点* 节点) noexcept {
            return 节点
                && 节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                && 私有_词性相同(节点->主信息.类型, 私有_学习子任务类型词(学习决策.学习类型))
                && 私有_词性相同(
                    节点->主信息.名称,
                    私有_学习子任务名称词(学习决策.学习类型, 宿主任务, 学习决策, 结果))
                && !私有_任务状态已终结(节点->主信息.状态);
        });
        if (现有子任务) {
            if (!结果.当前子任务节点) {
                结果.当前子任务节点 = 现有子任务;
            }
            结果.已复用学习子任务 = true;
            return 现有子任务;
        }

        auto* 场景 = 宿主任务 && 宿主任务->主信息.场景.指针
            ? 宿主任务->主信息.场景.指针
            : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());

        任务主信息类 主信息{};
        主信息.名称 = 私有_学习子任务名称词(学习决策.学习类型, 宿主任务, 学习决策, 结果);
        主信息.类型 = 私有_学习子任务类型词(学习决策.学习类型);
        主信息.节点种类 = 枚举_任务节点种类::头结点;
        主信息.状态 = 枚举_任务状态::挂起;
        主信息.启动时间 = now;
        主信息.统计.记录观测(now);
        主信息.基准优先级 = 宿主任务 ? 宿主任务->主信息.基准优先级 : 0;
        主信息.局部优先级偏移 = 宿主任务 ? 宿主任务->主信息.局部优先级偏移 : 0;
        主信息.调度优先级 = 宿主任务 ? 宿主任务->主信息.调度优先级 : 0;
        私有_写入引用(主信息.来源需求, 结果.上下文.当前主需求);
        auto* 来源父任务 = 学习决策.学习类型 == 枚举_任务管理学习承接类型::广泛学习
            ? 管理任务
            : 宿主任务;
        私有_写入引用(主信息.父任务, 来源父任务);
        私有_写入引用(主信息.场景, 场景);

        auto* 子任务头 = 私有_独立树添加子节点<任务类>(学习分支, 主信息);
        if (!子任务头) {
            return nullptr;
        }

        if (!结果.当前子任务节点) {
            结果.当前子任务节点 = 子任务头;
        }
        结果.已创建学习子任务 = true;
        return 子任务头;
    }

    I64 私有_统计待学习方法数量(const 任务节点* 管理任务) noexcept
    {
        const auto* 学习树根 = 私有_查找任务管理学习树根(管理任务);
        if (!学习树根) {
            return 0;
        }

        I64 总数 = 0;
        私有_枚举直接子节点<任务节点>(学习树根, [&](const 任务节点* 分支节点) {
            if (!分支节点) {
                return;
            }
            私有_枚举直接子节点<任务节点>(分支节点, [&](const 任务节点* 学习节点) {
                if (学习节点
                    && 学习节点->主信息.节点种类 == 枚举_任务节点种类::头结点
                    && 私有_学习子任务类型匹配(学习节点->主信息.类型)
                    && !私有_任务状态已终结(学习节点->主信息.状态)) {
                    ++总数;
                }
            });
        });
        return 总数;
    }

    bool 私有_学习方法有条件侧(const 方法节点* 方法头) noexcept
    {
        if (!方法头) {
            return false;
        }
        if (方法头->主信息.条件场景.指针) {
            return true;
        }

        auto* 首节点 = static_cast<const 方法节点*>(方法头->子);
        if (!首节点) {
            return false;
        }

        auto* 当前 = 首节点;
        do {
            if (当前 && 当前->主信息.节点种类 == 枚举_方法节点种类::方法条件节点) {
                return true;
            }
            当前 = static_cast<const 方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return false;
    }

    bool 私有_学习方法有动作骨架(const 方法节点* 方法头) noexcept
    {
        return 方法头
            && 方法头->主信息.动作句柄.类型 != 枚举_动作句柄类型::未设置;
    }

    bool 私有_学习方法仅需求位(const 方法节点* 方法头) noexcept
    {
        return 方法头
            && !(私有_学习方法有动作骨架(方法头) && 私有_学习方法有条件侧(方法头));
    }

    bool 私有_学习方法可直接进入学习(const 方法节点* 方法头) noexcept
    {
        return 方法头
            && 私有_学习方法有动作骨架(方法头)
            && 私有_学习方法有条件侧(方法头);
    }

    I64 私有_学习方法条件数量(const 方法节点* 方法头) noexcept
    {
        if (!方法头) {
            return 0;
        }
        if (!方法头->主信息.条件判定索引.empty()) {
            return static_cast<I64>(方法头->主信息.条件判定索引.size());
        }
        return 私有_学习方法有条件侧(方法头) ? 1 : 0;
    }

    I64 私有_学习方法结果数量(const 方法节点* 方法头) noexcept
    {
        if (!方法头) {
            return 0;
        }
        if (!方法头->主信息.结果摘要索引.empty()) {
            return static_cast<I64>(方法头->主信息.结果摘要索引.size());
        }
        return 方法头->主信息.主结果特征类型 ? 1 : 0;
    }

    bool 私有_学习方法结果已具备(const 方法节点* 方法头) noexcept
    {
        return 私有_学习方法结果数量(方法头) > 0;
    }

    bool 私有_学习方法已具备可用骨架(const 方法节点* 方法头) noexcept
    {
        return 方法头
            && 私有_学习方法结果已具备(方法头)
            && 私有_学习方法有动作骨架(方法头)
            && 私有_学习方法条件数量(方法头) > 0;
    }

    struct 结构_学习任务虚拟存在镜像 {
        bool 存在读回 = false;
        方法节点* 当前待学习方法头 = nullptr;
        枚举_学习目标类型 当前学习目标类型 = 枚举_学习目标类型::未定义;
        I64 当前学习需求数量 = 0;
        I64 当前学习动作数量 = 0;
        枚举_学习程序决策 当前学习程序裁决 = 枚举_学习程序决策::未定义;
        bool 当前学习方法有动作骨架 = false;
        I64 当前学习方法条件数量 = 0;
        I64 当前学习方法结果数量 = 0;
        bool 当前学习方法可用 = false;
        bool 当前已切换到兜底可执行方法 = false;
    };

    结构_学习任务虚拟存在镜像 私有_读取学习任务虚拟存在镜像(
        const 自我类& 自我对象,
        const 任务节点* 学习子任务,
        const std::string& 调用点) noexcept
    {
        结构_学习任务虚拟存在镜像 输出{};
        if (!学习子任务) {
            return 输出;
        }

        输出.当前待学习方法头 = 私有_读取宿主任务治理方法节点指针特征(
            自我对象,
            const_cast<任务节点*>(学习子任务),
            私有_管理特征_当前待学习方法头(),
            调用点 + "/当前待学习方法头");

        输出.当前学习目标类型 = 私有_读取宿主任务治理I64枚举特征<枚举_学习目标类型>(
            自我对象,
            const_cast<任务节点*>(学习子任务),
            私有_管理特征_当前学习目标类型(),
            调用点 + "/当前学习目标类型");

        输出.当前学习程序裁决 = 私有_读取宿主任务治理I64枚举特征<枚举_学习程序决策>(
            自我对象,
            const_cast<任务节点*>(学习子任务),
            私有_管理特征_当前学习程序裁决(),
            调用点 + "/当前学习程序裁决");

        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            const_cast<任务节点*>(学习子任务),
            私有_管理特征_当前学习需求数量(),
            当前值,
            调用点 + "/当前学习需求数量")) {
            输出.当前学习需求数量 = 当前值;
            输出.存在读回 = true;
        }
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            const_cast<任务节点*>(学习子任务),
            私有_管理特征_当前学习动作数量(),
            当前值,
            调用点 + "/当前学习动作数量")) {
            输出.当前学习动作数量 = 当前值;
            输出.存在读回 = true;
        }
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            const_cast<任务节点*>(学习子任务),
            私有_管理特征_当前学习方法有动作骨架(),
            当前值,
            调用点 + "/当前学习方法有动作骨架")) {
            输出.当前学习方法有动作骨架 = 当前值 != 0;
            输出.存在读回 = true;
        }
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            const_cast<任务节点*>(学习子任务),
            私有_管理特征_当前学习方法条件数量(),
            当前值,
            调用点 + "/当前学习方法条件数量")) {
            输出.当前学习方法条件数量 = 当前值;
            输出.存在读回 = true;
        }
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            const_cast<任务节点*>(学习子任务),
            私有_管理特征_当前学习方法结果数量(),
            当前值,
            调用点 + "/当前学习方法结果数量")) {
            输出.当前学习方法结果数量 = 当前值;
            输出.存在读回 = true;
        }
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            const_cast<任务节点*>(学习子任务),
            私有_管理特征_当前学习方法可用(),
            当前值,
            调用点 + "/当前学习方法可用")) {
            输出.当前学习方法可用 = 当前值 != 0;
            输出.存在读回 = true;
        }
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            const_cast<任务节点*>(学习子任务),
            私有_管理特征_当前已切换到兜底可执行方法(),
            当前值,
            调用点 + "/当前已切换到兜底可执行方法")) {
            输出.当前已切换到兜底可执行方法 = 当前值 != 0;
            输出.存在读回 = true;
        }

        if (输出.当前待学习方法头
            || 输出.当前学习目标类型 != 枚举_学习目标类型::未定义
            || 输出.当前学习程序裁决 != 枚举_学习程序决策::未定义) {
            输出.存在读回 = true;
        }
        return 输出;
    }

    bool 私有_同步学习任务虚拟存在特征(
        自我类& 自我对象,
        任务节点* 学习子任务,
        方法节点* 待学习方法头,
        方法节点* 学习兜底方法,
        const 结构_学习承接决策& 学习决策,
        const 结构_任务管理结果& 结果,
        bool 当前方法已可用,
        bool 已切换到兜底可执行方法,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!学习子任务) {
            return false;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);
        const std::string 前缀 = 调用点 + "/学习任务虚拟存在";
        const std::string 待学习方法标题 = 私有_方法标题(待学习方法头);
        const std::string 兜底方法标题 = 私有_方法标题(学习兜底方法);

        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前待学习方法头(),
            待学习方法头,
            待学习方法标题,
            "任务管理_当前待学习方法头",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前待学习方法头",
            const_cast<结构_任务管理结果&>(结果),
            nullptr,
            枚举_动作事件相位::未定义);

        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前学习兜底方法(),
            学习兜底方法,
            兜底方法标题,
            "任务管理_当前学习兜底方法",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前学习兜底方法",
            const_cast<结构_任务管理结果&>(结果),
            nullptr,
            枚举_动作事件相位::未定义);

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前学习目标类型(),
            static_cast<I64>(学习决策.学习目标.类型),
            "任务管理_当前学习目标类型",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前学习目标类型",
            const_cast<结构_任务管理结果&>(结果));

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前学习需求数量(),
            static_cast<I64>(学习决策.学习需求集合.size()),
            "任务管理_当前学习需求数量",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前学习需求数量",
            const_cast<结构_任务管理结果&>(结果));

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前学习动作数量(),
            static_cast<I64>(学习决策.学习动作集合.size()),
            "任务管理_当前学习动作数量",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前学习动作数量",
            const_cast<结构_任务管理结果&>(结果));

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前学习程序裁决(),
            static_cast<I64>(学习决策.候选学习程序.当前裁决),
            "任务管理_当前学习程序裁决",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前学习程序裁决",
            const_cast<结构_任务管理结果&>(结果));

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前学习方法有动作骨架(),
            私有_学习方法有动作骨架(待学习方法头) ? 1 : 0,
            "任务管理_当前学习方法有动作骨架",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前学习方法有动作骨架",
            const_cast<结构_任务管理结果&>(结果));

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前学习方法条件数量(),
            私有_学习方法条件数量(待学习方法头),
            "任务管理_当前学习方法条件数量",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前学习方法条件数量",
            const_cast<结构_任务管理结果&>(结果));

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前学习方法结果数量(),
            私有_学习方法结果数量(待学习方法头),
            "任务管理_当前学习方法结果数量",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前学习方法结果数量",
            const_cast<结构_任务管理结果&>(结果));

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前学习方法可用(),
            当前方法已可用 ? 1 : 0,
            "任务管理_当前学习方法可用",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前学习方法可用",
            const_cast<结构_任务管理结果&>(结果));

        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前已切换到兜底可执行方法(),
            已切换到兜底可执行方法 ? 1 : 0,
            "任务管理_当前已切换到兜底可执行方法",
            当前方法,
            学习子任务,
            now,
            前缀 + "/当前已切换到兜底可执行方法",
            const_cast<结构_任务管理结果&>(结果));

        return true;
    }

    状态节点类* 私有_学习方法条件目标状态(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        if (结果.宿主目标状态) {
            return 结果.宿主目标状态;
        }
        if (结果.最新目标状态) {
            return 结果.最新目标状态;
        }
        return 宿主任务 ? 宿主任务->主信息.目标状态.指针 : nullptr;
    }

    状态节点类* 私有_学习方法结果目标状态(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        if (结果.宿主结果状态) {
            return 结果.宿主结果状态;
        }
        if (结果.最新状态) {
            return 结果.最新状态;
        }
        return 宿主任务 ? 宿主任务->主信息.结果状态信息.指针 : nullptr;
    }

    const 词性节点类* 私有_状态节点特征类型(const 状态节点类* 状态) noexcept
    {
        if (!状态) {
            return nullptr;
        }

        const auto* 状态主信息 = 世界树.基础信息().取主信息<状态节点主信息类>(
            const_cast<状态节点类*>(状态));
        if (!状态主信息 || !状态主信息->状态特征.指针) {
            return nullptr;
        }

        const auto* 特征主信息 = 世界树.基础信息().取主信息<特征节点主信息类>(
            状态主信息->状态特征.指针);
        return 特征主信息 ? 特征主信息->类型 : nullptr;
    }

    const 词性节点类* 私有_学习方法主结果特征类型(
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果) noexcept
    {
        if (const auto* 结果状态 = 私有_学习方法结果目标状态(宿主任务, 结果)) {
            if (const auto* 特征类型 = 私有_状态节点特征类型(结果状态)) {
                return 特征类型;
            }
        }

        return 私有_状态节点特征类型(私有_学习方法条件目标状态(宿主任务, 结果));
    }

    bool 私有_动作句柄为尝试学习(const 结构体_动作句柄& 动作句柄) noexcept
    {
        return 动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && 动作句柄.本能ID == static_cast<I64>(枚举_本能方法ID::学习_尝试学习);
    }

    方法节点* 私有_查找学习方法头候选(
        const 自我类& 自我对象,
        const 任务节点* 学习子任务,
        const 任务节点* 宿主任务,
        const 结构_任务管理结果& 结果,
        bool 作为尝试学习选项) noexcept
    {
        auto* 自我存在 = const_cast<存在节点类*>(自我对象.获取自我存在());
        if (!自我存在 || !学习子任务) {
            return nullptr;
        }

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        auto* 动作名 = 私有_学习方法动作名词(宿主任务, 结果);
        if (!方法根节点 || !动作名) {
            return nullptr;
        }

        auto* 首节点 = static_cast<方法节点*>(方法根节点->子);
        if (!首节点) {
            return nullptr;
        }

        auto* 当前 = 首节点;
        do {
            if (当前
                && 当前->主信息.节点种类 == 枚举_方法节点种类::方法首节点
                && 私有_词性相同(当前->主信息.动作名, 动作名)
                && 私有_动作句柄为尝试学习(当前->主信息.动作句柄) == 作为尝试学习选项
                && reinterpret_cast<const void*>(当前->主信息.来源任务.指针)
                    == reinterpret_cast<const void*>(学习子任务)) {
                return 当前;
            }

            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    void 私有_写入尝试学习动作句柄(结构体_动作句柄& 动作句柄) noexcept
    {
        动作句柄 = {};
        动作句柄.类型 = 枚举_动作句柄类型::本能函数ID;
        动作句柄.本能ID = static_cast<I64>(枚举_本能方法ID::学习_尝试学习);
    }

    bool 私有_P2_写场景治理二次特征_布尔(
        场景节点类* 场景,
        const std::string& 概念名称,
        const bool 是否满足,
        const std::string& 原因摘要,
        const I64 置信度,
        二次特征节点类** 输出节点 = nullptr) noexcept
    {
        if (!场景 || 概念名称.empty()) {
            return false;
        }

        结构_场景二次特征写入请求 请求{};
        请求.概念名称 = 概念名称;
        请求.概念词性 = "名词";
        请求.同步名称和类型 = true;
        请求.种类 = 枚举_二次特征种类::状态比较;
        请求.域 = 枚举_二次特征域::存在;
        请求.粒度 = 枚举_二次特征粒度::当前场景;
        请求.基准类型 = 枚举_二次特征基准类型::无;
        请求.时间归一方式 = 枚举_时间归一方式::无;
        请求.值形态 = 枚举_二次特征值形态::布尔;
        请求.离散编码 = 是否满足 ? 1 : 0;
        请求.离散语义键 = 原因摘要.empty() ? (是否满足 ? "满足" : "未满足") : 原因摘要;
        请求.标量值 = 是否满足 ? 1 : 0;
        请求.是否满足 = 是否满足;
        请求.置信度 = 置信度;
        return 世界树.二次特征生成().写入场景二次特征(场景, 请求, 输出节点);
    }

    std::string 私有_P2_抽象因果条件签名(
        const 特征节点主信息类* 特征主信息,
        const 特征节点类* 特征节点) noexcept
    {
        const auto 类型键 = 特征主信息 && 特征主信息->类型 ? 特征主信息->类型->获取主键() : std::string{};
        const auto 名称键 = 特征主信息 && 特征主信息->名称 ? 特征主信息->名称->获取主键() : std::string{};
        const auto 节点键 = 特征节点 ? 特征节点->获取主键() : std::string{};
        return "抽象因果条件|"
            + (类型键.empty() ? "_" : 类型键)
            + "|"
            + (名称键.empty() ? "_" : 名称键)
            + "|"
            + (节点键.empty() ? "_" : 节点键);
    }

    二次特征节点类* 私有_P2_桥接条件模板到方法条件特征(
        场景节点类* 条件场景,
        特征节点类* 条件模板节点,
        const I64 置信度,
        const std::string& 调用点) noexcept
    {
        (void)调用点;
        if (!条件场景 || !条件模板节点) {
            return nullptr;
        }

        const auto* 特征主信息 = 世界树.特征().取特征主信息(条件模板节点);
        if (!特征主信息) {
            return nullptr;
        }

        const auto 概念名称 =
            私有_安全词(特征主信息->名称 ? 特征主信息->名称 : 特征主信息->类型);
        if (概念名称.empty()) {
            return nullptr;
        }

        结构_场景二次特征写入请求 请求{};
        请求.度量签名_链键 = 私有_P2_抽象因果条件签名(特征主信息, 条件模板节点);
        请求.概念名称 = 概念名称;
        请求.概念词性 = "状态词";
        请求.同步名称和类型 = true;
        请求.形态 = 二次特征主信息类::枚举_形态::条件;
        请求.种类 = 枚举_二次特征种类::观察关系;
        请求.域 = 枚举_二次特征域::存在;
        请求.粒度 = 枚举_二次特征粒度::当前场景;
        请求.基准类型 = 枚举_二次特征基准类型::无;
        请求.时间归一方式 = 枚举_时间归一方式::无;
        请求.置信度 = 置信度;
        请求.是否满足 = true;

        if (特征主信息->区间.has_value() && 特征主信息->区间->有效()) {
            请求.值形态 = 枚举_二次特征值形态::区间;
            请求.标量值 = 特征主信息->区间->点值();
        } else if (const auto* 标量 = std::get_if<I64>(&特征主信息->当前值)) {
            请求.值形态 = 枚举_二次特征值形态::区间;
            请求.标量值 = *标量;
        } else if (const auto* 指针值 = std::get_if<指针句柄>(&特征主信息->当前值)) {
            请求.值形态 = 枚举_二次特征值形态::枚举;
            请求.离散编码 = 1;
            请求.离散语义键 = 指针值->有效() ? "指针值" : "值对";
        } else {
            请求.值形态 = 枚举_二次特征值形态::布尔;
            请求.离散编码 = 1;
            请求.标量值 = 1;
        }

        二次特征节点类* 节点 = nullptr;
        if (!世界树.二次特征生成().写入场景二次特征(条件场景, 请求, &节点) || !节点) {
            return nullptr;
        }

        if (auto* 主信息 = 世界树.二次特征().取二次特征主信息(节点)) {
            if (特征主信息->区间.has_value() && 特征主信息->区间->有效()) {
                主信息->标量区间 = 特征主信息->区间;
                if (特征主信息->区间->是否点()) {
                    主信息->标量值 = 特征主信息->区间->点值();
                }
            } else if (const auto* 标量 = std::get_if<I64>(&特征主信息->当前值)) {
                主信息->标量区间 = I64区间::点(*标量);
                主信息->标量值 = *标量;
            }
        }

        return 节点;
    }

    void 私有_P2_吸收抽象因果到方法结构(
        任务节点* 宿主任务,
        方法节点* 方法头,
        因果模板节点类* 抽象因果,
        std::vector<可解析引用<二次特征节点类>>& 条件索引,
        std::vector<可解析引用<二次特征节点类>>& 结果索引,
        const I64 置信度,
        结构_任务管理结果& 结果,
        const std::string& 调用点) noexcept
    {
        if (!方法头 || !抽象因果) {
            return;
        }

        auto* 因果主信息 = 世界树.因果().取模板主信息(抽象因果);
        if (!因果主信息) {
            return;
        }

        auto& 方法主信息 = 方法头->主信息;
        if (!方法主信息.动作名 && !因果主信息->动作名称.empty()) {
            方法主信息.动作名 = 语素集.添加词性词(因果主信息->动作名称, "动词");
        }
        if (方法主信息.动作句柄.类型 == 枚举_动作句柄类型::未设置) {
            if (const auto* 动作方法 = reinterpret_cast<const 方法节点*>(因果主信息->动作模板.指针)) {
                方法主信息.动作句柄 = 动作方法->主信息.动作句柄;
                if (!方法主信息.动作名) {
                    方法主信息.动作名 = 动作方法->主信息.动作名;
                }
            }
        }

        auto* 条件场景 = 方法类::取或创建_方法条件场景(
            方法头,
            nullptr,
            结构体_时间戳::当前_微秒(),
            调用点 + "/条件场景");
        auto* 结果场景 = 方法类::取或创建_方法结果场景(
            方法头,
            nullptr,
            结构体_时间戳::当前_微秒(),
            调用点 + "/结果场景");

        if (方法主信息.条件主键.empty()) {
            方法主信息.条件主键 = 抽象因果->获取主键();
        }
        if (方法主信息.条件场景模板主键.empty() && 条件场景) {
            方法主信息.条件场景模板主键 = 条件场景->获取主键();
        }
        if (方法主信息.结果主键.empty() && 因果主信息->主状态迁移.有效()) {
            方法主信息.结果主键 = !因果主信息->主状态迁移.主键.empty()
                ? 因果主信息->主状态迁移.主键
                : (因果主信息->主状态迁移.指针 ? 因果主信息->主状态迁移.指针->获取主键() : std::string{});
        }

        if (条件场景) {
            for (const auto& 条件模板 : 因果主信息->条件模板) {
                auto* 条件模板节点 = 条件模板.指针;
                if (!条件模板节点 && !条件模板.主键.empty()) {
                    条件模板节点 = static_cast<特征节点类*>(世界树.基础信息().查找主键(条件模板.主键));
                }
                if (auto* 条件特征 = 私有_P2_桥接条件模板到方法条件特征(
                    条件场景,
                    条件模板节点,
                    置信度,
                    调用点 + "/抽象因果条件")) {
                    私有_P2_追加二次特征引用(条件索引, 条件特征);
                }
            }
        }

        for (const auto& 状态迁移模板 : 因果主信息->状态迁移模板) {
            auto* 迁移节点 = 状态迁移模板.指针;
            if (!迁移节点 && !状态迁移模板.主键.empty()) {
                迁移节点 = static_cast<二次特征节点类*>(世界树.基础信息().查找主键(状态迁移模板.主键));
            }
            私有_P2_追加二次特征引用(结果索引, 迁移节点);
            if (const auto* 迁移主信息 = 迁移节点 ? 世界树.二次特征().取二次特征主信息(迁移节点) : nullptr) {
                if (auto* 初始状态 = 迁移主信息->左对象.指针
                    ? reinterpret_cast<状态节点类*>(迁移主信息->左对象.指针)
                    : nullptr) {
                    私有_追加状态引用(方法主信息.结果初始状态, 初始状态);
                }
            }
        }

        私有_P2_吸收二次特征引用列表(方法主信息.条件判定索引, 条件索引);
        私有_P2_吸收二次特征引用列表(方法主信息.结果摘要索引, 结果索引);
        世界树.二次特征生成().确保二次特征列表具备默认值(方法主信息.条件判定索引);
        世界树.二次特征生成().确保二次特征列表具备默认值(方法主信息.结果摘要索引);

        if (条件场景) {
            (void)方法类::创建条件节点(
                方法头,
                条件场景,
                方法主信息.条件判定索引,
                方法主信息.条件主键,
                0,
                0,
                调用点 + "/抽象因果条件节点");
        }
        if (结果场景) {
            (void)方法类::创建结果节点(
                方法头,
                结果场景,
                方法主信息.结果摘要索引,
                调用点 + "/抽象因果结果节点");
        }

        私有_追加最近反馈片段(
            结果,
            "抽象因果补齐方法骨架="
            + 私有_方法标题(方法头)
            + " | 抽象因果="
            + 抽象因果->获取主键()
            + " | 调用点="
            + 调用点);
    }

    void 私有_P2_补齐学习方法证据索引(
        const 结构_学习方法资产提交输入& 输入,
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        方法节点* 方法头,
        结构_任务管理结果& 结果,
        const std::string& 调用点) noexcept
    {
        if (!方法头) {
            return;
        }

        auto& 方法主信息 = 方法头->主信息;
        auto* 条件场景 = 方法主信息.条件场景.指针;
        auto* 结果场景 = 方法主信息.结果场景.指针;
        if (!条件场景 && !结果场景) {
            return;
        }

        const auto& 治理证据 = 结果.当前治理二次特征证据;
        const auto 证据摘要 = !输入.当前治理证据摘要.empty()
            ? 输入.当前治理证据摘要
            : 治理证据.摘要;
        const auto 因果摘要 = !输入.最近抽象因果摘要.empty()
            ? 输入.最近抽象因果摘要
            : 结果.当前需求位特征.最近抽象因果摘要;
        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        const bool 已触发学习承接 = 私有_P0_已触发学习承接(结果);
        const bool 有抽象因果 =
            !输入.最近抽象因果主键.empty()
            || 结果.当前需求位特征.最近抽象因果 != nullptr
            || !因果摘要.empty();
        const I64 置信度 = 治理证据.综合置信度_Q10000 != 0
            ? 治理证据.综合置信度_Q10000
            : 7000;

        std::vector<可解析引用<二次特征节点类>> 条件索引 = 方法主信息.条件判定索引;
        std::vector<可解析引用<二次特征节点类>> 结果索引 = 方法主信息.结果摘要索引;
        auto 写条件证据 = [&](const std::string& 名称, const bool 值, const std::string& 原因) {
            二次特征节点类* 节点 = nullptr;
            if (私有_P2_写场景治理二次特征_布尔(
                条件场景,
                名称,
                值,
                原因,
                置信度,
                &节点)) {
                私有_P2_追加二次特征引用(条件索引, 节点);
            }
        };
        auto 写结果证据 = [&](const std::string& 名称, const bool 值, const std::string& 原因) {
            二次特征节点类* 节点 = nullptr;
            if (私有_P2_写场景治理二次特征_布尔(
                结果场景,
                名称,
                值,
                原因,
                置信度,
                &节点)) {
                私有_P2_追加二次特征引用(结果索引, 节点);
            }
        };

        if (条件场景) {
            if (治理证据.有证据) {
                写条件证据("任务条件满足", 治理证据.任务条件满足, 证据摘要);
                写条件证据("目标结果方向一致", 治理证据.目标结果方向一致, 证据摘要);
                写条件证据("目标结果已落范围", 治理证据.目标结果已落范围, 证据摘要);
                写条件证据("当前存在显式冲突", 治理证据.当前存在显式冲突, 证据摘要);
                写条件证据("影子验证稳定", 治理证据.影子验证稳定, 证据摘要);
                写条件证据("控制请求待确认", 治理证据.控制请求待确认, 证据摘要);
                写条件证据("控制请求已拒绝", 治理证据.控制请求已拒绝, 证据摘要);
                写条件证据("高阶排序已完成", 治理证据.高阶排序已完成, 证据摘要);
                写条件证据("高阶排序胜出已绑定", 治理证据.高阶排序胜出已绑定, 证据摘要);
                写条件证据("稳定等待态复盘学习", 治理证据.稳定等待态复盘学习, 证据摘要);
            }
            写条件证据("当前步骤为等待维持", 当前步骤语义类型 == 枚举_任务管理步骤语义类型::等待维持步骤, 证据摘要);
            写条件证据("当前步骤为业务推进", 当前步骤语义类型 == 枚举_任务管理步骤语义类型::业务推进步骤, 证据摘要);
            写条件证据("当前已触发学习承接", 已触发学习承接, 私有_P0_当前学习承接摘要(结果));
            写条件证据("抽象因果已具备", 有抽象因果, 因果摘要);
        }

        if (结果场景) {
            if (治理证据.有证据) {
                写结果证据("当前可回接最近结果", 治理证据.当前可回接最近结果, 证据摘要);
                写结果证据("稳定等待态复盘学习", 治理证据.稳定等待态复盘学习, 证据摘要);
                写结果证据("业务子任务已完成", 治理证据.业务子任务已完成, 证据摘要);
                写结果证据("最近结果命中当前治理", 治理证据.最近结果命中当前治理, 证据摘要);
                写结果证据("当前步骤可复用", 治理证据.当前步骤可复用, 证据摘要);
                写结果证据("可继续推进", 治理证据.可继续推进, 证据摘要);
                写结果证据("当前需等待", 治理证据.当前需等待, 证据摘要);
                写结果证据("当前需收束", 治理证据.当前需收束, 证据摘要);
                写结果证据("当前需重筹办", 治理证据.当前需重筹办, 证据摘要);
            }
            写结果证据("当前步骤为等待维持", 当前步骤语义类型 == 枚举_任务管理步骤语义类型::等待维持步骤, 证据摘要);
            写结果证据("当前步骤为业务推进", 当前步骤语义类型 == 枚举_任务管理步骤语义类型::业务推进步骤, 证据摘要);
            写结果证据("当前已触发学习承接", 已触发学习承接, 私有_P0_当前学习承接摘要(结果));
            写结果证据("抽象因果已具备", 有抽象因果, 因果摘要);
        }

        if (auto* 抽象因果 = 结果.当前需求位特征.最近抽象因果) {
            私有_P2_吸收抽象因果到方法结构(
                宿主任务,
                方法头,
                抽象因果,
                条件索引,
                结果索引,
                置信度,
                结果,
                调用点 + "/抽象因果");
        }

        私有_P2_吸收二次特征引用列表(方法主信息.条件判定索引, 条件索引);
        私有_P2_吸收二次特征引用列表(方法主信息.结果摘要索引, 结果索引);
        世界树.二次特征生成().确保二次特征列表具备默认值(方法主信息.条件判定索引);
        世界树.二次特征生成().确保二次特征列表具备默认值(方法主信息.结果摘要索引);
        if (方法主信息.条件主键.empty()) {
            方法主信息.条件主键 = !输入.当前输入条件主键.empty()
                ? 输入.当前输入条件主键
                : 结果.当前需求位特征.输入条件主键;
        }
        if (方法主信息.结果主键.empty()) {
            方法主信息.结果主键 = !输入.当前结果主键.empty()
                ? 输入.当前结果主键
                : 结果.当前需求位特征.结果主键;
        }
        if (方法主信息.条件场景模板主键.empty() && 条件场景) {
            方法主信息.条件场景模板主键 = 条件场景->获取主键();
        }

        auto* 条件节点 = 私有_查找直接子节点_按谓词(方法头, [&](const 方法节点* 节点) {
            return 节点
                && 节点->主信息.节点种类 == 枚举_方法节点种类::方法条件节点
                && 私有_词性相同(节点->主信息.动作名, 私有_学习方法条件节点动作名词(宿主任务, 结果))
                && (!学习子任务
                    || reinterpret_cast<const void*>(节点->主信息.来源任务.指针)
                        == reinterpret_cast<const void*>(学习子任务));
        });
        if (条件节点) {
            私有_P2_吸收二次特征引用列表(条件节点->主信息.条件判定索引, 方法主信息.条件判定索引);
            世界树.二次特征生成().确保二次特征列表具备默认值(条件节点->主信息.条件判定索引);
            if (条件节点->主信息.条件主键.empty()) {
                条件节点->主信息.条件主键 = 方法主信息.条件主键;
            }
            if (条件节点->主信息.条件场景模板主键.empty()) {
                条件节点->主信息.条件场景模板主键 = 方法主信息.条件场景模板主键;
            }
        }

        auto* 结果节点 = 私有_查找直接子节点_按谓词(方法头, [&](const 方法节点* 节点) {
            return 节点
                && 节点->主信息.节点种类 == 枚举_方法节点种类::方法结果节点
                && 私有_词性相同(节点->主信息.动作名, 私有_学习方法结果节点动作名词(宿主任务, 结果))
                && (!学习子任务
                    || reinterpret_cast<const void*>(节点->主信息.来源任务.指针)
                        == reinterpret_cast<const void*>(学习子任务));
        });
        if (结果节点) {
            私有_P2_吸收二次特征引用列表(结果节点->主信息.结果摘要索引, 方法主信息.结果摘要索引);
            世界树.二次特征生成().确保二次特征列表具备默认值(结果节点->主信息.结果摘要索引);
            if (结果节点->主信息.结果主键.empty()) {
                结果节点->主信息.结果主键 = 方法主信息.结果主键;
            }
        }

        (void)调用点;
    }

    void 私有_补齐学习方法结构(
        自我类& 自我对象,
        任务节点* 学习子任务,
        任务节点* 宿主任务,
        方法节点* 方法头,
        bool 作为尝试学习选项,
        结构_任务管理结果& 结果,
        const std::string& 调用点) noexcept
    {
        if (!方法头) {
            return;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);
        auto& 主信息 = 方法头->主信息;
        主信息.节点种类 = 枚举_方法节点种类::方法首节点;
        if (!主信息.动作名) {
            主信息.动作名 = 私有_学习方法动作名词(宿主任务, 结果);
        }
        if (学习子任务 && !主信息.来源任务.指针) {
            私有_写入引用(主信息.来源任务, 学习子任务);
        }
        if (作为尝试学习选项
            && (!主信息.动作名 || !私有_动作句柄为尝试学习(主信息.动作句柄))) {
            主信息.动作名 = 私有_学习方法动作名词(宿主任务, 结果);
            私有_写入尝试学习动作句柄(主信息.动作句柄);
        }
        if (!作为尝试学习选项
            && 主信息.动作句柄.类型 == 枚举_动作句柄类型::未设置
            && 当前方法
            && !私有_动作句柄为尝试学习(当前方法->主信息.动作句柄)
            && 当前方法->主信息.动作句柄.类型 != 枚举_动作句柄类型::未设置) {
            主信息.动作句柄 = 当前方法->主信息.动作句柄;
        }
        if (主信息.结果初始状态.empty()) {
            if (auto* 初始状态 = 私有_学习方法条件目标状态(宿主任务, 结果)) {
                私有_追加状态引用(主信息.结果初始状态, 初始状态);
            }
        }
        if (!主信息.主结果特征类型) {
            主信息.主结果特征类型 = 私有_学习方法主结果特征类型(宿主任务, 结果);
        }

        auto* 自我内部世界 = 自我对象.确保自我内部世界(调用点 + "/自我内部世界");
        if (!自我内部世界) {
            return;
        }

        auto* 方法虚拟存在 = 主信息.方法虚拟存在.指针;
        if (!方法虚拟存在) {
            方法虚拟存在 = 世界树.存在().取或创建子存在_按名称类型(
                自我内部世界,
                私有_学习方法虚拟存在名称词(宿主任务, 结果),
                私有_方法虚拟存在类型词());
            if (方法虚拟存在) {
                私有_写入引用(主信息.方法虚拟存在, 方法虚拟存在);
            }
        }

        if (方法虚拟存在) {
            const auto 方法类型 = 私有_推导方法类型(方法头, &结果);
            if (方法类型 != 枚举_方法类型::未定义) {
                (void)私有_通过执行壳写回I64(
                    自我对象,
                    枚举_最小原语写回目标宿主::方法资产,
                    私有_方法特征_方法类型(),
                    static_cast<I64>(方法类型),
                    "方法_方法类型写回",
                    方法头,
                    宿主任务,
                    学习子任务 && 学习子任务->主信息.启动时间 != 0
                        ? 学习子任务->主信息.启动时间
                        : (宿主任务 ? 宿主任务->主信息.启动时间 : 0),
                    调用点 + "/方法类型",
                    结果);
            }
        }

        if (方法虚拟存在 && !主信息.条件场景.指针) {
            if (auto* 条件场景 = 世界树.场景().取或创建子场景_按名称(
                方法虚拟存在,
                私有_学习方法条件场景名称词(宿主任务, 结果),
                枚举_世界类型::内部世界)) {
                私有_写入引用(主信息.条件场景, 条件场景);
            }
        }

        if (方法虚拟存在 && !主信息.结果场景.指针) {
            if (auto* 结果场景 = 世界树.场景().取或创建子场景_按名称(
                方法虚拟存在,
                私有_学习方法结果场景名称词(宿主任务, 结果),
                枚举_世界类型::内部世界)) {
                私有_写入引用(主信息.结果场景, 结果场景);
            }
        }
    }

    方法节点* 私有_创建或复用学习方法头(
        自我类& 自我对象,
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        bool 作为尝试学习选项,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!学习子任务) {
            return nullptr;
        }

        auto* 当前方法 = 私有_P0_当前方法(结果);
        if (!作为尝试学习选项
            && !私有_应新生学习方法头(结果)
            && 当前方法
            && !私有_动作句柄为尝试学习(当前方法->主信息.动作句柄)) {
            结果.当前学习方法节点 = 当前方法;
            结果.已复用学习方法头 = true;
            私有_写入引用(学习子任务->主信息.执行方法, 当前方法);
            学习子任务->主信息.动作句柄 = 当前方法->主信息.动作句柄;
            return 结果.当前学习方法节点;
        }

        auto* 自我存在 = 自我对象.确保自我存在(调用点 + "/自我存在");
        if (!自我存在) {
            return nullptr;
        }

        auto* 方法根节点 = 世界树.存在().获取方法根节点(自我存在);
        if (!方法根节点) {
            return nullptr;
        }

        auto* 方法头 = 私有_查找学习方法头候选(
            自我对象,
            学习子任务,
            宿主任务,
            结果,
            作为尝试学习选项);
        if (方法头) {
            结果.已复用学习方法头 = true;
        }
        else {
            方法首节点主信息类 主信息{};
            主信息.动作名 = 私有_学习方法动作名词(宿主任务, 结果);
            主信息.来源 = 枚举_方法来源::运行时学习沉淀;
            主信息.主结果特征类型 = 私有_学习方法主结果特征类型(宿主任务, 结果);
            if (作为尝试学习选项) {
                私有_写入尝试学习动作句柄(主信息.动作句柄);
            }
            else if (当前方法
                && !私有_动作句柄为尝试学习(当前方法->主信息.动作句柄)
                && 当前方法->主信息.动作句柄.类型 != 枚举_动作句柄类型::未设置) {
                主信息.动作句柄 = 当前方法->主信息.动作句柄;
            }
            if (auto* 初始状态 = 私有_学习方法条件目标状态(宿主任务, 结果)) {
                私有_追加状态引用(主信息.结果初始状态, 初始状态);
            }
            私有_写入引用(主信息.来源任务, 学习子任务);
            方法头 = 私有_独立树添加方法子节点(方法根节点, 主信息);
            结果.已创建学习方法头 = 方法头 != nullptr;
        }

        私有_补齐学习方法结构(
            自我对象,
            学习子任务,
            宿主任务,
            方法头,
            作为尝试学习选项,
            结果,
            调用点 + "/补齐结构");

        if (方法头) {
            私有_写入引用(学习子任务->主信息.执行方法, 方法头);
            学习子任务->主信息.动作句柄 = 方法头->主信息.动作句柄;
            结果.当前学习方法节点 = 方法头;
        }

        (void)now;
        return 方法头;
    }

    方法节点* 私有_查找学习方法直接子节点(
        const 方法节点* 方法头,
        枚举_方法节点种类 节点种类,
        const 词性节点类* 动作名,
        const 任务节点* 学习子任务) noexcept
    {
        if (!方法头 || !动作名) {
            return nullptr;
        }

        auto* 首节点 = static_cast<方法节点*>(方法头->子);
        if (!首节点) {
            return nullptr;
        }

        auto* 当前 = 首节点;
        do {
            if (当前
                && 当前->主信息.节点种类 == 节点种类
                && 私有_词性相同(当前->主信息.动作名, 动作名)
                && (!学习子任务
                    || reinterpret_cast<const void*>(当前->主信息.来源任务.指针)
                        == reinterpret_cast<const void*>(学习子任务))) {
                return 当前;
            }

            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    方法节点* 私有_P2_查找运行方法直接子节点(
        const 方法节点* 方法头,
        枚举_方法节点种类 节点种类,
        const 词性节点类* 动作名,
        const 任务节点* 来源任务) noexcept
    {
        if (!方法头 || !动作名) {
            return nullptr;
        }

        auto* 首节点 = static_cast<方法节点*>(方法头->子);
        if (!首节点) {
            return nullptr;
        }

        auto* 当前 = 首节点;
        do {
            if (当前
                && 当前->主信息.节点种类 == 节点种类
                && 私有_词性相同(当前->主信息.动作名, 动作名)
                && (!来源任务
                    || reinterpret_cast<const void*>(当前->主信息.来源任务.指针)
                        == reinterpret_cast<const void*>(来源任务))) {
                return 当前;
            }

            当前 = static_cast<方法节点*>(当前->下);
        } while (当前 && 当前 != 首节点);

        return nullptr;
    }

    方法节点* 私有_P2_创建或复用运行方法条件节点(
        任务节点* 宿主任务,
        方法节点* 方法头) noexcept
    {
        if (!方法头 || !方法头->主信息.动作名 || !方法头->主信息.条件场景.指针) {
            return nullptr;
        }

        auto* 条件节点 = 私有_P2_查找运行方法直接子节点(
            方法头,
            枚举_方法节点种类::方法条件节点,
            方法头->主信息.动作名,
            宿主任务);
        if (条件节点) {
            if (条件节点->主信息.条件判定索引.empty()) {
                条件节点->主信息.条件判定索引 = 方法头->主信息.条件判定索引;
            }
            if (条件节点->主信息.条件主键.empty()) {
                条件节点->主信息.条件主键 = 方法头->主信息.条件主键;
            }
            if (条件节点->主信息.条件场景模板主键.empty()) {
                条件节点->主信息.条件场景模板主键 = 方法头->主信息.条件场景模板主键;
            }
            return 条件节点;
        }

        方法条件节点主信息类 主信息{};
        主信息.动作名 = 方法头->主信息.动作名;
        主信息.条件场景 = 方法头->主信息.条件场景;
        主信息.条件判定索引 = 方法头->主信息.条件判定索引;
        主信息.条件主键 = 方法头->主信息.条件主键;
        主信息.条件场景模板主键 = 方法头->主信息.条件场景模板主键;
        私有_写入引用(主信息.来源任务, 宿主任务);
        私有_写入引用(主信息.父方法, 方法头);
        for (const auto& 初始状态引用 : 方法头->主信息.结果初始状态) {
            if (auto* 初始状态 = 私有_解析状态引用(初始状态引用)) {
                私有_追加状态引用(主信息.结果初始状态, 初始状态);
            }
        }
        return 私有_独立树添加方法子节点(方法头, 主信息);
    }

    方法节点* 私有_P2_创建或复用运行方法结果节点(
        任务节点* 宿主任务,
        方法节点* 方法头) noexcept
    {
        if (!方法头 || !方法头->主信息.动作名 || !方法头->主信息.结果场景.指针) {
            return nullptr;
        }

        auto* 结果节点 = 私有_P2_查找运行方法直接子节点(
            方法头,
            枚举_方法节点种类::方法结果节点,
            方法头->主信息.动作名,
            宿主任务);
        if (结果节点) {
            if (结果节点->主信息.结果摘要索引.empty()) {
                结果节点->主信息.结果摘要索引 = 方法头->主信息.结果摘要索引;
            }
            if (结果节点->主信息.结果主键.empty()) {
                结果节点->主信息.结果主键 = 方法头->主信息.结果主键;
            }
            return 结果节点;
        }

        方法结果节点主信息类 主信息{};
        主信息.动作名 = 方法头->主信息.动作名;
        主信息.结果场景 = 方法头->主信息.结果场景;
        主信息.结果摘要索引 = 方法头->主信息.结果摘要索引;
        主信息.结果主键 = 方法头->主信息.结果主键;
        私有_写入引用(主信息.来源任务, 宿主任务);
        私有_写入引用(主信息.父方法, 方法头);
        for (const auto& 初始状态引用 : 方法头->主信息.结果初始状态) {
            if (auto* 初始状态 = 私有_解析状态引用(初始状态引用)) {
                私有_追加状态引用(主信息.结果初始状态, 初始状态);
            }
        }
        return 私有_独立树添加方法子节点(方法头, 主信息);
    }

    bool 私有_P2_运行证据可沉淀当前方法(const 结构_任务管理结果& 结果) noexcept
    {
        const auto& 治理证据 = 结果.当前治理二次特征证据;
        if (!治理证据.有证据) {
            return false;
        }

        const bool 有因果来源 =
            结果.当前需求位特征.最近抽象因果 != nullptr
            || !结果.当前需求位特征.最近抽象因果摘要.empty();
        const bool 有稳定判据 =
            结果.当前原语判定汇总.因果稳定
            || 治理证据.当前动作已形成稳定结果
            || 治理证据.影子验证稳定
            || 治理证据.目标结果已落范围;
        return 有因果来源 && 有稳定判据;
    }

    void 私有_P2_补齐运行方法证据索引(
        任务节点* 宿主任务,
        方法节点* 方法头,
        结构_任务管理结果& 结果,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务 || !方法头 || !私有_P2_运行证据可沉淀当前方法(结果)) {
            return;
        }

        auto& 方法主信息 = 方法头->主信息;
        auto* 条件场景 = 方法主信息.条件场景.指针;
        auto* 结果场景 = 方法主信息.结果场景.指针;
        if (!条件场景 && !结果场景) {
            return;
        }

        const auto& 治理证据 = 结果.当前治理二次特征证据;
        const auto& 证据摘要 = 治理证据.摘要;
        const auto& 因果摘要 = 结果.当前需求位特征.最近抽象因果摘要;
        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        const bool 已触发学习承接 = 私有_P0_已触发学习承接(结果);
        const bool 有抽象因果 =
            结果.当前需求位特征.最近抽象因果 != nullptr
            || !因果摘要.empty();
        const I64 置信度 = 治理证据.综合置信度_Q10000 != 0
            ? 治理证据.综合置信度_Q10000
            : (结果.当前原语判定汇总.综合置信度_Q10000 != 0
                ? 结果.当前原语判定汇总.综合置信度_Q10000
                : 7000);

        std::vector<可解析引用<二次特征节点类>> 条件索引 = 方法主信息.条件判定索引;
        std::vector<可解析引用<二次特征节点类>> 结果索引 = 方法主信息.结果摘要索引;
        auto 写条件证据 = [&](const std::string& 名称, const bool 值, const std::string& 原因) {
            二次特征节点类* 节点 = nullptr;
            if (私有_P2_写场景治理二次特征_布尔(
                条件场景,
                名称,
                值,
                原因,
                置信度,
                &节点)) {
                私有_P2_追加二次特征引用(条件索引, 节点);
            }
        };
        auto 写结果证据 = [&](const std::string& 名称, const bool 值, const std::string& 原因) {
            二次特征节点类* 节点 = nullptr;
            if (私有_P2_写场景治理二次特征_布尔(
                结果场景,
                名称,
                值,
                原因,
                置信度,
                &节点)) {
                私有_P2_追加二次特征引用(结果索引, 节点);
            }
        };

        if (条件场景) {
            写条件证据("任务条件满足", 治理证据.任务条件满足, 证据摘要);
            写条件证据("目标结果方向一致", 治理证据.目标结果方向一致, 证据摘要);
            写条件证据("目标结果已落范围", 治理证据.目标结果已落范围, 证据摘要);
            写条件证据("当前存在显式冲突", 治理证据.当前存在显式冲突, 证据摘要);
            写条件证据("影子验证稳定", 治理证据.影子验证稳定, 证据摘要);
            写条件证据("当前步骤为等待维持", 当前步骤语义类型 == 枚举_任务管理步骤语义类型::等待维持步骤, 证据摘要);
            写条件证据("当前步骤为业务推进", 当前步骤语义类型 == 枚举_任务管理步骤语义类型::业务推进步骤, 证据摘要);
            写条件证据("稳定等待态复盘学习", 治理证据.稳定等待态复盘学习, 证据摘要);
            写条件证据("业务子任务应转执行", 治理证据.业务子任务应转执行, 证据摘要);
            写条件证据("当前已触发学习承接", 已触发学习承接, 私有_P0_当前学习承接摘要(结果));
            写条件证据("抽象因果已具备", 有抽象因果, 因果摘要);
        }

        if (结果场景) {
            写结果证据("当前动作已形成稳定结果", 治理证据.当前动作已形成稳定结果, 证据摘要);
            写结果证据("当前可回接最近结果", 治理证据.当前可回接最近结果, 证据摘要);
            写结果证据("最近结果命中当前治理", 治理证据.最近结果命中当前治理, 证据摘要);
            写结果证据("当前步骤可复用", 治理证据.当前步骤可复用, 证据摘要);
            写结果证据("可继续推进", 治理证据.可继续推进, 证据摘要);
            写结果证据("当前需等待", 治理证据.当前需等待, 证据摘要);
            写结果证据("当前需收束", 治理证据.当前需收束, 证据摘要);
            写结果证据("稳定等待态复盘学习", 治理证据.稳定等待态复盘学习, 证据摘要);
            写结果证据("当前需重筹办", 治理证据.当前需重筹办, 证据摘要);
            写结果证据("当前步骤为等待维持", 当前步骤语义类型 == 枚举_任务管理步骤语义类型::等待维持步骤, 证据摘要);
            写结果证据("当前步骤为业务推进", 当前步骤语义类型 == 枚举_任务管理步骤语义类型::业务推进步骤, 证据摘要);
            写结果证据("业务子任务应转执行", 治理证据.业务子任务应转执行, 证据摘要);
            写结果证据("当前已触发学习承接", 已触发学习承接, 私有_P0_当前学习承接摘要(结果));
            写结果证据("抽象因果已具备", 有抽象因果, 因果摘要);
        }

        if (auto* 抽象因果 = 结果.当前需求位特征.最近抽象因果) {
            私有_P2_吸收抽象因果到方法结构(
                宿主任务,
                方法头,
                抽象因果,
                条件索引,
                结果索引,
                置信度,
                结果,
                调用点 + "/抽象因果");
        }

        私有_P2_吸收二次特征引用列表(方法主信息.条件判定索引, 条件索引);
        私有_P2_吸收二次特征引用列表(方法主信息.结果摘要索引, 结果索引);
        世界树.二次特征生成().确保二次特征列表具备默认值(方法主信息.条件判定索引);
        世界树.二次特征生成().确保二次特征列表具备默认值(方法主信息.结果摘要索引);
        if (方法主信息.条件主键.empty()) {
            方法主信息.条件主键 = 结果.当前需求位特征.输入条件主键;
        }
        if (方法主信息.结果主键.empty()) {
            方法主信息.结果主键 = 结果.当前需求位特征.结果主键;
        }
        if (方法主信息.条件场景模板主键.empty() && 条件场景) {
            方法主信息.条件场景模板主键 = 条件场景->获取主键();
        }

        auto* 条件节点 = 私有_P2_创建或复用运行方法条件节点(宿主任务, 方法头);
        if (条件节点) {
            私有_P2_吸收二次特征引用列表(条件节点->主信息.条件判定索引, 方法主信息.条件判定索引);
            世界树.二次特征生成().确保二次特征列表具备默认值(条件节点->主信息.条件判定索引);
            if (条件节点->主信息.条件主键.empty()) {
                条件节点->主信息.条件主键 = 方法主信息.条件主键;
            }
            if (条件节点->主信息.条件场景模板主键.empty()) {
                条件节点->主信息.条件场景模板主键 = 方法主信息.条件场景模板主键;
            }
        }

        auto* 结果节点 = 私有_P2_创建或复用运行方法结果节点(宿主任务, 方法头);
        if (结果节点) {
            私有_P2_吸收二次特征引用列表(结果节点->主信息.结果摘要索引, 方法主信息.结果摘要索引);
            世界树.二次特征生成().确保二次特征列表具备默认值(结果节点->主信息.结果摘要索引);
            if (结果节点->主信息.结果主键.empty()) {
                结果节点->主信息.结果主键 = 方法主信息.结果主键;
            }
        }

        私有_追加最近反馈片段(
            结果,
            "运行时方法证据沉淀="
            + 私有_方法标题(方法头)
            + " | 因果="
            + "抽象因果"
            + " | 调用点="
            + 调用点);
    }

    方法节点* 私有_创建或复用学习方法条件节点(
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        方法节点* 方法头,
        结构_任务管理结果& 结果) noexcept
    {
        if (!方法头 || !学习子任务) {
            return nullptr;
        }

        auto* 动作名 = 私有_学习方法条件节点动作名词(宿主任务, 结果);
        auto* 条件节点 = 私有_查找学习方法直接子节点(
            方法头,
            枚举_方法节点种类::方法条件节点,
            动作名,
            学习子任务);
        if (条件节点) {
            if (条件节点->主信息.条件判定索引.empty()) {
                条件节点->主信息.条件判定索引 = 方法头->主信息.条件判定索引;
            }
            for (const auto& 条件引用 : 条件节点->主信息.条件判定索引) {
                auto* 条件特征 = 条件引用.指针;
                (void)世界树.二次特征生成().确保二次特征具备默认值(条件特征);
            }
            return 条件节点;
        }

        方法条件节点主信息类 主信息{};
        主信息.动作名 = 动作名;
        主信息.条件场景 = 方法头->主信息.条件场景;
        主信息.条件判定索引 = 方法头->主信息.条件判定索引;
        for (const auto& 条件引用 : 主信息.条件判定索引) {
            auto* 条件特征 = 条件引用.指针;
            (void)世界树.二次特征生成().确保二次特征具备默认值(条件特征);
        }
        私有_写入引用(主信息.来源任务, 学习子任务);
        私有_写入引用(主信息.父方法, 方法头);
        if (auto* 初始状态 = 私有_学习方法条件目标状态(宿主任务, 结果)) {
            私有_追加状态引用(主信息.结果初始状态, 初始状态);
        }
        条件节点 = 私有_独立树添加方法子节点(方法头, 主信息);
        结果.已创建学习方法条件节点 = 条件节点 != nullptr;
        return 条件节点;
    }

    方法节点* 私有_创建或复用学习方法结果节点(
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        方法节点* 方法头,
        结构_任务管理结果& 结果) noexcept
    {
        if (!方法头 || !学习子任务) {
            return nullptr;
        }

        auto* 动作名 = 私有_学习方法结果节点动作名词(宿主任务, 结果);
        auto* 结果节点 = 私有_查找学习方法直接子节点(
            方法头,
            枚举_方法节点种类::方法结果节点,
            动作名,
            学习子任务);
        if (结果节点) {
            return 结果节点;
        }

        方法结果节点主信息类 主信息{};
        主信息.动作名 = 动作名;
        主信息.结果场景 = 方法头->主信息.结果场景;
        主信息.结果摘要索引 = 方法头->主信息.结果摘要索引;
        主信息.结果主键 = 方法头->主信息.结果主键;
        私有_写入引用(主信息.来源任务, 学习子任务);
        私有_写入引用(主信息.父方法, 方法头);
        for (const auto& 初始状态引用 : 方法头->主信息.结果初始状态) {
            if (auto* 初始状态 = 私有_解析状态引用(初始状态引用)) {
                私有_追加状态引用(主信息.结果初始状态, 初始状态);
            }
        }
        结果节点 = 私有_独立树添加方法子节点(方法头, 主信息);
        结果.已创建学习方法结果节点 = 结果节点 != nullptr;
        return 结果节点;
    }

    方法节点* 私有_写回学习方法骨架(
        自我类& 自我对象,
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        bool 作为尝试学习选项,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        auto* 方法头 = 私有_创建或复用学习方法头(
            自我对象,
            宿主任务,
            学习子任务,
            作为尝试学习选项,
            结果,
            now,
            调用点 + "/方法头");
        if (!方法头) {
            return nullptr;
        }

        (void)私有_创建或复用学习方法条件节点(
            宿主任务,
            学习子任务,
            方法头,
            结果);
        (void)私有_创建或复用学习方法结果节点(
            宿主任务,
            学习子任务,
            方法头,
            结果);
        return 方法头;
    }

    bool 私有_同步学习任务实体(
        自我类& 自我对象,
        任务节点* 宿主任务,
        任务节点* 学习子任务,
        方法节点* 学习任务兜底方法,
        方法节点* 当前选中学习方法,
        const std::string& 学习方法选项摘要,
        const 结构_学习承接决策& 学习决策,
        const 结构_任务管理结果& 结果,
        时间戳 now) noexcept
    {
        if (!学习子任务) {
            return false;
        }

        auto* 当前步骤 = 私有_P0_当前步骤节点(结果);
        auto* 最近结果 = 私有_P0_最新结果节点(结果);
        auto* 当前方法 = 私有_P0_当前方法(结果);
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前方法来源 = 私有_P0_当前方法来源(结果);
        const bool 本能补齐学习 = 学习决策.补原语或治理本能;
        const bool 兜底方法已可执行 = 本能补齐学习
            ? false
            : 私有_学习方法可直接进入学习(学习任务兜底方法);
        const bool 当前候选方法已可执行 = 本能补齐学习
            ? false
            : 私有_学习方法可直接进入学习(当前选中学习方法);
        const bool 已切换到兜底可执行方法 =
            !当前候选方法已可执行
            && 兜底方法已可执行
            && 学习任务兜底方法
            && 学习任务兜底方法 != 当前选中学习方法;
        auto* 实际学习方法 = 当前候选方法已可执行
            ? 当前选中学习方法
            : (兜底方法已可执行 ? 学习任务兜底方法 : (当前选中学习方法 ? 当前选中学习方法 : 学习任务兜底方法));
        std::ostringstream 当前学习方法选择摘要;
        if (本能补齐学习) {
            当前学习方法选择摘要 << "本轮为本能补齐学习，当前方法按补齐通道执行";
        }
        else if (已切换到兜底可执行方法) {
            当前学习方法选择摘要
                << "当前候选未就绪，已切换到兜底可执行方法="
                << 私有_方法标题(学习任务兜底方法);
        }
        else if (当前候选方法已可执行) {
            当前学习方法选择摘要
                << "当前候选已可执行，保持当前学习方法="
                << 私有_方法标题(当前选中学习方法);
        }
        else if (兜底方法已可执行) {
            当前学习方法选择摘要
                << "当前学习方法使用兜底可执行方法="
                << 私有_方法标题(学习任务兜底方法);
        }
        else {
            当前学习方法选择摘要 << "当前仍缺可执行学习方法头";
        }

        auto* 待学习方法头 = 当前选中学习方法
            ? 当前选中学习方法
            : 实际学习方法;
        (void)私有_同步学习任务虚拟存在特征(
            自我对象,
            学习子任务,
            待学习方法头,
            学习任务兜底方法,
            学习决策,
            结果,
            本能补齐学习 ? false : 私有_学习方法可直接进入学习(待学习方法头),
            本能补齐学习 ? false : 已切换到兜底可执行方法,
            now,
            "任务管理任务模块::同步学习任务实体");
        const auto 学习镜像 = 私有_读取学习任务虚拟存在镜像(
            自我对象,
            学习子任务,
            "任务管理任务模块::同步学习任务实体/读回学习任务虚拟存在");
        if (学习镜像.当前待学习方法头) {
            待学习方法头 = 学习镜像.当前待学习方法头;
            实际学习方法 = 学习镜像.当前待学习方法头;
        }

        结构_学习任务同步输入 输入{};
        输入.当前时间 = now;
        输入.去重键 = 学习子任务->获取主键();
        输入.学习子任务 = 学习子任务;
        输入.学习方法 = 实际学习方法;
        输入.学习任务兜底方法 = 学习任务兜底方法;
        输入.当前选中学习方法 = 实际学习方法;
        输入.来源宿主任务 = 宿主任务;
        输入.来源步骤 = 当前步骤;
        输入.来源结果 = 最近结果;
        输入.来源方法 = 当前方法;
        输入.最近根层重判结果 = 结果.最近根层重判结果;
        输入.当前缺口类型 = 当前缺口类型;
        输入.当前下一步去向 = 当前下一步去向;
        输入.当前方法来源 = 当前方法来源;
        输入.当前触发来源 = 结果.当前触发来源;
        输入.当前学习承接类型 = 学习决策.学习类型;
        输入.来源方法需求位 = 学习决策.来源方法需求位;
        输入.来源本能能力缺口类型 = 学习决策.来源本能能力缺口类型;
        输入.方法服务目标 = 学习决策.方法服务目标;
        输入.学习情况消费策略 = 学习决策.默认消费策略;
        输入.回流目标方法位 = 学习决策.回流目标方法位;
        输入.仅需求位方法头 = 本能补齐学习
            ? false
            : (学习镜像.存在读回
                ? !(学习镜像.当前学习方法有动作骨架 && 学习镜像.当前学习方法条件数量 > 0)
                : 私有_学习方法仅需求位(输入.学习方法));
        输入.当前学习方法有动作骨架 = 学习镜像.存在读回
            ? 学习镜像.当前学习方法有动作骨架
            : 私有_学习方法有动作骨架(待学习方法头);
        输入.当前学习方法条件数量 = 学习镜像.存在读回
            ? 学习镜像.当前学习方法条件数量
            : 私有_学习方法条件数量(待学习方法头);
        输入.当前学习方法结果数量 = 学习镜像.存在读回
            ? 学习镜像.当前学习方法结果数量
            : 私有_学习方法结果数量(待学习方法头);
        输入.当前学习方法可用 = 学习镜像.存在读回
            ? 学习镜像.当前学习方法可用
            : (本能补齐学习 ? false : 私有_学习方法已具备可用骨架(待学习方法头));
        输入.学习任务兜底方法已可执行 = 本能补齐学习 ? false : 兜底方法已可执行;
        输入.当前选中学习方法已可执行 = 本能补齐学习
            ? false
            : 私有_学习方法可直接进入学习(输入.当前选中学习方法);
        输入.已切换到兜底可执行方法 = 本能补齐学习
            ? false
            : (学习镜像.存在读回
                ? 学习镜像.当前已切换到兜底可执行方法
                : 已切换到兜底可执行方法);
        输入.当前安全值 = 自我对象.获取安全值();
        输入.当前服务值 = 自我对象.获取服务值();
        输入.当前风险安全 = 自我对象.获取风险安全值();
        输入.来源最小原语位图 = 学习决策.来源最小原语位图;
        输入.当前学习目标 = 学习决策.学习目标;
        输入.当前学习需求集合 = 学习决策.学习需求集合;
        输入.当前学习动作集合 = 学习决策.学习动作集合;
        输入.当前原函数需求集合 = 学习决策.原函数需求集合;
        输入.当前候选学习程序 = 学习决策.候选学习程序;
        输入.来源正式原语摘要 = 学习决策.来源正式原语摘要;
        输入.来源主观察特征 = 学习决策.来源主观察特征;
        输入.当前治理证据摘要 = 结果.当前治理二次特征证据.摘要;
        输入.当前输入条件主键 = 结果.当前需求位特征.输入条件主键;
        输入.当前结果主键 = 结果.当前需求位特征.结果主键;
        输入.最近抽象因果摘要 = 结果.当前需求位特征.最近抽象因果摘要;
        输入.最近抽象因果主键 =
            结果.当前需求位特征.最近抽象因果
            ? 结果.当前需求位特征.最近抽象因果->获取主键()
            : std::string{};
        输入.当前影子验证状态 = 结果.当前影子验证状态;
        输入.允许正式资产提交 = 学习决策.允许正式资产提交;
        输入.学习承接摘要.clear();
        输入.方法服务范围 = 学习决策.方法服务范围;
        输入.学习方法选项摘要 = 学习方法选项摘要;
        输入.当前学习方法选择摘要 = 当前学习方法选择摘要.str();
        输入.学习反馈摘要.clear();
        输入.回流目标摘要 = 学习决策.回流目标摘要;
        输入.可直接进入学习 = 本能补齐学习
            ? (!输入.来源主观察特征.empty()
                || 输入.来源最小原语位图 != 0
                || !输入.回流目标摘要.empty())
            : 输入.当前选中学习方法已可执行;
        输入.最近反馈摘要.clear();
        if (学习镜像.当前学习目标类型 != 枚举_学习目标类型::未定义) {
            输入.当前学习目标.类型 = 学习镜像.当前学习目标类型;
        }
        if (学习镜像.当前学习程序裁决 != 枚举_学习程序决策::未定义) {
            输入.当前候选学习程序.当前裁决 = 学习镜像.当前学习程序裁决;
        }
        return 学习任务模块::创建或更新学习任务(输入, nullptr);
    }

    bool 私有_推进步骤结果子链(
        自我类& 自我对象,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!宿主任务) {
            return false;
        }

        const auto& 原语汇总 = 结果.当前原语判定汇总;
        const bool 有可回接最近结果 = 私有_P0_当前可回接最近结果(结果);
        const bool 业务子任务转执行 = 私有_业务子任务应转入执行位(宿主任务, 结果);
        const auto 已有步骤位类型 = 私有_P0_当前步骤位类型(结果);
        const auto 已有步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        结果.当前步骤位类型 = 业务子任务转执行
            ? 枚举_任务管理步骤位类型::执行位
            : (
                已有步骤位类型 != 枚举_任务管理步骤位类型::未定义
                ? 已有步骤位类型
                : 私有_推导步骤位类型(结果, 有可回接最近结果)
            );
        结果.当前步骤语义类型 = 业务子任务转执行
            ? 枚举_任务管理步骤语义类型::业务推进步骤
            : (
                已有步骤语义类型 != 枚举_任务管理步骤语义类型::未定义
                ? 已有步骤语义类型
                : 私有_推导步骤语义类型(结果, 宿主任务)
            );
        私有_P0_刷新步骤与学习结构字段(结果);
        (void)私有_创建或复用补条件子任务头(
            自我对象,
            宿主任务,
            结果,
            now,
            调用点 + "/补条件子任务");

        auto* 最近结果 = 结果.上下文.最近结果;
        const bool 最近结果命中当前治理 = 私有_P0_最近结果命中当前治理(结果);
        if (!业务子任务转执行 && 最近结果命中当前治理) {
            结果.已命中最近结果 = true;
            结果.当前步骤位类型 = 私有_推导步骤位类型(结果, true);
            结果.当前步骤语义类型 = 私有_推导步骤语义类型(结果, 宿主任务);
            私有_P0_刷新步骤与学习结构字段(结果);
            结果.最新结果节点 = 最近结果;
            auto* 当前步骤 = 最近结果 && 最近结果->父
                ? static_cast<任务节点*>(最近结果->父)
                : nullptr;
            if (当前步骤
                && 当前步骤->主信息.节点种类 == 枚举_任务节点种类::步骤节点
                && 私有_任务节点归属宿主任务(当前步骤, 宿主任务)) {
                结果.当前步骤节点 = 当前步骤;
                结果.上下文.当前步骤 = 当前步骤;
            }
            return false;
        }

        auto* 当前步骤 = 结果.上下文.当前步骤;
        const bool 可复用当前步骤 = 私有_P0_当前步骤可复用(结果);

        if (可复用当前步骤) {
            结果.已复用当前步骤 = true;
            auto* 场景 = 宿主任务->主信息.场景.指针
                ? 宿主任务->主信息.场景.指针
                : (自我对象.获取自我现实场景() ? 自我对象.获取自我现实场景() : 自我对象.获取自我内部世界());
            auto* 父节点 = 当前步骤->父
                ? static_cast<任务节点*>(当前步骤->父)
                : 宿主任务;
            const bool 当前步骤回接最近结果 =
                父节点
                && 父节点->主信息.节点种类 == 枚举_任务节点种类::结果节点
                && 私有_任务节点归属宿主任务(父节点, 宿主任务);
            结果.当前步骤位类型 = 业务子任务转执行
                ? 枚举_任务管理步骤位类型::执行位
                : 私有_推导步骤位类型(结果, 当前步骤回接最近结果);
            结果.当前步骤语义类型 = 业务子任务转执行
                ? 枚举_任务管理步骤语义类型::业务推进步骤
                : 私有_推导步骤语义类型(结果, 宿主任务);
            私有_P0_刷新步骤与学习结构字段(结果);
            私有_同步治理步骤节点(当前步骤, 父节点, 宿主任务, 结果, 场景, now);
            结果.当前步骤节点 = 当前步骤;
            结果.上下文.当前步骤 = 当前步骤;
        }
        else {
            结果.当前步骤位类型 = 业务子任务转执行
                ? 枚举_任务管理步骤位类型::执行位
                : 私有_推导步骤位类型(结果, 有可回接最近结果);
            结果.当前步骤语义类型 = 业务子任务转执行
                ? 枚举_任务管理步骤语义类型::业务推进步骤
                : 私有_推导步骤语义类型(结果, 宿主任务);
            私有_P0_刷新步骤与学习结构字段(结果);
            当前步骤 = 私有_创建治理步骤节点(
                自我对象,
                宿主任务,
                结果,
                now,
                调用点 + "/创建步骤");
        }

        if (!当前步骤) {
            return 结果.已创建步骤节点;
        }

        auto* 结果节点 = 私有_创建治理结果节点(
            自我对象,
            宿主任务,
            当前步骤,
            结果,
            now,
            调用点 + "/创建结果");
        return 结果.已创建步骤节点 || 结果节点 != nullptr;
    }
}

namespace {
    bool 私有_写入宿主任务治理当前值_I64(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        I64 新值,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::宿主任务治理);
        auto* 特征节点 = 私有_确保宿主任务治理特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/任务虚拟存在");
        if (!特征节点) {
            return false;
        }
        I64 当前值 = 0;
        if (世界树.读取特征_I64(特征节点, 当前值) && 当前值 == 新值) {
            return false;
        }
        return 世界树.写入特征_I64(特征节点, 新值, now);
    }

    bool 私有_写入宿主任务治理当前值_I64(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        I64 新值,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        return 私有_写入宿主任务治理当前值_I64(
            自我对象,
            自我对象.获取当前主任务(),
            特征类型,
            新值,
            now,
            调用点);
    }

    bool 私有_写入宿主任务治理当前值_指针(
        自我类& 自我对象,
        任务节点* 宿主任务,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        (void)私有_校验任务管理特征目标宿主(
            特征类型,
            枚举_任务管理特征目标宿主::宿主任务治理);
        auto* 特征节点 = 私有_确保宿主任务治理特征节点(
            自我对象,
            宿主任务,
            特征类型,
            调用点 + "/任务虚拟存在");
        if (!特征节点) {
            return false;
        }
        std::uintptr_t 当前值 = 0;
        const auto 新值 = reinterpret_cast<std::uintptr_t>(新指针值);
        if (私有_读取指针特征(特征节点, 当前值) && 当前值 == 新值) {
            return false;
        }
        return 世界树.写入特征_指针(特征节点, 新指针值, now);
    }

    bool 私有_写入宿主任务治理当前值_指针(
        自我类& 自我对象,
        const 词性节点类* 特征类型,
        const void* 新指针值,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        return 私有_写入宿主任务治理当前值_指针(
            自我对象,
            自我对象.获取当前主任务(),
            特征类型,
            新指针值,
            now,
            调用点);
    }
}

bool 任务管理任务模块::写入上位输入镜像(
    自我类& 自我对象,
    枚举_任务管理根层重判结果 根层重判结果,
    枚举_任务管理执行前门控结果 执行前门控结果,
    const std::string& 上层反馈摘要,
    时间戳 now,
    const std::string& 调用点) noexcept
{
    const auto 当前时间 = 私有_归一化时间(now);
    bool 已变化 = false;

    已变化 = 私有_写入宿主任务治理当前值_I64(
        自我对象,
        私有_管理特征_最近根层重判结果(),
        static_cast<I64>(根层重判结果),
        当前时间,
        调用点 + "/最近根层重判结果") || 已变化;

    已变化 = 私有_写入宿主任务治理当前值_I64(
        自我对象,
        私有_管理特征_最近执行前门控结果(),
        static_cast<I64>(执行前门控结果),
        当前时间,
        调用点 + "/最近执行前门控结果") || 已变化;

    (void)上层反馈摘要;

    return 已变化;
}

结构_任务管理上下文 任务管理任务模块::读取任务管理上下文(
    const 自我类& 自我对象,
    时间戳 now) noexcept
{
    结构_任务管理上下文 上下文{};
    上下文.当前时间 = 私有_归一化时间(now);
    上下文.当前主需求 = 自我对象.获取当前主需求();
    上下文.当前管理任务 = 自我对象.获取任务管理任务();
    上下文.当前宿主任务 = 自我对象.获取当前主任务();
    上下文.当前方法 = 自我对象.获取当前主方法();
    上下文.当前步骤 = 私有_读取宿主任务治理任务节点指针特征(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_当前步骤指针(),
        "任务管理任务模块::读取任务管理上下文/当前步骤");
    上下文.最近结果 = 私有_读取宿主任务治理任务节点指针特征(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近结果指针(),
        "任务管理任务模块::读取任务管理上下文/最近结果");
    上下文.当前缺口类型 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理缺口类型>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_当前缺口类型(),
        "任务管理任务模块::读取任务管理上下文/当前缺口类型");
    上下文.当前下一步去向 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理下一步去向>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_当前下一步去向(),
        "任务管理任务模块::读取任务管理上下文/当前下一步去向");
    上下文.当前步骤位类型 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理步骤位类型>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_当前步骤位类型(),
        "任务管理任务模块::读取任务管理上下文/当前步骤位类型");
    上下文.当前步骤语义类型 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理步骤语义类型>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_当前步骤语义类型(),
        "任务管理任务模块::读取任务管理上下文/当前步骤语义类型");
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前步骤可复用(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/当前步骤可复用")) {
            上下文.当前步骤可复用 = 当前值 != 0;
        }
    }
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_最近结果命中当前治理(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/最近结果命中当前治理")) {
            上下文.最近结果命中当前治理 = 当前值 != 0;
        }
    }
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前动作长时间无推进(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/当前动作长时间无推进")) {
            上下文.当前动作长时间无推进 = 当前值 != 0;
        }
    }
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前动作需重试(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/当前动作需重试")) {
            上下文.当前动作需重试 = 当前值 != 0;
        }
    }
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前需等待(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/当前需等待")) {
            上下文.当前需等待 = 当前值 != 0;
        }
    }
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前需收束(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/当前需收束")) {
            上下文.当前需收束 = 当前值 != 0;
        }
    }
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_可继续推进(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/可继续推进")) {
            上下文.可继续推进 = 当前值 != 0;
        }
    }
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前可回接最近结果(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/当前可回接最近结果")) {
            上下文.当前可回接最近结果 = 当前值 != 0;
        }
    }
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_稳定等待态复盘学习(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/稳定等待态复盘学习")) {
            上下文.稳定等待态复盘学习 = 当前值 != 0;
        }
    }
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_业务子任务已完成(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/业务子任务已完成")) {
            上下文.业务子任务已完成 = 当前值 != 0;
        }
    }
    {
        I64 当前值 = 0;
        if (私有_读取宿主任务治理I64特征当前值(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_业务子任务应转执行(),
            当前值,
            "任务管理任务模块::读取任务管理上下文/业务子任务应转执行")) {
            上下文.业务子任务应转执行 = 当前值 != 0;
        }
    }
    上下文.最近根层重判结果 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理根层重判结果>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近根层重判结果(),
        "任务管理任务模块::读取任务管理上下文/最近根层重判结果");
    上下文.最近执行前门控结果 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理执行前门控结果>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近执行前门控结果(),
        "任务管理任务模块::读取任务管理上下文/最近执行前门控结果");
    上下文.最近上层反馈摘要.clear();
    上下文.最近功能域 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理功能域>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近功能域(),
        "任务管理任务模块::读取任务管理上下文/最近功能域");
    上下文.最近总控结果 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理总控结果>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近总控结果(),
        "任务管理任务模块::读取任务管理上下文/最近总控结果");
    上下文.最近反馈类型 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理反馈类型>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_最近反馈类型(),
        "任务管理任务模块::读取任务管理上下文/最近反馈类型");
    私有_按正式主体桥接结构补齐上下文(上下文);
    结构_任务管理主体虚拟存在 主体正式承载{};
    const bool 存在主体正式承载 =
        上下文.当前宿主任务
        && 任务管理任务模块::读取任务管理主体虚拟存在(
            上下文.当前宿主任务,
            &主体正式承载);
    const bool 主体正式承载已有本能缺口 =
        存在主体正式承载
        && 主体正式承载.最近本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义;
    const bool 主体正式承载已有治理态型 =
        存在主体正式承载
        && !主体正式承载.最近治理态型.empty();
    if (上下文.当前主体ID == 0) {
        上下文.当前主体ID = 私有_读取主体桥接U64文本特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前主体ID(),
            "任务管理任务模块::读取任务管理上下文/当前主体ID");
    }
    if (上下文.当前分身ID == 0) {
        上下文.当前分身ID = 私有_读取主体桥接U64文本特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前分身ID(),
            "任务管理任务模块::读取任务管理上下文/当前分身ID");
    }
    if (上下文.当前方法需求位 == 枚举_任务管理方法需求位::未定义
        && !存在主体正式承载) {
        上下文.当前方法需求位 = 私有_读取主体桥接I64枚举特征<枚举_任务管理方法需求位>(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前方法需求位(),
            "任务管理任务模块::读取任务管理上下文/当前方法需求位");
    }
    if (上下文.当前本能能力缺口类型 == 枚举_任务管理本能能力缺口类型::未定义
        && !主体正式承载已有本能缺口) {
        上下文.当前本能能力缺口类型 = 私有_读取主体桥接I64枚举特征<枚举_任务管理本能能力缺口类型>(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前本能能力缺口类型(),
            "任务管理任务模块::读取任务管理上下文/当前本能能力缺口类型");
    }
    if (上下文.当前输入条件主键.empty() && !存在主体正式承载) {
        上下文.当前输入条件主键 = 私有_读取主体桥接反馈摘要特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前输入条件主键(),
            "任务管理任务模块::读取任务管理上下文/当前输入条件主键");
    }
    if (上下文.当前治理态型.empty() && !主体正式承载已有治理态型) {
        上下文.当前治理态型 = 私有_读取主体桥接反馈摘要特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前治理态型(),
            "任务管理任务模块::读取任务管理上下文/当前治理态型");
    }
    if (上下文.当前方法位专项动态标题.empty() && !存在主体正式承载) {
        上下文.当前方法位专项动态标题 = 私有_读取主体桥接反馈摘要特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前方法位专项动态(),
            "任务管理任务模块::读取任务管理上下文/当前方法位专项动态");
    }
    if (上下文.当前最小原语动态标题.empty() && !存在主体正式承载) {
        上下文.当前最小原语动态标题 = 私有_读取主体桥接反馈摘要特征(
            自我对象,
            上下文.当前宿主任务,
            私有_管理特征_当前最小原语动态(),
            "任务管理任务模块::读取任务管理上下文/当前最小原语动态");
    }
    上下文.当前学习承接类型 = 私有_读取宿主任务治理I64枚举特征<枚举_任务管理学习承接类型>(
        自我对象,
        上下文.当前宿主任务,
        私有_管理特征_当前学习承接类型(),
        "任务管理任务模块::读取任务管理上下文/当前学习承接类型");
    上下文.当前学习承接摘要.clear();
    上下文.已触发学习承接 =
        上下文.当前学习承接类型 != 枚举_任务管理学习承接类型::未定义;
    上下文.最近反馈摘要.clear();
    私有_按宿主清洗治理上下文镜像(上下文);
    return 上下文;
}

namespace {
    bool 私有_治理轮返回值(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.已补齐治理任务
            || 结果.已选中宿主任务
            || 结果.已绑定方法挂点
            || 结果.已推进任务状态
            || 结果.已创建步骤节点
            || 结果.已创建结果节点
            || 结果.已创建业务子任务
            || 结果.已创建学习子任务
            || 结果.已创建学习方法头
            || 结果.已创建学习方法条件节点
            || 结果.已创建学习方法结果节点
            || 结果.已确保任务管理主体虚拟存在
            || 结果.已初始化分身继承面
            || 结果.已刷新方法需求位
            || 结果.已评估本能能力缺口
            || 结果.已回并分身增量
            || 结果.已写回目标状态
            || 结果.已生成治理证据;
    }

    bool 私有_M2_提前结束治理轮(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        任务节点* 宿主任务,
        结构_任务管理结果& 结果) noexcept
    {
        (void)私有_记录任务管理方法位专项动态(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/方法位动态/开始",
            结果,
            枚举_动作事件相位::开始运行);
        私有_写回治理稳定字段(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/稳定字段",
            结果);
        (void)私有_记录任务管理方法位专项动态(
            自我对象,
            宿主任务,
            当前时间,
            调用点 + "/方法位动态/完成",
            结果,
            枚举_动作事件相位::完成);
        私有_刷新P0分阶段骨架(自我对象, 当前时间, 结果);
        结果.摘要 = 私有_治理摘要文本(结果);
        任务管理任务模块_治理缓存::缓存最近治理结果(结果);
        return false;
    }

    bool 私有_M2_受理请求阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        私有_刷新P0分阶段骨架(自我对象, 当前时间, 结果);
        结果.最近根层重判结果 = 结果.上下文.最近根层重判结果;
        结果.最近执行前门控结果 = 结果.上下文.最近执行前门控结果;
        结果.最近上层反馈摘要.clear();

        if (!结果.上下文.当前管理任务) {
            (void)自我初始化模块::执行自我初始化(自我对象, 调用点 + "/补齐治理任务", nullptr);
            结果.当前请求 = {};
            私有_刷新P0分阶段骨架(自我对象, 当前时间, 结果);
            结果.已补齐治理任务 = 结果.上下文.当前管理任务 != nullptr;
            结果.最近根层重判结果 = 结果.上下文.最近根层重判结果;
            结果.最近执行前门控结果 = 结果.上下文.最近执行前门控结果;
            结果.最近上层反馈摘要.clear();
        }

        阶段状态.当前管理任务 = 结果.上下文.当前管理任务;
        if (!阶段状态.当前管理任务) {
            结果.上下文.当前管理任务 = nullptr;
            结果.上下文.当前宿主任务 = nullptr;
            结果.上下文.当前方法 = nullptr;
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::筹办,
                枚举_任务管理缺口类型::治理任务缺失,
                枚举_任务管理下一步去向::回到重筹办,
                枚举_任务管理治理状态迁移::回到重筹办,
                枚举_任务管理总控结果::宿主待重筹办,
                枚举_任务管理反馈类型::重筹办提示,
                "任务管理任务缺失");
            return 私有_M2_提前结束治理轮(
                自我对象,
                当前时间,
                调用点 + "/受理请求",
                nullptr,
                结果);
        }

        if (阶段状态.当前管理任务->主信息.来源需求.指针 != 结果.上下文.当前主需求) {
            私有_写入引用(阶段状态.当前管理任务->主信息.来源需求, 结果.上下文.当前主需求);
        }
        (void)私有_迁移任务状态(阶段状态.当前管理任务, 枚举_任务状态::运行中, 当前时间);
        return true;
    }

    bool 私有_M2_宿主锚点确认阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        std::string 锚点裁决摘要{};
        阶段状态.宿主任务 = 私有_选择宿主任务(自我对象, 结果.上下文, &锚点裁决摘要);
        结果.锚点裁决摘要 = std::move(锚点裁决摘要);
        if (!阶段状态.宿主任务) {
            结果.上下文.当前宿主任务 = nullptr;
        }
        else if (阶段状态.宿主任务 != 结果.上下文.当前宿主任务) {
            结果.已选中宿主任务 = 自我对象.设置当前主任务(
                阶段状态.宿主任务,
                当前时间,
                调用点 + "/设置当前宿主任务");
            结果.上下文.当前宿主任务 = 阶段状态.宿主任务;
        }
        私有_按宿主清洗治理上下文镜像(结果.上下文);

        if (!阶段状态.宿主任务) {
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::筹办,
                枚举_任务管理缺口类型::宿主任务缺失,
                枚举_任务管理下一步去向::回到重筹办,
                枚举_任务管理治理状态迁移::回到重筹办,
                枚举_任务管理总控结果::宿主待重筹办,
                枚举_任务管理反馈类型::重筹办提示,
                "未选中宿主任务");
            return 私有_M2_提前结束治理轮(
                自我对象,
                当前时间,
                调用点 + "/宿主锚点确认",
                nullptr,
                结果);
        }

        结果.已确保任务管理主体虚拟存在 =
            任务管理任务模块::确保任务管理主体虚拟存在(
                自我对象,
                阶段状态.宿主任务,
                当前时间,
                &结果.当前主体);
        if (结果.已确保任务管理主体虚拟存在) {
            结果.当前主体ID = 结果.当前主体.主体ID;
            结果.上下文.当前主体ID = 结果.当前主体ID;
        }

        结果.宿主任务旧状态 = 阶段状态.宿主任务->主信息.状态;
        阶段状态.待回接业务子任务 =
            私有_查找待回接的已完成业务子任务(阶段状态.宿主任务, 结果.上下文);
        if (阶段状态.待回接业务子任务) {
            结果.当前子任务节点 = 阶段状态.待回接业务子任务;
        }
        return true;
    }

    void 私有_M2_绑定宿主方法阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        if (!阶段状态.宿主任务
            || 阶段状态.已受上位输入约束
            || !私有_上下文允许普通执行前筹办(结果.上下文)) {
            return;
        }

        方法节点* 当前方法 = 私有_P0_当前方法(结果);
        const bool 已绑定方法 = 私有_绑定宿主任务方法(
            自我对象,
            阶段状态.宿主任务,
            结果,
            当前时间,
            调用点 + "/绑定宿主方法",
            当前方法);
        结果.已绑定方法挂点 = 已绑定方法;
        结果.上下文.当前方法 = 当前方法;

        if (!当前方法 && 阶段状态.宿主任务->主信息.执行方法.指针) {
            当前方法 = reinterpret_cast<方法节点*>(阶段状态.宿主任务->主信息.执行方法.指针);
            结果.上下文.当前方法 = 当前方法;
        }

        if (当前方法 && 当前方法 != 自我对象.获取当前主方法()) {
            (void)自我对象.设置当前主方法(当前方法, 当前时间, 调用点 + "/同步当前主方法");
        }
    }

    void 私有_M2_上位输入约束阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        (void)自我对象;
        (void)当前时间;
        (void)调用点;
        阶段状态.已受上位输入约束 = 私有_应用上位输入约束(结果);
    }

    void 私有_M2_刷新局部可执行判断阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        (void)当前时间;
        if (结果.已确保任务管理主体虚拟存在) {
            结果.已初始化分身继承面 =
                任务管理任务模块::初始化任务管理分身继承面(
                    结果.上下文,
                    结果.当前主体,
                    &结果.当前分身);
            if (结果.已初始化分身继承面) {
                结果.当前分身.宿主任务 = 阶段状态.宿主任务;
                结果.当前分身.当前管理方法 = 私有_P0_当前方法(结果);
                结果.当前分身.当前步骤 = 结果.上下文.当前步骤;
                结果.当前分身.最近结果 = 结果.上下文.最近结果;
                结果.当前分身ID = 结果.当前分身.分身ID;
                结果.上下文.当前分身ID = 结果.当前分身ID;
                (void)私有_形成治理特征面(结果);
                (void)私有_命中方法需求位(结果);
                私有_收口触发信息(结果);
                (void)私有_评估本能能力缺口(自我对象, 结果);
            }
        }
    }

    void 私有_M2_应用允许执行判断阶段(
        时间戳 当前时间,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        阶段状态.桥接补层允许继续 =
            !阶段状态.已受上位输入约束
            && (!结果.已初始化分身继承面
                || 私有_桥接补层裁决(结果));

        if (!阶段状态.桥接补层允许继续) {
            结果.当前写回结果.结构动作摘要 = 私有_P0_最近反馈摘要(结果);
        }

        if (阶段状态.桥接补层允许继续 && !阶段状态.已受上位输入约束 && !私有_P0_当前方法(结果)) {
            if (阶段状态.宿主任务->主信息.状态 != 枚举_任务状态::挂起) {
                结果.已推进任务状态 = 私有_迁移任务状态(阶段状态.宿主任务, 枚举_任务状态::挂起, 当前时间);
            }
            私有_设定治理结论(
                结果,
                枚举_任务管理功能域::筹办,
                枚举_任务管理缺口类型::方法挂点缺失,
                枚举_任务管理下一步去向::绑定方法,
                枚举_任务管理治理状态迁移::进入挂起,
                枚举_任务管理总控结果::宿主已挂起,
                枚举_任务管理反馈类型::重筹办提示,
                "宿主任务缺少方法挂点");
        }
        else if (阶段状态.桥接补层允许继续 && !阶段状态.已受上位输入约束) {
            switch (阶段状态.宿主任务->主信息.状态) {
            case 枚举_任务状态::完成:
            case 枚举_任务状态::失败:
            case 枚举_任务状态::取消:
                私有_设定治理结论(
                    结果,
                    枚举_任务管理功能域::收束,
                    枚举_任务管理缺口类型::任务已终结,
                    枚举_任务管理下一步去向::进入收束,
                    枚举_任务管理治理状态迁移::进入收束,
                    枚举_任务管理总控结果::宿主进入收束,
                    枚举_任务管理反馈类型::收束回执,
                    "宿主任务已终结，进入收束");
                break;
            case 枚举_任务状态::运行中:
                if (私有_业务子任务执行已完成(阶段状态.宿主任务, 结果)) {
                    结果.已推进任务状态 = 私有_迁移任务状态(阶段状态.宿主任务, 枚举_任务状态::完成, 当前时间)
                        || 结果.已推进任务状态;
                    私有_设定治理结论(
                        结果,
                        枚举_任务管理功能域::收束,
                        枚举_任务管理缺口类型::任务已终结,
                        枚举_任务管理下一步去向::进入收束,
                        枚举_任务管理治理状态迁移::进入收束,
                        枚举_任务管理总控结果::宿主进入收束,
                        枚举_任务管理反馈类型::收束回执,
                        "业务子任务已完成业务执行，进入收束");
                }
                else if (阶段状态.待回接业务子任务) {
                    私有_设定治理结论(
                        结果,
                        枚举_任务管理功能域::维护,
                        枚举_任务管理缺口类型::任务运行中,
                        枚举_任务管理下一步去向::保持运行,
                        枚举_任务管理治理状态迁移::保持运行,
                        枚举_任务管理总控结果::宿主保持运行,
                        枚举_任务管理反馈类型::治理推进,
                        "直接业务子任务已完成，回接父任务结果");
                }
                else if (私有_可进入等待分支(阶段状态.宿主任务, 结果)) {
                    私有_设定治理结论(
                        结果,
                        枚举_任务管理功能域::等待,
                        枚举_任务管理缺口类型::无缺口,
                        枚举_任务管理下一步去向::保持等待,
                        枚举_任务管理治理状态迁移::保持等待,
                        枚举_任务管理总控结果::宿主保持等待,
                        枚举_任务管理反馈类型::等待维持,
                        "当前无新增缺口，保持等待");
                }
                else {
                    私有_设定治理结论(
                        结果,
                        枚举_任务管理功能域::维护,
                        枚举_任务管理缺口类型::任务运行中,
                        枚举_任务管理下一步去向::保持运行,
                        枚举_任务管理治理状态迁移::保持运行,
                        枚举_任务管理总控结果::宿主保持运行,
                        枚举_任务管理反馈类型::治理推进,
                        "宿主任务运行中");
                }
                break;
            case 枚举_任务状态::挂起:
                结果.已推进任务状态 = 私有_迁移任务状态(阶段状态.宿主任务, 枚举_任务状态::运行中, 当前时间);
                私有_设定治理结论(
                    结果,
                    枚举_任务管理功能域::纠偏,
                    枚举_任务管理缺口类型::任务待启动,
                    枚举_任务管理下一步去向::进入运行,
                    枚举_任务管理治理状态迁移::进入运行,
                    枚举_任务管理总控结果::宿主已进入运行,
                    枚举_任务管理反馈类型::治理推进,
                    "宿主任务已推进到运行中");
                break;
            case 枚举_任务状态::未定义:
            case 枚举_任务状态::未启动:
            default:
                结果.已推进任务状态 = 私有_迁移任务状态(阶段状态.宿主任务, 枚举_任务状态::运行中, 当前时间);
                私有_设定治理结论(
                    结果,
                    枚举_任务管理功能域::维护,
                    枚举_任务管理缺口类型::任务待启动,
                    枚举_任务管理下一步去向::进入运行,
                    枚举_任务管理治理状态迁移::进入运行,
                    枚举_任务管理总控结果::宿主已进入运行,
                    枚举_任务管理反馈类型::治理推进,
                    "宿主任务已推进到运行中");
                break;
            }
        }

        结果.宿主任务新状态 = 阶段状态.宿主任务->主信息.状态;
        私有_同步学习承接决策(
            结果,
            私有_形成学习承接决策(
                阶段状态.当前管理任务,
                阶段状态.宿主任务,
                结果));
        私有_同步治理上下文镜像(结果);
    }

    void 私有_M2_形成单步决策阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        (void)调用点;
        私有_M2_刷新局部可执行判断阶段(自我对象, 当前时间, 结果, 阶段状态);
        私有_M2_应用允许执行判断阶段(当前时间, 结果, 阶段状态);
    }

    void 私有_M2_形成写回结果阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept
    {
        auto* 当前证据方法 = 私有_P0_当前方法(结果);
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前宿主任务指针(),
            阶段状态.宿主任务,
            私有_任务标题(阶段状态.宿主任务),
            "任务管理_宿主任务切换",
            当前证据方法,
            阶段状态.宿主任务,
            当前时间,
            调用点 + "/证据/宿主任务",
            结果);
        (void)私有_通过执行壳写回指针(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_当前方法指针(),
            当前证据方法,
            私有_方法标题(当前证据方法),
            "任务管理_当前方法挂点",
            当前证据方法,
            阶段状态.宿主任务,
            当前时间,
            调用点 + "/证据/当前方法",
            结果);
        (void)私有_通过执行壳写回I64(
            自我对象,
            枚举_最小原语写回目标宿主::宿主任务治理,
            私有_管理特征_宿主任务状态(),
            static_cast<I64>(结果.宿主任务新状态),
            "任务管理_宿主任务状态迁移",
            当前证据方法,
            阶段状态.宿主任务,
            当前时间,
            调用点 + "/证据/宿主任务状态",
            结果);
    }

    void 私有_M2_组装一步结果阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept
    {
        const bool 调试M10回填 = 调用点.find("M10长期策略驱动回填") != std::string::npos;
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | M2组装-准备完成治理轮收口");
        }
        私有_完成治理轮收口(
            自我对象,
            阶段状态.宿主任务,
            当前时间,
            调用点,
            结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | M2组装-治理轮收口完成");
            项目运行日志("任务管理/M10回填 | M2组装-准备刷新P0分阶段骨架");
        }
        私有_刷新P0分阶段骨架(自我对象, 当前时间, 结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | M2组装-P0分阶段骨架完成");
            项目运行日志("任务管理/M10回填 | M2组装-准备生成治理摘要");
        }
        结果.摘要 = 私有_治理摘要文本(结果);
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | M2组装-治理摘要完成");
        }
        任务管理任务模块_治理缓存::缓存最近治理结果(结果);
    }

    void 私有_M9_装配请求基本骨架(
        const 结构_任务管理上下文& 上下文,
        时间戳 当前时间,
        std::uint32_t 提交序号,
        结构_任务管理请求& 输出) noexcept
    {
        输出.宿主任务 = 上下文.当前宿主任务;
        输出.提交时间 = 当前时间;
        输出.提交序号 = 提交序号;
        输出.上下文 = 上下文;
        输出.请求ID = static_cast<std::uint64_t>(std::hash<std::string>{}(
            std::to_string(当前时间)
            + "|" + std::to_string(提交序号)
            + "|" + (上下文.当前宿主任务 ? 上下文.当前宿主任务->获取主键() : std::string("空宿主"))
            + "|" + (上下文.当前方法 ? 上下文.当前方法->获取主键() : std::string("空方法"))));
    }

    结构_M9请求触发态 私有_M9_消费请求触发态() noexcept
    {
        结构_M9请求触发态 触发态{};
        触发态.命中显式事件 = 私有_尝试消费治理显式事件控制(&触发态.显式事件);
        触发态.命中控制请求 = 私有_尝试消费治理控制请求(&触发态.控制请求);
        触发态.命中恢复重建 = 私有_尝试消费治理恢复重建控制(&触发态.恢复快照);
        return 触发态;
    }

    void 私有_M9_装配请求触发信息(
        结构_任务管理请求& 输出,
        const 结构_M9请求触发态& 触发态) noexcept
    {
        if (触发态.命中恢复重建) {
            输出.触发事件类型 = 枚举_任务管理触发事件类型::恢复重建;
        }
        else if (触发态.命中控制请求) {
            输出.触发事件类型 = 枚举_任务管理触发事件类型::控制请求;
        }
        else if (触发态.命中显式事件) {
            输出.触发事件类型 = 触发态.显式事件.事件类型;
        }
        else {
            输出.触发事件类型 = 枚举_任务管理触发事件类型::普通推进;
        }

        输出.触发来源 =
            触发态.命中控制请求
            ? 枚举_任务管理触发来源::自我线程控制意图
            : (触发态.命中显式事件
            ? 触发态.显式事件.触发来源
            : 枚举_任务管理触发来源::自然运行态);
        输出.来源最小原语位图 = 触发态.命中显式事件 ? 触发态.显式事件.来源最小原语位图 : 0;
        输出.来源主观察特征 = 触发态.显式事件.来源主观察特征;
        输出.影子验证状态 =
            触发态.显式事件.影子验证状态.empty()
            ? std::string("未触发")
            : 触发态.显式事件.影子验证状态;
        输出.允许正式资产提交 = 触发态.显式事件.允许正式资产提交;
        输出.存在控制请求 = 触发态.命中控制请求;
        if (触发态.命中控制请求) {
            输出.当前控制请求 = 触发态.控制请求;
            输出.当前控制承接 = {};
            输出.当前控制承接.控制请求主键 = 触发态.控制请求.请求主键;
            输出.当前控制承接.目标线程主键 = 触发态.控制请求.目标线程主键;
            输出.当前控制承接.目标任务管理任务主键 = 触发态.控制请求.目标任务管理任务主键;
            输出.当前控制承接.目标任务主键 = 触发态.控制请求.目标任务主键;
            输出.当前控制承接.当前控制意图 = 触发态.控制请求.控制意图值;
            输出.当前控制承接.当前响应状态 = 枚举_任务管理控制响应状态::待承接;
            输出.当前控制承接.摘要 = 私有_P0_控制请求摘要(触发态.控制请求);
        }
        输出.触发事件摘要 =
            触发态.命中控制请求
            ? 私有_P0_控制请求摘要(触发态.控制请求)
            : (触发态.命中显式事件 ? 触发态.显式事件.事件摘要 : std::string{});
        输出.当前恢复点类型.clear();
        输出.存在待消费学习反馈 = false;
        输出.存在待消费学习回流 = false;
        输出.存在待消费外部反馈 = false;
        输出.恢复重建摘要.clear();
    }

    void 私有_M9_生成恢复重建请求摘要(
        结构_任务管理请求& 输出,
        const 结构_M9请求触发态& 触发态) noexcept
    {
        if (!触发态.命中恢复重建) {
            return;
        }

        const auto& 恢复快照 = 触发态.恢复快照;
        if (恢复快照.最近触发来源 != 枚举_任务管理触发来源::未定义) {
            输出.触发来源 = 恢复快照.最近触发来源;
        }
        if (输出.来源最小原语位图 == 0 && 恢复快照.最近来源最小原语位图 != 0) {
            输出.来源最小原语位图 = 恢复快照.最近来源最小原语位图;
        }
        if (输出.来源主观察特征.empty()) {
            输出.来源主观察特征 = 恢复快照.最近来源主观察特征;
        }
        输出.影子验证状态 =
            恢复快照.最近影子验证状态.empty()
            ? "未触发"
            : 恢复快照.最近影子验证状态;
        输出.允许正式资产提交 = 恢复快照.允许正式资产提交;
        输出.当前恢复点类型 = 恢复快照.当前恢复点类型;
        输出.存在待消费学习反馈 = 恢复快照.存在待消费学习反馈;
        输出.存在待消费学习回流 = 恢复快照.存在待消费学习回流;
        输出.存在待消费外部反馈 = 恢复快照.存在待消费外部反馈;
        私有_P0_覆盖高阶排序请求(输出.当前高阶排序请求, 恢复快照.当前高阶排序请求);
        私有_P0_覆盖高阶胜出绑定(输出.当前高阶胜出绑定, 恢复快照.当前高阶胜出绑定);
        if (输出.当前需求轴键.empty() && !恢复快照.当前高阶排序请求.需求轴键.empty()) {
            输出.当前需求轴键 = 恢复快照.当前高阶排序请求.需求轴键;
        }

        std::ostringstream 恢复摘要;
        恢复摘要 << "恢复点=" << (恢复快照.当前恢复点类型.empty() ? "未定义" : 恢复快照.当前恢复点类型)
            << " | 学习反馈待消费=" << (恢复快照.存在待消费学习反馈 ? "是" : "否")
            << " | 学习回流待消费=" << (恢复快照.存在待消费学习回流 ? "是" : "否")
            << " | 外部反馈待消费=" << (恢复快照.存在待消费外部反馈 ? "是" : "否")
            << " | 待回判事件=" << 恢复快照.待恢复治理事件数;
        if (!恢复快照.最近学习等待摘要.empty()) {
            恢复摘要 << " | 学习等待=" << 恢复快照.最近学习等待摘要;
        }
        else if (!恢复快照.最近学习承接摘要.empty()) {
            恢复摘要 << " | 学习承接=" << 恢复快照.最近学习承接摘要;
        }
        if (!恢复快照.最近学习反馈摘要.empty()) {
            恢复摘要 << " | 学习反馈=" << 恢复快照.最近学习反馈摘要;
        }
        if (!恢复快照.最近学习回流摘要.empty()) {
            恢复摘要 << " | 兼容回流=" << 恢复快照.最近学习回流摘要;
        }
        if (私有_P0_高阶排序请求有效(恢复快照.当前高阶排序请求)) {
            恢复摘要 << " | 高阶请求=" << 私有_P0_高阶排序请求摘要(恢复快照.当前高阶排序请求);
        }
        if (私有_P0_高阶胜出绑定有效(恢复快照.当前高阶胜出绑定)) {
            恢复摘要 << " | 高阶绑定=" << 私有_P0_高阶胜出绑定摘要(恢复快照.当前高阶胜出绑定);
        }
        if (!恢复快照.待恢复治理事件摘要.empty()) {
            恢复摘要 << " | 待回判摘要=" << 恢复快照.待恢复治理事件摘要;
        }
        输出.恢复重建摘要 = 恢复摘要.str();
        if (输出.触发事件摘要.empty()) {
            输出.触发事件摘要 = 输出.恢复重建摘要;
        }
    }

    void 私有_M9_生成请求补充材料摘要(
        const 结构_任务管理上下文& 上下文,
        结构_任务管理请求& 输出) noexcept
    {
        std::ostringstream 摘要;
        摘要 << "宿主=" << 私有_任务标题(上下文.当前宿主任务)
            << " | 方法=" << 私有_方法标题(上下文.当前方法)
            << " | 事件=" << 私有_触发事件类型文本(输出.触发事件类型)
            << " | 根层=" << 私有_根层重判结果文本(上下文.最近根层重判结果)
            << " | 门控=" << 私有_执行前门控结果文本(上下文.最近执行前门控结果)
            << " | 缺口=" << 私有_缺口文本(上下文.当前缺口类型)
            << " | 去向=" << 私有_去向文本(上下文.当前下一步去向)
            << " | 主体ID=" << 上下文.当前主体ID
            << " | 分身ID=" << 上下文.当前分身ID
            << " | 需求位=" << 私有_方法需求位文本(上下文.当前方法需求位)
            << " | 本能缺口=" << 私有_本能能力缺口文本(上下文.当前本能能力缺口类型)
            << " | 触发来源=" << 私有_触发来源文本(输出.触发来源)
            << " | 主观察=" << (输出.来源主观察特征.empty() ? "空" : 输出.来源主观察特征)
            << " | 影子验证=" << (输出.影子验证状态.empty() ? "未触发" : 输出.影子验证状态)
            << " | 正式提交=" << (输出.允许正式资产提交 ? "允许" : "禁止");
        if (!输出.触发事件摘要.empty()) {
            摘要 << " | mailbox=" << 输出.触发事件摘要;
        }
        if (!输出.当前恢复点类型.empty()) {
            摘要 << " | 恢复点=" << 输出.当前恢复点类型;
        }
        if (输出.存在待消费学习反馈
            || 输出.存在待消费学习回流
            || 输出.存在待消费外部反馈) {
            摘要 << " | 待消费="
                << "学习反馈=" << (输出.存在待消费学习反馈 ? "是" : "否")
                << "/"
                << "学习回流=" << (输出.存在待消费学习回流 ? "是" : "否")
                << "/外部反馈=" << (输出.存在待消费外部反馈 ? "是" : "否");
        }
        if (!输出.恢复重建摘要.empty()) {
            摘要 << " | 恢复重建=" << 输出.恢复重建摘要;
        }
        输出.补充材料摘要.clear();
    }

    std::string 私有_M10_主体长期策略摘要(
        const 结构_任务管理主体虚拟存在& 主体) noexcept
    {
        const auto 需求位索引 = static_cast<std::size_t>(主体.最近学习反馈方法位);
        const auto 需求位学习反馈次数 =
            需求位索引 < std::size(主体.需求位学习反馈次数)
            ? 主体.需求位学习反馈次数[需求位索引]
            : 0U;
        const auto 需求位已使用已完善方法次数 =
            需求位索引 < std::size(主体.需求位已使用已完善方法次数)
            ? 主体.需求位已使用已完善方法次数[需求位索引]
            : 0U;
        const auto 需求位正式资产次数 =
            需求位索引 < std::size(主体.需求位正式资产提交次数)
            ? 主体.需求位正式资产提交次数[需求位索引]
            : 0U;
        std::string 策略结论 = "继续等待更多反馈证据";
        if (!私有_M10_长期策略可靠度允许回填(主体)) {
            策略结论 = "降低长期策略权重，等待新成功证据";
        }
        else if (需求位已使用已完善方法次数 >= 2 || 主体.累计已使用已完善方法次数 >= 3) {
            策略结论 = "优先复用已验证正式方法资产";
        }
        else if (主体.累计已使用已完善方法次数 > 0) {
            策略结论 = "优先沿用最近已完善方法";
        }
        else if (主体.累计学习反馈次数 > 0) {
            策略结论 = "保留学习反馈候选，等待主循环继续复判";
        }

        const bool 最近失败更新更晚 =
            主体.最近长期策略失败时间 != 0
            && 主体.最近长期策略失败时间 >= 主体.最近长期策略成功时间;
        std::ostringstream 摘要;
        摘要 << "需求位=" << 私有_方法需求位文本(主体.最近学习反馈方法位)
            << " | 学习反馈=" << 主体.累计学习反馈次数
            << " | 已使用已完善方法=" << 主体.累计已使用已完善方法次数
            << " | 正式资产=" << 主体.累计正式方法资产提交次数
            << " | 需求位反馈=" << 需求位学习反馈次数
            << " | 需求位已使用已完善方法=" << 需求位已使用已完善方法次数
            << " | 需求位正式资产=" << 需求位正式资产次数
            << " | 稳定复现=" << 主体.累计稳定复现次数
            << " | 价值成立=" << 主体.累计价值成立次数
            << " | 成功=" << 主体.累计长期策略成功次数
            << " | 失败=" << 主体.累计长期策略失败次数
            << " | 权重=" << 主体.当前长期策略权重_Q10000
            << " | 降权=" << 主体.当前长期策略降权级别
            << " | 策略=" << 策略结论;
        if (!主体.最近正式方法标题.empty()) {
            摘要 << " | 最近方法=" << 主体.最近正式方法标题;
        }
        if (!主体.最近长期策略条件签名.empty()) {
            摘要 << " | 条件=" << 主体.最近长期策略条件签名;
        }
        if (!主体.长期策略条件签名范围摘要.empty()) {
            摘要 << " | 条件范围=" << 主体.长期策略条件签名范围摘要;
        }
        if (!主体.最近长期策略业务差额签名.empty()) {
            摘要 << " | 差额=" << 主体.最近长期策略业务差额签名;
        }
        if (!主体.长期策略业务差额范围摘要.empty()) {
            摘要 << " | 差额范围=" << 主体.长期策略业务差额范围摘要;
        }
        if (最近失败更新更晚 && !主体.最近长期策略失效原因.empty()) {
            摘要 << " | 最近失效=" << 主体.最近长期策略失效原因;
        }
        return 摘要.str();
    }

    void 私有_M10_回写长期策略可靠度(
        任务节点* 宿主任务,
        时间戳 当前时间,
        结构_任务管理结果& 结果) noexcept
    {
        if (!宿主任务) {
            return;
        }

        结构_任务管理主体虚拟存在 主体 = 结果.当前主体;
        if (主体.主体ID == 0) {
            if (!任务管理任务模块::读取任务管理主体虚拟存在(
                reinterpret_cast<任务类::节点类*>(宿主任务),
                &主体)) {
                return;
            }
        }

        const bool 当前轮命中长期策略 =
            结果.当前方法来源 == 枚举_任务管理方法来源::主体长期策略回填
            || 私有_M10_当前方法命中主体长期策略(结果);
        if (!当前轮命中长期策略 || 主体.累计正式方法资产提交次数 == 0) {
            return;
        }

        const auto 条件签名 = 私有_M10_当前条件签名(结果);
        const auto 业务差额签名 = 私有_M10_业务差额签名(宿主任务, 结果);
        主体.最近长期策略条件签名 = 条件签名;
        主体.最近长期策略业务差额签名 = 业务差额签名;
        私有_M10_合并长期策略范围签名(主体.长期策略条件签名范围摘要, 条件签名);
        私有_M10_合并长期策略范围签名(主体.长期策略业务差额范围摘要, 业务差额签名);

        if (私有_M10_当前轮形成有效长期策略推进(结果)) {
            主体.累计长期策略成功次数 += 1;
            主体.最近长期策略成功时间 = 当前时间;
        }
        else {
            主体.累计长期策略失败次数 += 1;
            主体.最近长期策略失败时间 = 当前时间;
            主体.最近长期策略失效原因 = 私有_M10_当前轮长期策略失效原因(结果);
        }

        私有_M10_刷新长期策略权重(主体);
        主体.最近长期策略摘要 = 私有_M10_主体长期策略摘要(主体);

        {
            std::scoped_lock 锁(私有_任务管理主体表锁);
            私有_任务管理主体表[主体.主体ID] = 主体;
        }

        结果.当前主体 = 主体;
        if (!主体.最近长期策略摘要.empty()) {
            结果.当前写回结果.主体回并摘要 = 主体.最近长期策略摘要;
            结果.当前一步结果.主体回并摘要 = 主体.最近长期策略摘要;
        }
        if (!主体.最近长期策略失效原因.empty()
            && 主体.最近长期策略失败时间 >= 主体.最近长期策略成功时间) {
            私有_追加最近反馈片段(
                结果,
                "长期策略失效原因=" + 主体.最近长期策略失效原因);
        }
    }

    bool 私有_M10_沉淀正式学习反馈到主体(
        自我类& 自我对象,
        const 结构_学习方法资产提交输入& 输入,
        const 结构_学习方法资产提交结果& 资产结果,
        时间戳 当前时间,
        结构_任务管理主体虚拟存在* 输出主体,
        std::string* 输出摘要) noexcept
    {
        if (!输入.来源宿主任务 || !资产结果.已写回正式方法资产) {
            return false;
        }

        结构_任务管理主体虚拟存在 主体{};
        if (!任务管理任务模块::确保任务管理主体虚拟存在(
            自我对象,
            输入.来源宿主任务,
            当前时间,
            &主体)) {
            return false;
        }

        const auto 需求位索引 = static_cast<std::size_t>(输入.来源方法需求位);
        主体.最近学习反馈时间 = 当前时间;
        主体.最近学习回流时间 = 当前时间;
        主体.最近学习反馈方法位 =
            输入.方法服务目标 != 枚举_任务管理方法需求位::未定义
            ? 输入.方法服务目标
            : 输入.来源方法需求位;
        主体.最近学习回流方法位 = 输入.来源方法需求位;
        主体.最近主方法需求位 = 输入.来源方法需求位;
        主体.最近宿主任务 = 输入.来源宿主任务;
        主体.最近管理方法 =
            资产结果.当前学习方法节点
            ? 资产结果.当前学习方法节点
            : 输入.来源方法;
        主体.最近原因摘要 =
            !输入.学习承接摘要.empty()
            ? 输入.学习承接摘要
            : 资产结果.摘要;
        主体.最近学习反馈摘要 =
            !输入.学习反馈摘要.empty()
            ? 输入.学习反馈摘要
            : (
                !输入.回流目标摘要.empty()
                ? 输入.回流目标摘要
                : 资产结果.摘要
            );
        主体.最近学习回流摘要 =
            !输入.回流目标摘要.empty()
            ? 输入.回流目标摘要
            : 资产结果.摘要;
        主体.最近正式方法标题 = 资产结果.正式方法标题;
        主体.最近长期策略条件签名 =
            !输入.来源主观察特征.empty()
            ? 输入.来源主观察特征
            : 主体.最近输入条件主键;
        {
            结构_任务管理结果 临时结果{};
            主体.最近长期策略业务差额签名 =
                私有_M10_业务差额签名(
                    reinterpret_cast<任务节点*>(输入.来源宿主任务),
                    临时结果);
        }
        私有_M10_合并长期策略范围签名(
            主体.长期策略条件签名范围摘要,
            主体.最近长期策略条件签名);
        私有_M10_合并长期策略范围签名(
            主体.长期策略业务差额范围摘要,
            主体.最近长期策略业务差额签名);
        主体.累计学习反馈次数 += 1;
        主体.累计学习回流次数 += 1;
        主体.累计已使用已完善方法次数 += 1;
        主体.累计正式方法资产提交次数 += 1;
        主体.累计稳定复现次数 += 1;
        if (资产结果.已创建学习方法结果节点) {
            主体.累计价值成立次数 += 1;
        }
        if (需求位索引 < std::size(主体.需求位学习反馈次数)) {
            主体.需求位学习反馈次数[需求位索引] += 1;
        }
        if (需求位索引 < std::size(主体.需求位学习回流次数)) {
            主体.需求位学习回流次数[需求位索引] += 1;
            主体.需求位已使用已完善方法次数[需求位索引] += 1;
            主体.需求位正式资产提交次数[需求位索引] += 1;
            主体.需求位验证证据次数[需求位索引] += 1;
            主体.需求位升格证据次数[需求位索引] += 1;
        }
        私有_M10_刷新长期策略权重(主体);
        主体.最近长期策略摘要 = 私有_M10_主体长期策略摘要(主体);

        {
            std::scoped_lock 锁(私有_任务管理主体表锁);
            私有_任务管理主体表[主体.主体ID] = 主体;
        }

        if (输出主体) {
            *输出主体 = 主体;
        }
        if (输出摘要) {
            *输出摘要 = 主体.最近长期策略摘要;
        }
        return true;
    }

    bool 私有_执行筹办模块(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        if (!私有_M2_受理请求阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态)) {
            return false;
        }
        if (!私有_M2_宿主锚点确认阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态)) {
            return false;
        }
        私有_M2_绑定宿主方法阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态);
        return true;
    }

    void 私有_执行可执行判断模块(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        (void)调用点;
        私有_M2_刷新局部可执行判断阶段(自我对象, 当前时间, 结果, 阶段状态);
    }

    void 私有_执行允许执行判断模块(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        私有_M2_上位输入约束阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态);
        私有_M2_应用允许执行判断阶段(当前时间, 结果, 阶段状态);
    }

    void 私有_执行执行模块(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept
    {
        const bool 桥接补层允许继续 = 阶段状态.桥接补层允许继续;
        const bool 允许进入执行后段 = 桥接补层允许继续 && !阶段状态.已受上位输入约束;
        const bool 允许进入派生与学习阶段 = !阶段状态.已受上位输入约束;
        bool 允许业务派生 = 桥接补层允许继续 && 允许进入派生与学习阶段;

        if (允许进入执行后段) {
            允许业务派生 =
                私有_M9_执行后段治理动作阶段(
                自我对象,
                当前时间,
                调用点,
                结果,
                阶段状态)
                && 允许业务派生;
        }
        if (允许进入派生与学习阶段) {
            私有_M9_执行学习承接与业务派生阶段(
                自我对象,
                当前时间,
                调用点,
                结果,
                阶段状态,
                允许业务派生);
        }
    }

    void 私有_执行反馈模块(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept
    {
        私有_M2_形成写回结果阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态);
        私有_M2_组装一步结果阶段(自我对象, 当前时间, 调用点, 结果, 阶段状态);
    }

    bool 私有_M9_执行前段治理阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        结构_M2治理阶段状态& 阶段状态) noexcept
    {
        if (!私有_执行筹办模块(自我对象, 当前时间, 调用点, 结果, 阶段状态)) {
            return false;
        }
        私有_执行可执行判断模块(自我对象, 当前时间, 调用点, 结果, 阶段状态);
        私有_执行允许执行判断模块(自我对象, 当前时间, 调用点, 结果, 阶段状态);
        return true;
    }

    void 私有_追加最近反馈片段(
        结构_任务管理结果& 结果,
        const std::string& 片段) noexcept
    {
        if (片段.empty()) {
            return;
        }
        if (!结果.最近反馈摘要.empty()) {
            结果.最近反馈摘要 += "；";
        }
        结果.最近反馈摘要 += 片段;
    }

    bool 私有_M9_事件轮禁止业务扩张(const 结构_任务管理结果& 结果) noexcept
    {
        return 结果.当前请求.触发事件类型 == 枚举_任务管理触发事件类型::学习反馈
            || 结果.当前请求.触发事件类型 == 枚举_任务管理触发事件类型::学习回流
            || 结果.当前请求.触发事件类型 == 枚举_任务管理触发事件类型::外部反馈
            || 结果.当前请求.触发事件类型 == 枚举_任务管理触发事件类型::恢复重建
            || 结果.当前下一步去向 == 枚举_任务管理下一步去向::保持等待
            || 结果.当前下一步去向 == 枚举_任务管理下一步去向::进入收束;
    }

    bool 私有_M9_步骤语义允许直接执行(枚举_任务管理步骤语义类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_任务管理步骤语义类型::治理步骤:
        case 枚举_任务管理步骤语义类型::业务推进步骤:
            return true;
        default:
            return false;
        }
    }

    bool 私有_M9_结果节点已是当前步骤实际结果(
        const 任务节点* 当前步骤,
        const 任务节点* 最新结果) noexcept
    {
        if (!当前步骤 || !最新结果) {
            return false;
        }
        return 最新结果->主信息.节点种类 == 枚举_任务节点种类::结果节点
            && 最新结果->主信息.结果角色 == 枚举_任务结果角色::实际结果
            && 私有_任务状态已终结(最新结果->主信息.状态)
            && 最新结果->父 == 当前步骤;
    }

    bool 私有_M9_执行当前任务步骤(
        任务节点* 宿主任务,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果) noexcept
    {
        auto* 当前方法 = 私有_P0_当前方法(结果);
        auto* 当前步骤 = 私有_P0_当前步骤节点(结果);
        auto* 最新结果 = 私有_P0_最新结果节点(结果);
        const auto 当前步骤语义类型 = 私有_P0_当前步骤语义类型(结果);
        if (!宿主任务
            || !当前方法
            || !私有_M9_步骤语义允许直接执行(当前步骤语义类型)) {
            return true;
        }

        if (结果.已命中最近结果
            || 私有_M9_结果节点已是当前步骤实际结果(当前步骤, 最新结果)) {
            return true;
        }

        自我_执行桥类 执行桥{};
        执行桥.启动(1);
        const auto 执行结果 = 执行任务步骤_最小_v0(
            执行桥,
            宿主任务,
            当前方法,
            当前时间,
            0,
            false,
            调用点 + "/执行步骤");

        if (!执行结果.有效) {
            结果.宿主任务新状态 = 枚举_任务状态::失败;
            return false;
        }

        if (执行结果.当前步骤节点) {
            结果.当前步骤节点 = 执行结果.当前步骤节点;
            结果.上下文.当前步骤 = 执行结果.当前步骤节点;
            私有_P0_覆盖非空指针(结果.当前写回结果.当前步骤节点, 执行结果.当前步骤节点);
            私有_P0_覆盖非空指针(结果.当前一步结果.当前步骤节点, 执行结果.当前步骤节点);
        }
        if (执行结果.最近结果节点) {
            结果.最新结果节点 = 执行结果.最近结果节点;
            结果.上下文.最近结果 = 执行结果.最近结果节点;
            私有_P0_覆盖非空指针(结果.当前写回结果.最新结果节点, 执行结果.最近结果节点);
            私有_P0_覆盖非空指针(结果.当前一步结果.最新结果节点, 执行结果.最近结果节点);
        }
        if (执行结果.当前方法) {
            结果.当前一步结果.当前方法节点 = 执行结果.当前方法;
        }
        if (宿主任务) {
            结果.宿主任务新状态 = 宿主任务->主信息.状态;
        }
        return 执行结果.方法执行结果.成功;
    }

    bool 私有_M9_执行后段治理动作阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态) noexcept
    {
        auto* 宿主任务 = 阶段状态.宿主任务;
        auto* 待回接业务子任务 = 阶段状态.待回接业务子任务;
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        const auto 当前功能域 = 私有_P0_当前功能域(结果);
        const bool 允许推进步骤结果子链 =
            当前下一步去向 != 枚举_任务管理下一步去向::进入收束
            || 当前功能域 == 枚举_任务管理功能域::等待;
        if (允许推进步骤结果子链) {
            (void)私有_推进步骤结果子链(
                自我对象,
                宿主任务,
                结果,
                当前时间,
                调用点 + "/步骤结果子链");
        }
        const bool 当前步骤执行成功 = 私有_M9_执行当前任务步骤(
            宿主任务,
            当前时间,
            调用点,
            结果);

        if (!结果.当前步骤节点) {
            结果.当前步骤节点 = 结果.上下文.当前步骤;
        }
        if (!结果.最新结果节点) {
            结果.最新结果节点 = 结果.上下文.最近结果;
        }
        if (结果.已创建补条件子任务) {
            私有_追加最近反馈片段(结果, "已创建补条件子任务=" + 私有_任务标题(结果.当前子任务节点));
        }
        if (结果.已复用补条件子任务) {
            私有_追加最近反馈片段(结果, "已复用补条件子任务=" + 私有_任务标题(结果.当前子任务节点));
        }

        const auto 当前方法来源 = 私有_P0_当前方法来源(结果);
        const auto 当前步骤位类型 = 私有_P0_当前步骤位类型(结果);
        if (当前方法来源 != 枚举_任务管理方法来源::未定义) {
            私有_追加最近反馈片段(结果, "方法来源=" + std::string(私有_方法来源文本(当前方法来源)));
        }
        if (当前步骤位类型 != 枚举_任务管理步骤位类型::未定义) {
            私有_追加最近反馈片段(结果, "步骤位=" + std::string(私有_步骤位类型文本(当前步骤位类型)));
        }

        (void)私有_写回任务目标状态(
            自我对象,
            宿主任务,
            结果,
            当前时间,
            调用点 + "/目标状态");

        (void)私有_写回任务结果状态(
            自我对象,
            宿主任务,
            结果,
            当前时间,
            调用点 + "/结果状态");

        私有_整理任务双面摘要(宿主任务, 结果);
        if (待回接业务子任务) {
            if (结果.宿主目标结果已对齐) {
                私有_设定治理结论(
                    结果,
                    枚举_任务管理功能域::等待,
                    枚举_任务管理缺口类型::无缺口,
                    枚举_任务管理下一步去向::保持等待,
                    枚举_任务管理治理状态迁移::保持等待,
                    枚举_任务管理总控结果::宿主保持等待,
                    枚举_任务管理反馈类型::等待维持,
                    "直接业务子任务已完成，父任务结果已对齐，进入等待");
            }
            else if (结果.宿主目标结果待推进) {
                私有_设定治理结论(
                    结果,
                    枚举_任务管理功能域::维护,
                    枚举_任务管理缺口类型::任务运行中,
                    枚举_任务管理下一步去向::保持运行,
                    枚举_任务管理治理状态迁移::保持运行,
                    枚举_任务管理总控结果::宿主保持运行,
                    枚举_任务管理反馈类型::治理推进,
                    "直接业务子任务已完成，但父任务仍有差额，继续派生下一段业务任务");
            }
            私有_同步治理上下文镜像(结果);
        }
        私有_整理后果治理(结果, 待回接业务子任务 != nullptr);
        私有_同步治理上下文镜像(结果);
        if (结果.已由目标差额驱动 && 结果.当前步骤语义类型 == 枚举_任务管理步骤语义类型::治理步骤) {
            结果.当前步骤语义类型 = 枚举_任务管理步骤语义类型::业务推进步骤;
            私有_P0_刷新步骤与学习结构字段(结果);
        }
        return 当前步骤执行成功;
    }

    void 私有_M9_执行学习承接与业务派生阶段(
        自我类& 自我对象,
        时间戳 当前时间,
        const std::string& 调用点,
        结构_任务管理结果& 结果,
        const 结构_M2治理阶段状态& 阶段状态,
        bool 允许业务派生) noexcept
    {
        auto* 当前管理任务 = 阶段状态.当前管理任务;
        auto* 宿主任务 = 阶段状态.宿主任务;
        auto* 当前方法 = 私有_P0_当前方法(结果);
        const auto 当前下一步去向 = 私有_P0_当前下一步去向(结果);
        if (允许业务派生) {
            const bool 事件轮禁止业务扩张 = 私有_M9_事件轮禁止业务扩张(结果);
            const bool 长期策略驱动业务派生 = 私有_M10_长期策略驱动业务子任务派生(结果);
            const auto 业务子任务复用键 = 私有_业务子任务键(宿主任务, 结果);
            if (长期策略驱动业务派生) {
                私有_追加最近反馈片段(结果, "长期策略驱动业务子任务=是");
                私有_追加最近反馈片段(结果, "业务子任务复用键=" + 业务子任务复用键);
            }
            if (事件轮禁止业务扩张) {
                私有_追加最近反馈片段(结果, "事件轮禁止继续扩张业务子任务");
                if (长期策略驱动业务派生) {
                    私有_追加最近反馈片段(结果, "业务子任务决策=禁止扩张");
                }
            }
            else {
                auto* 业务子任务 = 私有_创建或复用业务子任务头(
                    自我对象,
                    宿主任务,
                    结果,
                    当前时间,
                    调用点 + "/业务子任务");
                if (业务子任务 && (结果.已创建业务子任务 || 结果.已复用业务子任务)) {
                    结果.当前写回结果.当前业务子任务 = 业务子任务;
                }
                if (长期策略驱动业务派生) {
                    if (结果.已复用业务子任务) {
                        私有_追加最近反馈片段(结果, "业务子任务决策=复用");
                    }
                    else if (结果.已创建业务子任务) {
                        私有_追加最近反馈片段(结果, "业务子任务决策=新建");
                    }
                    else {
                        私有_追加最近反馈片段(结果, "业务子任务决策=未命中");
                    }
                }
            }
        }

        if (当前方法) {
            私有_P2_补齐运行方法证据索引(
                宿主任务,
                当前方法,
                结果,
                调用点 + "/运行方法证据沉淀");
        }

        const auto 学习承接决策 = 私有_形成学习承接决策(
            当前管理任务,
            宿主任务,
            结果);
        私有_同步学习承接决策(结果, 学习承接决策);
        auto* 学习子任务 = 学习承接决策.是否触发学习
            ? 私有_创建或复用学习子任务头(
                自我对象,
                当前管理任务,
                宿主任务,
                学习承接决策,
                结果,
                当前时间,
                调用点 + "/学习子任务")
            : nullptr;
        auto* 学习任务兜底方法 = 学习承接决策.是否触发学习
            ? 私有_写回学习方法骨架(
                自我对象,
                宿主任务,
                学习子任务,
                true,
                结果,
                当前时间,
                调用点 + "/学习方法兜底")
            : nullptr;
        auto* 当前轮学习方法 = 学习承接决策.是否触发学习
            ? 私有_写回学习方法骨架(
                自我对象,
                宿主任务,
                学习子任务,
                false,
                结果,
                当前时间,
                调用点 + "/学习方法候选")
            : nullptr;
        const bool 当前轮学习方法已可执行 = 私有_学习方法可直接进入学习(当前轮学习方法);
        const bool 学习任务兜底方法已可执行 = 私有_学习方法可直接进入学习(学习任务兜底方法);
        const bool 已切换到兜底可执行方法 =
            !当前轮学习方法已可执行
            && 学习任务兜底方法已可执行
            && 学习任务兜底方法
            && 学习任务兜底方法 != 当前轮学习方法;
        auto* 学习方法 = 当前轮学习方法已可执行
            ? 当前轮学习方法
            : (学习任务兜底方法已可执行 ? 学习任务兜底方法 : (当前轮学习方法 ? 当前轮学习方法 : 学习任务兜底方法));
        std::ostringstream 学习方法选项摘要;
        if (学习承接决策.是否触发学习) {
            学习方法选项摘要 << "兜底=" << 私有_方法标题(学习任务兜底方法)
                << "(可执行=" << (学习任务兜底方法已可执行 ? "是" : "否") << ")"
                << " | 当前候选=" << 私有_方法标题(当前轮学习方法)
                << "(可执行=" << (当前轮学习方法已可执行 ? "是" : "否") << ")"
                << " | 当前选中=" << 私有_方法标题(学习方法)
                << " | 默认消费=" << 私有_学习反馈消费策略文本(学习承接决策.默认消费策略);
        }
        if (学习承接决策.是否触发学习) {
            (void)私有_同步学习任务实体(
                自我对象,
                宿主任务,
                学习子任务,
                学习任务兜底方法,
                学习方法,
                学习方法选项摘要.str(),
                学习承接决策,
                结果,
                当前时间);
        }
        结果.当前写回结果.当前学习子任务 = 学习子任务;
        结果.当前写回结果.当前学习方法 = 学习方法;
        I64 待学习方法数量 = 学习任务模块::统计活跃学习任务数量();
        if (待学习方法数量 <= 0) {
            待学习方法数量 = 私有_统计待学习方法数量(当前管理任务);
        }
        (void)自我对象.更新待学习方法数量(
            待学习方法数量,
            当前时间,
            调用点 + "/学习计数");
        (void)自我对象.更新尝试学习状态(
            学习任务模块::统计活跃学习任务数量() > 0,
            当前时间,
            调用点 + "/学习状态");
        if (!结果.目标结果差额摘要.empty()) {
            私有_追加最近反馈片段(结果, 结果.目标结果差额摘要);
        }
        if (结果.已创建业务子任务) {
            私有_追加最近反馈片段(结果, "已创建业务子任务=" + 私有_任务标题(结果.当前子任务节点));
        }
        if (结果.已复用业务子任务) {
            私有_追加最近反馈片段(结果, "已复用业务子任务=" + 私有_任务标题(结果.当前子任务节点));
        }
        if (结果.已创建学习子任务) {
            私有_追加最近反馈片段(结果, "已创建学习子任务=" + 私有_任务标题(学习子任务));
        }
        if (结果.已复用学习子任务) {
            私有_追加最近反馈片段(结果, "已复用学习子任务=" + 私有_任务标题(学习子任务));
        }
        const auto 当前缺口类型 = 私有_P0_当前缺口类型(结果);
        if ((结果.已创建学习子任务 || 结果.已复用学习子任务)
            && 当前缺口类型 == 枚举_任务管理缺口类型::无缺口
            && 当前下一步去向 == 枚举_任务管理下一步去向::保持等待) {
            私有_追加最近反馈片段(
                结果,
                学习承接决策.学习类型 == 枚举_任务管理学习承接类型::广泛学习
                    ? "稳定等待态已触发广泛学习复盘"
                    : "稳定等待态已触发方法复盘学习");
        }
        if ((结果.已创建学习子任务 || 结果.已复用学习子任务) && 学习承接决策.补原语或治理本能) {
            私有_追加最近反馈片段(结果, "当前学习通道=本能补齐/治理本能组合");
        }
        if (结果.已创建学习方法头) {
            私有_追加最近反馈片段(结果, "已创建学习方法头=" + 私有_方法标题(学习方法));
        }
        if (结果.已复用学习方法头) {
            私有_追加最近反馈片段(结果, "已复用学习方法头=" + 私有_方法标题(学习方法));
        }
        if (学习承接决策.是否触发学习 && 学习方法) {
            私有_追加最近反馈片段(
                结果,
                std::string("学习当前选中方法=") + 私有_方法标题(学习方法)
                + (私有_学习方法可直接进入学习(学习方法) ? "（已可执行）" : "（待补执行骨架）"));
        }
        if (学习承接决策.是否触发学习 && 已切换到兜底可执行方法) {
            私有_追加最近反馈片段(
                结果,
                "学习方法选择=当前候选不可执行，已切换到兜底尝试学习方法");
        }
        if (结果.已创建学习方法条件节点) {
            私有_追加最近反馈片段(结果, "已创建学习方法条件节点");
        }
        if (结果.已创建学习方法结果节点) {
            私有_追加最近反馈片段(结果, "已创建学习方法结果节点");
        }
    }
}

bool 任务管理任务模块::设置治理显式事件控制(
    const 结构_任务管理显式事件控制& 控制) noexcept
{
    std::scoped_lock 锁(私有_治理显式事件控制锁);
    私有_治理显式事件控制 = 控制;
    私有_治理显式事件控制.启用 =
        控制.启用
        && 控制.事件类型 != 枚举_任务管理触发事件类型::未定义;
    return true;
}

void 任务管理任务模块::清除治理显式事件控制() noexcept
{
    std::scoped_lock 锁(私有_治理显式事件控制锁);
    私有_治理显式事件控制 = {};
}

bool 任务管理任务模块::设置治理控制请求(
    const 结构_任务管理控制请求& 请求) noexcept
{
    if (!私有_P0_控制请求有效(请求)) {
        return false;
    }

    {
        std::scoped_lock 锁(私有_治理控制请求锁);
        私有_治理控制请求 = 请求;
    }

    结构_任务管理控制承接记录 初始承接{};
    初始承接.控制请求主键 = 请求.请求主键;
    初始承接.目标线程主键 = 请求.目标线程主键;
    初始承接.目标任务管理任务主键 = 请求.目标任务管理任务主键;
    初始承接.目标任务主键 = 请求.目标任务主键;
    初始承接.当前控制意图 = 请求.控制意图值;
    初始承接.当前响应状态 = 枚举_任务管理控制响应状态::待承接;
    初始承接.摘要 = 私有_P0_控制请求摘要(请求);
    {
        std::scoped_lock 锁(私有_最近治理控制承接锁);
        私有_最近治理控制承接 = 初始承接;
    }
    return true;
}

void 任务管理任务模块::清除治理控制请求() noexcept
{
    std::scoped_lock 锁(私有_治理控制请求锁);
    私有_治理控制请求 = {};
}

bool 任务管理任务模块::写入治理控制响应(
    const 结构_任务管理控制响应摘要& 响应) noexcept
{
    if (响应.请求主键.empty()
        && 响应.控制意图值 == 枚举_任务管理控制意图值::未定义
        && 响应.响应状态 == 枚举_任务管理控制响应状态::未定义) {
        return false;
    }

    std::scoped_lock 锁(私有_最近治理控制承接锁);
    私有_P0_覆盖非空文本(私有_最近治理控制承接.控制请求主键, 响应.请求主键);
    私有_P0_覆盖非空文本(私有_最近治理控制承接.对应执行尝试主键, 响应.执行尝试主键);
    if (响应.控制意图值 != 枚举_任务管理控制意图值::未定义) {
        私有_最近治理控制承接.当前控制意图 = 响应.控制意图值;
    }
    if (响应.响应状态 != 枚举_任务管理控制响应状态::未定义) {
        私有_最近治理控制承接.当前响应状态 = 响应.响应状态;
    }
    if (响应.是否已下发) {
        私有_最近治理控制承接.是否已下发 = true;
    }
    if (响应.是否已响应) {
        私有_最近治理控制承接.是否已响应 = true;
    }
    if (响应.是否已执行完成) {
        私有_最近治理控制承接.是否已执行完成 = true;
    }
    if (响应.是否超时) {
        私有_最近治理控制承接.是否超时 = true;
    }
    if (响应.最近回报时间 != 0) {
        私有_最近治理控制承接.最近回报时间 = 响应.最近回报时间;
    }
    私有_P0_覆盖非空文本(私有_最近治理控制承接.拒绝原因, 响应.拒绝原因);
    私有_P0_覆盖非空文本(私有_最近治理控制承接.摘要, 响应.响应摘要);
    return true;
}

bool 任务管理任务模块::读取最近治理控制承接记录(
    结构_任务管理控制承接记录* 输出) noexcept
{
    std::scoped_lock 锁(私有_最近治理控制承接锁);
    if (!私有_P0_控制承接有效(私有_最近治理控制承接)) {
        return false;
    }
    if (输出) {
        *输出 = 私有_最近治理控制承接;
    }
    return true;
}

bool 任务管理任务模块::设置治理恢复重建控制(
    const 结构_治理恢复快照& 快照,
    const std::string& 调用点) noexcept
{
    (void)调用点;
    std::scoped_lock 锁(私有_治理恢复重建控制锁);
    私有_治理恢复重建控制 = {};
    私有_治理恢复重建控制.启用 = true;
    私有_治理恢复重建控制.快照 = 快照;
    return true;
}

void 任务管理任务模块::清除治理恢复重建控制() noexcept
{
    std::scoped_lock 锁(私有_治理恢复重建控制锁);
    私有_治理恢复重建控制 = {};
}

bool 任务管理任务模块::构造任务管理请求(
    const 自我类& 自我对象,
    时间戳 now,
    结构_任务管理请求* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    const auto 当前时间 = 私有_归一化时间(now);
    const auto 上下文 = 读取任务管理上下文(自我对象, 当前时间);

    std::uint32_t 提交序号 = 0;
    {
        std::scoped_lock 锁(私有_任务管理请求序号锁);
        提交序号 = ++私有_任务管理请求序号;
    }

    私有_M9_装配请求基本骨架(上下文, 当前时间, 提交序号, *输出);
    const auto 触发态 = 私有_M9_消费请求触发态();
    私有_M9_装配请求触发信息(*输出, 触发态);
    私有_M9_生成恢复重建请求摘要(*输出, 触发态);
    私有_M9_生成请求补充材料摘要(上下文, *输出);
    私有_P0_刷新请求原语锚点(*输出);
    auto& 可写自我 = const_cast<自我类&>(自我对象);
    (void)私有_P1_刷新任务治理二次特征证据(
        可写自我,
        *输出,
        "任务管理任务模块::构造任务管理请求");
    (void)私有_P0_生成高阶排序请求(*输出, 输出->当前高阶排序请求);
    return 形成任务管理原语判定汇总(*输出, &输出->当前原语判定汇总);
}

bool 任务管理任务模块::形成任务管理原语判定汇总(
    const 结构_任务管理请求& 请求,
    结构_原语判定汇总* 输出) noexcept
{
    if (!输出) {
        return false;
    }
    return 私有_P0_生成原语判定汇总(请求, *输出);
}

bool 任务管理任务模块::选择任务管理下一步(
    const 结构_任务管理请求& 请求,
    const 结构_原语判定汇总& 汇总,
    结构_任务管理单步决策* 输出) noexcept
{
    if (!输出) {
        return false;
    }
    return 私有_P0_选择下一步(请求, 汇总, *输出);
}

bool 任务管理任务模块::形成任务管理单步决策(
    const 结构_任务管理请求& 请求,
    结构_任务管理单步决策* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    *输出 = {};

    结构_任务管理分身继承面 临时分身{};
    临时分身.宿主任务 = 请求.上下文.当前宿主任务;
    临时分身.当前步骤 = 请求.上下文.当前步骤;
    临时分身.最近结果 = 请求.上下文.最近结果;
    临时分身.当前管理方法 = 请求.上下文.当前方法;
    临时分身.本轮输入条件主键 = 私有_主体化输入条件主键(请求.上下文, 临时分身);
    临时分身.本轮结果主键 = 私有_主体化结果主键(临时分身, 请求.上下文);

    auto 原语汇总 = 请求.当前原语判定汇总;
    if (原语汇总.明细.empty() && !形成任务管理原语判定汇总(请求, &原语汇总)) {
        return false;
    }
    (void)私有_P0_生成高阶排序请求(请求, 输出->当前高阶排序请求);
    (void)私有_P0_生成高阶排序结果(请求, 输出->当前高阶排序结果);

    输出->当前功能域 = 请求.上下文.最近功能域;
    输出->当前缺口类型 = 请求.上下文.当前缺口类型;
    输出->当前下一步去向 = 请求.上下文.当前下一步去向;
    输出->当前治理状态迁移 = 枚举_任务管理治理状态迁移::未定义;
    输出->当前方法来源 = 枚举_任务管理方法来源::未定义;
    输出->当前方法需求位 = 私有_推导任务管理方法需求位(请求.上下文, 临时分身);
    输出->当前本能能力缺口类型 = 私有_P0_推导本能能力缺口(请求.上下文, 输出->当前方法需求位);
    输出->当前触发来源 = 请求.触发来源;
    输出->当前步骤位类型 =
        请求.上下文.当前步骤位类型 != 枚举_任务管理步骤位类型::未定义
        ? 请求.上下文.当前步骤位类型
        : 私有_P0_推导步骤位类型(请求.上下文);
    输出->当前步骤语义类型 =
        请求.上下文.当前步骤语义类型 != 枚举_任务管理步骤语义类型::未定义
        ? 请求.上下文.当前步骤语义类型
        : 私有_P0_推导步骤语义类型(请求.上下文, 输出->当前步骤位类型);
    输出->当前后果治理 =
        请求.上下文.当前下一步去向 == 枚举_任务管理下一步去向::进入收束
        ? 枚举_任务管理后果治理::进入收束
        : (
            请求.上下文.当前下一步去向 == 枚举_任务管理下一步去向::保持等待
            ? 枚举_任务管理后果治理::保持等待
            : 枚举_任务管理后果治理::无额外后果
        );
    输出->来源最小原语位图 = 请求.来源最小原语位图;
    输出->来源主观察特征 = 请求.来源主观察特征;
    输出->影子验证状态 = 请求.影子验证状态;
    输出->允许正式资产提交 = 请求.允许正式资产提交;
    输出->当前治理二次特征证据 = 请求.当前治理二次特征证据;
    输出->当前原语判定汇总 = 原语汇总;
    输出->当前控制承接 = 请求.当前控制承接;

    结构_任务管理单步决策 原语建议{};
    if (选择任务管理下一步(请求, 原语汇总, &原语建议)) {
        私有_P0_覆盖高阶排序请求(输出->当前高阶排序请求, 原语建议.当前高阶排序请求);
        私有_P0_覆盖高阶排序结果(输出->当前高阶排序结果, 原语建议.当前高阶排序结果);
        私有_P0_覆盖高阶胜出绑定(输出->当前高阶胜出绑定, 原语建议.当前高阶胜出绑定);
        私有_P0_覆盖控制承接(输出->当前控制承接, 原语建议.当前控制承接);
        if (输出->当前治理状态迁移 == 枚举_任务管理治理状态迁移::未定义) {
            输出->当前治理状态迁移 = 原语建议.当前治理状态迁移;
        }
        if (原语汇总.出现冲突
            || 原语汇总.结果符合
            || 原语汇总.需要升阶
            || 输出->当前后果治理 == 枚举_任务管理后果治理::未定义) {
            输出->当前后果治理 = 原语建议.当前后果治理;
        }
    }

    std::ostringstream 摘要;
    摘要 << "功能域=" << 私有_功能域文本(输出->当前功能域)
        << " | 缺口=" << 私有_缺口文本(输出->当前缺口类型)
        << " | 去向=" << 私有_去向文本(输出->当前下一步去向)
        << " | 需求位=" << 私有_方法需求位文本(输出->当前方法需求位)
        << " | 本能缺口=" << 私有_本能能力缺口文本(输出->当前本能能力缺口类型)
        << " | 触发来源=" << 私有_触发来源文本(输出->当前触发来源)
        << " | 本轮原语命中=" << (输出->来源最小原语位图 == 0 ? std::string("空") : 私有_原语位图摘要(输出->来源最小原语位图))
        << " | 证据=" << (输出->当前治理二次特征证据.摘要.empty() ? std::string("空") : 输出->当前治理二次特征证据.摘要)
        << " | 原语判定="
        << "条件=" << (原语汇总.条件满足 ? "满足" : "未满足")
        << "/方向=" << (原语汇总.方向一致 ? "同向" : "未明")
        << "/结果=" << (原语汇总.结果符合 ? "命中" : "未命中")
        << "/冲突=" << (原语汇总.出现冲突 ? "是" : "否")
        << "/因果=" << (原语汇总.因果稳定 ? "稳定" : "未稳")
        << "/升阶=" << (原语汇总.需要升阶 ? "是" : "否")
        << " | 综合满足度=" << 原语汇总.综合满足度_Q10000
        << " | 综合置信度=" << 原语汇总.综合置信度_Q10000
        << " | 需求轴=" << (请求.当前需求轴键.empty() ? "空" : 请求.当前需求轴键)
        << " | 升阶候选=" << 私有_P0_升阶候选摘要(请求)
        << " | 高阶请求=" << 私有_P0_高阶排序请求摘要(输出->当前高阶排序请求)
        << " | 高阶排序=" << 私有_P0_高阶排序结果摘要(输出->当前高阶排序结果)
        << " | 高阶绑定=" << 私有_P0_高阶胜出绑定摘要(输出->当前高阶胜出绑定)
        << " | 控制承接=" << (私有_P0_控制承接有效(输出->当前控制承接) ? 私有_P0_控制承接摘要(输出->当前控制承接) : std::string("空"))
        << " | 主观察=" << (输出->来源主观察特征.empty() ? "空" : 输出->来源主观察特征)
        << " | 影子验证=" << (输出->影子验证状态.empty() ? "未触发" : 输出->影子验证状态)
        << " | 正式提交=" << (输出->允许正式资产提交 ? "允许" : "禁止");
    if (!原语汇总.升阶原因.empty()) {
        摘要 << " | 升阶原因=" << 原语汇总.升阶原因;
    }
    输出->决策摘要.clear();
    return true;
}

bool 任务管理任务模块::形成任务管理单步决策(
    const 结构_任务管理结果& 来源结果,
    结构_任务管理单步决策* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    *输出 = {};
    私有_P0_覆盖决策字段(*输出, 来源结果.当前单步决策);
    私有_P0_覆盖兼容决策字段(*输出, 来源结果);

    输出->当前功能域 = 私有_P0_阶段当前功能域(来源结果);
    输出->当前缺口类型 = 私有_P0_阶段当前缺口类型(来源结果);
    输出->当前下一步去向 = 私有_P0_阶段当前下一步去向(来源结果);
    输出->当前治理状态迁移 = 私有_P0_当前治理状态迁移(来源结果);
    输出->当前方法来源 = 私有_P0_当前方法来源(来源结果);
    输出->当前方法需求位 = 私有_P0_当前方法需求位(来源结果);
    输出->当前本能能力缺口类型 = 私有_P0_当前本能缺口(来源结果);
    输出->当前触发来源 = 私有_P0_选用非默认值(
        来源结果.当前一步结果.当前触发来源,
        来源结果.当前单步决策.当前触发来源,
        私有_P0_选用非默认值(
            来源结果.当前触发来源,
            来源结果.当前请求.触发来源,
            枚举_任务管理触发来源::未定义,
            枚举_任务管理触发来源::自然运行态),
        枚举_任务管理触发来源::未定义);
    输出->当前步骤位类型 = 私有_P0_当前步骤位类型(来源结果);
    输出->当前步骤语义类型 = 私有_P0_当前步骤语义类型(来源结果);
    输出->当前后果治理 = 私有_P0_当前后果治理(来源结果);
    输出->来源最小原语位图 =
        来源结果.来源最小原语位图 != 0
        ? 来源结果.来源最小原语位图
        : 来源结果.当前单步决策.来源最小原语位图;
    输出->来源主观察特征 = 私有_P0_选用非空文本(
        来源结果.来源主观察特征,
        来源结果.当前一步结果.来源主观察特征,
        私有_P0_选用非空文本(
            来源结果.当前单步决策.来源主观察特征,
            来源结果.当前请求.来源主观察特征,
            std::string{}));
    输出->影子验证状态 = 私有_P0_选用非空文本(
        来源结果.当前影子验证状态,
        来源结果.当前一步结果.影子验证状态,
        私有_P0_选用非空文本(
            来源结果.当前单步决策.影子验证状态,
            来源结果.当前请求.影子验证状态,
            std::string{"未触发"}));
    输出->允许正式资产提交 =
        来源结果.允许正式资产提交
        || 来源结果.当前一步结果.允许正式资产提交
        || 来源结果.当前单步决策.允许正式资产提交
        || 来源结果.当前请求.允许正式资产提交;
    私有_P1_覆盖任务治理二次特征证据(输出->当前治理二次特征证据, 来源结果.当前治理二次特征证据);
    if (!输出->当前治理二次特征证据.有证据) {
        私有_P1_覆盖任务治理二次特征证据(输出->当前治理二次特征证据, 来源结果.当前一步结果.当前治理二次特征证据);
    }
    if (!输出->当前治理二次特征证据.有证据) {
        私有_P1_覆盖任务治理二次特征证据(输出->当前治理二次特征证据, 来源结果.当前单步决策.当前治理二次特征证据);
    }
    if (!输出->当前治理二次特征证据.有证据) {
        私有_P1_覆盖任务治理二次特征证据(输出->当前治理二次特征证据, 来源结果.当前请求.当前治理二次特征证据);
    }
    私有_P0_覆盖原语判定汇总(输出->当前原语判定汇总, 来源结果.当前原语判定汇总);
    if (输出->当前原语判定汇总.明细.empty()) {
        私有_P0_覆盖原语判定汇总(输出->当前原语判定汇总, 来源结果.当前一步结果.当前原语判定汇总);
    }
    if (输出->当前原语判定汇总.明细.empty()) {
        私有_P0_覆盖原语判定汇总(输出->当前原语判定汇总, 来源结果.当前单步决策.当前原语判定汇总);
    }
    if (输出->当前原语判定汇总.明细.empty()) {
        私有_P0_覆盖原语判定汇总(输出->当前原语判定汇总, 来源结果.当前请求.当前原语判定汇总);
    }
    if (输出->当前原语判定汇总.明细.empty()) {
        (void)形成任务管理原语判定汇总(来源结果.当前请求, &输出->当前原语判定汇总);
    }
    私有_P0_覆盖高阶排序请求(输出->当前高阶排序请求, 来源结果.当前高阶排序请求);
    if (!私有_P0_高阶排序请求有效(输出->当前高阶排序请求)) {
        私有_P0_覆盖高阶排序请求(输出->当前高阶排序请求, 来源结果.当前一步结果.当前高阶排序请求);
    }
    if (!私有_P0_高阶排序请求有效(输出->当前高阶排序请求)) {
        私有_P0_覆盖高阶排序请求(输出->当前高阶排序请求, 来源结果.当前单步决策.当前高阶排序请求);
    }
    if (!私有_P0_高阶排序请求有效(输出->当前高阶排序请求)) {
        私有_P0_覆盖高阶排序请求(输出->当前高阶排序请求, 来源结果.当前请求.当前高阶排序请求);
    }
    if (!私有_P0_高阶排序请求有效(输出->当前高阶排序请求)) {
        (void)私有_P0_生成高阶排序请求(来源结果.当前请求, 输出->当前高阶排序请求);
    }
    私有_P0_覆盖高阶排序结果(输出->当前高阶排序结果, 来源结果.当前高阶排序结果);
    if (!私有_P0_高阶排序结果有效(输出->当前高阶排序结果)) {
        私有_P0_覆盖高阶排序结果(输出->当前高阶排序结果, 来源结果.当前一步结果.当前高阶排序结果);
    }
    if (!私有_P0_高阶排序结果有效(输出->当前高阶排序结果)) {
        私有_P0_覆盖高阶排序结果(输出->当前高阶排序结果, 来源结果.当前单步决策.当前高阶排序结果);
    }
    if (!私有_P0_高阶排序结果有效(输出->当前高阶排序结果)) {
        (void)私有_P0_生成高阶排序结果(来源结果.当前请求, 输出->当前高阶排序结果);
    }
    私有_P0_覆盖高阶胜出绑定(输出->当前高阶胜出绑定, 来源结果.当前高阶胜出绑定);
    if (!私有_P0_高阶胜出绑定有效(输出->当前高阶胜出绑定)) {
        私有_P0_覆盖高阶胜出绑定(输出->当前高阶胜出绑定, 来源结果.当前一步结果.当前高阶胜出绑定);
    }
    if (!私有_P0_高阶胜出绑定有效(输出->当前高阶胜出绑定)) {
        私有_P0_覆盖高阶胜出绑定(输出->当前高阶胜出绑定, 来源结果.当前单步决策.当前高阶胜出绑定);
    }
    if (!私有_P0_高阶胜出绑定有效(输出->当前高阶胜出绑定)) {
        私有_P0_覆盖高阶胜出绑定(输出->当前高阶胜出绑定, 来源结果.当前请求.当前高阶胜出绑定);
    }
    if (!私有_P0_高阶胜出绑定有效(输出->当前高阶胜出绑定)) {
        (void)私有_P0_解析高阶胜出绑定(
            来源结果.当前请求,
            输出->当前高阶排序结果,
            输出->当前高阶胜出绑定);
    }

    if (输出->决策摘要.empty()) {
        std::ostringstream 摘要;
        摘要 << "功能域=" << 私有_功能域文本(输出->当前功能域)
            << " | 缺口=" << 私有_缺口文本(输出->当前缺口类型)
            << " | 去向=" << 私有_去向文本(输出->当前下一步去向)
            << " | 需求位=" << 私有_方法需求位文本(输出->当前方法需求位)
            << " | 本能缺口=" << 私有_本能能力缺口文本(输出->当前本能能力缺口类型)
            << " | 触发来源=" << 私有_触发来源文本(输出->当前触发来源)
            << " | 本轮原语命中=" << (输出->来源最小原语位图 == 0 ? std::string("空") : 私有_原语位图摘要(输出->来源最小原语位图))
            << " | 证据=" << (输出->当前治理二次特征证据.摘要.empty() ? "空" : 输出->当前治理二次特征证据.摘要)
            << " | 主观察=" << (输出->来源主观察特征.empty() ? "空" : 输出->来源主观察特征)
            << " | 影子验证=" << (输出->影子验证状态.empty() ? "未触发" : 输出->影子验证状态)
            << " | 高阶请求=" << 私有_P0_高阶排序请求摘要(输出->当前高阶排序请求)
            << " | 高阶排序=" << 私有_P0_高阶排序结果摘要(输出->当前高阶排序结果)
            << " | 高阶绑定=" << 私有_P0_高阶胜出绑定摘要(输出->当前高阶胜出绑定)
            << " | 正式提交=" << (输出->允许正式资产提交 ? "允许" : "禁止");
        (void)摘要;
        输出->决策摘要.clear();
    }
    return true;
}

bool 任务管理任务模块::形成任务管理写回结果(
    自我类&,
    const 结构_任务管理请求& 请求,
    const 结构_任务管理单步决策& 决策,
    结构_任务管理写回结果* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    输出->当前步骤节点 = 请求.上下文.当前步骤;
    输出->最新结果节点 = 请求.上下文.最近结果;
    输出->当前业务子任务 = nullptr;
    输出->当前补条件子任务 = nullptr;
    输出->当前学习子任务 = nullptr;
    输出->当前学习方法 = nullptr;
    输出->宿主目标状态 = 请求.上下文.当前宿主任务 ? 请求.上下文.当前宿主任务->主信息.目标状态.指针 : nullptr;
    输出->宿主结果状态 = 请求.上下文.当前宿主任务 ? 请求.上下文.当前宿主任务->主信息.结果状态信息.指针 : nullptr;
    输出->双面摘要 = {};
    输出->结构动作摘要 =
        "桥接补层写回="
        + 私有_去向文本(决策.当前下一步去向)
        + " | 需求位="
        + 私有_方法需求位文本(决策.当前方法需求位)
        + " | 本能缺口="
        + 私有_本能能力缺口文本(决策.当前本能能力缺口类型);
    输出->主体回并摘要 = {};
    return true;
}

bool 任务管理任务模块::形成任务管理写回结果(
    const 结构_任务管理结果& 来源结果,
    结构_任务管理写回结果* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    *输出 = {};
    私有_P0_覆盖写回字段(*输出, 来源结果.当前写回结果);
    私有_P0_覆盖兼容写回字段(*输出, 来源结果);
    私有_P0_覆盖非空指针(输出->当前步骤节点, 私有_P0_当前步骤节点(来源结果));
    私有_P0_覆盖非空指针(输出->最新结果节点, 私有_P0_最新结果节点(来源结果));
    私有_P0_覆盖非空指针(输出->当前学习方法, 来源结果.当前学习方法节点);
    私有_P0_覆盖非空指针(输出->宿主目标状态, 来源结果.宿主目标状态);
    私有_P0_覆盖非空指针(输出->宿主结果状态, 来源结果.宿主结果状态);
    私有_P0_覆盖非空文本(输出->双面摘要, 私有_P0_双面摘要(来源结果));
    if (输出->结构动作摘要.empty()) {
        输出->结构动作摘要 = 私有_P0_阶段最近反馈摘要(来源结果);
    }
    return true;
}

bool 任务管理任务模块::组装任务管理一步结果(
    const 结构_任务管理请求& 请求,
    const 结构_任务管理单步决策& 决策,
    const 结构_任务管理写回结果& 写回,
    结构_任务管理一步结果* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    *输出 = {};

    输出->宿主任务 = 请求.宿主任务;
    输出->当前主体ID = 请求.上下文.当前主体ID;
    输出->当前分身ID = 请求.上下文.当前分身ID;
    输出->当前功能域 = 决策.当前功能域;
    输出->当前缺口类型 = 决策.当前缺口类型;
    输出->当前下一步去向 = 决策.当前下一步去向;
    输出->当前治理状态迁移 = 决策.当前治理状态迁移;
    输出->当前方法需求位 = 决策.当前方法需求位;
    输出->当前本能能力缺口类型 = 决策.当前本能能力缺口类型;
    输出->当前触发来源 = 决策.当前触发来源;
    输出->当前步骤节点 = 写回.当前步骤节点;
    输出->当前方法节点 = 请求.上下文.当前方法;
    输出->最新结果节点 = 写回.最新结果节点;
    输出->最近总控结果 = 请求.上下文.最近总控结果;
    输出->最近反馈类型 = 请求.上下文.最近反馈类型;
    输出->当前输入条件主键 = 请求.上下文.当前输入条件主键;
    输出->当前治理态型 = 请求.上下文.当前治理态型;
    输出->来源最小原语位图 = 决策.来源最小原语位图;
    输出->来源主观察特征 = 决策.来源主观察特征;
    输出->影子验证状态 = 决策.影子验证状态;
    输出->允许正式资产提交 = 决策.允许正式资产提交;
    输出->当前治理二次特征证据 = 决策.当前治理二次特征证据;
    输出->当前原语判定汇总 = 决策.当前原语判定汇总;
    输出->当前高阶排序请求 = 决策.当前高阶排序请求;
    if (!私有_P0_高阶排序请求有效(输出->当前高阶排序请求)) {
        (void)私有_P0_生成高阶排序请求(请求, 输出->当前高阶排序请求);
    }
    输出->当前高阶排序结果 = 决策.当前高阶排序结果;
    if (!私有_P0_高阶排序结果有效(输出->当前高阶排序结果)) {
        (void)私有_P0_生成高阶排序结果(请求, 输出->当前高阶排序结果);
    }
    输出->当前高阶胜出绑定 = 决策.当前高阶胜出绑定;
    if (!私有_P0_高阶胜出绑定有效(输出->当前高阶胜出绑定)) {
        (void)私有_P0_解析高阶胜出绑定(
            请求,
            输出->当前高阶排序结果,
            输出->当前高阶胜出绑定);
    }
    输出->当前控制承接 = 决策.当前控制承接;
    if (输出->当前高阶胜出绑定.已绑定执行方法
        && 输出->当前高阶胜出绑定.胜出方法节点) {
        输出->当前方法节点 = 输出->当前高阶胜出绑定.胜出方法节点;
    }
    输出->已触发学习承接 =
        写回.当前学习子任务 != nullptr
        || 写回.当前学习方法 != nullptr
        || 请求.上下文.已触发学习承接;
    输出->当前学习承接类型 = 输出->已触发学习承接
        ? 请求.上下文.当前学习承接类型
        : 枚举_任务管理学习承接类型::未定义;
    输出->当前学习为本能补齐 =
        (私有_本能缺口属于原语或治理补齐(决策.当前本能能力缺口类型))
        && 输出->已触发学习承接;
    if (输出->已触发学习承接) {
        输出->学习承接摘要.clear();
    }
    输出->主体回并摘要 = 写回.主体回并摘要;
    输出->最近反馈摘要.clear();
    return true;
}

bool 任务管理任务模块::组装任务管理一步结果(
    const 结构_任务管理结果& 来源结果,
    结构_任务管理一步结果* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    *输出 = {};
    私有_P0_覆盖一步结果字段(*输出, 来源结果.当前一步结果);
    私有_P0_覆盖兼容一步结果字段(*输出, 来源结果);

    私有_P0_覆盖非空指针(输出->宿主任务, 私有_P0_当前宿主任务(来源结果));
    私有_P0_覆盖非空指针(输出->当前方法节点, 私有_P0_当前方法(来源结果));
    私有_P0_覆盖非空指针(输出->当前步骤节点, 私有_P0_当前步骤节点(来源结果));
    私有_P0_覆盖非空指针(输出->最新结果节点, 私有_P0_最新结果节点(来源结果));
    if (输出->当前主体ID == 0) {
        输出->当前主体ID = 来源结果.当前主体ID;
    }
    if (输出->当前分身ID == 0) {
        输出->当前分身ID = 来源结果.当前分身ID;
    }
    输出->当前功能域 = 私有_P0_阶段当前功能域(来源结果);
    输出->当前缺口类型 = 私有_P0_阶段当前缺口类型(来源结果);
    输出->当前下一步去向 = 私有_P0_阶段当前下一步去向(来源结果);
    输出->当前治理状态迁移 = 私有_P0_当前治理状态迁移(来源结果);
    输出->当前方法需求位 = 私有_P0_当前方法需求位(来源结果);
    输出->当前本能能力缺口类型 = 私有_P0_当前本能缺口(来源结果);
    输出->当前触发来源 = 私有_P0_选用非默认值(
        来源结果.当前一步结果.当前触发来源,
        来源结果.当前单步决策.当前触发来源,
        私有_P0_选用非默认值(
            来源结果.当前触发来源,
            来源结果.当前请求.触发来源,
            枚举_任务管理触发来源::未定义,
            枚举_任务管理触发来源::自然运行态),
        枚举_任务管理触发来源::未定义);
    输出->最近总控结果 = 私有_P0_阶段最近总控结果(来源结果);
    输出->最近反馈类型 = 私有_P0_阶段最近反馈类型(来源结果);
    输出->当前输入条件主键 = 私有_P0_选用非空文本(
        来源结果.当前一步结果.当前输入条件主键,
        来源结果.上下文.当前输入条件主键,
        来源结果.当前请求.上下文.当前输入条件主键);
    输出->当前治理态型 = 私有_P0_选用非空文本(
        来源结果.当前一步结果.当前治理态型,
        来源结果.上下文.当前治理态型,
        来源结果.当前请求.上下文.当前治理态型);
    if (输出->来源最小原语位图 == 0) {
        输出->来源最小原语位图 =
            来源结果.来源最小原语位图 != 0
            ? 来源结果.来源最小原语位图
            : 来源结果.当前单步决策.来源最小原语位图;
    }
    私有_P0_覆盖非空文本(输出->来源主观察特征, 来源结果.来源主观察特征);
    私有_P0_覆盖非空文本(输出->影子验证状态, 来源结果.当前影子验证状态);
    if (!输出->允许正式资产提交) {
        输出->允许正式资产提交 =
            来源结果.允许正式资产提交
            || 来源结果.当前单步决策.允许正式资产提交
            || 来源结果.当前请求.允许正式资产提交;
    }
    私有_P1_覆盖任务治理二次特征证据(输出->当前治理二次特征证据, 来源结果.当前治理二次特征证据);
    if (!输出->当前治理二次特征证据.有证据) {
        私有_P1_覆盖任务治理二次特征证据(输出->当前治理二次特征证据, 来源结果.当前单步决策.当前治理二次特征证据);
    }
    if (!输出->当前治理二次特征证据.有证据) {
        私有_P1_覆盖任务治理二次特征证据(输出->当前治理二次特征证据, 来源结果.当前请求.当前治理二次特征证据);
    }
    私有_P0_覆盖原语判定汇总(输出->当前原语判定汇总, 来源结果.当前原语判定汇总);
    if (输出->当前原语判定汇总.明细.empty()) {
        私有_P0_覆盖原语判定汇总(输出->当前原语判定汇总, 来源结果.当前单步决策.当前原语判定汇总);
    }
    if (输出->当前原语判定汇总.明细.empty()) {
        私有_P0_覆盖原语判定汇总(输出->当前原语判定汇总, 来源结果.当前请求.当前原语判定汇总);
    }
    if (输出->当前原语判定汇总.明细.empty()) {
        (void)形成任务管理原语判定汇总(来源结果.当前请求, &输出->当前原语判定汇总);
    }
    私有_P0_覆盖高阶排序请求(输出->当前高阶排序请求, 来源结果.当前高阶排序请求);
    if (!私有_P0_高阶排序请求有效(输出->当前高阶排序请求)) {
        私有_P0_覆盖高阶排序请求(输出->当前高阶排序请求, 来源结果.当前单步决策.当前高阶排序请求);
    }
    if (!私有_P0_高阶排序请求有效(输出->当前高阶排序请求)) {
        私有_P0_覆盖高阶排序请求(输出->当前高阶排序请求, 来源结果.当前请求.当前高阶排序请求);
    }
    if (!私有_P0_高阶排序请求有效(输出->当前高阶排序请求)) {
        (void)私有_P0_生成高阶排序请求(来源结果.当前请求, 输出->当前高阶排序请求);
    }
    私有_P0_覆盖高阶排序结果(输出->当前高阶排序结果, 来源结果.当前高阶排序结果);
    if (!私有_P0_高阶排序结果有效(输出->当前高阶排序结果)) {
        私有_P0_覆盖高阶排序结果(输出->当前高阶排序结果, 来源结果.当前单步决策.当前高阶排序结果);
    }
    if (!私有_P0_高阶排序结果有效(输出->当前高阶排序结果)) {
        (void)私有_P0_生成高阶排序结果(来源结果.当前请求, 输出->当前高阶排序结果);
    }
    if (来源结果.当前学习承接决策.是否触发学习) {
        输出->已触发学习承接 = true;
    }
    私有_P0_覆盖非默认值(
        输出->当前学习承接类型,
        来源结果.当前学习承接决策.学习类型,
        枚举_任务管理学习承接类型::未定义);
    if (来源结果.当前学习承接决策.补原语或治理本能) {
        输出->当前学习为本能补齐 = true;
    }
    输出->学习承接摘要.clear();
    私有_P0_覆盖非空文本(输出->主体回并摘要, 来源结果.当前写回结果.主体回并摘要);
    输出->最近反馈摘要.clear();
    return true;
}

namespace {

std::uint64_t 私有_运行包主键_按指针(const void* 指针) noexcept
{
    return static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(指针));
}

std::uint64_t 私有_运行包主键_按请求(
    const 结构_任务管理请求& 请求,
    const 结构_任务管理写回结果& 写回) noexcept
{
    if (请求.请求ID != 0) {
        return 请求.请求ID;
    }
    if (写回.当前步骤节点) {
        return 私有_运行包主键_按指针(写回.当前步骤节点);
    }
    if (请求.宿主任务) {
        return 私有_运行包主键_按指针(请求.宿主任务)
            ^ (static_cast<std::uint64_t>(请求.提交序号) << 1);
    }
    return (static_cast<std::uint64_t>(请求.提交时间) << 16)
        ^ static_cast<std::uint64_t>(请求.提交序号);
}

任务运行包::枚举_需求大类 私有_映射运行包需求大类(
    const 枚举_任务管理缺口类型 缺口类型) noexcept
{
    switch (缺口类型) {
    case 枚举_任务管理缺口类型::上位等待约束:
    case 枚举_任务管理缺口类型::上位收束约束:
    case 枚举_任务管理缺口类型::上位停止约束:
        return 任务运行包::枚举_需求大类::安全;
    default:
        return 任务运行包::枚举_需求大类::服务;
    }
}

任务运行包::枚举_动作类型 私有_映射运行包动作类型(
    const 枚举_任务管理下一步去向 下一步去向) noexcept
{
    switch (下一步去向) {
    case 枚举_任务管理下一步去向::进入收束:
        return 任务运行包::枚举_动作类型::收束;
    case 枚举_任务管理下一步去向::保持等待:
        return 任务运行包::枚举_动作类型::预警;
    case 枚举_任务管理下一步去向::回到重筹办:
        return 任务运行包::枚举_动作类型::解释;
    case 枚举_任务管理下一步去向::绑定方法:
    case 枚举_任务管理下一步去向::进入运行:
    case 枚举_任务管理下一步去向::保持运行:
    default:
        return 任务运行包::枚举_动作类型::建议;
    }
}

std::int64_t 私有_映射运行包方法置信度(
    const 枚举_任务管理方法来源 方法来源) noexcept
{
    switch (方法来源) {
    case 枚举_任务管理方法来源::主体长期策略回填:
        return 8500;
    case 枚举_任务管理方法来源::条件节点筛选通过:
        return 8000;
    case 枚举_任务管理方法来源::已挂方法复用:
    case 枚举_任务管理方法来源::当前主方法回填:
        return 7500;
    case 枚举_任务管理方法来源::方法树首节点候选:
        return 6500;
    case 枚举_任务管理方法来源::最近结果回看:
        return 6000;
    case 枚举_任务管理方法来源::默认本能兜底:
        return 4500;
    default:
        return 5000;
    }
}

std::int64_t 私有_映射运行包安全监测等级(
    const 枚举_任务管理后果治理 后果治理) noexcept
{
    switch (后果治理) {
    case 枚举_任务管理后果治理::进入收束:
        return 3;
    case 枚举_任务管理后果治理::保持等待:
        return 2;
    default:
        return 1;
    }
}

std::string 私有_构建运行包差额说明(
    const 结构_任务管理请求& 请求,
    const 结构_任务管理单步决策& 决策) noexcept
{
    (void)请求;
    (void)决策;
    return {};
}

void 私有_运行包追加方法候选(
    任务运行包::结构_一步治理任务包& 任务包,
    方法节点* 方法节点,
    const std::int64_t 置信度,
    const std::string& 命中原因,
    const bool 设为选定) noexcept
{
    if (!方法节点) {
        return;
    }

    const auto 方法主键 = 私有_运行包主键_按指针(方法节点);
    auto 已有 = std::find_if(
        任务包.候选方法集.begin(),
        任务包.候选方法集.end(),
        [&](const 任务运行包::结构_方法候选项& 候选) noexcept {
            return 候选.方法主键 == 方法主键;
        });

    if (已有 == 任务包.候选方法集.end()) {
        任务运行包::结构_方法候选项 候选{};
        候选.方法主键 = 方法主键;
        候选.置信度 = 置信度;
        候选.命中原因 = 命中原因;
        任务包.候选方法集.push_back(std::move(候选));
        已有 = std::prev(任务包.候选方法集.end());
    }
    else {
        if (置信度 > 已有->置信度) {
            已有->置信度 = 置信度;
        }
        if (!命中原因.empty()) {
            if (已有->命中原因.empty()) {
                已有->命中原因 = 命中原因;
            }
            else if (已有->命中原因.find(命中原因) == std::string::npos) {
                已有->命中原因 += " | " + 命中原因;
            }
        }
    }

    if (设为选定) {
        任务包.选定方法 = *已有;
    }
}

} // namespace

bool 任务管理任务模块::生成一步治理任务包(
    const 结构_任务管理请求& 请求,
    const 结构_任务管理单步决策& 决策,
    const 结构_任务管理写回结果& 写回,
    任务运行包::结构_一步治理任务包* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    auto& 任务包 = *输出;
    任务包 = {};

    任务包.包头.包主键 = 私有_运行包主键_按请求(请求, 写回);
    任务包.包头.线程主键 = 请求.上下文.当前分身ID;
    任务包.包头.轮次主键 = 请求.请求ID != 0 ? 请求.请求ID : 任务包.包头.包主键;
    任务包.包头.宿主存在主键 = 请求.上下文.当前主体ID;
    任务包.包头.父包主键 = 私有_运行包主键_按指针(请求.宿主任务);
    任务包.包头.创建时间 = 请求.提交时间;
    任务包.包头.追踪签名 =
        任务包.包头.轮次主键 ^ (static_cast<std::uint64_t>(请求.提交序号) << 1);

    任务包.来源需求主键 = 私有_运行包主键_按指针(请求.上下文.当前主需求);
    任务包.来源任务主键 = 私有_运行包主键_按指针(
        请求.上下文.当前管理任务 ? 请求.上下文.当前管理任务 : 请求.宿主任务);
    任务包.来源需求大类 = 私有_映射运行包需求大类(决策.当前缺口类型);
    任务包.本轮要缩小的差额说明.clear();

    任务包.动作类型 = 私有_映射运行包动作类型(决策.当前下一步去向);
    任务包.任务语义键 = !请求.上下文.当前治理态型.empty()
        ? 请求.上下文.当前治理态型
        : 私有_步骤语义类型文本(决策.当前步骤语义类型);
    if (任务包.任务语义键.empty()) {
        任务包.任务语义键 = 私有_功能域文本(决策.当前功能域);
    }
    任务包.目标主体主键 = 请求.上下文.当前主体ID;
    任务包.目标场景主键 = 0;
    任务包.完成条件 = 私有_去向文本(决策.当前下一步去向);
    任务包.预期结果.clear();

    任务包.授权约束.授权等级 = 请求.允许正式资产提交 ? 1 : 0;
    任务包.授权约束.资源配额 = 1;
    任务包.授权约束.时间窗_ms = 0;
    任务包.授权约束.并发上限 = 1;
    任务包.授权约束.允许真实写回 = 请求.允许正式资产提交;
    任务包.授权约束.允许回滚 =
        决策.当前后果治理 == 枚举_任务管理后果治理::进入收束;
    任务包.授权约束.必须人工确认 = 请求.允许正式资产提交;
    任务包.授权约束.动作白名单.push_back(任务包.动作类型);
    if (请求.上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::禁止继续) {
        任务包.授权约束.停止条件.push_back("上位禁止继续");
    }
    if (决策.当前下一步去向 == 枚举_任务管理下一步去向::进入收束) {
        任务包.授权约束.停止条件.push_back("进入收束");
    } else if (决策.当前下一步去向 == 枚举_任务管理下一步去向::保持等待) {
        任务包.授权约束.停止条件.push_back("保持等待");
    }
    if (决策.当前本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::无缺口
        && 决策.当前本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义) {
        任务包.授权约束.停止条件.push_back("存在本能能力缺口");
    }

    结构_高阶胜出绑定 高阶胜出绑定 = 决策.当前高阶胜出绑定;
    if (!私有_P0_高阶胜出绑定有效(高阶胜出绑定)) {
        私有_P0_覆盖高阶胜出绑定(高阶胜出绑定, 请求.当前高阶胜出绑定);
    }
    if (!私有_P0_高阶胜出绑定有效(高阶胜出绑定)) {
        结构_高阶排序结果 高阶排序结果 = 决策.当前高阶排序结果;
        if (!私有_P0_高阶排序结果有效(高阶排序结果)) {
            (void)私有_P0_生成高阶排序结果(请求, 高阶排序结果);
        }
        (void)私有_P0_解析高阶胜出绑定(请求, 高阶排序结果, 高阶胜出绑定);
    }

    任务包.高阶排序需求轴键 =
        !高阶胜出绑定.需求轴键.empty()
        ? 高阶胜出绑定.需求轴键
        : (!决策.当前高阶排序请求.需求轴键.empty()
            ? 决策.当前高阶排序请求.需求轴键
            : 请求.当前需求轴键);
    任务包.高阶排序胜出候选主键 =
        !高阶胜出绑定.胜出候选主键.empty()
        ? 高阶胜出绑定.胜出候选主键
        : 决策.当前高阶排序结果.胜出候选主键;
    任务包.高阶排序胜出方法主键 =
        高阶胜出绑定.胜出方法节点
        ? 私有_运行包主键_按指针(高阶胜出绑定.胜出方法节点)
        : 0;
    任务包.已显式消费高阶排序绑定 =
        高阶胜出绑定.已绑定执行方法
        && 高阶胜出绑定.胜出方法节点 != nullptr;
    任务包.高阶排序绑定摘要.clear();
    if (请求.存在控制请求) {
        任务包.控制请求主键 = 请求.当前控制请求.请求主键;
        任务包.控制意图值 = 请求.当前控制请求.控制意图值;
        任务包.控制意图需要确认 = 请求.当前控制请求.是否需要确认;
        任务包.控制意图摘要.clear();
    }

    const auto 默认方法置信度 = 私有_映射运行包方法置信度(决策.当前方法来源);
    if (高阶胜出绑定.已绑定执行方法 && 高阶胜出绑定.胜出方法节点) {
        const auto 高阶置信度 =
            决策.当前高阶排序结果.置信度_Q10000 != 0
            ? 决策.当前高阶排序结果.置信度_Q10000
            : std::max<std::int64_t>(默认方法置信度, 7000);
        const auto 高阶原因 =
            !高阶胜出绑定.绑定摘要.empty()
            ? ("高阶排序胜出绑定 | " + 高阶胜出绑定.绑定摘要)
            : std::string("高阶排序胜出绑定");
        私有_运行包追加方法候选(
            任务包,
            高阶胜出绑定.胜出方法节点,
            高阶置信度,
            高阶原因,
            true);
    }

    if (请求.上下文.当前方法) {
        const auto 当前方法原因 =
            任务包.已显式消费高阶排序绑定
            && 高阶胜出绑定.胜出方法节点 != 请求.上下文.当前方法
            ? std::string("当前方法回退候选 | ") + 私有_方法来源文本(决策.当前方法来源)
            : std::string(私有_方法来源文本(决策.当前方法来源));
        私有_运行包追加方法候选(
            任务包,
            请求.上下文.当前方法,
            默认方法置信度,
            当前方法原因,
            !任务包.选定方法.has_value());
    }

    if (写回.宿主目标状态) {
        任务包.预期输入状态主键集.push_back(私有_运行包主键_按指针(写回.宿主目标状态));
    }
    if (写回.宿主结果状态) {
        任务包.预期输出状态主键集.push_back(私有_运行包主键_按指针(写回.宿主结果状态));
    }

    任务包.要求记录动态 = true;
    任务包.要求记录副作用 = true;
    任务包.要求记录失败原因 = true;
    任务包.安全监测等级 = 私有_映射运行包安全监测等级(决策.当前后果治理);

    return 任务运行包::校验任务包(任务包, nullptr);
}

bool 任务管理任务模块::生成一步治理任务包(
    const 结构_任务管理结果& 来源结果,
    任务运行包::结构_一步治理任务包* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    结构_任务管理请求 请求 = 来源结果.当前请求;
    if (请求.宿主任务 == nullptr) {
        请求.宿主任务 = 私有_P0_当前宿主任务(来源结果);
    }
    if (请求.提交时间 == 0) {
        请求.提交时间 = 来源结果.上下文.当前时间;
    }
    if (请求.提交序号 == 0) {
        请求.提交序号 = 1;
    }
    if (请求.上下文.当前主需求 == nullptr) {
        请求.上下文.当前主需求 = 来源结果.上下文.当前主需求;
    }
    if (请求.上下文.当前管理任务 == nullptr) {
        请求.上下文.当前管理任务 = 来源结果.上下文.当前管理任务;
    }
    if (请求.上下文.当前宿主任务 == nullptr) {
        请求.上下文.当前宿主任务 = 私有_P0_当前宿主任务(来源结果);
    }
    if (请求.上下文.当前方法 == nullptr) {
        请求.上下文.当前方法 = 私有_P0_当前方法(来源结果);
    }
    if (请求.上下文.当前步骤 == nullptr) {
        请求.上下文.当前步骤 = 私有_P0_当前步骤节点(来源结果);
    }
    if (请求.上下文.最近结果 == nullptr) {
        请求.上下文.最近结果 = 私有_P0_最新结果节点(来源结果);
    }
    if (请求.上下文.当前主体ID == 0) {
        请求.上下文.当前主体ID = 来源结果.当前主体ID;
    }
    if (请求.上下文.当前分身ID == 0) {
        请求.上下文.当前分身ID = 来源结果.当前分身ID;
    }
    if (请求.上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::未定义) {
        请求.上下文.最近执行前门控结果 = 来源结果.最近执行前门控结果;
    }
    if (请求.上下文.当前治理态型.empty()) {
        请求.上下文.当前治理态型 = 来源结果.上下文.当前治理态型;
    }
    if (!请求.当前治理二次特征证据.有证据) {
        私有_P1_覆盖任务治理二次特征证据(请求.当前治理二次特征证据, 来源结果.当前治理二次特征证据);
    }
    if (!请求.允许正式资产提交) {
        请求.允许正式资产提交 =
            来源结果.允许正式资产提交
            || 来源结果.当前单步决策.允许正式资产提交;
    }
    if (请求.触发事件摘要.empty()) {
        请求.触发事件摘要 = 来源结果.摘要;
    }

    结构_任务管理单步决策 决策{};
    if (!形成任务管理单步决策(来源结果, &决策)) {
        return false;
    }
    结构_任务管理写回结果 写回{};
    if (!形成任务管理写回结果(来源结果, &写回)) {
        return false;
    }
    return 生成一步治理任务包(请求, 决策, 写回, 输出);
}

bool 任务管理任务模块::确保任务管理主体虚拟存在(
    自我类& 自我对象,
    任务类::节点类* 宿主任务,
    时间戳 now,
    结构_任务管理主体虚拟存在* 输出) noexcept
{
    auto* 锚点宿主 = 宿主任务 ? reinterpret_cast<任务节点*>(宿主任务) : 自我对象.获取当前主任务();
    if (!锚点宿主) {
        return false;
    }

    const auto 当前时间 = 私有_归一化时间(now);
    const auto 主体ID = 私有_主体ID_按宿主任务(锚点宿主);
    if (主体ID == 0) {
        return false;
    }

    std::scoped_lock 锁(私有_任务管理主体表锁);
    auto& 主体 = 私有_任务管理主体表[主体ID];
    if (主体.主体ID == 0) {
        主体 = {};
        主体.创建时间 = 当前时间;
        主体.主体ID = 主体ID;
        主体.版本号 = 1;
    }

    主体.最近激活时间 = 当前时间;
    主体.最近宿主任务 = 锚点宿主;
    主体.安全取向_Q10000 = 私有_Q10000比率(自我对象.获取安全值(), 自我对象.根目标安全值());
    主体.服务取向_Q10000 = 私有_Q10000比率(自我对象.获取服务值(), 自我对象.根目标服务值());
    主体.当前预算_Q10000 = 主体.服务取向_Q10000;
    主体.风险偏保守 =
        自我对象.是否待机状态()
        || 自我对象.获取安全值() <= (自我对象.根目标安全值() / 2);

    if (输出) {
        *输出 = 主体;
    }
    return true;
}

bool 任务管理任务模块::读取任务管理主体虚拟存在(
    任务类::节点类* 宿主任务,
    结构_任务管理主体虚拟存在* 输出) noexcept
{
    if (!宿主任务 || !输出) {
        return false;
    }

    const auto 主体ID = 私有_主体ID_按宿主任务(reinterpret_cast<任务节点*>(宿主任务));
    if (主体ID == 0) {
        return false;
    }

    std::scoped_lock 锁(私有_任务管理主体表锁);
    const auto it = 私有_任务管理主体表.find(主体ID);
    if (it == 私有_任务管理主体表.end()) {
        return false;
    }
    *输出 = it->second;
    return true;
}

bool 任务管理任务模块::初始化任务管理分身继承面(
    const 结构_任务管理上下文& 上下文,
    const 结构_任务管理主体虚拟存在& 主体,
    结构_任务管理分身继承面* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    输出->创建时间 = 上下文.当前时间;
    输出->最近推进时间 = 上下文.当前时间;
    输出->最近回并时间 = 0;
    输出->主体ID = 主体.主体ID != 0
        ? 主体.主体ID
        : (
            上下文.当前宿主任务
            ? 私有_主体ID_按宿主任务(上下文.当前宿主任务)
            : 私有_节点主键摘要(上下文.当前管理任务)
        );
    输出->分身ID = static_cast<std::uint64_t>(std::hash<std::string>{}(
        std::to_string(输出->主体ID)
        + "|" + (上下文.当前宿主任务 ? 上下文.当前宿主任务->获取主键() : std::string("空宿主"))
        + "|" + (上下文.当前方法 ? 上下文.当前方法->获取主键() : std::string("空方法"))
        + "|" + std::to_string(上下文.当前时间)));
    输出->回并序号 = 0;
    输出->当前状态 = 上下文.当前宿主任务
        ? 枚举_任务管理分身状态::可推进
        : 枚举_任务管理分身状态::待继承;
    输出->汇总策略 =
        上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::允许普通执行
        ? 枚举_任务管理主体汇总策略::高置信优先
        : 枚举_任务管理主体汇总策略::最近优先;
    输出->当前主方法需求位 = 枚举_任务管理方法需求位::未定义;
    输出->当前本能能力缺口类型 =
        上下文.当前本能能力缺口类型 != 枚举_任务管理本能能力缺口类型::未定义
        ? 上下文.当前本能能力缺口类型
        : 主体.最近本能能力缺口类型;
    输出->宿主任务 = 上下文.当前宿主任务;
    输出->当前步骤 = 上下文.当前步骤;
    输出->最近结果 = 上下文.最近结果;
    输出->当前管理方法 = 上下文.当前方法;
    输出->当前方法位专项动态 =
        上下文.当前方法位专项动态
        ? 上下文.当前方法位专项动态
        : 主体.最近方法位专项动态;
    输出->当前最小原语动态 =
        上下文.当前最小原语动态
        ? 上下文.当前最小原语动态
        : 主体.最近最小原语动态;
    输出->已继承宿主任务虚拟存在特征 = 输出->宿主任务 != nullptr;
    输出->已继承方法虚拟存在特征 = 输出->当前管理方法 != nullptr;
    私有_刷新分身继承位图(*输出, 上下文);
    输出->本轮输入条件主键 = 私有_主体化输入条件主键(上下文, *输出);
    输出->本轮结果主键 = 私有_主体化结果主键(*输出, 上下文);
    输出->当前治理态型 =
        !上下文.当前治理态型.empty()
        ? 上下文.当前治理态型
        : 主体.最近治理态型;
    输出->当前方法位专项动态标题 =
        !上下文.当前方法位专项动态标题.empty()
        ? 上下文.当前方法位专项动态标题
        : 主体.最近方法位专项动态标题;
    输出->当前最小原语动态标题 =
        !上下文.当前最小原语动态标题.empty()
        ? 上下文.当前最小原语动态标题
        : 主体.最近最小原语动态标题;

    std::ostringstream 摘要;
    摘要 << "宿主=" << 私有_任务标题(输出->宿主任务)
        << " | 方法=" << 私有_方法标题(输出->当前管理方法)
        << " | 域=" << 私有_功能域文本(上下文.最近功能域)
        << " | 缺口=" << 私有_缺口文本(上下文.当前缺口类型)
        << " | 去向=" << 私有_去向文本(上下文.当前下一步去向);
    输出->继承摘要 = 摘要.str();
    return true;
}

bool 任务管理任务模块::刷新任务管理方法需求位(
    const 结构_任务管理上下文& 上下文,
    const 结构_任务管理分身继承面& 分身,
    结构_任务管理方法需求位特征面* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    const auto* 当前方法 = 分身.当前管理方法;
    const bool 当前方法缺失 = 当前方法 == nullptr;
    const bool 当前方法未具备可用骨架 =
        当前方法 && !私有_学习方法已具备可用骨架(当前方法);

    输出->当前功能域 = 上下文.最近功能域;
    输出->当前缺口类型 = 上下文.当前缺口类型;
    输出->当前下一步去向 = 上下文.当前下一步去向;
    输出->当前步骤位类型 =
        上下文.当前步骤位类型 != 枚举_任务管理步骤位类型::未定义
        ? 上下文.当前步骤位类型
        : 私有_P0_推导步骤位类型(上下文);
    输出->当前步骤语义类型 =
        上下文.当前步骤语义类型 != 枚举_任务管理步骤语义类型::未定义
        ? 上下文.当前步骤语义类型
        : 私有_P0_推导步骤语义类型(上下文, 输出->当前步骤位类型);
    输出->当前步骤可复用 = 上下文.当前步骤可复用;
    输出->最近结果命中当前治理 = 上下文.最近结果命中当前治理;
    输出->当前动作长时间无推进 = 上下文.当前动作长时间无推进;
    输出->当前动作需重试 = 上下文.当前动作需重试;
    输出->当前需等待 = 上下文.当前需等待;
    输出->当前需收束 = 上下文.当前需收束;
    输出->可继续推进 = 上下文.可继续推进;
    输出->当前可回接最近结果 = 上下文.当前可回接最近结果;
    输出->稳定等待态复盘学习 = 上下文.稳定等待态复盘学习;
    输出->业务子任务已完成 = 上下文.业务子任务已完成;
    输出->业务子任务应转执行 = 上下文.业务子任务应转执行;
    输出->已触发学习承接 = 上下文.已触发学习承接;
    输出->当前学习承接类型 = 上下文.当前学习承接类型;
    输出->学习承接摘要.clear();
    输出->当前需求位 = 私有_推导任务管理方法需求位(上下文, 分身);
    输出->已命中需求位 = 输出->当前需求位 != 枚举_任务管理方法需求位::未定义;
    输出->需要新结构 =
        !分身.宿主任务
        || !分身.当前步骤
        || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::治理任务缺失
        || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::宿主任务缺失
        || 上下文.当前缺口类型 == 枚举_任务管理缺口类型::任务待启动;
    输出->需要新观察 = 分身.最近结果 == nullptr;
    输出->需要新因果 = 输出->最近抽象因果 == nullptr;
    输出->已满足完成判据 =
        (分身.宿主任务 && 分身.宿主任务->主信息.状态 == 枚举_任务状态::完成);
    输出->已确定无法继续 =
        (分身.宿主任务
            && (分身.宿主任务->主信息.状态 == 枚举_任务状态::失败
                || 分身.宿主任务->主信息.状态 == 枚举_任务状态::取消))
        || 上下文.最近执行前门控结果 == 枚举_任务管理执行前门控结果::禁止继续
        || 上下文.最近根层重判结果 == 枚举_任务管理根层重判结果::停止退出边界;
    输出->需要新学习 =
        输出->已触发学习承接
        || 输出->当前学习承接类型 != 枚举_任务管理学习承接类型::未定义
        || 输出->当前步骤语义类型 == 枚举_任务管理步骤语义类型::等待维持步骤
        || 输出->当前步骤语义类型 == 枚举_任务管理步骤语义类型::补条件步骤
        || 输出->当前需求位 == 枚举_任务管理方法需求位::失败策略
        || (输出->已命中需求位 && (当前方法缺失 || 当前方法未具备可用骨架))
        || 输出->需要新因果;
    输出->输入条件主键 =
        !分身.本轮输入条件主键.empty()
        ? 分身.本轮输入条件主键
        : 私有_主体化输入条件主键(上下文, 分身);
    输出->结果主键 =
        !分身.本轮结果主键.empty()
        ? 分身.本轮结果主键
        : 私有_主体化结果主键(分身, 上下文);
    输出->最新目标状态 = 分身.宿主任务 ? 分身.宿主任务->主信息.目标状态.指针 : nullptr;
    输出->最新结果状态 = 分身.宿主任务 ? 分身.宿主任务->主信息.结果状态信息.指针 : nullptr;
    输出->当前方法位专项动态 = 分身.当前方法位专项动态;
    输出->当前方法位专项动态标题 = 分身.当前方法位专项动态标题;
    if (输出->最近抽象因果 && 输出->最近抽象因果摘要.empty()) {
        输出->最近抽象因果摘要 = 私有_P0_抽象因果摘要(输出->最近抽象因果);
    }

    std::ostringstream 摘要;
    摘要 << "需求位=" << 私有_方法需求位文本(输出->当前需求位)
        << " | 宿主=" << 私有_任务标题(分身.宿主任务)
        << " | 缺口=" << 私有_缺口文本(上下文.当前缺口类型)
        << " | 去向=" << 私有_去向文本(上下文.当前下一步去向)
        << " | 步骤位=" << 私有_步骤位类型文本(输出->当前步骤位类型)
        << " | 步骤语义=" << 私有_步骤语义类型文本(输出->当前步骤语义类型);
    if (当前方法缺失) {
        摘要 << " | 方法状态=缺失";
    }
    else if (当前方法未具备可用骨架) {
        摘要 << " | 方法状态=待补齐";
    }
    if (输出->已触发学习承接 || 输出->当前学习承接类型 != 枚举_任务管理学习承接类型::未定义) {
        摘要 << " | 学习承接=" << 私有_学习承接类型文本(输出->当前学习承接类型);
    }
    输出->需求摘要 = 摘要.str();
    return 输出->已命中需求位;
}

bool 任务管理任务模块::评估任务管理本能能力缺口(
    自我类& 自我对象,
    const 结构_任务管理方法需求位特征面& 需求位特征,
    结构_任务管理本能能力缺口面* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    输出->对应需求位 = 需求位特征.当前需求位;
    输出->所需原语位图 =
        私有_原语位(枚举_任务管理最小原语ID::确保单承载存在)
        | 私有_原语位(枚举_任务管理最小原语ID::读取单特征当前值)
        | 私有_原语位(枚举_任务管理最小原语ID::写入单特征当前值)
        | 私有_原语位(枚举_任务管理最小原语ID::写入单挂点指针)
        | 私有_原语位(枚举_任务管理最小原语ID::记录动作开始)
        | 私有_原语位(枚举_任务管理最小原语ID::记录动作结束);
    输出->所需派生能力位图 =
        私有_派生能力位_状态差分取证
        | 私有_派生能力位_实例因果派生
        | 私有_派生能力位_抽象因果归纳;

    (void)本能方法类::注册默认自我本能方法集(
        自我对象,
        "任务管理任务模块::评估任务管理本能能力缺口/注册默认本能方法");

    // 这里先按“正式动作资产已存在”判定六原语是否具备，
    // 不再直接把工程 helper 视为默认全具备。
    输出->已具备确保单承载存在 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::确保单承载存在,
        "任务管理任务模块::评估任务管理本能能力缺口/确保单承载存在");
    输出->已具备读取单特征当前值 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::读取单特征当前值,
        "任务管理任务模块::评估任务管理本能能力缺口/读取单特征当前值");
    输出->已具备写入单特征当前值 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::写入单特征当前值,
        "任务管理任务模块::评估任务管理本能能力缺口/写入单特征当前值");
    输出->已具备写入单挂点指针 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::写入单挂点指针,
        "任务管理任务模块::评估任务管理本能能力缺口/写入单挂点指针");
    输出->已具备记录动作开始 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::记录动作开始,
        "任务管理任务模块::评估任务管理本能能力缺口/记录动作开始");
    输出->已具备记录动作结束 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::记录动作结束,
        "任务管理任务模块::评估任务管理本能能力缺口/记录动作结束");
    输出->已完成固定派生规则 =
        输出->已具备记录动作开始 && 输出->已具备记录动作结束;
    输出->已具备状态差分取证 =
        需求位特征.最新目标状态 != nullptr && 需求位特征.最新结果状态 != nullptr;
    输出->已具备实例因果派生 =
        需求位特征.最近开始动态 != nullptr
        || 需求位特征.最近结束动态 != nullptr
        || 需求位特征.当前方法位专项动态 != nullptr;
    输出->已具备抽象因果归纳 =
        需求位特征.最近抽象因果 != nullptr
        || !需求位特征.最近抽象因果摘要.empty();

    输出->已具备原语位图 = 0;
    if (输出->已具备确保单承载存在) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::确保单承载存在);
    if (输出->已具备读取单特征当前值) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::读取单特征当前值);
    if (输出->已具备写入单特征当前值) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::写入单特征当前值);
    if (输出->已具备写入单挂点指针) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::写入单挂点指针);
    if (输出->已具备记录动作开始) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::记录动作开始);
    if (输出->已具备记录动作结束) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::记录动作结束);

    输出->已具备派生能力位图 = 0;
    if (输出->已具备状态差分取证) 输出->已具备派生能力位图 |= 私有_派生能力位_状态差分取证;
    if (输出->已具备实例因果派生) 输出->已具备派生能力位图 |= 私有_派生能力位_实例因果派生;
    if (输出->已具备抽象因果归纳) 输出->已具备派生能力位图 |= 私有_派生能力位_抽象因果归纳;

    输出->主观察特征键 =
        !需求位特征.结果主键.empty() ? 需求位特征.结果主键 : 需求位特征.输入条件主键;
    输出->原语账本摘要 = 私有_原语位图摘要(输出->已具备原语位图);
    输出->派生规则摘要 =
        "固定派生规则="
        + std::string(输出->已完成固定派生规则 ? "已接线" : "未接线")
        + " | 派生能力="
        + 私有_派生能力位图摘要(输出->已具备派生能力位图);
    输出->允许直接执行 = false;
    输出->需要学习补齐 = false;
    输出->需要等待外部条件 = false;
    输出->需要退回补结构 = false;

    if (需求位特征.需要新学习) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::需学习补齐;
        输出->需要学习补齐 = true;
    }
    else if (!输出->已具备确保单承载存在 || !输出->已具备读取单特征当前值) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺观察原语;
    }
    else if (!输出->已具备写入单特征当前值) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺写回原语;
    }
    else if (!输出->已具备写入单挂点指针) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺挂点原语;
    }
    else if (!输出->已完成固定派生规则) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺动作边界;
    }
    else if (!输出->已具备状态差分取证) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺状态差分取证;
        输出->需要退回补结构 = true;
    }
    else if (!输出->已具备实例因果派生) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺实例因果取证;
    }
    else if (!输出->已具备抽象因果归纳) {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::缺抽象因果归纳;
    }
    else {
        输出->当前缺口类型 = 枚举_任务管理本能能力缺口类型::无缺口;
        输出->允许直接执行 = true;
    }

    std::ostringstream 摘要;
    摘要 << "需求位=" << 私有_方法需求位文本(需求位特征.当前需求位)
        << " | 缺口=" << 私有_本能能力缺口文本(输出->当前缺口类型)
        << " | 主观察=" << (输出->主观察特征键.empty() ? "空" : 输出->主观察特征键)
        << " | 正式已具备=" << (输出->原语账本摘要.empty() ? "空" : 输出->原语账本摘要)
        << " | 派生摘要=" << (输出->派生规则摘要.empty() ? "空" : 输出->派生规则摘要);
    输出->缺口摘要 = 摘要.str();
    return true;
}

bool 任务管理任务模块::读取任务管理正式原语资产概览(
    const 自我类& 自我对象,
    结构_任务管理正式原语资产概览* 输出) noexcept
{
    if (!输出) {
        return false;
    }

    *输出 = {};
    输出->所需原语位图 =
        私有_原语位(枚举_任务管理最小原语ID::确保单承载存在)
        | 私有_原语位(枚举_任务管理最小原语ID::读取单特征当前值)
        | 私有_原语位(枚举_任务管理最小原语ID::写入单特征当前值)
        | 私有_原语位(枚举_任务管理最小原语ID::写入单挂点指针)
        | 私有_原语位(枚举_任务管理最小原语ID::记录动作开始)
        | 私有_原语位(枚举_任务管理最小原语ID::记录动作结束);

    输出->已具备确保单承载存在 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::确保单承载存在,
        "任务管理任务模块::读取任务管理正式原语资产概览/确保单承载存在");
    输出->已具备读取单特征当前值 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::读取单特征当前值,
        "任务管理任务模块::读取任务管理正式原语资产概览/读取单特征当前值");
    输出->已具备写入单特征当前值 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::写入单特征当前值,
        "任务管理任务模块::读取任务管理正式原语资产概览/写入单特征当前值");
    输出->已具备写入单挂点指针 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::写入单挂点指针,
        "任务管理任务模块::读取任务管理正式原语资产概览/写入单挂点指针");
    输出->已具备记录动作开始 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::记录动作开始,
        "任务管理任务模块::读取任务管理正式原语资产概览/记录动作开始");
    输出->已具备记录动作结束 = 私有_任务管理最小原语已正式注册(
        自我对象,
        枚举_任务管理最小原语ID::记录动作结束,
        "任务管理任务模块::读取任务管理正式原语资产概览/记录动作结束");

    if (输出->已具备确保单承载存在) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::确保单承载存在);
    if (输出->已具备读取单特征当前值) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::读取单特征当前值);
    if (输出->已具备写入单特征当前值) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::写入单特征当前值);
    if (输出->已具备写入单挂点指针) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::写入单挂点指针);
    if (输出->已具备记录动作开始) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::记录动作开始);
    if (输出->已具备记录动作结束) 输出->已具备原语位图 |= 私有_原语位(枚举_任务管理最小原语ID::记录动作结束);

    输出->缺失原语位图 = 输出->所需原语位图 & ~输出->已具备原语位图;
    输出->已全部具备 = 输出->缺失原语位图 == 0;
    输出->已具备摘要 = 私有_原语位图摘要(输出->已具备原语位图);
    输出->缺失摘要 = 输出->缺失原语位图 == 0
        ? "无"
        : 私有_原语位图摘要(输出->缺失原语位图);

    std::ostringstream 摘要;
    摘要 << "状态=" << (输出->已全部具备 ? "齐全" : "缺失")
        << " | 正式已具备=" << (输出->已具备摘要.empty() ? std::string("空") : 输出->已具备摘要)
        << " | 正式缺失=" << 输出->缺失摘要;
    输出->摘要 = 摘要.str();
    return true;
}

std::string 任务管理任务模块::摘要任务管理最小原语位图(std::uint64_t 位图) noexcept
{
    return 私有_原语位图摘要(位图);
}

bool 任务管理任务模块::回并分身增量到任务管理主体(
    const 结构_任务管理分身继承面& 分身,
    结构_任务管理主体虚拟存在* 主体) noexcept
{
    if (!主体 || 主体->主体ID == 0) {
        return false;
    }

    const auto 需求位索引 = static_cast<std::size_t>(分身.当前主方法需求位);
    主体->最近回并时间 = 分身.最近回并时间 != 0 ? 分身.最近回并时间 : 分身.最近推进时间;
    主体->最近回并序号 = 分身.回并序号;
    主体->最近输入条件主键 = 分身.本轮输入条件主键;
    主体->最近结果主键 = 分身.本轮结果主键;
    主体->最近原因摘要 = 分身.继承摘要;
    主体->最近宿主任务 = 分身.宿主任务;
    主体->最近管理方法 = 分身.当前管理方法;
    私有_P0_覆盖非默认值(
        主体->最近本能能力缺口类型,
        分身.当前本能能力缺口类型,
        枚举_任务管理本能能力缺口类型::未定义);
    私有_P0_覆盖非空文本(主体->最近治理态型, 分身.当前治理态型);
    主体->最近方法位专项动态 = 分身.当前方法位专项动态;
    主体->最近最小原语动态 = 分身.当前最小原语动态;
    主体->最近方法位专项动态标题 = 分身.当前方法位专项动态标题;
    主体->最近最小原语动态标题 = 分身.当前最小原语动态标题;
    主体->累计回并分身数 += 1;
    主体->累计推进次数 += 分身.本地推进次数;
    主体->累计完成次数 += 分身.本地完成次数;
    主体->累计失败次数 += 分身.本地失败次数;
    主体->累计稳定复现次数 += 分身.本地验证证据次数;
    主体->累计价值成立次数 += 分身.本地价值证据次数;
    if (主体->当前活跃分身数 > 0) {
        主体->当前活跃分身数 -= 1;
    }
    if (需求位索引 < std::size(主体->需求位命中次数)) {
        主体->最近主方法需求位 = 分身.当前主方法需求位;
        主体->需求位命中次数[需求位索引] += 1;
        主体->需求位成功次数[需求位索引] += 分身.本地推进次数 > 0 ? 1U : 0U;
        主体->需求位弱出生证据次数[需求位索引] += 分身.本地弱出生证据次数;
        主体->需求位验证证据次数[需求位索引] += 分身.本地验证证据次数;
        主体->需求位升格证据次数[需求位索引] += 分身.本地升格证据次数;
    }

    std::scoped_lock 锁(私有_任务管理主体表锁);
    私有_任务管理主体表[主体->主体ID] = *主体;
    return true;
}

bool 任务管理任务模块::提交学习方法资产(
    自我类& 自我对象,
    const 结构_学习方法资产提交输入& 输入,
    时间戳 now,
    const std::string& 调用点,
    结构_学习方法资产提交结果* 输出) noexcept
{
    if (!输入.来源宿主任务) {
        return false;
    }

    结构_任务管理结果 结果{};
    结构_任务管理上下文 提交上下文{};
    提交上下文.当前管理任务 = 自我对象.获取任务管理任务();
    提交上下文.当前宿主任务 = 输入.来源宿主任务;
    提交上下文.当前方法 = 输入.来源方法;
    提交上下文.当前步骤 = 输入.来源步骤;
    提交上下文.最近结果 = 输入.来源结果;
    提交上下文.当前缺口类型 = 输入.当前缺口类型;
    提交上下文.当前下一步去向 = 输入.当前下一步去向;
    提交上下文.当前方法需求位 = 输入.来源方法需求位;
    提交上下文.当前本能能力缺口类型 = 输入.来源本能能力缺口类型;
    提交上下文.当前输入条件主键 =
        !输入.当前输入条件主键.empty()
        ? 输入.当前输入条件主键
        : 输入.来源主观察特征;
    提交上下文.当前治理态型 =
        !输入.学习反馈摘要.empty()
        ? 输入.学习反馈摘要
        : 输入.回流目标摘要;

    结构_任务管理请求 提交请求{};
    提交请求.宿主任务 = 输入.来源宿主任务;
    提交请求.上下文 = 提交上下文;
    提交请求.触发来源 = 输入.当前触发来源;
    提交请求.来源最小原语位图 = 输入.来源最小原语位图;
    提交请求.来源主观察特征 = 输入.来源主观察特征;
    提交请求.当前治理二次特征证据.有证据 = !输入.当前治理证据摘要.empty();
    提交请求.当前治理二次特征证据.摘要 = 输入.当前治理证据摘要;
    提交请求.影子验证状态 = 输入.当前影子验证状态;
    提交请求.允许正式资产提交 = 输入.允许正式资产提交;
    提交请求.补充材料摘要.clear();
    提交请求.当前高阶胜出绑定.胜出候选主键 = 输入.来源高阶排序胜出候选主键;
    提交请求.当前高阶胜出绑定.胜出方法主键 = 输入.来源高阶排序胜出方法主键;
    提交请求.当前高阶胜出绑定.已绑定执行方法 = !输入.来源高阶排序胜出方法主键.empty();
    提交请求.当前高阶胜出绑定.绑定摘要 = 输入.来源高阶排序绑定摘要;
    私有_P0_刷新请求原语锚点(提交请求);
    (void)私有_P1_刷新任务治理二次特征证据(
        自我对象,
        提交请求,
        调用点 + "/刷新治理证据");
    私有_P0_覆盖请求字段(结果.当前请求, 提交请求);
    私有_P0_覆盖上下文字段(结果.上下文, 提交上下文);

    结果.当前学习方法节点 = 输入.学习方法;
    结果.当前子任务节点 = 输入.学习子任务;
    结果.当前步骤节点 = 输入.来源步骤;
    结果.最新结果节点 = 输入.来源结果;
    结果.当前缺口类型 = 输入.当前缺口类型;
    结果.当前下一步去向 = 输入.当前下一步去向;
    结果.当前方法来源 = 输入.当前方法来源;
    结果.当前主方法需求位 = 输入.来源方法需求位;
    结果.当前本能能力缺口类型 = 输入.来源本能能力缺口类型;
    私有_P1_覆盖任务治理二次特征证据(结果.当前治理二次特征证据, 提交请求.当前治理二次特征证据);
    结果.当前需求位特征.输入条件主键 = 输入.当前输入条件主键;
    结果.当前需求位特征.结果主键 = 输入.当前结果主键;
    结果.当前需求位特征.最近抽象因果摘要 = 输入.最近抽象因果摘要;

    结构_任务管理单步决策 提交决策{};
    提交决策.当前缺口类型 = 输入.当前缺口类型;
    提交决策.当前下一步去向 = 输入.当前下一步去向;
    提交决策.当前方法来源 = 输入.当前方法来源;
    提交决策.当前触发来源 = 输入.当前触发来源;
    提交决策.当前方法需求位 = 输入.来源方法需求位;
    提交决策.当前本能能力缺口类型 = 输入.来源本能能力缺口类型;
    提交决策.来源最小原语位图 = 输入.来源最小原语位图;
    提交决策.来源主观察特征 = 输入.来源主观察特征;
    提交决策.影子验证状态 = 输入.当前影子验证状态;
    提交决策.允许正式资产提交 = 输入.允许正式资产提交;
    提交决策.决策摘要.clear();
    提交决策.当前高阶胜出绑定 = 提交请求.当前高阶胜出绑定;
    私有_P0_覆盖决策字段(结果.当前单步决策, 提交决策);

    结构_任务管理写回结果 提交写回{};
    提交写回.当前学习方法 = 输入.学习方法;
    提交写回.当前学习子任务 = 输入.学习子任务;
    提交写回.当前步骤节点 = 输入.来源步骤;
    提交写回.最新结果节点 = 输入.来源结果;
    提交写回.主体回并摘要 =
        !输入.学习反馈摘要.empty()
        ? 输入.学习反馈摘要
        : 输入.回流目标摘要;
    私有_P0_覆盖写回字段(结果.当前写回结果, 提交写回);

    结构_任务管理一步结果 提交一步结果{};
    提交一步结果.宿主任务 = 输入.来源宿主任务;
    提交一步结果.当前方法节点 = 输入.来源方法;
    提交一步结果.当前步骤节点 = 输入.来源步骤;
    提交一步结果.最新结果节点 = 输入.来源结果;
    提交一步结果.当前缺口类型 = 输入.当前缺口类型;
    提交一步结果.当前下一步去向 = 输入.当前下一步去向;
    提交一步结果.当前方法需求位 = 输入.来源方法需求位;
    提交一步结果.当前本能能力缺口类型 = 输入.来源本能能力缺口类型;
    提交一步结果.当前触发来源 = 输入.当前触发来源;
    提交一步结果.当前输入条件主键 = 输入.来源主观察特征;
    提交一步结果.来源最小原语位图 = 输入.来源最小原语位图;
    提交一步结果.来源主观察特征 = 输入.来源主观察特征;
    提交一步结果.影子验证状态 = 输入.当前影子验证状态;
    提交一步结果.允许正式资产提交 = 输入.允许正式资产提交;
    提交一步结果.已触发学习承接 = true;
    提交一步结果.当前学习承接类型 = 输入.当前学习承接类型;
    提交一步结果.当前学习为本能补齐 =
        私有_本能缺口属于原语或治理补齐(输入.来源本能能力缺口类型);
    提交一步结果.学习承接摘要.clear();
    提交一步结果.当前高阶胜出绑定 = 提交请求.当前高阶胜出绑定;
    提交一步结果.主体回并摘要 =
        !输入.学习反馈摘要.empty()
        ? 输入.学习反馈摘要
        : 输入.回流目标摘要;
    私有_P0_覆盖一步结果字段(结果.当前一步结果, 提交一步结果);

    结果.当前学习承接决策.是否触发学习 = true;
    结果.当前学习承接决策.学习类型 = 输入.当前学习承接类型;
    结果.当前学习承接决策.补原语或治理本能 =
        私有_本能缺口属于原语或治理补齐(输入.来源本能能力缺口类型);
    结果.当前学习承接决策.来源根任务 =
        输入.来源宿主任务 ? 私有_主体锚点任务(输入.来源宿主任务) : nullptr;
    结果.当前学习承接决策.来源父任务 =
        输入.当前学习承接类型 == 枚举_任务管理学习承接类型::广泛学习
            ? reinterpret_cast<任务节点*>(自我对象.获取任务管理任务())
            : reinterpret_cast<任务节点*>(输入.来源宿主任务);
    结果.当前学习承接决策.来源功能域 = 结果.当前功能域;
    结果.当前学习承接决策.来源缺口类型 = 输入.当前缺口类型;
    结果.当前学习承接决策.来源方法需求位 = 输入.来源方法需求位;
    结果.当前学习承接决策.来源本能能力缺口类型 = 输入.来源本能能力缺口类型;
    结果.当前学习承接决策.来源最小原语位图 = 输入.来源最小原语位图;
    结果.当前学习承接决策.来源主观察特征 = 输入.来源主观察特征;
    结果.当前学习承接决策.方法服务目标 = 输入.方法服务目标;
    结果.当前学习承接决策.方法服务范围 = 输入.方法服务范围;
    结果.当前学习承接决策.默认消费策略 = 输入.默认消费策略;
    结果.当前学习承接决策.回流目标方法位 = 输入.回流目标方法位;
    结果.当前学习承接决策.来源高阶排序胜出候选主键 = 输入.来源高阶排序胜出候选主键;
    结果.当前学习承接决策.来源高阶排序胜出方法主键 = 输入.来源高阶排序胜出方法主键;
    结果.当前学习承接决策.来源高阶排序已显式绑定 = !输入.来源高阶排序胜出方法主键.empty();
    结果.当前学习承接决策.来源高阶排序绑定摘要.clear();
    结果.当前学习承接决策.回流目标摘要 =
        !输入.学习反馈摘要.empty()
        ? 输入.学习反馈摘要
        : 输入.回流目标摘要;
    结果.当前学习承接决策.是否影子验证 = !输入.允许正式资产提交;
    结果.当前学习承接决策.允许正式资产提交 = 输入.允许正式资产提交;
    结果.当前学习承接决策.决策摘要.clear();

    auto* 学习子任务 = 输入.学习子任务;
    if (!学习子任务) {
        auto* 当前管理任务 = 自我对象.获取任务管理任务();
        结果.上下文.当前管理任务 = 当前管理任务;
        学习子任务 = 私有_创建或复用学习子任务头(
            自我对象,
            reinterpret_cast<任务节点*>(当前管理任务),
            reinterpret_cast<任务节点*>(输入.来源宿主任务),
            结果.当前学习承接决策,
            结果,
            now,
            调用点 + "/补齐学习子任务");
        if (!学习子任务) {
            return false;
        }
        结果.当前子任务节点 = 学习子任务;
        结果.当前写回结果.当前学习子任务 = 学习子任务;
    }

    auto* 方法头 = 私有_写回学习方法骨架(
        自我对象,
        输入.来源宿主任务,
        学习子任务,
        false,
        结果,
        now,
        调用点 + "/正式入账");
    if (!方法头) {
        return false;
    }
    私有_P2_补齐学习方法证据索引(
        输入,
        reinterpret_cast<任务节点*>(输入.来源宿主任务),
        学习子任务,
        方法头,
        结果,
        调用点 + "/补齐证据索引");

    结构_学习方法资产提交结果 本地结果{};
    本地结果.已写回正式方法资产 = true;
    本地结果.已创建学习方法头 = 结果.已创建学习方法头;
    本地结果.已复用学习方法头 = 结果.已复用学习方法头;
    本地结果.已创建学习方法条件节点 = 结果.已创建学习方法条件节点;
    本地结果.已创建学习方法结果节点 = 结果.已创建学习方法结果节点;
    本地结果.当前学习方法节点 = 方法头;
    本地结果.正式方法标题 = 私有_方法标题(方法头);
    本地结果.治理证据摘要 =
        !结果.当前治理二次特征证据.摘要.empty()
        ? 结果.当前治理二次特征证据.摘要
        : 输入.当前治理证据摘要;
    本地结果.因果来源摘要 =
        !输入.最近抽象因果主键.empty()
        ? ("抽象因果=" + 输入.最近抽象因果主键)
        : std::string{};
    if (!输入.最近抽象因果摘要.empty()) {
        if (!本地结果.因果来源摘要.empty()) {
            本地结果.因果来源摘要 += " | ";
        }
        本地结果.因果来源摘要 += "抽象因果摘要=" + 输入.最近抽象因果摘要;
    }

    const bool 复用现有方法头 =
        !结果.已创建学习方法头
        && 输入.来源方法
        && 方法头
        && 输入.来源方法->获取主键() == 方法头->获取主键();
    const bool 视为新生正式方法资产 =
        结果.已创建学习方法头
        || !输入.来源方法
        || 输入.当前方法来源 == 枚举_任务管理方法来源::默认本能兜底;
    本地结果.资产类型 = 视为新生正式方法资产
        ? "新生正式方法资产"
        : (复用现有方法头 ? "复用现有方法头" : "沿用学习方法资产");

    std::ostringstream 摘要;
    摘要 << 本地结果.资产类型 << "="
        << (本地结果.正式方法标题.empty() ? std::string("空方法") : 本地结果.正式方法标题);
    if (结果.已创建学习方法条件节点) {
        摘要 << " | 已补条件节点";
    }
    if (结果.已创建学习方法结果节点) {
        摘要 << " | 已补结果节点";
    }
    if (!本地结果.治理证据摘要.empty()) {
        摘要 << " | 治理证据=" << 本地结果.治理证据摘要;
    }
    if (!本地结果.因果来源摘要.empty()) {
        摘要 << " | 因果=" << 本地结果.因果来源摘要;
    }
    std::string 主体长期策略摘要{};
    if (私有_M10_沉淀正式学习反馈到主体(
        自我对象,
        输入,
        本地结果,
        now,
        &结果.当前主体,
        &主体长期策略摘要)) {
        结果.已确保任务管理主体虚拟存在 = true;
        结果.当前主体ID = 结果.当前主体.主体ID;
        结果.上下文.当前主体ID = 结果.当前主体ID;
        结果.当前写回结果.主体回并摘要 = 主体长期策略摘要;
        结果.当前一步结果.主体回并摘要 = 主体长期策略摘要;
        本地结果.主体长期策略摘要 = 主体长期策略摘要;
        if (!主体长期策略摘要.empty()) {
            摘要 << " | 主体策略=" << 主体长期策略摘要;
        }
    }
    本地结果.摘要 = 摘要.str();

    if (输出) {
        *输出 = std::move(本地结果);
    }
    return true;
}

bool 任务管理任务模块::执行一步学习推进(
    自我类& 自我对象,
    时间戳 当前时间,
    const std::string& 调用点,
    结构_任务管理学习推进结果* 输出) noexcept
{
    结构_任务管理学习推进结果 结果{};
    结构_学习调度项 调度项{};
    结构_学习调度快照 调度快照{};
    结果.已构建学习调度快照 = true;
    if (学习调度模块::选择一条可执行学习任务(&调度项, &调度快照)) {
        结果.已选中学习任务 = true;
        结果.当前学习账本ID = 调度项.实体.账本ID;
        结果.学习调度摘要 = 调度快照.摘要;
        const auto 执行结果 = 学习执行模块::执行一步学习任务(
            自我对象,
            调度项,
            当前时间,
            调用点);
        结果.已执行学习任务 = 执行结果.已命中任务;
        结果.学习执行摘要 = 执行结果.摘要;
        结果.学习反馈摘要 = 执行结果.学习反馈摘要;
    }
    else {
        调度快照 = 学习调度模块::构建学习调度快照();
        结果.学习调度摘要 = 调度快照.摘要;
    }

    if (输出) {
        *输出 = std::move(结果);
    }
    return true;
}

bool 任务管理任务模块::执行一步治理(
    自我类& 自我对象,
    时间戳 now,
    const std::string& 调用点,
    结构_任务管理结果* 输出) noexcept
{
    const bool 调试M10回填 = 调用点.find("M10长期策略驱动回填") != std::string::npos;
    if (调试M10回填) {
        项目运行日志("任务管理/M10回填 | 进入执行一步治理");
    }
    const auto 当前时间 = 私有_归一化时间(now);
    结构_任务管理结果 结果{};
    结构_M2治理阶段状态 阶段状态{};
    if (调试M10回填) {
        项目运行日志("任务管理/M10回填 | 准备执行筹办模块");
    }
    if (!私有_执行筹办模块(自我对象, 当前时间, 调用点, 结果, 阶段状态)) {
        if (调试M10回填) {
            项目运行日志("任务管理/M10回填 | 筹办模块返回停止");
        }
        if (输出) {
            *输出 = 结果;
        }
        return 私有_治理轮返回值(结果);
    }
    if (调试M10回填) {
        项目运行日志("任务管理/M10回填 | 筹办模块完成");
    }
    if (调试M10回填) {
        项目运行日志("任务管理/M10回填 | 准备执行可执行判断模块");
    }
    私有_执行可执行判断模块(自我对象, 当前时间, 调用点, 结果, 阶段状态);
    if (调试M10回填) {
        项目运行日志("任务管理/M10回填 | 可执行判断模块完成");
        项目运行日志("任务管理/M10回填 | 准备执行允许执行判断模块");
    }
    私有_执行允许执行判断模块(自我对象, 当前时间, 调用点, 结果, 阶段状态);
    if (调试M10回填) {
        项目运行日志("任务管理/M10回填 | 允许执行判断模块完成");
        项目运行日志("任务管理/M10回填 | 准备执行执行模块");
    }
    私有_执行执行模块(自我对象, 当前时间, 调用点, 结果, 阶段状态);
    if (调试M10回填) {
        项目运行日志("任务管理/M10回填 | 执行模块完成");
        项目运行日志("任务管理/M10回填 | 准备执行反馈模块");
    }
    私有_执行反馈模块(自我对象, 当前时间, 调用点, 结果, 阶段状态);
    if (调试M10回填) {
        项目运行日志("任务管理/M10回填 | 反馈模块完成");
    }

    if (输出) {
        *输出 = 结果;
    }

    return 私有_治理轮返回值(结果);
}
