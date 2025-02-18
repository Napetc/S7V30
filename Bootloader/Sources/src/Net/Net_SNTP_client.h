#ifndef NET_SNTP_CLIENT_H
  #define NET_SNTP_CLIENT_H


uint32_t Net_SNTP_client_create(NX_IP  *ip_ptr);
uint32_t SNTP_client_delete(void);
void     Net_SNTP_client_task(NX_IP   *ip_ptr);
uint32_t Is_sntp_time_received(void);

#endif // NET_SNTP_CLIENT_H



