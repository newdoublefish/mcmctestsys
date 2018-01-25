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
	unsigned int mask; //�����16λ�ļ̵������������0xffff,�����32λ�ļ̵���,�������0xffffffff
}RelayOperate;
int getRelayMask(char *key,RelayOperate *relayOperate);
void parseRelayProtocol(void);
#ifdef __cplusplus
    }
#endif


#endif		
