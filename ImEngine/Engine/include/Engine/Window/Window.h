#ifndef IMENGINE_ENGINE_WINDOW_WINDOW_H
#define IMENGINE_ENGINE_WINDOW_WINDOW_H

#include "EngineExport.h"

#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ime::engine::platform {

struct WindowConfig {
  std::string window_title = "ImEngine";

  struct WindowSize {
    uint32_t width = 1280;
    uint32_t height = 720;
  } size;
};

class IMENGINE_ENGINE_API IWindow {
 public:
  virtual ~IWindow() = default;
  virtual void Initialize() = 0;
  virtual void* GetNativeHandle() = 0;
  [[nodiscard]] virtual bool ShouldClose() const = 0;
  [[nodiscard]] virtual const WindowConfig& GetWindowConfig() const = 0;

 protected:
  IWindow() = default;
  IWindow(const IWindow&) = default;
  IWindow& operator=(const IWindow&) = default;
  IWindow(IWindow&&) = default;
  IWindow& operator=(IWindow&&) = default;
};

class IMENGINE_ENGINE_API IBackendManager {
 public:
  IBackendManager() = default;
  virtual ~IBackendManager() = default;

  IBackendManager(const IBackendManager&) = delete;
  IBackendManager& operator=(const IBackendManager&) = delete;
  IBackendManager(IBackendManager&&) = delete;
  IBackendManager& operator=(IBackendManager&&) = delete;

  void AddWindow() {
    if (window_count_++ == 0) {
      OnFirstWindowCreated();
    }
  }

  void RemoveWindow() {
    if (--window_count_ == 0) {
      OnLastWindowDestroyed();
    }
  }

  [[nodiscard]] int GetWindowCount() const noexcept {
    return window_count_.load();
  }

 protected:
  virtual void OnFirstWindowCreated() = 0;
  virtual void OnLastWindowDestroyed() = 0;

 private:
  std::atomic<int> window_count_{0};
};

class IMENGINE_ENGINE_API WindowManager {
 public:
  WindowManager(const WindowManager&) = delete;
  WindowManager& operator=(const WindowManager&) = delete;
  WindowManager(WindowManager&&) = delete;
  WindowManager& operator=(WindowManager&&) = delete;

  enum class Backend : uint8_t {
    GLFW,
  };

  using WindowFactory =
      std::function<std::shared_ptr<IWindow>(const WindowConfig&)>;
  using BackendPollFunction = std::function<void()>;

  static WindowManager& Instance();

  void RegisterBackend(Backend backend, WindowFactory factory,
                       BackendPollFunction poll_function);

  std::shared_ptr<IWindow> CreateWindow(const WindowConfig& config,
                                        Backend backend = Backend::GLFW);

  [[nodiscard]] size_t GetWindowCount() const;
  void CleanupDestroyedWindows();
  void PollEvents();

  [[nodiscard]] std::vector<std::shared_ptr<IWindow>> GetActiveWindows() const;

 private:
  WindowManager() = default;
  ~WindowManager() = default;

  void TrackWindow(const std::shared_ptr<IWindow>& window);

  std::vector<std::weak_ptr<IWindow>> windows_;
  std::unordered_map<Backend, WindowFactory> factories_;
  std::unordered_map<Backend, BackendPollFunction> poll_functions_;
};

}  // namespace ime::engine::platform

#endif  // IMENGINE_ENGINE_WINDOW_WINDOW_H