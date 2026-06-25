// 文件头部规则注释模块：
// 1. 修改本文件前先阅读本模块；双目相机适配器只把外设观测转换为结构化观察材料。
// 2. 评分、比例和坐标等纯数值工具优先复用全局共享函数，不在本文件保留重复包装。
// 3. 本模块只供人读，不参与机器判断；业务状态仍必须由项目通用结构承载。

#include "基础数据类型.h"

#include <exception>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "双目相机本能适配器.h"

import D455相机模块;
import 全局共享函数类;

namespace {
    HMODULE g_RealSense运行时模块 = nullptr;
    std::mutex g_互斥;
    std::unique_ptr<D455_相机实现> g_相机;
    bool g_相机已打开 = false;
    constexpr std::int64_t 观察确认补偿状态_部分确认 = 6;
    constexpr std::int64_t 观察确认补偿状态_待验证 = 8;
    constexpr std::int64_t 观察确认补偿状态_条件不足 = 7;
    constexpr std::int64_t 缺失原因_深度空洞 = 1;
    constexpr std::int64_t 缺失原因_轮廓断裂 = 2;
    constexpr std::int64_t 缺失原因_对齐缺失 = 4;
    constexpr std::int64_t 缺失原因_有效点不足 = 8;
    constexpr std::int64_t 缺失原因_边界不稳定 = 16;
    constexpr std::int64_t 缺失原因_补全依赖过高 = 32;
    constexpr std::int64_t 缺失原因_融合低稳定 = 64;
    constexpr std::int64_t 补观察建议_重新取得观察特征帧 = 1;
    constexpr std::int64_t 补观察建议_调整双目相机工作模式 = 2;
    constexpr std::int64_t 补观察建议_缩小ROI重新观察 = 4;
    constexpr std::int64_t 补观察建议_融合稳定观察帧深度 = 8;
    constexpr std::int64_t 补观察建议_内部世界细分 = 16;
    constexpr std::int64_t 补观察建议_继续观察未解释区域 = 32;
    constexpr std::int64_t 诊断区域图层_深度空洞 = 1;
    constexpr std::int64_t 诊断区域图层_补全深度 = 2;
    constexpr std::int64_t 诊断区域图层_未解释 = 3;
    constexpr std::int64_t 诊断区域图层_轮廓风险 = 4;
    constexpr std::int64_t 诊断区域掩码_深度有效性 = 1;
    constexpr std::int64_t 诊断区域掩码_候选投影 = 2;
    constexpr std::size_t 最大深度空洞诊断区域数量 = 96;
    constexpr std::size_t 最大诊断区域数量 = 160;

    // 功能：按函数名执行对应处理。
    std::string 路径UTF8(const std::filesystem::path& 路径)
    {
        const auto 文本 = 路径.u8string();
        return std::string(
            reinterpret_cast<const char*>(文本.data()),
            文本.size());
    }

    // 功能：按函数名执行对应处理。
    std::filesystem::path 模块目录() noexcept
    {
        wchar_t 缓冲区[MAX_PATH]{};
        const DWORD 长度 = GetModuleFileNameW(nullptr, 缓冲区, MAX_PATH);
        if (长度 == 0 || 长度 >= MAX_PATH) {
            return {};
        }
        return std::filesystem::path(缓冲区).parent_path();
    }

    // 功能：确保目标结构、状态或前置条件存在并可用。
    bool 确保RealSense运行时(std::string& 错误消息) noexcept
    {
        if (g_RealSense运行时模块) {
            return true;
        }

        try {
            const auto 根目录 = 模块目录();
            const auto 主DLL = (根目录 / L"realsense2.dll").lexically_normal();
            const auto LZ4DLL = (根目录 / L"lz4.dll").lexically_normal();

            if (!std::filesystem::exists(主DLL)) {
                错误消息 = "RealSense 运行时缺失: " + 路径UTF8(主DLL);
                return false;
            }
            if (!std::filesystem::exists(LZ4DLL)) {
                错误消息 = "RealSense 依赖缺失: " + 路径UTF8(LZ4DLL);
                return false;
            }

            g_RealSense运行时模块 = LoadLibraryExW(
                主DLL.c_str(),
                nullptr,
                LOAD_WITH_ALTERED_SEARCH_PATH);
            if (!g_RealSense运行时模块) {
                错误消息 = "RealSense 运行时加载失败 | Win32="
                    + std::to_string(GetLastError())
                    + " | 路径="
                    + 路径UTF8(主DLL);
                return false;
            }
            return true;
        }
        catch (const std::exception& e) {
            错误消息 = std::string("RealSense 运行时检查异常: ") + e.what();
            return false;
        }
        catch (...) {
            错误消息 = "RealSense 运行时检查发生未知异常";
            return false;
        }
    }

    // 功能：按函数名执行对应处理。
    bool 帧像素有有效空间点(const 结构体_原始场景帧& 帧, std::size_t 索引) noexcept
    {
        if (索引 >= 帧.深度.size()) {
            return false;
        }
        const bool 深度有效 = 帧.有效深度掩膜()
            ? 帧.深度有效[索引] != 0
            : 帧.深度[索引] > 0.0;
        if (!深度有效 || 帧.深度[索引] <= 0.0) {
            return false;
        }
        if (!帧.有效点云() || 索引 >= 帧.点云.size()) {
            return false;
        }
        return 帧.点云[索引].z > 0.0;
    }

    // 功能：按函数名执行对应处理。
    bool 空间邻近(const 结构体_原始场景帧& 帧, std::size_t a, std::size_t b) noexcept
    {
        const auto& A = 帧.点云[a];
        const auto& B = 帧.点云[b];
        const double 基准深度 = std::min(A.z, B.z);
        const double 深度阈值 = std::clamp(基准深度 * 0.08, 60.0, 300.0);
        if (std::abs(A.z - B.z) > 深度阈值) {
            return false;
        }
        const double 空间阈值 = std::clamp(基准深度 * 0.10, 80.0, 420.0);
        const double dx = A.x - B.x;
        const double dy = A.y - B.y;
        const double dz = A.z - B.z;
        return dx * dx + dy * dy + dz * dz <= 空间阈值 * 空间阈值;
    }

    // 功能：按函数名执行对应处理。
    bool 帧像素深度有效(const 结构体_原始场景帧& 帧, std::size_t 索引) noexcept
    {
        if (索引 >= 帧.深度.size()) {
            return false;
        }
        const bool 掩膜有效 = 帧.有效深度掩膜()
            ? 帧.深度有效[索引] != 0
            : 帧.深度[索引] > 0.0;
        return 掩膜有效 && 帧.深度[索引] > 0.0;
    }

    // 功能：按函数名执行对应处理。
    bool 深度向量像素有效(
        const std::vector<double>& 深度,
        const std::vector<std::uint8_t>& 有效掩膜,
        std::size_t 索引) noexcept
    {
        if (索引 >= 深度.size()) {
            return false;
        }
        const bool 掩膜有效 = 有效掩膜.size() == 深度.size()
            ? 有效掩膜[索引] != 0
            : 深度[索引] > 0.0;
        return 掩膜有效 && 深度[索引] > 0.0;
    }

    // 功能：按函数名执行对应处理。
    void 填充深度毫米结构(
        const std::vector<double>& 深度,
        const std::vector<std::uint8_t>& 有效掩膜,
        std::vector<std::int64_t>& 输出) noexcept
    {
        输出.clear();
        输出.reserve(深度.size());
        for (std::size_t i = 0; i < 深度.size(); ++i) {
            输出.push_back(深度向量像素有效(深度, 有效掩膜, i) ? 转换毫米(深度[i]) : 0);
        }
    }

    // 功能：按函数名执行对应处理。
    void 建立深度三口径诊断(双目相机本能适配器::调用结果& 结果) noexcept
    {
        const std::size_t 总数 = 结果.原始深度毫米.size();
        if (总数 == 0) {
            return;
        }

        const bool 有滤波 = 结果.滤波深度毫米.size() == 总数;
        const bool 有补全 = 结果.补全深度毫米.size() == 总数;
        结果.深度来源.assign(总数, 0);
        结果.深度稳定性.assign(总数, 0);
        结果.深度邻域一致性.assign(总数, 0);

        auto 像素深度 = [&](std::size_t i) noexcept -> std::int64_t {
            switch (结果.深度来源[i]) {
            case 1:
                return 结果.原始深度毫米[i];
            case 2:
                return 有滤波 ? 结果.滤波深度毫米[i] : 0;
            case 3:
                return 有补全 ? 结果.补全深度毫米[i] : 0;
            default:
                return 0;
            }
        };

        std::int64_t 稳定累计 = 0;
        std::int64_t 邻域累计 = 0;
        std::size_t 有来源数量 = 0;
        std::size_t 有邻域数量 = 0;

        for (std::size_t i = 0; i < 总数; ++i) {
            const bool 原始有效 = 结果.原始深度毫米[i] > 0;
            const bool 滤波有效 = 有滤波 && 结果.滤波深度毫米[i] > 0;
            const bool 补全有效 = 有补全 && 结果.补全深度毫米[i] > 0;

            if (原始有效) {
                结果.深度来源[i] = 1;
                ++结果.原始深度来源像素数量;
            } else if (滤波有效) {
                结果.深度来源[i] = 2;
                ++结果.滤波深度来源像素数量;
            } else if (补全有效) {
                结果.深度来源[i] = 3;
                ++结果.补全深度来源像素数量;
                ++结果.补全深度低置信像素数量;
            } else {
                ++结果.无有效深度来源像素数量;
                continue;
            }

            std::int64_t 稳定性 = 0;
            if (原始有效 && 滤波有效) {
                稳定性 = 深度差异评分(绝对差I64(结果.原始深度毫米[i], 结果.滤波深度毫米[i]));
            } else if (原始有效) {
                稳定性 = 8500;
            } else if (滤波有效) {
                稳定性 = 6500;
            } else if (补全有效) {
                稳定性 = 3000;
            }
            if (结果.深度来源[i] == 3) {
                稳定性 = std::min<std::int64_t>(稳定性, 3000);
            }
            结果.深度稳定性[i] = 稳定性;
            稳定累计 += 稳定性;
            ++有来源数量;
        }

        const int 宽 = 结果.宽度;
        const int 高 = 结果.高度;
        if (宽 > 0 && 高 > 0 && 总数 == static_cast<std::size_t>(宽) * static_cast<std::size_t>(高)) {
            const int dx[4] = { -1, 1, 0, 0 };
            const int dy[4] = { 0, 0, -1, 1 };
            for (int y = 0; y < 高; ++y) {
                for (int x = 0; x < 宽; ++x) {
                    const std::size_t i = static_cast<std::size_t>(y) * static_cast<std::size_t>(宽)
                        + static_cast<std::size_t>(x);
                    const std::int64_t 当前深度 = 像素深度(i);
                    if (当前深度 <= 0) {
                        continue;
                    }
                    std::int64_t 差异累计 = 0;
                    std::size_t 邻居数量 = 0;
                    for (int n = 0; n < 4; ++n) {
                        const int nx = x + dx[n];
                        const int ny = y + dy[n];
                        if (nx < 0 || ny < 0 || nx >= 宽 || ny >= 高) {
                            continue;
                        }
                        const std::size_t ni = static_cast<std::size_t>(ny) * static_cast<std::size_t>(宽)
                            + static_cast<std::size_t>(nx);
                        const std::int64_t 邻居深度 = 像素深度(ni);
                        if (邻居深度 <= 0) {
                            continue;
                        }
                        差异累计 += 绝对差I64(当前深度, 邻居深度);
                        ++邻居数量;
                    }
                    if (邻居数量 == 0) {
                        continue;
                    }
                    std::int64_t 一致性 = 深度差异评分(差异累计 / static_cast<std::int64_t>(邻居数量));
                    if (结果.深度来源[i] == 3) {
                        一致性 = std::min<std::int64_t>(一致性, 3000);
                    }
                    结果.深度邻域一致性[i] = 一致性;
                    邻域累计 += 一致性;
                    ++有邻域数量;
                }
            }
        }

        结果.深度来源结构状态 = 结果.深度来源.size() == 总数 ? 1 : 0;
        结果.深度稳定性结构状态 = 结果.深度稳定性.size() == 总数 ? 1 : 0;
        结果.深度邻域一致性结构状态 = 结果.深度邻域一致性.size() == 总数 ? 1 : 0;
        结果.深度稳定性平均值 = 有来源数量 == 0
            ? 0
            : 稳定累计 / static_cast<std::int64_t>(有来源数量);
        结果.深度邻域一致性平均值 = 有邻域数量 == 0
            ? 0
            : 邻域累计 / static_cast<std::int64_t>(有邻域数量);
    }

    // 功能：按函数名执行对应处理。
    void 建立静态多帧融合诊断(
        双目相机本能适配器::调用结果& 结果,
        const std::vector<结构体_原始场景帧>& 帧组) noexcept
    {
        const std::size_t 总数 = 结果.原始深度毫米.size();
        if (总数 == 0 || 帧组.size() < 2) {
            结果.观察帧组帧数 = 转换像素计数(帧组.size());
            return;
        }

        std::vector<const 结构体_原始场景帧*> 可用帧{};
        可用帧.reserve(帧组.size());
        for (const auto& 帧 : 帧组) {
            if (帧.宽度 == 结果.宽度
                && 帧.高度 == 结果.高度
                && 帧.原始深度.size() == 总数) {
                可用帧.push_back(&帧);
            }
        }
        结果.观察帧组帧数 = 转换像素计数(可用帧.size());
        if (可用帧.size() < 2) {
            return;
        }

        结果.融合深度毫米.assign(总数, 0);
        结果.融合深度有效性.assign(总数, 0);
        结果.多帧深度稳定性.assign(总数, 0);
        结果.每像素深度方差.assign(总数, 0);

        std::vector<std::int64_t> 样本{};
        样本.reserve(可用帧.size());
        std::int64_t 方差累计 = 0;
        std::int64_t 稳定累计 = 0;
        std::size_t 融合有效数 = 0;
        std::size_t 补偿空洞数 = 0;
        std::size_t 低稳定数 = 0;

        for (std::size_t i = 0; i < 总数; ++i) {
            样本.clear();
            for (const auto* 帧 : 可用帧) {
                const bool 有掩膜 = 帧->原始深度有效.size() == 帧->原始深度.size();
                const bool 有效 = 有掩膜 ? 帧->原始深度有效[i] != 0 : 帧->原始深度[i] > 0.0;
                if (有效 && 帧->原始深度[i] > 0.0) {
                    样本.push_back(转换毫米(帧->原始深度[i]));
                }
            }
            if (样本.size() < 2) {
                continue;
            }

            std::sort(样本.begin(), 样本.end());
            const std::int64_t 中位数 = 样本[样本.size() / 2];
            std::int64_t 绝对偏差累计 = 0;
            std::int64_t 平方偏差累计 = 0;
            for (const std::int64_t 样本值 : 样本) {
                const std::int64_t 偏差 = 绝对差I64(样本值, 中位数);
                绝对偏差累计 += 偏差;
                if (偏差 > 0
                    && 偏差 > std::numeric_limits<std::int64_t>::max() / std::max<std::int64_t>(1, 偏差)) {
                    平方偏差累计 = std::numeric_limits<std::int64_t>::max();
                } else if (平方偏差累计 != std::numeric_limits<std::int64_t>::max()) {
                    const std::int64_t 平方 = 偏差 * 偏差;
                    if (平方 > std::numeric_limits<std::int64_t>::max() - 平方偏差累计) {
                        平方偏差累计 = std::numeric_limits<std::int64_t>::max();
                    } else {
                        平方偏差累计 += 平方;
                    }
                }
            }

            const auto 样本数 = static_cast<std::int64_t>(样本.size());
            const std::int64_t 平均绝对偏差 = 绝对偏差累计 / 样本数;
            const std::int64_t 方差 = 平方偏差累计 == std::numeric_limits<std::int64_t>::max()
                ? std::numeric_limits<std::int64_t>::max()
                : 平方偏差累计 / 样本数;
            const std::int64_t 稳定性 = 深度差异评分(平均绝对偏差);

            结果.融合深度毫米[i] = 中位数;
            结果.融合深度有效性[i] = 1;
            结果.多帧深度稳定性[i] = 稳定性;
            结果.每像素深度方差[i] = 方差;
            ++融合有效数;
            if (结果.原始深度毫米[i] <= 0) {
                ++补偿空洞数;
            }
            if (稳定性 < 7000) {
                ++低稳定数;
            }
            稳定累计 += 稳定性;
            if (方差累计 != std::numeric_limits<std::int64_t>::max()) {
                if (方差 == std::numeric_limits<std::int64_t>::max()
                    || 方差 > std::numeric_limits<std::int64_t>::max() - 方差累计) {
                    方差累计 = std::numeric_limits<std::int64_t>::max();
                } else {
                    方差累计 += 方差;
                }
            }
        }

        结果.观察帧组状态 = 1;
        结果.融合依据帧集合状态 = 1;
        结果.融合深度毫米结构状态 = 结果.融合深度毫米.size() == 总数 ? 1 : 0;
        结果.融合深度有效性结构状态 = 结果.融合深度有效性.size() == 总数 ? 1 : 0;
        结果.多帧深度稳定性结构状态 = 结果.多帧深度稳定性.size() == 总数 ? 1 : 0;
        结果.每像素深度方差结构状态 = 结果.每像素深度方差.size() == 总数 ? 1 : 0;
        结果.融合深度有效像素数量 = 转换像素计数(融合有效数);
        结果.融合深度有效率 = 比例万分比(融合有效数, 总数);
        结果.融合补偿深度空洞像素数量 = 转换像素计数(补偿空洞数);
        结果.融合低稳定像素数量 = 转换像素计数(低稳定数);
        结果.融合深度稳定性平均值 = 融合有效数 == 0
            ? 0
            : 稳定累计 / static_cast<std::int64_t>(融合有效数);
        结果.融合深度平均方差 = 融合有效数 == 0
            ? 0
            : (方差累计 == std::numeric_limits<std::int64_t>::max()
                ? std::numeric_limits<std::int64_t>::max()
                : 方差累计 / static_cast<std::int64_t>(融合有效数));
    }

    // 功能：按函数名执行对应处理。
    std::vector<双目相机本能适配器::诊断区域摘要> 收集深度空洞诊断区域(
        const 结构体_原始场景帧& 帧,
        std::size_t& 输出总数量)
    {
        输出总数量 = 0;
        if (!帧.有效深度()) {
            return {};
        }

        const int 宽 = 帧.宽度;
        const int 高 = 帧.高度;
        const std::size_t 总数 = 帧.深度.size();
        std::vector<std::uint8_t> 已访问(总数, 0);
        std::vector<std::size_t> 栈{};
        std::vector<双目相机本能适配器::诊断区域摘要> 全部区域{};

        const auto 推入 = [&](int x, int y) noexcept {
            if (x < 0 || y < 0 || x >= 宽 || y >= 高) {
                return;
            }
            const auto idx = static_cast<std::size_t>(y) * static_cast<std::size_t>(宽)
                + static_cast<std::size_t>(x);
            if (已访问[idx] || 帧像素深度有效(帧, idx)) {
                return;
            }
            已访问[idx] = 1;
            栈.push_back(idx);
        };

        for (std::size_t i = 0; i < 总数; ++i) {
            if (已访问[i] || 帧像素深度有效(帧, i)) {
                continue;
            }
            ++输出总数量;
            已访问[i] = 1;
            栈.clear();
            栈.push_back(i);
            int 最小X = static_cast<int>(i % static_cast<std::size_t>(宽));
            int 最大X = 最小X;
            int 最小Y = static_cast<int>(i / static_cast<std::size_t>(宽));
            int 最大Y = 最小Y;
            std::size_t 像素数量 = 0;
            while (!栈.empty()) {
                const std::size_t 当前 = 栈.back();
                栈.pop_back();
                const int x = static_cast<int>(当前 % static_cast<std::size_t>(宽));
                const int y = static_cast<int>(当前 / static_cast<std::size_t>(宽));
                最小X = std::min(最小X, x);
                最大X = std::max(最大X, x);
                最小Y = std::min(最小Y, y);
                最大Y = std::max(最大Y, y);
                ++像素数量;
                推入(x - 1, y);
                推入(x + 1, y);
                推入(x, y - 1);
                推入(x, y + 1);
            }

            双目相机本能适配器::诊断区域摘要 区域{};
            区域.区域编号 = 转换像素计数(输出总数量);
            区域.图层类型 = 诊断区域图层_深度空洞;
            区域.来源候选编号 = -1;
            区域.投影最小X = 最小X;
            区域.投影最大X = 最大X;
            区域.投影最小Y = 最小Y;
            区域.投影最大Y = 最大Y;
            区域.像素数量 = 转换像素计数(像素数量);
            区域.置信度 = 10000;
            区域.掩码状态 = 诊断区域掩码_深度有效性;
            全部区域.push_back(区域);
        }
        std::sort(
            全部区域.begin(),
            全部区域.end(),
            [](const auto& a, const auto& b) noexcept {
                if (a.像素数量 != b.像素数量) {
                    return a.像素数量 > b.像素数量;
                }
                return a.区域编号 < b.区域编号;
            });
        if (全部区域.size() > 最大深度空洞诊断区域数量) {
            全部区域.resize(最大深度空洞诊断区域数量);
        }
        std::sort(
            全部区域.begin(),
            全部区域.end(),
            [](const auto& a, const auto& b) noexcept {
                return a.区域编号 < b.区域编号;
            });
        return 全部区域;
    }

    // 功能：按函数名执行对应处理。
    bool 候选诊断投影有效(
        const 双目相机本能适配器::空间候选摘要& 候选,
        int 宽,
        int 高) noexcept
    {
        return 宽 > 0
            && 高 > 0
            && 候选.投影最大X >= 候选.投影最小X
            && 候选.投影最大Y >= 候选.投影最小Y;
    }

    // 功能：按函数名执行对应处理。
    void 追加候选诊断区域(
        std::vector<双目相机本能适配器::诊断区域摘要>& 区域列表,
        const 双目相机本能适配器::空间候选摘要& 候选,
        std::int64_t 图层类型,
        std::int64_t 像素数量,
        std::int64_t 置信度,
        std::int64_t& 下一区域编号,
        int 宽,
        int 高) noexcept
    {
        if (区域列表.size() >= 最大诊断区域数量
            || !候选诊断投影有效(候选, 宽, 高)) {
            return;
        }

        const auto 左 = std::max<std::int64_t>(0, std::min(候选.投影最小X, 候选.投影最大X));
        const auto 右 = std::min<std::int64_t>(static_cast<std::int64_t>(宽 - 1), std::max(候选.投影最小X, 候选.投影最大X));
        const auto 上 = std::max<std::int64_t>(0, std::min(候选.投影最小Y, 候选.投影最大Y));
        const auto 下 = std::min<std::int64_t>(static_cast<std::int64_t>(高 - 1), std::max(候选.投影最小Y, 候选.投影最大Y));
        if (右 < 左 || 下 < 上) {
            return;
        }

        双目相机本能适配器::诊断区域摘要 区域{};
        区域.区域编号 = 下一区域编号++;
        区域.图层类型 = 图层类型;
        区域.来源候选编号 = 候选.候选编号;
        区域.投影最小X = 左;
        区域.投影最大X = 右;
        区域.投影最小Y = 上;
        区域.投影最大Y = 下;
        区域.像素数量 = std::max<std::int64_t>(1, 像素数量);
        区域.置信度 = std::max<std::int64_t>(0, std::min<std::int64_t>(10000, 置信度));
        区域.掩码状态 = 诊断区域掩码_候选投影;
        区域列表.push_back(区域);
    }

    // 功能：按函数名执行对应处理。
    void 追加空间候选诊断区域(
        std::vector<双目相机本能适配器::诊断区域摘要>& 区域列表,
        const std::vector<双目相机本能适配器::空间候选摘要>& 候选列表,
        std::int64_t 起始区域编号,
        int 宽,
        int 高) noexcept
    {
        std::int64_t 下一区域编号 = std::max<std::int64_t>(1, 起始区域编号);
        for (const auto& 候选 : 候选列表) {
            if (候选.补全候选状态 > 0 || 候选.轮廓依赖补全深度 > 0) {
                追加候选诊断区域(
                    区域列表,
                    候选,
                    诊断区域图层_补全深度,
                    std::max<std::int64_t>(候选.像素数量, 候选.空间覆盖像素数),
                    std::min<std::int64_t>(候选.轮廓置信度 > 0 ? 候选.轮廓置信度 : 5000, 5000),
                    下一区域编号,
                    宽,
                    高);
            }
            if (候选.未解释区域状态 > 0 || 候选.未解释像素数 > 0 || 候选.待补观察区域数量 > 0) {
                追加候选诊断区域(
                    区域列表,
                    候选,
                    诊断区域图层_未解释,
                    std::max<std::int64_t>(候选.未解释像素数, 候选.像素数量),
                    候选.像素归属验证状态 > 0 ? 6500 : 3500,
                    下一区域编号,
                    宽,
                    高);
            }
            if (候选.轮廓穿过深度无效区域 > 0
                || 候选.轮廓跨越深度断裂 > 0
                || 候选.轮廓依赖补全深度 > 0
                || 候选.低置信轮廓状态 > 0) {
                追加候选诊断区域(
                    区域列表,
                    候选,
                    诊断区域图层_轮廓风险,
                    std::max<std::int64_t>(候选.轮廓冲突像素数, 候选.像素数量),
                    候选.轮廓置信度,
                    下一区域编号,
                    宽,
                    高);
            }
        }
        std::sort(
            区域列表.begin(),
            区域列表.end(),
            [](const auto& a, const auto& b) noexcept {
                if (a.图层类型 != b.图层类型) {
                    return a.图层类型 < b.图层类型;
                }
                return a.区域编号 < b.区域编号;
            });
    }

    // 功能：按函数名执行对应处理。
    bool 点在范围内(
        const Vector3D& P,
        double minX,
        double maxX,
        double minY,
        double maxY,
        double minZ,
        double maxZ) noexcept
    {
        return P.x >= minX && P.x <= maxX
            && P.y >= minY && P.y <= maxY
            && P.z >= minZ && P.z <= maxZ;
    }

    // 功能：按函数名执行对应处理。
    void 填充像素归属验证摘要(
        const 结构体_原始场景帧& 帧,
        双目相机本能适配器::空间候选摘要& 候选,
        const std::vector<std::size_t>& 候选像素索引,
        int minPX,
        int maxPX,
        int minPY,
        int maxPY,
        double minX,
        double maxX,
        double minY,
        double maxY,
        double minZ,
        double maxZ) noexcept
    {
        const int 宽 = 帧.宽度;
        const int 高 = 帧.高度;
        if (宽 <= 0 || 高 <= 0 || minPX > maxPX || minPY > maxPY || 候选像素索引.empty()) {
            return;
        }

        const std::size_t 候选像素数 = 候选像素索引.size();
        std::size_t 投影像素数 = 0;
        std::size_t 范围内点数 = 0;
        std::size_t 深度一致点数 = 0;
        std::size_t 空间匹配点数 = 0;
        std::size_t 补全支撑点数 = 0;
        for (int y = std::max(0, minPY); y <= std::min(高 - 1, maxPY); ++y) {
            for (int x = std::max(0, minPX); x <= std::min(宽 - 1, maxPX); ++x) {
                ++投影像素数;
            }
        }

        std::size_t 候选有效点数 = 0;
        for (const std::size_t idx : 候选像素索引) {
            if (!帧像素有有效空间点(帧, idx)) {
                continue;
            }
            ++候选有效点数;
            const auto& P = 帧.点云[idx];
            const bool 范围内 = 点在范围内(P, minX, maxX, minY, maxY, minZ, maxZ);
            const bool 深度一致 = P.z >= minZ && P.z <= maxZ;
            if (范围内) {
                ++范围内点数;
            }
            if (深度一致) {
                ++深度一致点数;
            }
            if (范围内 && 深度一致) {
                ++空间匹配点数;
            }
            const bool 补全有效 = 帧.补全深度.size() == 帧.深度.size()
                && 深度向量像素有效(帧.补全深度, 帧.补全深度有效, idx);
            const bool 原始有效 = 帧.原始深度.size() == 帧.深度.size()
                ? 深度向量像素有效(帧.原始深度, 帧.原始深度有效, idx)
                : 帧像素深度有效(帧, idx);
            if (!原始有效 && 补全有效) {
                ++补全支撑点数;
            }
        }

        const std::size_t 空间冲突点数 = 候选有效点数 > 空间匹配点数
            ? 候选有效点数 - 空间匹配点数
            : 0;
        const std::size_t 轮廓冲突点数 = 投影像素数 > 候选像素数
            ? 投影像素数 - 候选像素数
            : 0;

        候选.空间覆盖像素数 = 转换像素计数(候选像素数);
        候选.空间匹配像素数 = 转换像素计数(空间匹配点数);
        候选.空间冲突像素数 = 转换像素计数(空间冲突点数);
        候选.轮廓支持像素数 = 转换像素计数(候选像素数);
        候选.轮廓冲突像素数 = 转换像素计数(轮廓冲突点数);
        候选.深度一致率 = 比例万分比(深度一致点数, 候选有效点数);
        候选.范围内点比例 = 比例万分比(范围内点数, 候选有效点数);
        候选.投影覆盖率 = 比例万分比(空间匹配点数, 候选有效点数);
        候选.轮廓吻合率 = 比例万分比(候选像素数, 投影像素数);
        候选.轮廓闭合度 = 候选.轮廓吻合率;
        候选.轮廓深度支持率 = 候选.深度一致率;
        候选.轮廓空间支持率 = (候选.范围内点比例 + 候选.投影覆盖率) / 2;
        候选.轮廓颜色支持率 = 帧.轮廓观测列表.empty() ? 0 : 候选.轮廓吻合率;
        候选.轮廓跨越深度断裂 = 候选.深度一致率 > 0 && 候选.深度一致率 < 6000 ? 1 : 0;
        候选.轮廓穿过深度无效区域 = 轮廓冲突点数 > 0 ? 1 : 0;
        候选.轮廓依赖补全深度 = 补全支撑点数 > 0 ? 1 : 0;
        候选.轮廓来源类型 = 0;
        if (候选.轮廓颜色支持率 > 0) 候选.轮廓来源类型 |= 1;
        if (候选.轮廓深度支持率 > 0) 候选.轮廓来源类型 |= 2;
        if (候选.轮廓空间支持率 > 0) 候选.轮廓来源类型 |= 4;
        if ((候选.轮廓来源类型 & 6) == 6) 候选.轮廓来源类型 |= 8;
        候选.轮廓置信度 = (
            候选.轮廓颜色支持率 * 20
            + 候选.轮廓深度支持率 * 35
            + 候选.轮廓空间支持率 * 45) / 100;
        if (候选.轮廓依赖补全深度) {
            候选.轮廓置信度 = std::min<std::int64_t>(候选.轮廓置信度, 3500);
        }
        候选.遮挡像素数 = 0;
        候选.未解释像素数 = 转换像素计数(轮廓冲突点数);
        候选.像素归属验证状态 = 空间匹配点数 > 0 ? 1 : 0;
        std::int64_t 缺失原因 = 0;
        if (候选.未解释像素数 > 0 || 候选.轮廓穿过深度无效区域) {
            缺失原因 |= 缺失原因_深度空洞;
        }
        if (候选.轮廓闭合度 > 0 && 候选.轮廓闭合度 < 7000) {
            缺失原因 |= 缺失原因_轮廓断裂;
        }
        if (!帧.深度已对齐到彩色) {
            缺失原因 |= 缺失原因_对齐缺失;
        }
        if (候选有效点数 < 16 || 候选.有效点比例 < 3000) {
            缺失原因 |= 缺失原因_有效点不足;
        }
        if (候选.空间连续性评分 < 5000 || 候选.范围稳定性评分 < 5000) {
            缺失原因 |= 缺失原因_边界不稳定;
        }
        if (候选.轮廓依赖补全深度) {
            缺失原因 |= 缺失原因_补全依赖过高;
        }
        候选.缺失原因 = 缺失原因;
        候选.未解释区域状态 = 候选.未解释像素数 > 0 ? 1 : 0;
        候选.补全候选状态 = (候选.轮廓依赖补全深度 || (缺失原因 & 缺失原因_深度空洞) != 0) ? 1 : 0;
        候选.低置信轮廓状态 = (候选.轮廓置信度 > 0 && 候选.轮廓置信度 < 7000) || 候选.轮廓依赖补全深度 ? 1 : 0;
        候选.待补观察区域数量 = (缺失原因 != 0 || 候选.低置信轮廓状态) ? 1 : 0;
        if ((缺失原因 & 缺失原因_有效点不足) != 0 || 空间匹配点数 == 0) {
            候选.观察确认补偿状态 = 观察确认补偿状态_条件不足;
        } else if (缺失原因 != 0) {
            候选.观察确认补偿状态 = 候选.像素归属验证状态 > 0
                ? 观察确认补偿状态_部分确认
                : 观察确认补偿状态_待验证;
        }
        if ((缺失原因 & 缺失原因_深度空洞) != 0) {
            候选.补观察需求建议 |= 补观察建议_继续观察未解释区域 | 补观察建议_重新取得观察特征帧;
        }
        if ((缺失原因 & 缺失原因_对齐缺失) != 0) {
            候选.补观察需求建议 |= 补观察建议_调整双目相机工作模式;
        }
        if ((缺失原因 & 缺失原因_有效点不足) != 0) {
            候选.补观察需求建议 |= 补观察建议_缩小ROI重新观察;
        }
        if ((缺失原因 & 缺失原因_补全依赖过高) != 0) {
            候选.补观察需求建议 |= 补观察建议_融合稳定观察帧深度;
        }
        if (候选.低置信轮廓状态) {
            候选.补观察需求建议 |= 补观察建议_内部世界细分;
        }
    }

    // 功能：按函数名执行对应处理。
    std::vector<双目相机本能适配器::空间候选摘要> 提取空间候选摘要(
        const 结构体_原始场景帧& 帧)
    {
        std::vector<双目相机本能适配器::空间候选摘要> 输出{};
        if (!帧.有效深度() || !帧.有效点云() || 帧.宽度 <= 0 || 帧.高度 <= 0) {
            return 输出;
        }

        const int 宽 = 帧.宽度;
        const int 高 = 帧.高度;
        const std::size_t 总数 = 帧.深度.size();
        std::size_t 有效总数 = 0;
        for (std::size_t i = 0; i < 总数; ++i) {
            if (帧像素有有效空间点(帧, i)) {
                ++有效总数;
            }
        }
        if (有效总数 == 0) {
            return 输出;
        }

        constexpr std::size_t 最小候选像素数 = 32;
        std::vector<std::uint8_t> 已访问(总数, 0);
        std::vector<std::size_t> 队列{};
        队列.reserve(1024);

        auto 推入邻居 = [&](std::size_t 当前, int nx, int ny) {
            if (nx < 0 || ny < 0 || nx >= 宽 || ny >= 高) {
                return;
            }
            const std::size_t ni = static_cast<std::size_t>(ny) * static_cast<std::size_t>(宽)
                + static_cast<std::size_t>(nx);
            if (已访问[ni] || !帧像素有有效空间点(帧, ni) || !空间邻近(帧, 当前, ni)) {
                return;
            }
            已访问[ni] = 1;
            队列.push_back(ni);
        };

        for (int y = 0; y < 高; ++y) {
            for (int x = 0; x < 宽; ++x) {
                const std::size_t 起点 = static_cast<std::size_t>(y) * static_cast<std::size_t>(宽)
                    + static_cast<std::size_t>(x);
                if (已访问[起点] || !帧像素有有效空间点(帧, 起点)) {
                    continue;
                }

                已访问[起点] = 1;
                队列.clear();
                队列.push_back(起点);
                std::size_t 游标 = 0;

                double sumX = 0.0;
                double sumY = 0.0;
                double sumZ = 0.0;
                double minX = std::numeric_limits<double>::max();
                double minY = std::numeric_limits<double>::max();
                double minZ = std::numeric_limits<double>::max();
                double maxX = std::numeric_limits<double>::lowest();
                double maxY = std::numeric_limits<double>::lowest();
                double maxZ = std::numeric_limits<double>::lowest();
                int minPX = x;
                int maxPX = x;
                int minPY = y;
                int maxPY = y;

                while (游标 < 队列.size()) {
                    const std::size_t 当前 = 队列[游标++];
                    const int cx = static_cast<int>(当前 % static_cast<std::size_t>(宽));
                    const int cy = static_cast<int>(当前 / static_cast<std::size_t>(宽));
                    const auto& P = 帧.点云[当前];
                    sumX += P.x;
                    sumY += P.y;
                    sumZ += P.z;
                    minX = std::min(minX, P.x);
                    minY = std::min(minY, P.y);
                    minZ = std::min(minZ, P.z);
                    maxX = std::max(maxX, P.x);
                    maxY = std::max(maxY, P.y);
                    maxZ = std::max(maxZ, P.z);
                    minPX = std::min(minPX, cx);
                    maxPX = std::max(maxPX, cx);
                    minPY = std::min(minPY, cy);
                    maxPY = std::max(maxPY, cy);

                    推入邻居(当前, cx - 1, cy);
                    推入邻居(当前, cx + 1, cy);
                    推入邻居(当前, cx, cy - 1);
                    推入邻居(当前, cx, cy + 1);
                }

                if (队列.size() < 最小候选像素数) {
                    continue;
                }

                双目相机本能适配器::空间候选摘要 候选{};
                候选.候选编号 = static_cast<std::int64_t>(输出.size());
                候选.像素数量 = 转换像素计数(队列.size());
                候选.有效点比例 = static_cast<std::int64_t>(
                    (static_cast<unsigned long long>(队列.size()) * 10000ULL)
                    / static_cast<unsigned long long>(有效总数));
                const double inv = 1.0 / static_cast<double>(队列.size());
                候选.中心X = 转换毫米(sumX * inv);
                候选.中心Y = 转换毫米(sumY * inv);
                候选.中心Z = 转换毫米(sumZ * inv);
                候选.范围最小X = 转换毫米(minX);
                候选.范围最大X = 转换毫米(maxX);
                候选.范围最小Y = 转换毫米(minY);
                候选.范围最大Y = 转换毫米(maxY);
                候选.范围最小Z = 转换毫米(minZ);
                候选.范围最大Z = 转换毫米(maxZ);
                候选.投影最小X = minPX;
                候选.投影最大X = maxPX;
                候选.投影最小Y = minPY;
                候选.投影最大Y = maxPY;
                候选.平均深度 = 候选.中心Z;
                候选.最近深度 = 转换毫米(minZ);
                候选.最远深度 = 转换毫米(maxZ);
                候选.像素索引集合.reserve(队列.size());
                for (const auto 像素索引 : 队列) {
                    if (像素索引 <= static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max())) {
                        候选.像素索引集合.push_back(static_cast<std::uint32_t>(像素索引));
                    }
                }
                候选.空间连续性评分 = std::min<std::int64_t>(
                    10000,
                    7000 + static_cast<std::int64_t>(std::min<std::size_t>(队列.size(), 3000) / 10));
                const bool 范围有效 = maxX >= minX && maxY >= minY && maxZ >= minZ;
                候选.范围稳定性评分 = 范围有效 ? 8000 : 0;
                填充像素归属验证摘要(
                    帧,
                    候选,
                    队列,
                    minPX,
                    maxPX,
                    minPY,
                    maxPY,
                    minX,
                    maxX,
                    minY,
                    maxY,
                    minZ,
                    maxZ);
                输出.push_back(候选);
            }
        }

        std::sort(
            输出.begin(),
            输出.end(),
            [](const auto& a, const auto& b) noexcept {
                return a.像素数量 > b.像素数量;
            });
        for (std::size_t i = 0; i < 输出.size(); ++i) {
            输出[i].候选编号 = static_cast<std::int64_t>(i);
        }
        return 输出;
    }

    // 功能：根据当前输入生成目标数据、场景、动态或回执。
    双目相机本能适配器::调用结果 从帧生成结果(const 结构体_原始场景帧& 帧)
    {
        双目相机本能适配器::调用结果 结果{};
        结果.成功 = true;
        结果.相机已打开 = true;
        结果.消息 = "D455 采集一帧完成";
        结果.宽度 = 帧.宽度;
        结果.高度 = 帧.高度;
        结果.系统到达时间_us = 帧.时间戳.系统到达时间_us;
        结果.设备时间_us = 帧.时间戳.设备时间_us;
        结果.时间域 = static_cast<std::uint8_t>(帧.时间戳.域);
        结果.深度帧号 = 帧.时间戳.深度帧号;
        结果.彩色帧号 = 帧.时间戳.彩色帧号;
        const std::size_t 预期像素数 = 帧.宽度 > 0 && 帧.高度 > 0
            ? static_cast<std::size_t>(帧.宽度) * static_cast<std::size_t>(帧.高度)
            : 0;
        结果.预期像素数量 = 转换像素计数(预期像素数);
        结果.像素特征数量 = 转换像素计数(帧.深度.size());
        结果.颜色像素数量 = 转换像素计数(帧.有效颜色() ? 帧.颜色.size() : 0);
        if (帧.有效颜色()) {
            结果.颜色RGB.reserve(帧.颜色.size());
            for (const auto& 颜色 : 帧.颜色) {
                结果.颜色RGB.push_back(双目相机本能适配器::RGB像素{
                    颜色.r,
                    颜色.g,
                    颜色.b
                });
            }
        }
        if (帧.有效深度()) {
            const auto& 原始深度 = 帧.原始深度.size() == 帧.深度.size()
                ? 帧.原始深度
                : 帧.深度;
            const auto& 原始有效 = 帧.原始深度有效.size() == 原始深度.size()
                ? 帧.原始深度有效
                : 帧.深度有效;
            填充深度毫米结构(原始深度, 原始有效, 结果.原始深度毫米);
            if (帧.滤波深度.size() == 帧.深度.size()) {
                填充深度毫米结构(帧.滤波深度, 帧.滤波深度有效, 结果.滤波深度毫米);
            }
            if (帧.补全深度.size() == 帧.深度.size()) {
                填充深度毫米结构(帧.补全深度, 帧.补全深度有效, 结果.补全深度毫米);
            }
            结果.深度有效性Mask.reserve(帧.深度.size());
            for (std::size_t i = 0; i < 帧.深度.size(); ++i) {
                const bool 有效 = 帧像素深度有效(帧, i);
                结果.深度有效性Mask.push_back(有效 ? 1 : 0);
            }
        }
        if (帧.有效点云()) {
            结果.空间坐标毫米XYZ.reserve(帧.点云.size());
            for (std::size_t i = 0; i < 帧.点云.size(); ++i) {
                const bool 有效 = 帧像素有有效空间点(帧, i);
                const auto& 点 = 帧.点云[i];
                结果.空间坐标毫米XYZ.push_back(双目相机本能适配器::空间坐标毫米{
                    有效 ? 转换毫米(点.x) : 0,
                    有效 ? 转换毫米(点.y) : 0,
                    有效 ? 转换毫米(点.z) : 0
                });
            }
        }
        结果.颜色RGB结构状态 = 预期像素数 > 0 && 结果.颜色RGB.size() == 预期像素数 ? 1 : 0;
        结果.原始深度毫米结构状态 = 预期像素数 > 0 && 结果.原始深度毫米.size() == 预期像素数 ? 1 : 0;
        结果.滤波深度毫米结构状态 = 结果.滤波深度毫米.empty() ? 0 : 1;
        结果.补全深度毫米结构状态 = 结果.补全深度毫米.empty() ? 0 : 1;
        结果.深度有效性Mask状态 = 预期像素数 > 0 && 结果.深度有效性Mask.size() == 预期像素数 ? 1 : 0;
        结果.空间坐标毫米XYZ结构状态 = 预期像素数 > 0 && 结果.空间坐标毫米XYZ.size() == 预期像素数 ? 1 : 0;
        建立深度三口径诊断(结果);
        结果.彩色深度已对齐 = 帧.深度已对齐到彩色 ? 1 : 0;
        结果.对齐目标 = 帧.深度已对齐到彩色 ? 1 : 0;
        结果.彩色深度对齐状态 = 结果.彩色深度已对齐;
        std::size_t 深度有效数 = 0;
        for (std::size_t i = 0; i < 帧.深度.size(); ++i) {
            if (帧像素深度有效(帧, i)) {
                ++深度有效数;
            }
        }
        结果.深度有效像素数量 = 转换像素计数(深度有效数);
        std::size_t 点云有效数 = 0;
        if (帧.有效点云()) {
            for (const auto& 点 : 帧.点云) {
                if (点.z > 0.0) {
                    ++点云有效数;
                }
            }
        }
        结果.点云有效像素数量 = 转换像素计数(点云有效数);
        结果.像素覆盖状态 = 预期像素数 > 0 && 帧.深度.size() == 预期像素数 ? 1 : 0;
        结果.空间候选列表 = 提取空间候选摘要(帧);
        结果.空间候选数量 = 转换像素计数(结果.空间候选列表.size());
        std::size_t 空间候选点数 = 0;
        for (const auto& 候选 : 结果.空间候选列表) {
            if (候选.像素数量 > 0) {
                空间候选点数 += static_cast<std::size_t>(候选.像素数量);
            }
        }
        结果.空间候选有效点数量 = 转换像素计数(空间候选点数);
        结果.轮廓数量 = 帧.轮廓观测列表.size();
        结果.深度有效率 = 比例万分比(深度有效数, 预期像素数);
        结果.空间坐标有效率 = 比例万分比(点云有效数, 预期像素数);
        结果.空间点有效率 = 结果.空间坐标有效率;
        结果.深度空洞数量 = 预期像素数 > 深度有效数
            ? 转换像素计数(预期像素数 - 深度有效数)
            : 0;
        std::size_t 深度空洞区域总数量 = 0;
        auto 深度空洞区域列表 = 收集深度空洞诊断区域(帧, 深度空洞区域总数量);
        结果.深度空洞区域数量 = 转换像素计数(深度空洞区域总数量);
        结果.深度空洞区域集合状态 = 结果.深度空洞区域数量 > 0 ? 1 : 0;
        结果.诊断区域列表 = std::move(深度空洞区域列表);
        追加空间候选诊断区域(
            结果.诊断区域列表,
            结果.空间候选列表,
            结果.深度空洞区域数量 + 1,
            帧.宽度,
            帧.高度);
        结果.诊断区域数量 = 转换像素计数(结果.诊断区域列表.size());
        结果.诊断区域集合状态 = 结果.诊断区域数量 > 0 ? 1 : 0;
        结果.诊断区域掩码状态 = 结果.诊断区域集合状态;
        std::size_t 有效轮廓数 = 0;
        for (const auto& 轮廓 : 帧.轮廓观测列表) {
            if (轮廓.有效 && 轮廓.w > 0 && 轮廓.h > 0 && !轮廓.掩膜.empty()) {
                ++有效轮廓数;
            }
        }
        结果.轮廓闭合率 = 比例万分比(有效轮廓数, 帧.轮廓观测列表.size());
        结果.轮廓断裂数量 = 帧.轮廓观测列表.size() > 有效轮廓数
            ? 转换像素计数(帧.轮廓观测列表.size() - 有效轮廓数)
            : 0;
        结果.彩图轮廓数量 = 转换像素计数(有效轮廓数);

        std::int64_t 深度轮廓支持累计 = 0;
        std::int64_t 颜色轮廓支持累计 = 0;
        std::int64_t 空间轮廓支持累计 = 0;
        std::int64_t 融合轮廓置信累计 = 0;
        for (const auto& 候选 : 结果.空间候选列表) {
            if (候选.轮廓深度支持率 > 0) {
                ++结果.深度轮廓数量;
                深度轮廓支持累计 += 候选.轮廓深度支持率;
            }
            if (候选.轮廓空间支持率 > 0) {
                ++结果.空间投影轮廓数量;
                空间轮廓支持累计 += 候选.轮廓空间支持率;
            }
            if (候选.轮廓颜色支持率 > 0) {
                颜色轮廓支持累计 += 候选.轮廓颜色支持率;
            }
            if ((候选.轮廓来源类型 & 8) != 0) {
                ++结果.融合轮廓数量;
                融合轮廓置信累计 += 候选.轮廓置信度;
            }
            if (候选.轮廓跨越深度断裂) {
                ++结果.跨越深度断裂轮廓数量;
            }
            if (候选.轮廓穿过深度无效区域) {
                ++结果.穿过深度无效区域轮廓数量;
                if (结果.补全深度低置信像素数量 > 0) {
                    ++结果.依赖补全深度轮廓数量;
                }
            }
        }
        结果.轮廓来源分层状态 = (结果.彩图轮廓数量
            + 结果.深度轮廓数量
            + 结果.空间投影轮廓数量
            + 结果.融合轮廓数量) > 0 ? 1 : 0;
        结果.轮廓深度支持率 = 结果.深度轮廓数量 == 0
            ? 0
            : 深度轮廓支持累计 / 结果.深度轮廓数量;
        结果.轮廓颜色支持率 = 结果.彩图轮廓数量 == 0
            ? 0
            : std::max<std::int64_t>(
                结果.轮廓闭合率,
                结果.空间候选列表.empty() ? 0 : 颜色轮廓支持累计 / static_cast<std::int64_t>(结果.空间候选列表.size()));
        结果.轮廓空间支持率 = 结果.空间投影轮廓数量 == 0
            ? 0
            : 空间轮廓支持累计 / 结果.空间投影轮廓数量;
        结果.融合轮廓置信度 = 结果.融合轮廓数量 == 0
            ? 0
            : 融合轮廓置信累计 / 结果.融合轮廓数量;
        std::size_t 验证通过候选数 = 0;
        std::int64_t 深度稳定累计 = 0;
        std::int64_t 未解释累计 = 0;
        for (const auto& 候选 : 结果.空间候选列表) {
            if (候选.像素归属验证状态 > 0) {
                ++验证通过候选数;
            }
            深度稳定累计 += 候选.深度一致率;
            未解释累计 += std::max<std::int64_t>(0, 候选.未解释像素数);
        }
        结果.边界深度稳定率 = 结果.空间候选列表.empty()
            ? 0
            : 深度稳定累计 / static_cast<std::int64_t>(结果.空间候选列表.size());
        结果.候选存在验证通过率 = 比例万分比(验证通过候选数, 结果.空间候选列表.size());
        const std::size_t 未解释像素数 = static_cast<std::size_t>(
            std::min<std::int64_t>(未解释累计, static_cast<std::int64_t>(std::min<std::size_t>(
                预期像素数,
                static_cast<std::size_t>(std::numeric_limits<std::int64_t>::max())))));
        结果.未解释区域比例 = 比例万分比(未解释像素数, 预期像素数);

        std::int64_t 补观察缺失原因 = 0;
        std::int64_t 补观察需求建议 = 0;
        if (结果.深度空洞数量 > 0 || 结果.深度空洞区域数量 > 0) {
            补观察缺失原因 |= 缺失原因_深度空洞;
            补观察需求建议 |= 补观察建议_重新取得观察特征帧 | 补观察建议_继续观察未解释区域;
        }
        if (结果.轮廓断裂数量 > 0) {
            补观察缺失原因 |= 缺失原因_轮廓断裂;
            补观察需求建议 |= 补观察建议_内部世界细分;
        }
        if (结果.彩色深度已对齐 <= 0) {
            补观察缺失原因 |= 缺失原因_对齐缺失;
            补观察需求建议 |= 补观察建议_调整双目相机工作模式;
        }
        if (结果.深度有效率 < 5000 || 结果.空间坐标有效率 < 5000) {
            补观察缺失原因 |= 缺失原因_有效点不足;
            补观察需求建议 |= 补观察建议_缩小ROI重新观察;
        }
        if (结果.融合低稳定像素数量 > 0) {
            补观察缺失原因 |= 缺失原因_融合低稳定;
            补观察需求建议 |= 补观察建议_融合稳定观察帧深度 | 补观察建议_重新取得观察特征帧;
        }
        结果.待补观察区域数量 = 结果.深度空洞区域数量;
        for (const auto& 候选 : 结果.空间候选列表) {
            补观察缺失原因 |= 候选.缺失原因;
            补观察需求建议 |= 候选.补观察需求建议;
            结果.待补观察区域数量 += std::max<std::int64_t>(0, 候选.待补观察区域数量);
            if (候选.补全候选状态 > 0) {
                ++结果.补全候选数量;
            }
            if (候选.低置信轮廓状态 > 0) {
                ++结果.低置信轮廓数量;
            }
            if (候选.观察确认补偿状态 == 观察确认补偿状态_条件不足) {
                ++结果.条件不足候选数量;
            } else if (候选.观察确认补偿状态 == 观察确认补偿状态_待验证) {
                ++结果.待验证候选数量;
            } else if (候选.观察确认补偿状态 == 观察确认补偿状态_部分确认) {
                ++结果.部分确认候选数量;
            }
        }
        结果.补观察缺失原因 = 补观察缺失原因;
        结果.补观察需求建议 = 补观察需求建议;
        结果.补观察缺口状态 =
            补观察缺失原因 != 0
            || 结果.待补观察区域数量 > 0
            || 结果.补全候选数量 > 0
            || 结果.低置信轮廓数量 > 0
            || 结果.条件不足候选数量 > 0
            || 结果.待验证候选数量 > 0
            || 结果.部分确认候选数量 > 0
            ? 1
            : 0;
        结果.帧质量评分 = (
            结果.深度有效率 * 35
            + 结果.空间坐标有效率 * 35
            + 结果.候选存在验证通过率 * 20
            + (10000 - std::min<std::int64_t>(10000, 结果.未解释区域比例)) * 10
            ) / 100;
        return 结果;
    }
}

namespace 双目相机本能适配器 {

    // 功能：按函数名执行对应处理。
    调用结果 打开() noexcept
    {
        std::lock_guard<std::mutex> 锁(g_互斥);
        调用结果 结果{};
        std::string 运行时错误{};
        if (!确保RealSense运行时(运行时错误)) {
            结果.原因 = 失败原因::运行时缺失;
            结果.消息 = 运行时错误;
            return 结果;
        }

        try {
            if (!g_相机) {
                D455_相机实现::配置项 配置{};
                配置.启用轮廓提取 = true;
                配置.轮廓_输出原始掩膜 = true;
                g_相机 = std::make_unique<D455_相机实现>(配置);
                g_相机已打开 = false;
            }
            if (!g_相机已打开) {
                if (!g_相机->打开()) {
                    g_相机.reset();
                    g_相机已打开 = false;
                    结果.原因 = 失败原因::打开失败;
                    结果.消息 = "D455 打开失败";
                    return 结果;
                }
                g_相机已打开 = true;
            }
            结果.成功 = true;
            结果.相机已打开 = true;
            结果.消息 = "D455 已可用";
            return 结果;
        }
        catch (const std::exception& e) {
            g_相机.reset();
            g_相机已打开 = false;
            结果.原因 = 失败原因::打开失败;
            结果.消息 = std::string("D455 打开异常: ") + e.what();
            return 结果;
        }
        catch (...) {
            g_相机.reset();
            g_相机已打开 = false;
            结果.原因 = 失败原因::打开失败;
            结果.消息 = "D455 打开未知异常";
            return 结果;
        }
    }

    // 功能：按函数名执行对应处理。
    调用结果 释放() noexcept
    {
        std::lock_guard<std::mutex> 锁(g_互斥);
        调用结果 结果{};
        try {
            if (g_相机) {
                g_相机->关闭();
            }
            g_相机.reset();
            g_相机已打开 = false;
            结果.成功 = true;
            结果.消息 = "D455 已释放";
            return 结果;
        }
        catch (...) {
            g_相机.reset();
            g_相机已打开 = false;
            结果.成功 = true;
            结果.消息 = "D455 释放时出现异常，已清理本地持有状态";
            return 结果;
        }
    }

    // 功能：检查状态、条件、缺口或运行约束。
    调用结果 检查() noexcept
    {
        std::lock_guard<std::mutex> 锁(g_互斥);
        调用结果 结果{};
        std::string 运行时错误{};
        if (!确保RealSense运行时(运行时错误)) {
            结果.成功 = true;
            结果.相机已打开 = false;
            结果.原因 = 失败原因::运行时缺失;
            结果.消息 = 运行时错误;
            return 结果;
        }
        结果.成功 = true;
        结果.相机已打开 = g_相机 && g_相机已打开;
        结果.消息 = 结果.相机已打开 ? "D455 正在运行" : "D455 未打开";
        return 结果;
    }

    // 功能：按函数名执行对应处理。
    调用结果 采集一帧() noexcept
    {
        std::lock_guard<std::mutex> 锁(g_互斥);
        调用结果 结果{};
        if (!g_相机 || !g_相机已打开) {
            结果.原因 = 失败原因::不可用;
            结果.消息 = "D455 未达到可用状态";
            return 结果;
        }

        try {
            constexpr int 静态融合目标帧数 = 3;
            std::vector<结构体_原始场景帧> 帧组{};
            帧组.reserve(静态融合目标帧数);
            结构体_原始场景帧 帧{};
            if (!g_相机->采集一帧(帧)) {
                结果.原因 = 失败原因::采集失败;
                结果.消息 = "D455 采集一帧失败";
                return 结果;
            }
            帧组.push_back(std::move(帧));
            for (int i = 1; i < 静态融合目标帧数; ++i) {
                结构体_原始场景帧 后续帧{};
                if (!g_相机->采集一帧(后续帧)) {
                    break;
                }
                帧组.push_back(std::move(后续帧));
            }
            auto 输出 = 从帧生成结果(帧组.back());
            建立静态多帧融合诊断(输出, 帧组);
            return 输出;
        }
        catch (const std::exception& e) {
            结果.原因 = 失败原因::采集失败;
            结果.消息 = std::string("D455 采集异常: ") + e.what();
            return 结果;
        }
        catch (...) {
            结果.原因 = 失败原因::采集失败;
            结果.消息 = "D455 采集未知异常";
            return 结果;
        }
    }
}
