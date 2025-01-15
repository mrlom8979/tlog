/*
 * Copyright (C) Yakiv Matiash
 */

#include "pch.h"
#include "common.h"
#include "style.h"

namespace tlog::formatter {

const char* get_current_time(const char* t) {
  static thread_local char buffer[64]; // Статический буфер для многопоточности
  std::time_t now = std::time(nullptr);
  std::strftime(buffer, sizeof(buffer), t, std::localtime(&now));
  return buffer;
}

char* format(const char* pattern, const char* name, int lvl, const char* msg) {
  size_t buffer_size = 1024;
  char* formatted_msg = (char*)malloc(buffer_size);
  if (!formatted_msg) {
    fprintf(stderr, "Failed to allocate memory for log message.\n");
    return NULL;
  }

  size_t pos = 0;
  for (const char* p = pattern; *p != '\0'; ++p) {
    if (pos >= buffer_size - 1) {
      buffer_size *= 2;
      char* new_buffer = (char*)realloc(formatted_msg, buffer_size);
      if (!new_buffer) {
        fprintf(stderr, "Failed to reallocate memory for log message.\n");
        free(formatted_msg);
        return NULL;
      }
      formatted_msg = new_buffer;
    }

    if (strncmp(p, "<t>", 3) == 0) { // Тег текста
      pos += snprintf(
        formatted_msg + pos,
        buffer_size - pos,
        "%s",
        style::get_color(lvl)
      );
      p += 2; // Пропустить "[T]"
    } else if (strncmp(p, "</t>", 4) == 0) { // Закрывающий тег текста
      pos += snprintf(
        formatted_msg + pos,
        buffer_size - pos,
        "%s",
        style::RESET_COLOR
      );
      p += 3; // Пропустить "[/T]"
    } else if (strncmp(p, "<b>", 3) == 0) { // Тег фона
      const char* bg_color = style::get_background_color(lvl);
      const char* txt_color = style::get_color_for_background(bg_color);
      pos += snprintf(
        formatted_msg + pos,
        buffer_size - pos,
        "%s%s",
        bg_color,
        txt_color
      );
      p += 2; // Пропустить "[B]"
    } else if (strncmp(p, "</b>", 4) == 0) { // Закрывающий тег фона
      pos += snprintf(
        formatted_msg + pos,
        buffer_size - pos,
        "%s",
        style::RESET_COLOR
      );
      p += 3; // Пропустить "[/B]"
    } else if (strncmp(p, "<r>", 3) == 0) { // Тег сброса
      pos += snprintf(
        formatted_msg + pos,
        buffer_size - pos,
        "%s",
        style::RESET_COLOR
      );
      p += 2; // Пропустить "[R]"
    } else if (*p == 'H') { // Время
      pos += snprintf(formatted_msg + pos, buffer_size - pos, "%s", get_current_time("%H"));
    } else if (*p == 'M') {
      pos += snprintf(formatted_msg + pos, buffer_size - pos, "%s", get_current_time("%M"));
    } else if (*p == 'S') {
      pos += snprintf(formatted_msg + pos, buffer_size - pos, "%s", get_current_time("%S"));
    } else if (*p == 'Y') { // Дата
      pos += snprintf(formatted_msg + pos, buffer_size - pos, "%s", get_current_time("%Y"));
    } else if (*p == 'm') { // Дата
      pos += snprintf(formatted_msg + pos, buffer_size - pos, "%s", get_current_time("%m"));
    } else if (*p == 'd') { // Дата
      pos += snprintf(formatted_msg + pos, buffer_size - pos, "%s", get_current_time("%d"));
    } else if (*p == 'N') { // Префикс
      pos += snprintf(formatted_msg + pos, buffer_size - pos, "%s", name ? name : "");
    } else if (*p == 'L') { // Уровень
      pos += snprintf(formatted_msg + pos, buffer_size - pos, "%s", style::level_to_string(lvl));
    } else if (*p == 'l') { // Сообщение
      pos += snprintf(formatted_msg + pos, buffer_size - pos, "%s", msg ? msg : "");
    } else { // Любой другой символ
      formatted_msg[pos++] = *p;
    }
  }

  formatted_msg[pos] = '\0';
  return formatted_msg;
}

} // namespace tlog

