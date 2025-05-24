# Crusty Pirate - Unreal Tuto 3

These instructions are for Mac. From:

> Unreal Engine 2D: Make Your Own Epic 2D Games Using C++

* Kaan Alpar, [GameDev.tv](https://www.gamedev.tv/dashboard/courses/115)

## PaperZD Animation

* See the [How-To Section](../../../Docs/HowTo.md#use-paperzd-for-fsm-animation) for PaperZD.

* [PaperZD Docummentation](https://www.criticalfailure-studio.com/paperzd-documentation/)

### Jump to Node

The usual way to get the animation statemachine to transition to a new node and begin playing
the relevant animation is for a rule condition to be satisfied.

But to trigger this in code, instead of having to create some boolean just for that you can
use `UPaperZDAnimInstance::JumpToNode` for example:

```cpp
    // Play the die animation
    GetAnimInstance()->JumpToNode(FName("JumpDie"), FName("CrabbyStateMachine"));
```

### Animation Override

For an attack, it could happen when running, idling, jumping or falling. So to do it with rules and connections would require a lot of added complexity that makes the state machine unmanageable. It would
be better to run the attack without the statemachine.

When you need to jump out of the animation state machine completely, and play an animation regardless of
the character's current state, you can call `UPaperZDAnimInstance::PlayAnimationOverride`.

This is not really discussed in the PaperZD animation documentation.

Steps to create animation override:

1. Include the header and then get a reference to the animation to jump to:

_Header file_: `Enemy.h`

```cpp
    #include "PaperZDAnimInstance.h"
```

and create this member variable, and set it inside the UE Editor Blueprint to the attack animation.

```cpp
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UPaperZDAnimSequence *attackAnimSequence;
```

Also create an override slot inside the `AnimBP_CharacterName > AnimGraph` - this will sit between the
state machine and the output animation. The name `DefaultSlot` is fine for now.

![override slot](./Docs/override-slot-creation.png)

2. If you need to respond to the animation ending, first create a delegate

_Header file_: `Enemy.h`

```cpp
    FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;
```

...then create a handler member function to receive the call:

```cpp
    void OnAttackOverrideAnimEnd(bool completed);
```

3. To respond to the animation ending, bind the delegate:

_Implementation file_: `Enemy.cpp` - in the Enemy's `BeginPlay()`

```cpp
    OnAttackOverrideEndDelegate.BindUObject(this, &AEnemy::OnAttackOverrideAnimEnd);
```

4. When its time to trigger the animation override:

_Implementation file_: `Enemy.cpp` - in the Enemy's `Attack()`

```cpp
    UPaperZDAnimInstance *anim = GetAnimInstance();
    anim->PlayAnimationOverride(attackAnimSequence, FName("DefaultSlot"), 1.0f, 0.0f, OnAttackOverrideEndDelegate);
```

5. Then in the handler respond to the end:

```cpp
void AEnemy::OnAttackOverrideAnimEnd(bool completed)
{
    // Do things when the animation ends
}
```

## Attribution

* [male-grunt wav file](https://freesound.org/s/621365/)

Male Grunt 6 - WITH reverb.wav by WelvynZPorterSamples -- License: Creative Commons 0