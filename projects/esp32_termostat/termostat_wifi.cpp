#include "termostat_wifi.hpp"
#include "termostat_nvm.hpp"
#include "termostat_debug.hpp"
#include "termostat_ctrlnode.hpp"
#include "termostat_hardware.hpp"
#include "Arduino.h"
#include <cstdio>
#include <cstring>

#define TERMOSTAT_WIFI_RECONNECTIONS_TIMEOUT (5)
#define TERMOSTAT_WIFI_SSID     TERMOSTAT_NVM_STRUCT_RAM.Termostat_Wifi_ssid_au8
#define TERMOSTAT_WIFI_PASSWORD TERMOSTAT_NVM_STRUCT_RAM.Termostat_Wifi_password_au8
#define TERMOSTAT_WIFI_CONNECT  TERMOSTAT_NVM_STRUCT_RAM.Termostat_Wifi_connect_u8

static byte Termostat_wifi_KeepConnect = 0;
static int Termostat_wifi_status = WL_DISCONNECTED;
static int Termostat_wifi_reconnection_remains = TERMOSTAT_WIFI_RECONNECTIONS_TIMEOUT;

static int Termostat_wifi_CheckConnection(void);
static String Termostat_wifi_Get_WiFiStatus(int Status);
static String Termostat_wifi_Get_EncryptMethod(int method);
static void Termostat_wifi_WiFiEventHandler(WiFiEvent_t event);

void Termostat_wifi_Init(void)
{
    TERMOSTAT_WIFI_DEVICE.scanDelete();
    TERMOSTAT_WIFI_DEVICE.mode(WIFI_STA);
    TERMOSTAT_WIFI_DEVICE.disconnect();
    TERMOSTAT_WIFI_DEVICE.onEvent(Termostat_wifi_WiFiEventHandler);
    Termostat_wifi_status = WL_DISCONNECTED;
    Termostat_wifi_reconnection_remains = TERMOSTAT_WIFI_RECONNECTIONS_TIMEOUT;

    if (TERMOSTAT_WIFI_CONNECT!=0)
    {
        TERMOSTAT_WIFI_DEVICE.begin(TERMOSTAT_WIFI_SSID, TERMOSTAT_WIFI_PASSWORD);
        Termostat_wifi_KeepConnect = 1;
        (void)Termostat_wifi_CheckConnection();
    }
}

void Termostat_wifi_ConnectID(int id,const char *password)
{
   Termostat_wifi_Connect(TERMOSTAT_WIFI_DEVICE.SSID(id).c_str(),password);
}

void Termostat_wifi_Connect(const char *ssid,const char *password)
{
  if (ssid==nullptr)return;
  Termostat_wifi_KeepConnect = 0;
  TERMOSTAT_WIFI_CONNECT=1;
  memset(&TERMOSTAT_WIFI_SSID[0],0,TERMOSTAT_NVM_WIFI_SSID_SIZE);
  memset(&TERMOSTAT_WIFI_PASSWORD[0],0,TERMOSTAT_NVM_WIFI_PASSWORD_SIZE);
  memcpy(&TERMOSTAT_WIFI_SSID[0],&ssid[0],strlen(ssid));
  if (password!=nullptr)memcpy(&TERMOSTAT_WIFI_PASSWORD[0],&password[0],strlen(password));
  Termostat_wifi_status = WL_DISCONNECTED;
  Termostat_wifi_reconnection_remains = TERMOSTAT_WIFI_RECONNECTIONS_TIMEOUT;
  TERMOSTAT_WIFI_DEVICE.disconnect();
  TERMOSTAT_WIFI_DEVICE.begin(TERMOSTAT_WIFI_SSID, TERMOSTAT_WIFI_PASSWORD);
  Termostat_wifi_KeepConnect = 1;
}

void Termostat_wifi_Disconnect(void)
{
  Termostat_wifi_KeepConnect = 0;
  Termostat_wifi_status = WL_DISCONNECTED;
  TERMOSTAT_WIFI_CONNECT=0;
  TERMOSTAT_WIFI_DEVICE.disconnect();
}

void Termostat_wifi_Reconnect(void)
{
  if ((TERMOSTAT_WIFI_CONNECT!=0)&&(Termostat_wifi_KeepConnect==0))
  {
    Termostat_wifi_reconnection_remains = TERMOSTAT_WIFI_RECONNECTIONS_TIMEOUT;
    Termostat_wifi_KeepConnect=1;
  }
}

void Termostat_wifi_Mainfunction(void)
{
  Termostat_wifi_CheckConnection();
}

void Termostat_wifi_printInfo(void)
{
  TERMOSTAT_DEBUG_FNC("WIFI_STATUS: %s\n",Termostat_wifi_Get_WiFiStatus(TERMOSTAT_WIFI_DEVICE.status()).c_str());
  TERMOSTAT_DEBUG_FNC("SSID: %s / %s\n",TERMOSTAT_WIFI_DEVICE.SSID().c_str(),TERMOSTAT_WIFI_DEVICE.BSSIDstr().c_str());
  TERMOSTAT_DEBUG_FNC("RSSI: %4d\n",TERMOSTAT_WIFI_DEVICE.RSSI());
  TERMOSTAT_DEBUG_FNC("IP_ADDRESS: %s / %s\n",TERMOSTAT_WIFI_DEVICE.localIP().toString().c_str(),TERMOSTAT_WIFI_DEVICE.subnetMask().toString().c_str());
  TERMOSTAT_DEBUG_FNC("MAC_ADDRESS: %s\n",TERMOSTAT_WIFI_DEVICE.macAddress().c_str());
  TERMOSTAT_DEBUG_FNC("CHANNEL: %2d\n", TERMOSTAT_WIFI_DEVICE.channel());
  TERMOSTAT_DEBUG_FNC("MULTICAST: %d.%d.%d.%d:%d\n", TERMOSTAT_CTRLNODE_MULTICAST_IP0,TERMOSTAT_CTRLNODE_MULTICAST_IP1,TERMOSTAT_CTRLNODE_MULTICAST_IP2,TERMOSTAT_CTRLNODE_MULTICAST_IP3,TERMOSTAT_CTRLNODE_MULTICAST_PORT);
}


static int Termostat_wifi_CheckConnection(void)
{
  int timeout = 10;
  int connected = 0;
  if (Termostat_wifi_KeepConnect==1)
  {
    if ((Termostat_wifi_reconnection_remains>0) && ((TERMOSTAT_WIFI_DEVICE.status() != WL_CONNECTED)))
    {
      while((TERMOSTAT_WIFI_DEVICE.status() != WL_CONNECTED) && (timeout>0))
      {
        Termostat_hardware_SleepMs(100);
        --timeout;
      }
      Termostat_wifi_status = TERMOSTAT_WIFI_DEVICE.status();
      connected = (Termostat_wifi_status!= WL_CONNECTED)?0:1;
      if(connected)
      {
          Termostat_wifi_reconnection_remains = TERMOSTAT_WIFI_RECONNECTIONS_TIMEOUT;
      }
      else
      {
         --Termostat_wifi_reconnection_remains;
         TERMOSTAT_WIFI_DEVICE.begin(TERMOSTAT_WIFI_SSID, TERMOSTAT_WIFI_PASSWORD);
      }
    }
    else
    {
      Termostat_wifi_KeepConnect=0;
    }
  }
  return connected;
}

int Termostat_wifi_ScanNetworks(int print)
{
    TERMOSTAT_WIFI_DEVICE.scanDelete();
    int n = TERMOSTAT_WIFI_DEVICE.scanNetworks();
    if (print==0)return n;
    if(n==0)
    {
        TERMOSTAT_DEBUG_FNC("No Networks found....\n");
    }
    else
    {
        TERMOSTAT_DEBUG_FNC("Networks found....\n");
        TERMOSTAT_DEBUG_FNC("Nr | SSID                             | RSSI | CH | Encryption\n");
        for (int i = 0; i < n; ++i) 
        {
            // Print SSID and RSSI for each network found
            TERMOSTAT_DEBUG_FNC("%2d",i);
            TERMOSTAT_DEBUG_FNC(" | ");
            TERMOSTAT_DEBUG_FNC("%-32.32s", TERMOSTAT_WIFI_DEVICE.SSID(i).c_str());
            TERMOSTAT_DEBUG_FNC(" | ");
            TERMOSTAT_DEBUG_FNC("%4d", TERMOSTAT_WIFI_DEVICE.RSSI(i));
            TERMOSTAT_DEBUG_FNC(" | ");
            TERMOSTAT_DEBUG_FNC("%2d", TERMOSTAT_WIFI_DEVICE.channel(i));
            TERMOSTAT_DEBUG_FNC(" | ");
            TERMOSTAT_DEBUG_FNC("%s",Termostat_wifi_Get_EncryptMethod(TERMOSTAT_WIFI_DEVICE.encryptionType(i)));
            TERMOSTAT_DEBUG_FNC("\n");
            Termostat_hardware_SleepMs(10);
        }
    }
    return n;
}

bool Termostat_wifi_isConnected(void)
{
  int connected = 0;
  connected = (Termostat_wifi_status != WL_CONNECTED)?0:1;
  return connected;
}

static void Termostat_wifi_WiFiEventHandler(WiFiEvent_t event)
{
  switch (event) 
  {
    case IP_EVENT_STA_GOT_IP:
      break;
    case WIFI_EVENT_STA_DISCONNECTED:
      break;
  }
}


static String Termostat_wifi_Get_EncryptMethod(int method)
{
    switch(method)
    {
      case WIFI_AUTH_OPEN:
        return("open");
      case WIFI_AUTH_WEP:
        return("WEP");
      case WIFI_AUTH_WPA_PSK:
        return("WPA");
      case WIFI_AUTH_WPA2_PSK:
        return("WPA2");
      case WIFI_AUTH_WPA_WPA2_PSK:
        return("WPA+WPA2");
      case WIFI_AUTH_WPA2_ENTERPRISE:
        return("WPA2-EAP");
      case WIFI_AUTH_WPA3_PSK:
        return("WPA3");
      case WIFI_AUTH_WPA2_WPA3_PSK:
        return("WPA2+WPA3");
      case WIFI_AUTH_WAPI_PSK:
        return("WAPI");
      default:
        return("unknown");
    }
    return "WL_UNKNOW";
}

static String Termostat_wifi_Get_WiFiStatus(int Status)
{
    switch(Status)
    {
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
        case WL_CONNECTED:
        return "WL_CONNECTED";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    }
    return "WL_UNKNOW";
}