cmake_minimum_required(VERSION 3.14)

set(PLAYDATE_GAME_NAME DarkAndUnder)
set(PLAYDATE_GAME_DEVICE ${PLAYDATE_GAME_NAME})

project(${PLAYDATE_GAME_NAME} C CXX ASM)

include_directories(
        src
        ${ARDUBOY_INCLUDES}
)

set(SOURCE
        src/Dark-And-Under.cpp
		src/DarkUnder_Battle.cpp
		src/DarkUnder_Initialise.cpp
		src/DarkUnder_Inventory.cpp
		src/DarkUnder_Item.cpp
		src/DarkUnder_LargeMap.cpp
		src/DarkUnder_Play.cpp
		src/DarkUnder_Render.cpp
		src/src/controllers/BaseController.cpp
		src/src/controllers/EnemyController.cpp
		src/src/controllers/PlayerController.cpp
		src/src/entities/Base.cpp
		src/src/entities/Enemy.cpp
		src/src/entities/Item.cpp
		src/src/entities/Player.cpp
		src/src/fonts/Font3x5.cpp
		src/src/levels/Level.cpp
		src/src/utils/Arduboy2Ext.cpp
		src/src/utils/EnemyNames.cpp
        ${ARDUBOY_SOURCE}
        ${ARDUBOYTONES_SOURCE}
        )

add_playdate_application(${PLAYDATE_GAME_NAME})  
target_sources(${PLAYDATE_GAME_NAME} PUBLIC ${SOURCE})
target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE pdcpp_core)
set_property(TARGET ${PLAYDATE_TARGET_NAME} APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PLAYDATE_GAME_NAME}.pdx")


