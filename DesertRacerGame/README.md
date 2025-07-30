# Desert Racer - Unreal Tuto 1

These instructions are for Mac. From:

> Unreal Engine 2D: Make Your Own Epic 2D Games Using C++

* Kaan Alpar, [GameDev.tv](https://www.gamedev.tv/dashboard/courses/115)

# Use Rider

* Crazy easy, crazy powerful and free to use for personal use.
* Much, much better integration with Unreal. 
* [Rider with Unreal instructions](https://www.jetbrains.com/help/rider/Unreal_Engine__Before_You_Start.html)

# Install and Run Unreal Engine 5.3.2

* [Download Unreal] and install
* Once Unreal has fully initialised on first run (can take a long time) close it down
* Double click the file DesertRacer.uproject to open DesertRacer in Unreal. 
* UnrealEditor > Preferences > General > Source Code Editor > Visual Studio Code
* Choose Tools > Refresh Visual Studio Code project

[Download Unreal]: https://www.unrealengine.com/en-US/download

# Generate Build Files

Disregard all this, and let Rider do it.

The regen of the project will create:

* compile command `*.rsp` files to define the clang commands for every source code file
    - under .vscode/compileCommands_{Default/DesertRacer}
* refreshed `DesertRacer.code-workspace` file which defines the build tasks

Instead of refreshing the project from Unreal you can also run:

* regen-unreal.sh - [full script]

```bash
/Users/Shared/Epic\ Games/UE_5.3/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh -project=${CUR_DIR}/${PROJ_NAME}.uproject -game
```

[full script]: https://gist.github.com/sarah-j-smith/cc4f2294de028c8be86c3255dfa52e17

# Editing Code

* Adding any files requires a re-run of the regen script.

# HowTo 

* [See How To Create this project] in Unreal and C++.

[See How To Create this project]: ./Docs/HowTo.md

# Troubleshooting

```
UE_LOG(LogTemp, Warning, TEXT("binding capsule collision handler"));
```

Checkin the **Output Log** bottom left of the Unreal Editor window.

* Ensure the Editor and all Unreal processes (in Activity monitor) are not running when rebuilding
* Do a regen after any header file changes to pick up changes
* If a clean doesn't help can try nuking all the build files:
    - `Binaries`
    - `Intermediate`
    - `Saved`
    
