#ifndef __KF_PARSE_H__
#define __KF_PARSE_H__

#include "KFInterfaces.h"
#include "KFExcel/KFExcelFile.h"
#include "KFFile.h"

namespace KFrame
{
	class KFParse : public KFSingleton< KFParse >
	{
	public:
		KFParse();
		~KFParse();

		bool LoadFromExcel( const char* file, bool utf8 );
		bool SaveToHpp( const char* file );
		bool SaveToCpp( const char* file );

		bool SaveToXml( const char* path );
		bool SaveToCSV( const char* path );


		// ���浽c#�ļ�
		bool SaveToCSharp( const char* path );

		//��ÿ����д��ĸǰ���»��ߣ��Ѵ�д��ĸתΪСд��ĸ
		std::string changeString( std::string& oldstring );

	private:
		bool LoadFromExcel( KFExcelSheet* sheet );

		KFFile* AddFile( std::string& name );

		bool SaveToHpp( const std::string& path, KFFile* kffile );
		bool SaveToCpp( const std::string& path, KFFile* kffile );

		bool SaveToXml( const std::string& path, KFFile* kffile );
		bool SaveToCSV( const std::string& path, KFFile* kffile );

		bool SaveToCSharp( const std::string& path, KFFile* kffile );

	private:
		bool _utf8 = false;

		// �ļ�
		std::map< std::string, KFFile > _files;
	};

	///////////////////////////////////////////////////////////////////////////
	static auto _kf_parse = KFParse::Instance();
	///////////////////////////////////////////////////////////////////////////
}
#endif
