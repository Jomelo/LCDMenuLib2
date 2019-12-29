/* ******************************************************************************
 *                        LCDMenuLib2 (LCDML)
 * ******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) [2019] [Nils Feldkämper]
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

// File header
#ifndef LCDMenuLib2_h
    #define LCDMenuLib2_h

    // ####################### //
    // This following defines can be changed
    // ####################### //

    // you can change this parameters
    #define _LCDML_cfg_use_ram                   0   // set this to 1 when you will use the ram mode

    // enable debug strings (remove comments from this line)
    //#define LCDML_DBG                          1 

    // debug special method groups          // enable a flag to control the function call order
    #define LCDML_DBG_function_name_LOOP        0
    #define LCDML_DBG_function_name_MENU        0
    #define LCDML_DBG_function_name_FUNC        0
    #define LCDML_DBG_function_name_BT          0
    #define LCDML_DBG_function_name_OTHER       0
    #define LCDML_DBG_function_name_DISP        0
    #define LCDML_DBG_function_name_TIMER       0
    #define LCDML_DBG_function_name_SCREEN      0

    // debug special complex functions
    #define LCDML_DBG_search                    0

    // ####################### //
    // Do nothing change here
    // ####################### //

    // create DBG_print makro when debugging is enabled
    #ifdef LCDML_DBG
        #define DBG_print(enable, str)                  if(enable == 1) { Serial.print(str); }
        #define DBG_println(enable, str)                if(enable == 1) { Serial.println(str); }
        #define DBG_printstrln(enable, str, val, form)  if(enable == 1) { Serial.print(str); Serial.println(val, form);}
    #else
        #define DBG_print(enable, str)
        #define DBG_println(enable, str)
        #define DBG_printstrln(enable, str, val, form)
    #endif

    // Version
    #define _LCDML_VERSION                       "LCDML2 v2.1.2"

    // Include Arduino ios
    #include "Arduino.h"

    // Arduino specific settings
    #if ARDUINO >= 160
        #define _LCDMenuLib2_arduino_version         1    // for new Arduino version like 1.6.0 or higher
    #else
        #define _LCDMenuLib2_arduino_version         0    // for old Arduino version like 1.0.6 or 1.0.5
    #endif

    // ESP specific settings
    #if defined( ESP8266 ) || defined( ESP32 ) || (_LCDML_cfg_use_ram == 1)
        #define _LCDML_ESP
    #endif

    // No function constant
    #define _LCDML_NO_FUNC                                  255

    // Bit pos control flags
    #define _LCDML_REG_control_dynMenuDisplayed             7
    #define _LCDML_REG_control_free_6                       6
    #define _LCDML_REG_control_free_5                       5
    #define _LCDML_REG_control_bt_init_setup                4
    #define _LCDML_REG_control_update_direct                3
    #define _LCDML_REG_control_search_display               2
    #define _LCDML_REG_control_rollover                     1
    #define _LCDML_REG_control_disable_hidden               0

    // screensaver, jump to function, go Root, ...
    #define _LCDML_REG_special_free_7                       7
    #define _LCDML_REG_special_jumpTo_enabled               6 
    #define _LCDML_REG_special_free_5                       5 
    #define _LCDML_REG_special_free_4                       4 
    #define _LCDML_REG_special_free_3                       3 
    #define _LCDML_REG_special_goRoot                       2 
    #define _LCDML_REG_special_disable_scroll               1 
    #define _LCDML_REG_special_disable_screensaver          0   

    // menu function flags
    #define _LCDML_REG_MenuFunction_free_7                  7
    #define _LCDML_REG_MenuFunction_free_6                  6
    #define _LCDML_REG_MenuFunction_free_5                  5
    #define _LCDML_REG_MenuFunction_free_4                  4
    #define _LCDML_REG_MenuFunction_free_3                  3
    #define _LCDML_REG_MenuFunction_close_active            2
    #define _LCDML_REG_MenuFunction_setup                   1
    #define _LCDML_REG_MenuFunction_end                     0

    // button flags
    #define _LCDML_REG_button_do_not_use_1                  7
    #define _LCDML_REG_button_do_not_use_2                  6
    #define _LCDML_REG_button_quit                          5
    #define _LCDML_REG_button_enter                         4
    #define _LCDML_REG_button_up                            3
    #define _LCDML_REG_button_down                          2
    #define _LCDML_REG_button_left                          1
    #define _LCDML_REG_button_right                         0

    // display update handling
    #define _LCDML_REG_update_content                       7
    #define _LCDML_REG_update_cursor                        6
    #define _LCDML_REG_update_menu                          5
    #define _LCDML_REG_update_free_4                        4
    #define _LCDML_REG_update_free_3                        3
    #define _LCDML_REG_update_free_2                        2
    #define _LCDML_REG_update_free_1                        1
    #define _LCDML_REG_update_free_0                        0



    // Configure Arduino flash lib and load it*/
    #ifndef __PROG_TYPES_COMPAT__
        #define __PROG_TYPES_COMPAT__
    #endif

    // Include PGMSPACE
    #ifndef _LCDML_ESP
        #include <avr/pgmspace.h>
    #endif

    // Include menu class
    #include "LCDMenuLib2_typedef.h"
    #include "LCDMenuLib2_menu.h"
  
    // types for advanced menu function (this types are used by the menu element initialisation)
    #define _LCDML_TYPE_f7                      128
    #define _LCDML_TYPE_f6                      64
    #define _LCDML_TYPE_f5                      32
    #define _LCDML_TYPE_f4                      16
    #define _LCDML_TYPE_f3                      8
    #define _LCDML_TYPE_f2                      4
    #define _LCDML_TYPE_dynParam                2
    #define _LCDML_TYPE_default                 1

    // Include macros for this lib
    #include "LCDMenuLib2_macros.h"



    


//# LCD Menu Lib
//# =======================
    class LCDMenuLib2
    {
        private:
            // object pointer
            LCDMenuLib2_menu *rootMenu;
            LCDMenuLib2_menu *curMenu;
            LCDMenuLib2_menu *activMenu;

            // callback functions
            LCDML_FuncPtr       callback_menuControl;                   // a callback function which checks the input buttons
            LCDML_FuncPtr       callback_contentUpdate;                 // a callback function which contains the menu function content
            LCDML_FuncPtr       callback_contentClear;                  // a callback function which clears the display
            LCDML_FuncPtr_pu8   cb_screensaver;                         // a callback function as screensaver (a normal menu function, but a defined name)

            // jump To variables
            uint8_t             jT_mode;                                // contains the jumpTo Mode            
            uint8_t             jT_id;                                  // contains the jumpTo id
            uint8_t             jT_param;                               // contains the jumpTo param
            uint8_t             jT_paramOld;                            // contains the jumpTo param
            LCDML_FuncPtr_pu8   jT_function;                            // contains the jumpTo function

            uint8_t maxElements;

            // menu intern values
            uint8_t window_rows;                                        // the maximum rows of the current windows  (1 is the minium)
            uint8_t window_start;                                       // the window start
            
            uint8_t cursor_obj_pos;
            uint8_t cursor_pos;                                        // the current cursor position on the lcd
            uint8_t cursor_pos_abs;

            uint8_t layer;                                              // contains the current layer
            
            // variables with bitfields => bit register
            uint8_t REG_button;                                         // control flags for button actions
            uint8_t REG_control;                                        // control flags 
            uint8_t REG_MenuFunction;                                   // control flags for menu functions
            uint8_t REG_special;                                        // control flags for special function like screensaver, jumpTo, setCursorTo, goRoot, ..
            uint8_t REG_update;                                         // control flags to update the content

            // variables for handling with menu function            
            uint8_t goBackCnt;                                          // save the layer to go back

            // timer variable for the loop time of a menu function
            unsigned long menu_timer;                       
            unsigned long menu_default_time;

            // timer variable for the screensaver
            unsigned long screensaver_timer;
            unsigned long screensaver_default_time;

            // private methods
            void    MENU_goInto(void);                                              // activate the menu under the cursor
            void    MENU_goBack(void);                                              // move to the parent menu
            void    MENU_doScroll(uint8_t state);                                   // scroll the menu
            void    MENU_goMenu(LCDMenuLib2_menu &p_m, uint8_t p_back);             // go to a menu element
            uint8_t MENU_countChilds(LCDMenuLib2_menu *menu, uint8_t all=false);    // how many children exists on next layer
            void    MENU_initFirstElement(void);                                         // set counter to the first object child
            
                     
            // callback function
            void    FUNC_call(void);

            // help function to search special elements
            boolean OTHER_searchFunction(LCDMenuLib2_menu &p_m, uint8_t mode, LCDML_FuncPtr_pu8 p_search, uint8_t p_id);

        public:

            // constructor
            LCDMenuLib2(LCDMenuLib2_menu &p_r ,const uint8_t p_rows, const uint8_t p_cols, LCDML_FuncPtr contentUpdate, LCDML_FuncPtr contentClear, LCDML_FuncPtr menuControl);

            // init method
            void init(uint8_t);                                         // initialisation of the menu / reset the complete menu

            // loop methods
            void loop(void);                                            // call the loop_menu and the loop_control function
            void loop_control(void);                                    // call callback function to control the menu 
            void loop_menu(void);                                       // checks if a button or a menu function are needed and call them

            // menu methods            
            void    MENU_display(uint8_t update=0);                     // update the current menu structure but do not display it
            void    MENU_goRoot(void);                                  // set the cursor to the root element
            void    MENU_enRollover(void);                              // enable menu rollover (first to last or last to first element on a layer)
            void    MENU_disRollover(void);                             // disable the menu rollover function
            void    MENU_enScroll(void);                                // enable the scroll function (this function can be disabled on dyn content elements and when a encoder is used)
            void    MENU_disScroll(void);                               // disable the scroll function to catch the cursor to an dyn content element when a encoder is used
            void    MENU_setDynContent(void);                           // set this value when dyn content is shown on the current windows of the display                          
            uint8_t MENU_checkDynContent(void);                         // check if dyn content is displayed 
            boolean MENU_getScrollDisableStatus(void);                  // check if the scroll function is disabled
            uint8_t MENU_getLayer(void);                                // get the current layer of the menu
            uint8_t MENU_getCursorPos(void);                            // get the current cursor position (row 0, row 1, row 2, ...)
            uint8_t MENU_getCursorPosAbs(void);                         // get the current cursor postion absolute (..., row 10, row 11, ..) based on the menu structure
            uint8_t MENU_getChilds(void);                               // get the current number of childs on this layer
            uint8_t MENU_getParentID(uint8_t p_layer=0);                // get the parent id of a selected higher layer
            uint8_t MENU_getScroll(void);                               // get the current scroll value
            
            LCDMenuLib2_menu * MENU_getDisplayedObj();                  // get the objection with the current conten to display                    
            LCDMenuLib2_menu * MENU_getCurrentObj();                    // get the current child object

            // BT = button methods
            boolean BT_setup(void);                                     // check if the button initialisation was done
            void    BT_enter(void);                                     // set button enter
            void    BT_up(void);                                        // set button up
            void    BT_down(void);                                      // set button down
            void    BT_left(void);                                      // set button left
            void    BT_right(void);                                     // set button right
            void    BT_quit(void);                                      // set button quit

            void    BT_resetAll(void);                                  // reset all button states
            void    BT_resetEnter(void);                                // reset enter button state
            void    BT_resetUp(void);                                   // reset up button state
            void    BT_resetDown(void);                                 // reset down button state
            void    BT_resetLeft(void);                                 // reset left button state
            void    BT_resetRight(void);                                // reset right button state
            void    BT_resetQuit(void);                                 // reset quit button state

            boolean BT_checkAny(void);                                  // check if any button was pressed
            boolean BT_checkEnter(void);                                // check enter button
            boolean BT_checkUp(void);                                   // check up button
            boolean BT_checkDown(void);                                 // check down button
            boolean BT_checkLeft(void);                                 // check left button
            boolean BT_checkRight(void);                                // check right button
            boolean BT_checkQuit(void);                                 // check quit button

            // display methods
            void    DISP_update(void);                                  // display the content and update the menu structure 
            void    DISP_menuUpdate(void);                              // display the content but do not update the menu structure
            void    DISP_clear(void);                                   // calls the callback function to clear the display
            boolean DISP_checkMenuUpdate(void);                         // check if there is new content to display
            boolean DISP_checkMenuCursorUpdate(void);                   // check if the cursor was moved
            uint8_t DISP_getMenuContentId(uint8_t n);                   // get the id`s from a contentelement for a special line 

            // menu function methods
            boolean FUNC_setup(void);                                   // check if a menu function is called the first time to init some things
            boolean FUNC_loop(void);                                    // check if a menu function is running in a loop
            boolean FUNC_close(void);                                   // check if a menu function is closed to reach a stable state 
            void    FUNC_goBackToMenu(uint8_t e=0);                     // close the current menu function (the FUNC_close check is true when this is set)
            uint8_t FUNC_getID(void);                                   // get the ID of the current menu function
            void    FUNC_setLoopInterval(unsigned long p_t);            // set a loop intervall for the current menu function the default loop intervall is 100000000 ms
            void    FUNC_disableScreensaver(void);                      // disable the screensaver for the current menu function  
                  

            // timer methods
            boolean TIMER_ms(unsigned long &p_var, unsigned long p_t);  // a small timer based on the millis() function
            void    TIMER_msReset(unsigned long &p_var);                // reset the millis timer 
            boolean TIMER_us(unsigned long &p_var, unsigned long p_t);  // a small timer based on the micros() function
            void    TIMER_usReset(unsigned long &p_var);                // reset the micros timer

            // other methods
            void    OTHER_jumpToFunc(LCDML_FuncPtr_pu8 p_search, uint8_t p_para = 0);   // jumpTo a defined function based on the function name 
            void    OTHER_jumpToID(uint8_t p_search, uint8_t p_para = 0);               // jumpTo a defined function based on the function id
            void    OTHER_setCursorToID(uint8_t p_search);                              // set the cursor to a defined function based on the id
            void    OTHER_setCursorToFunc(LCDML_FuncPtr_pu8 p_search);                  // set the cursor to a defined function based on the function name

            // screensaver methods
            void    SCREEN_enable(LCDML_FuncPtr_pu8 p_function, unsigned long p_t);     // enable the screensaver feature 
            void    SCREEN_disable(void);                                               // disable the screensaver feature
            void    SCREEN_resetTimer(void);                                            // reset the screensaver timer 
            void    SCREEN_start(void);                                                 // start the screensaver directly
    };
#endif