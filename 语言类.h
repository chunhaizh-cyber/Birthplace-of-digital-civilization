#pragma once

#include <string>

#include "语言主信息类.h"

class 语言类 : public 链表模板<自然语言成分基类*> {
public:
    using 基类 = 链表模板<自然语言成分基类*>;
    using 节点类 = 基类::节点类;

public:
    语言类();
    ~语言类();

    语言类(const 语言类&) = delete;
    语言类& operator=(const 语言类&) = delete;
    语言类(语言类&&) = delete;
    语言类& operator=(语言类&&) = delete;

    void 初始化();

    std::string 获取基础信息名称(const 基础信息节点类* 节点) const;
    std::string 生成二次特征反义词结果(const 二次特征节点类* 节点) const;
    std::string 生成二次特征介词短语(const 二次特征节点类* 节点) const;
    std::string 生成二次特征自然语言(const 二次特征节点类* 节点) const;
    std::string 生成存在有什么表达(const 存在节点类* 节点, std::size_t 上限 = 5) const;
    std::string 生成存在是什么表达(const 存在节点类* 节点) const;
    std::string 生成存在能什么表达(const 存在节点类* 节点, std::size_t 上限 = 5) const;
    std::string 生成存在会什么表达(const 存在节点类* 节点, std::size_t 上限 = 3) const;
    std::string 生成存在需要什么表达(const 存在节点类* 节点, std::size_t 上限 = 5) const;

    文章节点类* 新建文章(const std::string& 标题, const std::string& 原文);
    段落节点类* 添加段落(文章节点类* 文章节点, std::size_t 在文章中的序号, const std::string& 段落文本);
    自然句节点类* 添加自然句(
        段落节点类* 段落节点,
        std::size_t 在段落中的序号,
        const std::vector<const 词性节点类*>& 词序列,
        const std::string& 原句文本
    );
    子句节点类* 添加子句(自然句节点类* 自然句节点, 子句主信息类* 子句主信息);

private:
    void 私有_清理主信息_已加锁() noexcept;
};

extern 语言类 语言集;
