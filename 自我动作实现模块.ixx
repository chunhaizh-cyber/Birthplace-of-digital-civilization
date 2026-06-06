module;

#include <cstdint>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <initializer_list>
#include <limits>
#include <optional>
#include <sstream>
#include <variant>
#include <string>
#include <vector>

#include "本能方法类.h"
#include "世界树类.h"
#include "方法类.h"
#include "方法虚拟存在服务类.h"
#include "任务类.h"
#include "需求类.h"
#include "语素类.h"

export module 自我动作实现模块;

import 自我类;
import 本能动作管理模块;
import 任务模块.筹办;
import 日志模块;
import 自我动作实现.内部模块;
import 自我动作实现.外部模块;

namespace {
    inline constexpr I64 历史本能方法ID_增加可用方法数量 = 6049;
    inline constexpr I64 历史本能方法ID_改变特征值 = 6058;
    inline constexpr I64 历史本能方法ID_任务筹办 = 6062;
    inline constexpr I64 历史本能方法ID_任务执行 = 6063;
    inline constexpr I64 OR组结算令牌状态_未领取 = 0;
    inline constexpr I64 OR组结算令牌状态_已领取 = 1;
    inline constexpr I64 OR组令牌提交状态_拒绝 = -1;
    inline constexpr I64 OR组令牌提交状态_首次领取 = 1;
    inline constexpr I64 OR组令牌提交状态_重复领取 = 2;

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

    inline const 语素入口节点类* 特征_OR组节点() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组节点", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OR组领取路径() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组领取路径", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OR组结算令牌记录() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组结算令牌记录", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OR组令牌记录键() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组令牌记录键", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OR组结算令牌状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组结算令牌状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OR组令牌领取任务() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组令牌领取任务", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OR组令牌领取动作动态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组令牌领取动作动态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OR组令牌领取时间() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组令牌领取时间", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OR组令牌重复领取次数() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组令牌重复领取次数", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OR组令牌重复领取状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组令牌重复领取状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_OR组令牌提交状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("OR组令牌提交状态", 枚举_信息入口类型::特征模板入口);
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

    inline const 语素入口节点类* 特征_来源动作动态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("来源动作动态", 枚举_信息入口类型::特征模板入口);
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

    inline const 语素入口节点类* 特征_新结果分支数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("新结果分支数量", 枚举_信息入口类型::特征模板入口);
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

    inline const 语素入口节点类* 特征_方法动作数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法动作数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法可执行入口数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法可执行入口数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法条件节点数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法条件节点数量", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法条件结果配对状态() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法条件结果配对状态", 枚举_信息入口类型::特征模板入口);
        return s_词;
    }

    inline const 语素入口节点类* 特征_方法条件结果对数量() noexcept
    {
        static const 语素入口节点类* s_词 = 语素集.添加信息入口词("方法条件结果对数量", 枚举_信息入口类型::特征模板入口);
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

    struct 结构_方法条件结果节点 {
        方法类::节点类* 条件节点 = nullptr;
        方法类::节点类* 结果节点 = nullptr;
    };

    struct 结构_方法条件结果对样本 {
        方法类::节点类* 来源方法 = nullptr;
        任务节点类* 来源任务 = nullptr;
        场景节点类* 执行前条件场景 = nullptr;
        场景节点类* 执行后实际结果场景 = nullptr;
        时间戳 执行结束时间 = 0;
    };

    inline 抽象特征节点类* 取或创建根下抽象特征(
        const 语素入口节点类* 特征类型) noexcept
    {
        auto* 抽象根 = 世界树.取或创建抽象特征根();
        return 抽象根 && 特征类型
            ? 世界树.取或创建抽象特征_按类型(抽象根, 特征类型)
            : nullptr;
    }

    inline 特征节点类* 写入方法虚拟存在节点实例特征(
        方法类::节点类* 方法首节点,
        const 语素入口节点类* 实例抽象类型,
        方法类::节点类* 节点,
        时间戳 now) noexcept
    {
        auto* 虚拟存在 = 方法虚拟存在(方法首节点, now);
        auto* 抽象特征 = 取或创建根下抽象特征(实例抽象类型);
        if (!虚拟存在 || !抽象特征 || !节点) return nullptr;

        auto* 实例特征 = 世界树.特征().取或创建实例特征_按抽象特征(
            reinterpret_cast<基础信息节点类*>(虚拟存在),
            抽象特征);
        if (!实例特征) return nullptr;

        (void)世界树.写入特征_指针(实例特征, 节点, now);
        (void)写入基础节点指针(
            reinterpret_cast<基础信息节点类*>(实例特征),
            特征_抽象特征引用(),
            抽象特征,
            now);
        return 实例特征;
    }

    inline void 增加基础节点I64(
        基础信息节点类* 宿主,
        const 语素入口节点类* 特征类型,
        I64 增量,
        时间戳 now) noexcept
    {
        if (!宿主 || !特征类型 || 增量 == 0) return;
        I64 当前 = 0;
        (void)读取基础节点I64(宿主, 特征类型, 当前);
        I64 新值 = 当前;
        if (增量 > 0 && 当前 > (std::numeric_limits<I64>::max)() - 增量) {
            新值 = (std::numeric_limits<I64>::max)();
        } else if (增量 < 0 && 当前 < (std::numeric_limits<I64>::min)() - 增量) {
            新值 = (std::numeric_limits<I64>::min)();
        } else {
            新值 = 当前 + 增量;
        }
        (void)写入基础节点I64(宿主, 特征类型, 新值, now);
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

    inline 基础信息节点类* 条件统计宿主_条件结果对归并(方法类::节点类* 条件节点) noexcept
    {
        const auto* 条件信息 = 条件节点 ? 条件节点->主信息.取条件节点信息() : nullptr;
        auto* 条件场景 = 条件信息 ? 条件信息->条件场景.指针 : nullptr;
        return 条件场景 ? reinterpret_cast<基础信息节点类*>(条件场景) : nullptr;
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

    inline I64 方法动作数量_动作模块(方法类::节点类* 方法首节点) noexcept
    {
        return 方法首节点 && 方法首节点->主信息.有方法动作() ? 1 : 0;
    }

    inline I64 方法可执行入口数量_动作模块(方法类::节点类* 方法首节点) noexcept
    {
        return 方法具有已注册本能执行入口(方法首节点) ? 1 : 0;
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
        case 枚举_本能方法ID::自我_结算叶子任务价值:
        case 枚举_本能方法ID::自我_提交方法可执行入口:
        case 枚举_本能方法ID::自我_建立条件结果对:
        case 枚举_本能方法ID::自我_提交方法状态变化:
        case 枚举_本能方法ID::提交_确认观察存在事实:
        case 枚举_本能方法ID::提交_当前观察范围可观测单位存在对应事实:
        case 枚举_本能方法ID::提交_观察存在发现事实:
        case 枚举_本能方法ID::提交_观察存在特征值变化事实:
        case 枚举_本能方法ID::提交_指定存在跟踪事实:
        case 枚举_本能方法ID::提交_确认内部世界子存在事实:
        case 枚举_本能方法ID::提交_风险安全场景影响部分状态变化:
        case 枚举_本能方法ID::提交_安全因果因素无负证据默认满足:
        case 枚举_本能方法ID::提交_安全因果因素证据评估值:
        case 枚举_本能方法ID::提交_领取OR组结算令牌:
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

    inline 方法类::节点类* 查找等价结果节点_条件结果对归并(
        方法类::节点类* 条件节点,
        场景节点类* 结果场景) noexcept
    {
        if (!条件节点 || !条件节点->子 || !结果场景) return nullptr;
        auto* 首子 = static_cast<方法类::节点类*>(条件节点->子);
        auto* 当前 = 首子;
        do {
            if (当前
                && 当前->主信息.节点种类() == 枚举_方法节点种类::方法结果节点) {
                if (auto* 结果信息 = 当前->主信息.取结果节点信息();
                    结果信息 && 场景等价_条件结果对归并(结果信息->结果场景.指针, 结果场景)) {
                    return 当前;
                }
            }
            当前 = 当前 ? static_cast<方法类::节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首子);
        return nullptr;
    }

    inline 方法类::节点类* 解析目标方法指针_动作模块(
        void* 指针,
        时间戳 now) noexcept;

    inline 结构_方法条件结果节点 归并方法条件结果对(
        方法类::节点类* 方法首节点,
        const 结构_方法条件结果对样本& 样本,
        时间戳 now) noexcept
    {
        结构_方法条件结果节点 节点{};
        if (!方法首节点) return 节点;
        if (auto* 已解析方法 = 解析目标方法指针_动作模块(方法首节点, now)) {
            方法首节点 = 已解析方法;
        } else {
            std::ostringstream 输出;
            输出 << "条件结果对归并拒绝非方法树目标"
                << " | 输入指针=" << reinterpret_cast<std::uintptr_t>(方法首节点);
            项目运行日志(输出.str());
            return 节点;
        }

        auto* 条件场景 = 样本.执行前条件场景
            ? 样本.执行前条件场景
            : 抽象条件场景(方法首节点, now);
        auto* 实际结果场景 = 样本.执行后实际结果场景
            ? 样本.执行后实际结果场景
            : 抽象结果场景(方法首节点, now);

        const bool 条件原已存在 = 条件场景
            && 查找等价条件节点_条件结果对归并(方法首节点, 条件场景);

        if (条件场景) {
            节点.条件节点 = 方法类::创建条件节点(
                方法首节点,
                条件场景,
                {},
                {},
                now,
                now);
            if (节点.条件节点
                && 节点.条件节点->主信息.节点种类() != 枚举_方法节点种类::方法条件节点) {
                std::ostringstream 输出;
                输出 << "条件结果对归并拒绝异常条件节点"
                    << " | 方法=" << 方法首节点->获取主键()
                    << " | 条件节点=" << 节点.条件节点->获取主键()
                    << " | 条件节点种类=" << static_cast<int>(节点.条件节点->主信息.节点种类());
                项目运行日志(输出.str());
                节点.条件节点 = nullptr;
                return 节点;
            }
            if (节点.条件节点) {
                (void)写入方法虚拟存在节点实例特征(
                    方法首节点,
                    特征_方法条件节点(),
                    节点.条件节点,
                    now);
            }
        }

        auto* 结果父节点 = 节点.条件节点 ? 节点.条件节点 : 方法首节点;
        const bool 条件下原有结果 = 节点.条件节点
            && 条件节点已有结果子节点_条件结果对归并(节点.条件节点);
        const bool 结果原已存在 = 节点.条件节点
            && 实际结果场景
            && 查找等价结果节点_条件结果对归并(节点.条件节点, 实际结果场景);
        if (实际结果场景) {
            节点.结果节点 = 方法类::创建结果节点(
                结果父节点,
                实际结果场景,
                {});
            if (节点.结果节点) {
                auto* 方法事实宿主 = static_cast<基础信息节点类*>(nullptr);
                if (auto* 虚拟存在 = 方法虚拟存在(方法首节点, now)) {
                    方法事实宿主 = reinterpret_cast<基础信息节点类*>(虚拟存在);
                }
                auto* 条件事实宿主 = 条件统计宿主_条件结果对归并(节点.条件节点);
                if (条件原已存在 && 结果原已存在) {
                    增加基础节点I64(条件事实宿主, 特征_结果符合次数(), 1, now);
                } else if (条件原已存在 && 条件下原有结果) {
                    增加基础节点I64(条件事实宿主, 特征_结果不符合次数(), 1, now);
                    增加基础节点I64(条件事实宿主, 特征_新结果分支数量(), 1, now);
                }
                if (条件事实宿主) {
                    增加基础节点I64(条件事实宿主, 特征_样本次数(), 1, now);
                    (void)写入基础节点I64(条件事实宿主, 特征_最近样本时间(), static_cast<I64>(now), now);
                }
                if (方法事实宿主) {
                    增加基础节点I64(方法事实宿主, 特征_样本次数(), 1, now);
                    (void)写入基础节点I64(方法事实宿主, 特征_最近样本时间(), static_cast<I64>(now), now);
                }
                (void)写入方法虚拟存在节点实例特征(
                    方法首节点,
                    特征_方法结果节点(),
                    节点.结果节点,
                    now);
            } else {
                std::ostringstream 输出;
                输出 << "条件结果对归并创建结果节点失败"
                    << " | 方法=" << (方法首节点 ? 方法首节点->获取主键() : std::string("空"))
                    << " | 条件节点=" << (节点.条件节点 ? 节点.条件节点->获取主键() : std::string("空"))
                    << " | 结果父节点=" << (结果父节点 ? 结果父节点->获取主键() : std::string("空"))
                    << " | 结果父种类=" << (结果父节点 ? static_cast<int>(结果父节点->主信息.节点种类()) : -1)
                    << " | 结果场景=" << 实际结果场景->获取主键()
                    << " | 父指针存在=" << (结果父节点 && 结果父节点->父 ? 1 : 0);
                项目运行日志(输出.str());
            }
        }

        if (auto* 虚拟存在 = 方法虚拟存在(方法首节点, now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(虚拟存在);
            if (节点.条件节点) {
                (void)写入基础节点指针(宿主, 特征_方法条件节点(), 节点.条件节点, now);
            }
            if (节点.结果节点) {
                (void)写入基础节点指针(宿主, 特征_方法结果节点(), 节点.结果节点, now);
            }
        }

        return 节点;
    }

    inline 结构_方法条件结果节点 构建方法条件和结果节点(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept
    {
        结构_方法条件结果对样本 样本{};
        样本.来源方法 = 方法首节点;
        样本.执行前条件场景 = 输入参数场景;
        样本.执行后实际结果场景 = 输出结果场景;
        样本.执行结束时间 = now;
        return 归并方法条件结果对(方法首节点, 样本, now);
    }

    inline bool 确保学习本能自举条件结果对(
        方法类::节点类* 方法首节点,
        时间戳 now) noexcept
    {
        if (!方法首节点) {
            return false;
        }
        if (方法真实条件结果对数量(方法首节点) > 0) {
            return true;
        }

        // 学习本能方法是让普通方法进入“可用”的核心反射。
        // 它自身不能再依赖同一个“方法状态=可用”需求来补齐，否则会自指停等。
        // 因此在确认专属规格时，为学习本能建立一组抽象条件结果对作为自举可用证据。
        auto* 条件场景 = 抽象条件场景(方法首节点, now);
        auto* 结果场景 = 抽象结果场景(方法首节点, now);
        const auto 节点 = 构建方法条件和结果节点(
            方法首节点,
            条件场景,
            结果场景,
            now);
        return 节点.条件节点 && 节点.结果节点;
    }

    inline bool 确保本能方法抽象条件结果对(
        方法类::节点类* 方法首节点,
        时间戳 now) noexcept
    {
        if (!方法首节点) {
            return false;
        }
        if (方法真实条件结果对数量(方法首节点) > 0) {
            return true;
        }

        auto* 条件场景 = 抽象条件场景(方法首节点, now);
        auto* 结果场景 = 抽象结果场景(方法首节点, now);
        const auto 节点 = 构建方法条件和结果节点(
            方法首节点,
            条件场景,
            结果场景,
            now);
        return 节点.条件节点 && 节点.结果节点;
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

    inline constexpr I64 配对状态_输入绑定失败 = 1;
    inline constexpr I64 配对状态_配对失败 = 2;
    inline constexpr I64 配对状态_配对成功 = 4;

    struct 结构_查找限制 {
        基础信息节点类* 最高允许目标特征类型 = nullptr;
        const 语素入口节点类* 最高允许目标特征类型词 = nullptr;
        基础信息节点类* 最高允许宿主类型 = nullptr;
        const 语素入口节点类* 最高允许宿主类型词 = nullptr;
        const 语素入口节点类* 允许值类型 = nullptr;
        bool 有允许执行差值区间 = true;
        I64 允许执行差值下界 = (std::numeric_limits<I64>::min)();
        I64 允许执行差值上界 = (std::numeric_limits<I64>::max)();
        I64 最大类型抽象层数 = 8;
        I64 最大值域抽象层数 = 8;
        I64 最大候选数量 = 256;
        I64 最大查找递归深度 = 64;
        bool 是否包含禁用方法 = false;
        bool 是否允许当前方法自身 = false;
    };

    struct 结构_条件项视图 {
        const 语素入口节点类* 条件特征类型 = nullptr;
        bool 有I64点值 = false;
        I64 I64点值 = 0;
        bool 有I64区间 = false;
        I64 区间下界 = 0;
        I64 区间上界 = 0;
        基础信息节点类* 抽象特征引用 = nullptr;
        基础信息节点类* 存在引用 = nullptr;
        场景节点类* 场景引用 = nullptr;
        基础信息节点类* 基础节点引用 = nullptr;
        const 语素入口节点类* 语素入口值 = nullptr;
        bool 必需 = true;
    };

    struct 结构_类型匹配结果 {
        bool 成功 = false;
        I64 抽象距离 = 0;
        const 语素入口节点类* 失败原因 = nullptr;
        std::vector<基础信息节点类*> 路径{};
    };

    inline const 语素入口节点类* 基础节点类型词(const 基础信息节点类* 节点) noexcept
    {
        return 节点 && 节点->主信息 ? 节点->主信息->类型 : nullptr;
    }

    inline 基础信息节点类* 指针若为基础信息节点(const void* 指针) noexcept
    {
        if (!指针) return nullptr;
        auto* 候选 = const_cast<基础信息节点类*>(
            reinterpret_cast<const 基础信息节点类*>(指针));
        for (auto* 节点 : 世界树.基础信息().枚举全部节点()) {
            if (节点 == 候选) return 节点;
        }
        return nullptr;
    }

    inline 抽象特征节点类* 查找抽象特征_按类型(const 语素入口节点类* 特征类型) noexcept
    {
        if (!特征类型) return nullptr;
        for (auto* 节点 : 世界树.基础信息().枚举节点_按类型<抽象特征主信息类>()) {
            const auto* 主信息 = 世界树.特征().取抽象特征主信息(
                static_cast<const 抽象特征节点类*>(节点));
            if (主信息 && 主信息->类型 == 特征类型) {
                return static_cast<抽象特征节点类*>(节点);
            }
        }
        return nullptr;
    }

    inline bool 抽象节点相同或类型相同(
        const 基础信息节点类* 节点,
        const 基础信息节点类* 目标节点,
        const 语素入口节点类* 目标类型) noexcept
    {
        if (!节点) return false;
        if (目标节点 && 节点 == 目标节点) return true;
        const auto* 主信息 = 世界树.特征().取抽象特征主信息(
            static_cast<const 抽象特征节点类*>(节点));
        return 主信息 && 目标类型 && 主信息->类型 == 目标类型;
    }

    inline const 语素入口节点类* 特征值节点对应特征类型词(
        const 基础信息节点类* 节点) noexcept
    {
        if (!节点) return nullptr;
        if (const auto* 抽象主信息 = 世界树.特征().取抽象特征主信息(
                static_cast<const 抽象特征节点类*>(节点))) {
            return 抽象主信息->类型;
        }
        if (const auto* 特征主信息 = 世界树.特征().取特征主信息(
                static_cast<const 特征节点类*>(节点))) {
            if (auto* 抽象特征 = 特征主信息->抽象特征.指针) {
                if (const auto* 抽象主信息 = 世界树.特征().取抽象特征主信息(
                        static_cast<const 抽象特征节点类*>(抽象特征))) {
                    return 抽象主信息->类型;
                }
            }
            return 特征主信息->类型;
        }
        return 基础节点类型词(节点);
    }

    inline 基础信息节点类* 查找包子节点(
        基础信息节点类* 包,
        const 语素入口节点类* 子类型) noexcept
    {
        auto* 子特征 = 查找子特征(包, 子类型);
        if (!子特征) return nullptr;

        void* 指针 = nullptr;
        if (世界树.读取特征_指针(子特征, 指针)) {
            if (auto* 节点 = 指针若为基础信息节点(指针)) {
                return 节点;
            }
        }
        return reinterpret_cast<基础信息节点类*>(子特征);
    }

    inline bool 读取包指针(
        基础信息节点类* 包,
        const 语素入口节点类* 子类型,
        void*& 输出指针) noexcept
    {
        输出指针 = nullptr;
        auto* 子特征 = 查找子特征(包, 子类型);
        return 子特征 && 世界树.读取特征_指针(子特征, 输出指针) && 输出指针;
    }

    inline std::optional<特征值> 读取特征当前值(特征节点类* 特征) noexcept
    {
        const auto* 主信息 = 世界树.特征().取特征主信息(特征);
        if (!主信息 || !主信息->有当前值()) return std::nullopt;
        return 主信息->当前值;
    }

    inline bool 语素入口可作为特征类型(const 语素入口节点类* 入口) noexcept
    {
        const auto* 主信息 = 入口
            ? dynamic_cast<const 语素入口主信息类*>(入口->主信息)
            : nullptr;
        if (!主信息) return false;
        if (主信息->对应基础信息类型 == 枚举_主信息类型::抽象特征
            || 主信息->对应基础信息类型 == 枚举_主信息类型::特征) {
            return true;
        }
        const auto 映射类型 = 语素_取信息入口主信息类型(主信息->信息入口类型);
        return 映射类型 == 枚举_主信息类型::抽象特征
            || 映射类型 == 枚举_主信息类型::特征;
    }

    inline void 吸收条件项指针值(结构_条件项视图& 项, const void* 指针) noexcept
    {
        if (!指针) return;
        if (auto* 节点 = 指针若为基础信息节点(指针)) {
            if (世界树.特征().取抽象特征主信息(static_cast<抽象特征节点类*>(节点))) {
                项.抽象特征引用 = 节点;
            }
            else if (const auto* 特征主信息 = 世界树.特征().取特征主信息(
                    static_cast<特征节点类*>(节点))) {
                项.基础节点引用 = 节点;
                if (特征主信息->抽象特征.指针) {
                    项.抽象特征引用 = reinterpret_cast<基础信息节点类*>(
                        特征主信息->抽象特征.指针);
                }
            }
            else if (世界树.基础信息().取主信息<存在节点主信息类>(节点)) {
                项.存在引用 = 节点;
            }
            else if (世界树.基础信息().取主信息<场景节点主信息类>(节点)) {
                项.场景引用 = static_cast<场景节点类*>(节点);
            }
            else {
                项.基础节点引用 = 节点;
            }
            return;
        }
        项.语素入口值 = reinterpret_cast<const 语素入口节点类*>(指针);
    }

    inline void 吸收条件项值(结构_条件项视图& 项, const 特征值& 值) noexcept
    {
        if (const auto* 标量 = std::get_if<I64>(&值)) {
            项.有I64点值 = true;
            项.I64点值 = *标量;
            return;
        }
        if (const auto* 指针 = std::get_if<指针句柄>(&值)) {
            吸收条件项指针值(项, reinterpret_cast<const void*>(指针->指针));
        }
    }

    inline const 语素入口节点类* 读取条件项特征类型(
        基础信息节点类* 条件项节点,
        const 语素入口节点类* 默认类型) noexcept
    {
        void* 指针 = nullptr;
        if (读取包指针(条件项节点, 特征_特征类型(), 指针)) {
            if (auto* 节点 = 指针若为基础信息节点(指针)) {
                return 特征值节点对应特征类型词(节点);
            }
            return reinterpret_cast<const 语素入口节点类*>(指针);
        }
        return 默认类型;
    }

    inline bool 条件项有值约束(const 结构_条件项视图& 项) noexcept
    {
        return 项.有I64点值
            || 项.有I64区间
            || 项.抽象特征引用
            || 项.存在引用
            || 项.场景引用
            || 项.基础节点引用
            || 项.语素入口值;
    }

    inline 结构_条件项视图 解析条件项视图(特征节点类* 条件项节点) noexcept
    {
        结构_条件项视图 项{};
        if (!条件项节点) return 项;

        const auto* 主信息 = 世界树.特征().取特征主信息(条件项节点);
        const auto* 默认类型 = 主信息 ? 主信息->类型 : nullptr;
        auto* 条件项宿主 = reinterpret_cast<基础信息节点类*>(条件项节点);

        项.条件特征类型 = 读取条件项特征类型(条件项宿主, 默认类型);
        I64 是否必需值 = 1;
        if (读取基础节点I64(条件项宿主, 特征_是否必需(), 是否必需值)) {
            项.必需 = 是否必需值 != 0;
        }

        if (auto 值 = 读取特征当前值(条件项节点)) {
            吸收条件项值(项, *值);
        }

        if (auto* 值特征 = 查找子特征(条件项宿主, 特征_特征值())) {
            if (auto 值 = 读取特征当前值(值特征)) {
                吸收条件项值(项, *值);
            }
        }
        if (auto* 范围特征 = 查找子特征(条件项宿主, 特征_特征值范围())) {
            if (auto 值 = 读取特征当前值(范围特征)) {
                吸收条件项值(项, *值);
            }
        }

        I64 下界 = 0;
        I64 上界 = 0;
        if (读取基础节点I64(条件项宿主, 特征_区间下界(), 下界)
            && 读取基础节点I64(条件项宿主, 特征_区间上界(), 上界)
            && 下界 <= 上界) {
            项.有I64区间 = true;
            项.区间下界 = 下界;
            项.区间上界 = 上界;
        }
        else if (主信息 && 主信息->区间.has_value() && 主信息->区间->有效()) {
            项.有I64区间 = true;
            项.区间下界 = 主信息->区间->低值;
            项.区间上界 = 主信息->区间->高值;
        }

        void* 指针 = nullptr;
        if (读取包指针(条件项宿主, 特征_抽象特征引用(), 指针)) {
            if (auto* 节点 = 指针若为基础信息节点(指针)) {
                项.抽象特征引用 = 节点;
            }
        }
        if (读取包指针(条件项宿主, 特征_存在引用(), 指针)) {
            if (auto* 节点 = 指针若为基础信息节点(指针)) {
                项.存在引用 = 节点;
            }
        }
        if (读取包指针(条件项宿主, 特征_场景引用(), 指针)) {
            if (auto* 节点 = 指针若为基础信息节点(指针)) {
                项.场景引用 = static_cast<场景节点类*>(节点);
            }
        }

        return 项;
    }

    inline std::vector<结构_条件项视图> 读取条件项列表(基础信息节点类* 条件包) noexcept
    {
        std::vector<结构_条件项视图> 列表{};
        if (!条件包) return 列表;
        for (auto* 子特征 : 世界树.特征().获取子特征(条件包)) {
            auto 项 = 解析条件项视图(子特征);
            if (项.条件特征类型 && 条件项有值约束(项)) {
                列表.push_back(项);
            }
        }
        return 列表;
    }

    inline std::vector<结构_条件项视图> 读取条件槽列表(基础信息节点类* 条件包) noexcept
    {
        std::vector<结构_条件项视图> 列表{};
        if (!条件包) return 列表;
        for (auto* 子特征 : 世界树.特征().获取子特征(条件包)) {
            auto 项 = 解析条件项视图(子特征);
            if (项.条件特征类型) {
                列表.push_back(项);
            }
        }
        return 列表;
    }

    struct 结构_方法条件节点匹配结果 {
        方法类::节点类* 条件节点 = nullptr;
        场景节点类* 条件场景 = nullptr;
        特征节点类* 条件集合 = nullptr;

        I64 条件槽数量 = 0;
        I64 必需条件数量 = 0;
        I64 已匹配必需条件数量 = 0;

        const 语素入口节点类* 失败原因 = nullptr;
        const 语素入口节点类* 失败条件特征类型 = nullptr;
        std::vector<结构_条件项视图> 条件项列表{};

        bool 条件节点已读取 = false;
        bool 条件集合已读取 = false;
        bool 成功 = false;
    };

    inline 基础信息节点类* 查找输入条件实参宿主(
        场景节点类* 输入参数场景,
        const 语素入口节点类* 条件特征类型,
        特征节点类** 输出特征 = nullptr) noexcept
    {
        if (输出特征) *输出特征 = nullptr;
        if (!输入参数场景 || !条件特征类型) return nullptr;

        auto* 输入宿主 = reinterpret_cast<基础信息节点类*>(输入参数场景);
        auto 尝试宿主 = [&](基础信息节点类* 宿主) noexcept -> 基础信息节点类* {
            if (!宿主) return nullptr;
            auto* 特征 = 查找子特征(宿主, 条件特征类型);
            if (!特征) return nullptr;
            if (输出特征) *输出特征 = 特征;
            return reinterpret_cast<基础信息节点类*>(特征);
        };

        if (auto* 命中 = 尝试宿主(输入宿主)) return 命中;

        if (语素入口主键相同(条件特征类型, 特征_输入条件包())) {
            if (auto* 包 = 输入条件包根(输入参数场景)) {
                if (输出特征) *输出特征 = 包;
                return reinterpret_cast<基础信息节点类*>(包);
            }
        }

        if (auto* 包 = 输入条件包根(输入参数场景)) {
            if (auto* 命中 = 尝试宿主(reinterpret_cast<基础信息节点类*>(包))) return 命中;
        }
        if (auto* 包 = 输入包根(输入参数场景, 特征_任务参数包())) {
            if (auto* 命中 = 尝试宿主(reinterpret_cast<基础信息节点类*>(包))) return 命中;
        }
        if (auto* 包 = 输入包根(输入参数场景, 特征_现实场景包())) {
            if (auto* 命中 = 尝试宿主(reinterpret_cast<基础信息节点类*>(包))) return 命中;
        }
        return nullptr;
    }

    inline bool 指针值等于条件引用(
        const 指针句柄& 值,
        const 结构_条件项视图& 条件项) noexcept
    {
        const auto 指针 = reinterpret_cast<const void*>(值.指针);
        if (!指针) return false;
        if (条件项.基础节点引用 && 指针 == 条件项.基础节点引用) return true;
        if (条件项.抽象特征引用 && 指针 == 条件项.抽象特征引用) return true;
        if (条件项.存在引用 && 指针 == 条件项.存在引用) return true;
        if (条件项.场景引用 && 指针 == 条件项.场景引用) return true;
        if (条件项.语素入口值 && 指针 == 条件项.语素入口值) return true;
        return false;
    }

    inline bool 输入实参满足条件项(
        场景节点类* 输入参数场景,
        const 结构_条件项视图& 条件项) noexcept
    {
        if (!条件项.条件特征类型) return false;

        特征节点类* 输入特征 = nullptr;
        auto* 输入宿主 = 查找输入条件实参宿主(
            输入参数场景,
            条件项.条件特征类型,
            &输入特征);
        if (!输入宿主) return false;

        if (!条件项有值约束(条件项)) {
            return true;
        }

        if (条件项.有I64点值 || 条件项.有I64区间) {
            I64 输入值 = 0;
            if (输入特征 && 世界树.读取特征_I64(输入特征, 输入值)) {
                if (条件项.有I64点值 && 输入值 != 条件项.I64点值) return false;
                if (条件项.有I64区间
                    && (输入值 < 条件项.区间下界 || 输入值 > 条件项.区间上界)) {
                    return false;
                }
                return true;
            }
            return false;
        }

        if (条件项.基础节点引用
            || 条件项.抽象特征引用
            || 条件项.存在引用
            || 条件项.场景引用
            || 条件项.语素入口值) {
            if (输入特征) {
                if (auto 值 = 读取特征当前值(输入特征)) {
                    if (const auto* 指针值 = std::get_if<指针句柄>(&*值)) {
                        return 指针值等于条件引用(*指针值, 条件项);
                    }
                }
            }

            void* 指针 = nullptr;
            if (读取基础节点指针(输入宿主, 条件项.条件特征类型, 指针) && 指针) {
                const 指针句柄 临时{ reinterpret_cast<std::uintptr_t>(指针) };
                return 指针值等于条件引用(临时, 条件项);
            }
            return false;
        }

        return true;
    }

    inline 结构_方法条件节点匹配结果 方法条件节点匹配输入场景(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        时间戳 now) noexcept
    {
        (void)now;
        结构_方法条件节点匹配结果 结果{};
        结果.条件节点 = 解析方法执行条件节点(
            方法首节点,
            抽象条件场景(方法首节点, now));
        结果.条件节点已读取 = 结果.条件节点 != nullptr;
        if (!结果.条件节点已读取) {
            结果.失败原因 = 值_方法条件结果场景缺失();
            return 结果;
        }

        const auto* 条件信息 = 结果.条件节点->主信息.取条件节点信息();
        结果.条件场景 = 条件信息 ? 条件信息->条件场景.指针 : nullptr;
        if (!结果.条件场景) {
            结果.失败原因 = 值_方法条件结果场景缺失();
            return 结果;
        }

        结果.条件集合 = 查找子特征(
            reinterpret_cast<基础信息节点类*>(结果.条件场景),
            特征_条件集合());
        结果.条件集合已读取 = 结果.条件集合 != nullptr;
        if (!结果.条件集合已读取) {
            结果.成功 = true;
            return 结果;
        }

        const auto 条件项列表 = 读取条件槽列表(
            reinterpret_cast<基础信息节点类*>(结果.条件集合));
        结果.条件项列表 = 条件项列表;
        结果.条件槽数量 = static_cast<I64>(结果.条件项列表.size());

        for (const auto& 条件项 : 结果.条件项列表) {
            if (!条件项.必需) continue;
            ++结果.必需条件数量;
            if (输入实参满足条件项(输入参数场景, 条件项)) {
                ++结果.已匹配必需条件数量;
                continue;
            }
            if (!结果.失败条件特征类型) {
                结果.失败条件特征类型 = 条件项.条件特征类型;
            }
        }

        结果.成功 = 结果.已匹配必需条件数量 == 结果.必需条件数量;
        if (!结果.成功) {
            结果.失败原因 = 输入参数场景 ? 值_必需参数缺失() : 值_输入条件包绑定失败();
        }
        return 结果;
    }

    inline bool 写入条件匹配结果(
        基础信息节点类* 宿主,
        const 结构_方法条件节点匹配结果& 匹配,
        时间戳 now) noexcept
    {
        if (!宿主) return false;
        bool ok = true;
        ok = 写入基础节点I64(宿主, 特征_条件特征类型数量(), 匹配.条件槽数量, now) && ok;
        ok = 写入基础节点I64(宿主, 特征_必需条件特征类型数量(), 匹配.必需条件数量, now) && ok;
        ok = 写入基础节点I64(
            宿主,
            特征_绑定参数数量(),
            匹配.已匹配必需条件数量,
            now) && ok;
        ok = 写入基础节点I64(
            宿主,
            特征_缺失参数数量(),
            匹配.必需条件数量 - 匹配.已匹配必需条件数量,
            now) && ok;
        if (匹配.条件节点) {
            ok = 写入基础节点指针(宿主, 特征_方法条件节点(), 匹配.条件节点, now) && ok;
        }
        if (匹配.失败原因) {
            ok = 写入基础节点指针(宿主, 特征_失败原因(), 匹配.失败原因, now) && ok;
        }
        return ok;
    }

    struct 结构_方法条件绑定结果 {
        bool 匹配成功 = false;
        bool 参数完整 = false;

        方法类::节点类* 条件节点 = nullptr;
        方法类::节点类* 结果节点 = nullptr;
        场景节点类* 输入参数场景 = nullptr;
        基础信息节点类* 输入包 = nullptr;

        方法类::节点类* 目标方法头节点 = nullptr;
        基础信息节点类* 来源因果信息 = nullptr;
        基础信息节点类* 来源观察动态 = nullptr;
        基础信息节点类* 条件结果对 = nullptr;
        基础信息节点类* 动作主体 = nullptr;
        基础信息节点类* 来源缺口 = nullptr;
        基础信息节点类* 目标特征节点 = nullptr;

        I64 绑定参数数量 = 0;
        I64 缺失参数数量 = 0;

        const 语素入口节点类* 失败原因 = nullptr;
        结构_方法条件节点匹配结果 条件匹配{};
    };

    inline bool 条件绑定声明了特征(
        const 结构_方法条件绑定结果& 绑定,
        const 语素入口节点类* 特征类型) noexcept
    {
        if (!特征类型) return false;
        if (!绑定.条件匹配.条件集合已读取) return true;
        for (const auto& 项 : 绑定.条件匹配.条件项列表) {
            if (语素入口主键相同(项.条件特征类型, 特征类型)) {
                return true;
            }
        }
        return false;
    }

    inline 基础信息节点类* 读取包基础引用或自身(
        基础信息节点类* 包,
        const 语素入口节点类* 子类型) noexcept;

    inline bool 读取条件绑定指针(
        const 结构_方法条件绑定结果& 绑定,
        const 语素入口节点类* 特征类型,
        void*& 输出指针) noexcept
    {
        输出指针 = nullptr;
        if (!条件绑定声明了特征(绑定, 特征类型)) {
            return false;
        }

        特征节点类* 输入特征 = nullptr;
        auto* 输入宿主 = 查找输入条件实参宿主(
            绑定.输入参数场景,
            特征类型,
            &输入特征);
        if (输入特征) {
            if (auto 值 = 读取特征当前值(输入特征)) {
                if (const auto* 指针值 = std::get_if<指针句柄>(&*值)) {
                    输出指针 = reinterpret_cast<void*>(指针值->指针);
                    return 输出指针 != nullptr;
                }
            }
        }

        if (输入宿主 && 读取基础节点指针(输入宿主, 特征类型, 输出指针) && 输出指针) {
            return true;
        }
        return 绑定.输入包 && 读取包指针(绑定.输入包, 特征类型, 输出指针) && 输出指针;
    }

    inline 基础信息节点类* 读取条件绑定基础节点(
        const 结构_方法条件绑定结果& 绑定,
        const 语素入口节点类* 特征类型) noexcept
    {
        void* 指针 = nullptr;
        if (读取条件绑定指针(绑定, 特征类型, 指针)) {
            if (auto* 节点 = 指针若为基础信息节点(指针)) {
                return 节点;
            }
        }
        return 条件绑定声明了特征(绑定, 特征类型)
            ? 读取包基础引用或自身(绑定.输入包, 特征类型)
            : nullptr;
    }

    inline 方法类::节点类* 自我方法根节点_动作模块() noexcept
    {
        auto* 自我存在 = 世界树.自我指针;
        return 自我存在 ? 世界树.存在().获取方法根节点(自我存在) : nullptr;
    }

    inline 方法类::节点类* 方法指针若属于自我方法树(void* 指针) noexcept
    {
        if (!指针) return nullptr;
        auto* 方法根 = 自我方法根节点_动作模块();
        if (!方法根) return nullptr;

        auto* 候选 = reinterpret_cast<方法类::节点类*>(指针);
        std::vector<方法类::节点类*> 待访问{};
        std::vector<方法类::节点类*> 已入栈{};

        const auto 已记录 = [&](const 方法类::节点类* 节点) noexcept {
            for (const auto* 已有 : 已入栈) {
                if (已有 == 节点) return true;
            }
            return false;
        };
        const auto 压入 = [&](方法类::节点类* 节点) {
            if (!节点 || 已记录(节点)) return;
            已入栈.push_back(节点);
            待访问.push_back(节点);
        };
        const auto 压入子链 = [&](方法类::节点类* 父节点) {
            auto* 首子 = 父节点 && 父节点->子
                ? static_cast<方法类::节点类*>(父节点->子)
                : nullptr;
            if (!首子) return;
            auto* 当前 = 首子;
            std::size_t 保护 = 0;
            do {
                压入(当前);
                当前 = 当前 ? static_cast<方法类::节点类*>(当前->下) : nullptr;
                ++保护;
            } while (当前 && 当前 != 首子 && 保护 < 4096);
        };

        压入(方法根);
        for (std::size_t i = 0; i < 待访问.size() && i < 4096; ++i) {
            auto* 当前 = 待访问[i];
            if (当前 == 候选) {
                return 当前;
            }
            压入子链(当前);
        }
        return nullptr;
    }

    inline 方法类::节点类* 按方法虚拟存在查找方法_动作模块(
        基础信息节点类* 方法虚拟存在,
        时间戳 now) noexcept
    {
        auto* 方法根 = 自我方法根节点_动作模块();
        if (!方法根 || !方法根->子 || !方法虚拟存在) return nullptr;

        auto* 首节点 = static_cast<方法类::节点类*>(方法根->子);
        auto* 当前 = 首节点;
        std::size_t 保护 = 0;
        do {
            if (当前 && 当前->主信息.节点种类() == 枚举_方法节点种类::方法首节点) {
                auto* 当前虚拟存在 = 当前->主信息.首节点信息().方法虚拟存在.指针
                    ? 当前->主信息.首节点信息().方法虚拟存在.指针
                    : 方法虚拟存在服务类::取或创建方法虚拟存在(
                        当前,
                        世界树.自我指针,
                        now);
                if (当前虚拟存在 == 方法虚拟存在
                    || (当前虚拟存在
                        && !当前虚拟存在->获取主键().empty()
                        && 当前虚拟存在->获取主键() == 方法虚拟存在->获取主键())) {
                    return 当前;
                }
            }
            当前 = 当前 ? static_cast<方法类::节点类*>(当前->下) : nullptr;
            ++保护;
        } while (当前 && 当前 != 首节点 && 保护 < 4096);
        return nullptr;
    }

    inline 方法类::节点类* 解析目标方法指针_动作模块(
        void* 指针,
        时间戳 now) noexcept
    {
        if (!指针) return nullptr;
        if (auto* 方法 = 方法指针若属于自我方法树(指针)) {
            return 方法;
        }
        return 按方法虚拟存在查找方法_动作模块(
            reinterpret_cast<基础信息节点类*>(指针),
            now);
    }

    inline 方法类::节点类* 读取条件绑定目标方法头节点(
        const 结构_方法条件绑定结果& 绑定,
        时间戳 now) noexcept
    {
        if (!条件绑定声明了特征(绑定, 特征_目标方法头节点())) {
            return nullptr;
        }
        // 条件节点确认“目标方法头节点”槽存在后，再兼容读取缓存 / 选中方法头字段。
        void* 指针 = nullptr;
        if (读取包指针(绑定.输入包, 特征_目标方法头节点(), 指针)
            || 读取包指针(绑定.输入包, 特征_缓存方法首节点(), 指针)
            || 读取包指针(绑定.输入包, 特征_选中方法首节点(), 指针)) {
            return 解析目标方法指针_动作模块(指针, now);
        }
        return nullptr;
    }

    inline 基础信息节点类* 读取条件绑定首个基础节点(
        const 结构_方法条件绑定结果& 绑定,
        std::initializer_list<const 语素入口节点类*> 特征集) noexcept
    {
        for (const auto* 特征类型 : 特征集) {
            if (auto* 节点 = 读取条件绑定基础节点(绑定, 特征类型)) {
                return 节点;
            }
        }
        return nullptr;
    }

    inline 结构_方法条件绑定结果 按方法条件节点绑定输入(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        时间戳 now) noexcept
    {
        结构_方法条件绑定结果 绑定{};
        绑定.输入参数场景 = 输入参数场景;
        绑定.条件匹配 = 方法条件节点匹配输入场景(
            方法首节点,
            输入参数场景,
            now);
        绑定.条件节点 = 绑定.条件匹配.条件节点;
        绑定.输入包 = reinterpret_cast<基础信息节点类*>(输入条件包根(输入参数场景));
        绑定.匹配成功 = 绑定.条件匹配.成功;
        绑定.绑定参数数量 = 绑定.条件匹配.已匹配必需条件数量;
        绑定.缺失参数数量 =
            绑定.条件匹配.必需条件数量 - 绑定.条件匹配.已匹配必需条件数量;
        if (!绑定.匹配成功) {
            绑定.失败原因 = 绑定.条件匹配.失败原因
                ? 绑定.条件匹配.失败原因
                : 值_必需参数缺失();
            return 绑定;
        }

        绑定.目标方法头节点 = 读取条件绑定目标方法头节点(绑定, now);
        绑定.来源因果信息 = 读取条件绑定基础节点(绑定, 特征_来源因果信息());
        绑定.来源观察动态 = 读取条件绑定基础节点(绑定, 特征_来源观察动态());
        绑定.条件结果对 = 读取条件绑定基础节点(绑定, 特征_条件结果对());
        绑定.动作主体 = 读取条件绑定基础节点(绑定, 特征_动作主体());
        绑定.来源缺口 = 读取条件绑定基础节点(绑定, 特征_来源缺口());
        绑定.目标特征节点 = 读取条件绑定首个基础节点(
            绑定,
            {
                特征_目标抽象特征(),
                特征_目标特征类型(),
                特征_目标特征()
            });
        绑定.参数完整 = 绑定.匹配成功 && 绑定.缺失参数数量 == 0;
        return 绑定;
    }

    inline void 写入方法条件匹配失败回执(
        基础信息节点类* 宿主,
        const 结构_方法条件节点匹配结果& 匹配,
        时间戳 now) noexcept
    {
        if (!宿主) return;
        (void)写入方法执行情况(宿主, 值_参数绑定失败(), now);
        (void)写入条件匹配结果(宿主, 匹配, now);
    }

    inline std::vector<I64区间> 抽象特征区间集合(基础信息节点类* 抽象特征) noexcept
    {
        std::vector<I64区间> 区间集合{};
        auto* 主信息 = 世界树.特征().取抽象特征主信息(
            static_cast<抽象特征节点类*>(抽象特征));
        if (!主信息) return 区间集合;

        if (主信息->值域.has_value()) {
            if (const auto* I64域 = std::get_if<I64值域>(&主信息->值域.value())) {
                for (const auto& 项 : I64域->项集) {
                    if (const auto* 值 = std::get_if<I64>(&项)) {
                        区间集合.push_back(I64区间::点(*值));
                        continue;
                    }
                    if (const auto* 区间 = std::get_if<I64区间>(&项)) {
                        if (区间->有效()) {
                            区间集合.push_back(*区间);
                        }
                    }
                }
            }
        }
        if (区间集合.empty() && 主信息->区间.has_value() && 主信息->区间->有效()) {
            区间集合.push_back(*主信息->区间);
        }
        return 区间集合;
    }

    inline 结构_类型匹配结果 抽象节点可覆盖(
        基础信息节点类* 需求节点,
        基础信息节点类* 能力节点,
        const 语素入口节点类* 能力类型,
        I64 最大层数,
        基础信息节点类* 最高允许节点,
        const 语素入口节点类* 最高允许类型) noexcept
    {
        结构_类型匹配结果 r{};
        if (!需求节点 || (!能力节点 && !能力类型)) {
            r.失败原因 = 值_没有匹配方法();
            return r;
        }

        auto* 当前 = 需求节点;
        I64 距离 = 0;
        while (当前) {
            r.路径.push_back(当前);
            if (抽象节点相同或类型相同(当前, 能力节点, 能力类型)) {
                r.成功 = true;
                r.抽象距离 = 距离;
                return r;
            }
            if (最高允许节点 || 最高允许类型) {
                if (抽象节点相同或类型相同(当前, 最高允许节点, 最高允许类型)
                    && !抽象节点相同或类型相同(当前, 能力节点, 能力类型)) {
                    r.失败原因 = 值_超出限制();
                    return r;
                }
            }
            if (距离 >= 最大层数) {
                r.失败原因 = 值_超出限制();
                return r;
            }
            当前 = 当前->父
                ? static_cast<基础信息节点类*>(当前->父)
                : nullptr;
            ++距离;
        }
        r.失败原因 = 值_没有匹配方法();
        return r;
    }

    inline bool 读取限制指针(
        基础信息节点类* 限制包,
        const 语素入口节点类* 特征类型,
        基础信息节点类*& 输出节点,
        const 语素入口节点类*& 输出语素入口) noexcept
    {
        void* 指针 = nullptr;
        if (!读取包指针(限制包, 特征类型, 指针)) return false;
        if (auto* 节点 = 指针若为基础信息节点(指针)) {
            输出节点 = 节点;
            输出语素入口 = 基础节点类型词(节点);
        }
        else {
            输出节点 = nullptr;
            输出语素入口 = reinterpret_cast<const 语素入口节点类*>(指针);
        }
        return 输出节点 || 输出语素入口;
    }

    inline bool 读取包区间(
        基础信息节点类* 包,
        const 语素入口节点类* 区间特征类型,
        I64& 输出下界,
        I64& 输出上界) noexcept
    {
        auto* 区间包 = 查找包子节点(包, 区间特征类型);
        if (!区间包) return false;
        I64 下界 = 0;
        I64 上界 = 0;
        if (读取基础节点I64(区间包, 特征_区间下界(), 下界)
            && 读取基础节点I64(区间包, 特征_区间上界(), 上界)
            && 下界 <= 上界) {
            输出下界 = 下界;
            输出上界 = 上界;
            return true;
        }

        const auto* 主信息 = 世界树.特征().取特征主信息(区间包);
        if (主信息 && 主信息->区间.has_value() && 主信息->区间->有效()) {
            输出下界 = 主信息->区间->低值;
            输出上界 = 主信息->区间->高值;
            return true;
        }
        return false;
    }

    inline bool 读取包基础节点指针(
        基础信息节点类* 包,
        const 语素入口节点类* 子类型,
        基础信息节点类*& 输出节点) noexcept
    {
        输出节点 = nullptr;
        void* 指针 = nullptr;
        if (!读取包指针(包, 子类型, 指针)) return false;
        输出节点 = 指针若为基础信息节点(指针);
        return 输出节点 != nullptr;
    }

    inline bool 复制特征当前标量到宿主(
        特征节点类* 源特征,
        基础信息节点类* 目标宿主,
        const 语素入口节点类* 目标特征类型,
        时间戳 now) noexcept
    {
        if (!源特征 || !目标宿主 || !目标特征类型) return false;
        const auto* 主信息 = 世界树.特征().取特征主信息(源特征);
        if (!主信息 || !主信息->有当前值()) return false;
        if (const auto* 值 = std::get_if<I64>(&主信息->当前值)) {
            return 写入基础节点I64(目标宿主, 目标特征类型, *值, now);
        }
        if (const auto* 指针 = std::get_if<指针句柄>(&主信息->当前值)) {
            return 写入基础节点指针(
                目标宿主,
                目标特征类型,
                reinterpret_cast<const void*>(指针->指针),
                now);
        }
        return false;
    }

    inline bool 复制包特征当前值(
        基础信息节点类* 源包,
        const 语素入口节点类* 源特征类型,
        基础信息节点类* 目标包,
        const 语素入口节点类* 目标特征类型,
        时间戳 now) noexcept
    {
        auto* 源特征 = 查找子特征(源包, 源特征类型);
        return 复制特征当前标量到宿主(
            源特征,
            目标包,
            目标特征类型 ? 目标特征类型 : 源特征类型,
            now);
    }

    inline bool 复制包I64(
        基础信息节点类* 源包,
        const 语素入口节点类* 源特征类型,
        基础信息节点类* 目标包,
        const 语素入口节点类* 目标特征类型,
        时间戳 now) noexcept
    {
        I64 值 = 0;
        return 读取基础节点I64(源包, 源特征类型, 值)
            && 写入基础节点I64(
                目标包,
                目标特征类型 ? 目标特征类型 : 源特征类型,
                值,
                now);
    }

    inline bool 复制包指针(
        基础信息节点类* 源包,
        const 语素入口节点类* 源特征类型,
        基础信息节点类* 目标包,
        const 语素入口节点类* 目标特征类型,
        时间戳 now) noexcept
    {
        void* 指针 = nullptr;
        return 读取包指针(源包, 源特征类型, 指针)
            && 写入基础节点指针(
                目标包,
                目标特征类型 ? 目标特征类型 : 源特征类型,
                指针,
                now);
    }

    inline 基础信息节点类* 读取包基础引用或自身(
        基础信息节点类* 包,
        const 语素入口节点类* 特征类型) noexcept
    {
        基础信息节点类* 节点 = nullptr;
        if (读取包基础节点指针(包, 特征类型, 节点)) {
            return 节点;
        }
        return 查找包子节点(包, 特征类型);
    }

    inline 基础信息节点类* 读取包抽象特征引用(
        基础信息节点类* 包,
        const 语素入口节点类* 主特征类型) noexcept
    {
        auto 解析一个 = [&](const 语素入口节点类* 特征类型) noexcept -> 基础信息节点类* {
            if (!特征类型) return nullptr;
            void* 指针 = nullptr;
            if (读取包指针(包, 特征类型, 指针)) {
                if (auto* 节点 = 指针若为基础信息节点(指针)) {
                    if (世界树.特征().取抽象特征主信息(static_cast<抽象特征节点类*>(节点))) {
                        return 节点;
                    }
                    if (const auto* 类型 = 基础节点类型词(节点)) {
                        return reinterpret_cast<基础信息节点类*>(
                            查找抽象特征_按类型(类型));
                    }
                }
                return reinterpret_cast<基础信息节点类*>(
                    查找抽象特征_按类型(reinterpret_cast<const 语素入口节点类*>(指针)));
            }

            auto* 子节点 = 查找包子节点(包, 特征类型);
            if (!子节点) return nullptr;
            if (世界树.特征().取抽象特征主信息(static_cast<抽象特征节点类*>(子节点))) {
                return 子节点;
            }
            const auto* 子特征主信息 = 世界树.特征().取特征主信息(
                static_cast<特征节点类*>(子节点));
            return 子特征主信息
                ? reinterpret_cast<基础信息节点类*>(
                    查找抽象特征_按类型(子特征主信息->类型))
                : nullptr;
        };

        return 解析一个(主特征类型);
    }

    inline bool 写入包区间(
        基础信息节点类* 包,
        I64 下界,
        I64 上界,
        时间戳 now) noexcept
    {
        return 包
            && 下界 <= 上界
            && 写入基础节点I64(包, 特征_区间下界(), 下界, now)
            && 写入基础节点I64(包, 特征_区间上界(), 上界, now);
    }

    inline 场景节点类* 创建子场景_按特征名(
        基础信息节点类* 宿主,
        const 语素入口节点类* 场景名) noexcept
    {
        return 宿主 && 场景名
            ? 世界树.取或创建子场景_按名称(
                宿主,
                场景名,
                枚举_世界类型::内部世界)
            : nullptr;
    }

    inline bool 读取包语素入口指针(
        基础信息节点类* 包,
        const 语素入口节点类* 子类型,
        const 语素入口节点类*& 输出语素入口) noexcept
    {
        输出语素入口 = nullptr;
        void* 指针 = nullptr;
        if (!读取包指针(包, 子类型, 指针)) return false;
        if (auto* 节点 = 指针若为基础信息节点(指针)) {
            输出语素入口 = 基础节点类型词(节点);
        }
        else {
            输出语素入口 = reinterpret_cast<const 语素入口节点类*>(指针);
        }
        return 输出语素入口 != nullptr;
    }

    inline bool 包目标值类型为I64(基础信息节点类* 输入条件包) noexcept
    {
        const 语素入口节点类* 语素入口值 = nullptr;
        if (读取包语素入口指针(输入条件包, 特征_目标值类型(), 语素入口值)) {
            return 语素入口值 == 值类型_I64()
                || 语素入口值 == 值类型_类型I64();
        }

        auto* 值类型特征 = 查找子特征(输入条件包, 特征_目标值类型());
        const auto* 主信息 = 世界树.特征().取特征主信息(值类型特征);
        return 主信息
            && (主信息->类型 == 值类型_I64()
                || 主信息->类型 == 值类型_类型I64());
    }

    inline void 吸收差值约束抽象区间(
        基础信息节点类* 抽象特征,
        std::vector<I64区间>& 输出区间) noexcept
    {
        if (!抽象特征) return;
        auto 区间集合 = 抽象特征区间集合(抽象特征);
        输出区间.insert(输出区间.end(), 区间集合.begin(), 区间集合.end());
    }

    inline void 吸收差值约束节点区间(
        基础信息节点类* 节点,
        std::vector<I64区间>& 输出区间) noexcept
    {
        if (!节点) return;

        const auto* 抽象主信息 = 世界树.特征().取抽象特征主信息(
            static_cast<const 抽象特征节点类*>(节点));
        if (抽象主信息) {
            吸收差值约束抽象区间(节点, 输出区间);
        }

        I64 下界 = 0;
        I64 上界 = 0;
        if (读取基础节点I64(节点, 特征_区间下界(), 下界)
            && 读取基础节点I64(节点, 特征_区间上界(), 上界)
            && 下界 <= 上界) {
            输出区间.push_back(I64区间{ 下界, 上界 });
        }

        const auto* 特征主信息 = 世界树.特征().取特征主信息(
            static_cast<const 特征节点类*>(节点));
        if (特征主信息 && 特征主信息->区间.has_value() && 特征主信息->区间->有效()) {
            输出区间.push_back(*特征主信息->区间);
        }
        if (特征主信息 && 特征主信息->有当前值()) {
            if (const auto* 指针 = std::get_if<指针句柄>(&特征主信息->当前值)) {
                if (auto* 引用节点 = 指针若为基础信息节点(
                        reinterpret_cast<const void*>(指针->指针))) {
                    吸收差值约束抽象区间(引用节点, 输出区间);
                }
            }
        }

        void* 抽象引用 = nullptr;
        if (读取包指针(节点, 特征_抽象特征引用(), 抽象引用)) {
            if (auto* 引用节点 = 指针若为基础信息节点(抽象引用)) {
                吸收差值约束抽象区间(引用节点, 输出区间);
            }
        }
    }

    inline std::vector<I64区间> 读取差值约束区间集合(
        基础信息节点类* 差值约束包) noexcept
    {
        std::vector<I64区间> 区间集合{};
        if (!差值约束包) return 区间集合;

        吸收差值约束节点区间(差值约束包, 区间集合);
        for (auto* 子特征 : 世界树.特征().获取子特征(差值约束包)) {
            吸收差值约束节点区间(
                reinterpret_cast<基础信息节点类*>(子特征),
                区间集合);
        }
        return 区间集合;
    }

    inline bool 差值约束为唯一零区间(const std::vector<I64区间>& 约束区间) noexcept
    {
        return 约束区间.size() == 1
            && 约束区间.front().低值 == 0
            && 约束区间.front().高值 == 0;
    }

    inline bool 命中差值约束(
        I64 实际差值,
        const std::vector<I64区间>& 约束区间) noexcept
    {
        for (const auto& 区间 : 约束区间) {
            if (区间.低值 <= 实际差值 && 实际差值 <= 区间.高值) {
                return true;
            }
        }
        return false;
    }

    inline 特征节点类* 查找宿主实例特征_按抽象特征(
        基础信息节点类* 宿主,
        基础信息节点类* 抽象特征) noexcept
    {
        auto* 抽象 = static_cast<抽象特征节点类*>(抽象特征);
        if (!宿主 || !世界树.特征().取抽象特征主信息(抽象)) return nullptr;
        return 世界树.特征().查找实例特征_按抽象特征(宿主, 抽象);
    }

    inline 特征节点类* 创建宿主实例特征_按抽象特征(
        基础信息节点类* 宿主,
        基础信息节点类* 抽象特征) noexcept
    {
        auto* 抽象 = static_cast<抽象特征节点类*>(抽象特征);
        if (!宿主 || !世界树.特征().取抽象特征主信息(抽象)) return nullptr;
        return 世界树.特征().取或创建实例特征_按抽象特征(宿主, 抽象);
    }

    inline bool 读取实例特征I64(特征节点类* 实例特征, I64& 输出值) noexcept
    {
        输出值 = 0;
        return 实例特征 && 世界树.读取特征_I64(实例特征, 输出值);
    }

    inline bool 写入实例特征I64(
        特征节点类* 实例特征,
        I64 新值,
        时间戳 now) noexcept
    {
        return 实例特征 && 世界树.写入特征_I64(实例特征, 新值, now);
    }

    inline 基础信息节点类* 解析实例特征命中抽象特征(
        特征节点类* 实例特征,
        时间戳 now) noexcept
    {
        (void)now;
        return reinterpret_cast<基础信息节点类*>(
            世界树.特征().解析实例特征命中抽象特征(实例特征));
    }

    inline bool 解析查找限制包(基础信息节点类* 限制包, 结构_查找限制& 输出限制) noexcept
    {
        if (!限制包) return false;
        输出限制 = 结构_查找限制{};

        (void)读取限制指针(
            限制包,
            特征_最高允许目标特征类型(),
            输出限制.最高允许目标特征类型,
            输出限制.最高允许目标特征类型词);
        (void)读取限制指针(
            限制包,
            特征_最高允许宿主类型(),
            输出限制.最高允许宿主类型,
            输出限制.最高允许宿主类型词);

        void* 允许值类型指针 = nullptr;
        if (读取包指针(限制包, 特征_允许值类型(), 允许值类型指针)) {
            输出限制.允许值类型 = reinterpret_cast<const 语素入口节点类*>(允许值类型指针);
        }

        (void)读取基础节点I64(限制包, 特征_最大类型抽象层数(), 输出限制.最大类型抽象层数);
        (void)读取基础节点I64(限制包, 特征_最大值域抽象层数(), 输出限制.最大值域抽象层数);
        (void)读取基础节点I64(限制包, 特征_最大候选数量(), 输出限制.最大候选数量);
        (void)读取基础节点I64(限制包, 特征_最大查找递归深度(), 输出限制.最大查找递归深度);

        I64 布尔值 = 0;
        if (读取基础节点I64(限制包, 特征_是否包含禁用方法(), 布尔值)) {
            输出限制.是否包含禁用方法 = 布尔值 != 0;
        }
        if (读取基础节点I64(限制包, 特征_是否允许当前方法自身(), 布尔值)) {
            输出限制.是否允许当前方法自身 = 布尔值 != 0;
        }
        I64 下界 = 0;
        I64 上界 = 0;
        if (读取包区间(限制包, 特征_允许执行差值区间(), 下界, 上界)) {
            输出限制.允许执行差值下界 = 下界;
            输出限制.允许执行差值上界 = 上界;
            输出限制.有允许执行差值区间 = true;
        }

        return 输出限制.最大类型抽象层数 >= 0
            && 输出限制.最大值域抽象层数 >= 0
            && 输出限制.最大候选数量 >= 0
            && 输出限制.最大查找递归深度 >= 0
            && (!输出限制.有允许执行差值区间
                || 输出限制.允许执行差值下界 <= 输出限制.允许执行差值上界);
    }

    inline bool 需求包有必需条件项(const std::vector<结构_条件项视图>& 需求项列表) noexcept
    {
        for (const auto& 项 : 需求项列表) {
            if (项.必需 && 项.条件特征类型 && 条件项有值约束(项)) return true;
        }
        return false;
    }

    inline I64 读取方法自我能力值(方法类::节点类* 方法首节点) noexcept
    {
        if (!方法首节点
            || 方法首节点->主信息.首节点信息().动作句柄.类型 != 枚举_动作句柄类型::本能函数ID) {
            return 0;
        }
        const I64 能力值 = 方法首节点->主信息.首节点信息().动作句柄.本能ID;
        return 本能方法类::是有效本能方法能力值(能力值) ? 能力值 : 0;
    }

}

export namespace 自我动作实现模块 {
    inline 存在节点类* 练习(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 尝试学习(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 定向学习(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交任务状态变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交方法可执行入口(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 建立条件结果对(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交方法状态变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 结算叶子任务价值(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我建立观察帧存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我建立观察帧到自我场景接口(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我建立像素观察事实索引(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我承接像素特征值(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我校验观察事实完备性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我评估观察帧质量(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我诊断观察质量缺口并派生补观察需求(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我判断基础观察事实可用性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 安全评估当前场景安全性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我读取自我关键特征变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我建立最小自我边界参照(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我生成存在自我相对安全特征(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 安全构造场景影响因果候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我明确当前场景持续观察需求(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 安全持续观察当前场景(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我明确负向影响处置需求(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 安全执行降级保护(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我验证外设观察像素簇候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我识别外设观察材料(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我扫描已识别区域(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我跟踪指定存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我确认外设观察存在候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交确认观察存在事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交观察存在特征值变化事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交指定存在跟踪事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交当前观察范围可观测单位存在对应事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交观察存在发现事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交风险安全场景影响部分状态变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 安全搜索安全因果因素证据(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交安全因果因素无负证据默认满足(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交安全因果因素证据评估值(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交领取OR组结算令牌(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我按已确认轮廓分区当前帧(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我调整相机参数并重采样(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我按ROI重算观察质量(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我执行多帧重采样并择优(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我生成低置信补偿观察候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我调整外设观察姿态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我调整观察位置(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我按像素坐标回查观察事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我提取空间候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我判断空间候选相对独立性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我生成空间候选观察队列(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我观察单个空间候选并组合存在假设(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我用空间范围投影覆盖原帧验证像素归属(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我确认观察存在并更新帧解释状态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我确保存在内部世界(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我建立存在内外场景接口(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我将存在局部观察切片映射到内部世界(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我在内部世界提取子候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我确认内部世界子存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 提交确认内部世界子存在事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我递归细分存在内部世界(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 使双目相机达到可用状态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 释放双目相机(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 检查双目相机运行状态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 用双目相机刷新当前观察场景(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 用双目相机发现候选存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 用双目相机取得当前观察帧(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 用双目相机取得当前观察特征帧(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 用双目相机建立当前帧像素特征索引(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我匹配当前观察存在与历史观察存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我比较存在特征变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我生成观察变化事件(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我导出当前场景识别快照(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 自我导出OpenGL复现数据(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;

    inline bool 注册本能函数执行闭环() noexcept
    {
        bool ok = true;
        内部模块::内部本能函数集 内部函数{};
        内部函数.练习 = &练习;
        内部函数.尝试学习 = &尝试学习;
        内部函数.定向学习 = &定向学习;
        内部函数.提交任务状态变化 = &提交任务状态变化;
        内部函数.提交方法可执行入口 = &提交方法可执行入口;
        内部函数.建立条件结果对 = &建立条件结果对;
        内部函数.提交方法状态变化 = &提交方法状态变化;
        内部函数.结算叶子任务价值 = &结算叶子任务价值;
        内部函数.建立观察帧存在 = &自我建立观察帧存在;
        内部函数.建立观察帧到自我场景接口 = &自我建立观察帧到自我场景接口;
        内部函数.建立像素观察事实索引 = &自我建立像素观察事实索引;
        内部函数.承接像素特征值 = &自我承接像素特征值;
        内部函数.校验观察事实完备性 = &自我校验观察事实完备性;
        内部函数.评估观察帧质量 = &自我评估观察帧质量;
        内部函数.诊断观察质量缺口并派生补观察需求 = &自我诊断观察质量缺口并派生补观察需求;
        内部函数.判断基础观察事实可用性 = &自我判断基础观察事实可用性;
        内部函数.安全评估当前场景安全性 = &安全评估当前场景安全性;
        内部函数.读取自我关键特征变化 = &自我读取自我关键特征变化;
        内部函数.建立最小自我边界参照 = &自我建立最小自我边界参照;
        内部函数.生成存在自我相对安全特征 = &自我生成存在自我相对安全特征;
        内部函数.构造场景影响因果候选 = &安全构造场景影响因果候选;
        内部函数.明确当前场景持续观察需求 = &自我明确当前场景持续观察需求;
        内部函数.持续观察当前场景 = &安全持续观察当前场景;
        内部函数.明确负向影响处置需求 = &自我明确负向影响处置需求;
        内部函数.执行降级保护 = &安全执行降级保护;
        内部函数.验证外设观察像素簇候选 = &自我验证外设观察像素簇候选;
        内部函数.识别外设观察材料 = &自我识别外设观察材料;
        内部函数.扫描已识别区域 = &自我扫描已识别区域;
        内部函数.跟踪指定存在 = &自我跟踪指定存在;
        内部函数.确认外设观察存在候选 = &自我确认外设观察存在候选;
        内部函数.提交确认观察存在事实 = &提交确认观察存在事实;
        内部函数.提交观察存在特征值变化事实 = &提交观察存在特征值变化事实;
        内部函数.提交指定存在跟踪事实 = &提交指定存在跟踪事实;
        内部函数.提交当前观察范围可观测单位存在对应事实 = &提交当前观察范围可观测单位存在对应事实;
        内部函数.提交观察存在发现事实 = &提交观察存在发现事实;
        内部函数.按已确认轮廓分区当前帧 = &自我按已确认轮廓分区当前帧;
        内部函数.提交风险安全场景影响部分状态变化 = &提交风险安全场景影响部分状态变化;
        内部函数.搜索安全因果因素证据 = &安全搜索安全因果因素证据;
        内部函数.提交安全因果因素无负证据默认满足 = &提交安全因果因素无负证据默认满足;
        内部函数.提交安全因果因素证据评估值 = &提交安全因果因素证据评估值;
        内部函数.提交领取OR组结算令牌 = &提交领取OR组结算令牌;
        内部函数.调整相机参数并重采样 = &自我调整相机参数并重采样;
        内部函数.按ROI重算观察质量 = &自我按ROI重算观察质量;
        内部函数.执行多帧重采样并择优 = &自我执行多帧重采样并择优;
        内部函数.生成低置信补偿观察候选 = &自我生成低置信补偿观察候选;
        内部函数.调整外设观察姿态 = &自我调整外设观察姿态;
        内部函数.调整观察位置 = &自我调整观察位置;
        内部函数.建立当前帧像素特征索引兼容入口 = &用双目相机建立当前帧像素特征索引;
        内部函数.按像素坐标回查观察事实 = &自我按像素坐标回查观察事实;
        内部函数.提取空间候选 = &自我提取空间候选;
        内部函数.判断空间候选相对独立性 = &自我判断空间候选相对独立性;
        内部函数.生成空间候选观察队列 = &自我生成空间候选观察队列;
        内部函数.观察单个空间候选并组合存在假设 = &自我观察单个空间候选并组合存在假设;
        内部函数.用空间范围投影覆盖原帧验证像素归属 = &自我用空间范围投影覆盖原帧验证像素归属;
        内部函数.确保存在内部世界 = &自我确保存在内部世界;
        内部函数.建立存在内外场景接口 = &自我建立存在内外场景接口;
        内部函数.将存在局部观察切片映射到内部世界 = &自我将存在局部观察切片映射到内部世界;
        内部函数.在内部世界提取子候选 = &自我在内部世界提取子候选;
        内部函数.确认内部世界子存在 = &自我确认内部世界子存在;
        内部函数.提交确认内部世界子存在事实 = &提交确认内部世界子存在事实;
        内部函数.递归细分存在内部世界 = &自我递归细分存在内部世界;
        内部函数.匹配当前观察存在与历史观察存在 = &自我匹配当前观察存在与历史观察存在;
        内部函数.比较存在特征变化 = &自我比较存在特征变化;
        内部函数.生成观察变化事件 = &自我生成观察变化事件;
        内部函数.导出当前场景识别快照 = &自我导出当前场景识别快照;
        内部函数.导出OpenGL复现数据 = &自我导出OpenGL复现数据;
        ok = 内部模块::注册内部本能函数执行闭环(内部函数) && ok;
        ok = 外部模块::注册外部本能函数执行闭环() && ok;
        return ok;
    }

    inline bool 确认练习规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标方法头节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源缺口(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标特征类型(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标抽象特征(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_练习方向(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_执行差值(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标执行后值(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标方法头节点(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_来源缺口(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标特征类型(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标抽象特征(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_练习方向(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_执行差值(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标执行后值(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法补齐类别()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法补齐状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法补齐尝试结果()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法补齐结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标方法头节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_试运行次数()) && ok;
            ok = 取或创建子特征(宿主, 特征_条件结果对()) && ok;
            ok = 取或创建子特征(宿主, 特征_练习模式()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选实参表()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选实参数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_绑定参数数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_缺失参数数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_可执行输入参数场景()) && ok;
            ok = 取或创建子特征(宿主, 特征_最近运行虚拟存在()) && ok;
            ok = 取或创建子特征(宿主, 特征_参数新颖性状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选方法能力包()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选形参包()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选结果包()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法条件节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法结果节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法最近连续成功次数()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_方法最近连续成功次数(),
            4) && ok;
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_方法状态(),
            方法状态_好用()) && ok;
        // 练习本身是内核学习反射：它负责把目标方法从可用继续推向好用。
        // 自身只要求具备动作入口和条件节点输入槽，不用先给自己伪造条件结果对。
        return ok;
    }

    inline 方法类::节点类* 读取目标方法头节点(
        基础信息节点类* 输入包,
        时间戳 now) noexcept
    {
        void* 指针 = nullptr;
        if (读取包指针(输入包, 特征_目标方法头节点(), 指针)
            || 读取包指针(输入包, 特征_缓存方法首节点(), 指针)
            || 读取包指针(输入包, 特征_选中方法首节点(), 指针)) {
            return 解析目标方法指针_动作模块(指针, now);
        }
        return nullptr;
    }

    inline 方法类::节点类* 读取目标方法头节点(
        基础信息节点类* 输入包) noexcept
    {
        return 读取目标方法头节点(
            输入包,
            结构体_时间戳::当前_微秒());
    }

    inline bool 练习输入具有目标特征(
        基础信息节点类* 输入包) noexcept
    {
        if (!输入包) return false;
        void* 指针 = nullptr;
        return 读取包指针(输入包, 特征_目标特征类型(), 指针)
            || 读取包指针(输入包, 特征_目标抽象特征(), 指针)
            || 读取包指针(输入包, 特征_目标特征(), 指针);
    }

    inline bool 练习输入具有方向或目标值(
        基础信息节点类* 输入包) noexcept
    {
        if (!输入包) return false;
        I64 值 = 0;
        return 读取基础节点I64(输入包, 特征_练习方向(), 值)
            || 读取基础节点I64(输入包, 特征_改变方向(), 值)
            || 读取基础节点I64(输入包, 特征_执行差值(), 值)
            || 读取基础节点I64(输入包, 特征_目标执行后值(), 值);
    }

    inline bool 练习输入具有指定参数(
        基础信息节点类* 输入包) noexcept
    {
        return 练习输入具有目标特征(输入包)
            || 练习输入具有方向或目标值(输入包);
    }

    inline I64 练习方向转差值(I64 方向) noexcept
    {
        if (方向 > 0) return 1;
        if (方向 < 0) return -1;
        return 0;
    }

    inline I64 生成无目的未出现I64参数(I64 已生成数量) noexcept
    {
        auto x = static_cast<std::uint64_t>(已生成数量) + 0x9E3779B97F4A7C15ull;
        x ^= x >> 30;
        x *= 0xBF58476D1CE4E5B9ull;
        x ^= x >> 27;
        x *= 0x94D049BB133111EBull;
        x ^= x >> 31;
        return static_cast<I64>(x);
    }

    inline bool 语素入口相同(
        const 语素入口节点类* 左,
        const 语素入口节点类* 右) noexcept
    {
        if (左 == 右) return true;
        if (!左 || !右) return false;
        return 左->获取主键() == 右->获取主键();
    }

    inline bool 参数值类型为I64(基础信息节点类* 形参节点) noexcept
    {
        void* 值类型指针 = nullptr;
        if (!读取包指针(形参节点, 特征_参数值类型(), 值类型指针)) {
            return false;
        }
        const auto* 值类型 = reinterpret_cast<const 语素入口节点类*>(值类型指针);
        return 语素入口相同(值类型, 值类型_I64())
            || 语素入口相同(值类型, 值类型_类型I64());
    }

    inline bool 形参为必需(基础信息节点类* 形参节点) noexcept
    {
        I64 是否必需值 = 1;
        (void)读取基础节点I64(形参节点, 特征_是否必需(), 是否必需值);
        return 是否必需值 != 0;
    }

    inline bool 读取参数允许区间(
        基础信息节点类* 形参节点,
        const 语素入口节点类* 参数特征类型,
        I64& 下界,
        I64& 上界) noexcept
    {
        下界 = (std::numeric_limits<I64>::min)();
        上界 = (std::numeric_limits<I64>::max)();
        if (形参节点) {
            I64 直接下界 = 0;
            I64 直接上界 = 0;
            if (读取基础节点I64(形参节点, 特征_区间下界(), 直接下界)
                && 读取基础节点I64(形参节点, 特征_区间上界(), 直接上界)
                && 直接下界 <= 直接上界) {
                下界 = 直接下界;
                上界 = 直接上界;
                return true;
            }
            if (读取包区间(形参节点, 特征_允许执行差值区间(), 直接下界, 直接上界)) {
                下界 = 直接下界;
                上界 = 直接上界;
                return true;
            }
        }
        if (语素入口相同(参数特征类型, 特征_练习方向())
            || 语素入口相同(参数特征类型, 特征_改变方向())) {
            下界 = -1;
            上界 = 1;
            return true;
        }
        return true;
    }

    inline I64 区间中点(I64 下界, I64 上界) noexcept
    {
        return 下界 / 2 + 上界 / 2 + (下界 % 2 + 上界 % 2) / 2;
    }

    inline std::uint64_t I64有序编码(I64 值) noexcept
    {
        return static_cast<std::uint64_t>(值) ^ (std::uint64_t{ 1 } << 63);
    }

    inline I64 I64由有序编码(std::uint64_t 编码) noexcept
    {
        return static_cast<I64>(编码 ^ (std::uint64_t{ 1 } << 63));
    }

    inline std::uint64_t 有效候选预算(I64 最大候选数量) noexcept
    {
        if (最大候选数量 <= 0) return 64;
        constexpr std::uint64_t 上限 = 4096;
        const auto 值 = static_cast<std::uint64_t>(最大候选数量);
        return 值 > 上限 ? 上限 : 值;
    }

    inline std::uint64_t 安全域大小(std::uint64_t 左侧数量, std::uint64_t 右侧数量) noexcept
    {
        constexpr auto 最大值 = (std::numeric_limits<std::uint64_t>::max)();
        if (最大值 - 左侧数量 <= 右侧数量) {
            return 最大值;
        }
        return 左侧数量 + 右侧数量 + 1;
    }

    inline I64 生成区间扩展覆盖I64参数(
        I64 已生成数量,
        I64 下界,
        I64 上界,
        I64 中点,
        I64 最大候选数量,
        I64& 输出策略) noexcept
    {
        输出策略 = 2;
        const auto 下界码 = I64有序编码(下界);
        const auto 上界码 = I64有序编码(上界);
        const auto 中点码 = I64有序编码(中点);
        const auto 左侧数量 = 中点码 - 下界码;
        const auto 右侧数量 = 上界码 - 中点码;

        auto 序号 = 已生成数量 < 0
            ? std::uint64_t{ 0 }
            : static_cast<std::uint64_t>(已生成数量);
        const auto 总数 = 安全域大小(左侧数量, 右侧数量);
        const auto 预算 = 有效候选预算(最大候选数量);
        const bool 值域过大 = 总数 > 预算;
        if (值域过大) {
            序号 %= 预算;
            const auto 步幅 = 总数 / 预算 + ((总数 % 预算) ? 1 : 0);
            if (序号 > 0 && 步幅 > 1) {
                序号 *= 步幅;
                if (序号 >= 总数) {
                    序号 = 总数 - 1;
                }
            }
            输出策略 = 4;
        } else if (序号 >= 总数) {
            序号 %= 总数;
        }
        if (序号 == 0) {
            return 中点;
        }

        const auto 公共侧数量 = (左侧数量 < 右侧数量)
            ? 左侧数量
            : 右侧数量;
        const auto 公共覆盖数量 = std::uint64_t{ 1 } + 公共侧数量 * std::uint64_t{ 2 };
        if (序号 < 公共覆盖数量) {
            const auto 步长 = (序号 + 1) / 2;
            if ((序号 % 2) == 1) {
                return I64由有序编码(中点码 - 步长);
            }
            return I64由有序编码(中点码 + 步长);
        }

        const auto 剩余序号 = 序号 - 公共覆盖数量;
        const auto 步长 = 公共侧数量 + std::uint64_t{ 1 } + 剩余序号;
        if (左侧数量 > 右侧数量) {
            return I64由有序编码(中点码 - 步长);
        }
        return I64由有序编码(中点码 + 步长);
    }

    inline I64 生成区间内I64参数(
        I64 已生成数量,
        I64 下界,
        I64 上界,
        bool 有历史值,
        I64 历史值,
        I64 最大候选数量,
        I64& 输出策略) noexcept
    {
        输出策略 = 0;
        if (下界 > 上界) {
            下界 = 上界 = 0;
        }
        if (有历史值 && 下界 <= 历史值 && 历史值 <= 上界
            && 已生成数量 > 0
            && 已生成数量 % 17 == 0) {
            输出策略 = 1;
            return 历史值;
        }
        const I64 中点 = 区间中点(下界, 上界);
        // 条件构建优先从区间中点开始，再左右扩展，逐步覆盖完整值域。
        return 生成区间扩展覆盖I64参数(
            已生成数量,
            下界,
            上界,
            中点,
            最大候选数量,
            输出策略);
    }

    inline I64 读取参数最大候选数量(基础信息节点类* 形参节点) noexcept
    {
        I64 最大候选数量 = 64;
        if (形参节点) {
            (void)读取基础节点I64(
                形参节点,
                特征_最大候选数量(),
                最大候选数量);
        }
        return 最大候选数量;
    }

    inline bool 复制任务参数到候选实参(
        基础信息节点类* 输入包,
        const 语素入口节点类* 参数特征类型,
        基础信息节点类* 候选参数,
        时间戳 now) noexcept
    {
        if (!输入包 || !参数特征类型 || !候选参数) return false;
        auto* 候选特征 = dynamic_cast<特征节点类*>(候选参数);
        if (!候选特征) return false;
        if (语素入口主键相同(参数特征类型, 特征_输入条件包())) {
            bool ok = true;
            ok = 写入基础节点指针(
                候选参数,
                特征_参数值类型(),
                值类型_场景指针(),
                now) && ok;
            ok = 世界树.写入特征_指针(
                候选特征,
                输入包,
                now) && ok;
            ok = 写入基础节点指针(
                候选参数,
                特征_参数来源(),
                值_来源_任务提供参数(),
                now) && ok;
            return ok;
        }
        auto* 源特征 = 查找子特征(输入包, 参数特征类型);
        const auto* 源主信息 = 源特征
            ? 世界树.特征().取特征主信息(源特征)
            : nullptr;
        if (!源主信息 || !源主信息->有当前值()) {
            return false;
        }

        bool 已复制 = false;
        if (const auto* 值 = std::get_if<I64>(&源主信息->当前值)) {
            已复制 = 世界树.写入特征_I64(候选特征, *值, now);
        }
        else if (const auto* 指针 = std::get_if<指针句柄>(&源主信息->当前值);
            指针 && 指针->有效()) {
            已复制 = 世界树.写入特征_指针(
                候选特征,
                reinterpret_cast<const void*>(指针->指针),
                now);
        }
        if (!已复制) {
            return false;
        }
        (void)写入基础节点指针(候选参数, 特征_参数来源(), 值_来源_任务提供参数(), now);
        return true;
    }

    inline bool 写入候选实参I64(
        基础信息节点类* 实参表宿主,
        const 语素入口节点类* 参数特征类型,
        I64 参数值,
        const 语素入口节点类* 参数来源,
        时间戳 now) noexcept
    {
        if (!实参表宿主 || !参数特征类型) return false;
        auto* 候选参数 = 取或创建子特征(实参表宿主, 参数特征类型);
        if (!候选参数) return false;
        auto* 候选宿主 = reinterpret_cast<基础信息节点类*>(候选参数);
        bool ok = true;
        ok = 写入基础节点指针(候选宿主, 特征_参数值类型(), 值类型_I64(), now) && ok;
        ok = 世界树.写入特征_I64(候选参数, 参数值, now) && ok;
        if (参数来源) {
            ok = 写入基础节点指针(候选宿主, 特征_参数来源(), 参数来源, now) && ok;
        }
        return ok;
    }

    inline bool 写入候选实参指针(
        基础信息节点类* 实参表宿主,
        const 语素入口节点类* 参数特征类型,
        const 语素入口节点类* 参数值类型,
        void* 参数值,
        const 语素入口节点类* 参数来源,
        时间戳 now) noexcept
    {
        if (!实参表宿主 || !参数特征类型 || !参数值) return false;
        auto* 候选参数 = 取或创建子特征(实参表宿主, 参数特征类型);
        if (!候选参数) return false;
        auto* 候选宿主 = reinterpret_cast<基础信息节点类*>(候选参数);
        bool ok = true;
        ok = 写入基础节点指针(
            候选宿主,
            特征_参数值类型(),
            参数值类型 ? 参数值类型 : 值类型_场景指针(),
            now) && ok;
        ok = 世界树.写入特征_指针(候选参数, 参数值, now) && ok;
        if (参数来源) {
            ok = 写入基础节点指针(候选宿主, 特征_参数来源(), 参数来源, now) && ok;
        }
        return ok;
    }

    inline bool 复制输入指针到候选实参(
        基础信息节点类* 输入包,
        基础信息节点类* 实参表宿主,
        const 语素入口节点类* 参数特征类型,
        const 语素入口节点类* 参数值类型,
        时间戳 now) noexcept
    {
        if (!输入包 || !实参表宿主 || !参数特征类型) return false;
        void* 指针 = nullptr;
        return 读取包指针(输入包, 参数特征类型, 指针)
            && 写入候选实参指针(
                实参表宿主,
                参数特征类型,
                参数值类型,
                指针,
                值_来源_任务提供参数(),
                now);
    }

    inline bool 复制输入I64到候选实参(
        基础信息节点类* 输入包,
        基础信息节点类* 实参表宿主,
        const 语素入口节点类* 参数特征类型,
        时间戳 now) noexcept
    {
        if (!输入包 || !实参表宿主 || !参数特征类型) return false;
        I64 值 = 0;
        return 读取基础节点I64(输入包, 参数特征类型, 值)
            && 写入候选实参I64(
                实参表宿主,
                参数特征类型,
                值,
                值_来源_任务提供参数(),
                now);
    }

    inline std::size_t 子节点数量(基础信息节点类* 集合) noexcept
    {
        auto* 首子 = 集合 && 集合->子 ? static_cast<基础信息节点类*>(集合->子) : nullptr;
        if (!首子) return 0;
        std::size_t 数量 = 0;
        auto* 当前 = 首子;
        do {
            ++数量;
            当前 = static_cast<基础信息节点类*>(当前->下);
        } while (当前 && 当前 != 首子);
        return 数量;
    }

    inline std::size_t 离散集合扩展索引(std::size_t 数量, I64 已生成数量) noexcept
    {
        if (数量 == 0) return 0;
        auto 序号 = 已生成数量 < 0
            ? std::size_t{ 0 }
            : static_cast<std::size_t>(已生成数量);
        序号 %= 数量;
        const auto 中点 = (数量 - 1) / 2;
        if (序号 == 0) return 中点;

        const auto 左侧数量 = 中点;
        const auto 右侧数量 = 数量 - 1 - 中点;
        const auto 公共侧数量 = (左侧数量 < 右侧数量) ? 左侧数量 : 右侧数量;
        const auto 公共覆盖数量 = std::size_t{ 1 } + 公共侧数量 * std::size_t{ 2 };
        if (序号 < 公共覆盖数量) {
            const auto 步长 = (序号 + 1) / 2;
            return (序号 % 2) == 1 ? 中点 - 步长 : 中点 + 步长;
        }

        const auto 剩余序号 = 序号 - 公共覆盖数量;
        if (左侧数量 > 右侧数量) {
            return 中点 - (公共侧数量 + std::size_t{ 1 } + 剩余序号);
        }
        return 中点 + (公共侧数量 + std::size_t{ 1 } + 剩余序号);
    }

    inline 基础信息节点类* 取离散候选节点(
        基础信息节点类* 集合,
        I64 已生成数量) noexcept
    {
        const auto 数量 = 子节点数量(集合);
        if (数量 == 0) return nullptr;
        const auto 目标索引 = 离散集合扩展索引(数量, 已生成数量);
        auto* 当前 = static_cast<基础信息节点类*>(集合->子);
        for (std::size_t i = 0; 当前 && i < 目标索引; ++i) {
            当前 = static_cast<基础信息节点类*>(当前->下);
        }
        return 当前;
    }

    inline bool 读取节点指针实参(
        基础信息节点类* 节点,
        void*& 输出指针) noexcept
    {
        输出指针 = nullptr;
        if (!节点) return false;
        if (auto* 特征 = dynamic_cast<特征节点类*>(节点)) {
            if (世界树.读取特征_指针(特征, 输出指针) && 输出指针) {
                return true;
            }
        }
        输出指针 = 节点;
        return true;
    }

    inline bool 读取特征当前指针实参(
        基础信息节点类* 节点,
        void*& 输出指针) noexcept
    {
        输出指针 = nullptr;
        auto* 特征 = dynamic_cast<特征节点类*>(节点);
        return 特征
            && 世界树.读取特征_指针(特征, 输出指针)
            && 输出指针;
    }

    inline bool 写入非I64候选实参(
        基础信息节点类* 实参表宿主,
        const 语素入口节点类* 参数特征类型,
        const 语素入口节点类* 参数值类型,
        void* 参数值,
        const 语素入口节点类* 参数来源,
        I64 生成策略,
        时间戳 now) noexcept
    {
        if (!写入候选实参指针(
                实参表宿主,
                参数特征类型,
                参数值类型,
                参数值,
                参数来源,
                now)) {
            return false;
        }
        if (auto* 候选参数 = 查找子特征(实参表宿主, 参数特征类型)) {
            (void)写入基础节点I64(
                reinterpret_cast<基础信息节点类*>(候选参数),
                特征_实参生成策略(),
                生成策略,
                now);
        }
        return true;
    }

    inline bool 复制固定参数到候选实参(
        基础信息节点类* 固定参数表,
        const 语素入口节点类* 参数特征类型,
        const 语素入口节点类* 参数值类型,
        基础信息节点类* 实参表宿主,
        时间戳 now) noexcept
    {
        auto* 固定参数 = 查找子特征(固定参数表, 参数特征类型);
        void* 指针 = nullptr;
        return 固定参数
            && 读取节点指针实参(reinterpret_cast<基础信息节点类*>(固定参数), 指针)
            && 写入非I64候选实参(
                实参表宿主,
                参数特征类型,
                参数值类型,
                指针,
                值_来源_方法固定参数(),
                1,
                now);
    }

    inline bool 生成非I64候选实参(
        基础信息节点类* 形参节点,
        基础信息节点类* 固定参数表,
        基础信息节点类* 实参表宿主,
        const 语素入口节点类* 参数特征类型,
        const 语素入口节点类* 参数值类型,
        I64 已生成数量,
        时间戳 now) noexcept
    {
        if (复制固定参数到候选实参(
                固定参数表,
                参数特征类型,
                参数值类型,
                实参表宿主,
                now)) {
            return true;
        }

        if (auto* 候选范围 = 查找子特征(形参节点, 特征_候选范围包())) {
            auto* 候选节点 = 取离散候选节点(
                reinterpret_cast<基础信息节点类*>(候选范围),
                已生成数量);
            void* 指针 = nullptr;
            if (读取节点指针实参(候选节点, 指针)) {
                // 非数值值域按离散集合处理中位起步，再向两侧扩展。
                return 写入非I64候选实参(
                    实参表宿主,
                    参数特征类型,
                    参数值类型,
                    指针,
                    值_来源_练习自动生成(),
                    2,
                    now);
            }
        }

        void* 默认指针 = nullptr;
        if (读取特征当前指针实参(形参节点, 默认指针)) {
            return 写入非I64候选实参(
                实参表宿主,
                参数特征类型,
                参数值类型,
                默认指针,
                值_来源_练习自动生成(),
                3,
                now);
        }
        return false;
    }

    inline bool 生成单个候选实参(
        基础信息节点类* 输入包,
        基础信息节点类* 形参节点,
        基础信息节点类* 固定参数表,
        基础信息节点类* 实参表宿主,
        I64 已生成数量,
        时间戳 now,
        I64& 绑定参数数量,
        I64& 缺失参数数量) noexcept
    {
        if (!形参节点 || !实参表宿主) return false;
        const auto* 形参主信息 = 世界树.特征().取特征主信息(
            static_cast<特征节点类*>(形参节点));
        const auto* 参数特征类型 = 形参主信息 ? 形参主信息->类型 : nullptr;
        if (!参数特征类型) return false;

        auto* 候选参数 = 取或创建子特征(实参表宿主, 参数特征类型);
        if (!候选参数) return false;
        auto* 候选宿主 = reinterpret_cast<基础信息节点类*>(候选参数);

        void* 参数值类型 = nullptr;
        if (读取包指针(形参节点, 特征_参数值类型(), 参数值类型)) {
            (void)写入基础节点指针(候选宿主, 特征_参数值类型(), 参数值类型, now);
        }
        const bool 任务已提供 = 复制任务参数到候选实参(
            输入包,
            参数特征类型,
            候选宿主,
            now);
        if (任务已提供) {
            ++绑定参数数量;
            return true;
        }

        const auto* 参数值类型词 = reinterpret_cast<const 语素入口节点类*>(参数值类型);
        if (参数值类型为I64(形参节点)) {
            I64 下界 = 0;
            I64 上界 = 0;
            (void)读取参数允许区间(形参节点, 参数特征类型, 下界, 上界);
            I64 历史值 = 0;
            const bool 有历史值 = 世界树.读取特征_I64(候选参数, 历史值);
            const I64 最大候选数量 = 读取参数最大候选数量(形参节点);
            I64 策略 = 0;
            const I64 生成值 = 生成区间内I64参数(
                已生成数量,
                下界,
                上界,
                有历史值,
                历史值,
                最大候选数量,
                策略);
            (void)世界树.写入特征_I64(候选参数, 生成值, now);
            (void)写入基础节点指针(候选宿主, 特征_参数来源(), 值_来源_练习自动生成(), now);
            (void)写入基础节点I64(候选宿主, 特征_区间下界(), 下界, now);
            (void)写入基础节点I64(候选宿主, 特征_区间上界(), 上界, now);
            (void)写入基础节点I64(候选宿主, 特征_实参生成策略(), 策略, now);
            ++绑定参数数量;
            return true;
        }

        if (生成非I64候选实参(
                形参节点,
                固定参数表,
                实参表宿主,
                参数特征类型,
                参数值类型词,
                已生成数量,
                now)) {
            ++绑定参数数量;
            return true;
        }

        if (形参为必需(形参节点)) {
            ++缺失参数数量;
            (void)写入基础节点指针(候选宿主, 特征_失败原因(), 值_参数条件缺口(), now);
        }
        return !形参为必需(形参节点);
    }

    inline bool 生成形参表候选实参(
        基础信息节点类* 形参表,
        基础信息节点类* 固定参数表,
        基础信息节点类* 输入包,
        基础信息节点类* 实参表宿主,
        I64 已生成数量,
        时间戳 now,
        I64& 绑定参数数量,
        I64& 缺失参数数量) noexcept
    {
        if (!形参表 || !实参表宿主) return false;
        auto* 首子 = 形参表->子 ? static_cast<基础信息节点类*>(形参表->子) : nullptr;
        if (!首子) return false;
        bool 已遍历 = false;
        auto* 当前 = 首子;
        do {
            已遍历 = true;
            (void)生成单个候选实参(
                输入包,
                当前,
                固定参数表,
                实参表宿主,
                已生成数量 + 绑定参数数量 + 缺失参数数量,
                now,
                绑定参数数量,
                缺失参数数量);
            当前 = static_cast<基础信息节点类*>(当前->下);
        } while (当前 && 当前 != 首子);
        return 已遍历;
    }

    inline bool 候选实参已具备值(
        基础信息节点类* 实参表宿主,
        const 语素入口节点类* 参数特征类型) noexcept
    {
        if (!实参表宿主 || !参数特征类型) return false;
        auto* 候选参数 = 查找子特征(实参表宿主, 参数特征类型);
        const auto* 候选主信息 = 候选参数
            ? 世界树.特征().取特征主信息(static_cast<特征节点类*>(候选参数))
            : nullptr;
        return 候选主信息 && 候选主信息->有当前值();
    }

    inline void 复核形参候选实参完整性(
        基础信息节点类* 形参表,
        基础信息节点类* 实参表宿主,
        I64& 绑定参数数量,
        I64& 缺失参数数量) noexcept
    {
        绑定参数数量 = 0;
        缺失参数数量 = 0;
        if (!形参表 || !实参表宿主) return;

        auto* 首子 = 形参表->子 ? static_cast<基础信息节点类*>(形参表->子) : nullptr;
        if (!首子) return;

        auto* 当前 = 首子;
        do {
            const auto* 形参主信息 = 世界树.特征().取特征主信息(
                static_cast<特征节点类*>(当前));
            const auto* 参数特征类型 = 形参主信息 ? 形参主信息->类型 : nullptr;
            const bool 已绑定 = 候选实参已具备值(实参表宿主, 参数特征类型);
            if (已绑定) {
                ++绑定参数数量;
            } else if (形参为必需(当前)) {
                ++缺失参数数量;
            }
            当前 = 当前 ? static_cast<基础信息节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首子);
    }

    inline std::string 语素日志文本(const 语素入口节点类* 词) noexcept
    {
        if (!词) return "空";
        const auto 文本 = 语素集.获取词(词);
        if (文本.empty()) {
            return 词->获取主键();
        }
        return 文本 + "(" + 词->获取主键() + ")";
    }

    inline std::string 指针语素日志文本(void* 指针) noexcept
    {
        return 指针
            ? 语素日志文本(reinterpret_cast<const 语素入口节点类*>(指针))
            : std::string("空");
    }

    inline void 记录练习候选实参明细(
        方法类::节点类* 目标方法,
        基础信息节点类* 形参表,
        基础信息节点类* 实参表宿主,
        基础信息节点类* 输入包,
        I64 绑定参数数量,
        I64 缺失参数数量) noexcept
    {
        if (!形参表 || !实参表宿主) return;
        auto* 首子 =形参表->子 ? static_cast<基础信息节点类*>(形参表->子) : nullptr;
        if (!首子) return;

        std::ostringstream 输出;
        输出 << "自我动作/练习候选实参明细"
            << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
            << " | 输入包=" << 指针日志文本(输入包)
            << " | 形参表=" << 指针日志文本(形参表)
            << " | 实参表=" << 指针日志文本(实参表宿主)
            << " | 绑定参数数量=" << 绑定参数数量
            << " | 缺失参数数量=" << 缺失参数数量;

        I64 序号 = 0;
        auto* 当前 = 首子;
        do {
            const auto* 形参主信息 = 世界树.特征().取特征主信息(
                static_cast<特征节点类*>(当前));
            const auto* 参数特征类型 = 形参主信息 ? 形参主信息->类型 : nullptr;
            auto* 候选参数 = 查找子特征(实参表宿主, 参数特征类型);
            const auto* 候选主信息 = 候选参数
                ? 世界树.特征().取特征主信息(static_cast<特征节点类*>(候选参数))
                : nullptr;
            void* 形参值类型 = nullptr;
            void* 形参来源 = nullptr;
            void* 候选来源 = nullptr;
            (void)读取包指针(当前, 特征_参数值类型(), 形参值类型);
            (void)读取包指针(当前, 特征_参数来源(), 形参来源);
            if (候选参数) {
                (void)读取包指针(
                    reinterpret_cast<基础信息节点类*>(候选参数),
                    特征_参数来源(),
                    候选来源);
            }

            输出 << " | p" << 序号
                << "=" << 语素日志文本(参数特征类型)
                << ",必需=" << (形参为必需(当前) ? 1 : 0)
                << ",已绑定=" << (候选主信息 && 候选主信息->有当前值() ? 1 : 0)
                << ",值类型=" << 指针语素日志文本(形参值类型)
                << ",形参来源=" << 指针语素日志文本(形参来源)
                << ",候选来源=" << 指针语素日志文本(候选来源);

            当前 = 当前 ? static_cast<基础信息节点类*>(当前->下) : nullptr;
            ++序号;
        } while (当前 && 当前 != 首子);

        项目运行日志(输出.str());
    }

    inline bool 复制候选实参到输入包(
        基础信息节点类* 候选实参表,
        基础信息节点类* 输入包,
        时间戳 now) noexcept
    {
        if (!候选实参表 || !输入包) return false;
        auto* 首子 = 候选实参表->子 ? static_cast<基础信息节点类*>(候选实参表->子) : nullptr;
        if (!首子) return false;
        bool 已复制 = false;
        auto* 当前 = 首子;
        do {
            const auto* 主信息 = 世界树.特征().取特征主信息(
                static_cast<特征节点类*>(当前));
            if (主信息 && 主信息->类型 && 查找子特征(当前, 特征_参数值类型())) {
                已复制 = 复制特征当前标量到宿主(
                    static_cast<特征节点类*>(当前),
                    输入包,
                    主信息->类型,
                    now) || 已复制;
            }
            当前 = static_cast<基础信息节点类*>(当前->下);
        } while (当前 && 当前 != 首子);
        return 已复制;
    }

    inline 场景节点类* 构建练习可执行输入场景(
        基础信息节点类* 宿主,
        基础信息节点类* 候选实参表,
        时间戳 now) noexcept
    {
        auto* 可执行场景 = 创建子场景_按特征名(宿主, 特征_可执行输入参数场景());
        if (!可执行场景) return nullptr;
        auto* 输入包 = 取或创建子特征(
            reinterpret_cast<基础信息节点类*>(可执行场景),
            特征_输入条件包());
        if (!输入包) return nullptr;
        return 复制候选实参到输入包(
            候选实参表,
            reinterpret_cast<基础信息节点类*>(输入包),
            now)
            ? 可执行场景
            : nullptr;
    }

    inline 场景节点类* 查找首个条件结果对条件场景(
        方法类::节点类* 方法首节点) noexcept
    {
        if (!方法首节点 || !方法首节点->子) return nullptr;
        auto* 首子 = static_cast<方法类::节点类*>(方法首节点->子);
        auto* 当前 = 首子;
        do {
            if (当前
                && 当前->主信息.节点种类() == 枚举_方法节点种类::方法条件节点
                && 条件节点已有结果子节点_条件结果对归并(当前)) {
                const auto* 条件信息 = 当前->主信息.取条件节点信息();
                if (条件信息 && 条件信息->条件场景.指针) {
                    return 条件信息->条件场景.指针;
                }
            }
            当前 = 当前 ? static_cast<方法类::节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首子);
        return nullptr;
    }

    inline const 语素入口节点类* 读取目标方法形参值类型(
        基础信息节点类* 目标方法宿主,
        const 语素入口节点类* 参数特征类型) noexcept
    {
        if (!目标方法宿主 || !参数特征类型) return nullptr;
        auto* 形参表 = 查找子特征(目标方法宿主, 特征_形参表());
        auto* 形参节点 = 形参表
            ? 查找子特征(reinterpret_cast<基础信息节点类*>(形参表), 参数特征类型)
            : nullptr;
        void* 指针 = nullptr;
        if (形参节点 && 读取包指针(
                reinterpret_cast<基础信息节点类*>(形参节点),
                特征_参数值类型(),
                指针)) {
            return reinterpret_cast<const 语素入口节点类*>(指针);
        }
        return nullptr;
    }

    inline bool 复制条件实参特征到候选实参(
        基础信息节点类* 目标方法宿主,
        特征节点类* 条件实参,
        基础信息节点类* 实参表宿主,
        时间戳 now) noexcept
    {
        if (!条件实参 || !实参表宿主) return false;
        const auto* 主信息 = 世界树.特征().取特征主信息(条件实参);
        const auto* 参数特征类型 = 主信息 ? 主信息->类型 : nullptr;
        if (!参数特征类型 || !主信息->有当前值()) return false;

        if (const auto* 值 = std::get_if<I64>(&主信息->当前值)) {
            return 写入候选实参I64(
                实参表宿主,
                参数特征类型,
                *值,
                值_来源_练习自动生成(),
                now);
        }
        if (const auto* 指针 = std::get_if<指针句柄>(&主信息->当前值);
            指针 && 指针->有效()) {
            const auto* 参数值类型 = 读取目标方法形参值类型(
                目标方法宿主,
                参数特征类型);
            return 写入候选实参指针(
                实参表宿主,
                参数特征类型,
                参数值类型 ? 参数值类型 : 值类型_场景指针(),
                reinterpret_cast<void*>(指针->指针),
                值_来源_练习自动生成(),
                now);
        }
        return false;
    }

    inline I64 复制条件包实参到候选实参表(
        基础信息节点类* 目标方法宿主,
        基础信息节点类* 源包,
        基础信息节点类* 实参表宿主,
        时间戳 now) noexcept
    {
        if (!源包 || !实参表宿主) return 0;
        auto* 首子 = 源包->子 ? static_cast<基础信息节点类*>(源包->子) : nullptr;
        if (!首子) return 0;

        I64 复制数量 = 0;
        auto* 当前 = 首子;
        do {
            if (当前
                && 世界树.特征().取特征主信息(static_cast<特征节点类*>(当前))
                && 复制条件实参特征到候选实参(
                    目标方法宿主,
                    static_cast<特征节点类*>(当前),
                    实参表宿主,
                    now)) {
                ++复制数量;
            }
            当前 = 当前 ? static_cast<基础信息节点类*>(当前->下) : nullptr;
        } while (当前 && 当前 != 首子);
        return 复制数量;
    }

    inline I64 复制条件场景实参到候选实参表(
        基础信息节点类* 目标方法宿主,
        场景节点类* 条件场景,
        基础信息节点类* 实参表宿主,
        时间戳 now) noexcept
    {
        if (!条件场景 || !实参表宿主) return 0;
        auto* 场景宿主 = reinterpret_cast<基础信息节点类*>(条件场景);
        I64 复制数量 = 0;
        if (auto* 输入包 = 查找子特征(场景宿主, 特征_输入条件包())) {
            复制数量 += 复制条件包实参到候选实参表(
                目标方法宿主,
                reinterpret_cast<基础信息节点类*>(输入包),
                实参表宿主,
                now);
        }
        // 兼容早期条件场景直接挂参数特征的情况。
        复制数量 += 复制条件包实参到候选实参表(
            目标方法宿主,
            场景宿主,
            实参表宿主,
            now);
        return 复制数量;
    }

    inline 特征节点类* 生成目标方法候选实参表(
        方法类::节点类* 目标方法,
        基础信息节点类* 目标方法宿主,
        基础信息节点类* 输入包,
        时间戳 now,
        bool 允许复用条件结果对,
        I64& 输出练习模式,
        I64& 输出候选实参数量,
        I64& 输出绑定参数数量,
        I64& 输出缺失参数数量) noexcept
    {
        输出练习模式 = 0;
        输出候选实参数量 = 0;
        输出绑定参数数量 = 0;
        输出缺失参数数量 = 0;
        if (!目标方法宿主) return nullptr;

        auto* 实参表 = 取或创建子特征(目标方法宿主, 特征_候选实参表());
        if (!实参表) return nullptr;

        I64 已生成数量 = 0;
        (void)读取基础节点I64(目标方法宿主, 特征_候选实参数量(), 已生成数量);
        if (已生成数量 < 0) 已生成数量 = 0;

        const bool 任务指定参数 = 练习输入具有指定参数(输入包);
        输出练习模式 = 任务指定参数 ? 1 : 2;
        输出候选实参数量 = 已生成数量 + 1;

        auto* 宿主 = reinterpret_cast<基础信息节点类*>(实参表);
        (void)写入基础节点I64(宿主, 特征_练习模式(), 输出练习模式, now);
        (void)写入基础节点I64(宿主, 特征_实参生成策略(), 任务指定参数 ? 1 : 2, now);
        (void)写入基础节点I64(宿主, 特征_参数生成序号(), 输出候选实参数量, now);
        (void)写入基础节点I64(宿主, 特征_参数新颖性状态(), 1, now);

        if (允许复用条件结果对) {
            if (auto* 条件场景 = 查找首个条件结果对条件场景(目标方法)) {
                const I64 条件实参数量 = 复制条件场景实参到候选实参表(
                    目标方法宿主,
                    条件场景,
                    宿主,
                    now);
                if (条件实参数量 > 0) {
                    输出练习模式 = 3;
                    输出绑定参数数量 = 条件实参数量;
                    输出缺失参数数量 = 0;
                    (void)写入基础节点I64(宿主, 特征_练习模式(), 输出练习模式, now);
                    (void)写入基础节点I64(宿主, 特征_实参生成策略(), 3, now);
                    (void)写入基础节点I64(宿主, 特征_绑定参数数量(), 输出绑定参数数量, now);
                    (void)写入基础节点I64(宿主, 特征_缺失参数数量(), 0, now);
                    (void)写入基础节点I64(宿主, 特征_候选实参数量(), 输出候选实参数量, now);
                    (void)写入基础节点I64(目标方法宿主, 特征_候选实参数量(), 输出候选实参数量, now);
                    记录练习候选实参明细(
                        目标方法,
                        reinterpret_cast<基础信息节点类*>(
                            查找子特征(目标方法宿主, 特征_形参表())),
                        宿主,
                        输入包,
                        输出绑定参数数量,
                        输出缺失参数数量);
                    return 实参表;
                }
            }
        }

        auto* 形参表 = 查找子特征(目标方法宿主, 特征_形参表());
        bool 已按形参生成 = false;
        if (形参表) {
            if (生成形参表候选实参(
                    reinterpret_cast<基础信息节点类*>(形参表),
                    reinterpret_cast<基础信息节点类*>(
                        查找子特征(目标方法宿主, 特征_固定参数表())),
                    输入包,
                    宿主,
                    已生成数量,
                    now,
                    输出绑定参数数量,
                    输出缺失参数数量)) {
                已按形参生成 = true;
                复核形参候选实参完整性(
                    reinterpret_cast<基础信息节点类*>(形参表),
                    宿主,
                    输出绑定参数数量,
                    输出缺失参数数量);
                (void)写入基础节点I64(宿主, 特征_绑定参数数量(), 输出绑定参数数量, now);
                (void)写入基础节点I64(宿主, 特征_缺失参数数量(), 输出缺失参数数量, now);
                (void)写入基础节点I64(宿主, 特征_候选实参数量(), 输出候选实参数量, now);
                (void)写入基础节点I64(目标方法宿主, 特征_候选实参数量(), 输出候选实参数量, now);
                记录练习候选实参明细(
                    目标方法,
                    reinterpret_cast<基础信息节点类*>(形参表),
                    宿主,
                    输入包,
                    输出绑定参数数量,
                    输出缺失参数数量);
                if (输出缺失参数数量 == 0) {
                    return 实参表;
                }
            }
        }

        if (输入包) {
            if (复制输入指针到候选实参(
                    输入包,
                    宿主,
                    特征_目标宿主(),
                    值类型_场景指针(),
                    now)) {
                ++输出绑定参数数量;
            }
            if (复制输入指针到候选实参(
                    输入包,
                    宿主,
                    特征_目标现实场景(),
                    值类型_场景指针(),
                    now)) {
                ++输出绑定参数数量;
            }
            if (复制输入指针到候选实参(
                    输入包,
                    宿主,
                    特征_目标特征类型(),
                    值类型_特征类型指针(),
                    now)) {
                ++输出绑定参数数量;
            }
            if (复制输入指针到候选实参(
                    输入包,
                    宿主,
                    特征_目标抽象特征(),
                    值类型_特征类型指针(),
                    now)) {
                ++输出绑定参数数量;
            }
            if (复制输入指针到候选实参(
                    输入包,
                    宿主,
                    特征_目标特征(),
                    值类型_特征类型指针(),
                    now)) {
                ++输出绑定参数数量;
            }
            if (复制输入指针到候选实参(
                    输入包,
                    宿主,
                    特征_目标实例特征(),
                    值类型_特征类型指针(),
                    now)) {
                ++输出绑定参数数量;
            }
            if (复制输入指针到候选实参(
                    输入包,
                    宿主,
                    特征_目标值类型(),
                    值类型_特征类型指针(),
                    now)) {
                ++输出绑定参数数量;
            }
            if (复制输入指针到候选实参(
                    输入包,
                    宿主,
                    特征_差值约束包(),
                    值类型_场景指针(),
                    now)) {
                ++输出绑定参数数量;
            }
            if (复制输入I64到候选实参(
                    输入包,
                    宿主,
                    特征_目标执行后值(),
                    now)) {
                ++输出绑定参数数量;
            }
        }

        I64 执行差值 = 0;
        if (任务指定参数) {
            I64 方向 = 0;
            if (!输入包
                || (!读取基础节点I64(输入包, 特征_执行差值(), 执行差值)
                    && !读取基础节点I64(输入包, 特征_练习方向(), 方向)
                    && !读取基础节点I64(输入包, 特征_改变方向(), 方向))) {
                执行差值 = 0;
            }
            else if (!读取基础节点I64(输入包, 特征_执行差值(), 执行差值)) {
                执行差值 = 练习方向转差值(方向);
            }
        }
        else {
            执行差值 = 生成无目的未出现I64参数(已生成数量);
        }

        if (写入候选实参I64(
                宿主,
                特征_执行差值(),
                执行差值,
                任务指定参数 ? 值_来源_任务提供参数() : 值_来源_练习自动生成(),
                now)) {
            ++输出绑定参数数量;
        }
        if (已按形参生成 && 形参表) {
            复核形参候选实参完整性(
                reinterpret_cast<基础信息节点类*>(形参表),
                宿主,
                输出绑定参数数量,
                输出缺失参数数量);
        }
        (void)写入基础节点I64(宿主, 特征_候选实参数量(), 输出候选实参数量, now);
        (void)写入基础节点I64(宿主, 特征_绑定参数数量(), 输出绑定参数数量, now);
        (void)写入基础节点I64(宿主, 特征_缺失参数数量(), 输出缺失参数数量, now);
        (void)写入基础节点I64(目标方法宿主, 特征_候选实参数量(), 输出候选实参数量, now);
        记录练习候选实参明细(
            目标方法,
            reinterpret_cast<基础信息节点类*>(形参表),
            宿主,
            输入包,
            输出绑定参数数量,
            输出缺失参数数量);
        return 实参表;
    }

    inline 特征节点类* 生成尝试学习探索实参表(
        方法类::节点类* 目标方法,
        基础信息节点类* 目标方法宿主,
        基础信息节点类* 输入包,
        时间戳 now,
        I64& 输出练习模式,
        I64& 输出候选实参数量,
        I64& 输出绑定参数数量,
        I64& 输出缺失参数数量) noexcept
    {
        // 尝试学习用于探索目标方法头节点声明出的参数空间；
        // 不优先借用既有条件结果对，否则会和练习职责混在一起。
        return 生成目标方法候选实参表(
            目标方法,
            目标方法宿主,
            输入包,
            now,
            false,
            输出练习模式,
            输出候选实参数量,
            输出绑定参数数量,
            输出缺失参数数量);
    }

    inline 特征节点类* 生成练习候选实参表(
        方法类::节点类* 目标方法,
        基础信息节点类* 目标方法宿主,
        基础信息节点类* 输入包,
        时间戳 now,
        I64& 输出练习模式,
        I64& 输出候选实参数量,
        I64& 输出绑定参数数量,
        I64& 输出缺失参数数量) noexcept
    {
        // 练习优先复用目标方法已有条件节点 / 条件结果对，
        // 没有可复用样本时再退回方法头形参空间。
        return 生成目标方法候选实参表(
            目标方法,
            目标方法宿主,
            输入包,
            now,
            true,
            输出练习模式,
            输出候选实参数量,
            输出绑定参数数量,
            输出缺失参数数量);
    }

    inline 场景节点类* 构建目标方法结果能力场景(
        方法类::节点类* 目标方法,
        场景节点类* 备用结果场景,
        时间戳 now) noexcept
    {
        if (!目标方法) return 备用结果场景;
        auto* 目标方法存在 = 方法虚拟存在(目标方法, now);
        auto* 结果场景 = 方法类::取或创建_方法结果场景(
            目标方法,
            目标方法存在,
            now);
        if (!结果场景) {
            return 备用结果场景;
        }

        auto* 目标主体 = reinterpret_cast<基础信息节点类*>(目标方法存在);
        auto* 场景宿主 = reinterpret_cast<基础信息节点类*>(结果场景);
        auto* 首信息 = 目标方法->主信息.是否方法首节点()
            ? 目标方法->主信息.取首节点信息()
            : nullptr;
        if (!目标主体 || !首信息) {
            return 结果场景;
        }

        bool 已写结果状态 = false;
        for (const auto& 结果项 : 首信息->能力.结果包.结果项集) {
            if (结果项.关系目标特征类型由参数决定()) {
                continue;
            }
            const auto* 目标特征类型 = 结果项.关系目标特征类型();
            const 特征值* 目标值 = 结果项.关系模板.有目标值
                ? &结果项.关系模板.目标值
                : &结果项.目标值;
            if (!目标特征类型
                || !目标值
                || std::holds_alternative<std::monostate>(*目标值)) {
                continue;
            }
            auto* 目标特征 = 世界树.确保特征(目标主体, 目标特征类型);
            if (!目标特征) {
                continue;
            }
            auto* 结果状态 = 世界树.状态().创建内部状态(
                结果场景,
                目标主体,
                目标特征,
                *目标值,
                枚举_存在状态事件::变化,
                true,
                now);
            if (结果状态) {
                (void)写入基础节点指针(场景宿主, 目标特征类型, 结果状态, now);
                已写结果状态 = true;
            }
        }

        return 已写结果状态 ? 结果场景 : (备用结果场景 ? 备用结果场景 : 结果场景);
    }

    inline bool 确认尝试学习规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标方法头节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源因果信息(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源观察动态(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_条件结果对(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_动作主体(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标方法头节点(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_来源因果信息(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_来源观察动态(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_条件结果对(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_动作主体(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法补齐类别()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法补齐状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法补齐尝试结果()) && ok;
            ok = 取或创建子特征(宿主, 特征_内化状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作主体替换状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法补齐结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标方法头节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_来源因果信息()) && ok;
            ok = 取或创建子特征(宿主, 特征_来源观察动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_条件结果对()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法动作状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法动作数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法条件结果配对状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法条件结果对数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法条件节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法结果节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选方法能力包()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选形参包()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选结果包()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作主体()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_方法动作数量(),
            1) && ok;
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_方法动作状态(),
            1) && ok;
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_方法条件结果对数量(),
            1) && ok;
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_方法条件结果配对状态(),
            1) && ok;
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_方法状态(),
            方法状态_可用()) && ok;
        // 尝试学习用于把已有内部因果样本内化为目标方法动作和条件结果对；
        // 它可以承接内部方法动作缺口，但不得凭空伪造动作。
        return ok;
    }

    inline bool 确认定向学习规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标方法头节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标特征类型(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标抽象特征(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标特征(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源因果信息(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源观察动态(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_输入条件包(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标方法头节点(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标特征类型(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标抽象特征(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标特征(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_来源因果信息(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_来源观察动态(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法补齐类别()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法补齐状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选动作节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_预测状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法补齐结构状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标方法头节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标抽象特征()) && ok;
            ok = 取或创建子特征(宿主, 特征_来源观察动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选方法能力包()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选形参包()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选结果包()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        // 定向学习只形成外界方向材料，不声明内部方法动作或可执行入口补齐结果能力。
        return ok;
    }

    inline bool 确认提交任务状态变化规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;

        // 提交任务状态变化是唯一改变“任务状态”特征的任务域提交方法。
        // 过程方法只给建议状态；这里读取建议状态并落任务状态动作动态。
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_建议任务状态(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_建议任务阶段(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_任务信息节点(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_建议任务状态(), now) && ok;

        if (auto* 回执根 = 方法规格根(方法首节点, 特征_执行回执表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(回执根);
            ok = 取或创建子特征(宿主, 特征_任务状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_建议任务状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_建议任务阶段()) && ok;
            ok = 取或创建子特征(宿主, 特征_任务状态动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_当前方法运行存在()) && ok;
        }
        return ok;
    }

    inline bool 确认提交领取OR组结算令牌规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;

        // OR组令牌是逻辑组织提交入口：令牌记录落在独立存在上，不写入需求主信息。
        ok = 声明形参规格(方法首节点, 特征_OR组节点(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_OR组领取路径(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源需求(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源动作动态(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_OR组领取路径(), now) && ok;
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_OR组结算令牌状态(),
            OR组结算令牌状态_已领取) && ok;

        if (auto* 回执根 = 方法规格根(方法首节点, 特征_执行回执表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(回执根);
            ok = 取或创建子特征(宿主, 特征_OR组节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_OR组领取路径()) && ok;
            ok = 取或创建子特征(宿主, 特征_OR组结算令牌状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_OR组令牌提交状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_OR组令牌重复领取状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_需求状态动作动态()) && ok;
        }
        return ok;
    }

    inline bool 确认结算叶子任务价值规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;

        // 结算叶子任务价值只处理叶子任务完成后的真实价值入账。
        // 输入以任务完成事实、任务自身权重折算额度、完成度和实际结果状态为准；
        // 来源动作 / 满足证据只作为因果补充，不作为本方法能否入账的硬前提。
        // 服务对象改善、验证和归因证据在本方法内部裁决服务增量，线程侧只打包证据。
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_任务虚拟存在(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_任务状态(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源需求(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_安全结算增量(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_服务结算增量(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_任务完成度百万分比(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_实际结果状态(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_服务对象外在特征状态变化数(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_服务对象外在特征状态改善证据(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_服务结果可验证(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_服务结果可归因于自我动作(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源动作动态(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_满足证据(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_任务信息节点(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_任务虚拟存在(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_任务状态(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_来源需求(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_安全结算增量(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_服务结算增量(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_任务完成度百万分比(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_实际结果状态(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_服务对象外在特征状态变化数(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_服务对象外在特征状态改善证据(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_服务结果可验证(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_服务结果可归因于自我动作(), now) && ok;
        ok = 确保方法结果能力方向(方法首节点, 特征_安全值(), 枚举_结果变化方向::增加) && ok;
        ok = 确保方法结果能力方向(方法首节点, 特征_服务值(), 枚举_结果变化方向::增加) && ok;

        if (auto* 回执根 = 方法规格根(方法首节点, 特征_执行回执表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(回执根);
            ok = 取或创建子特征(宿主, 特征_任务信息节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_任务虚拟存在()) && ok;
            ok = 取或创建子特征(宿主, 特征_任务状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_来源需求()) && ok;
            ok = 取或创建子特征(宿主, 特征_任务完成度百万分比()) && ok;
            ok = 取或创建子特征(宿主, 特征_实际结果状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_服务对象外在特征状态变化数()) && ok;
            ok = 取或创建子特征(宿主, 特征_服务对象外在特征状态改善证据()) && ok;
            ok = 取或创建子特征(宿主, 特征_服务结果可验证()) && ok;
            ok = 取或创建子特征(宿主, 特征_服务结果可归因于自我动作()) && ok;
            ok = 取或创建子特征(宿主, 特征_服务结算证据通过()) && ok;
            ok = 取或创建子特征(宿主, 特征_旧安全值()) && ok;
            ok = 取或创建子特征(宿主, 特征_新安全值()) && ok;
            ok = 取或创建子特征(宿主, 特征_旧服务值()) && ok;
            ok = 取或创建子特征(宿主, 特征_新服务值()) && ok;
            ok = 取或创建子特征(宿主, 特征_安全结算增量()) && ok;
            ok = 取或创建子特征(宿主, 特征_服务结算增量()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
        }
        return ok;
    }

    inline bool 确认筹办协作回执槽(
        方法类::节点类* 方法首节点,
        时间戳 now) noexcept
    {
        bool ok = true;
        if (auto* 回执根 = 方法规格根(方法首节点, 特征_执行回执表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(回执根);
            ok = 取或创建子特征(宿主, 特征_方法执行情况()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法运行结果状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标方法头节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选方法数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_可执行输入参数场景()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法条件节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_绑定参数数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_缺失参数数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_等待原因()) && ok;
            ok = 取或创建子特征(宿主, 特征_派生需求提示()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        return ok;
    }

    inline bool 确认挂起任务等待子需求规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源需求(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_等待原因(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_派生需求提示(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 确认筹办协作回执槽(方法首节点, now) && ok;
        return ok;
    }

    inline bool 确认唤醒任务重筹办规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_等待原因(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 确认筹办协作回执槽(方法首节点, now) && ok;
        return ok;
    }

    inline bool 确认派生需求入树规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_来源需求(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标特征类型(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_派生需求提示(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 确认筹办协作回执槽(方法首节点, now) && ok;
        return ok;
    }

    inline bool 确认登记任务发起意图规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_来源需求(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_候选方法数量(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 确认筹办协作回执槽(方法首节点, now) && ok;
        return ok;
    }

    inline bool 确认承接需求创建任务规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_来源需求(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 确认筹办协作回执槽(方法首节点, now) && ok;
        return ok;
    }

    inline bool 确认建立条件结果对规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_目标方法头节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_方法条件节点(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_方法结果节点(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_条件结果对(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 确认筹办协作回执槽(方法首节点, now) && ok;
        // 方法域提交动作只供学习动作显式调用；结果能力由学习动作声明，避免普通任务筹办直接选中提交口。
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_目标方法头节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法条件结果配对状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_条件结果对()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        return ok;
    }

    inline bool 确认提交方法状态变化规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_目标方法头节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_方法状态(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 确认筹办协作回执槽(方法首节点, now) && ok;
        // 方法状态提交是方法域落账口，不参与普通候选召回；尝试学习 / 练习负责声明可达成的学习结果。
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_目标方法头节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        return ok;
    }

    inline bool 确认提交方法可执行入口规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;

        // 提交方法可执行入口只提交“目标方法绑定哪个可执行本能动作”。
        // 目标方法是否可用仍由条件结果对和方法状态提交链继续判定。
        ok = 声明形参规格(方法首节点, 特征_目标方法头节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_选中自我能力值(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明条件特征(方法首节点, 特征_目标方法头节点(), now) && ok;
        ok = 声明条件特征(方法首节点, 特征_选中自我能力值(), now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法动作状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法可执行入口数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标方法头节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_选中自我能力值()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
            ok = 取或创建子特征(宿主, 特征_失败原因()) && ok;
        }
        if (auto* 首信息 = 方法首节点->主信息.取首节点信息()) {
            首信息->能力.可被方法查找命中 = true;
            首信息->能力.是否根写入原语 = false;
            bool 已有方法动作状态结果 = false;
            for (const auto& 结果项 : 首信息->能力.结果包.结果项集) {
                if (!结果项.关系目标特征类型由参数决定()
                    && 语素入口主键相同(结果项.关系目标特征类型(), 特征_方法动作状态())) {
                    已有方法动作状态结果 = true;
                    break;
                }
            }
            if (!已有方法动作状态结果) {
                结构_方法结果项 结果项{};
                结果项.特征类型 = 特征_方法动作状态();
                结果项.方向 = 枚举_结果变化方向::已形成;
                结果项.目标值 = 特征值{ static_cast<I64>(1) };
                结果项.关系模板.目标特征类型 = 特征_方法动作状态();
                结果项.关系模板.允许方向 = 方法结果变化方向允许方向(结果项.方向);
                结果项.关系模板.有目标值 = true;
                结果项.关系模板.目标值 = 结果项.目标值;
                首信息->能力.结果包.结果项集.push_back(std::move(结果项));
            }
        }
        ok = 确保方法结果能力I64(
            方法首节点,
            特征_方法可执行入口数量(),
            1) && ok;
        ok = 确认筹办协作回执槽(方法首节点, now) && ok;
        return ok;
    }

    inline bool 确认双目相机控制规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now,
        bool 目标为可用) noexcept
    {
        return 外部模块::确认双目相机控制规格(方法首节点, 方法存在, now, 目标为可用);
    }

    inline bool 确认双目相机检查运行状态规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认双目相机检查运行状态规格(方法首节点, 方法存在, now);
    }

    inline bool 确认双目相机刷新当前观察场景规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认双目相机刷新当前观察场景规格(方法首节点, 方法存在, now);
    }

    inline bool 确认双目相机发现候选存在规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认双目相机发现候选存在规格(方法首节点, 方法存在, now);
    }

    inline bool 确认双目相机取得当前观察帧规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认双目相机取得当前观察帧规格(方法首节点, 方法存在, now);
    }

    inline bool 确认双目相机取得当前观察特征帧规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认双目相机取得当前观察特征帧规格(方法首节点, 方法存在, now);
    }

    inline bool 确认双目相机建立当前帧像素特征索引规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认双目相机建立当前帧像素特征索引规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我建立观察帧存在规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我建立观察帧存在规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我建立观察帧到自我场景接口规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我建立观察帧到自我场景接口规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我建立像素观察事实索引规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我建立像素观察事实索引规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我承接像素特征值规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我承接像素特征值规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我校验观察事实完备性规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我校验观察事实完备性规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我评估观察帧质量规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我评估观察帧质量规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我诊断观察质量缺口并派生补观察需求规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我诊断观察质量缺口并派生补观察需求规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我判断基础观察事实可用性规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我判断基础观察事实可用性规格(
            方法首节点,
            方法存在,
            now);
        // 该方法是确定性的自我观察摘要判定；抽象条件结果对只描述方法规格，
        // 实际是否写入“可用”仍由运行时观察证据和失败条件共同决定。
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认安全评估当前场景安全性规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认安全评估当前场景安全性规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我读取自我关键特征变化规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我读取自我关键特征变化规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我建立最小自我边界参照规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我建立最小自我边界参照规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我生成存在自我相对安全特征规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我生成存在自我相对安全特征规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认安全构造场景影响因果候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认安全构造场景影响因果候选规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我明确当前场景持续观察需求规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我明确当前场景持续观察需求规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认安全持续观察当前场景规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认安全持续观察当前场景规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我明确负向影响处置需求规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我明确负向影响处置需求规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认安全执行降级保护规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认安全执行降级保护规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我验证外设观察像素簇候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我验证外设观察像素簇候选规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我识别外设观察材料规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我识别外设观察材料规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我扫描已识别区域规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我扫描已识别区域规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我跟踪指定存在规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我跟踪指定存在规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我确认外设观察存在候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我确认外设观察存在候选规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认提交确认观察存在事实规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认提交确认观察存在事实规格(方法首节点, 方法存在, now);
    }

    inline bool 确认提交观察存在特征值变化事实规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认提交观察存在特征值变化事实规格(方法首节点, 方法存在, now);
    }

    inline bool 确认提交指定存在跟踪事实规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认提交指定存在跟踪事实规格(方法首节点, 方法存在, now);
    }

    inline bool 确认提交当前观察范围可观测单位存在对应事实规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认提交当前观察范围可观测单位存在对应事实规格(方法首节点, 方法存在, now);
    }

    inline bool 确认提交观察存在发现事实规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认提交观察存在发现事实规格(方法首节点, 方法存在, now);
    }

    inline bool 确认提交风险安全场景影响部分状态变化规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认提交风险安全场景影响部分状态变化规格(方法首节点, 方法存在, now);
    }

    inline bool 确认安全搜索安全因果因素证据规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认安全搜索安全因果因素证据规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认提交安全因果因素无负证据默认满足规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认提交安全因果因素无负证据默认满足规格(方法首节点, 方法存在, now);
    }

    inline bool 确认提交安全因果因素证据评估值规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认提交安全因果因素证据评估值规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我按已确认轮廓分区当前帧规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        const bool ok = 外部模块::确认自我按已确认轮廓分区当前帧规格(
            方法首节点,
            方法存在,
            now);
        return 确保本能方法抽象条件结果对(方法首节点, now) && ok;
    }

    inline bool 确认自我调整相机参数并重采样规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我调整相机参数并重采样规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我按ROI重算观察质量规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我按ROI重算观察质量规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我执行多帧重采样并择优规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我执行多帧重采样并择优规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我生成低置信补偿观察候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我生成低置信补偿观察候选规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我调整外设观察姿态规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我调整外设观察姿态规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我调整观察位置规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我调整观察位置规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我按像素坐标回查观察事实规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我按像素坐标回查观察事实规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我提取空间候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我提取空间候选规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我判断空间候选相对独立性规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我判断空间候选相对独立性规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我生成空间候选观察队列规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我生成空间候选观察队列规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我观察单个空间候选并组合存在假设规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我观察单个空间候选并组合存在假设规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我用空间范围投影覆盖原帧验证像素归属规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我用空间范围投影覆盖原帧验证像素归属规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我确认观察存在并更新帧解释状态规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我确认观察存在并更新帧解释状态规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我确保存在内部世界规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我确保存在内部世界规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我建立存在内外场景接口规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我建立存在内外场景接口规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我将存在局部观察切片映射到内部世界规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我将存在局部观察切片映射到内部世界规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我在内部世界提取子候选规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我在内部世界提取子候选规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我确认内部世界子存在规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我确认内部世界子存在规格(方法首节点, 方法存在, now);
    }

    inline bool 确认提交确认内部世界子存在事实规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认提交确认内部世界子存在事实规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我递归细分存在内部世界规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我递归细分存在内部世界规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我匹配当前观察存在与历史观察存在规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我匹配当前观察存在与历史观察存在规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我比较存在特征变化规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我比较存在特征变化规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我生成观察变化事件规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我生成观察变化事件规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我导出当前场景识别快照规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我导出当前场景识别快照规格(方法首节点, 方法存在, now);
    }

    inline bool 确认自我导出OpenGL复现数据规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        return 外部模块::确认自我导出OpenGL复现数据规格(方法首节点, 方法存在, now);
    }

    inline bool 确认归并子结果到父任务筹办规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_输出结果场景(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 确认筹办协作回执槽(方法首节点, now) && ok;
        return ok;
    }

    inline bool 确认检查父任务是否可重筹办规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_任务信息节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源需求(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前方法运行存在(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 确认筹办协作回执槽(方法首节点, now) && ok;
        return ok;
    }

    // 差异点集中到专属规格分发，避免每个本能函数手写规格确认链。
    inline bool 确认本能方法专属规格(
        枚举_本能方法ID 本能ID,
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        switch (本能ID) {
        case 枚举_本能方法ID::自我_尝试学习:
            return 确认尝试学习规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_定向学习:
            return 确认定向学习规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_练习:
            return 确认练习规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_提交任务状态变化:
            return 确认提交任务状态变化规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_结算叶子任务价值:
            return 确认结算叶子任务价值规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_提交方法可执行入口:
            return 确认提交方法可执行入口规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_建立条件结果对:
            return 确认建立条件结果对规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_提交方法状态变化:
            return 确认提交方法状态变化规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_建立观察帧存在:
            return 确认自我建立观察帧存在规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_建立观察帧到自我场景接口:
            return 确认自我建立观察帧到自我场景接口规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_建立像素观察事实索引:
            return 确认自我建立像素观察事实索引规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_承接像素特征值:
            return 确认自我承接像素特征值规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_校验观察事实完备性:
            return 确认自我校验观察事实完备性规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_评估观察帧质量:
            return 确认自我评估观察帧质量规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_诊断观察质量缺口并派生补观察需求:
            return 确认自我诊断观察质量缺口并派生补观察需求规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_判断基础观察事实可用性:
            return 确认自我判断基础观察事实可用性规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::安全_评估当前场景安全性:
            return 确认安全评估当前场景安全性规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_读取自我关键特征变化:
            return 确认自我读取自我关键特征变化规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_建立最小自我边界参照:
            return 确认自我建立最小自我边界参照规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_生成存在自我相对安全特征:
            return 确认自我生成存在自我相对安全特征规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::安全_构造场景影响因果候选:
            return 确认安全构造场景影响因果候选规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_明确当前场景持续观察需求:
            return 确认自我明确当前场景持续观察需求规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::安全_持续观察当前场景:
            return 确认安全持续观察当前场景规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_明确负向影响处置需求:
            return 确认自我明确负向影响处置需求规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::安全_执行降级保护:
            return 确认安全执行降级保护规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_验证外设观察像素簇候选:
            return 确认自我验证外设观察像素簇候选规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_识别外设观察材料:
            return 确认自我识别外设观察材料规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_扫描已识别区域:
            return 确认自我扫描已识别区域规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_跟踪指定存在:
            return 确认自我跟踪指定存在规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_确认外设观察存在候选:
            return 确认自我确认外设观察存在候选规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::提交_确认观察存在事实:
            return 确认提交确认观察存在事实规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::提交_观察存在特征值变化事实:
            return 确认提交观察存在特征值变化事实规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::提交_指定存在跟踪事实:
            return 确认提交指定存在跟踪事实规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::提交_当前观察范围可观测单位存在对应事实:
            return 确认提交当前观察范围可观测单位存在对应事实规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::提交_观察存在发现事实:
            return 确认提交观察存在发现事实规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::提交_风险安全场景影响部分状态变化:
            return 确认提交风险安全场景影响部分状态变化规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::安全_搜索安全因果因素证据:
            return 确认安全搜索安全因果因素证据规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::提交_安全因果因素无负证据默认满足:
            return 确认提交安全因果因素无负证据默认满足规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::提交_安全因果因素证据评估值:
            return 确认提交安全因果因素证据评估值规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::提交_领取OR组结算令牌:
            return 确认提交领取OR组结算令牌规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_按已确认轮廓分区当前帧:
            return 确认自我按已确认轮廓分区当前帧规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_调整相机参数并重采样:
            return 确认自我调整相机参数并重采样规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_按ROI重算观察质量:
            return 确认自我按ROI重算观察质量规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_执行多帧重采样并择优:
            return 确认自我执行多帧重采样并择优规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_生成低置信补偿观察候选:
            return 确认自我生成低置信补偿观察候选规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_调整外设观察姿态:
            return 确认自我调整外设观察姿态规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_调整观察位置:
            return 确认自我调整观察位置规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_按像素坐标回查观察事实:
            return 确认自我按像素坐标回查观察事实规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_提取空间候选:
            return 确认自我提取空间候选规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_判断空间候选相对独立性:
            return 确认自我判断空间候选相对独立性规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_生成空间候选观察队列:
            return 确认自我生成空间候选观察队列规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_观察单个空间候选并组合存在假设:
            return 确认自我观察单个空间候选并组合存在假设规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_用空间范围投影覆盖原帧验证像素归属:
            return 确认自我用空间范围投影覆盖原帧验证像素归属规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_确保存在内部世界:
            return 确认自我确保存在内部世界规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_建立存在内外场景接口:
            return 确认自我建立存在内外场景接口规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_将存在局部观察切片映射到内部世界:
            return 确认自我将存在局部观察切片映射到内部世界规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_在内部世界提取子候选:
            return 确认自我在内部世界提取子候选规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_确认内部世界子存在:
            return 确认自我确认内部世界子存在规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::提交_确认内部世界子存在事实:
            return 确认提交确认内部世界子存在事实规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_递归细分存在内部世界:
            return 确认自我递归细分存在内部世界规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_匹配当前观察存在与历史观察存在:
            return 确认自我匹配当前观察存在与历史观察存在规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_比较存在特征变化:
            return 确认自我比较存在特征变化规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_生成观察变化事件:
            return 确认自我生成观察变化事件规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_导出当前场景识别快照:
            return 确认自我导出当前场景识别快照规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::自我_导出OpenGL复现数据:
            return 确认自我导出OpenGL复现数据规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::双目相机_使达到可用状态:
            return 确认双目相机控制规格(方法首节点, 方法存在, now, true);
        case 枚举_本能方法ID::双目相机_释放:
            return 确认双目相机控制规格(方法首节点, 方法存在, now, false);
        case 枚举_本能方法ID::双目相机_检查运行状态:
            return 确认双目相机检查运行状态规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::双目相机_刷新当前观察场景:
            return 确认双目相机刷新当前观察场景规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::双目相机_发现候选存在:
            return 确认双目相机发现候选存在规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::双目相机_取得当前观察帧:
            return 确认双目相机取得当前观察帧规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::双目相机_建立当前帧像素特征索引:
            return 确认双目相机建立当前帧像素特征索引规格(方法首节点, 方法存在, now);
        case 枚举_本能方法ID::双目相机_取得当前观察特征帧:
            return 确认双目相机取得当前观察特征帧规格(方法首节点, 方法存在, now);
        default:
            return false;
        }
    }

    inline std::size_t 确认默认本能方法专属规格集(
        自我类& 自我对象,
        时间戳 now = 结构体_时间戳::当前_微秒()) noexcept
    {
        static std::atomic<std::uintptr_t> s_最近确认自我存在{0};
        static std::atomic<std::size_t> s_最近确认数量{0};

        if (now == 0) {
            now = 结构体_时间戳::当前_微秒();
        }

        auto* 自我存在 = 自我对象.确保自我存在();
        if (!自我存在) {
            项目运行错误日志("默认本能方法规格确认失败 | 自我存在=空");
            return 0;
        }

        const auto 默认本能方法集 = 本能方法类::枚举默认自我本能方法ID();
        const auto 自我存在指针 = reinterpret_cast<std::uintptr_t>(自我存在);
        const auto 最近自我存在 = s_最近确认自我存在.load(std::memory_order_acquire);
        const auto 最近确认数量 = s_最近确认数量.load(std::memory_order_acquire);
        if (最近自我存在 == 自我存在指针
            && 最近确认数量 >= 默认本能方法集.size()
            && 本能方法类::默认自我本能方法已注册(自我对象)) {
            项目运行日志(
                "默认本能方法规格确认快路径"
                " | 已确认=" + std::to_string(最近确认数量)
                + " | 默认数量=" + std::to_string(默认本能方法集.size()));
            return 最近确认数量;
        }

        const auto 确认开始 = std::chrono::steady_clock::now();
        std::size_t 已确认数量 = 0;
        for (const auto 本能ID : 默认本能方法集) {
            auto* 方法首节点 = 本能方法类::查找或创建本能方法首节点(
                自我存在,
                本能ID,
                now);
            auto* 方法存在 = 方法首节点
                ? 方法虚拟存在服务类::取或创建方法虚拟存在(
                    方法首节点,
                    自我存在,
                    now)
                : nullptr;
            if (!方法首节点 || !方法存在) {
                continue;
            }

            if (确认本能方法专属规格(本能ID, 方法首节点, 方法存在, now)) {
                (void)方法虚拟存在服务类::同步方法节点到虚拟存在(
                    方法首节点,
                    自我存在,
                    now);
                ++已确认数量;
            }
        }

        s_最近确认自我存在.store(自我存在指针, std::memory_order_release);
        s_最近确认数量.store(已确认数量, std::memory_order_release);
        const auto 确认耗时 = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - 确认开始).count();
        项目运行日志(
            "默认本能方法规格确认"
            " | 已确认=" + std::to_string(已确认数量)
            + " | 默认数量=" + std::to_string(默认本能方法集.size())
            + " | 耗时ms=" + std::to_string(确认耗时));
        return 已确认数量;
    }

    struct 结构_方法域I64提交结果 {
        bool 成功 = false;
        I64 旧值 = 0;
        I64 新值 = 0;
        状态节点类* 实际状态 = nullptr;
        动态节点类* 动态 = nullptr;
        const 语素入口节点类* 失败原因 = nullptr;
    };

    struct 结构_条件结果对提交结果 {
        bool 成功 = false;
        结构_方法条件结果节点 节点{};
        动态节点类* 动态 = nullptr;
        const 语素入口节点类* 失败原因 = nullptr;
    };

    inline 方法类::节点类* 确保方法域提交本能方法(
        方法类::节点类* 方法树锚点,
        枚举_本能方法ID 本能ID,
        时间戳 now) noexcept
    {
        auto* 提交方法 = 确保本能方法(方法树锚点, 本能ID, now);
        auto* 提交方法存在 = 提交方法 ? 方法虚拟存在(提交方法, now) : nullptr;
        if (提交方法 && 提交方法存在) {
            (void)确认本能方法专属规格(本能ID, 提交方法, 提交方法存在, now);
            (void)方法虚拟存在服务类::同步方法节点到虚拟存在(
                提交方法,
                nullptr,
                now);
        }
        return 提交方法;
    }

    inline 场景节点类* 确保方法域提交输出场景(
        方法类::节点类* 提交方法首节点,
        场景节点类*& 输出结果场景,
        时间戳 now) noexcept
    {
        if (!输出结果场景 && 提交方法首节点) {
            输出结果场景 = 方法类::取或创建_方法结果场景(
                提交方法首节点,
                nullptr,
                now);
        }
        if (!输出结果场景) {
            输出结果场景 = 世界树.取或创建自我现实场景();
        }
        return 输出结果场景;
    }

    inline void 记录并提示方法域提交动态缺失(
        const char* 动作名,
        方法类::节点类* 提交方法首节点,
        方法类::节点类* 目标方法,
        const 语素入口节点类* 特征类型,
        I64 旧值,
        I64 新值) noexcept
    {
        std::ostringstream 输出;
        输出 << "方法域提交/动作动态创建逻辑错误"
            << " | 动作=" << (动作名 ? 动作名 : "空")
            << " | 提交方法=" << (提交方法首节点 ? 提交方法首节点->获取主键() : std::string("空"))
            << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
            << " | 特征=" << (特征类型 ? 特征类型->获取主键() : std::string("空"))
            << " | 旧值=" << 旧值
            << " | 新值=" << 新值
            << " | 处置=状态已写但动作动态为空，记录错误并返回失败";
        const auto 文本 = 输出.str();
        项目运行错误日志(文本);

        static std::atomic_bool s_同类错误已弹窗{ false };
        if (!s_同类错误已弹窗.exchange(true)) {
            项目弹窗错误提示("鱼巢 - 方法域动作动态逻辑错误", 文本);
        } else {
            项目运行错误日志(
                "方法域提交/动作动态创建逻辑错误 | 同类弹窗已提示，本次仅记录错误日志");
        }
    }

    inline 结构_方法域I64提交结果 提交目标方法I64特征_方法域动作(
        方法类::节点类* 提交方法首节点,
        存在节点类* 提交运行存在,
        方法类::节点类* 目标方法,
        const 语素入口节点类* 特征类型,
        I64 新值,
        场景节点类* 输入参数场景,
        场景节点类*& 输出结果场景,
        时间戳 now) noexcept
    {
        结构_方法域I64提交结果 结果{};
        结果.新值 = 新值;
        auto* 目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        auto* 目标方法宿主 = reinterpret_cast<基础信息节点类*>(目标方法存在);
        auto* 状态特征 = 目标方法宿主 && 特征类型
            ? 世界树.取或创建子特征_按类型(目标方法宿主, 特征类型)
            : nullptr;

        if (!提交方法首节点) {
            结果.失败原因 = 值_自身方法头节点缺失();
        } else if (!目标方法) {
            结果.失败原因 = 值_目标方法头节点缺失();
        } else if (!目标方法存在 || !目标方法宿主) {
            结果.失败原因 = 值_方法虚拟存在缺失();
        } else if (!特征类型 || !状态特征) {
            结果.失败原因 = 值_方法配对结果创建失败();
        }
        if (结果.失败原因) {
            return 结果;
        }

        (void)读取基础节点I64(目标方法宿主, 特征类型, 结果.旧值);
        const bool 已写目标值 = 写入目标方法I64特征(
            目标方法,
            特征类型,
            新值,
            now);
        auto* 状态场景 = 已写目标值
            ? 确保方法域提交输出场景(提交方法首节点, 输出结果场景, now)
            : nullptr;
        if (!已写目标值 || !状态场景) {
            结果.失败原因 = 值_方法配对结果创建失败();
            return 结果;
        }

        (void)世界树.状态().创建内部状态(
            状态场景,
            目标方法宿主,
            状态特征,
            结果.旧值,
            枚举_存在状态事件::创建,
            false,
            now);
        结果.实际状态 = 世界树.状态().创建内部状态(
            状态场景,
            目标方法宿主,
            状态特征,
            新值,
            枚举_存在状态事件::变化,
            结果.旧值 != 新值,
            now);
        if (结果.实际状态) {
            (void)世界树.写入特征_指针(
                reinterpret_cast<基础信息节点类*>(状态场景),
                特征类型,
                结果.实际状态,
                now);
        }
        (void)世界树.写入特征_I64(
            reinterpret_cast<基础信息节点类*>(状态场景),
            特征_需求满足情况(),
            1,
            now);

        结果.动态 = 世界树.动态().创建方法动作动态(
            状态场景,
            目标方法宿主,
            状态特征,
            reinterpret_cast<方法节点类*>(提交方法首节点),
            词_被动动作(),
            now,
            now,
            true,
            0,
            枚举_动作事件相位::完成,
            输入参数场景,
            输出结果场景);
        if (!结果.动态) {
            结果.失败原因 = 值_方法域动作动态创建失败();
            记录并提示方法域提交动态缺失(
                提交方法首节点->主信息.首节点信息().动作名
                    ? 提交方法首节点->主信息.首节点信息().动作名->获取主键().c_str()
                    : "空",
                提交方法首节点,
                目标方法,
                特征类型,
                结果.旧值,
                新值);
            return 结果;
        }

        if (语素入口主键相同(特征类型, 特征_方法状态())) {
            std::ostringstream 输出;
            输出 << "方法域提交/方法状态被动派生动态"
                << " | 提交方法=" << (提交方法首节点 ? 提交方法首节点->获取主键() : std::string("空"))
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 旧状态=" << 结果.旧值
                << " | 新状态=" << 新值
                << " | 动作语义=被动动作"
                << " | 动态=" << 指针日志文本(结果.动态);
            项目运行日志(输出.str());
        }

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入方法执行情况(宿主, 值_执行成功(), now);
            (void)写入基础节点I64(宿主, 特征类型, 新值, now);
            (void)写入基础节点指针(宿主, 特征_目标方法头节点(), 目标方法, now);
            (void)写入基础节点指针(宿主, 特征_动作动态(), 结果.动态, now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(提交运行存在));
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景));
        if (auto* 提交方法存在 = 方法虚拟存在(提交方法首节点, now)) {
            写结果(reinterpret_cast<基础信息节点类*>(提交方法存在));
        }
        写结果(目标方法宿主);

        结果.成功 = true;
        return 结果;
    }

    inline 结构_条件结果对提交结果 提交目标方法条件结果对_方法域动作(
        方法类::节点类* 提交方法首节点,
        存在节点类* 提交运行存在,
        方法类::节点类* 目标方法,
        场景节点类* 条件场景,
        场景节点类* 结果场景,
        场景节点类* 输入参数场景,
        场景节点类*& 输出结果场景,
        时间戳 now) noexcept
    {
        结构_条件结果对提交结果 结果{};
        if (!提交方法首节点) {
            结果.失败原因 = 值_自身方法头节点缺失();
            return 结果;
        }
        if (!目标方法) {
            结果.失败原因 = 值_目标方法头节点缺失();
            return 结果;
        }

        结果.节点 = 构建方法条件和结果节点(
            目标方法,
            条件场景,
            结果场景,
            now);
        if (!结果.节点.条件节点 || !结果.节点.结果节点) {
            结果.失败原因 = 值_方法配对结果创建失败();
            return 结果;
        }

        auto 状态提交 = 提交目标方法I64特征_方法域动作(
            提交方法首节点,
            提交运行存在,
            目标方法,
            特征_方法条件结果配对状态(),
            1,
            输入参数场景,
            输出结果场景,
            now);
        结果.动态 = 状态提交.动态;
        结果.成功 = 状态提交.成功;
        结果.失败原因 = 状态提交.失败原因;

        auto 写节点结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入基础节点指针(宿主, 特征_方法条件节点(), 结果.节点.条件节点, now);
            (void)写入基础节点指针(宿主, 特征_方法结果节点(), 结果.节点.结果节点, now);
            if (结果.动态) {
                (void)写入基础节点指针(宿主, 特征_动作动态(), 结果.动态, now);
            }
        };
        写节点结果(reinterpret_cast<基础信息节点类*>(提交运行存在));
        写节点结果(reinterpret_cast<基础信息节点类*>(输出结果场景));
        if (auto* 提交方法存在 = 方法虚拟存在(提交方法首节点, now)) {
            写节点结果(reinterpret_cast<基础信息节点类*>(提交方法存在));
        }
        return 结果;
    }

    inline bool 确保目标方法候选补齐结构(
        方法类::节点类* 目标方法,
        存在节点类* 目标方法存在,
        时间戳 now,
        特征节点类*& 能力包,
        特征节点类*& 形参包,
        特征节点类*& 结果包,
        特征节点类*& 条件结果对,
        特征节点类*& 限制包) noexcept
    {
        能力包 = nullptr;
        形参包 = nullptr;
        结果包 = nullptr;
        条件结果对 = nullptr;
        限制包 = nullptr;
        if (!目标方法 || !目标方法存在) return false;
        auto* 宿主 = reinterpret_cast<基础信息节点类*>(目标方法存在);
        能力包 = 取或创建子特征(宿主, 特征_方法能力包());
        形参包 = 取或创建子特征(宿主, 特征_形参表());
        结果包 = 取或创建子特征(宿主, 特征_结果表());
        条件结果对 = 取或创建子特征(宿主, 特征_条件结果对());
        限制包 = 取或创建子特征(宿主, 特征_试运行限制包());
        (void)写入基础节点I64(宿主, 特征_方法补齐结构状态(), 1, now);
        return 能力包 && 形参包 && 结果包 && 条件结果对 && 限制包;
    }

    inline 存在节点类* 尝试学习(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        // 第一段只确认自身方法环境，目标方法环境在读取输入后再处理。
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_尝试学习,
            输出结果场景,
            now,
            "自我_尝试学习",
            [](方法类::节点类* 待确认方法首节点, 存在节点类* 待确认方法存在, 时间戳 当前时间) noexcept {
                return 确认本能方法专属规格(
                    枚举_本能方法ID::自我_尝试学习,
                    待确认方法首节点,
                    待确认方法存在,
                    当前时间);
            });
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(
                环境,
                输入参数场景,
                输出结果场景,
                now);
        }
        清理执行输出派生需求标记(输出结果场景, "自我_尝试学习");
        auto* 实际方法首节点 = 环境.方法首节点;
        auto* 运行存在 = 环境.本轮运行虚拟存在;
        const bool 规格已确认 = 环境.方法规格已确认;

        // 学习本能也不能直接把输入包当规格权威；先用自身条件节点绑定本轮输入。
        auto 绑定 = 按方法条件节点绑定输入(
            实际方法首节点,
            输入参数场景,
            now);
        if (!绑定.匹配成功) {
            // 即使尝试学习自身输入未能完成条件绑定，也要尽量从输入包中解析目标方法。
            // 否则工作线程只能看到“参数绑定失败”，无法形成带目标状态的原子需求。
            方法类::节点类* 目标方法 = nullptr;
            void* 目标方法指针 = nullptr;
            if (绑定.输入包
                && (读取包指针(绑定.输入包, 特征_目标方法头节点(), 目标方法指针)
                    || 读取包指针(绑定.输入包, 特征_缓存方法首节点(), 目标方法指针)
                    || 读取包指针(绑定.输入包, 特征_选中方法首节点(), 目标方法指针))) {
                目标方法 = 解析目标方法指针_动作模块(目标方法指针, now);
            }
            if (目标方法) {
                写入目标方法I64特征派生需求(
                    实际方法首节点,
                    目标方法,
                    特征_方法条件结果对数量(),
                    方法真实条件结果对数量(目标方法),
                    1,
                    输出结果场景,
                    now);
            }
            写入方法条件匹配失败回执(
                reinterpret_cast<基础信息节点类*>(运行存在),
                绑定.条件匹配,
                now);
            写入方法条件匹配失败回执(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                绑定.条件匹配,
                now);
            auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
            更新方法运行账(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        auto* 输入包 = 绑定.输入包;
        auto* 目标方法 = 绑定.目标方法头节点;
        auto* 目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        auto* 因果证据 = 绑定.来源因果信息
            ? 绑定.来源因果信息
            : 绑定.来源观察动态;
        const bool 目标已有方法动作 = 目标方法 && 目标方法->主信息.有方法动作();
        {
            void* 目标特征类型指针 = nullptr;
            const auto* 输入目标特征类型 = 输入包
                && 读取包指针(输入包, 特征_目标特征类型(), 目标特征类型指针)
                ? reinterpret_cast<const 语素入口节点类*>(目标特征类型指针)
                : nullptr;
            const auto& 动作句柄 = 目标方法
                ? 目标方法->主信息.首节点信息().动作句柄
                : 结构体_动作句柄{};
            std::ostringstream 输出;
            输出 << "尝试学习目标方法结构检查"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 目标方法虚拟存在=" << 指针日志文本(目标方法存在)
                << " | 输入目标特征=" << (输入目标特征类型 ? 输入目标特征类型->获取主键() : std::string("空"))
                << " | 目标已有方法动作=" << (目标已有方法动作 ? 1 : 0)
                << " | 目标动作句柄有效=" << (动作句柄.有效() ? 1 : 0)
                << " | 目标动作句柄类型=" << static_cast<int>(动作句柄.类型)
                << " | 目标本能ID=" << 动作句柄.本能ID;
            项目运行日志(输出.str());
        }

        特征节点类* 能力包 = nullptr;
        特征节点类* 形参包 = nullptr;
        特征节点类* 结果包 = nullptr;
        特征节点类* 条件结果对 = nullptr;
        特征节点类* 限制包 = nullptr;
        const bool 已补结构 = 确保目标方法候选补齐结构(
            目标方法,
            目标方法存在,
            now,
            能力包,
            形参包,
            结果包,
            条件结果对,
            限制包);

        I64 尝试学习练习模式 = 0;
        I64 尝试学习候选实参数量 = 0;
        I64 尝试学习绑定参数数量 = 0;
        I64 尝试学习缺失参数数量 = 0;
        auto* 尝试学习候选实参表 = (目标方法 && 目标方法存在)
            ? 生成尝试学习探索实参表(
                目标方法,
                reinterpret_cast<基础信息节点类*>(目标方法存在),
                输入包,
                now,
                尝试学习练习模式,
                尝试学习候选实参数量,
                尝试学习绑定参数数量,
                尝试学习缺失参数数量)
            : nullptr;
        auto* 生成条件场景 = (目标方法存在 && 尝试学习候选实参表 && 尝试学习缺失参数数量 == 0)
            ? 构建练习可执行输入场景(
                reinterpret_cast<基础信息节点类*>(目标方法存在),
                reinterpret_cast<基础信息节点类*>(尝试学习候选实参表),
                now)
            : nullptr;
        auto* 生成结果场景 = 目标方法
            ? 构建目标方法结果能力场景(
                目标方法,
                输出结果场景,
                now)
            : 输出结果场景;
        auto* 建立条件结果对方法 = (目标方法 && 目标已有方法动作)
            ? 确保方法域提交本能方法(
                实际方法首节点,
                枚举_本能方法ID::自我_建立条件结果对,
                now)
            : nullptr;
        auto* 建立条件结果对运行 = 建立条件结果对方法
            ? 本轮运行虚拟存在(建立条件结果对方法, 输出结果场景, now)
            : nullptr;
        auto 条件结果提交 = (目标方法 && 目标已有方法动作)
            ? 提交目标方法条件结果对_方法域动作(
                建立条件结果对方法,
                建立条件结果对运行,
                目标方法,
                生成条件场景 ? 生成条件场景 : 输入参数场景,
                生成结果场景,
                输入参数场景,
                输出结果场景,
                now)
            : 结构_条件结果对提交结果{};
        if (建立条件结果对方法) {
            auto* 提交输出运行 = 输出运行虚拟存在(
                建立条件结果对运行,
                输入参数场景,
                输出结果场景,
                now);
            更新方法运行账(建立条件结果对方法, 提交输出运行, 条件结果提交.成功, now);
        }
        auto 目标条件结果节点 = 条件结果提交.节点;

        if (条件结果对) {
            auto* 对宿主 = reinterpret_cast<基础信息节点类*>(条件结果对);
            (void)写入基础节点I64(
                对宿主,
                特征_方法补齐类别(),
                方法补齐类别值(枚举_方法补齐类别::条件结果对补齐),
                now);
            (void)写入基础节点I64(对宿主, 特征_方法补齐结构状态(), 1, now);
            if (因果证据) {
                (void)写入基础节点指针(对宿主, 特征_来源因果信息(), 因果证据, now);
            }
            if (目标条件结果节点.条件节点) {
                (void)写入基础节点指针(对宿主, 特征_方法条件节点(), 目标条件结果节点.条件节点, now);
            }
            if (目标条件结果节点.结果节点) {
                (void)写入基础节点指针(对宿主, 特征_方法结果节点(), 目标条件结果节点.结果节点, now);
            }
            if (世界树.自我指针) {
                (void)写入基础节点指针(对宿主, 特征_动作主体(), 世界树.自我指针, now);
            }
        }

        const bool 条件结果对提交成功 = 条件结果提交.成功;
        const bool 条件结果阶段成功 = 规格已确认
            && 输入包
            && 目标方法
            && 目标方法存在
            && 目标已有方法动作
            && 已补结构
            && 条件结果对
            && 目标条件结果节点.条件节点
            && 目标条件结果节点.结果节点
            && 条件结果对提交成功;
        const bool 已触发目标方法虚拟存在同步 = 条件结果阶段成功
            && 方法虚拟存在服务类::同步方法节点到虚拟存在(
                目标方法,
                nullptr,
                now);
        auto* 同步后目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        I64 服务派生方法状态 = 方法状态_待条件结果对();
        if (同步后目标方法存在) {
            (void)读取基础节点I64(
                reinterpret_cast<基础信息节点类*>(同步后目标方法存在),
                特征_方法状态(),
                服务派生方法状态);
        }
        const bool 成功 = 条件结果阶段成功 && 同步后目标方法存在 != nullptr;
        const auto* 失败原因 = !输入包
                ? 值_输入条件包绑定失败()
                : (!目标方法
                    ? 值_目标方法头节点缺失()
                        : (!目标已有方法动作
                            ? 值_自我动作缺失()
                            : (!已补结构
                                ? 值_方法虚拟存在缺失()
                                : (!条件结果对提交成功
                                    ? (条件结果提交.失败原因 ? 条件结果提交.失败原因 : 值_方法配对结果创建失败())
                                    : (!同步后目标方法存在 ? 值_方法虚拟存在缺失() : nullptr)))));

        {
            std::ostringstream 输出;
            输出 << "尝试学习条件结果对产物"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 规格已确认=" << (规格已确认 ? 1 : 0)
                << " | 输入包=" << 指针日志文本(输入包)
                << " | 目标方法虚拟存在=" << 指针日志文本(目标方法存在)
                << " | 目标已有方法动作=" << (目标已有方法动作 ? 1 : 0)
                << " | 已补结构=" << (已补结构 ? 1 : 0)
                << " | 条件结果对特征=" << 指针日志文本(条件结果对)
                << " | 条件生成模式=" << 尝试学习练习模式
                << " | 候选实参数量=" << 尝试学习候选实参数量
                << " | 绑定参数数量=" << 尝试学习绑定参数数量
                << " | 缺失参数数量=" << 尝试学习缺失参数数量
                << " | 生成条件场景=" << 指针日志文本(生成条件场景)
                << " | 生成结果场景=" << 指针日志文本(生成结果场景)
                << " | 条件节点=" << (目标条件结果节点.条件节点 ? 目标条件结果节点.条件节点->获取主键() : std::string("空"))
                << " | 结果节点=" << (目标条件结果节点.结果节点 ? 目标条件结果节点.结果节点->获取主键() : std::string("空"))
                << " | 条件结果提交动态=" << 指针日志文本(条件结果提交.动态)
                << " | 目标方法虚拟存在同步=" << (已触发目标方法虚拟存在同步 ? 1 : 0)
                << " | 同步后目标方法虚拟存在=" << 指针日志文本(同步后目标方法存在)
                << " | 服务派生方法状态=" << 服务派生方法状态
                << " | 成功=" << (成功 ? 1 : 0);
            项目运行日志(输出.str());
        }

        // 尝试学习自身已有本能动作；这里只处理被学习的目标缺方法动作。
        // 定向学习只由显式动作缺口触发，不通过“方法状态”中间值绕行。
        if (!成功 && 目标方法 && !目标已有方法动作) {
            写入目标方法I64特征派生需求(
                实际方法首节点,
                目标方法,
                特征_方法动作数量(),
                方法动作数量_动作模块(目标方法),
                1,
                输出结果场景,
                now);
        }
        if (!成功
            && 目标方法
            && 目标已有方法动作
            && (!目标条件结果节点.条件节点 || !目标条件结果节点.结果节点)) {
            // 尝试学习的直接职责是让目标方法拥有真实条件结果对。
            // 目标方法已有动作时，缺口不应退化成“执行失败原因”，而要明确输出
            // 目标方法.方法条件结果对数量 = 1，供自我线程生成更细的需求节点。
            写入目标方法I64特征派生需求(
                实际方法首节点,
                目标方法,
                特征_方法条件结果对数量(),
                方法真实条件结果对数量(目标方法),
                1,
                输出结果场景,
                now);
        }

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            const auto* 执行情况 = 成功
                ? 值_执行成功()
                : ((!输入包 || !目标方法)
                    ? 值_参数绑定失败()
                    : 值_执行失败());
            (void)写入方法执行情况(宿主, 执行情况, now);
            (void)写入基础节点I64(
                宿主,
                特征_方法补齐类别(),
                方法补齐类别值(枚举_方法补齐类别::条件结果对补齐),
                now);
            (void)写入基础节点I64(宿主, 特征_方法补齐状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法动作状态(), 目标已有方法动作 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法动作数量(), 方法动作数量_动作模块(目标方法), now);
            (void)写入基础节点I64(宿主, 特征_方法条件结果配对状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法条件结果对数量(), 方法真实条件结果对数量(目标方法), now);
            (void)写入基础节点I64(宿主, 特征_方法状态(), 成功 ? 服务派生方法状态 : 方法状态_待条件结果对(), now);
            (void)写入基础节点I64(宿主, 特征_内化状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_动作主体替换状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法补齐结构状态(), 已补结构 ? 1 : 0, now);
            (void)写入基础节点指针(宿主, 特征_目标方法头节点(), 目标方法, now);
            (void)写入基础节点指针(宿主, 特征_来源因果信息(), 因果证据, now);
            (void)写入基础节点指针(宿主, 特征_方法补齐尝试结果(), 目标方法存在, now);
            (void)写入基础节点指针(宿主, 特征_条件结果对(), 条件结果对, now);
            (void)写入基础节点指针(宿主, 特征_候选方法能力包(), 能力包, now);
            (void)写入基础节点指针(宿主, 特征_候选形参包(), 形参包, now);
            (void)写入基础节点指针(宿主, 特征_候选结果包(), 结果包, now);
            (void)写入基础节点指针(宿主, 特征_候选实参表(), 尝试学习候选实参表, now);
            (void)写入基础节点指针(宿主, 特征_可执行输入参数场景(), 生成条件场景, now);
            (void)写入基础节点I64(宿主, 特征_练习模式(), 尝试学习练习模式, now);
            (void)写入基础节点I64(宿主, 特征_候选实参数量(), 尝试学习候选实参数量, now);
            (void)写入基础节点I64(宿主, 特征_绑定参数数量(), 尝试学习绑定参数数量, now);
            (void)写入基础节点I64(宿主, 特征_缺失参数数量(), 尝试学习缺失参数数量, now);
            if (世界树.自我指针) {
                (void)写入基础节点指针(宿主, 特征_动作主体(), 世界树.自我指针, now);
            }
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        };
        写结果(reinterpret_cast<基础信息节点类*>(运行存在));
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景));

        auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
        更新方法运行账(实际方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 定向学习(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        // 第一段只确认自身方法环境，目标方法环境在读取输入后再处理。
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_定向学习,
            输出结果场景,
            now,
            "自我_定向学习",
            [](方法类::节点类* 待确认方法首节点, 存在节点类* 待确认方法存在, 时间戳 当前时间) noexcept {
                return 确认本能方法专属规格(
                    枚举_本能方法ID::自我_定向学习,
                    待确认方法首节点,
                    待确认方法存在,
                    当前时间);
            });
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(
                环境,
                输入参数场景,
                输出结果场景,
                now);
        }
        清理执行输出派生需求标记(输出结果场景, "自我_定向学习");
        auto* 实际方法首节点 = 环境.方法首节点;
        auto* 运行存在 = 环境.本轮运行虚拟存在;
        const bool 规格已确认 = 环境.方法规格已确认;

        // 定向学习的目标特征和证据来源，也必须来自条件节点确认后的绑定结果。
        auto 绑定 = 按方法条件节点绑定输入(
            实际方法首节点,
            输入参数场景,
            now);
        if (!绑定.匹配成功) {
            auto* 失败输入包 = reinterpret_cast<基础信息节点类*>(输入条件包根(输入参数场景));
            auto* 失败目标方法 = 读取目标方法头节点(失败输入包);
            void* 失败目标特征类型指针 = nullptr;
            const auto* 失败目标特征类型 = 失败输入包
                && 读取包指针(失败输入包, 特征_目标特征类型(), 失败目标特征类型指针)
                ? reinterpret_cast<const 语素入口节点类*>(失败目标特征类型指针)
                : nullptr;
            const bool 失败目标为方法动作补齐 =
                语素入口主键相同(失败目标特征类型, 特征_方法动作数量())
                || 语素入口主键相同(失败目标特征类型, 特征_方法动作状态());
            const bool 失败目标为方法可执行入口补齐 =
                语素入口主键相同(失败目标特征类型, 特征_方法可执行入口数量());
            if (失败目标方法 && (失败目标为方法动作补齐 || 失败目标为方法可执行入口补齐)) {
                // 即使定向学习自身的条件绑定失败，也要把本轮明确目标写出；
                // 后续派生需求必须围绕真实量化目标，不能退化为空目标。
                写入目标方法I64特征派生需求(
                    实际方法首节点,
                    失败目标方法,
                    失败目标为方法可执行入口补齐
                        ? 特征_方法可执行入口数量()
                        : 特征_方法动作数量(),
                    失败目标为方法可执行入口补齐
                        ? 方法可执行入口数量_动作模块(失败目标方法)
                        : 方法动作数量_动作模块(失败目标方法),
                    1,
                    输出结果场景,
                    now);
            }
            {
                std::ostringstream 输出;
                输出 << "自我_定向学习/条件绑定失败"
                    << " | 目标方法=" << (失败目标方法 ? 失败目标方法->获取主键() : std::string{})
                    << " | 目标特征类型=" << (失败目标特征类型 ? 失败目标特征类型->获取主键() : std::string{})
                    << " | 方法动作目标=" << (失败目标为方法动作补齐 ? 1 : 0)
                    << " | 可执行入口目标=" << (失败目标为方法可执行入口补齐 ? 1 : 0)
                    << " | 已写显式目标=" << ((失败目标方法 && (失败目标为方法动作补齐 || 失败目标为方法可执行入口补齐)) ? 1 : 0);
                项目运行日志(输出.str());
            }
            写入方法条件匹配失败回执(
                reinterpret_cast<基础信息节点类*>(运行存在),
                绑定.条件匹配,
                now);
            写入方法条件匹配失败回执(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                绑定.条件匹配,
                now);
            auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
            更新方法运行账(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        auto* 输入包 = 绑定.输入包;
        auto* 目标方法 = 绑定.目标方法头节点;
        auto* 目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        auto* 目标特征 = 绑定.目标特征节点;
        void* 目标特征类型指针 = nullptr;
        const auto* 目标特征类型 = 输入包
            && 读取包指针(输入包, 特征_目标特征类型(), 目标特征类型指针)
            ? reinterpret_cast<const 语素入口节点类*>(目标特征类型指针)
            : nullptr;
        // 定向学习承接“缺方法动作”需求时，输入通常只给出目标特征类型，
        // 还没有可读取的实例特征节点；此时以特征类型本身作为规格目标。
        const bool 目标为方法动作补齐 =
            语素入口主键相同(目标特征类型, 特征_方法动作数量())
            || 语素入口主键相同(目标特征类型, 特征_方法动作状态());
        const bool 目标为方法可执行入口补齐 =
            语素入口主键相同(目标特征类型, 特征_方法可执行入口数量());
        const bool 目标为动作相关补齐 = 目标为方法动作补齐 || 目标为方法可执行入口补齐;
        const bool 目标特征已确认 = 目标特征 || 目标为动作相关补齐;
        const void* 目标特征输出引用 = 目标特征
            ? static_cast<const void*>(目标特征)
            : static_cast<const void*>(目标特征类型);
        {
            std::ostringstream 输出;
            输出 << "自我_定向学习/目标读取"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string{})
                << " | 目标方法存在=" << (目标方法存在 ? 目标方法存在->获取主键() : std::string{})
                << " | 目标特征节点=" << (目标特征 ? 目标特征->获取主键() : std::string{})
                << " | 目标特征类型=" << (目标特征类型 ? 目标特征类型->获取主键() : std::string{})
                << " | 方法动作目标=" << (目标为方法动作补齐 ? 1 : 0)
                << " | 可执行入口目标=" << (目标为方法可执行入口补齐 ? 1 : 0)
                << " | 目标已确认=" << (目标特征已确认 ? 1 : 0);
            项目运行日志(输出.str());
        }
        auto* 观察证据 = 绑定.来源观察动态
            ? 绑定.来源观察动态
            : 绑定.来源因果信息;

        特征节点类* 能力包 = nullptr;
        特征节点类* 形参包 = nullptr;
        特征节点类* 结果包 = nullptr;
        特征节点类* 条件结果对 = nullptr;
        特征节点类* 限制包 = nullptr;
        const bool 已补结构 = 确保目标方法候选补齐结构(
            目标方法,
            目标方法存在,
            now,
            能力包,
            形参包,
            结果包,
            条件结果对,
            限制包);

        if (条件结果对) {
            auto* 对宿主 = reinterpret_cast<基础信息节点类*>(条件结果对);
            (void)写入基础节点I64(
                对宿主,
                特征_方法补齐类别(),
                方法补齐类别值(枚举_方法补齐类别::候选动作补齐),
                now);
            (void)写入基础节点I64(对宿主, 特征_方法补齐结构状态(), 1, now);
            (void)写入基础节点指针(对宿主, 特征_目标抽象特征(), 目标特征输出引用, now);
            (void)写入基础节点指针(对宿主, 特征_候选动作节点(), 目标方法, now);
            (void)写入基础节点指针(对宿主, 特征_预测状态(), 值_候选动作未验证(), now);
            if (观察证据) {
                (void)写入基础节点指针(对宿主, 特征_来源观察动态(), 观察证据, now);
            }
        }

        const bool 目标已有动作句柄 = 目标方法
            && 目标方法->主信息.首节点信息().动作句柄.有效();
        const I64 目标方法动作数量 = 方法动作数量_动作模块(目标方法);
        const I64 目标方法可执行入口数量 = 方法可执行入口数量_动作模块(目标方法);
        const bool 目标动作补齐可闭合 = !目标为方法动作补齐
            || 目标方法动作数量 > 0;
        const bool 目标可执行入口补齐可闭合 = !目标为方法可执行入口补齐
            || 目标方法可执行入口数量 > 0;
        if (目标为动作相关补齐 && (!目标动作补齐可闭合 || !目标可执行入口补齐可闭合)) {
            std::ostringstream 输出;
            输出 << "自我_定向学习/动作目标不可闭合"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string{})
                << " | 目标方法有方法动作=" << (目标方法 && 目标方法->主信息.有方法动作() ? 1 : 0)
                << " | 方法动作数量=" << 目标方法动作数量
                << " | 方法可执行入口数量=" << 目标方法可执行入口数量
                << " | 目标已有动作句柄=" << (目标已有动作句柄 ? 1 : 0)
                << " | 动作目标闭合=" << (目标动作补齐可闭合 ? 1 : 0)
                << " | 入口目标闭合=" << (目标可执行入口补齐可闭合 ? 1 : 0)
                << " | 说明=定向学习只能记录动作语义候选，不能伪造真实动作数量";
            项目运行日志(输出.str());
        }

        const bool 成功 = 规格已确认
            && 输入包
            && 目标方法
            && 目标方法存在
            && 目标特征已确认
            && 已补结构
            && 条件结果对
            && 目标动作补齐可闭合
            && 目标可执行入口补齐可闭合;
        const auto* 失败原因 = !输入包
            ? 值_输入条件包绑定失败()
                : (!目标方法
                    ? 值_目标方法头节点缺失()
                    : (!目标特征已确认
                        ? 值_目标特征缺失()
                        : ((!目标动作补齐可闭合 || !目标可执行入口补齐可闭合)
                            ? 值_自我动作缺失()
                            : (!已补结构 ? 值_方法虚拟存在缺失() : nullptr))));

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            const auto* 执行情况 = 成功
                ? 值_执行成功()
                : ((!输入包 || !目标方法 || !目标特征已确认)
                    ? 值_参数绑定失败()
                    : 值_执行失败());
            (void)写入方法执行情况(宿主, 执行情况, now);
            (void)写入基础节点I64(
                宿主,
                特征_方法补齐类别(),
                方法补齐类别值(枚举_方法补齐类别::候选动作补齐),
                now);
            (void)写入基础节点I64(宿主, 特征_方法补齐状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法动作状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法动作数量(), 目标方法动作数量, now);
            (void)写入基础节点I64(宿主, 特征_方法可执行入口数量(), 目标方法可执行入口数量, now);
            (void)写入基础节点I64(宿主, 特征_方法状态(), 成功 ? 方法状态_待条件结果对() : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法补齐结构状态(), 已补结构 ? 1 : 0, now);
            (void)写入基础节点指针(宿主, 特征_目标方法头节点(), 目标方法, now);
            (void)写入基础节点指针(宿主, 特征_目标抽象特征(), 目标特征输出引用, now);
            (void)写入基础节点指针(宿主, 特征_来源观察动态(), 观察证据, now);
            (void)写入基础节点指针(宿主, 特征_候选动作节点(), 目标方法, now);
            (void)写入基础节点指针(宿主, 特征_候选方法能力包(), 能力包, now);
            (void)写入基础节点指针(宿主, 特征_候选形参包(), 形参包, now);
            (void)写入基础节点指针(宿主, 特征_候选结果包(), 结果包, now);
            (void)写入基础节点指针(宿主, 特征_条件结果对(), 条件结果对, now);
            (void)写入基础节点指针(宿主, 特征_预测状态(), 值_候选动作未验证(), now);
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        };
        写结果(reinterpret_cast<基础信息节点类*>(运行存在));
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景));
        if (!成功 && 目标方法 && 目标为动作相关补齐) {
            // 定向学习暂不能完成时，也必须把“补方法动作”这个原子目标显式写出；
            // 工作线程据此生成有目标状态的派生需求，禁止再发空目标缺口。
            写入目标方法I64特征派生需求(
                实际方法首节点,
                目标方法,
                目标为方法可执行入口补齐
                    ? 特征_方法可执行入口数量()
                    : 特征_方法动作数量(),
                目标为方法可执行入口补齐
                    ? 目标方法可执行入口数量
                    : 目标方法动作数量,
                1,
                输出结果场景,
                now);
        }

        auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
        更新方法运行账(实际方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 练习(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        // 第一段只确认自身方法环境，目标方法环境在读取输入后再处理。
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_练习,
            输出结果场景,
            now,
            "自我_练习",
            [](方法类::节点类* 待确认方法首节点, 存在节点类* 待确认方法存在, 时间戳 当前时间) noexcept {
                return 确认本能方法专属规格(
                    枚举_本能方法ID::自我_练习,
                    待确认方法首节点,
                    待确认方法存在,
                    当前时间);
            });
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(
                环境,
                输入参数场景,
                输出结果场景,
                now);
        }
        清理执行输出派生需求标记(输出结果场景, "自我_练习");
        auto* 实际方法首节点 = 环境.方法首节点;
        auto* 运行存在 = 环境.本轮运行虚拟存在;
        const bool 规格已确认 = 环境.方法规格已确认;

        // 练习用于补参数和试运行，入口参数必须先通过练习方法自己的条件节点绑定。
        auto 绑定 = 按方法条件节点绑定输入(
            实际方法首节点,
            输入参数场景,
            now);
        if (!绑定.匹配成功) {
            写入方法条件匹配失败回执(
                reinterpret_cast<基础信息节点类*>(运行存在),
                绑定.条件匹配,
                now);
            写入方法条件匹配失败回执(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                绑定.条件匹配,
                now);
            auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
            更新方法运行账(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        auto* 输入包 = 绑定.输入包;
        auto* 目标方法 = 绑定.目标方法头节点;
        auto* 目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        结构_方法条件结果节点 目标条件结果节点{};
        特征节点类* 条件结果对 = nullptr;
        特征节点类* 候选实参表 = nullptr;
        I64 练习模式 = 0;
        I64 候选实参数量 = 0;
        I64 绑定参数数量 = 0;
        I64 缺失参数数量 = 0;
        场景节点类* 可执行输入场景 = nullptr;
        存在节点类* 目标方法运行结果 = nullptr;
        特征节点类* 能力包 = nullptr;
        特征节点类* 形参包 = nullptr;
        特征节点类* 结果包 = nullptr;
        特征节点类* 限制包 = nullptr;
        场景节点类* 目标练习条件场景 = nullptr;
        I64 同条件样本次数 = 0;
        I64 同条件结果符合次数 = 0;
        I64 同条件结果不符合次数 = 0;
        I64 同条件新结果分支数量 = 0;
        constexpr I64 方法好用_目标连续成功次数 = 4;

        auto 读取目标方法运行账 = [&](I64& 运行次数, I64& 成功次数, I64& 最近连续成功次数) noexcept {
            运行次数 = 0;
            成功次数 = 0;
            最近连续成功次数 = 0;
            if (!目标方法存在) {
                return;
            }
            auto* 目标宿主 = reinterpret_cast<基础信息节点类*>(目标方法存在);
            (void)读取基础节点I64(目标宿主, 特征_方法运行次数(), 运行次数);
            (void)读取基础节点I64(目标宿主, 特征_方法成功次数(), 成功次数);
            (void)读取基础节点I64(
                目标宿主,
                特征_方法最近连续成功次数(),
                最近连续成功次数);
        };

        auto 读取目标方法同条件统计 = [&]() noexcept {
            目标练习条件场景 = 查找首个条件结果对条件场景(目标方法);
            同条件样本次数 = 0;
            同条件结果符合次数 = 0;
            同条件结果不符合次数 = 0;
            同条件新结果分支数量 = 0;
            auto* 条件宿主 = reinterpret_cast<基础信息节点类*>(目标练习条件场景);
            if (!条件宿主) {
                return;
            }
            (void)读取基础节点I64(条件宿主, 特征_样本次数(), 同条件样本次数);
            (void)读取基础节点I64(条件宿主, 特征_结果符合次数(), 同条件结果符合次数);
            (void)读取基础节点I64(条件宿主, 特征_结果不符合次数(), 同条件结果不符合次数);
            (void)读取基础节点I64(条件宿主, 特征_新结果分支数量(), 同条件新结果分支数量);
        };

        bool 已补结构 = false;
        if (目标方法存在) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(目标方法存在);
            已补结构 = 确保目标方法候选补齐结构(
                目标方法,
                目标方法存在,
                now,
                能力包,
                形参包,
                结果包,
                条件结果对,
                限制包);
            候选实参表 = 生成练习候选实参表(
                目标方法,
                宿主,
                输入包,
                now,
                练习模式,
                候选实参数量,
                绑定参数数量,
                缺失参数数量);
            已补结构 = 候选实参表 != nullptr || 已补结构;
            if (候选实参表 && 缺失参数数量 == 0) {
                可执行输入场景 = 构建练习可执行输入场景(
                    宿主,
                    reinterpret_cast<基础信息节点类*>(候选实参表),
                    now);
            }
            if (条件结果对) {
                auto* 对宿主 = reinterpret_cast<基础信息节点类*>(条件结果对);
                if (候选实参表) {
                    (void)写入基础节点指针(
                        对宿主,
                        特征_候选实参表(),
                        候选实参表,
                        now);
                    (void)写入基础节点I64(
                        对宿主,
                        特征_练习模式(),
                        练习模式,
                        now);
                    (void)写入基础节点I64(
                        对宿主,
                        特征_候选实参数量(),
                        候选实参数量,
                        now);
                }
            }
            const I64 能力值 = 读取方法自我能力值(目标方法);
            if (能力值 != 0) {
                (void)写入基础节点I64(宿主, 特征_选中自我能力值(), 能力值, now);
            }
        }

        const bool 有入口 = 方法具有本能执行入口(目标方法);
        const auto 目标本能ID = 有入口
            ? static_cast<枚举_本能方法ID>(目标方法->主信息.首节点信息().动作句柄.本能ID)
            : 枚举_本能方法ID::未定义;
        const bool 目标本能是当前默认 = 本能ID是当前默认自我本能_动作模块(目标本能ID);
        const bool 目标本能已注册 = 目标本能是当前默认
            && 本能动作已注册_动作模块(目标本能ID);
        const bool 有已注册真实入口 = 有入口 && 目标本能已注册;
        const bool 目标本能可练习 = 本能动作可作为练习目标(目标本能ID);
        const bool 可试运行 = 有已注册真实入口
            && 目标本能可练习
            && 目标方法 != 实际方法首节点
            && 目标本能ID != 枚举_本能方法ID::自我_练习;
        const bool 有真实条件节点 = 方法真实条件节点数量(目标方法) > 0;
        const bool 有真实条件结果对 = 方法真实条件结果对数量(目标方法) > 0;
        const bool 目标本能句柄非当前默认 = 有入口 && !目标本能是当前默认;
        const bool 目标本能句柄未注册 = 有入口 && 目标本能是当前默认 && !目标本能已注册;
        I64 执行前目标方法运行次数 = 0;
        I64 执行前目标方法成功次数 = 0;
        I64 执行前目标方法最近连续成功次数 = 0;
        读取目标方法运行账(
            执行前目标方法运行次数,
            执行前目标方法成功次数,
            执行前目标方法最近连续成功次数);
        if (可试运行 && 可执行输入场景 && 缺失参数数量 == 0) {
            目标方法运行结果 = 取本能动作集().执行功能函数(
                static_cast<枚举_本能动作ID>(目标本能ID),
                目标方法,
                可执行输入场景,
                输出结果场景);
        }
        const bool 目标执行成功 = 本能动作运行成功(目标方法运行结果);
        const auto 目标运行码 = 本能动作运行结果值(目标方法运行结果);
        const auto* 目标运行语义 = 本能动作运行结果语义_按码(目标运行码);
        const I64 失败阶段方法状态 = !有真实条件节点
            ? 方法状态_待条件节点()
            : (!有真实条件结果对 ? 方法状态_待条件结果对() : 方法状态_可用());
        I64 目标方法运行次数 = 0;
        I64 目标方法成功次数 = 0;
        I64 目标方法最近连续成功次数 = 0;
        读取目标方法运行账(
            目标方法运行次数,
            目标方法成功次数,
            目标方法最近连续成功次数);
        读取目标方法同条件统计();
        const bool 目标方法已自行更新运行账 = 目标方法运行结果
            && 目标方法运行次数 > 执行前目标方法运行次数;
        const I64 练习后方法状态 = 推导方法状态_结构与运行账(
            目标方法,
            目标方法最近连续成功次数);
        const bool 本轮试运行成功 = 目标方法运行结果 && 目标执行成功;
        const bool 同条件样本足够 =
            同条件样本次数 >= 方法好用_目标连续成功次数;
        const bool 同条件未出现新结果分支 =
            同条件新结果分支数量 <= 0;
        const bool 练习目标达成 =
            练习后方法状态 >= 方法状态_好用()
            && 目标方法最近连续成功次数 >= 方法好用_目标连续成功次数
            && 同条件样本足够
            && 同条件未出现新结果分支;
        const bool 成功 = 规格已确认
            && 目标方法
            && 已补结构
            && 有真实条件节点
            && 有真实条件结果对
            && 可试运行
            && 候选实参表
            && 缺失参数数量 == 0
            && 可执行输入场景
            && 本轮试运行成功
            && 练习目标达成;
        const 语素入口节点类* 失败原因 = nullptr;
        if (!输入包) {
            失败原因 = 值_输入条件包绑定失败();
        } else if (!目标方法) {
            失败原因 = 值_目标方法头节点缺失();
        } else if (!有已注册真实入口) {
            失败原因 = 值_缺少执行入口();
        } else if (!可试运行) {
            失败原因 = 值_候选动作未验证();
        } else if (!有真实条件节点 || !有真实条件结果对 || 缺失参数数量 > 0) {
            失败原因 = 值_参数条件缺口();
        } else if (!可执行输入场景) {
            失败原因 = 值_可执行输入参数场景创建失败();
        } else if (!目标方法运行结果 || !目标执行成功) {
            失败原因 = 值_执行失败();
        } else if (!练习目标达成) {
            失败原因 = 值_目标方法尚未好用();
        }

        {
            std::ostringstream 输出;
            输出 << "自我动作/练习目标方法运行账"
                << " | 练习方法=" << (实际方法首节点 ? 实际方法首节点->获取主键() : std::string("空"))
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 目标方法虚拟存在=" << 指针日志文本(目标方法存在)
                << " | 规格已确认=" << (规格已确认 ? 1 : 0)
                << " | 输入包=" << 指针日志文本(输入包)
                << " | 有入口=" << (有入口 ? 1 : 0)
                << " | 目标本能是当前默认=" << (目标本能是当前默认 ? 1 : 0)
                << " | 目标本能已注册=" << (目标本能已注册 ? 1 : 0)
                << " | 有已注册真实入口=" << (有已注册真实入口 ? 1 : 0)
                << " | 目标本能可练习=" << (目标本能可练习 ? 1 : 0)
                << " | 可试运行=" << (可试运行 ? 1 : 0)
                << " | 目标本能ID=" << static_cast<I64>(目标本能ID)
                << " | 有真实条件节点=" << (有真实条件节点 ? 1 : 0)
                << " | 有真实条件结果对=" << (有真实条件结果对 ? 1 : 0)
                << " | 条件结果对=" << 指针日志文本(条件结果对)
                << " | 候选实参表=" << 指针日志文本(候选实参表)
                << " | 候选实参数量=" << 候选实参数量
                << " | 绑定参数数量=" << 绑定参数数量
                << " | 缺失参数数量=" << 缺失参数数量
                << " | 可执行输入场景=" << 指针日志文本(可执行输入场景)
                << " | 目标方法运行结果=" << 指针日志文本(目标方法运行结果)
                << " | 目标运行码=" << 目标运行码
                << " | 目标运行语义=" << (目标运行语义 ? 目标运行语义 : "空")
                << " | 目标执行成功=" << (目标执行成功 ? 1 : 0)
                << " | 目标方法已自行更新运行账=" << (目标方法已自行更新运行账 ? 1 : 0)
                << " | 运行次数=" << 执行前目标方法运行次数 << "->" << 目标方法运行次数
                << " | 成功次数=" << 执行前目标方法成功次数 << "->" << 目标方法成功次数
                << " | 连续成功=" << 执行前目标方法最近连续成功次数 << "->" << 目标方法最近连续成功次数
                << " | 目标练习条件场景=" << 指针日志文本(目标练习条件场景)
                << " | 同条件样本次数=" << 同条件样本次数
                << " | 同条件结果符合次数=" << 同条件结果符合次数
                << " | 同条件结果不符合次数=" << 同条件结果不符合次数
                << " | 同条件新结果分支数量=" << 同条件新结果分支数量
                << " | 同条件样本足够=" << (同条件样本足够 ? 1 : 0)
                << " | 同条件未出现新结果分支=" << (同条件未出现新结果分支 ? 1 : 0)
                << " | 练习后方法状态=" << 练习后方法状态
                << " | 练习目标达成=" << (练习目标达成 ? 1 : 0)
                << " | 成功=" << (成功 ? 1 : 0)
                << " | 失败原因=" << (失败原因 ? 失败原因->获取主键() : std::string("空"));
            项目运行日志(输出.str());
        }

        // 业务缺口才写结构化派生需求；方法句柄存在但运行时未注册属于逻辑错误，
        // 只能报错和记录日志，不能伪装成可学习 / 可派生的业务缺口。
        bool 已写结构化派生需求 = false;
        bool 已阻断业务派生需求 = false;
        auto 记录练习尾部派生处置 = [&](
            const char* 分支,
            const 语素入口节点类* 特征类型,
            I64 当前值,
            I64 目标值,
            bool 将写派生需求) noexcept {
            std::ostringstream 输出;
            输出 << "自我动作/练习尾部派生处置"
                << " | 分支=" << (分支 ? 分支 : "空")
                << " | 练习方法=" << (实际方法首节点 ? 实际方法首节点->获取主键() : std::string("空"))
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 目标方法虚拟存在=" << 指针日志文本(目标方法存在)
                << " | 特征=" << (特征类型 ? 特征类型->获取主键() : std::string("空"))
                << " | 当前值=" << 当前值
                << " | 目标值=" << 目标值
                << " | 将写派生需求=" << (将写派生需求 ? 1 : 0)
                << " | 已写结构化派生需求=" << (已写结构化派生需求 ? 1 : 0)
                << " | 有入口=" << (有入口 ? 1 : 0)
                << " | 目标本能是当前默认=" << (目标本能是当前默认 ? 1 : 0)
                << " | 目标本能已注册=" << (目标本能已注册 ? 1 : 0)
                << " | 有已注册真实入口=" << (有已注册真实入口 ? 1 : 0)
                << " | 目标本能可练习=" << (目标本能可练习 ? 1 : 0)
                << " | 可试运行=" << (可试运行 ? 1 : 0)
                << " | 有真实条件节点=" << (有真实条件节点 ? 1 : 0)
                << " | 有真实条件结果对=" << (有真实条件结果对 ? 1 : 0)
                << " | 缺失参数数量=" << 缺失参数数量
                << " | 可执行输入场景=" << 指针日志文本(可执行输入场景)
                << " | 目标练习条件场景=" << 指针日志文本(目标练习条件场景)
                << " | 同条件样本次数=" << 同条件样本次数
                << " | 同条件新结果分支数量=" << 同条件新结果分支数量
                << " | 失败原因=" << (失败原因 ? 失败原因->获取主键() : std::string("空"));
            项目运行日志(输出.str());
        };
        auto 提示目标本能未注册逻辑错误 = [&]() noexcept {
            std::ostringstream 输出;
            输出 << "自我动作/练习目标本能未注册逻辑错误"
                << " | 练习方法=" << (实际方法首节点 ? 实际方法首节点->获取主键() : std::string("空"))
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 目标方法虚拟存在=" << 指针日志文本(目标方法存在)
                << " | 目标本能ID=" << static_cast<I64>(目标本能ID)
                << " | 有入口=" << (有入口 ? 1 : 0)
                << " | 目标本能是当前默认=" << (目标本能是当前默认 ? 1 : 0)
                << " | 目标本能已注册=" << (目标本能已注册 ? 1 : 0)
                << " | 有已注册真实入口=" << (有已注册真实入口 ? 1 : 0)
                << " | 有真实条件节点=" << (有真实条件节点 ? 1 : 0)
                << " | 有真实条件结果对=" << (有真实条件结果对 ? 1 : 0)
                << " | 缺失参数数量=" << 缺失参数数量
                << " | 处置=记录错误并阻断业务派生需求";
            const auto 文本 = 输出.str();
            项目运行错误日志(文本);

            static std::atomic_bool s_同类错误已弹窗{ false };
            if (!s_同类错误已弹窗.exchange(true)) {
                项目弹窗错误提示("鱼巢 - 自我练习入口逻辑错误", 文本);
            } else {
                项目运行错误日志(
                    "自我动作/练习目标本能未注册逻辑错误 | 同类弹窗已提示，本次仅记录错误日志");
            }
        };
        auto 提示目标本能非当前默认逻辑错误 = [&]() noexcept {
            std::ostringstream 输出;
            输出 << "自我动作/练习目标本能非当前默认逻辑错误"
                << " | 练习方法=" << (实际方法首节点 ? 实际方法首节点->获取主键() : std::string("空"))
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 目标方法虚拟存在=" << 指针日志文本(目标方法存在)
                << " | 目标本能ID=" << static_cast<I64>(目标本能ID)
                << " | 有入口=" << (有入口 ? 1 : 0)
                << " | 目标本能是当前默认=" << (目标本能是当前默认 ? 1 : 0)
                << " | 目标本能已注册=" << (目标本能已注册 ? 1 : 0)
                << " | 处置=记录错误并阻断业务派生需求";
            const auto 文本 = 输出.str();
            项目运行错误日志(文本);

            static std::atomic_bool s_同类错误已弹窗{ false };
            if (!s_同类错误已弹窗.exchange(true)) {
                项目弹窗错误提示("鱼巢 - 自我练习旧本能句柄逻辑错误", 文本);
            } else {
                项目运行错误日志(
                    "自我动作/练习目标本能非当前默认逻辑错误 | 同类弹窗已提示，本次仅记录错误日志");
            }
        };
        auto 提示练习发现缺失参数数量 = [&]() noexcept {
            if (缺失参数数量 <= 0) {
                return;
            }
            std::ostringstream 输出;
            输出 << "自我动作/练习发现缺失参数数量"
                << " | 练习方法=" << (实际方法首节点 ? 实际方法首节点->获取主键() : std::string("空"))
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 目标方法虚拟存在=" << 指针日志文本(目标方法存在)
                << " | 输入包=" << 指针日志文本(输入包)
                << " | 候选实参表=" << 指针日志文本(候选实参表)
                << " | 候选实参数量=" << 候选实参数量
                << " | 绑定参数数量=" << 绑定参数数量
                << " | 缺失参数数量=" << 缺失参数数量
                << " | 有真实条件节点=" << (有真实条件节点 ? 1 : 0)
                << " | 有真实条件结果对=" << (有真实条件结果对 ? 1 : 0)
                << " | 可执行输入场景=" << 指针日志文本(可执行输入场景)
                << " | 失败原因=" << (失败原因 ? 失败原因->获取主键() : std::string("空"))
                << " | 处置=仅作为参数诊断并阻断缺失参数数量派生需求";
            const auto 文本 = 输出.str();
            项目运行错误日志(文本);

            static std::atomic_bool s_同类错误已弹窗{ false };
            if (!s_同类错误已弹窗.exchange(true)) {
                项目弹窗错误提示("鱼巢 - 练习缺失参数诊断", 文本);
            } else {
                项目运行错误日志(
                    "自我动作/练习发现缺失参数数量 | 同类弹窗已提示，本次仅记录错误日志");
            }
        };
        if (缺失参数数量 > 0) {
            提示练习发现缺失参数数量();
        }
        if (!成功 && 目标方法) {
            if (目标本能句柄非当前默认) {
                记录练习尾部派生处置(
                    "目标本能非当前默认-已报错",
                    nullptr,
                    0,
                    0,
                    false);
                提示目标本能非当前默认逻辑错误();
                已阻断业务派生需求 = true;
            } else if (目标本能句柄未注册) {
                记录练习尾部派生处置(
                    "目标本能未注册-已报错",
                    nullptr,
                    0,
                    0,
                    false);
                提示目标本能未注册逻辑错误();
                已阻断业务派生需求 = true;
            } else if (!有已注册真实入口) {
                记录练习尾部派生处置(
                    "缺已注册真实入口",
                    特征_方法可执行入口数量(),
                    0,
                    1,
                    true);
                写入目标方法I64特征派生需求(
                    实际方法首节点,
                    目标方法,
                    特征_方法可执行入口数量(),
                    0,
                    1,
                    输出结果场景,
                    now);
                已写结构化派生需求 = true;
            } else if (!目标本能可练习) {
                记录练习尾部派生处置(
                    "目标本能非练习目标-仅记录观察",
                    nullptr,
                    0,
                    0,
                    false);
                已阻断业务派生需求 = true;
            } else if (!有真实条件节点) {
                记录练习尾部派生处置(
                    "缺真实条件节点",
                    特征_方法条件节点数量(),
                    方法真实条件节点数量(目标方法),
                    1,
                    true);
                写入目标方法I64特征派生需求(
                    实际方法首节点,
                    目标方法,
                    特征_方法条件节点数量(),
                    方法真实条件节点数量(目标方法),
                    1,
                    输出结果场景,
                    now);
                已写结构化派生需求 = true;
            } else if (!有真实条件结果对) {
                记录练习尾部派生处置(
                    "缺真实条件结果对",
                    特征_方法条件结果对数量(),
                    方法真实条件结果对数量(目标方法),
                    1,
                    true);
                写入目标方法I64特征派生需求(
                    实际方法首节点,
                    目标方法,
                    特征_方法条件结果对数量(),
                    方法真实条件结果对数量(目标方法),
                    1,
                    输出结果场景,
                    now);
                已写结构化派生需求 = true;
            } else if (缺失参数数量 > 0) {
                记录练习尾部派生处置(
                    "缺失参数-仅记录参数诊断",
                    特征_缺失参数数量(),
                    缺失参数数量,
                    0,
                    false);
                已阻断业务派生需求 = true;
            } else if (!可执行输入场景) {
                记录练习尾部派生处置(
                    "可执行输入场景缺失",
                    特征_配对状态(),
                    配对状态_配对失败,
                    配对状态_配对成功,
                    true);
                写入目标方法I64特征派生需求(
                    实际方法首节点,
                    目标方法,
                    特征_配对状态(),
                    配对状态_配对失败,
                    配对状态_配对成功,
                    输出结果场景,
                    now);
                已写结构化派生需求 = true;
            }
        }
        if (!成功 && 目标方法 && !已写结构化派生需求 && !已阻断业务派生需求) {
            if (有真实条件结果对
                && 目标方法最近连续成功次数 < 方法好用_目标连续成功次数) {
                记录练习尾部派生处置(
                    "好用目标未达-连续成功次数不足",
                    特征_方法最近连续成功次数(),
                    目标方法最近连续成功次数,
                    方法好用_目标连续成功次数,
                    true);
                写入目标方法I64特征派生需求(
                    实际方法首节点,
                    目标方法,
                    特征_方法最近连续成功次数(),
                    目标方法最近连续成功次数,
                    方法好用_目标连续成功次数,
                    输出结果场景,
                    now);
                已写结构化派生需求 = true;
            } else if (有真实条件结果对
                && 同条件样本次数 < 方法好用_目标连续成功次数) {
                记录练习尾部派生处置(
                    "好用目标未达-同条件样本统计不足",
                    特征_样本次数(),
                    同条件样本次数,
                    方法好用_目标连续成功次数,
                    false);
                已阻断业务派生需求 = true;
            } else if (有真实条件结果对
                && 同条件新结果分支数量 > 0) {
                记录练习尾部派生处置(
                    "好用目标未达-同条件出现新结果分支",
                    特征_新结果分支数量(),
                    同条件新结果分支数量,
                    0,
                    false);
                已阻断业务派生需求 = true;
            }
            记录练习尾部派生处置(
                "好用目标未达-方法状态仅被动派生",
                特征_方法状态(),
                有真实条件结果对 ? 练习后方法状态 : 失败阶段方法状态,
                方法状态_好用(),
                false);
        }

        const bool 已触发目标方法虚拟存在同步 = 成功
            && 方法虚拟存在服务类::同步方法节点到虚拟存在(
                目标方法,
                nullptr,
                now);
        auto* 同步后目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        I64 服务派生方法状态 = 失败阶段方法状态;
        if (同步后目标方法存在) {
            (void)读取基础节点I64(
                reinterpret_cast<基础信息节点类*>(同步后目标方法存在),
                特征_方法状态(),
                服务派生方法状态);
        }
        const bool 已写方法稳定状态 = 成功
            && 方法虚拟存在服务类::写入方法I64特征(
                目标方法,
                特征_方法稳定状态(),
                练习目标达成 ? 1 : 0,
                nullptr,
                now);
        const bool 最终成功 = 成功 && 同步后目标方法存在 != nullptr;
        const auto* 最终失败原因 = 成功 && !同步后目标方法存在
            ? 值_方法虚拟存在缺失()
            : 失败原因;

        {
            std::ostringstream 输出;
            输出 << "自我动作/练习尾部实际结果闸门"
                << " | 练习方法=" << (实际方法首节点 ? 实际方法首节点->获取主键() : std::string("空"))
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 成功=" << (最终成功 ? 1 : 0)
                << " | 将写实际结果=" << (最终成功 ? 1 : 0)
                << " | 已写结构化派生需求=" << (已写结构化派生需求 ? 1 : 0)
                << " | 已阻断业务派生需求=" << (已阻断业务派生需求 ? 1 : 0)
                << " | 目标运行码=" << 目标运行码
                << " | 目标运行语义=" << (目标运行语义 ? 目标运行语义 : "空")
                << " | 目标执行成功=" << (目标执行成功 ? 1 : 0)
                << " | 练习后方法状态=" << 练习后方法状态
                << " | 目标方法虚拟存在同步=" << (已触发目标方法虚拟存在同步 ? 1 : 0)
                << " | 同步后目标方法虚拟存在=" << 指针日志文本(同步后目标方法存在)
                << " | 服务派生方法状态=" << 服务派生方法状态
                << " | 方法稳定状态服务写入=" << (已写方法稳定状态 ? 1 : 0)
                << " | 连续成功=" << 目标方法最近连续成功次数
                << " | 同条件样本次数=" << 同条件样本次数
                << " | 同条件新结果分支数量=" << 同条件新结果分支数量
                << " | 练习目标达成=" << (练习目标达成 ? 1 : 0)
                << " | 失败原因=" << (最终失败原因 ? 最终失败原因->获取主键() : std::string("空"));
            项目运行日志(输出.str());
        }

        auto 写结果 = [&](基础信息节点类* 宿主, const char* 宿主标签) noexcept {
            if (!宿主) return;
            const auto* 写入执行情况 = 最终成功 ? 值_执行成功()
                : ((!输入包 || !目标方法 || 缺失参数数量 > 0)
                    ? 值_参数绑定失败()
                    : 值_执行失败());
            (void)写入方法执行情况(
                宿主,
                写入执行情况,
                now);
            (void)写入基础节点I64(
                宿主,
                特征_方法补齐类别(),
                方法补齐类别值(枚举_方法补齐类别::实参与验证补齐),
                now);
            (void)写入基础节点I64(宿主, 特征_方法补齐状态(), 最终成功 ? 1 : 0, now);
            (void)写入基础节点I64(
                宿主,
                特征_方法状态(),
                服务派生方法状态,
                now);
            (void)写入基础节点I64(宿主, 特征_方法运行账完整性(), 最终成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法稳定状态(), 最终成功 && 练习目标达成 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法运行次数(), 目标方法运行次数, now);
            (void)写入基础节点I64(宿主, 特征_方法成功次数(), 目标方法成功次数, now);
            (void)写入基础节点I64(
                宿主,
                特征_方法最近连续成功次数(),
                目标方法最近连续成功次数,
                now);
            (void)写入基础节点I64(宿主, 特征_样本次数(), 同条件样本次数, now);
            (void)写入基础节点I64(宿主, 特征_结果符合次数(), 同条件结果符合次数, now);
            (void)写入基础节点I64(宿主, 特征_结果不符合次数(), 同条件结果不符合次数, now);
            (void)写入基础节点I64(宿主, 特征_新结果分支数量(), 同条件新结果分支数量, now);
            (void)写入基础节点I64(宿主, 特征_方法补齐结构状态(), 最终成功 ? 2 : (已补结构 ? 1 : 0), now);
            (void)写入基础节点I64(宿主, 特征_试运行次数(), 目标方法运行结果 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_练习模式(), 练习模式, now);
            (void)写入基础节点I64(宿主, 特征_候选实参数量(), 候选实参数量, now);
            (void)写入基础节点I64(宿主, 特征_绑定参数数量(), 绑定参数数量, now);
            (void)写入基础节点I64(宿主, 特征_缺失参数数量(), 缺失参数数量, now);
            (void)写入基础节点指针(宿主, 特征_目标方法头节点(), 目标方法, now);
            (void)写入基础节点指针(宿主, 特征_方法补齐尝试结果(), 目标方法存在, now);
            (void)写入基础节点指针(宿主, 特征_条件结果对(), 条件结果对, now);
            (void)写入基础节点指针(宿主, 特征_候选方法能力包(), 能力包, now);
            (void)写入基础节点指针(宿主, 特征_候选形参包(), 形参包, now);
            (void)写入基础节点指针(宿主, 特征_候选结果包(), 结果包, now);
            (void)写入基础节点指针(宿主, 特征_候选实参表(), 候选实参表, now);
            (void)写入基础节点指针(宿主, 特征_可执行输入参数场景(), 可执行输入场景, now);
            (void)写入基础节点指针(宿主, 特征_最近运行虚拟存在(), 目标方法运行结果, now);
            if (最终失败原因 && !已阻断业务派生需求) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 最终失败原因, now);
            }

            std::ostringstream 输出;
            输出 << "自我动作/练习目标执行成功标识写入"
                << " | 宿主标签=" << (宿主标签 ? 宿主标签 : "空")
                << " | 宿主=" << 指针日志文本(宿主)
                << " | 练习方法=" << (实际方法首节点 ? 实际方法首节点->获取主键() : std::string("空"))
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 目标方法运行结果=" << 指针日志文本(目标方法运行结果)
                << " | 目标运行码=" << 目标运行码
                << " | 目标运行语义=" << (目标运行语义 ? 目标运行语义 : "空")
                << " | 目标执行成功=" << (目标执行成功 ? 1 : 0)
                << " | 写入执行情况=" << (写入执行情况 ? 写入执行情况->获取主键() : std::string("空"))
                << " | 写入方法状态=" << 服务派生方法状态
                << " | 写入运行次数=" << 目标方法运行次数
                << " | 写入成功次数=" << 目标方法成功次数
                << " | 写入连续成功=" << 目标方法最近连续成功次数
                << " | 写入同条件样本次数=" << 同条件样本次数
                << " | 写入新结果分支数量=" << 同条件新结果分支数量
                << " | 写入最近运行虚拟存在=" << 指针日志文本(目标方法运行结果);
            项目运行日志(输出.str());
        };
        写结果(reinterpret_cast<基础信息节点类*>(运行存在), "练习运行存在");
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景), "输出结果场景");
        auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
        更新方法运行账(实际方法首节点, 输出运行, 最终成功, now);
        return 输出运行;
    }

    inline 任务类::节点类* 读取任务筹办输入任务(
        场景节点类* 输入参数场景,
        基础信息节点类* 输入条件包) noexcept
    {
        void* 指针 = nullptr;
        if (读取调用信息指针(输入参数场景, 特征_任务信息节点(), 指针)
            || (输入条件包 && 读取包指针(输入条件包, 特征_任务信息节点(), 指针))) {
            return reinterpret_cast<任务类::节点类*>(指针);
        }
        return nullptr;
    }

    inline 基础信息节点类* 读取任务层回执宿主(
        场景节点类* 输入参数场景,
        基础信息节点类* 输入条件包) noexcept
    {
        // 管理线程可以把已整理好的任务层回执直接放在输入条件包里；
        // 若这里先跟随“当前方法运行存在”指针，显式建议状态会被旧方法运行结果遮住。
        if (输入条件包
            && (查找子特征(输入条件包, 特征_任务层执行回执())
                || 查找子特征(输入条件包, 特征_建议任务状态())
                || 查找子特征(输入条件包, 特征_筹办缺口类型())
                || 查找子特征(输入条件包, 特征_执行缺口类型())
                || 查找子特征(输入条件包, 特征_目标是否达成())
                || 查找子特征(输入条件包, 特征_本轮是否有推进())
                || 查找子特征(输入条件包, 特征_本轮推进量())
                || 查找子特征(输入条件包, 特征_剩余差距())
                || 查找子特征(输入条件包, 特征_执行结果类型())
                || 查找子特征(输入条件包, 特征_当前方法运行存在())
                || 查找子特征(输入条件包, 特征_输出结果场景()))) {
            return 输入条件包;
        }

        void* 指针 = nullptr;
        if (读取调用信息指针(输入参数场景, 特征_当前方法运行存在(), 指针)
            || (输入条件包 && 读取包指针(输入条件包, 特征_当前方法运行存在(), 指针))) {
            return reinterpret_cast<基础信息节点类*>(指针);
        }
        return nullptr;
    }

    inline bool 记录任务虚拟存在指针特征(
        任务类::节点类* 任务,
        const 语素入口节点类* 特征类型,
        const void* 指针,
        场景节点类* 承载场景,
        时间戳 now) noexcept
    {
        return 任务
            && 特征类型
            && 指针
            && 任务类::记录任务虚拟存在特征(
                任务,
                特征类型,
                特征值{ 指针句柄{ reinterpret_cast<std::uintptr_t>(指针) } },
                承载场景,
                now);
    }

    inline bool 记录任务虚拟存在I64特征(
        任务类::节点类* 任务,
        const 语素入口节点类* 特征类型,
        I64 值,
        场景节点类* 承载场景,
        时间戳 now) noexcept
    {
        return 任务
            && 特征类型
            && 任务类::记录任务虚拟存在特征(
                任务,
                特征类型,
                特征值{ 值 },
                承载场景,
                now);
    }

    inline bool 从回执复制指针到任务虚拟存在(
        任务类::节点类* 任务,
        基础信息节点类* 回执宿主,
        const 语素入口节点类* 特征类型,
        场景节点类* 承载场景,
        时间戳 now) noexcept
    {
        void* 指针 = nullptr;
        return 回执宿主
            && 读取基础节点指针(回执宿主, 特征类型, 指针)
            && 记录任务虚拟存在指针特征(任务, 特征类型, 指针, 承载场景, now);
    }

    inline bool 从回执复制I64到任务虚拟存在(
        任务类::节点类* 任务,
        基础信息节点类* 回执宿主,
        const 语素入口节点类* 特征类型,
        场景节点类* 承载场景,
        时间戳 now) noexcept
    {
        I64 值 = 0;
        return 回执宿主
            && 读取基础节点I64(回执宿主, 特征类型, 值)
            && 记录任务虚拟存在I64特征(任务, 特征类型, 值, 承载场景, now);
    }

    inline std::size_t 从回执同步候选方法头节点到任务(
        任务类::节点类* 任务,
        基础信息节点类* 回执宿主,
        场景节点类* 承载场景,
        时间戳 now) noexcept
    {
        if (!任务 || !回执宿主) {
            return 0;
        }

        std::size_t 已同步数量 = 0;
        for (auto* 子特征 : 世界树.特征().获取子特征(回执宿主)) {
            const auto* 主信息 = 世界树.特征().取特征主信息(子特征);
            if (!主信息 || !语素入口相同(主信息->类型, 特征_候选方法头节点())) {
                continue;
            }

            void* 候选方法指针 = nullptr;
            if (!世界树.读取特征_指针(子特征, 候选方法指针) || !候选方法指针) {
                continue;
            }

            auto* 候选方法 = 解析目标方法指针_动作模块(候选方法指针, now);
            auto* 方法存在 = 方法虚拟存在(候选方法, now);
            if (!候选方法 || !方法存在) {
                std::ostringstream 输出;
                输出 << "筹办回执同步/候选方法头节点无效"
                    << " | 任务=" << 任务->获取主键()
                    << " | 候选方法指针=" << 候选方法指针;
                项目运行错误日志(输出.str());
                continue;
            }

            if (任务类::追加任务方法候选虚拟存在(
                    任务,
                    方法存在,
                    承载场景,
                    now)) {
                ++已同步数量;
            }
        }
        return 已同步数量;
    }

    enum class 枚举_任务状态动作动态失败原因 : std::uint8_t {
        无 = 0,
        提交方法首节点缺失,
        任务节点缺失,
        任务场景缺失,
        任务虚拟存在缺失,
        任务状态特征缺失,
        来源方法入口无效,
        动态节点创建失败,
    };

    struct 结构_任务状态动作动态输出结果 {
        动态节点类* 动态 = nullptr;
        枚举_任务状态动作动态失败原因 失败原因 =
            枚举_任务状态动作动态失败原因::无;
    };

    inline const char* 任务状态动作动态失败原因文本(
        枚举_任务状态动作动态失败原因 原因) noexcept
    {
        switch (原因) {
        case 枚举_任务状态动作动态失败原因::提交方法首节点缺失:
            return "提交方法首节点缺失";
        case 枚举_任务状态动作动态失败原因::任务节点缺失:
            return "任务节点缺失";
        case 枚举_任务状态动作动态失败原因::任务场景缺失:
            return "任务场景缺失";
        case 枚举_任务状态动作动态失败原因::任务虚拟存在缺失:
            return "任务虚拟存在缺失";
        case 枚举_任务状态动作动态失败原因::任务状态特征缺失:
            return "任务状态特征缺失";
        case 枚举_任务状态动作动态失败原因::来源方法入口无效:
            return "来源方法入口无效";
        case 枚举_任务状态动作动态失败原因::动态节点创建失败:
            return "动态节点创建失败";
        default:
            return "无";
        }
    }

    inline const 语素入口节点类* 任务状态动作动态失败原因词(
        枚举_任务状态动作动态失败原因 原因) noexcept
    {
        switch (原因) {
        case 枚举_任务状态动作动态失败原因::提交方法首节点缺失:
            return 值_任务状态动作动态_提交方法首节点缺失();
        case 枚举_任务状态动作动态失败原因::任务节点缺失:
            return 值_任务状态动作动态_任务节点缺失();
        case 枚举_任务状态动作动态失败原因::任务场景缺失:
            return 值_任务状态动作动态_任务场景缺失();
        case 枚举_任务状态动作动态失败原因::任务虚拟存在缺失:
            return 值_任务状态动作动态_任务虚拟存在缺失();
        case 枚举_任务状态动作动态失败原因::任务状态特征缺失:
            return 值_任务状态动作动态_任务状态特征缺失();
        case 枚举_任务状态动作动态失败原因::来源方法入口无效:
            return 值_任务状态动作动态_来源方法入口无效();
        case 枚举_任务状态动作动态失败原因::动态节点创建失败:
            return 值_任务状态动作动态_动态节点创建失败();
        default:
            return 值_任务状态动作动态创建失败();
        }
    }

    inline bool 任务状态提交来源方法可生成动作动态(
        const 方法类::节点类* 提交方法首节点) noexcept
    {
        return 提交方法首节点
            && 提交方法首节点->主信息.节点种类() == 枚举_方法节点种类::方法首节点
            && 提交方法首节点->主信息.首节点信息().动作句柄.有效();
    }

    inline void 记录并提示任务状态动作动态逻辑错误(
        枚举_任务状态动作动态失败原因 原因,
        const 方法类::节点类* 提交方法首节点,
        const 任务类::节点类* 任务,
        场景节点类* 场景,
        时间戳 now) noexcept
    {
        std::ostringstream 输出;
        输出 << "任务状态提交/动作动态创建逻辑错误"
            << " | 原因=" << 任务状态动作动态失败原因文本(原因)
            << " | 方法="
            << (提交方法首节点 ? 提交方法首节点->获取主键() : std::string{})
            << " | 任务="
            << (任务 ? 任务->获取主键() : std::string{})
            << " | 场景="
            << (场景 ? 场景->获取主键() : std::string{})
            << " | now=" << now;
        const auto 文本 = 输出.str();
        项目运行错误日志(文本);
        项目弹窗错误提示("鱼巢 - 任务状态动作动态逻辑错误", 文本);
    }

    inline 结构_任务状态动作动态输出结果 输出任务状态提交动作动态(
        方法类::节点类* 提交方法首节点,
        任务类::节点类* 任务,
        枚举_任务状态 旧状态,
        枚举_任务状态 新状态,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept
    {
        if (!提交方法首节点) {
            return { nullptr, 枚举_任务状态动作动态失败原因::提交方法首节点缺失 };
        }
        if (!任务) {
            return { nullptr, 枚举_任务状态动作动态失败原因::任务节点缺失 };
        }

        auto* 场景 = 任务->主信息.场景.指针;
        if (!场景) {
            return { nullptr, 枚举_任务状态动作动态失败原因::任务场景缺失 };
        }
        auto* 任务存在 = 任务类::取或创建_任务虚拟存在(任务, 场景);
        if (!任务存在) {
            return { nullptr, 枚举_任务状态动作动态失败原因::任务虚拟存在缺失 };
        }
        auto* 特征 = 任务存在
            ? 取或创建子特征(reinterpret_cast<基础信息节点类*>(任务存在), 特征_任务状态())
            : nullptr;
        if (!特征) {
            return { nullptr, 枚举_任务状态动作动态失败原因::任务状态特征缺失 };
        }
        if (!任务状态提交来源方法可生成动作动态(提交方法首节点)) {
            return { nullptr, 枚举_任务状态动作动态失败原因::来源方法入口无效 };
        }

        auto* 旧状态节点 = 世界树.状态().创建内部状态(
            场景,
            reinterpret_cast<基础信息节点类*>(任务存在),
            特征,
            特征值{ static_cast<I64>(旧状态) },
            枚举_存在状态事件::创建,
            false,
            now);
        auto* 新状态节点 = 世界树.状态().创建内部状态(
            场景,
            reinterpret_cast<基础信息节点类*>(任务存在),
            特征,
            特征值{ static_cast<I64>(新状态) },
            枚举_存在状态事件::变化,
            旧状态 != 新状态,
            now);
        auto* 动态 = 世界树.动态().创建方法动作动态(
            场景,
            reinterpret_cast<基础信息节点类*>(任务存在),
            特征,
            reinterpret_cast<方法节点类*>(提交方法首节点),
            词_被动动作(),
            now,
            now,
            true,
            0,
            枚举_动作事件相位::完成,
            输入参数场景,
            输出结果场景,
            旧状态节点,
            新状态节点);
        if (!动态) {
            return { nullptr, 枚举_任务状态动作动态失败原因::动态节点创建失败 };
        }
        return { 动态, 枚举_任务状态动作动态失败原因::无 };
    }

    inline bool 任务结果达到目标_提交前检查(
        const 任务类::节点类* 任务,
        时间戳 now) noexcept
    {
        auto* 结果状态 = 任务 ? 任务->主信息.结果状态信息.指针 : nullptr;
        auto* 目标状态 = 任务 ? 任务->主信息.目标状态.指针 : nullptr;
        if (!任务 || !结果状态 || !目标状态) {
            std::ostringstream 输出;
            输出 << "任务状态提交/拒绝完成"
                << " | 任务=" << (任务 ? 任务->获取主键() : std::string{})
                << " | 结果状态=" << (结果状态 ? 结果状态->获取主键() : std::string{})
                << " | 目标状态=" << (目标状态 ? 目标状态->获取主键() : std::string{})
                << " | 原因=缺少任务结果或目标状态";
            项目运行警告日志(输出.str());
            return false;
        }

        const auto 比较 = 世界树.特征().比较状态(结果状态, 目标状态);
        const bool 达成 = 比较.可比较
            && 三向关系被接受(比较.关系, 关系_大于等于);
        std::ostringstream 输出;
        输出 << "任务状态提交/完成判定"
            << " | 任务=" << 任务->获取主键()
            << " | 结果状态=" << 结果状态->获取主键()
            << " | 目标状态=" << 目标状态->获取主键()
            << " | 可比较=" << (比较.可比较 ? 1 : 0)
            << " | 比较关系=" << static_cast<int>(比较.关系)
            << " | 差值=" << 比较.差值
            << " | 允许完成=" << (达成 ? 1 : 0)
            << " | now=" << now;
        if (达成) {
            项目运行日志(输出.str());
        } else {
            项目运行警告日志(输出.str());
        }
        return 达成;
    }

    inline bool 读取状态I64值_任务提交(
        const 状态节点类* 状态,
        I64& 输出值) noexcept
    {
        const auto* 主信息 = 状态 ? 世界树.状态().取状态主信息(状态) : nullptr;
        const auto* 标量值 = 主信息 ? std::get_if<I64>(&主信息->状态值) : nullptr;
        if (!标量值) {
            return false;
        }
        输出值 = *标量值;
        return true;
    }

    inline 状态节点类* 任务提交前一结果状态(
        const 任务类::节点类* 任务) noexcept
    {
        auto* 需求 = reinterpret_cast<const 需求类::节点类*>(
           任务 && 任务->主信息.对应需求.指针
                ? 任务->主信息.对应需求.指针
                : nullptr);
        return 需求 ? 需求->主信息.被需求当前状态.指针 : nullptr;
    }

    inline void 同步任务推进特征_提交路径(
        任务类::节点类* 任务,
        时间戳 now) noexcept
    {
        auto* 结果状态 = 任务 ? 任务->主信息.结果状态信息.指针 : nullptr;
        auto* 目标状态 = 任务 ? 任务->主信息.目标状态.指针 : nullptr;
        if (!任务 || !结果状态 || !目标状态) {
            return;
        }

        const auto 目标比较 = 世界树.特征().比较状态(结果状态, 目标状态);
        const bool 目标达成 = 目标比较.可比较
            && 三向关系被接受(目标比较.关系, 关系_大于等于);

        I64 结果值 = 0;
        I64 目标值 = 0;
        I64 之前值 = 0;
        const bool 结果值有效 = 读取状态I64值_任务提交(结果状态, 结果值);
        const bool 目标值有效 = 读取状态I64值_任务提交(目标状态, 目标值);
        const bool 之前值有效 = 读取状态I64值_任务提交(
            任务提交前一结果状态(任务),
            之前值);

        const I64 剩余差距 = 结果值有效 && 目标值有效 && 目标值 > 结果值
            ? 目标值 - 结果值
            : 0;
        const I64 推进量 = 结果值有效 && 之前值有效
            ? 结果值 - 之前值
            : 0;
        const bool 有推进 = 推进量 > 0;
        const auto 执行结果类型 = 目标达成
            ? 任务模块::筹办::枚举_任务执行结果类型::目标已达成
            : (有推进
                ? 任务模块::筹办::枚举_任务执行结果类型::有推进未达成
                : 任务模块::筹办::枚举_任务执行结果类型::无推进未达成);

        (void)记录任务虚拟存在I64特征(任务, 特征_目标是否达成(), 目标达成 ? 1 : 0, 任务->主信息.场景.指针, now);
        (void)记录任务虚拟存在I64特征(任务, 特征_本轮是否有推进(), 有推进 ? 1 : 0, 任务->主信息.场景.指针, now);
        (void)记录任务虚拟存在I64特征(任务, 特征_本轮推进量(), 推进量, 任务->主信息.场景.指针, now);
        (void)记录任务虚拟存在I64特征(任务, 特征_剩余差距(), 剩余差距, 任务->主信息.场景.指针, now);
        (void)记录任务虚拟存在I64特征(
            任务,
            特征_执行结果类型(),
            static_cast<I64>(执行结果类型),
            任务->主信息.场景.指针,
            now);

        std::ostringstream 输出;
        输出 << "任务状态提交/同步任务推进特征"
            << " | 任务=" << 任务->获取主键()
            << " | 目标达成=" << (目标达成 ? 1 : 0)
            << " | 有推进=" << (有推进 ? 1 : 0)
            << " | 推进量=" << 推进量
            << " | 剩余差距=" << 剩余差距
            << " | 执行结果类型=" << static_cast<int>(执行结果类型);
        项目运行日志(输出.str());
    }

    inline 存在节点类* 提交任务状态变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_提交任务状态变化,
            输出结果场景,
            now,
            "自我_提交任务状态变化",
            确认提交任务状态变化规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 输入条件 = reinterpret_cast<基础信息节点类*>(输入条件包根(输入参数场景));
        auto* 任务 = 读取任务筹办输入任务(输入参数场景, 输入条件);
        auto* 回执宿主 = 读取任务层回执宿主(输入参数场景, 输入条件);
        if (!回执宿主) {
            回执宿主 = 输入条件;
        }

        auto* 运行存在 = 环境.本轮运行虚拟存在;
        I64 新状态值 = 0;
        const bool 状态有效 = 任务
            && 回执宿主
            && 读取基础节点I64(回执宿主, 特征_建议任务状态(), 新状态值)
            && static_cast<枚举_任务状态>(新状态值) != 枚举_任务状态::未定义;

        动态节点类* 动态 = nullptr;
        bool 提交成功 = 状态有效;
        const 语素入口节点类* 失败原因 = nullptr;
        if (状态有效) {
            const auto 旧状态 = 任务类::读取任务状态(任务, 枚举_任务状态::未定义);
            const auto 新状态 = static_cast<枚举_任务状态>(新状态值);
            同步任务推进特征_提交路径(任务, now);
            if (新状态 == 枚举_任务状态::完成
                && !任务结果达到目标_提交前检查(任务, now)) {
                auto* 宿主 = reinterpret_cast<基础信息节点类*>(运行存在);
                (void)写入方法执行情况(宿主, 值_执行失败(), now);
                if (任务) {
                    (void)写入基础节点指针(宿主, 特征_任务信息节点(), 任务, now);
                }
                (void)写入基础节点I64(宿主, 特征_任务状态(), 0, now);
                auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
                更新方法运行账(环境.方法首节点, 输出运行, false, now);
                return 输出运行;
            }
            (void)任务类::写入任务状态(任务, 新状态, 任务->主信息.场景.指针, now);

            I64 新阶段值 = 0;
            if (读取基础节点I64(回执宿主, 特征_建议任务阶段(), 新阶段值)
                && static_cast<枚举_任务阶段>(新阶段值) != 枚举_任务阶段::未定义) {
                (void)任务类::写入任务当前阶段(
                    任务,
                    static_cast<枚举_任务阶段>(新阶段值),
                    任务->主信息.场景.指针,
                    now);
            }

            auto 动态结果 = 输出任务状态提交动作动态(
                环境.方法首节点,
                任务,
                旧状态,
                新状态,
                输入参数场景,
                输出结果场景,
                now);
            动态 = 动态结果.动态;
            if (动态) {
                (void)记录任务虚拟存在指针特征(任务, 特征_任务状态动作动态(), 动态, 任务->主信息.场景.指针, now);
                (void)记录任务虚拟存在指针特征(任务, 特征_动作动态(), 动态, 任务->主信息.场景.指针, now);
            } else {
                提交成功 = false;
                const auto 动态失败原因 =
                    动态结果.失败原因 == 枚举_任务状态动作动态失败原因::无
                    ? 枚举_任务状态动作动态失败原因::动态节点创建失败
                    : 动态结果.失败原因;
                失败原因 = 任务状态动作动态失败原因词(动态失败原因);
                记录并提示任务状态动作动态逻辑错误(
                    动态失败原因,
                    环境.方法首节点,
                    任务,
                    任务->主信息.场景.指针,
                    now);
            }
        }

        auto* 宿主 = reinterpret_cast<基础信息节点类*>(运行存在);
        (void)写入方法执行情况(宿主, 提交成功 ? 值_执行成功() : 值_执行失败(), now);
        if (任务) {
            (void)写入基础节点指针(宿主, 特征_任务信息节点(), 任务, now);
        }
        (void)写入基础节点I64(宿主, 特征_任务状态(), 状态有效 ? 新状态值 : 0, now);
        if (动态) {
            (void)写入基础节点指针(宿主, 特征_任务状态动作动态(), 动态, now);
            (void)写入基础节点指针(宿主, 特征_动作动态(), 动态, now);
        }
        if (失败原因) {
            (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
        }

        auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
        更新方法运行账(环境.方法首节点, 输出运行, 提交成功, now);
        return 输出运行;
    }

    // 提交方法可执行入口是方法域提交本能方法：它只把已选定的自我本能能力
    // 写入目标方法的动作句柄，并输出可验收的“方法动作状态=1 / 可执行入口数量=1”事实；
    // 方法可用仍由条件结果对和后续方法状态提交链决定。
    inline 存在节点类* 提交方法可执行入口(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_提交方法可执行入口,
            输出结果场景,
            now,
            "自我_提交方法可执行入口",
            确认提交方法可执行入口规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 输入包 = reinterpret_cast<基础信息节点类*>(输入条件包根(输入参数场景));
        auto* 运行存在 = 环境.本轮运行虚拟存在;
        auto* 运行宿主 = reinterpret_cast<基础信息节点类*>(运行存在);

        void* 目标方法指针 = nullptr;
        auto* 目标方法 = (输入包
            && 读取基础节点指针(输入包, 特征_目标方法头节点(), 目标方法指针)
            && 目标方法指针)
            ? 解析目标方法指针_动作模块(目标方法指针, now)
            : nullptr;

        I64 能力值 = 0;
        const bool 有能力值 = 输入包
            && 读取基础节点I64(输入包, 特征_选中自我能力值(), 能力值);
        const bool 能力值范围有效 = 能力值 > 0
            && static_cast<unsigned long long>(能力值) <= (std::numeric_limits<std::uint32_t>::max)();
        const auto 本能ID = 能力值范围有效
            ? static_cast<枚举_本能方法ID>(static_cast<std::uint32_t>(能力值))
            : 枚举_本能方法ID::未定义;
        const bool 目标动作运行时已注册 = 本能动作已注册_动作模块(本能ID);
        const bool 是根写入原语 =
            能力值 == 历史本能方法ID_改变特征值;
        const bool 是内部计数治理入口 =
            能力值 == 历史本能方法ID_增加可用方法数量;
        const bool 是历史任务生命周期入口 =
            能力值 == 历史本能方法ID_任务筹办
            || 能力值 == 历史本能方法ID_任务执行;
        const bool 能力可提交 = 能力值范围有效
            && 本能ID != 枚举_本能方法ID::未定义
            && 目标动作运行时已注册
            && !是根写入原语
            && !是内部计数治理入口
            && !是历史任务生命周期入口
            && 本能方法类::是默认自我本能方法(本能ID);

        auto* 目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        auto* 目标方法宿主 = reinterpret_cast<基础信息节点类*>(目标方法存在);

        bool 入口冲突 = false;
        if (目标方法) {
            const auto& 已有句柄 = 目标方法->主信息.首节点信息().动作句柄;
            入口冲突 = 已有句柄.有效()
                && !(已有句柄.类型 == 枚举_动作句柄类型::本能函数ID
                    && 已有句柄.本能ID == 能力值);
        }

        const auto* 失败原因 = !输入包
            ? 值_输入条件包绑定失败()
            : (!目标方法
                ? 值_目标方法头节点缺失()
                : (!有能力值 || !能力可提交
                    ? 值_自我动作缺失()
                    : (入口冲突
                        ? 值_候选方法不可用()
                        : (!目标方法存在 ? 值_方法虚拟存在缺失() : nullptr))));

        const bool 成功 = 输入包
            && 目标方法
            && 目标方法存在
            && 能力可提交
            && !入口冲突;

        I64 旧动作状态 = 0;
        I64 旧可执行入口数量 = 0;
        if (目标方法宿主) {
            (void)读取基础节点I64(目标方法宿主, 特征_方法动作状态(), 旧动作状态);
            (void)读取基础节点I64(目标方法宿主, 特征_方法可执行入口数量(), 旧可执行入口数量);
        }

        动态节点类* 动态 = nullptr;
        场景节点类* 实际输出结果场景 = 输出结果场景;
        I64 目标方法可执行入口数量 = 0;
        if (成功) {
            auto& 首信息 = 目标方法->主信息.首节点信息();
            首信息.动作句柄.类型 = 枚举_动作句柄类型::本能函数ID;
            首信息.动作句柄.本能ID = 能力值;
            首信息.动作句柄.外部实现主键.clear();
            首信息.动作句柄.动作序列主键.clear();
            if (首信息.来源 == 枚举_方法来源::未定义) {
                首信息.来源 = 枚举_方法来源::本能;
            }
            if (!首信息.动作名) {
                if (const auto* 动作名称 = 本能方法类::查询默认动作名称(本能ID)) {
                    首信息.动作名 = 语素集.添加信息入口词(
                        动作名称,
                        枚举_信息入口类型::动态模板入口);
                }
            }

            (void)方法虚拟存在服务类::同步方法节点到虚拟存在(
                目标方法,
                nullptr,
                now);
            (void)写入目标方法I64特征(目标方法, 特征_方法动作状态(), 1, now);
            目标方法可执行入口数量 = 方法可执行入口数量_动作模块(目标方法);
            (void)写入目标方法I64特征(
                目标方法,
                特征_方法可执行入口数量(),
                目标方法可执行入口数量,
                now);
            (void)写入目标方法I64实际结果状态(
                环境.方法首节点,
                目标方法,
                特征_方法动作状态(),
                1,
                实际输出结果场景,
                now);
            (void)写入目标方法I64实际结果状态(
                环境.方法首节点,
                目标方法,
                特征_方法可执行入口数量(),
                目标方法可执行入口数量,
                实际输出结果场景,
                now);

            if (目标方法宿主) {
                auto* 状态特征 = 世界树.取或创建子特征_按类型(
                    目标方法宿主,
                    特征_方法动作状态());
                if (状态特征) {
                    auto* 场景 = 实际输出结果场景
                        ? 实际输出结果场景
                        : 方法类::取或创建_方法结果场景(环境.方法首节点, nullptr, now);
                    if (场景) {
                        (void)世界树.状态().创建内部状态(
                            场景,
                            目标方法宿主,
                            状态特征,
                            旧动作状态,
                            枚举_存在状态事件::创建,
                            false,
                            now);
                        (void)世界树.状态().创建内部状态(
                            场景,
                            目标方法宿主,
                            状态特征,
                            1,
                            枚举_存在状态事件::变化,
                            旧动作状态 != 1,
                            now);
                        动态 = 世界树.动态().创建方法动作动态(
                            场景,
                            目标方法宿主,
                            状态特征,
                            reinterpret_cast<方法节点类*>(环境.方法首节点),
                            环境.方法首节点->主信息.首节点信息().动作名,
                            now,
                            now,
                            true,
                            0,
                            枚举_动作事件相位::完成,
                            输入参数场景,
                            实际输出结果场景);
                    }
                }
            }
        }

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入方法执行情况(宿主, 成功 ? 值_执行成功() : 值_执行失败(), now);
            (void)写入基础节点I64(宿主, 特征_方法动作状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(
                宿主,
                特征_方法可执行入口数量(),
                成功 ? 目标方法可执行入口数量 : 0,
                now);
            (void)写入基础节点I64(宿主, 特征_选中自我能力值(), 有能力值 ? 能力值 : 0, now);
            if (目标方法) {
                (void)写入基础节点指针(宿主, 特征_目标方法头节点(), 目标方法, now);
            }
            if (动态) {
                (void)写入基础节点指针(宿主, 特征_动作动态(), 动态, now);
            }
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        };

        写结果(运行宿主);
        写结果(reinterpret_cast<基础信息节点类*>(实际输出结果场景));
        if (成功 && 目标方法宿主) {
            写结果(目标方法宿主);
        }

        {
            std::ostringstream 输出;
            输出 << "提交方法可执行入口"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 能力值=" << (有能力值 ? 能力值 : 0)
                << " | 目标动作运行时已注册=" << (目标动作运行时已注册 ? 1 : 0)
                << " | 能力可提交=" << (能力可提交 ? 1 : 0)
                << " | 入口冲突=" << (入口冲突 ? 1 : 0)
                << " | 旧可执行入口数量=" << 旧可执行入口数量
                << " | 方法可执行入口数量=" << 目标方法可执行入口数量
                << " | 成功=" << (成功 ? 1 : 0);
            项目运行日志(输出.str());
        }

        auto* 输出运行 = 输出运行虚拟存在(
            运行存在,
            输入参数场景,
            实际输出结果场景,
            now);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 使双目相机达到可用状态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::使双目相机达到可用状态(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 释放双目相机(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::释放双目相机(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 检查双目相机运行状态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::检查双目相机运行状态(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 用双目相机刷新当前观察场景(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::用双目相机刷新当前观察场景(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 用双目相机发现候选存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::用双目相机发现候选存在(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 用双目相机取得当前观察帧(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::用双目相机取得当前观察帧(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 用双目相机取得当前观察特征帧(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::用双目相机取得当前观察特征帧(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 用双目相机建立当前帧像素特征索引(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::用双目相机建立当前帧像素特征索引(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我建立观察帧存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我建立观察帧存在(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我建立观察帧到自我场景接口(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我建立观察帧到自我场景接口(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我建立像素观察事实索引(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我建立像素观察事实索引(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我承接像素特征值(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我承接像素特征值(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我校验观察事实完备性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我校验观察事实完备性(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我评估观察帧质量(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我评估观察帧质量(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我诊断观察质量缺口并派生补观察需求(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我诊断观察质量缺口并派生补观察需求(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我判断基础观察事实可用性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我判断基础观察事实可用性(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 安全评估当前场景安全性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::安全评估当前场景安全性(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我读取自我关键特征变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我读取自我关键特征变化(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我建立最小自我边界参照(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我建立最小自我边界参照(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我生成存在自我相对安全特征(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我生成存在自我相对安全特征(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 安全构造场景影响因果候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::安全构造场景影响因果候选(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我明确当前场景持续观察需求(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我明确当前场景持续观察需求(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 安全持续观察当前场景(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::安全持续观察当前场景(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我明确负向影响处置需求(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我明确负向影响处置需求(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 安全执行降级保护(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::安全执行降级保护(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我验证外设观察像素簇候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我验证外设观察像素簇候选(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我识别外设观察材料(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我识别外设观察材料(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我扫描已识别区域(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我扫描已识别区域(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我跟踪指定存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我跟踪指定存在(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我确认外设观察存在候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我确认外设观察存在候选(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 提交确认观察存在事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::提交确认观察存在事实(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 提交观察存在特征值变化事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::提交观察存在特征值变化事实(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 提交指定存在跟踪事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::提交指定存在跟踪事实(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 提交当前观察范围可观测单位存在对应事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::提交当前观察范围可观测单位存在对应事实(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 提交观察存在发现事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::提交观察存在发现事实(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 提交风险安全场景影响部分状态变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::提交风险安全场景影响部分状态变化(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 安全搜索安全因果因素证据(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::安全搜索安全因果因素证据(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 提交安全因果因素无负证据默认满足(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::提交安全因果因素无负证据默认满足(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 提交安全因果因素证据评估值(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::提交安全因果因素证据评估值(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我按已确认轮廓分区当前帧(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我按已确认轮廓分区当前帧(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我调整相机参数并重采样(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我调整相机参数并重采样(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我按ROI重算观察质量(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我按ROI重算观察质量(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我执行多帧重采样并择优(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我执行多帧重采样并择优(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我生成低置信补偿观察候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我生成低置信补偿观察候选(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我调整外设观察姿态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我调整外设观察姿态(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我调整观察位置(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我调整观察位置(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我按像素坐标回查观察事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我按像素坐标回查观察事实(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我提取空间候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我提取空间候选(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我判断空间候选相对独立性(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我判断空间候选相对独立性(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我生成空间候选观察队列(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我生成空间候选观察队列(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我观察单个空间候选并组合存在假设(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我观察单个空间候选并组合存在假设(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我用空间范围投影覆盖原帧验证像素归属(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我用空间范围投影覆盖原帧验证像素归属(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我确认观察存在并更新帧解释状态(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我确认观察存在并更新帧解释状态(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我确保存在内部世界(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我确保存在内部世界(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我建立存在内外场景接口(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我建立存在内外场景接口(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我将存在局部观察切片映射到内部世界(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我将存在局部观察切片映射到内部世界(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我在内部世界提取子候选(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我在内部世界提取子候选(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我确认内部世界子存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我确认内部世界子存在(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 提交确认内部世界子存在事实(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::提交确认内部世界子存在事实(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我递归细分存在内部世界(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我递归细分存在内部世界(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我匹配当前观察存在与历史观察存在(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我匹配当前观察存在与历史观察存在(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我比较存在特征变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我比较存在特征变化(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我生成观察变化事件(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我生成观察变化事件(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我导出当前场景识别快照(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我导出当前场景识别快照(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 自我导出OpenGL复现数据(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        return 外部模块::自我导出OpenGL复现数据(方法首节点, 输入参数场景, 输出结果场景);
    }

    inline 存在节点类* 建立条件结果对(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_建立条件结果对,
            输出结果场景,
            now,
            "自我_建立条件结果对",
            确认建立条件结果对规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 输入包 = reinterpret_cast<基础信息节点类*>(输入条件包根(输入参数场景));
        auto* 目标方法 = 读取目标方法头节点(输入包, now);

        void* 条件场景指针 = nullptr;
        auto* 条件场景 = (输入包
            && (读取基础节点指针(输入包, 特征_方法条件节点(), 条件场景指针)
                || 读取基础节点指针(输入包, 特征_可执行输入参数场景(), 条件场景指针)))
            ? reinterpret_cast<场景节点类*>(条件场景指针)
            : 输入参数场景;

        void* 结果场景指针 = nullptr;
        auto* 结果场景 = (输入包
            && (读取基础节点指针(输入包, 特征_方法结果节点(), 结果场景指针)
                || 读取基础节点指针(输入包, 特征_输出结果场景(), 结果场景指针)
                || 读取基础节点指针(输入包, 特征_条件结果对(), 结果场景指针)))
            ? reinterpret_cast<场景节点类*>(结果场景指针)
            : 输出结果场景;

        const bool 输入包有效 = 输入包 != nullptr;
        auto 提交结果 = 输入包有效
            ? 提交目标方法条件结果对_方法域动作(
                环境.方法首节点,
                环境.本轮运行虚拟存在,
                目标方法,
                条件场景,
                结果场景,
                输入参数场景,
                输出结果场景,
                now)
            : 结构_条件结果对提交结果{};
        const bool 成功 = 输入包有效 && 提交结果.成功;
        const auto* 失败原因 = !输入包有效
            ? 值_输入条件包绑定失败()
            : 提交结果.失败原因;

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入方法执行情况(宿主, 成功 ? 值_执行成功() : 值_执行失败(), now);
            (void)写入基础节点I64(宿主, 特征_方法条件结果配对状态(), 成功 ? 1 : 0, now);
            if (目标方法) {
                (void)写入基础节点指针(宿主, 特征_目标方法头节点(), 目标方法, now);
            }
            if (提交结果.节点.条件节点) {
                (void)写入基础节点指针(宿主, 特征_方法条件节点(), 提交结果.节点.条件节点, now);
            }
            if (提交结果.节点.结果节点) {
                (void)写入基础节点指针(宿主, 特征_方法结果节点(), 提交结果.节点.结果节点, now);
            }
            if (提交结果.动态) {
                (void)写入基础节点指针(宿主, 特征_动作动态(), 提交结果.动态, now);
            }
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景));

        {
            std::ostringstream 输出;
            输出 << "建立条件结果对"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 条件节点=" << (提交结果.节点.条件节点 ? 提交结果.节点.条件节点->获取主键() : std::string("空"))
                << " | 结果节点=" << (提交结果.节点.结果节点 ? 提交结果.节点.结果节点->获取主键() : std::string("空"))
                << " | 动态=" << 指针日志文本(提交结果.动态)
                << " | 成功=" << (成功 ? 1 : 0);
            项目运行日志(输出.str());
        }

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            输出结果场景,
            now);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 提交方法状态变化(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_提交方法状态变化,
            输出结果场景,
            now,
            "自我_提交方法状态变化",
            确认提交方法状态变化规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 输入包 = reinterpret_cast<基础信息节点类*>(输入条件包根(输入参数场景));
        auto* 目标方法 = 读取目标方法头节点(输入包, now);
        I64 新状态 = 0;
        const bool 有新状态 = 输入包
            && 读取基础节点I64(输入包, 特征_方法状态(), 新状态);

        auto 提交结果 = 有新状态
            ? 提交目标方法I64特征_方法域动作(
                环境.方法首节点,
                环境.本轮运行虚拟存在,
                目标方法,
                特征_方法状态(),
                新状态,
                输入参数场景,
                输出结果场景,
                now)
            : 结构_方法域I64提交结果{};
        const bool 成功 = 有新状态 && 提交结果.成功;
        const auto* 失败原因 = !输入包
            ? 值_输入条件包绑定失败()
            : (!有新状态 ? 值_必需参数缺失() : 提交结果.失败原因);

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入方法执行情况(宿主, 成功 ? 值_执行成功() : 值_执行失败(), now);
            (void)写入基础节点I64(宿主, 特征_方法状态(), 有新状态 ? 新状态 : 0, now);
            if (目标方法) {
                (void)写入基础节点指针(宿主, 特征_目标方法头节点(), 目标方法, now);
            }
            if (提交结果.动态) {
                (void)写入基础节点指针(宿主, 特征_动作动态(), 提交结果.动态, now);
            }
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        };
        写结果(reinterpret_cast<基础信息节点类*>(环境.本轮运行虚拟存在));
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景));

        {
            std::ostringstream 输出;
            输出 << "提交方法状态变化"
                << " | 目标方法=" << (目标方法 ? 目标方法->获取主键() : std::string("空"))
                << " | 旧状态=" << 提交结果.旧值
                << " | 新状态=" << (有新状态 ? 新状态 : 0)
                << " | 动态=" << 指针日志文本(提交结果.动态)
                << " | 成功=" << (成功 ? 1 : 0);
            项目运行日志(输出.str());
        }

        auto* 输出运行 = 输出运行虚拟存在(
            环境.本轮运行虚拟存在,
            输入参数场景,
            输出结果场景,
            now);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline void 刷新父级需求结算_需求提交方法内(
        需求类::节点类* 起点需求) noexcept
    {
        auto* 父需求 = 起点需求 ? reinterpret_cast<需求类::节点类*>(起点需求->父) : nullptr;
        while (父需求 && 父需求->父) {
            I64 子安全合计 = 0;
            I64 子服务合计 = 0;
            if (auto* 首子 = reinterpret_cast<需求类::节点类*>(父需求->子)) {
                auto* 子 = 首子;
                do {
                    if (子) {
                        子安全合计 += (std::max<I64>)(0, 子->主信息.累计安全结算);
                        子服务合计 += (std::max<I64>)(0, 子->主信息.累计服务结算);
                    }
                    子 = 子 ? reinterpret_cast<需求类::节点类*>(子->下) : nullptr;
                } while (子 && 子 != 首子);
            }
            父需求->主信息.累计安全结算 =
                (std::min<I64>)((std::max<I64>)(0, 父需求->主信息.安全权重), 子安全合计);
            父需求->主信息.累计服务结算 =
                (std::min<I64>)((std::max<I64>)(0, 父需求->主信息.服务权重), 子服务合计);
            父需求->主信息.最近结算时间 = 起点需求->主信息.最近结算时间;
            父需求->主信息.最近结算任务主键 = 起点需求->主信息.最近结算任务主键;
            父需求 = reinterpret_cast<需求类::节点类*>(父需求->父);
        }
    }

    inline bool 需求节点目标是OR组_动作模块(const 需求类::节点类* 需求) noexcept
    {
        return 需求
            && 语素入口主键相同(
                需求->主信息.目标特征类型缓存,
                需求类::逻辑组织目标特征类型(需求类::枚举_逻辑组织需求类型::OR组));
    }

    inline 需求类::节点类* 查找最近OR组祖先_动作模块(
        需求类::节点类* 起点) noexcept
    {
        auto* 当前 = 起点;
        std::size_t 保护计数 = 0;
        while (当前 && 保护计数 < 1024) {
            if (需求节点目标是OR组_动作模块(当前)) {
                return 当前;
            }
            当前 = reinterpret_cast<需求类::节点类*>(当前->父);
            ++保护计数;
        }
        return nullptr;
    }

    inline bool 需求路径属于OR组_动作模块(
        const 需求类::节点类* OR组,
        const 需求类::节点类* 路径) noexcept
    {
        if (!OR组 || !路径) return false;
        auto* 当前 = 路径;
        std::size_t 保护计数 = 0;
        while (当前 && 保护计数 < 1024) {
            if (当前 == OR组) {
                return true;
            }
            当前 = reinterpret_cast<const 需求类::节点类*>(当前->父);
            ++保护计数;
        }
        return false;
    }

    inline I64 OR组令牌记录键_动作模块(const 需求类::节点类* OR组) noexcept
    {
        std::uint64_t 哈希 = 1469598103934665603ull;
        const auto 主键 = OR组 ? OR组->获取主键() : std::string{};
        for (const unsigned char 字符 : 主键) {
            哈希 ^= static_cast<std::uint64_t>(字符);
            哈希 *= 1099511628211ull;
        }
        if (主键.empty()) {
            哈希 ^= static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(OR组));
            哈希 *= 1099511628211ull;
        }
        const auto 正值 = static_cast<I64>(哈希 & 0x7fffffffffffffffULL);
        return 正值 != 0 ? 正值 : 1;
    }

    inline 存在节点类* 取或创建OR组令牌记录_动作模块(
        需求类::节点类* OR组,
        时间戳 now) noexcept
    {
        auto* 自我对象 = 自我类::当前活动自我();
        if (!自我对象) {
            自我对象 = &自我;
        }
        auto* 自我存在 = 自我对象 ? 自我对象->确保自我存在() : nullptr;
        if (!OR组 || !自我存在) {
            return nullptr;
        }
        auto* 记录 = 世界树.取或创建子存在_按类型并按特征I64(
            reinterpret_cast<基础信息节点类*>(自我存在),
            特征_OR组结算令牌记录(),
            特征_OR组令牌记录键(),
            OR组令牌记录键_动作模块(OR组));
        if (记录) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(记录);
            (void)写入基础节点指针(宿主, 特征_OR组节点(), OR组, now);
        }
        return 记录;
    }

    inline 动态节点类* 输出OR组令牌动作动态(
        方法类::节点类* 提交方法首节点,
        存在节点类* 令牌记录,
        需求类::节点类* OR组,
        需求类::节点类* 领取路径,
        动态节点类* 来源动作动态,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept
    {
        if (!提交方法首节点 || !令牌记录 || !OR组 || !领取路径) {
            return nullptr;
        }
        auto* 场景 = OR组->主信息.需求场景.指针
            ? OR组->主信息.需求场景.指针
            : (输出结果场景 ? 输出结果场景 : 输入参数场景);
        auto* 宿主 = reinterpret_cast<基础信息节点类*>(令牌记录);
        auto* 特征 = 取或创建子特征(宿主, 特征_OR组结算令牌状态());
        if (!场景 || !宿主 || !特征) {
            return nullptr;
        }

        auto* 动态 = 世界树.动态().创建方法动作动态(
            场景,
            宿主,
            特征,
            reinterpret_cast<方法节点类*>(提交方法首节点),
            提交方法首节点->主信息.首节点信息().动作名
                ? 提交方法首节点->主信息.首节点信息().动作名
                : 词_被动动作(),
            now,
            now,
            true,
            0,
            枚举_动作事件相位::完成,
            输入参数场景,
            输出结果场景);
        if (!动态) {
            return nullptr;
        }

        (void)写入基础节点指针(宿主, 特征_OR组节点(), OR组, now);
        (void)写入基础节点指针(宿主, 特征_OR组领取路径(), 领取路径, now);
        (void)写入基础节点指针(宿主, 特征_OR组令牌领取动作动态(), 动态, now);
        (void)写入基础节点指针(宿主, 特征_动作动态(), 动态, now);
        if (来源动作动态) {
            (void)世界树.动态().追加来源低层动态(动态, 来源动作动态);
            (void)写入基础节点指针(宿主, 特征_来源动作动态(), 来源动作动态, now);
        }
        return 动态;
    }

    inline 动态节点类* 输出叶子任务价值结算动作动态(
        方法类::节点类* 结算方法首节点,
        自我类& 自我对象,
        const 语素入口节点类* 特征类型,
        I64 旧值,
        I64 新值,
        动态节点类* 来源动作动态,
        动态节点类* 满足证据动态,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept
    {
        auto* 自我存在 = 自我对象.确保自我存在();
        auto* 场景 = 自我对象.确保自我内部世界();
        if (!结算方法首节点 || !自我存在 || !场景 || !特征类型) {
            return nullptr;
        }

        auto* 特征 = 世界树.取或创建子特征_按类型(
            reinterpret_cast<基础信息节点类*>(自我存在),
            特征类型);
        if (!特征) {
            return nullptr;
        }

        auto* 旧状态 = 世界树.状态().创建状态(
            场景,
            reinterpret_cast<基础信息节点类*>(自我存在),
            特征,
            特征值{ 旧值 },
            now,
            now);
        auto* 新状态 = 世界树.状态().创建状态(
            场景,
            reinterpret_cast<基础信息节点类*>(自我存在),
            特征,
            特征值{ 新值 },
            now,
            now);
        auto* 动态 = 世界树.动态().创建方法动作动态(
            场景,
            reinterpret_cast<基础信息节点类*>(自我存在),
            特征,
            reinterpret_cast<方法节点类*>(结算方法首节点),
            结算方法首节点->主信息.首节点信息().动作名,
            now,
            now,
            true,
            0,
            枚举_动作事件相位::完成,
            输入参数场景,
            输出结果场景,
            旧状态,
            新状态);
        if (!动态) {
            return nullptr;
        }

        if (来源动作动态) {
            (void)世界树.动态().追加来源低层动态(动态, 来源动作动态);
        }
        if (满足证据动态 && 满足证据动态 != 来源动作动态) {
            (void)世界树.动态().追加来源低层动态(动态, 满足证据动态);
        }
        (void)世界树.按动态创建因果信息(
            场景,
            动态,
            reinterpret_cast<基础信息节点类*>(自我存在),
            reinterpret_cast<基础信息节点类*>(特征),
            {},
            true);
        return 动态;
    }

    inline bool 合法叶子任务结算增量(I64 值) noexcept
    {
        return 值 >= 0 && 值 != (std::numeric_limits<I64>::max)();
    }

    inline 存在节点类* 结算叶子任务价值(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::自我_结算叶子任务价值,
            输出结果场景,
            now,
            "自我_结算叶子任务价值",
            确认结算叶子任务价值规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 输入条件 = reinterpret_cast<基础信息节点类*>(输入条件包根(输入参数场景));
        auto* 运行存在 = 环境.本轮运行虚拟存在;
        auto* 运行宿主 = reinterpret_cast<基础信息节点类*>(运行存在);
        void* 任务指针 = nullptr;
        void* 任务存在指针 = nullptr;
        void* 需求指针 = nullptr;
        void* 来源动态指针 = nullptr;
        void* 证据动态指针 = nullptr;
        void* 实际结果状态指针 = nullptr;
        I64 输入任务状态 = static_cast<I64>(枚举_任务状态::未定义);
        I64 计划安全增量 = 0;
        I64 计划服务增量 = 0;
        I64 任务完成度百万分比 = 0;
        I64 服务对象外在特征状态变化数 = 0;
        I64 服务对象外在特征状态改善证据 = 0;
        I64 服务结果可验证 = 0;
        I64 服务结果可归因于自我动作 = 0;
        (void)读取包指针(输入条件, 特征_任务信息节点(), 任务指针);
        (void)读取包指针(输入条件, 特征_任务虚拟存在(), 任务存在指针);
        (void)读取包指针(输入条件, 特征_来源需求(), 需求指针);
        (void)读取包指针(输入条件, 特征_来源动作动态(), 来源动态指针);
        (void)读取包指针(输入条件, 特征_满足证据(), 证据动态指针);
        (void)读取包指针(输入条件, 特征_实际结果状态(), 实际结果状态指针);
        (void)读取基础节点I64(输入条件, 特征_任务状态(), 输入任务状态);
        (void)读取基础节点I64(输入条件, 特征_安全结算增量(), 计划安全增量);
        (void)读取基础节点I64(输入条件, 特征_服务结算增量(), 计划服务增量);
        (void)读取基础节点I64(输入条件, 特征_任务完成度百万分比(), 任务完成度百万分比);
        (void)读取基础节点I64(输入条件, 特征_服务对象外在特征状态变化数(), 服务对象外在特征状态变化数);
        (void)读取基础节点I64(输入条件, 特征_服务对象外在特征状态改善证据(), 服务对象外在特征状态改善证据);
        (void)读取基础节点I64(输入条件, 特征_服务结果可验证(), 服务结果可验证);
        (void)读取基础节点I64(输入条件, 特征_服务结果可归因于自我动作(), 服务结果可归因于自我动作);

        auto* 自我对象 = 自我类::当前活动自我();
        if (!自我对象) {
            自我对象 = &自我;
        }
        auto* 任务 = 任务指针 ? reinterpret_cast<任务类::节点类*>(任务指针) : nullptr;
        auto* 任务存在 = 任务存在指针 ? reinterpret_cast<存在节点类*>(任务存在指针) : nullptr;
        auto* 来源需求 = 需求指针 ? reinterpret_cast<需求类::节点类*>(需求指针) : nullptr;
        auto* 实际结果状态 = 实际结果状态指针
            ? reinterpret_cast<状态节点类*>(实际结果状态指针)
            : (任务 ? 任务类::解析任务结果状态摘要_兼容(任务) : nullptr);
        const bool 任务存在匹配 =
            任务 && 任务存在 && 任务->主信息.任务虚拟存在.指针 == 任务存在;
        const bool 输入任务状态已结算 =
            输入任务状态 == static_cast<I64>(枚举_任务状态::已结算);
        const bool 任务已形成结算被动因 =
            任务
            && 任务存在匹配
            && 输入任务状态已结算
            && 任务类::读取任务状态(任务) == 枚举_任务状态::已结算;
        const I64 有效完成度百万分比 =
            std::clamp<I64>(任务完成度百万分比, 0, 1'000'000);
        const bool 完成度合法 = 有效完成度百万分比 > 0;
        const bool 增量合法 =
            合法叶子任务结算增量(计划安全增量)
            && 合法叶子任务结算增量(计划服务增量);
        bool 成功 =
            自我对象
            && 任务
            && 任务存在
            && 来源需求
            && 任务已形成结算被动因
            && 完成度合法
            && 增量合法
            && 实际结果状态;
        I64 旧安全值 = 自我对象 ? 自我对象->获取安全值() : 0;
        I64 旧服务值 = 自我对象 ? 自我对象->获取服务值() : 0;
        I64 新安全值 = 旧安全值;
        I64 新服务值 = 旧服务值;
        I64 实际安全增量 = 0;
        I64 实际服务增量 = 0;
        const bool 服务结算证据通过 =
            计划服务增量 <= 0
            || (服务对象外在特征状态变化数 > 0
                && 服务对象外在特征状态改善证据 > 0
                && 服务结果可验证 > 0
                && 服务结果可归因于自我动作 > 0);
        const I64 有效计划服务增量 =
            服务结算证据通过 ? 计划服务增量 : 0;
        动态节点类* 安全动态 = nullptr;
        动态节点类* 服务动态 = nullptr;

        if (成功) {
            const auto 按完成度折算 = [](I64 增量, I64 完成度) noexcept -> I64 {
                增量 = (std::max<I64>)(0, 增量);
                完成度 = std::clamp<I64>(完成度, 0, 1'000'000);
                if (增量 <= 0 || 完成度 <= 0) {
                    return 0;
                }
                if (完成度 >= 1'000'000) {
                    return 增量;
                }
                constexpr I64 分母 = 1'000'000;
                return (增量 / 分母) * 完成度
                    + ((增量 % 分母) * 完成度) / 分母;
            };
            const I64 安全增量 = 按完成度折算(计划安全增量, 有效完成度百万分比);
            const I64 服务增量 = 按完成度折算(有效计划服务增量, 有效完成度百万分比);
            if (安全增量 > 0 && 旧安全值 < 自我对象->根目标安全值()) {
                新安全值 = (std::min<I64>)(旧安全值 + 安全增量, 自我对象->根目标安全值());
                if (新安全值 != 旧安全值
                    && 自我对象->写入自我I64特征当前值(特征_安全值(), 新安全值, now)) {
                    实际安全增量 = (std::max<I64>)(0, 新安全值 - 旧安全值);
                    安全动态 = 输出叶子任务价值结算动作动态(
                        环境.方法首节点,
                        *自我对象,
                        特征_安全值(),
                        旧安全值,
                        新安全值,
                        reinterpret_cast<动态节点类*>(来源动态指针),
                        reinterpret_cast<动态节点类*>(证据动态指针),
                        输入参数场景,
                        输出结果场景,
                        now);
                } else {
                    新安全值 = 旧安全值;
                }
            }
            if (服务增量 > 0 && 旧服务值 < 自我对象->根目标服务值()) {
                新服务值 = (std::min<I64>)(旧服务值 + 服务增量, 自我对象->根目标服务值());
                if (新服务值 != 旧服务值
                    && 自我对象->写入自我I64特征当前值(特征_服务值(), 新服务值, now)) {
                    实际服务增量 = (std::max<I64>)(0, 新服务值 - 旧服务值);
                    服务动态 = 输出叶子任务价值结算动作动态(
                        环境.方法首节点,
                        *自我对象,
                        特征_服务值(),
                        旧服务值,
                        新服务值,
                        reinterpret_cast<动态节点类*>(来源动态指针),
                        reinterpret_cast<动态节点类*>(证据动态指针),
                        输入参数场景,
                        输出结果场景,
                        now);
                } else {
                    新服务值 = 旧服务值;
                }
            }
        }

        if (成功 && 计划服务增量 > 0 && !服务结算证据通过) {
            std::ostringstream 证据日志;
            证据日志 << "自我动作/结算叶子任务价值服务结算证据不足"
                << " | 任务=" << (任务 ? 任务->获取主键() : std::string("空"))
                << " | 需求=" << (来源需求 ? 来源需求->获取主键() : std::string("空"))
                << " | 原计划服务增量=" << 计划服务增量
                << " | 服务对象外在特征状态变化数=" << 服务对象外在特征状态变化数
                << " | 外在改善证据=" << (服务对象外在特征状态改善证据 > 0 ? "有" : "缺")
                << " | 外在反馈可验证=" << (服务结果可验证 > 0 ? "是" : "否")
                << " | 可归因=" << (服务结果可归因于自我动作 > 0 ? "是" : "否")
                << " | 说明=服务对象外在特征改善/反馈验证/归因证据不足，服务增量按0结算";
            项目运行警告日志(证据日志.str());
        }

        if (运行宿主) {
            (void)写入方法执行情况(运行宿主, 成功 ? 值_执行成功() : 值_执行失败(), now);
            if (任务) {
                (void)写入基础节点指针(运行宿主, 特征_任务信息节点(), 任务, now);
            }
            if (任务存在) {
                (void)写入基础节点指针(运行宿主, 特征_任务虚拟存在(), 任务存在, now);
            }
            (void)写入基础节点I64(运行宿主, 特征_任务状态(), 输入任务状态, now);
            if (来源需求) {
                (void)写入基础节点指针(运行宿主, 特征_来源需求(), 来源需求, now);
            }
            (void)写入基础节点I64(运行宿主, 特征_旧安全值(), 旧安全值, now);
            (void)写入基础节点I64(运行宿主, 特征_新安全值(), 新安全值, now);
            (void)写入基础节点I64(运行宿主, 特征_旧服务值(), 旧服务值, now);
            (void)写入基础节点I64(运行宿主, 特征_新服务值(), 新服务值, now);
            (void)写入基础节点I64(运行宿主, 特征_安全结算增量(), 实际安全增量, now);
            (void)写入基础节点I64(运行宿主, 特征_服务结算增量(), 实际服务增量, now);
            (void)写入基础节点I64(运行宿主, 特征_服务对象外在特征状态变化数(), 服务对象外在特征状态变化数, now);
            (void)写入基础节点I64(
                运行宿主,
                特征_服务对象外在特征状态改善证据(),
                服务对象外在特征状态改善证据 > 0 ? 1 : 0,
                now);
            (void)写入基础节点I64(
                运行宿主,
                特征_服务结果可验证(),
                服务结果可验证 > 0 ? 1 : 0,
                now);
            (void)写入基础节点I64(
                运行宿主,
                特征_服务结果可归因于自我动作(),
                服务结果可归因于自我动作 > 0 ? 1 : 0,
                now);
            (void)写入基础节点I64(
                运行宿主,
                特征_服务结算证据通过(),
                服务结算证据通过 ? 1 : 0,
                now);
            (void)写入基础节点I64(
                运行宿主,
                特征_任务完成度百万分比(),
                有效完成度百万分比,
                now);
            if (实际结果状态) {
                (void)写入基础节点指针(运行宿主, 特征_实际结果状态(), 实际结果状态, now);
            }
            if (安全动态) {
                (void)写入基础节点指针(运行宿主, 特征_动作动态(), 安全动态, now);
            } else if (服务动态) {
                (void)写入基础节点指针(运行宿主, 特征_动作动态(), 服务动态, now);
            }
        }

        std::ostringstream 日志;
        日志 << "自我动作/结算叶子任务价值"
            << " | 成功=" << (成功 ? "是" : "否")
            << " | 任务=" << (任务 ? 任务->获取主键() : std::string("空"))
            << " | 任务虚拟存在=" << (任务存在 ? 任务存在->获取主键() : std::string("空"))
            << " | 需求=" << (来源需求 ? 来源需求->获取主键() : std::string("空"))
            << " | 结算被动因=" << (任务已形成结算被动因 ? "是" : "否")
            << " | 任务存在匹配=" << (任务存在匹配 ? "是" : "否")
            << " | 输入任务状态=" << 输入任务状态
            << " | 有实际结果状态=" << (实际结果状态 ? "是" : "否")
            << " | 完成度百万分比=" << 有效完成度百万分比
            << " | 输入安全+=" << 计划安全增量
            << " | 输入服务+=" << 计划服务增量
            << " | 服务证据通过=" << (服务结算证据通过 ? "是" : "否")
            << " | 服务对象外在特征状态变化数=" << 服务对象外在特征状态变化数
            << " | 实际安全+=" << 实际安全增量
            << " | 实际服务+=" << 实际服务增量;
        if (!增量合法) {
            日志 << " | 非法结算增量=是";
        }
        项目运行日志(日志.str());

        auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 提交领取OR组结算令牌(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto 环境 = 确保本能方法自身环境(
            方法首节点,
            枚举_本能方法ID::提交_领取OR组结算令牌,
            输出结果场景,
            now,
            "提交_领取OR组结算令牌",
            确认提交领取OR组结算令牌规格);
        if (!环境.已成功) {
            return 输出本能方法自身环境失败(环境, 输入参数场景, 输出结果场景, now);
        }

        auto* 输入条件 = reinterpret_cast<基础信息节点类*>(输入条件包根(输入参数场景));
        auto* 运行存在 = 环境.本轮运行虚拟存在;
        auto* 运行宿主 = reinterpret_cast<基础信息节点类*>(运行存在);
        void* OR组指针 = nullptr;
        void* 路径指针 = nullptr;
        void* 来源需求指针 = nullptr;
        void* 任务指针 = nullptr;
        void* 来源动态指针 = nullptr;
        (void)读取包指针(输入条件, 特征_OR组节点(), OR组指针);
        (void)读取包指针(输入条件, 特征_OR组领取路径(), 路径指针);
        (void)读取包指针(输入条件, 特征_来源需求(), 来源需求指针);
        (void)读取包指针(输入条件, 特征_任务信息节点(), 任务指针);
        (void)读取包指针(输入条件, 特征_来源动作动态(), 来源动态指针);

        auto* 来源需求 = 来源需求指针
            ? reinterpret_cast<需求类::节点类*>(来源需求指针)
            : nullptr;
        auto* 领取路径 = 路径指针
            ? reinterpret_cast<需求类::节点类*>(路径指针)
            : 来源需求;
        auto* OR组 = OR组指针
            ? reinterpret_cast<需求类::节点类*>(OR组指针)
            : 查找最近OR组祖先_动作模块(领取路径);
        if (!OR组 && 来源需求 && 来源需求 != 领取路径) {
            OR组 = 查找最近OR组祖先_动作模块(来源需求);
        }

        auto* 任务 = 任务指针 ? reinterpret_cast<任务类::节点类*>(任务指针) : nullptr;
        auto* 来源动态 = 来源动态指针
            ? reinterpret_cast<动态节点类*>(来源动态指针)
            : nullptr;
        auto* 令牌记录 = 取或创建OR组令牌记录_动作模块(OR组, now);
        const bool 输入有效 =
            输入条件
            && OR组
            && 领取路径
            && 令牌记录
            && 需求节点目标是OR组_动作模块(OR组)
            && 需求路径属于OR组_动作模块(OR组, 领取路径);

        I64 已有令牌状态 = OR组结算令牌状态_未领取;
        const bool 已领取 =
            令牌记录
            && 读取基础节点I64(
                reinterpret_cast<基础信息节点类*>(令牌记录),
                特征_OR组结算令牌状态(),
                已有令牌状态)
            && 已有令牌状态 == OR组结算令牌状态_已领取;
        bool 首次领取 = false;
        bool 重复领取 = false;
        bool OR组已收束 = false;
        动态节点类* 令牌动态 = nullptr;
        I64 提交状态 = OR组令牌提交状态_拒绝;

        if (输入有效 && !已领取) {
            auto* 记录宿主 = reinterpret_cast<基础信息节点类*>(令牌记录);
            (void)写入基础节点I64(记录宿主, 特征_OR组结算令牌状态(), OR组结算令牌状态_已领取, now);
            (void)写入基础节点指针(记录宿主, 特征_OR组节点(), OR组, now);
            (void)写入基础节点指针(记录宿主, 特征_OR组领取路径(), 领取路径, now);
            if (任务) {
                (void)写入基础节点指针(记录宿主, 特征_OR组令牌领取任务(), 任务, now);
            }
            (void)写入基础节点I64(记录宿主, 特征_OR组令牌领取时间(), static_cast<I64>(now), now);
            令牌动态 = 输出OR组令牌动作动态(
                环境.方法首节点,
                令牌记录,
                OR组,
                领取路径,
                来源动态,
                输入参数场景,
                输出结果场景,
                now);

            if (OR组->主信息.需求状态.指针
                || !OR组->主信息.需求状态.主键.empty()) {
                OR组->主信息.被需求当前状态 = OR组->主信息.需求状态;
                OR组->主信息.需求有效截止 = now;
                OR组->主信息.最近结算时间 = now;
                if (任务) {
                    OR组->主信息.最近结算任务主键 = 任务->获取主键();
                }
                OR组->主信息.统计.最后观测时间 = now;
                刷新父级需求结算_需求提交方法内(OR组);
                需求类 需求服务{};
                if (auto* 父需求 = reinterpret_cast<需求类::节点类*>(OR组->父)) {
                    (void)需求服务.刷新子需求权重(父需求);
                }
                (void)需求服务.刷新子需求权重(OR组);
                需求类::刷新需求及祖先结构角色(OR组);
                OR组已收束 = true;
            }
            首次领取 = true;
            提交状态 = OR组令牌提交状态_首次领取;
        } else if (输入有效 && 已领取) {
            auto* 记录宿主 = reinterpret_cast<基础信息节点类*>(令牌记录);
            I64 重复次数 = 0;
            (void)读取基础节点I64(记录宿主, 特征_OR组令牌重复领取次数(), 重复次数);
            (void)写入基础节点I64(记录宿主, 特征_OR组令牌重复领取次数(), 重复次数 + 1, now);
            (void)写入基础节点I64(记录宿主, 特征_OR组令牌重复领取状态(), 1, now);
            (void)写入基础节点指针(记录宿主, 特征_OR组领取路径(), 领取路径, now);
            重复领取 = true;
            提交状态 = OR组令牌提交状态_重复领取;
        }

        const bool 成功 = 输入有效 && (首次领取 || 重复领取);
        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入方法执行情况(宿主, 成功 ? 值_执行成功() : 值_执行失败(), now);
            if (OR组) {
                (void)写入基础节点指针(宿主, 特征_OR组节点(), OR组, now);
            }
            if (领取路径) {
                (void)写入基础节点指针(宿主, 特征_OR组领取路径(), 领取路径, now);
            }
            if (令牌记录) {
                (void)写入基础节点指针(宿主, 特征_OR组结算令牌记录(), 令牌记录, now);
            }
            (void)写入基础节点I64(宿主, 特征_OR组结算令牌状态(), 成功 ? OR组结算令牌状态_已领取 : OR组结算令牌状态_未领取, now);
            (void)写入基础节点I64(宿主, 特征_OR组令牌提交状态(), 提交状态, now);
            (void)写入基础节点I64(宿主, 特征_OR组令牌重复领取状态(), 重复领取 ? 1 : 0, now);
            if (令牌动态) {
                (void)写入基础节点指针(宿主, 特征_动作动态(), 令牌动态, now);
                (void)写入基础节点指针(宿主, 特征_OR组令牌领取动作动态(), 令牌动态, now);
            }
            if (!输入有效) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 值_必需参数缺失(), now);
            }
        };
        写结果(运行宿主);
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景));

        {
            std::ostringstream 日志;
            日志 << "提交_领取OR组结算令牌"
                << " | OR组=" << (OR组 ? OR组->获取主键() : std::string("空"))
                << " | 路径=" << (领取路径 ? 领取路径->获取主键() : std::string("空"))
                << " | 任务=" << (任务 ? 任务->获取主键() : std::string("空"))
                << " | 输入有效=" << (输入有效 ? 1 : 0)
                << " | 首次领取=" << (首次领取 ? 1 : 0)
                << " | 重复领取=" << (重复领取 ? 1 : 0)
                << " | OR组已收束=" << (OR组已收束 ? 1 : 0)
                << " | 令牌动态=" << (令牌动态 ? 令牌动态->获取主键() : std::string("缺"))
                << " | 令牌状态不写需求主信息=1";
            项目运行日志(日志.str());
        }

        auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
        更新方法运行账(环境.方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

}
