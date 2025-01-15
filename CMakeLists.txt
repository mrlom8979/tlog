# Поиск всех .cpp и .h файлов
file(GLOB_RECURSE LIB_SOURCES "include/*.cpp")
file(GLOB_RECURSE LIB_HEADERS "include/*.h")

# Создание библиотеки
add_library(tlog STATIC ${LIB_SOURCES} ${LIB_HEADERS})

# Подключение предкомпилированного заголовка
target_precompile_headers(tlog PRIVATE include/pch.h)

# Установка директорий для поиска заголовков
target_include_directories(tlog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Настройка платформо-зависимых флагов
if (UNIX)
    target_compile_definitions(tlog PRIVATE __linux__)
    target_compile_options(tlog PRIVATE -Wall -Wextra -pedantic)
elseif(WIN32)
    target_compile_definitions(tlog PRIVATE _WIN32)
    target_compile_options(tlog PRIVATE /W4)
endif()

