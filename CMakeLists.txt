cmake_minimum_required(VERSION 3.16)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Engine)
project(Sandbox)

add_subdirectory(engine)
add_subdirectory(sandbox)

set(VS_STARTUP_PROJECT Sandbox)
