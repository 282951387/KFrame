﻿#include "KFMessageModule.hpp"

namespace KFrame
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFMessageModule::AddFunction( KFModule* module, uint32 msgid, KFMessageFunction& function )
    {
        auto kffunction = _kf_message_function.Find( msgid );
        if ( kffunction == nullptr )
        {
            kffunction = _kf_message_function.Create( msgid );
            kffunction->SetFunction( module, function );
        }
        else
        {
            __LOG_ERROR__( "msgid[{}] already register", msgid );
        }
    }

    bool KFMessageModule::CallFunction( const Route& route, uint32 msgid, const char* data, uint32 length )
    {
        auto kffunction = _kf_message_function.Find( msgid );
        if ( kffunction == nullptr )
        {
            return false;
        }

        kffunction->Call( route, msgid, data, length );
        return true;
    }

    void KFMessageModule::UnRegisterFunction( uint32 msgid )
    {
        auto ok = _kf_message_function.Remove( msgid );
        if ( !ok )
        {
            __LOG_ERROR__( "msgid[{}] unregister failed", msgid );
        }
    }

    bool KFMessageModule::OpenFunction( uint32 msgid, bool open )
    {
        auto kffunction = _kf_message_function.Find( msgid );
        if ( kffunction == nullptr )
        {
            return false;
        }

        kffunction->_is_open = open;
        return true;
    }

}