# Third Party Notices

This file lists third-party components known to be bundled with or directly referenced by this repository. The project code is licensed under the root `LICENSE`; third-party components remain under their own licenses.

## Microsoft.Web.WebView2

- Component: Microsoft.Web.WebView2
- Version: 1.0.3856.49
- Local package metadata: `third_party/webview2/1.0.3856.49/pkg/Microsoft.Web.WebView2.nuspec`
- Local license: `third_party/webview2/1.0.3856.49/pkg/LICENSE.txt`
- Local notices: `third_party/webview2/1.0.3856.49/pkg/NOTICE.txt`
- Purpose in this repository: native Windows WebView2 host and control-panel display support.

The WebView2 package includes its own license and third-party notices. Those files must be preserved when redistributing the package or a derived binary distribution.

## Intel RealSense / librealsense2 Runtime

- Component: Intel RealSense SDK / librealsense2 runtime
- Bundled artifact: `realsense2.dll`
- Detected local binary version metadata: not populated in the bundled DLL metadata
- Upstream project: https://github.com/realsenseai/librealsense
- License identified by upstream project: Apache License 2.0
- Purpose in this repository: D455 / RealSense depth-camera runtime access for peripheral observation experiments.

The bundled `realsense2.dll` is a third-party runtime component and is not covered by the project `LICENSE`. If this binary is refreshed or redistributed, preserve the exact upstream license and notice materials from the SDK package used for that binary.

## LZ4 Runtime Dependency

- Component: LZ4 runtime DLL
- Referenced artifact: `lz4.dll`
- Bundled in this snapshot: no
- Purpose in this repository: runtime dependency checked by the RealSense adapter when opening the camera path.

If `lz4.dll` is added to a future distribution, add the exact source, version, license, and notice text for the distributed binary.
