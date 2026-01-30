// MIT License
//
// Copyright (c) 2024 FUSTER Mattias
//
// This software utilizes code from the following GitHub repositories, which are
// also licensed under the MIT License:
//
// - [ImGui](https://github.com/ocornut/imgui)
// - [GLFW](https://github.com/glfw/glfw)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Engine/Window/Contexts/GLFWWindow.h"

#include "Core/Debug/Logger.h"


using namespace ime::engine::platform;

namespace {
bool RegisterGLFWBackend() {
  WindowManager::Instance().RegisterBackend(
      WindowManager::Backend::GLFW,
      [](const WindowConfig& config) -> std::shared_ptr<IWindow> {
        return std::make_shared<GLFWWindow>(config);
      },
      []() {
        if (GLFWBackendManager::Instance().GetWindowCount() > 0) {
          glfwPollEvents();
        }
      });
  return true;
}

const bool glfw_registered = RegisterGLFWBackend();
}  // namespace

GLFWBackendManager& GLFWBackendManager::Instance() {
  static GLFWBackendManager instance;
  return instance;
}

void GLFWBackendManager::OnFirstWindowCreated() {
  IM_INFO("Initializing GLFW backend...");
  if (!glfwInit()) {
    IM_ERROR("Failed to initialize GLFW");
    throw std::runtime_error("Failed to initialize GLFW");
  }
  IM_INFO("GLFW backend initialized (window count: 1)");
}

void GLFWBackendManager::OnLastWindowDestroyed() {
  IM_INFO("Terminating GLFW backend (no more windows)");
  glfwTerminate();
}

GLFWWindow::GLFWWindow(WindowConfig config) : config_(std::move(config)) {}

void GLFWWindow::Initialize() {
  IM_INFO("Creating GLFW window: \"{}\" ({}x{})", config_.window_title,
          config_.size.width, config_.size.height);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // Vulkan
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  handle_.reset(glfwCreateWindow(static_cast<int>(config_.size.width),
                                 static_cast<int>(config_.size.height),
                                 config_.window_title.c_str(), nullptr,
                                 nullptr));

  if (!handle_) {
    IM_ERROR("Failed to create GLFW window");
    throw std::runtime_error("Failed to create GLFW window");
  }

  IM_INFO("GLFW window created successfully");
}

void* GLFWWindow::GetNativeHandle() {
  return static_cast<void*>(handle_.get());
}

bool GLFWWindow::ShouldClose() const {
  return handle_ && glfwWindowShouldClose(handle_.get());
}