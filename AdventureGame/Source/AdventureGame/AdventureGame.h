// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAdventureGame, Verbose, All);

class FAdventureGame : public FDefaultGameModuleImpl
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
