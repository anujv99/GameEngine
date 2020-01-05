
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/lua)
add_subdirectory(vendor/freetype)
add_subdirectory(vendor/freetype-gl)

file(GLOB_RECURSE engine_source_files "src/*.*")

add_library(Engine STATIC ${engine_source_files})

target_precompile_headers(Engine PRIVATE "src/enginepch.h")
target_include_directories(Engine PRIVATE "src" "vendor/glfw/include")
target_include_directories(Engine PRIVATE "src" "vendor/glad/include")
target_include_directories(Engine PRIVATE "src" "vendor/prevmath/include")
target_include_directories(Engine PRIVATE "src" "vendor/lua")
target_include_directories(Engine PRIVATE "src" "vendor/freetype/include")
target_include_directories(Engine PRIVATE "src" "vendor/freetype-gl")

target_link_libraries(Engine PRIVATE glfw)
target_link_libraries(Engine PRIVATE GLAD)
target_link_libraries(Engine PRIVATE Lua)
target_link_libraries(Engine PRIVATE freetype)
target_link_libraries(Engine PRIVATE freetype-gl)

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

	option(ENGINE_DIRECTX "enable/disable DirectX" TRUE)
	option(ENGINE_OPENGL "enable/disable OpenGL" TRUE)

	if (ENGINE_OPENGL)
		add_compile_definitions(ENGINE_OPENGL)
	endif()

	if (ENGINE_DIRECTX)
		add_compile_definitions(ENGINE_DIRECTX)
		target_link_libraries(Engine PRIVATE d3d11 d3dcompiler dxgi dxguid comsuppw)
	endif()
elseif(UNIX)
	add_compile_definitions(ENGINE_LINUX)
	add_compile_definitions(ENGINE_OPENGL)
endif()
