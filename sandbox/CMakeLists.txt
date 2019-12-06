
file(GLOB_RECURSE sandbox_source_files "src/*.*")

add_executable(Sandbox ${sandbox_source_files})

target_precompile_headers(Sandbox PRIVATE "src/sandboxpch.h")

target_include_directories(Sandbox PRIVATE "/src/" "../engine/includes")

target_link_libraries(Sandbox PRIVATE Engine)