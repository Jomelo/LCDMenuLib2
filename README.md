Arduino LCDMenuLib2 (LCDML) with layers for any LCD Type (Row Displays, Graphic Displays, Console output)
=================================================================

https://github.com/Jomelo/LCDMenuLib2/wiki

Features:
*  max 254 menu elements
*  max 254 menu elements per layer
*  based on a tree model (parent, child, sibling)
*  max support for 6 buttons up, down, left, right, back/quit, enter
*  min 3 buttons needed up, down, enter
*  separation of structural and functional level
*  support for screensaver which is shown after x seconds
*  display a scrollbar when more menu elements in a layer then display rows
*  the menu function are only updated when a button is hit or a trigger is set
*  it is possible to set special conditions to display an element
*  it is possible to change parameter direct from the menu
*  possibility to jump from one menu element directly to another
*  examples for many different LCD libraries
*  examples for different functionality (change param, set params, ...)


Attention:
*  when you change from LCDMenuLib v2.3.4 to LCDMenuLib2 v1.0.0 or higher
** the backend system is removed from this lib use this when you need a task system: https://github.com/arkhipenko/TaskScheduler


Description (German):
http://forum.arduino.cc/index.php?topic=73816.0

Images:

* 20x4

![20x4 display](extras/img/20x4_1.jpg?raw=true "20x4 display")
![20x4 display](extras/img/20x4_2.jpg?raw=true "20x4 display")
![20x4 display](extras/img/20x4_3.jpg?raw=true "20x4 display")
![20x4 display](extras/img/20x4_4.jpg?raw=true "20x4 display")

* glcd with u8glib

![Graphic display](extras/img/glcd_1.jpg?raw=true "Graphic display")
![Graphic display](extras/img/glcd_2.jpg?raw=true "Graphic display")
![Graphic display](extras/img/glcd_3.jpg?raw=true "Graphic display")
![Graphic display](extras/img/glcd_4.jpg?raw=true "Graphic display")

* serial monitor for testing or programming without a connected display

![Serial Monitor](extras/img/console_1.png?raw=true "Serial Monitor")
![Serial Monitor](extras/img/console_2.png?raw=true "Serial Monitor")
![Serial Monitor](extras/img/console_3.png?raw=true "Serial Monitor")


MIT License

Copyright (c) [2018] [Nils Feldkämper]

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

