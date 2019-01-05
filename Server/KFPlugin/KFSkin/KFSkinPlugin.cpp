﻿#include "KFSkinPlugin.h"
#include "KFSkinModule.h"

//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFSkinPlugin::Install()
    {
        __REGISTER_MODULE__( KFSkin );
    }

    void KFSkinPlugin::UnInstall()
    {
        __UNREGISTER_MODULE__( KFSkin );
    }

    void KFSkinPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_timer, KFTimerInterface );
        __FIND_MODULE__( _kf_config, KFConfigInterface );
        __FIND_MODULE__( _kf_kernel, KFKernelInterface );
        __FIND_MODULE__( _kf_player, KFPlayerInterface );
        __FIND_MODULE__( _kf_message, KFMessageInterface );
        __FIND_MODULE__( _kf_mail, KFMailClientInterface );
        __FIND_MODULE__( _kf_display, KFDisplayInterface );
    }
}