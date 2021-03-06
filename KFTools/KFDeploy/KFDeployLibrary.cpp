﻿#include "KFDeployLibrary.h"

#if __KF_SYSTEM__ == __KF_WIN__
    #include "windows.h"
#else
    #include <dlfcn.h>
    #include <errno.h>
#endif

namespace KFrame
{
    KFDeployLibrary::KFDeployLibrary()
    {
        _instance = nullptr;
    }

    KFDeployLibrary::~KFDeployLibrary()
    {
        UnLoad();
    }

    bool KFDeployLibrary::Load( const std::string& path, const std::string& name )
    {
#if __KF_SYSTEM__ == __KF_WIN__
        _path = path + name + ".dll";
        _instance = LoadLibraryEx( _path.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
        if ( _instance == nullptr )
        {
            __LOG_ERROR__( "load dll - {}", GetLastError() );
        }
#else
        _path = path + name + ".so";
        _instance = dlopen( _path.c_str(), RTLD_GLOBAL | RTLD_LAZY );
        if ( _instance == nullptr )
        {
            __LOG_ERROR__( "dlopen - {}", dlerror() );
        }
#endif

        return _instance != nullptr;
    }

    void KFDeployLibrary::UnLoad()
    {
        if ( _instance == nullptr )
        {
            return;
        }

#if __KF_SYSTEM__ == __KF_WIN__
        FreeLibrary( ( HMODULE )_instance );
#else
        dlclose( _instance );
#endif

        _instance = nullptr;
    }

    void* KFDeployLibrary::GetFunction( const char* function )
    {
#if __KF_SYSTEM__ == __KF_WIN__
        return GetProcAddress( ( HMODULE )_instance, function );
#else
        return dlsym( _instance, function );
#endif
    }
}
