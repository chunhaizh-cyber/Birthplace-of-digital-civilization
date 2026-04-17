#pragma once

#include "任务主信息类.h"

class 任务类 : public 链表模板<任务主信息类> {
public:
    using 基类 = 链表模板<任务主信息类>;
    using 节点类 = typename 基类::节点类;

public:
    节点类* 创建任务头结点(
        需求节点类* 来源需求,
        节点类* 上级任务节点 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "任务类::创建任务头结点");

    节点类* 创建任务步骤节点(
        节点类* 所属任务头结点,
        节点类* 上级任务节点 = nullptr,
        方法节点类* 当前方法首节点 = nullptr,
        状态节点类* 步骤目标状态 = nullptr,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "任务类::创建任务步骤节点");
};
