#ifndef __KF_HTTP_H__
#define __KF_HTTP_H__

#include "KFrame.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
#include "Poco/ThreadPool.h"
#include "Poco/Exception.h"

namespace KFrame
{
    class KFHttp
    {
    public:
        KFHttp() = default;
        virtual ~KFHttp() = default;

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

