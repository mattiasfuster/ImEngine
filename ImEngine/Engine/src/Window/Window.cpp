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

#include "Engine/Window/Window.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include "Core/Debug/Logger.h"

using namespace ime::engine::platform;

WindowManager& WindowManager::Instance() {
  static WindowManager instance;
  return instance;
}

void WindowManager::RegisterBackend(
    Backend backend, WindowManager::WindowFactory factory,
    WindowManager::BackendPollFunction poll_function) {
  factories_[backend] = std::move(factory);
  poll_functions_[backend] = std::move(poll_function);
  IM_INFO("Backend registered: {}", static_cast<int>(backend));
}

std::shared_ptr<IWindow> WindowManager::CreateWindow(const WindowConfig& config,
                                                     Backend backend) {
  auto it = factories_.find(backend);
  if (it == factories_.end()) {
    IM_ERROR("Backend not registered: {}", static_cast<int>(backend));
    throw std::runtime_error("Backend not registered");
  }

  std::shared_ptr<IWindow> window = it->second(config);
  window->Initialize();
  TrackWindow(window);

  IM_INFO("Window created and tracked (total windows: {})", GetWindowCount());

  return window;
}

size_t WindowManager::GetWindowCount() const {
  return std::count_if(
      windows_.begin(), windows_.end(),
      [](const auto& weak_win) { return !weak_win.expired(); });
}

void WindowManager::CleanupDestroyedWindows() {
  const size_t before = windows_.size();
  std::erase_if(windows_,
                [](const auto& weak_win) { return weak_win.expired(); });
  const size_t after = windows_.size();

  if (before != after) {
    IM_INFO("Cleaned up {} destroyed window(s)", before - after);
  }
}

void WindowManager::PollEvents() {
  CleanupDestroyedWindows();
  for (const auto& [backend, poll_function] : poll_functions_) {
    if (poll_function) {
      poll_function();
    }
  }
}

std::vector<std::shared_ptr<IWindow>> WindowManager::GetActiveWindows() const {
  std::vector<std::shared_ptr<IWindow>> active;
  active.reserve(windows_.size());

  for (const auto& weak_window : windows_) {
    if (auto window = weak_window.lock()) {
      active.push_back(window);
    }
  }

  return active;
}

void WindowManager::TrackWindow(const std::shared_ptr<IWindow>& window) {
  windows_.push_back(window);
}
