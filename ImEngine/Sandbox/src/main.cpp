import ImEngine.Core;
import std;

int main() {
  ime::core::Logger::Start(ime::core::Level::Debug);
  ime::core::Logger::AddSink(std::make_unique<ime::core::ConsoleSink>());

  ime::log::Warn("Hello World! {} {}", 6, 7);

  return 0;
}