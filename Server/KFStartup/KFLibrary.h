#ifndef __KF_LIBRARY_H__
#define __KF_LIBRARY_H__

#include "KFrame.h"

namespace KFrame
{
    class KFLibrary
    {
    public:
        KFLibrary();
        ~KFLibrary();

        // ���ز��
        bool Load( const std::string& path, const std::string& name );

        // ж�ز��
        void UnLoad();

        // ��ú�����ַ
        void* GetFunction( const char* function );

    protected:
        // ������
        void* _instance;

    public:
        // ���·��
        std::string _path;
    };
}

#endif
