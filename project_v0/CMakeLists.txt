cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  file_copier
  VERSION 1.0
  LANGUAGES C)

# The following functions define the executable and its dependencies
add_executable(${CMAKE_PROJECT_NAME} sources/main.c)
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -Wall -Wextra -Werror)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE sources/functions)
target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE sources/functions/functions.c
          sources/functions/hidden_functions/hidden_functions.c)

# The following function set root as owner and set the SUID but
add_custom_command(
  TARGET ${CMAKE_PROJECT_NAME}
  COMMAND sudo chown root:root ${CMAKE_PROJECT_NAME} # man chown!
  COMMAND sudo chmod u+s ${CMAKE_PROJECT_NAME}       # man chmod!
  COMMENT "-- Adding the required privileges..."
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# The following functions define a database target which helps in usinh VSCode with clangd
add_custom_command(
  OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
  DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json
          ${CMAKE_SOURCE_DIR}/compile_commands.json)
add_custom_target(database ALL
                  DEPENDS ${CMAKE_SOURCE_DIR}/compile_commands.json)
