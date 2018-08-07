#ifndef __KF_MYSQL_INTERFACE_H__
#define __KF_MYSQL_INTERFACE_H__

#include "KFrame.h"

namespace KFrame
{

    class KFMySQLDriver
    {
    public:
        // ��ѯ�ֶ�
        template< class T >
        bool Select( const std::string& table, const std::string& key, const std::string& field, T& outvalue )
        {
            std::string strvalue = "";
            auto ok = SelectExecute( table, key, field, strvalue );
            if ( ok )
            {
                outvalue = KFUtility::ToValue< T >( strvalue );
            }

            return ok;
        }

        // ��ѯ�ֶ��б�
        template< class T >
        bool Select( const std::string& table, const std::string& key, const std::string& field, uint32 limitcount, std::list< T >& outvalue )
        {
            ListString queryvalue;
            auto ok = SelectExecute( table, key, field, limitcount, queryvalue );
            if ( ok )
            {
                for ( auto strvalue : queryvalue )
                {
                    outvalue.push_back( KFUtility::ToValue< T >( strvalue ) );
                }
            }

            return ok;
        }

        // ��ѯ����ֶε�ֵ
        virtual bool Select( const std::string& table, const std::string& key, MapString& outvalue ) = 0;
        virtual bool Select( const std::string& table, const std::string& key, std::list< MapString >& outvalue ) = 0;
        virtual bool Select( const std::string& table, const std::string& key, ListString& fields, std::list< MapString >& outvalue ) = 0;

        // �����¼
        virtual bool Insert( const std::string& table, const MapString& invalue ) = 0;

        // ɾ��
        virtual bool Delete( const std::string& table, const std::string& key ) = 0;
        virtual bool Delete( const std::string& table, const ListString& keys ) = 0;

        // �����ֶ�
        template< class T >
        bool Update( const std::string& table, const std::string& key, const std::string& field, T invalue )
        {
            auto strvalue = KFUtility::ToString< T >( invalue );
            return UpdateExecute( table, key, field, strvalue );
        }

        // ���¶���ֶ�
        virtual bool Update( const std::string& table, const std::string& key, const MapString& invalue ) = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:
        template< typename... P >
        bool Execute( const char* myfmt, P&& ... args )
        {
            auto strsql = __FORMAT__( myfmt, std::forward<P>( args )... );
            return UpdateExecute( strsql );
        }

        // ����
        virtual void Pipeline( const VectorString& commands ) = 0;

    protected:
        // ��ѯ
        virtual bool SelectExecute( const std::string& table, const std::string& key, const std::string& field, std::string& outvalue ) = 0;
        virtual bool SelectExecute( const std::string& table, const std::string& key, const std::string& field, uint32 limit, ListString& outvalue ) = 0;

        // ����
        virtual bool UpdateExecute( const std::string& strsql ) = 0;
        virtual bool UpdateExecute( const std::string& table, const std::string& key, const std::string& field, const std::string& invalue ) = 0;
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    class KFMySQLInterface : public KFModule
    {
    public:
        // ����Execute
        virtual KFMySQLDriver* CreateExecute( uint32 id ) = 0;
        virtual KFMySQLDriver* CreateExecute( const std::string& filed, uint32 logicid = 0 ) = 0;
        virtual KFMySQLDriver* CreateExecute( uint32 id, const std::string& user, const std::string& password, const std::string& database, const std::string& ip, uint32 port ) = 0;

    };

    ///////////////////////////////////////////////////////////////////////////////////////////////
    __KF_INTERFACE__( _kf_mysql, KFMySQLInterface );
    ///////////////////////////////////////////////////////////////////////////////////////////////
}

#endif