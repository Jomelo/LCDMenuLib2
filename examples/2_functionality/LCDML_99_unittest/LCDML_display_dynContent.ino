/* ===================================================================== *
 *                                                                       *
 * Dynamic content                                                       *
 *                                                                       *
 * ===================================================================== *
 */


// *********************************************************************
uint8_t g_dynParam = 100; // when this value comes from an eeprom, load it in setup 
                          // at the moment here is no setup function (todo) 
void cFunc_s1(uint8_t line)
// *********************************************************************
{ 
  // check if this function is active     
  if (line == LCDML.MENU_getCursorPos()) 
  {
    // set cursor
    Serial.print(F("(x) "));

    // make only an action when the cursor stands on this menuitem
    //check Button
    if(LCDML.BT_checkAny()) {
      if(LCDML.BT_checkEnter()) {
        // dosomething for example save the data or something else
        Serial.println("Save data");     
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
  else 
  {
    // clear cursor
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.print(F("change 'a' or 'd': "));
  Serial.println(g_dynParam);
  
}

// *********************************************************************
void cFunc_s2(uint8_t line)
// *********************************************************************
{  
    
  if (line == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));          
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.println(F("dummy 2"));  
}

// *********************************************************************
void cFunc_s3(uint8_t line)
// *********************************************************************
{  
   
  if (line == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));          
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.println(F("dummy 3"));    
}

// *********************************************************************
void cFunc_s4(uint8_t line)
// *********************************************************************
{
  
 
  if (line == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));          
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.println(F("dummy 4"));   
}

// *********************************************************************
void cFunc_s5(uint8_t line)
// *********************************************************************
{  
   
  if (line == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));          
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.println(F("hidden dummy 5"));    
}


// *********************************************************************
void cFunc_s6(uint8_t line)
// *********************************************************************
{   
   
  if (line == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));          
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.println(F("dummy 6"));    
}
