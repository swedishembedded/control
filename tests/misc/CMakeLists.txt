# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

define_test(misc)
target_sources(misc PRIVATE main.cpp)
target_sources(misc PRIVATE cat.cpp)
target_sources(misc PRIVATE cut.cpp)
target_sources(misc PRIVATE insert.cpp)
target_sources(misc PRIVATE mean.cpp)
target_sources(misc PRIVATE print.cpp)
target_sources(misc PRIVATE randn.cpp)
target_sources(misc PRIVATE stddev.cpp)
target_sources(misc PRIVATE constrain.cpp)
target_sources(misc PRIVATE sign.cpp)
