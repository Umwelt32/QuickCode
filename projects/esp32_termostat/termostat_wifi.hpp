#pragma once
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include "Arduino.h"

#define TERMOSTAT_WIFI_DEVICE WiFi

int Termostat_wifi_ScanNetworks(int print);
void Termostat_wifi_Init(void);
void Termostat_wifi_ConnectID(int id,const char *password);
void Termostat_wifi_Connect(const char *ssid,const char *password);
void Termostat_wifi_Disconnect(void);
void Termostat_wifi_Mainfunction(void);
void Termostat_wifi_printInfo(void);
bool Termostat_wifi_isConnected(void);
void Termostat_wifi_Reconnect(void);
