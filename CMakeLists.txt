cmake_minimum_required(VERSION 3.27.1)
project(calaga VERSION 0.1.0
                    DESCRIPTION "A game inspired by Galaga"
                    LANGUAGES C)
set(CMAKE_BUILD_TYPE Debug)
# SETUP VARIABLES:
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE})

#SDL2 Paths
set(SDL2_PATH C:/SDL)
set(SDL2_IMAGE_PATH C:/SDL)
set(SDL2_MIXER_PATH C:/SDL)
set(SDL2_TTF_PATH C:/SDL)

#SDL2 Libs
set(SDL2_LIBRARY C:/SDL/bin/SDL2.dll)
set(SDL2_IMAGE_LIBRARY C:/SDL/bin/SDL2_image.dll)
set(SDL2_MIXER_LIBRARY C:/SDL/bin/SDL2_mixer.dll)
set(SDL2_TTF_LIBRARY C:/SDL/bin/SDL2_ttf.dll)

# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB SRC src/*.c)

message(STATUS "Add source files:")
foreach(SRC_FILE IN LISTS SRC)
    message(STATUS "${SRC_FILE}")
endforeach()
message(STATUS "")

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}
                    SDL2::Main
                    SDL2::Image
                    SDL2::Mixer
                    SDL2::TTF
)


# COPY OVER STATIC FILES
message(STATUS "Copying all assets to destination folder...")
file(
    COPY res
    DESTINATION ${OUTPUT_DIR}
)

set(SDL2_DEPS C:/SDL/bin)
message(STATUS "Copying all dependencies to destination folder...")
file(GLOB PROJECT_DEPENDENCIES ${SDL2_DEPS}/*.dll)

foreach(Dependency IN LISTS PROJECT_DEPENDENCIES)
    file(
        COPY ${Dependency}
        DESTINATION ${OUTPUT_DIR}
    )
endforeach()

# Adds 'make run' that just runs calaga.exe
add_custom_target(run 
                COMMAND calaga
                DEPENDS calaga
                WORKING_DIRECTORY ${OUTPUT_DIR}
)