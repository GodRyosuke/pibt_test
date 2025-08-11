find_package(glfw3 CONFIG REQUIRED)
find_package(assimp REQUIRED)
find_package(Boost COMPONENTS uuid operators lexical_cast REQUIRED)
find_package(yaml-cpp REQUIRED)

set(ASSET_PATH ${CMAKE_SOURCE_DIR}/asset/)
set(SHADER_PATH ${PROJECT_SOURCE_DIR}/src/shader/)
configure_file(
  ${PROJECT_SOURCE_DIR}/src/definitions.hpp.in
  ${PROJECT_SOURCE_DIR}/src/definitions.hpp
  @ONLY
)

set(DEPEND_LIBRARIES 
    glfw 
    assimp::assimp
    boost::boost
    yaml-cpp::yaml-cpp
)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(wander_game_engine_core ${SOURCE_FILES})
target_link_libraries(wander_game_engine_core PRIVATE ${DEPEND_LIBRARIES})
target_include_directories(wander_game_engine_core PRIVATE 
  ${CMAKE_SOURCE_DIR}/src
  ${glfw3_INCLUDE_DIR}
)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${DEPEND_LIBRARIES} wander_game_engine_core)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)