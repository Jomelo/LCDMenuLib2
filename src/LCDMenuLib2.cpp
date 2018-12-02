/* ******************************************************************************
 *                        LCDMenuLib2 (LCDML)
 * ******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) [2018] [Nils Feldkämper]
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
    REG_control             = 0;            
    REG_MenuFunction        = 0;
    REG_button              = 0;
    cursor_pos              = 0;
    layer                   = 0;
    layer_save[0]           = 0;
    child_cnt               = 0;
    display_rows            = p_rows;
    // the cols variable is removed
    activMenu               = NULL;
    menu_timer              = 0;
    menu_default_time       = 100000000;
    callback_contentUpdate  = contentUpdate;  // callback update content
    callback_contentClear   = contentClear;   // callback clear content
    callback_menuControl    = menuControl;    // callback buttons

    BT_resetAll();
}

/* ******************************************************************** */
/* ******************************************************************** */
/* LOOP methods                                                         */
/* ******************************************************************** */
/* ******************************************************************** */


/* ******************************************************************** */
void LCDMenuLib2::loop(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_LOOP, F("LCDML.loop"));

    loop_control();
    loop_menu();
}

/* ******************************************************************** */
void LCDMenuLib2::loop_control(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_LOOP, F("LCDML.loop_control"));

    // check callback
    // check if this function is valid
    if(callback_menuControl != NULL)
    {
        // function is defined
        callback_menuControl();
    }

    // screensaver handling
    // check 
    if(cb_screensaver != NULL && bitRead(REG_special, _LCDML_REG_special_disable_screensaver) == false)
    {
        if(activMenu != NULL)
        {
            // a menu function is active
            if(activMenu->getCbFunction() != cb_screensaver) // check if screensaver is active
            {
                if(TIMER_ms(screensaver_timer, screensaver_default_time))
                {  
                    // close the running function 
                    FUNC_goBackToMenu(); 
                    loop_control();
                    REG_button = 0;
                    OTHER_jumpToFunc(cb_screensaver);
                }
            }
        }
        else
        {
            // only the menu is active
            if(TIMER_ms(screensaver_timer, screensaver_default_time))
            {
                 REG_button = 0;
                OTHER_jumpToFunc(cb_screensaver);
            }
        }
    }
}

/* ******************************************************************** */
void LCDMenuLib2::loop_menu(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_LOOP, F("LCDML.loop_menu"));

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
                bitSet(REG_control, _LCDML_REG_control_update_direct);
            }

            // -- UP --
            if(BT_checkUp() == true)
            {               
                if(bitRead(REG_special, _LCDML_REG_special_disable_scroll) == false)
                {                   
                    if (curloc > 0) {
                        curloc--;
                        MENU_doScroll();
                    }
                    else
                    {
                        if(bitRead(REG_control, _LCDML_REG_control_rollover))
                        {
                            // jump to the end of the menu
                            curloc = child_cnt;
                            if(child_cnt-display_rows < 0) {
                                scroll = 0;
                            } else {
                                scroll = child_cnt-display_rows;
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
                    DISP_update();
                }                
            }

            // -- DOWN --
            if(BT_checkDown() == true)
            {
                if(bitRead(REG_special, _LCDML_REG_special_disable_scroll) == false)
                {
                    if(curloc < child_cnt) {
                        curloc++;
                        MENU_doScroll();
                    }
                    else
                    {
                        if(bitRead(REG_control, _LCDML_REG_control_rollover))
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
                    DISP_update();
                }
            }


            // -- LEFT or RIGHT --
            if(BT_checkLeft() == true or BT_checkRight() == true)
            {
                DISP_update();
            }

            // -- QUIT Part 1 --
            if(BT_checkQuit() == true)
            {
                MENU_enScroll();

                if(layer > 0)
                {
                    MENU_goBack();
                    DISP_update();
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

    // check if a menu function is active
    if(activMenu != NULL)
    {
        // check if the running function should be closed
        if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_end) == false && bitRead(REG_special, _LCDML_REG_special_goRoot) == false)
        {
            // check if a loop time was set or a button was pressed or the function is called the first time
            if(TIMER_ms(menu_timer, menu_default_time) == true || bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_setup) == false || REG_button > 0)
            {
                // call the current menu function
                FUNC_call();
                // reset all buttons
                BT_resetAll();
            }
        }
        else
        {
            // set this bit to call the FUNC_close function when the goRoot function is called
            bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_end);

            if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_close_active) == false)
            {
                FUNC_call();    // call active function to bring it on a stable state;
            }
            BT_resetAll();

            activMenu = NULL;
            
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_close_active);
            bitClear(REG_special, _LCDML_REG_special_disable_screensaver);
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_end);
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_setup);
            bitClear(REG_special, _LCDML_REG_special_jumpTo_w_para);

            for(uint8_t i=0; i<goBackCnt; i++)
            {
                MENU_goBack();
            }
            goBackCnt = 0;

            if(bitRead(REG_special, _LCDML_REG_special_goRoot) == true)
            {
                // set the cursor to the root position
                bitClear(REG_special, _LCDML_REG_special_goRoot);

                curMenu     = rootMenu;
                layer       = 0;
                cursor_pos  = 0;
                curloc      = 0;
                scroll      = 0;
            }

            if(bitRead(REG_control, _LCDML_REG_control_search_display) == false)
            {
                // only update the content when no jump function is active
                DISP_update();
            }
        }
    }
    else
    {
        menu_default_time = 100000000;
    }
}


/* ******************************************************************** */
/* ******************************************************************** */
/* MENU methods                                                         */
/* ******************************************************************** */
/* ******************************************************************** */


/* ******************************************************************** */
void        LCDMenuLib2::MENU_goRoot(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_goRoot"));

    if(activMenu != NULL)
    {
        bitSet(REG_special, _LCDML_REG_special_goRoot);
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

        if(bitRead(REG_control, _LCDML_REG_control_search_display) == false)
        {
            // only update the content when no jump function is active
            MENU_display();
        }
    }
}

/* ******************************************************************** */
uint8_t        LCDMenuLib2::MENU_curlocCorrection(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_curlocCorrection"));

    uint8_t curloc_cor  = 0;
    uint8_t j           = 0;
    LCDMenuLib2_menu *tmp;

    //correct function / menu element to open for hidden menu elements
    if ((tmp = curMenu->getChild(0)) != NULL)
    {
        do
        {
            if (tmp->checkCondition() || bitRead(REG_control, _LCDML_REG_control_disable_hidden))
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
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_goBack"));

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
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_goInto"));

    LCDMenuLib2_menu *tmp;

    // check if a menu function is not active
    if (activMenu == NULL)
    {
        // get element to to open
        tmp = curMenu->getChild(curloc + MENU_curlocCorrection());

        // check if element is a menu function
        if(tmp->checkCallback() == true && tmp->checkType_menu() == true && bitRead(REG_control, _LCDML_REG_control_search_display) == false)
        {
            // Menu function found
            activMenu = tmp;
        }
        else
        {
            if(tmp->checkType_dynParam() == true && bitRead(REG_control, _LCDML_REG_control_search_display) == false)
            {
                DISP_update();
            }
            else
            {
                if(tmp->getChild(0) != NULL)
                {
                    while ((tmp = tmp->getSibling(0)) != NULL)
                    {
                        if (tmp->checkCondition() || bitRead(REG_control, _LCDML_REG_control_disable_hidden))
                        {
                            // Menu found, goInto
                            MENU_goMenu(*curMenu->getChild(curloc + MENU_curlocCorrection()), false);

                            if(bitRead(REG_control, _LCDML_REG_control_search_display) == false)
                            {
                                BT_resetAll(); // reset all buttons

                                child_cnt = MENU_countChilds();

                                DISP_update();
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
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_goMenu"));

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

                if(curloc >= display_rows)
                {
                    diff = curloc - display_rows + 1;
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
uint8_t    LCDMenuLib2::MENU_countChilds(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_countChilds"));

    //declaration
    uint8_t j = 0;
    LCDMenuLib2_menu *tmp;

    //check if element has children
    if ((tmp = curMenu->getChild(0)) != NULL)
    {
        do
        {
            if (tmp->checkCondition() || bitRead(REG_control, _LCDML_REG_control_disable_hidden))
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
LCDMenuLib2_menu * LCDMenuLib2::MENU_getObj(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getObj"));

    return curMenu;
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_display(uint8_t update)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_display"));

    //declaration
    uint8_t i = scroll;
    LCDMenuLib2_menu *tmp;

    //check children
    if((tmp = curMenu->getChild(i)))
    {
        bitSet(REG_control, _LCDML_REG_control_disp_update);
    }
    MENU_setCursor();
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getScroll(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getScroll"));

    return scroll;
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_setCursor(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_setCursor"));

    child_cnt = MENU_countChilds();

    //save current cursor position
    cursor_pos = curloc - scroll;

    if (cursor_pos > child_cnt) {
        cursor_pos = child_cnt;
    }

    bitSet(REG_control, _LCDML_REG_control_cursor_update);
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_doScroll(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_doScroll"));

    //only allow it to go up to menu element
    while (curloc > 0 && curMenu->getChild(curloc) == false)
    {
        curloc--;
    }

    //scroll down
    if (curloc >= (display_rows + scroll))
    {
        scroll++;
        if(bitRead(REG_control, _LCDML_REG_control_search_display) == false)
        {
            MENU_display();
        }
    }
    //scroll up
    else if (curloc < scroll)
    {
        scroll--;
        if(bitRead(REG_control, _LCDML_REG_control_search_display) == false)
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
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_enRollover"));

    bitSet(REG_control, _LCDML_REG_control_rollover);
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_disRollover(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_disRollover"));

    bitClear(REG_control, _LCDML_REG_control_rollover);
}

/* ******************************************************************** */
uint8_t    LCDMenuLib2::MENU_getLayer(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getLayer"));

    return layer;
}

/* ******************************************************************** */
uint8_t    LCDMenuLib2::MENU_getCursorPos(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getCursorPos"));

    return (cursor_pos); //return the current cursor position
}

/* ******************************************************************** */
uint8_t    LCDMenuLib2::MENU_getCursorPosAbs(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getCursorPosAbs"));

    return (curloc + MENU_curlocCorrection()); //return the current cursor position
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getChilds(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getChilds"));

    return child_cnt+1;
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getParentId(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getParentId"));

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
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getParentId"));

    if(p_layer > 0 && p_layer < _LCDML_DISP_cfg_cursor_deep) {
        return parents[p_layer];
    } else {
        return _LCDML_NO_FUNC;
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_setDynContent(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_setDynContent"));

    bitSet(REG_control, _LCDML_REG_control_dynMenuDisplayed);
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_checkDynContent(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_checkDynContent"));

    if(bitRead(REG_control, _LCDML_REG_control_dynMenuDisplayed))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_enScroll(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_enScroll"));

    bitClear(REG_special, _LCDML_REG_special_disable_scroll);
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_disScroll(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_disScroll"));

    if(activMenu == NULL)
    {
        bitSet(REG_special, _LCDML_REG_special_disable_scroll);
    }
    else
    {
        bitClear(REG_special, _LCDML_REG_special_disable_scroll);
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::MENU_getScrollDisableStatus(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getScrollDisableStatus"));

    return bitRead(REG_special, _LCDML_REG_special_disable_scroll);
}


/* ******************************************************************** */
/* ******************************************************************** */
/* DISP methods                                                         */
/* ******************************************************************** */
/* ******************************************************************** */


/* ******************************************************************** */
void LCDMenuLib2::DISP_menuUpdate(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_DISP, F("LCDML.DISP_menuUpdate"));

    if(activMenu == NULL || bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_end) || bitRead(REG_control, _LCDML_REG_control_update_direct) || bitRead(REG_control, _LCDML_REG_control_disp_update) )
    {
        if(bitRead(REG_special, _LCDML_REG_special_disable_scroll) == false)
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
void LCDMenuLib2::DISP_clear(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_DISP, F("LCDML.DISP_clear"));

    if(bitRead(REG_control, _LCDML_REG_control_search_display) == false)
    {
        callback_contentClear();
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::DISP_checkMenuUpdate(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_DISP, F("LCDML.DISP_checkMenuUpdate"));

    if (bitRead(REG_control, _LCDML_REG_control_disp_update) || bitRead(REG_control, _LCDML_REG_control_update_direct)) {
        bitClear(REG_control, _LCDML_REG_control_disp_update);
        bitClear(REG_control, _LCDML_REG_control_update_direct);
        bitClear(REG_control, _LCDML_REG_control_dynMenuDisplayed);
        return true;
    } else {
        return false;
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::DISP_checkMenuCursorUpdate(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_DISP, F("LCDML.DISP_checkMenuCursorUpdate"));

    if(bitRead(REG_control, _LCDML_REG_control_cursor_update)) {
        bitClear(REG_control, _LCDML_REG_control_cursor_update);
        return true;
    } else {
        return false;
    }
}

/* ******************************************************************** */
void LCDMenuLib2::DISP_update(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_DISP, F("LCDML.DISP_update"));

    MENU_display();
    DISP_menuUpdate();
}


/* ******************************************************************** */
/* ******************************************************************** */
/* FUNC methods                                                         */
/* ******************************************************************** */
/* ******************************************************************** */


/* ******************************************************************** */
uint8_t    LCDMenuLib2::FUNC_getID(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_getID"));

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
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_setLoopInterval"));

    menu_default_time = p_t;
}

/* ******************************************************************** */
void LCDMenuLib2::FUNC_call(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_call"));

    if(activMenu != NULL)
    {
        if(bitRead(REG_special, _LCDML_REG_special_jumpTo_w_para))
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
boolean LCDMenuLib2::FUNC_setup(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_setup"));

    if(activMenu != NULL)
    {
        // function active
        if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_setup) == 0) {
            bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_setup); // run setup
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
        bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_setup); // run setup
		
        return false;
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::FUNC_loop(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_loop"));


    if(activMenu != NULL)
    {
        if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_end) == false)
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
        bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_end);
		
        return false;
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::FUNC_close(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_close"));

    if(activMenu != NULL)
    {
        if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_end) == true)
        {
            bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_close_active);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_end);
		
        return false;
    }
}


/* ******************************************************************** */
void LCDMenuLib2::FUNC_goBackToMenu(uint8_t p_e)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_goBackToMenu"));

    if(activMenu != NULL)
    {
        goBackCnt = p_e;
        bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_end);
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::FUNC_disableScreensaver(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_disableScreensaver"));

    bitSet(REG_special, _LCDML_REG_special_disable_screensaver);
}


/* ******************************************************************** */
/* ******************************************************************** */
/* BT methods                                                           */
/* ******************************************************************** */
/* ******************************************************************** */


/* ******************************************************************** */
boolean LCDMenuLib2::BT_setup(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_setup"));

    // check if the button initialisation was done
    if(bitRead(REG_control, _LCDML_REG_control_bt_init_setup) == false)
    {
        bitSet(REG_control, _LCDML_REG_control_bt_init_setup);
        return true;
    }
    else
    {
        return false;
    }
}

/* ******************************************************************** */
void LCDMenuLib2::BT_enter(void)            { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_enter"));       bitSet(REG_button, _LCDML_REG_button_enter);    }
void LCDMenuLib2::BT_up(void)               { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_up"));          bitSet(REG_button, _LCDML_REG_button_up);       }
void LCDMenuLib2::BT_down(void)             { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_down"));        bitSet(REG_button, _LCDML_REG_button_down);     }
void LCDMenuLib2::BT_left(void)             { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_left"));        bitSet(REG_button, _LCDML_REG_button_left);     }
void LCDMenuLib2::BT_right(void)            { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_right"));       bitSet(REG_button, _LCDML_REG_button_right);    }
void LCDMenuLib2::BT_quit(void)             { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_quit"));        bitSet(REG_button, _LCDML_REG_button_quit);     }
/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkAny(void)      { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkAny"));    if((REG_button > 0)) { return true; } else { return false; }                            }
boolean LCDMenuLib2::BT_checkEnter(void)    { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkEnter"));  if(bitRead(REG_button, _LCDML_REG_button_enter)) { return true; } else { return false; }    }
boolean LCDMenuLib2::BT_checkUp(void)       { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkUp"));     if(bitRead(REG_button, _LCDML_REG_button_up)) { return true; } else { return false; }       }
boolean LCDMenuLib2::BT_checkDown(void)     { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkDown"));   if(bitRead(REG_button, _LCDML_REG_button_down)) { return true; } else { return false; }     }
boolean LCDMenuLib2::BT_checkLeft(void)     { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkLeft"));   if(bitRead(REG_button, _LCDML_REG_button_left)) { return true; } else { return false; }     }
boolean LCDMenuLib2::BT_checkRight(void)    { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkRight"));  if(bitRead(REG_button, _LCDML_REG_button_right)) { return true; } else { return false; }    }
boolean LCDMenuLib2::BT_checkQuit(void)     { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkQuit"));   if(bitRead(REG_button, _LCDML_REG_button_quit)) { return true; } else { return false; }     }
/* ******************************************************************** */
void LCDMenuLib2::BT_resetAll(void)         { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetAll"));    REG_button = 0;}
void LCDMenuLib2::BT_resetEnter(void)       { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetEnter"));  bitClear(REG_button, _LCDML_REG_button_enter);    }
void LCDMenuLib2::BT_resetUp(void)          { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetUp"));     bitClear(REG_button, _LCDML_REG_button_up);       }
void LCDMenuLib2::BT_resetDown(void)        { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetDown"));   bitClear(REG_button, _LCDML_REG_button_down);     }
void LCDMenuLib2::BT_resetLeft(void)        { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetLeft"));   bitClear(REG_button, _LCDML_REG_button_left);     }
void LCDMenuLib2::BT_resetRight(void)       { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetRight"));  bitClear(REG_button, _LCDML_REG_button_right);    }
void LCDMenuLib2::BT_resetQuit(void)        { DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetQuit"));   bitClear(REG_button, _LCDML_REG_button_quit);     }
/* ******************************************************************** */


/* ******************************************************************** */
/* ******************************************************************** */
/* TIMER methods                                                        */
/* ******************************************************************** */
/* ******************************************************************** */


/* ******************************************************************** */
void    LCDMenuLib2::TIMER_msReset(unsigned long &p_var)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_TIMER, F("LCDML.TIMER_msReset"));

    p_var = millis();
}

/* ******************************************************************** */
boolean LCDMenuLib2::TIMER_ms(unsigned long &p_var, unsigned long p_t)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_TIMER, F("LCDML.TIMER_ms"));

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
    // debug information
    DBG_println(LCDML_DBG_function_name_TIMER, F("LCDML.TIMER_usReset"));

    p_var = micros();
}

/* ******************************************************************** */
boolean LCDMenuLib2::TIMER_us(unsigned long &p_var, unsigned long p_t)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_TIMER, F("LCDML.TIMER_us"));

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
/* ******************************************************************** */
/* OTHER methods                                                        */
/* ******************************************************************** */
/* ******************************************************************** */


/* ******************************************************************** */
boolean     LCDMenuLib2::OTHER_searchFunction(LCDMenuLib2_menu &p_m, uint8_t mode, LCDML_FuncPtr_pu8 p_search, uint8_t p_id)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_searchFunction"));

    // declaration
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
                DBG_println(LCDML_DBG_search, F(""));

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
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_helpFunction"));

    boolean found = false;

    bitClear(REG_control, _LCDML_REG_control_dynMenuDisplayed);

    bitSet(REG_control, _LCDML_REG_control_disable_hidden);
    bitSet(REG_control, _LCDML_REG_control_search_display);
    // got to root
    if(activMenu != NULL)
    {
        // this function called be recursive (only once)
        FUNC_goBackToMenu();                    
        loop_menu();
        MENU_goRoot();
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
    bitClear(REG_special, _LCDML_REG_special_jumpTo_w_para);

    // check element handling
    if(found == true)
    {
        bitClear(REG_control, _LCDML_REG_control_search_display);
        
        switch(mode)
        {
            case 0: // jumpToFunc
            case 1: // jumpToID
                // set jump param flag
                bitSet(REG_special, _LCDML_REG_special_jumpTo_w_para);
                jumpTo_w_para = p_para;

                // open menu element
                MENU_goInto();
                break;

            case 2: // setCursorToFunc
            case 3: // setCursorToID
            default:
                // update the menu
                DISP_update();
                break;
        }

        bitClear(REG_control, _LCDML_REG_control_disable_hidden);
        return true;
    }
    else
    {
        bitClear(REG_control, _LCDML_REG_control_search_display);
        bitClear(REG_control, _LCDML_REG_control_disable_hidden);

        return false;
    }
}


/* ******************************************************************** */
boolean LCDMenuLib2::OTHER_jumpToFunc(LCDML_FuncPtr_pu8 p_search, uint8_t p_para)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToFunc"));

    MENU_enScroll();
    return OTHER_helpFunction(0, p_search, 0, p_para);
}

/* ******************************************************************** */
boolean LCDMenuLib2::OTHER_jumpToID(uint8_t p_id, uint8_t p_para)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToID"));

    MENU_enScroll();
    return OTHER_helpFunction(1, NULL, p_id, p_para);
}

/* ******************************************************************** */
boolean LCDMenuLib2::OTHER_setCursorToFunc(LCDML_FuncPtr_pu8 p_search)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToFunc"));

    MENU_enScroll();
    return OTHER_helpFunction(2, p_search, 0, 0);
}

/* ******************************************************************** */
boolean LCDMenuLib2::OTHER_setCursorToID(uint8_t p_id)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToID"));

    MENU_enScroll();
    return OTHER_helpFunction(3, NULL, p_id, 0);
}


/* ******************************************************************** */
/* ******************************************************************** */
/* SCREEN methods                                                       */
/* ******************************************************************** */
/* ******************************************************************** */


/* ******************************************************************** */
void    LCDMenuLib2::SCREEN_enable(LCDML_FuncPtr_pu8 function, unsigned long p_t)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_SCREEN, F("LCDML.SCREEN_enable"));

    cb_screensaver           = function;
    screensaver_default_time = p_t;
}

/* ******************************************************************** */
void    LCDMenuLib2::SCREEN_disable(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_SCREEN, F("LCDML.SCREEN_disable"));

    cb_screensaver = NULL;
}

/* ******************************************************************** */
void    LCDMenuLib2::SCREEN_resetTimer(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_SCREEN, F("LCDML.SCREEN_resetTimer"));

    TIMER_msReset(screensaver_timer);
}

/* ******************************************************************** */
void    LCDMenuLib2::SCREEN_start(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_SCREEN, F("LCDML.SCREEN_start"));

    screensaver_timer = millis() + 1000;
}