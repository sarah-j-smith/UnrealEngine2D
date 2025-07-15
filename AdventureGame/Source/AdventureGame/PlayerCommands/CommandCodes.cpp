// (c) 2025 Sarah Smith


#include "CommandCodes.h"

bool IsVerbCommandCode(const ECommandCodes &Command)
{
    return static_cast<uint8>(Command) >= static_cast<uint8>(ECommandCodes::LookAt) &&
        static_cast<uint8>(Command) <= static_cast<uint8>(ECommandCodes::Pull);
}

bool IsActiveCommandCode(const ECommandCodes &CommandCode)
{
    return static_cast<uint8>(CommandCode) >= static_cast<uint8>(ECommandCodes::WalkToHotSpot) &&
        static_cast<uint8>(CommandCode) <= static_cast<uint8>(ECommandCodes::LookAtItem);
}

bool IsHoverCommandCode(const ECommandCodes &CommandCode)
{
    return static_cast<uint8>(CommandCode) >= static_cast<uint8>(ECommandCodes::HoverScene) &&
        static_cast<uint8>(CommandCode) <= static_cast<uint8>(ECommandCodes::HoverVerb);
}

FString CommandCodesToString(const ECommandCodes &CommandCode)
{
    switch (CommandCode)
    {
    case ECommandCodes::HoverScene:
        return "HoverScene";
    case ECommandCodes::HoverInventory:
        return "HoverInventory";
    case ECommandCodes::HoverItem:
        return "HoverItem";
    case ECommandCodes::HoverHotSpot:
        return "HoverHotSpot";
    case ECommandCodes::HoverVerb:
        return "HoverVerb";
    case ECommandCodes::WalkToHotSpot:
        return "WalkToHotSpot";
    case ECommandCodes::WalkToLocation:
        return "WalkToLocation";
    case ECommandCodes::LookAtItem:
        return "LookAtItem";
    case ECommandCodes::LookAt:
        return "LookAt";
    case ECommandCodes::Give:
        return "Give";
    case ECommandCodes::Open:
        return "Open";
    case ECommandCodes::Close:
        return "Close";
    case ECommandCodes::PickUp:
        return "PickUp";
    case ECommandCodes::TalkTo:
        return "TalkTo";
    case ECommandCodes::Use:
        return "Use";
    case ECommandCodes::Push:
        return "Push";
    case ECommandCodes::Pull:
        return "Pull";
    case ECommandCodes::GiveTo:
        return "GiveTo";
    case ECommandCodes::UseOn:
        return "UseOn";
    case ECommandCodes::Free:
        return "Free";
    case ECommandCodes::Pending:
        return "Pending";
    case ECommandCodes::Targeting:
        return "Targeting";
    case ECommandCodes::Active:
        return "Active";
    default:
        return "Unknown";
    }
}

EVerbType GetVerbFromCommandCode(const ECommandCodes &CommandCode)
{
    switch (CommandCode)
    {
    case ECommandCodes::LookAt:
        return EVerbType::LookAt;
    case ECommandCodes::Give:
        return EVerbType::Give;
    case ECommandCodes::Open:
        return EVerbType::Open;
    case ECommandCodes::Close:
        return EVerbType::Close;
    case ECommandCodes::PickUp:
        return EVerbType::PickUp;
    case ECommandCodes::TalkTo:
        return EVerbType::TalkTo;
    case ECommandCodes::Use:
        return EVerbType::Use;
    case ECommandCodes::Push:
        return EVerbType::Push;
    case ECommandCodes::Pull:
        return EVerbType::Pull;
    case ECommandCodes::GiveTo:
        return EVerbType::GiveItem;
    case ECommandCodes::UseOn:
        return EVerbType::UseItem;
    default:
        return EVerbType::WalkTo;
    }
}

ECommandCodes GetCommandCodeFromVerb(const EVerbType& Verb)
{
    switch (Verb)
    {
    case EVerbType::LookAt:
        return ECommandCodes::LookAt;
    case EVerbType::Give:
        return ECommandCodes::Give;
    case EVerbType::Open:
        return ECommandCodes::Open;
    case EVerbType::Close:
        return ECommandCodes::Close;
    case EVerbType::PickUp:
        return ECommandCodes::PickUp;
    case EVerbType::TalkTo:
        return ECommandCodes::TalkTo;
    case EVerbType::Use:
        return ECommandCodes::Use;
    case EVerbType::Push:
        return ECommandCodes::Push;
    case EVerbType::Pull:
        return ECommandCodes::Pull;
    case EVerbType::GiveItem:
        return ECommandCodes::GiveTo;
    case EVerbType::UseItem:
        return ECommandCodes::UseOn;
    default:
        return ECommandCodes::WalkToLocation;
    }
}
