module;

#include <array>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "基础信息节点类型.h"

export module 本能线程模块;

export struct 绝对坐标快照 {
    std::array<std::int64_t, 3> 相对位置_mm{ 0, 0, 0 };
    std::array<float, 16> 世界变换4x4{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
};

export struct 绝对坐标结果 {
    const 存在节点类* e = nullptr;
    std::array<std::int64_t, 3> 绝对位置_mm{ 0, 0, 0 };
};

export struct 子特征快照 {
    const 存在节点类* 父 = nullptr;
    std::uint64_t 版本签名 = 0;
    std::vector<std::uint64_t> 轮廓blocks{};
    std::int32_t 裁剪宽 = 0;
    std::int32_t 裁剪高 = 0;
    std::vector<std::uint8_t> 裁剪BGR{};
    std::vector<std::uint8_t> 裁剪mask{};
};

export struct 子部件结果 {
    std::string 部件键{};
    std::array<std::int64_t, 3> 相对位置_mm{ 0, 0, 0 };
    std::array<std::int64_t, 3> 尺寸_mm{ 1, 1, 1 };
    std::vector<std::uint64_t> 轮廓blocks{};
    std::array<std::int64_t, 3> 颜色_BGR{ 0, 0, 0 };
};

export struct 子特征结果 {
    const 存在节点类* 父 = nullptr;
    std::uint64_t 版本签名 = 0;
    std::vector<子部件结果> 部件{};
};

export class 本能线程类 {
public:
    using Fn取绝对快照 = std::function<bool(const 存在节点类* e, 绝对坐标快照& out)>;
    using Fn算绝对坐标 = std::function<bool(const 绝对坐标快照& in, 绝对坐标结果& out)>;
    using Fn写入绝对坐标 = std::function<void(const 绝对坐标结果& r)>;
    using Fn取子特征快照 = std::function<bool(const 存在节点类* 父, std::uint64_t sig, 子特征快照& out)>;
    using Fn算子特征 = std::function<bool(const 子特征快照& in, 子特征结果& out)>;
    using Fn写入子特征 = std::function<void(const 子特征结果& r)>;

    本能线程类();
    ~本能线程类();

    本能线程类(const 本能线程类&) = delete;
    本能线程类& operator=(const 本能线程类&) = delete;

    void 设置_绝对坐标流程(Fn取绝对快照 snap, Fn算绝对坐标 calc, Fn写入绝对坐标 commit);
    void 设置_子特征流程(Fn取子特征快照 snap, Fn算子特征 calc, Fn写入子特征 commit);

    void 启动();
    void 停止();
    bool 是否正在运行() const noexcept;

    void 请求刷新绝对坐标(const 存在节点类* e);
    void 请求提取子特征(const 存在节点类* 父, std::uint64_t sig);
    void 通知存在已移除(const 存在节点类* e);
};
