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

#ifndef KEYMAPPER_H
#define KEYMAPPER_H

#include <QObject>
#include <qkeysequence.h>
#include <qlist.h>
#include <qlocale.h>
#include <qevent.h>
#include <qhash.h>

#include <e32keys.h>

class QKeyEvent;


struct KeyMapping{
    TKeyCode s60KeyCode;
    TStdScanCode s60ScanCode;
    Qt::Key qtKey;
};

class QKeyMapper
{
public:
    QKeyMapper();
    ~QKeyMapper();

#if defined(Q_OS_SYMBIAN)
public:
    int mapS60KeyToQt(TUint s60key);
    int mapS60ScanCodesToQt(TUint s60key);
    int mapQtToS60Key(int qtKey);
    int mapQtToS60ScanCodes(int qtKey);
#endif
};

#endif // KEYMAPPER_H
