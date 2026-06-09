export module ImEngine.Core.Thread:Worker;
import std;

export namespace ime::core {

class Worker {
public:
  Worker() = default;
  Worker(const Worker&) = delete;
  Worker(Worker&&) = delete;
  Worker& operator=(const Worker&) = delete;
  Worker& operator=(Worker&&) = delete;

  virtual ~Worker() = default;

  explicit Worker(std::optional<int> _numWorkers = std::nullopt);

private:

};

}

