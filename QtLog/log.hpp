#ifndef _CYS_CALIBCAM_LOG_
#define _CYS_CALIBCAM_LOG_

#pragma execution_character_set("utf-8")

#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>

#if __cplusplus == 202002L
#define CPP20
#endif // __cplusplus == 202002L

#ifndef LOG_INIT
#define LOG_INIT Log::Instance().Init
#endif // !LOG_INIT

#ifndef WRITE_LOG

#ifdef CPP20
#include <source_location>
#define WRITE_LOG(logLevel, logInfo)\
        {const std::source_location location{std::source_location::current()};\
        Log::Instance().writeLog(\
			logLevel,\
			location.file_name(),\
			location.function_name(),\
			location.line(),\
			logInfo);}\

#else
#define WRITE_LOG(logLevel, logInfo)\
        Log::Instance().writeLog(\
			logLevel,\
			__FILE__,\
			__func__,\
			__LINE__,\
			logInfo)\

#endif // !CPP20

#endif // !WRITE_LOG

using std::chrono::system_clock;

static constexpr const char* LOGFILE { "\\CYSCalibCam" };

enum class LOGLEVEL
{
	NONE,
	ERROR,
	WARNING,
	DEBUG,
	INFO,
};

enum class LOGTARGET
{
	NONE             	= 0b00,
	CONSOLE          	= 0b01,
	FILE             	= 0b10,
	CONSOLE_AND_FILE	= 0b11
};

enum class LOGFORMAT
{
	TXT,
	CSV
};

/* 以键值对形式存储日志等级对应的宽字符串 */
static const std::unordered_map<LOGLEVEL, const char*> STR_LOGLEVEL
{
	std::pair<LOGLEVEL, const char*>(LOGLEVEL::ERROR,   "ERROR"),
	std::pair<LOGLEVEL, const char*>(LOGLEVEL::WARNING,	"WARNING"),
	std::pair<LOGLEVEL, const char*>(LOGLEVEL::DEBUG,   "DEBUG"),
	std::pair<LOGLEVEL, const char*>(LOGLEVEL::INFO,    "INFO")
};

/* 日志类 */
class Log
{
public:
	~Log() = default;
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
	Log(Log&&) = delete;
	Log& operator=(Log&&) = delete;

	static void Init(LOGLEVEL _LogLevel, LOGTARGET _LogTarget, LOGFORMAT _LogFormat);
	static Log& Instance() noexcept;
	static QStringList getLogFiles();
	static bool getLogFromFile(QStringList&, const QString& date);
	static bool outputToTarget();
	static bool writeLog
	(
		const LOGLEVEL _LogLevel,    // Log等级
		const QString& _FileName,    // 函数所在文件名
		const QString& _Function,    // 函数名
		const int _LineNumber,       // 行号
		const QString& _Text         // 日志正文
	);

	/* 获取Log等级 */
	static LOGLEVEL	getLogLevel() noexcept { return m_logLevel; }
	/* 设置Log等级 */
	static void setLogLevel(LOGLEVEL _LogLevel) noexcept { m_logLevel = _LogLevel; }
	/* 获取Log输出位置 */
	static LOGTARGET getLogTarget() noexcept { return m_logTarget; }
	/* 设置Log输出位置 */
	static void setLogTarget(LOGTARGET _LogTarget) noexcept { m_logTarget = _LogTarget; }
	/* 获取Log输出文件的格式 */
	static LOGFORMAT getLogFormat() noexcept { return m_logFormat; }
	/* 设置Log输出文件的格式 */
	static void setLogFormat(LOGFORMAT _LogFormat) noexcept { m_logFormat = _LogFormat; }

private:
	Log(LOGLEVEL _LogLevel = LOGLEVEL::INFO,
		LOGTARGET _LogTarget = LOGTARGET::FILE,
		LOGFORMAT _LogFormat = LOGFORMAT::TXT);
	
	/**
	 * @brief 根据日志格式获取日志文件后缀
	 * 
	 * @return QString 日志文件后缀字符串
	 */
	static inline QString logSuffix() {
		return m_logFormat == LOGFORMAT::TXT ?
				QString(".txt") : QString(".csv");
	}

	static	std::shared_mutex		m_LogMutex;		// 读写互斥
	static	QString					m_LogBuffer;	// 存储Log的buffer
	static	LOGLEVEL				m_logLevel;		// Log等级
	static	LOGTARGET				m_logTarget;	// Log输出位置
	static	LOGFORMAT				m_logFormat;	// Log输出文件的格式
};

/**
 * @brief 获取当前本地时间
 *
 * @param _TimeBuffer 输出参数 当前本地时间
 */
inline void getCurrentLocalTime(char* _TimeBuffer)
{
	time_t now = system_clock::to_time_t(system_clock::now());
	struct tm* tmNow(localtime(&now));
	snprintf(_TimeBuffer, 20, "%d-%02d-%02d %02d:%02d:%02d",
		(int)tmNow->tm_year + 1900, (int)tmNow->tm_mon + 1, (int)tmNow->tm_mday,
		(int)tmNow->tm_hour, (int)tmNow->tm_min, (int)tmNow->tm_sec);
}

/**
 * @brief 检查传入的日期字符串是否符合格式要求，即是否为“年-月-日”的形式
 * 
 * @param dateStr 	待检查的日期字符串
 * @return true 	表示传入的日期字符串符合格式要求
 * @return false 	表示传入的日期字符串不符合格式要求
 */
inline bool isValidDate(const QString& dateStr) {
    QRegularExpression regex("\\d{4}-\\d{2}-\\d{2}");
    QRegularExpressionMatch match = regex.match(dateStr);
    return match.hasMatch();
}

#endif // !_CYS_CALIBCAM_LOG_