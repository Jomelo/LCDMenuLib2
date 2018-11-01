/* ===================================================================== *
 *                                                                       *
 * Dynamic content                                                       *
 *                                                                       *
 * ===================================================================== *
 */


uint8_t g_dynParam = 100; // when this value comes from an EEPROM, load it in setup
                          // at the moment here is no setup function (To-Do)
void mDyn_para(uint8_t line)
// *********************************************************************
{
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos())
  {
    // make only an action when the cursor stands on this menu item
    //check Button
    if(LCDML.BT_checkAny())
    {
      if(LCDML.BT_checkEnter())
      {
        // this function checks returns the scroll disable status (0 = menu scrolling enabled, 1 = menu scrolling disabled)
        if(LCDML.MENU_getScrollDisableStatus() == 0)
        {
          // disable the menu scroll function to catch the cursor on this point
          // now it is possible to work with BT_checkUp and BT_checkDown in this function
          // this function can only be called in a menu, not in a menu function
          LCDML.MENU_disScroll();
        }
        else
        {
          // enable the normal menu scroll function
          LCDML.MENU_enScroll();
        }

        // do something
        // ...
      }

      // This check have only an effect when MENU_disScroll is set
      if(LCDML.BT_checkUp())
      {
        g_dynParam++;
      }

      // This check have only an effect when MENU_disScroll is set
      if(LCDML.BT_checkDown())
      {
        g_dynParam--;
      }


      if(LCDML.BT_checkLeft())
      {
        g_dynParam++;
      }
      if(LCDML.BT_checkRight())
      {
        g_dynParam--;
      }
    }
  }

  char buf[20];
  sprintf (buf, "dynValue: %d", g_dynParam);

  // use the line from function parameters
  lcd.setCursor(1, line);
  lcd.print(buf);

}




// *********************************************************************
void mDyn_time(uint8_t line)
// *********************************************************************
{
  char buf[20];
  // http://www.c-howto.de/tutorial/benutzerinteraktion/bildschirmausgaben/
  sprintf (buf, "%02d:%02d:%02d", dyn_hour, dyn_min, dyn_sec);

  // use the line from function parameters
  lcd.setCursor(1, line);
  lcd.print(buf);

  // reset initscreen timer when this function is displayed
  LCDML.SCREEN_resetTimer();
  
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos())
  {
    // ...


  
  }
}


