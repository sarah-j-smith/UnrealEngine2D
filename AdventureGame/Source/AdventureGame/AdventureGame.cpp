// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdventureGame.h"
#include "Constants.h"
#include "Internationalization/StringTableRegistry.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogAdventureGame);

void FAdventureGame::StartupModule()
{
    FDefaultGameModuleImpl::StartupModule();

    LOCTABLE_FROMFILE_GAME(ITEM_STRINGS_KEY, "ItemsNamespace", ITEM_STRINGS_CSV_PATH);
    LOCTABLE_FROMFILE_GAME(ITEM_DESCRIPTIONS_KEY, "ItemsNamespace", ITEM_DESCRIPTIONS_CSV_PATH);
}

void FAdventureGame::ShutdownModule()
{
    FDefaultGameModuleImpl::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE( FAdventureGame, AdventureGame, "AdventureGame" );

