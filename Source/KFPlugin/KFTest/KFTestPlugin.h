#ifndef __KF_TEST_PLUGIN_H__
#define __KF_TEST_PLUGIN_H__

#include "KFPlugin/KFPlugin.h"

namespace KFrame
{
	class KFTestPlugin : public KFPlugin
	{
	public:
		// ע��
		virtual void Install();

		// ж��
		virtual void UnInstall();

		// ����
		virtual void LoadModule();
	};
}


#endif