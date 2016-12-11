/*
 * fcu_core__net__packet_types.h
 *
 *  Created on: Dec 9, 2016
 *      Author: Lachlan
 */

#ifndef RLOOP_LCCM655__RLOOP__FCU_CORE_NETWORKING_FCU_CORE__NET__PACKET_TYPES_H_
#define RLOOP_LCCM655__RLOOP__FCU_CORE_NETWORKING_FCU_CORE__NET__PACKET_TYPES_H_


	/** Enumerated packet types for network transmission for UDP diagnostics */
	typedef enum
	{

		/** no packet type */
		FCU_PKT__NONE = 0x0000U,

		/** enable timed streaming */
		FCU_PKT__ENABLE_STREAMING = 0x0FFFU,

		/** Request from the host to send the Acclelerometer calibration data */
		FCU_PKT__ACCEL__REQUEST_CAL_DATA = 0x1000U,

		/** Transmit the accel calibration data to the host */
		FCU_PKT__ACCEL__TX_CAL_DATA = 0x1001U,

		/** request a full data stream from our devices */
		FCU_PKT__ACCEL__REQUEST_FULL_DATA = 0x1002U,

		/** Transmit the full available accel data to the host */
		FCU_PKT__ACCEL__TX_FULL_DATA = 0x1003U,

		/** enter auto calibration mode */
		FCU_PKT__ACCEL__AUTO_CALIBRATE = 0x1004U,

		/** Fine adjustment on Any */
		FCU_PKT__ACCEL__FINE_ZERO_ADJUSTMENT = 0x1005U,


	}E_FCU_NET_PACKET_TYPES;



#endif /* RLOOP_LCCM655__RLOOP__FCU_CORE_NETWORKING_FCU_CORE__NET__PACKET_TYPES_H_ */
