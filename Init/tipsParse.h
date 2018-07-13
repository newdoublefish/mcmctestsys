#ifndef __TIPSPARSE_H__
#define __TIPSPARSE_H__

#ifdef __cplusplus
    extern "C" {
#endif

HRESULT getTip(char *key,char *value);
void getTipsFromExcel(void);
HashTableType getTipsHashTable();

#ifdef __cplusplus
    }
#endif

#endif 
