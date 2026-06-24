module;

#include "基础数据类型.h"

export module 全局共享函数类;

// 没有领域自定义主信息 / 领域类型参与的通用函数统一放在本模块。
// 新增前必须先扫描 说明书/函数功能说明文档.md，确认没有同名或同功能函数。
// 本模块可直接包含不含项目自定义数据结构的标准库或纯工具头。
// 必要的非主信息类通用自定义数据结构应先迁移到 基础数据类型.h，再供本模块使用。
// 不直接包含含项目自定义数据结构的领域头。
// 涉及语素、基础信息、主信息类、存在、场景、需求、任务、方法等领域类型的共享函数，
// 放入对应功能类、专用类或对应功能头文件，不进入本模块。

// 功能：对 I64 执行带上下限保护的饱和加法。
export inline I64 饱和增加(I64 左值, I64 右值) noexcept
{
    if (右值 > 0 && 左值 > (std::numeric_limits<I64>::max)() - 右值) {
        return (std::numeric_limits<I64>::max)();
    }
    if (右值 < 0 && 左值 < (std::numeric_limits<I64>::min)() - 右值) {
        return (std::numeric_limits<I64>::min)();
    }
    return 左值 + 右值;
}

// 功能：返回两个 I64 值的非饱和绝对差，保持原调用点既有算术语义。
export inline I64 绝对差I64(I64 左值, I64 右值) noexcept
{
    return 左值 >= 右值 ? 左值 - 右值 : 右值 - 左值;
}

// 功能：返回两个 I64 边界值的非负正向差，反向时返回 0。
export inline I64 正差I64(I64 最大值, I64 最小值) noexcept
{
    return 最大值 >= 最小值 ? 最大值 - 最小值 : 0;
}

// 功能：返回两个闭区间之间的 I64 间隔，区间重叠时返回 0。
export inline I64 区间间隔I64(I64 第一最小值, I64 第一最大值, I64 第二最小值, I64 第二最大值) noexcept
{
    if (第一最大值 < 第二最小值) return 第二最小值 - 第一最大值;
    if (第二最大值 < 第一最小值) return 第一最小值 - 第二最大值;
    return 0;
}

// 功能：将布尔值格式化为中文显示文本。
export constexpr const char* 布尔文本_是或否(bool 值) noexcept
{
    return 值 ? "是" : "否";
}

// 功能：将布尔值格式化为数字协议文本。
export constexpr const char* 布尔文本_一或零(bool 值) noexcept
{
    return 值 ? "1" : "0";
}

// 功能：将布尔值格式化为英文调试文本。
export inline std::string 布尔文本_true或false(bool 值)
{
    return 值 ? "true" : "false";
}
