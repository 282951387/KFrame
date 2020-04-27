﻿#include "KFZoneConfig.hpp"

namespace KFrame
{
    bool KFZoneConfig::LoadConfig( const std::string& filename, const std::string& filepath, uint32 loadmask )
    {
        _zone_list.Clear();
        ////////////////////////////////////////////////////////////////////////////////

        KFXml kfxml( filepath );
        auto config = kfxml.RootNode();

        auto defaultnode = config.FindNode( "Default" );
        _zone_template._name = defaultnode.GetString( "Name" );
        _zone_template._login_zone_id = defaultnode.GetUInt32( "LoginId" );
        _zone_template._data_zone_id = defaultnode.GetUInt32( "DataId" );

        // 本小区属性
        _zone = _zone_template;
        /////////////////////////////////////////////////////////////////////////////////////
        auto zones = config.FindNode( "Zones" );
        _recommend_zone_id = zones.GetUInt32( "Recommend", true );

        auto xmlnode = zones.FindNode( "Zone" );
        while ( xmlnode.IsValid() )
        {
            auto id = xmlnode.GetUInt32( "Id" );
            auto name = xmlnode.GetString( "Name" );
            auto loginid = xmlnode.GetUInt32( "LoginId" );
            auto dataid = xmlnode.GetUInt32( "DataId" );

            // 小区列表
            auto zone = _zone_list.Create( id );
            zone->_id = id;
            zone->_name = name;
            zone->_login_zone_id = loginid;
            zone->_data_zone_id = dataid;

            // 本小区
            if ( KFGlobal::Instance()->_app_id->GetZoneId() == id )
            {
                _zone._id = id;
                _zone._name = name;
                _zone._login_zone_id = loginid;
                _zone._data_zone_id = dataid;
            }

            xmlnode.NextNode();
        }
        /////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////
        SetZoneData( &_zone, KFGlobal::Instance()->_app_id->GetZoneId() );
        /////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////
        return true;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFZoneConfig::SetZoneData( KFZoneSetting* zone, uint32 zoneid )
    {
        // id
        zone->_id = zoneid;

        // 名字
        zone->_name = __FORMAT__( zone->_name, zone->_id );

        // 登录小区id
        if ( zone->_login_zone_id == _invalid_int )
        {
            zone->_login_zone_id = ( uint32 )zone->_id;
        }

        // 数据小区id
        if ( zone->_login_zone_id == _invalid_int )
        {
            zone->_data_zone_id = ( uint32 )zone->_id;
        }
    }

    const KFZoneSetting* KFZoneConfig::ZoneSetting()
    {
        return &_zone;
    }

    const KFZoneSetting* KFZoneConfig::FindSetting( uint32 zoneid )
    {
        auto zone = _zone_list.Create( zoneid );
        if ( zone->_id == _invalid_int )
        {
            *zone = _zone_template;
            SetZoneData( zone, zoneid );
        }

        return zone;
    }
}