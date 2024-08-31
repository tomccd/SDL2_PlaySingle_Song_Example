cmake_minimum_required(VERSION 3.5)

project(Single_Music)


add_executable(
    main
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(
    main
    SDL2_mixer
    SDL2
    avformat
    avutil
    avcodec
    stdc++fs
)