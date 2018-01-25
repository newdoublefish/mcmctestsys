#ifndef __RELAY_PROTOCOL_H__
#define __RELAY_PROTOCOL_H__

#include "cvidef.h" 

#ifdef __cplusplus
    extern "C" {
#endif
typedef struct{
	char key[50];
	unsigned int beforeTestMask;
	unsigned int afterTestMask;
	unsigned int mask; //如果是16位的继电器，掩码就是0xffff,如何是32位的继电器,掩码就是0xffffffff
}RelayOperate;
int getRelayMask(char *key,RelayOperate *relayOperate);
void parseRelayProtocol(void);
#ifdef __cplusplus
    }
#endif


#endif		
