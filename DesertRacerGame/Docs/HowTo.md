# How To

## Create a Game and Main Level
* Open Unreal and from the welcome wizard or File > New Project choose "Blank" Game
    - Name it "DesertRacer" and choose "C++"
    - click "Create"
    - On creation ignore the map, we create a blank level
* Create a New Level "MainLevel" > Save it into a new folder `Content/Levels`

## Project Setup
* Edit > Project Settings > Maps & Modes
    - Set "MainLevel" to be the Editor Startup map and Game Default map
* Add copyright notice and other info in the 
    - Edit > Project Settings > Project > Description

## Paper2D setup
* Check Edit > Plugins
    - Check Paper2D is enabled and context menu in the content draw shows Paper2D items
* Edit `ProjectDir/Source/ProjName/ProjName.Build.cs`
    - Include `"Paper2D", "EnhancedInput"` in the array of `PublicDependencyModuleNames`

## Import Assets
* Create a folder `All/Content/Assets/Textures` and put all the graphics in there
* Unreal will import them automatically but Paper2D requires some extra setup
    - right click and Sprite Actions > Apply Paper2D Texture Settings
    - ensure textures are set to the TranslucentUnlit sprite material
* From each texture right-click and Sprite Action > Create Sprite
    - drag the resulting sprite into a new `Assets/Sprites` folder

## Create the C++ Class 
* Tools > New C++ Class - Choose Pawn and name it `PlayerCharacter`
* Refresh the VSCode project & close Unreal
    - Can also use the `regen-unreal.sh` [script]
* Open `ProjName.code-workspace` which will launch VSCode
* Fill PlayerCharacter.{h,cpp} out per the code here - in VSCode
* Compile the code via **cmd-shift-B** > DesertRacerEditor Mac Development Build
* Open up Unreal Editor again via `ProjName.uproject`

[script]: https://gist.github.com/sarah-j-smith/317386821d3de5b08fcf7605187caf60

## Blueprints
* Create a Blueprint via
    - Right click in a new folder `Content/Blueprints` and Create Blueprint Class
    - Search for `PlayerCharacter` - and name it `Blueprint_PlayerCharacter`
* Set the `Translucency Sort Priority` for the sprite so that it renders in order
    - Can set backgrounds and environment sprites to say `-100` up to `-10`
    - Can set the player to `0` 

## Create Inputs
* Create a `All/Content/Inputs` folder
* Right click and create an **Input Action** and an **Input Mapping Context**
    - set the input action to have a Vector 2D - this gives it 4 axes
    - set the input mapping context as per below
    - plug these into the slots in the blueprint created by the `UPROPERTY(EditAnywhere, BlueprintReadOnly)`

![input-context](input-context.png)

## Camera
* For 2D - want to set camera to Orthographic
* Use the spring arm to set the camera position via rotating it 90 degrees
* Turn the rotation to Global so it won't move with the rotation of the car
* Set the camera width to allow a decent view 

## Game Mode
* Create a new C++ Class for this based off the Game Mode base class
* Create a blueprint `BP_MainGameMode` based off of it and:
    - Edit > Project Settings > Maps & Modes
    - Default game mode: `BP_MainGameMode`
    - Default pawn class: `BP_PlayerCharacter`

## Obstacles
* Can create a single `Obstacle` class and 
    - Create a `BP_TyreObstacle` and a `BP_FenceObstacle` from it
        - just changing the sprite and collision capsule
        - turn off the collision preset - `NoCollision` for the sprite
        - the capsule will be `DynamicAll`

# Music

Background music can be launched off the Game Instance, a class instance
that persists between level changes. But the Start Game event cannot
be used for this - it has to be triggered from the Game Mode when the 
game starts.

## Create a Game Instance

* Right click in Blueprints folder and create a new Blueprint
* Base it off the Game Instance class (not the Platform Game Instance)
    - Call it `BP_MyGameInstance`
* Edit > Project Settings > Maps & Modes > Game Instance
    - Select `BP_MyGameInstance`

## Add the Logic for the Music to Start

* Inside `BP_MyGameInstance`

![screenshot of blueprint](./BP_MyGameInstance.png)

* Inside `BP_MyGameMode`

![screenshot of blueprint](./BP_MyGameMode.png)
