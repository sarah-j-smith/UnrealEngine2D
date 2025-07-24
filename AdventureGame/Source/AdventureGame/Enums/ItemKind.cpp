// (c) 2025 Sarah Smith


#include "ItemKind.h"

#include "AdventureGame/Constants.h"

FString FItemKind::GetListDescription(const TArray<EItemKind>& List)
{
    /// Mostly for debug purposes, don't translate any of this
    FString Accum = "";
    FString Concat = "";
    for (int i = 0; i < List.Num(); i++)
    {
        Accum += Concat + GetDescription(List[i]).ToString();
        Concat = ", ";
        if (i == List.Num() - 1) Concat = " and ";
    }
    return Accum;
}

FText FItemKind::GetDescription(const EItemKind &ItemKind)
{
    if (ItemKind == EItemKind::None) return FText::GetEmpty();
    const FName ItemName = GetUniqueName(ItemKind);
    return FText::FromStringTable(ITEM_DESCRIPTIONS_KEY, ItemName.ToString());
}

FName FItemKind::GetUniqueName(const EItemKind &ItemKind)
{
    // These values must match the data table row names
    // See Content/StringTables/ItemDescriptions.csv and
    // Content/PointAndClick/Blueprints/Inventory/DT_ItemList
    switch (ItemKind)
    {
    case EItemKind::Pickle:
        return FName("Pickle");
    case EItemKind::PickleKey:
        return FName("Pickle_Key");
    case EItemKind::Knife:
        return FName("Knife");
    case EItemKind::None:
    default:
        return FName("None");
    }
}
