#ifndef __KF_ACHIEVE_INTERFACE_H__
#define __KF_ACHIEVE_INTERFACE_H__

#include "KFrame.h"
#include "KFKernel/KFKernelInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    class KFAchieveInterface : public KFModule
    {
    public:
        // �������ս���ɾ�δ�������
        virtual void FormatBattleAchieve( KFData* kfobject, KFMsg::PBTaskDatas* pbachieve ) = 0;
    };
    __KF_INTERFACE__( _kf_achieve, KFAchieveInterface );
}



#endif