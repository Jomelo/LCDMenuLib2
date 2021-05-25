/* ******************************************************************************
 *                        LCDMenuLib2 (LCDML)
 * ******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) [2021] [Nils Feldkämper]
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
    // unused variables
    LCDML_UNUSED(p_cols);

    // initialization   
    curMenu                 = &p_r;    
    window_rows             = p_rows;    

    callback_contentUpdate  = contentUpdate;  // callback update content
    callback_contentClear   = contentClear;   // callback clear content
    callback_menuControl    = menuControl;    // callback buttons

    cb_screensaver          = NULL;
}

/* ******************************************************************** */
/* ******************************************************************** */
/* init methods                                                         */
/* ******************************************************************** */
/* ******************************************************************** */

/* ******************************************************************** */
void LCDMenuLib2::init(uint8_t p_last_element_id)
/* ******************************************************************** */
{
    // init some variables
    REG_control             = 0;            
    REG_MenuFunction        = 0;
    REG_button              = 0;
    REG_custom_event        = 0;
    REG_special             = 0;
    REG_update              = 0;
    cursor_pos              = 0;
    layer                   = 0;    
    cursor_obj_pos          = 0;
    cursor_pos_abs          = 0;
    menuFunction_loopTimer  = 0;
    last_element_id         = p_last_element_id;       

    MENU_resetActiveMenu();

    // reset custom events callback functions
    for(uint8_t i=0; i<_LCDML_CE_cb_function_cnt; i++)
    {
        ce_cb[i] = NULL;
    }

    // enable called from menu
    bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_called_from_menu);   

    // set the cursor object position to the right postion
    // this is needed when the first and following elements are hidden
    MENU_initFirstElement();

    // update the current cursor possition
    // paramter 10 => do not scroll
    MENU_doScroll(10);
    // update the menu    
    bitSet(REG_update, _LCDML_REG_update_menu); 
    DISP_menuUpdate();
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

    // call this function together
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
        // in this function the user defined button control is hanedled
        callback_menuControl();
    }
    else
    {
        // do nothing
    }    

    // check if a "jumpTo - setCursoTo" function is active
    // this check is necessary because there can be a bad timing between a timebased function call or a event and
    // another OTHER_...() function.
    if(bitRead(REG_special, _LCDML_REG_special_OTHER_function_active) == false)
    {
        // debug information
        DBG_println(LCDML_DBG_function_name_LOOP, F("LCDML.loop_control - check screensaver"));

        // screensaver handling
        // check if the screensaver is enabled and a callback function is defined
        if(bitRead(REG_special, _LCDML_REG_special_disable_screensaver) == false && cb_screensaver != NULL)
        {
            // check the state when menu is active do another handling as when a function is active
            if(actMenu_id != _LCDML_NO_FUNC)
            {
                // a menu function is active
                // when the screensaver function is active do nothing otherwise close the old function and call the screensaver 
                if(actMenu_cb_function != cb_screensaver) 
                {
                    if(TIMER_ms(screensaver_timer, screensaver_default_time))
                    {  
                        // close the running function 
                        FUNC_goBackToMenu(); 
                        loop_control();
                        REG_button = 0;

                        // debug information
                        DBG_println(LCDML_DBG_function_name_LOOP, F("LCDML.loop_control - jump to screensaver"));

                        // open the screensaver                                        
                        OTHER_jumpToFunc(cb_screensaver);                        
                    }
                    else
                    {
                        // do nothing
                    }
                }
                else
                {
                    // do nothing
                }
            }
            else
            {
                // only the menu is active
                if(TIMER_ms(screensaver_timer, screensaver_default_time))
                {
                    // reset all button states
                    REG_button = 0;

                    // debug information
                    DBG_println(LCDML_DBG_function_name_LOOP, F("LCDML.loop_control - jump to screensaver"));

                    // open the screensaver
                    OTHER_jumpToFunc(cb_screensaver);
                }
                else
                {
                    // do nothing
                }
            }
        }

        // check custom event handling
        if(CE_checkAny() == true)
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_LOOP, F("LCDML.loop_control - check event"));

            for(uint8_t i=0; i<_LCDML_CE_cb_function_cnt; i++)
            {
                // event found
                if(CE_check(i) == true)
                {               
                    // callback function found
                    if(ce_cb[i] != NULL)
                    {                   
                        // check if a function is running
                        if(actMenu_id == _LCDML_NO_FUNC)
                        {
                            // debug information
                            DBG_println(LCDML_DBG_function_name_LOOP, F("LCDML.loop_control - jump to event"));

                            // no function is active
                            // switch to new callback function
                            OTHER_jumpToFunc(ce_cb[i]);
                        }
                        else
                        {
                            // a function is active
                            // check if function is running
                            if(actMenu_cb_function != ce_cb[i])
                            {
                                // debug information
                                DBG_println(LCDML_DBG_function_name_LOOP, F("LCDML.loop_control - jump to event"));

                                // switch to new callback function
                                OTHER_jumpToFunc(ce_cb[i]);
                            }
                            else
                            {
                                // do nothing, function is running
                            }
                        }                   
                    }
                    else
                    {
                        // do nothing
                    }
                }
                else
                {
                    // do nothing
                }
            }
        }
        else
        {
            // do nothing
        } 
    }
}

/* ******************************************************************** */
void LCDMenuLib2::loop_menu(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_LOOP, F("LCDML.loop_menu"));

    // local declaration
    uint8_t cnt     = 0;   
    uint8_t found   = _LCDML_FIRST_ELEMENT_ID; 
    LCDMenuLib2_menu *tmp;

    // ============================================
    // check control activity
    // ============================================
    if(REG_button > 0)
    {
        SCREEN_resetTimer();       

        if(actMenu_id == _LCDML_NO_FUNC) 
        {
            // -- ENTER --
            if(bitRead(REG_button, _LCDML_REG_button_enter) == true)
            {
                //menu is active
                MENU_goInto();
                bitSet(REG_control, _LCDML_REG_control_update_direct);
            }
            else
            {
                // do nothing
            }

            // -- UP --
            if(bitRead(REG_special, _LCDML_REG_special_disable_scroll) == false)
            {
                if(bitRead(REG_button, _LCDML_REG_button_up) == true)
                {  
                    MENU_doScroll(0); 
                }
                else
                {
                    // do nothing
                }

                // -- DOWN --
                if(bitRead(REG_button, _LCDML_REG_button_down) == true)
                {       
                    MENU_doScroll(1);                    
                }
                else
                {
                    // do nothing
                }
            }
            else
            {   
                if(bitRead(REG_button, _LCDML_REG_button_up) == true || bitRead(REG_button, _LCDML_REG_button_down) == true)
                {                   
                    DISP_update();
                }
                else
                {
                    // do nothing
                }
            } 

            // -- LEFT or RIGHT --
            if(bitRead(REG_button, _LCDML_REG_button_left) == true or bitRead(REG_button, _LCDML_REG_button_right) == true)
            {
                DISP_update();
            }
            else
            {
                // do nothing
            }

            // -- QUIT Part 1 --
            if(bitRead(REG_button, _LCDML_REG_button_quit) == true)
            {
                MENU_enScroll();

                if(layer > 0)
                {
                    MENU_goBack();
                    DISP_update();
                }
                else
                {
                    // do nothing
                }
            }
            else
            {
                // do nothing
            }         
        }
        else
        {
            // -- QUIT Part 2 --
            if(bitRead(REG_button, _LCDML_REG_button_quit) == true)
            {
                MENU_enScroll();
                FUNC_goBackToMenu();
                bitClear(REG_button, _LCDML_REG_button_quit);
            }
            else
            {
                // do nothing
            }
        }
    }

    // ============================================
    // check control activity
    // ============================================
    if(CE_checkAny() == true)
    {     
        // reset screensaver timer
        SCREEN_resetTimer();        
    }
    
    // ============================================
    // jump to handling
    // ============================================
    if(bitRead(REG_special, _LCDML_REG_special_OTHER_function_active))
    {
        // debug information
        DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpTo.. / LCDML.OTHER_setCursorTo... - is active"));

        MENU_enScroll();

        bitClear(REG_special, _LCDML_REG_special_OTHER_function_active);         
 
        // stop running function and go back to the menu
        if(actMenu_id != _LCDML_NO_FUNC)
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpTo.. / LCDML.OTHER_setCursorTo... - close active function"));

            // this function called be recursive (only once)
            // set this bit to call the FUNC_close function when the goRoot function is called
            bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_end);            

            if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_close_active) == false)
            {
                FUNC_call();    // call active function to bring it on a stable state;
            }
            else
            {
                // do nothing
            }

            REG_button = 0;

            // Reset active menu
            MENU_resetActiveMenu();                                                     
            
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_close_active);            
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_end);
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_setup);

            bitClear(REG_special, _LCDML_REG_special_disable_screensaver);                    
        }
        else
        {
            // do nothing            
        }

        // reset the old parameter from the closed function / this parameter is set when a new jumpTo.. function is called
        jT_paramOld     = 0;
        
        // Root Menu
        curMenu         = MENU_getRootObj();
        layer           = 0;
        cursor_pos      = 0;
        cursor_obj_pos  = 0;
        cursor_pos_abs  = 0;
        window_start    = 0; 

        // Check if this Menu have childs
        if((curMenu = curMenu->getChild(0)) != NULL)
        {  
            // This loop runs through the menu structure and stops after checking the last element
            while(true)
            { 
                // Debug information
                DBG_print(LCDML_DBG_function_name_OTHER, F("ID: "));
                DBG_println(LCDML_DBG_function_name_OTHER, curMenu->getID());               

                // ***
                // check loop end condetions
                // ***

                // check if something is found:
                if(jT_function != NULL)
                {
                    // Debug information
                    DBG_println(LCDML_DBG_function_name_OTHER, F("check function"));
                           
                    if(curMenu->getCbFunction() == jT_function)
                    {
                        // something found                                               
                        found       = curMenu->getID();
                        break;
                    }
                }
                else
                {
                    // Debug Information
                    DBG_println(LCDML_DBG_function_name_OTHER, F("check id"));

                    if(curMenu->getID() == jT_id)
                    {
                        // something found                       
                        found   = curMenu->getID();
                        break;
                    }  
                } 
                
                // check if the last item is reached
                if(last_element_id == curMenu->getID())
                {
                    // Debug information                           
                    DBG_print(LCDML_DBG_function_name_OTHER, F("nothing found: endpoint (C) / ID: "));
                    DBG_println(LCDML_DBG_function_name_OTHER, curMenu->getID()); 

                    break;
                }

                // ***
                // set the next item
                // ***

                // check childs for childs
                if(curMenu->getChild(0) != NULL)
                {
                    // Debug information 
                    DBG_println(LCDML_DBG_function_name_OTHER, F("gointo an element"));

                    // -- GO INTO --
                    layer++;                    

                    // set next child for check
                    curMenu = curMenu->getChild(0);
                }
                else
                {
                    if(curMenu->getSibling(1) != NULL)
                    {
                        // Debug information 
                        DBG_println(LCDML_DBG_function_name_OTHER, F("set next sibling"));

                        // set next sibling for check
                        curMenu = curMenu->getSibling(1); 
                    }
                    else
                    { 
                        // check parent element
                        if(curMenu->getParent()->getID() == _LCDML_FIRST_ELEMENT_ID)
                        { 
                            // Debug information                           
                            DBG_print(LCDML_DBG_function_name_OTHER, F("nothing found: endpoint (A) / ID: "));
                            DBG_println(LCDML_DBG_function_name_OTHER, curMenu->getID()); 

                            // Here stands no break, because this element have to be checked (id - function_name)                                                   
                        }
                        else
                        {
                            // Debug information 
                            DBG_println(LCDML_DBG_function_name_OTHER, F("gointo an element"));

                            // all childs are checked
                            do
                            {
                                // set next parent for check
                                curMenu = curMenu->getParent(); 

                                // -- GO Back --
                                layer--;  

                                // Debug information 
                                DBG_println(LCDML_DBG_function_name_OTHER, F("goback to parent element"));                          
                                
                                // check if the parent element have siblings
                                if(curMenu->getSibling(1) != NULL)
                                {
                                    // set next sibling
                                    curMenu = curMenu->getSibling(1);
                                    break;                                
                                }
                                else
                                {                                    
                                    if(curMenu->getParent()->getID() == _LCDML_FIRST_ELEMENT_ID)
                                    {                                         
                                        break;
                                    }
                                    else
                                    {
                                        // continue                                        
                                    }                                                                        
                                }
                            } while (true);                          
                        }
                    }
                }
            }

            // set menu back to the parent layer
            curMenu = curMenu->getParent();
        }

        // check element handling
        if(found != _LCDML_FIRST_ELEMENT_ID)
        { 
            // debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpTo.. / LCDML.OTHER_setCursorTo... - element found"));
             
            tmp = curMenu->getChild(0);
            // get the current obj position in this layer             
            do
            {                            
                if(tmp->getID() == found)
                {                                                                                                                               
                    break; 
                }
                else
                {
                    // do nothing
                }

                // count elements
                cnt++;       
            }
            while((tmp = tmp->getSibling(1)) != NULL); 
            
            // tmp have here the pointer to the right position
            //tmp = --pointer_position--
            cursor_pos_abs =  cnt;
            cursor_obj_pos =  cnt;             
            
            if(bitRead(REG_special, _LCDML_REG_special_setCursorTo) == false)
            {
                // OTHER jump to function ...

                // check if element is a menu function and no dynamic function
                //if(tmp->checkCallback() == false || tmp->checkType_menu() == false)
                if(tmp->getChild(0) != NULL || tmp->checkType_menu() == false)
                {  
                    // only set the cursor to this element because it is not callable                        
                    // do nothing else here
                } 
                else
                {
                    // open menu element
                    MENU_goInto();
                }
            } 
            else
            {
                // OTHER set cursor to ... 
                if(tmp->checkCondition() == false)
                {    
                    // Debug information                 
                    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorTo.. - element is hidden, the cursor cannot be set to this possition: go back to root menu")); 
                    
                    MENU_goRoot();                       
                } 
                else
                {                        
                    // do nothing
                }                   
            }

            // update the menu
            DISP_update();

            // set current cursor possition with hidden elements
            tmp = curMenu->getChild(0);            
            cnt = 0;

            do
            {   
                if(tmp->checkCondition())
                {
                    if(tmp->getID() == found)
                    {
                        cursor_pos_abs =  cnt;                           
                        if(cursor_pos_abs - (window_rows-1) > 0)
                        {
                            window_start = cursor_pos_abs-(window_rows-1);
                        }
                        else
                        {
                            window_start = 0;
                        }
                        MENU_doScroll(10);                                                                    
                        break; 
                    }
                    else
                    {
                        // do nothing
                    }
                    // count visible elments
                    cnt++;
                }
                else
                { 
                    // do nothing                                
                }                           
            }
            while((tmp = tmp->getSibling(1)) != NULL);
        }
        else
        {
            // do nothing
            // debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpTo.. / LCDML.OTHER_setCursorTo... - nothing found -> goRoot"));
            MENU_goRoot();

            // update the menu
            DISP_update();
        }
    }
    else
    {
        // do nothing
    }
    

    // ============================================
    // check if a menu function is active
    // ============================================
    if(actMenu_id != _LCDML_NO_FUNC)
    {
        // check if the running function should be closed
        if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_end) == false && bitRead(REG_special, _LCDML_REG_special_goRoot) == false)
        {
            // => do not close the running function
            // check if a loop time was set or a button was pressed or the function is called the first time
            if( TIMER_ms(menuFunction_loopTimer, actMenu_default_time) == true || 
                bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_setup) == false || 
                REG_button > 0 || 
                CE_checkAny() > 0 ||
                bitRead(REG_update, _LCDML_REG_update_menu_function_content)
              )
            {
                // reset update flag
                bitClear(REG_update, _LCDML_REG_update_menu_function_content);

                // call the current menu function
                FUNC_call();                
            }
            else
            {
                // do nothing
            }
        }
        else
        {
            // close the running function 

            // set this bit to call the FUNC_close function when the goRoot function is called
            bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_end);

            if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_close_active) == false)
            {
                FUNC_call();    // call active function to bring it on a stable state;
            }
            else
            {
                // do nothing
            }  

            // Reset active menu
            MENU_resetActiveMenu();                                    

            // reset the custom parameter
            jT_param    = 0;            
            
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_close_active);
            bitClear(REG_special, _LCDML_REG_special_disable_screensaver);
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_end);
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_setup);            

            if(bitRead(REG_special, _LCDML_REG_special_goRoot) == true)
            {
                // set the cursor to the root position
                bitClear(REG_special, _LCDML_REG_special_goRoot);

                MENU_goRoot();              
            }
            else
            {
                // check go back                
                for(; goBackCnt > 0; goBackCnt--)
                {
                    MENU_goBack();
                }                                              
                
                // check if the cursor stands on the correct possition                 
                if(layer == 0)
                {
                    if(curMenu->getChild(cursor_obj_pos)->checkCondition() == false)
                    {
                        MENU_goRoot();
                    }
                    else
                    {
                        // do nothing
                    }
                }
                else
                {                    
                    // on going back, when the parent element is hidden, the condition is checken
                    // if the element is hidden, go to the next parent element and so on ... 
                    while(layer > 0 && MENU_countChilds(curMenu) == 0)
                    {
                        MENU_goBack();
                    }                    
                }
            }            
            
            // only update the content when no jump function is active
            DISP_update();            
        }
    }
    else
    {
       // do nothing
    }

    // ============================================
    // check update handling
    // ============================================
    if(bitRead(REG_update, _LCDML_REG_update_menu) == true || bitRead(REG_update, _LCDML_REG_update_cursor) == true)
    {   
        // save last cursor position
        if(bitRead(REG_update, _LCDML_REG_update_cursor) == true)
        {
            actMenu_lastCursorPositionID = actMenu_cursorPositionID;
            actMenu_cursorPositionID = curMenu->getChild(MENU_getCursorPosAbs())->getID();
        }

        if(actMenu_id == _LCDML_NO_FUNC)
        {            
            if(bitRead(REG_special, _LCDML_REG_special_disable_scroll) == false)
            {
                // clear button status from scrolling
                bitClear(REG_button, _LCDML_REG_button_up);
                bitClear(REG_button, _LCDML_REG_button_down);
            }
            else
            {
                // do nothing
            }

            //
            // detect if dynamic refreshed content is currently displayed 
            // some limit values
            cnt = MENU_getScroll();
            uint8_t maxi = window_rows + cnt;           

            MENU_clearDynRContent();
            
            if ((tmp = MENU_getDisplayedObj()) != NULL)
            {
                // loop to display lines
                do
                {
                    // check if a menu element has a condition and if the condition be true
                    if (tmp->checkCondition())
                    {
                        // call a dyn content element
                        if(tmp->checkType_dynParam_enabledCustomRefresh() == true)
                        {
                            // new function in version 2.2.7 to set a flag that dynamic enabled content is displayed
                            MENU_setDynRContent();
                            break;          
                        }
                        // increment some values
                        cnt++;                      
                    }
                // try to go to the next sibling and check the number of displayed rows
                } while (((tmp = tmp->getSibling(1)) != NULL) && (cnt < maxi));
            }
            // end of detection of dynamic refreshed content
                        
            // update content
            callback_contentUpdate();            
            REG_button = 0;            

            // this update function is neccessary for dynamic contents when a low element in the menu update the content of 
            // a higher element
            if(bitRead(REG_update, _LCDML_REG_update_update_dyn_content) == true)
            {
                // force a rebuild of the menu output content and cursor position                
                bitSet(REG_update, _LCDML_REG_update_menu); 
                bitSet(REG_update, _LCDML_REG_update_cursor);

                // update content without button handling
                callback_contentUpdate();  
 
                // clear this flag    
                MENU_clearDynFunctionContentUpdate();
            }
        }  
        else
        {            
            // do nothing
        }       

        // reset both values here because it can be happen on some examples, that not all values are reset
        bitClear(REG_update, _LCDML_REG_update_menu);
        bitClear(REG_update, _LCDML_REG_update_cursor);
    } 
    else
    {
        // reset all button actions
        REG_button = 0;      
    }       
}


/* ******************************************************************** */
/* ******************************************************************** */
/* MENU methods                                                         */
/* ******************************************************************** */
/* ******************************************************************** */


/* ******************************************************************** */
void        LCDMenuLib2::MENU_initFirstElement(void)
/* ******************************************************************** */
{
    // 
    uint8_t obj_pos = cursor_obj_pos;
    LCDMenuLib2_menu *tmp;

    // get the current object
    if((tmp = curMenu->getChild(cursor_obj_pos)) != NULL)
    {
        do
        {   
            if(tmp->checkCondition())
            {                
                cursor_obj_pos = obj_pos;                
                break;
            }
            else
            { 
                // do nothing                                
            }

            obj_pos++;             
        }
        while((tmp = tmp->getSibling(1)) != NULL);
    }
    else
    {
        // do nothing
    }

}

/* ******************************************************************** */
void        LCDMenuLib2::MENU_goRoot(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_goRoot"));

    if(actMenu_id != _LCDML_NO_FUNC)
    {
        bitSet(REG_special, _LCDML_REG_special_goRoot);
    }
    else
    {
        // check if root position is reached
        REG_button = 0;

        curMenu         = MENU_getRootObj();
        layer           = 0;
        cursor_pos      = 0;
        cursor_obj_pos  = 0;
        cursor_pos_abs  = 0;
        window_start    = 0;

        MENU_initFirstElement();

        MENU_doScroll(10);
        bitSet(REG_update, _LCDML_REG_update_menu);         
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_resetActiveMenu(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_resetActiveMenu"));

    // reset callback function
    actMenu_cb_function = NULL;

    // save last active function
    if(actMenu_id != _LCDML_NO_FUNC)
    {
        actMenu_lastFuncID = actMenu_id; 
    }

    // reset variables                                   
    actMenu_id              = _LCDML_NO_FUNC;                             
    actMenu_param           = 0; 
    actMenu_default_time    = 0xFFFFFFFF; // 32 Bit
    actMenu_ce_mask         = 0xFFFFFFFFFFFFFFFF; // 64 Bit    
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_goBack(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_goBack"));

    //declare variables
    LCDMenuLib2_menu *tmp;    
    uint8_t old_id = 0;
    uint8_t j = 0;
    uint8_t obj_pos = 0;
    bool found_visible = false;

    old_id  = curMenu->getID();
    tmp     = curMenu->getParent();
    
    // set the curMenu pointer to a other menu element
    curMenu = curMenu->getParent();   
    
    // update the layer counter
    if(layer > 0)
    {
        layer--;
    }
    else
    {
        // do nothing
    }       

    // init the cursor_position variables to 0
    cursor_obj_pos = 0;
    cursor_pos_abs = 0;

    // get childs from the current menu element
    if((tmp = curMenu->getChild(0)) != NULL)
    {
        // search the current possition of the cursor
        do
        {             
            // check conditions
            if (tmp->checkCondition())
            {                    
                // when the current element id is the same as the old_id the current cursor position is found
                if(tmp->getID() == old_id)
                {
                    cursor_pos_abs =  j;
                    cursor_obj_pos =  obj_pos;
                    if(cursor_pos_abs - (window_rows-1) > 0)
                    {
                        window_start = cursor_pos_abs-(window_rows-1);
                    }
                    else
                    {
                        window_start = 0;
                    }                        

                    MENU_doScroll(10);
                    found_visible = true;                                                
                    break; 
                }
                else
                {
                    // do nothing
                }

                // count all visible elements
                j++;
            }
            else
            {
                // do nothing
            }

            obj_pos++;
        // select the next sibling when it exists                
        } while ((tmp = tmp->getSibling(1)) != NULL);

        if(found_visible == false)
        {
            MENU_goRoot();
        }
    }
    else
    {
        // Something is wrong, go to a stable state (root menu)
        MENU_goRoot();
    }    
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_goInto(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_goInto"));

    // check if a menu function is not active
    if (actMenu_id == _LCDML_NO_FUNC)
    {
        // check if element is a menu function
        if(curMenu->getChild(cursor_obj_pos)->getChild(0) == NULL               // no children
            && curMenu->getChild(cursor_obj_pos)->checkType_menu() == true      // menu element
            && curMenu->getChild(cursor_obj_pos)->getCbFunction() != LCDML_cb_default_function) // default call back function
        {
            // Menu function found
            actMenu_cb_function     = curMenu->getChild(cursor_obj_pos)->getCbFunction(); 
            actMenu_id              = curMenu->getChild(cursor_obj_pos)->getID();                             
            actMenu_param           = curMenu->getChild(cursor_obj_pos)->getParam();                   
        }
        else
        {
            if(curMenu->getChild(cursor_obj_pos)->checkType_dynParam() == true                          // dynamic menu element
                && curMenu->getChild(cursor_obj_pos)->checkType_dynParam_enabledSubMenu() == false      // submenus disabled
                && bitRead(REG_control, _LCDML_REG_control_en_use_dyn_elements_as_menu) == false)       // dyn sub menus general disabled
            {               
                DISP_update();
            }
            else
            { 
                if(curMenu->getChild(cursor_obj_pos)->getChild(0) != NULL)
                {
                    //check if element has visible children 
                    if(MENU_countChilds(curMenu->getChild(cursor_obj_pos)) > 0)
                    { 
                        // set the curMenu pointer to a other menu element
                        curMenu = curMenu->getChild(cursor_obj_pos);
                        
                        // new layer
                        window_start = 0;        
                        cursor_obj_pos = 0;
                        cursor_pos_abs = 0;

                        MENU_initFirstElement();

                        // update the layer counter
                        layer++;                        

                        REG_button = 0; // reset all buttons                            
                        DISP_update();                                               
                    }
                    else
                    {
                        // do nothing
                    }
                }
                else
                {
                    if(curMenu->getChild(cursor_obj_pos)->checkType_dynParam() == true)
                    {
                        DISP_update();
                    }
                }
            }
        }
    }
    else
    {
        // do nothing
    }
}


/* ******************************************************************** */
LCDMenuLib2_menu * LCDMenuLib2::MENU_getCurrentObj(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getCurrentObj"));

    return curMenu;
}


/* ******************************************************************** */
LCDMenuLib2_menu * LCDMenuLib2::MENU_getRootObj(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getRootObj"));
    
    LCDMenuLib2_menu *tmp;

    if(curMenu->getID() != _LCDML_FIRST_ELEMENT_ID)
    {  
        tmp = curMenu;         
            
        while(true)
        {
            if(tmp->getID() != _LCDML_FIRST_ELEMENT_ID)
            {   
                tmp = tmp->getParent();             
            }
            else
            {            
                return tmp;
            }            
        }

        return tmp;
    }
    else
    {       
        return curMenu;
    }
}


/* ******************************************************************** */
LCDMenuLib2_menu * LCDMenuLib2::MENU_getDisplayedObj(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getDisplayedObj"));
    
    LCDMenuLib2_menu *tmp;
    uint8_t obj_pos = 0;
    uint8_t cnt_visible = 0;

    if(window_start > 0)
    {
        // get the current object
        if((tmp = curMenu->getChild(0)) != NULL)
        {
            do
            { 
                if(tmp->checkCondition())
                { 
                    if(cnt_visible == window_start)
                    {                        
                        break;
                    } 
                    else
                    {
                        cnt_visible++;  
                    }               
                }
                else
                { 
                    // do nothing                              
                }
                obj_pos++;

            }
            while((tmp = tmp->getSibling(1)) != NULL);
        }
        else
        {
            // do nothing
        }
    }
    else
    {
        obj_pos = 0;
    }   

    return curMenu->getChild(obj_pos);
}
   


/* ******************************************************************** */
void    LCDMenuLib2::MENU_display(uint8_t update)
/* ******************************************************************** */
{
    LCDML_UNUSED(update);
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_display"));
    
    bitSet(REG_update, _LCDML_REG_update_menu);      
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getScroll(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getScroll"));

    return window_start;
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_doScroll(uint8_t state)
/* ******************************************************************** */
{ 
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_doScroll"));    

    LCDMenuLib2_menu *tmp;
  
    uint8_t cnt_visible = 0;
    uint8_t obj_pos = 0;
    uint8_t last_valid_obj_pos = 0;
    bool rollover = false;   

    //content:
    // 0 = scroll up
    // 1 = scroll down

    switch(state)
    {
        case 0: // up

            obj_pos = 0;

            if(cursor_pos_abs > 0)
            {
                if((tmp = curMenu->getChild(0)) != NULL)
                {
                    do
                    { 
                        if(obj_pos == cursor_obj_pos)
                        {                           
                            if(cnt_visible > 0)
                            {
                                cursor_pos_abs--;
                                cursor_obj_pos = last_valid_obj_pos;
                            }
                            else
                            {    
                                // rollover is possible                            
                                rollover = true;                                
                            }
                            break;
                        }
                        else
                        {
                            if(tmp->checkCondition() == true)
                            {
                                cnt_visible++; 
                                last_valid_obj_pos = obj_pos;                       
                            } 
                            else
                            {
                                // do nothing
                            } 
                        }

                        obj_pos++;
                    }
                    while((tmp = tmp->getSibling(1)) != NULL);
                } 
                else
                {
                    // do nothing 
                }
            } 
            else
            {
               // rollover is possible 
               rollover = true;            
            }

            if(bitRead(REG_control, _LCDML_REG_control_rollover) && rollover == true)
            {
                // rollover the menu
                if((tmp = curMenu->getChild(0)) != NULL)
                {
                    do
                    {  
                        if(tmp->checkCondition() == true)
                        {
                            cnt_visible++; 
                            last_valid_obj_pos = obj_pos;                     
                        } 
                        else
                        {
                            // do nothing
                        }                    

                        obj_pos++;
                    }
                    while((tmp = tmp->getSibling(1)) != NULL);

                    cursor_pos_abs = cnt_visible-1;
                    cursor_obj_pos = last_valid_obj_pos;
                    if(cursor_pos_abs - (window_rows-1) > 0)
                    {
                        window_start = cursor_pos_abs-(window_rows-1);
                        // update menu content when window is moved
                        bitSet(REG_update, _LCDML_REG_update_menu);
                    }
                    else
                    {
                        window_start = 0;
                    }
                } 
                else
                {
                    // do nothing 
                }
            }
            else
            {
                // do nothing
            }

            bitClear(REG_button, _LCDML_REG_button_up); 
        break;

        case 1: // down

            obj_pos = cursor_obj_pos;

            // get the next object if it exists
            if((tmp = curMenu->getChild(cursor_obj_pos)) != NULL)
            {                
                
                while((tmp = tmp->getSibling(1)) != NULL)
                {  
                    obj_pos++;

                    if(tmp->checkCondition())
                    {
                        cnt_visible++;
                        cursor_pos_abs++;
                        cursor_obj_pos = obj_pos;
                        break;
                    }
                    else
                    { 
                        // do nothing                    
                    }
                }  

                if(cnt_visible == 0)
                {
                    rollover = true;
                }           
            }
            else
            {
                rollover = true;
            }

            if(bitRead(REG_control, _LCDML_REG_control_rollover) && rollover == true)
            {
                cursor_obj_pos  = 0;
                cursor_pos_abs  = 0;
                window_start    = 0;
                MENU_initFirstElement();
                // update menu content
                bitSet(REG_update, _LCDML_REG_update_menu);                
            }
            else
            {
                // do nothing
            }            

            // reset the button state here and update the menu
            bitClear(REG_button, _LCDML_REG_button_down);            

        break;       

        default: // all others
            // do nothing
        break;
    }   

    // update window possition
    // only update the window position when the menu is not rolled over
    if(rollover == false)
    {
        if(cursor_pos_abs - (window_rows-1) > 0 || window_start > 0)
        {
            switch(state)
            {
                case 0: // up
                    if(cursor_pos == 0) 
                    {
                        if(window_start > 0)
                        {
                            window_start--;
                            // update menu content when window is moved
                            bitSet(REG_update, _LCDML_REG_update_menu);
                        }
                        else
                        {
                            // do nothing
                        }                   
                    }
                    else
                    {
                        // do nothing
                    }
                break;

                case 1: // down                
                    if(cursor_pos == (window_rows-1))
                    {
                        window_start = cursor_pos_abs - (window_rows-1);
                        // update menu content when window is moved
                        bitSet(REG_update, _LCDML_REG_update_menu);                        
                    }
                    else
                    {
                        // do nothing
                    }                
                break;

                default: // all others 
                break;                  
            } 
        }
        else
        {
            window_start = 0;
        }
    }
    else
    {
        // do nothing
    }
    
    // update cursor position
    cursor_pos = cursor_pos_abs - window_start;         
  
    // check if the cursor position is updated
    if(actMenu_id == _LCDML_NO_FUNC)
    {       
        bitSet(REG_update, _LCDML_REG_update_cursor);
        if(bitRead(REG_update, _LCDML_REG_control_refresh_all_on_button_action) == true)
        {
            bitSet(REG_update, _LCDML_REG_update_menu);
        }    
    }   
    else
    {
        // do nothing
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
void    LCDMenuLib2::MENU_enUseDynElementsWithSubElements(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_enUseDynElementsWithSubElements"));

    bitSet(REG_control, _LCDML_REG_control_en_use_dyn_elements_as_menu);
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_disUseDynElementsWithSubElements(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_disUseDynElementsWithSubElements"));

    bitClear(REG_control, _LCDML_REG_control_en_use_dyn_elements_as_menu);
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

    return cursor_pos_abs; //return the current cursor position
}

/* ******************************************************************** */
uint8_t    LCDMenuLib2::MENU_getCursorObjPos(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getCursorObjPos"));

    return cursor_obj_pos; //return the current cursor position
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_countChilds(LCDMenuLib2_menu *menu, uint8_t all)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_countChilds"));

    //declaration
    uint8_t j = 0;
    LCDMenuLib2_menu *tmp;

    //check if element has children
    if(menu != NULL)
    {
        if ((tmp = menu->getChild(0)) != NULL)
        {
            do
            { 
                if (tmp->checkCondition() || all == true)
                {
                    j++;
                }
                else
                {
                    // do nothing
                }
            } while ((tmp = tmp->getSibling(1)) != NULL);
        }
        else
        {
            // do nothing
        }
    } 
    else
    {
        // do nothing
    }

    return j;
}


/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getChilds(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getChilds"));
    
    return MENU_countChilds(curMenu);
}


/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getParentID(uint8_t p_layer)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getParentID"));
    LCDMenuLib2_menu *tmp;

    if((tmp = curMenu->getChild(0)) != NULL)
    {
        for(uint8_t i=0; i<p_layer; i++)
        {
            if(tmp->getParent()->getID() != _LCDML_FIRST_ELEMENT_ID)
            {
                tmp = tmp->getParent();
            }
            else
            {
                return _LCDML_FIRST_ELEMENT_ID;
                break;
            }
        }

        return tmp->getParent()->getID();
    }
    else
    {
        return _LCDML_FIRST_ELEMENT_ID;
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

    if(actMenu_id == _LCDML_NO_FUNC)
    {
        bitSet(REG_special, _LCDML_REG_special_disable_scroll);
    }
    else
    {
        bitClear(REG_special, _LCDML_REG_special_disable_scroll);
    }
}

/* ******************************************************************** */
bool LCDMenuLib2::MENU_getScrollDisableStatus(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getScrollDisableStatus"));

    return bitRead(REG_special, _LCDML_REG_special_disable_scroll);
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getLastActiveFunctionID(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getLastActiveFunctionID"));

    return actMenu_lastFuncID;
}                   

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getLastCursorPositionID(void)                    
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getLastCursorPositionID"));

    return actMenu_lastCursorPositionID;
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::MENU_getElementIDFromCursorPos(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_getElementIDFromCursorPosition"));

    if(curMenu->getChild(cursor_obj_pos) != NULL)
    {
        return curMenu->getChild(cursor_obj_pos)->getID();
    }
    else
    {
        return _LCDML_FIRST_ELEMENT_ID;
    }    
}

/* ******************************************************************** */
void LCDMenuLib2::MENU_setDynFunctionContentUpdate(void) 
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_setMenuContentUpdate2"));

    bitSet(REG_update, _LCDML_REG_update_update_dyn_content);
}

/* ******************************************************************** */
void LCDMenuLib2::MENU_clearDynFunctionContentUpdate(void)
/* ******************************************************************** */
{
     // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_clearMenuContentUpdate2"));

    bitClear(REG_update, _LCDML_REG_update_update_dyn_content);
}

/* ******************************************************************** */
void LCDMenuLib2::MENU_allCondetionRefresh(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.MENU_allCondetionRefresh"));
    
    // define local variables
    LCDMenuLib2_menu *tmp;

    // set object
    tmp = MENU_getRootObj();     

    // Check if this Menu have childs
    if((tmp = tmp->getChild(0)) != NULL)
    {  
        // This loop runs through the menu structure and stops after checking the last element
        while(true)
        { 
            // Debug information
            DBG_print(LCDML_DBG_function_name_OTHER, F("ID: "));
            DBG_println(LCDML_DBG_function_name_OTHER, tmp->getID());               

            // ***
            // check loop end condetions
            // ***

            // check if something is found:
            
            // Debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("check function"));
                    
            tmp->updateCondetion();              

            if(last_element_id == tmp->getID())
            {
                // Debug information                           
                DBG_print(LCDML_DBG_function_name_OTHER, F("nothing found: endpoint (C) / ID: "));
                DBG_println(LCDML_DBG_function_name_OTHER, tmp->getID()); 

                break;
            }

            // ***
            // set the next item
            // ***

            // check childs for childs
            if(tmp->getChild(0) != NULL)
            {
                // Debug information 
                DBG_println(LCDML_DBG_function_name_OTHER, F("gointo an element"));

                // -- GO INTO --

                // set next child for check
                tmp = tmp->getChild(0);
            }
            else
            {
                if(tmp->getSibling(1) != NULL)
                {
                    // set next sibling for check
                    tmp = tmp->getSibling(1); 
                }
                else
                { 
                    // check parent element
                    if(tmp->getParent()->getID() == _LCDML_FIRST_ELEMENT_ID)
                    { 
                        // Debug information                           
                        DBG_print(LCDML_DBG_function_name_OTHER, F("end point: (A) / ID: "));
                        DBG_println(LCDML_DBG_function_name_OTHER, tmp->getID());                                                    
                    }
                    else
                    {
                        do
                        {
                            // set next parent for check
                            tmp = tmp->getParent(); 

                            // -- GO Back --                          

                            // Debug information 
                            DBG_println(LCDML_DBG_function_name_OTHER, F("goback to parent element"));                          
                            
                            // check if the parent element have siblings
                            if(tmp->getSibling(1) != NULL)
                            {
                                // set next sibling
                                tmp = tmp->getSibling(1);
                                break;                                
                            }
                            else
                            {                                    
                                if(tmp->getParent()->getID() == _LCDML_FIRST_ELEMENT_ID)
                                {
                                    break;
                                }
                                else
                                {
                                    // continue
                                }     
                            }
                        } while (true);                                                
                    }
                }
            }
        }        
    }   
}


/* ******************************************************************** */
void LCDMenuLib2::MENU_enRefreshAllOnButtonAction(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.MENU_enRefreshAllOnButtonAction"));

    bitSet(REG_control, _LCDML_REG_control_refresh_all_on_button_action);  

}


/* ******************************************************************** */
void LCDMenuLib2::MENU_disRefreshAllOnButtonAction(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.MENU_disRefreshAllOnButtonAction"));

    bitClear(REG_control, _LCDML_REG_control_refresh_all_on_button_action); 
}


/* ******************************************************************** */
void LCDMenuLib2::MENU_setDynRContent(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.MENU_setDynRContent"));

    bitSet(REG_control, _LCDML_REG_control_content_ref_is_displayed); 
}


/* ******************************************************************** */
void LCDMenuLib2::MENU_clearDynRContent(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.MENU_clearDynRContent"));

    bitClear(REG_control, _LCDML_REG_control_content_ref_is_displayed); 
}

/* ******************************************************************** */
bool LCDMenuLib2::MENU_checkDynRContent(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.MENU_checkDynRContent"));

    return bitRead(REG_control, _LCDML_REG_control_content_ref_is_displayed); 
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

    bitSet(REG_update, _LCDML_REG_update_menu);    
}

/* ******************************************************************** */
void LCDMenuLib2::DISP_clear(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_DISP, F("LCDML.DISP_clear"));
    
    callback_contentClear();    
}

/* ******************************************************************** */
bool LCDMenuLib2::DISP_checkMenuUpdate(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_DISP, F("LCDML.DISP_checkMenuUpdate"));

    if (bitRead(REG_update, _LCDML_REG_update_menu)) 
    {
        bitClear(REG_update, _LCDML_REG_update_menu);       
        return true;
    } 
    else 
    {
        return false;
    }
}

/* ******************************************************************** */
bool LCDMenuLib2::DISP_checkMenuCursorUpdate(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_DISP, F("LCDML.DISP_checkMenuCursorUpdate"));

    if(bitRead(REG_update, _LCDML_REG_update_cursor)) 
    {  
        bitClear(REG_update, _LCDML_REG_update_cursor);       
        return true;
    } 
    else 
    {
        return false;
    }
}

/* ******************************************************************** */
void LCDMenuLib2::DISP_update(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_DISP, F("LCDML.DISP_update"));

    MENU_doScroll(10);

    bitSet(REG_update, _LCDML_REG_update_menu);          
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
     
    if(actMenu_id != _LCDML_NO_FUNC) 
    {
        return actMenu_id;
    } 
    else 
    {
        if(curMenu->getChild(cursor_obj_pos) != NULL)
        {
            if(curMenu->getChild(cursor_obj_pos)->checkType_dynParam() == true)
            {
                return curMenu->getChild(cursor_obj_pos)->getID();
            }
            else
            {
                return _LCDML_NO_FUNC;
            }
        }
        else
        {
            return _LCDML_NO_FUNC;
        }
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::FUNC_setLoopInterval(unsigned long p_t)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_setLoopInterval"));

    actMenu_default_time = p_t;
}

/* ******************************************************************** */
void LCDMenuLib2::FUNC_call(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_call"));

    if(actMenu_id != _LCDML_NO_FUNC)
    {
        if(jT_paramOld > 0)
        {
            actMenu_cb_function(jT_paramOld);            
        }
        else if(jT_param > 0)
        {
            actMenu_cb_function(jT_param);
        }
        else
        {
            actMenu_cb_function(actMenu_param);
        }
    }
    else
    {
        // do nothing
    }
}

/* ******************************************************************** */
bool LCDMenuLib2::FUNC_setup(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_setup - check"));

    if(actMenu_id != _LCDML_NO_FUNC)
    {
        // function active
        if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_setup) == 0) 
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_setup - check: true"));

            bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_setup); // run setup
            DISP_clear();
            REG_button = 0;
            return true;
        }
        else
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_setup - check: false"));
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
bool LCDMenuLib2::FUNC_loop(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_loop - check"));

    if(actMenu_id != _LCDML_NO_FUNC)
    {
        if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_end) == false)
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_loop - check: true"));
            return true;
        }
        else
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_loop - check: false"));
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
bool LCDMenuLib2::FUNC_close(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_close - check"));

    if(actMenu_id != _LCDML_NO_FUNC)
    {
        if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_end) == true)
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_close - check: true"));

            bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_close_active);
            return true;
        }
        else
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_close - check: false"));
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
void LCDMenuLib2::FUNC_goBackToMenu(uint8_t p_goBackCnt)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_goBackToMenu"));

    // special handling
    if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_goBackToFuncID) == true)
    {
        bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_goBackToFuncID);        
        OTHER_jumpToID(actMenu_lastFuncID);
    } 
    
    // special handling
    if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_setCursorToID) == true)
    {
        bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_setCursorToID);
        OTHER_setCursorToID(actMenu_lastCursorPositionID);
    }        

    if(actMenu_id != _LCDML_NO_FUNC)
    {
        goBackCnt = p_goBackCnt;
        bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_end);
    }
    else
    {
        // do nothing
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
void    LCDMenuLib2::FUNC_setGBAToLastCursorPos(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_setGBAToLastCursorPos"));

    // delte other configurations
    bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_goBackToFuncID);

    // set new configuration
    bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_setCursorToID);
}

/* ******************************************************************** */
void    LCDMenuLib2::FUNC_setGBAToLastFunc(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_setGBAToLastFunc"));

    // delete other configurations
    bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_setCursorToID);

    // set new configuration
    bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_goBackToFuncID);
}

/* ******************************************************************** */
void    LCDMenuLib2::FUNC_setGBA(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_setGBA"));

    // decide which option is better (to function or to last cursor option)
    if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_called_from_menu) == true)
    {
        FUNC_setGBAToLastCursorPos();
    }
    else
    {
        FUNC_setGBAToLastFunc();
    }           
}

/* ******************************************************************** */
void    LCDMenuLib2::FUNC_setCEMask(unsigned long p_mask)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_FUNC, F("LCDML.FUNC_setCEMask"));

    if(actMenu_id != _LCDML_NO_FUNC)
    {
        actMenu_ce_mask = p_mask;
    }
    else
    {
        actMenu_ce_mask = 0xFFFFFFFFFFFFFFFF;
    }
}



/* ******************************************************************** */
/* ******************************************************************** */
/* BT and custom event methods                                          */
/* ******************************************************************** */
/* ******************************************************************** */

/* ******************************************************************** */
bool LCDMenuLib2::BT_setup(void)
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
bool LCDMenuLib2::CE_setup(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.CE_setup"));

    // check if the button initialisation was done
    if(bitRead(REG_control, _LCDML_REG_control_ce_init_setup) == false)
    {
        bitSet(REG_control, _LCDML_REG_control_ce_init_setup);
        return true;
    }
    else
    {
        return false;
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::CE_setOnChangeCbFunction(uint8_t p_event, LCDML_FuncPtr_pu8 p_function)
/* ******************************************************************** */
{
    // debug information 
    DBG_println(LCDML_DBG_function_name_CE, F("LCDML.CE_setOnChangeCbFunction"));
    
    if(p_event < _LCDML_CE_cb_function_cnt) 
    {
        ce_cb[p_event] = p_function;
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::CE_clearOnChangeCbFunction(uint8_t p_event)
/* ******************************************************************** */
{
    // debug information 
    DBG_println(LCDML_DBG_function_name_CE, F("LCDML.CE_clearOnChangeCbFunction"));
    
    if(p_event < _LCDML_CE_cb_function_cnt) 
    {
        ce_cb[p_event] = NULL;
    }
}


/* ******************************************************************** */
void LCDMenuLib2::REG_set(uint8_t p_reg, uint8_t p_val)
/* ******************************************************************** */
{
    // debug information 
    DBG_print(LCDML_DBG_function_name_BT, F("LCDML.REG_set("));
    DBG_print(LCDML_DBG_function_name_BT, p_reg);
    DBG_println(LCDML_DBG_function_name_BT, F(")"));

    switch(p_reg)
    {
        case _LCDML_REG_CTRL__button: 
            bitSet(REG_button, p_val); 
            break;
        case _LCDML_REG_CTRL__events: 
            bitSet(REG_custom_event, p_val); 
            break;

        default:    
            break;
    }    
}

/* ******************************************************************** */
void LCDMenuLib2::REG_reset(uint8_t p_reg, uint8_t p_val)
/* ******************************************************************** */
{
    // debug information 
    DBG_print(LCDML_DBG_function_name_BT, F("LCDML.REG_reset("));
    DBG_print(LCDML_DBG_function_name_BT, p_reg);
    DBG_println(LCDML_DBG_function_name_BT, F(")"));

    switch(p_reg)
    {
        case _LCDML_REG_CTRL__button: 
            bitClear(REG_button, p_val); 
            break;
        case _LCDML_REG_CTRL__events: 
            bitClear(REG_custom_event, p_val); 
            break;

        default:    
            break;
    }  
}

/* ******************************************************************** */
void LCDMenuLib2::REG_resetAll(uint8_t p_reg)  
/* ******************************************************************** */       
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.REG_resetAll"));    
    
     switch(p_reg)
    {
        case _LCDML_REG_CTRL__button: 
            REG_button = 0;
            break;
        case _LCDML_REG_CTRL__events: 
            REG_custom_event = 0;
            break;

        default:    
            break;
    } 
}

/* ******************************************************************** */
bool LCDMenuLib2::REG_check(uint8_t p_reg, uint8_t p_val)
/* ******************************************************************** */
{
    // debug information 
    DBG_print(LCDML_DBG_function_name_BT, F("LCDML.REG_check("));
    DBG_print(LCDML_DBG_function_name_BT, p_reg);
    DBG_println(LCDML_DBG_function_name_BT, F(")"));

    switch(p_reg)
    {
        case _LCDML_REG_CTRL__button: 
            return bitRead(REG_button, p_val);
            break;
        case _LCDML_REG_CTRL__events: 
            return bitRead(REG_custom_event, p_val);
            break;

        default:
            return false;    
            break;
    }     
}

/* ******************************************************************** */
bool LCDMenuLib2::REG_checkAny(uint8_t p_reg)      
/* ******************************************************************** */
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.REG_checkAny")); 

    switch(p_reg)
    {
        case _LCDML_REG_CTRL__button: 
            return (bool)(REG_button > 0); 
            break;
        case _LCDML_REG_CTRL__events: 
            return (bool)(REG_custom_event > 0);
            break;

        default: 
            return false;   
            break;
    }                             
}

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
bool LCDMenuLib2::TIMER_ms(unsigned long &p_var, unsigned long p_t)
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
bool LCDMenuLib2::TIMER_us(unsigned long &p_var, unsigned long p_t)
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
void LCDMenuLib2::OTHER_jumpToFunc(LCDML_FuncPtr_pu8 p_search, uint8_t p_para)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToFunc"));

    if(p_search == NULL)
    {       
        // debug information
        DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToFunc - no function selected"));
    }
    else
    {        
        if(bitRead(REG_special, _LCDML_REG_special_OTHER_function_active) == true)
        {           
            // debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToFunc - is still activ"));
        } 
        else 
        {            
            // check if this menu function is active - do nothing   
            if(p_search == actMenu_cb_function)
            {                
                // debug information
                DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToFunc - function is still running")); 
            }   
            else
            {                 
                // debug information
                DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToFunc - start"));

                bitSet(REG_special, _LCDML_REG_special_OTHER_function_active);
                bitClear(REG_special, _LCDML_REG_special_setCursorTo); 
                MENU_clearDynRContent(); 
                
                // Save last active Menu ID
                actMenu_lastFuncID = actMenu_id;

                if(actMenu_id != _LCDML_NO_FUNC)
                {
                    // handle parameters            
                    jT_paramOld = jT_param;
                    bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_called_from_menu);
                }
                else
                {
                    // handle parameters
                    jT_paramOld = 0;
                    bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_called_from_menu);

                    // Save last cursor position
                    actMenu_lastCursorPositionID = actMenu_cursorPositionID;
                    actMenu_cursorPositionID = curMenu->getChild(MENU_getCursorPosAbs())->getID();            
                }

                // enable jump to Func
                jT_id       = 0;        
                jT_param    = p_para;
                jT_function = p_search;
            }
        }   
    } 
}

/* ******************************************************************** */
void LCDMenuLib2::OTHER_jumpToID(uint8_t p_id, uint8_t p_para)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToID"));

    if(p_id != _LCDML_NO_FUNC && p_id <= last_element_id)
    {
        if(bitRead(REG_special, _LCDML_REG_special_OTHER_function_active) == true)
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToID - is still activ"));
        } 
        else 
        {
            // check if this menu function is active - do nothing  
            if(p_id == actMenu_id)
            {
                // debug information
                DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToFunc - function is still running"));
            }
            else
            {
                // debug information
                DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToID - start"));

                bitSet(REG_special, _LCDML_REG_special_OTHER_function_active);                
                bitClear(REG_special, _LCDML_REG_special_setCursorTo);
                MENU_clearDynRContent();               

                // Save last active Menu ID           
                actMenu_lastFuncID = actMenu_id;
                
                if(actMenu_id != _LCDML_NO_FUNC)
                {
                    // handle parameters
                    jT_paramOld = jT_param;
                    bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_called_from_menu);
                }
                else
                {
                    // handle parameters
                    jT_paramOld = 0;

                    bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_called_from_menu);

                    // Save last cursor position
                    actMenu_lastCursorPositionID = actMenu_cursorPositionID;
                    actMenu_cursorPositionID = curMenu->getChild(MENU_getCursorPosAbs())->getID();                    
                }

                // enable jump to ID 
                jT_id       = p_id;
                jT_param    = p_para;
                jT_function = NULL;
            }
        }
    }
    else
    {
        // debug information
        DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToID - stop - ID range is wrong"));        
    }
}

/* ******************************************************************** */
void LCDMenuLib2::OTHER_setCursorToFunc(LCDML_FuncPtr_pu8 p_search)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToFunc"));

    if(p_search == NULL)
    {
        // debug information
        DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToFunc - no function selected"));
    }
    else
    {
        if(bitRead(REG_special, _LCDML_REG_special_OTHER_function_active) == true)
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToFunc - is still activ"));
        } 
        else 
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToFunc - start"));

            // enable jump to Func
            bitSet(REG_special, _LCDML_REG_special_OTHER_function_active);
            bitSet(REG_special, _LCDML_REG_special_setCursorTo);
            MENU_clearDynRContent();
            jT_id       = 0;

            // Save last active Menu ID           
            actMenu_lastFuncID = actMenu_id;
        
            if(actMenu_id != _LCDML_NO_FUNC)
            {
                // handle parameters            
                jT_paramOld = jT_param; 
                bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_called_from_menu);          
            }
            else
            {
                // handle parameters
                jT_paramOld = 0;

                bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_called_from_menu);

                // Save last cursor position
                actMenu_lastCursorPositionID = actMenu_cursorPositionID;
                actMenu_cursorPositionID = curMenu->getChild(MENU_getCursorPosAbs())->getID();            
            } 
            
            jT_param    = 0;
            jT_function = p_search;
        }
    }
}

/* ******************************************************************** */
void LCDMenuLib2::OTHER_setCursorToID(uint8_t p_id)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToID"));

    if(p_id != _LCDML_NO_FUNC && p_id <= last_element_id)
    {
        if(bitRead(REG_special, _LCDML_REG_special_OTHER_function_active) == true)
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToID - is still activ"));
        } 
        else 
        {
            // debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToID - start"));

            // enable jump to Func
            bitSet(REG_special, _LCDML_REG_special_OTHER_function_active);
            bitSet(REG_special, _LCDML_REG_special_setCursorTo);
            MENU_clearDynRContent();
            jT_id       = p_id;

            // Save last active Menu ID           
            actMenu_lastFuncID = actMenu_id;       

            if(actMenu_id != _LCDML_NO_FUNC)
            {
                // handle paramerts
                jT_paramOld = jT_param;
                bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_called_from_menu);            
            }
            else
            {
                // handle parameters
                jT_paramOld = 0;

                bitSet(REG_MenuFunction, _LCDML_REG_MenuFunction_called_from_menu);

                // save last cursor position
                actMenu_lastCursorPositionID = actMenu_cursorPositionID;
                actMenu_cursorPositionID = curMenu->getChild(MENU_getCursorPosAbs())->getID();                
            } 
            
            jT_param    = 0;
            jT_function = NULL;  
        } 
    }
    else
    {
        // debug information
        DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToID - stop - ID range is wrong")); 
    }
}

/* ******************************************************************** */
uint8_t LCDMenuLib2::OTHER_getIDFromFunction(LCDML_FuncPtr_pu8 p_function_name)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_getIDFromFunction"));
    
    // define local variables
    LCDMenuLib2_menu *tmp;

    // set object
    tmp = MENU_getRootObj();     

    // Check if this Menu have childs
    if((tmp = tmp->getChild(0)) != NULL)
    {  
        // This loop runs through the menu structure and stops after checking the last element
        while(true)
        { 
            // Debug information
            DBG_print(LCDML_DBG_function_name_OTHER, F("ID: "));
            DBG_println(LCDML_DBG_function_name_OTHER, tmp->getID());               

            // ***
            // check loop end condetions
            // ***

            // check if something is found:
            
            // Debug information
            DBG_println(LCDML_DBG_function_name_OTHER, F("check function"));
                    
            if(tmp->getCbFunction() == p_function_name)
            {
                // something found                                               
                return tmp->getID();                
            }
                 

            if(last_element_id == tmp->getID())
            {
                // Debug information                           
                DBG_print(LCDML_DBG_function_name_OTHER, F("nothing found: endpoint (C) / ID: "));
                DBG_println(LCDML_DBG_function_name_OTHER, tmp->getID()); 

                break;
            }

            // ***
            // set the next item
            // ***

            // check childs for childs
            if(tmp->getChild(0) != NULL)
            {
                // Debug information 
                DBG_println(LCDML_DBG_function_name_OTHER, F("gointo an element"));

                // -- GO INTO --

                // set next child for check
                tmp = tmp->getChild(0);
            }
            else
            {
                if(tmp->getSibling(1) != NULL)
                {
                    // set next sibling for check
                    tmp = tmp->getSibling(1); 
                }
                else
                { 
                    // check parent element
                    if(tmp->getParent()->getID() == _LCDML_FIRST_ELEMENT_ID)
                    { 
                        // Debug information                           
                        DBG_print(LCDML_DBG_function_name_OTHER, F("end point: (A) / ID: "));
                        DBG_println(LCDML_DBG_function_name_OTHER, tmp->getID());                                                    
                    }
                    else
                    {
                        do
                        {
                            // set next parent for check
                            tmp = tmp->getParent(); 

                            // -- GO Back --                          

                            // Debug information 
                            DBG_println(LCDML_DBG_function_name_OTHER, F("goback to parent element"));                          
                            
                            // check if the parent element have siblings
                            if(tmp->getSibling(1) != NULL)
                            {
                                // set next sibling
                                tmp = tmp->getSibling(1);
                                break;                                
                            }
                            else
                            {                                    
                                if(tmp->getParent()->getID() == _LCDML_FIRST_ELEMENT_ID)
                                {
                                    break;
                                }
                                else
                                {
                                    // continue
                                }     
                            }
                        } while (true);                                                
                    }
                }
            }
        }        
    }

    // nothing found
    return _LCDML_NO_FUNC;
}

/* ******************************************************************** */
void    LCDMenuLib2::OTHER_updateFunc(void)                               
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_updateFunc"));

    if(actMenu_id != _LCDML_NO_FUNC)
    {
        bitSet(REG_update, _LCDML_REG_update_menu_function_content);
    }
    else
    {
        // do nothing
    }
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

    if(function != NULL)
    {
        cb_screensaver           = function;
        screensaver_default_time = p_t;
    }
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