#pragma once

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
