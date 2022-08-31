# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>

define_test(ai)
target_sources(ai PRIVATE main.cpp)
target_sources(ai PRIVATE a_star.cpp)
target_sources(ai PRIVATE inpolygon.cpp)
