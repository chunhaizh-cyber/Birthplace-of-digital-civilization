#include "基础信息主信息类.h"

#include "需求类.h"
#include "任务类.h"
#include "方法类.h"

存在节点主信息类::~存在节点主信息类()
{
    auto*& 需求根 = reinterpret_cast<需求类::节点类*&>(需求根节点);
    auto*& 任务根 = reinterpret_cast<任务类::节点类*&>(任务根节点);
    auto*& 方法根 = reinterpret_cast<方法类::节点类*&>(方法根节点);

    需求类::释放树_独立(需求根);
    任务类::释放树_独立(任务根);
    方法类::释放树_独立(方法根);
}
