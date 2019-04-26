﻿#include "KFTcpDiscoverModule.hpp"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    void KFTcpDiscoverModule::BeforeRun()
    {
        __REGISTER_SERVER_DISCOVER_FUNCTION__( &KFTcpDiscoverModule::OnServerDiscoverClient );
        __REGISTER_SERVER_LOST_FUNCTION__( &KFTcpDiscoverModule::OnServerLostClient );
        __REGISTER_CLIENT_CONNECTION_FUNCTION__( &KFTcpDiscoverModule::OnClientConnectServer );
        //////////////////////////////////////////////////////////////////////////////////////////////////
        __REGISTER_MESSAGE__( KFMsg::S2S_TELL_DISCOVER_SERVER_TO_MASTER, &KFTcpDiscoverModule::HandleTellDiscoverServerToMaster );
        __REGISTER_MESSAGE__( KFMsg::S2S_TELL_LOST_SERVER_TO_MASTER, &KFTcpDiscoverModule::HandleTellLostServerToMaster );
        __REGISTER_MESSAGE__( KFMsg::S2S_TELL_REGISTER_SERVER_TO_MASTER, &KFTcpDiscoverModule::HandleTellRegisterServerToMaster );
        __REGISTER_MESSAGE__( KFMsg::S2S_TELL_SERVER_LIST_TO_MASTER, &KFTcpDiscoverModule::HandleTellServerListToMaster );
    }

    void KFTcpDiscoverModule::ShutDown()
    {
        __UNREGISTER_SERVER_DISCOVER_FUNCTION__();
        __UNREGISTER_SERVER_LOST_FUNCTION__();
        __UNREGISTER_CLIENT_CONNECTION_FUNCTION__();
        //////////////////////////////////////////////////////////////////////////////////////////////////
        __UNREGISTER_MESSAGE__( KFMsg::S2S_TELL_DISCOVER_SERVER_TO_MASTER );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_TELL_LOST_SERVER_TO_MASTER );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_TELL_REGISTER_SERVER_TO_MASTER );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_TELL_SERVER_LIST_TO_MASTER );
    }

    void KFTcpDiscoverModule::OnceRun()
    {
        auto kfglobal = KFGlobal::Instance();

        // 为了防止单点, 自动连接相同类型的服务器, 不在bus表里配置
        IpAddressList iplist;
        _kf_ip_address->FindIpAddress( kfglobal->_app_name, kfglobal->_app_type, _globbing_str, iplist );

        for ( auto netdata : iplist )
        {
            // 开启连接
            auto port = _kf_ip_address->CalcListenPort( netdata->_port_type, netdata->_port, netdata->_id );
            _kf_tcp_client->StartClient( netdata->_name, netdata->_type, netdata->_id, netdata->_ip, port );
        }
    }

    __KF_NET_EVENT_FUNCTION__( KFTcpDiscoverModule::OnServerDiscoverClient )
    {
        // 不是相同服务不要广播
        auto kfglobal = KFGlobal::Instance();
        if ( kfglobal->_app_name != netdata->_name )
        {
            return;
        }

        // 如果是master, 则连接( 为了做master多点 )
        if ( netdata->_type == kfglobal->_app_type )
        {
            _kf_tcp_client->StartClient( netdata );
        }
        else
        {
            KFMsg::ListenData listendata;
            listendata.set_appname( netdata->_name );
            listendata.set_apptype( netdata->_type );
            listendata.set_appid( netdata->_id );
            listendata.set_ip( netdata->_ip );
            listendata.set_port( netdata->_port );

            // 广播新连接给所有连接
            {
                KFMsg::TellRegisterToServer tell;
                tell.mutable_listen()->CopyFrom( listendata );
                _kf_tcp_server->SendNetMessage( KFMsg::S2S_TELL_REGISTER_TO_SERVER, &tell, netdata->_id );
            }

            // 所有连接信息发送给新连接
            {
                NetDataList handlelist;
                _kf_tcp_server->GetHandleList( handlelist );

                for ( auto kfhandle : handlelist )
                {
                    if ( kfhandle->_id == netdata->_id )
                    {
                        continue;
                    }

                    KFMsg::TellRegisterToServer tell;
                    auto listen = tell.mutable_listen();
                    listen->set_appname( kfhandle->_name );
                    listen->set_apptype( kfhandle->_type );
                    listen->set_appid( kfhandle->_id );
                    listen->set_ip( kfhandle->_ip );
                    listen->set_port( kfhandle->_port );
                    _kf_tcp_server->SendNetMessage( netdata->_id, KFMsg::S2S_TELL_REGISTER_TO_SERVER, &tell );
                }
            }

            // 通知其他master, 有新的连接
            KFMsg::S2STellDiscoverServerToMaster tell;
            tell.set_serverid( kfglobal->_app_id->GetId() );
            tell.mutable_listen()->CopyFrom( listendata );
            _kf_tcp_client->SendMessageToType( __KF_STRING__( master ), KFMsg::S2S_TELL_DISCOVER_SERVER_TO_MASTER, &tell );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFTcpDiscoverModule::HandleTellDiscoverServerToMaster )
    {
        __PROTO_PARSE__( KFMsg::S2STellDiscoverServerToMaster );

        auto listendata = &kfmsg.listen();

        // 广播新连接给所有连接
        {
            KFMsg::TellRegisterToServer tell;
            tell.mutable_listen()->CopyFrom( *listendata );
            _kf_tcp_server->SendNetMessage( KFMsg::S2S_TELL_REGISTER_TO_SERVER, &tell );
        }

        // 所有连接信息发送给新连接
        {
            NetDataList handlelist;
            _kf_tcp_server->GetHandleList( handlelist );

            for ( auto kfhandle : handlelist )
            {
                KFMsg::S2STellRegisterServerToMaster tell;
                tell.set_serverid( listendata->appid() );

                auto listen = tell.mutable_listen();
                listen->set_appname( kfhandle->_name );
                listen->set_apptype( kfhandle->_type );
                listen->set_appid( kfhandle->_id );
                listen->set_ip( kfhandle->_ip );
                listen->set_port( kfhandle->_port );
                _kf_tcp_client->SendNetMessage( kfmsg.serverid(), KFMsg::S2S_TELL_REGISTER_SERVER_TO_MASTER, &tell );
            }
        }
    }

    __KF_MESSAGE_FUNCTION__( KFTcpDiscoverModule::HandleTellRegisterServerToMaster )
    {
        __PROTO_PARSE__( KFMsg::S2STellRegisterServerToMaster );

        KFMsg::TellRegisterToServer tell;
        tell.mutable_listen()->CopyFrom( kfmsg.listen() );
        _kf_tcp_server->SendNetMessage( kfmsg.serverid(), KFMsg::S2S_TELL_REGISTER_TO_SERVER, &tell );
    }

    __KF_NET_EVENT_FUNCTION__( KFTcpDiscoverModule::OnServerLostClient )
    {
        // 不是相同服务不要广播
        auto kfglobal = KFGlobal::Instance();
        if ( kfglobal->_app_name != netdata->_name )
        {
            return;
        }

        if ( netdata->_type == kfglobal->_app_type )
        {
            _kf_tcp_client->CloseClient( netdata->_id, __FUNC_LINE__ );
        }
        else
        {
            {
                KFMsg::TellUnRegisterFromServer tell;
                tell.set_appname( netdata->_name );
                tell.set_apptype( netdata->_type );
                tell.set_appid( netdata->_id );
                _kf_tcp_server->SendNetMessage( KFMsg::S2S_TELL_UNREGISTER_FROM_SERVER, &tell, netdata->_id );
            }

            {
                // 通知其他master
                KFMsg::S2STellLostServerToMaster tell;
                tell.set_appname( netdata->_name );
                tell.set_apptype( netdata->_type );
                tell.set_appid( netdata->_id );
                _kf_tcp_client->SendMessageToType( __KF_STRING__( master ), KFMsg::S2S_TELL_LOST_SERVER_TO_MASTER, &tell );
            }
        }
    }

    __KF_MESSAGE_FUNCTION__( KFTcpDiscoverModule::HandleTellLostServerToMaster )
    {
        __PROTO_PARSE__( KFMsg::S2STellLostServerToMaster );

        KFMsg::TellUnRegisterFromServer tell;
        tell.set_appname( kfmsg.appname() );
        tell.set_apptype( kfmsg.apptype() );
        tell.set_appid( kfmsg.appid() );
        _kf_tcp_server->SendNetMessage( KFMsg::S2S_TELL_UNREGISTER_FROM_SERVER, &tell );
    }

    __KF_NET_EVENT_FUNCTION__( KFTcpDiscoverModule::OnClientConnectServer )
    {
        auto kfglobal = KFGlobal::Instance();
        if ( kfglobal->_app_name != netdata->_name && kfglobal->_app_type != netdata->_type )
        {
            return;
        }

        KFMsg::S2STellServerListToMaster tell;

        NetDataList handlelist;
        _kf_tcp_server->GetHandleList( handlelist );
        for ( auto kfhandle : handlelist )
        {
            auto listen = tell.add_listen();
            listen->set_appname( kfhandle->_name );
            listen->set_apptype( kfhandle->_type );
            listen->set_appid( kfhandle->_id );
            listen->set_ip( kfhandle->_ip );
            listen->set_port( kfhandle->_port );
        }

        _kf_tcp_client->SendNetMessage( netdata->_id, KFMsg::S2S_TELL_SERVER_LIST_TO_MASTER, &tell );
    }

    __KF_MESSAGE_FUNCTION__( KFTcpDiscoverModule::HandleTellServerListToMaster )
    {
        __PROTO_PARSE__( KFMsg::S2STellServerListToMaster );

        for ( auto i = 0; i < kfmsg.listen_size(); ++i )
        {
            KFMsg::TellRegisterToServer tell;
            tell.mutable_listen()->CopyFrom( kfmsg.listen( i ) );
            _kf_tcp_server->SendNetMessage( KFMsg::S2S_TELL_REGISTER_TO_SERVER, &tell );
        }
    }
}