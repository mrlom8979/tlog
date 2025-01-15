/*
 * Copyright (C) Yakiv Matiash
 */

#include "tlog.h"

namespace tlog {


namespace {

static log_thread* s_tlog = NULL;
static int global_task_id = 0;

void tlog_destroy() {
  if (s_tlog) {

    s_tlog->stop_flag = true;

    queue::task stop_task = {NULL, NULL, 0, 0};
    queue::enqueue(&s_tlog->log_queue, stop_task);
    pthread_join(s_tlog->thread, NULL);

    // free(s_tlog->thread);
    pthread_mutex_destroy(&s_tlog->log_queue.mutex);
    pthread_cond_destroy(&s_tlog->log_queue.not_empty);
    pthread_cond_destroy(&s_tlog->log_queue.not_full);

    free(s_tlog);
    s_tlog = NULL;
  }
}

void tlog_init() {
  if (!s_tlog) {
    s_tlog = (log_thread*)malloc(sizeof(log_thread));
    if (!s_tlog) {
      printf("Failed to allocate memory for tlog.\n");
      exit(EXIT_FAILURE);
    }
    s_tlog->stop_flag = false;
    queue::init(&s_tlog->log_queue);
    pthread_create(&s_tlog->thread, NULL, logger_thread, s_tlog);

    std::atexit(tlog_destroy);
  }
}

} // anonyous namespace

void* logger_thread(void* arg) {
  log_thread* logger = (log_thread*)arg;
  while (true) {
    queue::task task;
    if (!queue::dequeue(&logger->log_queue, &task)) {
      continue;
    }

    if (logger->stop_flag && task.function == NULL) {
      break;
    }

    if (task.function) {
      task.function(task.argument);
    }
  }
  return NULL;
}

log_thread* get_ctx() {
  tlog_init();
  return s_tlog;
}

void log_task(void (*function)(void*), void* argument) {
  tlog_init();
  int task_id = __sync_fetch_and_add(&global_task_id, 1);

  queue::task task = {function, argument, 0, task_id};
  if (!queue::enqueue(&s_tlog->log_queue, task)) {
    fprintf(stderr, "Failed to add task(ID %d): Queue is full!\n", task_id);
  }
}

} // namespace tlog

