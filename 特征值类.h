#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <unordered_map>
#include <vector>

#include "特征值主信息类.h"

using 特征值节点类 = 链表模板<特征值主信息类>::节点类;

enum class 枚举_轮廓维度 : std::uint8_t {
    未定义 = 0,
    二维 = 1,
    三维 = 2,
};

class 特征值类 : public 链表模板<特征值主信息类> {
public:
    using 基类 = 链表模板<特征值主信息类>;
    using 节点类 = 基类::节点类;
    using 写锁守卫 = 基类::写锁守卫;

public:
    // 功能：初始化相关对象、状态或运行上下文。
    void 初始化() {
        写锁守卫 lk(this->链表锁);
        私有_重建缓存_已加锁();
    }

    // 功能：按函数名执行对应处理。
    void 调试清空缓存() noexcept {
        缓存已构建_ = false;
        decltype(哈希到候选_){}.swap(哈希到候选_);
        decltype(索引池_){}.swap(索引池_);
        根2D_ = nullptr;
        根3D_ = nullptr;
    }

    // 功能：按条件查找目标对象、方法或事实。
    节点类* 查找(const 特征值主信息类& mi) const {
        return 查找(mi.值);
    }

    节点类* 获取或创建(特征值主信息类&& mi, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) {
        写锁守卫 lk(this->链表锁);
        私有_确保缓存_已加锁();
        return 获取或创建_已加锁(std::move(mi), dim);
    }

    // 功能：按条件查找目标对象、方法或事实。
    节点类* 查找(const VecIU64& v) const {
        写锁守卫 lk(this->链表锁);
        私有_确保缓存_已加锁();
        return 查找_已加锁(v);
    }

    节点类* 获取或创建(VecIU64 v, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) {
        写锁守卫 lk(this->链表锁);
        私有_确保缓存_已加锁();
        return 获取或创建_已加锁(std::move(v), dim);
    }

    节点类* 获取或创建VecI64特征值节点(const VecI64& v, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) {
        return 获取或创建(编码VecI64为VecIU64(v), dim);
    }

    VecU句柄 获取或创建句柄(VecIU64 v, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) {
        auto* n = 获取或创建(std::move(v), dim);
        VecU句柄 h{};
        h.主信息指针 = n ? reinterpret_cast<std::uintptr_t>(&n->主信息) : 0;
        return h;
    }

    VecU句柄 获取或创建VecI64句柄(const VecI64& v, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) {
        return 获取或创建句柄(编码VecI64为VecIU64(v), dim);
    }

    VecU句柄 获取或创建VecU(VecIU64 v, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) {
        return 获取或创建句柄(std::move(v), dim);
    }

    static VecU句柄 生成句柄(const 节点类* n) noexcept {
        VecU句柄 h{};
        h.主信息指针 = n ? reinterpret_cast<std::uintptr_t>(&n->主信息) : 0;
        return h;
    }

    节点类* 取节点(VecU句柄 h) const {
        if (!h.有效()) return nullptr;
        写锁守卫 lk(this->链表锁);
        auto* 目标主信息 = reinterpret_cast<const 特征值主信息类*>(h.主信息指针);
        if (!目标主信息 || !this->根指针) return nullptr;
        for (auto* it = this->根指针->链下; it && it != this->根指针; it = it->链下) {
            if (&it->主信息 == 目标主信息) return it;
        }
        return nullptr;
    }

    节点类* 创建值节点(VecIU64 v, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) {
        写锁守卫 lk(this->链表锁);
        私有_确保缓存_已加锁();
        特征值主信息类 mi(std::move(v));
        mi.内容哈希 = 哈希VecIU64(mi.值);
        return 添加_分层_已加锁(std::move(mi), dim);
    }

    节点类* 创建子值节点(节点类* 父节点, VecIU64 v) {
        if (!父节点) return nullptr;
        写锁守卫 lk(this->链表锁);
        私有_确保缓存_已加锁();

        特征值主信息类 mi(std::move(v));
        mi.内容哈希 = 哈希VecIU64(mi.值);
        auto* node = this->添加子节点_已加锁(父节点, std::move(mi));
        if (node) {
            哈希到候选_[node->主信息.内容哈希].push_back(node);
        }
        return node;
    }

    // 功能：按函数名执行对应处理。
    const VecIU64* 取VecU只读指针(VecU句柄 h) const noexcept {
        if (!h.有效()) return nullptr;
        auto* mi = reinterpret_cast<const 特征值主信息类*>(h.主信息指针);
        return mi ? &mi->值 : nullptr;
    }

    // 功能：按条件查找目标对象、方法或事实。
    节点类* 查找_已加锁(const 特征值主信息类& mi) const {
        私有_确保缓存_已加锁();
        return 查找_已加锁(mi.值);
    }

    // 功能：创建并返回或登记对应对象。
    节点类* 获取或创建_已加锁(特征值主信息类&& mi, 枚举_轮廓维度 dim) {
        私有_确保缓存_已加锁();
        if (mi.内容哈希 == 0) mi.内容哈希 = 哈希VecIU64(mi.值);
        if (auto* hit = 查找_已加锁(mi.值)) return hit;
        return 添加_分层_已加锁(std::move(mi), dim);
    }

    // 功能：按条件查找目标对象、方法或事实。
    节点类* 查找_已加锁(const VecIU64& v) const {
        私有_确保缓存_已加锁();

        const std::uint64_t h = 哈希VecIU64(v);
        auto it = 哈希到候选_.find(h);
        if (it == 哈希到候选_.end()) return nullptr;

        for (auto* n : it->second) {
            if (!n) continue;
            if (n->主信息.值 == v) return n;
        }
        return nullptr;
    }

    // 功能：创建并返回或登记对应对象。
    节点类* 获取或创建_已加锁(VecIU64&& v, 枚举_轮廓维度 dim) {
        私有_确保缓存_已加锁();
        if (auto* hit = 查找_已加锁(v)) return hit;

        特征值主信息类 mi(std::move(v));
        mi.内容哈希 = 哈希VecIU64(mi.值);
        return 添加_分层_已加锁(std::move(mi), dim);
    }

private:
    mutable bool 缓存已构建_ = false;
    // 运行期索引缓存：按内容哈希缩小候选范围，命中后仍以 VecU 值绝对相同为准。
    mutable std::unordered_map<std::uint64_t, std::vector<节点类*>> 哈希到候选_{};

    struct 分层索引节点 {
        std::unordered_map<I64, 分层索引节点*> 子{};
        std::vector<节点类*> 叶子{};
    };

    // 轮廓分层索引只服务近似查找 / 聚类，不改变特征值主信息的权威值语义。
    std::vector<std::unique_ptr<分层索引节点>> 索引池_{};
    分层索引节点* 根2D_ = nullptr;
    分层索引节点* 根3D_ = nullptr;

private:
    // 功能：服务所在模块的内部辅助流程。
    static std::size_t 私有_向上取整平方边长(std::size_t x) noexcept {
        if (x <= 1) return x;

        auto n = static_cast<std::size_t>(std::sqrt(static_cast<long double>(x)));
        while (n * n < x) ++n;
        return n;
    }

    // 功能：服务所在模块的内部辅助流程。
    static std::size_t 私有_向上取整立方边长(std::size_t x) noexcept {
        if (x <= 1) return x;

        auto n = static_cast<std::size_t>(std::cbrt(static_cast<long double>(x)));
        while (n * n * n < x) ++n;
        return n;
    }

    // 功能：确保目标结构、状态或前置条件存在并可用。
    void 私有_确保缓存_已加锁() const {
        if (!缓存已构建_) 私有_重建缓存_已加锁();
    }

    // 功能：服务所在模块的内部辅助流程。
    void 私有_重建缓存_已加锁() const {
        哈希到候选_.clear();

        auto* root = this->根指针;
        if (!root) {
            缓存已构建_ = true;
            return;
        }

        for (auto* it = root->链下; it && it != root; it = it->链下) {
            const auto& mi = it->主信息;
            const auto h = mi.内容哈希 == 0 ? 哈希VecIU64(mi.值) : mi.内容哈希;
            哈希到候选_[h].push_back(it);
        }

        缓存已构建_ = true;
    }

    // 功能：按函数名执行对应处理。
    static std::uint64_t FNV1a64_混入64位值(std::uint64_t h, std::uint64_t x) noexcept {
        for (int i = 0; i < 8; ++i) {
            const auto b = static_cast<std::uint8_t>((x >> (i * 8)) & 0xFF);
            h ^= static_cast<std::uint64_t>(b);
            h *= 1099511628211ull;
        }
        return h;
    }

    // 功能：按函数名执行对应处理。
    static std::uint64_t 哈希VecIU64(const VecIU64& v) noexcept {
        std::uint64_t h = 1469598103934665603ull;
        h = FNV1a64_混入64位值(h, static_cast<std::uint64_t>(v.size()));
        for (auto x : v) h = FNV1a64_混入64位值(h, x);
        return h;
    }

    // 功能：服务所在模块的内部辅助流程。
    static std::uint64_t 私有_哈希层(const VecIU64& layer) noexcept {
        return 哈希VecIU64(layer);
    }

    // 功能：根据当前输入生成目标数据、场景、动态或回执。
    static std::vector<I64> 生成二维轮廓金字塔(VecIU64 base, std::vector<VecIU64>& 输出层) {
        输出层.clear();
        std::vector<I64> tokens{};
        if (base.empty()) return tokens;

        std::size_t side = 私有_向上取整平方边长(base.size());
        if (side % 8 != 0) side = ((side + 7) / 8) * 8;

        const std::size_t target = side * side;
        if (base.size() < target) base.resize(target, 0);
        输出层.push_back(std::move(base));

        std::size_t W = side;
        std::size_t H = side;

        auto idx2 = [](std::size_t x, std::size_t y, std::size_t w) noexcept {
            return y * w + x;
        };

        while (W > 1 || H > 1) {
            const auto& prev = 输出层.back();
            const std::size_t newW = (W + 1) / 2;
            const std::size_t newH = (H + 1) / 2;

            VecIU64 cur(newW * newH, 0);

            auto pick = [&](std::size_t x, std::size_t y) noexcept -> std::uint64_t {
                if (x >= W || y >= H) return 0;
                return prev[idx2(x, y, W)];
            };

            for (std::size_t y = 0; y < newH; ++y) {
                for (std::size_t x = 0; x < newW; ++x) {
                    cur[idx2(x, y, newW)] =
                        pick(x * 2 + 0, y * 2 + 0) |
                        pick(x * 2 + 1, y * 2 + 0) |
                        pick(x * 2 + 0, y * 2 + 1) |
                        pick(x * 2 + 1, y * 2 + 1);
                }
            }

            输出层.push_back(std::move(cur));
            W = newW;
            H = newH;
        }

        tokens.reserve(输出层.size());
        for (auto it = 输出层.rbegin(); it != 输出层.rend(); ++it) {
            tokens.push_back(static_cast<I64>(私有_哈希层(*it)));
        }
        return tokens;
    }

    // 功能：根据当前输入生成目标数据、场景、动态或回执。
    static std::vector<I64> 生成三维轮廓金字塔(VecIU64 base, std::vector<VecIU64>& 输出层) {
        输出层.clear();
        std::vector<I64> tokens{};
        if (base.empty()) return tokens;

        std::size_t side = 私有_向上取整立方边长(base.size());
        if (side % 4 != 0) side = ((side + 3) / 4) * 4;

        const std::size_t target = side * side * side;
        if (base.size() < target) base.resize(target, 0);
        输出层.push_back(std::move(base));

        std::size_t W = side;
        std::size_t H = side;
        std::size_t D = side;

        auto idx3 = [](std::size_t x, std::size_t y, std::size_t z, std::size_t w, std::size_t h) noexcept {
            return (z * h + y) * w + x;
        };

        while (W > 1 || H > 1 || D > 1) {
            const auto& prev = 输出层.back();
            const std::size_t newW = (W + 1) / 2;
            const std::size_t newH = (H + 1) / 2;
            const std::size_t newD = (D + 1) / 2;

            VecIU64 cur(newW * newH * newD, 0);

            auto pick = [&](std::size_t x, std::size_t y, std::size_t z) noexcept -> std::uint64_t {
                if (x >= W || y >= H || z >= D) return 0;
                return prev[idx3(x, y, z, W, H)];
            };

            for (std::size_t z = 0; z < newD; ++z) {
                for (std::size_t y = 0; y < newH; ++y) {
                    for (std::size_t x = 0; x < newW; ++x) {
                        cur[idx3(x, y, z, newW, newH)] =
                            pick(x * 2 + 0, y * 2 + 0, z * 2 + 0) |
                            pick(x * 2 + 1, y * 2 + 0, z * 2 + 0) |
                            pick(x * 2 + 0, y * 2 + 1, z * 2 + 0) |
                            pick(x * 2 + 1, y * 2 + 1, z * 2 + 0) |
                            pick(x * 2 + 0, y * 2 + 0, z * 2 + 1) |
                            pick(x * 2 + 1, y * 2 + 0, z * 2 + 1) |
                            pick(x * 2 + 0, y * 2 + 1, z * 2 + 1) |
                            pick(x * 2 + 1, y * 2 + 1, z * 2 + 1);
                    }
                }
            }

            输出层.push_back(std::move(cur));
            W = newW;
            H = newH;
            D = newD;
        }

        tokens.reserve(输出层.size());
        for (auto it = 输出层.rbegin(); it != 输出层.rend(); ++it) {
            tokens.push_back(static_cast<I64>(私有_哈希层(*it)));
        }
        return tokens;
    }

    // 功能：创建并返回或登记对应对象。
    分层索引节点* 分层_取或创建路径_已加锁(分层索引节点*& root, const std::vector<I64>& tokens) {
        if (!root) {
            索引池_.emplace_back(std::make_unique<分层索引节点>());
            root = 索引池_.back().get();
        }

        auto* cur = root;
        for (auto token : tokens) {
            auto it = cur->子.find(token);
            if (it == cur->子.end()) {
                索引池_.emplace_back(std::make_unique<分层索引节点>());
                auto* created = 索引池_.back().get();
                cur->子.emplace(token, created);
                cur = created;
            }
            else {
                cur = it->second;
            }
        }

        return cur;
    }

    // 功能：登记方法、模板、对象或运行入口。
    void 分层_登记叶子_已加锁(枚举_轮廓维度 dim, const std::vector<I64>& tokens, 节点类* leaf) {
        if (!leaf) return;

        auto*& root = (dim == 枚举_轮廓维度::二维) ? 根2D_ : 根3D_;
        auto* path = 分层_取或创建路径_已加锁(root, tokens);
        path->叶子.push_back(leaf);
    }

    // 功能：按函数名执行对应处理。
    节点类* 添加_分层_已加锁(特征值主信息类&& mi, 枚举_轮廓维度 dim) {
        if (mi.内容哈希 == 0) mi.内容哈希 = 哈希VecIU64(mi.值);

        auto* node = this->添加节点_已加锁(nullptr, std::move(mi));
        if (!node) return nullptr;

        哈希到候选_[node->主信息.内容哈希].push_back(node);

        if (dim != 枚举_轮廓维度::未定义) {
            std::vector<VecIU64> 层{};
            std::vector<I64> tokens;

            if (dim == 枚举_轮廓维度::二维) {
                tokens = 生成二维轮廓金字塔(node->主信息.值, 层);
            }
            else {
                tokens = 生成三维轮廓金字塔(node->主信息.值, 层);
            }

            分层_登记叶子_已加锁(dim, tokens, node);
        }

        return node;
    }
};
