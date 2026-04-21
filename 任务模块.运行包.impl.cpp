module;

#include <string>

module 任务模块.运行包;

namespace {

void 私有_写错误(std::string* 错误, const char* 文本) noexcept
{
    if (错误) {
        *错误 = 文本 ? 文本 : "";
    }
}

bool 私有_校验封口时间(
    const 任务运行包::结构_线程运行包头& 包头,
    std::string* 错误) noexcept
{
    if (包头.包状态 == 任务运行包::枚举_包状态::已封口 && 包头.封口时间 == 0) {
        私有_写错误(错误, "包头已封口但缺少封口时间");
        return false;
    }
    return true;
}

template<class T包>
bool 私有_封口包(
    T包& 包,
    const 任务运行包::时间戳 封口时间,
    bool(*校验函数)(const T包&, std::string*) noexcept,
    std::string* 错误) noexcept
{
    if (包.包头.包状态 == 任务运行包::枚举_包状态::已封口) {
        return 校验函数(包, 错误);
    }
    if (!校验函数(包, 错误)) {
        return false;
    }
    包.包头.包状态 = 任务运行包::枚举_包状态::已封口;
    包.包头.封口时间 = 封口时间 != 0 ? 封口时间 : 包.包头.创建时间;
    return 校验函数(包, 错误);
}

} // namespace

namespace 任务运行包 {

bool 包已封口(const 结构_线程运行包头& 包头) noexcept
{
    return 包头.包状态 == 枚举_包状态::已封口;
}

bool 校验包头(const 结构_线程运行包头& 包头, std::string* 错误) noexcept
{
    if (!私有_校验封口时间(包头, 错误)) {
        return false;
    }
    if (包头.包状态 != 枚举_包状态::草稿 && 包头.包状态 != 枚举_包状态::已封口) {
        私有_写错误(错误, "包头状态非法");
        return false;
    }
    return true;
}

bool 校验任务包(const 结构_一步治理任务包& 包, std::string* 错误) noexcept
{
    if (!校验包头(包.包头, 错误)) {
        return false;
    }
    if (包.任务语义键.empty()) {
        私有_写错误(错误, "任务包缺少任务语义键");
        return false;
    }
    if (包.授权约束.动作白名单.empty()) {
        私有_写错误(错误, "任务包缺少动作白名单");
        return false;
    }
    if (包.授权约束.并发上限 < 0 || 包.安全监测等级 < 0) {
        私有_写错误(错误, "任务包并发或安全等级非法");
        return false;
    }
    if (包.选定方法.has_value() && 包.候选方法集.empty()) {
        私有_写错误(错误, "任务包存在选定方法但缺少候选方法集");
        return false;
    }
    if (包.已显式消费高阶排序绑定 && !包.选定方法.has_value()) {
        私有_写错误(错误, "任务包已消费高阶排序绑定但缺少选定方法");
        return false;
    }
    if (包.已显式消费高阶排序绑定 && 包.高阶排序胜出方法主键 == 0) {
        私有_写错误(错误, "任务包已消费高阶排序绑定但缺少胜出方法主键");
        return false;
    }
    if (!包.控制请求主键.empty() && 包.控制意图值 == 枚举_任务管理控制意图值::未定义) {
        私有_写错误(错误, "任务包携带控制请求但缺少控制意图值");
        return false;
    }
    return true;
}

bool 校验结算包(const 结构_回流结算包& 包, std::string* 错误) noexcept
{
    if (!校验包头(包.包头, 错误)) {
        return false;
    }
    if (包.任务包主键 == 0) {
        私有_写错误(错误, "结算包缺少来源任务包主键");
        return false;
    }
    for (const auto& 候选 : 包.学习候选集) {
        if (候选.已验证) {
            私有_写错误(错误, "结算包中的学习候选默认不得已验证");
            return false;
        }
    }
    return true;
}

bool 封口任务包(结构_一步治理任务包& 包, 时间戳 封口时间, std::string* 错误) noexcept
{
    return 私有_封口包(包, 封口时间, 校验任务包, 错误);
}

bool 封口结算包(结构_回流结算包& 包, 时间戳 封口时间, std::string* 错误) noexcept
{
    return 私有_封口包(包, 封口时间, 校验结算包, 错误);
}

} // namespace 任务运行包
