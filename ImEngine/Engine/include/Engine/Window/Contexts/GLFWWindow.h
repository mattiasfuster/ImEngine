#ifndef IMENGINE_ENGINE_WINDOW_CONTEXTS_GLFWWINDOW_H
#define IMENGINE_ENGINE_WINDOW_CONTEXTS_GLFWWINDOW_H

#include "Engine/Window/Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace ime::engine::platform {

class IMENGINE_ENGINE_API GLFWBackendManager : public IBackendManager {
 public:
  static GLFWBackendManager& Instance();

  ~GLFWBackendManager() override = default;
  GLFWBackendManager(const GLFWBackendManager&) = delete;
  GLFWBackendManager& operator=(const GLFWBackendManager&) = delete;
  GLFWBackendManager(GLFWBackendManager&&) = delete;
  GLFWBackendManager& operator=(GLFWBackendManager&&) = delete;

 protected:
  void OnFirstWindowCreated() override;
  void OnLastWindowDestroyed() override;

 private:
  GLFWBackendManager() = default;
};

class IMENGINE_ENGINE_API GLFWBackendScope {
 public:
  GLFWBackendScope() { GLFWBackendManager::Instance().AddWindow(); }

  ~GLFWBackendScope() { GLFWBackendManager::Instance().RemoveWindow(); }

  GLFWBackendScope(const GLFWBackendScope&) = delete;
  GLFWBackendScope& operator=(const GLFWBackendScope&) = delete;
  GLFWBackendScope(GLFWBackendScope&&) = delete;
  GLFWBackendScope& operator=(GLFWBackendScope&&) = delete;
};

class IMENGINE_ENGINE_API GLFWWindow : public IWindow {
 public:
  explicit GLFWWindow(WindowConfig config);
  ~GLFWWindow() override = default;

  GLFWWindow(const GLFWWindow&) = delete;
  GLFWWindow& operator=(const GLFWWindow&) = delete;
  GLFWWindow(GLFWWindow&&) = delete;
  GLFWWindow& operator=(GLFWWindow&&) = delete;

  void Initialize() override;
  void* GetNativeHandle() override;
  [[nodiscard]] bool ShouldClose() const override;
  [[nodiscard]] const WindowConfig& GetWindowConfig() const override {
    return config_;
  }

  [[nodiscard]] GLFWwindow* GetGLFWHandle() const noexcept {
    return handle_.get();
  }

 private:
  GLFWBackendScope backend_scope_;

  struct GLFWDeleter {
    void operator()(GLFWwindow* window) const noexcept {
      if (window) {
        glfwDestroyWindow(window);
      }
    }
  };

  std::unique_ptr<GLFWwindow, GLFWDeleter> handle_;
  WindowConfig config_;
};

}  // namespace ime::engine::platform

#endif  // IMENGINE_ENGINE_WINDOW_CONTEXTS_GLFWWINDOW_H