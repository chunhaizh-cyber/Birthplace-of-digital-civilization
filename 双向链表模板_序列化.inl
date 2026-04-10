// 说明：这是模板分片文件，不单独声明 module，供 双向链表模板核心.ixx 末尾 #include 使用。
//
// 注意：
// - 对“指针型主信息”，本分片仍支持通过 Pointee::序列化 / Pointee::反序列化 读写。
// - 但模板核心仍不拥有指针主信息的生命周期，因此这类对象的长期释放策略必须由上层统一管理。
// - 如果你的主信息指针来自外部共享仓库，这里更稳的做法是序列化“主键/索引”，而不是直接序列化对象本体。



template<typename 主信息类型>
template<class T>
inline void 链表模板<主信息类型>::_写POD(std::ostream& os, const T& v)
{
    static_assert(std::is_trivially_copyable_v<T>);
    os.write(reinterpret_cast<const char*>(&v), sizeof(T));
}

template<typename 主信息类型>
template<class T>
inline bool 链表模板<主信息类型>::_读POD(std::istream& is, T& v)
{
    static_assert(std::is_trivially_copyable_v<T>);
    is.read(reinterpret_cast<char*>(&v), sizeof(T));
    return static_cast<bool>(is);
}

template<typename 主信息类型>
inline void 链表模板<主信息类型>::_写字符串(std::ostream& os, const std::string& s)
{
    const std::uint32_t n = static_cast<std::uint32_t>(s.size());
    _写POD(os, n);
    if (n) os.write(s.data(), n);
}

template<typename 主信息类型>
inline bool 链表模板<主信息类型>::_读字符串(std::istream& is, std::string& out, std::uint32_t 上限)
{
    std::uint32_t n = 0;
    if (!_读POD(is, n)) return false;
    if (n > 上限) return false;
    out.assign(n, '\0');
    if (n) is.read(out.data(), n);
    return static_cast<bool>(is);
}

template<typename 主信息类型>
inline std::uint64_t 链表模板<主信息类型>::_索引或空(
    const std::unordered_map<const 节点类*, std::uint64_t>& mp,
    const 节点类* p)
{
    const std::uint64_t NIL = (std::numeric_limits<std::uint64_t>::max)();
    if (!p) return NIL;
    auto it = mp.find(p);
    return (it == mp.end()) ? NIL : it->second;
}

template<typename 主信息类型>
inline typename 链表模板<主信息类型>::节点类*
链表模板<主信息类型>::_由索引取指针(const std::vector<节点类*>& vec, std::uint64_t idx)
{
    const std::uint64_t NIL = (std::numeric_limits<std::uint64_t>::max)();
    if (idx == NIL) return nullptr;
    if (idx >= static_cast<std::uint64_t>(vec.size())) return nullptr;
    return vec[static_cast<std::size_t>(idx)];
}

template<typename 主信息类型>
bool 链表模板<主信息类型>::保存数据_已加锁(const std::string& 文件名) const
{
    std::ofstream ofs(文件名, std::ios::binary);
    if (!ofs) return false;

    const std::uint16_t ver = 1;
    _写POD(ofs, _存档魔数);
    _写POD(ofs, ver);

    std::vector<节点类*> nodes;
    nodes.reserve(1024);
    nodes.push_back(根指针);
    for (节点类* it = 根指针 ? 根指针->链下 : nullptr; it && it != 根指针; it = it->链下) {
        nodes.push_back(it);
    }

    const std::uint64_t count = static_cast<std::uint64_t>(nodes.size());
    _写POD(ofs, count);

    std::unordered_map<const 节点类*, std::uint64_t> idx;
    idx.reserve(static_cast<std::size_t>(count) * 2);
    for (std::uint64_t i = 0; i < count; ++i) idx[nodes[static_cast<std::size_t>(i)]] = i;

    for (auto* n : nodes) {
        _写字符串(ofs, n ? n->主键 : std::string{});
        const std::int64_t childCnt = n ? n->子节点数量 : 0;
        _写POD(ofs, childCnt);

        const std::uint64_t i_up = _索引或空(idx, n ? n->上 : nullptr);
        const std::uint64_t i_dn = _索引或空(idx, n ? n->下 : nullptr);
        const std::uint64_t i_pa = _索引或空(idx, n ? n->父 : nullptr);
        const std::uint64_t i_ch = _索引或空(idx, n ? n->子 : nullptr);
        const std::uint64_t i_ro = _索引或空(idx, n ? n->同层环头 : nullptr);
        const std::uint64_t i_cu = _索引或空(idx, n ? n->链上 : nullptr);
        const std::uint64_t i_cd = _索引或空(idx, n ? n->链下 : nullptr);

        _写POD(ofs, i_up); _写POD(ofs, i_dn); _写POD(ofs, i_pa); _写POD(ofs, i_ch);
        _写POD(ofs, i_ro); _写POD(ofs, i_cu); _写POD(ofs, i_cd);

        if constexpr (std::is_pointer_v<主信息类型>) {
            const std::uint8_t has = (n && n->主信息) ? 1u : 0u;
            _写POD(ofs, has);
            if (has) {
                n->主信息->序列化(ofs);
            }
        }
        else if constexpr (requires(const 主信息类型 & x, std::ostream & os) { x.序列化(os); }) {
            if (n) n->主信息.序列化(ofs);
            else {
                主信息类型 tmp{};
                tmp.序列化(ofs);
            }
        }
        else if constexpr (std::is_trivially_copyable_v<主信息类型>) {
            if (n) _写POD(ofs, n->主信息);
            else {
                主信息类型 tmp{};
                _写POD(ofs, tmp);
            }
        }
        else {
            static_assert(std::is_trivially_copyable_v<主信息类型>,
                "链表模板::保存数据：主信息类型 需要可序列化（提供 序列化/反序列化）或可平凡拷贝。"
            );
        }
    }

    return static_cast<bool>(ofs);
}

template<typename 主信息类型>
bool 链表模板<主信息类型>::保存数据(const std::string& 文件名) const
{
    读锁守卫 lk(链表锁);
    return 保存数据_已加锁(文件名);
}

template<typename 主信息类型>
bool 链表模板<主信息类型>::读取数据_已加锁(const std::string& 文件名)
{
    std::ifstream ifs(文件名, std::ios::binary);
    if (!ifs) return false;

    std::uint32_t magic = 0;
    std::uint16_t ver = 0;
    if (!_读POD(ifs, magic)) return false;
    if (!_读POD(ifs, ver)) return false;
    if (magic != _存档魔数) return false;
    if (ver != 1) return false;

    std::uint64_t count = 0;
    if (!_读POD(ifs, count)) return false;
    if (count == 0 || count > 10000000ull) return false;

    struct 索引包 { std::uint64_t up{}, dn{}, pa{}, ch{}, ro{}, cu{}, cd{}; };
    std::vector<节点类*> nodes(count, nullptr);
    std::vector<索引包> links(count);

    for (std::uint64_t i = 0; i < count; ++i) {
        auto* n = new 节点类{};
        nodes[static_cast<std::size_t>(i)] = n;

        if (!_读字符串(ifs, n->主键)) goto FAIL;
        if (!_读POD(ifs, n->子节点数量)) goto FAIL;

        if (!_读POD(ifs, links[static_cast<std::size_t>(i)].up)) goto FAIL;
        if (!_读POD(ifs, links[static_cast<std::size_t>(i)].dn)) goto FAIL;
        if (!_读POD(ifs, links[static_cast<std::size_t>(i)].pa)) goto FAIL;
        if (!_读POD(ifs, links[static_cast<std::size_t>(i)].ch)) goto FAIL;
        if (!_读POD(ifs, links[static_cast<std::size_t>(i)].ro)) goto FAIL;
        if (!_读POD(ifs, links[static_cast<std::size_t>(i)].cu)) goto FAIL;
        if (!_读POD(ifs, links[static_cast<std::size_t>(i)].cd)) goto FAIL;

        if constexpr (std::is_pointer_v<主信息类型>) {
            std::uint8_t has = 0;
            if (!_读POD(ifs, has)) goto FAIL;
            if (has) {
                using Pointee = std::remove_pointer_t<主信息类型>;
                n->主信息 = Pointee::反序列化(ifs);
                if (!n->主信息) goto FAIL;
            }
            else {
                n->主信息 = nullptr;
            }
        }
        else if constexpr (requires(std::istream & is) { 主信息类型::反序列化(is); }) {
            n->主信息 = 主信息类型::反序列化(ifs);
        }
        else if constexpr (std::is_trivially_copyable_v<主信息类型>) {
            if (!_读POD(ifs, n->主信息)) goto FAIL;
        }
        else {
            static_assert(std::is_trivially_copyable_v<主信息类型>,
                "链表模板::读取数据：主信息类型 需要可反序列化（提供 static 反序列化）或可平凡拷贝。"
            );
        }
    }

    for (std::uint64_t i = 0; i < count; ++i) {
        auto* n = nodes[static_cast<std::size_t>(i)];
        const auto& L = links[static_cast<std::size_t>(i)];
        n->上 = _由索引取指针(nodes, L.up);
        n->下 = _由索引取指针(nodes, L.dn);
        n->父 = _由索引取指针(nodes, L.pa);
        n->子 = _由索引取指针(nodes, L.ch);
        n->同层环头 = _由索引取指针(nodes, L.ro);
        n->链上 = _由索引取指针(nodes, L.cu);
        n->链下 = _由索引取指针(nodes, L.cd);
    }

    删除链表_已加锁();
    根指针 = nodes[0];
    return true;

FAIL:
    for (auto* n : nodes) {
        if (!n) continue;
        if constexpr (std::is_pointer_v<主信息类型>) {
            delete n->主信息;
            n->主信息 = nullptr;
        }
        delete n;
    }
    return false;
}

template<typename 主信息类型>
bool 链表模板<主信息类型>::读取数据(const std::string& 文件名)
{
    写锁守卫 lk(链表锁);
    return 读取数据_已加锁(文件名);
}
