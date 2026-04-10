#pragma once

#include <istream>
#include <ostream>

#include "双向链表模板核心.h"

class 特征值主信息类 {
public:
    VecIU64 值{};
    std::uint64_t 内容哈希 = 0;
    std::uint64_t 命中次数 = 0;
    时间戳 最后命中 = 0;

    特征值主信息类() = default;
    explicit 特征值主信息类(VecIU64 v) : 值(std::move(v)) {}

    bool 绝对相同(const 特征值主信息类& rhs) const noexcept {
        return 值 == rhs.值;
    }

    std::int64_t 比较(const 特征值主信息类* 对象, 枚举_比较字段 字段) const {
        return 对象 ? 比较(*对象, 字段) : -1;
    }

    std::int64_t 比较(const 特征值主信息类& 对象, 枚举_比较字段 字段) const {
        switch (字段) {
        case 枚举_比较字段::特征值_值:
            return 私有_比较向量(值, 对象.值);
        case 枚举_比较字段::特征值_类型:
            return 私有_比较整数(内容哈希, 对象.内容哈希);
        case 枚举_比较字段::特征值_命中次数:
            return 私有_比较整数(命中次数, 对象.命中次数);
        default:
            return -1;
        }
    }

private:
    static std::int64_t 私有_比较向量(const VecIU64& a, const VecIU64& b) noexcept {
        if (a == b) return 0;

        const auto count = std::min(a.size(), b.size());
        for (std::size_t i = 0; i < count; ++i) {
            if (a[i] == b[i]) continue;
            return a[i] < b[i] ? -1 : 1;
        }

        if (a.size() == b.size()) return 0;
        return a.size() < b.size() ? -1 : 1;
    }

    static std::int64_t 私有_比较整数(std::uint64_t a, std::uint64_t b) noexcept {
        if (a == b) return 0;
        return a < b ? -1 : 1;
    }

public:
    void 序列化(std::ostream& os) const {
        const std::uint64_t count = static_cast<std::uint64_t>(值.size());
        os.write(reinterpret_cast<const char*>(&count), sizeof(count));
        if (count != 0) {
            os.write(
                reinterpret_cast<const char*>(值.data()),
                static_cast<std::streamsize>(count * sizeof(std::uint64_t))
            );
        }

        os.write(reinterpret_cast<const char*>(&内容哈希), sizeof(内容哈希));
        os.write(reinterpret_cast<const char*>(&命中次数), sizeof(命中次数));
        os.write(reinterpret_cast<const char*>(&最后命中), sizeof(最后命中));
    }

    static 特征值主信息类 反序列化(std::istream& is) {
        特征值主信息类 out{};

        std::uint64_t count = 0;
        is.read(reinterpret_cast<char*>(&count), sizeof(count));
        if (!is) return out;

        constexpr std::uint64_t 最大元素数 = 16ull * 1024ull * 1024ull;
        if (count > 最大元素数) {
            is.setstate(std::ios::failbit);
            return out;
        }

        out.值.resize(static_cast<std::size_t>(count));
        if (count != 0) {
            is.read(
                reinterpret_cast<char*>(out.值.data()),
                static_cast<std::streamsize>(count * sizeof(std::uint64_t))
            );
            if (!is) return 特征值主信息类{};
        }

        is.read(reinterpret_cast<char*>(&out.内容哈希), sizeof(out.内容哈希));
        is.read(reinterpret_cast<char*>(&out.命中次数), sizeof(out.命中次数));
        is.read(reinterpret_cast<char*>(&out.最后命中), sizeof(out.最后命中));
        if (!is) return 特征值主信息类{};

        return out;
    }
};
