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
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>

Engine::Engine(const EngineConfig& config)
    : window_title_(config.title),
      width_(config.width),
      height_(config.height) {
  InitWindow();
  InitVulkan();
}

Engine::~Engine() {
  Cleanup();
}

Engine::Engine(Engine&& other) noexcept
    : window_(other.window_),
      vulkan_instance_(other.vulkan_instance_),
      debug_messenger_(other.debug_messenger_),
      window_title_(other.window_title_),
      width_(other.width_),
      height_(other.height_) {
  other.window_ = nullptr;
  other.vulkan_instance_ = VK_NULL_HANDLE;
  other.debug_messenger_ = VK_NULL_HANDLE;
}

Engine& Engine::operator=(Engine&& other) noexcept {
  if (this != &other) {
    Cleanup();
    window_ = other.window_;
    vulkan_instance_ = other.vulkan_instance_;
    debug_messenger_ = other.debug_messenger_;
    window_title_ = other.window_title_;
    width_ = other.width_;
    height_ = other.height_;
    other.window_ = nullptr;
    other.vulkan_instance_ = VK_NULL_HANDLE;
    other.debug_messenger_ = VK_NULL_HANDLE;
  }
  return *this;
}

void Engine::Run() {
  MainLoop();
}

void Engine::InitWindow() {
  IM_INFO("Initializing window...");
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

  if constexpr (kEnableValidationLayers) {
    if (!CheckValidationLayerSupport()) {
      throw std::runtime_error("validation layers requested, but not available!");
    }
  }

  if (!CheckRequiredInstanceExtensionsSupport()) {
    throw std::runtime_error("required Vulkan extensions not available!");
  }

  CreateInstance();
  SetupDebugMessenger();
  CreateSurface();
  PickPhysicalDevice();
  CreateLogicalDevice();
  CreateSwapChain();
}

void Engine::CreateInstance() {
  IM_INFO("Creating instance...");
  VkApplicationInfo app_info{
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pNext = nullptr,
      .pApplicationName = window_title_,
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName = "ImEngine",
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = VK_API_VERSION_1_3,
  };

  auto extensions = GetRequiredInstanceExtensions();

  VkInstanceCreateInfo create_info{
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .pApplicationInfo = &app_info,
      .enabledLayerCount = 0,
      .ppEnabledLayerNames = nullptr,
      .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
      .ppEnabledExtensionNames = extensions.data(),
  };

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};

  if constexpr (kEnableValidationLayers) {
    create_info.enabledLayerCount =
        static_cast<uint32_t>(kValidationLayers.size());
    create_info.ppEnabledLayerNames = kValidationLayers.data();

    PopulateDebugMessengerCreateInfo(debug_create_info);
    create_info.pNext = &debug_create_info;
  }

  if (vkCreateInstance(&create_info, nullptr, &vulkan_instance_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create Vulkan instance!");
  }
}

void Engine::PopulateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& create_info) {
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = DebugCallback;
  create_info.pUserData = nullptr;
}

void Engine::SetupDebugMessenger() {
  if constexpr (!kEnableValidationLayers) return;

  IM_INFO("Setup Vulkan Debug Messenger...");

  VkDebugUtilsMessengerCreateInfoEXT create_info;
  PopulateDebugMessengerCreateInfo(create_info);

  auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(vulkan_instance_, "vkCreateDebugUtilsMessengerEXT"));

  if (func == nullptr) {
    IM_ERROR("Failed to load vkCreateDebugUtilsMessengerEXT");
    return;
  }

  if (func(vulkan_instance_, &create_info, nullptr, &debug_messenger_) != VK_SUCCESS) {
    IM_ERROR("Failed to set up debug messenger");
  }
}

void Engine::CreateSurface()
{
  IM_INFO("Creating surface...");
  if (glfwCreateWindowSurface(vulkan_instance_, window_, nullptr, &vulkan_surface_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface!");
  }
}

void Engine::PickPhysicalDevice() {
  IM_INFO("Picking physical device...");

  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(vulkan_instance_, &deviceCount, nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("Failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(vulkan_instance_, &deviceCount, devices.data());

  for (const VkPhysicalDevice& device : devices) {
    if (IsDeviceSuitable(device)) {
      physical_device_ = device;
      break;
    }
  }

  if (physical_device_ == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to find a suitable GPU!");
  }
}

void Engine::CreateLogicalDevice() {
  IM_INFO("Creating logical device...");

  QueueFamilyIndices indices = FindQueueFamiliesIndices(physical_device_);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = {
      indices.graphics_family.value(),
      indices.present_family.value()
  };

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount = static_cast<uint32_t>(kDeviceExtensions.size());
  createInfo.ppEnabledExtensionNames = kDeviceExtensions.data();

  if (kEnableValidationLayers) {
      createInfo.enabledLayerCount = static_cast<uint32_t>(kValidationLayers.size());
      createInfo.ppEnabledLayerNames = kValidationLayers.data();
  } else {
      createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(physical_device_, &createInfo, nullptr, &logical_device_) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create logical device!");
  }

  vkGetDeviceQueue(logical_device_, indices.graphics_family.value(), 0, &graphics_queue_);
  vkGetDeviceQueue(logical_device_, indices.present_family.value(), 0, &present_queue_);
}

void Engine::CreateSwapChain()
{
  IM_INFO("Creating swap chain...");

  SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physical_device_);

  VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = ChooseSwapPrensentMode(swapChainSupport.presentModes);
  VkExtent2D extent = ChooseSwapExtenT(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo {};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = vulkan_surface_;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = FindQueueFamiliesIndices(physical_device_);
  uint32_t queueFamilyIndices[] = {indices.graphics_family.value(), indices.present_family.value()};
  if (indices.graphics_family != indices.present_family) {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      createInfo.queueFamilyIndexCount = 0;
      createInfo.pQueueFamilyIndices = nullptr;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(logical_device_, &createInfo, nullptr, &swap_chain_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create swapchain!");
  }

  vkGetSwapchainImagesKHR(logical_device_, swap_chain_, &imageCount, nullptr);
  swap_chain_images_.resize(imageCount);
  vkGetSwapchainImagesKHR(logical_device_, swap_chain_, &imageCount, swap_chain_images_.data());

  swap_chain_image_format_ = surfaceFormat.format;
  swap_chain_extent_ = extent;
}

void Engine::CreateImageViews() {
}

void Engine::MainLoop() {
  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();
    // Future: update / render
  }
}

void Engine::Cleanup() {
  if (vulkan_instance_ == VK_NULL_HANDLE && window_ == nullptr) {
    return;
  }

  IM_INFO("Cleaning up Engine...");

  if constexpr (kEnableValidationLayers) {
    if (debug_messenger_ != VK_NULL_HANDLE) {
      auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(vulkan_instance_, "vkDestroyDebugUtilsMessengerEXT"));
      if (func != nullptr) {
        func(vulkan_instance_, debug_messenger_, nullptr);
      }
      debug_messenger_ = VK_NULL_HANDLE;
    }
  }

  if (swap_chain_ != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(logical_device_, swap_chain_, nullptr);
  }

  if (logical_device_ != VK_NULL_HANDLE) {
    vkDestroyDevice(logical_device_, nullptr);
    physical_device_ = VK_NULL_HANDLE;
  }

  if (vulkan_surface_ != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(vulkan_instance_, vulkan_surface_, nullptr);
    vulkan_surface_ = VK_NULL_HANDLE;
  }

  if (vulkan_instance_ != VK_NULL_HANDLE) {
    vkDestroyInstance(vulkan_instance_, nullptr);
    vulkan_instance_ = VK_NULL_HANDLE;
  }

  if (window_) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }
  
  glfwTerminate();
}

bool Engine::CheckValidationLayerSupport() const {
  uint32_t layer_count = 0;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const char* layer_name : kValidationLayers) {
    bool found = std::ranges::any_of(available_layers, [&](const auto& props) {
      return strcmp(layer_name, props.layerName) == 0;
    });
    if (!found) {
      return false;
    }
  }
  return true;
}

bool Engine::CheckRequiredInstanceExtensionsSupport() const {
  auto required_extensions = GetRequiredInstanceExtensions();

  uint32_t count = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

  std::vector<VkExtensionProperties> available(count);
  vkEnumerateInstanceExtensionProperties(nullptr, &count, available.data());

  for (const char* required_cstr : required_extensions) {
    std::string_view required(required_cstr);

    bool found = std::ranges::any_of( available,
      [&](const VkExtensionProperties& ext) {
      return required == std::string_view(ext.extensionName);
    });

    if (!found) {
      IM_ERROR("Missing Vulkan extension: {}", required);
      return false;
    }
  }

  return true;
}

bool Engine::CheckRequiredDeviceExtensionsSupport(VkPhysicalDevice device) const {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  return std::ranges::all_of(kDeviceExtensions, [&](std::string_view required) {
      return std::ranges::any_of(availableExtensions, [&](const VkExtensionProperties& available) {
          return required == available.extensionName; 
      });
  });
}

std::vector<const char*> Engine::GetRequiredInstanceExtensions() const {
  uint32_t glfw_ext_count = 0;
  const char** glfw_extensions = 
    glfwGetRequiredInstanceExtensions(&glfw_ext_count);

  std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_ext_count);

  if (kEnableValidationLayers) {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

bool Engine::IsDeviceSuitable(const VkPhysicalDevice& device) const {
  QueueFamilyIndices indices = FindQueueFamiliesIndices(device);
  bool extensionsSupported = CheckRequiredDeviceExtensionsSupport(device);
  bool swapChainAdequate = false;

  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
    swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
  }

  IM_INFO("Physical Device selected: {}", static_cast<std::string>(deviceProperties.deviceName));

  return indices.is_complete() && extensionsSupported && swapChainAdequate;
}

QueueFamilyIndices Engine::FindQueueFamiliesIndices(VkPhysicalDevice device) const {
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        indices.graphics_family = i;
    }
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vulkan_surface_, &presentSupport);
    if (presentSupport) {
        indices.present_family = i;
    }
    if (indices.is_complete()) {
        break;
    }
    i++;
  }

  return indices;
}

SwapChainSupportDetails Engine::QuerySwapChainSupport(VkPhysicalDevice device) const {
  SwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vulkan_surface_, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan_surface_, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan_surface_, &formatCount, details.formats.data());
  }

  uint32_t presentModeCount;  
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan_surface_, &presentModeCount, nullptr);
  if (presentModeCount != 0) {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan_surface_, &presentModeCount, details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR Engine::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const {
  for(const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
      availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
      }
  }
  return availableFormats[0];
}

VkPresentModeKHR Engine::ChooseSwapPrensentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Engine::ChooseSwapExtenT(const VkSurfaceCapabilitiesKHR& capabilities) const {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    VkExtent2D currentExtent = {width_, height_};

    currentExtent.width = std::clamp(currentExtent.width,
      capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    currentExtent.height = std::clamp(currentExtent.height,
      capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

      return currentExtent;
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL Engine::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    [[maybe_unused]] void* user_data) {
  switch (message_severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      IM_TRACE("[Vulkan] {}", callback_data->pMessage);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      IM_INFO("[Vulkan] {}", callback_data->pMessage);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      IM_WARN("[Vulkan] {}", callback_data->pMessage);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    default:
      IM_ERROR("[Vulkan] {}", callback_data->pMessage);
      break;
  }
  return VK_FALSE;
}