// need access to COMMON_CODE/MULTICORE/LCCM650__MULTICORE__ATA6870/ata6870.h
// for vATA6870_CELL__Get_Voltages function and discharge resistor control
// NOT SURE ABOUT THIS v
#include "../../../COMMON_CODE/MULTICORE/LCCM650__MULTICORE__ATA6870/ata6870.h"
// Constant: number of battery cells in each module.
// Needed for BMS__Get_Voltages function
#ifndef NUM_CELLS_PER_MODULE
#define NUM_CELLS_PER_MODULE    (6U)
#endif

// Constant: number of devices in power node.
// Needed for BMS__Get_Voltages function and
// discharge resistor control
#ifndef NUM_DEVICES
#define NUM_DEVICES     (3U)
#endif

struct
{
  Luint8 u8DeviceIndex;
  Lfloat32 pf32Voltages[NUM_CELLS_PER_MODULE];
  Lfloat32 pf32DeviceTemperature;
} sATA6870_Data;

// Array of ATA6870 to represent the total number of devices we have
sATA6870_Data[NUM_DEVICES] Devices;

// Records the voltages of every cell in the above array of structs
void vBMS__Get_Voltages();

// Discharge resistor control //

// Turn on discharge resistor for a given cell on a given device
void vBMS__Turn_On_Disch_Resistor(Luint8 u8DeviceIndex, Luint8 u8CellIndex);

// Turn off all discharge resistors for a given device
void vBMS__Turn_Off_Disch_Resistors(Luint8 u8DeviceIndex);

// Turn off all discharge resistors
void vBMS__Turn_All_Off();

// Set undervoltage threshold. The default should be 3V
void vBMS__Set_Undervoltage_Threshold(Lfloat32 f32Threshold);
