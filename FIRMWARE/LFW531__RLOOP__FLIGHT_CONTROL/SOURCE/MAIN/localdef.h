#ifndef LOCALDEF_H_
#define LOCALDEF_H_

	//The PCB's main files
	#include <../../../BOARD_SUPPORT/lpcb235r0__board_support.h>


/*******************************************************************************
ETHERNET TRANSPORT
*******************************************************************************/
	#define C_LOCALDEF__LCCM325__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM325__ENABLE_THIS_MODULE == 1U

		//CPU Support
#ifndef WIN32
		#define C_LOCALDEF__LCCM325__USE_ON_RM4								(1U)
		#define C_LOCALDEF__LCCM325__USE_ON_XILINX							(0U)
		#define C_LOCALDEF__LCCM325__USE_ON_WIN32							(0U)
		#define C_LOCALDEF__LCCM325__USE_ON_MSP430							(0U)
#else
		#define C_LOCALDEF__LCCM325__USE_ON_RM4								(0U)
		#define C_LOCALDEF__LCCM325__USE_ON_XILINX							(0U)
		#define C_LOCALDEF__LCCM325__USE_ON_WIN32							(1U)
		#define C_LOCALDEF__LCCM325__USE_ON_MSP430							(0U)
#endif


		//various protocol options
		//DHCP Client
		#define C_LOCALDEF__LCCM325__ENABLE_DHCP_CLIENT						(0U)
		//Link Layer Discovery Protocol
		#define C_LOCALDEF__LCCM325__ENABLE_LLDP							(0U)

		//UDP Rx
		#define C_LOCALDEF__LCCM325__UDP_RX_CALLBACK(buffer,length,dest_port)	vFCU_NET_RX__RxUDP(buffer, length, dest_port)
		/*vECU_ETHERNET_RX__UDPPacket*/

		//testing options
		#define C_LOCALDEF__LCCM325__ENABLE_TEST_SPEC						(0U)

		//protocol specific options
		//set to 1 to consider port numbers
		#define C_LOCALDEF__LCCM325__PROTO_UDP__ENABLE_PORT_NUMBERS			(1U)

		//main include file
		#include <MULTICORE/LCCM325__MULTICORE__802_3/eth.h>

	#endif //C_LOCALDEF__LCCM325__ENABLE_THIS_MODULE

/*******************************************************************************
SAFETY UDP LAYER
*******************************************************************************/
	#define C_LOCALDEF__LCCM528__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM528__ENABLE_THIS_MODULE == 1U

		/* Architecture Options*/
		#define C_LOCALDEF__LCCM528__USE_ON_XILINX							(0U)
		#define C_LOCALDEF__LCCM528__USE_ON_RM4								(1U)
		#define C_LOCALDEF__LCCM528__USE_ON_WIN32							(0U)

		/** User Rx Callback
		* Payload, Length, Type, DestPort, Faults
		*/
		#define C_LOCALDEF__LCCM528__RX_CALLBACK(p,l,t,d,f)					vFCU_NET_RX__RxSafeUDP(p,l,t,d,f)

		/** The one and only UDP port we can operate on */
		#define C_LOCALDEF__LCCM528__ETHERNET_PORT_NUMBER					(9900U)
		#define C_LOCALDEF__LCCM528__ETHERNET_PORT_NUMBER2					(0U)

		/** Vision over SafeUDP Options */
		#define C_LOCALDEF__LCCM528__VISION__ENABLE_TX						(0U)
		#define C_LOCALDEF__LCCM528__VISION__ENABLE_RX						(0U)
		#define C_LOCALDEF__LCCM528__VISION__MAX_BUFFER_SIZE				(640UL * 480UL * 2UL)


		/** Testing Options */
		#define C_LOCALDEF__LCCM528__ENABLE_TEST_SPEC						(0U)

		/** Main include file */
		#include <MULTICORE/LCCM528__MULTICORE__SAFE_UDP/safe_udp.h>
	#endif //#if C_LOCALDEF__LCCM528__ENABLE_THIS_MODULE == 1U

/*******************************************************************************
AMC7812
*******************************************************************************/
	#define C_LOCALDEF__LCCM658__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM658__ENABLE_THIS_MODULE == 1U

		//I2C Bus Address
		// See Table 8, p. 49 and p. 51, ACM7812 datasheet
		#define C_LOCALDEF__LCCM658__BUS_ADDX								(0xC2)

		/** Num devices on the bus */
		#define C_LOCALDEF__LCCM658__NUM_DEVICES							(1U)

		/** Testing Options */
		#define C_LOCALDEF__LCCM658__ENABLE_TEST_SPEC						(0U)

		/** The number of main program loops to wait for conversion */
		#define C_LOCALDEF__LCCM658__NUM_CONVERSION_LOOPS					(10000U)

		/** Main include file */
		#include <MULTICORE/LCCM658__MULTICORE__AMC7812/amc7812.h>
	#endif //#if C_LOCALDEF__LCCM658__ENABLE_THIS_MODULE == 1U


/*******************************************************************************
RLOOP - PI COMMUNICATIONS MODULE
*******************************************************************************/
	#define C_LOCALDEF__LCCM656__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM656__ENABLE_THIS_MODULE == 1U

		//arch
		#define C_LOCALDEF__LCCM656__USE_ON_RM4								(1U)
		#define C_LOCALDEF__LCCM656__USE_ON_WIN32							(0U)

		/** enable the receiver side? */
		#define C_LOCALDEF__LCCM656__ENABLE_RX								(1U)

		/** Testing Options */
		#define C_LOCALDEF__LCCM656__ENABLE_TEST_SPEC						(0U)

		/** Main include file */
		#include <LCCM656__RLOOP__PI_COMMS/pi_comms.h>
	#endif //#if C_LOCALDEF__LCCM656__ENABLE_THIS_MODULE == 1U

/*******************************************************************************
RLOOP - FLIGHT CONTROL UNIT - CORE
*******************************************************************************/
	#define C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U

		/** Enable or disable the PiComms layer */
		#define C_LOCALDEF__LCCM655__ENABLE_PI_COMMS						(1U)

		/** Enable the OptoNCDT laser interface */
		#define C_LOCALDEF__LCCM655__ENABLE_LASER_OPTONCDT					(1U)

		/** Number of OptoNCDT lasers, must be in order from A0:2, B0:2*/
		#define C_LOCALDEF__LCCM655__NUM_LASER_OPTONCDT						(3U)

		/** Enable accel subsystem */
		#define C_LOCALDEF__LCCM655__ENABLE_ACCEL							(1U)

		/** Enable the braking subsystems */
		#define C_LOCALDEF__LCCM655__ENABLE_BRAKES							(1U)
		#define C_LOCALDEF__LCCM655__ENABLE_DEBUG_BRAKES					(1U)

		/** Enable the throttle control */
		#define C_LOCALDEF__LCCM655__ENABLE_THROTTLE						(0U)

		/** Enable the ASI_RS485 */
		#define C_LOCALDEF__LCCM655__ENABLE_ASI_RS485						(0U)

		/** Enable the pusher detection system */
		#define C_LOCALDEF__LCCM655__ENABLE_PUSHER							(0U)

		/** Ethernet Systems */
		#define C_LOCALDEF__LCCM655__ENABLE_ETHERNET						(1U)

		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__BRAKES_HEADER			(40U)
		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__BRAKE0_ZERO				(41U)
		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__BRAKE0_SPAN				(42U)
		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__BRAKE1_ZERO				(43U)
		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__BRAKE1_SPAN				(44U)
		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__BRAKES_CRC				(45U)

		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP0_VELOC				(46U)
		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP0_ACCEL				(47U)
		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP1_VELOC				(48U)
		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP1_ACCEL				(49U)
		#define C_LOCALDEF__LCCM655__EEPROM_OFFSET__STEP_CRC				(50U)

		/** ADC Sample Limits */
		#define C_LOCALDEF__LCCM655__ADC_SAMPLE__LOWER_BOUND				(300U)
		#define C_LOCALDEF__LCCM655__ADC_SAMPLE__UPPER_BOUND				(3000U)

		/** Testing Options */
		#define C_LOCALDEF__LCCM655__ENABLE_TEST_SPEC						(0U)

		/** Main include file */
		#include <LCCM655__RLOOP__FCU_CORE/fcu_core.h>
	#endif //#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U


#endif /* LOCALDEF_H_ */
