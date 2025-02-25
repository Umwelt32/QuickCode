      /*---------------------------------------------------------*/
     /* Author: https://github.com/Umwelt32/QuickCode           */
    /* Copyright: 2024                                         */
   /*                                                         */
  /* References:                                             */
 /*                                                         */
/*---------------------------------------------------------*/
#include "termostat_hardware.hpp"
#include "termostat_debug.hpp"
#include "termostat_nvm.hpp"
#include "termostat_wifi.hpp"
#include "termostat_C3_protocol.hpp"
#include "termostat_ctrlnode.hpp"
#include "termostat_watchdog.hpp"

#define TERMOSTAT_LOOP_POST_DELAY_MS (200)

void setup() 
{
  Termostat_hardware_PowerUp_Init();
  Termostat_hardware_WriteAlarm_Digital(0);
  Termostat_hardware_RS485_EN(0);
  Termostat_nvm_Init();
  Termostat_debug_Init();
  Termostat_wifi_Init();
  Termostat_ctrlnode_Init();
  Termostat_watchdog_Init();
  Termostat_hardware_AlarmBeep(1000);
}

void loop() 
{
    Termostat_hardware_Mainfunction();
    Termostat_wifi_Mainfunction();
    Termostat_ctrlnode_Mainfunction();
    Termostat_debug_Mainfunction();
    Termostat_watchdog_Service(false);
    Termostat_hardware_SleepMs(TERMOSTAT_LOOP_POST_DELAY_MS);
}
