﻿#include "KFMySQLPlugin.hpp"
#include "KFMySQLModule.hpp"

//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFMySQLPlugin::Install()
    {
        __REGISTER_MODULE__( KFMySQL );
    }

    void KFMySQLPlugin::UnInstall()
    {
        __UN_MODULE__( KFMySQL );
    }

    void KFMySQLPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_config, KFConfigInterface );
    }
}