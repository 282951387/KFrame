﻿#include "KFConfigModule.hpp"

namespace KFrame
{
    static std::string _config_path = "./config/";
    //////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////
    KFConfigModule::KFConfigModule()
    {
        _kf_config_config = new KFConfigConfig();
    }

    KFConfigModule::~KFConfigModule()
    {
        delete _kf_config_config;
        _kf_config_config = nullptr;
    }

    void KFConfigModule::LoadConfig()
    {
        // 先读取
        auto configfile = _config_path + "config.xml";
        LoadConfigFile( _kf_config_config, configfile, KFConfigEnum::None );

        for ( auto& iter : _config_list )
        {
            auto kfconfig = iter.second;
            auto kfsetting = _kf_config_config->FindSetting( kfconfig->_file_name );
            if ( kfsetting == nullptr )
            {
                __LOG_ERROR__( "config=[{}:{}] can't find setting!", iter.first, kfconfig->_file_name );
                continue;
            }

            for ( auto& file : kfsetting->_file_list )
            {
                LoadConfigFile( kfconfig, file, kfsetting->_load_mask );
            }
        }
    }

    void KFConfigModule::ShutDown()
    {
        for ( auto& iter : _config_list )
        {
            delete iter.second;
        }
        _config_list.clear();
    }

    void KFConfigModule::AddConfig( const std::string& name, KFConfig* kfconfig )
    {
        _config_list[ name ] = kfconfig;
    }

    KFConfig* KFConfigModule::FindConfig( const std::string& name )
    {
        auto iter = _config_list.find( name );
        if ( iter == _config_list.end() )
        {
            return nullptr;
        }

        return iter->second;
    }

    void KFConfigModule::ReloadConfig( const std::string& file )
    {
        __LOG_INFO__( "reload [{}] start!", file );

        for ( auto& iter : _config_list )
        {
            auto kfconfig = iter.second;
            auto kfsetting = _kf_config_config->FindSetting( kfconfig->_file_name );
            if ( kfsetting == nullptr || !KFUtility::HaveBitMask<uint32>( kfsetting->_load_mask, KFConfigEnum::CanReload ) )
            {
                continue;
            }

            // 判断是否指定文件
            if ( file != _globbing_str )
            {
                auto configfile = kfsetting->IsFile( file );
                if ( configfile.empty() )
                {
                    continue;
                }
            }

            for ( auto& file : kfsetting->_file_list )
            {
                LoadConfigFile( kfconfig, file, kfsetting->_load_mask );
            }
        }

        __LOG_INFO__( "reload [{}] ok!", file );
    }

    void KFConfigModule::LoadConfigFile( KFConfig* config, const std::string& file, uint32 loadmask )
    {
        __LOG_INFO__( "load [{}] start!", file );
        try
        {
            config->LoadConfig( file, loadmask );
            config->LoadComplete();
            __LOG_INFO__( "load [{}] ok!", file );
        }
        catch ( ... )
        {
            __LOG_ERROR__( "load [{}] failed!", file );
        }
    }
}