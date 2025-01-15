/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "common.h"
#include "logger.h"

namespace tlog::queue {

struct log_data {
  struct logger* log;
  char* message;
  int level;
};

typedef struct task {
  void (*function)(void*);
  void* argument;
  int priority;
  int id;
} task;

typedef struct log_queue {
  task tasks[128];
  int front;
  int rear;
  int size;
  pthread_mutex_t mutex;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
} log_queue;

void init(log_queue* queue);
bool enqueue(log_queue* queue, task task);
bool dequeue(log_queue* queue, task* task);
bool empty(log_queue* queue);

} // namespace tlog

