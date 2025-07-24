// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdventureGame.h"
#include "Constants.h"
#include "Internationalization/StringTableRegistry.h"
#include "PropertyEditorModule.h"
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
    
    LOCTABLE_FROMFILE_GAME(ITEM_STRINGS_KEY, ITEMS_NAMESPACE_KEY, "StringTables/ItemStrings.csv");
    LOCTABLE_FROMFILE_GAME(ITEM_DESCRIPTIONS_KEY, ITEMS_NAMESPACE_KEY, "StringTables/ItemDescriptions.csv");
    LOCTABLE_FROMFILE_GAME(ITEM_LONG_DESCRIPTIONS_KEY, ITEMS_NAMESPACE_KEY, "StringTables/ItemLongDescriptions.csv");
}

void FAdventureGame::ShutdownModule()
{
    if(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        auto& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomClassLayout("AdventurePlayerController");
    }
    FDefaultGameModuleImpl::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE( FAdventureGame, AdventureGame, "AdventureGame" );

