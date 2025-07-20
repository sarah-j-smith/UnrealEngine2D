# Unreal Engine Notes

These are code and notes from my learning process for C++ dev with Unreal Engine.

There's two main sets of projects here: courses I did on GameDev.tv and also
a C++ version of Lesser Dog's Unreal Engine Lucas Arts style Adventure Game tut.

## Lucas Arts 2D Adventure Game

* Justin of Lesser Dog Tut's [Lucas Arts Adventure Game tuto](https://www.youtube.com/watch?v=sEy3c5JcLys&t=7s)

* Learning by porting to C++ from his Blueprint version with very little C++.

Go [to my notes and C++ source code](./AdventureGame/README.md) for more.

## Unreal Engine 2D: Make Your Own Epic 2D Games Using C++

* Kaan Alpar, [GameDev.tv](https://www.gamedev.tv/dashboard/courses/115)
1. Desert Racer - drive a car around a track avoiding obstacles
2. Gun Survivors - shoot monsters that attack from all directions
3. Crusty Pirate - platformer with attacking crabs

Covers making sprites with Paper2D, animating with flipbooks, making blueprint classes
from base classes to allow configuration in the engine, via the blueprint visual coding
language. Desert Racer and Gun Survivors both include sections on camera configuration
for 2D. The Crusty Pirate game includes the PaperZD plugin and its state machine for
automatically switching between flipbooks based on conditional tests.

Go [to the game dev notes on Desert Racer](./DesertRacerGame/README.md) for more details
on all of the above.

Go [to the game dev notes on Gun Survivors](./GunSurvivorsGame/README.md) for creating a 
cursor and converting the game to run on iOS.

Go [to the game dev notes on Crusty Pirate](./CrustyPirateGame/README.md) for animation
overrides and jump nodes.