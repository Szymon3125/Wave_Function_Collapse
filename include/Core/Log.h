#pragma once

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma warning(pop)

class Log {

public:
	static void Init();
	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return _logger; }

private:
	static std::shared_ptr<spdlog::logger> _logger;

};

#define LOG_TRACE(...)        ::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)         ::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)         ::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)        ::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)        ::Log::GetLogger()->critical(__VA_ARGS__)

#if ENABLE_ASSERT
#define ASSERT(x, ...) { if (!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define ASSERT(x, ...)
#endif