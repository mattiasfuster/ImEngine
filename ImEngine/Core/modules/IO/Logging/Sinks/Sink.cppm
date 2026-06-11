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

enum class Color : unsigned char {
};

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
  ConsoleSink() = default;

  void Flush() override {

  };

  void Write(Level level, std::string_view text) override {
    std::cout << text;
  };
};

}
