/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "common.h"

namespace tlog::style {

inline constexpr const char* RESET_COLOR = "\033[0m";

namespace text {
inline constexpr const char* BLACK = "\033[30m";
inline constexpr const char* RED = "\033[31m";
inline constexpr const char* GREEN = "\033[32m";
inline constexpr const char* YELLOW = "\033[33m";
inline constexpr const char* BLUE = "\033[34m";
inline constexpr const char* PURPLE = "\033[35m";
inline constexpr const char* CYAN = "\033[36m";
inline constexpr const char* WHITE = "\033[37m";

inline constexpr const char* LIGHT_BLACK = "\033[90m";
inline constexpr const char* LIGHT_RED = "\033[91m";
inline constexpr const char* LIGHT_GREEN = "\033[92m";
inline constexpr const char* LIGHT_YELLOW = "\033[93m";
inline constexpr const char* LIGHT_BLUE = "\033[94m";
inline constexpr const char* LIGHT_PURPLE = "\033[95m";
inline constexpr const char* LIGHT_CYAN = "\033[96m";
inline constexpr const char* LIGHT_WHITE = "\033[97m";
}

namespace background {
inline constexpr const char* BLACK = "\033[40m";
inline constexpr const char* RED = "\033[41m";
inline constexpr const char* GREEN = "\033[42m";
inline constexpr const char* YELLOW = "\033[43m";
inline constexpr const char* BLUE = "\033[44m";
inline constexpr const char* PURPLE = "\033[45m";
inline constexpr const char* CYAN = "\033[46m";
inline constexpr const char* WHITE = "\033[47m";

inline constexpr const char* LIGHT_BLACK = "\033[100m";
inline constexpr const char* LIGHT_RED = "\033[101m";
inline constexpr const char* LIGHT_GREEN = "\033[102m";
inline constexpr const char* LIGHT_YELLOW = "\033[103m";
inline constexpr const char* LIGHT_BLUE = "\033[104m";
inline constexpr const char* LIGHT_PURPLE = "\033[105m";
inline constexpr const char* LIGHT_CYAN = "\033[106m";
inline constexpr const char* LIGHT_WHITE = "\033[107m";
}

inline constexpr const char* get_color_for_background(const char* bg_color) {
  if (bg_color == background::WHITE ||
    bg_color == background::LIGHT_WHITE ||
    bg_color == background::YELLOW ||
    bg_color == background::LIGHT_YELLOW)
    return text::BLACK;

  return text::LIGHT_WHITE;
}

// Получение цвета текста по уровню
inline constexpr const char* get_color(int level) {
  switch (level) {
    case TLOG_RAW: return style::RESET_COLOR;
    case TLOG_DEBUG: return style::text::LIGHT_WHITE;
    case TLOG_INFO: return style::text::BLUE;
    case TLOG_WARNING: return style::text::YELLOW;
    case TLOG_ERROR: return style::text::RED;
    case TLOG_CRITICAL: return style::text::LIGHT_RED;
    case TLOG_SUCCESS: return style::text::GREEN;
    default: return style::RESET_COLOR;
  }
}

// Получение цвета фона по уровню
inline constexpr const char* get_background_color(int level) {
  switch (level) {
    case TLOG_RAW: return style::RESET_COLOR;
    case TLOG_DEBUG: return style::background::LIGHT_WHITE;
    case TLOG_INFO: return style::background::BLUE;
    case TLOG_WARNING: return style::background::YELLOW;
    case TLOG_ERROR: return style::background::RED;
    case TLOG_CRITICAL: return style::background::LIGHT_RED;
    case TLOG_SUCCESS: return style::background::GREEN;
    default: return style::RESET_COLOR;
  }
}

// Преобразование уровня в строку
const char* level_to_string(int level) {
  switch (level) {
    case TLOG_RAW: return "";
    case TLOG_DEBUG: return "DEBUG";
    case TLOG_INFO: return "INFO";
    case TLOG_WARNING: return "WARNING";
    case TLOG_ERROR: return "ERROR";
    case TLOG_CRITICAL: return "CRITICAL";
    case TLOG_SUCCESS: return "SUCCESS";
    default: return "UNKNOWN";
  }
}

} // namespace tlog::style

