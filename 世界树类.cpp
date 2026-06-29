#include "世界树类.h"

// 文件头部规则注释模块：
// 1. 修改本文件前先阅读本模块；世界树事实写入必须保持存在、场景、特征、状态、动态等项目结构承载。
// 2. SQL 投影和字段恢复比对只用于显示 / 诊断 / 存储验证，不得反向作为世界树业务判断来源。
// 3. 本模块只供人读，不参与机器判断；业务状态仍必须由项目通用结构承载。

#include <algorithm>
#include <bit>
#include <cmath>
#include <initializer_list>
#include <limits>
#include <mutex>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "日志接入.h"
#include "度量模板注册表桥接.h"
#include "语素类.h"

import 二次特征应用模块;
import 数据库ADO模块;
import 全局共享函数类;

namespace {

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_存在概念入口性(const char* 名称) noexcept
    {
        return 名称 && *名称
            ? 语素集.添加信息入口词(名称, 枚举_信息入口类型::存在概念入口)
            : nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征模板入口性(const char* 名称) noexcept
    {
        return 名称 && *名称
            ? 语素集.添加信息入口词(名称, 枚举_信息入口类型::特征模板入口)
            : nullptr;
    }

    // 功能：解析已绑定的存在概念入口；不创建新概念。
    存在节点类* 私有_解析已绑定存在概念(const 语素入口节点类* 类型词) noexcept
    {
        auto* 基础节点 = 类型词 ? 语素集.查找语素入口对应基础信息(类型词) : nullptr;
        return dynamic_cast<存在节点类*>(基础节点);
    }

    // 功能：把已绑定类型概念补入运行存在概念集。
    bool 私有_补齐类型概念集(存在类& 存在服务, 存在节点类* 实例, const 语素入口节点类* 类型词) noexcept
    {
        auto* 概念 = 私有_解析已绑定存在概念(类型词);
        return 概念 ? 存在服务.添加实例概念(实例, 概念, false) : false;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在场景绝对坐标X() noexcept
    {
        return 私有_特征模板入口性("存在_场景绝对坐标X");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在场景绝对坐标Y() noexcept
    {
        return 私有_特征模板入口性("存在_场景绝对坐标Y");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在场景绝对坐标Z() noexcept
    {
        return 私有_特征模板入口性("存在_场景绝对坐标Z");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在场景绝对坐标明确状态() noexcept
    {
        return 私有_特征模板入口性("存在_场景绝对坐标明确状态");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在坐标来源() noexcept
    {
        return 私有_特征模板入口性("存在_坐标来源");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在坐标时间戳() noexcept
    {
        return 私有_特征模板入口性("存在_坐标时间戳");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_存在坐标置信度() noexcept
    {
        return 私有_特征模板入口性("存在_坐标置信度");
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_三维体素模型() noexcept
    {
        return 私有_特征模板入口性("三维体素模型");
    }

    // 功能：为出生期自我存在补齐最小边界体素模型，不用于外部存在事实入账。
    bool 私有_确保自我最小边界体素模型(
        世界树类& 世界树对象,
        特征类& 特征服务,
        存在节点类* 自我存在,
        时间戳 now)
    {
        if (!自我存在) return false;

        auto* 体素特征 = 特征服务.取或创建子特征_按类型(
            reinterpret_cast<基础信息节点类*>(自我存在),
            私有_特征_三维体素模型());
        if (!体素特征) return false;

        VecU句柄 已有根句柄{};
        if (特征服务.读取三维体素特征值(体素特征, 已有根句柄)) {
            return true;
        }

        constexpr std::uint32_t 宽度 = 1;
        constexpr std::uint32_t 高度 = 1;
        constexpr std::uint32_t 深度 = 1;
        constexpr std::uint32_t 最小体素边长_mm = 1;
        auto 占据位块 = 特征类::创建三维体素长方体占据位块(宽度, 高度, 深度);
        if (占据位块.empty()
            || !特征类::写入三维体素长方体占据位(占据位块, 宽度, 高度, 深度, 0, 0, 0, true)) {
            return false;
        }

        const auto 写入结果 = 特征服务.写入三维体素长方体二分层链(
            世界树对象.值池(),
            体素特征,
            占据位块,
            宽度,
            高度,
            深度,
            最小体素边长_mm,
            now);
        if (写入结果.成功) {
            项目运行日志("自我存在最小边界体素模型已补齐 | 来源=世界树自我初始化 | 宽度=1 | 高度=1 | 深度=1");
        }
        return 写入结果.成功;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::vector<基础信息节点类*> 私有_直接基础信息子节点(
        const 基础信息类& 基础信息,
        const 基础信息节点类* 父节点)
    {
        std::vector<基础信息节点类*> 结果{};
        auto lk = 基础信息.获取读锁();
        auto* 父 = 父节点 ? const_cast<基础信息节点类*>(父节点) : 基础信息.世界根();
        if (!父 || !父->子) return 结果;

        auto* 首节点 = static_cast<基础信息节点类*>(父->子);
        auto* 当前 = 首节点;
        do {
            结果.push_back(当前);
            当前 = static_cast<基础信息节点类*>(当前->下);
        } while (当前 && 当前 != 首节点);
        return 结果;
    }

    template<class T主信息>
    std::vector<基础信息节点类*> 私有_直接基础信息子节点_按类型(
        const 基础信息类& 基础信息,
        const 基础信息节点类* 父节点)
    {
        std::vector<基础信息节点类*> 结果{};
        auto lk = 基础信息.获取读锁();
        auto* 父 = 父节点 ? const_cast<基础信息节点类*>(父节点) : 基础信息.世界根();
        if (!父 || !父->子) return 结果;

        auto* 首节点 = static_cast<基础信息节点类*>(父->子);
        auto* 当前 = 首节点;
        do {
            if (dynamic_cast<T主信息*>(当前->主信息)) {
                结果.push_back(当前);
            }
            当前 = static_cast<基础信息节点类*>(当前->下);
        } while (当前 && 当前 != 首节点);
        return 结果;
    }

    // 功能：确保目标结构、状态或前置条件存在并可用。
    bool 私有_确保概念特征槽(世界树类& 世界树对象, 存在节点类* 概念, const char* 特征名) noexcept
    {
        if (!概念 || !特征名 || !*特征名) return false;
        const auto* 特征类型 = 私有_特征模板入口性(特征名);
        if (!特征类型) return false;

        // 概念默认槽优先引用已初始化的抽象特征；这样“方法状态”等槽位
        // 不是孤立同名特征，而能直接命中抽象状态值域。
        auto* 抽象根 = 世界树对象.取或创建抽象特征根();
        auto* 抽象特征 = 世界树对象.特征().查找子抽象特征_按类型(抽象根, 特征类型);
        if (!抽象特征) {
            return 世界树对象.确保特征(概念, 特征类型) != nullptr;
        }

        if (auto* 既有槽 = 世界树对象.查找子特征_按类型(概念, 特征类型)) {
            if (auto* 主信息 = 世界树对象.特征().取特征主信息(既有槽)) {
                主信息->抽象特征.绑定(抽象特征);
                return true;
            }
            return false;
        }

        return 世界树对象.取或创建实例特征_按抽象特征(概念, 抽象特征) != nullptr;
    }

    // 功能：确保目标结构、状态或前置条件存在并可用。
    bool 私有_确保概念特征槽集合(
        世界树类& 世界树对象,
        存在节点类* 概念,
        std::initializer_list<const char*> 特征名集) noexcept
    {
        if (!概念) return false;
        bool 全部成功 = true;
        for (const char* 特征名 : 特征名集) {
            全部成功 = 私有_确保概念特征槽(世界树对象, 概念, 特征名) && 全部成功;
        }
        return 全部成功;
    }

    // 功能：服务所在模块的内部辅助流程。
    I64 私有_VecU差异率Q10000(const VecIU64& 左值, const VecIU64& 右值) noexcept
    {
        const std::size_t 块数 = std::max(左值.size(), 右值.size());
        if (块数 == 0) return 0;

        std::uint64_t 差异位数 = 0;
        const auto 公共块数 = std::min(左值.size(), 右值.size());
        for (std::size_t i = 0; i < 公共块数; ++i) {
            差异位数 += static_cast<std::uint64_t>(std::popcount(左值[i] ^ 右值[i]));
        }
        if (左值.size() > 公共块数) {
            差异位数 += static_cast<std::uint64_t>(左值.size() - 公共块数) * 64ull;
        }
        if (右值.size() > 公共块数) {
            差异位数 += static_cast<std::uint64_t>(右值.size() - 公共块数) * 64ull;
        }

        const std::uint64_t 总位数 = static_cast<std::uint64_t>(块数) * 64ull;
        return static_cast<I64>((差异位数 * 10000ull) / (总位数 ? 总位数 : 1ull));
    }

    // 功能：服务所在模块的内部辅助流程。
    场景节点类* 私有_取存在所在场景(
        const 基础信息类& 基础信息,
        const 存在节点类* 存在) noexcept
    {
        auto* 父节点 = 存在 ? static_cast<基础信息节点类*>(存在->父) : nullptr;
        return 父节点 && 基础信息.取主信息<场景节点主信息类>(父节点)
            ? static_cast<场景节点类*>(父节点)
            : nullptr;
    }

    struct 结构_世界树SQL节点行 {
        int 行号 = 0;
        const 基础信息节点类* 节点指针 = nullptr;
        std::string 节点主键{};
        std::string 父节点主键{};
        int 深度 = 0;
        int 同层序号 = 0;
        int 直接子数量 = 0;
        std::string 路径{};
        bool 有主信息类型 = false;
        int 主信息类型值 = 0;
        std::string 名称主键{};
        std::string 类型主键{};
        std::string 名称文本{};
        std::string 类型文本{};
    };

    struct 结构_世界树SQL关系行 {
        int 行号 = 0;
        int 宿主行号 = 0;
        int 目标行号 = 0;
        const 基础信息节点类* 基础目标指针 = nullptr;
        std::string 宿主主键{};
        std::string 关系名{};
        std::string 目标类别{};
        std::string 目标主键{};
        int 序号 = 0;
    };

    enum class 枚举_世界树SQL主信息表 {
        指代,
        抽象特征,
        特征,
        存在,
        场景,
        状态,
        动态,
        二次特征,
        因果,
        语言记录,
        其他,
    };

    std::mutex& 私有_世界树SQL投影互斥() noexcept
    {
        static std::mutex 互斥{};
        return 互斥;
    }

    std::string 私有_世界树SQL字符串(const std::string& 文本, const bool 空为NULL = true)
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

    std::string 私有_世界树SQL入口主键(const 语素入口节点类* 入口)
    {
        return 入口 ? 入口->获取主键() : std::string{};
    }

    std::string 私有_世界树SQL入口文本(const 语素入口节点类* 入口)
    {
        if (!入口) {
            return {};
        }
        const auto 词面 = 语素集.安全获取词(入口);
        return 词面.empty() ? 入口->获取主键() : 词面;
    }

    const char* 私有_世界树SQL主信息类型文本(const 枚举_主信息类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_主信息类型::指代: return "指代";
        case 枚举_主信息类型::特征: return "特征";
        case 枚举_主信息类型::抽象特征: return "抽象特征";
        case 枚举_主信息类型::存在: return "存在";
        case 枚举_主信息类型::状态: return "状态";
        case 枚举_主信息类型::场景: return "场景";
        case 枚举_主信息类型::动态: return "动态";
        case 枚举_主信息类型::因果: return "因果";
        case 枚举_主信息类型::二次特征_修饰: return "二次特征";
        case 枚举_主信息类型::语言信息_仅记录: return "语言信息_仅记录";
        case 枚举_主信息类型::基础信息基类: return "基础信息基类";
        case 枚举_主信息类型::未定义: return "未定义";
        default: return "其它";
        }
    }

    const char* 私有_世界树SQL世界类型文本(const 枚举_世界类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_世界类型::现实世界: return "现实世界";
        case 枚举_世界类型::文本世界: return "文本世界";
        case 枚举_世界类型::想象世界: return "想象世界";
        case 枚举_世界类型::记忆世界: return "记忆世界";
        case 枚举_世界类型::推理世界: return "推理世界";
        case 枚举_世界类型::内部世界: return "内部世界";
        case 枚举_世界类型::虚拟世界: return "虚拟世界";
        default: return "其它";
        }
    }

    // 功能：按主信息运行类型选择 SQL 投影分表。
    枚举_世界树SQL主信息表 私有_世界树SQL主信息分表(const 基础信息基类* 主信息) noexcept
    {
        if (dynamic_cast<const 指代节点主信息类*>(主信息)) return 枚举_世界树SQL主信息表::指代;
        if (dynamic_cast<const 抽象特征主信息类*>(主信息)) return 枚举_世界树SQL主信息表::抽象特征;
        if (dynamic_cast<const 特征节点主信息类*>(主信息)) return 枚举_世界树SQL主信息表::特征;
        if (dynamic_cast<const 存在节点主信息类*>(主信息)) return 枚举_世界树SQL主信息表::存在;
        if (dynamic_cast<const 场景节点主信息类*>(主信息)) return 枚举_世界树SQL主信息表::场景;
        if (dynamic_cast<const 状态节点主信息类*>(主信息)) return 枚举_世界树SQL主信息表::状态;
        if (dynamic_cast<const 动态节点主信息类*>(主信息)) return 枚举_世界树SQL主信息表::动态;
        if (dynamic_cast<const 二次特征主信息类*>(主信息)) return 枚举_世界树SQL主信息表::二次特征;
        if (dynamic_cast<const 因果主信息类*>(主信息)) return 枚举_世界树SQL主信息表::因果;
        if (dynamic_cast<const 语言记录主信息类*>(主信息)) return 枚举_世界树SQL主信息表::语言记录;
        return 枚举_世界树SQL主信息表::其他;
    }

    // 功能：返回世界树 SQL 投影主信息分表名。
    const char* 私有_世界树SQL主信息表名(const 枚举_世界树SQL主信息表 表) noexcept
    {
        switch (表) {
        case 枚举_世界树SQL主信息表::指代: return "[鱼巢].[世界树指代主信息]";
        case 枚举_世界树SQL主信息表::抽象特征: return "[鱼巢].[世界树抽象特征主信息]";
        case 枚举_世界树SQL主信息表::特征: return "[鱼巢].[世界树特征主信息]";
        case 枚举_世界树SQL主信息表::存在: return "[鱼巢].[世界树存在主信息]";
        case 枚举_世界树SQL主信息表::场景: return "[鱼巢].[世界树场景主信息]";
        case 枚举_世界树SQL主信息表::状态: return "[鱼巢].[世界树状态主信息]";
        case 枚举_世界树SQL主信息表::动态: return "[鱼巢].[世界树动态主信息]";
        case 枚举_世界树SQL主信息表::二次特征: return "[鱼巢].[世界树二次特征主信息]";
        case 枚举_世界树SQL主信息表::因果: return "[鱼巢].[世界树因果主信息]";
        case 枚举_世界树SQL主信息表::语言记录: return "[鱼巢].[世界树语言记录主信息]";
        default: return "[鱼巢].[世界树其他主信息]";
        }
    }

    // 功能：返回主信息分表统一列名；只存身份、底层类型和控制面板可读词面。
    const char* 私有_世界树SQL主信息通用列名() noexcept
    {
        return "[快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本]";
    }

    // 功能：返回主信息分表统一建表列定义；词面文本只作为 SQL 控制面板显示镜像。
    const char* 私有_世界树SQL主信息通用列定义() noexcept
    {
        return "    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,\n"
            "    [快照标识] uniqueidentifier NOT NULL,\n"
            "    [节点行号] int NOT NULL,\n"
            "    [节点主键] nvarchar(80) NOT NULL,\n"
            "    [主信息类型值] int NULL,\n"
            "    [名称主键] nvarchar(80) NULL,\n"
            "    [类型主键] nvarchar(80) NULL,\n"
            "    [名称文本] nvarchar(300) NULL,\n"
            "    [类型文本] nvarchar(300) NULL";
    }

    // 功能：输出主信息分表统一字段值。
    void 私有_世界树SQL写主信息通用值(std::ostringstream& SQL, const 结构_世界树SQL节点行& 行)
    {
        SQL << "@快照标识, "
            << 行.行号 << ", "
            << 私有_世界树SQL字符串(行.节点主键, false) << ", "
            << SQL可空整数文本(行.有主信息类型, 行.主信息类型值) << ", "
            << 私有_世界树SQL字符串(行.名称主键) << ", "
            << 私有_世界树SQL字符串(行.类型主键) << ", "
            << 私有_世界树SQL字符串(行.名称文本) << ", "
            << 私有_世界树SQL字符串(行.类型文本);
    }

    // 功能：返回主信息分表的大类附加列定义。
    const char* 私有_世界树SQL主信息附加列定义(const 枚举_世界树SQL主信息表 表) noexcept
    {
        switch (表) {
        case 枚举_世界树SQL主信息表::指代:
            return ",\n    [代词主键] nvarchar(80) NULL,\n"
                "    [有指代对象] int NOT NULL";
        case 枚举_世界树SQL主信息表::抽象特征:
            return ",\n    [形态值] int NOT NULL,\n"
                "    [有值域] int NOT NULL,\n"
                "    [有区间] int NOT NULL,\n"
                "    [来源实例特征数量] int NOT NULL";
        case 枚举_世界树SQL主信息表::特征:
            return ",\n    [当前值类型值] int NOT NULL,\n"
                "    [当前值I64] bigint NULL,\n"
                "    [当前值VecU指针] decimal(20,0) NULL,\n"
                "    [当前值指针] decimal(20,0) NULL,\n"
                "    [当前稳态索引] int NOT NULL,\n"
                "    [稳态数量] int NOT NULL";
        case 枚举_世界树SQL主信息表::存在:
            return ",\n    [概念数量] int NOT NULL,\n"
                "    [有概念模板] int NOT NULL,\n"
                "    [有内部世界] int NOT NULL,\n"
                "    [有需求根] int NOT NULL,\n"
                "    [有任务根] int NOT NULL,\n"
                "    [有方法根] int NOT NULL,\n"
                "    [有位置历史] int NOT NULL,\n"
                "    [最近位移mm] bigint NOT NULL,\n"
                "    [连续静止帧] bigint NOT NULL,\n"
                "    [连续未命中帧] bigint NOT NULL";
        case 枚举_世界树SQL主信息表::场景:
            return ",\n    [世界类型值] int NOT NULL,\n"
                "    [状态索引数量] int NOT NULL,\n"
                "    [动态索引数量] int NOT NULL,\n"
                "    [二次特征索引数量] int NOT NULL";
        case 枚举_世界树SQL主信息表::状态:
            return ",\n    [状态值类型值] int NOT NULL,\n"
                "    [状态值I64] bigint NULL,\n"
                "    [状态值VecU指针] decimal(20,0) NULL,\n"
                "    [状态值指针] decimal(20,0) NULL,\n"
                "    [收到时间] bigint NOT NULL,\n"
                "    [发生时间] bigint NOT NULL,\n"
                "    [是否变化] int NOT NULL";
        case 枚举_世界树SQL主信息表::动态:
            return ",\n    [时间起] bigint NOT NULL,\n"
                "    [时间止] bigint NOT NULL,\n"
                "    [来源执行成功] int NOT NULL,\n"
                "    [来源错误码] bigint NOT NULL,\n"
                "    [动态路径签名] decimal(20,0) NOT NULL,\n"
                "    [动态层级] int NOT NULL,\n"
                "    [聚合方式值] int NOT NULL";
        case 枚举_世界树SQL主信息表::二次特征:
            return ",\n    [形态值] int NOT NULL,\n"
                "    [种类值] int NOT NULL,\n"
                "    [域值] int NOT NULL,\n"
                "    [粒度值] int NOT NULL,\n"
                "    [标量值] bigint NOT NULL,\n"
                "    [离散编码] int NOT NULL";
        case 枚举_世界树SQL主信息表::因果:
            return ",\n    [适用锚点类型值] int NOT NULL,\n"
                "    [适用层级] int NOT NULL,\n"
                "    [观察次数] decimal(20,0) NOT NULL,\n"
                "    [条件命中次数] decimal(20,0) NOT NULL,\n"
                "    [因出现次数] decimal(20,0) NOT NULL,\n"
                "    [果出现次数] decimal(20,0) NOT NULL,\n"
                "    [成立次数] decimal(20,0) NOT NULL,\n"
                "    [失败次数] decimal(20,0) NOT NULL,\n"
                "    [最近命中时间] bigint NOT NULL,\n"
                "    [最近失败时间] bigint NOT NULL,\n"
                "    [已验证] int NOT NULL";
        case 枚举_世界树SQL主信息表::语言记录:
            return ",\n    [语言词性主键] nvarchar(80) NULL";
        default:
            return "";
        }
    }

    // 功能：返回主信息分表的大类附加列名。
    const char* 私有_世界树SQL主信息附加列名(const 枚举_世界树SQL主信息表 表) noexcept
    {
        switch (表) {
        case 枚举_世界树SQL主信息表::指代:
            return ", [代词主键], [有指代对象]";
        case 枚举_世界树SQL主信息表::抽象特征:
            return ", [形态值], [有值域], [有区间], [来源实例特征数量]";
        case 枚举_世界树SQL主信息表::特征:
            return ", [当前值类型值], [当前值I64], [当前值VecU指针], [当前值指针], [当前稳态索引], [稳态数量]";
        case 枚举_世界树SQL主信息表::存在:
            return ", [概念数量], [有概念模板], [有内部世界], [有需求根], [有任务根], [有方法根], [有位置历史], [最近位移mm], [连续静止帧], [连续未命中帧]";
        case 枚举_世界树SQL主信息表::场景:
            return ", [世界类型值], [状态索引数量], [动态索引数量], [二次特征索引数量]";
        case 枚举_世界树SQL主信息表::状态:
            return ", [状态值类型值], [状态值I64], [状态值VecU指针], [状态值指针], [收到时间], [发生时间], [是否变化]";
        case 枚举_世界树SQL主信息表::动态:
            return ", [时间起], [时间止], [来源执行成功], [来源错误码], [动态路径签名], [动态层级], [聚合方式值]";
        case 枚举_世界树SQL主信息表::二次特征:
            return ", [形态值], [种类值], [域值], [粒度值], [标量值], [离散编码]";
        case 枚举_世界树SQL主信息表::因果:
            return ", [适用锚点类型值], [适用层级], [观察次数], [条件命中次数], [因出现次数], [果出现次数], [成立次数], [失败次数], [最近命中时间], [最近失败时间], [已验证]";
        case 枚举_世界树SQL主信息表::语言记录:
            return ", [语言词性主键]";
        default:
            return "";
        }
    }

    // 功能：把特征值类型映射为 SQL 投影底层类型值。
    int 私有_世界树SQL特征值类型值(const 特征值& 值) noexcept
    {
        if (std::holds_alternative<I64>(值)) return 1;
        if (std::holds_alternative<VecU句柄>(值)) return 2;
        if (std::holds_alternative<指针句柄>(值)) return 3;
        return 0;
    }

    // 功能：输出特征值 I64 底层列。
    std::string 私有_世界树SQL特征值I64文本(const 特征值* 值)
    {
        return 值 && std::holds_alternative<I64>(*值)
            ? std::to_string(std::get<I64>(*值))
            : "NULL";
    }

    // 功能：输出特征值 VecU 句柄底层列。
    std::string 私有_世界树SQL特征值VecU文本(const 特征值* 值)
    {
        return 值 && std::holds_alternative<VecU句柄>(*值)
            ? std::to_string(static_cast<unsigned long long>(std::get<VecU句柄>(*值).主信息指针))
            : "NULL";
    }

    // 功能：输出特征值指针句柄底层列。
    std::string 私有_世界树SQL特征值指针文本(const 特征值* 值)
    {
        return 值 && std::holds_alternative<指针句柄>(*值)
            ? std::to_string(static_cast<unsigned long long>(std::get<指针句柄>(*值).指针))
            : "NULL";
    }

    // 功能：输出主信息分表的大类附加字段值。
    void 私有_世界树SQL写主信息附加值(
        std::ostringstream& SQL,
        const 结构_世界树SQL节点行& 行,
        const 枚举_世界树SQL主信息表 表)
    {
        const auto* 主信息 = 行.节点指针 ? 行.节点指针->主信息 : nullptr;
        switch (表) {
        case 枚举_世界树SQL主信息表::指代: {
            const auto* 指代 = dynamic_cast<const 指代节点主信息类*>(主信息);
            const bool 有对象 = 指代 && (指代->指代对象.指针 || !指代->指代对象.主键.empty());
            SQL << ", " << 私有_世界树SQL字符串(指代 ? 私有_世界树SQL入口主键(指代->代词) : "")
                << ", " << (有对象 ? 1 : 0);
            break;
        }
        case 枚举_世界树SQL主信息表::抽象特征: {
            const auto* 抽象 = dynamic_cast<const 抽象特征主信息类*>(主信息);
            SQL << ", " << (抽象 ? static_cast<int>(抽象->形态) : 0)
                << ", " << (抽象 && 抽象->值域.has_value() ? 1 : 0)
                << ", " << (抽象 && 抽象->区间.has_value() ? 1 : 0)
                << ", " << (抽象 ? 抽象->来源实例特征.size() : 0);
            break;
        }
        case 枚举_世界树SQL主信息表::特征: {
            const auto* 特征 = dynamic_cast<const 特征节点主信息类*>(主信息);
            const auto* 值 = 特征 ? &特征->当前值 : nullptr;
            SQL << ", " << (值 ? 私有_世界树SQL特征值类型值(*值) : 0)
                << ", " << 私有_世界树SQL特征值I64文本(值)
                << ", " << 私有_世界树SQL特征值VecU文本(值)
                << ", " << 私有_世界树SQL特征值指针文本(值)
                << ", " << (特征 ? 特征->当前稳态索引 : -1)
                << ", " << (特征 ? 特征->稳态列表.size() : 0);
            break;
        }
        case 枚举_世界树SQL主信息表::存在: {
            const auto* 存在 = dynamic_cast<const 存在节点主信息类*>(主信息);
            SQL << ", " << (存在 ? 存在->概念集.size() : 0)
                << ", " << (存在 && (存在->概念模板.指针 || !存在->概念模板.主键.empty()) ? 1 : 0)
                << ", " << (存在 && (存在->内部世界.指针 || !存在->内部世界.主键.empty()) ? 1 : 0)
                << ", " << (存在 && 存在->需求根节点 ? 1 : 0)
                << ", " << (存在 && 存在->任务根节点 ? 1 : 0)
                << ", " << (存在 && 存在->方法根节点 ? 1 : 0)
                << ", " << (存在 && 存在->有位置历史 ? 1 : 0)
                << ", " << (存在 ? 存在->最近位移_mm : 0)
                << ", " << (存在 ? 存在->连续静止帧 : 0)
                << ", " << (存在 ? 存在->连续未命中帧 : 0);
            break;
        }
        case 枚举_世界树SQL主信息表::场景: {
            const auto* 场景 = dynamic_cast<const 场景节点主信息类*>(主信息);
            SQL << ", " << (场景 ? static_cast<int>(场景->世界类型) : 0)
                << ", " << (场景 ? 场景->状态索引.size() : 0)
                << ", " << (场景 ? 场景->动态索引.size() : 0)
                << ", " << (场景 ? 场景->二次特征索引.size() : 0);
            break;
        }
        case 枚举_世界树SQL主信息表::状态: {
            const auto* 状态 = dynamic_cast<const 状态节点主信息类*>(主信息);
            const auto* 值 = 状态 ? &状态->状态值 : nullptr;
            SQL << ", " << (值 ? 私有_世界树SQL特征值类型值(*值) : 0)
                << ", " << 私有_世界树SQL特征值I64文本(值)
                << ", " << 私有_世界树SQL特征值VecU文本(值)
                << ", " << 私有_世界树SQL特征值指针文本(值)
                << ", " << (状态 ? 状态->收到时间 : 0)
                << ", " << (状态 ? 状态->发生时间 : 0)
                << ", " << (状态 && 状态->是否变化 ? 1 : 0);
            break;
        }
        case 枚举_世界树SQL主信息表::动态: {
            const auto* 动态 = dynamic_cast<const 动态节点主信息类*>(主信息);
            SQL << ", " << (动态 ? 动态->时间.起 : 0)
                << ", " << (动态 ? 动态->时间.止 : 0)
                << ", " << (动态 && 动态->来源执行成功 ? 1 : 0)
                << ", " << (动态 ? 动态->来源错误码 : 0)
                << ", " << (动态 ? 动态->动态路径签名 : 0)
                << ", " << (动态 ? 动态->动态层级 : 0)
                << ", " << (动态 ? static_cast<int>(动态->聚合方式) : 0);
            break;
        }
        case 枚举_世界树SQL主信息表::二次特征: {
            const auto* 二次 = dynamic_cast<const 二次特征主信息类*>(主信息);
            SQL << ", " << (二次 ? static_cast<int>(二次->形态) : 0)
                << ", " << (二次 ? static_cast<int>(二次->种类) : 0)
                << ", " << (二次 ? static_cast<int>(二次->域) : 0)
                << ", " << (二次 ? static_cast<int>(二次->粒度) : 0)
                << ", " << (二次 ? 二次->标量值 : 0)
                << ", " << (二次 ? 二次->离散编码 : 0);
            break;
        }
        case 枚举_世界树SQL主信息表::因果: {
            const auto* 因果 = dynamic_cast<const 因果主信息类*>(主信息);
            SQL << ", " << (因果 ? static_cast<int>(因果->适用锚点类型) : 0)
                << ", " << (因果 ? 因果->适用层级 : 0)
                << ", " << (因果 ? 因果->观察次数 : 0)
                << ", " << (因果 ? 因果->条件命中次数 : 0)
                << ", " << (因果 ? 因果->因出现次数 : 0)
                << ", " << (因果 ? 因果->果出现次数 : 0)
                << ", " << (因果 ? 因果->成立次数 : 0)
                << ", " << (因果 ? 因果->失败次数 : 0)
                << ", " << (因果 ? 因果->最近命中时间 : 0)
                << ", " << (因果 ? 因果->最近失败时间 : 0)
                << ", " << (因果 && 因果->已验证 ? 1 : 0);
            break;
        }
        case 枚举_世界树SQL主信息表::语言记录: {
            const auto* 语言 = dynamic_cast<const 语言记录主信息类*>(主信息);
            SQL << ", " << 私有_世界树SQL字符串(语言 ? 私有_世界树SQL入口主键(语言->语言词性) : "");
            break;
        }
        default:
            break;
        }
    }

    std::string 私有_世界树SQL特征值类别(const 特征值& 值)
    {
        if (std::holds_alternative<I64>(值)) return "I64";
        if (std::holds_alternative<VecU句柄>(值)) return "VecU句柄";
        if (std::holds_alternative<指针句柄>(值)) return "指针句柄";
        return {};
    }

    std::string 私有_世界树SQL特征值文本(const 特征值& 值)
    {
        if (std::holds_alternative<I64>(值)) {
            return std::to_string(std::get<I64>(值));
        }
        if (std::holds_alternative<VecU句柄>(值)) {
            return "VecU#" + std::to_string(std::get<VecU句柄>(值).主信息指针);
        }
        if (std::holds_alternative<指针句柄>(值)) {
            return "Ptr#" + std::to_string(std::get<指针句柄>(值).指针);
        }
        return {};
    }

    std::string 私有_世界树SQL节点短标题(const 基础信息节点类* 节点)
    {
        if (!节点 || !节点->主信息) {
            return "世界树查询根";
        }
        const auto* 主信息 = 节点->主信息;
        const auto 名称 = 私有_世界树SQL入口文本(主信息->名称);
        const auto 类型 = 私有_世界树SQL入口文本(主信息->类型);
        if (!名称.empty() && !类型.empty() && 名称 != 类型) {
            return 名称 + "(" + 类型 + ")";
        }
        if (!名称.empty()) {
            return 名称;
        }
        if (!类型.empty()) {
            return 类型;
        }
        if (const auto* 特征 = dynamic_cast<const 特征节点主信息类*>(主信息)) {
            const auto 值文本 = 私有_世界树SQL特征值文本(特征->当前值);
            return 值文本.empty() ? "特征" : "特征=" + 值文本;
        }
        if (const auto* 状态 = dynamic_cast<const 状态节点主信息类*>(主信息)) {
            const auto 值文本 = 私有_世界树SQL特征值文本(状态->状态值);
            return 值文本.empty() ? "状态" : "状态=" + 值文本;
        }
        return std::string(私有_世界树SQL主信息类型文本(主信息->主信息类型))
            + "#"
            + 节点->获取主键();
    }

    template<class T节点>
    std::string 私有_世界树SQL引用主键(const 可解析引用<T节点>& 引用)
    {
        if (!引用.主键.empty()) {
            return 引用.主键;
        }
        if constexpr (std::is_same_v<std::remove_cv_t<T节点>, 基础信息节点类>) {
            return 引用.指针 ? 引用.指针->获取主键() : std::string{};
        }
        else {
            return {};
        }
    }

    template<class T节点>
    void 私有_世界树SQL添加引用(
        std::vector<结构_世界树SQL关系行>& 关系集,
        const std::string& 宿主主键,
        const int 宿主行号,
        const char* 关系名,
        const char* 目标类别,
        const 可解析引用<T节点>& 引用,
        const int 序号)
    {
        const auto 目标主键 = 私有_世界树SQL引用主键(引用);
        if (目标主键.empty()) {
            return;
        }
        结构_世界树SQL关系行 行{};
        行.行号 = static_cast<int>(关系集.size() + 1);
        行.宿主行号 = 宿主行号;
        行.宿主主键 = 宿主主键;
        行.关系名 = 关系名 ? 关系名 : "";
        行.目标类别 = 目标类别 ? 目标类别 : "";
        行.目标主键 = 目标主键;
        if constexpr (std::is_same_v<std::remove_cv_t<T节点>, 基础信息节点类>) {
            行.基础目标指针 = 引用.指针;
        }
        行.序号 = 序号;
        关系集.push_back(std::move(行));
    }

    template<class T节点>
    void 私有_世界树SQL添加引用集(
        std::vector<结构_世界树SQL关系行>& 关系集,
        const std::string& 宿主主键,
        const int 宿主行号,
        const char* 关系名,
        const char* 目标类别,
        const std::vector<可解析引用<T节点>>& 引用集)
    {
        int 序号 = 0;
        for (const auto& 引用 : 引用集) {
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, 关系名, 目标类别, 引用, 序号++);
        }
    }

    void 私有_世界树SQL收集关系(
        const 基础信息节点类* 节点,
        const int 宿主行号,
        std::vector<结构_世界树SQL关系行>& 关系集)
    {
        if (!节点 || !节点->主信息) {
            return;
        }
        const auto 宿主主键 = 节点->获取主键();
        const auto* 主信息 = 节点->主信息;
        if (const auto* 指代 = dynamic_cast<const 指代节点主信息类*>(主信息)) {
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "指代对象", "基础信息", 指代->指代对象, 0);
        }
        if (const auto* 抽象 = dynamic_cast<const 抽象特征主信息类*>(主信息)) {
            私有_世界树SQL添加引用集(关系集, 宿主主键, 宿主行号, "来源实例特征", "基础信息", 抽象->来源实例特征);
        }
        if (const auto* 特征 = dynamic_cast<const 特征节点主信息类*>(主信息)) {
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "抽象特征", "基础信息", 特征->抽象特征, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "当前命中抽象特征", "基础信息", 特征->当前命中抽象特征, 0);
        }
        if (const auto* 存在 = dynamic_cast<const 存在节点主信息类*>(主信息)) {
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "概念模板", "基础信息", 存在->概念模板, 0);
            私有_世界树SQL添加引用集(关系集, 宿主主键, 宿主行号, "概念集", "基础信息", 存在->概念集);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "内部世界", "基础信息", 存在->内部世界, 0);
            if (存在->需求根节点) {
                关系集.push_back({ static_cast<int>(关系集.size() + 1), 宿主行号, 0, nullptr, 宿主主键, "需求根节点", "需求", 存在->需求根节点->获取主键(), 0 });
            }
            if (存在->任务根节点) {
                关系集.push_back({ static_cast<int>(关系集.size() + 1), 宿主行号, 0, nullptr, 宿主主键, "任务根节点", "任务", {}, 0 });
            }
            if (存在->方法根节点) {
                关系集.push_back({ static_cast<int>(关系集.size() + 1), 宿主行号, 0, nullptr, 宿主主键, "方法根节点", "方法", {}, 0 });
            }
        }
        if (const auto* 场景 = dynamic_cast<const 场景节点主信息类*>(主信息)) {
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "宿主存在", "基础信息", 场景->宿主存在, 0);
            私有_世界树SQL添加引用集(关系集, 宿主主键, 宿主行号, "状态索引", "基础信息", 场景->状态索引);
            私有_世界树SQL添加引用集(关系集, 宿主主键, 宿主行号, "动态索引", "基础信息", 场景->动态索引);
            私有_世界树SQL添加引用集(关系集, 宿主主键, 宿主行号, "二次特征索引", "基础信息", 场景->二次特征索引);
        }
        if (const auto* 状态 = dynamic_cast<const 状态节点主信息类*>(主信息)) {
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "状态主体", "基础信息", 状态->状态主体, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "状态特征", "基础信息", 状态->状态特征, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "对应信息节点", "基础信息", 状态->对应信息节点, 0);
        }
        if (const auto* 动态 = dynamic_cast<const 动态节点主信息类*>(主信息)) {
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "初始状态", "基础信息", 动态->初始状态, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "结果状态", "基础信息", 动态->结果状态, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "动态主体", "基础信息", 动态->动态主体, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "动态特征", "基础信息", 动态->动态特征, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "来源输入场景", "基础信息", 动态->来源输入场景, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "来源输出场景", "基础信息", 动态->来源输出场景, 0);
            私有_世界树SQL添加引用集(关系集, 宿主主键, 宿主行号, "来源低层动态", "基础信息", 动态->来源低层动态);
        }
        if (const auto* 二次 = dynamic_cast<const 二次特征主信息类*>(主信息)) {
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "概念模板", "基础信息", 二次->概念模板, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "所属场景", "基础信息", 二次->所属场景, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "来源起始状态", "基础信息", 二次->来源起始状态, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "来源结果状态", "基础信息", 二次->来源结果状态, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "来源动态", "基础信息", 二次->来源动态, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "来源场景", "基础信息", 二次->来源场景, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "主体", "基础信息", 二次->主体, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "客体", "基础信息", 二次->客体, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "左对象", "基础信息", 二次->左对象, 0);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "右对象", "基础信息", 二次->右对象, 0);
        }
        if (const auto* 因果 = dynamic_cast<const 因果主信息类*>(主信息)) {
            私有_世界树SQL添加引用集(关系集, 宿主主键, 宿主行号, "条件比较模板", "基础信息", 因果->条件比较模板);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "因方法模板", "方法", 因果->因方法模板, 0);
            私有_世界树SQL添加引用集(关系集, 宿主主键, 宿主行号, "果比较模板", "基础信息", 因果->果比较模板);
            私有_世界树SQL添加引用(关系集, 宿主主键, 宿主行号, "主果比较模板", "基础信息", 因果->主果比较模板, 0);
            私有_世界树SQL添加引用集(关系集, 宿主主键, 宿主行号, "证据动态样本", "基础信息", 因果->证据动态样本);
        }
    }

    void 私有_世界树SQL填充节点主信息(const 基础信息节点类* 节点, 结构_世界树SQL节点行& 行)
    {
        if (!节点 || !节点->主信息) {
            return;
        }
        const auto* 主信息 = 节点->主信息;
        行.有主信息类型 = true;
        行.主信息类型值 = static_cast<int>(主信息->主信息类型);
        行.名称主键 = 私有_世界树SQL入口主键(主信息->名称);
        行.类型主键 = 私有_世界树SQL入口主键(主信息->类型);
        行.名称文本 = 私有_世界树SQL入口文本(主信息->名称);
        行.类型文本 = 私有_世界树SQL入口文本(主信息->类型);
    }

    void 私有_收集世界树SQL节点(
        const 基础信息节点类* 节点,
        const std::string& 父节点主键,
        int 深度,
        int 同层序号,
        const std::string& 父路径,
        std::unordered_set<const 基础信息节点类*>& 已访问,
        std::vector<结构_世界树SQL节点行>& 节点集,
        std::vector<结构_世界树SQL关系行>& 关系集);

    void 私有_收集世界树SQL同层(
        const 基础信息节点类* 首节点,
        const 基础信息节点类* 停止节点,
        const std::string& 父节点主键,
        const int 深度,
        const std::string& 父路径,
        std::unordered_set<const 基础信息节点类*>& 已访问,
        std::vector<结构_世界树SQL节点行>& 节点集,
        std::vector<结构_世界树SQL关系行>& 关系集)
    {
        if (!首节点) {
            return;
        }
        auto* 当前 = 首节点;
        int 同层序号 = 0;
        std::size_t 保护 = 0;
        do {
            if (当前 == 停止节点) {
                break;
            }
            私有_收集世界树SQL节点(
                当前,
                父节点主键,
                深度,
                同层序号,
                父路径,
                已访问,
                节点集,
                关系集);
            当前 = static_cast<const 基础信息节点类*>(当前->下);
            ++同层序号;
            ++保护;
        } while (当前 && 当前 != 首节点 && 当前 != 停止节点 && 保护 < 200000);
    }

    void 私有_收集世界树SQL节点(
        const 基础信息节点类* 节点,
        const std::string& 父节点主键,
        const int 深度,
        const int 同层序号,
        const std::string& 父路径,
        std::unordered_set<const 基础信息节点类*>& 已访问,
        std::vector<结构_世界树SQL节点行>& 节点集,
        std::vector<结构_世界树SQL关系行>& 关系集)
    {
        if (!节点 || !已访问.insert(节点).second) {
            return;
        }
        const auto 节点主键 = 节点->获取主键();
        const auto 路径 = 父路径.empty() ? 节点主键 : 父路径 + "/" + 节点主键;
        结构_世界树SQL节点行 行{};
        行.行号 = static_cast<int>(节点集.size() + 1);
        行.节点指针 = 节点;
        行.节点主键 = 节点主键;
        行.父节点主键 = 父节点主键;
        行.深度 = 深度;
        行.同层序号 = 同层序号;
        行.直接子数量 = static_cast<int>(节点->子节点数量);
        行.路径 = 路径;
        私有_世界树SQL填充节点主信息(节点, 行);
        const int 当前行号 = 行.行号;
        节点集.push_back(std::move(行));
        私有_世界树SQL收集关系(节点, 当前行号, 关系集);

        if (节点->子) {
            私有_收集世界树SQL同层(
                static_cast<const 基础信息节点类*>(节点->子),
                nullptr,
                节点主键,
                深度 + 1,
                路径,
                已访问,
                节点集,
                关系集);
        }
    }

    void 私有_解析世界树SQL关系序号(
        const std::vector<结构_世界树SQL节点行>& 节点集,
        std::vector<结构_世界树SQL关系行>& 关系集)
    {
        std::unordered_map<const 基础信息节点类*, int> 节点序号{};
        节点序号.reserve(节点集.size());
        for (const auto& 行 : 节点集) {
            if (行.节点指针) {
                节点序号.emplace(行.节点指针, 行.行号);
            }
        }
        for (auto& 行 : 关系集) {
            if (行.基础目标指针) {
                if (const auto it = 节点序号.find(行.基础目标指针); it != 节点序号.end()) {
                    行.目标行号 = it->second;
                }
            }
        }
    }

    // 功能：追加世界树主信息大类分表建表脚本。
    void 私有_世界树SQL追加主信息建表脚本(
        std::ostringstream& SQL,
        const 枚举_世界树SQL主信息表 表)
    {
        const auto* 表名 = 私有_世界树SQL主信息表名(表);
        SQL << "IF OBJECT_ID(N'" << 表名 << "', N'U') IS NULL\n"
            << "CREATE TABLE " << 表名 << " (\n"
            << 私有_世界树SQL主信息通用列定义()
            << 私有_世界树SQL主信息附加列定义(表)
            << "\n);\n";
    }

    std::string 私有_世界树SQL建库脚本()
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "IF DB_ID(N'鱼巢投影库') IS NULL CREATE DATABASE [鱼巢投影库];\n";
        return SQL.str();
    }

    std::string 私有_世界树SQL建表脚本()
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "IF SCHEMA_ID(N'鱼巢') IS NULL EXEC(N'CREATE SCHEMA [鱼巢]');\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前世界树根树关系]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前世界树根树关系];\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前世界树根树节点]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前世界树根树节点];\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前世界树根树节点主信息]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前世界树根树节点主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前世界树根树]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前世界树根树];\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前世界树节点详情]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前世界树节点详情];\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前世界树主信息]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前世界树主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前世界树关系]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前世界树关系];\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前世界树节点]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前世界树节点];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树根树关系]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树根树关系];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树根树节点主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树根树节点主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树根树节点]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树根树节点];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树根树]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树根树];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树其他主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树其他主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树语言记录主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树语言记录主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树因果主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树因果主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树二次特征主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树二次特征主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树动态主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树动态主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树状态主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树状态主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树场景主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树场景主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树存在主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树存在主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树特征主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树特征主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树抽象特征主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树抽象特征主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树指代主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树指代主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树节点关系]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树节点关系];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树节点]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树节点];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树快照]', N'U') IS NOT NULL DROP TABLE [鱼巢].[世界树快照];\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树快照]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[世界树快照] (\n"
            << "    [快照标识] uniqueidentifier NOT NULL PRIMARY KEY,\n"
            << "    [捕获时间] datetime2(3) NOT NULL,\n"
            << "    [来源类型] nvarchar(80) NOT NULL,\n"
            << "    [来源原因] nvarchar(300) NULL,\n"
            << "    [节点数量] int NOT NULL,\n"
            << "    [关系数量] int NOT NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[世界树节点]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[世界树节点] (\n"
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
            << "IF OBJECT_ID(N'[鱼巢].[世界树节点关系]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[世界树节点关系] (\n"
            << "    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,\n"
            << "    [快照标识] uniqueidentifier NOT NULL,\n"
            << "    [行号] int NOT NULL,\n"
            << "    [宿主行号] int NOT NULL,\n"
            << "    [目标行号] int NULL,\n"
            << "    [宿主主键] nvarchar(80) NOT NULL,\n"
            << "    [关系名] nvarchar(120) NOT NULL,\n"
            << "    [目标类别] nvarchar(80) NULL,\n"
            << "    [目标主键] nvarchar(80) NULL,\n"
            << "    [序号] int NOT NULL\n"
            << ");\n"
            << "IF COL_LENGTH(N'[鱼巢].[世界树节点关系]', N'宿主行号') IS NULL\n"
            << "    ALTER TABLE [鱼巢].[世界树节点关系] ADD [宿主行号] int NOT NULL CONSTRAINT DF_world_tree_node_relation_owner_row_index DEFAULT(0);\n"
            << "IF COL_LENGTH(N'[鱼巢].[世界树节点关系]', N'目标行号') IS NULL\n"
            << "    ALTER TABLE [鱼巢].[世界树节点关系] ADD [目标行号] int NULL;\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_world_tree_node_key' AND object_id = OBJECT_ID(N'[鱼巢].[世界树节点]'))\n"
            << "    CREATE INDEX IX_world_tree_node_key ON [鱼巢].[世界树节点]([快照标识], [节点主键], [父节点主键]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_world_tree_node_parent' AND object_id = OBJECT_ID(N'[鱼巢].[世界树节点]'))\n"
            << "    CREATE INDEX IX_world_tree_node_parent ON [鱼巢].[世界树节点]([快照标识], [父节点主键], [节点主键]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_world_tree_node_path' AND object_id = OBJECT_ID(N'[鱼巢].[世界树节点]'))\n"
            << "    CREATE INDEX IX_world_tree_node_path ON [鱼巢].[世界树节点]([快照标识], [路径文本]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_world_tree_relation_owner' AND object_id = OBJECT_ID(N'[鱼巢].[世界树节点关系]'))\n"
            << "    CREATE INDEX IX_world_tree_relation_owner ON [鱼巢].[世界树节点关系]([快照标识], [宿主主键], [目标主键]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_world_tree_relation_owner_row' AND object_id = OBJECT_ID(N'[鱼巢].[世界树节点关系]'))\n"
            << "    CREATE INDEX IX_world_tree_relation_owner_row ON [鱼巢].[世界树节点关系]([快照标识], [宿主行号], [目标行号]);\n";
        for (const auto 表 : {
            枚举_世界树SQL主信息表::指代,
            枚举_世界树SQL主信息表::抽象特征,
            枚举_世界树SQL主信息表::特征,
            枚举_世界树SQL主信息表::存在,
            枚举_世界树SQL主信息表::场景,
            枚举_世界树SQL主信息表::状态,
            枚举_世界树SQL主信息表::动态,
            枚举_世界树SQL主信息表::二次特征,
            枚举_世界树SQL主信息表::因果,
            枚举_世界树SQL主信息表::语言记录,
            枚举_世界树SQL主信息表::其他 }) {
            私有_世界树SQL追加主信息建表脚本(SQL, 表);
        }
        return SQL.str();
    }

    std::string 私有_世界树SQL视图脚本()
    {
        std::ostringstream SQL;
        SQL << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前世界树主信息] AS\n"
            << "SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树指代主信息]\n"
            << "UNION ALL SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树抽象特征主信息]\n"
            << "UNION ALL SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树特征主信息]\n"
            << "UNION ALL SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树存在主信息]\n"
            << "UNION ALL SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树场景主信息]\n"
            << "UNION ALL SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树状态主信息]\n"
            << "UNION ALL SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树动态主信息]\n"
            << "UNION ALL SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树二次特征主信息]\n"
            << "UNION ALL SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树因果主信息]\n"
            << "UNION ALL SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树语言记录主信息]\n"
            << "UNION ALL SELECT [快照标识], [节点行号], [节点主键], [主信息类型值], [名称主键], [类型主键], [名称文本], [类型文本] FROM [鱼巢].[世界树其他主信息];');\n"
            << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前世界树节点] AS\n"
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
            << "    CASE m.[主信息类型值]\n"
            << "        WHEN 0 THEN N''指代'' WHEN 10 THEN N''特征'' WHEN 11 THEN N''抽象特征''\n"
            << "        WHEN 14 THEN N''存在'' WHEN 15 THEN N''状态'' WHEN 16 THEN N''场景''\n"
            << "        WHEN 22 THEN N''动态'' WHEN 23 THEN N''因果'' WHEN 30 THEN N''二次特征''\n"
            << "        WHEN 35 THEN N''语言信息_仅记录'' WHEN 4 THEN N''基础信息基类'' ELSE N'''' END AS [节点类型],\n"
            << "    COALESCE(NULLIF(m.[名称文本], N''''), NULLIF(m.[类型文本], N''''), NULLIF(m.[名称主键], N''''), NULLIF(m.[类型主键], N''''), n.[节点主键]) AS [显示文本],\n"
            << "    m.[主信息类型值],\n"
            << "    CASE m.[主信息类型值]\n"
            << "        WHEN 0 THEN N''指代'' WHEN 10 THEN N''特征'' WHEN 11 THEN N''抽象特征''\n"
            << "        WHEN 14 THEN N''存在'' WHEN 15 THEN N''状态'' WHEN 16 THEN N''场景''\n"
            << "        WHEN 22 THEN N''动态'' WHEN 23 THEN N''因果'' WHEN 30 THEN N''二次特征''\n"
            << "        WHEN 35 THEN N''语言信息_仅记录'' WHEN 4 THEN N''基础信息基类'' ELSE N'''' END AS [主信息类型文本],\n"
            << "    m.[名称主键],\n"
            << "    m.[名称文本],\n"
            << "    m.[类型主键],\n"
            << "    m.[类型文本],\n"
            << "    CASE COALESCE(f.[当前值类型值], s.[状态值类型值], 0)\n"
            << "        WHEN 1 THEN N''I64'' WHEN 2 THEN N''VecU句柄'' WHEN 3 THEN N''指针句柄'' ELSE N'''' END AS [值类别],\n"
            << "    COALESCE(CONVERT(nvarchar(80), f.[当前值I64]), CONVERT(nvarchar(80), f.[当前值VecU指针]), CONVERT(nvarchar(80), f.[当前值指针]),\n"
            << "        CONVERT(nvarchar(80), s.[状态值I64]), CONVERT(nvarchar(80), s.[状态值VecU指针]), CONVERT(nvarchar(80), s.[状态值指针]), N'''') AS [值文本],\n"
            << "    CAST(NULL AS nvarchar(1000)) AS [辅助文本]\n"
            << "FROM [鱼巢].[世界树节点] n\n"
            << "LEFT JOIN [鱼巢].[当前世界树主信息] m\n"
            << "    ON m.[快照标识] = n.[快照标识] AND m.[节点行号] = n.[行号]\n"
            << "LEFT JOIN [鱼巢].[世界树特征主信息] f\n"
            << "    ON f.[快照标识] = n.[快照标识] AND f.[节点行号] = n.[行号]\n"
            << "LEFT JOIN [鱼巢].[世界树状态主信息] s\n"
            << "    ON s.[快照标识] = n.[快照标识] AND s.[节点行号] = n.[行号]\n"
            << "WHERE n.[快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[世界树快照] ORDER BY [捕获时间] DESC);');\n"
            << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前世界树关系] AS\n"
            << "SELECT\n"
            << "    r.[记录标识], r.[快照标识], r.[行号], r.[宿主行号], r.[目标行号],\n"
            << "    r.[宿主主键], r.[关系名], r.[目标类别], r.[目标主键],\n"
            << "    COALESCE(NULLIF(t.[显示文本], N''''), NULLIF(r.[目标主键], N''''), r.[目标类别]) AS [目标文本],\n"
            << "    r.[序号]\n"
            << "FROM [鱼巢].[世界树节点关系] r\n"
            << "LEFT JOIN [鱼巢].[当前世界树节点] t\n"
            << "    ON t.[快照标识] = r.[快照标识]\n"
            << "    AND ((r.[目标行号] > 0 AND t.[行号] = r.[目标行号])\n"
            << "        OR ((r.[目标行号] IS NULL OR r.[目标行号] <= 0) AND t.[节点主键] = r.[目标主键]))\n"
            << "WHERE r.[快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[世界树快照] ORDER BY [捕获时间] DESC);');\n";
        SQL << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前世界树节点详情] AS\n"
            << "SELECT n.[节点主键], v.[排序], v.[字段名], v.[字段类型], v.[字段值],\n"
            << "    CAST(NULL AS nvarchar(80)) AS [指针主键],\n"
            << "    CAST(NULL AS nvarchar(120)) AS [指针类型],\n"
            << "    CAST(NULL AS nvarchar(300)) AS [指针名称]\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''主键'', N''文本'', n.[节点主键]),\n"
            << "    (20, N''父节点主键'', N''文本'', COALESCE(n.[父节点主键], N'''')),\n"
            << "    (30, N''深度'', N''i64'', CONVERT(nvarchar(80), n.[深度])),\n"
            << "    (40, N''同层序号'', N''i64'', CONVERT(nvarchar(80), n.[同层序号])),\n"
            << "    (50, N''直接子数量'', N''i64'', CONVERT(nvarchar(80), n.[直接子数量])),\n"
            << "    (60, N''路径文本'', N''文本'', COALESCE(n.[路径文本], N'''')),\n"
            << "    (70, N''主信息类型'', N''文本'', COALESCE(n.[主信息类型文本], N'''')),\n"
            << "    (80, N''名称'', N''文本'', COALESCE(n.[名称文本], N'''')),\n"
            << "    (90, N''类型'', N''文本'', COALESCE(n.[类型文本], N'''')),\n"
            << "    (100, N''值类别'', N''文本'', COALESCE(n.[值类别], N'''')),\n"
            << "    (110, N''值文本'', N''文本'', COALESCE(n.[值文本], N''''))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT n.[节点主键], 1000 + v.[排序], v.[字段名], v.[字段类型], v.[字段值], NULL, NULL, NULL\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "JOIN [鱼巢].[世界树指代主信息] p ON p.[快照标识] = n.[快照标识] AND p.[节点行号] = n.[行号]\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''代词主键'', N''文本'', COALESCE(p.[代词主键], N'''')),\n"
            << "    (20, N''有指代对象'', N''bool'', CONVERT(nvarchar(80), p.[有指代对象]))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT n.[节点主键], 1100 + v.[排序], v.[字段名], v.[字段类型], v.[字段值], NULL, NULL, NULL\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "JOIN [鱼巢].[世界树抽象特征主信息] p ON p.[快照标识] = n.[快照标识] AND p.[节点行号] = n.[行号]\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''形态值'', N''i64'', CONVERT(nvarchar(80), p.[形态值])),\n"
            << "    (20, N''有值域'', N''bool'', CONVERT(nvarchar(80), p.[有值域])),\n"
            << "    (30, N''有区间'', N''bool'', CONVERT(nvarchar(80), p.[有区间])),\n"
            << "    (40, N''来源实例特征数量'', N''i64'', CONVERT(nvarchar(80), p.[来源实例特征数量]))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT n.[节点主键], 1200 + v.[排序], v.[字段名], v.[字段类型], v.[字段值], NULL, NULL, NULL\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "JOIN [鱼巢].[世界树特征主信息] p ON p.[快照标识] = n.[快照标识] AND p.[节点行号] = n.[行号]\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''当前值类型值'', N''i64'', CONVERT(nvarchar(80), p.[当前值类型值])),\n"
            << "    (20, N''当前值I64'', N''i64'', COALESCE(CONVERT(nvarchar(80), p.[当前值I64]), N'''')),\n"
            << "    (30, N''当前值VecU指针'', N''VecU句柄'', COALESCE(CONVERT(nvarchar(80), p.[当前值VecU指针]), N'''')),\n"
            << "    (40, N''当前值指针'', N''指针句柄'', COALESCE(CONVERT(nvarchar(80), p.[当前值指针]), N'''')),\n"
            << "    (50, N''当前稳态索引'', N''i64'', CONVERT(nvarchar(80), p.[当前稳态索引])),\n"
            << "    (60, N''稳态数量'', N''i64'', CONVERT(nvarchar(80), p.[稳态数量]))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT n.[节点主键], 1300 + v.[排序], v.[字段名], v.[字段类型], v.[字段值], NULL, NULL, NULL\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "JOIN [鱼巢].[世界树存在主信息] p ON p.[快照标识] = n.[快照标识] AND p.[节点行号] = n.[行号]\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''概念数量'', N''i64'', CONVERT(nvarchar(80), p.[概念数量])),\n"
            << "    (20, N''有概念模板'', N''bool'', CONVERT(nvarchar(80), p.[有概念模板])),\n"
            << "    (30, N''有内部世界'', N''bool'', CONVERT(nvarchar(80), p.[有内部世界])),\n"
            << "    (40, N''有需求根'', N''bool'', CONVERT(nvarchar(80), p.[有需求根])),\n"
            << "    (50, N''有任务根'', N''bool'', CONVERT(nvarchar(80), p.[有任务根])),\n"
            << "    (60, N''有方法根'', N''bool'', CONVERT(nvarchar(80), p.[有方法根])),\n"
            << "    (70, N''有位置历史'', N''bool'', CONVERT(nvarchar(80), p.[有位置历史])),\n"
            << "    (80, N''最近位移mm'', N''i64'', CONVERT(nvarchar(80), p.[最近位移mm])),\n"
            << "    (90, N''连续静止帧'', N''i64'', CONVERT(nvarchar(80), p.[连续静止帧])),\n"
            << "    (100, N''连续未命中帧'', N''i64'', CONVERT(nvarchar(80), p.[连续未命中帧]))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT n.[节点主键], 1400 + v.[排序], v.[字段名], v.[字段类型], v.[字段值], NULL, NULL, NULL\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "JOIN [鱼巢].[世界树场景主信息] p ON p.[快照标识] = n.[快照标识] AND p.[节点行号] = n.[行号]\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''世界类型值'', N''i64'', CONVERT(nvarchar(80), p.[世界类型值])),\n"
            << "    (20, N''状态索引数量'', N''i64'', CONVERT(nvarchar(80), p.[状态索引数量])),\n"
            << "    (30, N''动态索引数量'', N''i64'', CONVERT(nvarchar(80), p.[动态索引数量])),\n"
            << "    (40, N''二次特征索引数量'', N''i64'', CONVERT(nvarchar(80), p.[二次特征索引数量]))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT n.[节点主键], 1500 + v.[排序], v.[字段名], v.[字段类型], v.[字段值], NULL, NULL, NULL\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "JOIN [鱼巢].[世界树状态主信息] p ON p.[快照标识] = n.[快照标识] AND p.[节点行号] = n.[行号]\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''状态值类型值'', N''i64'', CONVERT(nvarchar(80), p.[状态值类型值])),\n"
            << "    (20, N''状态值I64'', N''i64'', COALESCE(CONVERT(nvarchar(80), p.[状态值I64]), N'''')),\n"
            << "    (30, N''状态值VecU指针'', N''VecU句柄'', COALESCE(CONVERT(nvarchar(80), p.[状态值VecU指针]), N'''')),\n"
            << "    (40, N''状态值指针'', N''指针句柄'', COALESCE(CONVERT(nvarchar(80), p.[状态值指针]), N'''')),\n"
            << "    (50, N''收到时间'', N''i64'', CONVERT(nvarchar(80), p.[收到时间])),\n"
            << "    (60, N''发生时间'', N''i64'', CONVERT(nvarchar(80), p.[发生时间])),\n"
            << "    (70, N''是否变化'', N''bool'', CONVERT(nvarchar(80), p.[是否变化]))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT n.[节点主键], 1600 + v.[排序], v.[字段名], v.[字段类型], v.[字段值], NULL, NULL, NULL\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "JOIN [鱼巢].[世界树动态主信息] p ON p.[快照标识] = n.[快照标识] AND p.[节点行号] = n.[行号]\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''时间起'', N''i64'', CONVERT(nvarchar(80), p.[时间起])),\n"
            << "    (20, N''时间止'', N''i64'', CONVERT(nvarchar(80), p.[时间止])),\n"
            << "    (30, N''来源执行成功'', N''bool'', CONVERT(nvarchar(80), p.[来源执行成功])),\n"
            << "    (40, N''来源错误码'', N''i64'', CONVERT(nvarchar(80), p.[来源错误码])),\n"
            << "    (50, N''动态路径签名'', N''u64'', CONVERT(nvarchar(80), p.[动态路径签名])),\n"
            << "    (60, N''动态层级'', N''i64'', CONVERT(nvarchar(80), p.[动态层级])),\n"
            << "    (70, N''聚合方式值'', N''i64'', CONVERT(nvarchar(80), p.[聚合方式值]))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT n.[节点主键], 1700 + v.[排序], v.[字段名], v.[字段类型], v.[字段值], NULL, NULL, NULL\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "JOIN [鱼巢].[世界树二次特征主信息] p ON p.[快照标识] = n.[快照标识] AND p.[节点行号] = n.[行号]\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''形态值'', N''i64'', CONVERT(nvarchar(80), p.[形态值])),\n"
            << "    (20, N''种类值'', N''i64'', CONVERT(nvarchar(80), p.[种类值])),\n"
            << "    (30, N''域值'', N''i64'', CONVERT(nvarchar(80), p.[域值])),\n"
            << "    (40, N''粒度值'', N''i64'', CONVERT(nvarchar(80), p.[粒度值])),\n"
            << "    (50, N''标量值'', N''i64'', CONVERT(nvarchar(80), p.[标量值])),\n"
            << "    (60, N''离散编码'', N''i64'', CONVERT(nvarchar(80), p.[离散编码]))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT n.[节点主键], 1800 + v.[排序], v.[字段名], v.[字段类型], v.[字段值], NULL, NULL, NULL\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "JOIN [鱼巢].[世界树因果主信息] p ON p.[快照标识] = n.[快照标识] AND p.[节点行号] = n.[行号]\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''适用锚点类型值'', N''i64'', CONVERT(nvarchar(80), p.[适用锚点类型值])),\n"
            << "    (20, N''适用层级'', N''i64'', CONVERT(nvarchar(80), p.[适用层级])),\n"
            << "    (30, N''观察次数'', N''u64'', CONVERT(nvarchar(80), p.[观察次数])),\n"
            << "    (40, N''条件命中次数'', N''u64'', CONVERT(nvarchar(80), p.[条件命中次数])),\n"
            << "    (50, N''因出现次数'', N''u64'', CONVERT(nvarchar(80), p.[因出现次数])),\n"
            << "    (60, N''果出现次数'', N''u64'', CONVERT(nvarchar(80), p.[果出现次数])),\n"
            << "    (70, N''成立次数'', N''u64'', CONVERT(nvarchar(80), p.[成立次数])),\n"
            << "    (80, N''失败次数'', N''u64'', CONVERT(nvarchar(80), p.[失败次数])),\n"
            << "    (90, N''最近命中时间'', N''i64'', CONVERT(nvarchar(80), p.[最近命中时间])),\n"
            << "    (100, N''最近失败时间'', N''i64'', CONVERT(nvarchar(80), p.[最近失败时间])),\n"
            << "    (110, N''已验证'', N''bool'', CONVERT(nvarchar(80), p.[已验证]))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT n.[节点主键], 1900 + v.[排序], v.[字段名], v.[字段类型], v.[字段值], NULL, NULL, NULL\n"
            << "FROM [鱼巢].[当前世界树节点] n\n"
            << "JOIN [鱼巢].[世界树语言记录主信息] p ON p.[快照标识] = n.[快照标识] AND p.[节点行号] = n.[行号]\n"
            << "CROSS APPLY (VALUES\n"
            << "    (10, N''语言词性主键'', N''文本'', COALESCE(p.[语言词性主键], N''''))\n"
            << ") v([排序], [字段名], [字段类型], [字段值])\n"
            << "UNION ALL\n"
            << "SELECT r.[宿主主键] AS [节点主键],\n"
            << "    3000 + ROW_NUMBER() OVER(PARTITION BY r.[宿主主键] ORDER BY r.[行号]) AS [排序],\n"
            << "    r.[关系名] + CASE WHEN r.[序号] > 0 THEN N''['' + CONVERT(nvarchar(20), r.[序号]) + N'']'' ELSE N'''' END AS [字段名],\n"
            << "    COALESCE(NULLIF(r.[目标类别], N''''), N''自定义'') + N''指针'' AS [字段类型],\n"
            << "    CONCAT(N''主键='', COALESCE(NULLIF(r.[目标主键], N''''), N''空''),\n"
            << "        N'' | 类型='', COALESCE(NULLIF(t.[主信息类型文本], N''''), NULLIF(r.[目标类别], N''''), N''空''),\n"
            << "        N'' | 名称='', COALESCE(NULLIF(t.[名称文本], N''''), NULLIF(t.[显示文本], N''''), N''空'')) AS [字段值],\n"
            << "    r.[目标主键] AS [指针主键],\n"
            << "    COALESCE(NULLIF(t.[主信息类型文本], N''''), NULLIF(r.[目标类别], N''''), N'''') AS [指针类型],\n"
            << "    COALESCE(NULLIF(t.[名称文本], N''''), NULLIF(t.[显示文本], N''''), N'''') AS [指针名称]\n"
            << "FROM [鱼巢].[当前世界树关系] r\n"
            << "LEFT JOIN [鱼巢].[当前世界树节点] t\n"
            << "    ON t.[快照标识] = r.[快照标识]\n"
            << "    AND ((r.[目标行号] > 0 AND t.[行号] = r.[目标行号])\n"
            << "        OR ((r.[目标行号] IS NULL OR r.[目标行号] <= 0) AND t.[节点主键] = r.[目标主键]));');\n";
        return SQL.str();
    }

    std::string 私有_构造世界树SQL重写脚本(
        const std::vector<结构_世界树SQL节点行>& 节点集,
        const std::vector<结构_世界树SQL关系行>& 关系集,
        const std::string& 来源原因)
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "SET XACT_ABORT ON;\n"
            << "BEGIN TRANSACTION;\n"
            << "DELETE FROM [鱼巢].[世界树其他主信息];\n"
            << "DELETE FROM [鱼巢].[世界树语言记录主信息];\n"
            << "DELETE FROM [鱼巢].[世界树因果主信息];\n"
            << "DELETE FROM [鱼巢].[世界树二次特征主信息];\n"
            << "DELETE FROM [鱼巢].[世界树动态主信息];\n"
            << "DELETE FROM [鱼巢].[世界树状态主信息];\n"
            << "DELETE FROM [鱼巢].[世界树场景主信息];\n"
            << "DELETE FROM [鱼巢].[世界树存在主信息];\n"
            << "DELETE FROM [鱼巢].[世界树特征主信息];\n"
            << "DELETE FROM [鱼巢].[世界树抽象特征主信息];\n"
            << "DELETE FROM [鱼巢].[世界树指代主信息];\n"
            << "DELETE FROM [鱼巢].[世界树节点关系];\n"
            << "DELETE FROM [鱼巢].[世界树节点];\n"
            << "DELETE FROM [鱼巢].[世界树快照];\n"
            << "DECLARE @快照标识 uniqueidentifier = NEWID();\n"
            << "INSERT INTO [鱼巢].[世界树快照] ([快照标识], [捕获时间], [来源类型], [来源原因], [节点数量], [关系数量])\n"
            << "VALUES (@快照标识, SYSUTCDATETIME(), N'world_tree_projection', "
            << 私有_世界树SQL字符串(来源原因)
            << ", " << 节点集.size()
            << ", " << 关系集.size()
            << ");\n";
        constexpr std::size_t 插入批量大小 = 500;
        for (std::size_t 起始 = 0; 起始 < 节点集.size(); 起始 += 插入批量大小) {
            const auto 结束 = std::min(起始 + 插入批量大小, 节点集.size());
            SQL << "INSERT INTO [鱼巢].[世界树节点] ([快照标识], [行号], [节点主键], [父节点主键], [深度], [同层序号], [直接子数量], [路径文本]) VALUES\n";
            for (std::size_t 索引 = 起始; 索引 < 结束; ++索引) {
                const auto& 行 = 节点集[索引];
                SQL << "(@快照标识, "
                    << 行.行号 << ", "
                    << 私有_世界树SQL字符串(行.节点主键, false) << ", "
                    << 私有_世界树SQL字符串(行.父节点主键) << ", "
                    << 行.深度 << ", "
                    << 行.同层序号 << ", "
                    << 行.直接子数量 << ", "
                    << 私有_世界树SQL字符串(行.路径) << ")"
                    << (索引 + 1 == 结束 ? ";\n" : ",\n");
            }
        }
        for (const auto 表 : {
            枚举_世界树SQL主信息表::指代,
            枚举_世界树SQL主信息表::抽象特征,
            枚举_世界树SQL主信息表::特征,
            枚举_世界树SQL主信息表::存在,
            枚举_世界树SQL主信息表::场景,
            枚举_世界树SQL主信息表::状态,
            枚举_世界树SQL主信息表::动态,
            枚举_世界树SQL主信息表::二次特征,
            枚举_世界树SQL主信息表::因果,
            枚举_世界树SQL主信息表::语言记录,
            枚举_世界树SQL主信息表::其他 }) {
            std::vector<const 结构_世界树SQL节点行*> 分表行集{};
            for (const auto& 行 : 节点集) {
                const auto* 主信息 = 行.节点指针 ? 行.节点指针->主信息 : nullptr;
                if (!主信息) {
                    continue;
                }
                if (私有_世界树SQL主信息分表(主信息) == 表) {
                    分表行集.push_back(&行);
                }
            }
            for (std::size_t 起始 = 0; 起始 < 分表行集.size(); 起始 += 插入批量大小) {
                const auto 结束 = std::min(起始 + 插入批量大小, 分表行集.size());
                SQL << "INSERT INTO " << 私有_世界树SQL主信息表名(表)
                    << " (" << 私有_世界树SQL主信息通用列名()
                    << 私有_世界树SQL主信息附加列名(表) << ") VALUES\n";
                for (std::size_t 索引 = 起始; 索引 < 结束; ++索引) {
                    SQL << "(";
                    私有_世界树SQL写主信息通用值(SQL, *分表行集[索引]);
                    私有_世界树SQL写主信息附加值(SQL, *分表行集[索引], 表);
                    SQL << ")"
                        << (索引 + 1 == 结束 ? ";\n" : ",\n");
                }
            }
        }
        for (std::size_t 起始 = 0; 起始 < 关系集.size(); 起始 += 插入批量大小) {
            const auto 结束 = std::min(起始 + 插入批量大小, 关系集.size());
            SQL << "INSERT INTO [鱼巢].[世界树节点关系] ([快照标识], [行号], [宿主行号], [目标行号], [宿主主键], [关系名], [目标类别], [目标主键], [序号]) VALUES\n";
            for (std::size_t 索引 = 起始; 索引 < 结束; ++索引) {
                const auto& 行 = 关系集[索引];
                SQL << "(@快照标识, "
                    << 行.行号 << ", "
                    << 行.宿主行号 << ", "
                    << SQL可空整数文本(行.目标行号 > 0, 行.目标行号) << ", "
                    << 私有_世界树SQL字符串(行.宿主主键, false) << ", "
                    << 私有_世界树SQL字符串(行.关系名, false) << ", "
                    << 私有_世界树SQL字符串(行.目标类别) << ", "
                    << 私有_世界树SQL字符串(行.目标主键) << ", "
                    << 行.序号 << ")"
                    << (索引 + 1 == 结束 ? ";\n" : ",\n");
            }
        }
        SQL << "COMMIT TRANSACTION;\n";
        return SQL.str();
    }

    bool 私有_执行世界树ADO命令(
        const std::string& 连接串,
        const std::string& 阶段,
        const std::string& SQL,
        std::string& 错误,
        const int 命令超时秒 = 30)
    {
        std::string ADO错误{};
        if (!执行ADO命令(连接串, SQL, ADO错误, 命令超时秒)) {
            错误 = 阶段 + "失败 | " + ADO错误;
            return false;
        }
        return true;
    }

    bool 私有_世界树SQL路径属于根(const std::string& 路径, const std::string& 根路径)
    {
        return 路径 == 根路径
            || (!根路径.empty()
                && 路径.size() > 根路径.size()
                && 路径.compare(0, 根路径.size(), 根路径) == 0
                && 路径[根路径.size()] == '/');
    }

    std::vector<const 结构_世界树SQL节点行*> 私有_世界树SQL根节点集(
        const std::vector<结构_世界树SQL节点行>& 节点集)
    {
        std::vector<const 结构_世界树SQL节点行*> 输出;
        for (const auto& 行 : 节点集) {
            if (行.父节点主键 == "WORLD_ROOT") {
                输出.push_back(&行);
            }
        }
        return 输出;
    }

    std::vector<std::vector<std::string>> 私有_世界树SQL快照预期字段(
        const std::string& 来源原因,
        const std::size_t 节点数,
        const std::size_t 关系数)
    {
        return {
            {
                "world_tree_projection",
                来源原因,
                std::to_string(节点数),
                std::to_string(关系数),
            },
        };
    }

    std::vector<std::string> 私有_世界树SQL节点字段(
        const 结构_世界树SQL节点行& 行)
    {
        return {
            SQL整数文本(行.行号),
            行.节点主键,
            行.父节点主键,
            SQL整数文本(行.深度),
            SQL整数文本(行.同层序号),
            SQL整数文本(行.直接子数量),
            行.路径,
        };
    }

    std::vector<std::vector<std::string>> 私有_世界树SQL节点预期字段(
        const std::vector<结构_世界树SQL节点行>& 节点集)
    {
        std::vector<std::vector<std::string>> 输出;
        输出.reserve(节点集.size());
        for (const auto& 行 : 节点集) {
            输出.push_back(私有_世界树SQL节点字段(行));
        }
        return 输出;
    }

    std::vector<std::string> 私有_世界树SQL关系字段(
        const 结构_世界树SQL关系行& 行)
    {
        return {
            SQL整数文本(行.行号),
            SQL整数文本(行.宿主行号),
            SQL字段可空整数文本(行.目标行号 > 0, 行.目标行号),
            行.宿主主键,
            行.关系名,
            行.目标类别,
            行.目标主键,
            SQL整数文本(行.序号),
        };
    }

    std::vector<std::vector<std::string>> 私有_世界树SQL关系预期字段(
        const std::vector<结构_世界树SQL关系行>& 关系集)
    {
        std::vector<std::vector<std::string>> 输出;
        输出.reserve(关系集.size());
        for (const auto& 行 : 关系集) {
            输出.push_back(私有_世界树SQL关系字段(行));
        }
        return 输出;
    }

    std::vector<std::vector<std::string>> 私有_世界树SQL主信息预期字段(
        const std::vector<结构_世界树SQL节点行>& 节点集)
    {
        std::vector<std::vector<std::string>> 输出;
        for (const auto& 行 : 节点集) {
            if (!行.有主信息类型) {
                continue;
            }
            输出.push_back({
                SQL整数文本(行.行号),
                行.节点主键,
                SQL字段可空整数文本(行.有主信息类型, 行.主信息类型值),
                行.名称主键,
                行.类型主键,
                行.名称文本,
                行.类型文本,
            });
        }
        return 输出;
    }

    bool 私有_执行世界树SQL字段恢复比对(
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

    bool 私有_验证世界树SQL存储字段(
        const std::string& 连接串,
        const std::vector<结构_世界树SQL节点行>& 节点集,
        const std::vector<结构_世界树SQL关系行>& 关系集,
        const std::string& 来源原因,
        std::string& 错误)
    {
        constexpr const char* 快照SQL = R"SQL(
SELECT
    COALESCE([来源类型], N''),
    COALESCE([来源原因], N''),
    COALESCE(CONVERT(nvarchar(30), [节点数量]), N''),
    COALESCE(CONVERT(nvarchar(30), [关系数量]), N'')
FROM [鱼巢].[世界树快照]
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
FROM [鱼巢].[世界树节点]
WHERE [快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[世界树快照] ORDER BY [捕获时间] DESC)
ORDER BY [行号];
)SQL";
        constexpr const char* 主信息SQL = R"SQL(
SELECT
    COALESCE(CONVERT(nvarchar(30), [节点行号]), N''),
    COALESCE([节点主键], N''),
    COALESCE(CONVERT(nvarchar(30), [主信息类型值]), N''),
    COALESCE([名称主键], N''),
    COALESCE([类型主键], N''),
    COALESCE([名称文本], N''),
    COALESCE([类型文本], N'')
FROM [鱼巢].[当前世界树主信息]
ORDER BY [节点行号];
)SQL";
        constexpr const char* 关系SQL = R"SQL(
SELECT
    COALESCE(CONVERT(nvarchar(30), [行号]), N''),
    COALESCE(CONVERT(nvarchar(30), [宿主行号]), N''),
    COALESCE(CONVERT(nvarchar(30), [目标行号]), N''),
    COALESCE([宿主主键], N''),
    COALESCE([关系名], N''),
    COALESCE([目标类别], N''),
    COALESCE([目标主键], N''),
    COALESCE(CONVERT(nvarchar(30), [序号]), N'')
FROM [鱼巢].[世界树节点关系]
WHERE [快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[世界树快照] ORDER BY [捕获时间] DESC)
ORDER BY [行号];
)SQL";

        return 私有_执行世界树SQL字段恢复比对(
            连接串,
            "世界树SQL字段恢复比对/快照",
            快照SQL,
            私有_世界树SQL快照预期字段(来源原因, 节点集.size(), 关系集.size()),
            错误)
            && 私有_执行世界树SQL字段恢复比对(
                连接串,
                "世界树SQL字段恢复比对/节点",
                节点SQL,
                私有_世界树SQL节点预期字段(节点集),
                错误)
            && 私有_执行世界树SQL字段恢复比对(
                连接串,
                "世界树SQL字段恢复比对/主信息",
                主信息SQL,
                私有_世界树SQL主信息预期字段(节点集),
                错误)
            && 私有_执行世界树SQL字段恢复比对(
                连接串,
                "世界树SQL字段恢复比对/关系",
                关系SQL,
                私有_世界树SQL关系预期字段(关系集),
                错误);
    }
}

世界树类& 世界树 = *new 世界树类{};

// 功能：初始化对象实例。
世界树类::世界树类()
    : 特征服务_(&基础信息链_)
    , 存在服务_(&基础信息链_)
    , 场景服务_(&基础信息链_)
    , 二次特征服务_(&基础信息链_)
    , 二次特征生成服务_(std::make_unique<二次特征生成模块>(this))
    , 状态服务_(&基础信息链_)
    , 动态服务_(&基础信息链_)
    , 因果服务_(&基础信息链_)
{
    特征服务_.设置差异度函数([this](const 语素入口节点类*, const 特征值& 左值, const 特征值& 右值) -> I64 {
        return 默认特征差异度(左值, 右值);
    });
}

世界树类::~世界树类() = default;

// 功能：初始化相关对象、状态或运行上下文。
void 世界树类::初始化()
{
    基础信息链_.初始化();
    值池_.初始化();
    语素集.初始化();
    初始化度量模板注册表();
    (void)取或创建抽象特征根();
}

// 功能：初始化相关对象、状态或运行上下文。
void 世界树类::初始化默认世界()
{
    现实世界 = 取或创建默认世界("现实世界", 枚举_世界类型::现实世界);
    文本世界 = 取或创建默认世界("文本世界", 枚举_世界类型::文本世界);
    想象世界 = 取或创建默认世界("想象世界", 枚举_世界类型::想象世界);
    记忆世界 = 取或创建默认世界("记忆世界", 枚举_世界类型::记忆世界);
    推理世界 = 取或创建默认世界("推理世界", 枚举_世界类型::推理世界);
    虚拟世界 = 取或创建默认世界("虚拟世界", 枚举_世界类型::虚拟世界);
    (void)初始化存在概念系统();
}

// 功能：按函数名执行对应处理。
基础信息类& 世界树类::基础信息() noexcept { return 基础信息链_; }

// 功能：按函数名执行对应处理。
const 基础信息类& 世界树类::基础信息() const noexcept { return 基础信息链_; }

// 功能：按函数名执行对应处理。
特征值类& 世界树类::值池() noexcept { return 值池_; }

// 功能：按函数名执行对应处理。
const 特征值类& 世界树类::值池() const noexcept { return 值池_; }

// 功能：按 VecU 句柄读取值池中的只读向量指针。
const VecIU64* 世界树类::取VecU只读指针(VecU句柄 句柄) const noexcept
{
    return 值池_.取VecU只读指针(句柄);
}

// 功能：按函数名执行对应处理。
特征类& 世界树类::特征() noexcept { return 特征服务_; }

// 功能：按函数名执行对应处理。
const 特征类& 世界树类::特征() const noexcept { return 特征服务_; }

// 功能：按函数名执行对应处理。
存在类& 世界树类::存在() noexcept { return 存在服务_; }

// 功能：按函数名执行对应处理。
const 存在类& 世界树类::存在() const noexcept { return 存在服务_; }

// 功能：按函数名执行对应处理。
场景类& 世界树类::场景() noexcept { return 场景服务_; }

// 功能：按函数名执行对应处理。
const 场景类& 世界树类::场景() const noexcept { return 场景服务_; }

// 功能：按函数名执行对应处理。
二次特征类& 世界树类::二次特征() noexcept { return 二次特征服务_; }

// 功能：按函数名执行对应处理。
const 二次特征类& 世界树类::二次特征() const noexcept { return 二次特征服务_; }

// 功能：根据当前输入生成目标数据、场景、动态或回执。
二次特征生成模块& 世界树类::二次特征生成() noexcept { return *二次特征生成服务_; }

// 功能：根据当前输入生成目标数据、场景、动态或回执。
const 二次特征生成模块& 世界树类::二次特征生成() const noexcept { return *二次特征生成服务_; }

// 功能：按函数名执行对应处理。
状态类& 世界树类::状态() noexcept { return 状态服务_; }

// 功能：按函数名执行对应处理。
const 状态类& 世界树类::状态() const noexcept { return 状态服务_; }

// 功能：按函数名执行对应处理。
动态类& 世界树类::动态() noexcept { return 动态服务_; }

// 功能：按函数名执行对应处理。
const 动态类& 世界树类::动态() const noexcept { return 动态服务_; }

// 功能：按函数名执行对应处理。
因果类& 世界树类::因果() noexcept { return 因果服务_; }

// 功能：按函数名执行对应处理。
const 因果类& 世界树类::因果() const noexcept { return 因果服务_; }

// 功能：按函数名执行对应处理。
基础信息节点类* 世界树类::世界根() const noexcept
{
    return 基础信息链_.世界根();
}

// 功能：按主键解析基础信息世界树节点，不创建或修改节点。
基础信息节点类* 世界树类::按主键解析节点(const std::string& 主键) const noexcept
{
    return 主键.empty() ? nullptr : 基础信息链_.查找主键(主键);
}

// 功能：按主键解析存在节点，不创建或修改节点。
存在节点类* 世界树类::按主键解析存在节点(const std::string& 主键) const noexcept
{
    return 存在服务_.按主键解析存在节点(主键);
}

// 功能：按主键解析场景节点，不创建或修改节点。
场景节点类* 世界树类::按主键解析场景节点(const std::string& 主键) const noexcept
{
    return 场景服务_.按主键解析场景节点(主键);
}

// 功能：按主键解析特征节点，不创建或修改节点。
特征节点类* 世界树类::按主键解析特征节点(const std::string& 主键) const noexcept
{
    return 特征服务_.按主键解析特征节点(主键);
}

// 功能：按主键解析状态节点，不创建或修改节点。
状态节点类* 世界树类::按主键解析状态节点(const std::string& 主键) const noexcept
{
    return 状态服务_.按主键解析状态节点(主键);
}

// 功能：按主键解析动态节点，不创建或修改节点。
动态节点类* 世界树类::按主键解析动态节点(const std::string& 主键) const noexcept
{
    return 动态服务_.按主键解析动态节点(主键);
}

// 功能：按主键解析二次特征节点，不创建或修改节点。
二次特征节点类* 世界树类::按主键解析二次特征节点(const std::string& 主键) const noexcept
{
    return 二次特征服务_.按主键解析二次特征节点(主键);
}

// 功能：读取世界树节点的父节点，不创建或修改节点。
基础信息节点类* 世界树类::获取父节点(const 基础信息节点类* 节点) const noexcept
{
    return 节点 ? static_cast<基础信息节点类*>(节点->父) : nullptr;
}

// 功能：读取并返回指定对象、状态或运行材料。
std::vector<基础信息节点类*> 世界树类::获取子节点(const 基础信息节点类* 父节点) const
{
    return 私有_直接基础信息子节点(基础信息链_, 父节点);
}

// 功能：统计当前世界树基础信息节点总数，不返回节点集合。
std::size_t 世界树类::统计全部节点() const
{
    return 基础信息链_.枚举全部节点().size();
}

// 功能：判断基础信息节点指针是否属于当前世界树。
bool 世界树类::基础节点属于当前世界树(const 基础信息节点类* 节点) const
{
    if (!节点) return false;
    if (节点 == 世界根()) return true;
    for (const auto* 当前 : 基础信息链_.枚举全部节点()) {
        if (当前 == 节点) {
            return true;
        }
    }
    return false;
}

// 功能：读取并返回指定对象、状态或运行材料。
std::string 世界树类::获取名称(const 基础信息节点类* 节点) const
{
    return 基础信息链_.获取名称(节点);
}

// 功能：把当前世界树本体重写到 SQL Server 查询投影。
bool 世界树类::重写世界树SQL投影(const char* 来源原因, const bool 执行字段恢复比对) const noexcept
{
    std::lock_guard<std::mutex> SQL锁{ 私有_世界树SQL投影互斥() };
    try {
        std::vector<结构_世界树SQL节点行> 节点集{};
        std::vector<结构_世界树SQL关系行> 关系集{};
        std::unordered_set<const 基础信息节点类*> 已访问{};
        {
            auto lk = 基础信息链_.获取读锁();
            结构_世界树SQL节点行 根行{};
            根行.行号 = 1;
            根行.节点主键 = "WORLD_ROOT";
            根行.父节点主键 = {};
            根行.深度 = 0;
            根行.同层序号 = 0;
            根行.直接子数量 = 基础信息链_.世界根()
                ? static_cast<int>(基础信息链_.世界根()->子节点数量)
                : 0;
            根行.路径 = "WORLD_ROOT";
            节点集.push_back(std::move(根行));

            auto* 根 = 基础信息链_.世界根();
            if (根 && 根->子) {
                私有_收集世界树SQL同层(
                    static_cast<const 基础信息节点类*>(根->子),
                    nullptr,
                    "WORLD_ROOT",
                    1,
                    "WORLD_ROOT",
                    已访问,
                    节点集,
                    关系集);
            }
        }
        私有_解析世界树SQL关系序号(节点集, 关系集);

        const auto 原因文本 = 来源原因 ? std::string(来源原因) : std::string{};
        const auto 主库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "master");
        const auto 投影库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "鱼巢投影库");
        std::string 错误{};
        if (!私有_执行世界树ADO命令(主库连接串, "世界树SQL投影建库", 私有_世界树SQL建库脚本(), 错误)
            || !私有_执行世界树ADO命令(投影库连接串, "世界树SQL投影建表", 私有_世界树SQL建表脚本(), 错误)
            || !私有_执行世界树ADO命令(投影库连接串, "世界树SQL投影视图", 私有_世界树SQL视图脚本(), 错误)
            || !私有_执行世界树ADO命令(
                投影库连接串,
                "世界树SQL投影重写",
                私有_构造世界树SQL重写脚本(节点集, 关系集, 原因文本),
                错误,
                180)
            || (执行字段恢复比对 && !私有_验证世界树SQL存储字段(投影库连接串, 节点集, 关系集, 原因文本, 错误))) {
            项目运行错误日志(
                "世界树SQL投影失败"
                " | 原因=" + 错误
                + " | 节点数=" + std::to_string(节点集.size())
                + " | 关系数=" + std::to_string(关系集.size()));
            return false;
        }

        项目运行日志(
            "世界树SQL投影完成"
            " | 来源=" + 原因文本
            + " | 节点数=" + std::to_string(节点集.size())
            + " | 关系数=" + std::to_string(关系集.size())
            + " | 字段恢复比对=" + (执行字段恢复比对 ? "通过" : "跳过"));
        return true;
    }
    catch (const std::exception& 异常) {
        项目运行错误日志(std::string("世界树SQL投影异常 | 原因=") + 异常.what());
    }
    catch (...) {
        项目运行错误日志("世界树SQL投影异常 | 原因=未知异常");
    }
    return false;
}

// 功能：把世界树对象节点当前态同步到 SQL 控制面板运行态显示镜像。
void 世界树类::同步SQL控制面板世界显示项(
    const 基础信息节点类* 节点,
    const char* 阶段) const noexcept
{
    if (!节点 || !节点->主信息) {
        return;
    }

    const auto* 主信息 = 节点->主信息;
    switch (主信息->主信息类型) {
    case 枚举_主信息类型::存在:
    case 枚举_主信息类型::场景:
    case 枚举_主信息类型::状态:
        break;
    default:
        return;
    }

    const auto* 阶段文本 = 阶段 ? 阶段 : "世界事实同步";
    const auto 短标题 = 私有_世界树SQL节点短标题(节点);

    结构_SQL控制面板显示项 显示项{};
    显示项.显示域 = 枚举_SQL控制面板显示域::世界;
    显示项.主键 = 节点->获取主键();
    显示项.父主键 = 节点->父
        ? static_cast<const 基础信息节点类*>(节点->父)->获取主键()
        : std::string{};
    显示项.类型文本 = 私有_世界树SQL主信息类型文本(主信息->主信息类型);
    显示项.显示摘要 = 短标题.empty()
        ? std::string(阶段文本)
        : std::string(阶段文本) + " | " + 短标题;

    std::string 错误{};
    if (同步写入SQL控制面板显示项(显示项, 错误)) {
        return;
    }

    std::string 状态错误{};
    if (!写入SQL控制面板同步状态("世界", 阶段文本, false, 错误, 状态错误)) {
        项目运行警告日志(
            "世界SQL控制面板同步状态写入失败"
            " | 对象=" + 显示项.主键
            + " | 原因=" + 状态错误
            + " | 原始同步错误=" + 错误);
    }
    项目运行警告日志(
        "世界SQL控制面板同步失败"
        " | 对象=" + 显示项.主键
        + " | 阶段=" + 阶段文本
        + " | 原因=" + 错误
        + " | 影响=仅控制面板SQL显示镜像未更新，世界树写入不回滚");
}

// 功能：创建并返回或登记对应对象。
存在节点类* 世界树类::取或创建存在概念根()
{
    存在概念根 = 基础信息链_.取或创建存在概念根();
    return 存在概念根;
}

// 功能：按函数名执行对应处理。
bool 世界树类::是存在概念(const 存在节点类* 节点) const noexcept
{
    return 存在服务_.是存在概念(节点, 存在概念根);
}

// 功能：按函数名执行对应处理。
bool 世界树类::是实例存在(const 存在节点类* 节点) const noexcept
{
    return 存在服务_.是实例存在(节点, 存在概念根);
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保根概念_存在()
{
    return 取或创建存在概念根();
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保根概念_实物存在()
{
    return 确保存在概念(私有_存在概念入口性("实物存在"), 确保根概念_存在());
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保根概念_虚拟存在()
{
    return 确保存在概念(私有_存在概念入口性("虚拟存在"), 确保根概念_存在());
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保根概念_信息存在()
{
    return 确保存在概念(私有_存在概念入口性("信息存在"), 确保根概念_存在());
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保根概念_数字生命()
{
    return 确保存在概念(私有_存在概念入口性("数字生命"), 确保根概念_存在());
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保核心概念_自我()
{
    return 确保存在概念(私有_存在概念入口性("自我"), 确保根概念_数字生命());
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保核心概念_需求()
{
    return 确保存在概念(语素集.添加信息入口词("需求", 枚举_信息入口类型::需求概念入口), 确保根概念_虚拟存在());
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保核心概念_任务()
{
    return 确保存在概念(语素集.添加信息入口词("任务", 枚举_信息入口类型::任务概念入口), 确保根概念_虚拟存在());
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保核心概念_方法()
{
    return 确保存在概念(语素集.添加信息入口词("方法", 枚举_信息入口类型::方法概念入口), 确保根概念_虚拟存在());
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保核心概念_状态()
{
    return 确保存在概念(私有_存在概念入口性("状态"), 确保根概念_信息存在());
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保核心概念_事件()
{
    return 确保存在概念(私有_存在概念入口性("事件"), 确保根概念_信息存在());
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保核心概念_规则()
{
    return 确保存在概念(私有_存在概念入口性("规则"), 确保根概念_信息存在());
}

// 功能：初始化相关对象、状态或运行上下文。
bool 世界树类::初始化核心存在概念树()
{
    bool 全部成功 = 取或创建存在概念根() != nullptr;
    全部成功 = 确保根概念_存在() != nullptr && 全部成功;
    全部成功 = 确保根概念_实物存在() != nullptr && 全部成功;
    全部成功 = 确保根概念_虚拟存在() != nullptr && 全部成功;
    全部成功 = 确保根概念_信息存在() != nullptr && 全部成功;
    全部成功 = 确保根概念_数字生命() != nullptr && 全部成功;
    全部成功 = 确保核心概念_自我() != nullptr && 全部成功;
    全部成功 = 确保核心概念_需求() != nullptr && 全部成功;
    全部成功 = 确保核心概念_任务() != nullptr && 全部成功;
    全部成功 = 确保核心概念_方法() != nullptr && 全部成功;
    全部成功 = 确保核心概念_状态() != nullptr && 全部成功;
    全部成功 = 确保核心概念_事件() != nullptr && 全部成功;
    全部成功 = 确保核心概念_规则() != nullptr && 全部成功;
    return 全部成功;
}

// 功能：初始化相关对象、状态或运行上下文。
bool 世界树类::初始化核心概念默认特征槽()
{
    bool 全部成功 = 初始化核心存在概念树();
    全部成功 = 私有_确保概念特征槽集合(
        *this,
        确保根概念_存在(),
        {
            "所属上下文",
            "来源",
            "存在状态",
            "创建时间",
            "更新时间",
            "最近确认时间",
            "可信度",
            "描述",
        }) && 全部成功;

    全部成功 = 私有_确保概念特征槽集合(
        *this,
        确保核心概念_任务(),
        {
            "发起存在",
            "作用对象",
            "任务目标",
            "任务状态",
            "完成条件",
            "候选方法",
            "任务方法候选集",
            "当前方法",
        }) && 全部成功;

    全部成功 = 私有_确保概念特征槽集合(
        *this,
        确保核心概念_方法(),
        {
            "方法类型",
            "方法来源",
            "方法状态",
            "输入信息",
            "输出结果",
            "前置条件",
            "成功条件",
            "方法体",
            "方法节点种类",
            "方法作用域",
            "动作句柄类型",
            "方法动作句柄类型",
            "本能动作ID",
            "自我能力",
            "方法是否可用",
            "方法结构是否完整",
            "方法稳定状态",
            "方法运行账完整性",
            "学习方向状态",
            "目标特征类型识别状态",
            "目标变化方向识别状态",
            "候选方法存在状态",
            "方法动作状态",
            "方法动作数量",
            "方法条件结果配对状态",
            "方法新用途发现状态",
            "可用方法数量镜像一致性",
            "成熟度阶段",
            "方法成熟度阶段",
            "允许自动召回",
            "允许默认执行",
            "方法允许自动查找",
            "特征账本生命周期",
            "方法结果初始状态",
            "父方法",
            "前置方法",
            "后续方法",
            "方法条件场景",
            "方法结果场景",
            "方法能力结果数量",
            "方法可查找结果数量",
            "方法成功次数",
            "方法失败次数",
            "方法候选命中次数",
            "方法最近执行时间",
            "暴露次数",
            "读取次数",
            "有效使用次数",
            "历史使用次数",
            "近期使用次数",
            "作为决策输入次数",
            "作为派生父特征次数",
            "作为需求触发次数",
            "作为风险闸门次数",
            "作为验证信号次数",
            "作为解释依据次数",
            "结果改变次数",
            "坏结果次数",
            "下游依赖特征数",
            "最近使用时间",
            "最近有效使用时间",
        }) && 全部成功;

    全部成功 = 私有_确保概念特征槽集合(
        *this,
        确保核心概念_自我(),
        {
            "安全值",
            "服务值",
            "风险安全值",
            "待机状态",
            "时序正向步长",
            "时序反向步长",
            "服务时序衰减步长",
        }) && 全部成功;
    return 全部成功;
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
bool 世界树类::绑定核心概念语素()
{
    bool 全部成功 = 初始化核心存在概念树();
    const auto 绑定 = [&](const char* 词, 枚举_信息入口类型 信息入口类型, 存在节点类* 概念) noexcept {
        if (!词 || !*词 || !概念) {
            return false;
        }
        auto* 词性 = 语素集.添加信息入口词(词, 信息入口类型);
        if (!词性) {
            return false;
        }
        try {
            return 语素集.添加对应基础信息(词性, 概念) > 0;
        }
        catch (...) {
            return false;
        }
    };

    全部成功 = 绑定("存在", 枚举_信息入口类型::存在概念入口, 确保根概念_存在()) && 全部成功;
    全部成功 = 绑定("实物存在", 枚举_信息入口类型::存在概念入口, 确保根概念_实物存在()) && 全部成功;
    全部成功 = 绑定("虚拟存在", 枚举_信息入口类型::存在概念入口, 确保根概念_虚拟存在()) && 全部成功;
    全部成功 = 绑定("信息存在", 枚举_信息入口类型::存在概念入口, 确保根概念_信息存在()) && 全部成功;
    全部成功 = 绑定("数字生命", 枚举_信息入口类型::存在概念入口, 确保根概念_数字生命()) && 全部成功;
    全部成功 = 绑定("自我", 枚举_信息入口类型::存在概念入口, 确保核心概念_自我()) && 全部成功;
    全部成功 = 绑定("需求", 枚举_信息入口类型::需求概念入口, 确保核心概念_需求()) && 全部成功;
    全部成功 = 绑定("任务", 枚举_信息入口类型::任务概念入口, 确保核心概念_任务()) && 全部成功;
    全部成功 = 绑定("方法", 枚举_信息入口类型::方法概念入口, 确保核心概念_方法()) && 全部成功;
    全部成功 = 绑定("状态", 枚举_信息入口类型::存在概念入口, 确保核心概念_状态()) && 全部成功;
    全部成功 = 绑定("事件", 枚举_信息入口类型::存在概念入口, 确保核心概念_事件()) && 全部成功;
    全部成功 = 绑定("规则", 枚举_信息入口类型::存在概念入口, 确保核心概念_规则()) && 全部成功;
    return 全部成功;
}

// 功能：初始化相关对象、状态或运行上下文。
bool 世界树类::初始化存在概念系统()
{
    bool 全部成功 = 初始化核心存在概念树();
    全部成功 = 特征服务_.初始化常用抽象特征(取或创建抽象特征根()) && 全部成功;
    全部成功 = 初始化核心概念默认特征槽() && 全部成功;
    全部成功 = 绑定核心概念语素() && 全部成功;
    return 全部成功;
}

// 功能：创建并返回或登记对应对象。
基础信息节点类* 世界树类::取或创建抽象特征根()
{
    if (抽象特征根 && 基础信息链_.取主信息<抽象特征主信息类>(抽象特征根)) {
        return 抽象特征根;
    }

    const auto* 根词 = 语素集.添加信息入口词("抽象特征根", 枚举_信息入口类型::特征模板入口);
    for (auto* 节点 : 私有_直接基础信息子节点_按类型<抽象特征主信息类>(基础信息链_, 世界根())) {
        const auto* 主信息 = 基础信息链_.取主信息<抽象特征主信息类>(节点);
        if (主信息 && 主信息->名称 == 根词) {
            抽象特征根 = 节点;
            return 抽象特征根;
        }
    }

    auto* 主信息 = new 抽象特征主信息类{};
    主信息->名称 = 根词;
    主信息->类型 = 根词;
    主信息->设为复合特征();
    抽象特征根 = 特征服务_.创建抽象特征(世界根(), 主信息);
    return 抽象特征根;
}

// 功能：读取抽象特征节点主信息，不创建或修改节点。
抽象特征主信息类* 世界树类::取抽象特征主信息(抽象特征节点类* 节点) const noexcept
{
    return 特征服务_.取抽象特征主信息(节点);
}

// 功能：读取抽象特征节点主信息，不创建或修改节点。
const 抽象特征主信息类* 世界树类::取抽象特征主信息(const 抽象特征节点类* 节点) const noexcept
{
    return 特征服务_.取抽象特征主信息(节点);
}

// 功能：创建并返回或登记对应对象。
抽象特征节点类* 世界树类::创建抽象特征(基础信息节点类* 父抽象特征, 抽象特征主信息类* 主信息)
{
    return 特征服务_.创建抽象特征(父抽象特征 ? 父抽象特征 : 取或创建抽象特征根(), 主信息);
}

// 功能：创建并返回或登记对应对象。
抽象特征节点类* 世界树类::创建叶子抽象特征(
    基础信息节点类* 父抽象特征,
    const 语素入口节点类* 名称,
    const 语素入口节点类* 类型,
    特征值域 值域)
{
    return 特征服务_.创建叶子抽象特征(
        父抽象特征 ? 父抽象特征 : 取或创建抽象特征根(),
        名称,
        类型,
        std::move(值域));
}

// 功能：创建并返回或登记对应对象。
抽象特征节点类* 世界树类::创建复合抽象特征(
    基础信息节点类* 父抽象特征,
    const 语素入口节点类* 名称,
    const 语素入口节点类* 类型)
{
    return 特征服务_.创建复合抽象特征(
        父抽象特征 ? 父抽象特征 : 取或创建抽象特征根(),
        名称,
        类型);
}

// 功能：按类型查找抽象特征直接子节点，不创建或修改节点。
抽象特征节点类* 世界树类::查找子抽象特征_按类型(
    const 基础信息节点类* 父抽象特征,
    const 语素入口节点类* 特征类型) const
{
    return 特征服务_.查找子抽象特征_按类型(父抽象特征, 特征类型);
}

// 功能：按函数名执行对应处理。
bool 世界树类::校验抽象特征子链(const 基础信息节点类* 抽象特征节点) const
{
    return 特征服务_.校验抽象特征子链(抽象特征节点);
}

// 功能：按函数名执行对应处理。
bool 世界树类::匹配抽象特征(const 抽象特征节点类* 抽象特征, const 特征节点类* 实例特征) const
{
    return 特征服务_.匹配抽象特征(抽象特征, 实例特征);
}

// 功能：初始化常用抽象特征节点。
bool 世界树类::初始化常用抽象特征(基础信息节点类* 抽象特征根)
{
    return 特征服务_.初始化常用抽象特征(抽象特征根);
}

// 功能：创建并返回或登记对应对象。
抽象特征节点类* 世界树类::取或创建抽象特征_按类型(
    基础信息节点类* 父抽象特征,
    const 语素入口节点类* 特征类型)
{
    return 特征服务_.取或创建子抽象特征_按类型(
        父抽象特征 ? 父抽象特征 : 取或创建抽象特征根(),
        特征类型);
}

// 功能：创建并返回或登记对应对象。
特征节点类* 世界树类::取或创建实例特征_按抽象特征(
    基础信息节点类* 宿主,
    抽象特征节点类* 抽象特征)
{
    return 特征服务_.取或创建实例特征_按抽象特征(宿主, 抽象特征);
}

// 功能：解析输入文本、消息、场景或运行包。
抽象特征节点类* 世界树类::解析实例特征命中抽象特征(特征节点类* 实例特征)
{
    return 特征服务_.解析实例特征命中抽象特征(实例特征);
}

// 功能：按函数名执行对应处理。
抽象特征节点类* 世界树类::升格实例稳态为抽象区间(特征节点类* 实例特征)
{
    return 特征服务_.升格当前稳态为抽象区间(实例特征);
}

// 功能：按函数名执行对应处理。
bool 世界树类::求值组合抽象特征(
    基础信息节点类* 宿主,
    抽象特征节点类* 组合抽象特征,
    时间戳 now)
{
    return 特征服务_.求值组合抽象特征(宿主, 组合抽象特征, now);
}

// 功能：创建并返回或登记对应对象。
特征节点类* 世界树类::创建特征(基础信息节点类* 宿主, 特征节点主信息类* 主信息)
{
    return 特征服务_.创建特征(宿主, 主信息);
}

// 功能：按类型创建特征节点，由世界树入口承接基础信息子节点创建。
特征节点类* 世界树类::创建特征_按类型(
    基础信息节点类* 宿主,
    const 语素入口节点类* 特征类型,
    const 特征值& 当前值)
{
    return 特征服务_.创建特征_按类型(宿主, 特征类型, 当前值);
}

// 功能：删除特征节点，由世界树入口承接基础信息节点删除。
bool 世界树类::删除特征(特征节点类* 节点)
{
    return 特征服务_.删除特征(节点);
}

// 功能：确保目标结构、状态或前置条件存在并可用。
特征节点类* 世界树类::确保特征(基础信息节点类* 宿主, const 语素入口节点类* 特征类型)
{
    return 特征服务_.取或创建子特征_按类型(宿主, 特征类型);
}

// 功能：创建并返回或登记对应对象。
特征节点类* 世界树类::取或创建子特征_按类型(基础信息节点类* 宿主, const 语素入口节点类* 特征类型)
{
    return 特征服务_.取或创建子特征_按类型(宿主, 特征类型);
}

// 功能：读取宿主下的直接子特征，不创建或修改节点。
std::vector<特征节点类*> 世界树类::获取子特征(const 基础信息节点类* 宿主) const
{
    return 特征服务_.获取子特征(宿主);
}

// 功能：按类型查找宿主下的直接子特征，不创建或修改节点。
特征节点类* 世界树类::查找子特征_按类型(
    const 基础信息节点类* 宿主,
    const 语素入口节点类* 特征类型) const
{
    return 特征服务_.查找子特征_按类型(宿主, 特征类型);
}

// 功能：读取特征节点主信息，不创建或修改节点。
特征节点主信息类* 世界树类::取特征主信息(特征节点类* 节点) const noexcept
{
    return 特征服务_.取特征主信息(节点);
}

// 功能：读取特征节点主信息，不创建或修改节点。
const 特征节点主信息类* 世界树类::取特征主信息(const 特征节点类* 节点) const noexcept
{
    return 特征服务_.取特征主信息(节点);
}

// 功能：比较当前值、目标值或历史基准。
结构_特征状态比较结果 世界树类::比较状态(
    const 状态节点类* 当前状态,
    const 状态节点类* 目标状态) const
{
    return 特征服务_.比较状态(当前状态, 目标状态);
}

// 功能：读取并返回特征值比较方法结果。
特征比较方法结果 世界树类::获取比较方法结果(
    const 语素入口节点类* 特征类型,
    const 特征值& 左值,
    const 特征值& 右值,
    const 特征节点主信息类* 左特征主信息,
    const 特征节点主信息类* 右特征主信息) const
{
    return 特征服务_.获取比较方法结果(特征类型, 左值, 右值, 左特征主信息, 右特征主信息);
}

// 功能：读取存在节点主信息，不创建或修改节点。
存在节点主信息类* 世界树类::取存在主信息(存在节点类* 节点) const noexcept
{
    return 存在服务_.取存在主信息(节点);
}

// 功能：读取存在节点主信息，不创建或修改节点。
const 存在节点主信息类* 世界树类::取存在主信息(const 存在节点类* 节点) const noexcept
{
    return 存在服务_.取存在主信息(节点);
}

// 功能：判断基础信息节点是否承载存在主信息。
bool 世界树类::是存在节点(const 基础信息节点类* 节点) const noexcept
{
    return 存在服务_.是存在节点(节点);
}

// 功能：判断基础信息节点是否为场景节点，不创建或修改节点。
bool 世界树类::是场景节点(const 基础信息节点类* 节点) const noexcept
{
    return 场景服务_.是场景节点(节点);
}

// 功能：读取存在名称入口，不创建或补齐任何结构。
const 语素入口节点类* 世界树类::读取存在名称(const 存在节点类* 节点) const noexcept
{
    return 存在服务_.读取存在名称(节点);
}

// 功能：读取存在类型入口，不创建或补齐任何结构。
const 语素入口节点类* 世界树类::读取存在类型(const 存在节点类* 节点) const noexcept
{
    return 存在服务_.读取存在类型(节点);
}

// 功能：读取并返回指定对象、状态或运行材料。
std::vector<存在节点类*> 世界树类::获取概念集(const 存在节点类* 节点) const
{
    return 存在服务_.获取概念集(节点);
}

// 功能：读取最近观测位置缓存；缓存只作为特征化或显示输入。
bool 世界树类::读取最近观测位置缓存(const 存在节点类* 节点, Vector3D& 输出位置_mm) const noexcept
{
    return 存在服务_.读取最近观测位置缓存(节点, 输出位置_mm);
}

// 功能：确保目标结构、状态或前置条件存在并可用。
bool 世界树类::确保存在三类根节点(存在节点类* 节点) const noexcept
{
    return 存在服务_.确保存在三类根节点(节点);
}

// 功能：创建并返回或登记对应对象。
存在节点类* 世界树类::创建存在(基础信息节点类* 父节点, 存在节点主信息类* 主信息)
{
    auto* 节点 = 存在服务_.创建存在(父节点, 主信息);
    同步SQL控制面板世界显示项(节点, "创建存在");
    return 节点;
}

// 功能：按名称和类型取或创建父节点下的直接子存在。
存在节点类* 世界树类::取或创建子存在_按名称类型(
    基础信息节点类* 父节点,
    const 语素入口节点类* 名称,
    const 语素入口节点类* 类型)
{
    const bool 已存在 = 名称
        ? 存在服务_.查找子存在_按名称(父节点, 名称) != nullptr
        : (类型 ? 存在服务_.查找子存在_按类型(父节点, 类型) != nullptr : false);
    auto* 节点 = 存在服务_.取或创建子存在_按名称类型(父节点, 名称, 类型);
    const bool 已补概念 = 私有_补齐类型概念集(存在服务_, 节点, 类型);
    if (节点 && !已存在) {
        同步SQL控制面板世界显示项(节点, "取或创建子存在/新建");
    }
    else if (节点 && 已补概念) {
        同步SQL控制面板世界显示项(节点, "取或创建子存在/概念集补齐");
    }
    return 节点;
}

// 功能：创建并返回或登记对应对象。
存在节点类* 世界树类::取或创建子存在_按类型(基础信息节点类* 父节点, const 语素入口节点类* 类型)
{
    if (!类型) return nullptr;
    if (auto* 已有存在 = 存在服务_.查找子存在_按类型(父节点, 类型)) {
        if (私有_补齐类型概念集(存在服务_, 已有存在, 类型)) {
            同步SQL控制面板世界显示项(已有存在, "取或创建子存在按类型/概念集补齐");
        }
        return 已有存在;
    }
    auto* 节点 = 存在服务_.创建存在_按名称类型(父节点, nullptr, 类型);
    (void)私有_补齐类型概念集(存在服务_, 节点, 类型);
    同步SQL控制面板世界显示项(节点, "取或创建子存在按类型/新建");
    return 节点;
}

// 功能：按名称查找父节点下的直接子存在，不创建或修改节点。
存在节点类* 世界树类::查找子存在_按名称(
    const 基础信息节点类* 父节点,
    const 语素入口节点类* 名称) const
{
    return 存在服务_.查找子存在_按名称(父节点, 名称);
}

// 功能：按类型查找父节点下的直接子存在，不创建或修改节点。
存在节点类* 世界树类::查找子存在_按类型(
    const 基础信息节点类* 父节点,
    const 语素入口节点类* 类型) const
{
    return 存在服务_.查找子存在_按类型(父节点, 类型);
}

// 功能：读取需求根节点，不创建或修改节点。
需求类::节点类* 世界树类::获取需求根节点(存在节点类* 节点) const noexcept
{
    return 存在服务_.获取需求根节点(节点);
}

// 功能：读取需求根节点，不创建或修改节点。
const 需求类::节点类* 世界树类::获取需求根节点(const 存在节点类* 节点) const noexcept
{
    return 存在服务_.获取需求根节点(节点);
}

// 功能：读取任务根节点，不创建或修改节点。
任务类::节点类* 世界树类::获取任务根节点(存在节点类* 节点) const noexcept
{
    return 存在服务_.获取任务根节点(节点);
}

// 功能：读取任务根节点，不创建或修改节点。
const 任务类::节点类* 世界树类::获取任务根节点(const 存在节点类* 节点) const noexcept
{
    return 存在服务_.获取任务根节点(节点);
}

// 功能：读取方法根节点，不创建或修改节点。
方法类::节点类* 世界树类::获取方法根节点(存在节点类* 节点) const noexcept
{
    return 存在服务_.获取方法根节点(节点);
}

// 功能：读取方法根节点，不创建或修改节点。
const 方法类::节点类* 世界树类::获取方法根节点(const 存在节点类* 节点) const noexcept
{
    return 存在服务_.获取方法根节点(节点);
}

// 功能：创建并返回或登记对应对象。
存在节点类* 世界树类::取或创建子存在_按类型并按特征I64(
    基础信息节点类* 父节点,
    const 语素入口节点类* 类型,
    const 语素入口节点类* 唯一键特征类型,
    I64 唯一键特征值)
{
    if (!类型 || !唯一键特征类型) return nullptr;

    for (auto* 存在节点 : 存在服务_.获取子存在(父节点)) {
        const auto* 存在类型 = 存在服务_.读取存在类型(存在节点);
        if (!语素入口同一(存在类型, 类型)) continue;

        auto* 特征节点 = 特征服务_.查找子特征_按类型(存在节点, 唯一键特征类型);
        I64 当前值 = 0;
        if (特征节点 && 读取特征_I64(特征节点, 当前值) && 当前值 == 唯一键特征值) {
            if (私有_补齐类型概念集(存在服务_, 存在节点, 类型)) {
                同步SQL控制面板世界显示项(存在节点, "取或创建子存在按特征/概念集补齐");
            }
            return 存在节点;
        }
    }

    auto* 新存在 = 存在服务_.创建存在_按名称类型(父节点, nullptr, 类型);
    if (!新存在) return nullptr;
    (void)私有_补齐类型概念集(存在服务_, 新存在, 类型);
    同步SQL控制面板世界显示项(新存在, "取或创建子存在按特征/新建");

    auto* 特征节点 = 确保特征(新存在, 唯一键特征类型);
    if (!特征节点) return 新存在;

    (void)写入特征_I64(特征节点, 唯一键特征值);
    return 新存在;
}

// 功能：确保目标结构、状态或前置条件存在并可用。
存在节点类* 世界树类::确保存在概念(
    const 语素入口节点类* 名称,
    存在节点类* 父概念)
{
    if (!名称) return nullptr;
    auto* 父节点 = 父概念 ? 父概念 : 取或创建存在概念根();
    const bool 已存在 = 存在服务_.查找子存在_按名称(父节点, 名称) != nullptr;
    auto* 概念 = 存在服务_.确保存在概念(父节点, 名称);
    if (!概念) return nullptr;
    if (!已存在) {
        同步SQL控制面板世界显示项(概念, "确保存在概念/新建");
    }

    if (auto* 主信息 = 存在服务_.取存在主信息(概念)) {
        const auto* 父主信息 = 存在服务_.取存在主信息(dynamic_cast<const 存在节点类*>(父节点));
        const auto* 上位归属类型 = 父主信息
            ? (父主信息->名称 ? 父主信息->名称 : 父主信息->类型)
            : nullptr;
        if (上位归属类型 && !语素入口同一(主信息->类型, 上位归属类型)) {
            主信息->类型 = 上位归属类型;
        }
    }

    try {
        (void)语素集.添加对应基础信息(名称, 概念);
    }
    catch (...) {
        // 非名词或已有特殊绑定时不阻断概念创建；核心名词入口由绑定核心概念语素兜底。
    }
    return 概念;
}

// 功能：创建并返回或登记对应对象。
存在节点类* 世界树类::创建存在实例(
    const 语素入口节点类* 名称,
    const std::vector<存在节点类*>& 概念集,
    场景节点类* 所属场景)
{
    auto* 父节点 = 所属场景 ? 所属场景 : (现实世界 ? 现实世界 : 取或创建自我现实场景());
    auto* 实例 = 存在服务_.创建存在实例(父节点, 名称, 概念集);
    同步SQL控制面板世界显示项(实例, "创建存在实例");
    根据概念扩充实例记忆(实例, 概念集);
    return 实例;
}

// 功能：按函数名执行对应处理。
bool 世界树类::添加实例概念(
    存在节点类* 实例,
    存在节点类* 概念,
    bool 是否扩充记忆)
{
    const bool 已添加 = 存在服务_.添加实例概念(实例, 概念, false);
    if (是否扩充记忆) {
        根据概念扩充实例记忆(实例, std::vector<存在节点类*>{ 概念 });
    }
    return 已添加;
}

// 功能：按函数名执行对应处理。
void 世界树类::根据概念扩充实例记忆(
    存在节点类* 实例,
    const std::vector<存在节点类*>& 概念集)
{
    if (!实例) return;

    std::vector<存在节点类*> 已收集{};
    std::vector<存在节点类*> 扩充顺序{};
    const auto 追加唯一概念 = [](std::vector<存在节点类*>& 列表, 存在节点类* 概念) {
        if (!概念) return false;
        if (std::find(列表.begin(), 列表.end(), 概念) != 列表.end()) return false;
        列表.push_back(概念);
        return true;
    };

    const auto 收集概念 = [&](auto&& self, 存在节点类* 概念) -> void {
        if (!概念 || 概念 == 实例) return;
        if (!追加唯一概念(已收集, 概念)) return;

        for (auto* 上位概念 : 存在服务_.获取概念集(概念)) {
            self(self, 上位概念);
        }
        扩充顺序.push_back(概念);
    };

    for (auto* 概念 : 概念集) {
        收集概念(收集概念, 概念);
    }

    for (auto* 概念 : 扩充顺序) {
        (void)存在服务_.添加实例概念(实例, 概念, false);
        (void)特征服务_.复制子特征到宿主(实例, 概念);
    }
}

// 功能：按函数名执行对应处理。
bool 世界树类::存在包含概念(const 存在节点类* 实例, const 存在节点类* 概念) const noexcept
{
    return 存在服务_.存在包含概念(实例, 概念);
}

// 功能：读取父节点下的直接子存在，不创建或修改节点。
std::vector<存在节点类*> 世界树类::获取子存在(const 基础信息节点类* 父节点) const
{
    return 存在服务_.获取子存在(父节点);
}

// 功能：枚举世界树中的全部存在节点，不创建或修改节点。
std::vector<存在节点类*> 世界树类::枚举全部存在() const
{
    return 存在服务_.枚举全部存在();
}

// 功能：按条件查找目标对象、方法或事实。
std::vector<存在节点类*> 世界树类::查找实例_按概念(
    const 存在节点类* 概念,
    const 基础信息节点类* 范围根) const
{
    return 存在服务_.查找实例_按概念(概念, 范围根);
}

// 功能：创建并返回或登记对应对象。
场景节点类* 世界树类::创建场景(基础信息节点类* 父节点, 场景节点主信息类* 主信息)
{
    auto* 节点 = 场景服务_.创建场景(父节点, 主信息);
    同步SQL控制面板世界显示项(节点, "创建场景");
    return 节点;
}

// 功能：按名称创建场景节点，由世界树入口承接基础信息子节点创建。
场景节点类* 世界树类::创建场景_按名称(
    基础信息节点类* 父节点,
    const 语素入口节点类* 名称,
    枚举_世界类型 世界类型)
{
    auto* 节点 = 场景服务_.创建场景_按名称(父节点, 名称, 世界类型);
    同步SQL控制面板世界显示项(节点, "创建场景按名称");
    return 节点;
}

// 功能：创建并返回或登记对应对象。
场景节点类* 世界树类::取或创建子场景_按名称(
    基础信息节点类* 父节点,
    const 语素入口节点类* 名称,
    枚举_世界类型 世界类型)
{
    const bool 已存在 = 场景服务_.查找子场景_按名称(父节点, 名称) != nullptr;
    auto* 节点 = 场景服务_.取或创建子场景_按名称(父节点, 名称, 世界类型);
    if (节点 && !已存在) {
        同步SQL控制面板世界显示项(节点, "取或创建子场景/新建");
    }
    return 节点;
}

// 功能：建立场景与宿主存在之间的绑定关系。
bool 世界树类::绑定场景宿主(场景节点类* 节点, 存在节点类* 宿主存在)
{
    return 场景服务_.绑定宿主(节点, 宿主存在);
}

// 功能：读取场景宿主存在引用，优先返回缓存指针，缺指针时按主键只读解析。
存在节点类* 世界树类::读取场景宿主存在(const 场景节点类* 场景) const noexcept
{
    return 场景服务_.读取场景宿主存在(场景);
}

// 功能：读取场景状态事实目录的引用快照，不解释状态值业务含义。
std::vector<可解析引用<状态节点类>> 世界树类::读取场景状态索引快照(const 场景节点类* 场景) const
{
    return 场景服务_.读取场景状态索引快照(场景);
}

// 功能：读取场景二次特征事实目录的引用快照，不解释二次特征业务含义。
std::vector<可解析引用<二次特征节点类>> 世界树类::读取场景二次特征索引快照(const 场景节点类* 场景) const
{
    return 场景服务_.读取场景二次特征索引快照(场景);
}

// 功能：在场景直接子存在中按局部深度/彩图轮廓查找候选存在，只返回比较结果。
std::vector<结构_场景三维体素轮廓存在比较结果> 世界树类::查找存在_按深度或彩图轮廓图(
    const 场景节点类* 场景,
    const std::vector<结构_三维体素轮廓图视角>& 局部视角集合,
    const 结构_三维体素轮廓融合参数& 局部融合参数,
    const 结构_场景三维体素轮廓查找参数& 参数,
    const 存在节点类* 存在概念根) const
{
    return 场景服务_.查找存在_按深度或彩图轮廓图(
        场景,
        局部视角集合,
        局部融合参数,
        参数,
        值池_,
        存在概念根);
}

// 功能：创建并返回或登记对应对象。
二次特征节点类* 世界树类::创建二次特征(基础信息节点类* 父节点, 二次特征主信息类* 主信息)
{
    return 二次特征服务_.创建二次特征(父节点, 主信息);
}

// 功能：按度量签名创建二次特征节点。
二次特征节点类* 世界树类::创建二次特征_按度量(
    基础信息节点类* 父节点,
    枚举_二次特征种类 种类,
    const std::string& 度量签名_链键)
{
    return 二次特征服务_.创建二次特征_按度量(父节点, 种类, 度量签名_链键);
}

// 功能：读取父节点下的直接子二次特征，不创建或修改节点。
std::vector<二次特征节点类*> 世界树类::获取子二次特征(const 基础信息节点类* 父节点) const
{
    return 二次特征服务_.获取子二次特征(父节点);
}

// 功能：按二次特征签名读取或显式创建父节点下的子二次特征。
二次特征节点类* 世界树类::获取或创建子二次特征_按签名(
    基础信息节点类* 父节点,
    const 二次特征主信息类& 模板)
{
    return 二次特征服务_.获取或创建子二次特征_按签名(父节点, 模板);
}

// 功能：读取二次特征节点主信息，不创建或修改节点。
二次特征主信息类* 世界树类::取二次特征主信息(二次特征节点类* 节点) const noexcept
{
    return 二次特征服务_.取二次特征主信息(节点);
}

// 功能：读取二次特征节点主信息，不创建或修改节点。
const 二次特征主信息类* 世界树类::取二次特征主信息(const 二次特征节点类* 节点) const noexcept
{
    return 二次特征服务_.取二次特征主信息(节点);
}

// 功能：绑定二次特征所属场景。
bool 世界树类::绑定二次特征场景(二次特征节点类* 节点, 场景节点类* 所属场景)
{
    return 二次特征服务_.绑定场景(节点, 所属场景);
}

// 功能：设置二次特征生成语义。
bool 世界树类::设置二次特征生成语义(
    二次特征节点类* 节点,
    枚举_二次特征域 域,
    枚举_二次特征粒度 粒度,
    枚举_二次特征基准类型 基准类型,
    枚举_时间归一方式 时间归一方式,
    枚举_二次特征值形态 值形态)
{
    return 二次特征服务_.设置生成语义(节点, 域, 粒度, 基准类型, 时间归一方式, 值形态);
}

// 功能：绑定二次特征来源状态、动态和场景。
bool 世界树类::绑定二次特征来源(
    二次特征节点类* 节点,
    状态节点类* 来源起始状态,
    状态节点类* 来源结果状态,
    动态节点类* 来源动态,
    场景节点类* 来源场景,
    std::uint32_t 来源动态层级,
    std::uint64_t 来源动态路径签名,
    枚举_二次特征来源类型 来源类型,
    结构_时间段 来源时间段,
    时间戳 分步时长,
    时间戳 总时长,
    时间戳 窗口时长)
{
    return 二次特征服务_.绑定来源(
        节点,
        来源起始状态,
        来源结果状态,
        来源动态,
        来源场景,
        来源动态层级,
        来源动态路径签名,
        来源类型,
        来源时间段,
        分步时长,
        总时长,
        窗口时长);
}

// 功能：设置二次特征主体和客体。
bool 世界树类::设置二次特征主体客体(二次特征节点类* 节点, 基础信息节点类* 主体, 基础信息节点类* 客体)
{
    return 二次特征服务_.设置主体客体(节点, 主体, 客体);
}

// 功能：设置二次特征左右比较对象。
bool 世界树类::设置二次特征左右对象(二次特征节点类* 节点, 基础信息节点类* 左对象, 基础信息节点类* 右对象)
{
    return 二次特征服务_.设置左右对象(节点, 左对象, 右对象);
}

// 功能：写入二次特征差值比较结果。
bool 世界树类::写入二次特征差值结果(二次特征节点类* 节点, I64 左值, I64 右值)
{
    return 二次特征服务_.写入差值结果(节点, 左值, 右值);
}

// 功能：写入二次特征标量值和满足状态。
bool 世界树类::写入二次特征标量值(二次特征节点类* 节点, I64 标量值, bool 是否满足, I64 置信度)
{
    return 二次特征服务_.写入标量值(节点, 标量值, 是否满足, 置信度);
}

// 功能：写入观察关系二次特征。
bool 世界树类::写入观察关系二次特征(
    场景节点类* 场景,
    基础信息节点类* 主体,
    基础信息节点类* 客体,
    动态节点类* 来源动态,
    const char* 关系名,
    I64 标量值,
    bool 使用标量值)
{
    return 二次特征服务_.写入观察关系二次特征(
        场景,
        主体,
        客体,
        来源动态,
        关系名,
        标量值,
        使用标量值);
}

// 功能：在已读取主信息时写入二次特征标量值和满足状态。
bool 世界树类::写入二次特征标量值_已取主信息(
    二次特征节点类* 节点,
    二次特征主信息类* 主信息,
    I64 标量值,
    bool 是否满足,
    I64 置信度,
    时间戳 观测时间戳,
    bool 跳过内部采样)
{
    return 二次特征服务_.写入标量值_已取主信息(
        节点,
        主信息,
        标量值,
        是否满足,
        置信度,
        观测时间戳,
        跳过内部采样);
}

// 功能：在已读取主信息时写入二次特征离散值。
bool 世界树类::写入二次特征离散值_已取主信息(
    二次特征节点类* 节点,
    二次特征主信息类* 主信息,
    std::int32_t 离散编码,
    const std::string& 离散语义键,
    枚举_二次特征值形态 值形态,
    I64 置信度,
    时间戳 观测时间戳,
    bool 跳过内部采样)
{
    return 二次特征服务_.写入离散值_已取主信息(
        节点,
        主信息,
        离散编码,
        离散语义键,
        值形态,
        置信度,
        观测时间戳,
        跳过内部采样);
}

// 功能：根据二次特征当前主信息刷新概念模板引用。
bool 世界树类::刷新二次特征概念模板(二次特征节点类* 节点)
{
    return 二次特征服务_.刷新概念模板(节点);
}

// 功能：确保二次特征引用列表中的条件节点具备默认值。
void 世界树类::确保二次特征列表具备默认值(
    const std::vector<可解析引用<二次特征节点类>>& 条件索引,
    const char* 默认离散语义键) noexcept
{
    二次特征生成服务_->确保二次特征列表具备默认值(条件索引, 默认离散语义键);
}

// 功能：按场景、特征节点、方向、主体和领域查找二次特征主信息，不创建节点。
const 二次特征主信息类* 世界树类::查找场景二次特征主信息(
    场景节点类* 场景,
    const 基础信息节点类* 特征节点,
    方向掩码 方向约束,
    const 基础信息节点类* 主体,
    const std::optional<枚举_二次特征域>& 域,
    bool 取最近观测) const noexcept
{
    return 二次特征生成服务_->查找场景二次特征主信息(
        场景,
        特征节点,
        方向约束,
        主体,
        域,
        取最近观测);
}

// 功能：读取场景二次特征布尔值，不创建节点。
std::optional<bool> 世界树类::读取场景二次特征布尔(
    场景节点类* 场景,
    const 基础信息节点类* 特征节点,
    方向掩码 方向约束,
    const 基础信息节点类* 主体,
    const std::optional<枚举_二次特征域>& 域) const noexcept
{
    return 二次特征生成服务_->读取场景二次特征布尔(
        场景,
        特征节点,
        方向约束,
        主体,
        域);
}

// 功能：把自我治理候选事实写入场景二次特征。
bool 世界树类::写入治理二次特征(
    场景节点类* 场景,
    const 语素入口节点类* 概念入口,
    枚举_二次特征种类 种类,
    枚举_二次特征域 域,
    枚举_二次特征粒度 粒度,
    枚举_二次特征基准类型 基准类型,
    枚举_时间归一方式 时间归一方式,
    枚举_二次特征值形态 值形态,
    基础信息节点类* 主体,
    基础信息节点类* 客体,
    基础信息节点类* 左对象,
    基础信息节点类* 右对象,
    I64 标量值,
    bool 是否满足,
    I64 置信度,
    二次特征节点类** 输出节点)
{
    结构_场景二次特征写入请求 请求{};
    请求.概念入口 = 概念入口;
    请求.同步名称和类型 = true;
    请求.种类 = 种类;
    请求.域 = 域;
    请求.粒度 = 粒度;
    请求.基准类型 = 基准类型;
    请求.时间归一方式 = 时间归一方式;
    请求.值形态 = 值形态;
    请求.主体 = 主体;
    请求.客体 = 客体;
    请求.左对象 = 左对象;
    请求.右对象 = 右对象;
    请求.离散编码 = 是否满足 ? 1 : 0;
    请求.标量值 = 标量值;
    请求.是否满足 = 是否满足;
    请求.置信度 = 置信度;
    return 二次特征生成服务_->写入场景二次特征(场景, 请求, 输出节点);
}

// 功能：读取或生成指定动态关联的二次特征集合。
std::vector<二次特征节点类*> 世界树类::获取动态二次特征(const 动态节点类& 动态)
{
    return 二次特征生成服务_->获取动态二次特征(动态);
}

// 功能：为相邻动态生成协同二次特征。
二次特征节点类* 世界树类::生成相邻动态协同二次特征(
    场景节点类* 场景,
    动态节点类* 动态,
    时间戳 时间窗口)
{
    return 二次特征生成服务_->生成相邻动态协同二次特征(场景, 动态, 时间窗口);
}

// 功能：尝试按连续同向规则生成聚合动态。
动态节点类* 世界树类::尝试生成连续同向聚合动态(场景节点类* 场景, 动态节点类* 动态)
{
    return 二次特征生成服务_->尝试生成连续同向聚合动态(场景, 动态);
}

// 功能：尝试按同类事件段规则生成聚合动态。
动态节点类* 世界树类::尝试生成同类事件段聚合动态(场景节点类* 场景, 动态节点类* 动态)
{
    return 二次特征生成服务_->尝试生成同类事件段聚合动态(场景, 动态);
}

// 功能：把轮廓比较结果写入二次特征材料。
bool 世界树类::写入轮廓比较二次特征(
    基础信息节点类* 父节点,
    const 语素入口节点类* 特征类型,
    const 结构_轮廓比较结果& 比较结果,
    特征节点类* 左特征,
    特征节点类* 右特征,
    场景节点类* 来源场景,
    动态节点类* 来源动态)
{
    return 二次特征生成服务_->写入轮廓比较二次特征(
        父节点,
        特征类型,
        比较结果,
        左特征,
        右特征,
        来源场景,
        来源动态);
}

// 功能：创建并返回或登记对应对象。
状态节点类* 世界树类::创建状态(场景节点类* 场景, 状态节点主信息类* 主信息)
{
    auto* 节点 = 状态服务_.创建状态(场景, 主信息);
    同步SQL控制面板世界显示项(节点, "创建状态");
    return 节点;
}

// 功能：创建并返回或登记对应对象。
状态节点类* 世界树类::创建状态(
    场景节点类* 场景,
    基础信息节点类* 主体,
    特征节点类* 特征,
    const 特征值& 状态值,
    时间戳 发生时间,
    时间戳 收到时间)
{
    auto* 节点 = 状态服务_.创建状态(场景, 主体, 特征, 状态值, 发生时间, 收到时间);
    同步SQL控制面板世界显示项(节点, "创建状态");
    return 节点;
}

// 功能：创建内部状态节点并登记到场景。
状态节点类* 世界树类::创建内部状态(
    场景节点类* 场景,
    基础信息节点类* 主体,
    特征节点类* 特征,
    const 特征值& 值,
    bool 是否变化,
    时间戳 now,
    const 状态类::状态后处理回调& 后处理)
{
    auto* 节点 = 状态服务_.创建内部状态(场景, 主体, 特征, 值, 是否变化, now, 后处理);
    同步SQL控制面板世界显示项(节点, "创建内部状态");
    return 节点;
}

// 功能：读取场景状态集合，不创建或修改节点。
std::vector<状态节点类*> 世界树类::获取场景状态(const 场景节点类* 场景) const
{
    return 状态服务_.获取场景状态(场景);
}

// 功能：判断指定状态是否仍是场景中同主体同特征口径的当前活动状态。
bool 世界树类::是否当前活动状态(const 场景节点类* 场景, const 状态节点类* 节点) const
{
    return 状态服务_.是否当前活动状态(场景, 节点);
}

// 功能：读取状态清理最近问题快照，不修改状态索引。
结构_状态清理问题快照 世界树类::读取最近清理问题快照() const
{
    return 状态服务_.读取最近清理问题快照();
}

// 功能：清除状态清理最近问题快照。
void 世界树类::清除最近清理问题快照() noexcept
{
    状态服务_.清除最近清理问题快照();
}

// 功能：绑定状态节点的主体和特征引用。
bool 世界树类::绑定状态主体特征(状态节点类* 节点, 基础信息节点类* 主体, 特征节点类* 特征)
{
    const bool 成功 = 状态服务_.绑定主体特征(节点, 主体, 特征);
    if (成功) {
        同步SQL控制面板世界显示项(节点, "绑定状态主体特征");
    }
    return 成功;
}

// 功能：写入状态节点的状态值和可选时间戳。
bool 世界树类::写入状态值(状态节点类* 节点, const 特征值& 值, 时间戳 发生时间, 时间戳 收到时间)
{
    const bool 成功 = 状态服务_.写入状态值(节点, 值, 发生时间, 收到时间);
    if (成功) {
        同步SQL控制面板世界显示项(节点, "写入状态值");
    }
    return 成功;
}

// 功能：读取状态节点主信息，不创建或修改节点。
状态节点主信息类* 世界树类::取状态主信息(状态节点类* 节点) const noexcept
{
    return 状态服务_.取状态主信息(节点);
}

// 功能：读取状态节点主信息，不创建或修改节点。
const 状态节点主信息类* 世界树类::取状态主信息(const 状态节点类* 节点) const noexcept
{
    return 状态服务_.取状态主信息(节点);
}

// 功能：仅写入状态节点显示缓存；权威变化关系由动态或二次特征比较生成。
bool 世界树类::标记状态变化(状态节点类* 节点, bool 是否变化)
{
    const bool 成功 = 状态服务_.标记变化(节点, 是否变化);
    if (成功) {
        同步SQL控制面板世界显示项(节点, "标记状态变化");
    }
    return 成功;
}

// 功能：创建并返回或登记对应对象。
动态节点类* 世界树类::创建动态(场景节点类* 场景, 动态节点主信息类* 主信息)
{
    return 动态服务_.创建动态(场景, 主信息);
}

// 功能：创建并返回或登记对应对象。
动态节点类* 世界树类::创建动态(
    场景节点类* 场景,
    动态节点主信息类* 主信息,
    const 动态创建后处理选项& 后处理选项)
{
    return 动态服务_.创建动态(场景, 主信息, 后处理选项);
}

// 功能：创建并返回或登记对应对象。
动态节点类* 世界树类::创建动态(
    场景节点类* 场景,
    状态节点类* 初始状态,
    状态节点类* 结果状态,
    时间戳 起,
    时间戳 止)
{
    return 动态服务_.创建动态(场景, 初始状态, 结果状态, 起, 止);
}

// 功能：创建方法动作动态并登记到场景。
动态节点类* 世界树类::创建方法动作动态(
    场景节点类* 场景,
    基础信息节点类* 动态主体,
    特征节点类* 动态特征,
    方法节点类* 来源动作,
    const 语素入口节点类* 来源动作名,
    时间戳 开始时间,
    时间戳 结束时间,
    bool 执行成功,
    I64 错误码,
    场景节点类* 输入场景,
    场景节点类* 输出场景,
    状态节点类* 初始状态,
    状态节点类* 结果状态)
{
    return 动态服务_.创建方法动作动态(
        场景,
        动态主体,
        动态特征,
        来源动作,
        来源动作名,
        开始时间,
        结束时间,
        执行成功,
        错误码,
        输入场景,
        输出场景,
        初始状态,
        结果状态);
}

// 功能：向动态来源链追加低层动态引用。
bool 世界树类::追加来源低层动态(动态节点类* 节点, 动态节点类* 来源低层动态)
{
    return 动态服务_.追加来源低层动态(节点, 来源低层动态);
}

// 功能：读取动态节点主信息，不创建或修改节点。
动态节点主信息类* 世界树类::取动态主信息(动态节点类* 节点) const noexcept
{
    return 动态服务_.取动态主信息(节点);
}

// 功能：读取动态节点主信息，不创建或修改节点。
const 动态节点主信息类* 世界树类::取动态主信息(const 动态节点类* 节点) const noexcept
{
    return 动态服务_.取动态主信息(节点);
}

// 功能：读取动态两个状态构成的时间段，不创建或修改节点。
bool 世界树类::读取动态双状态时间段(const 动态节点类* 动态, 时间戳& 起, 时间戳& 止) const noexcept
{
    return 动态服务_.读取动态双状态时间段(动态, 起, 止);
}

// 功能：按主体、特征和双状态时间段包含判断两个动态是否可复算同源。
bool 世界树类::动态双状态同源可复算(
    const 动态节点类* 状态迁移动态,
    const 动态节点类* 动作致变动态) const noexcept
{
    return 动态服务_.动态双状态同源可复算(状态迁移动态, 动作致变动态);
}

// 功能：读取动态聚合层级、聚合方式和路径签名，不创建或修改节点。
bool 世界树类::读取动态聚合信息(
    const 动态节点类* 动态,
    std::uint32_t& 动态层级,
    枚举_动态聚合方式& 聚合方式,
    std::uint64_t& 动态路径签名) const noexcept
{
    return 动态服务_.读取动态聚合信息(动态, 动态层级, 聚合方式, 动态路径签名);
}

// 功能：判断动态是否为原子相邻动态，不创建或修改节点。
bool 世界树类::是原子相邻动态(const 动态节点类* 动态) const noexcept
{
    return 动态服务_.是原子相邻动态(动态);
}

// 功能：读取动态来源低层动态引用链，不创建或修改节点。
bool 世界树类::读取动态来源低层动态(
    const 动态节点类* 动态,
    std::vector<动态节点类*>& 输出来源低层动态) const
{
    return 动态服务_.读取动态来源低层动态(动态, 输出来源低层动态);
}

// 功能：按动态节点列表计算聚合路径签名，用于来源链快速过滤。
std::uint64_t 世界树类::计算动态列表路径签名(const std::vector<动态节点类*>& 动态列表) const noexcept
{
    return 动态服务_.计算动态列表路径签名(动态列表);
}

// 功能：比较聚合动态的来源低层动态链是否与输入动态列表一致。
bool 世界树类::动态聚合来源链相同(
    const 动态节点类* 聚合动态,
    const std::vector<动态节点类*>& 来源动态列表) const noexcept
{
    return 动态服务_.动态聚合来源链相同(聚合动态, 来源动态列表);
}

// 功能：读取场景动态事实目录中的动态节点快照。
std::vector<动态节点类*> 世界树类::获取场景动态(const 场景节点类* 场景) const
{
    return 动态服务_.获取场景动态(场景);
}

// 功能：从场景动态事实目录筛出位置变化原子动态候选，不创建或修改节点。
std::vector<动态节点类*> 世界树类::读取位置变化原子动态候选(const 场景节点类* 场景) const
{
    return 动态服务_.读取位置变化原子动态候选(场景);
}

// 功能：按时间连续性把位置变化原子动态候选归为同主体只读片段组，不创建或修改节点。
std::vector<std::vector<动态节点类*>> 世界树类::读取位置变化主体动态片段组(
    const 场景节点类* 场景,
    时间戳 最大时间缺口) const
{
    return 动态服务_.读取位置变化主体动态片段组(场景, 最大时间缺口);
}

// 功能：从位置变化片段组提取运行期运动基元候选，不创建稳定规则或方法候选。
std::vector<结构_位置运动基元候选> 世界树类::读取位置变化运动基元候选(
    const 场景节点类* 场景,
    时间戳 最大时间缺口,
    I64 停止最大变化量,
    时间戳 停止最小持续时间) const
{
    return 动态服务_.读取位置变化运动基元候选(
        场景,
        最大时间缺口,
        停止最大变化量,
        停止最小持续时间);
}

// 功能：把已入账观察关系二次特征按二主体、关系类型和时间连续性归为只读片段组。
std::vector<结构_关系动态片段组> 世界树类::读取观察关系动态片段组(
    const 场景节点类* 场景,
    时间戳 最大时间缺口) const
{
    return 动态服务_.读取观察关系动态片段组(场景, 最大时间缺口);
}

// 功能：从观察关系片段组提取运行期关系运动基元候选，不创建稳定规则或方法候选。
std::vector<结构_关系运动基元候选> 世界树类::读取观察关系运动基元候选(
    const 场景节点类* 场景,
    时间戳 最大时间缺口) const
{
    return 动态服务_.读取观察关系运动基元候选(场景, 最大时间缺口);
}

// 功能：记录状态后刷新或创建相邻动态。
动态节点类* 世界树类::状态记录后刷新动态(
    场景节点类* 场景,
    状态节点类* 新状态,
    时间戳 now)
{
    return 动态服务_.状态记录后刷新动态(场景, 新状态, now);
}

// 功能：设置动态主体和特征。
bool 世界树类::设置主体特征(动态节点类* 节点, 基础信息节点类* 主体, 特征节点类* 特征)
{
    return 动态服务_.设置主体特征(节点, 主体, 特征);
}

// 功能：绑定动态来源方法。
bool 世界树类::绑定来源方法(
    动态节点类* 节点,
    方法节点类* 来源动作,
    const 语素入口节点类* 来源动作名)
{
    return 动态服务_.绑定来源方法(节点, 来源动作, 来源动作名);
}

// 功能：绑定动态来源输入和输出场景。
bool 世界树类::绑定来源场景(动态节点类* 节点, 场景节点类* 输入场景, 场景节点类* 输出场景)
{
    return 动态服务_.绑定来源场景(节点, 输入场景, 输出场景);
}

// 功能：按动态信息生成或命中因果信息。
因果模板节点类* 世界树类::生成因果信息(
    场景节点类* 场景,
    动态节点类* 动态节点,
    基础信息节点类* 动作主体,
    基础信息节点类* 动作语义,
    const std::string& 动作语义键,
    bool 设为主结果)
{
    return 因果服务_.生成因果信息(
        场景,
        动态节点,
        动作主体,
        动作语义,
        动作语义键,
        设为主结果);
}

// 功能：读取因果节点主信息，不创建或修改节点。
因果主信息类* 世界树类::取因果主信息(因果节点类* 节点) const noexcept
{
    return 因果服务_.取因果主信息(节点);
}

// 功能：读取因果节点主信息，不创建或修改节点。
const 因果主信息类* 世界树类::取因果主信息(const 因果节点类* 节点) const noexcept
{
    return 因果服务_.取因果主信息(节点);
}

// 功能：读取因果模板节点主信息，不创建或修改节点。
因果模板主信息类* 世界树类::取因果模板主信息(因果模板节点类* 节点) const noexcept
{
    return 因果服务_.取模板主信息(节点);
}

// 功能：读取因果模板节点主信息，不创建或修改节点。
const 因果模板主信息类* 世界树类::取因果模板主信息(const 因果模板节点类* 节点) const noexcept
{
    return 因果服务_.取模板主信息(节点);
}

// 功能：把基础信息节点解析为因果节点，不创建或修改节点。
因果节点类* 世界树类::解析因果节点(基础信息节点类* 节点) const noexcept
{
    return 因果服务_.解析因果节点(节点);
}

// 功能：把基础信息节点解析为因果节点，不创建或修改节点。
const 因果节点类* 世界树类::解析因果节点(const 基础信息节点类* 节点) const noexcept
{
    return 因果服务_.解析因果节点(节点);
}

// 功能：按主键解析因果节点，不创建或修改节点。
因果节点类* 世界树类::按主键解析因果节点(const std::string& 主键) noexcept
{
    return 因果服务_.按主键解析因果节点(主键);
}

// 功能：按主键解析因果节点，不创建或修改节点。
const 因果节点类* 世界树类::按主键解析因果节点(const std::string& 主键) const noexcept
{
    return 因果服务_.按主键解析因果节点(主键);
}

// 功能：判断基础信息节点是否承载因果主信息，不创建或修改节点。
bool 世界树类::是因果节点(const 基础信息节点类* 节点) const noexcept
{
    return 因果服务_.是因果节点(节点);
}

// 功能：把因果模板转换为基础命名显示使用的自然语言。
std::string 世界树类::生成抽象因果自然语言(const 因果模板节点类* 模板) const
{
    return 因果服务_.生成抽象因果自然语言(模板);
}

// 功能：按二次特征条件和结果候选查找匹配的因果模板。
std::vector<因果模板节点类*> 世界树类::查找匹配因果模板(
    枚举_因果锚点类型 锚点类型,
    std::uint32_t 状态层级,
    const std::vector<二次特征节点类*>& 条件候选,
    const std::vector<二次特征节点类*>& 结果候选) const
{
    return 因果服务_.查找匹配因果模板(锚点类型, 状态层级, 条件候选, 结果候选);
}

// 功能：读取因果动作来源的方法模板和动作语义，不创建或修改节点。
void 世界树类::查询因果动作来源(
    const 因果节点类* 因果,
    方法节点类*& 因方法,
    bool& 有方法模板,
    bool& 有动作语义,
    std::string& 动作语义键) const
{
    因果服务_.查询因果动作来源(因果, 因方法, 有方法模板, 有动作语义, 动作语义键);
}

// 功能：读取因果证据状态和基础计数判断，不创建或修改节点。
void 世界树类::读取因果证据状态(
    const 因果节点类* 因果,
    bool& 是初始模板,
    bool& 已验证,
    std::uint64_t& 证据动态样本数,
    I64& 稳定度,
    bool& 证据足够) const noexcept
{
    因果服务_.读取因果证据状态(
        因果,
        是初始模板,
        已验证,
        证据动态样本数,
        稳定度,
        证据足够);
}

// 功能：判断因果节点是否登记了条件模板，不创建或修改节点。
bool 世界树类::存在因果条件模板(const 因果节点类* 因果) const noexcept
{
    return 因果服务_.存在因果条件模板(因果);
}

// 功能：从因果条件模板和证据动态初始状态枚举条件目标状态，不创建或修改节点。
void 世界树类::枚举因果条件目标状态(
    const 因果节点类* 因果,
    std::vector<状态节点类*>& 条件目标状态集,
    std::vector<结构_因果缺失证据项>& 缺失证据) const
{
    因果服务_.枚举因果条件目标状态(因果, 条件目标状态集, 缺失证据);
}

// 功能：按目标二次特征比较查询主果同向和反向命中的因果节点。
void 世界树类::查询主果方向命中因果(
    const 二次特征节点类* 目标比较,
    bool 包含未验证路径,
    std::size_t& 因果节点数量,
    std::size_t& 主果可解析数量,
    std::vector<因果节点类*>& 同向命中因果,
    std::vector<因果节点类*>& 反向命中因果,
    std::vector<结构_因果缺失证据项>& 缺失证据) const
{
    因果服务_.查询主果方向命中因果(
        目标比较,
        包含未验证路径,
        因果节点数量,
        主果可解析数量,
        同向命中因果,
        反向命中因果,
        缺失证据);
}

// 功能：确保被动触发初始因果模板存在，按结构签名命中后追加证据动态。
因果模板节点类* 世界树类::确保被动触发初始因果模板(
    基础信息节点类* 父节点,
    场景节点类* 适用场景,
    二次特征节点类* 条件比较,
    二次特征节点类* 主果比较,
    动态节点类* 证据动态,
    std::uint32_t 适用层级,
    const std::string& 动作语义键,
    bool& 已创建)
{
    return 因果服务_.确保被动触发初始因果模板(
        父节点,
        适用场景,
        条件比较,
        主果比较,
        证据动态,
        适用层级,
        动作语义键,
        已创建);
}

// 功能：从目标结果状态或目标结果特征回溯现有状态转换因果边。
void 世界树类::查询因果链(
    状态节点类* 目标结果状态,
    特征节点类* 目标结果特征,
    std::uint32_t 最大深度,
    bool 包含未验证路径,
    std::size_t& 边数量,
    std::vector<因果模板节点类*>& 来源因果模板,
    std::vector<结构_因果缺失证据项>& 缺失证据) const
{
    因果服务_.查询因果链(
        目标结果状态,
        目标结果特征,
        最大深度,
        包含未验证路径,
        边数量,
        来源因果模板,
        缺失证据);
}

// 功能：查询叶子任务目标可能落入的结算贡献投影层级。
void 世界树类::查询叶子任务目标投影层级(
    任务节点类* 叶子任务,
    需求节点类* 来源需求,
    基础信息节点类* 目标宿主,
    特征节点类* 目标特征类型,
    bool 允许未验证路径,
    std::size_t& 候选投影数量,
    std::vector<std::uint32_t>& 结算贡献层级集,
    std::vector<std::uint32_t>& 因果距离层级集,
    std::vector<结构_因果缺失证据项>& 缺失证据,
    bool& 是否允许形成D0) const
{
    因果服务_.查询叶子任务目标投影层级(
        叶子任务,
        来源需求,
        目标宿主,
        目标特征类型,
        允许未验证路径,
        候选投影数量,
        结算贡献层级集,
        因果距离层级集,
        缺失证据,
        是否允许形成D0);
}

// 功能：查询自检原子目标的状态转换因果投影、来源因果和缺失证据。
void 世界树类::查询自检原子目标投影(
    任务节点类* 固定来源任务,
    需求节点类* 固定来源需求,
    基础信息节点类* 目标宿主,
    特征节点类* 目标特征类型,
    const 语素入口节点类* 目标特征类型词,
    bool 允许未验证路径,
    std::size_t& 候选投影数量,
    std::vector<std::string>& 来源因果主键集,
    std::vector<结构_因果缺失证据项>& 缺失证据) const
{
    因果服务_.查询自检原子目标投影(
        固定来源任务,
        固定来源需求,
        目标宿主,
        目标特征类型,
        目标特征类型词,
        允许未验证路径,
        候选投影数量,
        来源因果主键集,
        缺失证据);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入特征_I64(特征节点类* 节点, I64 值, 时间戳 now)
{
    return 特征服务_.写入特征值_I64(节点, 值, now);
}

// 功能：把通用特征值写入指定特征节点。
bool 世界树类::写入特征值(特征节点类* 节点, const 特征值& 值, 时间戳 now)
{
    return 特征服务_.写入特征值(节点, 值, now);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入特征_I64(基础信息节点类* 宿主, const 语素入口节点类* 特征类型, I64 值, 时间戳 now)
{
    auto* 特征节点 = 确保特征(宿主, 特征类型);
    return 特征节点 ? 写入特征_I64(特征节点, 值, now) : false;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入特征_指针(特征节点类* 节点, const void* 指针, 时间戳 now)
{
    指针句柄 句柄{};
    句柄.指针 = reinterpret_cast<std::uintptr_t>(指针);
    return 特征服务_.写入特征值_指针句柄(节点, 句柄, now);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入特征_指针(
    基础信息节点类* 宿主,
    const 语素入口节点类* 特征类型,
    const void* 指针,
    时间戳 now)
{
    auto* 特征节点 = 确保特征(宿主, 特征类型);
    return 特征节点 ? 写入特征_指针(特征节点, 指针, now) : false;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入特征_VecU(
    特征节点类* 节点,
    VecIU64 值,
    枚举_轮廓维度 维度,
    时间戳 now)
{
    const auto 句柄 = 值池_.获取或创建句柄(std::move(值), 维度);
    return 特征服务_.写入特征值_VecU句柄(节点, 句柄, now);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入特征_VecU(
    基础信息节点类* 宿主,
    const 语素入口节点类* 特征类型,
    VecIU64 值,
    枚举_轮廓维度 维度,
    时间戳 now)
{
    auto* 特征节点 = 确保特征(宿主, 特征类型);
    return 特征节点 ? 写入特征_VecU(特征节点, std::move(值), 维度, now) : false;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入特征_VecI64(
    特征节点类* 节点,
    const VecI64& 值,
    枚举_轮廓维度 维度,
    时间戳 now)
{
    return 写入特征_VecU(节点, 编码VecI64为VecIU64(值), 维度, now);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入特征_VecI64(
    基础信息节点类* 宿主,
    const 语素入口节点类* 特征类型,
    const VecI64& 值,
    枚举_轮廓维度 维度,
    时间戳 now)
{
    auto* 特征节点 = 确保特征(宿主, 特征类型);
    return 特征节点 ? 写入特征_VecI64(特征节点, 值, 维度, now) : false;
}

// 功能：从指定来源读取数据或状态。
特征值 世界树类::读取特征快照(const 特征节点类* 节点) const
{
    return 特征服务_.读取特征值(节点);
}

// 功能：从指定来源读取数据或状态。
特征值 世界树类::读取特征快照(const 基础信息节点类* 宿主, const 语素入口节点类* 特征类型) const
{
    return 特征服务_.读取子特征值_按类型(宿主, 特征类型);
}

// 功能：从指定来源读取数据或状态。
bool 世界树类::读取特征_I64(const 特征节点类* 节点, I64& 输出值) const
{
    const auto 值 = 读取特征快照(节点);
    if (const auto* 标量 = std::get_if<I64>(&值)) {
        输出值 = *标量;
        return true;
    }
    return false;
}

// 功能：从指定来源读取数据或状态。
bool 世界树类::读取特征_I64(const 基础信息节点类* 宿主, const 语素入口节点类* 特征类型, I64& 输出值) const
{
    const auto 值 = 读取特征快照(宿主, 特征类型);
    if (const auto* 标量 = std::get_if<I64>(&值)) {
        输出值 = *标量;
        return true;
    }
    return false;
}

// 功能：从指定来源读取数据或状态。
bool 世界树类::读取特征_指针(const 特征节点类* 节点, void*& 输出指针) const
{
    输出指针 = nullptr;
    const auto 值 = 读取特征快照(节点);
    if (const auto* 句柄 = std::get_if<指针句柄>(&值)) {
        输出指针 = reinterpret_cast<void*>(句柄->指针);
        return 句柄->有效();
    }
    return false;
}

// 功能：从指定来源读取数据或状态。
bool 世界树类::读取特征_指针(
    const 基础信息节点类* 宿主,
    const 语素入口节点类* 特征类型,
    void*& 输出指针) const
{
    输出指针 = nullptr;
    const auto 值 = 读取特征快照(宿主, 特征类型);
    if (const auto* 句柄 = std::get_if<指针句柄>(&值)) {
        输出指针 = reinterpret_cast<void*>(句柄->指针);
        return 句柄->有效();
    }
    return false;
}

// 功能：从指定来源读取数据或状态。
const VecIU64* 世界树类::读取特征VecU(const 特征节点类* 节点) const
{
    const auto 值 = 读取特征快照(节点);
    if (const auto* 句柄 = std::get_if<VecU句柄>(&值)) {
        return 值池_.取VecU只读指针(*句柄);
    }
    return nullptr;
}

// 功能：从指定来源读取数据或状态。
bool 世界树类::读取特征VecI64(const 特征节点类* 节点, VecI64& 输出值) const
{
    输出值.clear();
    const auto* 原始值 = 读取特征VecU(节点);
    if (!原始值) return false;
    输出值 = 解码VecIU64为VecI64(*原始值);
    return true;
}

// 功能：从指定来源读取数据或状态。
bool 世界树类::读取特征VecI64(
    const 基础信息节点类* 宿主,
    const 语素入口节点类* 特征类型,
    VecI64& 输出值) const
{
    输出值.clear();
    const auto 值 = 读取特征快照(宿主, 特征类型);
    const auto* 句柄 = std::get_if<VecU句柄>(&值);
    if (!句柄) return false;
    const auto* 原始值 = 值池_.取VecU只读指针(*句柄);
    if (!原始值) return false;
    输出值 = 解码VecIU64为VecI64(*原始值);
    return true;
}

// 功能：比较当前值、目标值或历史基准。
结构_轮廓比较结果 世界树类::比较轮廓特征_按特征类型(
    const 语素入口节点类* 特征类型,
    const 特征节点类* 左特征,
    const 特征节点类* 右特征) const
{
    if (!特征类::轮廓坐标维度_按特征类型(特征类型).has_value()) {
        return 特征类::比较轮廓VecI64_按特征类型(特征类型, {}, {});
    }

    VecI64 左值{};
    VecI64 右值{};
    if (!读取特征VecI64(左特征, 左值) || !读取特征VecI64(右特征, 右值)) {
        结构_轮廓比较结果 结果{};
        结果.状态 = 枚举_轮廓比较状态::格式非法;
        return 结果;
    }

    return 特征类::比较轮廓VecI64_按特征类型(特征类型, 左值, 右值);
}

// 功能：比较当前值、目标值或历史基准。
存在比较结果 世界树类::比较存在(
    const 存在节点类* 左存在,
    const 存在节点类* 右存在,
    const 集合比较参数& 参数) const
{
    if (!左存在 || !右存在) return {};
    const auto 左特征集合 = 特征服务_.获取子特征(左存在);
    const auto 右特征集合 = 特征服务_.获取子特征(右存在);
    return 特征服务_.比较特征集合(左特征集合, 右特征集合, 参数);
}

// 功能：比较当前值、目标值或历史基准。
std::vector<二次特征节点类*> 世界树类::刷新存在比较二次特征(
    存在节点类* 左存在,
    存在节点类* 右存在,
    const 集合比较参数& 参数)
{
    if (!左存在 || !右存在 || 左存在 == 右存在) return {};

    auto* 左场景 = 私有_取存在所在场景(基础信息链_, 左存在);
    auto* 右场景 = 私有_取存在所在场景(基础信息链_, 右存在);
    if (!左场景 && !右场景) return {};
    if (左场景 && 右场景 && 左场景 != 右场景) return {};

    auto* 所属场景 = 左场景 ? 左场景 : 右场景;
    const auto 比较结果 = 比较存在(左存在, 右存在, 参数);
    return 二次特征生成服务_->由存在比较结果生成二次特征(
        所属场景,
        左存在,
        右存在,
        比较结果);
}

// 功能：创建并返回或登记对应对象。
场景节点类* 世界树类::取或创建默认世界(const std::string& 名称, 枚举_世界类型 世界类型)
{
    const auto* 名称词 = 语素集.添加信息入口词(名称, 枚举_信息入口类型::场景实例入口);
    return 取或创建子场景_按名称(nullptr, 名称词, 世界类型);
}

// 功能：创建并返回或登记对应对象。
场景节点类* 世界树类::取或创建自我现实场景()
{
    if (!现实世界) 初始化默认世界();

    const auto* 场景名称 = 语素集.添加信息入口词("自我初始场景", 枚举_信息入口类型::场景实例入口);
    自我所在场景 = 取或创建子场景_按名称(现实世界, 场景名称, 枚举_世界类型::现实世界);
    return 自我所在场景;
}

// 功能：创建并返回或登记对应对象。
存在节点类* 世界树类::取或创建自我存在()
{
    auto* 当前场景 = 自我所在场景 ? 自我所在场景 : 取或创建自我现实场景();
    const auto* 名称词 = 语素集.添加信息入口词("自我", 枚举_信息入口类型::存在实例入口);
    const auto* 类型词 = 语素集.添加信息入口词("自我", 枚举_信息入口类型::存在概念入口);
    auto* 已有自我 = 存在服务_.查找子存在_按名称(当前场景, 名称词);
    const bool 已存在 = 已有自我 != nullptr;
    自我指针 = 已有自我 ? 已有自我 : 存在服务_.取或创建子存在_按名称类型(当前场景, 名称词, 类型词);
    const bool 类型需纠偏 = 自我指针 && 类型词 && !语素入口同一(存在服务_.读取存在类型(自我指针), 类型词);
    if (类型需纠偏) {
        (void)存在服务_.写入类型(自我指针, 类型词);
    }
    if (自我指针 && (!已存在 || 类型需纠偏)) {
        同步SQL控制面板世界显示项(自我指针, 类型需纠偏 ? "取或创建自我存在/类型纠偏" : "取或创建自我存在/新建");
    }
    Vector3D 已有坐标{};
    if (自我指针 && !读取存在场景绝对坐标(自我指针, 已有坐标)) {
        const auto now = 结构体_时间戳::当前_微秒();
        if (写入存在场景绝对坐标(自我指针, Vector3D{ 0.0, 0.0, 0.0 }, now, 10000)) {
            (void)写入特征_指针(
                reinterpret_cast<基础信息节点类*>(自我指针),
                私有_特征_存在坐标来源(),
                当前场景,
                now);
        }
    }
    if (自我指针) {
        (void)私有_确保自我最小边界体素模型(*this, 特征服务_, 自我指针, 结构体_时间戳::当前_微秒());
    }
    return 自我指针;
}

// 功能：按函数名执行对应处理。
场景节点类* 世界树类::取存在内部世界(const 存在节点类* 宿主存在) const noexcept
{
    return 存在服务_.读取内部世界(宿主存在);
}

// 功能：创建并返回或登记对应对象。
场景节点类* 世界树类::取或创建存在内部世界(存在节点类* 宿主存在, const 语素入口节点类* 场景名称)
{
    if (!宿主存在) return nullptr;

    if (auto* 现有 = 取存在内部世界(宿主存在)) {
        return 现有;
    }

    const auto* 名称词 = 场景名称 ? 场景名称 : 语素集.添加信息入口词("内部世界", 枚举_信息入口类型::场景实例入口);
    auto* 内部世界 = 取或创建子场景_按名称(宿主存在, 名称词, 枚举_世界类型::内部世界);
    if (!内部世界) return nullptr;

    场景服务_.绑定宿主(内部世界, 宿主存在);
    存在服务_.绑定内部世界(宿主存在, 内部世界);
    return 内部世界;
}

// 功能：从指定来源读取数据或状态。
bool 世界树类::读取存在场景绝对坐标(const 存在节点类* 节点, Vector3D& 输出坐标_mm) const
{
    return 存在服务_.读取存在场景绝对坐标(节点, 输出坐标_mm);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入存在场景绝对坐标(
    存在节点类* 节点,
    const Vector3D& 坐标_mm,
    时间戳 now,
    I64 置信度)
{
    if (!存在服务_.写入存在场景绝对坐标(节点, 坐标_mm, now, 置信度)) {
        return false;
    }

    if (auto* 所属场景 = 私有_取存在所在场景(基础信息链_, 节点)) {
        (void)二次特征生成服务_->刷新场景存在型基础二次特征(所属场景);
    }
    return true;
}

// 功能：计算权重、状态、差值或派生结果。
bool 世界树类::计算场景绝对坐标_由参考存在相对坐标(
    const 存在节点类* 参考存在,
    const Vector3D& 相对坐标_mm,
    Vector3D& 输出绝对坐标_mm) const
{
    return 存在服务_.计算场景绝对坐标_由参考存在相对坐标(
        参考存在,
        相对坐标_mm,
        输出绝对坐标_mm);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入存在场景绝对坐标_由参考存在相对坐标(
    存在节点类* 目标存在,
    const 存在节点类* 参考存在,
    const Vector3D& 相对坐标_mm,
    时间戳 now,
    I64 置信度)
{
    if (!存在服务_.写入存在场景绝对坐标_由参考存在相对坐标(
        目标存在,
        参考存在,
        相对坐标_mm,
        now,
        置信度)) {
        return false;
    }

    if (auto* 目标场景 = 私有_取存在所在场景(基础信息链_, 目标存在)) {
        (void)二次特征生成服务_->刷新场景存在型基础二次特征(目标场景);
    }
    return true;
}

// 功能：计算权重、状态、差值或派生结果。
结构_存在空间绝对坐标换算结果 世界树类::计算场景绝对坐标_由相对坐标表(
    const std::vector<结构_存在空间相对坐标项>& 相对坐标表) const
{
    return 存在服务_.计算场景绝对坐标_由相对坐标表(相对坐标表);
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入存在场景绝对坐标_由相对坐标表(
    存在节点类* 目标存在,
    const std::vector<结构_存在空间相对坐标项>& 相对坐标表,
    时间戳 now)
{
    if (!存在服务_.写入存在场景绝对坐标_由相对坐标表(目标存在, 相对坐标表, now)) {
        return false;
    }

    if (auto* 目标场景 = 私有_取存在所在场景(基础信息链_, 目标存在)) {
        (void)二次特征生成服务_->刷新场景存在型基础二次特征(目标场景);
    }
    return true;
}

// 功能：把处理结果写入指定对象、场景或日志。
bool 世界树类::写入存在观测位置(存在节点类* 节点, const Vector3D& 位置_mm)
{
    if (!节点) return false;
    if (!存在服务_.写入观测位置(节点, 位置_mm)) {
        return false;
    }

    auto* 父节点 = static_cast<基础信息节点类*>(节点->父);
    auto* 场景节点 = 父节点 && 基础信息链_.取主信息<场景节点主信息类>(父节点)
        ? static_cast<场景节点类*>(父节点)
        : nullptr;
    if (场景节点) {
        (void)二次特征生成服务_->刷新场景存在型基础二次特征(场景节点);
    }
    return true;
}

// 功能：按函数名执行对应处理。
I64 世界树类::默认特征差异度(const 特征值& 左值, const 特征值& 右值) const
{
    if (左值.index() != 右值.index()) return 10000;

    if (const auto* 左标量 = std::get_if<I64>(&左值)) {
        const auto* 右标量 = std::get_if<I64>(&右值);
        return 右标量 ? 饱和绝对值I64(饱和减少(*左标量, *右标量)) : 10000;
    }

    if (const auto* 左句柄 = std::get_if<VecU句柄>(&左值)) {
        const auto* 右句柄 = std::get_if<VecU句柄>(&右值);
        if (!右句柄) return 10000;
        const auto* 左Vec = 值池_.取VecU只读指针(*左句柄);
        const auto* 右Vec = 值池_.取VecU只读指针(*右句柄);
        if (!左Vec || !右Vec) return 10000;
        return 私有_VecU差异率Q10000(*左Vec, *右Vec);
    }

    if (const auto* 左指针 = std::get_if<指针句柄>(&左值)) {
        const auto* 右指针 = std::get_if<指针句柄>(&右值);
        return (右指针 && (*左指针 == *右指针)) ? 0 : 10000;
    }

    return 0;
}
