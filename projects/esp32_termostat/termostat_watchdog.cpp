#include "termostat_watchdog.hpp"
#include "termostat_hardware.hpp"
#include "termostat_debug.hpp"
#include <esp_task_wdt.h>


#define TERMOSTAT_WATCHDOG_TIMEOUT_MS      (12000)
#define TERMOSTAT_WATCHDOG_RST_PEROID_MS   (2000)
#define TERMOSTAT_WATCHDOG_CORES_MASK      (0x01<< (0x01)) /*everything running on core1?*/

static hw_time_t Termostat_watchdog_last_TS = 0;
static bool      Termostat_watchdog_enabled = false;

void Termostat_watchdog_Init(void)
{
  const static esp_task_wdt_config_t twdt_config = {TERMOSTAT_WATCHDOG_TIMEOUT_MS,TERMOSTAT_WATCHDOG_CORES_MASK,true};
  esp_task_wdt_init(&twdt_config);
  esp_task_wdt_add(NULL);
  Termostat_watchdog_enabled = true;
  Termostat_watchdog_last_TS = Termostat_hardware_clock_ms();
}

void Termostat_watchdog_SetEnabled(bool en)
{
  Termostat_watchdog_enabled=en;
}

void Termostat_watchdog_Service(bool forced)
{
  hw_time_t current_TS       = Termostat_hardware_clock_ms();
  hw_time_t delta_TS         = (current_TS-Termostat_watchdog_last_TS);
  if ((delta_TS>TERMOSTAT_WATCHDOG_RST_PEROID_MS) || (forced))
  {
      if (Termostat_watchdog_enabled==true)
      {
          Termostat_watchdog_last_TS = current_TS;
          esp_task_wdt_reset();
      }
  }
}

