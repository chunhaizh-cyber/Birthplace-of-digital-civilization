#pragma once

#include <string>

#include "基础信息主信息类.h"

class 基础信息类 : public 链表模板<基础信息基类*> {
public:
    using 基类 = 链表模板<基础信息基类*>;
    using 节点类 = 基础信息节点类;

public:
    基础信息类();
    ~基础信息类();

    基础信息类(const 基础信息类&) = delete;
    基础信息类& operator=(const 基础信息类&) = delete;
    基础信息类(基础信息类&&) = delete;
    基础信息类& operator=(基础信息类&&) = delete;

    void 初始化();

    节点类* 世界根() const noexcept;
    节点类* 添加节点(节点类* 位置节点, 基础信息基类* 主信息);
    节点类* 添加子节点(节点类* 父节点, 基础信息基类* 主信息);
    bool 删除节点(节点类* 节点);
    bool 更新主信息(节点类* 节点, 基础信息基类* 主信息);

    std::vector<节点类*> 枚举全部节点() const;
    std::vector<节点类*> 枚举子节点(const 节点类* 父节点) const;
    std::string 获取名称(const 节点类* 节点) const;

    template<class T主信息>
    T主信息* 取主信息(节点类* 节点) const noexcept
    {
        return 节点 ? dynamic_cast<T主信息*>(节点->主信息) : nullptr;
    }

    template<class T主信息>
    const T主信息* 取主信息(const 节点类* 节点) const noexcept
    {
        return 节点 ? dynamic_cast<const T主信息*>(节点->主信息) : nullptr;
    }

    template<class T主信息>
    std::vector<节点类*> 枚举节点_按类型() const
    {
        std::vector<节点类*> out;
        auto lk = 获取读锁();
        if (!根指针) return out;

        for (auto* it = 根指针->链下; it && it != 根指针; it = it->链下) {
            if (dynamic_cast<T主信息*>(it->主信息)) {
                out.push_back(static_cast<节点类*>(it));
            }
        }
        return out;
    }

    template<class T主信息>
    std::vector<节点类*> 枚举子节点_按类型(const 节点类* 父节点) const
    {
        std::vector<节点类*> out;
        auto lk = 获取读锁();
        auto* parent = 父节点 ? const_cast<节点类*>(父节点) : 世界根();
        if (!parent || !parent->子) return out;

        auto* first = static_cast<节点类*>(parent->子);
        auto* it = first;
        do {
            if (dynamic_cast<T主信息*>(it->主信息)) {
                out.push_back(it);
            }
            it = static_cast<节点类*>(it->下);
        } while (it && it != first);

        return out;
    }

private:
    void 私有_清理主信息_已加锁() noexcept;
};
