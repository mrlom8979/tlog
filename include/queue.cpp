/*
 * Copyright (C) Yakiv Matiash
 */

#include "queue.h"

namespace tlog::queue {

#define PHX_QUEUE_SIZE 100

// Task queue functions
void init(log_queue* q) {
  q->front = 0;
  q->rear = -1;
  q->size = 0;
  pthread_mutex_init(&q->mutex, NULL);
  pthread_cond_init(&q->not_empty, NULL);
  pthread_cond_init(&q->not_full, NULL);
}

bool enqueue(log_queue* q, task task) {
  pthread_mutex_lock(&q->mutex);

  while (q->size == PHX_QUEUE_SIZE) {
    pthread_cond_wait(&q->not_full, &q->mutex);
  }

  q->rear = (q->rear + 1) % PHX_QUEUE_SIZE;
  q->tasks[q->rear] = task;
  q->size++;

  pthread_cond_signal(&q->not_empty);
  pthread_mutex_unlock(&q->mutex);
  return true;
}

bool dequeue(log_queue* q, task* task) {
  pthread_mutex_lock(&q->mutex);

  while (q->size == 0) {
    pthread_cond_wait(&q->not_empty, &q->mutex);
  }

  *task = q->tasks[q->front];
  q->front = (q->front + 1) % PHX_QUEUE_SIZE;
  q->size--;

  pthread_cond_signal(&q->not_full);
  pthread_mutex_unlock(&q->mutex);
  return true;
}

bool empty(log_queue* q) {
  pthread_mutex_lock(&q->mutex);
  bool is_empty = (q->size == 0);
  pthread_mutex_unlock(&q->mutex);
  return is_empty;
}

} // namespace tlog

