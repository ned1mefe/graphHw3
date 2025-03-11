# TetrisGL - 3D Tetris with OpenGL

## Middle East Technical University  
**Department of Computer Engineering**  
**CENG 477 - Introduction to Computer Graphics**  
**Fall 2024 - Assignment 3**

## Overview
TetrisGL is a 3D adaptation of the classic Tetris game, developed using OpenGL and GLSL shaders. This project is an implementation for the CENG 477 course at METU, focusing on hardware-accelerated graphics programming. The game utilizes modern OpenGL techniques, including Blinn-Phong shading and smooth camera transformations, to provide an interactive 3D experience.

## Features
- **3D Tetris Gameplay:** A single 3×3 block falls step-by-step towards a 9×9 board.
- **User Controls:**
  - `A`: Move block left (relative to current view)
  - `D`: Move block right (relative to current view)
  - `S`: Speed up block fall
  - `W`: Slow down block fall
  - `H`: Rotate the camera left (90° smooth rotation)
  - `K`: Rotate the camera right (90° smooth rotation)
- **Lighting and Shading:** Uses ambient, diffuse, and Blinn-Phong shading models.
- **Collision Handling:** Ensures blocks land properly and do not overlap.
- **Score System:** Tracks completed layers and updates points accordingly.
- **Game Over Condition:** The game ends if a new block cannot spawn due to occupied space.
- **Smooth Camera Rotations:** Players can rotate the game area for a different perspective.
- **Graphical Enhancements:** Visible grid lines for better gameplay visualization.

## Installation and Execution
### Requirements
- C++ compiler (compatible with OpenGL & GLSL 3.3+)
- OpenGL, GLEW, GLFW, and GLM installed

### Compilation & Execution
```sh
# Extract files
tar -xf tetrisGL.tar.gz

# Compile
make

# Run the game
./tetrisGL
```

## Development Details
- **Graphics Pipeline:** Uses forward rendering, where each frame is rendered from scratch.
- **Lighting Setup:** Includes an ambient light and at least one point light for proper scene illumination.
- **Game Logic:** Step-by-step block motion, collision detection, and scoring implemented.
- **Camera Rotation:** Controlled by adjusting the view matrix dynamically.

## Additional Notes
- **Game Design Considerations:** Step-by-step block falling ensures clear gameplay.
- **Bonus Features:** Possible expansions include new block shapes, advanced visual effects, or extra game mechanics.
- **Testing Environment:** Developed and tested on METU’s `inek` machines to ensure compatibility.

## Submission Guidelines
- The project is packaged as `tetrisGL.tar.gz`.
- It must compile and run without errors on `inek` machines.
- Submission via ODTUClass, following the standard rules.
- Late submissions and plagiarism will result in penalties.

## License
This project was developed as an educational assignment for **CENG 477** and is not intended for commercial use.

