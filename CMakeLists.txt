cmake_minimum_required(VERSION 3.10)

# a macro to load dependencies from environment
macro(LoadEnv var desc)
if(DEFINED ENV{${var}})
  set(${var} "$ENV{${var}}")
  message(STATUS "Found ${desc} environment at ${${var}}")
elseif(DEFINED ${var})
  # support passing this variable from commandline with '-D'
  message(STATUS "Found ${desc} environment at ${${var}}")
else()
  message(FATAL_ERROR "Cannot find ${desc} environment, please set ${${var}}")
endif()
endmacro(LoadEnv)

LoadEnv("LLVM_ROOT_DIR"       "LLVM")
LoadEnv("CLANG_ROOT_DIR"      "Clang")
LoadEnv("ROSE_ROOT_DIR"       "ROSE")
LoadEnv("RAPIDJSON_ROOT_DIR"  "RapidJSON")

# first locate clang compiler
set(CMAKE_C_COMPILER "${LLVM_ROOT_DIR}/bin/clang")
set(CMAKE_CXX_COMPILER "${LLVM_ROOT_DIR}/bin/clang++")

project(DsGenerator)

# For FindLLVM and FindBoost
find_package(Boost 1.53 REQUIRED
              COMPONENTS regex filesystem thread system)
find_package(Threads)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
find_package(LLVM REQUIRED)

# For FindJSONCPP
find_package(JsonCPP REQUIRED)
include_directories(${JsonCPP_INCLUDE_DIRS})
link_directories(${JsonCPP_LIBRARY_DIRS})

# Google Test
configure_file(${CMAKE_MODULE_PATH}/FindGoogleTest.cmake googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# find docker executable
find_program(Docker_EXECUTABLE docker)
if(NOT Docker_EXECUTABLE)
  message(FATAL_ERROR "Cannot find the docker executable")
endif()

# default the build type to release
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# get version str from git
execute_process(
    COMMAND git describe --tags --always
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(
    COMMAND git rev-list --count HEAD 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_REV_COUNT
    OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(
    COMMAND git log -1 --format=%cd 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE BUILD_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(BUILD_VERSION "${GIT_VERSION}.${GIT_REV_COUNT}")
message(STATUS "build version: ${BUILD_VERSION}")
message(STATUS "build version date: ${BUILD_DATE}")

set(CMAKE_CXX_FLAGS "-Wall -Wno-c++17-extensions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_VERSION=\"\\\"${BUILD_VERSION}\"\\\"")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_DATE=\"\\\"${BUILD_DATE}\"\\\"")

set(CMAKE_CXX_FLAGS_DEBUG "-DPROJDEBUG=1 -std=c++14 -O0 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-DPROJDEBUG=1 -std=c++14 -O3")

option(COVERAGE "Build with coverage report" OFF)
if(COVERAGE)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# Let's dump all the useful artifacts in build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/merlin"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/merlin/legacy/ir"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/merlin/legacy/parsers"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${ROSE_ROOT_DIR}/include/rose"
    "${JsonCPP_INCLUDE_DIRS}"
    "${RAPIDJSON_ROOT_DIR}"
)

link_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    "${ROSE_ROOT_DIR}/lib"
    "${JsonCPP_LIBRARY_DIRS}"
    "${Boost_LIBRARY_DIRS}"
)

add_executable(ds_generator
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_gen_main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_generator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_pipeline.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_tiling.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_parallel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_bitwidth_opt.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dse_utils.cpp"
)
add_dependencies(ds_generator JsonCPP)

add_executable(ds_generator_test
    "${CMAKE_CURRENT_SOURCE_DIR}/test/unit_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_generator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_pipeline.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_tiling.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_parallel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ds_bitwidth_opt.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dse_utils.cpp"
)
add_dependencies(ds_generator_test JsonCPP)

set_target_properties(ds_generator PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_FLAGS "-DLINUX_COMMANDS -DHAS_DS_MARK \
    -DHAS_SYSTOLIC_FRONTEND -DHAS_SYSTOLIC_EXTRACT -DHAS_DS_LOOP_PARALLEL \
    -DHAS_LOOP_PARALLEL -DHAS_MARS_IR \
    -DHAS_STREAM_IR -DHAS_MEMORY_BURST -DHAS_LOWER_CPP \
    -DHAS_DEPENDENCY_RESOLVE \
    -DHAS_LOWER_SEPARATE -DHAS_COARSE_PIPELINE -DHAS_COARSE_PARALLEL \
    -DHAS_COARSE_PIPELINE_ALTERA -DHAS_DS_PIPELINE -DHAS_BITWIDTH_OPT \
    -DHAS_DS_BITWIDTH_OPT -DHAS_REDUCTION -DHAS_REDUCTION_GENERAL \
    -DHAS_PROGRAM_ANALYSIS -DHAS_ID_UPDATE -DHAS_PREPROCESS \
    -DHAS_FUNCTION_INLINE -DHAS_AUTO_FUNC_INLINE \
    -DHAS_AUTO_DIM_INTERCHANGE \
    -DHAS_KERNEL_WRAPPER -DHAS_KERNEL_PRAGMA_GEN \
    -DHAS_LOOP_TILING -DHAS_ALTERA_PREPROCESS \
    -DHAS_MEMORY_REDUCE -DHAS_MIDEND_PREPROCESS -DHAS_ALTERA_POSTPROCESS \
    -DHAS_STRUCT_DECOMPOSE  -DHAS_COMM_OPT  -DHAS_ARRAY_LINEARIZE \
    -DHAS_KERNEL_SEPARATE -DHAS_POSTWRAP_PROCESS -DHAS_MEMORY_PARTITION \
    -DHAS_PERFORMANCE_ESTIMATION -DHAS_GEN_TOKEN_ID -DHAS_GEN_HIERARCHY \
    -DHAS_ADD_PRAGMA_KERNEL -DHAS_FINAL_CODE_GEN -DHAS_DS_SCOPE -DHAS_DS_RESOURCE_EVAL -DHAS_DS_TILING \
    -DHAS_DS_SKELETON_EXTRACT -DHAS_LINE_BUFFER")

target_link_libraries(ds_generator
  Ir
  IdUpdate 
  KernelSeparate
  mars_rose
  KernelSeparate
  mars_opt_utils
  legacy_ir 
  legacy_parsers
  rose
  ${Boost_LIBRARIES}
  ${JsonCPP_LIBRARIES}
  ${CMAKE_DL_LIBS} 
  ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(ds_generator_test
  gtest_main
  Ir
  IdUpdate 
  KernelSeparate
  mars_rose
  KernelSeparate
  mars_opt_utils
  legacy_ir 
  legacy_parsers
  rose
  ${JsonCPP_LIBRARIES}
  ${CMAKE_DL_LIBS} 
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
)

enable_testing()
add_test(NAME unit_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/ds_generator_test -I${CMAKE_CURRENT_SOURCE_DIR}/test/aes/common ${CMAKE_CURRENT_SOURCE_DIR}/test/aes/aes.cpp)
add_test(NAME fn_test_black_euro COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/ds_generator ${CMAKE_CURRENT_SOURCE_DIR}/test/black_euro/black_euro.cpp)
add_test(NAME fn_test_aes_int COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/ds_generator -I${CMAKE_CURRENT_SOURCE_DIR}/test/aes_int/common ${CMAKE_CURRENT_SOURCE_DIR}/test/aes_int/aes_int.cpp)

# run code coverage
add_custom_target(coverage
    COMMAND lcov --no-external --directory ${PROJECT_SOURCE_DIR} --base-directory ${PROJECT_SOURCE_DIR}/build/ --gcov-tool ${PROJECT_SOURCE_DIR}/llvm-gcov.sh --capture -o mc.info
    COMMAND genhtml mc.info -o output)
