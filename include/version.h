/*
 * Copyright (C) Yakiv Matiash
 */

#pragma once

#define TLOG_VER_MAJOR 2025
#define TLOG_VER_MINOR 1
#define TLOG_VER_PATCH 14

#define TLOG_TO_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)
#define TLOG_VERSION TLOG_TO_VERSION(TLOG_VER_MAJOR, TLOG_VER_MINOR, TLOG_VER_PATCH)
