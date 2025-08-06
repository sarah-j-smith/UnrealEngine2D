# Differences Between This and Original

This is my work from following the excellent Lesser Dog [Point and Click 2D Adventure Game tutorial](https://www.youtube.com/watch?v=sEy3c5JcLys&t=7s) on Youtube.

Justin's design and love for the classic Lucas Arts 2D Adventure Games is so good. Its really inspiring and got me running down this track.

# Approach I wanted to take

My favourite Adventure Games were 

1. Gabriel Knight 1 - Sins of the Fathers.
2. Leisure Suit Larry 1
3. Gemini Rue 
4. Return to Monkey Island

...and a bunch of others. GK1 was 640 x 480. 

Generally I wanted to play games that were a higher resolution, and also I didn't want to necessarily worry so much about pixel perfect artwork.

But I wanted to learn as much as I could by following Justin's excellent design.

# Main Variations from Justin's Design

* More C++
* Enhanced Input
* i18n-ready from the start
* Plan to do Higher Game resolutions later
* Not pixel perfect
* Use Timeline nodes instead of 

# Base functionality in C++

Blueprints excel at level design and customising individual pieces of gameplay. But for core logic they can get very complicated and
hard to debug. At least for me. I'm used to debugging C++, with log statements and sometimes the debugger. Justin does create one
C++ class, to handle splitting the screen for the buttons and game area:

* SplitScreenManager

But I wanted to put all the base functionality into C++ and then inherit blueprints to customise as needed. The most important and full-featured C++ classes which implement functionality that is translated from Justin's Blueprints are:

|    Class                        |    What it does                                  |
|---------------------------------|--------------------------------------------------|
|  FollowCamera                   |  Camera LERPS to player in game confines         |
|  BarkText                       |  Player speech display in game                   |
|  Puck                           |  Receives player inputs                          |
|  AdventureCharacter             |  Animations, Player Character AI agent           |
|  AdventurePlayerController      |  Handles player commands, calls AI nav           |
|  AdventureGameInstance          |  Rooms, game save & load                         |
|  ItemList                       |  Inventory                                       |
|  InventoryItem                  |  Behaviour for items, vs verbs                   |
|  ItemDataAsset                  |  Behaviour for combining items & hotspots        |


# Enhanced Input

I wanted to change to using Enhanced Input so that I could more easilty switch to mobile, and also
to use C++ for the bulk of the logic as the blueprints are very complex and hard to follow in the Lesser Dog tutorial.

* [Inputs How to](../../Docs/HowTo.md#create-inputs)

I found that in C++ if I set the `AdventureCharacter` to be possessed by AI, then the 
EnhancedInput system stopped delivering events to it. The solution I used for this was
to create a very small actor called `Puck` that exists only to receive input events. 

During the Player Controllers BeginPlay function an instance of the `Puck` is created
and the enhanced input set up on it, and then the AI possesses the player.

```cpp
void AAdventurePlayerController::BeginPlay()
{
    APawn* PlayerPawn = GetPawn();
    PlayerCharacter = Cast<AAdventureCharacter>(PlayerPawn);

    APawn* Pawn = SetupPuck(PlayerCharacter);
    SetupAIController(PlayerCharacter);
}
```

# PaperZD for the Character Sprite 

Also I wanted to have animation states handled using Paper ZD.  See the doc here for this.

* [Paper ZD animation states](AnimationStateMachine.md)

I moved to using a [character sprite sheet](https://danger-goose.itch.io/point-and-click-adventure-game-sprite-template) from _Danger Goose_ on itch.io. Its intended 
to be a template that you can then overwrite with your own character art.

![Danger goose character](Docs/images/SpH4M4.gif)

This character suits the PaperZD direction based setup which handles all the left/right/up/down
stuff for you as that gets very complex.

## Strings and i18n

I want the game to be in other languages than just English. I have experience of development where we paid a lot of attention
and effort to translation files and it seems bad to cut this out, even at this early stage.

So: I'm using string tables.

* See [this Unreal Garden blog post](https://unreal-garden.com/tutorials/stringtable-cpp/) for a good discussion of how to set this up.

Note: By default Unreal will not include your CSV file(s) as part of the packaging process, even if they are inside the content folder. Make sure to add your CSVs directory to Additional Non-Asset Directories to Package in your project's Packaging settings.

At the end of Justin's tutorial he does go through and replace everything with FText but I had
already gotten ahead of this by then, so started to drift away from his approach.