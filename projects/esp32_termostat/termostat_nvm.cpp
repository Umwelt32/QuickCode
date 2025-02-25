#include "termostat_nvm.hpp"
#include "termostat_hardware.hpp"
#include "termostat_utils.hpp"

#include <EEPROM.h>

#define TERMOSTAT_EEPROM_START_ADDR (0x0000)

Termostat_nvm_data_t Termostat_nvm_data_RAM;

static void Termostat_nvm_RestoreDefaults(void);

void Termostat_nvm_Init(void)
{
  uint16_t crc; 
  EEPROM.begin(TERMOSTAT_NVM_EEPROM_SIZE);
  EEPROM.readBytes(TERMOSTAT_EEPROM_START_ADDR, &TERMOSTAT_NVM_STRUCT_RAM,TERMOSTAT_NVM_EEPROM_SIZE);
  crc=Termostat_utils_CalculateU16Crc(&TERMOSTAT_NVM_STRUCT_RAM,&TERMOSTAT_NVM_STRUCT_RAM.CRC_u16);
  if (TERMOSTAT_NVM_STRUCT_RAM.CRC_u16!=crc)
  {
	  Termostat_nvm_RestoreDefaults(); /*reflash block*/
	  Termostat_nvm_WriteAndCommit();
  }
  else
  {
      Termostat_hardware_RS485_SetTimeout(TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_RS485_timeout_u16);
      Termostat_hardware_RS485_SetBaudRate(TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_RS485_baudrate_u16);
      Termostat_hardware_MQ9_Disable(TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_MQ9Disable);
      Termostat_hardware_DHT11_Disable(TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_DHT11Disable);
  }
}

void Termostat_nvm_WriteAndCommit(void)
{
  uint16_t crc;
	crc=Termostat_utils_CalculateU16Crc(&TERMOSTAT_NVM_STRUCT_RAM,&TERMOSTAT_NVM_STRUCT_RAM.CRC_u16);
	TERMOSTAT_NVM_STRUCT_RAM.CRC_u16=crc;
	EEPROM.writeBytes(TERMOSTAT_EEPROM_START_ADDR, &TERMOSTAT_NVM_STRUCT_RAM, TERMOSTAT_NVM_EEPROM_SIZE);
  EEPROM.commit();
}

void Termostat_nvm_Restore(void)
{
  Termostat_nvm_RestoreDefaults();
}

static void Termostat_nvm_RestoreDefaults(void)
{
	uint16_t crc;
	memset(&TERMOSTAT_NVM_STRUCT_RAM,0,TERMOSTAT_NVM_EEPROM_SIZE);
	TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_RS485_baudrate_u16=1200;
  TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_RS485_timeout_u16=1000;
  TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_ip[0]=239;
  TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_ip[1]=0;
  TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_ip[2]=0;
  TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_ip[3]=2;
  TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_port=7171;
  TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_MQ9_LowerTh=1900;
  TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_MQ9_UpperTh=2100;
  TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_MQ9Disable   = 0;
  TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_DHT11Disable = 0;
	crc=Termostat_utils_CalculateU16Crc(&TERMOSTAT_NVM_STRUCT_RAM,&TERMOSTAT_NVM_STRUCT_RAM.CRC_u16);
	TERMOSTAT_NVM_STRUCT_RAM.CRC_u16=crc;
}
