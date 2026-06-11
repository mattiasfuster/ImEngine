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

  static void SetLevel(const Level _level) { Instance().level = _level; };

  static void AddSink(std::unique_ptr<ISink> _sink);

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
  std::clog << __func__ << std::endl;
}

void Logger::Stop() {
  auto& inst = Instance();
  inst.sinks.clear();
  std::clog << __func__ << std::endl;
}
void Logger::AddSink(std::unique_ptr<ISink> _sink) {
  Instance().sinks.emplace_back(std::move(_sink));
}

}  // namespace ime::core

