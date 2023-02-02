#ifndef __MyLog_H__
#define __MyLog_H__
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <memory>
#include <string.h>
namespace mogger {

#define debug (format, ... ) \
    Logger::getInstance()->log(__LINE__, __FILE__, Logger::DEBUG, format, ##__VA_ARGS__ );

#define info(format, ... ) \
Logger::getInstance()->log(__LINE__, __FILE__, Logger::INFO, format, ##__VA_ARGS__ );

#define  warn(format, ... ) \
    Logger::getInstance()->log(__LINE__, __FILE__, Logger::,WARN, format, ##__VA_ARGS__ );

#define  error(format, ... ) \
    Logger::getInstance()->log(__LINE__, __FILE__, Logger::ERROR, format, ##__VA_ARGS__ );

#define  fatal(format, ... ) \
    Logger::getInstance()->log(__LINE__, __FILE__, Logger::FATAL, format, ##__VA_ARGS__ );

class Logger {
public:
  enum Level {
    DEBUG = 0,
    INFO,
    WARN,
    FATAL,  
    LEVEL_COUNT  
  };

 void openFile(std::string& filename);
 void close();
 static Logger* getInstance();
 void log(int line, const char* file, Level level,  char* format, ...);
 void setLevel(Level level);
 void setMax(int bytes);
 void rotate();
private:  
  Logger() = default;
  Logger(Logger& log) = delete;
  ~Logger() = default;
 
private:
  Level m_level;
  std::string m_filename;
  std::ofstream  m_out;
  static Logger* m_instance;
  static const char* s_level[LEVEL_COUNT];
  int m_len = 0;
  int m_max = 0;

};

}


#endif
