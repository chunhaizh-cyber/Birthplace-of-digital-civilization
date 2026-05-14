module;

#include <cstdint>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "日志接入.h"

export module 度量模板注册表模块;

import 基础数据类型模块;

export {

class 度量模板注册表类 {
public:
    enum class 枚举_模板族 : std::uint8_t {
        未定义 = 0,

        存在_归一化差异度Q = 1,
        存在_总体相似度Q = 2,
        存在_命中特征数 = 3,
        存在_比较特征数 = 4,
        存在_覆盖率Q = 5,
        存在_加权平均差异度Q = 6,
        存在_加权相似度Q = 7,

        特征_差异度Q = 20,
        特征_权重Q = 21,
        特征_缺失码 = 22,
        特征_贡献度Q = 23,
    };

    struct 结构_刻度片 {
        I64区间 区间{ 0, -1 };
        std::string 名称{};
        bool 是小区间 = false;
    };

    struct 结构_模板参数 {
        std::string 特征类型主键{};
    };

    class 模板基类 {
    public:
        virtual ~模板基类() = default;

        [[nodiscard]] virtual 枚举_模板族 模板族() const noexcept = 0;
        [[nodiscard]] virtual std::string 生成链键(const 结构_模板参数& 参数) const = 0;
        [[nodiscard]] virtual bool 匹配链键(std::string_view 链键) const noexcept = 0;

        [[nodiscard]] virtual std::vector<结构_刻度片> 默认刻度(const 结构_模板参数& 参数) const
        {
            (void)参数;
            return {};
        }

        virtual void 校验参数(const 结构_模板参数& 参数) const
        {
            (void)参数;
        }
    };

public:
    static 度量模板注册表类& 实例()
    {
        static 度量模板注册表类 实例{};
        return 实例;
    }

    void 注册(std::unique_ptr<模板基类> 模板)
    {
        if (!模板) {
            return;
        }

        std::unique_lock 锁(锁_);
        const auto 模板族 = 模板->模板族();
        表_[模板族] = std::move(模板);
    }

    [[nodiscard]] bool 已注册(枚举_模板族 模板族) const
    {
        std::shared_lock 锁(锁_);
        return 表_.find(模板族) != 表_.end();
    }

    [[nodiscard]] const 模板基类* 查找(枚举_模板族 模板族) const
    {
        std::shared_lock 锁(锁_);
        const auto it = 表_.find(模板族);
        return it == 表_.end() ? nullptr : it->second.get();
    }

    [[nodiscard]] const 模板基类& 获取或抛(枚举_模板族 模板族) const
    {
        const auto* 结果 = 查找(模板族);
        if (!结果) {
            项目运行错误日志(
                "度量模板注册表类/runtime_error | 模板未注册 | 模板族="
                + std::to_string(static_cast<std::uint8_t>(模板族)));
            throw std::runtime_error("度量模板注册表类：模板未注册");
        }
        return *结果;
    }

    [[nodiscard]] const 模板基类* 由链键查找(std::string_view 链键) const
    {
        std::shared_lock 锁(锁_);
        for (const auto& [_, 模板] : 表_) {
            if (模板 && 模板->匹配链键(链键)) {
                return 模板.get();
            }
        }
        return nullptr;
    }

    [[nodiscard]] std::string 生成链键_存在(枚举_模板族 模板族) const
    {
        结构_模板参数 参数{};
        return 获取或抛(模板族).生成链键(参数);
    }

    [[nodiscard]] std::string 生成链键_特征(枚举_模板族 模板族, std::string_view 特征类型主键) const
    {
        结构_模板参数 参数{};
        参数.特征类型主键 = std::string(特征类型主键);

        const auto& 模板 = 获取或抛(模板族);
        模板.校验参数(参数);
        return 模板.生成链键(参数);
    }

    [[nodiscard]] std::vector<结构_刻度片> 默认刻度_由链键(
        std::string_view 链键,
        std::string_view 特征类型主键 = {}) const
    {
        const auto* 模板 = 由链键查找(链键);
        if (!模板) {
            return {};
        }

        结构_模板参数 参数{};
        if (!特征类型主键.empty()) {
            参数.特征类型主键 = std::string(特征类型主键);
        }
        return 模板->默认刻度(参数);
    }

    void 注册默认模板()
    {
        std::unique_lock 锁(锁_);
        if (默认模板已注册_) {
            return;
        }

        表_[枚举_模板族::存在_归一化差异度Q] = std::make_unique<模板_存在_归一化差异度Q>();
        表_[枚举_模板族::存在_总体相似度Q] = std::make_unique<模板_存在_总体相似度Q>();
        表_[枚举_模板族::存在_命中特征数] = std::make_unique<模板_存在_命中特征数>();
        表_[枚举_模板族::存在_比较特征数] = std::make_unique<模板_存在_比较特征数>();
        表_[枚举_模板族::存在_覆盖率Q] = std::make_unique<模板_存在_覆盖率Q>();
        表_[枚举_模板族::存在_加权平均差异度Q] = std::make_unique<模板_存在_加权平均差异度Q>();
        表_[枚举_模板族::存在_加权相似度Q] = std::make_unique<模板_存在_加权相似度Q>();

        表_[枚举_模板族::特征_差异度Q] = std::make_unique<模板_特征_差异度Q>();
        表_[枚举_模板族::特征_权重Q] = std::make_unique<模板_特征_权重Q>();
        表_[枚举_模板族::特征_缺失码] = std::make_unique<模板_特征_缺失码>();
        表_[枚举_模板族::特征_贡献度Q] = std::make_unique<模板_特征_贡献度Q>();

        默认模板已注册_ = true;
    }

private:
    class 模板_存在_归一化差异度Q final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_归一化差异度Q; }
        [[nodiscard]] std::string 生成链键(const 结构_模板参数&) const override { return "存在|归一化差异度Q"; }
        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override { return 链键 == "存在|归一化差异度Q"; }
        [[nodiscard]] std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override
        {
            return {
                { I64区间{ 0, 3333 }, "小差异", false },
                { I64区间{ 3334, 6666 }, "中差异", false },
                { I64区间{ 6667, 10000 }, "大差异", false },
            };
        }
    };

    class 模板_存在_总体相似度Q final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_总体相似度Q; }
        [[nodiscard]] std::string 生成链键(const 结构_模板参数&) const override { return "存在|总体相似度Q"; }
        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override { return 链键 == "存在|总体相似度Q"; }
        [[nodiscard]] std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override
        {
            return {
                { I64区间{ 0, 3333 }, "低相似", false },
                { I64区间{ 3334, 6666 }, "中相似", false },
                { I64区间{ 6667, 10000 }, "高相似", false },
            };
        }
    };

    class 模板_存在_命中特征数 final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_命中特征数; }
        [[nodiscard]] std::string 生成链键(const 结构_模板参数&) const override { return "存在|命中特征数"; }
        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override { return 链键 == "存在|命中特征数"; }
    };

    class 模板_存在_比较特征数 final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_比较特征数; }
        [[nodiscard]] std::string 生成链键(const 结构_模板参数&) const override { return "存在|比较特征数"; }
        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override { return 链键 == "存在|比较特征数"; }
    };

    class 模板_存在_覆盖率Q final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_覆盖率Q; }
        [[nodiscard]] std::string 生成链键(const 结构_模板参数&) const override { return "存在|覆盖率Q"; }
        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override { return 链键 == "存在|覆盖率Q"; }
        [[nodiscard]] std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override
        {
            return {
                { I64区间{ 0, 3333 }, "覆盖低", false },
                { I64区间{ 3334, 6666 }, "覆盖中", false },
                { I64区间{ 6667, 10000 }, "覆盖高", false },
            };
        }
    };

    class 模板_存在_加权平均差异度Q final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_加权平均差异度Q; }
        [[nodiscard]] std::string 生成链键(const 结构_模板参数&) const override { return "存在|加权平均差异度Q"; }
        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override { return 链键 == "存在|加权平均差异度Q"; }
    };

    class 模板_存在_加权相似度Q final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_加权相似度Q; }
        [[nodiscard]] std::string 生成链键(const 结构_模板参数&) const override { return "存在|加权相似度Q"; }
        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override { return 链键 == "存在|加权相似度Q"; }
        [[nodiscard]] std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override
        {
            return {
                { I64区间{ 0, 3333 }, "低相似", false },
                { I64区间{ 3334, 6666 }, "中相似", false },
                { I64区间{ 6667, 10000 }, "高相似", false },
            };
        }
    };

    class 模板_特征_差异度Q final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::特征_差异度Q; }

        void 校验参数(const 结构_模板参数& 参数) const override
        {
            if (参数.特征类型主键.empty()) {
                throw std::invalid_argument("模板_特征_差异度Q：特征类型主键为空");
            }
        }

        [[nodiscard]] std::string 生成链键(const 结构_模板参数& 参数) const override
        {
            return std::string("特征|差异度Q|T=") + 参数.特征类型主键;
        }

        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override
        {
            return 链键.rfind("特征|差异度Q|T=", 0) == 0;
        }

        [[nodiscard]] std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override
        {
            return {
                { I64区间{ 0, 3333 }, "小差异", false },
                { I64区间{ 3334, 6666 }, "中差异", false },
                { I64区间{ 6667, 10000 }, "大差异", false },
            };
        }
    };

    class 模板_特征_权重Q final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::特征_权重Q; }

        void 校验参数(const 结构_模板参数& 参数) const override
        {
            if (参数.特征类型主键.empty()) {
                throw std::invalid_argument("模板_特征_权重Q：特征类型主键为空");
            }
        }

        [[nodiscard]] std::string 生成链键(const 结构_模板参数& 参数) const override
        {
            return std::string("特征|权重Q|T=") + 参数.特征类型主键;
        }

        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override
        {
            return 链键.rfind("特征|权重Q|T=", 0) == 0;
        }
    };

    class 模板_特征_缺失码 final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::特征_缺失码; }

        void 校验参数(const 结构_模板参数& 参数) const override
        {
            if (参数.特征类型主键.empty()) {
                throw std::invalid_argument("模板_特征_缺失码：特征类型主键为空");
            }
        }

        [[nodiscard]] std::string 生成链键(const 结构_模板参数& 参数) const override
        {
            return std::string("特征|缺失码|T=") + 参数.特征类型主键;
        }

        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override
        {
            return 链键.rfind("特征|缺失码|T=", 0) == 0;
        }

        [[nodiscard]] std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override
        {
            return {
                { I64区间{ 0, 0 }, "齐全", false },
                { I64区间{ 1, 1 }, "缺A", false },
                { I64区间{ 2, 2 }, "缺B", false },
                { I64区间{ 3, 3 }, "都缺", false },
            };
        }
    };

    class 模板_特征_贡献度Q final : public 模板基类 {
    public:
        [[nodiscard]] 枚举_模板族 模板族() const noexcept override { return 枚举_模板族::特征_贡献度Q; }

        void 校验参数(const 结构_模板参数& 参数) const override
        {
            if (参数.特征类型主键.empty()) {
                throw std::invalid_argument("模板_特征_贡献度Q：特征类型主键为空");
            }
        }

        [[nodiscard]] std::string 生成链键(const 结构_模板参数& 参数) const override
        {
            return std::string("特征|贡献度Q|T=") + 参数.特征类型主键;
        }

        [[nodiscard]] bool 匹配链键(std::string_view 链键) const noexcept override
        {
            return 链键.rfind("特征|贡献度Q|T=", 0) == 0;
        }
    };

private:
    度量模板注册表类() = default;

    mutable std::shared_mutex 锁_{};
    std::unordered_map<枚举_模板族, std::unique_ptr<模板基类>> 表_{};
    bool 默认模板已注册_ = false;
};

} // export
