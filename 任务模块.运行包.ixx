module;

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "语素信息类型.h"

export module 任务模块.运行包;

import 自我模块.特征定义;
import 任务管理任务模块.消息协议;

export namespace 任务运行包 {

using 主键 = std::uint64_t;
using 时间戳 = std::uint64_t;
using I64 = std::int64_t;

enum class 枚举_包状态 : std::uint8_t {
    草稿 = 0,
    已封口 = 1,
};

enum class 枚举_需求大类 : std::uint8_t {
    服务 = 0,
    安全 = 1,
};

enum class 枚举_服务子类 : std::uint8_t {
    无 = 0,
    直接服务 = 1,
    准备性服务 = 2,
};

enum class 枚举_安全子类 : std::uint8_t {
    无 = 0,
    物理安全 = 1,
    风险安全 = 2,
};

enum class 枚举_动作类型 : std::uint8_t {
    观察 = 0,
    解释 = 1,
    预警 = 2,
    建议 = 3,
    执行 = 4,
    回滚 = 5,
    收束 = 6,
};

enum class 枚举_结算结论 : std::uint8_t {
    无效推进 = 0,
    缩差 = 1,
    持平 = 2,
    恶化 = 3,
    越界 = 4,
};

enum class 枚举_学习样本类别 : std::uint8_t {
    正样本 = 0,
    负样本 = 1,
    待验证 = 2,
    黑名单 = 3,
};

struct 结构_线程运行包头 {
    主键 包主键 = 0;
    主键 线程主键 = 0;
    主键 轮次主键 = 0;
    主键 宿主存在主键 = 0;
    主键 父包主键 = 0;
    时间戳 创建时间 = 0;
    时间戳 封口时间 = 0;
    std::uint64_t 追踪签名 = 0;
    枚举_包状态 包状态 = 枚举_包状态::草稿;
    const 词性节点类* 包状态抽象特征指针 = nullptr;
    I64 包状态值 = static_cast<I64>(枚举_包状态::草稿);
};

struct 结构_差额值 {
    I64 目标值 = 0;
    I64 当前值 = 0;
    I64 差额 = 0;
    I64 趋势 = 0;
};

struct 结构_根需求项 {
    主键 需求节点主键 = 0;
    枚举_需求大类 大类 = 枚举_需求大类::服务;
    枚举_服务子类 服务子类 = 枚举_服务子类::无;
    枚举_安全子类 安全子类 = 枚举_安全子类::无;
    const 词性节点类* 需求大类抽象特征指针 = nullptr;
    I64 需求大类值 = static_cast<I64>(枚举_需求大类::服务);
    const 词性节点类* 服务子类抽象特征指针 = nullptr;
    I64 服务子类值 = static_cast<I64>(枚举_服务子类::无);
    const 词性节点类* 安全子类抽象特征指针 = nullptr;
    I64 安全子类值 = static_cast<I64>(枚举_安全子类::无);
    I64 紧迫度 = 0;
    I64 优先级 = 0;
    std::string 触发原因{};
};

struct 结构_授权约束段 {
    I64 授权等级 = 0;
    I64 资源配额 = 0;
    I64 时间窗_ms = 0;
    I64 并发上限 = 1;
    bool 允许真实写回 = false;
    bool 允许回滚 = false;
    bool 必须人工确认 = false;
    std::vector<枚举_动作类型> 动作白名单{};
    const 词性节点类* 动作白名单抽象特征指针 = nullptr;
    std::vector<I64> 动作白名单值集{};
    std::vector<std::string> 停止条件{};
};

struct 结构_方法候选项 {
    主键 方法主键 = 0;
    I64 置信度 = 0;
    std::string 命中原因{};
};

struct 结构_状态变化项 {
    主键 主体主键 = 0;
    主键 特征主键 = 0;
    I64 变化前 = 0;
    I64 变化后 = 0;
    std::string 变化原因{};
};

struct 结构_学习候选项 {
    主键 锚点主键 = 0;
    主键 来源任务主键 = 0;
    主键 来源方法主键 = 0;
    std::string 动作语义键{};
    I64 置信度 = 0;
    枚举_学习样本类别 样本类别 = 枚举_学习样本类别::待验证;
    const 词性节点类* 学习样本类别抽象特征指针 = nullptr;
    I64 学习样本类别值 = static_cast<I64>(枚举_学习样本类别::待验证);
    bool 已验证 = false;
};

struct 结构_一步治理任务包 {
    结构_线程运行包头 包头{};
    主键 来源需求主键 = 0;
    主键 来源任务主键 = 0;
    枚举_需求大类 来源需求大类 = 枚举_需求大类::服务;
    const 词性节点类* 来源需求大类抽象特征指针 = nullptr;
    I64 来源需求大类值 = static_cast<I64>(枚举_需求大类::服务);
    std::string 本轮要缩小的差额说明{};
    枚举_动作类型 动作类型 = 枚举_动作类型::解释;
    const 词性节点类* 动作类型抽象特征指针 = nullptr;
    I64 动作类型值 = static_cast<I64>(枚举_动作类型::解释);
    std::string 任务语义键{};
    主键 目标主体主键 = 0;
    主键 目标场景主键 = 0;
    std::string 完成条件{};
    std::string 预期结果{};
    结构_授权约束段 授权约束{};
    std::vector<结构_方法候选项> 候选方法集{};
    std::optional<结构_方法候选项> 选定方法{};
    std::string 高阶排序需求轴键{};
    std::string 高阶排序胜出候选主键{};
    主键 高阶排序胜出方法主键 = 0;
    bool 已显式消费高阶排序绑定 = false;
    std::string 高阶排序绑定摘要{};
    std::string 控制请求主键{};
    枚举_任务管理控制意图值 控制意图值 = 枚举_任务管理控制意图值::未定义;
    const 词性节点类* 控制意图抽象特征指针 = nullptr;
    I64 控制意图值镜像 = static_cast<I64>(枚举_任务管理控制意图值::未定义);
    bool 控制意图需要确认 = false;
    std::string 控制意图摘要{};
    std::vector<主键> 预期输入状态主键集{};
    std::vector<主键> 预期输出状态主键集{};
    bool 要求记录动态 = true;
    bool 要求记录副作用 = true;
    bool 要求记录失败原因 = true;
    I64 安全监测等级 = 0;
};

struct 结构_回流结算包 {
    结构_线程运行包头 包头{};
    主键 任务包主键 = 0;
    主键 选定方法主键 = 0;
    主键 输入场景主键 = 0;
    主键 输出场景主键 = 0;
    bool 执行成功 = false;
    bool 被中断 = false;
    bool 已超时 = false;
    I64 错误码 = 0;
    std::uint64_t 动态路径签名 = 0;
    std::vector<结构_状态变化项> 状态变化集{};
    std::vector<主键> 关系变化主键集{};
    std::vector<主键> 副作用状态主键集{};
    bool 触发降级 = false;
    bool 触发回滚 = false;
    bool 触发收束 = false;
    std::string 止损原因{};
    结构_差额值 服务差额结算前{};
    结构_差额值 服务差额结算后{};
    结构_差额值 物理安全差额结算前{};
    结构_差额值 物理安全差额结算后{};
    结构_差额值 风险安全差额结算前{};
    结构_差额值 风险安全差额结算后{};
    枚举_结算结论 结算结论 = 枚举_结算结论::无效推进;
    const 词性节点类* 结算结论抽象特征指针 = nullptr;
    I64 结算结论值 = static_cast<I64>(枚举_结算结论::无效推进);
    bool 是否有效推进 = false;
    std::vector<结构_学习候选项> 学习候选集{};
    bool 建议进入待机 = false;
    bool 建议进入收束 = false;
    bool 建议停止 = false;
    bool 建议死亡退出 = false;
    bool 建议重试 = false;
};

bool 包已封口(const 结构_线程运行包头& 包头) noexcept;
bool 校验包头(const 结构_线程运行包头& 包头, std::string* 错误 = nullptr) noexcept;
bool 校验任务包(const 结构_一步治理任务包& 包, std::string* 错误 = nullptr) noexcept;
bool 校验结算包(const 结构_回流结算包& 包, std::string* 错误 = nullptr) noexcept;
void 同步运行包语义槽(结构_线程运行包头& 包头) noexcept;
void 同步运行包语义槽(结构_根需求项& 根需求) noexcept;
void 同步运行包语义槽(结构_授权约束段& 授权约束) noexcept;
void 同步运行包语义槽(结构_学习候选项& 学习候选) noexcept;
void 同步运行包语义槽(结构_一步治理任务包& 包) noexcept;
void 同步运行包语义槽(结构_回流结算包& 包) noexcept;
bool 封口任务包(结构_一步治理任务包& 包, 时间戳 封口时间 = 0, std::string* 错误 = nullptr) noexcept;
bool 封口结算包(结构_回流结算包& 包, 时间戳 封口时间 = 0, std::string* 错误 = nullptr) noexcept;

} // namespace 任务运行包
