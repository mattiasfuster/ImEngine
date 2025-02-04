// MIT License
//
// Copyright (c) 2024 FUSTER Mattias
//
// This software utilizes code from the following GitHub repositories, which are also licensed under
// the MIT License:
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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//
// Created by m.fuster on 04/02/2025.
//

#include "VulkanContext.h"

namespace ImEngine_Core
{
	VulkanContext::VulkanContext()
	{
	}

	VulkanContext::~VulkanContext()
	{
	}

	VkAllocationCallbacks* VulkanContext::GetAllocator() const
	{
		std::lock_guard lock(m_Mutex);
		return m_Allocator;
	}

	VkInstance VulkanContext::GetInstance() const
	{
		std::lock_guard lock(m_Mutex);
		return m_Instance;
	}

	VkPhysicalDevice VulkanContext::GetPhysicalDevice() const
	{
		std::lock_guard lock(m_Mutex);
		return m_PhysicalDevice;
	}

	VkDevice VulkanContext::GetDevice() const
	{
		std::lock_guard lock(m_Mutex);
		return m_Device;
	}

	uint32_t VulkanContext::GetQueueFamily() const
	{
		std::lock_guard lock(m_Mutex);
		return m_QueueFamily;
	}

	VkQueue VulkanContext::GetQueue() const
	{
		std::lock_guard lock(m_Mutex);
		return m_Queue;
	}

	VkDebugReportCallbackEXT VulkanContext::GetDebugReport() const
	{
		std::lock_guard lock(m_Mutex);
		return m_DebugReport;
	}

	VkPipelineCache VulkanContext::GetPipelineCache() const
	{
		std::lock_guard lock(m_Mutex);
		return m_PipelineCache;
	}

	VkDescriptorPool VulkanContext::GetDescriptorPool() const
	{
		std::lock_guard lock(m_Mutex);
		return m_DescriptorPool;
	}

	ImGui_ImplVulkanH_Window VulkanContext::GetMainWindowData() const
	{
		std::lock_guard lock(m_Mutex);
		return m_MainWindowData;
	}

	uint32_t VulkanContext::GetMinImageCount() const
	{
		std::lock_guard lock(m_Mutex);
		return m_MinImageCount;
	}

	bool VulkanContext::GetSwapChainRebuild() const
	{
		std::lock_guard lock(m_Mutex);
		return m_SwapChainRebuild;
	}

	void VulkanContext::SetAllocator(VkAllocationCallbacks* allocator)
	{
		std::lock_guard lock(m_Mutex);
		m_Allocator = const_cast<VkAllocationCallbacks*>(allocator);
	}

	void VulkanContext::SetInstance(VkInstance instance)
	{
		std::lock_guard lock(m_Mutex);
		m_Instance = instance;
	}

	void VulkanContext::SetPhysicalDevice(VkPhysicalDevice physicalDevice)
	{
		std::lock_guard lock(m_Mutex);
		m_PhysicalDevice = physicalDevice;
	}

	void VulkanContext::SetDevice(VkDevice device)
	{
		std::lock_guard lock(m_Mutex);
		m_Device = device;
	}

	void VulkanContext::SetQueueFamily(uint32_t queueFamily)
	{
		std::lock_guard lock(m_Mutex);
		m_QueueFamily = queueFamily;
	}

	void VulkanContext::SetQueue(VkQueue queue)
	{
		std::lock_guard lock(m_Mutex);
		m_Queue = queue;
	}

	void VulkanContext::SetDebugReport(VkDebugReportCallbackEXT debugReport)
	{
		std::lock_guard lock(m_Mutex);
		m_DebugReport = debugReport;
	}

	void VulkanContext::SetPipelineCache(VkPipelineCache pipelineCache)
	{
		std::lock_guard lock(m_Mutex);
		m_PipelineCache = pipelineCache;
	}

	void VulkanContext::SetDescriptorPool(VkDescriptorPool descriptorPool)
	{
		std::lock_guard lock(m_Mutex);
		m_DescriptorPool = descriptorPool;
	}

	void VulkanContext::SetMainWindowData(const ImGui_ImplVulkanH_Window& mainWindowData)
	{
		std::lock_guard lock(m_Mutex);
		m_MainWindowData = mainWindowData;
	}

	void VulkanContext::SetMinImageCount(uint32_t minImageCount)
	{
		std::lock_guard lock(m_Mutex);
		m_MinImageCount = minImageCount;
	}

	void VulkanContext::SetSwapChainRebuild(bool swapChainRebuild)
	{
		std::lock_guard lock(m_Mutex);
		m_SwapChainRebuild = swapChainRebuild;
	}
	void VulkanContext::SetupVulkan(ImVector<const char*> instance_extensions)
	{
		VkResult err;
		#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
		    volkInitialize();
		#endif

		    // Create Vulkan Instance
		    {
		        VkInstanceCreateInfo create_info = {};
		        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		        // Enumerate available extensions
		        uint32_t properties_count;
		        ImVector<VkExtensionProperties> properties;
		        vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
		        properties.resize(static_cast<int>(properties_count));
		        err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.Data);
		        CheckVkResult(err);

		        // Enable required extensions
		        if (IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
		            instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
		        if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
		        {
		            instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
		            create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
		        }
		#endif

		        // Enabling validation layers
		#ifdef APP_USE_VULKAN_DEBUG_REPORT
		        const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
		        create_info.enabledLayerCount = 1;
		        create_info.ppEnabledLayerNames = layers;
		        instance_extensions.push_back("VK_EXT_debug_report");
		#endif

		        // Create Vulkan Instance
		        create_info.enabledExtensionCount = static_cast<uint32_t>(instance_extensions.Size);
		        create_info.ppEnabledExtensionNames = instance_extensions.Data;
		        err = vkCreateInstance(&create_info, m_Allocator, &m_Instance);
		        CheckVkResult(err);
		#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
		        volkLoadInstance(g_Instance);
		#endif

		        // Set up the debug report callback
		#ifdef APP_USE_VULKAN_DEBUG_REPORT
		        auto f_vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
		        IM_ASSERT(f_vkCreateDebugReportCallbackEXT != nullptr);
		        VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
		        debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		        debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		        debug_report_ci.pfnCallback = debug_report;
		        debug_report_ci.pUserData = nullptr;
		        err = f_vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
		        check_vk_result(err);
		#endif
		    }

		    // Select Physical Device (GPU)
		    m_PhysicalDevice = ImGui_ImplVulkanH_SelectPhysicalDevice(m_Instance);
		    IM_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE);

		    // Select graphics queue family
		    m_QueueFamily = ImGui_ImplVulkanH_SelectQueueFamilyIndex(m_PhysicalDevice);
		    IM_ASSERT(m_QueueFamily != static_cast<uint32_t>(-1));

		    // Create Logical Device (with 1 queue)
		    {
		        ImVector<const char*> device_extensions;
		        device_extensions.push_back("VK_KHR_swapchain");

		        // Enumerate physical device extension
		        uint32_t properties_count;
		        ImVector<VkExtensionProperties> properties;
		        vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &properties_count, nullptr);
		        properties.resize(static_cast<int>(properties_count));
		        vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &properties_count, properties.Data);
		#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
		        if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
		            device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
		#endif

			    constexpr float queue_priority[] = { 1.0f };
		        VkDeviceQueueCreateInfo queue_info[1] = {};
		        queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		        queue_info[0].queueFamilyIndex = m_QueueFamily;
		        queue_info[0].queueCount = 1;
		        queue_info[0].pQueuePriorities = queue_priority;
		        VkDeviceCreateInfo create_info = {};
		        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		        create_info.queueCreateInfoCount = std::size(queue_info);
		        create_info.pQueueCreateInfos = queue_info;
		        create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.Size);
		        create_info.ppEnabledExtensionNames = device_extensions.Data;
		        err = vkCreateDevice(m_PhysicalDevice, &create_info, m_Allocator, &m_Device);
		        CheckVkResult(err);
		        vkGetDeviceQueue(m_Device, m_QueueFamily, 0, &m_Queue);
		    }

		    // Create Descriptor Pool
		    // The example only requires a single combined image sampler descriptor for the font image and only uses one descriptor set (for that)
		    // If you wish to load e.g. additional textures you may need to alter pools sizes.
		    {
		        VkDescriptorPoolSize pool_sizes[] =
		        {
		            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
		        };
		        VkDescriptorPoolCreateInfo pool_info = {};
		        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		        pool_info.maxSets = 1;
		        pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(pool_sizes));
		        pool_info.pPoolSizes = pool_sizes;
		        err = vkCreateDescriptorPool(m_Device, &pool_info, m_Allocator, &m_DescriptorPool);
		        CheckVkResult(err);
		    }
	}

	void VulkanContext::CheckVkResult(VkResult err)
	{
		if (err == 0)
			return;
		fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
		if (err < 0)
			abort();
	}

} // namespace ImEngine_Core