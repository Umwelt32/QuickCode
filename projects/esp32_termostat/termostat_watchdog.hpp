#pragma once
#include "Arduino.h"
#include "termostat_hardware.hpp"

void Termostat_watchdog_Init(void);
void Termostat_watchdog_SetEnabled(bool en);
void Termostat_watchdog_Service(bool forced);
