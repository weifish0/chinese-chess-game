cmake_minimum_required(VERSION 3.27)
project(CHINESE-CHESS-GAME)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(Resource)

# file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 添加 libcurl 依賴
find_package(CURL REQUIRED)

# 添加 nlohmann/json 依賴
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

add_executable(CHINESE-CHESS-GAME

    ChessPiece/ChessPiece.cpp
    ChessPiece/ChessPiece.hpp
    ChessPiece/CannonPiece.cpp
    ChessPiece/CannonPiece.hpp
    ChessPiece/ChariotPiece.cpp
    ChessPiece/ChariotPiece.hpp
    ChessPiece/ElephantPiece.cpp
    ChessPiece/ElephantPiece.hpp
    ChessPiece/GuardPiece.cpp
    ChessPiece/GuardPiece.hpp
    ChessPiece/HorsePiece.cpp
    ChessPiece/HorsePiece.hpp
    ChessPiece/KingPiece.cpp
    ChessPiece/KingPiece.hpp
    ChessPiece/PawnPiece.cpp
    ChessPiece/PawnPiece.hpp

    Engine/Allegro5Exception.cpp
    Engine/Allegro5Exception.hpp
    Engine/AudioHelper.cpp
    Engine/AudioHelper.hpp
    Engine/Collider.cpp
    Engine/Collider.hpp
    Engine/GameEngine.cpp
    Engine/GameEngine.hpp
    Engine/Group.cpp
    Engine/Group.hpp
    Engine/IControl.cpp
    Engine/IControl.hpp
    Engine/IObject.cpp
    Engine/IObject.hpp
    Engine/IScene.cpp
    Engine/IScene.hpp
    Engine/LOG.cpp
    Engine/LOG.hpp
    Engine/Point.cpp
    Engine/Point.hpp
    Engine/Resources.cpp
    Engine/Resources.hpp
    Engine/Sprite.cpp
    Engine/Sprite.hpp
    
    
    Scene/anqi_start_scene.cpp
    Scene/anqi_start_scene.hpp
    Scene/mode_select.cpp
    Scene/mode_select.hpp
    Scene/anqi_scene.cpp
    Scene/anqi_scene.hpp
    Scene/harmony_scene.cpp
    Scene/harmony_scene.hpp
    Scene/red_win_scene.cpp
    Scene/red_win_scene.hpp
    Scene/black_win_scene.cpp
    Scene/black_win_scene.hpp
    Scene/harmony_scene.hpp
    Scene/harmony_scene.cpp
    Scene/start_scene.cpp
    Scene/start_scene.hpp
    Scene/xiangqi_scene.cpp
    Scene/xiangqi_scene.hpp
    Scene/xiangqi_win_scene.cpp
    Scene/xiangqi_win_scene.hpp
    Scene/playground.cpp
    Scene/playground.hpp
    Scene/login.cpp
    Scene/login.hpp
    Player/player.cpp
    Player/player.hpp
    NPC/npc.cpp
    NPC/npc.hpp

    UI/Component/Image.cpp
    UI/Component/Image.hpp
    UI/Component/ImageButton.cpp
    UI/Component/ImageButton.hpp
    UI/Component/Label.cpp
    UI/Component/Label.hpp
    UI/Component/Slider.cpp
    UI/Component/Slider.hpp

    Chess/chess.cpp
    Chess/chess.hpp
    
    main.cpp
)

IF (WIN32)
    target_include_directories(CHINESE-CHESS-GAME PUBLIC C:/allegro/include)
    target_link_directories(CHINESE-CHESS-GAME PUBLIC C:/allegro/lib)
    target_compile_options(CHINESE-CHESS-GAME PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(CHINESE-CHESS-GAME 
        liballegro_monolith
        CURL::libcurl
        nlohmann_json::nlohmann_json
    )
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(CHINESE-CHESS-GAME PUBLIC lib)
    target_include_directories(CHINESE-CHESS-GAME PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(CHINESE-CHESS-GAME PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(CHINESE-CHESS-GAME 
        ${ALLEGRO5_LIBRARIES}
        CURL::libcurl
        nlohmann_json::nlohmann_json
    )
    target_link_directories(CHINESE-CHESS-GAME PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()