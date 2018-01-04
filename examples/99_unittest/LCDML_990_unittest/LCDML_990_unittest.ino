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
// global variables
// *********************************************************************
  boolean g_condetion_test_allowed = false;

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

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    35  

  // LCDMenuLib_add(id, prev_layer, new_num, condetion, lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_add         (0  , LCDML_0         , 1  , "Information"   , mFunc_information);  // a small function
  LCDML_addAdvanced (1  , LCDML_0         , 2  , NULL     ,  "Time info"         , mFunc_timer_info    ,   0, _LCDML_TYPE_default);  // a function which display a secound counter
  // test: more then display rows dynamic functions with a hidden function
  LCDML_addAdvanced (2  , LCDML_0         , 3  , NULL     ,  "DynParam"          , NULL                ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (3  , LCDML_0_3       , 1  , NULL     ,  ""                  , cFunc_s1            ,   0, _LCDML_TYPE_dynParam);       
  LCDML_addAdvanced (4  , LCDML_0_3       , 2  , NULL     ,  ""                  , cFunc_s2            ,   0, _LCDML_TYPE_dynParam); 
  LCDML_addAdvanced (5  , LCDML_0_3       , 3  , NULL     ,  ""                  , cFunc_s3            ,   0, _LCDML_TYPE_dynParam);       
  LCDML_addAdvanced (6  , LCDML_0_3       , 4  , NULL     ,  ""                  , cFunc_s4            ,   0, _LCDML_TYPE_dynParam); 
  LCDML_addAdvanced (7  , LCDML_0_3       , 5  , COND_hide,  ""                  , cFunc_s5            ,   0, _LCDML_TYPE_dynParam);       
  LCDML_addAdvanced (8  , LCDML_0_3       , 6  , NULL     ,  ""                  , cFunc_s6            ,   0, _LCDML_TYPE_dynParam);       
  LCDML_addAdvanced (9  , LCDML_0_3       , 7  , NULL     ,  "Back"              , mFunc_back          ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (10 , LCDML_0         , 4  , NULL     ,  "Program"           , NULL                ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (11 , LCDML_0_4       , 1  , NULL     ,  "Program 1"         , NULL                ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (12 , LCDML_0_4_1     , 1  , NULL     ,  "P1 start"          , NULL                ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (13 , LCDML_0_4_1     , 2  , NULL     ,  "Settings"          , NULL                ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (14 , LCDML_0_4_1_2   , 1  , NULL     ,  "Warm"              , NULL                ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (15 , LCDML_0_4_1_2   , 2  , NULL     ,  "Long"              , NULL                ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (16 , LCDML_0_4_1_2   , 3  , NULL     ,  "Back"              , mFunc_back          ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (17 , LCDML_0_4_1     , 3  , NULL     ,  "Back"              , mFunc_back          ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (18 , LCDML_0_4       , 2  , NULL     ,  "Program 2"         , mFunc_p2            ,   0, _LCDML_TYPE_default);           // a small function to press
  LCDML_addAdvanced (19 , LCDML_0_4       , 3  , NULL     ,  "Back"              , mFunc_back          ,   0, _LCDML_TYPE_default);  
  // test: hidden menu elements
  LCDML_addAdvanced (20 , LCDML_0         , 5  , COND_hide,  ""                  , mFunc_screensaver   ,   0, _LCDML_TYPE_default); 
  LCDML_addAdvanced (21 , LCDML_0         , 6  , COND_hide,  ""                  , NULL                ,   0, _LCDML_TYPE_default);  
  // test: normal buttons after hidden menus + jump functions
  LCDML_addAdvanced (22 , LCDML_0         , 7  , NULL     ,  "goRoot"            , mFunc_goRoot        ,   0, _LCDML_TYPE_default); 
  LCDML_addAdvanced (23 , LCDML_0         , 8  , NULL     ,  "special"           , NULL                ,   0, _LCDML_TYPE_default); 
  LCDML_addAdvanced (24 , LCDML_0_8       , 1  , NULL     ,  "jump to func p2"   , mFunc_jumpToFunc    ,   0, _LCDML_TYPE_default); 
  LCDML_addAdvanced (25 , LCDML_0_8       , 2  , NULL     ,  "something else"    , NULL                ,   0, _LCDML_TYPE_default);   
  // special test: a hidden menu with visible sub menus (you can only enter this menu with LCDML.OTHER_setCursorToMenu(id) or with LCDML.Other_jumpToFunc(id) 
  LCDML_addAdvanced (26 , LCDML_0         , 9  , COND_hide,  "hidden menu"       , NULL                ,   0, _LCDML_TYPE_default); 
  LCDML_addAdvanced (27 , LCDML_0_9       , 1  , NULL     ,  "visible submenu 1" , mFunc_test_hidden   ,   0, _LCDML_TYPE_default); 
  LCDML_addAdvanced (28 , LCDML_0_9       , 2  , NULL     ,  "visible submenu 2" , NULL                ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (29 , LCDML_0_9       , 3  , NULL     ,  "Back"              , NULL                ,   0, _LCDML_TYPE_default);   
  // hidden menu test with a special condetion
  LCDML_addAdvanced (30 , LCDML_0         ,10  , NULL     ,  "Test Hidden Menu"  , NULL                ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (31 , LCDML_0_10      , 1  , COND_a1  ,  "show"              , mFunc_test_complex  ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (32 , LCDML_0_10      , 2  , COND_a2  ,  "hide"              , mFunc_test_complex  ,  10, _LCDML_TYPE_default);
  LCDML_addAdvanced (33 , LCDML_0_10      , 3  , COND_a2  ,  "test 1"            , NULL                ,   0, _LCDML_TYPE_default);
  LCDML_addAdvanced (34 , LCDML_0_10      , 4  , COND_a2  ,  "test 2"            , NULL                ,   0, _LCDML_TYPE_default);
  // normal function
  LCDML_add         (35 , LCDML_0         ,11  , "B1"  , NULL ); 
  // ***TIP*** Try to update the last ID when you add an menu elment.
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

    // Enable Menü Rollover 
    LCDML.MENU_enRollover();

    // Enable Screensaver (screensaver menu function, time to activate in ms)
    //LCDML.SCREEN_enable(mFunc_screensaver, 10000); // set to 10 secounds 
    //LCDML.SCREEN_disable();

    // Test Jumpingb
    //LCDML.OTHER_jumpToFunc(mFunc_p2);           
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