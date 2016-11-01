/**
 * @file		POWER_CORE__SERIAL_NETWORK.C
 * @brief		Serial network interface to the Pi
 * @author		Lachlan Grogan
 * @copyright	rLoop Inc.
 * @st_fileID	LCCM653R0.FILE.004
 */
/**
 * @addtogroup RLOOP
 * @{ */
/**
 * @addtogroup POWER_NODE
 * @ingroup RLOOP
 * @{ */
/**
 * @addtogroup POWER_NODE__SERIAL_NETWORK
 * @ingroup POWER_NODE
 * @{ */


#include "../power_core.h"

#if C_LOCALDEF__LCCM653__ENABLE_THIS_MODULE == 1U

//Pi sending data to power node in struct decParam. Receiving data from Pi and coding RM48
void vPWRNODE_COMM__ReceiveParam(struct rI2CRX_decParam decParam) 
{
#ifndef WIN32
	// make something happen in the power node depending on index
#else
	// testing with emulator
	if (decParam.index == 5) //Check if received value for index is 5. 5 is an arbitrary number
	{
		// TODO: need to check type and then cast to right int size to get right value
		Luint8* t_ptr = (Luint8*)decParam.val; // force compiler to think the data located at val is a 8 bit integer
		Luint8 t = t_ptr; // retrieve the data from that location
		//emit a message
		DEBUG_PRINT("SHUTDOWN");
		if (t == 0) 
		{
			DEBUG_PRINT("SHUTDOWN BATTERY POWER");
		}
		else if (t == 1) 
		{
			DEBUG_PRINT("SHUTDOWN POWER NODE ITSELF");
		}
	}
}

void vPWRNODE_COMM__Process()
{
	//process RX
	Luint8 rx_buffer[I2C_BUFFER_SIZE];
	Luint32 rx_buffer_len;

#ifndef WIN32
	if (u32RM4_SCI__Is__RxReady(SCI_CHANNEL__2))
	{
		//get bytes from serial port
		u8RM4_SCI__RxByteArray(SCI_CHANNEL__2, rx_buf_len, rx_buffer);
	}
#else
	// make up a RX frame for emulator
	rx_buf_len = 8;
	rx_buffer[0] = I2C_CONTROL_CHAR;
	rx_buffer[1] = I2C_FRAME_START;
	rx_buffer[2] = 4; // frame length, 4 bytes
	rx_buffer[3] = 0; // second digit of frame length
	rx_buffer[4] = I2C_FRAME_START;
	rx_buffer[5] = 5; // data we want to send
	rx_buffer[6] = 0; // second digit of data
	rx_buffer[7] = I2C_FRAME_END;
#endif
	//process bytes if there were any read
	if (rx_buf_len > 0)
	{
		rI2CRX_receiveBytes(rx_buffer, rx_buf_len);
	}

	//process TX
	if (rI2CTX_frameLength)
	{
#ifndef WIN32
		// send bytes to serial port
		// no need to check if tx is ready since already did when preparing frame
		vRM4_SCI__TxByteArray(SCI_CHANNEL__2, rI2CTX_frameLength, rI2CTX_buffer);
#else
		Lint32 i;
		for (i = 0; i < rI2CTX_frameLength; i++)
		{
			char ch[3];
			sprintf(ch, "%d,", rI2CTX_buffer[i]);
			DEBUG_PRINT(ch);
		}
#endif
		rI2CTX_frameLength = 0;
	}

}

void vPWRNODE_COMM__Init()
{
	rI2CRX_begin();
	//assigning callback required for rI2C RX. callback will happen when packet is received.
	rI2CRX_recvDecParamCB = vPWRNODE_COMM__ReceiveParam; 
}


#endif //#if C_LOCALDEF__LCCM653__ENABLE_THIS_MODULE == 1U
//safetys
#ifndef C_LOCALDEF__LCCM653__ENABLE_THIS_MODULE
	#error
#endif
/** @} */
/** @} */
/** @} */

