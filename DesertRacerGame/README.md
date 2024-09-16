# Desert Racer - Unreal Tuto 1

These instructions are for Mac. From:

> Unreal Engine 2D: Make Your Own Epic 2D Games Using C++

* Kaan Alpar, [GameDev.tv](https://www.gamedev.tv/dashboard/courses/115)

# Setup C++, Visual Studio Code for Unreal Engine

* [Install VS Code]
* [Install clang and setup VS Code for clang]
   - Including [Prerequisites] of setup of VS Code for Mac
   - And installing clang/Xcode and setting up

[Install VS Code]: https://code.visualstudio.com/
[Install clang and setup VS Code fo clang]: https://code.visualstudio.com/docs/cpp/config-clang-mac
[Prerequisites]: https://code.visualstudio.com/docs/cpp/config-clang-mac#_prerequisites

# Install and Run Unreal Engine 5.3.2

* [Download Unreal] and install
* Once Unreal has fully initialised on first run (can take a long time) close it down
* Double click the file DesertRacer.uproject to open DesertRacer in Unreal. 
* UnrealEditor > Preferences > General > Source Code Editor > Visual Studio Code
* Choose Tools > Refresh Visual Studio Code project

[Download Unreal]: https://www.unrealengine.com/en-US/download

# Generate Build Files

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
* If there are issues with intellisense check [this guide for VSCode]
* Try including these lines in `c_cpp_properties.json`:

* c_cpp_properties.json - [full file]

```json
{
    "configurations": [
        {
            // other settings
            "includePath": [ 
                "${workspaceFolder}/Intermediate/**", 
                "${workspaceFolder}/Plugins/**", 
                "${workspaceFolder}/Source/**" 
            ]           
        },
    ],
    "version": 4
}
```

* [Reference for variables] like `workspaceFolder`

[this guide for VSCode]: https://code.visualstudio.com/docs/cpp/configure-intellisense

[Reference for variables]: https://code.visualstudio.com/docs/editor/variables-reference

[full file]: https://gist.github.com/sarah-j-smith/317386821d3de5b08fcf7605187caf60

# HowTo 

* [See How To Create this project] in Unreal and C++.

[See How To Create this project]: ./Docs/HowTo.md