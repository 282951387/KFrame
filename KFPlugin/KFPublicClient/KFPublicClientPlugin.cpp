﻿#include "KFPublicClientPlugin.hpp"
#include "KFPublicClientModule.hpp"

namespace KFrame
{
    void KFPublicClientPlugin::Install()
    {
        __REGISTER_MODULE__( KFPublicClient );
    }

    void KFPublicClientPlugin::UnInstall()
    {
        __UNREGISTER_MODULE__( KFPublicClient );
    }

    void KFPublicClientPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_player, KFPlayerInterface );
        __FIND_MODULE__( _kf_kernel, KFKernelInterface );
        __FIND_MODULE__( _kf_filter, KFFilterInterface );
        __FIND_MODULE__( _kf_display, KFDisplayInterface );
        __FIND_MODULE__( _kf_message, KFMessageInterface );
        __FIND_MODULE__( _kf_route, KFRouteClientInterface );
    }

}