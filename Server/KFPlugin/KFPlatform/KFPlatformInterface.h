#ifndef __KF_PLATFORM_INTERFACE_H__
#define __KF_PLATFORM_INTERFACE_H__

#include "KFrame.h"

namespace KFrame
{
    class KFPlatformInterface : public KFModule
    {
    public:
        // ���api��ַ
        virtual const std::string& GetPlatformApiUrl() = 0;

        // ����ǩ��
        virtual const std::string& MakePlatformSign( uint32 nowtime ) = 0;

        // ����ƽ̨����
        virtual const std::string& MakePlatformUrl( const std::string& path ) = 0;
    };

    /////////////////////////////////////////////////////////////////////////
    __KF_INTERFACE__( _kf_platform, KFPlatformInterface );
    /////////////////////////////////////////////////////////////////////////
}

#endif