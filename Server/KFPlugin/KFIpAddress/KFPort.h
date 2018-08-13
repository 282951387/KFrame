#ifndef __KF_NET_PORT_H__
#define __KF_NET_PORT_H__

#include "KFrame.h"

namespace KFrame
{
    class KFPort
    {
    public:
        KFPort()
        {
            _id = 0;
            _port = 0;
        }

    public:
        uint32 _id;
        uint32 _port;
    };

#define __MAX_PORT_COUNT__ 200
    class KFramePort
    {
    public:
        KFramePort()
        {
            _is_inited = 0;
        }

        // ���Ҷ˿�
        KFPort* FindPort( uint32 id );

    public:
        // �Ƿ��ʼ��
        uint32 _is_inited;

        // ������
        KFMutex _kf_mutex;

        // �˿��б�
        KFPort _kf_port[ __MAX_PORT_COUNT__ ];
    };

#define __FIX_PORT__ 0	// �̶�����˿�
#define __ID_PORT__ 1	// id����˿�
#define __SH_PORT__ 2	// �����ڴ����˿�
#define __ZONE_PORT__ 3 // ���շ�������˿�

    ///////////////////////////////////////////////////////////////////////
    class KFNetProt
    {
    public:
        KFNetProt();
        ~KFNetProt();

        // ��������
        void InitMemoryType( const std::string& type );

        // ����һ�������˿�
        uint32 CalcListenPort( uint32 type, uint32 baseport, uint32 appid );

    private:
        // �ڴ�����
        std::string _memory_type;

        // �����ڴ�
        KFShareMemory _kf_share_memory;
    };
}
#endif

