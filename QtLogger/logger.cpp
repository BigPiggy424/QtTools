#include <QDir>
#include <QFile>
#include <QDebug>

#include "logger.hpp"

#ifdef _WIN32

#include <Windows.h>
inline DWORD gettid() { return GetCurrentThreadId(); }

#elif __linux__
#include <unistd.h>

#else
#error Unknown compiler

#endif // _WIN32 __linux__

std::mutex	Logger::m_Mutex		{ std::mutex() };
std::mutex	Logger::m_FileMutex	{ std::mutex() };
LOGFORMAT	Logger::m_LogFormat	{ LOGFORMAT::TXT };
QString		Logger::m_LogFile	{ LOG_FILE };

Logger::Logger(LOGLEVEL _LogLevel, LOGTARGET _LogTarget) : 
	m_LogLevel(_LogLevel), m_LogTarget(_LogTarget) {}

/**
 * @brief 设置Log输出文件路径
 * 
 * @note 该函数在程序运行期间只运行一次，故不用考虑线程安全的问题
 */
void Logger::setLogFileWithDate()
{
	static bool isSet = false;
	if (isSet)
		return;
	char timeBuffer[TIME_BUFFER_SIZE];
	getCurrentLocalTime(timeBuffer);
	char dateBuffer[DATE_SIZE + 1];
	dateBuffer[0] = '/';
	strncpy(dateBuffer + 1, timeBuffer, DATE_SIZE);
	dateBuffer[DATE_SIZE] = '\0';
	m_LogFile += dateBuffer;
	m_LogFile += logSuffix();
	isSet = true;
}

/**
 * @brief 根据日志格式获取日志文件后缀
 * 
 * @return const char* 日志文件后缀字符串
 */
const char* Logger::logSuffix()
{
	switch (m_LogFormat)
	{
	case LOGFORMAT::TXT:
		return ".txt";
	case LOGFORMAT::CSV:
		return ".csv";
	default:
		return "";
	}
}

/**
 * @brief 获取程序目录下所有日志文件
 * 
 * @return QStringList 包含所有日志文件路径
 */
QStringList Logger::getLogFiles()
{
    QDir dir(QDir::currentPath());
    QStringList filters;
    filters << "*.txt" << "*.csv";
    dir.setNameFilters(filters);
    return dir.entryList();
}

/**
 * @brief 从文件中读取日志
 * 
 * @param _LogData	用于存储从文件读出的日志
 * @param _Date		日志日期
 * @return true		日志读取成功 
 * @return false	日志读取失败
 */
bool Logger::getLogFromFile(QStringList& _LogData, const QString& _Date)
{
	if (!isValidDate(_Date))
		return false;
	std::unique_lock<std::mutex> lock(m_FileMutex);
    QFile fileInput(QDir::currentPath()
		 + LOG_FILE + _Date + logSuffix());
    if (fileInput.open(QIODevice::ReadOnly)) {
        QTextStream stream(&fileInput);

        // 读取每一行数据
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            _LogData << line;
        }

        fileInput.close();
		lock.unlock();
        return true;
    }
	lock.unlock();
    return false;
}

/**
 * @brief 初始化日志对象
 * 
 * @param _LogLevel		日志等级
 * @param _LogTarget	日志输出目标，命令行或者文件系统。
 */
void Logger::Init(LOGLEVEL _LogLevel, LOGTARGET _LogTarget, LOGFORMAT _LogFormat)
{
	setLogLevel(_LogLevel);
	setLogTarget(_LogTarget);
	setLogFormat(_LogFormat);
	QString path = QDir::currentPath() + LOG_FILE;
	qDebug() << path << endl;
	QDir dir(path);
	if (!dir.exists())
		dir.mkdir(path);
	setLogFileWithDate();
}

/**
 * @brief 记录日志
 * 
 * @param _LogLevel		日志等级
 * @param _FileName		函数所在文件名
 * @param _Function		函数名
 * @param _LineNumber 	行号
 * @param _Format		格式化字符串
 * @param ...			参数列表
 */
void Logger::writeLog
(
	const LOGLEVEL	_LogLevel,
	const char*		_FileName,
	const char*		_Function,
	const int		_LineNumber,
	const char*		_Format,
	...
)
{
	// 按日志等级确定是否需要记录
	int logLevel = (int)_LogLevel;
	if (logLevel > (int)getLogLevel())
		return;

	// 清空之前的日志
	if (m_LogBuffer.size())
		m_LogBuffer.clear();

	// 获取日期和时间
	char timeBuffer[TIME_BUFFER_SIZE];
	getCurrentLocalTime(timeBuffer);

	// 获取日志正文
	char text[LOG_TEXT_SIZE];
	va_list args;
	va_start(args, _Format);
	vsnprintf(text, LOG_TEXT_SIZE, _Format, args);
	va_end(args);

	// 拼接日志
	constexpr int LOG_SIZE{ LOG_INFO_SIZE + LOG_TEXT_SIZE + 1 };
	char log[LOG_SIZE];
	switch (m_LogFormat)
	{
	case LOGFORMAT::TXT:
		// [时间] [Log等级] [进程号] [线程号] [文件名] [函数名] [行号] [内容]
		snprintf(log, LOG_SIZE,
			"[%s] [%s] [PID : %-5d] [TID : %-5d] [%s] [%s] [LineNumber : %-4d] %s\n",
			timeBuffer, LOG_LEVEL_STRING[logLevel], getpid(), gettid(), _FileName, _Function, _LineNumber, text);
		break;
	case LOGFORMAT::CSV:
		{
			QFile file(QDir::currentPath() + getLogFile());
			bool needsHeader = !file.exists();
			if (needsHeader)
				m_LogBuffer += QString("时间,日志等级,进程号,线程号,文件名,函数名,行号,内容\n");
			snprintf(log, LOG_SIZE, "%s,%s,%d,%d,%s,%s,%d,%s\n",
				timeBuffer, LOG_LEVEL_STRING[logLevel],  getpid(), gettid(), _FileName, _Function, _LineNumber, text);
		}
		break;
	default:
		break;
	}
	m_LogBuffer += log;

	outputToTarget();
}

/**
 * @brief 输出日志
 */
void Logger::outputToTarget()
{
	int target = (int)getLogTarget();
	if (target & (int)LOGTARGET::FILE)
	{
		std::scoped_lock<std::mutex> lock(m_FileMutex);
		QFile fileOutput(QDir::currentPath() + getLogFile());
		if (fileOutput.open(QIODevice::ReadWrite | QIODevice::Append))
		{
			fileOutput.write(m_LogBuffer.toUtf8());
			fileOutput.close();
		}
	}
	if (target & (int)LOGTARGET::CONSOLE)
		qDebug() << m_LogBuffer << endl;
}