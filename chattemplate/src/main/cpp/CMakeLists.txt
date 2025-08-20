cmake_minimum_required(VERSION 3.22.1)

project("chat-template-formatter")

include_directories(${CMAKE_SOURCE_DIR}/third-party/include)

add_library(${CMAKE_PROJECT_NAME} SHARED
        jni_template_formatter.cpp
        llm_chat_templates.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log)
