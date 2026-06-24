#include "需求类.h"

#include <algorithm>
#include <initializer_list>
#include <mutex>
#include <sstream>
#include <unordered_set>

#include "日志接入.h"
#include "二次特征类.h"
#include "方法类.h"
#include "任务类.h"
#include "语素类.h"
#include "世界树类.h"

import 数据库ADO模块;
import 自我类.特征定义;

namespace {
    template<class T节点>
    可解析引用<T节点> 私有_生成引用(T节点* 节点) noexcept
    {
        可解析引用<T节点> 输出{};
        输出.指针 = 节点;
        if (节点) {
            输出.主键 = 节点->获取主键();
        }
        return 输出;
    }

    struct 结构_需求树SQL行 {
        int 行号 = 0;
        std::string 节点主键{};
        std::string 父节点主键{};
        int 父节点行号 = 0;
        int 深度 = 0;
        int 同层序号 = 0;
        int 直接子数量 = 0;
        std::string 路径{};
        std::string 节点名称{};
        std::string 目标语义{};
        std::string 逻辑组织类型{};
        std::string 描述信息主键{};
        std::int64_t 统计创建时间 = 0;
        std::int64_t 统计最后观测时间 = 0;
        std::uint64_t 统计命中次数 = 0;
        bool 已截止 = false;
        bool 阻塞父任务执行 = false;
        std::string 需求主体主键{};
        std::string 需求场景主键{};
        std::string 被需求存在主键{};
        std::string 当前状态主键{};
        std::string 目标状态主键{};
        std::string 目标特征类型主键{};
        std::string 对应任务主键{};
        std::int64_t 满足关系 = 0;
        std::int64_t 安全权重 = 0;
        std::int64_t 服务权重 = 0;
        std::int64_t 累计安全结算 = 0;
        std::int64_t 累计服务结算 = 0;
        std::int64_t 需求有效截止 = 0;
        std::string 最近结算任务主键{};
        std::int64_t 最近结算时间 = 0;
    };

    std::mutex& 私有_需求树SQL投影互斥() noexcept
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
            }
            else {
                输出.push_back(字符);
            }
        }
        输出.push_back('\'');
        return 输出;
    }

    std::string 私有_SQL布尔(const bool 值)
    {
        return 值 ? "1" : "0";
    }

    template<class T节点>
    std::string 私有_引用主键(const 可解析引用<T节点>& 引用)
    {
        if (引用.指针) {
            return 引用.指针->获取主键();
        }
        return 引用.主键;
    }

    std::string 私有_引用主键(const 可解析引用<任务节点类>& 引用)
    {
        if (!引用.指针) {
            return 引用.主键;
        }
        const auto* 任务节点 = reinterpret_cast<const 任务类::节点类*>(引用.指针);
        return 任务节点 ? 任务节点->获取主键() : 引用.主键;
    }

    std::size_t 私有_需求直接子数量(const 需求类::节点类* 节点)
    {
        if (!节点 || !节点->子) {
            return 0;
        }
        auto* 首节点 = static_cast<const 需求类::节点类*>(节点->子);
        auto* 当前 = 首节点;
        std::size_t 数量 = 0;
        std::size_t 保护 = 0;
        do {
            ++数量;
            当前 = static_cast<const 需求类::节点类*>(当前->下);
            ++保护;
        } while (当前 && 当前 != 首节点 && 保护 < 100000);
        return 数量;
    }

    void 私有_收集需求树SQL行(
        const 需求类::节点类* 节点,
        const std::string& 父节点主键,
        const int 父节点行号,
        const int 深度,
        const int 同层序号,
        const std::string& 父路径,
        std::unordered_set<const 需求类::节点类*>& 已访问,
        std::vector<结构_需求树SQL行>& 行集)
    {
        if (!节点 || !已访问.insert(节点).second) {
            return;
        }

        const auto 节点主键 = 节点->获取主键();
        const auto 路径 = 父路径.empty()
            ? 节点主键
            : 父路径 + "/" + 节点主键;
        const auto 目标语义视图 = 需求类::需求目标语义视图(节点);
        const auto* 目标特征类型 = 需求类::读取需求目标特征类型(节点);
        auto 节点名称 = std::string{};
        if (深度 == 0) {
            节点名称 = "需求树";
        }
        else if (深度 == 1
            && 语素入口同一(目标特征类型, 自我特征定义类::类型_自我_安全值())) {
            节点名称 = "安全";
        }
        else if (深度 == 1
            && 语素入口同一(目标特征类型, 自我特征定义类::类型_自我_服务值())) {
            节点名称 = "服务";
        }
        else {
            节点名称 = 语素集.安全获取词(目标特征类型);
            if (节点名称.empty()) {
                节点名称 = "未命名需求";
            }
        }

        结构_需求树SQL行 行{};
        行.行号 = static_cast<int>(行集.size() + 1);
        行.节点主键 = 节点主键;
        行.父节点主键 = 父节点主键;
        行.父节点行号 = 父节点行号;
        行.深度 = 深度;
        行.同层序号 = 同层序号;
        行.直接子数量 = static_cast<int>(私有_需求直接子数量(节点));
        行.路径 = 路径;
        行.节点名称 = std::move(节点名称);
        行.目标语义 = 目标语义视图.语义名称 ? 目标语义视图.语义名称 : "";
        行.逻辑组织类型 = 需求类::逻辑组织需求类型文本(目标语义视图.逻辑组织类型);
        行.描述信息主键 = 节点->主信息.描述信息
            ? 节点->主信息.描述信息->获取主键()
            : std::string{};
        行.统计创建时间 = static_cast<std::int64_t>(节点->主信息.统计.创建时间);
        行.统计最后观测时间 = static_cast<std::int64_t>(节点->主信息.统计.最后观测时间);
        行.统计命中次数 = 节点->主信息.统计.命中次数;
        行.已截止 = 节点->主信息.需求有效截止 != 0;
        行.阻塞父任务执行 = 节点->主信息.是否阻塞父任务执行;
        行.需求主体主键 = 私有_引用主键(节点->主信息.需求主体);
        行.需求场景主键 = 私有_引用主键(节点->主信息.需求场景);
        行.被需求存在主键 = 私有_引用主键(节点->主信息.被需求存在);
        行.当前状态主键 = 私有_引用主键(节点->主信息.被需求当前状态);
        行.目标状态主键 = 私有_引用主键(节点->主信息.需求状态);
        行.目标特征类型主键 = 节点->主信息.目标特征类型缓存
            ? 节点->主信息.目标特征类型缓存->获取主键()
            : std::string{};
        行.对应任务主键 = 私有_引用主键(节点->主信息.对应任务);
        行.满足关系 = static_cast<std::int64_t>(节点->主信息.满足关系);
        行.安全权重 = 节点->主信息.安全权重;
        行.服务权重 = 节点->主信息.服务权重;
        行.累计安全结算 = 节点->主信息.累计安全结算;
        行.累计服务结算 = 节点->主信息.累计服务结算;
        行.需求有效截止 = static_cast<std::int64_t>(节点->主信息.需求有效截止);
        行.最近结算任务主键 = 节点->主信息.最近结算任务主键;
        行.最近结算时间 = static_cast<std::int64_t>(节点->主信息.最近结算时间);
        行集.push_back(std::move(行));

        if (!节点->子) {
            return;
        }
        auto* 首子节点 = static_cast<const 需求类::节点类*>(节点->子);
        auto* 当前子节点 = 首子节点;
        int 子序号 = 0;
        std::size_t 保护 = 0;
        do {
            私有_收集需求树SQL行(
                当前子节点,
                节点主键,
                行.行号,
                深度 + 1,
                子序号,
                路径,
                已访问,
                行集);
            当前子节点 = static_cast<const 需求类::节点类*>(当前子节点->下);
            ++子序号;
            ++保护;
        } while (当前子节点 && 当前子节点 != 首子节点 && 保护 < 100000);
    }

    std::string 私有_需求树SQL建库脚本()
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "IF DB_ID(N'鱼巢投影库') IS NULL CREATE DATABASE [鱼巢投影库];\n";
        return SQL.str();
    }

    std::string 私有_需求树SQL建表脚本()
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "IF SCHEMA_ID(N'鱼巢') IS NULL EXEC(N'CREATE SCHEMA [鱼巢]');\n"
            << "IF OBJECT_ID(N'[鱼巢].[需求树快照]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[需求树快照] (\n"
            << "    snapshot_id uniqueidentifier NOT NULL PRIMARY KEY,\n"
            << "    captured_at datetime2(3) NOT NULL,\n"
            << "    source_kind nvarchar(80) NOT NULL,\n"
            << "    source_reason nvarchar(300) NULL,\n"
            << "    root_key nvarchar(80) NULL,\n"
            << "    node_count int NOT NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[需求树节点]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[需求树节点] (\n"
            << "    id bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,\n"
            << "    snapshot_id uniqueidentifier NOT NULL,\n"
            << "    row_index int NOT NULL,\n"
            << "    node_key nvarchar(80) NOT NULL,\n"
            << "    parent_key nvarchar(80) NULL,\n"
            << "    parent_row_index int NULL,\n"
            << "    depth int NOT NULL,\n"
            << "    sibling_index int NOT NULL,\n"
            << "    direct_child_count int NOT NULL,\n"
            << "    path_text nvarchar(1000) NULL,\n"
            << "    node_name nvarchar(200) NULL,\n"
            << "    target_semantics nvarchar(120) NULL,\n"
            << "    logic_group_type nvarchar(120) NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'[鱼巢].[需求主信息]', N'U') IS NULL\n"
            << "CREATE TABLE [鱼巢].[需求主信息] (\n"
            << "    id bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,\n"
            << "    snapshot_id uniqueidentifier NOT NULL,\n"
            << "    node_key nvarchar(80) NOT NULL,\n"
            << "    node_row_index int NOT NULL CONSTRAINT DF_demand_main_info_node_row_index DEFAULT(0),\n"
            << "    description_key nvarchar(80) NULL,\n"
            << "    stat_created_time_us bigint NULL,\n"
            << "    stat_last_observed_time_us bigint NULL,\n"
            << "    stat_hit_count bigint NULL,\n"
            << "    is_closed bit NOT NULL,\n"
            << "    blocks_parent bit NOT NULL,\n"
            << "    subject_key nvarchar(80) NULL,\n"
            << "    scene_key nvarchar(80) NULL,\n"
            << "    target_host_key nvarchar(80) NULL,\n"
            << "    current_state_key nvarchar(80) NULL,\n"
            << "    target_state_key nvarchar(80) NULL,\n"
            << "    target_feature_key nvarchar(160) NULL,\n"
            << "    task_key nvarchar(80) NULL,\n"
            << "    relation_mask bigint NULL,\n"
            << "    safety_weight bigint NULL,\n"
            << "    service_weight bigint NULL,\n"
            << "    safety_settled bigint NULL,\n"
            << "    service_settled bigint NULL,\n"
            << "    valid_until_us bigint NULL,\n"
            << "    recent_settlement_task_key nvarchar(80) NULL,\n"
            << "    recent_settlement_time_us bigint NULL\n"
            << ");\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'parent_row_index') IS NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] ADD parent_row_index int NULL;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'node_name') IS NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] ADD node_name nvarchar(200) NULL;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求主信息]', N'description_key') IS NULL\n"
            << "    ALTER TABLE [鱼巢].[需求主信息] ADD description_key nvarchar(80) NULL;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求主信息]', N'stat_created_time_us') IS NULL\n"
            << "    ALTER TABLE [鱼巢].[需求主信息] ADD stat_created_time_us bigint NULL;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求主信息]', N'stat_last_observed_time_us') IS NULL\n"
            << "    ALTER TABLE [鱼巢].[需求主信息] ADD stat_last_observed_time_us bigint NULL;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求主信息]', N'stat_hit_count') IS NULL\n"
            << "    ALTER TABLE [鱼巢].[需求主信息] ADD stat_hit_count bigint NULL;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求主信息]', N'recent_settlement_task_key') IS NULL\n"
            << "    ALTER TABLE [鱼巢].[需求主信息] ADD recent_settlement_task_key nvarchar(80) NULL;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求主信息]', N'recent_settlement_time_us') IS NULL\n"
            << "    ALTER TABLE [鱼巢].[需求主信息] ADD recent_settlement_time_us bigint NULL;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'structure_role') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN structure_role;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'derived_method_key') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN derived_method_key;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'derived_causal_key') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN derived_causal_key;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'is_closed') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN is_closed;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'blocks_parent') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN blocks_parent;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'subject_key') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN subject_key;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'scene_key') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN scene_key;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'target_host_key') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN target_host_key;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'current_state_key') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN current_state_key;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'target_state_key') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN target_state_key;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'target_feature_key') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN target_feature_key;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'task_key') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN task_key;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'relation_mask') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN relation_mask;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'safety_weight') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN safety_weight;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'service_weight') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN service_weight;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'safety_settled') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN safety_settled;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'service_settled') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN service_settled;\n"
            << "IF COL_LENGTH(N'[鱼巢].[需求树节点]', N'valid_until_us') IS NOT NULL\n"
            << "    ALTER TABLE [鱼巢].[需求树节点] DROP COLUMN valid_until_us;\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_demand_tree_node_key' AND object_id = OBJECT_ID(N'[鱼巢].[需求树节点]'))\n"
            << "    CREATE INDEX IX_demand_tree_node_key ON [鱼巢].[需求树节点](node_key, parent_key);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_demand_tree_node_snapshot_key' AND object_id = OBJECT_ID(N'[鱼巢].[需求树节点]'))\n"
            << "    CREATE INDEX IX_demand_tree_node_snapshot_key ON [鱼巢].[需求树节点](snapshot_id, node_key, parent_key);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_demand_tree_node_parent_row' AND object_id = OBJECT_ID(N'[鱼巢].[需求树节点]'))\n"
            << "    CREATE INDEX IX_demand_tree_node_parent_row ON [鱼巢].[需求树节点](snapshot_id, parent_row_index, row_index);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_demand_main_info_node' AND object_id = OBJECT_ID(N'[鱼巢].[需求主信息]'))\n"
            << "    CREATE INDEX IX_demand_main_info_node ON [鱼巢].[需求主信息](snapshot_id, node_key);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_demand_main_info_node_row' AND object_id = OBJECT_ID(N'[鱼巢].[需求主信息]'))\n"
            << "    CREATE INDEX IX_demand_main_info_node_row ON [鱼巢].[需求主信息](snapshot_id, node_row_index);\n";
        return SQL.str();
    }

    std::string 私有_需求树SQL视图脚本()
    {
        std::ostringstream SQL;
        SQL << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前需求主信息] AS\n"
            << "SELECT m.*\n"
            << "FROM [鱼巢].[需求主信息] m\n"
            << "WHERE m.snapshot_id = (SELECT TOP (1) snapshot_id FROM [鱼巢].[需求树快照] ORDER BY captured_at DESC);');\n"
            << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前需求树节点] AS\n"
            << "SELECT\n"
            << "    n.id,\n"
            << "    n.snapshot_id,\n"
            << "    n.row_index,\n"
            << "    n.node_key,\n"
            << "    n.parent_key,\n"
            << "    n.parent_row_index,\n"
            << "    n.depth,\n"
            << "    n.sibling_index,\n"
            << "    n.direct_child_count,\n"
            << "    n.path_text,\n"
            << "    n.node_name,\n"
            << "    n.target_semantics,\n"
            << "    n.logic_group_type,\n"
            << "    m.id AS main_info_id,\n"
            << "    m.node_row_index,\n"
            << "    m.description_key,\n"
            << "    m.stat_created_time_us,\n"
            << "    m.stat_last_observed_time_us,\n"
            << "    m.stat_hit_count,\n"
            << "    m.is_closed,\n"
            << "    m.blocks_parent,\n"
            << "    m.subject_key,\n"
            << "    m.scene_key,\n"
            << "    m.target_host_key,\n"
            << "    m.current_state_key,\n"
            << "    m.target_state_key,\n"
            << "    m.target_feature_key,\n"
            << "    m.task_key,\n"
            << "    m.relation_mask,\n"
            << "    m.safety_weight,\n"
            << "    m.service_weight,\n"
            << "    m.safety_settled,\n"
            << "    m.service_settled,\n"
            << "    m.valid_until_us,\n"
            << "    m.recent_settlement_task_key,\n"
            << "    m.recent_settlement_time_us\n"
            << "FROM [鱼巢].[需求树节点] n\n"
            << "LEFT JOIN [鱼巢].[需求主信息] m\n"
            << "    ON m.snapshot_id = n.snapshot_id\n"
            << "    AND m.node_row_index = n.row_index\n"
            << "WHERE n.snapshot_id = (SELECT TOP (1) snapshot_id FROM [鱼巢].[需求树快照] ORDER BY captured_at DESC);');\n"
            << "EXEC(N'CREATE OR ALTER VIEW [鱼巢].[当前需求面板节点] AS\n"
            << "WITH demand_rows AS (\n"
            << "    SELECT *\n"
            << "    FROM [鱼巢].[当前需求树节点]\n"
            << "), enriched AS (\n"
            << "    SELECT\n"
            << "        d.*,\n"
            << "        COALESCE(NULLIF(subject.display_text, N''''), NULLIF(subject.name_text, N''''), d.subject_key) AS subject_display_name,\n"
            << "        COALESCE(NULLIF(subject.type_text, N''''), NULLIF(subject.main_type_text, N''''), NULLIF(subject.node_kind, N'''')) AS subject_type_name,\n"
            << "        COALESCE(NULLIF(scene.display_text, N''''), NULLIF(scene.name_text, N''''), d.scene_key) AS scene_display_name,\n"
            << "        COALESCE(NULLIF(scene.type_text, N''''), NULLIF(scene.main_type_text, N''''), NULLIF(scene.node_kind, N'''')) AS scene_type_name,\n"
            << "        COALESCE(NULLIF(target_host.display_text, N''''), NULLIF(target_host.name_text, N''''), d.target_host_key) AS target_host_display_name,\n"
            << "        COALESCE(NULLIF(target_host.type_text, N''''), NULLIF(target_host.main_type_text, N''''), NULLIF(target_host.node_kind, N'''')) AS target_host_type_name,\n"
            << "        COALESCE(NULLIF(current_state.display_text, N''''), NULLIF(current_state.name_text, N''''), d.current_state_key) AS current_state_display_name,\n"
            << "        COALESCE(NULLIF(current_state.type_text, N''''), NULLIF(current_state.main_type_text, N''''), NULLIF(current_state.node_kind, N'''')) AS current_state_type_name,\n"
            << "        COALESCE(NULLIF(target_state.display_text, N''''), NULLIF(target_state.name_text, N''''), d.target_state_key) AS target_state_display_name,\n"
            << "        COALESCE(NULLIF(target_state.type_text, N''''), NULLIF(target_state.main_type_text, N''''), NULLIF(target_state.node_kind, N'''')) AS target_state_type_name,\n"
            << "        COALESCE(NULLIF(target_feature.display_text, N''''), NULLIF(target_feature.word_text, N''''), d.target_feature_key) AS target_feature_display_name,\n"
            << "        COALESCE(NULLIF(target_feature.entry_type_text, N''''), NULLIF(target_feature.mapped_main_type_text, N''''), NULLIF(target_feature.node_kind, N'''')) AS target_feature_type_name,\n"
            << "        COALESCE(NULLIF(task.name_text, N''''), NULLIF(task.type_text, N''''), d.task_key) AS task_display_name,\n"
            << "        COALESCE(NULLIF(task.type_text, N''''), NULLIF(task.node_kind_text, N''''), NULLIF(task.task_state_text, N'''')) AS task_type_name,\n"
            << "        COALESCE(NULLIF(recent_task.name_text, N''''), NULLIF(recent_task.type_text, N''''), d.recent_settlement_task_key) AS recent_settlement_task_display_name,\n"
            << "        COALESCE(NULLIF(recent_task.type_text, N''''), NULLIF(recent_task.node_kind_text, N''''), NULLIF(recent_task.task_state_text, N'''')) AS recent_settlement_task_type_name\n"
            << "    FROM demand_rows d\n"
            << "    LEFT JOIN [鱼巢].[当前世界树节点] subject\n"
            << "        ON subject.node_key COLLATE Latin1_General_BIN2 = d.subject_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN [鱼巢].[当前世界树节点] scene\n"
            << "        ON scene.node_key COLLATE Latin1_General_BIN2 = d.scene_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN [鱼巢].[当前世界树节点] target_host\n"
            << "        ON target_host.node_key COLLATE Latin1_General_BIN2 = d.target_host_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN [鱼巢].[当前世界树节点] current_state\n"
            << "        ON current_state.node_key COLLATE Latin1_General_BIN2 = d.current_state_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN [鱼巢].[当前世界树节点] target_state\n"
            << "        ON target_state.node_key COLLATE Latin1_General_BIN2 = d.target_state_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN [鱼巢].[当前语素节点] target_feature\n"
            << "        ON target_feature.node_key COLLATE Latin1_General_BIN2 = d.target_feature_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN [鱼巢].[当前任务树节点] task\n"
            << "        ON task.node_key COLLATE Latin1_General_BIN2 = d.task_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN [鱼巢].[当前任务树节点] recent_task\n"
            << "        ON recent_task.node_key COLLATE Latin1_General_BIN2 = d.recent_settlement_task_key COLLATE Latin1_General_BIN2\n"
            << ")\n"
            << "SELECT\n"
            << "    row_index,\n"
            << "    node_key,\n"
            << "    parent_key,\n"
            << "    parent_row_index,\n"
            << "    CONVERT(nvarchar(20), row_index) AS display_node_id,\n"
            << "    CASE WHEN depth = 1 THEN NULL ELSE CONVERT(nvarchar(20), parent_row_index) END AS display_parent_id,\n"
            << "    CASE WHEN depth > 0 THEN depth - 1 ELSE 0 END AS display_depth,\n"
            << "    node_name,\n"
            << "    CASE WHEN COALESCE(direct_child_count, 0) > 0\n"
            << "        THEN CONCAT(N''有子='', CONVERT(nvarchar(20), direct_child_count))\n"
            << "        ELSE N''叶子''\n"
            << "    END AS tree_shape,\n"
            << "    target_semantics,\n"
            << "    logic_group_type,\n"
            << "    target_feature_display_name AS target_feature_display,\n"
            << "    target_feature_type_name AS target_feature_type,\n"
            << "    task_display_name AS task_display,\n"
            << "    task_type_name AS task_type,\n"
            << "    description_key,\n"
            << "    stat_created_time_us,\n"
            << "    stat_last_observed_time_us,\n"
            << "    stat_hit_count,\n"
            << "    is_closed,\n"
            << "    blocks_parent,\n"
            << "    subject_key,\n"
            << "    subject_display_name,\n"
            << "    subject_type_name,\n"
            << "    scene_key,\n"
            << "    scene_display_name,\n"
            << "    scene_type_name,\n"
            << "    target_host_key,\n"
            << "    target_host_display_name,\n"
            << "    target_host_type_name,\n"
            << "    current_state_key,\n"
            << "    current_state_display_name,\n"
            << "    current_state_type_name,\n"
            << "    target_state_key,\n"
            << "    target_state_display_name,\n"
            << "    target_state_type_name,\n"
            << "    target_feature_key,\n"
            << "    target_feature_display_name,\n"
            << "    target_feature_type_name,\n"
            << "    task_key,\n"
            << "    task_display_name,\n"
            << "    task_type_name,\n"
            << "    relation_mask,\n"
            << "    safety_weight,\n"
            << "    service_weight,\n"
            << "    safety_settled,\n"
            << "    service_settled,\n"
            << "    valid_until_us,\n"
            << "    recent_settlement_task_key,\n"
            << "    recent_settlement_task_display_name,\n"
            << "    recent_settlement_task_type_name,\n"
            << "    recent_settlement_time_us\n"
            << "FROM enriched\n"
            << "WHERE depth > 0;');\n";
        return SQL.str();
    }

    std::string 私有_构造需求树SQL重写脚本(
        const std::vector<结构_需求树SQL行>& 行集,
        const std::string& 来源原因,
        const std::string& 根主键)
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "SET XACT_ABORT ON;\n"
            << "BEGIN TRANSACTION;\n"
            << "DELETE FROM [鱼巢].[需求主信息];\n"
            << "DELETE FROM [鱼巢].[需求树节点];\n"
            << "DELETE FROM [鱼巢].[需求树快照];\n"
            << "DECLARE @snapshot_id uniqueidentifier = NEWID();\n"
            << "INSERT INTO [鱼巢].[需求树快照] (snapshot_id, captured_at, source_kind, source_reason, root_key, node_count)\n"
            << "VALUES (@snapshot_id, SYSUTCDATETIME(), N'demand_tree_update', "
            << 私有_SQL字符串(来源原因)
            << ", "
            << 私有_SQL字符串(根主键)
            << ", "
            << 行集.size()
            << ");\n";
        for (const auto& 行 : 行集) {
            SQL << "INSERT INTO [鱼巢].[需求树节点] (snapshot_id, row_index, node_key, parent_key, parent_row_index, depth, sibling_index, direct_child_count, path_text, node_name, target_semantics, logic_group_type) VALUES (@snapshot_id, "
                << 行.行号 << ", "
                << 私有_SQL字符串(行.节点主键, false) << ", "
                << 私有_SQL字符串(行.父节点主键) << ", "
                << (行.父节点行号 > 0 ? std::to_string(行.父节点行号) : std::string("NULL")) << ", "
                << 行.深度 << ", "
                << 行.同层序号 << ", "
                << 行.直接子数量 << ", "
                << 私有_SQL字符串(行.路径) << ", "
                << 私有_SQL字符串(行.节点名称) << ", "
                << 私有_SQL字符串(行.目标语义) << ", "
                << 私有_SQL字符串(行.逻辑组织类型) << ");\n";
            SQL << "INSERT INTO [鱼巢].[需求主信息] (snapshot_id, node_key, node_row_index, description_key, stat_created_time_us, stat_last_observed_time_us, stat_hit_count, is_closed, blocks_parent, subject_key, scene_key, target_host_key, current_state_key, target_state_key, target_feature_key, task_key, relation_mask, safety_weight, service_weight, safety_settled, service_settled, valid_until_us, recent_settlement_task_key, recent_settlement_time_us) VALUES (@snapshot_id, "
                << 私有_SQL字符串(行.节点主键, false) << ", "
                << 行.行号 << ", "
                << 私有_SQL字符串(行.描述信息主键) << ", "
                << 行.统计创建时间 << ", "
                << 行.统计最后观测时间 << ", "
                << 行.统计命中次数 << ", "
                << 私有_SQL布尔(行.已截止) << ", "
                << 私有_SQL布尔(行.阻塞父任务执行) << ", "
                << 私有_SQL字符串(行.需求主体主键) << ", "
                << 私有_SQL字符串(行.需求场景主键) << ", "
                << 私有_SQL字符串(行.被需求存在主键) << ", "
                << 私有_SQL字符串(行.当前状态主键) << ", "
                << 私有_SQL字符串(行.目标状态主键) << ", "
                << 私有_SQL字符串(行.目标特征类型主键) << ", "
                << 私有_SQL字符串(行.对应任务主键) << ", "
                << 行.满足关系 << ", "
                << 行.安全权重 << ", "
                << 行.服务权重 << ", "
                << 行.累计安全结算 << ", "
                << 行.累计服务结算 << ", "
                << 行.需求有效截止 << ", "
                << 私有_SQL字符串(行.最近结算任务主键) << ", "
                << 行.最近结算时间 << ");\n";
        }
        SQL << "IF EXISTS (\n"
            << "    SELECT 1\n"
            << "    FROM [鱼巢].[需求树节点] child\n"
            << "    LEFT JOIN [鱼巢].[需求树节点] parent\n"
            << "        ON parent.snapshot_id = child.snapshot_id AND parent.row_index = child.parent_row_index\n"
            << "    WHERE child.snapshot_id = @snapshot_id\n"
            << "        AND child.parent_row_index IS NOT NULL\n"
            << "        AND parent.row_index IS NULL\n"
            << ")\n"
            << "    THROW 51002, N'demand_tree_node parent_row_index invalid', 1;\n"
            << "IF EXISTS (\n"
            << "    SELECT 1\n"
            << "    FROM [鱼巢].[需求主信息] info\n"
            << "    LEFT JOIN [鱼巢].[需求树节点] node\n"
            << "        ON node.snapshot_id = info.snapshot_id AND node.row_index = info.node_row_index\n"
            << "    WHERE info.snapshot_id = @snapshot_id AND node.row_index IS NULL\n"
            << ")\n"
            << "    THROW 51001, N'demand_main_info node_row_index invalid', 1;\n";
        SQL << "COMMIT TRANSACTION;\n";
        return SQL.str();
    }

    bool 私有_执行需求树ADO命令(
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

    // 功能：解析输入文本、消息、场景或运行包。
    状态节点类* 私有_解析状态引用(const 可解析引用<状态节点类>& 引用) noexcept
    {
        if (引用.指针) {
            return 引用.指针;
        }
        if (引用.主键.empty()) {
            return nullptr;
        }
        return 世界树.按主键解析状态节点(引用.主键);
    }

    // 功能：解析输入文本、消息、场景或运行包。
    存在节点类* 私有_解析存在引用(const 可解析引用<存在节点类>& 引用) noexcept
    {
        if (引用.指针) {
            return 引用.指针;
        }
        if (引用.主键.empty()) {
            return nullptr;
        }
        return 世界树.按主键解析存在节点(引用.主键);
    }

    // 功能：解析输入文本、消息、场景或运行包。
    场景节点类* 私有_解析场景引用(const 可解析引用<场景节点类>& 引用) noexcept
    {
        if (引用.指针) {
            return 引用.指针;
        }
        if (引用.主键.empty()) {
            return nullptr;
        }
        return 世界树.按主键解析场景节点(引用.主键);
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_自检维护管理状态词(const char* 名称) noexcept
    {
        return 名称
            ? 语素集.添加信息入口词(名称, 枚举_信息入口类型::特征模板入口)
            : nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_目标特征是自检维护管理状态(const 语素入口节点类* 目标特征类型) noexcept
    {
        if (!目标特征类型) {
            return false;
        }
        return 语素入口同一(
                目标特征类型,
                私有_自检维护管理状态词("学习维护管理状态"))
            || 语素入口同一(
                目标特征类型,
                私有_自检维护管理状态词("方法维护管理状态"))
            || 语素入口同一(
                目标特征类型,
                私有_自检维护管理状态词("世界模型维护管理状态"))
            || 语素入口同一(
                目标特征类型,
                私有_自检维护管理状态词("自我结构维护管理状态"));
    }

    template<class T节点>
    std::string 私有_引用主键_需求方向(const 可解析引用<T节点>& 引用) noexcept
    {
        if (!引用.主键.empty()) {
            return 引用.主键;
        }
        return 引用.指针 ? 引用.指针->获取主键() : std::string{};
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_状态主体主键_需求方向(const 状态节点类* 状态) noexcept
    {
        可解析引用<基础信息节点类> 状态主体{};
        可解析引用<特征节点类> 状态特征{};
        特征值 状态值{};
        return 状态类::读取状态主体特征和值引用(状态, 状态主体, 状态特征, 状态值)
            ? 私有_引用主键_需求方向(状态主体)
            : std::string{};
    }

    // 功能：服务所在模块的内部辅助流程。
    场景节点类* 私有_取所属场景(状态节点类* 状态节点) noexcept
    {
        if (!状态节点) {
            return nullptr;
        }
        return dynamic_cast<场景节点类*>(世界树.获取父节点(状态节点));
    }

    // 功能：解析输入文本、消息、场景或运行包。
    状态节点类* 私有_解析状态节点_需求校验(
        const std::uintptr_t 指针,
        const std::string& 主键) noexcept
    {
        if (指针 != 0) {
            auto* 候选 = reinterpret_cast<状态节点类*>(指针);
            可解析引用<基础信息节点类> 状态主体{};
            可解析引用<特征节点类> 状态特征{};
            特征值 状态值{};
            if (状态类::读取状态主体特征和值引用(候选, 状态主体, 状态特征, 状态值)) {
                return 候选;
            }
        }
        if (主键.empty()) {
            return nullptr;
        }
        auto* 状态节点 = 世界树.按主键解析状态节点(主键);
        可解析引用<基础信息节点类> 状态主体{};
        可解析引用<特征节点类> 状态特征{};
        特征值 状态值{};
        return 状态类::读取状态主体特征和值引用(状态节点, 状态主体, 状态特征, 状态值)
            ? 状态节点
            : nullptr;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    特征节点类* 私有_解析状态特征节点_需求校验(
        const 状态节点类* 状态) noexcept
    {
        可解析引用<基础信息节点类> 状态主体{};
        可解析引用<特征节点类> 状态特征{};
        特征值 状态值{};
        if (!状态类::读取状态主体特征和值引用(状态, 状态主体, 状态特征, 状态值)) {
            return nullptr;
        }
        auto* 特征节点 = 状态特征.指针;
        if (!特征节点 && !状态特征.主键.empty()) {
            特征节点 = 世界树.按主键解析特征节点(状态特征.主键);
        }
        return 特征类::读取特征语义类型_直接(特征节点) ? 特征节点 : nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_状态特征类型_需求校验(
        const 状态节点类* 状态) noexcept
    {
        return 特征类::读取特征语义类型_直接(
            私有_解析状态特征节点_需求校验(状态));
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_状态特征类型主键_需求方向(const 状态节点类* 状态) noexcept
    {
        const auto* 特征类型 = 私有_状态特征类型_需求校验(状态);
        return 特征类型 ? 特征类型->获取主键() : std::string{};
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_方向掩码反向_需求方向(
        const 三向关系掩码 左,
        const 三向关系掩码 右) noexcept
    {
        const bool 左增 = (左 & 关系_大于) != 0;
        const bool 左减 = (左 & 关系_小于) != 0;
        const bool 右增 = (右 & 关系_大于) != 0;
        const bool 右减 = (右 & 关系_小于) != 0;
        return (左增 && 右减) || (左减 && 右增);
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_方向区间反向_需求方向(
        const 二次特征主信息类::枚举_方向区间 左,
        const 二次特征主信息类::枚举_方向区间 右) noexcept
    {
        using 枚举方向区间 = 二次特征主信息类::枚举_方向区间;
        return (左 == 枚举方向区间::正区间 && 右 == 枚举方向区间::负区间)
            || (左 == 枚举方向区间::负区间 && 右 == 枚举方向区间::正区间);
    }

    // 功能：服务所在模块的内部辅助流程。
    int 私有_I64符号_需求方向(const I64 值) noexcept
    {
        if (值 > 0) {
            return 1;
        }
        if (值 < 0) {
            return -1;
        }
        return 0;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_方法树存在生产者(
        const 语素入口节点类* 目标特征类型) noexcept
    {
        auto* 自我存在 = 世界树.自我指针;
        auto* 方法根节点 = 自我存在 ? 世界树.获取方法根节点(自我存在) : nullptr;
        if (!方法根节点 || !目标特征类型) {
            return false;
        }
        return 方法类::方法树存在生产目标特征(方法根节点, 目标特征类型);
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_目标特征是逻辑流程产物(
        const 语素入口节点类* 目标特征类型) noexcept
    {
        return 需求类::目标特征类型是逻辑组织目标(目标特征类型)
            || (目标特征类型 && 语素入口同一(
                目标特征类型,
                需求类::OR组结算令牌目标特征类型()));
    }

    // 功能：服务所在模块的内部辅助流程。
    枚举_枚举目标生产者分级 私有_查询枚举目标生产者分级(
        const 语素入口节点类* 目标特征类型) noexcept
    {
        if (!目标特征类型) {
            return 枚举_枚举目标生产者分级::未登记;
        }
        if (私有_目标特征是逻辑流程产物(目标特征类型)) {
            return 枚举_枚举目标生产者分级::通用流程生产;
        }
        if (私有_方法树存在生产者(目标特征类型)) {
            return 枚举_枚举目标生产者分级::专用方法生产;
        }
        return 枚举_枚举目标生产者分级::未登记;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_生产者分级已有生产者(枚举_枚举目标生产者分级 分级) noexcept
    {
        return 分级 == 枚举_枚举目标生产者分级::专用方法生产
            || 分级 == 枚举_枚举目标生产者分级::通用流程生产
            || 分级 == 枚举_枚举目标生产者分级::规则生产
            || 分级 == 枚举_枚举目标生产者分级::初始化生产;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_特征_方法状态_需求校验() noexcept
    {
        static const 语素入口节点类* s_词 =
            语素集.添加信息入口词("方法状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 功能：比较当前值、目标值或历史基准。
    bool 私有_枚举目标允许有序比较(const 语素入口节点类* 目标特征类型) noexcept
    {
        return 目标特征类型 && 语素入口同一(
            目标特征类型,
            私有_特征_方法状态_需求校验());
    }

    // 功能：比较当前值、目标值或历史基准。
    bool 私有_枚举目标比较符合法(
        const 语素入口节点类* 目标特征类型,
        三向关系掩码 满足关系) noexcept
    {
        const auto 关系 = 满足关系 != 0 ? 满足关系 : 关系_等于;
        if (关系 == 关系_等于) {
            return true;
        }
        if (!私有_枚举目标允许有序比较(目标特征类型)) {
            return false;
        }
        return (关系 & 关系_大于) != 0
            && (关系 & 关系_等于) != 0
            && (关系 & 关系_小于) == 0;
    }

    // 功能：服务所在模块的内部辅助流程。
    std::string 私有_构造枚举目标校验摘要(const 结构_枚举目标校验结果& 结果)
    {
        std::ostringstream 输出;
        输出 << "枚举目标校验"
            << " | 是枚举=" << (结果.是枚举目标 ? 1 : 0)
            << " | 合法=" << (结果.合法 ? 1 : 0)
            << " | 比较符合法=" << (结果.比较符合法 ? 1 : 0)
            << " | 值域命中=" << (结果.目标值属于值域 ? 1 : 0)
            << " | 有生产者=" << (结果.有合法生产者 ? 1 : 0)
            << " | 生产者分级=" << 需求类::枚举目标生产者分级文本(结果.生产者分级)
            << " | 目标特征=" << (结果.目标特征类型 ? 结果.目标特征类型->获取主键() : std::string("空"))
            << " | 结论=" << 需求类::枚举目标校验结论文本(结果.结论);
        return 输出.str();
    }

    // 功能：根据二次特征方向签名材料生成需求方向签名。
    需求类::结构_需求方向签名 私有_生成需求方向签名_从材料(
        const 结构_二次特征方向签名材料& 材料) noexcept
    {
        需求类::结构_需求方向签名 签名{};
        auto* 结果状态 = 私有_解析状态引用(材料.来源结果状态);
        auto* 起始状态 = 私有_解析状态引用(材料.来源起始状态);
        const auto* 目标状态 = 结果状态 ? 结果状态 : 起始状态;
        if (目标状态) {
            签名.目标主体主键 = 私有_状态主体主键_需求方向(目标状态);
            签名.目标特征类型主键 = 私有_状态特征类型主键_需求方向(目标状态);
        }
        if (签名.目标主体主键.empty()) {
            签名.目标主体主键 = 私有_引用主键_需求方向(材料.主体);
        }
        if (签名.目标特征类型主键.empty() && 起始状态) {
            签名.目标特征类型主键 = 私有_状态特征类型主键_需求方向(起始状态);
        }

        签名.方向掩码 = 材料.允许方向;
        签名.方向区间 = 材料.方向区间;
        if (材料.差值.has_value()) {
            签名.有差值 = true;
            签名.差值 = *材料.差值;
            if (签名.方向区间 == 二次特征主信息类::枚举_方向区间::未定义) {
                签名.方向区间 =
                    二次特征主信息类::由差值判断方向区间(签名.差值);
            }
        }
        if (材料.有标量值) {
            签名.有标量值 = true;
            签名.标量值 = 材料.标量值;
        }
        return 签名;
    }

    // 功能：刷新父级需求结算摘要，不修改需求树结构。
    void 私有_刷新父级需求结算摘要_需求类(需求类::节点类* 起点需求) noexcept
    {
        auto* 父需求 = 起点需求 ? reinterpret_cast<需求类::节点类*>(起点需求->父) : nullptr;
        while (父需求 && 父需求->父) {
            I64 子安全合计 = 0;
            I64 子服务合计 = 0;
            if (auto* 首子 = reinterpret_cast<需求类::节点类*>(父需求->子)) {
                auto* 子 = 首子;
                do {
                    if (子) {
                        子安全合计 += (std::max<I64>)(0, 子->主信息.累计安全结算);
                        子服务合计 += (std::max<I64>)(0, 子->主信息.累计服务结算);
                    }
                    子 = 子 ? reinterpret_cast<需求类::节点类*>(子->下) : nullptr;
                } while (子 && 子 != 首子);
            }
            父需求->主信息.累计安全结算 =
                (std::min<I64>)((std::max<I64>)(0, 父需求->主信息.安全权重), 子安全合计);
            父需求->主信息.累计服务结算 =
                (std::min<I64>)((std::max<I64>)(0, 父需求->主信息.服务权重), 子服务合计);
            父需求->主信息.最近结算时间 = 起点需求->主信息.最近结算时间;
            父需求->主信息.最近结算任务主键 = 起点需求->主信息.最近结算任务主键;
            父需求 = reinterpret_cast<需求类::节点类*>(父需求->父);
        }
    }
}

// 功能：按函数名执行对应处理。
const char* 需求类::逻辑组织需求类型文本(
    枚举_逻辑组织需求类型 类型) noexcept
{
    switch (类型) {
    case 枚举_逻辑组织需求类型::AND组: return "AND组";
    case 枚举_逻辑组织需求类型::OR组: return "OR组";
    case 枚举_逻辑组织需求类型::方法路径组: return "方法路径组";
    case 枚举_逻辑组织需求类型::因果子链支撑: return "因果子链支撑";
    case 枚举_逻辑组织需求类型::未定义:
    default:
        return "未定义";
    }
}

// 功能：按函数名执行对应处理。
const 语素入口节点类* 需求类::逻辑组织目标特征类型(
    枚举_逻辑组织需求类型 类型) noexcept
{
    const char* 特征名 = nullptr;
    switch (类型) {
    case 枚举_逻辑组织需求类型::AND组:
        特征名 = "AND组满足状态";
        break;
    case 枚举_逻辑组织需求类型::OR组:
        特征名 = "OR组满足状态";
        break;
    case 枚举_逻辑组织需求类型::方法路径组:
        特征名 = "方法路径完成状态";
        break;
    case 枚举_逻辑组织需求类型::因果子链支撑:
        特征名 = "因果子链支撑状态";
        break;
    case 枚举_逻辑组织需求类型::未定义:
    default:
        return nullptr;
    }
    return 语素集.添加信息入口词(特征名, 枚举_信息入口类型::特征模板入口);
}

// 功能：结算任务、需求、价值或结果贡献。
const 语素入口节点类* 需求类::OR组结算令牌目标特征类型() noexcept
{
    return 语素集.添加信息入口词("OR组结算令牌状态", 枚举_信息入口类型::特征模板入口);
}

// 功能：按函数名执行对应处理。
需求类::枚举_逻辑组织需求类型 需求类::逻辑组织需求类型_按目标特征类型(
    const 语素入口节点类* 目标特征类型) noexcept
{
    if (!目标特征类型) {
        return 枚举_逻辑组织需求类型::未定义;
    }
    if (语素入口同一(
            目标特征类型,
            逻辑组织目标特征类型(枚举_逻辑组织需求类型::AND组))) {
        return 枚举_逻辑组织需求类型::AND组;
    }
    if (语素入口同一(
            目标特征类型,
            逻辑组织目标特征类型(枚举_逻辑组织需求类型::OR组))) {
        return 枚举_逻辑组织需求类型::OR组;
    }
    if (语素入口同一(
            目标特征类型,
            逻辑组织目标特征类型(枚举_逻辑组织需求类型::方法路径组))) {
        return 枚举_逻辑组织需求类型::方法路径组;
    }
    if (语素入口同一(
            目标特征类型,
            逻辑组织目标特征类型(枚举_逻辑组织需求类型::因果子链支撑))) {
        return 枚举_逻辑组织需求类型::因果子链支撑;
    }
    if (私有_目标特征是自检维护管理状态(目标特征类型)) {
        return 枚举_逻辑组织需求类型::方法路径组;
    }
    return 枚举_逻辑组织需求类型::未定义;
}

// 功能：按函数名执行对应处理。
bool 需求类::目标特征类型是逻辑组织目标(
    const 语素入口节点类* 目标特征类型) noexcept
{
    return 逻辑组织需求类型_按目标特征类型(目标特征类型)
        != 枚举_逻辑组织需求类型::未定义;
}

// 功能：按函数名执行对应处理。
const char* 需求类::逻辑组织目标语义文本(
    const 语素入口节点类* 目标特征类型) noexcept
{
    switch (逻辑组织需求类型_按目标特征类型(目标特征类型)) {
    case 枚举_逻辑组织需求类型::AND组:
        return "AND组满足状态";
    case 枚举_逻辑组织需求类型::OR组:
        return "OR组满足状态";
    case 枚举_逻辑组织需求类型::方法路径组:
        return "方法路径完成状态";
    case 枚举_逻辑组织需求类型::因果子链支撑:
        return "因果子链支撑状态";
    case 枚举_逻辑组织需求类型::未定义:
    default:
        return "";
    }
}

// 功能：按函数名执行对应处理。
需求类::枚举_需求目标语义 需求类::需求目标语义_按目标特征类型(
    const 语素入口节点类* 目标特征类型) noexcept
{
    switch (逻辑组织需求类型_按目标特征类型(目标特征类型)) {
    case 枚举_逻辑组织需求类型::AND组:
        return 枚举_需求目标语义::AND组满足;
    case 枚举_逻辑组织需求类型::OR组:
        return 枚举_需求目标语义::OR组满足;
    case 枚举_逻辑组织需求类型::方法路径组:
        return 枚举_需求目标语义::方法路径完成;
    case 枚举_逻辑组织需求类型::因果子链支撑:
        return 枚举_需求目标语义::因果子链支撑;
    case 枚举_逻辑组织需求类型::未定义:
    default:
        break;
    }
    if (目标特征类型 && 语素入口同一(
            目标特征类型,
            OR组结算令牌目标特征类型())) {
        return 枚举_需求目标语义::OR组结算令牌;
    }
    return 目标特征类型
        ? 枚举_需求目标语义::普通可任务化目标
        : 枚举_需求目标语义::目标特征未解析;
}

// 功能：按函数名执行对应处理。
const char* 需求类::需求目标语义文本(
    枚举_需求目标语义 语义) noexcept
{
    switch (语义) {
    case 枚举_需求目标语义::普通可任务化目标:
        return "普通可任务化目标";
    case 枚举_需求目标语义::AND组满足:
        return "AND组满足状态";
    case 枚举_需求目标语义::OR组满足:
        return "OR组满足状态";
    case 枚举_需求目标语义::方法路径完成:
        return "方法路径完成状态";
    case 枚举_需求目标语义::因果子链支撑:
        return "因果子链支撑状态";
    case 枚举_需求目标语义::OR组结算令牌:
        return "OR组结算令牌状态";
    case 枚举_需求目标语义::目标特征未解析:
    default:
        return "目标特征未解析";
    }
}

// 功能：读取并返回指定对象、状态或运行材料。
const char* 需求类::需求获取途径文本(
    枚举_需求获取途径 途径) noexcept
{
    switch (途径) {
    case 枚举_需求获取途径::普通目标直接生成:
        return "普通目标直接生成";
    case 枚举_需求获取途径::任务筹办原子缺口:
        return "任务筹办原子缺口";
    case 枚举_需求获取途径::任务执行回执原子缺口:
        return "任务执行回执原子缺口";
    case 枚举_需求获取途径::自检原子缺口:
        return "自检原子缺口";
    case 枚举_需求获取途径::观察反馈原子缺口:
        return "观察反馈原子缺口";
    case 枚举_需求获取途径::结算后处理原子目标:
        return "结算后处理原子目标";
    case 枚举_需求获取途径::复合目标AND组生成:
        return "复合目标AND组生成";
    case 枚举_需求获取途径::多候选替代路径组生成:
        return "多候选替代路径组生成";
    case 枚举_需求获取途径::方法能力路径组生成:
        return "方法能力路径组生成";
    case 枚举_需求获取途径::因果链支撑组生成:
        return "因果链支撑组生成";
    case 枚举_需求获取途径::自检因果影响链生成:
        return "自检因果影响链生成";
    case 枚举_需求获取途径::OR组路径完成令牌提交:
        return "OR组路径完成令牌提交";
    case 枚举_需求获取途径::未定义:
    default:
        return "未定义";
    }
}

// 功能：按函数名执行对应处理。
需求类::结构_需求目标语义视图 需求类::需求目标语义视图(
    const 语素入口节点类* 目标特征类型) noexcept
{
    结构_需求目标语义视图 视图{};
    视图.目标特征类型 = 目标特征类型;
    视图.语义 = 需求目标语义_按目标特征类型(目标特征类型);
    视图.逻辑组织类型 = 逻辑组织需求类型_按目标特征类型(目标特征类型);
    视图.语义名称 = 需求目标语义文本(视图.语义);
    视图.是逻辑组织目标 = 视图.逻辑组织类型 != 枚举_逻辑组织需求类型::未定义;
    视图.是OR组目标 = 视图.逻辑组织类型 == 枚举_逻辑组织需求类型::OR组;
    视图.是结算令牌目标 = 视图.语义 == 枚举_需求目标语义::OR组结算令牌;
    视图.可进入普通候选方法筹办 =
        视图.语义 == 枚举_需求目标语义::普通可任务化目标;
    return 视图;
}

// 功能：读取并返回指定对象、状态或运行材料。
需求类::枚举_需求目标语义 需求类::需求目标语义_按获取途径(
    枚举_需求获取途径 途径) noexcept
{
    switch (途径) {
    case 枚举_需求获取途径::普通目标直接生成:
    case 枚举_需求获取途径::任务筹办原子缺口:
    case 枚举_需求获取途径::任务执行回执原子缺口:
    case 枚举_需求获取途径::自检原子缺口:
    case 枚举_需求获取途径::观察反馈原子缺口:
    case 枚举_需求获取途径::结算后处理原子目标:
        return 枚举_需求目标语义::普通可任务化目标;
    case 枚举_需求获取途径::复合目标AND组生成:
        return 枚举_需求目标语义::AND组满足;
    case 枚举_需求获取途径::多候选替代路径组生成:
        return 枚举_需求目标语义::OR组满足;
    case 枚举_需求获取途径::方法能力路径组生成:
        return 枚举_需求目标语义::方法路径完成;
    case 枚举_需求获取途径::因果链支撑组生成:
    case 枚举_需求获取途径::自检因果影响链生成:
        return 枚举_需求目标语义::因果子链支撑;
    case 枚举_需求获取途径::OR组路径完成令牌提交:
        return 枚举_需求目标语义::OR组结算令牌;
    case 枚举_需求获取途径::未定义:
    default:
        return 枚举_需求目标语义::目标特征未解析;
    }
}

// 功能：读取并返回指定对象、状态或运行材料。
需求类::枚举_逻辑组织需求类型 需求类::逻辑组织需求类型_按获取途径(
    枚举_需求获取途径 途径) noexcept
{
    switch (途径) {
    case 枚举_需求获取途径::复合目标AND组生成:
        return 枚举_逻辑组织需求类型::AND组;
    case 枚举_需求获取途径::多候选替代路径组生成:
        return 枚举_逻辑组织需求类型::OR组;
    case 枚举_需求获取途径::方法能力路径组生成:
        return 枚举_逻辑组织需求类型::方法路径组;
    case 枚举_需求获取途径::因果链支撑组生成:
    case 枚举_需求获取途径::自检因果影响链生成:
        return 枚举_逻辑组织需求类型::因果子链支撑;
    case 枚举_需求获取途径::未定义:
    case 枚举_需求获取途径::普通目标直接生成:
    case 枚举_需求获取途径::任务筹办原子缺口:
    case 枚举_需求获取途径::任务执行回执原子缺口:
    case 枚举_需求获取途径::自检原子缺口:
    case 枚举_需求获取途径::观察反馈原子缺口:
    case 枚举_需求获取途径::结算后处理原子目标:
    case 枚举_需求获取途径::OR组路径完成令牌提交:
    default:
        return 枚举_逻辑组织需求类型::未定义;
    }
}

// 功能：读取并返回指定对象、状态或运行材料。
const 语素入口节点类* 需求类::需求获取途径默认目标特征类型(
    枚举_需求获取途径 途径) noexcept
{
    if (const auto 逻辑类型 = 逻辑组织需求类型_按获取途径(途径);
        逻辑类型 != 枚举_逻辑组织需求类型::未定义) {
        return 逻辑组织目标特征类型(逻辑类型);
    }
    if (途径 == 枚举_需求获取途径::OR组路径完成令牌提交) {
        return OR组结算令牌目标特征类型();
    }
    return nullptr;
}

// 功能：读取并返回指定对象、状态或运行材料。
需求类::结构_需求获取途径语义视图 需求类::需求获取途径语义视图(
    枚举_需求获取途径 途径) noexcept
{
    结构_需求获取途径语义视图 视图{};
    视图.获取途径 = 途径;
    视图.获取途径名称 = 需求获取途径文本(途径);
    视图.默认目标语义 = 需求目标语义_按获取途径(途径);
    视图.默认逻辑组织类型 = 逻辑组织需求类型_按获取途径(途径);
    视图.默认目标特征类型 = 需求获取途径默认目标特征类型(途径);
    视图.语义由获取途径确定 = 途径 != 枚举_需求获取途径::未定义;
    视图.需要目标特征上下文 =
        视图.默认目标语义 == 枚举_需求目标语义::普通可任务化目标;
    视图.可直接构造逻辑组织需求 =
        视图.默认逻辑组织类型 != 枚举_逻辑组织需求类型::未定义;
    视图.是OR令牌提交途径 =
        视图.默认目标语义 == 枚举_需求目标语义::OR组结算令牌;
    return 视图;
}

// 功能：从指定来源读取数据或状态。
const 语素入口节点类* 需求类::读取需求目标特征类型(
    const 节点类* 需求) noexcept
{
    if (!需求) {
        return nullptr;
    }
    if (需求->主信息.目标特征类型缓存) {
        return 需求->主信息.目标特征类型缓存;
    }
    auto* 目标状态 = 私有_解析状态引用(需求->主信息.需求状态);
    if (const auto* 目标特征类型 = 私有_状态特征类型_需求校验(目标状态)) {
        return 目标特征类型;
    }
    auto* 当前状态 = 私有_解析状态引用(需求->主信息.被需求当前状态);
    return 私有_状态特征类型_需求校验(当前状态);
}

// 功能：读取需求主体引用，不修改需求树。
bool 需求类::读取需求主体引用(
    const 节点类* 需求,
    可解析引用<存在节点类>& 输出引用) noexcept
{
    输出引用 = {};
    if (!需求) {
        return false;
    }
    输出引用 = 需求->主信息.需求主体;
    return 输出引用.有效();
}

// 功能：读取需求目标宿主引用，不修改需求树。
bool 需求类::读取需求目标宿主引用(
    const 节点类* 需求,
    可解析引用<存在节点类>& 输出引用) noexcept
{
    输出引用 = {};
    if (!需求) {
        return false;
    }
    输出引用 = 需求->主信息.被需求存在;
    return 输出引用.有效();
}

// 功能：解析需求主体，不创建或修改节点。
存在节点类* 需求类::解析需求主体(const 节点类* 需求) noexcept
{
    可解析引用<存在节点类> 引用{};
    return 读取需求主体引用(需求, 引用)
        ? 私有_解析存在引用(引用)
        : nullptr;
}

// 功能：解析需求目标宿主，不创建或修改节点。
存在节点类* 需求类::解析需求目标宿主(const 节点类* 需求) noexcept
{
    可解析引用<存在节点类> 引用{};
    return 读取需求目标宿主引用(需求, 引用)
        ? 私有_解析存在引用(引用)
        : nullptr;
}

// 功能：读取需求当前状态引用，不修改需求树。
bool 需求类::读取需求当前状态引用(
    const 节点类* 需求,
    可解析引用<状态节点类>& 输出引用) noexcept
{
    输出引用 = {};
    if (!需求) {
        return false;
    }
    输出引用 = 需求->主信息.被需求当前状态;
    return 输出引用.有效();
}

// 功能：读取需求目标状态引用，不修改需求树。
bool 需求类::读取需求目标状态引用(
    const 节点类* 需求,
    可解析引用<状态节点类>& 输出引用) noexcept
{
    输出引用 = {};
    if (!需求) {
        return false;
    }
    输出引用 = 需求->主信息.需求状态;
    return 输出引用.有效();
}

// 功能：解析需求当前状态，不创建或修改节点。
状态节点类* 需求类::解析需求当前状态(const 节点类* 需求) noexcept
{
    可解析引用<状态节点类> 引用{};
    return 读取需求当前状态引用(需求, 引用)
        ? 私有_解析状态引用(引用)
        : nullptr;
}

// 功能：解析需求目标状态，不创建或修改节点。
状态节点类* 需求类::解析需求目标状态(const 节点类* 需求) noexcept
{
    可解析引用<状态节点类> 引用{};
    return 读取需求目标状态引用(需求, 引用)
        ? 私有_解析状态引用(引用)
        : nullptr;
}

// 功能：读取需求场景引用，不修改需求树。
bool 需求类::读取需求场景引用(
    const 节点类* 需求,
    可解析引用<场景节点类>& 输出引用) noexcept
{
    输出引用 = {};
    if (!需求) {
        return false;
    }
    输出引用 = 需求->主信息.需求场景;
    return 输出引用.有效();
}

// 功能：解析需求场景，不创建或修改节点。
场景节点类* 需求类::解析需求场景(const 节点类* 需求) noexcept
{
    可解析引用<场景节点类> 引用{};
    return 读取需求场景引用(需求, 引用)
        ? 私有_解析场景引用(引用)
        : nullptr;
}

// 功能：读取需求当前对应任务引用，不修改需求树。
bool 需求类::读取需求对应任务引用(
    const 节点类* 需求,
    可解析引用<任务节点类>& 输出引用) noexcept
{
    输出引用 = {};
    if (!需求) {
        return false;
    }
    输出引用 = 需求->主信息.对应任务;
    return 输出引用.有效();
}

// 功能：读取需求关联任务列表引用视图，不修改需求树。
const std::vector<可解析引用<任务节点类>>* 需求类::读取需求任务列表引用(
    const 节点类* 需求) noexcept
{
    return 需求 ? &需求->主信息.任务列表 : nullptr;
}

// 功能：读取需求满足关系，不修改需求树。
三向关系掩码 需求类::读取需求满足关系(
    const 节点类* 需求,
    三向关系掩码 默认关系) noexcept
{
    return 需求 && 需求->主信息.满足关系 != 0
        ? 需求->主信息.满足关系
        : 默认关系;
}

// 功能：读取需求有效截止时间，不修改需求树。
时间戳 需求类::读取需求有效截止(
    const 节点类* 需求,
    时间戳 默认值) noexcept
{
    return 需求 ? 需求->主信息.需求有效截止 : 默认值;
}

// 功能：判断需求是否已经截止，不修改需求树。
bool 需求类::需求已截止(const 节点类* 需求) noexcept
{
    return 读取需求有效截止(需求) != 0;
}

// 功能：判断需求是否阻塞父任务执行，不修改需求树。
bool 需求类::需求阻塞父任务执行(const 节点类* 需求) noexcept
{
    return 需求 && 需求->主信息.是否阻塞父任务执行;
}

// 功能：读取需求安全权重，不修改需求树。
I64 需求类::读取需求安全权重(
    const 节点类* 需求,
    I64 默认值) noexcept
{
    return 需求 ? 需求->主信息.安全权重 : 默认值;
}

// 功能：读取需求服务权重，不修改需求树。
I64 需求类::读取需求服务权重(
    const 节点类* 需求,
    I64 默认值) noexcept
{
    return 需求 ? 需求->主信息.服务权重 : 默认值;
}

// 功能：读取需求综合权重，不修改需求树。
I64 需求类::读取需求综合权重(
    const 节点类* 需求,
    I64 默认值) noexcept
{
    return 需求
        ? 需求->主信息.安全权重 + 需求->主信息.服务权重
        : 默认值;
}

// 功能：提交需求结算摘要和可选目标状态收束，不直接修改需求树结构。
需求类::结构_需求结算收束结果 需求类::提交需求结算收束(
    节点类* 需求,
    const std::string& 最近结算任务主键,
    I64 安全结算增量,
    I64 服务结算增量,
    bool 允许收束当前状态,
    时间戳 now) noexcept
{
    结构_需求结算收束结果 结果{};
    if (!需求) {
        return 结果;
    }

    const auto 旧状态指针 = 需求->主信息.被需求当前状态.指针;
    const auto 旧状态主键 = 需求->主信息.被需求当前状态.主键;
    if (允许收束当前状态 && 需求->主信息.需求状态.有效()) {
        需求->主信息.被需求当前状态 = 需求->主信息.需求状态;
        需求->主信息.需求有效截止 = now;
        结果.已提交收束 = true;
        结果.当前状态已回写 =
            需求->主信息.被需求当前状态.指针 != 旧状态指针
            || 需求->主信息.被需求当前状态.主键 != 旧状态主键;
    }

    if (安全结算增量 > 0) {
        需求->主信息.累计安全结算 += 安全结算增量;
        结果.已写结算 = true;
    }
    if (服务结算增量 > 0) {
        需求->主信息.累计服务结算 += 服务结算增量;
        结果.已写结算 = true;
    }
    需求->主信息.累计安全结算 =
        (std::min<I64>)((std::max<I64>)(0, 需求->主信息.安全权重), 需求->主信息.累计安全结算);
    需求->主信息.累计服务结算 =
        (std::min<I64>)((std::max<I64>)(0, 需求->主信息.服务权重), 需求->主信息.累计服务结算);

    if (!最近结算任务主键.empty()) {
        需求->主信息.最近结算任务主键 = 最近结算任务主键;
    }
    if (结果.已提交收束 || 结果.当前状态已回写 || 结果.已写结算) {
        需求->主信息.最近结算时间 = now;
        需求->主信息.统计.最后观测时间 = now;
        私有_刷新父级需求结算摘要_需求类(需求);

        需求类 需求服务{};
        if (auto* 父需求 = reinterpret_cast<节点类*>(需求->父)) {
            (void)需求服务.刷新子需求权重(父需求);
        }
        (void)需求服务.刷新子需求权重(需求);
        结果.已刷新权重 = true;
    }

    结果.成功 = true;
    结果.需求有效截止 = 需求->主信息.需求有效截止;
    结果.累计安全结算 = 需求->主信息.累计安全结算;
    结果.累计服务结算 = 需求->主信息.累计服务结算;
    结果.安全权重 = 需求->主信息.安全权重;
    结果.服务权重 = 需求->主信息.服务权重;
    结果.最近结算任务主键 = 需求->主信息.最近结算任务主键;
    return 结果;
}

// 功能：判断来源需求是否携带安全方向，不修改需求树。
bool 需求类::来源需求有安全方向(const 节点类* 需求) noexcept
{
    return 需求 && 需求->主信息.安全权重 > 0;
}

// 功能：判断来源需求是否携带服务方向，不修改需求树。
bool 需求类::来源需求有服务方向(const 节点类* 需求) noexcept
{
    return 需求 && 需求->主信息.服务权重 > 0;
}

// 功能：按函数名执行对应处理。
需求类::结构_需求目标语义视图 需求类::需求目标语义视图(
    const 节点类* 需求) noexcept
{
    return 需求目标语义视图(读取需求目标特征类型(需求));
}

// 功能：根据当前输入生成目标数据、场景、动态或回执。
需求类::结构_需求方向签名 需求类::生成需求方向签名(
    const 二次特征节点类* 比较节点) noexcept
{
    结构_二次特征方向签名材料 材料{};
    if (!二次特征类::读取方向签名材料_直接(比较节点, 材料)) {
        return {};
    }
    return 私有_生成需求方向签名_从材料(材料);
}

// 功能：比较当前值、目标值或历史基准。
需求类::枚举_需求方向比较结果 需求类::比较需求方向签名(
    const 结构_需求方向签名& 左,
    const 结构_需求方向签名& 右) noexcept
{
    if (!左.有效() || !右.有效()) {
        return 枚举_需求方向比较结果::不可比较;
    }
    if (左.目标特征类型主键 != 右.目标特征类型主键) {
        return 枚举_需求方向比较结果::不同目标;
    }
    if (!左.目标主体主键.empty()
        && !右.目标主体主键.empty()
        && 左.目标主体主键 != 右.目标主体主键) {
        return 枚举_需求方向比较结果::不同目标;
    }

    if (左.方向掩码 != 0 && 右.方向掩码 != 0) {
        if ((左.方向掩码 & 右.方向掩码) != 0) {
            return 枚举_需求方向比较结果::同向;
        }
        if (私有_方向掩码反向_需求方向(左.方向掩码, 右.方向掩码)) {
            return 枚举_需求方向比较结果::反向;
        }
        return 枚举_需求方向比较结果::不可比较;
    }

    using 枚举方向区间 = 二次特征主信息类::枚举_方向区间;
    if (左.方向区间 != 枚举方向区间::未定义
        && 右.方向区间 != 枚举方向区间::未定义) {
        if (左.方向区间 == 右.方向区间) {
            return 枚举_需求方向比较结果::同向;
        }
        if (私有_方向区间反向_需求方向(左.方向区间, 右.方向区间)) {
            return 枚举_需求方向比较结果::反向;
        }
        return 枚举_需求方向比较结果::不可比较;
    }

    if (左.有差值 && 右.有差值) {
        const int 左符号 = 私有_I64符号_需求方向(左.差值);
        const int 右符号 = 私有_I64符号_需求方向(右.差值);
        if (左符号 == 右符号) {
            return 枚举_需求方向比较结果::同向;
        }
        if (左符号 != 0 && 右符号 != 0 && 左符号 == -右符号) {
            return 枚举_需求方向比较结果::反向;
        }
        return 枚举_需求方向比较结果::不可比较;
    }

    if (左.有标量值 && 右.有标量值) {
        const int 左符号 = 私有_I64符号_需求方向(左.标量值);
        const int 右符号 = 私有_I64符号_需求方向(右.标量值);
        if (左符号 == 右符号) {
            return 枚举_需求方向比较结果::同向;
        }
        if (左符号 != 0 && 右符号 != 0 && 左符号 == -右符号) {
            return 枚举_需求方向比较结果::反向;
        }
    }
    return 枚举_需求方向比较结果::不可比较;
}

// 功能：比较当前值、目标值或历史基准。
需求类::枚举_需求方向比较结果 需求类::比较需求方向(
    const 二次特征节点类* 左,
    const 二次特征节点类* 右) noexcept
{
    return 比较需求方向签名(
        生成需求方向签名(左),
        生成需求方向签名(右));
}

// 功能：按函数名执行对应处理。
bool 需求类::需求方向同向(
    const 二次特征节点类* 左,
    const 二次特征节点类* 右) noexcept
{
    return 比较需求方向(左, 右) == 枚举_需求方向比较结果::同向;
}

// 功能：按函数名执行对应处理。
bool 需求类::需求方向反向(
    const 二次特征节点类* 左,
    const 二次特征节点类* 右) noexcept
{
    return 比较需求方向(左, 右) == 枚举_需求方向比较结果::反向;
}

// 功能：推进任务筹办，形成候选、缺口或就绪回执。
bool 需求类::目标特征类型可进入普通候选方法筹办(
    const 语素入口节点类* 目标特征类型) noexcept
{
    return 需求目标语义视图(目标特征类型).可进入普通候选方法筹办;
}

// 功能：推进任务筹办，形成候选、缺口或就绪回执。
const char* 需求类::普通候选方法筹办禁止原因(
    const 结构_需求目标语义视图& 视图) noexcept
{
    switch (视图.语义) {
    case 枚举_需求目标语义::目标特征未解析:
        return "目标特征未解析";
    case 枚举_需求目标语义::OR组满足:
        return "OR组满足状态等待专用提交链领取结算令牌";
    case 枚举_需求目标语义::AND组满足:
    case 枚举_需求目标语义::方法路径完成:
    case 枚举_需求目标语义::因果子链支撑:
        return "逻辑组织目标由直接子需求聚合，不填充普通候选方法";
    case 枚举_需求目标语义::OR组结算令牌:
        return "OR组结算令牌只能由逻辑组织提交链调用";
    case 枚举_需求目标语义::普通可任务化目标:
    default:
        return "无";
    }
}

// 功能：推进任务筹办，形成候选、缺口或就绪回执。
const char* 需求类::普通候选方法筹办禁止原因(
    const 语素入口节点类* 目标特征类型) noexcept
{
    return 普通候选方法筹办禁止原因(需求目标语义视图(目标特征类型));
}

// 功能：按函数名执行对应处理。
const char* 需求类::需求结构形态文本(
    const 节点类* 需求) noexcept
{
    if (!需求) {
        return "空需求";
    }
    return 需求->子 ? "非叶子" : "叶子";
}

// 功能：构造满足需求更新指令，不直接修改需求树。
结构_需求树更新指令 需求类::构造满足需求指令(
    节点类* 父需求,
    节点类* 目标需求,
    const 节点类* 状态来源需求,
    时间戳 观测时间) noexcept
{
    结构_需求树更新指令 指令{};
    if (!目标需求) {
        return 指令;
    }

    指令.指令类型 = 枚举_需求树更新指令类型::满足需求;
    指令.目标需求指针 = reinterpret_cast<std::uintptr_t>(目标需求);
    指令.目标需求主键 = 目标需求->获取主键();
    if (父需求) {
        指令.父需求指针 = reinterpret_cast<std::uintptr_t>(父需求);
        指令.父需求主键 = 父需求->获取主键();
    }

    const auto* 状态来源 = 状态来源需求 ? 状态来源需求 : 目标需求;
    可解析引用<状态节点类> 当前状态引用{};
    可解析引用<状态节点类> 目标状态引用{};
    (void)读取需求当前状态引用(状态来源, 当前状态引用);
    (void)读取需求目标状态引用(状态来源, 目标状态引用);
    指令.当前状态指针 =
        reinterpret_cast<std::uintptr_t>(当前状态引用.指针);
    指令.当前状态主键 = 当前状态引用.主键;
    指令.需求状态指针 =
        reinterpret_cast<std::uintptr_t>(目标状态引用.指针);
    指令.需求状态主键 = 目标状态引用.主键;

    const auto 满足关系值 = 读取需求满足关系(状态来源);
    指令.满足关系 = 满足关系值 != 0 ? 满足关系值 : 关系_等于;
    指令.使用新安全权重 = true;
    指令.新安全权重 = 0;
    指令.使用新服务权重 = true;
    指令.新服务权重 = 0;
    指令.标记已满足 = true;
    指令.使用阻塞父任务执行 = true;
    指令.阻塞父任务执行 = false;
    指令.记录观测统计 = true;
    指令.统计观测时间 = 观测时间;
    return 指令;
}

// 功能：构造删除需求更新指令，不直接修改需求树。
结构_需求树更新指令 需求类::构造删除需求指令(
    节点类* 父需求,
    节点类* 目标需求) noexcept
{
    结构_需求树更新指令 指令{};
    if (!目标需求) {
        return 指令;
    }

    指令.指令类型 = 枚举_需求树更新指令类型::删除需求;
    指令.目标需求指针 = reinterpret_cast<std::uintptr_t>(目标需求);
    指令.目标需求主键 = 目标需求->获取主键();
    if (父需求) {
        指令.父需求指针 = reinterpret_cast<std::uintptr_t>(父需求);
        指令.父需求主键 = 父需求->获取主键();
    }
    return 指令;
}

// 功能：构造需求权重和阻塞状态更新指令，不直接修改需求树。
结构_需求树更新指令 需求类::构造需求权重阻塞更新指令(
    节点类* 父需求,
    节点类* 目标需求,
    const 语素入口节点类* 目标特征类型,
    std::int64_t 新安全权重,
    std::int64_t 新服务权重,
    bool 使用阻塞父任务执行,
    bool 阻塞父任务执行,
    bool 记录观测统计,
    时间戳 观测时间) noexcept
{
    结构_需求树更新指令 指令{};
    if (!目标需求) {
        return 指令;
    }

    指令.指令类型 = 枚举_需求树更新指令类型::更新需求;
    指令.目标需求指针 = reinterpret_cast<std::uintptr_t>(目标需求);
    指令.目标需求主键 = 目标需求->获取主键();
    if (父需求) {
        指令.父需求指针 = reinterpret_cast<std::uintptr_t>(父需求);
        指令.父需求主键 = 父需求->获取主键();
    }
    指令.目标特征类型缓存 = 目标特征类型;
    指令.使用新安全权重 = true;
    指令.新安全权重 = 新安全权重;
    指令.使用新服务权重 = true;
    指令.新服务权重 = 新服务权重;
    指令.使用阻塞父任务执行 = 使用阻塞父任务执行;
    指令.阻塞父任务执行 = 阻塞父任务执行;
    指令.记录观测统计 = 记录观测统计;
    指令.统计观测时间 = 观测时间;
    return 指令;
}

// 功能：构造需求目标关系更新指令，不直接修改需求树。
结构_需求树更新指令 需求类::构造需求目标关系更新指令(
    节点类* 父需求,
    节点类* 目标需求,
    状态节点类* 当前状态,
    状态节点类* 目标状态,
    三向关系掩码 满足关系,
    const 语素入口节点类* 目标特征类型,
    bool 使用阻塞父任务执行,
    bool 阻塞父任务执行,
    bool 记录观测统计,
    时间戳 观测时间) noexcept
{
    结构_需求树更新指令 指令{};
    if (!目标需求) {
        return 指令;
    }

    指令.指令类型 = 枚举_需求树更新指令类型::更新需求;
    指令.目标需求指针 = reinterpret_cast<std::uintptr_t>(目标需求);
    指令.目标需求主键 = 目标需求->获取主键();
    if (父需求) {
        指令.父需求指针 = reinterpret_cast<std::uintptr_t>(父需求);
        指令.父需求主键 = 父需求->获取主键();
    }
    if (当前状态) {
        指令.当前状态指针 = reinterpret_cast<std::uintptr_t>(当前状态);
        指令.当前状态主键 = 当前状态->获取主键();
    }
    if (目标状态) {
        指令.需求状态指针 = reinterpret_cast<std::uintptr_t>(目标状态);
        指令.需求状态主键 = 目标状态->获取主键();
    }
    指令.满足关系 = 满足关系 != 0 ? 满足关系 : 关系_等于;
    指令.目标特征类型缓存 = 目标特征类型;
    指令.使用阻塞父任务执行 = 使用阻塞父任务执行;
    指令.阻塞父任务执行 = 阻塞父任务执行;
    指令.记录观测统计 = 记录观测统计;
    指令.统计观测时间 = 观测时间;
    return 指令;
}

// 功能：构造新增原子需求指令，不直接修改需求树。
结构_需求树更新指令 需求类::构造新增原子需求指令(
    节点类* 父需求,
    const 语素入口节点类* 需求名称,
    const 语素入口节点类* 目标特征类型,
    const 可解析引用<存在节点类>& 需求主体,
    const 可解析引用<场景节点类>& 需求场景,
    const 可解析引用<存在节点类>& 目标宿主,
    const 可解析引用<状态节点类>& 当前状态,
    const 可解析引用<状态节点类>& 目标状态,
    三向关系掩码 满足关系,
    std::int64_t 新安全权重,
    std::int64_t 新服务权重,
    bool 使用阻塞父任务执行,
    bool 阻塞父任务执行,
    bool 记录观测统计,
    时间戳 观测时间) noexcept
{
    结构_需求树更新指令 指令{};
    if (!父需求) {
        return 指令;
    }

    指令.指令类型 = 枚举_需求树更新指令类型::新增需求;
    指令.父需求指针 = reinterpret_cast<std::uintptr_t>(父需求);
    指令.父需求主键 = 父需求->获取主键();
    指令.需求名称 = 需求名称 ? 需求名称 : 目标特征类型;
    指令.目标特征类型缓存 = 目标特征类型;
    if (需求主体.指针) {
        指令.需求主体指针 = reinterpret_cast<std::uintptr_t>(需求主体.指针);
        指令.需求主体主键 = 需求主体.指针->获取主键();
    } else {
        指令.需求主体主键 = 需求主体.主键;
    }
    if (需求场景.指针) {
        指令.需求场景指针 = reinterpret_cast<std::uintptr_t>(需求场景.指针);
        指令.需求场景主键 = 需求场景.指针->获取主键();
    } else {
        指令.需求场景主键 = 需求场景.主键;
    }
    if (目标宿主.指针) {
        指令.被需求存在指针 = reinterpret_cast<std::uintptr_t>(目标宿主.指针);
        指令.被需求存在主键 = 目标宿主.指针->获取主键();
    } else {
        指令.被需求存在主键 = 目标宿主.主键;
    }
    if (当前状态.指针) {
        指令.当前状态指针 = reinterpret_cast<std::uintptr_t>(当前状态.指针);
        指令.当前状态主键 = 当前状态.指针->获取主键();
    } else {
        指令.当前状态主键 = 当前状态.主键;
    }
    if (目标状态.指针) {
        指令.需求状态指针 = reinterpret_cast<std::uintptr_t>(目标状态.指针);
        指令.需求状态主键 = 目标状态.指针->获取主键();
    } else {
        指令.需求状态主键 = 目标状态.主键;
    }
    指令.满足关系 = 满足关系 != 0 ? 满足关系 : 关系_等于;
    指令.使用新安全权重 = true;
    指令.新安全权重 = 新安全权重;
    指令.使用新服务权重 = true;
    指令.新服务权重 = 新服务权重;
    指令.使用阻塞父任务执行 = 使用阻塞父任务执行;
    指令.阻塞父任务执行 = 阻塞父任务执行;
    指令.记录观测统计 = 记录观测统计;
    指令.统计观测时间 = 观测时间;
    return 指令;
}

// 功能：更新已有对象、状态、索引或缓存。
结构_需求树更新指令 需求类::构造逻辑组织需求更新指令(
    const 结构_逻辑组织需求更新输入& 输入,
    枚举_逻辑组织需求类型 类型) noexcept
{
    结构_需求树更新指令 指令{};
    const auto* 目标特征 = 逻辑组织目标特征类型(类型);
    if (!目标特征) {
        return 指令;
    }

    指令.指令类型 = 枚举_需求树更新指令类型::新增需求;
    if (输入.父需求) {
        指令.父需求指针 = reinterpret_cast<std::uintptr_t>(输入.父需求);
        指令.父需求主键 = 输入.父需求->获取主键();
    } else {
        指令.父需求主键 = 输入.父需求主键;
    }
    if (输入.需求主体) {
        指令.需求主体指针 = reinterpret_cast<std::uintptr_t>(输入.需求主体);
        指令.需求主体主键 = 输入.需求主体->获取主键();
    } else {
        指令.需求主体主键 = 输入.需求主体主键;
    }
    if (输入.需求场景) {
        指令.需求场景指针 = reinterpret_cast<std::uintptr_t>(输入.需求场景);
        指令.需求场景主键 = 输入.需求场景->获取主键();
    } else {
        指令.需求场景主键 = 输入.需求场景主键;
    }
    if (输入.目标宿主) {
        指令.被需求存在指针 = reinterpret_cast<std::uintptr_t>(输入.目标宿主);
        指令.被需求存在主键 = 输入.目标宿主->获取主键();
    } else {
        指令.被需求存在主键 = 输入.目标宿主主键;
    }
    if (输入.当前状态) {
        指令.当前状态指针 = reinterpret_cast<std::uintptr_t>(输入.当前状态);
        指令.当前状态主键 = 输入.当前状态->获取主键();
    } else {
        指令.当前状态主键 = 输入.当前状态主键;
    }
    if (输入.目标状态) {
        指令.需求状态指针 = reinterpret_cast<std::uintptr_t>(输入.目标状态);
        指令.需求状态主键 = 输入.目标状态->获取主键();
    } else {
        指令.需求状态主键 = 输入.目标状态主键;
    }

    指令.需求名称 = 目标特征;
    指令.目标特征类型缓存 = 目标特征;
    指令.满足关系 = 输入.满足关系 != 0 ? 输入.满足关系 : 关系_等于;
    指令.使用新安全权重 = 输入.使用新安全权重;
    指令.新安全权重 = 输入.新安全权重;
    指令.使用新服务权重 = 输入.使用新服务权重;
    指令.新服务权重 = 输入.新服务权重;
    指令.使用阻塞父任务执行 = 输入.使用阻塞父任务执行;
    指令.阻塞父任务执行 = 输入.阻塞父任务执行;
    指令.记录观测统计 = 输入.记录观测统计;
    指令.统计观测时间 = 输入.统计观测时间;

    std::ostringstream 日志;
    日志 << "需求类/逻辑组织需求更新指令构造"
        << " | 逻辑类型=" << 逻辑组织需求类型文本(类型)
        << " | 目标特征=" << 目标特征->获取主键()
        << " | 父需求=" << (指令.父需求主键.empty() ? std::string("空") : 指令.父需求主键)
        << " | 目标宿主=" << (指令.被需求存在主键.empty() ? std::string("空") : 指令.被需求存在主键)
        << " | 当前状态=" << (指令.当前状态主键.empty() ? std::string("空") : 指令.当前状态主键)
        << " | 目标状态=" << (指令.需求状态主键.empty() ? std::string("空") : 指令.需求状态主键)
        << " | 目标语义来源=目标特征类型"
        << " | 统一更新指令=1"
        << " | 专用主信息字段=0";
    项目运行日志(日志.str());
    return 指令;
}

// 功能：读取并返回指定对象、状态或运行材料。
结构_需求树更新指令 需求类::构造获取途径逻辑组织需求更新指令(
    const 结构_逻辑组织需求更新输入& 输入,
    枚举_需求获取途径 途径) noexcept
{
    const auto 视图 = 需求获取途径语义视图(途径);
    if (!视图.可直接构造逻辑组织需求) {
        std::ostringstream 日志;
        日志 << "需求类/获取途径逻辑组织需求更新指令跳过"
            << " | 获取途径=" << 视图.获取途径名称
            << " | 默认目标语义=" << 需求目标语义文本(视图.默认目标语义)
            << " | 需要目标特征上下文=" << (视图.需要目标特征上下文 ? 1 : 0)
            << " | OR令牌提交途径=" << (视图.是OR令牌提交途径 ? 1 : 0)
            << " | 原因=该获取途径不直接生成逻辑组织需求";
        项目运行日志(日志.str());
        return {};
    }
    return 构造逻辑组织需求更新指令(
        输入,
        视图.默认逻辑组织类型);
}

// 功能：更新已有对象、状态、索引或缓存。
结构_需求树更新指令 需求类::构造AND组需求更新指令(
    const 结构_逻辑组织需求更新输入& 输入) noexcept
{
    return 构造逻辑组织需求更新指令(
        输入,
        枚举_逻辑组织需求类型::AND组);
}

// 功能：更新已有对象、状态、索引或缓存。
结构_需求树更新指令 需求类::构造OR组需求更新指令(
    const 结构_逻辑组织需求更新输入& 输入) noexcept
{
    return 构造逻辑组织需求更新指令(
        输入,
        枚举_逻辑组织需求类型::OR组);
}

// 功能：更新已有对象、状态、索引或缓存。
结构_需求树更新指令 需求类::构造方法路径组需求更新指令(
    const 结构_逻辑组织需求更新输入& 输入) noexcept
{
    return 构造逻辑组织需求更新指令(
        输入,
        枚举_逻辑组织需求类型::方法路径组);
}

// 功能：更新已有对象、状态、索引或缓存。
结构_需求树更新指令 需求类::构造因果子链支撑需求更新指令(
    const 结构_逻辑组织需求更新输入& 输入) noexcept
{
    return 构造逻辑组织需求更新指令(
        输入,
        枚举_逻辑组织需求类型::因果子链支撑);
}

// 功能：按函数名执行对应处理。
const char* 需求类::枚举目标生产者分级文本(
    枚举_枚举目标生产者分级 分级) noexcept
{
    switch (分级) {
    case 枚举_枚举目标生产者分级::专用方法生产: return "专用方法生产";
    case 枚举_枚举目标生产者分级::通用流程生产: return "通用流程生产";
    case 枚举_枚举目标生产者分级::规则生产: return "规则生产";
    case 枚举_枚举目标生产者分级::初始化生产: return "初始化生产";
    case 枚举_枚举目标生产者分级::待补齐: return "待补齐";
    case 枚举_枚举目标生产者分级::未登记:
    default:
        return "未登记";
    }
}

// 功能：把枚举目标校验结论转换为日志文本。
const char* 需求类::枚举目标校验结论文本(
    枚举_枚举目标校验结论 结论) noexcept
{
    switch (结论) {
    case 枚举_枚举目标校验结论::缺目标特征或目标状态:
        return "缺目标特征或目标状态，跳过枚举目标校验";
    case 枚举_枚举目标校验结论::非枚举_含非点值状态:
        return "抽象特征含非点值状态，按非枚举目标处理";
    case 枚举_枚举目标校验结论::非枚举_未识别:
        return "未识别为离散点枚举目标";
    case 枚举_枚举目标校验结论::有序枚举比较符不合法:
        return "有序枚举目标只能使用等于或大于等于比较符";
    case 枚举_枚举目标校验结论::普通枚举比较符不合法:
        return "枚举目标只能使用等于比较符";
    case 枚举_枚举目标校验结论::目标值不属于值域:
        return "枚举目标值不属于抽象特征值域";
    case 枚举_枚举目标校验结论::生产者待补齐:
        return "枚举目标生产者待补齐，不能进入普通执行目标";
    case 枚举_枚举目标校验结论::生产者未登记:
        return "枚举目标特征未登记生产者，不能进入普通执行目标";
    case 枚举_枚举目标校验结论::通过:
        return "枚举目标比较符、值域和生产者分级通过";
    case 枚举_枚举目标校验结论::未检查:
    default:
        return "未检查";
    }
}

// 功能：按函数名执行对应处理。
结构_枚举目标校验结果 需求类::校验枚举目标合法性(
    const 语素入口节点类* 目标特征类型,
    状态节点类* 当前状态,
    状态节点类* 目标状态,
    三向关系掩码 满足关系) noexcept
{
    结构_枚举目标校验结果 结果{};
    结果.已检查 = true;
    结果.目标特征类型 = 目标特征类型;

    if (!结果.目标特征类型) {
        结果.目标特征类型 = 私有_状态特征类型_需求校验(目标状态);
    }
    if (!结果.目标特征类型) {
        结果.目标特征类型 = 私有_状态特征类型_需求校验(当前状态);
    }
    if (!结果.目标特征类型 || !目标状态) {
        结果.结论 = 枚举_枚举目标校验结论::缺目标特征或目标状态;
        return 结果;
    }

    auto* 抽象根 = 世界树.抽象特征根;
    特征类 特征服务{ &获取基础信息集() };
    auto* 抽象特征 = 抽象根
        ? 特征服务.查找子抽象特征_按类型(抽象根, 结果.目标特征类型)
        : nullptr;
    bool 有枚举子状态 = false;
    结果.是枚举目标 = 特征类::抽象特征是离散点枚举(抽象特征, &有枚举子状态);
    if (!结果.是枚举目标) {
        结果.结论 = 有枚举子状态
            ? 枚举_枚举目标校验结论::非枚举_含非点值状态
            : 枚举_枚举目标校验结论::非枚举_未识别;
        return 结果;
    }

    结果.生产者分级 = 私有_查询枚举目标生产者分级(结果.目标特征类型);
    结果.比较符合法 = 私有_枚举目标比较符合法(
        结果.目标特征类型,
        满足关系);

    可解析引用<基础信息节点类> 目标状态主体{};
    可解析引用<特征节点类> 目标状态特征{};
    特征值 目标状态值{};
    结果.目标值属于值域 =
        状态类::读取状态主体特征和值引用(
            目标状态,
            目标状态主体,
            目标状态特征,
            目标状态值)
        && 特征类::特征值命中抽象特征值域(抽象特征, 目标状态值);

    结果.有合法生产者 = 私有_生产者分级已有生产者(结果.生产者分级);
    结果.合法 = 结果.比较符合法
        && 结果.目标值属于值域
        && 结果.有合法生产者;

    if (!结果.比较符合法) {
        结果.结论 = 私有_枚举目标允许有序比较(结果.目标特征类型)
            ? 枚举_枚举目标校验结论::有序枚举比较符不合法
            : 枚举_枚举目标校验结论::普通枚举比较符不合法;
    } else if (!结果.目标值属于值域) {
        结果.结论 = 枚举_枚举目标校验结论::目标值不属于值域;
    } else if (!结果.有合法生产者) {
        结果.结论 = 结果.生产者分级 == 枚举_枚举目标生产者分级::待补齐
            ? 枚举_枚举目标校验结论::生产者待补齐
            : 枚举_枚举目标校验结论::生产者未登记;
    } else {
        结果.结论 = 枚举_枚举目标校验结论::通过;
    }
    return 结果;
}

// 功能：更新已有对象、状态、索引或缓存。
结构_枚举目标校验结果 需求类::校验更新指令枚举目标(
    const 结构_需求树更新指令& 指令) noexcept
{
    auto* 当前状态 = 私有_解析状态节点_需求校验(
        指令.当前状态指针,
        指令.当前状态主键);
    auto* 目标状态 = 私有_解析状态节点_需求校验(
        指令.需求状态指针,
        指令.需求状态主键);

    auto 校验 = 校验枚举目标合法性(
        指令.目标特征类型缓存,
        当前状态,
        目标状态,
        指令.满足关系 != 0 ? 指令.满足关系 : 关系_等于);

    if (校验.已检查 && 校验.是枚举目标 && !校验.合法) {
        项目运行错误日志(
            "需求类/枚举目标入树校验拒绝 | "
            + 私有_构造枚举目标校验摘要(校验)
            + " | 当前状态=" + (当前状态 ? 当前状态->获取主键() : std::string("空"))
            + " | 目标状态=" + (目标状态 ? 目标状态->获取主键() : std::string("空")));
    }
    return 校验;
}

// 功能：按主键解析给定需求树根下的需求节点，不创建或修改节点。
需求类::节点类* 需求类::按主键解析需求节点(
    节点类* 需求根节点,
    const std::string& 主键) noexcept
{
    if (!需求根节点 || 主键.empty()) {
        return nullptr;
    }

    std::lock_guard<std::recursive_mutex> 需求树锁{ 借用需求树全局互斥() };
    auto* 真实根 = 获取独立树根_已加锁(需求根节点);
    if (!真实根) {
        return nullptr;
    }
    return 按主键定位节点_已加锁(真实根, 主键);
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
bool 需求类::重写需求树SQL投影(
    节点类* 需求根节点,
    const char* 来源原因) noexcept
{
    if (!需求根节点) {
        项目运行警告日志("需求树SQL投影跳过 | 原因=需求根为空");
        return false;
    }

    std::lock_guard<std::mutex> SQL锁{ 私有_需求树SQL投影互斥() };
    try {
        std::vector<结构_需求树SQL行> 行集{};
        std::unordered_set<const 节点类*> 已访问{};
        {
            std::lock_guard<std::recursive_mutex> 需求树锁{ 借用需求树全局互斥() };
            私有_收集需求树SQL行(
                需求根节点,
                {},
                0,
                0,
                0,
                {},
                已访问,
                行集);
        }

        const auto 根主键 = 需求根节点->获取主键();
        const auto 原因文本 = 来源原因 ? std::string(来源原因) : std::string{};

        const auto 主库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "master");
        const auto 投影库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "鱼巢投影库");
        std::string 错误{};
        if (!私有_执行需求树ADO命令(主库连接串, "需求树SQL投影建库", 私有_需求树SQL建库脚本(), 错误)
            || !私有_执行需求树ADO命令(投影库连接串, "需求树SQL投影建表", 私有_需求树SQL建表脚本(), 错误)
            || !私有_执行需求树ADO命令(投影库连接串, "需求树SQL投影视图", 私有_需求树SQL视图脚本(), 错误)
            || !私有_执行需求树ADO命令(投影库连接串, "需求树SQL投影重写", 私有_构造需求树SQL重写脚本(行集, 原因文本, 根主键), 错误)) {
            项目运行错误日志(
                "需求树SQL投影失败"
                " | 原因=" + 错误
                + " | 节点数=" + std::to_string(行集.size()));
            return false;
        }

        项目运行日志(
            "需求树SQL投影完成"
            " | 来源=" + 原因文本
            + " | 根=" + 根主键
            + " | 节点数=" + std::to_string(行集.size()));
        return true;
    }
    catch (const std::exception& 异常) {
        项目运行错误日志(std::string("需求树SQL投影异常 | 原因=") + 异常.what());
    }
    catch (...) {
        项目运行错误日志("需求树SQL投影异常 | 原因=未知异常");
    }
    return false;
}

// 功能：建立对象、任务、方法或因果之间的绑定关系。
bool 需求类::绑定对应任务(
    节点类* 需求,
    任务节点类* 任务) noexcept
{
    std::lock_guard<std::recursive_mutex> 借用锁{ 需求类::借用需求树全局互斥() };
    if (!需求 || !任务) {
        return false;
    }

    auto* 任务节点 = reinterpret_cast<任务类::节点类*>(任务);
    可解析引用<任务节点类> 引用{};
    引用.指针 = 任务;
    引用.主键 = 任务节点 ? 任务节点->获取主键() : std::string{};

    需求->主信息.对应任务 = 引用;

    const auto 已存在 = std::any_of(
        需求->主信息.任务列表.begin(),
        需求->主信息.任务列表.end(),
        [&](const auto& 项) noexcept {
            return 项.指针 == 任务
                || (!引用.主键.empty() && 项.主键 == 引用.主键);
        });
    if (!已存在) {
        需求->主信息.任务列表.push_back(引用);
    }
    return true;
}

// 功能：初始化相关对象、状态或运行上下文。
需求类::结构_任务初始化上下文 需求类::生成任务初始化上下文(
    节点类* 需求) noexcept
{
    结构_任务初始化上下文 输出{};
    if (!需求) {
        return 输出;
    }

    const auto& 主信息 = 需求->主信息;
    输出.来源需求 = 需求;
    输出.发起存在 = 主信息.需求主体;
    输出.所属场景 = 主信息.需求场景;
    输出.作用对象 = 主信息.被需求存在;
    输出.当前状态 = 主信息.被需求当前状态;
    输出.目标状态 = 主信息.需求状态;
    输出.满足关系 = 主信息.满足关系 != 0 ? 主信息.满足关系 : 关系_等于;
    输出.目标特征类型 = 主信息.目标特征类型缓存;
    输出.安全权重 = 主信息.安全权重;
    输出.服务权重 = 主信息.服务权重;

    可解析引用<基础信息节点类> 状态主体{};
    可解析引用<特征节点类> 状态特征{};
    特征值 状态值{};
    const auto* 状态 = 主信息.需求状态.指针
        ? 主信息.需求状态.指针
        : (主信息.被需求当前状态.指针 ? 主信息.被需求当前状态.指针 : nullptr);
    if (状态类::读取状态主体特征和值引用(状态, 状态主体, 状态特征, 状态值)) {
        if (!输出.作用对象.有效()) {
            if (auto* 主体 = dynamic_cast<存在节点类*>(状态主体.指针)) {
                输出.作用对象.绑定(主体);
            } else if (!状态主体.主键.empty()) {
                输出.作用对象.主键 = 状态主体.主键;
            }
        }

        if (!输出.目标特征类型) {
            输出.目标特征类型 =
                特征类::读取特征语义类型_直接(状态特征.指针);
        }
    }

    return 输出;
}

// 功能：读取并返回指定对象、状态或运行材料。
二次特征节点类* 需求类::获取需求方向(节点类* 需求节点) const noexcept
{
    if (!需求节点) {
        return nullptr;
    }

    auto* 初始状态 = 私有_解析状态引用(需求节点->主信息.被需求当前状态);
    auto* 结果状态 = 私有_解析状态引用(需求节点->主信息.需求状态);
    if (!初始状态 || !结果状态) {
        return nullptr;
    }

    可解析引用<基础信息节点类> 初始状态主体{};
    可解析引用<基础信息节点类> 结果状态主体{};
    可解析引用<特征节点类> 初始状态特征{};
    可解析引用<特征节点类> 结果状态特征{};
    特征值 初始状态值{};
    特征值 结果状态值{};
    if (!状态类::读取状态主体特征和值引用(
            初始状态,
            初始状态主体,
            初始状态特征,
            初始状态值)
        || !状态类::读取状态主体特征和值引用(
            结果状态,
            结果状态主体,
            结果状态特征,
            结果状态值)) {
        return nullptr;
    }

    const auto 比较方法结果 = 世界树.特征().比较同语义特征值(
        初始状态特征,
        初始状态值,
        结果状态特征,
        结果状态值);
    if (!比较方法结果.可比较) {
        return nullptr;
    }

    auto* 所属场景 = 私有_取所属场景(初始状态);
    if (!所属场景) {
        所属场景 = 私有_取所属场景(结果状态);
    }
    if (!所属场景) {
        return nullptr;
    }

    结构_时间段 来源时间段{
        世界树.状态().读取状态排序时间(初始状态),
        世界树.状态().读取状态排序时间(结果状态)
    };
    if (来源时间段.起 != 0 && 来源时间段.止 != 0 && 来源时间段.止 < 来源时间段.起) {
        std::swap(来源时间段.起, 来源时间段.止);
    }

    二次特征主信息类 模板{};
    模板.种类 = 枚举_二次特征种类::状态比较;
    模板.域 = 枚举_二次特征域::存在;
    模板.粒度 = 枚举_二次特征粒度::当前场景;
    模板.基准类型 = 枚举_二次特征基准类型::无;
    模板.时间归一方式 = 枚举_时间归一方式::无;
    模板.值形态 = 枚举_二次特征值形态::标量;
    模板.主体 = 初始状态主体;
    if (初始状态主体.指针 != 结果状态主体.指针
        && 初始状态主体.主键 != 结果状态主体.主键) {
        模板.客体 = 结果状态主体;
    }
    模板.左对象 = 私有_生成引用(static_cast<基础信息节点类*>(初始状态));
    模板.右对象 = 私有_生成引用(static_cast<基础信息节点类*>(结果状态));
    模板.来源起始状态 = 私有_生成引用(初始状态);
    模板.来源结果状态 = 私有_生成引用(结果状态);
    模板.来源场景 = 私有_生成引用(所属场景);
    模板.来源类型 = 枚举_二次特征来源类型::状态;
    模板.来源时间段 = 来源时间段;
    模板.刷新度量签名_链键();

    auto* 节点 = 世界树.获取或创建子二次特征_按签名(所属场景, 模板);
    if (!节点) {
        return nullptr;
    }

    (void)世界树.绑定二次特征场景(节点, 所属场景);
    (void)世界树.设置二次特征生成语义(
        节点,
        枚举_二次特征域::存在,
        枚举_二次特征粒度::当前场景,
        枚举_二次特征基准类型::无,
        枚举_时间归一方式::无,
        枚举_二次特征值形态::标量);
    (void)世界树.绑定二次特征来源(
        节点,
        初始状态,
        结果状态,
        nullptr,
        所属场景,
        0,
        0,
        枚举_二次特征来源类型::状态,
        来源时间段,
        0,
        0,
        0);
    (void)世界树.设置二次特征主体客体(
        节点,
        初始状态主体.指针,
        (初始状态主体.指针 != 结果状态主体.指针
            && 初始状态主体.主键 != 结果状态主体.主键)
            ? 结果状态主体.指针
            : nullptr);
    (void)世界树.设置二次特征左右对象(
        节点,
        static_cast<基础信息节点类*>(初始状态),
        static_cast<基础信息节点类*>(结果状态));
    (void)世界树.写入二次特征差值结果(
        节点,
        比较方法结果.左比较值,
        比较方法结果.右比较值);

    结构_二次特征方向签名材料 差值材料{};
    const auto 差值 = 二次特征类::读取方向签名材料_直接(节点, 差值材料)
        && 差值材料.差值.has_value()
        ? *差值材料.差值
        : 0;
    (void)世界树.写入二次特征标量值(节点, 差值, 差值 != 0, 10000);
    return 节点;
}
