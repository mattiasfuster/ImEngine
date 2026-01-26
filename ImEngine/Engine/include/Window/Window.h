#ifndef IMENGINE_ENGINE_WINDOW_WINDOW_H
#define IMENGINE_ENGINE_WINDOW_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Debug/Logger.h"

namespace ImEngine::Engine {

struct WindowConfig {
  std::string window_title = "ImEngine";
  uint32_t width = 1280;
  uint32_t height = 720;
};

class IMENGINE_ENGINE_API Window {
public:
  explicit Window(uint32_t width, uint32_t height, std::string_view title);
  explicit Window(const WindowConfig &config = {});

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  Window(Window &&) noexcept;
  Window &operator=(Window &&) noexcept;

  ~Window();

  void Initialize() {
    IM_INFO("Initializing window...");

    if (!glfwInit()) {
      IM_ERROR("Failed to initialize GLFW");
      throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfw_window_ =
        glfwCreateWindow(static_cast<int>(window_config_.width),
                         static_cast<int>(window_config_.height),
                         window_config_.window_title.c_str(), nullptr, nullptr);

    if (!glfw_window_) {
      IM_ERROR("Failed to create GLFW window");
      glfwTerminate();
      throw std::runtime_error("Failed to create GLFW window");
    }
  };

  void Cleanup() {
    if (glfw_window_) {
      glfwDestroyWindow(glfw_window_);
      glfw_window_ = nullptr;
    }
    glfwTerminate();
  }

private:
  WindowConfig window_config_;

  GLFWwindow *glfw_window_ = nullptr;
};

} // namespace ImEngine::Engine

#endif // IMENGINE_ENGINE_WINDOW_WINDOW_H