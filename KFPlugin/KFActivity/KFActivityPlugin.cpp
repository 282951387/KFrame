﻿#include "KFActivityPlugin.hpp"
#include "KFActivityModule.hpp"
#include "KFConfig/KFConfigInterface.h"
//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFActivityPlugin::Install()
    {
        __REGISTER_MODULE__( KFActivity );
    }

    void KFActivityPlugin::UnInstall()
    {
        __UN_MODULE__( KFActivity );
    }

    void KFActivityPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_config, KFConfigInterface );
        __FIND_MODULE__( _kf_kernel, KFKernelInterface );
        __FIND_MODULE__( _kf_player, KFPlayerInterface );
        __FIND_MODULE__( _kf_message, KFMessageInterface );
        __FIND_MODULE__( _kf_display, KFDisplayInterface );
    }

    void KFActivityPlugin::AddConfig()
    {
        __KF_ADD_CONFIG__( KFActivityConfig );
    }

}