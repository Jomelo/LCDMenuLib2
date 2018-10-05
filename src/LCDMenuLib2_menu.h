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

#ifndef LCDMenuLib2_menu_h
#   define LCDMenuLib2_menu_h

#   include "Arduino.h"
#   include "LCDMenuLib2_typedef.h"

    #define _LCDML_menu_f7          7
    #define _LCDML_menu_f6          6
    #define _LCDML_menu_f5          5
    #define _LCDML_menu_f4          4
    #define _LCDML_menu_f3          3
    #define _LCDML_menu_f2          2
    #define _LCDML_menu_dynParam    1
    #define _LCDML_menu_default     0


    class LCDMenuLib2_menu
    {
        private:
            LCDMenuLib2_menu * parent;                               // Parent menu, NULL if this is the top
            LCDMenuLib2_menu * child;                                // First child menu, NULL if no children
            LCDMenuLib2_menu * sibling;                              // Next sibling menu, NULL if this is the last sibling

            LCDML_FuncPtr_pu8 cb_function;                          // Menu Function callback
            LCDML_FuncPtr_rb  cb_condition;                         // Menu Condition callback

            void setParent(LCDMenuLib2_menu &p);                     // Sets the menu's parent to p
            void addSibling(LCDMenuLib2_menu &s,LCDMenuLib2_menu &p); // Adds a sibling s with parent p.  If the menu already has a sibling, ask that sibling to add it

            uint8_t id;                                             // Name of this menu
            uint8_t param;
            uint8_t configuration;

        public:
             // Constructs the menu with a name and a NULL use function (be careful calling it)
            LCDMenuLib2_menu(uint8_t p_id, uint8_t p_param, uint8_t p_configuration, LCDML_FuncPtr_pu8 p_callback_function, LCDML_FuncPtr_rb p_condition_function);

            LCDML_FuncPtr_pu8 getCbFunction();

            void addChild(LCDMenuLib2_menu &c);                      // Adds the child c to the menu.
                                                                    // If the menu already has a child, ask the child to add it as a sibling
            LCDMenuLib2_menu * getChild(uint8_t which);              // Returns a pointer to the which'th child of this menu
            LCDMenuLib2_menu * getSibling(uint8_t howfar);           // Returns a pointer to the sibling how far siblings away from this menu
            LCDMenuLib2_menu * getParent();                          // Returns this menu's parent menu.  If no parent, returns itself

            uint8_t getID();                                        // return the id of a menu element
            boolean checkCondition();                                 // check menu display condition
            uint8_t getParam();                                     // return a parameter which is set on the initialization
            boolean checkType_menu();
            boolean checkType_dynParam();

            void callback(uint8_t p);                                        // call menu callback
            boolean checkCallback();                                  // check if callback can called
    };
#endif
