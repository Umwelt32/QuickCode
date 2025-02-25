#pragma once
#include <stdio.h>
#include <stdarg.h>
#include "Arduino.h"

#define TERMOSTAT_DEBUG_EN (1)
#define TERMOSTAT_DEBUG_FNC Termostat_debug_dbgfnc

void Termostat_debug_Init(void);
void Termostat_debug_WriteCommandBuffer(void *buffer,int len);
void Termostat_debug_Mainfunction(void);
void Termostat_debug_dbgfnc(const char *fmt,...);
void Termostat_debug_out_buffer(void *out,byte *len);