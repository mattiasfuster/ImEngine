export module ImEngine.Core.IO;

export import :Logger;
export import :Sink;

export namespace ime::log {

template<typename... Args>
struct FormatWithLoc {
  std::format_string<Args...> format_str;
  std::source_location location;

  template<typename T>
  consteval FormatWithLoc(
    const T& _format_str,
    std::source_location _location = std::source_location::current()
  ) : format_str(_format_str), location(_location) {}
};

template <typename... Args>
void Info(
  FormatWithLoc<std::type_identity_t<Args>...> _fmt,
  Args&&... _args) {
  std::cout
    << std::format("[{}:{}] ", _fmt.location.file_name(), _fmt.location.line())
    << std::format(_fmt.format_str, std::forward<Args>(_args)...)
    << std::endl;
}

}