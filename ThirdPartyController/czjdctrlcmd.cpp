#include "czjdctrlcmd.h"

CZjdCtrlCmd::CZjdCtrlCmd(QObject *parent) :
    CControllerCmd(parent)
{
}

void CZjdCtrlCmd::Initialize( QByteArray& byCmdData, quint16 nControllerAddr )
{
    byCmdData.clear( );
    char cAddr = ( char ) nControllerAddr;
    //A5H+����+����+20H+20H / Return B5 31 31
    char cCmd[ ] = { 0xA5, cAddr, cAddr, 0x20, 0x20 };

    byCmdData.append( cCmd, sizeof (  cCmd ) / sizeof ( cCmd[ 0 ] ) );
}

void CZjdCtrlCmd::ModifyTime( QByteArray& byCmdData, quint16 nControllerAddr )
{
    byCmdData.clear( );
    char cAddr = ( char ) nControllerAddr;
    //A5H+����+����+31H+31H+������ʱ�������ڣ�7�ֽڣ�+У�飨1��
    char cHYear = 0;
    char clYear = 0;
    char cMonnth = 0;
    char cDay = 0;
    char cMinute = 0;
    char cSecond = 0;
    char cWeek = 0;
    char cCheck = 0;

    GetDateTimeWeek( cHYear, clYear, cDay, cMonnth, cDay, cMinute, cSecond, cWeek );
    char cCmd[ ] = { 0xA5, cAddr, cAddr, 0x31, 0x31, 0x84, 0x84, 0x01, 0x01 };

    byCmdData.append( cCmd, sizeof (  cCmd ) / sizeof ( cCmd[ 0 ] ) );
}

void CZjdCtrlCmd::ReadTime( QByteArray& byCmdData, quint16 nControllerAddr )
{
    byCmdData.clear( );
    char cAddr = ( char ) nControllerAddr;
    //A5H+����+����+32H+32H / Return B5 30 30 14 09 01 09 34 59 01 79
    char cCmd[ ] = { 0xA5, cAddr, cAddr, 0x32, 0x32 };

    byCmdData.append( cCmd, sizeof (  cCmd ) / sizeof ( cCmd[ 0 ] ) );
}

void CZjdCtrlCmd::SelectWorkMode( QByteArray& byCmdData, quint16 nControllerAddr, bool bOffline )
{
    byCmdData.clear( );
    char cAddr = ( char ) nControllerAddr;
    //A5H+����+����+34H+34H+����1+����1+����2+����2
    char cCmd[ ] = { 0xA5, cAddr, cAddr, 0x34, 0x34, 0x84, 0x84, 0x01, 0x01 };

    byCmdData.append( cCmd, sizeof (  cCmd ) / sizeof ( cCmd[ 0 ] ) );
}

void CZjdCtrlCmd::GetOpenGateCmd( QByteArray& byCmdData, quint16 nControllerAddr )
{
    byCmdData.clear( );
    char cAddr = ( char ) nControllerAddr;
    //A5H+����+����+34H+34H+����1+����1+����2+����2
    char cCmd[ ] = { 0xA5, cAddr, cAddr, 0x34, 0x34, 0x0C, 0x0C, 0x01, 0x01 };

    byCmdData.append( cCmd, sizeof (  cCmd ) / sizeof ( cCmd[ 0 ] ) );
}

void CZjdCtrlCmd::GetCloseGateCmd( QByteArray& byCmdData, quint16 nControllerAddr )
{
    byCmdData.clear( );
    char cAddr = ( char ) nControllerAddr;
    //A5H+����+����+34H+34H+����1+����1+����2+����2
    char cCmd[ ] = { 0xA5, cAddr, cAddr, 0x34, 0x34, 0x02, 0x02, 0x01, 0x01 };

    byCmdData.append( cCmd, sizeof (  cCmd ) / sizeof ( cCmd[ 0 ] ) );
}

void CZjdCtrlCmd::GetStopGateCmd( QByteArray& byCmdData, quint16 nControllerAddr )
{
    byCmdData.clear( );
    char cAddr = ( char ) nControllerAddr;
    //A5H+����+����+34H+34H+����1+����1+����2+����2
    char cCmd[ ] = { 0xA5, cAddr, cAddr, 0x34, 0x34, 0x01, 0x01, 0x01, 0x01 };

    byCmdData.append( cCmd, sizeof (  cCmd ) / sizeof ( cCmd[ 0 ] ) );
}

void CZjdCtrlCmd::GetRejectOpenGateCmd( QByteArray& byCmdData, quint16 nControllerAddr )
{
    byCmdData.clear( );
    char cAddr = ( char ) nControllerAddr;
    //A5H+����+����+34H+34H+����1+����1+����2+����2
    char cCmd[ ] = { 0xA5, cAddr, cAddr, 0x34, 0x34, 0x84, 0x84, 0x01, 0x01 };

    byCmdData.append( cCmd, sizeof (  cCmd ) / sizeof ( cCmd[ 0 ] ) );
}

void CZjdCtrlCmd::CZjdCtrlCmd::ReadRecordState( QByteArray& byCmdData, quint16 nControllerAddr )
{
    byCmdData.clear( );
    char cAddr = ( char ) nControllerAddr;
    //A5H+����+����+3CH+3CH
    char cCmd[ ] = { 0xA5, cAddr, cAddr, 0x3C, 0x3C };

    byCmdData.append( cCmd, sizeof (  cCmd ) / sizeof ( cCmd[ 0 ] ) );
}

void CZjdCtrlCmd::CZjdCtrlCmd::ReadCardID( QByteArray& byCmdData, quint16 nControllerAddr )
{
    byCmdData.clear( );
    char cAddr = ( char ) nControllerAddr;
    //A5H+����+����+40H+40H
    char cCmd[ ] = { 0xA5, cAddr, cAddr, 0x40, 0x40 };

    byCmdData.append( cCmd, sizeof (  cCmd ) / sizeof ( cCmd[ 0 ] ) );
}
