#pragma once

/// Table of item kind fallback description text. Key name assigned for the
/// Item Descriptions table on load. See AdventureGame.cpp for loading details.
/// Use a plain C macro so that LOCTABLE and other macros will work.
#define ITEM_DESCRIPTIONS_KEY "ItemDescriptions"

/// Table of default item behaviour texts. Key name assigned for the Item Strings
/// table on load.  See AdventureGame.cpp for loading details. Use a plain C macro
/// so that LOCTABLE and other macros will work.
#define ITEM_STRINGS_KEY "ItemStrings"

/// Folder underneath the root `Content` directory to put all string tables into
#define STRING_TABLES_DIR "StringTables"

#define ITEM_STRINGS_CSV_PATH STRING_TABLES_DIR "/" ITEM_STRINGS_KEY ".csv"

#define ITEM_DESCRIPTIONS_CSV_PATH STRING_TABLES_DIR "/" ITEM_DESCRIPTIONS_KEY ".csv"