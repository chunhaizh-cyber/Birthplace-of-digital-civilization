#pragma once

// 文件头部规则注释模块：
// 1. 本文件只提供日志输出接入口和关闭日志时的编译期 no-op 宏，不承载业务状态。
// 2. 日志文本不得作为机器判断、需求 / 任务 / 方法 / 因果闭环或验收结论来源。
// 3. 分类日志关闭时，应尽量在调用点切断字符串构造成本；逻辑错误检测本身不由本文件关闭。

#include "预处理开关变量.h"

#include <exception>
#include <string>

void 初始化项目日志() noexcept;
void 关闭项目日志() noexcept;

void 项目运行日志(const std::string& 文本) noexcept;
void 项目运行警告日志(const std::string& 文本) noexcept;
void 项目运行错误日志(const std::string& 文本) noexcept;
void 项目弹窗错误提示(const std::string& 标题, const std::string& 文本) noexcept;
void 项目提示不允许空指(
    const char* 上下文,
    const char* 表达式,
    const char* 文件,
    int 行,
    const char* 函数) noexcept;
void 项目自检无上级需求日志(const std::string& 文本) noexcept;

void 项目记录异常日志(const std::exception& 异常, const std::string& 上下文) noexcept;
void 项目致命日志(const std::string& 文本) noexcept;

#if !鱼巢_开关_启用运行日志输出 || !鱼巢_开关_启用登记管理日志输出
#define 项目运行日志(...) ((void)0)
#define 项目运行警告日志(...) ((void)0)
#endif

#if !鱼巢_开关_启用逻辑错误日志输出
#define 项目运行错误日志(...) ((void)0)
#define 项目弹窗错误提示(...) ((void)0)
#define 项目记录异常日志(...) ((void)0)
#define 项目致命日志(...) ((void)0)
#endif

#if !鱼巢_开关_启用逻辑错误排查日志输出
#define 项目提示不允许空指(...) ((void)0)
#endif

#if !鱼巢_开关_启用调试日志输出 || !鱼巢_开关_启用自检日志入口
#define 项目自检无上级需求日志(...) ((void)0)
#endif
