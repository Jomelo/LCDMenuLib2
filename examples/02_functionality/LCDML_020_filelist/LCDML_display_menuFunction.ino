/* ===================================================================== *
 *                                                                       *
 * Menu Callback Function                                                *
 *                                                                       *
 * ===================================================================== *
 *
 * EXAMPLE CODE:

// *********************************************************************
void your_function_name(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    //LCDML_UNUSED(param);
    // setup
    // is called only if it is started

    // starts a trigger event for the loop function every 100 milliseconds
    LCDML.FUNC_setLoopInterval(100);

    // uncomment this line when the menu should go back to the last called position
    // this could be a cursor position or the an active menu function
    // GBA means => go back advanced
    //LCDML.FUNC_setGBA() 

    //
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop
    // is called when it is triggered
    // - with LCDML_DISP_triggerMenu( milliseconds )
    // - with every button or event status change

    // uncomment this line when the screensaver should not be called when this function is running
    // reset screensaver timer
    //LCDML.SCREEN_resetTimer();

    // check if any button is pressed (enter, up, down, left, right)
    if(LCDML.BT_checkAny()) {
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // loop end
    // you can here reset some global vars or delete it
    // this function is always called when the functions ends.
    // this means when you are calling a jumpTo ore a goRoot function
    // that this part is called before a function is closed
  }
}


 * ===================================================================== *
 */




// this needs some bytes in ram
#define MAX_FILES_IN_LIST 20

// scroll row position
uint8_t scroll_row;
// cursor real position on lcd lines (0 - _LCDML_DISP_rows - 1)
uint8_t cursor_real_pos;
// max filelist count
uint8_t current_max_list_count;
// cursor position to file list entry
uint8_t cursor_position_cur;
// filelist
static char filelist[MAX_FILES_IN_LIST][20];

// comment this line out when you are using a display like 20x4 or 16x4
#define _LCDML_USE_SERIAL_MONITOR

// *********************************************************************
void mFunc_filelist(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup
    // is called only if it is started

    // use this to init some vars 
  
    // set max file counter
    current_max_list_count = 0;
    // set current cursor position
    cursor_position_cur = 0;
    // 
    scroll_row = 0;
    cursor_real_pos = 0;
  
  
    // for example read sd card files here 
   
    // for example here only with a dummy list
    // with 12 dummyfiles
    // subfolders are not supported
  
    // generate file names
    for(uint8_t i = 0; i<13; i++) {
      if(i < MAX_FILES_IN_LIST) { 
        // copy dummy string to new position
        memcpy(filelist[i], "File   .txt", 11);
        
        if(i<10) {                    // filenames < 10
          filelist[i][5] = 0+48;      // number + '0'     '0' = 48
          filelist[i][6] = i+48;      // number
        } else {                      // filenames >= 10
          filelist[i][5] = i/10+48;  
          filelist[i][6] = i%10+48;
        }      
        current_max_list_count++;     
      }
    } 
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop
    // is called when it is triggered
    // - with LCDML_DISP_triggerMenu( milliseconds )
    // - with every button status change
   
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function
  
    // init some vars for scrollbar
    uint8_t n_max             = (current_max_list_count >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (current_max_list_count);;
    uint8_t scrollbar_min     = 0;
    uint8_t scrollbar_max     = current_max_list_count;
    //uint8_t scrollbar_cur_pos = cursor_position_cur;
    uint8_t scroll_pos        = ((1.*n_max * _LCDML_DISP_rows) / (scrollbar_max - 1) * cursor_position_cur);
      
    if(LCDML.BT_checkUp())
    {
      // scroll up   
      if(cursor_position_cur > 0) { // check minimum cursor
        cursor_position_cur--;      // scroll up
        if(cursor_real_pos == 0) {  
          scroll_row--;             // scroll display rows
        }      
      }
    }

    if(LCDML.BT_checkDown())
    {
      // scroll down
      if(cursor_position_cur < (current_max_list_count-1)) {    // check list end   
        cursor_position_cur++;                                  // go to next element
        if(cursor_real_pos == (_LCDML_DISP_rows-1)) {           // check if current cursor is in last display line
          scroll_row++;                                         // scroll content
        } 
      }
    }

  
    // clear display
    // ================
    lcdml_menu_clear();
    
    // display content
    // ==================
    for (uint8_t n = scroll_row; n < (scroll_row+_LCDML_DISP_rows); n++)
    { 
      #ifndef _LCDML_USE_SERIAL_MONITOR      
        // set cursor
        lcd.setCursor(1, n-scroll_row);
        // set content
        lcd.print(filelist[n]);
      #else
        if (n == cursor_position_cur) 
        {
          Serial.print(F("->"));
          cursor_real_pos = n-scroll_row;
        }
        else
        {
          Serial.print(F("  "));
        }
        Serial.println(filelist[n]);
      #endif      
    }
  
    // set cursor and scrollbar
    // =============================
    #ifndef _LCDML_USE_SERIAL_MONITOR
      for (uint8_t n = scroll_row; n <(scroll_row+_LCDML_DISP_rows); n++)
      {
        lcd.setCursor(0, n-scroll_row);
          
        // set cursor
        // =====================
        if (n == cursor_position_cur) {
          lcd.write(_LCDML_DISP_cfg_cursor);
          cursor_real_pos = n-scroll_row;
        } else {
          lcd.write(' ');
        }    
    
        // display scrollbar
        // ==============================
        // delete or reset scrollbar
        if (scrollbar_max > n_max) {
          lcd.setCursor((_LCDML_DISP_cols - 1), n-scroll_row);
          lcd.write((uint8_t)0);
        }
        else {
          lcd.setCursor((_LCDML_DISP_cols - 1), n-scroll_row);
          lcd.print(' ');
        }
    
        // set scrollbar
        if (scrollbar_max > n_max) {
          //set scroll position
          if (cursor_position_cur == scrollbar_min) {
            // min pos
            lcd.setCursor((_LCDML_DISP_cols - 1), 0);
            lcd.write((uint8_t)1);
          } else if (cursor_position_cur == (scrollbar_max - 1)) {
            // max pos
            lcd.setCursor((_LCDML_DISP_cols - 1), (n_max - 1));
            lcd.write((uint8_t)4);
          } else {
            // between
            lcd.setCursor((_LCDML_DISP_cols - 1), scroll_pos / n_max);
            lcd.write((uint8_t)(scroll_pos % n_max) + 1);
          }
        }    
      }
    #endif

   

    if(LCDML.BT_checkEnter())
    {
      Serial.println(filelist[cursor_position_cur]);
      LCDML.FUNC_goBackToMenu();      
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // loop end
    // you can here reset some global vars or delete it
  }
}





// *********************************************************************
void mFunc_information(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    Serial.println(F("==========================================="));
    Serial.println(F("================  FUNC ===================="));
    Serial.println(F("==========================================="));

    Serial.println(F("To close this"));
    Serial.println(F("function press"));
    Serial.println(F("any button or use"));
    Serial.println(F("back button"));
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if(LCDML.BT_checkAny()) { // check if any button is pressed (enter, up, down, left, right)
      // LCDML_goToMenu stops a running menu function and goes to the menu
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
uint8_t g_func_timer_info = 0;  // time counter (global variable)
unsigned long g_timer_1 = 0;    // timer variable (global variable)
void mFunc_timer_info(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    Serial.println(F("==========================================="));
    Serial.println(F("================  FUNC ===================="));
    Serial.println(F("==========================================="));
    Serial.println(F("wait 20 seconds or press back button"));
    g_func_timer_info = 20;       // reset and set timer
    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds

    LCDML.TIMER_msReset(g_timer_1);
  }


  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function

    // reset screensaver timer
    LCDML.SCREEN_resetTimer();

    // this function is called every 100 milliseconds

    // this method checks every 1000 milliseconds if it is called
    if(LCDML.TIMER_ms(g_timer_1, 1000)) {
      g_func_timer_info--;                // increment the value every second
      Serial.println(g_func_timer_info);  // print the time counter value
    }

    // this function can only be ended when quit button is pressed or the time is over
    // check if the function ends normally
    if (g_func_timer_info <= 0)
    {
      // leave this function
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
uint8_t g_button_value = 0; // button value counter (global variable)
void mFunc_p2(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    // print LCD content
    Serial.println(F("==========================================="));
    Serial.println(F("================  FUNC ===================="));
    Serial.println(F("==========================================="));
    Serial.println(F("press a or w"));
    Serial.println(F("count: 0 of 3"));
    // Reset Button Value
    g_button_value = 0;

    // Disable the screensaver for this function until it is closed
    LCDML.FUNC_disableScreensaver();

  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      if (LCDML.BT_checkLeft() || LCDML.BT_checkUp()) // check if button left is pressed
      {
        LCDML.BT_resetLeft(); // reset the left button
        LCDML.BT_resetUp(); // reset the left button
        g_button_value++;

        // update LCD content
        Serial.print(F("count: "));
        Serial.print(g_button_value); //print change content
        Serial.println(F(" of 3"));
      }
    }

    // check if button count is three
    if (g_button_value >= 3) {
      LCDML.FUNC_goBackToMenu();      // leave this function
    }
  }

  if(LCDML.FUNC_close())     // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
void mFunc_back(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // end function and go an layer back
    LCDML.FUNC_goBackToMenu(1);      // leave this function
  }
}


// *********************************************************************
void mFunc_screensaver(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    Serial.println(F("start screensaver"));
    Serial.println(F("press any key to quit"));
    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop())
  {
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())
  {
     LCDML.MENU_goRoot();
  }
}


// *********************************************************************
void mFunc_goToRootMenu(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // go to root and display menu
    LCDML.MENU_goRoot();
  }
}


// *********************************************************************
void mFunc_jumpTo_timer_info(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    LCDML.OTHER_jumpToFunc(mFunc_timer_info);
  }
}


// *********************************************************************
void mFunc_para(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    Serial.println(F("==========================================="));
    Serial.println(F("================  FUNC ===================="));
    Serial.println(F("==========================================="));
    Serial.print(F("parameter: "));
    Serial.println(param);

    Serial.println(F("press any key"));
    Serial.println(F("to leave it"));

    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop())          // ****** LOOP *********
  {
    // For example
    switch(param)
    {
      case 10:
        // do something
        break;

      case 20:
        // do something
        break;

      case 30:
        // do something
        break;

      default:
        // do nothing
        break;
    }


    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
  }

  if(LCDML.FUNC_close())        // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}
