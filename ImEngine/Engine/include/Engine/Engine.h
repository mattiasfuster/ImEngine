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

#pragma once
#include "Engine/Window/Window.h"
#include "EngineExport.h"


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <memory>
#include <optional>
#include <vector>

namespace ime {

struct IMENGINE_ENGINE_API QueueFamilyIndices {
  std::optional<uint32_t> graphics_family;
  std::optional<uint32_t> present_family;

  [[nodiscard]] constexpr bool is_complete() const noexcept {
    return graphics_family.has_value() && present_family.has_value();
  }
};

struct IMENGINE_ENGINE_API SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities = {};
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

struct IMENGINE_ENGINE_API EngineConfig {
  std::optional<ime::engine::platform::WindowConfig> window_config;
};

class IMENGINE_ENGINE_API Engine {
 public:
  // Static Data members
  static constexpr int kMaxFramesInFlight = 2;

  static constexpr std::array<const char*, 1> kValidationLayers = {
      "VK_LAYER_KHRONOS_validation"};

  static constexpr std::array<const char*, 1> kDeviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef IMENGINE_DEBUG
  static constexpr bool kEnableValidationLayers = true;
#else
  static constexpr bool kEnableValidationLayers = false;
#endif

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

  Engine(Engine&& other) noexcept;
  Engine& operator=(Engine&& other) noexcept;

  Engine(const EngineConfig& Config);

  ~Engine();

  void Run();

 private:
  static void PopulateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT& create_info);

  static VKAPI_ATTR VkBool32 VKAPI_CALL
  DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                VkDebugUtilsMessageTypeFlagsEXT message_type,
                const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                void* user_data);

  void InitVulkan();

  void CreateInstance();
  void SetupDebugMessenger();
  void CreateSurface();
  void PickPhysicalDevice();
  void CreateLogicalDevice();
  void CreateSwapChain();
  void CreateImageViews();
  void CreateRenderPass();
  void CreateGraphicsPipeline();
  void CreateFramebuffers();
  void CreateCommandPool();
  void CreateCommandBuffer();
  void CreateSyncObjects();

  void MainLoop();
  void DrawFrame();
  void Cleanup();

  void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

  [[nodiscard]]
  std::vector<const char*> GetRequiredInstanceExtensions() const;
  [[nodiscard]]
  bool CheckValidationLayerSupport() const;
  [[nodiscard]]
  bool CheckRequiredInstanceExtensionsSupport() const;
  [[nodiscard]]
  bool CheckRequiredDeviceExtensionsSupport(VkPhysicalDevice device) const;
  [[nodiscard]]
  bool IsDeviceSuitable(const VkPhysicalDevice& device) const;
  [[nodiscard]]
  QueueFamilyIndices FindQueueFamiliesIndices(VkPhysicalDevice device) const;
  [[nodiscard]]
  SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
  [[nodiscard]]
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
  [[nodiscard]]
  VkPresentModeKHR ChooseSwapPresentMode(
      const std::vector<VkPresentModeKHR>& availablePresentModes) const;
  [[nodiscard]]
  VkExtent2D ChooseSwapExtent(
      const VkSurfaceCapabilitiesKHR& capabilities) const;
  [[nodiscard]] VkShaderModule CreateShaderModule(
      const std::vector<char>& code) const;

  std::shared_ptr<ime::engine::platform::IWindow> window_;

  VkInstance vulkan_instance_ = VK_NULL_HANDLE;

  VkDebugUtilsMessengerEXT debug_messenger_ = VK_NULL_HANDLE;

  VkSurfaceKHR vulkan_surface_ = VK_NULL_HANDLE;

  VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
  VkDevice logical_device_ = VK_NULL_HANDLE;

  VkQueue graphics_queue_ = VK_NULL_HANDLE;
  VkQueue present_queue_ = VK_NULL_HANDLE;

  VkSwapchainKHR swap_chain_ = VK_NULL_HANDLE;
  std::vector<VkImage> swap_chain_images_;
  std::vector<VkImageView> swap_chain_image_views_;
  VkFormat swap_chain_image_format_ = VkFormat::VK_FORMAT_B8G8R8_SRGB;
  VkExtent2D swap_chain_extent_ = {1280, 720};

  VkRenderPass render_pass_ = VK_NULL_HANDLE;
  VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
  VkPipeline graphics_pipeline_ = VK_NULL_HANDLE;

  std::vector<VkFramebuffer> swap_chain_framebuffers_;

  VkCommandPool command_pool_ = VK_NULL_HANDLE;
  std::vector<VkCommandBuffer> command_buffers_;

  std::vector<VkSemaphore> image_available_semaphores_;
  std::vector<VkSemaphore> render_finished_semaphores_;
  std::vector<VkFence> in_flight_fences_;

  uint32_t current_frame_ = 0;
};

}  // namespace ime