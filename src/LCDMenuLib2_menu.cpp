/* ******************************************************************************
 *                        LCDMenuLib2 (LCDML)
 * ******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) [2025] [Nils Feldkämper]
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

#include "LCDMenuLib2_menu.h"
#include "LCDMenuLib2_typedef.h"


/* ******************************************************************** */
LCDMenuLib2_menu::LCDMenuLib2_menu(uint8_t p_id, uint8_t p_param, uint8_t p_configuration, LCDML_FuncPtr_pu8 p_callback_function, LCDML_FuncPtr_rb p_callback_condition)
/* ******************************************************************** */
{
    parent  = NULL;
    sibling = NULL;
    child   = NULL;
    id      = p_id;        // element name
    param   = p_param;     // element configuration

    REG_control = p_configuration;    

    if(p_callback_function == NULL) 
    { 
        cb_function = LCDML_cb_default_function; 
    } 
    else 
    { 
        cb_function = p_callback_function; 
    }

    if(p_callback_condition == NULL) 
    { 
        cb_condition = LCDML_cb_default_condetion; 
    } 
    else 
    { 
        cb_condition = p_callback_condition; 
    }  

    // update some things
    updateCondetion();  
}

/* ******************************************************************** */
void LCDMenuLib2_menu::setParent(LCDMenuLib2_menu &p)
/* ******************************************************************** */
{
    parent = &p;
}

/* ******************************************************************** */
void LCDMenuLib2_menu::addSibling(LCDMenuLib2_menu &s,LCDMenuLib2_menu &p)
/* ******************************************************************** */
{
    if (sibling)
    { // add sibling if sibling exists
        sibling->addSibling(s,p);
    }
    else
    { // add sibling and parent
        sibling =& s;
        sibling->setParent(p);
    }
}

/* ******************************************************************** */
void LCDMenuLib2_menu::addChild(LCDMenuLib2_menu &c)
/* ******************************************************************** */
{
    if (child)
    { // add sibling if child exists
        child->addSibling(c,*this);
    }
    else
    { // add child  and parent
        child=&c;
        child->setParent(*this);
    }
}

/* ******************************************************************** */
LCDMenuLib2_menu * LCDMenuLib2_menu::getChild(uint8_t which)
/* ******************************************************************** */
{
    if (child)
    { // return child if exists
        return child->getSibling(which);
    }
    else
    { // this menu item has no children
        return NULL;
    }
}

/* ******************************************************************** */
LCDMenuLib2_menu * LCDMenuLib2_menu::getSibling(uint8_t howfar)
/* ******************************************************************** */
{
    if (howfar == 0)
    { // this sibling
        return this;
    }
    else if (sibling)
    {    // get next sibling
        return sibling->getSibling(howfar - 1);
    }
    else
    { // asking for a nonexistent sibling
        return NULL;
    }
}

/* ******************************************************************** */
LCDMenuLib2_menu * LCDMenuLib2_menu::getParent()
/* ******************************************************************** */
{
    if (parent)
    { // get parent if exists
        return parent;
    }
    else
    {   // root menu       
        return this;
    }
}

/* ******************************************************************** */
uint8_t LCDMenuLib2_menu::getID()
/* ******************************************************************** */
{
    return id;
}

/* ******************************************************************** */
bool LCDMenuLib2_menu::checkCondition()
/* ******************************************************************** */
{  
    return bitRead(REG_control, _LCDML_REG_MENU_CONTROL_condetion);       
}

/* ******************************************************************** */
void LCDMenuLib2_menu::updateCondetion()
/* ******************************************************************** */
{
    if(cb_condition() == true)
    {
        bitSet(REG_control, _LCDML_REG_MENU_CONTROL_condetion);
    }
    else
    {
        bitClear(REG_control, _LCDML_REG_MENU_CONTROL_condetion);
    }
}

/* ******************************************************************** */
LCDML_FuncPtr_pu8 LCDMenuLib2_menu::getCbFunction()
/* ******************************************************************** */
{
    return cb_function;
}

/* ******************************************************************** */
uint8_t LCDMenuLib2_menu::getParam()
/* ******************************************************************** */
{
    return param;
}

/* ******************************************************************** */
void LCDMenuLib2_menu::callback(uint8_t p)
/* ******************************************************************** */
{
    cb_function(p);    
}

/* ******************************************************************** */
bool LCDMenuLib2_menu::checkType_menu()
/* ******************************************************************** */
{
    return bitRead(REG_control, _LCDML_REG_MENU_CONTROL_type_default);    
}

/* ******************************************************************** */
bool LCDMenuLib2_menu::checkType_dynParam()
/* ******************************************************************** */
{
    return bitRead(REG_control, _LCDML_REG_MENU_CONTROL_type_dynParam);    
}

/* ******************************************************************** */
bool LCDMenuLib2_menu::checkType_dynParam_enabledSubMenu()
/* ******************************************************************** */
{
    return bitRead(REG_control, _LCDML_REG_MENU_CONTROL_type_dynParam);    
}

/* ******************************************************************** */
bool LCDMenuLib2_menu::checkType_dynParam_enabledCustomRefresh()
/* ******************************************************************** */
{
    return bitRead(REG_control, _LCDML_REG_MENU_CONTROL_type_dynParam_enable_custom_refresh);    
}   

