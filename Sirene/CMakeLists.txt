cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME Sirene)
set(PLAYDATE_GAME_DEVICE ${PLAYDATE_GAME_NAME})

project(${PLAYDATE_GAME_NAME} C CXX ASM)

include_directories(
        src
        src/res
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
		src/SRN_AB.cpp
       

        ${ARDUBOY_SOURCES}
		${ARDUBOYTONES_SOURCES}
        )

add_playdate_application(${PLAYDATE_GAME_NAME})  
target_sources(${PLAYDATE_GAME_NAME} PUBLIC ${SOURCE})
target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE pdcpp_core)
set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")