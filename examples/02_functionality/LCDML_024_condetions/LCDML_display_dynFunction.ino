/* ===================================================================== *
 *                                                                       *
 * Dynamic content                                                       *
 *                                                                       *
 * ===================================================================== *
 */


uint8_t g_dynParam = 100; // when this value comes from an EEPROM, load it in setup
                          // at the moment here is no setup function (To-Do)
void mDyn_change_condetion(uint8_t line)
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


      if(LCDML.BT_checkLeft() || LCDML.BT_checkRight())
      {
        g_condetion_a = !g_condetion_a;
        
        // this functions are new in this version
       
        // The handling of condetions is changed in this version because a condetion must not called with every element call
        // The condetions would only be update when this function is called.         
        LCDML.MENU_allCondetionRefresh();

        // this function is usefull when content above this menu element needs an update. 
        // for example one dyn function change the content of another dyn function above this line.        
        LCDML.MENU_setDynFunctionContentUpdate();  
        
        // after the condetion changes it is usefull to go back to the root menu 
        //LCDML.MENU_goRoot();
        // or set the cursor to the right defined position
        LCDML.OTHER_setCursorToID(8); // to this function              
      }      
    }
  }

  if(g_condetion_a == true)
  {
    Serial.println("change to condetion: < B >");
  }
  else
  {
    Serial.println("change to condetion: < A >");
  }
  
}
