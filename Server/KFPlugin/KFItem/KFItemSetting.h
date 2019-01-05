﻿#ifndef __KF_ITEM_SETTING_H__
#define __KF_ITEM_SETTING_H__

#include "KFrame.h"
#include "KFCore/KFElement.h"

namespace KFrame
{
    namespace KFItemEnum
    {
        enum EConstDefine
        {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////
            // 物品类型
            None = 0,		// 无类型
            Consume = 101,	// 普通道具
            Script = 102,	// 脚本
            ///////////////////////////////////////////////////////////////////////////////////////////////////////

            // 消耗品类型
            ConsumeExp = 1,	// 经验消耗品

            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////
            ConfigReward = 1,	// 使用配置奖励
            LuaReward = 2,		// 使用lua奖励
            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////
            // 脚本调用类型
            AddFunction = 1,
            UseFunction = 2,
            RemoveFunction = 3,
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////
        };
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    class KFItemSetting
    {
    public:
        KFItemSetting();

        // 判断是否能使用
        bool CheckCanUse() const;

        // 获得lua函数
        const std::string& GetFunction( uint32 type ) const;

        // 判断是否真正的物品
        bool IsRealItem() const;

    public:
        // 物品id
        uint32 _id;

        // 物品类型
        uint32 _type;

        // 可叠加类型
        uint32 _overlay_type;

        // 可叠加数量
        uint32 _overlay_count;

        // 1 = 获得物品是开始计时 2 = 使用物品是开始计时
        uint32 _time_type;

        // 物品有效时间( 单位为秒 )
        uint32 _valid_time;

        // 可使用次数( 0 = 不能使用 )
        uint32 _use_count;

        // 删除物品发送的邮件id
        uint32 _mail_id;

        // 品质
        uint32 _quality;

        // lua文件
        std::string _lua_file;

        // lua函数
        std::unordered_map< uint32, std::string > _function;

        // 奖励类型
        uint32 _reward_type;

        // 奖励列表
        KFElements _rewards;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

}


#endif