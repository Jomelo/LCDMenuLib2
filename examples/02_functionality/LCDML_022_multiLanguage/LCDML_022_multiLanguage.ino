// ============================================================
// Example:     LCDML_multilanguage
// ============================================================
// Description:
// This example includes the complete functionality over some
// tabs. All Tabs which are started with "LCDML_display_.."
// generates an output on the display / console / ....
// This example is for the author to test the complete functionality
//
// !!!!
// Here are some diffcult parts, when something is not working in your code 
// look in this example and please read the comments
// !!!!
// ============================================================

// *********************************************************************
// includes
// *********************************************************************
  #include <LCDMenuLib2.h>

// *********************************************************************
// LCDML display settings
// *********************************************************************
  // settings for LCD
  #define _LCDML_DISP_cols  30
  #define _LCDML_DISP_rows  4

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
  LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);

// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // LCDML_0        => layer 0
  // LCDML_0_X      => layer 1
  // LCDML_0_X_X    => layer 2
  // LCDML_0_X_X_X  => layer 3
  // LCDML_0_...      => layer ...

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
  // create a new language with a short name
  // parameter (id from the element above, short language name, string)
  LCDML_langDef     (0, DE, "Information");
  
  LCDML_add         (1  , LCDML_0         , 2  , "Time info"        , mFunc_timer_info);        // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (1, DE, "Zeit Info");
  
  LCDML_add         (2  , LCDML_0         , 3  , "Program"          , NULL);                    // NULL = no menu function
  LCDML_langDef     (2, DE, "Programm");
  
  LCDML_add         (3  , LCDML_0_3       , 1  , "Program 1"        , NULL);                    // NULL = no menu function
  LCDML_langDef     (3, DE, "Programm 1");
  
  LCDML_add         (4  , LCDML_0_3_1     , 1  , "P1 dummy"         , NULL);                    // NULL = no menu function
  LCDML_langDef     (4, DE, "P1 Platzhalter");
  
  LCDML_add         (5  , LCDML_0_3_1     , 2  , "P1 Settings"      , NULL);                    // NULL = no menu function
  LCDML_langDef     (5, DE, "P1 Einstellungen");
  
  LCDML_add         (6  , LCDML_0_3_1_2   , 1  , "Warm"             , NULL);                    // NULL = no menu function
  LCDML_langDef     (6, DE, "Warm");
  
  LCDML_add         (7  , LCDML_0_3_1_2   , 2  , "Cold"             , NULL);                    // NULL = no menu function
  LCDML_langDef     (7, DE, "Kalt");
  
  LCDML_add         (8  , LCDML_0_3_1_2   , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (8, DE, "Zur\365ck");
  
  LCDML_add         (9  , LCDML_0_3_1     , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (9, DE, "Zur\365ck");
  
  LCDML_add         (10 , LCDML_0_3       , 2  , "Program 2"        , mFunc_p2);                // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (10, DE, "Programm 2");
  
  LCDML_add         (11 , LCDML_0_3       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (11, DE, "Zur\365ck");
  
  LCDML_add         (12 , LCDML_0         , 4  , "Special"          , NULL);                    // NULL = no menu function
  LCDML_langDef     (12, DE, "Spezial");
  
  LCDML_add         (13 , LCDML_0_4       , 1  , "Go to Root"       , mFunc_goToRootMenu);      // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (13, DE, "Geh zum Anfang");
  
  LCDML_add         (14 , LCDML_0_4       , 2  , "Jump to Time info", mFunc_jumpTo_timer_info); // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (14, DE, "Springe zu Zeit Info");
  
  LCDML_add         (15 , LCDML_0_4       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (15, DE, "Zur\365ck");

  // Advanced menu (for profit) part with more settings
  // Example for one function and different parameters
  // It is recommend to use parameters for switching settings like, (small drink, medium drink, big drink) or (200ml, 400ml, 600ml, 800ml) ...
  // the parameter change can also be released with dynParams on the next example
  // LCDMenuLib_add(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (16 , LCDML_0         , 5  , NULL,          "Parameter"      , NULL,                0,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_langDef     (16, DE, "Parameter");
  
  LCDML_addAdvanced (17 , LCDML_0_5       , 1  , NULL,          "Parameter 10"      , mFunc_para,       10,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_langDef     (17, DE, "Parameter 10");
  
  LCDML_addAdvanced (18 , LCDML_0_5       , 2  , NULL,          "Parameter 20"      , mFunc_para,       20,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_langDef     (18, DE, "Parameter 20");
  
  LCDML_addAdvanced (19 , LCDML_0_5       , 3  , NULL,          "Parameter 30"      , mFunc_para,       30,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_langDef     (19, DE, "Parameter 30");
  
  LCDML_add         (20 , LCDML_0_5       , 4  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (20, DE, "Zur\365ck");

  // Example for dynamic content
  // 1. set the string to ""
  // 2. use type  _LCDML_TYPE_dynParam   instead of    _LCDML_TYPE_default
  // this function type can not be used in combination with different parameters
  // LCDMenuLib_add(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (21 , LCDML_0         , 6  , NULL,          ""                  , mDyn_para,                0,   _LCDML_TYPE_dynParam);                     // NULL = no menu function
  LCDML_langDef     (21, DE, "");


  // Example for conditions (for example for a screensaver)
  // 1. define a condition as a function of a boolean type -> return false = not displayed, return true = displayed
  // 2. set the function name as callback (remove the braces '()' it gives bad errors)
  // LCDMenuLib_add(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (22 , LCDML_0         , 7  , COND_hide,  ""        , mFunc_screensaver,        0,   _LCDML_TYPE_default);       // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (22, DE, "");


  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    22

  // create menu
  LCDML_createMenu(_LCDML_DISP_cnt);

  // CREATE language types
  // the language definition here should not be the same like in the definition above 
  // please write here "LANG_" before the short name 
  // EN -> LANG_EN
  // DE -> LANG_DE
  // ...
  enum lang_types
  {
    LANG_DEFAULT  = 0,  // this value is every 0
    LANG_DE       = 1   // the last element have no "," at the end
    // custom elements can be added
  };

  // set the default language with the long name
  //lang_types g_lcdml_lang_select = DEFAULT_LANG;
  lang_types g_lcdml_lang_select = LANG_DE;

  // in the source code you can change the language with this line:
  // g_lcdml_lang_select = DEFAULT_LANG;
  // g_lcdml_lang_select = LANG_DE;
  // ...  

  // create custom language with short name, not with the long name   
  LCDML_createCustomLang(_LCDML_DISP_cnt, DE);

  // next step when you add a new language:
  // go into the LCDML_display_menu tab and change or add a switch case condition

  // next step:
  // change the menu function 

  

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
  }

// *********************************************************************
// LOOP
// *********************************************************************
  void loop()
  {
    LCDML.loop();
  }
