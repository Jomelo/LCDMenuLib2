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

    switch(g_lcdml_lang_select)
    {
      case LANG_DE: // custom language long name
        Serial.println(F("Zum schlie\342en der"));
        Serial.println(F("Funktion, bitte druck"));
        Serial.println(F("irgendeine Taste oder"));
        Serial.println(F("den Zur\365ck Knopf"));
      break;

      default:
        Serial.println(F("To close this"));
        Serial.println(F("function press"));
        Serial.println(F("any button or use"));
        Serial.println(F("back button"));        
      break;
    }

    
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
void mFunc_switch_language(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    if(g_lcdml_lang_select == LANG_DE)
    {
      g_lcdml_lang_select = LANG_DEFAULT;
    }
    else
    {
      g_lcdml_lang_select = LANG_DE;
    } 

    
    switch(g_lcdml_lang_select)
    {
      case LANG_DE: // custom language long name
        Serial.println(F("Sprache geändert"));
      break;

      default:
        Serial.println(F("wait 20 seconds or press back button"));
      break;
    }

    // go back
    LCDML.FUNC_goBackToMenu();
  }


  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function

    // not needed here
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}
