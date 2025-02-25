#pragma once
#include "Arduino.h"
#define TERMOSTAT_HARDWARE_SW_VERSION (0x10)
typedef unsigned long hw_time_t;
extern void Termostat_hardware_MQ9_Disable(bool disabled);
extern void Termostat_hardware_DHT11_Disable(bool disabled);
extern int  Termostat_hardware_ReadMQ9_Analog(void);
extern int  Termostat_hardware_ReadMQ9_Digital(void);
extern void Termostat_hardware_WriteAlarm_Digital(int v);
extern int  Termostat_hardware_ReadDHT11(int *temp, int *huminity);
extern void Termostat_hardware_RS485_EN(bool enabled);
extern int  Termostat_hardware_RS485_Write(byte *buf,int len,bool readback);
extern int  Termostat_hardware_RS485_Read(byte *buf,int len,bool writeback);
extern void Termostat_hardware_PowerUp_Init(void);
extern void Termostat_hardware_RS485_SetBaudRate(uint16_t brate);
extern void Termostat_hardware_RS485_SetTimeout(uint16_t timeout);
extern void Termostat_hardware_Mainfunction(void);
extern hw_time_t Termostat_hardware_clock_ms(void);
extern void Termostat_hardware_Reset(void);
extern int Termostat_hardware_coreID(void);
extern void Termostat_hardware_AlarmBeep(uint16_t time_ms);
extern void Termostat_hardware_SleepMs(uint32_t time_ms);
extern void Termostat_hardware_printSensorsInfo(void);
