﻿#include "KFRobotPlugin.hpp"
#include "KFRobotModule.hpp"
#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
    void KFRobotPlugin::Install()
    {
        __REGISTER_MODULE__( KFRobot );
    }

    void KFRobotPlugin::UnInstall()
    {
        __UN_MODULE__( KFRobot );
    }

    void KFRobotPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_timer, KFTimerInterface );
        __FIND_MODULE__( _kf_config, KFConfigInterface );
        __FIND_MODULE__( _kf_kernel, KFKernelInterface );
        __FIND_MODULE__( _kf_http_client, KFHttpClientInterface );
    }

    void KFRobotPlugin::AddConfig()
    {
        __KF_ADD_CONFIG__( KFRobotConfig );
    }
}