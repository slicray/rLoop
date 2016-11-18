/**
 * @file		FCU__LASER_OPTO.C
 * @brief		OptoNCDT laser interface for pitch and yaw
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
 * @addtogroup FCU__LASER_OPTO
 * @ingroup FCU
 * @{ */

#include "../fcu_core.h"

#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U
#if C_LOCALDEF__LCCM655__ENABLE_LASER_OPTONCDT == 1U

//the structure
extern struct _strFCU sFCU;

//locals
static void vFCU_LASEROPTO__Process_Packet(Luint8 u8LaserIndex);
static void vFCU_LASEROPTO__Append_Byte(Luint8 u8LaserIndex, Luint8 u8Value);

/***************************************************************************//**
 * @brief
 * Init any variables
 *
 */
void vFCU_LASEROPTO__Init(void)
{
	Luint8 u8Counter;

	sFCU.sLasers.eOptoNCDTState = OPTOLASER_STATE__RESET;

	//reset everything that is needed
	for(u8Counter = 0U; u8Counter < C_LOCALDEF__LCCM655__NUM_LASER_OPTONCDT; u8Counter++)
	{
		sFCU.sLasers.sOptoLaser[u8Counter].eRxState = OPTONCDT_RX__BYTE_1;
		sFCU.sLasers.sOptoLaser[u8Counter].u8NewPacket = 0U;
		sFCU.sLasers.sOptoLaser[u8Counter].u8NewDistanceAvail = 0U;
		//just set to some obscene distance
		sFCU.sLasers.sOptoLaser[u8Counter].f32Distance = 99999.9F;
	}

}


/***************************************************************************//**
 * @brief
 * Process any laser opto tasks
 * 
 */
void vFCU_LASEROPTO__Process(void)
{
	Luint8 u8Counter;
	Luint8 u8Temp;
	Luint8 u8BurstCount;

	//handle the optoNCDT laser state
	switch(sFCU.sLasers.eOptoNCDTState)
	{
		case OPTOLASER_STATE__RESET:
			//just fresh out of reset.

			//setup the lasers
			sFCU.sLasers.eOptoNCDTState = OPTOLASER_STATE__INIT_LASER;
			break;

		case OPTOLASER_STATE__INIT_LASER:

			//sucks but we have had to set the profile called rLoop into each sensor
			//put the lasers into RS422 mode.
			for(u8Counter = 0U; u8Counter < C_LOCALDEF__LCCM655__NUM_LASER_OPTONCDT; u8Counter++)
			{
				//send the command
				vSC16__Tx_ByteArray(u8Counter, "\r\nOUTPUT RS422\r\n", 16U);
			}
			//todo:
			//Check that the laser is in the mode.
			sFCU.sLasers.eOptoNCDTState = OPTOLASER_STATE__WAIT_INIT_DONE;
			break;

		case OPTOLASER_STATE__WAIT_INIT_DONE:
			//wait until the laser is up


			//continue to check for new data.
			sFCU.sLasers.eOptoNCDTState = OPTOLASER_STATE__CHECK_NEW_DATA;
			break;

		case OPTOLASER_STATE__CHECK_NEW_DATA:

			//do a sneeky burst here
			for(u8BurstCount = 0U; u8BurstCount < 3U; u8BurstCount++)
			{

				//check if any new laser data is available on the bus.
				for(u8Counter = 0U; u8Counter < C_LOCALDEF__LCCM655__NUM_LASER_OPTONCDT; u8Counter++)
				{
					//see if there is at least one byte of data avail in the FIFO's
					u8Temp = u8SC16_USER__Get_ByteAvail(u8Counter);
					if(u8Temp == 0U)
					{
						//no new data
					}
					else
					{
						//yep some new laser data avail, what to do with it?

						//get the byte and send it off for processing if we have enough data
						u8Temp = u8SC16_USER__Get_Byte(u8Counter);

						//process the byte.
						vFCU_LASEROPTO__Append_Byte(u8Counter, u8Temp);
					}

				}
			}


			sFCU.sLasers.eOptoNCDTState = OPTOLASER_STATE__CHECK_NEW_PACKET;
			break;

		case OPTOLASER_STATE__CHECK_NEW_PACKET:

			//check the packet state of each laser
			for(u8Counter = 0U; u8Counter < C_LOCALDEF__LCCM655__NUM_LASER_OPTONCDT; u8Counter++)
			{
				if(sFCU.sLasers.sOptoLaser[u8Counter].u8NewPacket == 1U)
				{
					//we have a new laser packet, process it for distance or error code.
					vFCU_LASEROPTO__Process_Packet(u8Counter);

					//clear the flag
					sFCU.sLasers.sOptoLaser[u8Counter].u8NewPacket = 0U;

				}
				else
				{
					//no new packet has arrived, loop around
				}
			}

			//back to check for more data
			sFCU.sLasers.eOptoNCDTState = OPTOLASER_STATE__CHECK_NEW_DATA;
			break;

	}//switch(sFCU.sLasers.eOptoNCDTState)

}

//get a lasers distance
Lfloat32 f32FCU_LASEROPTO__Get_Distance(Luint8 u8LaserIndex)
{
	return sFCU.sLasers.sOptoLaser[u8LaserIndex].f32Distance;
}

//Process the laser packet
void vFCU_LASEROPTO__Process_Packet(Luint8 u8LaserIndex)
{
	Lfloat32 f32Temp;
	Luint32 u32ValA;
	Luint32 u32ValB;
	Luint32 u32ValC;

	//protect the laser index
	if(u8LaserIndex < C_LOCALDEF__LCCM655__NUM_LASER_OPTONCDT)
	{

		//assemble
		u32ValA = sFCU.sLasers.sOptoLaser[u8LaserIndex].u8NewByteArray[0];
		u32ValB = sFCU.sLasers.sOptoLaser[u8LaserIndex].u8NewByteArray[1];
		u32ValC = sFCU.sLasers.sOptoLaser[u8LaserIndex].u8NewByteArray[2];

		//format
		u32ValA += u32ValB << 6U;
		u32ValA += u32ValC << 12U;

		//convert
		f32Temp = (Lfloat32)u32ValA;
		f32Temp *= 102.0F;
		f32Temp /= 65520.0F;
		f32Temp -= 1.0F;
		f32Temp *= 50.0F;
		f32Temp /= 100.0F;

		//save off the distance.
		sFCU.sLasers.sOptoLaser[u8LaserIndex].f32Distance = f32Temp;

		//save off.
		sFCU.sLasers.sOptoLaser[u8LaserIndex].u8NewDistanceAvail = 1U;

	}
	else
	{
		//laser addressing error
	}


}

//append a newly received byte of data onto the laser subsystem
/* 8.2 Measurement Data Format
 * 18 bits are transmitted per output value, see Chap. 7.5.2. An output value is divided into three bytes
 * that differ in the two most significant bits. The transmission of additional output values is optional.
 * Output value 1 / additional:
 *
 * L-Byte 	0 	0 	D5 	D4 	D3 	D2 	D1 	D0
 * M-Byte 	0 	1 	D11 D10 D9 	D8 	D7 	D6
 * H-Byte 	1 	0(2)0(3) 0(3) D15 D14 D13 D12
 * Output sequence: L-Byte, M-Byte, H-Byte.
 * 1, 3) Error values are coded with 18 Bit.
 * 2) To decide between the 1st output value and additional output values, bit 7 in the H-Byte is set to 1. Bit 7 in
 * the H-Byte is set to 0 for the 1st output value. This simultaneously represents the identifier of a new block. De-
 * pending on the measuring rate, baud rate and output data rate output all data can be output in one block. If
 * data output is not possible, a run-time error will be output. Use the command GETOUTINFO_RS422 to query
 * for data selection and output sequence.
 */
void vFCU_LASEROPTO__Append_Byte(Luint8 u8LaserIndex, Luint8 u8Value)
{
	//protect the laser structures from accidential addressing.
	if(u8LaserIndex < C_LOCALDEF__LCCM655__NUM_LASER_OPTONCDT)
	{
		switch(sFCU.sLasers.sOptoLaser[u8LaserIndex].eRxState)
		{
			case OPTONCDT_RX__BYTE_1:

				//make sure the first two bits are valid
				if((u8Value & 0xC0U) == 0U)
				{
					//the top two bits are zero, we are good to go
					//save the byte
					sFCU.sLasers.sOptoLaser[u8LaserIndex].u8NewByteArray[0] = u8Value & 0x3FU;

					//wait for byte 2
					sFCU.sLasers.sOptoLaser[u8LaserIndex].eRxState = OPTONCDT_RX__BYTE_2;
				}
				else
				{
					//we are not at the right point for detection of the packet start, loop back

					//todo: check if we can see a laser error here
				}

				break;
			case OPTONCDT_RX__BYTE_2:

				//check for byte 1
				if((u8Value & 0xC0U) == 0x40U)
				{
					//the top two bits are 1, we are good to go
					//save the byte
					sFCU.sLasers.sOptoLaser[u8LaserIndex].u8NewByteArray[1] = u8Value & 0x3FU;

					//wait for byte 3
					sFCU.sLasers.sOptoLaser[u8LaserIndex].eRxState = OPTONCDT_RX__BYTE_3;
				}
				else
				{
					//go back to the start, becase we have lost our position
					sFCU.sLasers.sOptoLaser[u8LaserIndex].eRxState = OPTONCDT_RX__BYTE_1;
				}
				break;

			case OPTONCDT_RX__BYTE_3:

				//check for byte 1
				if((u8Value & 0xC0U) == 0x80U)
				{
					//the top two bits are valid, we are good to go
					//save the byte
					sFCU.sLasers.sOptoLaser[u8LaserIndex].u8NewByteArray[2] = u8Value & 0x0FU;

					//signal that a new packet is ready
					sFCU.sLasers.sOptoLaser[u8LaserIndex].u8NewPacket = 1U;

					//go back and rx the next new packet
					sFCU.sLasers.sOptoLaser[u8LaserIndex].eRxState = OPTONCDT_RX__BYTE_1;
				}
				else
				{
					//go back to the start, becase we have lost our position
					sFCU.sLasers.sOptoLaser[u8LaserIndex].eRxState = OPTONCDT_RX__BYTE_1;
				}

				break;

			default:
				//todo, log the error
				break;
		}//switch(sFCU.sOptoLaser[u8LaserIndex].eRxState)
	}
	else
	{
		//some major error
	}
}





#endif //C_LOCALDEF__LCCM655__ENABLE_LASER_OPTONCDT

#endif //#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U
//safetys
#ifndef C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE
	#error
#endif
/** @} */
/** @} */
/** @} */

