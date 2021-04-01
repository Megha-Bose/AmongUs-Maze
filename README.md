# AmongUs-Maze
2D maze game built with OpenGL

## Overview

This is a 2D maze game derived from the Among Us game, in the form
of a single-player variant.

## Run
- `cd src`
- `make`
- `./game`


## Details

This game’s objective is to exit the maze after finishing two tasks.

- World: A procedural maze (different maze layout on every game startup). Maze needs to have a single exit, and if the player reaches this point after completing two tasks, the player can exit the maze. The walls of the maze are visible as lines on the screen (2D).
 
- Player: A single-player (blue-suit) with some assigned health value can navigate the generated maze with the keyboard.

- HUD (Heads Up Display): A box of rendered text on the top left of the screen, present at all times, detailing the following:

	- Health: This indicates the player’s points.
		- Increases by 1 on collecting coins (yellow)
		- Decreases by 1 on encountering bombs (red)
		- Decreases by 2 on encountering imposter (red suit)

	- Tasks: Tasks completed / Total number of tasks available.

	- Time: A countdown from some X number of seconds. When the clock runs out, end the game.

- Tasks done:
	- An imposter character (enemy) exists at a random location in the maze and begins to get to your position as soon as the game starts. A button (a white 	circle drawn in the maze path) to vaporize (make the imposter disappear) the imposter exists at another random location in the maze. The task here is to 	reach the button before the imposter gets you else; you lose points, as mentioned above.
	- A button to release coins and bombs exists in a random part of the maze. The task is to reach the button and then collect the coins to increase your 		score.
	
- Characters: The player and imposter look like real Among Us characters with basic shapes like a semicircle plus a rectangle stacked for the body and another rectangle for the glass part of the helmet.
