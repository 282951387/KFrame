﻿#ifndef __KF_REDIS_EXECUTE_H__
#define __KF_REDIS_EXECUTE_H__

#include "KFrame.h"
#include "hiredis/hiredis.h"

namespace KFrame
{
    class KFRedisExecute
    {
    public:
        /////////////////////////////////////////////////////////////////////////////////////////////
        KFRedisExecute();
        virtual ~KFRedisExecute();

        // 初始化
        int32 Initialize( const std::string& name, const std::string& ip, uint32 port, const std::string& password );

        // 关闭
        void ShutDown();

    protected:

        // 连接
        int32 TryConnect();

        // 是否断开连接
        bool IsDisconnected();

        // 执行语句
        redisReply* Execute( const std::string& strsql );
        redisReply* TryExecute( KFBaseResult* kfresult, const std::string& strsql );

    protected:
        // 名字
        std::string _name;

        // ip
        std::string _ip;

        // 端口
        uint32 _port;

        // 密码
        std::string _password;

        // redis环境
        redisContext* _redis_context;

        // 返回结果列表
        std::vector< KFBaseResultQueue* > _result_queue_list;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    class KFWriteExecute : public KFRedisExecute
    {
    public:
        KFWriteExecute() = default;
        virtual ~KFWriteExecute() = default;

        // 写操作
        virtual KFResult< voidptr >::UniqueType VoidExecute( const std::string& strsql );
        virtual KFResult< uint64 >::UniqueType UpdateExecute( const std::string& strsql );

        // 批量写
        virtual KFResult< voidptr >::UniqueType Pipeline( const ListString& commands );

        /////////////////////////////////////////////////////////////////////////////////////////////
    };
    /////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    class KFReadExecute : public KFRedisExecute
    {
    public:
        KFReadExecute() = default;
        virtual ~KFReadExecute() = default;

        ///////////////////////////////////////////////////////
        // 读操作
        virtual KFResult< uint64 >::UniqueType UInt64Execute( const std::string& strsql );
        virtual KFResult< std::string >::UniqueType StringExecute( const std::string& strsql );
        virtual KFResult< MapString >::UniqueType MapExecute( const std::string& strsql );
        virtual KFResult< ListString >::UniqueType ListExecute( const std::string& strsql );
        virtual KFResult< VectorString >::UniqueType VectorExecute( const std::string& strsql );

        // 添加执行命令
        virtual KFResult< ListString >::UniqueType ListPipelineExecute( const ListString& commands );
        virtual KFResult< std::list< MapString > >::UniqueType ListMapPipelineExecute( const ListString& commands );
        ///////////////////////////////////////////////////////
    };
}
#endif