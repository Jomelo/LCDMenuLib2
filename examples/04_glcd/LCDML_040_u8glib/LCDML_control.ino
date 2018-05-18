// =====================================================================
//
// CONTROL
//
// =====================================================================
// *********************************************************************
// *********************************************************************
// content:
// (0) Control over serial interface  with asdw_e_q
// (1) Control over one analog input
// (2) Control over 4 - 6 digital input pins (internal pullups enabled)
// (3) Control over encoder [third party lib] (Download: https://github.com/PaulStoffregen/Encoder)
// (4) Control with Keypad  [third party lib] (Download: http://playground.arduino.cc/Main/KeypadTutorial )
// (5) Control with an IR remote [third party lib] (Download: https://github.com/z3t0/Arduino-IRremote )
// (6) Control with a joystick
// (7) Control over I2C PCF8574
// *********************************************************************

#define _LCDML_CONTROL_cfg      0

// theory:
// "#if" is a preprocessor directive and no error, look here:
// (English) https://en.wikipedia.org/wiki/C_preprocessor
// (German)  https://de.wikipedia.org/wiki/C-Pr%C3%A4prozessor


// *********************************************************************
// *************** (0) CONTROL OVER SERIAL INTERFACE *******************
// *********************************************************************
#if(_LCDML_CONTROL_cfg == 0)
// settings
  # define _LCDML_CONTROL_serial_enter           'e'
  # define _LCDML_CONTROL_serial_up              'w'
  # define _LCDML_CONTROL_serial_down            's'
  # define _LCDML_CONTROL_serial_left            'a'
  # define _LCDML_CONTROL_serial_right           'd'
  # define _LCDML_CONTROL_serial_quit            'q'
// *********************************************************************
void lcdml_menu_control(void)
{
  // If something must init, put in in the setup condition
  if(LCDML.BT_setup()) {
    // runs only once
  }

  // check if new serial input is available
  if (Serial.available()) {
    // read one char from input buffer
    switch (Serial.read())
    {
      case _LCDML_CONTROL_serial_enter:  LCDML.BT_enter(); break;
      case _LCDML_CONTROL_serial_up:     LCDML.BT_up();    break;
      case _LCDML_CONTROL_serial_down:   LCDML.BT_down();  break;
      case _LCDML_CONTROL_serial_left:   LCDML.BT_left();  break;
      case _LCDML_CONTROL_serial_right:  LCDML.BT_right(); break;
      case _LCDML_CONTROL_serial_quit:   LCDML.BT_quit();  break;
      default: break;
    }
  }
}

// *********************************************************************
// ******************************* END *********************************
// *********************************************************************





// *********************************************************************
// *************** (1) CONTROL OVER ONE ANALOG PIN *********************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 1)

  unsigned long g_LCDML_DISP_press_time = 0;
// settings
  #define _LCDML_CONTROL_analog_pin              0
  // when you did not use a button set the value to zero
  #define _LCDML_CONTROL_analog_enter_min        850     // Button Enter
  #define _LCDML_CONTROL_analog_enter_max        920
  #define _LCDML_CONTROL_analog_up_min           520     // Button Up
  #define _LCDML_CONTROL_analog_up_max           590
  #define _LCDML_CONTROL_analog_down_min         700     // Button Down
  #define _LCDML_CONTROL_analog_down_max         770
  #define _LCDML_CONTROL_analog_back_min         950     // Button Back
  #define _LCDML_CONTROL_analog_back_max         1020
  #define _LCDML_CONTROL_analog_left_min         430     // Button Left
  #define _LCDML_CONTROL_analog_left_max         500
  #define _LCDML_CONTROL_analog_right_min        610     // Button Right
  #define _LCDML_CONTROL_analog_right_max        680
// *********************************************************************

void lcdml_menu_control(void)
{
  // If something must init, put in in the setup condition
  if(LCDML.BT_setup()) {
    // runs only once
  }
  // check debounce timer
  if((millis() - g_LCDML_DISP_press_time) >= 200) {
    g_LCDML_DISP_press_time = millis(); // reset debounce timer

    uint16_t value = analogRead(_LCDML_CONTROL_analog_pin);  // analog pin for keypad

    if (value >= _LCDML_CONTROL_analog_enter_min && value <= _LCDML_CONTROL_analog_enter_max) { LCDML.BT_enter(); }
    if (value >= _LCDML_CONTROL_analog_up_min    && value <= _LCDML_CONTROL_analog_up_max)    { LCDML.BT_up();    }
    if (value >= _LCDML_CONTROL_analog_down_min  && value <= _LCDML_CONTROL_analog_down_max)  { LCDML.BT_down();  }
    if (value >= _LCDML_CONTROL_analog_left_min  && value <= _LCDML_CONTROL_analog_left_max)  { LCDML.BT_left();  }
    if (value >= _LCDML_CONTROL_analog_right_min && value <= _LCDML_CONTROL_analog_right_max) { LCDML.BT_right(); }
    if (value >= _LCDML_CONTROL_analog_back_min  && value <= _LCDML_CONTROL_analog_back_max)  { LCDML.BT_quit();  }
  }
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************






// *********************************************************************
// *************** (2) CONTROL OVER DIGITAL PINS ***********************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 2)
// settings
  unsigned long g_LCDML_DISP_press_time = 0;

  #define _LCDML_CONTROL_digital_low_active      0    // 0 = high active (pulldown) button, 1 = low active (pullup)
                                                      // http://playground.arduino.cc/CommonTopics/PullUpDownResistor
  #define _LCDML_CONTROL_digital_enable_quit     1
  #define _LCDML_CONTROL_digital_enable_lr       1
  #define _LCDML_CONTROL_digital_enter           8
  #define _LCDML_CONTROL_digital_up              9
  #define _LCDML_CONTROL_digital_down            10
  #define _LCDML_CONTROL_digital_quit            11
  #define _LCDML_CONTROL_digital_left            12
  #define _LCDML_CONTROL_digital_right           13
// *********************************************************************
void lcdml_menu_control(void)
{
  // If something must init, put in in the setup condition
  if(LCDML.BT_setup()) {
    // runs only once
    // init buttons
    pinMode(_LCDML_CONTROL_digital_enter      , INPUT_PULLUP);
    pinMode(_LCDML_CONTROL_digital_up         , INPUT_PULLUP);
    pinMode(_LCDML_CONTROL_digital_down       , INPUT_PULLUP);
    # if(_LCDML_CONTROL_digital_enable_quit == 1)
      pinMode(_LCDML_CONTROL_digital_quit     , INPUT_PULLUP);
    # endif
    # if(_LCDML_CONTROL_digital_enable_lr == 1)
      pinMode(_LCDML_CONTROL_digital_left     , INPUT_PULLUP);
      pinMode(_LCDML_CONTROL_digital_right    , INPUT_PULLUP);
    # endif
  }

  #if(_LCDML_CONTROL_digital_low_active == 1)
  #  define _LCDML_CONTROL_digital_a !
  #else
  #  define _LCDML_CONTROL_digital_a
  #endif

  uint8_t but_stat = 0x00;

  bitWrite(but_stat, 0, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_enter)));
  bitWrite(but_stat, 1, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_up)));
  bitWrite(but_stat, 2, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_down)));
  #if(_LCDML_CONTROL_digital_enable_quit == 1)
  bitWrite(but_stat, 3, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_quit)));
  #endif
  #if(_LCDML_CONTROL_digital_enable_lr == 1)
  bitWrite(but_stat, 4, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_left)));
  bitWrite(but_stat, 5, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_right)));
  #endif

  if (but_stat > 0) {
    if((millis() - g_LCDML_DISP_press_time) >= 200) {
      g_LCDML_DISP_press_time = millis(); // reset press time

      if (bitRead(but_stat, 0)) { LCDML.BT_enter(); }
      if (bitRead(but_stat, 1)) { LCDML.BT_up();    }
      if (bitRead(but_stat, 2)) { LCDML.BT_down();  }
      if (bitRead(but_stat, 3)) { LCDML.BT_quit();  }
      if (bitRead(but_stat, 4)) { LCDML.BT_left();  }
      if (bitRead(but_stat, 5)) { LCDML.BT_right(); }
    }
  }
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************






// *********************************************************************
// *************** (3) CONTROL WITH ENCODER ****************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 3)
  /*
   * Thanks to "MuchMore" (Arduino forum) to add this encoder functionality
   *
   * rotate left = Up
   * rotate right = Down
   * push = Enter
   * push long = Quit
   * push + rotate left = Left
   * push + rotate right = Right
   */

  /* encoder connection
   * button * (do not use an external resistor, the internal pullup resistor is used)
   * .-------o  Arduino Pin
   * |
   * |
   * o  /
   *   /
   *  /
   * o
   * |
   * '-------o   GND
   *
   * encoder * (do not use an external resistor, the internal pullup resistors are used)
   *
   * .---------------o  Arduino Pin A
   * |        .------o  Arduino Pin B
   * |        |
   * o  /     o  /
   *   /        /
   *  /        /
   * o        o
   * |        |
   * '--------o----o   GND (common pin)
   */

  // global defines
  #define encoder_A_pin       20    // physical pin has to be 2 or 3 to use interrupts (on mega e.g. 20 or 21), use internal pullups
  #define encoder_B_pin       21    // physical pin has to be 2 or 3 to use interrupts (on mega e.g. 20 or 21), use internal pullups
  #define encoder_button_pin  49    // physical pin , use internal pullup

  #define g_LCDML_CONTROL_button_long_press    800   // ms
  #define g_LCDML_CONTROL_button_short_press   120   // ms

  #define ENCODER_OPTIMIZE_INTERRUPTS //Only when using pin2/3 (or 20/21 on mega)
  #include <Encoder.h> //for Encoder    Download:  https://github.com/PaulStoffregen/Encoder

  Encoder ENCODER(encoder_A_pin, encoder_B_pin);

  long  g_LCDML_CONTROL_button_press_time = 0;
  bool  g_LCDML_CONTROL_button_prev       = HIGH;

// *********************************************************************
void lcdml_menu_control(void)
// *********************************************************************
{
  // If something must init, put in in the setup condition
  if(LCDML.BT_setup())
  {
    // runs only once

    // init pins
    pinMode(encoder_A_pin      , INPUT_PULLUP);
    pinMode(encoder_B_pin      , INPUT_PULLUP);
    pinMode(encoder_button_pin  , INPUT_PULLUP);
  }

  //volatile Variable
  long g_LCDML_CONTROL_Encoder_position = ENCODER.read();
  bool button                           = digitalRead(encoder_button_pin);

  if(g_LCDML_CONTROL_Encoder_position <= -3) {

    if(!button)
    {
      LCDML.BT_left();
      g_LCDML_CONTROL_button_prev = LOW;
      g_LCDML_CONTROL_button_press_time = -1;
    }
    else
    {
      LCDML.BT_down();
    }
    ENCODER.write(g_LCDML_CONTROL_Encoder_position+4);
  }
  else if(g_LCDML_CONTROL_Encoder_position >= 3)
  {

    if(!button)
    {
      LCDML.BT_right();
      g_LCDML_CONTROL_button_prev = LOW;
      g_LCDML_CONTROL_button_press_time = -1;
    }
    else
    {
      LCDML.BT_up();
    }
    ENCODER.write(g_LCDML_CONTROL_Encoder_position-4);
  }
  else
  {
    if(!button && g_LCDML_CONTROL_button_prev)  //falling edge, button pressed
    {
      g_LCDML_CONTROL_button_prev = LOW;
      g_LCDML_CONTROL_button_press_time = millis();
    }
    else if(button && !g_LCDML_CONTROL_button_prev) //rising edge, button not active
    {
       g_LCDML_CONTROL_button_prev = HIGH;

       if(g_LCDML_CONTROL_button_press_time < 0)
       {
         g_LCDML_CONTROL_button_press_time = millis();
         //Reset for left right action
       }
       else if((millis() - g_LCDML_CONTROL_button_press_time) >= g_LCDML_CONTROL_button_long_press)
       {
         LCDML.BT_quit();
       }
       else if((millis() - g_LCDML_CONTROL_button_press_time) >= g_LCDML_CONTROL_button_short_press)
       {
         LCDML.BT_enter();
       }
    }
  }
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************





// *********************************************************************
// *************** (4) CONTROL WITH A KEYPAD ***************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 4)
// include
  // more information under http://playground.arduino.cc/Main/KeypadTutorial
  #include <Keypad.h>
// settings
  #define _LCDML_CONTROL_keypad_rows 4 // Four rows
  #define _LCDML_CONTROL_keypad_cols 3 // Three columns
// global vars
  char keys[_LCDML_CONTROL_keypad_rows][_LCDML_CONTROL_keypad_cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'#','0','*'}
  };
  byte rowPins[_LCDML_CONTROL_keypad_rows] = { 9, 8, 7, 6 };  // Connect keypad COL0, COL1 and COL2 to these Arduino pins.
  byte colPins[_LCDML_CONTROL_keypad_cols] = { 12, 11, 10 };  // Create the Keypad
// objects
  Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, _LCDML_CONTROL_keypad_rows, _LCDML_CONTROL_keypad_cols );
// *********************************************************************
void lcdml_menu_control(void)
{
  // If something must init, put in in the setup condition
  if(LCDML.BT_setup()) {
    // runs only once
  }
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case '#': LCDML.BT_enter(); break;
      case '2': LCDML.BT_up();    break;
      case '8': LCDML.BT_down();  break;
      case '4': LCDML.BT_left();  break;
      case '6': LCDML.BT_right(); break;
      case '*': LCDML.BT_quit();  break;
      default: break;
    }
  }
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************


// *********************************************************************
// *************** (5) CONTROL WITH IR REMOTE ***************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 5)
    // IR include (this lib have to be installed)
    #include <IRremote.h>
    // IR global vars
    int RECV_PIN = 11;
    // IR objects
    IRrecv irrecv(RECV_PIN);
    decode_results results;

// *********************************************************************
// change in this function the IR values to your values
void lcdml_menu_control(void)
{
  // If something must init, put in in the setup condition
  if(LCDML.BT_setup()) {
    // runs only once
    irrecv.enableIRIn(); // Start the receiver
  }

  if (irrecv.decode(&results))
  {
      // comment this line out, to check the correct code
      //Serial.println(results.value, HEX);

      // in this switch case you have to change the value 0x...1 to the correct IR code
      switch (results.value)
      {
          case 0x00000001: LCDML.BT_enter(); break;
          case 0x00000002: LCDML.BT_up();    break;
          case 0x00000003: LCDML.BT_down();  break;
          case 0x00000004: LCDML.BT_left();  break;
          case 0x00000005: LCDML.BT_right(); break;
          case 0x00000006: LCDML.BT_quit();  break;
          default: break;
      }
  }
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************



// *********************************************************************
// *************** (6) CONTROL OVER JOYSTICK ***************************
// *********************************************************************
#elif(_LCDML_CONTROL_cfg == 6)
  unsigned long g_LCDML_DISP_press_time = 0;
    // settings
    #define _LCDML_CONTROL_analog_pinx A0
    #define _LCDML_CONTROL_analog_piny A1
    #define _LCDML_CONTROL_digitalread 33 //don't work with u8glib

    // when you did not use a button set the value to zero
    #define _LCDML_CONTROL_analog_up_min 612 // Button Up
    #define _LCDML_CONTROL_analog_up_max 1023
    #define _LCDML_CONTROL_analog_down_min 0 // Button Down
    #define _LCDML_CONTROL_analog_down_max 412
    #define _LCDML_CONTROL_analog_left_min 612 // Button Left
    #define _LCDML_CONTROL_analog_left_max 1023
    #define _LCDML_CONTROL_analog_right_min 0 // Button Right
    #define _LCDML_CONTROL_analog_right_max 412
// *********************************************************************
void lcdml_menu_control(void)
{
  // If something must init, put in in the setup condition
  if(LCDML.BT_setup()) {
    // runs only once
    pinMode (_LCDML_CONTROL_digitalread, INPUT);
  }
  // check debounce timer
  if((millis() - g_LCDML_DISP_press_time) >= 200) {
    g_LCDML_DISP_press_time = millis(); // reset debounce timer

    uint16_t valuex = analogRead(_LCDML_CONTROL_analog_pinx);  // analogpinx
    uint16_t valuey = analogRead(_LCDML_CONTROL_analog_piny);  // analogpinx
    uint16_t valuee = digitalRead(_LCDML_CONTROL_digitalread);  //digitalpinenter


    if (valuey >= _LCDML_CONTROL_analog_up_min && valuey <= _LCDML_CONTROL_analog_up_max) { LCDML.BT_up(); }        // up
    if (valuey >= _LCDML_CONTROL_analog_down_min && valuey <= _LCDML_CONTROL_analog_down_max) { LCDML.BT_down(); }    // down
    if (valuex >= _LCDML_CONTROL_analog_left_min && valuex <= _LCDML_CONTROL_analog_left_max) { LCDML.BT_left(); }     // left
    if (valuex >= _LCDML_CONTROL_analog_right_min && valuex <= _LCDML_CONTROL_analog_right_max) { LCDML.BT_right(); }    // right

    if(valuee == true) {LCDML.BT_enter();}    // enter

    // back buttons have to be included as menu item
    // lock at the example LCDML_back_button
  }
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************

// *********************************************************************
// *************** (7) CONTROL OVER PCF8574 ****************************
// *********************************************************************

#elif(_LCDML_CONTROL_cfg == 7)
  unsigned long g_LCDML_DISP_press_time = 0;
  #define PCF8574_1 0x26   #define PCF8574_1 0x26 // I2C address for the buttons

  #define PCF8574_Pin0 254
  #define PCF8574_Pin1 253
  #define PCF8574_Pin2 251
  #define PCF8574_Pin3 247
  #define PCF8574_Pin4 239
  #define PCF8574_Pin5 223
  #define PCF8574_Pin6 191
  #define PCF8574_Pin7 127

  // Specify the PCF8574 pins here
  #define _LCDML_CONTROL_PCF8574_enable_quit    0
  #define _LCDML_CONTROL_PCF8574_enable_lr      0
  #define _LCDML_CONTROL_PCF8574_enter          PCF8574_Pin1
  #define _LCDML_CONTROL_PCF8574_up             PCF8574_Pin2
  #define _LCDML_CONTROL_PCF8574_down           PCF8574_Pin0
  #define _LCDML_CONTROL_PCF8574_left           PCF8574_Pin2
  #define _LCDML_CONTROL_PCF8574l_right         PCF8574_Pin2
  #define _LCDML_CONTROL_PCF8574_quit           PCF8574_Pin2
// **********************************************************
void lcdml_menu_control(void)
{
  // If something must init, put in in the setup condition
  if(LCDML.BT_setup()) {
    // runs only once
  }

  if((millis() - g_LCDML_DISP_press_time) >= 200) {
      g_LCDML_DISP_press_time = millis(); // reset press time

    Wire.write(0xff); // All pins as input?
    Wire.requestFrom(PCF8574_1, 1);
   if (Wire.available()) {
    switch (Wire.read())
    {
      case _LCDML_CONTROL_PCF8574_enter:  LCDML.BT_enter(); break;
      case _LCDML_CONTROL_PCF8574_up:     LCDML.BT_up();    break;
      case _LCDML_CONTROL_PCF8574_down:   LCDML.BT_down();  break;
       #if(_LCDML_CONTROL_PCF8574_enable_quit == 1)
      case _LCDML_CONTROL_PCF8574_left:   LCDML.BT_left();  break;
    #endif
       #if(_LCDML_CONTROL_PCF8574_enable_lr   == 1)
      case _LCDML_CONTROL_PCF8574l_right: LCDML.BT_right(); break;
      case _LCDML_CONTROL_PCF8574_quit:   LCDML.BT_quit();  break;
    #endif
      default: break;
    }
  }
 }
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************


#else
  #error _LCDML_CONTROL_cfg is not defined or not in range
#endif
