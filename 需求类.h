#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>

#include "基础信息主信息类.h"
#include "需求主信息类.h"

enum class 枚举_需求树更新指令类型 : std::uint8_t {
    未定义 = 0,
    新增需求 = 1,
    更新需求 = 2,
    满足需求 = 3,
    删除需求 = 4,
};

struct 结构_需求树更新指令 {
    枚举_需求树更新指令类型 指令类型 = 枚举_需求树更新指令类型::未定义;
    std::uintptr_t 父需求指针 = 0;
    std::string 父需求主键{};
    std::uintptr_t 目标需求指针 = 0;
    std::string 目标需求主键{};
    const 词性节点类* 需求类型 = nullptr;
    const 词性节点类* 需求名称 = nullptr;
    std::int64_t 新安全权重 = 0;
    bool 使用新安全权重 = false;
    std::int64_t 新服务权重 = 0;
    bool 使用新服务权重 = false;
    bool 标记已满足 = false;
    std::uintptr_t 需求状态指针 = 0;
    std::string 需求状态主键{};
    std::uintptr_t 当前状态指针 = 0;
    std::string 当前状态主键{};
    std::uintptr_t 来源任务头指针 = 0;
    std::string 来源任务头主键{};
    std::uintptr_t 来源步骤指针 = 0;
    std::string 来源步骤主键{};
    std::string 来源方法主键{};
    std::string 派生需求类型{};
    std::string 方法需求位{};
    std::string 本能能力缺口类型{};
    std::string 主观察特征{};
    枚举_需求任务挂接方式 任务挂接方式 = 枚举_需求任务挂接方式::未定义;
};

class 需求类 : public 链表模板<需求主信息类> {
public:
    using 基类 = 链表模板<需求主信息类>;
    using 节点类 = typename 基类::节点类;

    struct 结构_子需求平分权重 {
        std::int64_t 安全权重 = 0;
        std::int64_t 服务权重 = 0;
    };

    static 结构_子需求平分权重 计算子需求平分权重(
        const std::int64_t 父需求安全权重,
        const std::int64_t 父需求服务权重,
        const std::int64_t 子需求数量) noexcept
    {
        结构_子需求平分权重 输出{};
        if (子需求数量 <= 0) {
            return 输出;
        }
        输出.安全权重 = 父需求安全权重 / 子需求数量;
        输出.服务权重 = 父需求服务权重 / 子需求数量;
        return 输出;
    }

    static 结构_子需求平分权重 计算子需求平分权重(const 节点类* 父需求) noexcept
    {
        if (!父需求 || 父需求->子节点数量 <= 0) {
            return {};
        }
        return 计算子需求平分权重(
            父需求->主信息.安全权重,
            父需求->主信息.服务权重,
            父需求->子节点数量);
    }

    std::size_t 刷新子需求权重(节点类* 父需求) noexcept
    {
        auto 锁 = this->获取锁();
        return 刷新子需求权重_已加锁(父需求);
    }

    节点类* 执行更新指令(
        节点类* 需求根节点,
        const 结构_需求树更新指令& 指令) noexcept
    {
        auto 锁 = this->获取锁();
        if (!需求根节点) {
            return nullptr;
        }

        // 存在内部的需求树是独立树根，不由临时需求类实例拥有。
        // 执行期间借用真实根节点，避免新增节点挂到临时根后随临时对象析构被释放。
        auto* 原根 = this->根指针;
        this->根指针 = 需求根节点;
        auto* 结果 = 执行更新指令_已加锁(需求根节点, 指令);
        this->根指针 = 原根;
        return 结果;
    }

    // 方向 = 结果状态 - 初始状态，这里对应 需求状态 - 被需求当前状态。
    二次特征节点类* 获取需求方向(节点类* 需求节点) const noexcept;

private:
    static void 刷新同层环头_已加锁(节点类* 同层环头) noexcept
    {
        if (!同层环头) {
            return;
        }

        auto* 当前节点 = 同层环头;
        do {
            当前节点->同层环头 = 同层环头;
            当前节点 = 当前节点->下;
        } while (当前节点 && 当前节点 != 同层环头);
    }

    static 节点类* 获取独立树根_已加锁(节点类* 节点) noexcept
    {
        while (节点 && 节点->父) {
            节点 = static_cast<节点类*>(节点->父);
        }
        return 节点;
    }

    static 节点类* 按主键定位节点_已加锁(
        节点类* 需求根节点,
        const std::string& 主键) noexcept
    {
        if (!需求根节点 || 主键.empty()) {
            return nullptr;
        }

        for (auto* 当前节点 = 需求根节点->链下;
             当前节点 && 当前节点 != 需求根节点;
             当前节点 = 当前节点->链下) {
            if (当前节点->获取主键() == 主键) {
                return 当前节点;
            }
        }
        return nullptr;
    }

    static bool 节点属于独立树_已加锁(
        const 节点类* 需求根节点,
        const 节点类* 候选节点) noexcept
    {
        if (!需求根节点 || !候选节点) {
            return false;
        }
        if (需求根节点 == 候选节点) {
            return true;
        }

        for (auto* 当前节点 = 需求根节点->链下;
             当前节点 && 当前节点 != 需求根节点;
             当前节点 = 当前节点->链下) {
            if (当前节点 == 候选节点) {
                return true;
            }
        }
        return false;
    }

    static 节点类* 定位节点_已加锁(
        节点类* 需求根节点,
        const std::uintptr_t 节点指针,
        const std::string& 主键) noexcept
    {
        if (节点指针 != 0) {
            auto* 候选节点 = reinterpret_cast<节点类*>(节点指针);
            if (节点属于独立树_已加锁(需求根节点, 候选节点)) {
                return 候选节点;
            }
        }
        return 按主键定位节点_已加锁(需求根节点, 主键);
    }

    static bool 词性节点相同_已加锁(
        const 词性节点类* 左,
        const 词性节点类* 右) noexcept
    {
        if (左 == 右) {
            return true;
        }
        if (!左 || !右) {
            return false;
        }

        const auto 左主键 = 左->获取主键();
        const auto 右主键 = 右->获取主键();
        return !左主键.empty() && 左主键 == 右主键;
    }

    static const 状态节点主信息类* 状态主信息_已加锁(const 状态节点类* 状态) noexcept
    {
        return 状态 ? dynamic_cast<const 状态节点主信息类*>(状态->主信息) : nullptr;
    }

    static std::string 状态引用主键_已加锁(const 可解析引用<状态节点类>& 引用) noexcept
    {
        if (!引用.主键.empty()) {
            return 引用.主键;
        }
        return 引用.指针 ? 引用.指针->获取主键() : std::string{};
    }

    static std::string 状态特征类型主键_已加锁(const 状态节点主信息类* 主信息) noexcept
    {
        if (!主信息) {
            return {};
        }

        const auto* 特征主信息 = 主信息->状态特征.指针
            ? dynamic_cast<const 特征节点主信息类*>(主信息->状态特征.指针->主信息)
            : nullptr;
        if (特征主信息 && 特征主信息->类型) {
            return 特征主信息->类型->获取主键();
        }
        if (!主信息->状态特征.主键.empty()) {
            return 主信息->状态特征.主键;
        }
        return {};
    }

    static bool 状态语义相同_已加锁(
        const 状态节点类* 左,
        const 状态节点类* 右) noexcept
    {
        if (左 == 右 && 左) {
            return true;
        }

        const auto* 左主信息 = 状态主信息_已加锁(左);
        const auto* 右主信息 = 状态主信息_已加锁(右);
        if (!左主信息 || !右主信息) {
            return false;
        }

        const auto 左特征 = 状态特征类型主键_已加锁(左主信息);
        const auto 右特征 = 状态特征类型主键_已加锁(右主信息);
        if (!左特征.empty() && !右特征.empty() && 左特征 != 右特征) {
            return false;
        }
        return 左主信息->状态值 == 右主信息->状态值;
    }

    static bool 状态引用匹配指令_已加锁(
        const 可解析引用<状态节点类>& 已有,
        const std::uintptr_t 指令状态指针,
        const std::string& 指令状态主键) noexcept
    {
        auto* 指令状态 = reinterpret_cast<状态节点类*>(指令状态指针);
        const bool 存在状态条件 = 指令状态 != nullptr || !指令状态主键.empty();
        if (!存在状态条件) {
            return true;
        }

        if (已有.指针 && 指令状态 && 状态语义相同_已加锁(已有.指针, 指令状态)) {
            return true;
        }

        const auto 已有主键 = 状态引用主键_已加锁(已有);
        const auto 指令主键 = !指令状态主键.empty()
            ? 指令状态主键
            : (指令状态 ? 指令状态->获取主键() : std::string{});
        return !已有主键.empty() && !指令主键.empty() && 已有主键 == 指令主键;
    }

    static bool 指令包含状态条件_已加锁(const 结构_需求树更新指令& 指令) noexcept
    {
        return 指令.当前状态指针 != 0
            || !指令.当前状态主键.empty()
            || 指令.需求状态指针 != 0
            || !指令.需求状态主键.empty();
    }

    static bool 任务引用匹配指令_已加锁(
        const 可解析引用<任务节点类>& 已有,
        const std::uintptr_t 指令任务指针,
        const std::string& 指令任务主键) noexcept
    {
        const bool 存在任务条件 = 指令任务指针 != 0 || !指令任务主键.empty();
        if (!存在任务条件) {
            return true;
        }

        auto* 指令任务 = reinterpret_cast<任务节点类*>(指令任务指针);
        if (已有.指针 && 指令任务 && 已有.指针 == 指令任务) {
            return true;
        }
        return !已有.主键.empty() && !指令任务主键.empty() && 已有.主键 == 指令任务主键;
    }

    static bool 文本条件兼容_已加锁(
        const std::string& 已有,
        const std::string& 指令值) noexcept
    {
        return 指令值.empty() || 已有.empty() || 已有 == 指令值;
    }

    static bool 指令包含派生语义条件_已加锁(
        const 结构_需求树更新指令& 指令) noexcept
    {
        return !指令.派生需求类型.empty()
            || !指令.方法需求位.empty()
            || !指令.本能能力缺口类型.empty()
            || !指令.来源方法主键.empty();
    }

    节点类* 添加子需求节点_已加锁(
        节点类* 父需求,
        const 结构_需求树更新指令& 指令) noexcept
    {
        if (!父需求) {
            return nullptr;
        }

        需求主信息类 主信息{};
        主信息.类型 = 指令.需求类型 ? 指令.需求类型 : 指令.需求名称;
        if (auto* 当前状态 = reinterpret_cast<状态节点类*>(指令.当前状态指针)) {
            主信息.被需求当前状态.指针 = 当前状态;
            主信息.被需求当前状态.主键 = 当前状态->获取主键();
        } else if (!指令.当前状态主键.empty()) {
            主信息.被需求当前状态.主键 = 指令.当前状态主键;
        }
        if (auto* 需求状态 = reinterpret_cast<状态节点类*>(指令.需求状态指针)) {
            主信息.需求状态.指针 = 需求状态;
            主信息.需求状态.主键 = 需求状态->获取主键();
        } else if (!指令.需求状态主键.empty()) {
            主信息.需求状态.主键 = 指令.需求状态主键;
        }
        if (指令.使用新安全权重) {
            主信息.安全权重 = 指令.新安全权重;
        }
        if (指令.使用新服务权重) {
            主信息.服务权重 = 指令.新服务权重;
        }

        return 基类::添加子节点_已加锁(父需求, 主信息);
    }

    bool 删除节点_已加锁(节点类* 节点) noexcept
    {
        if (!节点) {
            return false;
        }

        auto* 独立根 = 获取独立树根_已加锁(节点);
        if (!独立根) {
            return false;
        }

        auto* 原根 = this->根指针;
        this->根指针 = 独立根;
        基类::删除节点_已加锁(节点);
        this->根指针 = 原根;
        return true;
    }

    节点类* 执行更新指令_已加锁(
        节点类* 需求根节点,
        const 结构_需求树更新指令& 指令) noexcept
    {
        if (!需求根节点) {
            return nullptr;
        }

        auto* 父需求 = 定位节点_已加锁(需求根节点, 指令.父需求指针, 指令.父需求主键);
        if (!父需求) {
            父需求 = 需求根节点;
        }

        auto* 目标需求 = 定位节点_已加锁(需求根节点, 指令.目标需求指针, 指令.目标需求主键);

        switch (指令.指令类型) {
        case 枚举_需求树更新指令类型::新增需求:
            if (!目标需求) {
                目标需求 = 添加子需求节点_已加锁(父需求, 指令);
            }
            if (父需求) {
                (void)刷新子需求权重_已加锁(父需求);
            }
            return 目标需求;

        case 枚举_需求树更新指令类型::更新需求:
        case 枚举_需求树更新指令类型::满足需求:
            if (!目标需求) {
                return nullptr;
            }
            if (指令.需求类型) {
                目标需求->主信息.类型 = 指令.需求类型;
            }
            if (指令.使用新安全权重) {
                目标需求->主信息.安全权重 = 指令.新安全权重;
            }
            if (指令.使用新服务权重) {
                目标需求->主信息.服务权重 = 指令.新服务权重;
            }
            if (指令.指令类型 == 枚举_需求树更新指令类型::满足需求 || 指令.标记已满足) {
                if (!指令.使用新安全权重) {
                    目标需求->主信息.安全权重 = 0;
                }
                if (!指令.使用新服务权重) {
                    目标需求->主信息.服务权重 = 0;
                }
            }
            if (auto* 当前状态 = reinterpret_cast<状态节点类*>(指令.当前状态指针)) {
                目标需求->主信息.被需求当前状态.指针 = 当前状态;
                目标需求->主信息.被需求当前状态.主键 = 当前状态->获取主键();
            } else if (!指令.当前状态主键.empty()) {
                目标需求->主信息.被需求当前状态.主键 = 指令.当前状态主键;
            }
            if (auto* 需求状态 = reinterpret_cast<状态节点类*>(指令.需求状态指针)) {
                目标需求->主信息.需求状态.指针 = 需求状态;
                目标需求->主信息.需求状态.主键 = 需求状态->获取主键();
            } else if (!指令.需求状态主键.empty()) {
                目标需求->主信息.需求状态.主键 = 指令.需求状态主键;
            }
            if (指令.指令类型 == 枚举_需求树更新指令类型::满足需求 || 指令.标记已满足) {
                目标需求->主信息.需求有效截止 = 结构体_时间戳::当前_微秒();
            }
            (void)刷新子需求权重_已加锁(目标需求);
            return 目标需求;

        case 枚举_需求树更新指令类型::删除需求:
            if (!目标需求 || 目标需求 == 需求根节点) {
                return nullptr;
            }
            父需求 = static_cast<节点类*>(目标需求->父);
            if (!删除节点_已加锁(目标需求)) {
                return nullptr;
            }
            if (父需求) {
                (void)刷新子需求权重_已加锁(父需求);
            }
            return 父需求;

        case 枚举_需求树更新指令类型::未定义:
        default:
            return nullptr;
        }
    }

    std::size_t 刷新子需求权重_已加锁(节点类* 父需求) noexcept
    {
        if (!父需求 || !父需求->子 || 父需求->子节点数量 <= 0) {
            return 0;
        }

        const auto 已刷新直接子节点数 = 刷新直接子需求权重_已加锁(父需求);
        auto* 首子节点 = 父需求->子;
        auto* 当前子节点 = 首子节点;
        std::size_t 已递归刷新数量 = 已刷新直接子节点数;

        do {
            已递归刷新数量 += 刷新子需求权重_已加锁(当前子节点);
            当前子节点 = 当前子节点->下;
        } while (当前子节点 && 当前子节点 != 首子节点);

        return 已递归刷新数量;
    }

    std::size_t 刷新直接子需求权重_已加锁(节点类* 父需求) noexcept
    {
        if (!父需求 || !父需求->子 || 父需求->子节点数量 <= 0) {
            return 0;
        }

        const auto 子需求权重 = 计算子需求平分权重(父需求);
        auto* 首子节点 = 父需求->子;
        auto* 当前子节点 = 首子节点;
        std::size_t 已刷新数量 = 0;

        do {
            当前子节点->主信息.安全权重 = 子需求权重.安全权重;
            当前子节点->主信息.服务权重 = 子需求权重.服务权重;
            ++已刷新数量;
            当前子节点 = 当前子节点->下;
        } while (当前子节点 && 当前子节点 != 首子节点);

        return 已刷新数量;
    }
};
