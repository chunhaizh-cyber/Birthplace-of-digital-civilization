#pragma once

#include "语素节点类型.h"
#include "基础信息节点类型.h"

#include "基础数据类型.h"

// ============================================================================
// 语素族
// ============================================================================
class 语素基类 {
public:
    枚举_主信息类型 主信息类型 = 枚举_主信息类型::语素基类;
    virtual ~语素基类() = default;
    virtual std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段) const = 0;
};

class 词主信息类 : public 语素基类 {
public:
    std::string 词{};
    explicit 词主信息类(std::string v = {}) : 词(std::move(v)) {
        主信息类型 = 枚举_主信息类型::词;
    }
    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<词主信息类*>(对象);
        if (!右) return -1;
        if (字段 != 枚举_比较字段::语素_词_词) return -1;
        return 词.compare(右->词);
    }
};

class 词性主信息类 : public 语素基类 {
public:
    枚举_词性 词性 = 枚举_词性::未定义;
    可解析引用<基础信息节点类> 对应基础信息{};

    词性主信息类() { 主信息类型 = 枚举_主信息类型::词性; }
    explicit 词性主信息类(枚举_词性 v) : 词性(v) {
        主信息类型 = 枚举_主信息类型::词性;
    }
    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<词性主信息类*>(对象);
        if (!右) return -1;
        if (字段 != 枚举_比较字段::语素_词性_词性) return -1;
        return static_cast<std::int64_t>(词性) - static_cast<std::int64_t>(右->词性);
    }
};

class 短语主信息类 : public 语素基类 {
public:
    const 词性节点类* 参照词 = nullptr;
    const 词性节点类* 比较对象词 = nullptr;

    短语主信息类(const 词性节点类* a = nullptr, const 词性节点类* b = nullptr)
        : 参照词(a), 比较对象词(b) {
        主信息类型 = 枚举_主信息类型::短语;
    }
    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<短语主信息类*>(对象);
        if (!右) return -1;
        switch (字段) {
        case 枚举_比较字段::语素_短语字节点_参照词:
            return reinterpret_cast<std::intptr_t>(参照词) - reinterpret_cast<std::intptr_t>(右->参照词);
        case 枚举_比较字段::语素_短语字节点_比较对象词:
            return reinterpret_cast<std::intptr_t>(比较对象词) - reinterpret_cast<std::intptr_t>(右->比较对象词);
        default:
            return -1;
        }
    }
};

class 短语子节点主信息类 : public 语素基类 {
public:
    const 词性节点类* 比较词 = nullptr;
    const 词性节点类* 结果词 = nullptr;

    短语子节点主信息类(const 词性节点类* a = nullptr, const 词性节点类* b = nullptr)
        : 比较词(a), 结果词(b) {
        主信息类型 = 枚举_主信息类型::短语子;
    }
    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<短语子节点主信息类*>(对象);
        if (!右) return -1;
        switch (字段) {
        case 枚举_比较字段::语素_短语子节点_比较词:
            return reinterpret_cast<std::intptr_t>(比较词) - reinterpret_cast<std::intptr_t>(右->比较词);
        case 枚举_比较字段::语素_短语子节点_结果词:
            return reinterpret_cast<std::intptr_t>(结果词) - reinterpret_cast<std::intptr_t>(右->结果词);
        default:
            return -1;
        }
    }
};
