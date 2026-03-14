cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME PocketFighter)
set(PLAYDATE_GAME_DEVICE PocketFighter)

project(${PLAYDATE_GAME_NAME} C CXX ASM)

include_directories(
        src
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
        src/main.cpp

        ${ARDUBOY_SOURCE}
        ${ARDUBOYPLAYTUNE_SOURCE}
        )

add_playdate_application(${PLAYDATE_GAME_NAME})  
target_sources(${PLAYDATE_GAME_NAME} PUBLIC ${SOURCE})
target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE pdcpp_core)
target_compile_definitions(${PLAYDATE_GAME_NAME} PRIVATE -DPLAYTUNE_DURATION_DECREASE_SIM=8 -DPLAYTUNE_DURATION_DECREASE_PD=2)
set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")


