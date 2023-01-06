#ifndef IMEDIAENGINE_H
#define IMEDIAENGINE_H

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <windef.h>
typedef HWND WindowIdType;
#ifdef MEDIA_EXPORTS
#define MEDIA_EXPORT __declspec(dllexport)
#else
#define MEDIA_EXPORT __declspec(dllimport)
#endif
#else
#include <stdint.h>
#define MEDIA_EXPORT __attribute__((visibility("default")))
typedef void* WindowIdType;
#endif


enum ColorSpace { Color_YUVI420 = 0, Color_RGB24, Color_RGB32 };

enum RCConnType {
    RCConnTypeP2P = 0,   // 直连
    RCConnTypeRelay = 1  //  中转
};

enum RCDiaplayType {
    RCDisplayScreen = 0,  // 屏幕
    RCDiaplayWindow = 1   // 应用窗口
};

enum RCDataChannelState {
    RCDataChannleConnecting = 0,
    RCDataChannleOpen = 1,  // The DataChannel is ready to send data.
    RCDataChannleClosing = 2,
    RCDataChannleClosed = 3
};

struct DisplaySource {
    uint32_t id;
    const char* name;
    RCDiaplayType type;
    uint32_t height;
    uint32_t width;
    const char* thumbnail;  // 缩略图，第一期可以先不实现
};

struct MediaPushConfig {
    int width;    //推流宽
    int height;   //推流高
    int bitrate;  //码率 kbps
    int fps;      //推流帧率

    MediaPushConfig() {
        width = 1280;
        height = 720;
        bitrate = 500;
        fps = 15;
    }
};

enum MediaChannelType {
    audio = 1,
    video = 2,
    screen = 4,
};

struct VideoFrame {
    const char* data;
    int width;
    int height;
    ColorSpace color;
    long long timestamps;
};

struct AudioFrame {
    const char* data;
    int len;
    int channel;
    int samplesPerSec;
};

struct MediaPacket {
    const char* data;
    int len;
    bool isKeyFrame;
    long long timestamp;
};

enum InputEventType {
    InputKeyEvent = 0, 
    InputMouseButtonEvent = 1,
    InputMouseScrollEvent = 2,
    InputMouseMoveEvent = 3
};

typedef struct KeyEvent {
    bool     pressed;
    uint8_t  key;
}KeyEvent;

typedef struct MouseButtonEvent {
    bool     pressed;
    uint8_t  button;
}MouseButtonEvent;

typedef struct MouseScrollEvent {
    int offset;
}MouseScrollEvent;

typedef struct MouseMove {
    double x; // 宽比
    double y; // 高比
} MouseMove;

typedef struct InputKeyMouseEvent {
    InputKeyMouseEvent(InputKeyMouseEvent&) = delete;
    InputKeyMouseEvent() {}
    InputEventType type;
    union {
        KeyEvent keyboard;
        MouseButtonEvent mouseButtom;
        MouseScrollEvent mouseScroll;
        MouseMove mouseMove;
    };
} InputKeyMouseEvent;

struct RCHostConnStats {
    uint32_t   packetsSent;            //发送包的数量
    uint32_t   bytesSent;              //发送的字节数
    uint32_t   sourceFramesPerSecond;  //采集FPS
    uint32_t   framesPerSecond;        // 编码FPSs
    uint32_t   roundTripTime;          // rtt  网络延迟 毫秒
    float      bitrate;                //视频发送码率
    uint32_t   frameWidth;             // 视频宽
    uint32_t   frameHeight;            // 视频长
    RCConnType connType;               // 连接类型
};

struct RCPeerConnStats {
    uint32_t   packetsReceived;  // 接受包的数量
    uint32_t   bytesReceived;    //  接受的字节数
    uint32_t   frameWidth;       //  视频宽
    uint32_t   frameHeight;      //  视频长度
    uint32_t   roundTripTime;    // rtt 网络延迟  毫秒
    float      bitrate;          //视频码率
    uint32_t   targetDelay;      // 从接收到包 到渲染出来的延迟
    uint32_t   framesPerSecond;  // 接收帧率
    RCConnType connType;         // 连接类型
};

class IMediaObserver {
 public:
    virtual ~IMediaObserver() {}

    // 本地采集视频回调
    virtual void onCaptureVideoFrame(const char* userId, const VideoFrame& videoFrame,
                                     const char* channelId, void* context) {}
    // 远端视频回调
    virtual void onRemoteVideoFrame(const char* userId, const VideoFrame& videoFrame,
                                    const char* channelId, void* context) {}
    // 远端音频回调
    virtual void onRemoteAudioFrame(const char* userId, const AudioFrame& audioFrame,
                                    const char* channelId, void* context) {}
    // 远端音视频编码数据回调
    virtual void onRemoteEncodedVideo(const char* userId, const MediaPacket& videoPacket,
                                      const char* channelId, void* context) {}
    virtual void onRemoteEncodedAudio(const char* userId, const MediaPacket& audioPacket,
                                      const char* channelId, void* context) {}
    // 远端channel通知
    virtual void onRemoteChannelAdd(const char* userId, const char* channelId,
                                    MediaChannelType type, void* context) {}
    virtual void onRemoteChannelRemove(const char* userId, const char* channelId,
                                       MediaChannelType type, void* context) {}

    // peer 建立了webrtc连接
    virtual void onPeerConnected(const char* userid, void* context) {}
    // host 建立了webrtc连接
    virtual void onHostConnected(const char* userid, void* context) {}

    // 远端自定义数据回调
    virtual void onRemoteMessage(const char* userId, const char* msg, int len, void* context) {}
    virtual void onError(const char* userId, int code, const char* msg, void* context) {}

    // 用户通知
    virtual void onJoined(const char* userId, bool local, const char* role, void* context) {}
    virtual void onLeaved(const char* userId, bool local, const char* role, void* context) {}

    // 远程控制信令
    virtual void onRequestAudio(const char* userId, uint32_t requestId, void* context) {}
    virtual void onAnswerAudio(const char* userId, uint32_t answerId, bool reject, void* context) {}
    virtual void onHangupAudio(const char* userId, void* context) {}
    virtual void onEndControl(const char* userId, void* context) {}

    // 剪切板
    virtual void onClipboard(const char* userId, const char* text, void* context) {}

    // host端统计信息回调
    virtual void onHostConnStats(const RCHostConnStats& stats, void* context) {}

    // peer端统计信息回调
    virtual void onPeerConnStats(const RCPeerConnStats& stats, void* context) {}

    // 切屏回调
    virtual void onDeviceChanged(uint32_t deviceIndex, uint32_t width, uint32_t height, uint32_t fps, void* context) {}

    // peer端回调
    virtual void onRemoteDisplays(const DisplaySource* displays, int numDisplays, void* context) {}

    // data channel状态
    virtual void onDataChannelState(const char* userId, RCDataChannelState state, void* context) {}
};

/*！
        @brief 该回调函数用于获取当时日志
        @param msg 日志信息
        @param object 接收回调函数的对象的指针，set时传入，回调时传出
*/
typedef void (*log_callback)(const char* msg, void* object);

class IMediaDeviceManager;  //视频采集设备管理
class IMicManager;          //音频采集设备管理
class IPlayoutManager;      //音频播放设备管理

class IMediaEngine {
 public:
    virtual ~IMediaEngine() {}

    virtual int RegisterMediaObserver(IMediaObserver* mediaObsverver, void* context) = 0;
    virtual int Join(const char* appKey, const char* roomId, const char* userId, const char* token,
                     const char* role) = 0;  //加入房间
    virtual int Leave() = 0;
    // 设置推流参数
    virtual int SetPushParam(const char* channelId, const MediaPushConfig& config) = 0;
    // channelId===> userId+MediaChannelType 比如 xxx_screen
    //创建 channnel,默认是不发送数据, 调用EnableLocalMediaChannel后开始发送
    virtual int CreateLocalMediaChannel(const char* channelId, MediaChannelType type, bool external,
                                        uint32_t deviceIndex) = 0;
    virtual int DestroyLocalMediaChannel(const char* channelId) = 0;

    // 在 onHostConnected 调用,开始发送数据
    virtual void EnableLocalMediaChannel(const char* channelId, bool enable) = 0;
    // peer 端连接host
    virtual bool ConnectToHost(const char* hostUserId) = 0;

    // 外部音视频输入
    virtual void SendExternalVideo(const VideoFrame& videoFrame, const char* channelId) = 0;
    virtual void SendExternalAudio(const AudioFrame& audioFrame, const char* channelId) = 0;

    // 本地channle创建后设置渲染
    virtual void StartLocalPreview(const char* channelId, WindowIdType winId) = 0;
    // onRemoteChannelAdd 后设置远端渲染
    virtual void StartRemotePreview(const char* userId, const char* channelId,
                                    WindowIdType winId) = 0;
    // 发送鼠标键盘事件
    virtual bool SendKeyMouseMessage(const char* userId,
                                     const InputKeyMouseEvent& keyMouseEvent) = 0;
    // 发送自定义事件
    virtual bool SendCustomMessage(const char* userId, const char* data, int len) = 0;
    // 是否处理发送过来的鼠标键盘事件
    virtual void IsHandleMouseKey(bool handle) = 0;

     // 远程控制信令
    virtual bool RequestAudio(const char* userId, uint32_t requestId) = 0;
    virtual bool AnswerAudio(const char* userId, uint32_t answerId, bool reject) = 0;
    virtual bool HangupAudio(const char* userId) = 0;
    virtual bool EndControl(const char* userId) = 0;

    //剪切板
    virtual bool SendClipboardText(const char* userId, const char* text, int len) = 0;

    // datachannel buffered amount; < 0 failed, >= 0 success
    virtual int GetDataChannelBufferedAmount(const char* userId) = 0;

    // 获取远端设备信息， peer 端发起
    virtual void GetRemoteDisplays(const char* userId, const RCDiaplayType* type, int numTypes) = 0;

    // 获取控屏幕的分辨率 displaySourceNum 返回DisplaySource个数
    virtual DisplaySource* GetLocalDisplays(const RCDiaplayType* type, int numTypes, int &displaySourceNum) = 0;

    // 设置是否是测试环境
    virtual void SetEnv(bool testEnv) = 0;

    /////////////////////////////////////////////////////////////////////////////////
    //                             设备管理
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 设备管理
    /// @{
    /**@brief 视频设备管理
     */
    virtual IMediaDeviceManager* GetMediaDeviceManager() = 0;
    /**@brief 播放器设备管理
     */

    // 音频采集设备
    virtual IMicManager* GetMicManager() = 0;
    // 音频播放设备
    virtual IPlayoutManager* GetPlayoutManager() = 0;
};

class IMediaDeviceManager {
 public:
    virtual ~IMediaDeviceManager() {}

    /**@brief 获取摄像头数量
     * @return 返回摄像头数量
     */
    virtual unsigned GetDeviceCount() = 0;

    /**@brief 获取当前使用的摄像头设备Id
     * @devceId 当前摄像头的设备ID
     * @return true：找打当前使用的摄像头； false：没有找到当前使用的摄像头
     */
    // 	virtual bool GetCurDeviceID(char* deviceId) = 0;
    // 	/**@brief 设置当前使用的摄像头
    // 	* @param devideIndex 摄像头的编号（从0开始， @see IVideoDeviceManager::getDevice）
    // 	* @return 是否成功设置了
    // 	*/
    // 	virtual bool SetCurDevice(unsigned deviceIndex) = 0;
    // 	/**@brief 设置当前使用的摄像头
    // 	* @param devideId 摄像头的Id
    // 	* @return 是否成功设置了
    // 	*/
    // 	virtual bool SetCurDeviceID(const char* deviceId) = 0;

    /**@brief 获取摄像头信息
     * @param nIndex 摄像头的编号（从0开始）
     * @param deviceName 摄像头的名字
     * @param deviceID 摄像头的ID
     * @return 是否成功找到摄像头
     */
    virtual bool GetDevice(unsigned nIndex, char* deviceName, char* deviceID) = 0;

    virtual unsigned GetScreenCount() = 0;
    // virtual bool SetCurScreen(unsigned screenId) = 0;
};

class IMicManager {
 public:
    virtual ~IMicManager(){};

    /**@brief 获取当前使用的麦克音量 [0~255]
     * @return 返回当前使用的麦克音量, 如果无法获取，则返回-1
     */
    virtual int GetVolume() = 0;
    /**@brief 设置当前使用的麦克音量
     * @param nVol 音量值 [0~255]
     * @return 是否成功设置了当前使用的麦克音量
     */
    virtual bool SetVolume(unsigned nVol) = 0;

    /**@brief 获取麦克数量
     * @return 返回麦克数量
     */
    virtual unsigned GetDeviceCount() = 0;

    /**@brief 获取当前使用的麦克设备Id
     * @devceId 当前麦克的设备ID
     * @return true：找打当前使用的麦克； false：没有找到当前使用的麦克
     */
    virtual bool GetCurDeviceID(char* deviceId) = 0;
    /**@brief 设置当前使用的麦克
     * @param devideIndex 麦克的编号（从0开始， @see IMicManager::getDevice）
     * @return 是否成功设置了
     */
    virtual bool SetCurDevice(unsigned deviceIndex) = 0;
    /**@brief 设置当前使用的麦克
     * @param devideId 麦克的Id
     * @return 是否成功设置了
     */
    virtual bool SetCurDeviceID(const char* deviceId) = 0;

    /**@brief 获取麦克信息
     * @param nIndex 麦克的编号（从0开始）
     * @param deviceName 麦克的名字
     * @param deviceID 麦克的ID
     * @return 是否成功找到麦克
     */
    virtual bool GetDevice(unsigned nIndex, char* deviceName, char* deviceID) = 0;
};

class IPlayoutManager {
 public:
    virtual ~IPlayoutManager(){};

    /**@brief 获取当前使用的麦克音量 [0~255]
     * @return 返回当前使用的麦克音量, 如果无法获取，则返回-1
     */
    virtual int GetVolume() = 0;
    /**@brief 设置当前使用的播放器音量
     * @param nVol 音量值  [0~255]
     * @return 是否成功设置了当前使用的播放器音量
     */
    virtual bool SetVolume(unsigned nVol) = 0;

    /**@brief 获取播放器数量
     * @return 返回播放器数量
     */
    virtual unsigned GetDeviceCount() = 0;

    /**@brief 获取当前使用的播放器设备Id
     * @devceId 当前播放器的设备ID
     * @return true：找打当前使用的播放器； false：没有找到当前使用的播放器
     */
    virtual bool GetCurDeviceID(char* deviceId) = 0;
    /**@brief 设置当前使用的播放器
     * @param devideIndex 播放器的编号（从0开始， @see IPlayoutManager::getDevice）
     * @return 是否成功设置了
     */
    virtual bool SetCurDevice(unsigned deviceIndex) = 0;
    /**@brief 设置当前使用的播放器
     * @param devideId 播放器的Id
     * @return 是否成功设置了
     */
    virtual bool SetCurDeviceID(const char* deviceId) = 0;

    /**@brief 获取播放器信息
     * @param nIndex 播放器的编号（从0开始）
     * @param deviceName 播放器的名字
     * @param deviceID 播放器的ID
     * @return 是否成功找到播放器
     */
    virtual bool GetDevice(unsigned nIndex, char* deviceName, char* deviceID) = 0;
};

#ifdef __cplusplus
extern "C" {
#endif  //  __cplusplus

/*!
        @brief 该函数是获取Engine实例，多次调用会成功多个，一般使用一个实例即可
        @return 实例指针
*/
MEDIA_EXPORT IMediaEngine* CreateMediaEngine();

/*
        @brief 该函数用来销毁SDK实例，当使用完成通过createMediaEngine获取的SDK实例
        @param IMediaEngine 获取实例指针的地址
*/
MEDIA_EXPORT void DestroyMediaEngine(IMediaEngine* MediaEngine);

/*!
        @brief 该函数用来注册日志回调函数
*/
MEDIA_EXPORT void RegisterLogFunc(log_callback fn, void* object = nullptr);

#ifdef __cplusplus
}
#endif  //  __cplusplus
#endif
