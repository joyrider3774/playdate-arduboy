cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME Catacombs)
project(${PLAYDATE_GAME_NAME} C CXX ASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

include_directories(
        src
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
        src/Arduboy3D.cpp

        src/Draw.cpp
        src/FixedMath.cpp
        src/MapGenerator.cpp
        src/Game.cpp
        src/Map.cpp
        src/Enemy.cpp
        src/Entity.cpp
        src/Particle.cpp
        src/Menu.cpp
        src/Player.cpp
        src/Projectile.cpp
        src/Sounds.cpp
        src/Font.cpp

        ${ARDUBOY_SOURCES}
        ${ARDUBOYTONES_SOURCES}
        )
add_playdate_application(${PLAYDATE_GAME_NAME})  
target_sources(${PLAYDATE_GAME_NAME} PUBLIC ${SOURCE})
target_link_libraries(${PLAYDATE_GAME_NAME} pdcpp_core)

set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")



