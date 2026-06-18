module;

#include <string>

export module 全局共享函数类;

// 没有领域自定义类 / 结构体 / 节点类型参与的通用函数统一放在本模块。
// 新增前必须先扫描 说明书/函数功能说明文档.md，确认没有同名或同功能函数。
// 本模块可使用 std 类型以及 I64、VecI64、VecIU64 等基础别名 / 基础容器，
// 但必须先确认不会造成模块 import 环。
// 涉及语素、基础信息、主信息类、存在、场景、特征、需求、任务、方法等领域类型的共享函数，
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
