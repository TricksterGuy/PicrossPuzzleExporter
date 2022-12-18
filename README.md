# PicrossPuzzleExporter (WIP)

## Overview

Converts an image into a Picross (Nonogram) Puzzle. Supports various Picross Puzzle Formats including homemade types involving color.  Also includes the ability to validate if a solution to a puzzle is unique, and the ability to export the data to  [protobuf](https://developers.google.com/protocol-buffers).

## Features

* Ability to load an image and have it automatically generate a picross puzzle from it.
* 5 Puzzle Modes supported
  1. Classic Black and White Picross Puzzles.
  2. Grayscale Picross Puzzles.
      * These puzzles have the traditional setup, but each cell is not your typical marked/unmarked state, Cells can be of one of 2<sup>bpc</sup> (where bpc = bits per cell) states indicating how dark the cell is. Numbers to the right and bottom indicate the number of cells with that value along with the final value indicating how many changes in shade there was for that row (ignoring unfilled cells).
  3. Colors of Light Picross Puzzles.
      * These puzzles are split up into 3 separate picross puzzles for each layer (Red, Green, and Blue).  Solving all three puzzles reveals a colored image.  If a cell is filled in multiple layers then the colors will combine in the final image (ex. red + blue = magenta, blue + green = cyan, etc.). Bits per cell can also be enabled for more colorful puzzles.
  4. Painting Picross Puzzles.
      * These puzzles are split up into 5 separate picross puzzles for each layer (Red, Blue, Yellow, White, and Black).  Solving all five puzzles reveals a colored image.  If a cell is filled in multiple layers then the colors will combine in the final image (ex. red + blue = purple, white + blue + yellow = light green, etc).
  5. Color Hints
      * These puzzles have the traditional setup, but each hint is colored, the hints now specify continuous cells with the same color as the hint.
* Ability to export to [protobuf](https://developers.google.com/protocol-buffers) and excel spreadsheet (xlsx format).
* Ability to validate a picross puzzle to see if the solution is unique.
* Ability to edit the puzzle on the fly.

## Screenshots

|![Classic](https://github.com/TricksterGuy/PicrossPuzzleExporter/blob/master/screenshots/classic.png)   |![Grayscale](https://github.com/TricksterGuy/PicrossPuzzleExporter/blob/master/screenshots/grayscale.png)|
|---|--------------------------------------------------------------------------------------------------------|
|![RGB](https://github.com/TricksterGuy/PicrossPuzzleExporter/blob/master/screenshots/light.png)|![RBY](https://github.com/TricksterGuy/PicrossPuzzleExporter/blob/master/screenshots/painting.png)       |
|---|--------------------------------------------------------------------------------------------------------|
|![ColoredHints](https://github.com/TricksterGuy/PicrossPuzzleExporter/blob/master/screenshots/colorhints.png)|--------------------------------------------------------------------------------------------------------|


## Building

### Linux (Debian Based Systems)
 1. Install dependencies `sudo apt install cmake libprotobuf-dev libwxgtk3.0-gtk3-dev pkg-config libgtk2.0-dev protobuf-compiler`
 2. Create a build directory `mkdir build && cd build`
 3. Generate a Makefile via cmake `cmake ..`
 4. Build the program `make`
 5. Install the program `sudo make install`

Optionally for xlsx export support [libxlsxwriter](https://libxlsxwriter.github.io/getting_started.html) can be installed and the option ENABLE_XLSX can be passed when running cmake.

### Windows (MSYS2)
 1. Install dependencies `pacman -S  mingw-w64-ucrt-x86_64-wxwidgets3.2-msw mingw-w64-ucrt-x86_64-protobuf`
 2. Create a build directory `mkdir build && cd build`
 3. Generate a Makefile via cmake `cmake ..`
 4. Build the program `make`
 5. Install the program `sudo make install`
 
Additionally libxlsxwriter can be installed with package mingw-w64-ucrt-x86_64-libxlsxwriter and enabling option ENABLE_XLSX
