# Asteroids Evolution
## Intro
Asteroids Evolution is a vertex drawn asteroids-style game, developed using SFML (Simple and Fast Multimedia Library), that showcases the creation of a robust gameplay framework inspired by Unreal Engine's architecture. Focus was placed on implementing advanced game algorithm implementations and exploring high-level game engine frameworks for educational purposes and learning SFML for fast prototyping in the future.

## How to run it & Requirements:
**Required**: CMake 3.11+ for 'FetchContent' to pull SFML 2.6.1 from GitHub 
(tested on CMake 3.30+)
##### Windows:
**Tested on:** Windows SDK 10.0.22621.0 + MSVC 19.41.34123.0
Just run `BuildAndRun.bat`, and you'll find the executable at `\build\Source\Core\Driver\Debug\Asteroids.exe` or run these in cmd:
```sh
mkdir build
cd build
cmake ..
cmake --build . #add "--config Release" here if you want to build for Release
start \Source\Core\Driver\Debug\Asteroids.exe #also replace Debug with Release here if you do
```
##### macOS:
**Tested on:** XCode 16.2 + AppleClang 16.0.0.16000026
Just run ```BuildAndRun.command```, and you'll find the executable at `\build\Source\Core\Driver\Debug\Asteroids` 
```sh
mkdir build
cd build
cmake -G Xcode ..
cmake --build . #add "--config Release" here if you want to build for Release
.\Source\Core\Driver\Debug\Asteroids #also replace Debug with Release here if you do
```

## Note:
Unfortunately, when I started working on this project I didn't know how to use CMake, and I later converted it to use CMake. 

## Features

###	Object-Oriented Design with Component-based features 
- Used `Component` -> `SceneComponent` -> `PrimitiveComponent` hierarchy to mimic Unreal Engine
- Settable **Render and Update Priorities.**
- Abstracted Object/ Entity/ Component lifetimes and ownership.
###	Enemy Ships
- Implemented **Craig Reynold's Flocking algorithm** for group movement and player seeking behavior. (see: `Source/Ships/Bot.cpp` -> `void Bot::AddFlockingForces()`)
- Group Shooting policy for limiting max number of enemy ships to Auto Shoot at player when they have Line of Sight
### **Implicit Grids** for collision detection using **Binary Spatial Partitioning**.
- Implementation in `Core/Physics/ImplicitGrid.h`
- Added LineCasting and CircleCasting functions. (`Core/Physics/Physics.h`)
### Timer & Timer Manager
- With `OnStart()` `OnComplete()` and `OnUpdate()` assignable functions
### A customisable and flexible **Particle System**.
- Pooled particles
- Support for Emission Bursts
- Update Colour & Scale over lifetime
- Set Emission Orientation and Initial velocity to be applied in Global or Local space.
- Set Emitter and Particle shape, shape can be:
    - **_Static_** vector of 
    - **_Logical_** function with a resolution specified, will generate points using the function using a 0-1.f
    - **_Static presets_** such as Circle, Square, Line.
### Templated **Object Pooling**
- Used for particles/ states and their lifetimes.
- Restricted to pools holding weak pointers to pooled objects.
- Make any object poolable by inheriting from APoolable.
### Global **Input Event Messaging** System
- Create Input bindings on **Button Pressed, Button Held, Button Released**.
- Wrapper on top of SFML input event polling
### **Graph** based on linked lists 
- Written to be scalable and ready to use with multiple prospect graph requirements (such as pathfinding).
- Used it for implementing **Finite State machine**. `Core/Graph/FSM/FSM.h`
    - Used by Portal, Enemy Ship, Kamikaze Enemy Ship
    - Reusable BaseWaitState so you can add functionality at the start, end and tick of the timer on child states.

#####	Added a Stars background to game.
- Random stars keep twinkling over time.
- 2D Parallax effect based on player’s acceleration.
##### `UIManager` for HUD
- Renders UI Objects only, made functionality to easily display Text but can be easily extended to render shapes etc.

## Design patterns
- Game loop (lol)
- Dirty Flag 
	- Deferred delete for `Core/ECS/Object.h` -> `Object::Kill()`
	- Component transform recalculation optimization (`Core/ECS/GameEntity.h` -> `GameEntity::bPositionDirty_`)
- Observer (`Core/Event.h`)
- Strategy (`Ships/AShootingStrategy.h`)
- Object Pooling (`Core/Pooling/WeakPointerObjectPool.h`)
- State Machine (`Graph/FSM/FSM.h`, `EnemyShip` has 2 FSMs, 1 for attacking and movement each, `Portal` for teleport cooldown)
- Singleton (`System` is a singleton, which owns `FSMManager`, `ScreenShaker`, etc and gives weak pointers when requested.)