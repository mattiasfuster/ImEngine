// MIT License
//
// Copyright (c) 2024 FUSTER Mattias
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

#include "Engine.h"

#include "Debug/Logger.h"

// Static accessor
Engine& Engine::Get() {
  static Engine instance;
  return instance;
}

Engine::Engine() = default;

Engine::~Engine() { Cleanup(); }

void Engine::Init([[maybe_unused]] int argc,
                  [[maybe_unused]] char* const argv[]) {
  InitWindow();
  InitVulkan();
}

void Engine::InitWindow() {
  IM_INFO("Initializing Window...");
  if (!glfwInit()) {
    IM_ERROR("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window_ = glfwCreateWindow(static_cast<int>(width_),
                             static_cast<int>(height_),
                             window_title_, nullptr, nullptr);
  if (!window_) {
    IM_ERROR("Failed to create GLFW window");
  }
}

void Engine::InitVulkan() {
  IM_INFO("Initializing Vulkan...");
  // Get required extensions from GLFW.
  uint32_t extension_count = 0;
  const char** extensions =
      glfwGetRequiredInstanceExtensions(&extension_count);

  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = window_title_;
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "ImEngine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  create_info.enabledExtensionCount = extension_count;
  create_info.ppEnabledExtensionNames = extensions;

  if (vkCreateInstance(&create_info, nullptr, &vulkan_instance_) !=
      VK_SUCCESS) {
    IM_ERROR("Failed to create Vulkan instance");
  }
}

void Engine::Run(int argc, char* const argv[]) {
  Init(argc, argv);
  MainLoop();
}

void Engine::MainLoop() const {
  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();
    // Future: update / render
  }
}

void Engine::Cleanup() const {
  IM_INFO("Cleaning up Engine...");
  if (vulkan_instance_ != VK_NULL_HANDLE) {
    vkDestroyInstance(vulkan_instance_, nullptr);
  }

  if (window_) {
    glfwDestroyWindow(window_);
    glfwTerminate();
  }
}