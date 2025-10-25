cmake_minimum_required(VERSION 3.28)
project(Pong LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp)
target_compile_features(main PRIVATE cxx_std_20)
target_link_libraries(main PRIVATE SFML::Graphics)
target_link_libraries(main PRIVATE SFML::Audio)
set_target_properties(main PROPERTIES OUTPUT_NAME "pong")

file(COPY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/BoldPixels.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/BoldPixels.ttf)
file(COPY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/pong.wav ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pong.wav)