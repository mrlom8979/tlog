# TLog for Application

## Introduction

This logger is designed for easy message logging in an application, with support for colored terminal output and file logging. It supports multiple logging levels and outputs messages in a convenient format. The logger processes messages asynchronously using a queue and a separate thread, preventing race conditions when outputting messages to the terminal and writing them to files.

## Setup

1. **Include the header file:**

   To use the logger, include the following header file:

   ```cpp
   #include "logger.h"
   ```

2. **Initialize the logger:**

   The logger must be initialized before use:

   log.h
   ```cpp
   #pragma once

   #include "pch.h"
   #include <logger.h>

   namespace log {

   void init();
   void shutdown();
   tlog::logger& get();


   } // namespace log

   #define APP_RAW(...)        ::tlog::log_msg(&log::get(), TLOG_RAW, __VA_ARGS__)
   #define APP_DEBUG(...)      ::tlog::log_msg(&log::get(), TLOG_DEBUG, __VA_ARGS__)
   #define APP_INFO(...)       ::tlog::log_msg(&log::get(), TLOG_INFO, __VA_ARGS__)
   #define APP_WARN(...)       ::tlog::log_msg(&log::get(), TLOG_WARNING, __VA_ARGS__)
   #define APP_ERROR(...)      ::tlog::log_msg(&log::get(), TLOG_ERROR, __VA_ARGS__)
   #define APP_CRITICAL(...)   ::tlog::log_msg(&log::get(), TLOG_CRITICAL, __VA_ARGS__)
   #define APP_SUCC(...)       ::tlog::log_msg(&log::get(), TLOG_SUCCESS, __VA_ARGS__)

   ```
   log.cpp
   ```cpp
   #include "log.h"

   namespace log {

   tlog::logger s_log;

   void init() {
     // tlog::logger_init(&s_log, TLOG_RAW, "log/app.log", "APP", "<t>Y-m-d H:M:S</t> <b> N -> L </b>: l");
     // tlog::logger_init(&s_log, TLOG_RAW, "log/app.log", "APP", "<b> H:M:S | N > L </b><t>: l</t>");
     tlog::logger_init(&s_log, TLOG_RAW, "log/app.log", "APP", "<b> H:M:S | N > L </b><t>:\n l</t>");
   }

   void shutdown() {
     tlog::logger_close(&s_log);
   }

   tlog::logger& get() {
     return s_log;
   }

   } // namespace log

   ```

3. **Shutdown the logger:**

   After the program finishes, call the following to shut down the logger:

   ```cpp
   log::shutdown();
   ```

## Main Usage

Example of using the logger:

```cpp
int main() {
    log::init();

    // Logging with different levels
    APP_RAW("application running.");
    APP_DEBUG("application running.");
    APP_INFO("application running.");
    APP_WARN("application running.");
    APP_ERROR("application running.");
    APP_CRITICAL("application running.");
    APP_SUCC("application running.");

    log::shutdown();
}
```

### Logging Levels

The logger supports the following logging levels:

- **TLOG_RAW** — For plain messages without special formatting.
- **TLOG_DEBUG** — For debugging messages.
- **TLOG_INFO** — For informational messages.
- **TLOG_WARNING** — For warning messages.
- **TLOG_ERROR** — For error messages.
- **TLOG_CRITICAL** — For critical error messages.
- **TLOG_SUCCESS** — For success messages.

For each of these levels, there is a macro for convenient logging:

```cpp
#define APP_RAW(...)        ::tlog::log_msg(&log::get(), TLOG_RAW, __VA_ARGS__)
#define APP_DEBUG(...)      ::tlog::log_msg(&log::get(), TLOG_DEBUG, __VA_ARGS__)
#define APP_INFO(...)       ::tlog::log_msg(&log::get(), TLOG_INFO, __VA_ARGS__)
#define APP_WARN(...)       ::tlog::log_msg(&log::get(), TLOG_WARNING, __VA_ARGS__)
#define APP_ERROR(...)      ::tlog::log_msg(&log::get(), TLOG_ERROR, __VA_ARGS__)
#define APP_CRITICAL(...)   ::tlog::log_msg(&log::get(), TLOG_CRITICAL, __VA_ARGS__)
#define APP_SUCC(...)       ::tlog::log_msg(&log::get(), TLOG_SUCCESS, __VA_ARGS__)
```

These macros allow you to log messages with the corresponding importance level.

## Message Format

The message format can be configured during the logger initialization in the `log::init()` function. Example configuration:

```cpp
tlog::logger_init(&s_log, TLOG_RAW, "log/app.log", "APP", "<b> H:M:S | N > L </b><t>:\n l</t>");
```

- In this example, time, name, and log level formatting is used, with color and background tags added:
  - `<b>` — Base background for messages.
  - `<t>` — Tag for text (color).
  - `%H:%M:%S` — Time format.
  - `%Y-%m-%d` — Date format.
  - `%N` — Name.
  - `%L` — Log level.

### Example Output

Depending on the logger's configuration, messages will be displayed in the terminal with color and background. Example output:

```bash
[2025-01-15 10:15:30] [APP] [INFO]: application running.
```

Colors will be automatically applied based on the log level, such as:

- **INFO** — Green
- **ERROR** — Red
- **WARNING** — Yellow

### Asynchronous Message Handling

The logger uses a message queue, and all messages are processed in a separate thread. This prevents race conditions when outputting logs to the terminal and writing them to a file.

## Example Integration with Graphics

For clarity, you can include examples of the terminal log output:

### Example Output for **INFO** level:
```bash
[INFO] application running.
```
### Example Output for **ERROR** level:
```bash
[ERROR] application running.
```

These examples can be supplemented with images or screenshots to help users visualize how the logs will appear in the terminal for different log levels.

## Conclusion

This logger provides a convenient interface for logging in your application, supporting multiple logging levels and offering both colored terminal output and file logging. The logger works asynchronously, avoiding race conditions when logging to the terminal and writing to files.

---

# Documentation for Tags in the Logging System

## Introduction

The logging system supports special tags for formatting text in the console, adding timestamps, and including other contextual information. This documentation describes the available tags and their behavior.

---

## Formatting Tags

### `<t>` and `</t>`
- **Description:** Changes the text color in the console according to the log level.
- **Usage Example:**
  ```cpp
  <t>Information</t>
  ```
- **Result:** The text "Information" will be displayed in the color associated with the log level.

---

### `<b>` and `</b>`
- **Description:** Sets the background and text colors to highlight a block of text.
- **Usage Example:**
  ```cpp
  <b>Important Message</b>
  ```
- **Result:** The text "Important Message" will be highlighted with a background color based on the log level.

---

### `<gb>`
- **Description:** Sets the background color to light black and automatically adjusts the text color for contrast.
- **Usage Example:**
  ```cpp
  <gb>System Message</gb>
  ```
- **Result:** The text "System Message" will appear on a light black background.

---

### `<r>`
- **Description:** Resets all formatting settings (text and background colors).
- **Usage Example:**
  ```cpp
  <r>Normal text</r>
  ```
- **Result:** The text will return to the default appearance.

---

## Timestamp Tags

### `H`, `M`, `S`
- **Description:** Inserts the current hour, minute, or second.
- **Usage Example:**
  ```cpp
  H:M:S
  ```
- **Result:** The current time, e.g., `14:30:45`.

### `Y`, `m`, `d`
- **Description:** Inserts the current year, month, or day.
- **Usage Example:**
  ```cpp
  Y-m-d
  ```
- **Result:** The current date, e.g., `2025-01-16`.

---

## Message Data Tags

### `N`
- **Description:** Inserts the name of the source (if provided).
- **Usage Example:**
  ```cpp
  Source: N
  ```
- **Result:** The name of the message source.

### `L`
- **Description:** Inserts the string representation of the log level.
- **Usage Example:**
  ```cpp
  Level: L
  ```
- **Result:** The log level, e.g., `INFO`.

### `l`
- **Description:** Inserts the message text.
- **Usage Example:**
  ```cpp
  Message: l
  ```
- **Result:** The message content.

---

## Other Symbols

### `\n`
- **Description:** Inserts a newline.
- **Usage Example:**
  ```cpp
  This is the first line\nThis is the second line
  ```

### `s`
- **Description:** Adds the symbol `⋮` in the console and `|` in the file.
- **Usage Example:**
  ```cpp
  Separator: s
  ```

---

### Notes
1. All tags must be properly closed to avoid display errors.
2. Tags only affect console output. Messages written to files are recorded without formatting.

---

