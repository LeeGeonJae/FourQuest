#include "FQMonsterModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FDefaultGameModuleImpl, FQMonster);

void FFQMonster::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("FQMonster StartupModule"));
}

void FFQMonster::ShutdownModule()
{
}
