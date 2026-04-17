module;

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "基础信息类.h"
#include "方法主信息类.h"
#include "世界树类.h"

export module 本能动作模块;

namespace 本能动作模块_detail {
    inline bool 是特征节点(const 基础信息节点类* 节点) noexcept
    {
        return 节点 && dynamic_cast<const 特征节点主信息类*>(节点->主信息) != nullptr;
    }

    inline 特征节点主信息类* 取特征主信息(基础信息节点类* 节点) noexcept
    {
        return 节点 ? dynamic_cast<特征节点主信息类*>(节点->主信息) : nullptr;
    }

    inline const 特征节点主信息类* 取特征主信息(const 基础信息节点类* 节点) noexcept
    {
        return 节点 ? dynamic_cast<const 特征节点主信息类*>(节点->主信息) : nullptr;
    }

    inline bool 特征有值(const 基础信息节点类* 节点) noexcept
    {
        const auto* 主信息 = 取特征主信息(节点);
        return 主信息 && 特征值有效(主信息->当前值);
    }

    inline const 特征值* 取特征值(const 基础信息节点类* 节点) noexcept
    {
        const auto* 主信息 = 取特征主信息(节点);
        return 主信息 ? &主信息->当前值 : nullptr;
    }

    inline const 词性节点类* 取类型(const 基础信息节点类* 节点) noexcept
    {
        return 节点 && 节点->主信息 ? 节点->主信息->类型 : nullptr;
    }

    inline std::string 取主键(const 基础信息节点类* 节点)
    {
        return 节点 ? 节点->获取主键() : std::string{};
    }

    inline std::vector<基础信息节点类*> 枚举子节点(基础信息节点类* 父节点)
    {
        std::vector<基础信息节点类*> 输出{};
        if (!父节点 || !父节点->子) {
            return 输出;
        }

        auto* 首节点 = static_cast<基础信息节点类*>(父节点->子);
        auto* 当前 = 首节点;
        do {
            输出.push_back(当前);
            当前 = 当前 ? static_cast<基础信息节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首节点);

        return 输出;
    }

    inline std::vector<const 基础信息节点类*> 枚举子节点_只读(const 基础信息节点类* 父节点)
    {
        std::vector<const 基础信息节点类*> 输出{};
        if (!父节点 || !父节点->子) {
            return 输出;
        }

        auto* 首节点 = static_cast<const 基础信息节点类*>(父节点->子);
        auto* 当前 = 首节点;
        do {
            输出.push_back(当前);
            当前 = 当前 ? static_cast<const 基础信息节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首节点);

        return 输出;
    }

    inline bool 是OR组(const 基础信息节点类* 模式节点) noexcept
    {
        return 是特征节点(模式节点)
            && !特征有值(模式节点)
            && 模式节点->子 != nullptr;
    }

    inline I64 饱和绝对差(I64 左, I64 右) noexcept
    {
        constexpr I64 最大值 = (std::numeric_limits<I64>::max)();
        if (左 >= 右) {
            if (右 < 0 && 左 > 最大值 + 右) {
                return 最大值;
            }
            return 左 - 右;
        }

        if (左 < 0 && 右 > 最大值 + 左) {
            return 最大值;
        }
        return 右 - 左;
    }
}

export {

class 本能动作类 {
public:
    struct 结构体_本能回执最小字段 {
        bool 成功 = false;
        I64 错误码 = 0;
        std::string 诊断{};
        时间戳 耗时_us = 0;
        时间戳 时间_系统 = 0;
    };

    struct 结构体_场景模式匹配参数 {
        I64 I64容忍误差 = 0;
        I64 歧义分差阈值 = 50;
        std::function<I64(const 词性节点类* 类型, const 特征值& 输入值, const 特征值& 模式值)> 差值算法{};
    };

    struct 结构体_场景模式匹配绑定 {
        const 基础信息节点类* 模式节点 = nullptr;
        基础信息节点类* 输入节点 = nullptr;
        I64 匹配分 = 0;
    };

    struct 结构体_场景模式匹配结果 {
        bool 匹配成功 = false;
        bool 有歧义 = false;
        I64 总分 = 0;
        std::vector<结构体_场景模式匹配绑定> 绑定表{};
        std::vector<const 基础信息节点类*> 未满足模式节点{};
    };

public:
    static inline void 回执_拼接诊断(std::string& 输入输出, const std::string& 追加)
    {
        if (追加.empty()) {
            return;
        }
        if (!输入输出.empty() && 输入输出.back() != ';') {
            输入输出.push_back(';');
        }
        输入输出 += 追加;
    }

    template<class T方法节点>
    static inline bool 方法首节点_确保条件参数类型(
        基础信息类& 基础信息,
        T方法节点* 方法首节点,
        const std::vector<const 词性节点类*>& 需要类型表)
    {
        if (!方法首节点) {
            return false;
        }

        auto* 方法主信息 = &方法首节点->主信息;
        if (方法主信息->节点种类 != 枚举_方法节点种类::方法首节点) {
            return false;
        }

        auto* 条件场景 = 方法主信息->条件场景.指针;
        if (!条件场景) {
            return false;
        }

        auto 已有该类型 = [&](const 词性节点类* 类型) {
            if (!类型) {
                return true;
            }
            for (auto* 子节点 : 基础信息.枚举子节点(条件场景)) {
                const auto* 特征主信息 = 基础信息.取主信息<特征节点主信息类>(子节点);
                if (特征主信息 && 特征主信息->类型 == 类型) {
                    return true;
                }
            }
            return false;
        };

        bool 已追加 = false;
        for (auto* 类型 : 需要类型表) {
            if (!类型 || 已有该类型(类型)) {
                continue;
            }

            auto* 新主信息 = new 特征节点主信息类(类型);
            auto* 新节点 = static_cast<特征节点类*>(基础信息.添加子节点(条件场景, 新主信息));
            if (!新节点) {
                delete 新主信息;
                continue;
            }

            if (!类型->获取主键().empty()) {
                新节点->设置主键(std::string("cond_") + 类型->获取主键());
            }
            已追加 = true;
        }

        return 已追加;
    }

    template<class T方法节点>
    static inline void 方法首节点_确保条件参数类型(
        T方法节点* 方法首节点,
        const std::vector<const 词性节点类*>& 需要类型表)
    {
        (void)方法首节点_确保条件参数类型(世界树.基础信息(), 方法首节点, 需要类型表);
    }

    static inline 结构体_场景模式匹配结果 场景_模式匹配(
        场景节点类* 输入场景根,
        const 场景节点类* 模式场景根,
        const 结构体_场景模式匹配参数& 参数 = {})
    {
        结构体_场景模式匹配结果 输出{};
        if (!输入场景根 || !模式场景根) {
            return 输出;
        }

        const auto 递归结果 = 递归匹配节点(输入场景根, 模式场景根, 参数, 输出);
        输出.匹配成功 = 递归结果.ok;
        输出.总分 = 递归结果.score;
        输出.有歧义 = 输出.有歧义 || 递归结果.ambiguous;
        return 输出;
    }

private:
    struct 局部结果 {
        bool ok = true;
        bool ambiguous = false;
        I64 score = 0;
    };

    static inline I64 差值(
        const 结构体_场景模式匹配参数& 参数,
        const 词性节点类* 类型,
        const 特征值& 输入值,
        const 特征值& 模式值)
    {
        if (参数.差值算法) {
            const I64 代价 = 参数.差值算法(类型, 输入值, 模式值);
            if (代价 >= 0) {
                return 代价;
            }
        }

        if (const auto* 左值 = std::get_if<I64>(&输入值)) {
            if (const auto* 右值 = std::get_if<I64>(&模式值)) {
                return 本能动作模块_detail::饱和绝对差(*左值, *右值);
            }
        }

        if (输入值 == 模式值) {
            return 0;
        }
        return (std::numeric_limits<I64>::max)();
    }

    static inline I64 值匹配分(
        const 结构体_场景模式匹配参数& 参数,
        const 词性节点类* 类型,
        const 基础信息节点类* 输入节点,
        const 基础信息节点类* 模式节点)
    {
        if (!本能动作模块_detail::是特征节点(模式节点)) {
            return 0;
        }

        if (!本能动作模块_detail::特征有值(模式节点)) {
            return 100;
        }

        const auto* 模式值 = 本能动作模块_detail::取特征值(模式节点);
        const auto* 输入值 = 本能动作模块_detail::取特征值(输入节点);
        if (!模式值 || !输入值) {
            return 0;
        }

        const I64 代价 = 差值(参数, 类型, *输入值, *模式值);
        if (代价 == (std::numeric_limits<I64>::max)()) {
            return 0;
        }

        if (std::holds_alternative<I64>(*输入值) && std::holds_alternative<I64>(*模式值)) {
            if (代价 <= 参数.I64容忍误差) {
                return 1000;
            }
        }

        return std::max<I64>(0, 1000 - std::min<I64>(1000, 代价));
    }

    static inline 局部结果 挑选绑定(
        基础信息节点类* 输入父节点,
        const 基础信息节点类* 模式子节点,
        const 结构体_场景模式匹配参数& 参数,
        结构体_场景模式匹配结果& 输入输出)
    {
        局部结果 输出{};
        if (!输入父节点 || !模式子节点) {
            输出.ok = false;
            return 输出;
        }

        const auto* 类型 = 本能动作模块_detail::取类型(模式子节点);
        if (!类型) {
            输出.ok = false;
            输入输出.未满足模式节点.push_back(模式子节点);
            return 输出;
        }

        struct 候选项 {
            基础信息节点类* 节点 = nullptr;
            I64 分数 = 0;
        };

        std::vector<候选项> 候选{};
        for (auto* 输入子节点 : 本能动作模块_detail::枚举子节点(输入父节点)) {
            if (!输入子节点 || 本能动作模块_detail::取类型(输入子节点) != 类型) {
                continue;
            }

            I64 分数 = 1000;
            分数 += 值匹配分(参数, 类型, 输入子节点, 模式子节点);
            候选.push_back({ 输入子节点, 分数 });
        }

        if (候选.empty()) {
            输出.ok = false;
            输入输出.未满足模式节点.push_back(模式子节点);
            return 输出;
        }

        std::sort(候选.begin(), 候选.end(), [](const 候选项& 左, const 候选项& 右) {
            if (左.分数 != 右.分数) {
                return 左.分数 > 右.分数;
            }
            return 本能动作模块_detail::取主键(左.节点) < 本能动作模块_detail::取主键(右.节点);
        });

        输入输出.绑定表.push_back({ 模式子节点, 候选.front().节点, 候选.front().分数 });
        输出.score += 候选.front().分数;

        if (候选.size() >= 2) {
            const I64 分差 = 本能动作模块_detail::饱和绝对差(候选[0].分数, 候选[1].分数);
            if (分差 <= 参数.歧义分差阈值) {
                输出.ambiguous = true;
                输入输出.有歧义 = true;
            }
        }

        return 输出;
    }

    static inline 局部结果 递归匹配节点(
        基础信息节点类* 输入节点,
        const 基础信息节点类* 模式节点,
        const 结构体_场景模式匹配参数& 参数,
        结构体_场景模式匹配结果& 输入输出)
    {
        局部结果 输出{};
        if (!输入节点 || !模式节点) {
            输出.ok = false;
            return 输出;
        }

        if (!模式节点->子) {
            return 输出;
        }

        return 递归匹配子节点(
            输入节点,
            模式节点,
            本能动作模块_detail::是OR组(模式节点),
            参数,
            输入输出);
    }

    static inline 局部结果 递归匹配子节点(
        基础信息节点类* 输入节点,
        const 基础信息节点类* 模式父节点,
        bool OR模式,
        const 结构体_场景模式匹配参数& 参数,
        结构体_场景模式匹配结果& 输入输出)
    {
        局部结果 输出{};
        const auto 模式子节点列表 = 本能动作模块_detail::枚举子节点_只读(模式父节点);

        if (OR模式) {
            bool 任一成功 = false;
            I64 最佳分数 = (std::numeric_limits<I64>::min)();
            结构体_场景模式匹配结果 最佳结果 = 输入输出;

            for (const auto* 模式子节点 : 模式子节点列表) {
                auto 临时结果 = 输入输出;
                auto 挑选结果 = 挑选绑定(输入节点, 模式子节点, 参数, 临时结果);
                if (!挑选结果.ok) {
                    continue;
                }

                auto* 已绑定输入节点 = 临时结果.绑定表.back().输入节点;
                auto 子结果 = 递归匹配节点(已绑定输入节点, 模式子节点, 参数, 临时结果);
                if (!子结果.ok) {
                    continue;
                }

                const I64 当前分数 = 挑选结果.score + 子结果.score;
                if (!任一成功 || 当前分数 > 最佳分数) {
                    最佳分数 = 当前分数;
                    最佳结果 = std::move(临时结果);
                    输出.ambiguous = 输出.ambiguous || 挑选结果.ambiguous || 子结果.ambiguous;
                    任一成功 = true;
                }
            }

            if (!任一成功) {
                输出.ok = false;
                输入输出.未满足模式节点.push_back(模式父节点);
                return 输出;
            }

            输入输出 = std::move(最佳结果);
            输出.score += 最佳分数;
            return 输出;
        }

        for (const auto* 模式子节点 : 模式子节点列表) {
            auto 挑选结果 = 挑选绑定(输入节点, 模式子节点, 参数, 输入输出);
            if (!挑选结果.ok) {
                输出.ok = false;
                return 输出;
            }

            输出.score += 挑选结果.score;
            输出.ambiguous = 输出.ambiguous || 挑选结果.ambiguous;

            auto* 已绑定输入节点 = 输入输出.绑定表.back().输入节点;
            auto 子结果 = 递归匹配节点(已绑定输入节点, 模式子节点, 参数, 输入输出);
            if (!子结果.ok) {
                输出.ok = false;
                return 输出;
            }

            输出.score += 子结果.score;
            输出.ambiguous = 输出.ambiguous || 子结果.ambiguous;
        }

        return 输出;
    }
};

} // export
