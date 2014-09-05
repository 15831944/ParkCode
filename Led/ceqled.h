#ifndef CEQLED_H
#define CEQLED_H

#include <QThread>
#include "cledevent.h"
#include <windows.h>
#include "EQ2008_Dll.h"

class CEqLed : public QThread
{
    Q_OBJECT
public:

    static CEqLed* CreateInstance( );
    void PostText( QString& strText );

protected:
    void run( );
    void customEvent( QEvent* e );

private:
    explicit CEqLed(QObject *parent = 0);

    inline void PostEvent( CLedEvent* pEvent );
    void GetFunctionPointer( );
    void ProcessPublishTextEvent( CLedEvent* pEvent );

private:
    static CEqLed* pThreadInstance;
    HMODULE hDllMod;

    typedef int ( WINAPI* User_AddProgram )( int CardNum, BOOL bWaitToEnd, int iPlayTime );

    //���ͼ����
    typedef int  ( WINAPI* User_AddBmpZone )( int CardNum,User_Bmp *pBmp,int iProgramIndex);
    typedef BOOL ( WINAPI* User_AddBmp )( int CardNum,int iBmpPartNum,HBITMAP hBitmap,User_MoveSet* pMoveSet,int iProgramIndex);
    typedef BOOL ( WINAPI* User_AddBmpFile )( int CardNum,int iBmpPartNum,char *strFileName,User_MoveSet* pMoveSet,int iProgramIndex);

    //����ı���
    typedef int ( WINAPI* User_AddText )( int CardNum,User_Text *pText,int iProgramIndex);

    //���RTF��
    typedef int ( WINAPI* User_AddRTF )( int CardNum,User_RTF *pRTF,int iProgramIndex);

    //��ӵ����ı���
    typedef int ( WINAPI* User_AddSingleText )( int CardNum,User_SingleText *pSingleText,int iProgramIndex);

    //���ʱ����
    typedef int ( WINAPI* User_AddTime )( int CardNum,User_DateTime *pDateTime,int iProgramIndex);

    //��Ӽ�ʱ��
    typedef int ( WINAPI* User_AddTimeCount )( int CardNum,User_Timer *pTimeCount,int iProgramIndex);

    //����¶���
    typedef int ( WINAPI* User_AddTemperature )( int CardNum,User_Temperature *pTemperature,int iProgramIndex);

    //ɾ����Ŀ
    typedef BOOL ( WINAPI* User_DelProgram )( int CardNum,int iProgramIndex);

    //ɾ�����н�Ŀ
    typedef BOOL ( WINAPI* User_DelAllProgram )( int CardNum);

    //��������
    typedef BOOL ( WINAPI* User_SendToScreen )( int CardNum);

    //���ͽ�Ŀ�ļ��������ļ�
    typedef BOOL ( WINAPI* User_SendFileToScreen )( int CardNum,char pSendPath[MAX_PATH],char pIndexPath[MAX_PATH]);

    typedef BOOL ( WINAPI* User_CloseScreen )( int CardNum);

    //����
    typedef BOOL ( WINAPI* User_OpenScreen )( int CardNum);

    //У���忨��ʱ��
    typedef BOOL ( WINAPI* User_AdjustTime )( int CardNum);

    //ʵʱ��������
    typedef BOOL ( WINAPI* User_RealtimeConnect )( int CardNum);	 //��������
    typedef BOOL ( WINAPI* User_RealtimeSendData )( int CardNum,int x,int y,int iWidth,int iHeight,HBITMAP hBitmap);	 //��������
    typedef BOOL ( WINAPI* User_RealtimeDisConnect )( int CardNum); //�Ͽ�����

    typedef BOOL ( WINAPI* User_RealtimeSendBmpData )( int CardNum,int x,int y,int iWidth,int iHeight,char *strFileName);	 //��������

    typedef BOOL ( WINAPI* User_ImportIniFile )( char*	pPath,int iLength); //����ini�����ļ�

    //���ȵ���
    typedef BOOL ( WINAPI* User_SetScreenLight )( int CardNum,int iLightDegreen);

    User_AddProgram MyUser_AddProgram;
    User_AddBmpZone MyUser_AddBmpZone;
    User_AddBmp MyUser_AddBmp;
    User_AddBmpFile MyUser_AddBmpFile;
    User_AddText MyUser_AddText;
    User_AddRTF MyUser_AddRTF;
    User_AddSingleText MyUser_AddSingleText;
    User_AddTime MyUser_AddTime;
    User_AddTimeCount MyUser_AddTimeCount;
    User_AddTemperature MyUser_AddTemperature;
    User_DelProgram MyUser_DelProgram;
    User_DelAllProgram MyUser_DelAllProgram;
    User_SendToScreen MyUser_SendToScreen;
    User_SendFileToScreen MyUser_SendFileToScreen;
    User_CloseScreen MyUser_CloseScreen;
    User_OpenScreen MyUser_OpenScreen;
    User_AdjustTime MyUser_AdjustTime;
    User_RealtimeConnect MyUser_RealtimeConnect;
    User_RealtimeSendData MyUser_RealtimeSendData;
    User_RealtimeDisConnect MyUser_RealtimeDisConnect;
    User_RealtimeSendBmpData MyUser_RealtimeSendBmpData;
    User_ImportIniFile MyUser_ImportIniFile;
    User_SetScreenLight MyUser_SetScreenLight;
    
signals:
    
public slots:
    
};

#endif // CEQLED_H
