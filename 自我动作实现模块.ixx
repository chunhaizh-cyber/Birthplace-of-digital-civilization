module;

#include <cstdint>
#include <algorithm>
#include <limits>
#include <optional>
#include <vector>

#include "本能方法类.h"
#include "世界树类.h"
#include "方法类.h"
#include "方法虚拟存在服务类.h"
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

    inline const 词性节点类* 特征_目标特征() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标特征", "名词");
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

    inline const 词性节点类* 特征_样本次数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("样本次数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_最近样本时间() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("最近样本时间", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_结果符合次数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("结果符合次数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_结果不符合次数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("结果不符合次数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_动作执行成功次数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("动作执行成功次数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_动作执行失败次数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("动作执行失败次数", "名词");
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

    inline const 词性节点类* 特征_最近执行时间() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("最近执行时间", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_最近完成时间() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("最近完成时间", "名词");
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

    inline const 词性节点类* 特征_待处理方法数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("待处理方法数量", "名词");
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

    inline const 词性节点类* 特征_方法配对结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法配对结果", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_参数配对结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("参数配对结果", "名词");
        return s_词;
    }

    inline const 词性节点类* 类型_参数配对结果存在() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("参数配对结果存在", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_参数配对情况() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("参数配对情况", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_可执行输入参数场景() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("可执行输入参数场景", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_绑定参数数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("绑定参数数量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_缺失参数数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("缺失参数数量", "名词");
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

    inline const 词性节点类* 值_超出限制() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("超出限制", "名词");
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

    inline const 词性节点类* 特征_目标方法头节点() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标方法头节点", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_来源缺口() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源缺口", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法补齐尝试结果() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法补齐尝试结果", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法补齐状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法补齐状态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_试运行次数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("试运行次数", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_条件结果对() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("条件结果对", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_试运行限制包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("试运行限制包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_练习模式() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("练习模式", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_练习方向() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("练习方向", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_候选实参表() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("候选实参表", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_候选实参数量() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("候选实参数量", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_参数生成序号() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("参数生成序号", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_实参生成策略() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("实参生成策略", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_参数新颖性状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("参数新颖性状态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法补齐类别() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法补齐类别", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_来源因果信息() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源因果信息", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_来源观察动态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源观察动态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_动作主体() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("动作主体", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_候选动作节点() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("候选动作节点", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_候选方法能力包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("候选方法能力包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_候选形参包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("候选形参包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_候选结果包() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("候选结果包", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_预测状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("预测状态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_内化状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("内化状态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_动作主体替换状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("动作主体替换状态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_方法补齐结构状态() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法补齐结构状态", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_需求满足情况() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("需求满足情况", "名词");
        return s_词;
    }

    inline const 词性节点类* 特征_满足证据() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("满足证据", "名词");
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

    inline const 词性节点类* 值_方法配对结果缺失() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("方法配对结果缺失", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_可执行输入参数场景创建失败() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("可执行输入参数场景创建失败", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_必需参数缺失() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("必需参数缺失", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_目标方法头节点缺失() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标方法头节点缺失", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_因果证据不足() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("因果证据不足", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_自我动作缺失() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("自我动作缺失", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_候选动作未验证() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("候选动作未验证", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_目标特征缺失() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标特征缺失", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_来源_任务提供参数() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源_任务提供参数", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_来源_练习自动生成() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("来源_练习自动生成", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_参数条件缺口() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("参数条件缺口", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_缺少执行入口() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("缺少执行入口", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_需求满足() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("需求满足", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_需求未满足() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("需求未满足", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_目标抽象特征无效() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("目标抽象特征无效", "名词");
        return s_词;
    }

    inline const 词性节点类* 值_实例特征未命中抽象特征() noexcept
    {
        static const 词性节点类* s_词 = 语素集.添加词性词("实例特征未命中抽象特征", "名词");
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
            && 方法虚拟存在服务类::记录方法虚拟存在特征(
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
        if (auto* 条件包 = 查找子特征(宿主, 特征_条件集合())) return 条件包;
        return 查找子特征(宿主, 特征_运行参数());
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
        return 方法首节点->主信息.首节点信息().方法虚拟存在.指针
            ? 方法首节点->主信息.首节点信息().方法虚拟存在.指针
            : 方法虚拟存在服务类::取或创建方法虚拟存在(方法首节点, nullptr, now);
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
        (void)方法虚拟存在服务类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
        bool ok = true;

        ok = 声明形参规格(方法首节点, 特征_目标现实场景(), 值类型_场景指针(), 值_来源_输入现实场景包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_当前值(), 值类型_I64(), 值_来源_目标现实场景(), true, now, 目标特征类型) && ok;
        ok = 声明形参规格(方法首节点, 特征_改变量(), 值类型_I64(), 值_来源_输入任务参数包(), false, now) && ok;
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
        (void)需要改变量;
        (void)读取输入包I64(
            输入参数场景,
            特征_任务参数包(),
            特征_改变量(),
            改变量);

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

    struct 结构_方法条件结果对样本 {
        方法类::节点类* 来源方法 = nullptr;
        任务节点类* 来源任务 = nullptr;
        场景节点类* 执行前条件场景 = nullptr;
        场景节点类* 执行后实际结果场景 = nullptr;
        时间戳 执行结束时间 = 0;
    };

    inline 抽象特征节点类* 取或创建根下抽象特征(
        const 词性节点类* 特征类型) noexcept
    {
        auto* 抽象根 = 世界树.取或创建抽象特征根();
        return 抽象根 && 特征类型
            ? 世界树.取或创建抽象特征_按类型(抽象根, 特征类型)
            : nullptr;
    }

    inline 特征节点类* 写入方法虚拟存在节点实例特征(
        方法类::节点类* 方法首节点,
        const 词性节点类* 实例抽象类型,
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
        const 词性节点类* 特征类型,
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

    inline 结构_方法条件结果节点 归并方法条件结果对(
        方法类::节点类* 方法首节点,
        const 结构_方法条件结果对样本& 样本,
        时间戳 now) noexcept
    {
        结构_方法条件结果节点 节点{};
        if (!方法首节点) return 节点;

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
            if (节点.条件节点) {
                auto* 条件宿主 = reinterpret_cast<基础信息节点类*>(节点.条件节点);
                增加基础节点I64(条件宿主, 特征_样本次数(), 1, now);
                (void)写入基础节点I64(条件宿主, 特征_最近样本时间(), static_cast<I64>(now), now);
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
                auto* 结果宿主 = reinterpret_cast<基础信息节点类*>(节点.结果节点);
                增加基础节点I64(结果宿主, 特征_样本次数(), 1, now);
                (void)写入基础节点I64(结果宿主, 特征_最近样本时间(), static_cast<I64>(now), now);
                auto* 方法事实宿主 = static_cast<基础信息节点类*>(nullptr);
                if (auto* 虚拟存在 = 方法虚拟存在(方法首节点, now)) {
                    方法事实宿主 = reinterpret_cast<基础信息节点类*>(虚拟存在);
                }
                if (条件原已存在 && 结果原已存在) {
                    增加基础节点I64(结果宿主, 特征_结果符合次数(), 1, now);
                    增加基础节点I64(方法事实宿主, 特征_结果符合次数(), 1, now);
                } else if (条件原已存在 && 条件下原有结果) {
                    增加基础节点I64(结果宿主, 特征_结果不符合次数(), 1, now);
                    增加基础节点I64(方法事实宿主, 特征_结果不符合次数(), 1, now);
                    if (节点.条件节点) {
                        增加基础节点I64(
                            reinterpret_cast<基础信息节点类*>(节点.条件节点),
                            特征_结果不符合次数(),
                            1,
                            now);
                    }
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
            方法首节点->主信息.首节点信息().动作名,
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
                (void)写入存在I64(虚拟存在, 特征_执行前值(), 执行前值, now);
                (void)写入存在I64(虚拟存在, 特征_执行后值(), 执行后值, now);
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

        bool 已写入 = 方法虚拟存在服务类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
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

        if (方法树锚点->主信息.公共.节点种类 == 枚举_方法节点种类::方法首节点
            && 方法树锚点->主信息.首节点信息().动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && 方法树锚点->主信息.首节点信息().动作句柄.本能ID == static_cast<I64>(本能ID)) {
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

        (void)本能方法类::补齐本能方法首节点(新方法, *元信息, nullptr, now);
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
        I64 累计运行时长 = 0;
        I64 本次运行时长 = 0;

        (void)读取基础节点I64(宿主, 特征_运行次数(), 运行次数);
        (void)读取基础节点I64(宿主, 特征_动作执行成功次数(), 动作执行成功次数);
        (void)读取基础节点I64(宿主, 特征_动作执行失败次数(), 动作执行失败次数);
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
        (void)写入基础节点I64(宿主, 特征_运行次数(), 运行次数, now);
        (void)写入基础节点I64(宿主, 特征_动作执行成功次数(), 动作执行成功次数, now);
        (void)写入基础节点I64(宿主, 特征_动作执行失败次数(), 动作执行失败次数, now);
        (void)写入基础节点I64(宿主, 特征_累计运行时长(), 累计运行时长, now);
        (void)写入基础节点I64(宿主, 特征_平均运行时长(), 平均运行时长, now);
        (void)写入基础节点I64(宿主, 特征_运行时长(), 本次运行时长, now);
        (void)写入基础节点I64(宿主, 特征_最近执行时间(), static_cast<I64>(now), now);
        (void)写入基础节点I64(宿主, 特征_最近完成时间(), static_cast<I64>(now), now);
        if (运行存在) {
            (void)写入基础节点指针(宿主, 特征_最近运行虚拟存在(), 运行存在, now);
        }
    }

    inline constexpr I64 配对状态_输入绑定失败 = 1;
    inline constexpr I64 配对状态_配对失败 = 2;
    inline constexpr I64 配对状态_配对成功 = 4;

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

    struct 结构_类型匹配结果 {
        bool 成功 = false;
        I64 抽象距离 = 0;
        const 词性节点类* 失败原因 = nullptr;
        std::vector<基础信息节点类*> 路径{};
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

    inline const 词性节点类* 特征值节点对应特征类型词(
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
                return 特征值节点对应特征类型词(节点);
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

    inline bool 复制特征当前标量到宿主(
        特征节点类* 源特征,
        基础信息节点类* 目标宿主,
        const 词性节点类* 目标特征类型,
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
        const 词性节点类* 源特征类型,
        基础信息节点类* 目标包,
        const 词性节点类* 目标特征类型,
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
        const 词性节点类* 源特征类型,
        基础信息节点类* 目标包,
        const 词性节点类* 目标特征类型,
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
        const 词性节点类* 源特征类型,
        基础信息节点类* 目标包,
        const 词性节点类* 目标特征类型,
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
        const 词性节点类* 特征类型) noexcept
    {
        基础信息节点类* 节点 = nullptr;
        if (读取包基础节点指针(包, 特征类型, 节点)) {
            return 节点;
        }
        return 查找包子节点(包, 特征类型);
    }

    inline 基础信息节点类* 读取包抽象特征引用(
        基础信息节点类* 包,
        const 词性节点类* 主特征类型) noexcept
    {
        auto 解析一个 = [&](const 词性节点类* 特征类型) noexcept -> 基础信息节点类* {
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
                    查找抽象特征_按类型(reinterpret_cast<const 词性节点类*>(指针)));
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
        const 词性节点类* 场景名) noexcept
    {
        return 宿主 && 场景名
            ? 世界树.取或创建子场景_按名称(
                宿主,
                场景名,
                枚举_世界类型::内部世界)
            : nullptr;
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
        const auto 读取目标抽象特征 = [&]() noexcept -> bool {
            if (读取包基础节点指针(
                    输出.输入条件包,
                    特征_目标抽象特征(),
                    输出.目标抽象特征)) {
                return true;
            }
            if (读取包基础节点指针(
                    输出.输入条件包,
                    特征_目标特征(),
                    输出.目标抽象特征)) {
                return true;
            }
            void* 类型指针 = nullptr;
            if (读取包指针(输出.输入条件包, 特征_目标特征类型(), 类型指针)) {
                auto* 抽象特征 = 查找抽象特征_按类型(
                    static_cast<const 词性节点类*>(类型指针));
                if (抽象特征) {
                    输出.目标抽象特征 = 抽象特征;
                    return true;
                }
            }
            return false;
        };
        if (!读取目标抽象特征()
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

    inline bool 确认改变特征值规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        (void)方法虚拟存在服务类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
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
            写入改变特征值执行摘要(
                reinterpret_cast<基础信息节点类*>(方法存在),
                输入帧,
                执行前值,
                执行后值,
                实际执行差值,
                发生溢出,
                当前命中抽象特征,
                动态,
                now);
        }

        return 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
    }


}

export namespace 自我动作实现模块 {
    inline 存在节点类* 配对参数(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
    inline 存在节点类* 改变特征值(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept;
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

    inline bool 注册本能函数执行闭环() noexcept
    {
        bool ok = true;
        ok = 取本能动作集().注册(
            枚举_本能方法ID::自我_配对参数,
            &配对参数) && ok;
        ok = 取本能动作集().注册(
            枚举_本能方法ID::自我_改变特征值,
            &改变特征值) && ok;
        ok = 取本能动作集().注册(
            枚举_本能方法ID::自我_练习,
            &练习) && ok;
        ok = 取本能动作集().注册(
            枚举_本能方法ID::自我_尝试学习,
            &尝试学习) && ok;
        ok = 取本能动作集().注册(
            枚举_本能方法ID::自我_定向学习,
            &定向学习) && ok;
        return ok;
    }

    inline bool 确认配对参数规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        (void)方法虚拟存在服务类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_方法配对结果(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_需求条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_参数配对情况()) && ok;
            ok = 取或创建子特征(宿主, 特征_参数配对结果()) && ok;
            ok = 取或创建子特征(宿主, 特征_可执行输入参数场景()) && ok;
            ok = 取或创建子特征(宿主, 特征_绑定参数数量()) && ok;
            ok = 取或创建子特征(宿主, 特征_缺失参数数量()) && ok;
        }
        auto* 能力包 = 取或创建子特征(
            reinterpret_cast<基础信息节点类*>(方法存在),
            特征_方法能力包());
        if (能力包) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(能力包);
            ok = 写入基础节点指针(宿主, 特征_方法配对结果(), 类型_方法配对结果存在(), now) && ok;
            ok = 写入基础节点指针(宿主, 特征_可执行输入参数场景(), 特征_可执行输入参数场景(), now) && ok;
        }
        return ok;
    }

    inline 基础信息节点类* 读取方法配对结果输入(
        基础信息节点类* 输入条件包) noexcept
    {
        基础信息节点类* 配对结果 = nullptr;
        if (读取包基础节点指针(输入条件包, 特征_方法配对结果(), 配对结果)) {
            return 配对结果;
        }
        return 读取包基础引用或自身(输入条件包, 特征_方法配对结果());
    }

    inline 基础信息节点类* 读取配对原始需求包(
        基础信息节点类* 输入条件包,
        基础信息节点类* 方法配对结果) noexcept
    {
        基础信息节点类* 需求包 = nullptr;
        if (方法配对结果) {
            if (读取包基础节点指针(方法配对结果, 特征_原始需求绑定包(), 需求包)
                || 读取包基础节点指针(方法配对结果, 特征_来源需求条件包(), 需求包)) {
                return 需求包;
            }
        }
        if (读取包基础节点指针(输入条件包, 特征_需求条件包(), 需求包)) {
            return 需求包;
        }
        return 读取包基础引用或自身(输入条件包, 特征_需求条件包());
    }

    inline bool 写入差值约束到可执行包(
        基础信息节点类* 需求包,
        基础信息节点类* 可执行输入包,
        时间戳 now) noexcept
    {
        基础信息节点类* 差值约束包 = nullptr;
        if (读取包基础节点指针(需求包, 特征_差值约束包(), 差值约束包)) {
            return 写入基础节点指针(
                可执行输入包,
                特征_差值约束包(),
                差值约束包,
                now);
        }

        auto* 需求差值节点 = 查找包子节点(需求包, 特征_执行差值());
        if (!需求差值节点) return false;
        auto 区间集合 = 读取差值约束区间集合(需求差值节点);
        if (区间集合.empty()) return false;

        auto* 新约束包 = 取或创建子特征(可执行输入包, 特征_差值约束包());
        if (!新约束包) return false;
        bool ok = true;
        for (std::size_t i = 0; i < 区间集合.size(); ++i) {
            auto* 区间节点 = i == 0
                ? reinterpret_cast<基础信息节点类*>(新约束包)
                : reinterpret_cast<基础信息节点类*>(
                    取或创建子特征(
                        reinterpret_cast<基础信息节点类*>(新约束包),
                        特征_允许执行差值区间()));
            ok = 写入包区间(
                区间节点,
                区间集合[i].低值,
                区间集合[i].高值,
                now) && ok;
        }
        return ok;
    }

    inline 存在节点类* 输出配对参数结果(
        方法类::节点类* 方法首节点,
        存在节点类* 运行存在,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景,
        基础信息节点类* 方法配对结果,
        场景节点类* 可执行输入场景,
        I64 配对情况,
        const 词性节点类* 执行情况,
        const 词性节点类* 失败原因,
        I64 绑定参数数量,
        I64 缺失参数数量,
        时间戳 now) noexcept
    {
        存在节点类* 参数配对结果 = nullptr;
        if (输出结果场景) {
            参数配对结果 = 世界树.取或创建子存在_按类型并按特征I64(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                类型_参数配对结果存在(),
                特征_开始时间(),
                static_cast<I64>(now));
        }
        if (参数配对结果) {
            (void)写入存在I64(参数配对结果, 特征_配对状态(), 配对情况, now);
            (void)写入存在I64(参数配对结果, 特征_绑定参数数量(), 绑定参数数量, now);
            (void)写入存在I64(参数配对结果, 特征_缺失参数数量(), 缺失参数数量, now);
            (void)写入存在指针(参数配对结果, 特征_方法配对结果(), 方法配对结果, now);
            (void)写入存在指针(参数配对结果, 特征_可执行输入参数场景(), 可执行输入场景, now);
            if (失败原因) {
                (void)写入存在指针(参数配对结果, 特征_失败原因(), 失败原因, now);
            }
        }

        auto 写摘要 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入方法执行情况(宿主, 执行情况, now);
            (void)写入基础节点I64(宿主, 特征_参数配对情况(), 配对情况, now);
            (void)写入基础节点I64(宿主, 特征_绑定参数数量(), 绑定参数数量, now);
            (void)写入基础节点I64(宿主, 特征_缺失参数数量(), 缺失参数数量, now);
            (void)写入基础节点指针(宿主, 特征_参数配对结果(), 参数配对结果, now);
            (void)写入基础节点指针(宿主, 特征_可执行输入参数场景(), 可执行输入场景, now);
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        };
        写摘要(reinterpret_cast<基础信息节点类*>(运行存在));
        写摘要(reinterpret_cast<基础信息节点类*>(输出结果场景));
        if (auto* 方法存在 = 方法虚拟存在(方法首节点, now)) {
            写摘要(reinterpret_cast<基础信息节点类*>(方法存在));
        }
        return 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
    }

    inline 存在节点类* 配对参数(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto* 实际方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_配对参数,
            now);
        if (!实际方法首节点) return nullptr;

        auto* 运行存在 = 本轮运行虚拟存在(实际方法首节点, 输出结果场景, now);
        if (!运行存在) return nullptr;
        auto* 方法存在 = 方法虚拟存在(实际方法首节点, now);
        if (!方法存在 || !确认配对参数规格(实际方法首节点, 方法存在, now)) {
            auto* 输出运行 = 输出配对参数结果(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                nullptr,
                nullptr,
                配对状态_输入绑定失败,
                值_执行失败(),
                值_方法虚拟存在缺失(),
                0,
                1,
                now);
            更新方法运行账(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        auto* 输入条件 = 输入条件包根(输入参数场景);
        auto* 输入条件包 = reinterpret_cast<基础信息节点类*>(输入条件);
        auto* 方法配对结果 = 读取方法配对结果输入(输入条件包);
        auto* 需求包 = 读取配对原始需求包(输入条件包, 方法配对结果);
        if (!输入条件包 || !方法配对结果 || !需求包) {
            auto* 输出运行 = 输出配对参数结果(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                方法配对结果,
                nullptr,
                配对状态_输入绑定失败,
                值_参数绑定失败(),
                方法配对结果 ? 值_输入条件包绑定失败() : 值_方法配对结果缺失(),
                0,
                1,
                now);
            更新方法运行账(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        auto* 可执行场景宿主 = reinterpret_cast<基础信息节点类*>(
            输出结果场景 ? 输出结果场景 : reinterpret_cast<场景节点类*>(nullptr));
        if (!可执行场景宿主) {
            可执行场景宿主 = reinterpret_cast<基础信息节点类*>(方法存在);
        }
        auto* 可执行输入场景 = 创建子场景_按特征名(
            可执行场景宿主,
            特征_可执行输入参数场景());
        auto* 可执行输入包 = 可执行输入场景
            ? 取或创建子特征(
                reinterpret_cast<基础信息节点类*>(可执行输入场景),
                特征_输入条件包())
            : nullptr;
        if (!可执行输入包) {
            auto* 输出运行 = 输出配对参数结果(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                方法配对结果,
                nullptr,
                配对状态_配对失败,
                值_执行失败(),
                值_可执行输入参数场景创建失败(),
                0,
                1,
                now);
            更新方法运行账(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        auto* 可执行输入包宿主 = reinterpret_cast<基础信息节点类*>(可执行输入包);
        I64 绑定数量 = 0;
        I64 缺失数量 = 0;
        auto 累计绑定 = [&](bool ok) noexcept {
            if (ok) ++绑定数量;
            else ++缺失数量;
        };

        const auto* 目标宿主类型 = 查找子特征(需求包, 特征_目标宿主())
            ? 特征_目标宿主()
            : 特征_目标现实场景();
        累计绑定(复制包指针(需求包, 目标宿主类型, 可执行输入包宿主, 特征_目标宿主(), now));

        auto* 目标抽象特征 = 读取包抽象特征引用(
            需求包,
            特征_目标抽象特征());
        累计绑定(写入基础节点指针(
            可执行输入包宿主,
            特征_目标抽象特征(),
            目标抽象特征,
            now));

        bool 值类型已绑定 = 复制包指针(
            需求包,
            特征_目标值类型(),
            可执行输入包宿主,
            特征_目标值类型(),
            now);
        if (!值类型已绑定) {
            值类型已绑定 = 复制包特征当前值(
                需求包,
                特征_目标值类型(),
                可执行输入包宿主,
                特征_目标值类型(),
                now);
        }
        累计绑定(值类型已绑定);

        累计绑定(写入差值约束到可执行包(需求包, 可执行输入包宿主, now));
        const bool 已复制执行差值 = 复制包I64(
            输入条件包,
            特征_执行差值(),
            可执行输入包宿主,
            特征_执行差值(),
            now)
            || 复制包I64(需求包, 特征_执行差值(), 可执行输入包宿主, 特征_执行差值(), now);
        (void)已复制执行差值;
        const bool 已复制目标后值 = 复制包I64(
            输入条件包,
            特征_目标执行后值(),
            可执行输入包宿主,
            特征_目标执行后值(),
            now)
            || 复制包I64(需求包, 特征_目标执行后值(), 可执行输入包宿主, 特征_目标执行后值(), now);
        (void)已复制目标后值;
        (void)复制包I64(输入条件包, 特征_执行前值快照(), 可执行输入包宿主, 特征_执行前值快照(), now);
        const bool 已复制目标实例 = 复制包指针(
            输入条件包,
            特征_目标实例特征(),
            可执行输入包宿主,
            特征_目标实例特征(),
            now)
            || 复制包指针(需求包, 特征_目标实例特征(), 可执行输入包宿主, 特征_目标实例特征(), now);
        (void)已复制目标实例;
        (void)复制包I64(输入条件包, 特征_实例缺失策略(), 可执行输入包宿主, 特征_实例缺失策略(), now);
        (void)复制包I64(输入条件包, 特征_溢出策略(), 可执行输入包宿主, 特征_溢出策略(), now);
        (void)写入基础节点指针(可执行输入包宿主, 特征_方法配对结果(), 方法配对结果, now);
        (void)写入基础节点指针(可执行输入包宿主, 特征_需求条件包(), 需求包, now);

        const bool 成功 = 缺失数量 == 0;
        auto* 输出运行 = 输出配对参数结果(
            实际方法首节点,
            运行存在,
            输入参数场景,
            输出结果场景,
            方法配对结果,
            成功 ? 可执行输入场景 : nullptr,
            成功 ? 配对状态_配对成功 : 配对状态_配对失败,
            成功 ? 值_执行成功() : 值_参数绑定失败(),
            成功 ? nullptr : 值_必需参数缺失(),
            绑定数量,
            缺失数量,
            now);
        更新方法运行账(实际方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline bool 确认查找特征类型规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        (void)方法虚拟存在服务类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标抽象特征(), 值类型_特征类型指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_特征值范围(), 值类型_特征类型指针(), 值_来源_输入条件包(), true, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_特征类型配对结果()) && ok;
            ok = 取或创建子特征(宿主, 特征_类型抽象距离()) && ok;
            ok = 取或创建子特征(宿主, 特征_匹配路径记录()) && ok;
        }
        return ok;
    }

    inline 存在节点类* 查找特征类型(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto* 实际方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_查找特征类型,
            now);
        if (!实际方法首节点) return nullptr;
        auto* 运行存在 = 本轮运行虚拟存在(实际方法首节点, 输出结果场景, now);
        if (!运行存在) return nullptr;
        auto* 方法存在 = 方法虚拟存在(实际方法首节点, now);
        const bool 规格已确认 = 方法存在
            && 确认查找特征类型规格(实际方法首节点, 方法存在, now);

        auto* 输入条件 = 输入条件包根(输入参数场景);
        auto* 输入包 = reinterpret_cast<基础信息节点类*>(输入条件);
        auto* 需求特征 = 读取包抽象特征引用(
            输入包,
            特征_目标抽象特征());
        auto* 能力范围 = 读取包抽象特征引用(
            输入包,
            特征_特征值范围());
        结构_查找限制 限制{};
        if (auto* 限制包 = 查找包子节点(输入包, 特征_限制包())) {
            (void)解析查找限制包(限制包, 限制);
        }

        const auto 匹配 = 规格已确认
            ? 抽象节点可覆盖(
                需求特征,
                能力范围,
                能力范围 ? 基础节点类型词(能力范围) : nullptr,
                限制.最大类型抽象层数,
                限制.最高允许目标特征类型,
                限制.最高允许目标特征类型词)
            : 结构_类型匹配结果{};
        const bool 成功 = 规格已确认 && 匹配.成功;

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入方法执行情况(宿主, 成功 ? 值_执行成功() : 值_参数绑定失败(), now);
            (void)写入基础节点I64(宿主, 特征_配对状态(), 成功 ? 配对状态_配对成功 : 配对状态_配对失败, now);
            (void)写入基础节点I64(宿主, 特征_类型抽象距离(), 匹配.抽象距离, now);
            (void)写入基础节点指针(宿主, 特征_目标抽象特征(), 需求特征, now);
            (void)写入基础节点指针(宿主, 特征_特征值范围(), 能力范围, now);
            if (!成功) {
                (void)写入基础节点指针(
                    宿主,
                    特征_失败原因(),
                    匹配.失败原因 ? 匹配.失败原因 : 值_输入条件包绑定失败(),
                    now);
            }
        };
        写结果(reinterpret_cast<基础信息节点类*>(运行存在));
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景));
        if (方法存在) {
            写结果(reinterpret_cast<基础信息节点类*>(方法存在));
        }
        auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
        更新方法运行账(实际方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline bool 确认练习规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        (void)方法虚拟存在服务类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标方法头节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源缺口(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标特征类型(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标抽象特征(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_练习方向(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_执行差值(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标执行后值(), 值类型_I64(), 值_来源_输入条件包(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法补齐类别()) && ok;
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
        }
        return ok;
    }

    inline 方法类::节点类* 读取目标方法头节点(
        基础信息节点类* 输入包) noexcept
    {
        void* 指针 = nullptr;
        if (读取包指针(输入包, 特征_目标方法头节点(), 指针)
            || 读取包指针(输入包, 特征_缓存方法首节点(), 指针)
            || 读取包指针(输入包, 特征_选中方法首节点(), 指针)) {
            return reinterpret_cast<方法类::节点类*>(指针);
        }
        return nullptr;
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

    inline bool 词性相同(
        const 词性节点类* 左,
        const 词性节点类* 右) noexcept
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
        const auto* 值类型 = reinterpret_cast<const 词性节点类*>(值类型指针);
        return 词性相同(值类型, 值类型_I64())
            || 词性相同(值类型, 值类型_类型I64());
    }

    inline bool 形参为必需(基础信息节点类* 形参节点) noexcept
    {
        I64 是否必需值 = 1;
        (void)读取基础节点I64(形参节点, 特征_是否必需(), 是否必需值);
        return 是否必需值 != 0;
    }

    inline bool 读取参数允许区间(
        基础信息节点类* 形参节点,
        const 词性节点类* 参数特征类型,
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
        if (词性相同(参数特征类型, 特征_练习方向())
            || 词性相同(参数特征类型, 特征_改变方向())) {
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

    inline I64 生成区间内I64参数(
        I64 已生成数量,
        I64 下界,
        I64 上界,
        bool 有历史值,
        I64 历史值,
        I64& 输出策略) noexcept
    {
        输出策略 = 0;
        if (下界 > 上界) {
            下界 = 上界 = 0;
        }
        if (有历史值 && 下界 <= 历史值 && 历史值 <= 上界
            && 已生成数量 % 5 == 0) {
            输出策略 = 1;
            return 历史值;
        }
        switch (已生成数量 % 5) {
        case 1:
            输出策略 = 2;
            return 下界;
        case 2:
            输出策略 = 2;
            return 上界;
        case 3:
            输出策略 = 2;
            return 区间中点(下界, 上界);
        default:
            输出策略 = 3;
            break;
        }
        const I64 候选值 = 生成无目的未出现I64参数(已生成数量);
        if (候选值 < 下界) return 下界;
        if (候选值 > 上界) return 上界;
        return 候选值;
    }

    inline bool 复制任务参数到候选实参(
        基础信息节点类* 输入包,
        const 词性节点类* 参数特征类型,
        基础信息节点类* 候选参数,
        时间戳 now) noexcept
    {
        if (!输入包 || !参数特征类型 || !候选参数) return false;
        if (!复制包特征当前值(输入包, 参数特征类型, 候选参数, 参数特征类型, now)) {
            return false;
        }
        (void)写入基础节点指针(候选参数, 特征_参数来源(), 值_来源_任务提供参数(), now);
        return true;
    }

    inline bool 写入候选实参I64(
        基础信息节点类* 实参表宿主,
        const 词性节点类* 参数特征类型,
        I64 参数值,
        const 词性节点类* 参数来源,
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
        const 词性节点类* 参数特征类型,
        const 词性节点类* 参数值类型,
        void* 参数值,
        const 词性节点类* 参数来源,
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
        const 词性节点类* 参数特征类型,
        const 词性节点类* 参数值类型,
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
        const 词性节点类* 参数特征类型,
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

    inline bool 生成单个候选实参(
        基础信息节点类* 输入包,
        基础信息节点类* 形参节点,
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

        if (参数值类型为I64(形参节点)) {
            I64 下界 = 0;
            I64 上界 = 0;
            (void)读取参数允许区间(形参节点, 参数特征类型, 下界, 上界);
            I64 历史值 = 0;
            const bool 有历史值 = 世界树.读取特征_I64(候选参数, 历史值);
            I64 策略 = 0;
            const I64 生成值 = 生成区间内I64参数(
                已生成数量,
                下界,
                上界,
                有历史值,
                历史值,
                策略);
            (void)世界树.写入特征_I64(候选参数, 生成值, now);
            (void)写入基础节点指针(候选宿主, 特征_参数来源(), 值_来源_练习自动生成(), now);
            (void)写入基础节点I64(候选宿主, 特征_区间下界(), 下界, now);
            (void)写入基础节点I64(候选宿主, 特征_区间上界(), 上界, now);
            (void)写入基础节点I64(候选宿主, 特征_实参生成策略(), 策略, now);
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
                实参表宿主,
                已生成数量 + 绑定参数数量 + 缺失参数数量,
                now,
                绑定参数数量,
                缺失参数数量);
            当前 = static_cast<基础信息节点类*>(当前->下);
        } while (当前 && 当前 != 首子);
        return 已遍历;
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

    inline 特征节点类* 生成练习候选实参表(
        基础信息节点类* 目标方法宿主,
        基础信息节点类* 输入包,
        时间戳 now,
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

        if (auto* 形参表 = 查找子特征(目标方法宿主, 特征_形参表())) {
            if (生成形参表候选实参(
                    reinterpret_cast<基础信息节点类*>(形参表),
                    输入包,
                    宿主,
                    已生成数量,
                    now,
                    输出绑定参数数量,
                    输出缺失参数数量)) {
                (void)写入基础节点I64(宿主, 特征_绑定参数数量(), 输出绑定参数数量, now);
                (void)写入基础节点I64(宿主, 特征_缺失参数数量(), 输出缺失参数数量, now);
                (void)写入基础节点I64(宿主, 特征_候选实参数量(), 输出候选实参数量, now);
                (void)写入基础节点I64(目标方法宿主, 特征_候选实参数量(), 输出候选实参数量, now);
                return 实参表;
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
        (void)写入基础节点I64(宿主, 特征_候选实参数量(), 输出候选实参数量, now);
        (void)写入基础节点I64(宿主, 特征_绑定参数数量(), 输出绑定参数数量, now);
        (void)写入基础节点I64(宿主, 特征_缺失参数数量(), 0, now);
        (void)写入基础节点I64(目标方法宿主, 特征_候选实参数量(), 输出候选实参数量, now);
        return 实参表;
    }

    inline bool 确认尝试学习规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        (void)方法虚拟存在服务类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标方法头节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源因果信息(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源观察动态(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_条件结果对(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_动作主体(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法补齐类别()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法补齐尝试结果()) && ok;
            ok = 取或创建子特征(宿主, 特征_内化状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_动作主体替换状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_条件结果对()) && ok;
        }
        return ok;
    }

    inline bool 确认定向学习规格(
        方法类::节点类* 方法首节点,
        存在节点类* 方法存在,
        时间戳 now) noexcept
    {
        if (!方法首节点 || !方法存在) return false;
        (void)方法虚拟存在服务类::初始化方法虚拟存在信息(方法首节点, nullptr, now);
        bool ok = true;
        ok = 声明形参规格(方法首节点, 特征_输入条件包(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标方法头节点(), 值类型_场景指针(), 值_来源_输入条件包(), true, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标特征类型(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标抽象特征(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_目标特征(), 值类型_特征类型指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源因果信息(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        ok = 声明形参规格(方法首节点, 特征_来源观察动态(), 值类型_场景指针(), 值_来源_输入条件包(), false, now) && ok;
        if (auto* 结果根 = 方法规格根(方法首节点, 特征_结果表(), now)) {
            auto* 宿主 = reinterpret_cast<基础信息节点类*>(结果根);
            ok = 取或创建子特征(宿主, 特征_方法补齐类别()) && ok;
            ok = 取或创建子特征(宿主, 特征_候选动作节点()) && ok;
            ok = 取或创建子特征(宿主, 特征_条件结果对()) && ok;
            ok = 取或创建子特征(宿主, 特征_预测状态()) && ok;
            ok = 取或创建子特征(宿主, 特征_方法补齐结构状态()) && ok;
        }
        return ok;
    }

    inline bool 方法具有本能执行入口(方法类::节点类* 方法首节点) noexcept
    {
        return 方法首节点
            && 方法首节点->主信息.首节点信息().动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && 方法首节点->主信息.首节点信息().动作句柄.本能ID != 0;
    }

    inline 基础信息节点类* 读取方法补齐证据节点(基础信息节点类* 输入包) noexcept
    {
        if (!输入包) return nullptr;
        const 词性节点类* 特征集[] = {
            特征_来源因果信息(),
            特征_来源观察动态(),
            特征_条件结果对(),
            特征_来源缺口()
        };
        for (const auto* 特征类型 : 特征集) {
            if (auto* 节点 = 读取包基础引用或自身(输入包, 特征类型)) {
                return 节点;
            }
        }
        return nullptr;
    }

    inline 基础信息节点类* 读取方法补齐目标特征节点(基础信息节点类* 输入包) noexcept
    {
        if (!输入包) return nullptr;
        const 词性节点类* 特征集[] = {
            特征_目标抽象特征(),
            特征_目标特征类型(),
            特征_目标特征()
        };
        for (const auto* 特征类型 : 特征集) {
            void* 指针 = nullptr;
            if (读取包指针(输入包, 特征类型, 指针)) {
                if (auto* 节点 = 指针若为基础信息节点(指针)) {
                    return 节点;
                }
                if (auto* 抽象特征 = 查找抽象特征_按类型(
                        reinterpret_cast<const 词性节点类*>(指针))) {
                    return reinterpret_cast<基础信息节点类*>(抽象特征);
                }
            }
            if (auto* 节点 = 读取包基础引用或自身(输入包, 特征类型)) {
                return 节点;
            }
        }
        return nullptr;
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
        auto* 实际方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_尝试学习,
            now);
        if (!实际方法首节点) return nullptr;

        auto* 运行存在 = 本轮运行虚拟存在(实际方法首节点, 输出结果场景, now);
        if (!运行存在) return nullptr;

        auto* 方法存在 = 方法虚拟存在(实际方法首节点, now);
        const bool 规格已确认 = 方法存在
            && 确认尝试学习规格(实际方法首节点, 方法存在, now);

        auto* 输入条件 = 输入条件包根(输入参数场景);
        auto* 输入包 = reinterpret_cast<基础信息节点类*>(输入条件);
        auto* 目标方法 = 读取目标方法头节点(输入包);
        auto* 目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        auto* 因果证据 = 读取方法补齐证据节点(输入包);
        const bool 有入口 = 方法具有本能执行入口(目标方法);

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

        auto 目标条件结果节点 = 目标方法
            ? 构建方法条件和结果节点(目标方法, 输入参数场景, 输出结果场景, now)
            : 结构_方法条件结果节点{};

        if (条件结果对) {
            auto* 对宿主 = reinterpret_cast<基础信息节点类*>(条件结果对);
            (void)写入基础节点I64(对宿主, 特征_方法补齐类别(), 1, now);
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

        const bool 成功 = 规格已确认
            && 输入包
            && 目标方法
            && 目标方法存在
            && 因果证据
            && 有入口
            && 已补结构
            && 条件结果对;
        const auto* 失败原因 = !输入包
            ? 值_输入条件包绑定失败()
            : (!目标方法
                ? 值_目标方法头节点缺失()
                : (!因果证据
                    ? 值_因果证据不足()
                    : (!有入口
                        ? 值_自我动作缺失()
                        : (!已补结构 ? 值_方法虚拟存在缺失() : nullptr))));

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            const auto* 执行情况 = 成功
                ? 值_执行成功()
                : ((!输入包 || !目标方法 || !因果证据)
                    ? 值_参数绑定失败()
                    : 值_执行失败());
            (void)写入方法执行情况(宿主, 执行情况, now);
            (void)写入基础节点I64(宿主, 特征_方法补齐类别(), 1, now);
            (void)写入基础节点I64(宿主, 特征_方法补齐状态(), 成功 ? 1 : 0, now);
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
            if (世界树.自我指针) {
                (void)写入基础节点指针(宿主, 特征_动作主体(), 世界树.自我指针, now);
            }
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        };
        写结果(reinterpret_cast<基础信息节点类*>(运行存在));
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景));
        if (方法存在) {
            写结果(reinterpret_cast<基础信息节点类*>(方法存在));
        }
        if (目标方法存在) {
            写结果(reinterpret_cast<基础信息节点类*>(目标方法存在));
        }

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
        auto* 实际方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_定向学习,
            now);
        if (!实际方法首节点) return nullptr;

        auto* 运行存在 = 本轮运行虚拟存在(实际方法首节点, 输出结果场景, now);
        if (!运行存在) return nullptr;

        auto* 方法存在 = 方法虚拟存在(实际方法首节点, now);
        const bool 规格已确认 = 方法存在
            && 确认定向学习规格(实际方法首节点, 方法存在, now);

        auto* 输入条件 = 输入条件包根(输入参数场景);
        auto* 输入包 = reinterpret_cast<基础信息节点类*>(输入条件);
        auto* 目标方法 = 读取目标方法头节点(输入包);
        auto* 目标方法存在 = 目标方法 ? 方法虚拟存在(目标方法, now) : nullptr;
        auto* 目标特征 = 读取方法补齐目标特征节点(输入包);
        auto* 观察证据 = 读取方法补齐证据节点(输入包);

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

        auto 目标条件结果节点 = 目标方法
            ? 构建方法条件和结果节点(目标方法, 输入参数场景, 输出结果场景, now)
            : 结构_方法条件结果节点{};

        if (条件结果对) {
            auto* 对宿主 = reinterpret_cast<基础信息节点类*>(条件结果对);
            (void)写入基础节点I64(对宿主, 特征_方法补齐类别(), 2, now);
            (void)写入基础节点I64(对宿主, 特征_方法补齐结构状态(), 1, now);
            (void)写入基础节点指针(对宿主, 特征_目标抽象特征(), 目标特征, now);
            (void)写入基础节点指针(对宿主, 特征_候选动作节点(), 目标方法, now);
            (void)写入基础节点指针(对宿主, 特征_预测状态(), 值_候选动作未验证(), now);
            if (观察证据) {
                (void)写入基础节点指针(对宿主, 特征_来源观察动态(), 观察证据, now);
            }
            if (目标条件结果节点.条件节点) {
                (void)写入基础节点指针(对宿主, 特征_方法条件节点(), 目标条件结果节点.条件节点, now);
            }
            if (目标条件结果节点.结果节点) {
                (void)写入基础节点指针(对宿主, 特征_方法结果节点(), 目标条件结果节点.结果节点, now);
            }
        }

        const bool 成功 = 规格已确认
            && 输入包
            && 目标方法
            && 目标方法存在
            && 目标特征
            && 已补结构
            && 条件结果对;
        const auto* 失败原因 = !输入包
            ? 值_输入条件包绑定失败()
            : (!目标方法
                ? 值_目标方法头节点缺失()
                : (!目标特征
                    ? 值_目标特征缺失()
                    : (!已补结构 ? 值_方法虚拟存在缺失() : nullptr)));

        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            const auto* 执行情况 = 成功
                ? 值_执行成功()
                : ((!输入包 || !目标方法 || !目标特征)
                    ? 值_参数绑定失败()
                    : 值_执行失败());
            (void)写入方法执行情况(宿主, 执行情况, now);
            (void)写入基础节点I64(宿主, 特征_方法补齐类别(), 2, now);
            (void)写入基础节点I64(宿主, 特征_方法补齐状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法补齐结构状态(), 已补结构 ? 1 : 0, now);
            (void)写入基础节点指针(宿主, 特征_目标方法头节点(), 目标方法, now);
            (void)写入基础节点指针(宿主, 特征_目标抽象特征(), 目标特征, now);
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
        if (方法存在) {
            写结果(reinterpret_cast<基础信息节点类*>(方法存在));
        }
        if (目标方法存在) {
            写结果(reinterpret_cast<基础信息节点类*>(目标方法存在));
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
        auto* 实际方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_练习,
            now);
        if (!实际方法首节点) return nullptr;
        auto* 运行存在 = 本轮运行虚拟存在(实际方法首节点, 输出结果场景, now);
        if (!运行存在) return nullptr;
        auto* 方法存在 = 方法虚拟存在(实际方法首节点, now);
        const bool 规格已确认 = 方法存在
            && 确认练习规格(实际方法首节点, 方法存在, now);

        auto* 输入条件 = 输入条件包根(输入参数场景);
        auto* 输入包 = reinterpret_cast<基础信息节点类*>(输入条件);
        auto* 目标方法 = 读取目标方法头节点(输入包);
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
            目标条件结果节点 = 构建方法条件和结果节点(
                目标方法,
                输入参数场景,
                输出结果场景,
                now);
            已补结构 = 目标条件结果节点.条件节点 != nullptr
                || 目标条件结果节点.结果节点 != nullptr
                || 已补结构;
            if (条件结果对) {
                auto* 对宿主 = reinterpret_cast<基础信息节点类*>(条件结果对);
                if (目标条件结果节点.条件节点) {
                    (void)写入基础节点指针(
                        对宿主,
                        特征_方法条件节点(),
                        目标条件结果节点.条件节点,
                        now);
                }
                if (目标条件结果节点.结果节点) {
                    (void)写入基础节点指针(
                        对宿主,
                        特征_方法结果节点(),
                        目标条件结果节点.结果节点,
                        now);
                }
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
        const bool 可试运行 = 有入口
            && 目标方法 != 实际方法首节点
            && 目标本能ID != 枚举_本能方法ID::自我_练习
            && 目标本能ID != 枚举_本能方法ID::自我_尝试学习
            && 目标本能ID != 枚举_本能方法ID::自我_定向学习;
        if (可试运行 && 可执行输入场景 && 缺失参数数量 == 0) {
            目标方法运行结果 = 取本能动作集().执行功能函数(
                static_cast<枚举_本能动作ID>(目标本能ID),
                目标方法,
                可执行输入场景,
                输出结果场景);
        }
        const bool 目标执行成功 = 本能动作运行成功(目标方法运行结果);
        const bool 成功 = 规格已确认
            && 目标方法
            && 已补结构
            && 可试运行
            && 候选实参表
            && 缺失参数数量 == 0
            && 可执行输入场景
            && 目标执行成功;
        const auto* 失败原因 = !输入包
            ? 值_输入条件包绑定失败()
            : (!目标方法
            ? 值_目标方法头节点缺失()
            : (!有入口
                ? 值_缺少执行入口()
                : (!可试运行
                    ? 值_候选动作未验证()
                    : (缺失参数数量 > 0
                        ? 值_参数条件缺口()
                        : (!可执行输入场景
                            ? 值_可执行输入参数场景创建失败()
                            : (目标方法运行结果 && !目标执行成功 ? 值_执行失败() : nullptr))))));
        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入方法执行情况(
                宿主,
                成功 ? 值_执行成功()
                    : ((!输入包 || !目标方法 || 缺失参数数量 > 0)
                        ? 值_参数绑定失败()
                        : 值_执行失败()),
                now);
            (void)写入基础节点I64(宿主, 特征_方法补齐类别(), 3, now);
            (void)写入基础节点I64(宿主, 特征_方法补齐状态(), 成功 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_方法补齐结构状态(), 成功 ? 2 : (已补结构 ? 1 : 0), now);
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
            (void)写入基础节点指针(
                宿主,
                特征_方法条件节点(),
                目标条件结果节点.条件节点,
                now);
            (void)写入基础节点指针(
                宿主,
                特征_方法结果节点(),
                目标条件结果节点.结果节点,
                now);
            if (失败原因) {
                (void)写入基础节点指针(宿主, 特征_失败原因(), 失败原因, now);
            }
        };
        写结果(reinterpret_cast<基础信息节点类*>(运行存在));
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景));
        if (方法存在) {
            写结果(reinterpret_cast<基础信息节点类*>(方法存在));
        }
        if (目标方法存在) {
            写结果(reinterpret_cast<基础信息节点类*>(目标方法存在));
        }
        auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
        更新方法运行账(实际方法首节点, 输出运行, 成功, now);
        return 输出运行;
    }

    inline 存在节点类* 检查需求是否满足(
        方法类::节点类* 方法首节点,
        场景节点类* 输入参数场景,
        场景节点类* 输出结果场景) noexcept
    {
        const auto now = 结构体_时间戳::当前_微秒();
        auto* 实际方法首节点 = 确保本能方法(
            方法首节点,
            枚举_本能方法ID::自我_检查需求是否满足,
            now);
        if (!实际方法首节点) return nullptr;
        auto* 运行存在 = 本轮运行虚拟存在(实际方法首节点, 输出结果场景, now);
        if (!运行存在) return nullptr;
        auto* 方法存在 = 方法虚拟存在(实际方法首节点, now);

        auto* 输入条件 = 输入条件包根(输入参数场景);
        auto* 输入包 = reinterpret_cast<基础信息节点类*>(输入条件);
        auto* 需求包 = 读取包基础引用或自身(输入包, 特征_需求条件包());
        const auto 需求项列表 = 读取条件项列表(需求包);
        const auto 需求条件项被输出满足 = [&](const 结构_条件项视图& 需求项) noexcept {
            if (!需求项.条件特征类型 || !输出结果场景) return false;
            auto* 输出特征 = 查找子特征(
                reinterpret_cast<基础信息节点类*>(输出结果场景),
                需求项.条件特征类型);
            if (!输出特征) return false;
            if (!需求项.有I64点值 && !需求项.有I64区间) {
                return true;
            }
            I64 输出值 = 0;
            if (!世界树.读取特征_I64(输出特征, 输出值)) return false;
            if (需求项.有I64点值 && 输出值 != 需求项.I64点值) return false;
            if (需求项.有I64区间
                && (输出值 < 需求项.区间下界 || 输出值 > 需求项.区间上界)) {
                return false;
            }
            return true;
        };
        I64 必需项数量 = 0;
        I64 满足项数量 = 0;
        for (const auto& 项 : 需求项列表) {
            if (!项.必需) continue;
            ++必需项数量;
            if (需求条件项被输出满足(项)) {
                ++满足项数量;
            }
        }
        const bool 满足 = 必需项数量 > 0 && 满足项数量 == 必需项数量;
        auto 写结果 = [&](基础信息节点类* 宿主) noexcept {
            if (!宿主) return;
            (void)写入方法执行情况(宿主, 满足 ? 值_执行成功() : 值_执行失败(), now);
            (void)写入基础节点I64(宿主, 特征_需求满足情况(), 满足 ? 1 : 0, now);
            (void)写入基础节点I64(宿主, 特征_绑定参数数量(), 满足项数量, now);
            (void)写入基础节点I64(宿主, 特征_缺失参数数量(), 必需项数量 - 满足项数量, now);
            (void)写入基础节点指针(宿主, 特征_需求条件包(), 需求包, now);
            (void)写入基础节点指针(宿主, 特征_满足证据(), 满足 ? 值_需求满足() : 值_需求未满足(), now);
        };
        写结果(reinterpret_cast<基础信息节点类*>(运行存在));
        写结果(reinterpret_cast<基础信息节点类*>(输出结果场景));
        if (方法存在) {
            写结果(reinterpret_cast<基础信息节点类*>(方法存在));
        }
        auto* 输出运行 = 输出运行虚拟存在(运行存在, 输入参数场景, 输出结果场景, now);
        更新方法运行账(实际方法首节点, 输出运行, 满足, now);
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
            更新方法运行账(实际方法首节点, 输出运行, false, now);
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
            更新方法运行账(实际方法首节点, 输出运行, false, now);
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
            更新方法运行账(实际方法首节点, 输出运行, false, now);
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
            更新方法运行账(实际方法首节点, 输出运行, false, now);
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
            更新方法运行账(实际方法首节点, 输出运行, false, now);
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
            更新方法运行账(实际方法首节点, 输出运行, false, now);
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
                更新方法运行账(实际方法首节点, 输出运行, false, now);
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
            更新方法运行账(实际方法首节点, 输出运行, false, now);
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
                更新方法运行账(实际方法首节点, 输出运行, false, now);
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
                更新方法运行账(实际方法首节点, 输出运行, false, now);
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
            更新方法运行账(实际方法首节点, 输出运行, false, now);
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
            更新方法运行账(实际方法首节点, 输出运行, false, now);
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
            更新方法运行账(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

        auto* 当前命中抽象特征 = 解析实例特征命中抽象特征(
            输入帧.目标实例特征,
            now);
        if (!当前命中抽象特征) {
            auto* 输出运行 = 输出改变特征值失败(
                实际方法首节点,
                运行存在,
                输入参数场景,
                输出结果场景,
                &输入帧,
                值_执行失败(),
                值_实例特征未命中抽象特征(),
                类型_需求_重新配对参数或调整目标值(),
                now);
            更新方法运行账(实际方法首节点, 输出运行, false, now);
            return 输出运行;
        }

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
        更新方法运行账(实际方法首节点, 输出运行, true, now);
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
        更新方法运行账(方法首节点, 输出运行, 执行成功, now);
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
        更新方法运行账(方法首节点, 输出运行, 执行成功, now);
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
            更新方法运行账(方法首节点, 输出运行, false, now);
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
        更新方法运行账(方法首节点, 输出运行, 执行成功, now);
        return 输出运行;
    }
}
