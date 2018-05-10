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

#ifndef _LCDML_macros_h
    #define _LCDML_macros_h

    /* ------------------
     * Include Arduino IOS
     * ------------------
     */
    #include <Arduino.h>


    /* ------------------
     * DISP / MENU
     * ------------------
     */

    // get global language variable for name xyz
    #define LCDML_DISP_lang(name)           g_LCDML_DISP_lang_ ## name ## _var

    // call repeat of function
    #define LCDML_DISP_initFunction(N)      LCDML_DISP_func_repeat(N);

    #ifndef _LCDML_ESP
        // stored in flash (Arduino)
        #define LCDML_LANG_DEF(name, content) \
            const char g_LCDML_DISP_lang_ ## name ##_var[] PROGMEM = {content}

        #define LCDML_getContent(var, id) \
            if(id < _LCDML_NO_FUNC) {\
                strcpy_P(var, (char*)pgm_read_word(&(g_LCDML_DISP_lang_table[id]))); \
            }

        #define LCDML_createMenu(N)\
            const char * const g_LCDML_DISP_lang_table[] PROGMEM = { LCDML_DISP_lang_repeat(N) }

        #define LCDML_getElementName(var, element_id) \
            if(element_id < _LCDML_NO_FUNC && (sizeof(g_LCDML_DISP_lang_table)-1) >= element_id) {\
                strcpy_P(var, (char*)pgm_read_word(&(g_LCDML_DISP_lang_table[element_id])));\
            }

    #else
        // stored in ram (esp)
        #define LCDML_LANG_DEF(name, content) \
            char g_LCDML_DISP_lang_ ## name ##_var[] = {content}

        #define LCDML_getContent(var, id) \
            if(id < _LCDML_NO_FUNC) {\
                strcpy(var, g_LCDML_DISP_lang_table[id]); \
            }

        #define LCDML_createMenu(N)\
            char * g_LCDML_DISP_lang_table[] = { LCDML_DISP_lang_repeat(N) }


        #define LCDML_getElementName(var, element_id) \
            if(element_id < _LCDML_NO_FUNC && (sizeof(g_LCDML_DISP_lang_table)-1) >= element_id) {\
                strcpy_P(var, (char*)(g_LCDML_DISP_lang_table[element_id]));\
            }

    #endif

    //Menu Item Types
    #define LCDML_addAdvanced(id, parent, child, condition, content, callback, param, settings) \
        LCDML_LANG_DEF(id, content); \
        LCDMenuLib2_menu parent ## _ ## child(id, param, settings, callback, condition ); \
        void LCDML_DISP_ ## id ## _function() { \
            parent.addChild(parent ## _ ## child); \
        }

    #define LCDML_add(id, parent, child, content, callback) \
        LCDML_addAdvanced(id, parent, child, NULL, content, callback, 0, _LCDML_TYPE_default)

    #define LCDML_setup(N)\
        LCDML_DISP_initFunction(N); \
        LCDML.MENU_display(); \
        LCDML.DISP_menuUpdate()


    /* ---------------------------------------
     * lang repeat
     * ---------------------------------------
     */
    #define LCDML_DISP_lang_repeat_255() LCDML_DISP_lang_repeat_254() LCDML_DISP_lang(255),
    #define LCDML_DISP_lang_repeat_254() LCDML_DISP_lang_repeat_253() LCDML_DISP_lang(254),
    #define LCDML_DISP_lang_repeat_253() LCDML_DISP_lang_repeat_252() LCDML_DISP_lang(253),
    #define LCDML_DISP_lang_repeat_252() LCDML_DISP_lang_repeat_251() LCDML_DISP_lang(252),
    #define LCDML_DISP_lang_repeat_251() LCDML_DISP_lang_repeat_250() LCDML_DISP_lang(251),
    #define LCDML_DISP_lang_repeat_250() LCDML_DISP_lang_repeat_249() LCDML_DISP_lang(250),
    #define LCDML_DISP_lang_repeat_249() LCDML_DISP_lang_repeat_248() LCDML_DISP_lang(249),
    #define LCDML_DISP_lang_repeat_248() LCDML_DISP_lang_repeat_247() LCDML_DISP_lang(248),
    #define LCDML_DISP_lang_repeat_247() LCDML_DISP_lang_repeat_246() LCDML_DISP_lang(247),
    #define LCDML_DISP_lang_repeat_246() LCDML_DISP_lang_repeat_245() LCDML_DISP_lang(246),
    #define LCDML_DISP_lang_repeat_245() LCDML_DISP_lang_repeat_244() LCDML_DISP_lang(245),
    #define LCDML_DISP_lang_repeat_244() LCDML_DISP_lang_repeat_243() LCDML_DISP_lang(244),
    #define LCDML_DISP_lang_repeat_243() LCDML_DISP_lang_repeat_242() LCDML_DISP_lang(243),
    #define LCDML_DISP_lang_repeat_242() LCDML_DISP_lang_repeat_241() LCDML_DISP_lang(242),
    #define LCDML_DISP_lang_repeat_241() LCDML_DISP_lang_repeat_240() LCDML_DISP_lang(241),
    #define LCDML_DISP_lang_repeat_240() LCDML_DISP_lang_repeat_239() LCDML_DISP_lang(240),
    #define LCDML_DISP_lang_repeat_239() LCDML_DISP_lang_repeat_238() LCDML_DISP_lang(239),
    #define LCDML_DISP_lang_repeat_238() LCDML_DISP_lang_repeat_237() LCDML_DISP_lang(238),
    #define LCDML_DISP_lang_repeat_237() LCDML_DISP_lang_repeat_236() LCDML_DISP_lang(237),
    #define LCDML_DISP_lang_repeat_236() LCDML_DISP_lang_repeat_235() LCDML_DISP_lang(236),
    #define LCDML_DISP_lang_repeat_235() LCDML_DISP_lang_repeat_234() LCDML_DISP_lang(235),
    #define LCDML_DISP_lang_repeat_234() LCDML_DISP_lang_repeat_233() LCDML_DISP_lang(234),
    #define LCDML_DISP_lang_repeat_233() LCDML_DISP_lang_repeat_232() LCDML_DISP_lang(233),
    #define LCDML_DISP_lang_repeat_232() LCDML_DISP_lang_repeat_231() LCDML_DISP_lang(232),
    #define LCDML_DISP_lang_repeat_231() LCDML_DISP_lang_repeat_230() LCDML_DISP_lang(231),
    #define LCDML_DISP_lang_repeat_230() LCDML_DISP_lang_repeat_229() LCDML_DISP_lang(230),
    #define LCDML_DISP_lang_repeat_229() LCDML_DISP_lang_repeat_228() LCDML_DISP_lang(229),
    #define LCDML_DISP_lang_repeat_228() LCDML_DISP_lang_repeat_227() LCDML_DISP_lang(228),
    #define LCDML_DISP_lang_repeat_227() LCDML_DISP_lang_repeat_226() LCDML_DISP_lang(227),
    #define LCDML_DISP_lang_repeat_226() LCDML_DISP_lang_repeat_225() LCDML_DISP_lang(226),
    #define LCDML_DISP_lang_repeat_225() LCDML_DISP_lang_repeat_224() LCDML_DISP_lang(225),
    #define LCDML_DISP_lang_repeat_224() LCDML_DISP_lang_repeat_223() LCDML_DISP_lang(224),
    #define LCDML_DISP_lang_repeat_223() LCDML_DISP_lang_repeat_222() LCDML_DISP_lang(223),
    #define LCDML_DISP_lang_repeat_222() LCDML_DISP_lang_repeat_221() LCDML_DISP_lang(222),
    #define LCDML_DISP_lang_repeat_221() LCDML_DISP_lang_repeat_220() LCDML_DISP_lang(221),
    #define LCDML_DISP_lang_repeat_220() LCDML_DISP_lang_repeat_219() LCDML_DISP_lang(220),
    #define LCDML_DISP_lang_repeat_219() LCDML_DISP_lang_repeat_218() LCDML_DISP_lang(219),
    #define LCDML_DISP_lang_repeat_218() LCDML_DISP_lang_repeat_217() LCDML_DISP_lang(218),
    #define LCDML_DISP_lang_repeat_217() LCDML_DISP_lang_repeat_216() LCDML_DISP_lang(217),
    #define LCDML_DISP_lang_repeat_216() LCDML_DISP_lang_repeat_215() LCDML_DISP_lang(216),
    #define LCDML_DISP_lang_repeat_215() LCDML_DISP_lang_repeat_214() LCDML_DISP_lang(215),
    #define LCDML_DISP_lang_repeat_214() LCDML_DISP_lang_repeat_213() LCDML_DISP_lang(214),
    #define LCDML_DISP_lang_repeat_213() LCDML_DISP_lang_repeat_212() LCDML_DISP_lang(213),
    #define LCDML_DISP_lang_repeat_212() LCDML_DISP_lang_repeat_211() LCDML_DISP_lang(212),
    #define LCDML_DISP_lang_repeat_211() LCDML_DISP_lang_repeat_210() LCDML_DISP_lang(211),
    #define LCDML_DISP_lang_repeat_210() LCDML_DISP_lang_repeat_209() LCDML_DISP_lang(210),
    #define LCDML_DISP_lang_repeat_209() LCDML_DISP_lang_repeat_208() LCDML_DISP_lang(209),
    #define LCDML_DISP_lang_repeat_208() LCDML_DISP_lang_repeat_207() LCDML_DISP_lang(208),
    #define LCDML_DISP_lang_repeat_207() LCDML_DISP_lang_repeat_206() LCDML_DISP_lang(207),
    #define LCDML_DISP_lang_repeat_206() LCDML_DISP_lang_repeat_205() LCDML_DISP_lang(206),
    #define LCDML_DISP_lang_repeat_205() LCDML_DISP_lang_repeat_204() LCDML_DISP_lang(205),
    #define LCDML_DISP_lang_repeat_204() LCDML_DISP_lang_repeat_203() LCDML_DISP_lang(204),
    #define LCDML_DISP_lang_repeat_203() LCDML_DISP_lang_repeat_202() LCDML_DISP_lang(203),
    #define LCDML_DISP_lang_repeat_202() LCDML_DISP_lang_repeat_201() LCDML_DISP_lang(202),
    #define LCDML_DISP_lang_repeat_201() LCDML_DISP_lang_repeat_200() LCDML_DISP_lang(201),
    #define LCDML_DISP_lang_repeat_200() LCDML_DISP_lang_repeat_199() LCDML_DISP_lang(200),
    #define LCDML_DISP_lang_repeat_199() LCDML_DISP_lang_repeat_198() LCDML_DISP_lang(199),
    #define LCDML_DISP_lang_repeat_198() LCDML_DISP_lang_repeat_197() LCDML_DISP_lang(198),
    #define LCDML_DISP_lang_repeat_197() LCDML_DISP_lang_repeat_196() LCDML_DISP_lang(197),
    #define LCDML_DISP_lang_repeat_196() LCDML_DISP_lang_repeat_195() LCDML_DISP_lang(196),
    #define LCDML_DISP_lang_repeat_195() LCDML_DISP_lang_repeat_194() LCDML_DISP_lang(195),
    #define LCDML_DISP_lang_repeat_194() LCDML_DISP_lang_repeat_193() LCDML_DISP_lang(194),
    #define LCDML_DISP_lang_repeat_193() LCDML_DISP_lang_repeat_192() LCDML_DISP_lang(193),
    #define LCDML_DISP_lang_repeat_192() LCDML_DISP_lang_repeat_191() LCDML_DISP_lang(192),
    #define LCDML_DISP_lang_repeat_191() LCDML_DISP_lang_repeat_190() LCDML_DISP_lang(191),
    #define LCDML_DISP_lang_repeat_190() LCDML_DISP_lang_repeat_189() LCDML_DISP_lang(190),
    #define LCDML_DISP_lang_repeat_189() LCDML_DISP_lang_repeat_188() LCDML_DISP_lang(189),
    #define LCDML_DISP_lang_repeat_188() LCDML_DISP_lang_repeat_187() LCDML_DISP_lang(188),
    #define LCDML_DISP_lang_repeat_187() LCDML_DISP_lang_repeat_186() LCDML_DISP_lang(187),
    #define LCDML_DISP_lang_repeat_186() LCDML_DISP_lang_repeat_185() LCDML_DISP_lang(186),
    #define LCDML_DISP_lang_repeat_185() LCDML_DISP_lang_repeat_184() LCDML_DISP_lang(185),
    #define LCDML_DISP_lang_repeat_184() LCDML_DISP_lang_repeat_183() LCDML_DISP_lang(184),
    #define LCDML_DISP_lang_repeat_183() LCDML_DISP_lang_repeat_182() LCDML_DISP_lang(183),
    #define LCDML_DISP_lang_repeat_182() LCDML_DISP_lang_repeat_181() LCDML_DISP_lang(182),
    #define LCDML_DISP_lang_repeat_181() LCDML_DISP_lang_repeat_180() LCDML_DISP_lang(181),
    #define LCDML_DISP_lang_repeat_180() LCDML_DISP_lang_repeat_179() LCDML_DISP_lang(180),
    #define LCDML_DISP_lang_repeat_179() LCDML_DISP_lang_repeat_178() LCDML_DISP_lang(179),
    #define LCDML_DISP_lang_repeat_178() LCDML_DISP_lang_repeat_177() LCDML_DISP_lang(178),
    #define LCDML_DISP_lang_repeat_177() LCDML_DISP_lang_repeat_176() LCDML_DISP_lang(177),
    #define LCDML_DISP_lang_repeat_176() LCDML_DISP_lang_repeat_175() LCDML_DISP_lang(176),
    #define LCDML_DISP_lang_repeat_175() LCDML_DISP_lang_repeat_174() LCDML_DISP_lang(175),
    #define LCDML_DISP_lang_repeat_174() LCDML_DISP_lang_repeat_173() LCDML_DISP_lang(174),
    #define LCDML_DISP_lang_repeat_173() LCDML_DISP_lang_repeat_172() LCDML_DISP_lang(173),
    #define LCDML_DISP_lang_repeat_172() LCDML_DISP_lang_repeat_171() LCDML_DISP_lang(172),
    #define LCDML_DISP_lang_repeat_171() LCDML_DISP_lang_repeat_170() LCDML_DISP_lang(171),
    #define LCDML_DISP_lang_repeat_170() LCDML_DISP_lang_repeat_169() LCDML_DISP_lang(170),
    #define LCDML_DISP_lang_repeat_169() LCDML_DISP_lang_repeat_168() LCDML_DISP_lang(169),
    #define LCDML_DISP_lang_repeat_168() LCDML_DISP_lang_repeat_167() LCDML_DISP_lang(168),
    #define LCDML_DISP_lang_repeat_167() LCDML_DISP_lang_repeat_166() LCDML_DISP_lang(167),
    #define LCDML_DISP_lang_repeat_166() LCDML_DISP_lang_repeat_165() LCDML_DISP_lang(166),
    #define LCDML_DISP_lang_repeat_165() LCDML_DISP_lang_repeat_164() LCDML_DISP_lang(165),
    #define LCDML_DISP_lang_repeat_164() LCDML_DISP_lang_repeat_163() LCDML_DISP_lang(164),
    #define LCDML_DISP_lang_repeat_163() LCDML_DISP_lang_repeat_162() LCDML_DISP_lang(163),
    #define LCDML_DISP_lang_repeat_162() LCDML_DISP_lang_repeat_161() LCDML_DISP_lang(162),
    #define LCDML_DISP_lang_repeat_161() LCDML_DISP_lang_repeat_160() LCDML_DISP_lang(161),
    #define LCDML_DISP_lang_repeat_160() LCDML_DISP_lang_repeat_159() LCDML_DISP_lang(160),
    #define LCDML_DISP_lang_repeat_159() LCDML_DISP_lang_repeat_158() LCDML_DISP_lang(159),
    #define LCDML_DISP_lang_repeat_158() LCDML_DISP_lang_repeat_157() LCDML_DISP_lang(158),
    #define LCDML_DISP_lang_repeat_157() LCDML_DISP_lang_repeat_156() LCDML_DISP_lang(157),
    #define LCDML_DISP_lang_repeat_156() LCDML_DISP_lang_repeat_155() LCDML_DISP_lang(156),
    #define LCDML_DISP_lang_repeat_155() LCDML_DISP_lang_repeat_154() LCDML_DISP_lang(155),
    #define LCDML_DISP_lang_repeat_154() LCDML_DISP_lang_repeat_153() LCDML_DISP_lang(154),
    #define LCDML_DISP_lang_repeat_153() LCDML_DISP_lang_repeat_152() LCDML_DISP_lang(153),
    #define LCDML_DISP_lang_repeat_152() LCDML_DISP_lang_repeat_151() LCDML_DISP_lang(152),
    #define LCDML_DISP_lang_repeat_151() LCDML_DISP_lang_repeat_150() LCDML_DISP_lang(151),
    #define LCDML_DISP_lang_repeat_150() LCDML_DISP_lang_repeat_149() LCDML_DISP_lang(150),
    #define LCDML_DISP_lang_repeat_149() LCDML_DISP_lang_repeat_148() LCDML_DISP_lang(149),
    #define LCDML_DISP_lang_repeat_148() LCDML_DISP_lang_repeat_147() LCDML_DISP_lang(148),
    #define LCDML_DISP_lang_repeat_147() LCDML_DISP_lang_repeat_146() LCDML_DISP_lang(147),
    #define LCDML_DISP_lang_repeat_146() LCDML_DISP_lang_repeat_145() LCDML_DISP_lang(146),
    #define LCDML_DISP_lang_repeat_145() LCDML_DISP_lang_repeat_144() LCDML_DISP_lang(145),
    #define LCDML_DISP_lang_repeat_144() LCDML_DISP_lang_repeat_143() LCDML_DISP_lang(144),
    #define LCDML_DISP_lang_repeat_143() LCDML_DISP_lang_repeat_142() LCDML_DISP_lang(143),
    #define LCDML_DISP_lang_repeat_142() LCDML_DISP_lang_repeat_141() LCDML_DISP_lang(142),
    #define LCDML_DISP_lang_repeat_141() LCDML_DISP_lang_repeat_140() LCDML_DISP_lang(141),
    #define LCDML_DISP_lang_repeat_140() LCDML_DISP_lang_repeat_139() LCDML_DISP_lang(140),
    #define LCDML_DISP_lang_repeat_139() LCDML_DISP_lang_repeat_138() LCDML_DISP_lang(139),
    #define LCDML_DISP_lang_repeat_138() LCDML_DISP_lang_repeat_137() LCDML_DISP_lang(138),
    #define LCDML_DISP_lang_repeat_137() LCDML_DISP_lang_repeat_136() LCDML_DISP_lang(137),
    #define LCDML_DISP_lang_repeat_136() LCDML_DISP_lang_repeat_135() LCDML_DISP_lang(136),
    #define LCDML_DISP_lang_repeat_135() LCDML_DISP_lang_repeat_134() LCDML_DISP_lang(135),
    #define LCDML_DISP_lang_repeat_134() LCDML_DISP_lang_repeat_133() LCDML_DISP_lang(134),
    #define LCDML_DISP_lang_repeat_133() LCDML_DISP_lang_repeat_132() LCDML_DISP_lang(133),
    #define LCDML_DISP_lang_repeat_132() LCDML_DISP_lang_repeat_131() LCDML_DISP_lang(132),
    #define LCDML_DISP_lang_repeat_131() LCDML_DISP_lang_repeat_130() LCDML_DISP_lang(131),
    #define LCDML_DISP_lang_repeat_130() LCDML_DISP_lang_repeat_129() LCDML_DISP_lang(130),
    #define LCDML_DISP_lang_repeat_129() LCDML_DISP_lang_repeat_128() LCDML_DISP_lang(129),
    #define LCDML_DISP_lang_repeat_128() LCDML_DISP_lang_repeat_127() LCDML_DISP_lang(128),
    #define LCDML_DISP_lang_repeat_127() LCDML_DISP_lang_repeat_126() LCDML_DISP_lang(127),
    #define LCDML_DISP_lang_repeat_126() LCDML_DISP_lang_repeat_125() LCDML_DISP_lang(126),
    #define LCDML_DISP_lang_repeat_125() LCDML_DISP_lang_repeat_124() LCDML_DISP_lang(125),
    #define LCDML_DISP_lang_repeat_124() LCDML_DISP_lang_repeat_123() LCDML_DISP_lang(124),
    #define LCDML_DISP_lang_repeat_123() LCDML_DISP_lang_repeat_122() LCDML_DISP_lang(123),
    #define LCDML_DISP_lang_repeat_122() LCDML_DISP_lang_repeat_121() LCDML_DISP_lang(122),
    #define LCDML_DISP_lang_repeat_121() LCDML_DISP_lang_repeat_120() LCDML_DISP_lang(121),
    #define LCDML_DISP_lang_repeat_120() LCDML_DISP_lang_repeat_119() LCDML_DISP_lang(120),
    #define LCDML_DISP_lang_repeat_119() LCDML_DISP_lang_repeat_118() LCDML_DISP_lang(119),
    #define LCDML_DISP_lang_repeat_118() LCDML_DISP_lang_repeat_117() LCDML_DISP_lang(118),
    #define LCDML_DISP_lang_repeat_117() LCDML_DISP_lang_repeat_116() LCDML_DISP_lang(117),
    #define LCDML_DISP_lang_repeat_116() LCDML_DISP_lang_repeat_115() LCDML_DISP_lang(116),
    #define LCDML_DISP_lang_repeat_115() LCDML_DISP_lang_repeat_114() LCDML_DISP_lang(115),
    #define LCDML_DISP_lang_repeat_114() LCDML_DISP_lang_repeat_113() LCDML_DISP_lang(114),
    #define LCDML_DISP_lang_repeat_113() LCDML_DISP_lang_repeat_112() LCDML_DISP_lang(113),
    #define LCDML_DISP_lang_repeat_112() LCDML_DISP_lang_repeat_111() LCDML_DISP_lang(112),
    #define LCDML_DISP_lang_repeat_111() LCDML_DISP_lang_repeat_110() LCDML_DISP_lang(111),
    #define LCDML_DISP_lang_repeat_110() LCDML_DISP_lang_repeat_109() LCDML_DISP_lang(110),
    #define LCDML_DISP_lang_repeat_109() LCDML_DISP_lang_repeat_108() LCDML_DISP_lang(109),
    #define LCDML_DISP_lang_repeat_108() LCDML_DISP_lang_repeat_107() LCDML_DISP_lang(108),
    #define LCDML_DISP_lang_repeat_107() LCDML_DISP_lang_repeat_106() LCDML_DISP_lang(107),
    #define LCDML_DISP_lang_repeat_106() LCDML_DISP_lang_repeat_105() LCDML_DISP_lang(106),
    #define LCDML_DISP_lang_repeat_105() LCDML_DISP_lang_repeat_104() LCDML_DISP_lang(105),
    #define LCDML_DISP_lang_repeat_104() LCDML_DISP_lang_repeat_103() LCDML_DISP_lang(104),
    #define LCDML_DISP_lang_repeat_103() LCDML_DISP_lang_repeat_102() LCDML_DISP_lang(103),
    #define LCDML_DISP_lang_repeat_102() LCDML_DISP_lang_repeat_101() LCDML_DISP_lang(102),
    #define LCDML_DISP_lang_repeat_101() LCDML_DISP_lang_repeat_100() LCDML_DISP_lang(101),
    #define LCDML_DISP_lang_repeat_100() LCDML_DISP_lang_repeat_99()  LCDML_DISP_lang(100),
    #define LCDML_DISP_lang_repeat_99()  LCDML_DISP_lang_repeat_98()  LCDML_DISP_lang(99),
    #define LCDML_DISP_lang_repeat_98()  LCDML_DISP_lang_repeat_97()  LCDML_DISP_lang(98),
    #define LCDML_DISP_lang_repeat_97()  LCDML_DISP_lang_repeat_96()  LCDML_DISP_lang(97),
    #define LCDML_DISP_lang_repeat_96()  LCDML_DISP_lang_repeat_95()  LCDML_DISP_lang(96),
    #define LCDML_DISP_lang_repeat_95()  LCDML_DISP_lang_repeat_94()  LCDML_DISP_lang(95),
    #define LCDML_DISP_lang_repeat_94()  LCDML_DISP_lang_repeat_93()  LCDML_DISP_lang(94),
    #define LCDML_DISP_lang_repeat_93()  LCDML_DISP_lang_repeat_92()  LCDML_DISP_lang(93),
    #define LCDML_DISP_lang_repeat_92()  LCDML_DISP_lang_repeat_91()  LCDML_DISP_lang(92),
    #define LCDML_DISP_lang_repeat_91()  LCDML_DISP_lang_repeat_90()  LCDML_DISP_lang(91),
    #define LCDML_DISP_lang_repeat_90()  LCDML_DISP_lang_repeat_89()  LCDML_DISP_lang(90),
    #define LCDML_DISP_lang_repeat_89()  LCDML_DISP_lang_repeat_88()  LCDML_DISP_lang(89),
    #define LCDML_DISP_lang_repeat_88()  LCDML_DISP_lang_repeat_87()  LCDML_DISP_lang(88),
    #define LCDML_DISP_lang_repeat_87()  LCDML_DISP_lang_repeat_86()  LCDML_DISP_lang(87),
    #define LCDML_DISP_lang_repeat_86()  LCDML_DISP_lang_repeat_85()  LCDML_DISP_lang(86),
    #define LCDML_DISP_lang_repeat_85()  LCDML_DISP_lang_repeat_84()  LCDML_DISP_lang(85),
    #define LCDML_DISP_lang_repeat_84()  LCDML_DISP_lang_repeat_83()  LCDML_DISP_lang(84),
    #define LCDML_DISP_lang_repeat_83()  LCDML_DISP_lang_repeat_82()  LCDML_DISP_lang(83),
    #define LCDML_DISP_lang_repeat_82()  LCDML_DISP_lang_repeat_81()  LCDML_DISP_lang(82),
    #define LCDML_DISP_lang_repeat_81()  LCDML_DISP_lang_repeat_80()  LCDML_DISP_lang(81),
    #define LCDML_DISP_lang_repeat_80()  LCDML_DISP_lang_repeat_79()  LCDML_DISP_lang(80),
    #define LCDML_DISP_lang_repeat_79()  LCDML_DISP_lang_repeat_78()  LCDML_DISP_lang(79),
    #define LCDML_DISP_lang_repeat_78()  LCDML_DISP_lang_repeat_77()  LCDML_DISP_lang(78),
    #define LCDML_DISP_lang_repeat_77()  LCDML_DISP_lang_repeat_76()  LCDML_DISP_lang(77),
    #define LCDML_DISP_lang_repeat_76()  LCDML_DISP_lang_repeat_75()  LCDML_DISP_lang(76),
    #define LCDML_DISP_lang_repeat_75()  LCDML_DISP_lang_repeat_74()  LCDML_DISP_lang(75),
    #define LCDML_DISP_lang_repeat_74()  LCDML_DISP_lang_repeat_73()  LCDML_DISP_lang(74),
    #define LCDML_DISP_lang_repeat_73()  LCDML_DISP_lang_repeat_72()  LCDML_DISP_lang(73),
    #define LCDML_DISP_lang_repeat_72()  LCDML_DISP_lang_repeat_71()  LCDML_DISP_lang(72),
    #define LCDML_DISP_lang_repeat_71()  LCDML_DISP_lang_repeat_70()  LCDML_DISP_lang(71),
    #define LCDML_DISP_lang_repeat_70()  LCDML_DISP_lang_repeat_69()  LCDML_DISP_lang(70),
    #define LCDML_DISP_lang_repeat_69()  LCDML_DISP_lang_repeat_68()  LCDML_DISP_lang(69),
    #define LCDML_DISP_lang_repeat_68()  LCDML_DISP_lang_repeat_67()  LCDML_DISP_lang(68),
    #define LCDML_DISP_lang_repeat_67()  LCDML_DISP_lang_repeat_66()  LCDML_DISP_lang(67),
    #define LCDML_DISP_lang_repeat_66()  LCDML_DISP_lang_repeat_65()  LCDML_DISP_lang(66),
    #define LCDML_DISP_lang_repeat_65()  LCDML_DISP_lang_repeat_64()  LCDML_DISP_lang(65),
    #define LCDML_DISP_lang_repeat_64()  LCDML_DISP_lang_repeat_63()  LCDML_DISP_lang(64),
    #define LCDML_DISP_lang_repeat_63()  LCDML_DISP_lang_repeat_62()  LCDML_DISP_lang(63),
    #define LCDML_DISP_lang_repeat_62()  LCDML_DISP_lang_repeat_61()  LCDML_DISP_lang(62),
    #define LCDML_DISP_lang_repeat_61()  LCDML_DISP_lang_repeat_60()  LCDML_DISP_lang(61),
    #define LCDML_DISP_lang_repeat_60()  LCDML_DISP_lang_repeat_59()  LCDML_DISP_lang(60),
    #define LCDML_DISP_lang_repeat_59()  LCDML_DISP_lang_repeat_58()  LCDML_DISP_lang(59),
    #define LCDML_DISP_lang_repeat_58()  LCDML_DISP_lang_repeat_57()  LCDML_DISP_lang(58),
    #define LCDML_DISP_lang_repeat_57()  LCDML_DISP_lang_repeat_56()  LCDML_DISP_lang(57),
    #define LCDML_DISP_lang_repeat_56()  LCDML_DISP_lang_repeat_55()  LCDML_DISP_lang(56),
    #define LCDML_DISP_lang_repeat_55()  LCDML_DISP_lang_repeat_54()  LCDML_DISP_lang(55),
    #define LCDML_DISP_lang_repeat_54()  LCDML_DISP_lang_repeat_53()  LCDML_DISP_lang(54),
    #define LCDML_DISP_lang_repeat_53()  LCDML_DISP_lang_repeat_52()  LCDML_DISP_lang(53),
    #define LCDML_DISP_lang_repeat_52()  LCDML_DISP_lang_repeat_51()  LCDML_DISP_lang(52),
    #define LCDML_DISP_lang_repeat_51()  LCDML_DISP_lang_repeat_50()  LCDML_DISP_lang(51),
    #define LCDML_DISP_lang_repeat_50()  LCDML_DISP_lang_repeat_49()  LCDML_DISP_lang(50),
    #define LCDML_DISP_lang_repeat_49()  LCDML_DISP_lang_repeat_48()  LCDML_DISP_lang(49),
    #define LCDML_DISP_lang_repeat_48()  LCDML_DISP_lang_repeat_47()  LCDML_DISP_lang(48),
    #define LCDML_DISP_lang_repeat_47()  LCDML_DISP_lang_repeat_46()  LCDML_DISP_lang(47),
    #define LCDML_DISP_lang_repeat_46()  LCDML_DISP_lang_repeat_45()  LCDML_DISP_lang(46),
    #define LCDML_DISP_lang_repeat_45()  LCDML_DISP_lang_repeat_44()  LCDML_DISP_lang(45),
    #define LCDML_DISP_lang_repeat_44()  LCDML_DISP_lang_repeat_43()  LCDML_DISP_lang(44),
    #define LCDML_DISP_lang_repeat_43()  LCDML_DISP_lang_repeat_42()  LCDML_DISP_lang(43),
    #define LCDML_DISP_lang_repeat_42()  LCDML_DISP_lang_repeat_41()  LCDML_DISP_lang(42),
    #define LCDML_DISP_lang_repeat_41()  LCDML_DISP_lang_repeat_40()  LCDML_DISP_lang(41),
    #define LCDML_DISP_lang_repeat_40()  LCDML_DISP_lang_repeat_39()  LCDML_DISP_lang(40),
    #define LCDML_DISP_lang_repeat_39()  LCDML_DISP_lang_repeat_38()  LCDML_DISP_lang(39),
    #define LCDML_DISP_lang_repeat_38()  LCDML_DISP_lang_repeat_37()  LCDML_DISP_lang(38),
    #define LCDML_DISP_lang_repeat_37()  LCDML_DISP_lang_repeat_36()  LCDML_DISP_lang(37),
    #define LCDML_DISP_lang_repeat_36()  LCDML_DISP_lang_repeat_35()  LCDML_DISP_lang(36),
    #define LCDML_DISP_lang_repeat_35()  LCDML_DISP_lang_repeat_34()  LCDML_DISP_lang(35),
    #define LCDML_DISP_lang_repeat_34()  LCDML_DISP_lang_repeat_33()  LCDML_DISP_lang(34),
    #define LCDML_DISP_lang_repeat_33()  LCDML_DISP_lang_repeat_32()  LCDML_DISP_lang(33),
    #define LCDML_DISP_lang_repeat_32()  LCDML_DISP_lang_repeat_31()  LCDML_DISP_lang(32),
    #define LCDML_DISP_lang_repeat_31()  LCDML_DISP_lang_repeat_30()  LCDML_DISP_lang(31),
    #define LCDML_DISP_lang_repeat_30()  LCDML_DISP_lang_repeat_29()  LCDML_DISP_lang(30),
    #define LCDML_DISP_lang_repeat_29()  LCDML_DISP_lang_repeat_28()  LCDML_DISP_lang(29),
    #define LCDML_DISP_lang_repeat_28()  LCDML_DISP_lang_repeat_27()  LCDML_DISP_lang(28),
    #define LCDML_DISP_lang_repeat_27()  LCDML_DISP_lang_repeat_26()  LCDML_DISP_lang(27),
    #define LCDML_DISP_lang_repeat_26()  LCDML_DISP_lang_repeat_25()  LCDML_DISP_lang(26),
    #define LCDML_DISP_lang_repeat_25()  LCDML_DISP_lang_repeat_24()  LCDML_DISP_lang(25),
    #define LCDML_DISP_lang_repeat_24()  LCDML_DISP_lang_repeat_23()  LCDML_DISP_lang(24),
    #define LCDML_DISP_lang_repeat_23()  LCDML_DISP_lang_repeat_22()  LCDML_DISP_lang(23),
    #define LCDML_DISP_lang_repeat_22()  LCDML_DISP_lang_repeat_21()  LCDML_DISP_lang(22),
    #define LCDML_DISP_lang_repeat_21()  LCDML_DISP_lang_repeat_20()  LCDML_DISP_lang(21),
    #define LCDML_DISP_lang_repeat_20()  LCDML_DISP_lang_repeat_19()  LCDML_DISP_lang(20),
    #define LCDML_DISP_lang_repeat_19()  LCDML_DISP_lang_repeat_18()  LCDML_DISP_lang(19),
    #define LCDML_DISP_lang_repeat_18()  LCDML_DISP_lang_repeat_17()  LCDML_DISP_lang(18),
    #define LCDML_DISP_lang_repeat_17()  LCDML_DISP_lang_repeat_16()  LCDML_DISP_lang(17),
    #define LCDML_DISP_lang_repeat_16()  LCDML_DISP_lang_repeat_15()  LCDML_DISP_lang(16),
    #define LCDML_DISP_lang_repeat_15()  LCDML_DISP_lang_repeat_14()  LCDML_DISP_lang(15),
    #define LCDML_DISP_lang_repeat_14()  LCDML_DISP_lang_repeat_13()  LCDML_DISP_lang(14),
    #define LCDML_DISP_lang_repeat_13()  LCDML_DISP_lang_repeat_12()  LCDML_DISP_lang(13),
    #define LCDML_DISP_lang_repeat_12()  LCDML_DISP_lang_repeat_11()  LCDML_DISP_lang(12),
    #define LCDML_DISP_lang_repeat_11()  LCDML_DISP_lang_repeat_10()  LCDML_DISP_lang(11),
    #define LCDML_DISP_lang_repeat_10()  LCDML_DISP_lang_repeat_9()   LCDML_DISP_lang(10),
    #define LCDML_DISP_lang_repeat_9()   LCDML_DISP_lang_repeat_8()   LCDML_DISP_lang(9),
    #define LCDML_DISP_lang_repeat_8()   LCDML_DISP_lang_repeat_7()   LCDML_DISP_lang(8),
    #define LCDML_DISP_lang_repeat_7()   LCDML_DISP_lang_repeat_6()   LCDML_DISP_lang(7),
    #define LCDML_DISP_lang_repeat_6()   LCDML_DISP_lang_repeat_5()   LCDML_DISP_lang(6),
    #define LCDML_DISP_lang_repeat_5()   LCDML_DISP_lang_repeat_4()   LCDML_DISP_lang(5),
    #define LCDML_DISP_lang_repeat_4()   LCDML_DISP_lang_repeat_3()   LCDML_DISP_lang(4),
    #define LCDML_DISP_lang_repeat_3()   LCDML_DISP_lang_repeat_2()   LCDML_DISP_lang(3),
    #define LCDML_DISP_lang_repeat_2()   LCDML_DISP_lang_repeat_1()   LCDML_DISP_lang(2),
    #define LCDML_DISP_lang_repeat_1()   LCDML_DISP_lang_repeat_0()   LCDML_DISP_lang(1),
    #define LCDML_DISP_lang_repeat_0()   LCDML_DISP_lang(0),
    #define LCDML_DISP_lang_repeat(N)     LCDML_DISP_lang_repeat_##N ()

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
