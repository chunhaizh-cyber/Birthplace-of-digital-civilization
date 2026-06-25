#include "任务类.h"

#include <algorithm>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_set>

#include "日志接入.h"
#include "需求类.h"
#include "方法类.h"
#include "世界树类.h"
#include "语素类.h"

import 数据库ADO模块;
import 二次特征应用模块;
import 自我类.特征定义;

namespace {
    using 任务节点 = 任务类::节点类;
    using 方法节点 = 方法类::节点类;

    场景节点类* 私有_推导任务场景(
        需求节点类* 来源需求,
        任务节点* 上级任务节点) noexcept;

    场景节点类* 私有_推导任务虚拟存在承载世界(
        任务节点* 任务头结点,
        场景节点类* 任务场景) noexcept;

    存在节点类* 私有_创建并初始化任务虚拟存在(
        任务节点* 任务头结点,
        场景节点类* 承载场景) noexcept;

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_来源需求是根目标需求(
        需求节点类* 来源需求,
        const 语素入口节点类* 目标特征类型) noexcept
    {
        return 来源需求
            && 来源需求->父
            && 来源需求->父->父 == nullptr
            && 语素入口同一(
                需求类::读取需求目标特征类型(来源需求),
                目标特征类型);
    }

    template<class T节点>
    void 私有_写入引用(可解析引用<T节点>& 输出, T节点* 节点) noexcept
    {
        输出.指针 = 节点;
        输出.主键 = 节点 ? 节点->获取主键() : std::string{};
    }

    // 功能：服务所在模块的内部辅助流程。
    方法节点* 私有_取方法节点(方法节点类* 节点) noexcept
    {
        return reinterpret_cast<方法节点*>(节点);
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_任务方法候选已存在(
        const 存在节点类* 任务虚拟存在,
        const 存在节点类* 方法虚拟存在) noexcept
    {
        if (!任务虚拟存在 || !方法虚拟存在) {
            return false;
        }

        const auto* 特征类型 = 自我特征定义类::类型_任务_方法候选集();
        for (auto* 特征 : 世界树.获取子特征(任务虚拟存在)) {
            const auto* 主信息 = 世界树.取特征主信息(特征);
            if (!主信息 || !语素入口同一(主信息->类型, 特征类型)) {
                continue;
            }

            void* 指针 = nullptr;
            if (世界树.读取特征_指针(特征, 指针)
                && (指针 == 方法虚拟存在
                    || (指针
                        && !方法虚拟存在->获取主键().empty()
                        && reinterpret_cast<存在节点类*>(指针)->获取主键() == 方法虚拟存在->获取主键()))) {
                return true;
            }
        }
        return false;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_存在节点相同(const 存在节点类* 左, const 存在节点类* 右) noexcept
    {
        if (左 == 右) {
            return true;
        }
        return 左
            && 右
            && !左->获取主键().empty()
            && 左->获取主键() == 右->获取主键();
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_追加唯一存在节点(std::vector<存在节点类*>& 列表, 存在节点类* 节点) noexcept
    {
        if (!节点) {
            return false;
        }
        for (const auto* 已有 : 列表) {
            if (私有_存在节点相同(已有, 节点)) {
                return false;
            }
        }
        列表.push_back(节点);
        return true;
    }

    // 功能：构建运行所需的数据结构或中间结果。
    std::vector<存在节点类*> 私有_构建任务虚拟存在概念集(const 任务节点* 任务头结点) noexcept
    {
        std::vector<存在节点类*> 概念集{};
        auto* 任务概念 = 世界树.确保核心概念_任务();

        (void)私有_追加唯一存在节点(概念集, 世界树.确保根概念_存在());
        (void)私有_追加唯一存在节点(概念集, 世界树.确保根概念_虚拟存在());
        (void)私有_追加唯一存在节点(概念集, 任务概念);

        const char* 角色概念名 = "任务实例";
        switch (任务类::读取任务角色(任务头结点, 枚举_任务角色::未定义)) {
        case 枚举_任务角色::需求管理任务:
            角色概念名 = "需求管理任务";
            break;
        case 枚举_任务角色::需求执行任务:
            角色概念名 = "需求执行任务";
            break;
        case 枚举_任务角色::缺口补全任务:
            角色概念名 = "缺口补全任务";
            break;
        default:
            break;
        }
        (void)私有_追加唯一存在节点(
            概念集,
            世界树.确保存在概念(
                语素集.添加信息入口词(角色概念名, 枚举_信息入口类型::任务概念入口),
                任务概念));

        const auto* 泛化任务入口 = 语素集.添加信息入口词("任务", 枚举_信息入口类型::任务概念入口);
        if (任务头结点
            && 任务头结点->主信息.类型
            && !语素入口同一(任务头结点->主信息.类型, 泛化任务入口)) {
            // 任务自身类型是比“任务”更具体的概念入口，用于实例记忆扩充，不替代任务信息节点。
            (void)私有_追加唯一存在节点(
                概念集,
                世界树.确保存在概念(任务头结点->主信息.类型, 任务概念));
        }
        return 概念集;
    }

    // 功能：补齐条件、参数、证据或方法能力缺口。
    bool 私有_补齐任务虚拟存在概念集(
        任务节点* 任务头结点,
        存在节点类* 任务虚拟存在) noexcept
    {
        if (!任务头结点 || !任务虚拟存在) {
            return false;
        }

        bool 全部成功 = true;
        for (auto* 概念 : 私有_构建任务虚拟存在概念集(任务头结点)) {
            全部成功 = 世界树.添加实例概念(任务虚拟存在, 概念, true) && 全部成功;
        }
        return 全部成功;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 方法节点* 私有_取方法节点(const 方法节点类* 节点) noexcept
    {
        return reinterpret_cast<const 方法节点*>(节点);
    }

    template<class T节点>
    可解析引用<T节点> 私有_生成引用(T节点* 节点) noexcept
    {
        可解析引用<T节点> 输出{};
        私有_写入引用(输出, 节点);
        return 输出;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    状态节点类* 私有_解析状态引用(
        const 可解析引用<状态节点类>& 引用) noexcept
    {
        if (引用.指针) {
            return 引用.指针;
        }
        if (!引用.主键.empty()) {
            return 世界树.按主键解析状态节点(引用.主键);
        }
        return nullptr;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    需求节点类* 私有_解析需求引用(
        const 可解析引用<需求节点类>& 引用) noexcept
    {
        return 引用.指针;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    存在节点类* 私有_解析存在引用(
        const 可解析引用<存在节点类>& 引用) noexcept
    {
        if (引用.指针) {
            return 引用.指针;
        }
        if (!引用.主键.empty()) {
            return 世界树.按主键解析存在节点(引用.主键);
        }
        return nullptr;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    场景节点类* 私有_解析场景引用(
        const 可解析引用<场景节点类>& 引用) noexcept
    {
        if (引用.指针) {
            return 引用.指针;
        }
        if (!引用.主键.empty()) {
            return 世界树.按主键解析场景节点(引用.主键);
        }
        return nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    状态节点类* 私有_取需求目标状态(需求节点类* 来源需求) noexcept
    {
        return 需求类::解析需求目标状态(来源需求);
    }

    // 功能：服务所在模块的内部辅助流程。
    状态节点类* 私有_取需求当前状态(需求节点类* 来源需求) noexcept
    {
        return 需求类::解析需求当前状态(来源需求);
    }

    // 功能：服务所在模块的内部辅助流程。
    任务节点* 私有_取独立树根(任务节点* 节点) noexcept
    {
        while (节点 && 节点->父) {
            节点 = static_cast<任务节点*>(节点->父);
        }
        return 节点;
    }

    // 功能：服务所在模块的内部辅助流程。
    任务节点* 私有_归一到任务头结点(任务节点* 节点) noexcept
    {
        auto* 当前 = 节点;
        while (当前) {
            if (任务类::读取任务节点种类(当前) == 枚举_任务节点种类::头结点) {
                return 当前;
            }
            当前 = static_cast<任务节点*>(当前->父);
        }
        return nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    任务节点* 私有_独立树添加子节点(任务节点* 父节点, const 任务主信息类& 主信息)
    {
        if (!父节点) return nullptr;

        auto* 独立根 = 私有_取独立树根(父节点);
        if (!独立根) return nullptr;

        任务类 临时树{};
        auto* 原临时根 = 临时树.根指针;
        临时树.根指针 = 独立根;

        auto* 新节点 = 临时树.添加子节点(父节点, 主信息);

        临时树.根指针 = 原临时根;
        return static_cast<任务节点*>(新节点);
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_默认任务名称词(需求节点类* 来源需求) noexcept
    {
        if (私有_来源需求是根目标需求(来源需求, 自我特征定义类::类型_自我_安全值())) {
            return 语素集.添加信息入口词("安全根任务", 枚举_信息入口类型::任务信息节点入口);
        }
        if (私有_来源需求是根目标需求(来源需求, 自我特征定义类::类型_自我_服务值())) {
            return 语素集.添加信息入口词("服务根任务", 枚举_信息入口类型::任务信息节点入口);
        }
        if (const auto* 目标特征类型 = 需求类::读取需求目标特征类型(来源需求)) {
            return 目标特征类型;
        }
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求任务", 枚举_信息入口类型::任务信息节点入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_默认任务类型词(需求节点类* 来源需求) noexcept
    {
        if (私有_来源需求是根目标需求(来源需求, 自我特征定义类::类型_自我_安全值())
            || 私有_来源需求是根目标需求(来源需求, 自我特征定义类::类型_自我_服务值())) {
            return 语素集.添加信息入口词("根任务", 枚举_信息入口类型::任务概念入口);
        }
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务", 枚举_信息入口类型::任务概念入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_任务虚拟存在类型词() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词(
            "任务虚拟存在",
            枚举_信息入口类型::任务虚拟存在入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_任务虚拟存在名称词(const 任务节点* 节点) noexcept
    {
        const auto 后缀 = 节点 && !节点->获取主键().empty()
            ? 节点->获取主键()
            : std::to_string(reinterpret_cast<std::uintptr_t>(节点));
        return 语素集.添加信息入口词(
            "任务虚拟存在_" + 后缀,
            枚举_信息入口类型::任务虚拟存在入口);
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_任务虚拟存在内部世界名称词(const 任务节点* 节点) noexcept
    {
        (void)节点;
        return 语素集.添加信息入口词("内部世界", 枚举_信息入口类型::场景实例入口);
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务特征账本生命周期() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("特征账本生命周期", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_当前阶段() noexcept
    {
        return 自我特征定义类::类型_任务_当前阶段();
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务状态() noexcept
    {
        return 自我特征定义类::类型_任务_状态();
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务节点种类() noexcept
    {
        return 自我特征定义类::类型_任务_节点种类();
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务基准优先级() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务基准优先级", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务局部优先级偏移() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务局部优先级偏移", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务调度优先级() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务调度优先级", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务承接安全权重上限() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务承接安全权重上限", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务承接服务权重上限() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务承接服务权重上限", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务权重评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务权重评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务权重版本号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务权重版本号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务已折算安全值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务已折算安全值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务已折算服务值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务已折算服务值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：执行对应模块、线程或方法的运行逻辑。
    const 语素入口节点类* 私有_特征_任务运行次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务运行次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务成功次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务成功次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务失败次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务失败次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务连续失败次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务连续失败次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务转入缺口需求次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务转入缺口需求次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务最近错误码() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务最近错误码", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：服务所在模块的内部辅助流程。
    枚举_任务阶段 私有_推导任务阶段(枚举_任务状态 状态) noexcept
    {
        switch (状态) {
        case 枚举_任务状态::完成:
        case 枚举_任务状态::已结算:
            return 枚举_任务阶段::已完成;
        case 枚举_任务状态::执行中:
        case 枚举_任务状态::运行中:
        case 枚举_任务状态::就绪:
        case 枚举_任务状态::排队中:
            return 枚举_任务阶段::执行;
        case 枚举_任务状态::挂起:
        case 枚举_任务状态::失败:
        case 枚举_任务状态::取消:
        case 枚举_任务状态::超时:
        case 枚举_任务状态::等待中:
        case 枚举_任务状态::无法执行:
            return 枚举_任务阶段::阻塞;
        case 枚举_任务状态::未启动:
        case 枚举_任务状态::筹办中:
        case 枚举_任务状态::待重筹办:
        case 枚举_任务状态::未定义:
        default:
            return 枚举_任务阶段::筹办;
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_任务特征使用统计(const char* 后缀) noexcept
    {
        return 语素集.添加信息入口词(后缀 ? 后缀 : "特征使用统计", 枚举_信息入口类型::特征模板入口);
    }

    // 功能：初始化相关对象、状态或运行上下文。
    bool 私有_确保I64特征已初始化(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        I64 默认值,
        时间戳 now) noexcept
    {
        if (!宿主 || !特征类型) return false;
        auto* 特征节点 = 世界树.确保特征(宿主, 特征类型);
        if (!特征节点) return false;

        I64 当前值 = 0;
        if (世界树.读取特征_I64(特征节点, 当前值)) {
            return true;
        }
        return 世界树.写入特征_I64(特征节点, 默认值, now);
    }

    // 功能：把处理结果写入指定对象、场景或日志。
    bool 私有_写入任务特征使用统计(
        存在节点类* 任务虚拟存在,
        const 结构_特征使用统计& 统计,
        枚举_特征生命周期 生命周期,
        时间戳 now) noexcept
    {
        if (!任务虚拟存在) return false;

        bool 已变更 = false;
        const auto 写I64 = [&](const char* 后缀, I64 值) {
            已变更 = 世界树.写入特征_I64(
                reinterpret_cast<基础信息节点类*>(任务虚拟存在),
                私有_特征_任务特征使用统计(后缀),
                值,
                now) || 已变更;
        };

        写I64("暴露次数", static_cast<I64>(统计.暴露次数));
        写I64("读取次数", static_cast<I64>(统计.读取次数));
        写I64("有效使用次数", static_cast<I64>(统计.有效使用次数));
        写I64("历史使用次数", static_cast<I64>(统计.历史使用次数));
        写I64("近期使用次数", static_cast<I64>(统计.近期使用次数));
        写I64("作为决策输入次数", static_cast<I64>(统计.作为决策输入次数));
        写I64("作为派生父特征次数", static_cast<I64>(统计.作为派生父特征次数));
        写I64("作为需求触发次数", static_cast<I64>(统计.作为需求触发次数));
        写I64("作为风险闸门次数", static_cast<I64>(统计.作为风险闸门次数));
        写I64("作为验证信号次数", static_cast<I64>(统计.作为验证信号次数));
        写I64("作为解释依据次数", static_cast<I64>(统计.作为解释依据次数));
        写I64("结果改变次数", static_cast<I64>(统计.结果改变次数));
        写I64("坏结果次数", static_cast<I64>(统计.坏结果次数));
        写I64("下游依赖特征数", static_cast<I64>(统计.下游依赖特征数));
        写I64("最近使用时间", static_cast<I64>(统计.最近使用时间));
        写I64("最近有效使用时间", static_cast<I64>(统计.最近有效使用时间));

        已变更 = 世界树.写入特征_I64(
            reinterpret_cast<基础信息节点类*>(任务虚拟存在),
            私有_特征_任务特征账本生命周期(),
            static_cast<I64>(生命周期),
            now) || 已变更;
        return 已变更;
    }

    // 功能：把处理结果写入指定对象、场景或日志。
    bool 私有_写入任务运行镜像特征(
        存在节点类* 任务虚拟存在,
        const 任务主信息类& 主信息,
        枚举_任务状态 状态,
        枚举_任务节点种类 节点种类,
        时间戳 now) noexcept
    {
        if (!任务虚拟存在) return false;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        auto* 宿主 = reinterpret_cast<基础信息节点类*>(任务虚拟存在);
        bool 已写入 = false;
        const auto 写I64 = [&](const 语素入口节点类* 特征类型, I64 值) {
            已写入 = 世界树.写入特征_I64(宿主, 特征类型, 值, now) || 已写入;
        };

        写I64(私有_特征_当前阶段(), static_cast<I64>(私有_推导任务阶段(状态)));
        写I64(私有_特征_任务状态(), static_cast<I64>(状态));
        写I64(私有_特征_任务节点种类(), static_cast<I64>(节点种类));
        return 已写入;
    }

    // 功能：从指定来源读取数据或状态。
    I64 私有_读取任务I64特征(
        const 任务节点* 任务头结点,
        const 语素入口节点类* 特征类型,
        I64 默认值,
        场景节点类* 承载场景) noexcept
    {
        if (!任务头结点 || !特征类型) {
            return 默认值;
        }
        auto* 可写任务 = const_cast<任务节点*>(任务头结点);
        auto* 头结点 = 私有_归一到任务头结点(可写任务);
        if (!头结点) {
            return 默认值;
        }
        (void)承载场景;
        auto* 任务虚拟存在 = reinterpret_cast<存在节点类*>(头结点->主信息.任务虚拟存在.指针);
        I64 当前值 = 默认值;
        return 任务虚拟存在 && 世界树.读取特征_I64(任务虚拟存在, 特征类型, 当前值)
            ? 当前值
            : 默认值;
    }

    // 功能：把处理结果写入指定对象、场景或日志。
    bool 私有_写入任务I64特征(
        任务节点* 任务头结点,
        const 语素入口节点类* 特征类型,
        I64 值,
        场景节点类* 承载场景,
        时间戳 now) noexcept
    {
        if (!任务头结点 || !特征类型) {
            return false;
        }
        if (now == 0) {
            now = 结构体_时间戳::当前_微秒();
        }
        return 任务类::记录任务虚拟存在特征(
            任务头结点,
            特征类型,
            特征值{ 值 },
            承载场景,
            now);
    }

    // 功能：把处理结果写入指定对象、场景或日志。
    bool 私有_写入任务指针特征(
        任务节点* 任务头结点,
        const 语素入口节点类* 特征类型,
        std::uintptr_t 指针值,
        场景节点类* 承载场景,
        时间戳 now) noexcept
    {
        if (!任务头结点 || !特征类型 || 指针值 == 0) {
            return false;
        }
        if (now == 0) {
            now = 结构体_时间戳::当前_微秒();
        }
        return 任务类::记录任务虚拟存在特征(
            任务头结点,
            特征类型,
            特征值{ 指针句柄{ 指针值 } },
            承载场景,
            now);
    }

    // 功能：从指定来源读取数据或状态。
    void* 私有_读取任务指针特征(
        const 任务节点* 任务头结点,
        const 语素入口节点类* 特征类型) noexcept
    {
        if (!任务头结点 || !特征类型) {
            return nullptr;
        }
        auto* 头结点 = 私有_归一到任务头结点(const_cast<任务节点*>(任务头结点));
        if (!头结点) {
            return nullptr;
        }
        auto* 任务虚拟存在 = reinterpret_cast<存在节点类*>(头结点->主信息.任务虚拟存在.指针);
        void* 指针 = nullptr;
        return 任务虚拟存在
            && 世界树.读取特征_指针(
                任务虚拟存在,
                特征类型,
                指针)
            ? 指针
            : nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    场景节点类* 私有_推导任务场景(
        需求节点类* 来源需求,
        任务节点* 上级任务节点) noexcept
    {
        if (上级任务节点 && 上级任务节点->主信息.场景.指针) {
            return 上级任务节点->主信息.场景.指针;
        }
        if (auto* 需求场景 = 需求类::解析需求场景(来源需求)) {
            return 需求场景;
        }
        if (!世界树.自我所在场景 && !世界树.虚拟世界) {
            世界树.初始化默认世界();
        }
        if (世界树.自我所在场景) {
            return 世界树.自我所在场景;
        }
        return 世界树.虚拟世界;
    }

    // 功能：服务所在模块的内部辅助流程。
    场景节点类* 私有_推导任务虚拟存在承载世界(
        任务节点* 任务头结点,
        场景节点类* 任务场景) noexcept
    {
        (void)任务头结点;

        if (任务场景) {
            if (auto* 宿主存在 = 世界树.读取场景宿主存在(任务场景)) {
                if (auto* 内部世界 = 世界树.取或创建存在内部世界(宿主存在)) {
                    return 内部世界;
                }
            }
        }

        if (auto* 自我存在 = 世界树.取或创建自我存在()) {
            if (auto* 自我内部世界 = 世界树.取或创建存在内部世界(自我存在)) {
                return 自我内部世界;
            }
        }

        if (!世界树.虚拟世界) {
            世界树.初始化默认世界();
        }
        return 世界树.虚拟世界;
    }

    // 功能：服务所在模块的内部辅助流程。
    I64 私有_推导基准优先级(需求节点类* 来源需求, 任务节点* 父任务头结点) noexcept
    {
        if (来源需求) {
            const auto 服务权重 = 需求类::读取需求服务权重(来源需求);
            if (服务权重 != 0) {
                return 服务权重;
            }
            const auto 安全权重 = 需求类::读取需求安全权重(来源需求);
            if (安全权重 != 0) {
                return 安全权重;
            }
        }
        return 父任务头结点 ? 任务类::读取任务基准优先级(父任务头结点) : 0;
    }

    // 功能：初始化相关对象、状态或运行上下文。
    存在节点类* 私有_创建并初始化任务虚拟存在(
        任务节点* 任务头结点,
        场景节点类* 承载场景) noexcept
    {
        if (!任务头结点) {
            return nullptr;
        }

        if (任务头结点->主信息.任务虚拟存在.指针) {
            return 任务头结点->主信息.任务虚拟存在.指针;
        }

        auto* 场景 = 承载场景 ? 承载场景
            : 私有_推导任务场景(任务头结点->主信息.对应需求.指针, nullptr);
        if (!场景) {
            return nullptr;
        }

        auto* 承载世界 = 私有_推导任务虚拟存在承载世界(任务头结点, 场景);
        if (!承载世界) {
            return nullptr;
        }

        auto* 任务虚拟存在 = 世界树.取或创建子存在_按名称类型(
            reinterpret_cast<基础信息节点类*>(承载世界),
            私有_任务虚拟存在名称词(任务头结点),
            私有_任务虚拟存在类型词());
        if (!任务虚拟存在) {
            return nullptr;
        }

        私有_写入引用(任务头结点->主信息.任务虚拟存在, 任务虚拟存在);
        (void)世界树.取存在主信息(任务虚拟存在);
        (void)世界树.取或创建存在内部世界(
            任务虚拟存在,
            私有_任务虚拟存在内部世界名称词(任务头结点));
        return 任务虚拟存在;
    }

    struct 结构_任务树SQL行 {
        int 行号 = 0;
        std::string 节点主键{};
        std::string 父节点主键{};
        int 深度 = 0;
        int 同层序号 = 0;
        int 直接子数量 = 0;
        std::string 路径{};
        int 节点种类值 = 0;
        std::string 节点种类文本{};
        int 任务状态值 = 0;
        std::string 任务状态文本{};
        std::string 名称文本{};
        std::string 类型文本{};
        std::string 对应需求主键{};
        std::string 任务虚拟存在主键{};
        std::string 场景主键{};
        std::string 目标状态主键{};
        std::string 结果状态主键{};
        std::int64_t 创建时间 = 0;
        std::int64_t 启动时间 = 0;
        std::int64_t 完成时间 = 0;
    };

    std::mutex& 私有_任务树SQL投影互斥() noexcept
    {
        static std::mutex 互斥{};
        return 互斥;
    }

    std::string 私有_任务SQL字符串(const std::string& 文本, const bool 空为NULL = true)
    {
        if (空为NULL && 文本.empty()) {
            return "NULL";
        }
        std::string 输出 = "N'";
        for (const char 字符 : 文本) {
            if (字符 == '\'') {
                输出 += "''";
            }
            else {
                输出.push_back(字符);
            }
        }
        输出.push_back('\'');
        return 输出;
    }

    std::string 私有_任务SQL入口文本(const 语素入口节点类* 入口)
    {
        if (!入口) {
            return {};
        }
        const auto 词面 = 语素集.安全获取词(入口);
        return 词面.empty() ? 入口->获取主键() : 词面;
    }

    template<class T节点>
    std::string 私有_任务SQL引用主键(const 可解析引用<T节点>& 引用)
    {
        if (!引用.主键.empty()) {
            return 引用.主键;
        }
        return 引用.指针 ? 引用.指针->获取主键() : std::string{};
    }

    const char* 私有_任务SQL节点种类文本(const 枚举_任务节点种类 种类) noexcept
    {
        switch (种类) {
        case 枚举_任务节点种类::头结点: return "头结点";
        case 枚举_任务节点种类::步骤节点: return "步骤节点";
        default: return "未定义";
        }
    }

    const char* 私有_任务SQL状态文本(const 枚举_任务状态 状态) noexcept
    {
        switch (状态) {
        case 枚举_任务状态::未启动: return "未启动";
        case 枚举_任务状态::运行中: return "运行中";
        case 枚举_任务状态::挂起: return "挂起";
        case 枚举_任务状态::完成: return "完成";
        case 枚举_任务状态::已结算: return "已结算";
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

    // 功能：把任务状态变化同步到 SQL 控制面板运行态显示镜像。
    void 私有_同步SQL控制面板任务显示项(
        const 任务节点* 任务头结点,
        const char* 阶段) noexcept
    {
        if (!任务头结点) {
            return;
        }

        结构_SQL控制面板显示项 显示项{};
        显示项.显示域 = 枚举_SQL控制面板显示域::任务;
        显示项.主键 = 任务头结点->获取主键();
        显示项.父主键 = 任务头结点->父 ? 任务头结点->父->获取主键() : std::string{};
        const auto 状态 = 任务类::读取任务状态(任务头结点, 枚举_任务状态::未定义);
        显示项.状态值 = static_cast<std::int64_t>(状态);
        显示项.状态文本 = 私有_任务SQL状态文本(状态);
        显示项.显示摘要 = 阶段 ? 阶段 : "任务状态同步";

        std::string 错误{};
        if (同步写入SQL控制面板显示项(显示项, 错误)) {
            return;
        }

        std::string 状态错误{};
        if (!写入SQL控制面板同步状态("任务", 阶段 ? 阶段 : "任务状态同步", false, 错误, 状态错误)) {
            项目运行警告日志(
                "任务SQL控制面板同步状态写入失败"
                " | 任务=" + 显示项.主键
                + " | 原因=" + 状态错误
                + " | 原始同步错误=" + 错误);
        }
        项目运行警告日志(
            "任务SQL控制面板同步失败"
            " | 任务=" + 显示项.主键
            + " | 阶段=" + (阶段 ? 阶段 : "任务状态同步")
            + " | 原因=" + 错误
            + " | 影响=仅控制面板SQL显示镜像未更新，任务状态写入不回滚");
    }

    void 私有_收集任务树SQL行(
        const 任务节点* 节点,
        const std::string& 父节点主键,
        const int 深度,
        const int 同层序号,
        const std::string& 父路径,
        std::unordered_set<const 任务节点*>& 已访问,
        std::vector<结构_任务树SQL行>& 行集)
    {
        if (!节点 || !已访问.insert(节点).second) {
            return;
        }

        const auto 节点主键 = 节点->获取主键();
        const auto 路径 = 父路径.empty() ? 节点主键 : 父路径 + "/" + 节点主键;
        const auto 节点种类 = 任务类::读取任务节点种类(节点, 枚举_任务节点种类::未定义);
        const auto 任务状态 = 任务类::读取任务状态(节点, 枚举_任务状态::未定义);

        结构_任务树SQL行 行{};
        行.行号 = static_cast<int>(行集.size() + 1);
        行.节点主键 = 节点主键;
        行.父节点主键 = 父节点主键;
        行.深度 = 深度;
        行.同层序号 = 同层序号;
        行.直接子数量 = static_cast<int>(节点->子节点数量);
        行.路径 = 路径;
        行.节点种类值 = static_cast<int>(节点种类);
        行.节点种类文本 = 私有_任务SQL节点种类文本(节点种类);
        行.任务状态值 = static_cast<int>(任务状态);
        行.任务状态文本 = 私有_任务SQL状态文本(任务状态);
        行.名称文本 = 私有_任务SQL入口文本(节点->主信息.名称);
        行.类型文本 = 私有_任务SQL入口文本(节点->主信息.类型);
        行.对应需求主键 = 私有_任务SQL引用主键(节点->主信息.对应需求);
        行.任务虚拟存在主键 = 私有_任务SQL引用主键(节点->主信息.任务虚拟存在);
        行.场景主键 = 私有_任务SQL引用主键(节点->主信息.场景);
        行.目标状态主键 = 私有_任务SQL引用主键(节点->主信息.目标状态);
        行.结果状态主键 = 私有_任务SQL引用主键(节点->主信息.实际结果状态);
        行.创建时间 = static_cast<std::int64_t>(节点->主信息.创建时间);
        行.启动时间 = static_cast<std::int64_t>(节点->主信息.启动时间);
        行.完成时间 = static_cast<std::int64_t>(节点->主信息.完成时间);
        行集.push_back(std::move(行));

        if (!节点->子) {
            return;
        }
        auto* 首子节点 = static_cast<const 任务节点*>(节点->子);
        auto* 当前子节点 = 首子节点;
        int 子序号 = 0;
        std::size_t 保护 = 0;
        do {
            私有_收集任务树SQL行(
                当前子节点,
                节点主键,
                深度 + 1,
                子序号,
                路径,
                已访问,
                行集);
            当前子节点 = static_cast<const 任务节点*>(当前子节点->下);
            ++子序号;
            ++保护;
        } while (当前子节点 && 当前子节点 != 首子节点 && 保护 < 100000);
    }

    std::string 私有_任务树SQL建库脚本()
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "IF DB_ID(N'鱼巢投影库') IS NULL CREATE DATABASE [鱼巢投影库];\n";
        return SQL.str();
    }

    std::string 私有_任务树SQL建表脚本()
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "IF SCHEMA_ID(N'鱼巢') IS NULL EXEC(N'CREATE SCHEMA [鱼巢]');\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前任务树节点]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前任务树节点];\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前任务主信息]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前任务主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[任务主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[任务主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[任务树节点]', N'U') IS NOT NULL DROP TABLE [鱼巢].[任务树节点];\n"
            << "IF OBJECT_ID(N'[鱼巢].[任务树快照]', N'U') IS NOT NULL DROP TABLE [鱼巢].[任务树快照];\n"
            << "IF OBJECT_ID(N'[鱼巢].[任务树快照]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[任务树快照] (\n"
            << "    [快照标识] uniqueidentifier NOT NULL PRIMARY KEY,\n"
            << "    [捕获时间] datetime2(3) NOT NULL,\n"
            << "    [来源类型] nvarchar(80) NOT NULL,\n"
            << "    [来源原因] nvarchar(300) NULL,\n"
            << "    [根主键] nvarchar(80) NULL,\n"
            << "    [节点数量] int NOT NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[任务树节点]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[任务树节点] (\n"
            << "    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,\n"
            << "    [快照标识] uniqueidentifier NOT NULL,\n"
            << "    [行号] int NOT NULL,\n"
            << "    [节点主键] nvarchar(80) NOT NULL,\n"
            << "    [父节点主键] nvarchar(80) NULL,\n"
            << "    [深度] int NOT NULL,\n"
            << "    [同层序号] int NOT NULL,\n"
            << "    [直接子数量] int NOT NULL,\n"
            << "    [路径文本] nvarchar(1000) NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[任务主信息]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[任务主信息] (\n"
            << "    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,\n"
            << "    [快照标识] uniqueidentifier NOT NULL,\n"
            << "    [节点主键] nvarchar(80) NOT NULL,\n"
            << "    [节点行号] int NOT NULL,\n"
            << "    [节点种类值] int NULL,\n"
            << "    [节点种类文本] nvarchar(80) NULL,\n"
            << "    [任务状态值] int NULL,\n"
            << "    [任务状态文本] nvarchar(80) NULL,\n"
            << "    [名称文本] nvarchar(300) NULL,\n"
            << "    [类型文本] nvarchar(300) NULL,\n"
            << "    [需求主键] nvarchar(80) NULL,\n"
            << "    [虚拟存在主键] nvarchar(80) NULL,\n"
            << "    [场景主键] nvarchar(80) NULL,\n"
            << "    [目标状态主键] nvarchar(80) NULL,\n"
            << "    [结果状态主键] nvarchar(80) NULL,\n"
            << "    [创建时间微秒] bigint NULL,\n"
            << "    [开始时间微秒] bigint NULL,\n"
            << "    [完成时间微秒] bigint NULL\n"
            << ");\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_task_tree_node_key' AND object_id = OBJECT_ID(N'[鱼巢].[任务树节点]'))\n"
            << "    CREATE INDEX IX_task_tree_node_key ON [鱼巢].[任务树节点]([快照标识], [节点主键], [父节点主键]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_task_tree_node_row' AND object_id = OBJECT_ID(N'[鱼巢].[任务树节点]'))\n"
            << "    CREATE INDEX IX_task_tree_node_row ON [鱼巢].[任务树节点]([快照标识], [行号]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_task_main_info_node' AND object_id = OBJECT_ID(N'[鱼巢].[任务主信息]'))\n"
            << "    CREATE INDEX IX_task_main_info_node ON [鱼巢].[任务主信息]([快照标识], [节点主键]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_task_main_info_node_row' AND object_id = OBJECT_ID(N'[鱼巢].[任务主信息]'))\n"
            << "    CREATE INDEX IX_task_main_info_node_row ON [鱼巢].[任务主信息]([快照标识], [节点行号]);\n";
        return SQL.str();
    }

    std::string 私有_任务树SQL视图脚本()
    {
        std::ostringstream SQL;
        SQL << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前任务主信息] AS\n"
            << "SELECT m.*\n"
            << "FROM [鱼巢].[任务主信息] m\n"
            << "WHERE m.[快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[任务树快照] ORDER BY [捕获时间] DESC);');\n"
            << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前任务树节点] AS\n"
            << "SELECT\n"
            << "    n.[记录标识],\n"
            << "    n.[快照标识],\n"
            << "    n.[行号],\n"
            << "    n.[节点主键],\n"
            << "    n.[父节点主键],\n"
            << "    n.[深度],\n"
            << "    n.[同层序号],\n"
            << "    n.[直接子数量],\n"
            << "    n.[路径文本],\n"
            << "    m.[记录标识] AS [主信息记录标识],\n"
            << "    m.[节点行号],\n"
            << "    m.[节点种类值],\n"
            << "    m.[节点种类文本],\n"
            << "    m.[任务状态值],\n"
            << "    m.[任务状态文本],\n"
            << "    m.[名称文本],\n"
            << "    m.[类型文本],\n"
            << "    m.[需求主键],\n"
            << "    m.[虚拟存在主键],\n"
            << "    m.[场景主键],\n"
            << "    m.[目标状态主键],\n"
            << "    m.[结果状态主键],\n"
            << "    m.[创建时间微秒],\n"
            << "    m.[开始时间微秒],\n"
            << "    m.[完成时间微秒]\n"
            << "FROM [鱼巢].[任务树节点] n\n"
            << "LEFT JOIN [鱼巢].[任务主信息] m\n"
            << "    ON m.[快照标识] = n.[快照标识]\n"
            << "    AND m.[节点行号] = n.[行号]\n"
            << "WHERE n.[快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[任务树快照] ORDER BY [捕获时间] DESC);');\n";
        return SQL.str();
    }

    std::string 私有_构造任务树SQL重写脚本(
        const std::vector<结构_任务树SQL行>& 行集,
        const std::string& 来源原因,
        const std::string& 根主键)
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "SET XACT_ABORT ON;\n"
            << "BEGIN TRANSACTION;\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'节点种类值') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [节点种类值];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'节点种类文本') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [节点种类文本];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'任务状态值') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [任务状态值];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'任务状态文本') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [任务状态文本];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'名称文本') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [名称文本];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'类型文本') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [类型文本];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'需求主键') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [需求主键];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'虚拟存在主键') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [虚拟存在主键];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'场景主键') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [场景主键];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'目标状态主键') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [目标状态主键];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'结果状态主键') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [结果状态主键];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'创建时间微秒') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [创建时间微秒];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'开始时间微秒') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [开始时间微秒];\n"
            << "IF COL_LENGTH(N'[鱼巢].[任务树节点]', N'完成时间微秒') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[任务树节点] DROP COLUMN [完成时间微秒];\n"
            << "DELETE FROM [鱼巢].[任务主信息];\n"
            << "DELETE FROM [鱼巢].[任务树节点];\n"
            << "DELETE FROM [鱼巢].[任务树快照];\n"
            << "DECLARE @快照标识 uniqueidentifier = NEWID();\n"
            << "INSERT INTO [鱼巢].[任务树快照] ([快照标识], [捕获时间], [来源类型], [来源原因], [根主键], [节点数量])\n"
            << "VALUES (@快照标识, SYSUTCDATETIME(), N'task_tree_projection', "
            << 私有_任务SQL字符串(来源原因)
            << ", " << 私有_任务SQL字符串(根主键)
            << ", " << 行集.size() << ");\n";
        for (const auto& 行 : 行集) {
            SQL << "INSERT INTO [鱼巢].[任务树节点] ([快照标识], [行号], [节点主键], [父节点主键], [深度], [同层序号], [直接子数量], [路径文本]) VALUES (@快照标识, "
                << 行.行号 << ", "
                << 私有_任务SQL字符串(行.节点主键, false) << ", "
                << 私有_任务SQL字符串(行.父节点主键) << ", "
                << 行.深度 << ", "
                << 行.同层序号 << ", "
                << 行.直接子数量 << ", "
                << 私有_任务SQL字符串(行.路径) << ");\n";
            SQL << "INSERT INTO [鱼巢].[任务主信息] ([快照标识], [节点主键], [节点行号], [节点种类值], [节点种类文本], [任务状态值], [任务状态文本], [名称文本], [类型文本], [需求主键], [虚拟存在主键], [场景主键], [目标状态主键], [结果状态主键], [创建时间微秒], [开始时间微秒], [完成时间微秒]) VALUES (@快照标识, "
                << 私有_任务SQL字符串(行.节点主键, false) << ", "
                << 行.行号 << ", "
                << 行.节点种类值 << ", "
                << 私有_任务SQL字符串(行.节点种类文本) << ", "
                << 行.任务状态值 << ", "
                << 私有_任务SQL字符串(行.任务状态文本) << ", "
                << 私有_任务SQL字符串(行.名称文本) << ", "
                << 私有_任务SQL字符串(行.类型文本) << ", "
                << 私有_任务SQL字符串(行.对应需求主键) << ", "
                << 私有_任务SQL字符串(行.任务虚拟存在主键) << ", "
                << 私有_任务SQL字符串(行.场景主键) << ", "
                << 私有_任务SQL字符串(行.目标状态主键) << ", "
                << 私有_任务SQL字符串(行.结果状态主键) << ", "
                << 行.创建时间 << ", "
                << 行.启动时间 << ", "
                << 行.完成时间 << ");\n";
        }
        SQL << "IF EXISTS (\n"
            << "    SELECT 1\n"
            << "    FROM [鱼巢].[任务主信息] info\n"
            << "    LEFT JOIN [鱼巢].[任务树节点] node\n"
            << "        ON node.[快照标识] = info.[快照标识] AND node.[行号] = info.[节点行号]\n"
            << "    WHERE info.[快照标识] = @快照标识 AND node.[行号] IS NULL\n"
            << ")\n"
            << "    THROW 51011, N'task_main_info [节点行号] invalid', 1;\n";
        SQL << "COMMIT TRANSACTION;\n";
        return SQL.str();
    }

    bool 私有_执行任务树ADO命令(
        const std::string& 连接串,
        const std::string& 阶段,
        const std::string& SQL,
        std::string& 错误)
    {
        std::string ADO错误{};
        if (!执行ADO命令(连接串, SQL, ADO错误)) {
            错误 = 阶段 + "失败 | " + ADO错误;
            return false;
        }
        return true;
    }

    std::string 私有_任务SQL字段整数(const int 值)
    {
        return std::to_string(值);
    }

    std::string 私有_任务SQL字段整数(const std::int64_t 值)
    {
        return std::to_string(值);
    }

    std::vector<std::vector<std::string>> 私有_任务SQL快照预期字段(
        const std::string& 来源原因,
        const std::string& 根主键,
        const std::size_t 节点数)
    {
        return {
            {
                "task_tree_projection",
                来源原因,
                根主键,
                std::to_string(节点数),
            },
        };
    }

    std::vector<std::vector<std::string>> 私有_任务SQL节点预期字段(
        const std::vector<结构_任务树SQL行>& 行集)
    {
        std::vector<std::vector<std::string>> 输出;
        输出.reserve(行集.size());
        for (const auto& 行 : 行集) {
            输出.push_back({
                私有_任务SQL字段整数(行.行号),
                行.节点主键,
                行.父节点主键,
                私有_任务SQL字段整数(行.深度),
                私有_任务SQL字段整数(行.同层序号),
                私有_任务SQL字段整数(行.直接子数量),
                行.路径,
            });
        }
        return 输出;
    }

    std::vector<std::vector<std::string>> 私有_任务SQL主信息预期字段(
        const std::vector<结构_任务树SQL行>& 行集)
    {
        std::vector<std::vector<std::string>> 输出;
        输出.reserve(行集.size());
        for (const auto& 行 : 行集) {
            输出.push_back({
                行.节点主键,
                私有_任务SQL字段整数(行.行号),
                私有_任务SQL字段整数(行.节点种类值),
                行.节点种类文本,
                私有_任务SQL字段整数(行.任务状态值),
                行.任务状态文本,
                行.名称文本,
                行.类型文本,
                行.对应需求主键,
                行.任务虚拟存在主键,
                行.场景主键,
                行.目标状态主键,
                行.结果状态主键,
                私有_任务SQL字段整数(行.创建时间),
                私有_任务SQL字段整数(行.启动时间),
                私有_任务SQL字段整数(行.完成时间),
            });
        }
        return 输出;
    }

    bool 私有_执行任务SQL字段恢复比对(
        const std::string& 连接串,
        const std::string& 阶段,
        const std::string& SQL,
        const std::vector<std::vector<std::string>>& 预期行集,
        std::string& 错误)
    {
        结构_ADO字段恢复比对结果 比对{};
        std::string ADO错误{};
        if (!执行ADO字段恢复比对(连接串, SQL, 预期行集, 比对, ADO错误)) {
            错误 = 阶段 + "查询失败 | " + ADO错误;
            return false;
        }
        if (!比对.匹配) {
            错误 = 阶段 + "字段比对失败 | " + 比对.首个差异;
            return false;
        }
        return true;
    }

    bool 私有_验证任务SQL存储字段(
        const std::string& 连接串,
        const std::vector<结构_任务树SQL行>& 行集,
        const std::string& 来源原因,
        const std::string& 根主键,
        std::string& 错误)
    {
        constexpr const char* 快照SQL = R"SQL(
SELECT
    COALESCE([来源类型], N''),
    COALESCE([来源原因], N''),
    COALESCE([根主键], N''),
    COALESCE(CONVERT(nvarchar(30), [节点数量]), N'')
FROM [鱼巢].[任务树快照]
ORDER BY [捕获时间] DESC;
)SQL";
        constexpr const char* 节点SQL = R"SQL(
SELECT
    COALESCE(CONVERT(nvarchar(30), [行号]), N''),
    COALESCE([节点主键], N''),
    COALESCE([父节点主键], N''),
    COALESCE(CONVERT(nvarchar(30), [深度]), N''),
    COALESCE(CONVERT(nvarchar(30), [同层序号]), N''),
    COALESCE(CONVERT(nvarchar(30), [直接子数量]), N''),
    COALESCE([路径文本], N'')
FROM [鱼巢].[任务树节点]
WHERE [快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[任务树快照] ORDER BY [捕获时间] DESC)
ORDER BY [行号];
)SQL";
        constexpr const char* 主信息SQL = R"SQL(
SELECT
    COALESCE([节点主键], N''),
    COALESCE(CONVERT(nvarchar(30), [节点行号]), N''),
    COALESCE(CONVERT(nvarchar(30), [节点种类值]), N''),
    COALESCE([节点种类文本], N''),
    COALESCE(CONVERT(nvarchar(30), [任务状态值]), N''),
    COALESCE([任务状态文本], N''),
    COALESCE([名称文本], N''),
    COALESCE([类型文本], N''),
    COALESCE([需求主键], N''),
    COALESCE([虚拟存在主键], N''),
    COALESCE([场景主键], N''),
    COALESCE([目标状态主键], N''),
    COALESCE([结果状态主键], N''),
    COALESCE(CONVERT(nvarchar(30), [创建时间微秒]), N''),
    COALESCE(CONVERT(nvarchar(30), [开始时间微秒]), N''),
    COALESCE(CONVERT(nvarchar(30), [完成时间微秒]), N'')
FROM [鱼巢].[任务主信息]
WHERE [快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[任务树快照] ORDER BY [捕获时间] DESC)
ORDER BY [节点行号];
)SQL";

        return 私有_执行任务SQL字段恢复比对(
            连接串,
            "任务SQL字段恢复比对/快照",
            快照SQL,
            私有_任务SQL快照预期字段(来源原因, 根主键, 行集.size()),
            错误)
            && 私有_执行任务SQL字段恢复比对(
                连接串,
                "任务SQL字段恢复比对/节点",
                节点SQL,
                私有_任务SQL节点预期字段(行集),
                错误)
            && 私有_执行任务SQL字段恢复比对(
                连接串,
                "任务SQL字段恢复比对/主信息",
                主信息SQL,
                私有_任务SQL主信息预期字段(行集),
                错误);
    }
}

// 功能：把当前任务树本体重写到 SQL Server 查询投影。
bool 任务类::重写任务树SQL投影(
    const 节点类* 任务根节点,
    const char* 来源原因,
    const bool 执行字段恢复比对) noexcept
{
    if (!任务根节点) {
        项目运行警告日志("任务树SQL投影跳过 | 原因=任务根为空");
        return false;
    }

    std::lock_guard<std::mutex> SQL锁{ 私有_任务树SQL投影互斥() };
    try {
        std::vector<结构_任务树SQL行> 行集{};
        std::unordered_set<const 任务节点*> 已访问{};
        私有_收集任务树SQL行(
            reinterpret_cast<const 任务节点*>(任务根节点),
            {},
            0,
            0,
            {},
            已访问,
            行集);

        const auto 根主键 = 任务根节点->获取主键();
        const auto 原因文本 = 来源原因 ? std::string(来源原因) : std::string{};
        const auto 主库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "master");
        const auto 投影库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "鱼巢投影库");
        std::string 错误{};
        if (!私有_执行任务树ADO命令(主库连接串, "任务树SQL投影建库", 私有_任务树SQL建库脚本(), 错误)
            || !私有_执行任务树ADO命令(投影库连接串, "任务树SQL投影建表", 私有_任务树SQL建表脚本(), 错误)
            || !私有_执行任务树ADO命令(投影库连接串, "任务树SQL投影视图", 私有_任务树SQL视图脚本(), 错误)
            || !私有_执行任务树ADO命令(投影库连接串, "任务树SQL投影重写", 私有_构造任务树SQL重写脚本(行集, 原因文本, 根主键), 错误)
            || (执行字段恢复比对 && !私有_验证任务SQL存储字段(投影库连接串, 行集, 原因文本, 根主键, 错误))) {
            项目运行错误日志(
                "任务树SQL投影失败"
                " | 原因=" + 错误
                + " | 节点数=" + std::to_string(行集.size()));
            return false;
        }

        项目运行日志(
            "任务树SQL投影完成"
            " | 来源=" + 原因文本
            + " | 根=" + 根主键
            + " | 节点数=" + std::to_string(行集.size())
            + " | 字段恢复比对=" + (执行字段恢复比对 ? "通过" : "跳过"));
        return true;
    }
    catch (const std::exception& 异常) {
        项目运行错误日志(std::string("任务树SQL投影异常 | 原因=") + 异常.what());
    }
    catch (...) {
        项目运行错误日志("任务树SQL投影异常 | 原因=未知异常");
    }
    return false;
}

// 功能：创建并返回或登记对应对象。
存在节点类* 任务类::取或创建_任务虚拟存在(
    节点类* 任务头结点,
    场景节点类* 承载场景)
{
    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) {
        return nullptr;
    }
    const bool 已有任务虚拟存在 = 头结点->主信息.任务虚拟存在.指针 != nullptr;
    auto* 任务虚拟存在 = 私有_创建并初始化任务虚拟存在(头结点, 承载场景);
    if (!已有任务虚拟存在) {
        (void)私有_补齐任务虚拟存在概念集(头结点, 任务虚拟存在);
    }
    return 任务虚拟存在;
}

// 功能：记录日志、动态、证据或运行痕迹。
bool 任务类::记录任务虚拟存在特征(
    节点类* 任务头结点,
    const 语素入口节点类* 特征类型,
    const 特征值& 值,
    场景节点类* 承载场景,
    时间戳 now)
{
    if (!特征类型) return false;
    if (now == 0) now = 结构体_时间戳::当前_微秒();

    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) return false;

    auto* 任务虚拟存在 = 取或创建_任务虚拟存在(头结点, 承载场景);
    if (!任务虚拟存在) return false;

    if (const auto* 标量值 = std::get_if<I64>(&值)) {
        return 世界树.写入特征_I64(任务虚拟存在, 特征类型, *标量值, now);
    }
    if (const auto* 指针值 = std::get_if<指针句柄>(&值)) {
        return 世界树.写入特征_指针(
            任务虚拟存在,
            特征类型,
            reinterpret_cast<const void*>(指针值->指针),
            now);
    }
    if (const auto* Vec句柄 = std::get_if<VecU句柄>(&值)) {
        const auto* Vec值 = 世界树.取VecU只读指针(*Vec句柄);
        return Vec值
            ? 世界树.写入特征_VecU(任务虚拟存在, 特征类型, *Vec值, 枚举_轮廓维度::未定义, now)
            : false;
    }
    return false;
}

// 功能：初始化相关对象、状态或运行上下文。
bool 任务类::初始化任务虚拟存在信息(
    节点类* 任务头结点,
    场景节点类* 承载场景,
    时间戳 now)
{
    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) return false;
    if (now == 0) now = 结构体_时间戳::当前_微秒();

    auto* 任务虚拟存在 = 取或创建_任务虚拟存在(头结点, 承载场景);
    if (!任务虚拟存在) return false;
    (void)私有_补齐任务虚拟存在概念集(头结点, 任务虚拟存在);

    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征账本生命周期(), static_cast<I64>(枚举_特征生命周期::候选), now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("暴露次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("读取次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("有效使用次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("历史使用次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("近期使用次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("作为决策输入次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("作为派生父特征次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("作为需求触发次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("作为风险闸门次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("作为验证信号次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("作为解释依据次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("结果改变次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("坏结果次数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("下游依赖特征数"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("最近使用时间"), 0, now);
    (void)私有_确保I64特征已初始化(任务虚拟存在, 私有_特征_任务特征使用统计("最近有效使用时间"), 0, now);
    (void)世界树.写入特征_I64(
        reinterpret_cast<基础信息节点类*>(任务虚拟存在),
        私有_特征_当前阶段(),
        static_cast<I64>(私有_推导任务阶段(任务类::读取任务状态(头结点, 枚举_任务状态::未启动))),
        now);
    (void)世界树.写入特征_I64(
        reinterpret_cast<基础信息节点类*>(任务虚拟存在),
        私有_特征_任务状态(),
        static_cast<I64>(任务类::读取任务状态(头结点, 枚举_任务状态::未启动)),
        now);
    (void)世界树.写入特征_I64(
        reinterpret_cast<基础信息节点类*>(任务虚拟存在),
        私有_特征_任务节点种类(),
        static_cast<I64>(任务类::读取任务节点种类(头结点, 枚举_任务节点种类::头结点)),
        now);
    (void)私有_写入任务运行镜像特征(
        任务虚拟存在,
        头结点->主信息,
        任务类::读取任务状态(头结点, 枚举_任务状态::未启动),
        任务类::读取任务节点种类(头结点, 枚举_任务节点种类::头结点),
        now);
    return true;
}

// 功能：补齐条件、参数、证据或方法能力缺口。
bool 任务类::补齐任务虚拟存在概念集(
    节点类* 任务头结点,
    场景节点类* 承载场景,
    时间戳 now)
{
    (void)now;
    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) return false;

    auto* 任务虚拟存在 = 私有_创建并初始化任务虚拟存在(头结点, 承载场景);
    return 私有_补齐任务虚拟存在概念集(头结点, 任务虚拟存在);
}

// 功能：同步线程、任务、场景或对象之间的状态。
bool 任务类::同步任务节点到虚拟存在(
    节点类* 任务头结点,
    场景节点类* 承载场景,
    时间戳 now)
{
    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) return false;
    if (now == 0) now = 结构体_时间戳::当前_微秒();

    // 任务虚拟存在的通用概念、账本和运行镜像统一在任务类内维护；界面线程只负责承接。
    return 补齐任务虚拟存在概念集(头结点, 承载场景, now)
        && 初始化任务虚拟存在信息(头结点, 承载场景, now)
        && 同步任务运行镜像特征(头结点, 承载场景, now);
}

// 功能：同步线程、任务、场景或对象之间的状态。
bool 任务类::同步任务运行镜像特征(
    节点类* 任务头结点,
    场景节点类* 承载场景,
    时间戳 now)
{
    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) return false;
    if (now == 0) now = 结构体_时间戳::当前_微秒();

    auto* 任务虚拟存在 = 取或创建_任务虚拟存在(头结点, 承载场景);
    if (!任务虚拟存在) return false;

    return 私有_写入任务运行镜像特征(
        任务虚拟存在,
        头结点->主信息,
        读取任务状态(头结点, 枚举_任务状态::未启动),
        读取任务节点种类(头结点, 枚举_任务节点种类::头结点),
        now);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务根实体特征(
    节点类* 任务头结点,
    std::uint64_t 任务根ID,
    std::uintptr_t 来源需求指针,
    std::uintptr_t 世界任务根指针,
    std::uintptr_t 父任务指针,
    std::uintptr_t 管理对象任务指针,
    I64 初始授权等级,
    bool 只读,
    场景节点类* 承载场景,
    时间戳 now)
{
    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) return false;
    if (now == 0) now = 结构体_时间戳::当前_微秒();

    bool 已写入 = false;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_根ID(), static_cast<I64>(任务根ID), 承载场景, now) || 已写入;
    已写入 = 私有_写入任务指针特征(头结点, 自我特征定义类::类型_任务_来源需求(), 来源需求指针, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务指针特征(头结点, 自我特征定义类::类型_任务_世界任务根(), 世界任务根指针, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务指针特征(头结点, 自我特征定义类::类型_任务_父任务(), 父任务指针, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务指针特征(头结点, 自我特征定义类::类型_任务_管理对象任务(), 管理对象任务指针, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_初始授权等级(), 初始授权等级, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_只读状态(), 只读 ? 1 : 0, 承载场景, now) || 已写入;
    return 已写入;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务控制实体特征(
    节点类* 任务头结点,
    std::uint64_t 任务根ID,
    I64 队列状态值,
    I64 全局优先级,
    I64 解阻加权,
    I64 时间预算_ms,
    I64 资源预算,
    std::uint64_t 绑定工作线程ID,
    bool 已派发,
    bool 等待子结果,
    bool 请求收束,
    bool 请求停止,
    bool 是否已响应控制意图,
    bool 控制请求待确认,
    场景节点类* 承载场景,
    时间戳 now)
{
    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) return false;
    if (now == 0) now = 结构体_时间戳::当前_微秒();

    bool 已写入 = false;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_根ID(), static_cast<I64>(任务根ID), 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_队列状态(), 队列状态值, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_全局优先级(), 全局优先级, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_解阻加权(), 解阻加权, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_时间预算(), 时间预算_ms, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_资源预算(), 资源预算, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_绑定工作线程(), static_cast<I64>(绑定工作线程ID), 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_已派发(), 已派发 ? 1 : 0, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_等待子结果(), 等待子结果 ? 1 : 0, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_请求收束(), 请求收束 ? 1 : 0, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_请求停止(), 请求停止 ? 1 : 0, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_已响应控制意图(), 是否已响应控制意图 ? 1 : 0, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_控制请求待确认(), 控制请求待确认 ? 1 : 0, 承载场景, now) || 已写入;
    return 已写入;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务局部运行实体特征(
    节点类* 任务头结点,
    std::uint64_t 任务根ID,
    I64 当前状态值,
    std::uintptr_t 当前步骤前沿,
    std::uintptr_t 最近局部结果,
    I64 局部完成度,
    场景节点类* 承载场景,
    时间戳 now)
{
    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) return false;
    if (now == 0) now = 结构体_时间戳::当前_微秒();

    bool 已写入 = false;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_根ID(), static_cast<I64>(任务根ID), 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_局部运行状态(), 当前状态值, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务指针特征(头结点, 自我特征定义类::类型_任务_当前步骤前沿(), 当前步骤前沿, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务指针特征(头结点, 自我特征定义类::类型_任务_最近局部结果(), 最近局部结果, 承载场景, now) || 已写入;
    已写入 = 私有_写入任务I64特征(头结点, 自我特征定义类::类型_任务_局部完成度(), 局部完成度, 承载场景, now) || 已写入;
    return 已写入;
}

// 功能：按函数名执行对应处理。
bool 任务类::任务状态已终结(枚举_任务状态 状态) noexcept
{
    return 状态 == 枚举_任务状态::完成
        || 状态 == 枚举_任务状态::已结算
        || 状态 == 枚举_任务状态::失败
        || 状态 == 枚举_任务状态::取消
        || 状态 == 枚举_任务状态::超时;
}

// 功能：从指定来源读取数据或状态。
const 可解析引用<需求节点类>& 任务类::读取任务对应需求引用(
    const 节点类* 任务节点) noexcept
{
    static const 可解析引用<需求节点类> 空引用{};
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    return 头结点 ? 头结点->主信息.对应需求 : 空引用;
}

// 功能：解析输入文本、消息、场景或运行包。
需求节点类* 任务类::解析任务对应需求(
    const 节点类* 任务节点) noexcept
{
    return 私有_解析需求引用(读取任务对应需求引用(任务节点));
}

// 功能：从指定来源读取数据或状态。
const 可解析引用<存在节点类>& 任务类::读取任务虚拟存在引用(
    const 节点类* 任务节点) noexcept
{
    static const 可解析引用<存在节点类> 空引用{};
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    return 头结点 ? 头结点->主信息.任务虚拟存在 : 空引用;
}

// 功能：解析输入文本、消息、场景或运行包。
存在节点类* 任务类::解析任务虚拟存在(
    const 节点类* 任务节点) noexcept
{
    return 私有_解析存在引用(读取任务虚拟存在引用(任务节点));
}

// 功能：从指定来源读取数据或状态。
const 可解析引用<场景节点类>& 任务类::读取任务场景引用(
    const 节点类* 任务节点) noexcept
{
    static const 可解析引用<场景节点类> 空引用{};
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    return 头结点 ? 头结点->主信息.场景 : 空引用;
}

// 功能：解析输入文本、消息、场景或运行包。
场景节点类* 任务类::解析任务场景(
    const 节点类* 任务节点) noexcept
{
    return 私有_解析场景引用(读取任务场景引用(任务节点));
}

// 功能：从指定来源读取数据或状态。
const 可解析引用<状态节点类>& 任务类::读取任务目标状态引用(
    const 节点类* 任务节点) noexcept
{
    static const 可解析引用<状态节点类> 空引用{};
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    return 头结点 ? 头结点->主信息.目标状态 : 空引用;
}

// 功能：解析输入文本、消息、场景或运行包。
状态节点类* 任务类::解析任务目标状态(
    const 节点类* 任务节点) noexcept
{
    return 私有_解析状态引用(读取任务目标状态引用(任务节点));
}

// 功能：写入指定任务的目标状态引用。
bool 任务类::写入任务目标状态(
    节点类* 任务节点,
    const 可解析引用<状态节点类>& 目标状态引用) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(任务节点);
    if (!头结点) {
        return false;
    }
    头结点->主信息.目标状态 = 目标状态引用;
    return true;
}

// 功能：从指定来源读取数据或状态。
const 可解析引用<状态节点类>& 任务类::读取任务实际结果状态(
    const 节点类* 任务节点) noexcept
{
    static const 可解析引用<状态节点类> 空引用{};
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    return 头结点 ? 头结点->主信息.实际结果状态 : 空引用;
}

// 功能：解析输入文本、消息、场景或运行包。
状态节点类* 任务类::解析任务实际结果状态(
    const 节点类* 任务节点) noexcept
{
    return 私有_解析状态引用(读取任务实际结果状态(任务节点));
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务实际结果状态(
    节点类* 任务节点,
    const 可解析引用<状态节点类>& 状态引用) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(任务节点);
    if (!头结点) {
        return false;
    }
    头结点->主信息.实际结果状态 = 状态引用;
    return 状态引用.指针 || !状态引用.主键.empty();
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务实际结果状态(
    节点类* 任务节点,
    状态节点类* 状态节点) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(任务节点);
    if (!头结点) {
        return false;
    }
    私有_写入引用(头结点->主信息.实际结果状态, 状态节点);
    return 状态节点 != nullptr;
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
bool 任务类::绑定对应需求(
    节点类* 任务节点,
    需求节点类* 来源需求) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(任务节点);
    if (!头结点 || !来源需求) {
        return false;
    }
    私有_写入引用(头结点->主信息.对应需求, 来源需求);
    return true;
}

// 功能：从指定来源读取数据或状态。
时间戳 任务类::读取任务创建时间(
    const 节点类* 任务节点,
    时间戳 默认值) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    return 头结点 ? 头结点->主信息.创建时间 : 默认值;
}

// 功能：从指定来源读取数据或状态。
时间戳 任务类::读取任务启动时间(
    const 节点类* 任务节点,
    时间戳 默认值) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    return 头结点 ? 头结点->主信息.启动时间 : 默认值;
}

// 功能：从指定来源读取数据或状态。
时间戳 任务类::读取任务完成时间(
    const 节点类* 任务节点,
    时间戳 默认值) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    return 头结点 ? 头结点->主信息.完成时间 : 默认值;
}

// 功能：从指定来源读取数据或状态。
枚举_任务状态 任务类::读取任务状态(
    const 节点类* 任务节点,
    枚举_任务状态 默认值,
    场景节点类* 承载场景) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    if (!头结点) {
        return 默认值;
    }
    return static_cast<枚举_任务状态>(
        私有_读取任务I64特征(
            头结点,
            自我特征定义类::类型_任务_状态(),
            static_cast<I64>(默认值),
            承载场景));
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务状态(
    节点类* 任务节点,
    枚举_任务状态 值,
    场景节点类* 承载场景,
    时间戳 now) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(任务节点);
    if (!头结点 || 值 == 枚举_任务状态::未定义) {
        return false;
    }
    if (now == 0) {
        now = 结构体_时间戳::当前_微秒();
    }
    if ((值 == 枚举_任务状态::完成 || 值 == 枚举_任务状态::已结算)
        && 头结点->主信息.完成时间 == 0) {
        头结点->主信息.完成时间 = now;
    }
    const bool 已写入 = 私有_写入任务I64特征(
        头结点,
        自我特征定义类::类型_任务_状态(),
        static_cast<I64>(值),
        承载场景,
        now);
    if (已写入) {
        私有_同步SQL控制面板任务显示项(头结点, "写入任务状态");
    }
    return 已写入;
}

// 功能：从指定来源读取数据或状态。
枚举_任务节点种类 任务类::读取任务节点种类(
    const 节点类* 任务节点,
    枚举_任务节点种类 默认值,
    场景节点类* 承载场景) noexcept
{
    if (!任务节点) {
        return 默认值;
    }
    (void)承载场景;
    auto* 任务虚拟存在 = reinterpret_cast<存在节点类*>(任务节点->主信息.任务虚拟存在.指针);
    I64 当前值 = static_cast<I64>(默认值);
    return 任务虚拟存在
        && 世界树.读取特征_I64(任务虚拟存在, 自我特征定义类::类型_任务_节点种类(), 当前值)
        ? static_cast<枚举_任务节点种类>(当前值)
        : static_cast<枚举_任务节点种类>(当前值);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务节点种类(
    节点类* 任务节点,
    枚举_任务节点种类 值,
    场景节点类* 承载场景,
    时间戳 now) noexcept
{
    if (!任务节点 || 值 == 枚举_任务节点种类::未定义) {
        return false;
    }
    if (now == 0) {
        now = 结构体_时间戳::当前_微秒();
    }
    auto* 任务虚拟存在 = 私有_创建并初始化任务虚拟存在(任务节点, 承载场景);
    return 任务虚拟存在
        && 世界树.写入特征_I64(
            reinterpret_cast<基础信息节点类*>(任务虚拟存在),
            自我特征定义类::类型_任务_节点种类(),
            static_cast<I64>(值),
            now);
}

// 功能：派发任务、消息或运行请求。
bool 任务类::任务允许派发(const 节点类* 任务节点) noexcept
{
    const auto 状态 = 读取任务状态(任务节点);
    if (!任务节点 || 任务状态已终结(状态)) {
        return false;
    }
    switch (状态) {
    case 枚举_任务状态::等待中:
    case 枚举_任务状态::挂起:
    case 枚举_任务状态::无法执行:
        return false;
    default:
        return true;
    }
}

// 功能：确保目标结构、状态或前置条件存在并可用。
void 任务类::确保任务初始状态(节点类* 任务节点) noexcept
{
    if (任务节点 && 读取任务状态(任务节点) == 枚举_任务状态::未定义) {
        (void)写入任务状态(任务节点, 枚举_任务状态::未启动);
    }
}

// 功能：从指定来源读取数据或状态。
任务类::节点类* 任务类::读取任务管理对象任务(节点类* 任务节点) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    if (!头结点) {
        return nullptr;
    }
    auto* 任务虚拟存在 = reinterpret_cast<存在节点类*>(头结点->主信息.任务虚拟存在.指针);
    void* 指针 = nullptr;
    return 任务虚拟存在
        && 世界树.读取特征_指针(
            任务虚拟存在,
            自我特征定义类::类型_任务_管理对象任务(),
            指针)
        && 指针
        ? reinterpret_cast<节点类*>(指针)
        : nullptr;
}

// 功能：从指定来源读取数据或状态。
const 任务类::节点类* 任务类::读取任务管理对象任务(const 节点类* 任务节点) noexcept
{
    return 读取任务管理对象任务(const_cast<节点类*>(任务节点));
}

// 功能：从指定来源读取数据或状态。
任务类::节点类* 任务类::读取任务管理对象或自身(节点类* 任务节点) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    if (!头结点) {
        return nullptr;
    }
    if (auto* 管理对象 = 读取任务管理对象任务(头结点)) {
        return 管理对象;
    }
    return 头结点;
}

// 功能：按函数名执行对应处理。
bool 任务类::任务管理对象指向(const 节点类* 任务节点, const 节点类* 目标任务) noexcept
{
    if (!任务节点 || !目标任务) {
        return false;
    }
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    if (!头结点) {
        return false;
    }
    auto* 当前对象 = 读取任务管理对象任务(const_cast<节点类*>(头结点));
    return 当前对象 == 目标任务
        || (当前对象
            && !当前对象->获取主键().empty()
            && 当前对象->获取主键() == 目标任务->获取主键());
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务管理对象任务(
    节点类* 任务节点,
    节点类* 管理对象任务,
    场景节点类* 承载场景,
    时间戳 now) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(任务节点);
    if (!头结点) {
        return false;
    }
    if (!管理对象任务) {
        return true;
    }
    return 私有_写入任务指针特征(
        头结点,
        自我特征定义类::类型_任务_管理对象任务(),
        reinterpret_cast<std::uintptr_t>(管理对象任务),
        承载场景,
        now);
}

// 功能：从指定来源读取数据或状态。
方法节点类* 任务类::读取任务执行方法(节点类* 任务节点) noexcept
{
    if (!任务节点) {
        return nullptr;
    }
    if (auto* 指针 = 私有_读取任务指针特征(
            任务节点,
            自我特征定义类::类型_任务_执行方法())) {
        return reinterpret_cast<方法节点类*>(指针);
    }
    return nullptr;
}

// 功能：从指定来源读取数据或状态。
const 方法节点类* 任务类::读取任务执行方法(const 节点类* 任务节点) noexcept
{
    return 读取任务执行方法(const_cast<节点类*>(任务节点));
}

// 功能：执行任务、方法或动作的主体逻辑。
bool 任务类::任务执行方法指向(const 节点类* 任务节点, const 方法节点类* 目标方法) noexcept
{
    if (!任务节点 || !目标方法) {
        return false;
    }
    auto* 当前方法 = reinterpret_cast<方法节点*>(
        读取任务执行方法(const_cast<节点类*>(任务节点)));
    const auto* 目标方法节点 = reinterpret_cast<const 方法节点*>(目标方法);
    return 当前方法 == 目标方法节点
        || (当前方法
            && 目标方法节点
            && !当前方法->获取主键().empty()
            && 当前方法->获取主键() == 目标方法节点->获取主键());
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务执行方法(
    节点类* 任务节点,
    方法节点类* 执行方法,
    场景节点类* 承载场景,
    时间戳 now) noexcept
{
    if (!任务节点 || !执行方法) {
        return false;
    }
    return 私有_写入任务指针特征(
        任务节点,
        自我特征定义类::类型_任务_执行方法(),
        reinterpret_cast<std::uintptr_t>(执行方法),
        承载场景,
        now);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务当前方法虚拟存在(
    节点类* 任务节点,
    存在节点类* 方法虚拟存在,
    场景节点类* 承载场景,
    时间戳 now) noexcept
{
    if (!任务节点 || !方法虚拟存在) {
        return false;
    }
    return 私有_写入任务指针特征(
        任务节点,
        自我特征定义类::类型_任务_当前方法(),
        reinterpret_cast<std::uintptr_t>(方法虚拟存在),
        承载场景,
        now);
}

// 功能：按函数名执行对应处理。
bool 任务类::追加任务方法候选虚拟存在(
    节点类* 任务节点,
    存在节点类* 方法虚拟存在,
    场景节点类* 承载场景,
    时间戳 now) noexcept
{
    auto* 头结点 = 私有_归一到任务头结点(任务节点);
    if (!头结点 || !方法虚拟存在) {
        return false;
    }
    if (now == 0) {
        now = 结构体_时间戳::当前_微秒();
    }

    auto* 任务虚拟存在 = 取或创建_任务虚拟存在(头结点, 承载场景);
    if (!任务虚拟存在) {
        return false;
    }
    if (私有_任务方法候选已存在(任务虚拟存在, 方法虚拟存在)) {
        return true;
    }

    // 候选集允许一个任务保留多个解法；同一特征类型可出现多次，每个值指向一个方法虚拟存在。
    auto* 候选特征 = 世界树.创建特征_按类型(
        任务虚拟存在,
        自我特征定义类::类型_任务_方法候选集());
    return 候选特征
        && 世界树.写入特征_指针(候选特征, 方法虚拟存在, now);
}

// 功能：从指定来源读取数据或状态。
std::vector<存在节点类*> 任务类::读取任务方法候选虚拟存在集(
    const 节点类* 任务节点,
    场景节点类* 承载场景) noexcept
{
    (void)承载场景;
    std::vector<存在节点类*> 结果{};
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务节点));
    if (!头结点) {
        return 结果;
    }

    auto* 任务虚拟存在 = reinterpret_cast<存在节点类*>(头结点->主信息.任务虚拟存在.指针);
    if (!任务虚拟存在 && !头结点->主信息.任务虚拟存在.主键.empty()) {
        任务虚拟存在 = 世界树.按主键解析存在节点(头结点->主信息.任务虚拟存在.主键);
    }
    if (!任务虚拟存在) {
        return 结果;
    }

    const auto* 特征类型 = 自我特征定义类::类型_任务_方法候选集();
    for (auto* 特征 : 世界树.获取子特征(任务虚拟存在)) {
        const auto* 主信息 = 世界树.取特征主信息(特征);
        if (!主信息 || !语素入口同一(主信息->类型, 特征类型)) {
            continue;
        }
        void* 指针 = nullptr;
        if (世界树.读取特征_指针(特征, 指针)) {
            (void)私有_追加唯一存在节点(
                结果,
                reinterpret_cast<存在节点类*>(指针));
        }
    }
    return 结果;
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务选中自我能力值(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(
        任务头结点,
        自我特征定义类::类型_任务_选中自我能力(),
        默认值,
        承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务选中自我能力值(
    节点类* 任务头结点,
    I64 值,
    场景节点类* 承载场景,
    时间戳 now)
{
    return 私有_写入任务I64特征(
        任务头结点,
        自我特征定义类::类型_任务_选中自我能力(),
        值,
        承载场景,
        now);
}

// 功能：从指定来源读取数据或状态。
枚举_任务角色 任务类::读取任务角色(
    const 节点类* 任务头结点,
    枚举_任务角色 默认值,
    场景节点类* 承载场景)
{
    return static_cast<枚举_任务角色>(
        私有_读取任务I64特征(
            任务头结点,
            自我特征定义类::类型_任务_角色(),
            static_cast<I64>(默认值),
            承载场景));
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务角色(
    节点类* 任务头结点,
    枚举_任务角色 值,
    场景节点类* 承载场景,
    时间戳 now)
{
    return 私有_写入任务I64特征(
        任务头结点,
        自我特征定义类::类型_任务_角色(),
        static_cast<I64>(值),
        承载场景,
        now);
}

// 功能：从指定来源读取数据或状态。
枚举_任务阶段 任务类::读取任务当前阶段(
    const 节点类* 任务头结点,
    枚举_任务阶段 默认值,
    场景节点类* 承载场景)
{
    const auto 已记录阶段 = static_cast<枚举_任务阶段>(
        私有_读取任务I64特征(
            任务头结点,
            自我特征定义类::类型_任务_当前阶段(),
            static_cast<I64>(默认值),
            承载场景));
    if (已记录阶段 != 枚举_任务阶段::未定义) {
        return 已记录阶段;
    }
    auto* 头结点 = 私有_归一到任务头结点(const_cast<节点类*>(任务头结点));
    return 头结点 ? 私有_推导任务阶段(读取任务状态(头结点)) : 默认值;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务当前阶段(
    节点类* 任务头结点,
    枚举_任务阶段 值,
    场景节点类* 承载场景,
    时间戳 now)
{
    return 私有_写入任务I64特征(
        任务头结点,
        自我特征定义类::类型_任务_当前阶段(),
        static_cast<I64>(值),
        承载场景,
        now);
}

// 功能：从指定来源读取数据或状态。
bool 任务类::读取任务等待缺口唤醒(const 节点类* 任务头结点, bool 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(
        任务头结点,
        自我特征定义类::类型_任务_等待缺口唤醒(),
        默认值 ? 1 : 0,
        承载场景) != 0;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务等待缺口唤醒(
    节点类* 任务头结点,
    bool 值,
    场景节点类* 承载场景,
    时间戳 now)
{
    return 私有_写入任务I64特征(
        任务头结点,
        自我特征定义类::类型_任务_等待缺口唤醒(),
        值 ? 1 : 0,
        承载场景,
        now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务基准优先级(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务基准优先级(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务基准优先级(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务基准优先级(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务局部优先级偏移(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务局部优先级偏移(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务局部优先级偏移(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务局部优先级偏移(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务调度优先级(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务调度优先级(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务调度优先级(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务调度优先级(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务承接安全权重上限(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务承接安全权重上限(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务承接安全权重上限(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务承接安全权重上限(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务承接服务权重上限(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务承接服务权重上限(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务承接服务权重上限(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务承接服务权重上限(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务权重评分(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务权重评分(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务权重评分(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务权重评分(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务权重版本号(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务权重版本号(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务权重版本号(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务权重版本号(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务已折算安全值(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务已折算安全值(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务已折算安全值(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务已折算安全值(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务已折算服务值(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务已折算服务值(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务已折算服务值(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务已折算服务值(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务运行次数(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务运行次数(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务运行次数(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务运行次数(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务成功次数(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务成功次数(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务成功次数(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务成功次数(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务失败次数(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务失败次数(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务失败次数(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务失败次数(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务连续失败次数(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务连续失败次数(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务连续失败次数(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务连续失败次数(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务转入缺口需求次数(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务转入缺口需求次数(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务转入缺口需求次数(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务转入缺口需求次数(), 值, 承载场景, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务最近错误码(const 节点类* 任务头结点, I64 默认值, 场景节点类* 承载场景)
{
    return 私有_读取任务I64特征(任务头结点, 私有_特征_任务最近错误码(), 默认值, 承载场景);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 任务类::写入任务最近错误码(节点类* 任务头结点, I64 值, 场景节点类* 承载场景, 时间戳 now)
{
    return 私有_写入任务I64特征(任务头结点, 私有_特征_任务最近错误码(), 值, 承载场景, now);
}

// 功能：同步线程、任务、场景或对象之间的状态。
bool 任务类::同步任务特征使用统计(
    节点类* 任务头结点,
    const 结构_特征使用统计& 统计,
    枚举_特征生命周期 生命周期,
    场景节点类* 承载场景,
    时间戳 now)
{
    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) return false;
    if (now == 0) now = 结构体_时间戳::当前_微秒();

    (void)初始化任务虚拟存在信息(头结点, 承载场景, now);
    auto* 任务虚拟存在 = 取或创建_任务虚拟存在(头结点, 承载场景);
    if (!任务虚拟存在) return false;

    return 私有_写入任务特征使用统计(任务虚拟存在, 统计, 生命周期, now);
}

// 功能：从指定来源读取数据或状态。
I64 任务类::读取任务特征账本生命周期标准特征(
    节点类* 任务头结点,
    I64 默认值,
    场景节点类* 承载场景,
    时间戳 now)
{
    auto* 头结点 = 私有_归一到任务头结点(任务头结点);
    if (!头结点) return 默认值;
    if (now == 0) now = 结构体_时间戳::当前_微秒();

    (void)初始化任务虚拟存在信息(头结点, 承载场景, now);
    auto* 任务虚拟存在 = 取或创建_任务虚拟存在(头结点, 承载场景);
    I64 当前值 = 默认值;
    if (任务虚拟存在 && 世界树.读取特征_I64(任务虚拟存在, 私有_特征_任务特征账本生命周期(), 当前值)) {
        return 当前值;
    }
    return 默认值;
}

// 功能：创建并返回或登记对应对象。
任务类::节点类* 任务类::创建任务头结点(
    需求节点类* 来源需求,
    节点类* 上级任务节点,
    时间戳 now)
{
    if (!来源需求) {
        return nullptr;
    }
    if (now == 0) {
        now = 结构体_时间戳::当前_微秒();
    }

    auto* 父任务头结点 = 私有_归一到任务头结点(上级任务节点);
    auto* 场景 = 私有_推导任务场景(来源需求, 上级任务节点);

    任务主信息类 主信息{};
    主信息.名称 = 私有_默认任务名称词(来源需求);
    主信息.类型 = 私有_默认任务类型词(来源需求);

    主信息.创建时间 = now;
    const auto 基准优先级 = 私有_推导基准优先级(来源需求, 父任务头结点);
    私有_写入引用(主信息.目标状态, 私有_取需求目标状态(来源需求));
    私有_写入引用(主信息.实际结果状态, 私有_取需求当前状态(来源需求));
    if (场景) {
        私有_写入引用(主信息.场景, 场景);
    }

    if (父任务头结点) {
    }

    auto* 新节点 = 上级任务节点
        ? 私有_独立树添加子节点(上级任务节点, 主信息)
        : static_cast<节点类*>(添加子节点(this->根指针, 主信息));
    if (!新节点) {
        return nullptr;
    }
    (void)任务类::绑定对应需求(
        新节点,
        reinterpret_cast<需求节点类*>(来源需求));
    (void)任务类::写入任务节点种类(新节点, 枚举_任务节点种类::头结点, 场景, now);
    (void)任务类::写入任务状态(新节点, 枚举_任务状态::未启动, 场景, now);
    (void)任务类::写入任务当前阶段(新节点, 枚举_任务阶段::筹办, 场景, now);
    (void)任务类::写入任务基准优先级(新节点, 基准优先级, 场景, now);
    (void)任务类::写入任务调度优先级(新节点, 基准优先级, 场景, now);

    (void)需求类::绑定对应任务(
        来源需求,
        reinterpret_cast<任务节点类*>(新节点));

    (void)初始化任务虚拟存在信息(
        新节点,
        场景,
        now);
    return 新节点;
}
