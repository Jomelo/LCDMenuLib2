/* ******************************************************************************
 *                        LCDMenuLib2 (LCDML)
 * ******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) [2025] [Nils Feldkämper]
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
    // These defines can be customized
    // ####################### //

    // Uncomment to enable RAM mode for dynamic content storage.
    // This is useful for devices with sufficient RAM (e.g., ESP32, ESP8266).
    //#define _LCDML_cfg_use_ram                     
    
    // Uncomment to enable PlatformIO support.
    // This resolves potential compilation issues in PlatformIO environments.
    //#define _LCDML_plattform_io_support          
    
    // Uncomment to enable support for 32-bit microcontrollers (e.g., Arduino UNO R4).
    //#define _LCDML_cfg_use_32bit_cpu              

    // Define the number of custom event callback functions supported.
    // These callbacks can be used for handling custom events like button presses.
    // Default value: 4 
    #define _LCDML_CE_cb_function_cnt           4
                                                    
    // Uncomment to enable debug output for troubleshooting.
    // Debug messages will be printed to the serial monitor.
    //#define LCDML_DBG                          1

    // Enable or disable debug output for specific function groups.
    // Set to 1 to enable debug messages for the respective group, or 0 to disable.
    #define LCDML_DBG_function_name_LOOP        0   // Debug for the main loop
    #define LCDML_DBG_function_name_MENU        0   // Debug for menu operations
    #define LCDML_DBG_function_name_FUNC        0   // Debug for menu functions
    #define LCDML_DBG_function_name_BT          0   // Debug for button handling
    #define LCDML_DBG_function_name_CE          0   // Debug for custom events
    #define LCDML_DBG_function_name_OTHER       0   // Debug for miscellaneous operations
    #define LCDML_DBG_function_name_DISP        0   // Debug for display updates
    #define LCDML_DBG_function_name_TIMER       0   // Debug for timer operations
    #define LCDML_DBG_function_name_SCREEN      0   // Debug for screensaver functionality

    // ####################### //
    // Do not modify anything below this line
    // ####################### //
    #ifdef _LCDML_plattform_io_support
        // This line resolves potential compilation issues in VS Code and PlatformIO environments.
        // It may also help with compatibility on other platforms.
        extern char* g_LCDML_DISP_lang_lcdml_table[254];
    #endif

    // Create debug print macros when debugging is enabled
    #ifdef LCDML_DBG
        // Macro for printing debug messages to the serial monitor
        #define DBG_print(enable, str)                  if(enable == 1) { Serial.print(str); }
        // Macro for printing debug messages with a newline
        #define DBG_println(enable, str)                if(enable == 1) { Serial.println(str); }
        // Macro for printing debug messages with a string, value, and format
        #define DBG_printstrln(enable, str, val, form)  if(enable == 1) { Serial.print(str); Serial.println(val, form);}
    #else
        // Define empty macros when debugging is disabled
        #define DBG_print(enable, str)
        #define DBG_println(enable, str)
        #define DBG_printstrln(enable, str, val, form)
    #endif

    // Define the library version
    #define _LCDML_VERSION                       "LCDML2 v2.2.8"

    // Macro for marking unused variables to avoid compiler warnings
    #define LCDML_UNUSED(expr) do { (void)(expr); } while (0)

    // Include the Arduino core library
    #include "Arduino.h"

    // ESP-specific settings
    #if defined( ESP8266 ) || defined( ESP32 )
        // Automatically enable RAM mode for dynamic content storage on ESP devices
        #ifndef _LCDML_cfg_use_ram
            #define _LCDML_cfg_use_ram           
        #endif
    #endif

    // Define the maximum number of supported events.
    // This value determines how many custom events (e.g., button presses) can be handled.
    #define _LCDML_CE_events_cnt                            64  

    // Define constants for "no function" and the first element ID.
    // These are used as placeholders or defaults in the menu system.
    #define _LCDML_NO_FUNC                                  255
    #define _LCDML_FIRST_ELEMENT_ID                         255

    // Bit position control flags for menu behavior and state management.
    // These flags are used internally to manage menu operations and states.
    #define _LCDML_REG_control_free_7                       7   // Reserved for future use
    #define _LCDML_REG_control_ce_init_setup                6   // Indicates if custom events are initialized
    #define _LCDML_REG_control_content_ref_is_displayed     5   // Content reference is currently displayed
    #define _LCDML_REG_control_bt_init_setup                4   // Indicates if button handling is initialized
    #define _LCDML_REG_control_update_direct                3   // Direct update of menu content
    #define _LCDML_REG_control_refresh_all_on_button_action 2   // Refresh the entire menu on button actions
    #define _LCDML_REG_control_rollover                     1   // Enable menu rollover (looping from last to first item)
    #define _LCDML_REG_control_en_use_dyn_elements_as_menu  0   // Enable dynamic elements to act as menu items

    // Special flags for screensaver, navigation, and other features.
    // These flags control specific menu behaviors and features.
    #define _LCDML_REG_special_setCursorTo                  7   // Set cursor to a specific position
    #define _LCDML_REG_special_OTHER_function_active        6   // Indicates if another function is active
    #define _LCDML_REG_special_free_5                       5   // Reserved for future use
    #define _LCDML_REG_special_free_4                       4   // Reserved for future use
    #define _LCDML_REG_special_free_3                       3   // Reserved for future use
    #define _LCDML_REG_special_goRoot                       2   // Navigate to the root menu
    #define _LCDML_REG_special_disable_scroll               1   // Disable scrolling in the menu
    #define _LCDML_REG_special_disable_screensaver          0   // Disable the screensaver

    // Menu function flags
    // These flags are used to control the behavior of menu functions.
    #define _LCDML_REG_MenuFunction_called_from_menu        7   // Indicates if the function was called from the menu
    #define _LCDML_REG_MenuFunction_free_6                  6   // Reserved for future use
    #define _LCDML_REG_MenuFunction_free_5                  5   // Reserved for future use
    #define _LCDML_REG_MenuFunction_goBackToFuncID          4   // Go back to a specific function ID
    #define _LCDML_REG_MenuFunction_setCursorToID           3   // Set the cursor to a specific menu element ID
    #define _LCDML_REG_MenuFunction_close_active            2   // Close the currently active menu function
    #define _LCDML_REG_MenuFunction_setup                   1   // Indicates if the function is in the setup phase
    #define _LCDML_REG_MenuFunction_end                     0   // Indicates if the function has ended

    // Button flags
    // These flags represent button states and actions.
    #define _LCDML_REG_button_custom_2                      7   // Custom button 2
    #define _LCDML_REG_button_custom_1                      6   // Custom button 1
    #define _LCDML_REG_button_quit                          5   // Quit button
    #define _LCDML_REG_button_right                         4   // Right navigation button
    #define _LCDML_REG_button_left                          3   // Left navigation button
    #define _LCDML_REG_button_down                          2   // Down navigation button
    #define _LCDML_REG_button_up                            1   // Up navigation button
    #define _LCDML_REG_button_enter                         0   // Enter/Select button

    // Display update handling flags
    // These flags are used to manage updates to the display content.
    #define _LCDML_REG_update_content                       7   // Update the entire content
    #define _LCDML_REG_update_cursor                        6   // Update the cursor position
    #define _LCDML_REG_update_menu                          5   // Update the menu structure
    #define _LCDML_REG_update_menu_function_content         4   // Update the content of the current menu function
    #define _LCDML_REG_update_update_dyn_content            3   // Update dynamic content
    #define _LCDML_REG_update_free_2                        2   // Reserved for future use
    #define _LCDML_REG_update_free_1                        1   // Reserved for future use
    #define _LCDML_REG_update_free_0                        0   // Reserved for future use

    // Register control flags
    // These flags are used to manage specific control states for buttons and events.
    #define _LCDML_REG_CTRL__button                         0   // Register for button control
    #define _LCDML_REG_CTRL__events                         1   // Register for event control

    // Configure Arduino flash library compatibility
    // Ensures compatibility with older Arduino versions by defining __PROG_TYPES_COMPAT__.
    #ifndef __PROG_TYPES_COMPAT__
        #define __PROG_TYPES_COMPAT__
    #endif

    // Include PGMSPACE for AVR-based microcontrollers
    // This library is used to store data in program memory (Flash) instead of RAM.
    #ifndef _LCDML_cfg_use_ram
        #include <avr/pgmspace.h>
    #endif

    // Include menu class definitions
    // These headers define the menu structure and related macros for the library.
    #include "LCDMenuLib2_typedef.h"  // Type definitions for the menu library
    #include "LCDMenuLib2_menu.h"     // Core menu class implementation

    // Include macros for the library
    // Provides utility macros used throughout the library.
    #include "LCDMenuLib2_macros.h"

    //# =======================
    //# LCD Menu Library Class
    //# =======================
    class LCDMenuLib2
    {
        private:
            // Pointer to the current menu object
            LCDMenuLib2_menu *curMenu;  // Represents the currently active menu structure

            // Callback functions
            LCDML_FuncPtr       callback_menuControl;                   // Callback function to handle button inputs
            LCDML_FuncPtr       callback_contentUpdate;                 // Callback function to update menu content
            LCDML_FuncPtr       callback_contentClear;                  // Callback function to clear the display
            LCDML_FuncPtr_pu8   cb_screensaver;                         // Callback function for the screensaver (treated as a menu function)

            LCDML_FuncPtr_pu8   ce_cb[_LCDML_CE_cb_function_cnt];       // Array of callback functions for custom events

            // Active menu values
            LCDML_FuncPtr_pu8   actMenu_cb_function;                    // Callback function for the active menu
            uint8_t             actMenu_id;                             // ID of the currently active menu
            uint8_t             actMenu_param;                          // Parameter for the active menu
            uint8_t             actMenu_lastFuncID;                     // History of the last three active menu functions
            uint8_t             actMenu_cursorPositionID;               // Current cursor position ID
            uint8_t             actMenu_lastCursorPositionID;           // Last cursor position before a new function was called
            unsigned long       actMenu_default_time;                   // Default loop time for the menu
            uint64_t            actMenu_ce_mask;                        // Mask for custom events

            // Jump-to variables
            uint8_t             jT_id;                                  // ID of the target menu to jump to
            uint8_t             jT_param;                               // Parameter for the jump-to menu
            uint8_t             jT_paramOld;                            // Previous parameter for the jump-to menu
            LCDML_FuncPtr_pu8   jT_function;                            // Function to jump to

            // Internal menu values
            uint8_t             last_element_id;                        // ID of the last menu element
            uint8_t             window_rows;                            // Maximum number of rows in the current window (minimum is 1)
            uint8_t             window_start;                           // Starting position of the window
            
            uint8_t             cursor_obj_pos;                         // Position of the cursor object
            uint8_t             cursor_pos;                             // Current cursor position on the LCD
            uint8_t             cursor_pos_abs;                         // Absolute cursor position

            uint8_t             layer;                                  // Current menu layer
            
            // Variables with bitfields (bit registers)
            uint8_t             REG_control;                            // Control flags for menu behavior
            uint8_t             REG_button;                             // Button state flags
            uint8_t             REG_MenuFunction;                       // Flags for menu function behavior
            uint8_t             REG_special;                            // Flags for special features (e.g., screensaver, jump-to)
            uint8_t             REG_update;                             // Flags for content updates
            uint64_t            REG_custom_event;                       // Flags for custom event actions

            // Variables for handling menu functions
            uint8_t             goBackCnt;                              // Counter for tracking layers to go back

            // Timer variables for menu functions
            unsigned long       menuFunction_loopTimer;                 // Timer for the loop time of a menu function

            // Timer variables for the screensaver
            unsigned long       screensaver_timer;                      // Timer for screensaver activation
            unsigned long       screensaver_default_time;               // Default timeout for the screensaver

            // Private methods
            void                MENU_goInto(void);                      // Activate the menu under the cursor
            void                MENU_goBack(void);                      // Navigate back to the parent menu
            void                MENU_doScroll(uint8_t state);           // Scroll through the menu
            uint8_t             MENU_countChilds(LCDMenuLib2_menu *menu, uint8_t all=false); // Count the number of child elements in the next layer
            void                MENU_initFirstElement(void);            // Initialize the first menu element (skip hidden elements)
            void                MENU_resetActiveMenu(void);             // Reset all active menu variables
            
            // Callback function
            void                FUNC_call(void);                        // Call a menu function

        public:

            // Constructor
            LCDMenuLib2(LCDMenuLib2_menu &p_r, const uint8_t p_rows, const uint8_t p_cols, LCDML_FuncPtr contentUpdate, LCDML_FuncPtr contentClear, LCDML_FuncPtr menuControl);

            // Initialization method
            void                init(uint8_t);                          // Initialize or reset the menu system

            // Loop methods
            void                loop(void);                             // Main loop for menu and control logic
            void                loop_control(void);                     // Handle menu control logic
            void                loop_menu(void);                        // Handle menu updates and function calls

            // Menu methods
            void                MENU_display(uint8_t update=0);         // Update the menu structure without displaying it
            void                MENU_goRoot(void);                      // Navigate to the root menu
            void                MENU_enRollover(void);                  // Enable menu rollover (loop from last to first item)
            void                MENU_disRollover(void);                 // Disable menu rollover
            void                MENU_enScroll(void);                    // Enable scrolling in the menu
            void                MENU_disScroll(void);                   // Disable scrolling in the menu
            bool                MENU_getScrollDisableStatus(void);      // Check if scrolling is disabled
            uint8_t             MENU_getLayer(void);                    // Get the current menu layer
            uint8_t             MENU_getCursorPos(void);                // Get the current cursor position (row index)
            uint8_t             MENU_getCursorPosAbs(void);             // Get the absolute cursor position
            uint8_t             MENU_getCursorObjPos(void);             // Get the cursor object position
            uint8_t             MENU_getChilds(void);                   // Get the number of child elements in the current layer
            uint8_t             MENU_getParentID(uint8_t p_layer=0);    // Get the parent ID of a specific layer
            uint8_t             MENU_getScroll(void);                   // Get the current scroll value
            uint8_t             MENU_getLastActiveFunctionID(void);     // Get the ID of the last active function
            uint8_t             MENU_getLastCursorPositionID(void);     // Get the last cursor position ID
            uint8_t             MENU_getElementIDFromCursorPos(void);   // Get the menu element ID from the current cursor position
            void                MENU_updateAllCondetions(void);         // Update all menu element conditions
            void                MENU_setDynFunctionContentUpdate(void); // Enable dynamic content updates for a menu function
            void                MENU_clearDynFunctionContentUpdate(void); // Clear dynamic content updates
            void                MENU_allCondetionRefresh(void);         // Refresh all menu conditions
            void                MENU_enUseDynElementsWithSubElements(void); // Enable subelements for dynamic menu elements
            void                MENU_disUseDynElementsWithSubElements(void); // Disable subelements for dynamic menu elements
            void                MENU_enRefreshAllOnButtonAction(void);  // Enable full menu refresh on button actions
            void                MENU_disRefreshAllOnButtonAction(void); // Disable full menu refresh on button actions
            void                MENU_setDynRContent(void);              // Enable dynamic refreshed content
            void                MENU_clearDynRContent(void);            // Disable dynamic refreshed content
            bool                MENU_checkDynRContent(void);            // Check if dynamic refreshed content is enabled

            LCDMenuLib2_menu *  MENU_getDisplayedObj(void);             // Get the object currently displayed
            LCDMenuLib2_menu *  MENU_getCurrentObj(void);               // Get the currently active menu object
            LCDMenuLib2_menu *  MENU_getRootObj(void);                  // Get the root menu object

            // Button and event methods
            bool                BT_setup(void);                         // Check if button initialization is complete
            bool                CE_setup(void);                         // Check if custom event initialization is complete
            void                CE_setOnChangeCbFunction(uint8_t p_event, LCDML_FuncPtr_pu8 p_function); // Add a callback for a custom event
            void                CE_clearOnChangeCbFunction(uint8_t p_event); // Remove a callback for a custom event

            // Register methods
            void                REG_set(uint8_t p_reg, uint8_t p_val);  // Set a register value
            void                REG_reset(uint8_t p_reg, uint8_t p_val); // Reset a specific register value
            void                REG_resetAll(uint8_t p_reg);            // Reset all values in a register
            bool                REG_check(uint8_t p_reg, uint8_t p_val); // Check if a specific value is set in a register
            bool                REG_checkAny(uint8_t p_reg);            // Check if any value is set in a register

            // Display methods
            void                DISP_update(void);                      // Update and display the menu content
            void                DISP_menuUpdate(void);                  // Update the menu content without displaying it
            void                DISP_clear(void);                       // Clear the display
            bool                DISP_checkMenuUpdate(void);             // Check if the menu content needs to be updated
            bool                DISP_checkMenuCursorUpdate(void);       // Check if the cursor position has changed
            uint8_t             DISP_getMenuContentId(uint8_t n);       // Get the content ID for a specific line

            // Menu function methods
            bool                FUNC_setup(void);                       // Check if a menu function is being set up
            bool                FUNC_loop(void);                        // Check if a menu function is running in a loop
            bool                FUNC_close(void);                       // Check if a menu function is closing
            void                FUNC_goBackToMenu(uint8_t p_goBackCnt=0); // Close the current menu function and return to the menu
            uint8_t             FUNC_getID(void);                       // Get the ID of the current menu function
            void                FUNC_setLoopInterval(unsigned long p_t); // Set the loop interval for the current menu function
            void                FUNC_disableScreensaver(void);          // Disable the screensaver for the current menu function
            void                FUNC_setGBAToLastCursorPos(void);       // Set "go back" behavior to the last cursor position
            void                FUNC_setGBAToLastFunc(void);            // Set "go back" behavior to the last function
            void                FUNC_setGBA(void);                      // Set "go back" behavior dynamically
            void                FUNC_setCEMask(unsigned long p_mask);   // Set a mask to enable specific custom events

            // Timer methods
            bool                TIMER_ms(unsigned long &p_var, unsigned long p_t); // Timer based on milliseconds
            void                TIMER_msReset(unsigned long &p_var);    // Reset the millisecond timer
            bool                TIMER_us(unsigned long &p_var, unsigned long p_t); // Timer based on microseconds
            void                TIMER_usReset(unsigned long &p_var);    // Reset the microsecond timer

            // Other methods
            void                OTHER_jumpToFunc(LCDML_FuncPtr_pu8 p_search, uint8_t p_para = 0); // Jump to a specific function by name
            void                OTHER_jumpToID(uint8_t p_search, uint8_t p_para = 0); // Jump to a specific function by ID
            void                OTHER_setCursorToID(uint8_t p_search);  // Set the cursor to a specific menu element by ID
            void                OTHER_setCursorToFunc(LCDML_FuncPtr_pu8 p_search); // Set the cursor to a specific menu element by function
            uint8_t             OTHER_getIDFromFunction(LCDML_FuncPtr_pu8 p_search); // Get the ID of a menu element by function
            void                OTHER_updateFunc(void);                 // Update a menu function directly

            // Screensaver methods
            void                SCREEN_enable(LCDML_FuncPtr_pu8 p_function, unsigned long p_t); // Enable the screensaver with a specific function and timeout
            void                SCREEN_disable(void);                   // Disable the screensaver
            void                SCREEN_resetTimer(void);                // Reset the screensaver timer
            void                SCREEN_start(void);                     // Start the screensaver immediately
    };
#endif