// ============================================================
// Example:     LCDML_serialmonitor
// ============================================================
// Description:
// This example includes the complete functionality over some
// tabs. All Tabs which are started with "LCDML_display_.."
// generates an output on the display / console / ....
// This example is for the author to test the complete functionality
// ============================================================
// *********************************************************************
// special settings
// *********************************************************************
// enable this line when you want to work only with ram 
//#define _LCDML_cfg_use_ram 

// enable this line when you are using an 32 bit processor
//#define _LCDML_cfg_use_32bit_cpu

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
  LCDML_add         (2  , LCDML_0         , 3  , "Program"          , NULL);                    // NULL = no menu function
  LCDML_add         (3  , LCDML_0_3       , 1  , "Program 1"        , NULL);                    // NULL = no menu function
  LCDML_add         (4  , LCDML_0_3_1     , 1  , "P1 dummy"         , NULL);                    // NULL = no menu function
  LCDML_add         (5  , LCDML_0_3_1     , 2  , "P1 Settings"      , NULL);                    // NULL = no menu function
  LCDML_add         (6  , LCDML_0_3_1_2   , 1  , "Warm"             , NULL);                    // NULL = no menu function
  LCDML_add         (7  , LCDML_0_3_1_2   , 2  , "Cold"             , NULL);                    // NULL = no menu function
  LCDML_add         (8  , LCDML_0_3_1_2   , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (9  , LCDML_0_3_1     , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (10 , LCDML_0_3       , 2  , "Program 2"        , mFunc_p2);                // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (11 , LCDML_0_3       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (12 , LCDML_0         , 4  , "Special"          , NULL);                    // NULL = no menu function
  LCDML_add         (13 , LCDML_0_4       , 1  , "Go to Root"       , mFunc_goToRootMenu);      // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (14 , LCDML_0_4       , 2  , "Jump to Time info", mFunc_jumpTo_timer_info); // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (15 , LCDML_0_4       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab

  // Advanced menu (for profit) part with more settings
  // Example for one function and different parameters
  // It is recommend to use parameters for switching settings like, (small drink, medium drink, big drink) or (200ml, 400ml, 600ml, 800ml) ...
  // the parameter change can also be released with dynParams on the next example
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (16 , LCDML_0         , 5  , NULL,          "Parameter"      , NULL,                0,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_addAdvanced (17 , LCDML_0_5       , 1  , NULL,          "Parameter 1"      , mFunc_para,       10,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_addAdvanced (18 , LCDML_0_5       , 2  , NULL,          "Parameter 2"      , mFunc_para,       20,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_addAdvanced (19 , LCDML_0_5       , 3  , NULL,          "Parameter 3"      , mFunc_para,       30,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_add         (20 , LCDML_0_5       , 4  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab


  // Example for dynamic content
  // 1. set the string to ""
  // 2. use type  _LCDML_TYPE_dynParam   instead of    _LCDML_TYPE_default
  // this function type can not be used in combination with different parameters
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (21 , LCDML_0         , 6  , NULL,          ""                  , mDyn_para,                0,   _LCDML_TYPE_dynParam);                     // NULL = no menu function

  // Example for conditions (for example for a screensaver)
  // 1. define a condition as a function of a boolean type -> return false = not displayed, return true = displayed
  // 2. set the function name as callback (remove the braces '()' it gives bad errors)
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (22 , LCDML_0         , 7  , COND_hide,  "screensaver"        , mFunc_screensaver,        0,   _LCDML_TYPE_default);       // this menu function can be found on "LCDML_display_menuFunction" tab

  // Example function for event handling (only serial output in this example)  
  LCDML_add         (23 , LCDML_0         , 8  , "Event Handling"                 , mFunc_exampleEventHandling);  // this menu function can be found on "LCDML_display_menuFunction" tab


  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    23

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
    LCDML.SCREEN_enable(mFunc_screensaver, 10000); // set to 10 seconds
    //LCDML.SCREEN_disable();

    // Some needful methods

    // You can jump to a menu function from anywhere with
    //LCDML.OTHER_jumpToFunc(mFunc_p2); // the parameter is the function name

    // In the following line the new event handling system ins configured
    // you can use 0 - 3 special events when calling the LCDML.CE_set(id) function
    // when you need more events you can change in LCDMenuLib.h the number until 0-63
    LCDML.CE_setOnChangeCbFunction(0, mFunc_exampleEventHandling);
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