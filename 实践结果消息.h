#pragma once

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

enum class 枚举_实践结果来源承接类型 : std::uint8_t {
    未定义 = 0,
    执行 = 1,
    缺口承接 = 2,
    外部回流 = 3,
    观察 = 4,
};

struct 结构_实践结果带值特征 {
    std::string 特征名{};
    std::int64_t 数值 = 0;
};

struct 结构_实践结果消息 {
    枚举_实践结果来源承接类型 来源承接类型 = 枚举_实践结果来源承接类型::未定义;
    std::uintptr_t 来源宿主任务指针 = 0;
    std::uintptr_t 来源当前步骤指针 = 0;
    std::uintptr_t 来源当前方法首节点指针 = 0;
    std::uintptr_t 最近结果节点指针或镜像 = 0;
    std::string 动作分类{};
    std::string 动作名称{};
    std::vector<结构_实践结果带值特征> 实际结果特征列表{};
    std::string 当前步骤后状态建议{};
    std::string 当前下一步去向建议{};
    bool 是否叶子完成 = false;
    std::vector<std::string> 命中需求回挂提示列表{};
    std::string 摘要{};
};

inline const char* 实践结果来源承接类型文本(枚举_实践结果来源承接类型 类型) noexcept
{
    switch (类型) {
    case 枚举_实践结果来源承接类型::执行: return "执行";
    case 枚举_实践结果来源承接类型::缺口承接: return "缺口承接";
    case 枚举_实践结果来源承接类型::外部回流: return "外部回流";
    case 枚举_实践结果来源承接类型::观察: return "观察";
    default: return "未定义";
    }
}

inline std::string 构造实践结果消息摘要(const 结构_实践结果消息& 消息)
{
    std::ostringstream 输出;
    输出 << "来源=" << 实践结果来源承接类型文本(消息.来源承接类型)
        << " | 宿主=" << 消息.来源宿主任务指针
        << " | 步骤=" << 消息.来源当前步骤指针
        << " | 方法=" << 消息.来源当前方法首节点指针;
    if (消息.最近结果节点指针或镜像 != 0) {
        输出 << " | 最近结果=" << 消息.最近结果节点指针或镜像;
    }
    if (!消息.动作分类.empty()) {
        输出 << " | 动作分类=" << 消息.动作分类;
    }
    if (!消息.动作名称.empty()) {
        输出 << " | 动作名=" << 消息.动作名称;
    }
    if (!消息.实际结果特征列表.empty()) {
        输出 << " | 特征=";
        for (std::size_t i = 0; i < 消息.实际结果特征列表.size(); ++i) {
            const auto& 特征 = 消息.实际结果特征列表[i];
            if (i > 0) {
                输出 << ",";
            }
            输出 << 特征.特征名 << "=" << 特征.数值;
        }
    }
    if (!消息.当前步骤后状态建议.empty()) {
        输出 << " | 步骤建议=" << 消息.当前步骤后状态建议;
    }
    if (!消息.当前下一步去向建议.empty()) {
        输出 << " | 去向建议=" << 消息.当前下一步去向建议;
    }
    if (消息.是否叶子完成) {
        输出 << " | 叶子完成=是";
    }
    if (!消息.命中需求回挂提示列表.empty()) {
        输出 << " | 回挂提示=";
        for (std::size_t i = 0; i < 消息.命中需求回挂提示列表.size(); ++i) {
            if (i > 0) {
                输出 << ",";
            }
            输出 << 消息.命中需求回挂提示列表[i];
        }
    }
    return 输出.str();
}
