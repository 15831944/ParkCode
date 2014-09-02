#ifndef ENUMTYPE_H
#define ENUMTYPE_H

// ������������
typedef enum __CmdType {
    CmdNone,
    CmdWriteCardID,
    CmdReadCardID,
    //Ʊ��ظ�
    CmdInBallotSenseEnter, //��������
    CmdInBallotSenseLeave, //�����뿪
    CmdOutBallotSenseEnter, //��������
    CmdOutBallotSenseLeave, //�����뿪
    //��բ�ظ�
    CmdInGateSenseEnter, //��������
    CmdInGateSenseLeave, //�����뿪
    CmdOutGateSenseEnter, //��������
    CmdOutGateSenseLeave, //�����뿪
    //����բ
    CmdInOpenGate, //��ڿ�բ
    CmdOutOpenGate,
    CmdInCloseGate,
    CmdOutCloseGate
} CmdType;

typedef enum __CtrlType {
    CtrlXw,
    CtrlVz
} CtrlType;

#endif // ENUMTYPE_H
