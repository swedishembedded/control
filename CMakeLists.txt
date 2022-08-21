# SPDX-License-Identifier: MIT
# Copyright 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/consulting
# Training: https://swedishembedded.com/training
# This is a standalone CMake file for use without Zephyr

cmake_minimum_required(VERSION 3.12.2)

project (control VERSION 1.0.1)

include(CTest)

set(SOURCES
    src/optimization/linprog.c
    src/misc/insert.c
    src/misc/randn.c
    src/misc/cut.c
    src/misc/vmax.c
    src/misc/stddev.c
    src/misc/cat.c
    src/misc/saturation.c
    src/misc/mean.c
    src/misc/sign.c
    src/misc/print.c
    src/misc/vmin.c
    src/ai/Astar.c
    src/ai/inpolygon.c
    src/controller/mpc.c
    src/controller/mrac.c
    src/controller/lqi.c
    src/controller/theta2ss.c
    src/controller/kalman.c
    src/controller/c2d.c
    src/controller/stability.c
    src/motion/motion_profile.c
    src/filter/filtfilt.c
    src/filter/sr_ukf_state_estimation.c
    src/filter/mcs.c
    src/linalg/linsolve_upper_triangular.c
    src/linalg/linsolve_chol.c
    src/linalg/expm.c
    src/linalg/svd_golub_reinsch.c
    src/linalg/pinv.c
    src/linalg/linsolve_qr.c
    src/linalg/eig_sym.c
    src/linalg/mul.c
    src/linalg/tran.c
    src/linalg/nonlinsolve.c
    src/linalg/linsolve_lower_triangular.c
    src/linalg/qr.c
    src/linalg/svd_jacobi_one_sided.c
    src/linalg/sum.c
    src/linalg/dlyap.c
    src/linalg/balance.c
    src/linalg/lup.c
    src/linalg/linsolve_lup.c
    src/linalg/eig.c
    src/linalg/norm.c
)

set(CONTROL_HEADERS
    include/control/ai.h
    include/control/controller.h
    include/control/filter.h
    include/control/linalg.h
    include/control/misc.h
    include/control/optimization.h
    include/control/sysid.h
)

add_library(control STATIC ${SOURCES})

target_include_directories( control
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_subdirectory(tests)

set(DOXYGEN_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/packages.toml.in
    ${CMAKE_BINARY_DIR}/packages.toml
)
configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/contents.toml.in
    ${CMAKE_BINARY_DIR}/contents.toml
)
configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/Doxyfile.in
    ${CMAKE_BINARY_DIR}/Doxyfile
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/build/output/index.adoc
    COMMAND doxygen ${CMAKE_BINARY_DIR}/Doxyfile &&
            asciidoxy --spec-file ${CMAKE_BINARY_DIR}/packages.toml -b adoc ${CMAKE_CURRENT_LIST_DIR}/doc/index.adoc
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/index.pdf
    DEPENDS
        ${CMAKE_CURRENT_LIST_FILE}
        ${CMAKE_BINARY_DIR}/build/output/index.adoc
    COMMAND asciidoctor-pdf
        -a pdf-themesdir=${PROJECT_SOURCE_DIR}/theme/
        -a pdf-theme=tutorial.yml
        -a pdf-fontsdir=${PROJECT_SOURCE_DIR}/theme/fonts
        -a iconsdir=${PROJECT_SOURCE_DIR}/theme/icons
        -r asciidoctor-mathematical
        -d book
        -D ${PROJECT_BINARY_DIR}
        -o index.pdf
        --failure-level=ERROR
        ${CMAKE_BINARY_DIR}/build/output/index.adoc
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(docs DEPENDS ${CMAKE_BINARY_DIR}/index.pdf)
install(TARGETS control
    COMPONENT linapp
    RUNTIME DESTINATION "/usr/"
    LIBRARY DESTINATION "/usr/"
    PUBLIC_HEADER DESTINATION "/control/"
    DESTINATION "/usr/"
)

install(FILES ${CONTROL_HEADERS} DESTINATION include/control COMPONENT devel)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Martin Schröder")
INCLUDE(CPack)
