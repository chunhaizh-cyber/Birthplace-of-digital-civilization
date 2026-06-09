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
    存在节点类* 取或创建存在概念根();

    // 语素/基础信息整合入口：
    // - 一个语素入口节点默认对应一种基础信息类型；
    // - 查找或创建该语素入口对应的基础信息模板节点；
    // - 基础信息模板仍放在基础信息树中，语素层只保存引用。
    节点类* 查找语素入口模板(const 语素入口节点类* 入口节点, 枚举_主信息类型 类型) const;
    节点类* 确保语素入口模板(const 语素入口节点类* 入口节点, 枚举_主信息类型 类型, 节点类* 父节点 = nullptr);
    节点类* 选择信息入口默认根(
        枚举_信息入口类型 信息入口类型,
        节点类* 上下文节点 = nullptr
    );
    std::vector<节点类*> 查找引用语素入口节点的基础信息(
        const 语素入口节点类* 入口节点,
        const 节点类* 范围根 = nullptr
    ) const;

    static bool 主信息类型是否可由语素入口模板创建(枚举_主信息类型 类型) noexcept;
    static bool 基础信息节点匹配类型(const 节点类* 节点, 枚举_主信息类型 类型) noexcept;
    static bool 基础信息节点引用语素入口节点(const 节点类* 节点, const 语素入口节点类* 入口节点) noexcept;

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
        if (!parent || !私有_节点属于当前树_已加锁(parent) || !parent->子) return out;

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
    节点类* 存在概念根_ = nullptr;

    bool 私有_节点属于当前树_已加锁(const 节点类* 候选节点) const noexcept
    {
        if (!根指针 || !候选节点) {
            return false;
        }
        if (根指针 == 候选节点) {
            return true;
        }

        for (auto* 当前节点 = 根指针->链下;
             当前节点 && 当前节点 != 根指针;
             当前节点 = 当前节点->链下) {
            if (当前节点 == 候选节点) {
                return true;
            }
        }
        return false;
    }

    static 基础信息基类* 私有_创建语素入口模板主信息(const 语素入口节点类* 入口节点, 枚举_主信息类型 类型);
    static bool 私有_主信息引用语素入口节点(const 基础信息基类* 主信息, const 语素入口节点类* 入口节点) noexcept;
    static bool 私有_节点在范围内(const 节点类* 范围根, const 节点类* 候选节点) noexcept;

    void 私有_清理主信息_已加锁() noexcept;
};

基础信息类& 获取基础信息集();
