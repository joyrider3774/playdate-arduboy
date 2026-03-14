cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME Tacklebox)
set(PLAYDATE_GAME_DEVICE ${PLAYDATE_GAME_NAME})

project(${PLAYDATE_GAME_NAME} C CXX ASM)

include_directories(
        src
        src/res
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
        src/tacklebox.cpp
        src/src/boat.cpp
        src/src/dialogUtils.cpp
        src/src/drawBitmap.cpp
        src/src/fish.cpp
        src/src/game.cpp
        src/src/girl.cpp
        src/src/girlDialog.cpp
        src/src/guy.cpp
        src/src/guyDialog.cpp
        src/src/hud.cpp
        src/src/player.cpp
        src/src/renderer.cpp
        src/src/sfx.cpp
        src/src/shop.cpp
        src/src/sign.cpp
        src/src/state.cpp
        src/src/tileFloor.cpp
        src/src/toast.cpp
        src/src/worm.cpp

        ${ARDUBOY_SOURCES}
        ${ARDUBOYTONES_SOURCES}
		${ARDUBOYPLAYTUNE_SOURCES}
        )

add_playdate_application(${PLAYDATE_GAME_NAME})  
target_sources(${PLAYDATE_GAME_NAME} PUBLIC ${SOURCE})
target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE pdcpp_core)
set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")


