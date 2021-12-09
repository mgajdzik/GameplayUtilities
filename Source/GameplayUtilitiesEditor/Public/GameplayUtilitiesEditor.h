// Copyright Mateusz Gajdzik. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FGameplayUtilitiesEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
