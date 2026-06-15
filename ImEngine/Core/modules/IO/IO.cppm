export module ImEngine.Core.IO;

export import :Logger;
export import :Sink;
export import :Format;

export namespace ime::log {

template <typename... Args>
void Log(
  const core::Level _level,
  core::FormatWithSrcLocation<std::type_identity_t<Args>...> _fmt,
  Args&&... _args)
{
  if (_level < core::Logger::GetLevel()) return;
  core::Logger::Dispatch(_level,
    _fmt.BuildFormatStr(std::forward<Args>(_args)...));
}

template <typename... Args>
void Info(core::FormatWithSrcLocation<std::type_identity_t<Args>...> _fmt, Args&&... _args)
{
  if (core::Level::Info < core::Logger::GetLevel()) return;
  core::Logger::Dispatch(core::Level::Info,
    _fmt.BuildFormatStr(std::forward<Args>(_args)...));
}

template <typename... Args>
void Warn(core::FormatWithSrcLocation<std::type_identity_t<Args>...> _fmt, Args&&... _args)
{
  if (core::Level::Warn < core::Logger::GetLevel()) return;
  core::Logger::Dispatch(core::Level::Warn,
    _fmt.BuildFormatStr(std::forward<Args>(_args)...));
}

template <typename... Args>
void Error(core::FormatWithSrcLocation<std::type_identity_t<Args>...> _fmt, Args&&... _args)
{
  if (core::Level::Error < core::Logger::GetLevel()) return;
  core::Logger::Dispatch(core::Level::Error,
    _fmt.BuildFormatStr(std::forward<Args>(_args)...));
}

}