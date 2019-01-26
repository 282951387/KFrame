#ifndef __KF_MYSQL_MODULE_H__
#define __KF_MYSQL_MODULE_H__

/************************************************************************
//    @Module			:    mysql���ݿ�
//    @Author           :    __��_��__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-5-10
************************************************************************/

#include "KFrame.h"
#include "KFMySQLInterface.h"
#include "KFMySQLExecute.h"
#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
    class KFMySQLModule : public KFMySQLInterface
    {
    public:
        KFMySQLModule() = default;
        ~KFMySQLModule() = default;

        // ��������
        virtual void InitModule();

        // �ر�
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // ����Execute
        virtual KFMySQLDriver* CreateExecute( const std::string& module, uint32 logicid = 0 );
        virtual KFMySQLDriver* CreateExecute( uint32 id, const std::string& user, const std::string& password, const std::string& database, const std::string& ip, uint32 port );

    protected:
        // ��ѯ
        KFMySQLDriver* FindMySQLExecute( uint32 id );

        // ����
        void InsertMySQLExecute( uint32 id, KFMySQLExecute* kfexecute );

    private:
        // ������
        KFMutex _kf_mutex;

        // ���ݿ��б�
        typedef std::pair< uint32, uint32 > MySQLKey;
        KFMap< MySQLKey, const MySQLKey&, KFMySQLExecute > _mysql_execute_map;
    };
}



#endif