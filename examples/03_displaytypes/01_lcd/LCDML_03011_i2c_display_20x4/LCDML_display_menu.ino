// =====================================================================
//
// Output function
//
// =====================================================================

/* ******************************************************************** */
void lcdml_menu_clear()
/* ******************************************************************** */
{
  lcd.clear();
  lcd.setCursor(0, 0);
}

/* ******************************************************************** */
void lcdml_menu_display()
/* ******************************************************************** */
{
  // update content
  // ***************
  if (LCDML.DISP_checkMenuUpdate()) {
    // clear menu
    // ***************
    LCDML.DISP_clear();

    // declaration of some variables
    // ***************
    // content variable
    char content_text[_LCDML_DISP_cols];  // save the content text of every menu element
    // menu element object
    LCDMenuLib2_menu *tmp;
    // some limit values
    uint8_t i = LCDML.MENU_getScroll();
    uint8_t maxi = _LCDML_DISP_rows + i;
    uint8_t n = 0;

    // check if this element has children
    if ((tmp = LCDML.MENU_getDisplayedObj()) != NULL)
    {
      // loop to display lines
      do
      {
        // check if a menu element has a condition and if the condition be true
        if (tmp->checkCondition())
        {
          // check the type off a menu element
          if(tmp->checkType_menu() == true)
          {
            // display normal content
            LCDML_getContent(content_text, tmp->getID());
            lcd.setCursor(1, n);
            lcd.print(content_text);
          }
          else
          {
            if(tmp->checkType_dynParam()) {
              tmp->callback(n);
            }
          }
          // increment some values
          i++;
          n++;
        }
      // try to go to the next sibling and check the number of displayed rows
      } while (((tmp = tmp->getSibling(1)) != NULL) && (i < maxi));
    }
  }

  if(LCDML.DISP_checkMenuCursorUpdate())
  {
    // init vars
    uint8_t n_max             = (LCDML.MENU_getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.MENU_getChilds());
    uint8_t scrollbar_min     = 0;
    uint8_t scrollbar_max     = LCDML.MENU_getChilds();
    uint8_t scrollbar_cur_pos = LCDML.MENU_getCursorPosAbs();
    uint8_t scroll_pos        = ((1.*n_max * _LCDML_DISP_rows) / (scrollbar_max - 1) * scrollbar_cur_pos);


    // display rows
    for (uint8_t n = 0; n < n_max; n++)
    {
      //set cursor
      lcd.setCursor(0, n);

      //set cursor char
      if (n == LCDML.MENU_getCursorPos()) {
        lcd.write(_LCDML_DISP_cfg_cursor);
      } else {
        lcd.write(' ');
      }

      // delete or reset scrollbar
      if (_LCDML_DISP_cfg_scrollbar == 1) {
        if (scrollbar_max > n_max) {
          lcd.setCursor((_LCDML_DISP_cols - 1), n);
          lcd.write((uint8_t)0);
        }
        else {
          lcd.setCursor((_LCDML_DISP_cols - 1), n);
          lcd.print(' ');
        }
      }
    }

    // display scrollbar
    if (_LCDML_DISP_cfg_scrollbar == 1) {
      if (scrollbar_max > n_max) {
        //set scroll position
        if (scrollbar_cur_pos == scrollbar_min) {
          // min pos
          lcd.setCursor((_LCDML_DISP_cols - 1), 0);
          lcd.write((uint8_t)1);
        } else if (scrollbar_cur_pos == (scrollbar_max - 1)) {
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
  }
}
