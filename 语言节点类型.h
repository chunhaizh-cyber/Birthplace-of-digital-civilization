#pragma once

#include "双向链表模板核心.h"

class 自然语言成分基类;

using 自然语言节点类 = 链表模板<自然语言成分基类*>::节点类;
using 文章节点类 = 自然语言节点类;
using 段落节点类 = 自然语言节点类;
using 自然句节点类 = 自然语言节点类;
using 子句节点类 = 自然语言节点类;