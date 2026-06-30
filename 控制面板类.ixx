module;

// 文件头部规则注释模块：
// 1. 本模块定义控制面板只读快照、命令和显示入口，不承载业务权威事实。
// 2. 控制面板字段只能作为显示、诊断和人工复核材料，不能反向作为世界树、需求、任务或动作动态判断源。
// 3. 自我场景相关字段读取自我所在场景和复现宿主；场景体素、候选和诊断区域仅作调试辅助。

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

export module 控制面板类;

export {

struct 结构_控制面板树节点 {
    std::uintptr_t 节点指针 = 0;
    std::uintptr_t 附加参数 = 0;
    std::string 文本{};
    bool 默认展开 = false;
    bool 可延迟展开 = false;
    bool 是字段分组 = false;
    std::string 展开类型{};
    std::vector<结构_控制面板树节点> 子项{};
    std::vector<结构_控制面板树节点> 详情{};
};

struct 结构_控制面板诊断区域 {
    std::int64_t 区域编号 = 0;
    std::int64_t 图层类型 = 0;
    std::int64_t 来源候选编号 = -1;
    std::int64_t 投影最小X = 0;
    std::int64_t 投影最大X = 0;
    std::int64_t 投影最小Y = 0;
    std::int64_t 投影最大Y = 0;
    std::int64_t 像素数量 = 0;
    std::int64_t 置信度 = 0;
    std::int64_t 掩码状态 = 0;
};

struct 结构_控制面板自我场景存在复现项 {
    std::uintptr_t 节点指针 = 0;
    std::string 标题{};
    std::string 类型{};
    std::int64_t 来源范围 = 0;
    std::int64_t 来源空间候选编号 = -1;
    std::int64_t 外设观察报告ID = 0;
    std::int64_t 外设观察像素簇ID = 0;
    std::int64_t 几何状态 = 0;
    std::int64_t 中心X = 0;
    std::int64_t 中心Y = 0;
    std::int64_t 中心Z = 0;
    std::int64_t AABB最小X = 0;
    std::int64_t AABB最大X = 0;
    std::int64_t AABB最小Y = 0;
    std::int64_t AABB最大Y = 0;
    std::int64_t AABB最小Z = 0;
    std::int64_t AABB最大Z = 0;
    std::int64_t 尺寸X = 0;
    std::int64_t 尺寸Y = 0;
    std::int64_t 尺寸Z = 0;
    std::int64_t 投影最小X = 0;
    std::int64_t 投影最大X = 0;
    std::int64_t 投影最小Y = 0;
    std::int64_t 投影最大Y = 0;
    std::int64_t 观察存在确认状态 = 0;
    std::int64_t 像素归属验证状态 = 0;
    std::int64_t 可绘制状态 = 0;
    std::int64_t 颜色RGB结构状态 = 0;
    std::int64_t 像素颜色层状态 = 0;
    std::int64_t 颜色缓冲状态 = 0;
    std::int64_t 彩图轮廓数量 = 0;
    std::int64_t 轮廓颜色支持率 = 0;
    std::int64_t 局部轮廓材料可回查状态 = 0;
    std::int64_t 平面轮廓状态 = 0;
    std::int64_t 空间极值轮廓状态 = 0;
    std::int64_t 平面轮廓点数 = 0;
    std::int64_t 空间极值轮廓点数 = 0;
};

struct 结构_控制面板自我场景体素块 {
    std::int64_t 最小X = 0;
    std::int64_t 最小Y = 0;
    std::int64_t 最小Z = 0;
    std::int64_t 最大X = 0;
    std::int64_t 最大Y = 0;
    std::int64_t 最大Z = 0;
    std::uint32_t 层级 = 0;
    std::uint32_t 状态 = 0;
    std::uint64_t 占据体素数量 = 0;
};

struct 结构_控制面板自我场景体素复现项 {
    std::uintptr_t 存在指针 = 0;
    std::uintptr_t 体素特征指针 = 0;
    std::uintptr_t 体素根句柄 = 0;
    std::string 标题{};
    std::int64_t 原点X = 0;
    std::int64_t 原点Y = 0;
    std::int64_t 原点Z = 0;
    std::uint32_t 最小体素边长_mm = 0;
    std::uint32_t 根最大层级 = 0;
    std::uint32_t 根边长体素 = 0;
    std::uint32_t 原始宽度 = 0;
    std::uint32_t 原始高度 = 0;
    std::uint32_t 原始深度 = 0;
    std::uint64_t 根占据体素数量 = 0;
    std::uint64_t 解码体素块总数 = 0;
    std::vector<结构_控制面板自我场景体素块> 体素块列表{};
};

struct 结构_控制面板快照 {
    bool 世界树已初始化 = false;
    bool 自我已初始化 = false;
    bool 自我线程已初始化 = false;
    bool 自我线程运行中 = false;
    bool 自我线程健康运行 = false;
    bool 自我存在已建立 = false;
    bool 自我内部世界已建立 = false;
    bool 自我需求根字段已建立 = false;
    bool 自我任务根字段已建立 = false;
    bool 自我方法根字段已建立 = false;
    bool 自我待机状态 = false;
    std::uint8_t 自我线程生命周期 = 0;

    std::size_t 基础信息节点数 = 0;
    std::size_t 场景数 = 0;
    std::size_t 存在数 = 0;
    std::size_t 特征数 = 0;
    std::size_t 状态数 = 0;
    std::size_t 动态数 = 0;
    std::size_t 因果模板数 = 0;
    std::size_t 因果证据动态样本数 = 0;
    std::size_t 需求数 = 0;
    std::size_t 任务数 = 0;
    std::size_t 任务头节点数 = 0;
    std::size_t 任务步骤节点数 = 0;
    std::size_t 任务其他节点数 = 0;
    std::size_t 任务状态未定义数 = 0;
    std::size_t 任务状态未启动数 = 0;
    std::size_t 任务状态运行中数 = 0;
    std::size_t 任务状态挂起数 = 0;
    std::size_t 任务状态完成数 = 0;
    std::size_t 任务状态已结算数 = 0;
    std::size_t 任务状态失败数 = 0;
    std::size_t 任务状态取消数 = 0;
    std::size_t 任务状态超时数 = 0;
    std::size_t 任务状态就绪数 = 0;
    std::size_t 任务状态执行中数 = 0;
    std::size_t 任务状态筹办中数 = 0;
    std::size_t 任务状态排队中数 = 0;
    std::size_t 任务状态等待中数 = 0;
    std::size_t 任务状态无法执行数 = 0;
    std::size_t 任务状态待重筹办数 = 0;
    std::size_t 方法数 = 0;
    std::size_t 方法首节点数 = 0;
    std::size_t 方法条件无结果数 = 0;
    std::size_t 方法结果无条件数 = 0;
    std::size_t 方法条件结果不成对数 = 0;
    std::size_t 线程数 = 0;

    std::size_t 需求本轮已截止数 = 0;
    std::size_t 需求状态已达未截止数 = 0;
    std::size_t 需求未满足数 = 0;
    std::size_t 需求树有子节点数 = 0;
    std::size_t 需求树叶子节点数 = 0;
    std::size_t 需求树活动阻塞需求数 = 0;
    std::size_t 需求树活动叶子需求数 = 0;
    std::size_t 需求树活动叶子需求未任务化数 = 0;
    std::string 需求树首个未任务化叶子需求主键{};
    std::string 需求树首个未任务化叶子需求父主键{};
    std::string 需求树首个未任务化叶子需求目标主体主键{};
    std::string 需求树首个未任务化叶子需求目标特征主键{};
    std::size_t 需求树目标绑定完整需求数 = 0;
    std::size_t 需求树目标绑定异常需求数 = 0;
    std::size_t 需求树缺目标状态需求数 = 0;
    std::size_t 需求树缺目标宿主需求数 = 0;
    std::size_t 需求树缺目标特征类型需求数 = 0;
    std::size_t 需求树目标状态宿主不一致需求数 = 0;
    std::size_t 需求树目标特征未归属宿主需求数 = 0;
    std::string 需求树首个目标绑定异常需求主键{};
    std::string 需求树首个目标绑定异常父主键{};
    std::string 需求树首个目标绑定异常目标状态主键{};
    std::string 需求树首个目标绑定异常目标宿主主键{};
    std::string 需求树首个目标绑定异常目标特征主键{};
    std::size_t 需求树非阻塞需求数 = 0;
    std::size_t 需求树已截止需求数 = 0;
    std::size_t 需求树逻辑组织需求数 = 0;
    std::size_t 需求树AND组需求数 = 0;
    std::size_t 需求树OR组需求数 = 0;
    std::size_t 需求树方法路径组需求数 = 0;
    std::size_t 需求树因果子链支撑需求数 = 0;
    std::size_t 需求树OR组结算令牌需求数 = 0;
    bool 需求树存在重复目标风险 = false;
    std::size_t 需求树重复目标组数 = 0;
    std::size_t 需求树重复目标需求数 = 0;
    bool 需求树父子权重异常 = false;
    std::size_t 需求树父子权重检查父需求数 = 0;
    std::size_t 需求树父子权重异常需求数 = 0;
    std::size_t 需求树完成任务仍活动需求数 = 0;
    std::string 需求树首个完成任务仍活动需求主键{};
    std::string 需求树首个完成任务仍活动任务主键{};
    std::size_t 任务完成但缺对应需求数 = 0;
    std::string 首个完成但缺对应需求任务主键{};
    bool 需求树当前主需求有任务 = false;
    std::string 需求树当前主需求主键{};
    std::string 需求树当前主需求父主键{};
    std::string 需求树当前主需求目标主体主键{};
    std::string 需求树当前主需求目标特征主键{};
    std::string 需求树当前主需求结构形态{};
    bool 需求树当前主需求可普通任务化 = false;
    std::string 需求树当前主需求OR令牌状态{};
    std::string 需求树当前主需求当前激活路径{};

    std::int64_t 自我安全值 = 0;
    std::int64_t 自我服务值 = 0;
    std::int64_t 自我风险安全值 = 0;
    std::int64_t 自我待处理方法数量 = 0;
    std::int64_t 自我可用方法数量 = 0;
    std::uint64_t 自我Tick计数 = 0;
    std::uint64_t 自我线程累计故障次数 = 0;
    std::uint64_t 自我线程累计恢复次数 = 0;

    std::uintptr_t 自我存在指针 = 0;
    std::uintptr_t 当前主需求指针 = 0;
    std::uintptr_t 当前主任务指针 = 0;
    std::uintptr_t 当前主方法指针 = 0;

    bool 自我场景复现有快照 = false;
    std::uintptr_t 自我所在场景指针 = 0;
    std::uintptr_t 自我场景复现宿主指针 = 0;
    std::uintptr_t 自我场景安全评估宿主指针 = 0;
    std::string 自我所在场景标题{};
    std::string 自我场景复现宿主标题{};
    std::string 自我场景安全评估宿主标题{};
    std::int64_t 自我场景直接子节点数量 = 0;
    std::int64_t 自我场景子树节点数量 = 0;
    std::int64_t 自我场景直接存在数量 = 0;
    std::int64_t 自我场景子树存在数量 = 0;
    std::int64_t 自我场景子树场景数量 = 0;
    std::int64_t 自我场景子树特征数量 = 0;
    std::int64_t 自我场景子树状态数量 = 0;
    std::int64_t 自我场景子树动态数量 = 0;
    std::int64_t 自我场景子树二次特征数量 = 0;
    std::int64_t 自我场景复现宿主子树节点数量 = 0;
    std::int64_t 自我场景复现宿主子树存在数量 = 0;
    std::int64_t 自我场景子树扫描上限 = 4096;
    std::int64_t 自我场景子树扫描达到上限 = 0;
    std::int64_t 自我场景复现宿主子树扫描达到上限 = 0;
    std::string 自我场景直接子层摘要{};
    std::string 自我场景子树类型摘要{};
    std::string 自我场景存在样例摘要{};
    std::string 自我场景复现宿主存在样例摘要{};
    std::int64_t 自我场景当前观察帧 = 0;
    std::int64_t 自我场景相机帧宽度 = 0;
    std::int64_t 自我场景相机帧高度 = 0;
    std::int64_t 自我场景深度帧号 = 0;
    std::int64_t 自我场景彩色帧号 = 0;
    std::int64_t 自我场景预期像素数量 = 0;
    std::int64_t 自我场景像素特征数量 = 0;
    std::int64_t 自我场景深度有效像素数量 = 0;
    std::int64_t 自我场景点云有效像素数量 = 0;
    std::int64_t 自我场景像素覆盖状态 = 0;
    std::int64_t 自我场景空间坐标单位毫米 = 1;
    std::int64_t 自我场景颜色RGB结构状态 = 0;
    std::int64_t 自我场景原始深度毫米结构状态 = 0;
    std::int64_t 自我场景滤波深度毫米结构状态 = 0;
    std::int64_t 自我场景补全深度毫米结构状态 = 0;
    std::int64_t 自我场景深度有效性Mask状态 = 0;
    std::int64_t 自我场景深度来源结构状态 = 0;
    std::int64_t 自我场景深度稳定性结构状态 = 0;
    std::int64_t 自我场景深度邻域一致性结构状态 = 0;
    std::int64_t 自我场景融合深度毫米结构状态 = 0;
    std::int64_t 自我场景融合深度有效性结构状态 = 0;
    std::int64_t 自我场景多帧深度稳定性结构状态 = 0;
    std::int64_t 自我场景每像素深度方差结构状态 = 0;
    std::int64_t 自我场景空间坐标毫米XYZ结构状态 = 0;
    std::int64_t 自我场景原始深度来源像素数量 = 0;
    std::int64_t 自我场景滤波深度来源像素数量 = 0;
    std::int64_t 自我场景补全深度来源像素数量 = 0;
    std::int64_t 自我场景无有效深度来源像素数量 = 0;
    std::int64_t 自我场景补全深度低置信像素数量 = 0;
    std::int64_t 自我场景深度稳定性平均值 = 0;
    std::int64_t 自我场景深度邻域一致性平均值 = 0;
    std::int64_t 自我场景观察帧组状态 = 0;
    std::int64_t 自我场景观察帧组帧数 = 0;
    std::int64_t 自我场景融合依据帧集合状态 = 0;
    std::int64_t 自我场景融合深度有效像素数量 = 0;
    std::int64_t 自我场景融合深度有效率 = 0;
    std::int64_t 自我场景融合补偿深度空洞像素数量 = 0;
    std::int64_t 自我场景融合低稳定像素数量 = 0;
    std::int64_t 自我场景融合深度稳定性平均值 = 0;
    std::int64_t 自我场景融合深度平均方差 = 0;
    std::int64_t 自我场景彩色深度已对齐 = 0;
    std::int64_t 自我场景深度有效率 = 0;
    std::int64_t 自我场景空间坐标有效率 = 0;
    std::int64_t 自我场景帧质量评分 = 0;
    std::int64_t 自我场景深度空洞数量 = 0;
    std::int64_t 自我场景深度空洞区域数量 = 0;
    std::int64_t 自我场景轮廓闭合率 = 0;
    std::int64_t 自我场景轮廓断裂数量 = 0;
    std::int64_t 自我场景彩图轮廓数量 = 0;
    std::int64_t 自我场景深度轮廓数量 = 0;
    std::int64_t 自我场景空间投影轮廓数量 = 0;
    std::int64_t 自我场景融合轮廓数量 = 0;
    std::int64_t 自我场景轮廓来源分层状态 = 0;
    std::int64_t 自我场景轮廓深度支持率 = 0;
    std::int64_t 自我场景轮廓颜色支持率 = 0;
    std::int64_t 自我场景轮廓空间支持率 = 0;
    std::int64_t 自我场景融合轮廓置信度 = 0;
    std::int64_t 自我场景跨越深度断裂轮廓数量 = 0;
    std::int64_t 自我场景穿过深度无效区域轮廓数量 = 0;
    std::int64_t 自我场景依赖补全深度轮廓数量 = 0;
    std::int64_t 自我场景边界深度稳定率 = 0;
    std::int64_t 自我场景候选存在验证通过率 = 0;
    std::int64_t 自我场景未解释区域比例 = 0;
    std::int64_t 自我场景补观察缺口状态 = 0;
    std::int64_t 自我场景补观察缺失原因 = 0;
    std::int64_t 自我场景待补观察区域数量 = 0;
    std::int64_t 自我场景补全候选数量 = 0;
    std::int64_t 自我场景低置信轮廓数量 = 0;
    std::int64_t 自我场景条件不足候选数量 = 0;
    std::int64_t 自我场景待验证候选数量 = 0;
    std::int64_t 自我场景部分确认候选数量 = 0;
    std::int64_t 自我场景补观察需求建议 = 0;
    std::int64_t 自我场景诊断区域数量 = 0;
    std::int64_t 自我场景诊断区域集合状态 = 0;
    std::int64_t 自我场景诊断区域掩码状态 = 0;
    std::vector<结构_控制面板诊断区域> 自我场景诊断区域列表{};
    std::vector<结构_控制面板自我场景存在复现项> 自我场景存在复现项列表{};
    std::vector<结构_控制面板自我场景体素复现项> 自我场景体素复现项列表{};
    std::int64_t 自我场景真实复现存在数量 = 0;
    std::int64_t 自我场景真实几何存在数量 = 0;
    std::int64_t 自我场景真实可绘制存在数量 = 0;
    std::int64_t 自我场景真实颜色状态存在数量 = 0;
    std::int64_t 自我场景真实彩图材料可回查存在数量 = 0;
    bool 自我场景体素快照有效 = false;
    std::int64_t 自我场景体素版本 = 0;
    std::int64_t 自我场景体素坐标系版本 = 0;
    std::int64_t 自我场景体素生成时间 = 0;
    std::int64_t 自我场景体素枚举存在数量 = 0;
    std::int64_t 自我场景体素有坐标存在数量 = 0;
    std::int64_t 自我场景体素有体素存在数量 = 0;
    std::int64_t 自我场景体素入选存在数量 = 0;
    std::int64_t 自我场景体素缺口数量 = 0;
    std::int64_t 自我场景体素缺体素特征类型数量 = 0;
    std::int64_t 自我场景体素缺坐标存在数量 = 0;
    std::int64_t 自我场景体素缺体素存在数量 = 0;
    std::int64_t 自我场景体素缺坐标系版本数量 = 0;
    std::int64_t 自我场景空间候选数量 = 0;
    std::int64_t 自我场景空间候选有效点数量 = 0;
    std::int64_t 自我场景主空间候选编号 = -1;
    std::int64_t 自我场景主空间候选像素数量 = 0;
    std::int64_t 自我场景中心空间坐标X = 0;
    std::int64_t 自我场景中心空间坐标Y = 0;
    std::int64_t 自我场景中心空间坐标Z = 0;
    std::int64_t 自我场景范围坐标AABB最小X = 0;
    std::int64_t 自我场景范围坐标AABB最大X = 0;
    std::int64_t 自我场景范围坐标AABB最小Y = 0;
    std::int64_t 自我场景范围坐标AABB最大Y = 0;
    std::int64_t 自我场景范围坐标AABB最小Z = 0;
    std::int64_t 自我场景范围坐标AABB最大Z = 0;
    std::int64_t 自我场景空间连续性评分 = 0;
    std::int64_t 自我场景范围稳定性评分 = 0;
    std::int64_t 自我场景观察存在假设 = 0;
    std::int64_t 自我场景来源空间候选编号 = -1;
    std::int64_t 自我场景存在假设验证状态 = 0;
    std::int64_t 自我场景假设距离 = 0;
    std::int64_t 自我场景假设尺寸X = 0;
    std::int64_t 自我场景假设尺寸Y = 0;
    std::int64_t 自我场景假设尺寸Z = 0;
    std::int64_t 自我场景假设有效点比例 = 0;
    std::int64_t 自我场景像素归属验证状态 = 0;
    std::int64_t 自我场景全帧像素归属账状态 = 0;
    std::int64_t 自我场景已归属像素数 = 0;
    std::int64_t 自我场景未解释像素数 = 0;
    std::int64_t 自我场景归属冲突像素数 = 0;
    std::int64_t 自我场景像素归属率 = 0;
    std::int64_t 自我场景深度一致率 = 0;
    std::int64_t 自我场景范围内点比例 = 0;
    std::int64_t 自我场景投影覆盖率 = 0;
    std::int64_t 自我场景轮廓吻合率 = 0;
    std::int64_t 自我场景已验证观察存在 = 0;
    std::int64_t 自我场景观察存在确认状态 = 0;
    std::int64_t 自我场景已验证观察存在数量 = 0;
    std::int64_t 自我场景帧解释状态 = 0;
    std::int64_t 自我场景基础观察事实可用状态 = 0;
    std::int64_t 自我场景基础风险判断状态 = 0;
    std::int64_t 自我场景风险状态明确状态 = 0;
    std::int64_t 自我场景基础风险值计算状态 = 0;
    std::int64_t 自我场景当前场景评估安全值候选 = 0;
    std::int64_t 自我场景当前场景安全度候选 = 0;
    std::int64_t 自我场景当前场景安全判定状态 = 0;
    std::int64_t 自我场景安全评估证据不足原因 = 0;
    std::int64_t 自我场景风险安全_场景影响部分候选 = 0;
    std::int64_t 自我场景风险安全_场景影响部分可结算状态 = 0;
    std::int64_t 自我场景风险安全_场景影响部分不可结算原因 = 0;
    std::int64_t 自我场景风险安全_场景影响部分已入账状态 = 0;
    std::int64_t 自我场景风险安全层候选 = 0;
    std::int64_t 自我场景风险安全层投影候选 = 0;
    std::int64_t 自我场景风险安全层缺失因素数量 = 0;
    std::int64_t 自我场景风险安全层明确状态 = 0;
    std::int64_t 自我场景风险安全层可聚合状态 = 0;
    std::int64_t 自我场景风险安全层已入账状态 = 0;
    std::int64_t 自我场景风险因素未搜索掩码 = 0;
    std::int64_t 自我场景风险因素搜索缺口掩码 = 0;
    std::int64_t 自我场景风险因素负向证据掩码 = 0;
    std::int64_t 自我场景风险因素默认满足掩码 = 0;
    std::int64_t 自我场景风险因素已入账掩码 = 0;
    std::int64_t 自我场景风险因素证据搜索未完成数量 = 0;
    std::int64_t 自我场景风险因素默认满足数量 = 0;

    std::string 自我线程当前阶段{};
    std::string 自我线程当前最终去向{};

    bool 控制面板摘要线程已启动 = false;
    bool 控制面板摘要线程健康运行 = false;
    std::uint8_t 控制面板摘要线程生命周期 = 0;
    std::uint64_t 控制面板摘要快照序号 = 0;
    std::uint64_t 控制面板摘要_运行证据方法数 = 0;
    std::uint64_t 控制面板摘要_失败任务数 = 0;
    std::uint64_t 控制面板摘要_等待任务数 = 0;
    std::string 控制面板摘要_最近方法主键{};
    std::uint64_t 控制面板摘要_最近方法时间 = 0;
    std::uint64_t 控制面板摘要_最近任务时间 = 0;
    std::int64_t 控制面板自检生命周期值 = 0;
    std::int64_t 控制面板自检最近模式值 = 0;
    std::int64_t 控制面板自检健康状态值 = 0;
    bool 控制面板自检健康运行 = false;
    std::uint64_t 控制面板自检Tick计数 = 0;
    std::uint64_t 控制面板自检累计发现问题数 = 0;
    std::uint64_t 控制面板自检累计提交需求数 = 0;
    std::uint64_t 控制面板自检累计提交报告数 = 0;
    std::uint64_t 控制面板自检累计需求化失败数 = 0;
    std::uint64_t 控制面板自检最近检查时间 = 0;
    std::uint64_t 控制面板自检最近提交时间 = 0;
    std::uint64_t 自检报告待处理数 = 0;
    std::uint64_t 自检报告待休眠修复数 = 0;
    std::uint64_t 自检报告待映射确认数 = 0;
    std::uint64_t 自检报告仅报告保留数 = 0;
    std::uint64_t 自检报告休眠期评估数 = 0;
    std::uint64_t 自检报告越界需求化拒绝数 = 0;
    std::uint64_t 自检报告最近休眠期门控数 = 0;
    std::string 自检报告最近处置{};
    std::string 自检报告修复门控摘要{};

    bool 任务管理工作线程已启动 = false;
    bool 任务管理工作线程正在执行 = false;
    bool 任务管理工作线程已收到请求 = false;
    std::uint64_t 任务管理工作线程累计推进次数 = 0;
    std::uint64_t 任务管理工作线程当前排队数 = 0;
    std::string 任务管理工作线程最近缺口归类{};
    std::uint64_t 任务管理工作线程最近特征变化数 = 0;

    bool 任务管理界面线程已启动 = false;
    bool 任务管理界面线程正在处理 = false;
    bool 任务管理界面线程故障 = false;
    std::uint64_t 任务管理界面线程累计接收请求数 = 0;
    std::uint64_t 任务管理界面线程累计绑定任务虚拟存在数 = 0;
    std::uint64_t 任务管理界面线程累计派发工作项数 = 0;
    std::uint64_t 任务管理界面线程累计收到工作结果数 = 0;
    std::uint64_t 任务管理界面线程累计上行消息数 = 0;
    std::uint64_t 任务管理界面线程累计调度事件数 = 0;
    std::uint64_t 任务管理界面线程累计消费工作项数 = 0;
    std::uint64_t 任务管理界面线程累计巡检任务数 = 0;
    std::uint64_t 任务管理界面线程累计巡检入队任务数 = 0;
    std::uint64_t 任务管理界面线程累计后台worker取项数 = 0;
    std::uint64_t 任务管理界面线程累计后台worker派发数 = 0;
    std::uint64_t 任务管理界面线程当前请求队列长度 = 0;
    std::uint64_t 任务管理界面线程当前等待工作项数 = 0;
    std::uint64_t 任务管理界面线程当前待筹办工作项数 = 0;
    std::uint64_t 任务管理界面线程当前待执行工作项数 = 0;
    std::uint64_t 任务管理界面线程当前完成工作项数 = 0;
    std::uint64_t 任务管理界面线程当前死信工作项数 = 0;
    std::uint64_t 任务管理界面线程当前在途工作项数 = 0;
    std::uint64_t 任务管理界面线程当前任务状态快照数 = 0;
    std::uint64_t 任务管理界面线程当前等待任务索引数 = 0;
    std::uint64_t 任务管理界面线程当前挂起任务索引数 = 0;
    std::uint64_t 任务管理界面线程当前上行队列长度 = 0;
    bool 任务管理界面线程后台worker池运行 = false;
    std::uint64_t 任务管理界面线程后台worker线程数 = 0;
    std::uint64_t 任务管理工作线程池默认大小 = 20;
    std::uint64_t 任务管理工作线程池最小大小 = 1;
    std::uint64_t 任务管理工作线程池最大大小 = 128;
    std::uint64_t 任务管理工作线程池当前有效大小 = 20;
    std::uint64_t 任务管理工作线程池待保存大小 = 20;
    std::uint64_t 任务管理工作线程池参数版本 = 1;
    std::uint64_t 任务管理界面线程最近请求ID = 0;
    std::uint64_t 任务管理界面线程最近工作项ID = 0;
    std::uint64_t 任务管理界面线程最近调度事件ID = 0;
    std::uint64_t 任务管理界面线程最近后台worker序号 = 0;
    std::uint64_t 任务管理界面线程最近后台worker工作项ID = 0;
    std::string 任务管理界面线程最近工作项类型{};
    std::string 任务管理界面线程最近调度事件类型{};
    std::string 任务管理界面线程最近任务主键{};
    std::uint64_t 任务管理界面线程最近任务状态版本 = 0;
    std::string 任务管理界面线程最近任务状态{};
    std::string 任务管理界面线程最近任务虚拟存在主键{};
    std::string 任务管理界面线程最近需求主键{};
    std::string 任务管理界面线程最近调度动作{};
    std::string 任务管理界面线程最近后台worker任务主键{};
    std::string 任务管理界面线程最近后台worker状态{};
    std::uint8_t 任务管理工作线程池最近参数保存状态值 = 0;
    std::uint8_t 任务管理工作线程池最近参数错误状态值 = 0;
    std::uint8_t 任务管理工作线程池最近参数应用状态值 = 0;
    std::string 任务管理界面线程待筹办队列头部任务主键{};
    std::string 任务管理界面线程待执行队列头部任务主键{};
    std::int64_t 任务管理界面线程待筹办队列头部优先级 = 0;
    std::int64_t 任务管理界面线程待执行队列头部优先级 = 0;
    std::string 任务管理界面线程待筹办队列头部排序兜底键{};
    std::string 任务管理界面线程待执行队列头部排序兜底键{};
    std::uint64_t 任务管理界面线程最近巡检任务数 = 0;
    std::uint64_t 任务管理界面线程最近巡检入队任务数 = 0;
    std::uint64_t 任务管理界面线程最近巡检耗时微秒 = 0;
    std::uint64_t 任务管理界面线程最近巡检可推进任务数 = 0;
    std::uint64_t 任务管理界面线程最近巡检等待任务数 = 0;
    std::uint64_t 任务管理界面线程最近巡检挂起任务数 = 0;
    std::uint64_t 任务管理界面线程最近巡检终结任务数 = 0;
    std::uint64_t 任务管理界面线程最近巡检跳过已有工作项数 = 0;
    std::uint64_t 任务管理界面线程最近巡检跳过缺结构数 = 0;
    std::uint64_t 任务管理界面线程最近巡检跳过不可推进数 = 0;
    std::uint64_t 任务管理界面线程最近巡检跳过入队预算数 = 0;
    std::size_t 缺口需求总数 = 0;
    std::size_t 缺口需求活跃数 = 0;
    std::size_t 缺口需求可调度数 = 0;
    std::size_t 缺口恢复请求数 = 0;
    std::size_t 缺口因兜底切换进入就绪数 = 0;
    std::size_t 缺口因兜底切换进入采样数 = 0;
    bool 应触发缺口承接 = false;
    bool 缺口应申请重试恢复 = false;
    bool 缺口应申请收束恢复 = false;
    std::string 缺口最近反馈摘要{};
    结构_控制面板树节点 线程状态树根{};
    结构_控制面板树节点 世界树根{};
    结构_控制面板树节点 因果信息根{};
    结构_控制面板树节点 需求树根{};
    结构_控制面板树节点 需求列表树根{};
    结构_控制面板树节点 任务树根{};
    结构_控制面板树节点 方法树根{};
};

enum class 枚举_控制面板命令 {
    无 = 0,
    输出摘要 = 1,
    输出任务管理摘要 = 2,
    输出缺口摘要 = 3,
    生成HTML = 4,
    打开窗口 = 5,
    输出需求树生长摘要 = 6,
    输出完成任务变化跟踪 = 7,
    打开相机窗口 = 8,
    打开自我场景窗口 = 9,
};

结构_控制面板快照 读取控制面板快照(
    std::size_t 树深度上限 = 10,
    std::size_t 树广度上限 = 24);

结构_控制面板快照 读取控制面板轻量摘要快照();

std::string 渲染控制面板摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 树层数上限 = 4,
    std::size_t 列表预览上限 = 8);

std::string 渲染任务管理摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限 = 12);

std::string 渲染缺口摘要(
    const 结构_控制面板快照& 快照,
    std::size_t 列表预览上限 = 12);

std::string 渲染需求树生长摘要(
    const 结构_控制面板快照& 快照);

std::string 生成自我场景独立窗口HTML(
    const 结构_控制面板快照& 快照);

// 功能：根据只读控制面板快照生成自我场景复现帧 JSON。
std::string 生成自我场景复现JSON(
    const 结构_控制面板快照& 快照);

// 功能：根据只读控制面板快照生成自我场景复现稳定签名。
std::uint64_t 生成自我场景复现签名(
    const 结构_控制面板快照& 快照) noexcept;

std::string 生成SQL控制面板HTML();

std::string 生成SQL控制面板启动HTML();

std::string 读取控制面板节点子项JSON(
    std::string_view 展开类型,
    std::uintptr_t 节点指针,
    std::size_t 树广度上限 = 16,
    std::uintptr_t 附加参数 = 0);

std::string 读取控制面板节点详情JSON(
    std::string_view 展开类型,
    std::uintptr_t 节点指针,
    std::size_t 树广度上限 = 16);

std::string 读取控制面板页面刷新JSON(
    std::string_view 页面);

std::string 读取SQL控制面板子链JSON(
    std::string_view 区段ID,
    std::string_view 节点键);

bool 保存控制面板HTML(
    const std::filesystem::path& 输出路径);

bool 启动控制面板窗口() noexcept;
bool 启动控制面板相机窗口() noexcept;
bool 启动控制面板自我场景窗口() noexcept;
void 请求关闭控制面板窗口() noexcept;
void 等待控制面板窗口关闭() noexcept;
int 获取控制面板启动诊断码() noexcept;

std::filesystem::path 默认控制面板HTML路径();

枚举_控制面板命令 解析控制面板命令行(int argc, char** argv) noexcept;

} // export
