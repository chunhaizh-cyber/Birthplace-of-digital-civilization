#pragma once

#include "基础信息节点类型.h"
#include "语素节点类型.h"
#include "语素主信息类.h"

class 基础信息类;

class 语素类 : public 链表模板<语素基类*> {
public:
    using 基类 = 链表模板<语素基类*>;
    using 链节点类 = 基类::节点类;

public:
    语素类();
    ~语素类();

    语素类(const 语素类&) = delete;
    语素类& operator=(const 语素类&) = delete;
    语素类(语素类&&) = delete;
    语素类& operator=(语素类&&) = delete;

    void 初始化();

    语素入口节点类* 生成语素节点(结构体_分词 分词信息);

    词节点类* 添加词(const std::string& 词字符串);
    词节点类* 查找词节点(const std::string& 词字符串) const;
    词节点类* 获取词节点指针(const std::string& 词字符串) const;

    语素入口节点类* 查找人类词性入口节点(const 词节点类* 词节点, 枚举_词性 词性值) const;
    语素入口节点类* 查找人类词性入口节点(const 语素节点类* 词节点, const std::string& 词性值) const;
    语素入口节点类* 查找人类词性入口节点(结构体_分词 分词信息) const;
    语素入口节点类* 查找信息入口节点(const 词节点类* 词节点, 枚举_信息入口类型 信息入口类型) const;

    // 人类词性入口：仅用于自然语言分词/词性标注和信息入口候选推断。
    // 业务语义落树、模板/实例入口创建必须使用 添加信息入口词。
    语素入口节点类* 添加词性词(词节点类* 词节点, 枚举_词性 词性);
    语素入口节点类* 添加词性词(const std::string& 词值, 枚举_词性 词性);
    语素入口节点类* 添加词性词(const std::string& 词值, const std::string& 词性值);
    语素入口节点类* 添加词性词(结构体_分词 词信息);

    // 信息入口类型入口：系统内部语义入口的权威创建接口。
    语素入口节点类* 添加信息入口词(词节点类* 词节点, 枚举_信息入口类型 信息入口类型);
    语素入口节点类* 添加信息入口词(const std::string& 词值, 枚举_信息入口类型 信息入口类型);

    基础信息节点类* 确保语素入口对应基础信息(
        语素入口节点类* 入口节点,
        基础信息类* 基础信息仓库 = nullptr,
        基础信息节点类* 父节点 = nullptr
    );
    基础信息节点类* 确保语素入口对应基础信息(
        const std::string& 词值,
        枚举_词性 词性,
        基础信息类* 基础信息仓库 = nullptr,
        基础信息节点类* 父节点 = nullptr
    );
    基础信息节点类* 确保信息入口对应基础信息(
        const std::string& 词值,
        枚举_信息入口类型 信息入口类型,
        基础信息类* 基础信息仓库 = nullptr,
        基础信息节点类* 父节点 = nullptr
    );
    基础信息节点类* 查找语素入口对应基础信息(const 语素入口节点类* 入口节点) const;
    枚举_主信息类型 获取语素入口对应基础信息类型(const 语素入口节点类* 入口节点) const;
    枚举_信息入口类型 获取语素入口信息入口类型(const 语素入口节点类* 入口节点) const;
    static 枚举_主信息类型 推断人类词性默认基础信息类型(枚举_词性 词性) noexcept;

    const 语素入口节点类* 绑定人类词性入口到场景基础信息(
        const std::string& 词字符串,
        枚举_词性 词性,
        场景节点类* 场景,
        基础信息节点类* 基础信息节点
    );

    std::vector<基础信息节点类*> 通过词和场景查找基础信息(
        const std::string& 词字符串,
        场景节点类* 当前场景,
        const 枚举_词性* 词性 = nullptr
    ) const;

    词节点类* 添加二次特征词(语素节点类* 参照物, 语素节点类* 比较对象, 语素节点类* 比较类型);
    std::int64_t 添加对应基础信息(const 语素入口节点类* 词节点, 基础信息节点类* 基础信息);
    std::string 获取词(const 语素入口节点类* 语素入口指针) const;

private:
    void 私有_清理主信息_已加锁() noexcept;
    词节点类* 查找词_已加锁(const std::string& 词字符串) const;
    语素入口节点类* 查找人类词性入口节点_已加锁(const 链节点类* 词节点, 枚举_词性 词性值) const;
    语素入口节点类* 查找信息入口节点_已加锁(const 链节点类* 词节点, 枚举_信息入口类型 信息入口类型) const;

    static std::size_t 私有_UTF8字符长度(unsigned char c) noexcept;
};

class 枚举_词性_工厂 {
public:
    static std::string 根据枚举类型获取文本(枚举_词性 词性值);
    static 枚举_词性 根据文本获取枚举值(const std::string& 文本);
    static void 添加动态映射(枚举_词性 词性值, const std::string& 文本描述);
    static void 批量添加动态映射(const std::vector<std::pair<枚举_词性, std::string>>& 映射列表);
    static void 清除动态映射();
};

extern 语素类 语素集;
