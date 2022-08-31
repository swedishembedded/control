# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

define_test(model)
target_sources(model PRIVATE main.cpp)
target_sources(model PRIVATE dc_motor.cpp)
