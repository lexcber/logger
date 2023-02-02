#include "MyLog.h"
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <iostream>
#include <stdarg.h>
using namespace mogger;

Logger* Logger::m_instance = nullptr;

Logger* Logger::getInstance() {
  if(m_instance == nullptr) {
    m_instance = new Logger;
  }

  return m_instance;
}

void Logger::openFile(std::string& filename) {
  m_filename = filename;
  m_out.open(filename);
  if(m_out.fail()) {
    throw std::logic_error("file open error " + filename);
  }
  m_out.seekp(0, std::ios::end);
  m_len = m_out.tellp();

}

void Logger::close() {
  m_out.close();
}

void Logger::log(int line, const char* file, Level level,  char* format, ...) {
  if(level < m_level) {
    return;
  }
  if(!m_out.is_open()) {
    throw std::logic_error("fail open log " + m_filename);
  }
  
  time_t tick ;
  struct tm* getime = localtime(&tick);
  char buf[32];
  memset(buf, 0x00, sizeof(buf));
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", getime);
  
  const char* file_format = "%s : %s : %s :%d"; 
  int size = std::snprintf(nullptr, 0, file_format, file, s_level[level], buf, line);
  if(size > 0) {
  char* file_buf = new char[size + 1] ;
  std::snprintf(file_buf, size+1, file_format, file, s_level[level], buf, line);
  m_out << file_buf;
  m_len += size; 
  delete []file_buf;
  }

  va_list arg_ptr;
  va_start(arg_ptr, format);
  size = vsnprintf(nullptr, 0, format, arg_ptr);
  va_end(arg_ptr);
  if(size > 0) {
    char* content = new char[size + 1];
    va_start(arg_ptr, format);
    vsnprintf(content, size + 1, format, arg_ptr);
    va_end(arg_ptr);
    m_out << content;
    m_len += size;
    delete []content;
  }
  m_out << "\n";

  if(m_len >= m_max && m_max > 0) {
    rotate();
  } 
  m_out.flush();    
}

void Logger::rotate() {
  close();
  time_t time;
  struct tm* getime = localtime(&time);
  char buf[32];
  memset(buf, 0x00, sizeof(buf));
  strftime(buf,  sizeof(buf), "%Y-%m-%d %H:%M:%S ", getime);
  std::string filename = buf + m_filename;
  if( (std::rename(m_filename.c_str(), filename.c_str())) != 0) {
    throw std::logic_error("fail rename :" + filename);
  }

  openFile(m_filename);

}

void Logger::setLevel(Level level) { 
  m_level = level;
}

void Logger::setMax(int bytes) {
  m_max = bytes;
}

 
