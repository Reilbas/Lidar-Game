# Lidar Game
This game was made for a school project.
The instructions were to create a game that runs on Linux with... :
- C or C++ programming language
- Mouse and keyboard input management
- Reading and writing files
- Dynamic allocation
- Compile with Makefile or CMake
- Clean code with headers, constants, comments

Link to our Trello [here](https://trello.com/b/veP9xZos/lidar-game)

## Build and Run Project
Build the project : ``$ make``<br>
Launch app : ``$ ./main``<br>
Clean the project : ``$ make clean``

## Rules and Controls
### Rules :
The rules are simple, you must pick up all the objects scattered around an invisible maze. How do you find your way? Well, by using our super gun, which projects a series of dots over the walls.

### Controls :
- As you may have guessed, the **Mouse** to look around you
- **Arrow keys** to move around the map (**ZQSD** for players)
- **Left Click** to continuously project dots where you look
- **Scroll Wheel** allows you to increase or reduce dot dispersion
- **Right Click** will throw points in all directions. *handy for scanning new rooms*
- **Left Alt** will bring up the options menu

## Configuration file
The game uses the ``include/Config.h`` file to setup itself. Feel free to modify *(don't forget to recompile before launching the application)*.
### Basic :
| ConfigKey | Description | Default |
| :--- | :--- | ---: |
| <span style="color:yellow">FENETRE_LARGEUR</span> | Window width in pixels | <span style="color:cyan">1 200</span> |
| <span style="color:yellow">FENETRE_HAUTEUR</span> | Window height in pixels | <span style="color:cyan">900</span> |
| <span style="color:yellow">PLAYER_SPEED</span> | Player speed | <span style="color:cyan">5.0</span> |
| <span style="color:yellow">FOV</span> | Field of View | <span style="color:cyan">45.0</span> |
| <span style="color:yellow">WALL_HEIGHT</span> | Wall height | <span style="color:cyan">2.0</span> |
| <span style="color:yellow">MAX_GRAF</span> | Maximum number of dots on the map | <span style="color:cyan">200 000</span> |

<br>

### Left Click Scanner :
| ConfigKey | Description | Default |
| :--- | :--- | ---: |
| <span style="color:yellow">GRAF_PER_FRAME_SPRAY</span> | Number of points drawn per refresh cycle | <span style="color:cyan">25</span> |
| <span style="color:yellow">MIN_SPREAD</span> | Minimum dot spread value | <span style="color:cyan">0.1</span> |
| <span style="color:yellow">MAX_SPREAD</span> | Maximum dot dispersion value | <span style="color:cyan">0.3</span> |

<br>

### Right Click Scanner :
| ConfigKey | Description | Default |
| :--- | :--- | ---: |
| <span style="color:yellow">GRAF_PER_FRAME_BOMB</span> | Number of points drawn per refresh cycle | <span style="color:cyan">200</span> |
| <span style="color:yellow">SCAN_X_MIN</span> | Minimum angle for drawing dots on the horizontal axis | <span style="color:cyan">-179</span> |
| <span style="color:yellow">SCAN_X_MAX</span> | Maximum angle for drawing dots on the horizontal axis | <span style="color:cyan">180</span> |
| <span style="color:yellow">SCAN_Y_MIN</span> | Minimum angle for drawing dots on the vertical axis | <span style="color:cyan">-70</span> |
| <span style="color:yellow">SCAN_Y_MAX</span> | Maximum angle for drawing dots on the vertical axis | <span style="color:cyan">70</span> |
| <span style="color:yellow">GRAF_BOMB_SPACING_X</span> | Size of drawing step on horizontal axis | <span style="color:cyan">1</span> |
| <span style="color:yellow">GRAF_BOMB_SPACING_Y</span> | Size of drawing step on vertical axis | <span style="color:cyan">1</span> |

<br>

**<span style="color:#ff0000">WATCH OUR FOR :</span>**

- High values for <span style="color:yellow">GRAF_PER_FRAME_SPRAY</span> and/or <span style="color:yellow">GRAF_PER_FRAME_BOMB</span> can reduce the fluidity of play
- These expressions must be true :
    - | <span style="color:yellow">SCAN_X_MIN</span> | + | <span style="color:yellow">SCAN_X_MAX</span> | + 1 ⩽ <span style="color:#0dff00">360</span>
    - <span style="color:yellow">SCAN_X_MIN</span> % <span style="color:yellow">GRAF_BOMB_SPACING_X</span> = <span style="color:#0dff00">0</span> **and** <span style="color:yellow">SCAN_X_MAX</span> % <span style="color:yellow">GRAF_BOMB_SPACING_X</span> = <span style="color:#0dff00">0</span>
    - <span style="color:yellow">SCAN_Y_MIN</span> % <span style="color:yellow">GRAF_BOMB_SPACING_Y</span> = <span style="color:#0dff00">0</span> **and** <span style="color:yellow">SCAN_Y_MAX</span> % <span style="color:yellow">GRAF_BOMB_SPACING_Y</span> = <span style="color:#0dff00">0</span>

## Features that may come in the future
- Sliding against walls
- Add ambient sounds
- Add a monster that chases the player
