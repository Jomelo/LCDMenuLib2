// ============================================================                                                            
// Example:     LCDML_serialmonitor
// ============================================================ 
// Descripton: 
// This example includes the complete functionality over some 
// tabs. All Tabs which are started with "LCDML_display_.." 
// generates an output on the display / console / .... 
// This example is for the autor to test the complete functionality
// ============================================================ 

// ********************************************************************* 
// includes
// *********************************************************************
  #include <LCDMenuLib2.h>
  
// ********************************************************************* 
// LCDML display settings
// *********************************************************************
  // settings for lcd 
  #define _LCDML_DISP_cols  20
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

  // LCDML_add(id, prev_layer, new_num, lang_char_array, callback_function)
  
  LCDML_add         (0  , LCDML_0         , 1  , "Information"      , mFunc_information);       // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (1  , LCDML_0         , 2  , "Time info"        , mFunc_timer_info);        // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (2  , LCDML_0         , 3  , "Programme"        , NULL);                    // NULL = no menu function    
  LCDML_add         (3  , LCDML_0_3       , 1  , "Program 1"        , NULL);                    // NULL = no menu function  
  LCDML_add         (4  , LCDML_0_3_1     , 1  , "P1 dummy"         , NULL);                    // NULL = no menu function  
  LCDML_add         (5  , LCDML_0_3_1     , 2  , "P1 Settings"      , NULL);                    // NULL = no menu function  
  LCDML_add         (6  , LCDML_0_3_1_2   , 1  , "Warm"             , NULL);                    // NULL = no menu function  
  LCDML_add         (7  , LCDML_0_3_1_2   , 2  , "Cold"             , NULL);                    // NULL = no menu function  
  LCDML_add         (8  , LCDML_0_3_1_2   , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab 
  LCDML_add         (9  , LCDML_0_3_1     , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (10 , LCDML_0_3       , 2  , "Program 2"        , mFunc_p2);                // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (11 , LCDML_0_3       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (12 , LCDML_0         , 4  , "special"          , NULL);                    // NULL = no menu function 
  LCDML_add         (13 , LCDML_0_4       , 1  , "go to Root"       , mFunc_goToRootMenu);      // this menu function can be found on "LCDML_display_menuFunction" tab 
  LCDML_add         (14 , LCDML_0_4       , 2  , "jump to Time info", mFunc_jumpTo_timer_info); // this menu function can be found on "LCDML_display_menuFunction" tab 
  LCDML_add         (15 , LCDML_0_4       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab   
  LCDML_add         (16 , LCDML_0         , 5  , "Dummy B"          , NULL);                    // NULL = no menu function
  LCDML_add         (17 , LCDML_0         , 6  , "Screemsaver"      , mFunc_screensaver);       // this menu function can be found on "LCDML_display_menuFunction" tab 
  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu elment.
  
  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    17  
  
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
    
    // Enable Menü Rollover 
    //LCDML.MENU_enRollover();

    // Enable Screensaver (screensaver menu function, time to activate in ms)
    LCDML.SCREEN_enable(mFunc_screensaver, 10000); // set to 10 secounds 
    //LCDML.SCREEN_disable();

    // Some needfull methods 
    
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

// *********************************************************************
// check some errors - do not change here anything
// *********************************************************************
# if(_LCDML_DISP_rows > _LCDML_DISP_cfg_max_rows)
# error change value of _LCDML_DISP_cfg_max_rows in LCDMenuLib2.h
# endif
