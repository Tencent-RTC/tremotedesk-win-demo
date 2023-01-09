## Windows
- include : 头文件 IMediaEngine.h
- lib: MediaEngine.lib
- dll: MediaEngine.dll

## 主要接口说明

### 初始化

```cpp
// 1、构造engine对象
IMediaEngine* MediaEngine = CreateMediaEngine();

// 2、注册监听, context可以视情况设置，sdk会在其它接口中回调回来
MediaEngine->RegisterMediaObserver(this, this);

// 3、进房
MediaEngine->Join(m_appKey.c_str(), m_roomId.c_str(), m_userId.c_str(), token.c_str(), "host");

// 4、加入房间成功后，在onJoined创建桌面采集
MediaEngine->CreateLocalMediaChannel(m_screenStreamId.c_str(), screen, 0, source[0].id);

// 5、创建音频
m_mediaEngine->CreateLocalMediaChannel(m_audioStreamId.c_str(), audio, false, 0);

// 6、收到onHostConnected回调后调用 EnableLocalMediaChannel 开始发送数据
m_mediaEngine->EnableLocalMediaChannel(m_screenStreamId.c_str(), true);

// 7、 如果要发送/停止音频
m_mediaEngine->EnableLocalMediaChannel(m_audioStreamId.c_str(), true/false);
```

### win-demo 编译

1. https://download.qt.io/archive/qt/5.13/5.13.0/qt-opensource-windows-x86-5.13.0.exe  安装Qt5.13
2. https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123  Qt VS Tools for Visual Studio 2017
3. vs2017打开 tremotedesk-win-demo.sln编译
