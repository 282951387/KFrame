#ifndef __KF_HTTP_H__
#define __KF_HTTP_H__

#include "KFrame.h"

namespace KFrame
{
	class KFHttp
	{
	public:
		KFHttp();
		virtual ~KFHttp();
		
		// ִ��http����
		virtual std::string RunHttp( const std::string& url, const std::string& data );

	protected:
		// ��÷���·��
		virtual std::string GetURI( const std::string& url, Poco::URI& pocouri );

		// �������
		virtual std::string HttpRequest( Poco::Net::HTTPRequest& request, Poco::URI& url, const std::string& data );

		// ���clientsession
		virtual Poco::Net::HTTPClientSession* GetHttpSession() = 0;
		//////////////////////////////////////////////////////////////////////////////////////
	};

}
#endif 

