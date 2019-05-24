﻿#ifndef __KF_ITEM_CONFIG_H__
#define __KF_ITEM_CONFIG_H__

#include "KFItemSetting.hpp"
#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    class KFItemConfig : public KFConfig, public KFSingleton< KFItemConfig >
    {
    public:
        KFItemConfig() = default;
        ~KFItemConfig() = default;

        // 获得成就配置
        const KFItemSetting* FindItemSetting( uint32 itemid ) const;

        // 读取配置
        bool LoadConfig( const std::string& file );

    private:
        // 物品列表
        KFHashMap< uint32, uint32, KFItemSetting > _item_setting;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static auto _kf_item_config = KFItemConfig::Instance();
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif