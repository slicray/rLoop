#ifndef _LCCM653__01__FAULT_FLAGS_H_
#define _LCCM653__01__FAULT_FLAGS_H_
/*
 * @fault_index
 * 00
 * 
 * @brief
 * GENERAL 
 * 
 * A general fault has occurred, check the other flags for more information. 
 * 
*/
#define C_LCCM653__CORE__FAULT_INDEX__00				0x00000000U
#define C_LCCM653__CORE__FAULT_INDEX_MASK__00			0x00000001U

/*
 * @fault_index
 * 01
 * 
 * @brief
 * GUARDING_FAULT 
 * 
 * A memory guarding fault has occurred and the Power Node data structure could be
 * corrupt. 
*/
#define C_LCCM653__CORE__FAULT_INDEX__01				0x00000001U
#define C_LCCM653__CORE__FAULT_INDEX_MASK__01			0x00000002U

#endif //#ifndef _LCCM653__FAULT_FLAGS_H_

