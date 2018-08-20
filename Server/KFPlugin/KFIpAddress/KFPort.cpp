﻿#include "KFPort.h"


namespace KFrame
{

    KFPort* KFramePort::FindPort( uint32 id )
    {
        KFLocker locker( _kf_mutex );

        for ( auto i = 0; i < __MAX_PORT_COUNT__; ++i )
        {
            auto kfport = &_kf_port[ i ];
            if ( kfport->_id == id )
            {
                return kfport;
            }

            if ( kfport->_id == 0 )
            {
                kfport->_id = id;
                kfport->_port = i + 1;
                return kfport;
            }
        }

        return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    KFNetProt::KFNetProt()
    {
    }

    KFNetProt::~KFNetProt()
    {

    }

    uint32 KFNetProt::CalcListenPort( uint32 porttype, uint32 baseport, uint32 appid )
    {
        uint32 port = baseport;
        switch ( porttype )
        {
        case __ID_PORT__:
        {
            KFAppID kfappid( appid );
            auto instanceid = kfappid._union._app_data._instance_id;
            port = baseport + instanceid;
        }
        break;
        case __SH_PORT__:
        {
            // 共享内存结算
            auto address = _kf_share_memory.CreateMemory( __KF_STRING__( port ), 10240 );

            auto kframeport = reinterpret_cast< KFramePort* >( address );
            if ( kframeport->_is_inited == 0 )
            {
                kframeport = new ( address ) KFramePort();
                kframeport->_is_inited = 1;
            }

            // 查找自己是否在列表中
            auto kfport = kframeport->FindPort( appid );
            port = baseport + kfport->_port;
        }
        break;
        case __ZONE_PORT__:
        {
            auto zoneid = ( appid / 1000 ) % 1000;
            port = baseport + zoneid;
        }
        break;
        default:
            break;
        }

        return port;
    }

}