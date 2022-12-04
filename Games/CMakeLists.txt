cmake_minimum_required(VERSION 3.0.0)

add_executable(Game)

set(GAME_SOURCES 
    "Sources/main.cpp"
    "Sources/RobotsGame.cpp")

set(GAME_HEADERS
    "Include/RobotsGame.h")

target_sources(Game PRIVATE ${GAME_SOURCES} ${GAME_HEADERS})
target_link_to_dependencies(Game PRIVATE SLEngine)
target_include_directories(Game PRIVATE Include)
