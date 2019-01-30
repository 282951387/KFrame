#ifndef __KF_FILE_H__
#define __KF_FILE_H__

#include "KFSingleton.h"
#include "KFMap.h"

namespace KFrame
{
	class KFAttribute
	{
	public:
		std::string _name;
		std::string _type;
		std::string _comment;
	};

	class KFClass
	{
	public:
		KFAttribute* GetAttribute( uint32 index )
		{
			auto iter = _attributes.find( index );
			if ( iter == _attributes.end() )
			{
				return nullptr;
			}
			
			return &iter->second;
		}

		void AddAttribute( uint32 index, KFAttribute& attribute )
		{
			_attributes[ index ] = attribute;
		}

	public:
		// ����
		std::string _class_name;

		// �����б�
		std::map< uint32, KFAttribute > _attributes;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////
	class KFData
	{
	public:
		void AddData( uint32 index, const std::string& value )
		{
			_datas[ index ] = value;
		}

	public:
		std::map< uint32, std::string > _datas;
	};
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////
	class KFFile
	{
	public:
		KFFile()
		{
			_is_server = false;
			_is_client = false;
		}

		// �����������
		bool AddData( KFData& data );
		bool AddData( uint32 index, const std::string& value );

	public:
		// ����ͻ���
		bool _is_server;

		// ���������
		bool _is_client;

		// �ඨ��
		KFClass _class;

		// �б�
		std::vector< KFData > _datas;
	};
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

}
#endif
