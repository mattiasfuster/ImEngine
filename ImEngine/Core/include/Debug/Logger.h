#ifndef IMENGINE_CORE_DEBUG_LOGGER_H
#define IMENGINE_CORE_DEBUG_LOGGER_H

#include <print>
#include <string_view>
#include <cstdio>
#include <format>
#include <chrono>
#include <thread>


namespace ImEngine::Core::Debug {
namespace Colors {
inline constexpr std::string_view kReset  = "\033[0m";
inline constexpr std::string_view kRed    = "\033[31m";
inline constexpr std::string_view kGreen  = "\033[32m";
inline constexpr std::string_view kYellow = "\033[33m";
inline constexpr std::string_view kBlue   = "\033[34m";
}  // namespace Colors

template <typename... Args>
void LogInfo(std::format_string<Args...> fmt, Args&&... args) {
  auto now = std::chrono::floor<std::chrono::microseconds>(
      std::chrono::system_clock::now());
  std::print(stdout, "[{:%H:%M:%S}] [TID:{}] {}[INFO] ", now,
             std::this_thread::get_id(), Colors::kGreen);
  std::vprint_unicode(stdout, fmt.get(), std::make_format_args(args...));
  std::println(stdout, "{}", Colors::kReset);
}

template <typename... Args>
void LogWarning(std::format_string<Args...> fmt, Args&&... args) {
  auto now = std::chrono::floor<std::chrono::microseconds>(
      std::chrono::system_clock::now());
  std::print(stdout, "[{:%H:%M:%S}] [TID:{}] {}[WARN] ", now,
             std::this_thread::get_id(), Colors::kYellow);
  std::vprint_unicode(stdout, fmt.get(), std::make_format_args(args...));
  std::println(stdout, "{}", Colors::kReset);
}

template <typename... Args>
void LogError(std::format_string<Args...> fmt, Args&&... args) {
  auto now = std::chrono::floor<std::chrono::microseconds>(
      std::chrono::system_clock::now());
  std::print(stderr, "[{:%H:%M:%S}] [TID:{}] {}[ERROR] ", now,
             std::this_thread::get_id(), Colors::kRed);
  std::vprint_unicode(stderr, fmt.get(), std::make_format_args(args...));
  std::println(stderr, "{}", Colors::kReset);
}

template <typename... Args>
void LogTrace([[maybe_unused]] std::format_string<Args...> fmt,
              [[maybe_unused]] Args&&... args) {
#ifndef NDEBUG
  auto now = std::chrono::floor<std::chrono::microseconds>(
      std::chrono::system_clock::now());
  std::print(stdout, "[{:%H:%M:%S}] [TID:{}] {}[TRACE] ", now,
             std::this_thread::get_id(), Colors::kBlue);
  std::vprint_unicode(stdout, fmt.get(), std::make_format_args(args...));
  std::println(stdout, "{}", Colors::kReset);
#endif
}
}

#define IM_INFO(...)  ::ImEngine::Core::Debug::LogInfo(__VA_ARGS__)
#define IM_WARN(...)  ::ImEngine::Core::Debug::LogWarning(__VA_ARGS__)
#define IM_ERROR(...) ::ImEngine::Core::Debug::LogError(__VA_ARGS__)
#define IM_TRACE(...) ::ImEngine::Core::Debug::LogTrace(__VA_ARGS__)

#endif // IMENGINE_CORE_DEBUG_LOGGER_H