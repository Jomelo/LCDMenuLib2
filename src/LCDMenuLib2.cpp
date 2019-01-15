/* ******************************************************************************
 *                        LCDMenuLib2 (LCDML)
 * ******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) [2019] [Nils Feldkämper]
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
    
    window_rows            = p_rows;
    // the cols variable is removed
   
    callback_contentUpdate  = contentUpdate;  // callback update content
    callback_contentClear   = contentClear;   // callback clear content
    callback_menuControl    = menuControl;    // callback buttons

    init();    
}

/* ******************************************************************** */
/* ******************************************************************** */
/* init methods                                                         */
/* ******************************************************************** */
/* ******************************************************************** */

/* ******************************************************************** */
void LCDMenuLib2::init()
/* ******************************************************************** */
{
    // init some variables
    REG_control             = 0;            
    REG_MenuFunction        = 0;
    REG_button              = 0;
    REG_special             = 0;
    REG_update              = 0;
    cursor_pos              = 0;
    layer                   = 0;    
    cursor_obj_pos          = 0;
    cursor_pos_abs          = 0;
    activMenu               = NULL;
    menu_timer              = 0;
    menu_default_time       = 100000000;

    // reset all buttons
    BT_resetAll();

    // set the cursor object position to the right postion
    // this is needed when the first and following elements are hidden
    MENU_initFirstElement();

    // update the current cursor possition
    // paramter 10 => do not scroll
    MENU_doScroll(10);
    // update the menu    
    MENU_display();
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
    else
    {
        // do nothing
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
                BT_resetAll();
                OTHER_jumpToFunc(cb_screensaver);
            }
            else
            {
                // do nothing
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

    // local declaration
    uint8_t cnt = 0;
    uint8_t obj_pos = 0;
    LCDMenuLib2_menu *tmp;
    boolean stop_while_loop = false;
    boolean found = false; 
         
   
    // ============================================
    // check control activity
    // ============================================
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
            else
            {
                // do nothing
            }

            // -- UP --
            if(bitRead(REG_special, _LCDML_REG_special_disable_scroll) == false)
            {
                if(BT_checkUp() == true)
                {  
                    MENU_doScroll(0); 
                }
                else
                {
                    // do nothing
                }

                // -- DOWN --
                if(BT_checkDown() == true)
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
                if(BT_checkUp() == true || BT_checkDown())
                {                   
                    DISP_update();
                }
                else
                {
                    // do nothing
                }
            } 

            // -- LEFT or RIGHT --
            if(BT_checkLeft() == true or BT_checkRight() == true)
            {
                DISP_update();
            }
            else
            {
                // do nothing
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
            if(BT_checkQuit() == true)
            {
                MENU_enScroll();
                FUNC_goBackToMenu();
                BT_resetQuit();
            }
            else
            {
                // do nothing
            }
        }
    }


    // ============================================
    // jump to handling
    // ============================================
    if(bitRead(REG_special, _LCDML_REG_special_jumpTo_enabled))
    {
        MENU_enScroll();

        bitClear(REG_special, _LCDML_REG_special_jumpTo_enabled); 
        bitClear(REG_control, _LCDML_REG_control_dynMenuDisplayed);

        bitSet(REG_control, _LCDML_REG_control_disable_hidden);
        bitSet(REG_control, _LCDML_REG_control_search_display);        

        // got to root
        if(activMenu != NULL)
        {
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

            BT_resetAll();

            activMenu = NULL;
            
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_close_active);            
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_end);
            bitClear(REG_MenuFunction, _LCDML_REG_MenuFunction_setup);

            bitClear(REG_special, _LCDML_REG_special_disable_screensaver);
                    
        }
        else
        {
            // do nothing            
        }

        // set the menu to root
        MENU_goRoot();

        // search element
        switch(jT_mode)
        {
            case 0: // jumpToFunc
            case 2: // setCursorToFunc
                found = OTHER_searchFunction(*rootMenu, 0, jT_function, 0);
                break;

            case 1: // jumpToID
            case 3: // setCursorToID
                found = OTHER_searchFunction(*rootMenu, 1, NULL, jT_id);
                break;

            default:
                // error
                break;
        }             

        // check element handling
        if(found == true)
        {      
            // get element to to open
            tmp = curMenu->getChild(cursor_obj_pos);                

            // reset the search action to do some normal handlings
            bitClear(REG_control, _LCDML_REG_control_search_display);

            
            switch(jT_mode)
            {
                case 0: // jumpToFunc
                case 1: // jumpToID

                    // check if element is a menu function and no dynamic function
                    if(tmp->checkCallback() == false || tmp->checkType_menu() == false)
                    {  
                        // only set the cursor to this element because it is not callable                        
                        // do nothing else here
                    } 
                    else
                    {
                        // open menu element
                        MENU_goInto();
                    }
                    break;

                case 2: // setCursorToFunc
                case 3: // setCursorToID
                
                    if(tmp->checkCondition() == false)
                    {                    
                        DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_helpFunction - callbackfunction is hidden, the cursor cannot be set to this possition: go back to root menu")); 
                        
                        bitClear(REG_control, _LCDML_REG_control_disable_hidden);
                        MENU_goRoot();                       
                    } 
                    else
                    {                        
                        // do nothing
                    }
                    break;            
                
                
                default:
                    // do nothing
                   
                    break;
            }

            // update the menu
            DISP_update();

            bitClear(REG_control, _LCDML_REG_control_disable_hidden); 

            // set current cursor possition without hidden elements
            if((tmp = curMenu->getChild(0)) != NULL)
            {
                cnt = 0;
                do
                {   
                    if(tmp->checkCondition() || bitRead(REG_control, _LCDML_REG_control_search_display) == true)
                    {             
                        if(obj_pos == cursor_obj_pos)
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
                    // count all elements
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
            // do nothing
        }

        bitClear(REG_control, _LCDML_REG_control_search_display);
        bitClear(REG_control, _LCDML_REG_control_disable_hidden);
    }
    else
    {
        // do nothing
    }
    



    // ============================================
    // check if a menu function is active
    // ============================================
    if(activMenu != NULL)
    {
        // check if the running function should be closed
        if(bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_end) == false && bitRead(REG_special, _LCDML_REG_special_goRoot) == false)
        {
            // => do not close the running function
            // check if a loop time was set or a button was pressed or the function is called the first time
            if(TIMER_ms(menu_timer, menu_default_time) == true || bitRead(REG_MenuFunction, _LCDML_REG_MenuFunction_setup) == false || REG_button > 0)
            {
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

            activMenu = NULL;
            
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
                for(uint8_t i=0; i<goBackCnt; i++)
                {
                    MENU_goBack();
                }
                goBackCnt = 0;                 
                
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
        menu_default_time = 100000000;
    }



    // ============================================
    // check update handling
    // ============================================
    if(bitRead(REG_update, _LCDML_REG_update_menu) == true || bitRead(REG_update, _LCDML_REG_update_cursor) == true)
    {  
        if(activMenu == NULL)
        {
            if(bitRead(REG_special, _LCDML_REG_special_disable_scroll) == false)
            {
                // clear button status from scrolling
                BT_resetUp();
                BT_resetDown();
            }
            else
            {
                // do nothing
            }

            callback_contentUpdate();
            BT_resetAll();
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
        BT_resetAll();        
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
    LCDMenuLib2_menu *tmp;
    uint8_t obj_pos = cursor_obj_pos;

    // get the current object
    if((tmp = curMenu->getChild(cursor_obj_pos)) != NULL)
    {
        do
        {   
            if(tmp->checkCondition() || bitRead(REG_control, _LCDML_REG_control_search_display) == true)
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

    if(activMenu != NULL)
    {
        bitSet(REG_special, _LCDML_REG_special_goRoot);
    }
    else
    {
        // check if root position is reached
        BT_resetAll();

        curMenu         = rootMenu;
        layer           = 0;
        cursor_pos      = 0;
        cursor_obj_pos  = 0;
        cursor_pos_abs  = 0;
        window_start    = 0;

        MENU_initFirstElement();

        MENU_doScroll(10);
        MENU_display();        
    }
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
    else
    {
        // do nothing
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_goInto(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_goInto"));

    // declaration
    LCDMenuLib2_menu *tmp;   

    // check if a menu function is not active
    if (activMenu == NULL)
    {
        // get element to to open
        tmp = curMenu->getChild(cursor_obj_pos);

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
                    //check if element has visible children 
                    if(MENU_countChilds(tmp) > 0)
                    {   
                        // Menu found, goInto
                        MENU_goMenu(*curMenu->getChild(cursor_obj_pos), false);

                        if(bitRead(REG_control, _LCDML_REG_control_search_display) == false)
                        {
                            BT_resetAll(); // reset all buttons
                            
                            DISP_update();
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
    }
    else
    {
        // do nothing
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::MENU_goMenu(LCDMenuLib2_menu &p_m, uint8_t p_back)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_goMenu"));

    //declare variables
    LCDMenuLib2_menu *tmp;    
    uint8_t old_id = curMenu->getID();
    uint8_t j = 0;
    uint8_t obj_pos = 0; 
    boolean found_visible = false;  
    
    // check if the menu is opening or closing
    if(p_back == false)
    {
        // set the curMenu pointer to a other menu element
        curMenu = &p_m;

        // only update the contet when the layer is smaller then 254
        if(layer < (_LCDML_NO_FUNC-1))
        {
            // new layer
            window_start = 0;        
            cursor_obj_pos = 0;
            cursor_pos_abs = 0;

            MENU_initFirstElement();

            // update the layer counter
            layer++;
        }
        else
        {
            // do nothing
        }
    }
    else
    {
        // update the layer counter
        if(layer > 0)
        {
            layer--;
        }
        else
        {
            // do nothing
        }

        // set the curMenu pointer to a other menu element
        curMenu = &p_m;

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
                if (tmp->checkCondition() || bitRead(REG_control, _LCDML_REG_control_disable_hidden))
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
    // debug information
    DBG_println(LCDML_DBG_function_name_MENU, F("LCDML.MENU_display"));

    //check children
    if(bitRead(REG_control, _LCDML_REG_control_search_display) == false)
    {     
        bitSet(REG_update, _LCDML_REG_update_menu);
    }
    else
    {
        // do nothing
    }
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
    boolean rollover = false;
    boolean update = false; 
    uint8_t child_cnt = MENU_countChilds(curMenu);   

    //content:
    // 0 = scroll up
    // 1 = scroll down

    // 10 = update cursor postion 

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
                                update = true;
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
                            if(tmp->checkCondition() == true || bitRead(REG_control, _LCDML_REG_control_search_display) == true)
                            {
                                cnt_visible++; 
                                last_valid_obj_pos = obj_pos; 
                                update = true;                         
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

            if(bitRead(REG_control, _LCDML_REG_control_rollover) && rollover == true && child_cnt > window_rows)
            {
                // rollover the menu
                if((tmp = curMenu->getChild(0)) != NULL)
                {
                    do
                    {  
                        if(tmp->checkCondition() == true || bitRead(REG_control, _LCDML_REG_control_search_display) == true)
                        {
                            cnt_visible++; 
                            last_valid_obj_pos = obj_pos; 
                            update = true;                         
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

            BT_resetUp();

            // reset the button state here or update the menu
            bitSet(REG_update, _LCDML_REG_update_menu);

        break;



        case 1: // down

            obj_pos = cursor_obj_pos;

            // get the next object if it exists
            if((tmp = curMenu->getChild(cursor_obj_pos)) != NULL)
            {                
                
                while((tmp = tmp->getSibling(1)) != NULL)
                {  
                    obj_pos++;

                    if(tmp->checkCondition() || bitRead(REG_control, _LCDML_REG_control_search_display) == true)
                    {
                        cnt_visible++;
                        cursor_pos_abs++;
                        cursor_obj_pos = obj_pos;
                        update = true;
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

            if(bitRead(REG_control, _LCDML_REG_control_rollover) && rollover == true && child_cnt > window_rows)
            {
                cursor_obj_pos  = 0;
                cursor_pos_abs  = 0;
                window_start    = 0;
                MENU_initFirstElement();
            }
            else
            {
                // do nothing
            }

            BT_resetDown();

            // reset the button state here or update the menu
            bitSet(REG_update, _LCDML_REG_update_menu);

        break;       

        default: // all others
            // do nothing
        break;
    }    


    // update window possition
    // only update the window position when no the menu is not rolled over
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
    if(activMenu == NULL &&  bitRead(REG_control, _LCDML_REG_control_search_display) == false)
    {       
        bitSet(REG_update, _LCDML_REG_update_cursor);     
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
                if (tmp->checkCondition() || bitRead(REG_control, _LCDML_REG_control_disable_hidden) || all == true)
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
            if(tmp->getParent() != NULL)
            {
                tmp = tmp->getParent();
            }
            else
            {
                return _LCDML_NO_FUNC;
                break;
            }
        }

        return tmp->getParent()->getID();
    }
    else
    {
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

    bitSet(REG_update, _LCDML_REG_update_menu);    
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
    else
    {
        // do nothing
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::DISP_checkMenuUpdate(void)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_DISP, F("LCDML.DISP_checkMenuUpdate"));

    if (bitRead(REG_update, _LCDML_REG_update_menu)) 
    {
        bitClear(REG_update, _LCDML_REG_update_menu);       
        bitClear(REG_control, _LCDML_REG_control_dynMenuDisplayed);
        return true;
    } 
    else 
    {
        return false;
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::DISP_checkMenuCursorUpdate(void)
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
    MENU_display();

    if(bitRead(REG_control, _LCDML_REG_control_search_display) == false)
    {
        bitSet(REG_update, _LCDML_REG_update_menu);
    }    
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
    
    LCDMenuLib2_menu *tmp;

    if(activMenu != NULL) 
    {
        return activMenu->getID();
    } 
    else 
    {
        if((tmp = curMenu->getChild(cursor_obj_pos)) != NULL)
        {
            if(tmp->checkType_dynParam() == true)
            {
                return tmp->getID();
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
        if(jT_param > 0)
        {
            activMenu->callback(jT_param);
        }
        else
        {
            activMenu->callback(activMenu->getParam());
        }
    }
    else
    {
        // do nothing
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
void LCDMenuLib2::BT_enter(void)            
/* ******************************************************************** */
{
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_enter"));       
    
    bitSet(REG_button, _LCDML_REG_button_enter);    
}

/* ******************************************************************** */
void LCDMenuLib2::BT_up(void)               
/* ******************************************************************** */
{ 
    // debug information
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_up"));  

    bitSet(REG_button, _LCDML_REG_button_up);       

}

/* ******************************************************************** */
void LCDMenuLib2::BT_down(void)             
/* ******************************************************************** */
{ 
    // debug information
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_down")); 

    bitSet(REG_button, _LCDML_REG_button_down);     
}

/* ******************************************************************** */
void LCDMenuLib2::BT_left(void)             
/* ******************************************************************** */
{ 
    // debug information
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_left")); 

    bitSet(REG_button, _LCDML_REG_button_left);     
}

/* ******************************************************************** */
void LCDMenuLib2::BT_right(void)            
/* ******************************************************************** */
{ 
    // debug information
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_right"));  

    bitSet(REG_button, _LCDML_REG_button_right);    
}

/* ******************************************************************** */
void LCDMenuLib2::BT_quit(void)             
/* ******************************************************************** */
{ 
    // debug information
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_quit")); 

    bitSet(REG_button, _LCDML_REG_button_quit);     
}

/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkAny(void)      
/* ******************************************************************** */
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkAny")); 

    if((REG_button > 0)) 
    { 
        return true; 
    } 
    else 
    { 
        return false; 
    }                            
}

/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkEnter(void)    
/* ******************************************************************** */
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkEnter"));  
    
    if(bitRead(REG_button, _LCDML_REG_button_enter)) 
    { 
        return true; 
    } 
    else 
    { 
        return false; 
    }    
}

/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkUp(void)       
/* ******************************************************************** */
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkUp"));     
    
    if(bitRead(REG_button, _LCDML_REG_button_up)) 
    { 
        return true; 
    } 
    else 
    { 
        return false; 
    }       
}

/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkDown(void) 
/* ******************************************************************** */    
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkDown")); 

    if(bitRead(REG_button, _LCDML_REG_button_down)) 
    { 
        return true; 
    } 
    else 
    { 
        return false; 
    }     
}

/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkLeft(void)     
/* ******************************************************************** */
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkLeft"));   
    
    if(bitRead(REG_button, _LCDML_REG_button_left)) 
    { 
        return true; 
    } 
    else 
    { 
        return false; 
    }    
}

/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkRight(void)  
/* ******************************************************************** */  
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkRight"));  
    
    if(bitRead(REG_button, _LCDML_REG_button_right)) 
    { 
        return true; 
    } 
    else 
    { 
        return false; 
    }    
}

/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkQuit(void)     
/* ******************************************************************** */
{ 
    // debug information     
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_checkQuit"));

    if(bitRead(REG_button, _LCDML_REG_button_quit)) 
    { 
        return true; 
    } 
    else 
    { 
        return false; 
    }     
}

/* ******************************************************************** */
void LCDMenuLib2::BT_resetAll(void)  
/* ******************************************************************** */       
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetAll"));    
    
    REG_button = 0;
}

/* ******************************************************************** */
void LCDMenuLib2::BT_resetEnter(void)   
/* ******************************************************************** */    
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetEnter"));  
    
    bitClear(REG_button, _LCDML_REG_button_enter);    
}
/* ******************************************************************** */
void LCDMenuLib2::BT_resetUp(void)          
/* ******************************************************************** */
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetUp"));     
    
    bitClear(REG_button, _LCDML_REG_button_up);       
}

/* ******************************************************************** */
void LCDMenuLib2::BT_resetDown(void)        
/* ******************************************************************** */
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetDown"));   
    
    bitClear(REG_button, _LCDML_REG_button_down);     
}

/* ******************************************************************** */
void LCDMenuLib2::BT_resetLeft(void)        
/* ******************************************************************** */
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetLeft"));   
    
    bitClear(REG_button, _LCDML_REG_button_left);     
}

/* ******************************************************************** */
void LCDMenuLib2::BT_resetRight(void)
/* ******************************************************************** */
{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetRight"));  
    
    bitClear(REG_button, _LCDML_REG_button_right);    
}

/* ******************************************************************** */
void LCDMenuLib2::BT_resetQuit(void)        
/* ******************************************************************** */

{ 
    // debug information 
    DBG_println(LCDML_DBG_function_name_BT, F("LCDML.BT_resetQuit"));   
    
    bitClear(REG_button, _LCDML_REG_button_quit);     
}
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
            DBG_print(LCDML_DBG_search, F(" : obj_pos : "));
            DBG_print(LCDML_DBG_search, cursor_obj_pos);
            DBG_print(LCDML_DBG_search, F(" : curMenu->ID : "));
            DBG_print(LCDML_DBG_search, curMenu->getID());
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

                        // go to next element (scroll down)                        
                        MENU_doScroll(1);
                        
                    }
                    else
                    {
                        // do nothing
                    }
                }
                else
                {
                    //select next element (scroll down)
                    MENU_doScroll(1);                    
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
void LCDMenuLib2::OTHER_jumpToFunc(LCDML_FuncPtr_pu8 p_search, uint8_t p_para)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToFunc"));

    bitSet(REG_special, _LCDML_REG_special_jumpTo_enabled);
    jT_mode     = 0;
    jT_id       = 0;
    jT_param    = p_para;
    jT_function = p_search;
}

/* ******************************************************************** */
void LCDMenuLib2::OTHER_jumpToID(uint8_t p_id, uint8_t p_para)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_jumpToID"));

    bitSet(REG_special, _LCDML_REG_special_jumpTo_enabled);
    jT_mode     = 1;
    jT_id       = p_id;
    jT_param    = p_para;
    jT_function = NULL;
}

/* ******************************************************************** */
void LCDMenuLib2::OTHER_setCursorToFunc(LCDML_FuncPtr_pu8 p_search)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToFunc"));

    bitSet(REG_special, _LCDML_REG_special_jumpTo_enabled);
    jT_mode     = 2;
    jT_id       = 0;
    jT_param    = 0;
    jT_function = p_search;
}

/* ******************************************************************** */
void LCDMenuLib2::OTHER_setCursorToID(uint8_t p_id)
/* ******************************************************************** */
{
    // debug information
    DBG_println(LCDML_DBG_function_name_OTHER, F("LCDML.OTHER_setCursorToID"));

    bitSet(REG_special, _LCDML_REG_special_jumpTo_enabled);
    jT_mode     = 3;
    jT_id       = p_id;
    jT_param    = 0;
    jT_function = NULL;    
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