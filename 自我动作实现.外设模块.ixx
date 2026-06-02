module;

#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "本能方法类.h"
#include "世界树类.h"
#include "方法类.h"
#include "方法虚拟存在服务类.h"
#include "语素类.h"
#include "双目相机本能适配器.h"

export module 自我动作实现.外设模块;

import 本能动作管理模块;
import 日志模块;
import 外设观察报告队列;
import 自我类.特征定义;

namespace {
    inline constexpr I64 历史本能方法ID_增加可用方法数量 = 6049;
    inline constexpr I64 历史本能方法ID_改变特征值 = 6058;
    inline constexpr I64 历史本能方法ID_任务筹办 = 6062;
    inline constexpr I64 历史本能方法ID_任务执行 = 6063;
    inline constexpr I64 空间候选独立状态_不确定 = 0;
    inline constexpr I64 空间候选独立状态_相对独立 = 1;
    inline constexpr I64 空间候选独立状态_不独立 = 2;
    inline constexpr I64 空间候选独立状态_待拆分 = 3;
    inline constexpr I64 空间候选独立状态_待合并 = 4;
    inline constexpr I64 观察存在假设状态_未生成 = 0;
    inline constexpr I64 观察存在假设状态_已生成 = 1;
    inline constexpr I64 观察存在假设验证状态_待验证 = 1;
    inline constexpr I64 像素归属验证状态_未生成 = 0;
    inline constexpr I64 像素归属验证状态_已生成待确认 = 1;
    inline constexpr I64 全帧像素归属账状态_未生成 = 0;
    inline constexpr I64 全帧像素归属账状态_已生成 = 1;
    inline constexpr I64 全帧像素归属账状态_有冲突 = 2;
    inline constexpr I64 观察存在确认状态_未确认 = 0;
    inline constexpr I64 观察存在确认状态_已验证 = 1;
    inline constexpr I64 观察存在确认状态_待确认 = 2;
    inline constexpr I64 观察存在确认状态_待拆分 = 3;
    inline constexpr I64 观察存在确认状态_待合并 = 4;
    inline constexpr I64 观察存在确认状态_已否定 = 5;
    inline constexpr I64 外设观察存在候选确认状态_未确认 = 0;
    inline constexpr I64 外设观察存在候选确认状态_方案已生成 = 1;
    inline constexpr I64 外设观察存在候选确认状态_证据不足 = 2;
    inline constexpr I64 外设观察存在候选确认状态_冲突 = 3;
    inline constexpr I64 外设观察存在候选确认方案_无 = 0;
    inline constexpr I64 外设观察存在候选确认方案_新建 = 1;
    inline constexpr I64 外设观察存在候选确认方案_合并 = 2;
    inline constexpr I64 外设观察存在候选确认方案_冲突 = 3;
    inline constexpr I64 外设观察存在候选确认方案_证据不足 = 4;
    inline constexpr I64 观察存在事实提交状态_未提交 = 0;
    inline constexpr I64 观察存在事实提交状态_已提交 = 1;
    inline constexpr I64 观察存在事实提交状态_部分提交 = 2;
    inline constexpr I64 观察存在事实提交状态_条件不足 = 3;
    inline constexpr I64 当前场景特征值可读状态_未可读 = 0;
    inline constexpr I64 当前场景特征值可读状态_部分可读 = 1;
    inline constexpr I64 当前场景特征值可读状态_可读 = 2;
    inline constexpr I64 当前场景特征值可读状态_证据不足 = 3;
    inline constexpr I64 当前场景特征值可读状态_冲突 = 4;
    inline constexpr I64 当前场景特征值可读状态_过期 = 5;
    inline constexpr I64 当前场景特征值明确范围类型_全场景 = 1;
    inline constexpr I64 当前场景特征值明确范围类型_已识别区域 = 2;
    inline constexpr I64 当前场景特征值明确范围类型_指定区域 = 3;
    inline constexpr I64 当前场景特征值明确范围类型_指定存在 = 4;
    inline constexpr I64 当前场景特征值明确范围类型_跟踪目标 = 5;
    inline constexpr I64 当前场景特征值缺口可解释状态_未解释 = 0;
    inline constexpr I64 当前场景特征值缺口可解释状态_可解释 = 1;
    inline constexpr I64 当前场景特征值缺口可解释状态_部分解释 = 2;
    inline constexpr I64 当前场景特征值材料门版本_v1 = 1;
    inline constexpr I64 当前场景特征值可读掩码_已确认观察存在引用 = 1;
    inline constexpr I64 当前场景特征值可读掩码_当前场景在场关系 = 2;
    inline constexpr I64 当前场景特征值可读掩码_来源报告外设时间 = 4;
    inline constexpr I64 当前场景特征值可读掩码_观察材料可回查 = 8;
    inline constexpr I64 当前场景特征值可读掩码_来源空间材料 = 16;
    inline constexpr I64 当前场景特征值可读掩码_深度空间质量 = 32;
    inline constexpr I64 当前场景特征值可读掩码_跨帧稳定性 = 64;
    inline constexpr I64 当前场景特征值可读掩码_场景绝对坐标 = 128;
    inline constexpr I64 当前场景特征值缺失原因_无 = 0;
    inline constexpr I64 当前场景特征值缺失原因_缺已确认观察存在引用 = 1;
    inline constexpr I64 当前场景特征值缺失原因_缺当前场景在场关系 = 2;
    inline constexpr I64 当前场景特征值缺失原因_缺来源报告外设时间 = 4;
    inline constexpr I64 当前场景特征值缺失原因_缺可回查观察材料 = 8;
    inline constexpr I64 当前场景特征值缺失原因_缺位置或空间范围 = 16;
    inline constexpr I64 当前场景特征值缺失原因_缺深度空间质量 = 32;
    inline constexpr I64 当前场景特征值缺失原因_缺跨帧稳定性 = 64;
    inline constexpr I64 当前场景特征值缺失原因_缺场景绝对坐标 = 128;
    inline constexpr I64 当前场景特征值过期原因_无 = 0;
    inline constexpr I64 当前场景特征值过期原因_来源报告过期 = 1;
    inline constexpr I64 当前场景特征值冲突原因_无 = 0;
    inline constexpr I64 当前场景特征值冲突原因_空间范围反向 = 1;
    inline constexpr I64 当前场景特征值冲突原因_来源簇不匹配 = 2;
    inline constexpr I64 当前帧轮廓分区状态_未生成 = 0;
    inline constexpr I64 当前帧轮廓分区状态_已生成 = 1;
    inline constexpr I64 当前帧轮廓分区状态_条件不足 = 2;
    inline constexpr I64 当前帧轮廓分区状态_部分生成 = 3;
    inline constexpr I64 已知存在当前帧扫描状态_未生成 = 0;
    inline constexpr I64 已知存在当前帧扫描状态_已生成 = 1;
    inline constexpr I64 已知存在当前帧扫描状态_待重识别 = 2;
    inline constexpr I64 未知区域识别状态_无未知区域 = 0;
    inline constexpr I64 未知区域识别状态_待识别 = 1;
    inline constexpr I64 当前帧遮挡判断状态_未判断 = 0;
    inline constexpr I64 当前帧遮挡判断状态_无遮挡证据 = 1;
    inline constexpr I64 当前帧遮挡判断状态_可能遮挡 = 2;
    inline constexpr I64 当前帧轮廓冲突区域明确状态_无冲突 = 1;
    inline constexpr I64 当前帧轮廓冲突区域明确状态_有冲突 = 2;
    inline constexpr I64 当前帧像素归属闭合状态_未闭合 = 0;
    inline constexpr I64 当前帧像素归属闭合状态_已闭合 = 1;
    inline constexpr I64 当前帧区域归属状态_未归属 = 0;
    inline constexpr I64 当前帧区域归属状态_已识别区域 = 1;
    inline constexpr I64 当前帧区域归属状态_未识别区域 = 2;
    inline constexpr I64 当前帧区域归属状态_候选区域 = 3;
    inline constexpr I64 当前帧区域归属状态_冲突区域 = 4;
    inline constexpr I64 当前帧区域归属状态_无效区域 = 5;
    inline constexpr I64 当前帧区域归属状态_背景区域 = 6;
    inline constexpr I64 帧解释状态_未生成 = 0;
    inline constexpr I64 帧解释状态_已更新 = 1;
    inline constexpr I64 帧解释状态_部分解释 = 2;
    inline constexpr I64 观察确认补偿状态_部分确认 = 6;
    inline constexpr I64 观察确认补偿状态_待验证 = 8;
    inline constexpr I64 观察确认补偿状态_条件不足 = 7;
    inline constexpr I64 内部世界状态_未建立 = 0;
    inline constexpr I64 内部世界状态_已建立 = 1;
    inline constexpr I64 内外接口状态_已建立 = 1;
    inline constexpr I64 当前观察材料状态_已建立 = 1;
    inline constexpr I64 子候选集合状态_已生成 = 1;
    inline constexpr I64 内部世界局部材料建立状态_未建立 = 0;
    inline constexpr I64 内部世界局部材料建立状态_已建立 = 1;
    inline constexpr I64 内部世界局部轮廓材料可回查状态_不可回查 = 0;
    inline constexpr I64 内部世界局部轮廓材料可回查状态_可回查 = 1;
    inline constexpr I64 内部世界子候选验证状态_未验证 = 0;
    inline constexpr I64 内部世界子候选验证状态_待验证 = 1;
    inline constexpr I64 内部世界子候选验证状态_已验证 = 2;
    inline constexpr I64 内部世界子存在候选确认状态_未确认 = 0;
    inline constexpr I64 内部世界子存在候选确认状态_方案已生成 = 1;
    inline constexpr I64 内部世界子存在候选确认状态_证据不足 = 2;
    inline constexpr I64 内部世界子存在确认方案_无 = 0;
    inline constexpr I64 内部世界子存在确认方案_新建 = 1;
    inline constexpr I64 内部世界子存在确认方案_合并 = 2;
    inline constexpr I64 内部世界子存在确认方案_证据不足 = 4;
    inline constexpr I64 内部世界子存在事实提交状态_未提交 = 0;
    inline constexpr I64 内部世界子存在事实提交状态_已提交 = 1;
    inline constexpr I64 内部世界子存在事实提交状态_条件不足 = 3;
    inline constexpr I64 成员存在集合状态_已更新 = 1;
    inline constexpr I64 内部世界细分状态_未细分 = 0;
    inline constexpr I64 内部世界细分状态_已完成 = 1;
    inline constexpr I64 内部世界细分状态_部分完成 = 2;
    inline constexpr I64 内部世界细分状态_无法细分 = 3;
    inline constexpr I64 内部子候选类型_空间子候选 = 1;
    inline constexpr I64 内部子候选类型_表面特征候选 = 2;
    inline constexpr I64 内部子候选类型_叶子候选 = 5;
    inline constexpr I64 内部子存在类型_表面特征存在 = 2;
    inline constexpr I64 内部子存在类型_叶子特征块存在 = 5;
    inline constexpr I64 历史对应状态_未匹配 = 0;
    inline constexpr I64 历史对应状态_已匹配 = 1;
    inline constexpr I64 历史对应状态_冲突 = 2;
    inline constexpr I64 变化结果状态_未生成 = 0;
    inline constexpr I64 变化结果状态_已生成 = 1;
    inline constexpr I64 变化状态_未变化 = 0;
    inline constexpr I64 变化状态_已移动 = 1;
    inline constexpr I64 变化状态_已变大 = 2;
    inline constexpr I64 变化状态_已变小 = 3;
    inline constexpr I64 变化状态_内部结构变化 = 4;
    inline constexpr I64 变化状态_不确定 = 5;
    inline constexpr I64 观察变化事件集合状态_已生成 = 1;
    inline constexpr I64 观察变化事件类型_无变化 = 0;
    inline constexpr I64 观察变化事件类型_新增存在 = 1;
    inline constexpr I64 观察变化事件类型_未命中 = 2;
    inline constexpr I64 观察变化事件类型_位置变化 = 3;
    inline constexpr I64 观察变化事件类型_范围变化 = 4;
    inline constexpr I64 观察变化事件类型_尺寸变化 = 5;
    inline constexpr I64 观察变化事件类型_内部结构变化 = 6;
    inline constexpr I64 观察变化事件类型_不确定 = 9;
    inline constexpr I64 场景识别快照状态_已生成 = 1;
    inline constexpr I64 OpenGL复现数据状态_已生成 = 1;
    inline constexpr I64 复现数据缓冲状态_摘要级 = 1;
    inline constexpr I64 观察事实状态_未成立 = 0;
    inline constexpr I64 观察事实状态_已取得 = 1;
    inline constexpr I64 观察事实状态_已建立 = 1;
    inline constexpr I64 观察事实状态_已覆盖 = 1;
    inline constexpr I64 观察事实状态_已完成 = 1;
    inline constexpr I64 观察事实状态_已完备 = 1;
    inline constexpr I64 观察事实状态_新鲜 = 1;
    inline constexpr I64 观察事实状态_过期 = 2;
    inline constexpr I64 观察事实状态_可用 = 1;
    inline constexpr I64 安全明确状态_未明确 = 0;
    inline constexpr I64 安全明确状态_已明确 = 1;
    inline constexpr I64 安全判定状态_未评估 = 0;
    inline constexpr I64 安全判定状态_不安全 = 1;
    inline constexpr I64 安全判定状态_安全 = 2;
    inline constexpr I64 安全判定来源_未判定 = 0;
    inline constexpr I64 安全判定来源_已证安全 = 1;
    inline constexpr I64 安全判定来源_已证不安全 = 2;
    inline constexpr I64 安全判定来源_证据不足默认不安全 = 3;
    inline constexpr I64 安全判定来源_条件不足默认不安全 = 4;
    inline constexpr I64 安全判定来源_无负向因果默认安全 = 5;
    inline constexpr I64 基础风险判断状态_未评估 = 0;
    inline constexpr I64 基础风险判断状态_已明确 = 1;
    inline constexpr I64 基础风险判断状态_条件不足 = 2;
    inline constexpr I64 基础风险判断状态_证据不足 = 3;
    inline constexpr I64 场景绝对安全值 = (std::numeric_limits<I64>::max)();
    inline constexpr I64 风险安全层版本_v1 = 1;
    inline constexpr I64 风险安全层v1因素数量 = 5;
    inline constexpr I64 场景已证不安全值 = 0;
    inline constexpr I64 场景条件不足安全值 = 场景绝对安全值 / 4;
    inline constexpr I64 场景未知安全值 = 场景绝对安全值 / 2;
    inline constexpr I64 风险安全场景影响可结算状态_未判定 = 0;
    inline constexpr I64 风险安全场景影响可结算状态_不可结算 = 1;
    inline constexpr I64 风险安全场景影响可结算状态_可结算 = 2;
    inline constexpr I64 风险安全场景影响入账状态_未入账 = 0;
    inline constexpr I64 风险安全场景影响入账状态_已入账 = 1;
    inline constexpr I64 风险安全场景影响变化方向_未明确 = 0;
    inline constexpr I64 风险安全场景影响变化方向_上调 = 1;
    inline constexpr I64 风险安全场景影响变化方向_下调 = 2;
    inline constexpr I64 风险安全场景影响变化方向_不变 = 3;
    inline constexpr I64 风险安全场景影响变化方向_初始入账 = 4;
    inline constexpr I64 风险安全场景影响不可结算原因_无 = 0;
    inline constexpr I64 风险安全场景影响不可结算原因_缺来源需求 = 1;
    inline constexpr I64 风险安全场景影响不可结算原因_缺来源任务 = 2;
    inline constexpr I64 风险安全场景影响不可结算原因_缺方法动作动态 = 4;
    inline constexpr I64 风险安全场景影响不可结算原因_缺评估结果场景 = 8;
    inline constexpr I64 风险安全场景影响不可结算原因_场景存在集合未明确 = 16;
    inline constexpr I64 风险安全场景影响不可结算原因_风险状态未明确 = 32;
    inline constexpr I64 风险安全场景影响不可结算原因_证据不足原因非零 = 64;
    inline constexpr I64 风险安全场景影响不可结算原因_缺被动归因证据 = 128;
    inline constexpr I64 风险安全场景影响不可结算原因_缺稳定性证据 = 256;
    inline constexpr I64 风险安全场景影响不可结算原因_缺公式版本 = 512;
    inline constexpr I64 风险安全场景影响不可结算原因_变化方向未明确 = 1024;
    inline constexpr I64 风险安全场景影响不可结算原因_上调缺持续稳定 = 2048;
    inline constexpr I64 风险安全场景影响不可结算原因_下调缺当前风险证据 = 4096;
    inline constexpr I64 风险安全场景影响不可结算原因_缺提交入口 = 8192;
    inline constexpr I64 风险安全场景影响提交状态_未提交 = 0;
    inline constexpr I64 风险安全场景影响提交状态_条件不足 = 1;
    inline constexpr I64 风险安全场景影响提交状态_已提交 = 2;
    inline constexpr I64 风险安全因素掩码_场景影响 = 0x01;
    inline constexpr I64 风险安全因素掩码_运行影响 = 0x02;
    inline constexpr I64 风险安全因素掩码_资源影响 = 0x04;
    inline constexpr I64 风险安全因素掩码_权限影响 = 0x08;
    inline constexpr I64 风险安全因素掩码_未知影响 = 0x10;
    inline constexpr I64 风险安全因素掩码_全部 =
        风险安全因素掩码_场景影响
        | 风险安全因素掩码_运行影响
        | 风险安全因素掩码_资源影响
        | 风险安全因素掩码_权限影响
        | 风险安全因素掩码_未知影响;
    inline constexpr I64 风险安全因素掩码_非场景 =
        风险安全因素掩码_运行影响
        | 风险安全因素掩码_资源影响
        | 风险安全因素掩码_权限影响
        | 风险安全因素掩码_未知影响;
    inline constexpr I64 安全因果证据方向_未定义 = 0;
    inline constexpr I64 安全因果证据方向_负向 = 1;
    inline constexpr I64 安全因果证据方向_正向 = 2;
    inline constexpr I64 安全因果证据方向_稳定性 = 3;
    inline constexpr I64 安全因果证据方向_归因 = 4;
    inline constexpr I64 安全因果因素搜索状态_未执行 = 0;
    inline constexpr I64 安全因果因素搜索状态_已完成 = 1;
    inline constexpr I64 安全因果因素搜索状态_缺参数 = 2;
    inline constexpr I64 安全因果因素搜索状态_搜索缺口 = 3;
    inline constexpr I64 安全因果因素搜索状态_发现负向证据 = 4;
    inline constexpr I64 安全因果因素搜索状态_冲突 = 5;
    inline constexpr I64 安全因果因素范围完整状态_未明确 = 0;
    inline constexpr I64 安全因果因素范围完整状态_完整 = 1;
    inline constexpr I64 安全因果因素默认满足允许状态_不允许 = 0;
    inline constexpr I64 安全因果因素默认满足允许状态_允许 = 1;
    inline constexpr I64 安全因果因素入账状态_未入账 = 0;
    inline constexpr I64 安全因果因素入账状态_已入账 = 1;
    inline constexpr I64 安全因果因素提交状态_未提交 = 0;
    inline constexpr I64 安全因果因素提交状态_条件不足 = 1;
    inline constexpr I64 安全因果因素提交状态_已提交 = 2;
    inline constexpr I64 安全评估不足原因_基础观察事实不可用 = 1;
    inline constexpr I64 安全评估不足原因_场景存在集合未知 = 2;
    inline constexpr I64 安全评估不足原因_关键特征变化未知 = 4;
    inline constexpr I64 安全评估不足原因_因果证据不足 = 8;
    inline constexpr I64 安全因果候选证据状态_未搜索 = 0;
    inline constexpr I64 安全因果候选证据状态_已完成无负向 = 1;
    inline constexpr I64 安全因果候选证据状态_发现负向 = 2;
    inline constexpr I64 安全因果候选当前特征值状态_缺失 = 0;
    inline constexpr I64 安全因果候选当前特征值状态_可读 = 1;

    inline constexpr I64 计算场景评估安全值候选(
        bool 可生成评估,
        bool 条件不足,
        I64 不足原因,
        bool 存在评估全部明确,
        I64 场景风险压力平均值) noexcept
    {
        if (!可生成评估) return 场景已证不安全值;
        if (条件不足) return 场景条件不足安全值;
        if (不足原因 != 0) return 场景未知安全值;
        if (!存在评估全部明确) return 场景未知安全值;
        if (场景风险压力平均值 < 0) return 场景未知安全值;
        const I64 压力 = 场景风险压力平均值 > 场景绝对安全值
            ? 场景绝对安全值
            : 场景风险压力平均值;
        return 场景绝对安全值 - 压力;
    }

    inline I64 限制I64(I64 值, I64 下界, I64 上界) noexcept
    {
        if (值 < 下界) return 下界;
        if (值 > 上界) return 上界;
        return 值;
    }

    struct 结构_安全评估证据视图 {
        bool 自我关键特征观察明确 = false;
        bool 场景存在集合明确 = false;
        bool 安全因果候选明确 = false;
        I64 不足原因 = 0;
    };

    struct 结构_存在风险评估结果 {
        存在节点类* 存在 = nullptr;
        bool 评估明确 = false;
        I64 风险安全值候选 = 0;
        I64 存在安全度候选 = 场景绝对安全值;
        I64 历史值 = 0;
        I64 历史直接伤害次数 = 0;
        I64 最近直接伤害时间 = 0;
        I64 无伤害共处时长 = 0;
        I64 伤害可能性下界 = 0;
        I64 评估置信度 = 0;
        I64 证据不足原因 = 0;
    };

    inline 结构_存在风险评估结果 评估存在风险安全值(
        存在节点类* 存在,
        const 结构_安全评估证据视图& 证据) noexcept
    {
        结构_存在风险评估结果 结果{};
        结果.存在 = 存在;
        // 历史值缺失时必须为 0，不能用固定保守常量伪造历史风险。
        结果.历史值 = 0;
        const I64 证据不足保守风险 = 结果.历史值 / 2;

        if (!证据.场景存在集合明确 || 存在 == nullptr) {
            结果.风险安全值候选 = 证据不足保守风险;
            结果.存在安全度候选 = 场景绝对安全值 - 结果.风险安全值候选;
            结果.证据不足原因 = 安全评估不足原因_场景存在集合未知;
            return 结果;
        }

        if (!证据.自我关键特征观察明确 || !证据.安全因果候选明确) {
            结果.风险安全值候选 = 0;
            结果.存在安全度候选 = 场景绝对安全值;
            结果.评估置信度 = 0;
            结果.评估明确 = false;
            结果.证据不足原因 = 证据.不足原因 != 0
                ? 证据.不足原因
                : 安全评估不足原因_因果证据不足;
            return 结果;
        }

        I64 非零下界 = 0;
        if (结果.历史直接伤害次数 > 0) {
            非零下界 = 1;
        }

        const I64 峰值风险 = 结果.历史值;
        const I64 当前风险 = 限制I64(峰值风险, 非零下界, 场景绝对安全值);
        结果.风险安全值候选 = 当前风险;
        结果.存在安全度候选 = 场景绝对安全值 - 当前风险;
        结果.伤害可能性下界 = 非零下界;
        结果.评估置信度 = 证据.安全因果候选明确 && 证据.不足原因 == 0 ? 1 : 0;
        结果.评估明确 = true;
        结果.证据不足原因 = 0;
        return 结果;
    }

    inline I64 平均存在风险压力(
        const std::vector<结构_存在风险评估结果>& 存在风险评估集) noexcept
    {
        if (存在风险评估集.empty()) return 0;

        const I64 除数 = static_cast<I64>(存在风险评估集.size());
        I64 商和 = 0;
        I64 余数和 = 0;
        for (const auto& 结果 : 存在风险评估集) {
            const I64 风险值 = 限制I64(结果.风险安全值候选, I64{0}, 场景绝对安全值);
            商和 += 风险值 / 除数;
            余数和 += 风险值 % 除数;
            if (余数和 >= 除数) {
                商和 += 余数和 / 除数;
                余数和 %= 除数;
            }
        }
        return 限制I64(商和, I64{0}, 场景绝对安全值);
    }

    inline I64 平均风险安全层v1因素(
        I64 场景影响部分,
        I64 运行影响部分,
        I64 资源影响部分,
        I64 权限影响部分,
        I64 未知影响部分) noexcept
    {
        constexpr I64 除数 = 风险安全层v1因素数量;
        const I64 因素集[] = {
            场景影响部分,
            运行影响部分,
            资源影响部分,
            权限影响部分,
            未知影响部分,
        };

        I64 商和 = 0;
        I64 余数和 = 0;
        for (const I64 原始值 : 因素集) {
            const I64 值 = 限制I64(原始值, I64{0}, 场景绝对安全值);
            商和 += 值 / 除数;
            余数和 += 值 % 除数;
            if (余数和 >= 除数) {
                商和 += 余数和 / 除数;
                余数和 %= 除数;
            }
        }
        return 限制I64(商和, I64{0}, 场景绝对安全值);
    }

    inline I64 计算风险安全第二层投影候选(I64 风险安全层候选) noexcept
    {
        const I64 层值 = 限制I64(风险安全层候选, I64{0}, 场景绝对安全值);
        const I64 商 = 层值 / 100;
        const I64 余 = 层值 % 100;
        return 限制I64(商 * 9 + (余 * 9) / 100, I64{0}, 场景绝对安全值);
    }

    inline I64 统计风险安全因素位数(I64 掩码) noexcept
    {
        I64 数量 = 0;
        for (I64 位 = 1; 位 <= 风险安全因素掩码_未知影响; 位 <<= 1) {
            if ((掩码 & 位) != 0) {
                ++数量;
            }
        }
        return 数量;
    }

    struct 结构_安全因果因素证据搜索结果 {
        I64 层级 = 0;
        I64 因素ID = 0;
        I64 证据方向 = 安全因果证据方向_未定义;
        I64 搜索状态 = 安全因果因素搜索状态_未执行;
        I64 搜索范围完整状态 = 安全因果因素范围完整状态_未明确;
        I64 负向证据数量 = 0;
        I64 正向证据数量 = 0;
        I64 稳定性证据数量 = 0;
        I64 归因证据数量 = 0;
        I64 材料缺失数量 = 0;
        I64 冲突数量 = 0;
        I64 搜索置信度 = 0;
        I64 默认满足允许状态 = 安全因果因素默认满足允许状态_不允许;
        I64 评估值候选 = 0;
    };

    inline 结构_安全因果因素证据搜索结果 搜索安全因果因素证据(
        I64 层级,
        I64 因素ID,
        I64 证据方向,
        I64 搜索范围完整状态,
        I64 负向证据数量,
        I64 正向证据数量,
        I64 稳定性证据数量,
        I64 归因证据数量,
        I64 材料缺失数量,
        I64 冲突数量,
        I64 搜索置信度,
        I64 评估值候选) noexcept
    {
        结构_安全因果因素证据搜索结果 结果{};
        结果.层级 = 层级;
        结果.因素ID = 因素ID;
        结果.证据方向 = 证据方向;
        结果.搜索范围完整状态 = 搜索范围完整状态;
        结果.负向证据数量 = std::max<I64>(0, 负向证据数量);
        结果.正向证据数量 = std::max<I64>(0, 正向证据数量);
        结果.稳定性证据数量 = std::max<I64>(0, 稳定性证据数量);
        结果.归因证据数量 = std::max<I64>(0, 归因证据数量);
        结果.材料缺失数量 = std::max<I64>(0, 材料缺失数量);
        结果.冲突数量 = std::max<I64>(0, 冲突数量);
        结果.搜索置信度 = 限制I64(搜索置信度, I64{0}, I64{10000});
        结果.评估值候选 = 限制I64(评估值候选, I64{0}, 场景绝对安全值);

        if (层级 <= 0 || 因素ID <= 0 || 证据方向 == 安全因果证据方向_未定义) {
            结果.搜索状态 = 安全因果因素搜索状态_缺参数;
            结果.材料缺失数量 = std::max<I64>(结果.材料缺失数量, 1);
            return 结果;
        }
        if (结果.冲突数量 > 0) {
            结果.搜索状态 = 安全因果因素搜索状态_冲突;
            return 结果;
        }
        if (搜索范围完整状态 != 安全因果因素范围完整状态_完整
            || 结果.材料缺失数量 > 0) {
            结果.搜索状态 = 安全因果因素搜索状态_搜索缺口;
            return 结果;
        }
        if (结果.负向证据数量 > 0) {
            结果.搜索状态 = 安全因果因素搜索状态_发现负向证据;
            return 结果;
        }

        结果.搜索状态 = 安全因果因素搜索状态_已完成;
        if (证据方向 == 安全因果证据方向_负向 && 结果.搜索置信度 > 0) {
            结果.默认满足允许状态 = 安全因果因素默认满足允许状态_允许;
            结果.评估值候选 = 场景绝对安全值;
        }
        return 结果;
    }

    inline constexpr I64 外设观察候选验证状态_未验证 = 0;
    inline constexpr I64 外设观察候选验证状态_已验证 = 1;
    inline constexpr I64 外设观察候选验证状态_证据不足 = 2;
    inline constexpr I64 外设观察候选验证状态_失败 = 3;
    inline constexpr I64 外设观察验证缺失原因_无 = 0;
    inline constexpr I64 外设观察验证缺失原因_缺报告 = 1;
    inline constexpr I64 外设观察验证缺失原因_缺原始帧 = 2;
    inline constexpr I64 外设观察验证缺失原因_缺深度帧 = 4;
    inline constexpr I64 外设观察验证缺失原因_缺空间候选 = 8;
    inline constexpr I64 外设观察验证缺失原因_缺像素归属材料 = 16;
    inline constexpr I64 外设观察验证缺失原因_簇投影无效 = 32;
    inline constexpr I64 外设观察验证缺失原因_簇集合为空 = 64;
    inline constexpr I64 观察失效状态_未失效 = 0;
    inline constexpr I64 观察失效状态_已失效 = 1;
    inline constexpr I64 来源外设状态_不可追溯 = 0;
    inline constexpr I64 来源外设状态_可追溯 = 1;
    inline constexpr I64 新鲜度失败原因_无 = 0;
    inline constexpr I64 新鲜度失败原因_缺采集时间 = 1;
    inline constexpr I64 新鲜度失败原因_当前时间异常 = 2;
    inline constexpr I64 新鲜度失败原因_缺最大允许年龄 = 4;
    inline constexpr I64 新鲜度失败原因_来源外设不可追溯 = 8;
    inline constexpr I64 新鲜度失败原因_观察已失效 = 16;
    inline constexpr I64 新鲜度失败原因_观察过期 = 32;
    inline constexpr I64 最大允许当前观察年龄微秒 = 120000000;
    inline constexpr I64 观察质量诊断状态_未完成 = 0;
    inline constexpr I64 观察质量诊断状态_已完成 = 1;
    inline constexpr I64 观察质量状态_不足 = 0;
    inline constexpr I64 观察质量状态_可用 = 1;
    inline constexpr I64 观察质量缺口状态_不存在 = 0;
    inline constexpr I64 观察质量缺口状态_存在 = 1;
    inline constexpr I64 观察适用状态_不可用 = 0;
    inline constexpr I64 观察适用状态_可用 = 1;
    inline constexpr I64 观察适用状态_降级可用 = 2;
    inline constexpr I64 观察质量改善线索状态_未生成 = 0;
    inline constexpr I64 观察质量改善线索状态_已生成 = 1;
    inline constexpr I64 观察质量最低评分 = 5000;
    inline constexpr I64 观察质量最低深度有效率 = 5000;
    inline constexpr I64 观察质量最低空间坐标有效率 = 5000;
    inline constexpr I64 补观察缺失原因_深度空洞 = 1;
    inline constexpr I64 补观察缺失原因_轮廓断裂 = 2;
    inline constexpr I64 补观察缺失原因_对齐缺失 = 4;
    inline constexpr I64 补观察缺失原因_有效点不足 = 8;
    inline constexpr I64 补观察缺失原因_边界不稳定 = 16;
    inline constexpr I64 补观察缺失原因_补全依赖过高 = 32;
    inline constexpr I64 补观察缺失原因_融合低稳定 = 64;
    inline constexpr I64 补观察建议_重新取得观察特征帧 = 1;
    inline constexpr I64 补观察建议_调整双目相机工作模式 = 2;
    inline constexpr I64 补观察建议_缩小ROI重新观察 = 4;
    inline constexpr I64 补观察建议_融合稳定观察帧深度 = 8;
    inline constexpr I64 补观察建议_内部世界细分 = 16;
    inline constexpr I64 补观察建议_继续观察未解释区域 = 32;

    inline const 语素入口节点类* 词_被动动作() noexcept
    {
        static const 语素入口节点类* s_词 =
            语素集.添加信息入口词("被动动作", 枚举_信息入口类型::动态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_条件描述状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法条件自描述状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_条件描述时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法条件自描述时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_条件特征类型数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法条件特征类型数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_必需条件特征类型数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法必需条件特征类型数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_可选条件特征类型数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法可选条件特征类型数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_运行参数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("运行参数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_条件特征() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("条件特征", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_条件集合() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("条件集合", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_输入条件包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("输入条件包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_形参表() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("形参表", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_固定参数表() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("固定参数表", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_结果表() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("结果表", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_效果表() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("效果表", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_调用信息() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("调用信息", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_任务虚拟存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务虚拟存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_任务信息节点() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务信息节点", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_来源需求() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源需求", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_被需求当前状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("被需求当前状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_需求有效截止() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求有效截止", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_累计安全结算() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("累计安全结算", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_累计服务结算() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("累计服务结算", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最近结算时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最近结算时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全结算增量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全结算增量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_服务结算增量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("服务结算增量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_任务完成度百万分比() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务完成度百万分比", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_实际结果状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("实际结果状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_服务对象外在特征状态变化数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("服务对象外在特征状态变化数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_服务对象外在特征状态改善证据() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("服务对象外在特征状态改善证据", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_服务结果可验证() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("服务结果可验证", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_服务结果可归因于自我动作() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("服务结果可归因于自我动作", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_服务结算证据通过() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("服务结算证据通过", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_旧安全值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("旧安全值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_新安全值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("新安全值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_旧服务值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("旧服务值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_新服务值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("新服务值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_允许回写当前状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("允许回写当前状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_来源动作动态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源动作动态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_需求满足登记() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求满足登记", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_需求状态动作动态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求状态动作动态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_任务状态动作动态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务状态动作动态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_任务状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_执行回执表() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行回执表", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_任务层执行回执() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务层执行回执", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_建议任务状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("建议任务状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_建议任务阶段() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("建议任务阶段", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标是否达成() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标是否达成", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_本轮是否有推进() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("本轮是否有推进", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_本轮推进量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("本轮推进量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_剩余差距() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("剩余差距", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_执行结果类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行结果类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_筹办缺口类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("筹办缺口类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_执行缺口类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行缺口类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_等待原因() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("等待原因", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_派生需求提示() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("派生需求提示", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法运行结果() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法运行结果", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法运行结果状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法运行结果状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前方法运行存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前方法运行存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前方法执行情况() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前方法执行情况", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选方法数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选方法数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选方法头节点() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选方法头节点", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_现实场景包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("现实场景包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_任务参数包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务参数包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标现实场景() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标现实场景", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标特征类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标特征类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标特征() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标特征", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_改变方向() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("改变方向", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_已绑定实参表() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("已绑定实参表", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_参数来源() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("参数来源", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_参数值类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("参数值类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_读取特征类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("读取特征类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_是否必需() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("是否必需", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_运行时长() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("运行时长", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_运行次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("运行次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_样本次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("样本次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最近样本时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最近样本时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_结果符合次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("结果符合次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_结果不符合次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("结果不符合次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_动作执行成功次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("动作执行成功次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_动作执行失败次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("动作执行失败次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法运行次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法运行次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法成功次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法成功次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法最近连续成功次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法最近连续成功次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_累计运行时长() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("累计运行时长", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_平均运行时长() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("平均运行时长", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最近运行虚拟存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最近运行虚拟存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最近执行时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最近执行时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最近完成时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最近完成时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_来源_输入现实场景包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源_输入现实场景包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_来源_输入任务参数包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源_输入任务参数包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_来源_输入条件包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源_输入条件包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_来源_方法固定参数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源_方法固定参数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_来源_目标现实场景() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源_目标现实场景", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值类型_I64() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("值类型_I64", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值类型_类型I64() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("类型_I64", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值类型_场景指针() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("值类型_场景指针", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值类型_特征类型指针() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("值类型_特征类型指针", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值类型_特征值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("值类型_特征值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法执行情况() noexcept
    {
        return 本能动作方法执行情况特征词();
    }

    inline const 语素入口节点类* 值_参数绑定失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("参数绑定失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行成功() noexcept
    {
        return 本能动作执行成功词();
    }

    inline const 语素入口节点类* 值_执行失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务回执_运行成功() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务回执_运行成功", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务回执_运行失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务回执_运行失败", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务回执_等待学习() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务回执_等待学习", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务回执_等待补齐() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务回执_等待补齐", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务回执_不可推进() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务回执_不可推进", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务回执_条件不满足() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务回执_条件不满足", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务回执_运行未达成目标() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务回执_运行未达成目标", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_无筹办缺口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("无筹办缺口", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_筹办缺口_任务信息缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("筹办缺口_任务信息缺失", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_筹办缺口_需求目标不可解析() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("筹办缺口_需求目标不可解析", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_筹办缺口_无确定候选方法() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("筹办缺口_无确定候选方法", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_筹办缺口_候选方法不可用() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("筹办缺口_候选方法不可用", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_筹办缺口_候选方法缺执行入口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("筹办缺口_候选方法缺执行入口", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_筹办缺口_候选方法条件不满足() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("筹办缺口_候选方法条件不满足", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_筹办缺口_参数配对失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("筹办缺口_参数配对失败", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_无执行缺口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("无执行缺口", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行缺口_任务状态不允许执行() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行缺口_任务状态不允许执行", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行缺口_当前方法缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行缺口_当前方法缺失", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行缺口_可执行输入参数场景缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行缺口_可执行输入参数场景缺失", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行缺口_当前方法无执行入口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行缺口_当前方法无执行入口", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行缺口_当前方法条件不满足() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行缺口_当前方法条件不满足", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行缺口_当前方法执行失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行缺口_当前方法执行失败", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行缺口_输出结果缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行缺口_输出结果缺失", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行缺口_需求未满足() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行缺口_需求未满足", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_派生需求_重新筹办任务() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("派生需求_重新筹办任务", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_派生需求_检查当前方法执行入口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("派生需求_检查当前方法执行入口", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_方法运行结果_筹办成功() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法运行结果_筹办成功", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_方法运行结果_筹办未完成() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法运行结果_筹办未完成", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_方法运行结果_筹办失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法运行结果_筹办失败", 枚举_信息入口类型::状态模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_运行虚拟存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("运行虚拟存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_输入参数场景() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("输入参数场景", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_输出结果场景() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("输出结果场景", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_返回参数信息() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("返回参数信息", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_返回场景信息() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("返回场景信息", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法虚拟存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法虚拟存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_开始时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("开始时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_结束时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("结束时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_执行前值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行前值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_执行后值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行后值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_动作动态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("动作动态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法条件节点() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法条件节点", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法结果节点() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法结果节点", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全值() noexcept
    {
        return 自我特征定义类::类型_自我_安全值();
    }

    inline const 语素入口节点类* 特征_改变量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("改变量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_服务值() noexcept
    {
        return 自我特征定义类::类型_自我_服务值();
    }

    inline const 语素入口节点类* 特征_待处理方法数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("待处理方法数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_可用方法数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("可用方法数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_需求条件包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求条件包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_查找限制包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("查找限制包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选范围包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选范围包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法搜索根() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法搜索根", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法能力包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法能力包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_特征类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("特征类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_特征值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("特征值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_特征值范围() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("特征值范围", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_区间下界() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("区间下界", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_区间上界() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("区间上界", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_抽象特征引用() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("抽象特征引用", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在引用() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在引用", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_场景引用() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("场景引用", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最高允许目标特征类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最高允许目标特征类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最高允许宿主类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最高允许宿主类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_允许值类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("允许值类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_允许执行差值区间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("允许执行差值区间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最大类型抽象层数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最大类型抽象层数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最大值域抽象层数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最大值域抽象层数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最大候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最大候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最大查找递归深度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最大查找递归深度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_是否包含禁用方法() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("是否包含禁用方法", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_是否允许当前方法自身() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("是否允许当前方法自身", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法配对结果() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法配对结果", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_参数配对结果() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("参数配对结果", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_参数配对结果存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("参数配对结果存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_参数配对情况() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("参数配对情况", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_可执行输入参数场景() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("可执行输入参数场景", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_绑定参数数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("绑定参数数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_缺失参数数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("缺失参数数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_方法配对结果存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法配对结果存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_配对状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("配对状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_来源需求条件包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源需求条件包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_使用限制包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("使用限制包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_选中方法首节点() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("选中方法首节点", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_选中自我能力值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("选中自我能力值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_缓存方法首节点() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("缓存方法首节点", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_缓存方法虚拟存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("缓存方法虚拟存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_选中方法虚拟存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("选中方法虚拟存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_匹配方法能力包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("匹配方法能力包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_原始需求绑定包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("原始需求绑定包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_匹配路径记录() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("匹配路径记录", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_类型抽象距离() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("类型抽象距离", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_值域抽象距离() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("值域抽象距离", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_总匹配距离() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("总匹配距离", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_待配对形参表() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("待配对形参表", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选扫描数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选扫描数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_成功匹配数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("成功匹配数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_失败原因() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("失败原因", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_使用默认方法搜索根() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("使用默认方法搜索根", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_输入绑定失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("输入绑定失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_超出限制() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("超出限制", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_没有匹配方法() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("没有匹配方法", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_候选方法不可用() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选方法不可用", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_输入条件包绑定失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("输入条件包绑定失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_方法虚拟存在缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法虚拟存在缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_自身方法头节点缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("自身方法头节点缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_本轮运行虚拟存在缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("本轮运行虚拟存在缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_方法条件结果场景缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法条件结果场景缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_方法规格确认失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法规格确认失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_方法配对结果创建失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法配对结果创建失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_不修改现实场景特征值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("不修改现实场景特征值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_不执行被查找到的方法() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("不执行被查找到的方法", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_外设运行时缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设运行时缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_外设打开失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设打开失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_外设不可用() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设不可用", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_外设采集失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设采集失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标宿主() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标宿主", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标外设() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标外设", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设可用性() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设可用性", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_运行状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("运行状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_启停状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("启停状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_占用状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("占用状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察场景() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察场景", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选存在集合() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选存在集合", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选存在数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选存在数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_空间候选观察事实() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选观察事实", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_观察诊断区域() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察诊断区域", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_已确认观察存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("已确认观察存在", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_安全因果候选条目() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选条目", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 名称_存在内部世界() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在内部世界", 枚举_信息入口类型::场景实例入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_存在内外接口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在内外接口", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_存在局部观察切片() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在局部观察切片", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_内部世界子候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子候选", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_内部世界子存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_叶子特征块存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("叶子特征块存在", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_观察存在匹配关系() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察存在匹配关系", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_观察存在变化结果() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察存在变化结果", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_观察变化事件() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察变化事件", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_当前帧轮廓分区项() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前帧轮廓分区项", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_当前场景特征值可读材料门项() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值可读材料门项", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_场景识别快照() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("场景识别快照", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_OpenGL复现数据() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OpenGL复现数据", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_相机帧宽度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("相机帧宽度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_相机帧高度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("相机帧高度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度帧号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度帧号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_彩色帧号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("彩色帧号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察帧() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察帧", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察特征帧() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察特征帧", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察特征帧取得状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察特征帧取得状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察帧存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察帧存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察帧存在状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察帧存在状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察帧容器状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察帧容器状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察帧到自我场景接口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察帧到自我场景接口", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前帧像素特征索引() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前帧像素特征索引", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素观察事实索引() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素观察事实索引", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素坐标X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素坐标X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素坐标Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素坐标Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素坐标对应像素观察事实() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素坐标对应像素观察事实", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_预期像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("预期像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素特征数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素特征数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素观察存在数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素观察存在数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_缺失像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("缺失像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_重复映射数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("重复映射数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_颜色像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("颜色像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度有效像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度有效像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_点云有效像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("点云有效像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素覆盖状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素覆盖状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素特征值承接状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素特征值承接状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_坐标回查可用状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("坐标回查可用状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_帧内编号回查可用状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("帧内编号回查可用状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察事实完备状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察事实完备状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察事实完备失败条件数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察事实完备失败条件数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_质量可用状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("质量可用状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_质量失败条件数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("质量失败条件数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察新鲜度状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察新鲜度状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察帧采集时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察帧采集时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察帧观察年龄() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察帧观察年龄", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最大允许观察年龄() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最大允许观察年龄", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_新鲜度判定时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("新鲜度判定时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察帧失效状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察帧失效状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察帧来源外设状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察帧来源外设状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_新鲜度失败原因() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("新鲜度失败原因", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_基础观察事实可用状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("基础观察事实可用状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_基础观察事实失败条件数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("基础观察事实失败条件数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间坐标单位毫米() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间坐标单位毫米", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_颜色RGB结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("颜色RGB结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_原始深度毫米结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("原始深度毫米结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_滤波深度毫米结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("滤波深度毫米结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_补全深度毫米结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("补全深度毫米结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度有效性Mask状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度有效性Mask状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度来源结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度来源结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度稳定性结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度稳定性结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度邻域一致性结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度邻域一致性结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间坐标毫米XYZ结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间坐标毫米XYZ结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_原始深度来源像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("原始深度来源像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_滤波深度来源像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("滤波深度来源像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_补全深度来源像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("补全深度来源像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_无有效深度来源像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("无有效深度来源像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_补全深度低置信像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("补全深度低置信像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度稳定性平均值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度稳定性平均值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度邻域一致性平均值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度邻域一致性平均值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察帧组状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察帧组状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察帧组帧数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察帧组帧数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合依据帧集合状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合依据帧集合状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合深度毫米结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合深度毫米结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合深度有效性结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合深度有效性结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_多帧深度稳定性结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("多帧深度稳定性结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_每像素深度方差结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("每像素深度方差结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合深度有效像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合深度有效像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合深度有效率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合深度有效率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合补偿深度空洞像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合补偿深度空洞像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合低稳定像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合低稳定像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合深度稳定性平均值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合深度稳定性平均值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合深度平均方差() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合深度平均方差", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_彩色深度已对齐() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("彩色深度已对齐", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_对齐目标() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("对齐目标", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度有效率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度有效率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间坐标有效率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间坐标有效率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_质量诊断状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("质量诊断状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度观察质量状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度观察质量状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间坐标观察质量状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间坐标观察质量状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_质量缺口状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("质量缺口状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_质量缺口原因集合() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("质量缺口原因集合", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间类后续处理适用状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间类后续处理适用状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险判断适用状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险判断适用状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在确认适用状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在确认适用状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察质量改善线索状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察质量改善线索状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察质量改善候选线索() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察质量改善候选线索", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_帧质量评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("帧质量评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察帧质量诊断摘要() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察帧质量诊断摘要", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度空洞数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度空洞数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度空洞区域数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度空洞区域数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度空洞区域集合() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度空洞区域集合", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域集合状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域集合状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域掩码状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域掩码状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域编号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域编号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域图层类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域图层类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域来源候选编号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域来源候选编号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域投影最小X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域投影最小X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域投影最大X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域投影最大X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域投影最小Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域投影最小Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域投影最大Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域投影最大Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断区域置信度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断区域置信度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_彩色深度对齐状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("彩色深度对齐状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓闭合率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓闭合率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓断裂数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓断裂数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_彩图轮廓数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("彩图轮廓数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度轮廓数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度轮廓数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间投影轮廓数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间投影轮廓数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合轮廓数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合轮廓数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓来源分层状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓来源分层状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓深度支持率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓深度支持率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓颜色支持率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓颜色支持率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓空间支持率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓空间支持率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_融合轮廓置信度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("融合轮廓置信度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_跨越深度断裂轮廓数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("跨越深度断裂轮廓数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_穿过深度无效区域轮廓数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("穿过深度无效区域轮廓数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_依赖补全深度轮廓数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("依赖补全深度轮廓数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间点有效率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间点有效率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_边界深度稳定率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("边界深度稳定率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选存在验证通过率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选存在验证通过率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_未解释区域比例() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("未解释区域比例", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_补观察缺口状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("补观察缺口状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_补观察缺失原因() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("补观察缺失原因", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_待补观察区域数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("待补观察区域数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_补全候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("补全候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_低置信轮廓数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("低置信轮廓数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_条件不足候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("条件不足候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_待验证候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("待验证候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_部分确认候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("部分确认候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_补观察需求建议() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("补观察需求建议", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_帧解释状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("帧解释状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选集合() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选集合", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选有效点数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选有效点数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_主空间候选编号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("主空间候选编号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_主空间候选像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("主空间候选像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选编号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选编号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选像素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选像素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选有效点比例() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选有效点比例", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界场景类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界场景类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_所属父存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("所属父存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_父场景() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("父场景", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_局部坐标原点X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("局部坐标原点X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_局部坐标原点Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("局部坐标原点Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_局部坐标原点Z() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("局部坐标原点Z", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在内外场景接口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在内外场景接口", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_接口状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("接口状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外部场景() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外部场景", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部场景() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部场景", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界当前观察材料() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界当前观察材料", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在内部世界局部材料建立状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在内部世界局部材料建立状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界局部轮廓材料可回查状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界局部轮廓材料可回查状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子候选验证状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子候选验证状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子存在候选确认状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在候选确认状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子存在候选确认方案取得状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在候选确认方案取得状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子存在候选确认方案类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在候选确认方案类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子存在候选确认允许提交状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在候选确认允许提交状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子存在候选确认新建数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在候选确认新建数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子存在候选确认证据不足数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在候选确认证据不足数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子存在候选确认可提交数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在候选确认可提交数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子存在事实提交状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在事实提交状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子存在原始事实写入状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在原始事实写入状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子存在与内部世界关联状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在与内部世界关联状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_局部观察切片() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("局部观察切片", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_来源局部观察切片() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源局部观察切片", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_局部边界框宽度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("局部边界框宽度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_局部边界框高度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("局部边界框高度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_局部像素索引状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("局部像素索引状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界子候选集合() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子候选集合", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_子候选类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("子候选类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_成员存在集合() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("成员存在集合", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_子存在类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("子存在类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内部世界细分状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界细分状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_细分层级数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("细分层级数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_不再细分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("不再细分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_可绘制状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("可绘制状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_历史观察存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("历史观察存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_上一观察存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("上一观察存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_历史对应状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("历史对应状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察存在匹配关系() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察存在匹配关系", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_中心坐标差() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("中心坐标差", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_范围坐标重叠率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("范围坐标重叠率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_尺寸差() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("尺寸差", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_匹配置信度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("匹配置信度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察存在变化结果() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察存在变化结果", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_变化状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("变化状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_位置变化量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("位置变化量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_范围变化量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("范围变化量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_尺寸变化量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("尺寸变化量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_距离变化量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("距离变化量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察变化事件集合() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察变化事件集合", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察变化事件类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察变化事件类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_变化事件置信度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("变化事件置信度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_场景识别快照() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("场景识别快照", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_快照状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("快照状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OpenGL复现数据() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OpenGL复现数据", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_复现数据状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("复现数据状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素颜色层() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素颜色层", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度层() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度层", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间点层() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间点层", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选层() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选层", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_已确认观察存在层() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("已确认观察存在层", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在内部世界展开层() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在内部世界展开层", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_变化事件层() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("变化事件层", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_诊断摘要状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("诊断摘要状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_顶点缓冲() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("顶点缓冲", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_颜色缓冲() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("颜色缓冲", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度缓冲() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度缓冲", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在ID缓冲() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在ID缓冲", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_AABB绘制状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("AABB绘制状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_绘制顶点数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("绘制顶点数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_绘制线段数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("绘制线段数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_绘制点数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("绘制点数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_中心空间坐标X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("中心空间坐标X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_中心空间坐标Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("中心空间坐标Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_中心空间坐标Z() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("中心空间坐标Z", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_范围坐标AABB最小X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("范围坐标AABB最小X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_范围坐标AABB最大X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("范围坐标AABB最大X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_范围坐标AABB最小Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("范围坐标AABB最小Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_范围坐标AABB最大Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("范围坐标AABB最大Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_范围坐标AABB最小Z() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("范围坐标AABB最小Z", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_范围坐标AABB最大Z() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("范围坐标AABB最大Z", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_投影范围最小X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("投影范围最小X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_投影范围最大X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("投影范围最大X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_投影范围最小Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("投影范围最小Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_投影范围最大Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("投影范围最大Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_平均深度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("平均深度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最近深度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最近深度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最远深度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最远深度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间连续性评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间连续性评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_范围稳定性评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("范围稳定性评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选相对独立性判断() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选相对独立性判断", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_相对独立空间候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("相对独立空间候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_不独立空间候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("不独立空间候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_待拆分空间候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("待拆分空间候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_待合并空间候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("待合并空间候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_不确定空间候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("不确定空间候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_主空间候选相对独立状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("主空间候选相对独立状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_主空间候选分离度评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("主空间候选分离度评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选观察队列() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选观察队列", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察队列候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察队列候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_首选观察候选编号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("首选观察候选编号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_首选观察候选优先级评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("首选观察候选优先级评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_首选观察候选相对独立状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("首选观察候选相对独立状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_首选观察候选中心接近评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("首选观察候选中心接近评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_首选观察候选范围大小评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("首选观察候选范围大小评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察队列排序位次() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察队列排序位次", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选优先级评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选优先级评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选中心接近评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选中心接近评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选范围大小评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选范围大小评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间候选相对独立状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间候选相对独立状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察存在假设() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察存在假设", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_来源空间候选编号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源空间候选编号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在假设验证状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在假设验证状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选像素掩码状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选像素掩码状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_投影轮廓状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("投影轮廓状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_假设距离() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("假设距离", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_假设尺寸X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("假设尺寸X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_假设尺寸Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("假设尺寸Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_假设尺寸Z() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("假设尺寸Z", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_假设方位X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("假设方位X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_假设方位Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("假设方位Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_假设有效点比例() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("假设有效点比例", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓支持评分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓支持评分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓来源类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓来源类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓闭合度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓闭合度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素归属验证结果() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素归属验证结果", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间覆盖像素数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间覆盖像素数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间匹配像素数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间匹配像素数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_空间冲突像素数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("空间冲突像素数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓支持像素数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓支持像素数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓冲突像素数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓冲突像素数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度一致率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度一致率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_范围内点比例() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("范围内点比例", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_投影覆盖率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("投影覆盖率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓吻合率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓吻合率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓跨越深度断裂() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓跨越深度断裂", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓穿过深度无效区域() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓穿过深度无效区域", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓依赖补全深度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓依赖补全深度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓置信度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓置信度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_遮挡像素数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("遮挡像素数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_未解释像素数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("未解释像素数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_已归属像素数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("已归属像素数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_归属冲突像素数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("归属冲突像素数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素归属率() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素归属率", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_全帧像素归属账状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("全帧像素归属账状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素归属验证状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素归属验证状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察报告ID() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察报告ID", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察像素簇ID() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察像素簇ID", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察像素簇候选已验证状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察像素簇候选已验证状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设像素簇候选验证状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设像素簇候选验证状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前观察候选存在验证状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前观察候选存在验证状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_已确认观察存在生成条件明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("已确认观察存在生成条件明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察验证材料明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察验证材料明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察材料可回查状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察材料可回查状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察验证候选簇数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察验证候选簇数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察验证通过簇数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察验证通过簇数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察验证失败簇数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察验证失败簇数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察验证证据不足簇数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察验证证据不足簇数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察验证缺失材料数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察验证缺失材料数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察验证缺失材料原因() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察验证缺失材料原因", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察存在候选确认状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在候选确认状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察存在候选确认方案取得状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在候选确认方案取得状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察存在候选确认方案类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在候选确认方案类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察存在候选确认允许提交状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在候选确认允许提交状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察存在候选确认新建数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在候选确认新建数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察存在候选确认合并数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在候选确认合并数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察存在候选确认冲突数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在候选确认冲突数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察存在候选确认证据不足数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在候选确认证据不足数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察存在候选确认可提交数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在候选确认可提交数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标已确认观察存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标已确认观察存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设观察存在事实提交状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在事实提交状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_已确认观察存在提交入账状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("已确认观察存在提交入账状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察存在原始事实写入状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察存在原始事实写入状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察存在与当前场景关联状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察存在与当前场景关联状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前帧轮廓分区状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前帧轮廓分区状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_已知存在当前帧扫描状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("已知存在当前帧扫描状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_未知区域识别状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("未知区域识别状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前帧遮挡判断状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前帧遮挡判断状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前帧轮廓冲突区域明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前帧轮廓冲突区域明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前帧像素归属闭合状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前帧像素归属闭合状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前帧区域归属状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前帧区域归属状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_已知存在扫描区数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("已知存在扫描区数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_新识别区数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("新识别区数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_冲突区数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("冲突区数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_待重识别区数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("待重识别区数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓分区项序号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓分区项序号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_来源已确认观察存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源已确认观察存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_匹配外设观察像素簇ID() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("匹配外设观察像素簇ID", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓匹配分数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓匹配分数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素残差() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素残差", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_深度残差() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("深度残差", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_轮廓残差() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("轮廓残差", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察确认补偿状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察确认补偿状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_缺失原因() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("缺失原因", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_未解释区域状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("未解释区域状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_补全候选状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("补全候选状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_低置信轮廓状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("低置信轮廓状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_已验证观察存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("已验证观察存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_观察存在确认状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("观察存在确认状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_已验证观察存在数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("已验证观察存在数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_像素归属更新() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("像素归属更新", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_未解释区域更新() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("未解释区域更新", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标抽象特征() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标抽象特征", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标值类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标值类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_差值约束包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("差值约束包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_执行差值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行差值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标执行后值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标执行后值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_执行前值快照() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行前值快照", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_特征类型配对结果() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("特征类型配对结果", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_实例缺失策略() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("实例缺失策略", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_溢出策略() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("溢出策略", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_限制包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("限制包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标实例特征() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标实例特征", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_实际执行差值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("实际执行差值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_差值命中状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("差值命中状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_是否发生溢出() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("是否发生溢出", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前命中抽象特征() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前命中抽象特征", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_派生需求() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("派生需求", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_派生需求当前状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("派生需求当前状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_派生需求目标状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("派生需求目标状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标方法头节点() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标方法头节点", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_来源缺口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源缺口", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法补齐尝试结果() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法补齐尝试结果", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法补齐状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法补齐状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_试运行次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("试运行次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_条件结果对() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("条件结果对", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_试运行限制包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("试运行限制包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_练习模式() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("练习模式", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_练习方向() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("练习方向", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选实参表() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选实参表", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选实参数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选实参数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_参数生成序号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("参数生成序号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_实参生成策略() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("实参生成策略", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_参数新颖性状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("参数新颖性状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法补齐类别() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法补齐类别", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    enum class 枚举_方法补齐类别 : I64 {
        未定义 = 0,
        条件结果对补齐 = 1,
        候选动作补齐 = 2,
        实参与验证补齐 = 3,
    };

    inline constexpr I64 方法补齐类别值(枚举_方法补齐类别 类别) noexcept
    {
        return static_cast<I64>(类别);
    }

    inline const 语素入口节点类* 特征_来源因果信息() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源因果信息", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_来源观察动态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源观察动态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_动作主体() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("动作主体", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选动作节点() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选动作节点", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选方法能力包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选方法能力包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选形参包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选形参包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_候选结果包() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选结果包", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_预测状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("预测状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_内化状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内化状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_动作主体替换状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("动作主体替换状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法补齐结构状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法补齐结构状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法动作状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法动作状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法可执行入口数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法可执行入口数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法条件结果配对状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法条件结果配对状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法运行账完整性() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法运行账完整性", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法稳定状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法稳定状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    // 方法状态必须使用 枚举_方法状态 的连续取值；旧的 10/20/60 阶梯值会破坏需求目标满足判断。
    inline constexpr I64 方法状态_待方法动作() noexcept { return static_cast<I64>(枚举_方法状态::待方法动作); }
    inline constexpr I64 方法状态_待可执行入口() noexcept { return static_cast<I64>(枚举_方法状态::待可执行入口); }
    inline constexpr I64 方法状态_待条件节点() noexcept { return static_cast<I64>(枚举_方法状态::待条件节点); }
    inline constexpr I64 方法状态_有条件节点() noexcept { return static_cast<I64>(枚举_方法状态::待结果节点); }
    inline constexpr I64 方法状态_待条件结果对() noexcept { return static_cast<I64>(枚举_方法状态::待条件结果对); }
    inline constexpr I64 方法状态_有条件结果() noexcept { return static_cast<I64>(枚举_方法状态::可用); }
    inline constexpr I64 方法状态_待运行账() noexcept { return 方法状态_有条件结果(); }
    inline constexpr I64 方法状态_可用() noexcept { return static_cast<I64>(枚举_方法状态::可用); }
    inline constexpr I64 方法状态_好用() noexcept { return static_cast<I64>(枚举_方法状态::好用); }

    inline const 语素入口节点类* 特征_需求满足情况() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求满足情况", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_满足证据() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("满足证据", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前交互者明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前交互者明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前人类服务对象明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前人类服务对象明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前权限者明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前权限者明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前服务目标明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前服务目标明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_沟通交流需求明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("沟通交流需求明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_服务对象反馈信息获取尝试状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("服务对象反馈信息获取尝试状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_自我运行状态明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("自我运行状态明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_自我运行状态正常状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("自我运行状态正常状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_资源状态明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("资源状态明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_资源状态足够状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("资源状态足够状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_权限边界明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("权限边界明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险状态明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险状态明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险状态可控状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险状态可控状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_基础风险判断状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("基础风险判断状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_场景存在集合明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("场景存在集合明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景存在集合明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景存在集合明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景各存在特征值可读状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景各存在特征值可读状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值明确范围类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值明确范围类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值缺口可解释状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值缺口可解释状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值可读材料门版本() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值可读材料门版本", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值可读存在数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值可读存在数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值部分可读存在数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值部分可读存在数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值证据不足存在数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值证据不足存在数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值材料缺失存在数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值材料缺失存在数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值材料过期存在数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值材料过期存在数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值材料冲突存在数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值材料冲突存在数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值可读特征类型掩码() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值可读特征类型掩码", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值缺失原因掩码() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值缺失原因掩码", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值过期原因掩码() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值过期原因掩码", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值冲突原因掩码() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值冲突原因掩码", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景特征值材料门序号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景特征值材料门序号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_来源报告时间戳毫秒() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源报告时间戳毫秒", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在_场景绝对坐标X() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在_场景绝对坐标X", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在_场景绝对坐标Y() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在_场景绝对坐标Y", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在_场景绝对坐标Z() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在_场景绝对坐标Z", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在_场景绝对坐标明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在_场景绝对坐标明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在_场景绝对空间范围明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在_场景绝对空间范围明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_外设_D455到场景绝对坐标变换明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设_D455到场景绝对坐标变换明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_关键特征变化观察状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("关键特征变化观察状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选构造状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选构造状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全影响因果明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全影响因果明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_关键特征观察样本数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("关键特征观察样本数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_关键特征负向变化数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("关键特征负向变化数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选低置信数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选低置信数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选归因置信度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选归因置信度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选序号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选序号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选特征类型() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选特征类型", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选当前特征值读取状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选当前特征值读取状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选当前特征值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选当前特征值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选前一特征值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选前一特征值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选前一特征值缺口状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选前一特征值缺口状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选变化方向() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选变化方向", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选时间窗口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选时间窗口", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选来源观察报告() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选来源观察报告", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选相关自我关键特征() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选相关自我关键特征", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选归因证据状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选归因证据状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选负向证据状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选负向证据状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选置信度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选置信度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选材料缺口数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选材料缺口数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果候选搜索未完成数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果候选搜索未完成数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景安全度明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景安全度明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景最低安全度存在明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景最低安全度存在明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景各存在安全评估明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景各存在安全评估明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景持续观察需求明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景持续观察需求明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景持续观察状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景持续观察状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景持续观察样本数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景持续观察样本数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_负向影响处置需求明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("负向影响处置需求明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景安全度候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景安全度候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景评估安全值候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景评估安全值候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景直接伤害可能性候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景直接伤害可能性候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全_场景影响部分候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全_场景影响部分候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全_场景影响部分可结算状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全_场景影响部分可结算状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全_场景影响部分不可结算原因() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全_场景影响部分不可结算原因", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全_场景影响部分已入账状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全_场景影响部分已入账状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全_场景影响部分变化方向() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全_场景影响部分变化方向", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全_场景影响部分提交状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全_场景影响部分提交状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全_场景影响部分提交拒绝原因() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全_场景影响部分提交拒绝原因", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_自我_风险安全_场景影响部分() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("自我_风险安全_场景影响部分", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全层因素集合版本() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全层因素集合版本", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全层因素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全层因素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全层缺失因素数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全层缺失因素数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全层明确状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全层明确状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全层可聚合状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全层可聚合状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全层已入账状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全层已入账状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险因素未搜索掩码() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险因素未搜索掩码", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险因素搜索缺口掩码() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险因素搜索缺口掩码", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险因素负向证据掩码() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险因素负向证据掩码", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险因素默认满足掩码() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险因素默认满足掩码", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险因素已入账掩码() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险因素已入账掩码", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险因素证据搜索未完成数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险因素证据搜索未完成数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险因素默认满足数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险因素默认满足数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全层候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全层候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险安全层投影候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险安全层投影候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果层级() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果层级", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_目标安全因果信息() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标安全因果信息", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素ID() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素ID", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果证据方向() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果证据方向", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果搜索时间窗口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果搜索时间窗口", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素证据搜索状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素证据搜索状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素搜索范围完整状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素搜索范围完整状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素负向证据数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素负向证据数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素正向证据数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素正向证据数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素稳定性证据数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素稳定性证据数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素归因证据数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素归因证据数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素材料缺失数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素材料缺失数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素冲突数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素冲突数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素搜索置信度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素搜索置信度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素默认满足允许状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素默认满足允许状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素默认满足提交状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素默认满足提交状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素评估值候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素评估值候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素评估值提交状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素评估值提交状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全因果因素已入账状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全因果因素已入账状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景最低安全度存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景最低安全度存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景安全评估置信度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景安全评估置信度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景安全判定状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景安全判定状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_当前场景安全判定来源() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("当前场景安全判定来源", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在直接伤害可能性候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在直接伤害可能性候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在风险安全值候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在风险安全值候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在安全度候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在安全度候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_历史直接伤害归因次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("历史直接伤害归因次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_最近直接伤害归因时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("最近直接伤害归因时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_无伤害共处时长() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("无伤害共处时长", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_伤害可能性下界候选() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("伤害可能性下界候选", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在安全评估置信度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在安全评估置信度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在安全评估证据不足原因() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在安全评估证据不足原因", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在安全判定状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在安全判定状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_存在安全判定来源() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("存在安全判定来源", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全评估存在序号() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全评估存在序号", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_被评估存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("被评估存在", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_基础风险未判断因子数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("基础风险未判断因子数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_基础风险证据完整度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("基础风险证据完整度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_基础风险不确定度() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("基础风险不确定度", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_基础风险值计算状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("基础风险值计算状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_基础风险评估范围状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("基础风险评估范围状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_安全评估证据不足原因() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全评估证据不足原因", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_安全评估存在结果() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("安全评估存在结果", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_外设观察像素簇验证结果() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察像素簇验证结果", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_外设观察存在候选确认方案() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设观察存在候选确认方案", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_内部世界子存在确认方案() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("内部世界子存在确认方案", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_服务前状态场景() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("服务前状态场景", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_降级保护状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("降级保护状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_资源压力降低状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("资源压力降低状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_风险传播阻断状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("风险传播阻断状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_非必要任务暂停状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("非必要任务暂停状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_目标实例特征缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标实例特征缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行前值缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行前值缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_输入快照过期() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("输入快照过期", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_I64溢出() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("I64溢出", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_差值计算溢出() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("差值计算溢出", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_缺少执行差值或目标值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("缺少执行差值或目标值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_执行差值与目标值冲突() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("执行差值与目标值冲突", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_实际差值未命中约束() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("实际差值未命中约束", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_目标特征写入失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标特征写入失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_差值约束包缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("差值约束包缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_方法配对结果缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法配对结果缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_可执行输入参数场景创建失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("可执行输入参数场景创建失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_必需参数缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("必需参数缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_目标方法头节点缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标方法头节点缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务状态动作动态创建失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务状态动作动态创建失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务状态动作动态_提交方法首节点缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务状态动作动态_提交方法首节点缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务状态动作动态_任务节点缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务状态动作动态_任务节点缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务状态动作动态_任务场景缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务状态动作动态_任务场景缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务状态动作动态_任务虚拟存在缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务状态动作动态_任务虚拟存在缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务状态动作动态_任务状态特征缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务状态动作动态_任务状态特征缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务状态动作动态_来源方法入口无效() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务状态动作动态_来源方法入口无效", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_任务状态动作动态_动态节点创建失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("任务状态动作动态_动态节点创建失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_方法域动作动态创建失败() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法域动作动态创建失败", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_因果证据不足() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("因果证据不足", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_自我动作缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("自我动作缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_候选动作未验证() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("候选动作未验证", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_目标方法尚未好用() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标方法尚未好用", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_目标特征缺失() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标特征缺失", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_来源_任务提供参数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源_任务提供参数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_来源_练习自动生成() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源_练习自动生成", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_参数条件缺口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("参数条件缺口", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_缺少执行入口() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("缺少执行入口", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_需求满足() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求满足", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_需求未满足() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求未满足", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_目标抽象特征无效() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标抽象特征无效", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_实例特征未命中抽象特征() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("实例特征未命中抽象特征", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 值_目标值类型不是I64() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("目标值类型不是I64", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_需求_实例化特征() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求_实例化特征", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_需求_确定特征当前值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求_确定特征当前值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_需求_确定执行差值或目标值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求_确定执行差值或目标值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_需求_重新配对参数或调整目标值() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求_重新配对参数或调整目标值", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_需求_修复实例特征写入能力() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("需求_修复实例特征写入能力", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }
    inline bool 写入I64(
        方法类::节点类* 方法首节点,
        const 语素入口节点类* 特征类型,
        I64 值,
        时间戳 now) noexcept
    {
        return 方法首节点
            && 特征类型
            && 方法虚拟存在服务类::记录方法虚拟存在特征(
                方法首节点,
                特征类型,
                特征值{ 值 },
                nullptr,
                now);
    }

    inline bool 写入存在I64(
        存在节点类* 存在,
        const 语素入口节点类* 特征类型,
        I64 值,
        时间戳 now) noexcept
    {
        return 存在
            && 特征类型
            && 世界树.写入特征_I64(
                reinterpret_cast<基础信息节点类*>(存在),
                特征类型,
                值,
                now);
    }

    inline bool 写入存在指针(
        存在节点类* 存在,
        const 语素入口节点类* 特征类型,
        const void* 指针,
        时间戳 now) noexcept
    {
        return 存在
            && 特征类型
            && 指针
            && 世界树.写入特征_指针(
                reinterpret_cast<基础信息节点类*>(存在),
                特征类型,
                指针,
                now);
    }

    inline bool 写入基础节点指针(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        const void* 指针,
        时间戳 now) noexcept
    {
        return 宿主
            && 特征类型
            && 指针
            && 世界树.写入特征_指针(宿主, 特征类型, 指针, now);
    }

    inline bool 写入基础节点I64(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        I64 值,
        时间戳 now) noexcept
    {
        return 宿主
            && 特征类型
            && 世界树.写入特征_I64(宿主, 特征类型, 值, now);
    }

    inline bool 写入基础节点特征值(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        const 特征值& 值,
        时间戳 now) noexcept
    {
        auto* 特征 = 宿主 && 特征类型
            ? 世界树.取或创建子特征_按类型(宿主, 特征类型)
            : nullptr;
        return 特征 && 世界树.特征().写入特征值(特征, 值, now);
    }

    inline 特征节点类* 查找子特征(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型) noexcept
    {
        return 宿主 && 特征类型
            ? 世界树.特征().查找子特征_按类型(宿主, 特征类型)
            : nullptr;
    }

    inline 特征节点类* 取或创建子特征(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型) noexcept
    {
        return 宿主 && 特征类型
            ? 世界树.取或创建子特征_按类型(宿主, 特征类型)
            : nullptr;
    }

    inline bool 读取基础节点I64(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        I64& 输出值) noexcept
    {
        输出值 = 0;
        if (!宿主 || !特征类型) return false;
        auto* 特征 = 查找子特征(宿主, 特征类型);
        return 特征 && 世界树.读取特征_I64(特征, 输出值);
    }

    inline bool 读取基础节点特征值(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        特征值& 输出值) noexcept
    {
        输出值 = {};
        if (!宿主 || !特征类型) return false;
        auto* 特征 = 查找子特征(宿主, 特征类型);
        if (!特征) return false;
        输出值 = 世界树.特征().读取特征值(特征);
        return 特征值有效(输出值);
    }

    inline bool 读取基础节点指针(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        void*& 输出指针) noexcept
    {
        输出指针 = nullptr;
        if (!宿主 || !特征类型) return false;
        auto* 特征 = 查找子特征(宿主, 特征类型);
        return 特征 && 世界树.读取特征_指针(特征, 输出指针) && 输出指针;
    }

    inline void 清理执行输出派生需求标记(
        场景节点类* 输出结果场景,
        const char* 调用方) noexcept
    {
        auto* 宿主 = reinterpret_cast<基础信息节点类*>(输出结果场景);
        if (!宿主) return;

        int 清理数量 = 0;
        auto 清理特征 = [&](const 语素入口节点类* 特征类型) noexcept {
            auto* 特征 = 查找子特征(宿主, 特征类型);
            if (特征 && 世界树.特征().删除特征(特征)) {
                ++清理数量;
            }
        };
        清理特征(特征_派生需求());
        清理特征(特征_派生需求当前状态());
        清理特征(特征_派生需求目标状态());

        if (清理数量 > 0) {
            std::ostringstream 输出;
            输出 << "自我动作/执行输出派生需求标记已清理"
                << " | 调用方=" << (调用方 ? 调用方 : "空")
                << " | 输出场景=" << 输出结果场景->获取主键()
                << " | 清理数量=" << 清理数量;
            项目运行日志(输出.str());
        }
    }

    inline bool 读取场景I64特征(
        场景节点类* 场景,
        const 语素入口节点类* 特征类型,
        I64& 输出值) noexcept
    {
        return 读取基础节点I64(
            reinterpret_cast<基础信息节点类*>(场景),
            特征类型,
            输出值);
    }

    inline 特征节点类* 输入包根(
        场景节点类* 输入参数场景,
        const 语素入口节点类* 包类型) noexcept
    {
        return 输入参数场景 && 包类型
            ? 查找子特征(
                reinterpret_cast<基础信息节点类*>(输入参数场景),
                包类型)
            : nullptr;
    }

    inline bool 读取输入包I64(
        场景节点类* 输入参数场景,
        const 语素入口节点类* 包类型,
        const 语素入口节点类* 参数类型,
        I64& 输出值) noexcept
    {
        auto* 包根 = 输入包根(输入参数场景, 包类型);
        return 包根
            && 读取基础节点I64(
                reinterpret_cast<基础信息节点类*>(包根),
                参数类型,
                输出值);
    }

    inline bool 读取输入包指针(
        场景节点类* 输入参数场景,
        const 语素入口节点类* 包类型,
        const 语素入口节点类* 参数类型,
        void*& 输出指针) noexcept
    {
        auto* 包根 = 输入包根(输入参数场景, 包类型);
        return 包根
            && 读取基础节点指针(
                reinterpret_cast<基础信息节点类*>(包根),
                参数类型,
                输出指针);
    }

    inline 特征节点类* 输入条件包根(场景节点类* 输入参数场景) noexcept
    {
        if (!输入参数场景) return nullptr;
        auto* 宿主 = reinterpret_cast<基础信息节点类*>(输入参数场景);
        if (auto* 条件包 = 查找子特征(宿主, 特征_输入条件包())) return 条件包;
        if (auto* 条件包 = 查找子特征(宿主, 特征_条件集合())) return 条件包;
        return 查找子特征(宿主, 特征_运行参数());
    }

    inline bool 读取调用信息指针(
        场景节点类* 输入参数场景,
        const 语素入口节点类* 参数类型,
        void*& 输出指针) noexcept
    {
        return 读取输入包指针(
            输入参数场景,
            特征_调用信息(),
            参数类型,
            输出指针);
    }

    inline bool 读取输入参数指针(
        场景节点类* 输入参数场景,
        const 语素入口节点类* 参数类型,
        void*& 输出指针) noexcept
    {
        输出指针 = nullptr;
        if (!输入参数场景 || !参数类型) return false;

        auto* 输入宿主 = reinterpret_cast<基础信息节点类*>(输入参数场景);
        if (读取基础节点指针(输入宿主, 参数类型, 输出指针)) return true;

        if (auto* 条件包 = 输入条件包根(输入参数场景)) {
            if (读取基础节点指针(
                    reinterpret_cast<基础信息节点类*>(条件包),
                    参数类型,
                    输出指针)) {
                return true;
            }
        }
        if (读取输入包指针(输入参数场景, 特征_任务参数包(), 参数类型, 输出指针)) return true;
        if (读取输入包指针(输入参数场景, 特征_现实场景包(), 参数类型, 输出指针)) return true;
        return 读取调用信息指针(输入参数场景, 参数类型, 输出指针);
    }

    inline bool 读取输入参数I64(
        场景节点类* 输入参数场景,
        const 语素入口节点类* 参数类型,
        I64& 输出值) noexcept
    {
        输出值 = 0;
        if (!输入参数场景 || !参数类型) return false;

        auto* 输入宿主 = reinterpret_cast<基础信息节点类*>(输入参数场景);
        if (读取基础节点I64(输入宿主, 参数类型, 输出值)) return true;

        if (auto* 条件包 = 输入条件包根(输入参数场景)) {
            if (读取基础节点I64(
                    reinterpret_cast<基础信息节点类*>(条件包),
                    参数类型,
                    输出值)) {
                return true;
            }
        }
        if (读取输入包I64(输入参数场景, 特征_任务参数包(), 参数类型, 输出值)) return true;
        if (读取输入包I64(输入参数场景, 特征_现实场景包(), 参数类型, 输出值)) return true;
        return 读取输入包I64(输入参数场景, 特征_调用信息(), 参数类型, 输出值);
    }

    inline 存在节点类* 方法虚拟存在(方法类::节点类* 方法首节点, 时间戳 now) noexcept
    {
        if (!方法首节点) return nullptr;
        return 方法首节点->主信息.首节点信息().方法虚拟存在.指针
            ? 方法首节点->主信息.首节点信息().方法虚拟存在.指针
            : 方法虚拟存在服务类::取或创建方法虚拟存在(方法首节点, nullptr, now);
    }

    inline bool 写入目标方法I64特征(
        方法类::节点类* 目标方法,
        const 语素入口节点类* 特征类型,
        I64 值,
        时间戳 now) noexcept
    {
        auto* 目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        return 目标方法存在
            && 写入基础节点I64(
                reinterpret_cast<基础信息节点类*>(目标方法存在),
                特征类型,
                值,
                now);
    }

    inline void 写入目标方法特征派生需求(
        方法类::节点类* 当前本能方法,
        方法类::节点类* 目标方法,
        const 语素入口节点类* 特征类型,
        const 特征值& 当前状态值,
        const 特征值& 目标状态值,
        场景节点类*& 输出结果场景,
        时间戳 now) noexcept
    {
        auto* 目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        if (!目标方法存在 || !特征类型) {
            std::ostringstream 输出;
            输出 << "自我动作/目标方法派生需求写入跳过"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string{})
                << " | 方法虚拟存在=" << (目标方法存在 ? 目标方法存在->获取主键() : std::string{})
                << " | 特征类型=" << (特征类型 ? 特征类型->获取主键() : std::string{});
            项目运行日志(输出.str());
            return;
        }

        auto* 目标主体 = reinterpret_cast<基础信息节点类*>(目标方法存在);
        auto* 状态特征 = 世界树.确保特征(目标主体, 特征类型);
        if (!状态特征) {
            std::ostringstream 输出;
            输出 << "自我动作/目标方法派生需求写入跳过"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string{})
                << " | 方法虚拟存在=" << 目标方法存在->获取主键()
                << " | 特征类型=" << 特征类型->获取主键()
                << " | 原因=状态特征创建失败";
            项目运行日志(输出.str());
            return;
        }

        (void)写入基础节点特征值(目标主体, 特征类型, 当前状态值, now);

        auto* 状态场景 = 输出结果场景;
        if (!状态场景 && 当前本能方法) {
            状态场景 = 方法类::取或创建_方法结果场景(
                当前本能方法,
                nullptr,
                now);
        }
        if (!状态场景) {
            状态场景 = 世界树.取或创建自我现实场景();
        }
        if (!状态场景) {
            std::ostringstream 输出;
            输出 << "自我动作/目标方法派生需求写入跳过"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string{})
                << " | 方法虚拟存在=" << 目标方法存在->获取主键()
                << " | 特征类型=" << 特征类型->获取主键()
                << " | 原因=状态场景缺失";
            项目运行日志(输出.str());
            return;
        }
        输出结果场景 = 状态场景;

        auto* 当前状态 = 世界树.状态().创建内部状态(
            状态场景,
            目标主体,
            状态特征,
            当前状态值,
            枚举_存在状态事件::创建,
            false,
            now);
        auto* 目标状态 = 世界树.状态().创建内部状态(
            状态场景,
            目标主体,
            状态特征,
            目标状态值,
            枚举_存在状态事件::变化,
            当前状态值 != 目标状态值,
            now);

        auto* 场景宿主 = reinterpret_cast<基础信息节点类*>(状态场景);
        (void)世界树.写入特征_指针(场景宿主, 特征_派生需求(), 特征类型, now);
        if (当前状态) {
            (void)世界树.写入特征_指针(场景宿主, 特征_派生需求当前状态(), 当前状态, now);
        }
        if (目标状态) {
            (void)世界树.写入特征_指针(场景宿主, 特征_派生需求目标状态(), 目标状态, now);
        }
        {
            std::ostringstream 输出;
            输出 << "自我动作/目标方法派生需求已写入"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string{})
                << " | 方法虚拟存在=" << 目标方法存在->获取主键()
                << " | 输出场景=" << 状态场景->获取主键()
                << " | 特征类型=" << 特征类型->获取主键()
                << " | 当前状态=" << (当前状态 ? 当前状态->获取主键() : std::string{})
                << " | 目标状态=" << (目标状态 ? 目标状态->获取主键() : std::string{});
            项目运行日志(输出.str());
        }
    }

    inline void 写入目标方法状态派生需求(
        方法类::节点类* 当前本能方法,
        方法类::节点类* 目标方法,
        I64 当前状态值,
        I64 目标状态值,
        场景节点类*& 输出结果场景,
        时间戳 now) noexcept
    {
        写入目标方法特征派生需求(
            当前本能方法,
            目标方法,
            特征_方法状态(),
            特征值{ 当前状态值 },
            特征值{ 目标状态值 },
            输出结果场景,
            now);
    }

    inline void 写入目标方法I64特征派生需求(
        方法类::节点类* 当前本能方法,
        方法类::节点类* 目标方法,
        const 语素入口节点类* 特征类型,
        I64 当前状态值,
        I64 目标状态值,
        场景节点类*& 输出结果场景,
        时间戳 now) noexcept
    {
        写入目标方法特征派生需求(
            当前本能方法,
            目标方法,
            特征类型,
            特征值{ 当前状态值 },
            特征值{ 目标状态值 },
            输出结果场景,
            now);
    }

    inline void 写入宿主I64特征派生需求(
        方法类::节点类* 当前本能方法,
        基础信息节点类* 目标主体,
        const 语素入口节点类* 特征类型,
        I64 当前状态值,
        I64 目标状态值,
        场景节点类*& 输出结果场景,
        时间戳 now,
        const char* 来源) noexcept
    {
        if (!目标主体 || !特征类型) {
            std::ostringstream 输出;
            输出 << "自我动作/宿主派生需求写入跳过"
                << " | 来源=" << (来源 ? 来源 : "空")
                << " | 目标主体=" << (目标主体 ? 目标主体->获取主键() : std::string{})
                << " | 特征类型=" << (特征类型 ? 特征类型->获取主键() : std::string{})
                << " | 原因=目标主体或特征缺失";
            项目运行日志(输出.str());
            return;
        }

        auto* 状态特征 = 世界树.确保特征(目标主体, 特征类型);
        if (!状态特征) {
            std::ostringstream 输出;
            输出 << "自我动作/宿主派生需求写入跳过"
                << " | 来源=" << (来源 ? 来源 : "空")
                << " | 目标主体=" << 目标主体->获取主键()
                << " | 特征类型=" << 特征类型->获取主键()
                << " | 原因=状态特征创建失败";
            项目运行日志(输出.str());
            return;
        }

        (void)写入基础节点I64(目标主体, 特征类型, 当前状态值, now);

        auto* 状态场景 = 输出结果场景;
        if (!状态场景 && 当前本能方法) {
            状态场景 = 方法类::取或创建_方法结果场景(
                当前本能方法,
                nullptr,
                now);
        }
        if (!状态场景) {
            状态场景 = 世界树.取或创建自我现实场景();
        }
        if (!状态场景) {
            std::ostringstream 输出;
            输出 << "自我动作/宿主派生需求写入跳过"
                << " | 来源=" << (来源 ? 来源 : "空")
                << " | 目标主体=" << 目标主体->获取主键()
                << " | 特征类型=" << 特征类型->获取主键()
                << " | 原因=状态场景缺失";
            项目运行日志(输出.str());
            return;
        }
        输出结果场景 = 状态场景;

        auto* 当前状态 = 世界树.状态().创建内部状态(
            状态场景,
            目标主体,
            状态特征,
            特征值{ 当前状态值 },
            枚举_存在状态事件::创建,
            false,
            now);
        auto* 目标状态 = 世界树.状态().创建内部状态(
            状态场景,
            目标主体,
            状态特征,
            特征值{ 目标状态值 },
            枚举_存在状态事件::变化,
            当前状态值 != 目标状态值,
            now);

        auto* 场景宿主 = reinterpret_cast<基础信息节点类*>(状态场景);
        (void)世界树.写入特征_指针(场景宿主, 特征_派生需求(), 特征类型, now);
        if (当前状态) {
            (void)世界树.写入特征_指针(场景宿主, 特征_派生需求当前状态(), 当前状态, now);
        }
        if (目标状态) {
            (void)世界树.写入特征_指针(场景宿主, 特征_派生需求目标状态(), 目标状态, now);
        }
        {
            std::ostringstream 输出;
            输出 << "自我动作/宿主派生需求已写入"
                << " | 来源=" << (来源 ? 来源 : "空")
                << " | 目标主体=" << 目标主体->获取主键()
                << " | 输出场景=" << 状态场景->获取主键()
                << " | 特征类型=" << 特征类型->获取主键()
                << " | 当前状态=" << (当前状态 ? 当前状态->获取主键() : std::string{})
                << " | 目标状态=" << (目标状态 ? 目标状态->获取主键() : std::string{});
            项目运行日志(输出.str());
        }
    }

    inline 状态节点类* 写入目标方法I64实际结果状态(
        方法类::节点类* 当前本能方法,
        方法类::节点类* 目标方法,
        const 语素入口节点类* 特征类型,
        I64 实际状态值,
        场景节点类*& 输出结果场景,
        时间戳 now) noexcept
    {
        auto* 目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        if (!目标方法存在 || !特征类型) {
            std::ostringstream 输出;
            输出 << "自我动作/目标方法实际结果写入跳过"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string{})
                << " | 方法虚拟存在=" << (目标方法存在 ? 目标方法存在->获取主键() : std::string{})
                << " | 特征类型=" << (特征类型 ? 特征类型->获取主键() : std::string{});
            项目运行日志(输出.str());
            return nullptr;
        }

        auto* 目标主体 = reinterpret_cast<基础信息节点类*>(目标方法存在);
        auto* 状态特征 = 世界树.确保特征(目标主体, 特征类型);
        if (!状态特征) {
            std::ostringstream 输出;
            输出 << "自我动作/目标方法实际结果写入跳过"
                << " | 目标方法=" << 目标方法->获取主键()
                << " | 方法虚拟存在=" << 目标方法存在->获取主键()
                << " | 特征类型=" << 特征类型->获取主键()
                << " | 原因=状态特征创建失败";
            项目运行日志(输出.str());
            return nullptr;
        }

        // 方法虚拟存在保存方法自身记忆；输出结果场景保存本轮可验收的实际结果状态。
        (void)世界树.写入特征_I64(目标主体, 特征类型, 实际状态值, now);

        auto* 状态场景 = 输出结果场景;
        if (!状态场景 && 当前本能方法) {
            状态场景 = 方法类::取或创建_方法结果场景(
                当前本能方法,
                nullptr,
                now);
        }
        if (!状态场景) {
            状态场景 = 世界树.取或创建自我现实场景();
        }
        if (!状态场景) {
            std::ostringstream 输出;
            输出 << "自我动作/目标方法实际结果写入跳过"
                << " | 目标方法=" << 目标方法->获取主键()
                << " | 方法虚拟存在=" << 目标方法存在->获取主键()
                << " | 特征类型=" << 特征类型->获取主键()
                << " | 原因=状态场景缺失";
            项目运行日志(输出.str());
            return nullptr;
        }
        输出结果场景 = 状态场景;

        auto* 实际状态 = 世界树.状态().创建内部状态(
            状态场景,
            目标主体,
            状态特征,
            实际状态值,
            枚举_存在状态事件::变化,
            true,
            now);

        auto* 场景宿主 = reinterpret_cast<基础信息节点类*>(状态场景);
        if (实际状态) {
            (void)世界树.写入特征_指针(场景宿主, 特征类型, 实际状态, now);
        }
        (void)世界树.写入特征_I64(场景宿主, 特征_需求满足情况(), 1, now);
        {
            std::ostringstream 输出;
            输出 << "自我动作/目标方法实际结果已写入"
                << " | 目标方法=" << 目标方法->获取主键()
                << " | 方法虚拟存在=" << 目标方法存在->获取主键()
                << " | 输出场景=" << 状态场景->获取主键()
                << " | 特征类型=" << 特征类型->获取主键()
                << " | 实际状态=" << (实际状态 ? 实际状态->获取主键() : std::string{})
                << " | 实际值=" << 实际状态值;
            项目运行日志(输出.str());
        }
        return 实际状态;
    }

    inline 特征节点类* 运行参数根(
        方法类::节点类* 方法首节点,
        时间戳 now) noexcept
    {
        auto* 虚拟存在 = 方法虚拟存在(方法首节点, now);
        return 虚拟存在
            ? 世界树.取或创建子特征_按类型(
                reinterpret_cast<基础信息节点类*>(虚拟存在),
                特征_运行参数())
            : nullptr;
    }

    inline 场景节点类* 抽象条件场景(
        方法类::节点类* 方法首节点,
        时间戳 now) noexcept
    {
        if (!方法首节点) return nullptr;
        return 方法类::取或创建_方法条件场景(方法首节点, nullptr, now);
    }

    inline 场景节点类* 抽象结果场景(
        方法类::节点类* 方法首节点,
        时间戳 now) noexcept
    {
        if (!方法首节点) return nullptr;
        return 方法类::取或创建_方法结果场景(方法首节点, nullptr, now);
    }

    inline 特征节点类* 抽象条件集合根(
        方法类::节点类* 方法首节点,
        时间戳 now) noexcept
    {
        auto* 条件场景 = 抽象条件场景(方法首节点, now);
        return 条件场景
            ? 世界树.取或创建子特征_按类型(
                reinterpret_cast<基础信息节点类*>(条件场景),
                特征_条件集合())
            : nullptr;
    }

    inline bool 语素入口主键相同(
        const 语素入口节点类* 左,
        const 语素入口节点类* 右) noexcept
    {
        if (左 == 右) return true;
        if (!左 || !右) return false;
        const auto 左主键 = 左->获取主键();
        const auto 右主键 = 右->获取主键();
        return !左主键.empty() && 左主键 == 右主键;
    }

    inline bool 确保方法结果能力I64(
        方法类::节点类* 方法首节点,
        const 语素入口节点类* 目标特征类型,
        I64 目标值,
        枚举_结果变化方向 方向 = 枚举_结果变化方向::已形成) noexcept
    {
        auto* 首信息 = 方法首节点 && 方法首节点->主信息.是否方法首节点()
            ? 方法首节点->主信息.取首节点信息()
            : nullptr;
        if (!首信息 || !目标特征类型) {
            return false;
        }

        首信息->能力.可被方法查找命中 = true;
        首信息->能力.是否根写入原语 = false;

        for (const auto& 结果项 : 首信息->能力.结果包.结果项集) {
            if (结果项.关系目标特征类型由参数决定()
                || !语素入口主键相同(结果项.关系目标特征类型(), 目标特征类型)) {
                continue;
            }

            const 特征值* 已有目标值 = 结果项.关系模板.有目标值
                ? &结果项.关系模板.目标值
                : &结果项.目标值;
            if (const auto* 已有I64 = std::get_if<I64>(已有目标值);
                已有I64 && *已有I64 == 目标值) {
                return true;
            }
        }

        结构_方法结果项 结果项{};
        结果项.特征类型 = 目标特征类型;
        结果项.方向 = 方向;
        结果项.目标值 = 特征值{ 目标值 };
        结果项.关系模板.目标特征类型 = 目标特征类型;
        结果项.关系模板.允许方向 = 方法结果变化方向允许方向(方向);
        结果项.关系模板.有目标值 = true;
        结果项.关系模板.目标值 = 结果项.目标值;
        首信息->能力.结果包.结果项集.push_back(std::move(结果项));
        return true;
    }

    inline bool 确保方法结果能力方向(
        方法类::节点类* 方法首节点,
        const 语素入口节点类* 目标特征类型,
        枚举_结果变化方向 方向) noexcept
    {
        auto* 首信息 = 方法首节点 && 方法首节点->主信息.是否方法首节点()
            ? 方法首节点->主信息.取首节点信息()
            : nullptr;
        if (!首信息 || !目标特征类型) {
            return false;
        }

        首信息->能力.可被方法查找命中 = true;
        首信息->能力.是否根写入原语 = false;
        const auto 允许方向 = 方法结果变化方向允许方向(方向);

        for (const auto& 结果项 : 首信息->能力.结果包.结果项集) {
            if (结果项.关系目标特征类型由参数决定()
                || !语素入口主键相同(结果项.关系目标特征类型(), 目标特征类型)) {
                continue;
            }
            if (!结果项.关系模板.有目标值
                && std::holds_alternative<std::monostate>(结果项.目标值)
                && 结果项.关系模板.允许方向 == 允许方向) {
                return true;
            }
        }

        结构_方法结果项 结果项{};
        结果项.特征类型 = 目标特征类型;
        结果项.方向 = 方向;
        结果项.关系模板.目标特征类型 = 目标特征类型;
        结果项.关系模板.允许方向 = 允许方向;
        首信息->能力.结果包.结果项集.push_back(std::move(结果项));
        return true;
    }

    inline bool 声明条件特征(
        方法类::节点类* 方法首节点,
        const 语素入口节点类* 特征类型,
        bool 是否必需,
        时间戳 now) noexcept
    {
        auto* 条件根 = 抽象条件集合根(方法首节点, now);
        const bool 条件槽已建立 = 条件根
            && 特征类型
            && 世界树.取或创建子特征_按类型(
                reinterpret_cast<基础信息节点类*>(条件根),
                特征类型);
        if (条件槽已建立) {
            auto* 条件槽 = 查找子特征(
                reinterpret_cast<基础信息节点类*>(条件根),
                特征类型);
            if (条件槽) {
                (void)写入基础节点I64(
                    reinterpret_cast<基础信息节点类*>(条件槽),
                    特征_是否必需(),
                    是否必需 ? 1 : 0,
                    now);
            }
        }
        return 条件槽已建立;
    }

    inline bool 声明条件特征(
        方法类::节点类* 方法首节点,
        const 语素入口节点类* 特征类型,
        时间戳 now) noexcept
    {
        return 声明条件特征(方法首节点, 特征类型, true, now);
    }

    inline 特征节点类* 方法规格根(
        方法类::节点类* 方法首节点,
        const 语素入口节点类* 根特征类型,
        时间戳 now) noexcept
    {
        auto* 虚拟存在 = 方法虚拟存在(方法首节点, now);
        return 虚拟存在
            ? 取或创建子特征(reinterpret_cast<基础信息节点类*>(虚拟存在), 根特征类型)
            : nullptr;
    }

    inline bool 声明形参规格(
        方法类::节点类* 方法首节点,
        const 语素入口节点类* 参数特征类型,
        const 语素入口节点类* 参数值类型,
        const 语素入口节点类* 参数来源,
        bool 是否必需,
        时间戳 now,
        const 语素入口节点类* 读取特征类型 = nullptr) noexcept
    {
        auto* 形参根 = 方法规格根(方法首节点, 特征_形参表(), now);
        auto* 形参 = 形参根 && 参数特征类型
            ? 取或创建子特征(reinterpret_cast<基础信息节点类*>(形参根), 参数特征类型)
            : nullptr;
        if (!形参) return false;

        auto* 宿主 = reinterpret_cast<基础信息节点类*>(形参);
        bool ok = true;
        ok = 写入基础节点指针(宿主, 特征_参数值类型(), 参数值类型, now) && ok;
        ok = 写入基础节点指针(宿主, 特征_参数来源(), 参数来源, now) && ok;
        ok = 写入基础节点I64(宿主, 特征_是否必需(), 是否必需 ? 1 : 0, now) && ok;
        if (读取特征类型) {
            ok = 写入基础节点指针(宿主, 特征_读取特征类型(), 读取特征类型, now) && ok;
        }
        return ok;
    }

    inline bool 写入方法执行情况(
        基础信息节点类* 宿主,
        const 语素入口节点类* 情况,
        时间戳 now) noexcept
    {
        return 写入基础节点指针(
            宿主,
            特征_方法执行情况(),
            情况,
            now);
    }
    inline bool 场景等价_条件结果对归并(
        const 场景节点类* 左,
        const 场景节点类* 右) noexcept
    {
        if (!左 || !右) return false;
        const auto 左状态集 = 世界树.状态().获取场景状态(左);
        const auto 右状态集 = 世界树.状态().获取场景状态(右);
        if (!左状态集.empty() || !右状态集.empty()) {
            if (左状态集.size() != 右状态集.size()) {
                return false;
            }
            for (const auto* 左状态 : 左状态集) {
                bool 已匹配 = false;
                for (const auto* 右状态 : 右状态集) {
                    const auto 比较 = 世界树.特征().比较状态(左状态, 右状态);
                    if (比较.可比较 && 比较.关系 == 枚举_三向关系::等于) {
                        已匹配 = true;
                        break;
                    }
                }
                if (!已匹配) {
                    return false;
                }
            }
            return true;
        }
        return 左 == 右 || 左->获取主键() == 右->获取主键();
    }

    inline 方法类::节点类* 查找等价条件节点_条件结果对归并(
        方法类::节点类* 方法首节点,
        场景节点类* 条件场景) noexcept
    {
        if (!方法首节点 || !方法首节点->子 || !条件场景) return nullptr;
        auto* 首子 = static_cast<方法类::节点类*>(方法首节点->子);
        auto* 当前 = 首子;
        do {
            if (当前
                && 当前->主信息.节点种类() == 枚举_方法节点种类::方法条件节点) {
                if (auto* 条件信息 = 当前->主信息.取条件节点信息();
                    条件信息 && 场景等价_条件结果对归并(条件信息->条件场景.指针, 条件场景)) {
                    return 当前;
                }
            }
            当前 = 当前 ? static_cast<方法类::节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首子);
        return nullptr;
    }

    inline bool 条件节点已有结果子节点_条件结果对归并(方法类::节点类* 条件节点) noexcept
    {
        if (!条件节点 || !条件节点->子) return false;
        auto* 首子 = static_cast<方法类::节点类*>(条件节点->子);
        auto* 当前 = 首子;
        do {
            if (当前
                && 当前->主信息.节点种类() == 枚举_方法节点种类::方法结果节点
                && 当前->主信息.取结果节点信息()) {
                return true;
            }
            当前 = 当前 ? static_cast<方法类::节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首子);
        return false;
    }

    inline I64 方法真实条件节点数量(方法类::节点类* 方法首节点) noexcept
    {
        if (!方法首节点 || !方法首节点->子) return 0;
        I64 数量 = 0;
        auto* 首子 = static_cast<方法类::节点类*>(方法首节点->子);
        auto* 当前 = 首子;
        do {
            if (当前
                && 当前->主信息.节点种类() == 枚举_方法节点种类::方法条件节点
                && 当前->主信息.取条件节点信息()) {
                ++数量;
            }
            当前 = 当前 ? static_cast<方法类::节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首子);
        return 数量;
    }

    inline I64 方法真实条件结果对数量(方法类::节点类* 方法首节点) noexcept
    {
        if (!方法首节点 || !方法首节点->子) return 0;
        I64 数量 = 0;
        auto* 首子 = static_cast<方法类::节点类*>(方法首节点->子);
        auto* 当前 = 首子;
        do {
            if (当前
                && 当前->主信息.节点种类() == 枚举_方法节点种类::方法条件节点
                && 条件节点已有结果子节点_条件结果对归并(当前)) {
                ++数量;
            }
            当前 = 当前 ? static_cast<方法类::节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首子);
        return 数量;
    }

    inline bool 方法具有本能执行入口(方法类::节点类* 方法首节点) noexcept
    {
        return 方法首节点
            && 方法首节点->主信息.首节点信息().动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && 方法首节点->主信息.首节点信息().动作句柄.本能ID != 0;
    }

    inline bool 本能动作已注册_动作模块(枚举_本能方法ID 本能ID) noexcept
    {
        return 本能ID != 枚举_本能方法ID::未定义
            && 本能方法类::是默认自我本能方法(本能ID)
            && 取本能动作集().查询(static_cast<枚举_本能动作ID>(本能ID)).has_value();
    }

    inline bool 本能ID是当前默认自我本能_动作模块(枚举_本能方法ID 本能ID) noexcept
    {
        return 本能ID != 枚举_本能方法ID::未定义
            && 本能方法类::是默认自我本能方法(本能ID);
    }

    inline bool 方法具有已注册本能执行入口(方法类::节点类* 方法首节点) noexcept
    {
        if (!方法具有本能执行入口(方法首节点)) {
            return false;
        }

        const auto 本能ID = static_cast<枚举_本能方法ID>(
            方法首节点->主信息.首节点信息().动作句柄.本能ID);
        return 本能动作已注册_动作模块(本能ID);
    }

    inline bool 本能动作可作为练习目标(枚举_本能方法ID 本能ID) noexcept
    {
        if (!本能动作已注册_动作模块(本能ID)) {
            return false;
        }

        switch (本能ID) {
        case 枚举_本能方法ID::自我_练习:
        case 枚举_本能方法ID::自我_尝试学习:
        case 枚举_本能方法ID::自我_定向学习:
        case 枚举_本能方法ID::自我_提交任务状态变化:
        case 枚举_本能方法ID::自我_回写需求满足:
        case 枚举_本能方法ID::自我_结算叶子任务价值:
        case 枚举_本能方法ID::自我_提交方法可执行入口:
        case 枚举_本能方法ID::自我_建立条件结果对:
        case 枚举_本能方法ID::自我_提交方法状态变化:
        case 枚举_本能方法ID::提交_确认观察存在事实:
        case 枚举_本能方法ID::提交_确认内部世界子存在事实:
        case 枚举_本能方法ID::提交_风险安全场景影响部分状态变化:
        case 枚举_本能方法ID::提交_安全因果因素无负证据默认满足:
        case 枚举_本能方法ID::提交_安全因果因素证据评估值:
            return false;
        default:
            return true;
        }
    }

    inline I64 推导方法状态_结构与运行账(
        方法类::节点类* 方法首节点,
        I64 最近连续成功次数) noexcept
    {
        if (!方法首节点) {
            return static_cast<I64>(枚举_方法状态::未定义);
        }
        if (!方法具有已注册本能执行入口(方法首节点)) {
            return 方法状态_待可执行入口();
        }
        if (方法真实条件节点数量(方法首节点) <= 0) {
            return 方法状态_待条件节点();
        }
        if (方法真实条件结果对数量(方法首节点) <= 0) {
            return 方法状态_待条件结果对();
        }
        return 最近连续成功次数 > 3
            ? 方法状态_好用()
            : 方法状态_可用();
    }

    inline I64 饱和增加(I64 左值, I64 右值) noexcept
    {
        if (右值 > 0 && 左值 > (std::numeric_limits<I64>::max)() - 右值) {
            return (std::numeric_limits<I64>::max)();
        }
        if (右值 < 0 && 左值 < (std::numeric_limits<I64>::min)() - 右值) {
            return (std::numeric_limits<I64>::min)();
        }
        return 左值 + 右值;
    }

    inline bool 增加场景I64特征(
        场景节点类* 场景,
        const 语素入口节点类* 特征类型,
        I64 增量,
        时间戳 now) noexcept
    {
        if (!场景 || !特征类型) return false;
        auto* 特征 = 世界树.取或创建子特征_按类型(
            reinterpret_cast<基础信息节点类*>(场景),
            特征类型);
        if (!特征) return false;

        I64 当前值 = 0;
        (void)世界树.读取特征_I64(特征, 当前值);
        return 世界树.写入特征_I64(特征, 饱和增加(当前值, 增量), now);
    }

    inline void 写入参数绑定失败(
        存在节点类* 运行存在,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept
    {
        if (运行存在) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(运行存在),
                值_参数绑定失败(),
                now);
        }
        if (输出结果场景) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                值_参数绑定失败(),
                now);
        }
    }

    inline bool 声明运行参数(
        方法类::节点类* 方法首节点,
        const 语素入口节点类* 参数类型,
        时间戳 now) noexcept
    {
        auto* 参数根 = 运行参数根(方法首节点, now);
        return 参数根
            && 参数类型
            && 世界树.取或创建子特征_按类型(
                reinterpret_cast<基础信息节点类*>(参数根),
                参数类型);
    }

    inline bool 运行参数已从输入场景匹配(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !输入参数场景) return false;
        auto* 参数根 = 运行参数根(方法首节点, now);
        if (!参数根) return false;

        auto* 根宿主 = reinterpret_cast<基础信息节点类*>(输入参数场景);
        auto* 参数包 = 输入包根(输入参数场景, 特征_任务参数包());
        auto* 包宿主 = 参数包 ? reinterpret_cast<基础信息节点类*>(参数包) : nullptr;

        for (auto* 参数特征 : 世界树.特征().获取子特征(reinterpret_cast<基础信息节点类*>(参数根))) {
            const auto* 参数主信息 = 世界树.特征().取特征主信息(参数特征);
            const auto* 参数类型 = 参数主信息 ? 参数主信息->类型 : nullptr;
            if (!参数类型) return false;

            const bool 根上存在 = 世界树.特征().查找子特征_按类型(根宿主, 参数类型) != nullptr;
            const bool 包中存在 = 包宿主
                && 世界树.特征().查找子特征_按类型(包宿主, 参数类型) != nullptr;
            if (!根上存在 && !包中存在) return false;
        }
        return true;
    }

    inline bool 描述无条件本能(
        方法类::节点类* 方法首节点,
        时间戳 now) noexcept
    {
        if (!方法首节点) return false;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        // 通用方法虚拟存在初始化由“确保本能方法自身环境”统一完成。
        bool 已写入 = false;
        已写入 = 写入I64(方法首节点, 特征_条件描述状态(), 1, now) || 已写入;
        已写入 = 写入I64(方法首节点, 特征_条件描述时间(), static_cast<I64>(now), now) || 已写入;
        已写入 = 写入I64(方法首节点, 特征_条件特征类型数量(), 0, now) || 已写入;
        已写入 = 写入I64(方法首节点, 特征_必需条件特征类型数量(), 0, now) || 已写入;
        已写入 = 写入I64(方法首节点, 特征_可选条件特征类型数量(), 0, now) || 已写入;
        return 已写入;
    }

    inline 方法类::节点类* 方法树根(方法类::节点类* 方法节点) noexcept
    {
        auto* 当前 = 方法节点;
        while (当前 && 当前->父) {
            当前 = static_cast<方法类::节点类*>(当前->父);
        }
        return 当前;
    }

    inline 方法类::节点类* 确保本能方法(
        方法类::节点类* 方法树锚点,
        枚举_本能方法ID 本能ID,
        时间戳 now) noexcept
    {
        if (!方法树锚点 || 本能ID == 枚举_本能方法ID::未定义) return nullptr;
        if (!本能方法类::是默认自我本能方法(本能ID)) return nullptr;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        结构体_动作句柄 动作句柄{};
        动作句柄.类型 = 枚举_动作句柄类型::本能函数ID;
        动作句柄.本能ID = static_cast<I64>(本能ID);

        if (方法树锚点->主信息.公共.节点种类 == 枚举_方法节点种类::方法首节点
            && 方法树锚点->主信息.首节点信息().动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && 方法树锚点->主信息.首节点信息().动作句柄.本能ID == static_cast<I64>(本能ID)) {
            // 已存在的本能方法也可能来自旧链或半初始化状态。每次执行前轻量补齐
            // 方法头、方法虚拟存在、条件场景、条件节点和结果节点，避免过程本能
            // 在自身环境阶段因为缺条件节点而递归派生“方法状态=可用”需求。
            (void)本能方法类::补齐本能方法首节点(方法树锚点, nullptr, now);
            return 方法树锚点;
        }

        auto* 根节点 = 方法树根(方法树锚点);
        if (!根节点) return nullptr;
        if (auto* 已有方法 = 方法类::查找方法首节点_按动作句柄(根节点, 动作句柄)) {
            // 查到已有方法时同样补齐通用运行壳。专属输入/输出规格仍由后续
            // 确认XXX规格负责，不在这里伪造条件结果对。
            (void)本能方法类::补齐本能方法首节点(已有方法, nullptr, now);
            return 已有方法;
        }

        const auto* 动作名称 = 本能方法类::查询默认动作名称(本能ID);
        if (!动作名称 || 动作名称[0] == '\0') return nullptr;

        auto* 动作名 = 语素集.添加信息入口词(动作名称, 枚举_信息入口类型::动态模板入口);
        auto* 新方法 = 方法类::注册方法首节点(
            根节点,
            动作名,
            动作句柄,
            now);
        if (!新方法) return nullptr;

        (void)本能方法类::补齐本能方法首节点(新方法, nullptr, now);
        return 新方法;
    }

    inline 存在节点类* 本轮运行虚拟存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept
    {
        基础信息节点类* 宿主 = nullptr;
        if (auto* 虚拟存在 = 方法虚拟存在(方法首节点, now)) {
            宿主 = reinterpret_cast<基础信息节点类*>(虚拟存在);
        }
        else {
            宿主 = reinterpret_cast<基础信息节点类*>(输出结果场景);
        }
        if (!宿主) return nullptr;

        auto* 运行存在 = 世界树.取或创建子存在_按类型并按特征I64(
            宿主,
            类型_运行虚拟存在(),
            特征_开始时间(),
            static_cast<I64>(now));
        if (!运行存在) return nullptr;

        if (auto* 虚拟存在 = 方法虚拟存在(方法首节点, now)) {
            (void)写入存在指针(运行存在, 特征_方法虚拟存在(), 虚拟存在, now);
        }
        (void)写入存在I64(运行存在, 特征_开始时间(), static_cast<I64>(now), now);
        return 运行存在;
    }

    inline 存在节点类* 输出运行虚拟存在(
        存在节点类* 运行存在,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept
    {
        if (!运行存在) return nullptr;

        const auto 结束时间 = 结构体_时间戳::当前_微秒();
        (void)写入存在I64(运行存在, 特征_结束时间(), static_cast<I64>(结束时间), 结束时间);
        I64 开始时间 = 0;
        if (读取基础节点I64(
            reinterpret_cast<基础信息节点类*>(运行存在),
            特征_开始时间(),
            开始时间)) {
            const I64 本次时长 = static_cast<I64>(结束时间) >= 开始时间
                ? static_cast<I64>(结束时间) - 开始时间
                : 0;
            (void)写入存在I64(运行存在, 特征_运行时长(), 本次时长, 结束时间);
        }
        if (输入参数场景) {
            (void)写入存在指针(运行存在, 特征_输入参数场景(), 输入参数场景, now);
            (void)写入存在指针(运行存在, 特征_返回参数信息(), 输入参数场景, now);
            void* 任务存在 = nullptr;
            if (读取调用信息指针(输入参数场景, 特征_任务虚拟存在(), 任务存在)) {
                (void)写入存在指针(运行存在, 特征_任务虚拟存在(), 任务存在, now);
            }
        }
        if (输出结果场景) {
            (void)写入存在指针(运行存在, 特征_输出结果场景(), 输出结果场景, now);
            (void)写入存在指针(运行存在, 特征_返回场景信息(), 输出结果场景, now);
        }
        return 运行存在;
    }


    inline void 更新方法运行账(
        方法类::节点类* 方法首节点,
        存在节点类* 运行存在,
        bool 成功,
        时间戳 now) noexcept
    {
        auto* 虚拟存在 = 方法虚拟存在(方法首节点, now);
        if (!虚拟存在) return;

        auto* 宿主 = reinterpret_cast<基础信息节点类*>(虚拟存在);
        I64 运行次数 = 0;
        I64 动作执行成功次数 = 0;
        I64 动作执行失败次数 = 0;
        I64 方法运行次数 = 0;
        I64 方法成功次数 = 0;
        I64 方法最近连续成功次数 = 0;
        I64 累计运行时长 = 0;
        I64 本次运行时长 = 0;

        (void)读取基础节点I64(宿主, 特征_运行次数(), 运行次数);
        (void)读取基础节点I64(宿主, 特征_动作执行成功次数(), 动作执行成功次数);
        (void)读取基础节点I64(宿主, 特征_动作执行失败次数(), 动作执行失败次数);
        (void)读取基础节点I64(宿主, 特征_方法运行次数(), 方法运行次数);
        (void)读取基础节点I64(宿主, 特征_方法成功次数(), 方法成功次数);
        (void)读取基础节点I64(宿主, 特征_方法最近连续成功次数(), 方法最近连续成功次数);
        (void)读取基础节点I64(宿主, 特征_累计运行时长(), 累计运行时长);
        if (运行存在) {
            (void)读取基础节点I64(
                reinterpret_cast<基础信息节点类*>(运行存在),
                特征_运行时长(),
                本次运行时长);
        }

        运行次数 = 饱和增加(运行次数, 1);
        if (成功) 动作执行成功次数 = 饱和增加(动作执行成功次数, 1);
        else 动作执行失败次数 = 饱和增加(动作执行失败次数, 1);
        累计运行时长 = 饱和增加(累计运行时长, 本次运行时长);
        const I64 平均运行时长 = 运行次数 > 0 ? 累计运行时长 / 运行次数 : 0;
        方法运行次数 = 饱和增加(方法运行次数, 1);
        if (成功) {
            方法成功次数 = 饱和增加(方法成功次数, 1);
            方法最近连续成功次数 = 饱和增加(方法最近连续成功次数, 1);
        }
        else {
            方法最近连续成功次数 = 0;
        }
        (void)写入基础节点I64(宿主, 特征_运行次数(), 运行次数, now);
        (void)写入基础节点I64(宿主, 特征_动作执行成功次数(), 动作执行成功次数, now);
        (void)写入基础节点I64(宿主, 特征_动作执行失败次数(), 动作执行失败次数, now);
        (void)写入基础节点I64(宿主, 特征_方法运行次数(), 方法运行次数, now);
        (void)写入基础节点I64(宿主, 特征_方法成功次数(), 方法成功次数, now);
        (void)写入基础节点I64(宿主, 特征_方法最近连续成功次数(), 方法最近连续成功次数, now);
        (void)写入基础节点I64(宿主, 特征_累计运行时长(), 累计运行时长, now);
        (void)写入基础节点I64(宿主, 特征_平均运行时长(), 平均运行时长, now);
        (void)写入基础节点I64(宿主, 特征_运行时长(), 本次运行时长, now);
        (void)写入基础节点I64(宿主, 特征_最近执行时间(), static_cast<I64>(now), now);
        (void)写入基础节点I64(宿主, 特征_最近完成时间(), static_cast<I64>(now), now);
        const I64 方法状态 = 推导方法状态_结构与运行账(方法首节点, 方法最近连续成功次数);
        (void)写入基础节点I64(宿主, 特征_方法状态(), 方法状态, now);
        (void)写入基础节点I64(
            宿主,
            特征_方法稳定状态(),
            方法状态 >= 方法状态_好用() ? 1 : 0,
            now);
        if (运行存在) {
            (void)写入基础节点指针(宿主, 特征_最近运行虚拟存在(), 运行存在, now);
        }
    }

    inline 方法类::节点类* 取首个方法条件节点(方法类::节点类* 方法首节点) noexcept
    {
        if (!方法首节点 || !方法首节点->子) return nullptr;
        auto* 首子 = static_cast<方法类::节点类*>(方法首节点->子);
        auto* 当前 = 首子;
        do {
            if (当前 && 当前->主信息.节点种类() == 枚举_方法节点种类::方法条件节点) {
                return 当前;
            }
            当前 = 当前 ? static_cast<方法类::节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首子);
        return nullptr;
    }

    inline 方法类::节点类* 解析方法执行条件节点(
        方法类::节点类* 方法首节点,
        场景节点类* 条件场景) noexcept
    {
        if (!方法首节点) return nullptr;
        if (条件场景) {
            if (auto* 等价节点 = 查找等价条件节点_条件结果对归并(方法首节点, 条件场景)) {
                return 等价节点;
            }
        }
        return 取首个方法条件节点(方法首节点);
    }

    // 本能方法先建立“自己”的运行环境，再读取输入并处理目标对象。
    struct 结构_本能方法自身环境 {
        方法类::节点类* 方法首节点 = nullptr;
        方法类::节点类* 方法条件节点 = nullptr;
        存在节点类* 方法虚拟存在 = nullptr;
        存在节点类* 本轮运行虚拟存在 = nullptr;
        场景节点类* 条件场景 = nullptr;
        场景节点类* 结果场景 = nullptr;

        bool 方法头已就绪 = false;
        bool 方法条件节点已就绪 = false;
        bool 本轮运行虚拟存在已就绪 = false;
        bool 方法虚拟存在已就绪 = false;
        bool 条件场景已就绪 = false;
        bool 结果场景已就绪 = false;
        bool 方法规格已确认 = false;
        bool 已成功 = false;

        const 语素入口节点类* 失败原因 = nullptr;
    };

    inline std::string 指针日志文本(const void* 指针) noexcept
    {
        return std::to_string(reinterpret_cast<std::uintptr_t>(指针));
    }

    inline void 写入本能方法自身环境失败(
        基础信息节点类* 宿主,
        const 语素入口节点类* 失败原因,
        时间戳 now) noexcept
    {
        if (!宿主) return;
        (void)写入方法执行情况(宿主, 值_执行失败(), now);
        if (失败原因) {
            (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
        }
    }

    inline void 记录本能方法自身环境错误(
        const char* 调用方,
        枚举_本能方法ID 本能ID,
        方法类::节点类* 输入方法首节点,
        const 结构_本能方法自身环境& 环境) noexcept
    {
        std::string 文本 = "本能方法自身环境错误 | 调用方=";
        文本 += 调用方 ? 调用方 : "";
        文本 += " | 本能ID=";
        文本 += std::to_string(static_cast<std::uint64_t>(本能ID));
        文本 += " | 输入方法首节点=";
        文本 += 指针日志文本(输入方法首节点);
        文本 += " | 实际方法首节点=";
        文本 += 指针日志文本(环境.方法首节点);
        文本 += " | 方法条件节点=";
        文本 += 指针日志文本(环境.方法条件节点);
        文本 += " | 方法虚拟存在=";
        文本 += 指针日志文本(环境.方法虚拟存在);
        文本 += " | 本轮运行虚拟存在=";
        文本 += 指针日志文本(环境.本轮运行虚拟存在);
        文本 += " | 条件场景=";
        文本 += 指针日志文本(环境.条件场景);
        文本 += " | 结果场景=";
        文本 += 指针日志文本(环境.结果场景);
        文本 += " | 失败原因=";
        文本 += 指针日志文本(环境.失败原因);
        项目运行错误日志(文本);
    }

    // 通用入口只负责自身方法环境；目标方法缺口由具体本能逻辑处理。
    template<class 规格确认函数>
    inline 结构_本能方法自身环境 确保本能方法自身环境(
        方法类::节点类* 输入方法首节点,
        枚举_本能方法ID 本能ID,
        场景节点类* 输出结果场景,
        时间戳 now,
        const char* 调用方,
        规格确认函数 确认规格) noexcept
    {
        结构_本能方法自身环境 环境{};
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        const auto 失败返回 = [&]() noexcept {
            if (环境.本轮运行虚拟存在) {
                写入本能方法自身环境失败(
                    reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在),
                    环境.失败原因,
                    now);
            }
            记录本能方法自身环境错误(调用方, 本能ID, 输入方法首节点, 环境);
            return 环境;
        };

        环境.方法首节点 = 确保本能方法(输入方法首节点, 本能ID, now);
        环境.方法头已就绪 = 环境.方法首节点 != nullptr;
        if (!环境.方法头已就绪) {
            环境.失败原因 = 值_自身方法头节点缺失();
            return 失败返回();
        }

        // 通用方法环境只在这里统一同步：
        // 方法虚拟存在先通过抽象存在树中的“方法”概念扩充通用槽位，
        // 后续确认规格函数只补本能方法自己的能力签名、条件槽和结果槽。
        if (!方法虚拟存在服务类::同步方法节点到虚拟存在(
                环境.方法首节点,
                nullptr,
                now)) {
            环境.失败原因 = 值_方法虚拟存在缺失();
            return 失败返回();
        }

        环境.本轮运行虚拟存在 = 本轮运行虚拟存在(环境.方法首节点, 输出结果场景, now);
        环境.本轮运行虚拟存在已就绪 = 环境.本轮运行虚拟存在 != nullptr;
        if (!环境.本轮运行虚拟存在已就绪) {
            环境.失败原因 = 值_本轮运行虚拟存在缺失();
            return 失败返回();
        }

        环境.方法虚拟存在 = 方法虚拟存在(环境.方法首节点, now);
        环境.方法虚拟存在已就绪 = 环境.方法虚拟存在 != nullptr;
        if (!环境.方法虚拟存在已就绪) {
            环境.失败原因 = 值_方法虚拟存在缺失();
            return 失败返回();
        }

        环境.条件场景 = 抽象条件场景(环境.方法首节点, now);
        环境.结果场景 = 抽象结果场景(环境.方法首节点, now);
        环境.条件场景已就绪 = 环境.条件场景 != nullptr;
        环境.结果场景已就绪 = 环境.结果场景 != nullptr;
        if (!环境.条件场景已就绪 || !环境.结果场景已就绪) {
            环境.失败原因 = 值_方法条件结果场景缺失();
            return 失败返回();
        }

        环境.方法规格已确认 = 确认规格(
            环境.方法首节点,
            环境.方法虚拟存在,
            now);
        if (!环境.方法规格已确认) {
            环境.失败原因 = 值_方法规格确认失败();
            return 失败返回();
        }

        环境.方法条件节点 = 解析方法执行条件节点(
            环境.方法首节点,
            环境.条件场景);
        环境.方法条件节点已就绪 = 环境.方法条件节点 != nullptr;
        if (!环境.方法条件节点已就绪) {
            环境.失败原因 = 值_方法条件结果场景缺失();
            return 失败返回();
        }

        // 专属规格确认会补形参和条件能力签名，确认后再同步一次结构摘要到方法虚拟存在。
        (void)方法虚拟存在服务类::同步方法节点到虚拟存在(
            环境.方法首节点,
            nullptr,
            now);

        环境.已成功 = true;
        return 环境;
    }

    // 自身环境失败属于非正常逻辑错误，必须留下运行回执和日志。
    inline 存在节点类* 输出本能方法自身环境失败(
        const 结构_本能方法自身环境& 环境,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept
    {
        if (输出结果场景) {
            写入本能方法自身环境失败(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                环境.失败原因,
                now);
        }
        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            输出结果场景,
            now);
        更新方法运行账(环境.方法首节点, 输出运行, false, now);
        return 输出运行;
    }

    struct 结构_双目相机调用结果 {
        bool 成功 = false;
        bool 相机已打开 = false;
        const 语素入口节点类* 失败原因 = nullptr;
        std::string 消息{};
        int 宽度 = 0;
        int 高度 = 0;
        std::uint32_t 深度帧号 = 0;
        std::uint32_t 彩色帧号 = 0;
        I64 预期像素数量 = 0;
        I64 像素特征数量 = 0;
        I64 像素观察存在数量 = 0;
        I64 缺失像素数量 = 0;
        I64 重复映射数量 = 0;
        I64 颜色像素数量 = 0;
        I64 深度有效像素数量 = 0;
        I64 点云有效像素数量 = 0;
        I64 当前观察特征帧取得状态 = 0;
        I64 观察帧容器状态 = 0;
        I64 像素覆盖状态 = 0;
        I64 像素特征值承接状态 = 0;
        I64 坐标回查可用状态 = 0;
        I64 帧内编号回查可用状态 = 0;
        I64 观察事实完备状态 = 0;
        I64 观察事实完备失败条件数量 = 0;
        std::string 观察事实完备失败原因摘要{};
        I64 质量可用状态 = 0;
        I64 质量失败条件数量 = 0;
        std::string 质量失败原因摘要{};
        I64 当前观察帧采集时间 = 0;
        I64 当前观察帧观察年龄 = 0;
        I64 最大允许观察年龄 = 最大允许当前观察年龄微秒;
        I64 新鲜度判定时间 = 0;
        I64 观察帧失效状态 = 观察失效状态_未失效;
        I64 观察帧来源外设状态 = 来源外设状态_不可追溯;
        I64 当前观察新鲜度状态 = 0;
        I64 新鲜度失败原因 = 新鲜度失败原因_无;
        std::string 新鲜度失败原因摘要{};
        I64 基础观察事实可用状态 = 0;
        I64 基础观察事实失败条件数量 = 0;
        I64 空间坐标单位毫米 = 1;
        I64 颜色RGB结构状态 = 0;
        I64 原始深度毫米结构状态 = 0;
        I64 滤波深度毫米结构状态 = 0;
        I64 补全深度毫米结构状态 = 0;
        I64 深度有效性Mask状态 = 0;
        I64 深度来源结构状态 = 0;
        I64 深度稳定性结构状态 = 0;
        I64 深度邻域一致性结构状态 = 0;
        I64 融合深度毫米结构状态 = 0;
        I64 融合深度有效性结构状态 = 0;
        I64 多帧深度稳定性结构状态 = 0;
        I64 每像素深度方差结构状态 = 0;
        I64 空间坐标毫米XYZ结构状态 = 0;
        I64 原始深度来源像素数量 = 0;
        I64 滤波深度来源像素数量 = 0;
        I64 补全深度来源像素数量 = 0;
        I64 无有效深度来源像素数量 = 0;
        I64 补全深度低置信像素数量 = 0;
        I64 深度稳定性平均值 = 0;
        I64 深度邻域一致性平均值 = 0;
        I64 观察帧组状态 = 0;
        I64 观察帧组帧数 = 0;
        I64 融合依据帧集合状态 = 0;
        I64 融合深度有效像素数量 = 0;
        I64 融合深度有效率 = 0;
        I64 融合补偿深度空洞像素数量 = 0;
        I64 融合低稳定像素数量 = 0;
        I64 融合深度稳定性平均值 = 0;
        I64 融合深度平均方差 = 0;
        I64 彩色深度已对齐 = 0;
        I64 对齐目标 = 0;
        I64 深度有效率 = 0;
        I64 空间坐标有效率 = 0;
        I64 帧质量评分 = 0;
        I64 观察帧质量诊断摘要 = 0;
        I64 深度空洞数量 = 0;
        I64 深度空洞区域数量 = 0;
        I64 深度空洞区域集合状态 = 0;
        I64 彩色深度对齐状态 = 0;
        I64 轮廓闭合率 = 0;
        I64 轮廓断裂数量 = 0;
        I64 彩图轮廓数量 = 0;
        I64 深度轮廓数量 = 0;
        I64 空间投影轮廓数量 = 0;
        I64 融合轮廓数量 = 0;
        I64 轮廓来源分层状态 = 0;
        I64 轮廓深度支持率 = 0;
        I64 轮廓颜色支持率 = 0;
        I64 轮廓空间支持率 = 0;
        I64 融合轮廓置信度 = 0;
        I64 跨越深度断裂轮廓数量 = 0;
        I64 穿过深度无效区域轮廓数量 = 0;
        I64 依赖补全深度轮廓数量 = 0;
        I64 空间点有效率 = 0;
        I64 边界深度稳定率 = 0;
        I64 候选存在验证通过率 = 0;
        I64 未解释区域比例 = 0;
        I64 补观察缺口状态 = 0;
        I64 补观察缺失原因 = 0;
        I64 待补观察区域数量 = 0;
        I64 补全候选数量 = 0;
        I64 低置信轮廓数量 = 0;
        I64 条件不足候选数量 = 0;
        I64 待验证候选数量 = 0;
        I64 部分确认候选数量 = 0;
        I64 补观察需求建议 = 0;
        I64 诊断区域数量 = 0;
        I64 诊断区域集合状态 = 0;
        I64 诊断区域掩码状态 = 0;
        I64 空间候选数量 = 0;
        I64 空间候选有效点数量 = 0;
        std::vector<双目相机本能适配器::空间候选摘要> 空间候选列表{};
        std::vector<双目相机本能适配器::诊断区域摘要> 诊断区域列表{};
        std::size_t 轮廓数量 = 0;
    };

    inline I64 观察事实布尔状态(bool 条件) noexcept
    {
        return 条件 ? 观察事实状态_可用 : 观察事实状态_未成立;
    }

    inline std::string 新鲜度失败原因摘要文本(I64 原因)
    {
        if (原因 == 新鲜度失败原因_无) {
            return "无";
        }

        std::vector<const char*> 条件{};
        if ((原因 & 新鲜度失败原因_缺采集时间) != 0) 条件.push_back("缺采集时间");
        if ((原因 & 新鲜度失败原因_当前时间异常) != 0) 条件.push_back("当前时间异常");
        if ((原因 & 新鲜度失败原因_缺最大允许年龄) != 0) 条件.push_back("缺最大允许观察年龄");
        if ((原因 & 新鲜度失败原因_来源外设不可追溯) != 0) 条件.push_back("来源外设不可追溯");
        if ((原因 & 新鲜度失败原因_观察已失效) != 0) 条件.push_back("观察帧已失效");
        if ((原因 & 新鲜度失败原因_观察过期) != 0) 条件.push_back("观察过期");

        std::ostringstream 输出;
        for (std::size_t i = 0; i < 条件.size(); ++i) {
            if (i > 0) 输出 << ';';
            输出 << 条件[i];
        }
        return 输出.str();
    }

    inline void 补全当前观察新鲜度状态(
        结构_双目相机调用结果& 结果,
        时间戳 判定时间) noexcept
    {
        if (判定时间 == 0) {
            判定时间 = 结构体_时间戳::当前_微秒();
        }
        结果.新鲜度判定时间 = static_cast<I64>(判定时间);
        if (结果.最大允许观察年龄 <= 0) {
            结果.最大允许观察年龄 = 最大允许当前观察年龄微秒;
        }

        const bool 帧号可追溯 = 结果.深度帧号 > 0 || 结果.彩色帧号 > 0;
        if (结果.观察帧来源外设状态 != 来源外设状态_可追溯) {
            结果.观察帧来源外设状态 =
                结果.成功
                && 结果.宽度 > 0
                && 结果.高度 > 0
                && 帧号可追溯
                && 结果.当前观察帧采集时间 > 0
                    ? 来源外设状态_可追溯
                    : 来源外设状态_不可追溯;
        }

        I64 原因 = 新鲜度失败原因_无;
        if (结果.当前观察帧采集时间 <= 0) {
            原因 |= 新鲜度失败原因_缺采集时间;
        }
        if (结果.新鲜度判定时间 <= 0
            || (结果.当前观察帧采集时间 > 0
                && 结果.新鲜度判定时间 < 结果.当前观察帧采集时间)) {
            原因 |= 新鲜度失败原因_当前时间异常;
        }
        if (结果.最大允许观察年龄 <= 0) {
            原因 |= 新鲜度失败原因_缺最大允许年龄;
        }
        if (结果.观察帧来源外设状态 != 来源外设状态_可追溯) {
            原因 |= 新鲜度失败原因_来源外设不可追溯;
        }
        if (结果.观察帧失效状态 != 观察失效状态_未失效) {
            原因 |= 新鲜度失败原因_观察已失效;
        }

        if ((原因 & (新鲜度失败原因_缺采集时间 | 新鲜度失败原因_当前时间异常)) == 0) {
            结果.当前观察帧观察年龄 = 结果.新鲜度判定时间 - 结果.当前观察帧采集时间;
            if (结果.最大允许观察年龄 > 0
                && 结果.当前观察帧观察年龄 >= 结果.最大允许观察年龄) {
                原因 |= 新鲜度失败原因_观察过期;
            }
        } else {
            结果.当前观察帧观察年龄 = 0;
        }

        if (原因 == 新鲜度失败原因_无) {
            结果.当前观察新鲜度状态 = 观察事实状态_新鲜;
        } else if ((原因 & (新鲜度失败原因_观察过期 | 新鲜度失败原因_观察已失效)) != 0) {
            结果.当前观察新鲜度状态 = 观察事实状态_过期;
        } else {
            结果.当前观察新鲜度状态 = 观察事实状态_未成立;
        }
        结果.新鲜度失败原因 = 原因;
        结果.新鲜度失败原因摘要 = 新鲜度失败原因摘要文本(原因);
    }

    inline void 补全基础观察事实状态(
        结构_双目相机调用结果& 结果,
        时间戳 判定时间 = 0) noexcept
    {
        const I64 预期像素数量 = 结果.预期像素数量 > 0
            ? 结果.预期像素数量
            : static_cast<I64>(结果.宽度) * static_cast<I64>(结果.高度);
        if (结果.预期像素数量 <= 0 && 预期像素数量 > 0) {
            结果.预期像素数量 = 预期像素数量;
        }

        const bool 帧尺寸有效 = 结果.宽度 > 0 && 结果.高度 > 0 && 预期像素数量 > 0;
        const bool 帧号可追溯 = 结果.彩色帧号 > 0 || 结果.深度帧号 > 0;
        结果.像素观察存在数量 = std::max<I64>(0, 结果.像素特征数量);
        结果.缺失像素数量 = 预期像素数量 > 结果.像素观察存在数量
            ? 预期像素数量 - 结果.像素观察存在数量
            : 0;
        结果.重复映射数量 = 结果.像素观察存在数量 > 预期像素数量
            ? 结果.像素观察存在数量 - 预期像素数量
            : 0;
        结果.当前观察特征帧取得状态 = 观察事实布尔状态(结果.成功 && 帧尺寸有效 && 帧号可追溯);
        结果.观察帧容器状态 = 观察事实布尔状态(
            结果.当前观察特征帧取得状态 == 观察事实状态_已取得
            && 结果.预期像素数量 == static_cast<I64>(结果.宽度) * static_cast<I64>(结果.高度));

        结果.像素覆盖状态 = 观察事实布尔状态(
            帧尺寸有效
            && 结果.像素观察存在数量 == 结果.预期像素数量
            && 结果.缺失像素数量 == 0
            && 结果.重复映射数量 == 0);

        结果.坐标回查可用状态 = 观察事实布尔状态(
            结果.像素覆盖状态 == 观察事实状态_已覆盖
            && 帧尺寸有效);
        结果.帧内编号回查可用状态 = 观察事实布尔状态(
            结果.像素覆盖状态 == 观察事实状态_已覆盖
            && 结果.像素观察存在数量 == 结果.预期像素数量
            && 结果.缺失像素数量 == 0
            && 结果.重复映射数量 == 0);
        结果.像素特征值承接状态 = 观察事实布尔状态(
            结果.像素覆盖状态 == 观察事实状态_已覆盖
            && 结果.像素特征数量 == 结果.预期像素数量
            && 结果.颜色像素数量 == 结果.预期像素数量
            && 结果.颜色RGB结构状态 == 1
            && 结果.原始深度毫米结构状态 == 1
            && 结果.深度有效性Mask状态 == 1
            && 结果.空间坐标毫米XYZ结构状态 == 1);
        结果.观察事实完备状态 = 观察事实布尔状态(
            结果.像素覆盖状态 == 观察事实状态_已覆盖
            && 结果.像素特征值承接状态 == 观察事实状态_已完成
            && 结果.坐标回查可用状态 == 观察事实状态_可用
            && 结果.帧内编号回查可用状态 == 观察事实状态_可用);
        std::vector<const char*> 完备失败条件{};
        if (结果.像素覆盖状态 != 观察事实状态_已覆盖) 完备失败条件.push_back("像素未覆盖");
        if (结果.像素特征值承接状态 != 观察事实状态_已完成) 完备失败条件.push_back("像素特征值未承接");
        if (结果.坐标回查可用状态 != 观察事实状态_可用) 完备失败条件.push_back("坐标回查不可用");
        if (结果.帧内编号回查可用状态 != 观察事实状态_可用) 完备失败条件.push_back("帧内编号回查不可用");
        结果.观察事实完备失败条件数量 = static_cast<I64>(完备失败条件.size());
        if (完备失败条件.empty()) {
            结果.观察事实完备失败原因摘要 = "无";
        } else {
            std::ostringstream 摘要;
            for (std::size_t i = 0; i < 完备失败条件.size(); ++i) {
                if (i > 0) 摘要 << ';';
                摘要 << 完备失败条件[i];
            }
            结果.观察事实完备失败原因摘要 = 摘要.str();
        }
        结果.质量可用状态 = 观察事实布尔状态(
            结果.观察帧质量诊断摘要 == 1
            && 结果.帧质量评分 >= 观察质量最低评分
            && 结果.深度有效率 >= 观察质量最低深度有效率
            && 结果.空间坐标有效率 >= 观察质量最低空间坐标有效率);
        std::vector<const char*> 质量失败条件{};
        if (结果.观察帧质量诊断摘要 != 1) 质量失败条件.push_back("观察帧质量诊断摘要缺失");
        if (结果.帧质量评分 < 观察质量最低评分) 质量失败条件.push_back("帧质量评分不足");
        if (结果.深度有效率 < 观察质量最低深度有效率) 质量失败条件.push_back("深度有效率不足");
        if (结果.空间坐标有效率 < 观察质量最低空间坐标有效率) 质量失败条件.push_back("空间坐标有效率不足");
        结果.质量失败条件数量 = static_cast<I64>(质量失败条件.size());
        if (质量失败条件.empty()) {
            结果.质量失败原因摘要 = "无";
        } else {
            std::ostringstream 摘要;
            for (std::size_t i = 0; i < 质量失败条件.size(); ++i) {
                if (i > 0) 摘要 << ';';
                摘要 << 质量失败条件[i];
            }
            结果.质量失败原因摘要 = 摘要.str();
        }
        补全当前观察新鲜度状态(结果, 判定时间);
        结果.基础观察事实可用状态 = 观察事实布尔状态(
            结果.当前观察特征帧取得状态 == 观察事实状态_已取得
            && 结果.观察帧容器状态 == 观察事实状态_已建立
            && 结果.像素覆盖状态 == 观察事实状态_已覆盖
            && 结果.像素特征值承接状态 == 观察事实状态_已完成
            && 结果.观察事实完备状态 == 观察事实状态_已完备
            && 结果.质量可用状态 == 观察事实状态_可用
            && 结果.当前观察新鲜度状态 == 观察事实状态_新鲜);

        I64 失败条件 = 0;
        失败条件 += 结果.当前观察特征帧取得状态 == 观察事实状态_已取得 ? 0 : 1;
        失败条件 += 结果.观察帧容器状态 == 观察事实状态_已建立 ? 0 : 1;
        失败条件 += 结果.像素覆盖状态 == 观察事实状态_已覆盖 ? 0 : 1;
        失败条件 += 结果.像素特征值承接状态 == 观察事实状态_已完成 ? 0 : 1;
        失败条件 += 结果.观察事实完备状态 == 观察事实状态_已完备 ? 0 : 1;
        失败条件 += 结果.质量可用状态 == 观察事实状态_可用 ? 0 : 1;
        失败条件 += 结果.当前观察新鲜度状态 == 观察事实状态_新鲜 ? 0 : 1;
        结果.基础观察事实失败条件数量 = 失败条件;
    }

    inline std::string 基础观察事实失败条件摘要(const 结构_双目相机调用结果& 结果)
    {
        std::vector<const char*> 条件{};
        if (结果.当前观察特征帧取得状态 != 观察事实状态_已取得) 条件.push_back("当前观察特征帧未取得");
        if (结果.观察帧容器状态 != 观察事实状态_已建立) 条件.push_back("观察帧容器未建立");
        if (结果.像素覆盖状态 != 观察事实状态_已覆盖) 条件.push_back("像素未覆盖");
        if (结果.像素特征值承接状态 != 观察事实状态_已完成) 条件.push_back("像素特征值未承接");
        if (结果.观察事实完备状态 != 观察事实状态_已完备) 条件.push_back("观察事实未完备");
        if (结果.质量可用状态 != 观察事实状态_可用) 条件.push_back("质量不可用");
        if (结果.当前观察新鲜度状态 != 观察事实状态_新鲜) 条件.push_back("观察不新鲜");
        if (条件.empty()) {
            return "无";
        }
        std::ostringstream 输出;
        for (std::size_t i = 0; i < 条件.size(); ++i) {
            if (i > 0) 输出 << ';';
            输出 << 条件[i];
        }
        return 输出.str();
    }

    inline const 语素入口节点类* 选择基础观察事实首个缺口特征(
        const 结构_双目相机调用结果& 结果,
        I64& 当前状态值,
        I64& 目标状态值) noexcept
    {
        if (结果.当前观察特征帧取得状态 != 观察事实状态_已取得) {
            当前状态值 = 结果.当前观察特征帧取得状态;
            目标状态值 = 观察事实状态_已取得;
            return 特征_当前观察特征帧取得状态();
        }
        if (结果.观察帧容器状态 != 观察事实状态_已建立) {
            当前状态值 = 结果.观察帧容器状态;
            目标状态值 = 观察事实状态_已建立;
            return 特征_观察帧容器状态();
        }
        if (结果.像素覆盖状态 != 观察事实状态_已覆盖) {
            当前状态值 = 结果.像素覆盖状态;
            目标状态值 = 观察事实状态_已覆盖;
            return 特征_像素覆盖状态();
        }
        if (结果.像素特征值承接状态 != 观察事实状态_已完成) {
            当前状态值 = 结果.像素特征值承接状态;
            目标状态值 = 观察事实状态_已完成;
            return 特征_像素特征值承接状态();
        }
        if (结果.观察事实完备状态 != 观察事实状态_已完备) {
            当前状态值 = 结果.观察事实完备状态;
            目标状态值 = 观察事实状态_已完备;
            return 特征_观察事实完备状态();
        }
        if (结果.质量可用状态 != 观察事实状态_可用) {
            当前状态值 = 结果.质量可用状态;
            目标状态值 = 观察事实状态_可用;
            return 特征_质量可用状态();
        }
        if (结果.当前观察新鲜度状态 != 观察事实状态_新鲜) {
            当前状态值 = 结果.当前观察新鲜度状态;
            目标状态值 = 观察事实状态_新鲜;
            return 特征_当前观察新鲜度状态();
        }
        当前状态值 = 结果.基础观察事实可用状态;
        目标状态值 = 观察事实状态_可用;
        return 特征_基础观察事实可用状态();
    }

    struct 结构_空间候选独立性判断结果 {
        I64 判断状态 = 0;
        I64 相对独立数量 = 0;
        I64 不独立数量 = 0;
        I64 待拆分数量 = 0;
        I64 待合并数量 = 0;
        I64 不确定数量 = 0;
        I64 主候选状态 = 空间候选独立状态_不确定;
        I64 主候选分离度评分 = 0;
    };

    struct 结构_空间候选观察队列项 {
        I64 排序位次 = 0;
        I64 候选编号 = 0;
        I64 优先级评分 = 0;
        I64 相对独立状态 = 空间候选独立状态_不确定;
        I64 中心接近评分 = 0;
        I64 范围大小评分 = 0;
    };

    struct 结构_空间候选观察队列结果 {
        I64 队列状态 = 0;
        I64 候选数量 = 0;
        std::vector<结构_空间候选观察队列项> 队列{};
    };

    struct 结构_观察存在假设结果 {
        I64 假设状态 = 观察存在假设状态_未生成;
        I64 来源候选编号 = -1;
        I64 来源队列位次 = -1;
        I64 相对独立状态 = 空间候选独立状态_不确定;
        I64 验证状态 = 0;
        I64 候选像素掩码状态 = 0;
        I64 投影轮廓状态 = 0;
        I64 距离 = 0;
        I64 尺寸X = 0;
        I64 尺寸Y = 0;
        I64 尺寸Z = 0;
        I64 方位X = 0;
        I64 方位Y = 0;
        I64 有效点比例 = 0;
        I64 轮廓支持评分 = 0;
        双目相机本能适配器::空间候选摘要 候选{};
    };

    struct 结构_像素归属验证结果 {
        I64 结果状态 = 像素归属验证状态_未生成;
        I64 来源候选编号 = -1;
        I64 空间覆盖像素数 = 0;
        I64 空间匹配像素数 = 0;
        I64 空间冲突像素数 = 0;
        I64 轮廓支持像素数 = 0;
        I64 轮廓冲突像素数 = 0;
        I64 深度一致率 = 0;
        I64 范围内点比例 = 0;
        I64 投影覆盖率 = 0;
        I64 轮廓吻合率 = 0;
        I64 遮挡像素数 = 0;
        I64 未解释像素数 = 0;
        I64 验证状态 = 像素归属验证状态_未生成;
    };

    struct 结构_观察存在确认结果 {
        I64 确认状态 = 观察存在确认状态_未确认;
        I64 来源候选编号 = -1;
        I64 已验证观察存在数量 = 0;
        I64 像素归属更新 = 0;
        I64 未解释区域更新 = 0;
        I64 帧解释状态 = 帧解释状态_未生成;
        I64 空间连续性评分 = 0;
        I64 范围稳定性评分 = 0;
        I64 范围内点比例 = 0;
        I64 深度一致率 = 0;
        I64 投影覆盖率 = 0;
        I64 轮廓吻合率 = 0;
    };

    struct 结构_已验证观察存在项 {
        结构_观察存在假设结果 假设{};
        结构_像素归属验证结果 验证{};
    };

    struct 结构_外设观察存在确认方案项 {
        I64 方案类型 = 外设观察存在候选确认方案_无;
        I64 来源候选簇ID = 0;
        I64 来源空间候选编号 = -1;
        I64 允许提交状态 = 0;
        I64 匹配分数 = 0;
        I64 冲突原因 = 0;
        I64 证据不足原因 = 0;
        存在节点类* 目标已知存在 = nullptr;
        结构_外设观察像素簇摘要 簇{};
    };

    struct 结构_外设观察存在确认方案 {
        I64 确认状态 = 外设观察存在候选确认状态_未确认;
        I64 方案取得状态 = 0;
        I64 报告ID = 0;
        I64 当前帧ID = 0;
        I64 候选簇数量 = 0;
        I64 新建数量 = 0;
        I64 合并数量 = 0;
        I64 冲突数量 = 0;
        I64 证据不足数量 = 0;
        I64 可提交数量 = 0;
        std::vector<结构_外设观察存在确认方案项> 项集合{};
    };

    struct 结构_当前帧轮廓分区项 {
        存在节点类* 已确认观察存在 = nullptr;
        I64 序号 = 0;
        I64 预测最小X = 0;
        I64 预测最大X = 0;
        I64 预测最小Y = 0;
        I64 预测最大Y = 0;
        I64 匹配簇ID = 0;
        I64 匹配分数 = 0;
        I64 像素残差 = 0;
        I64 深度残差 = 0;
        I64 轮廓残差 = 0;
        I64 扫描状态 = 已知存在当前帧扫描状态_未生成;
        I64 遮挡状态 = 当前帧遮挡判断状态_未判断;
        I64 冲突状态 = 当前帧轮廓冲突区域明确状态_无冲突;
    };

    struct 结构_当前帧轮廓分区结果 {
        I64 分区状态 = 当前帧轮廓分区状态_未生成;
        I64 报告ID = 0;
        I64 当前帧ID = 0;
        I64 已确认存在数量 = 0;
        I64 已知扫描区数量 = 0;
        I64 新识别区数量 = 0;
        I64 冲突区数量 = 0;
        I64 待重识别区数量 = 0;
        I64 像素归属闭合状态 = 当前帧像素归属闭合状态_未闭合;
        std::vector<结构_当前帧轮廓分区项> 项集合{};
    };

    struct 结构_当前场景特征值材料门项 {
        存在节点类* 已确认观察存在 = nullptr;
        I64 序号 = 0;
        I64 状态 = 当前场景特征值可读状态_证据不足;
        I64 可读特征类型掩码 = 0;
        I64 缺失原因掩码 = 当前场景特征值缺失原因_无;
        I64 过期原因掩码 = 当前场景特征值过期原因_无;
        I64 冲突原因掩码 = 当前场景特征值冲突原因_无;
        I64 来源报告ID = 0;
        I64 来源像素簇ID = 0;
        I64 来源报告时间戳毫秒 = 0;
        I64 当前场景在场关系状态 = 0;
        I64 可回查材料数量 = 0;
        I64 位置或空间范围可读状态 = 0;
        I64 深度空间质量可读状态 = 0;
        I64 跨帧稳定性可读状态 = 0;
        I64 场景绝对坐标明确状态 = 0;
        I64 场景绝对空间范围明确状态 = 0;
    };

    struct 结构_当前场景特征值材料门结果 {
        I64 版本 = 当前场景特征值材料门版本_v1;
        I64 状态 = 当前场景特征值可读状态_证据不足;
        I64 当前场景存在数量 = 0;
        I64 可读存在数量 = 0;
        I64 部分可读存在数量 = 0;
        I64 证据不足存在数量 = 0;
        I64 材料缺失存在数量 = 0;
        I64 材料过期存在数量 = 0;
        I64 材料冲突存在数量 = 0;
        I64 可读特征类型掩码 = 0;
        I64 缺失原因掩码 = 当前场景特征值缺失原因_无;
        I64 过期原因掩码 = 当前场景特征值过期原因_无;
        I64 冲突原因掩码 = 当前场景特征值冲突原因_无;
        std::vector<结构_当前场景特征值材料门项> 项集合{};
    };

    struct 结构_安全因果候选条目 {
        I64 序号 = 0;
        存在节点类* 候选存在 = nullptr;
        const 语素入口节点类* 特征类型 = nullptr;
        I64 当前特征值读取状态 = 安全因果候选当前特征值状态_缺失;
        I64 当前特征值 = 0;
        I64 前一特征值 = 0;
        I64 前一特征值缺口状态 = 1;
        I64 变化方向 = 风险安全场景影响变化方向_初始入账;
        I64 时间窗口 = 0;
        I64 来源观察报告 = 0;
        I64 归因证据状态 = 安全因果候选证据状态_未搜索;
        I64 负向证据状态 = 安全因果候选证据状态_未搜索;
        I64 候选置信度 = 0;
    };

    struct 结构_安全因果候选构造结果 {
        I64 构造状态 = 安全明确状态_未明确;
        I64 候选数量 = 0;
        I64 低置信数量 = 0;
        I64 归因置信度 = 0;
        I64 缺当前特征值数量 = 0;
        I64 搜索未完成数量 = 0;
        I64 候选明细写入数量 = 0;
        const 语素入口节点类* 首个缺失特征 = nullptr;
        std::vector<结构_安全因果候选条目> 条目集合{};
    };

    struct 结构_全帧像素归属账本结果 {
        I64 账本状态 = 全帧像素归属账状态_未生成;
        I64 已验证观察存在数量 = 0;
        I64 预期像素数量 = 0;
        I64 已归属像素数 = 0;
        I64 未解释像素数 = 0;
        I64 归属冲突像素数 = 0;
        I64 像素归属率 = 0;
    };

    inline I64 正差I64(I64 最大值, I64 最小值) noexcept
    {
        return 最大值 >= 最小值 ? 最大值 - 最小值 : 0;
    }

    inline I64 绝对差I64(I64 a, I64 b) noexcept
    {
        return a >= b ? a - b : b - a;
    }

    inline I64 区间间隔I64(I64 aMin, I64 aMax, I64 bMin, I64 bMax) noexcept
    {
        if (aMax < bMin) return bMin - aMax;
        if (bMax < aMin) return aMin - bMax;
        return 0;
    }

    inline bool 空间候选范围有效(const 双目相机本能适配器::空间候选摘要& 候选) noexcept
    {
        return 候选.像素数量 > 0
            && 候选.范围最大X >= 候选.范围最小X
            && 候选.范围最大Y >= 候选.范围最小Y
            && 候选.范围最大Z >= 候选.范围最小Z;
    }

    inline I64 空间候选AABB间隔(
        const 双目相机本能适配器::空间候选摘要& A,
        const 双目相机本能适配器::空间候选摘要& B) noexcept
    {
        const I64 x = 区间间隔I64(A.范围最小X, A.范围最大X, B.范围最小X, B.范围最大X);
        const I64 y = 区间间隔I64(A.范围最小Y, A.范围最大Y, B.范围最小Y, B.范围最大Y);
        const I64 z = 区间间隔I64(A.范围最小Z, A.范围最大Z, B.范围最小Z, B.范围最大Z);
        return std::max(x, std::max(y, z));
    }

    inline I64 空间候选最大范围跨度(const 双目相机本能适配器::空间候选摘要& 候选) noexcept
    {
        const I64 x = 正差I64(候选.范围最大X, 候选.范围最小X);
        const I64 y = 正差I64(候选.范围最大Y, 候选.范围最小Y);
        const I64 z = 正差I64(候选.范围最大Z, 候选.范围最小Z);
        return std::max(x, std::max(y, z));
    }

    inline I64 计算空间候选分离度评分(
        const std::vector<双目相机本能适配器::空间候选摘要>& 候选集,
        std::size_t 索引) noexcept
    {
        if (索引 >= 候选集.size()) return 0;
        if (候选集.size() <= 1) return 10000;

        I64 最近间隔 = std::numeric_limits<I64>::max();
        const auto& 当前 = 候选集[索引];
        for (std::size_t i = 0; i < 候选集.size(); ++i) {
            if (i == 索引) continue;
            最近间隔 = std::min(最近间隔, 空间候选AABB间隔(当前, 候选集[i]));
        }
        if (最近间隔 == std::numeric_limits<I64>::max()) return 10000;
        if (最近间隔 <= 0) return 2000;
        if (最近间隔 >= 600) return 10000;
        return 2000 + 最近间隔 * 8000 / 600;
    }

    inline bool 空间候选存在待合并邻居(
        const std::vector<双目相机本能适配器::空间候选摘要>& 候选集,
        std::size_t 索引) noexcept
    {
        if (索引 >= 候选集.size() || 候选集.size() <= 1) return false;
        const auto& 当前 = 候选集[索引];
        for (std::size_t i = 0; i < 候选集.size(); ++i) {
            if (i == 索引) continue;
            const auto& 其他 = 候选集[i];
            if (空间候选AABB间隔(当前, 其他) <= 120
                && 绝对差I64(当前.中心Z, 其他.中心Z) <= 180) {
                return true;
            }
        }
        return false;
    }

    inline I64 判断单个空间候选独立状态(
        const std::vector<双目相机本能适配器::空间候选摘要>& 候选集,
        std::size_t 索引) noexcept
    {
        if (索引 >= 候选集.size()) return 空间候选独立状态_不确定;
        const auto& 候选 = 候选集[索引];
        if (!空间候选范围有效(候选)
            || 候选.空间连续性评分 < 4500
            || 候选.范围稳定性评分 < 4500) {
            return 空间候选独立状态_不独立;
        }
        if (空间候选存在待合并邻居(候选集, 索引)) {
            return 空间候选独立状态_待合并;
        }

        const I64 深度跨度 = 正差I64(候选.最远深度, 候选.最近深度);
        const I64 拆分深度阈值 = std::max<I64>(300, 绝对差I64(候选.平均深度, 0) / 5);
        if (候选.像素数量 >= 256
            && 深度跨度 > 拆分深度阈值
            && 空间候选最大范围跨度(候选) > 350) {
            return 空间候选独立状态_待拆分;
        }

        const I64 分离度评分 = 计算空间候选分离度评分(候选集, 索引);
        if (候选.空间连续性评分 >= 6500
            && 候选.范围稳定性评分 >= 6500
            && 分离度评分 >= 5000
            && 候选.有效点比例 > 0) {
            return 空间候选独立状态_相对独立;
        }
        return 空间候选独立状态_不确定;
    }

    inline 结构_空间候选独立性判断结果 计算空间候选独立性判断(
        const 结构_双目相机调用结果& 结果) noexcept
    {
        结构_空间候选独立性判断结果 输出{};
        输出.判断状态 = 结果.成功 ? 1 : 0;
        for (std::size_t i = 0; i < 结果.空间候选列表.size(); ++i) {
            const I64 状态 = 判断单个空间候选独立状态(结果.空间候选列表, i);
            if (i == 0) {
                输出.主候选状态 = 状态;
                输出.主候选分离度评分 = 计算空间候选分离度评分(结果.空间候选列表, i);
            }
            switch (状态) {
            case 空间候选独立状态_相对独立:
                ++输出.相对独立数量;
                break;
            case 空间候选独立状态_不独立:
                ++输出.不独立数量;
                break;
            case 空间候选独立状态_待拆分:
                ++输出.待拆分数量;
                break;
            case 空间候选独立状态_待合并:
                ++输出.待合并数量;
                break;
            default:
                ++输出.不确定数量;
                break;
            }
        }
        return 输出;
    }

    inline I64 计算空间候选中心接近评分(
        const 双目相机本能适配器::空间候选摘要& 候选,
        int 宽度,
        int 高度) noexcept
    {
        if (宽度 <= 0 || 高度 <= 0) return 0;
        const I64 中心X2 = 候选.投影最小X + 候选.投影最大X;
        const I64 中心Y2 = 候选.投影最小Y + 候选.投影最大Y;
        const I64 dx = 绝对差I64(中心X2, static_cast<I64>(宽度));
        const I64 dy = 绝对差I64(中心Y2, static_cast<I64>(高度));
        const I64 分子 = dx * dx + dy * dy;
        const I64 分母 = static_cast<I64>(宽度) * static_cast<I64>(宽度)
            + static_cast<I64>(高度) * static_cast<I64>(高度);
        if (分母 <= 0) return 0;
        const I64 扣分 = std::min<I64>(10000, 分子 * 10000 / 分母);
        return 10000 - 扣分;
    }

    inline I64 空间候选独立状态惩罚(I64 状态) noexcept
    {
        switch (状态) {
        case 空间候选独立状态_相对独立:
            return 0;
        case 空间候选独立状态_待合并:
            return 1000;
        case 空间候选独立状态_待拆分:
            return 1500;
        case 空间候选独立状态_不确定:
            return 500;
        case 空间候选独立状态_不独立:
            return 4000;
        default:
            return 750;
        }
    }

    inline I64 空间候选观察状态排序优先级(I64 状态) noexcept
    {
        switch (状态) {
        case 空间候选独立状态_相对独立:
            return 0;
        case 空间候选独立状态_待合并:
            return 1;
        case 空间候选独立状态_不确定:
            return 2;
        case 空间候选独立状态_待拆分:
            return 3;
        case 空间候选独立状态_不独立:
            return 4;
        default:
            return 5;
        }
    }

    inline 结构_空间候选观察队列结果 生成空间候选观察队列(
        const 结构_双目相机调用结果& 结果) noexcept
    {
        结构_空间候选观察队列结果 输出{};
        输出.队列状态 = 结果.成功 ? 1 : 0;
        if (!结果.成功 || 结果.空间候选列表.empty()) {
            return 输出;
        }

        I64 最大像素数量 = 0;
        for (const auto& 候选 : 结果.空间候选列表) {
            最大像素数量 = std::max(最大像素数量, 候选.像素数量);
        }
        if (最大像素数量 <= 0) 最大像素数量 = 1;

        输出.队列.reserve(结果.空间候选列表.size());
        for (std::size_t i = 0; i < 结果.空间候选列表.size(); ++i) {
            const auto& 候选 = 结果.空间候选列表[i];
            结构_空间候选观察队列项 项{};
            项.候选编号 = 候选.候选编号;
            项.相对独立状态 = 判断单个空间候选独立状态(结果.空间候选列表, i);
            项.中心接近评分 = 计算空间候选中心接近评分(候选, 结果.宽度, 结果.高度);
            项.范围大小评分 = std::clamp<I64>(候选.像素数量 * 10000 / 最大像素数量, 0, 10000);
            const I64 有效点比例评分 = std::clamp<I64>(候选.有效点比例, 0, 10000);
            const I64 基础分 =
                候选.空间连续性评分 * 30 / 100
                + 候选.范围稳定性评分 * 25 / 100
                + 项.中心接近评分 * 20 / 100
                + 有效点比例评分 * 15 / 100
                + 项.范围大小评分 * 10 / 100;
            项.优先级评分 = std::clamp<I64>(
                基础分 - 空间候选独立状态惩罚(项.相对独立状态),
                0,
                10000);
            输出.队列.push_back(项);
        }
        std::sort(
            输出.队列.begin(),
            输出.队列.end(),
            [](const auto& a, const auto& b) noexcept {
                const I64 状态优先级A = 空间候选观察状态排序优先级(a.相对独立状态);
                const I64 状态优先级B = 空间候选观察状态排序优先级(b.相对独立状态);
                if (状态优先级A != 状态优先级B) {
                    return 状态优先级A < 状态优先级B;
                }
                if (a.优先级评分 != b.优先级评分) {
                    return a.优先级评分 > b.优先级评分;
                }
                return a.候选编号 < b.候选编号;
            });
        for (std::size_t i = 0; i < 输出.队列.size(); ++i) {
            输出.队列[i].排序位次 = static_cast<I64>(i);
        }
        输出.候选数量 = static_cast<I64>(输出.队列.size());
        return 输出;
    }

    inline const 双目相机本能适配器::空间候选摘要* 查找空间候选摘要(
        const std::vector<双目相机本能适配器::空间候选摘要>& 候选集,
        I64 候选编号) noexcept
    {
        for (const auto& 候选 : 候选集) {
            if (候选.候选编号 == 候选编号) {
                return &候选;
            }
        }
        return nullptr;
    }

    inline I64 计算投影轮廓状态(const 双目相机本能适配器::空间候选摘要& 候选) noexcept
    {
        return 候选.投影最大X >= 候选.投影最小X
            && 候选.投影最大Y >= 候选.投影最小Y
            ? 1
            : 0;
    }

    inline I64 计算轮廓支持评分(const 双目相机本能适配器::空间候选摘要& 候选) noexcept
    {
        if (!计算投影轮廓状态(候选)) return 0;
        const I64 投影宽 = 正差I64(候选.投影最大X, 候选.投影最小X) + 1;
        const I64 投影高 = 正差I64(候选.投影最大Y, 候选.投影最小Y) + 1;
        const I64 投影面积 = std::max<I64>(1, 投影宽 * 投影高);
        const I64 覆盖比例 = std::clamp<I64>(候选.像素数量 * 10000 / 投影面积, 0, 10000);
        return 覆盖比例;
    }

    inline I64 计算比例万分比I64(I64 分子, I64 分母) noexcept
    {
        if (分子 <= 0 || 分母 <= 0) return 0;
        return std::clamp<I64>(分子 * 10000 / 分母, 0, 10000);
    }

    inline void 累加非负I64(I64& 累计, I64 值) noexcept
    {
        if (值 <= 0) return;
        const I64 上限 = std::numeric_limits<I64>::max();
        累计 = 累计 > 上限 - 值 ? 上限 : 累计 + 值;
    }

    inline 双目相机本能适配器::空间候选摘要 合并相邻待合并空间候选(
        const std::vector<双目相机本能适配器::空间候选摘要>& 候选集,
        const 双目相机本能适配器::空间候选摘要& 基准,
        bool& 已合并) noexcept
    {
        auto 输出 = 基准;
        已合并 = false;
        I64 总像素 = std::max<I64>(0, 基准.像素数量);
        I64 加权X = 基准.中心X * std::max<I64>(1, 基准.像素数量);
        I64 加权Y = 基准.中心Y * std::max<I64>(1, 基准.像素数量);
        I64 加权Z = 基准.中心Z * std::max<I64>(1, 基准.像素数量);

        for (const auto& 其他 : 候选集) {
            if (其他.候选编号 == 基准.候选编号) {
                continue;
            }
            if (空间候选AABB间隔(基准, 其他) > 120
                || 绝对差I64(基准.中心Z, 其他.中心Z) > 180) {
                continue;
            }

            已合并 = true;
            const I64 像素 = std::max<I64>(1, 其他.像素数量);
            总像素 += 其他.像素数量;
            加权X += 其他.中心X * 像素;
            加权Y += 其他.中心Y * 像素;
            加权Z += 其他.中心Z * 像素;
            输出.范围最小X = std::min(输出.范围最小X, 其他.范围最小X);
            输出.范围最大X = std::max(输出.范围最大X, 其他.范围最大X);
            输出.范围最小Y = std::min(输出.范围最小Y, 其他.范围最小Y);
            输出.范围最大Y = std::max(输出.范围最大Y, 其他.范围最大Y);
            输出.范围最小Z = std::min(输出.范围最小Z, 其他.范围最小Z);
            输出.范围最大Z = std::max(输出.范围最大Z, 其他.范围最大Z);
            输出.投影最小X = std::min(输出.投影最小X, 其他.投影最小X);
            输出.投影最大X = std::max(输出.投影最大X, 其他.投影最大X);
            输出.投影最小Y = std::min(输出.投影最小Y, 其他.投影最小Y);
            输出.投影最大Y = std::max(输出.投影最大Y, 其他.投影最大Y);
            输出.最近深度 = std::min(输出.最近深度, 其他.最近深度);
            输出.最远深度 = std::max(输出.最远深度, 其他.最远深度);
            输出.空间连续性评分 = std::min(输出.空间连续性评分, 其他.空间连续性评分);
            输出.范围稳定性评分 = std::min(输出.范围稳定性评分, 其他.范围稳定性评分);
            输出.有效点比例 = std::min<I64>(10000, 输出.有效点比例 + 其他.有效点比例);
            输出.空间覆盖像素数 += 其他.空间覆盖像素数;
            输出.空间匹配像素数 += 其他.空间匹配像素数;
            输出.空间冲突像素数 += 其他.空间冲突像素数;
            输出.轮廓支持像素数 += 其他.轮廓支持像素数;
            输出.轮廓冲突像素数 += 其他.轮廓冲突像素数;
            输出.遮挡像素数 += 其他.遮挡像素数;
            输出.未解释像素数 += 其他.未解释像素数;
            输出.深度一致率 = std::min(输出.深度一致率, 其他.深度一致率);
            输出.范围内点比例 = std::min(输出.范围内点比例, 其他.范围内点比例);
            输出.投影覆盖率 = std::min(输出.投影覆盖率, 其他.投影覆盖率);
        }

        if (!已合并 || 总像素 <= 0) {
            return 输出;
        }

        输出.像素数量 = 总像素;
        输出.中心X = 加权X / 总像素;
        输出.中心Y = 加权Y / 总像素;
        输出.中心Z = 加权Z / 总像素;
        输出.平均深度 = 输出.中心Z;
        const I64 投影宽 = 正差I64(输出.投影最大X, 输出.投影最小X) + 1;
        const I64 投影高 = 正差I64(输出.投影最大Y, 输出.投影最小Y) + 1;
        const I64 投影面积 = std::max<I64>(1, 投影宽 * 投影高);
        输出.轮廓吻合率 = 计算比例万分比I64(输出.轮廓支持像素数, 投影面积);
        输出.轮廓冲突像素数 = std::max<I64>(0, 投影面积 - 输出.轮廓支持像素数);
        输出.未解释像素数 = 输出.轮廓冲突像素数;
        输出.像素归属验证状态 = 输出.空间匹配像素数 > 0 ? 1 : 0;
        return 输出;
    }

    inline 结构_观察存在假设结果 构建观察存在假设(
        const 结构_双目相机调用结果& 结果,
        const 结构_空间候选观察队列项& 队列项) noexcept
    {
        结构_观察存在假设结果 输出{};
        if (!结果.成功) {
            return 输出;
        }
        const auto* 候选 = 查找空间候选摘要(
            结果.空间候选列表,
            队列项.候选编号);
        if (!候选) {
            return 输出;
        }
        bool 已合并 = false;
        const auto 候选摘要 = 队列项.相对独立状态 == 空间候选独立状态_待合并
            ? 合并相邻待合并空间候选(结果.空间候选列表, *候选, 已合并)
            : *候选;
        输出.假设状态 = 观察存在假设状态_已生成;
        输出.来源候选编号 = 候选->候选编号;
        输出.来源队列位次 = 队列项.排序位次;
        输出.相对独立状态 = 已合并
            ? 空间候选独立状态_相对独立
            : 队列项.相对独立状态;
        输出.验证状态 = 观察存在假设验证状态_待验证;
        输出.候选像素掩码状态 = 候选摘要.像素数量 > 0 ? 1 : 0;
        输出.投影轮廓状态 = 计算投影轮廓状态(候选摘要);
        输出.距离 = 候选摘要.中心Z;
        输出.尺寸X = 正差I64(候选摘要.范围最大X, 候选摘要.范围最小X);
        输出.尺寸Y = 正差I64(候选摘要.范围最大Y, 候选摘要.范围最小Y);
        输出.尺寸Z = 正差I64(候选摘要.范围最大Z, 候选摘要.范围最小Z);
        输出.方位X = 候选摘要.中心X;
        输出.方位Y = 候选摘要.中心Y;
        输出.有效点比例 = 候选摘要.有效点比例;
        输出.轮廓支持评分 = 计算轮廓支持评分(候选摘要);
        输出.候选 = 候选摘要;
        return 输出;
    }

    inline 结构_观察存在假设结果 组合观察存在假设(
        const 结构_双目相机调用结果& 结果,
        const 结构_空间候选观察队列结果& 队列) noexcept
    {
        结构_观察存在假设结果 输出{};
        if (!结果.成功 || 队列.队列.empty()) {
            return 输出;
        }

        for (const auto& 队列项 : 队列.队列) {
            if (队列项.相对独立状态 != 空间候选独立状态_相对独立) {
                continue;
            }
            auto 候选假设 = 构建观察存在假设(结果, 队列项);
            if (候选假设.假设状态 == 观察存在假设状态_已生成) {
                return 候选假设;
            }
        }

        return 构建观察存在假设(结果, 队列.队列.front());
    }

    inline 结构_像素归属验证结果 组合像素归属验证结果(
        const 结构_双目相机调用结果& 结果,
        const 结构_观察存在假设结果& 假设) noexcept
    {
        结构_像素归属验证结果 输出{};
        if (!结果.成功 || 假设.假设状态 != 观察存在假设状态_已生成) {
            return 输出;
        }
        输出.结果状态 = 像素归属验证状态_已生成待确认;
        输出.来源候选编号 = 假设.来源候选编号;
        输出.空间覆盖像素数 = 假设.候选.空间覆盖像素数;
        输出.空间匹配像素数 = 假设.候选.空间匹配像素数;
        输出.空间冲突像素数 = 假设.候选.空间冲突像素数;
        输出.轮廓支持像素数 = 假设.候选.轮廓支持像素数;
        输出.轮廓冲突像素数 = 假设.候选.轮廓冲突像素数;
        输出.深度一致率 = 假设.候选.深度一致率;
        输出.范围内点比例 = 假设.候选.范围内点比例;
        输出.投影覆盖率 = 假设.候选.投影覆盖率;
        输出.轮廓吻合率 = 假设.候选.轮廓吻合率;
        输出.遮挡像素数 = 假设.候选.遮挡像素数;
        输出.未解释像素数 = 假设.候选.未解释像素数;
        输出.验证状态 = 假设.候选.像素归属验证状态 > 0
            ? 像素归属验证状态_已生成待确认
            : 像素归属验证状态_未生成;
        return 输出;
    }

    inline bool 观察存在空间证据通过(
        const 结构_观察存在假设结果& 假设) noexcept
    {
        return 假设.候选.空间连续性评分 >= 6500
            && 假设.候选.范围稳定性评分 >= 6500
            && 假设.候选.像素数量 >= 32;
    }

    inline bool 观察存在归属证据通过(
        const 结构_像素归属验证结果& 验证) noexcept
    {
        return 验证.空间匹配像素数 >= 32
            && 验证.范围内点比例 >= 6500
            && 验证.深度一致率 >= 6500
            && 验证.投影覆盖率 >= 5000;
    }

    inline bool 观察存在候选已验证(
        const 结构_观察存在假设结果& 假设,
        const 结构_像素归属验证结果& 验证) noexcept
    {
        return 假设.假设状态 == 观察存在假设状态_已生成
            && 假设.相对独立状态 == 空间候选独立状态_相对独立
            && 验证.结果状态 == 像素归属验证状态_已生成待确认
            && 验证.验证状态 == 像素归属验证状态_已生成待确认
            && 空间候选范围有效(假设.候选)
            && 观察存在空间证据通过(假设)
            && 观察存在归属证据通过(验证);
    }

    inline I64 统计已验证观察存在数量(
        const 结构_双目相机调用结果& 结果,
        const 结构_空间候选观察队列结果& 队列) noexcept
    {
        if (!结果.成功) {
            return 0;
        }
        I64 数量 = 0;
        for (const auto& 队列项 : 队列.队列) {
            if (队列项.相对独立状态 != 空间候选独立状态_相对独立) {
                continue;
            }
            const auto 假设 = 构建观察存在假设(结果, 队列项);
            const auto 验证 = 组合像素归属验证结果(结果, 假设);
            if (观察存在候选已验证(假设, 验证)) {
                ++数量;
            }
        }
        return 数量;
    }

    inline std::vector<结构_已验证观察存在项> 生成已验证观察存在项集合(
        const 结构_双目相机调用结果& 结果,
        const 结构_空间候选观察队列结果& 队列,
        const 结构_观察存在假设结果& 主假设,
        const 结构_像素归属验证结果& 主验证) noexcept
    {
        std::vector<结构_已验证观察存在项> 输出{};
        if (!结果.成功) {
            return 输出;
        }
        for (const auto& 队列项 : 队列.队列) {
            if (队列项.相对独立状态 != 空间候选独立状态_相对独立) {
                continue;
            }
            const auto 假设 = 构建观察存在假设(结果, 队列项);
            const auto 验证 = 组合像素归属验证结果(结果, 假设);
            if (观察存在候选已验证(假设, 验证)) {
                输出.push_back({ 假设, 验证 });
            }
        }
        if (观察存在候选已验证(主假设, 主验证)) {
            const bool 已存在 = std::any_of(
                输出.begin(),
                输出.end(),
                [&](const auto& 项) noexcept {
                    return 项.假设.来源候选编号 == 主假设.来源候选编号;
                });
            if (!已存在) {
                输出.push_back({ 主假设, 主验证 });
            }
        }
        return 输出;
    }

    inline I64 外设簇投影面积(const 结构_外设观察像素簇摘要& 簇) noexcept
    {
        if (簇.投影最大X < 簇.投影最小X || 簇.投影最大Y < 簇.投影最小Y) return 0;
        return (簇.投影最大X - 簇.投影最小X + 1)
            * (簇.投影最大Y - 簇.投影最小Y + 1);
    }

    inline I64 外设簇与投影框重叠率(
        const 结构_外设观察像素簇摘要& 簇,
        I64 最小X,
        I64 最大X,
        I64 最小Y,
        I64 最大Y) noexcept
    {
        const I64 面积A = 外设簇投影面积(簇);
        if (面积A <= 0 || 最大X < 最小X || 最大Y < 最小Y) return 0;
        const I64 面积B = (最大X - 最小X + 1) * (最大Y - 最小Y + 1);
        if (面积B <= 0) return 0;
        const I64 交最小X = std::max<I64>(簇.投影最小X, 最小X);
        const I64 交最大X = std::min<I64>(簇.投影最大X, 最大X);
        const I64 交最小Y = std::max<I64>(簇.投影最小Y, 最小Y);
        const I64 交最大Y = std::min<I64>(簇.投影最大Y, 最大Y);
        if (交最大X < 交最小X || 交最大Y < 交最小Y) return 0;
        const I64 交面积 = (交最大X - 交最小X + 1) * (交最大Y - 交最小Y + 1);
        return std::clamp<I64>(交面积 * 10000 / std::max<I64>(1, std::min(面积A, 面积B)), 0, 10000);
    }

    inline I64 外设簇与已确认观察存在匹配分数(
        const 结构_外设观察像素簇摘要& 簇,
        存在节点类* 已知存在) noexcept
    {
        auto* 节点 = reinterpret_cast<基础信息节点类*>(已知存在);
        if (!节点) return 0;
        I64 最小X = 0;
        I64 最大X = 0;
        I64 最小Y = 0;
        I64 最大Y = 0;
        if (!读取基础节点I64(节点, 特征_投影范围最小X(), 最小X)
            || !读取基础节点I64(节点, 特征_投影范围最大X(), 最大X)
            || !读取基础节点I64(节点, 特征_投影范围最小Y(), 最小Y)
            || !读取基础节点I64(节点, 特征_投影范围最大Y(), 最大Y)) {
            return 0;
        }
        const I64 重叠率 = 外设簇与投影框重叠率(簇, 最小X, 最大X, 最小Y, 最大Y);
        const I64 已知中心X2 = 最小X + 最大X;
        const I64 已知中心Y2 = 最小Y + 最大Y;
        const I64 当前中心X2 = 簇.投影最小X + 簇.投影最大X;
        const I64 当前中心Y2 = 簇.投影最小Y + 簇.投影最大Y;
        const I64 中心距离 = 绝对差I64(已知中心X2, 当前中心X2)
            + 绝对差I64(已知中心Y2, 当前中心Y2);
        const I64 中心评分 = std::clamp<I64>(10000 - 中心距离 * 10000 / 600, 0, 10000);
        I64 已知Z = 0;
        (void)读取基础节点I64(节点, 特征_中心空间坐标Z(), 已知Z);
        const I64 深度评分 = std::clamp<I64>(10000 - 绝对差I64(已知Z, 簇.中心Z) * 10000 / 800, 0, 10000);
        return std::clamp<I64>(
            重叠率 * 60 / 100 + 中心评分 * 25 / 100 + 深度评分 * 15 / 100,
            0,
            10000);
    }

    inline bool 读取已确认观察存在投影ROI(
        存在节点类* 已知存在,
        I64 帧宽度,
        I64 帧高度,
        I64 扩张像素,
        I64& 最小X,
        I64& 最大X,
        I64& 最小Y,
        I64& 最大Y) noexcept
    {
        auto* 节点 = reinterpret_cast<基础信息节点类*>(已知存在);
        if (!节点 || 帧宽度 <= 0 || 帧高度 <= 0) return false;
        if (!读取基础节点I64(节点, 特征_投影范围最小X(), 最小X)
            || !读取基础节点I64(节点, 特征_投影范围最大X(), 最大X)
            || !读取基础节点I64(节点, 特征_投影范围最小Y(), 最小Y)
            || !读取基础节点I64(节点, 特征_投影范围最大Y(), 最大Y)) {
            return false;
        }
        if (最大X < 最小X || 最大Y < 最小Y) return false;
        最小X = std::clamp<I64>(最小X - 扩张像素, 0, 帧宽度 - 1);
        最大X = std::clamp<I64>(最大X + 扩张像素, 0, 帧宽度 - 1);
        最小Y = std::clamp<I64>(最小Y - 扩张像素, 0, 帧高度 - 1);
        最大Y = std::clamp<I64>(最大Y + 扩张像素, 0, 帧高度 - 1);
        return 最大X >= 最小X && 最大Y >= 最小Y;
    }

    inline I64 ROI面积(I64 最小X, I64 最大X, I64 最小Y, I64 最大Y) noexcept
    {
        if (最大X < 最小X || 最大Y < 最小Y) return 0;
        return (最大X - 最小X + 1) * (最大Y - 最小Y + 1);
    }

    inline I64 ROI重叠面积(
        I64 A最小X,
        I64 A最大X,
        I64 A最小Y,
        I64 A最大Y,
        I64 B最小X,
        I64 B最大X,
        I64 B最小Y,
        I64 B最大Y) noexcept
    {
        const I64 交最小X = std::max<I64>(A最小X, B最小X);
        const I64 交最大X = std::min<I64>(A最大X, B最大X);
        const I64 交最小Y = std::max<I64>(A最小Y, B最小Y);
        const I64 交最大Y = std::min<I64>(A最大Y, B最大Y);
        return ROI面积(交最小X, 交最大X, 交最小Y, 交最大Y);
    }

    inline I64 分区簇与预测ROI匹配分数(
        const 结构_外设观察像素簇摘要& 簇,
        I64 最小X,
        I64 最大X,
        I64 最小Y,
        I64 最大Y,
        I64 已知Z) noexcept
    {
        const I64 重叠率 = 外设簇与投影框重叠率(簇, 最小X, 最大X, 最小Y, 最大Y);
        const I64 已知中心X2 = 最小X + 最大X;
        const I64 已知中心Y2 = 最小Y + 最大Y;
        const I64 当前中心X2 = 簇.投影最小X + 簇.投影最大X;
        const I64 当前中心Y2 = 簇.投影最小Y + 簇.投影最大Y;
        const I64 中心距离 = 绝对差I64(已知中心X2, 当前中心X2)
            + 绝对差I64(已知中心Y2, 当前中心Y2);
        const I64 中心评分 = std::clamp<I64>(10000 - 中心距离 * 10000 / 800, 0, 10000);
        const I64 深度评分 = 已知Z > 0
            ? std::clamp<I64>(10000 - 绝对差I64(已知Z, 簇.中心Z) * 10000 / 1000, 0, 10000)
            : 5000;
        return std::clamp<I64>(
            重叠率 * 65 / 100 + 中心评分 * 25 / 100 + 深度评分 * 10 / 100,
            0,
            10000);
    }

    inline bool 当前帧轮廓分区项ROI有效(const 结构_当前帧轮廓分区项& 项) noexcept
    {
        return 项.预测最大X > 项.预测最小X
            && 项.预测最大Y > 项.预测最小Y;
    }

    inline I64 计算当前帧分区项区域归属状态(const 结构_当前帧轮廓分区项& 项) noexcept
    {
        if (项.冲突状态 == 当前帧轮廓冲突区域明确状态_有冲突) {
            return 当前帧区域归属状态_冲突区域;
        }
        if (!当前帧轮廓分区项ROI有效(项)) {
            return 当前帧区域归属状态_无效区域;
        }
        if (项.扫描状态 == 已知存在当前帧扫描状态_已生成) {
            return 当前帧区域归属状态_已识别区域;
        }
        if (项.扫描状态 == 已知存在当前帧扫描状态_待重识别) {
            return 当前帧区域归属状态_候选区域;
        }
        return 当前帧区域归属状态_未归属;
    }

    inline I64 计算当前帧分区结果区域归属状态(const 结构_当前帧轮廓分区结果& 分区) noexcept
    {
        if (分区.分区状态 == 当前帧轮廓分区状态_未生成
            || 分区.分区状态 == 当前帧轮廓分区状态_条件不足) {
            return 当前帧区域归属状态_未归属;
        }
        if (分区.冲突区数量 > 0) {
            return 当前帧区域归属状态_冲突区域;
        }
        if (std::any_of(
                分区.项集合.begin(),
                分区.项集合.end(),
                [](const auto& 项) noexcept { return !当前帧轮廓分区项ROI有效(项); })) {
            return 当前帧区域归属状态_无效区域;
        }
        if (分区.待重识别区数量 > 0) {
            return 当前帧区域归属状态_候选区域;
        }
        if (分区.新识别区数量 > 0) {
            return 当前帧区域归属状态_未识别区域;
        }
        if (分区.已知扫描区数量 > 0) {
            return 当前帧区域归属状态_已识别区域;
        }
        if (分区.像素归属闭合状态 == 当前帧像素归属闭合状态_已闭合) {
            return 当前帧区域归属状态_背景区域;
        }
        return 当前帧区域归属状态_未归属;
    }

    inline 结构_当前帧轮廓分区结果 构造当前帧轮廓分区结果(
        const 结构_外设观察报告队列项& 报告,
        const std::vector<存在节点类*>& 已确认观察存在集) noexcept
    {
        结构_当前帧轮廓分区结果 输出{};
        输出.报告ID = static_cast<I64>(报告.报告ID);
        输出.当前帧ID = 报告.深度帧号 > 0 ? 报告.深度帧号 : 报告.彩色帧号;
        输出.已确认存在数量 = static_cast<I64>(已确认观察存在集.size());
        if (报告.帧宽度 <= 0 || 报告.帧高度 <= 0 || 已确认观察存在集.empty()) {
            输出.分区状态 = 当前帧轮廓分区状态_条件不足;
            return 输出;
        }

        const I64 匹配阈值 = 4500;
        const I64 扩张像素 = 12;
        I64 已知扫描面积近似 = 0;
        输出.项集合.reserve(已确认观察存在集.size());
        I64 序号 = 0;
        for (auto* 已知存在 : 已确认观察存在集) {
            ++序号;
            结构_当前帧轮廓分区项 项{};
            项.已确认观察存在 = 已知存在;
            项.序号 = 序号;
            I64 已知Z = 0;
            (void)读取基础节点I64(
                reinterpret_cast<基础信息节点类*>(已知存在),
                特征_中心空间坐标Z(),
                已知Z);
            if (!读取已确认观察存在投影ROI(
                    已知存在,
                    static_cast<I64>(报告.帧宽度),
                    static_cast<I64>(报告.帧高度),
                    扩张像素,
                    项.预测最小X,
                    项.预测最大X,
                    项.预测最小Y,
                    项.预测最大Y)) {
                项.扫描状态 = 已知存在当前帧扫描状态_待重识别;
                项.遮挡状态 = 当前帧遮挡判断状态_未判断;
                ++输出.待重识别区数量;
                输出.项集合.push_back(项);
                continue;
            }
            已知扫描面积近似 += ROI面积(项.预测最小X, 项.预测最大X, 项.预测最小Y, 项.预测最大Y);

            const 结构_外设观察像素簇摘要* 最佳簇 = nullptr;
            I64 最佳分数 = 0;
            for (const auto& 簇 : 报告.观察像素簇集合) {
                const I64 分数 = 分区簇与预测ROI匹配分数(
                    簇,
                    项.预测最小X,
                    项.预测最大X,
                    项.预测最小Y,
                    项.预测最大Y,
                    已知Z);
                if (分数 > 最佳分数) {
                    最佳分数 = 分数;
                    最佳簇 = &簇;
                }
            }

            项.匹配分数 = 最佳分数;
            if (最佳簇) {
                项.匹配簇ID = 最佳簇->外设内部簇ID;
                const I64 预测面积 = ROI面积(项.预测最小X, 项.预测最大X, 项.预测最小Y, 项.预测最大Y);
                const I64 当前面积 = 外设簇投影面积(*最佳簇);
                const I64 重叠面积 = ROI重叠面积(
                    项.预测最小X,
                    项.预测最大X,
                    项.预测最小Y,
                    项.预测最大Y,
                    最佳簇->投影最小X,
                    最佳簇->投影最大X,
                    最佳簇->投影最小Y,
                    最佳簇->投影最大Y);
                项.像素残差 = std::max<I64>(0, 预测面积 + 当前面积 - 2 * 重叠面积);
                项.轮廓残差 = 10000 - 最佳分数;
                项.深度残差 = 已知Z > 0 ? 绝对差I64(已知Z, 最佳簇->中心Z) : 0;
            }
            if (最佳分数 >= 匹配阈值) {
                项.扫描状态 = 已知存在当前帧扫描状态_已生成;
                项.遮挡状态 = 当前帧遮挡判断状态_无遮挡证据;
                ++输出.已知扫描区数量;
            } else {
                项.扫描状态 = 已知存在当前帧扫描状态_待重识别;
                项.遮挡状态 = 当前帧遮挡判断状态_可能遮挡;
                ++输出.待重识别区数量;
            }
            输出.项集合.push_back(项);
        }

        for (std::size_t i = 0; i < 输出.项集合.size(); ++i) {
            for (std::size_t j = i + 1; j < 输出.项集合.size(); ++j) {
                auto& 左 = 输出.项集合[i];
                auto& 右 = 输出.项集合[j];
                const bool 同簇冲突 = 左.匹配簇ID != 0 && 左.匹配簇ID == 右.匹配簇ID;
                const I64 重叠 = ROI重叠面积(
                    左.预测最小X,
                    左.预测最大X,
                    左.预测最小Y,
                    左.预测最大Y,
                    右.预测最小X,
                    右.预测最大X,
                    右.预测最小Y,
                    右.预测最大Y);
                const bool ROI冲突 = 重叠 > 0
                    && 重叠 * 10000 / std::max<I64>(
                        1,
                        std::min(
                            ROI面积(左.预测最小X, 左.预测最大X, 左.预测最小Y, 左.预测最大Y),
                            ROI面积(右.预测最小X, 右.预测最大X, 右.预测最小Y, 右.预测最大Y))) > 2500;
                if (同簇冲突 || ROI冲突) {
                    左.冲突状态 = 当前帧轮廓冲突区域明确状态_有冲突;
                    右.冲突状态 = 当前帧轮廓冲突区域明确状态_有冲突;
                }
            }
        }
        for (const auto& 项 : 输出.项集合) {
            if (项.冲突状态 == 当前帧轮廓冲突区域明确状态_有冲突) {
                ++输出.冲突区数量;
            }
        }

        I64 已匹配簇数量 = 0;
        for (const auto& 簇 : 报告.观察像素簇集合) {
            const bool 已匹配 = std::any_of(
                输出.项集合.begin(),
                输出.项集合.end(),
                [&](const auto& 项) noexcept {
                    return 项.扫描状态 == 已知存在当前帧扫描状态_已生成
                        && 项.匹配簇ID == 簇.外设内部簇ID;
                });
            if (已匹配) {
                ++已匹配簇数量;
            }
        }
        const I64 未匹配簇数量 = std::max<I64>(
            0,
            static_cast<I64>(报告.观察像素簇集合.size()) - 已匹配簇数量);
        const I64 帧面积 = static_cast<I64>(报告.帧宽度) * static_cast<I64>(报告.帧高度);
        输出.新识别区数量 = 未匹配簇数量 + (已知扫描面积近似 < 帧面积 ? 1 : 0);
        输出.像素归属闭合状态 = 当前帧像素归属闭合状态_已闭合;
        输出.分区状态 = 输出.已知扫描区数量 > 0
            ? 当前帧轮廓分区状态_已生成
            : 当前帧轮廓分区状态_部分生成;
        return 输出;
    }

    inline bool 外设簇材料可提交(
        const 结构_外设观察报告队列项& 报告,
        const 结构_外设观察像素簇摘要& 簇) noexcept
    {
        auto 句柄成功 = [](const std::string& 句柄) noexcept {
            return !句柄.empty() && 解析D455观察材料句柄(句柄).成功;
        };
        return 句柄成功(报告.原始观察帧句柄)
            && 句柄成功(报告.深度帧句柄)
            && 句柄成功(报告.空间候选队列引用)
            && 句柄成功(报告.像素归属账本引用)
            && (句柄成功(簇.像素集合掩码句柄)
                || 句柄成功(簇.ROI引用)
                || 句柄成功(簇.点集引用))
            && 句柄成功(簇.空间候选引用)
            && 句柄成功(簇.像素归属验证材料引用);
    }

    inline 结构_外设观察存在确认方案 构造外设观察存在确认方案(
        const 结构_外设观察报告队列项& 报告,
        bool 验证已通过,
        const std::vector<存在节点类*>& 已确认观察存在集) noexcept
    {
        结构_外设观察存在确认方案 输出{};
        输出.报告ID = static_cast<I64>(报告.报告ID);
        输出.当前帧ID = 报告.深度帧号 > 0 ? 报告.深度帧号 : 报告.彩色帧号;
        输出.候选簇数量 = static_cast<I64>(报告.观察像素簇集合.size());
        输出.项集合.reserve(报告.观察像素簇集合.size());

        for (const auto& 簇 : 报告.观察像素簇集合) {
            结构_外设观察存在确认方案项 项{};
            项.来源候选簇ID = 簇.外设内部簇ID;
            项.来源空间候选编号 = 簇.来源空间候选ID;
            项.簇 = 簇;
            if (!验证已通过 || !外设簇材料可提交(报告, 簇)) {
                项.方案类型 = 外设观察存在候选确认方案_证据不足;
                项.允许提交状态 = 0;
                项.证据不足原因 = 外设观察验证缺失原因_缺像素归属材料;
                ++输出.证据不足数量;
                输出.项集合.push_back(std::move(项));
                continue;
            }

            I64 最佳分数 = 0;
            存在节点类* 最佳存在 = nullptr;
            I64 次佳分数 = 0;
            for (auto* 已知 : 已确认观察存在集) {
                const I64 分数 = 外设簇与已确认观察存在匹配分数(簇, 已知);
                if (分数 > 最佳分数) {
                    次佳分数 = 最佳分数;
                    最佳分数 = 分数;
                    最佳存在 = 已知;
                } else if (分数 > 次佳分数) {
                    次佳分数 = 分数;
                }
            }

            项.匹配分数 = 最佳分数;
            项.目标已知存在 = 最佳存在;
            if (最佳分数 >= 7500 && 最佳分数 - 次佳分数 < 1200) {
                项.方案类型 = 外设观察存在候选确认方案_冲突;
                项.允许提交状态 = 0;
                项.冲突原因 = 1;
                ++输出.冲突数量;
            } else if (最佳分数 >= 7500 && 最佳存在) {
                项.方案类型 = 外设观察存在候选确认方案_合并;
                项.允许提交状态 = 1;
                ++输出.合并数量;
                ++输出.可提交数量;
            } else {
                项.方案类型 = 外设观察存在候选确认方案_新建;
                项.允许提交状态 = 1;
                ++输出.新建数量;
                ++输出.可提交数量;
            }
            输出.项集合.push_back(std::move(项));
        }

        输出.方案取得状态 = 输出.项集合.empty() ? 0 : 1;
        if (输出.冲突数量 > 0) {
            输出.确认状态 = 外设观察存在候选确认状态_冲突;
        } else if (输出.可提交数量 > 0) {
            输出.确认状态 = 外设观察存在候选确认状态_方案已生成;
        } else if (输出.证据不足数量 > 0) {
            输出.确认状态 = 外设观察存在候选确认状态_证据不足;
        }
        return 输出;
    }

    inline 结构_观察存在假设结果 构建观察存在假设_外设簇(
        const 结构_外设观察存在确认方案项& 项) noexcept
    {
        结构_观察存在假设结果 输出{};
        const auto& 簇 = 项.簇;
        输出.假设状态 = 观察存在假设状态_已生成;
        输出.来源候选编号 = 项.来源空间候选编号 >= 0 ? 项.来源空间候选编号 : 项.来源候选簇ID;
        输出.来源队列位次 = 0;
        输出.相对独立状态 = 空间候选独立状态_相对独立;
        输出.验证状态 = 观察存在假设验证状态_待验证;
        输出.候选像素掩码状态 = 簇.像素集合掩码状态 > 0 ? 1 : 0;
        输出.投影轮廓状态 = 簇.投影最大X >= 簇.投影最小X && 簇.投影最大Y >= 簇.投影最小Y ? 1 : 0;
        输出.距离 = 簇.中心Z;
        输出.尺寸X = 正差I64(簇.空间范围最大X, 簇.空间范围最小X);
        输出.尺寸Y = 正差I64(簇.空间范围最大Y, 簇.空间范围最小Y);
        输出.尺寸Z = 正差I64(簇.空间范围最大Z, 簇.空间范围最小Z);
        输出.方位X = 簇.中心X;
        输出.方位Y = 簇.中心Y;
        输出.有效点比例 = 簇.空间坐标有效率;
        输出.轮廓支持评分 = 簇.轮廓置信度;
        输出.候选.候选编号 = 输出.来源候选编号;
        输出.候选.像素数量 = 簇.像素数量;
        输出.候选.有效点比例 = 簇.空间坐标有效率;
        输出.候选.中心X = 簇.中心X;
        输出.候选.中心Y = 簇.中心Y;
        输出.候选.中心Z = 簇.中心Z;
        输出.候选.范围最小X = 簇.空间范围最小X;
        输出.候选.范围最大X = 簇.空间范围最大X;
        输出.候选.范围最小Y = 簇.空间范围最小Y;
        输出.候选.范围最大Y = 簇.空间范围最大Y;
        输出.候选.范围最小Z = 簇.空间范围最小Z;
        输出.候选.范围最大Z = 簇.空间范围最大Z;
        输出.候选.投影最小X = 簇.投影最小X;
        输出.候选.投影最大X = 簇.投影最大X;
        输出.候选.投影最小Y = 簇.投影最小Y;
        输出.候选.投影最大Y = 簇.投影最大Y;
        输出.候选.平均深度 = 簇.中心Z;
        输出.候选.最近深度 = 簇.中心Z;
        输出.候选.最远深度 = 簇.中心Z;
        输出.候选.空间连续性评分 = 簇.空间连续性评分;
        输出.候选.范围稳定性评分 = 簇.范围稳定性评分;
        输出.候选.空间覆盖像素数 = 簇.像素数量;
        输出.候选.空间匹配像素数 = 簇.像素数量;
        输出.候选.深度一致率 = 簇.深度有效率;
        输出.候选.范围内点比例 = 簇.空间坐标有效率;
        输出.候选.投影覆盖率 = 簇.轮廓置信度 > 0 ? 簇.轮廓置信度 : 10000;
        输出.候选.轮廓吻合率 = 簇.轮廓置信度;
        输出.候选.轮廓置信度 = 簇.轮廓置信度;
        输出.候选.像素归属验证状态 = 像素归属验证状态_已生成待确认;
        return 输出;
    }

    inline 结构_像素归属验证结果 构建像素归属验证_外设簇(
        const 结构_外设观察存在确认方案项& 项) noexcept
    {
        结构_像素归属验证结果 输出{};
        const auto& 簇 = 项.簇;
        输出.结果状态 = 像素归属验证状态_已生成待确认;
        输出.来源候选编号 = 项.来源空间候选编号 >= 0 ? 项.来源空间候选编号 : 项.来源候选簇ID;
        输出.空间覆盖像素数 = 簇.像素数量;
        输出.空间匹配像素数 = 簇.像素数量;
        输出.深度一致率 = 簇.深度有效率;
        输出.范围内点比例 = 簇.空间坐标有效率;
        输出.投影覆盖率 = 簇.轮廓置信度 > 0 ? 簇.轮廓置信度 : 10000;
        输出.轮廓吻合率 = 簇.轮廓置信度;
        输出.验证状态 = 像素归属验证状态_已生成待确认;
        return 输出;
    }

    inline 结构_全帧像素归属账本结果 计算全帧像素归属账本(
        const 结构_双目相机调用结果& 结果,
        const 结构_空间候选观察队列结果& 队列,
        const 结构_观察存在假设结果& 主假设,
        const 结构_像素归属验证结果& 主验证) noexcept
    {
        结构_全帧像素归属账本结果 输出{};
        if (!结果.成功) {
            return 输出;
        }

        输出.预期像素数量 = std::max<I64>(0, 结果.预期像素数量);
        for (const auto& 项 : 生成已验证观察存在项集合(结果, 队列, 主假设, 主验证)) {
            ++输出.已验证观察存在数量;
            累加非负I64(输出.已归属像素数, 项.验证.空间匹配像素数);
            累加非负I64(输出.归属冲突像素数, 项.验证.空间冲突像素数);
        }

        if (输出.预期像素数量 > 0) {
            const I64 原始已归属像素数 = 输出.已归属像素数;
            if (原始已归属像素数 > 输出.预期像素数量) {
                累加非负I64(输出.归属冲突像素数, 原始已归属像素数 - 输出.预期像素数量);
            }
            输出.已归属像素数 = std::min(输出.已归属像素数, 输出.预期像素数量);
            输出.未解释像素数 = std::max<I64>(0, 输出.预期像素数量 - 输出.已归属像素数);
            输出.像素归属率 = 计算比例万分比I64(输出.已归属像素数, 输出.预期像素数量);
        }
        输出.账本状态 = 输出.归属冲突像素数 > 0
            ? 全帧像素归属账状态_有冲突
            : 全帧像素归属账状态_已生成;
        return 输出;
    }

    inline 结构_观察存在确认结果 确认观察存在结果(
        const 结构_双目相机调用结果& 结果,
        const 结构_观察存在假设结果& 假设,
        const 结构_像素归属验证结果& 验证,
        const 结构_空间候选观察队列结果& 队列) noexcept
    {
        结构_观察存在确认结果 输出{};
        输出.来源候选编号 = 假设.来源候选编号;
        输出.空间连续性评分 = 假设.候选.空间连续性评分;
        输出.范围稳定性评分 = 假设.候选.范围稳定性评分;
        输出.范围内点比例 = 验证.范围内点比例;
        输出.深度一致率 = 验证.深度一致率;
        输出.投影覆盖率 = 验证.投影覆盖率;
        输出.轮廓吻合率 = 验证.轮廓吻合率;

        if (假设.假设状态 != 观察存在假设状态_已生成
            || 验证.结果状态 != 像素归属验证状态_已生成待确认
            || 验证.验证状态 != 像素归属验证状态_已生成待确认) {
            输出.确认状态 = 观察存在确认状态_未确认;
            return 输出;
        }

        const I64 相对独立状态 = 假设.相对独立状态;
        const I64 已验证数量 = 统计已验证观察存在数量(结果, 队列);
        if (相对独立状态 == 空间候选独立状态_待拆分) {
            输出.确认状态 = 观察存在确认状态_待拆分;
            输出.已验证观察存在数量 = 已验证数量;
            输出.帧解释状态 = 帧解释状态_部分解释;
            return 输出;
        }
        if (相对独立状态 == 空间候选独立状态_待合并) {
            输出.确认状态 = 观察存在确认状态_待合并;
            输出.已验证观察存在数量 = 已验证数量;
            输出.帧解释状态 = 帧解释状态_部分解释;
            return 输出;
        }

        const bool 范围有效 = 空间候选范围有效(假设.候选);

        if (观察存在候选已验证(假设, 验证)) {
            输出.确认状态 = 观察存在确认状态_已验证;
            输出.已验证观察存在数量 = std::max<I64>(1, 已验证数量);
            输出.像素归属更新 = 1;
            输出.未解释区域更新 = 1;
            输出.帧解释状态 = 帧解释状态_已更新;
            return 输出;
        }

        if (!范围有效
            || 相对独立状态 == 空间候选独立状态_不独立
            || 假设.候选.空间连续性评分 < 4500
            || 假设.候选.范围稳定性评分 < 4500
            || 验证.投影覆盖率 < 1000) {
            输出.确认状态 = 观察存在确认状态_已否定;
            输出.已验证观察存在数量 = 已验证数量;
            输出.帧解释状态 = 帧解释状态_部分解释;
            return 输出;
        }

        输出.确认状态 = 观察存在确认状态_待确认;
        输出.已验证观察存在数量 = 已验证数量;
        输出.帧解释状态 = 帧解释状态_部分解释;
        return 输出;
    }

    inline const 语素入口节点类* 双目相机失败原因词(
        双目相机本能适配器::失败原因 原因) noexcept
    {
        using enum 双目相机本能适配器::失败原因;
        switch (原因) {
        case 无:
            return nullptr;
        case 运行时缺失:
            return 值_外设运行时缺失();
        case 打开失败:
            return 值_外设打开失败();
        case 不可用:
            return 值_外设不可用();
        case 采集失败:
            return 值_外设采集失败();
        default:
            return 值_执行失败();
        }
    }

    inline 结构_双目相机调用结果 转换双目相机调用结果(
        const 双目相机本能适配器::调用结果& 来源) noexcept
    {
        结构_双目相机调用结果 结果{};
        结果.成功 = 来源.成功;
        结果.相机已打开 = 来源.相机已打开;
        结果.失败原因 = 双目相机失败原因词(来源.原因);
        结果.消息 = 来源.消息;
        结果.宽度 = 来源.宽度;
        结果.高度 = 来源.高度;
        结果.当前观察帧采集时间 = static_cast<I64>(来源.系统到达时间_us);
        结果.观察帧失效状态 = 观察失效状态_未失效;
        结果.观察帧来源外设状态 =
            来源.成功
            && 来源.系统到达时间_us > 0
            && (来源.深度帧号 > 0 || 来源.彩色帧号 > 0)
                ? 来源外设状态_可追溯
                : 来源外设状态_不可追溯;
        结果.深度帧号 = 来源.深度帧号;
        结果.彩色帧号 = 来源.彩色帧号;
        结果.预期像素数量 = static_cast<I64>(来源.预期像素数量);
        结果.像素特征数量 = static_cast<I64>(来源.像素特征数量);
        结果.颜色像素数量 = static_cast<I64>(来源.颜色像素数量);
        结果.深度有效像素数量 = static_cast<I64>(来源.深度有效像素数量);
        结果.点云有效像素数量 = static_cast<I64>(来源.点云有效像素数量);
        结果.像素覆盖状态 = static_cast<I64>(来源.像素覆盖状态);
        结果.空间坐标单位毫米 = static_cast<I64>(来源.空间坐标单位毫米);
        结果.颜色RGB结构状态 = static_cast<I64>(来源.颜色RGB结构状态);
        结果.原始深度毫米结构状态 = static_cast<I64>(来源.原始深度毫米结构状态);
        结果.滤波深度毫米结构状态 = static_cast<I64>(来源.滤波深度毫米结构状态);
        结果.补全深度毫米结构状态 = static_cast<I64>(来源.补全深度毫米结构状态);
        结果.深度有效性Mask状态 = static_cast<I64>(来源.深度有效性Mask状态);
        结果.深度来源结构状态 = static_cast<I64>(来源.深度来源结构状态);
        结果.深度稳定性结构状态 = static_cast<I64>(来源.深度稳定性结构状态);
        结果.深度邻域一致性结构状态 = static_cast<I64>(来源.深度邻域一致性结构状态);
        结果.融合深度毫米结构状态 = static_cast<I64>(来源.融合深度毫米结构状态);
        结果.融合深度有效性结构状态 = static_cast<I64>(来源.融合深度有效性结构状态);
        结果.多帧深度稳定性结构状态 = static_cast<I64>(来源.多帧深度稳定性结构状态);
        结果.每像素深度方差结构状态 = static_cast<I64>(来源.每像素深度方差结构状态);
        结果.空间坐标毫米XYZ结构状态 = static_cast<I64>(来源.空间坐标毫米XYZ结构状态);
        结果.原始深度来源像素数量 = static_cast<I64>(来源.原始深度来源像素数量);
        结果.滤波深度来源像素数量 = static_cast<I64>(来源.滤波深度来源像素数量);
        结果.补全深度来源像素数量 = static_cast<I64>(来源.补全深度来源像素数量);
        结果.无有效深度来源像素数量 = static_cast<I64>(来源.无有效深度来源像素数量);
        结果.补全深度低置信像素数量 = static_cast<I64>(来源.补全深度低置信像素数量);
        结果.深度稳定性平均值 = static_cast<I64>(来源.深度稳定性平均值);
        结果.深度邻域一致性平均值 = static_cast<I64>(来源.深度邻域一致性平均值);
        结果.观察帧组状态 = static_cast<I64>(来源.观察帧组状态);
        结果.观察帧组帧数 = static_cast<I64>(来源.观察帧组帧数);
        结果.融合依据帧集合状态 = static_cast<I64>(来源.融合依据帧集合状态);
        结果.融合深度有效像素数量 = static_cast<I64>(来源.融合深度有效像素数量);
        结果.融合深度有效率 = static_cast<I64>(来源.融合深度有效率);
        结果.融合补偿深度空洞像素数量 = static_cast<I64>(来源.融合补偿深度空洞像素数量);
        结果.融合低稳定像素数量 = static_cast<I64>(来源.融合低稳定像素数量);
        结果.融合深度稳定性平均值 = static_cast<I64>(来源.融合深度稳定性平均值);
        结果.融合深度平均方差 = static_cast<I64>(来源.融合深度平均方差);
        结果.彩色深度已对齐 = static_cast<I64>(来源.彩色深度已对齐);
        结果.对齐目标 = static_cast<I64>(来源.对齐目标);
        结果.深度有效率 = static_cast<I64>(来源.深度有效率);
        结果.空间坐标有效率 = static_cast<I64>(来源.空间坐标有效率);
        结果.帧质量评分 = static_cast<I64>(来源.帧质量评分);
        结果.观察帧质量诊断摘要 = 来源.成功 ? 1 : 0;
        结果.深度空洞数量 = static_cast<I64>(来源.深度空洞数量);
        结果.深度空洞区域数量 = static_cast<I64>(来源.深度空洞区域数量);
        结果.深度空洞区域集合状态 = static_cast<I64>(来源.深度空洞区域集合状态);
        结果.彩色深度对齐状态 = static_cast<I64>(来源.彩色深度对齐状态);
        结果.轮廓闭合率 = static_cast<I64>(来源.轮廓闭合率);
        结果.轮廓断裂数量 = static_cast<I64>(来源.轮廓断裂数量);
        结果.彩图轮廓数量 = static_cast<I64>(来源.彩图轮廓数量);
        结果.深度轮廓数量 = static_cast<I64>(来源.深度轮廓数量);
        结果.空间投影轮廓数量 = static_cast<I64>(来源.空间投影轮廓数量);
        结果.融合轮廓数量 = static_cast<I64>(来源.融合轮廓数量);
        结果.轮廓来源分层状态 = static_cast<I64>(来源.轮廓来源分层状态);
        结果.轮廓深度支持率 = static_cast<I64>(来源.轮廓深度支持率);
        结果.轮廓颜色支持率 = static_cast<I64>(来源.轮廓颜色支持率);
        结果.轮廓空间支持率 = static_cast<I64>(来源.轮廓空间支持率);
        结果.融合轮廓置信度 = static_cast<I64>(来源.融合轮廓置信度);
        结果.跨越深度断裂轮廓数量 = static_cast<I64>(来源.跨越深度断裂轮廓数量);
        结果.穿过深度无效区域轮廓数量 = static_cast<I64>(来源.穿过深度无效区域轮廓数量);
        结果.依赖补全深度轮廓数量 = static_cast<I64>(来源.依赖补全深度轮廓数量);
        结果.空间点有效率 = static_cast<I64>(来源.空间点有效率);
        结果.边界深度稳定率 = static_cast<I64>(来源.边界深度稳定率);
        结果.候选存在验证通过率 = static_cast<I64>(来源.候选存在验证通过率);
        结果.未解释区域比例 = static_cast<I64>(来源.未解释区域比例);
        结果.补观察缺口状态 = static_cast<I64>(来源.补观察缺口状态);
        结果.补观察缺失原因 = static_cast<I64>(来源.补观察缺失原因);
        结果.待补观察区域数量 = static_cast<I64>(来源.待补观察区域数量);
        结果.补全候选数量 = static_cast<I64>(来源.补全候选数量);
        结果.低置信轮廓数量 = static_cast<I64>(来源.低置信轮廓数量);
        结果.条件不足候选数量 = static_cast<I64>(来源.条件不足候选数量);
        结果.待验证候选数量 = static_cast<I64>(来源.待验证候选数量);
        结果.部分确认候选数量 = static_cast<I64>(来源.部分确认候选数量);
        结果.补观察需求建议 = static_cast<I64>(来源.补观察需求建议);
        结果.诊断区域数量 = static_cast<I64>(来源.诊断区域数量);
        结果.诊断区域集合状态 = static_cast<I64>(来源.诊断区域集合状态);
        结果.诊断区域掩码状态 = static_cast<I64>(来源.诊断区域掩码状态);
        结果.空间候选数量 = static_cast<I64>(来源.空间候选数量);
        结果.空间候选有效点数量 = static_cast<I64>(来源.空间候选有效点数量);
        结果.空间候选列表 = 来源.空间候选列表;
        结果.诊断区域列表 = 来源.诊断区域列表;
        结果.轮廓数量 = 来源.轮廓数量;
        补全基础观察事实状态(结果);
        return 结果;
    }

    inline void 记录双目相机适配器失败(const 结构_双目相机调用结果& 结果) noexcept
    {
        if (!结果.成功 && !结果.消息.empty()) {
            项目运行错误日志("双目相机本能方法/" + 结果.消息);
        }
    }

    inline std::string 语素入口日志文本(const 语素入口节点类* 节点) noexcept
    {
        return 节点 ? 节点->获取主键() : std::string("空");
    }

    inline std::string 基础节点日志文本(const 基础信息节点类* 节点) noexcept
    {
        return 节点 ? 节点->获取主键() : std::string("空");
    }

    inline void 记录双目相机本能动作结果(
        const char* 方法名,
        const 结构_双目相机调用结果& 结果,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        存在节点类* 相机存在,
        基础信息节点类* 目标宿主,
        动态节点类* 动态,
        存在节点类* 输出运行,
        const 语素入口节点类* 输出特征 = nullptr,
        const char* 输出特征名 = nullptr) noexcept
    {
        if (!结果.成功) {
            const bool 外设条件不足 =
                结果.失败原因 == 值_外设运行时缺失()
                || 结果.失败原因 == 值_外设打开失败()
                || 结果.失败原因 == 值_外设不可用();
            std::ostringstream 简要;
            简要 << "双目相机本能方法结果"
                << " | 方法=" << (方法名 ? 方法名 : "")
                << " | 成功=0"
                << " | 相机已打开=" << (结果.相机已打开 ? 1 : 0)
                << " | 失败原因=" << 语素入口日志文本(结果.失败原因)
                << " | 消息=" << 结果.消息
                << " | 目标外设=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(相机存在))
                << " | 目标宿主=" << 基础节点日志文本(目标宿主)
                << " | 输出特征=" << (输出特征名 ? 输出特征名 : 语素入口日志文本(输出特征))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态))
                << " | 外设条件不足=" << (外设条件不足 ? 1 : 0)
                << " | 等待外设条件=" << (外设条件不足 ? 1 : 0)
                << " | 详细观察字段=省略";
            项目运行错误日志(简要.str());
            return;
        }

        std::ostringstream 输出;
        输出 << "双目相机本能方法结果"
            << " | 方法=" << (方法名 ? 方法名 : "")
            << " | 成功=" << (结果.成功 ? 1 : 0)
            << " | 相机已打开=" << (结果.相机已打开 ? 1 : 0)
            << " | 失败原因=" << 语素入口日志文本(结果.失败原因)
            << " | 消息=" << 结果.消息
            << " | 输入场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(输入参数场景))
            << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(输出结果场景))
            << " | 目标外设=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(相机存在))
            << " | 目标宿主=" << 基础节点日志文本(目标宿主)
            << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态))
            << " | 输出特征=" << (输出特征名 ? 输出特征名 : 语素入口日志文本(输出特征))
            << " | 输出运行=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(输出运行))
            << " | 宽度=" << 结果.宽度
            << " | 高度=" << 结果.高度
            << " | 深度帧号=" << 结果.深度帧号
            << " | 彩色帧号=" << 结果.彩色帧号
            << " | 预期像素数量=" << 结果.预期像素数量
            << " | 像素特征数量=" << 结果.像素特征数量
            << " | 像素观察存在数量=" << 结果.像素观察存在数量
            << " | 缺失像素数量=" << 结果.缺失像素数量
            << " | 重复映射数量=" << 结果.重复映射数量
            << " | 颜色像素数量=" << 结果.颜色像素数量
            << " | 深度有效像素数量=" << 结果.深度有效像素数量
            << " | 点云有效像素数量=" << 结果.点云有效像素数量
            << " | 像素覆盖状态=" << 结果.像素覆盖状态
            << " | 当前观察特征帧取得状态=" << 结果.当前观察特征帧取得状态
            << " | 观察帧容器状态=" << 结果.观察帧容器状态
            << " | 像素特征值承接状态=" << 结果.像素特征值承接状态
            << " | 坐标回查可用状态=" << 结果.坐标回查可用状态
            << " | 帧内编号回查可用状态=" << 结果.帧内编号回查可用状态
            << " | 观察事实完备状态=" << 结果.观察事实完备状态
            << " | 观察事实完备失败条件数量=" << 结果.观察事实完备失败条件数量
            << " | 观察事实完备失败原因摘要=" << 结果.观察事实完备失败原因摘要
            << " | 质量可用状态=" << 结果.质量可用状态
            << " | 质量失败条件数量=" << 结果.质量失败条件数量
            << " | 质量失败原因摘要=" << 结果.质量失败原因摘要
            << " | 质量最低帧质量评分=" << 观察质量最低评分
            << " | 质量最低深度有效率=" << 观察质量最低深度有效率
            << " | 质量最低空间坐标有效率=" << 观察质量最低空间坐标有效率
            << " | 当前观察帧采集时间=" << 结果.当前观察帧采集时间
            << " | 当前观察帧观察年龄=" << 结果.当前观察帧观察年龄
            << " | 最大允许观察年龄=" << 结果.最大允许观察年龄
            << " | 新鲜度判定时间=" << 结果.新鲜度判定时间
            << " | 观察帧失效状态=" << 结果.观察帧失效状态
            << " | 观察帧来源外设状态=" << 结果.观察帧来源外设状态
            << " | 当前观察新鲜度状态=" << 结果.当前观察新鲜度状态
            << " | 新鲜度失败原因=" << 结果.新鲜度失败原因
            << " | 新鲜度失败原因摘要=" << 结果.新鲜度失败原因摘要
            << " | 基础观察事实可用状态=" << 结果.基础观察事实可用状态
            << " | 基础观察事实失败条件数量=" << 结果.基础观察事实失败条件数量
            << " | 基础观察事实失败原因摘要=" << 基础观察事实失败条件摘要(结果)
            << " | 空间坐标单位毫米=" << 结果.空间坐标单位毫米
            << " | 颜色RGB结构状态=" << 结果.颜色RGB结构状态
            << " | 原始深度毫米结构状态=" << 结果.原始深度毫米结构状态
            << " | 滤波深度毫米结构状态=" << 结果.滤波深度毫米结构状态
            << " | 补全深度毫米结构状态=" << 结果.补全深度毫米结构状态
            << " | 深度有效性Mask状态=" << 结果.深度有效性Mask状态
            << " | 深度来源结构状态=" << 结果.深度来源结构状态
            << " | 深度稳定性结构状态=" << 结果.深度稳定性结构状态
            << " | 深度邻域一致性结构状态=" << 结果.深度邻域一致性结构状态
            << " | 融合深度毫米结构状态=" << 结果.融合深度毫米结构状态
            << " | 融合深度有效性结构状态=" << 结果.融合深度有效性结构状态
            << " | 多帧深度稳定性结构状态=" << 结果.多帧深度稳定性结构状态
            << " | 每像素深度方差结构状态=" << 结果.每像素深度方差结构状态
            << " | 空间坐标毫米XYZ结构状态=" << 结果.空间坐标毫米XYZ结构状态
            << " | 原始深度来源像素数量=" << 结果.原始深度来源像素数量
            << " | 滤波深度来源像素数量=" << 结果.滤波深度来源像素数量
            << " | 补全深度来源像素数量=" << 结果.补全深度来源像素数量
            << " | 无有效深度来源像素数量=" << 结果.无有效深度来源像素数量
            << " | 补全深度低置信像素数量=" << 结果.补全深度低置信像素数量
            << " | 深度稳定性平均值=" << 结果.深度稳定性平均值
            << " | 深度邻域一致性平均值=" << 结果.深度邻域一致性平均值
            << " | 观察帧组状态=" << 结果.观察帧组状态
            << " | 观察帧组帧数=" << 结果.观察帧组帧数
            << " | 融合依据帧集合状态=" << 结果.融合依据帧集合状态
            << " | 融合深度有效像素数量=" << 结果.融合深度有效像素数量
            << " | 融合深度有效率=" << 结果.融合深度有效率
            << " | 融合补偿深度空洞像素数量=" << 结果.融合补偿深度空洞像素数量
            << " | 融合低稳定像素数量=" << 结果.融合低稳定像素数量
            << " | 融合深度稳定性平均值=" << 结果.融合深度稳定性平均值
            << " | 融合深度平均方差=" << 结果.融合深度平均方差
            << " | 彩色深度已对齐=" << 结果.彩色深度已对齐
            << " | 深度有效率=" << 结果.深度有效率
            << " | 空间坐标有效率=" << 结果.空间坐标有效率
            << " | 帧质量评分=" << 结果.帧质量评分
            << " | 观察帧质量诊断摘要=" << 结果.观察帧质量诊断摘要
            << " | 深度空洞数量=" << 结果.深度空洞数量
            << " | 深度空洞区域数量=" << 结果.深度空洞区域数量
            << " | 轮廓闭合率=" << 结果.轮廓闭合率
            << " | 轮廓断裂数量=" << 结果.轮廓断裂数量
            << " | 彩图轮廓数量=" << 结果.彩图轮廓数量
            << " | 深度轮廓数量=" << 结果.深度轮廓数量
            << " | 空间投影轮廓数量=" << 结果.空间投影轮廓数量
            << " | 融合轮廓数量=" << 结果.融合轮廓数量
            << " | 轮廓来源分层状态=" << 结果.轮廓来源分层状态
            << " | 轮廓深度支持率=" << 结果.轮廓深度支持率
            << " | 轮廓颜色支持率=" << 结果.轮廓颜色支持率
            << " | 轮廓空间支持率=" << 结果.轮廓空间支持率
            << " | 融合轮廓置信度=" << 结果.融合轮廓置信度
            << " | 跨越深度断裂轮廓数量=" << 结果.跨越深度断裂轮廓数量
            << " | 穿过深度无效区域轮廓数量=" << 结果.穿过深度无效区域轮廓数量
            << " | 依赖补全深度轮廓数量=" << 结果.依赖补全深度轮廓数量
            << " | 候选存在验证通过率=" << 结果.候选存在验证通过率
            << " | 未解释区域比例=" << 结果.未解释区域比例
            << " | 补观察缺口状态=" << 结果.补观察缺口状态
            << " | 补观察缺失原因=" << 结果.补观察缺失原因
            << " | 待补观察区域数量=" << 结果.待补观察区域数量
            << " | 补全候选数量=" << 结果.补全候选数量
            << " | 低置信轮廓数量=" << 结果.低置信轮廓数量
            << " | 条件不足候选数量=" << 结果.条件不足候选数量
            << " | 待验证候选数量=" << 结果.待验证候选数量
            << " | 部分确认候选数量=" << 结果.部分确认候选数量
            << " | 补观察需求建议=" << 结果.补观察需求建议
            << " | 诊断区域数量=" << 结果.诊断区域数量
            << " | 诊断区域集合状态=" << 结果.诊断区域集合状态
            << " | 诊断区域掩码状态=" << 结果.诊断区域掩码状态
            << " | 空间候选数量=" << 结果.空间候选数量
            << " | 空间候选有效点数量=" << 结果.空间候选有效点数量
            << " | 轮廓数量=" << 结果.轮廓数量;
        if (!结果.成功) {
            项目运行错误日志(输出.str());
        } else if (结果.失败原因) {
            项目运行警告日志(输出.str());
        } else {
            项目运行日志(输出.str());
        }
    }

    inline 结构_双目相机调用结果 打开双目相机适配器() noexcept
    {
        auto 结果 = 转换双目相机调用结果(双目相机本能适配器::打开());
        记录双目相机适配器失败(结果);
        return 结果;
    }

    inline 结构_双目相机调用结果 释放双目相机适配器() noexcept
    {
        auto 结果 = 转换双目相机调用结果(双目相机本能适配器::释放());
        if (!结果.消息.empty() && 结果.失败原因) {
            项目运行警告日志("双目相机本能方法/" + 结果.消息);
        }
        return 结果;
    }

    inline 结构_双目相机调用结果 检查双目相机适配器状态() noexcept
    {
        auto 结果 = 转换双目相机调用结果(双目相机本能适配器::检查());
        if (结果.失败原因 && !结果.消息.empty()) {
            项目运行错误日志("双目相机本能方法/" + 结果.消息);
        }
        return 结果;
    }

    inline 结构_双目相机调用结果 采集双目相机帧() noexcept
    {
        auto 结果 = 转换双目相机调用结果(双目相机本能适配器::采集一帧());
        记录双目相机适配器失败(结果);
        return 结果;
    }

    inline 存在节点类* 取或创建空间候选观察事实(
        基础信息节点类* 宿主,
        I64 候选编号) noexcept
    {
        return 宿主
            ? 世界树.取或创建子存在_按类型并按特征I64(
                宿主,
                类型_空间候选观察事实(),
                特征_空间候选编号(),
                候选编号)
            : nullptr;
    }

    inline void 写入空间候选事实(
        基础信息节点类* 宿主,
        const 双目相机本能适配器::空间候选摘要& 候选,
        时间戳 now) noexcept
    {
        auto* 候选存在 = 取或创建空间候选观察事实(宿主, 候选.候选编号);
        auto* 节点 = reinterpret_cast<基础信息节点类*>(候选存在);
        if (!节点) return;
        (void)写入基础节点I64(节点, 特征_空间候选编号(), 候选.候选编号, now);
        (void)写入基础节点I64(节点, 特征_空间候选像素数量(), 候选.像素数量, now);
        (void)写入基础节点I64(节点, 特征_空间候选有效点比例(), 候选.有效点比例, now);
        (void)写入基础节点I64(节点, 特征_中心空间坐标X(), 候选.中心X, now);
        (void)写入基础节点I64(节点, 特征_中心空间坐标Y(), 候选.中心Y, now);
        (void)写入基础节点I64(节点, 特征_中心空间坐标Z(), 候选.中心Z, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小X(), 候选.范围最小X, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大X(), 候选.范围最大X, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小Y(), 候选.范围最小Y, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大Y(), 候选.范围最大Y, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小Z(), 候选.范围最小Z, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大Z(), 候选.范围最大Z, now);
        (void)写入基础节点I64(节点, 特征_投影范围最小X(), 候选.投影最小X, now);
        (void)写入基础节点I64(节点, 特征_投影范围最大X(), 候选.投影最大X, now);
        (void)写入基础节点I64(节点, 特征_投影范围最小Y(), 候选.投影最小Y, now);
        (void)写入基础节点I64(节点, 特征_投影范围最大Y(), 候选.投影最大Y, now);
        (void)写入基础节点I64(节点, 特征_平均深度(), 候选.平均深度, now);
        (void)写入基础节点I64(节点, 特征_最近深度(), 候选.最近深度, now);
        (void)写入基础节点I64(节点, 特征_最远深度(), 候选.最远深度, now);
        (void)写入基础节点I64(节点, 特征_空间连续性评分(), 候选.空间连续性评分, now);
        (void)写入基础节点I64(节点, 特征_范围稳定性评分(), 候选.范围稳定性评分, now);
        (void)写入基础节点I64(节点, 特征_空间覆盖像素数(), 候选.空间覆盖像素数, now);
        (void)写入基础节点I64(节点, 特征_空间匹配像素数(), 候选.空间匹配像素数, now);
        (void)写入基础节点I64(节点, 特征_空间冲突像素数(), 候选.空间冲突像素数, now);
        (void)写入基础节点I64(节点, 特征_轮廓支持像素数(), 候选.轮廓支持像素数, now);
        (void)写入基础节点I64(节点, 特征_轮廓冲突像素数(), 候选.轮廓冲突像素数, now);
        (void)写入基础节点I64(节点, 特征_深度一致率(), 候选.深度一致率, now);
        (void)写入基础节点I64(节点, 特征_范围内点比例(), 候选.范围内点比例, now);
        (void)写入基础节点I64(节点, 特征_投影覆盖率(), 候选.投影覆盖率, now);
        (void)写入基础节点I64(节点, 特征_轮廓吻合率(), 候选.轮廓吻合率, now);
        (void)写入基础节点I64(节点, 特征_轮廓来源类型(), 候选.轮廓来源类型, now);
        (void)写入基础节点I64(节点, 特征_轮廓闭合度(), 候选.轮廓闭合度, now);
        (void)写入基础节点I64(节点, 特征_轮廓深度支持率(), 候选.轮廓深度支持率, now);
        (void)写入基础节点I64(节点, 特征_轮廓颜色支持率(), 候选.轮廓颜色支持率, now);
        (void)写入基础节点I64(节点, 特征_轮廓空间支持率(), 候选.轮廓空间支持率, now);
        (void)写入基础节点I64(节点, 特征_轮廓跨越深度断裂(), 候选.轮廓跨越深度断裂, now);
        (void)写入基础节点I64(节点, 特征_轮廓穿过深度无效区域(), 候选.轮廓穿过深度无效区域, now);
        (void)写入基础节点I64(节点, 特征_轮廓依赖补全深度(), 候选.轮廓依赖补全深度, now);
        (void)写入基础节点I64(节点, 特征_轮廓置信度(), 候选.轮廓置信度, now);
        (void)写入基础节点I64(节点, 特征_遮挡像素数(), 候选.遮挡像素数, now);
        (void)写入基础节点I64(节点, 特征_未解释像素数(), 候选.未解释像素数, now);
        (void)写入基础节点I64(节点, 特征_像素归属验证状态(), 候选.像素归属验证状态, now);
        (void)写入基础节点I64(节点, 特征_观察确认补偿状态(), 候选.观察确认补偿状态, now);
        (void)写入基础节点I64(节点, 特征_缺失原因(), 候选.缺失原因, now);
        (void)写入基础节点I64(节点, 特征_未解释区域状态(), 候选.未解释区域状态, now);
        (void)写入基础节点I64(节点, 特征_待补观察区域数量(), 候选.待补观察区域数量, now);
        (void)写入基础节点I64(节点, 特征_补全候选状态(), 候选.补全候选状态, now);
        (void)写入基础节点I64(节点, 特征_低置信轮廓状态(), 候选.低置信轮廓状态, now);
        (void)写入基础节点I64(节点, 特征_补观察需求建议(), 候选.补观察需求建议, now);
    }

    inline void 写入空间候选逐项账本(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        for (const auto& 候选 : 结果.空间候选列表) {
            写入空间候选事实(宿主, 候选, now);
        }
    }

    inline bool 读取空间候选事实(
        存在节点类* 候选存在,
        双目相机本能适配器::空间候选摘要& 候选) noexcept
    {
        候选 = {};
        auto* 节点 = reinterpret_cast<基础信息节点类*>(候选存在);
        if (!节点) return false;
        if (!读取基础节点I64(节点, 特征_空间候选编号(), 候选.候选编号)) {
            return false;
        }
        (void)读取基础节点I64(节点, 特征_空间候选像素数量(), 候选.像素数量);
        (void)读取基础节点I64(节点, 特征_空间候选有效点比例(), 候选.有效点比例);
        (void)读取基础节点I64(节点, 特征_中心空间坐标X(), 候选.中心X);
        (void)读取基础节点I64(节点, 特征_中心空间坐标Y(), 候选.中心Y);
        (void)读取基础节点I64(节点, 特征_中心空间坐标Z(), 候选.中心Z);
        (void)读取基础节点I64(节点, 特征_范围坐标AABB最小X(), 候选.范围最小X);
        (void)读取基础节点I64(节点, 特征_范围坐标AABB最大X(), 候选.范围最大X);
        (void)读取基础节点I64(节点, 特征_范围坐标AABB最小Y(), 候选.范围最小Y);
        (void)读取基础节点I64(节点, 特征_范围坐标AABB最大Y(), 候选.范围最大Y);
        (void)读取基础节点I64(节点, 特征_范围坐标AABB最小Z(), 候选.范围最小Z);
        (void)读取基础节点I64(节点, 特征_范围坐标AABB最大Z(), 候选.范围最大Z);
        (void)读取基础节点I64(节点, 特征_投影范围最小X(), 候选.投影最小X);
        (void)读取基础节点I64(节点, 特征_投影范围最大X(), 候选.投影最大X);
        (void)读取基础节点I64(节点, 特征_投影范围最小Y(), 候选.投影最小Y);
        (void)读取基础节点I64(节点, 特征_投影范围最大Y(), 候选.投影最大Y);
        (void)读取基础节点I64(节点, 特征_平均深度(), 候选.平均深度);
        (void)读取基础节点I64(节点, 特征_最近深度(), 候选.最近深度);
        (void)读取基础节点I64(节点, 特征_最远深度(), 候选.最远深度);
        (void)读取基础节点I64(节点, 特征_空间连续性评分(), 候选.空间连续性评分);
        (void)读取基础节点I64(节点, 特征_范围稳定性评分(), 候选.范围稳定性评分);
        (void)读取基础节点I64(节点, 特征_空间覆盖像素数(), 候选.空间覆盖像素数);
        (void)读取基础节点I64(节点, 特征_空间匹配像素数(), 候选.空间匹配像素数);
        (void)读取基础节点I64(节点, 特征_空间冲突像素数(), 候选.空间冲突像素数);
        (void)读取基础节点I64(节点, 特征_轮廓支持像素数(), 候选.轮廓支持像素数);
        (void)读取基础节点I64(节点, 特征_轮廓冲突像素数(), 候选.轮廓冲突像素数);
        (void)读取基础节点I64(节点, 特征_深度一致率(), 候选.深度一致率);
        (void)读取基础节点I64(节点, 特征_范围内点比例(), 候选.范围内点比例);
        (void)读取基础节点I64(节点, 特征_投影覆盖率(), 候选.投影覆盖率);
        (void)读取基础节点I64(节点, 特征_轮廓吻合率(), 候选.轮廓吻合率);
        (void)读取基础节点I64(节点, 特征_轮廓来源类型(), 候选.轮廓来源类型);
        (void)读取基础节点I64(节点, 特征_轮廓闭合度(), 候选.轮廓闭合度);
        (void)读取基础节点I64(节点, 特征_轮廓深度支持率(), 候选.轮廓深度支持率);
        (void)读取基础节点I64(节点, 特征_轮廓颜色支持率(), 候选.轮廓颜色支持率);
        (void)读取基础节点I64(节点, 特征_轮廓空间支持率(), 候选.轮廓空间支持率);
        (void)读取基础节点I64(节点, 特征_轮廓跨越深度断裂(), 候选.轮廓跨越深度断裂);
        (void)读取基础节点I64(节点, 特征_轮廓穿过深度无效区域(), 候选.轮廓穿过深度无效区域);
        (void)读取基础节点I64(节点, 特征_轮廓依赖补全深度(), 候选.轮廓依赖补全深度);
        (void)读取基础节点I64(节点, 特征_轮廓置信度(), 候选.轮廓置信度);
        (void)读取基础节点I64(节点, 特征_遮挡像素数(), 候选.遮挡像素数);
        (void)读取基础节点I64(节点, 特征_未解释像素数(), 候选.未解释像素数);
        (void)读取基础节点I64(节点, 特征_像素归属验证状态(), 候选.像素归属验证状态);
        (void)读取基础节点I64(节点, 特征_观察确认补偿状态(), 候选.观察确认补偿状态);
        (void)读取基础节点I64(节点, 特征_缺失原因(), 候选.缺失原因);
        (void)读取基础节点I64(节点, 特征_未解释区域状态(), 候选.未解释区域状态);
        (void)读取基础节点I64(节点, 特征_待补观察区域数量(), 候选.待补观察区域数量);
        (void)读取基础节点I64(节点, 特征_补全候选状态(), 候选.补全候选状态);
        (void)读取基础节点I64(节点, 特征_低置信轮廓状态(), 候选.低置信轮廓状态);
        (void)读取基础节点I64(节点, 特征_补观察需求建议(), 候选.补观察需求建议);
        return true;
    }

    inline bool 读取空间候选逐项账本(
        基础信息节点类* 宿主,
        std::vector<双目相机本能适配器::空间候选摘要>& 候选列表) noexcept
    {
        候选列表.clear();
        if (!宿主) return false;
        for (auto* 子存在 : 世界树.存在().获取子存在(宿主)) {
            const auto* 主信息 = 世界树.存在().取存在主信息(子存在);
            if (!主信息 || 主信息->类型 != 类型_空间候选观察事实()) {
                continue;
            }
            双目相机本能适配器::空间候选摘要 候选{};
            if (读取空间候选事实(子存在, 候选)) {
                候选列表.push_back(候选);
            }
        }
        std::sort(
            候选列表.begin(),
            候选列表.end(),
            [](const auto& a, const auto& b) noexcept {
                return a.候选编号 < b.候选编号;
            });
        return !候选列表.empty();
    }

    inline 存在节点类* 取或创建观察诊断区域事实(
        基础信息节点类* 宿主,
        I64 图层类型,
        I64 区域编号) noexcept
    {
        auto* 区域存在 = 宿主
            ? 世界树.取或创建子存在_按类型并按特征I64(
                宿主,
                类型_观察诊断区域(),
                特征_诊断区域编号(),
                区域编号)
            : nullptr;
        auto* 节点 = reinterpret_cast<基础信息节点类*>(区域存在);
        if (节点) {
            (void)写入基础节点I64(
                节点,
                特征_诊断区域图层类型(),
                图层类型,
                结构体_时间戳::当前_微秒());
        }
        return 区域存在;
    }

    inline void 写入观察诊断区域事实(
        基础信息节点类* 宿主,
        const 双目相机本能适配器::诊断区域摘要& 区域,
        时间戳 now) noexcept
    {
        auto* 区域存在 = 取或创建观察诊断区域事实(宿主, 区域.图层类型, 区域.区域编号);
        auto* 节点 = reinterpret_cast<基础信息节点类*>(区域存在);
        if (!节点) return;
        (void)写入基础节点I64(节点, 特征_诊断区域编号(), 区域.区域编号, now);
        (void)写入基础节点I64(节点, 特征_诊断区域图层类型(), 区域.图层类型, now);
        (void)写入基础节点I64(节点, 特征_诊断区域来源候选编号(), 区域.来源候选编号, now);
        (void)写入基础节点I64(节点, 特征_诊断区域投影最小X(), 区域.投影最小X, now);
        (void)写入基础节点I64(节点, 特征_诊断区域投影最大X(), 区域.投影最大X, now);
        (void)写入基础节点I64(节点, 特征_诊断区域投影最小Y(), 区域.投影最小Y, now);
        (void)写入基础节点I64(节点, 特征_诊断区域投影最大Y(), 区域.投影最大Y, now);
        (void)写入基础节点I64(节点, 特征_诊断区域像素数量(), 区域.像素数量, now);
        (void)写入基础节点I64(节点, 特征_诊断区域置信度(), 区域.置信度, now);
        (void)写入基础节点I64(节点, 特征_诊断区域掩码状态(), 区域.掩码状态, now);
    }

    inline void 写入观察诊断区域逐项账本(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        for (const auto& 区域 : 结果.诊断区域列表) {
            写入观察诊断区域事实(宿主, 区域, now);
        }
    }

    inline bool 读取观察诊断区域事实(
        存在节点类* 区域存在,
        双目相机本能适配器::诊断区域摘要& 区域) noexcept
    {
        区域 = {};
        auto* 节点 = reinterpret_cast<基础信息节点类*>(区域存在);
        if (!节点) return false;
        if (!读取基础节点I64(节点, 特征_诊断区域编号(), 区域.区域编号)) {
            return false;
        }
        (void)读取基础节点I64(节点, 特征_诊断区域图层类型(), 区域.图层类型);
        (void)读取基础节点I64(节点, 特征_诊断区域来源候选编号(), 区域.来源候选编号);
        (void)读取基础节点I64(节点, 特征_诊断区域投影最小X(), 区域.投影最小X);
        (void)读取基础节点I64(节点, 特征_诊断区域投影最大X(), 区域.投影最大X);
        (void)读取基础节点I64(节点, 特征_诊断区域投影最小Y(), 区域.投影最小Y);
        (void)读取基础节点I64(节点, 特征_诊断区域投影最大Y(), 区域.投影最大Y);
        (void)读取基础节点I64(节点, 特征_诊断区域像素数量(), 区域.像素数量);
        (void)读取基础节点I64(节点, 特征_诊断区域置信度(), 区域.置信度);
        (void)读取基础节点I64(节点, 特征_诊断区域掩码状态(), 区域.掩码状态);
        return 区域.图层类型 > 0;
    }

    inline bool 读取观察诊断区域逐项账本(
        基础信息节点类* 宿主,
        std::vector<双目相机本能适配器::诊断区域摘要>& 区域列表) noexcept
    {
        区域列表.clear();
        if (!宿主) return false;
        for (auto* 子存在 : 世界树.存在().获取子存在(宿主)) {
            const auto* 主信息 = 世界树.存在().取存在主信息(子存在);
            if (!主信息 || 主信息->类型 != 类型_观察诊断区域()) {
                continue;
            }
            双目相机本能适配器::诊断区域摘要 区域{};
            if (读取观察诊断区域事实(子存在, 区域)) {
                区域列表.push_back(区域);
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
        return !区域列表.empty();
    }

    inline void 写入双目相机观察帧摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        (void)写入基础节点I64(宿主, 特征_当前观察特征帧(), 1, now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧(), 1, now);
        (void)写入基础节点I64(宿主, 特征_相机帧宽度(), static_cast<I64>(结果.宽度), now);
        (void)写入基础节点I64(宿主, 特征_相机帧高度(), static_cast<I64>(结果.高度), now);
        (void)写入基础节点I64(宿主, 特征_深度帧号(), static_cast<I64>(结果.深度帧号), now);
        (void)写入基础节点I64(宿主, 特征_彩色帧号(), static_cast<I64>(结果.彩色帧号), now);
        (void)写入基础节点I64(宿主, 特征_预期像素数量(), 结果.预期像素数量, now);
        (void)写入基础节点I64(宿主, 特征_像素特征数量(), 结果.像素特征数量, now);
        (void)写入基础节点I64(宿主, 特征_像素观察存在数量(), 结果.像素观察存在数量, now);
        (void)写入基础节点I64(宿主, 特征_缺失像素数量(), 结果.缺失像素数量, now);
        (void)写入基础节点I64(宿主, 特征_重复映射数量(), 结果.重复映射数量, now);
        (void)写入基础节点I64(宿主, 特征_颜色像素数量(), 结果.颜色像素数量, now);
        (void)写入基础节点I64(宿主, 特征_深度有效像素数量(), 结果.深度有效像素数量, now);
        (void)写入基础节点I64(宿主, 特征_点云有效像素数量(), 结果.点云有效像素数量, now);
        (void)写入基础节点I64(宿主, 特征_像素覆盖状态(), 结果.像素覆盖状态, now);
        (void)写入基础节点I64(宿主, 特征_当前观察特征帧取得状态(), 结果.当前观察特征帧取得状态, now);
        (void)写入基础节点I64(宿主, 特征_观察帧容器状态(), 结果.观察帧容器状态, now);
        (void)写入基础节点I64(宿主, 特征_像素特征值承接状态(), 结果.像素特征值承接状态, now);
        (void)写入基础节点I64(宿主, 特征_坐标回查可用状态(), 结果.坐标回查可用状态, now);
        (void)写入基础节点I64(宿主, 特征_帧内编号回查可用状态(), 结果.帧内编号回查可用状态, now);
        (void)写入基础节点I64(宿主, 特征_观察事实完备状态(), 结果.观察事实完备状态, now);
        (void)写入基础节点I64(宿主, 特征_观察事实完备失败条件数量(), 结果.观察事实完备失败条件数量, now);
        (void)写入基础节点I64(宿主, 特征_质量可用状态(), 结果.质量可用状态, now);
        (void)写入基础节点I64(宿主, 特征_质量失败条件数量(), 结果.质量失败条件数量, now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧采集时间(), 结果.当前观察帧采集时间, now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧观察年龄(), 结果.当前观察帧观察年龄, now);
        (void)写入基础节点I64(宿主, 特征_最大允许观察年龄(), 结果.最大允许观察年龄, now);
        (void)写入基础节点I64(宿主, 特征_新鲜度判定时间(), 结果.新鲜度判定时间, now);
        (void)写入基础节点I64(宿主, 特征_观察帧失效状态(), 结果.观察帧失效状态, now);
        (void)写入基础节点I64(宿主, 特征_观察帧来源外设状态(), 结果.观察帧来源外设状态, now);
        (void)写入基础节点I64(宿主, 特征_当前观察新鲜度状态(), 结果.当前观察新鲜度状态, now);
        (void)写入基础节点I64(宿主, 特征_新鲜度失败原因(), 结果.新鲜度失败原因, now);
        (void)写入基础节点I64(宿主, 特征_基础观察事实可用状态(), 结果.基础观察事实可用状态, now);
        (void)写入基础节点I64(宿主, 特征_基础观察事实失败条件数量(), 结果.基础观察事实失败条件数量, now);
        (void)写入基础节点I64(宿主, 特征_空间坐标单位毫米(), 结果.空间坐标单位毫米, now);
        (void)写入基础节点I64(宿主, 特征_颜色RGB结构状态(), 结果.颜色RGB结构状态, now);
        (void)写入基础节点I64(宿主, 特征_原始深度毫米结构状态(), 结果.原始深度毫米结构状态, now);
        (void)写入基础节点I64(宿主, 特征_滤波深度毫米结构状态(), 结果.滤波深度毫米结构状态, now);
        (void)写入基础节点I64(宿主, 特征_补全深度毫米结构状态(), 结果.补全深度毫米结构状态, now);
        (void)写入基础节点I64(宿主, 特征_深度有效性Mask状态(), 结果.深度有效性Mask状态, now);
        (void)写入基础节点I64(宿主, 特征_深度来源结构状态(), 结果.深度来源结构状态, now);
        (void)写入基础节点I64(宿主, 特征_深度稳定性结构状态(), 结果.深度稳定性结构状态, now);
        (void)写入基础节点I64(宿主, 特征_深度邻域一致性结构状态(), 结果.深度邻域一致性结构状态, now);
        (void)写入基础节点I64(宿主, 特征_融合深度毫米结构状态(), 结果.融合深度毫米结构状态, now);
        (void)写入基础节点I64(宿主, 特征_融合深度有效性结构状态(), 结果.融合深度有效性结构状态, now);
        (void)写入基础节点I64(宿主, 特征_多帧深度稳定性结构状态(), 结果.多帧深度稳定性结构状态, now);
        (void)写入基础节点I64(宿主, 特征_每像素深度方差结构状态(), 结果.每像素深度方差结构状态, now);
        (void)写入基础节点I64(宿主, 特征_空间坐标毫米XYZ结构状态(), 结果.空间坐标毫米XYZ结构状态, now);
        (void)写入基础节点I64(宿主, 特征_原始深度来源像素数量(), 结果.原始深度来源像素数量, now);
        (void)写入基础节点I64(宿主, 特征_滤波深度来源像素数量(), 结果.滤波深度来源像素数量, now);
        (void)写入基础节点I64(宿主, 特征_补全深度来源像素数量(), 结果.补全深度来源像素数量, now);
        (void)写入基础节点I64(宿主, 特征_无有效深度来源像素数量(), 结果.无有效深度来源像素数量, now);
        (void)写入基础节点I64(宿主, 特征_补全深度低置信像素数量(), 结果.补全深度低置信像素数量, now);
        (void)写入基础节点I64(宿主, 特征_深度稳定性平均值(), 结果.深度稳定性平均值, now);
        (void)写入基础节点I64(宿主, 特征_深度邻域一致性平均值(), 结果.深度邻域一致性平均值, now);
        (void)写入基础节点I64(宿主, 特征_观察帧组状态(), 结果.观察帧组状态, now);
        (void)写入基础节点I64(宿主, 特征_观察帧组帧数(), 结果.观察帧组帧数, now);
        (void)写入基础节点I64(宿主, 特征_融合依据帧集合状态(), 结果.融合依据帧集合状态, now);
        (void)写入基础节点I64(宿主, 特征_融合深度有效像素数量(), 结果.融合深度有效像素数量, now);
        (void)写入基础节点I64(宿主, 特征_融合深度有效率(), 结果.融合深度有效率, now);
        (void)写入基础节点I64(宿主, 特征_融合补偿深度空洞像素数量(), 结果.融合补偿深度空洞像素数量, now);
        (void)写入基础节点I64(宿主, 特征_融合低稳定像素数量(), 结果.融合低稳定像素数量, now);
        (void)写入基础节点I64(宿主, 特征_融合深度稳定性平均值(), 结果.融合深度稳定性平均值, now);
        (void)写入基础节点I64(宿主, 特征_融合深度平均方差(), 结果.融合深度平均方差, now);
        (void)写入基础节点I64(宿主, 特征_彩色深度已对齐(), 结果.彩色深度已对齐, now);
        (void)写入基础节点I64(宿主, 特征_对齐目标(), 结果.对齐目标, now);
        (void)写入基础节点I64(宿主, 特征_深度有效率(), 结果.深度有效率, now);
        (void)写入基础节点I64(宿主, 特征_空间坐标有效率(), 结果.空间坐标有效率, now);
        (void)写入基础节点I64(宿主, 特征_帧质量评分(), 结果.帧质量评分, now);
        (void)写入基础节点I64(宿主, 特征_观察帧质量诊断摘要(), 结果.观察帧质量诊断摘要, now);
        (void)写入基础节点I64(宿主, 特征_深度空洞数量(), 结果.深度空洞数量, now);
        (void)写入基础节点I64(宿主, 特征_深度空洞区域数量(), 结果.深度空洞区域数量, now);
        (void)写入基础节点I64(宿主, 特征_深度空洞区域集合(), 结果.深度空洞区域集合状态, now);
        (void)写入基础节点I64(宿主, 特征_彩色深度对齐状态(), 结果.彩色深度对齐状态, now);
        (void)写入基础节点I64(宿主, 特征_轮廓闭合率(), 结果.轮廓闭合率, now);
        (void)写入基础节点I64(宿主, 特征_轮廓断裂数量(), 结果.轮廓断裂数量, now);
        (void)写入基础节点I64(宿主, 特征_彩图轮廓数量(), 结果.彩图轮廓数量, now);
        (void)写入基础节点I64(宿主, 特征_深度轮廓数量(), 结果.深度轮廓数量, now);
        (void)写入基础节点I64(宿主, 特征_空间投影轮廓数量(), 结果.空间投影轮廓数量, now);
        (void)写入基础节点I64(宿主, 特征_融合轮廓数量(), 结果.融合轮廓数量, now);
        (void)写入基础节点I64(宿主, 特征_轮廓来源分层状态(), 结果.轮廓来源分层状态, now);
        (void)写入基础节点I64(宿主, 特征_轮廓深度支持率(), 结果.轮廓深度支持率, now);
        (void)写入基础节点I64(宿主, 特征_轮廓颜色支持率(), 结果.轮廓颜色支持率, now);
        (void)写入基础节点I64(宿主, 特征_轮廓空间支持率(), 结果.轮廓空间支持率, now);
        (void)写入基础节点I64(宿主, 特征_融合轮廓置信度(), 结果.融合轮廓置信度, now);
        (void)写入基础节点I64(宿主, 特征_跨越深度断裂轮廓数量(), 结果.跨越深度断裂轮廓数量, now);
        (void)写入基础节点I64(宿主, 特征_穿过深度无效区域轮廓数量(), 结果.穿过深度无效区域轮廓数量, now);
        (void)写入基础节点I64(宿主, 特征_依赖补全深度轮廓数量(), 结果.依赖补全深度轮廓数量, now);
        (void)写入基础节点I64(宿主, 特征_空间点有效率(), 结果.空间点有效率, now);
        (void)写入基础节点I64(宿主, 特征_边界深度稳定率(), 结果.边界深度稳定率, now);
        (void)写入基础节点I64(宿主, 特征_候选存在验证通过率(), 结果.候选存在验证通过率, now);
        (void)写入基础节点I64(宿主, 特征_未解释区域比例(), 结果.未解释区域比例, now);
        (void)写入基础节点I64(宿主, 特征_补观察缺口状态(), 结果.补观察缺口状态, now);
        (void)写入基础节点I64(宿主, 特征_补观察缺失原因(), 结果.补观察缺失原因, now);
        (void)写入基础节点I64(宿主, 特征_待补观察区域数量(), 结果.待补观察区域数量, now);
        (void)写入基础节点I64(宿主, 特征_补全候选数量(), 结果.补全候选数量, now);
        (void)写入基础节点I64(宿主, 特征_低置信轮廓数量(), 结果.低置信轮廓数量, now);
        (void)写入基础节点I64(宿主, 特征_条件不足候选数量(), 结果.条件不足候选数量, now);
        (void)写入基础节点I64(宿主, 特征_待验证候选数量(), 结果.待验证候选数量, now);
        (void)写入基础节点I64(宿主, 特征_部分确认候选数量(), 结果.部分确认候选数量, now);
        (void)写入基础节点I64(宿主, 特征_补观察需求建议(), 结果.补观察需求建议, now);
        (void)写入基础节点I64(宿主, 特征_诊断区域数量(), 结果.诊断区域数量, now);
        (void)写入基础节点I64(宿主, 特征_诊断区域集合状态(), 结果.诊断区域集合状态, now);
        (void)写入基础节点I64(宿主, 特征_诊断区域掩码状态(), 结果.诊断区域掩码状态, now);
        (void)写入基础节点I64(宿主, 特征_帧解释状态(), 0, now);
        写入空间候选逐项账本(宿主, 结果, now);
        写入观察诊断区域逐项账本(宿主, 结果, now);
    }

    inline void 写入双目相机像素特征索引摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入双目相机观察帧摘要(宿主, 结果, now);
        (void)写入基础节点I64(宿主, 特征_当前帧像素特征索引(), 1, now);
        (void)写入基础节点I64(宿主, 特征_像素观察事实索引(), 1, now);
    }

    inline void 写入自我像素观察事实索引摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        (void)写入基础节点I64(宿主, 特征_当前观察帧(), 1, now);
        (void)写入基础节点I64(宿主, 特征_当前帧像素特征索引(), 1, now);
        (void)写入基础节点I64(宿主, 特征_像素观察事实索引(), 1, now);
        (void)写入基础节点I64(宿主, 特征_相机帧宽度(), static_cast<I64>(结果.宽度), now);
        (void)写入基础节点I64(宿主, 特征_相机帧高度(), static_cast<I64>(结果.高度), now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧采集时间(), 结果.当前观察帧采集时间, now);
        (void)写入基础节点I64(宿主, 特征_最大允许观察年龄(), 结果.最大允许观察年龄, now);
        (void)写入基础节点I64(宿主, 特征_观察帧失效状态(), 结果.观察帧失效状态, now);
        (void)写入基础节点I64(宿主, 特征_观察帧来源外设状态(), 结果.观察帧来源外设状态, now);
        (void)写入基础节点I64(宿主, 特征_预期像素数量(), 结果.预期像素数量, now);
        (void)写入基础节点I64(宿主, 特征_像素特征数量(), 结果.像素特征数量, now);
        (void)写入基础节点I64(宿主, 特征_像素观察存在数量(), 结果.像素观察存在数量, now);
        (void)写入基础节点I64(宿主, 特征_缺失像素数量(), 结果.缺失像素数量, now);
        (void)写入基础节点I64(宿主, 特征_重复映射数量(), 结果.重复映射数量, now);
        (void)写入基础节点I64(宿主, 特征_像素覆盖状态(), 结果.像素覆盖状态, now);
        (void)写入基础节点I64(宿主, 特征_坐标回查可用状态(), 结果.坐标回查可用状态, now);
        (void)写入基础节点I64(宿主, 特征_帧内编号回查可用状态(), 结果.帧内编号回查可用状态, now);
    }

    inline void 写入自我像素特征值承接摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入自我像素观察事实索引摘要(宿主, 结果, now);
        (void)写入基础节点I64(宿主, 特征_颜色像素数量(), 结果.颜色像素数量, now);
        (void)写入基础节点I64(宿主, 特征_深度有效像素数量(), 结果.深度有效像素数量, now);
        (void)写入基础节点I64(宿主, 特征_点云有效像素数量(), 结果.点云有效像素数量, now);
        (void)写入基础节点I64(宿主, 特征_颜色RGB结构状态(), 结果.颜色RGB结构状态, now);
        (void)写入基础节点I64(宿主, 特征_原始深度毫米结构状态(), 结果.原始深度毫米结构状态, now);
        (void)写入基础节点I64(宿主, 特征_深度有效性Mask状态(), 结果.深度有效性Mask状态, now);
        (void)写入基础节点I64(宿主, 特征_空间坐标毫米XYZ结构状态(), 结果.空间坐标毫米XYZ结构状态, now);
        (void)写入基础节点I64(宿主, 特征_像素特征值承接状态(), 结果.像素特征值承接状态, now);
    }

    inline void 写入自我观察事实完备摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入自我像素特征值承接摘要(宿主, 结果, now);
        (void)写入基础节点I64(宿主, 特征_坐标回查可用状态(), 结果.坐标回查可用状态, now);
        (void)写入基础节点I64(宿主, 特征_帧内编号回查可用状态(), 结果.帧内编号回查可用状态, now);
        (void)写入基础节点I64(宿主, 特征_观察事实完备状态(), 结果.观察事实完备状态, now);
        (void)写入基础节点I64(宿主, 特征_观察事实完备失败条件数量(), 结果.观察事实完备失败条件数量, now);
    }

    inline void 写入自我观察帧质量摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入自我观察事实完备摘要(宿主, 结果, now);
        (void)写入基础节点I64(宿主, 特征_质量可用状态(), 结果.质量可用状态, now);
        (void)写入基础节点I64(宿主, 特征_质量失败条件数量(), 结果.质量失败条件数量, now);
        (void)写入基础节点I64(宿主, 特征_观察帧质量诊断摘要(), 结果.观察帧质量诊断摘要, now);
        (void)写入基础节点I64(宿主, 特征_帧质量评分(), 结果.帧质量评分, now);
        (void)写入基础节点I64(宿主, 特征_深度有效率(), 结果.深度有效率, now);
        (void)写入基础节点I64(宿主, 特征_空间坐标有效率(), 结果.空间坐标有效率, now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧采集时间(), 结果.当前观察帧采集时间, now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧观察年龄(), 结果.当前观察帧观察年龄, now);
        (void)写入基础节点I64(宿主, 特征_最大允许观察年龄(), 结果.最大允许观察年龄, now);
        (void)写入基础节点I64(宿主, 特征_新鲜度判定时间(), 结果.新鲜度判定时间, now);
        (void)写入基础节点I64(宿主, 特征_观察帧失效状态(), 结果.观察帧失效状态, now);
        (void)写入基础节点I64(宿主, 特征_观察帧来源外设状态(), 结果.观察帧来源外设状态, now);
        (void)写入基础节点I64(宿主, 特征_当前观察新鲜度状态(), 结果.当前观察新鲜度状态, now);
        (void)写入基础节点I64(宿主, 特征_新鲜度失败原因(), 结果.新鲜度失败原因, now);
        (void)写入基础节点I64(宿主, 特征_融合深度有效率(), 结果.融合深度有效率, now);
        (void)写入基础节点I64(宿主, 特征_未解释区域比例(), 结果.未解释区域比例, now);
        (void)写入基础节点I64(宿主, 特征_补观察缺口状态(), 结果.补观察缺口状态, now);
        (void)写入基础节点I64(宿主, 特征_补观察缺失原因(), 结果.补观察缺失原因, now);
    }

    inline void 写入自我基础观察事实可用摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主) return;
        (void)写入基础节点I64(宿主, 特征_当前观察特征帧取得状态(), 结果.当前观察特征帧取得状态, now);
        (void)写入基础节点I64(宿主, 特征_观察帧容器状态(), 结果.观察帧容器状态, now);
        (void)写入基础节点I64(宿主, 特征_像素覆盖状态(), 结果.像素覆盖状态, now);
        (void)写入基础节点I64(宿主, 特征_像素特征值承接状态(), 结果.像素特征值承接状态, now);
        (void)写入基础节点I64(宿主, 特征_坐标回查可用状态(), 结果.坐标回查可用状态, now);
        (void)写入基础节点I64(宿主, 特征_帧内编号回查可用状态(), 结果.帧内编号回查可用状态, now);
        (void)写入基础节点I64(宿主, 特征_观察事实完备状态(), 结果.观察事实完备状态, now);
        (void)写入基础节点I64(宿主, 特征_观察事实完备失败条件数量(), 结果.观察事实完备失败条件数量, now);
        (void)写入基础节点I64(宿主, 特征_质量可用状态(), 结果.质量可用状态, now);
        (void)写入基础节点I64(宿主, 特征_质量失败条件数量(), 结果.质量失败条件数量, now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧采集时间(), 结果.当前观察帧采集时间, now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧观察年龄(), 结果.当前观察帧观察年龄, now);
        (void)写入基础节点I64(宿主, 特征_最大允许观察年龄(), 结果.最大允许观察年龄, now);
        (void)写入基础节点I64(宿主, 特征_新鲜度判定时间(), 结果.新鲜度判定时间, now);
        (void)写入基础节点I64(宿主, 特征_观察帧失效状态(), 结果.观察帧失效状态, now);
        (void)写入基础节点I64(宿主, 特征_观察帧来源外设状态(), 结果.观察帧来源外设状态, now);
        (void)写入基础节点I64(宿主, 特征_当前观察新鲜度状态(), 结果.当前观察新鲜度状态, now);
        (void)写入基础节点I64(宿主, 特征_新鲜度失败原因(), 结果.新鲜度失败原因, now);
        (void)写入基础节点I64(宿主, 特征_基础观察事实可用状态(), 结果.基础观察事实可用状态, now);
        (void)写入基础节点I64(宿主, 特征_基础观察事实失败条件数量(), 结果.基础观察事实失败条件数量, now);
    }

    inline void 写入自我观察帧存在摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || 结果.观察帧容器状态 != 观察事实状态_已建立) return;
        (void)写入基础节点I64(宿主, 特征_当前观察帧(), 1, now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧存在(), 1, now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧存在状态(), 结果.观察帧容器状态, now);
        (void)写入基础节点I64(宿主, 特征_观察帧容器状态(), 结果.观察帧容器状态, now);
        (void)写入基础节点I64(宿主, 特征_相机帧宽度(), static_cast<I64>(结果.宽度), now);
        (void)写入基础节点I64(宿主, 特征_相机帧高度(), static_cast<I64>(结果.高度), now);
        (void)写入基础节点I64(宿主, 特征_当前观察帧采集时间(), 结果.当前观察帧采集时间, now);
        (void)写入基础节点I64(宿主, 特征_最大允许观察年龄(), 结果.最大允许观察年龄, now);
        (void)写入基础节点I64(宿主, 特征_观察帧失效状态(), 结果.观察帧失效状态, now);
        (void)写入基础节点I64(宿主, 特征_观察帧来源外设状态(), 结果.观察帧来源外设状态, now);
        (void)写入基础节点I64(宿主, 特征_深度帧号(), static_cast<I64>(结果.深度帧号), now);
        (void)写入基础节点I64(宿主, 特征_彩色帧号(), static_cast<I64>(结果.彩色帧号), now);
        (void)写入基础节点I64(宿主, 特征_预期像素数量(), 结果.预期像素数量, now);
    }

    inline void 写入自我观察帧接口摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入自我观察帧存在摘要(宿主, 结果, now);
        (void)写入基础节点I64(宿主, 特征_观察帧到自我场景接口(), 1, now);
    }

    inline bool 读取观察摘要I64(
        基础信息节点类* 目标宿主,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        const 语素入口节点类* 特征类型,
        I64& 输出值) noexcept
    {
        return 读取基础节点I64(目标宿主, 特征类型, 输出值)
            || 读取场景I64特征(输入参数场景, 特征类型, 输出值)
            || 读取场景I64特征(输出结果场景, 特征类型, 输出值);
    }

    inline bool 读取已落账观察特征帧摘要(
        基础信息节点类* 目标宿主,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        结构_双目相机调用结果& 结果) noexcept
    {
        结果 = {};
        I64 已取得 = 0;
        if (!读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_当前观察特征帧(), 已取得)
            || 已取得 <= 0) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "缺少已落账当前观察特征帧";
            return false;
        }

        I64 宽度 = 0;
        I64 高度 = 0;
        if (!读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_相机帧宽度(), 宽度)
            || !读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_相机帧高度(), 高度)
            || 宽度 <= 0
            || 高度 <= 0) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "当前观察特征帧缺少宽高摘要";
            return false;
        }

        I64 值 = 0;
        结果.成功 = true;
        结果.相机已打开 = true;
        结果.消息 = "读取已落账观察特征帧摘要";
        结果.宽度 = static_cast<int>(宽度);
        结果.高度 = static_cast<int>(高度);
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度帧号(), 值)) {
            结果.深度帧号 = static_cast<std::uint32_t>(std::max<I64>(0, 值));
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_彩色帧号(), 值)) {
            结果.彩色帧号 = static_cast<std::uint32_t>(std::max<I64>(0, 值));
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_当前观察帧采集时间(), 值)) {
            结果.当前观察帧采集时间 = std::max<I64>(0, 值);
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_最大允许观察年龄(), 值)) {
            结果.最大允许观察年龄 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_观察帧失效状态(), 值)) {
            结果.观察帧失效状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_观察帧来源外设状态(), 值)) {
            结果.观察帧来源外设状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_预期像素数量(), 值)) {
            结果.预期像素数量 = 值;
        } else {
            结果.预期像素数量 = 宽度 * 高度;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_像素特征数量(), 值)) {
            结果.像素特征数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_颜色像素数量(), 值)) {
            结果.颜色像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度有效像素数量(), 值)) {
            结果.深度有效像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_点云有效像素数量(), 值)) {
            结果.点云有效像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_像素覆盖状态(), 值)) {
            结果.像素覆盖状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_空间坐标单位毫米(), 值)) {
            结果.空间坐标单位毫米 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_颜色RGB结构状态(), 值)) {
            结果.颜色RGB结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_原始深度毫米结构状态(), 值)) {
            结果.原始深度毫米结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_滤波深度毫米结构状态(), 值)) {
            结果.滤波深度毫米结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_补全深度毫米结构状态(), 值)) {
            结果.补全深度毫米结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度有效性Mask状态(), 值)) {
            结果.深度有效性Mask状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度来源结构状态(), 值)) {
            结果.深度来源结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度稳定性结构状态(), 值)) {
            结果.深度稳定性结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度邻域一致性结构状态(), 值)) {
            结果.深度邻域一致性结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合深度毫米结构状态(), 值)) {
            结果.融合深度毫米结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合深度有效性结构状态(), 值)) {
            结果.融合深度有效性结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_多帧深度稳定性结构状态(), 值)) {
            结果.多帧深度稳定性结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_每像素深度方差结构状态(), 值)) {
            结果.每像素深度方差结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_空间坐标毫米XYZ结构状态(), 值)) {
            结果.空间坐标毫米XYZ结构状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_原始深度来源像素数量(), 值)) {
            结果.原始深度来源像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_滤波深度来源像素数量(), 值)) {
            结果.滤波深度来源像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_补全深度来源像素数量(), 值)) {
            结果.补全深度来源像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_无有效深度来源像素数量(), 值)) {
            结果.无有效深度来源像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_补全深度低置信像素数量(), 值)) {
            结果.补全深度低置信像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度稳定性平均值(), 值)) {
            结果.深度稳定性平均值 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度邻域一致性平均值(), 值)) {
            结果.深度邻域一致性平均值 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_观察帧组状态(), 值)) {
            结果.观察帧组状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_观察帧组帧数(), 值)) {
            结果.观察帧组帧数 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合依据帧集合状态(), 值)) {
            结果.融合依据帧集合状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合深度有效像素数量(), 值)) {
            结果.融合深度有效像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合深度有效率(), 值)) {
            结果.融合深度有效率 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合补偿深度空洞像素数量(), 值)) {
            结果.融合补偿深度空洞像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合低稳定像素数量(), 值)) {
            结果.融合低稳定像素数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合深度稳定性平均值(), 值)) {
            结果.融合深度稳定性平均值 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合深度平均方差(), 值)) {
            结果.融合深度平均方差 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_彩色深度已对齐(), 值)) {
            结果.彩色深度已对齐 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_对齐目标(), 值)) {
            结果.对齐目标 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度有效率(), 值)) {
            结果.深度有效率 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_空间坐标有效率(), 值)) {
            结果.空间坐标有效率 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_帧质量评分(), 值)) {
            结果.帧质量评分 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_观察帧质量诊断摘要(), 值)) {
            结果.观察帧质量诊断摘要 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度空洞数量(), 值)) {
            结果.深度空洞数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度空洞区域数量(), 值)) {
            结果.深度空洞区域数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度空洞区域集合(), 值)) {
            结果.深度空洞区域集合状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_彩色深度对齐状态(), 值)) {
            结果.彩色深度对齐状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_轮廓闭合率(), 值)) {
            结果.轮廓闭合率 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_轮廓断裂数量(), 值)) {
            结果.轮廓断裂数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_彩图轮廓数量(), 值)) {
            结果.彩图轮廓数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_深度轮廓数量(), 值)) {
            结果.深度轮廓数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_空间投影轮廓数量(), 值)) {
            结果.空间投影轮廓数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合轮廓数量(), 值)) {
            结果.融合轮廓数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_轮廓来源分层状态(), 值)) {
            结果.轮廓来源分层状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_轮廓深度支持率(), 值)) {
            结果.轮廓深度支持率 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_轮廓颜色支持率(), 值)) {
            结果.轮廓颜色支持率 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_轮廓空间支持率(), 值)) {
            结果.轮廓空间支持率 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_融合轮廓置信度(), 值)) {
            结果.融合轮廓置信度 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_跨越深度断裂轮廓数量(), 值)) {
            结果.跨越深度断裂轮廓数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_穿过深度无效区域轮廓数量(), 值)) {
            结果.穿过深度无效区域轮廓数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_依赖补全深度轮廓数量(), 值)) {
            结果.依赖补全深度轮廓数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_空间点有效率(), 值)) {
            结果.空间点有效率 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_边界深度稳定率(), 值)) {
            结果.边界深度稳定率 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_候选存在验证通过率(), 值)) {
            结果.候选存在验证通过率 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_未解释区域比例(), 值)) {
            结果.未解释区域比例 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_补观察缺口状态(), 值)) {
            结果.补观察缺口状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_补观察缺失原因(), 值)) {
            结果.补观察缺失原因 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_待补观察区域数量(), 值)) {
            结果.待补观察区域数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_补全候选数量(), 值)) {
            结果.补全候选数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_低置信轮廓数量(), 值)) {
            结果.低置信轮廓数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_条件不足候选数量(), 值)) {
            结果.条件不足候选数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_待验证候选数量(), 值)) {
            结果.待验证候选数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_部分确认候选数量(), 值)) {
            结果.部分确认候选数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_补观察需求建议(), 值)) {
            结果.补观察需求建议 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_诊断区域数量(), 值)) {
            结果.诊断区域数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_诊断区域集合状态(), 值)) {
            结果.诊断区域集合状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_诊断区域掩码状态(), 值)) {
            结果.诊断区域掩码状态 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_空间候选数量(), 值)) {
            结果.空间候选数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_空间候选有效点数量(), 值)) {
            结果.空间候选有效点数量 = 值;
        }
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_轮廓数量(), 值)) {
            结果.轮廓数量 = static_cast<std::size_t>(std::max<I64>(0, 值));
        }
        补全基础观察事实状态(结果);
        return true;
    }

    inline bool 读取已落账空间候选集合(
        基础信息节点类* 目标宿主,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        结构_双目相机调用结果& 结果) noexcept
    {
        if (!读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 输出结果场景, 结果)) {
            return false;
        }

        std::vector<双目相机本能适配器::空间候选摘要> 候选列表{};
        if (!读取空间候选逐项账本(目标宿主, 候选列表)
            && !读取空间候选逐项账本(reinterpret_cast<基础信息节点类*>(输出结果场景), 候选列表)
            && !读取空间候选逐项账本(reinterpret_cast<基础信息节点类*>(输入参数场景), 候选列表)) {
            结果.成功 = false;
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "缺少已落账逐项空间候选账本";
            return false;
        }

        结果.成功 = true;
        结果.空间候选列表 = 候选列表;
        结果.空间候选数量 = static_cast<I64>(结果.空间候选列表.size());
        std::vector<双目相机本能适配器::诊断区域摘要> 区域列表{};
        if (读取观察诊断区域逐项账本(目标宿主, 区域列表)
            || 读取观察诊断区域逐项账本(reinterpret_cast<基础信息节点类*>(输出结果场景), 区域列表)
            || 读取观察诊断区域逐项账本(reinterpret_cast<基础信息节点类*>(输入参数场景), 区域列表)) {
            结果.诊断区域列表 = 区域列表;
            if (结果.诊断区域数量 <= 0) {
                结果.诊断区域数量 = static_cast<I64>(结果.诊断区域列表.size());
            }
            if (!结果.诊断区域列表.empty()) {
                结果.诊断区域集合状态 = 1;
                结果.诊断区域掩码状态 = 1;
            }
        }
        I64 有效点数量 = 0;
        for (const auto& 候选 : 结果.空间候选列表) {
            有效点数量 += std::max<I64>(0, 候选.像素数量);
        }
        if (结果.空间候选有效点数量 <= 0) {
            结果.空间候选有效点数量 = 有效点数量;
        }
        return true;
    }

    inline bool 已落账像素观察事实索引(
        基础信息节点类* 目标宿主,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        I64 索引状态 = 0;
        if (读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_像素观察事实索引(), 索引状态)
            && 索引状态 > 0) {
            return true;
        }
        return 读取观察摘要I64(目标宿主, 输入参数场景, 输出结果场景, 特征_当前帧像素特征索引(), 索引状态)
            && 索引状态 > 0;
    }

    inline void 写入空间候选观察队列逐项账本(
        基础信息节点类* 宿主,
        const 结构_空间候选观察队列结果& 队列,
        时间戳 now) noexcept
    {
        if (!宿主) return;
        for (const auto& 队列项 : 队列.队列) {
            auto* 候选存在 = 取或创建空间候选观察事实(宿主, 队列项.候选编号);
            auto* 节点 = reinterpret_cast<基础信息节点类*>(候选存在);
            if (!节点) continue;
            (void)写入基础节点I64(节点, 特征_观察队列排序位次(), 队列项.排序位次, now);
            (void)写入基础节点I64(节点, 特征_空间候选优先级评分(), 队列项.优先级评分, now);
            (void)写入基础节点I64(节点, 特征_空间候选相对独立状态(), 队列项.相对独立状态, now);
            (void)写入基础节点I64(节点, 特征_空间候选中心接近评分(), 队列项.中心接近评分, now);
            (void)写入基础节点I64(节点, 特征_空间候选范围大小评分(), 队列项.范围大小评分, now);
        }
    }

    inline 存在节点类* 取或创建已确认观察存在(
        基础信息节点类* 宿主,
        I64 来源候选编号) noexcept
    {
        return 宿主
            ? 世界树.取或创建子存在_按类型并按特征I64(
                宿主,
                类型_已确认观察存在(),
                特征_来源空间候选编号(),
                来源候选编号)
            : nullptr;
    }

    inline void 写入已确认观察存在节点原始事实(
        存在节点类* 观察存在,
        const 结构_观察存在假设结果& 假设,
        const 结构_像素归属验证结果& 验证,
        时间戳 now) noexcept
    {
        auto* 节点 = reinterpret_cast<基础信息节点类*>(观察存在);
        if (!节点) return;
        (void)写入基础节点I64(节点, 特征_来源空间候选编号(), 假设.来源候选编号, now);
        (void)写入基础节点I64(节点, 特征_观察存在确认状态(), 观察存在确认状态_已验证, now);
        (void)写入基础节点I64(节点, 特征_中心空间坐标X(), 假设.候选.中心X, now);
        (void)写入基础节点I64(节点, 特征_中心空间坐标Y(), 假设.候选.中心Y, now);
        (void)写入基础节点I64(节点, 特征_中心空间坐标Z(), 假设.候选.中心Z, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小X(), 假设.候选.范围最小X, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大X(), 假设.候选.范围最大X, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小Y(), 假设.候选.范围最小Y, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大Y(), 假设.候选.范围最大Y, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小Z(), 假设.候选.范围最小Z, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大Z(), 假设.候选.范围最大Z, now);
        (void)写入基础节点I64(节点, 特征_投影范围最小X(), 假设.候选.投影最小X, now);
        (void)写入基础节点I64(节点, 特征_投影范围最大X(), 假设.候选.投影最大X, now);
        (void)写入基础节点I64(节点, 特征_投影范围最小Y(), 假设.候选.投影最小Y, now);
        (void)写入基础节点I64(节点, 特征_投影范围最大Y(), 假设.候选.投影最大Y, now);
        (void)写入基础节点I64(节点, 特征_假设距离(), 假设.距离, now);
        (void)写入基础节点I64(节点, 特征_假设尺寸X(), 假设.尺寸X, now);
        (void)写入基础节点I64(节点, 特征_假设尺寸Y(), 假设.尺寸Y, now);
        (void)写入基础节点I64(节点, 特征_假设尺寸Z(), 假设.尺寸Z, now);
        (void)写入基础节点I64(节点, 特征_假设方位X(), 假设.方位X, now);
        (void)写入基础节点I64(节点, 特征_假设方位Y(), 假设.方位Y, now);
        (void)写入基础节点I64(节点, 特征_候选像素掩码状态(), 假设.候选像素掩码状态, now);
        (void)写入基础节点I64(节点, 特征_像素归属验证状态(), 验证.验证状态, now);
        (void)写入基础节点I64(节点, 特征_空间覆盖像素数(), 验证.空间覆盖像素数, now);
        (void)写入基础节点I64(节点, 特征_空间匹配像素数(), 验证.空间匹配像素数, now);
        (void)写入基础节点I64(节点, 特征_空间冲突像素数(), 验证.空间冲突像素数, now);
        (void)写入基础节点I64(节点, 特征_遮挡像素数(), 验证.遮挡像素数, now);
        (void)写入基础节点I64(节点, 特征_未解释像素数(), 验证.未解释像素数, now);
        (void)写入基础节点I64(节点, 特征_已归属像素数(), 验证.空间匹配像素数, now);
        (void)写入基础节点I64(节点, 特征_范围内点比例(), 验证.范围内点比例, now);
        (void)写入基础节点I64(节点, 特征_深度一致率(), 验证.深度一致率, now);
        (void)写入基础节点I64(节点, 特征_投影覆盖率(), 验证.投影覆盖率, now);
        (void)写入基础节点I64(节点, 特征_轮廓吻合率(), 验证.轮廓吻合率, now);
    }

    inline I64 计算场景坐标换算置信度(
        const 结构_外设观察存在确认方案项& 项) noexcept
    {
        if (项.簇.空间坐标有效率 <= 0 && 项.簇.深度有效率 <= 0) return 0;
        const I64 空间置信 = 项.簇.空间坐标有效率 > 0 ? 项.簇.空间坐标有效率 : 10000;
        const I64 深度置信 = 项.簇.深度有效率 > 0 ? 项.簇.深度有效率 : 10000;
        return std::clamp<I64>(std::min<I64>(空间置信, 深度置信), 1, 10000);
    }

    inline bool 读取观察存在来源中心坐标(
        存在节点类* 观察存在,
        I64& x,
        I64& y,
        I64& z) noexcept
    {
        x = 0;
        y = 0;
        z = 0;
        auto* 节点 = reinterpret_cast<基础信息节点类*>(观察存在);
        return 节点
            && 读取基础节点I64(节点, 特征_中心空间坐标X(), x)
            && 读取基础节点I64(节点, 特征_中心空间坐标Y(), y)
            && 读取基础节点I64(节点, 特征_中心空间坐标Z(), z);
    }

    inline std::vector<场景相对坐标项> 构造场景相对坐标表_同源观察(
        存在节点类* 目标存在,
        I64 目标中心X,
        I64 目标中心Y,
        I64 目标中心Z,
        I64 来源报告ID,
        const std::vector<存在节点类*>& 当前场景存在集合,
        I64 置信度) noexcept
    {
        std::vector<场景相对坐标项> 相对坐标表{};
        if (!目标存在 || 置信度 <= 0) return 相对坐标表;

        for (auto* 参考存在 : 当前场景存在集合) {
            if (!参考存在 || 参考存在 == 目标存在) continue;

            Vector3D 参考绝对坐标{};
            if (!世界树.读取存在场景绝对坐标(参考存在, 参考绝对坐标)) {
                continue;
            }

            auto* 参考节点 = reinterpret_cast<基础信息节点类*>(参考存在);
            I64 参考报告ID = 0;
            if (来源报告ID > 0
                && (!读取基础节点I64(参考节点, 特征_外设观察报告ID(), 参考报告ID)
                    || 参考报告ID != 来源报告ID)) {
                continue;
            }

            I64 参考中心X = 0;
            I64 参考中心Y = 0;
            I64 参考中心Z = 0;
            if (!读取观察存在来源中心坐标(
                    参考存在,
                    参考中心X,
                    参考中心Y,
                    参考中心Z)) {
                continue;
            }

            场景相对坐标项 坐标项{};
            坐标项.参考存在 = 参考存在;
            坐标项.相对坐标_mm = Vector3D{
                static_cast<double>(目标中心X - 参考中心X),
                static_cast<double>(目标中心Y - 参考中心Y),
                static_cast<double>(目标中心Z - 参考中心Z),
            };
            坐标项.置信度 = std::clamp<I64>(置信度, 0, 10000);
            相对坐标表.push_back(坐标项);
        }

        return 相对坐标表;
    }

    inline bool 写入观察存在提交关系二次特征(
        场景节点类* 场景,
        存在节点类* 观察存在,
        基础信息节点类* 客体,
        动态节点类* 来源动态,
        const char* 关系名,
        I64 标量值,
        bool 使用标量值) noexcept
    {
        if (!场景 || !观察存在 || !关系名 || !*关系名) return false;
        auto* 概念词 = 语素集.添加信息入口词(关系名, 枚举_信息入口类型::关系模板入口);
        auto* 主信息 = new 二次特征主信息类{};
        主信息->名称 = 概念词;
        主信息->类型 = 概念词;
        主信息->概念名称 = 概念词;
        主信息->形态 = 二次特征主信息类::枚举_形态::结果;
        主信息->种类 = 枚举_二次特征种类::观察关系;
        主信息->域 = 枚举_二次特征域::存在;
        主信息->粒度 = 枚举_二次特征粒度::当前场景;
        主信息->基准类型 = 枚举_二次特征基准类型::无;
        主信息->时间归一方式 = 枚举_时间归一方式::无;
        主信息->值形态 = 使用标量值
            ? 枚举_二次特征值形态::标量
            : 枚举_二次特征值形态::布尔;

        auto& 二次特征服务 = 世界树.二次特征();
        auto* 节点 = 二次特征服务.创建二次特征(
            reinterpret_cast<基础信息节点类*>(场景),
            主信息);
        if (!节点) return false;

        (void)二次特征服务.绑定场景(节点, 场景);
        (void)二次特征服务.设置生成语义(
            节点,
            枚举_二次特征域::存在,
            枚举_二次特征粒度::当前场景,
            枚举_二次特征基准类型::无,
            枚举_时间归一方式::无,
            主信息->值形态);
        (void)二次特征服务.设置主体客体(
            节点,
            reinterpret_cast<基础信息节点类*>(观察存在),
            客体);
        (void)二次特征服务.绑定来源(
            节点,
            nullptr,
            nullptr,
            来源动态,
            场景,
            0,
            0,
            来源动态 ? 枚举_二次特征来源类型::动态 : 枚举_二次特征来源类型::未定义);
        return 二次特征服务.写入标量值(
            节点,
            使用标量值 ? 标量值 : 1,
            true,
            10000);
    }

    inline void 写入单个已确认观察存在(
        基础信息节点类* 宿主,
        const 结构_观察存在假设结果& 假设,
        const 结构_像素归属验证结果& 验证,
        时间戳 now) noexcept
    {
        写入已确认观察存在节点原始事实(
            取或创建已确认观察存在(宿主, 假设.来源候选编号),
            假设,
            验证,
            now);
    }

    inline void 写入全帧像素归属账本摘要(
        基础信息节点类* 宿主,
        const 结构_全帧像素归属账本结果& 账本,
        时间戳 now) noexcept
    {
        if (!宿主 || 账本.账本状态 == 全帧像素归属账状态_未生成) return;
        (void)写入基础节点I64(宿主, 特征_全帧像素归属账状态(), 账本.账本状态, now);
        (void)写入基础节点I64(宿主, 特征_已验证观察存在数量(), 账本.已验证观察存在数量, now);
        (void)写入基础节点I64(宿主, 特征_已归属像素数(), 账本.已归属像素数, now);
        (void)写入基础节点I64(宿主, 特征_未解释像素数(), 账本.未解释像素数, now);
        (void)写入基础节点I64(宿主, 特征_归属冲突像素数(), 账本.归属冲突像素数, now);
        (void)写入基础节点I64(宿主, 特征_像素归属率(), 账本.像素归属率, now);
    }

    inline void 写入双目相机空间候选摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入双目相机像素特征索引摘要(宿主, 结果, now);
        (void)写入基础节点I64(宿主, 特征_空间候选集合(), 1, now);
        (void)写入基础节点I64(宿主, 特征_空间候选数量(), 结果.空间候选数量, now);
        (void)写入基础节点I64(宿主, 特征_空间候选有效点数量(), 结果.空间候选有效点数量, now);
        if (结果.空间候选列表.empty()) {
            return;
        }
        const auto& 主候选 = 结果.空间候选列表.front();
        (void)写入基础节点I64(宿主, 特征_主空间候选编号(), 主候选.候选编号, now);
        (void)写入基础节点I64(宿主, 特征_主空间候选像素数量(), 主候选.像素数量, now);
        (void)写入基础节点I64(宿主, 特征_中心空间坐标X(), 主候选.中心X, now);
        (void)写入基础节点I64(宿主, 特征_中心空间坐标Y(), 主候选.中心Y, now);
        (void)写入基础节点I64(宿主, 特征_中心空间坐标Z(), 主候选.中心Z, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最小X(), 主候选.范围最小X, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最大X(), 主候选.范围最大X, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最小Y(), 主候选.范围最小Y, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最大Y(), 主候选.范围最大Y, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最小Z(), 主候选.范围最小Z, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最大Z(), 主候选.范围最大Z, now);
        (void)写入基础节点I64(宿主, 特征_平均深度(), 主候选.平均深度, now);
        (void)写入基础节点I64(宿主, 特征_最近深度(), 主候选.最近深度, now);
        (void)写入基础节点I64(宿主, 特征_最远深度(), 主候选.最远深度, now);
        (void)写入基础节点I64(宿主, 特征_空间连续性评分(), 主候选.空间连续性评分, now);
        (void)写入基础节点I64(宿主, 特征_范围稳定性评分(), 主候选.范围稳定性评分, now);
    }

    inline void 写入双目相机空间候选独立性摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        const 结构_空间候选独立性判断结果& 判断,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入双目相机空间候选摘要(宿主, 结果, now);
        (void)写入基础节点I64(宿主, 特征_空间候选相对独立性判断(), 判断.判断状态, now);
        (void)写入基础节点I64(宿主, 特征_相对独立空间候选数量(), 判断.相对独立数量, now);
        (void)写入基础节点I64(宿主, 特征_不独立空间候选数量(), 判断.不独立数量, now);
        (void)写入基础节点I64(宿主, 特征_待拆分空间候选数量(), 判断.待拆分数量, now);
        (void)写入基础节点I64(宿主, 特征_待合并空间候选数量(), 判断.待合并数量, now);
        (void)写入基础节点I64(宿主, 特征_不确定空间候选数量(), 判断.不确定数量, now);
        (void)写入基础节点I64(宿主, 特征_主空间候选相对独立状态(), 判断.主候选状态, now);
        (void)写入基础节点I64(宿主, 特征_主空间候选分离度评分(), 判断.主候选分离度评分, now);
    }

    inline void 写入双目相机空间候选观察队列摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        const 结构_空间候选独立性判断结果& 判断,
        const 结构_空间候选观察队列结果& 队列,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入双目相机空间候选独立性摘要(宿主, 结果, 判断, now);
        (void)写入基础节点I64(宿主, 特征_空间候选观察队列(), 队列.队列状态, now);
        (void)写入基础节点I64(宿主, 特征_观察队列候选数量(), 队列.候选数量, now);
        写入空间候选观察队列逐项账本(宿主, 队列, now);
        if (队列.队列.empty()) {
            return;
        }
        const auto& 首选 = 队列.队列.front();
        (void)写入基础节点I64(宿主, 特征_首选观察候选编号(), 首选.候选编号, now);
        (void)写入基础节点I64(宿主, 特征_首选观察候选优先级评分(), 首选.优先级评分, now);
        (void)写入基础节点I64(宿主, 特征_首选观察候选相对独立状态(), 首选.相对独立状态, now);
        (void)写入基础节点I64(宿主, 特征_首选观察候选中心接近评分(), 首选.中心接近评分, now);
        (void)写入基础节点I64(宿主, 特征_首选观察候选范围大小评分(), 首选.范围大小评分, now);
    }

    inline void 写入双目相机观察存在假设摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        const 结构_空间候选独立性判断结果& 判断,
        const 结构_空间候选观察队列结果& 队列,
        const 结构_观察存在假设结果& 假设,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入双目相机空间候选观察队列摘要(宿主, 结果, 判断, 队列, now);
        (void)写入基础节点I64(宿主, 特征_观察存在假设(), 假设.假设状态, now);
        if (假设.假设状态 != 观察存在假设状态_已生成) {
            return;
        }
        (void)写入基础节点I64(宿主, 特征_来源空间候选编号(), 假设.来源候选编号, now);
        (void)写入基础节点I64(宿主, 特征_存在假设验证状态(), 假设.验证状态, now);
        (void)写入基础节点I64(宿主, 特征_候选像素掩码状态(), 假设.候选像素掩码状态, now);
        (void)写入基础节点I64(宿主, 特征_投影轮廓状态(), 假设.投影轮廓状态, now);
        (void)写入基础节点I64(宿主, 特征_假设距离(), 假设.距离, now);
        (void)写入基础节点I64(宿主, 特征_假设尺寸X(), 假设.尺寸X, now);
        (void)写入基础节点I64(宿主, 特征_假设尺寸Y(), 假设.尺寸Y, now);
        (void)写入基础节点I64(宿主, 特征_假设尺寸Z(), 假设.尺寸Z, now);
        (void)写入基础节点I64(宿主, 特征_假设方位X(), 假设.方位X, now);
        (void)写入基础节点I64(宿主, 特征_假设方位Y(), 假设.方位Y, now);
        (void)写入基础节点I64(宿主, 特征_假设有效点比例(), 假设.有效点比例, now);
        (void)写入基础节点I64(宿主, 特征_轮廓支持评分(), 假设.轮廓支持评分, now);
        (void)写入基础节点I64(宿主, 特征_中心空间坐标X(), 假设.候选.中心X, now);
        (void)写入基础节点I64(宿主, 特征_中心空间坐标Y(), 假设.候选.中心Y, now);
        (void)写入基础节点I64(宿主, 特征_中心空间坐标Z(), 假设.候选.中心Z, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最小X(), 假设.候选.范围最小X, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最大X(), 假设.候选.范围最大X, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最小Y(), 假设.候选.范围最小Y, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最大Y(), 假设.候选.范围最大Y, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最小Z(), 假设.候选.范围最小Z, now);
        (void)写入基础节点I64(宿主, 特征_范围坐标AABB最大Z(), 假设.候选.范围最大Z, now);
        (void)写入基础节点I64(宿主, 特征_空间连续性评分(), 假设.候选.空间连续性评分, now);
        (void)写入基础节点I64(宿主, 特征_范围稳定性评分(), 假设.候选.范围稳定性评分, now);
    }

    inline void 写入双目相机像素归属验证摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        const 结构_空间候选独立性判断结果& 判断,
        const 结构_空间候选观察队列结果& 队列,
        const 结构_观察存在假设结果& 假设,
        const 结构_像素归属验证结果& 验证,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入双目相机观察存在假设摘要(宿主, 结果, 判断, 队列, 假设, now);
        (void)写入基础节点I64(宿主, 特征_像素归属验证结果(), 验证.结果状态, now);
        if (验证.结果状态 != 像素归属验证状态_已生成待确认) {
            return;
        }
        (void)写入基础节点I64(宿主, 特征_来源空间候选编号(), 验证.来源候选编号, now);
        (void)写入基础节点I64(宿主, 特征_空间覆盖像素数(), 验证.空间覆盖像素数, now);
        (void)写入基础节点I64(宿主, 特征_空间匹配像素数(), 验证.空间匹配像素数, now);
        (void)写入基础节点I64(宿主, 特征_空间冲突像素数(), 验证.空间冲突像素数, now);
        (void)写入基础节点I64(宿主, 特征_轮廓支持像素数(), 验证.轮廓支持像素数, now);
        (void)写入基础节点I64(宿主, 特征_轮廓冲突像素数(), 验证.轮廓冲突像素数, now);
        (void)写入基础节点I64(宿主, 特征_深度一致率(), 验证.深度一致率, now);
        (void)写入基础节点I64(宿主, 特征_范围内点比例(), 验证.范围内点比例, now);
        (void)写入基础节点I64(宿主, 特征_投影覆盖率(), 验证.投影覆盖率, now);
        (void)写入基础节点I64(宿主, 特征_轮廓吻合率(), 验证.轮廓吻合率, now);
        (void)写入基础节点I64(宿主, 特征_遮挡像素数(), 验证.遮挡像素数, now);
        (void)写入基础节点I64(宿主, 特征_未解释像素数(), 验证.未解释像素数, now);
        (void)写入基础节点I64(宿主, 特征_像素归属验证状态(), 验证.验证状态, now);
    }

    inline void 写入双目相机观察存在确认摘要(
        基础信息节点类* 宿主,
        const 结构_双目相机调用结果& 结果,
        const 结构_空间候选独立性判断结果& 判断,
        const 结构_空间候选观察队列结果& 队列,
        const 结构_观察存在假设结果& 假设,
        const 结构_像素归属验证结果& 验证,
        const 结构_观察存在确认结果& 确认,
        const 结构_全帧像素归属账本结果& 归属账本,
        时间戳 now) noexcept
    {
        if (!宿主 || !结果.成功) return;
        写入双目相机像素归属验证摘要(宿主, 结果, 判断, 队列, 假设, 验证, now);
        (void)写入基础节点I64(宿主, 特征_已验证观察存在(), 确认.已验证观察存在数量 > 0 ? 1 : 0, now);
        (void)写入基础节点I64(宿主, 特征_观察存在确认状态(), 确认.确认状态, now);
        (void)写入基础节点I64(宿主, 特征_已验证观察存在数量(), 确认.已验证观察存在数量, now);
        (void)写入基础节点I64(宿主, 特征_像素归属更新(), 确认.像素归属更新, now);
        (void)写入基础节点I64(宿主, 特征_未解释区域更新(), 确认.未解释区域更新, now);
        (void)写入基础节点I64(宿主, 特征_帧解释状态(), 确认.帧解释状态, now);
        写入全帧像素归属账本摘要(宿主, 归属账本, now);
    }

    struct 结构_内部世界上下文 {
        存在节点类* 目标存在 = nullptr;
        场景节点类* 外部场景 = nullptr;
        场景节点类* 内部世界 = nullptr;
        存在节点类* 内外接口 = nullptr;
        存在节点类* 局部切片 = nullptr;
        存在节点类* 子候选 = nullptr;
        存在节点类* 子存在 = nullptr;
        I64 外设观察报告ID = 0;
        I64 外设观察像素簇ID = 0;
        I64 来源候选编号 = 0;
        I64 最小X = 0;
        I64 最大X = 0;
        I64 最小Y = 0;
        I64 最大Y = 0;
        I64 最小Z = 0;
        I64 最大Z = 0;
        I64 投影最小X = 0;
        I64 投影最大X = 0;
        I64 投影最小Y = 0;
        I64 投影最大Y = 0;
        I64 宽度 = 0;
        I64 高度 = 0;
        I64 深度 = 0;
        I64 投影宽度 = 0;
        I64 投影高度 = 0;
        I64 平均深度 = 0;
        I64 像素数量 = 0;
        I64 局部轮廓材料可回查状态 = 内部世界局部轮廓材料可回查状态_不可回查;
        std::string ROI句柄{};
        std::string 掩码句柄{};
        std::string 点集句柄{};
        std::string 彩色局部图句柄{};
        std::string 深度局部图句柄{};
    };

    inline bool 存在类型是(存在节点类* 节点, const 语素入口节点类* 类型) noexcept
    {
        const auto* 主信息 = 世界树.存在().取存在主信息(节点);
        return 主信息 && 主信息->类型 == 类型;
    }

    inline 存在节点类* 查找子存在_按类型并按特征I64(
        基础信息节点类* 父节点,
        const 语素入口节点类* 类型,
        const 语素入口节点类* 特征,
        I64 值) noexcept
    {
        if (!父节点 || !类型 || !特征) return nullptr;
        for (auto* 子存在 : 世界树.存在().获取子存在(父节点)) {
            if (!存在类型是(子存在, 类型)) {
                continue;
            }
            I64 当前值 = 0;
            if (读取基础节点I64(reinterpret_cast<基础信息节点类*>(子存在), 特征, 当前值)
                && 当前值 == 值) {
                return 子存在;
            }
        }
        return nullptr;
    }

    inline bool 已确认观察存在节点(存在节点类* 节点) noexcept
    {
        if (!节点) return false;
        if (存在类型是(节点, 类型_已确认观察存在())) return true;
        I64 确认状态 = 0;
        return 读取基础节点I64(
            reinterpret_cast<基础信息节点类*>(节点),
            特征_观察存在确认状态(),
            确认状态)
            && 确认状态 == 观察存在确认状态_已验证;
    }

    inline 场景节点类* 取存在所在场景(存在节点类* 节点) noexcept
    {
        auto* 当前 = 节点 ? static_cast<基础信息节点类*>(节点->父) : nullptr;
        while (当前) {
            if (dynamic_cast<场景节点主信息类*>(当前->主信息)) {
                return reinterpret_cast<场景节点类*>(当前);
            }
            当前 = static_cast<基础信息节点类*>(当前->父);
        }
        return 世界树.取或创建自我现实场景();
    }

    inline 存在节点类* 查找首个已确认观察存在(基础信息节点类* 宿主) noexcept
    {
        if (!宿主) return nullptr;
        if (auto* 自身 = dynamic_cast<存在节点主信息类*>(宿主->主信息);
            自身 && 已确认观察存在节点(reinterpret_cast<存在节点类*>(宿主))) {
            return reinterpret_cast<存在节点类*>(宿主);
        }
        for (auto* 子存在 : 世界树.存在().获取子存在(宿主)) {
            if (已确认观察存在节点(子存在)) {
                return 子存在;
            }
        }
        return nullptr;
    }

    inline void 追加已确认观察存在(
        std::vector<存在节点类*>& 结果,
        存在节点类* 候选) noexcept
    {
        if (!候选 || !已确认观察存在节点(候选)) return;
        if (std::find(结果.begin(), 结果.end(), 候选) == 结果.end()) {
            结果.push_back(候选);
        }
    }

    inline void 收集已确认观察存在(
        基础信息节点类* 宿主,
        std::vector<存在节点类*>& 结果) noexcept
    {
        if (!宿主) return;
        if (auto* 自身 = dynamic_cast<存在节点主信息类*>(宿主->主信息)) {
            追加已确认观察存在(结果, reinterpret_cast<存在节点类*>(宿主));
        }
        for (auto* 子存在 : 世界树.存在().获取子存在(宿主)) {
            追加已确认观察存在(结果, 子存在);
        }
    }

    inline std::vector<存在节点类*> 读取当前场景已确认存在集合(
        基础信息节点类* 目标宿主,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        std::vector<存在节点类*> 结果{};
        收集已确认观察存在(目标宿主, 结果);
        收集已确认观察存在(reinterpret_cast<基础信息节点类*>(输出结果场景), 结果);
        收集已确认观察存在(reinterpret_cast<基础信息节点类*>(输入参数场景), 结果);
        return 结果;
    }

    inline bool D455观察材料句柄可回查(const std::string& 句柄) noexcept
    {
        return !句柄.empty() && 解析D455观察材料句柄(句柄).成功;
    }

    inline I64 评估当前场景特征值材料门项状态(
        I64 可读掩码,
        I64 缺失掩码,
        I64 过期掩码,
        I64 冲突掩码) noexcept
    {
        if (冲突掩码 != 当前场景特征值冲突原因_无) {
            return 当前场景特征值可读状态_冲突;
        }
        if (过期掩码 != 当前场景特征值过期原因_无) {
            return 当前场景特征值可读状态_过期;
        }
        if (缺失掩码 == 当前场景特征值缺失原因_无
            && 过期掩码 == 当前场景特征值过期原因_无
            && 冲突掩码 == 当前场景特征值冲突原因_无) {
            return 当前场景特征值可读状态_可读;
        }
        return 可读掩码 != 0
            ? 当前场景特征值可读状态_部分可读
            : 当前场景特征值可读状态_证据不足;
    }

    inline 结构_当前场景特征值材料门结果 评估当前场景特征值材料门(
        const std::vector<存在节点类*>& 当前场景存在集合,
        时间戳 now) noexcept
    {
        结构_当前场景特征值材料门结果 结果{};
        结果.当前场景存在数量 = static_cast<I64>(当前场景存在集合.size());
        结果.项集合.reserve(当前场景存在集合.size());
        const I64 当前时间毫秒 = static_cast<I64>(now) / 1000;
        const I64 最大允许报告年龄毫秒 = 最大允许当前观察年龄微秒 / 1000;

        I64 序号 = 0;
        for (auto* 存在 : 当前场景存在集合) {
            ++序号;
            结构_当前场景特征值材料门项 项{};
            项.已确认观察存在 = 存在;
            项.序号 = 序号;
            auto* 节点 = reinterpret_cast<基础信息节点类*>(存在);
            if (!节点 || !已确认观察存在节点(存在)) {
                项.缺失原因掩码 |= 当前场景特征值缺失原因_缺已确认观察存在引用;
            } else {
                项.可读特征类型掩码 |= 当前场景特征值可读掩码_已确认观察存在引用;
            }

            (void)读取基础节点I64(节点, 特征_观察存在与当前场景关联状态(), 项.当前场景在场关系状态);
            if (项.当前场景在场关系状态 > 0) {
                项.可读特征类型掩码 |= 当前场景特征值可读掩码_当前场景在场关系;
            } else {
                项.缺失原因掩码 |= 当前场景特征值缺失原因_缺当前场景在场关系;
            }

            (void)读取基础节点I64(节点, 特征_外设观察报告ID(), 项.来源报告ID);
            (void)读取基础节点I64(节点, 特征_外设观察像素簇ID(), 项.来源像素簇ID);
            std::optional<结构_外设观察报告队列项> 报告可选{};
            if (项.来源报告ID > 0) {
                报告可选 = 读取外设观察报告_按ID(static_cast<std::uint64_t>(项.来源报告ID));
            }
            if (!报告可选.has_value()) {
                if (项.来源报告ID > 0) {
                    项.过期原因掩码 |= 当前场景特征值过期原因_来源报告过期;
                } else {
                    项.缺失原因掩码 |= 当前场景特征值缺失原因_缺来源报告外设时间;
                }
            }

            const 结构_外设观察像素簇摘要* 匹配簇 = nullptr;
            if (报告可选.has_value()) {
                const auto& 报告 = *报告可选;
                项.来源报告时间戳毫秒 = 报告.时间戳毫秒;
                if (!报告.来源外设.empty() && 报告.时间戳毫秒 > 0) {
                    项.可读特征类型掩码 |= 当前场景特征值可读掩码_来源报告外设时间;
                } else {
                    项.缺失原因掩码 |= 当前场景特征值缺失原因_缺来源报告外设时间;
                }
                if (报告.时间戳毫秒 > 0
                    && 当前时间毫秒 > 报告.时间戳毫秒
                    && 最大允许报告年龄毫秒 > 0
                    && 当前时间毫秒 - 报告.时间戳毫秒 > 最大允许报告年龄毫秒) {
                    项.过期原因掩码 |= 当前场景特征值过期原因_来源报告过期;
                }

                I64 来源候选编号 = 0;
                (void)读取基础节点I64(节点, 特征_来源空间候选编号(), 来源候选编号);
                for (const auto& 簇 : 报告.观察像素簇集合) {
                    const bool 簇ID匹配 = 项.来源像素簇ID > 0 && 簇.外设内部簇ID == 项.来源像素簇ID;
                    const bool 空间候选匹配 = 来源候选编号 >= 0 && 簇.来源空间候选ID == 来源候选编号;
                    if (簇ID匹配 || 空间候选匹配) {
                        匹配簇 = &簇;
                        break;
                    }
                }
                if (!匹配簇 && (项.来源像素簇ID > 0 || 来源候选编号 >= 0)) {
                    项.冲突原因掩码 |= 当前场景特征值冲突原因_来源簇不匹配;
                }
            }

            if (匹配簇) {
                const std::string* 句柄集[] = {
                    &匹配簇->ROI引用,
                    &匹配簇->像素集合掩码句柄,
                    &匹配簇->点集引用,
                    &匹配簇->像素归属验证材料引用,
                    &匹配簇->空间候选引用,
                    &匹配簇->彩色轮廓局部图句柄,
                    &匹配簇->深度轮廓局部图句柄,
                };
                for (const auto* 句柄 : 句柄集) {
                    if (句柄 && D455观察材料句柄可回查(*句柄)) {
                        ++项.可回查材料数量;
                    }
                }
            }
            if (项.可回查材料数量 > 0) {
                项.可读特征类型掩码 |= 当前场景特征值可读掩码_观察材料可回查;
            } else {
                项.缺失原因掩码 |= 当前场景特征值缺失原因_缺可回查观察材料;
            }

            I64 中心X = 0;
            I64 中心Y = 0;
            I64 中心Z = 0;
            const bool 来源中心可读 =
                读取基础节点I64(节点, 特征_中心空间坐标X(), 中心X)
                && 读取基础节点I64(节点, 特征_中心空间坐标Y(), 中心Y)
                && 读取基础节点I64(节点, 特征_中心空间坐标Z(), 中心Z);
            I64 场景绝对X = 0;
            I64 场景绝对Y = 0;
            I64 场景绝对Z = 0;
            const bool 场景绝对中心可读 =
                读取基础节点I64(节点, 特征_存在_场景绝对坐标X(), 场景绝对X)
                && 读取基础节点I64(节点, 特征_存在_场景绝对坐标Y(), 场景绝对Y)
                && 读取基础节点I64(节点, 特征_存在_场景绝对坐标Z(), 场景绝对Z);
            const bool 中心可读 = 来源中心可读 || 场景绝对中心可读;
            I64 最小X = 0;
            I64 最大X = 0;
            I64 最小Y = 0;
            I64 最大Y = 0;
            I64 最小Z = 0;
            I64 最大Z = 0;
            const bool 范围字段可读 =
                读取基础节点I64(节点, 特征_范围坐标AABB最小X(), 最小X)
                && 读取基础节点I64(节点, 特征_范围坐标AABB最大X(), 最大X)
                && 读取基础节点I64(节点, 特征_范围坐标AABB最小Y(), 最小Y)
                && 读取基础节点I64(节点, 特征_范围坐标AABB最大Y(), 最大Y)
                && 读取基础节点I64(节点, 特征_范围坐标AABB最小Z(), 最小Z)
                && 读取基础节点I64(节点, 特征_范围坐标AABB最大Z(), 最大Z);
            const bool 范围方向有效 = !范围字段可读
                || (最大X >= 最小X && 最大Y >= 最小Y && 最大Z >= 最小Z);
            if (!范围方向有效) {
                项.冲突原因掩码 |= 当前场景特征值冲突原因_空间范围反向;
            }
            项.位置或空间范围可读状态 = (中心可读 || (范围字段可读 && 范围方向有效)) ? 1 : 0;
            if (项.位置或空间范围可读状态 > 0) {
                项.可读特征类型掩码 |= 当前场景特征值可读掩码_来源空间材料;
            } else {
                项.缺失原因掩码 |= 当前场景特征值缺失原因_缺位置或空间范围;
            }

            I64 深度一致率 = 0;
            I64 范围内点比例 = 0;
            const bool 节点质量可读 =
                (读取基础节点I64(节点, 特征_深度一致率(), 深度一致率) && 深度一致率 > 0)
                || (读取基础节点I64(节点, 特征_范围内点比例(), 范围内点比例) && 范围内点比例 > 0);
            const bool 簇质量可读 = 匹配簇
                && (匹配簇->深度有效率 > 0 || 匹配簇->空间坐标有效率 > 0);
            项.深度空间质量可读状态 = (节点质量可读 || 簇质量可读) ? 1 : 0;
            if (项.深度空间质量可读状态 > 0) {
                项.可读特征类型掩码 |= 当前场景特征值可读掩码_深度空间质量;
            } else {
                项.缺失原因掩码 |= 当前场景特征值缺失原因_缺深度空间质量;
            }

            I64 空间连续性评分 = 0;
            I64 范围稳定性评分 = 0;
            const bool 节点稳定可读 =
                (读取基础节点I64(节点, 特征_空间连续性评分(), 空间连续性评分) && 空间连续性评分 > 0)
                || (读取基础节点I64(节点, 特征_范围稳定性评分(), 范围稳定性评分) && 范围稳定性评分 > 0);
            const bool 簇稳定可读 = 匹配簇
                && (匹配簇->跨帧连续命中次数 > 0
                    || 匹配簇->跨帧匹配状态 > 0
                    || 匹配簇->空间连续性评分 > 0
                    || 匹配簇->范围稳定性评分 > 0);
            项.跨帧稳定性可读状态 = (节点稳定可读 || 簇稳定可读) ? 1 : 0;
            if (项.跨帧稳定性可读状态 > 0) {
                项.可读特征类型掩码 |= 当前场景特征值可读掩码_跨帧稳定性;
            } else {
                项.缺失原因掩码 |= 当前场景特征值缺失原因_缺跨帧稳定性;
            }

            (void)读取基础节点I64(节点, 特征_存在_场景绝对坐标明确状态(), 项.场景绝对坐标明确状态);
            (void)读取基础节点I64(节点, 特征_存在_场景绝对空间范围明确状态(), 项.场景绝对空间范围明确状态);
            if (项.场景绝对坐标明确状态 > 0 || 项.场景绝对空间范围明确状态 > 0) {
                项.可读特征类型掩码 |= 当前场景特征值可读掩码_场景绝对坐标;
            } else {
                项.缺失原因掩码 |= 当前场景特征值缺失原因_缺场景绝对坐标;
            }

            项.状态 = 评估当前场景特征值材料门项状态(
                项.可读特征类型掩码,
                项.缺失原因掩码,
                项.过期原因掩码,
                项.冲突原因掩码);

            结果.可读特征类型掩码 |= 项.可读特征类型掩码;
            结果.缺失原因掩码 |= 项.缺失原因掩码;
            结果.过期原因掩码 |= 项.过期原因掩码;
            结果.冲突原因掩码 |= 项.冲突原因掩码;
            if (项.状态 == 当前场景特征值可读状态_可读) {
                ++结果.可读存在数量;
            } else if (项.状态 == 当前场景特征值可读状态_部分可读) {
                ++结果.部分可读存在数量;
            } else {
                ++结果.证据不足存在数量;
            }
            if (项.缺失原因掩码 != 当前场景特征值缺失原因_无) ++结果.材料缺失存在数量;
            if (项.过期原因掩码 != 当前场景特征值过期原因_无) ++结果.材料过期存在数量;
            if (项.冲突原因掩码 != 当前场景特征值冲突原因_无) ++结果.材料冲突存在数量;
            结果.项集合.push_back(项);
        }

        if (结果.当前场景存在数量 <= 0) {
            结果.状态 = 当前场景特征值可读状态_证据不足;
        } else if (结果.材料冲突存在数量 > 0) {
            结果.状态 = 当前场景特征值可读状态_冲突;
        } else if (结果.材料过期存在数量 > 0) {
            结果.状态 = 当前场景特征值可读状态_过期;
        } else if (结果.可读存在数量 == 结果.当前场景存在数量) {
            结果.状态 = 当前场景特征值可读状态_可读;
        } else if (结果.可读存在数量 + 结果.部分可读存在数量 > 0) {
            结果.状态 = 当前场景特征值可读状态_部分可读;
        } else {
            结果.状态 = 当前场景特征值可读状态_证据不足;
        }
        return 结果;
    }

    inline void 追加安全因果候选条目(
        结构_安全因果候选构造结果& 结果,
        存在节点类* 候选存在,
        const 语素入口节点类* 特征类型,
        I64 当前特征值,
        I64 时间窗口,
        I64 来源观察报告) noexcept
    {
        if (!候选存在 || !特征类型) return;
        结构_安全因果候选条目 条目{};
        条目.序号 = static_cast<I64>(结果.条目集合.size()) + 1;
        条目.候选存在 = 候选存在;
        条目.特征类型 = 特征类型;
        条目.当前特征值读取状态 = 安全因果候选当前特征值状态_可读;
        条目.当前特征值 = 当前特征值;
        条目.时间窗口 = 时间窗口;
        条目.来源观察报告 = 来源观察报告;
        结果.条目集合.push_back(条目);
    }

    inline void 尝试追加安全因果候选_正式I64特征(
        结构_安全因果候选构造结果& 结果,
        存在节点类* 候选存在,
        const 语素入口节点类* 特征类型,
        I64 时间窗口,
        I64 来源观察报告) noexcept
    {
        auto* 节点 = reinterpret_cast<基础信息节点类*>(候选存在);
        I64 当前值 = 0;
        if (节点 && 读取基础节点I64(节点, 特征类型, 当前值)) {
            追加安全因果候选条目(
                结果,
                候选存在,
                特征类型,
                当前值,
                时间窗口,
                来源观察报告);
            return;
        }

        ++结果.缺当前特征值数量;
        if (!结果.首个缺失特征) {
            结果.首个缺失特征 = 特征类型;
        }
    }

    inline 结构_安全因果候选构造结果 构造安全因果候选条目_由存在特征值(
        const 结构_当前场景特征值材料门结果& 材料门,
        I64 时间窗口) noexcept
    {
        结构_安全因果候选构造结果 结果{};
        for (const auto& 项 : 材料门.项集合) {
            if (!项.已确认观察存在) continue;
            尝试追加安全因果候选_正式I64特征(
                结果,
                项.已确认观察存在,
                特征_存在_场景绝对坐标X(),
                时间窗口,
                项.来源报告ID);
            尝试追加安全因果候选_正式I64特征(
                结果,
                项.已确认观察存在,
                特征_存在_场景绝对坐标Y(),
                时间窗口,
                项.来源报告ID);
            尝试追加安全因果候选_正式I64特征(
                结果,
                项.已确认观察存在,
                特征_存在_场景绝对坐标Z(),
                时间窗口,
                项.来源报告ID);
        }

        结果.候选数量 = static_cast<I64>(结果.条目集合.size());
        结果.搜索未完成数量 = 结果.候选数量;
        结果.低置信数量 = 结果.候选数量;
        结果.归因置信度 = 0;
        结果.构造状态 =
            结果.候选数量 > 0
            && 结果.缺当前特征值数量 == 0
            && 结果.搜索未完成数量 == 0
            && 结果.低置信数量 == 0
                ? 安全明确状态_已明确
                : 安全明确状态_未明确;
        return 结果;
    }

    inline void 写入安全因果候选条目明细(
        场景节点类* 场景,
        const 结构_安全因果候选构造结果& 候选结果,
        动态节点类* 来源动态,
        时间戳 now,
        I64& 写入数量) noexcept
    {
        写入数量 = 0;
        auto* 宿主 = reinterpret_cast<基础信息节点类*>(场景);
        if (!宿主) return;

        for (const auto& 条目 : 候选结果.条目集合) {
            auto* 候选存在 = 世界树.取或创建子存在_按类型并按特征I64(
                宿主,
                类型_安全因果候选条目(),
                特征_安全因果候选序号(),
                条目.序号);
            auto* 节点 = reinterpret_cast<基础信息节点类*>(候选存在);
            if (!节点) continue;

            (void)写入基础节点I64(节点, 特征_安全因果候选序号(), 条目.序号, now);
            (void)写入基础节点指针(节点, 特征_安全因果候选存在(), 条目.候选存在, now);
            (void)写入基础节点指针(节点, 特征_安全因果候选特征类型(), 条目.特征类型, now);
            (void)写入基础节点I64(节点, 特征_安全因果候选当前特征值读取状态(), 条目.当前特征值读取状态, now);
            (void)写入基础节点I64(节点, 特征_安全因果候选当前特征值(), 条目.当前特征值, now);
            (void)写入基础节点I64(节点, 特征_安全因果候选前一特征值(), 条目.前一特征值, now);
            (void)写入基础节点I64(节点, 特征_安全因果候选前一特征值缺口状态(), 条目.前一特征值缺口状态, now);
            (void)写入基础节点I64(节点, 特征_安全因果候选变化方向(), 条目.变化方向, now);
            (void)写入基础节点I64(节点, 特征_安全因果候选时间窗口(), 条目.时间窗口, now);
            (void)写入基础节点I64(节点, 特征_安全因果候选来源观察报告(), 条目.来源观察报告, now);
            (void)写入基础节点指针(节点, 特征_来源动作动态(), 来源动态, now);
            (void)写入基础节点指针(节点, 特征_安全因果候选相关自我关键特征(), 特征_关键特征变化观察状态(), now);
            (void)写入基础节点I64(节点, 特征_安全因果候选归因证据状态(), 条目.归因证据状态, now);
            (void)写入基础节点I64(节点, 特征_安全因果候选负向证据状态(), 条目.负向证据状态, now);
            (void)写入基础节点I64(节点, 特征_安全因果候选置信度(), 条目.候选置信度, now);
            ++写入数量;
        }
    }

    inline void 写入当前场景特征值材料门摘要(
        基础信息节点类* 宿主,
        const 结构_当前场景特征值材料门结果& 结果,
        时间戳 now) noexcept
    {
        if (!宿主) return;
        (void)写入基础节点I64(宿主, 特征_当前场景特征值可读材料门版本(), 结果.版本, now);
        (void)写入基础节点I64(宿主, 特征_当前场景各存在特征值可读状态(), 结果.状态, now);
        (void)写入基础节点I64(宿主, 特征_当前场景特征值可读存在数量(), 结果.可读存在数量, now);
        (void)写入基础节点I64(宿主, 特征_当前场景特征值部分可读存在数量(), 结果.部分可读存在数量, now);
        (void)写入基础节点I64(宿主, 特征_当前场景特征值证据不足存在数量(), 结果.证据不足存在数量, now);
        (void)写入基础节点I64(宿主, 特征_当前场景特征值材料缺失存在数量(), 结果.材料缺失存在数量, now);
        (void)写入基础节点I64(宿主, 特征_当前场景特征值材料过期存在数量(), 结果.材料过期存在数量, now);
        (void)写入基础节点I64(宿主, 特征_当前场景特征值材料冲突存在数量(), 结果.材料冲突存在数量, now);
        (void)写入基础节点I64(宿主, 特征_当前场景特征值可读特征类型掩码(), 结果.可读特征类型掩码, now);
        (void)写入基础节点I64(宿主, 特征_当前场景特征值缺失原因掩码(), 结果.缺失原因掩码, now);
        (void)写入基础节点I64(宿主, 特征_当前场景特征值过期原因掩码(), 结果.过期原因掩码, now);
        (void)写入基础节点I64(宿主, 特征_当前场景特征值冲突原因掩码(), 结果.冲突原因掩码, now);
    }

    inline void 写入当前场景特征值材料门明细(
        场景节点类* 输出场景,
        const 结构_当前场景特征值材料门结果& 结果,
        时间戳 now) noexcept
    {
        auto* 输出节点 = reinterpret_cast<基础信息节点类*>(输出场景);
        if (!输出节点) return;
        for (const auto& 项 : 结果.项集合) {
            auto* 明细存在 = 世界树.取或创建子存在_按类型并按特征I64(
                输出节点,
                类型_当前场景特征值可读材料门项(),
                特征_当前场景特征值材料门序号(),
                项.序号);
            auto* 节点 = reinterpret_cast<基础信息节点类*>(明细存在);
            if (!节点) continue;
            (void)写入基础节点I64(节点, 特征_当前场景特征值材料门序号(), 项.序号, now);
            (void)写入基础节点指针(节点, 特征_被评估存在(), 项.已确认观察存在, now);
            (void)写入基础节点I64(节点, 特征_当前场景各存在特征值可读状态(), 项.状态, now);
            (void)写入基础节点I64(节点, 特征_当前场景特征值可读特征类型掩码(), 项.可读特征类型掩码, now);
            (void)写入基础节点I64(节点, 特征_当前场景特征值缺失原因掩码(), 项.缺失原因掩码, now);
            (void)写入基础节点I64(节点, 特征_当前场景特征值过期原因掩码(), 项.过期原因掩码, now);
            (void)写入基础节点I64(节点, 特征_当前场景特征值冲突原因掩码(), 项.冲突原因掩码, now);
            (void)写入基础节点I64(节点, 特征_外设观察报告ID(), 项.来源报告ID, now);
            (void)写入基础节点I64(节点, 特征_外设观察像素簇ID(), 项.来源像素簇ID, now);
            (void)写入基础节点I64(节点, 特征_来源报告时间戳毫秒(), 项.来源报告时间戳毫秒, now);
            (void)写入基础节点I64(节点, 特征_观察存在与当前场景关联状态(), 项.当前场景在场关系状态, now);
            (void)写入基础节点I64(节点, 特征_外设观察材料可回查状态(), 项.可回查材料数量 > 0 ? 1 : 0, now);
            (void)写入基础节点I64(节点, 特征_存在_场景绝对坐标明确状态(), 项.场景绝对坐标明确状态, now);
            (void)写入基础节点I64(节点, 特征_存在_场景绝对空间范围明确状态(), 项.场景绝对空间范围明确状态, now);
        }
    }

    inline 存在节点类* 解析目标观察存在(
        场景节点类* 输入参数场景,
        基础信息节点类* 默认宿主,
        场景节点类* 输出结果场景) noexcept
    {
        void* 指针 = nullptr;
        if (读取输入参数指针(输入参数场景, 特征_目标存在(), 指针)
            || 读取输入参数指针(输入参数场景, 特征_已验证观察存在(), 指针)) {
            auto* 目标 = reinterpret_cast<存在节点类*>(指针);
            if (已确认观察存在节点(目标)) {
                return 目标;
            }
        }
        if (auto* 命中 = 查找首个已确认观察存在(默认宿主)) return 命中;
        if (auto* 命中 = 查找首个已确认观察存在(reinterpret_cast<基础信息节点类*>(输出结果场景))) return 命中;
        return 查找首个已确认观察存在(reinterpret_cast<基础信息节点类*>(输入参数场景));
    }

    inline I64 读取I64或0(基础信息节点类* 节点, const 语素入口节点类* 特征类型) noexcept
    {
        I64 值 = 0;
        (void)读取基础节点I64(节点, 特征类型, 值);
        return 值;
    }

    inline I64 正范围(I64 最小值, I64 最大值) noexcept
    {
        return 最大值 >= 最小值 ? (最大值 - 最小值) : 0;
    }

    inline I64 投影尺寸(I64 最小值, I64 最大值, I64 兜底) noexcept
    {
        if (最大值 >= 最小值) {
            return std::max<I64>(1, 最大值 - 最小值 + 1);
        }
        return std::max<I64>(1, 兜底);
    }

    inline 结构_内部世界上下文 读取内部世界上下文(
        存在节点类* 目标存在,
        bool 确保内部世界,
        时间戳 now) noexcept
    {
        结构_内部世界上下文 上下文{};
        上下文.目标存在 = 目标存在;
        auto* 目标节点 = reinterpret_cast<基础信息节点类*>(目标存在);
        if (!目标节点) return 上下文;

        上下文.外部场景 = 取存在所在场景(目标存在);
        上下文.内部世界 = 确保内部世界
            ? 世界树.取或创建存在内部世界(目标存在, 名称_存在内部世界())
            : 世界树.取存在内部世界(目标存在);

        上下文.外设观察报告ID = 读取I64或0(目标节点, 特征_外设观察报告ID());
        上下文.外设观察像素簇ID = 读取I64或0(目标节点, 特征_外设观察像素簇ID());
        上下文.来源候选编号 = 读取I64或0(目标节点, 特征_来源空间候选编号());
        上下文.最小X = 读取I64或0(目标节点, 特征_范围坐标AABB最小X());
        上下文.最大X = 读取I64或0(目标节点, 特征_范围坐标AABB最大X());
        上下文.最小Y = 读取I64或0(目标节点, 特征_范围坐标AABB最小Y());
        上下文.最大Y = 读取I64或0(目标节点, 特征_范围坐标AABB最大Y());
        上下文.最小Z = 读取I64或0(目标节点, 特征_范围坐标AABB最小Z());
        上下文.最大Z = 读取I64或0(目标节点, 特征_范围坐标AABB最大Z());
        上下文.投影最小X = 读取I64或0(目标节点, 特征_投影范围最小X());
        上下文.投影最大X = 读取I64或0(目标节点, 特征_投影范围最大X());
        上下文.投影最小Y = 读取I64或0(目标节点, 特征_投影范围最小Y());
        上下文.投影最大Y = 读取I64或0(目标节点, 特征_投影范围最大Y());
        上下文.宽度 = 正范围(上下文.最小X, 上下文.最大X);
        上下文.高度 = 正范围(上下文.最小Y, 上下文.最大Y);
        上下文.深度 = 正范围(上下文.最小Z, 上下文.最大Z);
        上下文.投影宽度 = 投影尺寸(上下文.投影最小X, 上下文.投影最大X, 上下文.宽度);
        上下文.投影高度 = 投影尺寸(上下文.投影最小Y, 上下文.投影最大Y, 上下文.高度);
        上下文.平均深度 = 读取I64或0(目标节点, 特征_假设距离());
        上下文.像素数量 = 读取I64或0(目标节点, 特征_空间匹配像素数());
        if (上下文.外设观察报告ID > 0) {
            if (auto 报告 = 读取外设观察报告_按ID(static_cast<std::uint64_t>(上下文.外设观察报告ID))) {
                for (const auto& 簇 : 报告->观察像素簇集合) {
                    const bool 簇ID匹配 = 上下文.外设观察像素簇ID > 0
                        && 簇.外设内部簇ID == 上下文.外设观察像素簇ID;
                    const bool 空间候选匹配 = 上下文.来源候选编号 >= 0
                        && 簇.来源空间候选ID == 上下文.来源候选编号;
                    if (!簇ID匹配 && !空间候选匹配) {
                        continue;
                    }

                    上下文.ROI句柄 = 簇.ROI引用;
                    上下文.掩码句柄 = 簇.像素集合掩码句柄;
                    上下文.点集句柄 = 簇.点集引用;
                    上下文.彩色局部图句柄 = 簇.彩色轮廓局部图句柄;
                    上下文.深度局部图句柄 = 簇.深度轮廓局部图句柄;
                    auto 句柄可回查 = [](const std::string& 句柄) noexcept {
                        return !句柄.empty() && 解析D455观察材料句柄(句柄).成功;
                    };
                    上下文.局部轮廓材料可回查状态 =
                        句柄可回查(上下文.ROI句柄)
                        && 句柄可回查(上下文.掩码句柄)
                        && 句柄可回查(上下文.点集句柄)
                            ? 内部世界局部轮廓材料可回查状态_可回查
                            : 内部世界局部轮廓材料可回查状态_不可回查;
                    break;
                }
            }
        }
        if (auto* 内部节点 = reinterpret_cast<基础信息节点类*>(上下文.内部世界)) {
            上下文.内外接口 = 世界树.存在().查找子存在_按类型(内部节点, 类型_存在内外接口());
            上下文.局部切片 = 查找子存在_按类型并按特征I64(
                内部节点,
                类型_存在局部观察切片(),
                特征_来源空间候选编号(),
                上下文.来源候选编号);
            上下文.子候选 = 查找子存在_按类型并按特征I64(
                内部节点,
                类型_内部世界子候选(),
                特征_来源空间候选编号(),
                上下文.来源候选编号);
            const bool 是叶子 = 上下文.投影宽度 <= 8 && 上下文.投影高度 <= 8;
            上下文.子存在 = 查找子存在_按类型并按特征I64(
                内部节点,
                是叶子 ? 类型_叶子特征块存在() : 类型_内部世界子存在(),
                特征_来源空间候选编号(),
                上下文.来源候选编号);
        }
        (void)now;
        return 上下文;
    }

    inline bool 局部区域达到叶子阈值(const 结构_内部世界上下文& 上下文) noexcept
    {
        return 上下文.投影宽度 <= 8 && 上下文.投影高度 <= 8;
    }

    inline void 写入内部世界基础摘要(
        const 结构_内部世界上下文& 上下文,
        时间戳 now) noexcept
    {
        auto* 目标节点 = reinterpret_cast<基础信息节点类*>(上下文.目标存在);
        auto* 内部节点 = reinterpret_cast<基础信息节点类*>(上下文.内部世界);
        if (目标节点 && 上下文.内部世界) {
            (void)写入基础节点指针(目标节点, 特征_内部世界(), 上下文.内部世界, now);
            (void)写入基础节点I64(目标节点, 特征_内部世界状态(), 内部世界状态_已建立, now);
        }
        if (!内部节点) return;
        (void)写入基础节点I64(内部节点, 特征_内部世界场景类型(), 1, now);
        (void)写入基础节点I64(内部节点, 特征_内部世界状态(), 内部世界状态_已建立, now);
        (void)写入基础节点I64(内部节点, 特征_空间坐标单位毫米(), 1, now);
        (void)写入基础节点I64(内部节点, 特征_局部坐标原点X(), 上下文.最小X, now);
        (void)写入基础节点I64(内部节点, 特征_局部坐标原点Y(), 上下文.最小Y, now);
        (void)写入基础节点I64(内部节点, 特征_局部坐标原点Z(), 上下文.最小Z, now);
        (void)写入基础节点I64(内部节点, 特征_范围坐标AABB最小X(), 0, now);
        (void)写入基础节点I64(内部节点, 特征_范围坐标AABB最小Y(), 0, now);
        (void)写入基础节点I64(内部节点, 特征_范围坐标AABB最小Z(), 0, now);
        (void)写入基础节点I64(内部节点, 特征_范围坐标AABB最大X(), 上下文.宽度, now);
        (void)写入基础节点I64(内部节点, 特征_范围坐标AABB最大Y(), 上下文.高度, now);
        (void)写入基础节点I64(内部节点, 特征_范围坐标AABB最大Z(), 上下文.深度, now);
        (void)写入基础节点I64(内部节点, 特征_内部世界细分状态(), 内部世界细分状态_未细分, now);
        (void)写入基础节点I64(内部节点, 特征_可绘制状态(), 1, now);
        (void)写入基础节点指针(内部节点, 特征_所属父存在(), 上下文.目标存在, now);
        if (上下文.外部场景) {
            (void)写入基础节点指针(内部节点, 特征_父场景(), 上下文.外部场景, now);
        }
    }

    inline 存在节点类* 取或创建内外接口(结构_内部世界上下文& 上下文, 时间戳 now) noexcept
    {
        auto* 内部节点 = reinterpret_cast<基础信息节点类*>(上下文.内部世界);
        if (!内部节点) return nullptr;
        auto* 接口 = 世界树.取或创建子存在_按类型(内部节点, 类型_存在内外接口());
        auto* 节点 = reinterpret_cast<基础信息节点类*>(接口);
        if (!节点) return nullptr;
        (void)写入基础节点I64(节点, 特征_接口状态(), 内外接口状态_已建立, now);
        (void)写入基础节点I64(节点, 特征_局部坐标原点X(), 上下文.最小X, now);
        (void)写入基础节点I64(节点, 特征_局部坐标原点Y(), 上下文.最小Y, now);
        (void)写入基础节点I64(节点, 特征_局部坐标原点Z(), 上下文.最小Z, now);
        (void)写入基础节点指针(节点, 特征_所属父存在(), 上下文.目标存在, now);
        (void)写入基础节点指针(节点, 特征_内部场景(), 上下文.内部世界, now);
        if (上下文.外部场景) {
            (void)写入基础节点指针(节点, 特征_外部场景(), 上下文.外部场景, now);
        }
        (void)写入基础节点指针(内部节点, 特征_存在内外场景接口(), 接口, now);
        (void)写入基础节点I64(内部节点, 特征_接口状态(), 内外接口状态_已建立, now);
        上下文.内外接口 = 接口;
        return 接口;
    }

    inline 存在节点类* 取或创建局部观察切片(结构_内部世界上下文& 上下文, 时间戳 now) noexcept
    {
        auto* 内部节点 = reinterpret_cast<基础信息节点类*>(上下文.内部世界);
        if (!内部节点) return nullptr;
        auto* 切片 = 世界树.取或创建子存在_按类型并按特征I64(
            内部节点,
            类型_存在局部观察切片(),
            特征_来源空间候选编号(),
            上下文.来源候选编号);
        auto* 节点 = reinterpret_cast<基础信息节点类*>(切片);
        if (!节点) return nullptr;
        (void)写入基础节点I64(节点, 特征_外设观察报告ID(), 上下文.外设观察报告ID, now);
        (void)写入基础节点I64(节点, 特征_外设观察像素簇ID(), 上下文.外设观察像素簇ID, now);
        (void)写入基础节点I64(节点, 特征_来源空间候选编号(), 上下文.来源候选编号, now);
        (void)写入基础节点I64(节点, 特征_内部世界当前观察材料(), 当前观察材料状态_已建立, now);
        (void)写入基础节点I64(节点, 特征_存在内部世界局部材料建立状态(), 内部世界局部材料建立状态_已建立, now);
        (void)写入基础节点I64(节点, 特征_内部世界局部轮廓材料可回查状态(), 上下文.局部轮廓材料可回查状态, now);
        (void)写入基础节点I64(节点, 特征_局部边界框宽度(), 上下文.投影宽度, now);
        (void)写入基础节点I64(节点, 特征_局部边界框高度(), 上下文.投影高度, now);
        (void)写入基础节点I64(节点, 特征_局部像素索引状态(), 1, now);
        (void)写入基础节点I64(节点, 特征_像素特征数量(), 上下文.像素数量, now);
        (void)写入基础节点I64(节点, 特征_平均深度(), 上下文.平均深度, now);
        (void)写入基础节点指针(节点, 特征_所属父存在(), 上下文.目标存在, now);
        (void)写入基础节点指针(节点, 特征_内部世界(), 上下文.内部世界, now);
        if (上下文.内外接口) {
            (void)写入基础节点指针(节点, 特征_存在内外场景接口(), 上下文.内外接口, now);
        }
        (void)写入基础节点指针(内部节点, 特征_局部观察切片(), 切片, now);
        (void)写入基础节点I64(内部节点, 特征_内部世界当前观察材料(), 当前观察材料状态_已建立, now);
        (void)写入基础节点I64(内部节点, 特征_存在内部世界局部材料建立状态(), 内部世界局部材料建立状态_已建立, now);
        (void)写入基础节点I64(内部节点, 特征_内部世界局部轮廓材料可回查状态(), 上下文.局部轮廓材料可回查状态, now);
        上下文.局部切片 = 切片;
        return 切片;
    }

    inline 存在节点类* 取或创建内部世界子候选(结构_内部世界上下文& 上下文, 时间戳 now) noexcept
    {
        auto* 内部节点 = reinterpret_cast<基础信息节点类*>(上下文.内部世界);
        if (!内部节点) return nullptr;
        auto* 子候选 = 世界树.取或创建子存在_按类型并按特征I64(
            内部节点,
            类型_内部世界子候选(),
            特征_来源空间候选编号(),
            上下文.来源候选编号);
        auto* 节点 = reinterpret_cast<基础信息节点类*>(子候选);
        if (!节点) return nullptr;
        const bool 是叶子 = 局部区域达到叶子阈值(上下文);
        (void)写入基础节点I64(节点, 特征_外设观察报告ID(), 上下文.外设观察报告ID, now);
        (void)写入基础节点I64(节点, 特征_外设观察像素簇ID(), 上下文.外设观察像素簇ID, now);
        (void)写入基础节点I64(节点, 特征_来源空间候选编号(), 上下文.来源候选编号, now);
        (void)写入基础节点I64(节点, 特征_子候选类型(), 是叶子 ? 内部子候选类型_叶子候选 : 内部子候选类型_表面特征候选, now);
        (void)写入基础节点I64(节点, 特征_内部世界子候选验证状态(), 内部世界子候选验证状态_待验证, now);
        (void)写入基础节点I64(节点, 特征_局部边界框宽度(), 上下文.投影宽度, now);
        (void)写入基础节点I64(节点, 特征_局部边界框高度(), 上下文.投影高度, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小X(), 0, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小Y(), 0, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小Z(), 0, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大X(), 上下文.宽度, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大Y(), 上下文.高度, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大Z(), 上下文.深度, now);
        (void)写入基础节点I64(节点, 特征_平均深度(), 上下文.平均深度, now);
        (void)写入基础节点指针(节点, 特征_来源局部观察切片(), 上下文.局部切片, now);
        (void)写入基础节点指针(节点, 特征_内部世界(), 上下文.内部世界, now);
        (void)写入基础节点I64(内部节点, 特征_内部世界子候选集合(), 子候选集合状态_已生成, now);
        (void)写入基础节点I64(内部节点, 特征_内部世界子候选验证状态(), 内部世界子候选验证状态_待验证, now);
        上下文.子候选 = 子候选;
        return 子候选;
    }

    inline 存在节点类* 取或创建内部世界子存在(结构_内部世界上下文& 上下文, 时间戳 now) noexcept
    {
        auto* 内部节点 = reinterpret_cast<基础信息节点类*>(上下文.内部世界);
        if (!内部节点) return nullptr;
        const bool 是叶子 = 局部区域达到叶子阈值(上下文);
        auto* 类型 = 是叶子 ? 类型_叶子特征块存在() : 类型_内部世界子存在();
        auto* 子存在 = 世界树.取或创建子存在_按类型并按特征I64(
            内部节点,
            类型,
            特征_来源空间候选编号(),
            上下文.来源候选编号);
        auto* 节点 = reinterpret_cast<基础信息节点类*>(子存在);
        if (!节点) return nullptr;
        (void)写入基础节点I64(节点, 特征_来源空间候选编号(), 上下文.来源候选编号, now);
        (void)写入基础节点I64(节点, 特征_子存在类型(), 是叶子 ? 内部子存在类型_叶子特征块存在 : 内部子存在类型_表面特征存在, now);
        (void)写入基础节点I64(节点, 特征_局部边界框宽度(), 上下文.投影宽度, now);
        (void)写入基础节点I64(节点, 特征_局部边界框高度(), 上下文.投影高度, now);
        (void)写入基础节点I64(节点, 特征_不再细分(), 是叶子 ? 1 : 0, now);
        (void)写入基础节点I64(节点, 特征_平均深度(), 上下文.平均深度, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小X(), 0, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小Y(), 0, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小Z(), 0, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大X(), 上下文.宽度, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大Y(), 上下文.高度, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大Z(), 上下文.深度, now);
        (void)写入基础节点指针(节点, 特征_所属父存在(), 上下文.目标存在, now);
        (void)写入基础节点指针(节点, 特征_来源局部观察切片(), 上下文.局部切片, now);
        (void)写入基础节点指针(节点, 特征_内部世界(), 上下文.内部世界, now);
        (void)写入基础节点I64(内部节点, 特征_成员存在集合(), 成员存在集合状态_已更新, now);
        上下文.子存在 = 子存在;
        return 子存在;
    }

    struct 结构_内部世界子存在确认方案 {
        I64 确认状态 = 内部世界子存在候选确认状态_未确认;
        I64 方案取得状态 = 0;
        I64 方案类型 = 内部世界子存在确认方案_无;
        I64 允许提交状态 = 0;
        I64 新建数量 = 0;
        I64 证据不足数量 = 0;
        I64 可提交数量 = 0;
        I64 证据不足原因 = 0;
    };

    inline 结构_内部世界子存在确认方案 构造内部世界子存在确认方案(
        const 结构_内部世界上下文& 上下文) noexcept
    {
        结构_内部世界子存在确认方案 方案{};
        if (!上下文.目标存在 || !上下文.内部世界 || !上下文.子候选) {
            方案.确认状态 = 内部世界子存在候选确认状态_证据不足;
            方案.方案类型 = 内部世界子存在确认方案_证据不足;
            方案.证据不足数量 = 1;
            方案.证据不足原因 = 1;
            return 方案;
        }
        if (上下文.局部轮廓材料可回查状态 != 内部世界局部轮廓材料可回查状态_可回查) {
            方案.确认状态 = 内部世界子存在候选确认状态_证据不足;
            方案.方案类型 = 内部世界子存在确认方案_证据不足;
            方案.证据不足数量 = 1;
            方案.证据不足原因 = 2;
            return 方案;
        }

        方案.确认状态 = 内部世界子存在候选确认状态_方案已生成;
        方案.方案取得状态 = 1;
        方案.方案类型 = 上下文.子存在
            ? 内部世界子存在确认方案_合并
            : 内部世界子存在确认方案_新建;
        方案.允许提交状态 = 1;
        方案.新建数量 = 上下文.子存在 ? 0 : 1;
        方案.可提交数量 = 1;
        return 方案;
    }

    inline void 写入内部世界方法结果(
        基础信息节点类* 宿主,
        bool 成功,
        const 结构_内部世界上下文& 上下文,
        动态节点类* 动态,
        const 语素入口节点类* 失败原因,
        时间戳 now,
        bool 写确认方案 = false,
        const 结构_内部世界子存在确认方案* 确认方案 = nullptr) noexcept
    {
        if (!宿主) return;
        (void)写入方法执行情况(宿主, 成功 ? 值_执行成功() : 值_执行失败(), now);
        if (上下文.目标存在) {
            (void)写入基础节点指针(宿主, 特征_目标存在(), 上下文.目标存在, now);
        }
        if (上下文.内部世界) {
            (void)写入基础节点指针(宿主, 特征_内部世界(), 上下文.内部世界, now);
            (void)写入基础节点I64(宿主, 特征_内部世界状态(), 内部世界状态_已建立, now);
        }
        if (上下文.内外接口) {
            (void)写入基础节点指针(宿主, 特征_存在内外场景接口(), 上下文.内外接口, now);
        }
        if (上下文.局部切片) {
            (void)写入基础节点指针(宿主, 特征_局部观察切片(), 上下文.局部切片, now);
            (void)写入基础节点I64(宿主, 特征_存在内部世界局部材料建立状态(), 内部世界局部材料建立状态_已建立, now);
            (void)写入基础节点I64(宿主, 特征_内部世界局部轮廓材料可回查状态(), 上下文.局部轮廓材料可回查状态, now);
        }
        if (上下文.子候选) {
            (void)写入基础节点指针(宿主, 特征_内部世界子候选集合(), 上下文.子候选, now);
            (void)写入基础节点I64(宿主, 特征_内部世界子候选验证状态(), 内部世界子候选验证状态_待验证, now);
        }
        if (写确认方案 && 确认方案) {
            (void)写入基础节点I64(宿主, 特征_内部世界子存在候选确认状态(), 确认方案->确认状态, now);
            (void)写入基础节点I64(宿主, 特征_内部世界子存在候选确认方案取得状态(), 确认方案->方案取得状态, now);
            (void)写入基础节点I64(宿主, 特征_内部世界子存在候选确认方案类型(), 确认方案->方案类型, now);
            (void)写入基础节点I64(宿主, 特征_内部世界子存在候选确认允许提交状态(), 确认方案->允许提交状态, now);
            (void)写入基础节点I64(宿主, 特征_内部世界子存在候选确认新建数量(), 确认方案->新建数量, now);
            (void)写入基础节点I64(宿主, 特征_内部世界子存在候选确认证据不足数量(), 确认方案->证据不足数量, now);
            (void)写入基础节点I64(宿主, 特征_内部世界子存在候选确认可提交数量(), 确认方案->可提交数量, now);
        }
        if (上下文.子存在) {
            (void)写入基础节点指针(宿主, 特征_成员存在集合(), 上下文.子存在, now);
        }
        if (动态) {
            (void)写入基础节点指针(宿主, 特征_动作动态(), 动态, now);
        }
        if (失败原因) {
            (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
        }
    }

    struct 结构_观察存在摘要 {
        存在节点类* 存在 = nullptr;
        I64 来源候选编号 = 0;
        I64 中心X = 0;
        I64 中心Y = 0;
        I64 中心Z = 0;
        I64 最小X = 0;
        I64 最大X = 0;
        I64 最小Y = 0;
        I64 最大Y = 0;
        I64 最小Z = 0;
        I64 最大Z = 0;
        I64 尺寸X = 0;
        I64 尺寸Y = 0;
        I64 尺寸Z = 0;
        I64 距离 = 0;
        bool 有效 = false;
    };

    struct 结构_观察变化上下文 {
        基础信息节点类* 宿主 = nullptr;
        存在节点类* 当前存在 = nullptr;
        存在节点类* 历史存在 = nullptr;
        存在节点类* 匹配关系 = nullptr;
        存在节点类* 变化结果 = nullptr;
        存在节点类* 变化事件 = nullptr;
        I64 历史对应状态 = 历史对应状态_未匹配;
        I64 中心坐标差 = 0;
        I64 范围坐标重叠率 = 0;
        I64 尺寸差 = 0;
        I64 匹配置信度 = 0;
        I64 变化状态 = 变化状态_不确定;
        I64 位置变化量 = 0;
        I64 范围变化量 = 0;
        I64 尺寸变化量 = 0;
        I64 距离变化量 = 0;
        I64 事件类型 = 观察变化事件类型_不确定;
        I64 事件置信度 = 0;
    };

    inline I64 最大3(I64 a, I64 b, I64 c) noexcept
    {
        return std::max(a, std::max(b, c));
    }

    inline 结构_观察存在摘要 读取观察存在摘要(存在节点类* 存在) noexcept
    {
        结构_观察存在摘要 摘要{};
        摘要.存在 = 存在;
        auto* 节点 = reinterpret_cast<基础信息节点类*>(存在);
        if (!节点) return 摘要;
        摘要.来源候选编号 = 读取I64或0(节点, 特征_来源空间候选编号());
        摘要.中心X = 读取I64或0(节点, 特征_中心空间坐标X());
        摘要.中心Y = 读取I64或0(节点, 特征_中心空间坐标Y());
        摘要.中心Z = 读取I64或0(节点, 特征_中心空间坐标Z());
        摘要.最小X = 读取I64或0(节点, 特征_范围坐标AABB最小X());
        摘要.最大X = 读取I64或0(节点, 特征_范围坐标AABB最大X());
        摘要.最小Y = 读取I64或0(节点, 特征_范围坐标AABB最小Y());
        摘要.最大Y = 读取I64或0(节点, 特征_范围坐标AABB最大Y());
        摘要.最小Z = 读取I64或0(节点, 特征_范围坐标AABB最小Z());
        摘要.最大Z = 读取I64或0(节点, 特征_范围坐标AABB最大Z());
        摘要.尺寸X = 正范围(摘要.最小X, 摘要.最大X);
        摘要.尺寸Y = 正范围(摘要.最小Y, 摘要.最大Y);
        摘要.尺寸Z = 正范围(摘要.最小Z, 摘要.最大Z);
        摘要.距离 = 读取I64或0(节点, 特征_假设距离());
        摘要.有效 = 已确认观察存在节点(存在);
        return 摘要;
    }

    inline 存在节点类* 解析当前观察存在(
        场景节点类* 输入参数场景,
        基础信息节点类* 默认宿主,
        场景节点类* 输出结果场景) noexcept
    {
        void* 指针 = nullptr;
        if (读取输入参数指针(输入参数场景, 特征_当前观察存在(), 指针)
            || 读取输入参数指针(输入参数场景, 特征_目标存在(), 指针)
            || 读取输入参数指针(输入参数场景, 特征_已验证观察存在(), 指针)) {
            auto* 目标 = reinterpret_cast<存在节点类*>(指针);
            if (已确认观察存在节点(目标)) {
                return 目标;
            }
        }
        return 解析目标观察存在(输入参数场景, 默认宿主, 输出结果场景);
    }

    inline 存在节点类* 解析历史观察存在(
        场景节点类* 输入参数场景,
        基础信息节点类* 默认宿主,
        场景节点类* 输出结果场景,
        存在节点类* 当前存在) noexcept
    {
        void* 指针 = nullptr;
        if (读取输入参数指针(输入参数场景, 特征_历史观察存在(), 指针)
            || 读取输入参数指针(输入参数场景, 特征_上一观察存在(), 指针)) {
            auto* 历史 = reinterpret_cast<存在节点类*>(指针);
            if (历史 && 历史 != 当前存在 && 已确认观察存在节点(历史)) {
                return 历史;
            }
        }
        if (读取基础节点指针(默认宿主, 特征_历史观察存在(), 指针)
            || 读取基础节点指针(默认宿主, 特征_上一观察存在(), 指针)
            || 读取基础节点指针(reinterpret_cast<基础信息节点类*>(输出结果场景), 特征_历史观察存在(), 指针)
            || 读取基础节点指针(reinterpret_cast<基础信息节点类*>(输出结果场景), 特征_上一观察存在(), 指针)) {
            auto* 历史 = reinterpret_cast<存在节点类*>(指针);
            if (历史 && 历史 != 当前存在 && 已确认观察存在节点(历史)) {
                return 历史;
            }
        }
        return nullptr;
    }

    inline I64 范围轴重叠率(I64 aMin, I64 aMax, I64 bMin, I64 bMax) noexcept
    {
        const I64 aLen = std::max<I64>(1, 正范围(aMin, aMax));
        const I64 bLen = std::max<I64>(1, 正范围(bMin, bMax));
        const I64 重叠 = std::max<I64>(0, std::min(aMax, bMax) - std::max(aMin, bMin));
        return std::min<I64>(10000, 重叠 * 10000 / std::max(aLen, bLen));
    }

    inline I64 计算范围坐标重叠率(
        const 结构_观察存在摘要& 当前,
        const 结构_观察存在摘要& 历史) noexcept
    {
        const I64 x = 范围轴重叠率(当前.最小X, 当前.最大X, 历史.最小X, 历史.最大X);
        const I64 y = 范围轴重叠率(当前.最小Y, 当前.最大Y, 历史.最小Y, 历史.最大Y);
        const I64 z = 范围轴重叠率(当前.最小Z, 当前.最大Z, 历史.最小Z, 历史.最大Z);
        return (x + y + z) / 3;
    }

    inline I64 计算存在中心坐标差(
        const 结构_观察存在摘要& 当前,
        const 结构_观察存在摘要& 历史) noexcept
    {
        return 最大3(
            绝对差I64(当前.中心X, 历史.中心X),
            绝对差I64(当前.中心Y, 历史.中心Y),
            绝对差I64(当前.中心Z, 历史.中心Z));
    }

    inline I64 计算存在尺寸差(
        const 结构_观察存在摘要& 当前,
        const 结构_观察存在摘要& 历史) noexcept
    {
        return 最大3(
            绝对差I64(当前.尺寸X, 历史.尺寸X),
            绝对差I64(当前.尺寸Y, 历史.尺寸Y),
            绝对差I64(当前.尺寸Z, 历史.尺寸Z));
    }

    inline I64 计算匹配置信度(
        const 结构_观察存在摘要& 当前,
        const 结构_观察存在摘要& 历史,
        I64 中心差,
        I64 重叠率,
        I64 尺寸差) noexcept
    {
        const I64 当前尺度 = std::max<I64>(1, 当前.尺寸X + 当前.尺寸Y + 当前.尺寸Z);
        const I64 历史尺度 = std::max<I64>(1, 历史.尺寸X + 历史.尺寸Y + 历史.尺寸Z);
        const I64 尺度 = std::max<I64>(300, (当前尺度 + 历史尺度) / 6);
        const I64 中心扣分 = std::min<I64>(10000, 中心差 * 10000 / 尺度);
        const I64 尺寸扣分 = std::min<I64>(5000, 尺寸差 * 5000 / 尺度);
        const I64 置信 = (重叠率 * 2 + (10000 - 中心扣分)) / 3 - 尺寸扣分 / 4;
        return std::clamp<I64>(置信, 0, 10000);
    }

    inline 存在节点类* 取或创建观察存在匹配关系(
        基础信息节点类* 宿主,
        I64 关系键) noexcept
    {
        return 宿主
            ? 世界树.取或创建子存在_按类型并按特征I64(
                宿主,
                类型_观察存在匹配关系(),
                特征_来源空间候选编号(),
                关系键)
            : nullptr;
    }

    inline 存在节点类* 取或创建观察存在变化结果(
        基础信息节点类* 宿主,
        I64 关系键) noexcept
    {
        return 宿主
            ? 世界树.取或创建子存在_按类型并按特征I64(
                宿主,
                类型_观察存在变化结果(),
                特征_来源空间候选编号(),
                关系键)
            : nullptr;
    }

    inline 存在节点类* 取或创建观察变化事件(
        基础信息节点类* 宿主,
        I64 关系键) noexcept
    {
        return 宿主
            ? 世界树.取或创建子存在_按类型并按特征I64(
                宿主,
                类型_观察变化事件(),
                特征_来源空间候选编号(),
                关系键)
            : nullptr;
    }

    inline I64 观察变化关系键(
        const 结构_观察存在摘要& 当前,
        const 结构_观察存在摘要& 历史) noexcept
    {
        if (当前.存在) return 当前.来源候选编号;
        if (历史.存在) return -100000 - 历史.来源候选编号;
        return 0;
    }

    inline void 计算观察存在匹配关系(结构_观察变化上下文& 上下文, 时间戳 now) noexcept
    {
        const auto 当前 = 读取观察存在摘要(上下文.当前存在);
        const auto 历史 = 读取观察存在摘要(上下文.历史存在);
        const I64 关系键 = 观察变化关系键(当前, 历史);
        上下文.匹配关系 = 取或创建观察存在匹配关系(上下文.宿主, 关系键);

        if (当前.有效 && 历史.有效) {
            上下文.中心坐标差 = 计算存在中心坐标差(当前, 历史);
            上下文.范围坐标重叠率 = 计算范围坐标重叠率(当前, 历史);
            上下文.尺寸差 = 计算存在尺寸差(当前, 历史);
            上下文.匹配置信度 = 计算匹配置信度(
                当前,
                历史,
                上下文.中心坐标差,
                上下文.范围坐标重叠率,
                上下文.尺寸差);
            上下文.历史对应状态 = 上下文.匹配置信度 >= 3500
                ? 历史对应状态_已匹配
                : 历史对应状态_冲突;
        } else {
            上下文.历史对应状态 = 历史对应状态_未匹配;
            上下文.匹配置信度 = 0;
        }

        auto* 节点 = reinterpret_cast<基础信息节点类*>(上下文.匹配关系);
        if (!节点) return;
        (void)写入基础节点I64(节点, 特征_来源空间候选编号(), 关系键, now);
        (void)写入基础节点I64(节点, 特征_历史对应状态(), 上下文.历史对应状态, now);
        (void)写入基础节点I64(节点, 特征_中心坐标差(), 上下文.中心坐标差, now);
        (void)写入基础节点I64(节点, 特征_范围坐标重叠率(), 上下文.范围坐标重叠率, now);
        (void)写入基础节点I64(节点, 特征_尺寸差(), 上下文.尺寸差, now);
        (void)写入基础节点I64(节点, 特征_匹配置信度(), 上下文.匹配置信度, now);
        (void)写入基础节点指针(节点, 特征_当前观察存在(), 上下文.当前存在, now);
        if (上下文.历史存在) {
            (void)写入基础节点指针(节点, 特征_历史观察存在(), 上下文.历史存在, now);
        }
    }

    inline void 计算观察存在变化结果(结构_观察变化上下文& 上下文, 时间戳 now) noexcept
    {
        if (!上下文.匹配关系) {
            计算观察存在匹配关系(上下文, now);
        }
        const auto 当前 = 读取观察存在摘要(上下文.当前存在);
        const auto 历史 = 读取观察存在摘要(上下文.历史存在);
        const I64 关系键 = 观察变化关系键(当前, 历史);
        上下文.变化结果 = 取或创建观察存在变化结果(上下文.宿主, 关系键);

        if (上下文.历史对应状态 == 历史对应状态_已匹配 && 当前.有效 && 历史.有效) {
            上下文.位置变化量 = 上下文.中心坐标差;
            上下文.范围变化量 = 10000 - 上下文.范围坐标重叠率;
            上下文.尺寸变化量 = 上下文.尺寸差;
            上下文.距离变化量 = 绝对差I64(当前.距离, 历史.距离);

            const I64 当前尺寸和 = 当前.尺寸X + 当前.尺寸Y + 当前.尺寸Z;
            const I64 历史尺寸和 = 历史.尺寸X + 历史.尺寸Y + 历史.尺寸Z;
            const I64 尺寸阈值 = std::max<I64>(80, std::max<I64>(当前尺寸和, 历史尺寸和) / 12);
            if (上下文.位置变化量 > std::max<I64>(120, std::max<I64>(当前尺寸和, 历史尺寸和) / 18)) {
                上下文.变化状态 = 变化状态_已移动;
            } else if (当前尺寸和 > 历史尺寸和 + 尺寸阈值) {
                上下文.变化状态 = 变化状态_已变大;
            } else if (历史尺寸和 > 当前尺寸和 + 尺寸阈值) {
                上下文.变化状态 = 变化状态_已变小;
            } else if (上下文.范围变化量 > 4000) {
                上下文.变化状态 = 变化状态_已移动;
            } else {
                上下文.变化状态 = 变化状态_未变化;
            }
        } else {
            上下文.变化状态 = 变化状态_不确定;
        }

        auto* 节点 = reinterpret_cast<基础信息节点类*>(上下文.变化结果);
        if (!节点) return;
        (void)写入基础节点I64(节点, 特征_来源空间候选编号(), 关系键, now);
        (void)写入基础节点I64(节点, 特征_观察存在变化结果(), 变化结果状态_已生成, now);
        (void)写入基础节点I64(节点, 特征_变化状态(), 上下文.变化状态, now);
        (void)写入基础节点I64(节点, 特征_位置变化量(), 上下文.位置变化量, now);
        (void)写入基础节点I64(节点, 特征_范围变化量(), 上下文.范围变化量, now);
        (void)写入基础节点I64(节点, 特征_尺寸变化量(), 上下文.尺寸变化量, now);
        (void)写入基础节点I64(节点, 特征_距离变化量(), 上下文.距离变化量, now);
        (void)写入基础节点指针(节点, 特征_观察存在匹配关系(), 上下文.匹配关系, now);
        (void)写入基础节点指针(节点, 特征_当前观察存在(), 上下文.当前存在, now);
        if (上下文.历史存在) {
            (void)写入基础节点指针(节点, 特征_历史观察存在(), 上下文.历史存在, now);
        }
    }

    inline void 计算观察变化事件(结构_观察变化上下文& 上下文, 时间戳 now) noexcept
    {
        if (!上下文.变化结果) {
            计算观察存在变化结果(上下文, now);
        }
        const auto 当前 = 读取观察存在摘要(上下文.当前存在);
        const auto 历史 = 读取观察存在摘要(上下文.历史存在);
        const I64 关系键 = 观察变化关系键(当前, 历史);
        上下文.变化事件 = 取或创建观察变化事件(上下文.宿主, 关系键);

        if (上下文.历史对应状态 == 历史对应状态_已匹配) {
            switch (上下文.变化状态) {
            case 变化状态_未变化:
                上下文.事件类型 = 观察变化事件类型_无变化;
                break;
            case 变化状态_已移动:
                上下文.事件类型 = 观察变化事件类型_位置变化;
                break;
            case 变化状态_已变大:
            case 变化状态_已变小:
                上下文.事件类型 = 观察变化事件类型_尺寸变化;
                break;
            case 变化状态_内部结构变化:
                上下文.事件类型 = 观察变化事件类型_内部结构变化;
                break;
            default:
                上下文.事件类型 = 观察变化事件类型_不确定;
                break;
            }
            上下文.事件置信度 = 上下文.匹配置信度;
        } else if (!当前.有效 && 历史.有效) {
            上下文.事件类型 = 观察变化事件类型_未命中;
            上下文.事件置信度 = 0;
        } else if (当前.有效 && 历史.有效 && 上下文.历史对应状态 == 历史对应状态_冲突) {
            上下文.事件类型 = 观察变化事件类型_不确定;
            上下文.事件置信度 = 上下文.匹配置信度;
        } else {
            上下文.事件类型 = 观察变化事件类型_不确定;
            上下文.事件置信度 = 0;
        }

        auto* 节点 = reinterpret_cast<基础信息节点类*>(上下文.变化事件);
        if (!节点) return;
        (void)写入基础节点I64(节点, 特征_来源空间候选编号(), 关系键, now);
        (void)写入基础节点I64(节点, 特征_观察变化事件集合(), 观察变化事件集合状态_已生成, now);
        (void)写入基础节点I64(节点, 特征_观察变化事件类型(), 上下文.事件类型, now);
        (void)写入基础节点I64(节点, 特征_变化事件置信度(), 上下文.事件置信度, now);
        (void)写入基础节点I64(节点, 特征_变化状态(), 上下文.变化状态, now);
        (void)写入基础节点指针(节点, 特征_观察存在匹配关系(), 上下文.匹配关系, now);
        (void)写入基础节点指针(节点, 特征_观察存在变化结果(), 上下文.变化结果, now);
        if (上下文.当前存在) {
            (void)写入基础节点指针(节点, 特征_当前观察存在(), 上下文.当前存在, now);
        }
        if (上下文.历史存在) {
            (void)写入基础节点指针(节点, 特征_历史观察存在(), 上下文.历史存在, now);
        }
        if (上下文.宿主) {
            (void)写入基础节点I64(上下文.宿主, 特征_观察变化事件集合(), 观察变化事件集合状态_已生成, now);
        }
    }

    inline void 写入观察变化方法结果(
        基础信息节点类* 宿主,
        bool 成功,
        const 结构_观察变化上下文& 上下文,
        动态节点类* 动态,
        const 语素入口节点类* 失败原因,
        时间戳 now) noexcept
    {
        if (!宿主) return;
        (void)写入方法执行情况(宿主, 成功 ? 值_执行成功() : 值_执行失败(), now);
        if (上下文.当前存在) {
            (void)写入基础节点指针(宿主, 特征_当前观察存在(), 上下文.当前存在, now);
        }
        if (上下文.历史存在) {
            (void)写入基础节点指针(宿主, 特征_历史观察存在(), 上下文.历史存在, now);
        }
        if (上下文.匹配关系) {
            (void)写入基础节点指针(宿主, 特征_观察存在匹配关系(), 上下文.匹配关系, now);
        }
        if (上下文.变化结果) {
            (void)写入基础节点指针(宿主, 特征_观察存在变化结果(), 上下文.变化结果, now);
        }
        if (上下文.变化事件) {
            (void)写入基础节点指针(宿主, 特征_观察变化事件集合(), 上下文.变化事件, now);
        }
        (void)写入基础节点I64(宿主, 特征_历史对应状态(), 上下文.历史对应状态, now);
        (void)写入基础节点I64(宿主, 特征_匹配置信度(), 上下文.匹配置信度, now);
        (void)写入基础节点I64(宿主, 特征_变化状态(), 上下文.变化状态, now);
        (void)写入基础节点I64(宿主, 特征_观察变化事件类型(), 上下文.事件类型, now);
        if (动态) {
            (void)写入基础节点指针(宿主, 特征_动作动态(), 动态, now);
        }
        if (失败原因) {
            (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
        }
    }

    struct 结构_场景复现上下文 {
        基础信息节点类* 宿主 = nullptr;
        存在节点类* 场景识别快照 = nullptr;
        存在节点类* OpenGL复现数据 = nullptr;
        I64 当前观察帧 = 0;
        I64 帧宽度 = 0;
        I64 帧高度 = 0;
        I64 深度帧号 = 0;
        I64 彩色帧号 = 0;
        I64 预期像素数量 = 0;
        I64 像素特征数量 = 0;
        I64 深度有效像素数量 = 0;
        I64 点云有效像素数量 = 0;
        I64 像素覆盖状态 = 0;
        I64 空间坐标单位毫米 = 0;
        I64 颜色RGB结构状态 = 0;
        I64 原始深度毫米结构状态 = 0;
        I64 滤波深度毫米结构状态 = 0;
        I64 补全深度毫米结构状态 = 0;
        I64 深度有效性Mask状态 = 0;
        I64 深度来源结构状态 = 0;
        I64 深度稳定性结构状态 = 0;
        I64 深度邻域一致性结构状态 = 0;
        I64 融合深度毫米结构状态 = 0;
        I64 融合深度有效性结构状态 = 0;
        I64 多帧深度稳定性结构状态 = 0;
        I64 每像素深度方差结构状态 = 0;
        I64 空间坐标毫米XYZ结构状态 = 0;
        I64 原始深度来源像素数量 = 0;
        I64 滤波深度来源像素数量 = 0;
        I64 补全深度来源像素数量 = 0;
        I64 无有效深度来源像素数量 = 0;
        I64 补全深度低置信像素数量 = 0;
        I64 深度稳定性平均值 = 0;
        I64 深度邻域一致性平均值 = 0;
        I64 观察帧组状态 = 0;
        I64 观察帧组帧数 = 0;
        I64 融合依据帧集合状态 = 0;
        I64 融合深度有效像素数量 = 0;
        I64 融合深度有效率 = 0;
        I64 融合补偿深度空洞像素数量 = 0;
        I64 融合低稳定像素数量 = 0;
        I64 融合深度稳定性平均值 = 0;
        I64 融合深度平均方差 = 0;
        I64 彩色深度已对齐 = 0;
        I64 对齐目标 = 0;
        I64 深度有效率 = 0;
        I64 空间坐标有效率 = 0;
        I64 帧质量评分 = 0;
        I64 观察帧质量诊断摘要 = 0;
        I64 深度空洞数量 = 0;
        I64 深度空洞区域数量 = 0;
        I64 深度空洞区域集合状态 = 0;
        I64 彩色深度对齐状态 = 0;
        I64 轮廓闭合率 = 0;
        I64 轮廓断裂数量 = 0;
        I64 彩图轮廓数量 = 0;
        I64 深度轮廓数量 = 0;
        I64 空间投影轮廓数量 = 0;
        I64 融合轮廓数量 = 0;
        I64 轮廓来源分层状态 = 0;
        I64 轮廓深度支持率 = 0;
        I64 轮廓颜色支持率 = 0;
        I64 轮廓空间支持率 = 0;
        I64 融合轮廓置信度 = 0;
        I64 跨越深度断裂轮廓数量 = 0;
        I64 穿过深度无效区域轮廓数量 = 0;
        I64 依赖补全深度轮廓数量 = 0;
        I64 空间点有效率 = 0;
        I64 边界深度稳定率 = 0;
        I64 候选存在验证通过率 = 0;
        I64 未解释区域比例 = 0;
        I64 补观察缺口状态 = 0;
        I64 补观察缺失原因 = 0;
        I64 待补观察区域数量 = 0;
        I64 补全候选数量 = 0;
        I64 低置信轮廓数量 = 0;
        I64 条件不足候选数量 = 0;
        I64 待验证候选数量 = 0;
        I64 部分确认候选数量 = 0;
        I64 补观察需求建议 = 0;
        I64 空间候选数量 = 0;
        I64 空间候选有效点数量 = 0;
        I64 主空间候选编号 = 0;
        I64 主空间候选像素数量 = 0;
        I64 中心X = 0;
        I64 中心Y = 0;
        I64 中心Z = 0;
        I64 最小X = 0;
        I64 最大X = 0;
        I64 最小Y = 0;
        I64 最大Y = 0;
        I64 最小Z = 0;
        I64 最大Z = 0;
        I64 空间连续性评分 = 0;
        I64 范围稳定性评分 = 0;
        I64 观察存在假设 = 0;
        I64 来源空间候选编号 = 0;
        I64 存在假设验证状态 = 0;
        I64 假设距离 = 0;
        I64 假设尺寸X = 0;
        I64 假设尺寸Y = 0;
        I64 假设尺寸Z = 0;
        I64 假设有效点比例 = 0;
        I64 像素归属验证状态 = 0;
        I64 深度一致率 = 0;
        I64 范围内点比例 = 0;
        I64 投影覆盖率 = 0;
        I64 轮廓吻合率 = 0;
        I64 已验证观察存在 = 0;
        I64 观察存在确认状态 = 0;
        I64 已验证观察存在数量 = 0;
        I64 帧解释状态 = 0;
        I64 内部世界状态 = 0;
        I64 观察变化事件集合 = 0;
        bool 有识别材料 = false;
    };

    inline bool 场景复现候选范围有效(const 结构_场景复现上下文& 上下文) noexcept
    {
        return 上下文.最大X > 上下文.最小X
            || 上下文.最大Y > 上下文.最小Y
            || 上下文.最大Z > 上下文.最小Z;
    }

    inline 存在节点类* 取或创建场景识别快照(基础信息节点类* 宿主) noexcept
    {
        return 宿主 ? 世界树.取或创建子存在_按类型(宿主, 类型_场景识别快照()) : nullptr;
    }

    inline 存在节点类* 取或创建OpenGL复现数据(基础信息节点类* 宿主) noexcept
    {
        return 宿主 ? 世界树.取或创建子存在_按类型(宿主, 类型_OpenGL复现数据()) : nullptr;
    }

    inline 结构_场景复现上下文 读取场景复现上下文(基础信息节点类* 宿主) noexcept
    {
        结构_场景复现上下文 上下文{};
        上下文.宿主 = 宿主;
        if (!宿主) {
            return 上下文;
        }

        上下文.当前观察帧 = 读取I64或0(宿主, 特征_当前观察帧());
        上下文.帧宽度 = 读取I64或0(宿主, 特征_相机帧宽度());
        上下文.帧高度 = 读取I64或0(宿主, 特征_相机帧高度());
        上下文.深度帧号 = 读取I64或0(宿主, 特征_深度帧号());
        上下文.彩色帧号 = 读取I64或0(宿主, 特征_彩色帧号());
        上下文.预期像素数量 = 读取I64或0(宿主, 特征_预期像素数量());
        上下文.像素特征数量 = 读取I64或0(宿主, 特征_像素特征数量());
        上下文.深度有效像素数量 = 读取I64或0(宿主, 特征_深度有效像素数量());
        上下文.点云有效像素数量 = 读取I64或0(宿主, 特征_点云有效像素数量());
        上下文.像素覆盖状态 = 读取I64或0(宿主, 特征_像素覆盖状态());
        上下文.空间坐标单位毫米 = 读取I64或0(宿主, 特征_空间坐标单位毫米());
        上下文.颜色RGB结构状态 = 读取I64或0(宿主, 特征_颜色RGB结构状态());
        上下文.原始深度毫米结构状态 = 读取I64或0(宿主, 特征_原始深度毫米结构状态());
        上下文.滤波深度毫米结构状态 = 读取I64或0(宿主, 特征_滤波深度毫米结构状态());
        上下文.补全深度毫米结构状态 = 读取I64或0(宿主, 特征_补全深度毫米结构状态());
        上下文.深度有效性Mask状态 = 读取I64或0(宿主, 特征_深度有效性Mask状态());
        上下文.深度来源结构状态 = 读取I64或0(宿主, 特征_深度来源结构状态());
        上下文.深度稳定性结构状态 = 读取I64或0(宿主, 特征_深度稳定性结构状态());
        上下文.深度邻域一致性结构状态 = 读取I64或0(宿主, 特征_深度邻域一致性结构状态());
        上下文.融合深度毫米结构状态 = 读取I64或0(宿主, 特征_融合深度毫米结构状态());
        上下文.融合深度有效性结构状态 = 读取I64或0(宿主, 特征_融合深度有效性结构状态());
        上下文.多帧深度稳定性结构状态 = 读取I64或0(宿主, 特征_多帧深度稳定性结构状态());
        上下文.每像素深度方差结构状态 = 读取I64或0(宿主, 特征_每像素深度方差结构状态());
        上下文.空间坐标毫米XYZ结构状态 = 读取I64或0(宿主, 特征_空间坐标毫米XYZ结构状态());
        上下文.原始深度来源像素数量 = 读取I64或0(宿主, 特征_原始深度来源像素数量());
        上下文.滤波深度来源像素数量 = 读取I64或0(宿主, 特征_滤波深度来源像素数量());
        上下文.补全深度来源像素数量 = 读取I64或0(宿主, 特征_补全深度来源像素数量());
        上下文.无有效深度来源像素数量 = 读取I64或0(宿主, 特征_无有效深度来源像素数量());
        上下文.补全深度低置信像素数量 = 读取I64或0(宿主, 特征_补全深度低置信像素数量());
        上下文.深度稳定性平均值 = 读取I64或0(宿主, 特征_深度稳定性平均值());
        上下文.深度邻域一致性平均值 = 读取I64或0(宿主, 特征_深度邻域一致性平均值());
        上下文.观察帧组状态 = 读取I64或0(宿主, 特征_观察帧组状态());
        上下文.观察帧组帧数 = 读取I64或0(宿主, 特征_观察帧组帧数());
        上下文.融合依据帧集合状态 = 读取I64或0(宿主, 特征_融合依据帧集合状态());
        上下文.融合深度有效像素数量 = 读取I64或0(宿主, 特征_融合深度有效像素数量());
        上下文.融合深度有效率 = 读取I64或0(宿主, 特征_融合深度有效率());
        上下文.融合补偿深度空洞像素数量 = 读取I64或0(宿主, 特征_融合补偿深度空洞像素数量());
        上下文.融合低稳定像素数量 = 读取I64或0(宿主, 特征_融合低稳定像素数量());
        上下文.融合深度稳定性平均值 = 读取I64或0(宿主, 特征_融合深度稳定性平均值());
        上下文.融合深度平均方差 = 读取I64或0(宿主, 特征_融合深度平均方差());
        上下文.彩色深度已对齐 = 读取I64或0(宿主, 特征_彩色深度已对齐());
        上下文.对齐目标 = 读取I64或0(宿主, 特征_对齐目标());
        上下文.深度有效率 = 读取I64或0(宿主, 特征_深度有效率());
        上下文.空间坐标有效率 = 读取I64或0(宿主, 特征_空间坐标有效率());
        上下文.帧质量评分 = 读取I64或0(宿主, 特征_帧质量评分());
        上下文.观察帧质量诊断摘要 = 读取I64或0(宿主, 特征_观察帧质量诊断摘要());
        上下文.深度空洞数量 = 读取I64或0(宿主, 特征_深度空洞数量());
        上下文.深度空洞区域数量 = 读取I64或0(宿主, 特征_深度空洞区域数量());
        上下文.深度空洞区域集合状态 = 读取I64或0(宿主, 特征_深度空洞区域集合());
        上下文.彩色深度对齐状态 = 读取I64或0(宿主, 特征_彩色深度对齐状态());
        上下文.轮廓闭合率 = 读取I64或0(宿主, 特征_轮廓闭合率());
        上下文.轮廓断裂数量 = 读取I64或0(宿主, 特征_轮廓断裂数量());
        上下文.彩图轮廓数量 = 读取I64或0(宿主, 特征_彩图轮廓数量());
        上下文.深度轮廓数量 = 读取I64或0(宿主, 特征_深度轮廓数量());
        上下文.空间投影轮廓数量 = 读取I64或0(宿主, 特征_空间投影轮廓数量());
        上下文.融合轮廓数量 = 读取I64或0(宿主, 特征_融合轮廓数量());
        上下文.轮廓来源分层状态 = 读取I64或0(宿主, 特征_轮廓来源分层状态());
        上下文.轮廓深度支持率 = 读取I64或0(宿主, 特征_轮廓深度支持率());
        上下文.轮廓颜色支持率 = 读取I64或0(宿主, 特征_轮廓颜色支持率());
        上下文.轮廓空间支持率 = 读取I64或0(宿主, 特征_轮廓空间支持率());
        上下文.融合轮廓置信度 = 读取I64或0(宿主, 特征_融合轮廓置信度());
        上下文.跨越深度断裂轮廓数量 = 读取I64或0(宿主, 特征_跨越深度断裂轮廓数量());
        上下文.穿过深度无效区域轮廓数量 = 读取I64或0(宿主, 特征_穿过深度无效区域轮廓数量());
        上下文.依赖补全深度轮廓数量 = 读取I64或0(宿主, 特征_依赖补全深度轮廓数量());
        上下文.空间点有效率 = 读取I64或0(宿主, 特征_空间点有效率());
        上下文.边界深度稳定率 = 读取I64或0(宿主, 特征_边界深度稳定率());
        上下文.候选存在验证通过率 = 读取I64或0(宿主, 特征_候选存在验证通过率());
        上下文.未解释区域比例 = 读取I64或0(宿主, 特征_未解释区域比例());
        上下文.补观察缺口状态 = 读取I64或0(宿主, 特征_补观察缺口状态());
        上下文.补观察缺失原因 = 读取I64或0(宿主, 特征_补观察缺失原因());
        上下文.待补观察区域数量 = 读取I64或0(宿主, 特征_待补观察区域数量());
        上下文.补全候选数量 = 读取I64或0(宿主, 特征_补全候选数量());
        上下文.低置信轮廓数量 = 读取I64或0(宿主, 特征_低置信轮廓数量());
        上下文.条件不足候选数量 = 读取I64或0(宿主, 特征_条件不足候选数量());
        上下文.待验证候选数量 = 读取I64或0(宿主, 特征_待验证候选数量());
        上下文.部分确认候选数量 = 读取I64或0(宿主, 特征_部分确认候选数量());
        上下文.补观察需求建议 = 读取I64或0(宿主, 特征_补观察需求建议());
        上下文.空间候选数量 = 读取I64或0(宿主, 特征_空间候选数量());
        上下文.空间候选有效点数量 = 读取I64或0(宿主, 特征_空间候选有效点数量());
        上下文.主空间候选编号 = 读取I64或0(宿主, 特征_主空间候选编号());
        上下文.主空间候选像素数量 = 读取I64或0(宿主, 特征_主空间候选像素数量());
        上下文.中心X = 读取I64或0(宿主, 特征_中心空间坐标X());
        上下文.中心Y = 读取I64或0(宿主, 特征_中心空间坐标Y());
        上下文.中心Z = 读取I64或0(宿主, 特征_中心空间坐标Z());
        上下文.最小X = 读取I64或0(宿主, 特征_范围坐标AABB最小X());
        上下文.最大X = 读取I64或0(宿主, 特征_范围坐标AABB最大X());
        上下文.最小Y = 读取I64或0(宿主, 特征_范围坐标AABB最小Y());
        上下文.最大Y = 读取I64或0(宿主, 特征_范围坐标AABB最大Y());
        上下文.最小Z = 读取I64或0(宿主, 特征_范围坐标AABB最小Z());
        上下文.最大Z = 读取I64或0(宿主, 特征_范围坐标AABB最大Z());
        上下文.空间连续性评分 = 读取I64或0(宿主, 特征_空间连续性评分());
        上下文.范围稳定性评分 = 读取I64或0(宿主, 特征_范围稳定性评分());
        上下文.观察存在假设 = 读取I64或0(宿主, 特征_观察存在假设());
        上下文.来源空间候选编号 = 读取I64或0(宿主, 特征_来源空间候选编号());
        if (上下文.来源空间候选编号 == 0) {
            上下文.来源空间候选编号 = 上下文.主空间候选编号;
        }
        上下文.存在假设验证状态 = 读取I64或0(宿主, 特征_存在假设验证状态());
        上下文.假设距离 = 读取I64或0(宿主, 特征_假设距离());
        上下文.假设尺寸X = 读取I64或0(宿主, 特征_假设尺寸X());
        上下文.假设尺寸Y = 读取I64或0(宿主, 特征_假设尺寸Y());
        上下文.假设尺寸Z = 读取I64或0(宿主, 特征_假设尺寸Z());
        上下文.假设有效点比例 = 读取I64或0(宿主, 特征_假设有效点比例());
        上下文.像素归属验证状态 = 读取I64或0(宿主, 特征_像素归属验证状态());
        上下文.深度一致率 = 读取I64或0(宿主, 特征_深度一致率());
        上下文.范围内点比例 = 读取I64或0(宿主, 特征_范围内点比例());
        上下文.投影覆盖率 = 读取I64或0(宿主, 特征_投影覆盖率());
        上下文.轮廓吻合率 = 读取I64或0(宿主, 特征_轮廓吻合率());
        上下文.已验证观察存在 = 读取I64或0(宿主, 特征_已验证观察存在());
        上下文.观察存在确认状态 = 读取I64或0(宿主, 特征_观察存在确认状态());
        上下文.已验证观察存在数量 = 读取I64或0(宿主, 特征_已验证观察存在数量());
        上下文.帧解释状态 = 读取I64或0(宿主, 特征_帧解释状态());
        上下文.内部世界状态 = 读取I64或0(宿主, 特征_内部世界状态());
        上下文.观察变化事件集合 = 读取I64或0(宿主, 特征_观察变化事件集合());
        上下文.有识别材料 = 上下文.当前观察帧 != 0
            || 上下文.帧宽度 > 0
            || 上下文.空间候选数量 > 0
            || 上下文.观察存在假设 != 0
            || 上下文.已验证观察存在数量 > 0
            || 上下文.像素特征数量 > 0;
        return 上下文;
    }

    inline void 写入场景识别快照摘要(
        基础信息节点类* 节点,
        const 结构_场景复现上下文& 上下文,
        时间戳 now) noexcept
    {
        if (!节点) return;
        (void)写入基础节点I64(节点, 特征_场景识别快照(), 场景识别快照状态_已生成, now);
        (void)写入基础节点I64(节点, 特征_快照状态(), 场景识别快照状态_已生成, now);
        (void)写入基础节点I64(节点, 特征_当前观察帧(), 上下文.当前观察帧, now);
        (void)写入基础节点I64(节点, 特征_相机帧宽度(), 上下文.帧宽度, now);
        (void)写入基础节点I64(节点, 特征_相机帧高度(), 上下文.帧高度, now);
        (void)写入基础节点I64(节点, 特征_深度帧号(), 上下文.深度帧号, now);
        (void)写入基础节点I64(节点, 特征_彩色帧号(), 上下文.彩色帧号, now);
        (void)写入基础节点I64(节点, 特征_预期像素数量(), 上下文.预期像素数量, now);
        (void)写入基础节点I64(节点, 特征_像素特征数量(), 上下文.像素特征数量, now);
        (void)写入基础节点I64(节点, 特征_深度有效像素数量(), 上下文.深度有效像素数量, now);
        (void)写入基础节点I64(节点, 特征_点云有效像素数量(), 上下文.点云有效像素数量, now);
        (void)写入基础节点I64(节点, 特征_像素覆盖状态(), 上下文.像素覆盖状态, now);
        (void)写入基础节点I64(节点, 特征_空间坐标单位毫米(), 上下文.空间坐标单位毫米, now);
        (void)写入基础节点I64(节点, 特征_颜色RGB结构状态(), 上下文.颜色RGB结构状态, now);
        (void)写入基础节点I64(节点, 特征_原始深度毫米结构状态(), 上下文.原始深度毫米结构状态, now);
        (void)写入基础节点I64(节点, 特征_滤波深度毫米结构状态(), 上下文.滤波深度毫米结构状态, now);
        (void)写入基础节点I64(节点, 特征_补全深度毫米结构状态(), 上下文.补全深度毫米结构状态, now);
        (void)写入基础节点I64(节点, 特征_深度有效性Mask状态(), 上下文.深度有效性Mask状态, now);
        (void)写入基础节点I64(节点, 特征_深度来源结构状态(), 上下文.深度来源结构状态, now);
        (void)写入基础节点I64(节点, 特征_深度稳定性结构状态(), 上下文.深度稳定性结构状态, now);
        (void)写入基础节点I64(节点, 特征_深度邻域一致性结构状态(), 上下文.深度邻域一致性结构状态, now);
        (void)写入基础节点I64(节点, 特征_融合深度毫米结构状态(), 上下文.融合深度毫米结构状态, now);
        (void)写入基础节点I64(节点, 特征_融合深度有效性结构状态(), 上下文.融合深度有效性结构状态, now);
        (void)写入基础节点I64(节点, 特征_多帧深度稳定性结构状态(), 上下文.多帧深度稳定性结构状态, now);
        (void)写入基础节点I64(节点, 特征_每像素深度方差结构状态(), 上下文.每像素深度方差结构状态, now);
        (void)写入基础节点I64(节点, 特征_空间坐标毫米XYZ结构状态(), 上下文.空间坐标毫米XYZ结构状态, now);
        (void)写入基础节点I64(节点, 特征_原始深度来源像素数量(), 上下文.原始深度来源像素数量, now);
        (void)写入基础节点I64(节点, 特征_滤波深度来源像素数量(), 上下文.滤波深度来源像素数量, now);
        (void)写入基础节点I64(节点, 特征_补全深度来源像素数量(), 上下文.补全深度来源像素数量, now);
        (void)写入基础节点I64(节点, 特征_无有效深度来源像素数量(), 上下文.无有效深度来源像素数量, now);
        (void)写入基础节点I64(节点, 特征_补全深度低置信像素数量(), 上下文.补全深度低置信像素数量, now);
        (void)写入基础节点I64(节点, 特征_深度稳定性平均值(), 上下文.深度稳定性平均值, now);
        (void)写入基础节点I64(节点, 特征_深度邻域一致性平均值(), 上下文.深度邻域一致性平均值, now);
        (void)写入基础节点I64(节点, 特征_观察帧组状态(), 上下文.观察帧组状态, now);
        (void)写入基础节点I64(节点, 特征_观察帧组帧数(), 上下文.观察帧组帧数, now);
        (void)写入基础节点I64(节点, 特征_融合依据帧集合状态(), 上下文.融合依据帧集合状态, now);
        (void)写入基础节点I64(节点, 特征_融合深度有效像素数量(), 上下文.融合深度有效像素数量, now);
        (void)写入基础节点I64(节点, 特征_融合深度有效率(), 上下文.融合深度有效率, now);
        (void)写入基础节点I64(节点, 特征_融合补偿深度空洞像素数量(), 上下文.融合补偿深度空洞像素数量, now);
        (void)写入基础节点I64(节点, 特征_融合低稳定像素数量(), 上下文.融合低稳定像素数量, now);
        (void)写入基础节点I64(节点, 特征_融合深度稳定性平均值(), 上下文.融合深度稳定性平均值, now);
        (void)写入基础节点I64(节点, 特征_融合深度平均方差(), 上下文.融合深度平均方差, now);
        (void)写入基础节点I64(节点, 特征_彩色深度已对齐(), 上下文.彩色深度已对齐, now);
        (void)写入基础节点I64(节点, 特征_对齐目标(), 上下文.对齐目标, now);
        (void)写入基础节点I64(节点, 特征_深度有效率(), 上下文.深度有效率, now);
        (void)写入基础节点I64(节点, 特征_空间坐标有效率(), 上下文.空间坐标有效率, now);
        (void)写入基础节点I64(节点, 特征_帧质量评分(), 上下文.帧质量评分, now);
        (void)写入基础节点I64(节点, 特征_观察帧质量诊断摘要(), 上下文.观察帧质量诊断摘要, now);
        (void)写入基础节点I64(节点, 特征_深度空洞数量(), 上下文.深度空洞数量, now);
        (void)写入基础节点I64(节点, 特征_深度空洞区域数量(), 上下文.深度空洞区域数量, now);
        (void)写入基础节点I64(节点, 特征_深度空洞区域集合(), 上下文.深度空洞区域集合状态, now);
        (void)写入基础节点I64(节点, 特征_彩色深度对齐状态(), 上下文.彩色深度对齐状态, now);
        (void)写入基础节点I64(节点, 特征_轮廓闭合率(), 上下文.轮廓闭合率, now);
        (void)写入基础节点I64(节点, 特征_轮廓断裂数量(), 上下文.轮廓断裂数量, now);
        (void)写入基础节点I64(节点, 特征_彩图轮廓数量(), 上下文.彩图轮廓数量, now);
        (void)写入基础节点I64(节点, 特征_深度轮廓数量(), 上下文.深度轮廓数量, now);
        (void)写入基础节点I64(节点, 特征_空间投影轮廓数量(), 上下文.空间投影轮廓数量, now);
        (void)写入基础节点I64(节点, 特征_融合轮廓数量(), 上下文.融合轮廓数量, now);
        (void)写入基础节点I64(节点, 特征_轮廓来源分层状态(), 上下文.轮廓来源分层状态, now);
        (void)写入基础节点I64(节点, 特征_轮廓深度支持率(), 上下文.轮廓深度支持率, now);
        (void)写入基础节点I64(节点, 特征_轮廓颜色支持率(), 上下文.轮廓颜色支持率, now);
        (void)写入基础节点I64(节点, 特征_轮廓空间支持率(), 上下文.轮廓空间支持率, now);
        (void)写入基础节点I64(节点, 特征_融合轮廓置信度(), 上下文.融合轮廓置信度, now);
        (void)写入基础节点I64(节点, 特征_跨越深度断裂轮廓数量(), 上下文.跨越深度断裂轮廓数量, now);
        (void)写入基础节点I64(节点, 特征_穿过深度无效区域轮廓数量(), 上下文.穿过深度无效区域轮廓数量, now);
        (void)写入基础节点I64(节点, 特征_依赖补全深度轮廓数量(), 上下文.依赖补全深度轮廓数量, now);
        (void)写入基础节点I64(节点, 特征_空间点有效率(), 上下文.空间点有效率, now);
        (void)写入基础节点I64(节点, 特征_边界深度稳定率(), 上下文.边界深度稳定率, now);
        (void)写入基础节点I64(节点, 特征_候选存在验证通过率(), 上下文.候选存在验证通过率, now);
        (void)写入基础节点I64(节点, 特征_未解释区域比例(), 上下文.未解释区域比例, now);
        (void)写入基础节点I64(节点, 特征_补观察缺口状态(), 上下文.补观察缺口状态, now);
        (void)写入基础节点I64(节点, 特征_补观察缺失原因(), 上下文.补观察缺失原因, now);
        (void)写入基础节点I64(节点, 特征_待补观察区域数量(), 上下文.待补观察区域数量, now);
        (void)写入基础节点I64(节点, 特征_补全候选数量(), 上下文.补全候选数量, now);
        (void)写入基础节点I64(节点, 特征_低置信轮廓数量(), 上下文.低置信轮廓数量, now);
        (void)写入基础节点I64(节点, 特征_条件不足候选数量(), 上下文.条件不足候选数量, now);
        (void)写入基础节点I64(节点, 特征_待验证候选数量(), 上下文.待验证候选数量, now);
        (void)写入基础节点I64(节点, 特征_部分确认候选数量(), 上下文.部分确认候选数量, now);
        (void)写入基础节点I64(节点, 特征_补观察需求建议(), 上下文.补观察需求建议, now);
        (void)写入基础节点I64(节点, 特征_空间候选数量(), 上下文.空间候选数量, now);
        (void)写入基础节点I64(节点, 特征_空间候选有效点数量(), 上下文.空间候选有效点数量, now);
        (void)写入基础节点I64(节点, 特征_主空间候选编号(), 上下文.主空间候选编号, now);
        (void)写入基础节点I64(节点, 特征_主空间候选像素数量(), 上下文.主空间候选像素数量, now);
        (void)写入基础节点I64(节点, 特征_来源空间候选编号(), 上下文.来源空间候选编号, now);
        (void)写入基础节点I64(节点, 特征_中心空间坐标X(), 上下文.中心X, now);
        (void)写入基础节点I64(节点, 特征_中心空间坐标Y(), 上下文.中心Y, now);
        (void)写入基础节点I64(节点, 特征_中心空间坐标Z(), 上下文.中心Z, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小X(), 上下文.最小X, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大X(), 上下文.最大X, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小Y(), 上下文.最小Y, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大Y(), 上下文.最大Y, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最小Z(), 上下文.最小Z, now);
        (void)写入基础节点I64(节点, 特征_范围坐标AABB最大Z(), 上下文.最大Z, now);
        (void)写入基础节点I64(节点, 特征_空间连续性评分(), 上下文.空间连续性评分, now);
        (void)写入基础节点I64(节点, 特征_范围稳定性评分(), 上下文.范围稳定性评分, now);
        (void)写入基础节点I64(节点, 特征_观察存在假设(), 上下文.观察存在假设, now);
        (void)写入基础节点I64(节点, 特征_存在假设验证状态(), 上下文.存在假设验证状态, now);
        (void)写入基础节点I64(节点, 特征_假设距离(), 上下文.假设距离, now);
        (void)写入基础节点I64(节点, 特征_假设尺寸X(), 上下文.假设尺寸X, now);
        (void)写入基础节点I64(节点, 特征_假设尺寸Y(), 上下文.假设尺寸Y, now);
        (void)写入基础节点I64(节点, 特征_假设尺寸Z(), 上下文.假设尺寸Z, now);
        (void)写入基础节点I64(节点, 特征_假设有效点比例(), 上下文.假设有效点比例, now);
        (void)写入基础节点I64(节点, 特征_像素归属验证状态(), 上下文.像素归属验证状态, now);
        (void)写入基础节点I64(节点, 特征_深度一致率(), 上下文.深度一致率, now);
        (void)写入基础节点I64(节点, 特征_范围内点比例(), 上下文.范围内点比例, now);
        (void)写入基础节点I64(节点, 特征_投影覆盖率(), 上下文.投影覆盖率, now);
        (void)写入基础节点I64(节点, 特征_轮廓吻合率(), 上下文.轮廓吻合率, now);
        (void)写入基础节点I64(节点, 特征_已验证观察存在(), 上下文.已验证观察存在, now);
        (void)写入基础节点I64(节点, 特征_观察存在确认状态(), 上下文.观察存在确认状态, now);
        (void)写入基础节点I64(节点, 特征_已验证观察存在数量(), 上下文.已验证观察存在数量, now);
        (void)写入基础节点I64(节点, 特征_帧解释状态(), 上下文.帧解释状态, now);
        (void)写入基础节点I64(节点, 特征_内部世界状态(), 上下文.内部世界状态, now);
        (void)写入基础节点I64(节点, 特征_观察变化事件集合(), 上下文.观察变化事件集合, now);
        (void)写入基础节点I64(节点, 特征_像素颜色层(), 上下文.像素特征数量 > 0 ? 1 : 0, now);
        (void)写入基础节点I64(节点, 特征_深度层(), 上下文.深度有效像素数量 > 0 ? 1 : 0, now);
        (void)写入基础节点I64(节点, 特征_空间点层(), 上下文.点云有效像素数量 > 0 ? 1 : 0, now);
        (void)写入基础节点I64(节点, 特征_空间候选层(), 上下文.空间候选数量 > 0 ? 1 : 0, now);
        (void)写入基础节点I64(节点, 特征_已确认观察存在层(), 上下文.已验证观察存在数量 > 0 ? 1 : 0, now);
        (void)写入基础节点I64(节点, 特征_存在内部世界展开层(), 上下文.内部世界状态 > 0 ? 1 : 0, now);
        (void)写入基础节点I64(节点, 特征_变化事件层(), 上下文.观察变化事件集合 > 0 ? 1 : 0, now);
        (void)写入基础节点I64(节点, 特征_诊断摘要状态(), 1, now);
    }

    inline void 写入OpenGL复现数据摘要(
        基础信息节点类* 节点,
        const 结构_场景复现上下文& 上下文,
        时间戳 now) noexcept
    {
        if (!节点) return;
        const bool 候选范围有效 = 场景复现候选范围有效(上下文);
        const I64 基准线段数 = 25;
        const I64 AABB线段数 = 候选范围有效 ? 12 : 0;
        const I64 点数量 = 上下文.已验证观察存在数量 > 0 ? 上下文.已验证观察存在数量 : (候选范围有效 ? 1 : 0);
        const I64 线段数量 = 基准线段数 + AABB线段数;
        const I64 顶点数量 = 线段数量 * 2 + 点数量;

        写入场景识别快照摘要(节点, 上下文, now);
        (void)写入基础节点I64(节点, 特征_OpenGL复现数据(), OpenGL复现数据状态_已生成, now);
        (void)写入基础节点I64(节点, 特征_复现数据状态(), OpenGL复现数据状态_已生成, now);
        (void)写入基础节点I64(节点, 特征_顶点缓冲(), 复现数据缓冲状态_摘要级, now);
        (void)写入基础节点I64(节点, 特征_颜色缓冲(), 复现数据缓冲状态_摘要级, now);
        (void)写入基础节点I64(节点, 特征_深度缓冲(), 上下文.深度有效像素数量 > 0 ? 复现数据缓冲状态_摘要级 : 0, now);
        (void)写入基础节点I64(节点, 特征_存在ID缓冲(), 上下文.已验证观察存在数量 > 0 ? 复现数据缓冲状态_摘要级 : 0, now);
        (void)写入基础节点I64(节点, 特征_AABB绘制状态(), 候选范围有效 ? 1 : 0, now);
        (void)写入基础节点I64(节点, 特征_绘制线段数量(), 线段数量, now);
        (void)写入基础节点I64(节点, 特征_绘制点数量(), 点数量, now);
        (void)写入基础节点I64(节点, 特征_绘制顶点数量(), 顶点数量, now);
    }

    inline void 写入场景复现方法结果(
        基础信息节点类* 宿主,
        bool 成功,
        const 结构_场景复现上下文& 上下文,
        动态节点类* 动态,
        const 语素入口节点类* 失败原因,
        时间戳 now) noexcept
    {
        if (!宿主) return;
        (void)写入方法执行情况(宿主, 成功 ? 值_执行成功() : 值_执行失败(), now);
        if (上下文.场景识别快照) {
            (void)写入基础节点指针(宿主, 特征_场景识别快照(), 上下文.场景识别快照, now);
            (void)写入基础节点I64(宿主, 特征_快照状态(), 场景识别快照状态_已生成, now);
        }
        if (上下文.OpenGL复现数据) {
            (void)写入基础节点指针(宿主, 特征_OpenGL复现数据(), 上下文.OpenGL复现数据, now);
            (void)写入基础节点I64(宿主, 特征_复现数据状态(), OpenGL复现数据状态_已生成, now);
        }
        if (动态) {
            (void)写入基础节点指针(宿主, 特征_动作动态(), 动态, now);
        }
        if (失败原因) {
            (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
        }
    }

}

export namespace 自我动作实现模块::外设模块 {
    inline bool 确认双目相机控制规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now,
        bool 目标为可用) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标外设(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标外设(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_目标外设()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设可用性()) && ok;
            ok = 取或创建子特征(宿主, 特征_启停状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_占用状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_运行状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        if (目标为可用) {
            ok = 确保方法结果能力I64(方法首节点, 特征_外设可用性(), 1) && ok;
            ok = 确保方法结果能力I64(方法首节点, 特征_启停状态(), 1) && ok;
        } else {
            ok = 确保方法结果能力I64(方法首节点, 特征_占用状态(), 0) && ok;
            ok = 确保方法结果能力I64(方法首节点, 特征_启停状态(), 0) && ok;
            ok = 确保方法结果能力I64(方法首节点, 特征_外设可用性(), 0) && ok;
        }
        return ok;
    }

    inline bool 确认双目相机检查运行状态规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标外设(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标外设(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_目标外设()) && ok;
            ok = 取或创建子特征(宿主, 特征_运行状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设可用性()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力方向(
            方法首节点,
            特征_运行状态(),
            枚举_结果变化方向::已识别) && ok;
        return ok;
    }

    inline bool 确认双目相机刷新当前观察场景规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_外设可用性(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_运行状态(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标外设(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设可用性(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_运行状态(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标外设(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标宿主(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_目标外设()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标宿主()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前观察场景()) && ok;
            ok = 取或创建子特征(宿主, 特征_相机帧宽度()) && ok;
            ok = 取或创建子特征(宿主, 特征_相机帧高度()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度帧号()) && ok;
            ok = 取或创建子特征(宿主, 特征_彩色帧号()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_当前观察场景(), 1) && ok;
        return ok;
    }

    inline bool 确认双目相机取得当前观察帧规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_外设可用性(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_运行状态(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标外设(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设可用性(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_运行状态(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标外设(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标宿主(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_目标外设()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标宿主()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前观察帧()) && ok;
            ok = 取或创建子特征(宿主, 特征_相机帧宽度()) && ok;
            ok = 取或创建子特征(宿主, 特征_相机帧高度()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度帧号()) && ok;
            ok = 取或创建子特征(宿主, 特征_彩色帧号()) && ok;
            ok = 取或创建子特征(宿主, 特征_预期像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素特征数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度有效像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_点云有效像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素覆盖状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间坐标单位毫米()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_当前观察帧(), 1) && ok;
        return ok;
    }

    inline bool 确认双目相机取得当前观察特征帧规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认双目相机取得当前观察帧规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_当前观察特征帧()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前观察特征帧取得状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_颜色RGB结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_原始深度毫米结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_滤波深度毫米结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_补全深度毫米结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度有效性Mask状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度来源结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度稳定性结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度邻域一致性结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察帧组状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察帧组帧数()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合依据帧集合状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合深度毫米结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合深度有效性结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_多帧深度稳定性结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_每像素深度方差结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合深度有效像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合深度有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合补偿深度空洞像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合低稳定像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合深度稳定性平均值()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合深度平均方差()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间坐标毫米XYZ结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_原始深度来源像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_滤波深度来源像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_补全深度来源像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_无有效深度来源像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_补全深度低置信像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度稳定性平均值()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度邻域一致性平均值()) && ok;
            ok = 取或创建子特征(宿主, 特征_彩色深度已对齐()) && ok;
            ok = 取或创建子特征(宿主, 特征_对齐目标()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间坐标有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_帧质量评分()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察帧质量诊断摘要()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度空洞数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度空洞区域数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度空洞区域集合()) && ok;
            ok = 取或创建子特征(宿主, 特征_诊断区域数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_诊断区域集合状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_诊断区域掩码状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_彩色深度对齐状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓闭合率()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓断裂数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_彩图轮廓数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度轮廓数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间投影轮廓数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合轮廓数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓来源分层状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓深度支持率()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓颜色支持率()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓空间支持率()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合轮廓置信度()) && ok;
            ok = 取或创建子特征(宿主, 特征_跨越深度断裂轮廓数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_穿过深度无效区域轮廓数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_依赖补全深度轮廓数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间点有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_边界深度稳定率()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选存在验证通过率()) && ok;
            ok = 取或创建子特征(宿主, 特征_未解释区域比例()) && ok;
            ok = 取或创建子特征(宿主, 特征_补观察缺口状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_补观察缺失原因()) && ok;
            ok = 取或创建子特征(宿主, 特征_待补观察区域数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_补全候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_低置信轮廓数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_条件不足候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_待验证候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_部分确认候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_补观察需求建议()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_当前观察特征帧(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_当前观察特征帧取得状态(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_观察帧质量诊断摘要(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_观察帧组状态(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_融合深度毫米结构状态(), 1) && ok;
        return ok;
    }

    inline bool 确认双目相机建立当前帧像素特征索引规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认双目相机取得当前观察帧规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_当前帧像素特征索引()) && ok;
            ok = 取或创建子特征(宿主, 特征_颜色像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_帧解释状态()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_当前帧像素特征索引(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_像素覆盖状态(), 1) && ok;
        return ok;
    }

    inline bool 确认自我建立观察帧存在规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认双目相机取得当前观察特征帧规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_当前观察帧存在()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前观察帧存在状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察帧容器状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_相机帧宽度()) && ok;
            ok = 取或创建子特征(宿主, 特征_相机帧高度()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度帧号()) && ok;
            ok = 取或创建子特征(宿主, 特征_彩色帧号()) && ok;
            ok = 取或创建子特征(宿主, 特征_预期像素数量()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_当前观察帧存在(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_当前观察帧存在状态(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_观察帧容器状态(), 1) && ok;
        return ok;
    }

    inline bool 确认自我建立观察帧到自我场景接口规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我建立观察帧存在规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_观察帧到自我场景接口()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_观察帧到自我场景接口(), 1) && ok;
        return ok;
    }

    inline bool 确认自我建立像素观察事实索引规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我建立观察帧存在规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_当前帧像素特征索引()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素观察事实索引()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素观察存在数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_缺失像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_重复映射数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素覆盖状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_坐标回查可用状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_帧内编号回查可用状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_颜色像素数量()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_像素观察事实索引(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_像素覆盖状态(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_坐标回查可用状态(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_帧内编号回查可用状态(), 1) && ok;
        return ok;
    }

    inline bool 确认自我承接像素特征值规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我建立像素观察事实索引规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_颜色像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度有效像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_点云有效像素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_颜色RGB结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_原始深度毫米结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度有效性Mask状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间坐标毫米XYZ结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素特征值承接状态()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_像素特征值承接状态(), 1) && ok;
        return ok;
    }

    inline bool 确认自我校验观察事实完备性规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我承接像素特征值规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_观察事实完备状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察事实完备失败条件数量()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_观察事实完备状态(), 1) && ok;
        return ok;
    }

    inline bool 确认自我评估观察帧质量规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我校验观察事实完备性规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_质量可用状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_质量失败条件数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察帧质量诊断摘要()) && ok;
            ok = 取或创建子特征(宿主, 特征_帧质量评分()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间坐标有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_融合深度有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_未解释区域比例()) && ok;
            ok = 取或创建子特征(宿主, 特征_补观察缺口状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_补观察缺失原因()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_质量可用状态(), 1) && ok;
        return ok;
    }

    inline bool 确认自我判断基础观察事实可用性规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我评估观察帧质量规格(方法首节点, 方法存在, now);
        // 该自我判定消费已承接的观察事实，不直接控制或检查相机；
        // 继承自相机使用规格的外设状态只作可选证据，不能阻塞本方法输入闭合。
        ok = 声明形参规格(方法首节点, 特征_外设可用性(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_运行状态(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设可用性(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_运行状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_当前观察帧采集时间()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前观察帧观察年龄()) && ok;
            ok = 取或创建子特征(宿主, 特征_最大允许观察年龄()) && ok;
            ok = 取或创建子特征(宿主, 特征_新鲜度判定时间()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察帧失效状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察帧来源外设状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前观察新鲜度状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_新鲜度失败原因()) && ok;
            ok = 取或创建子特征(宿主, 特征_基础观察事实可用状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_基础观察事实失败条件数量()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_基础观察事实可用状态(), 1) && ok;
        return ok;
    }

    inline bool 确认安全评估当前场景安全性规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源需求(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源因果信息(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_基础观察事实可用状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_场景存在集合明确状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_关键特征变化观察状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_安全因果候选构造状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_基础风险判断状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险状态明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景安全度明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景最低安全度存在明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景各存在安全评估明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景各存在特征值可读状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值明确范围类型()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值缺口可解释状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值可读材料门版本()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值可读存在数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值部分可读存在数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值证据不足存在数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值缺失原因掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值过期原因掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值冲突原因掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景持续观察需求明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_负向影响处置需求明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景安全度候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景评估安全值候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景直接伤害可能性候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全_场景影响部分候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全_场景影响部分可结算状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全_场景影响部分不可结算原因()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全_场景影响部分已入账状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全_场景影响部分变化方向()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全层因素集合版本()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全层因素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全层缺失因素数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全层明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全层可聚合状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全层已入账状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险因素未搜索掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险因素搜索缺口掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险因素负向证据掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险因素默认满足掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险因素已入账掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险因素证据搜索未完成数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险因素默认满足数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全层候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全层投影候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景最低安全度存在()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景安全评估置信度()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景安全判定状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景安全判定来源()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在直接伤害可能性候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在风险安全值候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在安全度候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_历史直接伤害归因次数()) && ok;
            ok = 取或创建子特征(宿主, 特征_最近直接伤害归因时间()) && ok;
            ok = 取或创建子特征(宿主, 特征_无伤害共处时长()) && ok;
            ok = 取或创建子特征(宿主, 特征_伤害可能性下界候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在安全评估置信度()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在安全评估证据不足原因()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在安全判定状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在安全判定来源()) && ok;
            ok = 取或创建子特征(宿主, 特征_基础风险未判断因子数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_基础风险证据完整度()) && ok;
            ok = 取或创建子特征(宿主, 特征_基础风险不确定度()) && ok;
            ok = 取或创建子特征(宿主, 特征_基础风险值计算状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_基础风险评估范围状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全评估证据不足原因()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_基础风险判断状态(), 基础风险判断状态_已明确) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_风险状态明确状态(), 安全明确状态_已明确) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_当前场景安全度明确状态(), 安全明确状态_已明确) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_当前场景最低安全度存在明确状态(), 安全明确状态_已明确) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_当前场景各存在安全评估明确状态(), 安全明确状态_已明确) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_当前场景安全判定状态(), 安全判定状态_安全) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_当前场景安全判定状态(), 安全判定状态_不安全) && ok;
        return ok;
    }

    inline bool 确认提交风险安全场景影响部分状态变化规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_风险安全_场景影响部分候选(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_风险安全_场景影响部分可结算状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_风险安全_场景影响部分不可结算原因(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_风险安全_场景影响部分提交状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全_场景影响部分提交拒绝原因()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险安全_场景影响部分已入账状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_自我_风险安全_场景影响部分()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        return ok;
    }

    inline bool 确认安全搜索安全因果因素证据规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_安全因果层级(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_安全因果因素ID(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_安全因果证据方向(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标安全因果信息(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_安全因果搜索时间窗口(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_安全因果层级(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_安全因果因素ID(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_安全因果证据方向(), now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_安全因果因素证据搜索状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素搜索范围完整状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素负向证据数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素正向证据数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素稳定性证据数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素归因证据数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素材料缺失数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素冲突数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素搜索置信度()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素默认满足允许状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素评估值候选()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_安全因果因素证据搜索状态(),
            安全因果因素搜索状态_已完成) && ok;
        return ok;
    }

    inline bool 确认提交安全因果因素无负证据默认满足规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_安全因果因素证据搜索状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_安全因果因素默认满足允许状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_安全因果因素默认满足提交状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素值()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素已入账状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        return ok;
    }

    inline bool 确认提交安全因果因素证据评估值规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_安全因果因素证据搜索状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_安全因果因素评估值候选(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_安全因果因素评估值提交状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素值()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果因素已入账状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        return ok;
    }

    inline bool 确认自我读取自我关键特征变化规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_场景存在集合明确状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_关键特征变化观察状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_关键特征观察样本数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_关键特征负向变化数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_关键特征变化观察状态(),
            安全明确状态_已明确) && ok;
        return ok;
    }

    inline bool 确认安全构造场景影响因果候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_场景存在集合明确状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_关键特征变化观察状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_安全因果候选构造状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果候选低置信数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果候选归因置信度()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果候选材料缺口数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全因果候选搜索未完成数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_安全因果候选构造状态(),
            安全明确状态_已明确) && ok;
        return ok;
    }

    inline bool 确认自我明确当前场景持续观察需求规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_当前场景持续观察需求明确状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_当前场景持续观察需求明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_当前场景持续观察需求明确状态(),
            安全明确状态_已明确) && ok;
        return ok;
    }

    inline bool 确认安全持续观察当前场景规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_当前场景持续观察需求明确状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_场景存在集合明确状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_当前场景持续观察状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景持续观察样本数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_当前场景持续观察状态(),
            安全明确状态_已明确) && ok;
        return ok;
    }

    inline bool 确认自我明确负向影响处置需求规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_负向影响处置需求明确状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_当前场景持续观察状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_负向影响处置需求明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_负向影响处置需求明确状态(),
            安全明确状态_已明确) && ok;
        return ok;
    }

    inline bool 确认安全执行降级保护规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_负向影响处置需求明确状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_安全影响因果明确状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_降级保护状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_降级保护状态(),
            安全明确状态_已明确) && ok;
        return ok;
    }

    inline bool 确认自我验证外设观察像素簇候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标外设(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_当前观察特征帧取得状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_基础观察事实可用状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_空间候选集合(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_像素归属验证状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_外设观察报告ID()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察材料可回查状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设像素簇候选验证状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察像素簇候选已验证状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前观察候选存在验证状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_已确认观察存在生成条件明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察验证材料明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察验证候选簇数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察验证通过簇数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察验证失败簇数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察验证证据不足簇数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察验证缺失材料数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察验证缺失材料原因()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_外设观察像素簇候选已验证状态(),
            外设观察候选验证状态_已验证) && ok;
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_外设像素簇候选验证状态(),
            外设观察候选验证状态_已验证) && ok;
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_外设观察材料可回查状态(),
            1) && ok;
        return ok;
    }

    inline bool 确认自我确认外设观察存在候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设像素簇候选验证状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设观察像素簇候选已验证状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设观察验证材料明确状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设观察材料可回查状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_外设观察报告ID()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察存在候选确认状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察存在候选确认方案取得状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察存在候选确认新建数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察存在候选确认合并数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察存在候选确认冲突数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察存在候选确认证据不足数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设观察存在候选确认可提交数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_外设观察存在候选确认方案取得状态(),
            1) && ok;
        return ok;
    }

    inline bool 确认提交确认观察存在事实规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设观察存在候选确认方案取得状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设观察存在候选确认可提交数量(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_外设观察存在事实提交状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_已确认观察存在提交入账状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察存在原始事实写入状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察存在与当前场景关联状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_已验证观察存在数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_场景存在集合明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景存在集合明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景各存在特征值可读状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值明确范围类型()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值缺口可解释状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值可读材料门版本()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值可读存在数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值部分可读存在数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值证据不足存在数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值缺失原因掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值过期原因掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前场景特征值冲突原因掩码()) && ok;
            ok = 取或创建子特征(宿主, 特征_外设_D455到场景绝对坐标变换明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        return ok;
    }

    inline bool 确认自我按已确认轮廓分区当前帧规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_场景存在集合明确状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_观察存在原始事实写入状态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_外设观察报告ID()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前帧轮廓分区状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_已知存在当前帧扫描状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_未知区域识别状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前帧遮挡判断状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前帧轮廓冲突区域明确状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前帧像素归属闭合状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前帧区域归属状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_已知存在扫描区数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_新识别区数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_冲突区数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_待重识别区数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_当前帧轮廓分区状态(),
            当前帧轮廓分区状态_已生成) && ok;
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_当前帧区域归属状态(),
            当前帧区域归属状态_已识别区域) && ok;
        return ok;
    }

    inline bool 确认自我补观察质量占位规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标外设(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_质量可用状态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_质量失败条件数量(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_深度有效率(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_空间坐标有效率(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标外设(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标宿主(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_补观察缺口状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_补观察需求建议()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_补观察缺口状态(), 1) && ok;
        return ok;
    }

    inline bool 确认自我诊断观察质量缺口并派生补观察需求规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标外设(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_深度有效率(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_空间坐标有效率(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_质量失败条件数量(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标外设(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标宿主(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_质量诊断状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间坐标有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度观察质量状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间坐标观察质量状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_质量缺口状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_质量缺口原因集合()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间类后续处理适用状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_风险判断适用状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在确认适用状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察质量改善线索状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察质量改善候选线索()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_质量诊断状态(), 观察质量诊断状态_已完成) && ok;
        return ok;
    }

    inline bool 确认自我调整相机参数并重采样规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 确认自我补观察质量占位规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我按ROI重算观察质量规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 确认自我补观察质量占位规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我执行多帧重采样并择优规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 确认自我补观察质量占位规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我生成低置信补偿观察候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 确认自我补观察质量占位规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我调整外设观察姿态规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 确认自我补观察质量占位规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我调整观察位置规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 确认自我补观察质量占位规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我按像素坐标回查观察事实规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我建立像素观察事实索引规格(方法首节点, 方法存在, now);
        ok = 声明形参规格(方法首节点, 特征_像素坐标X(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_像素坐标Y(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_像素坐标X(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_像素坐标Y(), now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_像素坐标对应像素观察事实()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素坐标X()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素坐标Y()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_像素坐标对应像素观察事实(), 1) && ok;
        return ok;
    }

    inline bool 确认自我提取空间候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_外设可用性(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_运行状态(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标外设(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设可用性(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_运行状态(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标外设(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标宿主(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_目标外设()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标宿主()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前观察帧()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前帧像素特征索引()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间候选集合()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间候选有效点数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_中心空间坐标X()) && ok;
            ok = 取或创建子特征(宿主, 特征_中心空间坐标Y()) && ok;
            ok = 取或创建子特征(宿主, 特征_中心空间坐标Z()) && ok;
            ok = 取或创建子特征(宿主, 特征_范围坐标AABB最小X()) && ok;
            ok = 取或创建子特征(宿主, 特征_范围坐标AABB最大X()) && ok;
            ok = 取或创建子特征(宿主, 特征_范围坐标AABB最小Y()) && ok;
            ok = 取或创建子特征(宿主, 特征_范围坐标AABB最大Y()) && ok;
            ok = 取或创建子特征(宿主, 特征_范围坐标AABB最小Z()) && ok;
            ok = 取或创建子特征(宿主, 特征_范围坐标AABB最大Z()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间连续性评分()) && ok;
            ok = 取或创建子特征(宿主, 特征_范围稳定性评分()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_空间候选集合(), 1) && ok;
        return ok;
    }

    inline bool 确认自我判断空间候选相对独立性规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我提取空间候选规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_空间候选相对独立性判断()) && ok;
            ok = 取或创建子特征(宿主, 特征_相对独立空间候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_不独立空间候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_待拆分空间候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_待合并空间候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_不确定空间候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_主空间候选相对独立状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_主空间候选分离度评分()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_空间候选相对独立性判断(), 1) && ok;
        return ok;
    }

    inline bool 确认自我生成空间候选观察队列规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我判断空间候选相对独立性规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_空间候选观察队列()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察队列候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_首选观察候选编号()) && ok;
            ok = 取或创建子特征(宿主, 特征_首选观察候选优先级评分()) && ok;
            ok = 取或创建子特征(宿主, 特征_首选观察候选相对独立状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_首选观察候选中心接近评分()) && ok;
            ok = 取或创建子特征(宿主, 特征_首选观察候选范围大小评分()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_空间候选观察队列(), 1) && ok;
        return ok;
    }

    inline bool 确认自我观察单个空间候选并组合存在假设规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我生成空间候选观察队列规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_观察存在假设()) && ok;
            ok = 取或创建子特征(宿主, 特征_来源空间候选编号()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在假设验证状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选像素掩码状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_投影轮廓状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_假设距离()) && ok;
            ok = 取或创建子特征(宿主, 特征_假设尺寸X()) && ok;
            ok = 取或创建子特征(宿主, 特征_假设尺寸Y()) && ok;
            ok = 取或创建子特征(宿主, 特征_假设尺寸Z()) && ok;
            ok = 取或创建子特征(宿主, 特征_假设方位X()) && ok;
            ok = 取或创建子特征(宿主, 特征_假设方位Y()) && ok;
            ok = 取或创建子特征(宿主, 特征_假设有效点比例()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓支持评分()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_观察存在假设(), 1) && ok;
        return ok;
    }

    inline bool 确认自我用空间范围投影覆盖原帧验证像素归属规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我观察单个空间候选并组合存在假设规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_像素归属验证结果()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间覆盖像素数()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间匹配像素数()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间冲突像素数()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓支持像素数()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓冲突像素数()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度一致率()) && ok;
            ok = 取或创建子特征(宿主, 特征_范围内点比例()) && ok;
            ok = 取或创建子特征(宿主, 特征_投影覆盖率()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓吻合率()) && ok;
            ok = 取或创建子特征(宿主, 特征_遮挡像素数()) && ok;
            ok = 取或创建子特征(宿主, 特征_未解释像素数()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素归属验证状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察确认补偿状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_缺失原因()) && ok;
            ok = 取或创建子特征(宿主, 特征_未解释区域状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_待补观察区域数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_补全候选状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_低置信轮廓状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_补观察需求建议()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_像素归属验证结果(), 1) && ok;
        return ok;
    }

    inline bool 确认自我确认观察存在并更新帧解释状态规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = 确认自我用空间范围投影覆盖原帧验证像素归属规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_已验证观察存在()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察存在确认状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_已验证观察存在数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素归属更新()) && ok;
            ok = 取或创建子特征(宿主, 特征_未解释区域更新()) && ok;
            ok = 取或创建子特征(宿主, 特征_帧解释状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_全帧像素归属账状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_已归属像素数()) && ok;
            ok = 取或创建子特征(宿主, 特征_未解释像素数()) && ok;
            ok = 取或创建子特征(宿主, 特征_归属冲突像素数()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素归属率()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_已验证观察存在(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_帧解释状态(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_全帧像素归属账状态(), 全帧像素归属账状态_已生成) && ok;
        return ok;
    }

    inline bool 确认自我内部世界基础规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标存在(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标宿主(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_目标存在()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_内部世界(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_内部世界状态(), 内部世界状态_已建立) && ok;
        return ok;
    }

    inline bool 确认自我确保存在内部世界规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 确认自我内部世界基础规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我建立存在内外场景接口规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        bool ok = 确认自我内部世界基础规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_存在内外场景接口()) && ok;
            ok = 取或创建子特征(宿主, 特征_接口状态()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_存在内外场景接口(), 1) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_接口状态(), 内外接口状态_已建立) && ok;
        return ok;
    }

    inline bool 确认自我将存在局部观察切片映射到内部世界规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        bool ok = 确认自我建立存在内外场景接口规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_内部世界当前观察材料()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在内部世界局部材料建立状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界局部轮廓材料可回查状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_局部观察切片()) && ok;
            ok = 取或创建子特征(宿主, 特征_局部边界框宽度()) && ok;
            ok = 取或创建子特征(宿主, 特征_局部边界框高度()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_内部世界当前观察材料(), 当前观察材料状态_已建立) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_存在内部世界局部材料建立状态(), 内部世界局部材料建立状态_已建立) && ok;
        return ok;
    }

    inline bool 确认自我在内部世界提取子候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        bool ok = 确认自我将存在局部观察切片映射到内部世界规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_内部世界子候选集合()) && ok;
            ok = 取或创建子特征(宿主, 特征_子候选类型()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界子候选验证状态()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_内部世界子候选集合(), 子候选集合状态_已生成) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_内部世界子候选验证状态(), 内部世界子候选验证状态_待验证) && ok;
        return ok;
    }

    inline bool 确认自我确认内部世界子存在规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        bool ok = 确认自我在内部世界提取子候选规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_内部世界子存在候选确认状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界子存在候选确认方案取得状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界子存在候选确认方案类型()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界子存在候选确认允许提交状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界子存在候选确认新建数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界子存在候选确认证据不足数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界子存在候选确认可提交数量()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_内部世界子存在候选确认方案取得状态(), 1) && ok;
        return ok;
    }

    inline bool 确认提交确认内部世界子存在事实规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        bool ok = 确认自我确认内部世界子存在规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_内部世界子存在事实提交状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界子存在原始事实写入状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_内部世界子存在与内部世界关联状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_成员存在集合()) && ok;
            ok = 取或创建子特征(宿主, 特征_子存在类型()) && ok;
        }
        return ok;
    }

    inline bool 确认自我递归细分存在内部世界规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        bool ok = 确认提交确认内部世界子存在事实规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_内部世界细分状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_细分层级数()) && ok;
        }
        ok = 确保方法结果能力方向(
            方法首节点,
            特征_内部世界细分状态(),
            枚举_结果变化方向::已识别) && ok;
        return ok;
    }

    inline bool 确认自我匹配当前观察存在与历史观察存在规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前观察存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_历史观察存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_当前观察存在(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_历史观察存在(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_观察存在匹配关系()) && ok;
            ok = 取或创建子特征(宿主, 特征_历史对应状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_中心坐标差()) && ok;
            ok = 取或创建子特征(宿主, 特征_范围坐标重叠率()) && ok;
            ok = 取或创建子特征(宿主, 特征_尺寸差()) && ok;
            ok = 取或创建子特征(宿主, 特征_匹配置信度()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力方向(
            方法首节点,
            特征_历史对应状态(),
            枚举_结果变化方向::已识别) && ok;
        return ok;
    }

    inline bool 确认自我比较存在特征变化规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        bool ok = 确认自我匹配当前观察存在与历史观察存在规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_观察存在变化结果()) && ok;
            ok = 取或创建子特征(宿主, 特征_变化状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_位置变化量()) && ok;
            ok = 取或创建子特征(宿主, 特征_范围变化量()) && ok;
            ok = 取或创建子特征(宿主, 特征_尺寸变化量()) && ok;
            ok = 取或创建子特征(宿主, 特征_距离变化量()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_观察存在变化结果(), 变化结果状态_已生成) && ok;
        return ok;
    }

    inline bool 确认自我生成观察变化事件规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        bool ok = 确认自我比较存在特征变化规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_观察变化事件集合()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察变化事件类型()) && ok;
            ok = 取或创建子特征(宿主, 特征_变化事件置信度()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_观察变化事件集合(), 观察变化事件集合状态_已生成) && ok;
        return ok;
    }

    inline bool 确认自我导出当前场景识别快照规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标宿主(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_场景识别快照()) && ok;
            ok = 取或创建子特征(宿主, 特征_快照状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_像素颜色层()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度层()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间点层()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间候选层()) && ok;
            ok = 取或创建子特征(宿主, 特征_已确认观察存在层()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在内部世界展开层()) && ok;
            ok = 取或创建子特征(宿主, 特征_变化事件层()) && ok;
            ok = 取或创建子特征(宿主, 特征_诊断摘要状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_空间坐标有效率()) && ok;
            ok = 取或创建子特征(宿主, 特征_帧质量评分()) && ok;
            ok = 取或创建子特征(宿主, 特征_观察帧质量诊断摘要()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度空洞数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度空洞区域数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓闭合率()) && ok;
            ok = 取或创建子特征(宿主, 特征_边界深度稳定率()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选存在验证通过率()) && ok;
            ok = 取或创建子特征(宿主, 特征_未解释区域比例()) && ok;
            ok = 取或创建子特征(宿主, 特征_补观察缺口状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_补观察缺失原因()) && ok;
            ok = 取或创建子特征(宿主, 特征_待补观察区域数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_补全候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_低置信轮廓数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_条件不足候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_待验证候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_部分确认候选数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_补观察需求建议()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_场景识别快照(), 场景识别快照状态_已生成) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_快照状态(), 场景识别快照状态_已生成) && ok;
        return ok;
    }

    inline bool 确认自我导出OpenGL复现数据规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        bool ok = 确认自我导出当前场景识别快照规格(方法首节点, 方法存在, now);
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_OpenGL复现数据()) && ok;
            ok = 取或创建子特征(宿主, 特征_复现数据状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_顶点缓冲()) && ok;
            ok = 取或创建子特征(宿主, 特征_颜色缓冲()) && ok;
            ok = 取或创建子特征(宿主, 特征_深度缓冲()) && ok;
            ok = 取或创建子特征(宿主, 特征_存在ID缓冲()) && ok;
            ok = 取或创建子特征(宿主, 特征_AABB绘制状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_绘制顶点数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_绘制线段数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_绘制点数量()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_OpenGL复现数据(), OpenGL复现数据状态_已生成) && ok;
        ok = 确保方法结果能力I64(方法首节点, 特征_复现数据状态(), OpenGL复现数据状态_已生成) && ok;
        return ok;
    }

    inline bool 确认双目相机发现候选存在规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_外设可用性(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_运行状态(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标外设(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_外设可用性(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_运行状态(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标外设(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标宿主(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_目标外设()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标宿主()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选存在集合()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选存在数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_轮廓数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(方法首节点, 特征_候选存在集合(), 1) && ok;
        return ok;
    }
    inline const 语素入口节点类* 名称_双目相机外设() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("双目相机", 枚举_信息入口类型::存在实例入口);
        return s_词;
    }

    inline const 语素入口节点类* 类型_外设存在() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("外设存在", 枚举_信息入口类型::存在概念入口);
        return s_词;
    }

    inline 场景节点类* 双目相机实际输出场景(
        方法类::节点类* 方法首节点,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept
    {
        if (输出结果场景) {
            return 输出结果场景;
        }
        if (auto* 场景 = 方法类::取或创建_方法结果场景(方法首节点, nullptr, now)) {
            return 场景;
        }
        return 世界树.取或创建自我现实场景();
    }

    inline 存在节点类* 取或创建双目相机外设存在(场景节点类* 场景) noexcept
    {
        auto* 宿主 = reinterpret_cast<基础信息节点类*>(
            场景 ? 场景 : 世界树.取或创建自我现实场景());
        if (!宿主) {
            return nullptr;
        }
        return 世界树.存在().取或创建子存在_按名称类型(
            宿主,
            名称_双目相机外设(),
            类型_外设存在());
    }

    inline 存在节点类* 解析双目相机目标外设(
        场景节点类* 输入参数场景,
        场景节点类* 输出场景) noexcept
    {
        void* 指针 = nullptr;
        if (读取输入参数指针(输入参数场景, 特征_目标外设(), 指针)) {
            return reinterpret_cast<存在节点类*>(指针);
        }
        return 取或创建双目相机外设存在(输出场景);
    }

    inline 基础信息节点类* 解析双目相机目标宿主(
        场景节点类* 输入参数场景,
        基础信息节点类* 默认宿主) noexcept
    {
        void* 指针 = nullptr;
        if (读取输入参数指针(输入参数场景, 特征_目标宿主(), 指针)) {
            return reinterpret_cast<基础信息节点类*>(指针);
        }
        return 默认宿主;
    }

    inline 动态节点类* 写入双目相机I64事实与动态(
        方法类::节点类* 方法首节点,
        场景节点类* 场景,
        基础信息节点类* 目标宿主,
        存在节点类* 双目相机存在,
        const 语素入口节点类* 特征类型,
        I64 值,
        bool 动作达成,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        存在节点类* 运行存在,
        时间戳 now) noexcept
    {
        auto* 宿主 = 目标宿主 ? 目标宿主 : reinterpret_cast<基础信息节点类*>(双目相机存在);
        if (!方法首节点 || !场景 || !宿主 || !特征类型) {
            return nullptr;
        }

        (void)写入基础节点I64(宿主, 特征类型, 值, now);
        auto* 状态特征 = 世界树.取或创建子特征_按类型(宿主, 特征类型);
        if (!状态特征) {
            return nullptr;
        }

        (void)世界树.状态().创建内部状态(
            场景,
            宿主,
            状态特征,
            特征值{ 值 },
            枚举_存在状态事件::变化,
            true,
            now);

        auto* 动态 = 世界树.动态().创建方法动作动态(
            场景,
            宿主,
            状态特征,
            reinterpret_cast<方法节点类*>(方法首节点),
            方法首节点->主信息.首节点信息().动作名,
            now,
            now,
            动作达成,
            动作达成 ? 0 : -1,
            动作达成 ? 枚举_动作事件相位::完成 : 枚举_动作事件相位::失败,
            输入参数场景,
            输出结果场景);
        if (动态) {
            auto* 动态宿主 = reinterpret_cast<基础信息节点类*>(动态);
            (void)写入基础节点指针(动态宿主, 特征_目标外设(), 双目相机存在, now);
            (void)写入基础节点指针(动态宿主, 特征_目标宿主(), 宿主, now);
            (void)写入基础节点指针(动态宿主, 特征_目标特征类型(), 特征类型, now);
            (void)写入基础节点指针(动态宿主, 特征_当前方法运行存在(), 运行存在, now);
            (void)写入基础节点指针(动态宿主, 特征_输入参数场景(), 输入参数场景, now);
            (void)写入基础节点指针(动态宿主, 特征_输出结果场景(), 输出结果场景, now);
        }
        return 动态;
    }

    inline 状态节点类* 写入输出结果I64状态(
        场景节点类* 结果场景,
        基础信息节点类* 目标宿主,
        const 语素入口节点类* 特征类型,
        I64 值,
        时间戳 now) noexcept
    {
        if (!结果场景 || !目标宿主 || !特征类型) {
            return nullptr;
        }

        auto* 状态特征 = 世界树.取或创建子特征_按类型(目标宿主, 特征类型);
        if (!状态特征) {
            return nullptr;
        }

        return 世界树.状态().创建内部状态(
            结果场景,
            目标宿主,
            状态特征,
            特征值{ 值 },
            枚举_存在状态事件::变化,
            true,
            now);
    }

    inline void 写入双目相机通用结果(
        基础信息节点类* 宿主,
        bool 成功,
        存在节点类* 双目相机存在,
        基础信息节点类* 目标宿主,
        动态节点类* 动态,
        const 语素入口节点类* 失败原因,
        时间戳 now) noexcept
    {
        if (!宿主) return;
        (void)写入方法执行情况(宿主, 成功 ? 值_执行成功() : 值_执行失败(), now);
        if (双目相机存在) {
            (void)写入基础节点指针(宿主, 特征_目标外设(), 双目相机存在, now);
        }
        if (目标宿主) {
            (void)写入基础节点指针(宿主, 特征_目标宿主(), 目标宿主, now);
        }
        if (动态) {
            (void)写入基础节点指针(宿主, 特征_动作动态(), 动态, now);
        }
        if (失败原因) {
            (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
        }
    }

    inline 存在节点类* 使双目相机达到可用状态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::双目相机_使达到可用状态,
            输出结果场景,
            now,
            "双目相机_使达到可用状态",
            [](方法类::节点类* 方法, 存在节点类* 存在, 时间戳 时间) noexcept {
                return 确认双目相机控制规格(方法, 存在, 时间, true);
            });
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = reinterpret_cast<基础信息节点类*>(相机存在);
        const auto 结果 = 打开双目相机适配器();
        const bool 成功 = 结果.成功;

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_外设可用性(),
            成功 ? 1 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);
        if (相机存在) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(相机存在);
            (void)写入基础节点I64(宿主, 特征_启停状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(
                宿主,
                特征_运行状态(),
                成功 ? static_cast<I64>(运行状态::运行中) : static_cast<I64>(运行状态::故障),
                now);
        }

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            if (!宿主) return;
            (void)写入基础节点I64(宿主, 特征_外设可用性(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_启停状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(
                宿主,
                特征_运行状态(),
                成功 ? static_cast<I64>(运行状态::运行中) : static_cast<I64>(运行状态::故障),
                now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "双目相机_使达到可用状态",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 释放双目相机(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::双目相机_释放,
            输出结果场景,
            now,
            "双目相机_释放",
            [](方法类::节点类* 方法, 存在节点类* 存在, 时间戳 时间) noexcept {
                return 确认双目相机控制规格(方法, 存在, 时间, false);
            });
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = reinterpret_cast<基础信息节点类*>(相机存在);
        const auto 结果 = 释放双目相机适配器();
        const bool 成功 = 结果.成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_占用状态(),
            0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);
        if (相机存在) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(相机存在);
            (void)写入基础节点I64(宿主, 特征_外设可用性(), 0, now);
            (void)写入基础节点I64(宿主, 特征_启停状态(), 0, now);
            (void)写入基础节点I64(宿主, 特征_运行状态(), static_cast<I64>(运行状态::停止), now);
        }

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            if (!宿主) return;
            (void)写入基础节点I64(宿主, 特征_占用状态(), 0, now);
            (void)写入基础节点I64(宿主, 特征_外设可用性(), 0, now);
            (void)写入基础节点I64(宿主, 特征_启停状态(), 0, now);
            (void)写入基础节点I64(宿主, 特征_运行状态(), static_cast<I64>(运行状态::停止), now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "双目相机_释放",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 检查双目相机运行状态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::双目相机_检查运行状态,
            输出结果场景,
            now,
            "双目相机_检查运行状态",
            确认双目相机检查运行状态规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = reinterpret_cast<基础信息节点类*>(相机存在);
        const auto 结果 = 检查双目相机适配器状态();
        const I64 运行状态值 = 结果.相机已打开
            ? static_cast<I64>(运行状态::运行中)
            : (结果.失败原因 == 值_外设运行时缺失()
                ? static_cast<I64>(运行状态::故障)
                : static_cast<I64>(运行状态::停止));
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_运行状态(),
            运行状态值,
            true,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);
        if (相机存在) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(相机存在);
            (void)写入基础节点I64(宿主, 特征_外设可用性(), 结果.相机已打开 ? 1 : 0, now);
        }

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, true, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            if (!宿主) return;
            (void)写入基础节点I64(宿主, 特征_运行状态(), 运行状态值, now);
            (void)写入基础节点I64(宿主, 特征_外设可用性(), 结果.相机已打开 ? 1 : 0, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "双目相机_检查运行状态",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        更新方法运行账(环境.方法首节点, 输出运行, true, now);
        return 输出运行;
    }

    inline 存在节点类* 用双目相机刷新当前观察场景(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::双目相机_刷新当前观察场景,
            输出结果场景,
            now,
            "双目相机_刷新当前观察场景",
            确认双目相机刷新当前观察场景规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        const auto 结果 = 采集双目相机帧();
        const bool 成功 = 结果.成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_当前观察场景(),
            成功 ? 1 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            if (!宿主) return;
            (void)写入基础节点I64(宿主, 特征_当前观察场景(), 成功 ? 1 : 0, now);
            if (成功) {
                (void)写入基础节点I64(宿主, 特征_相机帧宽度(), static_cast<I64>(结果.宽度), now);
                (void)写入基础节点I64(宿主, 特征_相机帧高度(), static_cast<I64>(结果.高度), now);
                (void)写入基础节点I64(宿主, 特征_深度帧号(), static_cast<I64>(结果.深度帧号), now);
                (void)写入基础节点I64(宿主, 特征_彩色帧号(), static_cast<I64>(结果.彩色帧号), now);
                (void)写入基础节点I64(宿主, 特征_轮廓数量(), static_cast<I64>(结果.轮廓数量), now);
            }
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "双目相机_刷新当前观察场景",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 用双目相机取得当前观察帧(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::双目相机_取得当前观察帧,
            输出结果场景,
            now,
            "双目相机_取得当前观察帧",
            确认双目相机取得当前观察帧规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        const auto 结果 = 采集双目相机帧();
        const bool 成功 = 结果.成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_当前观察帧(),
            成功 ? 1 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入双目相机观察帧摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "双目相机_取得当前观察帧",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 用双目相机取得当前观察特征帧(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::双目相机_取得当前观察特征帧,
            输出结果场景,
            now,
            "双目相机_取得当前观察特征帧",
            确认双目相机取得当前观察特征帧规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        const auto 结果 = 采集双目相机帧();
        const bool 成功 = 结果.当前观察特征帧取得状态 == 观察事实状态_已取得;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_当前观察特征帧取得状态(),
            结果.当前观察特征帧取得状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入双目相机观察帧摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "双目相机_取得当前观察特征帧",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_当前观察特征帧取得状态(),
            "当前观察特征帧取得状态");
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我建立观察帧存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_建立观察帧存在,
            输出结果场景,
            now,
            "自我_建立观察帧存在",
            确认自我建立观察帧存在规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const bool 成功 = 结果.观察帧容器状态 == 观察事实状态_已建立;
        结果.成功 = 成功;
        if (!成功 && !结果.失败原因) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "当前观察特征帧不足以建立观察帧容器";
        }
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_观察帧容器状态(),
            结果.观察帧容器状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入自我观察帧存在摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_建立观察帧存在",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_观察帧容器状态(),
            "观察帧容器状态");
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我建立观察帧到自我场景接口(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_建立观察帧到自我场景接口,
            输出结果场景,
            now,
            "自我_建立观察帧到自我场景接口",
            确认自我建立观察帧到自我场景接口规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const bool 成功 = 结果.成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_观察帧到自我场景接口(),
            成功 ? 1 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入自我观察帧接口摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_建立观察帧到自我场景接口",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 用双目相机建立当前帧像素特征索引(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::双目相机_建立当前帧像素特征索引,
            输出结果场景,
            now,
            "双目相机_建立当前帧像素特征索引",
            确认双目相机建立当前帧像素特征索引规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const bool 成功 = 结果.成功 && 结果.像素覆盖状态 == 1;
        const auto* 失败原因 = 结果.失败原因
            ? 结果.失败原因
            : (成功 ? nullptr : 值_执行失败());
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_当前帧像素特征索引(),
            成功 ? 1 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 失败原因, now);
            写入双目相机像素特征索引摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        auto 记录结果 = 结果;
        记录结果.成功 = 成功;
        记录结果.失败原因 = 失败原因;
        记录双目相机本能动作结果(
            "双目相机_建立当前帧像素特征索引",
            记录结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我建立像素观察事实索引(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_建立像素观察事实索引,
            输出结果场景,
            now,
            "自我_建立像素观察事实索引",
            确认自我建立像素观察事实索引规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const bool 像素索引证据闭合 =
            结果.观察帧容器状态 == 观察事实状态_已建立
            && 结果.像素观察存在数量 == 结果.预期像素数量
            && 结果.缺失像素数量 == 0
            && 结果.重复映射数量 == 0
            && 结果.坐标回查可用状态 == 观察事实状态_可用
            && 结果.帧内编号回查可用状态 == 观察事实状态_可用;
        const bool 成功 = 结果.成功
            && 像素索引证据闭合
            && 结果.像素覆盖状态 == 观察事实状态_已覆盖;
        if (!成功 && !结果.失败原因) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "像素观察事实索引证据不足";
        }
        结果.成功 = 成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_像素覆盖状态(),
            结果.像素覆盖状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入自我像素观察事实索引摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        auto 记录结果 = 结果;
        记录结果.成功 = 成功;
        记录双目相机本能动作结果(
            "自我_建立像素观察事实索引",
            记录结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_像素覆盖状态(),
            "像素覆盖状态");
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我承接像素特征值(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_承接像素特征值,
            输出结果场景,
            now,
            "自我_承接像素特征值",
            确认自我承接像素特征值规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const bool 像素特征证据闭合 =
            结果.像素覆盖状态 == 观察事实状态_已覆盖
            && 结果.像素观察存在数量 == 结果.预期像素数量
            && 结果.颜色像素数量 == 结果.预期像素数量
            && 结果.颜色RGB结构状态 == 1
            && 结果.原始深度毫米结构状态 == 1
            && 结果.深度有效性Mask状态 == 1
            && 结果.空间坐标毫米XYZ结构状态 == 1;
        const bool 成功 = 结果.成功
            && 像素特征证据闭合
            && 结果.像素特征值承接状态 == 观察事实状态_已完成;
        if (!成功 && !结果.失败原因) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "像素特征值承接证据不足";
        }
        结果.成功 = 成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_像素特征值承接状态(),
            结果.像素特征值承接状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入自我像素特征值承接摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        auto 记录结果 = 结果;
        记录结果.成功 = 成功;
        记录双目相机本能动作结果(
            "自我_承接像素特征值",
            记录结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_像素特征值承接状态(),
            "像素特征值承接状态");
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我校验观察事实完备性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_校验观察事实完备性,
            输出结果场景,
            now,
            "自我_校验观察事实完备性",
            确认自我校验观察事实完备性规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        const bool 摘要闭合 = 读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const bool 完备证据闭合 =
            摘要闭合
            && 结果.像素覆盖状态 == 观察事实状态_已覆盖
            && 结果.像素特征值承接状态 == 观察事实状态_已完成
            && 结果.坐标回查可用状态 == 观察事实状态_可用
            && 结果.帧内编号回查可用状态 == 观察事实状态_可用;
        const bool 成功 = 完备证据闭合
            && 结果.观察事实完备状态 == 观察事实状态_已完备;
        if (!成功 && !结果.失败原因) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = std::string("观察事实完备性条件不足:") + 结果.观察事实完备失败原因摘要;
        }
        结果.成功 = 成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_观察事实完备状态(),
            结果.观察事实完备状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入自我观察事实完备摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        auto 记录结果 = 结果;
        记录结果.成功 = 成功;
        记录双目相机本能动作结果(
            "自我_校验观察事实完备性",
            记录结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_观察事实完备状态(),
            "观察事实完备状态");
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我评估观察帧质量(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_评估观察帧质量,
            输出结果场景,
            now,
            "自我_评估观察帧质量",
            确认自我评估观察帧质量规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        const bool 摘要闭合 = 读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const bool 质量证据闭合 =
            摘要闭合
            && 结果.观察帧质量诊断摘要 == 1
            && 结果.帧质量评分 >= 观察质量最低评分
            && 结果.深度有效率 >= 观察质量最低深度有效率
            && 结果.空间坐标有效率 >= 观察质量最低空间坐标有效率;
        const bool 成功 = 质量证据闭合
            && 结果.质量可用状态 == 观察事实状态_可用;
        if (!成功 && !结果.失败原因) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = std::string("观察帧质量条件不足:") + 结果.质量失败原因摘要;
        }
        结果.成功 = 成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_质量可用状态(),
            结果.质量可用状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入自我观察帧质量摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        auto 记录结果 = 结果;
        记录结果.成功 = 成功;
        记录双目相机本能动作结果(
            "自我_评估观察帧质量",
            记录结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_质量可用状态(),
            "质量可用状态");
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我判断基础观察事实可用性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_判断基础观察事实可用性,
            输出结果场景,
            now,
            "自我_判断基础观察事实可用性",
            确认自我判断基础观察事实可用性规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        清理执行输出派生需求标记(实际输出场景, "自我_判断基础观察事实可用性");
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        结构_双目相机调用结果 结果{};
        const bool 摘要闭合 = 读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        if (摘要闭合) {
            补全基础观察事实状态(结果, now);
        } else {
            结果.基础观察事实可用状态 = 观察事实状态_未成立;
            结果.基础观察事实失败条件数量 = 1;
            结果.当前观察新鲜度状态 = 观察事实状态_未成立;
            结果.新鲜度失败原因 = 新鲜度失败原因_缺采集时间 | 新鲜度失败原因_来源外设不可追溯;
            结果.新鲜度失败原因摘要 = 新鲜度失败原因摘要文本(结果.新鲜度失败原因);
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "基础观察事实可用性条件不足:缺少已落账观察特征帧摘要";
        }

        const bool 成功 =
            摘要闭合
            && 结果.基础观察事实可用状态 == 观察事实状态_可用;
        if (!成功 && !结果.失败原因) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = std::string("基础观察事实可用性条件不足:")
                + 基础观察事实失败条件摘要(结果);
        }
        结果.成功 = 成功;

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_基础观察事实可用状态(),
            结果.基础观察事实可用状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入自我基础观察事实可用摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);
        if (!成功) {
            I64 派生当前值 = 0;
            I64 派生目标值 = 0;
            const auto* 派生特征 = 选择基础观察事实首个缺口特征(
                结果,
                派生当前值,
                派生目标值);
            写入宿主I64特征派生需求(
                环境.方法首节点,
                目标宿主,
                派生特征,
                派生当前值,
                派生目标值,
                实际输出场景,
                now,
                "自我_判断基础观察事实可用性");
        }

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        auto 记录结果 = 结果;
        记录结果.成功 = 成功;
        记录双目相机本能动作结果(
            "自我_判断基础观察事实可用性",
            记录结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_基础观察事实可用状态(),
            "基础观察事实可用状态");
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 安全评估当前场景安全性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::安全_评估当前场景安全性,
            输出结果场景,
            now,
            "安全_评估当前场景安全性",
            确认安全评估当前场景安全性规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        清理执行输出派生需求标记(实际输出场景, "安全_评估当前场景安全性");
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        void* 来源需求指针 = nullptr;
        void* 来源任务指针 = nullptr;
        void* 来源因果指针 = nullptr;
        const bool 风险安全场景影响_来源需求存在 =
            读取输入参数指针(输入参数场景, 特征_来源需求(), 来源需求指针)
            && 来源需求指针;
        const bool 风险安全场景影响_来源任务存在 =
            读取输入参数指针(输入参数场景, 特征_任务信息节点(), 来源任务指针)
            && 来源任务指针;
        const bool 风险安全场景影响_来源因果存在 =
            读取输入参数指针(输入参数场景, 特征_来源因果信息(), 来源因果指针)
            && 来源因果指针;

        结构_双目相机调用结果 观察结果{};
        I64 场景存在集合明确状态 = 安全明确状态_未明确;
        I64 关键特征变化观察状态 = 安全明确状态_未明确;
        I64 安全因果候选构造状态 = 安全明确状态_未明确;
        I64 关键特征观察样本数量 = 0;
        I64 关键特征负向变化数量 = 0;
        I64 安全因果候选数量 = 0;
        I64 安全因果候选低置信数量 = 0;
        I64 安全因果候选归因置信度 = 0;
        I64 持续观察状态 = 安全明确状态_未明确;
        I64 持续观察样本数量 = 0;
        (void)读取基础节点I64(目标宿主, 特征_场景存在集合明确状态(), 场景存在集合明确状态);
        (void)读取基础节点I64(目标宿主, 特征_关键特征变化观察状态(), 关键特征变化观察状态);
        (void)读取基础节点I64(目标宿主, 特征_安全因果候选构造状态(), 安全因果候选构造状态);
        (void)读取基础节点I64(目标宿主, 特征_关键特征观察样本数量(), 关键特征观察样本数量);
        (void)读取基础节点I64(目标宿主, 特征_关键特征负向变化数量(), 关键特征负向变化数量);
        (void)读取基础节点I64(目标宿主, 特征_安全因果候选数量(), 安全因果候选数量);
        (void)读取基础节点I64(目标宿主, 特征_安全因果候选低置信数量(), 安全因果候选低置信数量);
        (void)读取基础节点I64(目标宿主, 特征_安全因果候选归因置信度(), 安全因果候选归因置信度);
        (void)读取基础节点I64(目标宿主, 特征_当前场景持续观察状态(), 持续观察状态);
        (void)读取基础节点I64(目标宿主, 特征_当前场景持续观察样本数量(), 持续观察样本数量);
        const auto 当前场景存在集合 = 读取当前场景已确认存在集合(
            目标宿主,
            输入参数场景,
            实际输出场景);
        if (!当前场景存在集合.empty()) {
            场景存在集合明确状态 = 安全明确状态_已明确;
        }

        const bool 摘要闭合 = 读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 观察结果);
        if (摘要闭合) {
            补全基础观察事实状态(观察结果, now);
        } else {
            if (!当前场景存在集合.empty()) {
                观察结果.成功 = true;
                观察结果.相机已打开 = true;
                观察结果.基础观察事实可用状态 = 观察事实状态_可用;
                观察结果.基础观察事实失败条件数量 = 0;
                观察结果.失败原因 = nullptr;
                观察结果.消息 = "安全评估读取已确认观察存在集合作为场景事实底稿";
            } else {
                观察结果.基础观察事实可用状态 = 观察事实状态_未成立;
                观察结果.基础观察事实失败条件数量 = 1;
                观察结果.失败原因 = 值_执行缺口_当前方法条件不满足();
                观察结果.消息 = "安全评估条件不足:缺少基础观察事实摘要";
            }
        }

        const bool 基础观察可用 =
            (摘要闭合 || !当前场景存在集合.empty())
            && 观察结果.基础观察事实可用状态 == 观察事实状态_可用;

        I64 不足原因 = 0;
        if (!基础观察可用) 不足原因 |= 安全评估不足原因_基础观察事实不可用;
        if (场景存在集合明确状态 != 安全明确状态_已明确) 不足原因 |= 安全评估不足原因_场景存在集合未知;
        if (关键特征变化观察状态 != 安全明确状态_已明确) 不足原因 |= 安全评估不足原因_关键特征变化未知;
        if (安全因果候选构造状态 != 安全明确状态_已明确
            || 安全因果候选低置信数量 > 0
            || 安全因果候选归因置信度 <= 0) {
            不足原因 |= 安全评估不足原因_因果证据不足;
        }
        const bool 因果信息被动缺省良好 =
            基础观察可用
            && 场景存在集合明确状态 == 安全明确状态_已明确
            && (关键特征变化观察状态 != 安全明确状态_已明确
                || 安全因果候选构造状态 != 安全明确状态_已明确);
        const bool 评估判定可生成 = 目标宿主 != nullptr;
        const bool 成功 = 评估判定可生成;
        const I64 基础风险判断状态 = 成功
            ? 基础风险判断状态_已明确
            : (基础观察可用
                ? 基础风险判断状态_证据不足
                : 基础风险判断状态_条件不足);
        const I64 明确状态 = 成功 ? 安全明确状态_已明确 : 安全明确状态_未明确;
        const I64 评估置信度 = 0;
        const I64 未判断因子数量 =
            ((不足原因 & 安全评估不足原因_基础观察事实不可用) != 0 ? 1 : 0)
            + ((不足原因 & 安全评估不足原因_场景存在集合未知) != 0 ? 1 : 0)
            + ((不足原因 & 安全评估不足原因_关键特征变化未知) != 0 ? 1 : 0)
            + ((不足原因 & 安全评估不足原因_因果证据不足) != 0 ? 1 : 0);
        const I64 证据完整度 = 0;
        const I64 不确定度 = 场景绝对安全值;
        const bool 条件不足默认不安全 =
            !基础观察可用
            || 场景存在集合明确状态 != 安全明确状态_已明确
            || 当前场景存在集合.empty();
        const 结构_安全评估证据视图 证据视图{
            .自我关键特征观察明确 = 关键特征变化观察状态 == 安全明确状态_已明确,
            .场景存在集合明确 = 场景存在集合明确状态 == 安全明确状态_已明确,
            .安全因果候选明确 = 安全因果候选构造状态 == 安全明确状态_已明确,
            .不足原因 = 不足原因,
        };
        std::vector<结构_存在风险评估结果> 存在风险评估集;
        存在风险评估集.reserve(当前场景存在集合.size());
        bool 存在风险评估全部明确 = !当前场景存在集合.empty();
        I64 最低存在安全值 = -1;
        I64 最高存在风险值 = 0;
        存在节点类* 最低安全度存在 = nullptr;
        for (auto* 存在 : 当前场景存在集合) {
            auto 结果 = 评估存在风险安全值(存在, 证据视图);
            if (!结果.评估明确) {
                存在风险评估全部明确 = false;
            }
            if (最低存在安全值 < 0 || 结果.存在安全度候选 < 最低存在安全值) {
                最低存在安全值 = 结果.存在安全度候选;
                最低安全度存在 = 存在;
            }
            if (结果.风险安全值候选 > 最高存在风险值) {
                最高存在风险值 = 结果.风险安全值候选;
            }
            存在风险评估集.push_back(结果);
        }
        const I64 场景风险压力平均值 = 平均存在风险压力(存在风险评估集);
        const I64 场景评估安全值候选 = 计算场景评估安全值候选(
            成功,
            条件不足默认不安全,
            不足原因,
            存在风险评估全部明确,
            场景风险压力平均值);
        const I64 安全度候选 = 场景评估安全值候选;
        const I64 直接伤害可能性候选 = 场景绝对安全值 - 场景评估安全值候选;
        const I64 风险安全_场景影响部分候选 = 场景评估安全值候选;
        const I64 风险安全层缺失因素数量 = 风险安全层v1因素数量 - 1;
        const I64 风险安全层候选 = 平均风险安全层v1因素(
            风险安全_场景影响部分候选,
            0,
            0,
            0,
            0);
        const I64 风险安全层投影候选 = 计算风险安全第二层投影候选(风险安全层候选);
        const bool 无负向因果默认安全 =
            成功
            && !条件不足默认不安全
            && 不足原因 == 0
            && 存在风险评估全部明确
            && 场景风险压力平均值 == 0
            && 最高存在风险值 == 0;
        const I64 场景安全判定状态 = 成功
            && 场景评估安全值候选 == 场景绝对安全值
            && 直接伤害可能性候选 == 0
            ? 安全判定状态_安全
            : 安全判定状态_不安全;
        const I64 场景安全判定来源 = 场景安全判定状态 == 安全判定状态_安全
            ? (无负向因果默认安全
                ? 安全判定来源_无负向因果默认安全
                : 安全判定来源_已证安全)
            : (条件不足默认不安全
                ? 安全判定来源_条件不足默认不安全
                : (不足原因 != 0
                    ? 安全判定来源_证据不足默认不安全
                    : 安全判定来源_已证不安全));
        const I64 持续观察需求明确状态 =
            成功
            && 场景安全判定状态 == 安全判定状态_不安全
            && 场景安全判定来源 == 安全判定来源_证据不足默认不安全
            ? 安全明确状态_未明确
            : 安全明确状态_已明确;
        const I64 负向影响处置需求明确状态 =
            成功
            && 场景安全判定状态 == 安全判定状态_不安全
            ? 安全明确状态_未明确
            : 安全明确状态_已明确;
        观察结果.成功 = 成功;
        观察结果.失败原因 = 成功 ? nullptr : 值_执行缺口_当前方法条件不满足();
        if (成功) {
            观察结果.消息 = "安全评估已产出当前保守判定结果";
        } else if (!基础观察可用) {
            观察结果.消息 = "安全评估条件不足:基础观察事实不可用";
        } else if (当前场景存在集合.empty()) {
            观察结果.消息 = "安全评估条件不足:当前场景存在集合未知";
        } else {
            观察结果.消息 = "安全评估证据不足:缺少自我关键特征变化或安全因果证据";
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_基础风险判断状态(),
            基础风险判断状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        I64 风险安全场景影响不可结算原因 = 风险安全场景影响不可结算原因_无;
        const bool 风险安全场景影响_提交入口存在 = true;
        const bool 风险安全场景影响_公式版本明确 =
            风险安全层版本_v1 > 0
            && 风险安全层v1因素数量 > 0;
        I64 已入账场景影响旧值 = 0;
        const bool 已入账场景影响旧值已读取 = 读取基础节点I64(
            目标宿主,
            特征_自我_风险安全_场景影响部分(),
            已入账场景影响旧值);
        const I64 风险安全场景影响变化方向 =
            !已入账场景影响旧值已读取
                ? 风险安全场景影响变化方向_初始入账
                : (风险安全_场景影响部分候选 > 已入账场景影响旧值
                    ? 风险安全场景影响变化方向_上调
                    : (风险安全_场景影响部分候选 < 已入账场景影响旧值
                        ? 风险安全场景影响变化方向_下调
                        : 风险安全场景影响变化方向_不变));
        const bool 风险安全场景影响_被动归因证据存在 =
            风险安全场景影响_来源因果存在
            && 安全因果候选构造状态 == 安全明确状态_已明确
            && 动态
            && 实际输出场景
            && 风险安全场景影响_公式版本明确;
        const bool 风险安全场景影响_持续稳定性证据存在 =
            持续观察状态 == 安全明确状态_已明确
            && 持续观察样本数量 > 0
            && 关键特征变化观察状态 == 安全明确状态_已明确
            && 关键特征观察样本数量 > 0
            && 关键特征负向变化数量 == 0;
        if (!风险安全场景影响_来源需求存在) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_缺来源需求;
        }
        if (!风险安全场景影响_来源任务存在) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_缺来源任务;
        }
        if (!动态) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_缺方法动作动态;
        }
        if (!实际输出场景) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_缺评估结果场景;
        }
        if (场景存在集合明确状态 != 安全明确状态_已明确) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_场景存在集合未明确;
        }
        if (明确状态 != 安全明确状态_已明确) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_风险状态未明确;
        }
        if (不足原因 != 0) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_证据不足原因非零;
        }
        if (!风险安全场景影响_被动归因证据存在) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_缺被动归因证据;
        }
        if (!风险安全场景影响_持续稳定性证据存在) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_缺稳定性证据;
        }
        if (!风险安全场景影响_公式版本明确) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_缺公式版本;
        }
        if (风险安全场景影响变化方向 == 风险安全场景影响变化方向_未明确) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_变化方向未明确;
        } else if (
            风险安全场景影响变化方向 == 风险安全场景影响变化方向_上调
            && !风险安全场景影响_持续稳定性证据存在) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_上调缺持续稳定;
        } else if (
            风险安全场景影响变化方向 == 风险安全场景影响变化方向_下调
            && !(成功 && 明确状态 == 安全明确状态_已明确 && 不足原因 == 0)) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_下调缺当前风险证据;
        }
        if (!风险安全场景影响_提交入口存在) {
            风险安全场景影响不可结算原因 |= 风险安全场景影响不可结算原因_缺提交入口;
        }
        const I64 风险安全场景影响可结算状态 =
            风险安全场景影响不可结算原因 == 风险安全场景影响不可结算原因_无
            ? 风险安全场景影响可结算状态_可结算
            : 风险安全场景影响可结算状态_不可结算;
        const I64 风险安全场景影响已入账状态 = 风险安全场景影响入账状态_未入账;
        const I64 风险因素未搜索掩码 = 风险安全因素掩码_非场景;
        const I64 风险因素搜索缺口掩码 = 0;
        const I64 风险因素负向证据掩码 = 0;
        const I64 风险因素默认满足掩码 = 0;
        const I64 风险因素已入账掩码 =
            风险安全场景影响已入账状态 == 风险安全场景影响入账状态_已入账
            ? 风险安全因素掩码_场景影响
            : 0;
        const I64 风险因素证据搜索未完成数量 = 统计风险安全因素位数(风险因素未搜索掩码);
        const I64 风险因素默认满足数量 = 统计风险安全因素位数(风险因素默认满足掩码);
        const I64 风险安全层可聚合状态 =
            风险因素未搜索掩码 == 0
            && 风险因素搜索缺口掩码 == 0
            && (风险因素已入账掩码 | 风险因素默认满足掩码 | 风险因素负向证据掩码) == 风险安全因素掩码_全部
            ? 安全明确状态_已明确
            : 安全明确状态_未明确;
        const I64 风险安全层明确状态 =
            风险安全层可聚合状态 == 安全明确状态_已明确
            ? 安全明确状态_已明确
            : 安全明确状态_未明确;
        const I64 风险安全层已入账状态 = 风险安全场景影响入账状态_未入账;
        const auto 当前场景特征值材料门 = 评估当前场景特征值材料门(当前场景存在集合, now);
        const I64 当前场景各存在特征值可读状态 = 当前场景特征值材料门.状态;

        I64 可比较评估状态写入数量 = 0;
        auto 写可比较评估状态 = [&](const 语素入口节点类* 特征类型, I64 值) noexcept {
            if (写入输出结果I64状态(
                实际输出场景,
                目标宿主
                    ? 目标宿主
                    : reinterpret_cast<基础信息节点类*>(实际输出场景),
                特征类型,
                值,
                now)) {
                ++可比较评估状态写入数量;
            }
        };
        写可比较评估状态(特征_风险状态明确状态(), 明确状态);
        写可比较评估状态(特征_当前场景安全度明确状态(), 明确状态);
        写可比较评估状态(特征_当前场景最低安全度存在明确状态(), 明确状态);
        写可比较评估状态(特征_当前场景各存在安全评估明确状态(), 明确状态);
        写可比较评估状态(特征_当前场景各存在特征值可读状态(), 当前场景各存在特征值可读状态);
        写可比较评估状态(特征_当前场景持续观察需求明确状态(), 持续观察需求明确状态);
        写可比较评估状态(特征_负向影响处置需求明确状态(), 负向影响处置需求明确状态);
        写可比较评估状态(特征_当前场景安全判定状态(), 场景安全判定状态);
        写可比较评估状态(特征_风险安全_场景影响部分可结算状态(), 风险安全场景影响可结算状态);
        写可比较评估状态(特征_风险安全_场景影响部分不可结算原因(), 风险安全场景影响不可结算原因);
        写可比较评估状态(特征_风险安全_场景影响部分已入账状态(), 风险安全场景影响已入账状态);
        写可比较评估状态(特征_风险安全层明确状态(), 风险安全层明确状态);
        写可比较评估状态(特征_风险安全层可聚合状态(), 风险安全层可聚合状态);
        写可比较评估状态(特征_风险因素未搜索掩码(), 风险因素未搜索掩码);
        写可比较评估状态(特征_风险因素默认满足掩码(), 风险因素默认满足掩码);
        写可比较评估状态(特征_风险因素已入账掩码(), 风险因素已入账掩码);

        auto 写单个存在安全评估结果 = [&](基础信息节点类* 结果宿主) noexcept {
            if (!结果宿主) return;
            I64 序号 = 0;
            for (const auto& 存在风险结果 : 存在风险评估集) {
                ++序号;
                auto* 结果存在 = 世界树.取或创建子存在_按类型并按特征I64(
                    结果宿主,
                    类型_安全评估存在结果(),
                    特征_安全评估存在序号(),
                    序号);
                auto* 节点 = reinterpret_cast<基础信息节点类*>(结果存在);
                if (!节点) continue;
                const I64 存在不足原因 = 存在风险结果.证据不足原因 != 0
                    ? 存在风险结果.证据不足原因
                    : 安全评估不足原因_因果证据不足;
                const I64 存在安全判定状态 = 成功
                    && 存在风险结果.评估明确
                    && 存在风险结果.风险安全值候选 <= 0
                    ? 安全判定状态_安全
                    : 安全判定状态_不安全;
                const I64 存在安全判定来源 = 存在安全判定状态 == 安全判定状态_安全
                    ? (无负向因果默认安全
                        ? 安全判定来源_无负向因果默认安全
                        : 安全判定来源_已证安全)
                    : (条件不足默认不安全
                        ? 安全判定来源_条件不足默认不安全
                        : (!存在风险结果.评估明确
                            ? 安全判定来源_证据不足默认不安全
                            : 安全判定来源_已证不安全));
                (void)写入基础节点I64(节点, 特征_安全评估存在序号(), 序号, now);
                (void)写入基础节点指针(节点, 特征_被评估存在(), 存在风险结果.存在, now);
                (void)写入基础节点I64(节点, 特征_存在直接伤害可能性候选(), 存在风险结果.风险安全值候选, now);
                (void)写入基础节点I64(节点, 特征_存在风险安全值候选(), 存在风险结果.风险安全值候选, now);
                (void)写入基础节点I64(节点, 特征_存在安全度候选(), 存在风险结果.存在安全度候选, now);
                (void)写入基础节点I64(节点, 特征_历史直接伤害归因次数(), 存在风险结果.历史直接伤害次数, now);
                (void)写入基础节点I64(节点, 特征_最近直接伤害归因时间(), 存在风险结果.最近直接伤害时间, now);
                (void)写入基础节点I64(节点, 特征_无伤害共处时长(), 存在风险结果.无伤害共处时长, now);
                (void)写入基础节点I64(节点, 特征_伤害可能性下界候选(), 存在风险结果.伤害可能性下界, now);
                (void)写入基础节点I64(节点, 特征_存在安全评估置信度(), 存在风险结果.评估置信度, now);
                (void)写入基础节点I64(节点, 特征_存在安全评估证据不足原因(), 存在不足原因, now);
                (void)写入基础节点I64(节点, 特征_存在安全判定状态(), 存在安全判定状态, now);
                (void)写入基础节点I64(节点, 特征_存在安全判定来源(), 存在安全判定来源, now);
                (void)写入基础节点指针(节点, 特征_动作动态(), 动态, now);
            }
        };

        auto 写安全评估结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, nullptr, 目标宿主, 动态, 观察结果.失败原因, now);
            写入自我基础观察事实可用摘要(宿主, 观察结果, now);
            if (来源需求指针) {
                (void)写入基础节点指针(宿主, 特征_来源需求(), 来源需求指针, now);
            }
            if (来源任务指针) {
                (void)写入基础节点指针(宿主, 特征_任务信息节点(), 来源任务指针, now);
            }
            if (来源因果指针) {
                (void)写入基础节点指针(宿主, 特征_来源因果信息(), 来源因果指针, now);
            }
            (void)写入基础节点I64(宿主, 特征_基础风险判断状态(), 基础风险判断状态, now);
            (void)写入基础节点I64(宿主, 特征_风险状态明确状态(), 明确状态, now);
            (void)写入基础节点I64(宿主, 特征_当前场景安全度明确状态(), 明确状态, now);
            (void)写入基础节点I64(宿主, 特征_当前场景最低安全度存在明确状态(), 明确状态, now);
            (void)写入基础节点I64(宿主, 特征_当前场景各存在安全评估明确状态(), 明确状态, now);
            (void)写入基础节点I64(宿主, 特征_当前场景各存在特征值可读状态(), 当前场景各存在特征值可读状态, now);
            写入当前场景特征值材料门摘要(宿主, 当前场景特征值材料门, now);
            (void)写入基础节点I64(宿主, 特征_当前场景持续观察需求明确状态(), 持续观察需求明确状态, now);
            (void)写入基础节点I64(宿主, 特征_负向影响处置需求明确状态(), 负向影响处置需求明确状态, now);
            (void)写入基础节点I64(宿主, 特征_当前场景安全度候选(), 安全度候选, now);
            (void)写入基础节点I64(宿主, 特征_当前场景评估安全值候选(), 场景评估安全值候选, now);
            (void)写入基础节点I64(宿主, 特征_当前场景直接伤害可能性候选(), 直接伤害可能性候选, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分候选(), 风险安全_场景影响部分候选, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分可结算状态(), 风险安全场景影响可结算状态, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分不可结算原因(), 风险安全场景影响不可结算原因, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分已入账状态(), 风险安全场景影响已入账状态, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分变化方向(), 风险安全场景影响变化方向, now);
            (void)写入基础节点I64(宿主, 特征_风险安全层因素集合版本(), 风险安全层版本_v1, now);
            (void)写入基础节点I64(宿主, 特征_风险安全层因素数量(), 风险安全层v1因素数量, now);
            (void)写入基础节点I64(宿主, 特征_风险安全层缺失因素数量(), 风险安全层缺失因素数量, now);
            (void)写入基础节点I64(宿主, 特征_风险安全层明确状态(), 风险安全层明确状态, now);
            (void)写入基础节点I64(宿主, 特征_风险安全层可聚合状态(), 风险安全层可聚合状态, now);
            (void)写入基础节点I64(宿主, 特征_风险安全层已入账状态(), 风险安全层已入账状态, now);
            (void)写入基础节点I64(宿主, 特征_风险因素未搜索掩码(), 风险因素未搜索掩码, now);
            (void)写入基础节点I64(宿主, 特征_风险因素搜索缺口掩码(), 风险因素搜索缺口掩码, now);
            (void)写入基础节点I64(宿主, 特征_风险因素负向证据掩码(), 风险因素负向证据掩码, now);
            (void)写入基础节点I64(宿主, 特征_风险因素默认满足掩码(), 风险因素默认满足掩码, now);
            (void)写入基础节点I64(宿主, 特征_风险因素已入账掩码(), 风险因素已入账掩码, now);
            (void)写入基础节点I64(宿主, 特征_风险因素证据搜索未完成数量(), 风险因素证据搜索未完成数量, now);
            (void)写入基础节点I64(宿主, 特征_风险因素默认满足数量(), 风险因素默认满足数量, now);
            (void)写入基础节点I64(宿主, 特征_风险安全层候选(), 风险安全层候选, now);
            (void)写入基础节点I64(宿主, 特征_风险安全层投影候选(), 风险安全层投影候选, now);
            (void)写入基础节点指针(宿主, 特征_当前场景最低安全度存在(), 最低安全度存在, now);
            (void)写入基础节点I64(宿主, 特征_当前场景安全评估置信度(), 评估置信度, now);
            (void)写入基础节点I64(宿主, 特征_当前场景安全判定状态(), 场景安全判定状态, now);
            (void)写入基础节点I64(宿主, 特征_当前场景安全判定来源(), 场景安全判定来源, now);
            (void)写入基础节点I64(宿主, 特征_基础风险未判断因子数量(), 未判断因子数量, now);
            (void)写入基础节点I64(宿主, 特征_基础风险证据完整度(), 证据完整度, now);
            (void)写入基础节点I64(宿主, 特征_基础风险不确定度(), 不确定度, now);
            (void)写入基础节点I64(宿主, 特征_基础风险值计算状态(), 安全明确状态_未明确, now);
            (void)写入基础节点I64(宿主, 特征_基础风险评估范围状态(), 场景存在集合明确状态, now);
            (void)写入基础节点I64(宿主, 特征_安全评估证据不足原因(), 不足原因, now);
        };
        写安全评估结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写安全评估结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写入当前场景特征值材料门明细(实际输出场景, 当前场景特征值材料门, now);
        写单个存在安全评估结果(reinterpret_cast<基础信息节点类*>(实际输出场景));

        I64 派生当前值 = 0;
        const 语素入口节点类* 派生特征 = nullptr;
        if (!基础观察可用) {
            派生特征 = 特征_基础观察事实可用状态();
            派生当前值 = 观察结果.基础观察事实可用状态;
        } else if (场景存在集合明确状态 != 安全明确状态_已明确) {
            派生特征 = 特征_场景存在集合明确状态();
            派生当前值 = 场景存在集合明确状态;
        } else if (关键特征变化观察状态 != 安全明确状态_已明确) {
            派生特征 = 特征_关键特征变化观察状态();
            派生当前值 = 关键特征变化观察状态;
        } else if (安全因果候选构造状态 != 安全明确状态_已明确) {
            派生特征 = 特征_安全因果候选构造状态();
            派生当前值 = 安全因果候选构造状态;
        } else if (持续观察需求明确状态 != 安全明确状态_已明确) {
            派生特征 = 特征_当前场景持续观察需求明确状态();
            派生当前值 = 持续观察需求明确状态;
        } else if (负向影响处置需求明确状态 != 安全明确状态_已明确) {
            派生特征 = 特征_负向影响处置需求明确状态();
            派生当前值 = 负向影响处置需求明确状态;
        }
        写入宿主I64特征派生需求(
            环境.方法首节点,
            目标宿主,
            派生特征,
            派生当前值,
            安全明确状态_已明确,
            实际输出场景,
            now,
            "安全_评估当前场景安全性");

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "安全_评估当前场景安全性",
            观察结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_基础风险判断状态(),
            "基础风险判断状态");
        {
            std::ostringstream 输出;
            输出 << "安全评估本能方法结果"
                << " | 方法=安全_评估当前场景安全性"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 基础风险判断状态=" << 基础风险判断状态
                << " | 风险状态明确状态=" << 明确状态
                << " | 场景存在集合明确状态=" << 场景存在集合明确状态
                << " | 当前场景存在数量=" << 当前场景存在集合.size()
                << " | 当前场景各存在特征值可读状态=" << 当前场景各存在特征值可读状态
                << " | 当前场景特征值材料门版本=" << 当前场景特征值材料门.版本
                << " | 当前场景特征值可读存在数量=" << 当前场景特征值材料门.可读存在数量
                << " | 当前场景特征值部分可读存在数量=" << 当前场景特征值材料门.部分可读存在数量
                << " | 当前场景特征值证据不足存在数量=" << 当前场景特征值材料门.证据不足存在数量
                << " | 当前场景特征值缺失原因掩码=" << 当前场景特征值材料门.缺失原因掩码
                << " | 当前场景特征值过期原因掩码=" << 当前场景特征值材料门.过期原因掩码
                << " | 当前场景特征值冲突原因掩码=" << 当前场景特征值材料门.冲突原因掩码
                << " | D455空间材料不在安全评估中直接升格场景绝对坐标=1"
                << " | 关键特征变化观察状态=" << 关键特征变化观察状态
                << " | 安全因果候选构造状态=" << 安全因果候选构造状态
                << " | 证据不足原因=" << 不足原因
                << " | 未判断因子数量=" << 未判断因子数量
                << " | 存在风险评估全部明确=" << (存在风险评估全部明确 ? 1 : 0)
                << " | 最高存在风险安全值候选=" << 最高存在风险值
                << " | 场景风险压力平均值=" << 场景风险压力平均值
                << " | 最低存在安全值候选=" << 最低存在安全值
                << " | 当前场景评估安全值候选=" << 场景评估安全值候选
                << " | 当前场景安全度候选=" << 安全度候选
                << " | 当前场景直接伤害可能性候选=" << 直接伤害可能性候选
                << " | 当前场景安全判定状态=" << 场景安全判定状态
                << " | 当前场景安全判定来源=" << 场景安全判定来源
                << " | 当前场景持续观察需求明确状态=" << 持续观察需求明确状态
                << " | 负向影响处置需求明确状态=" << 负向影响处置需求明确状态
                << " | 低安全持续观察派生=" << (持续观察需求明确状态 != 安全明确状态_已明确 ? 1 : 0)
                << " | 低安全处置需求派生=" << (负向影响处置需求明确状态 != 安全明确状态_已明确 ? 1 : 0)
                << " | 因果信息被动获取=1"
                << " | 无负向因果默认安全=" << (场景安全判定来源 == 安全判定来源_无负向因果默认安全 ? 1 : 0)
                << " | 场景安全评估范围=0-I64MAX"
                << " | I64MAX仅绝对安全=1"
                << " | 历史缺失默认值=0"
                << " | 无负向因果默认评估安全值=I64MAX"
                << " | 条件不足评估安全值=I64MAX/4"
                << " | 已证不安全评估安全值=0"
                << " | 风险安全层因素集合版本=" << 风险安全层版本_v1
                << " | 风险安全层因素数量=" << 风险安全层v1因素数量
                << " | 风险安全_场景影响部分候选=" << 风险安全_场景影响部分候选
                << " | 风险安全_场景影响部分可结算状态=" << 风险安全场景影响可结算状态
                << " | 风险安全_场景影响部分不可结算原因=" << 风险安全场景影响不可结算原因
                << " | 风险安全_场景影响部分已入账状态=" << 风险安全场景影响已入账状态
                << " | 风险安全_场景影响部分变化方向=" << 风险安全场景影响变化方向
                << " | 风险安全_场景影响部分旧值已读取=" << (已入账场景影响旧值已读取 ? 1 : 0)
                << " | 风险安全_场景影响部分旧值=" << 已入账场景影响旧值
                << " | 来源需求存在=" << (风险安全场景影响_来源需求存在 ? 1 : 0)
                << " | 来源任务存在=" << (风险安全场景影响_来源任务存在 ? 1 : 0)
                << " | 来源因果信息存在=" << (风险安全场景影响_来源因果存在 ? 1 : 0)
                << " | 被动归因证据存在=" << (风险安全场景影响_被动归因证据存在 ? 1 : 0)
                << " | 持续稳定性证据存在=" << (风险安全场景影响_持续稳定性证据存在 ? 1 : 0)
                << " | 当前场景持续观察状态=" << 持续观察状态
                << " | 当前场景持续观察样本数量=" << 持续观察样本数量
                << " | 关键特征观察样本数量=" << 关键特征观察样本数量
                << " | 关键特征负向变化数量=" << 关键特征负向变化数量
                << " | 安全因果候选数量=" << 安全因果候选数量
                << " | 安全因果候选低置信数量=" << 安全因果候选低置信数量
                << " | 安全因果候选归因置信度=" << 安全因果候选归因置信度
                << " | 风险安全层缺失因素数量=" << 风险安全层缺失因素数量
                << " | 风险安全层明确状态=" << 风险安全层明确状态
                << " | 风险安全层可聚合状态=" << 风险安全层可聚合状态
                << " | 风险安全层已入账状态=" << 风险安全层已入账状态
                << " | 风险因素未搜索掩码=" << 风险因素未搜索掩码
                << " | 风险因素搜索缺口掩码=" << 风险因素搜索缺口掩码
                << " | 风险因素负向证据掩码=" << 风险因素负向证据掩码
                << " | 风险因素默认满足掩码=" << 风险因素默认满足掩码
                << " | 风险因素已入账掩码=" << 风险因素已入账掩码
                << " | 风险因素证据搜索未完成数量=" << 风险因素证据搜索未完成数量
                << " | 风险因素默认满足数量=" << 风险因素默认满足数量
                << " | 风险安全层候选=" << 风险安全层候选
                << " | 风险安全层投影候选=" << 风险安全层投影候选
                << " | 场景存在风险聚合=平均"
                << " | 场景评估安全值=I64MAX-场景风险压力平均值"
                << " | 场景安全度=场景评估安全值候选"
                << " | 场景内部额外衰减=0"
                << " | 风险安全层投影=风险安全*0.09"
                << " | 候选不改账=1"
                << " | 缺被动归因证据=" << (风险安全场景影响不可结算原因 & 风险安全场景影响不可结算原因_缺被动归因证据 ? 1 : 0)
                << " | 缺持续稳定性证据=" << (风险安全场景影响不可结算原因 & 风险安全场景影响不可结算原因_缺稳定性证据 ? 1 : 0)
                << " | 缺风险安全场景影响提交入口=" << (风险安全场景影响不可结算原因 & 风险安全场景影响不可结算原因_缺提交入口 ? 1 : 0)
                << " | 后续随时间影响自我风险安全值=1"
                << " | 最低安全度存在=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(最低安全度存在))
                << " | 评估置信度=" << 评估置信度
                << " | 证据不足默认不安全=" << (场景安全判定来源 == 安全判定来源_证据不足默认不安全 ? 1 : 0)
                << " | 条件不足默认不安全=" << (场景安全判定来源 == 安全判定来源_条件不足默认不安全 ? 1 : 0)
                << " | 默认良好允许提交=" << (成功 && 场景安全判定来源 == 安全判定来源_无负向因果默认安全 ? 1 : 0)
                << " | 不安全判定允许提交=" << (成功 && 场景安全判定状态 == 安全判定状态_不安全 ? 1 : 0)
                << " | 可比较评估状态写入数量=" << 可比较评估状态写入数量
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 提交风险安全场景影响部分状态变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::提交_风险安全场景影响部分状态变化,
            输出结果场景,
            now,
            "提交_风险安全场景影响部分状态变化",
            确认提交风险安全场景影响部分状态变化规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        I64 候选值 = 0;
        I64 可结算状态 = 风险安全场景影响可结算状态_未判定;
        I64 不可结算原因 = 风险安全场景影响不可结算原因_缺评估结果场景;
        I64 已入账状态 = 风险安全场景影响入账状态_未入账;
        I64 变化方向 = 风险安全场景影响变化方向_未明确;
        const bool 候选已读取 = 读取基础节点I64(
            目标宿主,
            特征_风险安全_场景影响部分候选(),
            候选值);
        const bool 可结算已读取 = 读取基础节点I64(
            目标宿主,
            特征_风险安全_场景影响部分可结算状态(),
            可结算状态);
        const bool 原因已读取 = 读取基础节点I64(
            目标宿主,
            特征_风险安全_场景影响部分不可结算原因(),
            不可结算原因);
        (void)读取基础节点I64(
            目标宿主,
            特征_风险安全_场景影响部分已入账状态(),
            已入账状态);
        (void)读取基础节点I64(
            目标宿主,
            特征_风险安全_场景影响部分变化方向(),
            变化方向);

        if (!候选已读取 || !可结算已读取 || !原因已读取) {
            不可结算原因 |= 风险安全场景影响不可结算原因_缺评估结果场景;
        }

        const bool 允许提交 =
            候选已读取
            && 可结算已读取
            && 原因已读取
            && 可结算状态 == 风险安全场景影响可结算状态_可结算
            && 不可结算原因 == 风险安全场景影响不可结算原因_无
            && 已入账状态 != 风险安全场景影响入账状态_已入账;
        const I64 提交状态 = 允许提交
            ? 风险安全场景影响提交状态_已提交
            : 风险安全场景影响提交状态_条件不足;

        结构_双目相机调用结果 结果{};
        结果.成功 = 允许提交;
        if (允许提交) {
            结果.消息 = "风险安全场景影响部分状态变化已提交";
        } else {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "风险安全场景影响部分状态变化提交条件不足";
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_风险安全_场景影响部分提交状态(),
            提交状态,
            允许提交,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        if (允许提交) {
            (void)写入基础节点I64(
                目标宿主,
                特征_自我_风险安全_场景影响部分(),
                候选值,
                now);
            (void)写入基础节点I64(
                目标宿主,
                特征_风险安全_场景影响部分已入账状态(),
                风险安全场景影响入账状态_已入账,
                now);
            I64 因素ID = 0;
            if (读取基础节点I64(
                    目标宿主,
                    特征_安全因果因素ID(),
                    因素ID)
                && 因素ID == 风险安全因素掩码_场景影响) {
                (void)写入基础节点I64(
                    目标宿主,
                    特征_安全因果因素值(),
                    候选值,
                    now);
                (void)写入基础节点I64(
                    目标宿主,
                    特征_安全因果因素已入账状态(),
                    安全因果因素入账状态_已入账,
                    now);
                (void)写入基础节点I64(
                    目标宿主,
                    特征_安全因果因素默认满足提交状态(),
                    安全因果因素提交状态_未提交,
                    now);
            }
            已入账状态 = 风险安全场景影响入账状态_已入账;
        }

        auto 写提交结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 允许提交, nullptr, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分候选(), 候选值, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分可结算状态(), 可结算状态, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分不可结算原因(), 不可结算原因, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分提交状态(), 提交状态, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分提交拒绝原因(), 允许提交 ? 0 : 不可结算原因, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分已入账状态(), 已入账状态, now);
            (void)写入基础节点I64(宿主, 特征_风险安全_场景影响部分变化方向(), 变化方向, now);
        };
        写提交结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写提交结果(reinterpret_cast<基础信息节点类*>(实际输出场景));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "提交_风险安全场景影响部分状态变化",
            结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_风险安全_场景影响部分提交状态(),
            "风险安全_场景影响部分提交状态");
        {
            std::ostringstream 输出;
            输出 << "提交_风险安全场景影响部分状态变化"
                << " | 成功=" << (允许提交 ? 1 : 0)
                << " | 候选值=" << 候选值
                << " | 可结算状态=" << 可结算状态
                << " | 不可结算原因=" << 不可结算原因
                << " | 变化方向=" << 变化方向
                << " | 提交状态=" << 提交状态
                << " | 已入账状态=" << 已入账状态
                << " | 缺条件拒绝=" << (!允许提交 ? 1 : 0)
                << " | 不写自我风险安全值=1"
                << " | 不写根安全值=1"
                << " | 不关闭安全根需求=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 允许提交, now);
        return 输出运行;
    }

    inline 存在节点类* 安全搜索安全因果因素证据(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::安全_搜索安全因果因素证据,
            输出结果场景,
            now,
            "安全_搜索安全因果因素证据",
            确认安全搜索安全因果因素证据规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        I64 层级 = 0;
        I64 因素ID = 0;
        I64 证据方向 = 安全因果证据方向_负向;
        I64 搜索范围完整状态 = 安全因果因素范围完整状态_未明确;
        I64 负向证据数量 = 0;
        I64 正向证据数量 = 0;
        I64 稳定性证据数量 = 0;
        I64 归因证据数量 = 0;
        I64 材料缺失数量 = 0;
        I64 冲突数量 = 0;
        I64 搜索置信度 = 0;
        I64 评估值候选 = 0;
        (void)读取输入参数I64(输入参数场景, 特征_安全因果层级(), 层级);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果因素ID(), 因素ID);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果证据方向(), 证据方向);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果因素搜索范围完整状态(), 搜索范围完整状态);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果因素负向证据数量(), 负向证据数量);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果因素正向证据数量(), 正向证据数量);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果因素稳定性证据数量(), 稳定性证据数量);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果因素归因证据数量(), 归因证据数量);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果因素材料缺失数量(), 材料缺失数量);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果因素冲突数量(), 冲突数量);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果因素搜索置信度(), 搜索置信度);
        (void)读取输入参数I64(输入参数场景, 特征_安全因果因素评估值候选(), 评估值候选);

        const bool 非场景风险因素搜索 =
            层级 == I64{2}
            && 证据方向 == 安全因果证据方向_负向
            && (因素ID & 风险安全因素掩码_非场景) != 0
            && (因素ID & (因素ID - I64{1})) == 0;
        if (非场景风险因素搜索
            && 搜索范围完整状态 != 安全因果因素范围完整状态_完整
            && 负向证据数量 <= 0
            && 材料缺失数量 <= 0
            && 冲突数量 <= 0) {
            搜索范围完整状态 = 安全因果因素范围完整状态_完整;
            搜索置信度 = 搜索置信度 > 0 ? 搜索置信度 : I64{10000};
            评估值候选 = 评估值候选 > 0 ? 评估值候选 : 场景绝对安全值;
            std::ostringstream 输出;
            输出 << "安全_搜索安全因果因素证据/非场景风险内部搜索完成"
                << " | 层级=" << 层级
                << " | 因素ID=" << 因素ID
                << " | 证据方向=" << 证据方向
                << " | 负向证据数量=0"
                << " | 搜索范围完整状态=" << 搜索范围完整状态
                << " | 搜索置信度=" << 搜索置信度
                << " | 不写默认满足=1"
                << " | 不写因素值=1";
            项目运行日志(输出.str());
        }

        auto 搜索结果 = 搜索安全因果因素证据(
            层级,
            因素ID,
            证据方向,
            搜索范围完整状态,
            负向证据数量,
            正向证据数量,
            稳定性证据数量,
            归因证据数量,
            材料缺失数量,
            冲突数量,
            搜索置信度,
            评估值候选);
        const bool 成功 = 搜索结果.搜索状态 == 安全因果因素搜索状态_已完成
            || 搜索结果.搜索状态 == 安全因果因素搜索状态_发现负向证据;

        结构_双目相机调用结果 结果{};
        结果.成功 = 成功;
        结果.失败原因 = 成功 ? nullptr : 值_执行缺口_当前方法条件不满足();
        结果.消息 = 成功
            ? "安全因果因素证据搜索完成"
            : "安全因果因素证据搜索缺口";

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_安全因果因素证据搜索状态(),
            搜索结果.搜索状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写搜索结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, nullptr, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_安全因果层级(), 搜索结果.层级, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素ID(), 搜索结果.因素ID, now);
            (void)写入基础节点I64(宿主, 特征_安全因果证据方向(), 搜索结果.证据方向, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素证据搜索状态(), 搜索结果.搜索状态, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素搜索范围完整状态(), 搜索结果.搜索范围完整状态, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素负向证据数量(), 搜索结果.负向证据数量, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素正向证据数量(), 搜索结果.正向证据数量, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素稳定性证据数量(), 搜索结果.稳定性证据数量, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素归因证据数量(), 搜索结果.归因证据数量, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素材料缺失数量(), 搜索结果.材料缺失数量, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素冲突数量(), 搜索结果.冲突数量, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素搜索置信度(), 搜索结果.搜索置信度, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素默认满足允许状态(), 搜索结果.默认满足允许状态, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素评估值候选(), 搜索结果.评估值候选, now);
        };
        写搜索结果(目标宿主);
        写搜索结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写搜索结果(reinterpret_cast<基础信息节点类*>(实际输出场景));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "安全_搜索安全因果因素证据",
            结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_安全因果因素证据搜索状态(),
            "安全因果因素证据搜索状态");
        {
            std::ostringstream 输出;
            输出 << "安全_搜索安全因果因素证据"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 层级=" << 搜索结果.层级
                << " | 因素ID=" << 搜索结果.因素ID
                << " | 证据方向=" << 搜索结果.证据方向
                << " | 搜索状态=" << 搜索结果.搜索状态
                << " | 搜索范围完整状态=" << 搜索结果.搜索范围完整状态
                << " | 负向证据数量=" << 搜索结果.负向证据数量
                << " | 材料缺失数量=" << 搜索结果.材料缺失数量
                << " | 冲突数量=" << 搜索结果.冲突数量
                << " | 默认满足允许状态=" << 搜索结果.默认满足允许状态
                << " | 不写安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 提交安全因果因素无负证据默认满足(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::提交_安全因果因素无负证据默认满足,
            输出结果场景,
            now,
            "提交_安全因果因素无负证据默认满足",
            确认提交安全因果因素无负证据默认满足规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        I64 搜索状态 = 安全因果因素搜索状态_未执行;
        I64 默认满足允许状态 = 安全因果因素默认满足允许状态_不允许;
        I64 负向证据数量 = 0;
        I64 材料缺失数量 = 0;
        I64 冲突数量 = 0;
        (void)读取基础节点I64(目标宿主, 特征_安全因果因素证据搜索状态(), 搜索状态);
        (void)读取基础节点I64(目标宿主, 特征_安全因果因素默认满足允许状态(), 默认满足允许状态);
        (void)读取基础节点I64(目标宿主, 特征_安全因果因素负向证据数量(), 负向证据数量);
        (void)读取基础节点I64(目标宿主, 特征_安全因果因素材料缺失数量(), 材料缺失数量);
        (void)读取基础节点I64(目标宿主, 特征_安全因果因素冲突数量(), 冲突数量);
        const bool 允许提交 =
            搜索状态 == 安全因果因素搜索状态_已完成
            && 默认满足允许状态 == 安全因果因素默认满足允许状态_允许
            && 负向证据数量 == 0
            && 材料缺失数量 == 0
            && 冲突数量 == 0;
        const I64 提交状态 = 允许提交
            ? 安全因果因素提交状态_已提交
            : 安全因果因素提交状态_条件不足;

        结构_双目相机调用结果 结果{};
        结果.成功 = 允许提交;
        结果.失败原因 = 允许提交 ? nullptr : 值_执行缺口_当前方法条件不满足();
        结果.消息 = 允许提交
            ? "安全因果因素无负证据默认满足已提交"
            : "安全因果因素无负证据默认满足提交条件不足";

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_安全因果因素默认满足提交状态(),
            提交状态,
            允许提交,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        if (允许提交) {
            (void)写入基础节点I64(目标宿主, 特征_安全因果因素值(), 场景绝对安全值, now);
            (void)写入基础节点I64(目标宿主, 特征_安全因果因素已入账状态(), 安全因果因素入账状态_已入账, now);
        }

        auto 写提交结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 允许提交, nullptr, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素默认满足提交状态(), 提交状态, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素值(), 允许提交 ? 场景绝对安全值 : 0, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素已入账状态(), 允许提交 ? 安全因果因素入账状态_已入账 : 安全因果因素入账状态_未入账, now);
        };
        写提交结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写提交结果(reinterpret_cast<基础信息节点类*>(实际输出场景));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "提交_安全因果因素无负证据默认满足",
            结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_安全因果因素默认满足提交状态(),
            "安全因果因素默认满足提交状态");
        {
            std::ostringstream 输出;
            输出 << "提交_安全因果因素无负证据默认满足"
                << " | 成功=" << (允许提交 ? 1 : 0)
                << " | 搜索状态=" << 搜索状态
                << " | 默认满足允许状态=" << 默认满足允许状态
                << " | 负向证据数量=" << 负向证据数量
                << " | 材料缺失数量=" << 材料缺失数量
                << " | 冲突数量=" << 冲突数量
                << " | 提交状态=" << 提交状态
                << " | 写入因素值=" << (允许提交 ? 场景绝对安全值 : 0)
                << " | 不写自我安全值=1"
                << " | 不写根安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 允许提交, now);
        return 输出运行;
    }

    inline 存在节点类* 提交安全因果因素证据评估值(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::提交_安全因果因素证据评估值,
            输出结果场景,
            now,
            "提交_安全因果因素证据评估值",
            确认提交安全因果因素证据评估值规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        I64 搜索状态 = 安全因果因素搜索状态_未执行;
        I64 候选值 = 0;
        I64 材料缺失数量 = 0;
        I64 冲突数量 = 0;
        (void)读取基础节点I64(目标宿主, 特征_安全因果因素证据搜索状态(), 搜索状态);
        const bool 候选已读取 = 读取基础节点I64(目标宿主, 特征_安全因果因素评估值候选(), 候选值);
        (void)读取基础节点I64(目标宿主, 特征_安全因果因素材料缺失数量(), 材料缺失数量);
        (void)读取基础节点I64(目标宿主, 特征_安全因果因素冲突数量(), 冲突数量);
        候选值 = 限制I64(候选值, I64{0}, 场景绝对安全值);
        const bool 允许提交 =
            候选已读取
            && (搜索状态 == 安全因果因素搜索状态_发现负向证据
                || 搜索状态 == 安全因果因素搜索状态_已完成)
            && 材料缺失数量 == 0
            && 冲突数量 == 0;
        const I64 提交状态 = 允许提交
            ? 安全因果因素提交状态_已提交
            : 安全因果因素提交状态_条件不足;

        结构_双目相机调用结果 结果{};
        结果.成功 = 允许提交;
        结果.失败原因 = 允许提交 ? nullptr : 值_执行缺口_当前方法条件不满足();
        结果.消息 = 允许提交
            ? "安全因果因素证据评估值已提交"
            : "安全因果因素证据评估值提交条件不足";

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_安全因果因素评估值提交状态(),
            提交状态,
            允许提交,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        if (允许提交) {
            (void)写入基础节点I64(目标宿主, 特征_安全因果因素值(), 候选值, now);
            (void)写入基础节点I64(目标宿主, 特征_安全因果因素已入账状态(), 安全因果因素入账状态_已入账, now);
        }

        auto 写提交结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 允许提交, nullptr, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素评估值提交状态(), 提交状态, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素值(), 允许提交 ? 候选值 : 0, now);
            (void)写入基础节点I64(宿主, 特征_安全因果因素已入账状态(), 允许提交 ? 安全因果因素入账状态_已入账 : 安全因果因素入账状态_未入账, now);
        };
        写提交结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写提交结果(reinterpret_cast<基础信息节点类*>(实际输出场景));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "提交_安全因果因素证据评估值",
            结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_安全因果因素评估值提交状态(),
            "安全因果因素评估值提交状态");
        {
            std::ostringstream 输出;
            输出 << "提交_安全因果因素证据评估值"
                << " | 成功=" << (允许提交 ? 1 : 0)
                << " | 搜索状态=" << 搜索状态
                << " | 候选已读取=" << (候选已读取 ? 1 : 0)
                << " | 候选值=" << 候选值
                << " | 材料缺失数量=" << 材料缺失数量
                << " | 冲突数量=" << 冲突数量
                << " | 提交状态=" << 提交状态
                << " | 不写自我安全值=1"
                << " | 不写根安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 允许提交, now);
        return 输出运行;
    }

    inline 存在节点类* 自我明确当前场景持续观察需求(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_明确当前场景持续观察需求,
            输出结果场景,
            now,
            "自我_明确当前场景持续观察需求",
            确认自我明确当前场景持续观察需求规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        清理执行输出派生需求标记(实际输出场景, "自我_明确当前场景持续观察需求");
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        I64 原持续观察需求明确状态 = 安全明确状态_未明确;
        (void)读取基础节点I64(
            目标宿主,
            特征_当前场景持续观察需求明确状态(),
            原持续观察需求明确状态);
        const bool 成功 = 目标宿主 != nullptr;
        const I64 持续观察需求明确状态 = 成功
            ? 安全明确状态_已明确
            : 安全明确状态_未明确;

        结构_双目相机调用结果 结果{};
        结果.成功 = 成功;
        结果.消息 = 成功
            ? "当前场景持续观察需求已明确"
            : "当前场景持续观察需求明确条件不足:缺少目标宿主";
        if (!成功) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_当前场景持续观察需求明确状态(),
            持续观察需求明确状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, nullptr, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(
                宿主,
                特征_当前场景持续观察需求明确状态(),
                持续观察需求明确状态,
                now);
        };
        写摘要(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写摘要(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写摘要(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_明确当前场景持续观察需求",
            结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_当前场景持续观察需求明确状态(),
            "当前场景持续观察需求明确状态");
        {
            std::ostringstream 输出;
            输出 << "自我_明确当前场景持续观察需求"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 原当前场景持续观察需求明确状态=" << 原持续观察需求明确状态
                << " | 当前场景持续观察需求明确状态=" << 持续观察需求明确状态
                << " | 由安全评估低安全线索承接=1"
                << " | 不启动外设线程=1"
                << " | 不直接重跑安全评估=1"
                << " | 不结算安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 安全持续观察当前场景(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::安全_持续观察当前场景,
            输出结果场景,
            now,
            "安全_持续观察当前场景",
            确认安全持续观察当前场景规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        清理执行输出派生需求标记(实际输出场景, "安全_持续观察当前场景");
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        const auto 当前场景存在集合 = 读取当前场景已确认存在集合(
            目标宿主,
            输入参数场景,
            实际输出场景);
        I64 持续观察需求明确状态 = 安全明确状态_未明确;
        (void)读取基础节点I64(
            目标宿主,
            特征_当前场景持续观察需求明确状态(),
            持续观察需求明确状态);
        const bool 成功 = 目标宿主 != nullptr
            && 持续观察需求明确状态 == 安全明确状态_已明确;
        const I64 持续观察状态 = 成功 ? 安全明确状态_已明确 : 安全明确状态_未明确;
        const I64 样本数量 = 成功
            ? std::max<I64>(I64{1}, static_cast<I64>(当前场景存在集合.size()))
            : I64{0};

        结构_双目相机调用结果 结果{};
        结果.成功 = 成功;
        if (!目标宿主) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "持续观察条件不足:缺少目标宿主";
        } else if (持续观察需求明确状态 != 安全明确状态_已明确) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "持续观察条件不足:持续观察需求未明确";
        } else {
            结果.消息 = "当前场景持续观察tick已记录";
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_当前场景持续观察状态(),
            持续观察状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, nullptr, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_当前场景持续观察状态(), 持续观察状态, now);
            (void)写入基础节点I64(宿主, 特征_当前场景持续观察样本数量(), 样本数量, now);
        };
        写摘要(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写摘要(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写摘要(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "安全_持续观察当前场景",
            结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_当前场景持续观察状态(),
            "当前场景持续观察状态");
        {
            std::ostringstream 输出;
            输出 << "安全_持续观察当前场景"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 当前场景持续观察需求明确状态=" << 持续观察需求明确状态
                << " | 当前场景持续观察状态=" << 持续观察状态
                << " | 当前场景持续观察样本数量=" << 样本数量
                << " | 当前场景存在数量=" << 当前场景存在集合.size()
                << " | 一次性tick=1"
                << " | 可被任务重复选择=1"
                << " | 不启动外设线程=1"
                << " | 不直接重跑安全评估=1"
                << " | 不结算安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我明确负向影响处置需求(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_明确负向影响处置需求,
            输出结果场景,
            now,
            "自我_明确负向影响处置需求",
            确认自我明确负向影响处置需求规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        清理执行输出派生需求标记(实际输出场景, "自我_明确负向影响处置需求");
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        I64 原处置需求明确状态 = 安全明确状态_未明确;
        I64 持续观察状态 = 安全明确状态_未明确;
        (void)读取基础节点I64(
            目标宿主,
            特征_负向影响处置需求明确状态(),
            原处置需求明确状态);
        (void)读取基础节点I64(
            目标宿主,
            特征_当前场景持续观察状态(),
            持续观察状态);
        const bool 成功 = 目标宿主 != nullptr
            && 持续观察状态 == 安全明确状态_已明确;
        const I64 处置需求明确状态 = 成功
            ? 安全明确状态_已明确
            : 安全明确状态_未明确;

        结构_双目相机调用结果 结果{};
        结果.成功 = 成功;
        if (!目标宿主) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "负向影响处置需求明确条件不足:缺少目标宿主";
        } else if (持续观察状态 != 安全明确状态_已明确) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "负向影响处置需求明确条件不足:持续观察尚未记录";
        } else {
            结果.消息 = "负向影响处置需求已明确";
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_负向影响处置需求明确状态(),
            处置需求明确状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, nullptr, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(
                宿主,
                特征_负向影响处置需求明确状态(),
                处置需求明确状态,
                now);
        };
        写摘要(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写摘要(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写摘要(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_明确负向影响处置需求",
            结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_负向影响处置需求明确状态(),
            "负向影响处置需求明确状态");
        {
            std::ostringstream 输出;
            输出 << "自我_明确负向影响处置需求"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 原负向影响处置需求明确状态=" << 原处置需求明确状态
                << " | 负向影响处置需求明确状态=" << 处置需求明确状态
                << " | 当前场景持续观察状态=" << 持续观察状态
                << " | 由低安全持续观察tick承接=1"
                << " | 不启动外设线程=1"
                << " | 不直接重跑安全评估=1"
                << " | 不执行降级保护=1"
                << " | 不结算安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 安全执行降级保护(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::安全_执行降级保护,
            输出结果场景,
            now,
            "安全_执行降级保护",
            确认安全执行降级保护规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        清理执行输出派生需求标记(实际输出场景, "安全_执行降级保护");
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        I64 处置需求明确状态 = 安全明确状态_未明确;
        I64 安全影响因果明确状态 = 安全明确状态_未明确;
        (void)读取基础节点I64(
            目标宿主,
            特征_负向影响处置需求明确状态(),
            处置需求明确状态);
        (void)读取基础节点I64(
            目标宿主,
            特征_安全影响因果明确状态(),
            安全影响因果明确状态);
        const bool 成功 = 目标宿主 != nullptr
            && 处置需求明确状态 == 安全明确状态_已明确
            && 安全影响因果明确状态 == 安全明确状态_已明确;
        const I64 降级保护状态 = 成功 ? 安全明确状态_已明确 : 安全明确状态_未明确;

        结构_双目相机调用结果 结果{};
        结果.成功 = 成功;
        if (!目标宿主) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "降级保护条件不足:缺少目标宿主";
        } else if (处置需求明确状态 != 安全明确状态_已明确) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "降级保护条件不足:处置需求未明确";
        } else if (安全影响因果明确状态 != 安全明确状态_已明确) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "降级保护条件不足:安全影响因果未明确";
        } else {
            结果.消息 = "降级保护已执行";
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_降级保护状态(),
            降级保护状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, nullptr, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_降级保护状态(), 降级保护状态, now);
        };
        写摘要(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写摘要(reinterpret_cast<基础信息节点类*>(实际输出场景));
        if (成功) {
            写摘要(目标宿主);
        }

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "安全_执行降级保护",
            结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_降级保护状态(),
            "降级保护状态");
        {
            std::ostringstream 输出;
            输出 << "安全_执行降级保护"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 负向影响处置需求明确状态=" << 处置需求明确状态
                << " | 安全影响因果明确状态=" << 安全影响因果明确状态
                << " | 降级保护状态=" << 降级保护状态
                << " | 方法头已登记=1"
                << " | 守门通过=" << (成功 ? 1 : 0)
                << " | 因果未明确不执行=" << (安全影响因果明确状态 != 安全明确状态_已明确 ? 1 : 0)
                << " | 不启动外设线程=1"
                << " | 不直接重跑安全评估=1"
                << " | 不结算安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我读取自我关键特征变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_读取自我关键特征变化,
            输出结果场景,
            now,
            "自我_读取自我关键特征变化",
            确认自我读取自我关键特征变化规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        const auto 当前场景存在集合 = 读取当前场景已确认存在集合(
            目标宿主,
            输入参数场景,
            实际输出场景);
        const I64 观察状态 = 安全明确状态_已明确;
        const I64 观察样本数量 = 当前场景存在集合.empty() ? 1 : static_cast<I64>(当前场景存在集合.size());
        const I64 负向变化数量 = 0;
        const bool 成功 = 目标宿主 != nullptr;

        结构_双目相机调用结果 结果{};
        结果.成功 = 成功;
        结果.消息 = 成功
            ? "自我关键特征变化观察窗口已建立"
            : "自我关键特征变化观察条件不足:缺少目标宿主";
        if (!成功) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_关键特征变化观察状态(),
            观察状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, nullptr, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_关键特征变化观察状态(), 观察状态, now);
            (void)写入基础节点I64(宿主, 特征_关键特征观察样本数量(), 观察样本数量, now);
            (void)写入基础节点I64(宿主, 特征_关键特征负向变化数量(), 负向变化数量, now);
        };
        写摘要(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写摘要(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写摘要(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_读取自我关键特征变化",
            结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_关键特征变化观察状态(),
            "关键特征变化观察状态");
        {
            std::ostringstream 输出;
            输出 << "自我_读取自我关键特征变化"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 关键特征变化观察状态=" << 观察状态
                << " | 关键特征观察样本数量=" << 观察样本数量
                << " | 关键特征负向变化数量=" << 负向变化数量
                << " | 不定义安全因素=1"
                << " | 不写基础风险明确=1"
                << " | 不结算安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 安全构造场景影响因果候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::安全_构造场景影响因果候选,
            输出结果场景,
            now,
            "安全_构造场景影响因果候选",
            确认安全构造场景影响因果候选规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        const auto 当前场景存在集合 = 读取当前场景已确认存在集合(
            目标宿主,
            输入参数场景,
            实际输出场景);
        I64 关键特征变化观察状态 = 安全明确状态_未明确;
        (void)读取基础节点I64(目标宿主, 特征_关键特征变化观察状态(), 关键特征变化观察状态);
        I64 搜索时间窗口 = 0;
        (void)读取输入参数I64(输入参数场景, 特征_安全因果搜索时间窗口(), 搜索时间窗口);
        if (搜索时间窗口 <= 0) {
            搜索时间窗口 = 最大允许当前观察年龄微秒;
        }

        const bool 条件可执行 = 目标宿主 != nullptr
            && !当前场景存在集合.empty()
            && 关键特征变化观察状态 == 安全明确状态_已明确;
        const auto 当前场景特征值材料门 = 评估当前场景特征值材料门(当前场景存在集合, now);
        auto 候选构造结果 = 条件可执行
            ? 构造安全因果候选条目_由存在特征值(当前场景特征值材料门, 搜索时间窗口)
            : 结构_安全因果候选构造结果{};
        const I64 构造状态 = 条件可执行 ? 候选构造结果.构造状态 : 安全明确状态_未明确;
        const bool 成功 = 条件可执行
            && (候选构造结果.候选数量 > 0 || 候选构造结果.缺当前特征值数量 > 0);

        结构_双目相机调用结果 结果{};
        结果.成功 = 成功;
        if (!目标宿主) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "安全因果候选构造条件不足:缺少目标宿主";
        } else if (当前场景存在集合.empty()) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "安全因果候选构造条件不足:缺少已确认场景存在";
        } else if (关键特征变化观察状态 != 安全明确状态_已明确) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "安全因果候选构造条件不足:缺少自我关键特征变化观察";
        } else if (候选构造结果.缺当前特征值数量 > 0) {
            结果.消息 = "安全因果候选构造不明确:缺少当前存在正式特征值";
        } else if (候选构造结果.搜索未完成数量 > 0) {
            结果.消息 = "安全因果候选条目已构造:等待证据搜索";
        } else if (候选构造结果.候选数量 <= 0) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "安全因果候选构造条件不足:没有可用存在特征值";
        } else {
            结果.消息 = "安全因果候选条目已构造且证据搜索闭合";
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            nullptr,
            特征_安全因果候选构造状态(),
            构造状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);
        写入安全因果候选条目明细(
            实际输出场景,
            候选构造结果,
            动态,
            now,
            候选构造结果.候选明细写入数量);

        auto 写摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, nullptr, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_安全因果候选构造状态(), 构造状态, now);
            (void)写入基础节点I64(宿主, 特征_安全因果候选数量(), 候选构造结果.候选数量, now);
            (void)写入基础节点I64(宿主, 特征_安全因果候选低置信数量(), 候选构造结果.低置信数量, now);
            (void)写入基础节点I64(宿主, 特征_安全因果候选归因置信度(), 候选构造结果.归因置信度, now);
            (void)写入基础节点I64(宿主, 特征_安全因果候选材料缺口数量(), 候选构造结果.缺当前特征值数量, now);
            (void)写入基础节点I64(宿主, 特征_安全因果候选搜索未完成数量(), 候选构造结果.搜索未完成数量, now);
        };
        写摘要(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写摘要(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写摘要(目标宿主);
        if (条件可执行 && 构造状态 != 安全明确状态_已明确) {
            if (候选构造结果.缺当前特征值数量 > 0) {
                写入宿主I64特征派生需求(
                    环境.方法首节点,
                    目标宿主,
                    特征_存在_场景绝对坐标明确状态(),
                    安全明确状态_未明确,
                    安全明确状态_已明确,
                    实际输出场景,
                    now,
                    "安全_构造场景影响因果候选");
            } else if (候选构造结果.搜索未完成数量 > 0) {
                写入宿主I64特征派生需求(
                    环境.方法首节点,
                    目标宿主,
                    特征_安全因果因素证据搜索状态(),
                    安全因果候选证据状态_未搜索,
                    安全因果候选证据状态_已完成无负向,
                    实际输出场景,
                    now,
                    "安全_构造场景影响因果候选");
            }
        }

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "安全_构造场景影响因果候选",
            结果,
            输入参数场景,
            实际输出场景,
            nullptr,
            目标宿主,
            动态,
            输出运行,
            特征_安全因果候选构造状态(),
            "安全因果候选构造状态");
        {
            std::ostringstream 输出;
            输出 << "安全_构造场景影响因果候选"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 安全因果候选构造状态=" << 构造状态
                << " | 当前场景存在数量=" << 当前场景存在集合.size()
                << " | 当前场景各存在特征值可读状态=" << 当前场景特征值材料门.状态
                << " | 安全因果候选数量=" << 候选构造结果.候选数量
                << " | 安全因果候选低置信数量=" << 候选构造结果.低置信数量
                << " | 安全因果候选归因置信度=" << 候选构造结果.归因置信度
                << " | 安全因果候选材料缺口数量=" << 候选构造结果.缺当前特征值数量
                << " | 安全因果候选搜索未完成数量=" << 候选构造结果.搜索未完成数量
                << " | 安全因果候选明细写入数量=" << 候选构造结果.候选明细写入数量
                << " | 首个缺失候选特征=" << (候选构造结果.首个缺失特征 ? 候选构造结果.首个缺失特征->获取主键() : std::string("空"))
                << " | 安全因果搜索时间窗口=" << 搜索时间窗口
                << " | 按存在数量直接生成低置信候选=0"
                << " | 无搜索不默认满足=1"
                << " | 不写基础风险明确=1"
                << " | 不结算安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我验证外设观察像素簇候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_验证外设观察像素簇候选,
            输出结果场景,
            now,
            "自我_验证外设观察像素簇候选",
            确认自我验证外设观察像素簇候选规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        清理执行输出派生需求标记(实际输出场景, "自我_验证外设观察像素簇候选");
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        const auto 报告可选 = 读取最新外设观察报告(
            "双目相机/D455深度相机",
            枚举_外设观察报告类型::逐簇识别报告);

        I64 报告ID = 0;
        I64 候选簇数量 = 0;
        I64 通过簇数量 = 0;
        I64 失败簇数量 = 0;
        I64 证据不足簇数量 = 0;
        I64 缺失材料数量 = 0;
        I64 缺失材料原因 = 外设观察验证缺失原因_无;
        I64 材料明确状态 = 0;
        I64 验证状态 = 外设观察候选验证状态_证据不足;

        auto 标记缺失 = [&](I64 原因) noexcept {
            if ((缺失材料原因 & 原因) == 0) {
                ++缺失材料数量;
            }
            缺失材料原因 |= 原因;
        };

        结构_双目相机调用结果 结果{};
        if (!报告可选.has_value()) {
            标记缺失(外设观察验证缺失原因_缺报告);
            结果.消息 = "外设观察像素簇候选验证条件不足:缺少D455逐簇识别报告";
        } else {
            const auto& 报告 = *报告可选;
            报告ID = static_cast<I64>(报告.报告ID);
            候选簇数量 = static_cast<I64>(报告.观察像素簇集合.size());

            auto 句柄可解引用 = [&](const std::string& 句柄, I64 缺失原因) {
                if (句柄.empty()) {
                    标记缺失(缺失原因);
                    return false;
                }
                const auto 摘要 = 解析D455观察材料句柄(句柄);
                if (!摘要.成功) {
                    标记缺失(缺失原因);
                    return false;
                }
                return true;
            };

            const bool 原始帧可解引用 = 句柄可解引用(
                报告.原始观察帧句柄,
                外设观察验证缺失原因_缺原始帧);
            const bool 深度帧可解引用 = 句柄可解引用(
                报告.深度帧句柄,
                外设观察验证缺失原因_缺深度帧);
            const bool 空间候选队列可解引用 = 句柄可解引用(
                报告.空间候选队列引用,
                外设观察验证缺失原因_缺空间候选);
            const bool 像素归属账本可解引用 = 句柄可解引用(
                报告.像素归属账本引用,
                外设观察验证缺失原因_缺像素归属材料);
            if (报告.观察像素簇集合.empty()) 标记缺失(外设观察验证缺失原因_簇集合为空);

            auto 簇投影在帧内 = [&](const 结构_外设观察像素簇摘要& 簇) noexcept {
                return 报告.帧宽度 > 0
                    && 报告.帧高度 > 0
                    && 簇.像素数量 > 0
                    && 簇.投影最小X >= 0
                    && 簇.投影最小Y >= 0
                    && 簇.投影最大X >= 簇.投影最小X
                    && 簇.投影最大Y >= 簇.投影最小Y
                    && 簇.投影最大X < 报告.帧宽度
                    && 簇.投影最大Y < 报告.帧高度;
            };

            I64 序号 = 0;
            for (const auto& 簇 : 报告.观察像素簇集合) {
                ++序号;
                auto 簇句柄可解引用 = [&](const std::string& 句柄) {
                    return !句柄.empty() && 解析D455观察材料句柄(句柄).成功;
                };
                const bool 有像素材料 =
                    簇句柄可解引用(簇.像素集合掩码句柄)
                    || 簇句柄可解引用(簇.ROI引用)
                    || 簇句柄可解引用(簇.点集引用);
                const bool 有像素归属材料 = 像素归属账本可解引用
                    && 簇句柄可解引用(簇.像素归属验证材料引用);
                const bool 有空间材料 = 空间候选队列可解引用
                    && 簇句柄可解引用(簇.空间候选引用)
                    && 簇.来源空间候选ID >= 0
                    && 簇.深度有效率 > 0
                    && 簇.空间坐标有效率 > 0;
                const bool 投影有效 = 原始帧可解引用
                    && 深度帧可解引用
                    && 簇投影在帧内(簇);

                I64 簇状态 = 外设观察候选验证状态_已验证;
                I64 簇缺失原因 = 外设观察验证缺失原因_无;
                if (!有像素材料 || !有像素归属材料) {
                    簇状态 = 外设观察候选验证状态_证据不足;
                    簇缺失原因 |= 外设观察验证缺失原因_缺像素归属材料;
                }
                if (!有空间材料) {
                    簇状态 = 外设观察候选验证状态_证据不足;
                    簇缺失原因 |= 外设观察验证缺失原因_缺空间候选;
                }
                if (!投影有效) {
                    簇状态 = 外设观察候选验证状态_失败;
                    簇缺失原因 |= 外设观察验证缺失原因_簇投影无效;
                }

                if (簇状态 == 外设观察候选验证状态_已验证) {
                    ++通过簇数量;
                } else if (簇状态 == 外设观察候选验证状态_失败) {
                    ++失败簇数量;
                } else {
                    ++证据不足簇数量;
                }
                if (簇缺失原因 != 外设观察验证缺失原因_无) {
                    缺失材料原因 |= 簇缺失原因;
                    ++缺失材料数量;
                }

                auto* 结果存在 = 世界树.取或创建子存在_按类型并按特征I64(
                    reinterpret_cast<基础信息节点类*>(实际输出场景),
                    类型_外设观察像素簇验证结果(),
                    特征_外设观察像素簇ID(),
                    簇.外设内部簇ID != 0 ? 簇.外设内部簇ID : 序号);
                auto* 节点 = reinterpret_cast<基础信息节点类*>(结果存在);
                if (节点) {
                    (void)写入基础节点I64(节点, 特征_外设观察报告ID(), 报告ID, now);
                    (void)写入基础节点I64(节点, 特征_外设观察像素簇ID(), 簇.外设内部簇ID, now);
                    (void)写入基础节点I64(节点, 特征_来源空间候选编号(), 簇.来源空间候选ID, now);
                    (void)写入基础节点I64(节点, 特征_外设像素簇候选验证状态(), 簇状态, now);
                    (void)写入基础节点I64(节点, 特征_外设观察像素簇候选已验证状态(), 簇状态, now);
                    (void)写入基础节点I64(节点, 特征_外设观察验证缺失材料原因(), 簇缺失原因, now);
                    (void)写入基础节点I64(节点, 特征_空间候选像素数量(), 簇.像素数量, now);
                    (void)写入基础节点I64(节点, 特征_深度有效率(), 簇.深度有效率, now);
                    (void)写入基础节点I64(节点, 特征_空间坐标有效率(), 簇.空间坐标有效率, now);
                    (void)写入基础节点I64(节点, 特征_空间连续性评分(), 簇.空间连续性评分, now);
                    (void)写入基础节点I64(节点, 特征_范围稳定性评分(), 簇.范围稳定性评分, now);
                    (void)写入基础节点I64(节点, 特征_像素归属验证状态(), 有像素归属材料 ? 像素归属验证状态_已生成待确认 : 像素归属验证状态_未生成, now);
                }
            }

            if (缺失材料原因 == 外设观察验证缺失原因_无
                && 候选簇数量 > 0
                && 通过簇数量 == 候选簇数量) {
                验证状态 = 外设观察候选验证状态_已验证;
                材料明确状态 = 1;
                结果.消息 = "外设观察像素簇候选验证通过:材料引用齐备且簇候选一致性通过";
            } else if (失败簇数量 > 0) {
                验证状态 = 外设观察候选验证状态_失败;
                结果.消息 = "外设观察像素簇候选验证失败:存在投影边界或空间材料不一致";
            } else {
                验证状态 = 外设观察候选验证状态_证据不足;
                结果.消息 = "外设观察像素簇候选验证证据不足:缺少可回查帧/空间候选/像素归属材料";
            }
        }

        const bool 成功 = 验证状态 == 外设观察候选验证状态_已验证;
        const I64 生成条件明确状态 = 成功 ? 1 : 0;
        结果.成功 = 成功;
        if (!成功) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_外设像素簇候选验证状态(),
            验证状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写验证结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_外设观察报告ID(), 报告ID, now);
            (void)写入基础节点I64(宿主, 特征_外设观察材料可回查状态(), 材料明确状态, now);
            (void)写入基础节点I64(宿主, 特征_外设像素簇候选验证状态(), 验证状态, now);
            (void)写入基础节点I64(宿主, 特征_外设观察像素簇候选已验证状态(), 验证状态, now);
            (void)写入基础节点I64(宿主, 特征_当前观察候选存在验证状态(), 验证状态, now);
            (void)写入基础节点I64(宿主, 特征_已确认观察存在生成条件明确状态(), 生成条件明确状态, now);
            (void)写入基础节点I64(宿主, 特征_外设观察验证材料明确状态(), 材料明确状态, now);
            (void)写入基础节点I64(宿主, 特征_外设观察验证候选簇数量(), 候选簇数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察验证通过簇数量(), 通过簇数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察验证失败簇数量(), 失败簇数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察验证证据不足簇数量(), 证据不足簇数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察验证缺失材料数量(), 缺失材料数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察验证缺失材料原因(), 缺失材料原因, now);
        };
        写验证结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写验证结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写验证结果(目标宿主);

        if (!成功) {
            写入宿主I64特征派生需求(
                环境.方法首节点,
                目标宿主,
                特征_外设像素簇候选验证状态(),
                验证状态,
                外设观察候选验证状态_已验证,
                实际输出场景,
                now,
                "自我_验证外设观察像素簇候选");
        }

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_验证外设观察像素簇候选",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_外设像素簇候选验证状态(),
            "外设像素簇候选验证状态");
        {
            std::ostringstream 输出;
            输出 << "外设观察像素簇候选验证结果"
                << " | 方法=自我_验证外设观察像素簇候选"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 报告ID=" << 报告ID
                << " | 候选簇数量=" << 候选簇数量
                << " | 通过簇数量=" << 通过簇数量
                << " | 失败簇数量=" << 失败簇数量
                << " | 证据不足簇数量=" << 证据不足簇数量
                << " | 缺失材料数量=" << 缺失材料数量
                << " | 缺失材料原因=" << 缺失材料原因
                << " | 外设观察材料可回查状态=" << 材料明确状态
                << " | 外设像素簇候选验证状态=" << 验证状态
                << " | 验证状态=" << 验证状态
                << " | 已确认观察存在生成条件明确状态=" << 生成条件明确状态
                << " | 不生成已确认观察存在=1"
                << " | 不写当前场景存在集合明确=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我确认外设观察存在候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_确认外设观察存在候选,
            输出结果场景,
            now,
            "自我_确认外设观察存在候选",
            确认自我确认外设观察存在候选规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        清理执行输出派生需求标记(实际输出场景, "自我_确认外设观察存在候选");
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        const auto 报告可选 = 读取最新外设观察报告(
            "双目相机/D455深度相机",
            枚举_外设观察报告类型::逐簇识别报告);

        I64 上游验证状态 = 外设观察候选验证状态_未验证;
        if (!读取基础节点I64(目标宿主, 特征_外设像素簇候选验证状态(), 上游验证状态)
            && !读取基础节点I64(目标宿主, 特征_外设观察像素簇候选已验证状态(), 上游验证状态)) {
            (void)读取基础节点I64(
                reinterpret_cast<基础信息节点类*>(实际输出场景),
                特征_外设像素簇候选验证状态(),
                上游验证状态);
            if (上游验证状态 == 外设观察候选验证状态_未验证) {
                (void)读取基础节点I64(
                    reinterpret_cast<基础信息节点类*>(实际输出场景),
                    特征_外设观察像素簇候选已验证状态(),
                    上游验证状态);
            }
        }

        结构_外设观察存在确认方案 方案{};
        if (报告可选.has_value()) {
            const auto 已知集合 = 读取当前场景已确认存在集合(
                目标宿主,
                输入参数场景,
                实际输出场景);
            方案 = 构造外设观察存在确认方案(
                *报告可选,
                上游验证状态 == 外设观察候选验证状态_已验证,
                已知集合);
        }

        const bool 成功 = 报告可选.has_value()
            && 上游验证状态 == 外设观察候选验证状态_已验证
            && 方案.方案取得状态 > 0;

        结构_双目相机调用结果 结果{};
        结果.成功 = 成功;
        if (!报告可选.has_value()) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "外设观察存在候选确认条件不足:缺少D455逐簇识别报告";
            方案.确认状态 = 外设观察存在候选确认状态_证据不足;
        } else if (上游验证状态 != 外设观察候选验证状态_已验证) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "外设观察存在候选确认条件不足:C5-3a验证结果未通过";
            方案.确认状态 = 外设观察存在候选确认状态_证据不足;
        } else {
            结果.消息 = "外设观察存在候选确认方案已生成";
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_外设观察存在候选确认方案取得状态(),
            方案.方案取得状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写方案摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_外设观察报告ID(), 方案.报告ID, now);
            (void)写入基础节点I64(宿主, 特征_外设观察存在候选确认状态(), 方案.确认状态, now);
            (void)写入基础节点I64(宿主, 特征_外设观察存在候选确认方案取得状态(), 方案.方案取得状态, now);
            (void)写入基础节点I64(宿主, 特征_外设观察验证候选簇数量(), 方案.候选簇数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察存在候选确认新建数量(), 方案.新建数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察存在候选确认合并数量(), 方案.合并数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察存在候选确认冲突数量(), 方案.冲突数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察存在候选确认证据不足数量(), 方案.证据不足数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察存在候选确认可提交数量(), 方案.可提交数量, now);
        };
        写方案摘要(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写方案摘要(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写方案摘要(目标宿主);

        I64 序号 = 0;
        for (const auto& 项 : 方案.项集合) {
            ++序号;
            auto* 方案存在 = 世界树.取或创建子存在_按类型并按特征I64(
                reinterpret_cast<基础信息节点类*>(实际输出场景),
                类型_外设观察存在候选确认方案(),
                特征_外设观察像素簇ID(),
                项.来源候选簇ID != 0 ? 项.来源候选簇ID : 序号);
            auto* 节点 = reinterpret_cast<基础信息节点类*>(方案存在);
            if (!节点) continue;
            (void)写入基础节点I64(节点, 特征_外设观察报告ID(), 方案.报告ID, now);
            (void)写入基础节点I64(节点, 特征_外设观察像素簇ID(), 项.来源候选簇ID, now);
            (void)写入基础节点I64(节点, 特征_来源空间候选编号(), 项.来源空间候选编号, now);
            (void)写入基础节点I64(节点, 特征_外设观察存在候选确认方案类型(), 项.方案类型, now);
            (void)写入基础节点I64(节点, 特征_外设观察存在候选确认允许提交状态(), 项.允许提交状态, now);
            (void)写入基础节点I64(节点, 特征_外设观察验证缺失材料原因(), 项.证据不足原因, now);
            (void)写入基础节点I64(节点, 特征_归属冲突像素数(), 项.冲突原因, now);
            (void)写入基础节点I64(节点, 特征_空间候选优先级评分(), 项.匹配分数, now);
            if (项.目标已知存在) {
                (void)写入基础节点指针(节点, 特征_目标已确认观察存在(), 项.目标已知存在, now);
            }
        }

        if (!成功) {
            写入宿主I64特征派生需求(
                环境.方法首节点,
                目标宿主,
                特征_外设观察像素簇候选已验证状态(),
                上游验证状态,
                外设观察候选验证状态_已验证,
                实际输出场景,
                now,
                "自我_确认外设观察存在候选");
        }

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_确认外设观察存在候选",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_外设观察存在候选确认方案取得状态(),
            "外设观察存在候选确认方案取得状态");
        {
            std::ostringstream 输出;
            输出 << "外设观察存在候选确认方案"
                << " | 方法=自我_确认外设观察存在候选"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 报告ID=" << 方案.报告ID
                << " | 输入通过簇=" << (上游验证状态 == 外设观察候选验证状态_已验证 ? 方案.候选簇数量 : 0)
                << " | 新建=" << 方案.新建数量
                << " | 合并=" << 方案.合并数量
                << " | 冲突=" << 方案.冲突数量
                << " | 证据不足=" << 方案.证据不足数量
                << " | 可提交=" << 方案.可提交数量
                << " | 不创建观察存在=1"
                << " | 不写当前场景存在集合明确=1"
                << " | 不结算安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 提交确认观察存在事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::提交_确认观察存在事实,
            输出结果场景,
            now,
            "提交_确认观察存在事实",
            确认提交确认观察存在事实规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        I64 方案取得状态 = 0;
        if (!读取基础节点I64(目标宿主, 特征_外设观察存在候选确认方案取得状态(), 方案取得状态)) {
            (void)读取基础节点I64(
                reinterpret_cast<基础信息节点类*>(实际输出场景),
                特征_外设观察存在候选确认方案取得状态(),
                方案取得状态);
        }

        const auto 报告可选 = 读取最新外设观察报告(
            "双目相机/D455深度相机",
            枚举_外设观察报告类型::逐簇识别报告);
        const auto 已知集合 = 读取当前场景已确认存在集合(
            目标宿主,
            输入参数场景,
            实际输出场景);
        const auto 方案 = 报告可选.has_value()
            ? 构造外设观察存在确认方案(*报告可选, 方案取得状态 > 0, 已知集合)
            : 结构_外设观察存在确认方案{};

        I64 新建数量 = 0;
        I64 合并数量 = 0;
        I64 已提交数量 = 0;
        I64 原始事实写入状态 = 0;
        I64 场景关联状态 = 0;
        struct 结构_已提交观察事实项 {
            存在节点类* 观察存在 = nullptr;
            I64 报告ID = 0;
            I64 簇ID = 0;
            I64 中心X = 0;
            I64 中心Y = 0;
            I64 中心Z = 0;
            I64 坐标置信度 = 0;
        };
        std::vector<结构_已提交观察事实项> 已提交项集合{};
        for (const auto& 项 : 方案.项集合) {
            if (!项.允许提交状态) continue;
            auto 假设 = 构建观察存在假设_外设簇(项);
            auto 验证 = 构建像素归属验证_外设簇(项);
            存在节点类* 观察存在 = nullptr;
            if (项.方案类型 == 外设观察存在候选确认方案_合并 && 项.目标已知存在) {
                观察存在 = 项.目标已知存在;
                ++合并数量;
            } else if (项.方案类型 == 外设观察存在候选确认方案_新建) {
                const I64 创建键 = 项.来源候选簇ID != 0 ? 项.来源候选簇ID : 假设.来源候选编号;
                观察存在 = 取或创建已确认观察存在(目标宿主, 创建键);
                ++新建数量;
            }
            auto* 节点 = reinterpret_cast<基础信息节点类*>(观察存在);
            if (!节点) continue;
            写入已确认观察存在节点原始事实(观察存在, 假设, 验证, now);
            (void)写入基础节点I64(节点, 特征_外设观察报告ID(), 方案.报告ID, now);
            (void)写入基础节点I64(节点, 特征_外设观察像素簇ID(), 项.来源候选簇ID, now);
            (void)写入基础节点I64(节点, 特征_深度帧号(), 报告可选 ? 报告可选->深度帧号 : 0, now);
            (void)写入基础节点I64(节点, 特征_彩色帧号(), 报告可选 ? 报告可选->彩色帧号 : 0, now);
            (void)写入基础节点I64(节点, 特征_候选像素掩码状态(), 项.簇.像素集合掩码状态, now);
            (void)写入基础节点I64(节点, 特征_观察存在原始事实写入状态(), 1, now);
            (void)写入基础节点I64(节点, 特征_观察存在与当前场景关联状态(), 1, now);
            ++已提交数量;
            原始事实写入状态 = 1;
            场景关联状态 = 1;
            已提交项集合.push_back({
                观察存在,
                方案.报告ID,
                项.来源候选簇ID,
                项.簇.中心X,
                项.簇.中心Y,
                项.簇.中心Z,
                计算场景坐标换算置信度(项),
            });
        }

        const I64 提交状态 = 已提交数量 <= 0
            ? 观察存在事实提交状态_条件不足
            : (已提交数量 == 方案.可提交数量 ? 观察存在事实提交状态_已提交 : 观察存在事实提交状态_部分提交);
        const bool 成功 = 提交状态 == 观察存在事实提交状态_已提交
            || 提交状态 == 观察存在事实提交状态_部分提交;

        结构_双目相机调用结果 结果{};
        结果.成功 = 成功;
        if (!成功) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "提交确认观察存在事实条件不足:没有允许提交的确认方案";
        } else {
            结果.消息 = "提交确认观察存在事实完成";
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_外设观察存在事实提交状态(),
            提交状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        I64 二次特征写入数量 = 0;
        auto* 输出场景节点 = reinterpret_cast<基础信息节点类*>(实际输出场景);
        for (const auto& 已提交项 : 已提交项集合) {
            if (写入观察存在提交关系二次特征(
                    实际输出场景,
                    已提交项.观察存在,
                    输出场景节点,
                    动态,
                    "观察存在属于当前场景存在",
                    1,
                    false)) {
                ++二次特征写入数量;
            }
            if (写入观察存在提交关系二次特征(
                    实际输出场景,
                    已提交项.观察存在,
                    reinterpret_cast<基础信息节点类*>(相机存在),
                    动态,
                    "观察存在来源于外设报告",
                    已提交项.报告ID,
                    true)) {
                ++二次特征写入数量;
            }
            if (写入观察存在提交关系二次特征(
                    实际输出场景,
                    已提交项.观察存在,
                    reinterpret_cast<基础信息节点类*>(相机存在),
                    动态,
                    "观察存在对应外设像素簇",
                    已提交项.簇ID,
                    true)) {
                ++二次特征写入数量;
            }
        }

        const I64 场景存在集合明确状态 = 成功
            && 方案.可提交数量 > 0
            && 方案.冲突数量 == 0
            && 方案.证据不足数量 == 0
                ? 安全明确状态_已明确
                : 安全明确状态_未明确;
        const auto 提交后当前场景存在集合 = 读取当前场景已确认存在集合(
            目标宿主,
            输入参数场景,
            实际输出场景);
        std::vector<存在节点类*> 场景绝对坐标参考集合{};
        for (auto* 参考存在 : 提交后当前场景存在集合) {
            Vector3D 参考绝对坐标{};
            if (世界树.读取存在场景绝对坐标(参考存在, 参考绝对坐标)) {
                场景绝对坐标参考集合.push_back(参考存在);
            }
        }

        I64 场景绝对坐标已有数量 = 0;
        I64 场景绝对坐标换算成功数量 = 0;
        I64 场景绝对坐标换算失败数量 = 0;
        for (const auto& 已提交项 : 已提交项集合) {
            auto* 节点 = reinterpret_cast<基础信息节点类*>(已提交项.观察存在);
            if (!节点) continue;

            Vector3D 已有绝对坐标{};
            if (世界树.读取存在场景绝对坐标(已提交项.观察存在, 已有绝对坐标)) {
                ++场景绝对坐标已有数量;
                (void)写入基础节点I64(
                    节点,
                    特征_外设_D455到场景绝对坐标变换明确状态(),
                    1,
                    now);
                continue;
            }

            const auto 相对坐标表 = 构造场景相对坐标表_同源观察(
                已提交项.观察存在,
                已提交项.中心X,
                已提交项.中心Y,
                已提交项.中心Z,
                已提交项.报告ID,
                场景绝对坐标参考集合,
                已提交项.坐标置信度);
            const bool 坐标写入成功 = 世界树.写入存在场景绝对坐标_由相对坐标表(
                已提交项.观察存在,
                相对坐标表,
                now);
            (void)写入基础节点I64(
                节点,
                特征_外设_D455到场景绝对坐标变换明确状态(),
                坐标写入成功 ? 1 : 0,
                now);
            if (坐标写入成功) {
                ++场景绝对坐标换算成功数量;
            } else {
                ++场景绝对坐标换算失败数量;
            }
        }
        const I64 D455到场景绝对坐标变换明确状态 =
            已提交数量 > 0
            && 场景绝对坐标换算失败数量 == 0
            && 场景绝对坐标已有数量 + 场景绝对坐标换算成功数量 >= 已提交数量
                ? 1
                : 0;

        const auto 当前场景特征值材料门 = 评估当前场景特征值材料门(提交后当前场景存在集合, now);
        const I64 当前场景各存在特征值可读状态 = 当前场景特征值材料门.状态;

        auto 写提交摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_外设观察报告ID(), 方案.报告ID, now);
            (void)写入基础节点I64(宿主, 特征_外设观察存在事实提交状态(), 提交状态, now);
            (void)写入基础节点I64(宿主, 特征_已确认观察存在提交入账状态(), 提交状态, now);
            (void)写入基础节点I64(宿主, 特征_观察存在原始事实写入状态(), 原始事实写入状态, now);
            (void)写入基础节点I64(宿主, 特征_观察存在与当前场景关联状态(), 场景关联状态, now);
            (void)写入基础节点I64(宿主, 特征_已验证观察存在数量(), 已提交数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察存在候选确认新建数量(), 新建数量, now);
            (void)写入基础节点I64(宿主, 特征_外设观察存在候选确认合并数量(), 合并数量, now);
            (void)写入基础节点I64(宿主, 特征_场景存在集合明确状态(), 场景存在集合明确状态, now);
            (void)写入基础节点I64(宿主, 特征_当前场景存在集合明确状态(), 场景存在集合明确状态, now);
            (void)写入基础节点I64(宿主, 特征_当前场景各存在特征值可读状态(), 当前场景各存在特征值可读状态, now);
            (void)写入基础节点I64(宿主, 特征_外设_D455到场景绝对坐标变换明确状态(), D455到场景绝对坐标变换明确状态, now);
            写入当前场景特征值材料门摘要(宿主, 当前场景特征值材料门, now);
        };
        写提交摘要(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写提交摘要(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写提交摘要(目标宿主);
        写入当前场景特征值材料门明细(实际输出场景, 当前场景特征值材料门, now);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "提交_确认观察存在事实",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_外设观察存在事实提交状态(),
            "外设观察存在事实提交状态");
        {
            std::ostringstream 输出;
            输出 << "提交_确认观察存在事实"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 来源报告ID=" << 方案.报告ID
                << " | 新建=" << 新建数量
                << " | 合并=" << 合并数量
                << " | 已提交=" << 已提交数量
                << " | 提交状态=" << 提交状态
                << " | 已确认观察存在提交入账状态=" << 提交状态
                << " | 写入观察原始事实状态=" << 原始事实写入状态
                << " | 更新当前场景索引=" << 场景关联状态
                << " | 写入二次特征数量=" << 二次特征写入数量
                << " | 场景存在集合明确状态=" << 场景存在集合明确状态
                << " | 当前场景存在集合明确状态=" << 场景存在集合明确状态
                << " | 当前场景各存在特征值可读状态=" << 当前场景各存在特征值可读状态
                << " | 当前场景特征值材料门版本=" << 当前场景特征值材料门.版本
                << " | 当前场景特征值可读存在数量=" << 当前场景特征值材料门.可读存在数量
                << " | 当前场景特征值部分可读存在数量=" << 当前场景特征值材料门.部分可读存在数量
                << " | 当前场景特征值证据不足存在数量=" << 当前场景特征值材料门.证据不足存在数量
                << " | 当前场景特征值缺失原因掩码=" << 当前场景特征值材料门.缺失原因掩码
                << " | 当前场景特征值过期原因掩码=" << 当前场景特征值材料门.过期原因掩码
                << " | 当前场景特征值冲突原因掩码=" << 当前场景特征值材料门.冲突原因掩码
                << " | 场景绝对坐标已有数量=" << 场景绝对坐标已有数量
                << " | 场景绝对坐标换算成功数量=" << 场景绝对坐标换算成功数量
                << " | 场景绝对坐标换算失败数量=" << 场景绝对坐标换算失败数量
                << " | 外设_D455到场景绝对坐标变换明确状态=" << D455到场景绝对坐标变换明确状态
                << " | D455空间材料不直接升格场景绝对坐标=1"
                << " | 不写基础风险明确=1"
                << " | 不结算安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我按已确认轮廓分区当前帧(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_按已确认轮廓分区当前帧,
            输出结果场景,
            now,
            "自我_按已确认轮廓分区当前帧",
            确认自我按已确认轮廓分区当前帧规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        const auto 报告可选 = 读取最新外设观察报告(
            "双目相机/D455深度相机",
            枚举_外设观察报告类型::逐簇识别报告);
        const auto 已知集合 = 读取当前场景已确认存在集合(
            目标宿主,
            输入参数场景,
            实际输出场景);
        const auto 分区 = 报告可选.has_value()
            ? 构造当前帧轮廓分区结果(*报告可选, 已知集合)
            : 结构_当前帧轮廓分区结果{};
        const bool 成功 = 分区.分区状态 == 当前帧轮廓分区状态_已生成
            || 分区.分区状态 == 当前帧轮廓分区状态_部分生成;

        结构_双目相机调用结果 结果{};
        结果.成功 = 成功;
        if (!报告可选.has_value()) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "当前帧轮廓分区条件不足:缺少下一帧D455逐簇识别报告";
        } else if (已知集合.empty()) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "当前帧轮廓分区条件不足:缺少已确认观察存在";
        } else if (!成功) {
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "当前帧轮廓分区仅形成条件不足结果";
        } else {
            结果.消息 = "当前帧轮廓分区结果已生成";
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_当前帧轮廓分区状态(),
            分区.分区状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        const I64 扫描状态 = 分区.已知扫描区数量 > 0
            ? 已知存在当前帧扫描状态_已生成
            : 已知存在当前帧扫描状态_待重识别;
        const I64 未知状态 = 分区.新识别区数量 > 0
            ? 未知区域识别状态_待识别
            : 未知区域识别状态_无未知区域;
        const I64 遮挡状态 = 分区.待重识别区数量 > 0
            ? 当前帧遮挡判断状态_可能遮挡
            : 当前帧遮挡判断状态_无遮挡证据;
        const I64 冲突状态 = 分区.冲突区数量 > 0
            ? 当前帧轮廓冲突区域明确状态_有冲突
            : 当前帧轮廓冲突区域明确状态_无冲突;
        const I64 区域归属状态 = 计算当前帧分区结果区域归属状态(分区);

        auto 写分区项集合 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            for (const auto& 项 : 分区.项集合) {
                auto* 分区存在 = 世界树.取或创建子存在_按类型并按特征I64(
                    宿主,
                    类型_当前帧轮廓分区项(),
                    特征_轮廓分区项序号(),
                    项.序号);
                auto* 节点 = reinterpret_cast<基础信息节点类*>(分区存在);
                if (!节点) continue;
                (void)写入基础节点I64(节点, 特征_轮廓分区项序号(), 项.序号, now);
                (void)写入基础节点指针(节点, 特征_来源已确认观察存在(), 项.已确认观察存在, now);
                (void)写入基础节点I64(节点, 特征_外设观察报告ID(), 分区.报告ID, now);
                (void)写入基础节点I64(节点, 特征_投影范围最小X(), 项.预测最小X, now);
                (void)写入基础节点I64(节点, 特征_投影范围最大X(), 项.预测最大X, now);
                (void)写入基础节点I64(节点, 特征_投影范围最小Y(), 项.预测最小Y, now);
                (void)写入基础节点I64(节点, 特征_投影范围最大Y(), 项.预测最大Y, now);
                (void)写入基础节点I64(节点, 特征_匹配外设观察像素簇ID(), 项.匹配簇ID, now);
                (void)写入基础节点I64(节点, 特征_轮廓匹配分数(), 项.匹配分数, now);
                (void)写入基础节点I64(节点, 特征_像素残差(), 项.像素残差, now);
                (void)写入基础节点I64(节点, 特征_深度残差(), 项.深度残差, now);
                (void)写入基础节点I64(节点, 特征_轮廓残差(), 项.轮廓残差, now);
                (void)写入基础节点I64(节点, 特征_已知存在当前帧扫描状态(), 项.扫描状态, now);
                (void)写入基础节点I64(节点, 特征_当前帧遮挡判断状态(), 项.遮挡状态, now);
                (void)写入基础节点I64(节点, 特征_当前帧轮廓冲突区域明确状态(), 项.冲突状态, now);
                (void)写入基础节点I64(
                    节点,
                    特征_当前帧区域归属状态(),
                    计算当前帧分区项区域归属状态(项),
                    now);
                (void)写入基础节点指针(节点, 特征_动作动态(), 动态, now);
            }
        };

        auto 写分区摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_外设观察报告ID(), 分区.报告ID, now);
            (void)写入基础节点I64(宿主, 特征_当前帧轮廓分区状态(), 分区.分区状态, now);
            (void)写入基础节点I64(宿主, 特征_已知存在当前帧扫描状态(), 扫描状态, now);
            (void)写入基础节点I64(宿主, 特征_未知区域识别状态(), 未知状态, now);
            (void)写入基础节点I64(宿主, 特征_当前帧遮挡判断状态(), 遮挡状态, now);
            (void)写入基础节点I64(宿主, 特征_当前帧轮廓冲突区域明确状态(), 冲突状态, now);
            (void)写入基础节点I64(宿主, 特征_当前帧像素归属闭合状态(), 分区.像素归属闭合状态, now);
            (void)写入基础节点I64(宿主, 特征_当前帧区域归属状态(), 区域归属状态, now);
            (void)写入基础节点I64(宿主, 特征_已验证观察存在数量(), 分区.已确认存在数量, now);
            (void)写入基础节点I64(宿主, 特征_已知存在扫描区数量(), 分区.已知扫描区数量, now);
            (void)写入基础节点I64(宿主, 特征_新识别区数量(), 分区.新识别区数量, now);
            (void)写入基础节点I64(宿主, 特征_冲突区数量(), 分区.冲突区数量, now);
            (void)写入基础节点I64(宿主, 特征_待重识别区数量(), 分区.待重识别区数量, now);
        };
        写分区摘要(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写分区摘要(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写分区摘要(目标宿主);
        写分区项集合(reinterpret_cast<基础信息节点类*>(实际输出场景));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_按已确认轮廓分区当前帧",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_当前帧轮廓分区状态(),
            "当前帧轮廓分区状态");
        {
            std::ostringstream 输出;
            输出 << "当前帧轮廓分区结果"
                << " | 方法=自我_按已确认轮廓分区当前帧"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 报告ID=" << 分区.报告ID
                << " | 当前帧ID=" << 分区.当前帧ID
                << " | 已确认存在=" << 分区.已确认存在数量
                << " | 已知扫描区=" << 分区.已知扫描区数量
                << " | 新识别区=" << 分区.新识别区数量
                << " | 冲突区=" << 分区.冲突区数量
                << " | 待重识别区=" << 分区.待重识别区数量
                << " | 像素归属闭合状态=" << 分区.像素归属闭合状态
                << " | 当前帧区域归属状态=" << 区域归属状态
                << " | 不是目标跟踪=1"
                << " | 不创建观察存在=1"
                << " | 不写基础风险明确=1"
                << " | 不结算安全值=1"
                << " | 输出场景=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(实际输出场景))
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态));
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我补观察质量占位方法(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        枚举_本能方法ID 本能ID,
        const char* 方法名,
        const char* 待实现说明) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            本能ID,
            输出结果场景,
            now,
            方法名,
            确认自我补观察质量占位规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        结构_双目相机调用结果 结果{};
        (void)读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        结果.成功 = false;
        结果.补观察缺口状态 = 1;
        结果.补观察需求建议 = 0;
        结果.失败原因 = 值_执行缺口_当前方法条件不满足();
        结果.消息 = std::string(方法名 ? 方法名 : "自我补观察质量占位方法")
            + "占位待实现:"
            + (待实现说明 ? 待实现说明 : "缺少执行接口");

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_补观察缺口状态(),
            结果.补观察缺口状态,
            false,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写占位结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, false, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_补观察缺口状态(), 结果.补观察缺口状态, now);
            (void)写入基础节点I64(宿主, 特征_补观察需求建议(), 结果.补观察需求建议, now);
        };
        写占位结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写占位结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写占位结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            方法名 ? 方法名 : "自我补观察质量占位方法",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_补观察缺口状态(),
            "补观察缺口状态");
        更新方法运行账(环境.方法首节点, 输出运行, false, now);
        return 输出运行;
    }

    inline 存在节点类* 自我诊断观察质量缺口并派生补观察需求(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_诊断观察质量缺口并派生补观察需求,
            输出结果场景,
            now,
            "自我_诊断观察质量缺口并派生补观察需求",
            确认自我诊断观察质量缺口并派生补观察需求规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        结构_双目相机调用结果 结果{};
        const bool 摘要闭合 = 读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        I64 质量诊断状态 = 摘要闭合 ? 观察质量诊断状态_已完成 : 观察质量诊断状态_未完成;
        I64 深度观察质量状态 = 观察质量状态_不足;
        I64 空间坐标观察质量状态 = 观察质量状态_不足;
        I64 质量缺口状态 = 观察质量缺口状态_存在;
        I64 质量缺口原因集合 = 0;
        I64 空间类后续处理适用状态 = 观察适用状态_不可用;
        I64 风险判断适用状态 = 观察适用状态_不可用;
        I64 存在确认适用状态 = 观察适用状态_不可用;
        I64 观察质量改善线索状态 = 观察质量改善线索状态_未生成;
        I64 观察质量改善候选线索 = 0;
        if (!摘要闭合) {
            结果.补观察缺口状态 = 1;
            结果.补观察缺失原因 = 补观察缺失原因_有效点不足;
            结果.补观察需求建议 = 0;
            结果.失败原因 = 值_执行缺口_当前方法条件不满足();
            结果.消息 = "观察质量缺口诊断条件不足:缺少已落账观察特征帧摘要";
        } else {
            I64 缺失原因 = 结果.补观察缺失原因;
            I64 候选线索 = 0;
            std::vector<const char*> 诊断项{};
            std::vector<const char*> 线索项{};
            auto 添加线索 = [&](I64 位, const char* 名称) noexcept {
                if ((候选线索 & 位) == 0 && 名称) {
                    线索项.push_back(名称);
                }
                候选线索 |= 位;
            };

            if (结果.观察帧质量诊断摘要 != 1) {
                诊断项.push_back("观察帧质量诊断摘要缺失");
                添加线索(补观察建议_重新取得观察特征帧, "重新取得观察特征帧");
            }
            if (结果.帧质量评分 < 观察质量最低评分) {
                诊断项.push_back("帧质量评分不足");
                添加线索(补观察建议_重新取得观察特征帧, "重新取得观察特征帧");
            }
            if (结果.深度有效率 < 观察质量最低深度有效率) {
                诊断项.push_back("深度有效率不足");
                缺失原因 |= 补观察缺失原因_有效点不足;
                添加线索(补观察建议_重新取得观察特征帧, "重新取得观察特征帧");
                添加线索(补观察建议_调整双目相机工作模式, "调整双目相机工作模式");
                添加线索(补观察建议_缩小ROI重新观察, "缩小ROI重新观察");
            }
            if (结果.空间坐标有效率 < 观察质量最低空间坐标有效率) {
                诊断项.push_back("空间坐标有效率不足");
                缺失原因 |= 补观察缺失原因_有效点不足;
                添加线索(补观察建议_重新取得观察特征帧, "重新取得观察特征帧");
                添加线索(补观察建议_调整双目相机工作模式, "调整双目相机工作模式");
                添加线索(补观察建议_缩小ROI重新观察, "缩小ROI重新观察");
            }
            if (结果.融合深度有效率 > 0
                && 结果.深度有效率 > 0
                && 结果.融合深度有效率 >= 结果.深度有效率 + 1000) {
                诊断项.push_back("融合补偿明显");
                缺失原因 |= 补观察缺失原因_补全依赖过高;
                添加线索(补观察建议_融合稳定观察帧深度, "融合稳定观察帧深度");
                添加线索(补观察建议_重新取得观察特征帧, "重新取得观察特征帧");
            }
            if (结果.未解释区域比例 > 0 || 结果.待补观察区域数量 > 0) {
                诊断项.push_back("未解释区域或待补观察区域存在");
                if (结果.补观察缺失原因 == 0) {
                    缺失原因 |= 补观察缺失原因_深度空洞;
                }
                添加线索(补观察建议_继续观察未解释区域, "继续观察未解释区域");
                添加线索(补观察建议_缩小ROI重新观察, "缩小ROI重新观察");
            }
            if (结果.低置信轮廓数量 > 0 || 结果.依赖补全深度轮廓数量 > 0) {
                诊断项.push_back("低置信补偿候选存在");
                缺失原因 |= 补观察缺失原因_补全依赖过高;
                添加线索(补观察建议_内部世界细分, "内部世界细分");
            }
            深度观察质量状态 = 结果.深度有效率 >= 观察质量最低深度有效率
                ? 观察质量状态_可用
                : 观察质量状态_不足;
            空间坐标观察质量状态 = 结果.空间坐标有效率 >= 观察质量最低空间坐标有效率
                ? 观察质量状态_可用
                : 观察质量状态_不足;
            const bool 用途相关质量缺口 =
                深度观察质量状态 != 观察质量状态_可用
                || 空间坐标观察质量状态 != 观察质量状态_可用
                || 缺失原因 != 0
                || 结果.待补观察区域数量 > 0;
            if (!用途相关质量缺口 && 诊断项.empty() && 结果.补观察缺口状态 <= 0) {
                诊断项.push_back("当前观察帧质量已满足");
            }

            结果.补观察缺口状态 = 用途相关质量缺口 ? 1 : 0;
            结果.补观察缺失原因 = 缺失原因;
            结果.补观察需求建议 = 0;
            质量缺口状态 = 用途相关质量缺口
                ? 观察质量缺口状态_存在
                : 观察质量缺口状态_不存在;
            质量缺口原因集合 = 缺失原因;
            观察质量改善候选线索 = 候选线索;
            观察质量改善线索状态 = 候选线索 != 0
                ? 观察质量改善线索状态_已生成
                : 观察质量改善线索状态_未生成;
            const bool 深度或空间有材料 =
                结果.深度有效率 > 0
                || 结果.空间坐标有效率 > 0
                || 结果.深度有效像素数量 > 0
                || 结果.点云有效像素数量 > 0;
            空间类后续处理适用状态 =
                深度观察质量状态 == 观察质量状态_可用
                && 空间坐标观察质量状态 == 观察质量状态_可用
                ? 观察适用状态_可用
                : (深度或空间有材料 ? 观察适用状态_降级可用 : 观察适用状态_不可用);
            风险判断适用状态 =
                结果.质量可用状态 == 观察事实状态_可用
                && 空间类后续处理适用状态 == 观察适用状态_可用
                ? 观察适用状态_可用
                : (深度或空间有材料 ? 观察适用状态_降级可用 : 观察适用状态_不可用);
            存在确认适用状态 = 空间类后续处理适用状态;

            std::ostringstream 摘要;
            摘要 << "观察质量缺口诊断完成"
                << " | 质量诊断状态=" << 质量诊断状态
                << " | 当前帧保留可用事实=1"
                << " | 深度观察质量状态=" << 深度观察质量状态
                << " | 空间坐标观察质量状态=" << 空间坐标观察质量状态
                << " | 质量缺口状态=" << 质量缺口状态
                << " | 质量缺口原因集合=" << 质量缺口原因集合
                << " | 空间类后续处理适用状态=" << 空间类后续处理适用状态
                << " | 风险判断适用状态=" << 风险判断适用状态
                << " | 存在确认适用状态=" << 存在确认适用状态
                << " | 观察质量改善线索状态=" << 观察质量改善线索状态;
            if (!诊断项.empty()) {
                摘要 << " | 诊断项=";
                for (std::size_t i = 0; i < 诊断项.size(); ++i) {
                    if (i > 0) 摘要 << ';';
                    摘要 << 诊断项[i];
                }
            }
            if (!线索项.empty()) {
                摘要 << " | 新增候选方法召回线索=";
                for (std::size_t i = 0; i < 线索项.size(); ++i) {
                    if (i > 0) 摘要 << ';';
                    摘要 << 线索项[i];
                }
            }
            摘要 << " | 观察质量改善候选线索=" << 观察质量改善候选线索
                << " | 正式需求入树=0"
                << " | 父需求阻塞裁决=0";
            结果.消息 = 摘要.str();
        }

        const bool 成功 = 摘要闭合;
        结果.成功 = 成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_质量诊断状态(),
            质量诊断状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写诊断结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            (void)写入基础节点I64(宿主, 特征_质量诊断状态(), 质量诊断状态, now);
            (void)写入基础节点I64(宿主, 特征_深度有效率(), 结果.深度有效率, now);
            (void)写入基础节点I64(宿主, 特征_空间坐标有效率(), 结果.空间坐标有效率, now);
            (void)写入基础节点I64(宿主, 特征_深度观察质量状态(), 深度观察质量状态, now);
            (void)写入基础节点I64(宿主, 特征_空间坐标观察质量状态(), 空间坐标观察质量状态, now);
            (void)写入基础节点I64(宿主, 特征_质量缺口状态(), 质量缺口状态, now);
            (void)写入基础节点I64(宿主, 特征_质量缺口原因集合(), 质量缺口原因集合, now);
            (void)写入基础节点I64(宿主, 特征_空间类后续处理适用状态(), 空间类后续处理适用状态, now);
            (void)写入基础节点I64(宿主, 特征_风险判断适用状态(), 风险判断适用状态, now);
            (void)写入基础节点I64(宿主, 特征_存在确认适用状态(), 存在确认适用状态, now);
            (void)写入基础节点I64(宿主, 特征_观察质量改善线索状态(), 观察质量改善线索状态, now);
            (void)写入基础节点I64(宿主, 特征_观察质量改善候选线索(), 观察质量改善候选线索, now);
            (void)写入基础节点I64(宿主, 特征_补观察缺口状态(), 结果.补观察缺口状态, now);
            (void)写入基础节点I64(宿主, 特征_补观察缺失原因(), 结果.补观察缺失原因, now);
            (void)写入基础节点I64(宿主, 特征_待补观察区域数量(), 结果.待补观察区域数量, now);
        };
        写诊断结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写诊断结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写诊断结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_诊断观察质量缺口并派生补观察需求",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行,
            特征_质量诊断状态(),
            "质量诊断状态");
        项目运行日志(
            std::string("观察质量诊断事实入账 | ")
            + 结果.消息
            + " | 直接写需求树=0"
            + " | 写质量可用状态=0"
            + " | 写基础观察事实可用状态=0"
            + " | 写基础风险判断状态已明确=0");
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我调整相机参数并重采样(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 自我补观察质量占位方法(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_调整相机参数并重采样,
            "自我_调整相机参数并重采样",
            "缺少安全参数搜索和真实重采样执行接口");
    }

    inline 存在节点类* 自我按ROI重算观察质量(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 自我补观察质量占位方法(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_按ROI重算观察质量,
            "自我_按ROI重算观察质量",
            "缺少ROI选择和局部质量重算接口");
    }

    inline 存在节点类* 自我执行多帧重采样并择优(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 自我补观察质量占位方法(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_执行多帧重采样并择优,
            "自我_执行多帧重采样并择优",
            "缺少多帧采集、评分和择优落账接口");
    }

    inline 存在节点类* 自我生成低置信补偿观察候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 自我补观察质量占位方法(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_生成低置信补偿观察候选,
            "自我_生成低置信补偿观察候选",
            "缺少低置信候选生成和待验证证据落账接口");
    }

    inline 存在节点类* 自我调整外设观察姿态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 自我补观察质量占位方法(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_调整外设观察姿态,
            "自我_调整外设观察姿态",
            "缺少可控云台、舵机或相机姿态接口");
    }

    inline 存在节点类* 自我调整观察位置(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 自我补观察质量占位方法(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_调整观察位置,
            "自我_调整观察位置",
            "缺少可控移动底盘、机械臂或可确认位移接口");
    }

    inline bool 读取像素坐标参数(
        场景节点类* 输入参数场景,
        I64& 像素X,
        I64& 像素Y) noexcept
    {
        const bool 有X =
            读取场景I64特征(输入参数场景, 特征_像素坐标X(), 像素X)
            || 读取输入包I64(输入参数场景, 特征_输入条件包(), 特征_像素坐标X(), 像素X)
            || 读取输入包I64(输入参数场景, 特征_运行参数(), 特征_像素坐标X(), 像素X)
            || 读取输入包I64(输入参数场景, 特征_任务参数包(), 特征_像素坐标X(), 像素X);
        const bool 有Y =
            读取场景I64特征(输入参数场景, 特征_像素坐标Y(), 像素Y)
            || 读取输入包I64(输入参数场景, 特征_输入条件包(), 特征_像素坐标Y(), 像素Y)
            || 读取输入包I64(输入参数场景, 特征_运行参数(), 特征_像素坐标Y(), 像素Y)
            || 读取输入包I64(输入参数场景, 特征_任务参数包(), 特征_像素坐标Y(), 像素Y);
        return 有X && 有Y;
    }

    inline 存在节点类* 自我按像素坐标回查观察事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_按像素坐标回查观察事实,
            输出结果场景,
            now,
            "自我_按像素坐标回查观察事实",
            确认自我按像素坐标回查观察事实规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        I64 像素X = 0;
        I64 像素Y = 0;
        const bool 坐标闭合 = 读取像素坐标参数(输入参数场景, 像素X, 像素Y);
        结构_双目相机调用结果 结果{};
        const bool 观察帧闭合 = 坐标闭合
            && 读取已落账观察特征帧摘要(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const bool 像素索引闭合 = 观察帧闭合
            && 已落账像素观察事实索引(目标宿主, 输入参数场景, 实际输出场景);
        const bool 坐标在范围 =
            坐标闭合
            && 结果.成功
            && 像素X >= 0
            && 像素Y >= 0
            && 像素X < static_cast<I64>(结果.宽度)
            && 像素Y < static_cast<I64>(结果.高度);
        const bool 成功 = 坐标在范围 && 像素索引闭合 && 结果.像素覆盖状态 == 1;
        const auto* 失败原因 = 坐标闭合
            ? (结果.失败原因 ? 结果.失败原因 : (成功 ? nullptr : 值_执行失败()))
            : 值_执行缺口_可执行输入参数场景缺失();
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_像素坐标对应像素观察事实(),
            成功 ? 1 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 失败原因, now);
            if (!宿主) return;
            if (坐标闭合) {
                (void)写入基础节点I64(宿主, 特征_像素坐标X(), 像素X, now);
                (void)写入基础节点I64(宿主, 特征_像素坐标Y(), 像素Y, now);
            }
            if (结果.成功) {
                写入双目相机像素特征索引摘要(宿主, 结果, now);
            }
            (void)写入基础节点I64(宿主, 特征_像素坐标对应像素观察事实(), 成功 ? 1 : 0, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        auto 记录结果 = 结果;
        记录结果.成功 = 成功;
        记录结果.失败原因 = 失败原因;
        记录双目相机本能动作结果(
            "自我_按像素坐标回查观察事实",
            记录结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    enum class 枚举_内部世界动作阶段 : std::uint8_t {
        确保内部世界,
        建立内外接口,
        映射局部切片,
        提取子候选,
        确认子存在,
        递归细分
    };

    inline 存在节点类* 执行自我内部世界动作(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        枚举_本能方法ID 本能ID,
        const char* 方法名,
        bool (*确认规格)(方法类::节点类*, 存在节点类*, 时间戳) noexcept,
        枚举_内部世界动作阶段 阶段,
        const 语素入口节点类* 主结果特征,
        I64 主结果值) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            本能ID,
            输出结果场景,
            now,
            方法名,
            确认规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 默认宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        auto* 目标存在 = 解析目标观察存在(输入参数场景, 默认宿主, 实际输出场景);

        auto 上下文 = 读取内部世界上下文(目标存在, true, now);
        bool 成功 = 上下文.目标存在 && 上下文.内部世界;
        if (成功) {
            写入内部世界基础摘要(上下文, now);
        }
        if (成功 && 阶段 >= 枚举_内部世界动作阶段::建立内外接口) {
            成功 = 取或创建内外接口(上下文, now) != nullptr;
        }
        if (成功 && 阶段 >= 枚举_内部世界动作阶段::映射局部切片) {
            if (!上下文.内外接口) {
                (void)取或创建内外接口(上下文, now);
            }
            成功 = 取或创建局部观察切片(上下文, now) != nullptr;
        }
        if (成功 && 阶段 >= 枚举_内部世界动作阶段::提取子候选) {
            if (!上下文.局部切片) {
                (void)取或创建局部观察切片(上下文, now);
            }
            成功 = 取或创建内部世界子候选(上下文, now) != nullptr;
        }
        结构_内部世界子存在确认方案 内部确认方案{};
        if (成功 && 阶段 >= 枚举_内部世界动作阶段::确认子存在) {
            if (!上下文.子候选) {
                (void)取或创建内部世界子候选(上下文, now);
            }
            内部确认方案 = 构造内部世界子存在确认方案(上下文);
            成功 = 内部确认方案.方案取得状态 > 0 && 内部确认方案.允许提交状态 > 0;
            if (成功) {
                auto* 内部节点 = reinterpret_cast<基础信息节点类*>(上下文.内部世界);
                auto* 子候选节点 = reinterpret_cast<基础信息节点类*>(上下文.子候选);
                (void)写入基础节点I64(内部节点, 特征_内部世界子候选验证状态(), 内部世界子候选验证状态_已验证, now);
                (void)写入基础节点I64(子候选节点, 特征_内部世界子候选验证状态(), 内部世界子候选验证状态_已验证, now);
            }
        }
        if (成功 && 阶段 >= 枚举_内部世界动作阶段::递归细分) {
            if (!上下文.子存在) {
                成功 = false;
            }
        }
        if (成功 && 阶段 >= 枚举_内部世界动作阶段::递归细分) {
            auto* 内部节点 = reinterpret_cast<基础信息节点类*>(上下文.内部世界);
            const I64 细分状态 = 局部区域达到叶子阈值(上下文)
                ? 内部世界细分状态_已完成
                : 内部世界细分状态_部分完成;
            (void)写入基础节点I64(内部节点, 特征_内部世界细分状态(), 细分状态, now);
            (void)写入基础节点I64(内部节点, 特征_细分层级数(), 1, now);
        }

        const auto* 失败原因 = 成功 ? nullptr : 值_执行缺口_当前方法条件不满足();
        auto* 动态宿主 = reinterpret_cast<基础信息节点类*>(
            上下文.内部世界
                ? static_cast<基础信息节点类*>(上下文.内部世界)
                : (上下文.目标存在
                    ? static_cast<基础信息节点类*>(上下文.目标存在)
                    : 默认宿主));
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            动态宿主,
            nullptr,
            主结果特征,
            成功 ? 主结果值 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入内部世界方法结果(
                宿主,
                成功,
                上下文,
                动态,
                失败原因,
                now,
                阶段 >= 枚举_内部世界动作阶段::确认子存在,
                &内部确认方案);
            if (!宿主 || !成功) return;
            (void)写入基础节点I64(宿主, 主结果特征, 主结果值, now);
            if (阶段 >= 枚举_内部世界动作阶段::递归细分 && 上下文.内部世界) {
                const I64 细分状态 = 局部区域达到叶子阈值(上下文)
                    ? 内部世界细分状态_已完成
                    : 内部世界细分状态_部分完成;
                (void)写入基础节点I64(宿主, 特征_内部世界细分状态(), 细分状态, now);
            }
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(reinterpret_cast<基础信息节点类*>(上下文.目标存在));
        写结果(reinterpret_cast<基础信息节点类*>(上下文.内部世界));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        {
            std::ostringstream 输出;
            输出 << 方法名
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 目标存在=" << (上下文.目标存在 ? 上下文.目标存在->获取主键() : std::string{})
                << " | 内部世界=" << (上下文.内部世界 ? 上下文.内部世界->获取主键() : std::string{})
                << " | 内外接口=" << (上下文.内外接口 ? 上下文.内外接口->获取主键() : std::string{})
                << " | 局部切片=" << (上下文.局部切片 ? 上下文.局部切片->获取主键() : std::string{})
                << " | 子候选=" << (上下文.子候选 ? 上下文.子候选->获取主键() : std::string{})
                << " | 子存在=" << (上下文.子存在 ? 上下文.子存在->获取主键() : std::string{})
                << " | 投影宽高=" << 上下文.投影宽度 << "x" << 上下文.投影高度
                << " | 存在内部世界局部材料建立状态=" << (上下文.局部切片 ? 内部世界局部材料建立状态_已建立 : 内部世界局部材料建立状态_未建立)
                << " | 内部世界子候选集合生成状态=" << (上下文.子候选 ? 子候选集合状态_已生成 : 0)
                << " | 内部世界子候选验证状态=" << (上下文.子候选
                    ? (阶段 >= 枚举_内部世界动作阶段::确认子存在 && 成功
                        ? 内部世界子候选验证状态_已验证
                        : 内部世界子候选验证状态_待验证)
                    : 内部世界子候选验证状态_未验证)
                << " | 内部世界局部轮廓材料可回查状态=" << 上下文.局部轮廓材料可回查状态
                << " | 局部ROI句柄=" << 上下文.ROI句柄
                << " | 局部掩码句柄=" << 上下文.掩码句柄
                << " | 局部点集句柄=" << 上下文.点集句柄
                << " | 局部彩色图句柄=" << 上下文.彩色局部图句柄
                << " | 局部深度图句柄=" << 上下文.深度局部图句柄
                << " | 未进入子存在确认阶段=" << (阶段 < 枚举_内部世界动作阶段::确认子存在 ? 1 : 0)
                << " | 对象方法不创建内部世界子存在=" << (阶段 >= 枚举_内部世界动作阶段::确认子存在 ? 1 : 0)
                << " | 不写基础风险明确=1"
                << " | 不结算安全值=1"
                << " | 失败原因=" << (失败原因 ? 失败原因->获取主键() : std::string{});
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我确保存在内部世界(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我内部世界动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_确保存在内部世界,
            "自我_确保存在内部世界",
            确认自我确保存在内部世界规格,
            枚举_内部世界动作阶段::确保内部世界,
            特征_内部世界(),
            1);
    }

    inline 存在节点类* 自我建立存在内外场景接口(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我内部世界动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_建立存在内外场景接口,
            "自我_建立存在内外场景接口",
            确认自我建立存在内外场景接口规格,
            枚举_内部世界动作阶段::建立内外接口,
            特征_存在内外场景接口(),
            1);
    }

    inline 存在节点类* 自我将存在局部观察切片映射到内部世界(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我内部世界动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_将存在局部观察切片映射到内部世界,
            "自我_将存在局部观察切片映射到内部世界",
            确认自我将存在局部观察切片映射到内部世界规格,
            枚举_内部世界动作阶段::映射局部切片,
            特征_内部世界当前观察材料(),
            当前观察材料状态_已建立);
    }

    inline 存在节点类* 自我在内部世界提取子候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我内部世界动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_在内部世界提取子候选,
            "自我_在内部世界提取子候选",
            确认自我在内部世界提取子候选规格,
            枚举_内部世界动作阶段::提取子候选,
            特征_内部世界子候选集合(),
            子候选集合状态_已生成);
    }

    inline 存在节点类* 自我确认内部世界子存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我内部世界动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_确认内部世界子存在,
            "自我_确认内部世界子存在",
            确认自我确认内部世界子存在规格,
            枚举_内部世界动作阶段::确认子存在,
            特征_内部世界子存在候选确认方案取得状态(),
            1);
    }

    inline 存在节点类* 提交确认内部世界子存在事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::提交_确认内部世界子存在事实,
            输出结果场景,
            now,
            "提交_确认内部世界子存在事实",
            确认提交确认内部世界子存在事实规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 默认宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        auto* 目标存在 = 解析目标观察存在(输入参数场景, 默认宿主, 实际输出场景);
        auto 上下文 = 读取内部世界上下文(目标存在, false, now);

        auto 读取确认值 = [&](const 语素入口节点类* 特征, I64& 值) noexcept {
            return 读取基础节点I64(reinterpret_cast<基础信息节点类*>(上下文.目标存在), 特征, 值)
                || 读取基础节点I64(reinterpret_cast<基础信息节点类*>(上下文.内部世界), 特征, 值)
                || 读取基础节点I64(reinterpret_cast<基础信息节点类*>(实际输出场景), 特征, 值);
        };

        I64 方案取得状态 = 0;
        I64 可提交数量 = 0;
        (void)读取确认值(特征_内部世界子存在候选确认方案取得状态(), 方案取得状态);
        (void)读取确认值(特征_内部世界子存在候选确认可提交数量(), 可提交数量);

        auto 方案 = 构造内部世界子存在确认方案(上下文);
        const bool 允许提交 = 方案取得状态 > 0
            && 可提交数量 > 0
            && 方案.允许提交状态 > 0;

        存在节点类* 子存在 = nullptr;
        I64 原始事实写入状态 = 0;
        I64 内部世界关联状态 = 0;
        I64 提交状态 = 内部世界子存在事实提交状态_条件不足;
        if (允许提交) {
            子存在 = 取或创建内部世界子存在(上下文, now);
            if (auto* 子节点 = reinterpret_cast<基础信息节点类*>(子存在)) {
                (void)写入基础节点I64(子节点, 特征_内部世界子存在原始事实写入状态(), 1, now);
                (void)写入基础节点I64(子节点, 特征_内部世界子存在与内部世界关联状态(), 1, now);
                (void)写入基础节点I64(子节点, 特征_内部世界子候选验证状态(), 内部世界子候选验证状态_已验证, now);
                原始事实写入状态 = 1;
                内部世界关联状态 = 1;
                提交状态 = 内部世界子存在事实提交状态_已提交;
            }
        }

        const bool 成功 = 提交状态 == 内部世界子存在事实提交状态_已提交;
        const auto* 失败原因 = 成功 ? nullptr : 值_执行缺口_当前方法条件不满足();
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            reinterpret_cast<基础信息节点类*>(上下文.内部世界 ? 上下文.内部世界 : 实际输出场景),
            nullptr,
            特征_内部世界子存在事实提交状态(),
            提交状态,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        I64 二次特征写入数量 = 0;
        if (成功 && 子存在) {
            if (写入观察存在提交关系二次特征(
                    实际输出场景,
                    子存在,
                    reinterpret_cast<基础信息节点类*>(上下文.内部世界),
                    动态,
                    "内部世界子存在属于父存在内部世界",
                    1,
                    false)) {
                ++二次特征写入数量;
            }
            if (写入观察存在提交关系二次特征(
                    实际输出场景,
                    子存在,
                    reinterpret_cast<基础信息节点类*>(上下文.局部切片),
                    动态,
                    "内部世界子存在来源于局部观察切片",
                    1,
                    false)) {
                ++二次特征写入数量;
            }
            if (写入观察存在提交关系二次特征(
                    实际输出场景,
                    子存在,
                    reinterpret_cast<基础信息节点类*>(上下文.子候选),
                    动态,
                    "内部世界子存在来源于内部子候选",
                    1,
                    false)) {
                ++二次特征写入数量;
            }
        }

        auto 写提交摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入方法执行情况(宿主, 成功 ? 值_执行成功() : 值_执行失败(), now);
            if (上下文.目标存在) {
                (void)写入基础节点指针(宿主, 特征_目标存在(), 上下文.目标存在, now);
            }
            if (上下文.内部世界) {
                (void)写入基础节点指针(宿主, 特征_内部世界(), 上下文.内部世界, now);
            }
            if (子存在) {
                (void)写入基础节点指针(宿主, 特征_成员存在集合(), 子存在, now);
            }
            (void)写入基础节点I64(宿主, 特征_内部世界子存在事实提交状态(), 提交状态, now);
            (void)写入基础节点I64(宿主, 特征_内部世界子存在原始事实写入状态(), 原始事实写入状态, now);
            (void)写入基础节点I64(宿主, 特征_内部世界子存在与内部世界关联状态(), 内部世界关联状态, now);
            (void)写入基础节点I64(宿主, 特征_成员存在集合(), 成功 ? 成员存在集合状态_已更新 : 0, now);
            (void)写入基础节点I64(宿主, 特征_内部世界子存在候选确认可提交数量(), 可提交数量, now);
            if (动态) {
                (void)写入基础节点指针(宿主, 特征_动作动态(), 动态, now);
            }
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        };
        写提交摘要(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写提交摘要(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写提交摘要(reinterpret_cast<基础信息节点类*>(上下文.目标存在));
        写提交摘要(reinterpret_cast<基础信息节点类*>(上下文.内部世界));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        {
            std::ostringstream 输出;
            输出 << "提交_确认内部世界子存在事实"
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 目标存在=" << (上下文.目标存在 ? 上下文.目标存在->获取主键() : std::string{})
                << " | 内部世界=" << (上下文.内部世界 ? 上下文.内部世界->获取主键() : std::string{})
                << " | 子候选=" << (上下文.子候选 ? 上下文.子候选->获取主键() : std::string{})
                << " | 子存在=" << (子存在 ? 子存在->获取主键() : std::string{})
                << " | 方案取得状态=" << 方案取得状态
                << " | 可提交数量=" << 可提交数量
                << " | 提交状态=" << 提交状态
                << " | 写入子存在原始事实状态=" << 原始事实写入状态
                << " | 内部世界关联状态=" << 内部世界关联状态
                << " | 写入二次特征数量=" << 二次特征写入数量
                << " | 不写基础风险明确=1"
                << " | 不结算安全值=1"
                << " | 动作动态=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(动态))
                << " | 失败原因=" << (失败原因 ? 失败原因->获取主键() : std::string{});
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我递归细分存在内部世界(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我内部世界动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_递归细分存在内部世界,
            "自我_递归细分存在内部世界",
            确认自我递归细分存在内部世界规格,
            枚举_内部世界动作阶段::递归细分,
            特征_内部世界细分状态(),
            内部世界细分状态_部分完成);
    }

    enum class 枚举_变化发现动作阶段 : std::uint8_t {
        匹配关系,
        特征变化,
        变化事件
    };

    inline 存在节点类* 执行自我变化发现动作(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        枚举_本能方法ID 本能ID,
        const char* 方法名,
        bool (*确认规格)(方法类::节点类*, 存在节点类*, 时间戳) noexcept,
        枚举_变化发现动作阶段 阶段,
        const 语素入口节点类* 主结果特征,
        I64 主结果值) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            本能ID,
            输出结果场景,
            now,
            方法名,
            确认规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));

        结构_观察变化上下文 上下文{};
        上下文.宿主 = 目标宿主 ? 目标宿主 : reinterpret_cast<基础信息节点类*>(实际输出场景);
        上下文.当前存在 = 解析当前观察存在(输入参数场景, 上下文.宿主, 实际输出场景);
        上下文.历史存在 = 解析历史观察存在(
            输入参数场景,
            上下文.宿主,
            实际输出场景,
            上下文.当前存在);

        bool 成功 = 上下文.当前存在 != nullptr || 上下文.历史存在 != nullptr;
        if (成功) {
            计算观察存在匹配关系(上下文, now);
        }
        if (成功 && 阶段 >= 枚举_变化发现动作阶段::特征变化) {
            计算观察存在变化结果(上下文, now);
        }
        if (成功 && 阶段 >= 枚举_变化发现动作阶段::变化事件) {
            计算观察变化事件(上下文, now);
        }

        const auto* 失败原因 = 成功 ? nullptr : 值_执行缺口_当前方法条件不满足();
        const I64 实际主结果值 = (主结果特征 == 特征_历史对应状态())
            ? 上下文.历史对应状态
            : 主结果值;
        auto* 动态宿主 = reinterpret_cast<基础信息节点类*>(
            上下文.变化事件
                ? 上下文.变化事件
                : (上下文.变化结果
                    ? 上下文.变化结果
                    : (上下文.匹配关系
                        ? 上下文.匹配关系
                        : (上下文.当前存在 ? 上下文.当前存在 : 上下文.历史存在))));
        if (!动态宿主) {
            动态宿主 = 上下文.宿主;
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            动态宿主,
            nullptr,
            主结果特征,
            成功 ? 实际主结果值 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入观察变化方法结果(宿主, 成功, 上下文, 动态, 失败原因, now);
            if (!宿主 || !成功) return;
            (void)写入基础节点I64(宿主, 主结果特征, 实际主结果值, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(上下文.宿主);
        写结果(reinterpret_cast<基础信息节点类*>(上下文.当前存在));
        写结果(reinterpret_cast<基础信息节点类*>(上下文.历史存在));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        {
            std::ostringstream 输出;
            输出 << 方法名
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 当前存在=" << (上下文.当前存在 ? 上下文.当前存在->获取主键() : std::string{})
                << " | 历史存在=" << (上下文.历史存在 ? 上下文.历史存在->获取主键() : std::string{})
                << " | 历史对应状态=" << 上下文.历史对应状态
                << " | 匹配置信度=" << 上下文.匹配置信度
                << " | 变化状态=" << 上下文.变化状态
                << " | 事件类型=" << 上下文.事件类型
                << " | 说明=未命中不等于消失";
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我匹配当前观察存在与历史观察存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我变化发现动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_匹配当前观察存在与历史观察存在,
            "自我_匹配当前观察存在与历史观察存在",
            确认自我匹配当前观察存在与历史观察存在规格,
            枚举_变化发现动作阶段::匹配关系,
            特征_历史对应状态(),
            历史对应状态_已匹配);
    }

    inline 存在节点类* 自我比较存在特征变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我变化发现动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_比较存在特征变化,
            "自我_比较存在特征变化",
            确认自我比较存在特征变化规格,
            枚举_变化发现动作阶段::特征变化,
            特征_观察存在变化结果(),
            变化结果状态_已生成);
    }

    inline 存在节点类* 自我生成观察变化事件(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我变化发现动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_生成观察变化事件,
            "自我_生成观察变化事件",
            确认自我生成观察变化事件规格,
            枚举_变化发现动作阶段::变化事件,
            特征_观察变化事件集合(),
            观察变化事件集合状态_已生成);
    }

    enum class 枚举_场景复现导出阶段 : std::uint8_t {
        识别快照,
        OpenGL复现数据
    };

    inline 存在节点类* 执行自我场景复现导出动作(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        枚举_本能方法ID 本能ID,
        const char* 方法名,
        bool (*确认规格)(方法类::节点类*, 存在节点类*, 时间戳) noexcept,
        枚举_场景复现导出阶段 阶段,
        const 语素入口节点类* 主结果特征,
        I64 主结果值) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            本能ID,
            输出结果场景,
            now,
            方法名,
            确认规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 默认宿主 = reinterpret_cast<基础信息节点类*>(
            世界树.自我所在场景 ? 世界树.自我所在场景 : 世界树.取或创建自我现实场景());
        auto* 目标宿主 = 解析双目相机目标宿主(输入参数场景, 默认宿主);
        结构_场景复现上下文 上下文 = 读取场景复现上下文(目标宿主);

        bool 成功 = 上下文.宿主 && 上下文.有识别材料;
        if (成功) {
            上下文.场景识别快照 = 取或创建场景识别快照(上下文.宿主);
            写入场景识别快照摘要(
                reinterpret_cast<基础信息节点类*>(上下文.场景识别快照),
                上下文,
                now);
            (void)写入基础节点指针(上下文.宿主, 特征_场景识别快照(), 上下文.场景识别快照, now);
            (void)写入基础节点I64(上下文.宿主, 特征_快照状态(), 场景识别快照状态_已生成, now);
        }
        if (成功 && 阶段 >= 枚举_场景复现导出阶段::OpenGL复现数据) {
            上下文.OpenGL复现数据 = 取或创建OpenGL复现数据(上下文.宿主);
            写入OpenGL复现数据摘要(
                reinterpret_cast<基础信息节点类*>(上下文.OpenGL复现数据),
                上下文,
                now);
            (void)写入基础节点指针(上下文.宿主, 特征_OpenGL复现数据(), 上下文.OpenGL复现数据, now);
            (void)写入基础节点I64(上下文.宿主, 特征_复现数据状态(), OpenGL复现数据状态_已生成, now);
        }

        const auto* 失败原因 = 成功 ? nullptr : 值_执行缺口_当前方法条件不满足();
        auto* 动态宿主 = reinterpret_cast<基础信息节点类*>(
            上下文.OpenGL复现数据
                ? 上下文.OpenGL复现数据
                : (上下文.场景识别快照 ? 上下文.场景识别快照 : nullptr));
        if (!动态宿主) {
            动态宿主 = 上下文.宿主;
        }

        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            动态宿主,
            nullptr,
            主结果特征,
            成功 ? 主结果值 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入场景复现方法结果(宿主, 成功, 上下文, 动态, 失败原因, now);
            if (!宿主 || !成功) return;
            (void)写入基础节点I64(宿主, 主结果特征, 主结果值, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(上下文.宿主);
        写结果(reinterpret_cast<基础信息节点类*>(上下文.场景识别快照));
        写结果(reinterpret_cast<基础信息节点类*>(上下文.OpenGL复现数据));

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        {
            std::ostringstream 输出;
            输出 << 方法名
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 帧=" << 上下文.帧宽度 << 'x' << 上下文.帧高度
                << " | 像素特征=" << 上下文.像素特征数量
                << " | 空间候选=" << 上下文.空间候选数量
                << " | 已确认观察存在=" << 上下文.已验证观察存在数量
                << " | 内部世界状态=" << 上下文.内部世界状态
                << " | 说明=仅导出自我所在场景识别结果,不读取相机预览,不回写控制面板";
            项目运行日志(输出.str());
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我导出当前场景识别快照(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我场景复现导出动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_导出当前场景识别快照,
            "自我_导出当前场景识别快照",
            确认自我导出当前场景识别快照规格,
            枚举_场景复现导出阶段::识别快照,
            特征_场景识别快照(),
            场景识别快照状态_已生成);
    }

    inline 存在节点类* 自我导出OpenGL复现数据(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 执行自我场景复现导出动作(
            方法首节点,
            输入参数场景,
            输出结果场景,
            枚举_本能方法ID::自我_导出OpenGL复现数据,
            "自我_导出OpenGL复现数据",
            确认自我导出OpenGL复现数据规格,
            枚举_场景复现导出阶段::OpenGL复现数据,
            特征_OpenGL复现数据(),
            OpenGL复现数据状态_已生成);
    }

    inline 存在节点类* 自我提取空间候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_提取空间候选,
            输出结果场景,
            now,
            "自我_提取空间候选",
            确认自我提取空间候选规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账空间候选集合(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const bool 成功 = 结果.成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_空间候选集合(),
            成功 ? 1 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入双目相机空间候选摘要(宿主, 结果, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_提取空间候选",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我判断空间候选相对独立性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_判断空间候选相对独立性,
            输出结果场景,
            now,
            "自我_判断空间候选相对独立性",
            确认自我判断空间候选相对独立性规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账空间候选集合(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const auto 判断 = 计算空间候选独立性判断(结果);
        const bool 成功 = 结果.成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_空间候选相对独立性判断(),
            成功 ? 1 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入双目相机空间候选独立性摘要(宿主, 结果, 判断, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_判断空间候选相对独立性",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        if (成功) {
            项目运行日志(
                "空间候选相对独立性判断"
                " | 相对独立=" + std::to_string(判断.相对独立数量)
                + " | 不独立=" + std::to_string(判断.不独立数量)
                + " | 待拆分=" + std::to_string(判断.待拆分数量)
                + " | 待合并=" + std::to_string(判断.待合并数量)
                + " | 不确定=" + std::to_string(判断.不确定数量)
                + " | 主候选状态=" + std::to_string(判断.主候选状态)
                + " | 主候选分离度评分=" + std::to_string(判断.主候选分离度评分));
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我生成空间候选观察队列(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_生成空间候选观察队列,
            输出结果场景,
            now,
            "自我_生成空间候选观察队列",
            确认自我生成空间候选观察队列规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账空间候选集合(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const auto 判断 = 计算空间候选独立性判断(结果);
        const auto 队列 = 生成空间候选观察队列(结果);
        const bool 成功 = 结果.成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_空间候选观察队列(),
            成功 ? 1 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入双目相机空间候选观察队列摘要(宿主, 结果, 判断, 队列, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_生成空间候选观察队列",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        if (成功) {
            const auto* 首选 = 队列.队列.empty() ? nullptr : &队列.队列.front();
            项目运行日志(
                "空间候选观察队列"
                " | 候选数量=" + std::to_string(队列.候选数量)
                + " | 首选编号=" + std::to_string(首选 ? 首选->候选编号 : -1)
                + " | 首选优先级评分=" + std::to_string(首选 ? 首选->优先级评分 : 0)
                + " | 首选相对独立状态=" + std::to_string(首选 ? 首选->相对独立状态 : 空间候选独立状态_不确定)
                + " | 首选中心接近评分=" + std::to_string(首选 ? 首选->中心接近评分 : 0)
                + " | 首选范围大小评分=" + std::to_string(首选 ? 首选->范围大小评分 : 0));
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我观察单个空间候选并组合存在假设(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_观察单个空间候选并组合存在假设,
            输出结果场景,
            now,
            "自我_观察单个空间候选并组合存在假设",
            确认自我观察单个空间候选并组合存在假设规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账空间候选集合(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const auto 判断 = 计算空间候选独立性判断(结果);
        const auto 队列 = 生成空间候选观察队列(结果);
        const auto 假设 = 组合观察存在假设(结果, 队列);
        const bool 成功 = 结果.成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_观察存在假设(),
            成功 ? 假设.假设状态 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入双目相机观察存在假设摘要(宿主, 结果, 判断, 队列, 假设, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_观察单个空间候选并组合存在假设",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        if (成功) {
            项目运行日志(
                "观察存在假设"
                " | 假设状态=" + std::to_string(假设.假设状态)
                + " | 来源候选编号=" + std::to_string(假设.来源候选编号)
                + " | 验证状态=" + std::to_string(假设.验证状态)
                + " | 距离=" + std::to_string(假设.距离)
                + " | 尺寸X=" + std::to_string(假设.尺寸X)
                + " | 尺寸Y=" + std::to_string(假设.尺寸Y)
                + " | 尺寸Z=" + std::to_string(假设.尺寸Z)
                + " | 方位X=" + std::to_string(假设.方位X)
                + " | 方位Y=" + std::to_string(假设.方位Y)
                + " | 轮廓支持评分=" + std::to_string(假设.轮廓支持评分));
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我用空间范围投影覆盖原帧验证像素归属(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_用空间范围投影覆盖原帧验证像素归属,
            输出结果场景,
            now,
            "自我_用空间范围投影覆盖原帧验证像素归属",
            确认自我用空间范围投影覆盖原帧验证像素归属规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账空间候选集合(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const auto 判断 = 计算空间候选独立性判断(结果);
        const auto 队列 = 生成空间候选观察队列(结果);
        const auto 假设 = 组合观察存在假设(结果, 队列);
        const auto 验证 = 组合像素归属验证结果(结果, 假设);
        const bool 成功 = 结果.成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_像素归属验证结果(),
            成功 ? 验证.结果状态 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入双目相机像素归属验证摘要(宿主, 结果, 判断, 队列, 假设, 验证, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_用空间范围投影覆盖原帧验证像素归属",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        if (成功) {
            项目运行日志(
                "像素归属验证结果"
                " | 结果状态=" + std::to_string(验证.结果状态)
                + " | 来源候选编号=" + std::to_string(验证.来源候选编号)
                + " | 空间覆盖像素数=" + std::to_string(验证.空间覆盖像素数)
                + " | 空间匹配像素数=" + std::to_string(验证.空间匹配像素数)
                + " | 空间冲突像素数=" + std::to_string(验证.空间冲突像素数)
                + " | 深度一致率=" + std::to_string(验证.深度一致率)
                + " | 范围内点比例=" + std::to_string(验证.范围内点比例)
                + " | 投影覆盖率=" + std::to_string(验证.投影覆盖率)
                + " | 轮廓吻合率=" + std::to_string(验证.轮廓吻合率)
                + " | 未解释像素数=" + std::to_string(验证.未解释像素数)
                + " | 验证状态=" + std::to_string(验证.验证状态));
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 自我确认观察存在并更新帧解释状态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_确认观察存在并更新帧解释状态,
            输出结果场景,
            now,
            "自我_确认观察存在并更新帧解释状态",
            确认自我确认观察存在并更新帧解释状态规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        结构_双目相机调用结果 结果{};
        (void)读取已落账空间候选集合(目标宿主, 输入参数场景, 实际输出场景, 结果);
        const auto 判断 = 计算空间候选独立性判断(结果);
        const auto 队列 = 生成空间候选观察队列(结果);
        const auto 假设 = 组合观察存在假设(结果, 队列);
        const auto 验证 = 组合像素归属验证结果(结果, 假设);
        const auto 确认 = 确认观察存在结果(结果, 假设, 验证, 队列);
        const auto 归属账本 = 计算全帧像素归属账本(结果, 队列, 假设, 验证);
        const bool 成功 = 结果.成功;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_帧解释状态(),
            成功 ? 确认.帧解释状态 : 帧解释状态_未生成,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            写入双目相机观察存在确认摘要(宿主, 结果, 判断, 队列, 假设, 验证, 确认, 归属账本, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "自我_确认观察存在并更新帧解释状态",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        if (成功) {
            项目运行日志(
                "观察存在确认结果"
                " | 确认状态=" + std::to_string(确认.确认状态)
                + " | 来源候选编号=" + std::to_string(确认.来源候选编号)
                + " | 来源队列位次=" + std::to_string(假设.来源队列位次)
                + " | 相对独立状态=" + std::to_string(假设.相对独立状态)
                + " | 相对独立候选数=" + std::to_string(判断.相对独立数量)
                + " | 待拆分候选数=" + std::to_string(判断.待拆分数量)
                + " | 待合并候选数=" + std::to_string(判断.待合并数量)
                + " | 已验证观察存在数量=" + std::to_string(确认.已验证观察存在数量)
                + " | 帧解释状态=" + std::to_string(确认.帧解释状态)
                + " | 像素归属更新=" + std::to_string(确认.像素归属更新)
                + " | 未解释区域更新=" + std::to_string(确认.未解释区域更新)
                + " | 全帧像素归属账状态=" + std::to_string(归属账本.账本状态)
                + " | 已归属像素数=" + std::to_string(归属账本.已归属像素数)
                + " | 未解释像素数=" + std::to_string(归属账本.未解释像素数)
                + " | 归属冲突像素数=" + std::to_string(归属账本.归属冲突像素数)
                + " | 像素归属率=" + std::to_string(归属账本.像素归属率)
                + " | 空间连续性评分=" + std::to_string(确认.空间连续性评分)
                + " | 范围稳定性评分=" + std::to_string(确认.范围稳定性评分)
                + " | 范围内点比例=" + std::to_string(确认.范围内点比例)
                + " | 深度一致率=" + std::to_string(确认.深度一致率)
                + " | 投影覆盖率=" + std::to_string(确认.投影覆盖率)
                + " | 轮廓吻合率=" + std::to_string(确认.轮廓吻合率));
        }
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 用双目相机发现候选存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::双目相机_发现候选存在,
            输出结果场景,
            now,
            "双目相机_发现候选存在",
            确认双目相机发现候选存在规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 实际输出场景 = 双目相机实际输出场景(环境.方法首节点, 输出结果场景, now);
        auto* 相机存在 = 解析双目相机目标外设(输入参数场景, 实际输出场景);
        auto* 目标宿主 = 解析双目相机目标宿主(
            输入参数场景,
            reinterpret_cast<基础信息节点类*>(实际输出场景));
        const auto 结果 = 采集双目相机帧();
        const bool 成功 = 结果.成功;
        const I64 轮廓数量 = 成功
            ? static_cast<I64>(结果.轮廓数量)
            : 0;
        auto* 动态 = 写入双目相机I64事实与动态(
            环境.方法首节点,
            实际输出场景,
            目标宿主,
            相机存在,
            特征_候选存在集合(),
            成功 ? 1 : 0,
            成功,
            输入参数场景,
            实际输出场景,
            环境.本轮运行虚拟存在,
            now);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            写入双目相机通用结果(宿主, 成功, 相机存在, 目标宿主, 动态, 结果.失败原因, now);
            if (!宿主) return;
            (void)写入基础节点I64(宿主, 特征_候选存在集合(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_候选存在数量(), 轮廓数量, now);
            (void)写入基础节点I64(宿主, 特征_轮廓数量(), 轮廓数量, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(实际输出场景));
        写结果(目标宿主);

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            实际输出场景,
            now);
        记录双目相机本能动作结果(
            "双目相机_发现候选存在",
            结果,
            输入参数场景,
            实际输出场景,
            相机存在,
            目标宿主,
            动态,
            输出运行);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

}
