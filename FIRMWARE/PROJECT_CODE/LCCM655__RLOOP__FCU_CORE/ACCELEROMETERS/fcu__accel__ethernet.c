/**
 * @file		FCU__ACCEL__ETHERNET.C
 * @brief		Ethernet UDP diagnostics for the accel system
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
 * @addtogroup FCU__ACCELEROMETER_ETHERNET
 * @ingroup FCU
 * @{ */

#include "../fcu_core.h"

#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U
#if C_LOCALDEF__LCCM655__ENABLE_ACCEL == 1U
#if C_LOCALDEF__LCCM655__ENABLE_ETHERNET == 1U


/***************************************************************************//**
 * @brief
 * Transmit acceleration data over ethernet
 *
 * @param[in]		u16PacketType			The type of packet to transmit
 * @st_funcMD5		E90CA00FA79E5384E92D64FCA0D05B8F
 * @st_funcID		LCCM655R0.FILE.030.FUNC.001
 */
void vFCU_ACCEL_ETH__Transmit(E_FCU_NET_PACKET_TYPES ePacketType)
{

	Lint16 s16Return;
	Luint8 * pu8Buffer;
	Luint8 u8BufferIndex;
	Luint16 u16Length;
	Luint8 u8Device;

	pu8Buffer = 0;

	//setup length based on packet.
	switch(ePacketType)
	{
		case FCU_PKT__ACCEL__TX_FULL_DATA:
			u16Length = C_FCU__NUM_ACCEL_CHIPS * 30U;
			break;

		case FCU_PKT__ACCEL__TX_CAL_DATA:
			u16Length = C_FCU__NUM_ACCEL_CHIPS * 10U;
			break;

		default:
			u16Length = 0U;
			break;

	}//switch(ePacketType)

	//pre-comit
	s16Return = s16SAFE_UDP_TX__PreCommit(u16Length, (SAFE_UDP__PACKET_T)ePacketType, &pu8Buffer, &u8BufferIndex);
	if(s16Return == 0)
	{
		//handle the packet
		switch(ePacketType)
		{
			case FCU_PKT__ACCEL__TX_FULL_DATA:
				//30
				for(u8Device = 0U; u8Device < C_FCU__NUM_ACCEL_CHIPS; u8Device++)
				{

					//fault flags
					vNUMERICAL_CONVERT__Array_U32(pu8Buffer, u32MMA8451__Get_FaultFlags(u8Device));
					pu8Buffer += 4U;

					//X Raw
					vNUMERICAL_CONVERT__Array_S16(pu8Buffer, s16MMA8451_FILTERING__Get_Average(u8Device, AXIS_X));
					pu8Buffer += 2U;

					//Y Raw
					vNUMERICAL_CONVERT__Array_S16(pu8Buffer, s16MMA8451_FILTERING__Get_Average(u8Device, AXIS_Y));
					pu8Buffer += 2U;

					//Z Raw
					vNUMERICAL_CONVERT__Array_S16(pu8Buffer, s16MMA8451_FILTERING__Get_Average(u8Device, AXIS_Z));
					pu8Buffer += 2U;

					//X Accel
					vNUMERICAL_CONVERT__Array_F32(pu8Buffer, f32MMA8451_MATH__Get_GForce(u8Device, AXIS_X));
					pu8Buffer += 4U;

					//Y Accel
					vNUMERICAL_CONVERT__Array_F32(pu8Buffer, f32MMA8451_MATH__Get_GForce(u8Device, AXIS_Y));
					pu8Buffer += 4U;

					//Z Accel
					vNUMERICAL_CONVERT__Array_F32(pu8Buffer, f32MMA8451_MATH__Get_GForce(u8Device, AXIS_Z));
					pu8Buffer += 4U;

					//Pitch
					vNUMERICAL_CONVERT__Array_F32(pu8Buffer, f32MMA8451_MATH__Get_PitchAngle(u8Device));
					pu8Buffer += 4U;

					//Roll
					vNUMERICAL_CONVERT__Array_F32(pu8Buffer, f32MMA8451_MATH__Get_RollAngle(u8Device));
					pu8Buffer += 4U;

				}//for(u8Device = 0; u8Device < 3; u8Device++)
				break;

			case FCU_PKT__ACCEL__TX_CAL_DATA:
				for(u8Device = 0U; u8Device < C_FCU__NUM_ACCEL_CHIPS; u8Device++)
				{

					//fault flags
					vNUMERICAL_CONVERT__Array_U32(pu8Buffer, u32MMA8451__Get_FaultFlags(u8Device));
					pu8Buffer += 4U;

					//X Raw
					vNUMERICAL_CONVERT__Array_S16(pu8Buffer, s16MMA8451_FILTERING__Get_Average(u8Device, AXIS_X));
					pu8Buffer += 2U;

					//Y Raw
					vNUMERICAL_CONVERT__Array_S16(pu8Buffer, s16MMA8451_FILTERING__Get_Average(u8Device, AXIS_Y));
					pu8Buffer += 2U;

					//Z Raw
					vNUMERICAL_CONVERT__Array_S16(pu8Buffer, s16MMA8451_FILTERING__Get_Average(u8Device, AXIS_Z));
					pu8Buffer += 2U;

				}
				break;

		}//switch(ePacketType)

		//send it
		vSAFE_UDP_TX__Commit(u8BufferIndex, u16Length, C_LOCALDEF__LCCM528__ETHERNET_PORT_NUMBER, C_LOCALDEF__LCCM528__ETHERNET_PORT_NUMBER);

	}//if(s16Return == 0)
	else
	{
		//fault

	}//else if(s16Return == 0)

}


#endif //C_LOCALDEF__LCCM655__ENABLE_ETHERNET
#endif //C_LOCALDEF__LCCM655__ENABLE_ACCEL
#endif //#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U
//safetys
#ifndef C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE
	#error
#endif
/** @} */
/** @} */
/** @} */
