export module 任务模块.管理任务;

import 任务管理任务模块;

export namespace 任务管理任务模块 {
using ::任务管理任务模块::读取最近治理结果;
using ::任务管理任务模块::读取治理恢复快照;
using ::任务管理任务模块::生成一步治理任务包;
using ::任务管理任务模块::执行一步治理;
}

export using ::枚举_任务管理触发事件类型;
export using ::枚举_任务管理触发来源;
export using ::枚举_任务管理根层重判结果;
export using ::枚举_任务管理执行前门控结果;
export using ::枚举_治理恢复事件处理方式;
export using ::结构_任务管理显式事件控制;
export using ::结构_治理恢复快照;
export using ::结构_任务管理结果;
