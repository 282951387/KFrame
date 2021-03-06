﻿#include "KFSignInModule.hpp"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    void KFSignInModule::BeforeRun()
    {
        __REGISTER_MESSAGE__( KFMsg::MSG_SEVEN_SIGNIN_REWARD_REQ, &KFSignInModule::HandleReceiveSevenRewardReq );
    }

    void KFSignInModule::ShutDown()
    {
        __UN_MESSAGE__( KFMsg::MSG_SEVEN_SIGNIN_REWARD_REQ );
    }

    __KF_MESSAGE_FUNCTION__( KFSignInModule::HandleReceiveSevenRewardReq )
    {
        __CLIENT_PROTO_PARSE__( KFMsg::MsgSevenSignInRewardReq );

        auto day = player->Get< uint32 >( __STRING__( sevenday ) );
        if ( day < kfmsg.day() )
        {
            return _kf_display->SendToClient( player, KFMsg::SignInNotDay );
        }

        auto kfsetting = KFSignInConfig::Instance()->FindSetting( kfmsg.day() );
        if ( kfsetting == nullptr )
        {
            return _kf_display->SendToClient( player, KFMsg::SignInCanNotFind );
        }

        auto sevenflag = player->Get< uint32 >( __STRING__( sevenreward ) );
        auto flag = 1u << kfmsg.day();
        if ( KFUtility::HaveBitMask< uint32 >( sevenflag, flag ) )
        {
            return _kf_display->SendToClient( player, KFMsg::SignInRewardAlready );
        }

        // 设置标记
        player->UpdateData( __STRING__( sevenreward ), KFEnum::Or, kfmsg.day() );

        // 添加奖励
        player->AddElement( &kfsetting->_reward, _default_multiple, __STRING__( signin ), kfmsg.day(), __FUNC_LINE__ );

        // 额外的奖励
        if ( KFGlobal::Instance()->RandCheck( KFRandEnum::TenThousand, kfsetting->_probability ) )
        {
            player->AddElement( &kfsetting->_extend, _default_multiple, __STRING__( signin ), kfmsg.day(), __FUNC_LINE__ );
        }

        _kf_display->SendToClient( player, KFMsg::SignInRewardOk );
    }
}