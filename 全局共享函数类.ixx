module;

#include "基础数据类型.h"

export module 全局共享函数类;

// 没有领域自定义主信息 / 领域类型参与的通用函数统一放在本模块。
// 新增前必须先扫描 说明书/函数功能说明文档.md，确认没有同名或同功能函数。
// 本模块只允许直接包含 基础数据类型.h。
// 必要的非主信息类自定义数据结构应先迁移到 基础数据类型.h，再供本模块使用。
// 涉及语素、基础信息、主信息类、存在、场景、需求、任务、方法等领域类型的共享函数，
// 放入对应功能类、专用类或对应功能头文件，不进入本模块。

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
