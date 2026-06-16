module;

#include "语素节点类型.h"

export module 全局共享函数类;

// 没有自定义主信息类参与的函数统一放在本模块。
// 新增前必须先扫描 说明书/函数功能说明文档.md，确认没有同名或同功能函数。

// 功能：比较两个语素入口节点是否指向同一入口；先比较指针，指针不同再比较主键。
export inline bool 全局_语素入口同一(const 语素入口节点类* 左, const 语素入口节点类* 右) noexcept
{
    if (左 == 右) {
        return true;
    }
    if (!左 || !右) {
        return false;
    }
    return 左->获取主键() == 右->获取主键();
}
