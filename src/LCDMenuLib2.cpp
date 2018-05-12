/* ******************************************************************************
 *                        LCDMenuLib2 (LCDML)
 * ******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) [2017] [Nils Feldkämper]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ******************************************************************************
 *
 * BUG / ISSUES REPORTING
 *    https://github.com/Jomelo/LCDMenuLib2/issues
 *
 * ARDUINO FORUM
 *    http://forum.arduino.cc/index.php?topic=73816.0
 *
 * ******************************************************************************
 */

#include "LCDMenuLib2.h"
#include "LCDMenuLib2_typedef.h"

/* ******************************************************************** */
LCDMenuLib2::LCDMenuLib2(LCDMenuLib2_menu &p_r, const uint8_t p_rows, const uint8_t p_cols, LCDML_FuncPtr contentUpdate, LCDML_FuncPtr contentClear, LCDML_FuncPtr   menuControl)
/* ******************************************************************** */
{
    // initialization
    rootMenu                = &p_r;
    curMenu                 = rootMenu;
    control                 = 0;
    funcReg                 = 0;
    cursor_pos              = 0;
    layer                   = 0;
    layer_save[0]           = 0;
    child_cnt               = 0;
    rows                    = p_rows;
    cols                    = (p_cols-1);
    activMenu               = NULL;
    menu_timer              = 0;
    menu_default_time       = 100000000;
    callback_contentUpdate  = contentUpdate;  // callback update content
    callback_contentClear   = contentClear;   // callback clear content
    callback_menuControl    = menuControl;    // callback buttons

    BT_resetAll();
}

/* ******************************************************************** */
void LCDMenuLib2::loop()
/* ******************************************************************** */
{
    loop_control();
    loop_menu();
}

/* ******************************************************************** */
void LCDMenuLib2::loop_control()
/* ******************************************************************** */
{
    // Check Control
    if(callback_menuControl != NULL)
    {
        callback_menuControl();
    }

    // Screensaver
    if(cb_screensaver != NULL && bitRead(funcReg, _LCDML_funcReg_disable_screensaver) == false)
    {
        if(activMenu != NULL)
        {
            // a menu function is active
            if(activMenu->getCbFunction() != cb_screensaver) // check if screensaver is active
            {
                if(TIMER_ms(screensaver_timer, screensaver_default_time))
                {
                    OTHER_jumpToFunc(cb_screensaver);
                }
            }
        }
        else
        {
            // only the menu is active
            if(TIMER_ms(screensaver_timer, screensaver_default_time))
            {
                OTHER_jumpToFunc(cb_screensaver);
            }
        }
    }
}

/* ******************************************************************** */
void LCDMenuLib2::loop_menu()
/* ******************************************************************** */
{
    // check control activity
    if(BT_checkAny() == true)
    {
        SCREEN_resetTimer();

        if(activMenu == NULL)
        {
            // -- ENTER --
            if(BT_checkEnter() == true)
            {
                //menu is active
                MENU_goInto();
                bitSet(control, _LCDML_control_update_direct);
            }

            // -- UP --
            if(BT_checkUp() == true)
            {               
                if(bitRead(funcReg, _LCDML_funcReg_disable_scroll) == false)
                {                   
                    if (curloc > 0) {
                        curloc--;
                        MENU_doScroll();
                    }
                    else
                    {
                        if(bitRead(control, _LCDML_control_rollover))
                        {
                            // jump to the end of the menu
                            curloc = child_cnt;
                            if(child_cnt-rows < 0) {
                                scroll = 0;
                            } else {
                                scroll = child_cnt-rows;
                            }
                            MENU_doScroll();
                            MENU_display();
                        }
                    }
                    MENU_setCursor();
                    DISP_menuUpdate();
                }
                else
                {                    
                    MENU_display();
                    DISP_menuUpdate();
                }                
            }

            // -- DOWN --
            if(BT_checkDown() == true)
            {
                if(bitRead(funcReg, _LCDML_funcReg_disable_scroll) == false)
                {
                    if(curloc < child_cnt) {
                        curloc++;
                        MENU_doScroll();
                    }
                    else
                    {
                        if(bitRead(control, _LCDML_control_rollover))
                        {
                            // jump to the first line
                            curloc = 0;
                            scroll = 0;
                            MENU_doScroll();
                            MENU_display();
                        }
                    }
                    MENU_setCursor();
                    DISP_menuUpdate();
                }
                else
                {                    
                    MENU_display();
                    DISP_menuUpdate();
                }
            }


            // -- LEFT or RIGHT --
            if(BT_checkLeft() == true or BT_checkRight() == true)
            {
                MENU_display();
                DISP_menuUpdate();
            }

            // -- QUIT Part 1 --
            if(BT_checkQuit() == true)
            {
                MENU_enScroll();

                if(layer > 0)
                {
                    MENU_goBack();
                    MENU_display();
                    DISP_menuUpdate();
                }
            }

            // reset all buttons
            BT_resetAll();
        }
        else
        {
            // -- QUIT Part 2 --
            if(BT_checkQuit() == true)
            {
                MENU_enScroll();
                FUNC_goBackToMenu();
                BT_resetQuit();
            }
        }
    }

    // Check Function
    if(activMenu != NULL)
    {
        if(bitRead(funcReg, _LCDML_funcReg_end) == false && bitRead(funcReg, _LCDML_funcReg_goRoot) == false)
        {
            if(TIMER_ms(menu_timer, menu_default_time) == true || bitRead(funcReg, _LCDML_funcReg_setup) == false || button > 0)
            {
                FUNC_call();
                BT_resetAll();
            }
        }
        else
        {
            // set this bit to call the FUNC_close function when the goRoot function is called
            bitSet(funcReg, _LCDML_funcReg_end);

            FUNC_call();    // call active function for save close;
            BT_resetAll();

            activMenu = NULL;

            bitClear(funcReg, _LCDML_funcReg_disable_screensaver);
            bitClear(funcReg, _LCDML_funcReg_end);
            bitClear(funcReg, _LCDML_funcReg_setup);
            bitClear(funcReg, _LCDML_funcReg_jumpTo_w_para);

            for(uint8_t i=0; i<goBackCnt; i++)
            {
                MENU_goBack();
            }
            goBackCnt = 0;

            if(bitRead(funcReg, _LCDML_funcReg_goRoot) == true)
            {
                // set the cursor the the root position
                bitClear(funcReg, _LCDML_funcReg_goRoot);

                curMenu     = rootMenu;
                layer       = 0;
                cursor_pos  = 0;
                curloc      = 0;
                scroll      = 0;
            }

            if(bitRead(control, _LCDML_control_search_display) == false)
            {
                // only update the content when no jump function is active
                MENU_display();
                DISP_menuUpdate();
            }
        }
    }
    else
    {
        menu_default_time = 100000000;
    }
}


/* ******************************************************************** */
void        LCDMenuLib2::MENU_goRoot()
/* ******************************************************************** */
{
    if(activMenu != NULL)
    {
        bitSet(funcReg, _LCDML_funcReg_goRoot);
    }
    else
    {
        // check if root position is reached
        BT_resetAll();

        curMenu     = rootMenu;
        layer       = 0;
        cursor_pos  = 0;
        curloc      = 0;
        scroll      = 0;

        if(bitRead(control, _LCDML_control_search_display) == false)
        {
            // only update the content when no jump function is active
            MENU_display();
        }
    }
}

/* ******************************************************************** */
uint8_t        LCDMenuLib2::MENU_curlocCorrection()
/* ******************************************************************** */
{
    uint8_t curloc_cor  = 0;
    uint8_t j           = 0;
    LCDMenuLib2_menu *tmp;

    //correct function / menu element to open for hidden menu elements
    if ((tmp = curMenu->getChild(0)) != NULL)
    {
        do
        {
            if (tmp->checkCondition() || bitRead(control, _LCDML_control_disable_hidden))
            {
                j++;
            }
            else
            {
                if (j <= curloc)
                {
                    curloc_cor++;
                }
            }
        } while ((tmp = tmp->getSibling(1)) != NULL);
    }
    return curloc_cor;
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_goBack(void)
/* ******************************************************************** */
{
    if(curMenu->getParent() != NULL)
    {
        //set current menu object
        MENU_goMenu(*curMenu->getParent(), true);
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_goInto(void)
/* ******************************************************************** */
{
    LCDMenuLib2_menu *tmp;

    // check if a menu function is not active
    if (activMenu == NULL)
    {
        // get element to to open
        tmp = curMenu->getChild(curloc + MENU_curlocCorrection());

        // check if element is a menu function
        if(tmp->checkCallback() == true && tmp->checkType_menu() == true && bitRead(control, _LCDML_control_search_display) == false)
        {
            // Menu function found
            activMenu = tmp;
        }
        else
        {
            if(tmp->checkType_dynParam() == true && bitRead(control, _LCDML_control_search_display) == false)
            {
                MENU_display();
                DISP_menuUpdate();
            }
            else
            {
                if(tmp->getChild(0) != NULL)
                {
                    while ((tmp = tmp->getSibling(0)) != NULL)
                    {
                        if (tmp->checkCondition() || bitRead(control, _LCDML_control_disable_hidden))
                        {
                            // Menu found, goInto
                            MENU_goMenu(*curMenu->getChild(curloc + MENU_curlocCorrection()), false);

                            if(bitRead(control, _LCDML_control_search_display) == false)
                            {
                                BT_resetAll(); // reset all buttons

                                child_cnt = MENU_countChilds();

                                MENU_display();
                                DISP_menuUpdate();
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_goMenu(LCDMenuLib2_menu &p_m, uint8_t p_back)
/* ******************************************************************** */
{
    //declare variables
    int diff;

    curMenu = &p_m;

    if(layer < _LCDML_DISP_cfg_cursor_deep) //check layer deep
    {
        if(p_back == false)
        {
            // go into the next layer
            // save old layer data
            layer_save[layer] = curloc;
            // to-do: sometimes on jumpTo or setCursorTo function with hidden elements the scroll position is saved wrong
            scroll_save[layer] = scroll;
            // new layer
            scroll = 0;
            layer++;
            // save parent id
            if(layer < _LCDML_DISP_cfg_cursor_deep)
            {
                parents[layer] = curMenu->getID();
            }
            curloc = 0;
        }
        else
        {
            if(layer > 0)
            {
                layer--;
                scroll = scroll_save[layer];
                curloc = layer_save[layer];

                if(curloc >= rows)
                {
                    diff = curloc - rows + 1;
                    for(int i=0; i<diff; i++)
                    {
                        MENU_doScroll();
                    }
                }
            }
        }
    }
}

/* ******************************************************************** */
uint8_t    LCDMenuLib2::MENU_countChilds()
/* ******************************************************************** */
{
    //declaration
    uint8_t j = 0;
    LCDMenuLib2_menu *tmp;

    //check if element has children
    if ((tmp = curMenu->getChild(0)) != NULL)
    {
        do
        {
            if (tmp->checkCondition() || bitRead(control, _LCDML_control_disable_hidden))
            {
                j++;
            }
        } while ((tmp = tmp->getSibling(1)) != NULL);
    }

    if(j == 0)
    {
        return 0;
    }
    else
    {
        return --j;
    }
}

/* ******************************************************************** */
LCDMenuLib2_menu * LCDMenuLib2::MENU_getObj()
/* ******************************************************************** */
{
    return curMenu;
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_display(uint8_t update)
/* ******************************************************************** */
{
    //declaration
    uint8_t i = scroll;
    LCDMenuLib2_menu *tmp;

    //check children
    if((tmp = curMenu->getChild(i)))
    {
        bitSet(control, _LCDML_control_disp_update);
    }
    MENU_setCursor();
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getScroll()
/* ******************************************************************** */
{
    return scroll;
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_setCursor()
/* ******************************************************************** */
{
    child_cnt = MENU_countChilds();

    //save current cursor position
    cursor_pos = curloc - scroll;

    if (cursor_pos > child_cnt) {
        cursor_pos = child_cnt;
    }

    bitSet(control, _LCDML_control_cursor_update);
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_doScroll()
/* ******************************************************************** */
{
    //only allow it to go up to menu element
    while (curloc > 0 && curMenu->getChild(curloc) == false)
    {
        curloc--;
    }

    //scroll down
    if (curloc >= (rows + scroll))
    {
        scroll++;
        if(bitRead(control, _LCDML_control_search_display) == false)
        {
            MENU_display();
        }
    }
    //scroll up
    else if (curloc < scroll)
    {
        scroll--;
        if(bitRead(control, _LCDML_control_search_display) == false)
        {
            MENU_display();
        }
    }
    //do not scroll
    else
    {
        MENU_setCursor();
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_enRollover(void)
/* ******************************************************************** */
{
    bitSet(control, _LCDML_control_rollover);
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_disRollover(void)
/* ******************************************************************** */
{
    bitClear(control, _LCDML_control_rollover);
}

/* ******************************************************************** */
uint8_t    LCDMenuLib2::FUNC_getID()
/* ******************************************************************** */
{
    if(activMenu != NULL) {
        return activMenu->getID();
    } else {
        return _LCDML_NO_FUNC;
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::FUNC_setLoopInterval(unsigned long p_t)
/* ******************************************************************** */
{
    menu_default_time = p_t;
}

/* ******************************************************************** */
uint8_t    LCDMenuLib2::MENU_getLayer()
/* ******************************************************************** */
{
    return layer;
}

/* ******************************************************************** */
uint8_t    LCDMenuLib2::MENU_getCursorPos()
/* ******************************************************************** */
{
    return (cursor_pos); //return the current cursor position
}

/* ******************************************************************** */
uint8_t    LCDMenuLib2::MENU_getCursorPosAbs()
/* ******************************************************************** */
{
    return (curloc + MENU_curlocCorrection()); //return the current cursor position
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getChilds()
/* ******************************************************************** */
{
    return child_cnt+1;
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getParentId()
/* ******************************************************************** */
{
    if(layer > 0 && layer < _LCDML_DISP_cfg_cursor_deep) {
        return parents[layer];
    } else {
        return _LCDML_NO_FUNC;
    }
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getParentId(uint8_t p_layer)
/* ******************************************************************** */
{
    if(p_layer > 0 && p_layer < _LCDML_DISP_cfg_cursor_deep) {
        return parents[p_layer];
    } else {
        return _LCDML_NO_FUNC;
    }
}

/* ******************************************************************** */
void LCDMenuLib2::DISP_menuUpdate()
/* ******************************************************************** */
{
    if(activMenu == NULL || bitRead(funcReg, _LCDML_funcReg_end) || bitRead(control, _LCDML_control_update_direct) || bitRead(control, _LCDML_control_disp_update) )
    {
        if(bitRead(funcReg, _LCDML_funcReg_disable_scroll) == false)
        {
            // clear button status from scrolling
            BT_resetUp();
            BT_resetDown();
        }

        callback_contentUpdate();
        BT_resetAll();
    }
}

/* ******************************************************************** */
void LCDMenuLib2::DISP_clear()
/* ******************************************************************** */
{
    if(bitRead(control, _LCDML_control_search_display) == false)
    {
        callback_contentClear();
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::DISP_checkMenuUpdate()
/* ******************************************************************** */
{
    if (bitRead(control, _LCDML_control_disp_update) || bitRead(control, _LCDML_control_update_direct)) {
        bitClear(control, _LCDML_control_disp_update);
        bitClear(control, _LCDML_control_update_direct);
        return true;
    } else {
        return false;
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::DISP_checkMenuCursorUpdate()
/* ******************************************************************** */
{
    if(bitRead(control, _LCDML_control_cursor_update)) {
        bitClear(control, _LCDML_control_cursor_update);
        return true;
    } else {
        return false;
    }
}

/* ******************************************************************** */
void LCDMenuLib2::FUNC_call()
/* ******************************************************************** */
{
    if(activMenu != NULL)
    {
        if(bitRead(funcReg, _LCDML_funcReg_jumpTo_w_para))
        {
            activMenu->callback(jumpTo_w_para);
        }
        else
        {
            activMenu->callback(activMenu->getParam());
        }
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::FUNC_setup()
/* ******************************************************************** */
{
    if(activMenu != NULL)
    {
        // function active
        if(bitRead(funcReg, _LCDML_funcReg_setup) == 0) {
            bitSet(funcReg, _LCDML_funcReg_setup); // run setup
            DISP_clear();
            BT_resetAll();
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        bitClear(funcReg, _LCDML_funcReg_setup); // run setup
		
        return false;
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::FUNC_loop()
/* ******************************************************************** */
{
    if(activMenu != NULL)
    {
        if(bitRead(funcReg, _LCDML_funcReg_end) == false)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        bitClear(funcReg, _LCDML_funcReg_end);
		
        return false;
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::FUNC_close()
/* ******************************************************************** */
{
    if(activMenu != NULL)
    {
        if(bitRead(funcReg, _LCDML_funcReg_end) == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        bitClear(funcReg, _LCDML_funcReg_end);
		
        return false;
    }
}


/* ******************************************************************** */
void LCDMenuLib2::FUNC_goBackToMenu(uint8_t p_e)
/* ******************************************************************** */
{
    if(activMenu != NULL)
    {
        goBackCnt = p_e;
        bitSet(funcReg, _LCDML_funcReg_end);
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::FUNC_disableScreensaver()
/* ******************************************************************** */
{
    bitSet(funcReg, _LCDML_funcReg_disable_screensaver);
}

/* ******************************************************************** */
boolean LCDMenuLib2::BT_setup()
/* ******************************************************************** */
{
    if(bitRead(control, _LCDML_control_setup) == false)
    {
        bitSet(control, _LCDML_control_setup);
        return true;
    }
    else
    {
        return false;
    }

}

/* ******************************************************************** */
void LCDMenuLib2::BT_enter()
/* ******************************************************************** */
{
    bitSet(button, _LCDML_button_enter);
}

/* ******************************************************************** */
void LCDMenuLib2::BT_up()
/* ******************************************************************** */
{
    bitSet(button, _LCDML_button_up);
}

/* ******************************************************************** */
void LCDMenuLib2::BT_down()
/* ******************************************************************** */
{
    bitSet(button, _LCDML_button_down);
}

/* ******************************************************************** */
void LCDMenuLib2::BT_left()
/* ******************************************************************** */
{
    bitSet(button, _LCDML_button_left);
}

/* ******************************************************************** */
void LCDMenuLib2::BT_right()
/* ******************************************************************** */
{
    bitSet(button, _LCDML_button_right);
}

/* ******************************************************************** */
void LCDMenuLib2::BT_quit()
/* ******************************************************************** */
{
    bitSet(button, _LCDML_button_quit);
}

/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkAny()       { if((button > 0)) { return true; } else { return false; }                              }
/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkEnter()     { if(bitRead(button, _LCDML_button_enter)) { return true; } else { return false; }      }
boolean LCDMenuLib2::BT_checkUp()        { if(bitRead(button, _LCDML_button_up)) { return true; } else { return false; }         }
boolean LCDMenuLib2::BT_checkDown()      { if(bitRead(button, _LCDML_button_down)) { return true; } else { return false; }       }
boolean LCDMenuLib2::BT_checkLeft()      { if(bitRead(button, _LCDML_button_left)) { return true; } else { return false; }       }
boolean LCDMenuLib2::BT_checkRight()     { if(bitRead(button, _LCDML_button_right)) { return true; } else { return false; }      }
boolean LCDMenuLib2::BT_checkQuit()      { if(bitRead(button, _LCDML_button_quit)) { return true; } else { return false; }      }
/* ******************************************************************** */
void LCDMenuLib2::BT_resetAll()          { button = 0;}
/* ******************************************************************** */
void LCDMenuLib2::BT_resetEnter()        { bitClear(button, _LCDML_button_enter);    }
void LCDMenuLib2::BT_resetUp()           { bitClear(button, _LCDML_button_up);       }
void LCDMenuLib2::BT_resetDown()         { bitClear(button, _LCDML_button_down);     }
void LCDMenuLib2::BT_resetLeft()         { bitClear(button, _LCDML_button_left);     }
void LCDMenuLib2::BT_resetRight()        { bitClear(button, _LCDML_button_right);    }
void LCDMenuLib2::BT_resetQuit()         { bitClear(button, _LCDML_button_quit);     }
/* ******************************************************************** */


/* ******************************************************************** */
void    LCDMenuLib2::TIMER_msReset(unsigned long &p_var)
/* ******************************************************************** */
{
    p_var = millis();
}

/* ******************************************************************** */
boolean LCDMenuLib2::TIMER_ms(unsigned long &p_var, unsigned long p_t)
/* ******************************************************************** */
{
    if((millis() - p_var) >= p_t)
    {
        p_var = millis();
        return true;
    }
    else
    {
        return false;
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::TIMER_usReset(unsigned long &p_var)
/* ******************************************************************** */
{
    p_var = micros();
}

/* ******************************************************************** */
boolean LCDMenuLib2::TIMER_us(unsigned long &p_var, unsigned long p_t)
/* ******************************************************************** */
{
    if((micros() - p_var) >= p_t)
    {
        p_var = micros();
        return true;
    }
    else
    {
        return false;
    }
}



/* ******************************************************************** */
boolean     LCDMenuLib2::OTHER_searchFunction(LCDMenuLib2_menu &p_m, uint8_t mode, LCDML_FuncPtr_pu8 p_search, uint8_t p_id)
/* ******************************************************************** */
{
    //declaration
    LCDMenuLib2_menu *search = &p_m;
    boolean found    = false;

    if(activMenu == NULL)
    {
        do
        {
            DBG_print(LCDML_DBG_search, search->getID());
            DBG_print(LCDML_DBG_search, F(" : "));
            DBG_print(LCDML_DBG_search, (activMenu == NULL)?"0":"1");
            DBG_print(LCDML_DBG_search, F(" : "));

            if((search->getCbFunction() == p_search && mode == 0)  || (search->getID() == p_id && mode == 1)) {
                DBG_println(LCDML_DBG_search, F("found"));
                // uncomment found when you will debug this function
                found = true;
            }
            else
            {
                DBG_println(LCDML_DBG_search, "");

                // check if an element have a child or children
                if (search->getChild(0) != NULL)
                {
                    //go into a menu, but do not start a menu function until the search mode is run
                    MENU_goInto();

                    found = OTHER_searchFunction(*search->getChild(0), mode, p_search, p_id); //recursive search until found is true or last item reached

                    if (found == false)
                    {
                        //quit layer,
                        MENU_goBack();
                        child_cnt = MENU_countChilds();
                        // go to next element
                        if (curloc < child_cnt)
                        {
                            curloc++;
                            MENU_doScroll();
                        }
                    }
                }
                else
                {
                    //select next element
                    if (curloc < child_cnt)
                    {
                        curloc++;
                        MENU_doScroll();
                    }
                }
            }
        } while ((search=search->getSibling(1)) != NULL && found == false);
    }
    else
    {
        // something is wrong, please report a bug
        found = false;
    }

    return found; //found;
}

/* ******************************************************************** */
boolean LCDMenuLib2::OTHER_helpFunction(uint8_t mode, LCDML_FuncPtr_pu8 p_search, uint8_t p_id, uint8_t p_para)
/* ******************************************************************** */
{
    boolean found = false;

    bitSet(control, _LCDML_control_disable_hidden);
    bitSet(control, _LCDML_control_search_display);

    // got to root
    MENU_goRoot();
    if(activMenu != NULL)
    {
        // this function called be recursive (only once)
        loop_menu();
    }

    // search element
    switch(mode)
    {
        case 0: // jumpToFunc
        case 2: // setCursorToFunc
            found = OTHER_searchFunction(*rootMenu, 0, p_search, 0);
            break;

        case 1: // jumpToID
        case 3: // setCursorToID
            found = OTHER_searchFunction(*rootMenu, 1, NULL, p_id);
            break;

        default:
            // error
            break;
    }

    // reset jump param flag
    bitClear(funcReg, _LCDML_funcReg_jumpTo_w_para);

    // check element handling
    if(found == true)
    {
        bitClear(control, _LCDML_control_search_display);

        switch(mode)
        {
            case 0: // jumpToFunc
            case 1: // jumpToID
                // set jump param flag
                bitSet(funcReg, _LCDML_funcReg_jumpTo_w_para);
                jumpTo_w_para = p_para;
                // open menu element
                MENU_goInto();
                break;

            case 2: // setCursorToFunc
            case 3: // setCursorToID
            default:
                MENU_display();
                DISP_menuUpdate();
                break;
        }

        bitClear(control, _LCDML_control_disable_hidden);
        return true;
    }
    else
    {
        bitClear(control, _LCDML_control_search_display);
        bitClear(control, _LCDML_control_disable_hidden);

        return false;
    }
}


/* ******************************************************************** */
boolean LCDMenuLib2::OTHER_jumpToFunc(LCDML_FuncPtr_pu8 p_search, uint8_t p_para)
/* ******************************************************************** */
{
    MENU_enScroll();
    return OTHER_helpFunction(0, p_search, 0, p_para);
}

/* ******************************************************************** */
boolean LCDMenuLib2::OTHER_jumpToID(uint8_t p_id, uint8_t p_para)
/* ******************************************************************** */
{
    MENU_enScroll();
    return OTHER_helpFunction(1, NULL, p_id, p_para);
}

/* ******************************************************************** */
boolean LCDMenuLib2::OTHER_setCursorToFunc(LCDML_FuncPtr_pu8 p_search)
/* ******************************************************************** */
{
    MENU_enScroll();
    return OTHER_helpFunction(2, p_search, 0, 0);
}

/* ******************************************************************** */
boolean LCDMenuLib2::OTHER_setCursorToID(uint8_t p_id)
/* ******************************************************************** */
{
    MENU_enScroll();
    return OTHER_helpFunction(3, NULL, p_id, 0);
}


/* ******************************************************************** */
void    LCDMenuLib2::SCREEN_enable(LCDML_FuncPtr_pu8 function, unsigned long p_t)
/* ******************************************************************** */
{
    cb_screensaver           = function;
    screensaver_default_time = p_t;
}

/* ******************************************************************** */
void    LCDMenuLib2::SCREEN_disable()
/* ******************************************************************** */
{
   cb_screensaver = NULL;
}

/* ******************************************************************** */
void    LCDMenuLib2::SCREEN_resetTimer()
/* ******************************************************************** */
{
    TIMER_msReset(screensaver_timer);
}

/* ******************************************************************** */
void    LCDMenuLib2::SCREEN_start()
/* ******************************************************************** */
{
    screensaver_timer = millis() + 1000;
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_enScroll()
/* ******************************************************************** */
{
    bitClear(funcReg, _LCDML_funcReg_disable_scroll);
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_disScroll()
/* ******************************************************************** */
{
    if(activMenu == NULL)
    {
        bitSet(funcReg, _LCDML_funcReg_disable_scroll);
    }
    else
    {
        bitClear(funcReg, _LCDML_funcReg_disable_scroll);
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::MENU_getScrollDisableStatus()
/* ******************************************************************** */
{
    return bitRead(funcReg, _LCDML_funcReg_disable_scroll);
}
