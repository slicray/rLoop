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
#include "../rI2CRX.h"
#include "../rI2CTX.h"

// buffer to receive data from UART port
Luint8 rx_buffer[I2C_BUFFER_SIZE];
Luint32 rx_buffer_len;

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
#ifndef WIN32
	if (u32RM4_SCI__Is__RxReady(SCI_CHANNEL__2))
	{
		//get bytes from serial port
		u8RM4_SCI__RxByteArray(SCI_CHANNEL__2, rx_buf_len, rx_buffer);
	}

#else
	// TODO read from emulator
	rx_buf_len = 1;
	rx_buffer[0] = 5;
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
		if (u32RM4_SCI__Is_TxReady(SCI_CHANNEL__2))
		{
			// send bytes to serial port
			vRM4_SCI__TxByteArray(SCI_CHANNEL__2, rI2CTX_frameLength, rI2CTX_buffer);
		}
#else
		Lint32 i;
		for (i = 0; i < rI2CTX_frameLength; i++)
		{
			char ch[3];
			sprintf(ch, "%d,", rI2CTX_buffer[i]);
			DEBUG_PRINT(ch);
		}
#endif
	}

}

void vPWRNODE_COMM__Init()
{
	rI2CRX_begin();
	//assigning callback required for rI2C RX. callback will happen when packet is received.
	rI2CRX_recvDecParamCB = vPWRNODE_COMM__ReceiveParam; 
	//reset rx buffer length
	rx_buf_len = 0;
}


#endif //#if C_LOCALDEF__LCCM653__ENABLE_THIS_MODULE == 1U
//safetys
#ifndef C_LOCALDEF__LCCM653__ENABLE_THIS_MODULE
	#error
#endif
/** @} */
/** @} */
/** @} */

