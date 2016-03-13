//ARINC429DRV.h  //429驱动函数声明
#define A429FUNCNUM 15
typedef void * WDC_DEVICE_HANDLE; 
const char A429FUN[A429FUNCNUM][100]=
{
	 "DeviceFindAndOpen",
	 "OBT429TOPCI_LibInit",
	 "RX_DATA",
	 "TX1_SEND" ,
	 "TX2_SEND",
	 "TX1_INIT" ,
	 "TX2_SEND",
	 "RX_ENABLE",
	 "TX_ENABLE",
	 "DeviceClose",
	 "OBT429_RESET",
	 "RX1_INIT",
	 "RX2_INIT",
	 "RX3_INIT" ,
	 "RX4_INIT"

};
 WDC_DEVICE_HANDLE (*pDeviceFindAndOpen)(int Board_num);//初始化函数                                              
 DWORD (*pOBT429TOPCI_LibInit)(void);                                                                             
 int (*pRX_DATA)( WDC_DEVICE_HANDLE hDev, unsigned int RX_N, unsigned int RX_DATA[256], unsigned int fifo_leve); 
                                                                                                       
 int (*pTX1_SEND)(WDC_DEVICE_HANDLE hDev, unsigned int txdata[64], int fifo_leve);                               
 int (*pTX2_SEND)(WDC_DEVICE_HANDLE hDev, unsigned int txdata[64], int fifo_leve); 

  void (*pTX1_INIT)(WDC_DEVICE_HANDLE hDev, unsigned int enable, unsigned int word_length, unsigned int parity, unsigned int scaler, unsigned int tgap, unsigned int mode);                  
  void (*pTX2_INIT)(WDC_DEVICE_HANDLE hDev, unsigned int enable, unsigned int word_length, unsigned int parity, unsigned int scaler, unsigned int tgap, unsigned int mode);                  
                                                                                                                                                                                   
  void (*pRX_ENABLE)(WDC_DEVICE_HANDLE hDev,unsigned int num,unsigned int enable);                                                                                                           
  void (*pTX_ENABLE)(WDC_DEVICE_HANDLE hDev,unsigned int num,unsigned int enable);                                                                                                           
                                                                                                                                                                                   
  void (*pDeviceClose)(WDC_DEVICE_HANDLE hDev);  //关闭板卡                                                                                                                                  
  void (*pOBT429_RESET)(WDC_DEVICE_HANDLE hDev);//板卡复位                                                                                                                                                   

  void (*pRX1_INIT)(WDC_DEVICE_HANDLE hDev,
			  unsigned int enable,		// (0, 1)
			  unsigned int word_length,		// (25, 32)
			  unsigned int parity_select, 	// (0,1,2)
			  unsigned int label_enable,	// (0,1)
			  unsigned int sdi, 		// (0,1)
              unsigned int scaler, 			// (0~65535)波特率
			  unsigned int label1,
			  unsigned int label2,
			  unsigned int label3,
			  unsigned int label4,
			  unsigned int label5,
			  unsigned int label6,
			  unsigned int label7 
			 );
  void (*pRX2_INIT)(WDC_DEVICE_HANDLE hDev,
			  unsigned int enable,		// (0, 1)
			  unsigned int word_length,		// (25, 32)
			  unsigned int parity_select, 	// (0,1,2)
			  unsigned int label_enable,	// (0,1)
			  unsigned int sdi, 		// (0,1)
              unsigned int scaler, 			// (0~65535)波特率
			  unsigned int label1,
			  unsigned int label2,
			  unsigned int label3,
			  unsigned int label4,
			  unsigned int label5,
			  unsigned int label6,
			  unsigned int label7 
			 );
   void (*pRX3_INIT)(WDC_DEVICE_HANDLE hDev,
			  unsigned int enable,		// (0, 1)
			  unsigned int word_length,		// (25, 32)
			  unsigned int parity_select, 	// (0,1,2)
			  unsigned int label_enable,	// (0,1)
			  unsigned int sdi, 		// (0,1)
              unsigned int scaler, 			// (0~65535)波特率
			  unsigned int label1,
			  unsigned int label2,
			  unsigned int label3,
			  unsigned int label4,
			  unsigned int label5,
			  unsigned int label6,
			  unsigned int label7 
			 );
  void (*pRX4_INIT)(WDC_DEVICE_HANDLE hDev,
			  unsigned int enable,		// (0, 1)
			  unsigned int word_length,		// (25, 32)
			  unsigned int parity_select, 	// (0,1,2)
			  unsigned int label_enable,	// (0,1)
			  unsigned int sdi, 		// (0,1)
              unsigned int scaler, 			// (0~65535)波特率
			  unsigned int label1,
			  unsigned int label2,
			  unsigned int label3,
			  unsigned int label4,
			  unsigned int label5,
			  unsigned int label6,
			  unsigned int label7 
			 );
