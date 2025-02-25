#pragma once

void Termostat_utils_split(const char *input,char delimeter,int token_size,char *token1,char *token2,char *token3,char *token4,char *token5);
uint16_t Termostat_utils_CalculateU16Crc(const void *data_start,const void *data_end);
void Termostat_utils_value2char(int value,char *sign,char *hun,char *cent,char *decs);
void Termostat_utils_C3_protocol_C3Temp2Int(byte sign,byte cents,byte dec,int *decimal_o,byte *fractal_o);
void Termostat_utils_C3_protocol_readTempFromReply(void *frame,byte *frame_ok,byte *nr,int *decimal_o,byte *fractal_o);
uint16_t Termostat_utils_C3_protocol_R311RawTempValue(int integral,byte fractal);
void Termostat_utils_C3_protocol_Parameter(int value,byte *sign,byte *cents,byte *decs);
void Termostat_utils_C2_protocol_readTempFromReply(void *frame,byte *frame_ok,int *decimal_o,byte *fractal_o);