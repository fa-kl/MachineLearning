#ifndef UTILS_EXCEPTION_H
#define UTILS_EXCEPTION_H

#include <exception>
#include <string>
#include <utility>

class Error : public std::exception {
protected:
  std::string m_type;
  std::string m_msg;

public:
  explicit Error(std::string msg) : m_type("Error"), m_msg(std::move(msg)) {}

  explicit Error(std::string type, std::string msg)
      : m_type(std::move(type)), m_msg(std::move(msg)) {}

  [[nodiscard]] const char *what() const noexcept override {
    static std::string str = "[" + m_type + "]: " + m_msg;
    return str.c_str();
  }
};

#endif  // UTILS_EXCEPTION_H
