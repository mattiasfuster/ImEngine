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

#pragma once
#include "imgui_impl_vulkan.h"

namespace ImEngine_Core
{

	class IMENGINE_CORE_API VulkanContext
	{
	public:
		explicit VulkanContext();
		~VulkanContext();

		// Getters
		VkAllocationCallbacks*   GetAllocator() const;
		VkInstance               GetInstance() const;
		VkPhysicalDevice         GetPhysicalDevice() const;
		VkDevice                 GetDevice() const;
		uint32_t                 GetQueueFamily() const;
		VkQueue                  GetQueue() const;
		VkDebugReportCallbackEXT GetDebugReport() const;
		VkPipelineCache          GetPipelineCache() const;
		VkDescriptorPool         GetDescriptorPool() const;
		ImGui_ImplVulkanH_Window GetMainWindowData() const;
		uint32_t                 GetMinImageCount() const;
		bool                     GetSwapChainRebuild() const;

		// Setters
		void        SetAllocator(VkAllocationCallbacks* allocator);
		void        SetInstance(VkInstance instance);
		void        SetPhysicalDevice(VkPhysicalDevice physicalDevice);
		void        SetDevice(VkDevice device);
		void        SetQueueFamily(uint32_t queueFamily);
		void        SetQueue(VkQueue queue);
		void        SetDebugReport(VkDebugReportCallbackEXT debugReport);
		void        SetPipelineCache(VkPipelineCache pipelineCache);
		void        SetDescriptorPool(VkDescriptorPool descriptorPool);
		void        SetMainWindowData(const ImGui_ImplVulkanH_Window& mainWindowData);
		void        SetMinImageCount(uint32_t minImageCount);
		void        SetSwapChainRebuild(bool swapChainRebuild);
		void        SetupVulkan(ImVector<const char*> instance_extensions);

		static void CheckVkResult(VkResult err);
		static bool IsExtensionAvailable(
		    const ImVector<VkExtensionProperties>& properties, const char* extension)
		{
			for (int i = 0; i < properties.Size; i++)
				if (strcmp(properties[i].extensionName, extension) == 0)
					return true;
			return false;
		}

	protected:
	private:
		// Ensure getter-setter thread safe
		mutable std::mutex m_Mutex;

		// VK & ImGui data
		VkAllocationCallbacks*   m_Allocator = nullptr;
		VkInstance               m_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice         m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice                 m_Device = VK_NULL_HANDLE;
		uint32_t                 m_QueueFamily = static_cast<uint32_t>(-1);
		VkQueue                  m_Queue = VK_NULL_HANDLE;
		VkDebugReportCallbackEXT m_DebugReport = VK_NULL_HANDLE;
		VkPipelineCache          m_PipelineCache = VK_NULL_HANDLE;
		VkDescriptorPool         m_DescriptorPool = VK_NULL_HANDLE;
		ImGui_ImplVulkanH_Window m_MainWindowData;
		uint32_t                 m_MinImageCount = 2;
		bool                     m_SwapChainRebuild = false;
	};
} // namespace ImEngine_Core
