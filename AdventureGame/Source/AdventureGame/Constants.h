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

#define ITEMS_NAMESPACE_KEY "ItemsNamespace"

/// See ItemDescriptions.csv - use these in LOCTABLE
#define G_WALK_TO_KEY "Walk_to"
#define G_VERB_SUBJECT_KEY "Verb_subject"
#define G_USE_SUBJECT_ON_OBJECT_KEY "Use_subject_on_object"
#define G_GIVE_SUBJECT_TO_OBJECT_KEY "Give_subject_to_object"
