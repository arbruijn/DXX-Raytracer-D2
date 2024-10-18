![scrn0004](d2/assets/scrn0004.png)

# DXX Raytracer-D2
DXX Raytracer-D2 is a fork of the [DXX Raytracer](https://github.com/BredaUniversityGames/DXX-Raytracer) project for Descent 1. It is in alpha state, it runs but has not been tweaked or tested. It might crash or become unplayable when playing the game.


## Installation
[Download d2x_raytracer.zip here.](https://github.com/arbruijn/DXX-Raytracer-D2/releases/latest) 

Extract the zip file to a location of your choosing, and add all the  .hog, .ham, .s22, .pig and .mvl files from your Descent 2 installation. Then run descent2.exe.

## Features
- Physically-based rendering
- Soft shadows
- Pathtraced global illumination
- Bloom
- Temporal anti-aliasing
- Motion blur
- Post-processing (Vignette, tonemap, etc.)

## Instructions
- SHIFT + ALT + F1: Open debug menus, more on those below
- SHIFT + ALT + F2: Toggle depth testing for debug lines
- SHIFT + ALT + F: Toggle free camera option
- F1: Menu with important keybindings from the game
- ALT + F2: Save your game
- ALT + F3: Load a save game

## Debug Menus
- Render Settings: All kinds of graphics settings and debug utility for rendering
- GPU Profiler: A profiling tool to see how much time individual render passes took on the GPU
- Material Editor: Adjust material properties from any level
- Polymodel Viewer: Allows you to view a polymodel in the current scene.
- Dynamic Lights: Allows for tweaking of dynamic lights, like weapons, explosions, and muzzle flashes.
- Light Explorer: Allows for tweaking of individual lights, and adds ability to debug view lights in the level.
