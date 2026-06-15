export module ImEngine.Core.IO:Logger;
import :Sink;
import std;

export namespace ime::core {

class Logger {
public:
  Logger(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger& operator=(Logger&&) = delete;

  static Logger& Instance() {
    static Logger instance;
    return instance;
  };

  static void Start(Level _level);
  static void Stop();

  static void SetLevel(const Level _level) {
    auto& inst = Instance();
    inst.level = _level;
  };

  static Level GetLevel() {
    const auto& inst = Instance();
    return inst.level;
  };

  static void AddSink(std::unique_ptr<ISink> _sink);

  static void Dispatch(Level _level, std::string_view _text);

private:
  Logger() = default;
  ~Logger() { Stop(); };

  Level level = Level::Debug;
  std::vector<std::unique_ptr<ISink>> sinks = {};
};

void Logger::Start(const Level _level) {
  auto& inst = Instance();
  SetLevel(_level);
  inst.sinks.reserve(8);
}

void Logger::Stop() {
  auto& inst = Instance();
  std::ranges::for_each(
    inst.sinks,
    [&](const std::unique_ptr<ISink>& _sink) {
      _sink->Flush();
    });
  inst.sinks.clear();
}

void Logger::AddSink(std::unique_ptr<ISink> _sink) {
  auto& inst = Instance();
  inst.sinks.emplace_back(std::move(_sink));
}
void Logger::Dispatch(const Level _level, const std::string_view _text) {
  auto& inst = Instance();
  std::ranges::for_each(
    inst.sinks,
    [&](const std::unique_ptr<ISink>& _sink) {
      _sink->Write(_level, _text);
    });
}

}  // namespace ime::core

