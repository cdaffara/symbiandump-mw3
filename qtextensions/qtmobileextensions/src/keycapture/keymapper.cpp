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

//
// TODO: this file is only tempral while we wait for Qt patch approval and include
// 
 
#include "keymapper.h"
#include <e32keys.h>
using namespace Qt;

static const KeyMapping keyMapping[] = {
    {EKeyBackspace, EStdKeyBackspace, Key_Backspace},
    {EKeyTab, EStdKeyTab, Key_Tab},
    {EKeyEnter, EStdKeyEnter, Key_Enter},
    {EKeyEscape, EStdKeyEscape, Key_Escape},
    {EKeySpace, EStdKeySpace, Key_Space},
    {EKeyDelete, EStdKeyDelete, Key_Delete},
    {EKeyPrintScreen, EStdKeyPrintScreen, Key_SysReq},
    {EKeyPause, EStdKeyPause, Key_Pause},
    {EKeyHome, EStdKeyHome, Key_Home},
    {EKeyEnd, EStdKeyEnd, Key_End},
    {EKeyPageUp, EStdKeyPageUp, Key_PageUp},
    {EKeyPageDown, EStdKeyPageDown, Key_PageDown},
    {EKeyInsert, EStdKeyInsert, Key_Insert},
    {EKeyLeftArrow, EStdKeyLeftArrow, Key_Left},
    {EKeyRightArrow, EStdKeyRightArrow, Key_Right},
    {EKeyUpArrow, EStdKeyUpArrow, Key_Up},
    {EKeyDownArrow, EStdKeyDownArrow, Key_Down},
    {EKeyLeftShift, EStdKeyLeftShift, Key_Shift},
    {EKeyRightShift, EStdKeyRightShift, Key_Shift},
    {EKeyLeftAlt, EStdKeyLeftAlt, Key_Alt},
    {EKeyRightAlt, EStdKeyRightAlt, Key_AltGr},
    {EKeyLeftCtrl, EStdKeyLeftCtrl, Key_Control},
    {EKeyRightCtrl, EStdKeyRightCtrl, Key_Control},
    {EKeyLeftFunc, EStdKeyLeftFunc, Key_Super_L},
    {EKeyRightFunc, EStdKeyRightFunc, Key_Super_R},
    {EKeyCapsLock, EStdKeyCapsLock, Key_CapsLock},
    {EKeyNumLock, EStdKeyNumLock, Key_NumLock},
    {EKeyScrollLock, EStdKeyScrollLock, Key_ScrollLock},
    {EKeyF1, EStdKeyF1, Key_F1},
    {EKeyF2, EStdKeyF2, Key_F2},
    {EKeyF3, EStdKeyF3, Key_F3},
    {EKeyF4, EStdKeyF4, Key_F4},
    {EKeyF5, EStdKeyF5, Key_F5},
    {EKeyF6, EStdKeyF6, Key_F6},
    {EKeyF7, EStdKeyF7, Key_F7},
    {EKeyF8, EStdKeyF8, Key_F8},
    {EKeyF9, EStdKeyF9, Key_F9},
    {EKeyF10, EStdKeyF10, Key_F10},
    {EKeyF11, EStdKeyF11, Key_F11},
    {EKeyF12, EStdKeyF12, Key_F12},
    {EKeyF13, EStdKeyF13, Key_F13},
    {EKeyF14, EStdKeyF14, Key_F14},
    {EKeyF15, EStdKeyF15, Key_F15},
    {EKeyF16, EStdKeyF16, Key_F16},
    {EKeyF17, EStdKeyF17, Key_F17},
    {EKeyF18, EStdKeyF18, Key_F18},
    {EKeyF19, EStdKeyF19, Key_F19},
    {EKeyF20, EStdKeyF20, Key_F20},
    {EKeyF21, EStdKeyF21, Key_F21},
    {EKeyF22, EStdKeyF22, Key_F22},
    {EKeyF23, EStdKeyF23, Key_F23},
    {EKeyF24, EStdKeyF24, Key_F24},
    {EKeyOff, EStdKeyOff, Key_PowerOff},
//    {EKeyMenu, EStdKeyMenu, Key_Menu}, // Menu is EKeyApplication0
    {EKeyHelp, EStdKeyHelp, Key_Help},
    {EKeyDial, EStdKeyDial, Key_Call},
    {EKeyIncVolume, EStdKeyIncVolume, Key_VolumeUp},
    {EKeyDecVolume, EStdKeyDecVolume, Key_VolumeDown},
    {EKeyDevice0, EStdKeyDevice0, Key_Context1}, // Found by manual testing.
    {EKeyDevice1, EStdKeyDevice1, Key_Context2}, // Found by manual testing.
    {EKeyDevice3, EStdKeyDevice3, Key_Select},
    {EKeyDevice7, EStdKeyDevice7, Key_Camera},  
    {EKeyApplication0, EStdKeyApplication0, Key_Menu}, // Found by manual testing.
    {EKeyApplication1, EStdKeyApplication1, Key_Launch1}, // Found by manual testing.
    {EKeyApplication2, EStdKeyApplication2, Key_MediaPlay}, // Found by manual testing.
    {EKeyApplication3, EStdKeyApplication3, Key_MediaStop}, // Found by manual testing.
    {EKeyApplication4, EStdKeyApplication4, Key_MediaNext}, // Found by manual testing.
    {EKeyApplication5, EStdKeyApplication5, Key_MediaPrevious}, // Found by manual testing.
    {EKeyApplication6, EStdKeyApplication6, Key_Launch6},
    {EKeyApplication7, EStdKeyApplication7, Key_Launch7},
    {EKeyApplication8, EStdKeyApplication8, Key_Launch8},
    {EKeyApplication9, EStdKeyApplication9, Key_Launch9},
    {EKeyApplicationA, EStdKeyApplicationA, Key_LaunchA},
    {EKeyApplicationB, EStdKeyApplicationB, Key_LaunchB},
    {EKeyApplicationC, EStdKeyApplicationC, Key_LaunchC},
    {EKeyApplicationD, EStdKeyApplicationD, Key_LaunchD},
    {EKeyApplicationE, EStdKeyApplicationE, Key_LaunchE},
    {EKeyApplicationF, EStdKeyApplicationF, Key_LaunchF},
    {EKeyApplication19, EStdKeyApplication19, Key_CameraFocus}, 
    {EKeyYes, EStdKeyYes, Key_Yes},
    {EKeyNo, EStdKeyNo, Key_No},
    {TKeyCode(0), TStdScanCode(0), Qt::Key(0)}
};

QKeyMapper::QKeyMapper()
{
   // fillKeyMap();
}

QKeyMapper::~QKeyMapper()
{
}

int QKeyMapper::mapS60KeyToQt(TUint s60key)
{
    int res = Qt::Key_unknown;
    for (int i = 0; keyMapping[i].s60KeyCode != 0; i++) {
        if (keyMapping[i].s60KeyCode == s60key) {
            res = keyMapping[i].qtKey;
            break;
        }
    }
    return res;
}

int QKeyMapper::mapS60ScanCodesToQt(TUint s60scanCode)
{
    int res = Qt::Key_unknown;
    for (int i = 0; keyMapping[i].s60KeyCode != 0; i++) {
        if (keyMapping[i].s60ScanCode == s60scanCode) {
            res = keyMapping[i].qtKey;
            break;
        }
    }
    return res;
}

int QKeyMapper::mapQtToS60Key(int qtKey)
{
    int res = KErrUnknown;
    for (int i = 0; keyMapping[i].s60KeyCode != 0; i++) {
        if (keyMapping[i].qtKey == qtKey) {
            res = keyMapping[i].s60KeyCode;
            break;
        }
    }
    return res;
}

int QKeyMapper::mapQtToS60ScanCodes(int qtKey)
{
    int res = KErrUnknown;
    for (int i = 0; keyMapping[i].s60KeyCode != 0; i++) {
        if (keyMapping[i].qtKey == qtKey) {
            res = keyMapping[i].s60ScanCode;
            break;
        }
    }
    return res;
}
