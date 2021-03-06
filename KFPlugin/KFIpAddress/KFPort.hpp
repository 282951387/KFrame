#ifndef __KF_NET_PORT_H__
#define __KF_NET_PORT_H__

#include "KFrame.h"

namespace KFrame
{
    class KFPort
    {
    public:
        KFPort()
        {
            _id = 0;
            _port = 0;
        }

    public:
        uint64 _id;
        uint32 _port;
    };

#define __MAX_PORT_COUNT__ 200
    class KFramePort
    {
    public:
        KFramePort()
        {
            _is_inited = 0;
        }

        // 查找端口
        KFPort* FindPort( uint64 id );

    public:
        // 是否初始化
        uint32 _is_inited;

        // 互斥量
        KFMutex _kf_mutex;

        // 端口列表
        KFPort _kf_port[ __MAX_PORT_COUNT__ ];
    };

#define __FIX_PORT__ 0	// 固定计算端口
#define __ID_PORT__ 1	// id计算端口
#define __SH_PORT__ 2	// 共享内存计算端口

}
#endif

