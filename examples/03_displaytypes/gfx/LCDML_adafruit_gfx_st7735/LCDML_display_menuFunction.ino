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
    // clear lcd
    display.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    // set text color / Textfarbe setzen
    display.setTextColor(_LCDML_ADAFRUIT_TEXT_COLOR);  
    // set text size / Textgroesse setzen
    display.setTextSize(_LCDML_ADAFRUIT_FONT_SIZE);
  
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 0); // line 0
    display.println(F("To close this")); 
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 1); // line 1
    display.println(F("function press")); 
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 2); // line 2
    display.println(F("any button or use")); 
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 3); // line 3
    display.println(F("back button")); 
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if(LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
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
     g_func_timer_info = 20;       // reset and set timer

    char buf[20];
    sprintf (buf, "wait %d seconds", g_func_timer_info);

    // clear lcd
    display.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    // set text color / Textfarbe setzen
    display.setTextColor(_LCDML_ADAFRUIT_TEXT_COLOR);  
    // set text size / Textgroesse setzen
    display.setTextSize(_LCDML_ADAFRUIT_FONT_SIZE);
  
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 0); // line 0
    display.println(buf); 
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 1); // line 1
    display.println(F("or press back button")); 
    
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
    if(LCDML.TIMER_ms(g_timer_1, 1000))
    {
      g_timer_1 = millis();
      g_func_timer_info--;                // increment the value every second
      char buf[20];
      sprintf (buf, "wait %d seconds", g_func_timer_info);

      // clear lcd
      display.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
      // set text color / Textfarbe setzen
      display.setTextColor(_LCDML_ADAFRUIT_TEXT_COLOR);  
      // set text size / Textgroesse setzen
      display.setTextSize(_LCDML_ADAFRUIT_FONT_SIZE);
    
      display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 0); // line 0
      display.println(buf); 
      display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 1); // line 1
      display.println(F("or press back button")); 

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
    char buf[17];
    sprintf (buf, "count: %d of 3", 0);

    // clear lcd
    display.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    // set text color / Textfarbe setzen
    display.setTextColor(_LCDML_ADAFRUIT_TEXT_COLOR);  
    // set text size / Textgroesse setzen
    display.setTextSize(_LCDML_ADAFRUIT_FONT_SIZE);
  
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 0); // line 0
    display.println(F("press a or w button")); 
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 1); // line 1
    display.println(buf); 

    // Reset Button Value
    g_button_value = 0;

    // Disable the screensaver for this function until it is closed
    LCDML.FUNC_disableScreensaver();
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function

    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      if (LCDML.BT_checkLeft() || LCDML.BT_checkUp()) // check if button left is pressed
      {
        LCDML.BT_resetLeft(); // reset the left button
        LCDML.BT_resetUp(); // reset the left button
        g_button_value++;

        // update LCD content
        char buf[20];
        sprintf (buf, "count: %d of 3", g_button_value);

         // clear lcd
        display.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
        // set text color / Textfarbe setzen
        display.setTextColor(_LCDML_ADAFRUIT_TEXT_COLOR);  
        // set text size / Textgroesse setzen
        display.setTextSize(_LCDML_ADAFRUIT_FONT_SIZE);
      
        display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 0); // line 0
        display.println(F("press a or w button")); 
        display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 1); // line 1
        display.println(buf); 
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
void mFunc_screensaver(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // setup function

    // clear lcd
    display.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    // set text color / Textfarbe setzen
    display.setTextColor(_LCDML_ADAFRUIT_TEXT_COLOR);  
    // set text size / Textgroesse setzen
    display.setTextSize(_LCDML_ADAFRUIT_FONT_SIZE);
  
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 0); // line 0
    display.println(F("screensaver")); 
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 1); // line 1
    display.println(F("press any key")); 
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 2); // line 2
    display.println(F("to leave it")); 

    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
  }

  if(LCDML.FUNC_close())          // ****** STABLE END *********
  {
    // The screensaver go to the root menu
    LCDML.MENU_goRoot();
  }
}



// *********************************************************************
void mFunc_back(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // end function and go an layer back
    LCDML.FUNC_goBackToMenu(1);      // leave this function and go a layer back
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

    char buf[20];
    sprintf (buf, "parameter: %d", param);
    
    // setup function
     // clear lcd
    display.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    // set text color / Textfarbe setzen
    display.setTextColor(_LCDML_ADAFRUIT_TEXT_COLOR);  
    // set text size / Textgroesse setzen
    display.setTextSize(_LCDML_ADAFRUIT_FONT_SIZE);
  
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 0); // line 0
    display.println(buf); 
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 1); // line 1
    display.println(F("press any key")); 
    display.setCursor(0, _LCDML_ADAFRUIT_FONT_H * 2); // line 2
    display.println(F("to leave it")); 

    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop())               // ****** LOOP *********
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

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}
