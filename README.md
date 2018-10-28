# PicrossPuzzleExporter

## Overview

Converts an image into a Picross (Nonogram) Puzzle. Supports various Picross Puzzle Formats including homemade types involving color.  Also includes the ability to validate if a solution to a puzzle is unique, and the ability to export the data as a QR code or a protobuf.

## Features

* Ability to load an image and have it automatically generate a picross puzzle from it.
* 4 Puzzle Modes supported
  1. Classic Black and White Picross Puzzles.
  2. Grayscale Picross Puzzles.
      * These puzzles have the traditional setup, but each cell requires multiple taps to get correct. Numbers to the right and bottom indicate the total number of taps for the row/column respectively.
  3. RGB (Colors of Light) Picross Puzzles.
      * These puzzles are split up into 3 separate picross puzzles for each layer (Red, Green, and Blue).  Solving all three puzzles reveals a colored image.  If a cell is filled in multiple layers then the colors will combine in the final image (ex. red + blue = magneta, blue + green = cyan, etc.). Taps per cell can also be enabled for more colorful puzzles.
  4. RBY-BW (Painting) Picross Puzzles.
      * These puzzles are split up into 5 separate picross puzzles for each layer (Red, Blue, Yellow, White, and Black).  Solving all five puzzles reveals a colored image.  If a cell is filled in multiple layers then the colors will combine in the final image (ex. red + blue = purple, white + blue + yellow = light green, etc).
* Ability to export to a proprietary format protobuf or QRcode for playback (currently no player exists).
* Ability to validate a picross puzzle to see if the solution is unique.
* Ability to edit the puzzle on the fly.

## Screenshots

|![Black/White](https://github.com/TricksterGuy/PicrossPuzzleExporter/blob/master/screenshots/bwpika.png)   |![Grayccale](https://github.com/TricksterGuy/PicrossPuzzleExporter/blob/master/screenshots/graypika.png)|
|---|---|
|![RGB](https://github.com/TricksterGuy/PicrossPuzzleExporter/blob/master/screenshots/rgbpika.png)|![RBY](https://github.com/TricksterGuy/PicrossPuzzleExporter/blob/master/screenshots/rbypika.png)|

