/**
 * @file		AMC7812.H
 * @brief		AMC7812 header file.
 * @author		Lachlan Grogan
 * @copyright	This file contains proprietary and confidential information of
 *				SIL3 Pty. Ltd. (ACN 123 529 064). This code may be distributed
 *				under a license from SIL3 Pty. Ltd., and may be used, copied
 *				and/or disclosed only pursuant to the terms of that license agreement.
 *				This copyright notice must be retained as part of this file at all times.
 * @copyright	This file is copyright SIL3 Pty. Ltd. 2003-2016, All Rights Reserved.
 * @st_fileID	LCCM658R0.FILE.001
 */

#ifndef _AMC7812_H_
#define _AMC7812_H_
	#include <localdef.h>
	#if C_LOCALDEF__LCCM658__ENABLE_THIS_MODULE == 1U

		/*******************************************************************************
		Defines
		*******************************************************************************/

#define DAC_OUT_MAX_MVOLTS 5000;		// DAC output voltage limit in millivolts
#define DAC_OUT_MIN_MVOLTS 0;		// DAC output voltage limit in millivolts


		// enum type for DAC 16-bit data registers

		typedef enum AMC7812_DAC_DATA_REG_ADDRESSES
		{
			AMC7812_REG_ADR__DAC_0_DATA = 0x33,		// address for AMC7812 DAC-0-OUT pin register (see Table 10, p. 60, AMC7812 Datasheet)
			AMC7812_REG_ADR__DAC_1_DATA = 0x34,
			AMC7812_REG_ADR__DAC_2_DATA = 0x35,
			AMC7812_REG_ADR__DAC_3_DATA = 0x36,
			AMC7812_REG_ADR__DAC_4_DATA = 0x37,
			AMC7812_REG_ADR__DAC_5_DATA = 0x38,
			AMC7812_REG_ADR__DAC_6_DATA = 0x39,
			AMC7812_REG_ADR__DAC_7_DATA = 0x3A,
			AMC7812_REG_ADR__DAC_8_DATA = 0x3B,
			AMC7812_REG_ADR__DAC_9_DATA = 0x3C,
			AMC7812_REG_ADR__DAC_10_DATA = 0x3D,
			AMC7812_REG_ADR__DAC_11_DATA = 0x3E,
		} E_AMC7812_DAC_DATA_REG_ADDRESSES;

		typedef enum
		{
			AMC7812_DAC_REG__SW_RESET = 0x7C,
			AMC7812_DAC_REG__SW_DAC_CLR = 0x55,
			AMC7812_DAC_REG__HW_DAC_CLR_EN_0 = 0x56,
			AMC7812_DAC_REG__HW_DAC_CLR_EN_1 = 0x57
		} E_AMC7812_DAC_CONTROL_REG_ADDRESSES;


		// States for the AMC7812 DAC state machine

		typedef enum
		{

			// do nothing
			AMC7812_DAC_STATE__IDLE = 0U,

			// We are in an error condition
			AMC7812_DAC_STATE__ERROR,

			// init the device, force a reset
			AMC7812_DAC_STATE__INIT_DEVICE,

			// Read the constants from the device
			AMC7812_DAC_STATE__START,

			// Waiting for the start of a conversion
			AMC7812_DAC_STATE__WAITING,

			// Set the output pin voltage
			AMC7812_DAC_STATE__SET_VOLTAGE,

			// Wait for a number of processing loops to expire
			AMC7812_DAC_STATE__WAIT_LOOPS,


		} E_AMC7812_DAC_STATES_T;

		// Task Numbers (input value maps to an output channel)

		typedef enum
		{
			HE1,
			HE2,
			HE3,
			HE4,
			HE5,
			HE6,
			HE7,
			HE8
		} E_AMC7812_TASKS;

		typedef enum
		{
			//
			DAC_0_DATA,
			//
			DAC_1_DATA,
			//
			DAC_2_DATA,
			//
			DAC_3_DATA,
			//
			DAC_4_DATA,
			//
			DAC_5_DATA,
			//
			DAC_6_DATA,
			//
			DAC_7_DATA

		} E_AMC7812_CHANNELS;

		/*******************************************************************************
		Structures
		*******************************************************************************/

		struct _strAMC7812_DAC
		{

			// the current state

			E_AMC7812_DAC_STATES_T eState;

			// counter for the number of main program loops

			Luint32 u32LoopCounter;

			// address for DAC output data register

			E_AMC7812_DAC_DATA_REG_ADDRESSES eDAC_Data_Addx;

			// DAC voltage limits

			Luint16 u16MaxVoltage;
			Luint16 u16MinVoltage;


		};

		/*******************************************************************************
		Function Prototypes
		*******************************************************************************/

		void vAMC7812__Init(void);

		void vAMC7812__Process(void);
		
		//Lowlevel
		void vAMC7812_LOWLEVEL__Init(void);

		//I2C
		void vAMC7812_I2C__Init(void);
		
		// DAC
		void vAMC7812_DAC__Init(void);
		Luint16 vAMC7812_DAC__Process( void );

		Lint16 s16AMC7812_I2C__WriteU16( Luint8 u8DeviceAddx, Luint8 u8RegisterAddx, Luint16 u16Value );
		Lint16 s16AMC7812_I2C__TxCommand( Luint8 u8DeviceAddx, E_AMC7812_DAC_CONTROL_REG_ADDRESSES eRegister );
		Lint16 s16AMC7812__SetPinVoltage( Luint16 u16Command, Luint16 u16MaxCommandValue, Luint16 u16MinCommandValue, E_AMC7812_TASKS eTask );
		
		//ADC
		void vAMC7812_ADC__Init(void);
		
		//setup the GPIO
		void vAMC7812_GPIO__Init(void);
		void vAMC7812_GPIO__Process(void);
		void vAMC7812_GPIO__Test( void );
		



	#endif //#if C_LOCALDEF__LCCM658__ENABLE_THIS_MODULE == 1U
	//safetys
	#ifndef C_LOCALDEF__LCCM658__ENABLE_THIS_MODULE
		#error
	#endif
#endif //_AMC7812_H_

