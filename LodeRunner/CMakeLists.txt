cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME LodeRunner)
set(PLAYDATE_GAME_DEVICE ${PLAYDATE_GAME_NAME})

project(${PLAYDATE_GAME_NAME} C CXX ASM)

include_directories(
        src
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
        src/LodeRunner.cpp
        src/LodeRunner_EnemyMovements.cpp
        src/LodeRunner_PlayerMovements.cpp
        src/LodeRunner_Render.cpp
        src/LodeRunner_SplashScreen.cpp
        src/LodeRunner_Utils.cpp
	src/src/utils/Arduboy2Ext.cpp
        src/src/utils/Utils.cpp
        src/src/levels/Level.cpp
        ${ARDUBOY_SOURCE}
        ${ARDUBOYTONES_SOURCE}
        )

add_playdate_application(${PLAYDATE_GAME_NAME})  
target_sources(${PLAYDATE_GAME_NAME} PUBLIC ${SOURCE})
target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE pdcpp_core)
set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")


