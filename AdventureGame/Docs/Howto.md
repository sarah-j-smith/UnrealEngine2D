# How to Use To Create a New Game

At present this is not a module or a plugin, its a demo project. It's not a good
idea to just reskin this, as its bare bones and includes Justin's art from his tuto.

## First Get this Running

Before trying to migrate or use content from this demo suggest running it under
a version of Unreal that matches what I used. Currently that is v5.5.4.

Download the project, and try to open the `AdventureGame.uproject` file by
either double-clicking it; or loading it in Rider, and using the run button
to run it from source code.

## Word of Caution 

* I strongly suggest create your own art, UI and interactions
* The ones in here probably won't suit your target resolution, style etc.

As far as I know the way to create a new game from it, is to:

* Create a brand new Empty 2D game
* Copy my C++ code into it and get it to compile
* Copy over the .CSV string files in the `Content/StringTables`
* Inheriting the relevant blueprints and configure them
* Add your artwork and animations
* Add your own UI
* Add your doors and scripting

To re-use this UI see "How to Migrate UI".

# Create a brand new Empty 2D game

This is the same as the the Desert Racer and other 2D games here.

## Create a Game and Main Level
* Open Unreal and from the welcome wizard or File > New Project choose "Blank" Game
    - Name it "MyGame" and choose "C++"
    - click "Create"
    - On creation ignore the map, we create a blank level
* Create a New Level "MainLevel" > Save it into a new folder `Content/Levels`

## Project Setup
* Edit > Project Settings > Maps & Modes
    - Set "MainLevel" to be the Editor Startup map and Game Default map
* Add copyright notice and other info in the 
    - Edit > Project Settings > Project > Description

## Setup for Pixel Art
* Motion blur off
* Auto exposure turn off
* Anti-aliasing off - for pixel art

## Import and sprite creation scale
* Pixels-per-Unreal-unit - eg 0.5 (instead of 1.0)
    * Under Editor - Paper2D - Import
    * Effectively scales up the pixel art
    * Needed for movements systems
    * Needs to approximate human measurements 

# Plugins and Code

To get the code and compile it first ensure the dependencies are correct.

## Plugins and Dependencies setup

* Check Edit > Plugins
    - Check `Paper2D` is enabled
    - Enable `PaperZD` and `AI Support` is enabled 

![Enable the AI support plugin](./images/AI-plugin.png)

* Edit `ProjectDir/Source/ProjName/ProjName.Build.cs`
    * in the array of `PublicDependencyModuleNames` include

```csharp
"Paper2D", "EnhancedInput", "AIModule", "Slate", "SlateCore"
```

Restart the Unreal Editor to load the plugins, check there's no errors. Try adding some 
test PNG texture to the content drawer and confirm that right-click gives the Paper2D Asset
actions menu.

## Initialise C++ 

Check your editor preferences include your code editor of choice. I like Rider, if using
that ensure its plugins are set up. It should prompt.

Use Tools > New C++ class to create an `AActor` subclass called "MyActor" and confirm it loads and compiles in Rider.

Copy the files over using your Finder Option-Drag or Windows Explorer Ctrl-Drag:

![Copy the code](./images/copy-code.png)

Either use Rider or your IDE to do a global replace of `ADVENTUREGAME_API` with whatever your games
macro is - eg `MYGAME_API`. Maybe you can use a define in `Constants.h` like this:

```
#define ADVENTUREGAME_API MYGAME_API
```

## Get the Code to Compile

There may be a few other search and replaces needed. I'll try to clean these up.

## Set up Redirects

Add the following to `MyGame/Config/DefaultEngine.ini`:

```ini
[CoreRedirects]
+ClassRedirects=(OldName="/Script/AdventureGame.InteractionHUD",NewName="/Script/RobinsRoom.InteractionHUD")
+ClassRedirects=(OldName="/Script/AdventureGame.InventoryUI",NewName="/Script/RobinsRoom.InventoryUI")
+ClassRedirects=(OldName="/Script/AdventureGame.ItemSlot",NewName="/Script/RobinsRoom.ItemSlot")
+ClassRedirects=(OldName="/Script/AdventureGame.BarkText",NewName="/Script/RobinsRoom.BarkText")
+ClassRedirects=(OldName="/Script/AdventureGame.VerbsUI",NewName="/Script/RobinsRoom.VerbsUI")
+ClassRedirects=(OldName="/Script/AdventureGame.AdventureGameHUD",NewName="/Script/RobinsRoom.AdventureGameHUD")
```


## Import Assets
* Create a folder `All/Content/Assets/Textures` and put all the graphics in there
* Unreal will import them automatically but Paper2D requires some extra setup
    - right click and Sprite Actions > Apply Paper2D Texture Settings
    - ensure textures are set to the TranslucentUnlit sprite material
* From each texture right-click and Sprite Action > Create Sprite
    - drag the resulting sprite into a new `Assets/Sprites` folder
* Create Flipbooks from the sprites 

# How to Migrate UI

You'll need to migrate the UI files, which will pull in the fonts and other
resources that those reference. Then you'll need to redirect the blueprints to use
the 
* Close your existing game and note where it is on disk
* Open the AdventureGame.uproject in the editor
* In the content browser select all the Blueprint files in the Widgets folder
* Choose "Migrate" and in the target select the "MyGame/Content" folder
* They will wind up in a `Content/PointAndClick/Blueprints/Widgets/` folder

If you did the C++ redirects above they should find their parents fine.

# Blueprint Setup and Configuration

* Create a top-level content folder named after your game
* Create a `Content/MyGame/Blueprints/Gameplay` folder
    * In `Gameplay` create:
    * 
* Create a `Content/MyGame/Blueprints/Player` folder
* In `Player`, create these off the relevant C++ classes:
    * BP_AdventurePlayerController
    * BP_AdventureCharacter
    * BP_FollowCamera
    * BP_Puck
    * BP_BarkText
* Open the `BP_AdventureCharacter` and set up:

![AI controller setup](./images/ai-controller-setup.png)

    * Set the Bark Text Class to your `BP_BarkText` class.

Copy the code from this project to populate those classes, taking care not to delete the
`MYPROJECT_API` macro from the header.


