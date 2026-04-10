#include <iostream>
#include "世界树类.h"
#include "自我初始化模块.h"
int main(){
    世界树.初始化();
    世界树.初始化默认世界();
    自我类 本地自我{};
    自我初始化模块::执行自我初始化(本地自我, "probe", nullptr);
    auto 根子 = 世界树.获取子节点(nullptr);
    std::cout << "root_children=" << 根子.size() << "\n";
    for (auto* 节点 : 根子) {
        if (!节点 || !节点->主信息) { std::cout << "- world-root\n"; continue; }
        std::cout << "- type=" << static_cast<int>(节点->主信息->主信息类型) << " name=";
        if (节点->主信息->名称) std::cout << 语素集.获取词(节点->主信息->名称);
        else std::cout << "<null>";
        std::cout << "\n";
    }
}
