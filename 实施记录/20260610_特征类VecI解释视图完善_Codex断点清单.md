# 20260610_特征类VecI解释视图完善_Codex断点清单

## 当前切片

```text
计划：计划/20260610_特征类VecI解释视图完善计划_v0.1.md
切片：P5g 二次特征写入适配
状态：P0 阻塞已解除；P1/P2/P3/P4 已闭合并提交；P5a-P5d 已新增轮廓比较结果结构、坐标 L1 误差 helper、通用坐标链比较和平面 / 空间显式比较入口并提交；P5e/P5f 已实施并通过 `git diff --check` 与 Debug x64 构建；当前进入 P5g 二次特征写入适配。
```

## 依据文件

```text
AGENTS.md
规范/规范目录.md
计划/计划索引.md
计划/20260610_特征类VecI解释视图完善计划_v0.1.md
规范/特征类规范20260429.md
规范/观察像素簇与存在候选分层规范20260527.md
规范/相机外设综合工作流程规范20260607.md
规范/识别本能方法唯一性收敛规范20260607.md
```

## 已运行命令

```powershell
rg -n "平面轮廓|空间极值轮廓|VecIU64|I64 到 VecIU64|直接存入|编码规则|字典序|二次特征" 规范/特征类规范20260429.md 规范/观察像素簇与存在候选分层规范20260527.md 规范/相机外设综合工作流程规范20260607.md 规范/识别本能方法唯一性收敛规范20260607.md
rg -n "平面轮廓|空间极值轮廓|轮廓编码|边界点序列|写入特征_VecU|获取或创建句柄|VecIU64\{|边界点" -g "!x64/**" -g "!Debug/**" -g "!.git/**" -g "!日志/**" -g "!logs/**"
rg -n "非负|直接存入|直接存|坐标非负|可以直接存入|一律 ZigZag|等待确认|暂停等待" 规范 计划 实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
rg -n "编码I64为U64_ZigZag|解码U64为I64_ZigZag|编码VecI64为VecIU64_ZigZag|解码VecIU64为VecI64_ZigZag" 基础数据类型.h 基础数据类型.ixx
git diff --check -- 基础数据类型.h 基础数据类型.ixx 实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
rg -n "VecI64|std::variant|枚举_特征值编码类型|编码类型" 特征值主信息类.h
rg -n "获取或创建VecI64|写入特征_VecI64|读取特征VecI64" 特征值类.h 世界树类.h 世界树类.cpp
git diff --check -- 特征值类.h 世界树类.h 世界树类.cpp 计划/计划索引.md 计划/20260610_特征类VecI解释视图完善计划_v0.1.md 实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
rg -n "VecU解释规则_按特征类型|轮廓坐标维度_按特征类型|VecIU64_ZigZag_I64_二维坐标链|VecIU64_ZigZag_I64_三维坐标链" 特征类.h 特征类.cpp
git diff --check -- 特征类.h 特征类.cpp 实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
rg -n "校验坐标链VecI64|校验平面轮廓VecI64|校验空间极值轮廓VecI64|校验坐标类VecI64_按特征类型" 特征类.h 特征类.cpp
git diff --check -- 特征类.h 特征类.cpp 实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
rg -n "枚举_轮廓比较状态|结构_轮廓比较结果|比较坐标轮廓VecI64|比较平面轮廓VecI64|比较空间极值轮廓VecI64|私有_坐标点L1误差|私有_计算点链平均最大误差" 特征类.h 特征类.cpp
git diff --check -- 特征类.h 特征类.cpp 实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
rg -n "比较轮廓VecI64_按特征类型|比较平面轮廓VecI64|比较空间极值轮廓VecI64|维度不支持" 特征类.h 特征类.cpp
rg -n "VecI64|std::variant|枚举_特征值编码类型|编码类型|坐标维度|轮廓比较" 特征值主信息类.h
git diff --check -- 特征类.h 特征类.cpp 实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
rg -n "比较轮廓特征_按特征类型|比较轮廓VecI64_按特征类型|读取特征VecI64\\(" 世界树类.h 世界树类.cpp 特征类.h
rg -n "VecI64|std::variant|枚举_特征值编码类型|编码类型|坐标维度|轮廓比较" 特征值主信息类.h
git diff --check -- 世界树类.h 世界树类.cpp 特征类.h 实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
```

## 关键证据

```text
修订前 规范/观察像素簇与存在候选分层规范20260527.md:
    平面轮廓值类型为 VecIU64 / VECIU64。
    坐标为当前观察平面坐标。
    当时允许全帧图像非负坐标直接存入 VecIU64。
    ROI 局部坐标、相对坐标或可能为负的坐标沿用既有 I64 到 VecIU64 的编码规则。
    VecIU64 本体不直接进入 I64 区间比较，识别 / 扫描 / 跟踪比较需生成二次特征。

规范/相机外设综合工作流程规范20260607.md:
    平面轮廓值类型为 VecIU64 / VECIU64。
    第一个二维坐标为轮廓中心点，后续二维坐标为按轮廓遍历顺序保存的边界点。
    平面轮廓匹配状态必须由量化二次特征推出。

计划/20260610_特征类VecI解释视图完善计划_v0.1.md:
    P3 当前写法为 平面轮廓 = VecIU64_ZigZag_I64_二维坐标链。
```

## 当前判断

```text
历史判断：
    计划 P3 的“一律 ZigZag 解释平面轮廓”与原 050004 的“全帧非负坐标可直接 VecIU64 存储”存在兼容冲突。

已按用户确认口径修订：
    规范/特征类规范20260429.md
    规范/抽象特征规范20260518.md
    规范/观察像素簇与存在候选分层规范20260527.md
    规范/相机外设综合工作流程规范20260607.md

当前口径：
    读取特征值类信息时，根据特征类型决定是否需要转换以及如何转换。
    平面轮廓按 VecIU64_ZigZag_I64_二维坐标链解释。
    空间极值轮廓按 VecIU64_ZigZag_I64_三维坐标链解释。
    类型信息不进入 `特征值主信息类`。
    `特征值主信息类::比较` 只用于去重、排序、快速查找和绝对相同；轮廓相似、扫描变化、跟踪连续性、识别同一性由特征类型专用比较函数生成二次特征。

P0 阻塞已解除，可进入 P1 编码工具和读取包装实现。
```

## P1 预测结果

```text
新增函数只落在：
    基础数据类型.h
    基础数据类型.ixx

预期：
    0 -> 0
    -1 -> 1
    1 -> 2
    -2 -> 3
    2 -> 4
    I64_MIN / I64_MAX 可往返

构建期 static_assert 应覆盖上述标量映射和极值往返。

不应发生：
    修改 `特征值主信息类` 字段；
    新增 `特征值主信息类::VecI64`；
    新增编码类型字段；
    拆轮廓子链；
    让 VecI64 变成新的底层证据池。
```

## P1 已改文件

```text
基础数据类型.h
基础数据类型.ixx
计划/计划索引.md
计划/20260610_特征类VecI解释视图完善计划_v0.1.md
实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
```

## P1 验证结果

```text
rg 工具函数落点：
    基础数据类型.h / 基础数据类型.ixx 均命中 ZigZag 标量函数、Vec 包装函数和 static_assert。

禁止字段扫描：
    `rg -n "VecI64|std::variant|枚举_特征值编码类型|编码类型" 特征值主信息类.h`
    退出码 1，无命中，表示 `特征值主信息类` 未新增 VecI64 / variant / 编码类型字段。

git diff --check：
    退出码 0。

Debug x64 构建：
    命令：msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
    退出码：0。
    构建日志：日志/鱼巢_build_20260610_VecI_P1.log。
```

## P2 预测结果

```text
新增入口：
    特征值类::获取或创建VecI64特征值节点
    特征值类::获取或创建VecI64句柄
    世界树类::写入特征_VecI64(特征节点类*, ...)
    世界树类::写入特征_VecI64(基础信息节点类*, 特征类型, ...)
    世界树类::读取特征VecI64(特征节点类*, ...)
    世界树类::读取特征VecI64(宿主, 特征类型, ...)

预期：
    写入 VecI64 时先调用 P1 的 VecI64 -> VecIU64 编码，再进入现有 VecIU64 证据池。
    读取 VecI64 时只通过现有 VecU句柄只读指针取得原始 VecIU64，再解码为 VecI64 输出。
    不复制或修改证据池原值。

不应发生：
    修改 `特征值主信息类` 字段；
    新增 VecI64 底层值池；
    新增编码类型字段；
    绕开 `写入特征_VecU` / `读取特征VecU` 的新裸写路径；
    拆轮廓子链。
```

## P2 已改文件

```text
特征值类.h
世界树类.h
世界树类.cpp
实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
```

## P2 验证结果

```text
rg 包装入口落点：
    特征值类.h 命中 `获取或创建VecI64特征值节点`、`获取或创建VecI64句柄`。
    世界树类.h / 世界树类.cpp 命中 `写入特征_VecI64`、`读取特征VecI64`。

禁止字段扫描：
    `rg -n "VecI64|std::variant|枚举_特征值编码类型|编码类型" 特征值主信息类.h`
    退出码 1，无命中，表示 `特征值主信息类` 未新增 VecI64 / variant / 编码类型字段。

git diff --check：
    退出码 0。

Debug x64 构建：
    命令：msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
    退出码：0。
    构建日志：日志/鱼巢_build_20260610_VecI_P2.log。
```

## P3 预测结果

```text
新增入口：
    枚举_VecU解释规则
    结构_VecU特征解释规则
    特征类::VecU解释规则_按特征类型
    特征类::轮廓坐标维度_按特征类型

预期：
    特征类型 == 平面轮廓：
        规则 = VecIU64_ZigZag_I64_二维坐标链
        坐标维度 = 2
        需要ZigZag解码 = true

    特征类型 == 空间极值轮廓：
        规则 = VecIU64_ZigZag_I64_三维坐标链
        坐标维度 = 3
        需要ZigZag解码 = true

    其他特征类型：
        规则 = 未定义
        坐标维度无值

不应发生：
    修改 `特征值主信息类` 字段；
    在值节点写入编码类型或维度字段；
    拆轮廓子链；
    把颜色摘要、掩码等未知类型猜成坐标链。
```

## P3 已改文件

```text
特征类.h
特征类.cpp
实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
```

## P3 验证结果

```text
rg 解释规则落点：
    特征类.h / 特征类.cpp 命中 `VecU解释规则_按特征类型`、`轮廓坐标维度_按特征类型`、二维 / 三维 ZigZag 坐标链枚举值。

禁止字段扫描：
    `rg -n "VecI64|std::variant|枚举_特征值编码类型|编码类型|坐标维度" 特征值主信息类.h`
    退出码 1，无命中，表示 `特征值主信息类` 未新增 VecI64 / variant / 编码类型 / 坐标维度字段。

git diff --check：
    退出码 0。

Debug x64 构建：
    命令：msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
    退出码：0。
    构建日志：日志/鱼巢_build_20260610_VecI_P3.log。
```

## P4 预测结果

```text
新增入口：
    特征类::校验坐标链VecI64
    特征类::校验平面轮廓VecI64
    特征类::校验空间极值轮廓VecI64
    特征类::校验坐标类VecI64_按特征类型

预期：
    平面轮廓：
        长度 >= 2
        长度 % 2 == 0

    空间极值轮廓：
        长度 >= 3
        长度 % 3 == 0

    按特征类型路由：
        平面轮廓 -> 二维校验
        空间极值轮廓 -> 三维校验
        未知特征类型 -> false

不应发生：
    截断尾值；
    补 0；
    静默忽略非法长度；
    修改 `特征值主信息类`；
    拆轮廓子链。
```

## P4 已改文件

```text
特征类.h
特征类.cpp
实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
```

## P4 验证结果

```text
rg 校验函数落点：
    特征类.h / 特征类.cpp 命中 `校验坐标链VecI64`、`校验平面轮廓VecI64`、`校验空间极值轮廓VecI64`、`校验坐标类VecI64_按特征类型`。

禁止字段扫描：
    `rg -n "VecI64|std::variant|枚举_特征值编码类型|编码类型|坐标维度" 特征值主信息类.h`
    退出码 1，无命中，表示 `特征值主信息类` 未新增 VecI64 / variant / 编码类型 / 坐标维度字段。

git diff --check：
    退出码 0。

Debug x64 构建：
    命令：msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
    退出码：0。
    构建日志：日志/鱼巢_build_20260610_VecI_P4.log。
```

## P5a-P5d 预测结果

```text
新增入口：
    枚举_轮廓比较状态
    结构_轮廓比较结果
    特征类::比较坐标轮廓VecI64
    特征类::比较平面轮廓VecI64
    特征类::比较空间极值轮廓VecI64

预期：
    坐标维度不是 2 / 3：
        状态 = 维度不支持

    任一向量格式不满足维度规则：
        状态 = 格式非法

    点数量不同：
        状态 = 点数量不一致
        写出左点数量、右点数量、点数量差异

    同维度、同点数：
        状态 = 可比较
        第一个点计算中心L1误差
        后续点按顺序计算点链平均L1误差和点链最大L1误差
        匹配评分Q10000 完全相同为 10000，误差越大越低

不应发生：
    点重采样；
    旋转、缩放、平移归一；
    掩码重合率计算；
    写二次特征；
    修改 `特征值主信息类::比较`；
    修改输入向量。
```

## P5a-P5d 已改文件

```text
特征类.h
特征类.cpp
实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
```

## P5a-P5d 验证结果

```text
rg 比较入口落点：
    特征类.h / 特征类.cpp 命中 `枚举_轮廓比较状态`、`结构_轮廓比较结果`、`比较坐标轮廓VecI64`、`比较平面轮廓VecI64`、`比较空间极值轮廓VecI64`、`私有_坐标点L1误差`、`私有_计算点链平均最大误差`。

禁止字段扫描：
    `rg -n "VecI64|std::variant|枚举_特征值编码类型|编码类型|坐标维度|轮廓比较" 特征值主信息类.h`
    退出码 1，无命中，表示 `特征值主信息类` 未新增 VecI64 / variant / 编码类型 / 坐标维度 / 轮廓比较字段。

git diff --check：
    退出码 0。

Debug x64 构建：
    命令：msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
    退出码：0。
    构建日志：日志/鱼巢_build_20260610_VecI_P5ad.log。
```

## P5e 预测结果

```text
新增入口：
    特征类::比较轮廓VecI64_按特征类型

预期：
    特征类型 == 平面轮廓：
        调用二维平面轮廓比较。

    特征类型 == 空间极值轮廓：
        调用三维空间极值轮廓比较。

    其他特征类型：
        状态 = 维度不支持。
        不猜测坐标维度。

不应发生：
    向 `特征值主信息类` 或值节点写入编码类型 / 维度字段；
    修改 `特征值主信息类::比较`；
    修改输入 VecI64；
    写二次特征。
```

## P5e 已改文件

```text
特征类.h
特征类.cpp
实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
计划/计划索引.md
```

## P5e 验证结果

```text
rg 路由比较入口落点：
    特征类.h / 特征类.cpp 命中 `比较轮廓VecI64_按特征类型`，并命中平面 / 空间显式比较分派。

禁止字段扫描：
    `rg -n "VecI64|std::variant|枚举_特征值编码类型|编码类型|坐标维度|轮廓比较" 特征值主信息类.h`
    退出码 1，无命中，表示 `特征值主信息类` 未新增 VecI64 / variant / 编码类型 / 坐标维度 / 轮廓比较字段。

git diff --check：
    退出码 0。

Debug x64 构建：
    命令：msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
    退出码：0。
    构建日志：日志/鱼巢_build_20260611_VecI_P5e.log。
```

## P5f 预测结果

```text
新增入口：
    世界树类::比较轮廓特征_按特征类型

预期：
    特征类型无轮廓维度：
        返回 P5e 的维度不支持结果。

    左右任一特征不能通过 `读取特征VecI64` 读取：
        状态 = 格式非法。

    左右特征均可读取：
        调用 `特征类::比较轮廓VecI64_按特征类型` 返回运行期比较结果。

不应发生：
    直接操作 VecU句柄或 VecIU64 原始值；
    读取失败时把缺值当 0 误差；
    写二次特征；
    修改 `特征值主信息类::比较`。
```

## P5f 已改文件

```text
世界树类.h
世界树类.cpp
特征类.h
实施记录/20260610_特征类VecI解释视图完善_Codex断点清单.md
计划/计划索引.md
```

## P5f 验证结果

```text
rg 特征节点比较入口落点：
    世界树类.h / 世界树类.cpp 命中 `比较轮廓特征_按特征类型`，并命中 `读取特征VecI64` 和 `比较轮廓VecI64_按特征类型` 调用。

禁止字段扫描：
    `rg -n "VecI64|std::variant|枚举_特征值编码类型|编码类型|坐标维度|轮廓比较" 特征值主信息类.h`
    退出码 1，无命中，表示 `特征值主信息类` 未新增 VecI64 / variant / 编码类型 / 坐标维度 / 轮廓比较字段。

git diff --check：
    退出码 0。

Debug x64 构建：
    命令：msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
    退出码：0。
    构建日志：日志/鱼巢_build_20260611_VecI_P5f.log。
```

## 已定解决方向

```text
底层值容器仍保持 `特征值主信息类::VecIU64`。
VecI64 只作为读取、创建、校验、比较时的解释视图。
特征类型定义负责声明 VecIU64 的解释规则，不在特征值主信息层新增 VecI64 字段、业务类型字段或编码类型字段。
```

## 下一步

```text
完成 P1：
    已完成差异检查、禁止字段扫描和 Debug x64 构建。

完成 P2：
    已完成差异检查、禁止字段扫描和 Debug x64 构建。

完成 P3：
    已完成差异检查、禁止字段扫描和 Debug x64 构建。

完成 P4：
    已完成差异检查、禁止字段扫描和 Debug x64 构建。

完成 P5a-P5d：
    已完成差异检查、禁止字段扫描和 Debug x64 构建。

P5a-P5d 完成后进入 P5e：
    已完成差异检查、禁止字段扫描和 Debug x64 构建。

P5e 完成后进入 P5f：
    已完成差异检查、禁止字段扫描和 Debug x64 构建。

P5f 完成后进入 P5g：
    新增轮廓比较结果到二次特征材料的写入适配。

后续 P5g 仍不得改 `特征值主信息类`，不得新增 VecI64 底层字段，不得拆轮廓子链。
```

## 禁止宣称项

```text
不得宣称 P5g 已实现。
不得把 VecI 解释视图、校验或比较函数完成宣称为观察链业务通过。
不得宣称识别 / 扫描 / 跟踪比较链已通过。
不得宣称自我苏醒完成或初步成熟完成。
```
