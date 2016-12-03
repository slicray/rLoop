/**
 * @file		FCU_CORE__THROTTLE.h
 * @brief		Definitions for throttle layer
 * @author		G. Sweriduk & ...
 * @copyright	rLoop Inc.
 * @st_fileID
 */

// File intent is to address issues listed here: https://github.com/rLoopTeam/eng-software-pod/

//#include "fcu_throttle_data.h"		// contains FCU system variables


#ifndef RLOOP_LCCM655__RLOOP__FCU_CORE_THROTTLE_FCU_THROTTLE_H_
#define RLOOP_LCCM655__RLOOP__FCU_CORE_THROTTLE_FCU_THROTTLE_H_

	#include <localdef.h>
	#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U

		/*******************************************************************************
		Defines
		*******************************************************************************/

		// State types for the THROTTLE state machine
		// see fcu_core.h

/*		typedef enum
		{

			// do nothing
			THROTTLE_STATE__IDLE = 0U,

			// an error was encountered
			THROTTLE_STATE__ERROR,

			// init the device, force a reset
			THROTTLE_STATE__INIT,

			// Ramp command(s) to the HEs
			THROTTLE_STATE__RAMP_COMMAND,

			// Step command(s) to the HEs
			THROTTLE_STATE__STEP_COMMAND,

			// Write throttle command(s) to the DAC, substate of RAMP and STEP
			THROTTLE_STATE__WRITE_TO_DAC,

			// Waiting for the start of a conversion, substate of I/O
			THROTTLE_STATE__WAITING,

			// Issue the conversion command, substate of I/O
			//THROTTLE_STATE__BEGIN_SAMPLE,

			//  No change since last command was given
			THROTTLE_STATE__HOLD


		} E_THROTTLE_STATES_T;
*/

		/*******************************************************************************
		Structures
		*******************************************************************************/

		// see fcu_core.h


		/*******************************************************************************
		Function Prototypes
		*******************************************************************************/
/*	moved to fcu_core.h
		void vFCU_THROTTLE__Init(void);
		void vFCU_THROTTLE__Process(void);

		Lint16 s16FCU_THROTTLE__Step_Command(void);
		Lint16 s16FCU_THROTTLE__Ramp_Command(void);
		Lint16 s16FCU_THROTTLE__Write_HEx_Throttle_Command_to_DAC(Luint16 u16ThrottleCommand, Luint8 u8EngineNumber);
		Lint16 s16FCU_THROTTLE__Write_All_HE_Throttle_Commands_to_DAC(Luint16 u16ThrottleCommand);
		Lint16 s16FCU_THROTTLE__Hold(void);

		void vFCU_THROTTLE__100MS_ISR(void);		// Timer function
 */


	#endif //#if C_LOCALDEF__LCCM647__ENABLE_THIS_MODULE == 1U
	//safetys
	#ifndef C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE
		#error
	#endif



#endif /* RLOOP_LCCM655__RLOOP__FCU_CORE_THROTTLE_FCU_THROTTLE_H_ */
