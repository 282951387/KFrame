#ifndef __KF_TEST_MODULE_H__
#define __KF_TEST_MODULE_H__

/************************************************************************
//    @Moudle			:    ����ģ��
//    @Author           :    __��_��__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-4-13
************************************************************************/

#include "KFrame.h"
#include "KFTestInterface.h"

namespace KFrame
{
	class KFTestModule : public KFTestInterface
	{
	public:
		KFTestModule();
		~KFTestModule();
		
		// ��������
		virtual void InitModule();

		// �߼�
		virtual void BeforeRun();
		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////

	protected:
		// ���������Ϣ
		void HandleProtoMessage( uint64 id, const char* data, uint32 length );
		void HandleBufferMessage( uint64 id, const char* data, uint32 length );

		// ���Ͳ�����Ϣ
		void ProcessClientSendTestMessage( uint64 serverid );
		void ProcessServerSendTestMessage( uint64 id, uint32 type, const std::string& ip, uint32 port );

	};
}



#endif