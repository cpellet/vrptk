cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "")
endif()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")

project(VRPTK)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(BUILD_DEPS = ON)
set(BUILD_re2 = ON)
set(BUILD_SCIP = ON)

file(GLOB_RECURSE VRPTK_SRC CONFIGURE_DEPENDS
"${PROJECT_SOURCE_DIR}/src/*.cpp"
"${PROJECT_SOURCE_DIR}/src/*.h"
)

file(GLOB_RECURSE GLIDESCOPE_SRC CONFIGURE_DEPENDS
"${PROJECT_SOURCE_DIR}/glidescope/*.cpp"
"${PROJECT_SOURCE_DIR}/glidescope/*.h"
)

set(HEXALY_INCLUDE_DIRS /opt/localsolver_12_5/include)

include(vcpkg/scripts/buildsystems/vcpkg.cmake)
find_package(Eigen3 CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(Threads REQUIRED)

file(GLOB LIBRARIES "/opt/localsolver_12_5/bin/*.dylib")

include_directories(src glidescope)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_library(vrptk STATIC ${VRPTK_SRC})
target_link_libraries(vrptk PRIVATE Eigen3::Eigen tinyxml2::tinyxml2 glog::glog ${LIBRARIES} Threads::Threads)
target_compile_definitions(vrptk PRIVATE TRUE ${LOCALSOLVER_FLAG})
target_compile_definitions(vrptk PRIVATE LOCALSOLVER_EXISTS=1)
target_include_directories(vrptk PUBLIC ${HEXALY_INCLUDE_DIRS})
target_compile_features(vrptk PRIVATE cxx_std_17)

add_executable(glidescope ${GLIDESCOPE_SRC})
target_link_libraries(glidescope  PRIVATE   vrptk
                                    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
                                    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
                                    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
                                    imgui::imgui
                                    implot::implot
                                    tinyfiledialogs::tinyfiledialogs
                                    glog::glog)
target_compile_features(glidescope PRIVATE cxx_std_17)

add_executable(vrptk_test ${PROJECT_SOURCE_DIR}/test/main.cpp)
target_link_libraries(vrptk_test vrptk)
target_compile_features(vrptk_test PRIVATE cxx_std_17)