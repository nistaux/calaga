cmake_minimum_required(VERSION 3.27.1)
project(calaga VERSION 0.1.0
                    DESCRIPTION "A game inspired by Galaga"
                    LANGUAGES C)

# SETUP VARIABLES:
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

# NOTE: Folders here contain both SDL2 and SDL2_image files.
set(SDL2_LIB_DIR C:/SDL/lib)
set(SDL2_INCLUDE_DIR C:/SDL/include)
set(SDL2_LIBRARY C:/SDL/bin)

# SETUP INCLUDE AND LINK DIRS:
include_directories(
    ${SDL2_INCLUDE_DIR}
)

link_directories(
    ${SDL2_LIB_DIR}
)


# INCLUDE FILES THAT SHOULD BE COMPILED:
file(GLOB_RECURSE SRC src/*.c src/*.h)

message(STATUS "Add source files:")
foreach(SRC_FILE IN LISTS SRC)
    message(STATUS "${SRC_FILE}")
endforeach()
message(STATUS "")

add_executable(${PROJECT_NAME} ${SRC})


# LINK PROGRAM
target_link_libraries(
        ${PROJECT_NAME}
        mingw32
        SDL2main
        SDL2_ttf
        SDL2_image
        SDL2_mixer
        SDL2
)


# COPY OVER STATIC FILES
message(STATUS "Copying all assets to destination folder...")
file(
    COPY res
    DESTINATION ${OUTPUT_DIR}
)

message(STATUS "Copying all dependencies to destination folder...")
file(GLOB PROJECT_DEPENDENCIES ${SDL2_LIBRARY}/*.dll)

foreach(Dependency IN LISTS PROJECT_DEPENDENCIES)
    file(
        COPY ${Dependency}
        DESTINATION ${OUTPUT_DIR}
    )
endforeach()