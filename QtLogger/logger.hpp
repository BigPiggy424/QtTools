#ifndef _QT_LOGGER_HPP_
#define _QT_LOGGER_HPP_

#pragma execution_character_set("utf-8")

#include <chrono>
#include <mutex>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>

#ifdef _WIN32
#if __cplusplus == 202002L
	#define CPP20
#endif // __cplusplus == 202002L
#endif // _WIN32

#ifndef LOG_INIT
#define LOG_INIT Logger::Instance().Init
#endif // !LOG_INIT

#ifndef LOG

#define LOG_QSTR(qstring) qstring.toUtf8().constData()

#ifdef CPP20
#include <source_location>
#define LOG(logLevel, text)\
    {const std::source_location location{std::source_location::current()};\
    Logger::Instance().writeLog(logLevel, location.file_name(), location.function_name(), location.line(), text);}
#define LOGF(logLevel, format, ...)\
	{const std::source_location location{std::source_location::current()};\
	Logger::Instance().writeLog(logLevel, location.file_name(), location.function_name(), location.line(), format, __VA_ARGS__);}

#else
#define LOG(logLevel, text)\
    Logger::Instance().writeLog(logLevel, __FILE__, __func__, __LINE__, text)
#define LOGF(logLevel, format, ...)\
	Logger::Instance().writeLog(logLevel, __FILE__, __func__, __LINE__, format, __VA_ARGS__)

#endif // !CPP20

#endif // !LOG

using std::chrono::system_clock;

static constexpr const char* LOG_FILE{ "/Log" };	// Log文件名
static constexpr int LOG_INFO_SIZE{ 128 };			// Log信息长度
static constexpr int LOG_TEXT_SIZE{ 256 };			// Log文本长度
static constexpr int DATE_SIZE{ 11 };  				// 日期长度
static constexpr int TIME_SIZE{ 9 };   				// 时间长度
static constexpr int TIME_BUFFER_SIZE{ DATE_SIZE + TIME_SIZE };

enum class LOGLEVEL
{
	NONE	= 0b0000,
	ERROR	= 0b0001,
	WARNING	= 0b0010,
	INFO	= 0b0100,
	DEBUG	= 0b1000,
	ALL		= 0b1111
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

/**
 * @brief 获取日志等级字符串
 */
static constexpr const char* LOG_LEVEL_STRING(LOGLEVEL _LogLevel)
{
	switch (_LogLevel)
	{
	case LOGLEVEL::DEBUG:
		return "DEBUG";
	case LOGLEVEL::INFO:
		return "INFO";
	case LOGLEVEL::WARNING:
		return "WARNING";
	case LOGLEVEL::ERROR:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}

/* 日志类 */
class Logger
{
public:
	~Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(Logger&&) = delete;

	static Logger& Instance() noexcept
	{
		thread_local Logger instance;
		return instance;
	}
	static QStringList getLogFiles();
	static bool getLogFromFile(QStringList& _LogData, const QString& _Date);

	void Init
	(
		LOGLEVEL	_LogLevel,
		LOGTARGET	_LogTarget,
		LOGFORMAT	_LogFormat = m_LogFormat
	);
	void writeLog
	(
		const LOGLEVEL	_LogLevel,		// Log等级
		const char*		_FileName,		// 函数所在文件名
		const char*		_Function,		// 函数名
		const int		_LineNumber,	// 行号
		const char*		_Format,		// 格式化字符串
		...								// 可变参数
	);
	void outputToTarget();

	/**
	 * @brief 获取Log等级
	 * 
	 * @return LOGLEVEL 日志等级
	 */
	LOGLEVEL getLogLevel() noexcept { return m_LogLevel; }
	
	/**
	 * @brief 设置Log等级
	 * 
	 * @param _LogLevel 日志等级
	 */
	void setLogLevel(LOGLEVEL _LogLevel) noexcept { m_LogLevel = _LogLevel; }
	
	/**
	 * @brief 获取Log输出位置
	 * 
	 * @return LOGTARGET Log输出位置
	 */
	LOGTARGET getLogTarget() noexcept { return m_LogTarget; }
	
	/**
	 * @brief 设置Log输出位置
	 * 
	 * @param _LogTarget Log输出位置
	 */
	void setLogTarget(LOGTARGET _LogTarget) noexcept { m_LogTarget = _LogTarget; }

	/**
	 * @brief 获取Log输出文件的格式
	 * 
	 * @return LOGFORMAT Log输出文件的格式
	 */
	static LOGFORMAT getLogFormat() noexcept
	{
		std::scoped_lock<std::mutex> lock(m_Mutex);
		return m_LogFormat;
	}

	/**
	 * @brief 设置Log输出文件的格式
	 * 
	 * @param _LogFormat Log输出文件的格式
	 */
	static void setLogFormat(LOGFORMAT _LogFormat) noexcept
	{
		std::scoped_lock<std::mutex> lock(m_Mutex);
		m_LogFormat = _LogFormat;
	}

	/**
	 * @brief 获取日志文件存放路径
	 * 
	 * @return const QString& 
    LOG(LOGLEVEL::INFO, "选择图像: %s 用于标定", LOG_QSTR(imagePath));
	 */
	static const QString& getLogFile() { return m_LogFile; }

private:
	Logger
	(
		LOGLEVEL _LogLevel		= LOGLEVEL::DEBUG,
		LOGTARGET _LogTarget	= LOGTARGET::CONSOLE_AND_FILE
	);

	static void setLogFileWithDate();
	static const char* logSuffix();

	static std::mutex	m_Mutex;				// 互斥
	static std::mutex	m_FileMutex;			// 文件读写互斥
	static LOGFORMAT	m_LogFormat;			// Log输出文件的格式
	static QString		m_LogFile;				// 日志文件名	

	QString				m_LogBuffer;			// 存储Log的buffer
	LOGLEVEL			m_LogLevel;				// Log等级
	LOGTARGET			m_LogTarget;			// Log输出位置
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
	snprintf(_TimeBuffer, TIME_BUFFER_SIZE, "%d-%02d-%02d %02d:%02d:%02d",
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

#endif //  _QT_LOGGER_HPP_