#ifndef __KF_TCP_SERVER_INTERFACE_H__
#define __KF_TCP_SERVER_INTERFACE_H__

#include "KFrame.h"

namespace KFrame
{
    class KFTcpServerInterface : public KFModule
    {
    public:
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////
        // ע�ᵽ������
        virtual bool RegisteNetHandle( uint32 sessionid, uint32 handleid, uint32 objectid ) = 0;

        // �ر�������
        virtual bool CloseNetHandle( uint32 handleid, uint32 delaytime, const char* function, uint32 line ) = 0;

        // ��������
        virtual uint32 GetHandleCount() = 0;

        // �����б�
        virtual void GetHandleList( std::list< uint32 >& handlelist ) = 0;

        // �������ip
        virtual const std::string& GetHandleIp( uint32 handleid ) = 0;

        // ����id
        virtual bool BindObjectId( uint32 handleid, uint32 objectid ) = 0;
        ////////////////////////////////////////////////////////////////////////////////////

        // ��ȫ���ͻ��˷�����Ϣ
        virtual void SendNetMessage( uint32 msgid, const char* data, uint32 length, uint32 excludeid = 0 ) = 0;
        virtual void SendNetMessage( uint32 msgid, google::protobuf::Message* message, uint32 excludeid = 0 ) = 0;

        // ��ָ���ͻ��˷�����Ϣ
        virtual bool SendNetMessage( uint32 handleid, uint32 msgid, const char* data, uint32 length ) = 0;
        virtual bool SendNetMessage( uint32 handleid, uint32 msgid, google::protobuf::Message* message ) = 0;

        // ��ָ����������Ϣ
        virtual bool SendNetMessage( uint32 handleid, uint32 objectid, uint32 msgid, const char* data, uint32 length ) = 0;
        virtual bool SendNetMessage( uint32 handleid, uint32 objectid, uint32 msgid, google::protobuf::Message* message ) = 0;

        // ��ָ����������Ϣ
        virtual bool SendNetMessage( const KFGuid& kfguid, uint32 msgid, const char* data, uint32 length ) = 0;
        virtual bool SendNetMessage( const KFGuid& kfguid, uint32 msgid, google::protobuf::Message* message ) = 0;

        // ��ָ�����ͷ�����Ϣ
        virtual void SendMessageToName( const std::string& name, uint32 msgid, google::protobuf::Message* message ) = 0;
        virtual void SendMessageToType( const std::string& type, uint32 msgid, google::protobuf::Message* message ) = 0;

        // ��ĳһ���Ϳͻ��˷�����Ϣ
        //////////////////////////////////////////////////////////////////////////////////////////
        // ע�����ӳɹ�����
        template< class T >
        void RegisterDiscoverFunction( T* object,
                                       void ( T::*handle )( uint32 handleid, const std::string& servername, const std::string& servertype, const std::string& ip, uint32 port ) )
        {
            KFServerDiscoverFunction function = std::bind( handle, object,
                                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5 );

            AddDiscoverFunction( typeid( T ).name(), function );
        }

        // ж��
        template< class T >
        void UnRegisterDiscoverFunction( T* object )
        {
            RemoveDiscoverFunction( typeid( T ).name() );
        }
        //////////////////////////////////////////////////////////////////////////////////////////

        // ע����߻ص�����
        template< class T >
        void RegisterLostFunction( T* object,
                                   void ( T::*handle )( uint32 id, const std::string& servername, const std::string& servertype  ) )
        {
            KFServerLostFunction function = std::bind( handle, object,
                                            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );

            AddLostFunction( typeid( T ).name(), function );
        }

        // ж��
        template< class T >
        void UnRegisterLostFunction( T* object )
        {
            RemoveLostFunction( typeid( T ).name() );
        }
        /////////////////////////////////////////////////////////////////////////////
        // ע��ת��
        template< class T >
        void RegisterTransmitFunction( T* object,
                                       bool ( T::*handle )( const KFGuid& guid, uint32 msgid, const char* data, uint32 length ) )
        {
            KFTransmitFunction function = std::bind( handle, object,
                                          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 );
            AddTransmitFunction( typeid( T ).name(), function );
        }

        template< class T >
        void UnRegisterTransmitFunction( T* object )
        {
            RemoveTransmitFunction( typeid( T ).name() );
        }

    private:
        virtual void AddDiscoverFunction( const char* module, KFServerDiscoverFunction& function ) = 0;
        virtual void RemoveDiscoverFunction( const char* module ) = 0;

        virtual void AddLostFunction( const char* module, KFServerLostFunction& function ) = 0;
        virtual void RemoveLostFunction( const char* module ) = 0;

        virtual void AddTransmitFunction( const char* module, KFTransmitFunction& function ) = 0;
        virtual void RemoveTransmitFunction( const char* module ) = 0;

    };

    //////////////////////////////////////////////////////////////////////////////////////////
    __KF_INTERFACE__( _kf_tcp_server, KFTcpServerInterface );
    //////////////////////////////////////////////////////////////////////////////////////////

#define __KF_SERVER_LOST_FUNCTION__( function ) \
    void function( uint32 handleid, const std::string& handlename, const std::string& handletype )

#define __KF_SERVER_DISCOVER_FUNCTION__( function ) \
    void function( uint32 handleid, const std::string& handlename , const std::string& handletype, const std::string& ip, uint32 port )

#define __REGISTER_SERVER_DISCOVER_FUNCTION__( function ) \
    _kf_tcp_server->RegisterDiscoverFunction( this, function )

#define __UNREGISTER_SERVER_DISCOVER_FUNCTION__() \
    _kf_tcp_server->UnRegisterDiscoverFunction( this )

#define __REGISTER_SERVER_LOST_FUNCTION__( function ) \
    _kf_tcp_server->RegisterLostFunction( this, function )

#define __UNREGISTER_SERVER_LOST_FUNCTION__() \
    _kf_tcp_server->UnRegisterLostFunction( this )

#define __REGISTER_SERVER_TRANSMIT_FUNCTION__( function ) \
    _kf_tcp_server->RegisterTransmitFunction( this, function )

#define __UNREGISTER_SERVER_TRANSMIT_FUNCTION__() \
    _kf_tcp_server->UnRegisterTransmitFunction( this )

}



#endif