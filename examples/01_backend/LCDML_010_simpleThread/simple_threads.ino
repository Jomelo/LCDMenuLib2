

// =====================================================================
void simpleThread_setup(Thread_Serial_Blink_Example)
// ===================================================================== 
{
  // for example set pinmode for some inputs
  Serial.println();
}

boolean simpleThread_loop(Thread_Serial_Blink_Example)
{
  // generate a serial-output in the console every secound
  Serial.print(".");


  return false;
}


void simpleThread_stable(Thread_Serial_Blink_Example)
{ 
  // stable state   
}




// =====================================================================
void simpleThread_setup(Thread_input_check)
// ===================================================================== 
{
  // for example set pinmode for some inputs
}

boolean simpleThread_loop(Thread_input_check)
{
  // check inputs

  // check LCDML inputs
  LCDML.loop_control();

  // go to the next function because the wait time is set to 0
  // when this return value is "false" all threads with a higher priority will not be called
  return true;
}


void simpleThread_stable(Thread_input_check)
{ 
  // stable state   
}



// =====================================================================
void simpleThread_setup(Thread_LCDMenuLib)
// =====================================================================
{     
}
boolean simpleThread_loop(Thread_LCDMenuLib)
{  

  LCDML.loop_menu();

  return false;
}

void simpleThread_stable(Thread_LCDMenuLib)
{ 
   
}
