#pragma once
#include "Arduino.h"
/*----------------------------------*/
typedef struct
{
    byte destination_device_u8;
    byte command_u8;
    byte source_device_u8;
    byte CRC8b_u8;
    byte payload_au8[6];
    byte eof_u8;
}Termostat_C3_protocol_C3Frame_t;
/*----------------------------------*/
typedef struct
{
    byte destination_device_u8;
    byte command_u8;
    byte source_device_u8;
    byte payload_au8[7];
    byte eof_u8;
}Termostat_C3_protocol_C2Frame_t;
/*----------------------------------*/

void Termostat_C3_protocol_Construct_C3Frame(Termostat_C3_protocol_C3Frame_t *p,byte dst,byte command,byte src,const byte *payload);
void Termostat_C3_protocol_Construct_C2Frame(Termostat_C3_protocol_C2Frame_t *p,byte dst,byte command,byte src,const byte *payload);
/*---------------------------------------------------------------------------------------------------------------------------------*/
void Termostat_C3_protocol_TemperatureRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte tempnr);
void Termostat_C3_protocol_ParameterRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte param,int *new_value);
void Termostat_C3_protocol_PopulateTemperature(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,int integral,byte fractal); /*TODO*/
void Termostat_C3_protocol_PopulateTime(void *buffer_out_p,byte *out_len_u8,byte dst,byte src); /*TODO*/
void Termostat_C3_protocol_ExecuteRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte idx); /*TODO*/
void Termostat_C3_protocol_readTempFromReply(void *frame,byte *frame_ok,byte *nr,int *decimal_o,byte *fractal_o);
void Termostat_C3_protocol_C3Temp2Int(byte sign,byte cents,byte dec,int *decimal_o,byte *fractal_o);
/*---------------------------------------------------------------------------------------------------------------------------------*/
void Termostat_C2_protocol_TemperatureRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte tempnr);
void Termostat_C2_protocol_ParameterRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte param,int *new_value);
void Termostat_C2_protocol_ExecuteRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte idx); /*TODO*/
/*-------------------------------------------------------------------------------------------------------------------------------*/



