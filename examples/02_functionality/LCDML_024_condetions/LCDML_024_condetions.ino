// ============================================================
// Example:     LCDML_condetions
// ============================================================
// Description:
// ToDo
// ============================================================
// *********************************************************************
// special settings for non arduino basic controllers
// *********************************************************************
// enable this line when you are not usigng a standard arduino
// for example when your chip is an ESP or a STM or SAM or something else

//#define _LCDML_cfg_use_ram 

// *********************************************************************
// includes
// *********************************************************************
  #include <LCDMenuLib2.h>

// *********************************************************************
// LCDML display settings
// *********************************************************************
  // settings for LCD
  #define _LCDML_DISP_cols  20
  #define _LCDML_DISP_rows  4
  
  // enable this line (set to 1) to show a header above the first menu element
  // this function can be changed in LCDML_display_menu tab
  #define _LCDML_DSIP_use_header 0

// *********************************************************************
// Prototypes
// *********************************************************************
  void lcdml_menu_display();
  void lcdml_menu_clear();
  void lcdml_menu_control();

// *********************************************************************
// Condetions
// *********************************************************************
  // global variable for condetion
  boolean g_condetion_a = true;
// *********************************************************************
// Objects
// *********************************************************************
  LCDMenuLib2_menu LCDML_0 (255, 0, 0, NULL, NULL); // root menu element (do not change)
  LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows-_LCDML_DSIP_use_header, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);

// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // LCDML_0        => layer 0
  // LCDML_0_X      => layer 1
  // LCDML_0_X_X    => layer 2
  // LCDML_0_X_X_X  => layer 3
  // LCDML_0_...      => layer ...

  // For beginners
  // LCDML_add(id, prev_layer, new_num, lang_char_array, callback_function)
  LCDML_add         (0  , LCDML_0         , 1  , "Information"      , mFunc_information);       // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (1  , LCDML_0         , 2  , "Time info"        , mFunc_timer_info);        // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_addAdvanced (2  , LCDML_0         , 3  , COND_a, "Condetion A"      , NULL, 0, _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_add         (3  , LCDML_0_3       , 1  , "Program 1"        , NULL);                    // NULL = no menu function 
  LCDML_add         (4  , LCDML_0_3       , 2  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_addAdvanced (5  , LCDML_0         , 4  , COND_not_a, "Condetion B"      , NULL, 0, _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_add         (6  , LCDML_0_4       , 1  , "Program 2"        , NULL);                    // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (7  , LCDML_0_4       , 2  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
 
  // Example for dynamic content
  // 1. set the string to ""
  // 2. use type  _LCDML_TYPE_dynParam   instead of    _LCDML_TYPE_default
  // this function type can not be used in combination with different parameters
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (8 , LCDML_0         , 5  , NULL,          ""                  , mDyn_change_condetion,                0,   _LCDML_TYPE_dynParam);                     // NULL = no menu function

  // Example for conditions (for example for a screensaver)
  // 1. define a condition as a function of a boolean type -> return false = not displayed, return true = displayed
  // 2. set the function name as callback (remove the braces '()' it gives bad errors)
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (9 , LCDML_0         , 6  , COND_hide,  "screensaver"        , mFunc_screensaver,        0,   _LCDML_TYPE_default);       // this menu function can be found on "LCDML_display_menuFunction" tab

 
  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    9

  // create menu
  LCDML_createMenu(_LCDML_DISP_cnt);

// *********************************************************************
// SETUP
// *********************************************************************
  void setup()
  {
    // serial init; only be needed if serial control is used
    Serial.begin(9600);                // start serial
    Serial.println(F(_LCDML_VERSION)); // only for examples

    // LCDMenuLib Setup
    LCDML_setup(_LCDML_DISP_cnt);

    // Some settings which can be used

    // Enable Menu Rollover
    //LCDML.MENU_enRollover();

    // Enable Screensaver (screensaver menu function, time to activate in ms)
    //LCDML.SCREEN_enable(mFunc_screensaver, 10000); // set to 10 seconds
    //LCDML.SCREEN_disable();

    // Some needful methods

    // You can jump to a menu function from anywhere with
    //LCDML.OTHER_jumpToFunc(mFunc_p2); // the parameter is the function name

    // In the following line the new event handling system ins configured
    // you can use 0 - 3 special events when calling the LCDML.CE_set(id) function
    // when you need more events you can change in LCDMenuLib.h the number until 0-63
    //LCDML.CE_setOnChangeCbFunction(0, mFunc_exampleEventHandling);
    //LCDML.CE_setOnChangeCbFunction(1, your_special_menu_function);
    //...

  }

// *********************************************************************
// LOOP
// *********************************************************************
  void loop()
  {
    LCDML.loop();
  }