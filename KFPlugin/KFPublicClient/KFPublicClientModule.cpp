﻿#include "KFPublicClientModule.hpp"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    void KFPublicClientModule::BeforeRun()
    {
        _kf_component = _kf_kernel->FindComponent( __KF_STRING__( player ) );
        __REGISTER_UPDATE_DATA__( &KFPublicClientModule::OnUpdateDataCallBack );
        __REGISTER_UPDATE_STRING__( &KFPublicClientModule::OnUpdateStringCallBack );

        __REGISTER_ENTER_PLAYER__( &KFPublicClientModule::OnEnterUpdatePublicData );
        __REGISTER_LEAVE_PLAYER__( &KFPublicClientModule::OnLeaveUpdatePublicData );

        _kf_basic = _kf_kernel->CreateObject( __KF_STRING__( basic ) );
        _kf_route->RegisterConnectionFunction( this, &KFPublicClientModule::OnRouteConnectCluster );
        ///////////////////////////////////////////////////////////////////////////////////////////////
        __REGISTER_MESSAGE__( KFMsg::MSG_QUERY_BASIC_REQ, &KFPublicClientModule::HandleQueryBasicReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_QUERY_BASIC_TO_GAME_ACK, &KFPublicClientModule::HandleQueryBasicToGameAck );
    }

    void KFPublicClientModule::BeforeShut()
    {
        __UN_UPDATE_DATA__();
        __UN_UPDATE_STRING__();
        __UN_ENTER_PLAYER__();
        __UN_LEAVE_PLAYER__();

        _kf_kernel->ReleaseObject( _kf_basic );
        _kf_route->UnRegisterConnectionFunction( this );
        ///////////////////////////////////////////////////////////////////////////////////////////////
        __UN_MESSAGE__( KFMsg::MSG_QUERY_BASIC_REQ );
        __UN_MESSAGE__( KFMsg::S2S_QUERY_BASIC_TO_GAME_ACK );
    }

    void KFPublicClientModule::OnRouteConnectCluster( uint64 serverid )
    {
        if ( !_need_refresh_online )
        {
            return;
        }

        _need_refresh_online = false;

        // 刷新在线逻辑
        KFMsg::S2SClearOnlineToPublicReq req;
        req.set_serverid( KFGlobal::Instance()->_app_id->GetId() );
        _kf_route->SendToRand( __KF_STRING__( public ), KFMsg::S2S_CLEAR_ONLINE_TO_PUBLIC_REQ, &req, true );
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFPublicClientModule::UpdatePublicData( KFEntity* player, MapString& values )
    {
        UpdatePublicData( player->GetKeyID(), values );
    }

    void KFPublicClientModule::UpdatePublicData( uint64 playerid, MapString& values )
    {
        KFMsg::S2SUpdateDataToPublicReq req;
        req.mutable_pbdata()->insert( values.begin(), values.end() );
        _kf_route->SendToRand( __KF_STRING__( public ), KFMsg::S2S_UPDATE_DATA_TO_PUBLIC_REQ, &req, true );
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFPublicClientModule::OnUpdateDataToPublic( KFEntity* player, KFData* kfdata )
    {
        // 只有公共属性才更新
        if ( !player->IsInited() ||
                !kfdata->HaveMask( KFDataDefine::Mask_Public ) ||
                !kfdata->GetParent()->HaveMask( KFDataDefine::Mask_Public ) )
        {
            return;
        }

        // 更新到公共属性集群
        MapString values;
        values[ kfdata->_data_setting->_name ] = kfdata->ToString();
        UpdatePublicData( player, values );
    }

    __KF_UPDATE_DATA_FUNCTION__( KFPublicClientModule::OnUpdateDataCallBack )
    {
        OnUpdateDataToPublic( player, kfdata );
    }

    __KF_UPDATE_STRING_FUNCTION__( KFPublicClientModule::OnUpdateStringCallBack )
    {
        OnUpdateDataToPublic( player, kfdata );
    }

    __KF_ENTER_PLAYER_FUNCTION__( KFPublicClientModule::OnEnterUpdatePublicData )
    {
        auto kfglobal = KFGlobal::Instance();

        MapString values;
        values[ __KF_STRING__( id ) ] = __TO_STRING__( player->GetKeyID() );
        values[ __KF_STRING__( status ) ] = __TO_STRING__( KFMsg::FrameOnlineStatus );
        values[ __KF_STRING__( statustime ) ] = __TO_STRING__( kfglobal->_real_time );
        values[ __KF_STRING__( serverid ) ] = __TO_STRING__( kfglobal->_app_id->GetId() );
        UpdatePublicData( player, values );
    }

    __KF_LEAVE_PLAYER_FUNCTION__( KFPublicClientModule::OnLeaveUpdatePublicData )
    {
        MapString values;
        values[ __KF_STRING__( serverid ) ] = "0";
        values[ __KF_STRING__( status ) ] = __TO_STRING__( KFMsg::FrameOfflineStatus );
        values[ __KF_STRING__( statustime ) ] = __TO_STRING__( KFGlobal::Instance()->_real_time );
        UpdatePublicData( player, values );
    }

    __KF_MESSAGE_FUNCTION__( KFPublicClientModule::HandleQueryBasicReq )
    {
        __CLIENT_PROTO_PARSE__( KFMsg::MsgQueryBasicReq );

        // 屏蔽字检查
        auto filter = _kf_filter->CheckFilter( kfmsg.name() );
        if ( filter )
        {
            return _kf_display->SendToClient( player, KFMsg::NameFilterError );
        }

        // 发送到public
        KFMsg::S2SQueryBasicToPublicReq req;
        req.set_name( kfmsg.name() );
        _kf_route->SendToRand( playerid, __KF_STRING__( public ), KFMsg::S2S_QUERY_BASIC_TO_PUBLIC_REQ, &req, false );
    }

    __KF_MESSAGE_FUNCTION__( KFPublicClientModule::HandleQueryBasicToGameAck )
    {
        __ROUTE_PROTO_PARSE__( KFMsg::S2SQueryBasicToGameAck );

        if ( kfmsg.result() != KFMsg::Ok )
        {
            _kf_display->SendToClient( player, kfmsg.result(), kfmsg.name() );
        }

        _kf_basic->Reset();
        for ( auto iter = kfmsg.pbdata().begin(); iter != kfmsg.pbdata().end(); ++iter )
        {
            auto kfdata = _kf_basic->FindData( iter->first );
            if ( kfdata != nullptr )
            {
                kfdata->SetValue< std::string >( iter->second );
            }
        }

        auto pbplayerdata = _kf_kernel->SerializeToView( _kf_basic );

        // 发送给客户端
        KFMsg::MsgQueryBasicAck ack;
        ack.mutable_player()->CopyFrom( *pbplayerdata );
        _kf_player->SendToClient( player, KFMsg::MSG_QUERY_BASIC_ACK, &ack );
    }
}
