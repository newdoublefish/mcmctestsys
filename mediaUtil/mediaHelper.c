#include <windows.h>
#include <mmsystem.h>
#include "settingConfig.h"
void soundSuccess()
{
	SETTING s=GetSetting();
	if(s.voice>0)	
		PlaySound("resource\\music\\success.wav",NULL,SND_FILENAME|SND_SYNC);
}

void soundError()
{
	SETTING s=GetSetting();
	if(s.voice>0)
		PlaySound("resource\\music\\error.wav",NULL,SND_FILENAME|SND_SYNC);
}

void soundFinish()
{
	SETTING s=GetSetting();
	if(s.voice>0)		
		PlaySound("finish.wav",NULL,SND_FILENAME|SND_SYNC);
}
