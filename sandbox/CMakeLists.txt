
file(GLOB_RECURSE sandbox_source_files "src/*.*")

add_executable(Sandbox ${sandbox_source_files})

target_precompile_headers(Sandbox PRIVATE "src/sandboxpch.h")

target_include_directories(Sandbox PRIVATE "src/" "../engine/includes")

target_link_libraries(Sandbox PRIVATE Engine)

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