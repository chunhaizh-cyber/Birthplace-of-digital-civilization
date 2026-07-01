module;

// 文件头部规则注释模块：
// 1. 本模块只导出 WebView2 控制面板窗口入口，不承载业务事实。
// 2. 各窗口只能读取显示快照，不能写世界树事实、需求状态、价值结算或动作动态。
// 3. 自我场景窗口显示的是自我所在场景快照；相机窗口显示外设材料预览，二者都不是事实裁决入口。

#include <cstdint>
#include <filesystem>

export module 控制面板WebView2;

export {

bool 启动控制面板WebView2窗口() noexcept;
bool 启动控制面板相机播放窗口() noexcept;
bool 启动控制面板WebView2自我场景窗口() noexcept;
bool 保存控制面板WebView2自我场景窗口截图(
    const std::filesystem::path& 输出路径,
    std::uint32_t 等待渲染毫秒) noexcept;
void 请求关闭控制面板WebView2窗口() noexcept;
void 等待控制面板WebView2窗口关闭() noexcept;
int 获取控制面板WebView2启动诊断码() noexcept;

} // export
