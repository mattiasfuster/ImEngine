export module ImEngine.Core.IO:Logger;
import :Sink;
import std;

export namespace ime::core {

template <typename... Args>
struct Formatter {
  std::format_string<Args...> fmt;
  std::source_location src;

  template <typename T>
  consteval Formatter(
    const T& _fmt,
    const std::source_location& _src = std::source_location::current()
  ) : fmt(_fmt), src(_src) {}
};

class Logger {
public:
  Logger() = delete;
  Logger(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(const Logger&) = delete;

  static const Logger& Instance;

  void Dispatch();
private:
  std::vector<std::unique_ptr<ISink>> sinks;
};

}

export namespace ime::log {

template <typename... Args>
void Info(core::Formatter<std::type_identity_t<Args>...> fmt, Args&&... args) {
  std::cout
    << std::format("[{}:{}] ", fmt.src.file_name(), fmt.src.line())
    << std::format(fmt.fmt, std::forward<Args>(args)...)
    << std::endl;
}

}




