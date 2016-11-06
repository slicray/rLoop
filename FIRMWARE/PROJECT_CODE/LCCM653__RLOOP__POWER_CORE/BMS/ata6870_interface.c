#include "ata6870_interface.h"

void vBMS__Get_Voltages()
{
  // We modify the content of each struct (each device)
  for (Luint8 u8LoopCounter = 0; u8LoopCounter < NUM_DEVICES; u8LoopCounter++)
    CELL__GetVoltages(Devices[u8LoopCounter].u8DeviceIndex, 
                      &Devices[u8LoopCounter].pf32Voltages, 
                      &Devices[u8LoopCounter].pf23DeviceTemperature);
}

// Turn on a discharge resistor of a given cell
void vBMS__Turn_On_Disch_Resistor(Luint8 u8DeviceIndex, Luint8 u8CellIndex)
{
    vATA6870_RES__TurnOn(u8DeviceIndex, u8CellIndex);
}

// Turn off the discharge resistors on a given device
void vBMS__Turn_Off_Disch_Resistor(Luint8 u8DeviceIndex)
{
    for (Luint8 u8LoopCounter = 0; u8LoopCounter < NUM_CELLS_PER_MODULE; u8LoopCounter++)
	    vATA6870_RES__TurnOff(u8DeviceIndex, u8LoopCounter);		
}

//Turn off all discharge resistors
void vBMS__Turn_All_Off()
{
    for(Luint8 u8LoopCounter = 0; u8LoopCounter < NUM_DEVICES; u8LoopCounter++)
	    vATA6870_RES__TurnAllOff(Devices[u8LoopCounter].u8DeviceIndex);	
}

// Set undervoltage threshold for a given device
void vBMS__Set_Undervoltage_Threshold(Lfloat32 f32Threshold)
{
    for (Luint8 u8LoopCounter = 0; u8LoopCounter < NUM_DEVICES; u8LoopCounter++)
        vATA6870_UV__Set_UVTrip(Devices[u8LoopCounter].u8DeviceIndex, f32Threshold);
}