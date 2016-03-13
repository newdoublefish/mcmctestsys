
int Load429DRV(int hdl);

int openObtDevice(int val);

void sendObdArinc429Data(int card,int ch,int*sendBuffer,int sendLen);

void closeObtDevice(void);

void recvObdArinc429Data(int card,int ch,unsigned int *A429databuf,int *recvLen)  ;

void recvObtArinc429Enable(int card,int ch,unsigned int enable) ;
