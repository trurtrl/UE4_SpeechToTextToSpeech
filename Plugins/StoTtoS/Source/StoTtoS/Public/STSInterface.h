#pragma once


#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AndroidGateway.h"


class FSTSInterface : public IModuleInterface
{
public:

	static inline FSTSInterface& Get()
	{
		return FModuleManager::GetModuleChecked<FSTSInterface>("StoTtoS");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("StoTtoS");
	}

};