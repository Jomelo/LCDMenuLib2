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

#ifndef _LCDML_macros_h
    #define _LCDML_macros_h

    /* ------------------
     * Include Arduino IOS
     * ------------------
     */
    #include <Arduino.h>

    /* ------------------
     * compatible to old versions
     * ------------------
     */ 
    // new on v2.2.5 (other button handling)

    #define BT_enter()        REG_set(_LCDML_REG_CTRL__button, _LCDML_REG_button_enter)
    #define BT_up()           REG_set(_LCDML_REG_CTRL__button, _LCDML_REG_button_up)
    #define BT_down()         REG_set(_LCDML_REG_CTRL__button, _LCDML_REG_button_down)
    #define BT_left()         REG_set(_LCDML_REG_CTRL__button, _LCDML_REG_button_left)
    #define BT_right()        REG_set(_LCDML_REG_CTRL__button, _LCDML_REG_button_right)
    #define BT_quit()         REG_set(_LCDML_REG_CTRL__button, _LCDML_REG_button_quit)

    #define BT_resetEnter()   REG_reset(_LCDML_REG_CTRL__button, _LCDML_REG_button_enter)
    #define BT_resetUp()      REG_reset(_LCDML_REG_CTRL__button,  _LCDML_REG_button_up)
    #define BT_resetDown()    REG_reset(_LCDML_REG_CTRL__button, _LCDML_REG_button_down)
    #define BT_resetLeft()    REG_reset(_LCDML_REG_CTRL__button, _LCDML_REG_button_left)
    #define BT_resetRight()   REG_reset(_LCDML_REG_CTRL__button, _LCDML_REG_button_right)
    #define BT_resetQuit()    REG_reset(_LCDML_REG_CTRL__button, _LCDML_REG_button_quit)
    #define BT_resetAll()     REG_resetAll(_LCDML_REG_CTRL__button)

    #define BT_checkEnter()   REG_check(_LCDML_REG_CTRL__button, _LCDML_REG_button_enter)
    #define BT_checkUp()      REG_check(_LCDML_REG_CTRL__button, _LCDML_REG_button_up)
    #define BT_checkDown()    REG_check(_LCDML_REG_CTRL__button, _LCDML_REG_button_down)
    #define BT_checkLeft()    REG_check(_LCDML_REG_CTRL__button, _LCDML_REG_button_left)
    #define BT_checkRight()   REG_check(_LCDML_REG_CTRL__button, _LCDML_REG_button_right)
    #define BT_checkQuit()    REG_check(_LCDML_REG_CTRL__button, _LCDML_REG_button_quit)
    #define BT_checkAny()     REG_checkAny(_LCDML_REG_CTRL__button)

    #define CE_set(val)       REG_set(_LCDML_REG_CTRL__events, val)
    #define CE_reset(val)     REG_reset(_LCDML_REG_CTRL__events, val)
    #define CE_resetAll()     REG_resetAll(_LCDML_REG_CTRL__events)
    #define CE_check(val)     REG_check(_LCDML_REG_CTRL__events, val)
    #define CE_checkAny()     REG_checkAny(_LCDML_REG_CTRL__events)

    /* ------------------
     * DISP / MENU
     * ------------------
     */

    // get global language variable for name xyz
    #define LCDML_DISP_lang(name, lang)           g_LCDML_DISP_lang_ ## lang ## _ ## name ## _var

    // call repeat of function
    #define LCDML_DISP_initFunction(N)      LCDML_DISP_func_repeat(N);

    #ifndef _LCDML_cfg_use_ram
        // stored in flash (Arduino)
        #define LCDML_langDef(name, lang, content) \
            const char g_LCDML_DISP_lang_ ## lang ## _ ## name ##_var[] PROGMEM = {content}

         #define LCDML_getCustomContent(lang, var, id) \
            if(id < _LCDML_NO_FUNC) {\
                strcpy_P(var, (char*)pgm_read_word(&(g_LCDML_DISP_lang_ ## lang ## _table[id]))); \
            }     

        #define LCDML_createCustomLang(N, lang) \
            const char * const g_LCDML_DISP_lang_ ## lang ## _table[] PROGMEM = { LCDML_DISP_lang_repeat(N, lang) }

        #define LCDML_getCustomElementName(lang, var, element_id) \
            if(element_id < _LCDML_NO_FUNC && (sizeof(g_LCDML_DISP_lang_ ## lang ## _table)-1) >= element_id) {\
                strcpy_P(var, (char*)pgm_read_word(&(g_LCDML_DISP_lang_ ## lang ## _table[element_id])));\
            }

    #else
        // stored in ram (esp, stm, other controllers)
        #define LCDML_langDef(name, lang, content) \
            char g_LCDML_DISP_lang_ ## lang ## _ ## name ##_var[] = {content}       

        #define LCDML_getCustomContent(lang, var, id) \
            if(id < _LCDML_NO_FUNC) {\
                strcpy(var, g_LCDML_DISP_lang_ ## lang ## _table[id]); \
            }

         #define LCDML_createCustomLang(N, lang) \
            char * g_LCDML_DISP_lang_ ## lang ## _table[] = { LCDML_DISP_lang_repeat(N, lang) }

        #define LCDML_getCustomElementName(lang, var, element_id) \
            if(element_id < _LCDML_NO_FUNC && (sizeof(g_LCDML_DISP_lang_ ## lang ## _table)-1) >= element_id) {\
                strcpy(var, g_LCDML_DISP_lang_ ## lang ## _table[element_id]);\
            }

    #endif

    #define LCDML_getElementName(var, element_id) \
        LCDML_getCustomElementName(lcdml, var, element_id)

    #define LCDML_createMenu(N)\
        LCDML_createCustomLang(N, lcdml)        

    #define LCDML_getContent(var, id) \
        LCDML_getCustomContent(lcdml, var, id)

    //Menu Item Types
    #ifndef _LCDML_cfg_not_use_arduino_ide
    #define LCDML_addAdvanced(id, parent, child, p_condition, p_content, p_callback, p_param, p_settings) \
        LCDML_langDef(id, lcdml, p_content); \
        LCDMenuLib2_menu parent ## _ ## child(id, p_param, p_settings, p_callback, p_condition ); \
        void LCDML_DISP_ ## id ## _function() { \
            parent.addChild(parent ## _ ## child); \
        }
    #else
    #define LCDML_addAdvanced(id, parent, child, p_condition, p_content, p_callback, p_param, p_settings) \
        LCDML_langDef(id, lcdml, p_content); \
        static LCDMenuLib2_menu parent ## _ ## child(id, p_param, p_settings, p_callback, p_condition ); \
        static void LCDML_DISP_ ## id ## _function() { \
            parent.addChild(parent ## _ ## child); \
        }
    #endif

    #define LCDML_add(id, parent, child, content, callback) \
        LCDML_addAdvanced(id, parent, child, NULL, content, callback, 0, _LCDML_TYPE_default)

    #define LCDML_setup(N)\
        LCDML_DISP_initFunction(N); \
        LCDML.init(N)
        


    /* ---------------------------------------
     * lang repeat
     * ---------------------------------------
     */
    #define LCDML_DISP_lang_repeat_255(lang) LCDML_DISP_lang_repeat_254(lang) LCDML_DISP_lang(255, lang),
    #define LCDML_DISP_lang_repeat_254(lang) LCDML_DISP_lang_repeat_253(lang) LCDML_DISP_lang(254, lang),
    #define LCDML_DISP_lang_repeat_253(lang) LCDML_DISP_lang_repeat_252(lang) LCDML_DISP_lang(253, lang),
    #define LCDML_DISP_lang_repeat_252(lang) LCDML_DISP_lang_repeat_251(lang) LCDML_DISP_lang(252, lang),
    #define LCDML_DISP_lang_repeat_251(lang) LCDML_DISP_lang_repeat_250(lang) LCDML_DISP_lang(251, lang),
    #define LCDML_DISP_lang_repeat_250(lang) LCDML_DISP_lang_repeat_249(lang) LCDML_DISP_lang(250, lang),
    #define LCDML_DISP_lang_repeat_249(lang) LCDML_DISP_lang_repeat_248(lang) LCDML_DISP_lang(249, lang),
    #define LCDML_DISP_lang_repeat_248(lang) LCDML_DISP_lang_repeat_247(lang) LCDML_DISP_lang(248, lang),
    #define LCDML_DISP_lang_repeat_247(lang) LCDML_DISP_lang_repeat_246(lang) LCDML_DISP_lang(247, lang),
    #define LCDML_DISP_lang_repeat_246(lang) LCDML_DISP_lang_repeat_245(lang) LCDML_DISP_lang(246, lang),
    #define LCDML_DISP_lang_repeat_245(lang) LCDML_DISP_lang_repeat_244(lang) LCDML_DISP_lang(245, lang),
    #define LCDML_DISP_lang_repeat_244(lang) LCDML_DISP_lang_repeat_243(lang) LCDML_DISP_lang(244, lang),
    #define LCDML_DISP_lang_repeat_243(lang) LCDML_DISP_lang_repeat_242(lang) LCDML_DISP_lang(243, lang),
    #define LCDML_DISP_lang_repeat_242(lang) LCDML_DISP_lang_repeat_241(lang) LCDML_DISP_lang(242, lang),
    #define LCDML_DISP_lang_repeat_241(lang) LCDML_DISP_lang_repeat_240(lang) LCDML_DISP_lang(241, lang),
    #define LCDML_DISP_lang_repeat_240(lang) LCDML_DISP_lang_repeat_239(lang) LCDML_DISP_lang(240, lang),
    #define LCDML_DISP_lang_repeat_239(lang) LCDML_DISP_lang_repeat_238(lang) LCDML_DISP_lang(239, lang),
    #define LCDML_DISP_lang_repeat_238(lang) LCDML_DISP_lang_repeat_237(lang) LCDML_DISP_lang(238, lang),
    #define LCDML_DISP_lang_repeat_237(lang) LCDML_DISP_lang_repeat_236(lang) LCDML_DISP_lang(237, lang),
    #define LCDML_DISP_lang_repeat_236(lang) LCDML_DISP_lang_repeat_235(lang) LCDML_DISP_lang(236, lang),
    #define LCDML_DISP_lang_repeat_235(lang) LCDML_DISP_lang_repeat_234(lang) LCDML_DISP_lang(235, lang),
    #define LCDML_DISP_lang_repeat_234(lang) LCDML_DISP_lang_repeat_233(lang) LCDML_DISP_lang(234, lang),
    #define LCDML_DISP_lang_repeat_233(lang) LCDML_DISP_lang_repeat_232(lang) LCDML_DISP_lang(233, lang),
    #define LCDML_DISP_lang_repeat_232(lang) LCDML_DISP_lang_repeat_231(lang) LCDML_DISP_lang(232, lang),
    #define LCDML_DISP_lang_repeat_231(lang) LCDML_DISP_lang_repeat_230(lang) LCDML_DISP_lang(231, lang),
    #define LCDML_DISP_lang_repeat_230(lang) LCDML_DISP_lang_repeat_229(lang) LCDML_DISP_lang(230, lang),
    #define LCDML_DISP_lang_repeat_229(lang) LCDML_DISP_lang_repeat_228(lang) LCDML_DISP_lang(229, lang),
    #define LCDML_DISP_lang_repeat_228(lang) LCDML_DISP_lang_repeat_227(lang) LCDML_DISP_lang(228, lang),
    #define LCDML_DISP_lang_repeat_227(lang) LCDML_DISP_lang_repeat_226(lang) LCDML_DISP_lang(227, lang),
    #define LCDML_DISP_lang_repeat_226(lang) LCDML_DISP_lang_repeat_225(lang) LCDML_DISP_lang(226, lang),
    #define LCDML_DISP_lang_repeat_225(lang) LCDML_DISP_lang_repeat_224(lang) LCDML_DISP_lang(225, lang),
    #define LCDML_DISP_lang_repeat_224(lang) LCDML_DISP_lang_repeat_223(lang) LCDML_DISP_lang(224, lang),
    #define LCDML_DISP_lang_repeat_223(lang) LCDML_DISP_lang_repeat_222(lang) LCDML_DISP_lang(223, lang),
    #define LCDML_DISP_lang_repeat_222(lang) LCDML_DISP_lang_repeat_221(lang) LCDML_DISP_lang(222, lang),
    #define LCDML_DISP_lang_repeat_221(lang) LCDML_DISP_lang_repeat_220(lang) LCDML_DISP_lang(221, lang),
    #define LCDML_DISP_lang_repeat_220(lang) LCDML_DISP_lang_repeat_219(lang) LCDML_DISP_lang(220, lang),
    #define LCDML_DISP_lang_repeat_219(lang) LCDML_DISP_lang_repeat_218(lang) LCDML_DISP_lang(219, lang),
    #define LCDML_DISP_lang_repeat_218(lang) LCDML_DISP_lang_repeat_217(lang) LCDML_DISP_lang(218, lang),
    #define LCDML_DISP_lang_repeat_217(lang) LCDML_DISP_lang_repeat_216(lang) LCDML_DISP_lang(217, lang),
    #define LCDML_DISP_lang_repeat_216(lang) LCDML_DISP_lang_repeat_215(lang) LCDML_DISP_lang(216, lang),
    #define LCDML_DISP_lang_repeat_215(lang) LCDML_DISP_lang_repeat_214(lang) LCDML_DISP_lang(215, lang),
    #define LCDML_DISP_lang_repeat_214(lang) LCDML_DISP_lang_repeat_213(lang) LCDML_DISP_lang(214, lang),
    #define LCDML_DISP_lang_repeat_213(lang) LCDML_DISP_lang_repeat_212(lang) LCDML_DISP_lang(213, lang),
    #define LCDML_DISP_lang_repeat_212(lang) LCDML_DISP_lang_repeat_211(lang) LCDML_DISP_lang(212, lang),
    #define LCDML_DISP_lang_repeat_211(lang) LCDML_DISP_lang_repeat_210(lang) LCDML_DISP_lang(211, lang),
    #define LCDML_DISP_lang_repeat_210(lang) LCDML_DISP_lang_repeat_209(lang) LCDML_DISP_lang(210, lang),
    #define LCDML_DISP_lang_repeat_209(lang) LCDML_DISP_lang_repeat_208(lang) LCDML_DISP_lang(209, lang),
    #define LCDML_DISP_lang_repeat_208(lang) LCDML_DISP_lang_repeat_207(lang) LCDML_DISP_lang(208, lang),
    #define LCDML_DISP_lang_repeat_207(lang) LCDML_DISP_lang_repeat_206(lang) LCDML_DISP_lang(207, lang),
    #define LCDML_DISP_lang_repeat_206(lang) LCDML_DISP_lang_repeat_205(lang) LCDML_DISP_lang(206, lang),
    #define LCDML_DISP_lang_repeat_205(lang) LCDML_DISP_lang_repeat_204(lang) LCDML_DISP_lang(205, lang),
    #define LCDML_DISP_lang_repeat_204(lang) LCDML_DISP_lang_repeat_203(lang) LCDML_DISP_lang(204, lang),
    #define LCDML_DISP_lang_repeat_203(lang) LCDML_DISP_lang_repeat_202(lang) LCDML_DISP_lang(203, lang),
    #define LCDML_DISP_lang_repeat_202(lang) LCDML_DISP_lang_repeat_201(lang) LCDML_DISP_lang(202, lang),
    #define LCDML_DISP_lang_repeat_201(lang) LCDML_DISP_lang_repeat_200(lang) LCDML_DISP_lang(201, lang),
    #define LCDML_DISP_lang_repeat_200(lang) LCDML_DISP_lang_repeat_199(lang) LCDML_DISP_lang(200, lang),
    #define LCDML_DISP_lang_repeat_199(lang) LCDML_DISP_lang_repeat_198(lang) LCDML_DISP_lang(199, lang),
    #define LCDML_DISP_lang_repeat_198(lang) LCDML_DISP_lang_repeat_197(lang) LCDML_DISP_lang(198, lang),
    #define LCDML_DISP_lang_repeat_197(lang) LCDML_DISP_lang_repeat_196(lang) LCDML_DISP_lang(197, lang),
    #define LCDML_DISP_lang_repeat_196(lang) LCDML_DISP_lang_repeat_195(lang) LCDML_DISP_lang(196, lang),
    #define LCDML_DISP_lang_repeat_195(lang) LCDML_DISP_lang_repeat_194(lang) LCDML_DISP_lang(195, lang),
    #define LCDML_DISP_lang_repeat_194(lang) LCDML_DISP_lang_repeat_193(lang) LCDML_DISP_lang(194, lang),
    #define LCDML_DISP_lang_repeat_193(lang) LCDML_DISP_lang_repeat_192(lang) LCDML_DISP_lang(193, lang),
    #define LCDML_DISP_lang_repeat_192(lang) LCDML_DISP_lang_repeat_191(lang) LCDML_DISP_lang(192, lang),
    #define LCDML_DISP_lang_repeat_191(lang) LCDML_DISP_lang_repeat_190(lang) LCDML_DISP_lang(191, lang),
    #define LCDML_DISP_lang_repeat_190(lang) LCDML_DISP_lang_repeat_189(lang) LCDML_DISP_lang(190, lang),
    #define LCDML_DISP_lang_repeat_189(lang) LCDML_DISP_lang_repeat_188(lang) LCDML_DISP_lang(189, lang),
    #define LCDML_DISP_lang_repeat_188(lang) LCDML_DISP_lang_repeat_187(lang) LCDML_DISP_lang(188, lang),
    #define LCDML_DISP_lang_repeat_187(lang) LCDML_DISP_lang_repeat_186(lang) LCDML_DISP_lang(187, lang),
    #define LCDML_DISP_lang_repeat_186(lang) LCDML_DISP_lang_repeat_185(lang) LCDML_DISP_lang(186, lang),
    #define LCDML_DISP_lang_repeat_185(lang) LCDML_DISP_lang_repeat_184(lang) LCDML_DISP_lang(185, lang),
    #define LCDML_DISP_lang_repeat_184(lang) LCDML_DISP_lang_repeat_183(lang) LCDML_DISP_lang(184, lang),
    #define LCDML_DISP_lang_repeat_183(lang) LCDML_DISP_lang_repeat_182(lang) LCDML_DISP_lang(183, lang),
    #define LCDML_DISP_lang_repeat_182(lang) LCDML_DISP_lang_repeat_181(lang) LCDML_DISP_lang(182, lang),
    #define LCDML_DISP_lang_repeat_181(lang) LCDML_DISP_lang_repeat_180(lang) LCDML_DISP_lang(181, lang),
    #define LCDML_DISP_lang_repeat_180(lang) LCDML_DISP_lang_repeat_179(lang) LCDML_DISP_lang(180, lang),
    #define LCDML_DISP_lang_repeat_179(lang) LCDML_DISP_lang_repeat_178(lang) LCDML_DISP_lang(179, lang),
    #define LCDML_DISP_lang_repeat_178(lang) LCDML_DISP_lang_repeat_177(lang) LCDML_DISP_lang(178, lang),
    #define LCDML_DISP_lang_repeat_177(lang) LCDML_DISP_lang_repeat_176(lang) LCDML_DISP_lang(177, lang),
    #define LCDML_DISP_lang_repeat_176(lang) LCDML_DISP_lang_repeat_175(lang) LCDML_DISP_lang(176, lang),
    #define LCDML_DISP_lang_repeat_175(lang) LCDML_DISP_lang_repeat_174(lang) LCDML_DISP_lang(175, lang),
    #define LCDML_DISP_lang_repeat_174(lang) LCDML_DISP_lang_repeat_173(lang) LCDML_DISP_lang(174, lang),
    #define LCDML_DISP_lang_repeat_173(lang) LCDML_DISP_lang_repeat_172(lang) LCDML_DISP_lang(173, lang),
    #define LCDML_DISP_lang_repeat_172(lang) LCDML_DISP_lang_repeat_171(lang) LCDML_DISP_lang(172, lang),
    #define LCDML_DISP_lang_repeat_171(lang) LCDML_DISP_lang_repeat_170(lang) LCDML_DISP_lang(171, lang),
    #define LCDML_DISP_lang_repeat_170(lang) LCDML_DISP_lang_repeat_169(lang) LCDML_DISP_lang(170, lang),
    #define LCDML_DISP_lang_repeat_169(lang) LCDML_DISP_lang_repeat_168(lang) LCDML_DISP_lang(169, lang),
    #define LCDML_DISP_lang_repeat_168(lang) LCDML_DISP_lang_repeat_167(lang) LCDML_DISP_lang(168, lang),
    #define LCDML_DISP_lang_repeat_167(lang) LCDML_DISP_lang_repeat_166(lang) LCDML_DISP_lang(167, lang),
    #define LCDML_DISP_lang_repeat_166(lang) LCDML_DISP_lang_repeat_165(lang) LCDML_DISP_lang(166, lang),
    #define LCDML_DISP_lang_repeat_165(lang) LCDML_DISP_lang_repeat_164(lang) LCDML_DISP_lang(165, lang),
    #define LCDML_DISP_lang_repeat_164(lang) LCDML_DISP_lang_repeat_163(lang) LCDML_DISP_lang(164, lang),
    #define LCDML_DISP_lang_repeat_163(lang) LCDML_DISP_lang_repeat_162(lang) LCDML_DISP_lang(163, lang),
    #define LCDML_DISP_lang_repeat_162(lang) LCDML_DISP_lang_repeat_161(lang) LCDML_DISP_lang(162, lang),
    #define LCDML_DISP_lang_repeat_161(lang) LCDML_DISP_lang_repeat_160(lang) LCDML_DISP_lang(161, lang),
    #define LCDML_DISP_lang_repeat_160(lang) LCDML_DISP_lang_repeat_159(lang) LCDML_DISP_lang(160, lang),
    #define LCDML_DISP_lang_repeat_159(lang) LCDML_DISP_lang_repeat_158(lang) LCDML_DISP_lang(159, lang),
    #define LCDML_DISP_lang_repeat_158(lang) LCDML_DISP_lang_repeat_157(lang) LCDML_DISP_lang(158, lang),
    #define LCDML_DISP_lang_repeat_157(lang) LCDML_DISP_lang_repeat_156(lang) LCDML_DISP_lang(157, lang),
    #define LCDML_DISP_lang_repeat_156(lang) LCDML_DISP_lang_repeat_155(lang) LCDML_DISP_lang(156, lang),
    #define LCDML_DISP_lang_repeat_155(lang) LCDML_DISP_lang_repeat_154(lang) LCDML_DISP_lang(155, lang),
    #define LCDML_DISP_lang_repeat_154(lang) LCDML_DISP_lang_repeat_153(lang) LCDML_DISP_lang(154, lang),
    #define LCDML_DISP_lang_repeat_153(lang) LCDML_DISP_lang_repeat_152(lang) LCDML_DISP_lang(153, lang),
    #define LCDML_DISP_lang_repeat_152(lang) LCDML_DISP_lang_repeat_151(lang) LCDML_DISP_lang(152, lang),
    #define LCDML_DISP_lang_repeat_151(lang) LCDML_DISP_lang_repeat_150(lang) LCDML_DISP_lang(151, lang),
    #define LCDML_DISP_lang_repeat_150(lang) LCDML_DISP_lang_repeat_149(lang) LCDML_DISP_lang(150, lang),
    #define LCDML_DISP_lang_repeat_149(lang) LCDML_DISP_lang_repeat_148(lang) LCDML_DISP_lang(149, lang),
    #define LCDML_DISP_lang_repeat_148(lang) LCDML_DISP_lang_repeat_147(lang) LCDML_DISP_lang(148, lang),
    #define LCDML_DISP_lang_repeat_147(lang) LCDML_DISP_lang_repeat_146(lang) LCDML_DISP_lang(147, lang),
    #define LCDML_DISP_lang_repeat_146(lang) LCDML_DISP_lang_repeat_145(lang) LCDML_DISP_lang(146, lang),
    #define LCDML_DISP_lang_repeat_145(lang) LCDML_DISP_lang_repeat_144(lang) LCDML_DISP_lang(145, lang),
    #define LCDML_DISP_lang_repeat_144(lang) LCDML_DISP_lang_repeat_143(lang) LCDML_DISP_lang(144, lang),
    #define LCDML_DISP_lang_repeat_143(lang) LCDML_DISP_lang_repeat_142(lang) LCDML_DISP_lang(143, lang),
    #define LCDML_DISP_lang_repeat_142(lang) LCDML_DISP_lang_repeat_141(lang) LCDML_DISP_lang(142, lang),
    #define LCDML_DISP_lang_repeat_141(lang) LCDML_DISP_lang_repeat_140(lang) LCDML_DISP_lang(141, lang),
    #define LCDML_DISP_lang_repeat_140(lang) LCDML_DISP_lang_repeat_139(lang) LCDML_DISP_lang(140, lang),
    #define LCDML_DISP_lang_repeat_139(lang) LCDML_DISP_lang_repeat_138(lang) LCDML_DISP_lang(139, lang),
    #define LCDML_DISP_lang_repeat_138(lang) LCDML_DISP_lang_repeat_137(lang) LCDML_DISP_lang(138, lang),
    #define LCDML_DISP_lang_repeat_137(lang) LCDML_DISP_lang_repeat_136(lang) LCDML_DISP_lang(137, lang),
    #define LCDML_DISP_lang_repeat_136(lang) LCDML_DISP_lang_repeat_135(lang) LCDML_DISP_lang(136, lang),
    #define LCDML_DISP_lang_repeat_135(lang) LCDML_DISP_lang_repeat_134(lang) LCDML_DISP_lang(135, lang),
    #define LCDML_DISP_lang_repeat_134(lang) LCDML_DISP_lang_repeat_133(lang) LCDML_DISP_lang(134, lang),
    #define LCDML_DISP_lang_repeat_133(lang) LCDML_DISP_lang_repeat_132(lang) LCDML_DISP_lang(133, lang),
    #define LCDML_DISP_lang_repeat_132(lang) LCDML_DISP_lang_repeat_131(lang) LCDML_DISP_lang(132, lang),
    #define LCDML_DISP_lang_repeat_131(lang) LCDML_DISP_lang_repeat_130(lang) LCDML_DISP_lang(131, lang),
    #define LCDML_DISP_lang_repeat_130(lang) LCDML_DISP_lang_repeat_129(lang) LCDML_DISP_lang(130, lang),
    #define LCDML_DISP_lang_repeat_129(lang) LCDML_DISP_lang_repeat_128(lang) LCDML_DISP_lang(129, lang),
    #define LCDML_DISP_lang_repeat_128(lang) LCDML_DISP_lang_repeat_127(lang) LCDML_DISP_lang(128, lang),
    #define LCDML_DISP_lang_repeat_127(lang) LCDML_DISP_lang_repeat_126(lang) LCDML_DISP_lang(127, lang),
    #define LCDML_DISP_lang_repeat_126(lang) LCDML_DISP_lang_repeat_125(lang) LCDML_DISP_lang(126, lang),
    #define LCDML_DISP_lang_repeat_125(lang) LCDML_DISP_lang_repeat_124(lang) LCDML_DISP_lang(125, lang),
    #define LCDML_DISP_lang_repeat_124(lang) LCDML_DISP_lang_repeat_123(lang) LCDML_DISP_lang(124, lang),
    #define LCDML_DISP_lang_repeat_123(lang) LCDML_DISP_lang_repeat_122(lang) LCDML_DISP_lang(123, lang),
    #define LCDML_DISP_lang_repeat_122(lang) LCDML_DISP_lang_repeat_121(lang) LCDML_DISP_lang(122, lang),
    #define LCDML_DISP_lang_repeat_121(lang) LCDML_DISP_lang_repeat_120(lang) LCDML_DISP_lang(121, lang),
    #define LCDML_DISP_lang_repeat_120(lang) LCDML_DISP_lang_repeat_119(lang) LCDML_DISP_lang(120, lang),
    #define LCDML_DISP_lang_repeat_119(lang) LCDML_DISP_lang_repeat_118(lang) LCDML_DISP_lang(119, lang),
    #define LCDML_DISP_lang_repeat_118(lang) LCDML_DISP_lang_repeat_117(lang) LCDML_DISP_lang(118, lang),
    #define LCDML_DISP_lang_repeat_117(lang) LCDML_DISP_lang_repeat_116(lang) LCDML_DISP_lang(117, lang),
    #define LCDML_DISP_lang_repeat_116(lang) LCDML_DISP_lang_repeat_115(lang) LCDML_DISP_lang(116, lang),
    #define LCDML_DISP_lang_repeat_115(lang) LCDML_DISP_lang_repeat_114(lang) LCDML_DISP_lang(115, lang),
    #define LCDML_DISP_lang_repeat_114(lang) LCDML_DISP_lang_repeat_113(lang) LCDML_DISP_lang(114, lang),
    #define LCDML_DISP_lang_repeat_113(lang) LCDML_DISP_lang_repeat_112(lang) LCDML_DISP_lang(113, lang),
    #define LCDML_DISP_lang_repeat_112(lang) LCDML_DISP_lang_repeat_111(lang) LCDML_DISP_lang(112, lang),
    #define LCDML_DISP_lang_repeat_111(lang) LCDML_DISP_lang_repeat_110(lang) LCDML_DISP_lang(111, lang),
    #define LCDML_DISP_lang_repeat_110(lang) LCDML_DISP_lang_repeat_109(lang) LCDML_DISP_lang(110, lang),
    #define LCDML_DISP_lang_repeat_109(lang) LCDML_DISP_lang_repeat_108(lang) LCDML_DISP_lang(109, lang),
    #define LCDML_DISP_lang_repeat_108(lang) LCDML_DISP_lang_repeat_107(lang) LCDML_DISP_lang(108, lang),
    #define LCDML_DISP_lang_repeat_107(lang) LCDML_DISP_lang_repeat_106(lang) LCDML_DISP_lang(107, lang),
    #define LCDML_DISP_lang_repeat_106(lang) LCDML_DISP_lang_repeat_105(lang) LCDML_DISP_lang(106, lang),
    #define LCDML_DISP_lang_repeat_105(lang) LCDML_DISP_lang_repeat_104(lang) LCDML_DISP_lang(105, lang),
    #define LCDML_DISP_lang_repeat_104(lang) LCDML_DISP_lang_repeat_103(lang) LCDML_DISP_lang(104, lang),
    #define LCDML_DISP_lang_repeat_103(lang) LCDML_DISP_lang_repeat_102(lang) LCDML_DISP_lang(103, lang),
    #define LCDML_DISP_lang_repeat_102(lang) LCDML_DISP_lang_repeat_101(lang) LCDML_DISP_lang(102, lang),
    #define LCDML_DISP_lang_repeat_101(lang) LCDML_DISP_lang_repeat_100(lang) LCDML_DISP_lang(101, lang),
    #define LCDML_DISP_lang_repeat_100(lang) LCDML_DISP_lang_repeat_99(lang)  LCDML_DISP_lang(100, lang),
    #define LCDML_DISP_lang_repeat_99(lang)  LCDML_DISP_lang_repeat_98(lang)  LCDML_DISP_lang(99, lang),
    #define LCDML_DISP_lang_repeat_98(lang)  LCDML_DISP_lang_repeat_97(lang)  LCDML_DISP_lang(98, lang),
    #define LCDML_DISP_lang_repeat_97(lang)  LCDML_DISP_lang_repeat_96(lang)  LCDML_DISP_lang(97, lang),
    #define LCDML_DISP_lang_repeat_96(lang)  LCDML_DISP_lang_repeat_95(lang)  LCDML_DISP_lang(96, lang),
    #define LCDML_DISP_lang_repeat_95(lang)  LCDML_DISP_lang_repeat_94(lang)  LCDML_DISP_lang(95, lang),
    #define LCDML_DISP_lang_repeat_94(lang)  LCDML_DISP_lang_repeat_93(lang)  LCDML_DISP_lang(94, lang),
    #define LCDML_DISP_lang_repeat_93(lang)  LCDML_DISP_lang_repeat_92(lang)  LCDML_DISP_lang(93, lang),
    #define LCDML_DISP_lang_repeat_92(lang)  LCDML_DISP_lang_repeat_91(lang)  LCDML_DISP_lang(92, lang),
    #define LCDML_DISP_lang_repeat_91(lang)  LCDML_DISP_lang_repeat_90(lang)  LCDML_DISP_lang(91, lang),
    #define LCDML_DISP_lang_repeat_90(lang)  LCDML_DISP_lang_repeat_89(lang)  LCDML_DISP_lang(90, lang),
    #define LCDML_DISP_lang_repeat_89(lang)  LCDML_DISP_lang_repeat_88(lang)  LCDML_DISP_lang(89, lang),
    #define LCDML_DISP_lang_repeat_88(lang)  LCDML_DISP_lang_repeat_87(lang)  LCDML_DISP_lang(88, lang),
    #define LCDML_DISP_lang_repeat_87(lang)  LCDML_DISP_lang_repeat_86(lang)  LCDML_DISP_lang(87, lang),
    #define LCDML_DISP_lang_repeat_86(lang)  LCDML_DISP_lang_repeat_85(lang)  LCDML_DISP_lang(86, lang),
    #define LCDML_DISP_lang_repeat_85(lang)  LCDML_DISP_lang_repeat_84(lang)  LCDML_DISP_lang(85, lang),
    #define LCDML_DISP_lang_repeat_84(lang)  LCDML_DISP_lang_repeat_83(lang)  LCDML_DISP_lang(84, lang),
    #define LCDML_DISP_lang_repeat_83(lang)  LCDML_DISP_lang_repeat_82(lang)  LCDML_DISP_lang(83, lang),
    #define LCDML_DISP_lang_repeat_82(lang)  LCDML_DISP_lang_repeat_81(lang)  LCDML_DISP_lang(82, lang),
    #define LCDML_DISP_lang_repeat_81(lang)  LCDML_DISP_lang_repeat_80(lang)  LCDML_DISP_lang(81, lang),
    #define LCDML_DISP_lang_repeat_80(lang)  LCDML_DISP_lang_repeat_79(lang)  LCDML_DISP_lang(80, lang),
    #define LCDML_DISP_lang_repeat_79(lang)  LCDML_DISP_lang_repeat_78(lang)  LCDML_DISP_lang(79, lang),
    #define LCDML_DISP_lang_repeat_78(lang)  LCDML_DISP_lang_repeat_77(lang)  LCDML_DISP_lang(78, lang),
    #define LCDML_DISP_lang_repeat_77(lang)  LCDML_DISP_lang_repeat_76(lang)  LCDML_DISP_lang(77, lang),
    #define LCDML_DISP_lang_repeat_76(lang)  LCDML_DISP_lang_repeat_75(lang)  LCDML_DISP_lang(76, lang),
    #define LCDML_DISP_lang_repeat_75(lang)  LCDML_DISP_lang_repeat_74(lang)  LCDML_DISP_lang(75, lang),
    #define LCDML_DISP_lang_repeat_74(lang)  LCDML_DISP_lang_repeat_73(lang)  LCDML_DISP_lang(74, lang),
    #define LCDML_DISP_lang_repeat_73(lang)  LCDML_DISP_lang_repeat_72(lang)  LCDML_DISP_lang(73, lang),
    #define LCDML_DISP_lang_repeat_72(lang)  LCDML_DISP_lang_repeat_71(lang)  LCDML_DISP_lang(72, lang),
    #define LCDML_DISP_lang_repeat_71(lang)  LCDML_DISP_lang_repeat_70(lang)  LCDML_DISP_lang(71, lang),
    #define LCDML_DISP_lang_repeat_70(lang)  LCDML_DISP_lang_repeat_69(lang)  LCDML_DISP_lang(70, lang),
    #define LCDML_DISP_lang_repeat_69(lang)  LCDML_DISP_lang_repeat_68(lang)  LCDML_DISP_lang(69, lang),
    #define LCDML_DISP_lang_repeat_68(lang)  LCDML_DISP_lang_repeat_67(lang)  LCDML_DISP_lang(68, lang),
    #define LCDML_DISP_lang_repeat_67(lang)  LCDML_DISP_lang_repeat_66(lang)  LCDML_DISP_lang(67, lang),
    #define LCDML_DISP_lang_repeat_66(lang)  LCDML_DISP_lang_repeat_65(lang)  LCDML_DISP_lang(66, lang),
    #define LCDML_DISP_lang_repeat_65(lang)  LCDML_DISP_lang_repeat_64(lang)  LCDML_DISP_lang(65, lang),
    #define LCDML_DISP_lang_repeat_64(lang)  LCDML_DISP_lang_repeat_63(lang)  LCDML_DISP_lang(64, lang),
    #define LCDML_DISP_lang_repeat_63(lang)  LCDML_DISP_lang_repeat_62(lang)  LCDML_DISP_lang(63, lang),
    #define LCDML_DISP_lang_repeat_62(lang)  LCDML_DISP_lang_repeat_61(lang)  LCDML_DISP_lang(62, lang),
    #define LCDML_DISP_lang_repeat_61(lang)  LCDML_DISP_lang_repeat_60(lang)  LCDML_DISP_lang(61, lang),
    #define LCDML_DISP_lang_repeat_60(lang)  LCDML_DISP_lang_repeat_59(lang)  LCDML_DISP_lang(60, lang),
    #define LCDML_DISP_lang_repeat_59(lang)  LCDML_DISP_lang_repeat_58(lang)  LCDML_DISP_lang(59, lang),
    #define LCDML_DISP_lang_repeat_58(lang)  LCDML_DISP_lang_repeat_57(lang)  LCDML_DISP_lang(58, lang),
    #define LCDML_DISP_lang_repeat_57(lang)  LCDML_DISP_lang_repeat_56(lang)  LCDML_DISP_lang(57, lang),
    #define LCDML_DISP_lang_repeat_56(lang)  LCDML_DISP_lang_repeat_55(lang)  LCDML_DISP_lang(56, lang),
    #define LCDML_DISP_lang_repeat_55(lang)  LCDML_DISP_lang_repeat_54(lang)  LCDML_DISP_lang(55, lang),
    #define LCDML_DISP_lang_repeat_54(lang)  LCDML_DISP_lang_repeat_53(lang)  LCDML_DISP_lang(54, lang),
    #define LCDML_DISP_lang_repeat_53(lang)  LCDML_DISP_lang_repeat_52(lang)  LCDML_DISP_lang(53, lang),
    #define LCDML_DISP_lang_repeat_52(lang)  LCDML_DISP_lang_repeat_51(lang)  LCDML_DISP_lang(52, lang),
    #define LCDML_DISP_lang_repeat_51(lang)  LCDML_DISP_lang_repeat_50(lang)  LCDML_DISP_lang(51, lang),
    #define LCDML_DISP_lang_repeat_50(lang)  LCDML_DISP_lang_repeat_49(lang)  LCDML_DISP_lang(50, lang),
    #define LCDML_DISP_lang_repeat_49(lang)  LCDML_DISP_lang_repeat_48(lang)  LCDML_DISP_lang(49, lang),
    #define LCDML_DISP_lang_repeat_48(lang)  LCDML_DISP_lang_repeat_47(lang)  LCDML_DISP_lang(48, lang),
    #define LCDML_DISP_lang_repeat_47(lang)  LCDML_DISP_lang_repeat_46(lang)  LCDML_DISP_lang(47, lang),
    #define LCDML_DISP_lang_repeat_46(lang)  LCDML_DISP_lang_repeat_45(lang)  LCDML_DISP_lang(46, lang),
    #define LCDML_DISP_lang_repeat_45(lang)  LCDML_DISP_lang_repeat_44(lang)  LCDML_DISP_lang(45, lang),
    #define LCDML_DISP_lang_repeat_44(lang)  LCDML_DISP_lang_repeat_43(lang)  LCDML_DISP_lang(44, lang),
    #define LCDML_DISP_lang_repeat_43(lang)  LCDML_DISP_lang_repeat_42(lang)  LCDML_DISP_lang(43, lang),
    #define LCDML_DISP_lang_repeat_42(lang)  LCDML_DISP_lang_repeat_41(lang)  LCDML_DISP_lang(42, lang),
    #define LCDML_DISP_lang_repeat_41(lang)  LCDML_DISP_lang_repeat_40(lang)  LCDML_DISP_lang(41, lang),
    #define LCDML_DISP_lang_repeat_40(lang)  LCDML_DISP_lang_repeat_39(lang)  LCDML_DISP_lang(40, lang),
    #define LCDML_DISP_lang_repeat_39(lang)  LCDML_DISP_lang_repeat_38(lang)  LCDML_DISP_lang(39, lang),
    #define LCDML_DISP_lang_repeat_38(lang)  LCDML_DISP_lang_repeat_37(lang)  LCDML_DISP_lang(38, lang),
    #define LCDML_DISP_lang_repeat_37(lang)  LCDML_DISP_lang_repeat_36(lang)  LCDML_DISP_lang(37, lang),
    #define LCDML_DISP_lang_repeat_36(lang)  LCDML_DISP_lang_repeat_35(lang)  LCDML_DISP_lang(36, lang),
    #define LCDML_DISP_lang_repeat_35(lang)  LCDML_DISP_lang_repeat_34(lang)  LCDML_DISP_lang(35, lang),
    #define LCDML_DISP_lang_repeat_34(lang)  LCDML_DISP_lang_repeat_33(lang)  LCDML_DISP_lang(34, lang),
    #define LCDML_DISP_lang_repeat_33(lang)  LCDML_DISP_lang_repeat_32(lang)  LCDML_DISP_lang(33, lang),
    #define LCDML_DISP_lang_repeat_32(lang)  LCDML_DISP_lang_repeat_31(lang)  LCDML_DISP_lang(32, lang),
    #define LCDML_DISP_lang_repeat_31(lang)  LCDML_DISP_lang_repeat_30(lang)  LCDML_DISP_lang(31, lang),
    #define LCDML_DISP_lang_repeat_30(lang)  LCDML_DISP_lang_repeat_29(lang)  LCDML_DISP_lang(30, lang),
    #define LCDML_DISP_lang_repeat_29(lang)  LCDML_DISP_lang_repeat_28(lang)  LCDML_DISP_lang(29, lang),
    #define LCDML_DISP_lang_repeat_28(lang)  LCDML_DISP_lang_repeat_27(lang)  LCDML_DISP_lang(28, lang),
    #define LCDML_DISP_lang_repeat_27(lang)  LCDML_DISP_lang_repeat_26(lang)  LCDML_DISP_lang(27, lang),
    #define LCDML_DISP_lang_repeat_26(lang)  LCDML_DISP_lang_repeat_25(lang)  LCDML_DISP_lang(26, lang),
    #define LCDML_DISP_lang_repeat_25(lang)  LCDML_DISP_lang_repeat_24(lang)  LCDML_DISP_lang(25, lang),
    #define LCDML_DISP_lang_repeat_24(lang)  LCDML_DISP_lang_repeat_23(lang)  LCDML_DISP_lang(24, lang),
    #define LCDML_DISP_lang_repeat_23(lang)  LCDML_DISP_lang_repeat_22(lang)  LCDML_DISP_lang(23, lang),
    #define LCDML_DISP_lang_repeat_22(lang)  LCDML_DISP_lang_repeat_21(lang)  LCDML_DISP_lang(22, lang),
    #define LCDML_DISP_lang_repeat_21(lang)  LCDML_DISP_lang_repeat_20(lang)  LCDML_DISP_lang(21, lang),
    #define LCDML_DISP_lang_repeat_20(lang)  LCDML_DISP_lang_repeat_19(lang)  LCDML_DISP_lang(20, lang),
    #define LCDML_DISP_lang_repeat_19(lang)  LCDML_DISP_lang_repeat_18(lang)  LCDML_DISP_lang(19, lang),
    #define LCDML_DISP_lang_repeat_18(lang)  LCDML_DISP_lang_repeat_17(lang)  LCDML_DISP_lang(18, lang),
    #define LCDML_DISP_lang_repeat_17(lang)  LCDML_DISP_lang_repeat_16(lang)  LCDML_DISP_lang(17, lang),
    #define LCDML_DISP_lang_repeat_16(lang)  LCDML_DISP_lang_repeat_15(lang)  LCDML_DISP_lang(16, lang),
    #define LCDML_DISP_lang_repeat_15(lang)  LCDML_DISP_lang_repeat_14(lang)  LCDML_DISP_lang(15, lang),
    #define LCDML_DISP_lang_repeat_14(lang)  LCDML_DISP_lang_repeat_13(lang)  LCDML_DISP_lang(14, lang),
    #define LCDML_DISP_lang_repeat_13(lang)  LCDML_DISP_lang_repeat_12(lang)  LCDML_DISP_lang(13, lang),
    #define LCDML_DISP_lang_repeat_12(lang)  LCDML_DISP_lang_repeat_11(lang)  LCDML_DISP_lang(12, lang),
    #define LCDML_DISP_lang_repeat_11(lang)  LCDML_DISP_lang_repeat_10(lang)  LCDML_DISP_lang(11, lang),
    #define LCDML_DISP_lang_repeat_10(lang)  LCDML_DISP_lang_repeat_9(lang)   LCDML_DISP_lang(10, lang),
    #define LCDML_DISP_lang_repeat_9(lang)   LCDML_DISP_lang_repeat_8(lang)   LCDML_DISP_lang(9, lang),
    #define LCDML_DISP_lang_repeat_8(lang)   LCDML_DISP_lang_repeat_7(lang)   LCDML_DISP_lang(8, lang),
    #define LCDML_DISP_lang_repeat_7(lang)   LCDML_DISP_lang_repeat_6(lang)   LCDML_DISP_lang(7, lang),
    #define LCDML_DISP_lang_repeat_6(lang)   LCDML_DISP_lang_repeat_5(lang)   LCDML_DISP_lang(6, lang),
    #define LCDML_DISP_lang_repeat_5(lang)   LCDML_DISP_lang_repeat_4(lang)   LCDML_DISP_lang(5, lang),
    #define LCDML_DISP_lang_repeat_4(lang)   LCDML_DISP_lang_repeat_3(lang)   LCDML_DISP_lang(4, lang),
    #define LCDML_DISP_lang_repeat_3(lang)   LCDML_DISP_lang_repeat_2(lang)   LCDML_DISP_lang(3, lang),
    #define LCDML_DISP_lang_repeat_2(lang)   LCDML_DISP_lang_repeat_1(lang)   LCDML_DISP_lang(2, lang),
    #define LCDML_DISP_lang_repeat_1(lang)   LCDML_DISP_lang_repeat_0(lang)   LCDML_DISP_lang(1, lang),
    #define LCDML_DISP_lang_repeat_0(lang)   LCDML_DISP_lang(0, lang),
    #define LCDML_DISP_lang_repeat(N, lang)     LCDML_DISP_lang_repeat_##N (lang)







    /* ---------------------------------------
     * func repeat
     * ---------------------------------------
     */
    #define LCDML_DISP_func_repeat_255() LCDML_DISP_func_repeat_254() LCDML_DISP_255_function();
    #define LCDML_DISP_func_repeat_254() LCDML_DISP_func_repeat_253() LCDML_DISP_254_function();
    #define LCDML_DISP_func_repeat_253() LCDML_DISP_func_repeat_252() LCDML_DISP_253_function();
    #define LCDML_DISP_func_repeat_252() LCDML_DISP_func_repeat_251() LCDML_DISP_252_function();
    #define LCDML_DISP_func_repeat_251() LCDML_DISP_func_repeat_250() LCDML_DISP_251_function();
    #define LCDML_DISP_func_repeat_250() LCDML_DISP_func_repeat_249() LCDML_DISP_250_function();
    #define LCDML_DISP_func_repeat_249() LCDML_DISP_func_repeat_248() LCDML_DISP_249_function();
    #define LCDML_DISP_func_repeat_248() LCDML_DISP_func_repeat_247() LCDML_DISP_248_function();
    #define LCDML_DISP_func_repeat_247() LCDML_DISP_func_repeat_246() LCDML_DISP_247_function();
    #define LCDML_DISP_func_repeat_246() LCDML_DISP_func_repeat_245() LCDML_DISP_246_function();
    #define LCDML_DISP_func_repeat_245() LCDML_DISP_func_repeat_244() LCDML_DISP_245_function();
    #define LCDML_DISP_func_repeat_244() LCDML_DISP_func_repeat_243() LCDML_DISP_244_function();
    #define LCDML_DISP_func_repeat_243() LCDML_DISP_func_repeat_242() LCDML_DISP_243_function();
    #define LCDML_DISP_func_repeat_242() LCDML_DISP_func_repeat_241() LCDML_DISP_242_function();
    #define LCDML_DISP_func_repeat_241() LCDML_DISP_func_repeat_240() LCDML_DISP_241_function();
    #define LCDML_DISP_func_repeat_240() LCDML_DISP_func_repeat_239() LCDML_DISP_240_function();
    #define LCDML_DISP_func_repeat_239() LCDML_DISP_func_repeat_238() LCDML_DISP_239_function();
    #define LCDML_DISP_func_repeat_238() LCDML_DISP_func_repeat_237() LCDML_DISP_238_function();
    #define LCDML_DISP_func_repeat_237() LCDML_DISP_func_repeat_236() LCDML_DISP_237_function();
    #define LCDML_DISP_func_repeat_236() LCDML_DISP_func_repeat_235() LCDML_DISP_236_function();
    #define LCDML_DISP_func_repeat_235() LCDML_DISP_func_repeat_234() LCDML_DISP_235_function();
    #define LCDML_DISP_func_repeat_234() LCDML_DISP_func_repeat_233() LCDML_DISP_234_function();
    #define LCDML_DISP_func_repeat_233() LCDML_DISP_func_repeat_232() LCDML_DISP_233_function();
    #define LCDML_DISP_func_repeat_232() LCDML_DISP_func_repeat_231() LCDML_DISP_232_function();
    #define LCDML_DISP_func_repeat_231() LCDML_DISP_func_repeat_230() LCDML_DISP_231_function();
    #define LCDML_DISP_func_repeat_230() LCDML_DISP_func_repeat_229() LCDML_DISP_230_function();
    #define LCDML_DISP_func_repeat_229() LCDML_DISP_func_repeat_228() LCDML_DISP_229_function();
    #define LCDML_DISP_func_repeat_228() LCDML_DISP_func_repeat_227() LCDML_DISP_228_function();
    #define LCDML_DISP_func_repeat_227() LCDML_DISP_func_repeat_226() LCDML_DISP_227_function();
    #define LCDML_DISP_func_repeat_226() LCDML_DISP_func_repeat_225() LCDML_DISP_226_function();
    #define LCDML_DISP_func_repeat_225() LCDML_DISP_func_repeat_224() LCDML_DISP_225_function();
    #define LCDML_DISP_func_repeat_224() LCDML_DISP_func_repeat_223() LCDML_DISP_224_function();
    #define LCDML_DISP_func_repeat_223() LCDML_DISP_func_repeat_222() LCDML_DISP_223_function();
    #define LCDML_DISP_func_repeat_222() LCDML_DISP_func_repeat_221() LCDML_DISP_222_function();
    #define LCDML_DISP_func_repeat_221() LCDML_DISP_func_repeat_220() LCDML_DISP_221_function();
    #define LCDML_DISP_func_repeat_220() LCDML_DISP_func_repeat_219() LCDML_DISP_220_function();
    #define LCDML_DISP_func_repeat_219() LCDML_DISP_func_repeat_218() LCDML_DISP_219_function();
    #define LCDML_DISP_func_repeat_218() LCDML_DISP_func_repeat_217() LCDML_DISP_218_function();
    #define LCDML_DISP_func_repeat_217() LCDML_DISP_func_repeat_216() LCDML_DISP_217_function();
    #define LCDML_DISP_func_repeat_216() LCDML_DISP_func_repeat_215() LCDML_DISP_216_function();
    #define LCDML_DISP_func_repeat_215() LCDML_DISP_func_repeat_214() LCDML_DISP_215_function();
    #define LCDML_DISP_func_repeat_214() LCDML_DISP_func_repeat_213() LCDML_DISP_214_function();
    #define LCDML_DISP_func_repeat_213() LCDML_DISP_func_repeat_212() LCDML_DISP_213_function();
    #define LCDML_DISP_func_repeat_212() LCDML_DISP_func_repeat_211() LCDML_DISP_212_function();
    #define LCDML_DISP_func_repeat_211() LCDML_DISP_func_repeat_210() LCDML_DISP_211_function();
    #define LCDML_DISP_func_repeat_210() LCDML_DISP_func_repeat_209() LCDML_DISP_210_function();
    #define LCDML_DISP_func_repeat_209() LCDML_DISP_func_repeat_208() LCDML_DISP_209_function();
    #define LCDML_DISP_func_repeat_208() LCDML_DISP_func_repeat_207() LCDML_DISP_208_function();
    #define LCDML_DISP_func_repeat_207() LCDML_DISP_func_repeat_206() LCDML_DISP_207_function();
    #define LCDML_DISP_func_repeat_206() LCDML_DISP_func_repeat_205() LCDML_DISP_206_function();
    #define LCDML_DISP_func_repeat_205() LCDML_DISP_func_repeat_204() LCDML_DISP_205_function();
    #define LCDML_DISP_func_repeat_204() LCDML_DISP_func_repeat_203() LCDML_DISP_204_function();
    #define LCDML_DISP_func_repeat_203() LCDML_DISP_func_repeat_202() LCDML_DISP_203_function();
    #define LCDML_DISP_func_repeat_202() LCDML_DISP_func_repeat_201() LCDML_DISP_202_function();
    #define LCDML_DISP_func_repeat_201() LCDML_DISP_func_repeat_200() LCDML_DISP_201_function();
    #define LCDML_DISP_func_repeat_200() LCDML_DISP_func_repeat_199() LCDML_DISP_200_function();
    #define LCDML_DISP_func_repeat_199() LCDML_DISP_func_repeat_198() LCDML_DISP_199_function();
    #define LCDML_DISP_func_repeat_198() LCDML_DISP_func_repeat_197() LCDML_DISP_198_function();
    #define LCDML_DISP_func_repeat_197() LCDML_DISP_func_repeat_196() LCDML_DISP_197_function();
    #define LCDML_DISP_func_repeat_196() LCDML_DISP_func_repeat_195() LCDML_DISP_196_function();
    #define LCDML_DISP_func_repeat_195() LCDML_DISP_func_repeat_194() LCDML_DISP_195_function();
    #define LCDML_DISP_func_repeat_194() LCDML_DISP_func_repeat_193() LCDML_DISP_194_function();
    #define LCDML_DISP_func_repeat_193() LCDML_DISP_func_repeat_192() LCDML_DISP_193_function();
    #define LCDML_DISP_func_repeat_192() LCDML_DISP_func_repeat_191() LCDML_DISP_192_function();
    #define LCDML_DISP_func_repeat_191() LCDML_DISP_func_repeat_190() LCDML_DISP_191_function();
    #define LCDML_DISP_func_repeat_190() LCDML_DISP_func_repeat_189() LCDML_DISP_190_function();
    #define LCDML_DISP_func_repeat_189() LCDML_DISP_func_repeat_188() LCDML_DISP_189_function();
    #define LCDML_DISP_func_repeat_188() LCDML_DISP_func_repeat_187() LCDML_DISP_188_function();
    #define LCDML_DISP_func_repeat_187() LCDML_DISP_func_repeat_186() LCDML_DISP_187_function();
    #define LCDML_DISP_func_repeat_186() LCDML_DISP_func_repeat_185() LCDML_DISP_186_function();
    #define LCDML_DISP_func_repeat_185() LCDML_DISP_func_repeat_184() LCDML_DISP_185_function();
    #define LCDML_DISP_func_repeat_184() LCDML_DISP_func_repeat_183() LCDML_DISP_184_function();
    #define LCDML_DISP_func_repeat_183() LCDML_DISP_func_repeat_182() LCDML_DISP_183_function();
    #define LCDML_DISP_func_repeat_182() LCDML_DISP_func_repeat_181() LCDML_DISP_182_function();
    #define LCDML_DISP_func_repeat_181() LCDML_DISP_func_repeat_180() LCDML_DISP_181_function();
    #define LCDML_DISP_func_repeat_180() LCDML_DISP_func_repeat_179() LCDML_DISP_180_function();
    #define LCDML_DISP_func_repeat_179() LCDML_DISP_func_repeat_178() LCDML_DISP_179_function();
    #define LCDML_DISP_func_repeat_178() LCDML_DISP_func_repeat_177() LCDML_DISP_178_function();
    #define LCDML_DISP_func_repeat_177() LCDML_DISP_func_repeat_176() LCDML_DISP_177_function();
    #define LCDML_DISP_func_repeat_176() LCDML_DISP_func_repeat_175() LCDML_DISP_176_function();
    #define LCDML_DISP_func_repeat_175() LCDML_DISP_func_repeat_174() LCDML_DISP_175_function();
    #define LCDML_DISP_func_repeat_174() LCDML_DISP_func_repeat_173() LCDML_DISP_174_function();
    #define LCDML_DISP_func_repeat_173() LCDML_DISP_func_repeat_172() LCDML_DISP_173_function();
    #define LCDML_DISP_func_repeat_172() LCDML_DISP_func_repeat_171() LCDML_DISP_172_function();
    #define LCDML_DISP_func_repeat_171() LCDML_DISP_func_repeat_170() LCDML_DISP_171_function();
    #define LCDML_DISP_func_repeat_170() LCDML_DISP_func_repeat_169() LCDML_DISP_170_function();
    #define LCDML_DISP_func_repeat_169() LCDML_DISP_func_repeat_168() LCDML_DISP_169_function();
    #define LCDML_DISP_func_repeat_168() LCDML_DISP_func_repeat_167() LCDML_DISP_168_function();
    #define LCDML_DISP_func_repeat_167() LCDML_DISP_func_repeat_166() LCDML_DISP_167_function();
    #define LCDML_DISP_func_repeat_166() LCDML_DISP_func_repeat_165() LCDML_DISP_166_function();
    #define LCDML_DISP_func_repeat_165() LCDML_DISP_func_repeat_164() LCDML_DISP_165_function();
    #define LCDML_DISP_func_repeat_164() LCDML_DISP_func_repeat_163() LCDML_DISP_164_function();
    #define LCDML_DISP_func_repeat_163() LCDML_DISP_func_repeat_162() LCDML_DISP_163_function();
    #define LCDML_DISP_func_repeat_162() LCDML_DISP_func_repeat_161() LCDML_DISP_162_function();
    #define LCDML_DISP_func_repeat_161() LCDML_DISP_func_repeat_160() LCDML_DISP_161_function();
    #define LCDML_DISP_func_repeat_160() LCDML_DISP_func_repeat_159() LCDML_DISP_160_function();
    #define LCDML_DISP_func_repeat_159() LCDML_DISP_func_repeat_158() LCDML_DISP_159_function();
    #define LCDML_DISP_func_repeat_158() LCDML_DISP_func_repeat_157() LCDML_DISP_158_function();
    #define LCDML_DISP_func_repeat_157() LCDML_DISP_func_repeat_156() LCDML_DISP_157_function();
    #define LCDML_DISP_func_repeat_156() LCDML_DISP_func_repeat_155() LCDML_DISP_156_function();
    #define LCDML_DISP_func_repeat_155() LCDML_DISP_func_repeat_154() LCDML_DISP_155_function();
    #define LCDML_DISP_func_repeat_154() LCDML_DISP_func_repeat_153() LCDML_DISP_154_function();
    #define LCDML_DISP_func_repeat_153() LCDML_DISP_func_repeat_152() LCDML_DISP_153_function();
    #define LCDML_DISP_func_repeat_152() LCDML_DISP_func_repeat_151() LCDML_DISP_152_function();
    #define LCDML_DISP_func_repeat_151() LCDML_DISP_func_repeat_150() LCDML_DISP_151_function();
    #define LCDML_DISP_func_repeat_150() LCDML_DISP_func_repeat_149() LCDML_DISP_150_function();
    #define LCDML_DISP_func_repeat_149() LCDML_DISP_func_repeat_148() LCDML_DISP_149_function();
    #define LCDML_DISP_func_repeat_148() LCDML_DISP_func_repeat_147() LCDML_DISP_148_function();
    #define LCDML_DISP_func_repeat_147() LCDML_DISP_func_repeat_146() LCDML_DISP_147_function();
    #define LCDML_DISP_func_repeat_146() LCDML_DISP_func_repeat_145() LCDML_DISP_146_function();
    #define LCDML_DISP_func_repeat_145() LCDML_DISP_func_repeat_144() LCDML_DISP_145_function();
    #define LCDML_DISP_func_repeat_144() LCDML_DISP_func_repeat_143() LCDML_DISP_144_function();
    #define LCDML_DISP_func_repeat_143() LCDML_DISP_func_repeat_142() LCDML_DISP_143_function();
    #define LCDML_DISP_func_repeat_142() LCDML_DISP_func_repeat_141() LCDML_DISP_142_function();
    #define LCDML_DISP_func_repeat_141() LCDML_DISP_func_repeat_140() LCDML_DISP_141_function();
    #define LCDML_DISP_func_repeat_140() LCDML_DISP_func_repeat_139() LCDML_DISP_140_function();
    #define LCDML_DISP_func_repeat_139() LCDML_DISP_func_repeat_138() LCDML_DISP_139_function();
    #define LCDML_DISP_func_repeat_138() LCDML_DISP_func_repeat_137() LCDML_DISP_138_function();
    #define LCDML_DISP_func_repeat_137() LCDML_DISP_func_repeat_136() LCDML_DISP_137_function();
    #define LCDML_DISP_func_repeat_136() LCDML_DISP_func_repeat_135() LCDML_DISP_136_function();
    #define LCDML_DISP_func_repeat_135() LCDML_DISP_func_repeat_134() LCDML_DISP_135_function();
    #define LCDML_DISP_func_repeat_134() LCDML_DISP_func_repeat_133() LCDML_DISP_134_function();
    #define LCDML_DISP_func_repeat_133() LCDML_DISP_func_repeat_132() LCDML_DISP_133_function();
    #define LCDML_DISP_func_repeat_132() LCDML_DISP_func_repeat_131() LCDML_DISP_132_function();
    #define LCDML_DISP_func_repeat_131() LCDML_DISP_func_repeat_130() LCDML_DISP_131_function();
    #define LCDML_DISP_func_repeat_130() LCDML_DISP_func_repeat_129() LCDML_DISP_130_function();
    #define LCDML_DISP_func_repeat_129() LCDML_DISP_func_repeat_128() LCDML_DISP_129_function();
    #define LCDML_DISP_func_repeat_128() LCDML_DISP_func_repeat_127() LCDML_DISP_128_function();
    #define LCDML_DISP_func_repeat_127() LCDML_DISP_func_repeat_126() LCDML_DISP_127_function();
    #define LCDML_DISP_func_repeat_126() LCDML_DISP_func_repeat_125() LCDML_DISP_126_function();
    #define LCDML_DISP_func_repeat_125() LCDML_DISP_func_repeat_124() LCDML_DISP_125_function();
    #define LCDML_DISP_func_repeat_124() LCDML_DISP_func_repeat_123() LCDML_DISP_124_function();
    #define LCDML_DISP_func_repeat_123() LCDML_DISP_func_repeat_122() LCDML_DISP_123_function();
    #define LCDML_DISP_func_repeat_122() LCDML_DISP_func_repeat_121() LCDML_DISP_122_function();
    #define LCDML_DISP_func_repeat_121() LCDML_DISP_func_repeat_120() LCDML_DISP_121_function();
    #define LCDML_DISP_func_repeat_120() LCDML_DISP_func_repeat_119() LCDML_DISP_120_function();
    #define LCDML_DISP_func_repeat_119() LCDML_DISP_func_repeat_118() LCDML_DISP_119_function();
    #define LCDML_DISP_func_repeat_118() LCDML_DISP_func_repeat_117() LCDML_DISP_118_function();
    #define LCDML_DISP_func_repeat_117() LCDML_DISP_func_repeat_116() LCDML_DISP_117_function();
    #define LCDML_DISP_func_repeat_116() LCDML_DISP_func_repeat_115() LCDML_DISP_116_function();
    #define LCDML_DISP_func_repeat_115() LCDML_DISP_func_repeat_114() LCDML_DISP_115_function();
    #define LCDML_DISP_func_repeat_114() LCDML_DISP_func_repeat_113() LCDML_DISP_114_function();
    #define LCDML_DISP_func_repeat_113() LCDML_DISP_func_repeat_112() LCDML_DISP_113_function();
    #define LCDML_DISP_func_repeat_112() LCDML_DISP_func_repeat_111() LCDML_DISP_112_function();
    #define LCDML_DISP_func_repeat_111() LCDML_DISP_func_repeat_110() LCDML_DISP_111_function();
    #define LCDML_DISP_func_repeat_110() LCDML_DISP_func_repeat_109() LCDML_DISP_110_function();
    #define LCDML_DISP_func_repeat_109() LCDML_DISP_func_repeat_108() LCDML_DISP_109_function();
    #define LCDML_DISP_func_repeat_108() LCDML_DISP_func_repeat_107() LCDML_DISP_108_function();
    #define LCDML_DISP_func_repeat_107() LCDML_DISP_func_repeat_106() LCDML_DISP_107_function();
    #define LCDML_DISP_func_repeat_106() LCDML_DISP_func_repeat_105() LCDML_DISP_106_function();
    #define LCDML_DISP_func_repeat_105() LCDML_DISP_func_repeat_104() LCDML_DISP_105_function();
    #define LCDML_DISP_func_repeat_104() LCDML_DISP_func_repeat_103() LCDML_DISP_104_function();
    #define LCDML_DISP_func_repeat_103() LCDML_DISP_func_repeat_102() LCDML_DISP_103_function();
    #define LCDML_DISP_func_repeat_102() LCDML_DISP_func_repeat_101() LCDML_DISP_102_function();
    #define LCDML_DISP_func_repeat_101() LCDML_DISP_func_repeat_100() LCDML_DISP_101_function();
    #define LCDML_DISP_func_repeat_100() LCDML_DISP_func_repeat_99()  LCDML_DISP_100_function();
    #define LCDML_DISP_func_repeat_99() LCDML_DISP_func_repeat_98() LCDML_DISP_99_function();
    #define LCDML_DISP_func_repeat_98() LCDML_DISP_func_repeat_97() LCDML_DISP_98_function();
    #define LCDML_DISP_func_repeat_97() LCDML_DISP_func_repeat_96() LCDML_DISP_97_function();
    #define LCDML_DISP_func_repeat_96() LCDML_DISP_func_repeat_95() LCDML_DISP_96_function();
    #define LCDML_DISP_func_repeat_95() LCDML_DISP_func_repeat_94() LCDML_DISP_95_function();
    #define LCDML_DISP_func_repeat_94() LCDML_DISP_func_repeat_93() LCDML_DISP_94_function();
    #define LCDML_DISP_func_repeat_93() LCDML_DISP_func_repeat_92() LCDML_DISP_93_function();
    #define LCDML_DISP_func_repeat_92() LCDML_DISP_func_repeat_91() LCDML_DISP_92_function();
    #define LCDML_DISP_func_repeat_91() LCDML_DISP_func_repeat_90() LCDML_DISP_91_function();
    #define LCDML_DISP_func_repeat_90() LCDML_DISP_func_repeat_89() LCDML_DISP_90_function();
    #define LCDML_DISP_func_repeat_89() LCDML_DISP_func_repeat_88() LCDML_DISP_89_function();
    #define LCDML_DISP_func_repeat_88() LCDML_DISP_func_repeat_87() LCDML_DISP_88_function();
    #define LCDML_DISP_func_repeat_87() LCDML_DISP_func_repeat_86() LCDML_DISP_87_function();
    #define LCDML_DISP_func_repeat_86() LCDML_DISP_func_repeat_85() LCDML_DISP_86_function();
    #define LCDML_DISP_func_repeat_85() LCDML_DISP_func_repeat_84() LCDML_DISP_85_function();
    #define LCDML_DISP_func_repeat_84() LCDML_DISP_func_repeat_83() LCDML_DISP_84_function();
    #define LCDML_DISP_func_repeat_83() LCDML_DISP_func_repeat_82() LCDML_DISP_83_function();
    #define LCDML_DISP_func_repeat_82() LCDML_DISP_func_repeat_81() LCDML_DISP_82_function();
    #define LCDML_DISP_func_repeat_81() LCDML_DISP_func_repeat_80() LCDML_DISP_81_function();
    #define LCDML_DISP_func_repeat_80() LCDML_DISP_func_repeat_79() LCDML_DISP_80_function();
    #define LCDML_DISP_func_repeat_79() LCDML_DISP_func_repeat_78() LCDML_DISP_79_function();
    #define LCDML_DISP_func_repeat_78() LCDML_DISP_func_repeat_77() LCDML_DISP_78_function();
    #define LCDML_DISP_func_repeat_77() LCDML_DISP_func_repeat_76() LCDML_DISP_77_function();
    #define LCDML_DISP_func_repeat_76() LCDML_DISP_func_repeat_75() LCDML_DISP_76_function();
    #define LCDML_DISP_func_repeat_75() LCDML_DISP_func_repeat_74() LCDML_DISP_75_function();
    #define LCDML_DISP_func_repeat_74() LCDML_DISP_func_repeat_73() LCDML_DISP_74_function();
    #define LCDML_DISP_func_repeat_73() LCDML_DISP_func_repeat_72() LCDML_DISP_73_function();
    #define LCDML_DISP_func_repeat_72() LCDML_DISP_func_repeat_71() LCDML_DISP_72_function();
    #define LCDML_DISP_func_repeat_71() LCDML_DISP_func_repeat_70() LCDML_DISP_71_function();
    #define LCDML_DISP_func_repeat_70() LCDML_DISP_func_repeat_69() LCDML_DISP_70_function();
    #define LCDML_DISP_func_repeat_69() LCDML_DISP_func_repeat_68() LCDML_DISP_69_function();
    #define LCDML_DISP_func_repeat_68() LCDML_DISP_func_repeat_67() LCDML_DISP_68_function();
    #define LCDML_DISP_func_repeat_67() LCDML_DISP_func_repeat_66() LCDML_DISP_67_function();
    #define LCDML_DISP_func_repeat_66() LCDML_DISP_func_repeat_65() LCDML_DISP_66_function();
    #define LCDML_DISP_func_repeat_65() LCDML_DISP_func_repeat_64() LCDML_DISP_65_function();
    #define LCDML_DISP_func_repeat_64() LCDML_DISP_func_repeat_63() LCDML_DISP_64_function();
    #define LCDML_DISP_func_repeat_63() LCDML_DISP_func_repeat_62() LCDML_DISP_63_function();
    #define LCDML_DISP_func_repeat_62() LCDML_DISP_func_repeat_61() LCDML_DISP_62_function();
    #define LCDML_DISP_func_repeat_61() LCDML_DISP_func_repeat_60() LCDML_DISP_61_function();
    #define LCDML_DISP_func_repeat_60() LCDML_DISP_func_repeat_59() LCDML_DISP_60_function();
    #define LCDML_DISP_func_repeat_59() LCDML_DISP_func_repeat_58() LCDML_DISP_59_function();
    #define LCDML_DISP_func_repeat_58() LCDML_DISP_func_repeat_57() LCDML_DISP_58_function();
    #define LCDML_DISP_func_repeat_57() LCDML_DISP_func_repeat_56() LCDML_DISP_57_function();
    #define LCDML_DISP_func_repeat_56() LCDML_DISP_func_repeat_55() LCDML_DISP_56_function();
    #define LCDML_DISP_func_repeat_55() LCDML_DISP_func_repeat_54() LCDML_DISP_55_function();
    #define LCDML_DISP_func_repeat_54() LCDML_DISP_func_repeat_53() LCDML_DISP_54_function();
    #define LCDML_DISP_func_repeat_53() LCDML_DISP_func_repeat_52() LCDML_DISP_53_function();
    #define LCDML_DISP_func_repeat_52() LCDML_DISP_func_repeat_51() LCDML_DISP_52_function();
    #define LCDML_DISP_func_repeat_51() LCDML_DISP_func_repeat_50() LCDML_DISP_51_function();
    #define LCDML_DISP_func_repeat_50() LCDML_DISP_func_repeat_49() LCDML_DISP_50_function();
    #define LCDML_DISP_func_repeat_49() LCDML_DISP_func_repeat_48() LCDML_DISP_49_function();
    #define LCDML_DISP_func_repeat_48() LCDML_DISP_func_repeat_47() LCDML_DISP_48_function();
    #define LCDML_DISP_func_repeat_47() LCDML_DISP_func_repeat_46() LCDML_DISP_47_function();
    #define LCDML_DISP_func_repeat_46() LCDML_DISP_func_repeat_45() LCDML_DISP_46_function();
    #define LCDML_DISP_func_repeat_45() LCDML_DISP_func_repeat_44() LCDML_DISP_45_function();
    #define LCDML_DISP_func_repeat_44() LCDML_DISP_func_repeat_43() LCDML_DISP_44_function();
    #define LCDML_DISP_func_repeat_43() LCDML_DISP_func_repeat_42() LCDML_DISP_43_function();
    #define LCDML_DISP_func_repeat_42() LCDML_DISP_func_repeat_41() LCDML_DISP_42_function();
    #define LCDML_DISP_func_repeat_41() LCDML_DISP_func_repeat_40() LCDML_DISP_41_function();
    #define LCDML_DISP_func_repeat_40() LCDML_DISP_func_repeat_39() LCDML_DISP_40_function();
    #define LCDML_DISP_func_repeat_39() LCDML_DISP_func_repeat_38() LCDML_DISP_39_function();
    #define LCDML_DISP_func_repeat_38() LCDML_DISP_func_repeat_37() LCDML_DISP_38_function();
    #define LCDML_DISP_func_repeat_37() LCDML_DISP_func_repeat_36() LCDML_DISP_37_function();
    #define LCDML_DISP_func_repeat_36() LCDML_DISP_func_repeat_35() LCDML_DISP_36_function();
    #define LCDML_DISP_func_repeat_35() LCDML_DISP_func_repeat_34() LCDML_DISP_35_function();
    #define LCDML_DISP_func_repeat_34() LCDML_DISP_func_repeat_33() LCDML_DISP_34_function();
    #define LCDML_DISP_func_repeat_33() LCDML_DISP_func_repeat_32() LCDML_DISP_33_function();
    #define LCDML_DISP_func_repeat_32() LCDML_DISP_func_repeat_31() LCDML_DISP_32_function();
    #define LCDML_DISP_func_repeat_31() LCDML_DISP_func_repeat_30() LCDML_DISP_31_function();
    #define LCDML_DISP_func_repeat_30() LCDML_DISP_func_repeat_29() LCDML_DISP_30_function();
    #define LCDML_DISP_func_repeat_29() LCDML_DISP_func_repeat_28() LCDML_DISP_29_function();
    #define LCDML_DISP_func_repeat_28() LCDML_DISP_func_repeat_27() LCDML_DISP_28_function();
    #define LCDML_DISP_func_repeat_27() LCDML_DISP_func_repeat_26() LCDML_DISP_27_function();
    #define LCDML_DISP_func_repeat_26() LCDML_DISP_func_repeat_25() LCDML_DISP_26_function();
    #define LCDML_DISP_func_repeat_25() LCDML_DISP_func_repeat_24() LCDML_DISP_25_function();
    #define LCDML_DISP_func_repeat_24() LCDML_DISP_func_repeat_23() LCDML_DISP_24_function();
    #define LCDML_DISP_func_repeat_23() LCDML_DISP_func_repeat_22() LCDML_DISP_23_function();
    #define LCDML_DISP_func_repeat_22() LCDML_DISP_func_repeat_21() LCDML_DISP_22_function();
    #define LCDML_DISP_func_repeat_21() LCDML_DISP_func_repeat_20() LCDML_DISP_21_function();
    #define LCDML_DISP_func_repeat_20() LCDML_DISP_func_repeat_19() LCDML_DISP_20_function();
    #define LCDML_DISP_func_repeat_19() LCDML_DISP_func_repeat_18() LCDML_DISP_19_function();
    #define LCDML_DISP_func_repeat_18() LCDML_DISP_func_repeat_17() LCDML_DISP_18_function();
    #define LCDML_DISP_func_repeat_17() LCDML_DISP_func_repeat_16() LCDML_DISP_17_function();
    #define LCDML_DISP_func_repeat_16() LCDML_DISP_func_repeat_15() LCDML_DISP_16_function();
    #define LCDML_DISP_func_repeat_15() LCDML_DISP_func_repeat_14() LCDML_DISP_15_function();
    #define LCDML_DISP_func_repeat_14() LCDML_DISP_func_repeat_13() LCDML_DISP_14_function();
    #define LCDML_DISP_func_repeat_13() LCDML_DISP_func_repeat_12() LCDML_DISP_13_function();
    #define LCDML_DISP_func_repeat_12() LCDML_DISP_func_repeat_11() LCDML_DISP_12_function();
    #define LCDML_DISP_func_repeat_11() LCDML_DISP_func_repeat_10() LCDML_DISP_11_function();
    #define LCDML_DISP_func_repeat_10() LCDML_DISP_func_repeat_9()  LCDML_DISP_10_function();
    #define LCDML_DISP_func_repeat_9() LCDML_DISP_func_repeat_8()   LCDML_DISP_9_function();
    #define LCDML_DISP_func_repeat_8() LCDML_DISP_func_repeat_7()   LCDML_DISP_8_function();
    #define LCDML_DISP_func_repeat_7() LCDML_DISP_func_repeat_6()   LCDML_DISP_7_function();
    #define LCDML_DISP_func_repeat_6() LCDML_DISP_func_repeat_5()   LCDML_DISP_6_function();
    #define LCDML_DISP_func_repeat_5() LCDML_DISP_func_repeat_4()   LCDML_DISP_5_function();
    #define LCDML_DISP_func_repeat_4() LCDML_DISP_func_repeat_3()   LCDML_DISP_4_function();
    #define LCDML_DISP_func_repeat_3() LCDML_DISP_func_repeat_2()   LCDML_DISP_3_function();
    #define LCDML_DISP_func_repeat_2() LCDML_DISP_func_repeat_1()   LCDML_DISP_2_function();
    #define LCDML_DISP_func_repeat_1() LCDML_DISP_func_repeat_0()   LCDML_DISP_1_function();
    #define LCDML_DISP_func_repeat_0() LCDML_DISP_0_function();
    #define LCDML_DISP_func_repeat(N) LCDML_DISP_func_repeat_ ## N ()

#endif
