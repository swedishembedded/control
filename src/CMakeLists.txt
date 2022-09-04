# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

file(GLOB_RECURSE SOURCES ./**/*.c)

add_library(control STATIC ${SOURCES})

target_compile_options(control PRIVATE -Wall -Wextra -Werror -pedantic)

target_include_directories(control PUBLIC "${CMAKE_SOURCE_DIR}/include")

configure_tidy(control)
