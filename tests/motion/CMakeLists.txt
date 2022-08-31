# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

add_executable(MotionProfileTest src/MotionProfileTest.cpp)
target_link_libraries(MotionProfileTest control gtest pthread)
add_test(NAME MotionProfileTest COMMAND MotionProfileTest)
