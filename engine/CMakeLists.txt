
file(GLOB_RECURSE engine_source_files "src/*.*")

add_library(Engine STATIC ${engine_source_files})

target_precompile_headers(Engine PRIVATE "src/enginepch.h")
target_include_directories(Engine PRIVATE "/src/")
#target_compile_options(Engine PRIVATE -m64)

option(ENGINE_LOGGING "enable/disable logging" TRUE)
option(ENGINE_ASSERT "enable/disable assertions" TRUE)
option(ENGINE_UNICODE "ascii or unicode" FALSE)

add_compile_definitions(PREV_GAME_ENGINE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(ENGINE_DEBUG)
endif()

if (ENGINE_LOGGING)
	add_compile_definitions(ENGINE_LOGGING)
endif()

if (ENGINE_ASSERT)
	add_compile_definitions(ENGINE_ASSERT)
endif()

if (ENGINE_UNICODE)
	add_compile_definitions(ENGINE_UNICODE)
endif()

if (WIN32)
	add_compile_definitions(ENGINE_WINDOWS)
elseif(UNIX)
	add_compile_definitions(ENGINE_LINUX)
endif()