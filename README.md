# Snake Game (SFML)
My first graphical game - a classic Snake implementation using SFML.

## Overview
After learning programming fundamentals, this was my first complete game with actual graphics. Built with SFML for window management, rendering, and input handling.

## Features
- Classic Snake gameplay
- Grid-based movement (60x33 grid on 1920x1080)
- Smooth controls with WASD
- Collision detection (walls and self)
- Food spawning system
- 60 FPS frame rate

## Technical Implementation
- **Movement System**: Fixed-interval movement (0.15s delay)
- **Data Structure**: Snake stored as vector of grid positions
- **Collision Detection**: Simple position checking for walls/self
- **Random Generation**: MT19937 for food placement
- **Rendering**: SFML rectangles for snake segments and food

## Controls
- **WASD**: Move snake
- **ESC**: Exit game

## Learning Progression
Originally implemented with STL vector, later refactored to use my custom Array class from the Data Structures learning project to understand memory management in a practical context.

## What This Taught Me
- Game loop structure
- Handling user input
- Basic collision detection
- Managing game state
- Working with graphics libraries

This simple project was the foundation that led to more complex projects like my physics engines and level editors.
