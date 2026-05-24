module;

#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
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
import 自我类.特征定义;

namespace {
    inline constexpr I64 历史本能方法ID_增加可用方法数量 = 6049;
    inline constexpr I64 历史本能方法ID_改变特征值 = 6058;
    inline constexpr I64 历史本能方法ID_任务筹办 = 6062;
    inline constexpr I64 历史本能方法ID_任务执行 = 6063;

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
        std::size_t 轮廓数量 = 0;
    };

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
        结果.深度帧号 = 来源.深度帧号;
        结果.彩色帧号 = 来源.彩色帧号;
        结果.轮廓数量 = 来源.轮廓数量;
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
        存在节点类* 输出运行) noexcept
    {
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
            << " | 输出运行=" << 基础节点日志文本(reinterpret_cast<基础信息节点类*>(输出运行))
            << " | 宽度=" << 结果.宽度
            << " | 高度=" << 结果.高度
            << " | 深度帧号=" << 结果.深度帧号
            << " | 彩色帧号=" << 结果.彩色帧号
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
