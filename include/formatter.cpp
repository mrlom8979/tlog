/*
 * Copyright (C) Yakiv Matiash
 */

#include "formatter.h"
#include "common.h"
#include "style.h"

namespace tlog::formatter {

const char* get_current_time(const char* t) {
  static thread_local char buffer[64];
  std::time_t now = std::time(nullptr);
  std::strftime(buffer, sizeof(buffer), t, std::localtime(&now));
  return buffer;
}

formatted_msg format(const char* pattern, const char* name, int lvl, const char* msg) {
  size_t buffer_size = 1024;
  char* console_buffer = (char*)malloc(buffer_size);
  char* file_buffer = (char*)malloc(buffer_size);

  if (!console_buffer || !file_buffer) {
    fprintf(stderr, "Failed to allocate memory for log message.\n");
    free(console_buffer);
    free(file_buffer);
    return formatted_msg{nullptr, nullptr};
  }

  size_t console_pos = 0;
  size_t file_pos = 0;

  for (const char* p = pattern; *p != '\0'; ++p) {

    if (console_pos >= buffer_size - 1) {
      buffer_size *= 2;
      char* new_console_buffer = (char*)realloc(console_buffer, buffer_size);
      if (!new_console_buffer) {
        fprintf(stderr, "Failed to reallocate memory for log message.\n");
        free(console_buffer);
        free(file_buffer);
        return formatted_msg{nullptr, nullptr};
      }
      console_buffer = new_console_buffer;
    }

    if (file_pos >= buffer_size - 1) {
      buffer_size *= 2;
      char* new_file_buffer = (char*)realloc(file_buffer, buffer_size);
      if (!new_file_buffer) {
        fprintf(stderr, "Failed to reallocate memory for file message.\n");
        free(console_buffer);
        free(file_buffer);
        return formatted_msg{nullptr, nullptr};
      }
      file_buffer = new_file_buffer;
    }

    if (strncmp(p, "<t>", 3) == 0) {
      console_pos += snprintf(
        console_buffer + console_pos,
        buffer_size - console_pos,
        "%s",
        style::get_color(lvl)
      );

      p += 2;
    } else if (strncmp(p, "</t>", 4) == 0) {
      console_pos += snprintf(
        console_buffer + console_pos,
        buffer_size - console_pos,
        "%s",
        style::RESET_COLOR
      );
      p += 3;
    } else if (strncmp(p, "<b>", 3) == 0) {
      const char* bg_color = style::get_background_color(lvl);
      const char* txt_color = style::get_color_for_background(bg_color);
      console_pos += snprintf(
        console_buffer + console_pos,
        buffer_size - console_pos,
        "%s%s",
        bg_color,
        txt_color
      );
      p += 2;
    } else if (strncmp(p, "</b>", 4) == 0) {
      console_pos += snprintf(
        console_buffer + console_pos,
        buffer_size - console_pos,
        "%s",
        style::RESET_COLOR
      );
      p += 3;
    } else if (strncmp(p, "<gb>", 3) == 0) {
      const char* bg_color = style::background::LIGHT_BLACK;
      const char* txt_color = style::get_color_for_background(bg_color);
      console_pos += snprintf(
        console_buffer + console_pos,
        buffer_size - console_pos,
        "%s%s",
        bg_color,
        txt_color
      );
      p += 3;
    } else if (strncmp(p, "<r>", 3) == 0) {
      console_pos += snprintf(
        console_buffer + console_pos,
        buffer_size - console_pos,
        "%s",
        style::RESET_COLOR
      );
      p += 2;
    } else if (*p == '\n') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", "\n");
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", "");
    } else if (*p == 's') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", "\u22EE");
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", "|");
    } else if (*p == 'H') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", get_current_time("%H"));
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", get_current_time("%H"));
    } else if (*p == 'M') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", get_current_time("%M"));
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", get_current_time("%M"));
    } else if (*p == 'S') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", get_current_time("%S"));
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", get_current_time("%S"));
    } else if (*p == 'Y') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", get_current_time("%Y"));
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", get_current_time("%Y"));
    } else if (*p == 'm') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", get_current_time("%m"));
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", get_current_time("%m"));
    } else if (*p == 'd') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", get_current_time("%d"));
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", get_current_time("%d"));
    } else if (*p == 'N') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", name ? name : "");
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", name ? name : "");
    } else if (*p == 'L') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", style::level_to_string(lvl));
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", style::level_to_string(lvl));
    } else if (*p == 'l') {
      console_pos += snprintf(console_buffer + console_pos, buffer_size - console_pos, "%s", msg ? msg : "");
      file_pos += snprintf(file_buffer + file_pos, buffer_size - file_pos, "%s", msg ? msg : "");
    } else {
      console_buffer[console_pos++] = *p;
      file_buffer[file_pos++] = *p;
    }
  }

  console_buffer[console_pos] = '\0';
  file_buffer[file_pos] = '\0';

  return formatted_msg{console_buffer, file_buffer};
}

} // namespace tlog

