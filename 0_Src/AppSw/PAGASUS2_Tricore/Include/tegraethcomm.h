/*
 * Copyright (c) 2017-2019, NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef __TEGRA_ETH_COMM_H_
#define __TEGRA_ETH_COMM_H_

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/** \brief Initial the ethernet communication between Tegra and Aurix
 * \param  none
 * \return None
 */
void TegraEthComm_init(void);

/** \brief Set resume flag to ignore some initial functions
 * \param  none
 * \return None
 */
void TegraEthComm_resume(void);

/** \brief handles the initial and checking functions, should be called periodic
 * \param  none
 * \return None
 */
void TegraEthComm_routine(void);

/** \brief Enable the heartbeat detection
 * \param  none
 * \return None
 */
void TegraEthComm_EnableHeartBeatDetect(void);


/** \brief Disable the heartbeat detection
 * \param  none
 * \return None
 */
void TegraEthComm_DisableHeartBeatDetect(void);


void iperf_bridge_show(void);
void iperf_bridge_udp_init(void);

void iperf_bridge_tcp_init(void);
void iperf_bridge_tcp_close(void);

#endif  // __TEGRA_ETH_COMM_H_
