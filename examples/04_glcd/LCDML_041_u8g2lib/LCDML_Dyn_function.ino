/* ===================================================================== *
 *                                                                       *
 * Dynamic content                                                       *
 *                                                                       *
 * ===================================================================== *
 */


uint8_t g_dynParam = 100; // when this value comes from an eeprom, load it in setup 
                          // at the moment here is no setup function (todo) 
void mDyn_para(uint8_t line)
// *********************************************************************
{ 
  // check if this function is active (cursor stands on this line)     
  if (line == LCDML.MENU_getCursorPos()) 
  {
    // make only an action when the cursor stands on this menuitem
    //check Button
    if(LCDML.BT_checkAny()) {
      if(LCDML.BT_checkEnter()) {
        // dosomething for example save the data or something else             
        LCDML.BT_resetEnter();
      }
      if(LCDML.BT_checkLeft()) {
        g_dynParam++;
        LCDML.BT_resetLeft();
      }
      if(LCDML.BT_checkRight()) {
        g_dynParam--;
        LCDML.BT_resetRight();
      }
    }              
  } 

  char buf[20];
  sprintf (buf, "dynValue: %d", g_dynParam);

  // setup function
  u8g2.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w + _LCDML_DISP_cur_space_behind,  (_LCDML_DISP_font_h * (1+line)), buf);     // the value can be changed with left or right   
 
}






