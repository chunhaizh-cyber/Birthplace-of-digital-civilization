#include "世界树类.h"

#include <algorithm>
#include <bit>
#include <limits>
#include <utility>

#include "度量模板注册表桥接.h"
#include "语素类.h"

import 二次特征应用模块;

namespace {
    I64 私有_饱和减(I64 a, I64 b) noexcept
    {
        if (b == I64_MIN) return I64_MAX;
        if (b > 0 && a < I64_MIN + b) return I64_MIN;
        if (b < 0 && a > I64_MAX + b) return I64_MAX;
        return a - b;
    }

    I64 私有_绝对值饱和(I64 值) noexcept
    {
        if (值 == I64_MIN) return I64_MAX;
        return 值 < 0 ? -值 : 值;
    }

    I64 私有_VecU差异率Q10000(const VecIU64& 左值, const VecIU64& 右值) noexcept
    {
        const std::size_t 块数 = std::max(左值.size(), 右值.size());
        if (块数 == 0) return 0;

        std::uint64_t 差异位数 = 0;
        const auto 公共块数 = std::min(左值.size(), 右值.size());
        for (std::size_t i = 0; i < 公共块数; ++i) {
            差异位数 += static_cast<std::uint64_t>(std::popcount(左值[i] ^ 右值[i]));
        }
        if (左值.size() > 公共块数) {
            差异位数 += static_cast<std::uint64_t>(左值.size() - 公共块数) * 64ull;
        }
        if (右值.size() > 公共块数) {
            差异位数 += static_cast<std::uint64_t>(右值.size() - 公共块数) * 64ull;
        }

        const std::uint64_t 总位数 = static_cast<std::uint64_t>(块数) * 64ull;
        return static_cast<I64>((差异位数 * 10000ull) / (总位数 ? 总位数 : 1ull));
    }

    场景节点类* 私有_取存在所在场景(
        const 基础信息类& 基础信息,
        const 存在节点类* 存在) noexcept
    {
        auto* 父节点 = 存在 ? static_cast<基础信息节点类*>(存在->父) : nullptr;
        return 父节点 && 基础信息.取主信息<场景节点主信息类>(父节点)
            ? static_cast<场景节点类*>(父节点)
            : nullptr;
    }
}

世界树类 世界树{};

世界树类::世界树类()
    : 特征服务_(&基础信息链_)
    , 存在服务_(&基础信息链_)
    , 场景服务_(&基础信息链_)
    , 二次特征服务_(&基础信息链_)
    , 二次特征生成服务_(std::make_unique<二次特征生成模块>(this))
    , 状态服务_(&基础信息链_)
    , 动态服务_(&基础信息链_)
    , 因果服务_(&基础信息链_)
{
    特征服务_.设置差异度函数([this](const 词性节点类*, const 特征值& 左值, const 特征值& 右值) -> I64 {
        return 默认特征差异度(左值, 右值);
    });
}

世界树类::~世界树类() = default;

void 世界树类::初始化()
{
    基础信息链_.初始化();
    值池_.初始化();
    语素集.初始化();
    初始化度量模板注册表();
}

void 世界树类::初始化默认世界()
{
    现实世界 = 取或创建默认世界("现实世界", 枚举_世界类型::现实世界);
    文本世界 = 取或创建默认世界("文本世界", 枚举_世界类型::文本世界);
    想象世界 = 取或创建默认世界("想象世界", 枚举_世界类型::想象世界);
    记忆世界 = 取或创建默认世界("记忆世界", 枚举_世界类型::记忆世界);
    推理世界 = 取或创建默认世界("推理世界", 枚举_世界类型::推理世界);
    虚拟世界 = 取或创建默认世界("虚拟世界", 枚举_世界类型::虚拟世界);
}

基础信息类& 世界树类::基础信息() noexcept { return 基础信息链_; }
const 基础信息类& 世界树类::基础信息() const noexcept { return 基础信息链_; }
特征值类& 世界树类::值池() noexcept { return 值池_; }
const 特征值类& 世界树类::值池() const noexcept { return 值池_; }
特征类& 世界树类::特征() noexcept { return 特征服务_; }
const 特征类& 世界树类::特征() const noexcept { return 特征服务_; }
存在类& 世界树类::存在() noexcept { return 存在服务_; }
const 存在类& 世界树类::存在() const noexcept { return 存在服务_; }
场景类& 世界树类::场景() noexcept { return 场景服务_; }
const 场景类& 世界树类::场景() const noexcept { return 场景服务_; }
二次特征类& 世界树类::二次特征() noexcept { return 二次特征服务_; }
const 二次特征类& 世界树类::二次特征() const noexcept { return 二次特征服务_; }
二次特征生成模块& 世界树类::二次特征生成() noexcept { return *二次特征生成服务_; }
const 二次特征生成模块& 世界树类::二次特征生成() const noexcept { return *二次特征生成服务_; }
状态类& 世界树类::状态() noexcept { return 状态服务_; }
const 状态类& 世界树类::状态() const noexcept { return 状态服务_; }
动态类& 世界树类::动态() noexcept { return 动态服务_; }
const 动态类& 世界树类::动态() const noexcept { return 动态服务_; }
因果类& 世界树类::因果() noexcept { return 因果服务_; }
const 因果类& 世界树类::因果() const noexcept { return 因果服务_; }

基础信息节点类* 世界树类::世界根() const noexcept
{
    return 基础信息链_.世界根();
}

std::vector<基础信息节点类*> 世界树类::获取子节点(const 基础信息节点类* 父节点) const
{
    return 基础信息链_.枚举子节点(父节点);
}

std::string 世界树类::获取名称(const 基础信息节点类* 节点) const
{
    return 基础信息链_.获取名称(节点);
}

特征节点类* 世界树类::创建特征(基础信息节点类* 宿主, 特征节点主信息类* 主信息)
{
    return 特征服务_.创建特征(宿主, 主信息);
}

特征节点类* 世界树类::确保特征(基础信息节点类* 宿主, const 词性节点类* 特征类型)
{
    return 特征服务_.取或创建子特征_按类型(宿主, 特征类型);
}

特征节点类* 世界树类::取或创建子特征_按类型(基础信息节点类* 宿主, const 词性节点类* 特征类型)
{
    return 特征服务_.取或创建子特征_按类型(宿主, 特征类型);
}

存在节点类* 世界树类::创建存在(基础信息节点类* 父节点, 存在节点主信息类* 主信息)
{
    return 存在服务_.创建存在(父节点, 主信息);
}

存在节点类* 世界树类::取或创建子存在_按类型(基础信息节点类* 父节点, const 词性节点类* 类型)
{
    if (!类型) return nullptr;
    if (auto* 已有存在 = 存在服务_.查找子存在_按类型(父节点, 类型)) {
        return 已有存在;
    }
    return 存在服务_.创建存在_按名称类型(父节点, nullptr, 类型);
}

存在节点类* 世界树类::取或创建子存在_按类型并按特征I64(
    基础信息节点类* 父节点,
    const 词性节点类* 类型,
    const 词性节点类* 唯一键特征类型,
    I64 唯一键特征值)
{
    if (!类型 || !唯一键特征类型) return nullptr;

    for (auto* 存在节点 : 存在服务_.获取子存在(父节点)) {
        const auto* 存在主信息 = 存在服务_.取存在主信息(存在节点);
        if (!存在主信息 || 存在主信息->类型 != 类型) continue;

        auto* 特征节点 = 特征服务_.查找子特征_按类型(存在节点, 唯一键特征类型);
        I64 当前值 = 0;
        if (特征节点 && 读取特征_I64(特征节点, 当前值) && 当前值 == 唯一键特征值) {
            return 存在节点;
        }
    }

    auto* 新存在 = 存在服务_.创建存在_按名称类型(父节点, nullptr, 类型);
    if (!新存在) return nullptr;

    auto* 特征节点 = 确保特征(新存在, 唯一键特征类型);
    if (!特征节点) return 新存在;

    (void)写入特征_I64(特征节点, 唯一键特征值);
    return 新存在;
}

场景节点类* 世界树类::创建场景(基础信息节点类* 父节点, 场景节点主信息类* 主信息)
{
    return 场景服务_.创建场景(父节点, 主信息);
}

场景节点类* 世界树类::取或创建子场景_按名称(
    基础信息节点类* 父节点,
    const 词性节点类* 名称,
    枚举_世界类型 世界类型)
{
    return 场景服务_.取或创建子场景_按名称(父节点, 名称, 世界类型);
}

二次特征节点类* 世界树类::创建二次特征(基础信息节点类* 父节点, 二次特征主信息类* 主信息)
{
    return 二次特征服务_.创建二次特征(父节点, 主信息);
}

状态节点类* 世界树类::创建状态(场景节点类* 场景, 状态节点主信息类* 主信息)
{
    return 状态服务_.创建状态(场景, 主信息);
}

动态节点类* 世界树类::创建动态(场景节点类* 场景, 动态节点主信息类* 主信息)
{
    return 动态服务_.创建动态(场景, 主信息);
}

因果模板节点类* 世界树类::创建因果模板(基础信息节点类* 父节点, 因果主信息类* 主信息)
{
    return 因果服务_.创建因果模板(父节点, 主信息);
}

因果模板节点类* 世界树类::按动态创建因果信息(
    场景节点类* 场景,
    动态节点类* 动态节点,
    枚举_因果来源类型 来源类型,
    基础信息节点类* 动作主体,
    基础信息节点类* 动作语义,
    const std::string& 动作语义键,
    bool 设为主结果)
{
    return 因果服务_.按动态创建因果信息(
        场景,
        动态节点,
        来源类型,
        动作主体,
        动作语义,
        动作语义键,
        设为主结果);
}

bool 世界树类::写入特征_I64(特征节点类* 节点, I64 值, 时间戳 now)
{
    return 特征服务_.写入特征值_I64(节点, 值, now);
}

bool 世界树类::写入特征_I64(基础信息节点类* 宿主, const 词性节点类* 特征类型, I64 值, 时间戳 now)
{
    auto* 特征节点 = 确保特征(宿主, 特征类型);
    return 特征节点 ? 写入特征_I64(特征节点, 值, now) : false;
}

bool 世界树类::写入特征_指针(特征节点类* 节点, const void* 指针, 时间戳 now)
{
    指针句柄 句柄{};
    句柄.指针 = reinterpret_cast<std::uintptr_t>(指针);
    return 特征服务_.写入特征值_指针句柄(节点, 句柄, now);
}

bool 世界树类::写入特征_指针(
    基础信息节点类* 宿主,
    const 词性节点类* 特征类型,
    const void* 指针,
    时间戳 now)
{
    auto* 特征节点 = 确保特征(宿主, 特征类型);
    return 特征节点 ? 写入特征_指针(特征节点, 指针, now) : false;
}

bool 世界树类::写入特征_VecU(
    特征节点类* 节点,
    VecIU64 值,
    枚举_轮廓维度 维度,
    时间戳 now)
{
    const auto 句柄 = 值池_.获取或创建句柄(std::move(值), 维度);
    return 特征服务_.写入特征值_VecU句柄(节点, 句柄, now);
}

bool 世界树类::写入特征_VecU(
    基础信息节点类* 宿主,
    const 词性节点类* 特征类型,
    VecIU64 值,
    枚举_轮廓维度 维度,
    时间戳 now)
{
    auto* 特征节点 = 确保特征(宿主, 特征类型);
    return 特征节点 ? 写入特征_VecU(特征节点, std::move(值), 维度, now) : false;
}

特征值 世界树类::读取特征快照(const 特征节点类* 节点) const
{
    return 特征服务_.读取特征值(节点);
}

特征值 世界树类::读取特征快照(const 基础信息节点类* 宿主, const 词性节点类* 特征类型) const
{
    auto* 特征节点 = 特征服务_.查找子特征_按类型(宿主, 特征类型);
    return 特征节点 ? 读取特征快照(特征节点) : 特征值{};
}

bool 世界树类::读取特征_I64(const 特征节点类* 节点, I64& 输出值) const
{
    const auto 值 = 读取特征快照(节点);
    if (const auto* 标量 = std::get_if<I64>(&值)) {
        输出值 = *标量;
        return true;
    }
    return false;
}

bool 世界树类::读取特征_I64(const 基础信息节点类* 宿主, const 词性节点类* 特征类型, I64& 输出值) const
{
    auto* 特征节点 = 特征服务_.查找子特征_按类型(宿主, 特征类型);
    return 特征节点 ? 读取特征_I64(特征节点, 输出值) : false;
}

const VecIU64* 世界树类::读取特征VecU(const 特征节点类* 节点) const
{
    const auto 值 = 读取特征快照(节点);
    if (const auto* 句柄 = std::get_if<VecU句柄>(&值)) {
        return 值池_.取VecU只读指针(*句柄);
    }
    return nullptr;
}

存在比较结果 世界树类::比较存在(
    const 存在节点类* 左存在,
    const 存在节点类* 右存在,
    const 集合比较参数& 参数) const
{
    if (!左存在 || !右存在) return {};
    const auto 左特征集合 = 特征服务_.获取子特征(左存在);
    const auto 右特征集合 = 特征服务_.获取子特征(右存在);
    return 特征服务_.比较特征集合(左特征集合, 右特征集合, 参数);
}

std::vector<二次特征节点类*> 世界树类::刷新存在比较二次特征(
    存在节点类* 左存在,
    存在节点类* 右存在,
    const 集合比较参数& 参数)
{
    if (!左存在 || !右存在 || 左存在 == 右存在) return {};

    auto* 左场景 = 私有_取存在所在场景(基础信息链_, 左存在);
    auto* 右场景 = 私有_取存在所在场景(基础信息链_, 右存在);
    if (!左场景 && !右场景) return {};
    if (左场景 && 右场景 && 左场景 != 右场景) return {};

    auto* 所属场景 = 左场景 ? 左场景 : 右场景;
    const auto 比较结果 = 比较存在(左存在, 右存在, 参数);
    return 二次特征生成服务_->由存在比较结果生成二次特征(
        所属场景,
        左存在,
        右存在,
        比较结果);
}

场景节点类* 世界树类::取或创建默认世界(const std::string& 名称, 枚举_世界类型 世界类型)
{
    const auto* 名称词 = 语素集.添加词性词(名称, "专有名词");
    return 取或创建子场景_按名称(nullptr, 名称词, 世界类型);
}

场景节点类* 世界树类::取或创建自我现实场景()
{
    if (!现实世界) 初始化默认世界();

    const auto* 场景名称 = 语素集.添加词性词("自我初始场景", "专有名词");
    自我所在场景 = 取或创建子场景_按名称(现实世界, 场景名称, 枚举_世界类型::现实世界);
    return 自我所在场景;
}

存在节点类* 世界树类::取或创建自我存在()
{
    auto* 当前场景 = 自我所在场景 ? 自我所在场景 : 取或创建自我现实场景();
    const auto* 名称词 = 语素集.添加词性词("自我", "专有名词");
    const auto* 类型词 = 语素集.添加词性词("自我类型", "名词");
    自我指针 = 存在服务_.取或创建子存在_按名称类型(当前场景, 名称词, 类型词);
    return 自我指针;
}

场景节点类* 世界树类::取存在内部世界(const 存在节点类* 宿主存在) const noexcept
{
    const auto* 主信息 = 存在服务_.取存在主信息(宿主存在);
    return (主信息 && 主信息->内部世界.指针) ? 主信息->内部世界.指针 : nullptr;
}

场景节点类* 世界树类::取或创建存在内部世界(存在节点类* 宿主存在, const 词性节点类* 场景名称)
{
    if (!宿主存在) return nullptr;

    if (auto* 现有 = 取存在内部世界(宿主存在)) {
        return 现有;
    }

    const auto* 名称词 = 场景名称 ? 场景名称 : 语素集.添加词性词("内部世界", "专有名词");
    auto* 内部世界 = 取或创建子场景_按名称(宿主存在, 名称词, 枚举_世界类型::内部世界);
    if (!内部世界) return nullptr;

    场景服务_.绑定宿主(内部世界, 宿主存在);
    存在服务_.绑定内部世界(宿主存在, 内部世界);
    return 内部世界;
}

bool 世界树类::写入存在观测位置(存在节点类* 节点, const Vector3D& 位置_mm)
{
    if (!节点) return false;
    if (!存在服务_.写入观测位置(节点, 位置_mm)) {
        return false;
    }

    auto* 父节点 = static_cast<基础信息节点类*>(节点->父);
    auto* 场景节点 = 父节点 && 基础信息链_.取主信息<场景节点主信息类>(父节点)
        ? static_cast<场景节点类*>(父节点)
        : nullptr;
    if (场景节点) {
        (void)二次特征生成服务_->刷新场景存在型基础二次特征(场景节点);
    }
    return true;
}

I64 世界树类::默认特征差异度(const 特征值& 左值, const 特征值& 右值) const
{
    if (左值.index() != 右值.index()) return 10000;

    if (const auto* 左标量 = std::get_if<I64>(&左值)) {
        const auto* 右标量 = std::get_if<I64>(&右值);
        return 右标量 ? 私有_绝对值饱和(私有_饱和减(*左标量, *右标量)) : 10000;
    }

    if (const auto* 左句柄 = std::get_if<VecU句柄>(&左值)) {
        const auto* 右句柄 = std::get_if<VecU句柄>(&右值);
        if (!右句柄) return 10000;
        const auto* 左Vec = 值池_.取VecU只读指针(*左句柄);
        const auto* 右Vec = 值池_.取VecU只读指针(*右句柄);
        if (!左Vec || !右Vec) return 10000;
        return 私有_VecU差异率Q10000(*左Vec, *右Vec);
    }

    if (const auto* 左指针 = std::get_if<指针句柄>(&左值)) {
        const auto* 右指针 = std::get_if<指针句柄>(&右值);
        return (右指针 && (*左指针 == *右指针)) ? 0 : 10000;
    }

    return 0;
}
