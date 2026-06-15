module;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif


export module ImEngine.Core.IO:Sink;
import std;

export namespace ime::core {

enum class Level : unsigned char {
  Trace = 0,
  Debug = 1,
  Info = 2,
  Warn = 3,
  Error = 4,
  Fatal = 5,
};

namespace ConsoleColor {
  constexpr std::string_view Reset   = "\x1b[0m";
  constexpr std::string_view Red     = "\x1b[31m";
  constexpr std::string_view Green   = "\x1b[32m";
  constexpr std::string_view Yellow  = "\x1b[33m";
  constexpr std::string_view Blue    = "\x1b[34m";
  constexpr std::string_view Magenta = "\x1b[35m";
  constexpr std::string_view Cyan    = "\x1b[36m";
  constexpr std::string_view White   = "\x1b[37m";
  constexpr std::string_view Bold    = "\x1b[1m";
  constexpr std::string_view Underline = "\x1b[4m";

  constexpr std::string_view LevelToColor(const Level level) {
    switch (level) {
      case Level::Trace:
        return Magenta;
      case Level::Debug:
        return Cyan;
      case Level::Info:
        return Blue;
      case Level::Warn:
        return Yellow;
      case Level::Error:
        return Red ;
      default:
        return White;
    }
  }
}

class ISink {
public:
  ISink() = default;

  virtual ~ISink() = default;

  ISink(const ISink&) = delete;
  ISink& operator=(const ISink&) = delete;

  ISink(ISink&&) = default;
  ISink& operator=(ISink&&) = default;

  virtual void Flush() = 0;
  virtual void Write(Level level, std::string_view text) = 0;
};

class ConsoleSink : public ISink {
public:
  ConsoleSink() {
    /* Fck u */
#ifdef _WIN32
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE) return;

    DWORD mode = 0;
    if (!GetConsoleMode(handle, &mode)) return;

    SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
  };

  void Flush() override {
    std::osyncstream(std::cout).flush();
  };

  void Write(const Level level, const std::string_view _text) override {
    std::osyncstream(std::cout)
      << ConsoleColor::Reset
      << ConsoleColor::LevelToColor(level)
      << _text
      << ConsoleColor::Reset << std::endl;
  };
};

}
