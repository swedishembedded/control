// SPDX-License-Identifier: MIT
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#pragma once

/**
 * \brief power invariant park transform
 * \param dqz output dq frame
 * \param xyz input in xyz frame
 * \param angle electrical angle
 **/
void park(float *dqz, const float *const xyz, const float angle);

/**
 * \brief power invariant inverse park transform
 * \param dqz input dq frame
 * \param xyz output in xyz frame
 * \param angle electrical angle
 **/
void inv_park(float *xyz, const float *const dqz, const float angle);

/**
 * \brief clarke transform
 * \param xyz output in xyz frame
 * \param abc input phase voltages in rotating frame
 **/
void clarke(float *xyz, const float *const abc);

/**
 * \brief clarke transform
 * \param xyz input in xyz frame
 * \param abc output phase voltages in rotating frame
 **/
void inv_clarke(float *abc, const float *const xyz);
