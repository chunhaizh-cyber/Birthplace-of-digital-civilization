module;

#include <atomic>
#include <cstdint>
#include <string>

#include "世界树类.h"

export module 自我模块;

export import 自我模块.特征定义;

export class 自我类;

export namespace 自我初始化模块 {

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

bool 执行自我初始化(::自我类& 自我对象, const std::string& 调用点, 结构_自我初始化结果* 输出) noexcept;
结构_自我初始化结果 读取自我初始化结果(const ::自我类& 自我对象) noexcept;

} // namespace 自我初始化模块

export struct 结构_线程存在初始化参数 {
    std::string 线程标签{};
    std::string 线程类型{};
    bool 是工作线程 = false;
    bool 是自我线程 = false;
    std::string 所属线程池标签{};
    std::string 所属线程池类型{};
};

export class 自我类 {
public:
    struct 结构_根任务权重 {
        std::int64_t 安全 = 10000;
        std::int64_t 服务 = 10000;
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
    存在节点类* 确保线程池存在(
        const std::string& 线程池标签,
        const std::string& 线程池类型 = "线程池",
        const std::string& 调用点 = "自我类::确保线程池存在") noexcept;
    存在节点类* 确保线程子存在(
        const 结构_线程存在初始化参数& 参数,
        const std::string& 调用点 = "自我类::确保线程子存在") noexcept;

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

    bool 应用自我需求安全值增加(
        I64 delta = 0,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用自我需求安全值增加"
    ) noexcept;
    bool 应用自我需求安全值减少(
        I64 delta = 0,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用自我需求安全值减少"
    ) noexcept;
    bool 应用自我需求安全值保持(
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用自我需求安全值保持"
    ) noexcept;
    bool 应用自我需求服务值增加(
        I64 delta = 0,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用自我需求服务值增加"
    ) noexcept;
    bool 应用自我需求服务值减少(
        I64 delta = 0,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用自我需求服务值减少"
    ) noexcept;
    bool 应用自我需求服务值保持(
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用自我需求服务值保持"
    ) noexcept;
    bool 应用安全服务闭环安全推进(
        I64 delta = 0,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用安全服务闭环安全推进"
    ) noexcept;
    bool 应用安全服务闭环服务推进(
        I64 delta = 0,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用安全服务闭环服务推进"
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
    bool 更新待处理方法数量(
        I64 数量,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::更新待处理方法数量"
    ) noexcept;
    bool 增加可用方法数量(
        I64 增量 = 1,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::增加可用方法数量"
    ) noexcept;
    bool 更新方法补齐状态(
        bool 正在方法补齐,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::更新方法补齐状态"
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
    void 初始化常用抽象特征_(const std::string& 调用点) noexcept;
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

export extern 自我类 自我;
