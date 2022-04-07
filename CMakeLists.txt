set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

cmake_minimum_required (VERSION 3.5)
project (Stakken)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++20 -pthread")
set (CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS} -ffast-math -O3")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release)
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

find_package(unofficial-enet CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)
find_package(EASTL CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/src")

# Client Target
add_executable (Stakken
    ${PROJECT_SOURCE_DIR}/include/gl3w.c
    ${source_dir}/main.cpp
    ${source_dir}/Application.cpp

    ${source_dir}/States/MenuState.cpp
    ${source_dir}/States/SettingsState.cpp
    ${source_dir}/States/LobbyState.cpp
    ${source_dir}/States/ChallengeSelectState.cpp
    ${source_dir}/States/ChallengePlayState.cpp
    
    ${source_dir}/Assets/Assets.cpp
    ${source_dir}/Assets/Texture.cpp
    ${source_dir}/Assets/BGShader.cpp
    ${source_dir}/Assets/Sound.cpp
    ${source_dir}/Assets/Font.cpp
    ${source_dir}/Assets/Sounds.cpp


    ${source_dir}/Rendering/Renderer.cpp
    ${source_dir}/Rendering/Shader.cpp
    ${source_dir}/Rendering/ScreenQuad.cpp
    ${source_dir}/Rendering/RenderGame.cpp

    ${source_dir}/Data/Identity.cpp

    ${source_dir}/UI/Node.cpp
    ${source_dir}/UI/UI.cpp
    ${source_dir}/UI/NodeTypes.cpp
    

    ${source_dir}/Game/Game.cpp
    ${source_dir}/Game/Board.cpp
    ${source_dir}/Game/Piece.cpp
    ${source_dir}/Game/Randomizers/XoroshiroRandomizer.cpp
    ${source_dir}/Game/Randomizers/SimonRandomizer.cpp

    ${source_dir}/Game/DemoPlayer.cpp
    
    ${source_dir}/Game/Demo.cpp
    ${source_dir}/Game/DemoRecorder.cpp

    ${source_dir}/Game/ClientLobby.cpp
    ${source_dir}/Game/LocalLobby.cpp

    ${source_dir}/Input/InputProfile.cpp
    ${source_dir}/Input/Keyboard.cpp
    ${source_dir}/Input/Input.cpp
    
    ${source_dir}/Challenges/Challenges.cpp

    ${source_dir}/Gamemodes/Gamemodes.cpp

    ${source_dir}/Networking/Server.cpp
)

target_link_libraries(Stakken
    dl
    X11
    unofficial::enet::enet
    SDL2::SDL2main
    SDL2::SDL2
    SDL2::SDL2_mixer
    SDL2::SDL2-static
    glm::glm
    gflags::gflags
    Vorbis::vorbis Vorbis::vorbisenc Vorbis::vorbisfile
    EASTL
)
target_compile_definitions(Stakken PUBLIC IS_CLIENT)

# Client Target
add_executable (Server
    ${source_dir}/Networking/Server.cpp
)

target_link_libraries(Server
    dl
    X11
    unofficial::enet::enet
    gflags::gflags
    EASTL
)
target_compile_definitions(Server PUBLIC IS_CLIENT)

# Client Test Target
add_executable (Client
    ${source_dir}/Networking/Client.cpp
)

target_link_libraries(Client
    dl
    X11
    unofficial::enet::enet
    gflags::gflags
    EASTL
)
target_compile_definitions(Client PUBLIC IS_CLIENT)

