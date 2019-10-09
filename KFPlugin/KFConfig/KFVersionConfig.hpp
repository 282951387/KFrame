﻿#ifndef __KF_VERSION_CONFIG_H__
#define __KF_VERSION_CONFIG_H__

#include "KFZConfig/KFConfig.h"

namespace KFrame
{
    class KFVersionSetting : public KFStrSetting
    {
    public:
        // 版本号
        std::string _version;
    };
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    class KFVersionConfig : public KFConfigT< KFVersionSetting >
    {
    protected:
        // 读取配置
        virtual void ReadSetting( KFNode& xmlnode, KFVersionSetting* kfsetting );

        // 创建
        virtual KFVersionSetting* CreateSetting( KFNode& xmlnode );

    };
}

#endif