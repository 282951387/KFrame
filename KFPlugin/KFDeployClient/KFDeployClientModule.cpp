﻿#include "KFDeployClientModule.hpp"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    void KFDeployClientModule::BeforeRun()
    {
        ////////////////////////////////////////////////////
        __REGISTER_MESSAGE__( KFMsg::S2S_DEPLOY_COMMAND_TO_CLIENT_REQ, &KFDeployClientModule::HandleDeployCommandToClientReq );
    }

    void KFDeployClientModule::ShutDown()
    {
        /////////////////////////////////////////////////////////////////////////
        __UNREGISTER_MESSAGE__( KFMsg::S2S_DEPLOY_COMMAND_TO_CLIENT_REQ );
    }

    void KFDeployClientModule::OnceRun()
    {
        auto agentdata = KFGlobal::Instance()->_startup_params[ __KF_STRING__( agent ) ];
        if ( agentdata.empty() )
        {
            return;
        }

        auto agentid = KFUtility::SplitString( agentdata, "|" );
        auto ip = KFUtility::SplitString( agentdata, "|" );
        auto port = KFUtility::SplitValue< uint32 >( agentdata, "|" );

        auto appid = KFAppId::ToUInt64( agentid );
        _kf_tcp_client->StartClient( __KF_STRING__( deploy ), __KF_STRING__( agent ), appid, ip, port );
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFDeployClientModule::AddFunction( const std::string& command, const std::string& module, KFDeployFunction& function )
    {
        auto kfcommand = _command_data.Create( command );
        auto kffunction = kfcommand->_functions.Create( module );
        kffunction->_function = function;
    }

    void KFDeployClientModule::RemoveFunction( const std::string& command, const std::string& module )
    {
        auto kfcommand = _command_data.Create( command );
        kfcommand->_functions.Remove( module );
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_MESSAGE_FUNCTION__( KFDeployClientModule::HandleDeployCommandToClientReq )
    {
        __PROTO_PARSE__( KFMsg::S2SDeployCommandToClientReq );

        auto pbdeploy = &kfmsg.deploycommand();

        // 部署命令
        DeployCommand( pbdeploy->command(), pbdeploy->value(), pbdeploy->appname(), pbdeploy->apptype(), pbdeploy->appid(), pbdeploy->zoneid() );
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFDeployClientModule::DeployCommand( const std::string& command, const std::string& value,
            const std::string& appname, const std::string& apptype, const std::string& appid, uint32 zoneid )
    {
        // 判断是不是自己
        auto ok = IsSelfServer( appname, apptype, appid, zoneid );
        if ( !ok )
        {
            return;
        }

        if ( command == __KF_STRING__( shutdown ) )
        {
            auto delaytime = KFUtility::ToValue< uint32 >( value );
            return ShutDownServer( appname, apptype, appid, zoneid, delaytime );
        }
        else if ( command == __KF_STRING__( loglevel ) )
        {
            auto level = KFUtility::ToValue< uint32 >( value );
            return KFLogger::Instance()->SetLogLevel( level );
        }
        else if ( command == __KF_STRING__( logmemory ) )
        {
            auto memory = KFUtility::ToValue< uint32 >( value );
            return KFMalloc::Instance()->SetLogOpen( memory == 1 ? true : false );
        }
        else if ( command == __KF_STRING__( loadconfig ) )
        {
            return _kf_config->ReloadConfig( value );
        }
        else if ( command == __KF_STRING__( loadplugin ) )
        {
            auto strcommand = __FORMAT__( "{}={}", command, value );
            return _kf_plugin_manage->AddCommand( strcommand );
        }

        auto kfcommand = _command_data.Find( command );
        if ( kfcommand == nullptr )
        {
            return;
        }

        __LOG_INFO__( "[{}:{} | {}:{}:{}:{}] deploy command process!", command, value, appname, apptype, appid, zoneid );

        for ( auto& iter : kfcommand->_functions._objects )
        {
            auto kffunction = iter.second;
            kffunction->_function( value );
        }
    }

    bool KFDeployClientModule::IsSelfServer( const std::string& appname, const std::string& apptype, const std::string& appid, uint32 zoneid )
    {
        auto kfglobal = KFGlobal::Instance();

        // 指定appid
        if ( appid != _globbing_str )
        {
            return ( appid == kfglobal->_app_id->ToString() );
        }

        // appname
        if ( appname != _globbing_str )
        {
            if ( appname != kfglobal->_app_name )
            {
                return false;
            }
        }

        // apptype
        if ( apptype != _globbing_str )
        {
            if ( apptype != kfglobal->_app_type )
            {
                return false;
            }
        }

        // zoneid
        if ( zoneid != _invalid_int )
        {
            if ( zoneid != kfglobal->_app_id->GetZoneId() )
            {
                return false;
            }
        }

        return true;
    }

    void KFDeployClientModule::ShutDownServer( const std::string& appname, const std::string& apptype, const std::string& appid, uint32 zoneid, uint32 delaytime )
    {
        auto kfglobal = KFGlobal::Instance();
        __LOG_INFO__( "[{}:{}:{}:{}] shutdown start!", kfglobal->_app_name, kfglobal->_app_type, kfglobal->_app_id->ToString(), delaytime );

        // 如果是服务
        if ( appname != __KF_STRING__( zone ) )
        {
            delaytime += 30000;
        }
        else
        {
            if ( apptype == __KF_STRING__( world ) )
            {
                delaytime += 20000;
            }
        }

        // 启动一个定时器
        __REGISTER_DELAY_TIMER__( kfglobal->_app_id->GetId(), delaytime, &KFDeployClientModule::OnTimerShutDownPrepare );
    }

    __KF_TIMER_FUNCTION__( KFDeployClientModule::OnTimerShutDownPrepare )
    {
        __REGISTER_LIMIT_TIMER__( objectid, 10000, 1, &KFDeployClientModule::OnTimerShutDownServer );

        auto kfglobal = KFGlobal::Instance();
        __LOG_INFO__( "[{}:{}:{}] shutdown prepare!", kfglobal->_app_name, kfglobal->_app_type, kfglobal->_app_id->ToString() );

        auto kfcommand = _command_data.Find( __KF_STRING__( shutdown ) );
        if ( kfcommand != nullptr )
        {
            for ( auto& iter : kfcommand->_functions._objects )
            {
                auto kffunction = iter.second;
                kffunction->_function( _invalid_str );
            }
        }
    }

    __KF_TIMER_FUNCTION__( KFDeployClientModule::OnTimerShutDownServer )
    {
        auto kfglobal = KFGlobal::Instance();

        __LOG_INFO__( "[{}:{}:{}] shutdown ok!", kfglobal->_app_name, kfglobal->_app_type, kfglobal->_app_id->ToString() );

        // linux程序正常退出时会core, 应该是每个so文件中的同名全局变量造成的
        // 这里不让程序退出, 由agent等待超时kill
        // kfglobal->_app_run = false;
    }
}