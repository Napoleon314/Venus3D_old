#include <jni.h>
#include <android/log.h>
#include <VePowerPch.h>
#include "android_native_app_glue.h"

VeInt32 main();

void android_main(struct android_app* state)
{
	app_dummy();

	/*VeConsoleInit("VePowerTest");

	AAssetManager* pkManager = state->activity->assetManager;
	AAssetDir* pkDir = AAssetManager_openDir(pkManager, "");
	if(pkDir)
	{
		__android_log_print(ANDROID_LOG_INFO, "Venus3D", "Has asset dir");

		const char* pcTemp = AAssetDir_getNextFileName(pkDir);
		while(pcTemp)
		{
			__android_log_print(ANDROID_LOG_INFO, "Venus3D", "%s", pcTemp);
			pcTemp = AAssetDir_getNextFileName(pkDir);
		}
		AAssetDir_close(pkDir);
	}

	__android_log_print(ANDROID_LOG_INFO, "Venus3D", "%s", state->activity->internalDataPath);
	__android_log_print(ANDROID_LOG_INFO, "Venus3D", "%s", state->activity->externalDataPath);
	__android_log_print(ANDROID_LOG_INFO, "Venus3D", "%d", state->activity->sdkVersion);

	{
		VeFilePathPtr spPath = VE_NEW VeFilePath("/");

		if(spPath->Access(VeDirectory::ACCESS_W_OK))
		{
			VeDebugOutput("Can write");
		}
		else
		{
			VeDebugOutput("Can't write");
		}
	}

	VeConsoleTerm();*/
	SetAssetManager(state->activity->assetManager);
	main();
}
