/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 2.1 of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, 
* see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
*
* Description:
*
*/

#include "mapping.h"

#include <QtCore>
#include <QMap>
#include <QString>

Mapping::Mapping() 
{
}

void Mapping::init()
{
    mapping.insert(Qt::Key_Escape, "Qt::Key_Escape");
    mapping.insert(Qt::Key_Tab, "Qt::Key_Tab");
    mapping.insert(Qt::Key_Backtab, "Qt::Key_Backtab");
    mapping.insert(Qt::Key_Backspace, "Qt::Key_Backspace");
    mapping.insert(Qt::Key_Return, "Qt::Key_Return");
    mapping.insert(Qt::Key_Enter, "Qt::Key_Enter");
    mapping.insert(Qt::Key_Insert, "Qt::Key_Insert");
    mapping.insert(Qt::Key_Delete, "Qt::Key_Delete");
    mapping.insert(Qt::Key_Pause, "Qt::Key_Pause");
    mapping.insert(Qt::Key_Print, "Qt::Key_Print");
    mapping.insert(Qt::Key_SysReq, "Qt::Key_SysReq");
    mapping.insert(Qt::Key_Clear, "Qt::Key_Clear");
    mapping.insert(Qt::Key_Home, "Qt::Key_Home");
    mapping.insert(Qt::Key_End, "Qt::Key_End");
    mapping.insert(Qt::Key_Left, "Qt::Key_Left");
    mapping.insert(Qt::Key_Up, "Qt::Key_Up");
    mapping.insert(Qt::Key_Right, "Qt::Key_Right");
    mapping.insert(Qt::Key_Down, "Qt::Key_Down");
    mapping.insert(Qt::Key_PageUp, "Qt::Key_PageUp");
    mapping.insert(Qt::Key_PageDown, "Qt::Key_PageDown");
    mapping.insert(Qt::Key_Shift, "Qt::Key_Shift");
    mapping.insert(Qt::Key_Control, "Qt::Key_Control");
    mapping.insert(Qt::Key_Meta, "Qt::Key_Meta");
    mapping.insert(Qt::Key_Alt, "Qt::Key_Alt");
    mapping.insert(Qt::Key_CapsLock, "Qt::Key_CapsLock");
    mapping.insert(Qt::Key_NumLock, "Qt::Key_NumLock");
    mapping.insert(Qt::Key_ScrollLock, "Qt::Key_ScrollLock");
    mapping.insert(Qt::Key_F1, "Qt::Key_F1");
    mapping.insert(Qt::Key_F2, "Qt::Key_F2");
    mapping.insert(Qt::Key_F3, "Qt::Key_F3");
    mapping.insert(Qt::Key_F4, "Qt::Key_F4");
    mapping.insert(Qt::Key_F5, "Qt::Key_F5");
    mapping.insert(Qt::Key_F6, "Qt::Key_F6");
    mapping.insert(Qt::Key_F7, "Qt::Key_F7");
    mapping.insert(Qt::Key_F8, "Qt::Key_F8");
    mapping.insert(Qt::Key_F9, "Qt::Key_F9");
    mapping.insert(Qt::Key_F10, "Qt::Key_F10");
    mapping.insert(Qt::Key_F11, "Qt::Key_F11");
    mapping.insert(Qt::Key_F12, "Qt::Key_F12");
    mapping.insert(Qt::Key_F13, "Qt::Key_F13");
    mapping.insert(Qt::Key_F14, "Qt::Key_F14");
    mapping.insert(Qt::Key_F15, "Qt::Key_F15");
    mapping.insert(Qt::Key_F16, "Qt::Key_F16");
    mapping.insert(Qt::Key_F17, "Qt::Key_F17");
    mapping.insert(Qt::Key_F18, "Qt::Key_F18");
    mapping.insert(Qt::Key_F19, "Qt::Key_F19");
    mapping.insert(Qt::Key_F20, "Qt::Key_F20");
    mapping.insert(Qt::Key_F21, "Qt::Key_F21");
    mapping.insert(Qt::Key_F22, "Qt::Key_F22");
    mapping.insert(Qt::Key_F23, "Qt::Key_F23");
    mapping.insert(Qt::Key_F24, "Qt::Key_F24");
    mapping.insert(Qt::Key_F25, "Qt::Key_F25");
    mapping.insert(Qt::Key_F26, "Qt::Key_F26");
    mapping.insert(Qt::Key_F27, "Qt::Key_F27");
    mapping.insert(Qt::Key_F28, "Qt::Key_F28");
    mapping.insert(Qt::Key_F29, "Qt::Key_F29");
    mapping.insert(Qt::Key_F30, "Qt::Key_F30");
    mapping.insert(Qt::Key_F31, "Qt::Key_F31");
    mapping.insert(Qt::Key_F32, "Qt::Key_F32");
    mapping.insert(Qt::Key_F33, "Qt::Key_F33");
    mapping.insert(Qt::Key_F34, "Qt::Key_F34");
    mapping.insert(Qt::Key_F35, "Qt::Key_F35");
    mapping.insert(Qt::Key_Super_L, "Qt::Key_Super_L");
    mapping.insert(Qt::Key_Super_R, "Qt::Key_Super_R");
    mapping.insert(Qt::Key_Menu, "Qt::Key_Menu");
    mapping.insert(Qt::Key_Hyper_L, "Qt::Key_Hyper_L");
    mapping.insert(Qt::Key_Hyper_R, "Qt::Key_Hyper_R");
    mapping.insert(Qt::Key_Help, "Qt::Key_Help");
    mapping.insert(Qt::Key_Direction_L, "Qt::Key_Direction_L");
    mapping.insert(Qt::Key_Direction_R, "Qt::Key_Direction_R");
    mapping.insert(Qt::Key_Space, "Qt::Key_Space");
    mapping.insert(Qt::Key_Any, "Qt::Key_Any");
    mapping.insert(Qt::Key_Exclam, "Qt::Key_Exclam");
    mapping.insert(Qt::Key_QuoteDbl, "Qt::Key_QuoteDbl");
    mapping.insert(Qt::Key_NumberSign, "Qt::Key_NumberSign");
    mapping.insert(Qt::Key_Dollar, "Qt::Key_Dollar");
    mapping.insert(Qt::Key_Percent, "Qt::Key_Percent");
    mapping.insert(Qt::Key_Ampersand, "Qt::Key_Ampersand");
    mapping.insert(Qt::Key_Apostrophe, "Qt::Key_Apostrophe");
    mapping.insert(Qt::Key_ParenLeft, "Qt::Key_ParenLeft");
    mapping.insert(Qt::Key_ParenRight, "Qt::Key_ParenRight");
    mapping.insert(Qt::Key_Asterisk, "Qt::Key_Asterisk");
    mapping.insert(Qt::Key_Plus, "Qt::Key_Plus");
    mapping.insert(Qt::Key_Comma, "Qt::Key_Comma");
    mapping.insert(Qt::Key_Minus, "Qt::Key_Minus");
    mapping.insert(Qt::Key_Period, "Qt::Key_Period");
    mapping.insert(Qt::Key_Slash, "Qt::Key_Slash");
    mapping.insert(Qt::Key_0, "Qt::Key_0");
    mapping.insert(Qt::Key_1, "Qt::Key_1");
    mapping.insert(Qt::Key_2, "Qt::Key_2");
    mapping.insert(Qt::Key_3, "Qt::Key_3");
    mapping.insert(Qt::Key_4, "Qt::Key_4");
    mapping.insert(Qt::Key_5, "Qt::Key_5");
    mapping.insert(Qt::Key_6, "Qt::Key_6");
    mapping.insert(Qt::Key_7, "Qt::Key_7");
    mapping.insert(Qt::Key_8, "Qt::Key_8");
    mapping.insert(Qt::Key_9, "Qt::Key_9");
    mapping.insert(Qt::Key_Colon, "Qt::Key_Colon");
    mapping.insert(Qt::Key_Semicolon, "Qt::Key_Semicolon");
    mapping.insert(Qt::Key_Less, "Qt::Key_Less");
    mapping.insert(Qt::Key_Equal, "Qt::Key_Equal");
    mapping.insert(Qt::Key_Greater, "Qt::Key_Greater");
    mapping.insert(Qt::Key_Question, "Qt::Key_Question");
    mapping.insert(Qt::Key_At, "Qt::Key_At");
    mapping.insert(Qt::Key_A, "Qt::Key_A");
    mapping.insert(Qt::Key_B, "Qt::Key_B");
    mapping.insert(Qt::Key_C, "Qt::Key_C");
    mapping.insert(Qt::Key_D, "Qt::Key_D");
    mapping.insert(Qt::Key_E, "Qt::Key_E");
    mapping.insert(Qt::Key_F, "Qt::Key_F");
    mapping.insert(Qt::Key_G, "Qt::Key_G");
    mapping.insert(Qt::Key_H, "Qt::Key_H");
    mapping.insert(Qt::Key_I, "Qt::Key_I");
    mapping.insert(Qt::Key_J, "Qt::Key_J");
    mapping.insert(Qt::Key_K, "Qt::Key_K");
    mapping.insert(Qt::Key_L, "Qt::Key_L");
    mapping.insert(Qt::Key_M, "Qt::Key_M");
    mapping.insert(Qt::Key_N, "Qt::Key_N");
    mapping.insert(Qt::Key_O, "Qt::Key_O");
    mapping.insert(Qt::Key_P, "Qt::Key_P");
    mapping.insert(Qt::Key_Q, "Qt::Key_Q");
    mapping.insert(Qt::Key_R, "Qt::Key_R");
    mapping.insert(Qt::Key_S, "Qt::Key_S");
    mapping.insert(Qt::Key_T, "Qt::Key_T");
    mapping.insert(Qt::Key_U, "Qt::Key_U");
    mapping.insert(Qt::Key_V, "Qt::Key_V");
    mapping.insert(Qt::Key_W, "Qt::Key_W");
    mapping.insert(Qt::Key_X, "Qt::Key_X");
    mapping.insert(Qt::Key_Y, "Qt::Key_Y");
    mapping.insert(Qt::Key_Z, "Qt::Key_Z");
    mapping.insert(Qt::Key_BracketLeft, "Qt::Key_BracketLeft");
    mapping.insert(Qt::Key_Backslash, "Qt::Key_Backslash");
    mapping.insert(Qt::Key_BracketRight, "Qt::Key_BracketRight");
    mapping.insert(Qt::Key_AsciiCircum, "Qt::Key_AsciiCircum");
    mapping.insert(Qt::Key_Underscore, "Qt::Key_Underscore");
    mapping.insert(Qt::Key_QuoteLeft, "Qt::Key_QuoteLeft");
    mapping.insert(Qt::Key_BraceLeft, "Qt::Key_BraceLeft");
    mapping.insert(Qt::Key_Bar, "Qt::Key_Bar");
    mapping.insert(Qt::Key_BraceRight, "Qt::Key_BraceRight");
    mapping.insert(Qt::Key_AsciiTilde, "Qt::Key_AsciiTilde");
    mapping.insert(Qt::Key_nobreakspace, "Qt::Key_nobreakspace");
    mapping.insert(Qt::Key_exclamdown, "Qt::Key_exclamdown");
    mapping.insert(Qt::Key_cent, "Qt::Key_cent");
    mapping.insert(Qt::Key_sterling, "Qt::Key_sterling");
    mapping.insert(Qt::Key_currency, "Qt::Key_currency");
    mapping.insert(Qt::Key_yen, "Qt::Key_yen");
    mapping.insert(Qt::Key_brokenbar, "Qt::Key_brokenbar");
    mapping.insert(Qt::Key_section, "Qt::Key_section");
    mapping.insert(Qt::Key_diaeresis, "Qt::Key_diaeresis");
    mapping.insert(Qt::Key_copyright, "Qt::Key_copyright");
    mapping.insert(Qt::Key_ordfeminine, "Qt::Key_ordfeminine");
    mapping.insert(Qt::Key_guillemotleft, "Qt::Key_guillemotleft");
    mapping.insert(Qt::Key_notsign, "Qt::Key_notsign");
    mapping.insert(Qt::Key_hyphen, "Qt::Key_hyphen");
    mapping.insert(Qt::Key_registered, "Qt::Key_registered");
    mapping.insert(Qt::Key_macron, "Qt::Key_macron");
    mapping.insert(Qt::Key_degree, "Qt::Key_degree");
    mapping.insert(Qt::Key_plusminus, "Qt::Key_plusminus");
    mapping.insert(Qt::Key_twosuperior, "Qt::Key_twosuperior");
    mapping.insert(Qt::Key_threesuperior, "Qt::Key_threesuperior");
    mapping.insert(Qt::Key_acute, "Qt::Key_acute");
    mapping.insert(Qt::Key_mu, "Qt::Key_mu");
    mapping.insert(Qt::Key_paragraph, "Qt::Key_paragraph");
    mapping.insert(Qt::Key_periodcentered, "Qt::Key_periodcentered");
    mapping.insert(Qt::Key_cedilla, "Qt::Key_cedilla");
    mapping.insert(Qt::Key_onesuperior, "Qt::Key_onesuperior");
    mapping.insert(Qt::Key_masculine, "Qt::Key_masculine");
    mapping.insert(Qt::Key_guillemotright, "Qt::Key_guillemotright");
    mapping.insert(Qt::Key_onequarter, "Qt::Key_onequarter");
    mapping.insert(Qt::Key_onehalf, "Qt::Key_onehalf");
    mapping.insert(Qt::Key_threequarters, "Qt::Key_threequarters");
    mapping.insert(Qt::Key_questiondown, "Qt::Key_questiondown");
    mapping.insert(Qt::Key_Agrave, "Qt::Key_Agrave");
    mapping.insert(Qt::Key_Aacute, "Qt::Key_Aacute");
    mapping.insert(Qt::Key_Acircumflex, "Qt::Key_Acircumflex");
    mapping.insert(Qt::Key_Atilde, "Qt::Key_Atilde");
    mapping.insert(Qt::Key_Adiaeresis, "Qt::Key_Adiaeresis");
    mapping.insert(Qt::Key_Aring, "Qt::Key_Aring");
    mapping.insert(Qt::Key_AE, "Qt::Key_AE");
    mapping.insert(Qt::Key_Ccedilla, "Qt::Key_Ccedilla");
    mapping.insert(Qt::Key_Egrave, "Qt::Key_Egrave");
    mapping.insert(Qt::Key_Eacute, "Qt::Key_Eacute");
    mapping.insert(Qt::Key_Ecircumflex, "Qt::Key_Ecircumflex");
    mapping.insert(Qt::Key_Ediaeresis, "Qt::Key_Ediaeresis");
    mapping.insert(Qt::Key_Igrave, "Qt::Key_Igrave");
    mapping.insert(Qt::Key_Iacute, "Qt::Key_Iacute");
    mapping.insert(Qt::Key_Icircumflex, "Qt::Key_Icircumflex");
    mapping.insert(Qt::Key_Idiaeresis, "Qt::Key_Idiaeresis");
    mapping.insert(Qt::Key_ETH, "Qt::Key_ETH");
    mapping.insert(Qt::Key_Ntilde, "Qt::Key_Ntilde");
    mapping.insert(Qt::Key_Ograve, "Qt::Key_Ograve");
    mapping.insert(Qt::Key_Oacute, "Qt::Key_Oacute");
    mapping.insert(Qt::Key_Ocircumflex, "Qt::Key_Ocircumflex");
    mapping.insert(Qt::Key_Otilde, "Qt::Key_Otilde");
    mapping.insert(Qt::Key_Odiaeresis, "Qt::Key_Odiaeresis");
    mapping.insert(Qt::Key_multiply, "Qt::Key_multiply");
    mapping.insert(Qt::Key_Ooblique, "Qt::Key_Ooblique");
    mapping.insert(Qt::Key_Ugrave, "Qt::Key_Ugrave");
    mapping.insert(Qt::Key_Uacute, "Qt::Key_Uacute");
    mapping.insert(Qt::Key_Ucircumflex, "Qt::Key_Ucircumflex");
    mapping.insert(Qt::Key_Udiaeresis, "Qt::Key_Udiaeresis");
    mapping.insert(Qt::Key_Yacute, "Qt::Key_Yacute");
    mapping.insert(Qt::Key_THORN, "Qt::Key_THORN");
    mapping.insert(Qt::Key_ssharp, "Qt::Key_ssharp");
    mapping.insert(Qt::Key_division, "Qt::Key_division");
    mapping.insert(Qt::Key_ydiaeresis, "Qt::Key_ydiaeresis");
    mapping.insert(Qt::Key_AltGr, "Qt::Key_AltGr");
    mapping.insert(Qt::Key_Multi_key, "Qt::Key_Multi_key");
    mapping.insert(Qt::Key_Codeinput, "Qt::Key_Codeinput");
    mapping.insert(Qt::Key_SingleCandidate, "Qt::Key_SingleCandidate");
    mapping.insert(Qt::Key_MultipleCandidate, "Qt::Key_MultipleCandidate");
    mapping.insert(Qt::Key_PreviousCandidate, "Qt::Key_PreviousCandidate");
    mapping.insert(Qt::Key_Mode_switch, "Qt::Key_Mode_switch");
    mapping.insert(Qt::Key_Kanji, "Qt::Key_Kanji");
    mapping.insert(Qt::Key_Muhenkan, "Qt::Key_Muhenkan");
    mapping.insert(Qt::Key_Henkan, "Qt::Key_Henkan");
    mapping.insert(Qt::Key_Romaji, "Qt::Key_Romaji");
    mapping.insert(Qt::Key_Hiragana, "Qt::Key_Hiragana");
    mapping.insert(Qt::Key_Katakana, "Qt::Key_Katakana");
    mapping.insert(Qt::Key_Hiragana_Katakana, "Qt::Key_Hiragana_Katakana");
    mapping.insert(Qt::Key_Zenkaku, "Qt::Key_Zenkaku");
    mapping.insert(Qt::Key_Hankaku, "Qt::Key_Hankaku");
    mapping.insert(Qt::Key_Zenkaku_Hankaku, "Qt::Key_Zenkaku_Hankaku");
    mapping.insert(Qt::Key_Touroku, "Qt::Key_Touroku");
    mapping.insert(Qt::Key_Massyo, "Qt::Key_Massyo");
    mapping.insert(Qt::Key_Kana_Lock, "Qt::Key_Kana_Lock");
    mapping.insert(Qt::Key_Kana_Shift, "Qt::Key_Kana_Shift");
    mapping.insert(Qt::Key_Eisu_Shift, "Qt::Key_Eisu_Shift");
    mapping.insert(Qt::Key_Eisu_toggle, "Qt::Key_Eisu_toggle");
    mapping.insert(Qt::Key_Hangul, "Qt::Key_Hangul");
    mapping.insert(Qt::Key_Hangul_Start, "Qt::Key_Hangul_Start");
    mapping.insert(Qt::Key_Hangul_End, "Qt::Key_Hangul_End");
    mapping.insert(Qt::Key_Hangul_Hanja, "Qt::Key_Hangul_Hanja");
    mapping.insert(Qt::Key_Hangul_Jamo, "Qt::Key_Hangul_Jamo");
    mapping.insert(Qt::Key_Hangul_Romaja, "Qt::Key_Hangul_Romaja");
    mapping.insert(Qt::Key_Hangul_Jeonja, "Qt::Key_Hangul_Jeonja");
    mapping.insert(Qt::Key_Hangul_Banja, "Qt::Key_Hangul_Banja");
    mapping.insert(Qt::Key_Hangul_PreHanja, "Qt::Key_Hangul_PreHanja");
    mapping.insert(Qt::Key_Hangul_PostHanja, "Qt::Key_Hangul_PostHanja");
    mapping.insert(Qt::Key_Hangul_Special, "Qt::Key_Hangul_Special");
    mapping.insert(Qt::Key_Dead_Grave, "Qt::Key_Dead_Grave");
    mapping.insert(Qt::Key_Dead_Acute, "Qt::Key_Dead_Acute");
    mapping.insert(Qt::Key_Dead_Circumflex, "Qt::Key_Dead_Circumflex");
    mapping.insert(Qt::Key_Dead_Tilde, "Qt::Key_Dead_Tilde");
    mapping.insert(Qt::Key_Dead_Macron, "Qt::Key_Dead_Macron");
    mapping.insert(Qt::Key_Dead_Breve, "Qt::Key_Dead_Breve");
    mapping.insert(Qt::Key_Dead_Abovedot, "Qt::Key_Dead_Abovedot");
    mapping.insert(Qt::Key_Dead_Diaeresis, "Qt::Key_Dead_Diaeresis");
    mapping.insert(Qt::Key_Dead_Abovering, "Qt::Key_Dead_Abovering");
    mapping.insert(Qt::Key_Dead_Doubleacute, "Qt::Key_Dead_Doubleacute");
    mapping.insert(Qt::Key_Dead_Caron, "Qt::Key_Dead_Caron");
    mapping.insert(Qt::Key_Dead_Cedilla, "Qt::Key_Dead_Cedilla");
    mapping.insert(Qt::Key_Dead_Ogonek, "Qt::Key_Dead_Ogonek");
    mapping.insert(Qt::Key_Dead_Iota, "Qt::Key_Dead_Iota");
    mapping.insert(Qt::Key_Dead_Voiced_Sound, "Qt::Key_Dead_Voiced_Sound");
    mapping.insert(Qt::Key_Dead_Semivoiced_Sound, "Qt::Key_Dead_Semivoiced_Sound");
    mapping.insert(Qt::Key_Dead_Belowdot, "Qt::Key_Dead_Belowdot");
    mapping.insert(Qt::Key_Dead_Hook, "Qt::Key_Dead_Hook");
    mapping.insert(Qt::Key_Dead_Horn, "Qt::Key_Dead_Horn");
    mapping.insert(Qt::Key_Back, "Qt::Key_Back");
    mapping.insert(Qt::Key_Forward, "Qt::Key_Forward");
    mapping.insert(Qt::Key_Stop, "Qt::Key_Stop");
    mapping.insert(Qt::Key_Refresh, "Qt::Key_Refresh");
    mapping.insert(Qt::Key_VolumeDown, "Qt::Key_VolumeDown");
    mapping.insert(Qt::Key_VolumeMute, "Qt::Key_VolumeMute");
    mapping.insert(Qt::Key_VolumeUp, "Qt::Key_VolumeUp");
    mapping.insert(Qt::Key_BassBoost, "Qt::Key_BassBoost");
    mapping.insert(Qt::Key_BassUp, "Qt::Key_BassUp");
    mapping.insert(Qt::Key_BassDown, "Qt::Key_BassDown");
    mapping.insert(Qt::Key_TrebleUp, "Qt::Key_TrebleUp");
    mapping.insert(Qt::Key_TrebleDown, "Qt::Key_TrebleDown");
    mapping.insert(Qt::Key_MediaPlay, "Qt::Key_MediaPlay");
    mapping.insert(Qt::Key_MediaStop, "Qt::Key_MediaStop");
    mapping.insert(Qt::Key_MediaPrevious, "Qt::Key_MediaPrevious");
    mapping.insert(Qt::Key_MediaNext, "Qt::Key_MediaNext");
    mapping.insert(Qt::Key_MediaRecord, "Qt::Key_MediaRecord");
    mapping.insert(Qt::Key_HomePage, "Qt::Key_HomePage");
    mapping.insert(Qt::Key_Favorites, "Qt::Key_Favorites");
    mapping.insert(Qt::Key_Search, "Qt::Key_Search");
    mapping.insert(Qt::Key_Standby, "Qt::Key_Standby");
    mapping.insert(Qt::Key_OpenUrl, "Qt::Key_OpenUrl");
    mapping.insert(Qt::Key_LaunchMail, "Qt::Key_LaunchMail");
    mapping.insert(Qt::Key_LaunchMedia, "Qt::Key_LaunchMedia");
    mapping.insert(Qt::Key_Launch0, "Qt::Key_Launch0");
    mapping.insert(Qt::Key_Launch1, "Qt::Key_Launch1");
    mapping.insert(Qt::Key_Launch2, "Qt::Key_Launch2");
    mapping.insert(Qt::Key_Launch3, "Qt::Key_Launch3");
    mapping.insert(Qt::Key_Launch4, "Qt::Key_Launch4");
    mapping.insert(Qt::Key_Launch5, "Qt::Key_Launch5");
    mapping.insert(Qt::Key_Launch6, "Qt::Key_Launch6");
    mapping.insert(Qt::Key_Launch7, "Qt::Key_Launch7");
    mapping.insert(Qt::Key_Launch8, "Qt::Key_Launch8");
    mapping.insert(Qt::Key_Launch9, "Qt::Key_Launch9");
    mapping.insert(Qt::Key_LaunchA, "Qt::Key_LaunchA");
    mapping.insert(Qt::Key_LaunchB, "Qt::Key_LaunchB");
    mapping.insert(Qt::Key_LaunchC, "Qt::Key_LaunchC");
    mapping.insert(Qt::Key_LaunchD, "Qt::Key_LaunchD");
    mapping.insert(Qt::Key_LaunchE, "Qt::Key_LaunchE");
    mapping.insert(Qt::Key_LaunchF, "Qt::Key_LaunchF");
    mapping.insert(Qt::Key_MonBrightnessUp, "Qt::Key_MonBrightnessUp");
    mapping.insert(Qt::Key_MonBrightnessDown, "Qt::Key_MonBrightnessDown");
    mapping.insert(Qt::Key_KeyboardLightOnOff, "Qt::Key_KeyboardLightOnOff");
    mapping.insert(Qt::Key_KeyboardBrightnessUp, "Qt::Key_KeyboardBrightnessUp");
    mapping.insert(Qt::Key_KeyboardBrightnessDown, "Qt::Key_KeyboardBrightnessDown");
    mapping.insert(Qt::Key_PowerOff, "Qt::Key_PowerOff");
    mapping.insert(Qt::Key_WakeUp, "Qt::Key_WakeUp");
    mapping.insert(Qt::Key_Eject, "Qt::Key_Eject");
    mapping.insert(Qt::Key_ScreenSaver, "Qt::Key_ScreenSaver");
    mapping.insert(Qt::Key_WWW, "Qt::Key_WWW");
    mapping.insert(Qt::Key_Memo, "Qt::Key_Memo");
    mapping.insert(Qt::Key_LightBulb, "Qt::Key_LightBulb");
    mapping.insert(Qt::Key_Shop, "Qt::Key_Shop");
    mapping.insert(Qt::Key_History, "Qt::Key_History");
    mapping.insert(Qt::Key_AddFavorite, "Qt::Key_AddFavorite");
    mapping.insert(Qt::Key_HotLinks, "Qt::Key_HotLinks");
    mapping.insert(Qt::Key_BrightnessAdjust, "Qt::Key_BrightnessAdjust");
    mapping.insert(Qt::Key_Finance, "Qt::Key_Finance");
    mapping.insert(Qt::Key_Community, "Qt::Key_Community");
    mapping.insert(Qt::Key_AudioRewind, "Qt::Key_AudioRewind");
    mapping.insert(Qt::Key_BackForward, "Qt::Key_BackForward");
    mapping.insert(Qt::Key_ApplicationLeft, "Qt::Key_ApplicationLeft");
    mapping.insert(Qt::Key_ApplicationRight, "Qt::Key_ApplicationRight");
    mapping.insert(Qt::Key_Book, "Qt::Key_Book");
    mapping.insert(Qt::Key_CD, "Qt::Key_CD");
    mapping.insert(Qt::Key_Calculator, "Qt::Key_Calculator");
    mapping.insert(Qt::Key_ToDoList, "Qt::Key_ToDoList");
    mapping.insert(Qt::Key_ClearGrab, "Qt::Key_ClearGrab");
    mapping.insert(Qt::Key_Close, "Qt::Key_Close");
    mapping.insert(Qt::Key_Copy, "Qt::Key_Copy");
    mapping.insert(Qt::Key_Cut, "Qt::Key_Cut");
    mapping.insert(Qt::Key_Display, "Qt::Key_Display");
    mapping.insert(Qt::Key_DOS, "Qt::Key_DOS");
    mapping.insert(Qt::Key_Documents, "Qt::Key_Documents");
    mapping.insert(Qt::Key_Excel, "Qt::Key_Excel");
    mapping.insert(Qt::Key_Explorer, "Qt::Key_Explorer");
    mapping.insert(Qt::Key_Game, "Qt::Key_Game");
    mapping.insert(Qt::Key_Go, "Qt::Key_Go");
    mapping.insert(Qt::Key_iTouch, "Qt::Key_iTouch");
    mapping.insert(Qt::Key_LogOff, "Qt::Key_LogOff");
    mapping.insert(Qt::Key_Market, "Qt::Key_Market");
    mapping.insert(Qt::Key_Meeting, "Qt::Key_Meeting");
    mapping.insert(Qt::Key_MenuKB, "Qt::Key_MenuKB");
    mapping.insert(Qt::Key_MenuPB, "Qt::Key_MenuPB");
    mapping.insert(Qt::Key_MySites, "Qt::Key_MySites");
    mapping.insert(Qt::Key_News, "Qt::Key_News");
    mapping.insert(Qt::Key_OfficeHome, "Qt::Key_OfficeHome");
    mapping.insert(Qt::Key_Option, "Qt::Key_Option");
    mapping.insert(Qt::Key_Paste, "Qt::Key_Paste");
    mapping.insert(Qt::Key_Phone, "Qt::Key_Phone");
    mapping.insert(Qt::Key_Calendar, "Qt::Key_Calendar");
    mapping.insert(Qt::Key_Reply, "Qt::Key_Reply");
    mapping.insert(Qt::Key_Reload, "Qt::Key_Reload");
    mapping.insert(Qt::Key_RotateWindows, "Qt::Key_RotateWindows");
    mapping.insert(Qt::Key_RotationPB, "Qt::Key_RotationPB");
    mapping.insert(Qt::Key_RotationKB, "Qt::Key_RotationKB");
    mapping.insert(Qt::Key_Save, "Qt::Key_Save");
    mapping.insert(Qt::Key_Send, "Qt::Key_Send");
    mapping.insert(Qt::Key_Spell, "Qt::Key_Spell");
    mapping.insert(Qt::Key_SplitScreen, "Qt::Key_SplitScreen");
    mapping.insert(Qt::Key_Support, "Qt::Key_Support");
    mapping.insert(Qt::Key_TaskPane, "Qt::Key_TaskPane");
    mapping.insert(Qt::Key_Terminal, "Qt::Key_Terminal");
    mapping.insert(Qt::Key_Tools, "Qt::Key_Tools");
    mapping.insert(Qt::Key_Travel, "Qt::Key_Travel");
    mapping.insert(Qt::Key_Video, "Qt::Key_Video");
    mapping.insert(Qt::Key_Word, "Qt::Key_Word");
    mapping.insert(Qt::Key_Xfer, "Qt::Key_Xfer");
    mapping.insert(Qt::Key_ZoomIn, "Qt::Key_ZoomIn");
    mapping.insert(Qt::Key_ZoomOut, "Qt::Key_ZoomOut");
    mapping.insert(Qt::Key_Away, "Qt::Key_Away");
    mapping.insert(Qt::Key_Messenger, "Qt::Key_Messenger");
    mapping.insert(Qt::Key_WebCam, "Qt::Key_WebCam");
    mapping.insert(Qt::Key_MailForward, "Qt::Key_MailForward");
    mapping.insert(Qt::Key_Pictures, "Qt::Key_Pictures");
    mapping.insert(Qt::Key_Music, "Qt::Key_Music");
    mapping.insert(Qt::Key_Battery, "Qt::Key_Battery");
    mapping.insert(Qt::Key_Bluetooth, "Qt::Key_Bluetooth");
    mapping.insert(Qt::Key_WLAN, "Qt::Key_WLAN");
    mapping.insert(Qt::Key_UWB, "Qt::Key_UWB");
    mapping.insert(Qt::Key_AudioForward, "Qt::Key_AudioForward");
    mapping.insert(Qt::Key_AudioRepeat, "Qt::Key_AudioRepeat");
    mapping.insert(Qt::Key_AudioRandomPlay, "Qt::Key_AudioRandomPlay");
    mapping.insert(Qt::Key_Subtitle, "Qt::Key_Subtitle");
    mapping.insert(Qt::Key_AudioCycleTrack, "Qt::Key_AudioCycleTrack");
    mapping.insert(Qt::Key_Time, "Qt::Key_Time");
    mapping.insert(Qt::Key_Hibernate, "Qt::Key_Hibernate");
    mapping.insert(Qt::Key_View, "Qt::Key_View");
    mapping.insert(Qt::Key_TopMenu, "Qt::Key_TopMenu");
    mapping.insert(Qt::Key_PowerDown, "Qt::Key_PowerDown");
    mapping.insert(Qt::Key_Suspend, "Qt::Key_Suspend");
    mapping.insert(Qt::Key_ContrastAdjust, "Qt::Key_ContrastAdjust");
    mapping.insert(Qt::Key_MediaLast, "Qt::Key_MediaLast");
    mapping.insert(Qt::Key_Select, "Qt::Key_Select");
    mapping.insert(Qt::Key_Yes, "Qt::Key_Yes");
    mapping.insert(Qt::Key_No, "Qt::Key_No");
    mapping.insert(Qt::Key_Cancel, "Qt::Key_Cancel");
    mapping.insert(Qt::Key_Printer, "Qt::Key_Printer");
    mapping.insert(Qt::Key_Execute, "Qt::Key_Execute");
    mapping.insert(Qt::Key_Sleep, "Qt::Key_Sleep");
    mapping.insert(Qt::Key_Play, "Qt::Key_Play");
    mapping.insert(Qt::Key_Zoom, "Qt::Key_Zoom");
    mapping.insert(Qt::Key_Context1, "Qt::Key_Context1");
    mapping.insert(Qt::Key_Context2, "Qt::Key_Context2");
    mapping.insert(Qt::Key_Context3, "Qt::Key_Context3");
    mapping.insert(Qt::Key_Context4, "Qt::Key_Context4");
    mapping.insert(Qt::Key_Call, "Qt::Key_Call");
    mapping.insert(Qt::Key_Hangup, "Qt::Key_Hangup");
    mapping.insert(Qt::Key_Flip, "Qt::Key_Flip");
    mapping.insert(Qt::Key_unknown, "Qt::Key_unknown");
}

const QString Mapping::name(Qt::Key key)
{
    if (mapping.size() == 0) {
        init();
    }
    
    const QString &val = mapping.value(key);
    
    if (val.isEmpty()) {
        return QString("???");
    } else {   
        return mapping.value(key);
    }
}

    
