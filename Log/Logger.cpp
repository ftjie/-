#pragma warning( disable : 4996 )
#include "Logger.h"

#include <time.h>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <stdarg.h>
#include <thread>

using namespace yazi::utility;

const char* Logger::s_level[LEVEL_COUNT] = {
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
};

Logger* Logger::m_instance = NULL; // 静态成员使用前要先初始化

Logger::Logger() : m_max(0), m_len(0), m_level(DEBUG)
{

}

Logger::~Logger()
{
	close();

}

Logger* Logger::instance()
{
	if (m_instance == NULL)
	{
		m_instance = new Logger();
	}

	return m_instance;
}

void Logger::open(const std::string& filename)
{
	m_filename = filename;
	m_fout.open(filename, std::ios::app);
	if (m_fout.fail())
	{
		throw std::logic_error("open file failed " + filename);
	}

	m_fout.seekp(0, std::ios::end); // 设置输出文件流的文件流指针位置
	m_len = m_fout.tellp(); // 当我们写入一个文件，并要知道内置指针的当前位置时，应该使用tellp().
}

void Logger::close()
{
	m_fout.close();

}

void Logger::log(Level level, const char* file, int line, const char* format, ...)
{
	if (m_level > level)
	{
		return;
	}

	if (m_fout.fail())
	{
		throw std::logic_error("open file failed " + m_filename);
	}

	// 时间戳
	// std::this_thread::sleep_for(std::chrono::seconds(1)); //休眠一秒
	time_t ticks = time(NULL);
	struct tm* ptm = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof(timestamp));
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);
	const char* fmt = "%s %s %s:%d ";

	int size = snprintf(NULL, 0, fmt, timestamp, s_level[level], file, line);

	if (size > 0)
	{
		char *buffer = new char[size + 1];
		snprintf(buffer, size + 1, fmt, timestamp, s_level[level], file, line);
		buffer[size] = '\0';
		std::cout << buffer << std::endl;
		m_fout << buffer;
		m_len += size;
		delete buffer;
	}

	va_list arg_ptr;
	va_start(arg_ptr, format);
	size = vsnprintf(NULL, 0, format, arg_ptr);
	va_end(arg_ptr);
	if (size > 0)
	{
		char* content = new char[size + 1];
		va_start(arg_ptr, format);
		vsnprintf(content, size + 1, format, arg_ptr);
		va_end(arg_ptr);
		std::cout << content << std::endl;
		m_fout << content;
		m_len += size;
		delete content;
	}
	m_fout << "\n";
	m_fout.flush();

	if (m_len >= m_max && m_max > 0)
	{
		rotate();
	}

	/*std::cout << timestamp << std::endl;
	std::cout << file << std::endl;
	std::cout << line << std::endl;
	std::cout << format << std::endl;*/

	return;
}

void Logger::level(Level levelTemp)
{
	m_level = levelTemp;
}

void Logger::max(int bytes)
{
	m_max = bytes;
}

void Logger::rotate()
{
	close();
	time_t ticks = time(NULL);
	struct tm* ptm = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof(timestamp));
	strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H-%M-%S", ptm);

	std::string filename = m_filename + timestamp;

	m_filename = filename;
	//if (rename(m_filename.c_str(), filename.c_str()) != 0)
	//{
	//	std::cout << m_filename << "     xxxxxxx         " <<filename<< std::endl;
	//	//throw std::logic_error("rename log file failed: " + std::string(strerror(errno)));
	//}

	open(m_filename);
	std::cout << m_filename << std::endl;
	return;
}