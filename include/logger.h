/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "common.h"

namespace tlog {

typedef struct logger {
  int log_level;
  FILE* log_file;
  const char* name;
  char* pattern;
  void (*pre_log_hook)();
  void (*post_log_hook)();
} logger;

void logger_init(logger* l,
                 int lvl,
                 const char* log_file,
                 const char* name,
                 const char* pattern,
                 void (*pre_hook)(),
                 void (*post_hook)()
                 );
void log_msg(logger* l, int lvl, const char* format, ...);
void logger_close(logger* l);


} // namespace tlog

