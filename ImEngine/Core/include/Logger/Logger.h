// MIT License
//
// Copyright (c) 2024 FUSTER Mattias
//
// This software utilizes code from the following GitHub repositories,
// which are also licensed under the MIT License:
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

#ifndef IMENGINE_LOGGER_H
#define IMENGINE_LOGGER_H

#include <format>
#include <iostream>
#include <ostream>

namespace ime::core {
  class Logger {
  public:
    template<typename... Args>
    static void Info(std::format_string<Args...> message, Args&&... args) {
      std::cerr << std::format(message, std::forward<Args>(args)...) << std::endl;
    };
    
    template<typename... Args>
    static void Warn(Args&&... args);
    
    template<typename... Args>
    static void Error(Args&&... args);
  };
}  // namespace ime::core

#define IME_INFO(...)  ime::core::Logger::Info(__VA_ARGS__)
#define IME_WARN(...)  ime::core::Logger::Warn(__VA_ARGS__)
#define IME_ERROR(...) ime::core::Logger::Error(__VA_ARGS__)

#endif  // IMENGINE_LOGGER_H
