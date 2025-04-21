# Arduino LCDMenuLib2 (LCDML)
A versatile menu library for Arduino, supporting various LCD types (row displays, graphic displays, console output).

[![GitHub Issues](https://img.shields.io/github/issues/Jomelo/LCDMenuLib2)](https://github.com/Jomelo/LCDMenuLib2/issues)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

## Table of Contents
1. [Features](#features)
2. [Installation](#installation)
3. [Code reference](#lcdml-code-reference)
4. [Forum Link](#forum-link)
3. [Images](#images)
4. [License](#license)

## Features
- **Supports up to 254 menu elements** per layer.
- **Tree-based structure** (parent, child, sibling).
- Requires a minimum of **3 buttons** (up, down, enter).
- Supports **6 predefined buttons** (up, down, left, right, back/quit, enter).
- Handles **64 custom events**, such as button clicks or other triggers. This events could be combined with a special handling to open a defined menu function when the status is changed.
- **Screensaver support** after a configurable timeout.
- Displays a **scrollbar** when menu elements exceed display rows.
- Allows **dynamic parameter changes** directly from the menu.
- Separation of structural and functional level
- Setting of **special conditions** to diplay elements
- Can be used for **statemashines** with jumping function between menu elements
- Provides **examples** for various LCD libraries and functionalities.

## Installation
### Arduino IDE -> Library Installation
1. Search into the Arduino Library for "LCDMenuLib2".
2. Follow the installation instruction.
### Arduino IDE -> Installation by ZIP file
1. Download the library from the [GitHub repository](https://github.com/Jomelo/LCDMenuLib2).
2. Open the Arduino IDE.
3. Go to `Sketch > Include Library > Add .ZIP Library`.
4. Select the downloaded `.zip` file and click `Open`.

### PlatformIO
1. Add the following to your `platformio.ini` file:
   ```ini
   lib_deps =
       Jomelo/LCDMenuLib2

## LCDML Code Reference
- https://github.com/Jomelo/LCDMenuLib2/wiki/Code-Reference

## Forum-Link
Description (German):
http://forum.arduino.cc/index.php?topic=73816.0

## Images
### 20x4 LCD

![20x4 display](extras/img/20x4_1.jpg?raw=true "20x4 display")
![20x4 display](extras/img/20x4_2.jpg?raw=true "20x4 display")
![20x4 display](extras/img/20x4_3.jpg?raw=true "20x4 display")
![20x4 display](extras/img/20x4_4.jpg?raw=true "20x4 display")

### glcd with u8glib

![Graphic display](extras/img/glcd_1.jpg?raw=true "Graphic display")
![Graphic display](extras/img/glcd_2.jpg?raw=true "Graphic display")
![Graphic display](extras/img/glcd_3.jpg?raw=true "Graphic display")
![Graphic display](extras/img/glcd_4.jpg?raw=true "Graphic display")

### serial monitor for testing or programming without a connected display

![Serial Monitor](extras/img/console_1.png?raw=true "Serial Monitor")
![Serial Monitor](extras/img/console_2.png?raw=true "Serial Monitor")
![Serial Monitor](extras/img/console_3.png?raw=true "Serial Monitor")

## License
MIT License

Copyright (c) [2025] [Nils Feldkämper]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.