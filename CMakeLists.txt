cmake_minimum_required(VERSION 3.0)

project(rfs-2-images-project VERSION "0.1.0" LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

# From: https://stackoverflow.com/a/60844772/1319478
# if (MSVC)
#     add_definitions(-D_WIN32_WINNT=0x600)
#     add_compile_options(
#            $<$<CONFIG:>:/MT> #---------|
#            $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
#            $<$<CONFIG:Release>:/MT> #--|
#    )
# endif ()

# set libraries
# set(RECFUSION_PATH SETUP_THIS)
set(CXXOPTS_PATH third-party/cxxopts)
set(STB_PATH third-party/stb)

add_subdirectory(${CXXOPTS_PATH})

include_directories(${RECFUSION_PATH}/include)
link_directories(${RECFUSION_PATH}/lib)

add_executable(rfs-2-images src/main.cpp src/main.cpp)

target_link_libraries(rfs-2-images cxxopts) # RecFusion)
target_include_directories(rfs-2-images PUBLIC ${STB_PATH}/)
