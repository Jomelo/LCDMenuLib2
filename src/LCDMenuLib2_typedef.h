/* ******************************************************************************
 *                        LCDMenuLib2 (LCDML)
 * ******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) [2017] [Nils Feldkämper]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ******************************************************************************
 *
 * BUG / ISSUES REPORTING
 *    https://github.com/Jomelo/LCDMenuLib2/issues
 *
 * ARDUINO FORUM
 *    http://forum.arduino.cc/index.php?topic=73816.0
 *
 * ******************************************************************************
 */

#ifndef _LCDML_typedef_h
    #define _LCDML_typedef_h

    typedef void    (* LCDML_FuncPtr)       ();
    typedef void    (* LCDML_FuncPtr_pu8)   (uint8_t);  // with parameter u8
    typedef boolean (* LCDML_FuncPtr_rb)    ();         // with return value boolean

#endif
