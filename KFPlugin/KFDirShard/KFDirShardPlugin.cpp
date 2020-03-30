﻿#include "KFDirShardPlugin.hpp"
#include "KFDirShardModule.hpp"
//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFDirShardPlugin::Install()
    {
        __REGISTER_MODULE__( KFDirShard );
    }

    void KFDirShardPlugin::UnInstall()
    {
        __UN_MODULE__( KFDirShard );
    }

    void KFDirShardPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_redis, KFRedisInterface );
        __FIND_MODULE__( _kf_mongo, KFMongoInterface );
        __FIND_MODULE__( _kf_http_server, KFHttpServerInterface );
    }

    void KFDirShardPlugin::AddConfig()
    {
    }
}