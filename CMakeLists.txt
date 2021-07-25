cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

cmake_policy(SET CMP0092 NEW) # no default MSVC warning flags in CMAKE_<LANG>_FLAGS

project(WordCounter LANGUAGES CXX)

set(BUILD_TESTS OFF CACHE BOOL "Build tests")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(Qt5 COMPONENTS Core Gui Qml REQUIRED)

add_executable(${PROJECT_NAME})

include(cmake/CompilerWarnings)

set_project_warnings(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES


    OUTPUT_NAME "Word Counter"

    WIN32_EXECUTABLE ON
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
)

target_sources(${PROJECT_NAME} PRIVATE
    src/models/word_counter.h
    src/models/word_counter.cpp
    src/models/word_counter_error.h
    src/models/word_counter_model.h
    src/models/word_counter_model.cpp
    src/models/word_counter_result.h
    src/models/word_counter_thread.h
    src/models/word_counter_thread.cpp
    src/main.cpp
    src/main.qrc
)

# Translations

find_package(Qt5LinguistTools REQUIRED)

set(APP_TS_FILES
    src/translations/en.ts
)

qt5_add_translation(APP_QM_FILES ${APP_TS_FILES})

include(cmake/GenerateResources)

set(APP_TRANSLATION_RESOURCES "${CMAKE_CURRENT_BINARY_DIR}/translations.qrc")

generate_resources(
    FILES ${APP_QM_FILES}
    OUTPUT ${APP_TRANSLATION_RESOURCES}
    PREFIX translations
)

target_sources(${PROJECT_NAME} PRIVATE
    ${APP_TRANSLATION_RESOURCES}
)

# Source Groups

source_group(src REGULAR_EXPRESSION "^.*\.(cpp|h|qrc)$")

# Tests

if(BUILD_TESTS)
    enable_testing()

    add_subdirectory(tests)
endif()
