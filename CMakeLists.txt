cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME AI_FOR_CHECKERS)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)


set(SOURCES
main/main.cpp
saving/Downloading.cpp
saving/Uploading.cpp
AI/BestMoveAI.cpp
user/UserMoves.cpp
window/Changes.cpp
window/MainLoop.cpp
window/Mouse.cpp
window/Classes.cpp
)

set(HEADERS
main/MainHead.hpp
AI/AI.hpp
saving/Saving.hpp
user/User.hpp
window/Window.hpp
)

set(DIRECTORIES
    AI
    saving
    user
    window
    main)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${DIRECTORIES})

target_link_libraries(${PROJECT_NAME} -lsfml-graphics  -lsfml-window  -lsfml-system)