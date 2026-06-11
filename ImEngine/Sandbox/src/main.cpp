import ImEngine.Core.IO;
import std;

int main() {
  ime::core::Logger::Start(ime::core::Level::Debug);
  ime::core::Logger::AddSink(std::make_unique<ime::core::ConsoleSink>());
}