module;

#include <cstdint>
#include <algorithm>
#include <limits>
#include <optional>
#include <vector>

#include "本能方法类.h"
#include "世界树类.h"
#include "方法类.h"
#include "语素类.h"

export module 自我动作实现模块;

import 自我模块;
import 本能动作管理模块;

namespace {
    inline const 词性节点类* 特征_条件描述状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法条件自描述状态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_条件描述时间() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法条件自描述时间", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_条件特征类型数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法条件特征类型数量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_必需条件特征类型数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法必需条件特征类型数量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_可选条件特征类型数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法可选条件特征类型数量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_运行参数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("运行参数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_条件特征() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("条件特征", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_条件集合() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("条件集合", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_输入条件包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("输入条件包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_形参表() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("形参表", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_固定参数表() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("固定参数表", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_结果表() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("结果表", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_效果表() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("效果表", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_调用信息() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("调用信息", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_任务虚拟存在() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务虚拟存在", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_现实场景包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("现实场景包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_任务参数包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("任务参数包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_快照包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("快照包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_目标现实场景() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标现实场景", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_目标特征类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标特征类型", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_当前值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("当前值", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_改变方向() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("改变方向", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_已绑定实参表() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("已绑定实参表", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_参数来源() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("参数来源", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_参数值类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("参数值类型", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_读取特征类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("读取特征类型", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_是否必需() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("是否必需", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_运行时长() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("运行时长", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_运行次数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("运行次数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_成功次数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("成功次数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_失败次数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("失败次数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_累计运行时长() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("累计运行时长", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_平均运行时长() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("平均运行时长", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_最近运行虚拟存在() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("最近运行虚拟存在", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_来源_输入现实场景包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源_输入现实场景包", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_来源_输入任务参数包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源_输入任务参数包", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_来源_输入条件包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源_输入条件包", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_来源_方法固定参数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源_方法固定参数", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_来源_目标现实场景() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源_目标现实场景", "名词");
        return s_词;
    }

    inline const 词性节点类* 值类型_I64() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("值类型_I64", "名词");
        return s_词;
    }

    inline const 词性节点类* 值类型_类型I64() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("类型_I64", "名词");
        return s_词;
    }

    inline const 词性节点类* 值类型_场景指针() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("值类型_场景指针", "名词");
        return s_词;
    }

    inline const 词性节点类* 值类型_特征类型指针() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("值类型_特征类型指针", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法执行情况() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法执行情况", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_参数绑定失败() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("参数绑定失败", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_执行成功() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("执行成功", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_执行失败() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("执行失败", "名词");
        return s_词;
    }

    inline const 词性节点类* 类型_运行虚拟存在() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("运行虚拟存在", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_输入参数场景() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("输入参数场景", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_输出结果场景() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("输出结果场景", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_返回参数信息() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("返回参数信息", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_返回场景信息() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("返回场景信息", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法虚拟存在() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法虚拟存在", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_开始时间() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("开始时间", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_结束时间() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("结束时间", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_执行前值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("执行前值", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_执行后值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("执行后值", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_动作动态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("动作动态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法条件节点() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法条件节点", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法结果节点() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法结果节点", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_安全值() noexcept
    {
        return 自我特征定义类::类型_自我_安全值();
    }

    inline const 词性节点类* 特征_改变量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("改变量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_服务值() noexcept
    {
        return 自我特征定义类::类型_自我_服务值();
    }

    inline const 词性节点类* 特征_待学习方法数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("待学习方法数量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_可用方法数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("可用方法数量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_需求条件包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("需求条件包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_查找限制包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("查找限制包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_候选范围包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("候选范围包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法搜索根() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法搜索根", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法能力包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法能力包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_特征类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("特征类型", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_特征值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("特征值", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_特征值范围() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("特征值范围", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_区间下界() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("区间下界", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_区间上界() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("区间上界", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_抽象特征引用() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("抽象特征引用", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_存在引用() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("存在引用", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_场景引用() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("场景引用", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_最高允许目标特征类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("最高允许目标特征类型", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_最高允许宿主类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("最高允许宿主类型", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_允许值类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("允许值类型", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_允许执行差值区间() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("允许执行差值区间", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_最大类型抽象层数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("最大类型抽象层数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_最大值域抽象层数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("最大值域抽象层数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_最大候选数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("最大候选数量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_最大查找递归深度() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("最大查找递归深度", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_是否包含禁用方法() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("是否包含禁用方法", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_是否允许当前方法自身() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("是否允许当前方法自身", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_需要方法能力包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("需要方法能力包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法查找情况() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法查找情况", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法配对结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法配对结果", "名词");
        return s_词;
    }

    inline const 词性节点类* 类型_方法配对结果存在() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法配对结果存在", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_配对状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("配对状态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_来源需求条件包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源需求条件包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_使用限制包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("使用限制包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_选中方法首节点() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("选中方法首节点", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_选中自我能力值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("选中自我能力值", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_缓存方法首节点() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("缓存方法首节点", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_缓存方法虚拟存在() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("缓存方法虚拟存在", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_选中方法虚拟存在() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("选中方法虚拟存在", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_匹配方法能力包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("匹配方法能力包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_原始需求绑定包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("原始需求绑定包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_匹配路径记录() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("匹配路径记录", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_类型抽象距离() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("类型抽象距离", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_值域抽象距离() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("值域抽象距离", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_总匹配距离() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("总匹配距离", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_待配对形参表() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("待配对形参表", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_候选扫描数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("候选扫描数量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_成功匹配数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("成功匹配数量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_失败原因() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("失败原因", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_使用默认方法搜索根() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("使用默认方法搜索根", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_输入绑定失败() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("输入绑定失败", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_配对失败() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("配对失败", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_超出限制() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("超出限制", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_配对成功() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("配对成功", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_没有匹配方法() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("没有匹配方法", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_候选方法不可用() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("候选方法不可用", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_方法能力包缺失() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法能力包缺失", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_输入条件包绑定失败() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("输入条件包绑定失败", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_方法虚拟存在缺失() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法虚拟存在缺失", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_方法配对结果创建失败() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法配对结果创建失败", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_不修改现实场景特征值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("不修改现实场景特征值", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_不执行被查找到的方法() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("不执行被查找到的方法", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_目标宿主() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标宿主", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_目标抽象特征() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标抽象特征", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_目标值类型() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标值类型", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_差值约束包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("差值约束包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_执行差值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("执行差值", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_目标执行后值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标执行后值", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_执行前值快照() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("执行前值快照", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_特征类型配对结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("特征类型配对结果", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_实例缺失策略() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("实例缺失策略", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_溢出策略() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("溢出策略", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_限制包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("限制包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_目标实例特征() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标实例特征", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_实际执行差值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("实际执行差值", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_差值命中状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("差值命中状态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_是否发生溢出() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("是否发生溢出", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_当前命中抽象特征() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("当前命中抽象特征", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_派生需求() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("派生需求", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_目标实例特征缺失() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标实例特征缺失", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_执行前值缺失() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("执行前值缺失", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_输入快照过期() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("输入快照过期", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_I64溢出() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("I64溢出", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_差值计算溢出() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("差值计算溢出", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_缺少执行差值或目标值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("缺少执行差值或目标值", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_执行差值与目标值冲突() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("执行差值与目标值冲突", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_实际差值未命中约束() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("实际差值未命中约束", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_目标特征写入失败() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标特征写入失败", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_差值约束包缺失() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("差值约束包缺失", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_目标抽象特征无效() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标抽象特征无效", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_目标值类型不是I64() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标值类型不是I64", "名词");
        return s_词;
    }

    inline const 词性节点类* 类型_需求_实例化特征() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("需求_实例化特征", "名词");
        return s_词;
    }

    inline const 词性节点类* 类型_需求_确定特征当前值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("需求_确定特征当前值", "名词");
        return s_词;
    }

    inline const 词性节点类* 类型_需求_确定执行差值或目标值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("需求_确定执行差值或目标值", "名词");
        return s_词;
    }

    inline const 词性节点类* 类型_需求_重新配对参数或调整目标值() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("需求_重新配对参数或调整目标值", "名词");
        return s_词;
    }

    inline const 词性节点类* 类型_需求_修复实例特征写入能力() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("需求_修复实例特征写入能力", "名词");
        return s_词;
    }

    inline bool 写入I64(
        方法类::节点类* 方法首节点,
        const 词性节点类* 特征类型,
        I64 值,
        时间戳 now) noexcept
    {
        return 方法首节点
            && 特征类型
            && 方法类::记录方法虚拟存在特征(
                方法首节点,
                特征类型,
                特征值{ 值 },
                nullptr,
                now);
    }

    inline bool 写入存在I64(
        存在节点类* 存在,
        const 词性节点类* 特征类型,
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
        const 词性节点类* 特征类型,
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
        const 词性节点类* 特征类型,
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
        const 词性节点类* 特征类型,
        I64 值,
        时间戳 now) noexcept
    {
        return 宿主
            && 特征类型
            && 世界树.写入特征_I64(宿主, 特征类型, 值, now);
    }

    inline 特征节点类* 查找子特征(
        基础信息节点类* 宿主,
        const 词性节点类* 特征类型) noexcept
    {
        return 宿主 && 特征类型
            ? 世界树.特征().查找子特征_按类型(宿主, 特征类型)
            : nullptr;
    }

    inline 特征节点类* 取或创建子特征(
        基础信息节点类* 宿主,
        const 词性节点类* 特征类型) noexcept
    {
        return 宿主 && 特征类型
            ? 世界树.取或创建子特征_按类型(宿主, 特征类型)
            : nullptr;
    }

    inline bool 读取基础节点I64(
        基础信息节点类* 宿主,
        const 词性节点类* 特征类型,
        I64& 输出值) noexcept
    {
        输出值 = 0;
        if (!宿主 || !特征类型) return false;
        auto* 特征 = 查找子特征(宿主, 特征类型);
        return 特征 && 世界树.读取特征_I64(特征, 输出值);
    }

    inline bool 读取基础节点指针(
        基础信息节点类* 宿主,
        const 词性节点类* 特征类型,
        void*& 输出指针) noexcept
    {
        输出指针 = nullptr;
        if (!宿主 || !特征类型) return false;
        auto* 特征 = 查找子特征(宿主, 特征类型);
        return 特征 && 世界树.读取特征_指针(特征, 输出指针) && 输出指针;
    }

    inline bool 读取场景I64特征(
        场景节点类* 场景,
        const 词性节点类* 特征类型,
        I64& 输出值) noexcept
    {
        return 读取基础节点I64(
            reinterpret_cast<基础信息节点类*>(场景),
            特征类型,
            输出值);
    }

    inline 特征节点类* 输入包根(
        场景节点类* 输入参数场景,
        const 词性节点类* 包类型) noexcept
    {
        return 输入参数场景 && 包类型
            ? 查找子特征(
                reinterpret_cast<基础信息节点类*>(输入参数场景),
                包类型)
            : nullptr;
    }

    inline bool 读取输入包I64(
        场景节点类* 输入参数场景,
        const 词性节点类* 包类型,
        const 词性节点类* 参数类型,
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
        const 词性节点类* 包类型,
        const 词性节点类* 参数类型,
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
        return 查找子特征(宿主, 特征_条件集合());
    }

    inline bool 读取输入条件包I64(
        场景节点类* 输入参数场景,
        const 词性节点类* 参数类型,
        I64& 输出值) noexcept
    {
        auto* 条件包 = 输入条件包根(输入参数场景);
        return 条件包
            && 读取基础节点I64(
                reinterpret_cast<基础信息节点类*>(条件包),
                参数类型,
                输出值);
    }

    inline bool 读取输入条件包指针(
        场景节点类* 输入参数场景,
        const 词性节点类* 参数类型,
        void*& 输出指针) noexcept
    {
        auto* 条件包 = 输入条件包根(输入参数场景);
        return 条件包
            && 读取基础节点指针(
                reinterpret_cast<基础信息节点类*>(条件包),
                参数类型,
                输出指针);
    }

    inline bool 方向值有效(I64 方向) noexcept
    {
        return 方向 == -1 || 方向 == 0 || 方向 == 1;
    }

    inline bool 读取调用信息指针(
        场景节点类* 输入参数场景,
        const 词性节点类* 参数类型,
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
        return 方法首节点->主信息.方法虚拟存在.指针
            ? 方法首节点->主信息.方法虚拟存在.指针
            : 方法类::取或创建_方法虚拟存在(方法首节点, nullptr, now);
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
        return 方法首节点->主信息.条件场景.指针
            ? 方法首节点->主信息.条件场景.指针
            : 方法类::取或创建_方法条件场景(方法首节点, nullptr, now);
    }

    inline 场景节点类* 抽象结果场景(
        方法类::节点类* 方法首节点,
        时间戳 now) noexcept
    {
        if (!方法首节点) return nullptr;
        return 方法首节点->主信息.结果场景.指针
            ? 方法首节点->主信息.结果场景.指针
            : 方法类::取或创建_方法结果场景(方法首节点, nullptr, now);
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

    inline bool 声明条件特征(
        方法类::节点类* 方法首节点,
        const 词性节点类* 特征类型,
        时间戳 now) noexcept
    {
        auto* 条件根 = 抽象条件集合根(方法首节点, now);
        return 条件根
            && 特征类型
            && 世界树.取或创建子特征_按类型(
                reinterpret_cast<基础信息节点类*>(条件根),
                特征类型);
    }

    inline 特征节点类* 方法规格根(
        方法类::节点类* 方法首节点,
        const 词性节点类* 根特征类型,
        时间戳 now) noexcept
    {
        auto* 虚拟存在 = 方法虚拟存在(方法首节点, now);
        return 虚拟存在
            ? 取或创建子特征(reinterpret_cast<基础信息节点类*>(虚拟存在), 根特征类型)
            : nullptr;
    }

    inline bool 声明形参规格(
        方法类::节点类* 方法首节点,
        const 词性节点类* 参数特征类型,
        const 词性节点类* 参数值类型,
        const 词性节点类* 参数来源,
        bool 是否必需,
        时间戳 now,
        const 词性节点类* 读取特征类型 = nullptr) noexcept
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

    inline bool 确认改变I64特征方法规格(
        方法类::节点类* 方法首节点,
        const 词性节点类* 目标特征类型,
        int 改变方向,
        bool 需要改变量,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !目标特征类型) return false;
        (void)方法类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
        bool ok = true;

        ok = 声明形参规格(方法首节点, 特征_目标现实场景(), 值类型_场景指针(), 值_来源_输入现实场景包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前值(), 值类型_I64(), 值_来源_目标现实场景(), true, now, 目标特征类型) && ok;
        ok = 声明形参规格(方法首节点, 特征_改变量(), 值类型_I64(), 值_来源_输入任务参数包(), 需要改变量, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标特征类型(), 值类型_特征类型指针(), 值_来源_方法固定参数(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_改变方向(), 值类型_I64(), 值_来源_方法固定参数(), true, now) && ok;

        if (auto* 固定根 = 方法规格根(方法首节点, 特征_固定参数表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(固定根);
            ok = 写入基础节点指针(宿主, 特征_目标特征类型(), 目标特征类型, now) && ok;
            ok = 写入基础节点I64(宿主, 特征_改变方向(), static_cast<I64>(改变方向), now) && ok;
        }
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法执行情况()) && ok;
            ok = 取或创建子特征(宿主, 特征_执行前值()) && ok;
            ok = 取或创建子特征(宿主, 特征_执行后值()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
        }
        if (auto* 效果根 = 方法规格根(方法首节点, 特征_效果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(效果根);
            ok = 写入基础节点指针(宿主, 特征_目标现实场景(), 值_来源_输入现实场景包(), now) && ok;
            ok = 写入基础节点指针(宿主, 特征_目标特征类型(), 目标特征类型, now) && ok;
            ok = 写入基础节点指针(宿主, 特征_执行后值(), 特征_执行后值(), now) && ok;
        }

        // 方法树条件节点仍可建立，但只声明调用帧中显式提供的形参，不再声明“当前值”。
        (void)声明条件特征(方法首节点, 特征_目标现实场景(), now);
        if (需要改变量) (void)声明条件特征(方法首节点, 特征_改变量(), now);
        return ok;
    }

    inline bool 确认通用改变I64特征方法规格(
        方法类::节点类* 方法首节点,
        时间戳 now) noexcept
    {
        if (!方法首节点) return false;
        (void)方法类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
        bool ok = true;

        ok = 声明形参规格(方法首节点, 特征_目标现实场景(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标特征类型(), 值类型_特征类型指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_改变方向(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_改变量(), 值类型_I64(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前值(), 值类型_I64(), 值_来源_目标现实场景(), true, now) && ok;

        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法执行情况()) && ok;
            ok = 取或创建子特征(宿主, 特征_执行前值()) && ok;
            ok = 取或创建子特征(宿主, 特征_执行后值()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
        }
        if (auto* 效果根 = 方法规格根(方法首节点, 特征_效果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(效果根);
            ok = 写入基础节点指针(宿主, 特征_目标现实场景(), 值_来源_输入条件包(), now) && ok;
            ok = 写入基础节点指针(宿主, 特征_目标特征类型(), 值_来源_输入条件包(), now) && ok;
            ok = 写入基础节点指针(宿主, 特征_执行后值(), 特征_执行后值(), now) && ok;
        }

        (void)声明条件特征(方法首节点, 特征_目标现实场景(), now);
        (void)声明条件特征(方法首节点, 特征_目标特征类型(), now);
        (void)声明条件特征(方法首节点, 特征_改变方向(), now);
        (void)声明条件特征(方法首节点, 特征_改变量(), now);
        return ok;
    }

    struct 结构_改变I64特征绑定 {
        场景节点类* 目标现实场景 = nullptr;
        const 词性节点类* 目标特征类型 = nullptr;
        I64 当前值 = 0;
        I64 改变量 = 0;
        int 改变方向 = 0;
    };

    inline bool 写入已绑定实参表(
        存在节点类* 运行存在,
        const 结构_改变I64特征绑定& 绑定,
        时间戳 now) noexcept
    {
        if (!运行存在 || !绑定.目标现实场景 || !绑定.目标特征类型) return false;
        auto* 实参根 = 取或创建子特征(
            reinterpret_cast<基础信息节点类*>(运行存在),
            特征_已绑定实参表());
        if (!实参根) return false;

        auto* 宿主 = reinterpret_cast<基础信息节点类*>(实参根);
        bool ok = true;
        ok = 写入基础节点指针(宿主, 特征_目标现实场景(), 绑定.目标现实场景, now) && ok;
        ok = 写入基础节点指针(宿主, 特征_目标特征类型(), 绑定.目标特征类型, now) && ok;
        ok = 写入基础节点I64(宿主, 特征_当前值(), 绑定.当前值, now) && ok;
        ok = 写入基础节点I64(宿主, 特征_改变量(), 绑定.改变量, now) && ok;
        ok = 写入基础节点I64(宿主, 特征_改变方向(), static_cast<I64>(绑定.改变方向), now) && ok;
        ok = 写入存在指针(运行存在, 特征_目标现实场景(), 绑定.目标现实场景, now) && ok;
        return ok;
    }

    inline bool 变化调用帧已绑定(
        存在节点类* 运行存在,
        场景节点类* 输入参数场景,
        const 词性节点类* 目标特征类型,
        int 改变方向,
        bool 需要改变量,
        结构_改变I64特征绑定& 输出绑定,
        时间戳 now) noexcept
    {
        输出绑定 = {};
        if (!输入参数场景 || !目标特征类型) return false;

        void* 目标现实场景指针 = nullptr;
        if (!读取输入包指针(
            输入参数场景,
            特征_现实场景包(),
            特征_目标现实场景(),
            目标现实场景指针)) {
            return false;
        }

        auto* 目标现实场景 = static_cast<场景节点类*>(目标现实场景指针);
        I64 改变量 = 0;
        if (需要改变量) {
            if (!读取输入包I64(
                输入参数场景,
                特征_任务参数包(),
                特征_改变量(),
                改变量)) {
                return false;
            }
        }
        else {
            (void)读取输入包I64(
                输入参数场景,
                特征_任务参数包(),
                特征_改变量(),
                改变量);
        }

        I64 当前值 = 0;
        if (!读取场景I64特征(目标现实场景, 目标特征类型, 当前值)) {
            return false;
        }

        输出绑定.目标现实场景 = 目标现实场景;
        输出绑定.目标特征类型 = 目标特征类型;
        输出绑定.当前值 = 当前值;
        输出绑定.改变量 = 改变量;
        输出绑定.改变方向 = 改变方向;
        return 写入已绑定实参表(运行存在, 输出绑定, now);
    }

    inline bool 变化条件包已绑定(
        存在节点类* 运行存在,
        场景节点类* 输入参数场景,
        结构_改变I64特征绑定& 输出绑定,
        时间戳 now) noexcept
    {
        输出绑定 = {};
        if (!输入参数场景) return false;

        void* 目标现实场景指针 = nullptr;
        void* 目标特征类型指针 = nullptr;
        I64 改变方向值 = 0;
        I64 改变量 = 0;

        if (!读取输入条件包指针(
            输入参数场景,
            特征_目标现实场景(),
            目标现实场景指针)) {
            return false;
        }
        if (!读取输入条件包指针(
            输入参数场景,
            特征_目标特征类型(),
            目标特征类型指针)) {
            return false;
        }
        if (!读取输入条件包I64(
            输入参数场景,
            特征_改变方向(),
            改变方向值)) {
            return false;
        }
        if (!方向值有效(改变方向值)) {
            return false;
        }
        if (!读取输入条件包I64(
            输入参数场景,
            特征_改变量(),
            改变量)) {
            return false;
        }

        auto* 目标现实场景 = static_cast<场景节点类*>(目标现实场景指针);
        auto* 目标特征类型 = static_cast<const 词性节点类*>(目标特征类型指针);
        if (!目标现实场景 || !目标特征类型) return false;

        I64 当前值 = 0;
        if (!读取场景I64特征(目标现实场景, 目标特征类型, 当前值)) {
            return false;
        }

        输出绑定.目标现实场景 = 目标现实场景;
        输出绑定.目标特征类型 = 目标特征类型;
        输出绑定.当前值 = 当前值;
        输出绑定.改变量 = 改变量;
        输出绑定.改变方向 = static_cast<int>(改变方向值);
        return 写入已绑定实参表(运行存在, 输出绑定, now);
    }

    inline bool 写入方法执行情况(
        基础信息节点类* 宿主,
        const 词性节点类* 情况,
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

    inline 结构_方法条件结果节点 构建方法条件和结果节点(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept
    {
        (void)输入参数场景;
        (void)输出结果场景;
        结构_方法条件结果节点 节点{};
        if (!方法首节点) return 节点;
        auto* 条件场景 = 抽象条件场景(方法首节点, now);
        auto* 结果场景 = 抽象结果场景(方法首节点, now);
        if (条件场景) {
            节点.条件节点 = 方法类::创建条件节点(
                方法首节点,
                条件场景,
                {},
                {},
                now,
                now);
        }
        if (结果场景) {
            节点.结果节点 = 方法类::创建结果节点(
                方法首节点,
                结果场景,
                {});
        }
        return 节点;
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
        const 词性节点类* 特征类型,
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

    inline bool 写入场景I64特征(
        场景节点类* 场景,
        const 词性节点类* 特征类型,
        I64 值,
        时间戳 now) noexcept
    {
        if (!场景 || !特征类型) return false;
        auto* 特征 = 世界树.取或创建子特征_按类型(
            reinterpret_cast<基础信息节点类*>(场景),
            特征类型);
        return 特征 && 世界树.写入特征_I64(特征, 值, now);
    }

    inline I64 按方向改变(I64 当前值, I64 改变量, int 方向) noexcept
    {
        if (方向 > 0) return 饱和增加(当前值, 改变量);
        if (方向 < 0) return 饱和增加(当前值, -改变量);
        return 当前值;
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

    inline 特征节点类* 场景特征(
        场景节点类* 场景,
        const 词性节点类* 特征类型) noexcept
    {
        return 场景 && 特征类型
            ? 世界树.取或创建子特征_按类型(
                reinterpret_cast<基础信息节点类*>(场景),
                特征类型)
            : nullptr;
    }

    inline 状态节点类* 创建场景状态(
        场景节点类* 场景,
        特征节点类* 特征,
        I64 值,
        bool 是否变化,
        时间戳 now) noexcept
    {
        if (!场景 || !特征) return nullptr;
        auto* 状态 = 世界树.状态().创建状态(
            场景,
            reinterpret_cast<基础信息节点类*>(场景),
            特征,
            特征值{ 值 },
            now,
            now);
        if (状态) {
            (void)世界树.状态().标记变化(状态, 是否变化);
        }
        return 状态;
    }

    inline 动态节点类* 输出动作动态(
        方法类::节点类* 方法首节点,
        场景节点类* 场景,
        特征节点类* 特征,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        bool 成功,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !场景 || !特征) return nullptr;
        return 世界树.动态().创建方法动作动态(
            场景,
            reinterpret_cast<基础信息节点类*>(场景),
            特征,
            reinterpret_cast<方法节点类*>(方法首节点),
            方法首节点->主信息.动作名,
            now,
            now,
            成功,
            0,
            成功 ? 枚举_动作事件相位::完成 : 枚举_动作事件相位::失败,
            输入参数场景,
            输出结果场景);
    }

    inline 存在节点类* 输出运行虚拟存在(
        存在节点类* 运行存在,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        时间戳 now) noexcept;

    inline 存在节点类* 输出变化执行(
        方法类::节点类* 方法首节点,
        存在节点类* 运行存在,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        场景节点类* 目标现实场景,
        const 词性节点类* 特征类型,
        I64 执行前值,
        I64 执行后值,
        bool 成功,
        const 词性节点类* 执行情况,
        时间戳 now) noexcept
    {
        const auto* 情况 = 执行情况
            ? 执行情况
            : (成功 ? 值_执行成功() : 值_执行失败());
        auto 条件结果节点 = 构建方法条件和结果节点(
            方法首节点,
            输入参数场景,
            输出结果场景,
            now);

        auto* 场景 = 目标现实场景;
        auto* 特征 = 场景特征(场景, 特征类型);
        动态节点类* 动态 = nullptr;
        if (成功 && 场景 && 特征) {
            (void)创建场景状态(场景, 特征, 执行前值, false, now);
            (void)创建场景状态(场景, 特征, 执行后值, 执行前值 != 执行后值, now);
        }
        if (场景 && 特征) {
            动态 = 输出动作动态(
                方法首节点,
                场景,
                特征,
                输入参数场景,
                输出结果场景,
                成功,
                now);
        }

        if (运行存在) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(运行存在),
                情况,
                now);
            (void)写入存在I64(运行存在, 特征_执行前值(), 执行前值, now);
            (void)写入存在I64(运行存在, 特征_执行后值(), 执行后值, now);
            (void)写入存在指针(运行存在, 特征_动作动态(), 动态, now);
            (void)写入存在指针(运行存在, 特征_方法条件节点(), 条件结果节点.条件节点, now);
            (void)写入存在指针(运行存在, 特征_方法结果节点(), 条件结果节点.结果节点, now);
        }

        if (输出结果场景) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                情况,
                now);
            if (成功 && 特征类型) {
                (void)写入场景I64特征(
                    输出结果场景,
                    特征类型,
                    执行后值,
                    now);
            }
            (void)写入基础节点指针(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                特征_动作动态(),
                动态,
                now);
        }

        if (方法首节点) {
            (void)写入I64(方法首节点, 特征_执行前值(), 执行前值, now);
            (void)写入I64(方法首节点, 特征_执行后值(), 执行后值, now);
            if (auto* 虚拟存在 = 方法虚拟存在(方法首节点, now)) {
                (void)写入方法执行情况(
                    reinterpret_cast<基础信息节点类*>(虚拟存在),
                    情况,
                    now);
                (void)写入基础节点指针(
                    reinterpret_cast<基础信息节点类*>(虚拟存在),
                    特征_动作动态(),
                    动态,
                    now);
            }
        }

        return 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
    }

    inline bool 声明运行参数(
        方法类::节点类* 方法首节点,
        const 词性节点类* 参数类型,
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

        bool 已写入 = 方法类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
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
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        结构体_动作句柄 动作句柄{};
        动作句柄.类型 = 枚举_动作句柄类型::本能函数ID;
        动作句柄.本能ID = static_cast<I64>(本能ID);

        if (方法树锚点->主信息.节点种类 == 枚举_方法节点种类::方法首节点
            && 方法树锚点->主信息.动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && 方法树锚点->主信息.动作句柄.本能ID == static_cast<I64>(本能ID)) {
            return 方法树锚点;
        }

        auto* 根节点 = 方法树根(方法树锚点);
        if (!根节点) return nullptr;
        if (auto* 已有方法 = 方法类::查找方法首节点_按动作句柄(根节点, 动作句柄)) {
            return 已有方法;
        }

        const auto* 元信息 = 本能方法类::查询元信息(本能ID);
        if (!元信息) return nullptr;

        auto* 动作名 = 语素集.添加词性词(元信息->动作名称 ? 元信息->动作名称 : "", "动词");
        auto* 新方法 = 方法类::注册方法首节点(
            根节点,
            动作名,
            动作句柄,
            枚举_方法作用域::主体局部,
            now);
        if (!新方法) return nullptr;

        (void)方法类::补齐本能方法首节点(新方法, *元信息, nullptr, now);
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


    inline void 更新方法运行统计(
        方法类::节点类* 方法首节点,
        存在节点类* 运行存在,
        bool 成功,
        时间戳 now) noexcept
    {
        auto* 虚拟存在 = 方法虚拟存在(方法首节点, now);
        if (!虚拟存在) return;

        auto* 宿主 = reinterpret_cast<基础信息节点类*>(虚拟存在);
        I64 运行次数 = 0;
        I64 成功次数 = 0;
        I64 失败次数 = 0;
        I64 累计运行时长 = 0;
        I64 本次运行时长 = 0;

        (void)读取基础节点I64(宿主, 特征_运行次数(), 运行次数);
        (void)读取基础节点I64(宿主, 特征_成功次数(), 成功次数);
        (void)读取基础节点I64(宿主, 特征_失败次数(), 失败次数);
        (void)读取基础节点I64(宿主, 特征_累计运行时长(), 累计运行时长);
        if (运行存在) {
            (void)读取基础节点I64(
                reinterpret_cast<基础信息节点类*>(运行存在),
                特征_运行时长(),
                本次运行时长);
        }

        运行次数 = 饱和增加(运行次数, 1);
        if (成功) 成功次数 = 饱和增加(成功次数, 1);
        else 失败次数 = 饱和增加(失败次数, 1);
        累计运行时长 = 饱和增加(累计运行时长, 本次运行时长);
        const I64 平均运行时长 = 运行次数 > 0 ? 累计运行时长 / 运行次数 : 0;

        (void)写入基础节点I64(宿主, 特征_运行次数(), 运行次数, now);
        (void)写入基础节点I64(宿主, 特征_成功次数(), 成功次数, now);
        (void)写入基础节点I64(宿主, 特征_失败次数(), 失败次数, now);
        (void)写入基础节点I64(宿主, 特征_累计运行时长(), 累计运行时长, now);
        (void)写入基础节点I64(宿主, 特征_平均运行时长(), 平均运行时长, now);
        (void)写入基础节点I64(宿主, 特征_运行时长(), 本次运行时长, now);
        if (运行存在) {
            (void)写入基础节点指针(宿主, 特征_最近运行虚拟存在(), 运行存在, now);
        }
    }

    inline constexpr I64 方法查找情况_输入绑定失败 = 1;
    inline constexpr I64 方法查找情况_配对失败 = 2;
    inline constexpr I64 方法查找情况_超出限制 = 3;
    inline constexpr I64 方法查找情况_配对成功 = 4;

    struct 结构_查找限制 {
        基础信息节点类* 最高允许目标特征类型 = nullptr;
        const 词性节点类* 最高允许目标特征类型词 = nullptr;
        基础信息节点类* 最高允许宿主类型 = nullptr;
        const 词性节点类* 最高允许宿主类型词 = nullptr;
        const 词性节点类* 允许值类型 = nullptr;
        bool 有允许执行差值区间 = true;
        I64 允许执行差值下界 = (std::numeric_limits<I64>::min)();
        I64 允许执行差值上界 = (std::numeric_limits<I64>::max)();
        I64 最大类型抽象层数 = 8;
        I64 最大值域抽象层数 = 8;
        I64 最大候选数量 = 256;
        I64 最大查找递归深度 = 64;
        bool 是否包含禁用方法 = false;
        bool 是否允许当前方法自身 = false;
        bool 需要方法能力包 = true;
    };

    struct 结构_条件项视图 {
        const 词性节点类* 条件特征类型 = nullptr;
        bool 有I64点值 = false;
        I64 I64点值 = 0;
        bool 有I64区间 = false;
        I64 区间下界 = 0;
        I64 区间上界 = 0;
        基础信息节点类* 抽象特征引用 = nullptr;
        基础信息节点类* 存在引用 = nullptr;
        场景节点类* 场景引用 = nullptr;
        基础信息节点类* 基础节点引用 = nullptr;
        const 词性节点类* 词性值 = nullptr;
        bool 必需 = true;
    };

    struct 结构_查找方法输入帧 {
        基础信息节点类* 输入条件包 = nullptr;
        基础信息节点类* 需求条件包 = nullptr;
        基础信息节点类* 查找限制包 = nullptr;
        基础信息节点类* 候选范围包 = nullptr;
        方法类::节点类* 方法搜索根 = nullptr;
        bool 使用默认方法搜索根 = false;
        结构_查找限制 限制{};
        std::vector<结构_条件项视图> 需求项列表{};
    };

    struct 结构_类型匹配结果 {
        bool 成功 = false;
        I64 抽象距离 = 0;
        const 词性节点类* 失败原因 = nullptr;
        std::vector<基础信息节点类*> 路径{};
    };

    struct 结构_值匹配结果 {
        bool 成功 = false;
        I64 抽象距离 = 0;
        I64 能力宽度 = 0;
        const 词性节点类* 失败原因 = nullptr;
        std::vector<基础信息节点类*> 路径{};
    };

    struct 结构_条件项匹配结果 {
        bool 成功 = false;
        I64 类型抽象距离 = 0;
        I64 值域抽象距离 = 0;
        I64 总距离 = 0;
        I64 能力宽度 = 0;
        const 词性节点类* 失败原因 = nullptr;
        std::vector<基础信息节点类*> 类型路径{};
        std::vector<基础信息节点类*> 值域路径{};
    };

    struct 结构_方法匹配结果 {
        bool 成功 = false;
        I64 总类型抽象距离 = 0;
        I64 总值域抽象距离 = 0;
        I64 总匹配距离 = 0;
        I64 能力宽度 = 0;
        const 词性节点类* 失败原因 = nullptr;
        结构_条件项视图 失败需求项{};
        std::vector<结构_条件项匹配结果> 条件项结果{};
    };

    struct 结构_候选方法结果 {
        bool 有效 = false;
        方法类::节点类* 方法首节点 = nullptr;
        存在节点类* 方法虚拟存在 = nullptr;
        基础信息节点类* 方法能力包 = nullptr;
        结构_方法匹配结果 匹配{};
        I64 自我能力值 = 0;
        bool 有自我能力值 = false;
        I64 方法成功率 = 0;
        I64 平均运行时长 = 0;
    };

    enum class 枚举_实例缺失策略_改变 : std::uint8_t {
        失败 = 0,
        允许定义实例化 = 1,
        允许观测实例化 = 2
    };

    enum class 枚举_溢出策略_改变 : std::uint8_t {
        不允许溢出 = 0,
        饱和写入 = 1
    };

    inline constexpr I64 差值命中状态_未命中 = 0;
    inline constexpr I64 差值命中状态_命中 = 1;

    struct 结构_改变特征值输入帧 {
        基础信息节点类* 输入条件包 = nullptr;
        基础信息节点类* 目标宿主 = nullptr;
        基础信息节点类* 目标抽象特征 = nullptr;
        特征节点类* 目标实例特征 = nullptr;

        bool 目标值类型为I64 = false;

        bool 有执行差值 = false;
        I64 执行差值 = 0;

        bool 有目标执行后值 = false;
        I64 目标执行后值 = 0;

        bool 有执行前值快照 = false;
        I64 执行前值快照 = 0;

        std::vector<I64区间> 差值约束区间{};
        基础信息节点类* 差值约束包 = nullptr;

        枚举_实例缺失策略_改变 实例缺失策略 = 枚举_实例缺失策略_改变::失败;
        枚举_溢出策略_改变 溢出策略 = 枚举_溢出策略_改变::不允许溢出;

        基础信息节点类* 特征类型配对结果 = nullptr;
        基础信息节点类* 限制包 = nullptr;
        const 词性节点类* 绑定失败原因 = nullptr;
    };

    struct 结构_安全I64结果 {
        I64 值 = 0;
        bool 溢出 = false;
    };

    struct 结构_I64差值结果 {
        I64 差值 = 0;
        bool 溢出 = false;
    };

    inline 结构_安全I64结果 安全增加I64(I64 左值, I64 右值) noexcept
    {
        if (右值 > 0 && 左值 > (std::numeric_limits<I64>::max)() - 右值) {
            return { (std::numeric_limits<I64>::max)(), true };
        }
        if (右值 < 0 && 左值 < (std::numeric_limits<I64>::min)() - 右值) {
            return { (std::numeric_limits<I64>::min)(), true };
        }
        return { static_cast<I64>(左值 + 右值), false };
    }

    inline 结构_I64差值结果 安全差值I64(I64 左值, I64 右值) noexcept
    {
        if (右值 < 0 && 左值 > (std::numeric_limits<I64>::max)() + 右值) {
            return { (std::numeric_limits<I64>::max)(), true };
        }
        if (右值 > 0 && 左值 < (std::numeric_limits<I64>::min)() + 右值) {
            return { (std::numeric_limits<I64>::min)(), true };
        }
        return { static_cast<I64>(左值 - 右值), false };
    }

    inline const 词性节点类* 基础节点类型词(const 基础信息节点类* 节点) noexcept
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

    inline 抽象特征节点类* 查找抽象特征_按类型(const 词性节点类* 特征类型) noexcept
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
        const 词性节点类* 目标类型) noexcept
    {
        if (!节点) return false;
        if (目标节点 && 节点 == 目标节点) return true;
        const auto* 主信息 = 世界树.特征().取抽象特征主信息(
            static_cast<const 抽象特征节点类*>(节点));
        return 主信息 && 目标类型 && 主信息->类型 == 目标类型;
    }

    inline bool I64区间包含(I64 能力下界, I64 能力上界, I64 需求下界, I64 需求上界) noexcept
    {
        return 能力下界 <= 需求下界 && 需求上界 <= 能力上界;
    }

    inline I64 饱和区间宽度(I64 下界, I64 上界) noexcept
    {
        if (上界 < 下界) return 0;
        if (下界 < 0 && 上界 > (std::numeric_limits<I64>::max)() + 下界) {
            return (std::numeric_limits<I64>::max)();
        }
        return 上界 - 下界;
    }

    inline 基础信息节点类* 查找包子节点(
        基础信息节点类* 包,
        const 词性节点类* 子类型) noexcept
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
        const 词性节点类* 子类型,
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

    inline void 吸收条件项指针值(结构_条件项视图& 项, const void* 指针) noexcept
    {
        if (!指针) return;
        if (auto* 节点 = 指针若为基础信息节点(指针)) {
            if (世界树.特征().取抽象特征主信息(static_cast<抽象特征节点类*>(节点))) {
                项.抽象特征引用 = 节点;
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
        项.词性值 = reinterpret_cast<const 词性节点类*>(指针);
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

    inline const 词性节点类* 读取条件项特征类型(
        基础信息节点类* 条件项节点,
        const 词性节点类* 默认类型) noexcept
    {
        void* 指针 = nullptr;
        if (读取包指针(条件项节点, 特征_特征类型(), 指针)) {
            if (auto* 节点 = 指针若为基础信息节点(指针)) {
                return 基础节点类型词(节点);
            }
            return reinterpret_cast<const 词性节点类*>(指针);
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
            || 项.词性值;
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

    inline std::vector<I64区间> 抽象特征区间集合(基础信息节点类* 抽象特征) noexcept
    {
        std::vector<I64区间> 区间集合{};
        auto* 主信息 = 世界树.特征().取抽象特征主信息(
            static_cast<抽象特征节点类*>(抽象特征));
        if (!主信息) return 区间集合;

        if (主信息->区间.has_value() && 主信息->区间->有效()) {
            区间集合.push_back(*主信息->区间);
        }
        for (const auto& 规则 : 主信息->组合规则) {
            if (!规则.已启用 || 规则.组合方式 != 枚举_抽象特征组合方式::任一命中) {
                continue;
            }
            for (const auto& 输入 : 规则.输入) {
                if (输入.期望区间.has_value() && 输入.期望区间->有效()) {
                    区间集合.push_back(*输入.期望区间);
                    continue;
                }
                auto* 期望状态 = 输入.期望抽象状态.取指针<抽象特征节点类>();
                auto* 期望主信息 = 世界树.特征().取抽象特征主信息(期望状态);
                if (期望主信息 && 期望主信息->区间.has_value() && 期望主信息->区间->有效()) {
                    区间集合.push_back(*期望主信息->区间);
                }
            }
        }
        return 区间集合;
    }

    inline std::vector<I64区间> 条件项区间集合(const 结构_条件项视图& 项) noexcept
    {
        std::vector<I64区间> 区间集合{};
        if (项.有I64区间) {
            区间集合.push_back(I64区间{ 项.区间下界, 项.区间上界 });
        }
        else if (项.有I64点值) {
            区间集合.push_back(I64区间::点(项.I64点值));
        }
        if (项.抽象特征引用) {
            auto 抽象区间 = 抽象特征区间集合(项.抽象特征引用);
            区间集合.insert(区间集合.end(), 抽象区间.begin(), 抽象区间.end());
        }
        return 区间集合;
    }

    inline 结构_类型匹配结果 抽象节点可覆盖(
        基础信息节点类* 需求节点,
        基础信息节点类* 能力节点,
        const 词性节点类* 能力类型,
        I64 最大层数,
        基础信息节点类* 最高允许节点,
        const 词性节点类* 最高允许类型) noexcept
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

    inline 结构_类型匹配结果 特征类型可覆盖(
        const 词性节点类* 需求类型,
        const 词性节点类* 能力范围类型,
        const 结构_查找限制& 限制) noexcept
    {
        结构_类型匹配结果 r{};
        if (!需求类型 || !能力范围类型) {
            r.失败原因 = 值_没有匹配方法();
            return r;
        }
        if (需求类型 == 能力范围类型) {
            r.成功 = true;
            return r;
        }

        auto* 需求抽象 = 查找抽象特征_按类型(需求类型);
        auto* 能力抽象 = 查找抽象特征_按类型(能力范围类型);
        return 抽象节点可覆盖(
            需求抽象,
            能力抽象,
            能力范围类型,
            限制.最大类型抽象层数,
            限制.最高允许目标特征类型,
            限制.最高允许目标特征类型词);
    }

    inline 结构_值匹配结果 抽象值可覆盖(
        基础信息节点类* 需求节点,
        基础信息节点类* 能力节点,
        const 词性节点类* 能力类型,
        const 结构_查找限制& 限制) noexcept
    {
        结构_值匹配结果 r{};
        auto 类型结果 = 抽象节点可覆盖(
            需求节点,
            能力节点,
            能力类型,
            限制.最大值域抽象层数,
            限制.最高允许目标特征类型,
            限制.最高允许目标特征类型词);
        r.成功 = 类型结果.成功;
        r.抽象距离 = 类型结果.抽象距离;
        r.失败原因 = 类型结果.失败原因;
        r.路径 = std::move(类型结果.路径);
        return r;
    }

    inline bool 区间集合被覆盖(
        const std::vector<I64区间>& 需求区间集合,
        const std::vector<I64区间>& 能力区间集合) noexcept
    {
        if (需求区间集合.empty() || 能力区间集合.empty()) return false;
        for (const auto& 需求区间 : 需求区间集合) {
            bool 当前被覆盖 = false;
            for (const auto& 能力区间 : 能力区间集合) {
                if (I64区间包含(
                        能力区间.低值,
                        能力区间.高值,
                        需求区间.低值,
                        需求区间.高值)) {
                    当前被覆盖 = true;
                    break;
                }
            }
            if (!当前被覆盖) return false;
        }
        return true;
    }

    inline I64 区间集合宽度(const std::vector<I64区间>& 区间集合) noexcept
    {
        I64 宽度 = 0;
        for (const auto& 区间 : 区间集合) {
            宽度 = 饱和增加(宽度, 饱和区间宽度(区间.低值, 区间.高值));
        }
        return 宽度;
    }

    inline 结构_值匹配结果 特征值约束可覆盖(
        const 结构_条件项视图& 需求项,
        const 结构_条件项视图& 能力项,
        const 结构_查找限制& 限制) noexcept
    {
        结构_值匹配结果 r{};
        const auto 需求区间 = 条件项区间集合(需求项);
        const auto 能力区间 = 条件项区间集合(能力项);
        if (!需求区间.empty() || !能力区间.empty()) {
            if (区间集合被覆盖(需求区间, 能力区间)) {
                r.成功 = true;
                r.能力宽度 = 区间集合宽度(能力区间);
                return r;
            }
            r.失败原因 = 值_没有匹配方法();
            return r;
        }

        if (需求项.词性值 && 能力项.词性值) {
            auto 类型结果 = 特征类型可覆盖(需求项.词性值, 能力项.词性值, 限制);
            r.成功 = 类型结果.成功;
            r.抽象距离 = 类型结果.抽象距离;
            r.失败原因 = 类型结果.失败原因;
            r.路径 = std::move(类型结果.路径);
            return r;
        }
        if (需求项.词性值 && 能力项.抽象特征引用) {
            auto* 需求抽象 = 查找抽象特征_按类型(需求项.词性值);
            const auto* 能力主信息 = 世界树.特征().取抽象特征主信息(
                static_cast<抽象特征节点类*>(能力项.抽象特征引用));
            return 抽象值可覆盖(
                需求抽象,
                能力项.抽象特征引用,
                能力主信息 ? 能力主信息->类型 : nullptr,
                限制);
        }
        if (需求项.抽象特征引用 && 能力项.词性值) {
            return 抽象值可覆盖(
                需求项.抽象特征引用,
                查找抽象特征_按类型(能力项.词性值),
                能力项.词性值,
                限制);
        }
        if (需求项.抽象特征引用 && 能力项.抽象特征引用) {
            return 抽象值可覆盖(
                需求项.抽象特征引用,
                能力项.抽象特征引用,
                nullptr,
                限制);
        }

        const auto* 需求节点类型 = 基础节点类型词(
            需求项.场景引用
                ? reinterpret_cast<基础信息节点类*>(需求项.场景引用)
                : (需求项.存在引用 ? 需求项.存在引用 : 需求项.基础节点引用));
        if (需求节点类型 && 能力项.抽象特征引用) {
            const auto* 能力主信息 = 世界树.特征().取抽象特征主信息(
                static_cast<抽象特征节点类*>(能力项.抽象特征引用));
            auto 类型结果 = 特征类型可覆盖(
                需求节点类型,
                能力主信息 ? 能力主信息->类型 : nullptr,
                限制);
            r.成功 = 类型结果.成功;
            r.抽象距离 = 类型结果.抽象距离;
            r.失败原因 = 类型结果.失败原因;
            r.路径 = std::move(类型结果.路径);
            return r;
        }

        const auto* 能力节点类型 = 基础节点类型词(
            能力项.场景引用
                ? reinterpret_cast<基础信息节点类*>(能力项.场景引用)
                : (能力项.存在引用 ? 能力项.存在引用 : 能力项.基础节点引用));
        if (需求节点类型 && 能力节点类型) {
            auto 类型结果 = 特征类型可覆盖(需求节点类型, 能力节点类型, 限制);
            r.成功 = 类型结果.成功;
            r.抽象距离 = 类型结果.抽象距离;
            r.失败原因 = 类型结果.失败原因;
            r.路径 = std::move(类型结果.路径);
            return r;
        }

        if (需求项.基础节点引用 && 能力项.基础节点引用
            && 需求项.基础节点引用 == 能力项.基础节点引用) {
            r.成功 = true;
            return r;
        }
        if (需求项.存在引用 && 能力项.存在引用 && 需求项.存在引用 == 能力项.存在引用) {
            r.成功 = true;
            return r;
        }
        if (需求项.场景引用 && 能力项.场景引用 && 需求项.场景引用 == 能力项.场景引用) {
            r.成功 = true;
            return r;
        }

        r.失败原因 = 值_没有匹配方法();
        return r;
    }

    inline 结构_条件项匹配结果 匹配条件项(
        const 结构_条件项视图& 需求项,
        const 结构_条件项视图& 能力项,
        const 结构_查找限制& 限制) noexcept
    {
        结构_条件项匹配结果 r{};
        auto 类型结果 = 特征类型可覆盖(
            需求项.条件特征类型,
            能力项.条件特征类型,
            限制);
        if (!类型结果.成功) {
            r.失败原因 = 类型结果.失败原因;
            return r;
        }

        auto 值结果 = 特征值约束可覆盖(需求项, 能力项, 限制);
        if (!值结果.成功) {
            r.失败原因 = 值结果.失败原因;
            return r;
        }

        r.成功 = true;
        r.类型抽象距离 = 类型结果.抽象距离;
        r.值域抽象距离 = 值结果.抽象距离;
        r.总距离 = 饱和增加(r.类型抽象距离, r.值域抽象距离);
        r.能力宽度 = 值结果.能力宽度;
        r.类型路径 = std::move(类型结果.路径);
        r.值域路径 = std::move(值结果.路径);
        return r;
    }

    inline 结构_方法匹配结果 匹配需求包与能力包(
        基础信息节点类* 需求条件包,
        基础信息节点类* 方法能力包,
        const 结构_查找限制& 限制) noexcept
    {
        结构_方法匹配结果 结果{};
        auto 需求项列表 = 读取条件项列表(需求条件包);
        auto 能力项列表 = 读取条件项列表(方法能力包);
        if (需求项列表.empty() || 能力项列表.empty()) {
            结果.失败原因 = 值_没有匹配方法();
            return 结果;
        }

        for (const auto& 需求项 : 需求项列表) {
            if (!需求项.必需) continue;

            结构_条件项匹配结果 当前最佳{};
            bool 找到匹配项 = false;
            for (const auto& 能力项 : 能力项列表) {
                auto 单项结果 = 匹配条件项(需求项, 能力项, 限制);
                if (!单项结果.成功) continue;

                if (!找到匹配项
                    || 单项结果.总距离 < 当前最佳.总距离
                    || (单项结果.总距离 == 当前最佳.总距离
                        && 单项结果.能力宽度 < 当前最佳.能力宽度)) {
                    当前最佳 = std::move(单项结果);
                    找到匹配项 = true;
                }
            }

            if (!找到匹配项) {
                结果.失败需求项 = 需求项;
                结果.失败原因 = 值_没有匹配方法();
                return 结果;
            }

            结果.总类型抽象距离 = 饱和增加(
                结果.总类型抽象距离,
                当前最佳.类型抽象距离);
            结果.总值域抽象距离 = 饱和增加(
                结果.总值域抽象距离,
                当前最佳.值域抽象距离);
            结果.总匹配距离 = 饱和增加(结果.总匹配距离, 当前最佳.总距离);
            结果.能力宽度 = 饱和增加(结果.能力宽度, 当前最佳.能力宽度);
            结果.条件项结果.push_back(std::move(当前最佳));
        }

        结果.成功 = !结果.条件项结果.empty();
        if (!结果.成功) 结果.失败原因 = 值_没有匹配方法();
        return 结果;
    }

    inline bool 读取限制指针(
        基础信息节点类* 限制包,
        const 词性节点类* 特征类型,
        基础信息节点类*& 输出节点,
        const 词性节点类*& 输出词性) noexcept
    {
        void* 指针 = nullptr;
        if (!读取包指针(限制包, 特征类型, 指针)) return false;
        if (auto* 节点 = 指针若为基础信息节点(指针)) {
            输出节点 = 节点;
            输出词性 = 基础节点类型词(节点);
        }
        else {
            输出节点 = nullptr;
            输出词性 = reinterpret_cast<const 词性节点类*>(指针);
        }
        return 输出节点 || 输出词性;
    }

    inline bool 读取包区间(
        基础信息节点类* 包,
        const 词性节点类* 区间特征类型,
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
        const 词性节点类* 子类型,
        基础信息节点类*& 输出节点) noexcept
    {
        输出节点 = nullptr;
        void* 指针 = nullptr;
        if (!读取包指针(包, 子类型, 指针)) return false;
        输出节点 = 指针若为基础信息节点(指针);
        return 输出节点 != nullptr;
    }

    inline bool 读取包词性指针(
        基础信息节点类* 包,
        const 词性节点类* 子类型,
        const 词性节点类*& 输出词性) noexcept
    {
        输出词性 = nullptr;
        void* 指针 = nullptr;
        if (!读取包指针(包, 子类型, 指针)) return false;
        if (auto* 节点 = 指针若为基础信息节点(指针)) {
            输出词性 = 基础节点类型词(节点);
        }
        else {
            输出词性 = reinterpret_cast<const 词性节点类*>(指针);
        }
        return 输出词性 != nullptr;
    }

    inline bool 包目标值类型为I64(基础信息节点类* 输入条件包) noexcept
    {
        const 词性节点类* 词性值 = nullptr;
        if (读取包词性指针(输入条件包, 特征_目标值类型(), 词性值)) {
            return 词性值 == 值类型_I64()
                || 词性值 == 值类型_类型I64();
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

    inline bool 绑定改变特征值输入帧(
        场景节点类* 输入参数场景,
        方法类::节点类* 方法首节点,
        结构_改变特征值输入帧& 输出) noexcept
    {
        (void)方法首节点;
        输出 = {};
        auto* 输入条件 = 输入条件包根(输入参数场景);
        if (!输入条件) {
            输出.绑定失败原因 = 值_输入条件包绑定失败();
            return false;
        }
        输出.输入条件包 = reinterpret_cast<基础信息节点类*>(输入条件);

        if (!读取包基础节点指针(输出.输入条件包, 特征_目标宿主(), 输出.目标宿主)) {
            输出.绑定失败原因 = 值_输入条件包绑定失败();
            return false;
        }
        if (!读取包基础节点指针(
                输出.输入条件包,
                特征_目标抽象特征(),
                输出.目标抽象特征)
            || !世界树.特征().取抽象特征主信息(
                static_cast<抽象特征节点类*>(输出.目标抽象特征))) {
            输出.绑定失败原因 = 值_目标抽象特征无效();
            return false;
        }

        输出.目标值类型为I64 = 包目标值类型为I64(输出.输入条件包);
        if (!输出.目标值类型为I64) {
            输出.绑定失败原因 = 值_目标值类型不是I64();
            return false;
        }

        输出.差值约束包 = 查找包子节点(输出.输入条件包, 特征_差值约束包());
        if (!输出.差值约束包) {
            输出.绑定失败原因 = 值_差值约束包缺失();
            return false;
        }
        输出.差值约束区间 = 读取差值约束区间集合(输出.差值约束包);
        if (输出.差值约束区间.empty()) {
            输出.绑定失败原因 = 值_差值约束包缺失();
            return false;
        }

        输出.有执行差值 = 读取基础节点I64(
            输出.输入条件包,
            特征_执行差值(),
            输出.执行差值);
        输出.有目标执行后值 = 读取基础节点I64(
            输出.输入条件包,
            特征_目标执行后值(),
            输出.目标执行后值);
        输出.有执行前值快照 = 读取基础节点I64(
            输出.输入条件包,
            特征_执行前值快照(),
            输出.执行前值快照);

        I64 策略值 = 0;
        if (读取基础节点I64(输出.输入条件包, 特征_实例缺失策略(), 策略值)) {
            if (策略值 == 1) {
                输出.实例缺失策略 = 枚举_实例缺失策略_改变::允许定义实例化;
            }
            else if (策略值 == 2) {
                输出.实例缺失策略 = 枚举_实例缺失策略_改变::允许观测实例化;
            }
        }
        if (读取基础节点I64(输出.输入条件包, 特征_溢出策略(), 策略值)) {
            if (策略值 == 1) {
                输出.溢出策略 = 枚举_溢出策略_改变::饱和写入;
            }
        }

        void* 指针 = nullptr;
        if (读取包指针(输出.输入条件包, 特征_目标实例特征(), 指针)) {
            if (auto* 节点 = 指针若为基础信息节点(指针)) {
                auto* 实例特征 = static_cast<特征节点类*>(节点);
                if (世界树.特征().取特征主信息(实例特征)) {
                    输出.目标实例特征 = 实例特征;
                }
            }
        }
        (void)读取包基础节点指针(
            输出.输入条件包,
            特征_特征类型配对结果(),
            输出.特征类型配对结果);
        输出.限制包 = 查找包子节点(输出.输入条件包, 特征_限制包());

        if (!输出.有执行差值
            && !输出.有目标执行后值
            && !差值约束为唯一零区间(输出.差值约束区间)) {
            输出.绑定失败原因 = 值_缺少执行差值或目标值();
            return false;
        }

        return true;
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
            输出限制.允许值类型 = reinterpret_cast<const 词性节点类*>(允许值类型指针);
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
        if (读取基础节点I64(限制包, 特征_需要方法能力包(), 布尔值)) {
            输出限制.需要方法能力包 = 布尔值 != 0;
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

    inline bool 绑定查找方法输入帧(
        场景节点类* 输入参数场景,
        方法类::节点类* 方法首节点,
        结构_查找方法输入帧& 输出) noexcept
    {
        输出 = {};
        auto* 输入条件 = 输入条件包根(输入参数场景);
        if (!输入条件) return false;

        输出.输入条件包 = reinterpret_cast<基础信息节点类*>(输入条件);
        输出.需求条件包 = 查找包子节点(输出.输入条件包, 特征_需求条件包());
        输出.查找限制包 = 查找包子节点(输出.输入条件包, 特征_查找限制包());
        输出.候选范围包 = 查找包子节点(输出.输入条件包, 特征_候选范围包());

        if (!输出.需求条件包 || !输出.查找限制包) return false;
        if (!解析查找限制包(输出.查找限制包, 输出.限制)) return false;

        输出.需求项列表 = 读取条件项列表(输出.需求条件包);
        if (!需求包有必需条件项(输出.需求项列表)) return false;

        void* 方法搜索根指针 = nullptr;
        if (输出.候选范围包
            && 读取包指针(输出.候选范围包, 特征_方法搜索根(), 方法搜索根指针)) {
            输出.方法搜索根 = reinterpret_cast<方法类::节点类*>(方法搜索根指针);
        }
        else if (读取包指针(输出.输入条件包, 特征_方法搜索根(), 方法搜索根指针)) {
            输出.方法搜索根 = reinterpret_cast<方法类::节点类*>(方法搜索根指针);
        }

        if (!输出.方法搜索根) {
            输出.方法搜索根 = 方法树根(方法首节点);
            输出.使用默认方法搜索根 = 输出.方法搜索根 != nullptr;
        }
        return 输出.方法搜索根 != nullptr;
    }

    inline 基础信息节点类* 查找方法能力包(存在节点类* 方法虚拟存在节点) noexcept
    {
        return 方法虚拟存在节点
            ? 查找子特征(
                reinterpret_cast<基础信息节点类*>(方法虚拟存在节点),
                特征_方法能力包())
            : nullptr;
    }

    inline void 枚举候选方法递归(
        方法类::节点类* 当前,
        方法类::节点类* 调用方法,
        const 结构_查找限制& 限制,
        I64 深度,
        std::vector<方法类::节点类*>& 输出候选) noexcept
    {
        if (!当前 || 深度 > 限制.最大查找递归深度) return;
        if (限制.最大候选数量 > 0
            && 输出候选.size() >= static_cast<std::size_t>(限制.最大候选数量)) {
            return;
        }

        if (当前->主信息.节点种类 == 枚举_方法节点种类::方法首节点
            && (限制.是否允许当前方法自身 || 当前 != 调用方法)) {
            auto* 方法存在 = 当前->主信息.方法虚拟存在.指针;
            auto* 能力包 = 查找方法能力包(方法存在);
            if ((!限制.需要方法能力包 || 能力包)
                && (限制.是否包含禁用方法 || 方法存在)) {
                输出候选.push_back(当前);
            }
        }

        auto* 首子 = 当前->子 ? static_cast<方法类::节点类*>(当前->子) : nullptr;
        if (!首子) return;
        auto* 子 = 首子;
        do {
            枚举候选方法递归(
                子,
                调用方法,
                限制,
                深度 + 1,
                输出候选);
            if (限制.最大候选数量 > 0
                && 输出候选.size() >= static_cast<std::size_t>(限制.最大候选数量)) {
                return;
            }
            子 = static_cast<方法类::节点类*>(子->下);
        } while (子 && 子 != 首子);
    }

    inline std::vector<方法类::节点类*> 枚举候选方法(
        方法类::节点类* 方法搜索根,
        方法类::节点类* 调用方法,
        const 结构_查找限制& 限制) noexcept
    {
        std::vector<方法类::节点类*> 候选{};
        枚举候选方法递归(方法搜索根, 调用方法, 限制, 0, 候选);
        return 候选;
    }

    inline I64 读取方法成功率(存在节点类* 方法存在) noexcept
    {
        I64 成功次数 = 0;
        I64 失败次数 = 0;
        auto* 宿主 = reinterpret_cast<基础信息节点类*>(方法存在);
        (void)读取基础节点I64(宿主, 特征_成功次数(), 成功次数);
        (void)读取基础节点I64(宿主, 特征_失败次数(), 失败次数);
        const I64 总数 = 饱和增加(成功次数, 失败次数);
        return 总数 > 0 ? (成功次数 * 10000) / 总数 : 0;
    }

    inline I64 读取方法平均运行时长(存在节点类* 方法存在) noexcept
    {
        I64 平均运行时长 = 0;
        (void)读取基础节点I64(
            reinterpret_cast<基础信息节点类*>(方法存在),
            特征_平均运行时长(),
            平均运行时长);
        return 平均运行时长;
    }

    inline I64 读取方法自我能力值(方法类::节点类* 方法首节点) noexcept
    {
        if (!方法首节点
            || 方法首节点->主信息.动作句柄.类型 != 枚举_动作句柄类型::本能函数ID) {
            return 0;
        }
        const I64 能力值 = 方法首节点->主信息.动作句柄.本能ID;
        return 方法类::是有效本能方法能力值(能力值) ? 能力值 : 0;
    }

    inline bool 方法匹配优于(
        const 结构_候选方法结果& A,
        const 结构_候选方法结果& B) noexcept
    {
        if (!A.有效) return false;
        if (!B.有效) return true;
        if (A.匹配.总匹配距离 != B.匹配.总匹配距离) {
            return A.匹配.总匹配距离 < B.匹配.总匹配距离;
        }
        if (A.匹配.总类型抽象距离 != B.匹配.总类型抽象距离) {
            return A.匹配.总类型抽象距离 < B.匹配.总类型抽象距离;
        }
        if (A.匹配.总值域抽象距离 != B.匹配.总值域抽象距离) {
            return A.匹配.总值域抽象距离 < B.匹配.总值域抽象距离;
        }
        if (A.匹配.能力宽度 != B.匹配.能力宽度) {
            return A.匹配.能力宽度 < B.匹配.能力宽度;
        }
        if (A.方法成功率 != B.方法成功率) {
            return A.方法成功率 > B.方法成功率;
        }
        return A.平均运行时长 < B.平均运行时长;
    }

    inline bool 确认查找方法规格(
        方法类::节点类* 方法首节点,
        存在节点类* 查找方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !查找方法存在) return false;
        (void)方法类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
        bool ok = true;

        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_需求条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_查找限制包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_候选范围包(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_方法搜索根(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;

        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法查找情况()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法配对结果()) && ok;
            ok = 取或创建子特征(宿主, 特征_选中自我能力值()) && ok;
            ok = 取或创建子特征(宿主, 特征_缓存方法首节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_缓存方法虚拟存在()) && ok;
            ok = 取或创建子特征(宿主, 特征_总匹配距离()) && ok;
            ok = 取或创建子特征(宿主, 特征_匹配路径记录()) && ok;
            ok = 取或创建子特征(宿主, 特征_待配对形参表()) && ok;
        }
        if (auto* 效果根 = 方法规格根(方法首节点, 特征_效果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(效果根);
            ok = 写入基础节点指针(宿主, 特征_方法配对结果(), 类型_方法配对结果存在(), now) && ok;
            ok = 写入基础节点指针(宿主, 值_不修改现实场景特征值(), 值_不修改现实场景特征值(), now) && ok;
            ok = 写入基础节点指针(宿主, 值_不执行被查找到的方法(), 值_不执行被查找到的方法(), now) && ok;
        }

        auto* 能力包 = 取或创建子特征(
            reinterpret_cast<基础信息节点类*>(查找方法存在),
            特征_方法能力包());
        if (能力包) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(能力包);
            ok = 写入基础节点指针(宿主, 特征_需求条件包(), 特征_需求条件包(), now) && ok;
            ok = 写入基础节点指针(宿主, 特征_查找限制包(), 特征_查找限制包(), now) && ok;
            ok = 写入基础节点指针(宿主, 特征_方法配对结果(), 类型_方法配对结果存在(), now) && ok;
        }
        else {
            ok = false;
        }

        (void)声明条件特征(方法首节点, 特征_需求条件包(), now);
        (void)声明条件特征(方法首节点, 特征_查找限制包(), now);
        (void)声明条件特征(方法首节点, 特征_候选范围包(), now);
        return ok;
    }

    inline void 写入查找方法运行摘要(
        方法类::节点类* 方法首节点,
        存在节点类* 运行存在,
        场景节点类* 输出结果场景,
        const 词性节点类* 执行情况,
        I64 查找情况码,
        const 词性节点类* 失败原因,
        std::uint64_t 已扫描数量,
        std::uint64_t 成功匹配数量,
        时间戳 now) noexcept
    {
        if (运行存在) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(运行存在);
            (void)写入方法执行情况(宿主, 执行情况, now);
            (void)写入存在I64(运行存在, 特征_方法查找情况(), 查找情况码, now);
            (void)写入存在I64(运行存在, 特征_候选扫描数量(), static_cast<I64>(已扫描数量), now);
            (void)写入存在I64(运行存在, 特征_成功匹配数量(), static_cast<I64>(成功匹配数量), now);
            if (失败原因) {
                (void)写入存在指针(运行存在, 特征_失败原因(), 失败原因, now);
            }
        }
        if (输出结果场景) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(输出结果场景);
            (void)写入方法执行情况(宿主, 执行情况, now);
            (void)写入基础节点I64(宿主, 特征_方法查找情况(), 查找情况码, now);
            (void)写入基础节点I64(宿主, 特征_候选扫描数量(), static_cast<I64>(已扫描数量), now);
            (void)写入基础节点I64(宿主, 特征_成功匹配数量(), static_cast<I64>(成功匹配数量), now);
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        }
        if (auto* 方法存在 = 方法虚拟存在(方法首节点, now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(方法存在);
            (void)写入方法执行情况(宿主, 执行情况, now);
            (void)写入基础节点I64(宿主, 特征_方法查找情况(), 查找情况码, now);
        }
    }

    inline 存在节点类* 输出查找方法失败(
        方法类::节点类* 方法首节点,
        存在节点类* 运行存在,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        const 词性节点类* 执行情况,
        const 词性节点类* 失败原因,
        时间戳 now,
        std::uint64_t 已扫描数量 = 0,
        std::uint64_t 成功匹配数量 = 0) noexcept
    {
        const I64 查找情况码 = 执行情况 == 值_输入绑定失败()
            || 执行情况 == 值_参数绑定失败()
            ? 方法查找情况_输入绑定失败
            : (失败原因 == 值_超出限制()
                ? 方法查找情况_超出限制
                : 方法查找情况_配对失败);
        写入查找方法运行摘要(
            方法首节点,
            运行存在,
            输出结果场景,
            执行情况,
            查找情况码,
            失败原因,
            已扫描数量,
            成功匹配数量,
            now);
        return 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
    }

    inline 存在节点类* 创建方法配对结果存在(
        场景节点类* 输出结果场景,
        const 结构_查找方法输入帧& 输入帧,
        const 结构_候选方法结果& 最佳,
        时间戳 now) noexcept
    {
        if (!输出结果场景 || !最佳.有效) return nullptr;
        auto* 配对结果 = 世界树.取或创建子存在_按类型并按特征I64(
            reinterpret_cast<基础信息节点类*>(输出结果场景),
            类型_方法配对结果存在(),
            特征_开始时间(),
            static_cast<I64>(now));
        if (!配对结果) return nullptr;

        (void)写入存在I64(配对结果, 特征_配对状态(), 方法查找情况_配对成功, now);
        (void)写入存在指针(配对结果, 特征_来源需求条件包(), 输入帧.需求条件包, now);
        (void)写入存在指针(配对结果, 特征_使用限制包(), 输入帧.查找限制包, now);
        if (最佳.有自我能力值) {
            (void)写入存在I64(配对结果, 特征_选中自我能力值(), 最佳.自我能力值, now);
        }
        (void)写入存在指针(配对结果, 特征_缓存方法首节点(), 最佳.方法首节点, now);
        (void)写入存在指针(配对结果, 特征_缓存方法虚拟存在(), 最佳.方法虚拟存在, now);
        (void)写入存在指针(配对结果, 特征_选中方法首节点(), 最佳.方法首节点, now);
        (void)写入存在指针(配对结果, 特征_选中方法虚拟存在(), 最佳.方法虚拟存在, now);
        (void)写入存在指针(配对结果, 特征_匹配方法能力包(), 最佳.方法能力包, now);
        (void)写入存在指针(配对结果, 特征_原始需求绑定包(), 输入帧.需求条件包, now);
        (void)写入存在I64(配对结果, 特征_类型抽象距离(), 最佳.匹配.总类型抽象距离, now);
        (void)写入存在I64(配对结果, 特征_值域抽象距离(), 最佳.匹配.总值域抽象距离, now);
        (void)写入存在I64(配对结果, 特征_总匹配距离(), 最佳.匹配.总匹配距离, now);
        (void)写入存在I64(配对结果, 特征_使用默认方法搜索根(), 输入帧.使用默认方法搜索根 ? 1 : 0, now);

        if (auto* 形参表 = 查找子特征(
                reinterpret_cast<基础信息节点类*>(最佳.方法虚拟存在),
                特征_形参表())) {
            (void)写入存在指针(配对结果, 特征_待配对形参表(), 形参表, now);
        }
        auto* 路径记录 = 取或创建子特征(
            reinterpret_cast<基础信息节点类*>(配对结果),
            特征_匹配路径记录());
        if (路径记录) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(路径记录);
            (void)写入基础节点I64(宿主, 特征_类型抽象距离(), 最佳.匹配.总类型抽象距离, now);
            (void)写入基础节点I64(宿主, 特征_值域抽象距离(), 最佳.匹配.总值域抽象距离, now);
            (void)写入基础节点I64(宿主, 特征_总匹配距离(), 最佳.匹配.总匹配距离, now);
        }
        return 配对结果;
    }

    inline void 写入查找方法成功输出(
        方法类::节点类* 方法首节点,
        存在节点类* 运行存在,
        场景节点类* 输出结果场景,
        存在节点类* 方法配对结果,
        const 结构_候选方法结果& 最佳,
        std::uint64_t 已扫描数量,
        std::uint64_t 成功匹配数量,
        时间戳 now) noexcept
    {
        写入查找方法运行摘要(
            方法首节点,
            运行存在,
            输出结果场景,
            值_执行成功(),
            方法查找情况_配对成功,
            nullptr,
            已扫描数量,
            成功匹配数量,
            now);

        if (运行存在) {
            (void)写入存在指针(运行存在, 特征_方法配对结果(), 方法配对结果, now);
            if (最佳.有自我能力值) {
                (void)写入存在I64(运行存在, 特征_选中自我能力值(), 最佳.自我能力值, now);
            }
            (void)写入存在指针(运行存在, 特征_缓存方法首节点(), 最佳.方法首节点, now);
            (void)写入存在指针(运行存在, 特征_缓存方法虚拟存在(), 最佳.方法虚拟存在, now);
            (void)写入存在指针(运行存在, 特征_选中方法首节点(), 最佳.方法首节点, now);
            (void)写入存在指针(运行存在, 特征_选中方法虚拟存在(), 最佳.方法虚拟存在, now);
            (void)写入存在I64(运行存在, 特征_总匹配距离(), 最佳.匹配.总匹配距离, now);
        }
        if (输出结果场景) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(输出结果场景);
            (void)写入基础节点指针(宿主, 特征_方法配对结果(), 方法配对结果, now);
            if (最佳.有自我能力值) {
                (void)写入基础节点I64(宿主, 特征_选中自我能力值(), 最佳.自我能力值, now);
            }
            (void)写入基础节点指针(宿主, 特征_缓存方法首节点(), 最佳.方法首节点, now);
            (void)写入基础节点指针(宿主, 特征_缓存方法虚拟存在(), 最佳.方法虚拟存在, now);
            (void)写入基础节点指针(宿主, 特征_选中方法首节点(), 最佳.方法首节点, now);
            (void)写入基础节点指针(宿主, 特征_选中方法虚拟存在(), 最佳.方法虚拟存在, now);
            (void)写入基础节点I64(宿主, 特征_总匹配距离(), 最佳.匹配.总匹配距离, now);
        }
    }

    inline bool 确认改变特征值规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        (void)方法类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
        bool ok = true;

        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标宿主(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标抽象特征(), 值类型_特征类型指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标值类型(), 值类型_特征类型指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_差值约束包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_执行差值(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标执行后值(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_实例缺失策略(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_溢出策略(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;

        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法执行情况()) && ok;
            ok = 取或创建子特征(宿主, 特征_执行前值()) && ok;
            ok = 取或创建子特征(宿主, 特征_执行后值()) && ok;
            ok = 取或创建子特征(宿主, 特征_实际执行差值()) && ok;
            ok = 取或创建子特征(宿主, 特征_目标实例特征()) && ok;
            ok = 取或创建子特征(宿主, 特征_差值命中状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_派生需求()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作动态()) && ok;
        }
        if (auto* 效果根 = 方法规格根(方法首节点, 特征_效果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(效果根);
            ok = 写入基础节点指针(宿主, 特征_目标宿主(), 值_来源_输入条件包(), now) && ok;
            ok = 写入基础节点指针(宿主, 特征_目标抽象特征(), 值_来源_输入条件包(), now) && ok;
            ok = 写入基础节点指针(宿主, 特征_执行后值(), 特征_执行后值(), now) && ok;
        }

        auto* 能力包 = 取或创建子特征(
            reinterpret_cast<基础信息节点类*>(方法存在),
            特征_方法能力包());
        if (能力包) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(能力包);
            ok = 写入基础节点指针(宿主, 特征_目标宿主(), 特征_目标宿主(), now) && ok;
            ok = 写入基础节点指针(宿主, 特征_目标抽象特征(), 特征_目标抽象特征(), now) && ok;
            ok = 写入基础节点指针(宿主, 特征_目标值类型(), 值类型_类型I64(), now) && ok;
            auto* 差值项 = 取或创建子特征(宿主, 特征_执行差值());
            if (差值项) {
                auto* 差值宿主 = reinterpret_cast<基础信息节点类*>(差值项);
                ok = 写入基础节点I64(差值宿主, 特征_区间下界(), (std::numeric_limits<I64>::min)(), now) && ok;
                ok = 写入基础节点I64(差值宿主, 特征_区间上界(), (std::numeric_limits<I64>::max)(), now) && ok;
            }
            else {
                ok = false;
            }
        }
        else {
            ok = false;
        }

        (void)声明条件特征(方法首节点, 特征_目标宿主(), now);
        (void)声明条件特征(方法首节点, 特征_目标抽象特征(), now);
        (void)声明条件特征(方法首节点, 特征_目标值类型(), now);
        (void)声明条件特征(方法首节点, 特征_差值约束包(), now);
        return ok;
    }

    inline void 写入改变特征值输入摘要(
        基础信息节点类* 宿主,
        const 结构_改变特征值输入帧* 输入帧,
        时间戳 now) noexcept
    {
        if (!宿主 || !输入帧) return;
        if (输入帧->输入条件包) {
            (void)写入基础节点指针(宿主, 特征_输入条件包(), 输入帧->输入条件包, now);
        }
        if (输入帧->目标宿主) {
            (void)写入基础节点指针(宿主, 特征_目标宿主(), 输入帧->目标宿主, now);
        }
        if (输入帧->目标抽象特征) {
            (void)写入基础节点指针(宿主, 特征_目标抽象特征(), 输入帧->目标抽象特征, now);
        }
        if (输入帧->目标实例特征) {
            (void)写入基础节点指针(宿主, 特征_目标实例特征(), 输入帧->目标实例特征, now);
        }
        if (输入帧->差值约束包) {
            (void)写入基础节点指针(宿主, 特征_差值约束包(), 输入帧->差值约束包, now);
        }
        if (输入帧->特征类型配对结果) {
            (void)写入基础节点指针(宿主, 特征_特征类型配对结果(), 输入帧->特征类型配对结果, now);
        }
        if (输入帧->限制包) {
            (void)写入基础节点指针(宿主, 特征_限制包(), 输入帧->限制包, now);
        }
        (void)写入基础节点I64(
            宿主,
            特征_实例缺失策略(),
            static_cast<I64>(输入帧->实例缺失策略),
            now);
        (void)写入基础节点I64(
            宿主,
            特征_溢出策略(),
            static_cast<I64>(输入帧->溢出策略),
            now);
    }

    inline void 写入改变特征值执行摘要(
        基础信息节点类* 宿主,
        const 结构_改变特征值输入帧& 输入帧,
        I64 执行前值,
        I64 执行后值,
        I64 实际执行差值,
        bool 发生溢出,
        基础信息节点类* 当前命中抽象特征,
        动态节点类* 动态,
        时间戳 now) noexcept
    {
        if (!宿主) return;
        写入改变特征值输入摘要(宿主, &输入帧, now);
        (void)写入基础节点I64(宿主, 特征_执行前值(), 执行前值, now);
        (void)写入基础节点I64(宿主, 特征_执行后值(), 执行后值, now);
        (void)写入基础节点I64(宿主, 特征_实际执行差值(), 实际执行差值, now);
        (void)写入基础节点I64(宿主, 特征_差值命中状态(), 差值命中状态_命中, now);
        (void)写入基础节点I64(宿主, 特征_是否发生溢出(), 发生溢出 ? 1 : 0, now);
        if (当前命中抽象特征) {
            (void)写入基础节点指针(宿主, 特征_当前命中抽象特征(), 当前命中抽象特征, now);
        }
        if (动态) {
            (void)写入基础节点指针(宿主, 特征_动作动态(), 动态, now);
        }
    }

    inline 存在节点类* 输出改变特征值失败(
        方法类::节点类* 方法首节点,
        存在节点类* 运行存在,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        const 结构_改变特征值输入帧* 输入帧,
        const 词性节点类* 执行情况,
        const 词性节点类* 失败原因,
        const 词性节点类* 派生需求,
        时间戳 now) noexcept
    {
        const auto* 情况 = 执行情况 ? 执行情况 : 值_执行失败();
        if (运行存在) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(运行存在);
            (void)写入方法执行情况(宿主, 情况, now);
            (void)写入基础节点I64(宿主, 特征_差值命中状态(), 差值命中状态_未命中, now);
            写入改变特征值输入摘要(宿主, 输入帧, now);
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
            if (派生需求) {
                (void)写入基础节点指针(宿主, 特征_派生需求(), 派生需求, now);
            }
        }
        if (输出结果场景) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(输出结果场景);
            (void)写入方法执行情况(宿主, 情况, now);
            (void)写入基础节点I64(宿主, 特征_差值命中状态(), 差值命中状态_未命中, now);
            写入改变特征值输入摘要(宿主, 输入帧, now);
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
            if (派生需求) {
                (void)写入基础节点指针(宿主, 特征_派生需求(), 派生需求, now);
            }
        }
        if (auto* 方法存在 = 方法虚拟存在(方法首节点, now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(方法存在);
            (void)写入方法执行情况(宿主, 情况, now);
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        }
        return 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
    }

    inline 存在节点类* 输出改变特征值成功(
        方法类::节点类* 方法首节点,
        存在节点类* 运行存在,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        const 结构_改变特征值输入帧& 输入帧,
        I64 执行前值,
        I64 执行后值,
        I64 实际执行差值,
        bool 发生溢出,
        基础信息节点类* 当前命中抽象特征,
        时间戳 now) noexcept
    {
        场景节点类* 目标场景 = nullptr;
        if (输入帧.目标宿主
            && 世界树.基础信息().取主信息<场景节点主信息类>(输入帧.目标宿主)) {
            目标场景 = static_cast<场景节点类*>(输入帧.目标宿主);
        }

        动态节点类* 动态 = nullptr;
        if (目标场景 && 输入帧.目标实例特征) {
            (void)创建场景状态(目标场景, 输入帧.目标实例特征, 执行前值, false, now);
            (void)创建场景状态(
                目标场景,
                输入帧.目标实例特征,
                执行后值,
                执行前值 != 执行后值,
                now);
            动态 = 输出动作动态(
                方法首节点,
                目标场景,
                输入帧.目标实例特征,
                输入参数场景,
                输出结果场景,
                true,
                now);
        }

        if (运行存在) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(运行存在);
            (void)写入方法执行情况(宿主, 值_执行成功(), now);
            写入改变特征值执行摘要(
                宿主,
                输入帧,
                执行前值,
                执行后值,
                实际执行差值,
                发生溢出,
                当前命中抽象特征,
                动态,
                now);
        }
        if (输出结果场景) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(输出结果场景);
            (void)写入方法执行情况(宿主, 值_执行成功(), now);
            写入改变特征值执行摘要(
                宿主,
                输入帧,
                执行前值,
                执行后值,
                实际执行差值,
                发生溢出,
                当前命中抽象特征,
                动态,
                now);
        }
        if (auto* 方法存在 = 方法虚拟存在(方法首节点, now)) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(方法存在),
                值_执行成功(),
                now);
        }

        return 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
    }


}

export namespace 自我动作实现模块 {
    inline 存在节点类* 查找方法(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto* 实际方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_查找方法,
            now);
        if (!实际方法首节点) return nullptr;

        auto* 运行存在 = 本轮运行虚拟存在(实际方法首节点, 输出结果场景, now);
        if (!运行存在) return nullptr;

        auto* 查找方法存在 = 方法虚拟存在(实际方法首节点, now);
        if (!查找方法存在) {
            auto* 输出运行 = 输出查找方法失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                值_执行失败(),
                值_方法虚拟存在缺失(),
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        const bool 规格已确认 = 确认查找方法规格(
            实际方法首节点,
            查找方法存在,
            now);
        auto 条件结果节点 = 构建方法条件和结果节点(
            实际方法首节点,
            输入参数场景,
            输出结果场景,
            now);
        if (运行存在) {
            (void)写入存在指针(运行存在, 特征_方法条件节点(), 条件结果节点.条件节点, now);
            (void)写入存在指针(运行存在, 特征_方法结果节点(), 条件结果节点.结果节点, now);
        }
        if (!规格已确认) {
            auto* 输出运行 = 输出查找方法失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                值_执行失败(),
                值_方法虚拟存在缺失(),
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        结构_查找方法输入帧 输入帧{};
        if (!绑定查找方法输入帧(输入参数场景, 实际方法首节点, 输入帧)) {
            auto* 输出运行 = 输出查找方法失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                值_输入绑定失败(),
                值_输入条件包绑定失败(),
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        结构_候选方法结果 最佳{};
        std::uint64_t 已扫描数量 = 0;
        std::uint64_t 成功匹配数量 = 0;

        const auto 候选列表 = 枚举候选方法(
            输入帧.方法搜索根,
            实际方法首节点,
            输入帧.限制);
        for (auto* 候选方法 : 候选列表) {
            if (!候选方法) continue;
            if (输入帧.限制.最大候选数量 > 0
                && 已扫描数量 >= static_cast<std::uint64_t>(输入帧.限制.最大候选数量)) {
                break;
            }
            ++已扫描数量;

            auto* 候选方法存在 = 候选方法->主信息.方法虚拟存在.指针;
            if (!候选方法存在) continue;

            auto* 能力包 = 查找方法能力包(候选方法存在);
            if (!能力包) continue;

            auto 匹配结果 = 匹配需求包与能力包(
                输入帧.需求条件包,
                能力包,
                输入帧.限制);
            if (!匹配结果.成功) continue;

            ++成功匹配数量;
            结构_候选方法结果 当前{};
            当前.有效 = true;
            当前.方法首节点 = 候选方法;
            当前.方法虚拟存在 = 候选方法存在;
            当前.方法能力包 = 能力包;
            当前.匹配 = std::move(匹配结果);
            当前.自我能力值 = 读取方法自我能力值(候选方法);
            当前.有自我能力值 = 当前.自我能力值 != 0;
            当前.方法成功率 = 读取方法成功率(候选方法存在);
            当前.平均运行时长 = 读取方法平均运行时长(候选方法存在);

            if (方法匹配优于(当前, 最佳)) {
                最佳 = std::move(当前);
            }
        }

        if (!最佳.有效) {
            auto* 输出运行 = 输出查找方法失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                值_执行失败(),
                成功匹配数量 == 0 ? 值_没有匹配方法() : 值_候选方法不可用(),
                now,
                已扫描数量,
                成功匹配数量);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        auto* 方法配对结果 = 创建方法配对结果存在(
            输出结果场景,
            输入帧,
            最佳,
            now);
        if (!方法配对结果) {
            auto* 输出运行 = 输出查找方法失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                值_执行失败(),
                值_方法配对结果创建失败(),
                now,
                已扫描数量,
                成功匹配数量);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        写入查找方法成功输出(
            实际方法首节点,
            运行存在,
            输出结果场景,
            方法配对结果,
            最佳,
            已扫描数量,
            成功匹配数量,
            now);
        auto* 输出运行 = 输出运行虚拟存在(
            运行存在,
            输入参数场景,
            输出结果场景,
            now);
        更新方法运行统计(实际方法首节点, 输出运行, true, now);
        return 输出运行;
    }

    inline 存在节点类* 改变特征值(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto* 实际方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_改变特征值,
            now);
        if (!实际方法首节点) return nullptr;

        // 模块一：方法规格建立与确认。
        auto* 运行存在 = 本轮运行虚拟存在(实际方法首节点, 输出结果场景, now);
        if (!运行存在) return nullptr;

        auto* 方法存在 = 方法虚拟存在(实际方法首节点, now);
        if (!方法存在) {
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                nullptr,
                值_执行失败(),
                值_方法虚拟存在缺失(),
                nullptr,
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        const bool 规格已确认 = 确认改变特征值规格(
            实际方法首节点,
            方法存在,
            now);
        auto 条件结果节点 = 构建方法条件和结果节点(
            实际方法首节点,
            输入参数场景,
            输出结果场景,
            now);
        if (运行存在) {
            (void)写入存在指针(运行存在, 特征_方法条件节点(), 条件结果节点.条件节点, now);
            (void)写入存在指针(运行存在, 特征_方法结果节点(), 条件结果节点.结果节点, now);
        }
        if (!规格已确认) {
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                nullptr,
                值_执行失败(),
                值_方法虚拟存在缺失(),
                nullptr,
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        // 模块二：输入条件包绑定与校验。
        结构_改变特征值输入帧 输入帧{};
        if (!绑定改变特征值输入帧(
                输入参数场景,
                实际方法首节点,
                输入帧)) {
            const auto* 失败原因 = 输入帧.绑定失败原因
                ? 输入帧.绑定失败原因
                : 值_输入条件包绑定失败();
            const auto* 派生需求 = 失败原因 == 值_缺少执行差值或目标值()
                ? 类型_需求_确定执行差值或目标值()
                : nullptr;
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                &输入帧,
                值_参数绑定失败(),
                失败原因,
                派生需求,
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        // 模块三：现实场景执行。
        if (!输入帧.目标实例特征) {
            输入帧.目标实例特征 = 查找宿主实例特征_按抽象特征(
                输入帧.目标宿主,
                输入帧.目标抽象特征);
        }
        if (!输入帧.目标实例特征
            && 输入帧.实例缺失策略 != 枚举_实例缺失策略_改变::失败) {
            输入帧.目标实例特征 = 创建宿主实例特征_按抽象特征(
                输入帧.目标宿主,
                输入帧.目标抽象特征);
        }
        if (!输入帧.目标实例特征) {
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                &输入帧,
                值_参数绑定失败(),
                值_目标实例特征缺失(),
                类型_需求_实例化特征(),
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        I64 执行前值 = 0;
        if (!读取实例特征I64(输入帧.目标实例特征, 执行前值)) {
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                &输入帧,
                值_参数绑定失败(),
                值_执行前值缺失(),
                类型_需求_确定特征当前值(),
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        if (输入帧.有执行前值快照 && 输入帧.执行前值快照 != 执行前值) {
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                &输入帧,
                值_执行失败(),
                值_输入快照过期(),
                类型_需求_重新配对参数或调整目标值(),
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        I64 执行后值 = 执行前值;
        bool 发生溢出 = false;
        const bool 不允许溢出 =
            输入帧.溢出策略 == 枚举_溢出策略_改变::不允许溢出;

        if (输入帧.有执行差值) {
            const auto 加法结果 = 安全增加I64(执行前值, 输入帧.执行差值);
            if (加法结果.溢出 && 不允许溢出) {
                auto* 输出运行 = 输出改变特征值失败(
                    实际方法首节点,
                    运行存在,
                    输入参数场景,
                    输出结果场景,
                    &输入帧,
                    值_执行失败(),
                    值_I64溢出(),
                    类型_需求_重新配对参数或调整目标值(),
                    now);
                更新方法运行统计(实际方法首节点, 输出运行, false, now);
                return 输出运行;
            }
            执行后值 = 加法结果.值;
            发生溢出 = 加法结果.溢出;
        }
        else if (输入帧.有目标执行后值) {
            执行后值 = 输入帧.目标执行后值;
        }
        else if (差值约束为唯一零区间(输入帧.差值约束区间)) {
            执行后值 = 执行前值;
        }
        else {
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                &输入帧,
                值_参数绑定失败(),
                值_缺少执行差值或目标值(),
                类型_需求_确定执行差值或目标值(),
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        if (输入帧.有执行差值 && 输入帧.有目标执行后值) {
            const auto 校验结果 = 安全增加I64(执行前值, 输入帧.执行差值);
            if (校验结果.溢出 && 不允许溢出) {
                auto* 输出运行 = 输出改变特征值失败(
                    实际方法首节点,
                    运行存在,
                    输入参数场景,
                    输出结果场景,
                    &输入帧,
                    值_执行失败(),
                    值_I64溢出(),
                    类型_需求_重新配对参数或调整目标值(),
                    now);
                更新方法运行统计(实际方法首节点, 输出运行, false, now);
                return 输出运行;
            }
            if (校验结果.值 != 输入帧.目标执行后值) {
                auto* 输出运行 = 输出改变特征值失败(
                    实际方法首节点,
                    运行存在,
                    输入参数场景,
                    输出结果场景,
                    &输入帧,
                    值_参数绑定失败(),
                    值_执行差值与目标值冲突(),
                    类型_需求_重新配对参数或调整目标值(),
                    now);
                更新方法运行统计(实际方法首节点, 输出运行, false, now);
                return 输出运行;
            }
            发生溢出 = 发生溢出 || 校验结果.溢出;
        }

        const auto 差值结果 = 安全差值I64(执行后值, 执行前值);
        if (差值结果.溢出 && 不允许溢出) {
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                &输入帧,
                值_执行失败(),
                值_差值计算溢出(),
                类型_需求_重新配对参数或调整目标值(),
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }
        const I64 实际执行差值 = 差值结果.差值;
        发生溢出 = 发生溢出 || 差值结果.溢出;

        if (!命中差值约束(实际执行差值, 输入帧.差值约束区间)) {
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                &输入帧,
                值_执行失败(),
                值_实际差值未命中约束(),
                类型_需求_重新配对参数或调整目标值(),
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        if (!写入实例特征I64(输入帧.目标实例特征, 执行后值, now)) {
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                &输入帧,
                值_执行失败(),
                值_目标特征写入失败(),
                类型_需求_修复实例特征写入能力(),
                now);
            更新方法运行统计(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        auto* 当前命中抽象特征 = 解析实例特征命中抽象特征(
            输入帧.目标实例特征,
            now);

        // 模块四：输出结果、运行事实和方法统计。
        auto* 输出运行 = 输出改变特征值成功(
            实际方法首节点,
            运行存在,
            输入参数场景,
            输出结果场景,
            输入帧,
            执行前值,
            执行后值,
            实际执行差值,
            发生溢出,
            当前命中抽象特征,
            now);
        更新方法运行统计(实际方法首节点, 输出运行, true, now);
        return 输出运行;
    }
    inline 存在节点类* 安全服务闭环安全推进(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        // 模块一：方法定位与规格确认。
        const auto now = 结构体_时间戳::当前_微秒();
        方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_安全服务闭环安全推进,
            now);
        auto* 运行存在 = 本轮运行虚拟存在(方法首节点, 输出结果场景, now);
        bool 规格已确认 = 方法首节点
            && 运行存在
            && 描述无条件本能(方法首节点, now);
        规格已确认 = 声明运行参数(方法首节点, 特征_安全值(), now) && 规格已确认;

        // 模块二：输入参数场景匹配。
        // 记录型本能不改变现实值，但输入场景存在时仍必须满足已声明运行参数。
        bool 输入已匹配 = true;
        if (输入参数场景) {
            输入已匹配 = 运行参数已从输入场景匹配(
                方法首节点,
                输入参数场景,
                now);
        }
        const bool 已绑定 = 规格已确认 && 输入已匹配;

        // 模块三：记录型执行。
        // 该类本能当前只形成运行事实，不执行数值写回。
        const bool 执行成功 = 已绑定;
        const auto* 情况 = 执行成功 ? 值_执行成功() : 值_参数绑定失败();

        // 模块四：输出结果、运行事实和方法统计。
        if (运行存在) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(运行存在),
                情况,
                now);
        }
        if (输出结果场景) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                情况,
                now);
        }
        if (auto* 虚拟存在 = 方法虚拟存在(方法首节点, now)) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(虚拟存在),
                情况,
                now);
        }
        auto* 输出运行 = 输出运行虚拟存在(
            运行存在,
            输入参数场景,
            输出结果场景,
            now);
        更新方法运行统计(方法首节点, 输出运行, 执行成功, now);
        return 输出运行;
    }

    inline 存在节点类* 安全服务闭环服务推进(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        // 模块一：方法定位与规格确认。
        const auto now = 结构体_时间戳::当前_微秒();
        方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_安全服务闭环服务推进,
            now);
        auto* 运行存在 = 本轮运行虚拟存在(方法首节点, 输出结果场景, now);
        bool 规格已确认 = 方法首节点
            && 运行存在
            && 描述无条件本能(方法首节点, now);
        规格已确认 = 声明运行参数(方法首节点, 特征_服务值(), now) && 规格已确认;

        // 模块二：输入参数场景匹配。
        // 记录型本能不改变现实值，但输入场景存在时仍必须满足已声明运行参数。
        bool 输入已匹配 = true;
        if (输入参数场景) {
            输入已匹配 = 运行参数已从输入场景匹配(
                方法首节点,
                输入参数场景,
                now);
        }
        const bool 已绑定 = 规格已确认 && 输入已匹配;

        // 模块三：记录型执行。
        // 该类本能当前只形成运行事实，不执行数值写回。
        const bool 执行成功 = 已绑定;
        const auto* 情况 = 执行成功 ? 值_执行成功() : 值_参数绑定失败();

        // 模块四：输出结果、运行事实和方法统计。
        if (运行存在) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(运行存在),
                情况,
                now);
        }
        if (输出结果场景) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                情况,
                now);
        }
        if (auto* 虚拟存在 = 方法虚拟存在(方法首节点, now)) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(虚拟存在),
                情况,
                now);
        }
        auto* 输出运行 = 输出运行虚拟存在(
            运行存在,
            输入参数场景,
            输出结果场景,
            now);
        更新方法运行统计(方法首节点, 输出运行, 执行成功, now);
        return 输出运行;
    }

    inline 存在节点类* 学习闭环维持(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        // 模块一：方法定位与规格确认。
        const auto now = 结构体_时间戳::当前_微秒();
        方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_学习闭环维持,
            now);
        auto* 运行存在 = 本轮运行虚拟存在(方法首节点, 输出结果场景, now);
        bool 规格已确认 = 方法首节点
            && 运行存在
            && 描述无条件本能(方法首节点, now);
        规格已确认 = 声明运行参数(方法首节点, 特征_待学习方法数量(), now) && 规格已确认;

        // 模块二：输入参数场景匹配。
        // 记录型本能不改变现实值，但输入场景存在时仍必须满足已声明运行参数。
        bool 输入已匹配 = true;
        if (输入参数场景) {
            输入已匹配 = 运行参数已从输入场景匹配(
                方法首节点,
                输入参数场景,
                now);
        }
        const bool 已绑定 = 规格已确认 && 输入已匹配;

        // 模块三：记录型执行。
        // 该类本能当前只形成运行事实，不执行数值写回。
        const bool 执行成功 = 已绑定;
        const auto* 情况 = 执行成功 ? 值_执行成功() : 值_参数绑定失败();

        // 模块四：输出结果、运行事实和方法统计。
        if (运行存在) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(运行存在),
                情况,
                now);
        }
        if (输出结果场景) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                情况,
                now);
        }
        if (auto* 虚拟存在 = 方法虚拟存在(方法首节点, now)) {
            (void)写入方法执行情况(
                reinterpret_cast<基础信息节点类*>(虚拟存在),
                情况,
                now);
        }
        auto* 输出运行 = 输出运行虚拟存在(
            运行存在,
            输入参数场景,
            输出结果场景,
            now);
        更新方法运行统计(方法首节点, 输出运行, 执行成功, now);
        return 输出运行;
    }

    inline 存在节点类* 增加可用方法数量(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        // 模块一：方法定位与规格确认。
        const auto now = 结构体_时间戳::当前_微秒();
        方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_增加可用方法数量,
            now);
        auto* 运行存在 = 本轮运行虚拟存在(方法首节点, 输出结果场景, now);
        const bool 规格已确认 = 方法首节点
            && 运行存在
            && 确认改变I64特征方法规格(
                方法首节点,
                特征_可用方法数量(),
                1,
                true,
                now);
        (void)构建方法条件和结果节点(
            方法首节点,
            输入参数场景,
            输出结果场景,
            now);

        // 模块二：调用帧绑定与校验。
        结构_改变I64特征绑定 绑定{};
        const bool 已绑定 = 规格已确认
            && 变化调用帧已绑定(
                运行存在,
                输入参数场景,
                特征_可用方法数量(),
                1,
                true,
                绑定,
                now);

        if (!已绑定) {
            auto* 输出运行 = 输出变化执行(
                方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                绑定.目标现实场景,
                特征_可用方法数量(),
                绑定.当前值,
                绑定.当前值,
                false,
                值_参数绑定失败(),
                now);
            更新方法运行统计(方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        // 模块三：现实场景执行。
        const I64 执行后值 = 按方向改变(绑定.当前值, 绑定.改变量, 1);
        const bool 执行成功 = 写入场景I64特征(
            绑定.目标现实场景,
            特征_可用方法数量(),
            执行后值,
            now);

        // 模块四：输出结果、运行事实和方法统计。
        auto* 输出运行 = 输出变化执行(
            方法首节点,
            运行存在,
            输入参数场景,
            输出结果场景,
            绑定.目标现实场景,
            特征_可用方法数量(),
            绑定.当前值,
            执行后值,
            执行成功,
            nullptr,
            now);
        更新方法运行统计(方法首节点, 输出运行, 执行成功, now);
        return 输出运行;
    }
}
