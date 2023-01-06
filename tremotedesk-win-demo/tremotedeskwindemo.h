#pragma once

#include <QtWidgets/QWidget>
#include "ui_tremotedeskwindemo.h"
#include "IMediaEngine.h"
#include <mutex>

class tremotedeskwindemo : public QWidget,
	                       public IMediaObserver
{
    Q_OBJECT

public:
    tremotedeskwindemo(QWidget *parent = nullptr);
    ~tremotedeskwindemo();
	void writeLog(const char* msg);
protected:
	void onHostConnected(const char* userid, void* context) override;
	void onJoined(const char* userId, bool local, const char* role, void* context) override;
	void onLeaved(const char* userId, bool local, const char* role, void* context);
public:
signals:
	void sdkLog(QString);
private slots:
	void onLoginClick();
	void onLeaveClick();
	void onSdkLog(QString);
private:
    Ui::tremotedeskwindemoClass ui;

	IMediaEngine* m_mediaEngine = nullptr;
	std::string m_appKey;
	std::string m_roomId;
	std::string m_userId;
	std::string m_screenStreamId;
	std::string m_audioStreamId;

	bool m_isjoined = false;
	std::string m_log;
	std::mutex  m_logMutex;
};
