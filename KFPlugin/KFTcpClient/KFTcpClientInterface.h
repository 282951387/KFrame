﻿#ifndef __KF_CLIENT_INTERFACE_H__
#define __KF_CLIENT_INTERFACE_H__

#include "KFrame.h"

namespace KFrame
{
    class KFTcpClientInterface : public KFModule
    {
    public:
        // 添加连接
        virtual void StartClient( const KFNetData* netdata ) = 0;
        virtual void StartClient( const std::string& name, const std::string& type, uint64 id, const std::string& ip, uint32 port ) = 0;

        // 断开连接
        virtual void CloseClient( uint64 serverid, const char* function, uint32 line ) = 0;
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // 发送消息
        virtual void SendNetMessage( uint32 msgid, google::protobuf::Message* message, uint32 delay = 0 ) = 0;
        virtual bool SendNetMessage( uint64 serverid, uint32 msgid, google::protobuf::Message* message, uint32 delay = 0 ) = 0;
        virtual bool SendNetMessage( uint64 serverid, uint64 recvid, uint32 msgid, google::protobuf::Message* message, uint32 delay = 0 ) = 0;

        virtual void SendNetMessage( uint32 msgid, const char* data, uint32 length ) = 0;
        virtual bool SendNetMessage( uint64 serverid, uint32 msgid, const char* data, uint32 length ) = 0;
        virtual bool SendNetMessage( uint64 serverid, uint64 recvid, uint32 msgid, const char* data, uint32 length ) = 0;

        // 给某一类型服务器发送消息
        virtual void SendMessageToName( const std::string& servername, uint32 msgid, google::protobuf::Message* message ) = 0;
        virtual void SendMessageToName( const std::string& servername, uint32 msgid, const char* data, uint32 length ) = 0;

        virtual void SendMessageToType( const std::string& servertype, uint32 msgid, google::protobuf::Message* message ) = 0;
        virtual void SendMessageToType( const std::string& servertype, uint32 msgid, const char* data, uint32 length ) = 0;

        virtual void SendMessageToServer( const std::string& servername, const std::string& servertype, uint32 msgid, google::protobuf::Message* message ) = 0;
        virtual void SendMessageToServer( const std::string& servername, const std::string& servertype, uint32 msgid, const char* data, uint32 length ) = 0;


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 添加注册回调函数
        template< class T >
        void RegisterConnectionFunction( T* module, void ( T::*handle )( const KFNetData* ) )
        {
            KFNetEventFunction function = std::bind( handle, module, std::placeholders::_1 );
            AddConnectionFunction( module, function );
        }

        // 删除注册回调函数
        template< class T >
        void UnRegisterConnectionFunction( T* module )
        {
            RemoveConnectionFunction( module );
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 断线函数回调
        template< class T >
        void RegisterLostFunction( T* module, void ( T::*handle )( const KFNetData* ) )
        {
            KFNetEventFunction function = std::bind( handle, module, std::placeholders::_1 );
            AddLostFunction( module, function );
        }

        // 删除注册回调函数
        template< class T >
        void UnRegisterLostFunction( T* module )
        {
            RemoveLostFunction( module );
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 关闭函数回调
        template< class T >
        void RegisterShutdownFunction( T* module, void ( T::*handle )( const KFNetData* ) )
        {
            KFNetEventFunction function = std::bind( handle, module, std::placeholders::_1 );
            AddShutdownFunction( module, function );
        }

        // 删除注册回调函数
        template< class T >
        void UnRegisterShutdownFunction( T* module )
        {
            RemoveShutdownFunction( module );
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 失败函数回调
        template< class T >
        void RegisterFailedFunction( T* module, void ( T::*handle )( const KFNetData* ) )
        {
            KFNetEventFunction function = std::bind( handle, module, std::placeholders::_1 );
            AddFailedFunction( module, function );
        }

        // 删除失败函数
        template< class T >
        void UnRegisterFailedFunction( T* module )
        {
            RemoveFailedFunction( module );
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 注册转发
        template< class T >
        void RegisterTranspondFunction( T* module, bool ( T::*handle )( const Route& route, uint32 msgid, const char* data, uint32 length ) )
        {
            KFTranspondFunction function = std::bind( handle, module, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 );
            AddTranspondFunction( module, function );
        }

        template< class T >
        void UnRegisterTranspondFunction( T* module )
        {
            RemoveTranspondFunction( module );
        }

    protected:
        // 添加注册回调函数
        virtual void AddConnectionFunction( KFModule* module, KFNetEventFunction& function ) = 0;
        virtual void RemoveConnectionFunction( KFModule* module ) = 0;

        // 添加断线函数
        virtual void AddLostFunction( KFModule* module, KFNetEventFunction& function ) = 0;
        virtual void RemoveLostFunction( KFModule* module ) = 0;

        // 添加关闭函数
        virtual void AddShutdownFunction( KFModule* module, KFNetEventFunction& function ) = 0;
        virtual void RemoveShutdownFunction( KFModule* module ) = 0;

        // 添加失败函数
        virtual void AddFailedFunction( KFModule* module, KFNetEventFunction& function ) = 0;
        virtual void RemoveFailedFunction( KFModule* module ) = 0;

        // 转发函数
        virtual void AddTranspondFunction( KFModule* module, KFTranspondFunction& function ) = 0;
        virtual void RemoveTranspondFunction( KFModule* module ) = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    __KF_INTERFACE__( _kf_tcp_client, KFTcpClientInterface );

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define __REGISTER_TCP_CLIENT_CONNECTION__( function ) \
    _kf_tcp_client->RegisterConnectionFunction( this, function )

#define __UN_TCP_CLIENT_CONNECTION__() \
    _kf_tcp_client->UnRegisterConnectionFunction( this )

#define __REGISTER_TCP_CLIENT_LOST__( function ) \
    _kf_tcp_client->RegisterLostFunction( this, function )

#define __UN_TCP_CLIENT_LOST__() \
    _kf_tcp_client->UnRegisterLostFunction( this )

#define __REGISTER_TCP_CLIENT_SHUTDOWN__( function ) \
    _kf_tcp_client->RegisterShutdownFunction( this, function )

#define __UN_TCP_CLIENT_SHUTDOWN__() \
    _kf_tcp_client->UnRegisterShutdownFunction( this )

#define __REGISTER_TCP_CLIENT_FAILED__( function ) \
    _kf_tcp_client->RegisterFailedFunction( this, function )

#define __UN_TCP_CLIENT_FAILED__() \
    _kf_tcp_client->UnRegisterFailedFunction( this )

#define __REGISTER_TCP_CLIENT_TRANSPOND__( function ) \
    _kf_tcp_client->RegisterTranspondFunction( this, function )

#define __UN_TCP_CLIENT_TRANSPOND__() \
    _kf_tcp_client->UnRegisterTranspondFunction( this )
}

#endif