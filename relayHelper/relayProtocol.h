#ifndef __RELAY_PROTOCOL_H__
#define __RELAY_PROTOCOL_H__

#include "cvidef.h" 

#ifdef __cplusplus
    extern "C" {
#endif
int getRelayMask(char *key,unsigned int *mask);
void parseRelayProtocol(void);
#ifdef __cplusplus
    }
#endif


#endif		
