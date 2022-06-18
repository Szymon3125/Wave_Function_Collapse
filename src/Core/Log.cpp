#include "pch.h"

#include "Core/Log.h"

std::shared_ptr<spdlog::logger> Log::_logger;

void Log::Init() {
	spdlog::set_pattern("%^[%T] %n: %v%$");

	_logger = spdlog::stdout_color_mt("logger");
	_logger->set_level(spdlog::level::trace);
}