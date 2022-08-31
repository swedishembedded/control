# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

define_test(filter)
target_sources(filter PRIVATE main.cpp)
target_sources(filter PRIVATE filtfilt.cpp)
target_sources(filter PRIVATE mcs.cpp)
target_sources(filter PRIVATE sqr_ukf.cpp)
