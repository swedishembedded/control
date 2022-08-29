file(GLOB_RECURSE SOURCES ./**/*.c)

add_library(control STATIC ${SOURCES})

target_include_directories(control PUBLIC "${CMAKE_SOURCE_DIR}/include")
