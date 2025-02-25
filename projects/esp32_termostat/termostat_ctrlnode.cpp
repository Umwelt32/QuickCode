#include "termostat_ctrlnode.hpp"
#include "termostat_hardware.hpp"
#include "termostat_wifi.hpp"
#include "termostat_utils.hpp"
#include "termostat_nvm.hpp"
#include "termostat_debug.hpp"
#include "termostat_C3_protocol.hpp"

#define TERMOSTAT_CTRLNODE_C3_DEVID (71)
#define TERMOSTAT_CTRLNODE_MQ9_ALARM_ON  (TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_MQ9_UpperTh)
#define TERMOSTAT_CTRLNODE_MQ9_ALARM_OFF (TERMOSTAT_NVM_STRUCT_RAM.Termostat_hardware_MQ9_LowerTh)
#define TERMOSTAT_CTRLNODE_UDP_BUFFER_SIZE (64)
#define TERMOSTAT_CTRLNODE_UDP_PAYLOAD_SIZE (57)
#define TERMOSTAT_CTRLNODE_UDP_HEADER_REQUEST  (0x5A)
#define TERMOSTAT_CTRLNODE_UDP_HEADER_RESPONSE (0xA5)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_HELLO (0x00)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_SENSORS_REQUEST (0x01)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_RS485_WRITE_REQUEST (0x02)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_DEBUG_CMD_REQUEST (0x03)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_RS485_FAST_WRITE_REQUEST  (0x04)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_FORCE_ALARM_REQUEST (0x05)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_C3_READ_TEMP_REQUEST (0x06)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_C3_RW_PARAMETER_REQUEST (0x07)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_C3_EXECUTE_REQUEST      (0x08)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_C3_POPULATE_TEMP        (0x09)
#define TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_READ_DEBUG_OUTPUT       (0x0A)
/*-----------------------------------------------------------*/
typedef struct
{
  byte header_u8;
  byte packet_type_u8;
  byte packet_unique_id_u8;
  byte payload_protocol_u8;
  byte payload_size_u8;
  byte payload_au8[TERMOSTAT_CTRLNODE_UDP_PAYLOAD_SIZE];
  uint16_t crc_u16;
}Termostat_ctrlnode_udp_frame_t;
/*-----------------------------------------------------------*/
typedef struct
{
   int Termostat_ctrlnode_Dht11_Temp;
   int Termostat_ctrlnode_Dht11_Humidity;
   int Termostat_ctrlnode_Dht11_result_ok;
   int Termostat_ctrlnode_MQ9_A;
   int Termostat_ctrlnode_AlarmActive;
   int Termostat_ctrlnode_AlarmActive_last;
   uint16_t Termostat_ctrlnode_AQ_Cycle;
}Termostat_ctrlnode_sensors_t;
/*-----------------------------------------------------------*/
typedef struct
{
  byte enabled;
  byte src;
  byte dst;
  byte tempnr;
  byte src_protocol;
}Termostat_ctrlnode_populate_temp_t;
/*----------------------------------------------------------*/
typedef struct
{
  uint16_t in_packets;
  uint16_t bad_crc_packets;
  uint16_t bad_header_packet;
  uint16_t processed_packets;
  uint16_t sent_packets;
  uint16_t last_packet_crc;
  uint16_t last_packet_calculated_crc;
  int last_packet_size;
}Termostat_ctrlnode_udp_stats_t;
/*----------------------------------------------------------*/
static Termostat_ctrlnode_sensors_t Termostat_Sensors={0,0,0,0,0xFF,0};
static Termostat_ctrlnode_populate_temp_t Termostat_TempPopulate={0,0,0,0,0};
static WiFiUDP Termostat_ctrlnode_udp_multicast;
static byte Termostat_ctrlnode_udp_buffer[TERMOSTAT_CTRLNODE_UDP_BUFFER_SIZE];
static Termostat_ctrlnode_udp_frame_t *multicast_frame = (Termostat_ctrlnode_udp_frame_t*)&Termostat_ctrlnode_udp_buffer[0];
static byte Termostat_ctrlnode_AlarmForced_u8 = 0;
static hw_time_t Termostat_ctrlnode_poll_clock_ms = 0;
static int Termostat_ctrlnode_multicast_started = 0;
static Termostat_ctrlnode_udp_stats_t Termostat_ctrlnode_udp_stats;
static IPAddress Termostat_ctrlnode_udp_multicast_IP;

static void Termostat_ctrlnode_SensorsAQ(void);
static void Termostat_ctrlnode_multicast_loop(void);
static void Termostat_ctrlnode_BuildPacket(byte type,byte uid,byte proto,const void *data,byte len,bool sent);
static void Termostat_ctrlnode_RS485_PopulateTemp(byte src,byte dst,byte src_proto,byte srctemp);
static void Termostat_ctrlnode_KeepWifiConnected(void);
static void Termostat_ctrlnode_RS485_UDP_RequestResponse(void);
static void Termostat_ctrlnode_parseUdpBuffer(void);
static void Termostat_ctrlnode_multicast_start(void);


void Termostat_ctrlnode_Init(void)
{
  Termostat_TempPopulate = {(TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_populate_temp_src!=0)?1:0,TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_populate_temp_src,TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_populate_temp_dst,TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_populate_temp_tempnr,TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_populate_temp_protocol};
  Termostat_ctrlnode_udp_multicast_IP = IPAddress(TERMOSTAT_CTRLNODE_MULTICAST_IP0, TERMOSTAT_CTRLNODE_MULTICAST_IP1, TERMOSTAT_CTRLNODE_MULTICAST_IP2, TERMOSTAT_CTRLNODE_MULTICAST_IP3);
  Termostat_ctrlnode_SensorsAQ();
  Termostat_ctrlnode_multicast_start();
}

void Termostat_ctrlnode_SetMulticastIp(byte *ip,uint16_t *port)
{
  if(ip!=nullptr)memcpy(&TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_ip[0],ip,4);
  if(port!=nullptr)memcpy(&TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_port,&port[0],2);
}

bool Termostat_ctrlnode_UdpSendPacket(const void *p,int len)
{
  bool success = false;
  const uint8_t *buffer = (const uint8_t*)p;
  Termostat_ctrlnode_udp_multicast.beginMulticastPacket();
  /*Termostat_ctrlnode_udp_multicast.beginPacket();*/
  Termostat_ctrlnode_udp_multicast.write(buffer,len);
  success = Termostat_ctrlnode_udp_multicast.endPacket();
  ++Termostat_ctrlnode_udp_stats.sent_packets;
  return success;
}

void Termostat_ctrlnode_printUdpStats(void)
{
  TERMOSTAT_DEBUG_FNC("CONNECTED: %U\n",Termostat_wifi_isConnected());
  TERMOSTAT_DEBUG_FNC("PACKETS_IN: %U\n",Termostat_ctrlnode_udp_stats.in_packets);
  TERMOSTAT_DEBUG_FNC("PACKETS_BAD: %U\n",Termostat_ctrlnode_udp_stats.bad_header_packet);
  TERMOSTAT_DEBUG_FNC("PACKETS_BAD_CRC: %U\n",Termostat_ctrlnode_udp_stats.bad_crc_packets);
  TERMOSTAT_DEBUG_FNC("LAST_CRC_PACKET/CAL: %X ; %X\n",Termostat_ctrlnode_udp_stats.last_packet_crc,Termostat_ctrlnode_udp_stats.last_packet_calculated_crc);
  TERMOSTAT_DEBUG_FNC("LAST_PACKET_SIZE: %U\n",Termostat_ctrlnode_udp_stats.last_packet_size);
  TERMOSTAT_DEBUG_FNC("PROCESSED_PACKETS: %U\n",Termostat_ctrlnode_udp_stats.processed_packets);
  TERMOSTAT_DEBUG_FNC("SENT_PACKETS: %U\n",Termostat_ctrlnode_udp_stats.sent_packets);
  TERMOSTAT_DEBUG_FNC("MULTICAST_STARTED: %U\n",Termostat_ctrlnode_multicast_started);
  TERMOSTAT_DEBUG_FNC("IP: %s\n",TERMOSTAT_WIFI_DEVICE.localIP().toString().c_str());
  TERMOSTAT_DEBUG_FNC("MULTICAST_GROUP: %s:%U\n",Termostat_ctrlnode_udp_multicast_IP.toString().c_str(), TERMOSTAT_CTRLNODE_MULTICAST_PORT);
}

void Termostat_ctrlnode_Mainfunction(void)
{
  Termostat_ctrlnode_poll_clock_ms=Termostat_hardware_clock_ms();
  Termostat_ctrlnode_SensorsAQ();
  Termostat_ctrlnode_multicast_loop();
  if (Termostat_TempPopulate.enabled!=0)
  {
    Termostat_ctrlnode_RS485_PopulateTemp(Termostat_TempPopulate.src,Termostat_TempPopulate.dst,Termostat_TempPopulate.src_protocol,Termostat_TempPopulate.tempnr);
  }
  Termostat_ctrlnode_KeepWifiConnected();
  Termostat_ctrlnode_multicast_start();
}

static void Termostat_ctrlnode_multicast_start(void)
{
    int connected = Termostat_wifi_isConnected();
    if (connected!=0)
    {
      if (Termostat_ctrlnode_multicast_started == 0)
      {
          Termostat_ctrlnode_multicast_started = Termostat_ctrlnode_udp_multicast.beginMulticast(Termostat_ctrlnode_udp_multicast_IP, TERMOSTAT_CTRLNODE_MULTICAST_PORT);
      }
    }
    else
    {
      Termostat_ctrlnode_multicast_started = 0;
    }
}

static void Termostat_ctrlnode_multicast_loop(void)
{
  int len = 0;
  int connected = Termostat_wifi_isConnected();
  if (connected == 0)return;
  len = Termostat_ctrlnode_udp_multicast.parsePacket();
  if (len<=0)return;
  len = Termostat_ctrlnode_udp_multicast.available();
  if (len<=0)return;
  if (len>TERMOSTAT_CTRLNODE_UDP_BUFFER_SIZE)len=TERMOSTAT_CTRLNODE_UDP_BUFFER_SIZE;
  Termostat_ctrlnode_udp_stats.last_packet_size=len;
  len = Termostat_ctrlnode_udp_multicast.read(&Termostat_ctrlnode_udp_buffer[0], len);
  if (len<=0)return;
  Termostat_ctrlnode_parseUdpBuffer();
}

static void Termostat_ctrlnode_parseUdpBuffer(void)
{
  uint16_t crc_u16 = 0;
  ++Termostat_ctrlnode_udp_stats.in_packets;
  if (multicast_frame->header_u8 != TERMOSTAT_CTRLNODE_UDP_HEADER_REQUEST)
  {
    ++Termostat_ctrlnode_udp_stats.bad_header_packet;
    return;
  }
  crc_u16=Termostat_utils_CalculateU16Crc(&multicast_frame->header_u8,&multicast_frame->crc_u16);
  Termostat_ctrlnode_udp_stats.last_packet_crc=multicast_frame->crc_u16;
  Termostat_ctrlnode_udp_stats.last_packet_calculated_crc=crc_u16;
  if (multicast_frame->crc_u16!=crc_u16)
  {
    ++Termostat_ctrlnode_udp_stats.bad_crc_packets;
    return;
  }
  ++Termostat_ctrlnode_udp_stats.processed_packets;
  if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_HELLO)
  {
      Termostat_ctrlnode_BuildPacket(multicast_frame->packet_type_u8,0,0,"HELLO ESP32 THERE!",strlen("HELLO ESP32 THERE!"),true);
  }
  else if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_SENSORS_REQUEST)
  {
    Termostat_ctrlnode_BuildPacket(multicast_frame->packet_type_u8,0,0,&Termostat_Sensors,sizeof(Termostat_Sensors),true);
  }
  else if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_RS485_WRITE_REQUEST)
  {
    Termostat_ctrlnode_RS485_UDP_RequestResponse();
  }
  else if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_DEBUG_CMD_REQUEST)
  {
    Termostat_debug_WriteCommandBuffer(&multicast_frame->payload_au8[0],multicast_frame->payload_size_u8);
  }
  else if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_RS485_FAST_WRITE_REQUEST)
  {
    if (multicast_frame->payload_size_u8>0)Termostat_hardware_RS485_Write(&multicast_frame->payload_au8[0],multicast_frame->payload_size_u8,false);
  }
  else if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_FORCE_ALARM_REQUEST)
  {
    Termostat_ctrlnode_AlarmForced_u8 = multicast_frame->payload_au8[0]!=0?1:0;
  }
  else if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_C3_READ_TEMP_REQUEST)
  {
      byte dst  = multicast_frame->payload_au8[0];
      byte tempnr = multicast_frame->payload_au8[1];
      byte proto  = multicast_frame->payload_protocol_u8;
      if (proto==0xC2)
      {
        multicast_frame->payload_protocol_u8 = 0xC2;
        Termostat_C2_protocol_TemperatureRequest(&multicast_frame->payload_au8[0],&multicast_frame->payload_size_u8,dst,TERMOSTAT_CTRLNODE_C3_DEVID,tempnr);
      }
      else
      {
        multicast_frame->payload_protocol_u8 = 0xC3;
        Termostat_C3_protocol_TemperatureRequest(&multicast_frame->payload_au8[0],&multicast_frame->payload_size_u8,dst,TERMOSTAT_CTRLNODE_C3_DEVID,tempnr);
      }

      Termostat_ctrlnode_RS485_UDP_RequestResponse();
  }
  else if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_C3_RW_PARAMETER_REQUEST)
  {
      /*TODO*/
      byte proto  = multicast_frame->payload_protocol_u8;
      byte dst   = multicast_frame->payload_au8[0];
      byte param = (byte)multicast_frame->payload_au8[1];
      byte write = (byte)multicast_frame->payload_au8[2];
      int new_value   = (int)multicast_frame->payload_au8[4];
      if (multicast_frame->payload_au8[3]==0x2D)new_value=new_value*-1; /*0x2D = '-'*/
      int *new_value_p = (write==0x57)?&new_value:nullptr; /*ASCII 'W' needed for write*/
      if (proto==0xC2)
      {
        multicast_frame->payload_protocol_u8 = 0xC2;
        Termostat_C2_protocol_ParameterRequest(&multicast_frame->payload_au8[0],&multicast_frame->payload_size_u8,dst,TERMOSTAT_CTRLNODE_C3_DEVID,param,new_value_p);
      }
      else
      {
        multicast_frame->payload_protocol_u8 = 0xC3;
        Termostat_C3_protocol_ParameterRequest(&multicast_frame->payload_au8[0],&multicast_frame->payload_size_u8,dst,TERMOSTAT_CTRLNODE_C3_DEVID,param,new_value_p);
      }
      
      Termostat_ctrlnode_RS485_UDP_RequestResponse();
  }
 else if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_C3_EXECUTE_REQUEST)
  {
      /*TODO*/
      byte proto  = multicast_frame->payload_protocol_u8;
      byte dst  = multicast_frame->payload_au8[0];
      byte param  = multicast_frame->payload_au8[1];
      if (proto==0xC2)
      {
        multicast_frame->payload_protocol_u8 = 0xC2;
        Termostat_C2_protocol_ExecuteRequest(&multicast_frame->payload_au8[0],&multicast_frame->payload_size_u8,dst,TERMOSTAT_CTRLNODE_C3_DEVID,param);
      }
      else
      {
        multicast_frame->payload_protocol_u8 = 0xC3;
        Termostat_C3_protocol_ExecuteRequest(&multicast_frame->payload_au8[0],&multicast_frame->payload_size_u8,dst,TERMOSTAT_CTRLNODE_C3_DEVID,param);
      }

      Termostat_ctrlnode_RS485_UDP_RequestResponse();
  }
  else if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_C3_POPULATE_TEMP)
  {
     byte buffer[32];
     byte olen = 0;
     byte dst  = multicast_frame->payload_au8[0];
     int temp  = 0;
     byte frac = 0;
     Termostat_utils_C3_protocol_C3Temp2Int(multicast_frame->payload_au8[1],multicast_frame->payload_au8[2],multicast_frame->payload_au8[3],&temp,&frac);
     Termostat_C3_protocol_PopulateTemperature(&buffer[0],&olen,dst,TERMOSTAT_CTRLNODE_C3_DEVID,temp,frac);
     Termostat_hardware_RS485_Write(&buffer[0],olen,false);
  }
  else if (multicast_frame->packet_type_u8 == TERMOSTAT_CTRLNODE_UDP_PACKET_TYPE_READ_DEBUG_OUTPUT)
  {
      byte len=0;
      Termostat_debug_out_buffer(&multicast_frame->payload_au8[0],&len);
      Termostat_ctrlnode_BuildPacket(multicast_frame->packet_type_u8,0,0,&multicast_frame->payload_au8[0],len,true);
  }

  memset(&Termostat_ctrlnode_udp_buffer[0],0,TERMOSTAT_CTRLNODE_UDP_BUFFER_SIZE);
}

static void Termostat_ctrlnode_SensorsAQ(void)
{
  Termostat_Sensors.Termostat_ctrlnode_MQ9_A = Termostat_hardware_ReadMQ9_Analog();
  Termostat_Sensors.Termostat_ctrlnode_Dht11_result_ok=Termostat_hardware_ReadDHT11(&Termostat_Sensors.Termostat_ctrlnode_Dht11_Temp, &Termostat_Sensors.Termostat_ctrlnode_Dht11_Humidity);
  if (Termostat_Sensors.Termostat_ctrlnode_MQ9_A > TERMOSTAT_CTRLNODE_MQ9_ALARM_ON)Termostat_Sensors.Termostat_ctrlnode_AlarmActive=1;
  if (Termostat_Sensors.Termostat_ctrlnode_MQ9_A < TERMOSTAT_CTRLNODE_MQ9_ALARM_OFF)Termostat_Sensors.Termostat_ctrlnode_AlarmActive=0;
  if (Termostat_ctrlnode_AlarmForced_u8!=0)Termostat_Sensors.Termostat_ctrlnode_AlarmActive=1;
  if (Termostat_Sensors.Termostat_ctrlnode_AlarmActive_last!=Termostat_Sensors.Termostat_ctrlnode_AlarmActive)
  {
      Termostat_Sensors.Termostat_ctrlnode_AlarmActive_last=Termostat_Sensors.Termostat_ctrlnode_AlarmActive;
      Termostat_hardware_WriteAlarm_Digital(Termostat_Sensors.Termostat_ctrlnode_AlarmActive);
  }
  Termostat_Sensors.Termostat_ctrlnode_AQ_Cycle++;
}

static void Termostat_ctrlnode_BuildPacket(byte type,byte uid,byte proto,const void *data,byte len,bool sent)
{
  static byte _temp_buffer[TERMOSTAT_CTRLNODE_UDP_PAYLOAD_SIZE+1];
  byte _len = (len<TERMOSTAT_CTRLNODE_UDP_PAYLOAD_SIZE)?len:TERMOSTAT_CTRLNODE_UDP_PAYLOAD_SIZE;
  memcpy(&_temp_buffer[0],data,_len);
  memset(multicast_frame,0,TERMOSTAT_CTRLNODE_UDP_BUFFER_SIZE);
  multicast_frame->packet_type_u8=type;
  multicast_frame->header_u8=TERMOSTAT_CTRLNODE_UDP_HEADER_RESPONSE;
  multicast_frame->packet_unique_id_u8 = uid;
  multicast_frame->payload_protocol_u8 = proto;
  multicast_frame->payload_size_u8=_len;
  memcpy(&multicast_frame->payload_au8[0],&_temp_buffer[0],multicast_frame->payload_size_u8);
  multicast_frame->crc_u16=Termostat_utils_CalculateU16Crc(&multicast_frame->header_u8,&multicast_frame->crc_u16);
  if (sent)Termostat_ctrlnode_UdpSendPacket(multicast_frame,TERMOSTAT_CTRLNODE_UDP_BUFFER_SIZE);
}

static void Termostat_ctrlnode_RS485_PopulateTemp(byte src,byte dst,byte src_proto,byte srctemp)
{
  int len;
  byte olen=0;
  byte buffer[32];
  int temp_dec = 0;
  byte temp_frac = 0;
  byte frame_ok = 0;
  byte temp_nr  = 0;
  hw_time_t current_clock = Termostat_ctrlnode_poll_clock_ms;
  static hw_time_t populate_ts = 0;
  if (populate_ts < current_clock)
  {
    populate_ts = current_clock+10000;
    if (src_proto==0xC2)
    {
      Termostat_C2_protocol_TemperatureRequest(&buffer[0],&olen,src,TERMOSTAT_CTRLNODE_C3_DEVID,srctemp);
    }
    else
    {
      Termostat_C3_protocol_TemperatureRequest(&buffer[0],&olen,src,TERMOSTAT_CTRLNODE_C3_DEVID,srctemp);
    }
    Termostat_hardware_RS485_Write(&buffer[0],olen,true);
    len=Termostat_hardware_RS485_Read(&buffer[0],olen,true);
    if (len>0)
    {
      if (src_proto==0xC2)
      {
        temp_nr=srctemp;
        Termostat_utils_C2_protocol_readTempFromReply(&buffer[0],&frame_ok,&temp_dec,&temp_frac);
      }
      else
      {
        Termostat_utils_C3_protocol_readTempFromReply(&buffer[0],&frame_ok,&temp_nr,&temp_dec,&temp_frac);
      }
      if ((frame_ok!=0)&&(temp_nr==srctemp))
      {
        Termostat_C3_protocol_PopulateTemperature(&buffer[0],&olen,dst,TERMOSTAT_CTRLNODE_C3_DEVID,temp_dec,temp_frac);
        Termostat_hardware_RS485_Write(&buffer[0],olen,false);
      }
    }
  }
}

static void Termostat_ctrlnode_KeepWifiConnected(void)
{
  hw_time_t current_clock = Termostat_ctrlnode_poll_clock_ms;
  static hw_time_t populate_ts = 0;
  if (populate_ts < current_clock)
  {
    populate_ts = current_clock+200000;
    Termostat_wifi_Reconnect();
  }
}

static void Termostat_ctrlnode_RS485_UDP_RequestResponse(void)
{
  int len   = 0;
  if (multicast_frame->payload_size_u8>0)Termostat_hardware_RS485_Write(&multicast_frame->payload_au8[0],multicast_frame->payload_size_u8,true);
  len=Termostat_hardware_RS485_Read(&multicast_frame->payload_au8[0],TERMOSTAT_CTRLNODE_UDP_PAYLOAD_SIZE,false);
  Termostat_ctrlnode_BuildPacket(multicast_frame->packet_type_u8,multicast_frame->packet_unique_id_u8,multicast_frame->payload_protocol_u8,&multicast_frame->payload_au8[0],len>0?len:0,true);
}


