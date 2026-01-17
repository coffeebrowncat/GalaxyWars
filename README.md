GALAXY WARS
A high-performance 2D arcade shooter engine built from scratch in C++ using SFML. 

<img width="1593" height="585" alt="g" src="https://github.com/user-attachments/assets/889495d6-b8b0-4d08-b5b4-d367eb667b89" />


PROJECT OVERVIEW
	
	Galaxy Wars is a vertical-scrolling space shooter that demonstrates advanced C++ concepts without relying on high-level game engines like Unity or Unreal.
	The primary goal of this project was to implement a robust Game Engine Architecture using strict Object-Oriented Programming (OOP) principles. It features a custom physics loop, manual memory management (no std::vector), and a persistent high-score system.

KEY FEATURES

	Polymorphic Enemy AI: Dynamic handling of multiple enemy types (Aliens, Asteroids, Bosses) using a single base pointer array.
	Finite State Machine (FSM): Modular game states including Menu, Gameplay, Pause, and Dynamic Game Over screens.
	Manual Memory Management: Utilization of Heap allocation (new/delete) with a custom destructor to prevent memory leaks, simulating embedded system constraints.
	Persistent Data: File I/O system to load, sort (Bubble Sort), and save the top 50 high scores to scores.txt
	Difficulty Scaling: A LevelManager that dynamically adjusts spawn rates and enemy patterns based on player score.
	Custom Assets: All sprites and UI elements were designed from scratch with a "Retro-Futurist" aesthetic.

TECHNICAL IMPLEMENTATION

	Core OOP Concepts:
		This project bridges the gap between theory and application by implementing:
			Polymorphism: The spawnEnemy() function dynamically allocates Alien or Asteroid objects into an Enemy* array. The game loop uses virtual functions to execute specific logic (e.g., strafing vs. rotating) at runtime.
			Inheritance: Utilized a multilevel hierarchy: Entity → Enemy → Boss to maximize code reuse for sprite rendering and collision logic.
			Encapsulation: Critical game states (Window, Player, Enemy Array) are private, accessible only through a strict public interface to ensure data integrity.
			Abstraction: The Entity and PowerUp classes are abstract base classes, enforcing a contract that all derived objects must implement their own update logic.

	Engineering Challenges Solved:
  		Memory Optimization: Implemented static texture loading to ensure asset files are loaded from the disk only once, regardless of how many entities are spawned.
			Resource Management: Created a custom Resource struct with Operator Overloading (+) to handle score arithmetic and weapon upgrades intuitively.
			Collision Accuracy: Implemented a "Hitbox Shrink" algorithm to account for sprite transparency, ensuring fair gameplay.


CONTROLS

	Key	Action:
	Arrow Keys				Move Ship (Up, Down, Left, Right)
	Spacebar				Shoot (Tap for Semi-Auto, Hold for Rapid Fire)
	P						Pause Game
	R						Restart Game (Post-Death)
	S						Save Score to File
	ESC						Return to Menu / Resume


INSTALLATION AND SETUP

	Prerequisites:
		Visual Studio 2022 (or any C++ IDE)
		SFML Library (Simple and Fast Multimedia Library)

	Build Instructions:
		Clone the repository
		
Bash

	git clone https://github.com/yourusername/GalaxyWars.git


LINK SFML

	Ensure the SFML include and lib folders are linked in your project settings.
	Copy the SFML .dll files (e.g., sfml-graphics-d-2.dll) into the directory containing your executable.

RUN
	
	Build and run GalaxyWars.sln.
