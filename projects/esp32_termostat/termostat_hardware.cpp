/*--------------------------------------------------------------*/
#include <DHT11.h>
#include "Arduino.h"
#include "termostat_hardware.hpp"
#include "termostat_debug.hpp"
/*------------------------------------------------------------*/
#define TERMOSTAT_GPIO_INOUT_DHT11_PIN  (21)
#define TERMOSTAT_GPIO_OUT_RS485_EN_PIN (22)
#define TERMOSTAT_GPIO_OUT_ALARM_EN_PIN (18)
#define TERMOSTAT_GPIO_INPUT_MQ9D_PIN (19)
#define TERMOSTAT_GPIO_INPUT_MQ9A_PIN (34)
#define TERMOSTAT_RS485_RXD_PIN (16)
#define TERMOSTAT_RS485_TXD_PIN (17)
#define TERMOSTAT_RS485_SERIAL_NODE Serial1
#define TERMOSTAT_RS485_SERIAL_TIMEOUT_MS (1000)
#define TERMOSTAT_RS485_SERIAL_DEFAULT_BAUDRATE (1200)
#define TERMOSTAT_SERIAL_DEBUG_BAUDRATE   (9600) 
/*------------------------------------------------------------*/
typedef struct
{
  hw_time_t Termostat_hardware_stm_ms;
  int Termostat_hardware_running_core;
  int Termostat_hardware_MQ9_value;
  int Termostat_hardware_MQ9_digital;
  int Termostat_hardware_DHT11_temp;
  int Termostat_hardware_DHT11_humi;
  int Termostat_hardware_DHT11_result;
  byte Termostat_hardware_disable_DHT11;
  byte Termostat_hardware_disable_MQ9;
  uint16_t Termostat_hardware_Cycle;
}Termostat_hardware_sensors_t;
/*------------------------------------------------------------*/
/*-----------------------------------------------------------*/
static DHT11 dht11(TERMOSTAT_GPIO_INOUT_DHT11_PIN); /*DHT11 sensor on PIN 21*/
static Termostat_hardware_sensors_t Termostat_hardware_sensors;
/*----------------------------------------------------------*/

void Termostat_hardware_MQ9_Disable(bool disabled)
{
  Termostat_hardware_sensors.Termostat_hardware_disable_MQ9 = (disabled!=0)?true:false;
}

void Termostat_hardware_DHT11_Disable(bool disabled)
{
  Termostat_hardware_sensors.Termostat_hardware_disable_DHT11 = (disabled!=0)?true:false;
}

int Termostat_hardware_ReadMQ9_Analog(void)
{
  return Termostat_hardware_sensors.Termostat_hardware_MQ9_value;
}

int Termostat_hardware_ReadMQ9_Digital(void)
{
  return Termostat_hardware_sensors.Termostat_hardware_MQ9_digital;
}

void Termostat_hardware_WriteAlarm_Digital(int v)
{
  if (v==0)
  {
    digitalWrite(TERMOSTAT_GPIO_OUT_ALARM_EN_PIN,LOW);
    pinMode(TERMOSTAT_GPIO_OUT_ALARM_EN_PIN, INPUT ); /*Buzzer output pin*/
  }
  else
  {
     pinMode(TERMOSTAT_GPIO_OUT_ALARM_EN_PIN, OUTPUT );
     digitalWrite(TERMOSTAT_GPIO_OUT_ALARM_EN_PIN,HIGH);
  }
}

int Termostat_hardware_ReadDHT11(int *temp, int *huminity)
{
  if(temp!=nullptr)(*temp)=Termostat_hardware_sensors.Termostat_hardware_DHT11_temp;
  if(huminity!=nullptr)(*huminity)=Termostat_hardware_sensors.Termostat_hardware_DHT11_humi;
  return Termostat_hardware_sensors.Termostat_hardware_DHT11_result;
}

void Termostat_hardware_RS485_SetBaudRate(uint16_t brate)
{
     if (TERMOSTAT_RS485_SERIAL_NODE.baudRate()!=brate)
     {
         TERMOSTAT_RS485_SERIAL_NODE.updateBaudRate(brate);
     }
}

void Termostat_hardware_RS485_SetTimeout(uint16_t timeout)
{
  if (TERMOSTAT_RS485_SERIAL_NODE.getTimeout()!=timeout)
  {
      TERMOSTAT_RS485_SERIAL_NODE.setTimeout(timeout);
  }
}

void Termostat_hardware_RS485_EN(bool enabled)
{
  digitalWrite(TERMOSTAT_GPIO_OUT_RS485_EN_PIN,enabled?HIGH:LOW);
}

int Termostat_hardware_RS485_Write(byte *buf,int len,bool readback)
{
  int w;
  Termostat_hardware_RS485_EN(true); /*write Enable for Tx*/
  w=TERMOSTAT_RS485_SERIAL_NODE.write(buf,len);
  TERMOSTAT_RS485_SERIAL_NODE.flush();
  if (readback!=0)Termostat_hardware_RS485_EN(false); /* Disable for Rx*/
  return w;
}

int Termostat_hardware_RS485_Read(byte *buf,int len,bool writeback)
{
  int w;
  Termostat_hardware_RS485_EN(false); /* Disable for Rx*/
  w=TERMOSTAT_RS485_SERIAL_NODE.readBytes(buf,len);
  if (writeback!=0)Termostat_hardware_RS485_EN(true); /* Enable for Tx*/
  return w;
}

void Termostat_hardware_PowerUp_Init(void)
{
  Termostat_hardware_sensors.Termostat_hardware_Cycle=0;
  Termostat_hardware_sensors.Termostat_hardware_disable_MQ9 = false;
  Serial.begin(TERMOSTAT_SERIAL_DEBUG_BAUDRATE); /*Debug serial output*/
  TERMOSTAT_RS485_SERIAL_NODE.begin(TERMOSTAT_RS485_SERIAL_DEFAULT_BAUDRATE, SERIAL_8N1, TERMOSTAT_RS485_RXD_PIN, TERMOSTAT_RS485_TXD_PIN); /*RS485, RX:16, TX:17 pins,1200 boud,8bits*/
  TERMOSTAT_RS485_SERIAL_NODE.setTimeout(TERMOSTAT_RS485_SERIAL_TIMEOUT_MS);
  pinMode(TERMOSTAT_GPIO_OUT_RS485_EN_PIN, OUTPUT); /*RS485 enable/rx pin*/
  pinMode(TERMOSTAT_GPIO_OUT_ALARM_EN_PIN, INPUT ); /*Buzzer output pin*/
  pinMode(TERMOSTAT_GPIO_INPUT_MQ9D_PIN, INPUT); /*MQ-9 sensor digital INPUT pin*/
  pinMode(TERMOSTAT_GPIO_INPUT_MQ9A_PIN, INPUT_PULLUP); /*MQ-9 sensor analog INPUT pin*/
}

void Termostat_hardware_Mainfunction(void)
{
  Termostat_hardware_sensors.Termostat_hardware_running_core = Termostat_hardware_coreID();
  Termostat_hardware_sensors.Termostat_hardware_stm_ms       = millis();

  if (Termostat_hardware_sensors.Termostat_hardware_disable_MQ9!=0)
  {
    Termostat_hardware_sensors.Termostat_hardware_MQ9_value = 0;
    Termostat_hardware_sensors.Termostat_hardware_MQ9_digital = 0;
  }
  else
  {
    Termostat_hardware_sensors.Termostat_hardware_MQ9_value    = analogRead(TERMOSTAT_GPIO_INPUT_MQ9A_PIN);
    Termostat_hardware_sensors.Termostat_hardware_MQ9_digital  = digitalRead(TERMOSTAT_GPIO_INPUT_MQ9D_PIN);
  }

  if(Termostat_hardware_sensors.Termostat_hardware_disable_DHT11!=0)
  {
    Termostat_hardware_sensors.Termostat_hardware_DHT11_result = 0xFFu;
    Termostat_hardware_sensors.Termostat_hardware_DHT11_temp=0u;
    Termostat_hardware_sensors.Termostat_hardware_DHT11_humi=0u;
  }
  else
  {
    Termostat_hardware_sensors.Termostat_hardware_DHT11_result = dht11.readTemperatureHumidity(Termostat_hardware_sensors.Termostat_hardware_DHT11_temp, Termostat_hardware_sensors.Termostat_hardware_DHT11_humi);
  }
  ++Termostat_hardware_sensors.Termostat_hardware_Cycle;
}

hw_time_t Termostat_hardware_clock_ms(void)
{
  return Termostat_hardware_sensors.Termostat_hardware_stm_ms;
}

void Termostat_hardware_Reset(void)
{
  ESP.restart();
}

int Termostat_hardware_coreID(void)
{
   return (int)xPortGetCoreID();
}

void Termostat_hardware_AlarmBeep(uint16_t time_ms)
{
  Termostat_hardware_WriteAlarm_Digital(1);
  Termostat_hardware_SleepMs(time_ms);
  Termostat_hardware_WriteAlarm_Digital(0);
}

void Termostat_hardware_SleepMs(uint32_t time_ms)
{
  delay(time_ms);
}

void Termostat_hardware_printSensorsInfo(void)
{
  TERMOSTAT_DEBUG_FNC("CORE: %d\n",Termostat_hardware_sensors.Termostat_hardware_running_core);
  TERMOSTAT_DEBUG_FNC("STM: %d\n",Termostat_hardware_sensors.Termostat_hardware_stm_ms);
  TERMOSTAT_DEBUG_FNC("MQ9A: %d\n",Termostat_hardware_sensors.Termostat_hardware_MQ9_value);
  TERMOSTAT_DEBUG_FNC("MQ9D: %d\n",Termostat_hardware_sensors.Termostat_hardware_MQ9_digital);
  TERMOSTAT_DEBUG_FNC("DHT11_TEMP: %d\n",Termostat_hardware_sensors.Termostat_hardware_DHT11_temp);
  TERMOSTAT_DEBUG_FNC("DHT11_HUMIDITY: %d\n",Termostat_hardware_sensors.Termostat_hardware_DHT11_humi);
  TERMOSTAT_DEBUG_FNC("DHT11_ERR: %d\n",Termostat_hardware_sensors.Termostat_hardware_DHT11_result);
  TERMOSTAT_DEBUG_FNC("DHT11_DIS: %d\n",Termostat_hardware_sensors.Termostat_hardware_disable_DHT11);
  TERMOSTAT_DEBUG_FNC("MQ9_DIS: %d\n",Termostat_hardware_sensors.Termostat_hardware_disable_MQ9);
  TERMOSTAT_DEBUG_FNC("CYCLE: %d\n",Termostat_hardware_sensors.Termostat_hardware_Cycle);
  if (Termostat_hardware_sensors.Termostat_hardware_DHT11_result!=0)TERMOSTAT_DEBUG_FNC("[DHT11_error]%s\n",DHT11::getErrorString(Termostat_hardware_sensors.Termostat_hardware_DHT11_result).c_str());

}

