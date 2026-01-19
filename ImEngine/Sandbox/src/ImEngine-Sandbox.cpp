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

#include "Engine.h"
#include "Debug/Logger.h"

constexpr int kSANDBOX_WINDOW_WIDTH = 1280;
constexpr int kSANDBOX_WINDOW_HEIGHT = 720;

int main([[maybe_unused]] int argk, [[maybe_unused]] char* argv[]) {
  try {
    IM_INFO("Starting ImEngine Sandbox...");


    EngineConfig config{
        .title = "ImEngine Sandbox",
        .width = kSANDBOX_WINDOW_WIDTH,
        .height = kSANDBOX_WINDOW_HEIGHT,
    };

    Engine engine(config);
    engine.Run();

    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    IM_ERROR("Fatal error: {}", e.what());
    return EXIT_FAILURE;
  }
}