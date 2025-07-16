// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventureGame/VerbType.h"
#include "UObject/Object.h"
#include "CommandCodes.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECommandCodes : uint8
{
    // FREE STATES aka hover
    HoverScene = 0       UMETA(DisplayName = "Hover Scene"),
    HoverInventory = 1   UMETA(DisplayName = "Hover Inventory"),
    HoverItem = 2        UMETA(DisplayName = "Hover Item"),
    HoverHotSpot = 3     UMETA(DisplayName = "Hover HotSpot"),
    HoverVerb = 4        UMETA(DisplayName = "Hover Verb"),

    // ACTIVE locked in and executing
    WalkToHotSpot = 10   UMETA(DisplayName = "Walk to HotSpot"),
    WalkToLocation = 11  UMETA(DisplayName = "Walk to Location"),
    LookAtItem = 12      UMETA(DisplayName = "Look at Item"),
    
    // ACTIVE AND PENDING verbs that are clicked and pending a target or active
    LookAt = 20          UMETA(DisplayName = "Look at It"),
    Give = 21            UMETA(DisplayName = "Give to Someone"),
    Open = 22            UMETA(DisplayName = "Open It"),
    Close = 23           UMETA(DisplayName = "Close It"),
    PickUp = 24          UMETA(DisplayName = "Pick up HotSpot"),
    TalkTo = 25          UMETA(DisplayName = "Talk to Someone"),
    Use = 26             UMETA(DisplayName = "Use It"),
    Push = 27            UMETA(DisplayName = "Push It"),
    Pull = 28            UMETA(DisplayName = "Pull It"),

    // TARGETING - finding a target for the verb
    GiveTo = 30          UMETA(DisplayName = "Give to Someone"),
    UseOn = 31           UMETA(DisplayName = "Use It on Something"),

    // PARENT STATES - top states for all the above
    Free = 40            UMETA(DisplayName = "Free"),
    Pending = 41         UMETA(DisplayName = "Pending"),
    Targeting = 42       UMETA(DisplayName = "Targeting"),
    Active = 43          UMETA(DisplayName = "Active"),
    InstantActive = 44   UMETA(DisplayName = "InstantActive"),
    PendingUse = 45      UMETA(DisplayName = "PendingUse"),
    TargetingUse = 46    UMETA(DisplayName = "TargetingUse"),

    // INTERNAL IMPLEMENTATION STATES
    Initial = 100        UMETA(DisplayName = "Initial"),
    Terminal = 101       UMETA(DisplayName = "Terminal"),
};

/// Create a string description of the code. Not translated, mostly for debugging.
FString CommandCodesToString(const ECommandCodes &Command);

/// Is the @param CommandCode one of the verb codes eg, <i>Push</i> or
/// <i>Open</i> which can only be in the Active or Pending parent states?
bool IsVerbCommandCode(const ECommandCodes &CommandCode);

/// Is the @param CommandCode one of the instantly activated verb codes eg,
/// <i>WalkToLocation</i> or <i>LookAtItem</i> which can only be in the
/// Active parent states?
bool IsActiveCommandCode(const ECommandCodes &CommandCode);

EVerbType GetVerbFromCommandCode(const ECommandCodes &CommandCode);

ECommandCodes GetCommandCodeFromVerb(const EVerbType &Verb);

bool IsHoverCommandCode(const ECommandCodes &CommandCode);