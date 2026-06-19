#include "需求类.h"

#include <algorithm>
#include <initializer_list>
#include <mutex>
#include <sstream>
#include <unordered_set>

#include "日志接入.h"
#include "方法类.h"
#include "任务类.h"
#include "语素类.h"
#include "世界树类.h"

import 数据库ADO模块;

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
        auto 节点名称 = 语素_安全获取词(需求类::读取需求目标特征类型(节点));
        if (深度 == 0) {
            节点名称 = "需求树";
        }
        else if (深度 == 1 && 节点名称 == "安全值") {
            节点名称 = "安全";
        }
        else if (深度 == 1 && 节点名称 == "服务值") {
            节点名称 = "服务";
        }
        else if (节点名称.empty()) {
            节点名称 = "未命名需求";
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
            << "IF DB_ID(N'FishnestProjection') IS NULL CREATE DATABASE [FishnestProjection];\n";
        return SQL.str();
    }

    std::string 私有_需求树SQL建表脚本()
    {
        std::ostringstream SQL;
        SQL << "SET NOCOUNT ON;\n"
            << "IF SCHEMA_ID(N'fishnest') IS NULL EXEC(N'CREATE SCHEMA fishnest');\n"
            << "IF OBJECT_ID(N'fishnest.demand_tree_snapshot', N'U') IS NULL\n"
            << "CREATE TABLE fishnest.demand_tree_snapshot (\n"
            << "    snapshot_id uniqueidentifier NOT NULL PRIMARY KEY,\n"
            << "    captured_at datetime2(3) NOT NULL,\n"
            << "    source_kind nvarchar(80) NOT NULL,\n"
            << "    source_reason nvarchar(300) NULL,\n"
            << "    root_key nvarchar(80) NULL,\n"
            << "    node_count int NOT NULL\n"
            << ");\n"
            << "IF OBJECT_ID(N'fishnest.demand_tree_node', N'U') IS NULL\n"
            << "CREATE TABLE fishnest.demand_tree_node (\n"
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
            << "IF OBJECT_ID(N'fishnest.demand_main_info', N'U') IS NULL\n"
            << "CREATE TABLE fishnest.demand_main_info (\n"
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
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'parent_row_index') IS NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node ADD parent_row_index int NULL;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'node_name') IS NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node ADD node_name nvarchar(200) NULL;\n"
            << "IF COL_LENGTH(N'fishnest.demand_main_info', N'description_key') IS NULL\n"
            << "    ALTER TABLE fishnest.demand_main_info ADD description_key nvarchar(80) NULL;\n"
            << "IF COL_LENGTH(N'fishnest.demand_main_info', N'stat_created_time_us') IS NULL\n"
            << "    ALTER TABLE fishnest.demand_main_info ADD stat_created_time_us bigint NULL;\n"
            << "IF COL_LENGTH(N'fishnest.demand_main_info', N'stat_last_observed_time_us') IS NULL\n"
            << "    ALTER TABLE fishnest.demand_main_info ADD stat_last_observed_time_us bigint NULL;\n"
            << "IF COL_LENGTH(N'fishnest.demand_main_info', N'stat_hit_count') IS NULL\n"
            << "    ALTER TABLE fishnest.demand_main_info ADD stat_hit_count bigint NULL;\n"
            << "IF COL_LENGTH(N'fishnest.demand_main_info', N'recent_settlement_task_key') IS NULL\n"
            << "    ALTER TABLE fishnest.demand_main_info ADD recent_settlement_task_key nvarchar(80) NULL;\n"
            << "IF COL_LENGTH(N'fishnest.demand_main_info', N'recent_settlement_time_us') IS NULL\n"
            << "    ALTER TABLE fishnest.demand_main_info ADD recent_settlement_time_us bigint NULL;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'structure_role') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN structure_role;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'derived_method_key') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN derived_method_key;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'derived_causal_key') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN derived_causal_key;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'is_closed') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN is_closed;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'blocks_parent') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN blocks_parent;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'subject_key') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN subject_key;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'scene_key') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN scene_key;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'target_host_key') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN target_host_key;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'current_state_key') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN current_state_key;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'target_state_key') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN target_state_key;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'target_feature_key') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN target_feature_key;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'task_key') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN task_key;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'relation_mask') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN relation_mask;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'safety_weight') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN safety_weight;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'service_weight') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN service_weight;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'safety_settled') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN safety_settled;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'service_settled') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN service_settled;\n"
            << "IF COL_LENGTH(N'fishnest.demand_tree_node', N'valid_until_us') IS NOT NULL\n"
            << "    ALTER TABLE fishnest.demand_tree_node DROP COLUMN valid_until_us;\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_demand_tree_node_key' AND object_id = OBJECT_ID(N'fishnest.demand_tree_node'))\n"
            << "    CREATE INDEX IX_demand_tree_node_key ON fishnest.demand_tree_node(node_key, parent_key);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_demand_tree_node_snapshot_key' AND object_id = OBJECT_ID(N'fishnest.demand_tree_node'))\n"
            << "    CREATE INDEX IX_demand_tree_node_snapshot_key ON fishnest.demand_tree_node(snapshot_id, node_key, parent_key);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_demand_tree_node_parent_row' AND object_id = OBJECT_ID(N'fishnest.demand_tree_node'))\n"
            << "    CREATE INDEX IX_demand_tree_node_parent_row ON fishnest.demand_tree_node(snapshot_id, parent_row_index, row_index);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_demand_main_info_node' AND object_id = OBJECT_ID(N'fishnest.demand_main_info'))\n"
            << "    CREATE INDEX IX_demand_main_info_node ON fishnest.demand_main_info(snapshot_id, node_key);\n"
            << "IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_demand_main_info_node_row' AND object_id = OBJECT_ID(N'fishnest.demand_main_info'))\n"
            << "    CREATE INDEX IX_demand_main_info_node_row ON fishnest.demand_main_info(snapshot_id, node_row_index);\n";
        return SQL.str();
    }

    std::string 私有_需求树SQL视图脚本()
    {
        std::ostringstream SQL;
        SQL << "EXEC(N'CREATE OR ALTER VIEW fishnest.v_current_demand_main_info AS\n"
            << "SELECT m.*\n"
            << "FROM fishnest.demand_main_info m\n"
            << "WHERE m.snapshot_id = (SELECT TOP (1) snapshot_id FROM fishnest.demand_tree_snapshot ORDER BY captured_at DESC);');\n"
            << "EXEC(N'CREATE OR ALTER VIEW fishnest.v_current_demand_tree_nodes AS\n"
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
            << "FROM fishnest.demand_tree_node n\n"
            << "LEFT JOIN fishnest.demand_main_info m\n"
            << "    ON m.snapshot_id = n.snapshot_id\n"
            << "    AND m.node_row_index = n.row_index\n"
            << "WHERE n.snapshot_id = (SELECT TOP (1) snapshot_id FROM fishnest.demand_tree_snapshot ORDER BY captured_at DESC);');\n"
            << "EXEC(N'CREATE OR ALTER VIEW fishnest.v_current_demand_panel_nodes AS\n"
            << "WITH demand_rows AS (\n"
            << "    SELECT *\n"
            << "    FROM fishnest.v_current_demand_tree_nodes\n"
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
            << "    LEFT JOIN fishnest.v_current_world_tree_nodes subject\n"
            << "        ON subject.node_key COLLATE Latin1_General_BIN2 = d.subject_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN fishnest.v_current_world_tree_nodes scene\n"
            << "        ON scene.node_key COLLATE Latin1_General_BIN2 = d.scene_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN fishnest.v_current_world_tree_nodes target_host\n"
            << "        ON target_host.node_key COLLATE Latin1_General_BIN2 = d.target_host_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN fishnest.v_current_world_tree_nodes current_state\n"
            << "        ON current_state.node_key COLLATE Latin1_General_BIN2 = d.current_state_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN fishnest.v_current_world_tree_nodes target_state\n"
            << "        ON target_state.node_key COLLATE Latin1_General_BIN2 = d.target_state_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN fishnest.v_current_lexeme_nodes target_feature\n"
            << "        ON target_feature.node_key COLLATE Latin1_General_BIN2 = d.target_feature_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN fishnest.v_current_task_tree_nodes task\n"
            << "        ON task.node_key COLLATE Latin1_General_BIN2 = d.task_key COLLATE Latin1_General_BIN2\n"
            << "    LEFT JOIN fishnest.v_current_task_tree_nodes recent_task\n"
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
            << "DELETE FROM fishnest.demand_main_info;\n"
            << "DELETE FROM fishnest.demand_tree_node;\n"
            << "DELETE FROM fishnest.demand_tree_snapshot;\n"
            << "DECLARE @snapshot_id uniqueidentifier = NEWID();\n"
            << "INSERT INTO fishnest.demand_tree_snapshot (snapshot_id, captured_at, source_kind, source_reason, root_key, node_count)\n"
            << "VALUES (@snapshot_id, SYSUTCDATETIME(), N'demand_tree_update', "
            << 私有_SQL字符串(来源原因)
            << ", "
            << 私有_SQL字符串(根主键)
            << ", "
            << 行集.size()
            << ");\n";
        for (const auto& 行 : 行集) {
            SQL << "INSERT INTO fishnest.demand_tree_node (snapshot_id, row_index, node_key, parent_key, parent_row_index, depth, sibling_index, direct_child_count, path_text, node_name, target_semantics, logic_group_type) VALUES (@snapshot_id, "
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
            SQL << "INSERT INTO fishnest.demand_main_info (snapshot_id, node_key, node_row_index, description_key, stat_created_time_us, stat_last_observed_time_us, stat_hit_count, is_closed, blocks_parent, subject_key, scene_key, target_host_key, current_state_key, target_state_key, target_feature_key, task_key, relation_mask, safety_weight, service_weight, safety_settled, service_settled, valid_until_us, recent_settlement_task_key, recent_settlement_time_us) VALUES (@snapshot_id, "
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
            << "    FROM fishnest.demand_tree_node child\n"
            << "    LEFT JOIN fishnest.demand_tree_node parent\n"
            << "        ON parent.snapshot_id = child.snapshot_id AND parent.row_index = child.parent_row_index\n"
            << "    WHERE child.snapshot_id = @snapshot_id\n"
            << "        AND child.parent_row_index IS NOT NULL\n"
            << "        AND parent.row_index IS NULL\n"
            << ")\n"
            << "    THROW 51002, N'demand_tree_node parent_row_index invalid', 1;\n"
            << "IF EXISTS (\n"
            << "    SELECT 1\n"
            << "    FROM fishnest.demand_main_info info\n"
            << "    LEFT JOIN fishnest.demand_tree_node node\n"
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
        auto* 基础节点 = 世界树.基础信息().查找主键(引用.主键);
        return dynamic_cast<状态节点类*>(基础节点);
    }

    // 功能：从指定来源读取数据或状态。
    时间戳 私有_读取状态时间(const 状态节点主信息类* 主信息) noexcept
    {
        if (!主信息) {
            return 0;
        }
        return 主信息->发生时间 != 0 ? 主信息->发生时间 : 主信息->收到时间;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 特征节点主信息类* 私有_取特征主信息(const 可解析引用<特征节点类>& 引用) noexcept
    {
        auto* 特征节点 = 引用.指针;
        if (!特征节点 && !引用.主键.empty()) {
            auto* 基础节点 = 世界树.基础信息().查找主键(引用.主键);
            特征节点 = dynamic_cast<特征节点类*>(基础节点);
        }
        return 特征节点 ? 世界树.特征().取特征主信息(特征节点) : nullptr;
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

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_语素入口主键相等(const 语素入口节点类* 左, const 语素入口节点类* 右) noexcept
    {
        if (左 == 右) {
            return 左 != nullptr;
        }
        if (!左 || !右) {
            return false;
        }
        const auto 左主键 = 左->获取主键();
        const auto 右主键 = 右->获取主键();
        return !左主键.empty() && 左主键 == 右主键;
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
        return 私有_语素入口主键相等(
                目标特征类型,
                私有_自检维护管理状态词("学习维护管理状态"))
            || 私有_语素入口主键相等(
                目标特征类型,
                私有_自检维护管理状态词("方法维护管理状态"))
            || 私有_语素入口主键相等(
                目标特征类型,
                私有_自检维护管理状态词("世界模型维护管理状态"))
            || 私有_语素入口主键相等(
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
        const auto* 主信息 = 状态
            ? 世界树.状态().取状态主信息(const_cast<状态节点类*>(状态))
            : nullptr;
        return 主信息 ? 私有_引用主键_需求方向(主信息->状态主体) : std::string{};
    }

    // 功能：服务所在模块的内部辅助流程。
    场景节点类* 私有_取所属场景(状态节点类* 状态节点) noexcept
    {
        if (!状态节点) {
            return nullptr;
        }
        return dynamic_cast<场景节点类*>(状态节点->父);
    }

    // 功能：解析输入文本、消息、场景或运行包。
    状态节点类* 私有_解析状态节点_需求校验(
        const std::uintptr_t 指针,
        const std::string& 主键) noexcept
    {
        if (指针 != 0) {
            auto* 候选 = reinterpret_cast<状态节点类*>(指针);
            if (世界树.状态().取状态主信息(候选)) {
                return 候选;
            }
        }
        if (主键.empty()) {
            return nullptr;
        }
        auto* 基础节点 = 世界树.基础信息().查找主键(主键);
        auto* 状态节点 = dynamic_cast<状态节点类*>(基础节点);
        return 世界树.状态().取状态主信息(状态节点) ? 状态节点 : nullptr;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    特征节点类* 私有_解析状态特征节点_需求校验(
        const 状态节点类* 状态) noexcept
    {
        const auto* 状态主信息 = 状态
            ? 世界树.状态().取状态主信息(const_cast<状态节点类*>(状态))
            : nullptr;
        if (!状态主信息) {
            return nullptr;
        }
        auto* 特征节点 = 状态主信息->状态特征.指针;
        if (!特征节点 && !状态主信息->状态特征.主键.empty()) {
            特征节点 = dynamic_cast<特征节点类*>(
                世界树.基础信息().查找主键(状态主信息->状态特征.主键));
        }
        return 世界树.特征().取特征主信息(特征节点) ? 特征节点 : nullptr;
    }

    // 功能：服务所在模块的内部辅助流程。
    const 语素入口节点类* 私有_状态特征类型_需求校验(
        const 状态节点类* 状态) noexcept
    {
        const auto* 特征主信息 =
            世界树.特征().取特征主信息(私有_解析状态特征节点_需求校验(状态));
        return 特征主信息 ? 特征主信息->类型 : nullptr;
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

    bool 私有_I64值域项是点(const I64值域项& 项, I64* 输出点 = nullptr) noexcept
    {
        if (const auto* 点 = std::get_if<I64>(&项)) {
            if (输出点) {
                *输出点 = *点;
            }
            return true;
        }
        const auto* 区间 = std::get_if<I64区间>(&项);
        if (!区间 || !区间->有效() || 区间->低值 != 区间->高值) {
            return false;
        }
        if (输出点) {
            *输出点 = 区间->低值;
        }
        return true;
    }

    bool 私有_值域是I64点值域(const 特征值域& 值域, I64* 输出点 = nullptr) noexcept
    {
        const auto* I64域 = std::get_if<I64值域>(&值域);
        if (!I64域 || I64域->项集.size() != 1) {
            return false;
        }
        return 私有_I64值域项是点(I64域->项集.front(), 输出点);
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_特征值命中I64值域(const 特征值& 值, const 特征值域& 值域) noexcept
    {
        const auto* 标量 = std::get_if<I64>(&值);
        if (!标量) {
            return false;
        }
        const auto* I64域 = std::get_if<I64值域>(&值域);
        if (!I64域) {
            return false;
        }
        for (const auto& 项 : I64域->项集) {
            if (const auto* 点 = std::get_if<I64>(&项)) {
                if (*点 == *标量) {
                    return true;
                }
            } else if (const auto* 区间 = std::get_if<I64区间>(&项)) {
                if (区间->有效() && 区间->包含(*标量)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool 私有_抽象特征是离散点枚举(
        const 抽象特征节点类* 抽象特征,
        bool* 输出有子状态 = nullptr) noexcept
    {
        if (输出有子状态) {
            *输出有子状态 = false;
        }
        if (!抽象特征) {
            return false;
        }

        std::size_t 点状态数 = 0;
        for (auto* 子节点 : 私有_直接基础信息子节点_按类型<抽象特征主信息类>(世界树.基础信息(), 抽象特征)) {
            auto* 子抽象 = static_cast<抽象特征节点类*>(子节点);
            const auto* 子主信息 = 世界树.特征().取抽象特征主信息(子抽象);
            if (!子主信息 || !子主信息->是叶子特征() || !子主信息->值域.has_value()) {
                return false;
            }
            if (!私有_值域是I64点值域(子主信息->值域.value())) {
                return false;
            }
            ++点状态数;
        }

        if (输出有子状态) {
            *输出有子状态 = 点状态数 > 0;
        }
        return 点状态数 >= 2;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_目标值命中枚举子状态值域(
        const 抽象特征节点类* 抽象特征,
        const 特征值& 目标值) noexcept
    {
        if (!抽象特征 || !特征值有效(目标值)) {
            return false;
        }
        bool 有子状态 = false;
        for (auto* 子节点 : 私有_直接基础信息子节点_按类型<抽象特征主信息类>(世界树.基础信息(), 抽象特征)) {
            有子状态 = true;
            auto* 子抽象 = static_cast<抽象特征节点类*>(子节点);
            const auto* 子主信息 = 世界树.特征().取抽象特征主信息(子抽象);
            if (子主信息
                && 子主信息->是叶子特征()
                && 子主信息->值域.has_value()
                && 私有_特征值命中I64值域(目标值, 子主信息->值域.value())) {
                return true;
            }
        }
        if (有子状态) {
            return false;
        }

        const auto* 抽象主信息 = 世界树.特征().取抽象特征主信息(抽象特征);
        return 抽象主信息
            && 抽象主信息->是叶子特征()
            && 抽象主信息->值域.has_value()
            && 私有_特征值命中I64值域(目标值, 抽象主信息->值域.value());
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_方法结果项精确生产目标特征(
        const 结构_方法结果项& 结果项,
        const 语素入口节点类* 目标特征类型) noexcept
    {
        if (!目标特征类型 || 结果项.关系目标特征类型由参数决定()) {
            return false;
        }
        return 私有_语素入口主键相等(
            结果项.关系目标特征类型(),
            目标特征类型);
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_方法首节点生产目标特征(
        const 方法类::节点类* 方法,
        const 语素入口节点类* 目标特征类型) noexcept
    {
        if (!方法
            || 方法->主信息.节点种类() != 枚举_方法节点种类::方法首节点
            || !目标特征类型) {
            return false;
        }

        const auto* 首信息 = 方法->主信息.取首节点信息();
        if (!首信息 || 首信息->能力.是否根写入原语) {
            return false;
        }

        for (const auto& 结果项 : 首信息->能力.结果包.结果项集) {
            if (私有_方法结果项精确生产目标特征(结果项, 目标特征类型)) {
                return true;
            }
        }
        return false;
    }

    // 功能：按条件查找目标对象、方法或事实。
    方法类::节点类* 私有_查找方法节点_按主键_需求校验(
        const std::string& 方法主键) noexcept
    {
        if (方法主键.empty()) {
            return nullptr;
        }

        auto* 自我存在 = 世界树.自我指针;
        auto* 方法根节点 = 自我存在 ? 世界树.存在().获取方法根节点(自我存在) : nullptr;
        if (!方法根节点) {
            return nullptr;
        }

        std::vector<方法类::节点类*> 待访问{};
        std::vector<方法类::节点类*> 已入栈{};

        const auto 已记录节点 = [&](const 方法类::节点类* 节点) noexcept {
            for (const auto* 已记录 : 已入栈) {
                if (已记录 == 节点) {
                    return true;
                }
            }
            return false;
        };

        const auto 压入待访问 = [&](方法类::节点类* 节点) noexcept {
            if (!节点 || 已记录节点(节点)) {
                return;
            }
            已入栈.push_back(节点);
            待访问.push_back(节点);
        };

        const auto 压入直接子链 = [&](方法类::节点类* 父节点) noexcept {
            auto* 首子 = 父节点
                ? static_cast<方法类::节点类*>(父节点->子)
                : nullptr;
            if (!首子) {
                return;
            }

            auto* 当前子 = 首子;
            std::size_t 兄弟保护计数 = 0;
            do {
                压入待访问(当前子);
                当前子 = static_cast<方法类::节点类*>(当前子->下);
                ++兄弟保护计数;
            } while (当前子 && 当前子 != 首子 && 兄弟保护计数 < 4096);
        };

        压入待访问(方法根节点);

        std::size_t 访问计数 = 0;
        constexpr std::size_t 访问保护上限 = 65536;
        while (!待访问.empty() && 访问计数 < 访问保护上限) {
            auto* 当前 = 待访问.back();
            待访问.pop_back();
            ++访问计数;

            if (当前
                && 当前->主信息.节点种类() == 枚举_方法节点种类::方法首节点
                && 当前->获取主键() == 方法主键) {
                return 当前;
            }
            压入直接子链(当前);
        }
        return nullptr;
    }

    // 功能：解析输入文本、消息、场景或运行包。
    方法类::节点类* 私有_解析方法引用_需求校验(
        const 可解析引用<方法节点类>& 引用) noexcept
    {
        if (引用.指针) {
            return reinterpret_cast<方法类::节点类*>(引用.指针);
        }
        return 私有_查找方法节点_按主键_需求校验(引用.主键);
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_方法树存在生产者(
        const 语素入口节点类* 目标特征类型) noexcept
    {
        auto* 自我存在 = 世界树.自我指针;
        auto* 方法根节点 = 自我存在 ? 世界树.存在().获取方法根节点(自我存在) : nullptr;
        if (!方法根节点 || !目标特征类型) {
            return false;
        }

        std::vector<方法类::节点类*> 待访问{};
        std::vector<方法类::节点类*> 已入栈{};

        const auto 已记录节点 = [&](const 方法类::节点类* 节点) noexcept {
            for (const auto* 已记录 : 已入栈) {
                if (已记录 == 节点) {
                    return true;
                }
            }
            return false;
        };

        const auto 压入待访问 = [&](方法类::节点类* 节点) noexcept {
            if (!节点 || 已记录节点(节点)) {
                return;
            }
            已入栈.push_back(节点);
            待访问.push_back(节点);
        };

        const auto 压入直接子链 = [&](方法类::节点类* 父节点) noexcept {
            auto* 首子 = 父节点
                ? static_cast<方法类::节点类*>(父节点->子)
                : nullptr;
            if (!首子) {
                return;
            }

            auto* 当前子 = 首子;
            std::size_t 兄弟保护计数 = 0;
            do {
                压入待访问(当前子);
                当前子 = static_cast<方法类::节点类*>(当前子->下);
                ++兄弟保护计数;
            } while (当前子 && 当前子 != 首子 && 兄弟保护计数 < 4096);
        };

        压入待访问(方法根节点);

        std::size_t 访问计数 = 0;
        constexpr std::size_t 访问保护上限 = 65536;
        while (!待访问.empty() && 访问计数 < 访问保护上限) {
            auto* 当前 = 待访问.back();
            待访问.pop_back();
            ++访问计数;

            if (私有_方法首节点生产目标特征(当前, 目标特征类型)) {
                return true;
            }
            压入直接子链(当前);
        }
        return false;
    }

    // 功能：服务所在模块的内部辅助流程。
    bool 私有_目标特征是逻辑流程产物(
        const 语素入口节点类* 目标特征类型) noexcept
    {
        return 需求类::目标特征类型是逻辑组织目标(目标特征类型)
            || 私有_语素入口主键相等(
                目标特征类型,
                需求类::OR组结算令牌目标特征类型());
    }

    // 功能：服务所在模块的内部辅助流程。
    枚举_枚举目标生产者分级 私有_查询来源因果生产者分级(
        const std::string& 来源因果主键,
        const 语素入口节点类* 目标特征类型) noexcept
    {
        if (来源因果主键.empty() || !目标特征类型) {
            return 枚举_枚举目标生产者分级::未登记;
        }
        auto* 基础节点 = 世界树.基础信息().查找主键(来源因果主键);
        auto* 因果节点 = reinterpret_cast<因果节点类*>(基础节点);
        const auto* 因果信息 = 世界树.因果().取因果主信息(因果节点);
        if (!因果信息) {
            return 枚举_枚举目标生产者分级::未登记;
        }

        if (因果信息->因方法模板.有效()) {
            auto* 方法 = 私有_解析方法引用_需求校验(因果信息->因方法模板);
            return 私有_方法首节点生产目标特征(方法, 目标特征类型)
                ? 枚举_枚举目标生产者分级::专用方法生产
                : 枚举_枚举目标生产者分级::待补齐;
        }

        if (!因果信息->因动作语义键.empty() || !因果信息->因动作名称.empty()) {
            return 枚举_枚举目标生产者分级::待补齐;
        }
        return 枚举_枚举目标生产者分级::未登记;
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
        return 私有_语素入口主键相等(
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
            << " | 说明=" << 结果.说明;
        return 输出.str();
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
    if (私有_语素入口主键相等(
            目标特征类型,
            逻辑组织目标特征类型(枚举_逻辑组织需求类型::AND组))) {
        return 枚举_逻辑组织需求类型::AND组;
    }
    if (私有_语素入口主键相等(
            目标特征类型,
            逻辑组织目标特征类型(枚举_逻辑组织需求类型::OR组))) {
        return 枚举_逻辑组织需求类型::OR组;
    }
    if (私有_语素入口主键相等(
            目标特征类型,
            逻辑组织目标特征类型(枚举_逻辑组织需求类型::方法路径组))) {
        return 枚举_逻辑组织需求类型::方法路径组;
    }
    if (私有_语素入口主键相等(
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
    if (私有_语素入口主键相等(
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

// 功能：按函数名执行对应处理。
需求类::结构_需求目标语义视图 需求类::需求目标语义视图(
    const 节点类* 需求) noexcept
{
    return 需求目标语义视图(读取需求目标特征类型(需求));
}

// 功能：根据当前输入生成目标数据、场景、动态或回执。
需求类::结构_需求方向签名 需求类::生成需求方向签名(
    const 二次特征主信息类* 比较主信息) noexcept
{
    结构_需求方向签名 签名{};
    if (!比较主信息) {
        return 签名;
    }

    auto* 结果状态 = 私有_解析状态引用(比较主信息->来源结果状态);
    auto* 起始状态 = 私有_解析状态引用(比较主信息->来源起始状态);
    const auto* 目标状态 = 结果状态 ? 结果状态 : 起始状态;
    if (目标状态) {
        签名.目标主体主键 = 私有_状态主体主键_需求方向(目标状态);
        签名.目标特征类型主键 = 私有_状态特征类型主键_需求方向(目标状态);
    }
    if (签名.目标主体主键.empty()) {
        签名.目标主体主键 = 私有_引用主键_需求方向(比较主信息->主体);
    }
    if (签名.目标特征类型主键.empty() && 起始状态) {
        签名.目标特征类型主键 = 私有_状态特征类型主键_需求方向(起始状态);
    }

    签名.方向掩码 = 比较主信息->允许方向;
    签名.方向区间 = 比较主信息->方向区间;
    if (比较主信息->差值.has_value()) {
        签名.有差值 = true;
        签名.差值 = *比较主信息->差值;
        if (签名.方向区间 == 二次特征主信息类::枚举_方向区间::未定义) {
            签名.方向区间 =
                二次特征主信息类::由差值判断方向区间(签名.差值);
        }
    }
    if (比较主信息->值形态 == 枚举_二次特征值形态::标量
        || 比较主信息->标量值 != 0) {
        签名.有标量值 = true;
        签名.标量值 = 比较主信息->标量值;
    }
    return 签名;
}

// 功能：根据当前输入生成目标数据、场景、动态或回执。
需求类::结构_需求方向签名 需求类::生成需求方向签名(
    const 二次特征节点类* 比较节点) noexcept
{
    const auto* 主信息 = 比较节点
        ? 世界树.二次特征().取二次特征主信息(比较节点)
        : nullptr;
    return 生成需求方向签名(主信息);
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
        结果.说明 = "缺目标特征或目标状态，跳过枚举目标校验";
        return 结果;
    }

    auto* 抽象根 = 世界树.抽象特征根;
    auto* 抽象特征 = 抽象根
        ? 世界树.特征().查找子抽象特征_按类型(抽象根, 结果.目标特征类型)
        : nullptr;
    bool 有枚举子状态 = false;
    结果.是枚举目标 = 私有_抽象特征是离散点枚举(抽象特征, &有枚举子状态);
    if (!结果.是枚举目标) {
        结果.说明 = 有枚举子状态
            ? "抽象特征含非点值状态，按非枚举目标处理"
            : "未识别为离散点枚举目标";
        return 结果;
    }

    结果.生产者分级 = 私有_查询枚举目标生产者分级(结果.目标特征类型);
    结果.比较符合法 = 私有_枚举目标比较符合法(
        结果.目标特征类型,
        满足关系);

    const auto* 目标状态主信息 = 世界树.状态().取状态主信息(目标状态);
    结果.目标值属于值域 = 目标状态主信息
        && 私有_目标值命中枚举子状态值域(抽象特征, 目标状态主信息->状态值);

    结果.有合法生产者 = 私有_生产者分级已有生产者(结果.生产者分级);
    结果.合法 = 结果.比较符合法
        && 结果.目标值属于值域
        && 结果.有合法生产者;

    if (!结果.比较符合法) {
        结果.说明 = 私有_枚举目标允许有序比较(结果.目标特征类型)
            ? "有序枚举目标只能使用等于或大于等于比较符"
            : "枚举目标只能使用等于比较符";
    } else if (!结果.目标值属于值域) {
        结果.说明 = "枚举目标值不属于抽象特征值域";
    } else if (!结果.有合法生产者) {
        结果.说明 = 结果.生产者分级 == 枚举_枚举目标生产者分级::待补齐
            ? "枚举目标生产者待补齐，不能进入普通执行目标"
            : "枚举目标特征未登记生产者，不能进入普通执行目标";
    } else {
        结果.说明 = "枚举目标比较符、值域和生产者分级通过";
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
        const auto 投影库连接串 = 生成SQLServerWindows认证ADO连接串(R"(.\SQLEXPRESS)", "FishnestProjection");
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

    const auto* 状态主信息 = 状态主信息_已加锁(
        主信息.需求状态.指针
            ? 主信息.需求状态.指针
            : (主信息.被需求当前状态.指针 ? 主信息.被需求当前状态.指针 : nullptr));
    if (状态主信息) {
        if (!输出.作用对象.有效()) {
            if (auto* 主体 = 状态主信息->状态主体.指针) {
                if (主体->主信息
                    && 主体->主信息->主信息类型 == 枚举_主信息类型::存在) {
                    输出.作用对象.绑定(reinterpret_cast<存在节点类*>(主体));
                }
            } else if (!状态主信息->状态主体.主键.empty()) {
                输出.作用对象.主键 = 状态主信息->状态主体.主键;
            }
        }

        if (!输出.目标特征类型) {
            const auto* 特征主信息 = 状态主信息->状态特征.指针
                ? dynamic_cast<const 特征节点主信息类*>(状态主信息->状态特征.指针->主信息)
                : nullptr;
            输出.目标特征类型 = 特征主信息 ? 特征主信息->类型 : nullptr;
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

    const auto* 初始状态主信息 = 世界树.状态().取状态主信息(初始状态);
    const auto* 结果状态主信息 = 世界树.状态().取状态主信息(结果状态);
    if (!初始状态主信息 || !结果状态主信息) {
        return nullptr;
    }

    const auto* 初始特征主信息 = 私有_取特征主信息(初始状态主信息->状态特征);
    const auto* 结果特征主信息 = 私有_取特征主信息(结果状态主信息->状态特征);
    const auto* 初始特征类型 = 初始特征主信息 ? 初始特征主信息->类型 : nullptr;
    const auto* 结果特征类型 = 结果特征主信息 ? 结果特征主信息->类型 : nullptr;
    if (!初始特征类型 || !结果特征类型 || !私有_语素入口主键相等(初始特征类型, 结果特征类型)) {
        return nullptr;
    }

    const auto 比较方法结果 = 世界树.特征().获取比较方法结果(
        初始特征类型,
        初始状态主信息->状态值,
        结果状态主信息->状态值,
        初始特征主信息,
        结果特征主信息);
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
        私有_读取状态时间(初始状态主信息),
        私有_读取状态时间(结果状态主信息)
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
    模板.主体 = 初始状态主信息->状态主体;
    if (初始状态主信息->状态主体.指针 != 结果状态主信息->状态主体.指针
        && 初始状态主信息->状态主体.主键 != 结果状态主信息->状态主体.主键) {
        模板.客体 = 结果状态主信息->状态主体;
    }
    模板.左对象 = 私有_生成引用(static_cast<基础信息节点类*>(初始状态));
    模板.右对象 = 私有_生成引用(static_cast<基础信息节点类*>(结果状态));
    模板.来源起始状态 = 私有_生成引用(初始状态);
    模板.来源结果状态 = 私有_生成引用(结果状态);
    模板.来源场景 = 私有_生成引用(所属场景);
    模板.来源类型 = 枚举_二次特征来源类型::状态;
    模板.来源时间段 = 来源时间段;
    模板.刷新度量签名_链键();

    auto* 节点 = 世界树.二次特征().获取或创建子二次特征_按签名(所属场景, 模板);
    if (!节点) {
        return nullptr;
    }

    (void)世界树.二次特征().绑定场景(节点, 所属场景);
    (void)世界树.二次特征().设置生成语义(
        节点,
        枚举_二次特征域::存在,
        枚举_二次特征粒度::当前场景,
        枚举_二次特征基准类型::无,
        枚举_时间归一方式::无,
        枚举_二次特征值形态::标量);
    (void)世界树.二次特征().绑定来源(
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
    (void)世界树.二次特征().设置主体客体(
        节点,
        初始状态主信息->状态主体.指针,
        (初始状态主信息->状态主体.指针 != 结果状态主信息->状态主体.指针
            && 初始状态主信息->状态主体.主键 != 结果状态主信息->状态主体.主键)
            ? 结果状态主信息->状态主体.指针
            : nullptr);
    (void)世界树.二次特征().设置左右对象(
        节点,
        static_cast<基础信息节点类*>(初始状态),
        static_cast<基础信息节点类*>(结果状态));
    (void)世界树.二次特征().写入差值结果(
        节点,
        比较方法结果.左比较值,
        比较方法结果.右比较值);

    const auto* 二次特征主信息 = 世界树.二次特征().取二次特征主信息(节点);
    const auto 差值 = 二次特征主信息 && 二次特征主信息->差值.has_value()
        ? *二次特征主信息->差值
        : 0;
    (void)世界树.二次特征().写入标量值(节点, 差值, 差值 != 0, 10000);
    return 节点;
}
