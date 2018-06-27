#ifndef __KF_HTTP_SERVER_H__
#define __KF_HTTP_SERVER_H__

#include "KFrame.h"
#include "KFThread/KFMutex.h"

namespace Poco
{
	namespace Net
	{
		class HTTPServer;
	}
}

namespace KFrame
{
	__ST_CLASS__( KFHttpFuncton )
	{
	public:
		// http����
		KFHttpMethodFunction _function;

		// �Ƿ�ͬ��( �����߼��߳�ִ�� )
		bool _sync;
	};

	__MT_CLASS__( KFFunctionData )
	{
	public:
		// ִ�к���
		KFHttpFuncton * _kf_function;

		// ip
		std::string _ip;

		// ����
		std::string _data;
	};

	class KFHttpServer
	{
	public:

		KFHttpServer();
		~KFHttpServer();

		////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////
		// ����
		void Start( const std::string& ip, uint32 port, uint32 maxthread, uint32 maxqueue, uint32 idletime, bool keeplive );

		// �߼�
		void Run();

		// �ر�
		void ShutDown();
		///////////////////////////////////////////////////////////////////
		// ע�ᴴ��httphandle
		void RegisterMethonFunction( const std::string& url, bool sync, KFHttpMethodFunction& function );

		// ж��
		void UnRegisterMethonFunction( const std::string& url );

		// ����http����
		std::string ProcessHttpRequest( const std::string& url, const std::string& ip, const std::string& data );

	private:
		Poco::Net::HTTPServer* _http_server;

		// �����б�
		KFMap< std::string, const std::string&, KFHttpFuncton > _functions;

		// �߳���
		KFMutex _kf_mutex;

		// ��������
		std::list< KFFunctionData* > _kf_function_data;
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

}


#endif