module;

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "任务类.h"
#include "方法类.h"

export module 学习任务模块;
import 任务管理任务模块;

export {

using 时间戳 = std::uint64_t;
using I64 = std::int64_t;

enum class 枚举_学习阶段 : std::uint8_t {
    未定义 = 0,
    待适配 = 1,
    待调度 = 2,
    采样 = 3,
    更新 = 4,
    验证 = 5,
    提交 = 6,
    回滚 = 7,
};

enum class 枚举_学习状态 : std::uint8_t {
    未定义 = 0,
    待创建 = 1,
    挂起 = 2,
    可调度 = 3,
    执行中 = 4,
    已提交 = 5,
    已回滚 = 6,
    已失败 = 7,
};

enum class 枚举_学习等待原因 : std::uint8_t {
    无 = 0,
    原料不足 = 1,
    仅需求位方法头 = 2,
    等待对象补齐 = 3,
    等待再次命中 = 4,
    根层禁止学习 = 5,
    等待提交前审计 = 6,
    等待正式提交放行 = 7,
};

enum class 枚举_学习影响面 : std::uint8_t {
    未定义 = 0,
    自我真值型 = 1,
    内部推演型 = 2,
    现实影响型 = 3,
};

enum class 枚举_学习结果状态 : std::uint8_t {
    未定义 = 0,
    原料待补 = 1,
    骨架生成中 = 2,
    待验证裁决 = 3,
    影子验证通过 = 4,
    验证门通过 = 5,
    审计门通过 = 6,
    正式提交完成 = 7,
    外层正式消费 = 8,
    外层挂起观察 = 9,
    外层驳回退回 = 10,
    验证回滚待补 = 11,
    正式提交失败 = 12,
};

enum class 枚举_学习外层裁决 : std::uint8_t {
    未定义 = 0,
    待裁决 = 1,
    正式消费 = 2,
    挂起观察 = 3,
    驳回退回 = 4,
};

enum class 枚举_学习执行策略 : std::uint8_t {
    未定义 = 0,
    镜像验证 = 1,
    受控微试探 = 2,
    正式资产提交 = 3,
};

struct 结构_学习任务同步输入 {
    时间戳 当前时间 = 0;
    std::string 去重键{};
    任务类::节点类* 学习子任务 = nullptr;
    方法类::节点类* 学习方法 = nullptr;
    方法类::节点类* 学习任务兜底方法 = nullptr;
    方法类::节点类* 当前选中学习方法 = nullptr;
    任务类::节点类* 来源宿主任务 = nullptr;
    任务类::节点类* 来源步骤 = nullptr;
    任务类::节点类* 来源结果 = nullptr;
    方法类::节点类* 来源方法 = nullptr;
    枚举_任务管理根层重判结果 最近根层重判结果 = static_cast<枚举_任务管理根层重判结果>(0);
    枚举_任务管理缺口类型 当前缺口类型 = static_cast<枚举_任务管理缺口类型>(0);
    枚举_任务管理下一步去向 当前下一步去向 = static_cast<枚举_任务管理下一步去向>(0);
    枚举_任务管理方法来源 当前方法来源 = static_cast<枚举_任务管理方法来源>(0);
    枚举_任务管理触发来源 当前触发来源 = static_cast<枚举_任务管理触发来源>(0);
    枚举_任务管理学习承接类型 当前学习承接类型 = static_cast<枚举_任务管理学习承接类型>(0);
    枚举_任务管理方法需求位 来源方法需求位 = static_cast<枚举_任务管理方法需求位>(0);
    枚举_任务管理本能能力缺口类型 来源本能能力缺口类型 = static_cast<枚举_任务管理本能能力缺口类型>(0);
    枚举_任务管理方法需求位 方法服务目标 = static_cast<枚举_任务管理方法需求位>(0);
    枚举_任务管理学习反馈消费策略 学习情况消费策略 = static_cast<枚举_任务管理学习反馈消费策略>(0);
    枚举_任务管理方法需求位 回流目标方法位 = static_cast<枚举_任务管理方法需求位>(0);
    bool 仅需求位方法头 = false;
    bool 可直接进入学习 = false;
    bool 当前学习方法有动作骨架 = false;
    I64 当前学习方法条件数量 = 0;
    I64 当前学习方法结果数量 = 0;
    bool 当前学习方法可用 = false;
    bool 学习任务兜底方法已可执行 = false;
    bool 当前选中学习方法已可执行 = false;
    bool 已切换到兜底可执行方法 = false;
    bool 已由兜底切换进入采样 = false;
    bool 允许正式资产提交 = false;
    I64 当前安全值 = 0;
    I64 当前服务值 = 0;
    I64 当前风险安全 = 0;
    std::uint64_t 来源最小原语位图 = 0;
    结构_学习目标描述 当前学习目标{};
    std::vector<结构_学习需求描述> 当前学习需求集合{};
    std::vector<结构_学习动作描述> 当前学习动作集合{};
    std::vector<结构_学习原函数需求项> 当前原函数需求集合{};
    结构_候选学习程序摘要 当前候选学习程序{};
    std::string 来源正式原语摘要{};
    std::string 来源主观察特征{};
    std::string 当前治理证据摘要{};
    std::string 当前输入条件主键{};
    std::string 当前结果主键{};
    std::string 最近抽象因果摘要{};
    std::string 最近抽象因果主键{};
    std::string 当前影子验证状态{};
    std::string 学习承接摘要{};
    std::string 方法服务范围{};
    std::string 学习方法选项摘要{};
    std::string 当前学习方法选择摘要{};
    std::string 学习反馈摘要{};
    std::string 回流目标摘要{};
    std::string 最近反馈摘要{};
};

struct 结构_学习任务实体 {
    std::uint64_t 账本ID = 0;
    时间戳 最近更新时间 = 0;
    std::string 去重键{};
    std::string 可合并键{};
    枚举_学习阶段 当前阶段 = 枚举_学习阶段::未定义;
    std::uintptr_t 当前阶段抽象特征指针 = 0;
    I64 当前阶段值 = 0;
    枚举_学习状态 当前状态 = 枚举_学习状态::未定义;
    std::uintptr_t 当前状态抽象特征指针 = 0;
    I64 当前状态值 = 0;
    枚举_学习等待原因 当前等待原因 = 枚举_学习等待原因::无;
    std::uintptr_t 当前等待原因抽象特征指针 = 0;
    I64 当前等待原因值 = 0;
    枚举_学习影响面 当前影响面 = 枚举_学习影响面::未定义;
    std::uintptr_t 当前影响面抽象特征指针 = 0;
    I64 当前影响面值 = 0;
    枚举_学习结果状态 当前结果状态 = 枚举_学习结果状态::未定义;
    std::uintptr_t 当前结果状态抽象特征指针 = 0;
    I64 当前结果状态值 = 0;
    枚举_学习外层裁决 当前外层裁决 = 枚举_学习外层裁决::未定义;
    std::uintptr_t 当前外层裁决抽象特征指针 = 0;
    I64 当前外层裁决值 = 0;
    枚举_学习执行策略 当前执行策略 = 枚举_学习执行策略::未定义;
    std::uintptr_t 当前执行策略抽象特征指针 = 0;
    I64 当前执行策略值 = 0;
    I64 当前优先级 = 0;
    I64 当前综合分 = 0;
    I64 重试次数 = 0;
    I64 审计次数 = 0;
    I64 外层裁决次数 = 0;
    I64 当前安全值 = 0;
    I64 当前服务值 = 0;
    I64 当前风险安全 = 0;
    I64 受控最大幅度 = 0;
    I64 受控最大时长 = 0;
    I64 受控最大次数 = 0;

    任务类::节点类* 学习子任务 = nullptr;
    方法类::节点类* 学习方法 = nullptr;
    方法类::节点类* 学习任务兜底方法 = nullptr;
    方法类::节点类* 当前选中学习方法 = nullptr;
    任务类::节点类* 来源宿主任务 = nullptr;
    任务类::节点类* 来源步骤 = nullptr;
    任务类::节点类* 来源结果 = nullptr;
    方法类::节点类* 来源方法 = nullptr;

    枚举_任务管理根层重判结果 最近根层重判结果 = static_cast<枚举_任务管理根层重判结果>(0);
    枚举_任务管理缺口类型 当前缺口类型 = static_cast<枚举_任务管理缺口类型>(0);
    枚举_任务管理下一步去向 当前下一步去向 = static_cast<枚举_任务管理下一步去向>(0);
    枚举_任务管理方法来源 当前方法来源 = static_cast<枚举_任务管理方法来源>(0);
    枚举_任务管理触发来源 当前触发来源 = static_cast<枚举_任务管理触发来源>(0);
    枚举_任务管理学习承接类型 当前学习承接类型 = static_cast<枚举_任务管理学习承接类型>(0);
    枚举_任务管理方法需求位 来源方法需求位 = static_cast<枚举_任务管理方法需求位>(0);
    枚举_任务管理本能能力缺口类型 来源本能能力缺口类型 = static_cast<枚举_任务管理本能能力缺口类型>(0);
    枚举_任务管理方法需求位 方法服务目标 = static_cast<枚举_任务管理方法需求位>(0);
    枚举_任务管理学习反馈消费策略 学习情况消费策略 = static_cast<枚举_任务管理学习反馈消费策略>(0);
    枚举_任务管理方法需求位 回流目标方法位 = static_cast<枚举_任务管理方法需求位>(0);

    bool 仅需求位方法头 = false;
    bool 可直接进入学习 = false;
    bool 当前学习方法有动作骨架 = false;
    I64 当前学习方法条件数量 = 0;
    I64 当前学习方法结果数量 = 0;
    bool 当前学习方法可用 = false;
    bool 学习任务兜底方法已可执行 = false;
    bool 当前选中学习方法已可执行 = false;
    bool 已切换到兜底可执行方法 = false;
    bool 已由兜底切换进入采样 = false;
    bool 允许正式资产提交 = false;
    bool 待提交前审计 = false;
    bool 待外层裁决 = false;
    bool 受控微试探放行 = false;
    bool 本轮新建 = false;
    bool 本轮更新 = false;
    std::uint64_t 来源最小原语位图 = 0;
    结构_学习目标描述 当前学习目标{};
    std::vector<结构_学习需求描述> 当前学习需求集合{};
    std::vector<结构_学习动作描述> 当前学习动作集合{};
    std::vector<结构_学习原函数需求项> 当前原函数需求集合{};
    结构_候选学习程序摘要 当前候选学习程序{};
    std::string 来源正式原语摘要{};

    std::string 学习子任务标题{};
    std::string 学习方法标题{};
    std::string 学习任务兜底方法标题{};
    std::string 当前选中学习方法标题{};
    std::string 来源宿主任务标题{};
    std::string 来源步骤标题{};
    std::string 来源结果标题{};
    std::string 来源方法标题{};
    std::string 来源主观察特征{};
    std::string 当前治理证据摘要{};
    std::string 当前输入条件主键{};
    std::string 当前结果主键{};
    std::string 最近抽象因果摘要{};
    std::string 最近抽象因果主键{};
    std::string 当前影子验证状态{};
    std::string 学习承接摘要{};
    std::string 方法服务范围{};
    std::string 学习方法选项摘要{};
    std::string 当前学习方法选择摘要{};
    std::string 学习反馈摘要{};
    std::string 回流目标摘要{};
    std::string 对象来源摘要{};
    std::string 待学习方法骨架摘要{};
    std::string 最近执行摘要{};
    std::string 最近失败摘要{};
    std::string 最近回流摘要{};
    std::string 最近审计摘要{};
    std::string 最近外层裁决摘要{};
    std::string 受控作用对象范围{};
    std::string 受控停止条件{};
    std::string 结果状态权重摘要{};
    std::string 最近试探摘要{};
    std::string 最近反馈摘要{};
    std::string 摘要{};
};

struct 结构_学习账本快照 {
    std::size_t 总数 = 0;
    std::size_t 活跃数 = 0;
    std::size_t 挂起数 = 0;
    std::size_t 可调度数 = 0;
    std::size_t 执行中数 = 0;
    std::size_t 已完成数 = 0;
    std::size_t 因兜底切换进入采样数 = 0;
    std::string 最近摘要{};
    std::string 首个兜底切换采样摘要{};
    std::vector<结构_学习任务实体> 任务列表{};
};

struct 结构_学习任务更新输入 {
    std::uint64_t 账本ID = 0;
    时间戳 当前时间 = 0;

    bool 覆写阶段 = false;
    枚举_学习阶段 当前阶段 = 枚举_学习阶段::未定义;

    bool 覆写状态 = false;
    枚举_学习状态 当前状态 = 枚举_学习状态::未定义;

    bool 覆写等待原因 = false;
    枚举_学习等待原因 当前等待原因 = 枚举_学习等待原因::无;

    bool 覆写可合并键 = false;
    std::string 可合并键{};

    bool 覆写当前优先级 = false;
    I64 当前优先级 = 0;

    bool 覆写仅需求位方法头 = false;
    bool 仅需求位方法头 = false;

    bool 覆写可直接进入学习 = false;
    bool 可直接进入学习 = false;

    bool 覆写已由兜底切换进入采样 = false;
    bool 已由兜底切换进入采样 = false;

    bool 累加重试次数 = false;
    I64 重试次数增量 = 1;

    bool 覆写待提交前审计 = false;
    bool 待提交前审计 = false;

    bool 累加审计次数 = false;
    I64 审计次数增量 = 1;

    bool 覆写待外层裁决 = false;
    bool 待外层裁决 = false;

    bool 覆写当前外层裁决 = false;
    枚举_学习外层裁决 当前外层裁决 = 枚举_学习外层裁决::未定义;

    bool 覆写学习情况消费策略 = false;
    枚举_任务管理学习反馈消费策略 学习情况消费策略 =
        static_cast<枚举_任务管理学习反馈消费策略>(0);

    bool 累加外层裁决次数 = false;
    I64 外层裁决次数增量 = 1;

    bool 覆写对象来源摘要 = false;
    std::string 对象来源摘要{};

    bool 覆写待学习方法骨架摘要 = false;
    std::string 待学习方法骨架摘要{};

    bool 覆写方法服务范围 = false;
    std::string 方法服务范围{};

    bool 覆写学习方法选项摘要 = false;
    std::string 学习方法选项摘要{};

    bool 覆写当前学习方法选择摘要 = false;
    std::string 当前学习方法选择摘要{};

    bool 覆写最近执行摘要 = false;
    std::string 最近执行摘要{};

    bool 覆写最近失败摘要 = false;
    std::string 最近失败摘要{};

    bool 覆写学习反馈摘要 = false;
    std::string 学习反馈摘要{};

    bool 覆写最近回流摘要 = false;
    std::string 最近回流摘要{};

    bool 覆写最近审计摘要 = false;
    std::string 最近审计摘要{};

    bool 覆写最近外层裁决摘要 = false;
    std::string 最近外层裁决摘要{};

    bool 覆写最近试探摘要 = false;
    std::string 最近试探摘要{};
};

namespace 学习任务模块 {

const char* 学习影响面文本(枚举_学习影响面 影响面) noexcept;
const char* 学习结果状态文本(枚举_学习结果状态 状态) noexcept;
const char* 学习外层裁决文本(枚举_学习外层裁决 裁决) noexcept;
const char* 学习执行策略文本(枚举_学习执行策略 策略) noexcept;

bool 创建或更新学习任务(
    const 结构_学习任务同步输入& 输入,
    结构_学习任务实体* 输出 = nullptr) noexcept;

bool 读取学习任务实体(
    std::uint64_t 账本ID,
    结构_学习任务实体* 输出) noexcept;

bool 更新学习任务实体(
    const 结构_学习任务更新输入& 输入,
    结构_学习任务实体* 输出 = nullptr) noexcept;

bool 读取学习账本快照(
    结构_学习账本快照* 输出) noexcept;

std::vector<结构_学习任务实体> 枚举学习任务实体();

I64 统计活跃学习任务数量() noexcept;

} // namespace 学习任务模块

} // export
