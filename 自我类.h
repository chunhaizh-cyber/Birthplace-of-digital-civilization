#pragma once

#include <atomic>
#include <cstdint>
#include <string>

#include "世界树类.h"
#include "自我特征定义.h"

class 自我类;

namespace 自我初始化模块 {
    struct 结构_自我初始化结果;
    bool 执行自我初始化(自我类& 自我对象, const std::string& 调用点, 结构_自我初始化结果* 输出) noexcept;
    结构_自我初始化结果 读取自我初始化结果(const 自我类& 自我对象) noexcept;
}

class 自我类 {
public:
    struct 结构_根任务权重 {
        std::int64_t 安全 = 10000;
        std::int64_t 服务 = 10000;
        std::int64_t 学习 = 5000;
    };

public:
    自我类();
    ~自我类();

    自我类(const 自我类&) = delete;
    自我类& operator=(const 自我类&) = delete;
    自我类(自我类&&) = delete;
    自我类& operator=(自我类&&) = delete;

    void 初始化自我(const std::string& 调用点 = "自我类::初始化自我");
    bool 已初始化() const noexcept;

    场景节点类* 获取自我现实场景() const noexcept;
    场景节点类* 获取自我内部世界() const noexcept;
    存在节点类* 获取自我存在() const noexcept;

    场景节点类* 确保自我现实场景(const std::string& 调用点 = "自我类::确保自我现实场景");
    场景节点类* 确保自我内部世界(const std::string& 调用点 = "自我类::确保自我内部世界");
    存在节点类* 确保自我存在(const std::string& 调用点 = "自我类::确保自我存在");

    I64 获取安全值() const noexcept;
    I64 获取服务值() const noexcept;
    I64 获取物理安全值() const noexcept;
    I64 获取风险安全值() const noexcept;
    bool 设置物理安全值(
        I64 值,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::设置物理安全值"
    ) noexcept;

    bool 是否已消亡() const noexcept;
    bool 是否待机状态() const noexcept;

    I64 根目标安全值() const noexcept;
    I64 根目标服务值() const noexcept;
    I64 安全根方向差值() const noexcept;

    I64 时序正向步长() const noexcept;
    I64 时序反向步长() const noexcept;
    I64 服务时序衰减步长() const noexcept;
    void 设置服务时序衰减步长(I64 步长) noexcept;

    bool 读取自我I64特征当前值(
        const 词性节点类* 特征类型,
        I64& 输出值,
        const std::string& 调用点 = "自我类::读取自我I64特征当前值"
    ) const noexcept;

    bool 写入自我I64特征当前值(
        const 词性节点类* 特征类型,
        I64 值,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::写入自我I64特征当前值"
    ) noexcept;

    bool 读取自我指针特征当前值(
        const 词性节点类* 特征类型,
        std::uintptr_t& 输出值,
        const std::string& 调用点 = "自我类::读取自我指针特征当前值"
    ) const noexcept;

    bool 写入自我指针特征当前值(
        const 词性节点类* 特征类型,
        const void* 指针值,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::写入自我指针特征当前值"
    ) noexcept;

    需求类::节点类* 获取安全根需求() const noexcept;
    需求类::节点类* 获取服务根需求() const noexcept;
    任务类::节点类* 获取安全根任务() const noexcept;
    任务类::节点类* 获取服务根任务() const noexcept;
    任务类::节点类* 获取任务管理任务() const noexcept;

    需求类::节点类* 获取当前主需求() const noexcept;
    任务类::节点类* 获取当前主任务() const noexcept;
    方法类::节点类* 获取当前主方法() const noexcept;

    bool 设置当前主需求(
        需求类::节点类* 节点,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::设置当前主需求"
    ) noexcept;
    bool 设置当前主任务(
        任务类::节点类* 节点,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::设置当前主任务"
    ) noexcept;
    bool 设置当前主方法(
        方法类::节点类* 节点,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::设置当前主方法"
    ) noexcept;

    void 安全值增加(
        I64 delta,
        const std::string& 原因类别 = "安全值增加",
        const std::string& 原因说明 = {}
    ) noexcept;
    void 安全值减少(
        I64 delta,
        const std::string& 原因类别 = "安全值减少",
        const std::string& 原因说明 = {}
    ) noexcept;
    void 服务值增加(
        I64 delta,
        const std::string& 原因类别 = "服务值增加",
        const std::string& 原因说明 = {}
    ) noexcept;
    void 服务值减少(
        I64 delta,
        const std::string& 原因类别 = "服务值减少",
        const std::string& 原因说明 = {}
    ) noexcept;

    bool 应用服务归零待机保护(
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用服务归零待机保护"
    ) noexcept;
    bool 应用定时衰减服务值(
        I64 delta = 0,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用定时衰减服务值"
    ) noexcept;
    bool 应用按双值重算服务衰减步长(
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用按双值重算服务衰减步长"
    ) noexcept;
    bool 应用按服务值重算安全增加步长(
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用按服务值重算安全增加步长"
    ) noexcept;
    bool 应用按服务值重算安全减少步长(
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用按服务值重算安全减少步长"
    ) noexcept;
    bool 应用安全低位回升(
        I64 delta = 0,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用安全低位回升"
    ) noexcept;
    bool 应用安全高位回落(
        I64 delta = 0,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用安全高位回落"
    ) noexcept;
    bool 应用风险安全回归(
        I64 delta = 1,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用风险安全回归"
    ) noexcept;

    结构_根任务权重& 根任务权重() noexcept;
    const 结构_根任务权重& 根任务权重() const noexcept;

    static 自我类* 当前活动自我() noexcept;

private:
    static constexpr I64 生命值上限_() noexcept;
    static constexpr I64 服务低位阈值_() noexcept;
    static constexpr I64 服务高位阈值_() noexcept;
    static constexpr I64 安全稳定带下限值_() noexcept;
    static constexpr I64 安全稳定带上限值_() noexcept;
    static constexpr I64 风险安全回归目标值_() noexcept;

    场景节点类* 确保自我现实场景_(const std::string& 调用点) noexcept;
    场景节点类* 确保自我内部世界_(const std::string& 调用点) noexcept;
    存在节点类* 确保自我存在_(const std::string& 调用点) noexcept;
    特征节点类* 确保自我特征_(const 词性节点类* 特征类型, const std::string& 调用点) noexcept;

    void 确保自我I64特征已初始化_(
        存在节点类* 自我存在,
        const 词性节点类* 特征类型,
        I64 默认值,
        const std::string& 调用点
    ) noexcept;

    void 确保自我指针特征已初始化_(
        存在节点类* 自我存在,
        const 词性节点类* 特征类型,
        std::uintptr_t 默认值,
        const std::string& 调用点
    ) noexcept;

    void 初始化自我特征与默认值_(const std::string& 调用点) noexcept;
    void 从特征恢复运行态_(const std::string& 调用点) noexcept;
    void 确保自我根需求与根任务已初始化_(const std::string& 调用点) noexcept;
    void 确保主链镜像已初始化_(const std::string& 调用点) noexcept;
    需求类::节点类* 确保根需求_(
        存在节点类* 自我存在,
        const 词性节点类* 需求类型,
        I64 默认权重,
        const std::string& 调用点
    ) noexcept;
    任务类::节点类* 确保根任务_(
        存在节点类* 自我存在,
        const 词性节点类* 任务名称,
        const 词性节点类* 任务类型,
        需求类::节点类* 来源需求,
        const std::string& 调用点
    ) noexcept;
    任务类::节点类* 确保任务管理任务_(
        存在节点类* 自我存在,
        需求类::节点类* 当前主需求,
        const std::string& 调用点
    ) noexcept;
    需求类::节点类* 选择当前主需求候选_() const noexcept;
    任务类::节点类* 选择当前主任务候选_(需求类::节点类* 当前主需求) const noexcept;
    void 同步运行态特征_(const std::string& 调用点) noexcept;
    void 重置时序步长为默认值_() noexcept;
    void 重算时序回归步长_按当前双值_() noexcept;
    I64 计算服务衰减步长_按双值_(I64 服务值, I64 安全值) const noexcept;
    I64 计算安全增加步长_按服务值_(I64 服务值) const noexcept;
    I64 计算安全减少步长_按服务值_(I64 服务值) const noexcept;

    I64 读取自我I64特征当前值_(
        const 词性节点类* 特征类型,
        I64 缺省值,
        const std::string& 调用点
    ) const noexcept;

    void 安全值_上升_(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept;
    void 安全值_下降_(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept;
    void 服务值_上升_(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept;
    void 服务值_下降_(I64 delta, const std::string& 原因类别, const std::string& 原因说明) noexcept;

private:
    friend bool 自我初始化模块::执行自我初始化(
        自我类& 自我对象,
        const std::string& 调用点,
        自我初始化模块::结构_自我初始化结果* 输出) noexcept;
    friend 自我初始化模块::结构_自我初始化结果 自我初始化模块::读取自我初始化结果(
        const 自我类& 自我对象) noexcept;

    inline static std::atomic<自我类*> 当前活动自我_{ nullptr };

    std::atomic_bool 已初始化_{ false };
    std::atomic<I64> temporal_forward_step_{ 1 };
    std::atomic<I64> temporal_backward_step_{ 1 };
    std::atomic<I64> service_decay_step_{ 1 };
    std::atomic_bool standby_mode_{ false };
    结构_根任务权重 roots_{};
};

extern 自我类 自我;
