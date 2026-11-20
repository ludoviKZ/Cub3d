# Cub3D

Cub3D is a small C raycasting project inspired by the early Wolfenstein 3D
rendering technique. It loads a `.cub` map, validates its layout, opens a
MiniLibX window, and renders a first-person view with wall collision and
keyboard-controlled movement.

The current version is a learning prototype: it uses a DDA raycaster and
solid colors for the ceiling, floor, and walls. Texture paths and floor/
ceiling color entries are parsed from the map, but the texture images are not
currently loaded into the renderer.

## Features

- `.cub` file parsing with required `NO`, `SO`, `WE`, `EA`, `F`, and `C` entries
- Map character and player validation
- Flood-fill based detection of open map areas
- DDA raycasting across a 60-degree field of view
- Fish-eye correction for projected wall distances
- Collision-aware movement
- Runtime adjustment of movement speed, cell size, and wall scale
- Local copies of Libft, `get_next_line`, and MiniLibX

## Requirements

The Makefile targets a Linux/X11 environment:

- `cc` or GCC
- `make`
- X11 development headers and libraries
- MiniLibX dependencies (`libxext-dev`, `libbsd-dev`, and related X11
  packages)

On Debian or Ubuntu, the MiniLibX documentation suggests:

```bash
sudo apt-get install gcc make xorg libxext-dev libbsd-dev
```

## Build and run

From the project directory:

```bash
make
./Cub3D ok.cub
```

Useful Make targets:

```bash
make clean   # Remove object files and clean local libraries
make fclean  # Also remove the executable
make re      # Clean everything and rebuild
```

The executable expects exactly one map file:

```text
Usage: ./Cub3D <map_file>
```

## Controls

| Key | Action |
| --- | --- |
| `W` | Move forward |
| `S` | Move backward |
| `A` | Move left |
| `D` | Move right |
| `I` | Rotate left |
| `O` | Rotate right |
| `[` / `]` | Decrease/increase movement step |
| `-` / `+` | Decrease/increase wall scale |
| `,` / `.` | Decrease/increase map cell size |
| `F` | Redraw the field of view |
| `Esc` | Close the window |

The controls are handled by MiniLibX key events. Movement and rotation trigger
a redraw of the scene and a new raycast.

## Map format

A map contains six configuration entries followed by the map grid:

```text
NO ./path_to_north_texture
SO ./path_to_south_texture
WE ./path_to_west_texture
EA ./path_to_east_texture
F 255,0,0
C 100,125,0

11111111111111
11000000000001
10000001000011
1000001 100001
10000001000001
1000001 1000N1
1111111 111111
```

The map uses:

- `1` for walls
- `0` for walkable space
- `N`, `S`, `E`, or `W` for the single player start position
- spaces for padding that must remain enclosed

## Project structure

- `main.c` — initialization, MiniLibX event loop, and keyboard handling
- `parsing.c` — file loading, map validation, player detection, and flood fill
- `checks.c` — configuration and map-line checks
- `player.c` — player state, movement, rotation, DDA raycasting, and 3D slices
- `draw2D.c` — primitive drawing helpers and scene redraws
- `utils.c` — matrix duplication and cleanup
- `Cub3D.h` — shared constants, structures, and function declarations
- `libft/` — reusable C library functions and `get_next_line`
- `minilibx/` — X11 windowing and pixel-drawing library

## Tools used

- **C** for low-level memory management, parsing, geometry, and rendering
- **Make** for compiling the project and its local libraries
- **MiniLibX/X11** for creating the window, receiving events, and drawing pixels
- **Libft** for string, memory, list, and formatted-output utilities
- **get_next_line** for reading map files line by line
- **Git** for tracking changes
- **GDB support** through the Makefile's `-g` compiler flag

## What was learned

- A raycaster can turn a 2D grid into a 3D-looking scene by casting one ray
  per screen column and measuring the first wall intersection.
- DDA is efficient for grid maps because each ray advances from cell boundary
  to cell boundary instead of testing every pixel.
- Fish-eye distortion must be corrected using the angle between the player's
  direction and each ray.
- Input validation is part of rendering reliability: malformed maps,
  multiple players, invalid characters, and open borders need to be rejected
  before the render loop starts.
- Flood fill is useful for detecting spaces or walkable regions that escape
  through an unclosed map boundary.
- Separating parsing, player movement, drawing, and utility code makes a
  graphics project easier to debug and extend.
- Runtime parameters are useful during development because cell size, movement
  step, and wall scale can be tuned without rebuilding.
- Ownership of allocated matrices and strings must be explicit, especially
  when parsing data is duplicated before the original matrix is freed.

## Current limitations

- Texture paths and RGB values are parsed but are not yet used to load textured
  walls or dynamic floor/ceiling colors.
- The renderer currently uses fixed window dimensions of `1024x512`.
- The Makefile is configured for Linux/X11 and may require platform-specific
  changes on macOS or Windows.
- There is no automated test suite; validation is currently performed through
  compilation and interactive map testing.
