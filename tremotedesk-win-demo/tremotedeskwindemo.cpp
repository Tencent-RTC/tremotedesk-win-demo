#include "tremotedeskwindemo.h"
#include <QScrollBar>
#include "libwebsockets/include/libwebsockets.h"
#include "cpp-httplib/httplib.h"
#include <util/url.hpp>
#include "nlohmann/json.hpp"

static std::string tokenUrl = "https://remott-api.rtc.qq.com/v1/token";


std::string getToken(const std::string& requesUrl, const std::string& sdkAppid,
	const std::string& roomId, const std::string userId) {
	std::string tokenUrl = requesUrl;
	http::url url = http::ParseHttpUrl(tokenUrl);
	std::unique_ptr<httplib::SSLClient> cli;
	cli.reset(new httplib::SSLClient(url.host.c_str(), url.port));
	cli->enable_server_certificate_verification(false);
	cli->set_connection_timeout(3);
	nlohmann::json jsonMsg;
	jsonMsg["appkey"] = sdkAppid;
	jsonMsg["roomid"] = roomId.c_str();
	jsonMsg["userid"] = userId.c_str();

	auto res = cli->Post(url.path.c_str(), jsonMsg.dump(), "application/json; charset=utf-8");
	if (res && res->status == 200) {
		nlohmann::json jsonMsg = nlohmann::json::parse(res->body);
		int code = jsonMsg["code"].get<int>();
		if (code == 10000) {
			nlohmann::json data = jsonMsg["data"];
			return data["token"].get<std::string>();
		}
	}
	return "";
}

void logCallback(const char* msg, void* object) {
	tremotedeskwindemo *demo = (tremotedeskwindemo*)object;
	emit demo->sdkLog(msg);
}

tremotedeskwindemo::tremotedeskwindemo(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	connect(ui.login_, SIGNAL(clicked()), this, SLOT(onLoginClick()));
	connect(ui.leave_, SIGNAL(clicked()), this, SLOT(onLeaveClick()));
	connect(this, SIGNAL(sdkLog(QString)), this, SLOT(onSdkLog(QString)));

	RegisterLogFunc(logCallback, this);
	m_mediaEngine = CreateMediaEngine();
	m_mediaEngine->RegisterMediaObserver(this, this);
}

tremotedeskwindemo::~tremotedeskwindemo()
{
	if (m_mediaEngine) {
		DestroyMediaEngine(m_mediaEngine);
		m_mediaEngine = nullptr;
	}
}

void tremotedeskwindemo::writeLog(const char* msg) {
	std::lock_guard<std::mutex> l(m_logMutex);
	if (m_log.size() > 10000) {
		m_log.clear();
	}
	m_log += msg;
	ui.log_->setPlainText(m_log.c_str());
	QScrollBar *vScrollbar = ui.log_->verticalScrollBar();
	vScrollbar->setSliderPosition(ui.log_->verticalScrollBar()->maximum());
}

void tremotedeskwindemo::onHostConnected(const char* userid, void* context) {
	std::string log;
	log = "usrId:";
	log += userid;
	log += " onHostConnected\n";
	emit sdkLog(log.c_str());
	m_mediaEngine->EnableLocalMediaChannel(m_screenStreamId.c_str(), true);

	// 开启音频
	// m_mediaEngine->EnableLocalMediaChannel(m_audioStreamId.c_str(), true);


	// 后续如果想关闭音频
	// m_mediaEngine->EnableLocalMediaChannel(m_audioStreamId.c_str(), false);
}

void tremotedeskwindemo::onJoined(const char* userId, bool local, const char* role, void* context) {
	if (local) {
		m_isjoined = true;
		m_mediaEngine->CreateLocalMediaChannel(m_audioStreamId.c_str(), audio, false, 0);


		RCDiaplayType type[1] = { RCDisplayScreen };
		int numDisplay = 0;
		DisplaySource* source =
			m_mediaEngine->GetLocalDisplays(type, sizeof(type) / sizeof(RCDiaplayType), numDisplay);
		if (numDisplay > 0) {
			m_mediaEngine->CreateLocalMediaChannel(m_screenStreamId.c_str(), screen, 0,
				source[0].id);

			MediaPushConfig config;
			config.width = source[0].width;
			config.height = source[0].height;
			config.fps = 30;
			config.bitrate = 1500;
			if (config.width >= 3000) {
				config.bitrate = 3000;
			}
			else if (config.width >= 2000) {
				config.bitrate = 2000;
			}
			else if (config.width >= 1500) {
				config.bitrate = 2000;
			}

			m_mediaEngine->SetPushParam(m_screenStreamId.c_str(), config);
		}
	}

	std::string log;
	log = "onJoined usrId:";
	log += userId;
	log += "\n";
	emit sdkLog(log.c_str());
}

void tremotedeskwindemo::onLeaved(const char* userId, bool local, const char* role, void* context) {
	if (local) {
		m_isjoined = false;
	}
}

void tremotedeskwindemo::onLoginClick() {
	if (m_isjoined) {
		writeLog("has joined!\n");
		return;
	}

	if (ui.lineEdit_appkey->text().isEmpty()) {
		writeLog("appkey is empty!\n");
		return;
	}
	if(ui.lineEdit_roomId->text().isEmpty()) {
		writeLog("roomId is empty!\n");
		return;
	}
	if (ui.lineEdit_userId->text().isEmpty()) {
		writeLog("userId is empty!\n");
		return;
	}

	m_appKey = ui.lineEdit_appkey->text().toStdString();
	m_roomId = ui.lineEdit_roomId->text().toStdString();
	m_userId = ui.lineEdit_userId->text().toStdString();
	m_screenStreamId = m_userId + std::string("_screen_track");
	m_audioStreamId = m_userId + std::string("_audio_track");
	std::string Url = tokenUrl;
	auto token = getToken(Url, m_appKey, m_roomId, m_userId);
	if (token.empty()) {
		writeLog("token is empty\n");
		return;
	}
	if (m_mediaEngine) {
		m_mediaEngine->Join(m_appKey.c_str(), m_roomId.c_str(), m_userId.c_str(), token.c_str(), "host");
	}
}

void tremotedeskwindemo::onLeaveClick() {
	if (m_mediaEngine) {
		m_mediaEngine->Leave();
	}
}

void tremotedeskwindemo::onSdkLog(QString msg) {
	writeLog(msg.toStdString().c_str());
}
