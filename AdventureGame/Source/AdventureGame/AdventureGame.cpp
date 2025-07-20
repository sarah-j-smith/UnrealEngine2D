// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdventureGame.h"
#include "Constants.h"
#include "InventoryCustomisation.h"
#include "Internationalization/StringTableRegistry.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogAdventureGame);

void FAdventureGame::StartupModule()
{
    FDefaultGameModuleImpl::StartupModule();

    // // Load a custom view for the Player Controller that shows the inventory
    // FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    // PropertyModule.RegisterCustomClassLayout("AdventurePlayerController", 
    //     FOnGetDetailCustomizationInstance::CreateStatic(&FInventoryCustomisation::MakeInstance));
    
    // https://unreal-garden.com/tutorials/stringtable-cpp/
    LOCTABLE_FROMFILE_GAME(ITEM_STRINGS_KEY, ITEMS_NAMESPACE_KEY, "StringTables/ItemStrings.csv");
    LOCTABLE_FROMFILE_GAME(ITEM_DESCRIPTIONS_KEY, ITEMS_NAMESPACE_KEY, "StringTables/ItemDescriptions.csv");
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

