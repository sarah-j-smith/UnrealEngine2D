#pragma once

/// Table of item kind fallback description text. Key name assigned for the
/// Item Descriptions table on load. See AdventureGame.cpp for loading details.
/// Use a plain C macro so that LOCTABLE and other macros will work.
#define ITEM_DESCRIPTIONS_KEY "ItemDescriptions"

#define ITEM_LONG_DESCRIPTIONS_KEY "ItemLongDescriptions"

/// Table of default item behaviour texts. Key name assigned for the Item Strings
/// table on load.  See AdventureGame.cpp for loading details. Use a plain C macro
/// so that LOCTABLE and other macros will work.
#define ITEM_STRINGS_KEY "ItemStrings"

/// Translations namespace for all item and hotspot in-game text
#define ITEMS_NAMESPACE_KEY "ItemsNamespace"

/// Table of strings for messages in the UI.
#define UI_STRINGS_KEY "UIStrings"

/// Translations namespace for all UI text not gameplay related
#define UI_NAMESPACE_KEY "UINamespace"


/// See ItemDescriptions.csv - use these in LOCTABLE
#define G_WALK_TO_KEY "Walk_to"
#define G_VERB_SUBJECT_KEY "Verb_subject"
#define G_USE_SUBJECT_ON_OBJECT_KEY "Use_subject_on_object"
#define G_GIVE_SUBJECT_TO_OBJECT_KEY "Give_subject_to_object"

#define SAVE_GAME_NAME "AdventureGame_SaveGameFile"

#define COL( C ) float(C) / 255.0f

/// Brighter orange D79115FF
constexpr FLinearColor G_UI_Highlight_Colour = FLinearColor( COL(0xD7), COL(0x91), COL(0x15), 1.0f);

/// Dimmed orange 7D652AFF
constexpr FLinearColor G_UI_Dimmed_Colour = FLinearColor( COL(0x7D), COL(0x65), COL(0x2A), 1.0f);

/// Disabled orange 58471BFF
constexpr FLinearColor G_UI_Disabled_Colour = FLinearColor( COL(0x58), COL(0x47), COL(0x1B), 1.0f);

/// Default text color 58471BFF
constexpr FLinearColor G_NPC_Default_Text_Colour = FLinearColor( COL(0x44), COL(0x47), COL(0xEE), 1.0f);

/// Default player bark text color FFFFFFFF
constexpr FLinearColor G_Player_Default_Text_Colour = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

/// Color for UI messages that have been just updated 00952EFF
constexpr FLinearColor G_UI_Status_Color = FLinearColor(COL(0x00), COL(0x95), COL(0x2E), 1.0);

/// Color for UI error messages that have just updated AB1212FF
constexpr FLinearColor G_UI_Error_Color = FLinearColor(COL(0xAB), COL(0x12), COL(0x12), 1.0);

constexpr int32 GMax_Number_Of_Prompts = 5;


#define NEW_LINE_SEPARATOR TEXT("\\n")

#define G_MAX_BARK_LINES 3
#define BARK_LINE_DELAY 3.0f
#define BARK_LINE_WIDTH 30