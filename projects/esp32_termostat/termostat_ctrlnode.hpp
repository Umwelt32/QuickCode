#pragma once
#include "Arduino.h"
#include "termostat_nvm.hpp"

#define TERMOSTAT_CTRLNODE_MULTICAST_IP0  (TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_ip[0])
#define TERMOSTAT_CTRLNODE_MULTICAST_IP1  (TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_ip[1])
#define TERMOSTAT_CTRLNODE_MULTICAST_IP2  (TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_ip[2])
#define TERMOSTAT_CTRLNODE_MULTICAST_IP3  (TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_ip[3])
#define TERMOSTAT_CTRLNODE_MULTICAST_PORT (TERMOSTAT_NVM_STRUCT_RAM.Termostat_ctrl_udp_port)

void Termostat_ctrlnode_Init(void);
void Termostat_ctrlnode_SetMulticastIp(byte *ip,uint16_t *port);
bool Termostat_ctrlnode_UdpSendPacket(const void *p,int len);
void Termostat_ctrlnode_Mainfunction(void);
void Termostat_ctrlnode_printUdpStats(void);