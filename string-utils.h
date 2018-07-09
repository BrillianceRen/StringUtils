#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

#ifndef STRING_UTILS_H
#define STRING_UTILS_H


static inline std::vector<std::string> Compact(const std::vector<std::string> &tokens){
  std::vector<std::string> compacted;

  for(size_t i=0; i<tokens.size(); ++i) {
    if (!tokens[i].empty()) {
      compacted.push_back(tokens[i]);
    }
  }

  return compacted;
}

static inline std::vector<std::string> Split(const std::string &str, const std::string &delim, const bool trim_empty = false){
  size_t pos, last_pos = 0, len;
  std::vector<std::string> tokens;

  while(true) {
    pos = str.find(delim, last_pos);
    if (pos == std::string::npos) {
      pos = str.size();
    }

    len = pos-last_pos;
    if ( !trim_empty || len != 0) {
      tokens.push_back(str.substr(last_pos, len));
    }

    if (pos == str.size()) {
      break; 
    } else {
      last_pos = pos + delim.size();
    }
  }

  return tokens;
}

static inline std::string Join(const std::vector<std::string> &tokens, const std::string &delim, const bool trim_empty = false){
  if(trim_empty) {
    return Join(Compact(tokens), delim, false);
  } else {
    std::stringstream ss;
    for(size_t i=0; i<tokens.size()-1; ++i) {
      ss << tokens[i] << delim;
    }
    ss << tokens[tokens.size()-1];

    return ss.str();
  }
}

static inline std::string Trim(const std::string &str){
  
  std::string blank = "\r\n\t ";
  size_t begin = str.size(), end = 0;
  for (size_t i=0; i<str.size(); ++i) {
    if ( blank.find(str[i]) == std::string::npos) {
      begin = i;
      break;
    }
  }

  for (size_t i=str.size(); i>0; --i) {
    if ( blank.find(str[i-1]) == std::string::npos) {
      end = i-1;
      break;
    }
  }
  
  if (begin >= end) {
    return "";
  } else {
    return str.substr(begin, end-begin+1);
  }
}

static inline std::string Repeat(const std::string &str, unsigned int times){
  std::stringstream ss;
  for(unsigned int i=0; i<times; ++i) {
    ss << str;
  }
  return ss.str();
}

static inline std::string ReplaceAll(const std::string &source, const std::string &target, const std::string &replacement){
  return Join(Split(source, target, false), replacement, false);
}

static inline std::string ToUpper(const std::string &str){
  std::string s(str);
  std::transform(s.begin(), s.end(), s.begin(), toupper);
  return s;
}

static inline std::string ToLower(const std::string &str){
  std::string s(str);
  std::transform(s.begin(), s.end(), s.begin(), tolower);
  return s;
}


static inline std::string ReadFile(const std::string &filepath) {
  std::ifstream ifs(filepath.c_str());
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
  ifs.close();
  return content;
}

static inline void WriteFile(const std::string &filepath, const std::string &content) {
  std::ofstream ofs(filepath.c_str());
  ofs << content;
  ofs.close();
  return;
}


/**
 * Convert all std::strings to const char* using constexpr if (C++17)
 */
template<typename T>
static auto Convert(T&& t) {
  if constexpr (std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value) {
    return std::forward<T>(t).c_str();
  }
  else {
    return std::forward<T>(t);
  }
}

/**
 * printf like formatting for C++ with std::string
 * Original source: https://stackoverflow.com/a/26221725/11722
 */
template<typename ... Args>
static std::string StringFormatInternal(const std::string& format, Args&& ... args)
{
  size_t size = snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args) ...) + 1;
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), args ...);
  return std::string(buf.get(), buf.get() + size - 1);
}

template<typename ... Args>
static std::string StringFormat(std::string fmt, Args&& ... args) {
  return StringFormatInternal(fmt, Convert(std::forward<Args>(args))...);
}

#endif
