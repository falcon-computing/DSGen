include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../tools/dse_utils"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../source-opt/tools/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../source-opt/tools/mars_opt/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../source-opt/tools/mars_opt/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../tools/ds_generator/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../jsoncpp/install/include"
)

link_directories(
    "${ROSE_ROOT_DIR}/lib"
    "${Boost_LIBRARY_DIRS}"
    "${JNI_LIBRARY_DIRS}")

add_executable(test_ds_generator
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/ds_generator/unit_test.cpp")

set_target_properties(test_ds_generator PROPERTIES
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

target_link_libraries(test_ds_generator 
                      gtest_main 
                      DsGenerator
		      mars_rose
		      mars_opt_utils
                      Ir
                      IdUpdate
		      legacy_ir
		      legacy_parsers
                      rose
                      ${Boost_LIBRARIES}
                      ${JNI_LIBRARIES}
                      ${JsonCPP_LIBRARIES}
                      ${CMAKE_DL_LIBS}
                      ${CMAKE_THREAD_LIBS_INIT}
 )

add_test(NAME dsgen_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_ds_generator)
