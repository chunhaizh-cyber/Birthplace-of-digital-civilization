#pragma once

#include "基础信息节点类型.h"
#include "语素节点类型.h"
#include "语素主信息类.h"

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

    词性节点类* 生成语素节点(结构体_分词 分词信息);

    词节点类* 添加词(const std::string& 词字符串);
    词节点类* 查找词节点(const std::string& 词字符串) const;
    词节点类* 获取词性词指针(const std::string& 词字符串) const;

    词性节点类* 查找词性节点(const 词节点类* 词节点, 枚举_词性 词性值) const;
    词性节点类* 查找词性节点(const 语素节点类* 词节点, const std::string& 词性值) const;
    词性节点类* 查找词性节点(结构体_分词 分词信息) const;

    词性节点类* 添加词性词(词节点类* 词节点, 枚举_词性 词性);
    词性节点类* 添加词性词(const std::string& 词值, const std::string& 词性值);
    词性节点类* 添加词性词(结构体_分词 词信息);

    const 词性节点类* 绑定词性词到场景基础信息(
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
    std::int64_t 添加对应基础信息(const 词性节点类* 词节点, 基础信息节点类* 基础信息);
    std::string 获取词(const 词性节点类* 词性词指针) const;

private:
    void 私有_清理主信息_已加锁() noexcept;
    词节点类* 查找词_已加锁(const std::string& 词字符串) const;
    词性节点类* 查找词性节点_已加锁(const 链节点类* 词节点, 枚举_词性 词性值) const;

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

