/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#include "pch.h"
#include "common.h"

namespace tlog::formatter {

const char* get_current_time();
char* format(const char* pattern, const char* name, int lvl, const char* msg);

} // namespace tlog

