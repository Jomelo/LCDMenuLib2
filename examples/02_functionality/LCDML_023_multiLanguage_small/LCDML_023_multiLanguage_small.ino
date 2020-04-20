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
// special settings
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
  
  LCDML_add         (1  , LCDML_0         , 2  , "Change Language"        , mFunc_switch_language);        // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_langDef     (1, DE, "Sprache umstellen");
  
  LCDML_add         (2  , LCDML_0         , 3  , "Program"          , NULL);                    // NULL = no menu function
  LCDML_langDef     (2, DE, "Programm");
  
  
  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    2

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
  lang_types g_lcdml_lang_select = LANG_DEFAULT;
  //lang_types g_lcdml_lang_select = LANG_DE;

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
