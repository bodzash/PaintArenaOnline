cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(IS_CLIENT ON)

if(IS_CLIENT)

  project(Client)
  
  add_executable(${PROJECT_NAME}
    Client/Source/Client.cpp
    Client/Source/Network.cpp
    Client/Source/Console.cpp
  )

  add_subdirectory(External/raylib)
  add_subdirectory(External/enet)

  target_include_directories(${PROJECT_NAME}
    PUBLIC Shared
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
    Server/Source/Server.cpp
  )

  add_subdirectory(External/enet)

  target_include_directories(${PROJECT_NAME}
    PUBLIC Shared
    PUBLIC Server/Include
    PUBLIC External/entt/src
    PUBLIC External/enet/include
  )

  target_link_directories(${PROJECT_NAME} PUBLIC enet)

  target_link_libraries(${PROJECT_NAME} enet)

  set(CMAKE_CXX_FLAGS "MetadataServer.res")

endif(IS_CLIENT)
unset(IS_CLIENT CACHE)