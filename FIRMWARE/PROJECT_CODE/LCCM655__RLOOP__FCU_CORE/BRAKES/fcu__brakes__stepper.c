/**
 * @file		FCU__BRAKES__STEPPER.C
 * @brief		Stepper interface for the brakes.
 * @author		Lachlan Grogan
 * @copyright	rLoop Inc.
 */
/**
 * @addtogroup RLOOP
 * @{ */
/**
 * @addtogroup FCU
 * @ingroup RLOOP
 * @{ */
/**
 * @addtogroup FCU__BRAKES__STEPPER
 * @ingroup FCU
 * @{ */

#include "../fcu_core.h"

#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U
#if C_LOCALDEF__LCCM655__ENABLE_BRAKES == 1U

//the structure
extern struct _strFCU sFCU;


/***************************************************************************//**
 * @brief
 * Init the stepper interface
 *
 */
void vFCU_BRAKES_STEP__Init(void)
{
	Luint8 u8Test;

	//init any IO as neede

	//setup the stepper drive system
	vSTEPDRIVE__Init();

	//reload the memory
	//check the CRC
	u8Test = u8EEPARAM_CRC__Is_CRC_OK(	C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP0_VELOC,
										C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP1_ACCEL,
										C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP_CRC);
	if(u8Test == 1U)
	{
		//valid
		sFCU.sBrakes[0].sMove.s32LinearVeloc = s32EEPARAM__Read(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP0_VELOC);
		sFCU.sBrakes[0].sMove.s32LinearAccel = s32EEPARAM__Read(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP0_ACCEL);
		sFCU.sBrakes[1].sMove.s32LinearVeloc = s32EEPARAM__Read(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP1_VELOC);
		sFCU.sBrakes[1].sMove.s32LinearAccel = s32EEPARAM__Read(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP1_ACCEL);

	}//if(u8Test == 1U)
	else
	{
		//CRC is invalid
		vEEPARAM__WriteS32(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP0_VELOC, 100U, 1U);
		vEEPARAM__WriteS32(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP0_ACCEL, 100U, 1U);
		vEEPARAM__WriteS32(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP1_VELOC, 100U, 1U);
		vEEPARAM__WriteS32(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP1_ACCEL, 100U, 0U);

		//redo the CRC;
		vEEPARAM_CRC__Calculate_And_Store_CRC(	C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP0_VELOC,
												C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP1_ACCEL,
												C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP_CRC);

		//1. Reload the structures.
		sFCU.sBrakes[0].sMove.s32LinearVeloc = s32EEPARAM__Read(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP0_VELOC);
		sFCU.sBrakes[0].sMove.s32LinearAccel = s32EEPARAM__Read(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP0_ACCEL);
		sFCU.sBrakes[1].sMove.s32LinearVeloc = s32EEPARAM__Read(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP1_VELOC);
		sFCU.sBrakes[1].sMove.s32LinearAccel = s32EEPARAM__Read(C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP1_ACCEL);

		//set the flags for a general fault and cal data reload fault.
		vFAULTTREE__Set_Flag(&sFCU.sBrakes[0].sFaultFlags, C_LCCM655__BRAKES__FAULT_INDEX__00);
		vFAULTTREE__Set_Flag(&sFCU.sBrakes[0].sFaultFlags, C_LCCM655__BRAKES__FAULT_INDEX__03);
		vFAULTTREE__Set_Flag(&sFCU.sBrakes[1].sFaultFlags, C_LCCM655__BRAKES__FAULT_INDEX__00);
		vFAULTTREE__Set_Flag(&sFCU.sBrakes[1].sFaultFlags, C_LCCM655__BRAKES__FAULT_INDEX__03);


	}//else if(u8Test == 1U)

}

/***************************************************************************//**
 * @brief
 * Process any brake commands
 *
 */
void vFCU_BRAKES_STEP__Process(void)
{


}


/***************************************************************************//**
 * @brief
 * Returns the current position of the lead screw for the brakes
 *
 */
Lint32 s32FCU_BRAKES__Get_CurrentPos(E_FCU__BRAKE_INDEX_T eBrake)
{
	return s32STEPDRIVE_POSITION__Get_Position((Luint8)eBrake);
}

/***************************************************************************//**
 * @brief
 * Move the brakes as a pair to a position based on the lead screw distance
 *
 */
void vFCU_BRAKES_STEP__Move(Lint32 s32Brake0Pos, Lint32 s32Brake1Pos)
{
	Lint16 s16Return;

	//The poistion to move to in microns
	Lint32 s32Pos[2];

	//The linear velocity for the move
	Lint32 s32Velocity[2];

	//The linear accel for the move.
	Lint32 s32Accel[2];

	/** Unique ID for each task */
	Luint32 u32TaskID;


	//format the move
	s32Pos[0] = s32Brake0Pos;
	s32Pos[1] = s32Brake1Pos;

	s32Velocity[0] = sFCU.sBrakes[0].sMove.s32LinearVeloc;
	s32Velocity[1] = sFCU.sBrakes[1].sMove.s32LinearVeloc;

	s32Accel[0] = sFCU.sBrakes[0].sMove.s32LinearAccel;
	s32Accel[1] = sFCU.sBrakes[1].sMove.s32LinearAccel;

	//just a dummy task ID
	u32TaskID = 1U;

	//clear the prev task if needed.
	vSTEPDRIVE__Clear_TaskComplete();

	//command the stepper to actual position, it will start moving based on timer interrupts
	//it is OK to do address of near here because we copy into the move planner in this call.
	s16Return = s16STEPDRIVE_POSITION__Set_Position(&s32Pos[0], &s32Velocity[0], &s32Accel[0], u32TaskID);

	//check the return to see if we were able to move.


}

#endif //C_LOCALDEF__LCCM655__ENABLE_BRAKES
#endif //#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U
//safetys
#ifndef C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE
	#error
#endif
/** @} */
/** @} */
/** @} */

