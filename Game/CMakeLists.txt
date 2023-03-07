cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(IS_CLIENT ON)

if(IS_CLIENT)

  project(Client)

  add_executable(${PROJECT_NAME}
    Client/Source/Client.cpp
    Client/Source/NetWork.cpp
  )

  add_subdirectory(External/raylib)
  add_subdirectory(External/enet)

  target_include_directories(${PROJECT_NAME}
    PUBLIC Shared
    PUBLIC Client/Include
    PUBLIC External/raylib/src
    PUBLIC External/entt/src
    PUBLIC External/enet/include
    PUBLIC External/imgui
  )

  target_link_directories(${PROJECT_NAME} PUBLIC raylib/raylib PUBLIC enet)

  target_link_libraries(${PROJECT_NAME} raylib enet)

else()

  project(Server)

  add_executable(${PROJECT_NAME} Server/Source/Server.cpp)

  add_subdirectory(External/enet)
  add_subdirectory(External/mingw-std-threads)

  target_include_directories(${PROJECT_NAME}
    PUBLIC Shared
    PUBLIC Server/Include
    PUBLIC External/entt/src
    PUBLIC External/enet/include
    PUBLIC External/mingw_stdthreads
  )

  target_link_directories(${PROJECT_NAME} PUBLIC enet)

  target_link_libraries(${PROJECT_NAME} enet mingw_stdthreads)

endif(IS_CLIENT)
unset(IS_CLIENT CACHE)