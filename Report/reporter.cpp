#include "reporter.h"
#include "Common/commonfunction.h"
#include "Common/logicinterface.h"
#include <QDebug>
#include <QFile>
#include <windows.h>
#include <psapi.h>
#include <QFile>

CReporter::CReporter(QObject *parent) :
    QObject(parent)
{
    QSettings* pSysSet = CCommonFunction::GetSettings( CommonDataType::CfgSystem );
    strPronvice = pSysSet->value( "CommonCfg/PronviceID", '��' ).toString( );
    bPersonTime = false;
    pReportThread = QReportThread::CreateReportThread( );
    connect( pReportThread, SIGNAL( ReportData( int, QStringList ) ),
             this, SLOT( HandleReportData( int, QStringList ) ) );
}

void CReporter::SetWebView( QWebView *pView )
{
    pReportView = pView;
}

void CReporter::HandleReportData( int nType, QStringList lstData )
{
    CommonDataType::ReportType rType = ( CommonDataType::ReportType ) ( nType - QEvent::User );
    QString strStart;
    QString strEnd;
    CCommonFunction::DateTime2String( dtStartTime, strStart );
    CCommonFunction::DateTime2String( dtEndTime, strEnd );
    QDateTime dtTimeEnd = dtEndTime;

    QDate dStart = dtStartTime.date( );
    QDate dEnd = dtEndTime.date( );


    CCommonFunction::Date2String( dStart, strStart );
    CCommonFunction::Date2String( dEnd, strEnd );

    QString strTitle;
    QString strFooter;
    QString strTableBody;
    GetHtml( rType, strTitle, strFooter, strTableBody, lstData );

    QString strTitleDate;
    GetTitle( rType, dtStartTime, dtTimeEnd, strTitleDate );


    QString strHtml = QString( "<HTML>\
                                <BODY>\
                                    <H3 ALIGN = \"CENTER\">�������ڣ�%1</H1>\
                                    <br><br>\
                                    <TABLE ALIGN = \"CENTER\" BORDER = \"1\" frame=\"box\" rules=\"all\"CELLPADDING = \"2\">\
                                        %2\
                                        %3\
                                    </TABLE>\
                                </BODY>\
                                </HTML>" ).arg( strTitleDate, strTitle, strTableBody );

    pReportView->setHtml( strHtml );
}

void CReporter::Print( CommonDataType::ReportType rType )
{
    if ( !GetAdboeExePath( strAdobeExe ) ) {
        return;
    }

    KillAdobeProcess( strAdobeExe );

    printer.setPageSize( QPrinter::A4 );
    printer.setOutputFormat( QPrinter::PdfFormat );

    QString strFile;
    CCommonFunction::GetPath( strFile, CommonDataType::PathSnapshot );
    strFile += "reporter.pdf";
    if ( QFile::exists( strFile ) ) {
        QFile::remove( strFile );
        Sleep( 1000 );
    }

    printer.setOutputFileName( strFile );
    pReportView->print( &printer );

    //Call Adobe Reader print pdf
    PrintPdf( strFile );
}

void CReporter::BuildHtmlDoc( QDateTime& dtStart, QDateTime& dtEnd, CommonDataType::ReportType rType )
{
    dtStartTime = dtStart;
    dtEndTime = dtEnd;

    QString strXml;
    QString strDateFormat = "yyyy-MM-dd";
    QString strDateTimeFormat = "yyyy-MM-dd HH:mm:ss";

    switch ( rType ) {
    case CommonDataType::ReportYearly :
        strXml = QString( "<Data><StartDate>%1</StartDate><EndDate>%2</EndDate></Data>" ).arg(
                    dtStart.date( ).toString( strDateFormat ), dtEnd.date().toString( strDateFormat ) );
        break;

    case CommonDataType::ReportMonthly :
        strXml = QString( "<Data><StartDate>%1</StartDate><EndDate>%2</EndDate></Data>" ).arg(
                    dtStart.date( ).toString( strDateFormat ), dtEnd.date().toString( strDateFormat ) );
        break;

    case CommonDataType::ReportDaily :
        strXml = QString( "<Data><StartDate>%1</StartDate><EndDate>%2</EndDate></Data>" ).arg(
                    dtStart.date( ).toString( strDateFormat ), dtEnd.date().toString( strDateFormat ) );
        break;

    case CommonDataType::ReportPerson :
        strXml = QString( "<Data><StartDate>%1</StartDate><EndDate>%2</EndDate></Data>" ).arg(
                    dtStart.toString( strDateTimeFormat  ), dtEnd.toString( strDateTimeFormat ) );
        break;

    case CommonDataType::ReportTimeCardDetail :
        strXml = QString( "<Data><StartDate>%1</StartDate><EndDate>%2</EndDate></Data>" ).arg(
                    dtStart.toString( strDateTimeFormat ), dtEnd.toString( strDateTimeFormat ) );
        break;

    case CommonDataType::ReportChannel :
        strXml = QString( "<Data><StartDate>%1</StartDate><EndDate>%2</EndDate></Data>" ).arg(
                    dtStart.toString( strDateTimeFormat ), dtEnd.toString( strDateTimeFormat ) );
        break;

    case CommonDataType::ReportProvince :
        strXml = QString( "<Data><StartDate>%1</StartDate><EndDate>%2</EndDate></Data>" ).arg(
                    dtStart.toString( strDateTimeFormat ), dtEnd.toString( strDateTimeFormat ) );
        break;

    case CommonDataType::ReportInProvince :
        strXml = QString( "<Data><StartDate>%1</StartDate><EndDate>%2</EndDate><Province>%3</Province></Data>" ).arg(
                    dtStart.toString( strDateTimeFormat ), dtEnd.toString( strDateTimeFormat ),
                    strPronvice );
        break;

    case CommonDataType::ReportMonth :
        strXml = QString( "<Data><DateFlag>%1</DateFlag><StartDate>%2</StartDate><EndDate>%3</EndDate></Data>" ).arg(
                    lstWheres.at( 0 ), lstWheres.at( 1 ), lstWheres.at( 2 ) );
        break;

    case CommonDataType::ReportMonthInOut :
        strXml = QString( "<Data><StartDate>%1</StartDate><EndDate>%2</EndDate><QueryFlag>%3</QueryFlag><CardNo>%4</CardNo><Plate>%5</Plate></Data>" ).arg(
                    dtStart.toString( strDateTimeFormat ), dtEnd.toString( strDateTimeFormat ), lstWheres.at( 0 ), lstWheres.at( 1 ), lstWheres.at( 2 ) );
        break;
    }

    PostReportEvent( strXml, ( QMyReportEvent::MyReportEvent ) ( rType + QEvent::User ) );
}

void CReporter::PostReportEvent( const QString &strXml, QMyReportEvent::MyReportEvent eEvent )
{
    pReportThread->PostReportEvent( strXml, eEvent );
}

void CReporter::SetWhere( QStringList &lstWhere )
{
    lstWheres = lstWhere;
}

void CReporter::SetPersonTime( bool bPerson )
{
    bPersonTime = bPerson;
}

void CReporter::Print( CommonDataType::ReportType rType, QWebView& wvReport )
{
    if ( !GetAdboeExePath( strAdobeExe ) ) {
        return;
    }

    KillAdobeProcess( strAdobeExe );

    printer.setPageSize( QPrinter::A4 );
    printer.setOutputFormat( QPrinter::PdfFormat );

    QString strFile;
    CCommonFunction::GetPath( strFile, CommonDataType::PathSnapshot );
    strFile += "reporter.pdf";
    if ( QFile::exists( strFile ) ) {
        QFile::remove( strFile );
        Sleep( 1000 );
    }

    printer.setOutputFileName( strFile );
    wvReport.print( &printer );

    //Call Adobe Reader print pdf
    PrintPdf( strFile );
}

void CReporter::KillAdobeProcess( QString &strExe )
{
    DWORD dwProcID[ 1024 ] = { 0 };
    DWORD dwRealByte = 0;

    if ( !::EnumProcesses( dwProcID, sizeof ( dwProcID ), &dwRealByte ) ) {
        return;
    }

    DWORD dwProcCount = dwRealByte / sizeof ( DWORD );
    HANDLE hProc = NULL;
    DWORD dwProc = 0;
    WCHAR wImgPath[ 512 ] = { 0 };
    QString strTargetPath;

    for ( DWORD dwIndex = 0; dwIndex < dwProcCount; dwIndex++ ) {
        dwProc = dwProcID[ dwIndex ];
        if ( 0 == dwProc ) {
            continue;
        }

        hProc = ::OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProc );
        if ( NULL == hProc ) {
            continue;
        }

        //if ( !::GetProcessImageFileName( hProc, wImgPath, sizeof ( wImgPath ) / sizeof ( WCHAR ) ) ) {
        if ( !::GetModuleFileNameEx( hProc, NULL, wImgPath, sizeof ( wImgPath ) / sizeof ( WCHAR ) ) ) {
            ::CloseHandle( hProc );
            continue;
        }

        strTargetPath = QString::fromWCharArray( wImgPath );
        if ( strTargetPath == strExe ) {
            ::TerminateProcess( hProc, 0 );
            ::WaitForSingleObject( hProc, 5000 ); // INFINITE
            ::CloseHandle( hProc );
            break;
        }

        ::CloseHandle( hProc );
    }
}

bool CReporter::GetAdboeExePath( QString &strExe )
{
    QString strReg = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Adobe\\Acrobat Reader";//\\10.0\\InstallPath";
    QSettings settings( strReg, QSettings::NativeFormat );
    QStringList lstKeys = settings.allKeys( );
    QString strTarget = "InstallPath";
    QString strKey = "";

    for ( int nIndex = 0; nIndex < lstKeys.count( ); nIndex++ ) {
        strKey = lstKeys[ nIndex ];
        if ( strKey.contains( strTarget ) ) { // 10.0/InstallPath/.
            strKey = strKey.left( strKey.count( ) - 1 );
            break;
        }
    }

    strExe = settings.value( strKey + "Default", "" ).toString( ) + "\\AcroRd32.exe";
    strExe.replace( QRegExp( "\\" ), "/" );

    bool bRet = false;
    if ( !QFile::exists( strExe ) ) {
        CCommonFunction::MsgBox( NULL, CCommonFunction::GetMsgTitle( QMessageBox::Information ), "�밲װ��Adobe Acrobat Reader��.", QMessageBox::Information );
        return bRet;
    }

    return true;
}

void CReporter::PrintPdf( QString &strFile )
{
    QStringList lstArgs;
    lstArgs << strFile;
    QProcess::startDetached( strAdobeExe, lstArgs );
}

void CReporter::GetSQL( QString &strSql, CommonDataType::ReportType rType, QDateTime& dtStart, QDateTime& dtEnd )
{
    QString strMonthCard = "���⿨";
    QString strTimeCard = "��ʱ��";
    QString strValueCard = "��ֵ��";
    QString strValue = "��ֵ����ֵ";

    QDate dStart = dtStart.date( );
    QDate dEnd = dtEnd.date( );
    QString strStart;
    QString strEnd;

    QTime time;
    time.setHMS( 23, 59, 59 );
    QDateTime dtTimeEnd = dtEnd;
    dtEnd.setTime( time );

    CCommonFunction::DateTime2String( dtStart, strStart );
    CCommonFunction::DateTime2String( dtEnd, strEnd );

    QString yBegin = QString::number( dStart.year( ) ) + "-01-01 00:00:00";
    QString yEnd = QString::number( dEnd.year( ) + 1 ) + "-01-01 00:00:00";

    QString mBegin = QString::number( dStart.year( ) ) + "-" + QString::number( dStart.month( ) ) + "-01 00:00:00";
    QString mEnd = QString::number( dEnd.year( ) ) + "-" + QString::number( dEnd.month( ) + 1 ) + "-01 00:00:00";

    switch ( rType ) {
    case CommonDataType::ReportYearly :
        strSql = "select f1,f2,sum( f3 ),sum(f4), sum(f5), sum(f6), sum(f7), sum(f8) from ( select year( intime ) f1, month( intime ) f2, count( cardno ) as f3, null as f4, null as f5, null as f6, null as f7, null as f8 from stoprd a where  ( '";
        strSql += yBegin;
        strSql += "' <= a.intime and a.intime < '";
        strSql += yEnd;
        strSql += "' ) and a.cardkind = '";
        strSql += strMonthCard;
        strSql += "' group by year( a.intime ), month( a.intime ) union all select year( feetime ) f1, month( feetime ) f2, null as f3, sum( feenumb ) f4, null as f5, null as f6, null as f7, null as f8  from feerd where ('";
        strSql += yBegin;
        strSql += "' <= feetime and feetime < '";
        strSql += yEnd;
        strSql += "') and feekind like '";
        strSql += strMonthCard;
        strSql += "%' group by year( feetime ), month( feetime ) union all select year( intime ) f1, month( intime ) f2, count( cardno ) f3, null as f4, null as f5, null as f6, null as f7, null as f8 from stoprd a where  ( '";
        strSql += yBegin;
        strSql += "' <= a.intime and a.intime < '";
        strSql += yEnd,
        strSql += "' ) and a.cardkind = '";
        strSql += strTimeCard;
        strSql += "' group by year( a.intime ), month( a.intime ) union all select year( feetime ) f1, month( feetime ) f2, null as f3, sum( feenumb ) f4, null as f5, null as f6, null as f7, null as f8 from feerd where ('";
        strSql += yBegin;
        strSql += "' <= feetime and feetime < '";
        strSql += yEnd;
        strSql += "') and feekind like '";
        strSql += strTimeCard;
        strSql += "%' group by year( feetime ), month( feetime ) union all select year( intime ) f1, month( intime ) f2, count( cardno ) f3, null as f4, null as f5, null as f6, null as f7, null as f8 from stoprd a where  ( '";
        strSql += yBegin;
        strSql += "' <= a.intime and a.intime < '";
        strSql += yEnd,
        strSql += "' ) and a.cardkind = '";
        strSql += strValueCard;
        strSql += "' group by year( a.intime ), month( a.intime ) union all select year( feetime ) f1, month( feetime ) f2, null as f3, sum( feenumb ) f4, null as f5, null as f6, null as f7, null as f8 from feerd where ('";
        strSql += yBegin;
        strSql += "' <= feetime and feetime < '";
        strSql += yEnd;
        strSql += "') and feekind like '";
        strSql += strValue;
        strSql += "%' group by year( feetime ), month( feetime ), day( feetime ) ) x group by f1, f2";
        break;

    case CommonDataType::ReportMonthly :
        strSql = "select f1,f2,f3,sum(f4), sum(f5), sum(f6), sum(f7), sum(f8), sum(f9) from ( select year( intime ) f1, month( intime ) f2, day( intime ) f3, count( cardno ) f4, null as f5, null as f6, null as f7, null as f8, null as f9 ";
        strSql += "from stoprd a ";
        strSql += "where  ( '";
        strSql += mBegin;
        strSql += "' <= a.intime and a.intime < '";
        strSql += mEnd;
        strSql += "' ) and a.cardkind = '";
        strSql += strMonthCard;
        strSql += "' group by year( a.intime ), month( a.intime ), day( intime ) ";
        strSql += "union all ";///////////
        strSql += "select year( feetime ) f1, month( feetime ) f2, day( feetime ) f3, null as f4, sum( feenumb ) f5, null as f6, null as f7, null as f8, null as f9 ";
        strSql += "from feerd ";
        strSql += "where  ( '";
        strSql += mBegin;
        strSql += "' <= feetime and feetime < '";
        strSql += mEnd;
        strSql += "') and feekind like '";
        strSql += strMonthCard;
        strSql += "%'  group by year( feetime ), month( feetime ), day( feetime ) ";
        strSql += "union all ";//////////
        strSql += "select year( intime ) f1, month( intime ) f2, day( intime ) f3, null as f4, null as f5, count( cardno ) f6, null as f7, null as f8, null as f9 ";
        strSql += "from stoprd a ";
        strSql += "where  ( '";
        strSql += mBegin;
        strSql += "' <= a.intime and a.intime < '";
        strSql += mEnd;
        strSql += "' ) and a.cardkind = '";
        strSql += strTimeCard;
        strSql += "' group by year( a.intime ), month( a.intime ), day( intime ) ";
        strSql += "union all ";
        strSql += "select year( feetime ) f1, month( feetime ) f2, day( feetime ) f3, null as f4, null as f5, null as f6, sum( feenumb ) f7,null as f8, null as f9 ";
        strSql += "from feerd ";
        strSql += "where  ( '";
        strSql += mBegin;
        strSql += "' <= feetime and feetime < '";
        strSql += mEnd;
        strSql += "') and feekind like '";
        strSql += strTimeCard;
        strSql += "%' group by year( feetime ), month( feetime ), day( feetime ) ";
        strSql += "union all ";//////////
        strSql += "select year( intime ) f1, month( intime ) f2, day( intime ) f3, null as f4, null as f5, null as f6, null as f7, count( cardno ) f8, null as f9 ";
        strSql += "from stoprd a ";
        strSql += "where  ( '";
        strSql += mBegin;
        strSql += "' <= a.intime and a.intime < '";
        strSql += mEnd;
        strSql += "' ) and a.cardkind = '";
        strSql += strValueCard;
        strSql += "' group by year( a.intime ), month( a.intime ), day( intime ) ";
        strSql += "union all ";
        strSql += "select year( feetime ) f1, month( feetime ) f2, day( feetime ) f3, null as f4, null as f5, null as f6, null as f7, null as f8, sum( feenumb ) f9 ";
        strSql += "from feerd ";
        strSql += "where  ( '";
        strSql += mBegin;
        strSql += "' <= feetime and feetime < '";
        strSql += mEnd;
        strSql += "') and feekind like '";
        strSql += strValueCard;
        strSql += "%' group by year( feetime ), month( feetime ), day( feetime ) ) x group by f1, f2, f3";
        break;

    case CommonDataType::ReportDaily :
        strSql = "select f1,f2,f3,sum(f4), sum(f5), sum(f6), sum(f7), sum(f8), sum(f9)  from ( select year( intime ) f1, month( intime ) f2, day( intime ) f3, count( cardno ) f4, null as f5, null as f6, null as f7, null as f8, null as f9 ";
        strSql += "from stoprd a ";
        strSql += "where  ( '";
        strSql += strStart;
        strSql += "' <= a.intime and a.intime < '";
        strSql += strEnd;
        strSql += "' ) and a.cardkind = '";
        strSql += strMonthCard;
        strSql += "' group by year( a.intime ), month( a.intime ), day( intime ) ";
        strSql += "union all ";
        strSql += "select year( feetime ) f1, month( feetime ) f2, day( feetime ) f3, null as f4, sum( feenumb ) f5, null as f6, null as f7, null as f8, null as f9 ";
        strSql += "from feerd ";
        strSql += "where  ( '";
        strSql += strStart;
        strSql += "' <= feetime and feetime < '";
        strSql += strEnd;
        strSql += "') and feekind like '";
        strSql += strMonthCard;
        strSql += "%'  group by year( feetime ), month( feetime ), day( feetime ) ";
        strSql += "union all ";
        strSql += "select year( intime ) f1, month( intime ) f2, day( intime ) f3, null as f4, null as f5, count( cardno ) f6, null as f7, null as f8, null as f9 ";
        strSql += "from stoprd a ";
        strSql += "where  ( '";
        strSql += strStart;
        strSql += "' <= a.intime and a.intime < '";
        strSql += strEnd;
        strSql += "' ) and a.cardkind = '";
        strSql += strTimeCard;
        strSql += "' group by year( a.intime ), month( a.intime ), day( intime ) ";
        strSql += "union all ";
        strSql += "select year( feetime ) f1, month( feetime ) f2, day( feetime ) f3, null as f4, null as f5, null as f6, sum( feenumb ) f7,null as f8, null as f9 ";
        strSql += "from feerd ";
        strSql += "where  ( '";
        strSql += strStart;
        strSql += "' <= feetime and feetime < '";
        strSql += strEnd;
        strSql += "') and feekind like '";
        strSql += strTimeCard;
        strSql += "%' group by year( feetime ), month( feetime ), day( feetime ) ";
        strSql += "union all ";
        strSql += "select year( intime ) f1, month( intime ) f2, day( intime ) f3, null as f4, null as f5, null as f6, null as f7, count( cardno ) f8, null as f9 ";
        strSql += "from stoprd a ";
        strSql += "where  ( '";
        strSql += strStart;
        strSql += "' <= a.intime and a.intime < '";
        strSql + strEnd;
        strSql += "' ) and a.cardkind = '";
        strSql += strValueCard;
        strSql += "' group by year( a.intime ), month( a.intime ), day( intime ) ";
        strSql += "union all ";
        strSql += "select year( feetime ) f1, month( feetime ) f2, day( feetime ) f3, null as f4, null as f5, null as f6, null as f7, null as f8, sum( feenumb ) f9 ";
        strSql += "from feerd ";
        strSql += "where  ( '";
        strSql += strStart;
        strSql += "' <= feetime and feetime < '";
        strSql += strEnd;
        strSql += "') and feekind like '";
        strSql += strValueCard;
        strSql += "%' group by year( feetime ), month( feetime ), day( feetime ) ) x group by f1, f2, f3";
        break;

    case CommonDataType::ReportPerson :
        if ( bPersonTime ) {
            CCommonFunction::DateTime2String( dtTimeEnd, strEnd );
        }

        strSql = "select feeoperator f1, sum( feenumb ) f2 from feerd where feetime between '";
        strSql += strStart;
        strSql += "' and '";
        strSql += strEnd;
        strSql += "' group by feeoperator";
        break;

    case CommonDataType::ReportChannel :
#if false
        ( select  date( intime ) a, inshebeiname b , cardkind c, count( cardno ) d from stoprd
        where intime between '2011-01-01 00:00:00' and '2012-01-01 00:00:00'
        group by date( intime ), inshebeiname, cardkind
        order by date( intime ), inshebeiname, cardkind ) t1
        -- inner join / cross full inner left right // union //union all
         union
        select * from ( select  date( outtime ) a, outshebeiname b, cardkind c, count( cardno ) d from stoprd
        where outtime between '2011-01-01 00:00:00' and '2012-01-01 00:00:00'
        group by date( outtime ), outshebeiname, cardkind
        order by date( outtime ), outshebeiname, cardkind ) t2
        order by a,b,c;
#endif
        strSql = "select * from ";
        strSql += "( select  date( intime ) a, inshebeiname b , cardkind c, count( cardno ) d from stoprd ";
        strSql += "where intime between '";
        strSql += strStart;
        strSql += "' and '";
        strSql += strEnd;
        strSql += "' group by date( intime ), inshebeiname, cardkind ";
        strSql += "order by date( intime ), inshebeiname, cardkind ) t1 ";
        strSql += "union ";
        strSql += "select * from ( select  date( outtime ) a, outshebeiname b, cardkind c, count( cardno ) d from stoprd ";
        strSql += "where outtime between '";
        strSql += strStart;
        strSql += "' and '";
        strSql += strEnd;
        strSql += "' group by date( outtime ), outshebeiname, cardkind ";
        strSql += "order by date( outtime ), outshebeiname, cardkind ) t2 ";
        strSql += "order by a,b,c";
        break;

    case CommonDataType::ReportTimeCardDetail :
        CCommonFunction::DateTime2String( dtTimeEnd, strEnd );
        strSql = "select date( feetime ) ftime, feeoperator, feekind, feezkyy, sum( feenum ), ";
        strSql += "sum( feefactnum ), sum( feenum - feefactnum ) from stoprd ";
        strSql += "where feenum >= feefactnum and cardkind = '";
        strSql += strTimeCard;
        strSql += "' and ( feetime between '";
        strSql += strStart;
        strSql += "' and '";
        strSql += strEnd;
        strSql += "' ) group by date( feetime ), feeoperator, feekind, feezkyy having ( sum( feenum ) + sum( feefactnum ) ) <> 0 ";
        strSql += "union all ";
        strSql += "select '�ϼ�' ftime, '', '', '', sum( feenum ), sum( feefactnum ), sum( feenum - feefactnum ) from stoprd ";
        strSql += "where feenum >= feefactnum and feetime between '";
        strSql += strStart;
        strSql += "' and '";
        strSql += strEnd;
        strSql += "' order by ftime desc";
        break;

    case CommonDataType::ReportProvince :
        strSql = "select case substring( carcp, 1, 1 ) \
                                   when '��' then '�Ĵ�ʡ'  \
                                   when '��' then '������' \
                                   when '��' then '������' \
                                   when '��' then '�����' \
                                   when '��' then '�Ϻ���' \
                                   when '��' then '�ӱ�ʡ' \
                                   when 'ԥ' then '����ʡ' \
                                   when '��' then '����ʡ' \
                                   when '��' then '����ʡ' \
                                   when '��' then '������ʡ' \
                                   when '��' then '����ʡ' \
                                   when '��' then '����ʡ' \
                                   when '³' then 'ɽ��ʡ' \
                                   when '��' then '�½�������' \
                                   when '��' then '����ʡ' \
                                   when '��' then '�㽭ʡ' \
                                   when '��' then '����ʡ' \
                                   when '��' then '����ʡ' \
                                   when '��' then '����������' \
                                   when '��' then '����ʡ' \
                                   when '��' then 'ɽ��ʡ' \
                                   when '��' then '���ɹ�������' \
                                   when '��' then '����ʡ' \
                                   when '��' then '����ʡ' \
                                   when '��' then '����ʡ' \
                                   when '��' then '����ʡ' \
                                   when '��' then '�㶫ʡ' \
                                   when '��' then '�ຣʡ' \
                                   when '��' then '����������' \
                                   when '��' then '����������' \
                                   when '��' then '����ʡ' \
                                   else 'δ֪' end  as ��������Ͻ��, \
                                   IFNULL( feekind, 'С�ͳ�' ) as ����, \
                                   count( carcp ) as ������ \
                                   from stoprd \
                                   where intime between '"
                                   + strStart +
                                   "' and '"
                                   + strEnd +
                                   //"' and cardkind = '��ʱ��' and feekind is not null group by substring( carcp, 1, 1 ) with rollup";
                                   "' group by substring( carcp, 1, 1 ) with rollup";
        break;

    case CommonDataType::ReportInProvince :
        strSql =  "select case UPPER( substring( carcp, 2, 1 ) )  \
                                   when 'A' then '�ɶ���' \
                                   when 'B' then '������' \
                                   when 'C' then '�Թ���' \
                                   when 'D' then '��֦����' \
                                   when 'E' then '������' \
                                   when 'F' then '������' \
                                   when 'H' then '��Ԫ��' \
                                   when 'J' then '������' \
                                   when 'K' then '�ڽ���' \
                                   when 'L' then '��ɽ��' \
                                   when 'M' then '������' \
                                   when 'Q' then '�˱���' \
                                   when 'R' then '�ϳ���' \
                                   when 'S' then '������' \
                                   when 'T' then '�Ű���' \
                                   when 'U' then '������' \
                                   when 'V' then '������' \
                                   when 'W' then '��ɽ��' \
                                   when 'X' then '�㰲��' \
                                   when 'Y' then '������' \
                                   when 'Z' then 'üɽ��' \
                                   else 'δ֪' end  as ��������Ͻ��,  \
                                   IFNULL( feekind, 'С�ͳ�' ) as ����, \
                                   count( carcp ) as ������ \
                                   from stoprd \
                                   where intime between '"
                                   + strStart +
                                   "' and '"
                                   + strEnd +
                                   //"' and cardkind = '��ʱ��' and feekind is not null and substring( carcp, 1, 1 ) = '��'
                                   "' and substring( carcp, 1, 1 ) = '��' group by substring( carcp, 2, 1 ) with rollup";
        break;

    case CommonDataType::ReportMonth :
        strSql = "select a.cardno, a.cardselfno , \
            case a.EnterMustCard when 0 then '��' when 1 then '��' end as EnterMustCard,  \
            case a.LeaveMustCard when 0 then '��' when 1 then '��' end as LeaveMustCard, \
            case a.MIMO when 0 then '��' when 1 then '��' end as MIMO, \
            case a.Inside when 0 then '��' when 1 then '��' end as Inside, \
            a.cardstate, a.starttime, a.endtime, b.carcp, c.username, c.userphone, \
            IF ( a.starttime > a.endtime, '��', '��' ) as Expire \
            from monthcard as a, carinfo as b, userinfo as c \
                where a.cardno = b.cardindex and a.cardno = c.cardindex ";

        if ( 3 <= lstWheres.count( ) ){
            int nIndex = lstWheres.at( 0 ).toInt( );
            QString strDateRange = "";
            switch ( nIndex ) {
            case 0 :
                strDateRange = QString( " And StartTime >='%1' And EndTime <= '%2' " ).arg( lstWheres.at( 1 ), lstWheres.at( 2 ) );
                break;

            case 1 :
                strDateRange = QString( " And StartTime between '%1' And '%2' " ).arg( lstWheres.at( 1 ), lstWheres.at( 2 ) );
                break;

            case 2 :
                strDateRange = QString( " And EndTime between '%1' And '%2' " ).arg( lstWheres.at( 1 ), lstWheres.at( 2 ) );
                break;
            }

            strSql += strDateRange;
        }

        strSql += " order by Expire";
    break;

   case CommonDataType::ReportMonthInOut :
        if (  2 != lstWheres.length( ) ) {
            CCommonFunction::MsgBox( NULL, "��ʾ", "��ѡ���ѯ������", QMessageBox::Information );
            return;
        }

        const QString& strWhere1 = lstWheres.at( 0 );
        const QString& strWhere2 = lstWheres.at( 1 );
         strSql = QString( "select m.cardno, m.cardselfno, m.carcp, m.username, m.userphone, s.counter from  \
                 ( select a.cardno, a.cardselfno, b.carcp, c.username, c.userphone from  \
                 monthcard as a, carinfo as b, userinfo as c \
                 where a.cardno = b.cardindex and a.cardno = c.cardindex  \
                 %1) m, ( select cardno, count( cardno ) as counter from stoprd where \
                 %2 intime >= '%3' and outtime <= '%4' group by cardno  \
                 order by cardno ) s  where m.cardno = s.cardno" ).arg( strWhere1, strWhere2, strStart, strEnd );
        break;
    }

//#ifdef QT_NO_DEBUG
    QFile file( "d:/sql.txt" );
    if ( !file.exists( ) || !file.open( QIODevice::WriteOnly ) ) {
        return;
    }

    // QDataStream
    QTextStream stream( &file );
    stream << strSql;
    stream.flush( );
    file.close( );
//#endif

}

void CReporter::BuildHtmlDoc( QDateTime& dtStart, QDateTime& dtEnd, CommonDataType::ReportType rType, QWebView& wvReport )
{
    QStringList lstData;
    QString strStart;
    QString strEnd;
    //QString strType = QString::number( rType );
    CCommonFunction::DateTime2String( dtStart, strStart );
    CCommonFunction::DateTime2String( dtEnd, strEnd );
    QString strSql;// = QString( "call GenerateReport( '%1', '%2', %3 )" ).arg( strStart, strEnd, strType );
    QDateTime dtTimeEnd = dtEnd;
    GetSQL( strSql, rType, dtStart, dtEnd );

    if ( strSql.isEmpty( ) ) {
        return;
    }

    CLogicInterface::GetInterface( )->ExecuteSql( strSql, lstData );

    //strSql= "Select @f1,@f2,@f3,@f4,@f5,@f6,@f7,@f8";
    //CLogicInterface::GetInterface( )->ExecuteSql( strSql, lstData );

    QDate dStart = dtStart.date( );
    QDate dEnd = dtEnd.date( );

    CCommonFunction::Date2String( dStart, strStart );
    CCommonFunction::Date2String( dEnd, strEnd );

    QString strTitle;
    QString strFooter;
    QString strTableBody;
    GetHtml( rType, strTitle, strFooter, strTableBody, lstData );

    QString strTitleDate;
    GetTitle( rType, dtStart, dtTimeEnd, strTitleDate );


    QString strHtml = QString( "<HTML>\
                                <BODY>\
                                    <H3 ALIGN = \"CENTER\">�������ڣ�%1</H1>\
                                    <br><br>\
                                    <TABLE ALIGN = \"CENTER\" BORDER = \"1\" frame=\"box\" rules=\"all\"CELLPADDING = \"2\">\
                                        %2\
                                        %3\
                                    </TABLE>\
                                </BODY>\
                                </HTML>" ).arg( strTitleDate, strTitle, strTableBody + strFooter );

    wvReport.setHtml( strHtml );
}

void CReporter::GetTitle( CommonDataType::ReportType rType, QDateTime &dtStart, QDateTime &dtEnd, QString& strTitle )
{
    QDate dStart = dtStart.date( );
    QDate dEnd = dtEnd.date( );

    QString strSYear = QString::number( dStart.year( ) );
    QString strEYear = QString::number( dEnd.year( ) );

    QString strSMonth = QString::number( dStart.month( ) );
    QString strEMonth = QString::number( dEnd.month( ) );

    QString strStart;
    CCommonFunction::Date2String( dStart, strStart );
    QString strEnd;
    CCommonFunction::Date2String( dEnd, strEnd );

    switch ( rType ) {
    case CommonDataType::ReportPerson :
        if ( bPersonTime ) {
            {
                QString strStart;
                CCommonFunction::DateTime2String( dtStart, strStart );
                QString strEnd;
                CCommonFunction::DateTime2String( dtEnd, strEnd );

                 strTitle = strStart + " �� " + strEnd;
            }
        } else {
            if ( strStart == strEnd ) {
                strTitle = strStart + "��";
            } else {
                strTitle = strStart + "�� �� " + strEnd + "��";
            }
        }
        break;

    case CommonDataType::ReportYearly :
        if ( strSYear == strEYear ) {
            strTitle = strSYear + "��";
        } else {
            strTitle = strSYear + "�� �� " + strEYear + "��";
        }
        break;

    case CommonDataType::ReportMonthly :
        if ( strSYear == strEYear ) {
            if ( strSMonth == strEMonth ) {
                strTitle = strSYear + "��" + strSMonth + "��";
            } else {
                strTitle = strSYear + "��" + strSMonth + "�� �� " + strEMonth + "��";
            }
        } else {
            strTitle = strSYear + "��" + strSMonth + "�� �� " + strEYear + "��" + strEMonth + "��";
        }
        break;

    case CommonDataType::ReportDaily :
    case CommonDataType::ReportChannel :
    case CommonDataType::ReportProvince :
    case CommonDataType::ReportInProvince :
    case CommonDataType::ReportMonth :
    case CommonDataType::ReportMonthInOut :
        if ( strStart == strEnd ) {
            strTitle = strStart + "��";
        } else {
            strTitle = strStart + "�� �� " + strEnd + "��";
        }
        break;
    case CommonDataType::ReportTimeCardDetail :
    {
        QString strStart;
        CCommonFunction::DateTime2String( dtStart, strStart );
        QString strEnd;
        CCommonFunction::DateTime2String( dtEnd, strEnd );

         strTitle = strStart + " �� " + strEnd;
    }
        break;
    }
}

void CReporter::GetRowHtml( QString &strRow, QStringList& lstData )
{
    if ( 0 == lstData.count( ) ) {
        return;
    }

    strRow = "<TR>";

    for ( int nIndex = 0; nIndex < lstData.count( ); nIndex++ ) {
       strRow += QString( "<TD HEIGHT = \"5\">%1</TD>" ).arg( lstData[ nIndex ] );
    }

    strRow += "</TR>";
    qDebug( ) << strRow << endl;
}

void CReporter::GetHtml( CommonDataType::ReportType rType, QString& strTitle, QString& strFooter,
                         QString& strTableBody, QStringList& lstData )
{
   QString strRow;
   int nCols = 0;
   int nSkip = 0;
   bool bPlate = false;

   switch ( rType ) {
   case CommonDataType::ReportYearly :
       strTitle = "<tr><th rowspan=\"2\">��</th>\
                   <th colspan=\"2\">���⿨</th>\
                   <th colspan=\"2\">��ֵ��</th>\
                   <th colspan=\"2\">��ʱ��</th>\
                   <th colspan=\"2\">�޿�����</th>\
                   <th>���ɿ�</th>\
                   <th colspan=\"2\">�ϼ�</th></tr>\
                   <tr>\
                   <td >������</td>\
                   <td >����</td>\
                   <td >������</td>\
                   <td>��ֵ</td>\
                   <td >������</td>\
                   <td >�շ�</td>\
                   <td >������</td>\
                   <td >�շ�</td>\
                   <td >������</td>\
                   <td>������</td>\
                   <td>�շ�</td>\
                   </tr>";
       strFooter = " <tr><td>�ϼ�</td> \
                   <td>%1</td><td>%2</td><td>%3</td> \
                   <td>%4</td><td>%5</td><td>%6</td> \
                   <td>%7</td><td>%8</td><td>%9</td> \
                   <td>%10</td><td>%11</td></tr>";
       nSkip = 1;
       nCols = 12;
       break;

   case CommonDataType::ReportMonthly :
       strTitle = "<tr><th rowspan=\"2\">��</th>\
                   <th rowspan=\"2\">��</th>\
                   <th colspan=\"2\">���⿨</th>\
                   <th colspan=\"2\">��ֵ��</th>\
                   <th colspan=\"2\">��ʱ��</th>\
                   <th colspan=\"2\">�޿�����</th>\
                   <th>���ɿ�</th>\
                   <th colspan=\"2\">�ϼ�</th></tr>\
                   <tr>\
                   <td >������</td>\
                   <td >����</td>\
                   <td >������</td>\
                   <td>��ֵ</td>\
                   <td >������</td>\
                   <td >�շ�</td>\
                   <td>������</td>\
                   <td>�շ�</td>\
                   <td>������</td>\
                   <td>������</td>\
                   <td>�շ�</td>\
                   </tr>";
       strFooter = " <tr><td colspan=\"2\">�ϼ�</td> \
                   <td>%1</td><td>%2</td><td>%3</td> \
                   <td>%4</td><td>%5</td><td>%6</td> \
                   <td>%7</td><td>%8</td><td>%9</td>\
                   <td>%10</td><td>%11</td></tr>";
       nSkip = 2;
       nCols = 13;
       break;

   case CommonDataType::ReportDaily :
       strTitle = "<tr><th rowspan=\"2\">��</th>\
                  <th rowspan=\"2\">��</th>\
                   <th rowspan=\"2\" >��</th>\
                   <th colspan=\"2\">���⿨</th>\
                   <th colspan=\"2\">��ֵ��</th>\
                   <th colspan=\"2\">��ʱ��</th>\
                   <th colspan=\"2\">�޿�����</th>\
                   <th>���ɿ�</th>\
                   <th colspan=\"2\">�ϼ�</th></tr>\
                   <tr>\
                   <td >������</td>\
                   <td >����</td>\
                   <td >������</td>\
                   <td>��ֵ</td>\
                   <td >������</td>\
                   <td >�շ�</td>\
                   <td >������</td>\
                   <td>�շ�</td>\
                   <td>������</td>\
                   <td>������</td>\
                   <td>�շ�</td>\
                   </tr>";
       strFooter = " <tr><td colspan=\"3\">�ϼ�</td> \
                   <td>%1</td><td>%2</td><td>%3</td> \
                   <td>%4</td><td>%5</td><td>%6</td> \
                   <td>%7</td><td>%8</td><td>%9</td>\
                   <td>%10</td><td>%11</td></tr>";
       nSkip = 3;
       nCols = 14;
       break;

   case CommonDataType::ReportPerson :
       strTitle = "<tr><th>�շ�Ա</th><th>���</th><tr>";
       strFooter = "<tr><th>�ϼ�</th><th>%1</th><tr>";
       nCols = 2;
       nSkip = 1;
       break;

   case CommonDataType::ReportChannel :
       strTitle = "<tr><th>����</th><th>ͨ����</th><th>������</th><th>������</th><tr>";
       //strFooter = "<tr><th>%1</th><th>%2</th><th>%3</th><th>%4</th><tr>";
       strFooter = "";
       nCols = 4;
       break;

   case CommonDataType::ReportTimeCardDetail :
       strTitle = "<tr><th>����</th><th>�շ�Ա</th><th>��������</th><th>�Ż�����</th>\
                        <th>Ӧ���ܶ�</th><th>ʵ���ܶ�</th><th>�շѲ��</th><tr>";
       strFooter = "<tr><td colspan=\"4\">�ϼ�</td><th>%1</th><th>%2</th><th>%3</th><tr>";
       nCols = 7;
       nSkip = 4;
       break;

  case CommonDataType::ReportProvince :
  case CommonDataType::ReportInProvince :
       strTitle = "<tr><th>��������Ͻ��</th><th>����</th><th>������</th></tr>";
       strFooter = " <tr><td colspan=\"2\">�ϼ�</td> \
                   <td>%1</td></tr>";
       nCols = 3;
       bPlate = true;
       nSkip = 2;
       break;
  case CommonDataType::ReportMonth :
       strTitle = "<tr><th>����</th><th>�Ա��</th>\
                       <th>��ˢ��</th>\
                       <th>��ˢ��</th> \
                       <th>������</th>\
                       <th>���볡</th>\
                       <th>��״̬</th>\
                       <th>��ʼʱ��</th>\
                       <th>��ֹʱ��</th>\
                       <th>����</th>\
                       <th>ҵ��</th>\
                       <th>��ϵ�绰</th>\
                       <th>����</th></tr>";
       strFooter = "";
       nCols = 13;
       break;

   case CommonDataType::ReportMonthInOut :
        strTitle = "<tr><th>����</th><th>�Ա��</th>\
                        <th>����</th>\
                        <th>ҵ��</th>\
                        <th>��ϵ�绰</th>\
                        <th>��������</th></tr>";
        strFooter = "";
        nCols = 6;
        break;
   }

   int nRows = lstData.count( ) / nCols;
   QStringList lstTmp;

   for ( int nRow = 0; nRow < nRows; nRow++ ) {
       int nField = nRow * nCols;
       lstTmp = lstData.mid( nField, nCols );
       if ( lstTmp.at( 0 ).isEmpty( ) ||
            ( bPlate && nRow == nRows- 1 ) ) {
           GetSumData( strFooter, lstTmp, nSkip );
           strRow = strFooter;
       } else {
           GetRowHtml( strRow, lstTmp );
       }

       strTableBody += strRow;
   }  
}

void CReporter::GetSumData( QString &strFooter,QStringList& lstData, int nSkip )
{
    for ( int nIndex = nSkip; nIndex < lstData.size( ); nIndex++ ) {
        strFooter = strFooter.arg( lstData.at( nIndex ) );
    }
}

void CReporter::RowData( QStringList& lstReslut, QStringList& lstData,
                         int nField, int nStartIndex, int nCols )
{
    for ( int nIndex = nStartIndex; nIndex < nCols; nIndex++ ) {
        lstReslut << lstData[ nField + nIndex ];
    }
}

void CReporter::RowData( int nSum[ ], QStringList& lstReslut, QStringList& lstData,
                         int nField, QString& strTotal1, QString& strTotal2, int nStartIndex, int nCols )
{
    for ( int nIndex = nStartIndex; nIndex < nCols; nIndex++ ) {
        lstReslut << lstData[ nField + nIndex ];
        nSum[ nIndex - nStartIndex ] += lstData[ nField + nIndex ].toInt( );
    }

    lstReslut << strTotal1
              << strTotal2;

    nStartIndex = nCols - nStartIndex;
    nSum[ nStartIndex ] += strTotal1.toInt( );
    nSum[ ++nStartIndex ] += strTotal2.toInt( );
}

