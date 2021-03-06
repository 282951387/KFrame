﻿#ifndef __KF_BASIC_DATABASE_MONGO_H__
#define __KF_BASIC_DATABASE_MONGO_H__

#include "KFBasicDatabaseLogic.hpp"
#include "KFMongo/KFMongoInterface.h"

namespace KFrame
{
    class KFBasicDatabaseMongo : public KFBasicDatabaseLogic
    {
    public:
        KFBasicDatabaseMongo() = default;
        ~KFBasicDatabaseMongo() = default;

        // 更新属性
        virtual void UpdateBasicIntValue( uint64 playerid, uint64 serverid, const StringUInt64& values );
        virtual void UpdateBasicStrValue( uint64 playerid, uint64 serverid, const StringMap& values );

        // 查询玩家id
        virtual uint64 QueryBasicPlayerid( const std::string& playername, uint32 zoneid );

        // 清除玩家的在线服务器id
        virtual void ClearBasicServerId( uint64 serverid );

        // 查询玩家的服务器id
        virtual uint64 QueryBasicServerId( uint64 playerid );

        // 查询玩家基础信息
        virtual uint32 QueryBasicAttribute( uint64 playerid, StringMap& values );

        // 设置玩家名字
        virtual uint32 SetPlayerName( uint32 zoneid, uint64 playerid, const std::string& oldname, const std::string& newname );

        // 查询属性
        virtual uint64 QueryBasicIntValue( uint64 playerid, const std::string& dataname );
        virtual std::string QueryBasicStrValue( uint64 playerid, const std::string& dataname );
    protected:
        // 格式化名字字段
        std::string FormatNameTable( uint32 zoneid );
    };
}



#endif