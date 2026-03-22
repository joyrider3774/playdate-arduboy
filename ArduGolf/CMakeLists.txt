cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME ArduGolf)
project(${PLAYDATE_GAME_NAME} C CXX ASM)

include_directories(
        src
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
		src/arduboy_minigolf.cpp
		src/camera.cpp
		src/div.cpp
		src/font.cpp
		src/mul.cpp
		src/draw.cpp
		src/graphics.cpp
		src/physics.cpp
		src/sincos.cpp
		src/render_scene.cpp
		src/mat.cpp
		src/game.cpp
		src/save.cpp

        ${ARDUBOY_SOURCE}
        ${ARDUBOYTONES_SOURCE}
        )


add_playdate_application(${PLAYDATE_GAME_NAME})  
target_sources(${PLAYDATE_GAME_NAME} PUBLIC ${SOURCE})
target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE pdcpp_core)
set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")
target_compile_definitions(${PLAYDATE_GAME_NAME} PUBLIC ARDUGOLF_FX=0)

