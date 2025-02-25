#pragma once
#include "Arduino.h"

#define TERMOSTAT_NVM_EEPROM_SIZE (sizeof(Termostat_nvm_data_t))
#define TERMOSTAT_NVM_STRUCT_RAM Termostat_nvm_data_RAM
#define TERMOSTAT_NVM_WIFI_SSID_SIZE (16)
#define TERMOSTAT_NVM_WIFI_PASSWORD_SIZE (16)

typedef struct
{
	char Termostat_Wifi_ssid_au8[TERMOSTAT_NVM_WIFI_SSID_SIZE];          /*16*/
	char Termostat_Wifi_password_au8[TERMOSTAT_NVM_WIFI_PASSWORD_SIZE]; /*32*/
	byte Termostat_Wifi_connect_u8;                                    /*33*/
	uint16_t Termostat_hardware_RS485_baudrate_u16;                   /*35*/
  uint16_t Termostat_hardware_RS485_timeout_u16;                   /*37*/
  byte Termostat_ctrl_udp_ip[4];                                  /*41*/
  uint16_t Termostat_ctrl_udp_port;                              /*43*/
  uint16_t Termostat_hardware_MQ9_UpperTh;                      /*45*/
  uint16_t Termostat_hardware_MQ9_LowerTh;                     /*47*/
  byte Termostat_ctrl_populate_temp_src;                      /*48*/
  byte Termostat_ctrl_populate_temp_dst;                     /*49*/
  byte Termostat_ctrl_populate_temp_tempnr;                 /*50*/
  byte Termostat_ctrl_populate_temp_protocol;              /*51*/
  byte Termostat_hardware_MQ9Disable;                     /*52*/
  byte Termostat_hardware_DHT11Disable;                  /*53*/
	byte Reserved_u8[9u];                                 /*63*/
	uint16_t CRC_u16;                                    /*64*/
}Termostat_nvm_data_t;


extern Termostat_nvm_data_t Termostat_nvm_data_RAM;

void Termostat_nvm_Init(void);
void Termostat_nvm_WriteAndCommit(void);
void Termostat_nvm_Restore(void);

