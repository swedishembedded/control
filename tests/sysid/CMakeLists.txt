# SPDX-License-Identifier: MIT
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

define_test(sysid)
target_sources(sysid PRIVATE main.cpp)
target_sources(sysid PRIVATE okid_era.cpp)
target_sources(sysid PRIVATE rls.cpp)
target_sources(sysid PRIVATE sqr_ukf_id.cpp)
