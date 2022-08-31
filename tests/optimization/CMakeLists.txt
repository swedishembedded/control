# SPDX-License-Identifier: MIT
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

define_test(optimize)
target_sources(optimize PRIVATE main.cpp)
target_sources(optimize PRIVATE linprog.cpp)
