# Asteroids
## Intro
This is an Asteroids clone made using SFML 2.6.1. Unfortunately, when I started working on this project I didn't know how to use CMake, and I later converted it to use it. As a result, git thinks all my source code is deleted instead of moved - and the files all have wiped git histories. A previous commit should have the history though.

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
### **Graph** based on linked lists 
- Written to be scalable and ready to use with multiple prospect graph requirements (such as pathfinding).
- Used it for implementing Finite State machine. `Core/Graph/FSM/FSM.h`
- Used by Portal, Enemy Ship, Kamikaze Enemy Ship
- Reusable BaseWaitState so you can add functionality at the start, end and tick of the timer on child states.

#####	Added a Stars background to game.
- Random stars keep twinkling over time.
- 2D Parallax effect based on player’s acceleration.
##### `UIManager` for HUD
- Renders UI Objects only, made functionality to easily display Text but can be easily extended to render shapes etc.

#### Misc
- #####	Added a `Debug` class that helped to easily send commonly used information to the console.
- ##### Impulse based `ScreenShaker`
- ##### `Portal` entity used to teleport to and from.

## Design patterns
- Game loop (lol)
- Dirty Flag (deferred delete for `Core/ECS/Object.h` -> `Object::Kill()`)
- Observer (`Core/Event.h`)
- Strategy (`Ships/AShootingStrategy.h`)
- Object Pooling (`Core/Pooling/WeakPointerObjectPool.h`)

## How to run it:
Just run GenerateAndBuild.bat, and you'll find the executable at `${build\Source\Core\Driver\Debug\Asteroids.exe}` 
or :
```sh
mkdir build
cd build
cmake ..
cmake --build .
```

## Note:
the CMake version I used is 3.30-rc3, you may try your own version.
Atleast CMake 3.11 (not tested) is required for "FetchContent" that I use to get sfml from github. Cheers!
