/**
 * @file		AMC7812__DAC.C
 * @brief		DAC Portion of the device
 * @author		Lachlan Grogan
 * @copyright	This file contains proprietary and confidential information of
 *				SIL3 Pty. Ltd. (ACN 123 529 064). This code may be distributed
 *				under a license from SIL3 Pty. Ltd., and may be used, copied
 *				and/or disclosed only pursuant to the terms of that license agreement.
 *				This copyright notice must be retained as part of this file at all times.
 * @copyright	This file is copyright SIL3 Pty. Ltd. 2003-2016, All Rights Reserved.
 * @st_fileID	LCCM658R0.FILE.007
 */
/**
 * @addtogroup MULTICORE
 * @{ */
/**
 * @addtogroup AMC7812
 * @ingroup MULTICORE
 * @{ */
/**
 * @addtogroup AMC7812__DAC
 * @ingroup AMC7812
 * @{ */

#include "../amc7812.h"
#if C_LOCALDEF__LCCM658__ENABLE_THIS_MODULE == 1U

extern struct _strAMC7812_DAC strAMC7812_DAC;


/***************************************************************************//**
 * @brief
 * Init the DAC portion.
 * 
 * @st_funcMD5		C180710CF0A27FA75265B2D3FA81C0E7
 * @st_funcID		LCCM658R0.FILE.007.FUNC.001
 */
void vAMC7812_DAC__Init(void)
{

	// structure init
	strAMC7812_DAC.eState = AMC7812_DAC_STATE__INIT_DEVICE;
	strAMC7812_DAC.u32LoopCounter = 0U;
	strAMC7812_DAC.u16MaxVoltage = (Luint16)DAC_OUT_MAX_MVOLTS;
	strAMC7812_DAC.u16MinVoltage = (Luint16)DAC_OUT_MIN_MVOLTS;
	strAMC7812_DAC.u8FlagSetDACVoltage = 0U;

}


Luint16 vAMC7812_DAC__Process( void )
{

	// declarations

	Lint16 s16Return;

	// initialize

	s16Return = -1;

	// check flag to set output

	if ( strAMC7812_DAC.u8FlagSetDACVoltage == 1U )
	{
		// change the state

		strAMC7812_DAC.eState = AMC7812_DAC_STATE__SET_VOLTAGE;
	}
	else
	{
		// no action req'd
	}

	// state machine

	switch ( strAMC7812_DAC.eState )
	{
		case AMC7812_DAC_STATE__IDLE:

			s16Return = 0;
			break;

		case AMC7812_DAC_STATE__INIT_DEVICE:

			// reset the device

			// wait in case we came in from clocking out bad I2C data

			vRM4_DELAYS__Delay_mS(10U);

			// software reset

			s16Return = -1;
			s16Return = s16AMC7812_I2C__TxCommand( C_LOCALDEF__LCCM658__BUS_ADDX, AMC7812_DAC_REG__SW_RESET );

			if ( s16Return >= 0 )
			{
				// reset successful, change state

				strAMC7812_DAC.eState = AMC7812_DAC_STATE__IDLE;
			}
			else
			{
				// reset failed, change state

				strAMC7812_DAC.eState = AMC7812_DAC_STATE__ERROR;

			}
			break;

		case AMC7812_DAC_STATE__SET_VOLTAGE:

			// set output pin voltage

			s16Return = s16AMC7812__SetPinVoltage();

			// check for errors

			if ( s16Return >= 0 )
			{
				// successful write, change state to idle, reset flag

				strAMC7812_DAC.eState = AMC7812_DAC_STATE__IDLE;
				strAMC7812_DAC.u8FlagSetDACVoltage = 0U;

			}
			else
			{
				// an error has occurred:

				// change the state to error

				strAMC7812_DAC.eState = AMC7812_DAC_STATE__ERROR;

			}
			break;

		case AMC7812_DAC_STATE__ERROR:

			// an error has occurred

			break;

	}	// end of switch ( strAMC7812_DAC.eState )

	return s16Return;

}


void vAMC7812_SetOutput( void )
{
	// set the flag to set the output voltage

	strAMC7812_DAC.u8FlagSetDACVoltage = 1U;
}



//--- Set the voltage of the specified pin for the given command and conversion factor (to millivolts) ---//

Lint16 s16AMC7812__SetPinVoltage( void )
{
	// declarations

	Lint16 s16Return;
	Luint16 u16OutputVolts;
	Lfloat32 f32ConversionFactor;
	E_AMC7812_DAC_DATA_REG_ADDRESSES eDAC_REG_ADDR;
	Luint16 u16Command;
	Luint16 u16MaxCommandValue;
	Luint16 u16MinCommandValue;
	Luint16 u16MaxVolts;
	Luint16 u16MinVolts;
	E_AMC7812_TASKS eTask;

	// set local variables

	u16Command = strAMC7812_DAC.u16Command;
	u16MaxCommandValue = strAMC7812_DAC.u16MaxCommandValue;
	u16MinCommandValue = strAMC7812_DAC.u16MinCommandValue;
	u16MaxVolts = strAMC7812_DAC.u16MaxVoltage;
	u16MinVolts = strAMC7812_DAC.u16MinVoltage;
	eTask = strAMC7812_DAC.eTask;


	// compute conversion factor (command units to volts)

	f32ConversionFactor = (Lfloat32)( u16MaxVolts -  u16MinVolts ) / (Lfloat32)( u16MaxCommandValue - u16MinCommandValue );

	// Compute required voltage

	u16OutputVolts = (Luint16)( u16Command * f32ConversionFactor );

	// Check that output is within range

	if ( u16OutputVolts > u16MaxVolts )
	{
		u16OutputVolts = u16MaxVolts;
	}
	else
	{
		if ( u16OutputVolts < u16MinVolts )
		{
			u16OutputVolts = u16MinVolts;
		}
		else
		{
			// okay, no action req'd
		}
	}

	// get the address of the output pin data register for the input task

	switch ( eTask )
	{
		case HE1:
			eDAC_REG_ADDR = AMC7812_REG_ADR__DAC_0_DATA;
			break;
		case HE2:
			eDAC_REG_ADDR = AMC7812_REG_ADR__DAC_1_DATA;
			break;
		case HE3:
			eDAC_REG_ADDR = AMC7812_REG_ADR__DAC_2_DATA;
			break;
		case HE4:
			eDAC_REG_ADDR = AMC7812_REG_ADR__DAC_3_DATA;
			break;
		case HE5:
			eDAC_REG_ADDR = AMC7812_REG_ADR__DAC_4_DATA;
			break;
		case HE6:
			eDAC_REG_ADDR = AMC7812_REG_ADR__DAC_5_DATA;
			break;
		case HE7:
			eDAC_REG_ADDR = AMC7812_REG_ADR__DAC_6_DATA;
			break;
		case HE8:
			eDAC_REG_ADDR = AMC7812_REG_ADR__DAC_7_DATA;
			break;
	}	// end of switch(u8TaskNumber)


	// initialize return flag

	s16Return = -1;

	// call I2C write function

	s16Return = s16AMC7812_I2C__WriteU16( C_LOCALDEF__LCCM658__BUS_ADDX, eDAC_REG_ADDR, u16OutputVolts );


	return s16Return;
}


#endif //#if C_LOCALDEF__LCCM658__ENABLE_THIS_MODULE == 1U
//safetys
#ifndef C_LOCALDEF__LCCM658__ENABLE_THIS_MODULE
	#error
#endif
/** @} */
/** @} */
/** @} */

