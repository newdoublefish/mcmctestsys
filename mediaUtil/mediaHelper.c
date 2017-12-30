#include <windows.h>
#include <mmsystem.h>
void soundSuccess()
{
	PlaySound("success.wav",NULL,SND_FILENAME|SND_SYNC);
}

void soundError()
{
	PlaySound("error.wav",NULL,SND_FILENAME|SND_SYNC);
}

void soundFinish()
{
	PlaySound("finish.wav",NULL,SND_FILENAME|SND_SYNC);
}
