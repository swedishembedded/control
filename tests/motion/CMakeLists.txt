# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

define_test(motion)
target_sources(motion PRIVATE main.cpp)
target_sources(motion PRIVATE motion_profile.cpp)
