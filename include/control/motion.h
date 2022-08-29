// SPDX-License-Identifier: MIT
/*
 * Copyright 2022 Martin Schröder <info@swedishembedded.com>
 * Consulting: https://swedishembedded.com/go
 * Training: https://swedishembedded.com/tag/training
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/** Motion profile state */
typedef enum {
	/** Uninitialized profile */
	MOTION_PROFILE_UNINITIALIZED = 0,
	/** Motion profile is completed */
	MOTION_PROFILE_COMPLETED,
	/** Motion profile is accelerating (has not reached maximum speed yet) */
	MOTION_PROFILE_ACCELERATING,
	/** Motion profile is in cruising state (at maximum velocity */
	MOTION_PROFILE_CRUISING,
	/** Motion profile is decelerating (approaching final setpoint) */
	MOTION_PROFILE_DECELERATING,
	/** Motion profile is in an invalid state (must be initialized again!) */
	MOTION_PROFILE_INVALID,
} motion_profile_state_t;

struct motion_profile {
	float acc;
	float dec;
	float vmax;
	float start_time;
	float sign;
	float t1, t2, t3;
	float d1, d2, d3;
	float v1, v2, v3;
	float total_time;
	float total_distance;
	float start_position;
	bool move_planned;
};

/**
 * \brief Initialize a motion profile
 * \param[in] self Motion profile state
 * \param[in] max_acc Maximum acceleration (absolute value)
 * \param[in] max_speed Maximum velocity while moving
 * \param[in] max_dec Maximum rate of deceleration at the end
 **/
void motion_profile_init(struct motion_profile *self, float max_acc, float max_speed,
			 float max_dec);

/**
 * \brief Generates a motion plan
 * \param[in] self Motion profile state
 * \param[in] current_pos Starting position
 * \param[in] current_vel Starting velocity
 * \param[in] target_pos Destination position
 * \param[in] target_vel Destination velocity
 **/
void motion_profile_plan_move(struct motion_profile *self, float current_pos, float current_vel,
			      float target_pos, float target_vel);

/**
 * \brief Retrieves position, velocity and acceleration setpoint for time
 * \param[in] self Motion profile state
 * \param[in] time Positive time since move start
 * \param[out] pos_sp current position setpoint
 * \param[out] vel_sp current velocity setpoint
 * \param[out] acc_sp current acceleration setpoint
 * \returns state of current profile
 **/
motion_profile_state_t motion_profile_get_pva(struct motion_profile *self, float time,
					      float *pos_sp, float *vel_sp, float *acc_sp);

/**
 * \brief Retrieves total profile traversal time given init settings
 * \returns positive traversal time
 **/
float motion_profile_get_traversal_time(struct motion_profile *self);
