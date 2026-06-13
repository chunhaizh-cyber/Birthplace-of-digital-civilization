module;

#include <string_view>

export module 动作闭环特征定义;

export namespace 动作闭环特征定义 {
    inline constexpr std::string_view 类型_动作事务ID = "类型_动作事务ID";
    inline constexpr std::string_view 类型_来源任务 = "类型_来源任务";
    inline constexpr std::string_view 类型_来源方法 = "类型_来源方法";
    inline constexpr std::string_view 类型_目标值 = "类型_目标值";
    inline constexpr std::string_view 类型_预测值 = "类型_预测值";
    inline constexpr std::string_view 类型_实际值 = "类型_实际值";
    inline constexpr std::string_view 类型_执行报告值 = "类型_执行报告值";
    inline constexpr std::string_view 类型_目标偏差 = "类型_目标偏差";
    inline constexpr std::string_view 类型_预测偏差 = "类型_预测偏差";
    inline constexpr std::string_view 类型_报告偏差 = "类型_报告偏差";
    inline constexpr std::string_view 类型_回读置信度_Q10000 = "类型_回读置信度_Q10000";
    inline constexpr std::string_view 类型_动作验证状态 = "类型_动作验证状态";
    inline constexpr std::string_view 类型_事实提交等级 = "类型_事实提交等级";
    inline constexpr std::string_view 类型_纠偏建议 = "类型_纠偏建议";
}
