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
    // setup
    // is called only if it is started

    // starts a trigger event for the loop function every 100 milliseconds
    LCDML.FUNC_setLoopInterval(100);
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop
    // is called when it is triggered
    // - with LCDML_DISP_triggerMenu( milliseconds )
    // - with every button status change

    // check if any button is pressed (enter, up, down, left, right)
    if(LCDML.BT_checkAny()) {
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // loop end
    // you can here reset some global vars or delete it
  }
}


 * ===================================================================== *
 */


// *********************************************************************
void mFunc_information(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
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
    Serial.println("start screensaver");
    Serial.println("press any key to quit");
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
