﻿#include "KFHttpsClient.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPSClientSession.h"

namespace KFrame
{
    using namespace Poco::Net;
    static Poco::Net::Context::Ptr _context = nullptr;

    KFHttpsClient::KFHttpsClient()
    {
        _https_session = __NEW_OBJECT__( Poco::Net::HTTPSClientSession, _context );
    }

    KFHttpsClient::~KFHttpsClient()
    {
        __DELETE_OBJECT__( _https_session );
    }

    void KFHttpsClient::Initialize()
    {
        Poco::Net::initializeSSL();
#if __KF_SYSTEM__ == __KF_WIN__
        _context = __NEW_OBJECT__( Context, Context::CLIENT_USE, "" );
#else
        _context = __NEW_OBJECT__( Context, Context::CLIENT_USE, "", Context::VERIFY_RELAXED, 9, true );
        _context->disableProtocols( Context::PROTO_SSLV2 | Context::PROTO_SSLV3 );
#endif

        Poco::SharedPtr<InvalidCertificateHandler> pCert = __NEW_OBJECT__( AcceptCertificateHandler, false );
        Poco::Net::SSLManager::instance().initializeClient( 0, pCert, _context );
    }


    Poco::Net::HTTPClientSession* KFHttpsClient::GetHttpSession()
    {
        return _https_session;
    }

}

