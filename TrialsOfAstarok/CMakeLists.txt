cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME TrialsOfAstarok)
set(PLAYDATE_GAME_DEVICE ${PLAYDATE_GAME_NAME})

project(${PLAYDATE_GAME_NAME} C CXX ASM)

include_directories(
        src
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
        src/Astarok.cpp
		src/AstarokGame_Logic.cpp
		src/AstarokGame_Render.cpp
		src/Astarok_HighScores.cpp
		src/Astarok_IntroText.cpp
		src/Astarok_Seed.cpp
		src/Astarok_SplashScreen.cpp
		src/Astarok_Title.cpp
		src/src/entities/AISprite.cpp
		src/src/entities/InteractiveObject.cpp
		src/src/entities/Map.cpp
		src/src/entities/Room.cpp
		src/src/entities/Sprite.cpp
		src/src/utils/Arduboy2Ext.cpp

        ${ARDUBOY_SOURCES}
		${ARDUBOYPLAYTUNE_SOURCES}
        )

add_playdate_application(${PLAYDATE_GAME_NAME})  
target_sources(${PLAYDATE_GAME_NAME} PUBLIC ${SOURCE})
target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE pdcpp_core)
set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")


