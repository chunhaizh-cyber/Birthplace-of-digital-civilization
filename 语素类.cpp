#include "语素类.h"

// 文件头部规则注释模块：
// 1. 修改本文件前先阅读本模块；语素入口只作为身份入口，机器逻辑不得用词面文本替代项目结构身份。
// 2. SQL 投影和字段恢复比对只用于显示 / 诊断 / 存储验证，不得反向作为语素业务判断来源。
// 3. 本模块只供人读，不参与机器判断；业务状态仍必须由语素节点、主信息和项目通用结构承载。

#include "基础信息类.h"
#include "场景类.h"
#include "日志接入.h"

#include <algorithm>
#include <cstdint>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

import 数据库ADO模块;
import 全局共享函数类;

namespace {

    // 功能：服务所在模块的内部辅助流程。
    const std::unordered_map<std::string, 枚举_词性>& 私有_文本到枚举映射表()
    {
        static const std::unordered_map<std::string, 枚举_词性> 映射表 = {
            { "名词", 枚举_词性::名词 },
            { "动词", 枚举_词性::动词 },
            { "形容词", 枚举_词性::形容词 },
            { "副词", 枚举_词性::副词 },
            { "连词", 枚举_词性::连词 },
            { "介词", 枚举_词性::介词 },
            { "专有名词", 枚举_词性::专有名词 },
            { "抽象名词", 枚举_词性::抽象名词 },
            { "代词", 枚举_词性::代词 },
            { "数词", 枚举_词性::数词 },
            { "量词", 枚举_词性::量词 },
            { "助词", 枚举_词性::助词 },
            { "叹词", 枚举_词性::叹词 },
            { "拟声词", 枚举_词性::拟声词 },
            { "标点符号", 枚举_词性::标点符号 },
            { "时间词", 枚举_词性::时间词 },
            { "状态词", 枚举_词性::状态词 },
            { "方位词", 枚举_词性::方位词 },
            { "习用语", 枚举_词性::习用语 },
            { "简称略语", 枚举_词性::简称略语 },
            { "非语素字", 枚举_词性::非语素字 },
            { "语气词", 枚举_词性::语气词 },
            { "外文字符", 枚举_词性::外文字符 },
            { "短语", 枚举_词性::短语 },
            { "未定义", 枚举_词性::未定义 },
            { "地名", 枚举_词性::地名 },
            { "n", 枚举_词性::n },
            { "f", 枚举_词性::f },
            { "s", 枚举_词性::s },
            { "t", 枚举_词性::t },
            { "nr", 枚举_词性::nr },
            { "ns", 枚举_词性::ns },
            { "nt", 枚举_词性::nt },
            { "nw", 枚举_词性::nw },
            { "nz", 枚举_词性::nz },
            { "v", 枚举_词性::v },
            { "vd", 枚举_词性::vd },
            { "vn", 枚举_词性::vn },
            { "a", 枚举_词性::a },
            { "ad", 枚举_词性::ad },
            { "an", 枚举_词性::an },
            { "d", 枚举_词性::d },
            { "m", 枚举_词性::m },
            { "q", 枚举_词性::q },
            { "r", 枚举_词性::r },
            { "p", 枚举_词性::p },
            { "c", 枚举_词性::c },
            { "u", 枚举_词性::u },
            { "xc", 枚举_词性::xc },
            { "w", 枚举_词性::w },
            { "PER", 枚举_词性::PER },
            { "LOC", 枚举_词性::LOC },
            { "ORG", 枚举_词性::ORG },
            { "TIME", 枚举_词性::TIME },
        };
        return 映射表;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::unordered_map<枚举_词性, std::string>& 私有_枚举到文本动态映射表()
    {
        static std::unordered_map<枚举_词性, std::string> 映射表 = {
            { 枚举_词性::名词, "名词" },
            { 枚举_词性::动词, "动词" },
            { 枚举_词性::形容词, "形容词" },
            { 枚举_词性::副词, "副词" },
            { 枚举_词性::连词, "连词" },
            { 枚举_词性::介词, "介词" },
            { 枚举_词性::专有名词, "专有名词" },
            { 枚举_词性::抽象名词, "抽象名词" },
            { 枚举_词性::代词, "代词" },
            { 枚举_词性::数词, "数词" },
            { 枚举_词性::量词, "量词" },
            { 枚举_词性::助词, "助词" },
            { 枚举_词性::叹词, "叹词" },
            { 枚举_词性::拟声词, "拟声词" },
            { 枚举_词性::标点符号, "标点符号" },
            { 枚举_词性::时间词, "时间词" },
            { 枚举_词性::状态词, "状态词" },
            { 枚举_词性::方位词, "方位词" },
            { 枚举_词性::习用语, "习用语" },
            { 枚举_词性::简称略语, "简称略语" },
            { 枚举_词性::非语素字, "非语素字" },
            { 枚举_词性::语气词, "语气词" },
            { 枚举_词性::外文字符, "外文字符" },
            { 枚举_词性::短语, "短语" },
            { 枚举_词性::未定义, "未定义" },
            { 枚举_词性::地名, "地名" },
            { 枚举_词性::n, "n" },
            { 枚举_词性::f, "f" },
            { 枚举_词性::s, "s" },
            { 枚举_词性::t, "t" },
            { 枚举_词性::nr, "nr" },
            { 枚举_词性::ns, "ns" },
            { 枚举_词性::nt, "nt" },
            { 枚举_词性::nw, "nw" },
            { 枚举_词性::nz, "nz" },
            { 枚举_词性::v, "v" },
            { 枚举_词性::vd, "vd" },
            { 枚举_词性::vn, "vn" },
            { 枚举_词性::a, "a" },
            { 枚举_词性::ad, "ad" },
            { 枚举_词性::an, "an" },
            { 枚举_词性::d, "d" },
            { 枚举_词性::m, "m" },
            { 枚举_词性::q, "q" },
            { 枚举_词性::r, "r" },
            { 枚举_词性::p, "p" },
            { 枚举_词性::c, "c" },
            { 枚举_词性::u, "u" },
            { 枚举_词性::xc, "xc" },
            { 枚举_词性::w, "w" },
            { 枚举_词性::PER, "PER" },
            { 枚举_词性::LOC, "LOC" },
            { 枚举_词性::ORG, "ORG" },
            { 枚举_词性::TIME, "TIME" },
        };
        return 映射表;
    }

    // 功能：服务所在模块的内部辅助流程。
    枚举_主信息类型 私有_取基础信息类型(const 基础信息节点类* 节点) noexcept
    {
        return (节点 && 节点->主信息)
            ? 节点->主信息->主信息类型
            : 枚举_主信息类型::未定义;
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_主信息类型文本(const 枚举_主信息类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_主信息类型::特征: return "特征";
        case 枚举_主信息类型::抽象特征: return "抽象特征";
        case 枚举_主信息类型::存在: return "存在";
        case 枚举_主信息类型::场景: return "场景";
        case 枚举_主信息类型::状态: return "状态";
        case 枚举_主信息类型::动态: return "动态";
        case 枚举_主信息类型::因果: return "因果";
        case 枚举_主信息类型::二次特征_修饰: return "二次特征";
        case 枚举_主信息类型::关系: return "关系";
        case 枚举_主信息类型::关系信息: return "关系信息";
        case 枚举_主信息类型::指代: return "指代";
        case 枚举_主信息类型::语言信息_仅记录: return "语言信息";
        case 枚举_主信息类型::未定义: return "未定义";
        default: return "基础信息";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    const char* 私有_信息入口类型文本(const 枚举_信息入口类型 类型) noexcept
    {
        switch (类型) {
        case 枚举_信息入口类型::存在概念入口: return "存在概念入口";
        case 枚举_信息入口类型::特征模板入口: return "特征模板入口";
        case 枚举_信息入口类型::状态模板入口: return "状态模板入口";
        case 枚举_信息入口类型::动态模板入口: return "动态模板入口";
        case 枚举_信息入口类型::关系模板入口: return "关系模板入口";
        case 枚举_信息入口类型::因果模板入口: return "因果模板入口";
        case 枚举_信息入口类型::场景模板入口: return "场景模板入口";
        case 枚举_信息入口类型::存在实例入口: return "存在实例入口";
        case 枚举_信息入口类型::特征实例入口: return "特征实例入口";
        case 枚举_信息入口类型::状态实例入口: return "状态实例入口";
        case 枚举_信息入口类型::动态实例入口: return "动态实例入口";
        case 枚举_信息入口类型::关系实例入口: return "关系实例入口";
        case 枚举_信息入口类型::场景实例入口: return "场景实例入口";
        case 枚举_信息入口类型::需求概念入口: return "需求概念入口";
        case 枚举_信息入口类型::任务概念入口: return "任务概念入口";
        case 枚举_信息入口类型::方法概念入口: return "方法概念入口";
        case 枚举_信息入口类型::需求节点入口: return "需求节点入口";
        case 枚举_信息入口类型::任务信息节点入口: return "任务信息节点入口";
        case 枚举_信息入口类型::任务虚拟存在入口: return "任务虚拟存在入口";
        case 枚举_信息入口类型::方法信息节点入口: return "方法信息节点入口";
        case 枚举_信息入口类型::方法虚拟存在入口: return "方法虚拟存在入口";
        case 枚举_信息入口类型::数值入口: return "数值入口";
        case 枚举_信息入口类型::单位入口: return "单位入口";
        case 枚举_信息入口类型::比较入口: return "比较入口";
        case 枚举_信息入口类型::逻辑连接入口: return "逻辑连接入口";
        case 枚举_信息入口类型::语言记录入口: return "语言记录入口";
        case 枚举_信息入口类型::未定义: return "未定义";
        default: return "信息入口";
        }
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_引用指向(const 可解析引用<基础信息节点类>& 引用, const 基础信息节点类* 节点)
    {
        if (!节点) return false;
        if (引用.指针 == 节点) return true;
        return !引用.主键.empty() && 引用.主键 == 节点->获取主键();
    }

    template<class T节点>
    void 私有_追加唯一引用(std::vector<可解析引用<T节点>>& 列表, T节点* 节点)
    {
        if (!节点) return;
        const std::string 主键 = 节点->获取主键();
        for (const auto& 已有 : 列表) {
            if (已有.指针 == 节点) return;
            if (!主键.empty() && 已有.主键 == 主键) return;
        }
        列表.emplace_back(节点);
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_场景索引包含(
        基础信息类& 基础信息,
        const 场景节点类* 场景,
        const 基础信息节点类* 基础信息节点)
    {
        if (!场景 || !基础信息节点) return false;

        场景类 场景服务(&基础信息);
        const auto 检查列表 = [&](const auto& 列表) {
            for (const auto& 引用 : 列表) {
                if (私有_引用指向(引用, 基础信息节点)) return true;
            }
            return false;
        };

        return 检查列表(场景服务.读取场景状态索引快照(场景))
            || 检查列表(场景服务.读取场景动态索引快照(场景))
            || 检查列表(场景服务.读取场景二次特征索引快照(场景));
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_把基础信息挂入场景索引(
        基础信息类& 基础信息,
        场景节点类* 场景,
        基础信息节点类* 基础信息节点)
    {
        if (!场景 || !基础信息节点 || !基础信息节点->主信息) return;

        场景类 场景服务(&基础信息);
        switch (基础信息节点->主信息->主信息类型) {
        case 枚举_主信息类型::状态:
            (void)场景服务.追加状态(场景, static_cast<状态节点类*>(基础信息节点));
            break;
        case 枚举_主信息类型::动态:
            (void)场景服务.追加动态(场景, static_cast<动态节点类*>(基础信息节点));
            break;
        case 枚举_主信息类型::二次特征_修饰:
        case 枚举_主信息类型::关系:
        case 枚举_主信息类型::关系信息:
            (void)场景服务.追加二次特征(场景, static_cast<二次特征节点类*>(基础信息节点));
            break;
        default:
            break;
        }
    }

    // 功能：读取并返回指定对象、状态或运行材料。
    std::string 私有_获取语素节点显示词(const 语素节点类* 节点)
    {
        if (!节点 || !节点->主信息) return {};
        if (auto* 词 = dynamic_cast<const 词主信息类*>(节点->主信息)) {
            return 词->词;
        }
        if (auto* 词性 = dynamic_cast<const 语素入口主信息类*>(节点->主信息)) {
            if (节点->父 && 节点->父->主信息) {
                if (auto* 父词 = dynamic_cast<const 词主信息类*>(节点->父->主信息)) {
                    return 父词->词;
                }
            }
            return 枚举_词性_工厂::根据枚举类型获取文本(词性->词性);
        }
        return {};
    }

    struct 结构_语素SQL行 {
        int 行号 = 0;
        std::string 节点主键{};
        std::string 父节点主键{};
        int 深度 = 0;
        int 同层序号 = 0;
        int 直接子数量 = 0;
        std::string 路径{};
        std::string 节点类型{};
        std::string 词面{};
        std::string 显示文本{};
        bool 有词性 = false;
        int 词性值 = 0;
        std::string 词性文本{};
        bool 有信息入口类型 = false;
        int 信息入口类型值 = 0;
        std::string 信息入口类型文本{};
        bool 有基础信息类型 = false;
        int 基础信息类型值 = 0;
        std::string 基础信息类型文本{};
        std::string 对应基础信息主键{};
        bool 已绑定基础信息 = false;
    };

    std::mutex& 私有_语素SQL投影互斥() noexcept
    {
        static std::mutex 互斥{};
        return 互斥;
    }

    std::string 私有_SQL字符串(const std::string& 文本, const bool 空为NULL = true)
    {
        if (空为NULL && 文本.empty()) {
            return "NULL";
        }
        std::string 输出 = "N'";
        for (const char 字符 : 文本) {
            if (字符 == '\'') {
                输出 += "''";
            } else {
                输出.push_back(字符);
            }
        }
        输出.push_back('\'');
        return 输出;
    }

    std::string 私有_语素父词面(const 语素节点类* 节点)
    {
        if (!节点 || !节点->父 || !节点->父->主信息) {
            return {};
        }
        auto* 父词 = dynamic_cast<const 词主信息类*>(节点->父->主信息);
        return 父词 ? 父词->词 : std::string{};
    }

    void 私有_填充语素SQL主信息(
        const 语素节点类* 节点,
        结构_语素SQL行& 行)
    {
        if (!节点 || !节点->主信息) {
            行.节点类型 = "空主信息";
            return;
        }

        if (auto* 词 = dynamic_cast<const 词主信息类*>(节点->主信息)) {
            行.节点类型 = "词";
            行.词面 = 词->词;
            行.显示文本 = 词->词;
            return;
        }

        if (auto* 入口 = dynamic_cast<const 语素入口主信息类*>(节点->主信息)) {
            行.节点类型 = 入口->信息入口类型已确定() ? "信息入口" : "人类词性入口";
            行.词面 = 私有_语素父词面(节点);
            行.显示文本 = 行.词面.empty() ? 私有_获取语素节点显示词(节点) : 行.词面;
            行.有词性 = true;
            行.词性值 = static_cast<int>(入口->词性);
            行.词性文本 = 枚举_词性_工厂::根据枚举类型获取文本(入口->词性);
            if (入口->信息入口类型已确定()) {
                行.有信息入口类型 = true;
                行.信息入口类型值 = static_cast<int>(入口->信息入口类型);
                行.信息入口类型文本 = 私有_信息入口类型文本(入口->信息入口类型);
            }
            if (入口->基础信息类型已确定()) {
                行.有基础信息类型 = true;
                行.基础信息类型值 = static_cast<int>(入口->对应基础信息类型);
                行.基础信息类型文本 = 私有_主信息类型文本(入口->对应基础信息类型);
            }
            if (入口->对应基础信息.指针) {
                行.对应基础信息主键 = 入口->对应基础信息.指针->获取主键();
            } else {
                行.对应基础信息主键 = 入口->对应基础信息.主键;
            }
            行.已绑定基础信息 = 入口->对应基础信息.有效();
            return;
        }

        行.节点类型 = "语素";
        行.显示文本 = 私有_获取语素节点显示词(节点);
    }

    void 私有_收集语素SQL节点(
        const 语素节点类* 节点,
        const std::string& 父节点主键,
        const int 深度,
        const int 同层序号,
        const std::string& 父路径,
        std::vector<结构_语素SQL行>& 行集);

    void 私有_收集语素SQL同层(
        const 语素节点类* 首节点,
        const 语素节点类* 停止节点,
        const std::string& 父节点主键,
        const int 深度,
        const std::string& 父路径,
        std::vector<结构_语素SQL行>& 行集)
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
            私有_收集语素SQL节点(
                当前,
                父节点主键,
                深度,
                同层序号,
                父路径,
                行集);
            当前 = static_cast<const 语素节点类*>(当前->下);
            ++同层序号;
            ++保护;
        } while (当前 && 当前 != 首节点 && 当前 != 停止节点 && 保护 < 100000);
    }

    void 私有_收集语素SQL节点(
        const 语素节点类* 节点,
        const std::string& 父节点主键,
        const int 深度,
        const int 同层序号,
        const std::string& 父路径,
        std::vector<结构_语素SQL行>& 行集)
    {
        if (!节点) {
            return;
        }

        const auto 节点主键 = 节点->获取主键();
        const auto 路径 = 父路径.empty()
            ? 节点主键
            : 父路径 + "/" + 节点主键;

        结构_语素SQL行 行{};
        行.行号 = static_cast<int>(行集.size() + 1);
        行.节点主键 = 节点主键;
        行.父节点主键 = 父节点主键;
        行.深度 = 深度;
        行.同层序号 = 同层序号;
        行.直接子数量 = static_cast<int>(节点->子节点数量);
        行.路径 = 路径;
        私有_填充语素SQL主信息(节点, 行);
        行集.push_back(std::move(行));

        if (节点->子) {
            私有_收集语素SQL同层(
                static_cast<const 语素节点类*>(节点->子),
                nullptr,
                节点主键,
                深度 + 1,
                路径,
                行集);
        }
    }

    std::string 私有_语素SQL建库脚本()
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "IF DB_ID(N'鱼巢投影库') IS NULL CREATE DATABASE [鱼巢投影库];\n";
        return SQL.str();
    }

    std::string 私有_语素SQL建表脚本()
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "IF SCHEMA_ID(N'鱼巢') IS NULL EXEC(N'CREATE SCHEMA [鱼巢]');\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前语素节点]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前语素节点];\n"
            << "IF OBJECT_ID(N'[鱼巢].[当前语素主信息]', N'V') IS NOT NULL DROP VIEW [鱼巢].[当前语素主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[语素主信息]', N'U') IS NOT NULL DROP TABLE [鱼巢].[语素主信息];\n"
            << "IF OBJECT_ID(N'[鱼巢].[语素节点]', N'U') IS NOT NULL DROP TABLE [鱼巢].[语素节点];\n"
            << "IF OBJECT_ID(N'[鱼巢].[语素快照]', N'U') IS NOT NULL DROP TABLE [鱼巢].[语素快照];\n"
            << "IF OBJECT_ID(N'[鱼巢].[语素快照]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[语素快照] (\n"
            << "    [快照标识] uniqueidentifier NOT NULL PRIMARY KEY,\n"
            << "    [捕获时间] datetime2(3) NOT NULL,\n"
            << "    [来源类型] nvarchar(80) NOT NULL,\n"
            << "    [来源原因] nvarchar(300) NULL,\n"
            << "    [节点数量] int NOT NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[语素节点]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[语素节点] (\n"
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
            << "IF OBJECT_ID(N'[鱼巢].[语素主信息]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[语素主信息] (\n"
            << "    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,\n"
            << "    [快照标识] uniqueidentifier NOT NULL,\n"
            << "    [节点主键] nvarchar(80) NOT NULL,\n"
            << "    [节点行号] int NOT NULL,\n"
            << "    [节点类型] nvarchar(40) NULL,\n"
            << "    [词面文本] nvarchar(400) NULL,\n"
            << "    [显示文本] nvarchar(400) NULL,\n"
            << "    [词性值] int NULL,\n"
            << "    [词性文本] nvarchar(80) NULL,\n"
            << "    [信息入口类型值] int NULL,\n"
            << "    [信息入口类型文本] nvarchar(80) NULL,\n"
            << "    [映射主信息类型值] int NULL,\n"
            << "    [映射主信息类型文本] nvarchar(80) NULL,\n"
            << "    [绑定基础信息主键] nvarchar(80) NULL,\n"
            << "    [已绑定基础信息] bit NOT NULL\n"
            << ");\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_lexeme_node_key' AND object_id = OBJECT_ID(N'[鱼巢].[语素节点]'))\n"
            << "    CREATE INDEX IX_lexeme_node_key ON [鱼巢].[语素节点]([快照标识], [节点主键], [父节点主键]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_lexeme_node_row' AND object_id = OBJECT_ID(N'[鱼巢].[语素节点]'))\n"
            << "    CREATE INDEX IX_lexeme_node_row ON [鱼巢].[语素节点]([快照标识], [行号]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_lexeme_main_info_node' AND object_id = OBJECT_ID(N'[鱼巢].[语素主信息]'))\n"
            << "    CREATE INDEX IX_lexeme_main_info_node ON [鱼巢].[语素主信息]([快照标识], [节点主键]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_lexeme_main_info_row' AND object_id = OBJECT_ID(N'[鱼巢].[语素主信息]'))\n"
            << "    CREATE INDEX IX_lexeme_main_info_row ON [鱼巢].[语素主信息]([快照标识], [节点行号]);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_lexeme_main_info_word' AND object_id = OBJECT_ID(N'[鱼巢].[语素主信息]'))\n"
            << "    CREATE INDEX IX_lexeme_main_info_word ON [鱼巢].[语素主信息]([快照标识], [词面文本], [节点类型]);\n";
        return SQL.str();
    }

    std::string 私有_语素SQL视图脚本()
    {
        std::ostringstream SQL;
        SQL << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前语素主信息] AS\n"
            << "SELECT m.*\n"
            << "FROM [鱼巢].[语素主信息] m\n"
            << "WHERE m.[快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[语素快照] ORDER BY [捕获时间] DESC);');\n"
            << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前语素节点] AS\n"
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
            << "    m.[节点类型],\n"
            << "    m.[词面文本],\n"
            << "    m.[显示文本],\n"
            << "    m.[词性值],\n"
            << "    m.[词性文本],\n"
            << "    m.[信息入口类型值],\n"
            << "    m.[信息入口类型文本],\n"
            << "    m.[映射主信息类型值],\n"
            << "    m.[映射主信息类型文本],\n"
            << "    m.[绑定基础信息主键],\n"
            << "    m.[已绑定基础信息]\n"
            << "FROM [鱼巢].[语素节点] n\n"
            << "LEFT JOIN [鱼巢].[语素主信息] m\n"
            << "    ON m.[快照标识] = n.[快照标识]\n"
            << "    AND m.[节点行号] = n.[行号]\n"
            << "WHERE n.[快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[语素快照] ORDER BY [捕获时间] DESC);');\n";
        return SQL.str();
    }

    std::string 私有_构造语素SQL重写脚本(
        const std::vector<结构_语素SQL行>& 行集,
        const std::string& 来源原因)
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "SET XACT_ABORT ON;\n"
            << "BEGIN TRANSACTION;\n"
            << "IF EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_lexeme_node_word' AND object_id = OBJECT_ID(N'[鱼巢].[语素节点]'))\n"
            << "    DROP INDEX IX_lexeme_node_word ON [鱼巢].[语素节点];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'节点类型') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [节点类型];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'词面文本') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [词面文本];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'显示文本') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [显示文本];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'词性值') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [词性值];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'词性文本') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [词性文本];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'信息入口类型值') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [信息入口类型值];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'信息入口类型文本') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [信息入口类型文本];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'映射主信息类型值') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [映射主信息类型值];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'映射主信息类型文本') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [映射主信息类型文本];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'绑定基础信息主键') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [绑定基础信息主键];\n"
            << "IF COL_LENGTH(N'[鱼巢].[语素节点]', N'已绑定基础信息') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[语素节点] DROP COLUMN [已绑定基础信息];\n"
            << "DELETE FROM [鱼巢].[语素主信息];\n"
            << "DELETE FROM [鱼巢].[语素节点];\n"
            << "DELETE FROM [鱼巢].[语素快照];\n"
            << "DECLARE @快照标识 uniqueidentifier = NEWID();\n"
            << "INSERT INTO [鱼巢].[语素快照] ([快照标识], [捕获时间], [来源类型], [来源原因], [节点数量])\n"
            << "VALUES (@快照标识, SYSUTCDATETIME(), N'lexeme_tree_projection', "
            << 私有_SQL字符串(来源原因)
            << ", "
            << 行集.size()
            << ");\n";
        for (const auto& 行 : 行集) {
            SQL << "INSERT INTO [鱼巢].[语素节点] ([快照标识], [行号], [节点主键], [父节点主键], [深度], [同层序号], [直接子数量], [路径文本]) VALUES (@快照标识, "
                << 行.行号 << ", "
                << 私有_SQL字符串(行.节点主键, false) << ", "
                << 私有_SQL字符串(行.父节点主键) << ", "
                << 行.深度 << ", "
                << 行.同层序号 << ", "
                << 行.直接子数量 << ", "
                << 私有_SQL字符串(行.路径) << ");\n";
            SQL << "INSERT INTO [鱼巢].[语素主信息] ([快照标识], [节点主键], [节点行号], [节点类型], [词面文本], [显示文本], [词性值], [词性文本], [信息入口类型值], [信息入口类型文本], [映射主信息类型值], [映射主信息类型文本], [绑定基础信息主键], [已绑定基础信息]) VALUES (@快照标识, "
                << 私有_SQL字符串(行.节点主键, false) << ", "
                << 行.行号 << ", "
                << 私有_SQL字符串(行.节点类型) << ", "
                << 私有_SQL字符串(行.词面) << ", "
                << 私有_SQL字符串(行.显示文本) << ", "
                << SQL可空整数文本(行.有词性, 行.词性值) << ", "
                << 私有_SQL字符串(行.词性文本) << ", "
                << SQL可空整数文本(行.有信息入口类型, 行.信息入口类型值) << ", "
                << 私有_SQL字符串(行.信息入口类型文本) << ", "
                << SQL可空整数文本(行.有基础信息类型, 行.基础信息类型值) << ", "
                << 私有_SQL字符串(行.基础信息类型文本) << ", "
                << 私有_SQL字符串(行.对应基础信息主键) << ", "
                << 布尔文本_一或零(行.已绑定基础信息) << ");\n";
        }
        SQL << "IF EXISTS (\n"
            << "    SELECT 1\n"
            << "    FROM [鱼巢].[语素主信息] info\n"
            << "    LEFT JOIN [鱼巢].[语素节点] node\n"
            << "        ON node.[快照标识] = info.[快照标识] AND node.[行号] = info.[节点行号]\n"
            << "    WHERE info.[快照标识] = @快照标识 AND node.[行号] IS NULL\n"
            << ")\n"
            << "    THROW 51041, N'lexeme_main_info [节点行号] invalid', 1;\n";
        SQL << "COMMIT TRANSACTION;\n";
        return SQL.str();
    }

    bool 私有_执行语素ADO命令(
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

    std::vector<std::vector<std::string>> 私有_语素SQL快照预期字段(
        const std::string& 来源原因,
        const std::size_t 节点数)
    {
        return {
            {
                "lexeme_tree_projection",
                来源原因,
                std::to_string(节点数),
            },
        };
    }

    std::vector<std::vector<std::string>> 私有_语素SQL节点预期字段(
        const std::vector<结构_语素SQL行>& 行集)
    {
        std::vector<std::vector<std::string>> 输出;
        输出.reserve(行集.size());
        for (const auto& 行 : 行集) {
            输出.push_back({
                SQL整数文本(行.行号),
                行.节点主键,
                行.父节点主键,
                SQL整数文本(行.深度),
                SQL整数文本(行.同层序号),
                SQL整数文本(行.直接子数量),
                行.路径,
            });
        }
        return 输出;
    }

    std::vector<std::vector<std::string>> 私有_语素SQL主信息预期字段(
        const std::vector<结构_语素SQL行>& 行集)
    {
        std::vector<std::vector<std::string>> 输出;
        输出.reserve(行集.size());
        for (const auto& 行 : 行集) {
            输出.push_back({
                行.节点主键,
                SQL整数文本(行.行号),
                行.节点类型,
                行.词面,
                行.显示文本,
                SQL字段可空整数文本(行.有词性, 行.词性值),
                行.词性文本,
                SQL字段可空整数文本(行.有信息入口类型, 行.信息入口类型值),
                行.信息入口类型文本,
                SQL字段可空整数文本(行.有基础信息类型, 行.基础信息类型值),
                行.基础信息类型文本,
                行.对应基础信息主键,
                std::string(布尔文本_一或零(行.已绑定基础信息)),
            });
        }
        return 输出;
    }

    bool 私有_执行语素SQL字段恢复比对(
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

    bool 私有_验证语素SQL存储字段(
        const std::string& 连接串,
        const std::vector<结构_语素SQL行>& 行集,
        const std::string& 来源原因,
        std::string& 错误)
    {
        constexpr const char* 快照SQL = R"SQL(
SELECT
    COALESCE([来源类型], N''),
    COALESCE([来源原因], N''),
    COALESCE(CONVERT(nvarchar(30), [节点数量]), N'')
FROM [鱼巢].[语素快照]
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
FROM [鱼巢].[语素节点]
WHERE [快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[语素快照] ORDER BY [捕获时间] DESC)
ORDER BY [行号];
)SQL";
        constexpr const char* 主信息SQL = R"SQL(
SELECT
    COALESCE([节点主键], N''),
    COALESCE(CONVERT(nvarchar(30), [节点行号]), N''),
    COALESCE([节点类型], N''),
    COALESCE([词面文本], N''),
    COALESCE([显示文本], N''),
    COALESCE(CONVERT(nvarchar(30), [词性值]), N''),
    COALESCE([词性文本], N''),
    COALESCE(CONVERT(nvarchar(30), [信息入口类型值]), N''),
    COALESCE([信息入口类型文本], N''),
    COALESCE(CONVERT(nvarchar(30), [映射主信息类型值]), N''),
    COALESCE([映射主信息类型文本], N''),
    COALESCE([绑定基础信息主键], N''),
    COALESCE(CONVERT(nvarchar(1), [已绑定基础信息]), N'')
FROM [鱼巢].[语素主信息]
WHERE [快照标识] = (SELECT TOP (1) [快照标识] FROM [鱼巢].[语素快照] ORDER BY [捕获时间] DESC)
ORDER BY [节点行号];
)SQL";

        return 私有_执行语素SQL字段恢复比对(
            连接串,
            "语素SQL字段恢复比对/快照",
            快照SQL,
            私有_语素SQL快照预期字段(来源原因, 行集.size()),
            错误)
            && 私有_执行语素SQL字段恢复比对(
                连接串,
                "语素SQL字段恢复比对/节点",
                节点SQL,
                私有_语素SQL节点预期字段(行集),
                错误)
            && 私有_执行语素SQL字段恢复比对(
                连接串,
                "语素SQL字段恢复比对/主信息",
                主信息SQL,
                私有_语素SQL主信息预期字段(行集),
                错误);
    }

}

语素类 语素集{};

语素类::语素类() = default;

// 功能：释放或收束对象资源。
语素类::~语素类()
{
    auto lk = 获取锁();
    私有_清理主信息_已加锁();
}

// 功能：初始化相关对象、状态或运行上下文。
void 语素类::初始化()
{
    static constexpr const char* 预热词表[][2] = {
        { "需求_权重", "名词" },
        { "二次特征_标量值", "名词" },
        { "安全需求", "名词" },
        { "服务需求", "名词" },
        { "自我根需求_安全", "名词" },
        { "自我根需求_服务", "名词" },
    };

    for (const auto& 项 : 预热词表) {
        (void)添加词性词(项[0], 项[1]);
    }
}

// 功能：把当前语素树本体重写到 SQL Server 查询投影。
bool 语素类::重写语素SQL投影(const char* 来源原因, const bool 执行字段恢复比对) const noexcept
{
    std::lock_guard<std::mutex> SQL锁{ 私有_语素SQL投影互斥() };
    try {
        std::vector<结构_语素SQL行> 行集{};
        {
            读锁守卫 锁{ 链表锁 };
            if (根指针 && 根指针->下 && 根指针->下 != 根指针) {
                私有_收集语素SQL同层(
                    static_cast<const 语素节点类*>(根指针->下),
                    static_cast<const 语素节点类*>(根指针),
                    {},
                    0,
                    {},
                    行集);
            }
        }

        const auto 原因文本 = 来源原因 ? std::string(来源原因) : std::string{};
        const auto 主库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "master");
        const auto 投影库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "鱼巢投影库");
        std::string 错误{};
        if (!私有_执行语素ADO命令(主库连接串, "语素SQL投影建库", 私有_语素SQL建库脚本(), 错误)
            || !私有_执行语素ADO命令(投影库连接串, "语素SQL投影建表", 私有_语素SQL建表脚本(), 错误)
            || !私有_执行语素ADO命令(投影库连接串, "语素SQL投影视图", 私有_语素SQL视图脚本(), 错误)
            || !私有_执行语素ADO命令(投影库连接串, "语素SQL投影重写", 私有_构造语素SQL重写脚本(行集, 原因文本), 错误)
            || (执行字段恢复比对 && !私有_验证语素SQL存储字段(投影库连接串, 行集, 原因文本, 错误))) {
            项目运行错误日志(
                "语素SQL投影失败"
                " | 原因=" + 错误
                + " | 节点数=" + std::to_string(行集.size()));
            return false;
        }

        项目运行日志(
            "语素SQL投影完成"
            " | 来源=" + 原因文本
            + " | 节点数=" + std::to_string(行集.size())
            + " | 字段恢复比对=" + (执行字段恢复比对 ? "通过" : "跳过"));
        return true;
    }
    catch (const std::exception& 异常) {
        项目运行错误日志(std::string("语素SQL投影异常 | 原因=") + 异常.what());
    }
    catch (...) {
        项目运行错误日志("语素SQL投影异常 | 原因=未知异常");
    }
    return false;
}

// 功能：根据当前输入生成目标数据、场景、动态或回执。
语素入口节点类* 语素类::生成语素节点(结构体_分词 分词信息)
{
    return 添加词性词(std::move(分词信息));
}

// 功能：按函数名执行对应处理。
词节点类* 语素类::添加词(const std::string& 词字符串)
{
    if (词字符串.empty()) {
        throw std::invalid_argument("语素类::添加词 - 输入词字符串为空");
    }

    写锁守卫 lk(链表锁);

    // 词节点按 UTF-8 前缀构建词典结构；词面本身不是世界事实或治理语义来源。
    链节点类* 游标 = 根指针;
    词节点类* 当前节点 = nullptr;
    std::string 前缀;

    for (std::size_t i = 0; i < 词字符串.size(); ) {
        const std::size_t step = std::min(私有_UTF8字符长度(static_cast<unsigned char>(词字符串[i])), 词字符串.size() - i);
        前缀.append(词字符串, i, step);
        i += step;

        当前节点 = 查找词_已加锁(前缀);
        if (当前节点) {
            游标 = 当前节点;
            continue;
        }

        auto* 新词节点 = new 词节点类{};
        新词节点->主信息 = new 词主信息类(前缀);

        链节点类* 新节点 = (游标 == 根指针)
            ? 添加节点_已加锁(游标, 新词节点)
            : 添加子节点_已加锁(游标, 新词节点);

        if (!新节点) {
            delete 新词节点->主信息;
            delete 新词节点;
            return nullptr;
        }

        当前节点 = static_cast<词节点类*>(新节点);
        游标 = 当前节点;
    }

    return 当前节点;
}

// 功能：按条件查找目标对象、方法或事实。
词节点类* 语素类::查找词节点(const std::string& 词字符串) const
{
    if (词字符串.empty()) {
        throw std::invalid_argument("语素类::查找词节点 - 输入词字符串为空");
    }

    读锁守卫 lk(链表锁);
    return 查找词_已加锁(词字符串);
}

// 功能：读取并返回指定对象、状态或运行材料。
词节点类* 语素类::获取词节点指针(const std::string& 词字符串) const
{
    if (词字符串.empty()) return nullptr;

    读锁守卫 lk(链表锁);
    return 查找词_已加锁(词字符串);
}

// 功能：只读返回语素树根节点。
const 语素节点类* 语素类::获取语素树根节点() const noexcept
{
    读锁守卫 lk(链表锁);
    return static_cast<const 语素节点类*>(根指针);
}

// 功能：判断指定节点是否属于当前语素树。
bool 语素类::节点属于语素树(const 语素节点类* 节点) const noexcept
{
    if (!节点) return false;
    读锁守卫 lk(链表锁);
    const auto* 根 = static_cast<const 语素节点类*>(根指针);
    if (!根) return false;
    if (节点 == 根) return true;
    for (auto* 当前 = static_cast<const 语素节点类*>(根->链下);
         当前 && 当前 != 根;
         当前 = static_cast<const 语素节点类*>(当前->链下)) {
        if (当前 == 节点) {
            return true;
        }
    }
    return false;
}

// 功能：按主键只读查找语素入口节点，不隐式创建节点。
const 语素入口节点类* 语素类::按主键查找语素入口节点(const std::string& 主键) const noexcept
{
    if (主键.empty()) return nullptr;
    读锁守卫 lk(链表锁);
    const auto* 根 = static_cast<const 语素节点类*>(根指针);
    if (!根) return nullptr;
    if (根->获取主键() == 主键) {
        return static_cast<const 语素入口节点类*>(根);
    }
    for (auto* 当前 = static_cast<const 语素节点类*>(根->链下);
         当前 && 当前 != 根;
         当前 = static_cast<const 语素节点类*>(当前->链下)) {
        if (当前->获取主键() == 主键) {
            return static_cast<const 语素入口节点类*>(当前);
        }
    }
    return nullptr;
}

// 功能：按条件查找目标对象、方法或事实。
语素入口节点类* 语素类::查找人类词性入口节点(const 词节点类* 词节点, 枚举_词性 词性值) const
{
    读锁守卫 lk(链表锁);
    return 查找人类词性入口节点_已加锁(词节点, 词性值);
}

// 功能：按词值和人类词性只读查找语素入口节点，不隐式创建词或入口。
语素入口节点类* 语素类::查找人类词性入口节点(const std::string& 词值, 枚举_词性 词性值) const
{
    if (词值.empty() || 词性值 == 枚举_词性::未定义) return nullptr;
    读锁守卫 lk(链表锁);
    auto* 词节点 = 查找词_已加锁(词值);
    return 查找人类词性入口节点_已加锁(词节点, 词性值);
}

// 功能：按词值和人类词性文本只读查找语素入口节点，不隐式创建词或入口。
语素入口节点类* 语素类::查找人类词性入口节点(const std::string& 词值, const std::string& 词性值) const
{
    return 查找人类词性入口节点(词值, 枚举_词性_工厂::根据文本获取枚举值(词性值));
}

// 功能：按条件查找目标对象、方法或事实。
语素入口节点类* 语素类::查找人类词性入口节点(const 语素节点类* 词节点, const std::string& 词性值) const
{
    return 查找人类词性入口节点(static_cast<const 词节点类*>(词节点), 枚举_词性_工厂::根据文本获取枚举值(词性值));
}

// 功能：按条件查找目标对象、方法或事实。
语素入口节点类* 语素类::查找人类词性入口节点(结构体_分词 分词信息) const
{
    auto* 词节点 = 查找词节点(分词信息.词);
    if (!词节点) return nullptr;
    return 查找人类词性入口节点(词节点, 分词信息.词性);
}

// 功能：按条件查找目标对象、方法或事实。
语素入口节点类* 语素类::查找信息入口节点(const 词节点类* 词节点, 枚举_信息入口类型 信息入口类型) const
{
    读锁守卫 lk(链表锁);
    return 查找信息入口节点_已加锁(词节点, 信息入口类型);
}

// 功能：按词值和信息入口类型只读查找语素入口节点，不隐式创建词或入口。
语素入口节点类* 语素类::查找信息入口节点(const std::string& 词值, 枚举_信息入口类型 信息入口类型) const
{
    if (词值.empty() || 信息入口类型 == 枚举_信息入口类型::未定义) return nullptr;
    读锁守卫 lk(链表锁);
    auto* 词节点 = 查找词_已加锁(词值);
    return 查找信息入口节点_已加锁(词节点, 信息入口类型);
}

// 功能：按函数名执行对应处理。
语素入口节点类* 语素类::添加词性词(词节点类* 词节点, 枚举_词性 词性)
{
    // 人类词性入口只记录语言标注，并由默认信息入口候选推导基础信息类型。
    // 内部语义对象、特征槽、任务/方法虚拟存在等必须走 添加信息入口词。
    if (!词节点) {
        throw std::invalid_argument("语素类::添加词性词 - 词节点为空");
    }
    if (词性 == 枚举_词性::未定义) {
        throw std::invalid_argument("语素类::添加词性词 - 词性未定义");
    }

    写锁守卫 lk(链表锁);

    if (auto* 已有节点 = 查找人类词性入口节点_已加锁(词节点, 词性)) {
        return 已有节点;
    }

    auto* 新入口节点 = new 语素入口节点类{};
    新入口节点->主信息 = new 语素入口主信息类(词性);

    auto* 节点 = 添加子节点_已加锁(const_cast<词节点类*>(词节点), 新入口节点);
    if (!节点) {
        delete 新入口节点->主信息;
        delete 新入口节点;
        return nullptr;
    }

    return static_cast<语素入口节点类*>(节点);
}


// 功能：按函数名执行对应处理。
语素入口节点类* 语素类::添加词性词(const std::string& 词值, 枚举_词性 词性值)
{
    auto* 词节点 = 添加词(词值);
    return 添加词性词(词节点, 词性值);
}

// 功能：按函数名执行对应处理。
语素入口节点类* 语素类::添加词性词(const std::string& 词值, const std::string& 词性值)
{
    auto* 词节点 = 添加词(词值);
    return 添加词性词(词节点, 枚举_词性_工厂::根据文本获取枚举值(词性值));
}

// 功能：按函数名执行对应处理。
语素入口节点类* 语素类::添加词性词(结构体_分词 词信息)
{
    return 添加词性词(词信息.词, 词信息.词性);
}

// 功能：按函数名执行对应处理。
语素入口节点类* 语素类::添加信息入口词(词节点类* 词节点, 枚举_信息入口类型 信息入口类型)
{
    // 系统语义入口的权威创建接口：信息入口类型决定主信息类型、默认存储树和上下文约束。
    if (!词节点) {
        throw std::invalid_argument("语素类::添加信息入口词 - 词节点为空");
    }
    if (信息入口类型 == 枚举_信息入口类型::未定义) {
        throw std::invalid_argument("语素类::添加信息入口词 - 信息入口类型未定义");
    }

    写锁守卫 lk(链表锁);

    if (auto* 已有节点 = 查找信息入口节点_已加锁(词节点, 信息入口类型)) {
        return 已有节点;
    }

    auto* 新入口节点 = new 语素入口节点类{};
    新入口节点->主信息 = new 语素入口主信息类(信息入口类型);

    auto* 节点 = 添加子节点_已加锁(const_cast<词节点类*>(词节点), 新入口节点);
    if (!节点) {
        delete 新入口节点->主信息;
        delete 新入口节点;
        return nullptr;
    }

    return static_cast<语素入口节点类*>(节点);
}

// 功能：按函数名执行对应处理。
语素入口节点类* 语素类::添加信息入口词(const std::string& 词值, 枚举_信息入口类型 信息入口类型)
{
    auto* 词节点 = 添加词(词值);
    return 添加信息入口词(词节点, 信息入口类型);
}

// 功能：根据已有语素入口所属词，只读查找同词下指定信息入口类型。
语素入口节点类* 语素类::查找同词信息入口(
    const 语素入口节点类* 已有入口,
    枚举_信息入口类型 信息入口类型) const
{
    if (!已有入口 || 信息入口类型 == 枚举_信息入口类型::未定义) {
        return nullptr;
    }

    读锁守卫 lk(链表锁);
    if (!dynamic_cast<语素入口主信息类*>(const_cast<语素入口节点类*>(已有入口)->主信息)) {
        return nullptr;
    }
    auto* 词节点 = static_cast<const 词节点类*>(已有入口->父);
    if (!词节点 || !dynamic_cast<词主信息类*>(const_cast<词节点类*>(词节点)->主信息)) {
        return nullptr;
    }

    return 查找信息入口节点_已加锁(词节点, 信息入口类型);
}

// 功能：根据已有语素入口所属词，取或创建同词下指定信息入口类型。
语素入口节点类* 语素类::取或创建同词信息入口(
    const 语素入口节点类* 已有入口,
    枚举_信息入口类型 信息入口类型)
{
    if (auto* 已有同词入口 = 查找同词信息入口(已有入口, 信息入口类型)) {
        return 已有同词入口;
    }
    if (!已有入口 || 信息入口类型 == 枚举_信息入口类型::未定义) {
        return nullptr;
    }
    if (!是语素入口节点(已有入口)) {
        return nullptr;
    }

    auto* 词节点 = static_cast<词节点类*>(已有入口->父);
    if (!词节点 || !节点属于语素树(词节点)) {
        return nullptr;
    }
    if (!dynamic_cast<词主信息类*>(词节点->主信息)) {
        return nullptr;
    }

    return 添加信息入口词(词节点, 信息入口类型);
}


// 功能：确保目标结构、状态或前置条件存在并可用。
基础信息节点类* 语素类::确保语素入口对应基础信息(语素入口节点类* 入口节点, 基础信息类* 基础信息仓库, 基础信息节点类* 父节点)
{
    if (!入口节点) {
        throw std::invalid_argument("语素类::确保语素入口对应基础信息 - 语素入口节点为空");
    }

    枚举_主信息类型 目标类型 = 枚举_主信息类型::未定义;
    基础信息节点类* 已绑定节点 = nullptr;

    {
        读锁守卫 lk(链表锁);
        auto* 入口主信息 = dynamic_cast<语素入口主信息类*>(入口节点->主信息);
        if (!入口主信息) {
            项目运行错误日志("语素类/runtime_error | 确保语素入口对应基础信息 | 节点主信息不是语素入口主信息类");
            throw std::runtime_error("语素类::确保语素入口对应基础信息 - 节点主信息不是语素入口主信息类");
        }
        目标类型 = 入口主信息->基础信息类型已确定()
            ? 入口主信息->对应基础信息类型
            : 语素主信息内部::推断人类词性默认基础信息类型(入口主信息->词性);
        已绑定节点 = 入口主信息->对应基础信息.指针;
    }

    if (目标类型 == 枚举_主信息类型::未定义) {
        return nullptr;
    }

    if (已绑定节点) {
        if (私有_取基础信息类型(已绑定节点) != 目标类型) {
            项目运行错误日志("语素类/runtime_error | 确保语素入口对应基础信息 | 已绑定基础信息类型与语素入口映射不一致");
            throw std::runtime_error("语素类::确保语素入口对应基础信息 - 已绑定基础信息类型与语素入口映射不一致");
        }
        return 已绑定节点;
    }

    基础信息类& 仓库 = 基础信息仓库 ? *基础信息仓库 : 获取基础信息集();
    auto* 基础节点 = 仓库.确保语素入口模板(入口节点, 目标类型, 父节点);
    if (!基础节点) return nullptr;

    (void)添加对应基础信息(入口节点, 基础节点);
    return 基础节点;
}

// 功能：确保目标结构、状态或前置条件存在并可用。
基础信息节点类* 语素类::确保语素入口对应基础信息(const std::string& 词值, 枚举_词性 词性, 基础信息类* 基础信息仓库, 基础信息节点类* 父节点)
{
    auto* 入口节点 = 添加词性词(词值, 词性);
    return 确保语素入口对应基础信息(入口节点, 基础信息仓库, 父节点);
}

// 功能：确保目标结构、状态或前置条件存在并可用。
基础信息节点类* 语素类::确保信息入口对应基础信息(const std::string& 词值, 枚举_信息入口类型 信息入口类型, 基础信息类* 基础信息仓库, 基础信息节点类* 父节点)
{
    auto* 入口节点 = 添加信息入口词(词值, 信息入口类型);
    return 确保语素入口对应基础信息(入口节点, 基础信息仓库, 父节点);
}

// 功能：按条件查找目标对象、方法或事实。
基础信息节点类* 语素类::查找语素入口对应基础信息(const 语素入口节点类* 入口节点) const
{
    if (!入口节点) return nullptr;
    读锁守卫 lk(链表锁);
    auto* 入口主信息 = dynamic_cast<语素入口主信息类*>(const_cast<语素入口节点类*>(入口节点)->主信息);
    return 入口主信息 ? 入口主信息->对应基础信息.指针 : nullptr;
}

// 功能：判断节点主信息是否为语素入口主信息。
bool 语素类::是语素入口节点(const 语素入口节点类* 入口节点) const noexcept
{
    if (!入口节点) return false;
    读锁守卫 lk(链表锁);
    return dynamic_cast<语素入口主信息类*>(const_cast<语素入口节点类*>(入口节点)->主信息) != nullptr;
}

// 功能：读取语素入口节点的人类词性。
枚举_词性 语素类::获取语素入口人类词性(const 语素入口节点类* 入口节点) const
{
    if (!入口节点) return 枚举_词性::未定义;
    读锁守卫 lk(链表锁);
    auto* 入口主信息 = dynamic_cast<语素入口主信息类*>(const_cast<语素入口节点类*>(入口节点)->主信息);
    return 入口主信息 ? 入口主信息->词性 : 枚举_词性::未定义;
}

// 功能：读取并返回指定对象、状态或运行材料。
枚举_主信息类型 语素类::获取语素入口对应基础信息类型(const 语素入口节点类* 入口节点) const
{
    if (!入口节点) return 枚举_主信息类型::未定义;
    读锁守卫 lk(链表锁);
    auto* 入口主信息 = dynamic_cast<语素入口主信息类*>(const_cast<语素入口节点类*>(入口节点)->主信息);
    if (!入口主信息) return 枚举_主信息类型::未定义;
    return 入口主信息->基础信息类型已确定()
        ? 入口主信息->对应基础信息类型
        : 语素主信息内部::推断人类词性默认基础信息类型(入口主信息->词性);
}

// 功能：读取语素入口节点显式绑定的基础信息类型，不做词性默认推断。
枚举_主信息类型 语素类::获取语素入口显式对应基础信息类型(const 语素入口节点类* 入口节点) const
{
    if (!入口节点) return 枚举_主信息类型::未定义;
    读锁守卫 lk(链表锁);
    auto* 入口主信息 = dynamic_cast<语素入口主信息类*>(const_cast<语素入口节点类*>(入口节点)->主信息);
    return 入口主信息 ? 入口主信息->对应基础信息类型 : 枚举_主信息类型::未定义;
}

// 功能：读取并返回指定对象、状态或运行材料。
枚举_信息入口类型 语素类::获取语素入口信息入口类型(const 语素入口节点类* 入口节点) const
{
    if (!入口节点) return 枚举_信息入口类型::未定义;
    读锁守卫 lk(链表锁);
    auto* 入口主信息 = dynamic_cast<语素入口主信息类*>(const_cast<语素入口节点类*>(入口节点)->主信息);
    return 入口主信息 ? 入口主信息->信息入口类型 : 枚举_信息入口类型::未定义;
}

// 功能：读取指定信息入口类型的定义项。
const 结构_信息入口定义* 语素类::读取信息入口定义(枚举_信息入口类型 信息入口类型) noexcept
{
    return 语素主信息内部::查找信息入口定义(信息入口类型);
}

// 功能：读取指定信息入口类型对应的主信息类型。
枚举_主信息类型 语素类::读取信息入口主信息类型(枚举_信息入口类型 信息入口类型) noexcept
{
    return 语素主信息内部::取信息入口主信息类型(信息入口类型);
}

// 功能：读取指定信息入口类型对应的默认存储树。
枚举_信息存储树 语素类::读取信息入口存储树(枚举_信息入口类型 信息入口类型) noexcept
{
    return 语素主信息内部::取信息入口存储树(信息入口类型);
}

// 功能：读取指定信息入口类型默认映射的人类词性。
枚举_词性 语素类::读取信息入口默认人类词性(枚举_信息入口类型 信息入口类型) noexcept
{
    return 语素主信息内部::取信息入口默认人类词性(信息入口类型);
}

// 功能：读取指定信息入口类型的默认根名称。
const char* 语素类::读取信息入口默认根名(枚举_信息入口类型 信息入口类型) noexcept
{
    return 语素主信息内部::取信息入口默认根名(信息入口类型);
}

// 功能：判断指定信息入口类型是否需要上下文限定。
bool 语素类::信息入口需要上下文限定(枚举_信息入口类型 信息入口类型) noexcept
{
    return 语素主信息内部::信息入口需要上下文限定(信息入口类型);
}

// 功能：判断指定信息入口类型是否允许裸词默认模板入口。
bool 语素类::信息入口裸词默认模板入口(枚举_信息入口类型 信息入口类型) noexcept
{
    return 语素主信息内部::信息入口裸词默认模板入口(信息入口类型);
}

// 功能：按函数名执行对应处理。
枚举_主信息类型 语素类::推断人类词性默认基础信息类型(枚举_词性 词性) noexcept
{
    return 语素主信息内部::推断人类词性默认基础信息类型(词性);
}

// 功能：判断基础信息主信息类型是否可由语素入口模板创建。
bool 语素类::基础信息类型可由语素入口模板创建(枚举_主信息类型 类型) noexcept
{
    return 语素主信息内部::基础信息类型可由语素入口模板创建(类型);
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
const 语素入口节点类* 语素类::绑定人类词性入口到场景基础信息(
    const std::string& 词字符串,
    枚举_词性 词性,
    场景节点类* 场景,
    基础信息节点类* 基础信息节点
)
{
    if (词字符串.empty() || !基础信息节点) {
        throw std::invalid_argument("语素类::绑定人类词性入口到场景基础信息 - 参数为空");
    }

    // 场景绑定只是“该语言入口在当前场景下引用哪个基础信息”的桥接，不写世界事实。
    auto* 词节点 = 添加词(词字符串);
    auto* 入口节点 = 添加词性词(词节点, 词性);
    (void)添加对应基础信息(入口节点, 基础信息节点);
    auto& 基础信息 = 获取基础信息集();
    私有_把基础信息挂入场景索引(基础信息, 场景, 基础信息节点);
    return 入口节点;
}

// 功能：按条件查找目标对象、方法或事实。
std::vector<基础信息节点类*> 语素类::通过词和场景查找基础信息(
    const std::string& 词字符串,
    场景节点类* 当前场景,
    const 枚举_词性* 词性
) const
{
    std::vector<基础信息节点类*> 全部结果;
    std::vector<基础信息节点类*> 场景结果;
    if (词字符串.empty()) return 全部结果;

    读锁守卫 lk(链表锁);
    // 该查询只解析语言入口绑定；返回的基础信息是否满足任务 / 需求需由特征和二次特征另行判断。
    auto* 词节点 = 查找词_已加锁(词字符串);
    if (!词节点 || !词节点->子) return 全部结果;

    auto* 起点 = 词节点->子;
    auto* 游标 = 起点;
    auto& 基础信息 = 获取基础信息集();
    do {
        auto* 主信息 = dynamic_cast<语素入口主信息类*>(游标->主信息);
        if (主信息 && (!词性 || 主信息->词性 == *词性)) {
            if (auto* 基础节点 = 主信息->对应基础信息.指针) {
                全部结果.push_back(基础节点);
                if (当前场景 && 私有_场景索引包含(基础信息, 当前场景, 基础节点)) {
                    场景结果.push_back(基础节点);
                }
            }
        }
        游标 = 游标->下;
    } while (游标 && 游标 != 起点);

    return 当前场景 && !场景结果.empty() ? 场景结果 : 全部结果;
}

// 功能：按函数名执行对应处理。
词节点类* 语素类::添加二次特征词(语素节点类* 参照物, 语素节点类* 比较对象, 语素节点类* 比较类型)
{
    const std::string 参照词 = 私有_获取语素节点显示词(参照物);
    const std::string 对象词 = 私有_获取语素节点显示词(比较对象);
    const std::string 类型词 = 私有_获取语素节点显示词(比较类型);

    std::string 组合词;
    if (!参照词.empty()) 组合词 += 参照词;
    组合词 += "_相对_";
    if (!对象词.empty()) 组合词 += 对象词;
    if (!类型词.empty()) {
        组合词 += "_";
        组合词 += 类型词;
    }
    if (组合词 == "_相对_") {
        组合词 = "二次特征_未命名";
    }

    auto* 词节点 = 添加词(组合词);
    if (auto* 入口节点 = 添加词性词(词节点, 枚举_词性::副词)) {
        (void)确保语素入口对应基础信息(入口节点);
    }
    return 词节点;
}

// 功能：按函数名执行对应处理。
std::int64_t 语素类::添加对应基础信息(const 语素入口节点类* 词节点, 基础信息节点类* 基础信息)
{
    if (!词节点) {
        throw std::invalid_argument("语素类::添加对应基础信息 - 词节点为空");
    }
    if (!基础信息) {
        throw std::invalid_argument("语素类::添加对应基础信息 - 基础信息为空");
    }

    const auto 实际类型 = 私有_取基础信息类型(基础信息);
    if (实际类型 == 枚举_主信息类型::未定义) {
        throw std::invalid_argument("语素类::添加对应基础信息 - 基础信息没有有效主信息类型");
    }

    // 这里只绑定语素入口到基础信息节点的路由引用，不复制或改写该基础信息的业务事实。
    写锁守卫 lk(链表锁);
    auto* 入口主信息 = dynamic_cast<语素入口主信息类*>(const_cast<语素入口节点类*>(词节点)->主信息);
    if (!入口主信息) {
        项目运行错误日志("语素类/runtime_error | 添加对应基础信息 | 词节点主信息不是语素入口主信息类");
        throw std::runtime_error("语素类::添加对应基础信息 - 词节点主信息不是语素入口主信息类");
    }

    auto 目标类型 = 入口主信息->基础信息类型已确定()
        ? 入口主信息->对应基础信息类型
        : 语素主信息内部::推断人类词性默认基础信息类型(入口主信息->词性);

    if (目标类型 == 枚举_主信息类型::未定义) {
        目标类型 = 实际类型;
    }
    if (!语素主信息内部::人类词性是否兼容基础信息类型(入口主信息->词性, 实际类型) && 目标类型 != 实际类型) {
        const auto 默认类型 = 语素主信息内部::推断人类词性默认基础信息类型(入口主信息->词性);
        std::string 已绑定主键 = 入口主信息->对应基础信息.主键;
        std::string 已绑定类型 = "空";
        if (auto* 已绑定 = 入口主信息->对应基础信息.指针) {
            已绑定主键 = 已绑定->获取主键();
            已绑定类型 = 私有_主信息类型文本(私有_取基础信息类型(已绑定));
        }
        项目运行错误日志(
            "语素类/runtime_error | 添加对应基础信息 | 基础信息类型与词性映射类型不一致"
            " | 词=" + 私有_获取语素节点显示词(词节点) +
            " | 入口地址=" + std::to_string(reinterpret_cast<std::uintptr_t>(词节点)) +
            " | 人类词性=" + 枚举_词性_工厂::根据枚举类型获取文本(入口主信息->词性) +
            " | 信息入口=" + 私有_信息入口类型文本(入口主信息->信息入口类型) +
            " | 默认类型=" + 私有_主信息类型文本(默认类型) +
            " | 目标类型=" + 私有_主信息类型文本(目标类型) +
            " | 实际类型=" + 私有_主信息类型文本(实际类型) +
            " | 基础节点主键=" + 基础信息->获取主键() +
            " | 基础节点地址=" + std::to_string(reinterpret_cast<std::uintptr_t>(基础信息)) +
            " | 已绑定主键=" + (已绑定主键.empty() ? "空" : 已绑定主键) +
            " | 已绑定类型=" + 已绑定类型);
        throw std::runtime_error("语素类::添加对应基础信息 - 基础信息类型与词性映射类型不一致");
    }

    入口主信息->对应基础信息类型 = 实际类型;
    入口主信息->对应基础信息.绑定(基础信息);
    return 1;
}

// 功能：读取并返回指定对象、状态或运行材料。
std::string 语素类::获取词(const 语素入口节点类* 语素入口指针) const
{
    if (!语素入口指针) {
        throw std::invalid_argument("语素类::获取词 - 语素入口指针为空");
    }
    if (!语素入口指针->父 || !语素入口指针->父->主信息) {
        throw std::invalid_argument("语素类::获取词 - 语素入口指针没有有效父词节点");
    }

    auto* 词主信息 = dynamic_cast<词主信息类*>(语素入口指针->父->主信息);
    if (!词主信息) {
        项目运行错误日志("语素类/runtime_error | 获取词 | 父节点主信息不是词主信息类");
        throw std::runtime_error("语素类::获取词 - 父节点主信息不是词主信息类");
    }

    return 词主信息->词;
}

// 功能：安全读取语素入口词面；入口为空或读取异常时返回空文本。
std::string 语素类::安全获取词(const 语素入口节点类* 语素入口指针) const noexcept
{
    if (!语素入口指针) {
        return {};
    }
    try {
        return 获取词(语素入口指针);
    }
    catch (...) {
        return {};
    }
}

// 功能：清理缓存、临时状态或运行资源。
void 语素类::私有_清理主信息_已加锁() noexcept
{
    if (!根指针) return;

    for (auto* it = 根指针->链下; it && it != 根指针; it = it->链下) {
        delete it->主信息;
        it->主信息 = nullptr;
    }
}

// 功能：按条件查找目标对象、方法或事实。
词节点类* 语素类::查找词_已加锁(const std::string& 词字符串) const
{
    if (词字符串.empty() || !根指针 || 根指针->下 == 根指针) return nullptr;

    // 这里只按词典前缀路径查找词项；业务语义由其下语素入口和基础信息引用决定。
    链节点类* 当前父节点 = 根指针;
    std::string 前缀;
    前缀.reserve(词字符串.size());

    for (std::size_t i = 0; i < 词字符串.size(); ) {
        std::size_t step = 私有_UTF8字符长度(static_cast<unsigned char>(词字符串[i]));
        if (i + step > 词字符串.size()) step = 1;

        前缀.append(词字符串, i, step);
        i += step;

        词节点类* 命中节点 = nullptr;
        if (当前父节点 == 根指针) {
            for (auto* 游标 = 当前父节点->下; 游标 && 游标 != 当前父节点; 游标 = 游标->下) {
                auto* 主信息 = dynamic_cast<词主信息类*>(游标->主信息);
                if (主信息 && 主信息->词 == 前缀) {
                    命中节点 = static_cast<词节点类*>(游标);
                    break;
                }
            }
        }
        else {
            auto* 首子节点 = 当前父节点->子;
            if (!首子节点) return nullptr;

            auto* 游标 = 首子节点;
            do {
                auto* 主信息 = dynamic_cast<词主信息类*>(游标->主信息);
                if (主信息 && 主信息->词 == 前缀) {
                    命中节点 = static_cast<词节点类*>(游标);
                    break;
                }
                游标 = 游标->下;
            } while (游标 && 游标 != 首子节点);
        }

        if (!命中节点) return nullptr;
        当前父节点 = 命中节点;
    }

    return 当前父节点 == 根指针 ? nullptr : static_cast<词节点类*>(当前父节点);
}

// 功能：按条件查找目标对象、方法或事实。
语素入口节点类* 语素类::查找人类词性入口节点_已加锁(const 链节点类* 词节点, 枚举_词性 词性值) const
{
    if (!词节点 || !词节点->子) return nullptr;

    auto* 起点 = 词节点->子;
    auto* 当前节点 = 起点;
    do {
        auto* 主信息 = dynamic_cast<语素入口主信息类*>(当前节点->主信息);
        if (主信息 && 主信息->词性 == 词性值) {
            return static_cast<语素入口节点类*>(当前节点);
        }

        当前节点 = 当前节点->下;
    } while (当前节点 && 当前节点 != 起点);

    return nullptr;
}

// 功能：按条件查找目标对象、方法或事实。
语素入口节点类* 语素类::查找信息入口节点_已加锁(const 链节点类* 词节点, 枚举_信息入口类型 信息入口类型) const
{
    if (!词节点 || !词节点->子 || 信息入口类型 == 枚举_信息入口类型::未定义) return nullptr;

    auto* 起点 = 词节点->子;
    auto* 当前节点 = 起点;
    do {
        auto* 主信息 = dynamic_cast<语素入口主信息类*>(当前节点->主信息);
        if (主信息 && 主信息->信息入口类型 == 信息入口类型) {
            return static_cast<语素入口节点类*>(当前节点);
        }

        当前节点 = 当前节点->下;
    } while (当前节点 && 当前节点 != 起点);

    return nullptr;
}

// 功能：服务所在模块的内部辅助流程。
std::size_t 语素类::私有_UTF8字符长度(unsigned char c) noexcept
{
    if ((c & 0x80u) == 0) return 1;
    if ((c & 0xE0u) == 0xC0u) return 2;
    if ((c & 0xF0u) == 0xE0u) return 3;
    if ((c & 0xF8u) == 0xF0u) return 4;
    return 1;
}

// 功能：读取并返回指定对象、状态或运行材料。
std::string 枚举_词性_工厂::根据枚举类型获取文本(枚举_词性 词性值)
{
    const auto& 动态映射 = 私有_枚举到文本动态映射表();
    auto it = 动态映射.find(词性值);
    return it != 动态映射.end() ? it->second : "未知词性";
}

// 功能：读取并返回指定对象、状态或运行材料。
枚举_词性 枚举_词性_工厂::根据文本获取枚举值(const std::string& 文本)
{
    const auto& 映射表 = 私有_文本到枚举映射表();
    auto it = 映射表.find(文本);
    return it != 映射表.end() ? it->second : 枚举_词性::未定义;
}

// 功能：按函数名执行对应处理。
void 枚举_词性_工厂::添加动态映射(枚举_词性 词性值, const std::string& 文本描述)
{
    私有_枚举到文本动态映射表()[词性值] = 文本描述;
}

// 功能：按函数名执行对应处理。
void 枚举_词性_工厂::批量添加动态映射(const std::vector<std::pair<枚举_词性, std::string>>& 映射列表)
{
    for (const auto& 项 : 映射列表) {
        添加动态映射(项.first, 项.second);
    }
}

// 功能：按函数名执行对应处理。
void 枚举_词性_工厂::清除动态映射()
{
    auto& 动态映射 = 私有_枚举到文本动态映射表();
    动态映射.clear();
    for (const auto& 项 : 私有_文本到枚举映射表()) {
        动态映射.emplace(项.second, 项.first);
    }
}
