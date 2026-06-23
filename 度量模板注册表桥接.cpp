#include "度量模板注册表桥接.h"

import 度量模板注册表模块;

namespace {
    // 功能：把模块内部刻度语义转为跨翻译单元桥接代码。
    枚举_度量刻度语义代码 私有_桥接刻度语义(
        const 度量模板注册表类::枚举_刻度语义 语义) noexcept
    {
        using 源 = 度量模板注册表类::枚举_刻度语义;
        switch (语义) {
        case 源::低: return 枚举_度量刻度语义代码::低;
        case 源::中: return 枚举_度量刻度语义代码::中;
        case 源::高: return 枚举_度量刻度语义代码::高;
        case 源::小差异: return 枚举_度量刻度语义代码::小差异;
        case 源::中差异: return 枚举_度量刻度语义代码::中差异;
        case 源::大差异: return 枚举_度量刻度语义代码::大差异;
        case 源::低相似: return 枚举_度量刻度语义代码::低相似;
        case 源::中相似: return 枚举_度量刻度语义代码::中相似;
        case 源::高相似: return 枚举_度量刻度语义代码::高相似;
        case 源::覆盖低: return 枚举_度量刻度语义代码::覆盖低;
        case 源::覆盖中: return 枚举_度量刻度语义代码::覆盖中;
        case 源::覆盖高: return 枚举_度量刻度语义代码::覆盖高;
        case 源::齐全: return 枚举_度量刻度语义代码::齐全;
        case 源::缺A: return 枚举_度量刻度语义代码::缺A;
        case 源::缺B: return 枚举_度量刻度语义代码::缺B;
        case 源::都缺: return 枚举_度量刻度语义代码::都缺;
        default: return 枚举_度量刻度语义代码::未定义;
        }
    }
}

// 功能：把刻度语义代码渲染为日志、显示或语素入口名称种子。
const char* 度量刻度语义显示文本(const 枚举_度量刻度语义代码 语义) noexcept
{
    switch (语义) {
    case 枚举_度量刻度语义代码::低: return "低";
    case 枚举_度量刻度语义代码::中: return "中";
    case 枚举_度量刻度语义代码::高: return "高";
    case 枚举_度量刻度语义代码::小差异: return "小差异";
    case 枚举_度量刻度语义代码::中差异: return "中差异";
    case 枚举_度量刻度语义代码::大差异: return "大差异";
    case 枚举_度量刻度语义代码::低相似: return "低相似";
    case 枚举_度量刻度语义代码::中相似: return "中相似";
    case 枚举_度量刻度语义代码::高相似: return "高相似";
    case 枚举_度量刻度语义代码::覆盖低: return "覆盖低";
    case 枚举_度量刻度语义代码::覆盖中: return "覆盖中";
    case 枚举_度量刻度语义代码::覆盖高: return "覆盖高";
    case 枚举_度量刻度语义代码::齐全: return "齐全";
    case 枚举_度量刻度语义代码::缺A: return "缺A";
    case 枚举_度量刻度语义代码::缺B: return "缺B";
    case 枚举_度量刻度语义代码::都缺: return "都缺";
    default: return "";
    }
}

// 功能：初始化相关对象、状态或运行上下文。
void 初始化度量模板注册表() noexcept
{
    度量模板注册表类::实例().注册默认模板();
}

// 功能：读取并返回指定对象、状态或运行材料。
std::vector<结构_度量刻度片_桥接> 获取度量默认刻度_由链键(
    const std::string& 链键,
    const std::string& 特征类型主键)
{
    std::vector<结构_度量刻度片_桥接> 输出{};
    const auto 刻度列表 = 度量模板注册表类::实例().默认刻度_由链键(链键, 特征类型主键);
    输出.reserve(刻度列表.size());
    for (const auto& 刻度 : 刻度列表) {
        输出.push_back({
            刻度.区间.低值,
            刻度.区间.高值,
            私有_桥接刻度语义(刻度.语义),
            刻度.是小区间,
        });
    }
    return 输出;
}
