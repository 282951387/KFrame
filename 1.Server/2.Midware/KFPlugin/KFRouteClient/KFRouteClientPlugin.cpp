﻿#include "KFRouteClientPlugin.h"
#include "KFRouteClientModule.h"
//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFRouteClientPlugin::Install()
    {
        __REGISTER_MODULE__( KFRouteClient );
    }

    void KFRouteClientPlugin::UnInstall()
    {
        __UNREGISTER_MODULE__( KFRouteClient );
    }

    void KFRouteClientPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_message, KFMessageInterface );
        __FIND_MODULE__( _kf_ip_address, KFIpAddressInterface );
        __FIND_MODULE__( _kf_tcp_client, KFTcpClientInterface );
        __FIND_MODULE__( _kf_cluster_client, KFClusterClientInterface );
    }
}