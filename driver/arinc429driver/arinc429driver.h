#include <stdlib.h> 
#include "toolbox.h"

HRESULT load429driver(void);

void (*send429Data)(int ch,int*sendBuffer,int sendLen);

HRESULT unLoad429driver(void);


