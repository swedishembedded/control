# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

add_executable(ModelTest main.cpp dc_motor.cpp)
target_link_libraries(ModelTest control gtest pthread)
add_test(NAME ModelTest COMMAND ModelTest)
