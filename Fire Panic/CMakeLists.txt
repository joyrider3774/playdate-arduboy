cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME FirePanic)
set(PLAYDATE_GAME_DEVICE ${PLAYDATE_GAME_NAME})

project(${PLAYDATE_GAME_NAME} C CXX ASM)

include_directories(
        src
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
		src/FirePanic.cpp
        src/src/Game.cpp
		src/src/arduboyTones/ArduboyTonesExt.cpp
		src/src/entities/Ambulance.cpp
		src/src/entities/Angel.cpp
		src/src/entities/Coin.cpp
		src/src/entities/OtherCar.cpp
		src/src/entities/Player.cpp
		src/src/entities/Victim.cpp
		src/src/states/BaseState.cpp
		src/src/states/GameIntroState.cpp
		src/src/states/HighScoreState.cpp
		src/src/states/PlayGameState.cpp
		src/src/states/RaceState.cpp
		src/src/states/SplashScreenState.cpp
		src/src/states/TitleScreenState.cpp
		src/src/utils/Arduboy2Ext.cpp
		src/src/utils/EEPROM_Utils.cpp
		src/src/utils/GameContext.cpp

        ${ARDUBOY_SOURCE}
        )
add_playdate_application(${PLAYDATE_GAME_NAME})
target_sources(${PLAYDATE_GAME_NAME} PUBLIC ${SOURCE})
target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE pdcpp_core)
set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")
