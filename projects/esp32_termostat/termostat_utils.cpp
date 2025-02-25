#include "Arduino.h"
#include "termostat_utils.hpp"
#include <cstdio>
#include <cstring>

void Termostat_utils_split(const char *input,char delimeter,int token_size,char *token1,char *token2,char *token3,char *token4,char *token5)
{
    int start_idx = 0;
    int token_idx = 0;
    int token_len = 0;
    int str_len   = input?strlen(input):0;
    char *tokens[] = {token1,token2,token3,token4,token5,nullptr};
    for(int i=0;tokens[i]!=nullptr;++i)
    {
      memset((void*)&tokens[i][0],0,token_size);
    }
    for (int i = 0; i <= str_len;++i)
    {
        if (tokens[token_idx]==nullptr)break;
        if ((input[i]==delimeter) || (i>=(str_len)))
        {
            token_len = (i-start_idx);
            if (token_len<=0)continue;
            memcpy(&tokens[token_idx][0],&input[start_idx],token_len);
            start_idx=i+1;
            ++token_idx;
        }
    }
}


uint16_t Termostat_utils_CalculateU16Crc(const void *data_start,const void *data_end)
{
	uint16_t crc=0;
	uint16_t value=0;
	uint16_t *data16_p =     (uint16_t*)data_start;
	uint16_t *data16_end_p = (uint16_t*)data_end;
	if ((data16_p!=nullptr)&&(data16_end_p!=nullptr))
	{
		while((&data16_p[0])<(&data16_end_p[0]))
		{
			value = *(data16_p++);
			crc+=value;
			if(crc < value) 
			{
				++crc;
			}
		}
		crc = crc&0xFFFFu;
		crc = (~crc)&(0xFFFFu);
	}
	
	return crc;
}

void Termostat_utils_value2char(int value,char *sign,char *hun,char *cent,char *decs)
{
  char _zero = 0x30;
  char _sign = (value<0)?(0x2D):(0x2B);
  int _value = abs(value);
  int _hun   = (_value/100);
  int _cents = (_value/10);
  int _decs  = (_value%10);
  if (_cents>9)_cents=9;
  if (_decs>9)_decs=9;
  if (_hun>9)_hun=9;
  if(sign!=nullptr)(*sign)=_sign;
  if(hun!=nullptr)(*hun)=(_hun+_zero);
  if(cent!=nullptr)(*cent)=(_cents+_zero);
  if(decs!=nullptr)(*decs)=(_decs+_zero);
}

void Termostat_utils_C3_protocol_C3Temp2Int(byte sign,byte cents,byte dec,int *decimal_o,byte *fractal_o)
{
  int decimal  = 0;
  byte fractal = 0;
  /*[05 12] = 51.2 deg*/
  decimal = cents*10;
  fractal = dec%10;
  decimal+= dec/10;
  if(sign==0x2D)decimal=decimal*-1;
  if (decimal_o!=nullptr)(*decimal_o)=decimal;
  if (fractal_o!=nullptr)(*fractal_o)=fractal;
}

void Termostat_utils_C3_protocol_readTempFromReply(void *frame,byte *frame_ok,byte *nr,int *decimal_o,byte *fractal_o)
{
  /*0x74 = 't' ; 0x2D = '-' */
  byte* buffer = (byte*)frame;
  byte tempnr  = 0;
  if (buffer[1]==0x74)
  {
    tempnr  = buffer[6];
    Termostat_utils_C3_protocol_C3Temp2Int(buffer[7],buffer[8],buffer[9],decimal_o,fractal_o);
    if(nr!=nullptr)(*nr)=tempnr;
    if(frame_ok!=nullptr)(*frame_ok)=1;
  }
  else
  {
    if(frame_ok!=nullptr)(*frame_ok)=0;
  }
}

void Termostat_utils_C2_protocol_readTempFromReply(void *frame,byte *frame_ok,int *decimal_o,byte *fractal_o)
{
  /*0x74 = 't' ; 0x2D = '-' */
  byte* buffer = (byte*)frame;
  char temp[6];
  int decimal  = 0;
  byte fractal = 0;
  byte sign = 0;
  if (buffer[1]==0x74)
  {
    memcpy(&temp[0],&buffer[6],3);
    temp[3]=0;
    decimal = atoi(temp);
    sign    = buffer[5];
    fractal = buffer[9]-0x30;
    if (sign==0x2D)decimal=decimal*-1;
    if (decimal_o!=nullptr)(*decimal_o)=decimal;
    if (fractal_o!=nullptr)(*fractal_o)=fractal;
    if(frame_ok!=nullptr)(*frame_ok)=1;
  }
  else
  {
    if(frame_ok!=nullptr)(*frame_ok)=0;
  }
}

uint16_t Termostat_utils_C3_protocol_R311RawTempValue(int integral,byte fractal)
{
  uint16_t temp=0;
  int dec_1 = 0;
  int dec_2 = 0;
  /*0x03 0x4D = -2.3 deg  ; -40 <> +200*/
  if (integral<=-40)integral=-40;
  if (integral>=200)integral=200;
  if (fractal>9)fractal=9;
  dec_1 = integral/10;
  dec_2 = ((abs(integral)-abs(dec_1*10))*10)+fractal;
  dec_1 = (dec_1+4);
  if (integral<0)
  {
    dec_2=(100-dec_2);
    dec_1--;
  }
  temp  = ((dec_1<<8)&0xFF00u) | (dec_2&0xFFu);
  return temp;
}

void Termostat_utils_C3_protocol_Parameter(int value,byte *sign,byte *cents,byte *decs)
{
  /*0x2D -, 0x2B +*/
  int _value = value;
  byte _sign = (_value<0)?0x2D:0x2B;
  _value = abs(_value);
  byte _cents = (_value/100);
  byte _decs  = (_value%100);
  if (sign!=nullptr)(*sign)=_sign;
  if (cents!=nullptr)(*cents)=_cents;
  if (decs!=nullptr)(*decs)=_decs;
}

