cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(IS_CLIENT ON)

if(IS_CLIENT)

  project(Client)
  
  add_executable(${PROJECT_NAME}
    # Client
    Client/Source/Client.cpp
    Client/Source/Network.cpp
    #Core
    Client/Source/Systems/AudioSystems.cpp
    Client/Source/Systems/RendererSystems.cpp
    Client/Source/Systems/ShakeEffectSystems.cpp
    Client/Source/Systems/SmudgeEffectSystems.cpp
    Client/Source/Systems/ClientInputSystems.cpp
    # Shared
    Shared/Source/Math.cpp
    Shared/Source/Systems/BulletSystems.cpp
    Shared/Source/Systems/MovementSystems.cpp
    Shared/Source/Systems/BoundsSystems.cpp
  )

  add_subdirectory(External/raylib)
  add_subdirectory(External/enet)

  target_include_directories(${PROJECT_NAME}
    PUBLIC Shared/Include
    PUBLIC Client/Include
    PUBLIC External/raylib/src
    PUBLIC External/entt/src
    PUBLIC External/enet/include
  )

  target_link_directories(${PROJECT_NAME} PUBLIC raylib/raylib PUBLIC enet)

  target_link_libraries(${PROJECT_NAME} raylib enet)

  set(CMAKE_CXX_FLAGS "-mwindows MetadataClient.res")

else()

  project(Server)

  add_executable(${PROJECT_NAME}
    # Server
    Server/Source/Server.cpp
    Server/Source/Systems/ServerInputSystems.cpp
    #Timing
    # Shared
    Shared/Source/Math.cpp
    Shared/Source/Systems/BulletSystems.cpp
    Shared/Source/Systems/MovementSystems.cpp
    Shared/Source/Systems/BoundsSystems.cpp
  )

  add_subdirectory(External/enet)

  target_include_directories(${PROJECT_NAME}
    PUBLIC Shared/Include
    PUBLIC Server/Include
    PUBLIC External/entt/src
    PUBLIC External/enet/include
  )

  target_link_directories(${PROJECT_NAME} PUBLIC enet)

  target_link_libraries(${PROJECT_NAME} enet)

  set(CMAKE_CXX_FLAGS "MetadataServer.res")

endif(IS_CLIENT)
unset(IS_CLIENT CACHE)