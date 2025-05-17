// MIT License
//
// Copyright (c) 2024 FUSTER Mattias
//
// This software utilizes code from the following GitHub repositories, which are also licensed under the MIT License:
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
// Created by damma on 17/05/2025.
//
#include "Engine.h"

// Static accessor
Engine& Engine::Get()
{
	static Engine instance;
	return instance;
}

Engine::Engine()
{
	Init();
}

Engine::~Engine()
{
	Cleanup();
}

void Engine::Init()
{
	InitWindow();
	InitVulkan();
}

void Engine::InitWindow()
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Disable OpenGL context
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	Window = glfwCreateWindow(Width, Height, WindowTitle.c_str(), nullptr, nullptr);
	if (!Window)
		throw std::runtime_error("Failed to create GLFW window");
}

void Engine::InitVulkan()
{
	// Get required extensions from GLFW
	uint32_t extensionCount = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "ImEngine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "ImEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensionCount;
	createInfo.ppEnabledExtensionNames = extensions;

	if (vkCreateInstance(&createInfo, nullptr, &VulkanInstance) != VK_SUCCESS)
		{
		throw std::runtime_error("Failed to create Vulkan instance");
	}
}

void Engine::Run() {
	MainLoop();
}

void Engine::MainLoop() {
	while (!glfwWindowShouldClose(Window))
	{
		glfwPollEvents();
		// Future: update / render
	}
}

void Engine::Cleanup()
{
	if (VulkanInstance != VK_NULL_HANDLE)
		{
		vkDestroyInstance(VulkanInstance, nullptr);
	}

	if (Window)
	{
		glfwDestroyWindow(Window);
		glfwTerminate();
	}
}