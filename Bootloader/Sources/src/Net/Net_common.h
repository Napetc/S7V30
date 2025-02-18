﻿#ifndef NET_COMMON_H
  #define NET_COMMON_H

#define                 PACKETS_IN_POOL     (70)
#define                 PACKET_MAX_SZ       1568

#define                 NX_ARP_CACHE_SIZE   (520)

#define                 DHCP_SERVER_ADDITIONAL_ADDR_NUM  0 // Количестово адресов добавляемых к пулу, если 0, то в пуле только один адрес

#define                 MAX_TLS_CERTIFICATE_SIZE    2000

extern NX_IP            *rndis_ip_ptr;
extern NX_IP            *ecm_host_ip_ptr;

typedef struct
{
  ULONG     ip_address;
  ULONG     network_mask;
  ULONG     gateway_address;

  uint8_t   en_dhcp;
  ULONG     dhcp_subnet_mask;
  ULONG     dhcp_dns_ip;
  ULONG     dhcp_start_ip;
  ULONG     dhcp_end_ip;

} T_app_net_props;


extern NX_PACKET_POOL   net_packet_pool;

void                    Net_packet_pool_init(void);
uint32_t                Init_Net(void);

void                    Net_ftp_client_init(NX_IP *nx_ip);
#endif



