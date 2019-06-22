﻿#include "KFNetServerEngine.h"

namespace KFrame
{
    KFNetServerEngine::KFNetServerEngine()
    {
        _net_function = nullptr;
        _net_server_services = nullptr;
        _server_lost_function = nullptr;
    }

    KFNetServerEngine::~KFNetServerEngine()
    {
        delete _net_server_services;
    }

    ////////////////////////////////////////////////////
    void KFNetServerEngine::InitEngine( uint32 maxqueuesize, uint32 messagetype, uint32 compress )
    {
        _net_server_services = new KFNetServerServices();
        _net_server_services->InitServices( 10000, maxqueuesize, messagetype, compress );
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        _net_server_services->_net_event->BindConnectFunction( this, &KFNetServerEngine::OnServerConnected );
        _net_server_services->_net_event->BindShutFunction( this, &KFNetServerEngine::OnServerShutDown );
        _net_server_services->_net_event->BindDisconnectFunction( this, &KFNetServerEngine::OnServerDisconnect );
    }

    int32 KFNetServerEngine::StartEngine( const std::string& ip, uint32 port )
    {
        KFNetData netdata;
        netdata._ip = ip;
        netdata._port = port;
        return _net_server_services->StartServices( &netdata );
    }

    void KFNetServerEngine::ShutEngine()
    {
        {
            auto kfhandle = _trustee_handles.First();
            while ( kfhandle != nullptr )
            {
                kfhandle->CloseHandle();
                kfhandle = _trustee_handles.Next();
            }
        }

        {
            auto kfhandle = _kf_handles.First();
            while ( kfhandle != nullptr )
            {
                kfhandle->CloseHandle();
                kfhandle = _kf_handles.Next();
            }
        }

        _net_server_services->ShutServices();
        _net_server_services->_net_event->ShutEvent();
    }

    void KFNetServerEngine::RunEngine( uint64 nowtime )
    {
        _net_server_services->_now_time = nowtime;
        _net_server_services->_net_event->RunEvent();

        // 判断托管超时
        RunCheckTrusteeTimeout();

        // 需要关闭的连接
        RunCloseHandle();

        // 托管连接消息
        RunTrusteeMessage( nowtime );
        RunRemoveTrusteeHandle();

        // 注册连接消息
        RunHandleMessage( nowtime );
    }

    uint32 KFNetServerEngine::GetHandleCount()
    {
        return _kf_handles.Size();
    }

    void KFNetServerEngine::GetHandleList( NetDataList& outlist )
    {
        outlist.clear();

        auto kfhandle = _kf_handles.First();
        while ( kfhandle != nullptr )
        {
            outlist.push_back( &kfhandle->_net_data );
            kfhandle = _kf_handles.Next();
        }
    }

    KFNetHandle* KFNetServerEngine::FindNetHandle( uint64 handleid )
    {
        return _kf_handles.Find( handleid );
    }

    const std::string& KFNetServerEngine::GetHandleIp( uint64 handleid )
    {
        auto kfhandle = FindNetHandle( handleid );
        if ( kfhandle == nullptr )
        {
            return _invalid_str;
        }

        return kfhandle->_remote_ip;
    }

    void KFNetServerEngine::OnServerConnected( const KFEventData* eventdata )
    {
        // 加入托管列表
        auto kfhandle = reinterpret_cast< KFNetHandle* >( eventdata->_data );
        _trustee_handles.Insert( eventdata->_id, kfhandle );

        // 设置托管超时时间
        kfhandle->SetTrusteeTimeout( _net_server_services->_now_time + 30000 );
    }

    void KFNetServerEngine::OnServerShutDown( const KFEventData* eventdata )
    {
        auto istrustee = reinterpret_cast< uint64 >( eventdata->_data ) == 1 ? true : false;
        if ( istrustee )
        {
            auto ok = _trustee_handles.Remove( eventdata->_id );
            if ( !ok )
            {
                __LOG_ERROR__( "trustee handle[{}:{}] shutdown failed!", eventdata->_id, KFAppId::ToString( eventdata->_id ) );
            }
        }
        else
        {
            auto ok = _kf_handles.Remove( eventdata->_id );
            if ( !ok )
            {
                __LOG_ERROR__( "handle[{}:{}] shutdown failed!", eventdata->_id, KFAppId::ToString( eventdata->_id ) );
            }
        }

        __LOG_DEBUG__( "handle[{}:{}|{}] shutdown ok!", eventdata->_id, KFAppId::ToString( eventdata->_id ), reinterpret_cast< uint64 >( eventdata->_data ) );
    }

    void KFNetServerEngine::OnServerDisconnect( const KFEventData* eventdata )
    {
        // 断开连接
        auto kfhandle = _kf_handles.Find( eventdata->_id );
        if ( kfhandle != nullptr )
        {
            if ( _server_lost_function != nullptr )
            {
                _server_lost_function( &kfhandle->_net_data );
            }
        }
        else
        {
            kfhandle = _trustee_handles.Find( eventdata->_id );
            if ( kfhandle == nullptr )
            {
                return __LOG_ERROR__( "can't find handle[{}]!", eventdata->_id );
            }
        }

        kfhandle->CloseHandle();
    }

    bool KFNetServerEngine::CloseHandle( uint64 id, uint32 delaytime, const char* function, uint32 line )
    {
        __LOG_DEBUG_FUNCTION__( function, line, "add close handle[{}:{}]!", id, KFAppId::ToString( id ) );

        _close_handles[ id ] = _net_server_services->_now_time + delaytime;
        return true;
    }

    bool KFNetServerEngine::BindObjectId( uint64 handleid, uint64 objectid )
    {
        auto kfhandle = _kf_handles.Find( handleid );
        if ( kfhandle == nullptr )
        {
            return false;
        }

        kfhandle->_object_id = objectid;
        kfhandle->_net_data._id = objectid;
        kfhandle->_net_data._session = handleid;
        return true;
    }

    KFNetHandle* KFNetServerEngine::RegisteHandle( uint64 trusteeid, uint64 handleid, uint64 objectid )
    {
        auto kfhandle = _trustee_handles.Find( trusteeid );
        if ( kfhandle == nullptr || kfhandle->_is_shutdown || !kfhandle->_is_connected )
        {
            __LOG_ERROR__( "trustee handle[{}:{}] can't find!", trusteeid, handleid );
            return nullptr;
        }

        // 已经在列表中
        if ( _kf_handles.Find( handleid ) != nullptr )
        {
            __LOG_ERROR__( "handle[{}:{}] already exist!", handleid, KFAppId::ToString( handleid ) );
            return nullptr;
        }

        // 设置属性, 并加入注册列表中
        kfhandle->_is_trustee = false;
        kfhandle->_trustee_timeout = 0;
        kfhandle->_object_id = objectid;
        kfhandle->_session_id = handleid;
        _kf_handles.Insert( handleid, kfhandle );

        _remove_trustees.insert( trusteeid );

        return kfhandle;
    }

    void KFNetServerEngine::RunRemoveTrusteeHandle()
    {
        if ( _remove_trustees.empty() )
        {
            return;
        }

        for ( auto id : _remove_trustees )
        {
            _trustee_handles.Remove( id, false );
        }

        _remove_trustees.clear();
    }

    void KFNetServerEngine::RunCheckTrusteeTimeout()
    {
        auto kfhandle = _trustee_handles.First();
        while ( kfhandle != nullptr )
        {
            if ( kfhandle->IsTrusteeTimeout( _net_server_services->_now_time ) )
            {
                kfhandle->CloseHandle();
            }

            kfhandle = _trustee_handles.Next();
        }
    }

    void KFNetServerEngine::RunCloseHandle()
    {
        for ( auto iter = _close_handles.begin(); iter != _close_handles.end(); )
        {
            if ( iter->second > _net_server_services->_now_time )
            {
                ++iter;
                continue;
            }

            auto kfhandle = _kf_handles.Find( iter->first );
            if ( kfhandle != nullptr )
            {
                kfhandle->CloseHandle();
            }

            iter = _close_handles.erase( iter );
        }
    }

    void KFNetServerEngine::RunTrusteeMessage( uint64 nowtime )
    {
        // 每次取1个消息, 只处理认证消息
        static const uint32 _max_message_count = 1;

        for ( auto& iter : _trustee_handles._objects )
        {
            auto kfhandle = iter.second;
            kfhandle->RunUpdate( _net_function, _max_message_count );
        }
    }

    void KFNetServerEngine::RunHandleMessage( uint64 nowtime )
    {
        // 每次取200个消息, 防止占用过多的cpu
        static const uint32 _max_message_count = 200;

        for ( auto& iter : _kf_handles._objects )
        {
            auto kfhandle = iter.second;
            kfhandle->RunUpdate( _net_function, _max_message_count );
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFNetServerEngine::SendNetMessage( uint32 msgid, const char* data, uint32 length, uint64 excludeid /* = 0 */ )
    {
        auto kfhandle = _kf_handles.First();
        while ( kfhandle != nullptr )
        {
            if ( kfhandle->_object_id != excludeid )
            {
                kfhandle->SendNetMessage( kfhandle->_object_id, msgid, data, length );
            }

            kfhandle = _kf_handles.Next();
        }
    }

    void KFNetServerEngine::SendMessageToName( const std::string& name, uint32 msgid, const char* data, uint32 length )
    {
        auto kfhandle = _kf_handles.First();
        while ( kfhandle != nullptr )
        {
            if ( kfhandle->_net_data._name == name )
            {
                kfhandle->SendNetMessage( kfhandle->_object_id, msgid, data, length );
            }

            kfhandle = _kf_handles.Next();
        }
    }

    void KFNetServerEngine::SendMessageToType( const std::string& type, uint32 msgid, const char* data, uint32 length )
    {
        auto kfhandle = _kf_handles.First();
        while ( kfhandle != nullptr )
        {
            if ( kfhandle->_net_data._type == type )
            {
                kfhandle->SendNetMessage( kfhandle->_object_id, msgid, data, length );
            }

            kfhandle = _kf_handles.Next();
        }
    }

    bool KFNetServerEngine::SendNetMessage( uint64 handleid, uint32 msgid, const char* data, uint32 length )
    {
        KFNetHandle* handle = FindNetHandle( handleid );
        if ( handle == nullptr )
        {
            __LOG_ERROR__( "msgid[{}] can't find handle[{}]!", msgid, KFAppId::ToString( handleid ) );
            return false;
        }

        return handle->SendNetMessage( handleid, msgid, data, length );
    }

    bool KFNetServerEngine::SendNetMessage( uint64 handleid, uint64 recvid, uint32 msgid, const char* data, uint32 length )
    {
        KFNetHandle* handle = FindNetHandle( handleid );
        if ( handle == nullptr )
        {
            __LOG_ERROR__( "msgid[{}] can't find handle[{}]!", msgid, KFAppId::ToString( handleid ) );
            return false;
        }

        return handle->SendNetMessage( recvid, msgid, data, length );
    }
}
