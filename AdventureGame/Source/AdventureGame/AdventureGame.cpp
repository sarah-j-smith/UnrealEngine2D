// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdventureGame.h"
#include "Constants.h"
#include "Internationalization/StringTableRegistry.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogAdventureGame);

void FAdventureGame::StartupModule()
{
    FDefaultGameModuleImpl::StartupModule();

    // https://unreal-garden.com/tutorials/stringtable-cpp/
    //
    // Note: sometimes the three lines of macro expanded code below throw an exception
    // when run in the editor. It seems to get caught and continue anyway, but hopefully
    // it won't ever throw in production.  I think the issue is that the translation unit
    // that contains the singleton instance as a static inside a function does not load
    // for some reason. Some kind of race condition. But for now, I'm ignoring it.
    //
    // FStringTableRegistry::Get()
    // .Internal_LocTableFromFile(ITEM_STRINGS_KEY,
    //     FTextKey(ITEMS_NAMESPACE_KEY),
    //     "StringTables/ItemStrings.csv",
    //     FPaths::ProjectContentDir());

    FStringTableRegistry::Get().UnregisterStringTable(ITEM_STRINGS_KEY);
	FStringTableRegistry::Get().UnregisterStringTable(ITEM_DESCRIPTIONS_KEY);
	FStringTableRegistry::Get().UnregisterStringTable(ITEM_LONG_DESCRIPTIONS_KEY);
	FStringTableRegistry::Get().UnregisterStringTable(UI_STRINGS_KEY);
    
    LOCTABLE_FROMFILE_GAME(ITEM_STRINGS_KEY, ITEMS_NAMESPACE_KEY, "StringTables/ItemStrings.csv");
    LOCTABLE_FROMFILE_GAME(ITEM_DESCRIPTIONS_KEY, ITEMS_NAMESPACE_KEY, "StringTables/ItemDescriptions.csv");
    LOCTABLE_FROMFILE_GAME(ITEM_LONG_DESCRIPTIONS_KEY, ITEMS_NAMESPACE_KEY, "StringTables/ItemLongDescriptions.csv");
    LOCTABLE_FROMFILE_GAME(UI_STRINGS_KEY, UI_NAMESPACE_KEY, "StringTables/UIStrings.csv");
}

void FAdventureGame::ShutdownModule()
{
	FStringTableRegistry::Get().UnregisterStringTable(ITEM_STRINGS_KEY);
	FStringTableRegistry::Get().UnregisterStringTable(ITEM_DESCRIPTIONS_KEY);
	FStringTableRegistry::Get().UnregisterStringTable(ITEM_LONG_DESCRIPTIONS_KEY);
	FStringTableRegistry::Get().UnregisterStringTable(UI_STRINGS_KEY);
    FDefaultGameModuleImpl::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE( FAdventureGame, AdventureGame, "AdventureGame" );

