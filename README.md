# First Person Shooter
> a doom style fps game built with **OpenGL**. walk through the maze and find the enemies (a red cube for now).

![demo](res/gameplay.gif)


## Features 
- Camera with mouse look
- grid based level system 
- level loading form text files (you cant change it live)
- shooting mechanics
- lighting
- textures 

## Controls
- **WASD**: move around
- **mouse**: look around
- **left click** shoot
- **R**: reload level
- **M**: print map to console
- **TAB**: show cursor
- **ESC**: exit game

## Building
### Requirements
- Cmake 
- c++ compiler
- GLFW 3.3+
- GLM 

### compilation
```bash

mkdir build
cd build
cmake ..
make
./fps

```
or download the pre built version from itch.io [here](https://shahd-moh-abdel.itch.io/fps)

## Level making
levels are stored in 'levels/' folder.

### level format 
```

# = Wall
. = empty space
P = pillar
S = player spawn point
E = enemy spawn point

```

## What next?
I still have a lot to add, like doors, items, real monsters, sounds, and shadows.
