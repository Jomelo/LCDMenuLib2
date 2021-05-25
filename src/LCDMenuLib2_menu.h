/* ******************************************************************************
 *                        LCDMenuLib2 (LCDML)
 * ******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) [2021] [Nils Feldkämper]
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

    // REG_control
    #define _LCDML_REG_MENU_CONTROL_condetion                               7
    #define _LCDML_REG_MENU_CONTROL_free_6                                  6
    #define _LCDML_REG_MENU_CONTROL_free_5                                  5
    #define _LCDML_REG_MENU_CONTROL_free_4                                  4
    #define _LCDML_REG_MENU_CONTROL_type_dynParam_enable_custom_refresh     3
    #define _LCDML_REG_MENU_CONTROL_type_dynParam_enable_sub                2
    #define _LCDML_REG_MENU_CONTROL_type_dynParam                           1
    #define _LCDML_REG_MENU_CONTROL_type_default                            0

    // types for advanced menu function (this types are used by the menu element initialisation)                       
    #define _LCDML_TYPE_dynParam_enableCustomRefresh                       10   //0b1010// setBit 1 & setBit 3
    #define _LCDML_TYPE_dynParam_enableSubMenus                             6   //0b0110// setBit 1 & setBit 2       
    #define _LCDML_TYPE_dynParam                                            2   //0b0010// setBit 1
    #define _LCDML_TYPE_default                                             1   //0b0000// setBit 0

    inline void LCDML_cb_default_function(uint8_t) { };
    inline bool LCDML_cb_default_condetion(void) { return true; };

    class LCDMenuLib2_menu
    {
        private:
            LCDMenuLib2_menu * parent;                               // Parent menu, NULL if this is the top
            LCDMenuLib2_menu * child;                                // First child menu, NULL if no children
            LCDMenuLib2_menu * sibling;                              // Next sibling menu, NULL if this is the last sibling

            LCDML_FuncPtr_pu8 cb_function;                           // Menu Function callback
            LCDML_FuncPtr_rb  cb_condition;                          // Menu Condition callback

            void setParent(LCDMenuLib2_menu &p);                     // Sets the menu's parent to p
            void addSibling(LCDMenuLib2_menu &s,LCDMenuLib2_menu &p);// Adds a sibling s with parent p.  If the menu already has a sibling, ask that sibling to add it

            uint8_t id;                                              // Name of this menu
            uint8_t param;                                           // parameter which can be set for this menu element
            uint8_t REG_control;                                     // control register

        public:
             // Constructs the menu with a name and a NULL use function (be careful calling it)
            LCDMenuLib2_menu(uint8_t p_id, uint8_t p_param, uint8_t p_configuration, LCDML_FuncPtr_pu8 p_callback_function, LCDML_FuncPtr_rb p_callback_condition);

            LCDML_FuncPtr_pu8 getCbFunction();

            void addChild(LCDMenuLib2_menu &c);                     // Adds the child c to the menu.
                                                                    // If the menu already has a child, ask the child to add it as a sibling
            LCDMenuLib2_menu * getChild(uint8_t which);             // Returns a pointer to the which'th child of this menu
            LCDMenuLib2_menu * getSibling(uint8_t howfar);          // Returns a pointer to the sibling how far siblings away from this menu
            LCDMenuLib2_menu * getParent();                         // Returns this menu's parent menu.  If no parent, returns itself

            uint8_t getID(void);                                    // return the id of a menu element
            
            bool checkCondition(void);                              // check menu display condition
            void updateCondetion(void);                             // refrech the condetion callback
            
            uint8_t getParam(void);                                 // return a parameter which is set on the initialization
            
            bool checkType_menu(void);                              // check menu type - normal menu function
            bool checkType_dynParam(void);                          // check menu type - dynamic menu function
            bool checkType_dynParam_enabledSubMenu(void);           // check menu type - dynamic menu function with sub menus
            bool checkType_dynParam_enabledCustomRefresh(void);     // check menu type - dynamic menu function with external custom refresh            

            void callback(uint8_t p);                               // call menu callback           
    };
#endif
