#pragma once
#include <cstring>
#include <iostream>
#include <fstream>

namespace yazi {
	namespace utility{
#define debug(format, ...)	\
	Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define info(format, ...)	\
	Logger::instance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define warn(format, ...)	\
	Logger::instance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define error(format, ...)	\
	Logger::instance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define fatal(format, ...)	\
	Logger::instance()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);


		class Logger {

		public:
			enum Level
			{
				DEBUG = 0,
				INFO,
				WARN,
				ERROR,
				FATAL,
				LEVEL_COUNT
			};
			static Logger* instance();
			void open(const std::string& filename);

			// 该日志属于线程不安全的，在多线程中log函数要加锁
			void log(Level level, const char* file, int line, const char* format, ...);

			void close();
			void level(Level level);
			void max(int bytes);
			
		private:
			Logger();
			~Logger();
			void rotate();

		private:
			Level m_level;
			int m_max; // 日志的最大值
			int m_len; // 当前日志的长度
			std::string m_filename; // 当前操作的文件名称
			std::ofstream m_fout;
			static const char* s_level[LEVEL_COUNT];
			static Logger* m_instance;
	};


}
}

