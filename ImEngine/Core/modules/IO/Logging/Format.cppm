export module ImEngine.Core.IO:Format;
import std;

export namespace ime::core {

template<typename... Args>
struct FormatWithSrcLocation {
  std::format_string<Args...> format_str;
  std::source_location location;

  template<typename T>
  consteval FormatWithSrcLocation(
    const T& _format_str,
    const std::source_location _location = std::source_location::current()
  ) : format_str(_format_str), location(_location) {}

  std::string BuildFormatStr(Args&&... _args) {
    return std::format("[{}:{}] ", location.file_name(), location.line()) +
      std::format(format_str, std::forward<Args>(_args)...);
  };
};

}

