#ifndef __KF_MEMORY_MANAGE_H__
#define __KF_MEMORY_MANAGE_H__

#include "KFInclude.h"

namespace KFrame
{
	class KFMutex;
	class BlockStore;
	class KFLogMemory;
	////////////////////////////////////////////////////////////////
	class KFMemory
	{
	public:
		KFMemory();
		~KFMemory();

		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////
		void* Create( const char* name, uint32 size, uint32 batch, const char* function, uint32 line );
		void Delete( void* object, const char* function, uint32 line );
		////////////////////////////////////////////////////////////////////////////////
		void* Malloc( uint32 size, uint32 batch, const char* function, uint32 line );
		void Free( void* memory, const char* function, uint32 line );
		////////////////////////////////////////////////////////////////////////////////

		// ������־����
		void SetLogOpen( bool open );

		// ��ӡ�ڴ���Ϣ
		void PrintLogMemory();

	protected:
		// �����ڴ������
		BlockStore* FindObjectBlock( void* object );
		BlockStore* CreateObjectBlock( const char* name, uint32 size, uint32 batch );

		BlockStore* FindMemoryBlock( void* memory );
		BlockStore* CreateMemoryBlock( const char* name, uint32 size, uint32 batch );
		////////////////////////////////////////////////////////////////////////////
		// ��ӡ�ڴ���Ϣ
		void PrintLogMemory( const char* type, uint32 count, uint64 usesize, uint64 totalsize );

	private:
		// �ڴ���־
		KFLogMemory * _kf_log_memory;

		// ���̵߳Ķ��������
		KFMutex* _kf_object_mutex;
		std::map< std::string, BlockStore* > _mt_object;

		KFMutex* _kf_object_block_mutex;
		std::map< void*, BlockStore* > _mt_object_block;

		// ���̵߳��ڴ������
		KFMutex* _kf_memory_mutex;
		std::map< uint32, BlockStore* > _mt_memory;

		KFMutex* _kf_memory_block_mutex;
		std::map< void*, BlockStore* > _mt_memory_block;
	};
}



#endif