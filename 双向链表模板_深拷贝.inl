// 说明：这是模板分片文件，不单独声明 module，供 双向链表模板核心.ixx 末尾 #include 使用。

template<typename 主信息类型>
typename 链表模板<主信息类型>::节点类*
链表模板<主信息类型>::深拷贝树_独立(const 节点类* 源根)
{
    return 私有_深拷贝树_独立_递归(源根);
}

template<typename 主信息类型>
typename 链表模板<主信息类型>::节点类*
链表模板<主信息类型>::深拷贝树(const 节点类* 源根) const
{
    读锁守卫 lk(链表锁);
    return 深拷贝树_独立(源根);
}

template<typename 主信息类型>
typename 链表模板<主信息类型>::节点类*
链表模板<主信息类型>::深拷贝树_已加锁(const 节点类* 源根) const
{
    return 深拷贝树_独立(源根);
}

template<typename 主信息类型>
void 链表模板<主信息类型>::释放树_独立(节点类*& 独立根)
{
    if (!独立根) return;
    私有_递归释放_独立(独立根);
    独立根 = nullptr;
}

template<typename 主信息类型>
主信息类型 链表模板<主信息类型>::私有_深拷贝主信息(const 主信息类型& src)
{
    if constexpr (std::is_pointer_v<主信息类型>) {
        // 指针主信息按“原样引用”规则处理，不在模板里深拷贝对象本体。
        return src;
    }
    else {
        static_assert(std::is_copy_constructible_v<主信息类型>,
            "链表模板::深拷贝树_独立：主信息为值类型时，需可拷贝构造。"
        );
        return src;
    }
}

template<typename 主信息类型>
typename 链表模板<主信息类型>::节点类*
链表模板<主信息类型>::私有_深拷贝树_独立_递归(const 节点类* src)
{
    if (!src) return nullptr;

    auto* dst = new 节点类{};

    dst->主键 = src->主键;
    dst->子节点数量 = src->子节点数量;

    dst->上 = dst;
    dst->下 = dst;
    dst->父 = nullptr;
    dst->子 = nullptr;
    dst->同层环头 = dst;
    dst->链上 = dst;
    dst->链下 = dst;
    dst->主信息 = 私有_深拷贝主信息(src->主信息);

    if (src->子) {
        const 节点类* src_head = src->子;
        const 节点类* cur = src_head;

        节点类* dst_head = nullptr;
        节点类* dst_tail = nullptr;
        std::int64_t cnt = 0;

        do {
            节点类* child = 私有_深拷贝树_独立_递归(cur);
            child->父 = dst;

            if (!dst_head) {
                dst_head = child;
                dst_tail = child;
                child->上 = child;
                child->下 = child;
            }
            else {
                child->上 = dst_tail;
                child->下 = dst_head;
                dst_tail->下 = child;
                dst_head->上 = child;
                dst_tail = child;
            }

            ++cnt;
            cur = cur->下;
        } while (cur && cur != src_head);

        dst->子 = dst_head;
        dst->子节点数量 = cnt;
        私有_设置同层环头(dst_head);
    }

    return dst;
}

template<typename 主信息类型>
void 链表模板<主信息类型>::私有_递归释放_独立(节点类* n)
{
    if (!n) return;

    if (n->子) {
        节点类* head = n->子;
        节点类* cur = head;
        do {
            节点类* nxt = cur->下;
            cur->上 = nullptr;
            cur->下 = nullptr;
            私有_递归释放_独立(cur);
            cur = nxt;
        } while (cur && cur != head);
        n->子 = nullptr;
    }

    if constexpr (std::is_pointer_v<主信息类型>) {
        // 模板不拥有指针主信息生命周期，独立树释放时也不 delete 主信息。
        n->主信息 = nullptr;
    }

    delete n;
}
