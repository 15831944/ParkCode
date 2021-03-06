#include "qserialframe.h"
#include "ui_qserialframe.h"

#include "SerialPort/processdata.h"

QSerialFrame::QSerialFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::QSerialFrame)
{
    ui->setupUi(this);
    //setWindowFlags( Qt::FramelessWindowHint );
    HWND hFrame = winId( );
    LONG nRet = ::GetWindowLong( hFrame, GWL_EXSTYLE );
    nRet = nRet | WS_EX_TOPMOST;
    nRet = ::SetWindowLong( hFrame, GWL_EXSTYLE, nRet );
    nRet = 0;

    bTimerStart = true;
    pAnalogThread = QAnalogThread::CreateInstance( );
    connect( pAnalogThread, SIGNAL( Command( QByteArray ) ),
             this, SLOT( HandleCommand( QByteArray ) ) );
}

QSerialFrame::~QSerialFrame()
{
    delete ui;
}

void QSerialFrame::GetCardId( QByteArray &byCardId, int nCardId )
{
    QString strHex;
    strHex.sprintf( "%06X", nCardId );

    for ( int nIndex = 0; nIndex < 3; nIndex++ ) {
        char cCardId = strHex.mid( nIndex * 2, 2 ).toShort( 0, 16 );
        byCardId.append( cCardId );
    }
}

void QSerialFrame::GetCmd( ECmd eType, QByteArray &byCmd, char cCan, int nCardId )
{
    byCmd.clear( );

    switch ( eType ) {
    case CmdCarLeave :
    case CmdCarEnter :
    {
        char aCmdEnter[ ] = { 0xAA, 0x04, 0x00, 0x54, 0x42, cCan, ( char ) ( eType - CmdCarLeave ), 0x00, 0x55 };
        byCmd.append( aCmdEnter, sizeof ( aCmdEnter ) );
    }
        break;

    case CmdCarGateLeave :
    case CmdCarGateEnter :
    {
        char aCmdGateSense[ ] = { 0xAA, 0x04, 0x00, 0x54, 0x46, cCan, ( char ) ( eType - CmdCarGateLeave ), 0x00, 0x55 };
        byCmd.append( aCmdGateSense, sizeof ( aCmdGateSense ) );
    }
        break;

    case CmdCarCard :
    {
        char aCmdCard[ ] = { 0xAA, 0x06, 0x00, 0x4B, 0x00, cCan, 0x00, 0x00, 0x00, 0x00, 0x55 };
        QByteArray byCardId;
        GetCardId( byCardId, nCardId );
        byCmd.append( aCmdCard, sizeof ( aCmdCard ) );
        byCmd.replace( 6, 3, byCardId );
    }
        break;
    }

    qDebug( ) << byCmd.toHex( ) << endl;
}

void QSerialFrame::closeEvent(QCloseEvent *e)
{
    e->ignore( );
}

void QSerialFrame::timerEvent( QTimerEvent *e )
{
}

void QSerialFrame::HandleCommand( QByteArray byCmd )
{
    CProcessData::GetProcessor( )->ParseData( byCmd );
}

void QSerialFrame::on_btnTimer_clicked()
{
    QByteArray byCmd;
    QAnalogThread* pThread = pAnalogThread;
    pThread->SetIntervalTime( ui->spTimer->value( ) );
    pThread->SetStopSend( true );

    if ( bTimerStart ) {
        pThread->ClearCmd( );

        if ( ui->chkEnterSense0->isChecked( ) ) {
            GetCmd( CmdCarEnter, byCmd, ui->spEnterCan->value( ) );
            pThread->AddCmd( byCmd );
        }

        if ( ui->chkEnterCard->isChecked( ) ) {
            GetCmd( CmdCarCard, byCmd, ui->spEnterCan->value( ), ui->spEnterCard->value( ) );
            pThread->AddCmd( byCmd );
        }

        if ( ui->chkEnterSense1->isChecked( ) ) {
            GetCmd( CmdCarLeave, byCmd, ui->spEnterCan->value( ) );
            pThread->AddCmd( byCmd );
        }

        if ( ui->chkEnterSense2->isChecked( ) ) {
            GetCmd( CmdCarGateEnter, byCmd, ui->spEnterCan->value( ) );
            pThread->AddCmd( byCmd );
        }

        if ( ui->chkEnterSense3->isChecked( ) ) {
            GetCmd( CmdCarGateLeave, byCmd, ui->spEnterCan->value( ) );
            pThread->AddCmd( byCmd );
        }

        if ( ui->chkLeaveSense0->isChecked( ) ) {
            GetCmd( CmdCarEnter, byCmd, ui->spLeaveCan->value( ) );
            pThread->AddCmd( byCmd );
        }

        if ( ui->chkLeaveCard->isChecked( ) ) {
            GetCmd( CmdCarCard, byCmd, ui->spLeaveCan->value( ), ui->spLeaveCard->value( ) );
            pThread->AddCmd( byCmd );
        }

        if ( ui->chkLeaveSense1->isChecked( ) ) {
            GetCmd( CmdCarLeave, byCmd, ui->spLeaveCan->value( ) );
            pThread->AddCmd( byCmd );
        }

        if ( ui->chkLeaveSense2->isChecked( ) ) {
            GetCmd( CmdCarGateEnter, byCmd, ui->spLeaveCan->value( ) );
            pThread->AddCmd( byCmd );
        }

        if ( ui->chkLeaveSense3->isChecked( ) ) {
            GetCmd( CmdCarGateLeave, byCmd, ui->spLeaveCan->value( ) );
            pThread->AddCmd( byCmd );
        }

        pThread->SetStopSend( false );

        pThread->PostEvent( );
    }

    bTimerStart = !bTimerStart;
    QPushButton* pBtn = ( QPushButton* ) sender( );
    pBtn->setText( bTimerStart ? "����" : "ͣ��" );
}

void QSerialFrame::SendCmd( QObject *pObj )
{
    QPushButton* pBtn = ( QPushButton* ) pObj;
    QByteArray byCmd;

    switch ( pBtn->statusTip( ).toShort( ) ) {
    case 0 :
        GetCmd( CmdCarEnter, byCmd, ui->spEnterCan->value( ) );
        break;

    case 1 :
        GetCmd( CmdCarCard, byCmd, ui->spEnterCan->value( ), ui->spEnterCard->value( ) );
        break;

    case 2 :
        GetCmd( CmdCarLeave, byCmd, ui->spEnterCan->value( ) );
        break;

    case 3 :
        GetCmd( CmdCarEnter, byCmd, ui->spLeaveCan->value( ) );
        break;

    case 4 :
        GetCmd( CmdCarCard, byCmd, ui->spLeaveCan->value( ), ui->spLeaveCard->value( ) );
        break;

    case 5 :
        GetCmd( CmdCarLeave, byCmd, ui->spLeaveCan->value( ) );
        break;

    case 6 :
        GetCmd( CmdCarGateEnter, byCmd, ui->spEnterCan->value( ) );
        break;

    case 7 :
        GetCmd( CmdCarGateLeave, byCmd, ui->spEnterCan->value( ) );
        break;

    case 8 :
        GetCmd( CmdCarGateEnter, byCmd, ui->spLeaveCan->value( ) );
        break;

    case 9 :
        GetCmd( CmdCarGateLeave, byCmd, ui->spLeaveCan->value( ) );
        break;
    }

    CProcessData::GetProcessor( )->ParseData( byCmd );
}

void QSerialFrame::on_btn0_clicked()
{
    SendCmd( sender( ) );
}

void QSerialFrame::on_btn1_clicked()
{
    SendCmd( sender( ) );
}

void QSerialFrame::on_btn2_clicked()
{
    SendCmd( sender( ) );
}

void QSerialFrame::on_btn3_clicked()
{
    SendCmd( sender( ) );
}

void QSerialFrame::on_btn4_clicked()
{
    SendCmd( sender( ) );
}

void QSerialFrame::on_btn5_clicked()
{
    SendCmd( sender( ) );
}

void QSerialFrame::on_btn6_clicked()
{
    SendCmd( sender( ) );
}

void QSerialFrame::on_btn7_clicked()
{
    SendCmd( sender( ) );
}

void QSerialFrame::on_btn8_clicked()
{
    SendCmd( sender( ) );
}

void QSerialFrame::on_btn9_clicked()
{
    SendCmd( sender( ) );
}
