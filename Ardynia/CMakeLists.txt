cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME Ardynia)
set(PLAYDATE_GAME_DEVICE ${PLAYDATE_GAME_NAME})

project(${PLAYDATE_GAME_NAME} C CXX ASM)

include_directories(
        src
        src/res
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
		src/ardynia.cpp
        src/src/drawBitmap.cpp
        src/src/entity.cpp
        src/src/game.cpp
        src/src/hud.cpp
        src/src/inGameMenu.cpp
        src/src/loadEntity.cpp
        src/src/map.cpp
        src/src/renderer.cpp
        src/src/sfx.cpp
        src/src/state.cpp
        src/src/tileRoom.cpp
        src/src/util.cpp
        src/src/entities/bat.cpp
        src/src/entities/blob.cpp
        src/src/entities/blobMother.cpp
        src/src/entities/bomb.cpp
        src/src/entities/boomerang.cpp
        src/src/entities/chest.cpp
        src/src/entities/commonEnemy.cpp
        src/src/entities/explosion.cpp
        src/src/entities/lock.cpp
        src/src/entities/nemesis.cpp
        src/src/entities/player.cpp
        src/src/entities/pot.cpp
        src/src/entities/projectile.cpp
        src/src/entities/secretWall.cpp
        src/src/entities/skeleton.cpp
        src/src/entities/snake.cpp
        src/src/entities/spike.cpp
        src/src/entities/switch.cpp
        src/src/entities/sword.cpp
        src/src/entities/torch.cpp
        src/src/entities/triggerDoor.cpp

        ${ARDUBOY_SOURCES}
		${ARDUBOYPLAYTUNE_SOURCES}
        )

add_playdate_application(${PLAYDATE_GAME_NAME})  
target_sources(${PLAYDATE_GAME_NAME} PUBLIC ${SOURCE})
target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE pdcpp_core)
target_compile_definitions(${PLAYDATE_GAME_NAME} PRIVATE -DPLAYTUNE_DURATION_DECREASE_SIM=8 -DPLAYTUNE_DURATION_DECREASE_PD=8)
set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")


