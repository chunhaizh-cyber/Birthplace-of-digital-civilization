#pragma once

#include "双向链表模板核心.h"

class 语素基类;

using 语素节点类 = 链表模板<语素基类*>::节点类;
using 词节点类 = 语素节点类;
using 词性节点类 = 语素节点类;
using 短语节点类 = 语素节点类;
using 短语子节点类 = 语素节点类;
