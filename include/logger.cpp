/*
 * Copyright (C) Yakiv Matiash
 */

#include "logger.h"
#include "queue.h"
#include "tlog.h"
#include "formatter.h"

namespace tlog {

void logger_init(logger* l, int lvl, const char* log_file, const char* name, const char* pattern) {
  std::filesystem::path file_path = log_file;
  std::filesystem::path log_dir = file_path.parent_path();

  if (!log_dir.empty() && !std::filesystem::exists(log_dir)) {
    try {
      std::filesystem::create_directories(log_dir);
      printf("Created log directory: %s\n", log_dir.string().c_str());
    } catch (const std::filesystem::filesystem_error& e) {
      fprintf(stderr, "Failed to create log directory: %s\n", e.what());
      return;
    }
  }

  // Попробуем открыть файл
  l->log_file = fopen(log_file, "a");
  if (!l->log_file) {
    fprintf(stderr, "Failed to open log file: %s\n", log_file);
    return;
  }

  l->log_level = lvl;
  // l->log_time = log_time;
  l->name = name ? strdup(name) : nullptr; // Сохраняем префикс
  l->pattern = pattern ? strdup(pattern) : strdup("H:M:S [N] <b> L </b>: l");
}

void log_task(void* arg) {
  auto* data = reinterpret_cast<queue::log_data*>(arg);
  if (!data || !data->log || !data->log->log_file || !data->message) {
    fprintf(stderr, "Invalid log data provided.\n");
    if (data) free(data);
    return;
  }

  /*
  const char* time_str = "";
  if (data->log->log_time) {
    time_str = get_current_time().c_str();
  }
  */
  char* formatted_msg = nullptr;

  if (data->level > TLOG_RAW) {
    formatted_msg = formatter::format(
      data->log->pattern,
      data->log->name,
      data->level,
      data->message
    );
  } else {
    formatted_msg = data->message;
  }
  // char log_message[2048];
  // snprintf(log_message, sizeof(log_message), "[%s] %s%s\n",
  // time_str,
  // data->log->prefix ? data->log->prefix : "",
  // data->message);

  if (!formatted_msg) {
    free(data->message);
    free(data);
    return;
  }

  // Записываем в файл
  fprintf(data->log->log_file, "%s\n", formatted_msg);
  fflush(data->log->log_file);

  if (data->level >= 2) { // Например, уровень 2 и выше выводим в stderr
    fprintf(stderr, "%s\n", formatted_msg);
  } else {
    fprintf(stdout, "%s\n", formatted_msg);
  }

  if (data->level > TLOG_RAW) free(formatted_msg);
  free(data->message);
  free(data);
}

void log_msg(logger* l, int lvl, const char* format, ...) {
  if (lvl < l->log_level) return;

  va_list args;
  va_start(args, format);
  char buffer[2048];
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  auto* task = (queue::log_data*)malloc(sizeof(queue::log_data));
  if (!task) {
    fprintf(stderr, "Failed to allocate memory for log task.\n");
    return;
  }

  task->log = l;
  task->level = lvl;
  task->message = strdup(buffer);

  // Передаем `task` напрямую
  log_task(task);
}

void logger_close(logger* l) {
  if (l->log_file) {
    fclose(l->log_file);
    l->log_file = nullptr;
  }
}

} // namespace tlog

