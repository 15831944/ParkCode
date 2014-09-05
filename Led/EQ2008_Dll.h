#ifndef __EQ2008_DLL__
#define __EQ2008_DLL__

#ifdef DLL_API
#else
#define DLL_API _declspec(dllimport) 
#endif

#include "DataStruct.h" 

/****************����˵��******************/
 //ͨѶ��ʽ����
#define DEVICE_TYPE_COM		0    //����ͨѶ
#define DEVICE_TYPE_NET		1    //����ͨѶ

//����ͨѶ�ٶȳ���
#define SBR_9600			9600
#define SBR_19200			19200
#define SBR_57600			57600
#define SBR_115200			115200

//EQ3002/2008���ƿ�����
#define EQ3002_I			0
#define EQ3002_II			1
#define EQ3002_III			2
#define EQ2008_I			3
#define EQ2008_II			3
#define EQ2010_I			4
#define EQ2010_II			4
#define EQ2010_64			4
#define EQ2008_IE			5
#define EQ2008_IIE			5
#define EQ3002_IE			6
#define EQ3002_IIE			6

//��ɫ����
#define RED					0x0000FF
#define GREEN				0x00FF00
#define YELLOW				0x00FFFF

//����ֵ����
#define EQ_FALSE			0
#define EQ_TRUE			    1

//��ӽ�Ŀ
DLL_API int __stdcall User_AddProgram(int CardNum,BOOL bWaitToEnd,int iPlayTime);

//���ͼ����
DLL_API int  __stdcall User_AddBmpZone(int CardNum,User_Bmp *pBmp,int iProgramIndex);
DLL_API BOOL __stdcall User_AddBmp(int CardNum,int iBmpPartNum,HBITMAP hBitmap,User_MoveSet* pMoveSet,int iProgramIndex);
DLL_API BOOL __stdcall User_AddBmpFile(int CardNum,int iBmpPartNum,char *strFileName,User_MoveSet* pMoveSet,int iProgramIndex);

//����ı���
DLL_API int __stdcall User_AddText(int CardNum,User_Text *pText,int iProgramIndex);

//���RTF��
DLL_API int __stdcall User_AddRTF(int CardNum,User_RTF *pRTF,int iProgramIndex);

//��ӵ����ı���
DLL_API int __stdcall User_AddSingleText(int CardNum,User_SingleText *pSingleText,int iProgramIndex);

//���ʱ����
DLL_API int __stdcall User_AddTime(int CardNum,User_DateTime *pDateTime,int iProgramIndex);

//��Ӽ�ʱ��
DLL_API int __stdcall User_AddTimeCount(int CardNum,User_Timer *pTimeCount,int iProgramIndex);

//����¶���
DLL_API int __stdcall User_AddTemperature(int CardNum,User_Temperature *pTemperature,int iProgramIndex);

//ɾ����Ŀ
DLL_API BOOL __stdcall User_DelProgram(int CardNum,int iProgramIndex);

//ɾ�����н�Ŀ
DLL_API BOOL __stdcall User_DelAllProgram(int CardNum);

//��������
DLL_API BOOL __stdcall User_SendToScreen(int CardNum);

//���ͽ�Ŀ�ļ��������ļ�
DLL_API BOOL __stdcall User_SendFileToScreen(int CardNum,char pSendPath[MAX_PATH],char pIndexPath[MAX_PATH]);

DLL_API BOOL __stdcall User_CloseScreen(int CardNum);

//����
DLL_API BOOL __stdcall User_OpenScreen(int CardNum);

//У���忨��ʱ��
DLL_API BOOL __stdcall User_AdjustTime(int CardNum);

//ʵʱ��������
DLL_API BOOL __stdcall User_RealtimeConnect(int CardNum);	 //��������
DLL_API BOOL __stdcall User_RealtimeSendData(int CardNum,int x,int y,int iWidth,int iHeight,HBITMAP hBitmap);	 //��������
DLL_API BOOL __stdcall User_RealtimeDisConnect(int CardNum); //�Ͽ�����

DLL_API BOOL __stdcall User_RealtimeSendBmpData(int CardNum,int x,int y,int iWidth,int iHeight,char *strFileName);	 //��������

DLL_API BOOL __stdcall User_ImportIniFile(char*	pPath,int iLength); //����ini�����ļ�

//���ȵ���
DLL_API BOOL __stdcall User_SetScreenLight(int CardNum,int iLightDegreen);

/********************************************************************************/

#endif __EQ2008_DLL__
