#include <QtGui/QApplication>
#include <QTextCodec>
#include <QtPlugin>
#include "mainwindow.h"
#include "Common/commonfunction.h"

//Q_IMPORT_PLUGIN(qjpeg)
//Q_IMPORT_PLUGIN(qgif)
//Q_IMPORT_PLUGIN(qkrcodecs)

/*
  �洢���̲���һ��������ڷ�ʽ
  SpEntance( in xmlParam, out xmlInfo )
<Params>
<Host>127.0.0.1</Host>
<User></User>
<-- Call SP -->
<SubSP>Test</SubSP> switch call sub SP
<Operation>Insert Update Delete</Operation> switch
<Data>
<p0>...</p0>
</Data>
</Params>
*/

//QPixmap ͼƬ��ʾ�Ż� QBitmap
//QImage IO�Ż�
//QPicture  ��¼�벥��QPainter����

// SMTP(25) �ʼ�����Э��  SMTP Client---->SMTP Server
// �ʼ�����Э��
// POP3 IMAP(143)

/*
  ��ѯ���⿨�շ���ϸ
select d.cardno ����, d.feetime �շ�ʱ��, d.feenumb �շѽ��, d.feeoperator �շ�Ա,
a.starttime ������ʼʱ��, a.endtime �������ʱ��,
b.username �����û�, b.userphone �����û��绰,
c.carcp ���ƺ���, c.carmodel ����
from parkadmin.monthcard a, parkadmin.userinfo b, parkadmin.carinfo c, parkadmin.feerd d
where a.cardno = b.cardindex and a.cardno = c.cardindex and a.cardno = d.cardno
and d.feekind = '���⿨����' and feetime between '2014-08-01 00:00:00' and '2014-09-10 23:59:59';
  */
// Intel AMD ARM PPC(Apple IBM Motorola) Alpha DEC SPARC MIPS
// Unix Linux Windows �ȶ� �ɿ� ��ȫ
// OpenStack CloudStack CloudPlatform
// Install DVD UEFI USB(UntraISO)
// Linux Redhat(RHEL) CentOS Ubuntu


int main(int argc, char *argv[])
{
    CMySqlDatabase::MySQLLibraryInit( 0, NULL );

    QApplication a(argc, argv);

    // MBCS(DBCS) UCS
    // ASCI ANSI ISO14060 GB2313 GBK GB18030 GB13000 BIG5 CJK
    //GBK��GB2312 GB18030����Unicode����UTF8
    //UTF8����Unicode����GBK��GB2312 GB18030
    QTextCodec *pCodec = CCommonFunction::GetTextCodec( );// QTextCodec::codecForName( "GB18030" );    //System//��ȡϵͳ����

    QTextCodec::setCodecForLocale( pCodec );
    QTextCodec::setCodecForCStrings( pCodec );
    QTextCodec::setCodecForTr( pCodec );

    //CCommonFunction::GetSyncPass( ).lock( );

   CCommonFunction::StartupSplash( );
    MainWindow w;
    w.show();
    CCommonFunction::CleanupSplash( &w );

    //CMySqlDatabase::MySQLLibrayEnd( );

    return a.exec( );
}
