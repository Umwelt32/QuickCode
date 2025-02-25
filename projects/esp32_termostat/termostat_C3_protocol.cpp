#include "termostat_C3_protocol.hpp"
#include "termostat_utils.hpp"

void Termostat_C3_protocol_Construct_C3Frame(Termostat_C3_protocol_C3Frame_t *p,byte dst,byte command,byte src,const byte *payload)
{
    if (p!=nullptr)
    {
        byte *raw_data = (byte*)p;
        memset(p,0,sizeof(Termostat_C3_protocol_C3Frame_t));
        if (p->destination_device_u8>100)p->destination_device_u8=100;
        p->destination_device_u8=(dst|0x80u);
        p->command_u8=command;
        p->source_device_u8=src;
        p->eof_u8=0x23; /*#*/
        if (payload!=nullptr)memcpy((void*)&p->payload_au8[0],&payload[0],6);
        p->CRC8b_u8=(raw_data[2]+raw_data[4]+raw_data[5]+raw_data[6]+raw_data[7]+raw_data[8]+raw_data[9]);
        p->CRC8b_u8=(p->CRC8b_u8&0x7F);
    }
}

void Termostat_C3_protocol_Construct_C2Frame(Termostat_C3_protocol_C2Frame_t *p,byte dst,byte command,byte src,const byte *payload)
{
    if (p!=nullptr)
    {
        memset(p,0,sizeof(Termostat_C3_protocol_C2Frame_t));
        if (p->destination_device_u8>100)p->destination_device_u8=100;
        p->destination_device_u8=(dst|0x80u);
        p->command_u8=command;
        p->source_device_u8=src;
        p->eof_u8=0x23;/*#*/
        if (payload!=nullptr)memcpy((void*)&p->payload_au8[0],&payload[0],7);
    }
}

void Termostat_C2_protocol_TemperatureRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte tempnr)
{
  Termostat_C3_protocol_C2Frame_t f;
  byte payload[] = {0x30,0x30,0x78,0x78,0x78,0x78,0x78};
  Termostat_utils_value2char(tempnr,nullptr,nullptr,(char*)&payload[0],(char*)&payload[1]);
  /* 0x54 = 'T' ; 0x78 = 'x' ; 0x30 = '0' */
  Termostat_C3_protocol_Construct_C2Frame(&f,dst,0x54,src,&payload[0]);
  if (out_len_u8!=nullptr)(*out_len_u8)=sizeof(f);
  memcpy(buffer_out_p,&f,sizeof(f));
}

void Termostat_C2_protocol_ParameterRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte param,int *new_value)
{
  Termostat_C3_protocol_C2Frame_t f;
  byte write = (new_value==nullptr)?0:0x57;
  int new_val= (new_value==nullptr)?0:(*new_value);
  byte _param=(param<100)?param:99;
  byte payload[] = {write,0x30,0x30,0x30,0x78,0x78,0x78};
  Termostat_utils_value2char(_param,nullptr,nullptr,(char*)&payload[2],(char*)&payload[3]);
  if (write!=0)
  {
    Termostat_utils_value2char(new_val,nullptr,(char*)&payload[4],(char*)&payload[5],(char*)&payload[6]);
  }
  /* 0x43 = 'C' ; 0x57 = 'W' ; 0x2B = + ; 0x2D = - */
  Termostat_C3_protocol_Construct_C2Frame(&f,dst,0x43,src,&payload[0]);
  if (out_len_u8!=nullptr)(*out_len_u8)=sizeof(f);
  memcpy(buffer_out_p,&f,sizeof(f));
}

void Termostat_C3_protocol_TemperatureRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte tempnr)
{
  Termostat_C3_protocol_C3Frame_t f;
  byte payload[]={0,0,tempnr,0,0,0};
  /* 0x54 = 'T' */
  Termostat_C3_protocol_Construct_C3Frame(&f,dst,0x54,src,&payload[0]);
  if (out_len_u8!=nullptr)(*out_len_u8)=sizeof(f);
  memcpy(buffer_out_p,&f,sizeof(f));
}

void Termostat_C3_protocol_ParameterRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte param,int *new_value)
{
  Termostat_C3_protocol_C3Frame_t f;
  byte write = (new_value==nullptr)?0:0x57;
  int new_val= (new_value==nullptr)?0:(*new_value);
  byte _param=(param<100)?param:99;
  byte payload[]={write,0,_param,0,0,0};
  if (write!=0)Termostat_utils_C3_protocol_Parameter(new_val,&payload[3],&payload[4],&payload[5]);
  /* 0x42 = 'B' ; 0x57 = 'W' ; 0x2B = + ; 0x2D = - */
  Termostat_C3_protocol_Construct_C3Frame(&f,dst,0x42,src,&payload[0]);
  if (out_len_u8!=nullptr)(*out_len_u8)=sizeof(f);
  memcpy(buffer_out_p,&f,sizeof(f));
}

void Termostat_C3_protocol_PopulateTemperature(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,int integral,byte fractal) /*TODO*/
{
  Termostat_C3_protocol_C3Frame_t f;
  /* 0x40 = '@'   */
  /*=> 11 ;  E4 40 C 4B 0 3 3C 0 0 0 23*/
  uint16_t t = Termostat_utils_C3_protocol_R311RawTempValue(integral,fractal);
  byte payload[]={0,(t>>8)&0xFF,(t>>0)&0xFF,0,0,0};
  Termostat_C3_protocol_Construct_C3Frame(&f,dst,0x40,src,&payload[0]);
  if (out_len_u8!=nullptr)(*out_len_u8)=sizeof(f);
  memcpy(buffer_out_p,&f,sizeof(f));
}

void Termostat_C3_protocol_PopulateTime(void *buffer_out_p,byte *out_len_u8,byte dst,byte src) /*TODO*/
{
  Termostat_C3_protocol_C2Frame_t f;
  byte payload[]={0,0,0,0,0,0,0,0};
  /* 0x68 = 'h'   */
  Termostat_C3_protocol_Construct_C2Frame(&f,dst,0x68,src,&payload[0]);
  if (out_len_u8!=nullptr)(*out_len_u8)=sizeof(f);
  memcpy(buffer_out_p,&f,sizeof(f));
}

void Termostat_C3_protocol_ExecuteRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte idx) /*TODO*/
{
  Termostat_C3_protocol_C3Frame_t f;
  byte _idx=(idx<100)?idx:99;
  byte payload[]={0,_idx,0,0,0,0};
  /* 0x58 = 'X'   */
  /*Need to figure out for what 'X' stands for, but somehow working for nano*/
  Termostat_C3_protocol_Construct_C3Frame(&f,dst,0x58,src,&payload[0]);
  if (out_len_u8!=nullptr)(*out_len_u8)=sizeof(f);
  memcpy(buffer_out_p,&f,sizeof(f));
}

void Termostat_C2_protocol_ExecuteRequest(void *buffer_out_p,byte *out_len_u8,byte dst,byte src,byte idx)
{
  Termostat_C3_protocol_C2Frame_t f;
  byte _idx=(idx<100)?idx:99;
  byte payload[]={0x30,0x30,0x78,0x78,0x78,0x78,0x78};
  /* 0x58 = 'X'   */
  /*Need to figure out for what 'X' stands for, but somehow working for nano*/
  Termostat_utils_value2char(_idx,nullptr,nullptr,(char*)&payload[0],(char*)&payload[1]);
  Termostat_C3_protocol_Construct_C2Frame(&f,dst,0x58,src,&payload[0]);
  if (out_len_u8!=nullptr)(*out_len_u8)=sizeof(f);
  memcpy(buffer_out_p,&f,sizeof(f));
}
