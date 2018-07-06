#ifndef __KF_HTTP_DATA_H__
#define __KF_HTTP_DATA_H__

#include "KFDefine.h"

namespace KFrame
{
    class KFHttp;
    class KFHttpData
    {
    public:
        KFHttpData();
        ~KFHttpData();

        // ִ���߼�
        void Request();

        // ���ʽ���
        void Finish();
    public:
        // ���ʵ�ַ
        std::string _url;

        // ��������
        std::string _data;

        // ��������
        std::string _result;

        // �ص�����
        KFHttpClientFunction _function;

        // httpclient
        KFHttp* _http;
    };
}


#endif