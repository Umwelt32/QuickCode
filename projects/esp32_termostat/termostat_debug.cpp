#include "termostat_debug.hpp"
#include "termostat_hardware.hpp"
#include "termostat_utils.hpp"
#include "termostat_wifi.hpp"
#include "termostat_nvm.hpp"
#include "termostat_ctrlnode.hpp"
#include "termostat_watchdog.hpp"
#include "Arduino.h"
#include <cstdio>
#include <cstring>

#define TERMOSTAT_DEBUG_BUFFER_SIZE (64)
#define TERMOSTAT_DEBUG_OUT_BUFFER_SIZE (256)
#define TERMOSTAT_DEBUG_OUT_BUFFER_FLUSH_SIZE (128)
#define TERMOSTAT_DEBUG_OUT_BUFFER_CHUNK_SIZE (52)
#define TERMOSTAT_DEBUG_OUT_BUFFER_SIZE_V2 (TERMOSTAT_DEBUG_OUT_BUFFER_SIZE+TERMOSTAT_DEBUG_OUT_BUFFER_FLUSH_SIZE)

#if (TERMOSTAT_DEBUG_EN)
static char Termostat_debug_serial_output_buffer[TERMOSTAT_DEBUG_OUT_BUFFER_SIZE_V2];
static char Termostat_debug_cyclic_buffer[TERMOSTAT_DEBUG_BUFFER_SIZE];
static hw_time_t Termostat_debug_last_TS = 0;
static hw_time_t Termostat_debug_last_delta_TS = 0;
static hw_time_t Termostat_debug_last_delta_max_TS = 0;
static void Termostat_debug_parse_input(char *input,int size);
static void Termostat_debug_serial_input(void);
#endif

void Termostat_debug_Init(void)
{
    #if (TERMOSTAT_DEBUG_EN)
        int c = Termostat_hardware_coreID();
        TERMOSTAT_DEBUG_FNC("[Core:%d]Debug Console Init\n",c);
        TERMOSTAT_DEBUG_FNC("Write 'help' to get commands list\n");
    #endif
}

void Termostat_debug_WriteCommandBuffer(void *buffer,int len)
{
  #if (TERMOSTAT_DEBUG_EN)
  if (buffer!=nullptr)
  {
    memset(&Termostat_debug_cyclic_buffer[0],0,TERMOSTAT_DEBUG_BUFFER_SIZE);
    memcpy(&Termostat_debug_cyclic_buffer[0],buffer,(len<TERMOSTAT_DEBUG_BUFFER_SIZE)?len:TERMOSTAT_DEBUG_BUFFER_SIZE);
  }
  #endif
}

void Termostat_debug_Mainfunction(void)
{
    #if (TERMOSTAT_DEBUG_EN)
    hw_time_t current_ts = Termostat_hardware_clock_ms();
    Termostat_debug_last_delta_TS = (current_ts - Termostat_debug_last_TS);
    Termostat_debug_last_TS = current_ts;
    if (Termostat_debug_last_delta_max_TS<Termostat_debug_last_delta_TS)Termostat_debug_last_delta_max_TS=Termostat_debug_last_delta_TS;
    Termostat_debug_serial_input();
    Termostat_debug_parse_input(&Termostat_debug_cyclic_buffer[0],TERMOSTAT_DEBUG_BUFFER_SIZE);
    #endif
}

void Termostat_debug_out_buffer(void *out,byte *len)
{
  #if (TERMOSTAT_DEBUG_EN)
  static uint16_t step_u16 = 0;
  if(out!=nullptr)memcpy(out,&Termostat_debug_serial_output_buffer[step_u16],TERMOSTAT_DEBUG_OUT_BUFFER_CHUNK_SIZE);
  if(len!=nullptr)(*len)=TERMOSTAT_DEBUG_OUT_BUFFER_CHUNK_SIZE;
  memset(&Termostat_debug_serial_output_buffer[step_u16],0,TERMOSTAT_DEBUG_OUT_BUFFER_CHUNK_SIZE);
  step_u16+=TERMOSTAT_DEBUG_OUT_BUFFER_CHUNK_SIZE;
  if (step_u16>=TERMOSTAT_DEBUG_OUT_BUFFER_SIZE)step_u16=0;
  #endif
}

void Termostat_debug_dbgfnc(const char *fmt,...)
{
  #if (TERMOSTAT_DEBUG_EN)
  static int buffer_pos = 0;
  char *b = (char*)&Termostat_debug_serial_output_buffer[buffer_pos];
  va_list args;
  va_start(args, fmt);
  int rc = vsnprintf((char*)b,TERMOSTAT_DEBUG_OUT_BUFFER_FLUSH_SIZE, fmt, args);
  va_end(args);
  buffer_pos+=rc;
  if (buffer_pos>TERMOSTAT_DEBUG_OUT_BUFFER_SIZE)buffer_pos=0;
  printf("%s",b);
  #endif
}

static void Termostat_debug_serial_input(void)
{
  #if (TERMOSTAT_DEBUG_EN)
   if (Serial.available())
   {
    Serial.readBytesUntil('\n', &Termostat_debug_cyclic_buffer[0], TERMOSTAT_DEBUG_BUFFER_SIZE);
   }
   #endif
}

static void Termostat_debug_parse_input(char *input,int size)
{
  #if (TERMOSTAT_DEBUG_EN)
  int temp[5]={0,0,0,0,0};
  char tokens[5][32];
  int read_len = strlen(input);
  if (read_len>0)
  {
    Termostat_watchdog_Service(true);
    if (strcmp(input, "help")==0)
    {
      TERMOSTAT_DEBUG_FNC("possible commands:\nalarm_en0\nalarm_en1\n");
      TERMOSTAT_DEBUG_FNC("rs485_en0\nrs485_en1\nread_sensors\nrs485_send\nwifi_scan\nwifi_info\n");
      TERMOSTAT_DEBUG_FNC("rs485_brate x\nwifi_connect_ssid ssid password\nwifi_connect_id scan_id password\n");
      TERMOSTAT_DEBUG_FNC("wifi_disconnect\nwrite_config\necu_reset\nwifi_udp_multicast_ip\nwifi_udp_multicast_port\nMQ9_thresh\n");
      TERMOSTAT_DEBUG_FNC("invalidate_nvm_crc\npopulate_temp\nrs485_timeout\nsw_version\nudp_info\n");
      TERMOSTAT_DEBUG_FNC("disable_wdt\nloop_delta\nMQ9_en0\nMQ9_en1\nDHT11_en0\nDHT11_en1\n");
    }
    else if (strcmp(input, "alarm_en0")==0){Termostat_hardware_WriteAlarm_Digital(0);}
    else if (strcmp(input, "alarm_en1")==0){Termostat_hardware_WriteAlarm_Digital(1);}
    else if (strcmp(input, "rs485_en0")==0){Termostat_hardware_RS485_EN(0);}
    else if (strcmp(input, "rs485_en1")==0){Termostat_hardware_RS485_EN(1);}
    else if (strcmp(input, "MQ9_en0")==0){TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_MQ9Disable=1;}
    else if (strcmp(input, "MQ9_en1")==0){TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_MQ9Disable=0;}
    else if (strcmp(input, "DHT11_en0")==0){TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_DHT11Disable=1;}
    else if (strcmp(input, "DHT11_en1")==0){TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_DHT11Disable=0;}
    else if (strcmp(input, "rs485_send")==0){Termostat_hardware_RS485_Write((byte*)"Hello RS485!\n",strlen("Hello RS485!\n"),false);}
    else if (strcmp(input, "wifi_scan")==0){(void)Termostat_wifi_ScanNetworks(1);}
    else if (strcmp(input, "wifi_info")==0){Termostat_wifi_printInfo();}
    else if (strcmp(input, "wifi_disconnect")==0){Termostat_wifi_Disconnect();}
    else if (strcmp(input, "write_config")==0){Termostat_nvm_WriteAndCommit();}
    else if (strcmp(input, "invalidate_nvm_crc")==0){Termostat_nvm_Restore();}
    else if (strcmp(input, "ecu_reset")==0){Termostat_hardware_Reset();}
    else if (strcmp(input, "sw_version")==0){TERMOSTAT_DEBUG_FNC("SW Version: %X\n",TERMOSTAT_HARDWARE_SW_VERSION);}
    else if (strcmp(input, "udp_info")==0){Termostat_ctrlnode_printUdpStats();}
    else if (strcmp(input, "disable_wdt")==0){Termostat_watchdog_SetEnabled(false);}
    else if (strcmp(input, "loop_delta")==0){int c = Termostat_hardware_coreID();TERMOSTAT_DEBUG_FNC("DeltaMs: %d / %d max, core: %u\n",Termostat_debug_last_delta_TS,Termostat_debug_last_delta_max_TS,c);}
    else if (strcmp(input, "read_sensors")==0){Termostat_hardware_printSensorsInfo();}
    else
    {
      Termostat_utils_split(input,' ',32,&tokens[0][0],&tokens[1][0],&tokens[2][0],&tokens[3][0],&tokens[4][0]);
      if (strcmp(tokens[0], "rs485_brate")==0)
      {
        TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_RS485_baudrate_u16=atoi(tokens[1]);
      }
      else if (strcmp(tokens[0], "rs485_timeout")==0)
      {
        TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_RS485_timeout_u16=atoi(tokens[1]);
      }
      else if (strcmp(tokens[0], "wifi_connect_ssid")==0)
      {
        TERMOSTAT_DEBUG_FNC("Connecting to: %s, password: %s \n",tokens[1],tokens[2]);
        Termostat_wifi_Connect(tokens[1],tokens[2]);
      }
      else if (strcmp(tokens[0], "wifi_connect_id")==0)
      {
        TERMOSTAT_DEBUG_FNC("Connecting to: %s, password: %s \n",tokens[1],tokens[2]);
        Termostat_wifi_ConnectID(atoi(tokens[1]),tokens[2]);
      }
      else if (strcmp(tokens[0], "wifi_udp_multicast_ip")==0)
      {
        byte ip[]={atoi(tokens[1]),atoi(tokens[2]),atoi(tokens[3]),atoi(tokens[4])};
        Termostat_ctrlnode_SetMulticastIp(&ip[0],nullptr);
      }
      else if (strcmp(tokens[0], "wifi_udp_multicast_port")==0)
      {
        uint16_t port=atoi(tokens[1]);
        Termostat_ctrlnode_SetMulticastIp(nullptr,&port);
      }
      else if (strcmp(tokens[0], "MQ9_thresh")==0)
      {
        TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_MQ9_LowerTh=atoi(tokens[1]);
        TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_MQ9_UpperTh=atoi(tokens[2]);
      }
      else if (strcmp(tokens[0], "populate_temp")==0)
      {
        TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_populate_temp_src=atoi(tokens[1]);
        TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_populate_temp_dst=atoi(tokens[2]);
        TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_populate_temp_tempnr=atoi(tokens[3]);
        TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_populate_temp_protocol=atoi(tokens[4]);
      }
    }
    memset(input,0,size);
    Termostat_watchdog_Service(true);
  }

  #endif
}
