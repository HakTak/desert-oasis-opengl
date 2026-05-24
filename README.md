# Desert Landscape Scene

## Project Overview

**Desert Landscape Scene** is a 3D graphics application that renders a desert landscape scene using OpenGL. The project demonstrates advanced graphics programming concepts including shader programming, texture mapping, lighting, and interactive user controls.

## Technical Details

- **Language**: C++
- **Graphics API**: OpenGL 3.3+ (Core Profile)
- **Dependencies**: 
  - GLFW 3.3.8 (Window management and input handling)
  - GLEW 2.2.0 (OpenGL extension loading)
  - GLM 0.9.9.8 (Mathematics library)
  - stb_image.h (Image loading)

## Scene Features

### Landscape Elements
- **Pyramids**: At least three pyramids forming the main landscape structures
- **Oasis**: Central water feature with grass along its shore
- **Terrain**: Sand desert ground with appropriate texturing

### Sky and Celestial Bodies
- **Dynamic Sky**: Color changes throughout the day-night cycle
- **Sun**: Moves along a circular path across the sky, gradually setting
- **Moon**: Crescent moon that rises on the opposite side as the sun sets, following the same circular path
- **Stars**: Visible during nighttime hours

### Interactive Features

#### Grass Control
- **Key 1**: Hide all grass around the oasis
- **Key 2**: Show grass (restore visibility)
- While grass is hidden: A fish becomes visible in the translucent water, swimming left and right over time

#### Time Control
- **Key P**: Pause the day-night cycle (the fish continues moving)
- **Key R**: Reset to morning and resume the day-night cycle

#### Pyramid Coloring
- **Key A**: Gradually change the largest pyramid's color to red in vertical stripes from left to right
- **Key D**: Adjust the red coloring effect

#### Pyramid Entrance Interaction
- **Key O**: Open entrances on all pyramids, which gradually expand from the bottom upward
- **Mouse Click**: Click on any pyramid entrance to trigger text display
  - Text "Nastaviće se na 3D projektu" (Will continue on the 3D project) displays letter by letter
  - After completion, the text gradually fades out letter by letter
  - Application automatically closes 2 seconds after the text completely disappears

### Display Information
- Student name, surname, and index number displayed in an arbitrary corner of the screen

## Controls

| Key | Action |
|-----|--------|
| **1** | Hide grass |
| **2** | Show grass |
| **A** | Add red striping to largest pyramid (left) |
| **D** | Adjust red striping on largest pyramid (right) |
| **P** | Pause day-night cycle |
| **R** | Reset to morning and resume time |
| **O** | Open pyramid entrances |
| **Mouse Click** | Activate entrance (when open) |
| **ESC** | Close application |

## Building and Running

### Requirements
- Visual Studio 2019 or later
- Windows 10 or later
- Graphics card supporting OpenGL 3.3 or higher

### Build
1. Open `Sablon.sln` in Visual Studio
2. Build the project (Debug or Release configuration)
3. Run the executable

### Full Screen Execution
The application automatically launches in full-screen mode at the monitor's native resolution.

## Project Structure

```
Desert Landscape Scene/
├── main.cpp                 # Main application code
├── basic.vert/frag          # Basic shader programs
├── basicTeksture.vert/frag  # Texture shader programs
├── trava.vert/frag          # Grass rendering shaders
├── crvena.frag              # Red color shader
├── prolazi.vert/frag        # Passage/transition shaders
├── zemlja.vert              # Ground/terrain shader
├── oaza.vert                # Oasis shader
├── slova.vert/frag          # Text rendering shaders
├── ribicaTekstura.vert      # Fish texture shader
├── zvezde.vert              # Stars shader
├── stb_image.h              # Image loading library
├── packages.config          # NuGet package configuration
├── Sablon.sln               # Visual Studio solution file
├── Sablon.vcxproj           # Visual Studio project file
└── README.md                # Original specification (Serbian)
```

## Shader Pipeline

The project uses multiple shader programs for different scene components:

- **Basic shaders**: Core geometry rendering
- **Texture shaders**: Textured surfaces (ground, textures)
- **Specialized shaders**: Grass, pyramids, oasis water, fish, stars, and text rendering
- **Color shaders**: Red coloring effect for pyramid transformation

## Graphics Concepts Demonstrated

- **Vertex and Fragment Shaders**: Custom GLSL shader programs
- **Texture Mapping**: Loading and applying textures to 3D objects
- **Time-based Animation**: 
  - Sun and moon movement along circular paths
  - Fish swimming animation
  - Color gradient transitions
  - Text fade-in/fade-out effects
- **Lighting and Colors**: Dynamic sky color changes, celestial lighting
- **User Input Handling**: Keyboard and mouse interaction
- **Transparency Effects**: Translucent water rendering, text fading
- **Text Rendering**: Dynamic text display with letter-by-letter animation

## Performance Considerations

- Resolution-independent rendering
- Efficient use of OpenGL 3.3+ features
- Optimized texture loading and management
- Smooth animation at monitor refresh rate

## Notes

This is an educational project created to demonstrate advanced graphics programming concepts using OpenGL and modern C++ practices.
