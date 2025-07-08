// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdventureGame.h"
#include "Constants.h"
#include "Internationalization/StringTableRegistry.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogAdventureGame);

void FAdventureGame::StartupModule()
{
    FDefaultGameModuleImpl::StartupModule();

    LOCTABLE_FROMFILE_GAME(ITEM_STRINGS_KEY, "ItemsNamespace", "Localization/ItemStrings.csv");
    LOCTABLE_FROMFILE_GAME(ITEM_DESCRIPTIONS_KEY, "ItemsNamespace", "Localization/ItemDescriptions.csv");
}

void FAdventureGame::ShutdownModule()
{
    FDefaultGameModuleImpl::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE( FAdventureGame, FAdventureGame, "AdventureGame" );

