﻿#ifndef __KF_TCP_SERVER_INTERFACE_H__
#define __KF_TCP_SERVER_INTERFACE_H__

#include "KFrame.h"

namespace KFrame
{
    class KFTcpServerInterface : public KFModule
    {
    public:
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////
        // 注册到连接器
        virtual bool RegisteNetHandle( uint64 sessionid, uint64 handleid, uint64 objectid ) = 0;

        // 关闭连接器
        virtual bool CloseNetHandle( uint64 handleid, uint32 delaytime, const char* function, uint32 line ) = 0;

        // 连接数量
        virtual uint32 GetHandleCount() = 0;

        // 是否存在连接
        virtual bool HaveHandle( uint64 handleid ) = 0;

        // 获得连接ip
        virtual const std::string& GetHandleIp( uint64 handleid ) = 0;

        // 设置id
        virtual bool BindObjectId( uint64 handleid, uint64 objectid ) = 0;

        // 连接列表
        virtual void GetHandleList( NetDataList& outlist ) = 0;
        ////////////////////////////////////////////////////////////////////////////////////

        // 给全部客户端发送消息
        virtual void SendNetMessage( uint32 msgid, const char* data, uint32 length, uint64 excludeid = 0 ) = 0;
        virtual void SendNetMessage( uint32 msgid, google::protobuf::Message* message, uint64 excludeid = 0 ) = 0;

        // 给指定客户端发送消息
        virtual bool SendNetMessage( uint64 handleid, uint32 msgid, const char* data, uint32 length, uint32 delay = 0u ) = 0;
        virtual bool SendNetMessage( uint64 handleid, uint32 msgid, google::protobuf::Message* message, uint32 delay = 0u ) = 0;

        // 给指定对象发送消息
        virtual bool SendNetMessage( uint64 handleid, uint64 recvid, uint32 msgid, const char* data, uint32 length, uint32 delay = 0u ) = 0;
        virtual bool SendNetMessage( uint64 handleid, uint64 recvid, uint32 msgid, google::protobuf::Message* message, uint32 delay = 0u ) = 0;

        // 给指定类型发送消息
        virtual void SendMessageToName( const std::string& name, uint32 msgid, google::protobuf::Message* message ) = 0;

        virtual void SendMessageToType( const std::string& type, uint32 msgid, google::protobuf::Message* message ) = 0;
        virtual void SendMessageToType( const std::string& type, uint32 msgid, const char* data, uint32 length ) = 0;

        // 给某一类型客户端发送消息
        //////////////////////////////////////////////////////////////////////////////////////////
        // 注册连接成功函数
        template< class T >
        void RegisterDiscoverFunction( T* module, void ( T::*handle )( const KFNetData* ) )
        {
            KFNetEventFunction function = std::bind( handle, module, std::placeholders::_1 );
            AddDiscoverFunction( module, function );
        }

        // 卸载
        template< class T >
        void UnRegisterDiscoverFunction( T* module )
        {
            RemoveDiscoverFunction( module );
        }
        //////////////////////////////////////////////////////////////////////////////////////////

        // 注册断线回调函数
        template< class T >
        void RegisterLostFunction( T* module, void ( T::*handle )( const KFNetData* ) )
        {
            KFNetEventFunction function = std::bind( handle, module, std::placeholders::_1 );
            AddLostFunction( module, function );
        }

        // 卸载
        template< class T >
        void UnRegisterLostFunction( T* module )
        {
            RemoveLostFunction( module );
        }
        /////////////////////////////////////////////////////////////////////////////
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

    private:
        virtual void AddDiscoverFunction( KFModule* module, KFNetEventFunction& function ) = 0;
        virtual void RemoveDiscoverFunction( KFModule* module ) = 0;

        virtual void AddLostFunction( KFModule* module, KFNetEventFunction& function ) = 0;
        virtual void RemoveLostFunction( KFModule* module ) = 0;

        virtual void AddTranspondFunction( KFModule* module, KFTranspondFunction& function ) = 0;
        virtual void RemoveTranspondFunction( KFModule* module ) = 0;

    };

    //////////////////////////////////////////////////////////////////////////////////////////
    __KF_INTERFACE__( _kf_tcp_server, KFTcpServerInterface );
    //////////////////////////////////////////////////////////////////////////////////////////

#define __REGISTER_TCP_SERVER_DISCOVER__( function ) \
    _kf_tcp_server->RegisterDiscoverFunction( this, function )

#define __UN_TCP_SERVER_DISCOVER__() \
    _kf_tcp_server->UnRegisterDiscoverFunction( this )

#define __REGISTER_TCP_SERVER_LOST__( function ) \
    _kf_tcp_server->RegisterLostFunction( this, function )

#define __UN_TCP_SERVER_LOST__() \
    _kf_tcp_server->UnRegisterLostFunction( this )

#define __REGISTER_TCP_SERVER_TRANSPOND__( function ) \
    _kf_tcp_server->RegisterTranspondFunction( this, function )

#define __UN_TCP_SERVER_TRANSPOND__() \
    _kf_tcp_server->UnRegisterTranspondFunction( this )

}



#endif