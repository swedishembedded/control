# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>

define_test(dynamics)
target_sources(dynamics PRIVATE main.cpp)
target_sources(dynamics PRIVATE c2d.cpp)
target_sources(dynamics PRIVATE kalman.cpp)
target_sources(dynamics PRIVATE lqi.cpp)
target_sources(dynamics PRIVATE mpc.cpp)
target_sources(dynamics PRIVATE mrac.cpp)
target_sources(dynamics PRIVATE pid.cpp)
target_sources(dynamics PRIVATE stability.cpp)
target_sources(dynamics PRIVATE theta2ss.cpp)
