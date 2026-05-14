module;

#include <cstddef>
#include <vector>

export module 任务模块.管理界面线程;

export import 任务模块.管理线程协议;

import 自我类;

export namespace 任务管理界面线程 {

bool 启动任务管理界面线程() noexcept;
void 停止任务管理界面线程() noexcept;

bool 提交任务治理请求(
    自我类& 自我对象,
    const 任务管理线程协议::结构_任务治理请求& 请求,
    任务管理线程协议::结构_任务界面承接结果* 输出 = nullptr) noexcept;

bool 提交任务状态推进请求(
    const 任务管理线程协议::结构_任务状态推进请求& 请求) noexcept;

bool 提交任务权重固化请求(
    const 任务管理线程协议::结构_任务权重固化请求& 请求) noexcept;

bool 提交任务价值结算回执(
    const 任务管理线程协议::结构_任务价值结算回执& 回执) noexcept;

bool 提取任务管理上行消息(
    std::vector<结构_任务管理上行消息>* 输出,
    std::size_t 最大数量 = 0) noexcept;

bool 读取任务管理界面线程快照(
    任务管理线程协议::结构_任务界面线程快照* 输出) noexcept;

} // namespace 任务管理界面线程
