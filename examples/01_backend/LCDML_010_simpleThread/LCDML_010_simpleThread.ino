// ============================================================
// Example:     LCDML_simpleThread example
// ============================================================
// Description:
// This example includes the complete functionality over some
// tabs. All Tabs which are started with "LCDML_display_.."
// generates an output on the display / console / ....
// This example is for the author to test the complete functionality
//
// The simpleThread Library provides a method to to run more then 
// one loop function. The loop functions are called with a priority 
// and a time interval. The lib is not using interrupts.
// When a function is called it runs until the function end is reached. 
//
// Download simple thread:
// https://github.com/Jomelo/simpleThread
// 
// documentation (german):
// https://forum.arduino.cc/index.php?topic=165552.0
// 
// ============================================================

// *********************************************************************
// includes
// *********************************************************************
  #include <LCDMenuLib2.h>
  #include <simpleThread.h>

// *********************************************************************
// LCDML display settings
// *********************************************************************
  // settings for LCD
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
  LCDML_add         (3  , LCDML_0_3       , 1  , "Blink Thread"     , NULL);                    // NULL = no menu function
  LCDML_add         (4  , LCDML_0_3_1     , 1  , "Start Thread"     , mFunc_thread_start);      // Thread control (start)
  LCDML_add         (5  , LCDML_0_3_1     , 2  , "Stop Thread"      , mFunc_thread_stop);       // Thread control (stop)
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
  // LCDMenuLib_add(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (16 , LCDML_0         , 5  , NULL,          "Parameter"      , NULL,                0,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_addAdvanced (17 , LCDML_0_5       , 1  , NULL,          "Parameter 1"      , mFunc_para,       10,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_addAdvanced (18 , LCDML_0_5       , 2  , NULL,          "Parameter 2"      , mFunc_para,       20,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_addAdvanced (19 , LCDML_0_5       , 3  , NULL,          "Parameter 3"      , mFunc_para,       30,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_add         (20 , LCDML_0_5       , 4  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab


  // Example for dynamic content
  // 1. set the string to ""
  // 2. use type  _LCDML_TYPE_dynParam   instead of    _LCDML_TYPE_default
  // this function type can not be used in combination with different parameters
  // LCDMenuLib_add(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (21 , LCDML_0         , 6  , NULL,          ""                  , mDyn_para,                0,   _LCDML_TYPE_dynParam);                     // NULL = no menu function

  // Example for conditions (for example for a screensaver)
  // 1. define a condition as a function of a boolean type -> return false = not displayed, return true = displayed
  // 2. set the function name as callback (remove the braces '()' it gives bad errors)
  // LCDMenuLib_add(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (22 , LCDML_0         , 7  , COND_hide,  "screensaver"        , mFunc_screensaver,        0,   _LCDML_TYPE_default);       // this menu function can be found on "LCDML_display_menuFunction" tab

  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    22

  // create menu
  LCDML_createMenu(_LCDML_DISP_cnt);


// *********************************************************************
// Simple Thread configuration
// *********************************************************************
  // define the thread number _sT_cnt_1 => 1,  _sT_cnt_2 => 2, ....  
  #define _sT_cnt         _sT_cnt_3

  // initialisation
  simpleThread_init(_sT_cnt);
    
  /* create a new thread
   * - params for "timebased_static" and "timebased_dynamic" (dynamic: the runtime can be changed)
   *    param 1: priority (unique id) (the first is "_sT_P1", next one "_sT_P2", ...) 
   *    param 2: time mode: "_sT_millis" or "_sT_micros"
   *    param 3: waittime as unsigned long ( 50UL or something else with UL at the end)
   *    param 4: the current state of a thread "_sT_start" or "_sT_stop" or "_sT_stable" 
   *             ("_sT_stable" => a stable state function is called) 
   *    param 5: callback function name
   *             
   * - params for "eventbased"            
   *    param 1: param 1: priority (unique id) (the first is "_sT_P1", next one "_sT_P2", ...) 
   *    param 2: callback function name
   */
  simpleThread_new_timebased_static   (_sT_P1  , _sT_micros, 0       , _sT_start , Thread_input_check);  
  simpleThread_new_timebased_static   (_sT_P2  , _sT_millis, 1000UL  , _sT_stop  , Thread_Serial_Blink_Example);
  simpleThread_new_timebased_static   (_sT_P3  , _sT_millis, 100UL   , _sT_start , Thread_LCDMenuLib);

  /* simple thread control makros reference
   *  initialisation:
   *  ===============
   *  void      simpleThread_init( number of threads )
   *  void      simpleThread_new_timebased_static(unique_id, time_mode, wait_time, current_state, callback_function)
   *  void      simpleThread_new_timebased_dynamic(unique_id, time_mode, wait_time, current_state, callback_function)
   *  void      simpleThread_new_eventbased(unique_id, callback_function)
   *  
   *  setup initialisation:
   *  ===============
   *  void      simpleThread_initSetup(_sT_cnt); // do no change something here
   *  
   *  loop initialisation:
   *  ===============
   *  void      simpleThread_run(_sT_priority);  
   *  or
   *  void      simpleThread_run(_sT_no_priority); // the return value of simpleThread_loop is set to true
   *  
   *  create a thread function: 
   *  ===============
   *  void      simpleThread_setup(callback_function)
   *  boolean   simpleThread_loop(callback_function)    // return true  =>  go to the next function (use this when the
   *                                                                        wait_time is 0 to call the other threads
   *                                                    // return false =>  go to the begin of the thread list
   *  void      simpleThread_stable(callback_function)
   *  
   *  thread handling:
   *  ===============
   *  void      simpleThread_start(callback_function) 
   *  void      simpleThread_stop(callback_function)
   *  void      simpleThread_reset(callback_function)           // setup function is called on the next start
   *  void      simpleThread_restart(callback_function)         // reset and start
   *  
   *  void      simpleThread_event_start(callback_function)
   *  void      simpleThread_event_reset(callback_function)     // setup function is called on the next start 
   *  void      simpleThread_event_restart(callback_function)   // reset and start
   *  
   *  call a thread directly:
   *  ===============
   *  void      simpleThread_call(callback_function)
   *  
   *  call a part of a thread directly
   *  ===============
   *  void      simpleThread_call_setup(callback_function)
   *  boolean   simpleThread_call_loop(callback_function)
   *  void      simpleThread_call_stable(callback_function)
   *  
   */

// *********************************************************************
// SETUP
// *********************************************************************
  void setup()
  {
    // serial init; only be needed if serial control is used
    Serial.begin(9600);                // start serial
    Serial.println(F(_LCDML_VERSION)); // only for examples

    simpleThread_initSetup(_sT_cnt); 

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
    simpleThread_run(_sT_priority);
  }