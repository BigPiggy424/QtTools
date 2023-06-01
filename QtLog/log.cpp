#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "log.hpp"

#ifdef _WIN32

#include <Windows.h>
inline DWORD gettid() { return GetCurrentThreadId(); }

#elif __linux__
#include <unistd.h>

#else
#error Unknown compiler

#endif // _WIN32 __linux__

constexpr int DATE_LENGTH = 10;

QString					Log::m_LogBuffer	{ "" };
std::shared_mutex		Log::m_LogMutex		{ std::shared_mutex() };
LOGLEVEL				Log::m_logLevel		{ LOGLEVEL::NONE };
LOGTARGET				Log::m_logTarget	{ LOGTARGET::NONE };
LOGFORMAT				Log::m_logFormat	{ LOGFORMAT::TXT };

Log::Log(LOGLEVEL _LogLevel, LOGTARGET _LogTarget, LOGFORMAT _LogFormat)
{
	Init(_LogLevel, _LogTarget, _LogFormat);
}

/**
 * @brief 初始化日志对象
 * 
 * @param _LogLevel		日志等级
 * @param _LogTarget	日志输出目标，命令行或者文件系统。
 */
void Log::Init(LOGLEVEL _LogLevel, LOGTARGET _LogTarget, LOGFORMAT _LogFormat)
{
	setLogLevel(_LogLevel);
	setLogTarget(_LogTarget);
	setLogFormat(_LogFormat);
}

/**
 * @brief 获取日志实例
 * 
 * @return Log& 当前日志对象实例
 */
Log& Log::Instance() noexcept
{
	static Log log;
	return log;
}

/**
 * @brief 获取程序目录下所有日志文件
 * 
 * @return QStringList 包含所有日志文件路径
 */
QStringList Log::getLogFiles()
{
    QDir dir(QCoreApplication::applicationDirPath());
    QStringList filters;
    filters << "*.txt" << "*.csv";
    dir.setNameFilters(filters);
    return dir.entryList();
}

/**
 * @brief 从文件中读取日志
 * 
 * @param _LogData	用于存储从文件读出的日志
 * @return true		日志读取成功 
 * @return false	日志读取失败
 */
bool Log::getLogFromFile(QStringList& _LogData, const QString& date)
{
	if (!isValidDate(date))
		return false;

    // 读锁
    std::shared_lock<std::shared_mutex> readLock(m_LogMutex);

    QFile fileInput(QCoreApplication::applicationDirPath() +
					LOGFILE + date + logSuffix());
    if (fileInput.open(QIODevice::ReadOnly)) {
        QTextStream stream(&fileInput);

        // 读取每一行数据
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            _LogData << line;
        }

        fileInput.close();
        return true;
    }
    return false;
}

/**
 * @brief 输出日志
 */
bool Log::outputToTarget()
{
	bool ret = false;
	int target = (int)getLogTarget();
	if (target & (int)LOGTARGET::FILE)
	{
		// 获取日期和时间
		char timeBuffer[20];
		getCurrentLocalTime(timeBuffer);
		QFile fileOutput(QCoreApplication::applicationDirPath() + LOGFILE +
						QString(timeBuffer).mid(0, DATE_LENGTH) + logSuffix());
		if (fileOutput.open(QIODevice::ReadWrite | QIODevice::Append))
		{
			fileOutput.write(m_LogBuffer.toUtf8());
			fileOutput.close();
			ret = true;
		}
	}
	if (target & (int)LOGTARGET::CONSOLE)
	{
		qDebug() << m_LogBuffer << endl;
		ret = true;
	}
	return ret;
}

/**
 * @brief 记录日志
 * 
 * @param _LogLevel		日志等级
 * @param _FileName		函数所在文件名
 * @param _Function		函数名
 * @param _LineNumber 	行号
 * @param _Text			日志正文
 */
bool Log::writeLog(
	const LOGLEVEL	_LogLevel,
	const QString& 	_FileName,
	const QString& 	_Function,
	const int 		_LineNumber,
	const QString& 	_Text)
{
	// 写锁
	std::scoped_lock<std::shared_mutex> writeLock(m_LogMutex);

	// 按日志等级确定是否需要记录
	if (_LogLevel > getLogLevel() || _LogLevel == LOGLEVEL::NONE)
		return false;

	// 清空之前的日志
	if (m_LogBuffer.size())
		m_LogBuffer.clear();

	// 获取日期和时间
	char timeBuffer[20];
	getCurrentLocalTime(timeBuffer);

	switch (m_logFormat)
	{
	case LOGFORMAT::TXT:
		// [Log等级][时间][进程号][线程号][文件名][函数名:行号]
		m_LogBuffer += QString("\n") +
			" [" + QString(STR_LOGLEVEL.at(_LogLevel))				+ "] " +
			" [" + QString(timeBuffer)								+ "] " +
			" [PID: " + QString::number(getpid())				  	+ "] " +
			" [TID: " + QString::number(gettid())				  	+ "] " +
			" [" + _FileName									  	+ "] " +
			" [" + _Function + ":" + QString::number(_LineNumber)	+ "]\n";
		break;
	case LOGFORMAT::CSV:
		{
			QFile file(QCoreApplication::applicationDirPath() + LOGFILE +
				QString(timeBuffer).mid(0, DATE_LENGTH) + logSuffix());
			bool needsHeader = !file.exists();
			if (needsHeader)
				m_LogBuffer += QString("时间,日志等级,进程号,线程号,文件名,位置\n");
			m_LogBuffer +=
				QString(timeBuffer) +							+ "," +
				QString(STR_LOGLEVEL.at(_LogLevel))				+ "," +
				QString::number(getpid())				  		+ "," +
				QString::number(gettid())				  		+ "," +
				_FileName									  	+ "," +
				_Function + ":" + QString::number(_LineNumber)	+ ",";
		}
		break;
	default:
		break;
	}

	// 日志正文
	m_LogBuffer += _Text + QString("\n");

	return outputToTarget();
}