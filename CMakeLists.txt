# SPDX-License-Identifier: MIT
# Copyright 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/consulting
# Training: https://swedishembedded.com/training
# This is a standalone CMake file for use without Zephyr

cmake_minimum_required(VERSION 3.12.2)

project (control VERSION 0.1.1)

include(CTest)

add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(tests)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.34)")

install(TARGETS control
    COMPONENT linapp
    RUNTIME DESTINATION "/usr/"
    LIBRARY DESTINATION "/usr/"
    PUBLIC_HEADER DESTINATION "/control/"
    DESTINATION "/usr/"
)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/control DESTINATION include COMPONENT devel)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Martin Schröder")
INCLUDE(CPack)
