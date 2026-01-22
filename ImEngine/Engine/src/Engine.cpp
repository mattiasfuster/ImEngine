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
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <limits>
#include <set>
#include <span>
#include <string_view>

namespace {
constexpr size_t kMaxShaderSize = 10485760;

std::vector<char> ReadFile(std::string_view filename) {
  std::ifstream file(std::filesystem::path(filename), std::ios::binary);

  if (!file) {
    throw std::runtime_error(std::format("Failed to open file: {}", filename));
  }

  file.seekg(0, std::ios::end);
  const auto file_size = static_cast<size_t>(file.tellg());
  file.seekg(0, std::ios::beg);

  if (file_size == 0) {
    throw std::runtime_error(std::format("File is empty: {}", filename));
  }

  if (file_size > kMaxShaderSize) {
    throw std::runtime_error(
        std::format("File too large ({} bytes, max {} bytes): {}", file_size,
                    kMaxShaderSize, filename));
  }

  std::vector<char> buffer;
  buffer.resize(file_size);

  if (!file.read(buffer.data(), static_cast<std::streamsize>(file_size))) {
    throw std::runtime_error(std::format("Failed to read file: {}", filename));
  }

  return buffer;
}
} // namespace

Engine::Engine(const EngineConfig &config)
    : window_title_(config.title), width_(config.width),
      height_(config.height) {
  InitWindow();
  InitVulkan();
}

Engine::~Engine() { Cleanup(); }

Engine::Engine(Engine &&other) noexcept
    : window_(other.window_), vulkan_instance_(other.vulkan_instance_),
      debug_messenger_(other.debug_messenger_),
      vulkan_surface_(other.vulkan_surface_),
      physical_device_(other.physical_device_),
      logical_device_(other.logical_device_),
      graphics_queue_(other.graphics_queue_),
      present_queue_(other.present_queue_), swap_chain_(other.swap_chain_),
      swap_chain_images_(std::move(other.swap_chain_images_)),
      swap_chain_image_views_(std::move(other.swap_chain_image_views_)),
      swap_chain_image_format_(other.swap_chain_image_format_),
      swap_chain_extent_(other.swap_chain_extent_),
      render_pass_(other.render_pass_),
      pipeline_layout_(other.pipeline_layout_),
      graphics_pipeline_(other.graphics_pipeline_),
      swap_chain_framebuffers_(std::move(other.swap_chain_framebuffers_)),
      command_pool_(other.command_pool_),
      command_buffers_(std::move(other.command_buffers_)),
      image_available_semaphores_(std::move(other.image_available_semaphores_)),
      render_finished_semaphores_(std::move(other.render_finished_semaphores_)),
      in_flight_fences_(std::move(other.in_flight_fences_)),
      current_frame_(other.current_frame_),
      window_title_(std::move(other.window_title_)), width_(other.width_),
      height_(other.height_) {
  other.window_ = nullptr;
  other.vulkan_instance_ = VK_NULL_HANDLE;
  other.debug_messenger_ = VK_NULL_HANDLE;
  other.vulkan_surface_ = VK_NULL_HANDLE;
  other.physical_device_ = VK_NULL_HANDLE;
  other.logical_device_ = VK_NULL_HANDLE;
  other.graphics_queue_ = VK_NULL_HANDLE;
  other.present_queue_ = VK_NULL_HANDLE;
  other.swap_chain_ = VK_NULL_HANDLE;
  other.render_pass_ = VK_NULL_HANDLE;
  other.pipeline_layout_ = VK_NULL_HANDLE;
  other.graphics_pipeline_ = VK_NULL_HANDLE;
  other.command_pool_ = VK_NULL_HANDLE;
  other.current_frame_ = 0;
}

Engine &Engine::operator=(Engine &&other) noexcept {
  if (this != &other) {
    Cleanup();

    window_ = other.window_;
    vulkan_instance_ = other.vulkan_instance_;
    debug_messenger_ = other.debug_messenger_;
    vulkan_surface_ = other.vulkan_surface_;
    physical_device_ = other.physical_device_;
    logical_device_ = other.logical_device_;
    graphics_queue_ = other.graphics_queue_;
    present_queue_ = other.present_queue_;
    swap_chain_ = other.swap_chain_;
    swap_chain_images_ = std::move(other.swap_chain_images_);
    swap_chain_image_views_ = std::move(other.swap_chain_image_views_);
    swap_chain_image_format_ = other.swap_chain_image_format_;
    swap_chain_extent_ = other.swap_chain_extent_;
    render_pass_ = other.render_pass_;
    pipeline_layout_ = other.pipeline_layout_;
    graphics_pipeline_ = other.graphics_pipeline_;
    swap_chain_framebuffers_ = std::move(other.swap_chain_framebuffers_);
    command_pool_ = other.command_pool_;
    command_buffers_ = std::move(other.command_buffers_);
    image_available_semaphores_ = std::move(other.image_available_semaphores_);
    render_finished_semaphores_ = std::move(other.render_finished_semaphores_);
    in_flight_fences_ = std::move(other.in_flight_fences_);
    current_frame_ = other.current_frame_;
    window_title_ = std::move(other.window_title_);
    width_ = other.width_;
    height_ = other.height_;

    other.window_ = nullptr;
    other.vulkan_instance_ = VK_NULL_HANDLE;
    other.debug_messenger_ = VK_NULL_HANDLE;
    other.vulkan_surface_ = VK_NULL_HANDLE;
    other.physical_device_ = VK_NULL_HANDLE;
    other.logical_device_ = VK_NULL_HANDLE;
    other.graphics_queue_ = VK_NULL_HANDLE;
    other.present_queue_ = VK_NULL_HANDLE;
    other.swap_chain_ = VK_NULL_HANDLE;
    other.render_pass_ = VK_NULL_HANDLE;
    other.pipeline_layout_ = VK_NULL_HANDLE;
    other.graphics_pipeline_ = VK_NULL_HANDLE;
    other.command_pool_ = VK_NULL_HANDLE;
    other.current_frame_ = 0;
  }
  return *this;
}

void Engine::Run() { MainLoop(); }

void Engine::InitWindow() {
  IM_INFO("Initializing window...");
  if (!glfwInit()) {
    IM_ERROR("Failed to initialize GLFW");
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window_ =
      glfwCreateWindow(static_cast<int>(width_), static_cast<int>(height_),
                       window_title_.c_str(), nullptr, nullptr);
  if (!window_) {
    IM_ERROR("Failed to create GLFW window");
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }
}

void Engine::InitVulkan() {
  IM_INFO("Initializing Vulkan...");

  if constexpr (kEnableValidationLayers) {
    if (!CheckValidationLayerSupport()) {
      throw std::runtime_error(
          "validation layers requested, but not available!");
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
  CreateImageViews();
  CreateRenderPass();
  CreateGraphicsPipeline();
  CreateFramebuffers();
  CreateCommandPool();
  CreateCommandBuffer();
  CreateSyncObjects();
}

void Engine::CreateInstance() {
  IM_INFO("Creating instance...");
  VkApplicationInfo app_info{
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pNext = nullptr,
      .pApplicationName = window_title_.c_str(),
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

  if (vkCreateInstance(&create_info, nullptr, &vulkan_instance_) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create Vulkan instance!");
  }
}

void Engine::PopulateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &create_info) {
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = DebugCallback;
  create_info.pUserData = nullptr;
}

void Engine::SetupDebugMessenger() {
  if constexpr (!kEnableValidationLayers)
    return;

  IM_INFO("Setup Vulkan Debug Messenger...");

  VkDebugUtilsMessengerCreateInfoEXT create_info;
  PopulateDebugMessengerCreateInfo(create_info);

  auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(vulkan_instance_,
                            "vkCreateDebugUtilsMessengerEXT"));

  if (func == nullptr) {
    IM_ERROR("Failed to load vkCreateDebugUtilsMessengerEXT");
    return;
  }

  if (func(vulkan_instance_, &create_info, nullptr, &debug_messenger_) !=
      VK_SUCCESS) {
    IM_ERROR("Failed to set up debug messenger");
  }
}

void Engine::CreateSurface() {
  IM_INFO("Creating surface...");
  if (glfwCreateWindowSurface(vulkan_instance_, window_, nullptr,
                              &vulkan_surface_) != VK_SUCCESS) {
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

  for (const VkPhysicalDevice &device : devices) {
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
  std::set<uint32_t> uniqueQueueFamilies = {indices.graphics_family.value(),
                                            indices.present_family.value()};

  float queuePriority = 1.0F;
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

  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(kDeviceExtensions.size());
  createInfo.ppEnabledExtensionNames = kDeviceExtensions.data();

  if (kEnableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(kValidationLayers.size());
    createInfo.ppEnabledLayerNames = kValidationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(physical_device_, &createInfo, nullptr,
                     &logical_device_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create logical device!");
  }

  vkGetDeviceQueue(logical_device_, indices.graphics_family.value(), 0,
                   &graphics_queue_);
  vkGetDeviceQueue(logical_device_, indices.present_family.value(), 0,
                   &present_queue_);
}

void Engine::CreateSwapChain() {
  IM_INFO("Creating swap chain...");

  SwapChainSupportDetails swapChainSupport =
      QuerySwapChainSupport(physical_device_);

  VkSurfaceFormatKHR surfaceFormat =
      ChooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
      ChooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = vulkan_surface_;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = FindQueueFamiliesIndices(physical_device_);
  std::array<uint32_t, 2> queueFamilyIndices = {indices.graphics_family.value(),
                                                indices.present_family.value()};
  if (indices.graphics_family != indices.present_family) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
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

  if (vkCreateSwapchainKHR(logical_device_, &createInfo, nullptr,
                           &swap_chain_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create swapchain!");
  }

  vkGetSwapchainImagesKHR(logical_device_, swap_chain_, &imageCount, nullptr);
  swap_chain_images_.resize(imageCount);
  vkGetSwapchainImagesKHR(logical_device_, swap_chain_, &imageCount,
                          swap_chain_images_.data());

  swap_chain_image_format_ = surfaceFormat.format;
  swap_chain_extent_ = extent;
}

void Engine::CreateImageViews() {
  swap_chain_image_views_.resize(swap_chain_images_.size());
  for (size_t i = 0; i < swap_chain_images_.size(); i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swap_chain_images_[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swap_chain_image_format_;
    createInfo.components = {.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                             .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                             .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                             .a = VK_COMPONENT_SWIZZLE_IDENTITY};
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(logical_device_, &createInfo, nullptr,
                          &swap_chain_image_views_[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create image views!");
    }
  }
}

void Engine::CreateRenderPass() {
  IM_INFO("Creating render pass...");

  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = swap_chain_image_format_;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;

  if (vkCreateRenderPass(logical_device_, &renderPassInfo, nullptr,
                         &render_pass_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create render pass!");
  }
}

void Engine::CreateGraphicsPipeline() {
  IM_INFO("Creating graphics pipeline...");

  auto vertShaderCode = ReadFile("shaders/shader.vert.spv");
  auto fragShaderCode = ReadFile("shaders/shader.frag.spv");

  VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
  VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName = "main";

  std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {
      vertShaderStageInfo, fragShaderStageInfo};

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = 0;
  vertexInputInfo.vertexAttributeDescriptionCount = 0;

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.scissorCount = 1;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0F;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0F;
  colorBlending.blendConstants[1] = 0.0F;
  colorBlending.blendConstants[2] = 0.0F;
  colorBlending.blendConstants[3] = 0.0F;

  std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                               VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
  dynamicState.pDynamicStates = dynamicStates.data();

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pushConstantRangeCount = 0;

  if (vkCreatePipelineLayout(logical_device_, &pipelineLayoutInfo, nullptr,
                             &pipeline_layout_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages.data();
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = &dynamicState;
  pipelineInfo.layout = pipeline_layout_;
  pipelineInfo.renderPass = render_pass_;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(logical_device_, VK_NULL_HANDLE, 1,
                                &pipelineInfo, nullptr,
                                &graphics_pipeline_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create graphics pipeline!");
  }

  vkDestroyShaderModule(logical_device_, fragShaderModule, nullptr);
  vkDestroyShaderModule(logical_device_, vertShaderModule, nullptr);
}

void Engine::CreateFramebuffers() {
  swap_chain_framebuffers_.resize(swap_chain_image_views_.size());
  for (size_t i = 0; i < swap_chain_image_views_.size(); i++) {
    std::array<VkImageView, 1> attachments = {swap_chain_image_views_[i]};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = render_pass_;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = swap_chain_extent_.width;
    framebufferInfo.height = swap_chain_extent_.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(logical_device_, &framebufferInfo, nullptr,
                            &swap_chain_framebuffers_[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
}

void Engine::CreateCommandPool() {
  QueueFamilyIndices queueFamilyIndices =
      FindQueueFamiliesIndices(physical_device_);

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphics_family.value();

  if (vkCreateCommandPool(logical_device_, &poolInfo, nullptr,
                          &command_pool_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
  }
}

void Engine::CreateCommandBuffer() {
  command_buffers_.resize(kMaxFramesInFlight);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = command_pool_;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = static_cast<uint32_t>(command_buffers_.size());

  if (vkAllocateCommandBuffers(logical_device_, &allocInfo,
                                command_buffers_.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void Engine::CreateSyncObjects() {
  image_available_semaphores_.resize(kMaxFramesInFlight);
  render_finished_semaphores_.resize(kMaxFramesInFlight);
  in_flight_fences_.resize(kMaxFramesInFlight);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < kMaxFramesInFlight; i++) {
    if (vkCreateSemaphore(logical_device_, &semaphoreInfo, nullptr,
                          &image_available_semaphores_[i]) != VK_SUCCESS ||
        vkCreateSemaphore(logical_device_, &semaphoreInfo, nullptr,
                          &render_finished_semaphores_[i]) != VK_SUCCESS ||
        vkCreateFence(logical_device_, &fenceInfo, nullptr,
                      &in_flight_fences_[i]) != VK_SUCCESS) {
      throw std::runtime_error(
          "failed to create synchronization objects for a frame!");
    }
  }
}

void Engine::MainLoop() {
  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();
    DrawFrame();
  }
  vkDeviceWaitIdle(logical_device_);
}

void Engine::DrawFrame() {
  vkWaitForFences(logical_device_, 1, &in_flight_fences_[current_frame_],
                  VK_TRUE, UINT64_MAX);
  vkResetFences(logical_device_, 1, &in_flight_fences_[current_frame_]);

  uint32_t imageIndex = 0;
  vkAcquireNextImageKHR(logical_device_, swap_chain_, UINT64_MAX,
                        image_available_semaphores_[current_frame_],
                        VK_NULL_HANDLE, &imageIndex);

  vkResetCommandBuffer(command_buffers_[current_frame_],
                       /*VkCommandBufferResetFlagBits*/ 0);
  RecordCommandBuffer(command_buffers_[current_frame_], imageIndex);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  std::array<VkSemaphore, 1> waitSemaphores = {
      image_available_semaphores_[current_frame_]};
  std::array<VkPipelineStageFlags, 1> waitStages = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores.data();
  submitInfo.pWaitDstStageMask = waitStages.data();

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &command_buffers_[current_frame_];

  std::array<VkSemaphore, 1> signalSemaphores = {
      render_finished_semaphores_[current_frame_]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores.data();

  if (vkQueueSubmit(graphics_queue_, 1, &submitInfo,
                    in_flight_fences_[current_frame_]) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores.data();

  std::array<VkSwapchainKHR, 1> swapChains = {swap_chain_};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains.data();

  presentInfo.pImageIndices = &imageIndex;

  vkQueuePresentKHR(present_queue_, &presentInfo);

  current_frame_ = (current_frame_ + 1) % kMaxFramesInFlight;
}

void Engine::Cleanup() {
  if (vulkan_instance_ == VK_NULL_HANDLE && window_ == nullptr) {
    return;
  }

  IM_INFO("Cleaning up Engine...");

  if (logical_device_ != VK_NULL_HANDLE) {
    vkDeviceWaitIdle(logical_device_);
  }

  if constexpr (kEnableValidationLayers) {
    if (debug_messenger_ != VK_NULL_HANDLE) {
      auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(vulkan_instance_,
                                "vkDestroyDebugUtilsMessengerEXT"));
      if (func != nullptr) {
        func(vulkan_instance_, debug_messenger_, nullptr);
      }
      debug_messenger_ = VK_NULL_HANDLE;
    }
  }

  for (size_t i = 0; i < kMaxFramesInFlight; i++) {
    vkDestroySemaphore(logical_device_, image_available_semaphores_[i],
                       nullptr);
    vkDestroySemaphore(logical_device_, render_finished_semaphores_[i],
                       nullptr);
    vkDestroyFence(logical_device_, in_flight_fences_[i], nullptr);
  }

  if (command_pool_ != VK_NULL_HANDLE) {
    vkDestroyCommandPool(logical_device_, command_pool_, nullptr);
    command_pool_ = VK_NULL_HANDLE;
  }

  for (auto *framebuffer : swap_chain_framebuffers_) {
    vkDestroyFramebuffer(logical_device_, framebuffer, nullptr);
  }

  if (graphics_pipeline_ != VK_NULL_HANDLE) {
    vkDestroyPipeline(logical_device_, graphics_pipeline_, nullptr);
    graphics_pipeline_ = VK_NULL_HANDLE;
  }

  if (pipeline_layout_ != VK_NULL_HANDLE) {
    vkDestroyPipelineLayout(logical_device_, pipeline_layout_, nullptr);
    pipeline_layout_ = VK_NULL_HANDLE;
  }

  if (render_pass_ != VK_NULL_HANDLE) {
    vkDestroyRenderPass(logical_device_, render_pass_, nullptr);
    render_pass_ = VK_NULL_HANDLE;
  }

  for (const auto &imageView : swap_chain_image_views_) {
    vkDestroyImageView(logical_device_, imageView, nullptr);
  }

  if (swap_chain_ != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(logical_device_, swap_chain_, nullptr);
    swap_chain_ = VK_NULL_HANDLE;
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

void Engine::RecordCommandBuffer(VkCommandBuffer commandBuffer,
                                 uint32_t imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = render_pass_;
  renderPassInfo.framebuffer = swap_chain_framebuffers_[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swap_chain_extent_;

  VkClearValue clearColor = {{{0.0F, 0.0F, 0.0F, 1.0F}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphics_pipeline_);

  VkViewport viewport{};
  viewport.x = 0.0F;
  viewport.y = 0.0F;
  viewport.width = (float)swap_chain_extent_.width;
  viewport.height = (float)swap_chain_extent_.height;
  viewport.minDepth = 0.0F;
  viewport.maxDepth = 1.0F;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swap_chain_extent_;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  vkCmdDraw(commandBuffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}

bool Engine::CheckValidationLayerSupport() const {
  uint32_t layer_count = 0;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (std::string_view layer_name : kValidationLayers) {
    bool found = std::ranges::any_of(available_layers, [&](const auto &props) {
      return layer_name ==
             std::string_view(static_cast<const char *>(props.layerName));
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

  for (const char *required_cstr : required_extensions) {
    std::string_view required(required_cstr);

    bool found =
        std::ranges::any_of(available, [&](const VkExtensionProperties &ext) {
          return required ==
                 std::string_view(static_cast<const char *>(ext.extensionName));
        });

    if (!found) {
      IM_ERROR("Missing Vulkan extension: {}", required);
      return false;
    }
  }

  return true;
}

bool Engine::CheckRequiredDeviceExtensionsSupport(
    VkPhysicalDevice device) const {
  uint32_t extensionCount = {};
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  return std::ranges::all_of(kDeviceExtensions, [&](std::string_view required) {
    return std::ranges::any_of(
        availableExtensions, [&](const VkExtensionProperties &available) {
          return required == std::string_view(static_cast<const char *>(
                                 available.extensionName));
        });
  });
}

std::vector<const char *> Engine::GetRequiredInstanceExtensions() const {
  uint32_t glfw_ext_count = 0;
  const char **glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_ext_count);

  std::span<const char *const> glfw_ext_span(glfw_extensions, glfw_ext_count);
  std::vector<const char *> extensions(glfw_ext_span.begin(),
                                       glfw_ext_span.end());

  if (kEnableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

bool Engine::IsDeviceSuitable(const VkPhysicalDevice &device) const {
  QueueFamilyIndices indices = FindQueueFamiliesIndices(device);
  bool extensionsSupported = CheckRequiredDeviceExtensionsSupport(device);
  bool swapChainAdequate = false;

  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  IM_INFO(
      "Physical Device selected: {}",
      std::string_view(static_cast<const char *>(deviceProperties.deviceName)));

  return indices.is_complete() && extensionsSupported && swapChainAdequate;
}

QueueFamilyIndices
Engine::FindQueueFamiliesIndices(VkPhysicalDevice device) const {
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  uint32_t queue_index = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphics_family = queue_index;
    }
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, queue_index, vulkan_surface_,
                                         &presentSupport);
    if (presentSupport) {
      indices.present_family = queue_index;
    }
    if (indices.is_complete()) {
      break;
    }
    queue_index++;
  }

  return indices;
}

SwapChainSupportDetails
Engine::QuerySwapChainSupport(VkPhysicalDevice device) const {
  SwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vulkan_surface_,
                                            &details.capabilities);

  uint32_t formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan_surface_, &formatCount,
                                       nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan_surface_, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan_surface_,
                                            &presentModeCount, nullptr);
  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan_surface_,
                                              &presentModeCount,
                                              details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR Engine::ChooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats) const {
  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }
  return availableFormats[0];
}

VkPresentModeKHR Engine::ChooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) const {
  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D
Engine::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }

  VkExtent2D currentExtent = {width_, height_};
  currentExtent.width =
      std::clamp(currentExtent.width, capabilities.minImageExtent.width,
                 capabilities.maxImageExtent.width);
  currentExtent.height =
      std::clamp(currentExtent.height, capabilities.minImageExtent.height,
                 capabilities.maxImageExtent.height);
  return currentExtent;
}

[[nodiscard]] VkShaderModule
Engine::CreateShaderModule(const std::vector<char> &code) const {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
  VkShaderModule shaderModule = {};
  if (vkCreateShaderModule(logical_device_, &createInfo, nullptr,
                           &shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module!");
  }
  return shaderModule;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Engine::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    [[maybe_unused]] void *user_data) {
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
