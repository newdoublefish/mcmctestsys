// EMBC1000－PCI429－42－exampleDlg.h : header file
//

#if !defined(AFX_EMBC1000PCI42942EXAMPLEDLG_H__5ACC0538_C64A_4316_8DDD_9A1CA8B3A7CF__INCLUDED_)
#define AFX_EMBC1000PCI42942EXAMPLEDLG_H__5ACC0538_C64A_4316_8DDD_9A1CA8B3A7CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef void * WDC_DEVICE_HANDLE;
/////////////////////////////////////////////////////////////////////////////
// CEMBC1000PCI42942exampleDlg dialog

class CEMBC1000PCI42942exampleDlg : public CDialog
{
// Construction
public:
	CEMBC1000PCI42942exampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEMBC1000PCI42942exampleDlg)
	enum { IDD = IDD_EMBC1000PCI42942EXAMPLE_DIALOG };

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEMBC1000PCI42942exampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEMBC1000PCI42942exampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInit();
	afx_msg void OnTX1Init();
	afx_msg void OnTX2Init();
	afx_msg void OnRX1Init();
	afx_msg void OnRX2Init();
	afx_msg void OnRX3Init();
	afx_msg void OnRX4Init();
	afx_msg void OnSendDataTX2();
	afx_msg void OnSendDataTX1();
	afx_msg void OnClose();
	afx_msg void OnRxread();
	afx_msg void OnRx1read();
	afx_msg void OnRx2read();
	afx_msg void OnRx3read();
	afx_msg void OnRx4read();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#pragma comment(lib,"PCI_LINK3.lib")
extern "C" _declspec(dllexport) WDC_DEVICE_HANDLE DeviceFindAndOpen(int Board_num);//初始化函数
extern "C" _declspec(dllexport) DWORD OBT429TOPCI_LibInit(void);   
extern "C" _declspec(dllexport)  int RX_DATA( WDC_DEVICE_HANDLE hDev, unsigned int RX_N, unsigned int RX_DATA[256], unsigned int fifo_leve);

extern "C" _declspec(dllexport)  int TX1_SEND(WDC_DEVICE_HANDLE hDev, unsigned int txdata[64], int fifo_leve);
extern "C" _declspec(dllexport)  int TX2_SEND(WDC_DEVICE_HANDLE hDev, unsigned int txdata[64], int fifo_leve);


extern "C" _declspec(dllexport)  void RX1_INIT(WDC_DEVICE_HANDLE hDev,
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
extern "C" _declspec(dllexport)  void RX2_INIT(WDC_DEVICE_HANDLE hDev,
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
extern "C" _declspec(dllexport)  void RX3_INIT(WDC_DEVICE_HANDLE hDev,
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
extern "C" _declspec(dllexport)  void RX4_INIT(WDC_DEVICE_HANDLE hDev,
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

extern "C" _declspec(dllexport)  void TX1_INIT(WDC_DEVICE_HANDLE hDev, unsigned int enable, unsigned int word_length, unsigned int parity, unsigned int scaler, unsigned int tgap, unsigned int mode);
extern "C" _declspec(dllexport)  void TX2_INIT(WDC_DEVICE_HANDLE hDev, unsigned int enable, unsigned int word_length, unsigned int parity, unsigned int scaler, unsigned int tgap, unsigned int mode);

extern "C" _declspec(dllexport)  void RX_ENABLE(WDC_DEVICE_HANDLE hDev,unsigned int num,unsigned int enable);
extern "C" _declspec(dllexport)  void TX_ENABLE(WDC_DEVICE_HANDLE hDev,unsigned int num,unsigned int enable);

extern "C" _declspec(dllexport)  void DeviceClose(WDC_DEVICE_HANDLE hDev);  //关闭板卡
extern "C" _declspec(dllexport)  void OBT429_RESET(WDC_DEVICE_HANDLE hDev);//板卡复位
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMBC1000PCI42942EXAMPLEDLG_H__5ACC0538_C64A_4316_8DDD_9A1CA8B3A7CF__INCLUDED_)
