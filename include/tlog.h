/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "common.h"
#include "queue.h"

namespace tlog {

typedef struct log_thread {
  pthread_t thread;
  bool stop_flag;
  queue::log_queue log_queue;
} log_thread;

log_thread* get_ctx();

void* logger_thread(void* arg);
void log_task(void (*function)(void*), void* argument);

} // namespace tlog

