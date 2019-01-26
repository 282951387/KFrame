#ifndef __KF_BATTLE_CLIENT_INTERFACE_H__
#define __KF_BATTLE_CLIENT_INTERFACE_H__

#include "KFrame.h"

namespace KFrame
{
    class KFBattleClientInterface : public KFModule
    {
    public:

        // ������Ϣ��Battle
        virtual bool SendMessageToBattle( uint32 msgid, ::google::protobuf::Message* message ) = 0;
        virtual bool SendMessageToBattle( uint64 roomid, uint32 msgid, ::google::protobuf::Message* message ) = 0;

        // ��ѯս��
        virtual void QueryBattleRoom( uint64 playerid, uint64 roomid ) = 0;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_INTERFACE__( _kf_battle, KFBattleClientInterface );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
}



#endif