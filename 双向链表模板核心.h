#pragma once

#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <type_traits>
#include <fstream>
#include <unordered_map>
#include <cstdint>
#include <limits>

#include "基础数据类型.h"

// 双向链表模板（环形）
// - 同层：用 上/下 构成环
// - 全链：用 链上/链下 把整棵树的所有节点串成一个环，便于全局扫描
// - 线程安全：默认对外接口会加锁（链表锁）
// - 注意：本模板不拥有“指针型主信息”的生命周期，不会自动 delete 主信息
//
// 拆分说明：
// - 本文件只放：节点定义、增删查改、锁、结构校验、以及扩展能力的声明。
// - 深拷贝与序列化的定义分别放在：
//     双向链表模板_深拷贝.ixx
//     双向链表模板_序列化.ixx
// - 由于这是模板，定义必须对实例化点可见；因此这两个文件在本文件末尾以 #include 方式并入。

template<typename 主信息类型>
class 链表模板 {
public:
    struct 节点类 {
        节点类* 上 = nullptr;
        节点类* 下 = nullptr;

        节点类* 父 = nullptr;
        节点类* 子 = nullptr;

        // 当前同层环的头结点，不是整棵树根
        节点类* 同层环头 = nullptr;

        // 全链环（把整棵树所有节点串起来）
        节点类* 链上 = nullptr;
        节点类* 链下 = nullptr;

        主信息类型 主信息{};
        std::int64_t 子节点数量 = 0;

        void 变更子节点数量(std::int64_t delta) { 子节点数量 += delta; }
        std::string 获取主键() const { return 主键; }
        void 设置主键(const std::string& 主键_) { 主键 = 主键_; }

        bool 是否叶子() const { return 子 == nullptr; }
        bool 是否同层孤立() const { return 上 == this && 下 == this; }

        void 初始化同层自环()
        {
            上 = this;
            下 = this;
            同层环头 = this;
        }

        void 初始化全链自环()
        {
            链上 = this;
            链下 = this;
        }

        void 初始化为孤立节点()
        {
            父 = nullptr;
            子 = nullptr;
            子节点数量 = 0;
            初始化同层自环();
            初始化全链自环();
        }

        void 从同层环摘除()
        {
            if (!上 || !下) return;

            上->下 = 下;
            下->上 = 上;

            上 = this;
            下 = this;
            同层环头 = this;
        }

        void 插入到同层节点后(节点类* 位置)
        {
            if (!位置) return;

            上 = 位置;
            下 = 位置->下;

            位置->下->上 = this;
            位置->下 = this;

            同层环头 = 位置->同层环头 ? 位置->同层环头 : 位置;
        }

        void 从全链环摘除()
        {
            if (!链上 || !链下) return;

            链上->链下 = 链下;
            链下->链上 = 链上;

            链上 = this;
            链下 = this;
        }

        void 插入到全链尾(节点类* 全局根)
        {
            if (!全局根) return;

            链上 = 全局根->链上;
            链下 = 全局根;

            全局根->链上->链下 = this;
            全局根->链上 = this;
        }

    private:
        std::string 主键{};
        friend class 链表模板;
    };

public:
    using 写锁守卫 = std::unique_lock<std::shared_mutex>;
    using 读锁守卫 = std::shared_lock<std::shared_mutex>;
    using 锁守卫 = 写锁守卫;

    mutable std::shared_mutex 链表锁;
    节点类* 根指针 = nullptr;

public:
    链表模板()
    {
        根指针 = new 节点类{};
        根指针->初始化为孤立节点();
        根指针->父 = nullptr;
        根指针->子 = nullptr;
        根指针->同层环头 = 根指针;
    }

    ~链表模板()
    {
        删除链表();
    }

    锁守卫 获取锁() const { return 锁守卫(链表锁); }
    读锁守卫 获取读锁() const { return 读锁守卫(链表锁); }

private:
    std::string 获取新主键(const std::string& 前一主键) const
    {
        constexpr char 字符集[] =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        constexpr std::size_t 字符集大小 = sizeof(字符集) - 1;

        std::string 新主键 = 前一主键;

        for (int i = static_cast<int>(新主键.size()) - 1; i >= 0; --i) {
            std::size_t pos = 0;
            while (pos < 字符集大小 && 字符集[pos] != 新主键[i]) ++pos;

            if (pos + 1 < 字符集大小) {
                新主键[i] = 字符集[pos + 1];
                return 新主键;
            }
            新主键[i] = 字符集[0];
        }

        新主键.insert(新主键.begin(), 字符集[1]);
        return 新主键;
    }

    void 初始化新节点_已加锁(节点类* n)
    {
        if (!n) return;
        n->初始化为孤立节点();
    }

    static void 私有_设置同层环头(节点类* 头)
    {
        if (!头) return;
        节点类* cur = 头;
        do {
            cur->同层环头 = 头;
            cur = cur->下;
        } while (cur && cur != 头);
    }

    template<class T>
    static constexpr bool 私有_可比较值类型 = requires(const T& a, const T& b, 枚举_比较字段 f) {
        a.比较(b, f);
    };

    template<class T>
    static constexpr bool 私有_可比较指针类型 = std::is_pointer_v<T> && requires(T a, T b, 枚举_比较字段 f) {
        a->比较(b, f);
    };

public:
    std::string 获取主键(节点类* 节点) const
    {
        if (!节点) return "";
        return 节点->获取主键();
    }

    // =============================================================
    // 1) 添加节点：同层环插入 + 全链环登记
    // =============================================================
    节点类* 添加节点_已加锁(节点类* 位置节点, 节点类* 新节点)
    {
        if (!新节点) return nullptr;
        if (!位置节点) 位置节点 = 根指针;

        初始化新节点_已加锁(新节点);

        新节点->父 = 位置节点->父;
        新节点->插入到同层节点后(位置节点);

        根指针->主键 = 获取新主键(根指针->主键);
        新节点->主键 = 根指针->主键;

        if (新节点->父) {
            新节点->父->变更子节点数量(1);
        }

        新节点->插入到全链尾(根指针);
        return 新节点;
    }

    节点类* 添加节点(节点类* 位置节点, 节点类* 新节点)
    {
        写锁守卫 lock(链表锁);
        return 添加节点_已加锁(位置节点, 新节点);
    }

    节点类* 添加节点_已加锁(节点类* 位置节点, 主信息类型 主信息)
    {
        auto* 新节点 = new 节点类{};
        新节点->主信息 = 主信息;
        return 添加节点_已加锁(位置节点, 新节点);
    }

    节点类* 添加节点(节点类* 位置节点, 主信息类型 主信息)
    {
        写锁守卫 lock(链表锁);
        return 添加节点_已加锁(位置节点, 主信息);
    }

    // =============================================================
    // 2) 添加子节点：挂到 父节点 的 子链（同层环）+ 全链环登记
    // =============================================================
    节点类* 添加子节点_已加锁(节点类* 父节点, 节点类* 子节点)
    {
        if (!父节点 || !子节点) return nullptr;

        初始化新节点_已加锁(子节点);
        子节点->父 = 父节点;

        根指针->主键 = 获取新主键(根指针->主键);
        子节点->主键 = 根指针->主键;

        if (!父节点->子) {
            父节点->子 = 子节点;
            子节点->同层环头 = 子节点;
        }
        else {
            子节点->插入到同层节点后(父节点->子);
            子节点->同层环头 = 父节点->子;
        }

        父节点->变更子节点数量(1);
        子节点->插入到全链尾(根指针);
        return 子节点;
    }

    节点类* 添加子节点(节点类* 父节点, 节点类* 子节点)
    {
        写锁守卫 lock(链表锁);
        return 添加子节点_已加锁(父节点, 子节点);
    }

    节点类* 添加子节点_已加锁(节点类* 父节点, 主信息类型 主信息)
    {
        auto* 子节点 = new 节点类{};
        子节点->主信息 = 主信息;
        return 添加子节点_已加锁(父节点, 子节点);
    }

    节点类* 添加子节点(节点类* 父节点, 主信息类型 主信息)
    {
        写锁守卫 lock(链表锁);
        return 添加子节点_已加锁(父节点, 主信息);
    }

    // =============================================================
    // 3) 查找（全链扫描 / 当前链扫描）
    // =============================================================
    节点类* 查找主键_已加锁(const std::string& 主键) const
    {
        if (!根指针) return nullptr;

        for (节点类* it = 根指针->链下; it && it != 根指针; it = it->链下) {
            if (it->主键 == 主键) return it;
        }
        return nullptr;
    }

    节点类* 查找主键(const std::string& 主键) const
    {
        读锁守卫 lk(链表锁);
        return 查找主键_已加锁(主键);
    }

    节点类* 查找主键_当前链_已加锁(节点类* 当前节点, const std::string& 主键) const
    {
        if (!当前节点) return nullptr;

        节点类* root = 当前节点->同层环头 ? 当前节点->同层环头 : 当前节点;
        if (!root) return nullptr;

        if (root == 根指针) {
            节点类* it = 根指针->下;
            while (it && it != 根指针) {
                if (it->主键 == 主键) return it;
                it = it->下;
            }
            return nullptr;
        }

        节点类* it = root;
        do {
            if (it->主键 == 主键) return it;
            it = it->下;
        } while (it && it != root);

        return nullptr;
    }

    节点类* 查找主键_当前链(节点类* 当前节点, const std::string& 主键) const
    {
        读锁守卫 lk(链表锁);
        return 查找主键_当前链_已加锁(当前节点, 主键);
    }

    节点类* 查找节点_当前链_已加锁(
        节点类* 当前节点,
        主信息类型 查找内容,
        枚举_比较字段 比较字段) const
    {
        if (!当前节点) return nullptr;

        if constexpr (std::is_pointer_v<主信息类型>) {
            if (!查找内容) return nullptr;
        }

        节点类* root = 当前节点->同层环头 ? 当前节点->同层环头 : 当前节点;
        if (!root) return nullptr;

        auto 命中 = [&](节点类* it) -> bool {
            if (!it) return false;

            if constexpr (私有_可比较指针类型<主信息类型>) {
                auto* mi = it->主信息;
                return mi && mi->比较(查找内容, 比较字段) == 1;
            }
            else if constexpr (私有_可比较值类型<主信息类型>) {
                return it->主信息.比较(查找内容, 比较字段) == 1;
            }
            else {
                static_assert(私有_可比较指针类型<主信息类型> || 私有_可比较值类型<主信息类型>,
                    "链表模板::查找节点_当前链：主信息类型必须提供 比较(...) 接口。"
                );
                return false;
            }
        };

        if (root == 根指针) {
            for (节点类* it = 根指针->下; it && it != 根指针; it = it->下) {
                if (命中(it)) return it;
            }
            return nullptr;
        }

        节点类* it = root;
        do {
            if (命中(it)) return it;
            it = it->下;
        } while (it && it != root);

        return nullptr;
    }

    节点类* 查找节点_当前链(节点类* 当前节点, 主信息类型 查找内容, 枚举_比较字段 比较字段) const
    {
        读锁守卫 lk(链表锁);
        return 查找节点_当前链_已加锁(当前节点, 查找内容, 比较字段);
    }

    // =============================================================
    // 4) 修改节点信息
    // =============================================================
    void 修改节点信息_已加锁(节点类* 节点, 主信息类型 新信息)
    {
        if (!节点 || 节点 == 根指针) return;

        // 新规则：链表模板不拥有指针型主信息的生命周期，这里不 delete 旧主信息
        节点->主信息 = 新信息;
    }

    void 修改节点信息(节点类* 节点, 主信息类型 新信息)
    {
        写锁守卫 lock(链表锁);
        修改节点信息_已加锁(节点, 新信息);
    }

    void 修改节点信息(const std::string& 主键, 主信息类型 新信息)
    {
        写锁守卫 lock(链表锁);
        节点类* 节点 = 查找主键_已加锁(主键);
        修改节点信息_已加锁(节点, 新信息);
    }

    // =============================================================
    // 5) 删除（支持递归删子链）
    // =============================================================
    void 删除节点_已加锁(节点类* 被删除节点)
    {
        if (!被删除节点 || 被删除节点 == 根指针) return;

        节点类* 父节点 = 被删除节点->父;
        const bool 是子链环头 = (父节点 != nullptr && 被删除节点->同层环头 == 被删除节点);
        节点类* 新同层环头 = nullptr;

        if (被删除节点->子) {
            删除子链_已加锁(被删除节点);
            被删除节点->子 = nullptr;
        }

        if (是子链环头 && 被删除节点->下 != 被删除节点) {
            新同层环头 = 被删除节点->下;
        }

        if (父节点 && 父节点->子 == 被删除节点) {
            父节点->子 = 新同层环头;
        }

        被删除节点->从同层环摘除();
        被删除节点->从全链环摘除();

        if (父节点) {
            父节点->变更子节点数量(-1);
            if (新同层环头) {
                私有_设置同层环头(新同层环头);
            }
        }

        delete 被删除节点;
    }

    void 删除节点(节点类* 被删除节点)
    {
        写锁守卫 lock(链表锁);
        删除节点_已加锁(被删除节点);
    }

    void 删除子链_已加锁(节点类* 父节点)
    {
        if (!父节点 || !父节点->子) return;

        while (父节点->子) {
            节点类* 当前 = 父节点->子;

            if (当前->下 == 当前) {
                删除节点_已加锁(当前);
                父节点->子 = nullptr;
                break;
            }
            else {
                父节点->子 = 当前->下;
                删除节点_已加锁(当前);
            }
        }
    }

    void 删除子链(节点类* 父节点)
    {
        写锁守卫 lock(链表锁);
        删除子链_已加锁(父节点);
    }

    void 删除链表_已加锁()
    {
        if (!根指针) return;

        while (根指针->链下 != 根指针) {
            删除节点_已加锁(根指针->链下);
        }

        delete 根指针;
        根指针 = nullptr;
    }

    void 删除链表()
    {
        写锁守卫 lock(链表锁);
        删除链表_已加锁();
    }

    // =============================================================
    // 6) 结构校验
    // =============================================================
    bool 校验结构() const
    {
        读锁守卫 lk(链表锁);
        return 校验结构_已加锁();
    }

    bool 校验结构_已加锁() const
    {
        if (!根指针) return false;
        if (!根指针->链上 || !根指针->链下) return false;
        if (根指针->链上->链下 != 根指针) return false;
        if (根指针->链下->链上 != 根指针) return false;
        if (!根指针->上 || !根指针->下) return false;
        if (根指针->上->下 != 根指针) return false;
        if (根指针->下->上 != 根指针) return false;
        if (根指针->同层环头 != 根指针) return false;

        for (节点类* n = 根指针->链下; n && n != 根指针; n = n->链下) {
            if (!n->链上 || !n->链下) return false;
            if (n->链上->链下 != n) return false;
            if (n->链下->链上 != n) return false;

            if (!n->上 || !n->下) return false;
            if (n->上->下 != n) return false;
            if (n->下->上 != n) return false;

            节点类* head = n->同层环头 ? n->同层环头 : n;
            节点类* cur = head;
            std::int64_t 同层计数 = 0;
            bool 找到自己 = false;

            do {
                if (!cur) return false;
                if (cur->同层环头 != head) return false;
                if (cur == n) 找到自己 = true;
                ++同层计数;
                cur = cur->下;
                if (同层计数 > 10000000) return false;
            } while (cur && cur != head);

            if (!找到自己) return false;

            if (n->子) {
                std::int64_t 实际子数 = 0;
                节点类* c = n->子;
                do {
                    if (!c) return false;
                    if (c->父 != n) return false;
                    ++实际子数;
                    c = c->下;
                    if (实际子数 > 10000000) return false;
                } while (c && c != n->子);

                if (实际子数 != n->子节点数量) return false;
            }
            else {
                if (n->子节点数量 != 0) return false;
            }
        }

        return true;
    }

public:
    // =============================================================
    // 7) 深拷贝（声明，定义在 双向链表模板_深拷贝.ixx）
    // =============================================================
    static 节点类* 深拷贝树_独立(const 节点类* 源根);
    节点类* 深拷贝树(const 节点类* 源根) const;
    节点类* 深拷贝树_已加锁(const 节点类* 源根) const;
    static void 释放树_独立(节点类*& 独立根);

private:
    static 主信息类型 私有_深拷贝主信息(const 主信息类型& src);
    static 节点类* 私有_深拷贝树_独立_递归(const 节点类* src);
    static void 私有_递归释放_独立(节点类* n);

public:
    // =============================================================
    // 8) 序列化（声明，定义在 双向链表模板_序列化.ixx）
    // =============================================================
    bool 保存数据_已加锁(const std::string& 文件名) const;
    bool 保存数据(const std::string& 文件名) const;
    bool 保存数据(std::string 文件名) const { return 保存数据((const std::string&)文件名); }

    bool 读取数据_已加锁(const std::string& 文件名);
    bool 读取数据(const std::string& 文件名);
    bool 读取数据(std::string 文件名) { return 读取数据((const std::string&)文件名); }

private:
    static constexpr std::uint32_t _存档魔数 = 0x4C4C4442u; // 'BDLL' little-endian

    template<class T>
    static inline void _写POD(std::ostream& os, const T& v);

    template<class T>
    static inline bool _读POD(std::istream& is, T& v);

    static inline void _写字符串(std::ostream& os, const std::string& s);
    static inline bool _读字符串(std::istream& is, std::string& out, std::uint32_t 上限 = 16u * 1024u * 1024u);
    static inline std::uint64_t _索引或空(const std::unordered_map<const 节点类*, std::uint64_t>& mp, const 节点类* p);
    static inline 节点类* _由索引取指针(const std::vector<节点类*>& vec, std::uint64_t idx);
};

#include "双向链表模板_深拷贝.inl"
#include "双向链表模板_序列化.inl"
