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
 * ARDUIONO FORUM                                                    
 *     http://forum.arduino.cc/index.php?topic=73816.0  
 *   
 * ****************************************************************************** 
 */

#include "LCDMenuLib2.h"
#include "LCDMenuLib2_typedef.h"

/* ******************************************************************** */
LCDMenuLib2::LCDMenuLib2(LCDMenuLib2_menu &p_r, const uint8_t p_rows, const uint8_t p_cols, LCDML_FuncPtr contentUpdate, LCDML_FuncPtr contentClear, LCDML_FuncPtr   menuControl)
/* ******************************************************************** */
{
    // initialisation   
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
    // Check Function
    if(activMenu != NULL)
    {        
        if(TIMER_ms(menu_timer, menu_default_time) || bitRead(funcReg, _LCDML_funcReg_setup) == 0 || button > 0) 
        {            
            FUNC_call();            
        }
    } else {        
        menu_default_time = 100000000;
    }
}

/* ******************************************************************** */
boolean     LCDMenuLib2::MENU_selectElementDirect(LCDMenuLib2_menu &p_m, LCDML_FuncPtr_pu8 p_search)
/* ******************************************************************** */
{
    //deklaration
    LCDMenuLib2_menu *search = &p_m;
    LCDMenuLib2_menu *tmp;    
    boolean found    = false;
    
    if(activMenu == NULL)
    {
        do 
        {  
            if (search->getChild(0) != NULL) 
            {
                tmp = search->getChild(0);
                
                if(tmp->checkCallback() == true) 
                {
                    if (tmp->getCbFunction() == p_search) 
                    { //search elements in this layer            
                        found = true;
                    }
                    else
                    {
                        
                        found = MENU_selectElementDirect(*tmp, p_search); //recursive search until found is true or last item reached
                                    
                        if (found == false) //something found
                        { 
                            // go down
                            if (curloc < child_cnt) 
                            { 
                                curloc++; 
                                MENU_doScroll();
                            } 
                            child_cnt = MENU_countChilds();     
                        } 
                        
                    }                
                } 
                else
                { 
                    //check elements for childs                    
                    MENU_goInto();               
                    
                    if (tmp->getCbFunction() == p_search) 
                    { 
                        //search elements in this layer            
                        found = true;                   
                    }  
                    else
                    {
                        found = MENU_selectElementDirect(*tmp, p_search); //recursive search until found is true or last item reached
                                    
                        if (found == false) 
                        {                     
                            //quit layer, go to next element
                            MENU_goBack();
                            child_cnt = MENU_countChilds();
                            
                            if (curloc < child_cnt) 
                            { 
                                curloc++; 
                                MENU_doScroll();
                            }
                        } 
                    }                
                }           
            } 
            else 
            {  
                //no childs found                
                if (search->getCbFunction() == p_search) //found something
                {  
                    found = true;
                } 
                else 
                {                
                    //select next element            
                    if (curloc < child_cnt) { 
                        curloc++; 
                        MENU_doScroll();
                    }                
                }
            }        
        } while ((search=search->getSibling(1)) != NULL && found == 0);    
    }
    //return result
    return found;    
}

/* ******************************************************************** */
void        LCDMenuLib2::MENU_goRoot()
/* ******************************************************************** */
{
    if(activMenu != NULL) 
    {  
        bitSet(funcReg, _LCDML_funcReg_setup);  // disable setup
        bitSet(funcReg, _LCDML_funcReg_end);    // disable loop, enable close
        FUNC_call();  // call active function for save close;
    } 
    
    BT_resetAll();
    bitClear(funcReg, _LCDML_funcReg_disable_screensaver);
    bitClear(funcReg, _LCDML_funcReg_end);
    bitClear(funcReg, _LCDML_funcReg_setup);
    
    curMenu = rootMenu;
    
    layer = 0;
    cursor_pos = 0;
    curloc = 0;
    scroll = 0;        

    button = 0;    
    
    MENU_display();
    
    if(activMenu != NULL)
    {
        activMenu = NULL;
        if(bitRead(control, _LCDML_control_search_display) == false)
        {
             DISP_menuUpdate();
        }
    }   
}

/* ******************************************************************** */
uint8_t        LCDMenuLib2::MENU_curlocCorrection()
/* ******************************************************************** */
{
    uint8_t    curloc_cor = 0;
    uint8_t j = 0;
    LCDMenuLib2_menu *tmp;
    
    //correct function / menu element to open for hidden menu elements
    if ((tmp = curMenu->getChild(0)) != NULL) 
    {
        do
        {
            if (tmp->checkCondetion() || bitRead(control, _LCDML_control_disable_hidden)) 
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
        //go back
        bitWrite(control, _LCDML_control_menu_back, 1);
        //set current menu object         
        MENU_goMenu(*curMenu->getParent());       
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
            // Menufunction found 
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
                        if (tmp->checkCondetion() || bitRead(control, _LCDML_control_disable_hidden)) 
                        {                   
                            // Menu found, goInto
                            MENU_goMenu(*curMenu->getChild(curloc + MENU_curlocCorrection()));
                            
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
void    LCDMenuLib2::MENU_goMenu(LCDMenuLib2_menu &m)
/* ******************************************************************** */
{
    //declare variables
    int diff;

    curMenu = &m;
            
    if(layer < _LCDML_DISP_cfg_cursor_deep) { //check layer deep        
        if(bitRead(control, _LCDML_control_menu_back) == 0) { //check back button        
            // go into the next layer
            // save old layer data
            layer_save[layer] = curloc;
            scroll_save[layer] = scroll;
            // new layer
            scroll = 0;
            layer++;            
            // save parent id
            if(layer < _LCDML_DISP_cfg_cursor_deep) {
                parents[layer] = curMenu->getID();
            }            
            curloc = 0;
        } 
        else {
            //button reset
            bitWrite(control, _LCDML_control_menu_back, 0);
            
            if(layer > 0) {                
                layer--; 
                scroll = scroll_save[layer];
                curloc = layer_save[layer];

                if(curloc >= rows) {
                    diff = curloc-(rows-1);
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
    
    //check if element has childs
    if ((tmp = curMenu->getChild(0)) != NULL) {    
        do
        {                
            if (tmp->checkCondetion() || bitRead(control, _LCDML_control_disable_hidden)) {                
                j++;
            } 
        } while ((tmp = tmp->getSibling(1)) != NULL);        
    }    

    if(j == 0) {
        return 0;
    } else {    
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
    uint8_t maxi = (rows + scroll);
    LCDMenuLib2_menu *tmp;
        
    //check children
    if ((tmp = curMenu->getChild(i))) 
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
    while (curloc>0 && !curMenu->getChild(curloc))
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
void    LCDMenuLib2::FUNC_setLoopInterval(unsigned long t)
/* ******************************************************************** */
{
    menu_default_time = t;
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
        callback_contentUpdate();
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
        activMenu->callback(activMenu->getParam());
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
        bitClear(funcReg, _LCDML_funcReg_setup);
        return false; 
    }      
}

/* ******************************************************************** */
boolean LCDMenuLib2::FUNC_loop()
/* ******************************************************************** */
{
    if(activMenu != NULL) 
    {
        if(bitRead(funcReg, _LCDML_funcReg_end) == false) {
            return true;
        } else {
            return false;
        }  
    }
    else
    {
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
            bitClear(funcReg, _LCDML_funcReg_setup);
            activMenu = NULL;
            return true;
        } else {
            return false;
        }
    }
    else
    {
        return false;
    }
}

/* ******************************************************************** */
void LCDMenuLib2::FUNC_goBackToMenu(uint8_t e)
/* ******************************************************************** */
{
    if(activMenu != NULL) 
    {       
        bitSet(funcReg, _LCDML_funcReg_end);        
        FUNC_call();  // call active function for save close;
        activMenu = NULL;        
    }
    BT_resetAll();
    bitClear(funcReg, _LCDML_funcReg_disable_screensaver);
    bitClear(funcReg, _LCDML_funcReg_end);
    bitClear(funcReg, _LCDML_funcReg_setup);       
   
    if(e == 1) {
        MENU_goBack();
    }
    MENU_display();
    DISP_menuUpdate();    
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
    if(bitRead(control, _LCDML_control_setup) == 0) 
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
    SCREEN_resetTimer();
    
    if(activMenu == NULL) 
    {            
        //menu is active      
        MENU_goInto(); 
        bitSet(control, _LCDML_control_update_direct);
    }
    
} 

/* ******************************************************************** */
void LCDMenuLib2::BT_up()  
/* ******************************************************************** */              
{ 
    bitSet(button, _LCDML_button_up);
    SCREEN_resetTimer();

    if(activMenu == NULL) 
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
        BT_resetAll();
        DISP_menuUpdate(); 
    }
}

/* ******************************************************************** */
void LCDMenuLib2::BT_down() 
/* ******************************************************************** */             
{
    bitSet(button, _LCDML_button_down);
    SCREEN_resetTimer();
    
    if(activMenu == NULL) 
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
        BT_resetAll();
        DISP_menuUpdate(); 
    }
}

/* ******************************************************************** */
void LCDMenuLib2::BT_left()
/* ******************************************************************** */              
{
    bitSet(button, _LCDML_button_left);
    SCREEN_resetTimer();
    
    if(activMenu == NULL)
    {
        MENU_display();
        DISP_menuUpdate();
    }    
}

/* ******************************************************************** */
void LCDMenuLib2::BT_right() 
/* ******************************************************************** */            
{ 
    bitSet(button, _LCDML_button_right);
    SCREEN_resetTimer();
    
    if(activMenu == NULL)
    {
        MENU_display();
        DISP_menuUpdate();
    }        
}

/* ******************************************************************** */
void LCDMenuLib2::BT_quit() 
/* ******************************************************************** */             
{ 
    SCREEN_resetTimer();
    
    if(activMenu != NULL) 
    {
        FUNC_goBackToMenu();
    } 
    else 
    {
        if(layer > 0) 
        {
            MENU_goBack();
            MENU_display();
            DISP_menuUpdate();
        }
    }     
}

/* ******************************************************************** */ 
boolean LCDMenuLib2::BT_checkAny()       { if((button > 0)) { return true; } else { return false; }                              }     
/* ******************************************************************** */
boolean LCDMenuLib2::BT_checkEnter()     { if(bitRead(button, _LCDML_button_enter)) { return true; } else { return false; }      }
boolean LCDMenuLib2::BT_checkUp()        { if(bitRead(button, _LCDML_button_up)) { return true; } else { return false; }         }     
boolean LCDMenuLib2::BT_checkDown()      { if(bitRead(button, _LCDML_button_down)) { return true; } else { return false; }       }  
boolean LCDMenuLib2::BT_checkLeft()      { if(bitRead(button, _LCDML_button_left)) { return true; } else { return false; }       }  
boolean LCDMenuLib2::BT_checkRight()     { if(bitRead(button, _LCDML_button_right)) { return true; } else { return false; }      }  
/* ******************************************************************** */    
void LCDMenuLib2::BT_resetAll()          { button = 0;}
/* ******************************************************************** */
void LCDMenuLib2::BT_resetEnter()        { bitClear(button, _LCDML_button_enter);    }
void LCDMenuLib2::BT_resetUp()           { bitClear(button, _LCDML_button_up);       }
void LCDMenuLib2::BT_resetDown()         { bitClear(button, _LCDML_button_down);     }
void LCDMenuLib2::BT_resetLeft()         { bitClear(button, _LCDML_button_left);     }
void LCDMenuLib2::BT_resetRight()        { bitClear(button, _LCDML_button_right);    } 
/* ******************************************************************** */


/* ******************************************************************** */
void    LCDMenuLib2::TIMER_msReset(unsigned long &var)
/* ******************************************************************** */
{
    var = millis();
}

/* ******************************************************************** */
boolean LCDMenuLib2::TIMER_ms(unsigned long &var, unsigned long t)
/* ******************************************************************** */
{
    if((millis() - var) >= t) 
    {
        var = millis(); 
        return true;
    }
    else
    {
        return false;
    }
}

/* ******************************************************************** */
void    LCDMenuLib2::TIMER_usReset(unsigned long &var)
/* ******************************************************************** */
{
    var = micros();
}

/* ******************************************************************** */
boolean LCDMenuLib2::TIMER_us(unsigned long &var, unsigned long t)
/* ******************************************************************** */
{
    if((micros() - var) >= t) 
    {
        var = micros(); 
        return true;
    }
    else
    {
        return false;
    }
}

/* ******************************************************************** */
boolean LCDMenuLib2::OTHER_jumpToFunc(LCDML_FuncPtr_pu8 p_search)
/* ******************************************************************** */
{
    bitSet(control, _LCDML_control_disable_hidden);  
    bitSet(control, _LCDML_control_search_display);
    // got to root
    MENU_goRoot();
    
    if(MENU_selectElementDirect(*rootMenu, p_search)) 
    {  
        bitClear(control, _LCDML_control_search_display);
        MENU_goInto();
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
void    LCDMenuLib2::SCREEN_enable(LCDML_FuncPtr_pu8 function, unsigned long t)
/* ******************************************************************** */
{
    cb_screensaver = function;
    screensaver_default_time = t;    
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
    screensaver_timer = millis()+1000;
}
