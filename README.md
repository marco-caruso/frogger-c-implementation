#Frogger Resurrection - Operating Systems Project (SOPR)

This repository contains the implementation of the videogame Frogger Resurrection, developed as the final project for the Operating Systems course (A.Y. 2024-2025). The software simulates the classic vintage arcade game, focusing on concurrency management and inter-process communication (IPC) in a Linux environment.  

##👥 Authors
The project was developed by:
- Contu Mauro (ID: 60/79/00115)
- Caruso Marco (ID: 60/79/00107)

##🛠️ Requirements & Installation
The project is designed to run on Ubuntu 22.04 LTS (64-bit). You must install the ncurses library (for terminal graphics) and SDL2 (for audio support).  To install the dependencies, run:
```bash
sudo apt update
sudo apt install libncurses5-dev libsdl2-dev
```

##🚀 Compilation and ExecutionThe project uses a Makefile to manage the build process.  Compilation:
```Bash
make
Execution:Bash./output
```

Upon execution, a splash screen will appear; press any key to enter the Main Menu.

##🎮 Game Guide
###Menu Structure
The menu offers four options selectable via arrow keys and the ENTER key:Select Difficulty: Start a new run by choosing your preferred challenge level.Controls: View a brief description of the keys used to move the frog.Rules: A description of the game mechanics and objectives.Credits: Information regarding the project developers.Objectives and MechanicsGoal: Guide the frog from the starting sidewalk to fill all 5 nests located at the top of the screen.  Obstacles: Cross the river by jumping on crocodiles (acting as rafts) while avoiding falling into the water.  Lives: You have a fixed number of lives (default: 5).  Time: Each round must be completed before the timer expires.  Combat: The frog can fire two grenades (left and right) by pressing the Spacebar to neutralize crocodile projectiles.  Difficulty and ScoringThe game starts with a base score of 200 points. Difficulty affects the speed of crocodiles and their projectiles, as well as the score balance:EventEasyMediumHardFinal Victory+1000+1500+2000Nest Closed+100+250+500Projectile Neutralized+5+5+5Life Lost-10-15-20Game Over-100-250-500Time Penalty-1 every ~6s-1 every ~6s-1 every ~6s🏗️ Technical ArchitectureThe project implements the Process-based Version as per the requirements:  Concurrency: Every dynamic object (frog, individual crocodiles, projectiles) is managed by a separate process.  Communication: A single Pipe is used to send coordinates from "producer" processes (game entities) to the "consumer" process (graphics and collision manager).  Graphics Manager: A central task redraws the ncurses matrix and verifies collisions in real-time.  Audio: Features thematic music for the intro, menu, victory, and game over screens using the SDL2 library.  📋 Submission NotesEnvironment: Fully tested and functional on the Ubuntu 22.04 Virtual Machine.  Structure: The project adheres to the required hierarchy, including source files and the Makefile.  
