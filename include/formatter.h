/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "common.h"

namespace tlog::formatter {

typedef struct {
  char* console_msg;
  char* file_msg;
} formatted_msg;

const char* get_current_time();
formatted_msg format(const char* pattern, const char* name, int lvl, const char* msg);

} // namespace tlog

