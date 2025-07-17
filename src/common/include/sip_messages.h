#ifndef __FOMODON_SIP_MSGS_H__
#define __FOMODON_SIP_MSGS_H__

void sip_initialize_context();
int sip_prepare_register_request(char* msg_buf, unsigned int buf_size);

#endif