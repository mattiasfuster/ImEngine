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

template <typename... Args>
struct Formatter {
  std::format_string<Args...> fmt;
  std::source_location src;

  explicit Formatter(
    std::format_string<Args...> _fmt,
    const std::source_location& _src = std::source_location::current()
  ) : fmt(_fmt), src(_src) {}
};

class ISink {
public:
  ISink() = default;

  virtual ~ISink() = default;

  ISink(const ISink&) = delete;
  ISink& operator=(const ISink&) = delete;

  ISink(ISink&&) = default;
  ISink& operator=(ISink&&) = default;

  virtual void flush() = 0;
  virtual void write(Level level, std::string_view text) = 0;
};

class ConsoleSink : public ISink {
public:
  ConsoleSink() = default;
  ~ConsoleSink() override;

  void flush() override;
  void write(Level level, std::string_view text) override;
};

}
