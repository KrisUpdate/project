/*
 * Gpio_ctrl_3570.h
 *
 *  Created on: Mar 4, 2019
 *      Author: bradf
 */

#ifndef _GPIO_CTRL_3570_H_
#define _GPIO_CTRL_3570_H_

extern boolean is_aurix2_recy_on;

typedef enum
{
	P2 = 0,
	IN_ACTIVE,
	P0,
	SC7,
	MAX_STATE,
}t_system_state;

typedef struct
{
	IoExpPort_Pin * pin;
	char*  name;
	boolean is_connected;
}t_pgood_signals;

/** \brief configure/set all outputs to target System State
 *
 * This function set the outputs to needed state for given system state
 */
void SetOutputs_to_System_State(t_system_state state);

#endif /* _GPIO_CTRL_3570_H_ */
