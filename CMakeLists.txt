cmake_minimum_required(VERSION 3.15)
project(pibt_test)

cmake_policy(SET CMP0091 NEW) # Enable C++17 standard

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src/viewer)

