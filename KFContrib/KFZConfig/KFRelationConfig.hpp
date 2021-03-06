﻿#ifndef __KF_RELATION_CONFIG_H__
#define __KF_RELATION_CONFIG_H__

#include "KFZConfig/KFConfig.h"
#include "KFZConfig/KFSetting.h"

namespace KFrame
{
    class KFRelationSetting : public KFStrSetting
    {
    public:
        // 最大数量
        uint32 _max_count = 0u;

        // 是否需要更新
        bool _need_update = false;

        // 是否是双向的
        bool _both_way = false;

        // 是否上线加载
        bool _online_load = false;

        // 邀请列表名字
        std::string _invite_data_name;

        // 邀请列表数量
        uint32 _invite_data_count = 0u;

        // 邀请数据库列表名
        std::string _invite_list_name;

        // 数据库列表名
        std::string _relation_list_name;

        // 邀请保留时间( 秒 )
        uint32 _invite_keep_time = 0u;

        // 拒绝申请名字
        std::string _refuse_name;
    };
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    class KFRelationConfig : public KFConfigT< KFRelationSetting >, public KFInstance< KFRelationConfig >
    {
    public:
        KFRelationConfig()
        {
            _file_name = "relation";
        }

        // 查找关系配置
        const KFRelationSetting* FindRelationSettingByInviteName( const std::string& name );

    protected:
        // 读取配置
        virtual void ReadSetting( KFNode& xmlnode, KFRelationSetting* kfsetting );
    };
}

#endif